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
/******************************************************************************
*   "Gif-Lib" - Yet another gif library.                      *
*                                          *
* Written by:  Gershon Elber            IBM PC Ver 1.1,    Aug. 1990     *
*******************************************************************************
* The kernel of the GIF Decoding process can be found here.              *
*******************************************************************************
* History:                                      *
* 16 Jun 89 - Version 1.0 by Gershon Elber.                      *
*  3 Sep 90 - Version 1.1 by Gershon Elber (Support for Gif89, Unique names). *
******************************************************************************/
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#else
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#include "MsCommon.h"
#include "gif_lib.h"
#include "gif_lib_private.h"
#include "gpd_reg.h"
#include "drvgpd.h"
#include "mdrvgpd.h"
#include "MsOS.h"



#define READ(_gif,_buf,_len)                                     \
  (((GifFilePrivateType*)_gif->Private)->Read ?                   \
    ((GifFilePrivateType*)_gif->Private)->Read(_gif,_buf,_len) : \
    read_byte(_gif,_buf,_len))



static MS_U32 DGifGetWord(GifFileType *GifFile, MS_U32 *Word);
//static MS_U32 DGifSetupDecompress(GifFileType *GifFile);

#ifdef SW_DEC
static MS_U32 DGifDecompressLine(GifFileType *GifFile, GifPixelType *Line,
                                MS_U32 LineLen);
static MS_U32 DGifGetPrefixChar(MS_U32 *Prefix, MS_U32 Code, MS_U32 ClearCode);
static MS_U32 DGifDecompressInput(GifFileType *GifFile, MS_U32 *Code);
static MS_U32 DGifBufferedInput(GifFileType *GifFile, GifByteType *Buf,
                             GifByteType *NextByte);
#endif
//----------------------------------------------------------------------------------
//                                              Local Variable
//----------------------------------------------------------------------------------

static GifFileType         g_GifFile;
static GifFilePrivateType  g_Private;
static ColorMapObject      CMObject[20];
static ColorMapObject      LoObject;
static GifColorType        ColorTyp[20];

//static struct SavedImage   g_SavedImages[20];
static MS_U32 obj_idx = 0;
static GifFileType *GifFile = &g_GifFile;
static MS_U32 FileHandle = 0;
static gpd_roi_info frame_roi;
static MS_U32 GIFmode = NO_SUPPORT_MGIF;

MS_U32 MstDGifGetDuration(void)
{
    return GifFile->GraphicCtrl.time;
}

void MstDGifSetMode (MS_U32 mode)
{
    GIFmode = mode;
}


MS_U32 read_byte(GifFileType* gif,GifByteType* buf,MS_U32 len)
{
    MS_U32 i=0,cnt=len;
    MS_U16 tmp;

    while (cnt>=2)
    {
        //tmp = show_bits(16);
        //console_printf("0show bit=%x\n",tmp);
        tmp = read_bits(16);
#if 1
        buf[i] = (MS_U8)(tmp&0xff);
        buf[i+1] = (MS_U8)(tmp>>8);
#else
        memcpy((void*)(buf+i),(void*)&tmp,2);
#endif
        i+=2;
        cnt-=2;
    }
    if(cnt>0)
    {
        //tmp = show_bits(8);
        //console_printf("1show bit=%x\n",tmp);
        buf[i] = (GifByteType)read_bits(8);
    }
    gif->UserData+=len;
    return len;
}

MS_U32 READ_ram(GifFileType* gif,GifByteType* buf,MS_U32 len)
{
#if 1
    MS_U32 u32i;
    for(u32i=0; u32i<len; u32i++)
    {
        buf[u32i] = *((MS_U8*)gif->UserData +u32i);
    }
#else
    memcpy((void*)buf,(void*)(gif->UserData),len);
#endif
    gif->UserData+=len;
    return len;
}

void rgb2yuv(MS_U8 r_in, MS_U8 g_in, MS_U8 b_in,
             MS_U8 *y_out_ptr, MS_U8 *cb_out_ptr, MS_U8 *cr_out_ptr)
{
    MS_U32 y_term_r;
    MS_U32 y_term_g;
    MS_U32 y_term_b;

    MS_U32 cb_term_r;
    MS_U32 cb_term_g;
    MS_U32 cb_term_b;

    MS_U32 cr_term_r;
    MS_U32 cr_term_g;
    MS_U32 cr_term_b;

    MS_U32 y_term_r_g_b;
    MS_U32 y_out_temp;

    MS_U32 cb_term_r_g;
    MS_U32 cb_term_b_e;
    MS_U32 cb_out_temp;

    MS_U32 cr_term_g_b;
    MS_U32 cr_term_r_e;
    MS_U32 cr_out_temp;


    y_term_r  = r_in * 263;
    y_term_g  = g_in * 129;
    y_term_b  = b_in * 25;
    y_term_r_g_b = (y_term_r >> 2) + y_term_g + y_term_b;
    y_out_temp = (y_term_r_g_b >> 7) + 33;
    *y_out_ptr  = (y_out_temp >> 1) & 0xFF;

    cb_term_r = r_in * 19;
    cb_term_g = g_in * 37;
    cb_term_b = b_in * 7;
    cb_term_r_g = cb_term_r + cb_term_g;
    cb_term_b_e = cb_term_b + 2056;
    cb_out_temp = (cb_term_b_e << 3) - cb_term_r_g;
    *cb_out_ptr = (cb_out_temp >> 7) & 0xFF;

    cr_term_r = r_in * 7;
    cr_term_g = g_in * 47;
    cr_term_b = b_in * 9;
    cr_term_g_b = cr_term_g + cr_term_b;
    cr_term_r_e = cr_term_r + 2056;
    cr_out_temp = (cr_term_r_e << 3) - cr_term_g_b;
    *cr_out_ptr = (cr_out_temp >> 7) & 0xFF;
}



void FillBkColor(GifFileType *GifFile, GifByteType* framebuff, MS_U32 ocolor,
                 MS_U32 width, MS_U32 height, MS_U32 pitch)
{
    GifByteType red   = 0;
    GifByteType green = 0;
    GifByteType blue  = 0;
    GifByteType alpha = 0xFF;
    GifByteType y, cb, cr;
    MS_U32 u32color;
    MS_U16 u16color;
    MS_U8 u8color;
    MS_U8 colorbuff[4];
    MS_U32 i, pixelbytes;

    if (GifFile->SColorMap && GifFile->SBackGroundColor < GifFile->SColorMap->ColorCount)
    {
        GifColorType *palette = GifFile->SColorMap->Colors;
        red   = palette[GifFile->SBackGroundColor].Red;
        green = palette[GifFile->SBackGroundColor].Green;
        blue  = palette[GifFile->SBackGroundColor].Blue;
    }


    switch (ocolor)
    {
    case ARGB8888:
        u32color = (alpha << 24) | (red << 16) | (green << 8) | (blue);
        memcpy(colorbuff, &u32color, 4);
        pixelbytes = 4;
        break;
    case RGB565:
        u16color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
        memcpy(colorbuff, &u16color, 2);
        pixelbytes = 2;
        break;
    case YUV422:

    case YVYU422:
        rgb2yuv(red, green, blue, &y, &cb, &cr);
        u32color = (cr << 24) | (y << 16) | (cb << 8) | (y);
        memcpy(colorbuff, &u32color, 4);
        pixelbytes = 2;
        break;
    case GRAY_ALPHA:
        rgb2yuv(red, green, blue, &y, &cb, &cr);
        u16color = (alpha << 8) | (y);
        memcpy(colorbuff, &u16color, 2);
        pixelbytes = 2;
        break;
    case GRAY:
        rgb2yuv(red, green, blue, &y, &cb, &cr);
        u8color = y;
        memcpy(colorbuff, &u8color, 1);
        pixelbytes = 1;
        break;
    default:
        return;
    }

    if(ocolor == YUV422)
    {
        for (i = 0; i < width * pixelbytes; i += pixelbytes)
        {
            if(i % 4)
                memcpy(framebuff + i, colorbuff + 2, pixelbytes);
            else
                memcpy(framebuff + i, colorbuff, pixelbytes);

        }

    }
    else if(ocolor == YVYU422)
    {
        for (i = 0; i < width * pixelbytes; i += pixelbytes)
        {
            if(i % 4)
                memcpy(framebuff + i, colorbuff, pixelbytes);
            else
                memcpy(framebuff + i, colorbuff + 2, pixelbytes);

        }

    }
    else
    {
        for (i = 0; i < width * pixelbytes; i += pixelbytes)
            memcpy(framebuff + i, colorbuff, pixelbytes);
    }


    for (i = 1; i < height; i++)
        memcpy(framebuff + pitch * i, framebuff, width * pixelbytes);
}

