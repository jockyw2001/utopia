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

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#include "stdio.h"
#else
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/compat.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#endif
#include "MsCommon.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "hwreg_utility2.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "color_reg.h"
#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "PQ_private.h"
#include "apiPQ_v2.h"

extern  MS_PQ_Mode_Info  _stMode_Info[PQ_MAX_WINDOW];

#define BW_DBG(x) //x

extern MS_BOOL _gIsH264;
extern MS_BOOL _gIsMMplayer;
extern MS_BOOL _gIsMVC4kx1k;
extern MS_BOOL _gIsRmvb[PQ_MAX_WINDOW];
extern MS_BOOL _gbIsG3D; //priority for G3D
extern MS_BOOL _gbSubWin_En;
extern MS_BOOL _gIsDualDecode;

typedef struct
{
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
    MS_U8 u8TabIdx;
} TAB_Info;

#ifndef PQ_BW_RMVB_ENABLE
#define PQ_BW_RMVB_ENABLE 0
#endif

#ifndef PQ_BW_MVC4kx1k_ENABLE
#define PQ_BW_MVC4kx1k_ENABLE 0
#endif

#ifndef PQ_BW_G3D_ENABLE
#define PQ_BW_G3D_ENABLE 0
#endif

#ifndef PQ_BW_PIP_ENABLE
#define PQ_BW_PIP_ENABLE 0
#endif

#ifndef PQ_BW_DUALDECODE_ENABLE
#define PQ_BW_DUALDECODE_ENABLE 0
#endif

#ifndef PQ_BW_HDMI4kx2k_ENABLE
#define PQ_BW_HDMI4kx2k_ENABLE 0
#endif

#ifndef PQ_BW_1KTraveling_ENABLE
#define PQ_BW_1KTraveling_ENABLE 0
#endif

#if PQ_BW_1KTraveling_ENABLE
extern MS_BOOL _gIs1KTraveling;
#endif

#include "QualityMap_BW.c"

#if !defined(MSOS_TYPE_LINUX_KERNEL)
MS_U8* Open_bandwidthtable_Bin(const MS_U8 * pName)
{
    FILE *pFile;
    MS_U32 u32Rst;
    MS_U8 *pu8TempBuffer;
    MS_S32 size;

    pFile = fopen((const char*)pName, "rb");
    if(pFile == NULL)
    {
        printf("Open Bandwidthtable.bin Failed\n");
        return NULL;
    }

    fseek(pFile, 0, SEEK_END);
    size = ftell(pFile);

    if (size <= 0)
    {
        fclose(pFile);
        return NULL;
    }

    rewind(pFile);
    pu8TempBuffer = (MS_U8*) malloc((MS_U32)size);
    if(pu8TempBuffer == NULL)
    {
        printf("out of memory \n");
        fclose(pFile);
        return NULL;
    }

    u32Rst = fread(pu8TempBuffer, 1, (MS_U32)size, pFile);
    if(u32Rst != (MS_U32)size)
    {
        printf("Read Bandwidthtable.bin Error %td %td\n", (ptrdiff_t)u32Rst,(ptrdiff_t) ((MS_U32)size));
    #if !defined(MSOS_TYPE_LINUX_KERNEL)
        free(pu8TempBuffer);
    #endif
        fclose(pFile);
        return NULL;
    }
    fclose(pFile);
    return pu8TempBuffer;
}
#else

MS_U8* Open_bandwidthtable_Bin(const MS_U8 * pName)
{
    long lFileSize;
    MS_U32 u32Rst;
    const char * Name;
    MS_U8 *pu8Buffer;
    Name = (const char *)pName;
    struct file *pFile = NULL;

    mm_segment_t cur_mm_seg;
    pFile = filp_open(Name, O_RDONLY, 0);

    if(IS_ERR(pFile))
    {
        printk("Open %s Failed\n", pName);

        return NULL;
    }

    vfs_llseek(pFile, 0, SEEK_END);
    lFileSize = pFile->f_pos;
    if (lFileSize <= 0)
        {
        filp_close(pFile, NULL);

            return NULL;
        }
    loff_t pos = vfs_llseek(pFile, 0, SEEK_SET);
    pu8Buffer = (MS_U8*)vmalloc(lFileSize);

    if(pu8Buffer == NULL)
    {
        printk("out of memory \n");
        filp_close(pFile, NULL);

            return NULL;
    }
    cur_mm_seg = get_fs();
    set_fs(KERNEL_DS);
    u32Rst = vfs_read(pFile, pu8Buffer, lFileSize, &pos);
    set_fs(cur_mm_seg);
    if(u32Rst != (MS_U32)lFileSize)
    {
        printk("Read %s Error %d %ld\n",pName, u32Rst, lFileSize);
        vfree(pu8Buffer);
        filp_close(pFile, NULL);

        return NULL;
    }

    filp_close(pFile, NULL);
    return pu8Buffer;

}
#endif


