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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/string.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <stddef.h>   /* for size_t */
#include "MsCommon.h"
#include "drvMMIO.h"
#include "gif_lib.h"
#include "apiGPD.h"
#include "drvgpd.h"
#include "gpd.h"
#include "drvMIU.h"
#include "halCHIP.h"
#ifdef CONFIG_MSTAR_CLKM
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/clkm.h>
#else
#include "drvCLKM.h"
#endif
#endif

#include "utopia.h"
#include "apiGPD_priv.h"
#include "apiGPD_v2.h"
void* pInstantGPD = NULL;

gpd_debug_level eEnableDbgMsg = E_GPD_DEBUG_LEVEL_NONE;

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------
#if (defined CONFIG_MLOG)
#include "ULog.h"

#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ULOGE("API_GPD", format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ULOGI("API_GPD", format, ##args);}while(0)
#else

#ifdef ANDROID
#ifndef LOGD
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ALOGE(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ALOGI(format, ##args);}while(0)
#else
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) LOGE(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) LOGI(format, ##args);}while(0)
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printk(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printk(format, ##args);}while(0)
#else
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printf(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printf(format, ##args);}while(0)
#endif
#endif

#endif

#define LOG_DATA_TO_USB         FALSE

#ifdef GPD_UTOPIA_2K
    #define GPD_DRIVER_BASE KERNEL_MODE
#else
    #define GPD_DRIVER_BASE 0
#endif

//#define VERIFY


#ifndef VERIFY
#define CBUFFER_SIZE         (1024UL*1024UL)
#define ZBUFFER_SIZE         (1024UL*1024UL)

#define GPD_XIU_BASE  (0x13F00UL * 2UL)
#define GPD_RIU_BASE  (0x13E00UL * 2UL)


#define MIU_CLI_BASE  (0x01200UL * 2UL)
#define MIU_SEL_BASE  (0x00600UL * 2UL)

#define STB_GPD_XIU_BASE  (0x10F00UL * 2UL)
#define STB_GPD_RIU_BASE  (0x10E00UL * 2UL)

#else

#define CBUFFER_SIZE         (1024UL*1024UL)
#define ZBUFFER_SIZE         (1024UL*1024UL)
#define STREAMBUFFER_SIZE  (4UL*1024UL*1024UL) //4MB
#define FRAMEBUFFER_SIZE  (22UL*1024UL*1024UL) //22MB


#define CBUFFER            (DRAMBASE + 0x11000000UL)
#define ZBUFFER            (CBUFFER + CBUFFER_SIZE)
#define STREAMBUFFER       (ZBUFFER + ZBUFFER_SIZE)
#define FRAMEBUFFER        (STREAMBUFFER + STREAMBUFFER_SIZE)
#define ARGBBUFFER         (FRAMEBUFFER + FRAMEBUFFER_SIZE)

#if (defined(__arm__))

#define HK_REG_BASE   0xFD200000UL

#else

#define HK_REG_BASE   0xBF200000UL

#endif

#define GPD_XIU_BASE  (HK_REG_BASE + 0x13F00UL * 2UL)
#define GPD_RIU_BASE  (HK_REG_BASE + 0x13E00UL * 2UL)

#if 0

#define CLK_GEN_BASE  (HK_REG_BASE + 0x00B00 * 2)

#else

#define CLK_GEN_BASE  (HK_REG_BASE + 0x03300UL * 2UL)

#endif

#define MIU_CLI_BASE  (HK_REG_BASE + 0x01200UL * 2UL)


#endif

#define HW_MAX_SUPPORTED_WIDTH          8192UL
#define HW_MAX_SUPPORTED_HEIGHT         8192UL

//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------
//#define MMIO8(addr) (*((volatile MS_U8*)(addr)))
#define _GPD_SHM_POINTER_CHECK()   \
    do\
    {\
        if (pGPDContext == NULL)\
        {\
            return;\
        }\
    }while(0)

#define _GPD_SHM_POINTER_CHECK_RETURN(x)   \
    do\
    {\
        if (pGPDContext == NULL)\
        {\
            return x;\
        }\
    }while(0)

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
MS_U32 g_gpd_set_access_region = FALSE;
MS_VIRT g_gpd_clock;

typedef struct
{
    gpd_addr_info GpdAddrInfo;
    gpd_pic_info Param;
    MS_U32 MIUAllocation;
    MS_U32 RIU_REG_BASE;
    MS_U8 bIsGPDInit;
} GPD_EX_CTX;

GPD_EX_CTX* pGPDContext = NULL;
GPD_EX_CTX gGPDContext;

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
#include "utopia_dapi.h"

typedef struct
{
MS_VIRT _u32ReadBufferAddr;
MS_PHY _u32ReadBufferAddr_PA;
MS_U32 _u32ReadBufferSize;
#ifndef MSOS_TYPE_LINUX_KERNEL
MS_VIRT _u32WriteBufferAddr;
#endif
MS_PHY _u32WriteBufferAddr_PA;
MS_U32 _u32WriteBufferSize;
MS_VIRT _u32InternalBufferAddr;
MS_U32 _u32InternalBufferSize;
MS_PHY _u32InternalBufferAddr_PA;

MS_U16 _u16Image_x_size;
MS_U16 _u16Image_y_size;

MS_U32 ocolor;
MS_BOOL bPNG;

MS_U32 _u32HWStartTime;
MS_U32 _u32HWEndTime;

}GPD_MDebug;

GPD_MDebug gGPDMDebug;

#endif

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------


//extern void *memset(void *ptr, MS_U32 value, size_t num);
extern void mdrv_gpd_setbuf(gpd_addr_info* addr_info);
extern MS_S32 mdrv_gpd_init(gpd_addr_info* param);
extern MS_S32 mdrv_gpd_decode(gpd_pic_info* pic_info, MS_U32 Ocolor, MS_U32 mode,gpd_roi_info* roi);
extern void mdrv_gpd_scaling_init(MS_U32 enable, MS_U32 mode);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
extern MS_BOOL mdrv_gpd_bPNG(void);
#endif

MS_U32 MApi_GPD_Input_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size);

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)
static MS_U32 u32FileIndex=0;
#endif

#if LOG_DATA_TO_USB
void GPD_WRITE_INPUT_2_USB(MS_U32 u32addr, MS_U32 u32Size, MS_U32 u32Index)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','I','n', 'p', 'u', 't','_','0', '0', '0', '.', 'p', 'n', 'g',0};
    MS_U8 u8FilenameIndex = 10;

    fileName2[u8FilenameIndex+6]=(u32Index/100)%10+'0';
    fileName2[u8FilenameIndex+7]=(u32Index/10)%10+'0';
    fileName2[u8FilenameIndex+8]=u32Index%10+'0';

    FILE *inBinfp;
    if((inBinfp = fopen((char *)fileName2, "wb"))==NULL)
    {
        GPD_API_ERROR("Can't open input png file\n");
    }
    else
    {
        GPD_API_ERROR("open input png file success\n");
    }

    fwrite((void *)u32addr, 1, u32Size, inBinfp);
    fclose(inBinfp);

}

void GPD_WRITE_OUTPUT_2_USB(MS_U32 u32addr, MS_U32 u32Size, MS_U32 u32Width, MS_U32 u32Height, MS_U32 u32Index)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','0','0', '0', '0', 'x','0','0', '0', '0', '_','0', '0', '0', '.', 'y', 'u', 'y', 'v',0};
    MS_U8 u8FilenameIndex = 10;
    fileName2[u8FilenameIndex]=u32Width/1000+'0';
    fileName2[u8FilenameIndex+1]=(u32Width/100)%10+'0';
    fileName2[u8FilenameIndex+2]=(u32Width/10)%10+'0';
    fileName2[u8FilenameIndex+3]=u32Width%10+'0';
    fileName2[u8FilenameIndex+5]=u32Height/1000+'0';
    fileName2[u8FilenameIndex+6]=(u32Height/100)%10+'0';
    fileName2[u8FilenameIndex+7]=(u32Height/10)%10+'0';
    fileName2[u8FilenameIndex+8]=u32Height%10+'0';

    fileName2[u8FilenameIndex+10]=(u32Index/100)%10+'0';
    fileName2[u8FilenameIndex+11]=(u32Index/10)%10+'0';
    fileName2[u8FilenameIndex+12]=u32Index%10+'0';


    FILE *outBinfp;
    if((outBinfp = fopen((char *)fileName2, "wb"))==NULL)
    {
        GPD_API_ERROR("Can't open output yuyv file\n");
    }
    else
    {
        GPD_API_ERROR("open output yuyv file success\n");
    }

    fwrite((void *)u32addr, 1, u32Size, outBinfp);
    fclose(outBinfp);

}
#endif

static void _GPD_Context_Init(void)
{




}


static MS_BOOL _GPD_Init_Share_Mem(void)
{
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL) || defined(MSOS_TYPE_ECOS)
    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;

    if (FALSE == MsOS_SHM_GetId( (MS_U8*)"Linux GPD driver",
                                          sizeof(GPD_EX_CTX),
                                          &u32ShmId,
                                          &u32Addr,
                                          &u32BufSize,
                                          MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux GPD driver",
                                             sizeof(GPD_EX_CTX),
                                             &u32ShmId,
                                             &u32Addr,
                                             &u32BufSize,
                                             MSOS_SHM_CREATE))
        {
            if(pGPDContext == NULL)
            {
                pGPDContext = &gGPDContext;
                memset(pGPDContext,0,sizeof(GPD_EX_CTX));
                _GPD_Context_Init();
            }
            else
            {
                GPD_API_INFO("[%s]Global structure exists!!!\n",__FUNCTION__);
            }
            return FALSE;
        }
        else
        {
            memset((MS_U8*)u32Addr, 0, sizeof(GPD_EX_CTX));
            pGPDContext = (GPD_EX_CTX*)u32Addr; // for one process
            _GPD_Context_Init();
        }
    }
    else
    {
        pGPDContext = (GPD_EX_CTX*)u32Addr; // for another process
    }


#else
    if(pGPDContext == NULL)
    {
        pGPDContext = &gGPDContext;
        memset(pGPDContext,0,sizeof(GPD_EX_CTX));
        _GPD_Context_Init();
    }
#endif

    return TRUE;
}

