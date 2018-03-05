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
////////////////////////////////////////////////////////////////////////////////
#define _HAL_NSK2_C

////////////////////////////////////////////////////////////////////////////////
/// @file halEMMflt.c
/// @author MStar Semiconductor Inc.
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif

#include "MsCommon.h"
#include "MsTypes.h"
#include "drvSYS.h"

#include "halNSK2.h"
#include "regNSK2.h"
#include "MsOS.h"

#include "regTSP.h" // todo
#include "halTSP.h" // todo
#include "drvDSCMB.h"
#include "halDSCMB.h" // todo

#include "drvNSK2Type.h"
#include "drvCA.h"

#include "nsk_282.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////

//#define SlowClockTest

#define POLLING_CNT     100

#ifdef SlowClockTest
#define BUSYCHECK_CNT   0x1000000
#else
#define BUSYCHECK_CNT   100
#endif

static MS_U32 _gBasicAddr = 0;
static MS_U32 _gNSK2_Addr = 0;
static MS_U32 _gOTP_Addr = 0;
static MS_U32 _gOTP_CTRL_Addr = 0;
static MS_U32 _gNI_Addr = 0;
static MS_U32 _gRSA_Addr = 0;
static MS_U32 _gKeyTable_Addr = 0;
static MS_U32 _gNDSJTagPwd_Addr = 0;
static MS_U32 _gCipherCH0_Addr = 0;
static MS_U32 _gCryptoDMA_Addr = 0;

static MS_U32 _g32NSK2HalDbgLv = NSK2_DBGLV_DEBUG;

#define HALNSK2_DBG(lv, x, args...)   if (lv <= _g32NSK2HalDbgLv ) \
                                        {printf(x, ##args);}

//bank 0x1700
#define OTP_REG(addr)           (*((volatile MS_U32*)(_gOTP_Addr + addr )))

//bank 0x1a13
#define OTP_CTRL_REG(addr)      (*((volatile MS_U32*)(_gOTP_CTRL_Addr + (addr<<2) )))

//bank 0x1620
#define NI_REG(addr)            (*((volatile MS_U32*)(_gNI_Addr + (addr<<2) )))

//bank 1630
#define RSA_REG(addr)           (*((volatile MS_U32*)(_gRSA_Addr + (addr<<2) )))

//bank 1626
#define KeyTable_REG(addr)      (*((volatile MS_U32*)(_gKeyTable_Addr + (addr<<2) )))

//bank xxxxxx
#define NDSJTagPwd_REG(addr)    (*((volatile MS_U16*)(_gNDSJTagPwd_Addr + (addr<<2) )))

//bank 1621
#define CMCHANNEL0_REG(addr)    (*((volatile MS_U32*)(_gCipherCH0_Addr + (addr<<2) )))


//#define FPGAMode
//#define NSK2SelfTest

//#define TestGenIn
//#define ReadSwitchInfoNSK2


#define StatusCheck(status)   do { if(status == FALSE) \
                                     { \
                                         printf("status error %s, %d\n",__FUNCTION__,__LINE__); \
                                         return status;   \
                                     } \
                                 } while(0);

#define NSK2HDI_CMCHANNEL_WITH_ALL_PARITY   ( NSK2HDI_CMCHANNEL_CLEAR_PARITY | NSK2HDI_CMCHANNEL_EVEN_PARITY |  NSK2HDI_CMCHANNEL_EVEN_CLEAR_PARITY | NSK2HDI_CMCHANNEL_ODD_PARITY | \
                                              NSK2HDI_CMCHANNEL_ODD_CLEAR_PARITY | NSK2HDI_CMCHANNEL_ODD_EVEN_PARITY | NSK2HDI_CMCHANNEL_ODD_EVEN_CLEAR_PARITY )

#define CMCHANNEL_NSK_All_CAPABILITY        (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG)

////////////////////////////////////////////////////////////////////////////////
// Local variable
////////////////////////////////////////////////////////////////////////////////

static MS_BOOL _gReset = FALSE;
static MS_BOOL _gCheckBusyFlag = FALSE;
static MS_U32 dead_polling_cnt = 1;

static cmchannel_group_capability_descriptor_t cm_capb =
{
    .descriptor_tag = NSK2HDI_CMCHANNELGROUP_CAPABILITY_DESC_TAG,
    .descriptor_length = sizeof(cmchannel_group_capability_descriptor_t) - 2,
    .number_of_channels[3] = 40,
    .switch_combination_bitmap[0] = 0xff,
    .user_context[0] = 'M',
    .user_context[1] = 'S',
    .user_context[2] = 't',
    .user_context[3] = 'a',
    .user_context[4] = 'r',
    .user_context[5] = 0x00,
    .user_context[6] = 0x00,
    .user_context[7] = 0x00,
};

static cmchannel_group_algorithm_record_descriptor_t cm_algo[] =
{

//-------------------------------LSA---------------------------------------//
    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDI_CBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDI_RCBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDD_CBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDD_RCBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SYNAMEDIA_AES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG ,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_AES_ECB_CLEARTAIL,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CIPLUS_AES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = CMCHANNEL_NSK_All_CAPABILITY,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SCTE41_DES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG ,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SCTE52_DES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG ,
        .capability = CMCHANNEL_NSK_All_CAPABILITY,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_TDES_ECB_CLEARTAIL,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG ,
    },


//-------------------------------LSA---------------------------------------//
//10



//-------------------------------ESA---------------------------------------//
    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_DVB_CSA2,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },


    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_DVB_CSA_CONFORMANCE,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_DVB_CSA3,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },


    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDI_CBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = CMCHANNEL_NSK_All_CAPABILITY,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SYNAMEDIA_AES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_AES_ECB_CLEARTAIL,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },


    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CIPLUS_AES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = CMCHANNEL_NSK_All_CAPABILITY,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SCTE41_DES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SCTE52_DES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = CMCHANNEL_NSK_All_CAPABILITY,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_TDES_ECB_CLEARTAIL,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },


    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_MULTI2_TS,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG),
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },
//-------------------------------ESA---------------------------------------//
//24

//-------------------------------LDA---------------------------------------//
    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDI_CBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

        {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDI_RCBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDD_CBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CPCM_LSA_MDD_RCBC,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SYNAMEDIA_AES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG ,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_AES_ECB_CLEARTAIL,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_CIPLUS_AES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        //.capability = CMCHANNEL_NSK_All_CAPABILITY,
        .capability = (NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG | NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG),
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SCTE41_DES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG ,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_SCTE52_DES,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = CMCHANNEL_NSK_All_CAPABILITY,
    },

    {
        .descriptor_tag = NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG,
        .descriptor_length = sizeof(cmchannel_group_algorithm_record_descriptor_t) - 2,
        .algorithm_type = NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE,
        .algorithm = NSK2HDI_SPROFILE_TDES_ECB_CLEARTAIL,
        .parity_combination_bitmap = NSK2HDI_CMCHANNEL_WITH_ALL_PARITY,
        .capability = NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG ,
    },

//-------------------------------LDA---------------------------------------//
//34

};

static M2MChGr_Capa_Desc_t m2m_capa_desc = {
    .descriptor_tag = NSK2HDI_M2MCHANNELGROUP_CAPABILITY_DESC_TAG,
    .descriptor_length = sizeof(M2MChGr_Capa_Desc_t) - 2,
    .number_of_channels[3] = 0x1,
    .user_context[0] = 0x4e,
    .user_context[1] = 0x44,
    .user_context[2] = 0x53,
    .user_context[3] = 0x5f,
    .user_context[4] = 0x44,
    .user_context[5] = 0x52,
    .user_context[6] = 0x4d,
    .user_context[7] = 0x00,
};

static M2MChGr_AlgoRecord_Desc_t m2m_algo[] =
{
    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_DES_ECB_CLR_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV1,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV2,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_DES_CBC_CLR_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_TDES_ECB_CLR_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV1,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV2,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_TDES_CBC_CLR_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_AES_ECB_CLR_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_IV1,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_IV1,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_AES_CBC_CLR_CLR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_RC4_64,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },

    {
        .descriptor_tag = NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG,
        .descriptor_length = sizeof(M2MChGr_AlgoRecord_Desc_t) - 2,
        .algorithm = NSK2HDI_SPROFILE_M2M_AES_CTR,
        .capability[0] = (MS_U8)(NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG>>24),
        .capability[3] = NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG,
    },
};


static DMA_Capa_Desc_t dma_capa_desc = {
    .descriptor_tag = NSK2HDI_DMA_CAPABILITY_DESC_TAG,
    .descriptor_length = sizeof(DMA_Capa_Desc_t) - 2,
    .maximum_data_size[0] = 0,
    .maximum_data_size[1] = 0,
    .maximum_data_size[2] = 0,
    .maximum_data_size[3] = 1,
    .minimum_data_size[0] = 0x01,
    .data_size_granularity[0] = 0x4,
    .data_alignment[0] = 0x4,
    .capability[0] = NSK2HDI_DMA_CONTIGUOUS_MEMORY_TYPE,
};
////////////////////////////////////////////////////////////////////////////////
// Global variable
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

extern MS_U32 TSP32_IdrR(TSP32 *preg);
extern void TSP32_IdrW(TSP32 *preg, MS_U32 value);
extern int ChkForNskTest(volatile unsigned int *OtpMemBase, volatile unsigned int *RSABase,
                         volatile unsigned int *NIBase, volatile unsigned int *CCh0Base);
extern int HW_CompareKTvalid(unsigned int pid_no, unsigned int scb, unsigned int compare, volatile unsigned int *KTBase);
extern void acpu_w_pidslotmap (unsigned char indx, unsigned char wmux, unsigned char wdata, volatile unsigned int *KTBase);
extern void acpu_r_keyslot (unsigned char key_indx, unsigned char key_field, volatile unsigned int *KTBase);


////////////////////////////////////////////////////////////////////////////////
// Function Declaration
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////


static MS_U32 HAL_NSK2_ReadReg(MS_U32 u32RegAddr)
{
    if(_gReset == FALSE)
        return 0;
    MS_U32 u32reg;
    MS_U32 u32Data;
    u32reg = u32RegAddr + _gNSK2_Addr;
    u32Data = (*(volatile MS_U32*)(u32reg));

    HALNSK2_DBG(NSK2_DBGLV_ARRAY, "read NSK2 %x = %x\n",u32RegAddr,u32Data);

    return u32Data;
}

static void HAL_NSK2_WriteReg(MS_U32 u32RegAddr,MS_U32 u32Data)
{
    if(_gReset == FALSE)
        return ;
    MS_U32 u32reg;
    u32reg = u32RegAddr + _gNSK2_Addr;
    (*(volatile MS_U32*)(u32reg)) = u32Data;

    HALNSK2_DBG(NSK2_DBGLV_ARRAY, "write NSK2 %x = %x\n",u32RegAddr,u32Data);
}

static MS_U32 HAL_NSK2_KIW_BusyPolling(void)
{
    MS_U32 xiu_rdata;
    MS_U32 cnt = 0;

    xiu_rdata = NI_REG(REG_NI_STATUS);
    while( (xiu_rdata & NI_KIW_BUSY) && (cnt < POLLING_CNT) )
    {
        xiu_rdata = NI_REG(REG_NI_STATUS);
        HALNSK2_DBG(NSK2_DBGLV_INFO," read NI (STATUS = %x)\n",xiu_rdata);

        cnt ++;
        MsOS_DelayTask(1);
    }

    if(cnt >= POLLING_CNT)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"KIW_BusyPolling TimeOut\n");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static MS_U32 HAL_NSK2_CheckBusy(void)
{
    //printf("_gCheckBusyFlag = %x\n",_gCheckBusyFlag);
	if(_gCheckBusyFlag == TRUE)
	{
        MS_U32 cnt = 0;

    #ifdef SlowClockTest
        while( (cnt < BUSYCHECK_CNT) && (HAL_NSK2_ReadReg(REG_NSK2_ACPU_WARNING) & NSK2_ACPU_BUSY) )
        {
            cnt ++;
            //MsOS_DelayTaskUs(1);
        }
    #else
        while( (cnt < BUSYCHECK_CNT) && (HAL_NSK2_ReadReg(REG_NSK2_ACPU_WARNING) & NSK2_ACPU_BUSY) )
        {
            cnt ++;
            MsOS_DelayTask(1);
        }
    #endif

        if(cnt == BUSYCHECK_CNT)
        {
            HALNSK2_DBG(NSK2_DBGLV_ERR, "NSK2 is still busy\n");
            return FALSE;
        }
	}
	else
	{
		HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NSK2 does't check busy\n");
	}
    return TRUE;
}


static void HAL_NSK2_DeadPolling(void)
{
#if 0
    MS_U32 polling_cnt = dead_polling_cnt;
    while(polling_cnt){
        MsOS_DelayTask(10);
        HALNSK2_DBG(NSK2_DBGLV_ERR, "Error, Dead Polling\n");
        polling_cnt --;
    }

    //read ACPU Error....
    printf("ACPU Error = %x\n", HAL_NSK2_ReadReg(REG_NSK2_ACPU_ERROR));
#endif
}


void HAL_NSK2_OTP_Get(MS_U32 Addr, MS_U8 Msb, MS_U8 Lsb, MS_U32 *pValue)
{
    MS_U32 u32Data;

    u32Data = OTP_REG(Addr);

    //read back first....
    //HALNSK2_DBG(NSK2_DBGLV_DEBUG, "read OTP %x = %x\n",Addr,u32Data);

    //write value next

    if( (Msb == 31) && (Lsb == 0) )
    {
        *pValue = u32Data;
    }
    else
    {
        *pValue = ((u32Data & BMASK(Msb:Lsb) ) >> Lsb);
    }

}

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////

MS_U32 HAL_NSK2_Init(void)
{
#if 0
    MS_U32 u32Data;
    {
        RSA_REG(REG_RSA_CLK_ENABLE) |= RSA_PM_NSKCLK_ENABLE;
        MsOS_DelayTaskUs(1);

        u32Data = RSA_REG(REG_RSA_CLK_ENABLE);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "RSA 0x1 = %x\n",u32Data);
    }
#endif


//self test....
#ifdef NSK2SelfTest
    NI_REG(REG_NI_NSK2_FREERUN) |= NI_NSK2_FREERUN_ENABLE;


    MsOS_DelayTaskUs(1);
    u32Data = NI_REG(REG_NI_NSK2_FREERUN);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI 0x1 = %x\n",u32Data);
