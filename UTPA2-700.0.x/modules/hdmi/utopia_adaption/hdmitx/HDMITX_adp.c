//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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

#ifndef _ADP_HDMITX_C_
#define _ADP_HDMITX_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsDevice.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#else
#include <string.h>
#endif
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"

#include "utopia_dapi.h"
#include "HDMITX_adp.h"
#include "apiHDMITx.h"
#include "apiHDMITx_v2.h"


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                    if(copy_from_user(a,b,c) != 0) { break; }
#define CPY_TO_USER(a,b,c)                      if(copy_to_user(a,b,c) != 0) { break; }
#else
#define CPY_FROM_USER                           memcpy
#define CPY_TO_USER                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                           compat_ptr(a)
#define COMPAT_NEXT_PTR(a)                      (*((MS_U32*)compat_ptr((unsigned long)a)))
#define IS_CMP_TASK()                           is_compat_task()
#define CMP_CPY_FROM_USER(a,b,c)                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
#define CMP_CPY_TO_USER(a,b,c)                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }
#define MS_COMPAT_HDMITX                        compat_uptr_t

#else
#define COMPAT_PTR(a)                           (a)
#define COMPAT_NEXT_PTR(a)                      (*((MS_U32*)a))
#define IS_CMP_TASK()                           (FALSE)
#define CMP_CPY_FROM_USER                       CPY_FROM_USER
#define CMP_CPY_TO_USER                         CPY_TO_USER
#define MS_COMPAT_HDMITX                        MS_U8*

#endif //CONFIG_COMPAT

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pu8Buffer;
    MS_U8 u8BufSize;
    MS_BOOL bReturn;
} stHDMITx_GetRxVideoFormatFromEDID_Compat,*pstHDMITx_GetRxVideoFormatFromEDID_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pu8Buffer;
    MS_U8 u8BufSize;
    MS_BOOL bReturn;
} stHDMITx_GetVICListFromEDID_Compat,*pstHDMITx_GetVICListFromEDID_Compat;

typedef struct DLL_PACKED
{
    HDMITX_VIDEO_TIMING timing;
    MS_COMPAT_HDMITX pColorFmt;
    MS_BOOL bReturn;
} stHDMITx_GetColorFormatFromEDID_Compat,*pstHDMITx_GetColorFormatFromEDID_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pu8Buffer;
    MS_U8 u8BufSize;
    MS_BOOL bReturn;
} stHDMITx_GetRxAudioFormatFromEDID_Compat,*pstHDMITx_GetRxAudioFormatFromEDID_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pu8Buffer;
    MS_BOOL bReturn;
} stHDMITx_GetRxIDManufacturerName_Compat,*pstHDMITx_GetRxIDManufacturerName_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pu8Buffer;
    MS_BOOL BlockIdx;
    MS_BOOL bReturn;
} stHDMITx_GetEDIDData_Compat,*pstHDMITx_GetEDIDData_Compat;

typedef struct DLL_PACKED
{
    HDMITX_VIDEO_TIMING timing;
    MS_COMPAT_HDMITX p3DStructure;
    MS_BOOL bReturn;
} stHDMITx_GetRx3DStructureFromEDID_Compat,*pstHDMITx_GetRx3DStructureFromEDID_Compat;


typedef struct DLL_PACKED
{
    HDMITX_PACKET_TYPE packet_type;
    MS_COMPAT_HDMITX data;
    MS_U8 length;
    MS_BOOL bReturn;
} stHDMITx_PKT_Content_Define_Compat,*pstHDMITx_PKT_Content_Define_Compat;

typedef struct DLL_PACKED
{
    MS_BOOL useinternalkey;
    MS_COMPAT_HDMITX data;
} stHDMITx_GetHdcpKey_Compat,*pstHDMITx_GetHdcpKey_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pdata;
    MS_BOOL bReturn;
} stHDMITx_GetBksv_Compat,*pstHDMITx_GetBksv_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pdata;
    MS_BOOL bReturn;
} stHDMITx_GetAksv_Compat,*pstHDMITx_GetAksv_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX data;
    MS_U16 size;
} stHDMITx_HDCP_RevocationKey_List_Compat,*pstHDMITx_HDCP_RevocationKey_List_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX data;
    MS_U32 size;
    MS_BOOL bReturn;
} stHDMITx_HDCP_IsSRMSignatureValid_Compat,*pstHDMITx_HDCP_IsSRMSignatureValid_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pdata;
} stHDMITx_GetEDIDPhyAdr_Compat,*pstHDMITx_GetEDIDPhyAdr_Compat;

typedef struct DLL_PACKED
{
    EN_HDMITX_CAPS eCapType;
    MS_COMPAT_HDMITX pRet;
    MS_U32 ret_size;
    MS_BOOL bReturn;
} stHDMITx_GetChipCaps_Compat,*pstHDMITx_GetChipCaps_Compat;

typedef struct DLL_PACKED
{
    HDMITX_CEA_DB_TAG_CODE enTagCode;
    HDMITX_CEA_EXT_TAG_CODE enExtTagCode;
    MS_COMPAT_HDMITX pu8Data;
    MS_U32 u32DataLen;
    MS_COMPAT_HDMITX pu32RealLen;
    MS_BOOL bReturn;
} stHDMITx_GetEdidDataBlocks_Compat, *pstHDMITx_GetEdidDataBlocks_Compat;

typedef struct DLL_PACKED
{
    MS_COMPAT_HDMITX pu8Bstatus;
    MS_COMPAT_HDMITX pu8KSVList;
    MS_U16 u16BufLen;
    MS_COMPAT_HDMITX pu16KSVLength;
    MS_BOOL bReturn;
} stHDMITx_GetKSVList_Compat, *pstHDMITx_GetKSVList_Compat;

typedef struct DLL_PACKED
{
    MS_U8 u8PortIdx;
    MS_U8 u8OffsetAddr;
    MS_U8 u8OpCode;
    MS_COMPAT_HDMITX pu8RdBuf;
    MS_U16 u16RdLen;
    MS_COMPAT_HDMITX pu8WRBuff;
    MS_U16 u16WrLen;
    MS_BOOL bReturn;
} stHDMITx_HDCP2AccessX74_Compat, *pstHDMITx_HDCP2AccessX74_Compat;

typedef struct DLL_PACKED
{
    MS_U8 u8PortIdx;
    MS_COMPAT_HDMITX pu8Riv;
    MS_COMPAT_HDMITX pu8KsXORLC128;
} stHDMITx_HDCP2TxFillCipherKey_Compat, *pstHDMITx_HDCP2TxFillCipherKey_Compat;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

UADP_SPT_NAMEnNXT_DEF(HDMITX_BOOL, 0);

//Mapi_HDMITx_init()
UADP_SPT_NAMEnNXT_DEF(HDMITX_INIT, 0);

//Mapi_HDMITx_Exit()
UADP_SPT_NAMEnNXT_DEF(HDMITX_EXIT, 0);

//Mapi_HDMITx_TurnOnOff()
UADP_SPT_NAMEnNXT_DEF(HDMITX_TURNONOFF, 0);

//Mapi_HDMITx_EnablePacketGen()
UADP_SPT_NAMEnNXT_DEF(HDMITX_ENPKTGEN, 0);

//Mapi_HDMITx_SetHDMITxMode()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETHDMITXMODE, 0);

//Mapi_HDMITx_SetHDMITxMode_CD()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETHDMITXMODECD, 0);

//Mapi_HDMITx_SetTMDSOnOff()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETTMDSONOFF, 0);

//Mapi_HDMITx_DisableTMDSCtrl()
UADP_SPT_NAMEnNXT_DEF(HDMITX_DISABLETMDSCTRL, 0);

//Mapi_HDMITx_SetRBChannelSwap()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETRBCHSWAP, 0);

//Mapi_HDMITx_GetRxStatus()
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETRXSTATUS, 0);

//Mapi_HDMITx_GetRxDCInfoFromEDID()
UADP_SPT_NAMEnNXT_DEF(ColorDepthPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETDCINFOFROMEDID, 1);

//Mapi_HDMITx_GetRxVideoFormatFromEDID()
#if 0
UADP_SPT_NAMEnNXT_DEF(VidFmtData, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETRXVIDFMTFROMEDID, 1);
#endif

//Mapi_HDMITx_GetVICListFromEDID()
#if 0
UADP_SPT_NAMEnNXT_DEF(VICListData, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GTVICLISTFROMEDID, 1);
#endif

//Mapi_HDMITx_GetDataBlockLengthFromEDID()
UADP_SPT_NAMEnNXT_DEF(DataBlkLenPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETDATABLKLENFROMEDID, 1);

//Mapi_HDMITx_GetColorFormatFromEDID()
#if 0
UADP_SPT_NAMEnNXT_DEF(ColorFmtPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETCOLORFMTFROMEDID, 1);
#endif

//Mapi_HDMITx_GetRxAudioFormatFromEDID()
#if 0
UADP_SPT_NAMEnNXT_DEF(AudFmtData, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITx_GETRXAUDFMTFROMEDID, 1);
#endif

//Mapi_HDMITx_EDID_HDMISupport()
UADP_SPT_NAMEnNXT_DEF(HDMISupPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_EDIDHDMISUPPORT, 1);

//Mapi_HDMITx_GetRxIDManufacturerName()
#if 0
UADP_SPT_NAMEnNXT_DEF(ManuIDData, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETRXIDMANUFACTNAME, 1);
#endif

//Mapi_HDMITx_GetEDIDData()
#if 0
//UADP_SPT_NAMEnNXT_DEF(SinkEDIDBlockData, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_GETEDIDDATA, 1);
#endif

//Mapi_HDMITx_GetRx3DStructureFromEDID()
#if 0
UADP_SPT_NAMEnNXT_DEF(Edid3DStPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETRX3DSTRUCTFROMEDID, 1);
#endif

//Mapi_HDMITx_PKT_User_Define()
UADP_SPT_NAMEnNXT_DEF(HDMITX_PKTUSERDEF, 0);

//Mapi_HDMITx_PKT_Content_Define()
#if 0
//UADP_SPT_NAMEnNXT_DEF(UserDefPktContent, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_PKTCONTENTDEF, 1);
#endif

//Mapi_HDMITx_SetVideoOnOff()
UADP_SPT_NAMEnNXT_DEF(HDMITx_SETVIDONOFF, 0);

//Mapi_HDMITx_SetColorFormat
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETCOLORFMT, 0);

//Mapi_HDMITx_Set_VS_InfoFrame()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETVSINFOFRAME, 0);

//Mapi_HDMITx_SetVideoOutputTiming()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETVIDOUTPUTTIMING, 0);

//Mapi_HDMITx_SetVideoOutputTimingByCustomer()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETVIDEOUTPUTTIMINGBYCUSTOMER, 0);

//Mapi_HDMITx_SetVideoOutputAsepctRatio()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETVIDOUTPUTASPECTRATIO, 0);

//Mapi_HDMITx_SetVideoOutputOverscan_AFD()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETVIDOUTPUTOVERSCANAFD, 0);