static MS_U32 _GPD_SetCacheable(MS_U32 param)
{
    MS_U32 cacheable;
    cacheable = (param == TRUE) ? TRUE: FALSE;
    drv_gpd_set_cacheable(cacheable);

    return TRUE;
}

static MS_U32 _GPD_SetAccessRegion(gpd_access_region *param)
{
    MS_VIRT BankBaseAddr;
    MS_PHY BankSize;
    MS_U32 u32GPDRIUBase, u32GPDXIUBase;

    MDrv_GPD_Reg_Base(&u32GPDRIUBase, &u32GPDXIUBase);

    if(FALSE == MDrv_MMIO_GetBASE((MS_VIRT*)&BankBaseAddr, &BankSize, MS_MODULE_GPD))
    {
        GPD_API_ERROR("MDrv_MMIO_GetBASE FAILED !! \n");
        return FALSE;
    }

    pGPDContext->GpdAddrInfo.RegBaseAddr = BankBaseAddr + (MS_VIRT)u32GPDRIUBase;
    mdrv_gpd_setbuf(&pGPDContext->GpdAddrInfo);
    drv_gpd_set_access_region(param->u32PA_StartAddr >> WORDALIGN, param->u32PA_EndAddr >> WORDALIGN);
    g_gpd_set_access_region = TRUE;

    GPD_API_INFO("Set g_gpd_set_access_region to TRUE!!!!\n");
    return TRUE;

}

static MS_U32 _GPD_GetPitch(gpd_get_pitch_info *para)
{
    if(para == NULL)
    {
        GPD_API_ERROR("_GPD_GetPitch FAILED !! para error\n");
        return FALSE;
    }
    if(para->ocolor > GRAY_ALPHA)
    {
        GPD_API_ERROR("_GPD_GetPitch FAILED !! ocolor error\n");
        return FALSE;
    }
    if(para->u32Width == 0)
    {
        GPD_API_ERROR("_GPD_GetPitch FAILED !! u32Width error\n");
        return FALSE;
    }
    switch(para->ocolor)
    {
        case ARGB8888:
            para->u32Pitch = (((para->u32Width) + 1) & ~1);     // 2-align
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            para->u32Pitch = (((para->u32Width) + 3) & ~3);     // 4-align
            break;
        case GRAY:
            para->u32Pitch = (((para->u32Width) + 7) & ~7);     // 8-align
            break;
    }
    return TRUE;

}


#ifndef VERIFY
void verGPD_ISR_CB(void)
{
    GPD_API_ERROR("verGPD_ISR_CB()!!!!!!!!!\n");
}