#endif


    //set OTP timing...
    (*((volatile MS_U8*)(_gBasicAddr + (0x1A27CC<<1)))) = 0xd;
    (*((volatile MS_U8*)(_gBasicAddr + (0x1A27CD<<1)))) = 0x17;
    (*((volatile MS_U8*)(_gBasicAddr + (0x1A27CE<<1)))) = 0x01;
    MsOS_DelayTask(1);
    (*((volatile MS_U8*)(_gBasicAddr + (0x1A27CE<<1)))) = 0x00;

    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"finish NSK2.1 init\n");

    return TRUE;
}

MS_U32 HAL_NSK2_Exit(void)
{
    NI_REG(REG_NI_NSK2_CTRL) &= (~NI_NSK2_RESET_DISABLE);
    return TRUE;
}

MS_U32 HAL_NSK2_UnlockOTPCtrl(void)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO, "OTP control to unlock NDS secret key\n");
    OTP_CTRL_REG(0x10) = 0x99885a5a;
    OTP_CTRL_REG(0x11) = 0x00114433;
    OTP_CTRL_REG(0x12) = 0x23456789;
    OTP_CTRL_REG(0x13) = 0xabcdef01;

    return TRUE;
}

MS_U32 HAL_NSK2_ColdReset(void)
{
    MS_U32 u32Data;

    HALNSK2_DBG(NSK2_DBGLV_DEBUG, "%s \n",__FUNCTION__);

    {
        RSA_REG(REG_RSA_CLK_ENABLE) |= RSA_PM_NSKCLK_ENABLE;
        MsOS_DelayTaskUs(1);

        u32Data = RSA_REG(REG_RSA_CLK_ENABLE);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "RSA 0x1 = %x\n",u32Data);
    }

    u32Data = NI_REG(REG_NI_NSK2_CTRL);

    NI_REG(REG_NI_NSK2_CTRL) = (u32Data & (~NI_NSK2_RESET_DISABLE));
    MsOS_DelayTaskUs(1);

    u32Data &= (~NI_N2ROM_PD);
    //u32Data &= (~NI_N2ROM_PD);

//bit 0 set to 1...
    NI_REG(REG_NI_NSK2_CTRL) = u32Data | NI_TS2NSK_ENABLE | NI_NSK2_CLK_ENABLE | NI_NSK2_RESET_DISABLE;

    //MsOS_DelayTaskUs(1);

    u32Data = NI_REG(REG_NI_NSK2_CTRL);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI 0x0 = %x\n",u32Data);

    _gReset = TRUE;
    _gCheckBusyFlag = TRUE;

    HAL_NSK2_EnableInt();
    return TRUE;
}


MS_U32 HAL_NSK2_EndSubtest(void)
{
    MS_U32 u32Data;

    HALNSK2_DBG(NSK2_DBGLV_DEBUG, "%s \n",__FUNCTION__);

    HAL_NSK2_DisableInt();

    u32Data = NI_REG(REG_NI_NSK2_CTRL);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG, "REG_NI_NSK2_CTRL = %x \n",u32Data);

    if(u32Data & NI_NSK2_RESET_DISABLE)
    {

        NI_REG(REG_NI_NSK2_CTRL) = (u32Data & (~NI_NSK2_RESET_DISABLE));
        //NI_REG(REG_NI_NSK2_CTRL) = 0;
        //printf("wait here\n");
        //while(1);
        MsOS_DelayTaskUs(1);
    }

    _gReset = FALSE;
    _gCheckBusyFlag = FALSE;
    return TRUE;
}


void HAL_NSK2_SetBase(MS_U32 u32Base)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO, "u32Base = %x\n",u32Base);

    _gBasicAddr = u32Base;

    _gOTP_Addr  = _gBasicAddr + REG_OTP_BASE;
    _gRSA_Addr  = _gBasicAddr + REG_RSA_BASE;
    _gOTP_CTRL_Addr  = _gBasicAddr + REG_OTP_CTRL_BASE;
    _gNSK2_Addr = _gBasicAddr + REG_NSK2_BASE;

    _gNI_Addr   = _gBasicAddr + REG_NI_BASE;

    _gKeyTable_Addr  = _gBasicAddr + REG_KEY_TABLE_BASE;
    _gNDSJTagPwd_Addr= _gBasicAddr + REG_JTAG_PWD_BASE;
    _gCipherCH0_Addr = _gBasicAddr + REG_CIPHER_CH0_BASE;
    _gCryptoDMA_Addr = _gBasicAddr + REG_CRYPTO_DMA_BASE;
    HALNSK2_DBG(NSK2_DBGLV_INFO, "%s _gNSK2_Addr : %x\n", __FUNCTION__, _gNSK2_Addr);

    //OTP_CTRL_REG(0x3A) = 0x0;
}




//compare the contect of a buffer in the NSK2's memory map to an expected value.
MS_U32 HAL_NSK2_CompareMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 CompareLens,
                           MS_U32 CompareSim, MS_U32 ExpectResult, void *pGolden)
{
    MS_U32 u32ReadData, u32GoldenData;
    MS_U32 u32Addr = StartAddr;
    MS_U32 *pGoldenData = (MS_U32 *)pGolden;
    MS_U32 u32CompLens = CompareLens;
    MS_U32 status = TRUE;
    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n",__FUNCTION__);
    //check lengths
    if(u32CompLens == 0)
    {
        HALNSK2_DBG(NSK2_DBGLV_DEBUG,"%s CompareLens = %x\n",__FUNCTION__,u32CompLens);
    }

    do
    {
        HAL_NSK2_CheckBusy();

        u32ReadData = HAL_NSK2_ReadReg(u32Addr);
        u32GoldenData =  *pGoldenData;

        if(u32ReadData != u32GoldenData)
        {
            HALNSK2_DBG(NSK2_DBGLV_ERR,"%s, Addr = %x, u32ReadData = %x, u32GoldenData = %x\n",__FUNCTION__,u32Addr, u32ReadData, u32GoldenData);
            HAL_NSK2_DeadPolling();
            //return FALSE;
            status = FALSE;
        }
        else
        {
            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"Data Correct, Addr = %x, u32GoldenData = %x\n",u32Addr, u32GoldenData);
        }

        pGoldenData ++;
        u32Addr += 4;
        u32CompLens --; //32 bit bus

    } while(u32CompLens > 0);

    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"%s, successful = %x\n",__FUNCTION__,StartAddr);
    return status;
}

MS_U32 HAL_NSK2_Compare(MS_U32 StartAddr, MS_U32 CompareLens, MS_U32 CompareSim,
                        MS_U32 Mask, MS_U32 ExpectResult)
{

    MS_U32 u32ReadData, u32GoldenData = ExpectResult;
    MS_U32 u32Addr = StartAddr;

    StatusCheck(HAL_NSK2_CheckBusy());

    u32ReadData = HAL_NSK2_ReadReg(u32Addr);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"Mask = %x\n",Mask);
    u32ReadData &= Mask;

    if(u32ReadData != u32GoldenData)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%s fail, Addr = %x, u32ReadData = %x, u32GoldenData = %x\n",__FUNCTION__,u32Addr, u32ReadData, u32GoldenData);
        HAL_NSK2_DeadPolling();
        return FALSE;
    }
    else
    {
        HALNSK2_DBG(NSK2_DBGLV_DEBUG,"compare data correct, Addr = %x, u32GoldenData = %x\n",u32Addr,u32GoldenData);
    }

    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"%s, successful\n",__FUNCTION__);
    return TRUE;
}

MS_U32 HAL_NSK2_WriteMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 WriteLens,
                         void *pWriteData)
{

    MS_U32 *pWriteD = (MS_U32 *)pWriteData;
    MS_U32 u32Data;
    while(WriteLens > 0)
    {
        StatusCheck(HAL_NSK2_CheckBusy());

        u32Data = *pWriteD ;

        HAL_NSK2_WriteReg(StartAddr,u32Data);
        HALNSK2_DBG(NSK2_DBGLV_ARRAY,"WriteMem (Addr,Data) = (%x,%x)\n",StartAddr,u32Data);

        WriteLens --;
        StartAddr += 4;
        pWriteD ++;
    }

    StatusCheck(HAL_NSK2_CheckBusy());
    return TRUE;
}

MS_U32 HAL_NSK2_WriteSFR(MS_U32 StartAddr, MS_U32 Data)
{
    StatusCheck(HAL_NSK2_CheckBusy());
    HALNSK2_DBG(NSK2_DBGLV_ARRAY,"WriteSFR (Addr,Data) = (%x,%x)\n",StartAddr,Data);
    HAL_NSK2_WriteReg(StartAddr,Data);
    StatusCheck(HAL_NSK2_CheckBusy());
    return TRUE;
}

MS_U32 HAL_NSK2_NSKBasicInitializationComplete(void)
{
    //$display($time,"NS  NSKBasicInitializationComplete ");
    //xiu_w_ni(16'h6,4'b0001,32'h0000000F);

    MS_U32 data;
    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_NSKBasicInitializationComplete\n");
    StatusCheck(HAL_NSK2_CheckBusy());

    data = NI_REG(REG_NI_COMMAND);
    //NI_REG(REG_NI_COMMAND) = (data | NI_NSKBIComplete | NI_COMMAND_START);
    NI_REG(REG_NI_COMMAND) = (NI_NSKBIComplete | NI_COMMAND_START);

    MsOS_DelayTaskUs(1);
    //NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_NopNop);
    NI_REG(REG_NI_NSK2_CLK_CSA) = NSK2_EN_CSA_VAR;


    return TRUE;
}

MS_U8 HAL_NSK2_SCBTransToHW(MS_U8 ForceSCB)
{
	MS_U8 NewForceSCB = 0;

	NewForceSCB = ForceSCB;
	if(ForceSCB == 0)
	{
		NewForceSCB = 1;
	}
	else if(ForceSCB == 1)
	{
		NewForceSCB = 0;
	}

	return NewForceSCB;
}


MS_U32 HAL_NSK2_WriteESA(MS_U8 ESASelect, MS_U8 ESASubSelect, MS_U8 pid_no)
{
#if 0
    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_WriteESA pid_no = %x, ESASelect = %x, ESASubSelect = %x\n",pid_no, ESASelect, ESASubSelect);
    //scb = 2'b00; pid_no = 1; ESAselect = 4'h0; ESAsubselect =4'h0;
    //wdata = {10'b0,ESAselect[3:0],ESAsubselect[2:0],3'b0,pid_no[12:0]};
    //xiu_w_ni(16'h7,4'b1111,wdata[31:0]);
    //xiu_w_ni(16'h6,4'b1111,32'h00000003);
    //xiu_rdata = 32'h8; while(xiu_rdata[3]===1'b1)begin xiu_r_ni(16'hc); end
    StatusCheck(HAL_NSK2_CheckBusy());

    MS_U32 data;

    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_WriteESA pid_no = %x\n",pid_no);
    data = (((MS_U32)ESASelect<<NI_WriteESA_ESASel_Shift) & NI_WriteESA_ESASel_MASK) +
           (((MS_U32)ESASubSelect<<NI_WriteESA_ESASubSel_Shift) & NI_WriteESA_ESASubSel_MASK)  +
           ((MS_U32)pid_no & NI_WriteESA_PidNo);


    HALNSK2_DBG(NSK2_DBGLV_INFO," write NI (REG_NI_PARAMETERS = %x)\n",data);
    NI_REG(REG_NI_PARAMETERS) = data;
    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteESA);

    StatusCheck(HAL_NSK2_KIW_BusyPolling());
#endif
    return TRUE;
}

MS_U32 HAL_NSK2_WriteTransportKey(MS_U8 SCB, MS_U8 ForceSCB, void *pLabel, MS_U8 pid_no)
{
#if 0
    //$display($time,"NS  WriteTransportKey 0134 00 01 IV__0");
    //wait_nsk_busy;
    //xiu_r_ni(16'hd);
    //if((xiu_rdata & 7)==0) begin
    //    scb = 2'b00; fscb = 2'b00; pid_no = 1;
    //    wdata = {12'b0,fscb[1:0],scb[1:0],3'b0,pid_no[12:0]};
    //    xiu_w_ni(16'h7,4'b1111,wdata[31:0]);
    //    xiu_w_ni(16'hb,4'b1111,32'h00000000);
    //    xiu_w_ni(16'ha,4'b1111,32'h00000000);
    //    xiu_w_ni(16'h9,4'b1111,32'h00000000);
    //    xiu_w_ni(16'h8,4'b1111,32'h00000000);
    //    xiu_w_ni(16'h6,4'b1111,32'h00000005);
    //    xiu_rdata = 32'h8; while(xiu_rdata[3]===1'b1)begin xiu_r_ni(16'hc); end
    //end
    //else $display("WriteTransportKey abnormal ignored: KteDest not zero");
#define N  0xfffffff0
#define N1 0xfffffff1
#define N2 0xfffffff2

    MS_U32 data = 0;
    MS_U32 data2 = 0;
	MS_U32 pIV[4] = {0,0,0,0};

	MS_U32* pIV_copy = (MS_U32*)pLabel;

	if((pLabel != NULL) && (pLabel != (MS_U32*)N) && (pLabel != (MS_U32*)N1) && (pLabel != (MS_U32*)N2))
	{
		memcpy(pIV,pIV_copy,16);
	}

    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_WriteTransportKey pid_no = %x, SCB = %x, ForceSCB = %x\n",pid_no, SCB, ForceSCB);
    StatusCheck(HAL_NSK2_CheckBusy());

    data = NI_REG(REG_NI_KTE_STATUS);
    if( ( data & NI_KTE_DEST_MASK ) == 0  )
    {

		ForceSCB = HAL_NSK2_SCBTransToHW(ForceSCB);

        HALNSK2_DBG(NSK2_DBGLV_INFO,"pid_no: %x\n", pid_no);
        data2 = pid_no + ( ( (MS_U32)SCB<<NI_WriteTKey_SCB_Shift) & NI_WriteTKey_SCB_MASK)
                + ( ( (MS_U32)ForceSCB<<NI_WriteTKey_FSCB_Shift) & NI_WriteTKey_FSCB_MASK) ;


        NI_REG(REG_NI_PARAMETERS) = data2;
        NI_REG(REG_NI_IV_127_96) = pIV[0];
        NI_REG(REG_NI_IV_95_64) = pIV[1];
        NI_REG(REG_NI_IV_63_31) = pIV[2];
        NI_REG(REG_NI_IV_31_00) = pIV[3];
        NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteTransportKey);

        HALNSK2_DBG(NSK2_DBGLV_INFO,"NI 7 : %x, NI 6 = %x\n", NI_REG(REG_NI_PARAMETERS) , NI_REG(REG_NI_COMMAND));

        StatusCheck(HAL_NSK2_KIW_BusyPolling());
    }
    else
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"WriteTransportKey abnormal ignored: KteDest not zero\n");
        return FALSE;
    }

    HAL_NSK2_ReadSwitchFromNSK2();
