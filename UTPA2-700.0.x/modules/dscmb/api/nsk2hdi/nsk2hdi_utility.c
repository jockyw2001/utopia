

#include <stdlib.h>
#include <string.h>
#include "MsCommon.h"

#include "nsk_dbg.h"
#include "nsk2hdi_header.h"
#include "apiDMX.h"

/*****************************************************************************\
  \Definitions
\*****************************************************************************/


/*****************************************************************************\
  \ global variables
\*****************************************************************************/
static P_NSK2_CbAllocateMemory _AllocateMemory = NULL;
static P_NSK2_CbFreeMemory	   _FreeMemory	= NULL;
static P_NSK2_CbEcmGetType	   _EcmGetType = NULL;
static MS_BOOL				   _bRunHarmonizer = FALSE;

/*****************************************************************************\
  \ NSK2HDI general functions
\*****************************************************************************/

MS_U32 MApi_NSK2HDI_Initial(HDIPara_Init_t *pInit)
{
    _bRunHarmonizer = pInit->bHarmonizer;
    _AllocateMemory = pInit->cbAllocMemory;
    _EcmGetType     = pInit->cbEcmGetType;
    _FreeMemory     = pInit->cbFreeMemory;
    return TRUE;
}


MS_BOOL MApi_NSK2_IsHarmonizer(void)
{
    return _bRunHarmonizer;
}

void *MApi_NSK2_AllocateMemory(MS_U32 u32Size, MS_BOOL bCached)
{
    if( _AllocateMemory != NULL)
    {
        void *pBuf = NULL;
        pBuf = _AllocateMemory( u32Size, bCached);

        if(pBuf == NULL)
        {
            printf("memory allocate fail\n");
        }
        return pBuf;
    }
    else
    {
        return NULL;
    }

}

MS_BOOL MApi_NSK2_FreeMemory(void *pAddress, MS_BOOL bCached)
{
    if( _FreeMemory != NULL)
    {
        return _FreeMemory(pAddress,bCached);
    }
    else
    {
        return FALSE;
    }
}

MS_U32 MApi_NSK2_EcmGetFilterType(MS_U32 x_conn)
{
    return _EcmGetType(x_conn);
}



MS_BOOL MApi_NSK2_DTV_ClosePidNo(MS_U8 PidNo)
{
    if( MApi_DMX_Stop(PidNo) != DMX_FILTER_STATUS_OK)
    {
        NSK_ERROR(("DMX Stop error\n"));
    }

    if( MApi_DMX_Close(PidNo) != DMX_FILTER_STATUS_OK)
    {
        NSK_ERROR(("DMX Close error\n"));
    }
    return TRUE;
}

