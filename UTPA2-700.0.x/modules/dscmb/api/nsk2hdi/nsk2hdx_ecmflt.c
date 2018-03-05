
#include <string.h>
#include "MsCommon.h"


#include "hdi_121.h"
#include "nsk_dbg.h"
#include "apiDMX.h"
#include "nsk2hdi_header.h"


/*****************************************************************************\
  \Definitions
\*****************************************************************************/
#define MDL_ECMFLT_OK                   1
#define MDL_ECMFLT_FAIL                 2
#define MDL_ECMFLT_INVALID_REQUEST      3
#define ECMFLT_TASK_STACK_SIZE          16*1024

#define ECMBufferSize       188

#define ECMDMX_BufferSize   ECMBufferSize*100

#define ECMPacketArrive         0x10000
#define ECMBufOverFlow          0x100000

#define ECMRecvieBuf            10

#define MaximumConnection       2

#define MaximumDmxId            128

#define ECMStopFlag             0xFF

#define EcmTmpBufSize           10*1024


/*****************************************************************************\
  \ local variables
\*****************************************************************************/
static MS_U8  _u8EcmFltIsInit = FALSE;

static MS_S32 _s32ECMFLTMutexId = -1;
static MS_S32 _s32ECMFLTEventId = -1;
static MS_S32 _s32ECMDFLTTaskId = -1;

static void*  _pECMDFLTTaskStack;
//static MS_U8  _u8ECMDFLT_StackBuffer[ECMFLT_TASK_STACK_SIZE];

static MS_U16 Pids[MaximumDmxId];
static MS_U8  *ECMAllocBuf[MaximumDmxId];
static MS_U16 ECM_Connection[MaximumDmxId];
static MS_U8 ECMBufCnt[MaximumDmxId];
static MS_U8 *pECMBufBack[MaximumDmxId][ECMRecvieBuf];
static MS_U8 ECMFlags[MaximumDmxId][2];


//prepare larger buffer first...
static MS_U8  TmpBuf[EcmTmpBufSize];
static MS_U8  last_ecm_polarity[2] = {2,2};

static MS_BOOL XConnFileInFlag[2] = {FALSE,FALSE};
static NDS_USHORT _gecm_pid[2] = {0x0,0x0};

static MS_U32 _bEcm_log = FALSE;

static NDS_UBYTE *plastECM;


/*****************************************************************************\
  \ global variables
\*****************************************************************************/


/*****************************************************************************\
  \ local functions
\*****************************************************************************/
void data_dump(MS_U8* pu8Buf, MS_U32 BufSize)
{
    MS_U32 i ;

    for( i = 0 ; i < BufSize ; i ++ ){
        if((i%16) == 0 ) printf("\n");
        printf("%02X ",*(pu8Buf + i));
    }
}