#endif
    return TRUE;
}

MS_U32 HAL_NSK2_wait_kte_valid(void)
{
    MS_U32 xiu_rdata = HAL_NSK2_ReadReg(REG_NSK2_KTE_VALID);
    MS_U32 cnt = 0;
    while( ( (xiu_rdata & NSK2_KTE_VALID_TRUE) == 0 ) && (cnt < POLLING_CNT) )
    {
        xiu_rdata = HAL_NSK2_ReadReg(REG_NSK2_KTE_VALID);
        HALNSK2_DBG(NSK2_DBGLV_INFO,"xiu_rdata = %x\n",xiu_rdata);
        cnt ++;
        MsOS_DelayTask(1);
    }

    if(cnt == POLLING_CNT)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"HAL_NSK2_wait_kte_valid time out\n");
        return FALSE;
    }

    return TRUE;
}

MS_U32 HAL_NSK2_CompareKTE(MS_U32 reserved_1, MS_U32 reserved_2, void *pLabel)
{

#ifdef FPGAMode
    MS_U32 data;
    MS_U32 *pKTEGolden = (MS_U32 *)pLabel;
    MS_U32 KTE_Index = REG_KT_KEYS_START_FPGA;
    MS_U32 status = TRUE;

    StatusCheck(HAL_NSK2_CheckBusy());
    //StatusCheck(HAL_NSK2_wait_kte_valid());
    data = NI_REG(REG_NI_NSK2_KTE_VALID_FPGA);
    HALNSK2_DBG(NSK2_DBGLV_INFO,"NSK2_KTE_VALID_FPGA = %x\n",data);

    for(KTE_Index = REG_KT_KEYS_END_FPGA; KTE_Index<=REG_KT_KEYS_START_FPGA; KTE_Index--)
    {
        data = NI_REG(KTE_Index);

        if(data != *pKTEGolden)
        {
            HALNSK2_DBG(NSK2_DBGLV_ERR," %x, (%x , %x)\n",KTE_Index,data,*pKTEGolden);
            status = FALSE;
        }

        pKTEGolden ++;
    }

    return status;

#else

#endif

    return TRUE;
}

MS_U32 HAL_NSK2_CompareOut(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 HighDWord, MS_U32 LowDWord)
{

   //wait_nsk_busy;
    //$display($time,"NS  CompareOut 1 1 0000001555400000");
    //xiu_r_ni(16'd24); genout = xiu_rdata; genout = genout << 32;
    //xiu_r_ni(16'd23); genout = genout | {6'b0,xiu_rdata[31:0]};
    //$display("CompareOut read PS",genout[37:0]);
    //    if(genout!==38'h0000001555400000)begin
    //$display("CompareOut Expected=38'h0000001555400000 Read=38'hPS",genout[37:0]);
    //    nsk2_GeneralOut_test_fail = 1;
    //    #2000;
    //$finish;
    //end


    MS_U32 high_data,low_data;
    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_CompareOut HighDWord = %x, LowDWord = %x\n",HighDWord, LowDWord);
    StatusCheck(HAL_NSK2_CheckBusy());
    low_data = NI_REG(REG_NI_COMPARE_GENOUT_L);
    high_data = (NI_REG(REG_NI_COMPARE_GENOUT_H)&NI_GENOUT_H_MASK);
    HALNSK2_DBG(NSK2_DBGLV_INFO,"NI 24 = %x , 23 = %x\n",high_data,low_data);

    if( (HighDWord != high_data) || (LowDWord!= low_data) )
    {
        HAL_NSK2_DeadPolling();
        return FALSE;
    }
    else
    {
        HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_CompareOut successfully\n");
    }

    return TRUE;
}

MS_U32 HAL_NSK2_SetRNG(MS_U32 reserved_1,MS_U32 RNG_Value)
{

    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_SetRNG = %x\n",RNG_Value);
    NI_REG(REG_NI_SW_SET_RNG) = (RNG_Value&NI_SW_RNG_MASK);
    return TRUE;
}

MS_U32 HAL_NSK2_WriteM2MKey(void *pIV, MS_U8 SubAlgo)
{
#if 0
    //wait_nsk_busy;
    //WriteM2MKey IV__1  0
    //$display($time,"NS  WriteM2MKey IV__1  0");
    //wdata = {12'b0,4'h0,16'b0};
    //xiu_w_ni(16'h7,4'b1111,wdata[31:0]);
    //xiu_w_ni(16'hb,4'b1111,32'h00010203);
    //xiu_w_ni(16'ha,4'b1111,32'h04050607);
    //xiu_w_ni(16'h9,4'b1111,32'h08090a0b);
    //xiu_w_ni(16'h8,4'b1111,32'h0c0d0e0f);
    //xiu_w_ni(16'h6,4'b1111,32'h00000007);
    //xiu_rdata = 32'h8; while(xiu_rdata[3]===1'b1)begin xiu_r_ni(16'hc); end

    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_WriteM2MKey, SubAlgo = %x \n", SubAlgo);
    HAL_NSK2_CheckBusy();

    MS_U32 wdata = 0;
    MS_U32 pWIV[4];

    memset(pWIV, 0x0, 4*4);
    if(pIV != NULL)
    {
        memcpy(pWIV,pIV,4*4);
    }


    wdata = ( ((MS_U32)SubAlgo << NI_WriteM2MKey_Shift) & NI_WriteM2MKey_MASK); //write sub algorithm....[19:16]
    NI_REG(REG_NI_PARAMETERS) = wdata;

    NI_REG(REG_NI_IV_127_96) = pWIV[0];
    NI_REG(REG_NI_IV_95_64)  = pWIV[1];
    NI_REG(REG_NI_IV_63_31)  = pWIV[2];
    NI_REG(REG_NI_IV_31_00)  = pWIV[3];

    HALNSK2_DBG(NSK2_DBGLV_INFO,"pwIV = (%x, %x, %x, %x)\n",pWIV[0],pWIV[1],pWIV[2],pWIV[3]);
    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteM2MKey); //write M2M key and start...
#endif
    return TRUE;
}

MS_U32 HAL_NSK2_WriteSCPUKey(void)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    StatusCheck(HAL_NSK2_CheckBusy());
    NI_REG(REG_NI_COMMAND) = (NI_WriteSCPUKey | NI_COMMAND_START);
    MsOS_DelayTaskUs(1);

    return HAL_NSK2_ReadKTEResp();
}

static MS_U32 KeyNum = 0;
MS_U32 HAL_NSK2_WriteReservedKey(void)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    StatusCheck(HAL_NSK2_CheckBusy());
    NI_REG(REG_NI_COMMAND) = (NI_WriteReservedKey | NI_COMMAND_START);
    MsOS_DelayTaskUs(1);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"write key = %x, %x\n", NI_REG(REG_NI_COMPARE_GENOUT_L), NI_REG(REG_NI_COMPARE_GENOUT_H));
    KeyNum = NI_REG(REG_NI_COMPARE_GENOUT_L)>>17;

    return HAL_NSK2_ReadKTEResp();
}

MS_U32 HAL_NSK2_GetReserveKeyNum(void)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s KeyNum = %x\n", __FUNCTION__,KeyNum);
    return KeyNum;
}

MS_U32 HAL_NSK2_DriveKteAck(void)
{
    //DriveKteAck
    //$display($time,"NS  DriveKteAck");
    //xiu_w_ni(16'h6,4'b1111,32'h0000000B);

    StatusCheck(HAL_NSK2_CheckBusy());
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);

    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_DriveAck);
    MsOS_DelayTaskUs(1);
    StatusCheck(HAL_NSK2_CheckBusy());
    return TRUE;
}

MS_U32 HAL_NSK2_SetJTagPswd(void)
{

#if 0
#define JTagPwd0Addr 0x64
#define InputPwd0Addr 0x4
    MS_U32 JTagPwd0[4];

    NDSJTagPwd_REG(3) = 0x0;
    NDSJTagPwd_REG(2) = 0x0;
    NDSJTagPwd_REG(1) = 0x0;
    NDSJTagPwd_REG(0) = 0x1;

    NDSJTagPwd_REG(0x14) = 0x1;

    JTagPwd0[0] = KeyTable_REG(JTagPwd0Addr);
    JTagPwd0[1] = KeyTable_REG((JTagPwd0Addr+1));
    JTagPwd0[2] = KeyTable_REG((JTagPwd0Addr+2));
    JTagPwd0[3] = KeyTable_REG((JTagPwd0Addr+3));

    HALNSK2_DBG(0,"JTagPwd0 = %x, %x, %x, %x\n",JTagPwd0[0],JTagPwd0[1],JTagPwd0[2],JTagPwd0[3]);

    NDSJTagPwd_REG((InputPwd0Addr+7)) = (MS_U16) ((JTagPwd0[3]>>16)&0xffff);
    NDSJTagPwd_REG((InputPwd0Addr+6)) = (MS_U16) (JTagPwd0[3]&0xffff);
    NDSJTagPwd_REG((InputPwd0Addr+5)) = (MS_U16) ((JTagPwd0[2]>>16)&0xffff);
    NDSJTagPwd_REG((InputPwd0Addr+4)) = (MS_U16) (JTagPwd0[2]&0xffff);
    NDSJTagPwd_REG((InputPwd0Addr+3)) = (MS_U16) ((JTagPwd0[1]>>16)&0xffff);
    NDSJTagPwd_REG((InputPwd0Addr+2)) = (MS_U16) (JTagPwd0[1]&0xffff);
    NDSJTagPwd_REG((InputPwd0Addr+1)) = (MS_U16) ((JTagPwd0[0]>>16)&0xffff);
    NDSJTagPwd_REG((InputPwd0Addr+0)) = (MS_U16) (JTagPwd0[0]&0xffff);

    NDSJTagPwd_REG(0x14) = 0x7;
#else
    MS_U32 jtag_status;
    jtag_status = KeyTable_REG(88);
    //88 bit 0 and 1 = 1

    HALNSK2_DBG(NSK2_DBGLV_INFO,"KeyTable 88 jtag_status = %x\n",jtag_status);
#endif

    return TRUE;
}

//MS_BOOL HAL_CA_OTP_GetCfg(MS_U32 u32Idx, MS_U32 *pu32Value);

MS_BOOL HAL_NSK2_CheckPubOTPConfig(void *pCheck)
{
#if 0
OTP bits	OTP name	         function
[127:64]	V_PubOtpUniqueID	 Unique ID, 0x3da8~0x3daf
[63:48]	    V_PubOtpGP	         General Purpose Field (bit-by-bit lock),3b08~3b09
[47:40]	    V_PubOtpMinConfVer	 Minimum Configuration Version,3b14
[39:36]	    V_PubOtpRSAIndex	 RSA Index, 3b10
[35:32]	    V_PubOtpBID	         BlackBox ID, 0x3b0c
[31:16]	    V_PubOtpVID	         Version ID, 0x3b04
[15:0]	    V_PubOtpOID	         Owner ID, 0x3b00
#endif

    MS_U32 *pDataArray;
    MS_U32 OTP_PubOtpOID,OTP_PubOtpVID,OTP_PubOtpBID,OTP_PubOtpRsaIndex,OTP_PubOtpMinConfVer ;
    MS_U32 OTP_PubOtpGP,OTP_PubOtpUniqueID[2];

    MS_U32 CHK_PubOtpOID,CHK_PubOtpVID,CHK_PubOtpBID,CHK_PubOtpRsaIndex,CHK_PubOtpMinConfVer ;
    MS_U32 CHK_PubOtpGP,CHK_PubOtpUniqueID[2];

    MS_BOOL status = TRUE;
    if(pCheck == NULL)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, pCheck = NULL\n",__LINE__,__FUNCTION__);
        return FALSE;
    }
    pDataArray = (MS_U32 *)pCheck;

    HAL_NSK2_OTP_Get(0x3da8,31,0, &OTP_PubOtpUniqueID[0]);
    HAL_NSK2_OTP_Get(0x3dac,31,0, &OTP_PubOtpUniqueID[1]);

    CHK_PubOtpUniqueID[1] = pDataArray[0];
    CHK_PubOtpUniqueID[0] = pDataArray[1];

    HALNSK2_DBG(NSK2_DBGLV_INFO,"uniqueID = (%x%x, %x%x)\n",CHK_PubOtpUniqueID[1],CHK_PubOtpUniqueID[0],OTP_PubOtpUniqueID[1],OTP_PubOtpUniqueID[0]);

    if( (CHK_PubOtpUniqueID[1] != OTP_PubOtpUniqueID[1]) || (CHK_PubOtpUniqueID[0] != OTP_PubOtpUniqueID[0]))
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, UniqueID WRONG\n",__LINE__,__FUNCTION__);
        status = FALSE;
    }

    HAL_NSK2_OTP_Get(0x3b08,15,0, &OTP_PubOtpGP);
    CHK_PubOtpGP = (pDataArray[2]>>16)&0xffff;

    if(OTP_PubOtpGP != CHK_PubOtpGP)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, PubOtpGP WRONG\n",__LINE__,__FUNCTION__);
        printf("OtpGP = (%x, %x)\n",CHK_PubOtpGP,OTP_PubOtpGP);
        status = FALSE;
    }

    HAL_NSK2_OTP_Get(0x3b14,7,0, &OTP_PubOtpMinConfVer);
    CHK_PubOtpMinConfVer = (pDataArray[2]>>8)&0xff;

    if(OTP_PubOtpMinConfVer != CHK_PubOtpMinConfVer)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, PubOtpMinConfVer WRONG\n",__LINE__,__FUNCTION__);
        printf("OtpMinConfVer = (%x, %x)\n",CHK_PubOtpMinConfVer,OTP_PubOtpMinConfVer);
        status = FALSE;
    }

    HAL_NSK2_OTP_Get(0x3b10,3,0, &OTP_PubOtpRsaIndex);
    CHK_PubOtpRsaIndex = (pDataArray[2]>>4)&0xf;

    if(OTP_PubOtpRsaIndex != CHK_PubOtpRsaIndex)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, PubOtpRsaIndex WRONG\n",__LINE__,__FUNCTION__);
        printf("OtpRsaIndex = (%x, %x)\n",CHK_PubOtpRsaIndex,OTP_PubOtpRsaIndex);
        status = FALSE;
    }

    HAL_NSK2_OTP_Get(0x3b0C,3,0, &OTP_PubOtpBID);
    CHK_PubOtpBID = (pDataArray[2])&0xf;

    if(OTP_PubOtpBID != CHK_PubOtpBID)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, PubOtpBID WRONG\n",__LINE__,__FUNCTION__);
        printf("OtpBID = (%x, %x)\n",CHK_PubOtpBID,OTP_PubOtpBID);
        status = FALSE;
    }

    HAL_NSK2_OTP_Get(0x3b04,15,0, &OTP_PubOtpVID);
    CHK_PubOtpVID = (pDataArray[3]>>16)&0xffff;

    if(OTP_PubOtpVID != CHK_PubOtpVID)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, PubOtpVID WRONG\n",__LINE__,__FUNCTION__);
        printf("OtpVID = (%x, %x)\n",CHK_PubOtpVID,CHK_PubOtpVID);
        status = FALSE;
    }

    HAL_NSK2_OTP_Get(0x3b00,15,0, &OTP_PubOtpOID);
    CHK_PubOtpOID = (pDataArray[3])&0xffff;

    if(OTP_PubOtpOID != CHK_PubOtpOID)
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"%d, %s, PubOtpOID WRONG\n",__LINE__,__FUNCTION__);
        printf("OtpOID = (%x, %x)\n",CHK_PubOtpOID,OTP_PubOtpOID);
        status = FALSE;
    }

    MS_U32 KeyValid;
    HAL_NSK2_OTP_Get(0x3b1c,31,24, &KeyValid);
    HALNSK2_DBG(NSK2_DBGLV_INFO,"KeyValid = %x\n",KeyValid);

    MS_U32 CheckSum[4];
    HAL_NSK2_OTP_Get(0x3c30,31,0, &CheckSum[0]);
    HAL_NSK2_OTP_Get(0x3c34,31,0, &CheckSum[1]);
    HAL_NSK2_OTP_Get(0x3c38,31,0, &CheckSum[2]);
    HAL_NSK2_OTP_Get(0x3c3c,31,0, &CheckSum[3]);
    HALNSK2_DBG(NSK2_DBGLV_INFO,"CheckSum = %x, %x, %x, %x\n",CheckSum[0],CheckSum[1],CheckSum[2],CheckSum[3]);
    return status;
}