void MApi_GPD_EnableISR(GPD_IsrFuncCb IsrCb)
{
    if(TRUE == MDrv_GPD_EnableISR((GPD_IsrFuncCb)IsrCb))
    {
    }
    else
    {
        GPD_API_ERROR("%s : Fail!!!!\n", __FUNCTION__);
    }
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Disable GPD ISR and unregister callback function.
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
void MApi_GPD_DisableISR(void)
{
    if(TRUE == MDrv_GPD_DisableISR())
    {
    }
    else
    {
        GPD_API_ERROR("%s : Fail!!!!\n", __FUNCTION__);
    }
    return;
}


void *_MApi_GPD_Init(MS_PHY SrcBuf)
{
    MS_VIRT BankBaseAddr;
    MS_PHY BankSize;
    MS_U32 GPD_CLK_BASE, GPD_CLK_OFFSET;
    MS_U32 u32GPDRIUBase, u32GPDXIUBase;

    _GPD_Init_Share_Mem();
    _GPD_SHM_POINTER_CHECK_RETURN(FALSE);

    if(pGPDContext->bIsGPDInit == TRUE)
    {
        GPD_API_ERROR("GPD is in used !! \n");
        return FALSE;
    }
    else
    {
        pGPDContext->bIsGPDInit = TRUE;
    }

    GPD_API_INFO("MApi_GPD_Init\n");
    GPD_API_INFO("GPD version:%x\n", GPD_VERSION);

    if(FALSE == MDrv_MMIO_GetBASE(&BankBaseAddr, &BankSize, MS_MODULE_GPD))
    {
        GPD_API_ERROR("MDrv_MMIO_GetBASE FAILED !! \n");
        pGPDContext->bIsGPDInit = FALSE;
        return FALSE;
    }

    pGPDContext->RIU_REG_BASE = BankBaseAddr;
    GPD_API_INFO("[%s] [%d] BankBaseAddr = %tx, BankSize = %tx\n", __FUNCTION__, __LINE__, (ptrdiff_t)BankBaseAddr, (ptrdiff_t)BankSize);

    //enable gpd clck

    GPD_CLK_BASE = drv_gpd_get_clkbase();

    GPD_CLK_OFFSET = drv_gpd_get_clkoffset();

    GPD_API_INFO("GPD_CLK_BASE=%tx, GPD_CLK_OFFSET=%tx\n", (ptrdiff_t)GPD_CLK_BASE, (ptrdiff_t)GPD_CLK_OFFSET);

    drv_gpd_init_outside_reg(BankBaseAddr);

    if (TRUE == g_gpd_set_access_region)
    {
        g_gpd_clock = (BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET);
        GPD_API_INFO("Set g_gpd_clock to %tx!!!!(%tx %tx %tx)\n", (ptrdiff_t)g_gpd_clock,
            (ptrdiff_t)BankBaseAddr, (ptrdiff_t)GPD_CLK_BASE, (ptrdiff_t)GPD_CLK_OFFSET);
    }
    else
    {
#ifdef CONFIG_MSTAR_CLKM
        drv_gpd_power_on();
#else
#if 1
        GPD_API_INFO("@@@set gpd_clk to 216MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0;   // 0x28 << 2
#elif 0
        GPD_API_INFO("@@@set gpd_clk to 192MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0x4;
#else
        GPD_API_INFO("@@@set gpd_clk to 144MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0xc;
#endif
#endif

    }
    MDrv_GPD_Reg_Base(&u32GPDRIUBase, &u32GPDXIUBase);

#if 0
    MS_U32 u32Offset = 0;
    _phy_to_miu_offset(pGPDContext->MIUAllocation, u32Offset, SrcBuf);
    UNUSED(u32Offset);
    switch(pGPDContext->MIUAllocation)
    {
        case E_CHIP_MIU_0:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU0);
            break;
        case E_CHIP_MIU_1:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU1);
            break;
        case E_CHIP_MIU_2:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU2);
            break;
        case E_CHIP_MIU_3:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU3);
            break;
        default:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU0);
            break;
    }
#else
    MS_U32 tmp = 0;
    MS_U8 u8MIU_Offset;
    MS_U16 u16MIU_Mask;
    MDrv_GPD_MIU_Client(&u8MIU_Offset, &u16MIU_Mask);

    //check miu sel is controlled by HW or MIU
    //GPD MIU select control is
    tmp = *((volatile MS_U32 *)(BankBaseAddr + MIU_SEL_BASE + (((MS_U32)u8MIU_Offset)<< 1)));
    tmp &= (~u16MIU_Mask);
    *((volatile MS_U32 *)(BankBaseAddr + MIU_SEL_BASE + (((MS_U32)u8MIU_Offset) << 1))) = tmp;

    tmp = *((volatile MS_U32 *)(BankBaseAddr + MIU_CLI_BASE + (((MS_U32)u8MIU_Offset) << 1)));

    GPD_API_INFO("GPD MIU value = %tx\n", (ptrdiff_t)tmp);
    GPD_API_INFO("SrcBuf :0x%tx\n", (ptrdiff_t)SrcBuf);

    pGPDContext->MIUAllocation = MDrv_GPD_MIU_Select(SrcBuf);
    if(pGPDContext->MIUAllocation)
    {
        tmp |= (u16MIU_Mask);
        GPD_API_INFO("Select MIU1\n");
    }
    else
    {
        tmp &= (~u16MIU_Mask);
        GPD_API_INFO("Select MIU0\n");
    }

    *((volatile MS_U32 *)(BankBaseAddr + MIU_CLI_BASE +(((MS_U32)u8MIU_Offset) << 1))) = tmp;
#endif

    pGPDContext->GpdAddrInfo.RegBaseAddr = BankBaseAddr + (MS_VIRT)u32GPDRIUBase;
    pGPDContext->GpdAddrInfo.ReadBitBaseAddr = BankBaseAddr + (MS_VIRT)u32GPDXIUBase;

    pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr = SrcBuf;

    if (FALSE == drv_gpd_get_cacheable())
    {
        pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr);
        pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr = pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr + 512UL * 1024UL;
        pGPDContext->GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr);
    }
    else
    {
        pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG0(pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr);
        pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr = pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr + 512UL * 1024UL;
        pGPDContext->GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG0(pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr);
    }


    if(pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_Init with SrcBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)SrcBuf);
        pGPDContext->bIsGPDInit = FALSE;
        return FALSE;
    }


    mdrv_gpd_init(&pGPDContext->GpdAddrInfo);
#if 0
    MApi_GPD_DisableISR();
    MApi_GPD_EnableISR((GPD_IsrFuncCb)verGPD_ISR_CB);
    MDrv_GPD_SetISRMask();
#endif

    return &(pGPDContext->GpdAddrInfo);

}

MS_S32 _MApi_GPD_InputSource(gpd_pic_info* param, MS_PHY InputBuf, MS_U32 Size)
{
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;
    _GPD_SHM_POINTER_CHECK_RETURN(FALSE);

    _phy_to_miu_offset(u8MiuSel, u32Offset, InputBuf);
    UNUSED(u32Offset);
    if(pGPDContext->MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }

    if(pGPDContext->bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }
    GPD_API_INFO("MApi_GPD_ParseHeader\n");
    pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr = InputBuf;

    GPD_API_INFO("InputBuf=%tx\n", (ptrdiff_t)InputBuf);

    if (FALSE == drv_gpd_get_cacheable())
    {
        pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr);
        GPD_API_INFO("MsOS_PA2KSEG1(InputBuf)=%tx\n", (ptrdiff_t)MsOS_PA2KSEG1(InputBuf));
    }
    else
    {
        pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG0(pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr);
        GPD_API_INFO("MsOS_PA2KSEG0(InputBuf)=%tx\n", (ptrdiff_t)MsOS_PA2KSEG0(InputBuf));
    }

    if(pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_InputSource with InputBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)InputBuf);
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }

    pGPDContext->GpdAddrInfo.u32ReadBufferSize = Size;

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)
    u32FileIndex++;
#if LOG_DATA_TO_USB
    GPD_WRITE_INPUT_2_USB((MS_U32)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr, (MS_U32)Size, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)(MS_VIRT)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr,
        pGPDContext->GpdAddrInfo.u32ReadBufferSize));
