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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    halDMS.c
/// @author MStar Semiconductor Inc.
/// @brief  DMS Utility HAL
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  HAL_DMS_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
//#include "apiDMS_Type.h"
#include "drvDMS.h"
#include "halDMS.h"
#include "UFO.h"
#include "apiPNL.h"

#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDMS_private.h"

#ifndef ANDROID
#define DMS_PRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#define DMS_PRINTF ALOGD
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define REG_MEMSYNC_BLACK_MODE               (0x117E)
#define BIT_MEMSYNC_PIXEL_MASK_EN            (0x100)
#define BIT_MEMSYNC_MOTION_RATIO_MASK_EN     (0x200)
#define BIT_MEMSYNC_RESET                    (0x400)
#define BIT_MEMSYNC_DELAY_ONE_FRAME          (0x800)
#define BIT_MEMSYNC_DOTLINE                  (0x1000)
static MS_BOOL _DMS_XC_SetVideoMute_AutoSync(MS_BOOL bEnable)
{
    E_APIXC_ReturnValue eXcRet;

    eXcRet = MApi_XC_GenerateBlackVideoByMode(bEnable, MAIN_WINDOW,E_XC_BLACK_VIDEO_MEMSYNC);
    if(eXcRet != E_APIXC_RET_OK)
    {
        DMS_PRINTF("[%s][%d]MApi_XC_GenerateBlackVideoByMode fail:%d", __FUNCTION__, __LINE__, eXcRet);
        return FALSE;
    }
    return TRUE;
}

MS_BOOL Hal_DMS_XC_SetMute(void* pInstance, MS_BOOL bEnable)
{
    MS_BOOL bRet = FALSE;
    MApi_XC_SkipWaitVsync(MAIN_WINDOW,TRUE);
    bRet = _DMS_XC_SetVideoMute_AutoSync(bEnable);
    MApi_XC_SkipWaitVsync(MAIN_WINDOW,FALSE);
    return bRet;
}

#define H_PostScalingRatio(Input, Output)          ((MS_U32)( ((MS_U64)(Input)) * 2097152ul / (Output) + 1 )/2)
#define V_PostScalingRatio(Input, Output)          ((MS_U32)( ((MS_U64)(Input) * 2097152ul) / Output + 1) / 2)
#define DMS_DS_DEPTH 0x8
#define DMS_DS_SET_COUNT 0x6
#define ALL_IP_COMMAND 0
extern void* g_pDMSRes;

MS_BOOL HAL_DMS_XC_DSSetConfig(void* pInstance)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    memset(&pDMSResPri->stHalDMS.stDMS_DSSWDB, 0 , sizeof(pDMSResPri->stHalDMS.stDMS_DSSWDB));
    pDMSResPri->stHalDMS.phyDSAddr = MApi_XC_R4BYTE(0x1F22);
    pDMSResPri->stHalDMS.phyDSAddr *= 0x20;
    printf("Get DS Addr 0x%llx\n",pDMSResPri->stHalDMS.phyDSAddr);
    // SC0_MCU_DS_AVAILABLE
    //Set DS is out of DMS
    #if 0
    XC_DynamicScaling_Info stDSInfo;
    stDSInfo.u32DS_Info_BaseAddr = gphy_DSAddr;
    stDSInfo.bOP_DS_On           = 0;
    stDSInfo.u8DS_Index_Depth    = DMS_DS_DEPTH;
    stDSInfo.bOP_DS_On           = TRUE;
    stDSInfo.bIPS_DS_On          = FALSE;
    stDSInfo.bIPM_DS_On          = TRUE;
    MApi_XC_SetDynamicScaling(&stDSInfo,sizeof(stDSInfo),MAIN_WINDOW);
    #endif
    #if ALL_IP_COMMAND
    MApi_XC_W2BYTEMSK(0x1F26,0x10,0xFF);
    #else
    //MApi_XC_W2BYTEMSK(0x2032,0x0,0x300);
    #endif
    return TRUE;
}

#define DS_CMD_LEN_64BITS                   8
#define BYTE_PER_WORD           (32)  // MIU 128: 16Byte/W, MIU 256: 32Byte/W
#define OP_BASE_OFFSET (2 * DMS_DS_SET_COUNT * DMS_DS_DEPTH * BYTE_PER_WORD)
void Hal_DMS_ResetSWDSCommand(void* pInstance, MS_U8 u8DSIndex)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