//Mapi_HDMITx_SetAudioOnOff()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETAUDONOFF, 0);

//Mapi_HDMITx_SetAudioFrequency()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETAUDFREQ, 0);

//Mapi_HDMITx_SetVideoOutputOverscan_AFD_II()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETVIDOUTPUTOVERSCANAFD_II, 0);

//Mapi_HDMITx_SetAudioConfiguration()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETAUDCONFIG, 0);

//Mapi_HDMITx_SetAudioSourceFormat()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETAUDSRCFMT, 0);

//Mapi_HDMITx_SetAudioSourceFormat()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETAVIINFOEXTCOLORIMETRY, 0);

//Mapi_HDMITx_GetAudioCTS()
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETAUDCTS, 0);

//Mapi_HDMITx_MuteAudioFIFO()
UADP_SPT_NAMEnNXT_DEF(HDMITX_MUTEAUDFIFO, 0);

//Mapi_HDMITx_GetHdcpKey()
//UADP_SPT_NAMEnNXT_DEF(Hdcp14KeyData, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_GETHDCPKEY, 1);

//Mapi_HDMITx_GetBksv()
//UADP_SPT_NAMEnNXT_DEF(Hdcp14BKSVData, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_GETBKSV, 1);

//Mapi_HDMITx_GetAksv()
//UADP_SPT_NAMEnNXT_DEF(Hdcp14AKSVData, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_GETAKSV, 1);

//Mapi_HDMITx_SetHDCPOnOff()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETHDCPONOFF, 0);

//Mapi_HDMITx_SetAVMUTE()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETAVMUTE, 0);

//Mapi_HDMITx_GetAVMUTEStatus()
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETAVMUTESTATUS, 0);

//Mapi_HDMITx_HDCP_RevocationKey_List()
//UADP_SPT_NAMEnNXT_DEF(Hdcp14RevokeList, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCPREVOKEKEYLIST, 1);

//Mapi_HDMITx_HDCP_RevocationKey_Check()
UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCP14REVOKEKEYCHK, 0);

//Mapi_HDMITx_HDCP_IsSRMSignatureValid()
#if 0
//UADP_SPT_NAMEnNXT_DEF(Hdcp14SRMSignData, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCP14ISSRMSIGNVALID, 1);
#endif

//Mapi_HDMITx_GetHDCPStatus()
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETHDCPSTATUS, 0);

//Mapi_HDMITx_HDCP_StartAuth()
UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCP14STARTAUTH, 0);

//Mapi_HDMITx_GetINTHDCPStatus()
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETINTHDCPSTATUS, 0);

//Mapi_HDMITx_GetHDCP_PreStatus()
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETHDCP14PRESTATUS, 0);

//Mapi_UnHDCPRxControl()
UADP_SPT_NAMEnNXT_DEF(HDMITX_UNHDCPRXCTRL, 0);

//Mapi_HDMITx_HDCPRxFailControl()
UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCPRXFAILCTRL, 0);

//Mapi_HDMITx_GetLibVer()
#if 0
UADP_SPT_NAMEnNXT_DEF(MSIF_VER_PT, 0);
UADP_SPT_NAMEnNXT_DEF(LibVerPt, 1);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETLIBVER, 1);
#endif

//Mapi_HDMITx_GetInfo()
UADP_SPT_NAMEnNXT_DEF(HdmiTxInfoPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETINFO, 1);

//Mapi_HDMITx_GetStatus()
UADP_SPT_NAMEnNXT_DEF(HdmiTxStatusPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETSTATUS, 1);

//Mapi_HDMITx_SetDbgLevels()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETDBGLEVEL, 0);

//Mapi_HDMITx_SetHPDGpioPin()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETHPDGPIOPIN, 0);

//Mapi_HDMITx_AnalogTuning()
UADP_SPT_NAMEnNXT_DEF(AnalogTuningPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_ANALOGTUNING, 1);

//Mapi_HDMITx_ForceHDMIOutputMode()
UADP_SPT_NAMEnNXT_DEF(HDMITX_FORCEHDMIOUTPUTMODE, 0);

//Mapi_HDMITx_ForceHDMIOutputColorFormat()
UADP_SPT_NAMEnNXT_DEF(HDMITX_FORCEHDMIOUTPUTCOLORFMT, 0);

//Mapi_HDMITx_DisableRegWrite()
UADP_SPT_NAMEnNXT_DEF(HDMITX_DISABLEREGWRITE, 0);

//Mapi_HDMITx_GetEDIDPhyAdr()
//UADP_SPT_NAMEnNXT_DEF(EdidPhyAddrData, 0);
//UADP_SPT_NAMEnNXT_DEF(HDMITX_GETEDIDPHYADR, 1);

//Mapi_HDMITx_SetCECOnOff()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETCECONOFF, 0);

//Mapi_HDMITx_GetCECStatus()
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETCECSTATUS, 0);

//Mapi_HDMITx_EdidChecking()
UADP_SPT_NAMEnNXT_DEF(HDMITX_EDIDCHECKING, 0);

//Mapi_HDMITx_RxBypass_Mode()
UADP_SPT_NAMEnNXT_DEF(HDMITX_RXBYPASSMODE, 0);

//Mapi_HDMITx_Disable_RxBypass()
UADP_SPT_NAMEnNXT_DEF(HDMITX_DISABLERXBYPASS, 0);

//Mapi_HDMITx_SetAksv2R0Interval()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETAKSV2R0INTERVAL, 0);

//Mapi_HDMITx_IsHDCPRxValid()
UADP_SPT_NAMEnNXT_DEF(HDMITX_ISHDCPRXVALID, 0);

//Mapi_HDMITx_GetChipCaps()
#if 0
UADP_SPT_NAMEnNXT_DEF(HdmiChipCapInfo, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETCHIPCAPS, 1);
#endif

//Mapi_HDMITx_SetPowerState()
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETPWRSTATE, 0);

//Mapi_HDMITx_GetEdidDataBlocks()
#if 0
UADP_SPT_NAMEnNXT_DEF(SinkEDIDDataBlk, 0);
UADP_SPT_NAMEnNXT_DEF(DataBlkRetLenPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETEDIDDATABLKS, 2);
#endif

//Mapi_HDMITx_GetKSVList()
#if 0
UADP_SPT_NAMEnNXT_DEF(SinkBstatusData, 0);
UADP_SPT_NAMEnNXT_DEF(SinkKSVList, 0);
UADP_SPT_NAMEnNXT_DEF(KSVListLenPt, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_GETKSVLIST, 3);
#endif

//Mapi_HDMITx_HDCP2AccessX74()
#if 0
UADP_SPT_NAMEnNXT_DEF(Hdcp22x74RdData, 0);
UADP_SPT_NAMEnNXT_DEF(Hdcp22x74WrData, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCP2ACCESSX74, 2);
#endif

//Mapi_HDMITx_HDCP2TxInit()
UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCP2TXINIT, 0);

//Mapi_HDMITx_HDCP2TxEnableEncrypt()
UADP_SPT_NAMEnNXT_DEF(HDMITX_HDCP2TXENABLEENC, 0);


//Mapi_HDMITx_HDCP2TxFillCipherKey()
#if 0
UADP_SPT_NAMEnNXT_DEF(Hdcp22RivData, 0);
UADP_SPT_NAMEnNXT_DEF(Hdcp22KsData, 0);
UADP_SPT_NAMEnNXT_DEF(HDMITx_HDCP2TXFILLCIPHERKEY, 2);
#endif

//MApi_HDMITx_ColorandRange_Transform
UADP_SPT_NAMEnNXT_DEF(HDMITX_COLORRANGE_TRANSFORM, 0);

//MApi_HDMITx_SSC_Enable
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETSSCENABLE, 0);

//MApi_HDMITx_SetAVIInfoExtColorimetry
UADP_SPT_NAMEnNXT_DEF(HDMITX_SETCOLORIMETRY, 0);

//MApi_HDMITx_SetAVIInfoExtColorimetry
UADP_SPT_NAMEnNXT_DEF(HDMITX_GET_FULL_RX_STATUS, 0);

//MApi_HDMITx_SetAVIInfoExtColorimetry
UADP_SPT_NAMEnNXT_DEF(HDMITX_CHECK_LEGAL_TIMING, 0);