static void HAL_NSK2_ReadDataTrans(MS_U32 read_data, MS_U8 *data)
{
    data[3] = (MS_U8)(read_data & 0xff);
    data[2] = (MS_U8)((read_data>>8) & 0xff);
    data[1] = (MS_U8)((read_data>>16) & 0xff);
    data[0] = (MS_U8)((read_data>>24) & 0xff);
}

static MS_U32 HAL_NSK2_WriteDataTrans(MS_U8 *data)
{
    MS_U32 write_data = 0;

    write_data = (MS_U32)data[3] + ((MS_U32)data[2]<<8) + ((MS_U32)data[1]<<16) + ((MS_U32)data[0]<<24) ;
    return write_data;
}

MS_U32 HAL_NSK2_ReadData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    MS_U32 read_data, read_data_size;
    MS_U8  *read_ptr;

    if(data_size < 4)
    {
        return FALSE;
    }

    read_data_size = data_size;
    read_ptr = data;

    while(read_data_size >= 4)
    {
        StatusCheck(HAL_NSK2_CheckBusy());

        read_data = HAL_NSK2_ReadReg(addr_offset);
        HAL_NSK2_ReadDataTrans(read_data,read_ptr);

        addr_offset += 4;
        read_ptr += 4;
        read_data_size -= 4;
    }

    return TRUE;
}

MS_U32 HAL_NSK2_WriteData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    MS_U32 write_data, write_data_size;
    MS_U8  *write_ptr;

    if(data_size < 4)
    {
        return FALSE;
    }

    write_data_size = data_size;
    write_ptr = data;

    while(write_data_size >= 4)
    {
        StatusCheck(HAL_NSK2_CheckBusy());

        write_data = HAL_NSK2_WriteDataTrans(write_ptr);
        HAL_NSK2_WriteReg(addr_offset,write_data);

        addr_offset += 4;
        write_ptr   += 4;
        write_data_size -= 4;
    }


    return TRUE;
}

MS_U32 HAL_NSK2_ReadData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    MS_U32 read_data, read_data_size;
    MS_U8  *read_ptr;

    if(data_size < 4)
    {
        return FALSE;
    }

    read_data_size = data_size;
    read_ptr = data;

    while(read_data_size >= 4)
    {
        StatusCheck(HAL_NSK2_CheckBusy());

        read_data = HAL_NSK2_ReadReg(addr_offset);
        HAL_NSK2_ReadDataTrans(read_data,read_ptr);

        //HALNSK2_DBG(0,"addr = %x, read_data = %x\n",addr_offset,read_data);
        addr_offset += 4;
        read_ptr += 4;
        read_data_size -= 4;
    }

    return TRUE;
}

MS_U32 HAL_NSK2_WriteData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    MS_U32 write_data, write_data_size;
    MS_U8  *write_ptr;

    if(data_size < 4)
    {
        return FALSE;
    }

    write_data_size = data_size;
    write_ptr = data;

    while(write_data_size >= 4)
    {
        StatusCheck(HAL_NSK2_CheckBusy());

        write_data = HAL_NSK2_WriteDataTrans(write_ptr);
        HAL_NSK2_WriteReg(addr_offset,write_data);
        //HALNSK2_DBG(0,"addr = %x, write_data = %x\n",addr_offset, write_data);
        addr_offset += 4;
        write_ptr   += 4;
        write_data_size -= 4;
    }

    return TRUE;
}


MS_U32 HAL_NSK2_ReadData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    MS_U32 read_data, read_data_size;
    MS_U32 *read_ptr;
    //MS_U32 test_read_data;

    if(data_size < 1)
    {
        return FALSE;
    }

    read_data_size = data_size;
    read_ptr = data;

    while(read_data_size >= 1)
    {
        StatusCheck(HAL_NSK2_CheckBusy());

        read_data = HAL_NSK2_ReadReg(addr_offset);
        *read_ptr = read_data;

        //HALNSK2_DBG(0,"addr = %x, read_data = %x\n",addr_offset,read_data);
        addr_offset += 4;
        read_ptr ++;
        read_data_size --;
    }

    return TRUE;
}

MS_U32 HAL_NSK2_WriteData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    MS_U32 write_data, write_data_size;
    MS_U32 *write_ptr;

    if(data_size < 1)
    {
        return FALSE;
    }

    write_data_size = data_size;
    write_ptr = data;

    while(write_data_size >= 1)
    {
        StatusCheck(HAL_NSK2_CheckBusy());

        write_data = *write_ptr;
        HAL_NSK2_WriteReg(addr_offset,write_data);
        //HALNSK2_DBG(0,"addr = %x, write_data = %x\n",addr_offset, write_data);
        addr_offset += 4;
        write_ptr   ++;
        write_data_size --;
    }


    return TRUE;
}

MS_U32 HAL_NSK2_EnableInt(void)
{
    MS_U32 u32IntReg = 0;
/*
#define REG_NSK2_ACPU_INT       0xFC08
    #define NSK2_INT_CMD_EXIT           __BIT0
    #define NSK2_INT_ASYNC_EVENT        __BIT1
    #define NSK2_INT_ILLEGAL_CMD        __BIT2
    #define NSK2_INT_ILLEGAL_ACCESS     __BIT3
    #define NSK2_INT_RESET              __BIT4
    #define NSK2_INT_HANG               __BIT5
    #define NSK2_INT_KTE_VALID          __BIT6
    #define NSK2_INT_MASK_CLEAR         __BIT31
*/

    u32IntReg = (NSK2_INT_CMD_EXIT | NSK2_INT_ASYNC_EVENT | NSK2_INT_ILLEGAL_CMD | \
                NSK2_INT_RESET | NSK2_INT_HANG | NSK2_INT_KTE_VALID | NSK2_INT_MASK_CLEAR);

    HAL_NSK2_WriteReg(REG_NSK2_ACPU_INT,u32IntReg);
    return TRUE;
}

MS_U32 HAL_NSK2_DisableInt(void)
{
    MS_U32 u32IntReg = 0;

    HAL_NSK2_WriteReg(REG_NSK2_ACPU_INT,u32IntReg);
    return TRUE;
}


MS_U32 HAL_NSK2_GetIntStatus(void)
{
    MS_U32 u32IntValue;
    u32IntValue = HAL_NSK2_ReadReg(REG_NSK2_ACPU_INT);

    //HALNSK2_DBG(NSK2_DBGLV_DEBUG,"Int status = %x\n",u32IntValue);
    return u32IntValue;
}

MS_U32 HAL_NSK2_ClearInt(MS_U32 u32IntValue)
{
    //HALNSK2_DBG(NSK2_DBGLV_DEBUG,"Clear Int = %x\n",u32IntValue);
    HAL_NSK2_WriteReg(REG_NSK2_ACPU_INT,u32IntValue);
    return TRUE;
}


MS_U32 HAL_NSK2_WriteControl(MS_U32 control)
{
    HAL_NSK2_WriteReg(REG_NSK2_ACPU_CTRL_BLOCK, control);
    return TRUE;
}


MS_U32 HAL_NSK2_WriteCommand(MS_U32 command)
{
    HAL_NSK2_WriteReg(REG_NSK2_ACPU_CMD, command);
    return TRUE;
}


MS_U32 HAL_NSK2_GetMaxXConn(void)
{
    return MaximumXConnection;
}


MS_U32 HAL_NSK2_CMChannelNum(void)
{
    return 1;
}


typedef struct
{
    MS_U8   u8Tag;
    MS_U8   u8Length;
    MS_U32  Offset;
    MS_U32  MSB;
    MS_U32  LSB;
} ASST_t;


static ASST_t K6liteAsst[] =
{
    //the first tag is the same called pub otp, we should implement one function to obtain this,
    //becasue this the same as HW test, we can check at HW Test also.
    {0x20,  9,  0x3DA8,  63,   0}, //U_OTP_v_pubOtpUniqueID1
    {0x21, 16,  0x7C00, 127,   0}, //Bulk data 1
    {0x22, 16,  0x7C10, 127,   0}, //Bulk data 2
    {0x23, 16,  0x7C20, 127,   0}, //Bulk data 3
    {0x24, 16,  0x7C30, 127,   0}, //Bulk data 4

    {0x40,  1,  0x3B20,  27,  24}, //U_OTP_allow_illegalNDSFlagChk
    {0x41,  1,  0x3B24,   3,   0}, //U_OTP_ena_ACPUUseNSK2
    {0x42,  1,  0x3B24,   7,   4}, //U_OTP_ena_DBUSUseNSK2
    {0x43,  1,  0x3B24,  11,   8}, //U_OTP_ena_ForceOneMilSec
    {0x44,  1,  0x3B24,  15,  12}, //U_OTP_allow_SCCheck
    {0x45,  1,  0x3B24,  19,  16}, //U_OTP_ena_TestRCFreq
    {0x46,  1,  0x3B24,  23,  20}, //U_OTP_ena_SWRN
    {0x47,  1,  0x3B24,  27,  24}, //U_OTP_ena_NSKSeedPRNG
    {0x48,  1,  0x3B24,  31,  28}, //U_OTP_OTPWritePWDProtect
    {0x49,  1,  0x3B28,  11,   8}, //U_OTP_ena_EMMFilter
    {0x4A,  1,  0x3B28,  15,  12}, //U_OTP_ena_TestGenIN
    {0x4B,  1,  0x3B28,   7,   6}, //U_OTP_allow_NSK2_PWD_Mode
    {0x4C,  1,  0x3B2C,   3,   2}, //U_OTP_allow_NDSSC_ReadFail_BadPkt
    {0x4D,  1,  0x3B2C,   5,   4}, //U_OTP_allow_RANDOM_keybus
    {0x4E,  1,  0x3B2C,   7,   6}, //U_OTP_allow_RANDOM_byteacc
    {0x4F,  1,  0x3B2C,   9,   8}, //U_OTP_allow_NDS_Rd55AA
    {0x50,  1,  0x3B2C,  11,  10}, //U_OTP_allow_NDS_Parity_chk
    {0x51,  1,  0x3B2C,  15,  14}, //U_OTP_forbid_OTPBuiltInTest
    {0x52,  1,  0x3C58,   3,   0}, //U_OTP_ena_ESAAlgo_invalidate
    {0x53,  1,  0x3C58,   7,   4}, //U_OTP_ena_LocalAlgo_Invalidate
    {0x54,  1,  0x3C58,  11,   8}, //U_OTP_ContentProtEn
    {0x55,  1,  0x3C58,  13,  12}, //U_OTP_concurrency_configuration
    {0x56,  1,  0x3C58,  15,  14}, //U_OTP_allow_NSK_RNG_ROSC
    {0x57,  1,  0x3C58,  16,  16}, //U_OTP_nds_fc_disable
    {0x58,  1,  0x3C58,  17,  17}, //U_OTP_NDS_CPNR0_sel
    {0x59,  1,  0x3C58,  18,  18}, //U_OTP_NDS_CPNR_off
    {0x5A,  1,  0x3C60,   0,   0}, //U_OTP_forbid_CLK_SEED_TEST
    {0x5B,  1,  0x3C60,   1,   1}, //U_OTP_MOBF_TOP_use_DES
    {0x5C,  1,  0x3C78,   3,   0}, //U_OTP_ena_PVR_secure_protect_0
    {0x5D,  1,  0x3C74,   3,   0}, //U_OTP_ena_PVRNS2S
    {0x5E,  1,  0x3C80,  17,  16}, //U_OTP_forbid_SW_SPSD_Key
    {0x5F,  1,  0x3C80,  19,  18}, //U_OTP_forbid_KL_SPSD_Key
    {0x60,  1,  0x3CC0,   1,   0}, //U_OTP_forbid_ACPUWriteOTP
    {0x61,  1,  0x3CC0,   3,   2}, //U_OTP_forbid_ACPUReadOTP
    {0x62,  1,  0x3CC0,   5,   4}, //U_OTP_forbid_SCPUWriteOTP
    {0x63,  1,  0x3CC0,   7,   6}, //U_OTP_forbid_SCPUReadOTP
    {0x64,  1,  0x3CC0,   9,   8}, //U_OTP_forbid_DBBUSWriteOTP
    {0x65,  1,  0x3CC0,  11,  10}, //U_OTP_forbid_DBBUSReadOTP
    {0x66,  1,  0x3CC0,  13,  12}, //U_OTP_allow_RANDOM
    {0x67,  1,  0x3CC0,  15,  14}, //U_OTP_allow_NOISE_Rd
    {0x68,  1,  0x3CC8,   3,   0}, //U_OTP_SBoot
    {0x69,  1,  0x3CC8,   7,   4}, //U_OTP_SecretAreaEnable
    {0x6A,  2,  0x3CCC,   8,   0}, //U_OTP_SCAN_MODE
    {0x6B,  1,  0x3CCC,  15,  10}, //U_OTP_MBIST_MODE
    {0x6C,  1,  0x3CCC,  21,  16}, //U_OTP_I2C_MODE
    {0x6D,  1,  0x3CCC,  31,  26}, //U_OTP_EJTAG_MODE
    {0x6E,  1,  0x3C68,   3,   0}, //U_OTP_forbid_USBSlaveMode
    {0x6F,  1,  0x3CD0,   9,   8}, //U_OTP_allow_DRAM_MOBF
    {0x70,  1,  0x3CD4,  13,   8}, //U_OTP_BootMode
    {0x71,  1,  0x3CD4,  21,  16}, //U_OTP_SEPBootMode
    {0x72,  2,  0x3CD8,  23,  12}, //U_OTP_PostMskAreaRange1
    {0x73,  2,  0x3CE0,  23,  16}, //U_OTP_BootDevice
    {0x74,  1,  0x3D24,  11,   8}, //U_OTP_ena_NSK2
    {0x75,  1,  0x3D24,  15,  14}, //U_OTP_ena_DMA
    {0x76,  1,  0x3D24,  19,  16}, //U_OTP_ena_SEP
    {0x77,  1,  0x3D40,  21,  18}, //U_OTP_allow_SPSSPDKeyShuffle
    {0x78,  1,  0x3D44,   3,   0}, //U_OTP_ena_ACPU2KT
    {0x79,  1,  0x3D44,   7,   4}, //U_OTP_ena_NSK2KT
    {0x7A,  1,  0x3D44,  27,  24}, //U_OTP_ena_NSKCW2CryptoDMA
    {0x7B,  1,  0x3D44,  31,  28}, //U_OTP_ena_ACPU2LSA
    {0x7C,  1,  0x3CD0,  31,  26}, //U_OTP_I2C2TEE_MODE
    {0x7D,  1,  0x3CC8,  19,  16}, //U_OTP_SecR2_Sboot
    {0x7E,  1,  0x3C88,  27,  24}, //U_OTP_NDS_CODE_INTEGRITY_RSA_KEY_SEL
    {0x7F,  1,  0x3CC8,  27,  24}, //U_OTP_xCPU_Sboot
};