#if ALL_IP_COMMAND
    MS_PHY u32IndexBase = pDMSResPri->stHalDMS.phyDSAddr + u8DSIndex * DMS_DS_DEPTH * 2 * BYTE_PER_WORD;
#else
    MS_PHY u32IndexBase = pDMSResPri->stHalDMS.phyDSAddr + u8DSIndex * DMS_DS_DEPTH * BYTE_PER_WORD;
    MS_PHY u32IndexBase_OP = u32IndexBase + OP_BASE_OFFSET;
#endif

    MS_U16 u16TotalCmdCnt = DMS_DS_DEPTH;
    MS_U16 i = 0;

    MS_U64 *pu64Data = NULL;
    u16TotalCmdCnt *= 2;
    u16TotalCmdCnt *= 2;

    for(i = 0; i < u16TotalCmdCnt; i++)
    {
        pu64Data  = (MS_U64*)MsOS_PA2KSEG1(u32IndexBase + i*DS_CMD_LEN_64BITS);
        *pu64Data = 0x00000009FFFF0000;
    }
#if (ALL_IP_COMMAND == 0)
    for(i = 0; i < u16TotalCmdCnt; i++)
    {
        pu64Data  = (MS_U64*)MsOS_PA2KSEG1(u32IndexBase_OP + i*DS_CMD_LEN_64BITS);
        *pu64Data = 0x00000009FFFF0000;
    }
#endif
}

void Hal_DMS_WriteSWDSCommand(void* pInstance, SCALER_WIN eWindow, MS_U32 u32CmdRegAddr, MS_U16 u16CmdRegValue, DMS_ds_reg_ip_op_sel IPOP_Sel, DMS_ds_reg_source_sel Source_Select, DMS_DS_CMDCNT *pstXC_DS_CmdCnt)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

    if(pstXC_DS_CmdCnt->u16CMDCNT_IPM == 0 && pstXC_DS_CmdCnt->u16CMDCNT_OPM == 0)
    {
        Hal_DMS_ResetSWDSCommand(pInstance, pDMSResPri->stHalDMS.u8DSIndex);
    }
#if ALL_IP_COMMAND
    MS_PHY u32Addr = pDMSResPri->stHalDMS.phyDSAddr + pDMSResPri->stHalDMS.u8DSIndex * DMS_DS_DEPTH * 2 * BYTE_PER_WORD;
#else
    MS_PHY u32Addr = pDMSResPri->stHalDMS.phyDSAddr + pDMSResPri->stHalDMS.u8DSIndex * DMS_DS_DEPTH * BYTE_PER_WORD;
#endif

    //XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] u32DynamicScalingBaseAddr:%x  CurIdx:%d  IdxDepth:%d  BYTE_PER_WORD:%d  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr,(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow],(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth,(int)BYTE_PER_WORD);
    //XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] u32Addr:%x  \n",__func__,__LINE__,(int)u32Addr);

    MS_U64 *pu64Data = NULL;
    MS_U64 u64CmdTemp = 0;
    MS_U8  u8AddrTemp = 0;
    MS_U16 u16BankTemp = 0;
    MS_U16 u16DataTemp = 0;
    MS_U16 u16MaskTemp = 0;
    MS_U8 u8MaxCmdCnt = DMS_DS_DEPTH * 2;
#if ALL_IP_COMMAND
    u8MaxCmdCnt *= 2;