gpd_roi_info GetFrameROI(GifFileType *GifFile, gpd_roi_info image_roi)
{
    MS_U32 left   = GifFile->Image.Left;
    MS_U32 right  = GifFile->Image.Left + GifFile->Image.Width;
    MS_U32 top    = GifFile->Image.Top;
    MS_U32 bottom = GifFile->Image.Top + GifFile->Image.Height;
    gpd_roi_info frame_roi;

    if (left < image_roi.hstart)
        left = image_roi.hstart;
    if (left > image_roi.hstart + image_roi.width)
        left = image_roi.hstart + image_roi.width;

    if (right < image_roi.hstart)
        right = image_roi.hstart;
    if (right > image_roi.hstart + image_roi.width)
        right = image_roi.hstart + image_roi.width;

    if (top < image_roi.vstart)
        top = image_roi.vstart;
    if (top > image_roi.vstart + image_roi.height)
        top = image_roi.vstart + image_roi.height;

    if (bottom < image_roi.vstart)
        bottom = image_roi.vstart;
    if (bottom > image_roi.vstart + image_roi.height)
        bottom = image_roi.vstart + image_roi.height;

    frame_roi.hstart = left - GifFile->Image.Left;
    frame_roi.vstart = top - GifFile->Image.Top;
    frame_roi.width  = right - left;
    frame_roi.height = bottom - top;

    //console_printf("image roi:  %d, %d, %d, %d\n", image_roi.hstart, image_roi.vstart, image_roi.width, image_roi.height);
    console_printf("frame roi:  %td, %td, %td, %td\n", (ptrdiff_t)frame_roi.hstart, (ptrdiff_t)frame_roi.vstart, (ptrdiff_t)frame_roi.width, (ptrdiff_t)frame_roi.height);
    //console_printf("frame area: %d, %d, %d, %d\n", GifFile->Image.Left, GifFile->Image.Top, GifFile->Image.Width, GifFile->Image.Height);

    return frame_roi;
}

