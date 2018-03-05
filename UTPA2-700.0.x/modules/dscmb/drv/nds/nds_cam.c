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
/// file    drvNDS_CAM.c
/// @brief  NDS CAM Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#ifdef MSOS_TYPE_LINUX
#include "string.h"
#endif

#include "drvDSCMB.h"

#include "regNDS.h"
#include "drvNDS.h"
#include "halNDS.h"

//#include "../rasp/halRASP.h"

#include "../dscmb/drvDSCMB.h" //FIXME: move up or embedded

#include "ddiNDS_HDI.h"

#include "nds.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define CAM_MAX_DSCMB_NUM           6
#define CAM_MAX_CONNECTION          2


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32                          u32DscmbId;
    MS_U16                          u16Pid;
    MS_U8                           u8Status;
} CAM_Dscmb;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static CAM_Dscmb                    _Dscmb[CAM_MAX_CONNECTION][CAM_MAX_DSCMB_NUM];
static MS_U8                        _u8DscmbType[CAM_MAX_CONNECTION];

#define DSCMB_TYPE_NULL             0xFF



//static MS_U32 u32ChipPropLen = 0;

static MS_U8 u8ChipType[] = // chip type descriptor
{
    CHIP_TYPE_DSCR_TAG, 4,
    ((NDS_CHIP_TYPE >> 24) & 0xFF), ((NDS_CHIP_TYPE >> 16) & 0xFF), ((NDS_CHIP_TYPE >> 8) & 0xFF), ((NDS_CHIP_TYPE >> 0) & 0xFF),
};

static MS_U8 u8ChipId[] =  // chip id descriptor
{
    CHIP_ID_DSCR_TAG, 4,
    ((NDS_CHIP_ID >> 24) & 0xFF), ((NDS_CHIP_ID >> 16) & 0xFF), ((NDS_CHIP_ID >> 8) & 0xFF), ((NDS_CHIP_ID >> 0) & 0xFF), // default
};

static MS_U8 _u8DefaultKey[] =
{
    0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87, 0x78, 0x69, 0x5A, 0x4B, 0x3C, 0x2D, 0x1E, 0x0F
};

static MS_U8 u8HashKey[] =  // Hash of Key descriptor
{
    HASH_KEY_DSCR_TAG, 16,
    NDS_HASH_KEY,
//    0xec, 0x04, 0x6d, 0x48, 0x0f, 0xc4, 0xd2, 0x81, 0x32, 0x3e, 0x1f, 0x5d, 0xbe, 0xed, 0x3f, 0x8b, // Hash of Key descriptor
};

static MS_U8 u8KLDepth[] =  // Hash of Key descriptor
{
    KEY_LADDER_DEPTH_DSCR_TAG, 1,
    NDS_KL_DEPTH, // key lader depth descriptor
};

static MS_U8 u8ChipEnFlags[] =  // Chip Enabled Flag Descriptor
{
    ENABLE_FLAGS_DSCR_TAG, 1,
    NDS_ENFLAGS_DEFAULT,
};



#define NDS_CHIP_PROP_LENGTH        (sizeof(u8ChipType)+ sizeof(u8ChipId)+ sizeof(u8HashKey)+ sizeof(u8KLDepth)+ sizeof(u8ChipEnFlags))
// static MS_U8 u8ChipProp[NDS_CHIP_PROP_LENGTH];


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
/*
unsigned long GetXConnection(void)
{
    return 0;
}
*/

