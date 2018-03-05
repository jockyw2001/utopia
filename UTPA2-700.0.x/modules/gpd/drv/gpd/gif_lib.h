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
* In order to make life a little bit easier when using the GIF file format,   *
* this library was written, and which does all the dirty work...              *
*                                                                             *
*                                        Written by Gershon Elber,  Jun. 1989 *
*                                        Hacks by Eric S. Raymond,  Sep. 1992 *
*******************************************************************************
* History:                                                                    *
* 14 Jun 89 - Version 1.0 by Gershon Elber.                                   *
*  3 Sep 90 - Version 1.1 by Gershon Elber (Support for Gif89, Unique names)  *
* 15 Sep 90 - Version 2.0 by Eric S. Raymond (Changes to suoport GIF slurp)   *
* 26 Jun 96 - Version 3.0 by Eric S. Raymond (Full GIF89 support)             *
* 17 Dec 98 - Version 4.0 by Toshio Kuratomi (Fix extension writing code)     *
******************************************************************************/

#ifndef _GIF_LIB_H
#define _GIF_LIB_H

#include "gpd.h"

#define GIF_LIB_VERSION	" Version 4.0, "

//#define SW_DEC

#define	GIF_ERROR	0
#define GIF_OK		1

#ifndef SUPPORT_MGIF
#define SUPPORT_MGIF 1
#endif

#ifndef NO_SUPPORT_MGIF
#define NO_SUPPORT_MGIF 0
#endif

#ifndef TRUE
#define TRUE		1
#define FALSE		0
#endif

#ifndef NULL
#define NULL		0
#endif /* NULL */

#define GIF_STAMP "GIFVER"          /* First chars in file - GIF stamp.  */
#define GIF_STAMP_LEN sizeof(GIF_STAMP) - 1
#define GIF_VERSION_POS	3           /* Version first character in stamp. */
#define GIF87_STAMP	"GIF87a"        /* First chars in file - GIF stamp.  */
#define GIF89_STAMP	"GIF89a"        /* First chars in file - GIF stamp.  */

#define GIF_FILE_BUFFER_SIZE 16384  /* Files uses bigger buffers than usual. */

typedef	int		GifBooleanType;
typedef	MS_U8	GifPixelType;
typedef MS_U8 *	GifRowType;
typedef MS_U8	GifByteType;

#ifdef SYSV
#define VoidPtr char *
#else
#define VoidPtr void *
#endif /* SYSV */

typedef struct GifColorType {
    GifByteType Red, Green, Blue;
} GifColorType;

typedef struct ColorMapObject
{
    int	ColorCount;
    MS_U32 BitsPerPixel;
    GifColorType *Colors;		/* on malloc(3) heap */
}
ColorMapObject;

typedef struct GifImageDesc {
    MS_U32 Left, Top, Width, Height,	/* Current image dimensions. */
	Interlace;			/* Sequential/Interlaced lines. */
    ColorMapObject *ColorMap;		/* The local color map */
} GifImageDesc;

typedef struct GifGraphicCtrl {
    MS_U32 enable;
    MS_U32 disposal_method;
    MS_U32 time;
    MS_U32 trans_bit;
    MS_U32 trans_idx;
} GifGraphicCtrl;

typedef struct GifFileType {
    MS_U32 SWidth, SHeight;		/* Screen dimensions. */
	MS_U32 SColorResolution, 		/* How many colors can we generate? */
	SBackGroundColor,		/* I hope you understand this one... */
	AspectRatio;            /* from 89a*/
    ColorMapObject *SColorMap;		/* NULL if not exists. */
    MS_U32 ImageCount;			/* Number of current image */
    GifImageDesc Image;			/* Block describing current image */
    GifGraphicCtrl GraphicCtrl;
    struct SavedImage *SavedImages;	/* Use this to accumulate file state */
    MS_VIRT UserData;           /* hook to attach user data (TVT) */
    VoidPtr Private;	  		/* Don't mess with this! */
    MS_U32  GifState;     /*record the decode state*/
    MS_U32 isMGIF;                 /*record the current GIF picture is animate or static*/
} GifFileType;

