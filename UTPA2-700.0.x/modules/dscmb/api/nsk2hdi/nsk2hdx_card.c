
#include <string.h>
#include "MsCommon.h"

#include "drvNDSSC.h"
#include "drvGPIO.h"

#include "hdi_121.h"
#include "nsk_dbg.h"
#include "nsk2hdx_header.h"

/*****************************************************************************\
  \ local defines
\*****************************************************************************/
#define SC_0                        0
#define SC_1                        1

#define Default_SC_DEV              SC_0

#define NSK2_SC_EVT_MASK0           0x0000000F
#define NSK2_SC_EVT_MASK1           0x000000F0
#define NSK2_SC_EVT_IN0             0x00000001
#define NSK2_SC_EVT_OUT0            0x00000002
#define NSK2_SC_EVT_IN1             0x00000010
#define NSK2_SC_EVT_OUT1            0x00000020
#define NSK2_SC_EVT_STOP            0x00000100

#define Default_SC_PARAM            _T0_InitParam
#define HDX_SC_TASK_STACK_SIZE      16*1024

#define SMC_PPS                      0


static MS_U32 SCTimeOut = 0;
static MS_U32 SCHWTimeOut = 0;
#define SCTimeOutOffset              SCTimeOut
#define SCHWTimeOutOffset            SCHWTimeOut

#define INITIAL_WAITING_TIME 960    //SPEC 7816-3
#define LEAST_TC2_VALUE 0xA         //least WWT value NDS stated

/*****************************************************************************\
  \ global variables
\*****************************************************************************/
NSK2HDX_UARTCONFIGURATION   gSC_conf[2] = {
    {
        .card_clock       = NSK2HDX_SC_CLOCK_1,
        .card_baud_rate   = NSK2HDX_SC_BR_3,
        .protocol         = NSK2HDX_SC_PROTOCOL_T_0,
    },
    {
        .card_clock       = NSK2HDX_SC_CLOCK_1,
        .card_baud_rate   = NSK2HDX_SC_BR_3,
        .protocol         = NSK2HDX_SC_PROTOCOL_T_0,
    }
};

SC_Param    gSCInitParams[2];
NDS_UBYTE   gbPin_Mask = NSK2HDX_UART_COMMAND_IO_C7_ON;
/*****************************************************************************\
  \ local variables
\*****************************************************************************/

//NDS_ULONG       sc_reader_id[] = {SC_0, SC_1};
NDS_ULONG       sc_reader_id[] = {SC_0};

static MS_U8  _atrbuf[SC_ATR_LEN_MAX];
static MS_U8  _historybuf[SC_HIST_LEN_MAX];
static MS_U16 _atrlen = 0;
static MS_U16 _historylen = 0;

static MS_U8  _u8HDX_SCIsInit = FALSE;
static MS_S32 _s32HDX_SCEventId = -1;
static MS_S32 _s32HDX_SCTaskId = -1;

static void*  _pHDX_SCTaskStack;
static MS_U8  _u8HDX_SC_StackBuffer[HDX_SC_TASK_STACK_SIZE];

static MS_U8  u8GT_record =0;
static MS_U8  u8Baudrate_record = 0;


static NDS_UBYTE _gSC_Vcc_Level = NSK2HDX_VCC_5V;
static MS_U32 _bSC_log = FALSE;
static NDS_UBYTE gGuardTime = 0;

static MS_BOOL isDefaultUART=TRUE;

extern void _NDS_T0_CmdTest(MS_U32 u8SCID);


typedef struct
{
    MS_U8 TA1byte;
    MS_U32 clock_rate;
    MS_U32 clock_rate_conversion;
} SC_rate_conversion;

static SC_rate_conversion SC_Rate_Table[] =
{
    {0x11,  72/*4.5*16*/,      372*16},
    {0x12,  72/*4.5*16*/,      372*16},
    {0x13,  72/*4.5*16*/,      372*16},
    {0x14,  72/*4.5*16*/,      372*16},
    {0x15,  72/*4.5*16*/,      372*16},
    {0x36,  108/*6.75*16*/,    744*16},
    {0x5f,  216/*13.5*16*/,    1488*16},
    {0x6f,  216/*13.5*16*/,    1860*16},
    {0x96,  72/*4.5*16*/,      512*16},
};