#endif
#endif


    mdrv_gpd_setbuf(&(pGPDContext->GpdAddrInfo));
    mdrv_gpd_decode(param,0,1,NULL);
    if(param->u32Height > HW_MAX_SUPPORTED_HEIGHT || param->u32Width >HW_MAX_SUPPORTED_WIDTH)
    {
        param->enGPDStatus = -2;
    }
    memcpy((void*)(&(pGPDContext->Param)),(void*)(param),sizeof(gpd_pic_info));
    return param->enGPDStatus;

}

#if ENABLE_GPD_MESSAGE
static void MApi_GPD_PrintMem(MS_VIRT u32Addr, MS_U32 u32Size)
{
    MS_U32 u32i;
    GPD_API_INFO("===========================================================\n");
    GPD_API_INFO("print memory addr=0x%tx, size=0x%tx\n", (ptrdiff_t)u32Addr, (ptrdiff_t)u32Size);
    GPD_API_INFO("===========================================================\n");

#if 0
    for(u32i=0; u32i<u32Size/8+((u32Size%8)? 1:0); u32i++)
    {
        GPD_API_INFO("%02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*8)), *((MS_U8*)(u32Addr+u32i*8+1)), *((MS_U8*)(u32Addr+u32i*8+2)), *((MS_U8*)(u32Addr+u32i*8+3)),
        *((MS_U8*)(u32Addr+u32i*8+4)), *((MS_U8*)(u32Addr+u32i*8+5)), *((MS_U8*)(u32Addr+u32i*8+6)), *((MS_U8*)(u32Addr+u32i*8+7))
        );
    }
#else
    for(u32i=0; u32i<u32Size/16+((u32Size%16)? 1:0); u32i++)
    {
        GPD_API_INFO("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*16)), *((MS_U8*)(u32Addr+u32i*16+1)), *((MS_U8*)(u32Addr+u32i*16+2)), *((MS_U8*)(u32Addr+u32i*16+3)),
        *((MS_U8*)(u32Addr+u32i*16+4)), *((MS_U8*)(u32Addr+u32i*16+5)), *((MS_U8*)(u32Addr+u32i*16+6)), *((MS_U8*)(u32Addr+u32i*16+7)),
        *((MS_U8*)(u32Addr+u32i*16+8)), *((MS_U8*)(u32Addr+u32i*16+9)), *((MS_U8*)(u32Addr+u32i*16+10)), *((MS_U8*)(u32Addr+u32i*16+11)),
        *((MS_U8*)(u32Addr+u32i*16+12)), *((MS_U8*)(u32Addr+u32i*16+13)), *((MS_U8*)(u32Addr+u32i*16+14)), *((MS_U8*)(u32Addr+u32i*16+15))
        );
    }
#endif
    GPD_API_INFO("===========================================================\n");
}
#endif


