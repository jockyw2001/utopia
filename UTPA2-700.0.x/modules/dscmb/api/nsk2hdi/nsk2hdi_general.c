

#include <stdlib.h>
#include <string.h>
#include "MsCommon.h"

#include "nsk_dbg.h"
#include "nsk2hdi_header.h"

/*****************************************************************************\
  \Definitions
\*****************************************************************************/



/*****************************************************************************\
  \ global variables
\*****************************************************************************/


/*****************************************************************************\
  \ NSK2HDI general functions
\*****************************************************************************/
MS_U32 NSKHDI_GetHandle(NSK2HDI_HANDLE Handle)
{
    MS_U32 u32Handle = 0;

    if(Handle != NULL)
    {
        u32Handle = *(MS_U32*)Handle;
        //NSK_TRACE(("u32Handle = %lx\n", u32Handle));
    }
    else
    {
        NSK_TRACE(("Handle = NULL\n"));
    }

    return u32Handle;
}


MS_U32 NSKHDI_IVDataTrans(MS_U8 NSK2HDI_Algo, MS_U8 *pu8IV1, MS_U8 *pu8IV2, MS_U32 *pu32HWIV)
{
    MS_U32 index;

    if( (NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV2 == NSK2HDI_Algo)  ||
        (NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV2 == NSK2HDI_Algo)  ||
        (NSK2HDI_SPROFILE_SCTE52_DES == NSK2HDI_Algo) )
    {
        for(index = 0; index<4 ; index++)
        {
            pu32HWIV[index] = ( ((MS_U32)pu8IV1[index*4] << 24 ) +
                              ((MS_U32)pu8IV1[index*4+1] << 16 ) +
                              ((MS_U32)pu8IV1[index*4+2] << 8 ) +
                              ((MS_U32)pu8IV1[index*4+3] )  );
        }

        for(index = 0; index<2 ; index++)
        {
            pu32HWIV[index+2] = ( ((MS_U32)pu8IV2[index*4] << 24 ) +
                                ((MS_U32)pu8IV2[index*4+1] << 16 ) +
                                ((MS_U32)pu8IV2[index*4+2] << 8 ) +
                                ((MS_U32)pu8IV2[index*4+3] )  );
        }
    }
    else
    {
        for(index = 0; index<4 ; index++)
        {
            pu32HWIV[index] = ( ((MS_U32)pu8IV1[index*4] << 24 ) +
                              ((MS_U32)pu8IV1[index*4+1] << 16 ) +
                              ((MS_U32)pu8IV1[index*4+2] << 8 ) +
                              ((MS_U32)pu8IV1[index*4+3] )  );
        }
    }
    return TRUE;
}


