#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvDSCMB.h"
#include "drvDSCMB_v2.h"
#include "DSCMB_adp.h"

#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/compat.h>

#include "utopia_adp.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                                    if(copy_from_user(a,b,c) != 0) { return UTOPIA_STATUS_FAIL; }
#define CPY_to_USER(a,b,c)                                      if(copy_to_user(a,b,c) != 0) { return UTOPIA_STATUS_FAIL; }
#else
#define CPY_FROM_USER                                           memcpy
#define CPY_to_USER                                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                                           compat_ptr(a)
#define IS_CMP_TASK()                                           is_compat_task()
#define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  return UTOPIA_STATUS_FAIL; }
#define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { return UTOPIA_STATUS_FAIL;  }

#else
#define COMPAT_PTR(a)                                           (a)
#define IS_CMP_TASK()                                           (FALSE)
#define CMP_CPY_FROM_USER                                       CPY_FROM_USER
#define CMP_CPY_TO_USER                                         CPY_to_USER
#endif //CONFIG_COMPAT

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
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltAlloc[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltFree[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltConnectFltId[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltDisconnectFltId[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltConnectPid[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltDisconnectPid[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltDscmb[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltIVSet[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltKeySet[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltKeyReset[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_FltTypeSet[SPT_TYPE_A_SIZE];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngSetAlgo[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngSetKey[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngResetKey[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngSetKeyFSCB[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngSetIV[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngSetIV_Ex[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngSetSwitch[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_EngSetFSCB[SPT_TYPE_A_SIZE];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_SetDefaultCAVid[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_HDCP2_SetRiv[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_HDCP2_SetRiv_Ex[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_Multi2_SetRound[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_Multi2_SetSysKey[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_CAPVR_FlowSet[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_DualPath_Enable[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_DualPath_Disable[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_SetPowerState[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_QueryCap[SPT_TYPE_C_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_SetDBGLevel[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_GetConnectStatus[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_PVR_RecCtrl[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_PidFlt_ScmbStatus[SPT_TYPE_B_SIZE];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_AtomicExec[SPT_TYPE_D_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_AtomicExec_Lv2[SPT_TYPE_D_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_AtomicExec_Lv3[SPT_TYPE_D_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_AtomicExec_Lv4[SPT_TYPE_D_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_AtomicExec_Lv5[SPT_TYPE_D_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_ETSI[SPT_TYPE_F_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_ETSI_Lv2[SPT_TYPE_F_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_ETSI_Lv3[SPT_TYPE_F_SIZE];


//BOTTOM
UADP_STRUCT_POINTER_TABLE spt_DSCMB_PidFlt_ScmbStatus_pScmbLevel[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_QueryCap_InOut[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_KLCfg[SPT_TYPE_C_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_KLCfg_Lv2[SPT_TYPE_C_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_KLCfg_Lv3[SPT_TYPE_C_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_KLCfg_Lv4[SPT_TYPE_C_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_KLCfg_Lv5[SPT_TYPE_C_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_u32Status[SPT_TYPE_A_SIZE];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_ACPU_Out[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_u8KeyACPU[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_pu8KeyKLIn80[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_pu8KeyKLIn64[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_pu8KeyKLIn48[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_pu8KeyKLIn32[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_pu8KeyKLIn16[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_pu8Nonse[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_KLadder_pu8Response[SPT_TYPE_A_SIZE];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_pu8Buf80[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pu8Buf64[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pu8Buf48[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pu8Buf32[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pu8Buf16[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pu8Buf8[SPT_TYPE_A_SIZE];


MS_U32 DSCMB_adp_Init(FUtopiaIOctl* pIoctl)
{
    //TOP
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_SetPowerState, EN_POWER_MODE);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltAlloc, DSCMB_FLTALLOC);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltFree, DSCMB_FLTFREE);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltConnectFltId, DSCMB_CONNECTFLT);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltConnectPid, DSCMB_CONNECTPID);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltDisconnectFltId, DSCMB_DISCONNECTFLT);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltDisconnectPid, DSCMB_DISCONNECTPID);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltDscmb, DSCMB_FLTDSCMB);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltTypeSet, DSCMB_TYPESET);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_FltKeySet, DSCMB_FLTKEYSET, pu8Key, spt_DSCMB_pu8Buf16);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_FltKeyReset, DSCMB_FLTKEYRESET);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_FltIVSet, DSCMB_FLTIVSET, pu8IV, spt_DSCMB_pu8Buf16);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_HDCP2_SetRiv, DSCMB_HDCP2_SETRIV, pu8Riv, spt_DSCMB_pu8Buf8);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_HDCP2_SetRiv_Ex, DSCMB_HDCP2_SETRIV_EX, pu8Riv, spt_DSCMB_pu8Buf8);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_Multi2_SetRound, DSCMB_MULTI2_SETROUND);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_Multi2_SetSysKey, DSCMB_MULTI2_SETSYSKEY, pu8SysKey, spt_DSCMB_pu8Buf32);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_PidFlt_ScmbStatus, DSCMB_DSCMBSTATUS, pScmbLevel, spt_DSCMB_PidFlt_ScmbStatus_pScmbLevel);
    UADP_SPT_NAME2NXT_TYPE(spt_DSCMB_QueryCap, DSCMB_QUERYCAP, pInput, spt_DSCMB_QueryCap_InOut, pOutput, spt_DSCMB_QueryCap_InOut);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_PVR_RecCtrl, DSCMB_PVR_RECCTRL);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_SetDefaultCAVid, DSCMB_DEFAULT_CAVID);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_EngSetAlgo, DSCMB_ENGALGO);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_EngSetKeyFSCB, DSCMB_ENGKEYFSCB);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_EngSetKey, DSCMB_ENGKEY, pu8Key, spt_DSCMB_pu8Buf16);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_EngResetKey, DSCMB_ENGRESETKEY);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_EngSetIV, DSCMB_ENGIV, pu8IV, spt_DSCMB_pu8Buf16);
    UADP_SPT_NAME1NXT_TYPE(spt_DSCMB_EngSetIV_Ex, DSCMB_ENGIV_EX, pu8IV, spt_DSCMB_pu8Buf16);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_EngSetSwitch, DSCMB_ENGSWITCH);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_EngSetFSCB, DSCMB_ENGFSCB);
    UADP_SPT_NAME3NXT_TYPE(spt_DSCMB_KLadder_AtomicExec, DSCMB_KL_ATOMICEXEC,
                           KLCfg,     spt_DSCMB_KLadder_KLCfg,
                           ACPU_Out,  spt_DSCMB_KLadder_ACPU_Out,
                           u32Status, spt_DSCMB_KLadder_u32Status);
    UADP_SPT_NAME3NXT_TYPE(spt_DSCMB_KLadder_AtomicExec_Lv2, DSCMB_KL_ATOMICEXEC,
                           KLCfg,     spt_DSCMB_KLadder_KLCfg_Lv2,
                           ACPU_Out,  spt_DSCMB_KLadder_ACPU_Out,
                           u32Status, spt_DSCMB_KLadder_u32Status);
    UADP_SPT_NAME3NXT_TYPE(spt_DSCMB_KLadder_AtomicExec_Lv3, DSCMB_KL_ATOMICEXEC,
                           KLCfg,     spt_DSCMB_KLadder_KLCfg_Lv3,
                           ACPU_Out,  spt_DSCMB_KLadder_ACPU_Out,
                           u32Status, spt_DSCMB_KLadder_u32Status);
    UADP_SPT_NAME3NXT_TYPE(spt_DSCMB_KLadder_AtomicExec_Lv4, DSCMB_KL_ATOMICEXEC,
                           KLCfg,     spt_DSCMB_KLadder_KLCfg_Lv4,
                           ACPU_Out,  spt_DSCMB_KLadder_ACPU_Out,
                           u32Status, spt_DSCMB_KLadder_u32Status);
    UADP_SPT_NAME3NXT_TYPE(spt_DSCMB_KLadder_AtomicExec_Lv5, DSCMB_KL_ATOMICEXEC,
                           KLCfg,     spt_DSCMB_KLadder_KLCfg_Lv5,
                           ACPU_Out,  spt_DSCMB_KLadder_ACPU_Out,
                           u32Status, spt_DSCMB_KLadder_u32Status);

    UADP_SPT_NAME5NXT_TYPE(spt_DSCMB_KLadder_ETSI, DSCMB_KL_ETSI,
                           KLCfg,       spt_DSCMB_KLadder_KLCfg,
                           ACPU_Out,    spt_DSCMB_KLadder_ACPU_Out,
                           pu8Nonce,    spt_DSCMB_KLadder_pu8Nonse,
                           pu8Response, spt_DSCMB_KLadder_pu8Response,
                           u32Status,   spt_DSCMB_KLadder_u32Status);

    UADP_SPT_NAME5NXT_TYPE(spt_DSCMB_KLadder_ETSI_Lv2, DSCMB_KL_ETSI,
                           KLCfg,       spt_DSCMB_KLadder_KLCfg_Lv2,
                           ACPU_Out,    spt_DSCMB_KLadder_ACPU_Out,
                           pu8Nonce,    spt_DSCMB_KLadder_pu8Nonse,
                           pu8Response, spt_DSCMB_KLadder_pu8Response,
                           u32Status,   spt_DSCMB_KLadder_u32Status);

    UADP_SPT_NAME5NXT_TYPE(spt_DSCMB_KLadder_ETSI_Lv3, DSCMB_KL_ETSI,
                           KLCfg,       spt_DSCMB_KLadder_KLCfg_Lv3,
                           ACPU_Out,    spt_DSCMB_KLadder_ACPU_Out,
                           pu8Nonce,    spt_DSCMB_KLadder_pu8Nonse,
                           pu8Response, spt_DSCMB_KLadder_pu8Response,
                           u32Status,   spt_DSCMB_KLadder_u32Status);

    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_SetDBGLevel, MS_U32);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_GetConnectStatus, DSCMB_CONNECTSTATUS);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_CAPVR_FlowSet, DSCMB_CAPVR_FLOWSET);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_DualPath_Enable, DSCMB_DUALPATH);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_DualPath_Disable, DSCMB_DUALPATH);



    //BOTTOM
    UADP_SPT_NAME2NXT_TYPE(spt_DSCMB_KLadder_KLCfg, DSCMB_KLCfg_All,
                           u8KeyACPU,  spt_DSCMB_KLadder_u8KeyACPU,
                           pu8KeyKLIn, spt_DSCMB_KLadder_pu8KeyKLIn16);
    UADP_SPT_NAME2NXT_TYPE(spt_DSCMB_KLadder_KLCfg_Lv2, DSCMB_KLCfg_All,
                           u8KeyACPU,  spt_DSCMB_KLadder_u8KeyACPU,
                           pu8KeyKLIn, spt_DSCMB_KLadder_pu8KeyKLIn32);
    UADP_SPT_NAME2NXT_TYPE(spt_DSCMB_KLadder_KLCfg_Lv3, DSCMB_KLCfg_All,
                           u8KeyACPU,  spt_DSCMB_KLadder_u8KeyACPU,
                           pu8KeyKLIn, spt_DSCMB_KLadder_pu8KeyKLIn48);
    UADP_SPT_NAME2NXT_TYPE(spt_DSCMB_KLadder_KLCfg_Lv4, DSCMB_KLCfg_All,
                           u8KeyACPU,  spt_DSCMB_KLadder_u8KeyACPU,
                           pu8KeyKLIn, spt_DSCMB_KLadder_pu8KeyKLIn64);
    UADP_SPT_NAME2NXT_TYPE(spt_DSCMB_KLadder_KLCfg_Lv5, DSCMB_KLCfg_All,
                           u8KeyACPU,  spt_DSCMB_KLadder_u8KeyACPU,
                           pu8KeyKLIn, spt_DSCMB_KLadder_pu8KeyKLIn80);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_KLadder_u32Status, DSCMB_KL_Status);

    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_KLadder_ACPU_Out, sizeof(MS_U8)*16);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_KLadder_u8KeyACPU, sizeof(MS_U8)*16);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_KLadder_pu8KeyKLIn16, sizeof(MS_U8)*16);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_KLadder_pu8KeyKLIn32, sizeof(MS_U8)*32);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_KLadder_pu8KeyKLIn48, sizeof(MS_U8)*48);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_KLadder_pu8KeyKLIn64, sizeof(MS_U8)*64);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_KLadder_pu8KeyKLIn80, sizeof(MS_U8)*80);

    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_PidFlt_ScmbStatus_pScmbLevel, SCMB_Level);
    UADP_SPT_NAME0NXT_TYPE(spt_DSCMB_QueryCap_InOut, MS_U32);

    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_pu8Buf80, sizeof(MS_U8)*80);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_pu8Buf64, sizeof(MS_U8)*64);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_pu8Buf48, sizeof(MS_U8)*48);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_pu8Buf32, sizeof(MS_U8)*32);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_pu8Buf16, sizeof(MS_U8)*16);
    UADP_SPT_NAME0NXT_SIZE(spt_DSCMB_pu8Buf8, sizeof(MS_U8)*8);

    *pIoctl = (FUtopiaIOctl)DSCMB_adp_Ioctl;

    return 0;

}


MS_U32 DSCMB_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[128];
    char buffer_array0[128];
    UADP_STRUCT_POINTER_TABLE* sptPtr=NULL;
    DSCMB_KL_ATOMICEXEC* pKLAtomicArgs=NULL;
    DSCMB_KL_ETSI* pKLETSIArgs=NULL;
    DSCMB_KLCfg_All*   KLCfg=NULL;

    switch(u32Cmd)
    {
        case E_MDRV_CMD_DSCMB_Init:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDRV_CMD_DSCMB_Exit:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDRV_CMD_DSCMB_SetPowerState:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_SetPowerState, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltAlloc:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltAlloc, spt_DSCMB_FltAlloc, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltFree:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltFree, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltConnectFltId:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltConnectFltId, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltConnectPid:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltConnectPid, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltDisconnectFltId:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltDisconnectFltId, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltDisconnectPid:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltDisconnectPid, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltDscmb:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltDscmb, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltTypeSet:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltTypeSet, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltKeySet:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltKeySet, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltKeyReset:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltKeyReset, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_FltIVSet:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_FltIVSet, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_HDCP2_SetRiv:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_HDCP2_SetRiv, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_Multi2_SetRound:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_Multi2_SetRound, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_Multi2_SetSysKey:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_Multi2_SetSysKey, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_PidFlt_ScmbStatus, spt_DSCMB_PidFlt_ScmbStatus, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_PVR_RecCtrl:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_PVR_RecCtrl, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_SetDefaultCAVid:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_SetDefaultCAVid, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngSetAlgo:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngSetAlgo, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngSetKeyFSCB:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngSetKeyFSCB, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngSetKey:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngSetKey, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngResetKey:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngResetKey, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngSetIV:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngSetIV, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngSetSwitch:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngSetSwitch, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngSetFSCB:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngSetFSCB, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_KLadder_AtomicExec:
            pKLAtomicArgs = (DSCMB_KL_ATOMICEXEC*)buffer_arg;

            if (IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DSCMB_KL_ATOMICEXEC));
                CMP_CPY_FROM_USER((void*)buffer_array0, (void*)pKLAtomicArgs->KLCfg, sizeof(DSCMB_KLCfg_All));
                pKLAtomicArgs->KLCfg = (DSCMB_KLCfg_All*)buffer_array0;
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DSCMB_KL_ATOMICEXEC));
                CPY_FROM_USER((void*)buffer_array0, (void*)pKLAtomicArgs->KLCfg, sizeof(DSCMB_KLCfg_All));
                pKLAtomicArgs->KLCfg = (DSCMB_KLCfg_All*)buffer_array0;
            }

            sptPtr  = spt_DSCMB_KLadder_AtomicExec;
            if(pKLAtomicArgs->KLCfg->u32Level == 1)
            {
                sptPtr = spt_DSCMB_KLadder_AtomicExec;
            }
            else if(pKLAtomicArgs->KLCfg->u32Level == 2)
            {
                sptPtr = spt_DSCMB_KLadder_AtomicExec_Lv2;
            }
            else if(pKLAtomicArgs->KLCfg->u32Level == 3)
            {
                sptPtr = spt_DSCMB_KLadder_AtomicExec_Lv3;
            }
            else if(pKLAtomicArgs->KLCfg->u32Level == 4)
            {
                sptPtr = spt_DSCMB_KLadder_AtomicExec_Lv4;
            }
            else if(pKLAtomicArgs->KLCfg->u32Level == 5)
            {
                sptPtr = spt_DSCMB_KLadder_AtomicExec_Lv5;
            }

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, sptPtr, sptPtr, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_KLadder_ETSI:
            pKLETSIArgs = (DSCMB_KL_ETSI*)buffer_arg;

            if (IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DSCMB_KL_ETSI));
                CMP_CPY_FROM_USER((void*)buffer_array0, (void*)pKLETSIArgs->KLCfg, sizeof(DSCMB_KLCfg_All));
                pKLETSIArgs->KLCfg = (DSCMB_KLCfg_All*)buffer_array0;
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DSCMB_KL_ETSI));
                CPY_FROM_USER((void*)buffer_array0, (void*)pKLETSIArgs->KLCfg, sizeof(DSCMB_KLCfg_All));
                pKLETSIArgs->KLCfg = (DSCMB_KLCfg_All*)buffer_array0;
            }
            sptPtr  = spt_DSCMB_KLadder_ETSI;

            if(pKLETSIArgs->KLCfg->u32Level == 1)
            {
                sptPtr = spt_DSCMB_KLadder_ETSI;
            }
            else if(pKLETSIArgs->KLCfg->u32Level == 2)
            {
                sptPtr = spt_DSCMB_KLadder_ETSI_Lv2;
            }
            else if(pKLETSIArgs->KLCfg->u32Level == 3)
            {
                sptPtr = spt_DSCMB_KLadder_ETSI_Lv3;
            }

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, sptPtr, sptPtr, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_QueryCap:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_QueryCap, spt_DSCMB_QueryCap, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_SetDBGLevel:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_SetDBGLevel, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        //case E_MDRV_CMD_DSCMB_GetLibVer:
        //    u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, , NULL,buffer_arg,sizeof(buffer_arg));
        //    break;
        case E_MDRV_CMD_DSCMB_GetConnectStatus:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_GetConnectStatus, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_HDCP2_SetRiv_Ex:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_HDCP2_SetRiv_Ex, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_CAPVR_FlowSet:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_CAPVR_FlowSet, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_DualPath_Enable:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_DualPath_Enable, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_DualPath_Disable:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_DualPath_Disable, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_DSCMB_EngSetIV_Ex:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DSCMB_EngSetIV_Ex, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        default:
            break;
    };

    return u32Ret;
}
