

#include "MsCommon.h"

#ifdef MSOS_TYPE_LINUX
#include "string.h"
#endif


#include "asmCPU.h"
#include "regNDSRASP.h"
#include "halNDSRASP.h"
#include "drvNDSRASP.h"
#include "XtvTypes.h"
#include "ndstypes.h"
#include "drvMMIO.h"
#include "drvEMMFlt.h"
#include "halEMMflt.h"
//--------------------------------------------------------------------------------------------------
//  Debug Macro
//--------------------------------------------------------------------------------------------------
static MS_U32       _g32RASPDrvDbgLv = RASP_DBGLV_DEBUG;

#define DRVRASP_DBG(lv, x, args...)   if (lv <= _g32RASPDrvDbgLv ) \
                                          { printf("%s, ",__FUNCTION__); \
                                            printf(x, ##args);  }

//------------------------------------------------------------------------//
//       Macro Definition
//------------------------------------------------------------------------//

#define RASP_INT_EVENT_ALL (RASP_INT_EFRAME_RD_OVF | RASP_INT_EFRAME_WT_OVF| \
                            RASP_INT_STR2MIU | RASP_INT_PAYLD2MIU | RASP_INT_ECM2MIU|\
                            RASP_INT_TIME_WATER_MARK|RASP_INT_EVENT_WATER_MARK |\
                            RASP_INT_ECM_PKT_RDY )


#define NDS_RASP_TASK_STACK_SIZE  (1024 * 16 )


#define NDS_RASP_TIME_WATER_MARK   0x00000200
#define NDS_RASP_EVENT_WATER_MARK   0x00000400
#define NDS_RASP_ECM_OVERFLOW_SHIFT   0x00000016

#define NDS_RASP_EVENT_ALL            0xFFFFFFFF



#define NDS_CAM_FLT_BUF_SIZE            (256)
#define NDS_CAM_FLT_BUF_ALIGN           (8)
#define NDS_CAM_ECM_FLT_NUM             (6)
#define NDS_CAM_EMM_BUF_NUM             (10)

#define NDS_RASP_PAYLOAD_PKT_NUM            (1000)

#define NDS_RASP_ECM_BUF_SIZE         (NDS_CAM_FLT_BUF_SIZE  * (NDS_CAM_ECM_FLT_NUM+2) )
#define NDS_RASP_PAYLOAD_BUF_SIZE       (192 * NDS_RASP_PAYLOAD_PKT_NUM )

#define NDS_RASP_ALL_BUF_SIZE         (NDS_RASP_ECM_BUF_SIZE + NDS_RASP_PAYLOAD_BUF_SIZE)
#define CA_OK                                   0
#define CA_REQUEST_NOT_SUPPORTED_BY_DRIVER      1
#define CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW  2
#define CA_GLOBAL_HARDWARE_FAILURE              3
#define CA_ECM_PID_FILTER_ALREADY_DEFINED       4
#define CA_DRIVER_INVALID_DATA_STREAM           5
#define CA_DRIVER_INVALID_LOGICAL_STREAM_ID     6
// #define CA_INSUFICIENT_BUFFER                   7
#define CA_INFO_NOT_AVAILABLE                           7
#define CA_INSUFFICIENT_BUFFER                          8
#define CA_SCHEME_NOT_SUPPORTED                         9
#define CA_FUNCTIONALITY_NOT_AVAILABLE_UNTIL_RESTART    10
#define CA_FUNCTIONALITY_PERMANENTLY_NOT_AVAILABLE      11
#define CA_FEATURE_NOT_SUPPORTED                        12

#if 1
#define MAX_PIDS                                8

/*  HDICA_ECM_FILTER_CONTROL_ODD_RECEPTION_MASK */
#define HDICA_ECM_FILTER_CONTROL_DISABLE_ODD_RECEPTION  0x00
#define HDICA_ECM_FILTER_CONTROL_ENABLE_ODD_RECEPTION   0x20

/*  HDICA_ECM_FILTER_CONTROL_EVEN_RECEPTION_MASK */
#define HDICA_ECM_FILTER_CONTROL_DISABLE_EVEN_RECEPTION 0x00
#define HDICA_ECM_FILTER_CONTROL_ENABLE_EVEN_RECEPTION  0x40

#define REG_RASP_ECM03_PID(n)        (0x0012+n*1)
    #define RASP_ECM_PID_MASK                        BMASK(12:0)
    #define RASP_ECM_PID_TID_MASK                    BMASK(14:13)
    #define RASP_ECM_PID_TID_ODD                     BIT(13)
    #define RASP_ECM_PID_TID_EVEN                    BIT(14)
    #define RASP_ECM_PID_ENABLE                      BIT(15)
//#define REG_RASP_ECM_TID             0x0016
//    #define RASP_ECM_TID_EVEN_MASK                   BMASK(7:0)
//    #define RASP_ECM_TID_ODD_MASK                    BMASK(15:8)
#define REG_RASP_ECM03_LOCK_CTRL     0x0017
    #define RASP_ECM_LOCK_BITS                       4
//#define REG_RASP_CA_INT              0x0018
    #define RASP_CA_INT_ECM                          BIT(0)
    #define RASP_CA_INT_MASK_RESET_MODE              BIT(7)  // set 1 to reset interrupt status
#define REG_RASP_ECM_INT_STAT        0x0019
    #define RASP_ECM_INT_FLT_MASK                    BMASK(7:0)

#define REG_RASP_ECM45_PID(n)        (0x0030+(n-4)*1)

#define REG_RASP_ECM45_LOCK_CTRL_     0x0032

/*  HDICA_ECM_RECEPTION_MASK */
#define HDICA_ECM_RECEPTION_DISABLE 0x00
#define HDICA_ECM_RECEPTION_ENABLE  0x80

#define HDICA_ECM_DISABLE_RECEPTION 0x00   /* it's multiply because of mismatch in the document */
#define HDICA_ECM_ENABLE_RECEPTION  0x80

//#define HW_RASP_REGBASE              0xBF223800
//#define RASP_REG(addr)               (*((volatile MS_U16*)(HW_RASP_REGBASE + ((addr)<<2))))

// Macros
#define REG_RASP_ECM_PID(n)          ( (n<4) ? (REG_RASP_ECM03_PID(n)) : (REG_RASP_ECM45_PID(n)) )
#define REG_RASP_ECM_LOCK(n)         ( (n<4) ? (REG_RASP_ECM03_LOCK_CTRL) : (REG_RASP_ECM45_LOCK_CTRL_) )
    #define RASP_ECM_LOCK_MASK(n)                    ( (n<4) ? (BMASK(2:0)<<(RASP_ECM_LOCK_BITS*n)) : (BMASK(2:0)<<(RASP_ECM_LOCK_BITS*(n-4))) )
    #define RASP_ECM_LOCK_AUTO(n)                    ( (n<4) ? (BIT(0)    <<(RASP_ECM_LOCK_BITS*n)) : (BIT(0)    <<(RASP_ECM_LOCK_BITS*(n-4))) )
    #define RASP_ECM_LOCK_EVEN(n)                    ( (n<4) ? (BIT(1)    <<(RASP_ECM_LOCK_BITS*n)) : (BIT(1)    <<(RASP_ECM_LOCK_BITS*(n-4))) )
    #define RASP_ECM_LOCK_ODD(n)                     ( (n<4) ? (BIT(2)    <<(RASP_ECM_LOCK_BITS*n)) : (BIT(2)    <<(RASP_ECM_LOCK_BITS*(n-4))) )
#endif

//------------------------------------------------------------------------//
//       Local Structure definition
//------------------------------------------------------------------------//

typedef struct
{

    MS_U16 pid ;
    MS_U16 ecmid;
    MS_U8  ecm_ctrl;
    MS_U32 Xconn;
    MS_BOOL b_defined;

} NDS_RASP_ECM_FLT;

typedef struct
{
    MS_U16            u16FltId;
    XUSHORT           Pid;
    F_RASP_EV_MASK    EventMask;
    F_RASP_EV_MASK    PayloadMask;
    DrvRASP_FltState  RASP_FltState;
} EventMask_s;

//------------------------------------------------------------------------//
//       Variable declaration
//------------------------------------------------------------------------//

//static MS_U8 *_rasp_ecm_addr[RASP_NUM] ;//{[0 ... (RASP_NUM)] = NULL } ;
static MS_S32 _nds_rasp_eventid = -1 , _nds_rasp_taskid  = -1 ;

//static MS_U32 _u32LastErr;
//static MS_BOOL _bSuspend = FALSE;

MS_U8    _u8RaspEcmBuf[RASP_NUM][NDS_CAM_ECM_FLT_NUM][2][NDS_CAM_FLT_BUF_SIZE];

NDS_RASP_ECM_FLT _rasp_ecm_flt[RASP_NUM][NDS_CAM_ECM_FLT_NUM];

static MS_U32  _nds_rasp_stack[NDS_RASP_TASK_STACK_SIZE/sizeof(MS_U32)];

//static EventMask_s _tEventMask[RASP_EVENT_NUM];
//static MS_U32 _u32PidIndEventMask = 0;

//static XUSHORT EcmPid[RASP_NUM][NDS_CAM_ECM_FLT_NUM];
static MS_U32 EcmPktNumer,EcmWritePtr,EcmTimeStamp;
static MS_U16 EcmIntStatus;
static MS_BOOL _bRASPIsInit = FALSE;

//#ifdef MSOS_TYPE_LINUX_KERNEL
//static irqreturn_t _NDS_RASP_Isr(int irq, void *dev_id);
//#else
//static void _NDS_RASP_Isr(InterruptNum eIntNum);
//#endif

static void _NDS_RASP_IsrTask(void);
RASP_EVENT_NOTIFY_FOUNCTION     *_RaspEventCB;
RASP_OVERFLOW_NOTIFY_FUNCTION   *_RaspOvfCB;
MS_PHYADDR _pPayloadBuf0;
MS_PHYADDR _pPayloadBuf1;
MS_U32 _PayloadSize0;
MS_U32 _PayloadSize1;
MS_U8 _u8pingpong;

//static MS_U8 *_rasp_payload_addr[RASP_NUM] ;//{[0 ... (RASP_NUM)] = NULL } ;
//RASP_EVENT _RaspEvents[NDS_RASP_PAYLOAD_PKT_NUM];	//egi

static MS_U32 _u32RaspEventFlag;
static MS_U32 _u32RaspOvfEventFlag;
static MS_U16 _RaspEventIdx;
//static MS_U16 _RaspNumEvents = 0;
static MS_U16 _RaspNumEventsTotal=0;
static MS_U16 _RaspReadIdx = 0;
MS_U16 _RaspEventHighMark;
MS_U32 _RaspTriggerExecutor;


//static MS_U16 EventLog = 0;
//static MS_U32 EventDescr = 0;
static MS_U16 EventNumber = 0;
static MS_U16 EventCurIndex[RASP_NUM];
static MS_U16 EventReadIndex[RASP_NUM];
static MS_U16 EventTmpIndex[RASP_NUM];

//static MS_U16 EventWriteIndex = 0;

static NDS_RASP_EVENT _EventDataArray[RASP_NUM][RASP_EVENT_FIFO_DEPTH];

//static MS_U16 EventNumber2 = 0;
//static MS_U32 _EventDataArray2[6];

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

// Safe Mode Configuration
// ============================================
//              ISR_SAFE            TASK_SAFE
//  ISR_SAFE    Int,Reg             Int,Reg,Task
//  TASK_SAFE   Int,Reg,Task        Mutex,Reg,Task
//
#define _RASP_SetEvent(flag)                            MsOS_SetEvent(_s32RASPEventId, (flag))
#define _RASP_SetSecRdyId(eng, flag)                    MsOS_SetEvent(_s32RASPSecRdyId[(eng)], (flag))
#define _RASP_SetSecOvfId(eng, flag)                    MsOS_SetEvent(_s32RASPSecOvfId[(eng)], (flag))

#define _RASP_GetEvent(events, flag, opt, timeout)      MsOS_WaitEvent(_s32RASPEventId, (flag), &(events), (opt), (timeout))
#define _RASP_GetSecRdyId(eng, flag, events, opt)       MsOS_WaitEvent(_s32RASPSecRdyId[(eng)], (flag), &(events), (opt), 0)
#define _RASP_GetSecOvfId(eng, flag, events, opt)       MsOS_WaitEvent(_s32RASPSecOvfId[(eng)], (flag), &(events), (opt), 0)

#define _RASP_SetSecCrcId(eng, flag)                    MsOS_SetEvent(_s32RASPSecCrcId[(eng)], (flag))
#define _RASP_GetSecCrcId(eng, flag, events, opt)       MsOS_WaitEvent(_s32RASPSecCrcId[(eng)], (flag), &(events), (opt), 0)

#define _RASP_SetFltDisableId(eng, flag)                MsOS_SetEvent(_s32RASPFltDisableId[(eng)], (flag))
#define _RASP_GetFltDisableId(eng, flag, events, opt)   MsOS_WaitEvent(_s32RASPFltDisableId[(eng)], (flag), &(events), (opt), 0)

//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------

#if defined (MSOS_TYPE_NOS)
#define _RASP_ENTRY()
#define _RASP_RETURN(_ret) {return _ret;}
#define _RASP_CHECK(idx)
#else
#define _RASP_ENTRY()       if (!MsOS_ObtainMutex(_prasp_res->_s32RASPMutexId, MSOS_WAIT_FOREVER))      \
                            {                                                               \
                                _prasp_res->_u32LastErr = DRVRASP_FUNC_ERROR;                           \
                                printf("_RASP_ENTRY return ERROR\n");                       \
                                return DRVRASP_FUNC_ERROR;                                  \
                            }                                                               \
                            if (_prasp_res->_bSuspend)                                                  \
                            {                                                               \
                                _RASP_RETURN(FALSE);                                 \
                            }

#define _RASP_RETURN(_ret)  if(MsOS_ReleaseMutex(_prasp_res->_s32RASPMutexId)) {return _ret;}           \
                            else                                                            \
                            {                                                               \
                                _prasp_res->_u32LastErr = DRVRASP_FUNC_ERROR;                           \
                                return DRVRASP_FUNC_ERROR;                                  \
                            }

#define _RASP_CHECK(idx)    if( (MS_U32)idx >= (MS_U32)RASP_NUM ) \
                            { printf("[RASP][ERR] Rasp Engine [%d] not exist ! \n",idx);   \
                             _RASP_RETURN(FALSE); }
#endif

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

typedef struct _DrvRASP_State
{
    MS_U32          _RASP_init;
    DrvRASP_FltState _RASP_FltState;
    DrvRASP_FltState _RASP_ECMFltState;
} DrvRASP_State;

typedef struct _RASP_RESOURCE_PRIVATE
{
    MS_S32                      _s32RASPMutexId;
    MS_U32                      _u32LastErr;
    MS_BOOL                     _bSuspend;

    DrvRASP_State               _RASPState[RASP_NUM][RASP_PIDFLT_NUM_ALL];

    DrvRASP_Event               _RASPEvtNotify[RASP_NUM];
    P_DrvRASP_EvtCallback       _RASPCallback[RASP_NUM];
	P_DrvRASP_EventCB			_RASPEventCB[RASP_NUM];	//event array callback

    MS_BOOL                     _bRASPInitReady;
}RASP_RESOURCE_PRIVATE;


//static MSIF_Version _drv_RASP_version = {
//    .DDI = { RASP_DRV_VERSION, },
//};


static RASP_RESOURCE_PRIVATE _rasp_res = {
    ._s32RASPMutexId                        = -1,
    ._u32LastErr                            = TRUE,
    ._bSuspend                              = FALSE,
    ._bRASPInitReady                        = FALSE,
};

static RASP_RESOURCE_PRIVATE*    _prasp_res = &_rasp_res;
static P_NDSRASP_Callback        _ndsrasp_cb = NULL;

static MS_PHYADDR _rasp_writeptr = 0;
static MS_PHYADDR _rasp_payloadptr = 0;

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
irqreturn_t _NDS_RASP_Isr(int irq, void *dev_id)
#else
void _NDS_RASP_Isr(InterruptNum eIntNum)
#endif
{
#if 0
    MS_U8               u8EcmReg;
    MS_U8               ecm_polarity;
    MS_U32              i;
    MS_U32              u32OvfEvents = 0;
#endif
    MS_U32 IntStatus[RASP_NUM];
    MS_U16 i;
    MS_U32 RaspID;

    //MsOS_DisableInterrupt(E_INT_IRQ_RASP);
	//printf("____NDS_RASP_Isr____\n");

    for( RaspID=0; RaspID<RASP_NUM; RaspID++)
    {
        IntStatus[RaspID] = (MS_U32)HAL_NDSRASP_INT_GetHW(RaspID);

        if(IntStatus[RaspID] == 0)
            continue;

        if(IntStatus[RaspID] & RASP_INT_STR2MIU)
        {
            _rasp_writeptr = HAL_NDSRASP_GetWritePtr(RaspID);
        }

        if( IntStatus[RaspID] & (RASP_INT_TIME_WATER_MARK | RASP_INT_EVENT_WATER_MARK) )
        {
            EventNumber = HAL_NDSRASP_EVENT_GetEventNum(RaspID);//reg_RASP0_event_log_ctrl_status

			//Ring queue push
            if(EventCurIndex[RaspID] + EventNumber < RASP_EVENT_FIFO_DEPTH)
            {
                for(i = EventCurIndex[RaspID] + 1; i <= (EventCurIndex[RaspID] + EventNumber); i++)
                	{HAL_NDSRASP_EVENT_GetEventDescriptor(RaspID, (MS_U32*)&_EventDataArray[RaspID][i], 5);}
                EventCurIndex[RaspID] = EventCurIndex[RaspID] + EventNumber;
            }
            else
            {
                for(i = EventCurIndex[RaspID] + 1; i < RASP_EVENT_FIFO_DEPTH; i++)
                	{HAL_NDSRASP_EVENT_GetEventDescriptor(RaspID, (MS_U32*)&_EventDataArray[RaspID][i], 5);}
                for(i = 0; i <= (EventCurIndex[RaspID] + EventNumber - RASP_EVENT_FIFO_DEPTH); i++)
                	{HAL_NDSRASP_EVENT_GetEventDescriptor(RaspID, (MS_U32*)&_EventDataArray[RaspID][i], 5);}
                EventCurIndex[RaspID] = (EventCurIndex[RaspID] + EventNumber - RASP_EVENT_FIFO_DEPTH);
            }

			EventTmpIndex[RaspID] = EventCurIndex[RaspID];
			printf("EventCurIndex=%x EventNumber=%x\n", (MS_U32)EventCurIndex[RaspID], (MS_U32)EventNumber);

		}

        if(IntStatus[RaspID] & RASP_INT_EFRAME_RD_OVF)
        {
        }

        if(IntStatus[RaspID] & RASP_INT_EFRAME_WT_OVF)
        {
        }

        if(IntStatus[RaspID] & RASP_INT_PAYLD2MIU)
        {
            _rasp_payloadptr = HAL_NDSRASP_GetPayloadWritePtr(RaspID);
        }

        if (IntStatus[RaspID] & RASP_INT_ECM_PKT_RDY) // ECM Packet Ready
        {
            EcmPktNumer = HAL_NDSRASP_GetEcmPktNum(RaspID);
			printf("ECM Packet Number is [0x%x]\n", EcmPktNumer);

			//printf("ECM Interrupt state = %X\n", HAL_NDSRASP_SetECM_GetHW(RaspID));
			HAL_NDSRASP_SetECM_ResetHW(RaspID, 0xFFFF);

			EcmWritePtr = HAL_NDSRASP_GetECMWritePtr(RaspID);
			printf("ECM EcmWritePtr is [0x%x]\n", EcmWritePtr);

        	#if 0	//TODO
            EcmWritePtr = HAL_NDSRASP_GetECMWritePtr(EngId);
            EcmTimeStamp= HAL_NDSRASP_GetECMTimeStamp(EngId);
            //NDS_RASP_DBG("HAL_NDSRASP_SetECM_GetHW = %X\n", HAL_NDSRASP_SetECM_GetHW(0));

            EcmIntStatus = HAL_NDSRASP_SetECM_GetHW(EngId);
            //HAL_NDSRASP_SetECM_ResetHW(EngId, EcmIntStatus);


            if (pEcmData[0] != 0x47)
            {
                continue; //_NDS_ASSERT_;
            }
            if (pEcmData[5] == 0x80)
            {
                ecm_polarity = 0; // EVEN
            }
            else if (pEcmData[5] == 0x81)
            {
                ecm_polarity = 1; // ODD
            }
            else
            {
                NDS_ASSERT(FALSE, , "[%s]-[%d]\n", __FUNCTION__, __LINE__);
            }


            //need to copy to ecm buffer to keep buffer for future usage.....
            //memcpy(_u8RaspEcmBuf,
        	#endif
        }

        HAL_NDSRASP_INT_ClrHW(RaspID , IntStatus[RaspID]);
        //HAL_NDSRASP_INT_ClrHW(RaspID , 0x0);
        //printf("int status after clear = %x",(MS_U32)HAL_NDSRASP_INT_GetHW(RaspID));
        //HAL_NDSRASP_INT_Enable(RaspID , 0xffff);

		MsOS_SetEvent(_nds_rasp_eventid, RaspID<<16 | (IntStatus[RaspID] ) );
    }


    if(FALSE == HAL_EMMFLT_GetIntMode())
    {
        MsOS_EnableInterrupt(E_INT_IRQ_RASP);
    }

}

static void _NDS_RASP_IsrTask(void)
{
    //MS_U32  i ,p ,hi_count ,lo_count;;
    MS_U32 u32Events;
    //MS_U32 DataArray[5];
    MS_U8  RaspID;
    NDSRaspCallBack_t cb_info;
	P_DrvRASP_EventCB _Event_CallBackFun;
	MS_U16 u16EventQueued=0;

    printf("ISR task\n");

    while(1)
    {
        MsOS_WaitEvent(_nds_rasp_eventid, NDS_RASP_EVENT_ALL, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        RaspID = ((u32Events>>16) & 0xff);
        printf("[RASP]_NDS_RASP_IsrTask!!----- u32Events = 0x%x, ID = %x \n",u32Events, RaspID);
        //printf("EventLog = %x, EventDescr = %x, EventNumber = %x\n",EventLog, EventDescr, EventNumber);

        if(RaspID >= RASP_NUM)
        {
            DRVRASP_DBG(RASP_DBGLV_ERR,"invalid Rasp ID\n");
            continue;
        }

		_Event_CallBackFun = _prasp_res->_RASPEventCB[RaspID];

        memset(&cb_info, 0x0, sizeof(cb_info));

        if(u32Events & RASP_INT_STR2MIU)
        {
            printf("Str2MIU INT, _rasp_writeptr = %x\n",_rasp_writeptr);
            cb_info.eEvent = NDSRASP_CB_STR2RAM;
            cb_info.index  = 1;
            cb_info.write_ptr = _rasp_writeptr;
            if(_ndsrasp_cb != NULL)
            {
                _ndsrasp_cb(RaspID, &cb_info);
            }
        }

        if(u32Events & RASP_INT_PVR2MIU)
        {
            printf("PVR2MIU INT\n");
        }

        if(u32Events & RASP_INT_PAYLD2MIU)
        {
            printf("Payload INT, _rasp_payloadptr = %x\n",_rasp_payloadptr);
            cb_info.eEvent = NDSRASP_CB_PAYLOAD2RAM;
            cb_info.index  = 1;
            cb_info.write_ptr = _rasp_payloadptr;
            if(_ndsrasp_cb != NULL)
            {
                _ndsrasp_cb(RaspID, &cb_info);
            }
        }

        if( u32Events & (RASP_INT_TIME_WATER_MARK | RASP_INT_EVENT_WATER_MARK) )
        {
            printf("Event TIME/EVENT WATER MARK Int\n");
			printf("EventReadIndex now [0x%x]\n", (MS_U32)EventReadIndex[RaspID]);

			if (EventReadIndex[RaspID] < EventTmpIndex[RaspID])
			{
				u16EventQueued = (MS_U16)(EventTmpIndex[RaspID] - EventReadIndex[RaspID]);
			}
			else if (EventReadIndex[RaspID] > EventTmpIndex[RaspID])
			{
				u16EventQueued = (MS_U16)(RASP_EVENT_FIFO_DEPTH - EventReadIndex[RaspID] + EventTmpIndex[RaspID]);
			}
			else
			{	printf("Event SW FIFO is not updated\n");	}
			printf("u16EventQueued [0x%x]\n", (MS_U32)u16EventQueued);

			EventReadIndex[RaspID] ++;
            if(NULL != _Event_CallBackFun)
            {
    			_Event_CallBackFun(RaspID, _EventDataArray[RaspID], u16EventQueued, EventReadIndex[RaspID]);
            }
			EventReadIndex[RaspID] += (u16EventQueued-1);

			printf("EventReadIndex then [0x%x]\n", (MS_U32)EventReadIndex[RaspID]);
			#if 0 //TODO
			for(i=0;i<EventNumber;i++)
            {
                printf("EventNumber = %x\n",i);
                printf("EventDescr = %x, PktPID = %x, PktNum = %x, PktTimer = %x, PktPCR = %x) \n",_EventDataArray[RaspID][i].Event_Descriptor, \
                    _EventDataArray[RaspID][i].Pid, _EventDataArray[RaspID][i].PacketNum,_EventDataArray[RaspID][i].Timer,_EventDataArray[RaspID][i].PCR);
            }
			#endif

        }

        if(u32Events & RASP_INT_ECM2MIU)
        {
            printf("ECM 2 MIU Int\n");
        }

        if(u32Events & RASP_INT_ECM_PKT_RDY)
        {
            printf("ECM Packet Ready, EcmPktNumer = %x, EcmWritePtr = %x, EcmTimeStamp = %x, EcmIntStatus = %x\n",EcmPktNumer, EcmWritePtr, EcmTimeStamp, EcmIntStatus);
        }

        if(u32Events & RASP_INT_EFRAME_RD_OVF)
        {
            printf("RASP_INT_EFRAME_RD_OVF \n");
        }

        if(u32Events & RASP_INT_EFRAME_WT_OVF)
        {
            printf("RASP_INT_EFRAME_WT_OVF \n");
        }

        MsOS_DelayTask(1);

    }// while(1)
}

#if 0	//TEST
#ifdef MSOS_TYPE_LINUX_KERNEL
static irqreturn_t _NDS_RASP_Isr2(int irq, void *dev_id)
#else
static void _NDS_RASP_Isr2(InterruptNum eIntNum)
#endif

{
#if 0
    MS_U8               u8EcmReg;
    MS_U8               ecm_polarity;
    MS_U32              i;
    MS_U32              u32OvfEvents = 0;
#endif
    MS_U32 IntStatus;
    MS_U32 i;
    MS_U32 RaspID=0;

    MsOS_DisableInterrupt(E_INT_IRQ_RASP);



        IntStatus = (MS_U32)HAL_NDSRASP_INT_GetHW(RaspID);
		printf("____Isr #2____[%08X]\n", IntStatus);

        if (IntStatus & RASP_INT_ECM_PKT_RDY)
        {
            //NDS_RASP_DBG("RASP_INT_ECM_PKT_RDY\n");
            EcmPktNumer = HAL_NDSRASP_GetEcmPktNum(RaspID);
			printf("ECM Packet Number is [0x%x]\n", EcmPktNumer);
			//printf("ECM Interrupt state = %X\n", HAL_NDSRASP_SetECM_GetHW(RaspID));
			HAL_NDSRASP_SetECM_ResetHW(RaspID, 0xFFFF);
			//EcmWritePtr = HAL_NDSRASP_GetECMWritePtr(RaspID);
			//printf("ECM EcmWritePtr is [0x%x]\n", EcmWritePtr);

			MsOS_ReleaseSemaphore(_nds_rasp_eventid);
        #if 0
            EcmWritePtr = HAL_NDSRASP_GetECMWritePtr(EngId);
            EcmTimeStamp= HAL_NDSRASP_GetECMTimeStamp(EngId);
            //NDS_RASP_DBG("HAL_NDSRASP_SetECM_GetHW = %X\n", HAL_NDSRASP_SetECM_GetHW(0));

            EcmIntStatus = HAL_NDSRASP_SetECM_GetHW(EngId);
            //HAL_NDSRASP_SetECM_ResetHW(EngId, EcmIntStatus);


            if (pEcmData[0] != 0x47)
            {
                continue; //_NDS_ASSERT_;
            }
            if (pEcmData[5] == 0x80)
            {
                ecm_polarity = 0; // EVEN
            }
            else if (pEcmData[5] == 0x81)
            {
                ecm_polarity = 1; // ODD
            }
            else
            {
                NDS_ASSERT(FALSE, , "[%s]-[%d]\n", __FUNCTION__, __LINE__);
            }


            //need to copy to ecm buffer to keep buffer for future usage.....
            //memcpy(_u8RaspEcmBuf,
        #endif
        }

		if(IntStatus & RASP_INT_EFRAME_RD_OVF)
        {
        }

        if(IntStatus & RASP_INT_EFRAME_WT_OVF)
        {
        }

        if(IntStatus & RASP_INT_PAYLD2MIU)
        {
            _rasp_payloadptr = HAL_NDSRASP_GetPayloadWritePtr(RaspID);
        }
		if(IntStatus & RASP_INT_EVENT_WATER_MARK)
			;
		if(IntStatus & RASP_INT_TIME_WATER_MARK)
        	;
		if(IntStatus & RASP_INT_STR2MIU)
        {
            _rasp_writeptr = HAL_NDSRASP_GetWritePtr(RaspID);
        }
        if(IntStatus & RASP_INT_PAYLD2MIU)
        {
            _rasp_payloadptr = HAL_NDSRASP_GetPayloadWritePtr(RaspID);
        }

        //HAL_NDSRASP_INT_ClrHW(RaspID , IntStatus[RaspID]);
        HAL_NDSRASP_INT_ClrHW(RaspID , 0xFFFF);

		//MsOS_SetEvent(_nds_rasp_eventid, RaspID<<16 | (IntStatus[RaspID] ) );




    MsOS_EnableInterrupt(E_INT_IRQ_RASP);
}
#endif
#if 0
static void _NDS_RASP_IsrTask2(void)
{

    MS_U32 u32Events=0;
    MS_U32 RaspID;


    printf("ISR task222\n");

    while(1)
    {
        //MsOS_WaitEvent(_nds_rasp_eventid, NDS_RASP_EVENT_ALL, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
		MsOS_ObtainSemaphore(_nds_rasp_eventid, MSOS_WAIT_FOREVER);

        printf("[RASP]_NDS_RASP_IsrTask2222!!----- u32Events = %x \n",u32Events);
        //printf("EventLog = %x, EventDescr = %x, EventNumber = %x\n",EventLog, EventDescr, EventNumber);

		#if 0
        RaspID = (u32Events>>16);
        if(RaspID >= RASP_NUM)
        {
            DRVRASP_DBG(RASP_DBGLV_ERR,"invalid Rasp ID\n");
            continue;
        }

        printf("Rasp ID = %x\n",RaspID);

        if(u32Events & RASP_INT_ECM_PKT_RDY)
        {
			printf("ECM Packet Ready, EcmPktNumer = %x, EcmWritePtr = %x, EcmTimeStamp = %x, EcmIntStatus = %x\n",EcmPktNumer, EcmWritePtr, EcmTimeStamp, EcmIntStatus);
		}
		#endif
    }
}
#endif

static MS_BOOL _RASP_InitResource(RASP_RESOURCE_PRIVATE* presource)
{
    if(presource == NULL)
        return FALSE;

    presource->_s32RASPMutexId = -1;
    presource->_bSuspend = FALSE;
    presource->_bRASPInitReady = FALSE;
    presource->_u32LastErr = TRUE;

    return TRUE;
}

static void _NDS_RASP_SwStateReset(void)
{
    MS_U32 _u32RASPnum = 0;
    MS_U32 i = 0;
    printf("%s\n",__FUNCTION__);

    for (_u32RASPnum = 0 ; _u32RASPnum < RASP_NUM ; _u32RASPnum++)
    {
        for (i = RASP_PIDFLT_START_NUM; i < RASP_PIDFLT_END_NUM; i++)
        {
            _prasp_res->_RASPState[_u32RASPnum][i]._RASP_init = 0;
            _prasp_res->_RASPState[_u32RASPnum][i]._RASP_FltState = E_DRVRASP_FLT_STATE_FREE;
            _prasp_res->_RASPState[_u32RASPnum][i]._RASP_ECMFltState = E_DRVRASP_FLT_STATE_FREE;
        }
    }
}

static void _NDS_RASP_RegStateReset(void)
{
    printf("%s\n",__FUNCTION__);

    MS_U16 _u16fltnum = 0;
    MS_U32 _u32RASPnum = 0;
    for (_u32RASPnum = 0 ; _u32RASPnum < RASP_NUM ; _u32RASPnum++)
    {
        HAL_RASP_SetPVRDstBuf(_u32RASPnum,0,0,0,0);
        for (_u16fltnum = 0 ; _u16fltnum < RASP_PIDFLT_NUM ; _u16fltnum++)
        {
            HAL_NDSRASP_SetPidflt(_u32RASPnum, _u16fltnum, RASP_PIDFLT_DEF);
        }

        for (_u16fltnum = 0 ; _u16fltnum < RASP_EVENT_NUM ; _u16fltnum++)
        {
            HAL_NDSRASP_SetEventMask(_u32RASPnum, _u16fltnum, ~RASP_BYPASS_MASK);
            HAL_NDSRASP_SetPayloadMask(_u32RASPnum, _u16fltnum, ~RASP_BYPASS_MASK);
        }
    }
}

static void _RASP_Close(void)
{
    MS_U8 u8ii;

    for(u8ii = 0; u8ii < RASP_NUM; u8ii++)
    {
        HAL_NDSRASP_Exit(u8ii);
    }

}

// General API
//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 NDS_RASP_InitLibResource(void* pResMemAddr)
{
    if(pResMemAddr == 0)
        return FALSE;

    _prasp_res = (RASP_RESOURCE_PRIVATE*)pResMemAddr;

    if(_prasp_res->_bRASPInitReady != TRUE)
    {
        if(_RASP_InitResource(_prasp_res) == FALSE)
        {
            _prasp_res->_u32LastErr = FALSE;
        }
        else
            _prasp_res->_u32LastErr = TRUE;
    }

    return (_prasp_res->_u32LastErr);
}

//--------------------------------------------------------------------------------------------------
/// Initialize RASP driver and RASP engine
/// @return MS_U32
/// @note
/// It should be called before calling any other RASP DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//--------------------------------------------------------------------------------------------------
MS_U32 NDS_RASP_Init(void)
{
	MS_U32 bRet = TRUE;
	MS_U8 u8ii;

	DRVRASP_DBG(RASP_DBGLV_INFO, "\n");

    if(_bRASPIsInit == FALSE)
    {
        MS_VIRT u32Bank;
        MS_PHY u32BankSize;
        printf("%s\n",__FUNCTION__);
        if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_PM))
        {
        	DRVRASP_DBG(RASP_DBGLV_ERR,"MDrv_MMIO_GetBASE fail\n");
        	MS_ASSERT(0);
        }

        printf("u32Bank = %X\n",u32Bank);
        HAL_NDSRASP_SetBank(u32Bank);

        for(u8ii = 0; u8ii < RASP_NUM; u8ii++)
        {
        	_prasp_res->_RASPEvtNotify[u8ii] = E_DRVRASP_EVENT_DATA_INIT;
        	_prasp_res->_RASPCallback[u8ii] = NULL;

        	HAL_NDSRASP_PowerCtrl(u8ii, ENABLE);
        	HAL_NDSRASP_Init(u8ii);
        }
        _NDS_RASP_SwStateReset();

        if (-1 == _prasp_res->_s32RASPMutexId)
        {
        	_prasp_res->_s32RASPMutexId = MsOS_CreateMutex(E_MSOS_FIFO , "RASP_Mutex" , MSOS_PROCESS_SHARED);
        }
        else
        {
        	DRVRASP_DBG(RASP_DBGLV_ERR,"Create _s32RASPMutexId more than once\n");
        }

        _u32RaspEventFlag = 0;
        _u32RaspOvfEventFlag = 0;
        _RaspEventIdx = 0;
        _RaspReadIdx = 0;
        _RaspNumEventsTotal=0;


        if (-1 == _nds_rasp_eventid)
        {
           _nds_rasp_eventid =	MsOS_CreateEventGroup("NDS_RASP_Event");
        }

        if (-1 == _nds_rasp_taskid)
        {
           _nds_rasp_taskid = MsOS_CreateTask(	(TaskEntry) _NDS_RASP_IsrTask,
                                                (MS_U32)NULL,
        										(TaskPriority) E_TASK_PRI_HIGHEST,
        										TRUE,
        										_nds_rasp_stack,
        										NDS_RASP_TASK_STACK_SIZE,
        										"NDS_RASP_IsrTask");
        }

        EMMRASP_IntAttach(RASP_ENGINE, TRUE);
        HAL_NDSRASP_INT_Enable(0 ,0xffff);
        _bRASPIsInit = TRUE;

        _RASP_ENTRY();
        _prasp_res->_bRASPInitReady = TRUE;
        _RASP_RETURN(bRet);
    }


    return bRet;
}