static void _NSK2_ECMDFLT_Isr_Proc(void)
{

    MS_U32 u32Events;
    MS_U8  DmxIdSect;
    MS_U8* pu8Read;
    MS_U8* pu8Write;
    MS_U8  ecm_polarity;
    MS_U8* pu8VARead;
    MS_U32 DataLen, DataRmn;
    MS_U8* pu8Ptr;
    MS_U16 connection;

    while (1)
    {
        //NSK_TRACE(("ECMDFLT_Isr_Proc wait Event\n"));
        MsOS_WaitEvent(_s32ECMFLTEventId, 0xFFFFFFFF /*ECMFLT_STATES_GROUP*/, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER /*1000*/ );

        //NSK_TRACE(("u32Event = %x\n",u32Events));
        MsOS_ObtainMutex(_s32ECMFLTMutexId,MSOS_WAIT_FOREVER);


        DmxIdSect = (u32Events & 0xff);
        connection = ((u32Events>>8) & 0xff);
        //NSK_TRACE(("DmxIdSect:%x, connection = %x\n",DmxIdSect,connection));

        if( ECMFlags[DmxIdSect][0] == ECMStopFlag )
        {
            NSK_TRACE(("ECM already stop\n"));
            goto ProcEnd;
        }

        // Get Buffer Read Pointer.
        MApi_DMX_SectReadGet(DmxIdSect, (MS_PHY*)(&pu8Read));
        //NSK_TRACE(("Read pointer = 0x%08lx\n", (MS_U32)pu8Read));

        // Get Buffer Write Pointer.
        MApi_DMX_SectWriteGet(DmxIdSect, (MS_PHY*)(&pu8Write));
        //NSK_TRACE(("Write pointer = 0x%08lx\n", (MS_U32)pu8Write));


        if ((pu8Write- pu8Read) == 0 )
        {
            NSK_TRACE(("\n !! No Section Get - Wait timeout \n" ));
        }
        else
        {
            //NSK_TRACE(("bytes need to be readed = %x\n",((MS_U32)pu8Write- (MS_U32)pu8Read)));
            //memcpy(_u8ECMBuf, (void *)MsOS_PA2KSEG1((MS_U32)pu8Read), 188);

        #if 1 // temp, Jeff
            MsOS_ReadMemory();
            //DMA_Api for user to Copy data from HW section Buffer into user data Buffer.
            MApi_DMX_CopyData(DmxIdSect,TmpBuf,EcmTmpBufSize,&DataLen,&DataRmn,NULL);
        #endif

            //NSK_TRACE(("copy data length = %x, data remain = %x\n",DataLen,DataRmn));

            if(DataRmn!=0)
            {
                MApi_DMX_CopyData(DmxIdSect,&TmpBuf[DataLen],EcmTmpBufSize,&DataLen,&DataRmn,NULL);
                //NSK_TRACE(("2. copy data length = %x, data remain = %x\n",DataLen,DataRmn));
            }

            pu8VARead = (MS_U8 *)TmpBuf;

            //NSK_TRACE(("first six bytes are (%x,%x,%x,%x,%x,%x)\n",pu8VARead[0],pu8VARead[1],pu8VARead[2],pu8VARead[3],pu8VARead[4],pu8VARead[5]));
            ecm_polarity = 0;

            //printf("Read[5]=%x,len=%d\n",pu8VARead[5],((MS_U32)pu8Write- (MS_U32)pu8Read) );
            if (pu8VARead[5] == 0x80)
            {
                ecm_polarity = NSK2HDX_EVEN_ECM; // EVEN
            }
            else if (pu8VARead[5] == 0x81)
            {
                ecm_polarity = NSK2HDX_ODD_ECM; // ODD
            }
            else
            {
                printf("there is error at packet data,%x\n",DmxIdSect);
                goto ProcEnd;
                //NDS_ASSERT(FALSE, , "[%s]-[%d]\n", __FUNCTION__, __LINE__);
            }


            if( ECMFlags[DmxIdSect][ecm_polarity] == 0 )
            {
                if(last_ecm_polarity[DmxIdSect] != ecm_polarity)
                {
                    ECMFlags[DmxIdSect][ecm_polarity] = 1;
                    last_ecm_polarity[DmxIdSect] = ecm_polarity;
                    ECMBufCnt[DmxIdSect] ++;

                    pu8Ptr = MApi_NSK2_AllocateMemory(180*2, TRUE /*CACHED*/);
                    if(pu8Ptr == NULL)
                    {
                        NSK_ERROR(("null pointer from allocate memory\n"));
                        continue;
                    }
                    pECMBufBack[DmxIdSect][ECMBufCnt[DmxIdSect]] = pu8Ptr;

                    memcpy(pu8Ptr,&pu8VARead[8],180);

                    //printf("call NSK2HDX_EcmArrived, conn = %x, polarity = %x, DmxIdSect = %x\n",connection,ecm_polarity,DmxIdSect);
                    //printf("call NSK2HDX_EcmArrived, conn = %x, polarity = %x, DmxIdSect = %x\n",connection,ecm_polarity,DmxIdSect);
                    //printf("polarity=%x, ",ecm_polarity);
                    //printf("%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",pu8Ptr[0],pu8Ptr[1],pu8Ptr[2],pu8Ptr[3],pu8Ptr[4],pu8Ptr[5],pu8Ptr[6],pu8Ptr[7],pu8Ptr[8],pu8Ptr[9]);
                    //printf("before calling ecm arrive = %d\n",MsOS_GetSystemTime());
                    //connection needs to modify....
                    NSK2HDX_EcmArrived(connection, Pids[DmxIdSect], (NDS_UBYTE *)pu8Ptr/* RPC SIZECONST(256) */, ecm_polarity /*polarity*/);
                    //printf("end of NSK2HDX_EcmArrived\n");
                    //printf("end of calling ecm arrive = %d\n",MsOS_GetSystemTime());
                    //NSK2Util_PrintPacket(&pu8VARead[0],188);
                }
            }
        }

    ProcEnd:
        MsOS_ReleaseMutex(_s32ECMFLTMutexId);

    } // Task while loop
}



