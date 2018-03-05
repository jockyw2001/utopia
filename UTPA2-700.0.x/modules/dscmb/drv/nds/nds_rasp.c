

#include "MsCommon.h"

#ifdef MSOS_TYPE_LINUX
#include "string.h"
#endif


#if 0

#include "asmCPU.h"
#include "regNDS.h"
#include "drvNDS.h"
#include "halNDS.h"
#include "../rasp/regRASP.h"
#include "../rasp/halRASP.h"
#include "../tsp2/drvTSP2.h"

#include "ddiNDS_HDI.h"

#include "nds.h"

//------------------------------------------------------------------------//
//       Macro Definition
//------------------------------------------------------------------------//

#define RASP_INT_EVENT_ALL (RASP_INT_EFRAME_RD_OVF | RASP_INT_EFRAME_WT_OVF| \
                            /*RASP_INT_STR2MIU|*/RASP_INT_PAYLD2MIU | RASP_INT_ECM2MIU|\
                            RASP_INT_TIME_WATER_MARK|RASP_INT_EVENT_WATER_MARK |\
                            RASP_INT_ECM_PKT_RDY )


#define NDS_RASP_TASK_STACK_SIZE  (1024 * 4 )




#define NDS_RASP_ECM_OVERFLOW_SHIFT   0x00000016

#define NDS_RASP_EVENT_ALL            0xFFFFFFFF


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

//------------------------------------------------------------------------//
//       Variable declaration
//------------------------------------------------------------------------//


static MS_U8 *_rasp_ecm_addr[RASP_NUM] ;//{[0 ... (RASP_NUM)] = NULL } ;

static MS_U32 _nds_rasp_eventid = -1 , _nds_rasp_taskid  = -1 ;

MS_U8    _u8RaspEcmBuf[RASP_NUM][NDS_CAM_ECM_FLT_NUM][2][NDS_CAM_FLT_BUF_SIZE];

NDS_RASP_ECM_FLT _rasp_ecm_flt[RASP_NUM][NDS_CAM_ECM_FLT_NUM];

static MS_U32  _nds_rasp_stack[NDS_RASP_TASK_STACK_SIZE/sizeof(MS_U32)];

#ifdef MSOS_TYPE_LINUX_KERNEL
static irqreturn_t _NDS_RASP_Isr(int irq, void *dev_id);
#else
static void _NDS_RASP_Isr(InterruptNum eIntNum);
#endif

static void _NDS_RASP_IsrTask(void);

// -----------------------------------------------------------------------//
//
//       RASP LDI Driver IMPLEMENTATION
//
// -----------------------------------------------------------------------//
XSTATUS RASPLDI_GetCapabilities(RASP_CAPS *RaspCaps)
{

    // [FIXME]
    RaspCaps->Supported_Event = 0x800001FF ;
    RaspCaps->Event_Timer_Rate = 0x200; //[FIXME]
    RaspCaps->Num_Rasps = RASP_NUM ;
    RaspCaps->Max_Event_Num_In_Fifo   = RASP_EVENT_FIFO_DEPTH ;
    RaspCaps->Num_of_Payloaad_Buffers = 1; // ?
    RaspCaps->Num_of_Filters     = RASP_EVENT_NUM ;
    RaspCaps->Event_Generation_Every_Slot_Time = TRUE ;
    RaspCaps->PCR_Stamp          = TRUE ;
    RaspCaps->Local_Clock_Stamp  = TRUE ;
    RaspCaps->Trigger_by_Evnet   = TRUE ;
    RaspCaps->Trigger_by_Time    = TRUE ;


    return X_OK ;

}

XSTATUS RASPLDI_GetEventMask
(
    XCONNECTION        Xconn,
    XUCHAR             FilterNum,
    XUSHORT           *Pid,
    F_RASP_EV_MASK    *EventMask,
    F_RASP_EV_MASK    *PayLoadMask
) ;