typedef enum {
    UNDEFINED_RECORD_TYPE,
    SCREEN_DESC_RECORD_TYPE,
    IMAGE_DESC_RECORD_TYPE,		/* Begin with ',' */
    EXTENSION_RECORD_TYPE,		/* Begin with '!' */
    TERMINATE_RECORD_TYPE		/* Begin with ';' */
} GifRecordType;


typedef enum {
    STATE_MGIF_TERMINATE,
    STATE_GIF_ERROR,
    STATE_MGIF_WAIT_BUFFFER,
    STATE_GIF_SUCCESS
} GifState;

/* DumpScreen2Gif routine constants identify type of window/screen to dump.  */
/* Note all values below 1000 are reserved for the IBMPC different display   */
/* devices (it has many!) and are compatible with the numbering TC2.0        */
/* (Turbo C 2.0 compiler for IBM PC) gives to these devices.		     */
typedef enum {
    GIF_DUMP_SGI_WINDOW = 1000,
    GIF_DUMP_X_WINDOW = 1001
} GifScreenDumpType;

/* func type to read gif data from arbitrary sources (TVT) */
typedef MS_U32 (*InputFunc)(GifFileType*,GifByteType*,int);

/* func type to write gif data ro arbitrary targets.
 * Returns count of bytes written. (MRB)
 */
typedef MS_U32 (*OutputFunc)(GifFileType *, const GifByteType *, int);
/******************************************************************************
*  GIF89 extension function codes                                             *
******************************************************************************/

#define COMMENT_EXT_FUNC_CODE		0xfe	/* comment */
#define GRAPHICS_EXT_FUNC_CODE		0xf9	/* graphics control */
#define PLAINTEXT_EXT_FUNC_CODE		0x01	/* plaintext */
#define APPLICATION_EXT_FUNC_CODE	0xff	/* application block */

/******************************************************************************
* O.K., here are the routines one can access in order to encode GIF file:     *
* (GIF_LIB file EGIF_LIB.C).						      *
******************************************************************************/

GifFileType *EGifOpenFileName(const char *GifFileName, MS_U32 GifTestExistance);
GifFileType *EGifOpenFileHandle(MS_U32 GifFileHandle);
GifFileType *EgifOpen(void *userPtr, OutputFunc writeFunc);
MS_U32 EGifSpew(GifFileType *GifFile);
void EGifSetGifVersion(const char *Version);
MS_U32 EGifPutScreenDesc(GifFileType *GifFile,
	MS_U32 GifWidth, MS_U32 GifHeight, MS_U32 GifColorRes, MS_U32 GifBackGround,
	const ColorMapObject *GifColorMap);
MS_U32 EGifPutImageDesc(GifFileType *GifFile,
	MS_U32 GifLeft, MS_U32 GifTop, MS_U32 Width, MS_U32 GifHeight, MS_U32 GifInterlace,
	const ColorMapObject *GifColorMap);
MS_U32 EGifPutLine(GifFileType *GifFile, GifPixelType *GifLine, MS_U32 GifLineLen);
MS_U32 EGifPutPixel(GifFileType *GifFile, GifPixelType GifPixel);
MS_U32 EGifPutComment(GifFileType *GifFile, const char *GifComment);
MS_U32 EGifPutExtensionFirst(GifFileType *GifFile, MS_U32 GifExtCode, MS_U32 GifExtLen,
                           const VoidPtr GifExtension);
MS_U32 EGifPutExtensionNext(GifFileType *GifFile, MS_U32 GifExtCode, MS_U32 GifExtLen,
                           const VoidPtr GifExtension);
MS_U32 EGifPutExtensionLast(GifFileType *GifFile, MS_U32 GifExtCode, MS_U32 GifExtLen,
                           const VoidPtr GifExtension);
MS_U32 EGifPutExtension(GifFileType *GifFile, MS_U32 GifExtCode, MS_U32 GifExtLen,
							const VoidPtr GifExtension);