#endif

    if(Source_Select == DMS_DS_XC)
    {
        u16MaskTemp = 0xFFFF;
        u16DataTemp = u16CmdRegValue;

        u8AddrTemp= (u32CmdRegAddr & 0x000000FF) >> 1;
        u16BankTemp= 0x1300 | ((u32CmdRegAddr >> 8) & 0x000000FF); // 0x13XX xc sread mode address

        u64CmdTemp|= (MS_U64)u16DataTemp;
        u64CmdTemp|= ((MS_U64)u8AddrTemp<<16);
        u64CmdTemp|= ((MS_U64)u16BankTemp<<23);
        u64CmdTemp|= ((MS_U64)u16MaskTemp<<48);

        if(IPOP_Sel == DS_IP)
        {
            if(eWindow == MAIN_WINDOW)
            {
                MS_U32 u32MemShift_IPM;

                if((pstXC_DS_CmdCnt->u16CMDCNT_IPM % 2) == 0)
                {
                    u32MemShift_IPM = (pstXC_DS_CmdCnt->u16CMDCNT_IPM / 2) * BYTE_PER_WORD;
                }
                else
                {
                    u32MemShift_IPM = (pstXC_DS_CmdCnt->u16CMDCNT_IPM / 2) * BYTE_PER_WORD  + DS_CMD_LEN_64BITS;
                }
                //XC_PRINTF("[%s,%5d] [DS_IP]pstXC_DS_CmdCnt->u16CMDCNT_IPM:%d u32MemShift_IPM:%lx \n",__func__,__LINE__,pstXC_DS_CmdCnt->u16CMDCNT_IPM,(MS_U32)u32MemShift_IPM);
                //u32Addr = u32Addr + DS_CMD_LEN_64BITS * pstXC_DS_CmdCnt->u16CMDCNT_IPM;
                u32Addr = u32Addr + u32MemShift_IPM;
                if(0)//u32Addr == 0x11956200)
                {
                    printf("[%s] addr 0x%llx, 0x%llx\n",__FUNCTION__,*((MS_U64*)MsOS_PA2KSEG1(0x11956200)),u64CmdTemp);
                }

                pu64Data = (MS_U64*)MsOS_PA2KSEG1(u32Addr);//IPM

                if(pstXC_DS_CmdCnt->u16CMDCNT_IPM < u8MaxCmdCnt)
                {
                    *pu64Data = u64CmdTemp;
                    pstXC_DS_CmdCnt->u16CMDCNT_IPM++;
                }
                else
                {
                    printf("[%s,%5d] [DS] IPM Command count overflow !!  u16CMDCNT_IPM:%d  Max command count: %d\n",__func__,__LINE__,pstXC_DS_CmdCnt->u16CMDCNT_IPM,DMS_DS_DEPTH * 2);
                }
            }
        }
        else
        {
            if(eWindow == MAIN_WINDOW)
            {
                MS_U32 u32MemShift_OPM;

                if((pstXC_DS_CmdCnt->u16CMDCNT_OPM % 2) == 0)
                {
                    u32MemShift_OPM = (pstXC_DS_CmdCnt->u16CMDCNT_OPM / 2) * BYTE_PER_WORD;
                }
                else
                {
                    u32MemShift_OPM = (pstXC_DS_CmdCnt->u16CMDCNT_OPM / 2) * BYTE_PER_WORD  + DS_CMD_LEN_64BITS;
                }
                //XC_PRINTF("[%s,%5d] [DS_IP]pstXC_DS_CmdCnt->u16CMDCNT_IPM:%d u32MemShift_IPM:%lx \n",__func__,__LINE__,pstXC_DS_CmdCnt->u16CMDCNT_IPM,(MS_U32)u32MemShift_IPM);
                //u32Addr = u32Addr + DS_CMD_LEN_64BITS * pstXC_DS_CmdCnt->u16CMDCNT_IPM;
                u32Addr = u32Addr + u32MemShift_OPM + OP_BASE_OFFSET;
                pu64Data = (MS_U64*)MsOS_PA2KSEG1(u32Addr);//IPM

                if(pstXC_DS_CmdCnt->u16CMDCNT_OPM < u8MaxCmdCnt)
                {
                    *pu64Data = u64CmdTemp;
                    pstXC_DS_CmdCnt->u16CMDCNT_OPM++;
                }
                else
                {
                    printf("[%s,%5d] [DS] OPM Command count overflow !!  u16CMDCNT_OPM:%d  Max command count: %d\n",__func__,__LINE__,pstXC_DS_CmdCnt->u16CMDCNT_OPM,DMS_DS_DEPTH * 2);
                }
            }
        }
    }
    else if(Source_Select == DMS_DS_MVOP)// only in IP cmd
    {
        u16DataTemp = u16CmdRegValue;
        u8AddrTemp = (u32CmdRegAddr&0x000000FF);
        u16BankTemp = (u32CmdRegAddr&0x00FFFF00) >> 8; //EX: mvop mainwinodw: 0x1014 mvop bank
        u16MaskTemp = 0xFFFF;

        u64CmdTemp|= (MS_U64)u16DataTemp;
        u64CmdTemp|= ((MS_U64)u8AddrTemp<<16);
        u64CmdTemp|= ((MS_U64)u16BankTemp<<23);
        u64CmdTemp|= ((MS_U64)u16MaskTemp<<48);

        if(eWindow == MAIN_WINDOW)
        {
            MS_U32 u32MemShift_IPM;

            if((pstXC_DS_CmdCnt->u16CMDCNT_IPM % 2) == 0)
            {
                u32MemShift_IPM = (pstXC_DS_CmdCnt->u16CMDCNT_IPM / 2) * BYTE_PER_WORD;
            }
            else
            {
                u32MemShift_IPM = (pstXC_DS_CmdCnt->u16CMDCNT_IPM / 2) * BYTE_PER_WORD  + DS_CMD_LEN_64BITS;
            }
            //XC_PRINTF("[%s,%5d] [DS_IP(MVOP)]pstXC_DS_CmdCnt->u16CMDCNT_IPM:%d u32MemShift_IPM:%lx \n",__func__,__LINE__,pstXC_DS_CmdCnt->u16CMDCNT_IPM,(MS_U32)u32MemShift_IPM);
            //u32Addr = u32Addr + DS_CMD_LEN_64BITS * pstXC_DS_CmdCnt->u16CMDCNT_IPM;
            u32Addr = u32Addr + u32MemShift_IPM;
            pu64Data = (MS_U64*)MsOS_PA2KSEG1(u32Addr);//IPM

            if(pstXC_DS_CmdCnt->u16CMDCNT_IPM < u8MaxCmdCnt)
            {
                *pu64Data = u64CmdTemp;
                pstXC_DS_CmdCnt->u16CMDCNT_IPM++;
            }
            else
            {
                printf("[%s,%5d] [DS] Command count overflow !!  u16CMDCNT_IPM:%d  Max command count: %d\n",__func__,__LINE__,pstXC_DS_CmdCnt->u16CMDCNT_IPM,DMS_DS_DEPTH * 2);
            }
        }
    }
    else
    {
        printf("[%s,%5d] [DS] Only can support XC/MVOP/GOP NOW !! \n",__func__,__LINE__);
    }

}