XSTATUS RASPLDI_GetTimerAndPacketNumber
(
    XCONNECTION     Xconn,
    XULONG          *Time,
    XULONG          *PacketNumber
) ;

XSTATUS RASPLDI_GetTSPayload
(
    XCONNECTION     Xconn,
    XULONG          PacketNum,
    XUCHAR          Buffer[188]
);

XSTATUS RASPLDI_Reset(XCONNECTION Xconn) ;

XSTATUS RASPLDI_SetEventNotifyFunction
(
    XCONNECTION     Xconn,
    RASP_EVENT_NOTIFY_FOUNCTION     *RaspEventCB
);

XSTATUS RASPLDI_SetOverflowNotifyFunction
(
    XCONNECTION    Xconn,
    RASP_OVERFLOW_NOTIFY_FUNCTION      *RaspOvfCB
) ;


XSTATUS RASPLDI_Configure
(
    XCONNECTION          Xconn,
    TRIGGER_EXECUTOR     TriggerExecutor,
    XUSHORT              EventHighMark,
    XUSHORT              TimeHighMark,
    F_RASP_EV_MASK       PidIndEvent
);


XSTATUS RASPLDI_SetEventMask
(
    XCONNECTION       Xconn,
    XUCHAR            FilterNum,
    XUSHORT           Pid,
    F_RASP_EV_MASK    EventMask,
    F_RASP_EV_MASK    PayloadMask
);


XSTATUS RASPLDI_ReplacePackets(XCONNECTION Xconn, XUSHORT Pid, XUCHAR FromByte, XUCHAR NumBytes, XUCHAR *ReplaceData)
{

    MS_U32 EngId = 0, i ;
    MS_U16 FltPid = 0  ;


    for( i = 0 ; i < RASP_EVENT_NUM ; i++)
    {
        HAL_RASP_GetPidflt(EngId, i , &FltPid);
        if (FltPid == Pid) break ;
    }

    if (RASP_EVENT_NUM  == i) return FALSE ;

    HAL_RASP_SetCorptData(EngId ,FromByte  ,FromByte+NumBytes ,*ReplaceData);

    HAL_RASP_SetCorptFlt(EngId,i,TRUE);

    return X_OK;

}


XSTATUS RASPLDI_Start(XCONNECTION Xconn);



//------------------------------------------------------------------//
//
//        Mstar Driver Implementation
//
//------------------------------------------------------------------//



NDS_Result NDS_RASP_Init(NDS_RASP_Param *Param)
{

    //MS_U32 regstatus=0;

    if (Param->ecm_size < RASP_NUM*(NDS_CAM_FLT_BUF_SIZE*NDS_CAM_ECM_FLT_NUM) )
    {
        NDS_ERR("RASP ECM buffer size is less than FLT_BUF_SIZE*ECM_FLT_NUM\n");
    }

    HAL_RASP_SetECM_StartAddr( 0 ,Param->ecm_addr);
    HAL_RASP_SetECM_MidAddr( 0 ,Param->ecm_addr);
    HAL_RASP_SetECM_EndAddr( 0 ,Param->ecm_addr +NDS_CAM_FLT_BUF_SIZE*NDS_CAM_ECM_FLT_NUM );

    _rasp_ecm_addr[0] = (MS_U8*)Param->ecm_addr;

    memset((MS_U8*)MS_PA2KSEG1(Param->ecm_addr),0x0,NDS_CAM_FLT_BUF_SIZE*NDS_CAM_ECM_FLT_NUM);
    memset(_u8RaspEcmBuf ,0x0, sizeof(_u8RaspEcmBuf));


    HAL_RASP_ECM_Enable(0 , TRUE);

    //Param->ecm_size ;
    //Param->payload_addr;
    //Param->pu8payload_buf;
    //Param->payload_size;

    MsOS_AttachInterrupt(E_INT_IRQ_RASP, _NDS_RASP_Isr); //
    MsOS_EnableInterrupt(E_INT_IRQ_RASP);

    RASP_REG(REG_RASP_CA_INT)= RASP_CA_INT_ECM | RASP_CA_INT_MASK_RESET_MODE ;
    RASP_REG(REG_RASP_CA_INT)= RASP_CA_INT_ECM ;
    RASP_REG(REG_RASP_ECM_INT_STAT) = RASP_ECM_INT_FLT_MASK ;


    //HAL_RASP_INT_Enable(0,(MS_U16)RASP_INT_EVENT_ALL);
    HAL_RASP_INT_Enable(0,0xffff);


    if (-1 == _nds_rasp_eventid)
    {
       _nds_rasp_eventid =  MsOS_CreateEventGroup("NDS_RASP_Event");
    }

    if (-1 == _nds_rasp_taskid)
    {
       _nds_rasp_taskid = MsOS_CreateTask(  (TaskEntry) _NDS_RASP_IsrTask,
                                            (MS_U32)NULL,
                                            (TaskPriority) (E_TASK_PRI_SYS+1),
                                            TRUE,
                                            _nds_rasp_stack,
                                            NDS_RASP_TASK_STACK_SIZE,
                                            "NDS_RASP_IsrTask");
    }


    return  E_NDS_OK ;
}


