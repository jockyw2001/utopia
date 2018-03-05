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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    jpeg_file.c
/// @brief  JPEG decoder file system
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "jpegmain.h"
#include "jpeg_cmodel_io.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
VOID JPEG_FILE_constructor( JPEG_FILE_FileSystem_t *thisPtr )
{
    thisPtr->Pfile = NULL;
    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
}

VOID JPEG_FILE_destructor( JPEG_FILE_FileSystem_t *thisPtr )
{
    thisPtr->close( thisPtr );
}

MS_BOOL JPEG_FILE_open( const char *Pfilename, JPEG_FILE_FileSystem_t *thisPtr )
{
    thisPtr->close( thisPtr );

    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
    thisPtr->Pfile = fopen( Pfilename, "rb" );
    if ( !thisPtr->Pfile )
    {
        return ( TRUE );
    }

    return ( FALSE );
}

VOID JPEG_FILE_close( JPEG_FILE_FileSystem_t *thisPtr )
{
    if ( thisPtr->Pfile )
    {
        fclose( thisPtr->Pfile );
        thisPtr->Pfile = NULL;
    }

    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
}

MS_S32 JPEG_FILE_read( MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag, JPEG_FILE_FileSystem_t *thisPtr)
{
    MS_U32 bytes_read;

    if ( !thisPtr->Pfile )
    {
        return ( -1 );
    }

    if ( thisPtr->eof_flag )
    {
        *Peof_flag = TRUE;
        return ( 0 );
    }

    if ( thisPtr->error_flag )
    {
        return ( -1 );
    }


    bytes_read = fread( Pbuf, 1, max_bytes_to_read, thisPtr->Pfile );

    if ( bytes_read < max_bytes_to_read )
    {
        if ( ferror( thisPtr->Pfile ) )
        {
            thisPtr->error_flag = TRUE;
            return ( -1 );
        }

        thisPtr->eof_flag = TRUE;
        *Peof_flag = TRUE;
    }

    return ( bytes_read );
}

MS_BOOL JPEG_FILE_get_error_status( JPEG_FILE_FileSystem_t *thisPtr ) //not used
{
    return ( thisPtr->error_flag );
}

MS_BOOL JPEG_FILE_reset( JPEG_FILE_FileSystem_t *thisPtr ) //not used
{
    if ( thisPtr->error_flag )
    {
        return ( TRUE );
    }

    fseek( thisPtr->Pfile, 0, SEEK_SET );

    thisPtr->eof_flag = FALSE;

    return ( FALSE );
}

MS_S32 JPEG_FILE_get_size( JPEG_FILE_FileSystem_t *thisPtr ) //not necessary
{
    MS_U32 size;
    MS_U32 loc;

    if ( !thisPtr->Pfile )
    {
        return ( -1 );
    }

    loc = ftell( thisPtr->Pfile );

    fseek( thisPtr->Pfile, 0, SEEK_END );

    size = ftell( thisPtr->Pfile );

    fseek( thisPtr->Pfile, loc, SEEK_SET );

    return ( size );
}

VOID JPEG_FILE_tga_constructor( JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    thisPtr->Pfile = NULL;
    thisPtr->width = thisPtr->height = thisPtr->bytes_per_pixel = thisPtr->bytes_per_line = 0;
    thisPtr->image_type = E_TGA_IMAGE_TYPE_NULL;
}

VOID JPEG_FILE_tga_destructor( JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    thisPtr->close( thisPtr );
}

MS_BOOL JPEG_FILE_tga_open( const char *Pfilename, MS_U16 _width, MS_U16 _height, JPEG_FILE_TGAImageType_e _image_type, JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    MS_U8 tga_header[18];
    MS_BOOL backwards_flag = FALSE;

    MS_ASSERT( Pfilename );

    thisPtr->close( thisPtr );

    thisPtr->Pfile = fopen( Pfilename, "wb" );
    if ( !thisPtr->Pfile )
    {
        return ( TRUE );
    }

    thisPtr->width = _width;
    thisPtr->height = _height;
    thisPtr->image_type = _image_type;

    memset( tga_header, 0, sizeof( tga_header ) );

    tga_header[12] = ( MS_U8 )( _width & 0xFF );
    tga_header[13] = ( MS_U8 )( ( _width >> 8 ) & 0xFF );
    tga_header[14] = ( MS_U8 )( _height & 0xFF );
    tga_header[15] = ( MS_U8 )( ( _height >> 8 ) & 0xFF );
    tga_header[17] = backwards_flag ? 0x00 : 0x20;

    switch ( _image_type )
    {
        case E_TGA_IMAGE_TYPE_BGR:
            {
                tga_header[2] = 2;
                tga_header[16] = 24;
                thisPtr->bytes_per_pixel = 3;
                break;
            }
        case E_TGA_IMAGE_TYPE_GREY:
            {
                tga_header[2] = 3;
                tga_header[16] = 8;
                thisPtr->bytes_per_pixel = 1;
                break;
            }
        default:
            MS_ASSERT( FALSE );
    }

    thisPtr->bytes_per_line = _width * thisPtr->bytes_per_pixel;

    if ( fwrite( tga_header, sizeof( tga_header ), 1, thisPtr->Pfile ) != 1 )
    {
        return ( TRUE );
    }

    return ( FALSE );
}

MS_BOOL JPEG_FILE_tga_close( JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    thisPtr->width = thisPtr->height = thisPtr->bytes_per_pixel = thisPtr->bytes_per_line = 0;
    thisPtr->image_type = E_TGA_IMAGE_TYPE_NULL;

    if ( thisPtr->Pfile )
    {
        MS_BOOL failed = ( fclose( thisPtr->Pfile ) == EOF );

        thisPtr->Pfile = NULL;

        return ( failed );
    }

    return ( FALSE );
}

MS_U32 JPEG_FILE_tga_writeline( MS_U8 *Pscan_line, JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    if ( !thisPtr->Pfile )
    {
        return ( TRUE );
    }

    if ( fwrite( Pscan_line, thisPtr->bytes_per_line, 1, thisPtr->Pfile ) != 1 )
    {
        return ( TRUE );
    }

    return ( FALSE );
}