//--------------------------------------------------------------------------------------------------
/// Poweroff RASP driver and RASP engine
/// @return MS_U32
/// @note
//--------------------------------------------------------------------------------------------------
MS_U32 NDS_RASP_Reset(void)
{
    _RASP_ENTRY();
    _NDS_RASP_SwStateReset();
    _NDS_RASP_RegStateReset();
    _RASP_RETURN(TRUE);
}

//--------------------------------------------------------------------------------------------------
/// Close RASP driver and reset software/register state
/// @return MS_U32
/// @note
//--------------------------------------------------------------------------------------------------
MS_U32 NDS_RASP_Exit(void)
{
    if (_prasp_res->_s32RASPMutexId != -1)
    {
        _RASP_ENTRY();
    }

    if(_prasp_res->_bRASPInitReady == TRUE)
    {
        _NDS_RASP_SwStateReset();
        _NDS_RASP_RegStateReset();
        _RASP_Close();
        _prasp_res->_bRASPInitReady = FALSE;
    }

    if (_prasp_res->_s32RASPMutexId != -1)
    {
        MsOS_ReleaseMutex(_prasp_res->_s32RASPMutexId);
        MsOS_DeleteMutex(_prasp_res->_s32RASPMutexId);
        _prasp_res->_s32RASPMutexId = -1;
    }

    return  TRUE ;
}

