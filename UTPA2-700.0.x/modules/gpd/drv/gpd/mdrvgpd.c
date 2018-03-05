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
#include "MsCommon.h"
#include "gpd.h"
#include "mdrvgpd.h"
#include "gif_lib.h"
#include "png.h"
#include "drvgpd.h"

#define ERROR_HANDLE_AFTER_DECODE     TRUE
#define MAX_ERROR_HANDLE_COUNT        10

static gpd_addr_info g_gpd_addr;

extern MS_VIRT GPD_REG_BASE;
extern MS_VIRT READBITBASE;

void mdrv_gpd_setbuf(gpd_addr_info* addr_info)
{

    console_printf("mdrv_gpd_setbuf\n");


    GPD_REG_BASE = addr_info->RegBaseAddr;
    READBITBASE = addr_info->ReadBitBaseAddr;
    console_printf("GPD_REG_BASE = 0x%tx\n", (ptrdiff_t)GPD_REG_BASE);
    console_printf("READBITBASE = 0x%tx\n", (ptrdiff_t)READBITBASE);

    memcpy((void*)&g_gpd_addr, (void*)addr_info, sizeof(gpd_addr_info));
}

MS_S32 mdrv_gpd_init(gpd_addr_info* addr_info)
{

    console_printf("mdrv_gpd_init\n");

    mdrv_gpd_setbuf(addr_info);
    drv_gpd_set_zbuf_bas((addr_info->u32PA_DepthBufferAddr)>>3);
    drv_gpd_set_cbuf_bas((addr_info->u32PA_ColorBufferAddr)>>3);
    drv_gpd_init_chip_specific_reg();
    return 0;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_BOOL bPNG =FALSE;
MS_BOOL mdrv_gpd_bPNG(void)
{
    return bPNG;
}
#endif

MS_S32 mdrv_gpd_decode(gpd_pic_info *pic_info, MS_U32 Ocolor, MS_U32 mode, gpd_roi_info *roi)
{
    MS_U8 sig[5] = {0};
    GifFileType *GifFile = NULL;
    PngInfop pnginfo = NULL;


    console_printf("mdrv_gpd_decode\n");

    if(mode != 3)
    {
        sig[0] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr);
        sig[1] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr+1);
        sig[2] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr+2);
        sig[3] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr+3);
    }

    console_printf("Signaute = %x,%x,%x,%x\n",sig[0],sig[1],sig[2],sig[3]);


    if ((sig[0] == 'G' && sig[1] == 'I' && sig[2] == 'F') || (mode == 3))
    {
        /* GIF Decode*/
        console_printf("GIF Decode!!\n");

        GifFile = MstDGifProcess((void*)&g_gpd_addr, Ocolor, mode, (void *)roi);
        if (GifFile == NULL)
        {
            console_printf("GIF Decode error!!\n");
        }
        if(!GifFile)
        {
            pic_info->enGPDStatus = -1;
        }
        else
        {
            //pic_info->enGPDStatus = 0;
            pic_info->enGPDStatus = GifFile->GifState;
            pic_info->u32Width = GifFile->SWidth;
            pic_info->u32Height = GifFile->SHeight;
            if(GifFile->SColorMap != NULL)
                pic_info->u8BitPP = GifFile->SColorMap->BitsPerPixel;
            pic_info->u8Interlace = GifFile->Image.Interlace;
            pic_info->u8MGIF = GifFile->isMGIF;
        }
    }
    else
    {
        /* PNG Decode*/
        if ((sig[0] == 0x89) && (sig[1] == 'P') && (sig[2] == 'N') && (sig[3] == 'G'))
        {

            console_printf("PNG Decode!!\n");

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
            bPNG =TRUE;
#endif

#if ERROR_HANDLE_AFTER_DECODE
            MS_U8 u8Cnt;
            for(u8Cnt=0; u8Cnt< MAX_ERROR_HANDLE_COUNT; u8Cnt++)
            {
                pnginfo = PngProcess((void *)&g_gpd_addr, Ocolor, mode, (void *)roi);
                if(pnginfo)
                {
                    console_printf("Decode Done!!!");
                    break;
                }
                else
                {
                    console_printf("Decode error!!! re-decode again!!!");
                }
            }
#else
            pnginfo = PngProcess((void *)&g_gpd_addr, Ocolor, mode, (void *)roi);
#endif

        }
        else
        {
            console_printf("Unknown decode format!!\n");
            pnginfo = NULL;
        }
        if(!pnginfo)
        {
            pic_info->enGPDStatus = -1;
        }
        else
        {
            pic_info->enGPDStatus   = 0;
            pic_info->u32Width      = pnginfo->png_width;
            pic_info->u32Height     = pnginfo->png_height;
            pic_info->u8BitPP       = pnginfo->bpp;
            pic_info->u8Interlace   = pnginfo->interlace;
            pic_info->u8MGIF        = 0;
        }
    }

    return pic_info->enGPDStatus;
}

void mdrv_gpd_scaling_init(MS_U32 enable, MS_U32 mode)
{
    drv_gpd_set_scaling(enable, mode);
}
