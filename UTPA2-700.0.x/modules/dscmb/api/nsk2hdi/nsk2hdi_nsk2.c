
#include <stdlib.h>
#include <string.h>
#include "MsCommon.h"

#include "nsk_dbg.h"

#include "nsk_282.h"
#include "nsk_dbg.h"
#include "nsk_3014.h"
#include "hdi_121.h"
#include "drvNSK2.h"
#include "nsk2hdi_header.h"
#include "drvDSCMB.h"

/*****************************************************************************\
  \Definitions
\*****************************************************************************/
//#define NSK2DataProbe
#define MaxNSK2HandleNum    10


/*****************************************************************************\
  \ typedef struct
\*****************************************************************************/

typedef struct _NSK2_Info_
{
    MS_BOOL         alloc;
    NDS_ULONG       Nsk_ID;
    MS_U32          handle_id;
} NSK2_Info;


/*****************************************************************************\
  \ local variables
\*****************************************************************************/


/*****************************************************************************\
  \ global variables
\*****************************************************************************/
HDINSK2_NSKEVENT_CB_FN  *_gcb_routine = NULL;
void                    *_gNSKcookie = NULL;
NSK2HDI_HANDLE          _gNsk_handle;

NSK2_Info NSK2Info[MaxNSK2HandleNum];


/*****************************************************************************\
  \ NSK2HDI general functions
\*****************************************************************************/


/*****************************************************************************\
  \ NSK device
\*****************************************************************************/



NSK2HDI_STATUS NSKHDI_NskDevice_Open (NSK2HDI_DEVICE_ID   Nsk_ID,
                                      NSK2HDI_HANDLE      *Nsk_handle)
{
    NSK_TRACE(("Enter, Nsk_ID = %x\n",Nsk_ID));

    MS_U32 i;

    if(*Nsk_handle != NULL)
    {
        NSK_ERROR(("Nsk_handle is not NULL\n"));
    }

    for(i = 0; i < MaxNSK2HandleNum; i++)
    {
        if (NSK2Info[i].alloc == FALSE)
        {
            break;
        }
    }
    if (i == MaxNSK2HandleNum)
    {
        return (NSK2HDI_STATUS_FAILED);
    }


    //should allocate dscmb here....
    NSK2Info[i].alloc = TRUE;
    NSK2Info[i].handle_id = i;
    NSK2Info[i].Nsk_ID = Nsk_ID;


    *Nsk_handle = (NSK2HDI_HANDLE)&NSK2Info[i].handle_id;

    NSK_TRACE(("Nsk_ID = 0x%x, NSK_handle = 0x%x\n",Nsk_ID,NSK2Info[i].handle_id));
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_Close (NSK2HDI_HANDLE Nsk_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("u32Handle = 0x%08x\n",u32Handle));

    NSK2Info[u32Handle].alloc = FALSE;
    NSK2Info[u32Handle].handle_id = 0;


    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_Read8 (NSK2HDI_HANDLE      Nsk_handle,
                                       NDS_ULONG           register_offset,
                                       NDS_ULONG           data_size,
                                       NDS_UBYTE           *data)
{

    //call to read data from NSK2 register...
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("Nsk_handle = 0x%x, offset = 0x%x, data_size = 0x%x\n", u32Handle,register_offset,data_size));

    MDrv_NSK2_ReadData8(register_offset,data_size,data);

#ifdef NSK2DataProbe
    NDS_UBYTE *pData = (NDS_UBYTE *)data;
    NDS_ULONG  u32Data_Size = data_size;
    NSK_TRACE(("\n( "));
    while(u32Data_Size>0)
    {
        printf("%x, ",*pData++);
        u32Data_Size --;
    }
    printf(")\n");
#endif

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_NskDevice_Write8 (NSK2HDI_HANDLE     Nsk_handle,
                                        NDS_ULONG          register_offset,
                                        NDS_ULONG          data_size,
                                        const NDS_UBYTE    *data)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("Nsk_handle = 0x%x, offset = 0x%x, data_size = 0x%x\n", u32Handle,register_offset,data_size));

#ifdef NSK2DataProbe
    NDS_UBYTE *pData = (NDS_UBYTE *)data;
    NDS_ULONG  u32Data_Size = data_size;
    NSK_TRACE(("\n( "));
    while(u32Data_Size>0)
    {
        printf("%x, ",*pData++);
        u32Data_Size --;
    }
    printf(")\n");
#endif


    MDrv_NSK2_WriteData8(register_offset,data_size,(MS_U8 *)data);

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_Read32 (NSK2HDI_HANDLE     Nsk_handle,
                                        NDS_ULONG          register_offset,
                                        NDS_ULONG          data_size,
                                        NDS_ULONG          *data)
{
    NSK_TRACE(("Enter\n"));

    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("Nsk_handle = 0x%x, offset = 0x%x, data_size = 0x%x\n", u32Handle,register_offset,data_size));


    MDrv_NSK2_ReadData32(register_offset,data_size, (MS_U32 *)data);