MS_S32 _MApi_GPD_OutputDecode(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;

    _GPD_SHM_POINTER_CHECK_RETURN(FALSE);
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    UNUSED(u32Offset);
    if(pGPDContext->MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }

    if(pGPDContext->bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }
    gpd_roi_info roi;
    GPD_API_INFO("MApi_GPD_Decode\n");
    pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    if (FALSE == drv_gpd_get_cacheable())
    {
        pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    else
    {
        pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    pGPDContext->GpdAddrInfo.u32WriteBufferSize = Size;


    if(pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_OutputDecode with DecodeBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)DecodeBuf);
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }

    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&pGPDContext->GpdAddrInfo);

    drv_gpd_set_access_bound(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr,pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr + pGPDContext->GpdAddrInfo.u32WriteBufferSize - 1,
                                                    pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr, pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr +CBUFFER_SIZE+ZBUFFER_SIZE-1);

    GPD_API_INFO("GPDBaseAddr=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.RegBaseAddr);
    GPD_API_INFO("READBITBaseAddr=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.ReadBitBaseAddr);
    GPD_API_INFO("Read  buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr);
    GPD_API_INFO("Write buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Depth buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_DepthBufferAddr);
    GPD_API_INFO("Read  buf size=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32ReadBufferSize);
    GPD_API_INFO("Write buf size=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32WriteBufferSize);


#if ENABLE_GPD_MESSAGE
//    memset((void *)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr, 0x37, (size_t)pGPDContext->GpdAddrInfo.u32WriteBufferSize);
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
        gGPDMDebug._u32HWStartTime= MsOS_GetSystemTime();
#endif
    mdrv_gpd_decode(&pGPDContext->Param, ocolor, 0, &roi);


#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    gGPDMDebug._u32HWEndTime= MsOS_GetSystemTime();
    gGPDMDebug.bPNG = mdrv_gpd_bPNG();
    gGPDMDebug._u32ReadBufferAddr_PA = pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr;
    gGPDMDebug._u32WriteBufferAddr_PA= pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr;
    gGPDMDebug._u32InternalBufferAddr_PA= pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr;

    gGPDMDebug._u32ReadBufferAddr = pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr;
#ifndef MSOS_TYPE_LINUX_KERNEL
    gGPDMDebug._u32WriteBufferAddr= pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr;
#endif
    gGPDMDebug._u32InternalBufferAddr= pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr;

    gGPDMDebug._u32ReadBufferSize = pGPDContext->GpdAddrInfo.u32ReadBufferSize;
    gGPDMDebug._u32WriteBufferSize= pGPDContext->GpdAddrInfo.u32WriteBufferSize;
    gGPDMDebug._u32InternalBufferSize= 0x200000;

    gGPDMDebug._u16Image_x_size = (MS_U16)pGPDContext->Param.u32Width;
    gGPDMDebug._u16Image_y_size= (MS_U16)pGPDContext->Param.u32Height;

#endif

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)

    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(pGPDContext->Param.u32Height)*(pGPDContext->Param.u32Width)*u8BytesPerPixel, pGPDContext->Param.u32Width, pGPDContext->Param.u32Height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    GPD_API_INFO("[%s, %td, %tdx%td], CRC result=0x%tx\n", __FUNCTION__,  (ptrdiff_t)u32FileIndex, (ptrdiff_t)pGPDContext->Param.u32Width, (ptrdiff_t)pGPDContext->Param.u32Height,
        (ptrdiff_t)_MApi_GPD_GetCRCResult((MS_U8 *)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr, (pGPDContext->Param.u32Height)*(pGPDContext->Param.u32Width)*u8BytesPerPixel, pGPDContext->Param.u32Width, ocolor));
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr,
        pGPDContext->GpdAddrInfo.u32ReadBufferSize));

#endif
#endif

    drv_gpd_power_off();
    pGPDContext->bIsGPDInit = FALSE;
    return pGPDContext->Param.enGPDStatus;


}

MS_S32 _MApi_GPD_OutputDecodeMGIF(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;
    _GPD_SHM_POINTER_CHECK_RETURN(FALSE);
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    UNUSED(u32Offset);
    if(pGPDContext->MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }
    if(pGPDContext->bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }
    gpd_roi_info roi;
    GPD_API_INFO("MApi_GPD_Decode\n");
    pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr);
    pGPDContext->GpdAddrInfo.u32WriteBufferSize = Size;

    if(pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_OutputDecodeMGIF with DecodeBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)DecodeBuf);
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }

    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&pGPDContext->GpdAddrInfo);

    drv_gpd_set_access_bound(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr,pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr + pGPDContext->GpdAddrInfo.u32WriteBufferSize - 1,
                                                    pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr, pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr +CBUFFER_SIZE+ZBUFFER_SIZE-1);

    GPD_API_INFO("GPDBaseAddr=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.RegBaseAddr);
    GPD_API_INFO("READBITBaseAddr=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.ReadBitBaseAddr);
    GPD_API_INFO("Read  buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr);
    GPD_API_INFO("Write buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Depth buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_DepthBufferAddr);
    GPD_API_INFO("Read  buf size=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32ReadBufferSize);
    GPD_API_INFO("Write buf size=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32WriteBufferSize);

    mdrv_gpd_decode(&pGPDContext->Param, ocolor, 3, &roi);

#if (ENABLE_GPD_MESSAGE ||LOG_DATA_TO_USB)
    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(pGPDContext->Param.u32Height)*(pGPDContext->Param.u32Width)*u8BytesPerPixel, pGPDContext->Param.u32Width, pGPDContext->Param.u32Height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    GPD_API_INFO("[%s, %td, %tdx%td], CRC result=0x%tx\n", __FUNCTION__, (ptrdiff_t)u32FileIndex, (ptrdiff_t)pGPDContext->Param.u32Width, (ptrdiff_t)pGPDContext->Param.u32Height,
        (ptrdiff_t)_MApi_GPD_GetCRCResult((MS_U8 *)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr,(pGPDContext->Param.u32Height)*(pGPDContext->Param.u32Width)*u8BytesPerPixel, pGPDContext->Param.u32Width, ocolor));
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr,
        pGPDContext->GpdAddrInfo.u32ReadBufferSize));

#endif

#endif

    drv_gpd_power_off();
    pGPDContext->bIsGPDInit = FALSE;
    return pGPDContext->Param.enGPDStatus;


}

MS_S32 _MApi_GPD_GetDuration(void)
{
    return MstDGifGetDuration();
}

void _MApi_GPD_SetGIFMode(MS_U32 mode)
{
    MstDGifSetMode(mode);
}



MS_S32 _MApi_GPD_OutputDecodeROI(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size, gpd_roi_info *roi)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;
    _GPD_SHM_POINTER_CHECK_RETURN(FALSE);
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    UNUSED(u32Offset);
    if(pGPDContext->MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }

    if(pGPDContext->bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }

    GPD_API_INFO("MApi_GPD_DecodeROI\n");
    pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    if (FALSE == drv_gpd_get_cacheable())
    {
        pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    else
    {
        pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    pGPDContext->GpdAddrInfo.u32WriteBufferSize = Size;

    if(pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_OutputDecodeROI with DecodeBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)DecodeBuf);
        pGPDContext->bIsGPDInit = FALSE;
        return -1;
    }


    mdrv_gpd_setbuf(&pGPDContext->GpdAddrInfo);

    drv_gpd_set_access_bound(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr,pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr + pGPDContext->GpdAddrInfo.u32WriteBufferSize - 1,
                                                    pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr, pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr +CBUFFER_SIZE+ZBUFFER_SIZE-1);

    GPD_API_INFO("GPDBaseAddr=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.RegBaseAddr);
    GPD_API_INFO("READBITBaseAddr=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.ReadBitBaseAddr);
    GPD_API_INFO("Read  buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr);
    GPD_API_INFO("Write buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Depth buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr,(ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_DepthBufferAddr);
    GPD_API_INFO("Read  buf size=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32ReadBufferSize);
    GPD_API_INFO("Write buf size=0x%tx\n", (ptrdiff_t)pGPDContext->GpdAddrInfo.u32WriteBufferSize);

#if ENABLE_GPD_MESSAGE
//    memset((void *)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr, 0x37, (size_t)pGPDContext->GpdAddrInfo.u32WriteBufferSize);
#endif
    mdrv_gpd_decode(&pGPDContext->Param, ocolor, 2, roi);

#if (ENABLE_GPD_MESSAGE || LOG_DATA_TO_USB)

    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(roi->height)*(roi->width)*u8BytesPerPixel, roi->width, roi->height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    MS_U32 u32OutputCRC = _MApi_GPD_GetCRCResult((MS_U8 *)pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr,(MS_U32) (roi->height)*(roi->width)*u8BytesPerPixel, roi->width, ocolor);
    GPD_API_INFO("[%s, %td, %tdx%td], CRC result=0x%tx\n", __FUNCTION__, (ptrdiff_t)u32FileIndex, (ptrdiff_t)roi->width, (ptrdiff_t)roi->height,
        (ptrdiff_t)u32OutputCRC);
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)pGPDContext->GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr,
        pGPDContext->GpdAddrInfo.u32ReadBufferSize));

#endif
#endif

    drv_gpd_power_off();
    pGPDContext->bIsGPDInit = FALSE;
    return pGPDContext->Param.enGPDStatus;


}


#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void _MApi_GPD_GetMdbInfo(MS_U64* u64ReqHdl)
{
    MdbPrint(u64ReqHdl,"---------MStar GPD Info---------\n\n");
    MdbPrint(u64ReqHdl,"Input buffer address PA               : 0x%lx\n", gGPDMDebug._u32ReadBufferAddr_PA);
    MdbPrint(u64ReqHdl,"Output buffer address PA              : 0x%lx\n", gGPDMDebug._u32WriteBufferAddr_PA);
    MdbPrint(u64ReqHdl,"Internal buffer address PA            : 0x%lx\n", gGPDMDebug._u32InternalBufferAddr_PA);
    MdbPrint(u64ReqHdl,"Input buffer address VA               : 0x%lx\n", gGPDMDebug._u32ReadBufferAddr);
#ifndef MSOS_TYPE_LINUX_KERNEL
    MdbPrint(u64ReqHdl,"Output buffer address VA              : 0x%lx\n", gGPDMDebug._u32WriteBufferAddr);
#endif
    MdbPrint(u64ReqHdl,"Internal buffer address VA            : 0x%lx\n", gGPDMDebug._u32InternalBufferAddr);
    MdbPrint(u64ReqHdl,"Input buffer size                     : 0x%lx\n", gGPDMDebug._u32ReadBufferSize);
    MdbPrint(u64ReqHdl,"Output buffer size                    : 0x%lx\n", gGPDMDebug._u32WriteBufferSize);
    MdbPrint(u64ReqHdl,"Internal buffer size                  : 0x%lx\n", gGPDMDebug._u32InternalBufferSize);
    MdbPrint(u64ReqHdl,"Picture resolution                    : %dx%d\n", gGPDMDebug._u16Image_x_size, gGPDMDebug._u16Image_y_size);

    switch(gGPDMDebug.ocolor)
    {
        case ARGB8888:
            MdbPrint(u64ReqHdl,"Output format                         : ARGB8888\n");
            break;
        case YUV422:
            MdbPrint(u64ReqHdl,"Output format                         : YUV422\n");
            break;
        case RGB565:
            MdbPrint(u64ReqHdl,"Output format                         : RGB565\n");
            break;
        case YVYU422:
            MdbPrint(u64ReqHdl,"Output format                         : YVYU422\n");
            break;
        case GRAY_ALPHA:
            MdbPrint(u64ReqHdl,"Output format                         : GRAY_ALPHA\n");
            break;
        case GRAY:
            MdbPrint(u64ReqHdl,"Output format                         : GRAY\n");
            break;
        default:
            break;
    }

    if(gGPDMDebug.bPNG)
    {
        MdbPrint(u64ReqHdl,"Picture type                          : png\n");
    }
    else
    {
        MdbPrint(u64ReqHdl,"Picture type                          : gif\n");
    }


    MdbPrint(u64ReqHdl,"HW decoding time                      : %dms\n", gGPDMDebug._u32HWEndTime - gGPDMDebug._u32HWStartTime);
}
#endif


#else

void* MAdp_GPD_Init(void)
{
    MS_S32 err;
    //MS_U32 size=0;
    MS_S32 tmp;

    console_printf("MAdp_GPD_Init\n");

    pGPDContext->GpdAddrInfo.RegBaseAddr = (MS_U32)GPD_RIU_BASE;
    pGPDContext->GpdAddrInfo.ReadBitBaseAddr = (MS_U32)GPD_XIU_BASE;

#if 0

   *((volatile MS_U32 *)(CLK_GEN_BASE + (0x5E << 1))) = 0;   // 0x2f << 2

#else

    *((volatile MS_U32 *) (CLK_GEN_BASE + (0x50 << 1))) = 0;   // 0x28 << 2
    //*((volatile MS_U32 *) (CLK_GEN_BASE + (0x50 << 1))) = (2 << 2);   // 0x28 << 2

#endif

    //set gpd miu channel

#if 1
    tmp = *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE + (0xf4 << 1)));


    if ((MS_U32)CBUFFER & (1 << 29))  //512mb
    {
        tmp |= (0x40);  // bit 6
        console_printf("Select MIU1\n");
    }
    else
    {
        tmp &= (~0x40);
        console_printf("Select MIU0\n");

    }

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0xf4 << 1))) = tmp;
#endif

