

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif

#include "MsCommon.h"
#include "drvDSCMB.h"
#include "drvNSK2.h"
#include "nsk2hdi_header.h"
#include "drvNSK2Type.h"
#include "apiDMX.h"
#include "drvMMIO.h"
#include "nsk_dbg.h"

/*****************************************************************************\
  \Definitions
\*****************************************************************************/

#define CMChannel_ID1                       0x01234567
#define CMChannel_ID2                       0x02461357

#define NSK2_CMCHANNEL_NUM                  256
#define NSK2_M2MCHANNEL_NUM                 16

#define NSK2_CM_ESPID_NUM                   256
#define MaxCAPidProxyNum                    256

#define GetCAPidInfo

#define CMChannelDescSize                   30
#define CmChannelGroupMaxNumber             2

#define CmChannelCookieSize                 0x1000

#define DefaultDSCMBID                      0xFFFF
#define DefaultPidFltID                     0xFFFF
#define DefaultPID                          0xFFFF

#define CAVID                               1

#define MaxXConnection                      2
/*****************************************************************************\
  \ typedef struct
\*****************************************************************************/
typedef enum
{
    ESA_SELECT_AES = 0,
    ESA_SELECT_DVBCSA2 = 1,
    ESA_SELECT_DES = 2,
    ESA_SELECT_TDES = 3,
    ESA_SELECT_MULTI2 = 4,
    ESA_SELECT_DVBCSA2_CONF = 5,
    ESA_SELECT_DVBCSA3 = 6,
} ESASelectMap_e;


typedef enum
{
    ESASUB_SELECT_SCTE41 = 0,
    ESASUB_SELECT_SCTE52 = 1,

    ESASUB_SELECT_MDI_CBC = 0,
    ESASUB_SELECT_SYNAMEDIA_AES = 1,
    ESASUB_SELECT_AES_ECB_CLR = 3,
    ESASUB_SELECT_CIPLUS_AES = 4,

    ESASUB_SELECT_NONE = 0,
} ESASubSelectMap_e;


typedef enum{
    CMCHANNEL_LDA_DESC = 0,
    CMCHANNEL_ESA_DESC    ,
    CMCHANNEL_LSA_DESC    ,
    CMCHANNEL_MAX_DESC    ,
}CMCHANNEL_DESC_e;


typedef struct{
    NSK2HDI_DEVICE_ID CmChGroup_ID;

}   CmChannelGroup_t;


typedef struct _CaPID_Info_
{
    MS_BOOL         alloc;
    MS_BOOL         enable;
    MS_U16          es_pid;
    MS_U32          handle_id;
    MS_U8           dscmb_id;
    MS_U32          cm_handle_id;
    MS_U32          xconn;
    MS_U32          ProxyGlobalIndex;

    MS_U32          PrimeFilterID;
    MS_U16          PrimePID;

    MS_U8           secondary_enable;
    MS_U8           SecondaryFilterID;
    MS_U16          SecondaryPID;
    void            *pCmCookie;
} CaPID_Info;

typedef struct
{
    NDS_UBYTE  type[NSK2_CMCHANNEL_NUM];
    NDS_USHORT es_pid[NSK2_CMCHANNEL_NUM];
    MS_U8      PidNo[NSK2_CMCHANNEL_NUM];
    MS_U32     DscmbId[NSK2_CMCHANNEL_NUM];
    MS_U32     used[NSK2_CMCHANNEL_NUM];
} AddXconnInfo_t;

typedef struct _CmChannelTable_Info_
{
    MS_BOOL         alloc;
    NDS_ULONG       CmChTable_ID;
    MS_U32          handle_id;
} CmChannelTable_Info;

typedef struct _CmChannelGroup_Info_
{
    MS_BOOL         alloc;
    NDS_ULONG       CmChGroup_ID;
    MS_U32          handle_id;
} CmChannelGroup_Info;


typedef struct _cmchannel_algorithm_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 algorithm;
} CmChannel_Algo_Desc_t;

typedef struct _cmchannel_iv_signaling_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 iv_signaling;
} CmChannel_IV_Signal_Desc_t;

typedef struct _cmchannel_iv1_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 iv1_payload[0x10];
} CmChannel_IV1_Desc_t;

typedef struct _cmchannel_iv2_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 iv2_payload[0x10];
} CmChannel_IV2_Desc_t;

typedef struct {
    CmChannel_Algo_Desc_t ESA_Algo;
    CmChannel_IV_Signal_Desc_t ESA_IV_Signal;
    CmChannel_IV1_Desc_t ESA_IV1_Payload;
    CmChannel_IV2_Desc_t ESA_IV2_Payload;
}ESA_Desc_St_t;


typedef struct{
    NDS_UBYTE Algorithm;
    NDS_UBYTE IV_Mode;
    NDS_UBYTE IV1_Enable;
    NDS_UBYTE IV2_Enable;
    NDS_UBYTE IV1[0x10];
    NDS_UBYTE IV2[0x10];
}CmChDesc_Parser_t;


typedef struct
{
    MS_U32          u32DscmbId;
    MS_U32          PidSlotID;
    MS_U32          u16Pid;
#if 0
    NDS_ULONG       lda_desc_size;
    NDS_UBYTE       *lda_desc;
    NDS_ULONG       esa_desc_size;
    NDS_UBYTE       *esa_desc;
    NDS_ULONG       lsa_desc_size;
    NDS_UBYTE       *lsa_desc;
    NDS_UBYTE       lda_iv1_payload[0x10];
    NDS_UBYTE       lda_iv2_payload[0x10];
    NDS_UBYTE       esa_iv1_payload[0x10];
    NDS_UBYTE       esa_iv2_payload[0x10];
    NDS_UBYTE       lsa_iv1_payload[0x10];
    NDS_UBYTE       lsa_iv2_payload[0x10];
#endif
    MS_BOOL         bWriteESA;
    NDS_UBYTE       ESA_Algorithm;
    CmChDesc_Parser_t  CmChDesc[CMCHANNEL_MAX_DESC];
    MS_BOOL          bConnect;
} CmChannelDevice_Cookie_t;


typedef struct _NSK2_CMChannel
{
    MS_BOOL                         alloc;
    MS_BOOL                         enable;
    MS_U32                          nsk_sw;
//    MS_U32                          esa_sel;
//    MS_U32                          esa_subsel;
    MS_U32                          dsc_id;
    DSCMB_Type                      dsc_type;
    MS_U32                          FilterID;
    MS_U32                          PidSlotID;
    MS_U32                          handle_id;
    MS_U32                          es_pid;

    CmChDesc_Parser_t               CmChDesc[CMCHANNEL_MAX_DESC];
    CmChannelDevice_Cookie_t        CmChCookie;
} NSK2_CMChannel_Info;


typedef struct _CaPidProxy
{
    MS_U16  EsPid;
    MS_U32  DscmbId;
    MS_BOOL IsUsed;
}CaPidProxy_t;

/*****************************************************************************\
  \ Local variables
\*****************************************************************************/