NDS_Result NDS_RASP_DefineEcmFilter
           (NDS_ULONG Xconn , NDS_BYTE ecm_filter_num , NDS_BYTE ecm_filter_control)
{
    MS_U16 ecm_pid , i ;


    //NDS_FUNC("[%s]-[%d] -- ecm_filter_num[%d]\n", __FUNCTION__, __LINE__, ecm_filter_num);

    //printf("[%s]-[%d] --xconn = %ld ,  ecm_filter_num[%d]\n", __FUNCTION__, __LINE__,Xconn, ecm_filter_num);

    if (ecm_filter_num >= NDS_CAM_ECM_FLT_NUM)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }
    for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++) // 8.2.2 Note
    {

        if(
           (i != ecm_filter_num)&& //[FIXME]
           (_rasp_ecm_flt[0][i].pid == _rasp_ecm_flt[0][ecm_filter_num].pid )&&
           (_rasp_ecm_flt[0][i].Xconn == _rasp_ecm_flt[0][ecm_filter_num].Xconn)&&
           (_rasp_ecm_flt[0][i].b_defined = TRUE )
          )
        {
            return CA_ECM_PID_FILTER_ALREADY_DEFINED;
        }
    }

    //-------//
    if ((_rasp_ecm_flt[0][ecm_filter_num].pid < 0x2 || _rasp_ecm_flt[0][ecm_filter_num].pid > 0x1FFE))
    {
        return CA_OK ;
    }

    ecm_pid = _rasp_ecm_flt[0][ecm_filter_num].pid;
    if (ecm_filter_control & HDICA_ECM_FILTER_CONTROL_ENABLE_ODD_RECEPTION)
    {
        ecm_pid |=RASP_ECM_PID_TID_ODD;
    }
    if (ecm_filter_control & HDICA_ECM_FILTER_CONTROL_ENABLE_EVEN_RECEPTION)
    {
        ecm_pid |= RASP_ECM_PID_TID_EVEN;
    }
    if (ecm_filter_control & HDICA_ECM_ENABLE_RECEPTION)
    {
        ecm_pid |= RASP_ECM_PID_ENABLE;
        _rasp_ecm_flt[0][ecm_filter_num].b_defined = TRUE;
    }
    else
    {
        _rasp_ecm_flt[0][ecm_filter_num].b_defined = FALSE;
    }

    RASP_REG(REG_RASP_ECM_PID(ecm_filter_num)) = 0;
    RASP_REG(REG_RASP_ECM_LOCK(ecm_filter_num)) |= RASP_ECM_LOCK_AUTO(ecm_filter_num) ;
    RASP_REG(REG_RASP_ECM_PID(ecm_filter_num)) = ecm_pid;

    return CA_OK;

}