void Hal_DMS_Add_NullCommand(void* pInstance, SCALER_WIN eWindow, DMS_ds_reg_ip_op_sel IPOP_Sel, DMS_DS_CMDCNT *pstXC_DS_CmdCnt)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

    MS_U16 u16MaxCmdCnt = DMS_DS_DEPTH * 2; // It has two 64bit commands per MIU bus width
    MS_U64 *pu64Data = NULL;
    MS_U16 i;
#if ALL_IP_COMMAND
    u16MaxCmdCnt *= 2;
#endif

    if(eWindow == MAIN_WINDOW)
    {
        if(IPOP_Sel == DS_IP)
        {
            for(i=pstXC_DS_CmdCnt->u16CMDCNT_IPM;i<u16MaxCmdCnt;i++)
            {
                MS_PHY u32IndexBase= (pDMSResPri->stHalDMS.phyDSAddr +
                    (MS_U32)pDMSResPri->stHalDMS.u8DSIndex * u16MaxCmdCnt * BYTE_PER_WORD +
                    (i / 2) * BYTE_PER_WORD + (i % 2) * DS_CMD_LEN_64BITS);
                pu64Data  = (MS_U64*)MsOS_PA2KSEG1(u32IndexBase);
                *pu64Data = 0x00000009FFFF0000;
            }
        }
#if (ALL_IP_COMMAND == 0)
        else
        {
            for(i=pstXC_DS_CmdCnt->u16CMDCNT_OPM;i<u16MaxCmdCnt;i++)
            {
                MS_PHY u32IndexBase= (pDMSResPri->stHalDMS.phyDSAddr + OP_BASE_OFFSET +
                    (MS_U32)pDMSResPri->stHalDMS.u8DSIndex * u16MaxCmdCnt * BYTE_PER_WORD +
                    (i / 2) * BYTE_PER_WORD + (i % 2) * DS_CMD_LEN_64BITS);
                pu64Data  = (MS_U64*)MsOS_PA2KSEG1(u32IndexBase);
                *pu64Data = 0x00000009FFFF0000;
            }
        }
#endif
    }
}