MS_U32 HDMITX_adp_Init(FUtopiaIOctl* pIoctl)
{
    MS_U32 u32DataSize = 0x00;

    //MS_BOOL
    UADP_SPT_NAME0NXT(HDMITX_BOOL, MS_BOOL);

    //E_HDMITX_CMD_INIT:: Mapi_HDMITx_init()
    UADP_SPT_NAME0NXT(HDMITX_INIT, stHDMITx_Init);

    //E_HDMITX_CMD_INIT:: Mapi_HDMITx_Exit()
    UADP_SPT_NAME0NXT(HDMITX_EXIT, stHDMITx_Exit);

    //E_HDMITX_CMD_TURNONOFF:: Mapi_HDMITx_TurnOnOff()
    UADP_SPT_NAME0NXT(HDMITX_TURNONOFF, stHDMITx_TurnOnOff);

    //Mapi_HDMITx_EnablePacketGen()
    UADP_SPT_NAME0NXT(HDMITX_ENPKTGEN, stHDMITx_EnablePacketGen);

    //Mapi_HDMITx_SetHDMITxMode()
    UADP_SPT_NAME0NXT(HDMITX_SETHDMITXMODE, stHDMITx_SetHDMITxMode);

    //Mapi_HDMITx_SetHDMITxMode_CD()
    UADP_SPT_NAME0NXT(HDMITX_SETHDMITXMODECD, stHDMITx_SetHDMITxMode_CD);

    //Mapi_HDMITx_SetTMDSOnOff()
    UADP_SPT_NAME0NXT(HDMITX_SETTMDSONOFF, stHDMITx_SetTMDSOnOff);

    //Mapi_HDMITx_DisableTMDSCtrl()
    UADP_SPT_NAME0NXT(HDMITX_DISABLETMDSCTRL, stHDMITx_DisableTMDSCtrl);

    //Mapi_HDMITx_SetRBChannelSwap()
    UADP_SPT_NAME0NXT(HDMITX_SETRBCHSWAP, stHDMITx_SetRBChannelSwap);

    //Mapi_HDMITx_GetRxStatus()
    UADP_SPT_NAME0NXT(HDMITX_GETRXSTATUS, stHDMITx_GetRxStatus);

    //Mapi_HDMITx_GetRxDCInfoFromEDID()
    UADP_SPT_NAME0NXT(ColorDepthPt, HDMITX_VIDEO_COLORDEPTH_VAL);
    UADP_SDT_NAME1(HDMITX_GETDCINFOFROMEDID, stHDMITx_GetRxDCInfoFromEDID, UADP_SDT_P2N, val, ColorDepthPt);

    //Mapi_HDMITx_GetRxVideoFormatFromEDID()
    #if 0
#define DEF_EDID_VIDFMT_SIZE 127U
    u32DataSize = sizeof(MS_U8) * DEF_EDID_VIDFMT_SIZE;
    UADP_SPT_BGN(&spt_VidFmtData[0], u32DataSize);
    UADP_SPT_FIN(&spt_VidFmtData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETRXVIDFMTFROMEDID, stHDMITx_GetRxVideoFormatFromEDID, pu8Buffer, VidFmtData);
#undef DEF_EDID_VIDFMT_SIZE
    #endif

    //Mapi_HDMITx_GetVICListFromEDID()
    #if 0
#define DEF_EDID_VICLIST_SIZE 127U
    u32DataSize = sizeof(MS_U8) * DEF_EDID_VICLIST_SIZE;
    UADP_SPT_BGN(&spt_VICListData[0], u32DataSize);
    UADP_SPT_FIN(&spt_VICListData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GTVICLISTFROMEDID, stHDMITx_GetVICListFromEDID, pu8Buffer, VICListData);
#undef DEF_EDID_VICLIST_SIZE
    #endif

    //Mapi_HDMITx_GetDataBlockLengthFromEDID()
    UADP_SPT_NAME0NXT(DataBlkLenPt, MS_U8);
    UADP_SPT_NAME1NXT(HDMITX_GETDATABLKLENFROMEDID, stHDMITx_GetDataBlockLengthFromEDID, pu8Length, DataBlkLenPt);

    //Mapi_HDMITx_GetColorFormatFromEDID()
    #if 0
    UADP_SPT_NAME0NXT(ColorFmtPt, HDMITX_EDID_COLOR_FORMAT);
    UADP_SDT_NAME1(HDMITX_GETCOLORFMTFROMEDID, stHDMITx_GetColorFormatFromEDID, UADP_SDT_P2N, pColorFmt,ColorFmtPt);
    #endif

    //Mapi_HDMITx_GetRxAudioFormatFromEDID()
    #if 0
#define DEF_EDID_AUDFMT_SIZE 127U
        u32DataSize = sizeof(MS_U8) * DEF_EDID_AUDFMT_SIZE;
        UADP_SPT_BGN(&spt_AudFmtData[0], u32DataSize);
        UADP_SPT_FIN(&spt_AudFmtData[1]);
        UADP_SPT_NAME1NXT(HDMITx_GETRXAUDFMTFROMEDID, stHDMITx_GetRxAudioFormatFromEDID, pu8Buffer, AudFmtData);
#undef DEF_EDID_AUDFMT_SIZE
    #endif

    //Mapi_HDMITx_EDID_HDMISupport()
    UADP_SPT_NAME1NXT(HDMITX_EDIDHDMISUPPORT, stHDMITx_EDID_HDMISupport, HDMI_Support, HDMITX_BOOL);


    //Mapi_HDMITx_GetRxIDManufacturerName()
    #if 0
#define DEF_EDID_MANUFACTORID_SIZE 3U
    u32DataSize = sizeof(MS_U8) * DEF_EDID_MANUFACTORID_SIZE;
    UADP_SPT_BGN(&spt_ManuIDData[0], u32DataSize);
    UADP_SPT_FIN(&spt_ManuIDData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETRXIDMANUFACTNAME, stHDMITx_GetRxIDManufacturerName, pu8Buffer, ManuIDData);
#undef DEF_EDID_MANUFACTORID_SIZE
    #endif

    //Mapi_HDMITx_GetEDIDData()
    #if 0
#define DEF_EDID_BLOCK_SIZE 128U
    u32DataSize = sizeof(MS_U8) * DEF_EDID_BLOCK_SIZE;
    UADP_SPT_BGN(&spt_SinkEDIDBlockData[0], u32DataSize);
    UADP_SPT_FIN(&spt_SinkEDIDBlockData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETEDIDDATA, stHDMITx_GetEDIDData, pu8Buffer, SinkEDIDBlockData);
#undef DEF_EDID_BLOCK_SIZE
    #endif

    //Mapi_HDMITx_GetRx3DStructureFromEDID()
    #if 0
    UADP_SPT_NAME0NXT(Edid3DStPt, HDMITX_EDID_3D_STRUCTURE_ALL);
    UADP_SDT_NAME1(HDMITX_GETRX3DSTRUCTFROMEDID, stHDMITx_GetRx3DStructureFromEDID, UADP_SDT_P2N, p3DStructure, Edid3DStPt);
    #endif

    //Mapi_HDMITx_PKT_User_Define()
    UADP_SPT_NAME0NXT(HDMITX_PKTUSERDEF, stHDMITx_PKT_User_Define);

    //Mapi_HDMITx_PKT_Content_Define()
    #if 0
#define DEF_MAX_U8_SIZE 127U
    u32DataSize = sizeof(MS_U8) * DEF_MAX_U8_SIZE;
    UADP_SPT_BGN(&spt_UserDefPktContent[0], u32DataSize);
    UADP_SPT_FIN(&spt_UserDefPktContent[1]);
    UADP_SPT_NAME1NXT(HDMITX_PKTCONTENTDEF, stHDMITx_PKT_Content_Define, data, UserDefPktContent);
#undef DEF_MAX_U8_SIZE
    #endif

    //Mapi_HDMITx_SetVideoOnOff()
    UADP_SPT_NAME0NXT(HDMITx_SETVIDONOFF, stHDMITx_SetVideoOnOff);

    //Mapi_HDMITx_SetColorFormat
    UADP_SPT_NAME0NXT(HDMITX_SETCOLORFMT, stHDMITx_SetColorFormat);

    //Mapi_HDMITx_Set_VS_InfoFrame()
    UADP_SPT_NAME0NXT(HDMITX_SETVSINFOFRAME, stHDMITx_Set_VS_InfoFrame);

    //Mapi_HDMITx_SetVideoOutputTiming()
    UADP_SPT_NAME0NXT(HDMITX_SETVIDOUTPUTTIMING, stHDMITx_SetVideoOutputTiming);

    //Mapi_HDMITx_SetVideoOutputTimingByCustomer()
    UADP_SPT_NAME0NXT(HDMITX_SETVIDEOUTPUTTIMINGBYCUSTOMER, stHDMITx_SetVideoOutputTimingByCustomer);

    //Mapi_HDMITx_SetVideoOutputAsepctRatio()
    UADP_SPT_NAME0NXT(HDMITX_SETVIDOUTPUTASPECTRATIO, stHDMITx_SetVideoOutputAsepctRatio);

    //Mapi_HDMITx_SetVideoOutputOverscan_AFD()
    UADP_SPT_NAME0NXT(HDMITX_SETVIDOUTPUTOVERSCANAFD, stHDMITx_SetVideoOutputOverscan_AFD);

    //Mapi_HDMITx_SetAudioOnOff()
    UADP_SPT_NAME0NXT(HDMITX_SETAUDONOFF, stHDMITx_SetAudioOnOff);

    //Mapi_HDMITx_SetAudioFrequency()
    UADP_SPT_NAME0NXT(HDMITX_SETAUDFREQ, stHDMITx_SetAudioFrequency);

    //Mapi_HDMITx_SetVideoOutputOverscan_AFD_II()
    UADP_SPT_NAME0NXT(HDMITX_SETVIDOUTPUTOVERSCANAFD_II, stHDMITx_SetVideoOutputOverscan_AFD_II);

    //Mapi_HDMITx_SetAudioConfiguration()
    UADP_SPT_NAME0NXT(HDMITX_SETAUDCONFIG, stHDMITx_SetAudioConfiguration);

    //Mapi_HDMITx_SetAudioSourceFormat()
    UADP_SPT_NAME0NXT(HDMITX_SETAUDSRCFMT, stHDMITx_SetAudioSourceFormat);

    //Mapi_HDMITx_SetAudioSourceFormat()
    UADP_SPT_NAME0NXT(HDMITX_SETAVIINFOEXTCOLORIMETRY, stHDMITx_SetAVIInfoExtColorimetry);

    //Mapi_HDMITx_GetAudioCTS()
    UADP_SPT_NAME0NXT(HDMITX_GETAUDCTS, stHDMITx_GetAudioCTS);

    //Mapi_HDMITx_MuteAudioFIFO()
    UADP_SPT_NAME0NXT(HDMITX_MUTEAUDFIFO, stHDMITx_MuteAudioFIFO);

    //Mapi_HDMITx_GetHdcpKey()
    #if 0
#define DEF_HDCP14_KEY_SIZE 304U
    u32DataSize = sizeof(MS_U8) * DEF_HDCP14_KEY_SIZE;
    UADP_SPT_BGN(&spt_Hdcp14KeyData[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp14KeyData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETHDCPKEY, stHDMITx_GetHdcpKey, data, Hdcp14KeyData);
#undef DEF_HDCP14_KEY_SIZE
    #endif

    #if 0
#define DEF_HDCP14_KSV_SIZE 5U
    //Mapi_HDMITx_GetBksv()
    u32DataSize = sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE;
    UADP_SPT_BGN(&spt_Hdcp14BKSVData[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp14BKSVData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETBKSV, stHDMITx_GetBksv, pdata, Hdcp14BKSVData);

    //Mapi_HDMITx_GetAksv()
    UADP_SPT_BGN(&spt_Hdcp14AKSVData[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp14AKSVData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETAKSV, stHDMITx_GetAksv, pdata, Hdcp14AKSVData);
#undef DEF_HDCP14_KSV_SIZE
    #endif

    //Mapi_HDMITx_SetHDCPOnOff()
    UADP_SPT_NAME0NXT(HDMITX_SETHDCPONOFF, stHDMITx_SetHDCPOnOff);

    //Mapi_HDMITx_SetAVMUTE()
    UADP_SPT_NAME0NXT(HDMITX_SETAVMUTE, stHDMITx_SetAVMUTE);

    //Mapi_HDMITx_GetAVMUTEStatus()
    UADP_SPT_NAME0NXT(HDMITX_GETAVMUTESTATUS, stHDMITx_GetAVMUTEStatus);

    //Mapi_HDMITx_HDCP_RevocationKey_List()
    #if 0
#define DEF_HDCP14_REVOKKEY_SIZE 5*127U
    u32DataSize = sizeof(MS_U8) * DEF_HDCP14_REVOKKEY_SIZE;
    UADP_SPT_BGN(&spt_Hdcp14RevokeList[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp14RevokeList[1]);
    UADP_SPT_NAME1NXT(HDMITX_HDCPREVOKEKEYLIST, stHDMITx_HDCP_RevocationKey_List, data, Hdcp14RevokeList);
#undef DEF_HDCP14_REVOKKEY_SIZE
    #endif

    //Mapi_HDMITx_HDCP_RevocationKey_Check()
    UADP_SPT_NAME0NXT(HDMITX_HDCP14REVOKEKEYCHK, stHDMITx_HDCP_RevocationKey_Check);

    //Mapi_HDMITx_HDCP_IsSRMSignatureValid()
    #if 0
#define DEF_HDCP14_SRMSIGN_SIZE 1U  //fake function, always return true
        u32DataSize = sizeof(MS_U8) * DEF_HDCP14_SRMSIGN_SIZE;
        UADP_SPT_BGN(&spt_Hdcp14SRMSignData[0], u32DataSize);
        UADP_SPT_FIN(&spt_Hdcp14SRMSignData[1]);
        UADP_SPT_NAME1NXT(HDMITX_HDCP14ISSRMSIGNVALID, stHDMITx_HDCP_IsSRMSignatureValid, data, Hdcp14SRMSignData);
#undef DEF_HDCP14_SRMSIGN_SIZE
    #endif

    //Mapi_HDMITx_GetHDCPStatus()
    UADP_SPT_NAME0NXT(HDMITX_GETHDCPSTATUS, stHDMITx_GetHDCPStatus);

    //Mapi_HDMITx_HDCP_StartAuth()
    UADP_SPT_NAME0NXT(HDMITX_HDCP14STARTAUTH, stHDMITx_HDCP_StartAuth);

    //Mapi_HDMITx_GetINTHDCPStatus()
    UADP_SPT_NAME0NXT(HDMITX_GETINTHDCPSTATUS, stHDMITx_GetINTHDCPStatus);

    //Mapi_HDMITx_GetHDCP_PreStatus()
    UADP_SPT_NAME0NXT(HDMITX_GETHDCP14PRESTATUS, stHDMITx_GetHDCP_PreStatus);

    //Mapi_UnHDCPRxControl()
    UADP_SPT_NAME0NXT(HDMITX_UNHDCPRXCTRL, stHDMITx_UnHDCPRxControl);

    //Mapi_HDMITx_HDCPRxFailControl()
    UADP_SPT_NAME0NXT(HDMITX_HDCPRXFAILCTRL, stHDMITx_HDCPRxFailControl);

    //undercheck!!!!
    #if 0 //no use
    //Mapi_HDMITx_GetLibVer()
    UADP_SPT_NAME1NXT(HDMITX_GETLIBVER, stHDMITx_GetLibVer, ppVersion, LibVerPt);
    #endif

/*
typedef struct _stHDMITx_GetLibVer
{
    const MSIF_Version **ppVersion;
    MS_BOOL bReturn;
} stHDMITx_GetLibVer,*pstHDMITx_GetLibVer;
*/
    //Mapi_HDMITx_GetInfo()
    UADP_SPT_NAME0NXT(HdmiTxInfoPt, HDMI_TX_INFO);
    UADP_SDT_NAME1(HDMITX_GETINFO, stHDMITx_GetInfo, UADP_SDT_P2N, pInfo, HdmiTxInfoPt);

    //Mapi_HDMITx_GetStatus()
    UADP_SPT_NAME0NXT(HdmiTxStatusPt, HDMI_TX_Status);
    UADP_SDT_NAME1(HDMITX_GETSTATUS, stHDMITx_GetStatus, UADP_SDT_P2N, pStatus, HdmiTxStatusPt);

    //Mapi_HDMITx_SetDbgLevels()
    UADP_SPT_NAME0NXT(HDMITX_SETDBGLEVEL, stHDMITx_SetDbgLevels);

    //Mapi_HDMITx_SetHPDGpioPin()
    UADP_SPT_NAME0NXT(HDMITX_SETHPDGPIOPIN, stHDMITx_SetHPDGpioPin);

    //Mapi_HDMITx_AnalogTuning()
    UADP_SDT_NAME0(AnalogTuningPt, HDMITX_ANALOG_TUNING);
    UADP_SDT_NAME1(HDMITX_ANALOGTUNING, stHDMITx_AnalogTuning, UADP_SDT_P2N, pInfo, AnalogTuningPt);

    //Mapi_HDMITx_ForceHDMIOutputMode()
    UADP_SPT_NAME0NXT(HDMITX_FORCEHDMIOUTPUTMODE, stHDMITx_ForceHDMIOutputMode);

    //Mapi_HDMITx_ForceHDMIOutputColorFormat()
    UADP_SPT_NAME0NXT(HDMITX_FORCEHDMIOUTPUTCOLORFMT, stHDMITx_ForceHDMIOutputColorFormat);

    //Mapi_HDMITx_DisableRegWrite()
    UADP_SPT_NAME0NXT(HDMITX_DISABLEREGWRITE, stHDMITx_DisableRegWrite);

    //Mapi_HDMITx_GetEDIDPhyAdr()
    #if 0
#define DEF_EDID_PHYADDR_SIZE 2U
    u32DataSize = sizeof(MS_U8) * DEF_EDID_PHYADDR_SIZE;
    UADP_SPT_BGN(&spt_EdidPhyAddrData[0], u32DataSize);
    UADP_SPT_FIN(&spt_EdidPhyAddrData[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETEDIDPHYADR, stHDMITx_GetEDIDPhyAdr, pdata, EdidPhyAddrData);
#undef DEF_EDID_PHYADDR_SIZE
    #endif

    //Mapi_HDMITx_SetCECOnOff()
    UADP_SPT_NAME0NXT(HDMITX_SETCECONOFF, stHDMITx_SetCECOnOff);

    //Mapi_HDMITx_GetCECStatus()
    UADP_SPT_NAME0NXT(HDMITX_GETCECSTATUS, stHDMITx_GetCECStatus);

    //Mapi_HDMITx_EdidChecking()
    UADP_SPT_NAME0NXT(HDMITX_EDIDCHECKING, stHDMITx_EdidChecking);

    //Mapi_HDMITx_RxBypass_Mode()
    UADP_SPT_NAME0NXT(HDMITX_RXBYPASSMODE, stHDMITx_RxBypass_Mode);

    //Mapi_HDMITx_Disable_RxBypass()
    UADP_SPT_NAME0NXT(HDMITX_DISABLERXBYPASS, stHDMITx_Disable_RxBypass);

    //Mapi_HDMITx_SetAksv2R0Interval()
    UADP_SPT_NAME0NXT(HDMITX_SETAKSV2R0INTERVAL, stHDMITx_SetAksv2R0Interval);

    //Mapi_HDMITx_IsHDCPRxValid()
    UADP_SPT_NAME0NXT(HDMITX_ISHDCPRXVALID, stHDMITx_IsHDCPRxValid);

    //Mapi_HDMITx_GetChipCaps()
    #if 0
#define DEF_HDMITX_CHIPCAP_SIZE 1U
    u32DataSize = sizeof(MS_U32) * DEF_HDMITX_CHIPCAP_SIZE;
    UADP_SPT_BGN(&spt_HdmiChipCapInfo[0], u32DataSize);
    UADP_SPT_FIN(&spt_HdmiChipCapInfo[1]);
    UADP_SPT_NAME1NXT(HDMITX_GETCHIPCAPS, stHDMITx_GetChipCaps, pRet, HdmiChipCapInfo);
#undef DEF_HDMITX_CHIPCAP_SIZE
    #endif

    //Mapi_HDMITx_SetPowerState()
    UADP_SPT_NAME0NXT(HDMITX_SETPWRSTATE, stHDMITx_SetPowerState);

    //Mapi_HDMITx_GetEdidDataBlocks()
    #if 0
#define DEF_EDID_BATABLK_SIZE 128U
    u32DataSize = sizeof(MS_U8) * DEF_EDID_BATABLK_SIZE;
    UADP_SPT_BGN(&spt_SinkEDIDDataBlk[0], u32DataSize);
    UADP_SPT_FIN(&spt_SinkEDIDDataBlk[1]);
    UADP_SPT_NAME2NXT(HDMITX_GETEDIDDATABLKS, stHDMITx_GetEdidDataBlocks, pu8Data, SinkEDIDDataBlk, pu32RealLen, DataBlkRetLenPt);
#undef DEF_EDID_BATABLK_SIZE
    #endif

    //Mapi_HDMITx_GetKSVList()
    #if 0
#define DEF_KSVLIST_SIZE 5*127U
#define DEF_HDCP14_BSTATUS_SIZE 2U
    u32DataSize = sizeof(MS_U8) * DEF_HDCP14_BSTATUS_SIZE;
    UADP_SPT_BGN(&spt_SinkBstatusData[0], u32DataSize);
    UADP_SPT_FIN(&spt_SinkBstatusData[1]);

    u32DataSize = sizeof(MS_U8) * DEF_KSVLIST_SIZE;
    UADP_SPT_BGN(&spt_SinkKSVList[0], u32DataSize);
    UADP_SPT_FIN(&spt_SinkKSVList[1]);

    UADP_SPT_NAME3NXT(HDMITX_GETKSVLIST, stHDMITx_GetKSVList, pu8Bstatus, SinkBstatusData, pu8KSVList, SinkKSVList, pu16KSVLength, KSVListLenPt);
#undef DEF_HDCP14_BSTATUS_SIZE
#undef DEF_KSVLIST_SIZE
    #endif

    //Mapi_HDMITx_HDCP2AccessX74()
    #if 0
#define DEF_HDCP22_RDBUFF_SIZE 534U //AKE_Send_Cert
#define DEF_HDCP22_WRBUFF_SIZE 129U //AKE_No_StoredKm
    u32DataSize = sizeof(MS_U8) * DEF_HDCP22_RDBUFF_SIZE;
    UADP_SPT_BGN(&spt_Hdcp22x74RdData[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp22x74RdData[1]);

    u32DataSize = sizeof(MS_U8) * DEF_HDCP22_WRBUFF_SIZE;
    UADP_SPT_BGN(&spt_Hdcp22x74WrData[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp22x74RdData[1]);

    UADP_SPT_NAME2NXT(HDMITX_HDCP2ACCESSX74, stHDMITx_HDCP2AccessX74, pu8RdBuf, Hdcp22x74RdData, pu8WRBuff, Hdcp22x74WrData);
#undef DEF_HDCP22_RDBUFF_SIZE
#undef DEF_HDCP22_WRBUFF_SIZE
    #endif

    //Mapi_HDMITx_HDCP2TxInit()
    UADP_SPT_NAME0NXT(HDMITX_HDCP2TXINIT, stHDMITx_HDCP2TxInit);

    //Mapi_HDMITx_HDCP2TxEnableEncrypt()
    UADP_SPT_NAME0NXT(HDMITX_HDCP2TXENABLEENC, stHDMITx_HDCP2TxEnableEncrypt);

    //Mapi_HDMITx_HDCP2TxFillCipherKey()
    #if 0
#define DEF_HDCP22_RIV_SIZE 8U
#define DEF_HDCP22_KS_SIZE 16U
    u32DataSize = sizeof(MS_U8) * DEF_HDCP22_RIV_SIZE;
    UADP_SPT_BGN(&spt_Hdcp22RivData[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp22RivData[1]);

    u32DataSize = sizeof(MS_U8) * DEF_HDCP22_KS_SIZE;
    UADP_SPT_BGN(&spt_Hdcp22KsData[0], u32DataSize);
    UADP_SPT_FIN(&spt_Hdcp22KsData[1]);

    UADP_SPT_NAME2NXT(HDMITx_HDCP2TXFILLCIPHERKEY, stHDMITx_HDCP2TxFillCipherKey, pu8Riv, Hdcp22RivData, pu8KsXORLC128, Hdcp22KsData);
#undef DEF_HDCP22_KS_SIZE
#undef DEF_HDCP22_RIV_SIZE
    #endif

    //MApi_HDMITx_ColorandRange_Transform
    UADP_SPT_NAME0NXT(HDMITX_COLORRANGE_TRANSFORM, stHDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS);

    //MApi_HDMITx_SSC_Enable
    UADP_SPT_NAME0NXT(HDMITX_SETSSCENABLE, stHDMITX_SSCENABLE);

    //MApi_HDMITx_SetAVIInfoExtColorimetry
    UADP_SPT_NAME0NXT(HDMITX_SETCOLORIMETRY, stHDMITX_SET_COLORIMETRY);

    //MApi_HDMITx_GetFullRxStatus
    UADP_SPT_NAME0NXT(HDMITX_GET_FULL_RX_STATUS, stHDMITX_GET_FULL_RX_STATUS);

    //MApi_HDMITx_GetFullRxStatus
    UADP_SPT_NAME0NXT(HDMITX_CHECK_LEGAL_TIMING, stHDMITX_CHECK_LEGAL_TIMING);

    *pIoctl= (FUtopiaIOctl)HDMITX_adp_Ioctl;

    return 0;

}
#define EN_HDMITX_ADP_LOG 0
MS_U32 HDMITX_adp_Ioctl(void *pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 ulReturnValue = UTOPIA_STATUS_SUCCESS;
    char usbuffer_arg[2];
    //MS_U32 u32DataSize = 0x00;

    switch(u32Cmd)
    {
        case E_HDMITX_CMD_INIT:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_INIT\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_INIT, spt_HDMITX_INIT, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_EXIT:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_EXIT\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_EXIT, spt_HDMITX_EXIT, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_TURNONOFF:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_TURNONOFF\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_TURNONOFF, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_ENABLEPACKETGEN:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_ENABLEPACKETGEN\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_ENPKTGEN, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETHDMITXMODE:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETHDMITXMODE\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETHDMITXMODE, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETHDMITXMODE_CD:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETHDMITXMODE_CD\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETHDMITXMODECD, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETTMDSONOFF:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETTMDSONOFF\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETTMDSONOFF, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_DISABLETMDSCTRL:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_DISABLETMDSCTRL\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_DISABLETMDSCTRL, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETRBCHANNELSWAP:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETRBCHANNELSWAP\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETRBCHSWAP, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_EXHIBIT:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_EXHIBIT\r\n");
            #endif
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_HDMITX_CMD_GETRXSTATUS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETRXSTATUS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETRXSTATUS, spt_HDMITX_GETRXSTATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETRXDCINFOFROMEDID:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETRXDCINFOFROMEDID\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETDCINFOFROMEDID, spt_HDMITX_GETDCINFOFROMEDID, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETRXVIDEODORMATFROMEDID:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETRXVIDEODORMATFROMEDID\r\n");
                #endif
                stHDMITx_GetRxVideoFormatFromEDID stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetRxVideoFormatFromEDID_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetRxVideoFormatFromEDID_Compat));

                    //copy Arguments
                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8)*stCompatHDMITxArgs.u8BufSize);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Buffer, stCompatHDMITxArgs.pu8Buffer, sizeof(MS_U8)*stHDMITxArgs.u8BufSize);
                    stHDMITxArgs.u8BufSize = stCompatHDMITxArgs.u8BufSize;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8Buffer, stHDMITxArgs.pu8Buffer, sizeof(MS_U8)*stCompatHDMITxArgs.u8BufSize);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetRxVideoFormatFromEDID_Compat));

                    free(stHDMITxArgs.pu8Buffer);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetRxVideoFormatFromEDID));

                    pData = stHDMITxArgs.pu8Buffer;
                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) *stHDMITxArgs.u8BufSize);
                    CPY_FROM_USER(stHDMITxArgs.pu8Buffer, pData, sizeof(MS_U8) *stHDMITxArgs.u8BufSize);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pu8Buffer, sizeof(MS_U8) *stHDMITxArgs.u8BufSize);
                    free(stHDMITxArgs.pu8Buffer);
                    stHDMITxArgs.pu8Buffer = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetRxVideoFormatFromEDID));
                }
            }
            break;

        case E_HDMITX_CMD_GETVICLISTFROMEDID:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETVICLISTFROMEDID\r\n");
                #endif
                stHDMITx_GetVICListFromEDID stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetVICListFromEDID_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetVICListFromEDID_Compat));

                    //copy arguments
                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) *stCompatHDMITxArgs.u8BufSize);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Buffer, stCompatHDMITxArgs.pu8Buffer, sizeof(MS_U8) *stCompatHDMITxArgs.u8BufSize);
                    stHDMITxArgs.u8BufSize = stCompatHDMITxArgs.u8BufSize;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8Buffer, stCompatHDMITxArgs.pu8Buffer, sizeof(MS_U8) *stCompatHDMITxArgs.u8BufSize);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetVICListFromEDID_Compat));

                    free(stHDMITxArgs.pu8Buffer);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetVICListFromEDID));

                    pData = stHDMITxArgs.pu8Buffer;
                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) *stHDMITxArgs.u8BufSize);
                    CPY_FROM_USER(stHDMITxArgs.pu8Buffer, pData, sizeof(MS_U8) *stHDMITxArgs.u8BufSize);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pu8Buffer, sizeof(MS_U8) *stHDMITxArgs.u8BufSize);
                    free(stHDMITxArgs.pu8Buffer);
                    stHDMITxArgs.pu8Buffer = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetVICListFromEDID));

                }
            }
            break;

        case E_HDMITX_CMD_GETDATABLOCKLENGTHFROMEDID:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETDATABLOCKLENGTHFROMEDID\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETDATABLKLENFROMEDID, spt_HDMITX_GETDATABLKLENFROMEDID, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETCOLORFORMATFROMEDID:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETCOLORFORMATFROMEDID\r\n");
                #endif
                stHDMITx_GetColorFormatFromEDID stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetColorFormatFromEDID_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetColorFormatFromEDID_Compat));

                    stHDMITxArgs.pColorFmt = malloc(sizeof(HDMITX_EDID_COLOR_FORMAT));
                    CMP_CPY_FROM_USER(stHDMITxArgs.pColorFmt, stCompatHDMITxArgs.pColorFmt, sizeof(HDMITX_EDID_COLOR_FORMAT));
                    stHDMITxArgs.timing = stCompatHDMITxArgs.timing;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pColorFmt, stHDMITxArgs.pColorFmt, sizeof(HDMITX_EDID_COLOR_FORMAT));
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetColorFormatFromEDID_Compat));

                    free(stHDMITxArgs.pColorFmt);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetColorFormatFromEDID));

                    pData = stHDMITxArgs.pColorFmt;
                    stHDMITxArgs.pColorFmt = malloc(sizeof(HDMITX_EDID_COLOR_FORMAT));
                    CPY_FROM_USER(stHDMITxArgs.pColorFmt, pData, sizeof(HDMITX_EDID_COLOR_FORMAT));

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pColorFmt, sizeof(HDMITX_EDID_COLOR_FORMAT));
                    free(stHDMITxArgs.pColorFmt);
                    stHDMITxArgs.pColorFmt = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetColorFormatFromEDID));
                }
                //ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETCOLORFMTFROMEDID, spt_HDMITX_GETCOLORFMTFROMEDID, usbuffer_arg, sizeof(usbuffer_arg));
            }
            break;

        case E_HDMITX_CMD_GETRXAUDIOFORMATFROMEDID:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETRXAUDIOFORMATFROMEDID\r\n");
                #endif
                stHDMITx_GetRxAudioFormatFromEDID stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetRxAudioFormatFromEDID_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetRxAudioFormatFromEDID_Compat));

                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8)*stCompatHDMITxArgs.u8BufSize);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Buffer, stCompatHDMITxArgs.pu8Buffer, sizeof(MS_U8)*stCompatHDMITxArgs.u8BufSize);
                    stHDMITxArgs.u8BufSize = stCompatHDMITxArgs.u8BufSize;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8Buffer, stHDMITxArgs.pu8Buffer, sizeof(MS_U8)*stCompatHDMITxArgs.u8BufSize);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetRxAudioFormatFromEDID_Compat));

                    free(stHDMITxArgs.pu8Buffer);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetRxAudioFormatFromEDID));

                    pData = stHDMITxArgs.pu8Buffer;
                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) *stHDMITxArgs.u8BufSize);
                    CPY_FROM_USER(stHDMITxArgs.pu8Buffer, pData, sizeof(MS_U8) *stHDMITxArgs.u8BufSize);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pu8Buffer, sizeof(MS_U8) *stHDMITxArgs.u8BufSize);
                    free(stHDMITxArgs.pu8Buffer);
                    stHDMITxArgs.pu8Buffer = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetRxAudioFormatFromEDID));

                }
            }
            break;

        case E_HDMITX_CMD_EDID_HDMISUPPORT:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_EDID_HDMISUPPORT\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_EDIDHDMISUPPORT, spt_HDMITX_EDIDHDMISUPPORT, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETRXIDMANUFACTURERNAME:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETRXIDMANUFACTURERNAME\r\n");
                #endif
                stHDMITx_GetRxIDManufacturerName stHDMITxArgs = {0};