/******************************************************************************
*   Open a new gif file for read, given by its name.                  *
*   Returns GifFileType pointer dynamically allocated which serves as the gif *
* info record. _MstGifError is cleared if succesfull.                  *
******************************************************************************/
GifFileType *MstDGifProcess(void *ptr, MS_U32 ocolor_typ, MS_U32 mode, void *roi)
{
    gpd_addr_info *buf_addr = (gpd_addr_info *)ptr;
    gpd_roi_info *roi_ptr = (gpd_roi_info *)roi;
    gpd_roi_info image_roi = {0};
    //gpd_roi_info frame_roi;
    //GifFileType *GifFile;
    //MS_U32 FileHandle = 0;
    GifRecordType RecordType;
    GifByteType *Extension;
    MS_U32 vdata, image_pitch, frame_pitch, pixelbytes;
    MS_VIRT FrameBuffAddr, TempBuffOffset, StreamBuffAddr;
    MS_U32 vstart_shift = 0;
    MS_U32 frame_cnt = 0;
    MS_U32 frame_size = 0;
    MS_U32 i, j, ExtCode;

    MS_U32 roi_mode = 0;
    MS_U32 dummy_pass = 0;
    MS_U32 bstart_set = 0;
    MS_U32 trans_idx = -1;

    obj_idx = 0;

    switch (ocolor_typ)
    {
        case ARGB8888:
            pixelbytes = 4; break;
        case RGB565:
        case YUV422:
        case YVYU422:
            pixelbytes = 2; break;
        case GRAY_ALPHA:
            //pixelbytes = 2; break;
        case GRAY:
            //pixelbytes = 1; break;
        default:
            console_printf("Unsupported output color %td\n", (ptrdiff_t)ocolor_typ);
            return NULL;
    }

    StreamBuffAddr = buf_addr->u32VA_ReadBufferAddr;
    FrameBuffAddr  = buf_addr->u32VA_WriteBufferAddr;
    TempBuffOffset = (buf_addr->u32WriteBufferSize / 2) & ~(64-1);

    //add new mode 3 as motion GIF mode
    if(mode != 3)
    {
        drv_gpd_gif_en();
        drv_gpd_Init();
        drv_gpd_set_ocolor(ocolor_typ);
        drv_gpd_set_default_alpha(DEFAULT_ALPHA);
        //drv_gpd_eng_active(0x3f);
        /*Get Screen Descriptor*/
        GifFile = MstDGifOpenFileHandle(FileHandle, (InputFunc)READ_ram, (GifByteType*)(StreamBuffAddr));
        console_printf("width = %td, height = %td\n", (ptrdiff_t)GifFile->SWidth, (ptrdiff_t)GifFile->SHeight);
    }

    /*Calculate image ROI and frame buffer size*/
    if (mode == 2)
    {
        roi_mode = 1;
        image_roi = *roi_ptr;
        frame_size = ((roi_ptr->width * pixelbytes + 7) & ~7) * (roi_ptr->height + 7);
    }
    else if((mode == 0) || (mode == 3)) // add mode 3 motion GIF
    {
        roi_mode = 0;
        image_roi.hstart = 0;
        image_roi.vstart = 0;
        image_roi.height = GifFile->SHeight;
        image_roi.width  = GifFile->SWidth;
        frame_size = ((GifFile->SWidth * pixelbytes + 7) & ~7) * GifFile->SHeight;
    }
    image_pitch = (image_roi.width * pixelbytes + 7) >> 3;

    if((drv_gpd_get_scaling_mode() & 0x1) && mode==0)
    {
        switch(drv_gpd_get_scaling_mode() >> 1)
        {
            case 0:
                frame_size/=4;
                break;
            case 1:
                frame_size/=16;
                break;
            case 2:
                frame_size/=64;
                break;
            case 3:
                frame_size/=256;
                break;
            default:
                break;
        }
    }

    /* Check buffer size */
    if (frame_size > TempBuffOffset)
    {
        console_printf("Frame buffer is too small\n");
        return NULL;
    }

    /*Fill background color*/
    if((mode != 1) && (mode != 3))
    {
        FillBkColor(GifFile, (GifByteType*)(FrameBuffAddr), ocolor_typ,
                    image_roi.width, image_roi.height, image_pitch << 3);
        if(MsOS_Dcache_Flush(FrameBuffAddr, buf_addr->u32WriteBufferSize))
        {
            console_printf("Invalidate cache\n");
        }
        MsOS_FlushMemory();
    }
    do
    {
        if (MstDGifGetRecordType(GifFile, &RecordType) == GIF_ERROR)
        {
            PrintGifError();
            return NULL;
        }

        switch (RecordType)
        {
          case IMAGE_DESC_RECORD_TYPE:
            if (MstDGifGetImageDesc(GifFile) == GIF_ERROR)
            {
                PrintGifError();
                return NULL;
            }

            /*Check frame boundary*/
            if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
                GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight)
                return NULL;

            /*Get info only*/
            if (mode == 1)
            {
                return GifFile;
            }

            /*Calculate frame ROI*/
            frame_roi = GetFrameROI(GifFile, image_roi);
            dummy_pass = 0;
            if (frame_roi.width == 0 || frame_roi.height == 0)
            {
                dummy_pass = 1;
                frame_roi.hstart = 0;
                frame_roi.vstart = 0;
                frame_roi.width  = 1;
                frame_roi.height = 1;
            }

            /*Check ROI alignment for interlace frame*/
            vstart_shift = 0;
            if (GifFile->Image.Interlace)
            {
                vstart_shift = frame_roi.vstart & 7UL;
                frame_roi.vstart -= vstart_shift;
                frame_roi.height += vstart_shift;
            }

            /*Set ROI*/
            drv_gpd_set_ROI(roi_mode, frame_roi.hstart, frame_roi.vstart, frame_roi.width, frame_roi.height);

            /*Set pitch*/
            frame_pitch = (frame_roi.width * pixelbytes + 7) >> 3;
            drv_gpd_set_pitch(frame_pitch);

            /*Set istart*/
            vdata = buf_addr->u32PA_WriteBufferAddr + TempBuffOffset;
            drv_gpd_set_istart(vdata);

            /*Fill background color when disposal method is 2*/
            if (GifFile->GraphicCtrl.enable && GifFile->GraphicCtrl.disposal_method == 2)
            {
                FillBkColor(GifFile, (GifByteType*)(FrameBuffAddr), ocolor_typ,
                            image_roi.width, image_roi.height, image_pitch << 3);
            }

            /*Set iwidth and iheight*/
            drv_gpd_set_iwidth_height(GifFile->Image.Width, GifFile->Image.Height);

            /*Set interlace*/
            drv_gpd_set_interlace(GifFile->Image.Interlace);

            /*Set bstart and bend*/
            if ((!bstart_set) && (mode != 3))
            {
                bstart_set = 1;
                ((GifFilePrivateType*)(GifFile->Private))->Read = NULL;
                vdata = GifFile->UserData - buf_addr->u32VA_ReadBufferAddr;
                drv_gpd_set_bstart_end((vdata + buf_addr->u32PA_ReadBufferAddr), 0);

                /*Flush fifo data*/
                for (i = 0; i < 4; i++)
                    read_bits(16);
            }

            console_printf("flush done\n");

            /*Set color map*/
            if (GifFile->GraphicCtrl.enable && GifFile->GraphicCtrl.trans_bit)
                trans_idx = GifFile->GraphicCtrl.trans_idx;
            else
                trans_idx = -1;
            drv_gpd_set_gif_local_map_size(0);
            if (GifFile->Image.ColorMap)
                drv_gpd_write_cmap(GifFile->Image.ColorMap->ColorCount, 0, trans_idx);
            else
                drv_gpd_write_cmap(GifFile->SColorMap->ColorCount, (MS_U8 *)GifFile->SColorMap->Colors, trans_idx);

            /*Start decoding*/
            drv_gpd_set_gif_go();

            console_printf("gif go\n");

            while (!drv_gpd_gif_done())
            {
                if (drv_gpd_ofifo_done())
                {
                   console_printf("Ofifo done but gif done.\n");
                }
            }
            #if 0
            drv_gpd_script_wait_gif_done(1);
            {
                SCRIPT_DUMP("#Dump frame %d\n", frame_cnt);
                SCRIPT_DUMP("setchannel XD2MIU-32\n");
                SCRIPT_DUMP("save (filename).frame%d.%dx%d.argb8888 0x%X 0x%X\n",
                            frame_cnt, frame_pitch * 2, frame_roi.height,
                            FrameBuffAddr + TempBuffOffset, frame_roi.height * frame_pitch * 8);
                SCRIPT_DUMP("setchannel TOP_RIU-16b\n\n");
            }
            #endif
            frame_cnt++;
            console_printf("gif done\n");

            if (MsOS_Dcache_Flush(FrameBuffAddr, buf_addr->u32WriteBufferSize))
            {
                console_printf("Invalidate cache\n");
            }
            MsOS_FlushMemory();

            /*Blending*/
            if (!dummy_pass)
            {
                console_printf("blending...\n");
                GifByteType *image_ptr = (GifByteType *)(FrameBuffAddr);
                GifByteType *frame_ptr = (GifByteType *)((FrameBuffAddr + TempBuffOffset));

                #if 0
                console_printf("image_ptr = %x\n", image_ptr);
                console_printf("frame_roi.vstart = %d\n", frame_roi.vstart);
                console_printf("GifFile->Image.Top = %d\n", GifFile->Image.Top);
                console_printf("roi_ptr->vstart = %d\n", roi_ptr->vstart);
                console_printf("vstart_shift = %d\n", vstart_shift);
                console_printf("image_pitch = %d\n", image_pitch);
                console_printf("frame_roi.hstart = %d\n", frame_roi.hstart);
                console_printf("GifFile->Image.Left = %d\n", GifFile->Image.Left);
                console_printf("roi_ptr->hstart = %d\n", roi_ptr->hstart);
                console_printf("pixelbytes; = %d\n", pixelbytes);
                #endif

                image_ptr += (frame_roi.vstart + GifFile->Image.Top  - roi_ptr->vstart + vstart_shift) * (image_pitch << 3) +
                             (frame_roi.hstart + GifFile->Image.Left - roi_ptr->hstart) * pixelbytes;

                //console_printf("image_ptr = %x\n", image_ptr);
                frame_ptr += vstart_shift * (frame_pitch << 3);
                //console_printf("frame_ptr = %x\n", frame_ptr);
                //console_printf("frame_roi_height = %d  vstart_shift = %d\n", frame_roi.height, vstart_shift);
                //MsOS_ReadMemory();
                //console_printf("Read Memory!!\n");

                for (j = 0; j < frame_roi.height - vstart_shift; j++)
                {
                    /*Only support ARGB8888*/

                    if(ocolor_typ == ARGB8888)
                    {
                        for (i = 0; i < frame_roi.width * 4; i += 4)
                        {
                            if (frame_ptr[i+3])
                            {
                                image_ptr[i+0] = frame_ptr[i+0];
                                image_ptr[i+1] = frame_ptr[i+1];
                                image_ptr[i+2] = frame_ptr[i+2];
                                image_ptr[i+3] = 0xFF;
                            }
                        }
                    }
                    else if(ocolor_typ == GRAY)
                    {
                        for (i = 0; i < frame_roi.width * 1; i += 1)
                            image_ptr[i+0] = frame_ptr[i+0];
                    }
                    else if(ocolor_typ == GRAY_ALPHA)
                    {
                        for (i = 0; i < frame_roi.width * 2; i += 2)
                        {
                            if (frame_ptr[i+1])
                            {
                                image_ptr[i+0] = frame_ptr[i+0];
                                image_ptr[i+1] = 0xFF;
                            }
                        }
                    }
                    else
                    {
                        for (i = 0; i < frame_roi.width * 2; i += 2)
                        {
                            //console_printf("data:%x\n", frame_ptr[i+1]);
                            //console_printf("data:%x\n", frame_ptr[i+0]);

                            image_ptr[i+0] = frame_ptr[i+0];
                            image_ptr[i+1] = frame_ptr[i+1];

                            //console_printf("data 0:%x\n", image_ptr[i+0]);
                            //image_ptr[i+0] = 0x77;
                            //console_printf("data 1:%x\n", image_ptr[i+0]);
                            //image_ptr[i+0] = frame_ptr[i+0];
                            //console_printf("data 2:%x\n", image_ptr[i+0]);
                        }
                    }
                    image_ptr += image_pitch << 3;
                    frame_ptr += frame_pitch << 3;
                }
            }

            if(MsOS_Dcache_Flush(FrameBuffAddr, buf_addr->u32WriteBufferSize))
            {
                console_printf("Invalidate cache\n");
            }
            MsOS_FlushMemory();

            GifFile->GraphicCtrl.enable = 0;

            if(GifFile->isMGIF && GIFmode)
            {
                GifFile->GifState = STATE_MGIF_WAIT_BUFFFER;
                return GifFile;
            }
            break;

        case EXTENSION_RECORD_TYPE:
            /* Skip any extension blocks in file: */
            if (MstDGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR)
            {
                PrintGifError();
                return NULL;
            }
            while (Extension != NULL)
            {
                if (MstDGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR)
                {
                    PrintGifError();
                    return NULL;
                }
            }
            break;

        case TERMINATE_RECORD_TYPE:
            break;

        default:
            /* Should be traps by MstDGifGetRecordType. */
            break;
        }
    } while (RecordType != TERMINATE_RECORD_TYPE);


    GifFile->GifState = STATE_MGIF_TERMINATE;
    return GifFile;
}