MS_U32 EGifPutCode(GifFileType *GifFile, MS_U32 GifCodeSize,
					  const GifByteType *GifCodeBlock);
MS_U32 EGifPutCodeNext(GifFileType *GifFile, const GifByteType *GifCodeBlock);
MS_U32 EGifCloseFile(GifFileType *GifFile);

#define	E_GIF_ERR_OPEN_FAILED	1		/* And EGif possible errors. */
#define	E_GIF_ERR_WRITE_FAILED	2
#define E_GIF_ERR_HAS_SCRN_DSCR	3
#define E_GIF_ERR_HAS_IMAG_DSCR	4
#define E_GIF_ERR_NO_COLOR_MAP	5
#define E_GIF_ERR_DATA_TOO_BIG	6
#define E_GIF_ERR_NOT_ENOUGH_MEM 7
#define E_GIF_ERR_DISK_IS_FULL	8
#define E_GIF_ERR_CLOSE_FAILED	9
#define E_GIF_ERR_NOT_WRITEABLE	10

/******************************************************************************
* O.K., here are the routines one can access in order to decode GIF file:     *
* (GIF_LIB file DGIF_LIB.C).						      *
******************************************************************************/


GifFileType *MstDGifProcess(void* ptr,MS_U32 ocolor_typ,MS_U32 mode,void *roi);
GifFileType *MstDGifOpenFileHandle(MS_U32 GifFileHandle,InputFunc read_fun, GifByteType* bitstream);
GifFileType *MstDGifOpen( void* userPtr, InputFunc readFunc );  /* new one (TVT) */
MS_U32 MstDGifSlurp(GifFileType *GifFile);
MS_U32 MstDGifGetScreenDesc(GifFileType *GifFile);
MS_U32 MstDGifGetRecordType(GifFileType *GifFile, GifRecordType *GifType);
MS_U32 MstDGifGetImageDesc(GifFileType *GifFile);
MS_U32 MstDGifGetLine(GifFileType *GifFile, GifPixelType *GifLine, MS_U32 GifLineLen);
MS_U32 MstDGifGetPixel(GifFileType *GifFile, GifPixelType GifPixel);
MS_U32 MstDGifGetComment(GifFileType *GifFile, char *GifComment);
MS_U32 MstDGifGetExtension(GifFileType *GifFile, MS_U32 *GifExtCode,
						GifByteType **GifExtension);
MS_U32 MstDGifGetExtensionNext(GifFileType *GifFile, GifByteType **GifExtension);
MS_U32 MstDGifGetCode(GifFileType *GifFile, MS_U32 *GifCodeSize,
						GifByteType **GifCodeBlock);
MS_U32 MstDGifGetCodeNext(GifFileType *GifFile, GifByteType **GifCodeBlock);
MS_U32 MstDGifGetLZCodes(GifFileType *GifFile, MS_U32 *GifCode);
MS_U32 MstDGifCloseFile(GifFileType *GifFile);
void MstDGifSetMode (MS_U32 mode);
MS_U32 MstDGifGetDuration(void);


#define	D_GIF_ERR_OPEN_FAILED	101		/* And DGif possible errors. */
#define	D_GIF_ERR_READ_FAILED	102
#define	D_GIF_ERR_NOT_GIF_FILE	103
#define D_GIF_ERR_NO_SCRN_DSCR	104
#define D_GIF_ERR_NO_IMAG_DSCR	105
#define D_GIF_ERR_NO_COLOR_MAP	106
#define D_GIF_ERR_WRONG_RECORD	107
#define D_GIF_ERR_DATA_TOO_BIG	108
#define D_GIF_ERR_NOT_ENOUGH_MEM 109
#define D_GIF_ERR_CLOSE_FAILED	110
#define D_GIF_ERR_NOT_READABLE	111
#define D_GIF_ERR_IMAGE_DEFECT	112
#define D_GIF_ERR_EOF_TOO_SOON	113