MS_BOOL HAL_DMS_XC_DSSetWin(void* pInstance, MS_WINDOW_TYPE* stMVOPCropWindow, const XC_SETWIN_INFO* stXCSetWinInfo, MS_U32 u32FlowFlag)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

    MS_U32 u32HRatio = 0;
    MS_U32 u32VRatio = 0;
    //printf("[%s] Post: %d -> %d, %d -> %d, flow %d\n",__FUNCTION__,stXCSetWinInfo->u16HCusScalingSrc,stXCSetWinInfo->u16HCusScalingDst,
    //    stXCSetWinInfo->u16VCusScalingSrc,stXCSetWinInfo->u16VCusScalingDst,u32FlowFlag);

    if(stXCSetWinInfo->u16HCusScalingSrc != stXCSetWinInfo->u16HCusScalingDst)
    {
        u32HRatio = H_PostScalingRatio(stXCSetWinInfo->u16HCusScalingSrc, stXCSetWinInfo->u16HCusScalingDst);
        u32HRatio &= 0x0FFFFFF;
        u32HRatio |= 0x1000000;
    }

    if(stXCSetWinInfo->u16VCusScalingSrc != stXCSetWinInfo->u16VCusScalingDst)
    {
        u32VRatio = V_PostScalingRatio(stXCSetWinInfo->u16VCusScalingSrc, stXCSetWinInfo->u16VCusScalingDst);
        u32VRatio &= 0x00FFFFFF;
        u32VRatio |= 0x81000000;
    }

    pDMSResPri->stHalDMS.stDMS_DSSWDB.u32FlowFlag = u32FlowFlag;
    pDMSResPri->stHalDMS.stDMS_DSSWDB.u32HPostRatio = u32HRatio;
    pDMSResPri->stHalDMS.stDMS_DSSWDB.u32VPostRatio = u32VRatio;

    if(1)//u32FlowFlag == 1)
    {
        //from DIP need display window
        pDMSResPri->stHalDMS.stDMS_DSSWDB.u16H_Start = g_IPanel.HStart() + stXCSetWinInfo->stDispWin.x;
        pDMSResPri->stHalDMS.stDMS_DSSWDB.u16H_End   = pDMSResPri->stHalDMS.stDMS_DSSWDB.u16H_Start + stXCSetWinInfo->stDispWin.width - 1;
        pDMSResPri->stHalDMS.stDMS_DSSWDB.u16V_Start = stXCSetWinInfo->stDispWin.y;
        pDMSResPri->stHalDMS.stDMS_DSSWDB.u16V_End   = pDMSResPri->stHalDMS.stDMS_DSSWDB.u16V_Start + stXCSetWinInfo->stDispWin.height - 1;
    }

    pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.x = stMVOPCropWindow->x;
    pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.y = stMVOPCropWindow->y;
    pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.width = stMVOPCropWindow->width;
    pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.height = stMVOPCropWindow->height;
    //printf("[%s] MVOP: %d %d, %d %d\n",__FUNCTION__,gDMS_DSSWDB.stMVOPCropWindow.x,gDMS_DSSWDB.stMVOPCropWindow.y,gDMS_DSSWDB.stMVOPCropWindow.width,gDMS_DSSWDB.stMVOPCropWindow.height);
    //printf("[%s] Disp: %d %d, %d %d\n",__FUNCTION__,gDMS_DSSWDB.u16H_Start,gDMS_DSSWDB.u16H_End,gDMS_DSSWDB.u16V_Start,gDMS_DSSWDB.u16V_End);
    //printf("[%s] H:0x%x , V:0x%x\n",__FUNCTION__,gDMS_DSSWDB.u32HPostRatio,gDMS_DSSWDB.u32VPostRatio);

    return TRUE;
}

MS_BOOL HAL_DMS_XC_DSSetMVOPPitch(void* pInstance, MS_U16 u16Pitch)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

    pDMSResPri->stHalDMS.stDMS_DSSWDB.u16MVOP_Pitch = u16Pitch;
    printf("[%s] Pitch: %d\n",__FUNCTION__,pDMSResPri->stHalDMS.stDMS_DSSWDB.u16MVOP_Pitch);
    return TRUE;
}

MS_BOOL HAL_DMS_XC_DSSetMVOPAddr(void* pInstance, MS_PHY phyLumaAddr, MS_PHY phyChromaAddr, MS_BOOL b422Pack)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

    pDMSResPri->stHalDMS.stDMS_DSSWDB.bMVOP_422Pack = b422Pack;
    pDMSResPri->stHalDMS.stDMS_DSSWDB.phyMVOP_LumaAddr = phyLumaAddr;
    pDMSResPri->stHalDMS.stDMS_DSSWDB.phyMVOP_ChromaAddr = phyChromaAddr;
    return TRUE;
}

#define MI_FLOW_PATCH 1
extern MS_U16 gDMSMVOPForceLookVDEC;
MS_BOOL HAL_DMS_XC_DSFire(void* pInstance)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