#define DEF_EDID_MANUFACTORID_SIZE 3U
                if(IS_CMP_TASK())
                {
                    stHDMITx_GetRxIDManufacturerName_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetRxIDManufacturerName_Compat));

                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) * DEF_EDID_MANUFACTORID_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Buffer, stCompatHDMITxArgs.pu8Buffer, sizeof(MS_U8) * DEF_EDID_MANUFACTORID_SIZE);
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8Buffer, stHDMITxArgs.pu8Buffer, sizeof(MS_U8) * DEF_EDID_MANUFACTORID_SIZE);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetRxIDManufacturerName_Compat));

                    free(stHDMITxArgs.pu8Buffer);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetRxIDManufacturerName));

                    pData = stHDMITxArgs.pu8Buffer;
                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) * DEF_EDID_MANUFACTORID_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pu8Buffer, pData, sizeof(MS_U8) * DEF_EDID_MANUFACTORID_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pu8Buffer, sizeof(MS_U8) * DEF_EDID_MANUFACTORID_SIZE);
                    free(stHDMITxArgs.pu8Buffer);
                    stHDMITxArgs.pu8Buffer = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetRxIDManufacturerName));

                }
#undef DEF_EDID_MANUFACTORID_SIZE
            }
            break;

        case E_HDMITX_CMD_GETEDIDDATA:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETEDIDDATA\r\n");
                #endif
                stHDMITx_GetEDIDData stHDMITxArgs = {0};