void endian_change(MS_U8* ptr, MS_U32 bytes)
{

	MS_U32 u32halve;
	MS_U32 u32tmp;
	MS_U32 i;

	u32halve = bytes/2;
	for(i=0; i<u32halve; i++)
	{
		u32tmp = ptr[i];
		ptr[i] = ptr[bytes-i-1];
		ptr[bytes-i-1] = u32tmp;
	}
}

MS_U32 HAL_NSK2_GetOTPProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    MS_U32 NumOfTag = sizeof(K6liteAsst)/sizeof(ASST_t);
    MS_U32 ret_size = 0, i, j;
    MS_U8 *pDesc = (MS_U8 *)desc;
    MS_U32 OTPValue;
    MS_U8  RunLens;
    MS_U32 RunOffset;

    ret_size = 0;
    for( i=0; i<NumOfTag; i++)
    {
        ret_size += (K6liteAsst[i].u8Length + 2);
    }

    *desc_size = ret_size;

    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"length of OTP Tag = %x\n",ret_size);
    if(desc == NULL)
    {
        return TRUE;
    }

    for( i=0; i<NumOfTag; i++)
    {
        *pDesc++ = K6liteAsst[i].u8Tag;
        *pDesc++ = K6liteAsst[i].u8Length;

        if(0x20 == K6liteAsst[i].u8Tag) //pubOTP...special case....
        {
            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"begin of Tag 0x20\n");
            *pDesc++ = 0;
            //4 Chip ID
            MS_U8 PubOTP[8];
            for(j=0;j<8;j+=4)
            {
                MS_U32 u32Tmp;
                u32Tmp = MDrv_CA_OTP_Read(K6liteAsst[i].Offset + j);
                memcpy(&PubOTP[j], &u32Tmp, 4);
            }


            endian_change(PubOTP, 8);

            memcpy(pDesc,PubOTP,8);
            pDesc += 8;

            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"end of Tag 0x20\n");
        }
        else if (0x25 == K6liteAsst[i].u8Tag) //NV Counter...special case....
        {
            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"begin of Tag 0x25\n");
            MS_U32 MaxNVCounter = 1024;
            *pDesc++ = (MS_U8)((MaxNVCounter>>24)&0xff);
            *pDesc++ = (MS_U8)((MaxNVCounter>>16)&0xff);
            *pDesc++ = (MS_U8)((MaxNVCounter>>8)&0xff);
            *pDesc++ = (MS_U8)(MaxNVCounter&0xff);

            *pDesc++ = 0;
            *pDesc++ = 0;
            *pDesc++ = 0;
            *pDesc++ = 0;

            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"end of Tag 0x25\n");
        }
        else
        {
            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"begin of Tag 0x%x\n",K6liteAsst[i].u8Tag);
            RunLens = K6liteAsst[i].u8Length;
            RunOffset = K6liteAsst[i].Offset;
            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"Offset = %x, length = %x\n",RunOffset, RunLens);
            if(K6liteAsst[i].u8Length <= 4)
            {
                HAL_NSK2_OTP_Get(K6liteAsst[i].Offset, K6liteAsst[i].MSB, K6liteAsst[i].LSB, &OTPValue);
                if(1 == RunLens)
                    *pDesc++ = (MS_U8)(OTPValue&0xff);
                else if(2 == RunLens)
                {
                    *pDesc++ = (MS_U8)((OTPValue>>8)&0xff);
                    *pDesc++ = (MS_U8)(OTPValue&0xff);
                }
                else if(4 == RunLens)
                {
                    *pDesc++ = (MS_U8)((OTPValue>>24)&0xff);
                    *pDesc++ = (MS_U8)((OTPValue>>16)&0xff);
                    *pDesc++ = (MS_U8)((OTPValue>>8)&0xff);
                    *pDesc++ = (MS_U8)(OTPValue&0xff);
                }
            }
            else //length > 4, tmp solution
            {
                HALNSK2_DBG(NSK2_DBGLV_DEBUG,"length = %x\n",RunLens);
                MS_U8 BulkData[16];
                for( j=0; j<RunLens; j+=4 )
                {
                    HAL_NSK2_OTP_Get(RunOffset + j, 31, 0, &OTPValue);
                    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"run offset = %x, Value = %x\n", (RunOffset + j),OTPValue);
                    memcpy(&BulkData[j],&OTPValue,4);
                }
                endian_change(BulkData, 16);
                memcpy(pDesc,BulkData,16);
                pDesc += 16;
            }

            HALNSK2_DBG(NSK2_DBGLV_DEBUG,"end of Tag 0x%x\n",K6liteAsst[i].u8Tag);
        }
    }

#if 0
    MS_U32 size = *desc_size;

    for(i=0;i<size;i++)
    {
        printf(" (%d, %x) \n", i , desc[i]);
    }
#endif
    return TRUE;
}



typedef struct
{
    MS_U32  Offset;
    MS_U32  MSB;
    MS_U32  LSB;
    MS_U32  Bits;
} ChipOTPCfg_t;


static ChipOTPCfg_t K6ChipCfg[] =
{
    {0x3B20,  7,   0,   8},  //[127:120]	U_OTP_forbid_NSK_wr_sck[7:0]	11111111
    {0x3B20, 15,   8,   8},  //[119:112]	U_OTP_UseCheckSum[7:0]	11111111
    {0x3B20, 17,  16,   2},  //[111:110]	U_OTP_allow_NDSKey_BlankChk[1:0]	11
    {0x3B20, 19,  18,   2},  //[109:108]	U_OTP_allow_ProgFail_RuinNDSKey[1:0]	11
    {0x3B20, 21,  20,   2},  //[107:106]	U_OTP_allow_NDSReadKeyWait200ms[1:0]	11
    {0x3B20, 23,  22,   2},  //[105:104]	U_OTP_allow_ReadErrorRstOtp[1:0]	11
    {0x3B20, 27,  24,   4},  //[103:100]	U_OTP_allow_illegalNDSFlagChk[3:0]	1111
    {0x3B20, 31,  28,   4},  //[99:96]	U_OTP_allow_Rst_NDS_SCFlag_ParityFail[3:0]	1111
    {0x3B24,  3,   0,   4},  //[95:92]	U_OTP_ena_ACPUUseNSK2[3:0]	0001
    {0x3B28,  7,   6,   2},  //[91:90]	U_OTP_allow_NSK2_PWD_Mode[1:0]	11
    {0x3B2C, 15,  14,   2},  //[[89:88]	U_OTP_forbid_OTPBuiltInTest[1:0]	11
    {0x3B2C, 17,  16,   2},  //[87:86]	U_OTP_forbid_Kilo_ProgRepair[1:0]	11
    {0x3C50,  1,   0,   2},  //[85:84]	U_OTP_NDS_ESCK_Key1_obfuscation[1:0]	11
    {0x3C50,  3,   2,   2},  //[83:82]	U_OTP_NDS_Key1_integrity_chk[1:0]	11
    {0x3C50,  5,   4,   2},  //[81:80]	U_OTP_NDS_ESCK_Key2_obfuscation[1:0]	11
    {0x3C50,  7,   6,   2},  //[79:78]	U_OTP_NDS_Key2_integrity_chk[1:0]	11
    {0x3C50,  9,   8,   2},  //[77:76]	U_OTP_NDS_ESCK_Key3_obfuscation[1:0]	11
    {0x3C50, 11,  10,   2},  //[75:74]	U_OTP_NDS_Key3_integrity_chk[1:0]	11
    {0x3C50, 13,  12,   2},  //[73:72]	U_OTP_NDS_ESCK_Key4_obfuscation[1:0]	11
    {0x3C50, 15,  14,   2},  //[71:70]	U_OTP_NDS_Key4_integrity_chk[1:0]	11
    {0x3C50, 17,  16,   2},  //[69:68]	U_OTP_NDS_ESCK_Key5_obfuscation[1:0]	11
    {0x3C50, 19,  18,   2},  //[67:66]	U_OTP_NDS_Key5_integrity_chk[1:0]	11
    {0x3C50, 21,  20,   2},  //[65:64]	U_OTP_NDS_ESCK_Key6_obfuscation[1:0]	11
    {0x3C50, 23,  22,   2},  //[63:62]	U_OTP_NDS_Key6_integrity_chk[1:0]	11
    {0x3C50, 25,  24,   2},  //[61:60]	U_OTP_NDS_ESCK_Key7_obfuscation[1:0]	11
    {0x3C50, 27,  26,   2},  //[59:58]	U_OTP_NDS_Key7_integrity_chk[1:0]	11
    {0x3C50, 29,  28,   2},  //[57:56]	U_OTP_NDS_ESCK_Key8_obfuscation[1:0]	11
    {0x3C50, 31,  30,   2},  //[55:54]	U_OTP_NDS_Key8_integrity_chk[1:0]	11
    {0x3C58, 11,   8,   4},  //[53:50]	U_OTP_ContentProtEn[3:0]	1111
    {0x3C58, 13,  12,   2},  //[49:48]	U_OTP_concurrency_configuration[1:0]	10
    {0x3C58, 15,  14,   2},  //[47:46]	U_OTP_allow_NSK_RNG_ROSC[1:0]	11
    {0x3C60,  0,   0,   1},  //[45]	U_OTP_forbid_CLK_SEED_TEST	1
    {0x3C60,  3,   2,   2},  //[44:43]	U_OTP_OBFUSCATEVideoStream[1:0]	00
    {0x3C78,  3,   0,   4},  //[42:39]	U_OTP_ena_PVR_secure_protect_0[3:0]	0001
    {0x3C78, 11,   8,   4},  //[38:35]	U_OTP_dis_TSO[3:0]	0001
    {0x3CC0, 13,  12,   2},  //[34:33]	U_OTP_allow_RANDOM[1:0]	11
    {0x3CC8,  3,   0,   4},  //[32:29]	U_OTP_SBoot[3:0]	0000
    {0x3C68,  3,   0,   4},  //[28:25]	U_OTP_forbid_USBSlaveMode[3:0]	1111
    {0x3CD0,  9,   8,   2},  //[24:23]	U_OTP_allow_DRAM_MOBF[1:0]	00
    {0x3CD0, 18,  16,   3},  //[22:20]	U_OTP_forbid_clk_otp_sel[2:0]	111
    {0x3CE0, 31,  30,   2},  //[19:18]	U_OTP_forbid_STR[1:0]	00
    {0x3D24, 11,   8,   4},  //[17:14]	U_OTP_ena_NSK2[3:0]	0001
    {0x3D24, 15,  14,   2},  //[13:12]	U_OTP_ena_DMA[1:0]	01
    {0x3D24, 19,  16,   4},  //[11:8]	U_OTP_ena_SCPU[3:0]	0001
    {0x3D44,  3,   0,   4},  //[7:4]	U_OTP_ena_ACPU2KT[3:0]	0001
    {0x3D44,  7,   4,   4},  //[3:0]	U_OTP_ena_NSK2KT[3:0]	0001

    {0x3D44, 19,  16,   4},  //[127:124]	U_OTP_ena_ACPUWrNSKKey2KT[3:0]	0111
    {0x3D50,  3,   0,   4},  //[123:120]	U_OTP_ena_LSACPCM[3:0]	0001
    {0x3D50,  7,   4,   4},  //[119:116]	U_OTP_ena_AESBasedCipher[3:0]	0001
    {0x3D50, 11,   8,   4},  //[115:112]	U_OTP_ena_DESBasedCipher[3:0]	0001
    {0x3D50, 15,  12,   4},  //[111:108]	U_OTP_ena_3DESBasedCipher[3:0]	0001
    {0x3D50, 19,  16,   4},  //[107:104]	U_OTP_ena_Multi2BasedCipher[3:0]	0001
    {0x3D50, 23,  20,   4},  //[103:100]	U_OTP_ena_DVBCSA2ConfCipher[3:0]	0001
    {0x3D50, 27,  24,   4},  //[99:96]	U_OTP_ena_DVBCSA2Cipher[3:0]	0001
    {0x3D50, 31,  28,   4},  //[95:92]	U_OTP_ena_DVBCSA3BasedCipher[3:0]	0001
    {0x3D54, 31,   0,  32},  //[91:28]	U_OTP_Ch_SwitchComb[63:0]	1111111111111111111111111111111111111111111111111111111111111111
    {0x3D58, 31,   0,  32},  //[91:28]	U_OTP_Ch_SwitchComb[63:0]	1111111111111111111111111111111111111111111111111111111111111111
    {0x3D5C,  3,   0,   4},  //[27:24]	U_OTP_ena_LSAD_MDI[3:0]	0001
    {0x3D5C,  7,   4,   4},  //[23:20]	U_OTP_ena_LSAD_MDD[3:0]	0001
    {0x3D5C, 11,   8,   4},  //[19:16]	U_OTP_ena_LSAD_CIPLUS_AES[3:0]	0001
    {0x3D5C, 15,  12,   4},  //[15:12]	U_OTP_ena_LSAD_AES_ECB_CLEAR[3:0]	0001
    {0x3D5C, 19,  16,   4},  //[11:8]	U_OTP_ena_LSAD_SCTE41_SCTE52_DES[3:0]	0001
    {0x3D60,  3,   0,   4},  //[7:4]	U_OTP_ena_ESA_CIPLUS_AES[3:0]	0001
    {0x3D60,  7,   4,   4},  //[3:0]	U_OTP_ena_ESA_CIPLUS_DES[3:0]	0001

    {0x3D60, 11,   8,   4},  //[127:124]	U_OTP_ena_ESA_SCTE52_DES[3:0]	0001
    {0x3D60, 15,  12,   4},  //[123:120]	U_OTP_ena_ESA_tDES_CBC_CLEAR[3:0]	0001
    {0x3D64,  3,   0,   4},  //[119:116]	U_OTP_ena_LSAS_MDI[3:0]	0001
    {0x3D64,  7,   4,   4},  //[115:112]	U_OTP_ena_LSAS_MDD[3:0]	0001
    {0x3D64, 11,   8,   4},  //[111:108]	U_OTP_ena_LSAS_CIPLUS_AES[3:0]	0001
    {0x3D64, 15,  12,   4},  //[107:104]	U_OTP_ena_LSAS_AES_ECB_CLEAR[3:0]	0001
    {0x3D64, 19,  16,   4},  //[103:100]	U_OTP_ena_LSAS_SCTE41_SCTE52_DES[3:0]	0001
    {0x3D68,  7,   4,   4},  //[99:96]	U_OTP_ena_ReviewFailPkt[3:0]	0001
    {0x3D68, 11,   8,   4},  //[95:92]	U_OTP_dis_NonSecRangeEncrypt[3:0]	0001
    {0x3D6C,  3,   0,   4},  //[91:88]	U_OTP_ena_CA_PVR_secure_protect_0[3:0]	0001
    {0x3D6C, 19,  16,   4},  //[87:84]	U_OTP_ena_LowerPathRec[3:0]	0111
    {0x3FA0,  9,   8,   2},  //[83:82]	U_OTP_I2C_PWD_obfuscation[1:0]	11
    {0x3FA0, 11,  10,   2},  //[81:80]	U_OTP_EJTAG_PWD_obfuscation[1:0]	11
    {0x3FA0, 13,  12,   2},  //[79:78]	U_OTP_SCAN_PWD_obfuscation[1:0]	11
    {0x3FA0, 15,  14,   2},  //[77:76]	U_OTP_MBIST_PWD_obfuscation[1:0]	11
    //[63:0]	0	0000000000000000000000000000000000000000000000000000000000000000

};

