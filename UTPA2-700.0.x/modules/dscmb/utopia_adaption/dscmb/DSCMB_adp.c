#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvDSCMB.h"
#include "drvDSCMB_v2.h"
#include "DSCMB_adp.h"


#include "utopia_adp.h"

//TOP
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pPowerState[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psQuery[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psOptConfig[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pFltAlloc[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pConnectFlt[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDisconnectFlt[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstConnectPid[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstDisconnectPid[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstFltDscmb[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pFltFree[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstFltIVSet[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstFltKeyReset[10];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstFltKeySet[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstTypeSet[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstSetRiv[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstSetRnd[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psSetSysKey[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbStatus[10];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbPvrRecCtrl[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbSetDefaultCaVid[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbEngSetAlgo[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbEngSetKey[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbEngResetKey[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbEngSetIv[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbEngSetSwitch[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbEngSetFSCB[10];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbKLaddrAtomicExec[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbKLaddrETSI[10];


UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbSetDbgLevel[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbSetConnectStatus[10];



//BOTTOM
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psQuery_InOut[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psOptConfig_InOut[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstSetRiv_Iv[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psSetSysKey_Key[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psEngSetKey_Key[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psEngSetIV_Iv[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pDscmbStatus_Status[10];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_psKLAtomicExec_KLCFG[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psKLAtomicExec_KLCFG_KeyACPU[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psKLAtomicExec_KLCFG_KeyKLIn[10];


UADP_STRUCT_POINTER_TABLE spt_DSCMB_psKLAtomicExec_ACPUOut[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psKLETSI_Nonce[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psKLETSI_Response[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_psKL_Status[10];

UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstFltKeySet_Key[10];
UADP_STRUCT_POINTER_TABLE spt_DSCMB_pstFltIVSet_Iv[10];




MS_U32 DSCMB_adp_Init(FUtopiaIOctl* pIoctl)
{

//set table
    DSCMB_QUERYCAP* pDSCMB_psQuery;
    DSCMB_OPTCONFIG* pDSCMB_psOptConfig;
    DSCMB_HDCP2_SETRIV* pDSCMB_pstSetRiv;
    DSCMB_MULTI2_SETSYSKEY* pDSCMB_psSetSysKey;
    DSCMB_DSCMBSTATUS* pDSCMB_psStatus;
    DSCMB_ENGKEY* pDSCMB_psEngSetKey;
    DSCMB_ENGIV* pDSCMB_psEngSetIv;

    DSCMB_KL_ATOMICEXEC* pDSCMB_psKlAtomicExec;
    DSCMB_KL_ETSI* pDSCMB_psKlETSI;
    DSCMB_KLCfg_All* pDSCMB_psSetSysKey_KCLfg_All;
    DSCMB_FLTIVSET* pDSCMB_pstFltIVSet;
    DSCMB_FLTKEYSET* pDSCMB_pstFltKeySet;    
#if 0
    //DSCMB
    DSCMB_FLTALLOC* pDSCMB_pFltAlloc = (DSCMB_FLTALLOC *)temp;
    DSCMB_CONNECTFLT* pDSCMB_pConnectFlt = (DSCMB_CONNECTFLT *)temp;
    DSCMB_DISCONNECTFLT* pDSCMB_pDisconnectFlt = (DSCMB_DISCONNECTFLT *)temp;
    DSCMB_CONNECTPID* pDSCMB_pstConnectPid = (DSCMB_CONNECTPID *)temp;
    DSCMB_DISCONNECTPID* pDSCMB_pstDisconnectPid = (DSCMB_DISCONNECTPID *)temp;
    DSCMB_FLTDSCMB* pDSCMB_pstFltDscmb = (DSCMB_FLTDSCMB *)temp;
    DSCMB_FLTFREE* pDSCMB_pFltFree = (DSCMB_FLTFREE *)temp;
    DSCMB_FLTIVSET* pDSCMB_pstFltIVSet = (DSCMB_FLTIVSET *)temp;
    DSCMB_FLTKEYRESET* pDSCMB_pstFltKeyReset = (DSCMB_FLTKEYRESET *)temp;
    DSCMB_FLTKEYSET* pDSCMB_pstFltKeySet = (DSCMB_FLTKEYSET *)temp;
    DSCMB_TYPESET* pDSCMB_pstTypeSet = (DSCMB_TYPESET *)temp;
    DSCMB_MULTI2_SETROUND* pDSCMB_pstSetRnd = (DSCMB_MULTI2_SETROUND *)temp;
    DSCMB_DSCMBSTATUS* pDSCMB_pDscmbStatus = (DSCMB_DSCMBSTATUS *)temp;
#endif

    //TOP

    UADPBypassSetSPT(&spt_DSCMB_pPowerState[0],UADP_SPT_SELF_SIZE,0, sizeof(EN_POWER_MODE));
    UADPBypassSetSPT(&spt_DSCMB_pPowerState[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pFltAlloc[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_FLTALLOC));
    UADPBypassSetSPT(&spt_DSCMB_pFltAlloc[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pFltFree[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_FLTFREE));
    UADPBypassSetSPT(&spt_DSCMB_pFltFree[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pConnectFlt[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_CONNECTFLT));
    UADPBypassSetSPT(&spt_DSCMB_pConnectFlt[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstConnectPid[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_CONNECTPID));
    UADPBypassSetSPT(&spt_DSCMB_pstConnectPid[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDisconnectFlt[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_DISCONNECTFLT));
    UADPBypassSetSPT(&spt_DSCMB_pDisconnectFlt[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstDisconnectPid[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_DISCONNECTPID));
    UADPBypassSetSPT(&spt_DSCMB_pstDisconnectPid[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstFltDscmb[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_FLTDSCMB));
    UADPBypassSetSPT(&spt_DSCMB_pstFltDscmb[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstTypeSet[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_TYPESET));
    UADPBypassSetSPT(&spt_DSCMB_pstTypeSet[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstFltKeySet[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_FLTKEYSET));
    UADPBypassSetSPT(&spt_DSCMB_pstFltKeySet[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_pstFltKeySet->pu8Key)) - (unsigned long)pDSCMB_pstFltKeySet), (unsigned long)spt_DSCMB_pstFltKeySet_Key);
    UADPBypassSetSPT(&spt_DSCMB_pstFltKeySet[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstFltKeyReset[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_FLTKEYRESET));
    UADPBypassSetSPT(&spt_DSCMB_pstFltKeyReset[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstFltIVSet[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_FLTIVSET));
    UADPBypassSetSPT(&spt_DSCMB_pstFltIVSet[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_pstFltIVSet->pu8IV)) - (unsigned long)pDSCMB_pstFltIVSet), (unsigned long)spt_DSCMB_pstFltIVSet_Iv);
    UADPBypassSetSPT(&spt_DSCMB_pstFltIVSet[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstSetRiv[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_HDCP2_SETRIV));
    UADPBypassSetSPT(&spt_DSCMB_pstSetRiv[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_pstSetRiv->pu8Riv)) - (unsigned long)pDSCMB_pstSetRiv), (unsigned long)spt_DSCMB_pstSetRiv_Iv);
    UADPBypassSetSPT(&spt_DSCMB_pstSetRiv[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstSetRnd[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_MULTI2_SETROUND));
    UADPBypassSetSPT(&spt_DSCMB_pstSetRnd[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psSetSysKey[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_MULTI2_SETSYSKEY));
    UADPBypassSetSPT(&spt_DSCMB_psSetSysKey[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psSetSysKey->pu8SysKey)) - (unsigned long)pDSCMB_psSetSysKey), (unsigned long)spt_DSCMB_psSetSysKey_Key);
    UADPBypassSetSPT(&spt_DSCMB_psSetSysKey[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbStatus[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_DSCMBSTATUS)); 
    UADPBypassSetSPT(&spt_DSCMB_pDscmbStatus[1],UADP_SPT_POINTER_TO_NEXT,((unsigned long)(&(pDSCMB_psStatus->pScmbLevel)) - (unsigned long)pDSCMB_psStatus), (unsigned long)spt_DSCMB_pDscmbStatus_Status);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbStatus[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psQuery[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_QUERYCAP));
    UADPBypassSetSPT(&spt_DSCMB_psQuery[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psQuery->pInput)) - (unsigned long)pDSCMB_psQuery), (unsigned long)spt_DSCMB_psQuery_InOut);
    UADPBypassSetSPT(&spt_DSCMB_psQuery[2],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psQuery->pOutput)) - (unsigned long)pDSCMB_psQuery), (unsigned long)spt_DSCMB_psQuery_InOut);
    UADPBypassSetSPT(&spt_DSCMB_psQuery[3],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psOptConfig[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_OPTCONFIG));
    UADPBypassSetSPT(&spt_DSCMB_psOptConfig[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psOptConfig->pInput)) - (unsigned long)pDSCMB_psOptConfig), (unsigned long)spt_DSCMB_psOptConfig_InOut);
    UADPBypassSetSPT(&spt_DSCMB_psOptConfig[2],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psOptConfig->pOutput)) - (unsigned long)pDSCMB_psOptConfig), (unsigned long)spt_DSCMB_psOptConfig_InOut);
    UADPBypassSetSPT(&spt_DSCMB_psOptConfig[3],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbPvrRecCtrl[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_PVR_RECCTRL));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbPvrRecCtrl[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbSetDefaultCaVid[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_DEFAULT_CAVID));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbSetDefaultCaVid[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetAlgo[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_ENGALGO));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetAlgo[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetKey[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_ENGKEY));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetKey[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psEngSetKey->pu8Key)) - (unsigned long)pDSCMB_psEngSetKey), (unsigned long)spt_DSCMB_psEngSetKey_Key);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetKey[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngResetKey[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_ENGRESETKEY));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngResetKey[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetIv[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_ENGIV));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetIv[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psEngSetIv->pu8IV)) - (unsigned long)pDSCMB_psEngSetIv), (unsigned long)spt_DSCMB_psEngSetIV_Iv);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetIv[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetSwitch[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_ENGSWITCH));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetSwitch[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetFSCB[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_ENGFSCB));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbEngSetFSCB[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrAtomicExec[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_KL_ATOMICEXEC));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrAtomicExec[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlAtomicExec->KLCfg)) - (unsigned long)pDSCMB_psKlAtomicExec), (unsigned long)spt_DSCMB_psKLAtomicExec_KLCFG);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrAtomicExec[2],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlAtomicExec->ACPU_Out)) - (unsigned long)pDSCMB_psKlAtomicExec), (unsigned long)spt_DSCMB_psKLAtomicExec_ACPUOut);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrAtomicExec[3],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlAtomicExec->u32Status)) - (unsigned long)pDSCMB_psKlAtomicExec), (unsigned long)spt_DSCMB_psKL_Status);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrAtomicExec[4],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrETSI[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_KL_ETSI));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrETSI[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlETSI->KLCfg)) - (unsigned long)pDSCMB_psKlETSI), (unsigned long)spt_DSCMB_psKLAtomicExec_KLCFG);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrETSI[2],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlETSI->ACPU_Out)) - (unsigned long)pDSCMB_psKlETSI), (unsigned long)spt_DSCMB_psKLAtomicExec_ACPUOut);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrETSI[3],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlETSI->pu8Nonce)) - (unsigned long)pDSCMB_psKlETSI), (unsigned long)spt_DSCMB_psKLETSI_Nonce);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrETSI[4],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlETSI->pu8Response)) - (unsigned long)pDSCMB_psKlETSI), (unsigned long)spt_DSCMB_psKLETSI_Response);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrETSI[5],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psKlETSI->u32Status)) - (unsigned long)pDSCMB_psKlETSI), (unsigned long)spt_DSCMB_psKL_Status);
    UADPBypassSetSPT(&spt_DSCMB_pDscmbKLaddrETSI[6],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_DSCMB_pDscmbSetDbgLevel[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U32));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbSetDbgLevel[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbSetConnectStatus[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_CONNECTSTATUS));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbSetConnectStatus[1],UADP_SPT_END , 0, 0);