// Data callback function for event setting.
// Callback function inform user thread the section status.
// If in Polling mode, user thread need to polling filter status by themselves.
static void _NSK2_ECMDFLT_DataCb(MS_U8 u8DmxId, DMX_EVENT enFilterStatus)
{
    MS_U8 connection;

    if(enFilterStatus == DMX_EVENT_DATA_READY)
    {
    	//MsOS_DelayTaskUs(1000);
        connection = ECM_Connection[u8DmxId];
        //printf("ecm buffer DATA_READY, u8DmxId = %x, connection = %x\n",u8DmxId,connection);
		MsOS_SetEvent(_s32ECMFLTEventId, ( (u8DmxId) | (connection<<8) | ECMPacketArrive) );

        //this delay is to separate the same ID cause the event error....
    	MsOS_DelayTask(1);
	}
    else if(enFilterStatus == DMX_EVENT_BUF_OVERFLOW)
    {
        printf("ecm buffer overflow\n");
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Start(u8DmxId);
    }
}

/*****************************************************************************\
  \ global functions
\*****************************************************************************/


MS_U32 NSK2HDX_SetEcmLog(MS_U32 *bEn)
{
    _bEcm_log = *bEn;
    return TRUE;
}

//use our tsp to do ecm filter...
//so, the function body is porting on middleware directly...

MS_U32 NSK2_ECMFLT_Init(void)
{
    //NSK_TRACE(("enter NSK2_ECMFLT_Init\n"));
    if(_u8EcmFltIsInit == FALSE)
    {
        //NSK_TRACE(("CreateEventGroup\n"));
        _s32ECMFLTEventId = MsOS_CreateEventGroup("ECMFLT_Event");
        if (_s32ECMFLTEventId < 0)
        {
            NSK_ERROR(("CreateEvent fail\n"));
            return MDL_ECMFLT_FAIL;
        }
        //NSK_TRACE(("_s32ECMFLTEventId = %d\n",_s32ECMFLTEventId));

        _s32ECMFLTMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "ECMFLT_Mutex", MSOS_PROCESS_SHARED);
        if (_s32ECMFLTMutexId < 0)
        {
            NSK_ERROR(("CreateMutex fail\n"));
            MsOS_DeleteEventGroup(_s32ECMFLTEventId);
            return MDL_ECMFLT_FAIL;
        }

        _pECMDFLTTaskStack = MApi_NSK2_AllocateMemory(ECMFLT_TASK_STACK_SIZE, TRUE); //_u8ECMDFLT_StackBuffer;
        if(_pECMDFLTTaskStack == NULL)
        {
            NSK_ERROR(("null pointer from allocate memory\n"));

            return FALSE;
        }
        _s32ECMDFLTTaskId = MsOS_CreateTask((TaskEntry)_NSK2_ECMDFLT_Isr_Proc,
                                            (MS_U32)NULL,
                                            E_TASK_PRI_HIGH/*E_TASK_PRI_HIGH*/,
                                            TRUE,
                                            NULL,
                                            ECMFLT_TASK_STACK_SIZE,
                                            "ECMFLT_ISR_Task");
        if (_s32ECMDFLTTaskId < 0)
        {
            NSK_ERROR(("CreateTask fail\n"));
            MsOS_DeleteEventGroup(_s32ECMFLTEventId);
            MsOS_DeleteMutex(_s32ECMFLTMutexId);
            return MDL_ECMFLT_FAIL;
        }
        _u8EcmFltIsInit = TRUE;
    }
    else
    {
        //NSK_TRACE(("EMMFlt had been initialed\n"));
    }


	MS_U16 i;

	for(i=0;i<MaximumDmxId;i++)
	{
		ECM_Connection[i]=0xff;
	}

    NSK2HDX_SetEcmHwInfo(ECMRecvieBuf);
    return MDL_ECMFLT_OK;
}