#if (ALL_IP_COMMAND == 0)
    if(MApi_XC_R2BYTEMSK(0x0006,0x1000))
    {
        MApi_XC_W2BYTEMSK(0x0006,0,0x1000);
    }
#endif

#if MI_FLOW_PATCH
//This Patch for FBL W/R diff need to be 0
    if(MApi_XC_R2BYTEMSK(0x120E,0xF000))
    {
        MApi_XC_W2BYTEMSK(0x120E,0,0xF000);
    }
//This Patch for FBL W/R must disable
    if(MApi_XC_R2BYTEMSK(0x120E,0x0003) != 3)
    {
        MApi_XC_W2BYTEMSK(0x120E,0x0003,0x0003);
    }
//This Patch for FBL DataFormat can not set
    if(MApi_XC_R2BYTEMSK(0x1202,0x2F00))
    {
        MApi_XC_W2BYTEMSK(0x1202,0,0x2F00);
    }
//This Patch for FBL do not need miu write size protect
    if(MApi_XC_R2BYTEMSK(0x1230,0x8000))
    {
        MApi_XC_W2BYTEMSK(0x1230,0,0x8000);
    }

#endif

    DMS_DS_CMDCNT STXC_DS_CMDCNT;
    STXC_DS_CMDCNT.u16CMDCNT_IPM =0;
    STXC_DS_CMDCNT.u16CMDCNT_IPS =0;
    STXC_DS_CMDCNT.u16CMDCNT_OPM =0;
    STXC_DS_CMDCNT.u16CMDCNT_OPS =0;
    //printf("[%s] MVOP: %d %d, %d %d\n",__FUNCTION__,gDMS_DSSWDB.stMVOPCropWindow.x,gDMS_DSSWDB.stMVOPCropWindow.y,gDMS_DSSWDB.stMVOPCropWindow.width,gDMS_DSSWDB.stMVOPCropWindow.height);
    //printf("[%s] Disp: %d %d, %d %d\n",__FUNCTION__,gDMS_DSSWDB.u16H_Start,gDMS_DSSWDB.u16H_End,gDMS_DSSWDB.u16V_Start,gDMS_DSSWDB.u16V_End);
    //printf("[%s] Pitch: %d\n",__FUNCTION__,gDMS_DSSWDB.u16MVOP_Pitch);
    //printf("[%s] H:0x%x , V:0x%x\n",__FUNCTION__,gDMS_DSSWDB.u32HPostRatio,gDMS_DSSWDB.u32VPostRatio);

#if ALL_IP_COMMAND
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1010, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16H_Start, DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1012, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16H_End, DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1014, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16V_Start, DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1016, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16V_End, DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);

    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x230E, pDMSResPri->stHalDMS.stDMS_DSSWDB.u32HPostRatio & 0xFFFF          , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x2310, (pDMSResPri->stHalDMS.stDMS_DSSWDB.u32HPostRatio >> 16) & 0xFFFF  , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x2312, pDMSResPri->stHalDMS.stDMS_DSSWDB.u32VPostRatio & 0xFFFF          , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x2314, (pDMSResPri->stHalDMS.stDMS_DSSWDB.u32VPostRatio >> 16) & 0xFFFF  , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);

    //Disable pre scaling for pre scaling be set out of DMS
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x0210, 0 , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x0212, 0 , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x0208, 0 , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x020A, 0 , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);

    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x202A, pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.height & 0x0FFF  , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);

#else
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1010, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16H_Start, DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1012, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16H_End, DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1014, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16V_Start, DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1016, pDMSResPri->stHalDMS.stDMS_DSSWDB.u16V_End, DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);

    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x230E, pDMSResPri->stHalDMS.stDMS_DSSWDB.u32HPostRatio & 0xFFFF          , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x2310, (pDMSResPri->stHalDMS.stDMS_DSSWDB.u32HPostRatio >> 16) & 0xFFFF  , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x2312, pDMSResPri->stHalDMS.stDMS_DSSWDB.u32VPostRatio & 0xFFFF          , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x2314, (pDMSResPri->stHalDMS.stDMS_DSSWDB.u32VPostRatio >> 16) & 0xFFFF  , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);

    //Disable pre scaling for pre scaling be set out of DMS
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x0210, 0 , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x0212, 0 , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x0208, 0 , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x020A, 0 , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);

    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x202A, pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.height & 0x0FFF  , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