#if 0
MS_U32 NDS_RASP_SetBuffer(MS_U32 u32RASPEng, MS_PHYADDR u32BufStart0, MS_PHYADDR u32BufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, StartAddr0 = %x, BufSize0 = %x, StartAddr1 = %x, BufSize1 = %x\n", u32RASPEng,u32BufStart0,u32BufSize0,u32BufStart1,u32BufSize1);

    _NDS_RASP_ENTRY();
    _NDS_RASP_CHECK(u32RASPEng);

    //check buffer size.....
    HAL_RASP_SetPVRDstBuf(u32RASPEng, u32BufStart0,(u32BufStart0 + u32BufSize0), u32BufStart1, (u32BufStart1 + u32BufSize1));
    _NDS_RASP_RETURN(TRUE);
}
#endif


MS_U32 NDS_RASP_RaspEngStart(MS_U32 u32RASPEng, DrvRASP_CtrlMode eCtrlMode, MS_BOOL bEnable)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, eCtrlMode = %d, bEnable = %d\n",u32RASPEng, eCtrlMode, bEnable);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if (!bEnable)
    {
        switch (eCtrlMode)
        {
            case E_DRVRASP_MODE_PAYLOAD:
            {
                HAL_NDSRASP_Payload_Enable(u32RASPEng , FALSE);
                break;
            }
            case E_DRVRASP_MODE_ECM:
            {
                HAL_NDSRASP_ECM_Enable(u32RASPEng , FALSE);
                break;
            }
            default :
                break;
        }

    }
    else
    {
        switch (eCtrlMode)
        {
            case E_DRVRASP_MODE_PAYLOAD:
            {
                HAL_NDSRASP_Payload_Enable(u32RASPEng , TRUE);
                break;
            }
            case E_DRVRASP_MODE_ECM:
            {
                HAL_NDSRASP_ECM_Enable(u32RASPEng , TRUE);
                break;
            }
            default :
                break;
        }

    }
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_PvrEngStart(MS_U32 u32RASPEng, DrvRASP_RecMode eRecMode, MS_BOOL bEnable)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, eRecMode = %d, bEnable = %d\n",u32RASPEng, eRecMode, bEnable);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if (!bEnable)
    {
        switch (eRecMode)
        {
            case E_DRVRASP_REC_MODE_PID:
            {
                HAL_NDSRASP_Stop(u32RASPEng);
                break;
            }
            case E_DRVRASP_REC_MODE_ALL:
            {
                HAL_NDSRASP_Stop(u32RASPEng);
                break;
            }

            default :
                break;
        }

    }
    else
    {
        switch (eRecMode)
        {
            case E_DRVRASP_REC_MODE_PID:
            {
                HAL_NDSRASP_Rec_PID(u32RASPEng, TRUE);
                HAL_NDSRASP_Pvr_Enable(u32RASPEng);
                break;
            }
            case E_DRVRASP_REC_MODE_ALL:
            {
                HAL_NDSRASP_Rec_PID(u32RASPEng, FALSE);
                HAL_NDSRASP_Pvr_Enable(u32RASPEng);
                break;
            }

            default :
                break;
        }

    }
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_AdvEnable(MS_U32 u32RASPEng, DrvRASP_RecMode eRecMode, MS_BOOL bEn)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, eRecMode = %d, bEn = %d\n",u32RASPEng, eRecMode, bEn);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if (!bEn)
    {
        switch (eRecMode)
        {
            case E_DRVRASP_MODE_PAYLOAD:
            {
                HAL_NDSRASP_Payload_Enable(u32RASPEng , FALSE);
                break;
            }
            case E_DRVRASP_MODE_ECM:
            {
                HAL_NDSRASP_ECM_Enable(u32RASPEng , FALSE);
                break;
            }
            default :
                break;
        }

    }
    else
    {
        switch (eRecMode)
        {
            case E_DRVRASP_MODE_PAYLOAD:
            {
                HAL_NDSRASP_Payload_Enable(u32RASPEng , TRUE);
                break;
            }
            case E_DRVRASP_MODE_ECM:
            {
                HAL_NDSRASP_ECM_Enable(u32RASPEng , TRUE);
                break;
            }
            default :
                break;
        }

    }
    _RASP_RETURN(TRUE);
}