//BOTTOM

    UADPBypassSetSPT(&spt_DSCMB_pstFltKeySet_Key[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_KEY_LENGTH));
    UADPBypassSetSPT(&spt_DSCMB_pstFltKeySet_Key[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstFltIVSet_Iv[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_IV_LENGTH));
    UADPBypassSetSPT(&spt_DSCMB_pstFltIVSet_Iv[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psQuery_InOut[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U32));
    UADPBypassSetSPT(&spt_DSCMB_psQuery_InOut[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psOptConfig_InOut[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U32));
    UADPBypassSetSPT(&spt_DSCMB_psOptConfig_InOut[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pstSetRiv_Iv[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_IV_LENGTH));
    UADPBypassSetSPT(&spt_DSCMB_pstSetRiv_Iv[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psSetSysKey_Key[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_MULTI2_SYSKEY_LENGTH));
    UADPBypassSetSPT(&spt_DSCMB_psSetSysKey_Key[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_pDscmbStatus_Status[0],UADP_SPT_SELF_SIZE,0, sizeof(SCMB_Level));
    UADPBypassSetSPT(&spt_DSCMB_pDscmbStatus_Status[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psEngSetKey_Key[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_KEY_LENGTH));
    UADPBypassSetSPT(&spt_DSCMB_psEngSetKey_Key[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psEngSetIV_Iv[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_IV_LENGTH));
    UADPBypassSetSPT(&spt_DSCMB_psEngSetIV_Iv[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_KLCfg_All));
    UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psSetSysKey_KCLfg_All->u8KeyACPU)) - (unsigned long)pDSCMB_psSetSysKey_KCLfg_All), (unsigned long)spt_DSCMB_psKLAtomicExec_KLCFG_KeyACPU);
    UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG[2],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDSCMB_psSetSysKey_KCLfg_All->pu8KeyKLIn)) - (unsigned long)pDSCMB_psSetSysKey_KCLfg_All), (unsigned long)spt_DSCMB_psKLAtomicExec_KLCFG_KeyKLIn);
    UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG[3],UADP_SPT_END , 0, 0);
        UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG_KeyACPU[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_KEY_LENGTH));
        UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG_KeyACPU[1],UADP_SPT_END , 0, 0);

        UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG_KeyKLIn[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*DSCMB_KEY_LENGTH));
        UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_KLCFG_KeyKLIn[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_ACPUOut[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*16));
    UADPBypassSetSPT(&spt_DSCMB_psKLAtomicExec_ACPUOut[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psKLETSI_Nonce[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*16));
    UADPBypassSetSPT(&spt_DSCMB_psKLETSI_Nonce[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psKLETSI_Response[0],UADP_SPT_SELF_SIZE,0, (sizeof(MS_U8)*16));
    UADPBypassSetSPT(&spt_DSCMB_psKLETSI_Response[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DSCMB_psKL_Status[0],UADP_SPT_SELF_SIZE,0, sizeof(DSCMB_KL_Status));
    UADPBypassSetSPT(&spt_DSCMB_psKL_Status[1],UADP_SPT_END , 0, 0);

    *pIoctl= (FUtopiaIOctl)DSCMB_adp_Ioctl;

    return 0;

}


MS_U32 DSCMB_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[32];
    switch(u32Cmd)
        {
            case E_MDRV_CMD_DSCMB_Init:
                u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
                break;
            case E_MDRV_CMD_DSCMB_Exit:
                u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
                break;
            case E_MDRV_CMD_DSCMB_SetPowerState:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pPowerState, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltAlloc:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pFltAlloc, spt_DSCMB_pFltAlloc,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltFree:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pFltFree, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltConnectFltId:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pConnectFlt, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltConnectPid:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstConnectPid, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltDisconnectFltId:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDisconnectFlt, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltDisconnectPid:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstDisconnectPid, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltDscmb:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstFltDscmb, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltTypeSet:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstTypeSet, spt_DSCMB_pstTypeSet,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltKeySet:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstFltKeySet, spt_DSCMB_pstFltKeySet,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltKeyReset:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstFltKeyReset, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_FltIVSet:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstFltIVSet, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_HDCP2_SetRiv:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstSetRiv, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_Multi2_SetRound:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pstSetRnd, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_Multi2_SetSysKey:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_psSetSysKey, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbStatus, spt_DSCMB_pDscmbStatus,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_PVR_RecCtrl:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbPvrRecCtrl, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_SetDefaultCAVid:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbSetDefaultCaVid, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_EngSetAlgo:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbEngSetAlgo, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_EngSetKey:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbEngSetKey, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_EngResetKey:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbEngResetKey, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_EngSetIV:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbEngSetIv, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_EngSetSwitch:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbEngSetSwitch, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_EngSetFSCB:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbEngSetFSCB, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_KLadder_AtomicExec:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbKLaddrAtomicExec, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_KLadder_ETSI:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbKLaddrETSI, spt_DSCMB_pDscmbKLaddrETSI,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_QueryCap:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_psQuery, spt_DSCMB_psQuery,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_OptConfig:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_psOptConfig, E_MDRV_CMD_DSCMB_OptConfig,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_SetDBGLevel:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbSetDbgLevel, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            case E_MDRV_CMD_DSCMB_GetConnectStatus:
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DSCMB_pDscmbSetConnectStatus, NULL,buffer_arg,sizeof(buffer_arg));
                break;
            default:
                break;
        };

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




