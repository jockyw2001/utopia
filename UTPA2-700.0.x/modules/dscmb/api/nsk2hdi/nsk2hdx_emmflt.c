
#include <string.h>
#include "MsCommon.h"

#include "hdi_121.h"
#include "nsk_dbg.h"
#include "drvEMMFlt.h"
#include "drvNSK2Type.h"
#include "nsk2hdi_header.h"
#include "nsk2hdx_header.h"

/*****************************************************************************\
  \ local defines
\*****************************************************************************/
#define HARMRetFrom8        8
#define EMMReturnStart      HARMRetFrom8

//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
static void *pDstBuf;
static MS_BOOL _bStop = FALSE;
//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------

/*****************************************************************************\
  \ local functions
\*****************************************************************************/

void NSK2_EMMFLT_Notify(MS_U32 conn, MS_U32 reason, void *pData)
{
    NDS_UBYTE *pEMMBuf;
    //printf("************************************NSK2_EMMFLT_Notify\n");

    if(EMM_EVENT_ARRIVE == reason)
    {
        //printf("************************************EMM_EVENT_ARRIVE\n");
        pEMMBuf = (NDS_UBYTE *)pData;
        //MsOS_ReadMemory();
        //printf("BYTE 29, 30 = %x, %x\n",pEMMBuf[28+EMMReturnStart],pEMMBuf[29+EMMReturnStart]);
        //printf("pData = %x\n", (MS_U32)pData);
        //printf("emm return buf = %x\n", (MS_U32)&pEMMBuf[EMMReturnStart]);
        if(_bStop == FALSE)
        {
            NSK2HDX_EmmArrived(conn, &pEMMBuf[EMMReturnStart]/* RPC SIZECONST(256) */);
        }
    }
    else if(EMM_EVENT_OVERFLOW == reason)
    {
        NSK_TRACE(("EMM_EVENT_OVERFLOW\n"));
        NSK2HDX_EmmBufferOverflow(conn);
    }
}

MS_U32 NSK2_EMMFLT_Init(EMM_Init_Paras_t *pInitPara)
{
    EMMFLT_RESULT init_result, bufcnt_result;
    EMMFLT_Require_t tRequre;
    MS_PHY u64AlignAddr;
    MS_U32 BufCnt;

    MDrv_EMMFLT_SetDbgLevel(EMM_DBGLV_INFO);

    memset(&tRequre,0x0,sizeof(tRequre));
    init_result = MDrv_EMMFLT_Init(&tRequre);
    NSK_TRACE(("test by Jeff, init_result = %x \n",init_result));

    //MDrv_EMMFLT_SrcSelect(0, EMM_SRC_LIVEIN, EMM_SRC_INTERDEMOD_0);
    MDrv_EMMFLT_SrcSelect(0, pInitPara->u32SrcFrom, pInitPara->u32SrcType);


    //allocate one memory for EMM fliter hw use....
    pDstBuf = MApi_NSK2_AllocateMemory((tRequre.u32DstBufSize + tRequre.u32AlignBytes), FALSE);
    if(pDstBuf == NULL)
    {
        NSK_ERROR(("null pointer from allocate memory\n"));
        return FALSE;
    }

    memset(pDstBuf,0x0,(tRequre.u32DstBufSize + tRequre.u32AlignBytes));

    u64AlignAddr = MsOS_VA2PA((MS_VIRT)pDstBuf);

    u64AlignAddr = ( (u64AlignAddr + tRequre.u32AlignBytes) & (~(tRequre.u32AlignBytes-1)) );
    NSK_TRACE(("u64AlignAddr = 0x%llx, AllocBuf = %p\n",u64AlignAddr,pDstBuf));


    MDrv_EMMFLT_SetDstBufInfo(0,(MS_U32)u64AlignAddr, tRequre.u32DstBufSize);

    bufcnt_result = MDrv_EMMFLT_GetHwBufCnt(&BufCnt);
    NSK2HDX_SetEmmHwInfo(BufCnt);
    MDrv_EMMFLT_SetNotify(&NSK2_EMMFLT_Notify);
    return TRUE;
}

