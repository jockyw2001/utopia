#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include "MsOS.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "apiXC_Dlc.h"
#include "apiXC_DBC.h"
#include "apiXC_Dlc_v2.h"
#include "DLC_adp.h"

//Function parameter

#ifdef MSOS_TYPE_LINUX_KERNEL
    #define CPY_FROM_USER(a,b,c)                                    if(copy_from_user(a,b,c) != 0) { break; }
    #define CPY_to_USER(a,b,c)                                      if(copy_to_user(a,b,c) != 0) { break; }
#else
    #define CPY_FROM_USER                                           memcpy
    #define CPY_to_USER                                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL


#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
    #define IS_CMP_TASK()                                             is_compat_task()
    #define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
    #define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }
#else
    #define IS_CMP_TASK()                                             (FALSE)
    #define CMP_CPY_FROM_USER                                       CPY_FROM_USER
    #define CMP_CPY_TO_USER                                         CPY_to_USER
#endif

//UADP_STRUCT_POINTER_TABLE spt_DLC_INIT_EX[10];
UADP_SPT_NAMEnNXT_DEF(ADP_DLC_InitData,0);
UADP_SPT_NAMEnNXT_DEF(DLC_INIT_EX,1);
//UADP_STRUCT_POINTER_TABLE spt_DLC_SET_SETTING_EX[10];
UADP_SPT_NAMEnNXT_DEF(ADP_DLC_MFinit_Ex,0);
UADP_SPT_NAMEnNXT_DEF(DLC_SET_SETTING_EX,1);
UADP_STRUCT_POINTER_TABLE spt_DLC_SET_CURVE[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_SET_BLE_SLOP_POINT[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_EXIT[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_SET_ON_OFF[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_SET_HANDLER_ON_OFF[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_SET_BLE_ON_OFF[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_HANDLER[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_HISTOGRAM_HANDLER[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_AVERAGE_VALUE[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_AVERAGE_VALUE_EX[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_INIT_CURVE[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_SPEEDUP_TRIGGER[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_LUMA_CURVE_STATUS[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_RESET_C_GAIN[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_CHECK_C_GAIN_IN_PQ_COM[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_RESET_Y_GAIN[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_CHECK_Y_GAIN_IN_PQ_COM[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_RESET[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_INIT[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_REINIT[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_CGC_HANDLER[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_LIB_VER[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_INFO[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_STATUS_EX[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_SET_DBG_LEVEL[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_WRITE_CURVE[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_WRITE_CURVE_SUB[10];
UADP_STRUCT_POINTER_TABLE spt_DLC_GET_HISTOGRAM[10];
//UADP_STRUCT_POINTER_TABLE spt_DLC_DECODE_EXT_CMD[10];
UADP_SPT_NAMEnNXT_DEF(ADP_DLC_DECODE_EXT_CMD_params,0);
UADP_SPT_NAMEnNXT_DEF(DLC_DECODE_EXT_CMD,1);
//UADP_STRUCT_POINTER_TABLE spt_DLC_SET_CAPTURE_RANGE[10];
UADP_SPT_NAMEnNXT_DEF(ADP_DLC_CAPTURE_Range,0);
UADP_SPT_NAMEnNXT_DEF(DLC_SET_CAPTURE_RANGE,1);
//UADP_STRUCT_POINTER_TABLE spt_DLC_SET_SETTING[10];
UADP_SPT_NAMEnNXT_DEF(DLC_SET_SETTING,0);
//UADP_STRUCT_POINTER_TABLE spt_DLC_INIT[10];
UADP_SPT_NAMEnNXT_DEF(ADP_DLC_init,0);
UADP_SPT_NAMEnNXT_DEF(DLC_INIT,1);
//UADP_STRUCT_POINTER_TABLE spt_DLC_GET_STATUS[10];
UADP_SPT_NAMEnNXT_DEF(ADP_DLC_ApiStatus,0);
UADP_SPT_NAMEnNXT_DEF(DLC_GET_STATUS,1);
UADP_STRUCT_POINTER_TABLE spt_DLC_SET_POWERSTATE[10];
//UADP_STRUCT_POINTER_TABLE spt_DBC_DLC_INIT[10];
UADP_SPT_NAMEnNXT_DEF(DBC_DLC_INIT,0);
UADP_STRUCT_POINTER_TABLE spt_DBC_INIT[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_SET_STATUS[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_GET_STATUS[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_SET_READY[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_SET_DEBUG_MODE[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_GET_DEBUG_MODE[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_UPDATE_PWM[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_HANDLER[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_ADJUST_YC_GAIN[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_YC_GAIN_INIT[10];
UADP_STRUCT_POINTER_TABLE spt_DBC_RESET[10];
//UADP_STRUCT_POINTER_TABLE spt_DBC_DECODE_EXT_CMD[10];
UADP_SPT_NAMEnNXT_DEF(ADP_DECODE_EXT_CMD_PARAMS,0);
UADP_SPT_NAMEnNXT_DEF(DBC_DECODE_EXT_CMD,1);

UADP_STRUCT_POINTER_TABLE spt_DrvDLC_MFinit[10];
UADP_STRUCT_POINTER_TABLE spt_DrvDLC_MFinit_Ex[10];
UADP_STRUCT_POINTER_TABLE spt_DrvDLC_DBC_MFinit[10];
UADP_STRUCT_POINTER_TABLE spt_DrvDLC_init[10];
UADP_STRUCT_POINTER_TABLE spt_DrvDLC_lumadata[10];
UADP_STRUCT_POINTER_TABLE spt_DrvDLC_ApiStatus[10];
UADP_STRUCT_POINTER_TABLE spt_DrvDLC_CAPTURE_Range[10];
UADP_STRUCT_POINTER_TABLE spt_DrvDLC_CONTROL_PARAMS[10];

UADP_SPT_NAMEnNXT_DEF(DLC_TMOInfo,0);
UADP_SPT_NAMEnNXT_DEF(DLC_SET_TMOINFO,1);

MS_U32 DLC_adp_Init(FUtopiaIOctl* pIoctl)
{

//set table
    MS_U32 temp;

    //Function parameter
    //stDLC_Init_Ex* pDLC_Init_Ex = (stDLC_Init_Ex*)temp;
    //stDLC_SetSetting_Ex* pDLC_SetSetting_Ex = (stDLC_SetSetting_Ex*)temp;
    //stDLC_SetCurve* pDLC_SetCurve = (stDLC_SetCurve*)temp;
    //stDLC_Exit* pDLC_Exit = (stDLC_Exit*)temp;
    //stDLC_SetOnOff* pDLC_SetOnOff = (stDLC_SetOnOff*)temp;
    //stDLC_SetDlcHandlerOnOff* pDLC_SetDlcHandlerOnOff = (stDLC_SetDlcHandlerOnOff*)temp;
    //stDLC_SetBleOnOff* pDLC_SetBleOnOff = (stDLC_SetBleOnOff*)temp;
    //stDLC_EnableMainSubCurveSynchronization* pDLC_EnableMainSubCurveSynchronization = (stDLC_EnableMainSubCurveSynchronization*)temp;
    //stDLC_Handler* pDLC_Handler = (stDLC_Handler*)temp;
    //stDLC_GetHistogramHandler* pDLC_GetHistogramHandler = (stDLC_GetHistogramHandler*)temp;
    //stDLC_GetAverageValue* pDLC_GetAverageValue = (stDLC_GetAverageValue*)temp;
    //stDLC_GetAverageValue_Ex* pDLC_GetAverageValue_Ex = (stDLC_GetAverageValue_Ex*)temp;
    //stDLC_InitCurve* pDLC_InitCurve = (stDLC_InitCurve*)temp;
    //stDLC_SpeedupTrigger* pDLC_SpeedupTrigger = (stDLC_SpeedupTrigger*)temp;
    //stDLC_GetLumaCurveStatus* pDLC_GetLumaCurveStatus = (stDLC_GetLumaCurveStatus*)temp;
    //stDLC_CGC_ResetCGain* pDLC_CGC_ResetCGain = (stDLC_CGC_ResetCGain*)temp;
    //stDLC_CGC_CheckCGainInPQCom* pDLC_CGC_CheckCGainInPQCom = (stDLC_CGC_CheckCGainInPQCom*)temp;
    //stDLC_CGC_ResetYGain* pDLC_CGC_ResetYGain = (stDLC_CGC_ResetYGain*)temp;
    //stDLC_CGC_CheckYGainInPQCom* pDLC_CGC_CheckYGainInPQCom = (stDLC_CGC_CheckYGainInPQCom*)temp;
    //stDLC_CGC_Reset* pDLC_CGC_Reset = (stDLC_CGC_Reset*)temp;
    //stDLC_CGC_Init* pDLC_CGC_Init = (stDLC_CGC_Init*)temp;
    //stDLC_CGC_ReInit* pDLC_CGC_ReInit = (stDLC_CGC_ReInit*)temp;
    //stDLC_CGC_Handler* pDLC_CGC_Handler = (stDLC_CGC_Handler*)temp;
    //stDLC_GetLibVer* pDLC_GetLibVer = (stDLC_GetLibVer*)temp;
    //stDLC_GetInfo* pDLC_GetInfo = (stDLC_GetInfo*)temp;
    //stDLC_GetStatus_Ex* pDLC_GetStatus_Ex = (stDLC_GetStatus_Ex*)temp;
    //stDLC_SetDbgLevel* pDLC_SetDbgLevel = (stDLC_SetDbgLevel*)temp;
    //stDLC_WriteCurve* pDLC_WriteCurve = (stDLC_WriteCurve*)temp;
    //stDLC_WriteCurve_Sub* pDLC_WriteCurve_Sub = (stDLC_WriteCurve_Sub*)temp;
    //stDLC_GetHistogram* pDLC_GetHistogram = (stDLC_GetHistogram*)temp;
    //stDLC_DecodeExtCmd* pDLC_DecodeExtCmd = (stDLC_DecodeExtCmd*)temp;
    //stDLC_SetCaptureRange* pDLC_SetCaptureRange = (stDLC_SetCaptureRange*)temp;
    //stDLC_SetSetting* pDLC_SetSetting = (stDLC_SetSetting*)temp;
    //stDLC_Init* pDLC_Init = (stDLC_Init*)temp;
    //stDLC_GetStatus* pDLC_GetStatus = (stDLC_GetStatus*)temp;
    //stDLC_SetPowerState* pDLC_SetPowerState = (stDLC_SetPowerState*)temp;
    //stDBC_DLC_Init* pDBC_DLC_Init = (stDBC_DLC_Init*)temp;
    //stDBC_Init* pDBC_Init = (stDBC_Init*)temp;
    //stDBC_Setstatus* pDBC_Setstatus = (stDBC_Setstatus*)temp;
    //stDBC_Getstatus* pDBC_Getstatus = (stDBC_Getstatus*)temp;
    //stDBC_SetReady* pDBC_SetReady = (stDBC_SetReady*)temp;
    //stDBC_SetDebugMode* pDBC_SetDebugMode = (stDBC_SetDebugMode*)temp;
    //stDBC_GetDebugMode* pDBC_GetDebugMode = (stDBC_GetDebugMode*)temp;
    //stDBC_UpdatePWM* pDBC_UpdatePWMT = (stDBC_UpdatePWM*)temp;
    //stDBC_Handler* pDBC_Handler = (stDBC_Handler*)temp;
    //stDBC_AdjustYCGain* pDBC_AdjustYCGain = (stDBC_AdjustYCGain*)temp;
    //stDBC_YCGainInit* pDBC_YCGainInit = (stDBC_YCGainInit*)temp;
    //stDBC_Reset* pDBC_Reset = (stDBC_Reset*)temp;
    //stDBC_DecodeExtCmd* pDBC_DecodeExtCmd = (stDBC_DecodeExtCmd*)temp;

    ////Function parameter's pointer
    //XC_DLC_MFinit* pDrvDLC_MFinit = (XC_DLC_MFinit*)temp;
    //XC_DLC_MFinit_Ex* pDrvDLC_MFinit_Ex = (XC_DLC_MFinit_Ex*)temp;
    //XC_DLC_DBC_MFinit* pDrvDLC_DBC_MFinit = (XC_DLC_DBC_MFinit*)temp;
    //XC_DLC_init* pDrvDLC_init = (XC_DLC_init*)temp;
    //XC_DLC_lumadata* pDrvDLC_lumadata = (XC_DLC_lumadata*)temp;
    //XC_DLC_ApiStatus* pDrvDLC_ApiStatus = (XC_DLC_ApiStatus*)temp;
    //XC_DLC_CAPTURE_Range* pDrvDLC_CAPTURE_Range = (XC_DLC_CAPTURE_Range*)temp;
    //tDLC_CONTROL_PARAMS* pDrvDLC_CONTROL_PARAMS = (tDLC_CONTROL_PARAMS*)temp;



    //Function parameter
    //UADPBypassSetSPT(&spt_DLC_INIT_EX[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_Init_Ex));
    //UADPBypassSetSPT(&spt_DLC_INIT_EX[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDLC_Init_Ex->pstXC_DLC_InitData)) - (unsigned int)pDLC_Init_Ex), &spt_DrvDLC_init);
    //UADPBypassSetSPT(&spt_DLC_INIT_EX[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(ADP_DLC_InitData,XC_DLC_init);
    UADP_SPT_NAME1NXT(DLC_INIT_EX,stDLC_Init_Ex,pstXC_DLC_InitData,ADP_DLC_InitData);

    //UADPBypassSetSPT(&spt_DLC_SET_SETTING_EX[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetSetting_Ex));
    //UADPBypassSetSPT(&spt_DLC_SET_SETTING_EX[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDLC_SetSetting_Ex->DLC_MFinit_Ex)) - (unsigned int)pDLC_SetSetting_Ex), &spt_DrvDLC_MFinit_Ex);
    //UADPBypassSetSPT(&spt_DLC_SET_SETTING_EX[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(ADP_DLC_MFinit_Ex,XC_DLC_MFinit_Ex);
    UADP_SPT_NAME1NXT(DLC_SET_SETTING_EX,stDLC_SetSetting_Ex,DLC_MFinit_Ex,ADP_DLC_MFinit_Ex);

    UADPBypassSetSPT(&spt_DLC_SET_CURVE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetCurve));
    UADPBypassSetSPT(&spt_DLC_SET_CURVE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_SET_BLE_SLOP_POINT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetBleSlopPoint));
    UADPBypassSetSPT(&spt_DLC_SET_BLE_SLOP_POINT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_EXIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_Exit));
    UADPBypassSetSPT(&spt_DLC_EXIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_SET_ON_OFF[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetOnOff));
    UADPBypassSetSPT(&spt_DLC_SET_ON_OFF[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_SET_HANDLER_ON_OFF[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetDlcHandlerOnOff));
    UADPBypassSetSPT(&spt_DLC_SET_HANDLER_ON_OFF[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_SET_BLE_ON_OFF[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetBleOnOff));
    UADPBypassSetSPT(&spt_DLC_SET_BLE_ON_OFF[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_EnableMainSubCurveSynchronization));
    UADPBypassSetSPT(&spt_DLC_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_HANDLER[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_Handler));
    UADPBypassSetSPT(&spt_DLC_HANDLER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_HISTOGRAM_HANDLER[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetHistogramHandler));
    UADPBypassSetSPT(&spt_DLC_GET_HISTOGRAM_HANDLER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_AVERAGE_VALUE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetAverageValue));
    UADPBypassSetSPT(&spt_DLC_GET_AVERAGE_VALUE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_AVERAGE_VALUE_EX[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetAverageValue_Ex));
    UADPBypassSetSPT(&spt_DLC_GET_AVERAGE_VALUE_EX[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_INIT_CURVE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_InitCurve));
    UADPBypassSetSPT(&spt_DLC_INIT_CURVE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_SPEEDUP_TRIGGER[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SpeedupTrigger));
    UADPBypassSetSPT(&spt_DLC_SPEEDUP_TRIGGER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_LUMA_CURVE_STATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetLumaCurveStatus));
    UADPBypassSetSPT(&spt_DLC_GET_LUMA_CURVE_STATUS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_RESET_C_GAIN[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_ResetCGain));
    UADPBypassSetSPT(&spt_DLC_CGC_RESET_C_GAIN[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_CHECK_C_GAIN_IN_PQ_COM[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_CheckCGainInPQCom));
    UADPBypassSetSPT(&spt_DLC_CGC_CHECK_C_GAIN_IN_PQ_COM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_RESET_Y_GAIN[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_ResetYGain));
    UADPBypassSetSPT(&spt_DLC_CGC_RESET_Y_GAIN[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_CHECK_Y_GAIN_IN_PQ_COM[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_CheckYGainInPQCom));
    UADPBypassSetSPT(&spt_DLC_CGC_CHECK_Y_GAIN_IN_PQ_COM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_RESET[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_Reset));
    UADPBypassSetSPT(&spt_DLC_CGC_RESET[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_Init));
    UADPBypassSetSPT(&spt_DLC_CGC_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_REINIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_ReInit));
    UADPBypassSetSPT(&spt_DLC_CGC_REINIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_CGC_HANDLER[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_CGC_Handler));
    UADPBypassSetSPT(&spt_DLC_CGC_HANDLER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_LIB_VER[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetLibVer));
    UADPBypassSetSPT(&spt_DLC_GET_LIB_VER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetInfo));
    UADPBypassSetSPT(&spt_DLC_GET_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_STATUS_EX[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetStatus_Ex));
    UADPBypassSetSPT(&spt_DLC_GET_STATUS_EX[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_SET_DBG_LEVEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetDbgLevel));
    UADPBypassSetSPT(&spt_DLC_SET_DBG_LEVEL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_WRITE_CURVE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_WriteCurve));
    UADPBypassSetSPT(&spt_DLC_WRITE_CURVE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_WRITE_CURVE_SUB[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_WriteCurve_Sub));
    UADPBypassSetSPT(&spt_DLC_WRITE_CURVE_SUB[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DLC_GET_HISTOGRAM[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetHistogram));
    UADPBypassSetSPT(&spt_DLC_GET_HISTOGRAM[1],UADP_SPT_END , 0, 0);

    //UADPBypassSetSPT(&spt_DLC_DECODE_EXT_CMD[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_DecodeExtCmd));
    //UADPBypassSetSPT(&spt_DLC_DECODE_EXT_CMD[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDLC_DecodeExtCmd->params)) - (unsigned int)pDLC_DecodeExtCmd), &spt_DrvDLC_CONTROL_PARAMS);
    //UADPBypassSetSPT(&spt_DLC_DECODE_EXT_CMD[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(ADP_DLC_DECODE_EXT_CMD_params,tDLC_CONTROL_PARAMS);
    UADP_SPT_NAME1NXT(DLC_DECODE_EXT_CMD,stDLC_DecodeExtCmd,params,ADP_DLC_DECODE_EXT_CMD_params);

    //UADPBypassSetSPT(&spt_DLC_SET_CAPTURE_RANGE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetCaptureRange));
    //UADPBypassSetSPT(&spt_DLC_SET_CAPTURE_RANGE[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDLC_SetCaptureRange->pu16_Range)) - (unsigned int)pDLC_SetCaptureRange), &spt_DrvDLC_CAPTURE_Range);
    //UADPBypassSetSPT(&spt_DLC_SET_CAPTURE_RANGE[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(ADP_DLC_CAPTURE_Range,XC_DLC_CAPTURE_Range);
    UADP_SPT_NAME1NXT(DLC_SET_CAPTURE_RANGE,stDLC_SetCaptureRange,pu16_Range,ADP_DLC_CAPTURE_Range);

    //UADPBypassSetSPT(&spt_DLC_SET_SETTING[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetSetting));
    //UADPBypassSetSPT(&spt_DLC_SET_SETTING[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDLC_SetSetting->DLC_MFinit)) - (unsigned int)pDLC_SetSetting), &spt_DrvDLC_MFinit);
    //UADPBypassSetSPT(&spt_DLC_SET_SETTING[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(DLC_SET_SETTING,stDLC_SetSetting);

    //UADPBypassSetSPT(&spt_DLC_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_Init));
    //UADPBypassSetSPT(&spt_DLC_INIT[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDLC_Init->pstXC_DLC_InitData)) - (unsigned int)pDLC_Init), &spt_DrvDLC_init);
    //UADPBypassSetSPT(&spt_DLC_INIT[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(ADP_DLC_init,XC_DLC_init);
    UADP_SPT_NAME1NXT(DLC_INIT,stDLC_Init,pstXC_DLC_InitData,ADP_DLC_init);

    //UADPBypassSetSPT(&spt_DLC_GET_STATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_GetStatus));
    //UADPBypassSetSPT(&spt_DLC_GET_STATUS[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDLC_GetStatus->pDrvStatus)) - (unsigned int)pDLC_GetStatus), &spt_DrvDLC_ApiStatus);
    //UADPBypassSetSPT(&spt_DLC_GET_STATUS[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(ADP_DLC_ApiStatus,XC_DLC_ApiStatus);
    UADP_SPT_NAME1NXT(DLC_GET_STATUS,stDLC_GetStatus,pDrvStatus,ADP_DLC_ApiStatus);

    UADPBypassSetSPT(&spt_DLC_SET_POWERSTATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDLC_SetPowerState));
    UADPBypassSetSPT(&spt_DLC_SET_POWERSTATE[1],UADP_SPT_END , 0, 0);

    //UADPBypassSetSPT(&spt_DBC_DLC_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_DLC_Init));
    //UADPBypassSetSPT(&spt_DBC_DLC_INIT[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDBC_DLC_Init->DLC_DBC_MFinit)) - (unsigned int)pDBC_DLC_Init), &spt_DrvDLC_DBC_MFinit);
    //UADPBypassSetSPT(&spt_DBC_DLC_INIT[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(DBC_DLC_INIT,stDBC_DLC_Init);

    UADPBypassSetSPT(&spt_DBC_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_Init));
    UADPBypassSetSPT(&spt_DBC_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_SET_STATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_Setstatus));
    UADPBypassSetSPT(&spt_DBC_SET_STATUS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_GET_STATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_Getstatus));
    UADPBypassSetSPT(&spt_DBC_GET_STATUS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_SET_READY[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_SetReady));
    UADPBypassSetSPT(&spt_DBC_SET_READY[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_SET_DEBUG_MODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_SetDebugMode));
    UADPBypassSetSPT(&spt_DBC_SET_DEBUG_MODE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_GET_DEBUG_MODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_GetDebugMode));
    UADPBypassSetSPT(&spt_DBC_GET_DEBUG_MODE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_UPDATE_PWM[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_UpdatePWM));
    UADPBypassSetSPT(&spt_DBC_UPDATE_PWM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_HANDLER[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_Handler));
    UADPBypassSetSPT(&spt_DBC_HANDLER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_ADJUST_YC_GAIN[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_AdjustYCGain));
    UADPBypassSetSPT(&spt_DBC_ADJUST_YC_GAIN[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_YC_GAIN_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_YCGainInit));
    UADPBypassSetSPT(&spt_DBC_YC_GAIN_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DBC_RESET[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_Reset));
    UADPBypassSetSPT(&spt_DBC_RESET[1],UADP_SPT_END , 0, 0);

    //UADPBypassSetSPT(&spt_DBC_DECODE_EXT_CMD[0],UADP_SPT_SELF_SIZE,0, sizeof(stDBC_DecodeExtCmd));
    //UADPBypassSetSPT(&spt_DBC_DECODE_EXT_CMD[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned int)(&(pDBC_DecodeExtCmd->params)) - (unsigned int)pDrvDLC_CONTROL_PARAMS), &spt_DrvDLC_CONTROL_PARAMS);
    //UADPBypassSetSPT(&spt_DBC_DECODE_EXT_CMD[2],UADP_SPT_END , 0, 0);
    UADP_SPT_NAME0NXT(ADP_DECODE_EXT_CMD_PARAMS,tDBC_CONTROL_PARAMS);
    UADP_SPT_NAME1NXT(DBC_DECODE_EXT_CMD,stDBC_DecodeExtCmd,params,ADP_DECODE_EXT_CMD_PARAMS);

#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    UADP_SPT_NAME0NXT(DLC_TMOInfo,ST_XC_DLC_TMO_INFO);
    UADP_SPT_NAME1NXT(DLC_SET_TMOINFO,stDLC_SetTMOInfo,pstDLC_TMOInfo,DLC_TMOInfo);
#endif

    *pIoctl= (FUtopiaIOctl)DLC_adp_Ioctl;
    return 0;
}

MS_U32 DLC_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[2048];
    switch(u32Cmd)
    {
        case E_DLC_CMD_INIT_EX :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_INIT_EX, spt_DLC_INIT_EX, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_SETTING_EX :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_SETTING_EX, spt_DLC_SET_SETTING_EX, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_CURVE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_CURVE, spt_DLC_SET_CURVE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_BLE_SLOP_POINT :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_BLE_SLOP_POINT, spt_DLC_SET_BLE_SLOP_POINT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_EXIT :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_EXIT, spt_DLC_EXIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_ON_OFF :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_ON_OFF, spt_DLC_SET_ON_OFF, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_HANDLER_ON_OFF :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_HANDLER_ON_OFF, spt_DLC_SET_HANDLER_ON_OFF, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_BLE_ON_OFF :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_BLE_ON_OFF, spt_DLC_SET_BLE_ON_OFF, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION, spt_DLC_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_HANDLER :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_HANDLER, spt_DLC_HANDLER, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_HISTOGRAM_HANDLER :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_HISTOGRAM_HANDLER, spt_DLC_GET_HISTOGRAM_HANDLER, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_AVERAGE_VALUE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_AVERAGE_VALUE, spt_DLC_GET_AVERAGE_VALUE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_AVERAGE_VALUE_EX :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_AVERAGE_VALUE_EX, spt_DLC_GET_AVERAGE_VALUE_EX, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_INIT_CURVE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_INIT_CURVE, spt_DLC_INIT_CURVE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SPEEDUP_TRIGGER :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SPEEDUP_TRIGGER, spt_DLC_SPEEDUP_TRIGGER, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_LUMA_CURVE_STATUS :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_LUMA_CURVE_STATUS, spt_DLC_GET_LUMA_CURVE_STATUS, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_RESET_C_GAIN :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_RESET_C_GAIN, spt_DLC_CGC_RESET_C_GAIN, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_CHECK_C_GAIN_IN_PQ_COM :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_CHECK_C_GAIN_IN_PQ_COM, spt_DLC_CGC_CHECK_C_GAIN_IN_PQ_COM, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_RESET_Y_GAIN :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_RESET_Y_GAIN, spt_DLC_CGC_RESET_Y_GAIN, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_CHECK_Y_GAIN_IN_PQ_COM :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_CHECK_Y_GAIN_IN_PQ_COM, spt_DLC_CGC_CHECK_Y_GAIN_IN_PQ_COM, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_RESET :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_RESET, spt_DLC_CGC_RESET, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_INIT :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_INIT, spt_DLC_CGC_INIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_REINIT :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_REINIT, spt_DLC_CGC_REINIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_CGC_HANDLER :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_CGC_HANDLER, spt_DLC_CGC_HANDLER, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_LIB_VER :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_LIB_VER, spt_DLC_GET_LIB_VER, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_INFO :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_INFO, spt_DLC_GET_INFO, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_STATUS_EX :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_STATUS_EX, spt_DLC_GET_STATUS_EX, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_DBG_LEVEL :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_DBG_LEVEL, spt_DLC_SET_DBG_LEVEL, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_WRITE_CURVE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_WRITE_CURVE, spt_DLC_WRITE_CURVE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_WRITE_CURVE_SUB :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_WRITE_CURVE_SUB, spt_DLC_WRITE_CURVE_SUB, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_HISTOGRAM :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_HISTOGRAM, spt_DLC_GET_HISTOGRAM, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_DECODE_EXT_CMD :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_DECODE_EXT_CMD, spt_DLC_DECODE_EXT_CMD, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_CAPTURE_RANGE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_CAPTURE_RANGE, spt_DLC_SET_CAPTURE_RANGE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_SETTING :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_SETTING, spt_DLC_SET_SETTING, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_INIT :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_INIT, spt_DLC_INIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_GET_STATUS :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_GET_STATUS, spt_DLC_GET_STATUS, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_POWERSTATE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_POWERSTATE, spt_DLC_SET_POWERSTATE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DLC_CMD_SET_TMOINFO :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DLC_SET_TMOINFO, spt_DLC_SET_TMOINFO, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_DLC_INIT :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_DLC_INIT, spt_DBC_DLC_INIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_INIT :
            //u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_CMD_INIT, spt_DBC_CMD_INIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_SET_STATUS :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_SET_STATUS, spt_DBC_SET_STATUS, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_GET_STATUS :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_GET_STATUS, spt_DBC_GET_STATUS, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_SET_READY :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_SET_READY, spt_DBC_SET_READY, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_SET_DEBUG_MODE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_SET_DEBUG_MODE, spt_DBC_SET_DEBUG_MODE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_GET_DEBUG_MODE :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_GET_DEBUG_MODE, spt_DBC_GET_DEBUG_MODE, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_UPDATE_PWM :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_UPDATE_PWM, spt_DBC_UPDATE_PWM, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_HANDLER :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_HANDLER, spt_DBC_HANDLER, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_ADJUST_YC_GAIN :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_ADJUST_YC_GAIN, spt_DBC_ADJUST_YC_GAIN, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_YC_GAIN_INIT :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_YC_GAIN_INIT, spt_DBC_YC_GAIN_INIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_RESET :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_RESET, spt_DBC_RESET, buffer_arg,sizeof(buffer_arg));
            break;

        case E_DBC_CMD_DECODE_EXT_CMD :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DBC_DECODE_EXT_CMD, spt_DBC_DECODE_EXT_CMD, buffer_arg,sizeof(buffer_arg));
            break;

    }

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




