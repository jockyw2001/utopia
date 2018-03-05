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
#if defined (MSOS_TYPE_LINUX)
#ifndef _FRHSL_DATALINK_
#define _FRHSL_DATALINK_

#include "MsTypes.h"

//---------------------------------------------------
// Message's Body
//---------------------------------------------------
// MS_U8 | MS_U8 | MS_U32 | MS_U32 |
// ---------------------------------
// GetCardStatusReq
// 0x01  | SN    |
// GetCardStatusResp
// 0x02  | SN    |used    |total   |
// GetDataReq
// 0x03  | SN    |req.size|
// GetDataResp
// 0x04  | SN    |size    | DATA (size) bytes ...
// SendCommandReq
// 0x05  | SN    |snd.size| DATA (size) bytes ...
// SendCommandResp
// 0x06  | SN    |err.code|
// SendFilterArrayReq
// 0x07  | SN    |snd.size| Byte array from Genie
// SendFilterArrayResp
// 0x08  | SN    |(MS_U8)[0|1]|

#define LINK_PROTOCOL_VERSION           0x02

#define LINK_BUFFER_SIZE        (32)
#define LINK_MAX_COMMAND        (sizeof(ST_HSL_CMD_REQ))
// connection timeout [s].
#define LINK_CONNECTION_TIMEOUT (15)

#define LINK_HEADER_SIZE                2
#define LINK_DATA_GET_CARD_STATUS       0
#define LINK_DATA_RESPONSE_CARD_STATUS  8
#define LINK_DATA_GET_HSL_DATA          4
#define LINK_DATA_RESPONSE_HSL_DATA     4
#define LINK_DATA_SEND_HSL_CMD          4
#define LINK_DATA_RESPONSE_HSL_CMD      4

typedef enum EnLinkCommandTag
{
    EN_LINK_CMD_NONE                 = 0x00,
    EN_LINK_CMD_GET_CARD_STATUS      = 0x01,
    EN_LINK_CMD_RESP_CARD_STATUS     = 0x02,
    EN_LINK_CMD_GET_HSL_DATA         = 0x03,
    EN_LINK_CMD_RESP_HSL_DATA        = 0x04,
    EN_LINK_CMD_SEND_HSL_CMD         = 0x05,
    EN_LINK_CMD_RESP_HSL_CMD         = 0x06,
    EN_LINK_CMD_SND_FILTER_MX        = 0x07,
    EN_LINK_CMD_RSP_FILTER_MX        = 0x08
} EN_LINK_CMD;

/* ===================================== */
/* COMMAND LAYER */

typedef enum EnCmdErrorCodeTag
{
    EN_CMD_EC_OK                      = 0,
    EN_CMD_EC_VERSION,
    EN_CMD_EC_DATA_SIZE,
    EN_CMD_EC_AT_FAILURE,
    EN_CMD_EC_THREAD_FAIL,
    EN_CMD_EC_CMD_UNKNOWN,
    EN_CMD_EC_LAST
} EN_CMD_ERROR_CODE;

typedef enum EnCommandIdTag
{
    EN_CMD_TEST_NONE                  = 0x00,
    EN_CMD_TEST_AT                    = 0x01,
    EN_CMD_TEST_LOAD                  = 0x02,
    EN_CMD_TEST_CMD_LAST
} EN_COMMAND_ID;

typedef enum EnResponseIdTag
{
    EN_RESP_TEST_AT = 0,
    EN_RESP_TEST_AT_ECHO = 1,
    EN_RESP_TEST_CMD_LAST
} EN_RESPONSE_ID;

#define CMD_MAX_AT              (128)
#define RSP_MAX_AT              (1024)
#define CMD_MAX_FILTER          (10000)  //maximum single use local variable
#define HSL_MAX_LOG_POINT_SIZE (20*1000)   // Maximum size of GKI signal that can be handled by Genie.
#define HSL_MAX_SILO_ARRAY_SIZE (HSL_MAX_LOG_POINT_SIZE - 4)  // The -4 accounts for length field of HslLogPointRawData (with 2 bytes padding)

typedef struct StHslSelftestCmd
{
    MS_U32  mRunTime;
    MS_U32  mThreadDelay;
    MS_U16  mThreadNo;
    MS_U16  mCntSimpleLP;
    MS_U16  mCntPrintf1p;
    MS_U16  mParamPrintf1p;
    MS_U16  mCntArray16;
    MS_U16  mParamArray16Size;
    MS_U16  mCntArray32;
    MS_U16  mParamArray32Size;
    MS_U16  mCntTypedef;
} ST_HSL_SELFTEST_CMD;

typedef struct StTestStruct
{
    MS_U32 a;
    MS_U32 b;
    MS_U32 c;
} ST_TEST_STRUCT;

typedef union U_HslCmdReqTag
{
  char    atCmd[CMD_MAX_AT];  /**< \assoc STRING */
  ST_HSL_SELFTEST_CMD loadTestCmd;
} U_HslCmdReq;

typedef struct ST_HSL_CMD_REQTag
{
    MS_U8                 ver;
    /** \assoc UNION \ref U_Cmd */
    EN_COMMAND_ID         cmdId;
    U_HslCmdReq           U_Cmd;
} ST_HSL_CMD_REQ;


#define HSL_CMD_RESP_LOG_POINT_ID (0x000A)  /* Hard coded log point id to containt the ST_HSL_CMD_RESP structure */


typedef char AtCommandEchoString[CMD_MAX_AT];
typedef struct AtCmdEchoTag
{
  MS_BOOL result;
  AtCommandEchoString atCmd; /**< \assoc STRING */
} AtCmdEcho;

typedef char AtCommandRespString[RSP_MAX_AT];
typedef union U_HslCmdRespTag
{
  AtCommandRespString atResponse;  /**< \assoc STRING */
  AtCmdEcho atCmdEcho;
} U_HslCmdResp;

/** Please update the function PackIntoHslCmdResp() when adding new members before U_Resp union.*/
typedef struct ST_HSL_CMD_RESPTag
{
    /** \assoc UNION \ref U_Resp */
    EN_RESPONSE_ID         respId;
    U_HslCmdResp            U_Resp;
} ST_HSL_CMD_RESP;


/** This type is used to wrap around every array HSL log point.
    This is not used for logged typedef arrays which use a different HSL macro. */
typedef struct HslArrayLogPointTag
{
  MS_U8  rawData[HSL_MAX_SILO_ARRAY_SIZE];
}HslArrayLogPoint;

/** This type is used to wrap around every printf HSL log point to present
    them as serialised SILO GKI signals. */
typedef struct HslLogPointRawDataTag
{
    /** \assoc VAR_ARRAY \ref rawData */
    MS_U16 length;
    MS_U8  rawData[HSL_MAX_SILO_ARRAY_SIZE];
}HslLogPointRawData;

/** This type is used on PC to interpret those Log points coming via log
    point ID 0x9 which is effectively a wrapper for string info. */
typedef struct HslMessageLogPointTag
{
  char message[RSP_MAX_AT];  /**< \assoc STRING */
}HslMessageLogPoint;


void PackAtRespIntoHslCmdResp(const char* a_atResp);
void PackIntoHslCmdResp(EN_RESPONSE_ID a_respId, const void* a_body);

#endif
#endif