#define DEF_EDID_BLOCK_SIZE 128U
                if(IS_CMP_TASK())
                {
                    stHDMITx_GetEDIDData_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetEDIDData_Compat));

                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) * DEF_EDID_BLOCK_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Buffer, stCompatHDMITxArgs.pu8Buffer, sizeof(MS_U8) * DEF_EDID_BLOCK_SIZE);
                    stHDMITxArgs.BlockIdx = stCompatHDMITxArgs.BlockIdx;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8Buffer, stHDMITxArgs.pu8Buffer, sizeof(MS_U8) * DEF_EDID_BLOCK_SIZE);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetEDIDData_Compat));

                    free(stHDMITxArgs.pu8Buffer);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetEDIDData));

                    pData = stHDMITxArgs.pu8Buffer;
                    stHDMITxArgs.pu8Buffer = malloc(sizeof(MS_U8) * DEF_EDID_BLOCK_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pu8Buffer, pData, sizeof(MS_U8) * DEF_EDID_BLOCK_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pu8Buffer, sizeof(MS_U8) * DEF_EDID_BLOCK_SIZE);
                    free(stHDMITxArgs.pu8Buffer);
                    stHDMITxArgs.pu8Buffer = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetEDIDData));


                }
#undef DEF_EDID_BLOCK_SIZE
            }
            break;

        case E_HDMITX_CMD_GETRX3DSTRUCTUREFROMEDID:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETRX3DSTRUCTUREFROMEDID\r\n");
                #endif
                stHDMITx_GetRx3DStructureFromEDID stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetRx3DStructureFromEDID_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetRx3DStructureFromEDID_Compat));

                    stHDMITxArgs.p3DStructure = malloc(sizeof(HDMITX_EDID_3D_STRUCTURE_ALL));

                    CMP_CPY_FROM_USER(stHDMITxArgs.p3DStructure, stCompatHDMITxArgs.p3DStructure, sizeof(HDMITX_EDID_3D_STRUCTURE_ALL));
                    stHDMITxArgs.timing = stCompatHDMITxArgs.timing;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.p3DStructure, stHDMITxArgs.p3DStructure, sizeof(HDMITX_EDID_3D_STRUCTURE_ALL));
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetRx3DStructureFromEDID_Compat));

                    free(stHDMITxArgs.p3DStructure);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetRx3DStructureFromEDID));

                    pData = stHDMITxArgs.p3DStructure;
                    stHDMITxArgs.p3DStructure = malloc(sizeof(HDMITX_EDID_3D_STRUCTURE_ALL));

                    CPY_FROM_USER(stHDMITxArgs.p3DStructure, pData, sizeof(HDMITX_EDID_3D_STRUCTURE_ALL));

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.p3DStructure, sizeof(HDMITX_EDID_3D_STRUCTURE_ALL));
                    free(stHDMITxArgs.p3DStructure);
                    stHDMITxArgs.p3DStructure = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetRx3DStructureFromEDID));

                }
            }
            break;

        case E_HDMITX_CMD_PKT_USER_DEFINE_CLEAR:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_PKT_USER_DEFINE_CLEAR\r\n");
            #endif
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_HDMITX_CMD_PKT_USER_DEFINE:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_PKT_USER_DEFINE\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_PKTUSERDEF, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_PKT_CONTENT_DEFINE:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_PKT_CONTENT_DEFINE\r\n");
                #endif
                stHDMITx_PKT_Content_Define stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_PKT_Content_Define_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_PKT_Content_Define_Compat));

                    stHDMITxArgs.data = malloc(sizeof(MS_U8) *stCompatHDMITxArgs.length);

                    CMP_CPY_FROM_USER(stHDMITxArgs.data, stCompatHDMITxArgs.data, sizeof(MS_U8) *stCompatHDMITxArgs.length);
                    stHDMITxArgs.packet_type = stCompatHDMITxArgs.packet_type;
                    stHDMITxArgs.length = stCompatHDMITxArgs.length;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.data, stHDMITxArgs.data, sizeof(MS_U8) *stCompatHDMITxArgs.length);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_PKT_Content_Define_Compat));

                    free(stHDMITxArgs.data);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_PKT_Content_Define));

                    pData = stHDMITxArgs.data;
                    stHDMITxArgs.data = malloc(sizeof(MS_U8) *stHDMITxArgs.length);

                    CPY_FROM_USER(stHDMITxArgs.data, pData, sizeof(MS_U8) *stHDMITxArgs.length);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.data, sizeof(MS_U8) * stHDMITxArgs.length);
                    free(stHDMITxArgs.data);
                    stHDMITxArgs.data = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_PKT_Content_Define));
                }
            }
            break;

        case E_HDMITX_CMD_SETVIDEOONOFF:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETVIDEOONOFF\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITx_SETVIDONOFF, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETCOLORFORMAT:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETCOLORFORMAT\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETCOLORFMT, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SET_VS_INFOFRAME:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SET_VS_INFOFRAME\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETVSINFOFRAME, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETVIDEOOUTPUTTIMING:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETVIDEOOUTPUTTIMING\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETVIDOUTPUTTIMING, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETVIDEOOUTPUTASEPCTRATIO:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETVIDEOOUTPUTASEPCTRATIO\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETVIDOUTPUTASPECTRATIO, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETVIDOUTPUTOVERSCANAFD, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETAUDIOONOFF:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETAUDIOONOFF\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETAUDONOFF, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETAUDIOFREQUENCY:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETAUDIOFREQUENCY\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETAUDFREQ, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD_II:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD_II\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETVIDOUTPUTOVERSCANAFD_II, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETAUDIOCONFIGURATION:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETAUDIOCONFIGURATION\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETAUDCONFIG, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETAUDIOSOURCEFORMAT:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETAUDIOSOURCEFORMAT\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETAUDSRCFMT, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETAVIINFOEXTCOLORIMETRY:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_EXIT\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETAVIINFOEXTCOLORIMETRY, spt_HDMITX_SETAVIINFOEXTCOLORIMETRY, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETAUDIOCTS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETAUDIOCTS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETAUDCTS, spt_HDMITX_GETAUDCTS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_MUTEAUDIOFIFO:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_MUTEAUDIOFIFO\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_MUTEAUDFIFO, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETHDCPKEY:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETHDCPKEY\r\n");
                #endif
                stHDMITx_GetHdcpKey stHDMITxArgs = {0};