NDS_STATUS32 NSK2HDX_RequestEcm(NDS_ULONG x_connection, NDS_USHORT ecm_pid)
{
    NSK_TRACE(("enxter NSK2HDX_RequestEcm\n"));
    NSK_TRACE(("x_connection = %x, ecm_pid = %x\n",x_connection,ecm_pid));

    MS_U8 pattern[16], mask[16], nmask[16];
    MS_U8                                  DmxIdSect;
    DMX_FILTER_TYPE                        FilterType;
    MS_U8*                                 pu8BufAddr;
    DMX_Flt_info                           FltInfo;
    MS_U16                                 u16Pid = ecm_pid;

    memset(pattern,0x0,16);
    memset(mask,0x0,16);
    memset(nmask,0x0,16);


    if(x_connection == 1)			//file In
	{
	    //char	FileName[50]="NSK2_D_EcmFilter.mpg";
	    if(XConnFileInFlag[x_connection] == FALSE)
        {
            XConnFileInFlag[x_connection] = TRUE;
            _gecm_pid[x_connection] = ecm_pid;
        }
        else
        {
            if(ecm_pid == _gecm_pid[x_connection])
                return NSK2HDX_OK;
        }

	    //FilterType = DMX_FILTER_TYPE_PACKET | DMX_FILTER_SOURCE_TYPE_FILE;
        FilterType = MApi_NSK2_EcmGetFilterType(x_connection);

        //MS_U32 pPktSize = 188;
        //appDemo_DmxFileIn_Start(FileName,&pPktSize);
	}
	else							//Live In if x_connection==0
	{
	    if(XConnFileInFlag[x_connection] == FALSE)
        {
            XConnFileInFlag[x_connection] = TRUE;
            _gecm_pid[x_connection] = ecm_pid;
        }
        else
        {
            if(ecm_pid == _gecm_pid[x_connection])
                return NSK2HDX_OK;
        }

		//FilterType = DMX_FILTER_TYPE_PACKET | DMX_FILTER_SOURCE_TYPE_LIVE;
        FilterType = MApi_NSK2_EcmGetFilterType(x_connection);
	}


    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        NSK_ERROR(("[%s] Allocate filter fail \n",__FUNCTION__));
    }

    NSK_TRACE(("Req DmxIdSect = %x \n",DmxIdSect));

    // Here we use pre-defined physical address of reserved section buffer.
    // [Note] The section buffe MUST be continus in physical address space.(for DMA usage)

    pu8BufAddr = (MS_U8*)MApi_NSK2_AllocateMemory(ECMDMX_BufferSize, FALSE);
    if(pu8BufAddr == NULL)
    {
        NSK_ERROR(("null pointer from allocate memory\n"));
        return NSK2HDX_FAIL;
    }
    memset(pu8BufAddr, 0 , ECMDMX_BufferSize);

    // Transfer Virtual Address to Phisical Hardware Address
    // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
    FltInfo.Info.SectInfo.SectBufAddr = MsOS_VA2PA((MS_VIRT)pu8BufAddr);

    //NSK_TRACE(("Physical Address = %08lx\n" ,FltInfo.Info.SectInfo.SectBufAddr));
    // Set buffer size
    FltInfo.Info.SectInfo.SectBufSize =     ECMDMX_BufferSize;
    //NSK_TRACE(("Size = %08lx\n" ,FltInfo.Info.SectInfo.SectBufSize));


    FltInfo.Info.SectInfo.SectMode    =  DMX_SECT_MODE_CONTI;

    // <DMX_EVENT_DATA_READY/DMX_EVENT_BUF_OVERFLOW>
    // Event trigger condition for driver, Driver will call ap's callback  < CallBack Mode Type2 >
    FltInfo.Info.SectInfo.Event        =     DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF ;

    // Set the pointer of the event CB function into Demux driver
    FltInfo.Info.SectInfo.pNotify =    &_NSK2_ECMDFLT_DataCb;

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxIdSect, &FltInfo, &FilterType, TRUE))
    {
        NSK_ERROR(("[%s] MApi_DMX_Info fail \n",__FUNCTION__));
    }

    // Set Filter PID --> Section PID
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxIdSect , &u16Pid , TRUE))
    {
        NSK_ERROR(("[%s] MApi_DMX_Pid fail \n",__FUNCTION__));
    }

    //keep PIDs, buffer.
    Pids[DmxIdSect] = u16Pid;
    ECMAllocBuf[DmxIdSect] = pu8BufAddr;

	//printf("Alloc ECM BUF:%x \n",ECMAllocBuf[DmxIdSect]);
    ECM_Connection[DmxIdSect] = x_connection;


    ECMBufCnt[DmxIdSect] = 0;
    ECMFlags[DmxIdSect][0] = ECMFlags[DmxIdSect][1] = 0;
    last_ecm_polarity[DmxIdSect] = 2;



    // Reset Section filter and section Buffer status;
    MApi_DMX_SectReset(DmxIdSect);


    // Set Section Match pattern
    // The Match pattern is used for Getting specific section
    // Pattern[16] =  Pattern for match
    // Mask[16]    =  Match Mask : set 1 for match / 0 for ignore
    MApi_DMX_SectPatternSet(DmxIdSect, pattern, mask, nmask, 16);


    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxIdSect))
    {
        NSK_ERROR(("enable section filter fail\n"));
    }

    NSK_TRACE(("leave NSK2HDX_RequestEcm\n"));

    return NSK2HDX_OK;
}