MS_U32 NDS_RASP_Pause(MS_U32 u32RASPEng, MS_BOOL bPause)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, bPause = %d\n",u32RASPEng, bPause);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_Pause(u32RASPEng,bPause);


    _RASP_RETURN(TRUE);
}


MS_U32 NDS_RASP_GetWriteAddr(MS_U32 u32RASPEng, MS_PHYADDR *pu32WriteAddr)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);
    *pu32WriteAddr = HAL_NDSRASP_GetWritePtr(u32RASPEng);
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_GetPayloadWriteAddr(MS_U32 u32RASPEng, MS_PHYADDR *pu32WriteAddr)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);
    *pu32WriteAddr = HAL_NDSRASP_GetPayloadWritePtr(u32RASPEng);
    _RASP_RETURN(TRUE);
}


MS_U32 NDS_RASP_SetNotify(MS_U32 u32RASPEng, DrvRASP_Event eEvents, P_DrvRASP_EvtCallback pfCallback)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "eEvents = %x\n",eEvents);
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if (pfCallback)
    {
        _prasp_res->_RASPEvtNotify[u32RASPEng] = eEvents;
        _prasp_res->_RASPCallback[u32RASPEng] = pfCallback;
    }
    else
    {
        _prasp_res->_RASPEvtNotify[u32RASPEng] = E_DRVRASP_EVENT_DATA_INIT;
        _prasp_res->_RASPCallback[u32RASPEng] = NULL;
    }
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_Set_EventNotify(MS_U32 u32RASPEng, P_DrvRASP_EventCB CallBackFun)
{

    //DRVRASP_DBG(RASP_DBGLV_INFO, "eEvents = %x\n",eEvents);
    DRVRASP_DBG(RASP_DBGLV_INFO, "\n");
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if (CallBackFun)
    {
        _prasp_res->_RASPEventCB[u32RASPEng] = CallBackFun;
    }

    _RASP_RETURN(TRUE);
}