NDS_Result NDS_RASP_DefineEcmPid (NDS_ULONG Xconn , NDS_BYTE ecm_filter_num ,
           NDS_USHORT Pid, NDS_BYTE odd_ecm_table_id , NDS_BYTE even_ecm_table_id )
{
    MS_U16  u16EcmTid = 0;

    //NDS_FUNC("[%s]-[%d]--ecm_filter_num[%d]\n", __FUNCTION__, __LINE__, ecm_filter_num);
    //printf("[%s]-[%d]-- xconn = %ld , ecm_filter_num[%d]\n", __FUNCTION__, __LINE__,Xconn ,ecm_filter_num);

    if (ecm_filter_num >= NDS_CAM_ECM_FLT_NUM)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    RASP_REG(REG_RASP_ECM_PID(ecm_filter_num)) = Pid & RASP_ECM_PID_MASK;

    u16EcmTid |= ( even_ecm_table_id    & RASP_ECM_TID_EVEN_MASK);
    u16EcmTid |= ((odd_ecm_table_id<<8) & RASP_ECM_TID_ODD_MASK );

    RASP_REG(REG_RASP_ECM_TID) = u16EcmTid;

    _rasp_ecm_flt[0][ecm_filter_num].pid = Pid ;
    _rasp_ecm_flt[0][ecm_filter_num].Xconn = Xconn ;

    if (Pid == 0xFFFF)
    {
        return CA_OK;
    }

    if ((Pid < 0x2 || Pid > 0x1FFE))
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }


    return CA_OK;

}



#ifdef MSOS_TYPE_LINUX_KERNEL
static irqreturn_t _NDS_RASP_Isr(int irq, void *dev_id)
#else
static void _NDS_RASP_Isr(InterruptNum eIntNum)
#endif
{

    MS_U8               u8EcmReg;
    MS_U8               ecm_polarity;
    MS_U16              IntStatus;
    MS_U32              i,EngId = 0 ;


    //E_INT_IRQ_RASP
    IntStatus = HAL_RASP_INT_GetHW(0);
    //printf("[RASP]Interrupt!!----- eIntNum = %d / Status = %04X \n",eIntNum,IntStatus);

    if (IntStatus & RASP_INT_ECM_PKT_RDY) // ECM Packet Ready
    {
        RASP_REG(REG_RASP_CA_INT) = (RASP_CA_INT_ECM );

        u8EcmReg = RASP_REG(REG_RASP_ECM_INT_STAT);
        //printf("== Ecm Reg Int = %08X == \n",u8EcmReg);

        for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++)
        {
            if (u8EcmReg & (0x1 << i))
            {
                MS_U8   *pEcmData;

                RASP_REG(REG_RASP_ECM_INT_STAT) = (0x1 << i);

                //if (_EcmFlt[i].u8Drop == 1)
                //    continue;
                pEcmData = (void *)MS_PA2KSEG1((MS_U32)_rasp_ecm_addr[0] + (NDS_CAM_FLT_BUF_SIZE*i));
                MsOS_ReadMemory();

                if (pEcmData[4] != 0x47)
                {
                    continue; //_NDS_ASSERT_;
                }

                if (pEcmData[9] == 0x80)
                {
                    ecm_polarity = 0; // EVEN
                }
                else if (pEcmData[9] == 0x81)
                {
                    ecm_polarity = 1; // ODD
                }
                else
                {
                    NDS_ASSERT(FALSE, , "[%s]-[%d]\n", __FUNCTION__, __LINE__);
                }

                if (_u8RaspEcmBuf[EngId][i][ecm_polarity][0] == 0) // if buffer empty , copy pkt from hw buffer to sw buffer.
                {

                    MsOS_ReadMemory();
                    memcpy(_u8RaspEcmBuf[EngId][i][ecm_polarity], pEcmData+4, 188);


                    if (RASP_REG(REG_RASP_ECM_INT_STAT) & ( 0x1 << i))
                    {
                        MsOS_SetEvent(_nds_rasp_eventid, 0x1 << (i + NDS_RASP_ECM_OVERFLOW_SHIFT ) );
                        continue;
                    }

                    MsOS_SetEvent(_nds_rasp_eventid, (0x1 << (i*2 )) << ecm_polarity);
                }
                else
                {
                    // set overflow event
                    MsOS_SetEvent(_nds_rasp_eventid, 0x1 << (i + NDS_RASP_ECM_OVERFLOW_SHIFT));
                    break;
                }
            }
        }
    }

    if(IntStatus & RASP_INT_PAYLD2MIU)
    {
        //[FIXME] WAIT FOR IMEPLEMENTATAION

    }

    if(IntStatus & RASP_INT_EVENT_WATER_MARK)
    {
        //[FIXME] not imeplemted
    }

    if(IntStatus & RASP_INT_TIME_WATER_MARK)
    {
        //[FIXME] not imeplemted
    }

    if(IntStatus & RASP_INT_EFRAME_RD_OVF)
    {
        //[FIXME] not imeplemted
    }

    if(IntStatus & RASP_INT_EFRAME_WT_OVF)
    {
        //[FIXME] not imeplemted
    }


    HAL_RASP_INT_ClrHW(EngId , 0xFFFF);
    MsOS_EnableInterrupt(E_INT_IRQ_RASP);

    //MsOS_EnableInterrupt(NDS_INT_EMM_ECM );

}