#define DEF_HDCP14_KEY_SIZE 304U

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetHdcpKey_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetHdcpKey_Compat));

                    stHDMITxArgs.data = malloc(sizeof(MS_U8) * DEF_HDCP14_KEY_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.data, stCompatHDMITxArgs.data, sizeof(MS_U8) * DEF_HDCP14_KEY_SIZE);
                    stHDMITxArgs.useinternalkey = stCompatHDMITxArgs.useinternalkey;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.data, stHDMITxArgs.data, sizeof(MS_U8) * DEF_HDCP14_KEY_SIZE);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetHdcpKey_Compat));

                    free(stHDMITxArgs.data);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetHdcpKey));

                    pData = stHDMITxArgs.data;
                    stHDMITxArgs.data = malloc(sizeof(MS_U8) * DEF_HDCP14_KEY_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.data, pData, sizeof(MS_U8) * DEF_HDCP14_KEY_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.data, sizeof(MS_U8) * DEF_HDCP14_KEY_SIZE);
                    free(stHDMITxArgs.data);
                    stHDMITxArgs.data = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetHdcpKey));
                }
#undef DEF_HDCP14_KEY_SIZE
            }
            break;

        case E_HDMITX_CMD_GETBKSV:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETBKSV\r\n");
                #endif
                stHDMITx_GetBksv stHDMITxArgs = {0};
#define DEF_HDCP14_KSV_SIZE 5U

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetBksv_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetBksv_Compat));

                    stHDMITxArgs.pdata = malloc(sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pdata, stCompatHDMITxArgs.pdata, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pdata, stHDMITxArgs.pdata, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetBksv_Compat));

                    free(stHDMITxArgs.pdata);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetBksv));

                    pData = stHDMITxArgs.pdata;
                    stHDMITxArgs.pdata = malloc(sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pdata, pData, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pdata, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    free(stHDMITxArgs.pdata);
                    stHDMITxArgs.pdata = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetBksv));

                }
#undef DEF_HDCP14_KSV_SIZE
            }
            break;

        case E_HDMITX_CMD_GETAKSV:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETAKSV\r\n");
                #endif
                stHDMITx_GetAksv stHDMITxArgs = {0};
#define DEF_HDCP14_KSV_SIZE 5U

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetAksv_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetAksv_Compat));

                    stHDMITxArgs.pdata = malloc(sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pdata, stCompatHDMITxArgs.pdata, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pdata, stHDMITxArgs.pdata, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetAksv_Compat));

                    free(stHDMITxArgs.pdata);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetAksv));

                    pData = stHDMITxArgs.pdata;
                    stHDMITxArgs.pdata = malloc(sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pdata, pData, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pdata, sizeof(MS_U8) * DEF_HDCP14_KSV_SIZE);
                    free(stHDMITxArgs.pdata);
                    stHDMITxArgs.pdata = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetAksv));

                }