MS_U32 NDS_RASP_SetCallBack(MS_U32 u32RASPEng, P_NDSRASP_Callback pfCallback)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x\n",u32RASPEng);
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if(pfCallback == NULL)
    {
        _RASP_RETURN(FALSE);
    }
    _ndsrasp_cb = pfCallback;
    _RASP_RETURN(TRUE);
}


MS_U32 NDS_RASP_SetPacketMode(MS_U32 u32RASPEng, NDSRASP_RecType eRecType, RASP_OUTSIZE_e eOutSize)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, eRecType = %d, eOutSize = %d\n",u32RASPEng, eRecType, eOutSize);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    MS_BOOL b192 = TRUE;

    if(eOutSize == RASP_OUT_188)
        b192 = FALSE;

    switch (eRecType)
    {
        case STR2MIU:
            HAL_NDSRASP_SetStrPacketMode(u32RASPEng, b192);
            break;
		case PAYLOAD2MIU:
			HAL_NDSRASP_SetPayloadPacketMode(u32RASPEng, !b192);
			break;
		case ECM2MIU:
			HAL_NDSRASP_SetECMPacketMode(u32RASPEng, !b192);
			break;

        default:
			DRVRASP_DBG(RASP_DBGLV_ERR, "Wrong eRecType\n");
            break;
    }
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_SetBufInfo(MS_U32 u32RASPEng, NDSRASP_RecType eRecType, MS_PHYADDR u32BufStart0, MS_PHYADDR u32BufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, eRecType = %d, StartAddr0 = %x, BufSize0 = %x, StartAddr1 = %x, BufSize1 = %x\n",u32RASPEng,eRecType,u32BufStart0,u32BufSize0,u32BufStart1,u32BufSize1);
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    switch (eRecType)
    {
        case STR2MIU:
            HAL_RASP_SetPVRDstBuf(u32RASPEng, u32BufStart0,(u32BufStart0 + u32BufSize0), u32BufStart1, (u32BufStart1 + u32BufSize1));
            break;

        case PAYLOAD2MIU:
            HAL_RASP_SetPayloadDstBuf(u32RASPEng, 	u32BufStart0,
													(u32BufStart0 + u32BufSize0),
													(u32BufStart0 + 2*u32BufSize0),
													u32BufStart1,
													(u32BufStart1 + u32BufSize1),
													(u32BufStart1 + 2*u32BufSize1)
								);
            break;

        case ECM2MIU:
            HAL_RASP_SetECMDstBuf(u32RASPEng, 		(u32BufStart0)	,
													(u32BufStart0 + u32BufSize0),
													(u32BufStart0 + 2*u32BufSize0)
								);
            break;
        default:
            break;
    }

    _RASP_RETURN(TRUE);
}