#if 0
/******************************************************************************
* O.K., here are the routines from GIF_LIB file QUANTIZE.C.		      *
******************************************************************************/
MS_U32 QuantizeBuffer(MS_U32 Width, MS_U32 Height, MS_U32 *ColorMapSize,
	GifByteType *RedInput, GifByteType *GreenInput, GifByteType *BlueInput,
	GifByteType *OutputBuffer, GifColorType *OutputColorMap);
#endif


/******************************************************************************
* O.K., here are the routines from GIF_LIB file GIF_ERR.C.		      *
******************************************************************************/
 void PrintGifError(void);
 MS_U32 MstGifLastError(void);

/******************************************************************************
* O.K., here are the routines from GIF_LIB file DEV2GIF.C.		      *
******************************************************************************/
 MS_U32 DumpScreen2Gif(const char *FileName,
			  MS_U32 ReqGraphDriver,
			  MS_U32 ReqGraphMode1,
			  MS_U32 ReqGraphMode2,
			  MS_U32 ReqGraphMode3);

/*****************************************************************************
 *
 * Everything below this point is new after version 1.2, supporting `slurp
 * mode' for doing I/O in two big belts with all the image-bashing in core.
 *
 *****************************************************************************/

/******************************************************************************
* Color Map handling from ALLOCGIF.C					      *
******************************************************************************/

 ColorMapObject *MstMakeMapObject(MS_U32 ColorCount, const GifColorType *ColorMap);
 void MstFreeMapObject(ColorMapObject *Object);
 ColorMapObject *UnionColorMap(
				const ColorMapObject *ColorIn1,
				const ColorMapObject *ColorIn2,
				GifPixelType ColorTransIn2[]);
 MS_U32 MstBitSize(MS_U32 n);

/******************************************************************************
* Support for the in-core structures allocation (slurp mode).		      *
******************************************************************************/

/* This is the in-core version of an extension record */
typedef struct {
    int		ByteCount;
    char	*Bytes;		/* on malloc(3) heap */
    MS_U32 Function;       /* Holds the type of the Extension block. */
} ExtensionBlock;

/* This holds an image header, its unpacked raster bits, and extensions */
typedef struct SavedImage {
    GifImageDesc	ImageDesc;

    char		*RasterBits;		/* on malloc(3) heap */

    int			Function; /* DEPRECATED: Use ExtensionBlocks[x].Function
                           * instead */
    int			ExtensionBlockCount;
    ExtensionBlock	*ExtensionBlocks;	/* on malloc(3) heap */
} SavedImage;

extern void ApplyTranslation(SavedImage *Image, GifPixelType Translation[]);

extern void MakeExtension(SavedImage *New, MS_U32 Function);
extern MS_U32 AddExtensionBlock(SavedImage *New, MS_U32 Len, char ExtData[]);
extern void FreeExtension(SavedImage *Image);

extern SavedImage *MakeSavedImage(GifFileType *GifFile, const SavedImage *CopyFrom);
extern void FreeSavedImages(GifFileType *GifFile);

/******************************************************************************
* The library's internal utility font					      *
******************************************************************************/
#if 0
#define GIF_FONT_WIDTH	8
#define GIF_FONT_HEIGHT	8
extern MS_U8 AsciiTable[][GIF_FONT_WIDTH];

extern void DrawText(SavedImage *Image,
		     const MS_U32 x, const MS_U32 y,
		     const char *legend,
		     const MS_U32 color);

extern void DrawBox(SavedImage *Image,
		     const MS_U32 x, const MS_U32 y,
		     const MS_U32 w, const MS_U32 d,
		     const MS_U32 color);

void DrawRectangle(SavedImage *Image,
		     const MS_U32 x, const MS_U32 y,
		     const MS_U32 w, const MS_U32 d,
		     const MS_U32 color);

extern void DrawBoxedText(SavedImage *Image,
		     const MS_U32 x, const MS_U32 y,
		     const char *legend,
		     const MS_U32 border,
		     const MS_U32 bg,
		     const MS_U32 fg);
#endif

#endif /* _GIF_LIB_H */
