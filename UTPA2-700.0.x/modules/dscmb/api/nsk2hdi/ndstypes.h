/*
Copyright (c) NDS Limited 2010

  P R O P R I E T A R Y & C O N F I D E N T I A L

    The copyright of this code and related documentation together with
    any other associated intellectual property rights are vested in
    NDS Limited and may not be used except in accordance with the terms
    of the license that you have entered into with NDS Limited.
    Use of this material without an express license from NDS Limited
    shall be an infringement of copyright and any other intellectual
    property rights that may be incorporated with this material.

*/

#ifndef  H_NDSTYPES
#define  H_NDSTYPES

/* This file contains the definitions of the standard NDS types */

typedef  unsigned char        NDS_UBYTE;
typedef  unsigned short       NDS_USHORT;
typedef  unsigned int         NDS_ULONG;
typedef  unsigned char        NDS_BOOLEAN;

typedef  unsigned int         NDS_STATUS32;//increase by Jeff

typedef  NDS_USHORT           NDS_STATUS;

#define STATUS_OK             (NDS_STATUS)0
#define STATUS_FAIL           (NDS_STATUS)1

#define NDS_TRUE              (NDS_BOOLEAN)1
#define NDS_FALSE             (NDS_BOOLEAN)0

/* the following definitions are obsolete and are included for
backward compatibility purpose only */
typedef  unsigned char        NDS_BYTE;    /* obsolete */
typedef  NDS_BOOLEAN          NBOOL;       /* obsolete */

#endif /* H_NDSTYPES */