//this function needs to check pid filter does open this pid or not....
MS_U32 NDS_RASP_SetECMPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u16Fltid = %x, u16Pid = %x\n",u32RASPEng, u16Fltid, u16Pid);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_SetEcmPidflt(u32RASPEng,u16Fltid,u16Pid);

    //MS_U16 ReadBackPid;
    //HAL_NDSRASP_ReadEcmPidflt(u32RASPEng,u16Fltid,&ReadBackPid);
    //DRVRASP_DBG(RASP_DBGLV_INFO, "Read Back ECM Pid = %x\n",ReadBackPid);
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_SetRecordTimeStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u32Stamp = %x\n",u32RASPEng, u32Stamp);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_SetPVRTimeStamp(u32RASPEng, u32Stamp);

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_GetRecordTimeStamp(MS_U32 u32RASPEng, MS_U32* u32Stamp)
{
    //_RASP_CHECK(u32RASPEng);
    *u32Stamp = HAL_NDSRASP_GetPVRTimeStamp(u32RASPEng);
    return TRUE;
}

MS_U32 NDS_RASP_SetECMTimeStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u32Stamp = %x\n",u32RASPEng, u32Stamp);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_SetECMTimeStamp(u32RASPEng, u32Stamp);

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_GetECMTimeStamp(MS_U32 u32RASPEng, MS_U32* u32Stamp)
{
    //_RASP_CHECK(u32RASPEng);
    *u32Stamp = HAL_NDSRASP_GetECMTimeStamp(u32RASPEng);
    return TRUE;
}

MS_U32 NDS_RASP_TimeStampSelRecordStampSrc(MS_U32 u32RASPEng, MS_BOOL bLocal)
{
//not implement
    return TRUE;
}

MS_U32 NDS_RASP_GetCurrentPktStatus(MS_U32 u32RASPEng, MS_U32* u32PktStamp, MS_U32* u32PktNumber)
{
    *u32PktStamp = HAL_NDSRASP_GetPktTimer(u32RASPEng);
    *u32PktNumber = HAL_NDSRASP_GetPktNum(u32RASPEng);
    return TRUE;
}

#if 0	//Migrate to RASP_FlowSet
MS_U32 NDS_RASP_SetTSIF(MS_U32 u32RASPEng, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP)
{
    _RASP_ENTRY();

    _RASP_CHECK(u32RASPEng);
    HAL_NDSRASP_SetTSIF(u32RASPEng, bPara, bExtSync, bDataSWP);
    _RASP_RETURN(TRUE);
}
#endif

MS_U32 NDS_RASP_GetTSIFStatus(MS_U32 u32RASPEng, MS_BOOL* pbExtSyc, MS_BOOL* pbParl, MS_BOOL *pbDataSWP)
{
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);
    //        HAL_NDSRASP_GetTSIF(u32RASPEng, pbParl, pbExtSyc, pbDataSWP);
    _RASP_RETURN(TRUE);
}

RASP_LIVEIN_SRC_e NDS_RASP_LiveSrcConvert(RASP_FLOW_INPUT eSource)
{
    RASP_LIVEIN_SRC_e eSrc = RASP_SRC_TS0;

    switch(eSource)
    {
        case RASP_FLOW_INPUT_DEMOD0:
            eSrc = RASP_SRC_DEMOD0;
            break;

        case RASP_FLOW_INPUT_DEMOD1:
            eSrc = RASP_SRC_DEMOD1;
            break;

        case RASP_FLOW_INPUT_TS0:
            eSrc = RASP_SRC_TS0;
            break;

        case RASP_FLOW_INPUT_TS1:
            eSrc = RASP_SRC_TS1;
            break;

        case RASP_FLOW_INPUT_TS2:
            eSrc = RASP_SRC_TS2;
            break;

        case RASP_FLOW_INPUT_TS3:
            eSrc = RASP_SRC_TS3;
            break;

        case RASP_FLOW_INPUT_TS4:
            eSrc = RASP_SRC_TS4;
            break;

        case RASP_FLOW_INPUT_TS5:
            eSrc = RASP_SRC_TS5;
            break;

        case RASP_FLOW_INPUT_TS6:
            eSrc = RASP_SRC_TS6;
            break;

        default:
            eSrc = RASP_SRC_TS0;
            break;
    }


    return eSrc;
}

MS_BOOL NDS_RASP_FlowSet(MS_U32 rasp_eng, RASP_FLOW_INPUT eSource, RASP_FLOW_OUTPUT eDest, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP, RASP_INPUTSIZE_e eMode )
{
	MS_U8 u8pLen=188;


	if (eMode==RASP_IN_188){
		u8pLen = 188;
    }
	else if (eMode==RASP_IN_192){
		u8pLen = 192;
    }

	DRVRASP_DBG(RASP_DBGLV_INFO, "Input packet Size = %x\n", u8pLen);
    DRVRASP_DBG(RASP_DBGLV_DEBUG, "rasp_eng = %x, eSource = %d, bPara = %d, eMode = %d\n", rasp_eng, eSource, bPara, eMode );

	if (eSource==RASP_FLOW_INPUT_MEM)
	{
		HAL_NDSRASP_SetFileinConfig(rasp_eng, TRUE, 6);				// filein en, blocklevel
		HAL_NDSRASP_FileIn_Init(rasp_eng, u8pLen);					//filein bank. reset, chunk_size, auto flush
	}
    else    //LIVEIN
	{
		HAL_NDSRASP_SetTSIF(rasp_eng, bPara, bExtSync, bDataSWP); 	// rasp_ctrl0, TSIF
		HAL_NDSRASP_SetFileinConfig(rasp_eng, FALSE, 0);			// filein dis, blocklevel
		HAL_NDSRASP_LiveIn_Init(rasp_eng, u8pLen);					//filein bank. reset, chunk_size, auto flush

        HAL_NDSRASP_Livein_Config(rasp_eng, NDS_RASP_LiveSrcConvert(eSource));
	}
    return TRUE;
}



MS_U32 NDS_RASP_AllocFlt(MS_U32 u32RASPEng, MS_U32 *pu16PidFltId)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);
    MS_U16 i = 0;

    for (i = RASP_PIDFLT_START_NUM ; i < RASP_PIDFLT_END_NUM ; i++)
    {
        if (_prasp_res->_RASPState[u32RASPEng][i]._RASP_FltState == E_DRVRASP_FLT_STATE_FREE)
        {
            _prasp_res->_RASPState[u32RASPEng][i]._RASP_FltState = E_DRVRASP_FLT_STATE_ALLOC;
            *pu16PidFltId = i;
            break;
        }
    }

    if (i == RASP_PIDFLT_END_NUM)
    {
        //RASP_ASSERT(0, OSAL_TSP_Print("[TSP_ERROR][%06d] No free fitler found 0x%02lx\n", __LINE__, i));
        _RASP_RETURN(FALSE);
    }

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_FreeFlt(MS_U32 u32RASPEng, MS_U32 u32PidFltId)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u32PidFltId = %x\n",u32RASPEng,u32PidFltId);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if(u32PidFltId >= RASP_PIDFLT_END_NUM)
    {
        _RASP_RETURN(FALSE);
    }

    HAL_NDSRASP_SetPidflt(u32RASPEng, u32PidFltId, DRVRASP_PID_NULL);
    _prasp_res->_RASPState[u32RASPEng][u32PidFltId]._RASP_FltState = E_DRVRASP_FLT_STATE_FREE;
    _RASP_RETURN(TRUE);
}