#define FullChipConfigSize     0x27

MS_U32 HAL_NSK2_GetFullChipSize(void)
{
    return FullChipConfigSize;
}

MS_U32 HAL_NSK2_GetFullChipConfig(MS_U32 *desc_size, MS_U8 *desc)
{
    *desc_size = FullChipConfigSize;
    if(desc == NULL)
    {
        return TRUE;
    }

    MS_U32 i, j, remain_bits, shift_bits;
    MS_U32 NumOfConfigs = sizeof(K6ChipCfg)/sizeof(ChipOTPCfg_t);
    MS_U32 TotalOTPValue[100];

    memset(desc, 0x0, FullChipConfigSize);
    memset(TotalOTPValue, 0x0, 100*sizeof(MS_U32));
    //desc[0x40-1] = 0x28;
    //desc[0x28] = 0x80;

    for(i=0; i<NumOfConfigs; i++)
    {
        HAL_NSK2_OTP_Get(K6ChipCfg[i].Offset, K6ChipCfg[i].MSB, K6ChipCfg[i].LSB, &TotalOTPValue[i]);
        printf("%d, %x\n",i, TotalOTPValue[i]);
    }

    j = 0;
    remain_bits = 8;
    for(i=0; i<NumOfConfigs; i++)
    {
        if(K6ChipCfg[i].Bits>remain_bits)
        {
            if(K6ChipCfg[i].Bits == 32)
            {
                if(remain_bits != 8)
                {
                    shift_bits = 8-remain_bits;
                    desc[j] |= 0xf;
                    j++;
                    desc[j] = 0xff;
                    j++;
                    desc[j] = 0xff;
                    j++;
                    desc[j] = 0xff;
                    j++;
                    desc[j] = 0xf0;
                    remain_bits = 4;
                }
                else
                {
                    memcpy(&desc[j], &TotalOTPValue[i], 4);
                    j += 4;
                    remain_bits = 0;
                }
            }
            else
            {

                shift_bits = K6ChipCfg[i].Bits-remain_bits;
                //printf("not equal case (%d), shift_bits = %x\n",i,shift_bits);
                desc[j] |= (TotalOTPValue[i] >> shift_bits);

                j++;

                remain_bits = (8-shift_bits);
                desc[j] |= (( TotalOTPValue[i] << remain_bits ) & 0xff);            }

        }
        else
        {
            remain_bits -= K6ChipCfg[i].Bits;

            desc[j] |= (TotalOTPValue[i] << (remain_bits));

        }

        if(remain_bits == 0)
        {
            remain_bits = 8;
            j++;
        }
    }

    return TRUE;
}


static NVCounter_Desc_t nvcounter_desc = {
    .descriptor_tag = NSK2HDI_OTP_NVCOUNTER_DESC_TAG,
    .descriptor_length = 8, //sizeof(NVCounter_Desc_t) - 2,
    .max_nvcounter[0] = 0x00,
    .max_nvcounter[1] = 0x00,
    .max_nvcounter[2] = 0x04,
    .max_nvcounter[3] = 0x00,
    .left_nvcounter[0] = 0x00,
    .left_nvcounter[1] = 0x00,
    .left_nvcounter[2] = 0x04,
    .left_nvcounter[3] = 0x00,
};

#define TotalNVNumber       1024



MS_U32 HAL_NSK2_GetNVCounterConfig(MS_U32 *desc_size, MS_U8 *desc)
{
    MS_U32 u32NvCounter = 0;
    MS_U32 u32LeftNvCounter = 0;

    if(desc == NULL)
    {
        *desc_size = sizeof(nvcounter_desc);
        HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_NVCounterUpdates return desc_size = %x\n",*desc_size);
        return TRUE;
    }

    *desc_size = sizeof(nvcounter_desc);
// Read NV Counter from register directly
#if 1
    u32NvCounter = NI_REG (REG_NI_NSK21_GET_NVCOUNTER);
#else
    HAL_NSK2_WriteControl(0x1);
    MsOS_DelayTaskUs(10);
    HAL_NSK2_WriteCommand(0x73);
    MsOS_DelayTaskUs(10);
    StatusCheck(HAL_NSK2_CheckBusy());

    MS_U32 NVValue, LeftNVCounter;
    HAL_NSK2_ReadData32(0x1C, 0x1, &NVValue);
#endif

    u32LeftNvCounter = TotalNVNumber - u32NvCounter;

    printf("NVValue = %d, LeftNVCounter = %d \n", u32NvCounter, u32LeftNvCounter);

    nvcounter_desc.left_nvcounter[0] = (MS_U8)((u32LeftNvCounter>>24)&0xff);
    nvcounter_desc.left_nvcounter[1] = (MS_U8)((u32LeftNvCounter>>16)&0xff);
    nvcounter_desc.left_nvcounter[2] = (MS_U8)((u32LeftNvCounter>>8)&0xff);
    nvcounter_desc.left_nvcounter[3] = (MS_U8)(u32LeftNvCounter&0xff);

    memcpy(desc,&nvcounter_desc,sizeof(nvcounter_desc));

    return TRUE;
}

void HAL_NSK2_SetDbgLevel(MS_U32 u32Level)
{
    _g32NSK2HalDbgLv = u32Level;
    HALNSK2_DBG(NSK2_DBGLV_INFO, "%s level: %x\n", __FUNCTION__, u32Level);
    return;
}

void HAL_NSK2_SetPollingCnt(MS_U32 u32Cnt)
{
    dead_polling_cnt = u32Cnt;
}

#if 0
void HAL_NSK2_BurstLen(MS_U32 u32PVREng, MS_U32 u32BurstMode)
{
    HAL_PVR_BurstLen(u32PVREng, u32BurstMode);
}
#endif

void HAL_NSK2_ClockTest(MS_U32 testnum)
{

    MS_U32 u32Data;
    switch(testnum)
    {
    case 0:
        RSA_REG(REG_RSA_CLK_ENABLE) |= RSA_PM_NSKCLK_ENABLE;
        MsOS_DelayTaskUs(1);

        u32Data = RSA_REG(REG_RSA_CLK_ENABLE);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "RSA REG_RSA_CLK_ENABLE = %x\n",u32Data);


        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);

        //bit 0 set to 1...
        NI_REG(REG_NI_NSK2_CTRL) = u32Data | NI_NSK2_RESET_DISABLE | NI_NSK2_CLK_ENABLE;

        MsOS_DelayTaskUs(1);
        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);
        break;


    case 1:
        RSA_REG(REG_RSA_CLK_ENABLE) |= RSA_PM_NSKCLK_ENABLE;
        MsOS_DelayTaskUs(1);

        u32Data = RSA_REG(REG_RSA_CLK_ENABLE);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "RSA REG_RSA_CLK_ENABLE = %x\n",u32Data);


        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);

        //disable nsk2 clock
        NI_REG(REG_NI_NSK2_CTRL) = u32Data & ~(NI_NSK2_CLK_ENABLE) ;
        MsOS_DelayTask(1);
        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);

        break;

    case 2:
        RSA_REG(REG_RSA_CLK_ENABLE) &= (~RSA_PM_NSKCLK_ENABLE);
        MsOS_DelayTaskUs(1);

        u32Data = RSA_REG(REG_RSA_CLK_ENABLE);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "RSA REG_RSA_CLK_ENABLE = %x\n",u32Data);


        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);

        //bit 0 set to 1...
        NI_REG(REG_NI_NSK2_CTRL) = u32Data | NI_NSK2_RESET_DISABLE | NI_NSK2_CLK_ENABLE;

        MsOS_DelayTaskUs(1);
        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);
        break;

    case 3:
        RSA_REG(REG_RSA_CLK_ENABLE) &= (~RSA_PM_NSKCLK_ENABLE);
        MsOS_DelayTaskUs(1);

        u32Data = RSA_REG(REG_RSA_CLK_ENABLE);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "RSA REG_RSA_CLK_ENABLE = %x\n",u32Data);

        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);

        //disable nsk2 clock
        NI_REG(REG_NI_NSK2_CTRL) = u32Data & ~(NI_NSK2_CLK_ENABLE) ;
        MsOS_DelayTask(1);
        u32Data = NI_REG(REG_NI_NSK2_CTRL);
        HALNSK2_DBG(NSK2_DBGLV_DEBUG, "NI REG_NI_NSK2_CTRL = %x\n",u32Data);
        break;
    }
}

void HAL_NSK2_GetRNGThroughPut(void *pRngData, MS_U32 u32DataSize, MS_BOOL bDump)
{
	MS_U32 u32StartTime;
    MS_U32 *pRNG_Data = (MS_U32 *)pRngData;
    MS_U32 valid = 0;
	MS_U32 i = 0;

    NI_REG(REG_NI_NSK2_CTRL) = NI_REG(REG_NI_NSK2_CTRL) & (~NI_NSK2_RESET_DISABLE);
    NI_REG(REG_NI_NSK2_FREERUN) = NI_REG(REG_NI_NSK2_FREERUN) & (~ (NI_NSK2_RANDOM_FREERUN | NI_NSK2_RANDOM_ONEBYONE) );

    u32StartTime = MsOS_GetSystemTime();

	for(i=0; i< u32DataSize; i++)
	{
		do
		{
			valid = NI_REG(REG_NI_NSK2_TRNG_VALID) & NI_NSK2_TRNG_VALID_MASK; //trng_sw_read_valid_nsk ;
		}while(valid != 1);

		pRNG_Data[i] = NI_REG(REG_NI_NSK2_TRNG_DATA);//trng_sw_read_data_nsk;
		NI_REG(REG_NI_NSK2_FREERUN) = NI_REG(REG_NI_NSK2_FREERUN) | NI_NSK2_RANDOM_ONEBYONE;  //lfsr_get_go_nsk = 1 ;
	}
	MS_U32 u32EndTime = MsOS_GetSystemTime();

	HALNSK2_DBG(NSK2_DBGLV_ERR,  "1M bit data size, total Time = %1d ms\n", (MS_U32)(u32EndTime - u32StartTime));

	if(bDump)
	{
		for(i = 0 ; i < u32DataSize ; i ++)
		{
			HALNSK2_DBG(NSK2_DBGLV_ERR, "%08x\n", (unsigned int)(pRNG_Data[i]));
		}
	}
}