#if 0
    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x67 << 2))) = 0x26;

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x68 << 2))) = 0x8800;

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x69 << 2))) = 0x9800;

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x60 << 2))) = 0x2;

    console_printf("Enable MIU Protection\n");
#endif

    pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr = (MS_U32)CBUFFER;
    pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr);

    console_printf("Get Color buffer\n");

    pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr = (MS_U32)ZBUFFER;
    pGPDContext->GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr);

    console_printf("Get Depth buffer\n");

    pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr = (MS_U32)STREAMBUFFER;
    pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr);

    console_printf("Get Read buffer\n");

    pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr = (MS_U32)FRAMEBUFFER;
    pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr);

    console_printf("Get Write buffer\n");

    pGPDContext->GpdAddrInfo.u32WriteBufferSize = FRAMEBUFFER_SIZE;


    console_printf("GPDBaseAddr=0x%08x\n",pGPDContext->GpdAddrInfo.RegBaseAddr);
    console_printf("READBITBaseAddr=0x%08x\n",pGPDContext->GpdAddrInfo.ReadBitBaseAddr);
    console_printf("Read  buf PA=0x%08x,VA=0x%08x\n",pGPDContext->GpdAddrInfo.u32PA_ReadBufferAddr,pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr);
    console_printf("Write buf PA=0x%08x,VA=0x%08x\n",pGPDContext->GpdAddrInfo.u32PA_WriteBufferAddr,pGPDContext->GpdAddrInfo.u32VA_WriteBufferAddr);
    console_printf("Color buf PA=0x%08x,VA=0x%08x\n",pGPDContext->GpdAddrInfo.u32PA_ColorBufferAddr,pGPDContext->GpdAddrInfo.u32VA_ColorBufferAddr);
    console_printf("Depth buf PA=0x%08x,VA=0x%08x\n",pGPDContext->GpdAddrInfo.u32PA_DepthBufferAddr,pGPDContext->GpdAddrInfo.u32VA_DepthBufferAddr);
    console_printf("Read  buf size=0x%08x\n", pGPDContext->GpdAddrInfo.u32ReadBufferSize);
    console_printf("Write buf size=0x%08x\n", pGPDContext->GpdAddrInfo.u32WriteBufferSize);


    //Call mdrv_gpd_init, This should include Color-buffer, Z-buffer, Bitstream buffer, Frame Buffer.
    err = mdrv_gpd_init(&pGPDContext->GpdAddrInfo);



    return &pGPDContext->GpdAddrInfo;

}

MS_S32 MAdp_GPD_Play(gpd_pic_info* param, MS_U32 ocolor, MS_U8 *srcbuffer, MS_U32 size)
{

    extern void *memcpy(void *destination, const void *source, size_t num);
    //FILE *hFile = 0;
    //long lSize,i=0;
    //size_t result;
    //MS_U32 i = 0;
    gpd_roi_info roi;
    //MS_U8 sig[10];
    //FILE *fp;

    console_printf("MAdp_GPD_Play\n");

    pGPDContext->Param = param;
    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&pGPDContext->GpdAddrInfo);


    #if 1

    console_printf("size:%d\n",size);

    memcpy((void *)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr, (void *)srcbuffer, size);

    pGPDContext->GpdAddrInfo.u32ReadBufferSize = size;

    console_printf("copy image done\n");

    #endif


    #if 0
    hFile = fopen(param->filename, "rb");
    if(!hFile)
    {
        console_printf("Open file error=%s!!\n",param->filename);
        return -1;
    }
    // obtain file size:
    fseek (hFile , 0 , SEEK_END);
    lSize = ftell (hFile);
    rewind (hFile);

    //fread file into bitstream buffer
    result = fread ((char *)pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr,1,lSize,hFile);

    #endif

#if 0
    sig[0] = MMIO8(pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr);
    sig[1] = MMIO8(pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr+1);
    sig[2] = MMIO8(pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr+2);
    sig[3] = MMIO8(pGPDContext->GpdAddrInfo.u32VA_ReadBufferAddr+3);
    console_printf("Signaute = %x,%x,%x,%x\n",sig[0],sig[1],sig[2],sig[3]);
#endif
    //Call mdrv_gpd_decode, specify output format;ARGB888/YUV422/RGB565

    #if 0
    roi.hstart = 0;
    roi.vstart = 0;
    roi.width = 64;
    roi.height = 64;
    #endif

    mdrv_gpd_decode(pGPDContext->Param, ocolor, 0, &roi);

    return pGPDContext->Param.enGPDStatus;
}


