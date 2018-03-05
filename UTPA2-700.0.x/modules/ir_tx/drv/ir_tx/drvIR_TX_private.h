#ifndef _DRVIRTX_PRIV_H_
#define _DRVIRTX_PRIV_H_



#include "MsTypes.h"
#include "drv_IR_TX.h"

typedef enum {
    MDrv_CMD_IR_TX_Trigger,
    MDrv_CMD_IR_TX_SetStatus,
    MDrv_CMD_IR_TX_SetMemStatus,
    MDrv_CMD_IR_TX_SetClkDiv,
    MDrv_CMD_IR_TX_SetDelayCycleTime,
    MDrv_CMD_IR_TX_SetMemAddr,
    MDrv_CMD_IR_TX_SetMemData,
    MDrv_CMD_IR_TX_SetUnitValue,
    MDrv_CMD_IR_TX_SetShotCount,
    MDrv_CMD_IR_TX_Init,
    MDrv_CMD_IR_TX_SetCarrierCount
} ir_txIoctlOpt;


typedef struct _IR_TX_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}IR_TX_RESOURCE_PRIVATE;


typedef MS_BOOL (*IOCTL_IRTX_SETSTATUS)(MS_U16);
typedef MS_BOOL (*IOCTL_IRTX_SETMEMSTATUS)(MS_U16);
typedef MS_BOOL (*IOCTL_IRTX_SETCLKDIV)(MS_U8);
typedef MS_BOOL (*IOCTL_IRTX_SETDELAYCLKTIME)(MS_U16, MS_U16);
typedef MS_BOOL (*IOCTL_IRTX_SETMEMADDR)(MS_U16);
typedef MS_BOOL (*IOCTL_IRTX_SETMEMDATA)(MS_U16);
typedef MS_BOOL (*IOCTL_IRTX_SETUNITVALUE)(MS_U16, MS_U8);
typedef MS_BOOL (*IOCTL_IRTX_SETSHOTCOUNT)(MS_U16, MS_U16, MS_U8);
typedef MS_BOOL (*IOCTL_IRTX_INIT)(void);
typedef MS_BOOL (*IOCTL_IRTX_TRIGGER)(void);
typedef MS_BOOL (*IOCTL_IRTX_SETCARRIERCOUNT)(MS_U16);

typedef struct _IR_TX_INSTANT_PRIVATE
{
    IOCTL_IRTX_SETSTATUS            fpIRTXSetStatus;	
    IOCTL_IRTX_SETMEMSTATUS         fpIRTXSetMemStatus;	
    IOCTL_IRTX_SETCLKDIV            fpIRTXSetClkDiv;	
    IOCTL_IRTX_SETDELAYCLKTIME      fpIRTXSetDelayCycleTime;	
    IOCTL_IRTX_SETMEMADDR           fpIRTXSetMemAddr;	
    IOCTL_IRTX_SETMEMDATA           fpIRTXSetMemData;	
    IOCTL_IRTX_SETUNITVALUE         fpIRTXSetUnitValue;		
    IOCTL_IRTX_SETSHOTCOUNT         fpIRTXSetShotCount;
    IOCTL_IRTX_INIT                 fpIRTX_Init;	
	IOCTL_IRTX_TRIGGER              fpIRTX_Trigger;	
	IOCTL_IRTX_SETCARRIERCOUNT      fpIRTX_SetCarrierCount;	
	
}IR_TX_INSTANT_PRIVATE;

void IR_TXRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 IR_TXOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 IR_TXIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 IR_TXClose(void* pInstance);


MS_BOOL _MDrv_IR_TX_SetStatus(MS_U16 Status);
MS_BOOL _MDrv_IR_TX_SetMemStatus(MS_U16 Status);
MS_BOOL _MDrv_IR_TX_SetClkDiv(MS_U8 Div);
MS_BOOL _MDrv_IR_TX_SetDelayCycleTime(MS_U16 CycleTime_H, MS_U16 CycleTime_L);
MS_BOOL _MDrv_IR_TX_SetMemAddr(MS_U16 MemAddr);
MS_BOOL _MDrv_IR_TX_SetMemData(MS_U16 MemData);
MS_BOOL _MDrv_IR_TX_SetUnitValue(MS_U16 Unit_Value, MS_U8 Unit_Number);
MS_BOOL _MDrv_IR_TX_SetShotCount(MS_U16 H_ShotCount, MS_U16 L_ShotCount, MS_U8 Unit_Number);
MS_BOOL _MDrv_IR_TX_Init(void);
MS_BOOL _MDrv_IR_TX_Trigger(void);
MS_BOOL _MDrv_IR_TX_SetCarrierCount(MS_U16 Count);
#endif