void HAL_NSK2_RunFree(MS_BOOL bRunFree)
{
    HALNSK2_DBG(NSK2_DBGLV_DEBUG, "bRunFree = %d\n", bRunFree);

	if(bRunFree)
	{
		_gCheckBusyFlag = FALSE;
	}
	else
	{
		_gCheckBusyFlag = TRUE;
	}
}


MS_U32 HAL_NSK2_PushSlowClock ( MS_BOOL HaltClk, MS_U32 NumOfMs)
{
    //HALNSK2_DBG(NSK2_DBGLV_DEBUG, "HaltClk = %d NumofTenSecond = %d\n", HaltClk, NumofTenSecond);
    _gCheckBusyFlag = TRUE;
    StatusCheck(HAL_NSK2_CheckBusy());
	MS_U32 u32Data = NI_REG(REG_NI_NSK2_CTRL);

	if(HaltClk == TRUE)//close clk
	{
		NI_REG(REG_NI_NSK2_CTRL) = u32Data & (~NI_NSK2_CLK_ENABLE) ;
        _gCheckBusyFlag = FALSE;
	}
	else //open clk
	{
	    _gCheckBusyFlag = TRUE;
        //_gCheckBusyFlag = FALSE;
	    if( (u32Data & NI_NSK2_CLK_ENABLE) == 0)
        {
    		NI_REG(REG_NI_NSK2_CTRL) = u32Data | NI_NSK2_CLK_ENABLE ;
            //MsOS_DelayTaskUs(1);
        }
	}

    //printf("REG_NI_NSK2_CTRL = %x\n",NI_REG(REG_NI_NSK2_CTRL));

    //u32Data = 0;
    u32Data = NI_REG(REG_NI_NSK2_CLK_CSA);


    NI_REG(REG_NI_NSK2_CLK_CSA) = u32Data | NSK2_PUSH_SLOW_CLK;


    if(NumOfMs <= 2)
    {
        MsOS_DelayTaskUs(NumOfMs*500);
    }
    else
    {
        MsOS_DelayTask(NumOfMs-1);
    }

    return TRUE;
}



MS_U32 HAL_NSK2_GetCMProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    MS_U32 i;
    if(desc == NULL)
    {
        *desc_size = sizeof(cmchannel_group_capability_descriptor_t) + sizeof(cm_algo) /*sizeof(cmchannel_group_algorithm_record_descriptor_t)*CMChannelDescSize*/ ;
        HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_GetCMProperties return desc_size = %x\n",*desc_size);
        return TRUE;
    }

    for(i=0;i<32;i++)
    {
        cm_capb.switch_combination_bitmap[i] = 0xFF;
    }

    *desc_size = sizeof(cmchannel_group_capability_descriptor_t) + sizeof(cm_algo);

    memcpy(desc,&cm_capb,sizeof(cmchannel_group_capability_descriptor_t));
    memcpy(desc+sizeof(cmchannel_group_capability_descriptor_t), &cm_algo[0] , sizeof(cm_algo) /*sizeof(cmchannel_group_algorithm_record_descriptor_t)*CMChannelDescSize*/ );

    HALNSK2_DBG(NSK2_DBGLV_INFO,"\nHAL_NSK2_GetCMProperties return *desc_size = %x,desc = %x\n",*desc_size,(MS_U32)desc);

    return TRUE;
}

MS_U32 HAL_NSK2_GetM2MProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    if(desc == NULL)
    {
        *desc_size = sizeof(m2m_capa_desc) + sizeof(m2m_algo);
        HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_GetM2MProperties #1 return desc_size = %x\n",*desc_size);
        return TRUE;
    }

    *desc_size = sizeof(m2m_capa_desc) + sizeof(m2m_algo);

    memcpy(desc, &m2m_capa_desc, sizeof(m2m_capa_desc));
    memcpy(desc+sizeof(m2m_capa_desc), &m2m_algo[0], sizeof(m2m_algo));

    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_GetM2MProperties #2 return desc_size = %x\n",*desc_size);
    return TRUE;
}

MS_U32 HAL_NSK2_GetDMAProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    if(desc == NULL)
    {
        *desc_size = sizeof(dma_capa_desc);
        HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_GetDMAProperties return desc_size = %x\n",*desc_size);
        return TRUE;
    }

    *desc_size = sizeof(dma_capa_desc);

    memcpy(desc, &dma_capa_desc, *desc_size);

    return TRUE;
}

//---------------Debug Information----------------------------//
//------------------------------------------------------------//

void HAL_NSK2_ReadTSPInfo(MS_U32 pid_no)
{
    MS_U32 pid_data;
    MS_U32 FltPid, TS_SRC, dscmb_key_en, prim, pid_pair;

    FltPid = TS_SRC = dscmb_key_en = prim = pid_pair = 0;
    REG_PidFlt *pPidFlt = PPIDFLT0(pid_no);

    pid_data = TSP32_IdrR(pPidFlt);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"pid_data = %x\n",pid_data);

    FltPid = (pid_data & 0x1fff);
    TS_SRC = ((pid_data>>13) & 0x7);
    dscmb_key_en = ((pid_data>>16) & 0x1);
    prim = ((pid_data>>17) & 0x1);
    pid_pair = ((pid_data>>18) & 0xf);

    if(dscmb_key_en)
    {
        HALNSK2_DBG(NSK2_DBGLV_INFO,"(%d) = (FltPid=%x, TS_SRC=%d, prim=%d, pid_pair=%d\n",pid_no,FltPid,TS_SRC,prim,pid_pair);
    }
}

void HAL_NSK2_ReadTSPDstInfo(MS_U32 pid_no)
{
    MS_U32 pid_data;
    REG_PidFlt *pPidFlt = PPIDFLT1(pid_no);

    pid_data = TSP32_IdrR(pPidFlt);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"dst_data = %x\n",pid_data);
}

void HAL_NSK2_AllTSPPidFilter(void)
{
    MS_U32 i;
    for(i=0;i<0x100;i++)
    {
        HAL_NSK2_ReadTSPInfo(i);
    }

    for(i=0;i<0x100;i++)
    {
        HAL_NSK2_ReadTSPDstInfo(i);
    }
}

void HAL_NSK2_ChangePidFilter(MS_U32 pid_no, MS_U32 Data)
{
    REG_PidFlt *pPidFlt = PPIDFLT0(pid_no);
    TSP32_IdrW(pPidFlt,Data);
}



MS_U32 HAL_NSK2_ReadSwitchFromNSK2(void)
{

#if 0
2.	N2_KteValid=1. NI bank offset 13 bit[3].
3.	N2_KteDest=0. NI bank offset 13 bit[2:0].
4.	Read the NSK bank offset 0xfc40-0xfc48 to read the NSK driven switch value. (see NSK-ICD-253 page21).
In the read 96 bits, you can find:
Bit[55] AU
Bit[54] BU
Bit[53] DU
Bit[52] EU
Bit[51] AL
Bit[50] BL
Bit[49] DL
Bit[48] EL
Bit[47:44] LocalSelectD
Bit[43:40] LocalSelectS
Bit[39:36] ESA select
Bit[35:33] ESA subselect
Please print out these fields and send the log to me.
#endif

#ifdef ReadSwitchInfoNSK2
    MS_U32 NI13 = NI_REG(13);
    HALNSK2_DBG(0,"NI13 = %x\n",NI13);

    MS_U32 NSK2Debug[3];
    NSK2Debug[0] = HAL_NSK2_ReadReg(0xFC40);
    NSK2Debug[1] = HAL_NSK2_ReadReg(0xFC44);
    NSK2Debug[2] = HAL_NSK2_ReadReg(0xFC48);

    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"NSK2Debug = (%x,%x,%x)\n",NSK2Debug[0],NSK2Debug[1],NSK2Debug[2]);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"AU=%x,BU=%x,DU=%x,EU=%x \n",(NSK2Debug[1]>>23)&0x1,(NSK2Debug[1]>>22)&0x1,(NSK2Debug[1]>>21)&0x1,(NSK2Debug[1]>>20)&0x1);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"AL=%x,BL=%x,DL=%x,EL=%x \n",(NSK2Debug[1]>>19)&0x1,(NSK2Debug[1]>>18)&0x1,(NSK2Debug[1]>>17)&0x1,(NSK2Debug[1]>>16)&0x1);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"LocalSelectD = %x, LocalSelectS = %x\n",(NSK2Debug[1]>>12)&0xf,(NSK2Debug[1]>>8)&0xf);
    HALNSK2_DBG(NSK2_DBGLV_DEBUG,"ESA select = %x, ESA subselect = %x\n",(NSK2Debug[1]>>4)&0xf,(NSK2Debug[1]>>0)&0xf);
#endif
    return TRUE;
}

MS_U32 HAL_NSK2_GetSRAlignBit(void)
{
    return 16;
}

/* Temporary, since RSA sec_range drv is non-ready*/
MS_BOOL HAL_NSK2_RSA_SetSecureRange(MS_U32 u32SecSet, MS_U32 u32SecStart, MS_U32 u32SecEnd, MS_BOOL bEnable)
{
    if(u32SecSet > REG_MPROT_SECRANGE_SET  || u32SecEnd <= u32SecStart)
        return FALSE;

    *(volatile MS_U32*) (_gBasicAddr + REG_MPROT_SECRANGE_START(u32SecSet)) = (u32SecStart & MPROT_SECRANGE_MASK);

    if (TRUE == bEnable)
    {
        *(volatile MS_U32*) (_gBasicAddr + REG_MPROT_SECRANGE_END(u32SecSet))   = REG_MPROT_SECRANGE_ENABLE & ( u32SecEnd   & MPROT_SECRANGE_MASK);
    }
    else
    {
        *(volatile MS_U32*) (_gBasicAddr + REG_MPROT_SECRANGE_END(u32SecSet))   = (~REG_MPROT_SECRANGE_ENABLE) | ( u32SecEnd   & MPROT_SECRANGE_MASK);
    }

    return TRUE;
}
/**
 * @brief Configure CA secure range mask
 */
void HAL_NSK2_RSA_SetSR_Mask(MS_U32 u32DramSize)
{
    MS_U32 u32Mask = 0x00000000;

    if (0 != ((u32DramSize >> 16) >> 8))
    {
        u32Mask = (((u32DramSize >> 16) >> 8) - 1) & 0xFF;
    
        *(volatile MS_U32*) (_gBasicAddr + REG_MPROT_SECRANGE_MASK) = u32Mask;
    }
}
//=====================================
//======   NSK2.1 new functions  ======
//=====================================


void HAL_NSK2_ReadAllOTP(void)
{
    MS_U32 u32RetValue, addr;

    for( addr=0 ; addr<0x4000; addr+=4)
    {
        HAL_NSK2_OTP_Get(addr, 31, 0, &u32RetValue);
        //if( (addr&0xf) == 0)
        //    printf("\n");

        if(u32RetValue != 0)
        {
            printf("addr = %x, value = %x \n",addr, u32RetValue);
        }
    }
}

MS_U32 HAL_NSK2_ReadKTEResp(void)
{
    StatusCheck(HAL_NSK2_CheckBusy());

    MS_U32 u32KTEResp;
    u32KTEResp = NI_REG(REG_NI_NSK2_KTE_RESP);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"KTE_RESP = %x\n",u32KTEResp);

    return u32KTEResp;
}

MS_BOOL HAL_NSK2_GetPubOTP(MS_U8 *pPubOTP)
{
#if 0
Table 7 Public OTP Bits
Bit 	Name 	Description
127:64 	U_OTP_v_pubOtpUniqueID1 	OTP unique ID.
This field is unique chip data that can be shared with other non-NDS modules.
63:48 	U_OTP_v_PubOtpGP 	        General purpose OTP field.
47:40 	U_OTP_v_PubOtpMinConfVer 	OTPpublic minimum configuration version.
39:36 	U_OTP_v_PubOtpRsaIndex   	OTP public RSA index
35:32 	U_OTP_v_PubOtpBID       	OTP public BlackBox ID
31:16 	U_OTP_v_PubOtpVID        	OTP public version ID
15:0 	U_OTP_v_PubOtpOID       	OTP public owner ID
#endif

//U_OTP_v_pubOtpUniqueID1           0x3DA8  63  0
//U_OTP_v_PubOtpGP                  0x3B08  15  0
//U_OTP_v_PubOtpMinConfVer          0x3B14   7  0
//U_OTP_v_PubOtpRsaIndex            0x3B10   3  0
//U_OTP_v_PubOtpBID                 0x3B0C   3  0
//U_OTP_v_PubOtpVID                 0x3B04  15  0
//U_OTP_v_PubOtpOID                 0x3B00  15  0

    MS_U8 *pRunPubOTP = (MS_U8 *)pPubOTP;
    MS_U32 ReadValue,ReadValue2;

    HAL_NSK2_OTP_Get(0x3B00, 15, 0, &ReadValue);
    printf("U_OTP_v_PubOtpOID = %x\n",ReadValue);
    memcpy(pRunPubOTP,&ReadValue,2);
    pRunPubOTP += 2;

    HAL_NSK2_OTP_Get(0x3B04, 15, 0, &ReadValue);
    printf("U_OTP_v_PubOtpVID = %x\n",ReadValue);
    memcpy(pRunPubOTP,&ReadValue,2);
    pRunPubOTP += 2;

    HAL_NSK2_OTP_Get(0x3B0C, 3, 0, &ReadValue);
    printf("U_OTP_v_PubOtpBID = %x\n",ReadValue);
    HAL_NSK2_OTP_Get(0x3B10, 3, 0, &ReadValue2);
    printf("U_OTP_v_PubOtpRsaIndex = %x\n",ReadValue2);
    *pRunPubOTP = (MS_U8)(ReadValue&0xF) + (MS_U8)((ReadValue2&0xF)<<4);
    pRunPubOTP ++;

    HAL_NSK2_OTP_Get(0x3B14, 7, 0, &ReadValue);
    printf("U_OTP_v_PubOtpMinConfVer = %x\n",ReadValue);
    memcpy(pRunPubOTP,&ReadValue,1);
    pRunPubOTP ++;

    HAL_NSK2_OTP_Get(0x3B08, 15, 0, &ReadValue);
    printf("U_OTP_v_PubOtpGP = %x\n",ReadValue);
    memcpy(pRunPubOTP,&ReadValue,2);
    pRunPubOTP += 2;

    HAL_NSK2_OTP_Get(0x3DA8, 31, 0, &ReadValue);
    printf("U_OTP_v_pubOtpUniqueID1 = %x\n",ReadValue);
    memcpy(pRunPubOTP,&ReadValue,4);
    pRunPubOTP += 4;

    HAL_NSK2_OTP_Get(0x3DAC, 31, 0, &ReadValue);
    memcpy(pRunPubOTP,&ReadValue,4);
    pRunPubOTP += 4;

    return TRUE;
}

