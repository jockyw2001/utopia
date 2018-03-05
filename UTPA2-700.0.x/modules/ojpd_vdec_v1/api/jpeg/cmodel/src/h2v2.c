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
//------------------------------------------------------------------------------
// h2v2.cpp
// Upsampling/colorspace conversion (H2V2, YCbCr)
// Last updated: Nov. 16, 2000
// Copyright (C) 1994-2000 Rich Geldreich
// richgel@voicenet.com
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//------------------------------------------------------------------------------

#include "jpegmain.h"
#include "apiJPEG.h"
#include "jpeg_cmodel.h"
//------------------------------------------------------------------------------
#define FIX(x, b) ((long) ((x) * (1L<<(b)) + 0.5))
//------------------------------------------------------------------------------
// YCbCr H2V2 (2x2:1:1, 6 blocks per MCU) to 24-bit RGB
// FIXME: Create all-asm version, so Intel Compiler isn't needed.

extern MS_U8 gu8Max_mcu_y_size;         /* MCU's max. Y size in pixels */
extern MS_U16 gu16Max_mcus_per_row;
extern MS_U16 gu16Mcu_lines_left;       /* total # lines left in this MCU */

void msAPI_JPEG_H2V2Convert( void )
{
    U8 row = gu8Max_mcu_y_size - gu16Mcu_lines_left;
    U8 *d0 = pgu8Scan_line_0;
    U8 *d1 = pgu8scan_line_1;
    U8 *y;
    U8 *c;
    U16 i, l, j;
//printf("H2V2::msAPI_JPEG_H2V2Convert\n");
    if ( row < 8 )
    {
        y = gpu8Sample_buf + row * 8;
    }
    else
    {
        y = gpu8Sample_buf + 64 * 2 + ( row & 7 ) * 8;
    }

    c = gpu8Sample_buf + 64 * 4 + ( row >> 1 ) * 8;

    for ( i = gu16Max_mcus_per_row; i > 0; i-- )
    {
        for ( l = 0; l < 2; l++ )
        {
            for ( j = 0; j < 8; j += 2 )
            {
                U8 cb = c[0];
                U8 cr = c[64];

                S32 rc = gs32Crr[cr];
                S32 gc = ( ( gs32Crg[cr] + gs32Cbg[cb] ) >> 16 );
                S32 bc = gs32Cbb[cb];

                U16 yy = y[j];
                d0[0] = JPEG_CMODEL_clamp( yy + rc );
                d0[1] = JPEG_CMODEL_clamp( yy + gc );
                d0[2] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[j + 1];
                d0[4] = JPEG_CMODEL_clamp( yy + rc );
                d0[5] = JPEG_CMODEL_clamp( yy + gc );
                d0[6] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[j + 8];
                d1[0] = JPEG_CMODEL_clamp( yy + rc );
                d1[1] = JPEG_CMODEL_clamp( yy + gc );
                d1[2] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[j + 8 + 1];
                d1[4] = JPEG_CMODEL_clamp( yy + rc );
                d1[5] = JPEG_CMODEL_clamp( yy + gc );
                d1[6] = JPEG_CMODEL_clamp( yy + bc );

                d0 += 8;
                d1 += 8;

                c++;
            }
            y += 64;
        }

        y += 64 * 6 - 64 * 2;
        c += 64 * 6 - 8;
    }
}