NDS_STATUS32 NSK2HDX_RequestEmm(NDS_ULONG x_connection, NSK2HDX_EMM_FILTER *emmfl/* RPC IN OUT */)
{
    MS_U16 i;

    //printf("enxter NSK2HDX_RequestEmm\n");
    NSK_INFO(("enxter x_connection = %x \n",x_connection));

    //MDrv_EMMFLT_SrcSelect(0, EMM_SRC_LIVEIN, EMM_SRC_INTERDEMOD_0);

    NSK_INFO(("pid = %x, num_of_filters = %x\n",emmfl->emm_pid,emmfl->num_of_filters));
    for(i=0;i<16;i++)
    {
        NSK_INFO(("table_id[%x]=%x, table_id_flag[%x]=%x\n",i,emmfl->table_id[i],i,emmfl->table_id_flag[i]));
    }

    for(i=0;i<emmfl->num_of_filters;i++)
    {
        if(i>7)
        {
            break;
        }
        NSK_INFO(("Address[%x] = (%x, %x, %x, %x)\n",i, emmfl->filter_def[i].filter_address[0], \
            emmfl->filter_def[i].filter_address[1],emmfl->filter_def[i].filter_address[2],emmfl->filter_def[i].filter_address[3]));

        NSK_INFO(("Mask[%x] = (%x, %x, %x, %x)\n",i, emmfl->filter_def[i].filter_address_mask[0], \
            emmfl->filter_def[i].filter_address_mask[1],emmfl->filter_def[i].filter_address_mask[2],emmfl->filter_def[i].filter_address_mask[3]));
        NSK_INFO(("type = %x\n",emmfl->filter_def[i].filter_type));
    }

#if 0
    emmfl->filter_def[0].filter_address[3] = 0x8;
    emmfl->filter_def[1].filter_address_mask[3] = 0x0;
    emmfl->filter_def[2].filter_address_mask[0] = emmfl->filter_def[2].filter_address_mask[1] = 0x0;
    emmfl->filter_def[2].filter_address_mask[2] = emmfl->filter_def[2].filter_address_mask[3] = 0x0;
    emmfl->filter_def[2].filter_address[0] = 0x1;
    emmfl->filter_def[2].filter_address[1] = 0x2;
    emmfl->filter_def[2].filter_address[2] = 0x3;
    emmfl->filter_def[2].filter_address[3] = 0x4;
#endif

    emmfl->emm_pid |= 0x8000;

    MS_U8 TID_Enable = FALSE;
    MS_U8 num_of_flt,u8I;
    EMM_FILTER_DEF_t tEMMFilter;
    //check TID flag.....
    for(i = 0; i< 16 ; i++)
    {
        if(emmfl->table_id[i] == 0)
        {
            MDrv_EMMFLT_SetTidMode(x_connection,(i&0xf),EMM_TIDMODE_INGORE);
            continue;
        }

        TID_Enable = TRUE;
        //DRVEMM_DBG(EMM_DBGLV_DEBUG,"table_id(%d) = %x\n",u8I,pEmmFlt->table_id[u8I]);

        if(emmfl->table_id_flag[i] == NSK2HDX_DISABLE_TID)
        {
            MDrv_EMMFLT_SetTidMode(x_connection,(i&0xf),EMM_TIDMODE_INGORE);
        }
        else if(emmfl->table_id_flag[i] == NSK2HDX_ENABLE_TID)
        {
            MDrv_EMMFLT_SetTidMode(x_connection,(i&0xf),EMM_TIDMODE_ACCEPT_ALL);
        }
        else if(emmfl->table_id_flag[i] == NSK2HDX_ENABLE_TID_NDS)
        {
            MDrv_EMMFLT_SetTidMode(x_connection,(i&0xf),EMM_TIDMODE_ACCEPT_ADDRESS);
        }
        else
        {
            MDrv_EMMFLT_SetTidMode(x_connection,(i&0xf),EMM_TIDMODE_RESERVED);
        }
    }

	NSK_INFO(("Set PID\n"));
    MDrv_EMMFLT_SetEmmPID(x_connection,emmfl->emm_pid);

	NSK_INFO(("Enable TID\n"));
    MDrv_EMMFLT_EnableEmmTID(x_connection,TID_Enable);

    if(emmfl->num_of_filters > 8)	//fixed bug 6th
    {
    	NSK_ERROR(("num_of_filters > 8 !!!!!\n"));
        return NSK2HDX_INVALID_REQUEST;
    }
    else
    {
        num_of_flt = (MS_U8) emmfl->num_of_filters;
    }

	NSK_INFO(("Set Filter CFG\n"));
    for(u8I = 0; u8I< num_of_flt ; u8I++)
    {
        memset(&tEMMFilter,0x0,sizeof(EMM_FILTER_DEF_t));
        memcpy(&tEMMFilter,&emmfl->filter_def[u8I],sizeof(EMM_FILTER_DEF_t));
        MDrv_EMMFLT_SetFilterCfg(x_connection,u8I,&tEMMFilter);
    }

    _bStop = FALSE;
    //fire EMM filter to wait stream...
    MDrv_EMMFLT_Fire(x_connection);

    NSK_INFO(("leave NSK2HDX_RequestEmm\n"));
    return NSK2HDX_OK;
}

NDS_STATUS32 NSK2HDX_StopEmm(NDS_ULONG x_connection)
{
    NSK_TRACE(("enxter NSK2HDX_StopEmm\n"));
    _bStop = TRUE;
    MDrv_EMMFLT_StopEmm(x_connection);
    NSK_TRACE(("leave NSK2HDX_StopEmm\n"));
    return NSK2HDX_OK;
}

NDS_STATUS32 NSK2HDX_EmmProcessingFinished(NDS_UBYTE *emm_buf/* RPC SIZECONST(256) */)
{
    NSK_TRACE(("enxter NSK2HDX_EmmProcessingFinished\n"));
    //MsOS_DelayTask(10);

    //printf("enxter NSK2HDX_EmmProcessingFinished \n");
    //return NSK2HDX_OK;
    MDrv_EMMFLT_EmmProcessingFinished(emm_buf-EMMReturnStart);
    //printf("leave NSK2HDX_EmmProcessingFinished \n");
    //NSK_TRACE(("leave NSK2HDX_EmmProcessingFinished\n"));
    return NSK2HDX_OK;
}