MS_U32 HAL_NSK21_InvalidCmChannel(MS_U16 PidSlot)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s PidSlot = %x\n", __FUNCTION__,PidSlot);

    MS_U32 u32PidSlot = 0;
    StatusCheck(HAL_NSK2_CheckBusy());

    u32PidSlot = (((MS_U32)PidSlot<<NI_WriteTKey_PidNo_Shift)&NI_WriteTransportKey_PidNo);
    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_InvalidateCmChannel | u32PidSlot);

    return TRUE;
}

typedef enum
{
    NSK21_DVBCSA2 = 0,
    NSK21_DVBCSA2_CONF,
    NSK21_DVBCSA3,
    NSK21_CPCM_LSA_MDI_CBC,
    NSK21_CPCM_LSA_MDI_RCBC,
    NSK21_CPCM_LSA_MDD_CBC,  //5
    NSK21_CPCM_LSA_MDD_RCBC,
    NSK21_SYNAMEDIA_AES,
    NSK21_AES_ECB_CLR,
    NSK21_CIPLUS_AES,
    NSK21_SCTE41_DES, //10
    NSK21_SCTE52_DES,
    NSK21_TDES_ECB_CLR,
    NSK21_MULTI2_TS,
    NSK21_TDES_ECB_CTS,
    NSK21_DES_ECB_CTS, //15
    NSK21_IDSA_AES,
}NSK21_CMChAlgo_e;

MS_U32 HAL_NSK21_CfgCmChannel(MS_U16 PidSlot, MS_U16 LocalDAlgo, MS_U16  ESAAlgo, MS_U16 LocalSAlgo, MS_U16 SysKeyIndex)
{

#define Algoffset     100
    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK2_CfgCmChannel PidSlot = %x, LocalDAlgo = %x, ESAAlgo = %x, LocalSAlgo = %x, SysKeyIndex = %x\n",PidSlot, LocalDAlgo, ESAAlgo, LocalSAlgo, SysKeyIndex);
    StatusCheck(HAL_NSK2_CheckBusy());
    MS_U32 algorithms = 0, u32PidSlot = 0;

    if(LocalDAlgo < Algoffset)
        LocalDAlgo = 0;
    else
        LocalDAlgo -= Algoffset;

    if(ESAAlgo < Algoffset)
        ESAAlgo = 0;
    else
        ESAAlgo -= Algoffset;

    if(LocalSAlgo < Algoffset)
        LocalSAlgo = 0;
    else
        LocalSAlgo -= Algoffset;

#if 0 //tmp solution...
    algorithms = ((LocalDAlgo - 100) & NI_KIW_LSAD_ALGO_MASK) +
                 (((ESAAlgo - 100) << NI_KIW_ESA_ALGO_SHIFT) & NI_KIW_ESA_ALGO_MASK) +
                 (((LocalSAlgo - 100) << NI_KIW_LSAS_ALGO_SHIFT) & NI_KIW_LSAS_ALGO_MASK) ;
#else
    algorithms = (LocalDAlgo & NI_KIW_LSAD_ALGO_MASK) +
                 ((ESAAlgo << NI_KIW_ESA_ALGO_SHIFT) & NI_KIW_ESA_ALGO_MASK) +
                 ((LocalSAlgo << NI_KIW_LSAS_ALGO_SHIFT) & NI_KIW_LSAS_ALGO_MASK) ;
#endif

    HALNSK2_DBG(NSK2_DBGLV_INFO,"algorithms = %x\n",algorithms);
    NI_REG(REG_NI_DSCMB_ALGO) = algorithms;

    u32PidSlot = (((MS_U32)PidSlot<<NI_WriteTKey_PidNo_Shift)&NI_WriteTransportKey_PidNo);
    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_ConfigureCmChannel | u32PidSlot);
    MsOS_DelayTaskUs(1);
    HALNSK2_DBG(NSK2_DBGLV_INFO,"REG_NI_DSCMB_ALGO = %x\n",NI_REG(REG_NI_DSCMB_ALGO));

    return TRUE;
}

MS_U32 HAL_NSK21_WriteTransportKey(MS_U8 SCB, MS_U8 ForceSCB, void *pLocalDIV1, void *pLocalDIV2,
                                   void *pESAIV1, void *pESAIV2, void *pLocalSIV1, void *pLocalSIV2, MS_U16 PidSlot )
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"HAL_NSK21_WriteTransportKey PidSlot = %x, SCB = %x, ForceSCB = %x\n", PidSlot, SCB, ForceSCB);

    MS_U32 data,data2;
    //check NULL pointer or not....

    StatusCheck(HAL_NSK2_CheckBusy());

    data = NI_REG(REG_NI_KTE_STATUS);
    if( ( data & NI_KTE_DEST_MASK ) == 0  )
    {

		ForceSCB = HAL_NSK2_SCBTransToHW(ForceSCB);

        HALNSK2_DBG(NSK2_DBGLV_INFO,"PidSlot: %x\n", PidSlot);
        data2 = (((MS_U32)PidSlot<<NI_WriteTKey_PidNo_Shift)&NI_WriteTransportKey_PidNo) +
                (((MS_U32)SCB<<NI_WriteTKey_SCB_Shift) & NI_WriteTKey_SCB_MASK)
                + ( ( (MS_U32)ForceSCB<<NI_WriteTKey_FSCB_Shift) & NI_WriteTKey_FSCB_MASK) ;

    #if 1
        NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteTransportKey | data2 | NI_OTP_ACK_NSK2 | NI_ERR_INVALID_SLOT);
    #else
        NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteTransportKey | data2);
    #endif
        HALNSK2_DBG(NSK2_DBGLV_INFO,"NI 6 = %x\n", NI_REG(REG_NI_COMMAND));

        StatusCheck(HAL_NSK2_KIW_BusyPolling());
    }
    else
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"WriteTransportKey abnormal ignored: KteDest not zero\n");
        return HAL_NSK2_ReadKTEResp();
    }

    return HAL_NSK2_ReadKTEResp();
}

MS_U32 HAL_NSK21_WriteM2MKey(MS_U32 M2MAlgo)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s M2MAlgo = %x\n",__FUNCTION__,M2MAlgo);

    MS_U32 algorithms;

    StatusCheck(HAL_NSK2_CheckBusy());

    algorithms = (M2MAlgo & NI_KIW_LSAD_ALGO_MASK) +
                 ((M2MAlgo << NI_KIW_LSAS_ALGO_SHIFT) & NI_KIW_LSAS_ALGO_MASK) ;

    HALNSK2_DBG(NSK2_DBGLV_INFO,"algorithms = %x\n",algorithms);
    NI_REG(REG_NI_DSCMB_ALGO) = algorithms;
    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteM2MKey);

    MsOS_DelayTaskUs(1);

    return HAL_NSK2_ReadKTEResp();
}

MS_U32 HAL_NSK21_ModifyGenIn(MS_U32 MaskVal,MS_U32 XorVal)
{
    MS_U32 GenIn = 0, WriteGenIn = 0;

    StatusCheck(HAL_NSK2_CheckBusy());

    GenIn = NI_REG(REG_NI_NSK2_REG_GENIN);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s GenIn = %x\n",__FUNCTION__,GenIn);

    GenIn = (GenIn&MaskVal);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s GenIn = %x, XorVal = %x\n",__FUNCTION__,GenIn,XorVal);

    WriteGenIn = GenIn^XorVal;
    printf("WriteGenIn = %x\n",WriteGenIn);

#ifdef TestGenIn
    NI_REG(REG_NI_NSK21_GENIN) = WriteGenIn;
    MsOS_DelayTaskUs(10);
#else

    //NI_REG(REG_NI_NSK21_GENIN) = ( (WriteGenIn & BMASK(9:0)) | (WriteGenIn & __BIT(13)) );
    NI_REG(REG_NI_NSK21_GENIN) = WriteGenIn;

    NI_REG(REG_NI_NSK21_CONCURR_PROT_EN) = ((WriteGenIn>>10) & BMASK(1:0));
    NI_REG(REG_NI_NSK21_CONCURR_SET) = ((WriteGenIn>>12) & BMASK(0:0));
    NI_REG(REG_NI_NSK21_GEN_SHOT) = ((WriteGenIn>>14) & BMASK(3:0));

    MsOS_DelayTaskUs(10);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"CONCURR_PROT_EN = %x, CONCURR_SET = %x, GEN_SHOT = %x\n", NI_REG(REG_NI_NSK21_CONCURR_PROT_EN), NI_REG(REG_NI_NSK21_CONCURR_SET), NI_REG(REG_NI_NSK21_GEN_SHOT));
#endif
    GenIn = NI_REG(REG_NI_NSK2_REG_GENIN);

    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s GenIn = %x\n",__FUNCTION__,GenIn);

    if(GenIn == WriteGenIn)
        return TRUE;
    else
        return FALSE;

}

MS_U32 HAL_NSK21_GetGenIn(void)
{
    return NI_REG(REG_NI_NSK2_REG_GENIN);
}

MS_U32 HAL_NSK21_ReadNIReg(MS_U32 offset)
{
    return NI_REG(offset);
}

void HAL_NSK21_WriteNIReg(MS_U32 offset, MS_U32 Value)
{
    NI_REG(offset) = Value;
}

MS_U32 HAL_NSK21_WriteJTagKey(MS_U32 OverrideOid, MS_U32 Select)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s OverrideOid = %x, Select = %x\n",__FUNCTION__,OverrideOid,Select);

    MS_U32 functionality = 0, OID = 0;

    StatusCheck(HAL_NSK2_CheckBusy());

    if(OverrideOid>1) //invalid OverrideOid
    {
        HALNSK2_DBG(NSK2_DBGLV_ERR,"invalid OverrideOid\n");
        return FALSE;
    }

    OID = (OverrideOid << NI_KIW_OID_SHIFT) & NI_KIW_OID_MASK ;
#if 0
    functionality =  (Select << NI_WriteTKey_PidNo_Shift) & NI_WriteTransportKey_PidNo ;
#else
    functionality = 0; //N21_EJTAG_PWD
#endif

    HALNSK2_DBG(NSK2_DBGLV_INFO,"functionality = %x\n",functionality);
    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteJTAGKey | functionality | OID);

    MsOS_DelayTaskUs(1);

    return HAL_NSK2_ReadKTEResp();
}

MS_U32 HAL_NSK21_IncrementNvCounter(void)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s \n",__FUNCTION__);

    StatusCheck(HAL_NSK2_CheckBusy());

    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_IncrementNvCounter);
    MsOS_DelayTaskUs(1);

    return HAL_NSK2_ReadKTEResp();
}

MS_U32 HAL_NSK2_WriteOtpKey(void)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s \n",__FUNCTION__);
    StatusCheck(HAL_NSK2_CheckBusy());

    NI_REG(REG_NI_COMMAND) = (NI_COMMAND_START | NI_WriteOTPKey);
    MsOS_DelayTask(10);

    return HAL_NSK2_ReadKTEResp();
}

MS_U32 HAL_NSK2_ReadPWD_Status(void)
{
    HALNSK2_DBG(NSK2_DBGLV_INFO,"%s \n",__FUNCTION__);
    StatusCheck(HAL_NSK2_CheckBusy());
    MS_U32 RegPwd;

    RegPwd = NI_REG(REG_NI_NSK2_PWD_ON);

    if(RegPwd & N2_JTAGPWD0_ON)
    {
        printf("JTAG PWD0 ON \n");
    }

    if(RegPwd & N2_JTAGPWD1_ON)
    {
        printf("JTAG PWD1 ON \n");
    }

    if(RegPwd & N2_SCANPWD_ON)
    {
        printf("SCAN PWD ON \n");
    }

    if(RegPwd & N2_MBISTPWD_ON)
    {
        printf("MBIST PWD ON \n");
    }

    if(RegPwd & N2_M2MKEY_ON)
    {
        printf("M2M KEY ON \n");
    }

    if(RegPwd & N2_SCPUKEY0_ON)
    {
        printf("SCPU KEY0 ON \n");
    }

    if(RegPwd & N2_SCPUKEY1_ON)
    {
        printf("SCPU KEY1 ON \n");
    }

    if(RegPwd & N2_RNGVALUE0_ON)
    {
        printf("RNG VALUE0 ON \n");
    }

    if(RegPwd & N2_RNGVALUE1_ON)
    {
        printf("RNG VALUE1 ON \n");
    }

    return TRUE;
}

MS_U32 HAL_NSK2_CtrlClk(MS_BOOL Enable)
{
    if(Enable)
    {
        printf("enable NSK clock\n");
        NI_REG(REG_NI_NSK2_CTRL) |= (NI_NSK2_CLK_ENABLE | NI_NSK2_RESET_DISABLE);
        HAL_NSK2_ColdReset();
    }
    else
    {
        printf("disable NSK clock\n");
        NI_REG(REG_NI_NSK2_CTRL) &= ~(NI_NSK2_CLK_ENABLE | NI_NSK2_RESET_DISABLE);
        _gReset = FALSE;
        _gCheckBusyFlag = FALSE;
    }

    MsOS_DelayTask(1);


    printf("REG_NI_NSK2_CTRL = %x\n",NI_REG(REG_NI_NSK2_CTRL));
    return TRUE;
}

MS_U32 HAL_NSK2_ReadClkStatus(void)
{
    MS_U32 status;
    MS_U32 rom[4];

    status = (NI_REG(REG_NI_KTE_STATUS)& NI_SLOW_CLOCK_DETECT);

    printf("REG_NI_KTE_STATUS = %x\n",NI_REG(REG_NI_KTE_STATUS));
    printf("NI_SLOW_CLOCK_DETECT = %x\n", (status>>5));

    if(status)
    {
        printf("no clock for NSK\n");
    }
    else
    {
        printf("clock is alive for NSK\n");
    }

    printf("read NSK ROM value\n");
    HAL_NSK2_ReadData32(0x8000, 4, &rom[0]);

    printf("%x, %x, %x, %x,\n",rom[0],rom[1],rom[2],rom[3]);

    return TRUE;
}