/*****************************************************************************\
  \ local functions
\*****************************************************************************/

static void _debug_param_print(MS_U8 u8SCID)
{
    NSK_TRACE(("u8SCID = %d\n",u8SCID));
    NSK_TRACE(("u8Protocal = %x\n",gSCInitParams[u8SCID].u8Protocal));
    NSK_TRACE(("eCardClk = %d\n",gSCInitParams[u8SCID].eCardClk));
    NSK_TRACE(("u8UartMode = %d\n",gSCInitParams[u8SCID].u8UartMode));
    NSK_TRACE(("u16ClkDiv = %d\n",gSCInitParams[u8SCID].u16ClkDiv));
    NSK_TRACE(("eVccCtrl = %d\n",gSCInitParams[u8SCID].eVccCtrl));
    NSK_TRACE(("u16Bonding = %d\n",gSCInitParams[u8SCID].u16Bonding));
    NSK_TRACE(("u8Convention = %d\n",gSCInitParams[u8SCID].u8Convention));
}

void _T0_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_4P5M;
    pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
    pParam->u16ClkDiv = (372 << 4);
}


MS_BOOL _NSK2_verSC_Reset(MS_U8 u8SCID,SC_ResetType RstType)
{
    MS_U8       _u8AtrBuf[SC_ATR_LEN_MAX];
    MS_U16      _u16AtrLen = 255;
    MS_U8       history[SC_HIST_LEN_MAX];
    MS_U16      history_len = 200;
    MS_U8       u8Protocol = 0xff;
    SC_Param    *pInitParams;
    MS_U8       sc_clock;
//    MS_U16      i;

	NSK_TRACE(("Come to _NSK2_verSC_Reset\n"));

	MDrv_SC_T0_SetTimeOut( 150 * 10 );
    memset(_u8AtrBuf, 0x00, sizeof(_u8AtrBuf));

    pInitParams = &gSCInitParams[u8SCID];
    _debug_param_print(u8SCID);

#if 0
    if (MDrv_SC_Reset(u8SCID, pInitParams) != E_SC_OK)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    MDrv_SC_ClearState(u8SCID);
#endif

    NSK_TRACE(("Start to Reset and Get ATR\n"));

    //if (MDrv_SC_GetATR(u8SCID, 100*5, _u8AtrBuf, &_u16AtrLen, history, &history_len) == E_SC_OK)
    if (MDrv_SC_Reset_ATR(u8SCID, pInitParams, _u8AtrBuf, &_u16AtrLen, history, &history_len) == E_SC_OK)
    {
    	if (_u8AtrBuf[1] & 0x80)
        {
        	u8Protocol = _u8AtrBuf[3] & 0xF;
        }
        else
        {
            u8Protocol = 0;
        }
        pInitParams->u8Protocal = u8Protocol;


        u8GT_record = _u8AtrBuf[4];					//Save GT
        u8Baudrate_record = _u8AtrBuf[2] & 0xf;		//Save TA1

        _atrlen = _u16AtrLen;
        memcpy(_atrbuf,_u8AtrBuf,SC_ATR_LEN_MAX);
        _historylen = history_len;
        memcpy(_historybuf,history,SC_HIST_LEN_MAX);

        //For setting SMC clock rate
        sc_clock = (_atrbuf[2]>>4)&0xf;
        if (3 == sc_clock )	//6.75MHz
        {
            pInitParams->eCardClk = E_SC_CLK_6M;
            NSK_TRACE(("Change to 6.75MHz\n"));
        }
        else if( (5 == sc_clock ) || (6 == sc_clock ) )//13.5MHz
        {
            pInitParams->eCardClk = E_SC_CLK_13M;
            NSK_TRACE(("Change to 13.5MHz\n"));
        }
        else if( (1 == sc_clock ) || (9 == sc_clock ) )//4.5MHz
        {
            pInitParams->eCardClk = E_SC_CLK_4P5M;
            NSK_TRACE(("Change to 4.5MHz\n"));
        }
        else
        {
            ;
        }

        if (MDrv_SC_Config(u8SCID, pInitParams) != E_SC_OK)
        {
            printf("[%s][%d] \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        MDrv_SC_SetGuardTime(u8SCID,_atrbuf[4]);
        return TRUE;
    }
    else    //For Card is not exist when initial
    {
        NSK_TRACE(("Get ATR Fail\n"));
        pInitParams->eCardClk = E_SC_CLK_4P5M;
        MDrv_SC_Config(u8SCID,pInitParams);
        MDrv_SC_SetGuardTime(u8SCID,_atrbuf[4]);
    }
    MsOS_DelayTask(120);

    return FALSE;

}



static void _SC_Notify(MS_U8 u8SCID, SC_Event eEvent)
{

    NSK_TRACE(("eEvent = %d, u8SCID = %x\n",eEvent,u8SCID));
    switch (eEvent)
    {
        case E_SC_EVENT_IN:
            NSK_TRACE(("SC_EVENT_IN, u8SCID = %x\n",u8SCID));
				//MDrv_SC_Activate(0);
                MsOS_SetEvent(_s32HDX_SCEventId, ( E_SC_EVENT_IN | (u8SCID<<8) ) );
            break;

        case E_SC_EVENT_OUT:
            NSK_TRACE(("SC_EVENT_OUT, u8SCID = %x\n",u8SCID));
            #if 1
				//MDrv_SC_DeactiveSeq();
                MsOS_SetEvent(_s32HDX_SCEventId, ( E_SC_EVENT_OUT | (u8SCID<<8) ) );
            #else
                NSK2HDX_CardRemoved((NDS_ULONG)u8SCID);  // inform the verifier
            #endif
            break;
        case E_SC_EVENT_DATA:
            NSK_TRACE(("SC_EVENT_DATA\n"));
            break;
        default:
            break;
    }


}

static void _HDX_SC_Notify_Proc(void)
{
    MS_U32 u32Events;
    MS_U8 u8SCID = 0;
    while (1)
    {
        NSK_TRACE(("_HDX_SC_Notify_Proc wait Event\n"));
        MsOS_WaitEvent(_s32HDX_SCEventId, 0xFFFFFFFF /*ECMFLT_STATES_GROUP*/, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER /*1000*/ );

        u8SCID = (MS_U8)((u32Events>>8)&0xff);
        NSK_TRACE(("u8SCID = %x\n",u8SCID));

        if( (u32Events&0xff) == E_SC_EVENT_IN)
        {
            //_NSK2_SC_Reset(u8SCID,&gSCInitParams[u8SCID]);
            NSK2HDX_CardInserted((NDS_ULONG)u8SCID);  // inform the verifier
        }
        else if( (u32Events&0xff) == E_SC_EVENT_OUT)
        {
            printf("card out notify\n");
            MDrv_SC_Deactivate(u8SCID);
            NSK2HDX_SetVcc(u8SCID,NSK2HDX_VCC_OFF);
            NSK2HDX_CardRemoved((NDS_ULONG)u8SCID);  // inform the verifier
        }

    }
}


MS_BOOL NSK2_SC_Init(SC_Init_Paras_t *pInitPara)
{
    NSK_TRACE(("Enter\n"));

    SC_Result   errCode;
    MS_U8       u8SCID = Default_SC_DEV;
    SC_Param    *pInitParams;
    SC_Result   eSCResult;

    if(_u8HDX_SCIsInit == FALSE)
    {
        NSK_TRACE(("CreateEventGroup\n"));

        _s32HDX_SCEventId = MsOS_CreateEventGroup("HDX_SC_Event");
        if (_s32HDX_SCEventId < 0)
        {
            NSK_ERROR(("CreateEvent fail\n"));
            return FALSE;
        }

       NSK_TRACE(("_s32ECMFLTEventId = %d\n",_s32HDX_SCEventId));

        _pHDX_SCTaskStack = _u8HDX_SC_StackBuffer;
		_s32HDX_SCTaskId   = MsOS_CreateTask((TaskEntry)_HDX_SC_Notify_Proc,
                                            (MS_U32)NULL,
                                            E_TASK_PRI_MEDIUM,
                                            TRUE,
                                            _pHDX_SCTaskStack,
                                            HDX_SC_TASK_STACK_SIZE,
                                            "xxx");
        if (_s32HDX_SCTaskId < 0)
        {
            NSK_ERROR(("CreateTask fail\n"));
            MsOS_DeleteEventGroup(_s32HDX_SCEventId);
            return FALSE;
        }

        _u8HDX_SCIsInit = TRUE;
    }

    if ( (errCode = MDrv_SC_Init(u8SCID)) != E_SC_OK )
    {
        NSK_ERROR(("Error code=%d\n", errCode));
        return FALSE;
    }

    pInitParams = &gSCInitParams[u8SCID];
    memset(pInitParams,0x0,sizeof(SC_Param));

    SCTimeOut = pInitPara->DelayTime;
    SCHWTimeOut = pInitPara->DelayTime;
    if(pInitPara->b8024OnChip == TRUE)
    {
        pInitParams->eVccCtrl     = E_SC_VCC_VCC_ONCHIP_8024;
        printf(">>>>>>>>>Onchip 8024<<<<<<<<<<<\n");
    }
    else
    {
	    pInitParams->eVccCtrl     = E_SC_VCC_CTRL_8024_ON;
	    printf(">>>>>>>>>External 8024<<<<<<<<<<<\n");
    }

    // Init params
    pInitParams->eCardClk = E_SC_CLK_4P5M;
    pInitParams->u16ClkDiv = (372<<4);
    pInitParams->u8Protocal = 0;
    pInitParams->pfOCPControl = NULL;
    pInitParams->eResetType = E_SC_WARM_RESET;
    pInitParams->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
    pInitParams->eVoltage   = E_SC_VOLTAGE_5V;

    // Open
    if ( (eSCResult = MDrv_SC_Open(u8SCID, pInitParams->u8Protocal, pInitParams, _SC_Notify)) != E_SC_OK)
        {
        NSK_ERROR(("MDrv_SC_Open fail code=%d \n", eSCResult));
        return FALSE;
    }

    NSK2HDX_SetScreaderHwInfo(  &sc_reader_id[0],
                                sizeof(sc_reader_id)/sizeof(NDS_ULONG));        // inform verifier

    _debug_param_print(u8SCID);

    NSK_TRACE(("Exit\n"));
    return TRUE;
}

NDS_STATUS32 NSK2HDX_CheckCardPresence(NDS_ULONG screader_id, NDS_ULONG *card_presence)
{
    NSK_TRACE(("Enter\n"));

    SC_Status sc_status;

    NSK_PRINT(("Card ID:%d\n", screader_id));
    if ( MDrv_SC_GetStatus((MS_U8)screader_id, &sc_status) != E_SC_OK )
    {
        NSK_ERROR(("Get Status fail\n"));
        return NSK2HDX_FAIL;
    }

    NSK_TRACE(("bCardIn = %x\n",sc_status.bCardIn));
    *card_presence = (sc_status.bCardIn) ? NSK2HDX_CARD_INSERTED : NSK2HDX_CARD_REMOVED;

    NSK_TRACE(("Exit\n"));

    return NSK2HDX_OK;
}

NDS_STATUS32 NSK2HDX_SetVccLevel(NDS_ULONG screader_id, NDS_UBYTE vcc_level)
{
    NSK_TRACE(("Enter\n"));

    NSK_PRINT(("Vcc Level:%s\n", (vcc_level==NSK2HDX_VCC_5V)? "5V" : "3V"));

    if(_gSC_Vcc_Level == vcc_level)
    {
        return NSK2HDX_OK;
    }
    else
    {
        _gSC_Vcc_Level = vcc_level;
    }

    NSK_TRACE(("Exit\n"));

    return NSK2HDX_OK;
}

NDS_STATUS32 NSK2HDX_SetVcc(NDS_ULONG screader_id, NDS_UBYTE vcc_switch)
{
    NSK_TRACE(("Enter\n"));

    NSK_PRINT(("_Vcc switch:%s\n", (vcc_switch==NSK2HDX_VCC_ON)?"ON":"OFF"));

    if(vcc_switch==NSK2HDX_VCC_ON)
    {
        if(_gSC_Vcc_Level==NSK2HDX_VCC_5V)
        {
            MDrv_SC_CardVoltage_Config(screader_id,E_SC_VOLTAGE_5V);
        }
        else if(_gSC_Vcc_Level==NSK2HDX_VCC_3V)
        {
            MDrv_SC_CardVoltage_Config(screader_id,E_SC_VOLTAGE_3_POINT_3V);
        }
        else
        {
            NSK_PRINT(("Error VCC\n"));
        }
    }
    else
    {
        MDrv_SC_CardVoltage_Config(screader_id,E_SC_VOLTAGE_OFF);
    }

    NSK_TRACE(("Exit\n"));

    return NSK2HDX_OK;
}

/*
 * Calculate WWT value based on TC2 byte and TA1 byte
 * TA1 value gives information of Clock-rate and Clock-rate-conversion
 * which is seeding to WWT calculation formula here
 */
MS_U32 _SC_Calc_WWT(MS_U8 u8Ta1Byte, MS_U8 u8Tc2Byte)
{
    MS_U32 u32WWT=0;
    MS_U32 wwt;
    MS_U32 u32ConvTabSize;
    MS_U32 i;

    u32ConvTabSize = sizeof(SC_Rate_Table)/sizeof(SC_rate_conversion);
    NSK_TRACE(("u32ConvTabSize=0x%x\n", u32ConvTabSize));

    for(i=0; i<u32ConvTabSize; ++i)
    {
        if ( SC_Rate_Table[i].TA1byte == u8Ta1Byte )
        {
            NSK_TRACE(("clock_rate_conversion=%x, clock_rate=%x\n",
                        SC_Rate_Table[i].clock_rate_conversion, SC_Rate_Table[i].clock_rate));
            wwt = INITIAL_WAITING_TIME * (MS_U32)u8Tc2Byte * (SC_Rate_Table[i].clock_rate_conversion / SC_Rate_Table[i].clock_rate);
            wwt /= 1000;    //convert to mSec
            u32WWT = (MS_U32)wwt;
            NSK_TRACE(("work waiting time, [%x]\n", u32WWT));
            return u32WWT;
        }
    }
    return u32WWT;
}


/*
 * performs a single smart card reset pulse, receives the answer-to-reset data and
 * parses the parameters of this ATR data
 */
NDS_STATUS32 NSK2HDX_WarmResetCard(NDS_ULONG screader_id, NDS_UBYTE *atr_data,
                                    NDS_UBYTE *atr_len, NDS_UBYTE *first_history_byte)
{
    NSK_TRACE(("Enter, screader_id = %x\n",screader_id));
    //printf("Come to NSK2HDX_WarmResetCard\n");
    MS_U16      u16historylen=200;
    MS_U16      u16atrlen=0, i;
    MS_U8       history[SC_HIST_LEN_MAX];
    MS_U32 u32waiting_time;

    //MsOS_DelayTask(50);
    _atrlen = 0;
	isDefaultUART=TRUE;

#if 0
    _verSC_Reset(screader_id);
#else
    _NSK2_verSC_Reset(screader_id,0);
#endif


	memcpy(atr_data,_atrbuf,_atrlen);
    u16atrlen = _atrlen;

    NSK_TRACE(("u16atrlen = %d, u16historylen = %d\n",u16atrlen, u16historylen));

    if(u16atrlen == 0)
    {
        NSK_ERROR(("atrlen = 0\n"));
        return NSK2HDX_FAIL;
    }

    NSK_TRACE(("ATR log\n"));
    for(i = 0; i< u16atrlen; i++)
    {
        NSK_TRACE(("( %d, %x )\n",i,atr_data[i]));
    }

    memcpy(history,_historybuf,_historylen);
    u16historylen = _historylen;

    NSK_TRACE(("history log\n"));
    for(i = 0; i< u16historylen; i++)
    {
        NSK_TRACE(("( %d, %x )\n",i,history[i]));
    }

    if(u16atrlen > u16historylen)
    {
        *first_history_byte = u16atrlen - u16historylen; //history[0];
    }
    else
    {
        *first_history_byte = 0;
    }
    *atr_len = u16atrlen;

    //WWT set
    if ( atr_data[7] < LEAST_TC2_VALUE )    //least WWT value
    {
        u32waiting_time = _SC_Calc_WWT(atr_data[2], LEAST_TC2_VALUE);
    }
    else
    {
        u32waiting_time = _SC_Calc_WWT(atr_data[2], atr_data[7]);
    }
    MDrv_SC_SetBlockWaitingTime(screader_id, u32waiting_time - SCHWTimeOutOffset);
    MDrv_SC_SetWaitingTime(screader_id, u32waiting_time - SCTimeOutOffset);

    NSK_TRACE(("Exit\n"));

    return NSK2HDX_OK;
}

NDS_STATUS32 NSK2HDX_SetUartParams(NDS_ULONG screader_id,
                                    NSK2HDX_UARTCONFIGURATION *conf)
{
    NSK_TRACE(("Enter\n"));


    NSK_TRACE(("card_clock = %x, card_baud_rate = %x, nak_retries = %x\n",conf->card_clock,conf->card_baud_rate,conf->nak_retries));
    NSK_TRACE(("parity_retries = %x, work_wait_time = %x, guard_time = %x\n",conf->parity_retries,conf->work_wait_time,conf->guard_time));
    NSK_TRACE(("protocol = %x\n",conf->protocol));

    //UART by verifier
	isDefaultUART=FALSE;

    if(gGuardTime != conf->guard_time)
    {
    	MDrv_SC_SetGuardTime(screader_id, conf->guard_time);
        gGuardTime = conf->guard_time;
    }

	conf->guard_time = u8GT_record;
	conf->card_baud_rate = u8Baudrate_record;
	//conf->work_wait_time = 60;

    //if there is WWT set by verifier
    if(conf->work_wait_time != 0)
    {
        NSK_TRACE(("Manual WWT = [%d]\n", conf->work_wait_time*10 ));
        MDrv_SC_SetBlockWaitingTime(screader_id, conf->work_wait_time*10 - SCHWTimeOutOffset);
        MDrv_SC_SetWaitingTime(screader_id, conf->work_wait_time*10 - SCTimeOutOffset);
    }

	memcpy(&gSC_conf[screader_id], conf, sizeof(NSK2HDX_UARTCONFIGURATION));

    return NSK2HDX_OK;
}

NDS_STATUS32 NSK2HDX_GetUartParams(NDS_ULONG screader_id,
                                    NSK2HDX_UARTCONFIGURATION *conf)
{
    NSK_TRACE(("Enter\n"));
	MS_U16 wait_time;
    //NSK_TRACE(("NSK2HDX_GetUartParams:%x\n",screader_id));
    memcpy(conf, &gSC_conf[screader_id], sizeof(NSK2HDX_UARTCONFIGURATION));

	conf->guard_time = u8GT_record;
	conf->card_baud_rate = u8Baudrate_record;

	//20150811, default work wait time depending on TC2 byte
	if (TRUE == isDefaultUART)
	{
		wait_time = MDrv_SC_GetWaitingTime(screader_id);
		NSK_TRACE(("NSK2HDX_GetUartParams: default wait_time = %x\n", wait_time));
		conf->work_wait_time = wait_time /10;
	}
    else
    {
        //this case just return value set by verifier
    }

    NSK_TRACE(("Exit\n"));

    return NSK2HDX_OK;
}



MS_U32 NSK2HDX_SetSCLog(MS_U32 *bEn)
{
    _bSC_log = *bEn;
    return TRUE;
}

extern void MApi_SC_T0_SetTimeOut(MS_U32 u32TimeOut);
//extern void MDrv_SC_T0_SetTimeOut(MS_U32 u32TimeOut);
//-------------------------------------------------------------------------------//
//--The buffers represented by the parameters to_card and from_card will hold ---//
//--only the data payload, not any additional bytes that are related to the -----//
//--protocol (for example PB/ACK, null bytes, etc.) -----------------------------//
//-------------------------------------------------------------------------------//

NDS_STATUS32 NSK2HDX_CardIoCommand( NDS_ULONG   screader_id,
                                    NDS_UBYTE   *to_card,
                                    NDS_ULONG   to_card_len,
                                    NDS_UBYTE   *from_card,
                                    NDS_ULONG   *from_card_len,
                                    NDS_UBYTE   direction,
                                    NDS_USHORT  timeout)
{
    MS_U32 u32CardIOTime = MsOS_GetSystemTime();
    SC_Result sc_result = E_SC_OK;

    NSK_TRACE(("start time is %d\n",MsOS_GetSystemTime()));
    NSK_TRACE(("Enter, screader_id = %d, direction = %d\n",screader_id,direction));
    NSK_TRACE(("to_card_len = %d, timeout = %d\n",to_card_len,timeout));
	MS_U32 i;

    //for( i=0; i<to_card_len ; i++)
    //{
    //    NSK_TRACE(("(%d, %x)\n",i,to_card[i]));
    //}

    if(_bSC_log == TRUE)
    {
        printf(" \n");
        for( i=0; i<to_card_len ; i++)
        {
            printf("0x%x, ",to_card[i]);
        }
        printf(" \n");
    }

	switch (direction)
    {
        case NSK2HDX_DIRECTION_FROM_CARD:
            MDrv_SC_T0_SetTimeOut( timeout * 10 - SCTimeOutOffset);
            sc_result = MDrv_SC_T0_SendRecv((MS_U8)screader_id, to_card, (MS_U16)to_card_len,from_card, (MS_U16*)from_card_len);
            break;

        case NSK2HDX_DIRECTION_TO_CARD:
            MDrv_SC_T0_SetTimeOut( timeout * 10 - SCTimeOutOffset);
            sc_result = MDrv_SC_T0_SendRecv((MS_U8)screader_id, to_card, (MS_U16)to_card_len, from_card, (MS_U16*)from_card_len);
            break;

        default:
            break;
    }


    NSK_TRACE(("Time of Card IO Time = %d\n", (MsOS_GetSystemTime() - u32CardIOTime)));
    NSK_TRACE(("Exit\n"));

    if(E_SC_OK != sc_result)
    {
        NSK_ERROR(("sc_result = %d\n",sc_result));
        NSK_ERROR(("now time is %d\n",MsOS_GetSystemTime()));
        NSK_ERROR(("Time of Card IO Time = %d\n", (MsOS_GetSystemTime() - u32CardIOTime) ));
        return NSK2HDX_FAIL;
    }

    return NSK2HDX_OK;
}

//-----------------------------------------------------------------------------------//
//-- In each call to this routine, exactly one of C4, C7 and C8 must be indicated ---//
//-- as the I/O line. The state of the other 2 (GPIO) lines must bothe be specified -//
//-- Before setting a pin for use as I/O, the pin must first be in state H. ---------//
//-----------------------------------------------------------------------------------//

//controls C7, C4, C8 pin setting in the smart card interface.
NDS_STATUS32 NSK2HDX_SetUartCommand(NDS_ULONG screader_id, NDS_UBYTE pin_mask)
{
    NSK_TRACE(("Enter\n"));

	printf("pin_mask:%x	\n",pin_mask);

    NDS_UBYTE IOLine = NSK2HDX_UART_COMMAND_IO_C7_ON;
    if(gbPin_Mask == pin_mask)
    {
        NSK_TRACE(("pin mask is the same, return\n"));
    }
    gbPin_Mask = pin_mask;

    if( pin_mask & NSK2HDX_UART_COMMAND_IO_C7_ON ) //case 1, C7 as I/O line...
    {
        NSK_TRACE(("C7 as I/O line...\n"));

        //check C4,C8 pin configs....
        if( pin_mask & NSK2HDX_UART_COMMAND_IO_C4_ON)
        {
            NSK_ERROR(("C7 is already the IO line, C4 \n"));
			return NSK2HDX_INVALID_REQUEST;
        }

        if( pin_mask & NSK2HDX_UART_COMMAND_IO_C8_ON)
        {
            NSK_ERROR(("C7 is already the IO line, C8 \n"));
        }

        //config C7 as I/O line
        MDrv_SC_IOC4C8Switch(E_SC_IO_USE_SMIO);

        IOLine = NSK2HDX_UART_COMMAND_IO_C7_ON;
    }
    else if( pin_mask & NSK2HDX_UART_COMMAND_IO_C4_ON ) //case 2, C4 as I/O line...
    {
        NSK_TRACE(("C4 as I/O line...\n"));
        //check C8 pin configs....
        if( pin_mask & NSK2HDX_UART_COMMAND_IO_C8_ON)
        {
            NSK_ERROR(("C4 is already the IO line, C8 \n"));
        }

        //config C4 as I/O line
        MDrv_SC_IOC4C8Switch(E_SC_IO_USE_C4);
        IOLine = NSK2HDX_UART_COMMAND_IO_C4_ON;
    }
    else if( pin_mask & NSK2HDX_UART_COMMAND_IO_C8_ON ) //case 3, C8 as I/O line...
    {
        NSK_TRACE(("C8 as I/O line...\n"));
        //config C8 as I/O line
		MDrv_SC_IOC4C8Switch(E_SC_IO_USE_C8);
        IOLine = NSK2HDX_UART_COMMAND_IO_C8_ON;
    }
    else
    {
        NSK_ERROR(("error pin config\n"));
        MDrv_SC_IOC4C8Switch(E_SC_IO_USE_SMIO);
    }

    if(IOLine != NSK2HDX_UART_COMMAND_IO_C4_ON)
    {
    	if( pin_mask & NSK2HDX_UART_COMMAND_C4_ON )
    	{
    		MDrv_SC_C4_SignalLevel(1);
    	}
    	else
    	{
    		MDrv_SC_C4_SignalLevel(0);
    	}
    }


    if(IOLine != NSK2HDX_UART_COMMAND_IO_C8_ON)
    {
    	if(pin_mask & NSK2HDX_UART_COMMAND_C8_ON )
    	{
    		MDrv_SC_C8_SignalLevel(1);
    	}
    	else
    	{
    		MDrv_SC_C8_SignalLevel(0);
    	}
    }

    if(IOLine != NSK2HDX_UART_COMMAND_IO_C7_ON)
    {
    	if(pin_mask & NSK2HDX_UART_COMMAND_C7_ON )
    	{
    		MDrv_SC_C7_SignalLevel(1);
    	}
    	else
    	{
    		MDrv_SC_C7_SignalLevel(0);
    	}
    }

    NSK_TRACE(("Exit\n"));

    return NSK2HDX_OK;
}

NDS_STATUS32 NSK2HDX_GetUartCommand(NDS_ULONG screader_id, NDS_UBYTE *pin_mask)
{
    NSK_TRACE(("Enter\n"));

	if((gbPin_Mask & 0xF) != 0x8)
	{
    	*pin_mask = gbPin_Mask & 0x7F;
	}
	else
	{
    *pin_mask = gbPin_Mask;
	}

    NSK_TRACE(("Exit\n"));

    return NSK2HDX_OK;
}