#undef DEF_HDCP14_KSV_SIZE
            }
            break;

        case E_HDMITX_CMD_SETHDCPONOFF:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETHDCPONOFF\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETHDCPONOFF, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETAVMUTE:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETAVMUTE\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETAVMUTE, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETAVMUTESTATUS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETAVMUTESTATUS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETAVMUTESTATUS, spt_HDMITX_GETAVMUTESTATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_HDCP_REVOCATIONKEY_LIST:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_HDCP_REVOCATIONKEY_LIST\r\n");
                #endif
                stHDMITx_HDCP_RevocationKey_List stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_HDCP_RevocationKey_List_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP_RevocationKey_List_Compat));

                    stHDMITxArgs.data = malloc(sizeof(MS_U8) *stCompatHDMITxArgs.size);
                    CMP_CPY_FROM_USER(stHDMITxArgs.data, stCompatHDMITxArgs.data, sizeof(MS_U8) *stCompatHDMITxArgs.size);
                    stHDMITxArgs.size = stCompatHDMITxArgs.size;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.data, stHDMITxArgs.data, sizeof(MS_U8) *stCompatHDMITxArgs.size);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_HDCP_RevocationKey_List_Compat));

                    free(stHDMITxArgs.data);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP_RevocationKey_List));

                    pData = stHDMITxArgs.data;
                    stHDMITxArgs.data = malloc(sizeof(MS_U8) *stHDMITxArgs.size);
                    CPY_FROM_USER(stHDMITxArgs.data, pData, sizeof(MS_U8) *stHDMITxArgs.size);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.data, sizeof(MS_U8) *stHDMITxArgs.size);
                    free(stHDMITxArgs.data);
                    stHDMITxArgs.data = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_HDCP_RevocationKey_List));
                }
            }
            break;

        case E_HDMITX_CMD_HDCP_REVOCATIONKEY_CHECK:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_HDCP_REVOCATIONKEY_CHECK\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_HDCP14REVOKEKEYCHK, spt_HDMITX_HDCP14REVOKEKEYCHK, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_HDCP_ISSRMSIGNATUREVALID:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_HDCP_ISSRMSIGNATUREVALID\r\n");
                #endif
                stHDMITx_HDCP_IsSRMSignatureValid stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_HDCP_IsSRMSignatureValid_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP_IsSRMSignatureValid_Compat));

                    stHDMITxArgs.data = malloc(sizeof(MS_U8) *stCompatHDMITxArgs.size);
                    CMP_CPY_FROM_USER(stHDMITxArgs.data, stCompatHDMITxArgs.data, sizeof(MS_U8) *stCompatHDMITxArgs.size);
                    stHDMITxArgs.size = stCompatHDMITxArgs.size;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_HDCP_IsSRMSignatureValid_Compat));

                    free(stHDMITxArgs.data);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP_IsSRMSignatureValid));

                    pData = stHDMITxArgs.data;
                    stHDMITxArgs.data = malloc(sizeof(MS_U8) *stHDMITxArgs.size);
                    CPY_FROM_USER(stHDMITxArgs.data, pData, sizeof(MS_U8) *stHDMITxArgs.size);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.data, sizeof(MS_U8) *stHDMITxArgs.size);
                    free(stHDMITxArgs.data);
                    stHDMITxArgs.data = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_HDCP_IsSRMSignatureValid));

                }
            }
            break;

        case E_HDMITX_CMD_GETHDCPSTATUS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETHDCPSTATUS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETHDCPSTATUS, spt_HDMITX_GETHDCPSTATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_HDCP_STARTAUTH:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_HDCP_STARTAUTH\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_HDCP14STARTAUTH, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETINTHDCPSTATUS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETINTHDCPSTATUS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETINTHDCPSTATUS, spt_HDMITX_GETINTHDCPSTATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETHDCP_PRESTATUS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETHDCP_PRESTATUS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETHDCP14PRESTATUS, spt_HDMITX_GETHDCP14PRESTATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_UNHDCPRXCONTROL:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_UNHDCPRXCONTROL\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_UNHDCPRXCTRL, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_HDCPRXFAILCONTROL:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_HDCPRXFAILCONTROL\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_HDCPRXFAILCTRL, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETLIBVER:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETLIBVER\r\n");
            #endif
            #if 0
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETLIBVER, spt_HDMITX_GETLIBVER, usbuffer_arg, sizeof(usbuffer_arg));
            #endif
            break;

        case E_HDMITX_CMD_GETINFO:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETINFO\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETINFO, spt_HDMITX_GETINFO, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETSTATUS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETSTATUS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETSTATUS, spt_HDMITX_GETSTATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETDBGLEVEL:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETDBGLEVEL\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETDBGLEVEL, spt_HDMITX_SETDBGLEVEL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETHPDGPIOPIN:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETHPDGPIOPIN\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETHPDGPIOPIN, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_ANALOGTUNING:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_ANALOGTUNING\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_ANALOGTUNING, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_FORCEHDMIOUTPUTMODE:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_FORCEHDMIOUTPUTMODE\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_FORCEHDMIOUTPUTMODE, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_FORCEHDMIOUTPUTCOLORFORMAT:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_FORCEHDMIOUTPUTCOLORFORMAT\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_FORCEHDMIOUTPUTCOLORFMT, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_DISABLEREGWRITE:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_DISABLEREGWRITE\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_DISABLEREGWRITE, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETEDIDPHYADR:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETEDIDPHYADR\r\n");
                #endif
                stHDMITx_GetEDIDPhyAdr stHDMITxArgs = {0};
#define DEF_EDID_PHYADDR_SIZE 2U

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetEDIDPhyAdr_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetEDIDPhyAdr_Compat));

                    stHDMITxArgs.pdata = malloc(sizeof(MS_U8) * DEF_EDID_PHYADDR_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pdata, stCompatHDMITxArgs.pdata, sizeof(MS_U8) * DEF_EDID_PHYADDR_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pdata, stHDMITxArgs.pdata, sizeof(MS_U8) * DEF_EDID_PHYADDR_SIZE);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetEDIDPhyAdr_Compat));

                    free(stHDMITxArgs.pdata);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetEDIDPhyAdr));

                    pData = stHDMITxArgs.pdata;
                    stHDMITxArgs.pdata = malloc(sizeof(MS_U8) * DEF_EDID_PHYADDR_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pdata, pData, sizeof(MS_U8) * DEF_EDID_PHYADDR_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pdata, sizeof(MS_U8) * DEF_EDID_PHYADDR_SIZE);
                    free(stHDMITxArgs.pdata);
                    stHDMITxArgs.pdata = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetEDIDPhyAdr));

                }
#undef DEF_EDID_PHYADDR_SIZE
            }
            break;

        case E_HDMITX_CMD_SETCECONOFF:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETCECONOFF\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETCECONOFF, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETCECSTATUS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_GETCECSTATUS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GETCECSTATUS, spt_HDMITX_GETCECSTATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_EDIDCHECKING:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_EDIDCHECKING111::: 0x%X\r\n", u32Cmd);
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_EDIDCHECKING, spt_HDMITX_EDIDCHECKING, usbuffer_arg, sizeof(usbuffer_arg));
            #if EN_HDMITX_ADP_LOG
            printk("end of E_HDMITX_CMD_EDIDCHECKING\r\n");
            #endif
            break;

        case E_HDMITX_CMD_RXBYPASS_MODE:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_RXBYPASS_MODE\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_RXBYPASSMODE, spt_HDMITX_RXBYPASSMODE, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_DISABLE_RXBYPASS:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_DISABLE_RXBYPASS\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_DISABLERXBYPASS, spt_HDMITX_DISABLERXBYPASS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_SETAKSV2R0INTERVAL:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETAKSV2R0INTERVAL\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETAKSV2R0INTERVAL, spt_HDMITX_SETAKSV2R0INTERVAL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_ISRXVALID:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_ISRXVALID\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_ISHDCPRXVALID, spt_HDMITX_ISHDCPRXVALID, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_GETCHIPCAPS:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETCHIPCAPS\r\n");
                #endif
                stHDMITx_GetChipCaps stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetChipCaps_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetChipCaps_Compat));

                    stHDMITxArgs.pRet = malloc(sizeof(MS_U32) * stCompatHDMITxArgs.ret_size);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pRet, stCompatHDMITxArgs.pRet, sizeof(MS_U32) * stCompatHDMITxArgs.ret_size);
                    stHDMITxArgs.eCapType = stCompatHDMITxArgs.eCapType;
                    stHDMITxArgs.ret_size = stCompatHDMITxArgs.ret_size;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pRet, stHDMITxArgs.pRet, sizeof(MS_U32) * stCompatHDMITxArgs.ret_size);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetChipCaps_Compat));

                    free(stHDMITxArgs.pRet);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetChipCaps));

                    pData = stHDMITxArgs.pRet;
                    stHDMITxArgs.pRet = malloc(sizeof(MS_U32) * stHDMITxArgs.ret_size);
                    CPY_FROM_USER(stHDMITxArgs.pRet, pData, sizeof(MS_U32) * stHDMITxArgs.ret_size);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pRet, sizeof(MS_U32) * stHDMITxArgs.ret_size);
                    free(stHDMITxArgs.pRet);
                    stHDMITxArgs.pRet = pData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetChipCaps));
                }
            }
            break;

        case E_HDMITX_CMD_SETPOWERSTATE:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_SETPOWERSTATE\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETPWRSTATE, spt_HDMITX_SETPWRSTATE, usbuffer_arg, sizeof(usbuffer_arg));
            break;
        case E_HDMITX_CMD_GETEDIDDATABLOCK:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETEDIDDATABLOCK\r\n");
                #endif
                stHDMITx_GetEdidDataBlocks stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetEdidDataBlocks_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetEdidDataBlocks_Compat));

                    stHDMITxArgs.pu8Data = malloc(sizeof(MS_U8) *stCompatHDMITxArgs.u32DataLen);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Data, stCompatHDMITxArgs.pu8Data, sizeof(MS_U8) *stCompatHDMITxArgs.u32DataLen);

                    stHDMITxArgs.enTagCode = stCompatHDMITxArgs.enTagCode;
                    stHDMITxArgs.enExtTagCode = stCompatHDMITxArgs.enExtTagCode;
                    stHDMITxArgs.u32DataLen = stCompatHDMITxArgs.u32DataLen;

                    stHDMITxArgs.pu32RealLen = malloc(sizeof(MS_U32));
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu32RealLen, stCompatHDMITxArgs.pu32RealLen, sizeof(MS_U32));

                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8Data, stHDMITxArgs.pu8Data, sizeof(MS_U8) *stCompatHDMITxArgs.u32DataLen);
                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu32RealLen, stHDMITxArgs.pu32RealLen, sizeof(MS_U32));
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetEdidDataBlocks_Compat));

                    free(stHDMITxArgs.pu8Data);
                    free(stHDMITxArgs.pu32RealLen);
                }
                else
                {
                    void *pData = NULL;
                    void *pLen = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetEdidDataBlocks));

                    pData = stHDMITxArgs.pu8Data;
                    stHDMITxArgs.pu8Data = malloc(sizeof(MS_U8) *stHDMITxArgs.u32DataLen);
                    CPY_FROM_USER(stHDMITxArgs.pu8Data, pData, sizeof(MS_U8) *stHDMITxArgs.u32DataLen);

                    pLen = stHDMITxArgs.pu32RealLen;
                    stHDMITxArgs.pu32RealLen = malloc(sizeof(MS_U32));
                    CPY_FROM_USER(stHDMITxArgs.pu32RealLen, pLen, sizeof(MS_U32));

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pData, stHDMITxArgs.pu8Data, sizeof(MS_U8) *stHDMITxArgs.u32DataLen);
                    CPY_TO_USER(pLen, stHDMITxArgs.pu32RealLen, sizeof(MS_U32));
                    free(stHDMITxArgs.pu8Data);
                    free(stHDMITxArgs.pu32RealLen);
                    stHDMITxArgs.pu8Data = pData;
                    stHDMITxArgs.pu32RealLen = pLen;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetEdidDataBlocks));

                }
            }
            break;
        case E_HDMITX_CMD_GETKSVLIST:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_GETKSVLIST\r\n");
                #endif
