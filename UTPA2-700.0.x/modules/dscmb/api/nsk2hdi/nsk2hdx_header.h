
#ifndef NSK2HDX_HEADER_H_
#define NSK2HDX_HEADER_H_

#include "MsCommon.h"


typedef struct
{
    MS_BOOL     b8024OnChip;
    MS_U32      DelayTime;
} SC_Init_Paras_t;

typedef struct
{
    MS_U32 u32SrcFrom;
    MS_U32 u32SrcType;
} EMM_Init_Paras_t;

MS_BOOL NSK2_SC_Init(SC_Init_Paras_t *pInitPara);
MS_U32 NSK2_ECMFLT_Init(void);
MS_U32 NSK2_EMMFLT_Init(EMM_Init_Paras_t *pInitPara);

#endif /*NSK2HDX_HEADER_H_*/

