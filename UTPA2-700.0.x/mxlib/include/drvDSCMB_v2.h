#ifndef __DRV_DSCMB_V2_H__
#define __DRV_DSCMB_V2_H__

#include "drvDSCMB.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DSCMB_IV_LENGTH 16
#define DSCMB_KEY_LENGTH 16
#define DSCMB_HDCP2_RIV_LENGTH 8
#define DSCMB_MULTI2_SYSKEY_LENGTH 32

typedef enum {
    E_DSCMB_ENG0,
    E_DSCMB_ENG1,
    E_DSCMB_ENG2,
    E_DSCMB_UNKNOWN,
} eDscmbEngID;

typedef enum {
    /* Init and exit */
    E_MDRV_CMD_DSCMB_Init,
    E_MDRV_CMD_DSCMB_Exit,
    E_MDRV_CMD_DSCMB_SetPowerState,

    E_MDRV_CMD_DSCMB_FltAlloc,
    E_MDRV_CMD_DSCMB_FltFree,
    E_MDRV_CMD_DSCMB_FltConnectFltId,
    E_MDRV_CMD_DSCMB_FltConnectPid,
    E_MDRV_CMD_DSCMB_FltDisconnectFltId,
    E_MDRV_CMD_DSCMB_FltDisconnectPid,

    /* basic configuration*/
    E_MDRV_CMD_DSCMB_FltDscmb,
    E_MDRV_CMD_DSCMB_FltTypeSet,
    E_MDRV_CMD_DSCMB_FltKeySet,
    E_MDRV_CMD_DSCMB_FltKeyReset,
    E_MDRV_CMD_DSCMB_FltIVSet,

    E_MDRV_CMD_DSCMB_HDCP2_SetRiv,
    E_MDRV_CMD_DSCMB_Multi2_SetRound,
    E_MDRV_CMD_DSCMB_Multi2_SetSysKey,
    E_MDRV_CMD_DSCMB_ES_SetStaticKey,
    E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus,
    E_MDRV_CMD_DSCMB_PVR_RecCtrl,

    /* advanced configuration*/
    E_MDRV_CMD_DSCMB_SetDefaultCAVid,
    E_MDRV_CMD_DSCMB_EngSetAlgo,
    E_MDRV_CMD_DSCMB_EngSetKeyFSCB,
    E_MDRV_CMD_DSCMB_EngSetKey,
    E_MDRV_CMD_DSCMB_EngResetKey,
    E_MDRV_CMD_DSCMB_EngSetIV,
    E_MDRV_CMD_DSCMB_EngSetSwitch,
    E_MDRV_CMD_DSCMB_EngSetFSCB,

    /*Key ladder*/
    E_MDRV_CMD_DSCMB_KLadder_AtomicExec,
    E_MDRV_CMD_DSCMB_KLadder_ETSI,

    /*System info*/
    E_MDRV_CMD_DSCMB_QueryCap,
    E_MDRV_CMD_DSCMB_SetDBGLevel,
    E_MDRV_CMD_DSCMB_GetLibVer,
    E_MDRV_CMD_DSCMB_GetConnectStatus,
    E_MDRV_CMD_DSCMB_OptConfig,
    
    E_MDRV_CMD_DSCMB_HDCP2_SetRiv_Ex,

    /* PayLoad Encryption */
    E_MDRV_CMD_DSCMB_CAPVR_FlowSet,
    E_MDRV_CMD_DSCMB_DualPath_Enable,
    E_MDRV_CMD_DSCMB_DualPath_Disable,

    E_MDRV_CMD_DSCMB_EngSetIV_Ex,

    E_MDRV_CMD_DSCMB_MAX,
} eDscmbIoctlOpt;

typedef struct DLL_PACKED _DSCMB_QUERYCAP
{
    MS_U32              u32EngId;
    DSCMB_Query_Type    eQueryType;
    void*               pInput;
    void*               pOutput;
} DSCMB_QUERYCAP;

typedef struct DLL_PACKED _DSCMB_OPTCONFIG
{
    MS_U32              u32EngId;
    DSCMB_Option        eOpt;
    void*               pInput;
    void*               pOutput;
    MS_BOOL             bSet;
} DSCMB_OPTCONFIG;

typedef struct DLL_PACKED _DSCMB_FLTALLOC
{
    MS_U32          u32EngId;
    DSCMB_Flt_Type  eFltType;
    MS_U32          u32DscmbId;
} DSCMB_FLTALLOC;

typedef struct DLL_PACKED _DSCMB_FLTFREE
{
    MS_U32          u32EngId;
    MS_U32          u32DscmbId;
} DSCMB_FLTFREE;

typedef struct DLL_PACKED _DSCMB_CONNECTFLT
{
    MS_U32      u32EngId;
    MS_U32      u32DscmbId;
    MS_U32      u32DmxFltId;
} DSCMB_CONNECTFLT, DSCMB_DISCONNECTFLT, DSCMB_CONNECTSTATUS;

typedef struct DLL_PACKED _DSCMB_CONNECTPID
{
    MS_U32      u32EngId;
    MS_U32      u32DscmbId;
    MS_U32      u32Pid;
} DSCMB_CONNECTPID, DSCMB_DISCONNECTPID;

typedef struct DLL_PACKED _DSCMB_FLTDSCMB
{
    MS_U32      u32EngId;
    MS_U32      u32DscmbId;
    MS_BOOL     bDscmb;
} DSCMB_FLTDSCMB;

typedef struct DLL_PACKED _DSCMB_FLTIVSET
{
    MS_U32          u32EngId;
    MS_U32          u32DscmbId;
    DSCMB_Key_Type  eKeyType;
    union {
        MS_U8*      pu8IV;
        MS_U8*      pu8Key;
    };
} DSCMB_FLTIVSET, DSCMB_FLTKEYSET, DSCMB_ENGIV;