#define DEF_HDCP14_BSTATUS_SIZE 2U
                stHDMITx_GetKSVList stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_GetKSVList_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_GetKSVList_Compat));

                    stHDMITxArgs.pu8Bstatus = malloc(sizeof(MS_U8) * DEF_HDCP14_BSTATUS_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Bstatus, stCompatHDMITxArgs.pu8Bstatus, sizeof(MS_U8) * DEF_HDCP14_BSTATUS_SIZE);

                    stHDMITxArgs.pu8KSVList = malloc(sizeof(MS_U8) *stCompatHDMITxArgs.u16BufLen);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8KSVList, stCompatHDMITxArgs.pu8KSVList, sizeof(MS_U8) *stCompatHDMITxArgs.u16BufLen);

                    stHDMITxArgs.u16BufLen = stCompatHDMITxArgs.u16BufLen;

                    stHDMITxArgs.pu16KSVLength = malloc(sizeof(MS_U16));
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu16KSVLength, stCompatHDMITxArgs.pu16KSVLength, sizeof(MS_U16));

                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8Bstatus, stHDMITxArgs.pu8Bstatus, sizeof(MS_U8) * DEF_HDCP14_BSTATUS_SIZE);
                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8KSVList, stHDMITxArgs.pu8KSVList, sizeof(MS_U8) *stCompatHDMITxArgs.u16BufLen);
                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu16KSVLength, stHDMITxArgs.pu16KSVLength, sizeof(MS_U16));
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_GetKSVList_Compat));

                    free(stHDMITxArgs.pu8Bstatus);
                    free(stHDMITxArgs.pu8KSVList);
                    free(stHDMITxArgs.pu16KSVLength);
                }
                else
                {
                    void *pBStatus = NULL;
                    void *pData = NULL;
                    void *pLen = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_GetKSVList));

                    pBStatus = stHDMITxArgs.pu8Bstatus;
                    stHDMITxArgs.pu8Bstatus = malloc(sizeof(MS_U8) * DEF_HDCP14_BSTATUS_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pu8Bstatus, pBStatus, sizeof(MS_U8) * DEF_HDCP14_BSTATUS_SIZE);

                    pData = stHDMITxArgs.pu8KSVList;
                    stHDMITxArgs.pu8KSVList = malloc(sizeof(MS_U8) * stHDMITxArgs.u16BufLen);
                    CPY_FROM_USER(stHDMITxArgs.pu8KSVList, pData, sizeof(MS_U8) * stHDMITxArgs.u16BufLen);

                    pLen = stHDMITxArgs.pu16KSVLength;
                    stHDMITxArgs.pu16KSVLength = malloc(sizeof(MS_U16));
                    CPY_FROM_USER(stHDMITxArgs.pu16KSVLength, pLen, sizeof(MS_U16));

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pBStatus, stHDMITxArgs.pu8Bstatus, sizeof(MS_U8) * DEF_HDCP14_BSTATUS_SIZE);
                    CPY_TO_USER(pData, stHDMITxArgs.pu8KSVList, sizeof(MS_U8) * stHDMITxArgs.u16BufLen);
                    CPY_TO_USER(pLen, stHDMITxArgs.pu16KSVLength, sizeof(MS_U16));
                    free(stHDMITxArgs.pu8Bstatus);
                    free(stHDMITxArgs.pu8KSVList);
                    free(stHDMITxArgs.pu16KSVLength);
                    stHDMITxArgs.pu8Bstatus = pBStatus;
                    stHDMITxArgs.pu8KSVList = pData;
                    stHDMITxArgs.pu16KSVLength= pLen;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_GetKSVList));

                }
#undef DEF_HDCP14_BSTATUS_SIZE
            }
            break;

        case E_HDMITX_CMD_HDCP2ACCESSX74:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_HDCP2ACCESSX74\r\n");
                #endif
                stHDMITx_HDCP2AccessX74 stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_HDCP2AccessX74_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP2AccessX74_Compat));

                    stHDMITxArgs.u8PortIdx = stCompatHDMITxArgs.u8PortIdx;
                    stHDMITxArgs.u8OffsetAddr = stCompatHDMITxArgs.u8OffsetAddr;
                    stHDMITxArgs.u8OpCode = stCompatHDMITxArgs.u8OpCode;

                    stHDMITxArgs.pu8RdBuf = malloc(sizeof(MS_U8) * stCompatHDMITxArgs.u16RdLen);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8RdBuf, stCompatHDMITxArgs.pu8RdBuf, sizeof(MS_U8) * stCompatHDMITxArgs.u16RdLen);

                    stHDMITxArgs.u16RdLen = stCompatHDMITxArgs.u16RdLen;

                    stHDMITxArgs.pu8WRBuff = malloc(sizeof(MS_U8) * stCompatHDMITxArgs.u16WrLen);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8WRBuff, stCompatHDMITxArgs.pu8WRBuff, sizeof(MS_U8) * stCompatHDMITxArgs.u16WrLen);

                    stHDMITxArgs.u16WrLen = stCompatHDMITxArgs.u16WrLen;
                    stHDMITxArgs.bReturn = stCompatHDMITxArgs.bReturn;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);
                    stCompatHDMITxArgs.bReturn = stHDMITxArgs.bReturn;

                    CMP_CPY_TO_USER(stCompatHDMITxArgs.pu8RdBuf, stHDMITxArgs.pu8RdBuf, sizeof(MS_U8) * stCompatHDMITxArgs.u16RdLen);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMITxArgs, sizeof(stHDMITx_HDCP2AccessX74_Compat));

                    free(stHDMITxArgs.pu8RdBuf);
                    free(stHDMITxArgs.pu8WRBuff);
                }
                else
                {
                    MS_U8 *pRdData = NULL;
                    MS_U8 *pWrData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP2AccessX74));

                    pRdData = stHDMITxArgs.pu8RdBuf;
                    stHDMITxArgs.pu8RdBuf = malloc(sizeof(MS_U8) * stHDMITxArgs.u16RdLen);
                    CPY_FROM_USER(stHDMITxArgs.pu8RdBuf, pRdData, sizeof(MS_U8) * stHDMITxArgs.u16RdLen);

                    pWrData = stHDMITxArgs.pu8WRBuff;
                    stHDMITxArgs.pu8WRBuff = malloc(sizeof(MS_U8) * stHDMITxArgs.u16WrLen);
                    CPY_FROM_USER(stHDMITxArgs.pu8WRBuff, pWrData, sizeof(MS_U8) * stHDMITxArgs.u16WrLen);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    CPY_TO_USER(pRdData, stHDMITxArgs.pu8RdBuf, sizeof(MS_U8) * stHDMITxArgs.u16RdLen);
                    //CPY_TO_USER(pWrData, stHDMITxArgs.pu8WRBuff, sizeof(MS_U8) * stHDMITxArgs.u16WrLen);
                    free(stHDMITxArgs.pu8RdBuf);
                    free(stHDMITxArgs.pu8WRBuff);
                    stHDMITxArgs.pu8RdBuf = pRdData;
                    stHDMITxArgs.pu8WRBuff = pWrData;
                    CPY_TO_USER(pArgs, &stHDMITxArgs, sizeof(stHDMITx_HDCP2AccessX74));
                }
            }
            break;

        case E_HDMITX_CMD_HDCP2TxInit:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_HDCP2TxInit\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_HDCP2TXINIT, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_HDCP2TxEnableEncrypt:
            #if EN_HDMITX_ADP_LOG
            printk("E_HDMITX_CMD_HDCP2TxEnableEncrypt\r\n");
            #endif
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_HDCP2TXENABLEENC, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case E_HDMITX_CMD_HDCP2TxFillCipherKey:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_HDCP2TxFillCipherKey\r\n");
                #endif
#define DEF_HDCP22_RIV_SIZE 8U
#define DEF_HDCP22_KS_SIZE 16U

                stHDMITx_HDCP2TxFillCipherKey stHDMITxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMITx_HDCP2TxFillCipherKey_Compat stCompatHDMITxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP2TxFillCipherKey_Compat));

                    stHDMITxArgs.u8PortIdx = stCompatHDMITxArgs.u8PortIdx;

                    stHDMITxArgs.pu8Riv = malloc(sizeof(MS_U8) * DEF_HDCP22_RIV_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8Riv, stCompatHDMITxArgs.pu8Riv, sizeof(MS_U8) * DEF_HDCP22_RIV_SIZE);

                    stHDMITxArgs.pu8Riv = malloc(sizeof(MS_U8) * DEF_HDCP22_KS_SIZE);
                    CMP_CPY_FROM_USER(stHDMITxArgs.pu8KsXORLC128, stCompatHDMITxArgs.pu8KsXORLC128, sizeof(MS_U8) * DEF_HDCP22_KS_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    free(stHDMITxArgs.pu8Riv);
                    free(stHDMITxArgs.pu8KsXORLC128);
                }
                else
                {
                    MS_U8 *pRivData = NULL;
                    MS_U8 *pKsData = NULL;

                    CPY_FROM_USER(&stHDMITxArgs, pArgs, sizeof(stHDMITx_HDCP2TxFillCipherKey));

                    pRivData = stHDMITxArgs.pu8Riv;
                    stHDMITxArgs.pu8Riv = malloc(sizeof(MS_U8) * DEF_HDCP22_RIV_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pu8Riv, pRivData, sizeof(MS_U8) * DEF_HDCP22_RIV_SIZE);

                    pKsData = stHDMITxArgs.pu8KsXORLC128;
                    stHDMITxArgs.pu8KsXORLC128 = malloc(sizeof(MS_U8) * DEF_HDCP22_KS_SIZE);
                    CPY_FROM_USER(stHDMITxArgs.pu8KsXORLC128, pKsData, sizeof(MS_U8) * DEF_HDCP22_KS_SIZE);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMITxArgs);

                    free(stHDMITxArgs.pu8Riv);
                    free(stHDMITxArgs.pu8KsXORLC128);
                }
#undef DEF_HDCP22_RIV_SIZE
#undef DEF_HDCP22_KS_SIZE
            }

            break;

            case E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM\r\n");
                #endif
                ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_COLORRANGE_TRANSFORM, spt_HDMITX_COLORRANGE_TRANSFORM, usbuffer_arg, sizeof(usbuffer_arg));
            }
            break;

            case E_HDMITX_CMD_SSC_ENABLE:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_SSC_ENABLE\r\n");
                #endif
                ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETSSCENABLE, spt_HDMITX_SETSSCENABLE, usbuffer_arg, sizeof(usbuffer_arg));
            }
            break;

            case E_HDMITX_CMD_SET_COLORIMETRY:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_SETCOLORIMETRY\r\n");
                #endif
                ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETCOLORIMETRY, spt_HDMITX_SETCOLORIMETRY, usbuffer_arg, sizeof(usbuffer_arg));
            }
            break;

            case E_HDMITX_CMD_GET_FULL_RX_STATUS:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_SETCOLORIMETRY\r\n");
                #endif
                ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_GET_FULL_RX_STATUS, spt_HDMITX_GET_FULL_RX_STATUS, usbuffer_arg, sizeof(usbuffer_arg));
            }
            break;

            case E_HDMITX_CMD_TIMING_CAPABILITY_CHECK:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_TIMING_CAPABILITY_CHECK\r\n");
                #endif
                ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_CHECK_LEGAL_TIMING, spt_HDMITX_CHECK_LEGAL_TIMING, usbuffer_arg, sizeof(usbuffer_arg));
            }
            break;

            case E_HDMITX_CMD_SETVIDEOUTPUTTIMINGBYCUSTOMER:
            {
                #if EN_HDMITX_ADP_LOG
                printk("E_HDMITX_CMD_SetVideoOutputTimingByCustomer\r\n");
                #endif
                ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMITX_SETVIDEOUTPUTTIMINGBYCUSTOMER, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            }
            break;
    };

    return ulReturnValue;
}

#endif //#ifndef _ADP_HDMITX_C_