/******************************************************************************
*   Update a new gif file, given its file handle.                             *
*   Returns GifFileType pointer dynamically allocated which serves as the gif *
*   info record. _MstGifError is cleared if succesfull.                          *
******************************************************************************/
GifFileType *MstDGifOpenFileHandle(MS_U32 FileHandle, InputFunc read_fun, GifByteType* bitstream)
{
    GifByteType Buf[GIF_STAMP_LEN+1];
    GifFileType *GifFile;
    GifFilePrivateType *Private;

#if 1
    GifFile = (GifFileType *)&g_GifFile;
    memset(GifFile, 0, sizeof(GifFileType));
    Private = (GifFilePrivateType *)&g_Private;
#else
    if ((GifFile = (GifFileType *)&g_GifFile) == NULL) {
        _MstGifError = D_GIF_ERR_NOT_ENOUGH_MEM;
        return NULL;
    }
    memset(GifFile, 0, sizeof(GifFileType));

    if ((Private = (GifFilePrivateType *)&g_Private) == NULL)
    {
        _MstGifError = D_GIF_ERR_NOT_ENOUGH_MEM;
        return NULL;
    }
#endif

    GifFile->Private = (VoidPtr) Private;
    Private->FileHandle = FileHandle;
    Private->FileState = FILE_STATE_READ;
    Private->Read     = read_fun;//0;  /* don't use alternate input method (TVT) */
    GifFile->UserData = (MS_VIRT)(bitstream);//0;  /* TVT */
    /* Lets see if this is a GIF file: */

    //if (read_byte(GifFile,Buf, GIF_STAMP_LEN) != GIF_STAMP_LEN) {
    if (READ(GifFile,Buf, GIF_STAMP_LEN) != GIF_STAMP_LEN) {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return NULL;
    }


    /* The GIF Version number is ignored at this time. Maybe we should do    */
    /* something more useful with it.                         */
    Buf[GIF_STAMP_LEN] = 0;
    if (strncmp(GIF_STAMP, (void*)Buf, GIF_VERSION_POS) != 0)
    {
        _MstGifError = D_GIF_ERR_NOT_GIF_FILE;
        return NULL;
    }
    if (MstDGifGetScreenDesc(GifFile) == GIF_ERROR) {
        return NULL;
    }

    _MstGifError = 0;

    return GifFile;
}
/******************************************************************************
* Miscellaneous utility functions                          *
******************************************************************************/

MS_U32 MstBitSize(MS_U32 n)
/* return smallest bitfield size n will fit in */
{
    MS_U32    i;

    for (i = 1; i <= 8; i++)
    if ((1 << i) >= n)
        break;
    return(i);
}

ColorMapObject *MstMakeMapObject(MS_U32 ColorCount, const GifColorType *ColorMap)
/*
 * Allocate a color map of given size; initialize with contents of
 * ColorMap if that pointer is non-NULL.
 */
{
    ColorMapObject *Object;

    if (ColorCount != (1 << MstBitSize(ColorCount)))
    return((ColorMapObject *)NULL);
    //console_printf("MstMakeMapObject=%d\n",ColorCount);
    Object = &CMObject[obj_idx];//(ColorMapObject *)malloc(sizeof(ColorMapObject));
    //Object = (ColorMapObject *)malloc(sizeof(ColorMapObject));

    //if (Object == (ColorMapObject *)NULL)
    //    return((ColorMapObject *)NULL);
    //console_printf("Object=%x\n",Object);

    Object->Colors = &ColorTyp[obj_idx];//(GifColorType *)calloc(ColorCount, sizeof(GifColorType));
    //Object->Colors = (GifColorType *)calloc(ColorCount, sizeof(GifColorType));
    //console_printf("Object->Colors=%x\n",Object->Colors);
    if (Object->Colors == (GifColorType *)NULL)
        return((ColorMapObject *)NULL);
    Object->ColorCount = ColorCount;
    Object->BitsPerPixel = MstBitSize(ColorCount);

    if (ColorMap)
    memcpy((char *)Object->Colors,
           (char *)ColorMap, ColorCount * sizeof(GifColorType));
    obj_idx++;
    return(Object);
}
void MstFreeMapObject(ColorMapObject *Object)
/*
 * Free a color map object
 */
{
    obj_idx--;
    //free(Object->Colors);
    //free(Object);
}