typedef struct DLL_PACKED _DSCMB_FLTKEYRESET
{
    MS_U32          u32EngId;
    MS_U32          u32DscmbId;
    DSCMB_Key_Type  eKeyType;
} DSCMB_FLTKEYRESET;

typedef struct DLL_PACKED _DSCMB_TYPESET
{
    MS_U32          u32EngId;
    MS_U32          u32DscmbId;
    DSCMB_Type      eType;
} DSCMB_TYPESET;

typedef struct DLL_PACKED _DSCMB_HDCP2_SETRIV
{
    MS_U32      u32EngId;
    MS_U8*      pu8Riv;
} DSCMB_HDCP2_SETRIV;

typedef struct DLL_PACKED _DSCMB_HDCP2_SETRIV_EX
{
    MS_U32      u32EngId;
    MS_U32      u32DscmbId;
    MS_U8*      pu8Riv;
} DSCMB_HDCP2_SETRIV_EX;

typedef struct DLL_PACKED _DSCMB_MULTI2_SETSYSKEY
{
    MS_U32      u32EngId;
    MS_U8*      pu8SysKey;
} DSCMB_MULTI2_SETSYSKEY;

typedef struct DLL_PACKED _DSCMB_MULTI2_SETROUND
{
    MS_U32      u32EngId;
    MS_U32      u32Round;
} DSCMB_MULTI2_SETROUND;

typedef struct DLL_PACKED _DSCMB_ES_SETSTATICKEY
{
    MS_U32      u32EngId;
    MS_U8*      pu8StaticKey;
} DSCMB_ES_SETSTATICKEY;

typedef struct DLL_PACKED _DSCMB_SCMBSTATUS
{
    MS_U32      u32EngId;
    MS_U32      u32DmxFltId;
    SCMB_Level* pScmbLevel;
} DSCMB_DSCMBSTATUS;

typedef struct DLL_PACKED _DSCMB_PVR_RECCTRL
{
    MS_U32      u32EngId;
    MS_BOOL     bEnable;
} DSCMB_PVR_RECCTRL;

typedef struct DLL_PACKED _DSCMB_DEFAULT_CAVID
{
    MS_U32        u32EngId;
    MS_U32        u32CAVid;
} DSCMB_DEFAULT_CAVID;

typedef struct DLL_PACKED _DSCMB_ENGFSCB
{
    MS_U32        u32EngId;
    MS_U32        u32DscmbId;
    DSCMB_FSCB    eForceSCB;
} DSCMB_ENGFSCB;

typedef struct DLL_PACKED _DSCMB_ENGSWITCH
{
    MS_U32         u32EngId;
    MS_U32         u32DscmbId;
    DSCMB_Eng_Type eUppSwitch;
    DSCMB_Eng_Type eLowSwitch;
} DSCMB_ENGSWITCH;

typedef struct DLL_PACKED _DSCMB_ENGALGO
{
    MS_U32         u32EngId;
    MS_U32         u32DscmbId;
    DSCMB_Eng_Type eEngType;
    DSCMB_Algo_Cfg stConfig;
} DSCMB_ENGALGO;

typedef struct DLL_PACKED _DSCMB_ENGKEYFSCB
{
    MS_U32         u32EngId;
    MS_U32         u32DscmbId;
    DSCMB_Eng_Type eEngType;
    DSCMB_Key_Type eKeyType;
    DSCMB_FSCB     eForceSCB;
} DSCMB_ENGKEYFSCB;

typedef struct DLL_PACKED _DSCMB_ENGKEY
{
    MS_U32         u32EngId;
    MS_U32         u32DscmbId;
    DSCMB_Eng_Type eEngType;
    DSCMB_Key_Type eKeyType;
    union {
        MS_U8*      pu8IV;
    MS_U8*         pu8Key;
    };
} DSCMB_ENGKEY, DSCMB_ENGIV_EX;

typedef struct DLL_PACKED _DSCMB_ENGRESETKEY
{
    MS_U32         u32EngId;
    MS_U32         u32DscmbId;
    DSCMB_Eng_Type eEngType;
    DSCMB_Key_Type eKeyType;
} DSCMB_ENGRESETKEY;

typedef struct DLL_PACKED _DSCMB_CAPVR_FLOWSET
{
    MS_U32           u32EngId;
    DSCMB_CAPVR_MODE eCaMode;
    DSCMB_TSIF       ePvrSrcTsif;
} DSCMB_CAPVR_FLOWSET;

typedef struct DLL_PACKED _DSCMB_DUALPATH
{
    MS_U32        u32EngId;
    MS_U32        u32DmxFltId;
} DSCMB_DUALPATH;

typedef struct DLL_PACKED _DSCMB_KL_ATOMICEXEC
{
    DSCMB_KLCfg_All*   KLCfg;
    MS_U8*             ACPU_Out;
    DSCMB_KL_Status*   u32Status;
} DSCMB_KL_ATOMICEXEC;

typedef struct DLL_PACKED _DSCMB_KL_ETSI
{
    DSCMB_KLCfg_All* KLCfg;
    MS_U8*           ACPU_Out;
    MS_U8*           pu8Nonce;
    MS_U8*           pu8Response;
    DSCMB_KL_Status* u32Status;
} DSCMB_KL_ETSI;

MS_U32 DSCMBOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DSCMBClose(void* pInstantTmp);
MS_U32 DSCMBIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pu32Args);


#ifdef __cplusplus
}
#endif

#endif // __DRV_DSCMB_V2_H__
