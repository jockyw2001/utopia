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
/*****************************************************************************
*   "Gif-Lib" - Yet another gif library.                     *
*                                         *
* Written by:  Gershon Elber            IBM PC Ver 0.1,    Jun. 1989    *
******************************************************************************
* Handle error reporting for the GIF library.                     *
******************************************************************************
* History:                                     *
* 17 Jun 89 - Version 1.0 by Gershon Elber.                     *
*****************************************************************************/
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
//#include <common.h>
//#include <malloc.h>
#include "MsCommon.h"
#include "gif_lib.h"

#define PROGRAM_NAME    "GIF_LIBRARY"

MS_U32 _MstGifError = 0;

#ifdef SYSV
static char *VersionStr =
        "Gif library module,\t\tEric S. Raymond\n\
    (C) Copyright 1997 Eric S. Raymond\n";
#else
#if 0
static char *VersionStr =
    PROGRAM_NAME
    "    IBMPC "
    GIF_LIB_VERSION
    "    Eric S. Raymond,    "
    __DATE__ ",   " __TIME__ "\n"
    "(C) Copyright 1997 Eric S. Raymond\n";
#endif
#endif /* SYSV */

/*****************************************************************************
* Return the last GIF error (0 if none) and reset the error.             *
*****************************************************************************/
MS_U32 MstGifLastError(void)
{
    MS_U32 i = _MstGifError;

    _MstGifError = 0;

    return i;
}

/*****************************************************************************
* Print the last GIF error to stderr.                         *
*****************************************************************************/
void PrintGifError(void)
{
    char *Err;

    switch(_MstGifError) {
    case E_GIF_ERR_OPEN_FAILED:
        Err = "Failed to open given file";
        break;
    case E_GIF_ERR_WRITE_FAILED:
        Err = "Failed to Write to given file";
        break;
    case E_GIF_ERR_HAS_SCRN_DSCR:
        Err = "Screen Descriptor already been set";
        break;
    case E_GIF_ERR_HAS_IMAG_DSCR:
        Err = "Image Descriptor is still active";
        break;
    case E_GIF_ERR_NO_COLOR_MAP:
        Err = "Neither Global Nor Local color map";
        break;
    case E_GIF_ERR_DATA_TOO_BIG:
        Err = "#Pixels bigger than Width * Height";
        break;
    case E_GIF_ERR_NOT_ENOUGH_MEM:
        Err = "Fail to allocate required memory";
        break;
    case E_GIF_ERR_DISK_IS_FULL:
        Err = "Write failed (disk full?)";
        break;
    case E_GIF_ERR_CLOSE_FAILED:
        Err = "Failed to close given file";
        break;
    case E_GIF_ERR_NOT_WRITEABLE:
        Err = "Given file was not opened for write";
        break;
    case D_GIF_ERR_OPEN_FAILED:
        Err = "Failed to open given file";
        break;
    case D_GIF_ERR_READ_FAILED:
        Err = "Failed to Read from given file";
        break;
    case D_GIF_ERR_NOT_GIF_FILE:
        Err = "Given file is NOT GIF file";
        break;
    case D_GIF_ERR_NO_SCRN_DSCR:
        Err = "No Screen Descriptor detected";
        break;
    case D_GIF_ERR_NO_IMAG_DSCR:
        Err = "No Image Descriptor detected";
        break;
    case D_GIF_ERR_NO_COLOR_MAP:
        Err = "Neither Global Nor Local color map";
        break;
    case D_GIF_ERR_WRONG_RECORD:
        Err = "Wrong record type detected";
        break;
    case D_GIF_ERR_DATA_TOO_BIG:
        Err = "#Pixels bigger than Width * Height";
        break;
    case D_GIF_ERR_NOT_ENOUGH_MEM:
        Err = "Fail to allocate required memory";
        break;
    case D_GIF_ERR_CLOSE_FAILED:
        Err = "Failed to close given file";
        break;
    case D_GIF_ERR_NOT_READABLE:
        Err = "Given file was not opened for read";
        break;
    case D_GIF_ERR_IMAGE_DEFECT:
        Err = "Image is defective, decoding aborted";
        break;
    case D_GIF_ERR_EOF_TOO_SOON:
        Err = "Image EOF detected, before image complete";
        break;
    default:
        Err = NULL;
        break;
    }
    if (Err != NULL)
    {
    //fprintf(stderr, "\nGIF-LIB error: %s.\n", Err);
    console_printf("\nGIF-LIB error: %s.\n", Err);
    }
    else
    {
    //fprintf(stderr, "\nGIF-LIB undefined error %d.\n", _MstGifError);
    console_printf("\nGIF-LIB undefined error %td.\n", (ptrdiff_t)_MstGifError);
    }
}