MS_S32 MAdp_GPD_Compare(MS_U32 ocolor_typ)
{

    extern MS_U32 memcmp(const void *ptr1, const void *ptr2, size_t num);

    MS_U32 frameptr, argbptr;

    MS_U32 hw_extra = 0, sw_extra = 0;

    MS_U32 pixelbytes = 0;

    MS_U32 width, height;

    MS_U32 result = 0;

    frameptr = MsOS_PA2KSEG1(FRAMEBUFFER);

    argbptr = MsOS_PA2KSEG1(ARGBBUFFER);

    console_printf("MAdp_GPD_Compare\n");

    #if 0

    console_printf("size:%d\n",size);

    memcpy((void *)ARGBBUFFER,(void *)srcbuffer,size);

    console_printf("copy image done\n");

    #endif

    width = pGPDContext->Param.u32Width;

    height = pGPDContext->Param.u32Height;

    console_printf("height:%d width:%d\n", height, width);


    switch (ocolor_typ)
    {
    case ARGB8888:
        pixelbytes = 4;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    case YUV422:
    case YVYU422:
        if(width & 1)
            sw_extra = 2;
    case RGB565:
    case GRAY_ALPHA:
        pixelbytes = 2;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    case GRAY:
        pixelbytes = 1;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    default:
        console_printf("Unsupported output color %d\n", ocolor_typ);
        return NULL;
    }


    while(height)
    {

        result = memcmp((void *) frameptr,(void *) argbptr, width * pixelbytes);

        if(result)
        {
            console_printf("compare failed\n");

            return result;
        }

        frameptr += width * pixelbytes + hw_extra;

        argbptr += width * pixelbytes + sw_extra;

        height--;
    }

    return 0;

}


#endif

void _MApi_GPD_ScalingEnable(MS_U32 mode)
{

    _GPD_SHM_POINTER_CHECK();

    mdrv_gpd_scaling_init(1, mode);

    GPD_API_INFO("GPD Scaling Enable, mode = %td\n", (ptrdiff_t)mode);
}



void _MApi_GPD_ScalingDisable(void)
{
    _GPD_SHM_POINTER_CHECK();

    mdrv_gpd_scaling_init(0, 0);

    GPD_API_INFO("GPD Scaling Disable\n");
}


MS_U32 _MApi_GPD_SetControl(gpd_user_cmd cmd_id, MS_VIRT param)
{
    _GPD_Init_Share_Mem();
    _GPD_SHM_POINTER_CHECK_RETURN(FALSE);
    switch (cmd_id)
    {
        case E_GPD_USER_CMD_SET_ACCESS_REGION:
            return _GPD_SetAccessRegion((gpd_access_region *)param);

	case E_GPD_USER_CMD_SET_CACHEABLE:
            return _GPD_SetCacheable((MS_U32)param);
	case E_GPD_USER_CMD_SET_DEBUG_LEVEL:
            eEnableDbgMsg = (gpd_debug_level)param;
            return TRUE;

        default:
            return FALSE;

    }

}


MS_U32 _MApi_GPD_GetControl(gpd_get_cmd cmd_id, MS_VIRT param)
{
    _GPD_SHM_POINTER_CHECK_RETURN(FALSE);
    switch (cmd_id)
    {
        case E_GPD_GET_CMD_GET_PITCH:
            return _GPD_GetPitch((gpd_get_pitch_info *)param);
        default:
            return FALSE;
    }

}

static MS_U32  u32CRC;

static void verGPD_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFF;
}


static MS_U32 verGPD_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verGPD_Init_CRC_Table(MS_U32 *u32CRCtemptable)
{

    MS_U32 i,j,tmp, result;

    for(i=0;i<=0xFF;i++)
    {
        tmp=i;
        result=0;

        for(j=1;j<9;j++)
        {
             if(tmp & 1)
             {
                        result |= (1 << (8 - j));
             }
             tmp >>= 1;
        }

        u32CRCtemptable[i]=result<<24;
        for(j=0; j<8; j++)
        {
            u32CRCtemptable[i] = (u32CRCtemptable[i] << 1) ^ ((u32CRCtemptable[i] & (1 << 31)) ? 0x04C11DB7UL : 0);
        }

        tmp=u32CRCtemptable[i];
        result=0;

        for(j=1;j<33;j++)
        {
             if(tmp & 1)
             {
                result |= (1 << (32 - j));
             }
             tmp >>= 1;
        }
        u32CRCtemptable[i]=result;
        }
}

static void verGPD_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, MS_U32 ocolor)
{
    MS_U32 u32CRCTable[256];
    MS_U32  u32LoopCounter;
    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

    if (pu8Data == NULL || u32Size == 0) return;

    verGPD_Init_CRC_Table(u32CRCTable);

    if((u32Width*u8BytesPerPixel)%8 ==0)
    {
        for (u32LoopCounter=0; u32LoopCounter<u32Size; u32LoopCounter++)
        {
            u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter] ^ (MS_U8)(u32CRC & 0xFF) ];
        }
    }
    else
    {
        MS_U32 u32LoopCounter2;
        MS_U32 u32WidthAlign;
        u32WidthAlign = u32Width + ((u32Width*u8BytesPerPixel)%8)/u8BytesPerPixel;

        for (u32LoopCounter=0; u32LoopCounter<u32Size/(u32Width*u8BytesPerPixel)*(u32WidthAlign*u8BytesPerPixel); u32LoopCounter+=u32WidthAlign*u8BytesPerPixel)
            for(u32LoopCounter2=0; u32LoopCounter2<u32Width*4; u32LoopCounter2++)
            {
                u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter+u32LoopCounter2] ^ (MS_U8)(u32CRC & 0xFF) ];
            }
    }
}

MS_U32 _MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, MS_U32 ocolor)
{
    verGPD_CRC32_Init();
    verGPD_CRC32_Update(pu8Data, u32Size, u32Width, ocolor);
    return verGPD_CRC32_GetResult();
}



static void verGPD_Input_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFFUL;
}


static MS_U32 verGPD_Input_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verGPD_Input_Init_CRC_Table(MS_U32 *u32CRCtemptable)
{

    MS_U32 i,j,tmp, result;

    for(i=0;i<=0xFF;i++)
    {
        tmp=i;
        result=0;

        for(j=1;j<9;j++)
        {
             if(tmp & 1)
             {
                        result |= (1 << (8 - j));
             }
             tmp >>= 1;
        }

        u32CRCtemptable[i]=result<<24;
        for(j=0; j<8; j++)
        {
            u32CRCtemptable[i] = (u32CRCtemptable[i] << 1) ^ ((u32CRCtemptable[i] & (1 << 31)) ? 0x04C11DB7UL : 0);
        }

        tmp=u32CRCtemptable[i];
        result=0;

        for(j=1;j<33;j++)
        {
             if(tmp & 1)
             {
                result |= (1 << (32 - j));
             }
             tmp >>= 1;
        }
        u32CRCtemptable[i]=result;
        }
}

static void verGPD_Input_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    MS_U32 u32CRCTable[256];
    MS_U32  u32LoopCounter;

    if (pu8Data == NULL || u32Size == 0) return;

    verGPD_Input_Init_CRC_Table(u32CRCTable);

    #if 0
        MS_U32 i;
        for(i = 0;i<256;i++)
        {
            printf("0x%08x,\n", u32CRCTable[i]);
        }
    #endif

    for (u32LoopCounter=0; u32LoopCounter<u32Size; u32LoopCounter++)
    {
        u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter] ^ (MS_U8)(u32CRC & 0xFF) ];
    }
}

