//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
/// file    XC_ModeParse_adp.c
/// @brief  Scaler API layer Interface.
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_MODEPARSE_ADP_C_
#define _XC_MODEPARSE_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_ModeParse.h"
#include "apiXC_ModeParse_v2.h"
#include "apiXC_v2.h"
#include "XC_ModeParse_adp.h"
#include "XC_adp.h"


#define MAX_MODEPARSER_TAB_NUM 100
//------------------------------------------------------------------------------
//  Driver Compiler Options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                                    if(copy_from_user(a,b,c) != 0) { break; }
#define CPY_to_USER(a,b,c)                                      if(copy_to_user(a,b,c) != 0) { break; }
#else
#define CPY_FROM_USER                                           memcpy
#define CPY_to_USER                                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                                           compat_ptr(a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)compat_ptr((unsigned long)a)))
#define IS_CMP_TASK()                                             is_compat_task()

#define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
#define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }

#else 
#define COMPAT_PTR(a)                                           (a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)a))
#define IS_CMP_TASK()                                             (FALSE)
#define CMP_CPY_FROM_USER                                       CPY_FROM_USER
#define CMP_CPY_TO_USER                                         CPY_to_USER
#endif //CONFIG_COMPAT
//------------------------------------------------------------------------------
//  Local Structurs
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------
#ifdef CONFIG_COMPAT
typedef struct DLL_PACKED
{
    compat_uptr_t ModeDB;
    MS_U8 u8NumberOfItems;
    compat_uptr_t psInputInfo;
    XC_MODEPARSE_RESULT eReturnValue;
} _compat_stXC_MODEPARSE_MATCHMODE;
#endif
//Function parameter
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_INIT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MATCHMODE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MATCHMODEEX[5];

//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MODETABLE_TYPE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_INPUT_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MODETABLE_TYPE_EX[5];


//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 XC_MODEPARSE_adp_Init(void)
{

//set table
    MS_U32 temp=0;

    //Function parameter
    //pstXC_MODEPARSE_INIT         pXC_MODEPARSE_INIT        = (pstXC_MODEPARSE_INIT)temp;
    pstXC_MODEPARSE_MATCHMODE    pXC_MODEPARSE_MATCHMODE   = (pstXC_MODEPARSE_MATCHMODE)temp;
    pstXC_MODEPARSE_MATCHMODEEX  pXC_MODEPARSE_MATCHMODEEX = (pstXC_MODEPARSE_MATCHMODEEX)temp;


    //Function parameter's pointer
    //MS_PCADC_MODETABLE_TYPE*     pXC_MODEPARSE_ModeDB      = (MS_PCADC_MODETABLE_TYPE*)temp;
    //XC_MODEPARSE_INPUT_INFO*     pXC_MODEPARSE_InputInfo   = (XC_MODEPARSE_INPUT_INFO*)temp;
    //MS_PCADC_MODETABLE_TYPE_EX*  pXC_MODEPARSE_ModeDB_EX   = (MS_PCADC_MODETABLE_TYPE_EX*)temp;

    //Function parameter's pointer
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_PCADC_MODETABLE_TYPE)*100);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_INPUT_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_MODEPARSE_INPUT_INFO));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_INPUT_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE_EX[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_PCADC_MODETABLE_TYPE_EX)*MAX_MODEPARSER_TAB_NUM);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE_EX[1],UADP_SPT_END , 0, 0);

    //Function parameter
    UADPBypassSetSPT(&spt_XC_MODEPARSE_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_MODEPARSE_INIT));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_MODEPARSE_MATCHMODE));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODE->ModeDB)) - (unsigned long)pXC_MODEPARSE_MATCHMODE), (unsigned long)spt_XC_MODEPARSE_MODETABLE_TYPE);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[2],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODE->psInputInfo)) - (unsigned long)pXC_MODEPARSE_MATCHMODE), (unsigned long)spt_XC_MODEPARSE_INPUT_INFO);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[3],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_MODEPARSE_MATCHMODEEX));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODEEX->ModeDB)) - (unsigned long)pXC_MODEPARSE_MATCHMODEEX), (unsigned long)spt_XC_MODEPARSE_MODETABLE_TYPE_EX);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[2],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODEEX->psInputInfo)) - (unsigned long)pXC_MODEPARSE_MATCHMODEEX), (unsigned long)spt_XC_MODEPARSE_INPUT_INFO);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[3],UADP_SPT_END , 0, 0);


    return 0;

}

