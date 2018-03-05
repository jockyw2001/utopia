#ifndef _DRVIRTX_H_
#define _DRVIRTX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

#define IRTX_UTOPIA20       (1)

typedef enum _IRTX_Result
{
 
    IRTX_FAIL = 0
    ,IRTX_OK = 1
    ,IRTX_TIMEOUT
    ,IRTX_QUEUE_FULL
    ,IRTX_BUSY
}IRTX_Result;


////////////////////////////////////////////////////////

typedef struct _IRTX_TRIGGER_PARAM
{
	
}IRTX_TRIGGER_PARAM, *PIRTX_TRIGGER_PARAM;

typedef struct _IRTX_SETSTATUS_PARAM
{
    MS_U16 Status;
}IRTX_SETSTATUS_PARAM, *PIRTX_SETSTATUS_PARAM;


typedef struct _IRTX_SETMEMSTATUS_PARAM
{
    MS_U16 Status;
}IRTX_SETMEMSTATUS_PARAM, *PIRTX_SETMEMSTATUS_PARAM;


typedef struct _IRTX_SETCLKDIV_PARAM
{
    MS_U8 Div;
}IRTX_SETCLKDIV_PARAM, *PIRTX_SETCLKDIV_PARAM;


typedef struct _IRTX_SETDELAYCLKTIME_PARAM
{
    MS_U16 CycleTime_H;
	MS_U16 CycleTime_L;
}IRTX_SETDELAYCLKTIME_PARAM, *PIRTX_SETDELAYCLKTIME_PARAM;


typedef struct _IRTX_SETMEMADDR_PARAM
{
    MS_U16 MemAddr;
}IRTX_SETMEMADDR_PARAM, *PIRTX_SETMEMADDR_PARAM;


typedef struct _IRTX_SETMEMDATA_PARAM
{
    MS_U16 MemData;
}IRTX_SETMEMDATA_PARAM, *PIRTX_SETMEMDATA_PARAM;


typedef struct _IRTX_SETUNITVALUE_PARAM
{
    MS_U16 Unit_Value;
	MS_U8 Unit_Number;
}IRTX_SETUNITVALUE_PARAM, *PIRTX_SETUNITVALUE_PARAM;


typedef struct _IRTX_SETSHOTCOUNT_PARAM
{
    MS_U16 H_ShotCount;
    MS_U16 L_ShotCount;
 	MS_U8 Unit_Number;
}IRTX_SETSHOTCOUNT_PARAM, *PIRTX_SETSHOTCOUNT_PARAM;



typedef struct _IRTX_INIT_PARAM
{

}IRTX_INIT_PARAM, *PIRTX_INIT_PARAM;



typedef struct _IRTX_SETCARRIERCOUNT_PARAM
{
    MS_U16 Count;
}IRTX_SETCARRIERCOUNT_PARAM, *PIRTX_SETCARRIERCOUNT_PARAM;




MS_BOOL MDrv_IR_TX_Trigger(void);
MS_BOOL MDrv_IR_TX_SetStatus(MS_U16 Status);
MS_BOOL MDrv_IR_TX_SetMemStatus(MS_U16 Status);
MS_BOOL MDrv_IR_TX_SetClkDiv(MS_U8 Div);
MS_BOOL MDrv_IR_TX_SetDelayCycleTime(MS_U16 CycleTime_H, MS_U16 CycleTime_L);
MS_BOOL MDrv_IR_TX_SetMemAddr(MS_U16 MemAddr);
MS_BOOL MDrv_IR_TX_SetMemData(MS_U16 MemData);
MS_BOOL MDrv_IR_TX_SetUnitValue(MS_U16 Unit_Value, MS_U8 Unit_Number);
MS_BOOL MDrv_IR_TX_SetShotCount(MS_U16 H_ShotCount, MS_U16 L_ShotCount, MS_U8 Unit_Number);
MS_BOOL MDrv_IR_TX_Init(void);
MS_BOOL MDrv_IR_TX_SetCarrierCount(MS_U16 Count);


#ifdef __cplusplus
}
#endif
#endif
