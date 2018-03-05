

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
#include "drvCA.h"
#include "drvNSK2.h"


/*****************************************************************************\
  \Definitions
\*****************************************************************************/


/*****************************************************************************\
  \ global variables
\*****************************************************************************/

/*****************************************************************************\
  \ NSK2HDI otp related functions
\*****************************************************************************/
/*****************************************************************************\
  \ OTP device
\*****************************************************************************/

#define USEDrvFunction

#ifndef USEDrvFunction
#define OTP_REG(addr)            (*((volatile MS_U32*)(0xbf320000 + addr )))
#define OTP_REG8(addr)           (*((volatile MS_U8*)(0xbf320000 + addr )))
#endif

extern MS_U32 NSKHDI_GetHandle(NSK2HDI_HANDLE Handle);


#define MaxOTPHandleNum    10


typedef struct _OTP_Info_
{
    MS_BOOL         alloc;
    NDS_ULONG       Otp_ID;
    MS_U32          handle_id;
} OTP_Info;


OTP_Info OTPInfo[MaxOTPHandleNum];

NSK2HDI_STATUS NSKHDI_OtpDevice_Open (NSK2HDI_DEVICE_ID   Otp_ID,
                                      NSK2HDI_HANDLE      *Otp_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 i;

    if(*Otp_handle != NULL)
    {
        NSK_TRACE(("Otp_handle is not NULL\n"));
    }

    for(i = 0; i < MaxOTPHandleNum; i++)
    {
        if (OTPInfo[i].alloc == FALSE)
        {
            break;
        }
    }
    if (i == MaxOTPHandleNum)
    {
        return (NSK2HDI_STATUS_FAILED);
    }


    //should allocate dscmb here....
    OTPInfo[i].alloc = TRUE;
    OTPInfo[i].handle_id = i;
    OTPInfo[i].Otp_ID = Otp_ID;


    *Otp_handle = (NSK2HDI_HANDLE)&OTPInfo[i].handle_id;

    NSK_TRACE(("Otp_ID = 0x%x, Otp_handle = 0x%08x\n",Otp_ID,OTPInfo[i].handle_id));
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_OtpDevice_Close (NSK2HDI_HANDLE Otp_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(Otp_handle);


    OTPInfo[u32Handle].alloc = FALSE;
    OTPInfo[u32Handle].handle_id = 0;


    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_OtpDevice_GetProperties( NSK2HDI_HANDLE  Otp_handle,
                                               NDS_ULONG       request_id,
                                               NDS_ULONG       *desc_size,
                                               NDS_UBYTE       *desc)
{
    NSK_TRACE(("Enter\n"));


    //in desc_size
    NSK_TRACE(("request_id = 0x%x, desc_size = 0x%x\n",request_id,*desc_size));

    if(request_id == NSK2HDI_OTP_READ_ALL_DESCRIPTORS_REQUEST)
    {
        MDrv_NSK2_GetOTPProperties((MS_U32*)desc_size, (MS_U8*)desc);
    }
    else if(request_id == NSK2HDI_OTP_READ_FULLCHIPCONFIGURATION)
    {
        MDrv_NSK2_GetFullChipConfig((MS_U32*)desc_size, (MS_U8*)desc);
    }
    else if(request_id == NSK2HDI_OTP_MAX_NVCOUNTER_UPDATES)
    {
        MDrv_NSK2_GetNVCounterConfig((MS_U32*)desc_size, (MS_U8*)desc);
    }


    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}