#ifdef NSK2DataProbe
    NDS_ULONG *pData = (NDS_ULONG *)data;
    NDS_ULONG  u32Data_Size = data_size;

    NSK_TRACE(("\n( "));

    while(u32Data_Size>0)
    {
        printf("%x, ",*pData++);
        u32Data_Size --;
    }
    printf(")\n");
#endif

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_Write32 (NSK2HDI_HANDLE     Nsk_handle,
                                         NDS_ULONG          register_offset,
                                         NDS_ULONG          data_size,
                                         const NDS_ULONG    *data)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("Nsk_handle = 0x%x, offset = 0x%x, data_size = 0x%x\n", u32Handle,register_offset,data_size));

#ifdef NSK2DataProbe
    NDS_ULONG *pData = (NDS_ULONG *)data;
    NDS_ULONG  u32Data_Size = data_size;

    NSK_TRACE(("\n( "));

    while(u32Data_Size>0)
    {
        printf("%x, ",*pData++);
        u32Data_Size --;
    }
    printf(")\n");
#endif

    MDrv_NSK2_WriteData32(register_offset,data_size, (MS_U32 *)data);

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_NskDevice_ExecuteCmd (NSK2HDI_HANDLE     Nsk_handle,
                                            NDS_ULONG          command,
                                            NDS_ULONG          control,
                                            NDS_ULONG          register_offset,
                                            NDS_ULONG          data_size,
                                            const NDS_UBYTE    *data)
{

    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("Nsk_handle = 0x%x, command = 0x%x, control = 0x%x, offset = 0x%x, data_size = 0x%x\n", u32Handle,command,control,register_offset,data_size));

    NSK_TRACE(("data = %p\n",data));

#ifdef NSK2DataProbe
    NDS_UBYTE *pData = (NDS_UBYTE *)data;
    NDS_ULONG  u32Data_Size = data_size;
    NSK_TRACE(("\n( "));
    while(u32Data_Size>0)
    {
        printf("%x, ",*pData++);
        u32Data_Size --;
    }
    printf(")\n");
#endif

    //MDrv_NSK2_WriteData32(register_offset,data_size, (MS_U32*)data);
    MDrv_NSK2_ExcuteCmd( command, control, register_offset, data_size, data);
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}



void _NSK2_Notify(MS_U32  reason, MS_U32 acpuError)
{
    NSK_TRACE(("Enter, reason = 0x%x, acpuError = 0x%x\n",reason, acpuError));

    if(_gcb_routine != NULL)
    {
        _gcb_routine(_gNsk_handle,_gNSKcookie,reason,acpuError);
    }

    //NSK_TRACE(("Exit\n"));
}

NSK2HDI_STATUS NSKHDI_NskDevice_RegisterEventCB ( NSK2HDI_HANDLE          Nsk_handle,
                                                  HDINSK2_NSKEVENT_CB_FN  *cb_routine,
                                                  void                    *cookie)
{
    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("Enter, Nsk_handle = 0x%x, cb_routine = 0x%p\n",u32Handle,cb_routine));


    //it might need to  change the way to keep handle.....
    _gcb_routine = cb_routine;
    _gNsk_handle = Nsk_handle;
    _gNSKcookie = cookie;

    MDrv_NSK2_SetIntNotify(&_NSK2_Notify);
    //cb_routine = &_NSK2_Notify;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_AcknowledgeKte(NSK2HDI_HANDLE Nsk_handle)
{
    NSK_TRACE(("Enter\n"));

    MDrv_NSK2_DriveKteAck();
    //NSK2DBG_KTEValid(0,0);

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_BasicInitializationComplete( NSK2HDI_HANDLE Nsk_handle, NDS_ULONG stage)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(Nsk_handle);
    NSK_TRACE(("Nsk_handle = 0x%x, stage = 0x%x\n",u32Handle,stage));

    MDrv_NSK2_BasicInitializationComplete();
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_UpdateNVCounter(NSK2HDI_HANDLE Nsk_handle)
{
    NSK_TRACE(("Enter\n"));

    MS_U32 status;
    status = MDrv_NSK21_IncrementNvCounter();
    MDrv_NSK2_DriveKteAck();

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_WriteOTPKey(NSK2HDI_HANDLE     Nsk_handle,
                                            NDS_ULONG          desc_size,
                                            const NDS_UBYTE    *desc)
{
    NSK_TRACE(("Enter\n"));
    NSK_TRACE(("desc_size = %x, desc = %p\n",desc_size,desc));

    MS_U32 i;
    MS_U8 *pDesc = (MS_U8 *)desc;
    for(i=0;i<desc_size;i++)
    {
        printf(" (%d, %x) \n", i, pDesc[i]);
    }

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_NskDevice_SetOneShotGenInBit(NSK2HDI_HANDLE     Nsk_handle,
                                                   NDS_ULONG          bits)
{
    NSK_TRACE(("Enter\n"));

    MS_U32 MaskVal, XorVal;
    XorVal = (MS_U32)bits;
    MaskVal = (~XorVal);
    MDrv_NSK21_ModifyGenIn(MaskVal, XorVal);

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