NDS_STATUS32 NSK2HDX_StopEcm(NDS_ULONG x_connection, NDS_USHORT ecm_pid)
{

    MsOS_ObtainMutex(_s32ECMFLTMutexId,MSOS_WAIT_FOREVER);

    //NSK_TRACE(("enxter NSK2HDX_StopEcm\n"));
    NSK_TRACE(("x_connection = %x, ecm_pid = %x\n",x_connection,ecm_pid));

	MS_U16 DmxIdSect = 0;

	MS_U16 FoundInList[0xff]={0};
	MS_U8 Founded = 0;

    XConnFileInFlag[x_connection] = FALSE;

#if 0
    if(x_connection == 1)
    {
        appDemo_DmxFileIn_Stop();
    }
#endif
    //it needs to free DMX filter.......

	for(DmxIdSect = 0 ; DmxIdSect<MaximumDmxId; DmxIdSect++)
	{
		//printf("Q_DmxIdSect:%x\n",ECM_Connection[DmxIdSect]);
	}

		for(DmxIdSect = 0 ; DmxIdSect<MaximumDmxId; DmxIdSect++)
	{
		//printf("Q_Pids[DmxIdSect]:%x\n",Pids[DmxIdSect]);
	}

	for(DmxIdSect = 0 ; DmxIdSect<MaximumDmxId; DmxIdSect++)
    {
        if ((ECM_Connection[DmxIdSect] == x_connection) && (ecm_pid == Pids[DmxIdSect]))
        {
        	ECM_Connection[DmxIdSect] = 0xff;
            FoundInList[DmxIdSect] = 1;
			Founded = 1;
            NSK_TRACE(("DmxIdSect found is = %d\n",DmxIdSect));
            break;
        }
    }


    if(Founded == 0)
    {
        NSK_ERROR(("can't find pid to stop\n"));
		MsOS_ReleaseMutex(_s32ECMFLTMutexId);
		return NSK2HDX_INVALID_REQUEST;
    }
    else
    {
        NSK_TRACE(("DmxIdSect = %x\n",DmxIdSect));
    }


    for(DmxIdSect = 0 ; DmxIdSect<MaximumDmxId; DmxIdSect++)
    {
        if( FoundInList[DmxIdSect] == 1)
        {
        	// Stop Filter
    	 	MApi_DMX_Stop(DmxIdSect);
        	MApi_DMX_Close(DmxIdSect);

        	Pids[DmxIdSect] = 0xffff;

        	// Free Filter.
        	//MApi_DMX_Close(DmxIdSect);

        	Pids[DmxIdSect] = 0xffff;
        	ECMBufCnt[DmxIdSect] = 0;
        	ECMFlags[DmxIdSect][0] = ECMFlags[DmxIdSect][1] = ECMStopFlag;

        	//free allocate buffer...Jeff, need to implement
        	//MsOS_FreeMemory((MS_U8*)ECMAllocBuf[DmxIdSect], gs32NonCachedPoolID);
        }

    }
    NSK_TRACE(("leave NSK2HDX_StopEcm\n"));

    MsOS_ReleaseMutex(_s32ECMFLTMutexId);

    return NSK2HDX_OK;
}
NDS_STATUS32 NSK2HDX_EcmProcessingFinished(NDS_USHORT ecm_pid, NDS_UBYTE *ecm_buf/* RPC SIZECONST(256) */, NDS_UBYTE polarity)
{
    NSK_TRACE(("ecm_buf = 0x%p, ecm_pid = 0x%x, polarity = 0x%x\n",ecm_buf,ecm_pid,polarity));

    MS_U8 DmxIdSect = 0;
    MS_U8 i,found_in_list;

    found_in_list=0;
    //it needs to free DMX filter.......

    for(DmxIdSect = 0 ; DmxIdSect<MaximumDmxId; DmxIdSect++)
    {
        for(i=0;i<ECMRecvieBuf;i++)
	{
	    if(ecm_buf == pECMBufBack[DmxIdSect][i])
	    {
	        pECMBufBack[DmxIdSect][i] = 0;
		found_in_list = 1;
		break;
	    }
	}
	if(found_in_list == 1)
	{
	    break;
	}
    }


    //printf("Free DmxIdSect:%x\n",DmxIdSect);

    ECMBufCnt[DmxIdSect] --;
    ECMFlags[DmxIdSect][polarity] = 0;

    if(plastECM != ecm_buf)	//patch double release bug of verifier lib, 2014Dec15
        MApi_NSK2_FreeMemory(ecm_buf, TRUE);

    plastECM = ecm_buf;

    NSK_TRACE(("leave NSK2HDX_EcmProcessingFinished\n"));

    return NSK2HDX_OK;
}