static void _NDS_RASP_IsrTask(void)
{

    MS_U32 u32Events ,i ,p ,hi_count ,lo_count;;
    XECM_MSG            ecm_msg;
    MS_U32              EngId = 0;

    while(1)
    {
        MsOS_WaitEvent(_nds_rasp_eventid, NDS_RASP_EVENT_ALL, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        //printf("Rasp Task Get Event = %08lX\n",u32Events);

        for( i = 0 ; i < NDS_CAM_ECM_FLT_NUM ; i ++ )
        {
            if(u32Events & (0x1 << (i + NDS_RASP_ECM_OVERFLOW_SHIFT)))
            {

                //memset(_u8RaspEcmBuf[EngId][i][0], 0x0 , 188 );
                //memset(_u8RaspEcmBuf[EngId][i][1], 0x0 , 188 );

                ecm_msg.ecm_buf_ptr = _u8RaspEcmBuf[EngId][i][0]; //NULL;
                ecm_msg.rcv_status  = ECM_OVERFLOW;

                XCORECA_EcmReceived(_rasp_ecm_flt[EngId][i].Xconn , &ecm_msg , hi_count, lo_count);
            }
            else
            {
                MS_U8 valid = (u32Events >> (i*2)) & 0x3;

                if(valid)
                {
                    for (p = 0; p < 2; p++)
                    {
                        if ( (valid & (0x1<<p)) && _u8RaspEcmBuf[EngId][i][p][0] == 0x47 && _u8RaspEcmBuf[EngId][i][p][5] == (0x80|p))
                        {
                            ecm_msg.ecm_filter_num = i;
                            ecm_msg.ecm_buf_ptr    = _u8RaspEcmBuf[EngId][i][p]+8;
                            ecm_msg.ecm_polarity   = ((p==0) ? HDICA_EVEN_BUFFER : HDICA_ODD_BUFFER);
                            ecm_msg.rcv_status     = ECM_OK;
                            //_NDS_REC_GetPktCount(_EcmFlt[i].x_connect, &hi_count, &lo_count);
                            if (XCORECA_EcmReceived(_rasp_ecm_flt[EngId][i].Xconn , &ecm_msg , hi_count, lo_count) != CA_OK)
                            {
                                XHDICA_ReleaseEcmBuffer(_rasp_ecm_flt[EngId][i].Xconn , ecm_msg.ecm_filter_num, ecm_msg.ecm_polarity, ecm_msg.ecm_buf_ptr);
                            }
                        }
                    }
                }

            } //end if overflow
        } // end for


        RASP_REG(REG_RASP_CA_INT) = (RASP_CA_INT_ECM);
    }// while(1)
}


#endif