MS_U32 NDS_RASP_AllocECMFlt(MS_U32 u32RASPEng, MS_U16 *pu16ECMFltId)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);
    MS_U16 i = 0;

    for (i = RASP_ECMFLT_START_NUM ; i < RASP_ECMFLT_END_NUM ; i++)
    {
        if (_prasp_res->_RASPState[u32RASPEng][i]._RASP_ECMFltState == E_DRVRASP_FLT_STATE_FREE)
        {
            _prasp_res->_RASPState[u32RASPEng][i]._RASP_ECMFltState = E_DRVRASP_FLT_STATE_ALLOC;
            *pu16ECMFltId = i;
            break;
        }
    }

    if (i == RASP_ECMFLT_END_NUM)
    {
        DRVRASP_DBG(RASP_DBGLV_ERR, "[ERROR][%d] No free fitler found 0x%x\n", __LINE__, (MS_U32)i);
        _RASP_RETURN(FALSE);
    }

    DRVRASP_DBG(RASP_DBGLV_INFO, "ECM Flt ID = %x\n",(MS_U32)*pu16ECMFltId);

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_FreeECMFlt(MS_U32 u32RASPEng, MS_U16 u16ECMFltId)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u32ECMFltId = %x\n",u32RASPEng,(MS_U32)u16ECMFltId);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if(u16ECMFltId >= RASP_ECMFLT_END_NUM)
    {
        _RASP_RETURN(FALSE);
    }

    HAL_NDSRASP_SetPidflt(u32RASPEng, u16ECMFltId, DRVRASP_PID_NULL);
    _prasp_res->_RASPState[u32RASPEng][u16ECMFltId]._RASP_ECMFltState = E_DRVRASP_FLT_STATE_FREE;
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_SetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u16Fltid = %x, u16Pid = %x\n",u32RASPEng,u16Fltid,u16Pid);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_SetPidflt(u32RASPEng, u16Fltid, u16Pid);
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_GetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 *pu16Pid)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u16Fltid = %x\n",u32RASPEng,u16Fltid);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_GetPidflt(u32RASPEng, u16Fltid, pu16Pid);

    DRVRASP_DBG(RASP_DBGLV_INFO, "Pid = %x\n",*pu16Pid);
     _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_GetFileinEnable(MS_U32 u32RASPEng, MS_BOOL* bEn)
{
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    *bEn = HAL_NDSRASP_GetFileinStart(u32RASPEng);

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_GetCap(NDSRASP_Cap eCap, void* pOutput)
{
	*((MS_U32*)pOutput) = 0;

    switch (eCap)
    {
        case E_NDSRASP_CAP_RESOURCE_SIZE:
            *((MS_U32*)pOutput) = sizeof(RASP_RESOURCE_PRIVATE);
            return TRUE;
        case E_NDSRASP_CAP_FILTER_NUM:
        case E_NDSRASP_CAP_FILTER_PVR_NUM:
            *((MS_U32*)pOutput) = RASP_PIDFLT_NUM_ALL;
            return TRUE;
        case E_NDSRASP_CAP_PVR_ALIGN:
            *((MS_U32*)pOutput) = 16;
            return TRUE;
        case E_NDSRASP_CAP_RASP_NUM:
            *((MS_U32*)pOutput) = RASP_NUM;
            return TRUE;
		case E_NDSRASP_CAP_EVENT_FIFO_DEPTH:
			return RASP_EVENT_FIFO_DEPTH;
		case E_NDSRASP_CAP_EVENT_NUM:
			return RASP_EVENT_NUM;
        default:
            return DRVRASP_INVALID_PARAM;
    }
}


MS_U32 NDS_RASP_SetEventMask(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Event)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u16Flt = %x, u32Event = %x\n",u32RASPEng, u16Flt, u32Event);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_SetEventMask(u32RASPEng, u16Flt, u32Event);

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_SetWatermark(MS_U32 u32RASPEng, WATERMARK_TYPE WType, MS_BOOL bEnable)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, WType = %d, bEnable = %d\n",u32RASPEng, WType, bEnable);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    if(WType == EVENT_WATERMARK)
    {
        //HAL_NDSRASP_SetEventWaterMark(u32RASPEng, 0xf);		//TPE team request, separate API
        HAL_NDSRASP_EVENT_EventWM_Enable(u32RASPEng, bEnable);
    }
    else if(WType == TIMER_WATERMARK)
    {
        //HAL_NDSRASP_SetTimerWaterMark(u32RASPEng, 2);		//TPE team request, separate API
        HAL_NDSRASP_EVENT_TimeWM_Enable(u32RASPEng, bEnable);
    }

    //HAL_NDSRASP_SetEventMask(u32RASPEng, u16Flt, u32Event);

    _RASP_RETURN(TRUE);
}

MS_BOOL NDS_RASP_SetEvent_Threshold(MS_U32 u32RASPEng, MS_U8 u8Threshold /*Maximum value is 31*/)
{
	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng [%x], Event Threshold [%x]\n", u32RASPEng, u8Threshold);

	if (u8Threshold <= 31 )
	{
		return HAL_NDSRASP_EVENT_SetEventThreshold(u32RASPEng, (MS_U32)u8Threshold);
	}

	DRVRASP_DBG(RASP_DBGLV_ERR, "Threshold exceeds\n");
	return FALSE;

}

MS_BOOL NDS_RASP_SetTime_Timeout(MS_U32 u32RASPEng, TIME_WATERMARK_e timeout)
{
	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng [%x], Event Threshold [%x]\n", u32RASPEng, timeout);
	return HAL_NDSRASP_EVENT_SetTimerThreshold(u32RASPEng, (MS_U16)timeout );
}

MS_U32 NDS_RASP_SetPayloadMask(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Payload)
{
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_SetPayloadMask(u32RASPEng, u16Flt, u32Payload);

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_SetPayloadTimeStamp(MS_U32 u32RASPEng , MS_U32 u32TimeStamp)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x, u32TimeStamp = %x\n",u32RASPEng, u32TimeStamp);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_NDSRASP_SetPayloadTimeStamp(u32RASPEng,u32TimeStamp);
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_GetPayloadTimeStamp(MS_U32 u32RASPEng , MS_U32 *pu32TimeStamp)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    *pu32TimeStamp = HAL_NDSRASP_GetPayloadTimeStamp(u32RASPEng);
    DRVRASP_DBG(RASP_DBGLV_INFO, "TimeStamp = %x\n",*pu32TimeStamp);
    _RASP_RETURN(TRUE);
}

#if 0	//move to NDS_RASP_SetPacketMode
MS_U32 NDS_RASP_SetPayloadPacketMode(MS_U32 u32RASPEng, MS_BOOL bDisable192)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

	//If want to disable 192 mode--> TRUE
	//If want to enable  192 mode--> FALSE
	HAL_NDSRASP_SetPayloadPacketMode(u32RASPEng, bDisable192);

	_RASP_RETURN(TRUE);
}
#endif

MS_U32 NDS_RASP_SetDataSwap(MS_U32 u32RASPEng, MS_BOOL bEn)
{
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);
    HAL_NDSRASP_SetDataSwap(u32RASPEng, bEn);
    _RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_CallbackSize(MS_U32 u32RASPEng, MS_U32* pu32CallbackSize, MS_BOOL bSet)
{
    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
    if(bSet)
    {
        HAL_NDSRASP_SetCallbackSize(u32RASPEng, *pu32CallbackSize);
        if(*pu32CallbackSize < CALLBACK_SIZE_MIN)
        {
            HAL_NDSRASP_INT_Disable(u32RASPEng, RASP_INT_CALLBACK_BY_SIZE);
        }
        else
        {
            if(_prasp_res->_RASPCallback[u32RASPEng])
                HAL_NDSRASP_INT_Enable(u32RASPEng, RASP_INT_CALLBACK_BY_SIZE);
        }
    }
    else
        *pu32CallbackSize = HAL_NDSRASP_GetCallbackSize(u32RASPEng);
#endif

    _RASP_RETURN(TRUE);
}

MS_U32 NDS_PROC_RASP_PVR_SizeMet(MS_U32 u32RASPEng)
{
    static DrvRASP_Msg stMsg;
    _RASP_CHECK(u32RASPEng);
    if(_prasp_res->_RASPEvtNotify[u32RASPEng] && _prasp_res->_RASPCallback[u32RASPEng])
    {
        if(_prasp_res->_RASPEvtNotify[u32RASPEng] & E_DRVRASP_EVENT_CALLBACK_SIZE_MET)
        {
            stMsg.u8PVREngNum = u32RASPEng;
            _prasp_res->_RASPCallback[u32RASPEng](E_DRVRASP_EVENT_CALLBACK_SIZE_MET, &stMsg);
        }
    }
    return TRUE;
}

MS_U32 NDS_RASP_CallbackIntCheck(MS_U32 u32RASPEng, MS_BOOL* bInterrupted)
{//this func may be used in interrupt context , so watch out!!! Here's no synchronization mechanism.
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
    if(!bInterrupted)
        return FALSE;
     _RASP_CHECK(u32RASPEng);
    if(HAL_NDSRASP_INT_GetHW(u32RASPEng) & RASP_INT_CALLBACK_BY_SIZE)
        *bInterrupted = TRUE;
    else
        *bInterrupted = FALSE;
#endif
    return TRUE;
}

MS_U32 NDS_RASP_CallbackIntClr(MS_U32 u32RASPEng)
{//this func may be used in interrupt context , so watch out!!! Here's no synchronization mechanism.
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
     _RASP_CHECK(u32RASPEng);
    HAL_NDSRASP_INT_ClrHW(u32RASPEng, RASP_INT_CALLBACK_BY_SIZE);
#endif
    return TRUE;
}


MS_U32 NDS_RASP_SetFileIn_Config(MS_U32 RaspEng, MS_U32 StartAddr, MS_U32 FileInSize)
{
    DRVRASP_DBG(RASP_DBGLV_INFO,"RaspEng = %x, StartAddr = %x, FileInSize = %x\n",RaspEng,StartAddr,FileInSize);
    return HAL_RASP_SetFileIn_Config(RaspEng, StartAddr, FileInSize);
}

MS_U32 NDS_RASP_FileIn_Start(MS_U32 RaspEng)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);

    _RASP_RETURN(HAL_RASP_FileIn_Start(RaspEng));
}

