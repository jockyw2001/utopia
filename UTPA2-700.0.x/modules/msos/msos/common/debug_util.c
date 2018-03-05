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
//<MStar Software>
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   debug_util.c
/// @brief  debug register module for all other IPs
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"

// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DBG_MSG(x)      //x

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
struct DBG_LIST_ENTRY
{
    const char *Func_Name;
    const char *Func_Help;
    MS_DBG_LINK *pAryDbgLink;
    struct DBG_LIST_ENTRY *next;
};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static struct DBG_LIST_ENTRY *pDbgRoot = NULL;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _MsOS_Dbg_PrintMenuBanner(const char *banner)
{
    printf("============\n");
    printf("  %s\n", banner);
    printf("============\n");
}

static void _MsOS_Dbg_PrintMenu(void)
{
    struct DBG_LIST_ENTRY *pCurrentEntry = pDbgRoot;

    _MsOS_Dbg_PrintMenuBanner("Main");

    while(pCurrentEntry != NULL)
    {
        printf("%s\t:%s\n", pCurrentEntry->Func_Name, pCurrentEntry->Func_Help);
        pCurrentEntry = pCurrentEntry->next;
    }
}

static void _MsOS_Dbg_PrintSubMenu(const char *Func_Name, MS_DBG_LINK *pAryDbgLink)
{
    // no more parameters, print sub menu banner
    _MsOS_Dbg_PrintMenuBanner(Func_Name);

    if(pAryDbgLink != NULL)
    {
        while(pAryDbgLink->pCallBack != NULL)
        {
            printf("%s\t:%s\n", pAryDbgLink->Func_Name, pAryDbgLink->Func_Help);
            pAryDbgLink++;
        }
    }
}

static MS_BOOL _MsOS_Dbg_IsRegisterd(const char *Func_Name)
{
    struct DBG_LIST_ENTRY *pCurrentEntry = pDbgRoot;

    if(pCurrentEntry == NULL)
    {
        return FALSE;
    }
    else
    {
        while(pCurrentEntry->next != NULL)
        {
            if(strcmp(pCurrentEntry->Func_Name, Func_Name) == 0)
            {
                MS_CRITICAL_MSG(printf("[%s] %s registered\n", __func__, Func_Name));
                return TRUE;
            }
            else
            {
                pCurrentEntry = pCurrentEntry->next;
            }
        }
        return FALSE;
    }
}

static MS_BOOL _MsOS_Dbg_ExecuteCB(int argc, char *argv[])
{
    // for example, input is "PNL Init"
    struct DBG_LIST_ENTRY *pCurrentEntry = pDbgRoot;
    int i;

    for(i=0; i<argc; i++)
    {
        DBG_MSG(printf("argv[%d]=%s\n",i,argv[i]));
    }

    while(pCurrentEntry != NULL)
    {
        DBG_MSG(printf("ExecuteCB(%s, %s)\n", argv[0], pCurrentEntry->Func_Name));

        if(strcmp(argv[0], pCurrentEntry->Func_Name) == 0)
        {
            // match, execute call back
            return MsOS_Dbg_ExecuteSubCB(pCurrentEntry->Func_Name, argc-1, argv+1, pCurrentEntry->pAryDbgLink);
        }
        else
        {
            pCurrentEntry = pCurrentEntry->next;
        }
    }

    // not found associate command
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dbg_ExecuteSubCB(const char *Func_Name, int argc, char *argv[], MS_DBG_LINK *pAryDbgLink)
{
    if(pAryDbgLink != NULL)
    {
        if(argc == 0)
        {
            _MsOS_Dbg_PrintSubMenu(Func_Name, pAryDbgLink);
            return TRUE;
        }
        else
        {
            while(pAryDbgLink->pCallBack != NULL)
            {
                if(strcmp(argv[0], pAryDbgLink->Func_Name) == 0)
                {
                    // found call back, execute it
                    return pAryDbgLink->pCallBack(argc-1, argv+1);
                }
                else
                {
                    pAryDbgLink++;
                }
            }

            return FALSE;
        }
    }

    return FALSE;
}

MS_BOOL MsOS_Dbg_Regist(const char *Func_Name, const char *Func_Help, MS_DBG_LINK *pAryDbgLink)
{
    struct DBG_LIST_ENTRY *pNewEntry;
    struct DBG_LIST_ENTRY *pCurrentEntry;

    // check if registerd before
    if(_MsOS_Dbg_IsRegisterd(Func_Name))
    {
        return FALSE;
    }
    else
    {
        pNewEntry = (struct DBG_LIST_ENTRY *) malloc(sizeof(struct DBG_LIST_ENTRY));
        if(pNewEntry == NULL)
        {
            MS_CRITICAL_MSG(printf("[%s] new entry failed\n", __func__));
            return FALSE;
        }
        else
        {
            pNewEntry->Func_Name = Func_Name;
            pNewEntry->Func_Help = Func_Help;
            pNewEntry->pAryDbgLink = pAryDbgLink;
            pNewEntry->next = NULL;
        }

        if(pDbgRoot == NULL)
        {
            // 1st time register
            pDbgRoot = pNewEntry;
        }
        else
        {
            // add to the end of the list
            pCurrentEntry = pDbgRoot;
            while(pCurrentEntry->next != NULL)
            {
                pCurrentEntry = pCurrentEntry->next;
            }
            pCurrentEntry->next = pNewEntry;
        }

        return TRUE;
    }
}

MS_BOOL MsOS_Dbg_ParseCmd(char *Cmd, MS_U32 u32CmdLen)
{
    char * token;
    const char delimiters[] = " \r";
    int argc = 0;
    char *argv[20];

    DBG_MSG(printf("In %s:%d\n",__func__,__LINE__));

    if((u32CmdLen == 1) && (*Cmd == 0x0D))
    {
        // enter only, dump main menu
        _MsOS_Dbg_PrintMenu();
        return TRUE;
    }
    else
    {
        // separate cmd to two part, token and remain cmd
        token = strtok(Cmd, delimiters);
        while ( token != NULL)
        {
            argv[argc] = token;
            DBG_MSG(printf("argv[%d]=%s\n", argc, argv[argc]));
            argc++;
            token = strtok(NULL, delimiters);
        }

        // check parameters
        if((argc == 0) || (argv[0] == NULL))
        {
            // error
            return FALSE;
        }
        else
        {
            // find registered func and execute it
            return _MsOS_Dbg_ExecuteCB(argc, argv);
        }
    }
}

void MsOS_RegMyDbg(void)
{
    return;
}