MS_U32  _u32KeepPidByFltId[MaxXConnection][NSK2_CMCHANNEL_NUM];
CaPidProxy_t _KeepCaPidProxy[MaxXConnection][NSK2_CMCHANNEL_NUM];
static MS_U32 _gu32WriteKeyTime = 0;
static MS_U32 _u32CaSoCGroupHandle = 0;

/*****************************************************************************\
  \ Global Variables
\*****************************************************************************/

void *pCmChannelcookie[NSK2_CMCHANNEL_NUM];
void *pM2MChannelcookie[NSK2_M2MCHANNEL_NUM];

AddXconnInfo_t HDI_XconnInfo[2];

NSK2HDI_CHILD_DEVICE _gChild_Device[4];

CmChannelTable_Info CmChannelTableInfo;
CmChannelGroup_Info CmChannelGroupInfo[CmChannelGroupMaxNumber];

void *CmChannelcookie;
CmChannelDevice_Cookie_t *pCmChCookie;

CmChDesc_Parser_t gCmChannelDesc[NSK2_CMCHANNEL_NUM][CMCHANNEL_MAX_DESC];


/*****************************************************************************\
  \ Global Functions
\*****************************************************************************/
MS_BOOL MDrv_DSCMB2_SetPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec);
MS_BOOL MDrv_DSCMB2_ClearPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec);


/*****************************************************************************\
  \ TSP Control functions
\*****************************************************************************/

//The extra functions for TSP control
//K7 can separate write key and connect to TSP filter ID
//The scenario is changed.
MS_U32 NSKHDI_AddDscmbConnect(NDS_ULONG  xconn,
                              NDS_USHORT es_pid,
                              MS_U32     u32FltID)
{
    NSK_TRACE(("xconn = 0x%x, es_pid = 0x%04x, FltID = %d\n",xconn,es_pid,u32FltID));

    MS_U32 u32DscmbId, i;

    for( i=0; i<NSK2_CMCHANNEL_NUM ; i++)
    {
        if( (_KeepCaPidProxy[xconn][i].EsPid == es_pid) && (_KeepCaPidProxy[xconn][i].IsUsed == TRUE))
            break;
    }

    if( i == NSK2_CMCHANNEL_NUM ) // the CA PID Proxy is not open with this PID, TSP PID add first...
    {
        _u32KeepPidByFltId[xconn][u32FltID] = es_pid;
        NSK_TRACE(("Add TSP filter first, not connect to dscmb\n"));
        return TRUE;
    }

    u32DscmbId = _KeepCaPidProxy[xconn][i].DscmbId;
    _u32KeepPidByFltId[xconn][u32FltID] = es_pid;

    NSK_TRACE(("already open dscmb handle, connect to dscmb\n"));
    if(MDrv_DSCMB2_FltConnectFltId(0,u32DscmbId, u32FltID) == FALSE)
    {
        NSK_ERROR(("MDrv_DSCMB_FltConnectPid() failed\n"));
        return FALSE;
    }

    return TRUE;
}

MS_U32 NSKHDI_RemoveDscmbConnect(NDS_ULONG xconn,
                                 NDS_USHORT es_pid)
{

    NSK_TRACE(("Enter\n"));
    MS_U32 i = 0, j=0, u32FltID;
    MS_U32 u32DscmbId;
    MS_BOOL discon_flag = TRUE;

    for( i=0; i<NSK2_CMCHANNEL_NUM ; i++)
    {
        if( _u32KeepPidByFltId[xconn][i] == es_pid)
            break;
    }


    if( i == NSK2_CMCHANNEL_NUM ) // the pid is not added.....
    {
        NSK_ERROR(("can't not find match pid filter\n"));
        return FALSE;
    }

    u32FltID = i;

    for( j=0; j<NSK2_CMCHANNEL_NUM ; j++)
    {
        if( (_KeepCaPidProxy[xconn][j].EsPid == es_pid) && (_KeepCaPidProxy[xconn][j].IsUsed == TRUE))
            break;
    }

    if( j == NSK2_CMCHANNEL_NUM ) // the CA PID Proxy is already remove this proxy, almost means already disconnect dscmb
    {
        NSK_TRACE(("can't find match pid proxy\n"));
        discon_flag = FALSE;

    }

    if(TRUE == discon_flag)
    {
        u32DscmbId = _KeepCaPidProxy[xconn][j].DscmbId;
        MDrv_DSCMB2_FltDisconnectFltId(0, u32DscmbId, u32FltID);
    }


    //MApi_NSK2_DTV_ClosePidNo(u32FltID);

    _u32KeepPidByFltId[xconn][u32FltID] = DefaultPID;

    NSK_TRACE(("Exit\n"));

    return TRUE;
}

/*****************************************************************************\
  \ NSK2HDI device related functions
\*****************************************************************************/

/*****************************************************************************\
  \ CA SoC Group device
\*****************************************************************************/


NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_Open (NSK2HDI_DEVICE_ID CaSoCGroup_ID,
                                             NSK2HDI_HANDLE    *CaSoCGroup_handle)
{
    NSK_TRACE(("Enter\n"));

    NSK_TRACE(("CaSoCGroup_ID = %x\n",CaSoCGroup_ID));


    *CaSoCGroup_handle = (NSK2HDI_HANDLE *)&_u32CaSoCGroupHandle;

    //memset(*CaSoCGroup_handle, 0x0 , 4);

    NSK_TRACE(("CaSoCGroup_handle = %p\n",*CaSoCGroup_handle));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_Close (NSK2HDI_HANDLE CaSoCGroup_handle)
{
    NSK_TRACE(("Enter\n"));

    NSK_TRACE(("CaSoCGroup_handle = %p\n",CaSoCGroup_handle));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_EnumerateChildren( NSK2HDI_HANDLE       CaSoCGroup_handle,
                                                          NDS_ULONG            *child_device_num,
                                                          NSK2HDI_CHILD_DEVICE *child_device)
{
    NSK_TRACE(("Enter\n"));
    NSK_TRACE(("CaSoCGroup_handle = %p\n",CaSoCGroup_handle));
//#define NSK2HDI_OTP_DEVICE_TYPE							0x00000002U /*!< @brief OTP device */
//#define NSK2HDI_NSK_DEVICE_TYPE							0x00000003U /*!< @brief NSK device */
//#define NSK2HDI_CMCHANNEL_TABLE_DEVICE_TYPE			    0x00000004U /*!< @brief CM Channel Table device */
//#define NSK2HDI_M2MTABLE_DEVICE_TYPE			            0x00000008U /*!< @brief M2M Table device */

    NSK_TRACE(("child_device_num = %x\n",*child_device_num));

    if(child_device == NULL)
    {
        //should fill the number of child device...
        *child_device_num = 4;
        return NSK2HDI_STATUS_OK;
    }

	*child_device_num = 4;

    child_device[0].device_id = 0x3333;
    child_device[0].device_type= NSK2HDI_NSK_DEVICE_TYPE;
    child_device[1].device_id = 0x4444;
    child_device[1].device_type= NSK2HDI_CMCHANNEL_TABLE_DEVICE_TYPE;
    child_device[2].device_id = 0x2222;
    child_device[2].device_type= NSK2HDI_OTP_DEVICE_TYPE;
    child_device[3].device_id = 0x8888;
    child_device[3].device_type= NSK2HDI_M2MTABLE_DEVICE_TYPE;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_RegisterEnumerationChangeCB (NSK2HDI_HANDLE                   CaSoCGroup_handle,
                                                                    HDINSK2_ENUMERATION_CHANGE_CB_FN *cb_routine,
                                                                    void                             *cookie)
{
    NSK_TRACE(("Enter\n"));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}



/*****************************************************************************\
  \ Content Module - Table device
\*****************************************************************************/



NSK2HDI_STATUS NSKHDI_CmChannelTable_Open (NSK2HDI_DEVICE_ID CmChannelTable_ID,
                                           NSK2HDI_HANDLE    *CmChannelTable_handle)
{
    NSK_TRACE(("Enter\n"));

    if(CmChannelTableInfo.alloc == TRUE)
    {
        NSK_ERROR(("CmChannelTable is already open\n"));
    }

    CmChannelTableInfo.CmChTable_ID = CmChannelTable_ID;
    CmChannelTableInfo.alloc = TRUE;
    CmChannelTableInfo.handle_id = 1;

    *CmChannelTable_handle = (NSK2HDI_HANDLE *)&CmChannelTableInfo.handle_id;

    NSK_TRACE(("CmChannelTable_ID = 0x%x, CmChannelTable_handle = 0x%08x\n",CmChannelTable_ID,CmChannelTableInfo.handle_id));
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelTable_Close (NSK2HDI_HANDLE CmChannelTable_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(CmChannelTable_handle);
    NSK_TRACE(("CmChannelTable_handle = 0x%08x\n",u32Handle));

    CmChannelTableInfo.alloc = FALSE;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelTable_EnumerateChildren( NSK2HDI_HANDLE       CmChannelTable_handle,
                                                        NDS_ULONG            *child_device_num,
                                                        NSK2HDI_CHILD_DEVICE *child_device)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 cm_ch_num;

    if(child_device == NULL)
    {
        //should fill the number of child device...
        *child_device_num = MDrv_NSK2_CMChannelNum();
        NSK_TRACE(("return child_device_num = %x\n",*child_device_num));
        return NSK2HDI_STATUS_OK;
    }
	*child_device_num = MDrv_NSK2_CMChannelNum();

    NSK_TRACE(("child_device_num = %x\n",*child_device_num));

    cm_ch_num = *child_device_num;

    if(cm_ch_num>2)
    {
        NSK_ERROR(("cm_ch_num>2\n"));
    }

    child_device[0].device_id = CMChannel_ID1;
    child_device[0].device_type = NSK2HDI_CMCHANNEL_GROUP_DEVICE_TYPE;

    if(cm_ch_num == 2)
    {
        child_device[1].device_id = CMChannel_ID2;
        child_device[1].device_type = NSK2HDI_CMCHANNEL_GROUP_DEVICE_TYPE;
    }

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

/*****************************************************************************\
  \ Content Module - Group device
\*****************************************************************************/


NSK2HDI_STATUS NSKHDI_CmChannelGroup_Open (NSK2HDI_DEVICE_ID CmChannelGroup_ID,
                                           NSK2HDI_HANDLE    *CmChannelGroup_handle)
{
    NSK_TRACE(("Enter\n"));
    CmChannelGroup_t *pGroup;

    //check CmChannel Group ID.

    if( (CmChannelGroup_ID != CMChannel_ID1) && (CmChannelGroup_ID != CMChannel_ID2) )
    {
        NSK_ERROR(("CmChannelGroup_ID is wrong\n"));
        return NSK2HDI_STATUS_FAILED;
    }

    pGroup = MApi_NSK2_AllocateMemory(sizeof(CmChannelGroup_t), TRUE);
    if(NULL == pGroup)
    {
        NSK_ERROR(("cm channel group allocate buffer error\n"));
        return NSK2HDI_STATUS_FAILED;
    }

    memset(pGroup, 0x0, sizeof(CmChannelGroup_t));

    pGroup->CmChGroup_ID = CmChannelGroup_ID;

    *CmChannelGroup_handle = (NSK2HDI_HANDLE*)pGroup;

    NSK_TRACE(("CmChannelGroup_ID = %x, CmChannelGroup_handle = %p\n",CmChannelGroup_ID,pGroup));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelGroup_Close (NSK2HDI_HANDLE CmChannelGroup_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_BOOL bFlag;

    bFlag = MApi_NSK2_FreeMemory(CmChannelGroup_handle, TRUE);

    if(bFlag == FALSE)
    {
        NSK_ERROR(("free memory error\n"));
    }

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelGroup_GetProperties( NSK2HDI_HANDLE  CmChannelGroup_handle,
                                                    NDS_ULONG       request_id,
                                                    NDS_ULONG       *desc_size,
                                                    NDS_UBYTE       *desc)
{
    CmChannelGroup_t *pGroup = (CmChannelGroup_t *)CmChannelGroup_handle;
    NSK_TRACE(("Enter\n"));
    NSK_TRACE(("CmChGroup_ID = %x, request_id = %x, desc_size = %x\n",pGroup->CmChGroup_ID, request_id, *desc_size));


    if(NSK2HDI_CMCHANNELGROUP_READ_ALL_DESCRIPTORS_REQUEST != request_id)
    {
        NSK_ERROR(("error request_id\n"));
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }


    if(desc == NULL)
    {
        MDrv_NSK2_GetCMProperties((MS_U32*)desc_size, (MS_U8*)desc);
        NSK_TRACE(("return desc_size = 0x%08x\n",(MS_U32)*desc_size));
        return NSK2HDI_STATUS_OK;
    }

    if(*desc_size == 0)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    MDrv_NSK2_GetCMProperties((MS_U32*)desc_size, (MS_U8*)desc);
    NSK_TRACE(("Exit\n"));

    return NSK2HDI_STATUS_OK;
}

/*****************************************************************************\
  \ Content Module - Device
\*****************************************************************************/


NSK2HDI_STATUS NSKHDI_CmChannelDevice_Open (NSK2HDI_DEVICE_ID CmChannelGroup_ID,
                                            NSK2HDI_HANDLE    *CmChannele_handle)
{
    NSK_TRACE(("Enter\n"));

    NSK2_CMChannel_Info *cm_handle;
    NSK_TRACE(("CmChannelGroup_ID = 0x%08x\n",(MS_U32)CmChannelGroup_ID));

    cm_handle = MApi_NSK2_AllocateMemory(sizeof(NSK2_CMChannel_Info), TRUE);
    if(NULL == cm_handle)
    {
        NSK_ERROR(("cm handle allocate buffer error\n"));
        return NSK2HDI_STATUS_FAILED;
    }

    memset(cm_handle, 0x0, sizeof(NSK2_CMChannel_Info));

    cm_handle->dsc_id = cm_handle->CmChCookie.u32DscmbId = MDrv_DSCMB2_FltAlloc_Ex(0,E_DSCMB_FLT_3_KEYS);

    NSK_TRACE(("CmChannele_handle = %p, CmChannelGroup_ID = 0x%08x\n",cm_handle,(MS_U32)CmChannelGroup_ID));
    NSK_TRACE(("dsc_id = 0x%08x\n",cm_handle->dsc_id));

    if(DRV_DSCMB_FLT_NULL == cm_handle->dsc_id)
    {
        NSK_ERROR(("dscmb alloc flt fail\n"));
        *CmChannele_handle = NSK2HDI_ILLEGAL_HANDLE;
        MApi_NSK2_FreeMemory(cm_handle, TRUE);
        return NSK2HDI_STATUS_FAILED;
    }

    *CmChannele_handle = cm_handle;
    NSK_TRACE(("Exit, CmChannelDevice_handle = %p\n", *CmChannele_handle));

    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelDevice_Close (NSK2HDI_HANDLE CmChannele_handle)
{

    NSK_TRACE(("Enter CmChannelDevice_handle = %p\n",CmChannele_handle));
    NSK2_CMChannel_Info *cm_handle;

    if(NULL == CmChannele_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    cm_handle = (NSK2_CMChannel_Info *)CmChannele_handle;

    NSK_TRACE(("dsc_id = %x\n",cm_handle->dsc_id));

    if(cm_handle->CmChCookie.bConnect == TRUE)
    {
        MDrv_DSCMB2_FltDisconnectFltId(0,cm_handle->dsc_id,cm_handle->CmChCookie.PidSlotID);
    }
    //should stop...de-allocate here.
    MDrv_DSCMB2_FltFree(0, cm_handle->dsc_id); //the 0 magic value, maybe need to re-fine....

    memset(cm_handle, 0x0, sizeof(NSK2_CMChannel_Info));

    MApi_NSK2_FreeMemory(cm_handle, TRUE);
    NSK_TRACE(("Exit\n"));

    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_CmChannel_SetWriteESA(NDS_UBYTE ESA_Algo, MS_U32 PidSlotID)
{
    NDS_UBYTE ESASelect = 0, ESASubSelect = 0;
    NSK_TRACE(("Enter, ESA_Algo = %d, FltID = 0x%08x\n", ESA_Algo, PidSlotID));

    if(ESA_Algo == NSK2HDI_SPROFILE_DVB_CSA2)
    {
        ESASelect = ESA_SELECT_DVBCSA2;
        ESASubSelect = ESASUB_SELECT_NONE;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_DVB_CSA_CONFORMANCE)
    {
        ESASelect = ESA_SELECT_DVBCSA2_CONF;
        ESASubSelect = ESASUB_SELECT_NONE;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_DVB_CSA3)
    {
        ESASelect = ESA_SELECT_DVBCSA3;
        ESASubSelect = ESASUB_SELECT_NONE;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_CPCM_LSA_MDI_CBC)
    {
        ESASelect = ESA_SELECT_AES;
        ESASubSelect = ESASUB_SELECT_MDI_CBC;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_CPCM_LSA_MDI_RCBC)
    {
    #if 1
        NSK_ERROR(("NSK2HDI_SPROFILE_CPCM_LSA_MDI_RCBC NOT SUPPORT\n"));
    #else
        ESASelect = ESA_SELECT_AES;
        ESASubSelect = DSCMB_SUBALGO_MDI_RCBC;

    #endif
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_CPCM_LSA_MDD_CBC)
    {
    #if 1
        NSK_ERROR(("NSK2HDI_SPROFILE_CPCM_LSA_MDD_CBC NOT SUPPORT\n"));
    #else
        ESASelect = DSCMB_ALGOTYPE_AES;
        ESASubSelect = DSCMB_SUBALGO_MDD_CBC;

    #endif
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_CPCM_LSA_MDD_RCBC)
    {
    #if 1
        NSK_ERROR(("NSK2HDI_SPROFILE_CPCM_LSA_MDD_RCBC NOT SUPPORT\n"));
    #else
        ESASelect = DSCMB_ALGOTYPE_AES;
        ESASubSelect = DSCMB_SUBALGO_MDD_RCBC;
    #endif
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_SYNAMEDIA_AES)
    {
        ESASelect = ESA_SELECT_AES;
        ESASubSelect = ESASUB_SELECT_SYNAMEDIA_AES;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_AES_ECB_CLEARTAIL)
    {
        ESASelect = ESA_SELECT_AES;
        ESASubSelect = ESASUB_SELECT_AES_ECB_CLR;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_CIPLUS_AES)
    {
        ESASelect = ESA_SELECT_AES;
        ESASubSelect = ESASUB_SELECT_CIPLUS_AES;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_SCTE41_DES)
    {
        ESASelect = ESA_SELECT_DES;
        ESASubSelect = ESASUB_SELECT_SCTE41;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_SCTE52_DES)
    {
        ESASelect = ESA_SELECT_DES;
        ESASubSelect = ESASUB_SELECT_SCTE52;
    }
    else if (ESA_Algo == NSK2HDI_SPROFILE_TDES_ECB_CLEARTAIL)
    {
        ESASelect = ESA_SELECT_TDES;
        ESASubSelect = ESASUB_SELECT_NONE;
    }
    else
    {
        NSK_TRACE(("Not implement\n"));
    }

    MDrv_NSK2_WriteESA(ESASelect, ESASubSelect, PidSlotID);

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_CmChannel_ParseDesc(NDS_ULONG desc_size,const NDS_UBYTE *pdesc, CmChDesc_Parser_t *pDescParser)
{
    NDS_ULONG size = desc_size;
    NDS_UBYTE  *pParser_buf = (NDS_UBYTE*)pdesc;
    NDS_UBYTE TagSwitch, TagLength;

    NSK_TRACE(("desc_size = %x, pdesc = %x\n",desc_size, (NDS_ULONG)pdesc));
    while(size>0)
    {
        NSK_TRACE(("desc buf = %x\n",*pParser_buf++));
        size --;
    }

    size = desc_size;
    pParser_buf = (NDS_UBYTE*)pdesc;

    while(size>=3)
    {
        TagSwitch = pParser_buf[0];
        TagLength = pParser_buf[1];

        NSK_TRACE(("TagSwitch = %x, TagLength = %x\n",TagSwitch,TagLength));

        switch(TagSwitch)
        {
            case NSK2HDI_CMCHANNEL_IV1_DESC_TAG:
                NSK_TRACE(("CMCHANNEL_IV1_DESC_TAG length = %x\n",TagLength));
                memcpy(pDescParser->IV1, &pParser_buf[2], TagLength);
                pDescParser->IV1_Enable = TRUE;
                break;

            case NSK2HDI_CMCHANNEL_IV2_DESC_TAG:
                NSK_TRACE(("CMCHANNEL_IV2_DESC_TAG length = %x\n",TagLength));
                memcpy(pDescParser->IV2, &pParser_buf[2], TagLength);
                pDescParser->IV2_Enable = TRUE;
                break;

            case NSK2HDI_CMCHANNEL_ALGORITHM_DESC_TAG:
                pDescParser->Algorithm = pParser_buf[2];
                NSK_TRACE(("ALGORITHM_DESC_TAG, Algorithm = %x\n",pDescParser->Algorithm));
                break;

            case NSK2HDI_CMCHANNEL_IV_SIGNALING_DESC_TAG:
                NSK_TRACE(("IV_SIGNALING_DESC_TAG iv_signaling = %x\n", pParser_buf[3]));
                pDescParser->IV_Mode = pParser_buf[3];
                if(pParser_buf[3] == NSK2HDI_CMCHANNEL_IV_AT_CONFIG_MODE)
                {
                    NSK_TRACE(("NSK2HDI_CMCHANNEL_IV_AT_CONFIG_MODE\n"));
                }
                else if(pParser_buf[3] == NSK2HDI_CMCHANNEL_IV_NOT_SET_MODE)
                {
                    NSK_TRACE(("NSK2HDI_CMCHANNEL_IV_NOT_SET_MODE\n"));
                }
                else if(pParser_buf[3] == NSK2HDI_CMCHANNEL_IV_AT_WRITEKEY_MODE)
                {
                    NSK_TRACE(("NSK2HDI_CMCHANNEL_IV_AT_WRITEKEY_MODE\n"));
                }
                break;

            default:
                NSK_ERROR(("unknown desc tag\n"));
                break;
        }

        size -= (TagLength+2);
        pParser_buf += (TagLength+2);
    }

    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_CmChannelDevice_Configure ( NSK2HDI_HANDLE  CmChannele_handle,
                                                  NDS_ULONG       lda_desc_size,
                                                  const NDS_UBYTE *lda_desc,
                                                  NDS_ULONG       esa_desc_size,
                                                  const NDS_UBYTE *esa_desc,
                                                  NDS_ULONG       lsa_desc_size,
                                                  const NDS_UBYTE *lsa_desc)
{
    NSK_TRACE(("Enter\n"));
    CmChannel_IV1_Desc_t ESA_IV1_Payload;
    CmChannel_IV2_Desc_t ESA_IV2_Payload;
    CmChDesc_Parser_t *pCmDescParser;
    NSK2_CMChannel_Info *cm_handle;

    if(NULL == CmChannele_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    cm_handle = (NSK2_CMChannel_Info *)CmChannele_handle;
    pCmDescParser = (CmChDesc_Parser_t *) &cm_handle->CmChDesc[0];

    memset(&ESA_IV1_Payload, 0x0, sizeof(CmChannel_IV1_Desc_t));
    memset(&ESA_IV2_Payload, 0x0, sizeof(CmChannel_IV2_Desc_t));


    NSK_TRACE(("lda_desc_size = %x, esa_desc_size = %x, lsa_desc_size = %x\n",lda_desc_size,esa_desc_size,lsa_desc_size));

    if( (lda_desc!=NULL) && (lda_desc_size!=0) )
    {
        NSKHDI_CmChannel_ParseDesc(lda_desc_size,lda_desc,(CmChDesc_Parser_t *)&pCmDescParser[CMCHANNEL_LDA_DESC]);
    }
    else
    {
        //NSK_TRACE(("lda_desc_size = 0\n"));
    }

    NSK_TRACE(("esa_desc_size = %x\n",esa_desc_size));
    if( (esa_desc!=NULL) && (esa_desc_size!=0) )
    {
        NSKHDI_CmChannel_ParseDesc(esa_desc_size,esa_desc,(CmChDesc_Parser_t *)&pCmDescParser[CMCHANNEL_ESA_DESC]);
        //NSKHDI_CmChannel_SetWriteESA(pCmDescParser[CMCHANNEL_ESA_DESC].Algorithm, PidSlotID);
    }
    else
    {
        //NSK_TRACE(("esa_desc_size = 0\n"));
    }

    NSK_TRACE(("lsa_desc_size = %x\n",lsa_desc_size));
    if( (lsa_desc!=NULL) && (lsa_desc_size!=0) )
    {
        NSKHDI_CmChannel_ParseDesc(lsa_desc_size,lsa_desc,(CmChDesc_Parser_t *)&pCmDescParser[CMCHANNEL_LSA_DESC]);
    }
    else
    {
        //NSK_TRACE(("lsa_desc_size = 0\n"));
    }

    memcpy(&cm_handle->CmChCookie.CmChDesc[0], pCmDescParser, sizeof(CmChDesc_Parser_t)*3 );

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelDevice_InvalidateKeys (NSK2HDI_HANDLE CmChannelDevice_handle)
{
    NSK_TRACE(("Enter CmChannelDevice_handle = %p\n",CmChannelDevice_handle));
    MS_U32 u32DscmbId;

    NSK2_CMChannel_Info *cm_handle;

    if(NULL == CmChannelDevice_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    cm_handle = (NSK2_CMChannel_Info *)CmChannelDevice_handle;


    u32DscmbId = cm_handle->dsc_id;

    //if(cm_handle->CmChCookie.bConnect == TRUE)
    {
        //MDrv_DSCMB2_FltDisconnectFltId(0,u32DscmbId,u32FltId);
        MDrv_DSCMB2_FltKeyReset(0,u32DscmbId,E_DSCMB_KEY_CLEAR);
        MDrv_DSCMB2_FltKeyReset(0,u32DscmbId,E_DSCMB_KEY_EVEN);
        MDrv_DSCMB2_FltKeyReset(0,u32DscmbId,E_DSCMB_KEY_ODD);
    }

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelDevice_WriteTransportKey ( NSK2HDI_HANDLE  CmChannelDevice_handle,
                                                          NDS_ULONG       scb_in_out_size,
                                                          NDS_UBYTE       *scb_in,
                                                          NDS_UBYTE       *scb_out,
                                                          NDS_ULONG       lda_desc_size,
                                                          const NDS_UBYTE *lda_desc,
                                                          NDS_ULONG       esa_desc_size,
                                                          const NDS_UBYTE *esa_desc,
                                                          NDS_ULONG       lsa_desc_size,
                                                          const NDS_UBYTE *lsa_desc)
{
    NSK2_CMChannel_Info *cm_handle;
    NSK_TRACE(("Enter CmChannelDevice_handle = %p\n",CmChannelDevice_handle));

    if(NULL == CmChannelDevice_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    cm_handle = (NSK2_CMChannel_Info *)CmChannelDevice_handle;

    NDS_ULONG  i = 0;
    ESA_Desc_St_t tEsaDesc;
    MS_U8 test_key[16];
    NDS_UBYTE SCB = 0, SCB_Out = 0;
    MS_U32 DscmbID = 0;
    NDS_UBYTE CurSCB_In,CurSCB_Out;

    CmChDesc_Parser_t *pCmDescParser;

    NSK_TRACE(("Time of Two Keys = %x\n", (MsOS_GetSystemTime() - _gu32WriteKeyTime) ));
    //printf("write key = %d\n",MsOS_GetSystemTime());

    pCmDescParser = (CmChDesc_Parser_t *) &cm_handle->CmChDesc[0];

    memset(test_key,0x0,16);
    memset(&tEsaDesc,0x0,sizeof(ESA_Desc_St_t));

    DscmbID = cm_handle->dsc_id;
    NSK_TRACE(("DscmbID = %d\n",DscmbID));

    //this routine is used to cause the NSK chip to copy the keys and control switches
    //from the NSK KTE register to the NSK chip key table.
    NSK_TRACE(("scb_in_out_size = %x, lda_desc_size = %x\n",scb_in_out_size, lda_desc_size));
    NSK_TRACE(("esa_desc_size = %x, lsa_desc_size = %x\n",esa_desc_size, lsa_desc_size));

    //NSK2DBG_KTEValid(1,DscmbID);
    NSK_TRACE(("lda_desc_size = %x, esa_desc_size = %x, lsa_desc_size = %x\n",lda_desc_size,esa_desc_size,lsa_desc_size));

    if( (lda_desc!=NULL) && (lda_desc_size!=0) )
    {
        NSKHDI_CmChannel_ParseDesc(lda_desc_size,lda_desc,(CmChDesc_Parser_t *)&pCmDescParser[CMCHANNEL_LDA_DESC]);
    }
    else
    {
    }

    if( (esa_desc!=NULL) && (esa_desc_size!=0) )
    {
        NSKHDI_CmChannel_ParseDesc(esa_desc_size,esa_desc,(CmChDesc_Parser_t *)&pCmDescParser[CMCHANNEL_ESA_DESC]);
    }
    else
    {
        //NSK_TRACE(("esa_desc_size = 0\n"));
    }

    if( (lsa_desc!=NULL) && (lsa_desc_size!=0) )
    {
        NSKHDI_CmChannel_ParseDesc(lsa_desc_size,lsa_desc,(CmChDesc_Parser_t *)&pCmDescParser[CMCHANNEL_LSA_DESC]);
    }
    else
    {

    }


    if(cm_handle->CmChCookie.bWriteESA == FALSE)
    {
        MDrv_NSK21_CfgCmChannel(DscmbID, pCmDescParser[CMCHANNEL_LDA_DESC].Algorithm,
                                pCmDescParser[CMCHANNEL_ESA_DESC].Algorithm, pCmDescParser[CMCHANNEL_LSA_DESC].Algorithm,  0);
    }


    while(i < scb_in_out_size)
    {
        NSK_TRACE(("(%d, %x, %x) \n",i,*(scb_in+i),*(scb_out+i)));
        CurSCB_In = *(scb_in+i);
        CurSCB_Out = *(scb_out+i);

        //scb value is match with the HW test patterns
        if(CurSCB_In == NSK2HDI_CMCHANNEL_ODD_PARITY)
        {
            SCB = 0x3;
        }
        else if(CurSCB_In == NSK2HDI_CMCHANNEL_EVEN_PARITY)
        {
            SCB = 0x2;
        }
        else if(CurSCB_In == NSK2HDI_CMCHANNEL_CLEAR_PARITY)
        {
            SCB = 0x0;
        }

        if(CurSCB_Out == NSK2HDI_CMCHANNEL_ODD_PARITY)
        {
            SCB_Out = 0x3;
        }
        else if(CurSCB_Out == NSK2HDI_CMCHANNEL_EVEN_PARITY)
        {
            SCB_Out = 0x2;
        }
        else if(CurSCB_Out == NSK2HDI_CMCHANNEL_CLEAR_PARITY)
        {
            SCB_Out = 0x0;
        }

        NSK_TRACE(("SCB_In = %x, SCB_Out = %x\n",SCB,SCB_Out));


        MDrv_NSK21_WriteTransportKey(SCB, SCB_Out,
                                     pCmDescParser[CMCHANNEL_LDA_DESC].IV1, pCmDescParser[CMCHANNEL_LDA_DESC].IV2,
                                     pCmDescParser[CMCHANNEL_ESA_DESC].IV1, pCmDescParser[CMCHANNEL_ESA_DESC].IV2,
                                     pCmDescParser[CMCHANNEL_LSA_DESC].IV1, pCmDescParser[CMCHANNEL_LSA_DESC].IV2, DscmbID);

        MS_U8 WriteESAIV[16], WriteLSADIV[16], WriteLSASIV[16];
        memset(WriteESAIV, 0x0, 16);
        memset(WriteLSADIV,  0x0, 16);
        memset(WriteLSASIV,  0x0, 16);

        CmChDesc_Parser_t *pESAParser = &pCmDescParser[CMCHANNEL_ESA_DESC];
        CmChDesc_Parser_t *pLDAParser = &pCmDescParser[CMCHANNEL_LDA_DESC];
        CmChDesc_Parser_t *pLSAParser = &pCmDescParser[CMCHANNEL_LSA_DESC];

        DSCMB_Key_Type eKeyType = E_DSCMB_KEY_CLEAR;
        if(SCB == 0){
    		eKeyType = E_DSCMB_KEY_CLEAR;
    	}
    	else if(SCB == 2){
    		eKeyType = E_DSCMB_KEY_EVEN;
    	}
    	else if(SCB == 3){
    		eKeyType = E_DSCMB_KEY_ODD;
    	}

    #if 1
        //if(pLDAParser->IV1_Enable)
        {
            memcpy(WriteLSADIV,pLDAParser->IV1,16);

            if(pLDAParser->IV2_Enable)
                memcpy(&WriteLSADIV[8], pLDAParser->IV2, 8);

            MDrv_DSCMB2_EngSetIV_Ex(0, DscmbID, E_DSCMB_ENG_LSAD, eKeyType, WriteLSADIV);
        }

        //if(pESAParser->IV1_Enable)
        {
            memcpy(WriteESAIV,pESAParser->IV1,16);

            if(pESAParser->IV2_Enable)
                memcpy(&WriteESAIV[8], pESAParser->IV2, 8);

            MDrv_DSCMB2_EngSetIV_Ex(0, DscmbID, E_DSCMB_ENG_ESA, eKeyType, (MS_U8*)WriteESAIV);
        }

        //if(pLSAParser->IV1_Enable)
        {
            memcpy(WriteLSASIV,pLSAParser->IV1,16);

            if(pLSAParser->IV2_Enable)
                memcpy(&WriteLSASIV[8], pLSAParser->IV2, 8);

            MDrv_DSCMB2_EngSetIV_Ex(0, DscmbID, E_DSCMB_ENG_LSAS, eKeyType, WriteLSASIV);
        }
    #endif

        i++;
    }

    _gu32WriteKeyTime = MsOS_GetSystemTime();

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CmChannelDevice_GetCookie(NSK2HDI_HANDLE CmChannelDevice_handle,
                                                void **cookie)
{
    NSK2_CMChannel_Info *cm_handle;
    NSK_TRACE(("Enter\n"));

    if(NULL == CmChannelDevice_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    cm_handle = (NSK2_CMChannel_Info *)CmChannelDevice_handle;

    *cookie = &cm_handle->CmChCookie;

    NSK_TRACE(("test_cookie = %p\n", *cookie));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

/*****************************************************************************\
  \ Content Module - CA PID Proxy devices
\*****************************************************************************/


NSK2HDI_STATUS NSKHDI_CaPIDProxy_Open (NDS_ULONG      xconn,
                                       NSK2HDI_HANDLE *CaPIDProxy_handle)
{
    NSK_TRACE(("Enter, xconn = %x\n",xconn));

    CaPID_Info *pCaPidProxy;
    pCaPidProxy = MApi_NSK2_AllocateMemory(sizeof(CaPID_Info), TRUE);
    if(NULL == pCaPidProxy)
    {
        NSK_ERROR(("pCaPidProxy allocate buffer error\n"));
        return NSK2HDI_STATUS_FAILED;
    }

    memset(pCaPidProxy, 0x0, sizeof(CaPID_Info));

    pCaPidProxy->xconn = xconn;

    *CaPIDProxy_handle = (NSK2HDI_HANDLE)pCaPidProxy;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CaPIDProxy_Close (NSK2HDI_HANDLE CaPIDProxy_handle)
{
    NSK_TRACE(("Enter\n"));

    if(NULL == CaPIDProxy_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    MApi_NSK2_FreeMemory(CaPIDProxy_handle, TRUE);

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CaPIDProxy_SetPrimaryStreamPID (NSK2HDI_HANDLE CaPIDProxy_handle,
                                                      NDS_USHORT     es_pid)
{
    NSK_TRACE(("Enter, es_pid = %x\n",es_pid));
    if(NULL == CaPIDProxy_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }
    CaPID_Info *pCaPidProxy = (CaPID_Info *)CaPIDProxy_handle;

    pCaPidProxy->es_pid = es_pid;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

//in this function, we connection sepecific PID to dscmb....
NSK2HDI_STATUS NSKHDI_CaPIDProxy_SetPrimaryStreamCmChannel (NSK2HDI_HANDLE CaPIDProxy_handle,
                                                            void           *CmChannel_cookie)
{
    NSK_TRACE(("Enter\n"));
    if(NULL == CaPIDProxy_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }
    CaPID_Info *pCaPidProxy = (CaPID_Info *)CaPIDProxy_handle;

    MS_U16 es_pid;
    CmChannelDevice_Cookie_t *pCmChDeCookie;
    MS_U32 u32DscmbId;
    NDS_UBYTE ESA_Algo;
    MS_U32 xconn,i;
    MS_U32 FilterID = 0;
    MS_BOOL AlreadyAdd = FALSE;

    pCmChDeCookie = (CmChannelDevice_Cookie_t *)CmChannel_cookie;
    es_pid = pCaPidProxy->es_pid;
    xconn = pCaPidProxy->xconn;

    pCaPidProxy->pCmCookie = CmChannel_cookie;
    ESA_Algo = pCmChDeCookie->CmChDesc[CMCHANNEL_ESA_DESC].Algorithm;
    u32DscmbId = pCmChDeCookie->u32DscmbId;

    pCaPidProxy->dscmb_id = u32DscmbId;
    pCmChDeCookie->u16Pid = es_pid;
    pCaPidProxy->PrimePID = es_pid;

    for(i=0;i<NSK2_CMCHANNEL_NUM;i++)
    {
        if(es_pid == _u32KeepPidByFltId[xconn][i])
        {
            FilterID = i;
            AlreadyAdd = TRUE;
            break;
        }
    }

    //_KeepCaPidProxy
    for(i=0;i<NSK2_CMCHANNEL_NUM;i++)
    {
        if(FALSE == _KeepCaPidProxy[xconn][i].IsUsed)
        {
            _KeepCaPidProxy[xconn][i].IsUsed  = TRUE;
            _KeepCaPidProxy[xconn][i].EsPid   = es_pid;
            _KeepCaPidProxy[xconn][i].DscmbId = u32DscmbId;
            pCaPidProxy->ProxyGlobalIndex = i;
            NSK_TRACE(("KeepCaPidProxy at %d\n",i));
            break;
        }
    }

    if(FALSE == AlreadyAdd) //not add...
    {
        NSK_TRACE(("Not Add Pid to TSP Filter\n"));
        if(FALSE == pCmChDeCookie->bWriteESA)
        {
            MDrv_NSK21_CfgCmChannel(u32DscmbId, pCmChDeCookie->CmChDesc[CMCHANNEL_LDA_DESC].Algorithm,
                                    pCmChDeCookie->CmChDesc[CMCHANNEL_ESA_DESC].Algorithm, pCmChDeCookie->CmChDesc[CMCHANNEL_LSA_DESC].Algorithm, 0);
            pCmChDeCookie->bWriteESA = TRUE;
        }


        return NSK2HDI_STATUS_OK;
    }
    else
    {
        NSK_TRACE(("Already Add TSP filter\n"));

        //if(  (pCmChDeCookie->bConnect == FALSE) )
        {
            if(MDrv_DSCMB2_FltConnectFltId(0, u32DscmbId, FilterID) == FALSE)
            {
                NSK_ERROR(("MDrv_DSCMB_FltConnectPid() failed\n"));
                return NSK2HDI_STATUS_FAILED;
            }

            if(FALSE == pCmChDeCookie->bWriteESA)
            {
                MDrv_NSK21_CfgCmChannel(u32DscmbId, pCmChDeCookie->CmChDesc[CMCHANNEL_LDA_DESC].Algorithm,
                                        pCmChDeCookie->CmChDesc[CMCHANNEL_ESA_DESC].Algorithm, pCmChDeCookie->CmChDesc[CMCHANNEL_LSA_DESC].Algorithm, 0);
                pCmChDeCookie->bWriteESA = TRUE;
            }

            //MDrv_DSCMB2_ReadPidSlotMap(FilterID);
        }
    }

    pCmChDeCookie->bConnect = TRUE;

    //NSK_TRACE(("es_pid = %x, u32DscmbId = %d, FilterID = %d(0x%08x)\n",es_pid,u32DscmbId,FilterID,FilterID));
    NSK_TRACE(("Exit\n"));

    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_CaPIDProxy_RemovePrimaryStreamCmChannel (NSK2HDI_HANDLE CaPIDProxy_handle)
{
    NSK_TRACE(("Enter\n"));
    if(NULL == CaPIDProxy_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }
    CaPID_Info *pCaPidProxy = (CaPID_Info *)CaPIDProxy_handle;
    CmChannelDevice_Cookie_t *pCmChDeCookie;

    MS_U32 u32DscmbId, u32FltID, i;
    MS_U32 xconn = 0;
    MS_U16 es_pid = 0;
    MS_U32 index;

    xconn = pCaPidProxy->xconn;
    es_pid = pCaPidProxy->es_pid;
    index = pCaPidProxy->ProxyGlobalIndex;

    pCmChDeCookie = (CmChannelDevice_Cookie_t*) pCaPidProxy->pCmCookie;

    for( i=0; i<NSK2_CMCHANNEL_NUM ; i++)
    {
        if( _u32KeepPidByFltId[xconn][i] == es_pid)
            break;
    }

    u32FltID = i;
    u32DscmbId = _KeepCaPidProxy[xconn][index].DscmbId;


    NSK_TRACE(("MDrv_DSCMB2_FltDisconnectFltId, u32DscmbId = %d, u32FltId = %d\n",u32DscmbId,u32FltID));

    MDrv_DSCMB2_FltDisconnectFltId(0,u32DscmbId,u32FltID);

    pCmChDeCookie->bConnect = FALSE;

    _KeepCaPidProxy[xconn][index].IsUsed  = FALSE;
    _KeepCaPidProxy[xconn][index].EsPid   = DefaultPID;
    _KeepCaPidProxy[xconn][index].DscmbId = DefaultDSCMBID;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_CaPIDProxy_SetSecondaryStream (NSK2HDI_HANDLE CaPIDProxy_handle,
                                                     void           *CmChannel_cookie,
                                                     NDS_USHORT     es_pid,
                                                     NDS_ULONG      merge_mode)
{
    NSK_TRACE(("Enter\n"));
    if(NULL == CaPIDProxy_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }
    CaPID_Info *pCaPidProxy = (CaPID_Info *)CaPIDProxy_handle;


    CmChannelDevice_Cookie_t *pCmChDeCookie;
    MS_U32 u32DscmbId;
    MS_U32 SecondaryFilterID = 0;
    MS_U32 PrimaryPID = 0, PrimeFilterID = 0;
    MS_U32 xconn, i;
    MS_U8 DmxIdSect;
    DMX_Flt_info dmx_fltinfo;
    DMX_FILTER_TYPE FilterType;

    //*CmChannel_cookie, this is used to set parameters, the parameters come from cmchannel command.
    pCmChDeCookie = (CmChannelDevice_Cookie_t *)CmChannel_cookie;
    PrimaryPID = pCaPidProxy->es_pid;
    xconn = pCaPidProxy->xconn;

    NSK_TRACE(("primary pid = 0x%x, Secondary pid = 0x%x\n", PrimaryPID, es_pid));

    u32DscmbId = pCmChDeCookie->u32DscmbId;

    for(i=0;i<NSK2_CMCHANNEL_NUM;i++)
    {
        if(PrimaryPID == _u32KeepPidByFltId[xconn][i])
        {
            PrimeFilterID = i;
            break;
        }
    }

    pCaPidProxy->PrimeFilterID = PrimaryPID;

    MApi_DMX_Info(PrimeFilterID, &dmx_fltinfo, &FilterType, FALSE);
    FilterType |= (MApi_NSK2_EcmGetFilterType(xconn) & 0xF0000000);

    NSK_TRACE(("PrimeFilterID = %d, Primary Filtertype is %d\n",PrimeFilterID, FilterType));

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        NSK_ERROR(("[%s] Allocate filter fail \n",__FUNCTION__));
        return NSK2HDI_STATUS_FAILED;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(DmxIdSect, &es_pid, TRUE))
    {
        NSK_ERROR(("[%s] MApi_DMX_Pid fail \n",__FUNCTION__));
        return NSK2HDI_STATUS_FAILED;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(DmxIdSect))
    {
        NSK_ERROR(("[%s] MApi_DMX_Start \n",__FUNCTION__));
        return NSK2HDI_STATUS_FAILED;
    }
    SecondaryFilterID = DmxIdSect; //one to one mapping.....

    NSK_TRACE(("es_pid = %x, u32DscmbId = %d, FilterID = %d\n",es_pid,u32DscmbId,SecondaryFilterID));

    pCaPidProxy->SecondaryFilterID = SecondaryFilterID;
    pCaPidProxy->SecondaryPID= es_pid;
    pCaPidProxy->secondary_enable = TRUE;

    if(MDrv_DSCMB2_FltConnectFltId(0,u32DscmbId, SecondaryFilterID) == FALSE)
    {
        NSK_ERROR(("MDrv_DSCMB_FltConnectPid() failed\n"));
        return NSK2HDI_STATUS_FAILED;
    }

    NSK_TRACE(("PrimeFilterID = 0x%x, SecondaryFilterID = 0x%x\n",PrimeFilterID, SecondaryFilterID));

    MDrv_DSCMB2_SetPidPair(PrimeFilterID, (MS_U32)pCaPidProxy->SecondaryFilterID);

#if 0
    _u32KeepPidByFltId[xconn][FilterID] = es_pid;
    _u32DscmbID[FilterID] = u32DscmbId;

    pCmChDeCookie->u16Pid = es_pid;
    pCmChDeCookie->PidSlotID = FilterID;
#endif
    MDrv_NSK21_CfgCmChannel(u32DscmbId, pCmChDeCookie->CmChDesc[CMCHANNEL_LDA_DESC].Algorithm,
                                        pCmChDeCookie->CmChDesc[CMCHANNEL_ESA_DESC].Algorithm, pCmChDeCookie->CmChDesc[CMCHANNEL_LSA_DESC].Algorithm, 0);


    NSK_TRACE(("Exit\n"));

    return NSK2HDI_STATUS_OK;
}


//need to refine....
NSK2HDI_STATUS NSKHDI_CaPIDProxy_RemoveSecondaryStream (NSK2HDI_HANDLE CaPIDProxy_handle)
{

    NSK_TRACE(("Enter\n"));
    if(NULL == CaPIDProxy_handle)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }
    CaPID_Info *pCaPidProxy = (CaPID_Info *)CaPIDProxy_handle;

    MDrv_DSCMB2_ClearPidPair(pCaPidProxy->PrimeFilterID, pCaPidProxy->SecondaryFilterID);

    MApi_NSK2_DTV_ClosePidNo(pCaPidProxy->SecondaryFilterID);
    NSK_TRACE(("MApi_NSK2_DTV_ClosePidNo, FltId = %d\n",pCaPidProxy->SecondaryFilterID));


    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


/*****************************************************************************\
  \ CA Master and SoC devices
\*****************************************************************************/
void NSK2HDI_ParamInit(void)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 i=0, j=0;
    debug_level = PRINT_TRACE;
    MS_U32 MaxConn;
    MS_U32 size;

    for( i=0; i<NSK2_CMCHANNEL_NUM; i++)
    {
        pCmChannelcookie[i]   = MApi_NSK2_AllocateMemory(CmChannelCookieSize, FALSE);
        pM2MChannelcookie[i] = MApi_NSK2_AllocateMemory(CmChannelCookieSize, FALSE);
    }

    for( j=0; j<MaxXConnection ; j++)
    {
        for( i=0; i<NSK2_CMCHANNEL_NUM; i++)
        {
            _u32KeepPidByFltId[j][i]     = DefaultPID;
            _KeepCaPidProxy[j][i].EsPid  = DefaultPID;
            _KeepCaPidProxy[j][i].DscmbId  = DefaultDSCMBID;
            _KeepCaPidProxy[j][i].IsUsed = 0;
        }

    }

    memset(HDI_XconnInfo,0x0,sizeof(AddXconnInfo_t)*2);

    MaxConn = MDrv_NSK2_GetMaxXConn();
    size = sizeof(AddXconnInfo_t)*MaxConn;
    NSK_TRACE(("MaxConn = 0x%08x, size = 0x%08x\n",MaxConn, size));

    NSK_TRACE(("Exit\n"));
}
