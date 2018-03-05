////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   osalHVD.h
// @brief  HVD Driver Interface
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////
// Modify from osalHVD_EX.h BY Apple.Chen //
////////////////////////////////////////////

#ifndef __OSAL_MVOP_H__
#define __OSAL_MVOP_H__

#include "MsOS.h"

//#if defined( MSOS_TYPE_LINUX) || defined( MSOS_TYPE_ECOS) || defined( MSOS_TYPE_NOS)
//    #define HVD_ENABLE_MSOS_SYSTEM_CALL     1
//#endif


//#define HVD_ISR_VECTOR          E_INT_IRQ_SVD_HVD
#define OSAL_MVOP_MUTEX_TIMEOUT  MSOS_WAIT_FOREVER



MS_S32 OSAL_MVOP_MutexCreate(MS_U8* pu8Name);
MS_BOOL OSAL_MVOP_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL OSAL_MVOP_MutexRelease(MS_S32 s32MutexId);
MS_BOOL OSAL_MVOP_MutexDelete(MS_S32 s32MutexId);

//MS_BOOL OSAL_HVD_ISR_Attach(void* pfnISRCB);
//MS_BOOL OSAL_HVD_ISR_Detach(void);
//MS_BOOL OSAL_HVD_ISR_Enable(void);
//MS_BOOL OSAL_HVD_ISR_Disable(void);

#endif // #ifndef __OSAL_HVD_H__