MS_U32 MApi_GPD_Input_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    verGPD_Input_CRC32_Init();
    verGPD_Input_CRC32_Update(pu8Data, u32Size);
    return verGPD_Input_CRC32_GetResult();
}


void *MApi_GPD_Init(MS_PHY SrcBuf)
{
    if (pInstantGPD == NULL)
    {
        if(UtopiaOpen(MODULE_GPD | GPD_DRIVER_BASE, &pInstantGPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Open GPD fail\n");
            return FALSE;
        }
    }
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    GPD_INIT_PARAM InitPara;
    memset(&InitPara , 0 ,sizeof(GPD_INIT_PARAM));
    InitPara.SrcBuf = SrcBuf;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_Init\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_Init, (void*)(&InitPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return FALSE;
#else
    return _MApi_GPD_Init(SrcBuf);
#endif

}


MS_S32 MApi_GPD_InputSource(gpd_pic_info* param, MS_PHY InputBuf, MS_U32 Size)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_INPUTSOURCE_PARAM InputPara;
    memset(&InputPara , 0 ,sizeof(GPD_INPUTSOURCE_PARAM));
    InputPara.param = param;
    InputPara.InputBuf = InputBuf;
    InputPara.Size = Size;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_InputSource\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_InputSource, (void*)(&InputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return InputPara.retVal;
#else
    return _MApi_GPD_InputSource(param, InputBuf, Size);
#endif
}

MS_S32 MApi_GPD_OutputDecode(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODE_PARAM OutputPara;
    memset(&OutputPara , 0 ,sizeof(GPD_OUTPUTDECODE_PARAM));
    OutputPara.DecodeBuf = DecodeBuf;
    OutputPara.ocolor = ocolor;
    OutputPara.Size = Size;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_OutputDecode\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecode, (void*)(&OutputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Utopia Close GPD failed\n");
            return OutputPara.retVal;
        }
        pInstantGPD = NULL;
    }
    return OutputPara.retVal;
#else
    return _MApi_GPD_OutputDecode(DecodeBuf, ocolor, Size);
#endif
}

MS_S32 MApi_GPD_OutputDecodeMGIF(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODE_PARAM OutputPara;
    memset(&OutputPara , 0 ,sizeof(GPD_OUTPUTDECODE_PARAM));
    OutputPara.DecodeBuf = DecodeBuf;
    OutputPara.ocolor = ocolor;
    OutputPara.Size = Size;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_OutputDecodeMGIF\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecodeMGIF, (void*)(&OutputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Utopia Close GPD failed\n");
            return OutputPara.retVal;
        }
        pInstantGPD = NULL;
    }
    return OutputPara.retVal;
#else
    return _MApi_GPD_OutputDecodeMGIF(DecodeBuf, ocolor, Size);
#endif
}

MS_S32 MApi_GPD_GetDuration(void)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GETDURATION_PARAM GetDuration;
    memset(&GetDuration , 0 ,sizeof(GPD_GETDURATION_PARAM));
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_GetDuration\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetDuration, (void*)(&GetDuration)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return GetDuration.retVal;
#else
    return _MApi_GPD_GetDuration();
#endif

}


void MApi_GPD_SetGIFMode(MS_U32 mode)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_SETGIFMODE_PARAM GIFMode;
    memset(&GIFMode , 0 ,sizeof(GPD_SETGIFMODE_PARAM));
    GIFMode.mode = mode;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_SetGIFMode\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_SetGIFMode, (void*)(&GIFMode)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    _MApi_GPD_SetGIFMode(mode);
#endif

}

MS_S32 MApi_GPD_OutputDecodeROI(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size, gpd_roi_info *roi)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODEROI_PARAM OutputParaROI;
    memset(&OutputParaROI , 0 ,sizeof(GPD_OUTPUTDECODEROI_PARAM));
    OutputParaROI.DecodeBuf = DecodeBuf;
    OutputParaROI.ocolor = ocolor;
    OutputParaROI.Size = Size;
    OutputParaROI.roi = roi;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_OutputDecodeROI\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecodeROI, (void*)(&OutputParaROI)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Utopia Close GPD failed\n");
            return OutputParaROI.retVal;
        }
        pInstantGPD = NULL;
    }
    return OutputParaROI.retVal;
#else
    return _MApi_GPD_OutputDecodeROI(DecodeBuf, ocolor, Size, roi);
#endif

}

void MApi_GPD_ScalingEnable(MS_U32 mode)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_SCALINGENABLE_PARAM ScalingEnable;
    memset(&ScalingEnable , 0 ,sizeof(GPD_SCALINGENABLE_PARAM));
    ScalingEnable.mode = mode;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_ScalingEnable\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_ScalingEnable, (void*)(&ScalingEnable)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    _MApi_GPD_ScalingEnable(mode);
#endif

}

void MApi_GPD_ScalingDisable(void)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_ScalingDisable\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_ScalingDisable, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    _MApi_GPD_ScalingDisable();
#endif
}

MS_U32 MApi_GPD_SetControl(gpd_user_cmd cmd_id, MS_VIRT param)
{
#ifdef UTOPIA_20
    if (pInstantGPD == NULL)
    {
        if(UtopiaOpen(MODULE_GPD | GPD_DRIVER_BASE, &pInstantGPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Open GPD fail\n");
            return FALSE;
        }
    }

    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_CONTROL_PARAM CtrlPara;
    memset(&CtrlPara , 0 ,sizeof(GPD_CONTROL_PARAM));
    CtrlPara.cmd_id = cmd_id;
    CtrlPara.param = param;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_SetControl\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_SetControl, (void*)(&CtrlPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CtrlPara.retVal;
#else
    return _MApi_GPD_SetControl(cmd_id, param);
#endif

}

MS_U32 MApi_GPD_GetControl(gpd_get_cmd cmd_id, MS_VIRT param)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GET_CONTROL_PARAM CtrlPara;
    memset(&CtrlPara , 0 ,sizeof(GPD_GET_CONTROL_PARAM));
    CtrlPara.cmd_id = cmd_id;
    CtrlPara.param = param;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_GetControl\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetControl, (void*)(&CtrlPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CtrlPara.retVal;
#else
    return _MApi_GPD_GetControl(cmd_id, param);
#endif

}

MS_U32 MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, MS_U32 ocolor)
{
#ifdef UTOPIA_20
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GETCRCRESULT_PARAM CRCPara;
    memset(&CRCPara , 0 ,sizeof(GPD_GETCRCRESULT_PARAM));
    CRCPara.pu8Data = pu8Data;
    CRCPara.u32Size = u32Size;
    CRCPara.u32Width = u32Width;
    CRCPara.ocolor = ocolor;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_GetCRCResult\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetCRCResult, (void*)(&CRCPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CRCPara.retVal;
#else
    return _MApi_GPD_GetCRCResult(pu8Data, u32Size, u32Width, ocolor);
#endif

}

void MApi_GPD_GetConfig(void)
{
    MApi_GPD_SetControl(E_GPD_USER_CMD_SET_DEBUG_LEVEL, E_GPD_DEBUG_LEVEL_NONE);
}