#if MI_FLOW_PATCH
//This Patch for keep ipm/opm fetch to max size
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x121E, 0x1000 , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x122E, 0x1000 , DS_OP, DMS_DS_XC, &STXC_DS_CMDCNT);
#endif

#endif

    MS_U16 u16MVOPForceWrite = MApi_XC_ReadByte(0x10144E);
    u16MVOPForceWrite |= 0x10;
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101427, u16MVOPForceWrite    , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101440, pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.x  , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    if(pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.y == 0)
    {
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101441, (pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.y | 0xE000), DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    }
    else
    {
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101441, (pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.y | 0xA000), DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    }
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101442, pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.width    , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101443, pDMSResPri->stHalDMS.stDMS_DSSWDB.stMVOPCropWindow.height   , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);

    if(1)//gDMS_DSSWDB.u32FlowFlag == 1)
    {
        MS_PHY phyAddr = pDMSResPri->stHalDMS.stDMS_DSSWDB.phyMVOP_LumaAddr >> 3;
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101421, phyAddr & 0xFFFF           , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101422, (phyAddr >> 16) & 0x1FFF  , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
        if(pDMSResPri->stHalDMS.stDMS_DSSWDB.bMVOP_422Pack)
        {
            phyAddr = pDMSResPri->stHalDMS.stDMS_DSSWDB.phyMVOP_LumaAddr + 16;
        }
        else
        {
            phyAddr = pDMSResPri->stHalDMS.stDMS_DSSWDB.phyMVOP_ChromaAddr;
        }
        phyAddr = phyAddr >> 3;
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101423, phyAddr & 0xFFFF           , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101424, (phyAddr >> 16) & 0x1FFF  , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    }

//Pitch & Tile
    MS_U8 u8MVOPCtrl0_H = MApi_XC_ReadByte(0x101441);
    MS_U8 u8MVOPCtrl0_L = MApi_XC_ReadByte(0x101440);
    MS_U8 u8MVOPPitch_L = MApi_XC_ReadByte(0x101430);
    u8MVOPPitch_L = (u8MVOPPitch_L & (0xFF - 0x03)) | ((pDMSResPri->stHalDMS.stDMS_DSSWDB.u16MVOP_Pitch >> 8) & 0x03);
    u8MVOPCtrl0_H = (pDMSResPri->stHalDMS.stDMS_DSSWDB.u16MVOP_Pitch & 0xFF);
    if(pDMSResPri->stHalDMS.stDMS_DSSWDB.u32FlowFlag == 1)
    {
        //DIP
        u8MVOPCtrl0_L = (u8MVOPCtrl0_L | 0xB0);
    }
    else if(pDMSResPri->stHalDMS.stDMS_DSSWDB.u32FlowFlag == 2)
    {
        //VDEC
        u8MVOPCtrl0_L = (u8MVOPCtrl0_L & (0xFF - 0xB0));
    }

    MS_U16 u16MVOPCtrl0 = u8MVOPCtrl0_H;
    u16MVOPCtrl0 = ((u16MVOPCtrl0 << 8) | u8MVOPCtrl0_L);
    if((gDMSMVOPForceLookVDEC == 1))
    {
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101418, 0 , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101420, 0xF006 , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    }
    else
    {
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101418, u8MVOPPitch_L , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
        Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101420, u16MVOPCtrl0 , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);
    }

    u16MVOPForceWrite &= ~0x10;
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x101427, u16MVOPForceWrite    , DS_IP, DMS_DS_MVOP, &STXC_DS_CMDCNT);

    MS_U32 ActiveSWClearEn = 0;
    ActiveSWClearEn = MApi_XC_R2BYTE(0x1F2E);
    ActiveSWClearEn |= 0x0010;//u32SW_Reset_IPM;
    Hal_DMS_WriteSWDSCommand(pInstance, MAIN_WINDOW, 0x1F2E, ActiveSWClearEn  , DS_IP, DMS_DS_XC, &STXC_DS_CMDCNT);

    Hal_DMS_Add_NullCommand(pInstance, MAIN_WINDOW, DS_IP,&STXC_DS_CMDCNT);
    Hal_DMS_Add_NullCommand(pInstance, MAIN_WINDOW, DS_OP,&STXC_DS_CMDCNT);

#if ALL_IP_COMMAND
    if(STXC_DS_CMDCNT.u16CMDCNT_IPM > DMS_DS_DEPTH*4)