static MS_BOOL _NDS_CamMapInit(CAM_Dscmb* pDscmb)
{
    pDscmb->u32DscmbId = DRV_DSCMB_FLT_NULL;
    pDscmb->u16Pid = 0x1FFF;
    pDscmb->u8Status = 0;
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

NDS_Result NDS_CAM_Init(void)
{
    MS_U32 i, j;

    for (i = 0; i < CAM_MAX_CONNECTION; i++)
    {
        for (j = 0; j < CAM_MAX_DSCMB_NUM; j++)
        {
            _NDS_CamMapInit(&_Dscmb[i][j]);
        }
    }

    MDrv_DSCMB_KLadder_SetInput(1, _u8DefaultKey, 16);
    MDrv_DSCMB_KLadder_SetInput(2, _u8DefaultKey, 16);

//    _NDS_SC_Ctrl_Reg |= CAM_UART_CTRL_AUTO_FLOWCTRL;//0x14;
//    CAM_REG(REG_CAM_UART_CTRL) = _NDS_SC_Ctrl_Reg;

    return E_NDS_OK;
}


NDS_Result NDS_CAM_Exit(void)
{
    return E_NDS_OK;
}


// CA module hardware indeitifcation --------------------------------------------------------------

// 5.2
NDS_STATUS XHDICA_GetLogicalStreamId (int request_type,
                                      NDS_ULONG x_connection,
                                      NDS_BYTE *hw_module_id)
{
    NDS_FUNC("[%s][%d] \n", __FUNCTION__, __LINE__);

    if ((request_type < HDICA_GET_EMM_STREAM) || (request_type > HDICA_GET_DESCRAMBLER_STREAM))
    {
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    hw_module_id = 0;
    return CA_OK;
}


// Smart card driver ------------------------------------------------------------------------------

// nds_sc.c

// 6.3.1
NDS_STATUS HDICA_SetCardId(NDS_ULONG card_id)
{
    NDS_FUNC("[%s][%d]--card_id[0x%lx] \n", __FUNCTION__, __LINE__, card_id);

    CAM_REG(REG_CAM_CA_CARDID_0) = 0xFF & card_id;
    CAM_REG(REG_CAM_CA_CARDID_1) = 0xFF & (card_id>>8);
    CAM_REG(REG_CAM_CA_CARDID_2) = 0xFF & (card_id>>16);
    CAM_REG(REG_CAM_CA_CARDID_3) = 0xFF & (card_id>>24);

    return CA_OK;
}


// 6.3.2
NDS_STATUS HDICA_SetIrdId(NDS_ULONG ird_id)
{
    NDS_FUNC("[%s][%d]--ird_id[0x%lx] \n", __FUNCTION__, __LINE__, ird_id);

    CAM_REG(REG_CAM_CA_IRDID_0) = 0xFF & ird_id;
    CAM_REG(REG_CAM_CA_IRDID_1) = 0xFF & (ird_id>>8);
    CAM_REG(REG_CAM_CA_IRDID_2) = 0xFF & (ird_id>>16);
    CAM_REG(REG_CAM_CA_IRDID_3) = 0xFF & (ird_id>>24);

    return CA_OK;
}

// ECM filtering and reception --------------------------------------------------------------------

// nds_flt.c

// 8.2.6
NDS_STATUS XHDICA_SetCaValue( NDS_ULONG  x_connection, NDS_BYTE ca_value )
{
    return (HDICA_SetCaValue(ca_value));
}


// 8.2.7
NDS_STATUS HDICA_SetCaValue (NDS_BYTE ca_value)
{
    NDS_FUNC("[%s]-[%d] ca_value[0x%x]\n", __FUNCTION__, __LINE__, ca_value);

    //printf("[%s]-[%d] ca_value[0x%x]\n", __FUNCTION__, __LINE__, ca_value);

    CAM_REG(REG_CAM_CA_SBOX) = ca_value;
    return CA_OK;
}


NDS_STATUS XHDICA_DefineDescramblerType(NDS_ULONG x_connection, NDS_BYTE descrambler_type)
{
    NDS_FUNC("[%s]-[%d] descrambler_type[0x%x]\n", __FUNCTION__, __LINE__, descrambler_type);

    switch (descrambler_type)
    {
        case CSA:
        case NSA:
        case DES:
        case AES:
            break;
    default:
        descrambler_type = DSCMB_TYPE_NULL;
        break;
    }
    _u8DscmbType[x_connection] = descrambler_type;
    return CA_OK;
}

NDS_STATUS XHDICA_DefineDescramblerEs(NDS_ULONG x_connection,
                                      NDS_BYTE descrambler_num,
                                      NDS_USHORT pid,
                                      NDS_BYTE filter_control)
{
    CAM_Dscmb* pDscmb = NULL;
    NDS_STATUS ret = CA_OK;
    DSCMB_Type DscmbType;

    NDS_FUNC("[%s]-[%d] descrambler_num[%d] pid[0x%x] con[0x%x]\n", __FUNCTION__, __LINE__, descrambler_num, pid, filter_control);
//    printf("[%s]-[%d] descrambler_num[%d] pid[0x%x] con[0x%x]\n", __FUNCTION__, __LINE__, descrambler_num, pid, filter_control);

    if (pid < 2 || pid > 0x1FFE)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    if (CAM_MAX_DSCMB_NUM <= descrambler_num)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    // HDICA_DESCRAMBLE_PES_PACKET_LEVEL
    if (HDICA_DESCRAMBLE_PES_PACKET_LEVEL & filter_control)
    {
        ret = CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
        NDS_ASSERT(FALSE, ret, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
        goto XHDICA_DefineDescramblerEs_exit;
    }

    pDscmb = &(_Dscmb[x_connection][descrambler_num]);
    if (!(HDICA_DESCRAMBLER_CONTROL_ENABLE_DESCRAMBLING & filter_control))
    {
        if (pDscmb->u32DscmbId != DRV_DSCMB_FLT_NULL)
        {
//            MS_U8        CWFF[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

            MDrv_DSCMB_FltKeyReset(pDscmb->u32DscmbId, E_DSCMB_KEY_CLEAR);
            MDrv_DSCMB_FltKeyReset(pDscmb->u32DscmbId, E_DSCMB_KEY_EVEN);
            MDrv_DSCMB_FltKeyReset(pDscmb->u32DscmbId, E_DSCMB_KEY_ODD);
            MDrv_DSCMB_FltDisconnectPid(pDscmb->u32DscmbId, pid);
            MDrv_DSCMB_FltFree(pDscmb->u32DscmbId);
            _NDS_CamMapInit(pDscmb);
        }
        goto XHDICA_DefineDescramblerEs_exit;
    }


    switch (_u8DscmbType[x_connection])
    {
    case CSA:
        DscmbType = E_DSCMB_TYPE_CSA;
        break;
    case NSA:
        DscmbType = E_DSCMB_TYPE_NSA_AS_ESA;
        break;
    case DES:
        DscmbType = E_DSCMB_TYPE_DES;
        break;
    case AES:
        DscmbType = E_DSCMB_TYPE_SYN_AES;
        break;
    default:
        ret = CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
        NDS_ASSERT(FALSE, ret, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
        goto XHDICA_DefineDescramblerEs_exit;
    }
    // HDICA_DESCRAMBLER_CONTROL_ENABLE_DESCRAMBLING || HDICA_DESCRAMBLE_TRANSPORT_PACKET_LEVEL
    if ((DRV_DSCMB_FLT_NULL == pDscmb->u32DscmbId) && (DRV_DSCMB_FLT_NULL == (pDscmb->u32DscmbId = MDrv_DSCMB_FltAlloc())))
    {
        ret = CA_GLOBAL_HARDWARE_FAILURE;
        NDS_ASSERT(FALSE, ret, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
        goto XHDICA_DefineDescramblerEs_exit;
    }
    MDrv_DSCMB_FltConnectPid(pDscmb->u32DscmbId, pid);
    MDrv_DSCMB_FltTypeSet(pDscmb->u32DscmbId, DscmbType);
    pDscmb->u16Pid = (MS_U16)pid;
    pDscmb->u8Status |= (MS_U8)filter_control;

XHDICA_DefineDescramblerEs_exit:
    return ret;
}

// Legacy interface
NDS_STATUS XHDICA_SetEvenKey(NDS_ULONG x_connection, NDS_BYTE descrambler_num, NDS_BYTE * key_ptr)
{
    CAM_Dscmb* pDscmb = NULL;

    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    if (CAM_MAX_DSCMB_NUM <= descrambler_num)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    pDscmb = &(_Dscmb[x_connection][descrambler_num]);
    if (pDscmb->u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        return CA_INFO_NOT_AVAILABLE;
    }
    MDrv_DSCMB_FltKeySet(pDscmb->u32DscmbId, E_DSCMB_KEY_EVEN, key_ptr);
    return CA_OK;
}

// Legacy interface
NDS_STATUS XHDICA_SetOddKey(NDS_ULONG x_connection, NDS_BYTE descrambler_num, NDS_BYTE * key_ptr)
{
    CAM_Dscmb* pDscmb = NULL;

    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    if (CAM_MAX_DSCMB_NUM <= descrambler_num)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    pDscmb = &(_Dscmb[x_connection][descrambler_num]);
    if (pDscmb->u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        return CA_INFO_NOT_AVAILABLE;
    }
    MDrv_DSCMB_FltKeySet(pDscmb->u32DscmbId, E_DSCMB_KEY_ODD, key_ptr);
    return CA_OK;
}

NDS_STATUS XHDICA_GetSecurityChipNumber (NDS_BYTE *scn)
{
    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    *scn = 0xFF;
    return CA_OK;
}

#if 0 //[FIXME] if enable Rasp
NDS_STATUS XHDICA_ECMCorruptionSettings(NDS_BYTE CorruptDataByte, NDS_BYTE FromByte)
{
    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    MS_U32 EngId =  0;

    //[FIXME] Not Xconection ?
    HAL_RASP_SetCorptData(EngId , FromByte , 187 , CorruptDataByte);

//    MDrv_DSCMB_EcmCorruptSet((MS_U8)CorruptDataByte, (MS_U8)FromByte);
    return CA_FEATURE_NOT_SUPPORTED;

}



NDS_STATUS XHDICA_EnableECMCorruption(NDS_ULONG x_connection, NDS_USHORT pid, NDS_BYTE enable_corruption)
{
    // @FIXME: complete _NDS_EMM_ECM_Flt_CorruptEnable implementation later

    MS_U32 EngId = 0, i ;
    MS_U16 FltPid = 0  ;
    MS_BOOL enable ;


    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    if (enable_corruption == 1)
       enable = TRUE ;
    else
       enable = FALSE ;


    for( i = 0 ; i < RASP_EVENT_NUM ; i++)
    {
        HAL_RASP_GetPidflt(EngId, i , &FltPid);
        if (FltPid == pid) break ;
    }
    if (RASP_EVENT_NUM  == i) return CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW ;

    HAL_RASP_SetCorptFlt(EngId,i,enable);


    return CA_OK;
}
#endif


NDS_STATUS XHDICA_SecondaryScramblingKey(NDS_ULONG x_connection, NDS_BYTE operation, NDS_BYTE * key_ptr)
{
// nsk packet
    // @FIXME: Richard don't know how to deal with secondary key
    //TODO:
    NDS_ASSERT(FALSE, CA_FEATURE_NOT_SUPPORTED, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);

    if (operation == CA_OP_BASE_KEY)
    {}
    else if (operation == CA_OP_WRITING)
    {}
    else
    {}

    //TODO:
    return CA_FEATURE_NOT_SUPPORTED;
}


static void _NDS_CamGetHashKey(MS_U8 *pu8KeyData)
{
    DSCMB_KLCfg KLConfig;

    NDS_DBG("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    KLConfig.bDecrypt = TRUE;
    KLConfig.bInverse = FALSE;
    KLConfig.bKeepRound = FALSE;
    KLConfig.u32Round = 6;
    KLConfig.u32EngMode = E_DSCMB_KL_TDES; // @FIXME: only TDES HASH key?

    MDrv_DSCMB_KLadder_SetConfig(&KLConfig);
    MDrv_DSCMB_KLadder_SetKey(E_DSCMB_KL_SRC_VGK, NULL, 0);
    MDrv_DSCMB_KLadder_SetDst(E_DSCMB_KL_DST_ACPU);

    MDrv_DSCMB_KLadder_SetInput(0, _u8DefaultKey, 16);
    MDrv_DSCMB_KLadder_SetInput(1, _u8DefaultKey, 16);
    MDrv_DSCMB_KLadder_SetInput(2, _u8DefaultKey, 16);

    MDrv_DSCMB_KLadder_Start();

    // (3) polling status
    while (1)
    {
        if (MDrv_DSCMB_KLadder_IsComplete())
        {
            break;
        }
    }

    //Read key
    MDrv_DSCMB_KLadder_ReadDataFromAcpu(pu8KeyData);
    //_verSC_PrintData(pu8KeyData, 18);

    //ACK --release key
    MDrv_DSCMB_KLadder_ResetAcpuAck();

    // (5) Poll key ladder done
    while (1)
    {
        if (MDrv_DSCMB_KLadder_Output_IsComplete())
        // if (kl_done == ((reg_kl_status & 0x1)))
        {
            break;
        }
    }

    MDrv_DSCMB_KLadder_Output_Stop();

    // (6) Clear key ladder control
    MDrv_DSCMB_KLadder_Stop();
}

NDS_STATUS XHDICA_GetChipProperties(NDS_ULONG hw_module_id,
                                                                unsigned int chip_prop_length,
                                                                unsigned int *actual_chip_prop_length,
                                                                NDS_BYTE *chip_properties,
                                                                void *auxiliary_data)
{
    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    MS_U8* u8Ptr;
    // @FIXME: return no data for capacity query at this stage
    //TODO:
    *actual_chip_prop_length = NDS_CHIP_PROP_LENGTH;
    if (0 == chip_prop_length)
    {
        return CA_OK;
    }
    NDS_ASSERT((chip_prop_length>= NDS_CHIP_PROP_LENGTH), CA_INSUFFICIENT_BUFFER, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);

    u8Ptr = chip_properties;
    memcpy(u8Ptr, u8ChipType, sizeof(u8ChipType));
    u8Ptr += sizeof(u8ChipType);

    // otp_cfg_did2
    u8ChipId[2] = (CFG_REG(REG_CFG_DID2_H) >> 8) & 0xFF;
    u8ChipId[3] = (CFG_REG(REG_CFG_DID2_H) >> 0) & 0xFF;
    u8ChipId[4] = (CFG_REG(REG_CFG_DID2_L) >> 8) & 0xFF;
    u8ChipId[5] = (CFG_REG(REG_CFG_DID2_L) >> 0) & 0xFF;
    memcpy(u8Ptr, u8ChipId, sizeof(u8ChipId));
    u8Ptr += sizeof(u8ChipId);

    _NDS_CamGetHashKey(&u8HashKey[2]);
    memcpy(u8Ptr, u8HashKey, sizeof(u8HashKey));
    u8Ptr += sizeof(u8HashKey);

    memcpy(u8Ptr, u8KLDepth, sizeof(u8KLDepth));
    u8Ptr += sizeof(u8KLDepth);

    //(0<< 3) // Jtag password enabled , 0xBF221400+0x11[10-15]
    //(0<< 4) // Boot Room enabled, 0xBF221400+0x0E[4-9] ==> 0x3F0
    if ( (CFG_REG(REG_CFG_MODE) & CFG_MODE_JTAG_PWD) && !(CFG_REG(REG_CFG_MODE) & CFG_MODE_JTAG_CLOSE) )
    {
        u8ChipEnFlags[2] |= NDS_ENFLAGS_JTAG_PWD;
    }
/*    if (((*((volatile unsigned int*)( 0xBF221400+(0x11<< 2))) >> 10) > 0) && ((*((volatile unsigned int*)( 0xBF221400+(0x11<< 2))) >> 10) <= 0x07))
        u8DscrEnableFlags[2] |= (1<<3);
*/
    if (IS_CFG_BOOT_OTP())
    {
        u8ChipEnFlags[2] |= NDS_ENFLAGS_BOOTROM;
    }
/*    if ((*((volatile unsigned int*)( 0xBF221400+(0x0E<< 2))) >> 4) > 0x3F)
        u8ChipEnFlags[2] |= (1<<4);
*/
    memcpy(u8Ptr, u8ChipEnFlags, sizeof(u8ChipEnFlags));
    u8Ptr += sizeof(u8ChipEnFlags);
    return CA_OK;
}

typedef struct
{
    MS_U8* pu8Key;
    MS_U8 u8Level;
    MS_U8 u8KeyLen;
} _EncyptKeyDscr;

static MS_U32 _XHDICA_GetKeyLength(DSCMB_Type DscmbType)
{
    switch (DscmbType)
    {
    case E_DSCMB_TYPE_CSA:
        return 8;
    case E_DSCMB_TYPE_NSA_AS_ESA:
        return 16;
    case E_DSCMB_TYPE_DES:
        return 8;
    case E_DSCMB_TYPE_AES:
        return 16;
    default:
        return 0;
    }
    return 0;
}


NDS_STATUS XHDICA_SetControlWord( NDS_ULONG x_connection,
                                  NDS_USHORT descrambler_num,
                                  unsigned int control_data_length,
                                  NDS_BYTE *control_data,
                                  unsigned int polarity_control )
{
    // New interface
    CAM_Dscmb *         pDscmb = NULL;
    MS_U32              u32Len;
    DSCMB_Key_Type      DscmbKeyType;
    MS_BOOL             bEncyptedCW = FALSE;
    MS_U32              u32KLScheme  = E_DSCMB_KL_TDES;
    _EncyptKeyDscr      enKeyDscr[4];
    MS_U32              nKeyDscr = 0;
    DSCMB_KLCfg         KLConfig;
    MS_U32              u32CWLength;
    MS_U8 *             pu8EnCW = NULL;
    MS_U32              u32EnCWLen = 0;
    MS_U32              u32Round;

    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    pDscmb = &(_Dscmb[x_connection][descrambler_num]);
    if (pDscmb->u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        return CA_INFO_NOT_AVAILABLE;
    }

    u32CWLength = _XHDICA_GetKeyLength((DSCMB_Type)_u8DscmbType[x_connection]);
    switch (polarity_control)
    {
    case POLARITY_CLEAR:
        DscmbKeyType = E_DSCMB_KEY_CLEAR;
        break;
    case POLARITY_ODD:
        DscmbKeyType = E_DSCMB_KEY_ODD;
        break;
    case POLARITY_EVEN:
        DscmbKeyType = E_DSCMB_KEY_EVEN;
        break;
    default:
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    nKeyDscr = 0;
    while (control_data_length)
    {
        u32Len = (control_data[1] + 2);
        switch (control_data[0]) // tag
        {
        case CLEAR_CW_DSCR_TAG: // 0x01
            NDS_ASSERT((u32CWLength == control_data[1]), CA_REQUEST_NOT_SUPPORTED_BY_DRIVER, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
            //if (descrambler_num == 0)
                //_verSC_PrintData(&(control_data[2]), 8);
            MDrv_DSCMB_FltKeySet(pDscmb->u32DscmbId, DscmbKeyType, &(control_data[2]));
            break;
        case ENCRYPTED_CW_DSCR_TAG: // 0x02
            bEncyptedCW = TRUE;
            pu8EnCW = &(control_data[2]);
            u32EnCWLen = control_data[1];
//            NDS_ASSERT((u32KeyLength == control_data[1]), CA_REQUEST_NOT_SUPPORTED_BY_DRIVER, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
            MDrv_DSCMB_KLadder_SetKey(E_DSCMB_KL_SRC_VGK, NULL, 0);//just ACPU need set key
            MDrv_DSCMB_KLadder_SetInput(0, pu8EnCW, u32EnCWLen);
            break;
        case ENCRYPTED_KEY_DSCR_TAG: // 0x03
            bEncyptedCW = TRUE;
            enKeyDscr[nKeyDscr].u8Level = control_data[2];
            //MS_ASSERT(1== enKeyDscr[nKeyDscr].u8Level); // must be level 1,2
            enKeyDscr[nKeyDscr].u8KeyLen = control_data[3];
            //MS_ASSERT(16 == enKeyDscr[nKeyDscr].u8KeyLen); // must be 128, 64 bits
            enKeyDscr[nKeyDscr].pu8Key = &(control_data[4]);
            MDrv_DSCMB_KLadder_SetInput(enKeyDscr[nKeyDscr].u8Level, enKeyDscr[nKeyDscr].pu8Key, enKeyDscr[nKeyDscr].u8KeyLen);
            nKeyDscr++;
            break;
        case ENCRYPTION_SCHEME_DSCR_TAG: // 0x04
            switch ((control_data[2]<< 8) | control_data[3])
            {
//#if NDS_KL_SCHEME_TDES_SUPPORTED
            case 0x0002:
                u32KLScheme = E_DSCMB_KL_TDES;
                break;
//#endif
#if NDS_KL_SCHEME_AES_SUPPORTED
            case 0x0010:
                u32KLScheme = E_DSCMB_KL_AES;
                break;
#endif
            default:
                u32KLScheme = E_DSCMB_KL_TDES;
                return CA_SCHEME_NOT_SUPPORTED;
            }

            break;
        default:
            printf("[%s][%d] Unsupported tag 0x%02x\n", __FUNCTION__, __LINE__, control_data[0]);
            break;
        }
        if (control_data_length >= u32Len)
        {
            control_data_length -= u32Len;
            control_data += u32Len;
        }
        else
        {
            NDS_ASSERT(FALSE, CA_REQUEST_NOT_SUPPORTED_BY_DRIVER, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    if (bEncyptedCW)
    {
        //MS_ASSERT(u32EnCWLen == u32KeyLength); // Given CW length must be
        if (8 == u32CWLength) // CSA
        {
            u32Round = 5;
        }
        else if (16 == u32CWLength) // NSA, Syn_AES
        {
            u32Round = 6;
        }
        else
        {
            NDS_ASSERT(FALSE, CA_REQUEST_NOT_SUPPORTED_BY_DRIVER, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
            return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
        }

        /// (1) put key
//        MDrv_DSCMB_KLadder_SetKey(E_DSCMB_KLAD_SRC_VGK, NULL, 0);

        /// (1.5) put input
        /// Level 0
    // reg_dcw_in_l = 0x3c, 0x2d, 0x1e, 0x0f;
        // reg_dcw_in_h = 0x78, 0x69, 0x5a, 0x4b;;
        // reg_dcw1_in_l = 0xb4, 0xa5, 0x96, 0x87;
        // reg_dcw1_in_h = 0xf0, 0xe1, 0xd2, 0xc3;
//MDrv_DSCMB_KLadder_SetInput(1, _u8NDS_DefaultIn, 16);//spec form 082.pdf for initaliataion(1~2)
//MDrv_DSCMB_KLadder_SetInput(2, _u8NDS_DefaultIn, 16);

        /// Level 1 for encryption key
        // reg_dcw2_in = enKeyDscr[0].pu8Key;
        // reg_dcw3_in = enKeyDscr[0].pu8Key;
//        MDrv_DSCMB_KLadder_SetInput(1, enKeyDscr[0].pu8Key, 16);

        // Level 2 for encryption CW
        // reg_dcw2_in = pu8EnCW;
        // reg_dcw3_in = pu8EnCW;
//        MDrv_DSCMB_KLadder_SetInput(2, pu8EnCW, 16);

        // (2) key ladder set up
        KLConfig.bDecrypt = TRUE;
        KLConfig.bInverse = FALSE;
        KLConfig.bKeepRound = FALSE;
        KLConfig.u32Round = (MS_U32) u32Round;
        KLConfig.u32EngMode = u32KLScheme;

        MDrv_DSCMB_KLadder_SetConfig(&KLConfig);
        MDrv_DSCMB_KLadder_SetDst(E_DSCMB_KL_DST_KT_ESA);
#if 0 // TEST
        vgk_test[16] = { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                         0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF  };
        MDrv_DSCMB_KLadder_SetKey(E_DSCMB_KL_SRC_ACPU, vgk_test, 16);
        MDrv_DSCMB_KLadder_SetDst(E_DSCMB_KL_DST_ACPU);
#endif

        MDrv_DSCMB_KLadder_Start();

        // (3) polling status
        while (1)
        {
            if (MDrv_DSCMB_KLadder_IsComplete())
            {
                break;
            }
        }

        MDrv_DSCMB_KLadder_Output_Start(pDscmb->u32DscmbId, DscmbKeyType);

        // (5) Poll key ladder done
        while (1)
        {
            if (MDrv_DSCMB_KLadder_Output_IsComplete())
            // if (kl_done == ((reg_kl_status & 0x1)))
            {
                break;
            }
        }
        MDrv_DSCMB_KLadder_Output_Stop();

        // (6) Clear key ladder control
        MDrv_DSCMB_KLadder_Stop();
    }
    return CA_OK;
}


NDS_STATUS XHDICA_GetSerializationInfo(unsigned int info_type,
                                                            unsigned int serializaion_data_len,
                                                            unsigned int *actual_serialization_data_len,
                                                            NDS_BYTE *serialization_data)
{
    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    *actual_serialization_data_len = 0;
    return CA_INFO_NOT_AVAILABLE;
}

NDS_STATUS XHDICA_GetResponseToChallenge( NDS_BYTE *nonce,
                                          NDS_BYTE nonce_length,
                                          int descrambling_key_length,
                                          NDS_BYTE *descrambling_key_descrip,
                                          NDS_BYTE *response,
                                          NDS_BYTE *response_length)
{
    DSCMB_KLCfg         KLConfig;
    MS_U32              u32KLScheme;
    MS_U32              u32Len=0;

    MDrv_DSCMB_KLadder_SetInput(0, nonce, nonce_length);//CW
    MDrv_DSCMB_KLadder_SetInput(1, _u8DefaultKey, 16);
    MDrv_DSCMB_KLadder_SetInput(2, _u8DefaultKey, 16);

    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);
    while (descrambling_key_length)
    {
        u32Len = (descrambling_key_descrip[1] + 2);
        switch (descrambling_key_descrip[0]) // tag
        {
        case CLEAR_CW_DSCR_TAG:
            break;
        case ENCRYPTED_CW_DSCR_TAG:
            break;
        case ENCRYPTED_KEY_DSCR_TAG:
            MDrv_DSCMB_KLadder_SetInput((MS_U32)descrambling_key_descrip[2],
                                        &descrambling_key_descrip[4],
                                        (MS_U32)descrambling_key_descrip[3]);
            break;
        case ENCRYPTION_SCHEME_DSCR_TAG:
            switch ((descrambling_key_descrip[2]<< 8) | descrambling_key_descrip[3])
            {
//#if NDS_KL_SCHEME_TDES_SUPPORTED
            case 0x0002:
                u32KLScheme = E_DSCMB_KL_TDES;
                break;
//#endif
#if NDS_KL_SCHEME_AES_SUPPORTED
            case 0x0010:
                u32KLScheme = E_DSCMB_KL_AES;
                break;
#endif
            default:
                return CA_SCHEME_NOT_SUPPORTED;
            }
            break;
        default:
            printf("[%s][%d] Unsupported tag 0x%02x\n", __FUNCTION__, __LINE__, descrambling_key_descrip[0]);
            break;
        }
        if (descrambling_key_length >= u32Len)
        {
            descrambling_key_length -= u32Len;
            descrambling_key_descrip+= u32Len;
        }
        else
        {
            NDS_ASSERT(FALSE, CA_REQUEST_NOT_SUPPORTED_BY_DRIVER, "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    KLConfig.bDecrypt = TRUE;
    KLConfig.bInverse = FALSE;
    KLConfig.bKeepRound = FALSE;
    KLConfig.u32Round = 6;
    KLConfig.u32EngMode = E_DSCMB_KL_TDES;

    MDrv_DSCMB_KLadder_SetConfig(&KLConfig);
    MDrv_DSCMB_KLadder_SetKey(E_DSCMB_KL_SRC_VGK, NULL, 0);
    MDrv_DSCMB_KLadder_SetDst(E_DSCMB_KL_DST_ACPU);

    MDrv_DSCMB_KLadder_Start();

    // (3) polling status
    while (1)
    {
        if (MDrv_DSCMB_KLadder_IsComplete())
        {
            break;
        }
    }
    //Read key
    MDrv_DSCMB_KLadder_ReadDataFromAcpu(response);
    *response_length = 16;
    //_verSC_PrintData(response, 16);

    //ACK --release key
    MDrv_DSCMB_KLadder_ResetAcpuAck();

    // (5) Poll key ladder done
    while (1)
    {
        if (MDrv_DSCMB_KLadder_Output_IsComplete())
        // if (kl_done == ((reg_kl_status & 0x1)))
        {
            break;
        }
    }
    MDrv_DSCMB_KLadder_Output_Stop();

    // (6) Clear key ladder control
    MDrv_DSCMB_KLadder_Stop();

    return CA_OK;
}

NDS_STATUS XHDICA_EncryptData( void *encrypt_in_params,
                               unsigned int *out_data_buf_len,
                               NDS_BYTE *out_data_buf)
{
    DSCMB_KLCfg KLConfig;
    MS_U8   u8Tag;
    MS_U16  u16InLen;
    MS_U8 pu8InData[16];
    MS_U8   u8Flag;
    MS_U8* pu8InParams = (MS_U8*)encrypt_in_params;
    MS_U32  i=0;
    u8Tag = ((pu8InParams[0])&0xFF);
    u16InLen = ((MS_U16)(((MS_U8*)(pu8InParams))[2]<<8)|(((MS_U8*)(pu8InParams))[3]));//((pu8InParams[3]) & 0xFF);
    u8Flag = ((pu8InParams[4+u16InLen])&0xFF);

    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    if (u8Tag != ENCRYPT_IN_PARAMS_DSCR_TAG)
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;

    if (u8Flag & 0x01)//flag:1
    {
        if (u16InLen > 16)
        {
            *out_data_buf_len = (16 * (u16InLen/16));
            if (u16InLen%16)
                *out_data_buf_len+=16;
        }
        else
            *out_data_buf_len = 16;
    }
    else
    {
        if (out_data_buf == NULL)
        {
            return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
        }
        for (i=0;i<((*out_data_buf_len)/16);i++)
        {
            memset(pu8InData, 0, sizeof(pu8InData));
            memcpy(pu8InData, &pu8InParams[4+(i*16)], (u16InLen>16)?16:u16InLen);
            if (u16InLen > 16)
            {
                u16InLen -=16;
            }
            MDrv_DSCMB_KLadder_SetInput(0, pu8InData, sizeof(pu8InData));//CW
            MDrv_DSCMB_KLadder_SetInput(1, _u8DefaultKey, sizeof(_u8DefaultKey));
            MDrv_DSCMB_KLadder_SetInput(2, _u8DefaultKey, sizeof(_u8DefaultKey));

            KLConfig.bDecrypt = TRUE;
            KLConfig.bInverse = FALSE;
            KLConfig.bKeepRound = FALSE;
            KLConfig.u32Round = 6;
            KLConfig.u32EngMode = E_DSCMB_KL_TDES;

            MDrv_DSCMB_KLadder_SetConfig(&KLConfig);
            MDrv_DSCMB_KLadder_SetKey(E_DSCMB_KL_SRC_VGK, NULL, 0);
            MDrv_DSCMB_KLadder_SetDst(E_DSCMB_KL_DST_ACPU);

            MDrv_DSCMB_KLadder_Start();

            // (3) polling status
            while (1)
            {
                if (MDrv_DSCMB_KLadder_IsComplete())
                {
                    break;
                }
            }
            //Read key
            MDrv_DSCMB_KLadder_ReadDataFromAcpu(&out_data_buf[i*16]);
            //_verSC_PrintData(&out_data_buf[i*16], 16);

            //ACK --release key
            MDrv_DSCMB_KLadder_ResetAcpuAck();

            // (5) Poll key ladder done
            while (1)
            {
                if (MDrv_DSCMB_KLadder_Output_IsComplete())
                // if (kl_done == ((reg_kl_status & 0x1)))
                {
                    break;
                }
            }
            MDrv_DSCMB_KLadder_Output_Stop();

            // (6) Clear key ladder control
            MDrv_DSCMB_KLadder_Stop();
        }
    }
    return CA_OK;
}