MS_U32 NDS_RASP_SetFileIn_Timer(MS_U32 RaspEng, MS_U16 u16Timer)
{
    DRVRASP_DBG(RASP_DBGLV_INFO,"RaspEng = %x,  u16Timer = %x\n",RaspEng,u16Timer);
    return HAL_RASP_SetFileIn_Timer(RaspEng, u16Timer);
}

MS_U32 NDS_RASP_SetFileIn_PktSize(MS_U32 RaspEng, MS_U16 PktSize)
{
	//(Packet Size ¡V 1) for sync detection in TSIF1
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x,  PktSize = %x\n", RaspEng,PktSize);
    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);

    _RASP_RETURN(HAL_RASP_SetFileIn_PktSize(RaspEng, PktSize));
}

MS_BOOL NDS_RASP_IsFileIn_Done(MS_U32 RaspEng)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n", RaspEng);
    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);
    _RASP_RETURN(HAL_RASP_IsFileIn_Done(RaspEng));
}

MS_U32 NDS_RASP_FileIn_Flush(MS_U32 RaspEng)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n", RaspEng);

    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);

    _RASP_RETURN(HAL_RASP_FileIn_Flush(RaspEng));
}

#if 0 //Migrate to RASP_FlowSet
MS_U32 NDS_RASP_FileinInit(MS_U32 u32RASPEng)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

    HAL_RASP_FileIn_Init(u32RASPEng);

    _RASP_RETURN(TRUE);
}
#endif

//--------------------------------------------------------------------------------------------------
/// FileIn mode bypass timestamp.
/// If 192 byte Filein mode --> bypass=FALSE
/// If 188 byte Filein mode --> NO NEED CALL THIS API
/// @return
//--------------------------------------------------------------------------------------------------
MS_U32 NDS_RASP_FileIn_BypassTimeStamp (MS_U32 u32RASPEng, MS_BOOL bypass)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

	HAL_RASP_FileIn_BlockTimeStamp(u32RASPEng, !bypass);

	_RASP_RETURN(TRUE);
}

//--------------------------------------------------------------------------------------------------
/// FileIn mode bypass timestamp.
/// If lpcr2 (system 90k counter) is higher than stream timestamp, then packet can output. Otherwise no packet will output.
/// So, the one should set lpcr2 time initial value according to FileIn packet initial timestamp value
/// @return
//--------------------------------------------------------------------------------------------------
MS_U32 NDS_RASP_FileIn_SetPlaybackTimeStamp (MS_U32 u32RASPEng, MS_U32 u32Stamp)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

	HAL_RASP_FileIn_SetPlaybackTimeStamp(u32RASPEng, u32Stamp);

	_RASP_RETURN(TRUE);
}

MS_U32 NDS_RASP_FileIn_GetPlaybackTimeStamp (MS_U32 u32RASPEng)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",u32RASPEng);

    _RASP_ENTRY();
    _RASP_CHECK(u32RASPEng);

	_RASP_RETURN( HAL_RASP_FileIn_GetPlaybackTimeStamp(u32RASPEng) );
}

MS_BOOL NDS_RASP_FileIn_IsCMDQ_Full(MS_U32 RaspEng)
{
	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);
	_RASP_RETURN( HAL_RASP_FileIn_IsCMDQ_Full(RaspEng) );
}

MS_BOOL NDS_RASP_FileIn_IsCMDQ_Empty(MS_U32 RaspEng)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);

	_RASP_RETURN( HAL_RASP_FileIn_IsCMDQ_Empty(RaspEng) );
}

MS_BOOL NDS_RASP_FileIn_GetCmdQueueLevel(MS_U32 RaspEng, FILEIN_CMDQ_LEVEL* peCMDQLvl)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);
	MS_U8 CmdQLevel=0xFF;

	/*
	cmd_wr_level = 2'b00 //empty
	cmd_wr_level = 2'b01 // 25% full
	cmd_wr_level = 2'b10 // 50% full
	cmd_wr_level = 2'b11 // 75% full
	*/
	HAL_RASP_FileIn_GetCmdQueueLevel( RaspEng, &CmdQLevel);

	if (CmdQLevel!=0xFF)
	{
		*peCMDQLvl = (FILEIN_CMDQ_LEVEL)CmdQLevel;
		_RASP_RETURN( TRUE );
	}

	DRVRASP_DBG(RASP_DBGLV_ERR, "CmdQLevel value ERROR\n");
	_RASP_RETURN( FALSE );

}


MS_BOOL NDS_RASP_FileIn_GetEmptyNum(MS_U32 RaspEng, MS_U8* peCMDQCnt)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

    _RASP_ENTRY();
    _RASP_CHECK(RaspEng);
	MS_U8 CmdQCnt=0xFF;

	/*	empty cmd_wr_count		*/
	HAL_RASP_FileIn_GetEmptyNum( RaspEng, &CmdQCnt);

	if (CmdQCnt!=0xFF)
	{
		*peCMDQCnt = CmdQCnt;
		_RASP_RETURN( TRUE );
	}

	DRVRASP_DBG(RASP_DBGLV_ERR, "peCMDQCnt value ERROR\n");
	_RASP_RETURN( FALSE );

}

MS_U32 NDS_RASP_FileIn_Timer(MS_U32 RaspEng, MS_BOOL bEnFileInTimer, MS_U16 u16Timer)
{
	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

	_RASP_ENTRY();
	_RASP_CHECK(RaspEng);

	HAL_RASP_FileIn_Timer(RaspEng, bEnFileInTimer, u16Timer);

	_RASP_RETURN( TRUE );
}

MS_BOOL NDS_RASP_FileIn_Init_TimeStamp(MS_U32 RaspEng, MS_BOOL bResetPacketTimeStamp)
{
    DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

	_RASP_ENTRY();
	_RASP_CHECK(RaspEng);

	HAL_RASP_FileIn_Reset_TStamp(RaspEng, bResetPacketTimeStamp);

    _RASP_RETURN( TRUE );
}

MS_BOOL NDS_RASP_Reset_EventPktCounter(MS_U32 RaspEng)
{
	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

	_RASP_ENTRY();
	_RASP_CHECK(RaspEng);

	HAL_NDSRASP_EVENT_RstCounter(RaspEng);

	_RASP_RETURN( TRUE );
}

MS_BOOL NDS_RASP_Reset_EventPktTimer(MS_U32 RaspEng)
{
	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

	_RASP_ENTRY();
	_RASP_CHECK(RaspEng);

	HAL_NDSRASP_EVENT_RstTimer(RaspEng);

	_RASP_RETURN( TRUE );
}

//--------------------------------------------------------------------------------------------------
/// Get Event descriptor from FIFO, intended for polling mode
/// Otherwise use interrupt callback method
/// @return
//--------------------------------------------------------------------------------------------------
MS_BOOL NDS_RASP_GetEventDescriptor(MS_U32 RaspEng, NDS_RASP_EVENT *pEventDesc, MS_U32 u32ArraySize)
{
	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

	_RASP_ENTRY();
	_RASP_CHECK(RaspEng);

	if (pEventDesc)
	{
		HAL_NDSRASP_EVENT_GetEventDescriptor(RaspEng, (MS_U32 *)pEventDesc, u32ArraySize);
		_RASP_RETURN( TRUE );
	}
	else
	{
		DRVRASP_DBG(RASP_DBGLV_INFO, "NULL descriptor pointer\n");

	}

	_RASP_RETURN( FALSE );
}

//--------------------------------------------------------------------------------------------------
/// Get remaining event numbers in event FIFO
/// @return
//--------------------------------------------------------------------------------------------------
MS_U16 NDS_RASP_GetEventNumber(MS_U32 RaspEng)
{

	DRVRASP_DBG(RASP_DBGLV_INFO, "RaspEng = %x\n",RaspEng);

	return HAL_NDSRASP_EVENT_GetEventNum(RaspEng);

}

void NDS_RASP_SetDbgLevel(MS_U32 level)
{
    _g32RASPDrvDbgLv = level;
    HAL_NDSRASP_SetDbgLevel(level);

}

void RaspDemoTest(MS_U32 Addr)
{
    MS_U32 u32Size = (188 * 10);
    printf("enter RaspDemoTest\n");
    NDS_RASP_SetDbgLevel(7);
    NDS_RASP_Init();

    MS_U32 rasp_eng = 0;
    HAL_NDSRASP_Rec_PID(0, TRUE);
    NDS_RASP_SetPid(0, 0, 0xc1);

	NDS_RASP_SetBufInfo(rasp_eng, STR2MIU /*PVR*/, Addr, (Addr + u32Size), u32Size, u32Size);

	//payload packetmode, FALSE-->188 byte mode
    //NDS_RASP_SetPayloadPacketMode(rasp_eng, TRUE);	//payload packet 192/188

	NDS_RASP_FlowSet(rasp_eng, RASP_FLOW_INPUT_DEMOD0, 0 /*not implement*/, TRUE, TRUE, FALSE, RASP_IN_188);

}

MS_BOOL NDS_RASP_SetExtendConfig(MS_U32 RaspEng, MS_U16 type, MS_U16 extendbytes, MS_U16 syncbyte)
{

    printf("NDS_RASP_SetExtendConfig, RaspEng = %x\n",RaspEng);
    HAL_NDSRASP_SetExtendConfig(RaspEng, type, extendbytes, syncbyte);

    return TRUE;
}