#else
    if(STXC_DS_CMDCNT.u16CMDCNT_IPM > DMS_DS_DEPTH*2)
#endif
    {
        printf("\033[1;31m[%s][%d] error, IPM DS Command conut %d > Max_size %d\033[0m\n",__FUNCTION__,__LINE__,STXC_DS_CMDCNT.u16CMDCNT_IPM,DMS_DS_DEPTH*2);
    }
    if(STXC_DS_CMDCNT.u16CMDCNT_OPM > DMS_DS_DEPTH*2)
    {
        printf("\033[1;31m[%s][%d] error, OPM DS Command conut %d > Max_size %d\033[0m\n",__FUNCTION__,__LINE__,STXC_DS_CMDCNT.u16CMDCNT_OPM,DMS_DS_DEPTH*2);
    }

    MApi_XC_Set_DSForceIndex(TRUE,pDMSResPri->stHalDMS.u8DSIndex,MAIN_WINDOW);
    if(pDMSResPri->stHalDMS.u8DSIndex == 0)
    {
        pDMSResPri->stHalDMS.u8DSIndex = 1;
    }
    else
    {
        pDMSResPri->stHalDMS.u8DSIndex = 0;
    }
    return TRUE;
}

MS_BOOL HAL_DMS_XC_GetCaps(void* pInstance, EN_DMS_CAPABILITY *peCapability)
{
    *peCapability |= E_DMS_CAPABILITY_MULTI_WINDOW;
    return TRUE;
}

#if(DMS_DONT_USE_CMA == 1)
#ifdef MSOS_TYPE_LINUX_KERNEL
#include "drvCMAPool_v2.h"
#else
#include "drvCMAPool.h"
#endif
#include "msos/linux/mdrv_cma_pool_st.h"
#endif

#define DMS_CMA_4MB 0x0400000
#define DMS_CMA_8MB 0x0800000
#define DMS_CMA_16MB 0x1000000
#define DMS_CMA_24MB 0x1800000
#define DMS_CMA_32MB 0x2000000
#define DMS_CMA_48MB 0x3000000

#if(DMS_DONT_USE_CMA == 1)
MS_BOOL HAL_DMS_Release_CMA(void* pInstance, EN_DMS_MEMORY_TYPE enMemType, MS_U32 u32CMAHeapID)
{
    return FALSE;
}

MS_BOOL HAL_DMS_Setup_CMA(void* pInstance, EN_DMS_MEMORY_TYPE enMemType, MS_U32 u32CMAHeapID)
{
    MS_U32 u32GetCMABufferSize = 0;
    switch(enMemType)
    {
        case E_DMS_MEMORY_TYPE_GOP_DISPLAY:
        {
#if (DMS_DIP_Support_HVSP == 0)
            u32GetCMABufferSize = DMS_CMA_48MB;
#else
            u32GetCMABufferSize = DMS_CMA_32MB;
#endif
            break;
        }
        case E_DMS_MEMORY_TYPE_FREEZE:
        {
            u32GetCMABufferSize = DMS_CMA_4MB;
            break;
        }
        case E_DMS_MEMORY_TYPE_MVOP_DISPLAY:
        {
            u32GetCMABufferSize = DMS_CMA_24MB;
            break;
        }
        default:
        {
            DMS_PRINTF("\033[1;31m[%s][%d] unuse mem type %d set CMA\033[0m\n",__FUNCTION__,__LINE__,enMemType);
            break;
        }
    }
    if(u32GetCMABufferSize == 0)
    {
        DMS_PRINTF("\033[1;31m[%s][%d] CMA buffer size = 0\033[0m\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    struct CMA_Pool_Alloc_Param CMA_Pool_GetMem;
    CMA_Pool_GetMem.pool_handle_id = u32CMAHeapID;
    CMA_Pool_GetMem.offset_in_pool = 0;
    CMA_Pool_GetMem.flags = CMA_FLAG_VIRT_ADDR;
    CMA_Pool_GetMem.length = u32GetCMABufferSize;

    if (MApi_CMA_Pool_GetMem(&CMA_Pool_GetMem) == FALSE)
    {
        DMS_PRINTF("\033[1;31m Function = %s, Line = %d, MApi_CMA_Pool_GetMem() fail!!! (Type %d, HeapID %d, Size 0x%x)\033[m\n", __PRETTY_FUNCTION__, __LINE__,
            enMemType,u32CMAHeapID,u32GetCMABufferSize);
        return FALSE;
    }

    return TRUE;
}
#endif