/******************************************************************************
*   This routine should be called before any other DGif calls. Note that      *
* this routine is called automatically from DGif file open routines.          *
******************************************************************************/
MS_U32 MstDGifGetScreenDesc(GifFileType *GifFile)
{
    MS_U32 BitsPerPixel;
    GifByteType Buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    /* Put the screen descriptor into the file: */
    if (DGifGetWord(GifFile, &GifFile->SWidth) == GIF_ERROR ||
        DGifGetWord(GifFile, &GifFile->SHeight) == GIF_ERROR)
    return GIF_ERROR;

    if (READ( GifFile, Buf, 3) != 3) {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return GIF_ERROR;
    }

    GifFile->SColorResolution = (((Buf[0] & 0x70) + 1) >> 4) + 1;
    BitsPerPixel = (Buf[0] & 0x07) + 1;
    GifFile->SBackGroundColor = Buf[1];
    GifFile->AspectRatio = Buf[2];
    GifFile->SColorMap = NULL;

    if (Buf[0] & 0x80)
    {/* Do we have global color map? */

        GifFile->SColorMap = MstMakeMapObject(1 << BitsPerPixel, NULL);
        if(GifFile->SColorMap)
        {
            GifFile->SColorMap->Colors = (GifColorType *)(GifFile->UserData);
            GifFile->UserData += 3 * GifFile->SColorMap->ColorCount;
        }
    }

    return GIF_OK;
}
/******************************************************************************
*   Get 2 bytes (word) from the given file:                      *
******************************************************************************/
static MS_U32 DGifGetWord(GifFileType *GifFile, MS_U32 *Word)
{
    MS_U8 c[2] = {0, 0};

    if (READ(GifFile,c, 2) != 2) {
    _MstGifError = D_GIF_ERR_READ_FAILED;
    return GIF_ERROR;
    }

    *Word = (((MS_U32) c[1]) << 8) + c[0];
    return GIF_OK;
}

/******************************************************************************
*   This routine should be called before any attemp to read an image.         *
******************************************************************************/
MS_U32 MstDGifGetRecordType(GifFileType *GifFile, GifRecordType *Type)
{
    GifByteType Buf = 0;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    if (!IS_READABLE(Private)) {
    /* This file was NOT open for reading: */
    _MstGifError = D_GIF_ERR_NOT_READABLE;
    return GIF_ERROR;
    }

    if (READ( GifFile, &Buf, 1) != 1) {
    _MstGifError = D_GIF_ERR_READ_FAILED;
    return GIF_ERROR;
    }

    switch (Buf) {
    case ',':
        *Type = IMAGE_DESC_RECORD_TYPE;
        break;
    case '!':
        *Type = EXTENSION_RECORD_TYPE;
        break;
    case ';':
        *Type = TERMINATE_RECORD_TYPE;
        break;
    default:
        *Type = UNDEFINED_RECORD_TYPE;
        _MstGifError = D_GIF_ERR_WRONG_RECORD;
        return GIF_ERROR;
    }

    return GIF_OK;
}

/******************************************************************************
*   This routine should be called before any attemp to read an image.         *
*   Note it is assumed the Image desc. header (',') has been read.          *
******************************************************************************/
MS_U32 MstDGifGetImageDesc(GifFileType *GifFile)
{
    MS_U32 BitsPerPixel;
    GifByteType Buf[3] = {0, 0, 0};
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    if (!IS_READABLE(Private))
    {
        /* This file was NOT open for reading: */
        _MstGifError = D_GIF_ERR_NOT_READABLE;
        return GIF_ERROR;
    }

    if (DGifGetWord(GifFile, &GifFile->Image.Left) == GIF_ERROR ||
        DGifGetWord(GifFile, &GifFile->Image.Top) == GIF_ERROR ||
        DGifGetWord(GifFile, &GifFile->Image.Width) == GIF_ERROR ||
        DGifGetWord(GifFile, &GifFile->Image.Height) == GIF_ERROR)
        return GIF_ERROR;

    if (READ(GifFile,Buf, 1) != 1)
    {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return GIF_ERROR;
    }

    BitsPerPixel = (Buf[0] & 0x07) + 1;
    GifFile->Image.Interlace = (Buf[0] & 0x40)>>6;

    if (Buf[0] & 0x80)
    {
        /* Does this image have local color map? */
        GifFile->Image.ColorMap = &LoObject;
        GifFile->Image.ColorMap->ColorCount = (1 << BitsPerPixel);
        GifFile->Image.ColorMap->BitsPerPixel = BitsPerPixel;
    }
    else
    {
        GifFile->Image.ColorMap = NULL;
    }
    return GIF_OK;
}
/******************************************************************************
*   Get an extension block (see GIF manual) from gif file. This routine only  *
* returns the first data block, and MstDGifGetExtensionNext shouldbe called      *
* after this one until NULL extension is returned.                  *
*   The Extension should NOT be freed by the user (not dynamically allocated).*
*   Note it is assumed the Extension desc. header ('!') has been read.          *
******************************************************************************/
MS_U32 MstDGifGetExtension(GifFileType *GifFile, MS_U32 *ExtCode,
                            GifByteType **Extension)
{
    GifByteType Buf = 0;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;
    MS_U32 ret;

    if (!IS_READABLE(Private))
    {
        /* This file was NOT open for reading: */
        _MstGifError = D_GIF_ERR_NOT_READABLE;
        return GIF_ERROR;
    }

    if (READ(GifFile,&Buf, 1) != 1)
    {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return GIF_ERROR;
    }
    *ExtCode = Buf;

    ret = MstDGifGetExtensionNext(GifFile, Extension);

    if (ret == GIF_OK && Buf == 0xF9)
    {
        GifFile->GraphicCtrl.enable          = 1;
        GifFile->GraphicCtrl.disposal_method = ((*Extension)[1] >> 2) & 0x7;
        GifFile->GraphicCtrl.trans_bit       = ((*Extension)[1]) & 0x1;
        GifFile->GraphicCtrl.time            = (*Extension)[2] + ((*Extension)[3] << 8);
        GifFile->GraphicCtrl.trans_idx       = (*Extension)[4];
    }
    else if(ret == GIF_OK && Buf == 0xFF) // for MGIF testing
    {
        #if 0
        MS_U32 i = 0;
        //dump extension block data
        console_printf("##################EXTENSION BLOCK######################################\n");
        for(i = 1;i < ((*Extension)[0]-1);i++)
        {
            console_printf("%x,",(*Extension)[i]);
        }
        console_printf("\n#######################################################################\n");
        #endif
        GifFile->isMGIF = 1;
    }

    return ret;
}

/******************************************************************************
*   Get a following extension block (see GIF manual) from gif file. This      *
* routine should be called until NULL Extension is returned.              *
*   The Extension should NOT be freed by the user (not dynamically allocated).*
******************************************************************************/
MS_U32 MstDGifGetExtensionNext(GifFileType *GifFile, GifByteType **Extension)
{
    GifByteType Buf = 0;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;


    if (READ(GifFile,&Buf, 1) != 1)
    {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return GIF_ERROR;
    }
    if (Buf > 0)
    {
        *Extension = Private->Buf;  /* Use private unused buffer. */
        (*Extension)[0] = Buf;      /* Pascal strings notation (pos. 0 is len.). */
        if (READ(GifFile,&((*Extension)[1]), Buf) != Buf)
        {
            _MstGifError = D_GIF_ERR_READ_FAILED;
            return GIF_ERROR;
        }
    }
    else
        *Extension = NULL;

    return GIF_OK;
}
/******************************************************************************
*   Setup the LZ decompression for this image:                      *
******************************************************************************/
#if 0
static MS_U32 DGifSetupDecompress(GifFileType *GifFile)
{
    MS_U32 i, BitsPerPixel;
    GifByteType CodeSize;
    MS_U32 *Prefix;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    READ(GifFile,&CodeSize, 1);    /* Read Code size from file. */
    BitsPerPixel = CodeSize;

    Private->Buf[0] = 0;                  /* Input Buffer empty. */
    Private->BitsPerPixel = BitsPerPixel;
    Private->ClearCode = (1 << BitsPerPixel);
    Private->EOFCode = Private->ClearCode + 1;
    Private->RunningCode = Private->EOFCode + 1;
    Private->RunningBits = BitsPerPixel + 1;     /* Number of bits per code. */
    Private->MaxCode1 = 1 << Private->RunningBits;     /* Max. code + 1. */
    Private->StackPtr = 0;            /* No pixels on the pixel stack. */
    Private->LastCode = NO_SUCH_CODE;
    Private->CrntShiftState = 0;    /* No information in CrntShiftDWord. */
    Private->CrntShiftDWord = 0;

    Prefix = Private->Prefix;
    for (i = 0; i <= LZ_MAX_CODE; i++)
        Prefix[i] = NO_SUCH_CODE;

    return GIF_OK;
}
#endif