#if 1 /// for New MIU HW design
static void _MDrv_BW_LoadTable(void *pInstance,MS_U8 u8TabIdx)
{
    MS_U8 *pu8Buffer;

    const MS_U8 bandwidthfileName[] = "/config/pq/Bandwidth_RegTable.bin";
    pu8Buffer = Open_bandwidthtable_Bin(bandwidthfileName);

    _MDrv_BW_PushBuffer(u8TabIdx, pu8Buffer);
    //printk("[%s] [%d] u8TabIdx=%lu\n", __FUNCTION__, __LINE__,u8TabIdx);
    BW_DBG(printk("[BW]LoadTable\n"));
#if !defined(MSOS_TYPE_LINUX_KERNEL)
    free(pu8Buffer);
#endif
}

#else
static void _MDrv_BW_LoadTable(void *pInstance,MS_U8 u8TabIdx)
{
    TAB_Info tab_Info;
    tab_Info.pIPTable  = (void*)BWTABLE;
    tab_Info.u8TabNums = BWTABLE_NUMS;
    tab_Info.u8TabIdx = u8TabIdx;
    MS_U32 u32Index = 0;

    MS_U8 *pu8Buffer;

    const MS_U8 bandwidthfileName[] = "/config/pq/Bandwidth_RegTable.bin";
    pu8Buffer = Open_bandwidthtable_Bin(bandwidthfileName);
    if(pu8Buffer != NULL)
    {

        tab_Info.pIPTable = pu8Buffer;
#if 1//def NEW_MIU
        printf("@@#@@[%s][%d]\n", __FUNCTION__, __LINE__);
        while(!(((pu8Buffer[u32Index]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+1]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+2]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+3]&0xFF) == 0x00) && ((pu8Buffer[u32Index+4]&0xFF) == 0x00)))
        {
            u32Index++;
            printf("@@#@@[%s][%d]u32Index=%lu\n", __FUNCTION__, __LINE__,u32Index);
        }

        tab_Info.pIPTable = pu8Buffer +u32Index + 5;
#else
        while(!(((pu8Buffer[u32Index]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+1]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+2]&0xFF) == 0x00) && ((pu8Buffer[u32Index+3]&0xFF) == 0x00)))
        {
            u32Index++;
        }

        tab_Info.pIPTable = pu8Buffer +u32Index + 4;
#endif
        printf("Read Bandwidthtable.bin\n");
    }

    BW_DBG(printf("[BW]LoadTable\n"));
    _MDrv_BW_DumpTable(&tab_Info);
    free(pu8Buffer);
}

#endif

void MDrv_BW_CusMode_LoadTable_U2(void *pInstance, MS_U8 u8TabIdx)
{
    if(u8TabIdx >= BWTABLE_NUMS)
    {
       BW_DBG(printf("[BW]LoadTable u8TabIdx bigger than BWTABLE_NUMS\n");)
       return;
    }

    _MDrv_BW_LoadTable(pInstance,u8TabIdx);
}

void MDrv_BW_CusMode_LoadTable( MS_U8 u8TabIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32BWInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ, &pu32BWInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
    }

    stPQ_BW_CusMode_LoadTable PQArgs;
    PQArgs.u8TabIdx = u8TabIdx;
    if(UtopiaIoctl(pu32BWInst, E_PQ_CMD_BW_CUSMODE_LOADTABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
    }
}