MS_U32 XC_MODEPARSE_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[2];

    switch(u32Cmd)
    {
        
        case E_XC_MODEPARSE_CMD_INIT :
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_XC_MODEPARSE_INIT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_MODEPARSE_CMD_MATCHMODE:
        {
            stXC_MODEPARSE_MATCHMODE tmp;
#ifdef CONFIG_COMPAT
            _compat_stXC_MODEPARSE_MATCHMODE tmp_compat;
            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER(&tmp_compat, pArgs, sizeof(_compat_stXC_MODEPARSE_MATCHMODE)); 
                tmp.ModeDB = malloc(sizeof(MS_PCADC_MODETABLE_TYPE)*tmp_compat.u8NumberOfItems);
                tmp.psInputInfo = malloc(sizeof(XC_MODEPARSE_INPUT_INFO));
                CMP_CPY_FROM_USER(tmp.ModeDB, tmp_compat.ModeDB, sizeof(MS_PCADC_MODETABLE_TYPE)*tmp_compat.u8NumberOfItems); 
                CMP_CPY_FROM_USER(tmp.psInputInfo, tmp_compat.psInputInfo, sizeof(XC_MODEPARSE_INPUT_INFO)); 
                tmp.u8NumberOfItems=tmp_compat.u8NumberOfItems;
                tmp.eReturnValue=tmp_compat.eReturnValue;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp);   
                tmp_compat.eReturnValue = tmp.eReturnValue;
                tmp_compat.u8NumberOfItems = tmp.u8NumberOfItems;
                //CMP_CPY_TO_USER(tmp_compat.ModeDB,tmp.ModeDB,sizeof(MS_PCADC_MODETABLE_TYPE));                //example
                CMP_CPY_TO_USER(tmp_compat.psInputInfo,tmp.psInputInfo,sizeof(XC_MODEPARSE_INPUT_INFO));      
                CMP_CPY_TO_USER(pArgs,&tmp_compat,sizeof(_compat_stXC_MODEPARSE_MATCHMODE));
                free(tmp.ModeDB);
                free(tmp.psInputInfo);
            }else
            {
#endif
                void *ptr;
                void *ptr1;
                CPY_FROM_USER(&tmp, pArgs, sizeof(stXC_MODEPARSE_MATCHMODE));
                ptr=tmp.ModeDB;
                ptr1=tmp.psInputInfo; 
                tmp.ModeDB = malloc(sizeof(MS_PCADC_MODETABLE_TYPE)*tmp.u8NumberOfItems);
                tmp.psInputInfo = malloc(sizeof(XC_MODEPARSE_INPUT_INFO));
                CPY_FROM_USER(tmp.ModeDB, ptr, sizeof(MS_PCADC_MODETABLE_TYPE)*tmp.u8NumberOfItems); 
                CPY_FROM_USER(tmp.psInputInfo, ptr1, sizeof(XC_MODEPARSE_INPUT_INFO)); 
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp);   
                CPY_to_USER(pArgs,&tmp,sizeof(stXC_MODEPARSE_MATCHMODE));
                free(tmp.ModeDB);
                free(tmp.psInputInfo);
#ifdef CONFIG_COMPAT
            }
#endif
            break; 
        }

        case E_XC_MODEPARSE_CMD_MATCHMODEEX:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_MODEPARSE_MATCHMODEEX, spt_XC_MODEPARSE_MATCHMODEEX,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_MODEPARSE_CMD_NONE:
        case E_XC_MODEPARSE_CMD_MAX:
        default:
        {
            printf("Command %ld is not existed\n",u32Cmd);
            u32Ret = UTOPIA_STATUS_ERR_INVALID_HANDLE;
            break;
        }

    }

	return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

#endif