/******************************************************************************
*  Get one full scanned line (Line) of length LineLen from GIF file.          *
******************************************************************************/
#ifdef SW_DEC
MS_U32 MstDGifGetLine(GifFileType *GifFile, GifPixelType *Line, MS_U32 LineLen)
{
    GifByteType *Dummy;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    if (!IS_READABLE(Private))
    {
        /* This file was NOT open for reading: */
        _MstGifError = D_GIF_ERR_NOT_READABLE;
        return GIF_ERROR;
    }

    if (!LineLen) LineLen = GifFile->Image.Width;

#if defined(__MSDOS__) || defined(__GNUC__)
    if ((Private->PixelCount -= LineLen) > 0xffff0000UL) {
#else
    if ((Private->PixelCount -= LineLen) > 0xffff0000) {
#endif /* __MSDOS__ */
    _MstGifError = D_GIF_ERR_DATA_TOO_BIG;
    return GIF_ERROR;
    }

    if (DGifDecompressLine(GifFile, Line, LineLen) == GIF_OK) {
    if (Private->PixelCount == 0) {
        /* We probably would not be called any more, so lets clean          */
        /* everything before we return: need to flush out all rest of    */
        /* image until empty block (size 0) detected. We use GetCodeNext.*/
        do if (MstDGifGetCodeNext(GifFile, &Dummy) == GIF_ERROR)
        return GIF_ERROR;
        while (Dummy != NULL);
    }
    return GIF_OK;
    }
    else
    return GIF_ERROR;
}

/******************************************************************************
* Put one pixel (Pixel) into GIF file.                          *
******************************************************************************/
MS_U32 MstDGifGetPixel(GifFileType *GifFile, GifPixelType Pixel)
{
    GifByteType *Dummy;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    if (!IS_READABLE(Private)) {
    /* This file was NOT open for reading: */
    _MstGifError = D_GIF_ERR_NOT_READABLE;
    return GIF_ERROR;
    }

#if defined(__MSDOS__) || defined(__GNUC__)
    if (--Private->PixelCount > 0xffff0000UL)
#else
    if (--Private->PixelCount > 0xffff0000)
#endif /* __MSDOS__ */
    {
    _MstGifError = D_GIF_ERR_DATA_TOO_BIG;
    return GIF_ERROR;
    }

    if (DGifDecompressLine(GifFile, &Pixel, 1) == GIF_OK) {
    if (Private->PixelCount == 0) {
        /* We probably would not be called any more, so lets clean          */
        /* everything before we return: need to flush out all rest of    */
        /* image until empty block (size 0) detected. We use GetCodeNext.*/
        do if (MstDGifGetCodeNext(GifFile, &Dummy) == GIF_ERROR)
        return GIF_ERROR;
        while (Dummy != NULL);
    }
    return GIF_OK;
    }
    else
    return GIF_ERROR;
}



/******************************************************************************
*   Get the image code in compressed form.  his routine can be called if the  *
* information needed to be piped out as is. Obviously this is much faster     *
* than decoding and encoding again. This routine should be followed by calls  *
* to MstDGifGetCodeNext, until NULL block is returned.                  *
*   The block should NOT be freed by the user (not dynamically allocated).    *
******************************************************************************/
MS_U32 MstDGifGetCode(GifFileType *GifFile, MS_U32 *CodeSize, GifByteType **CodeBlock)
{
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    if (!IS_READABLE(Private)) {
    /* This file was NOT open for reading: */
    _MstGifError = D_GIF_ERR_NOT_READABLE;
    return GIF_ERROR;
    }

    *CodeSize = Private->BitsPerPixel;

    return MstDGifGetCodeNext(GifFile, CodeBlock);
}

/******************************************************************************
*   Continue to get the image code in compressed form. This routine should be *
* called until NULL block is returned.                          *
*   The block should NOT be freed by the user (not dynamically allocated).    *
******************************************************************************/
MS_U32 MstDGifGetCodeNext(GifFileType *GifFile, GifByteType **CodeBlock)
{
    GifByteType Buf;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    if (READ(GifFile,&Buf, 1) != 1) {
    _MstGifError = D_GIF_ERR_READ_FAILED;
    return GIF_ERROR;
    }

    if (Buf > 0) {
    *CodeBlock = Private->Buf;           /* Use private unused buffer. */
    (*CodeBlock)[0] = Buf;  /* Pascal strings notation (pos. 0 is len.). */
    if (READ(GifFile,&((*CodeBlock)[1]), Buf) != Buf) {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return GIF_ERROR;
    }
    }
    else {
    *CodeBlock = NULL;
    Private->Buf[0] = 0;           /* Make sure the buffer is empty! */
    Private->PixelCount = 0;   /* And local info. indicate image read. */
    }

    return GIF_OK;
}

/******************************************************************************
*   The LZ decompression routine:                          *
*   This version decompress the given gif file into Line of length LineLen.   *
*   This routine can be called few times (one per scan line, for example), in *
* order the complete the whole image.                          *
******************************************************************************/
static MS_U32 DGifDecompressLine(GifFileType *GifFile, GifPixelType *Line,
                                MS_U32 LineLen)
{
    MS_U32 i = 0, j, CrntCode, EOFCode, ClearCode, CrntPrefix, LastCode, StackPtr;
    GifByteType *Stack, *Suffix;
    MS_U32 *Prefix;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    StackPtr = Private->StackPtr;
    Prefix = Private->Prefix;
    Suffix = Private->Suffix;
    Stack = Private->Stack;
    EOFCode = Private->EOFCode;
    ClearCode = Private->ClearCode;
    LastCode = Private->LastCode;

    if (StackPtr != 0) {
    /* Let pop the stack off before continueing to read the gif file: */
    while (StackPtr != 0 && i < LineLen) Line[i++] = Stack[--StackPtr];
    }

    while (i < LineLen) {                /* Decode LineLen items. */
    if (DGifDecompressInput(GifFile, &CrntCode) == GIF_ERROR)
            return GIF_ERROR;
//printf("crntcode=%02x\n",CrntCode);
//printf("Lastcode=%02x",    LastCode);
    if (CrntCode == EOFCode) {
        /* Note however that usually we will not be here as we will stop */
        /* decoding as soon as we got all the pixel, or EOF code will    */
        /* not be read at all, and MstDGifGetLine/Pixel clean everything.   */
        if (i != LineLen - 1 || Private->PixelCount != 0) {
        _MstGifError = D_GIF_ERR_EOF_TOO_SOON;
        return GIF_ERROR;
        }
        i++;
    }
    else if (CrntCode == ClearCode) {
        /* We need to start over again: */
        for (j = 0; j <= LZ_MAX_CODE; j++) Prefix[j] = NO_SUCH_CODE;
        Private->RunningCode = Private->EOFCode + 1;
        Private->RunningBits = Private->BitsPerPixel + 1;
        Private->MaxCode1 = 1 << Private->RunningBits;
        LastCode = Private->LastCode = NO_SUCH_CODE;
    }
    else {
        /* Its regular code - if in pixel range simply add it to output  */
        /* stream, otherwise trace to codes linked list until the prefix */
        /* is in pixel range:                         */
        if (CrntCode < ClearCode) {
        /* This is simple - its pixel scalar, so add it to output:   */
        Line[i++] = CrntCode;
        }
        else {
        /* Its a code to needed to be traced: trace the linked list  */
        /* until the prefix is a pixel, while pushing the suffix     */
        /* pixels on our stack. If we done, pop the stack in reverse */
        /* (thats what stack is good for!) order to output.         */
        if (Prefix[CrntCode] == NO_SUCH_CODE) {
            /* Only allowed if CrntCode is exactly the running code: */
            /* In that case CrntCode = XXXCode, CrntCode or the         */
            /* prefix code is last code and the suffix char is         */
            /* exactly the prefix of last code!                 */
            if (CrntCode == Private->RunningCode - 2) {
            CrntPrefix = LastCode;
            Suffix[Private->RunningCode - 2] =
            Stack[StackPtr++] = DGifGetPrefixChar(Prefix,
                            LastCode, ClearCode);
            }
            else {
            _MstGifError = D_GIF_ERR_IMAGE_DEFECT;
            return GIF_ERROR;
            }
        }
        else
            CrntPrefix = CrntCode;

        /* Now (if image is O.K.) we should not get an NO_SUCH_CODE  */
        /* During the trace. As we might loop forever, in case of    */
        /* defective image, we count the number of loops we trace    */
        /* and stop if we got LZ_MAX_CODE. obviously we can not      */
        /* loop more than that.                         */
        j = 0;
        while (j++ <= LZ_MAX_CODE &&
               CrntPrefix > ClearCode &&
               CrntPrefix <= LZ_MAX_CODE) {
            Stack[StackPtr++] = Suffix[CrntPrefix];
            CrntPrefix = Prefix[CrntPrefix];
        }
        if (j >= LZ_MAX_CODE || CrntPrefix > LZ_MAX_CODE) {
            _MstGifError = D_GIF_ERR_IMAGE_DEFECT;
            return GIF_ERROR;
        }
        /* Push the last character on stack: */
        Stack[StackPtr++] = CrntPrefix;

        /* Now lets pop all the stack into output: */
        console_printf("----- put %d [0x%02x]\n ",StackPtr,CrntPrefix);
        while (StackPtr != 0 && i < LineLen)
            Line[i++] = Stack[--StackPtr];
        }
        if (LastCode != NO_SUCH_CODE) {
        Prefix[Private->RunningCode - 2] = LastCode;
        console_printf("add code %d prefix=%d\n", Private->RunningCode - 2,LastCode);

        if (CrntCode == Private->RunningCode - 2) {
            /* Only allowed if CrntCode is exactly the running code: */
            /* In that case CrntCode = XXXCode, CrntCode or the         */
            /* prefix code is last code and the suffix char is         */
            /* exactly the prefix of last code!                 */
            Suffix[Private->RunningCode - 2] =
            DGifGetPrefixChar(Prefix, LastCode, ClearCode);
        }
        else {
            Suffix[Private->RunningCode - 2] =
            DGifGetPrefixChar(Prefix, CrntCode, ClearCode);
        }
        }
        LastCode = CrntCode;
//printf("Lastcode=%02x",    LastCode);
    }
    }

    Private->LastCode = LastCode;
    Private->StackPtr = StackPtr;

    return GIF_OK;
}

/******************************************************************************
* Routine to trace the Prefixes linked list until we get a prefix which is    *
* not code, but a pixel value (less than ClearCode). Returns that pixel value.*
* If image is defective, we might loop here forever, so we limit the loops to *
* the maximum possible if image O.k. - LZ_MAX_CODE times.              *
******************************************************************************/
static MS_U32 DGifGetPrefixChar(MS_U32 *Prefix, MS_U32 Code, MS_U32 ClearCode)
{
    MS_U32 i = 0;

    while (Code > ClearCode && i++ <= LZ_MAX_CODE) Code = Prefix[Code];
    return Code;
}

/******************************************************************************
*   Interface for accessing the LZ codes directly. Set Code to the real code  *
* (12bits), or to -1 if EOF code is returned.                      *
******************************************************************************/
MS_U32 MstDGifGetLZCodes(GifFileType *GifFile, MS_U32 *Code)
{
    GifByteType *CodeBlock;
    GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

    if (!IS_READABLE(Private)) {
    /* This file was NOT open for reading: */
    _MstGifError = D_GIF_ERR_NOT_READABLE;
    return GIF_ERROR;
    }

    if (DGifDecompressInput(GifFile, Code) == GIF_ERROR)
    return GIF_ERROR;

    if (*Code == Private->EOFCode) {
    /* Skip rest of codes (hopefully only NULL terminating block): */
    do if (MstDGifGetCodeNext(GifFile, &CodeBlock) == GIF_ERROR)
            return GIF_ERROR;
    while (CodeBlock != NULL);

    *Code = -1;
    }
    else if (*Code == Private->ClearCode) {
    /* We need to start over again: */
    Private->RunningCode = Private->EOFCode + 1;
    Private->RunningBits = Private->BitsPerPixel + 1;
    Private->MaxCode1 = 1 << Private->RunningBits;
    }

    return GIF_OK;
}

/******************************************************************************
*   The LZ decompression input routine:                          *
*   This routine is responsable for the decompression of the bit stream from  *
* 8 bits (bytes) packets, into the real codes.                      *
*   Returns GIF_OK if read succesfully.                          *
******************************************************************************/
static MS_U32 DGifDecompressInput(GifFileType *GifFile, MS_U32 *Code)
{
    GifFilePrivateType *Private = (GifFilePrivateType *)GifFile->Private;

    GifByteType NextByte;
    static MS_U32 CodeMasks[] = {
    0x0000, 0x0001, 0x0003, 0x0007,
    0x000f, 0x001f, 0x003f, 0x007f,
    0x00ff, 0x01ff, 0x03ff, 0x07ff,
    0x0fff
    };

    console_printf("\n DGifDecompressInput :");


    while (Private->CrntShiftState < Private->RunningBits) {
    /* Needs to get more bytes from input stream for next code: */
    if (DGifBufferedInput(GifFile, Private->Buf, &NextByte)
        == GIF_ERROR) {
        return GIF_ERROR;
    }

    console_printf(" %02x ",NextByte);

    Private->CrntShiftDWord |=
        ((MS_U32) NextByte) << Private->CrntShiftState;
    Private->CrntShiftState += 8;
    }
    *Code = Private->CrntShiftDWord & CodeMasks[Private->RunningBits];

    Private->CrntShiftDWord >>= Private->RunningBits;
    Private->CrntShiftState -= Private->RunningBits;

    /* If code cannt fit into RunningBits bits, must raise its size. Note */
    /* however that codes above 4095 are used for special signaling.      */
    if (++Private->RunningCode > Private->MaxCode1 &&
    Private->RunningBits < LZ_BITS) {
    Private->MaxCode1 <<= 1;
    Private->RunningBits++;
    console_printf("run bits=%d\n",Private->RunningBits);
    }

    console_printf("\n return code = 0x%04x \n",*Code);

    return GIF_OK;
}

/******************************************************************************
*   This routines read one gif data block at a time and buffers it internally *
* so that the decompression routine could access it.                  *
*   The routine returns the next byte from its internal buffer (or read next  *
* block in if buffer empty) and returns GIF_OK if succesful.              *
******************************************************************************/
static MS_U32 DGifBufferedInput(GifFileType *GifFile, GifByteType *Buf,
                              GifByteType *NextByte)
{
    if (Buf[0] == 0) {
    /* Needs to read the next buffer - this one is empty: */
    if (READ(GifFile, Buf, 1) != 1)
    {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return GIF_ERROR;
    }
    if (READ(GifFile,&Buf[1], Buf[0]) != Buf[0])
    {
        _MstGifError = D_GIF_ERR_READ_FAILED;
        return GIF_ERROR;
    }
    *NextByte = Buf[1];
    Buf[1] = 2;       /* We use now the second place as last char read! */
    Buf[0]--;
    }
    else {
    *NextByte = Buf[Buf[1]++];
    Buf[0]--;
    }

    return GIF_OK;
}
MS_U32 AddExtensionBlock(SavedImage *New, MS_U32 Len, char ExtData[])
{
    ExtensionBlock    *ep;

    if (New->ExtensionBlocks == NULL)
    New->ExtensionBlocks = (ExtensionBlock *)malloc(sizeof(ExtensionBlock));
    else
    New->ExtensionBlocks =
        (ExtensionBlock *)realloc(New->ExtensionBlocks,
              sizeof(ExtensionBlock) * (New->ExtensionBlockCount + 1));

    if (New->ExtensionBlocks == NULL)
    return(GIF_ERROR);

    ep = &New->ExtensionBlocks[New->ExtensionBlockCount++];

    if ((ep->Bytes = (char *)malloc(ep->ByteCount = Len)) == NULL)
    return(GIF_ERROR);

    if (ExtData) {
        memcpy(ep->Bytes, ExtData, Len);
        ep->Function = New->Function;
    }

    return(GIF_OK);
}

void FreeExtension(SavedImage *Image)
{
    ExtensionBlock    *ep;

    for (ep = Image->ExtensionBlocks;
     ep < Image->ExtensionBlocks + Image->ExtensionBlockCount;
     ep++)
    (void) free((char *)ep->Bytes);
    //free((char *)Image->ExtensionBlocks);
    Image->ExtensionBlocks = NULL;
}
void FreeSavedImages(GifFileType *GifFile)
{
    SavedImage    *sp;

    for (sp = GifFile->SavedImages;
     sp < GifFile->SavedImages + GifFile->ImageCount;
     sp++)
    {
    if (sp->ImageDesc.ColorMap)
        MstFreeMapObject(sp->ImageDesc.ColorMap);

    if (sp->RasterBits)
        free((char *)sp->RasterBits);

    //if (sp->ExtensionBlocks)
        //(sp);
    }
    //free((char *) GifFile->SavedImages);
    GifFile->SavedImages=NULL;
}

/******************************************************************************
* This routine reads an entire GIF into core, hanging all its state info off  *
* the GifFileType pointer.  Call DGifOpenFileName() or MstDGifOpenFileHandle()   *
* first to initialize I/O.  Its inverse is EGifSpew().                  *
*
 ******************************************************************************/
MS_U32 MstDGifSlurp(GifFileType *GifFile)
{
    MS_U32 ImageSize;
    MS_U32 gif_err = 0;
    GifRecordType RecordType;
    SavedImage *sp;
    GifByteType *ExtData;
    SavedImage temp_save;


    temp_save.ExtensionBlocks=NULL;
    temp_save.ExtensionBlockCount=0;

    do {
    if (MstDGifGetRecordType(GifFile, &RecordType) == GIF_ERROR)
    {
        gif_err = 1;
        break;
    }

    switch (RecordType) {
        case IMAGE_DESC_RECORD_TYPE:
        if (MstDGifGetImageDesc(GifFile) == GIF_ERROR)
        {
            gif_err = 1;
            break;
        }

        sp = &GifFile->SavedImages[GifFile->ImageCount-1];
        ImageSize = sp->ImageDesc.Width * sp->ImageDesc.Height;

        sp->RasterBits
            = (char *) malloc(ImageSize * sizeof(GifPixelType));

        if (MstDGifGetLine(GifFile, (GifPixelType *) sp->RasterBits, ImageSize)
            == GIF_ERROR)
        {
            gif_err = 1;
            break;
        }

        if (temp_save.ExtensionBlocks) {
            sp->ExtensionBlocks = temp_save.ExtensionBlocks;
            sp->ExtensionBlockCount = temp_save.ExtensionBlockCount;

            temp_save.ExtensionBlocks = NULL;
            temp_save.ExtensionBlockCount=0;

        /* FIXME: The following is wrong.  It is left in only for backwards
         * compatibility.  Someday it should go away.  Use the
         * sp->ExtensionBlocks->Function variable instead.
         */
            sp->Function = sp->ExtensionBlocks[0].Function;

        }

        break;

        case EXTENSION_RECORD_TYPE:
        if (MstDGifGetExtension(GifFile,&temp_save.Function,&ExtData)==GIF_ERROR)
        {
            gif_err = 1;
            break;
        }
        while (ExtData != NULL) {

            /* Create an extension block with our data */
            if (AddExtensionBlock(&temp_save, ExtData[0],(char *) &ExtData[1])
               == GIF_ERROR)
            {
                gif_err = 1;
                break;
            }


            if (MstDGifGetExtensionNext(GifFile, &ExtData) == GIF_ERROR)
            {
                gif_err = 1;
                break;
            }
            temp_save.Function = 0;
        }
        break;

        case TERMINATE_RECORD_TYPE:
        break;

        default:    /* Should be trapped by MstDGifGetRecordType */
        break;
    }
    } while (RecordType != TERMINATE_RECORD_TYPE);

    /* Just in case the Gif has an extension block without an associated
     * image... (Should we save this into a savefile structure with no image
     * instead?  Have to check if the present writing code can handle that as
     * well....
     */
    if (temp_save.ExtensionBlocks)
        FreeExtension(&temp_save);

    if (gif_err)
        return(GIF_ERROR);
    else
        return(GIF_OK);



}
/******************************************************************************
*   This routine should be called last, to close the GIF file.              *
******************************************************************************/
MS_U32 MstDGifCloseFile(GifFileType *GifFile)
{
    GifFilePrivateType *Private;
    //FILE *File;

    if (GifFile == NULL) return GIF_ERROR;

    Private = (GifFilePrivateType *) GifFile->Private;

    if (!IS_READABLE(Private)) {
    /* This file was NOT open for reading: */
    _MstGifError = D_GIF_ERR_NOT_READABLE;
    return GIF_ERROR;
    }

    //File = Private->File;

    if (GifFile->Image.ColorMap)
    {
    MstFreeMapObject(GifFile->Image.ColorMap);
        GifFile->Image.ColorMap = NULL;
    }

    if (GifFile->SColorMap)
    {
    MstFreeMapObject(GifFile->SColorMap);
    GifFile->SColorMap = NULL;
    }

    if (Private)
    {
    free((char *) Private);
    Private = NULL;
    }

    if (GifFile->SavedImages)
    {
    FreeSavedImages(GifFile);
    GifFile = NULL;
    }

    return GIF_OK;
}

#endif