void MDrv_BW_LoadInitTable_U2(void *pInstance)
{
    TAB_Info tab_Info;
    tab_Info.pIPTable = (void*)BWTABLE_COM;
    tab_Info.u8TabNums = 1;
    tab_Info.u8TabIdx = 0;
    MS_U8 u8TabIdx = 0;

    MS_U8 *pu8Buffer;
    const MS_U8 bandwidthfileName[] = "/config/pq/Bandwidth_RegTable.bin";
    pu8Buffer = Open_bandwidthtable_Bin(bandwidthfileName);
    if(pu8Buffer != NULL)
    {
        tab_Info.pIPTable = pu8Buffer;
        printf("Read Bandwidthtable.bin\n");
    }

    BW_DBG(printf("[BW]LoadCommTable\n"));
    _MDrv_BW_DumpTable(&tab_Info);

#if !defined(MSOS_TYPE_LINUX_KERNEL)
    free(pu8Buffer);
#endif

    //load normal as init status
    u8TabIdx = BWTABLE_Normal_mode;
    _MDrv_BW_LoadTable(pInstance,u8TabIdx);

}
void MDrv_BW_LoadInitTable(void)
{
    if (pu32BWInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ, &pu32BWInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
    }

    if(UtopiaIoctl(pu32BWInst, E_PQ_CMD_BW_LOAD_INIT_TABLE, NULL) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

MS_BOOL MDrv_BW_IsDualDecode(void)
{
    if(  ((MDrv_PQ_GetInputSourceType(PQ_MAIN_WINDOW) == PQ_INPUT_SOURCE_STORAGE) && (MDrv_PQ_GetInputSourceType(PQ_SUB_WINDOW) == PQ_INPUT_SOURCE_STORAGE))
      || ((MDrv_PQ_GetInputSourceType(PQ_MAIN_WINDOW) == PQ_INPUT_SOURCE_STORAGE) && (MDrv_PQ_GetInputSourceType(PQ_SUB_WINDOW) == PQ_INPUT_SOURCE_DTV))
      || ((MDrv_PQ_GetInputSourceType(PQ_MAIN_WINDOW) == PQ_INPUT_SOURCE_DTV) && (MDrv_PQ_GetInputSourceType(PQ_SUB_WINDOW) == PQ_INPUT_SOURCE_STORAGE))
      )
    {
        _gIsDualDecode = TRUE;
    }
    else
    {
        _gIsDualDecode = FALSE;
    }
    return _gIsDualDecode;
}

void MDrv_BW_LoadTableByContext_U2(void* pInstance,PQ_WIN eWindow)
{
    MS_U16 u16Input_HSize = 0;
    MS_U16 u16Input_VSize = 0;
    MS_U8 u8TabIdx = 0;
    MS_U16 u16Input_VFreq = 0;
    MS_BOOL bIsInterlaced = FALSE;

    bIsInterlaced  = _stMode_Info[eWindow].bInterlace;
    u16Input_HSize = _stMode_Info[eWindow].u16input_hsize;
    u16Input_VSize = _stMode_Info[eWindow].u16input_vsize;
    u16Input_VFreq = _stMode_Info[eWindow].u16input_vfreq;

    printf("BW Window:%u, PQSource:%u, Hsize:%u, Vsize: %u, interlace:%u, Vfreq=%u\n",
                    eWindow, MDrv_PQ_GetInputSourceType_U2(pInstance, eWindow), u16Input_HSize, u16Input_VSize, bIsInterlaced, u16Input_VFreq);
#if PQ_BW_DUALDECODE_ENABLE
    if(MDrv_BW_IsDualDecode() && _gbSubWin_En)
    {
        u8TabIdx = BWTABLE_DUALDECODE_mode;
    }
    else
#endif
#if PQ_BW_G3D_ENABLE
    if(_gbIsG3D)
    {
        u8TabIdx = BWTABLE_G3D_mode;
    }
    else
#endif
#if PQ_BW_PIP_ENABLE
    if(_gbSubWin_En)
    {
        u8TabIdx = BWTABLE_PIP_mode;
    }
    else
#endif
#if PQ_BW_RMVB_ENABLE
    if( _gIsRmvb[PQ_MAIN_WINDOW] || _gIsRmvb[PQ_SUB_WINDOW])
    {
        BW_DBG(printf("RMVB flag: main:%u, sub:%u\n",_gIsRmvb[PQ_MAIN_WINDOW],_gIsRmvb[PQ_SUB_WINDOW]));
        u8TabIdx = BWTABLE_Rmvb_mode ;
#if PQ_BW_1KTraveling_ENABLE
        if (_gIs1KTraveling)
        {
            u8TabIdx = BWTABLE_VLC_MM_1080p_mode;
            printf("load VLC_MM_1080p_mode in Rmvb\n");
        }
#endif
    }
    else
#endif
#if PQ_BW_MVC4kx1k_ENABLE
    if (   (u16Input_HSize>=3800) && (u16Input_VSize >= 1080)
        && (MDrv_PQ_GetInputSourceType_U2(pInstance, PQ_MAIN_WINDOW) != PQ_INPUT_SOURCE_HDMI))
    {

        u8TabIdx = BWTABLE_MVC4kx1k_mode;
    }
    else
#endif
#if (PQ_BW_MM_ENABLE && PQ_BW_H264_ENABLE)
#if PQ_BW_1KTraveling_ENABLE
    if (_gIsH264 && _gIsMMplayer && _gIs1KTraveling)
    {
        u8TabIdx = BWTABLE_VLC_MM_1080p_mode;
        printf("load VLC_MM_1080p_mode in H264\n");
    }
    else
#endif
    if((u16Input_HSize>=1440) && (u16Input_VSize >= 900) && _gIsH264 &&_gIsMMplayer)
    {
        u8TabIdx = BWTABLE_MM_HD_H264_mode;
    }
    else
#endif
#if PQ_BW_H264_ENABLE
    if((u16Input_HSize>=1440) && (u16Input_VSize >= 900) && _gIsH264)
    {
        u8TabIdx = BWTABLE_HD_H264_mode;

    }
    else
#endif
#if PQ_BW_MM_ENABLE
#if PQ_BW_1KTraveling_ENABLE
    if (_gIsMMplayer && _gIs1KTraveling)
    {
        u8TabIdx = BWTABLE_VLC_MM_1080p_mode;
        printf("load VLC_MM_1080p_mode in MM\n");
    }
    else
#endif
    if ((u16Input_HSize>=3840) && (u16Input_VSize >= 1080) && _gIsMMplayer)
    {
        // 4K2K and 4K1K MM bandwidth
        u8TabIdx = BWTABLE_MM_4k2k_mode;
    }
    else
    if (_gIsMMplayer)
    {
        // FHD MM bandwidth
        u8TabIdx = BWTABLE_MM_1080p_mode;
    }
    else
#endif
#if PQ_BW_HDMI4kx2k_ENABLE
    if (   (u16Input_HSize>=3800) && (u16Input_VSize >= 2000)
        && (MDrv_PQ_GetInputSourceType_U2(pInstance, PQ_MAIN_WINDOW) == PQ_INPUT_SOURCE_HDMI))
    {
        u8TabIdx = BWTABLE_HDMI4kx2k_mode;
    }
    else
#endif
    if ((u16Input_HSize>=1440) && (u16Input_VSize >= 900))
    {
        u8TabIdx = BWTABLE_1080p_mode;
    }
    else
    {
        u8TabIdx = BWTABLE_Normal_mode;
    }

#if (PQ_BW_PIP_ENABLE == 0)
    if(eWindow == PQ_MAIN_WINDOW)
#endif
    {
        _MDrv_BW_LoadTable(pInstance,u8TabIdx);
    }
}
void MDrv_BW_LoadTableByContext(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32BWInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ, &pu32BWInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
    }

    stPQ_BW_LoadTableByContext PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32BWInst, E_PQ_CMD_BW_LOAD_TABLE_BY_CONTEXT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
void MDrv_BW_ResetToDefault_U2(void* pInstance)
{
    printf("Load default BW table\n");
    _MDrv_BW_LoadTable(pInstance,BWTABLE_Normal_mode);
}
void MDrv_BW_resetToDefault(void)
{
    if (pu32BWInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ, &pu32BWInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
    }

    if(UtopiaIoctl(pu32BWInst, E_PQ_CMD_BW_RESET_TO_DEFAULT, NULL) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

EXPORT_SYMBOL(MDrv_BW_LoadInitTable);
EXPORT_SYMBOL(MDrv_BW_LoadTableByContext);
