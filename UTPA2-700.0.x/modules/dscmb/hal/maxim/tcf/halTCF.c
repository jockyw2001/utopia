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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halTCF.c
// @brief  TCF HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "MsCommon.h"
#include "halTCF.h"
#include "regTCF.h"


//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define _TCF_DBG(fmt, args...)  //printf("\033[32m""[DBG]"fmt"\033[m", ## args)
#define _TCF_ERR(fmt, args...)  printf("\033[31m""[ERR]"fmt"\033[m", ## args)


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
static MS_U32                _u32NonPmBankAddr = 0;
static REG_CFCtrl           *_CFCtrl      = (REG_CFCtrl*)CF_BASE;
static REG_CFKECtrl         *_CFKECtrl    = (REG_CFKECtrl*)CFKE_BASE;
static REG_CRIKLCtrl        *_CRIKLCtrl   = (REG_CRIKLCtrl*)CRI_KL_BASE;

static MS_U16               u16KeyPara[4] = {0}; // Key Parameter-- shared by DSCMB Key, AESDMA Key


//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
void _REG32_W(REG32 *reg, MS_U32 value)
{
    (*((volatile MS_U32*)(reg))) = value;
}

MS_U32 _REG32_R(REG32 *reg)
{
    MS_U32 value = (*(volatile MS_U32*)(reg));
    return value;
}


//--------------------------------------------------------------------------------------------------
//  Inline Function
//--------------------------------------------------------------------------------------------------
void HAL_TCF_SetBank(MS_U32 u32NonPmBankAddr)
{
    _u32NonPmBankAddr = u32NonPmBankAddr;
    /// CF
    _CFCtrl   = (REG_CFCtrl*) (_u32NonPmBankAddr + CF_BASE);
    /// CFKE
    _CFKECtrl = (REG_CFKECtrl*) (_u32NonPmBankAddr + CFKE_BASE);

    /// CRIKL
    _CRIKLCtrl = (REG_CRIKLCtrl*) (_u32NonPmBankAddr + CRI_KL_BASE);
}

void HAL_TCF_Clk(MS_BOOL bEnable)
{
    REG32 *Reg32_Clk_CF = (REG32*)(_u32NonPmBankAddr + CLKGEN0_BASE + REG_CLK_TCF_SEL);

    if(bEnable){
        _TCF_DBG("[%s] Enable CF Clock!!\n", __FUNCTION__);
        _REG32_W( Reg32_Clk_CF, (_REG32_R( Reg32_Clk_CF)|TCF_CLK_54_MHZ));
    }
    else{
        _TCF_DBG("[%s] Disable CF Clock!!\n", __FUNCTION__);
        _REG32_W( Reg32_Clk_CF, (_REG32_R( Reg32_Clk_CF)&~TCF_CLK_54_MHZ));
    }
}


// CF
//
void HAL_CF_Version_Info(HAL_CF_VERSION_INFO *_pstTransStatus)
{
    MS_U32 u32Value= _REG32_R( &_CFCtrl[0].Cf_Version);

    _pstTransStatus->_u8ManufacturerId = ((u32Value & CF_VERSION_MANUFACTURER_ID_MASK) >> CF_VERSION_MANUFACTURER_ID_POS);
    _pstTransStatus->_u8NetlistVersion = ((u32Value & CF_VERSION_NETLIST_VERSION_MASK) >> CF_VERSION_NETLIST_VERSION_POS);
    _pstTransStatus->_u8VersionEpoch   = ((u32Value & CF_VERSION_VERSION_EPOCH_MASK) >> CF_VERSION_VERSION_EPOCH_POS);
    _pstTransStatus->_u8VersionBuildId = ((u32Value & CF_VERSION_BUILD_ID_MASK) >> CF_VERSION_BUILD_ID_POS);
}

void HAL_CF_Trans_Status(HAL_CF_TRANS_STATUS *_pstStatus)
{
    MS_U32 u32Value= _REG32_R( &_CFCtrl[0].Cf_Status);

    _pstStatus->_eTransStatus    = ((u32Value & CF_TRANS_STATUS_MASK) >> CF_TRANS_STATUS_POS);
    _pstStatus->_u8UseNvmKey     = ((u32Value & CF_USE_NVMKEY_MASK) >> CF_USE_NVMKEY_POS);
    _pstStatus->_eOperationType  = ((u32Value & CF_OPERATION_TYPE_MASK) >> CF_OPERATION_TYPE_POS);
    _pstStatus->_eDecmSrc        = ((u32Value & CF_DECM_SOURCE_MASK) >> CF_DECM_SOURCE_POS);
    _pstStatus->_eOutputUsage    = ((u32Value & CF_OUTPUT_USAGE_MASK) >> CF_OUTPUT_USAGE_POS);
    _pstStatus->_u8ProductRange  = ((u32Value & CF_PRODUCT_RANGE_MASK) >> CF_PRODUCT_RANGE_POS);
    _pstStatus->_u8ProductOffset = ((u32Value & CF_PRODUCT_OFFSET_MASK) >> CF_PRODUCT_OFFSET_POS);
}

void HAL_CF_Cf_Status(HAL_CF_CF_STATUS *_pstCfStatus)
{
    MS_U32 u32Value= _REG32_R( &_CFCtrl[0].Cf_Status);
    _pstCfStatus->_eNvmStatus        = ((u32Value & CF_NVM_STATUS_MASK) >> CF_NVM_STATUS_POS);
    _pstCfStatus->_eDiffStatus       = ((u32Value & CF_DIFF_STATUS_MASK) >> CF_DIFF_STATUS_POS);

    u32Value= _REG32_R( &_CFCtrl[0].Cf_Platform);
    if( ((_REG32_R( &_CFCtrl[0].Cf_Version) & CF_VERSION_NETLIST_VERSION_MASK) >> CF_VERSION_NETLIST_VERSION_POS) == 0 ) // Netlist Version 0
    {
        _pstCfStatus->_u8RecentReset     = ((u32Value & CF_PLATFORM_RECENT_RESET_MASK) >> CF_PLATFORM_RECENT_RESET_POS);
        _pstCfStatus->_u8CfAlert         = ((u32Value & CF_PLATFORM_CF_ALERT_MASK) >> CF_PLATFORM_CF_ALERT_POS);
        // Netlist Version 0 has no development mode bit.
        _pstCfStatus->_u8DevelopmentMode = 0x0;
        // Netlist Version 0 has a different Fuse Activate and Fuse Block positions.
        _pstCfStatus->_u8FuseActivate    = ((u32Value & 0x00200000) >> 21);
        _pstCfStatus->_u8FuseBlock       = ((u32Value & 0x00100000) >> 20);
    }
    else
    {
        _pstCfStatus->_u8RecentReset     = ((u32Value & CF_PLATFORM_RECENT_RESET_MASK) >> CF_PLATFORM_RECENT_RESET_POS);
        _pstCfStatus->_u8CfAlert         = ((u32Value & CF_PLATFORM_CF_ALERT_MASK) >> CF_PLATFORM_CF_ALERT_POS);
        _pstCfStatus->_u8DevelopmentMode = ((u32Value & CF_PLATFORM_DEVELOPMENT_MODE_MASK) >> CF_PLATFORM_DEVELOPMENT_MODE_POS);
        _pstCfStatus->_u8FuseActivate    = ((u32Value & CF_PLATFORM_CF_ACTIVATED_MASK) >> CF_PLATFORM_CF_ACTIVATED_POS);
        _pstCfStatus->_u8FuseBlock       = ((u32Value & 0x00000080) >> 7);
    }
}

void HAL_CF_Cf_FeatureVector(HAL_CF_FEATURE_VECTOR *_pstCfFeature)
{
    _pstCfFeature->_u32FeatureVector= _REG32_R( &_CFCtrl[0].Cf_Feature);
}

MS_U8 HAL_CF_Get_Trans_Status(void)
{
    MS_U32 u32Value= _REG32_R( &_CFCtrl[0].Cf_Status);
	//_TCF_DBG("[%s] ===  Trans_Status= 0x%08lx  ===\n", __FUNCTION__, u32Value);
    return (MS_U8)((u32Value & CF_TRANS_STATUS_MASK) >> CF_TRANS_STATUS_POS);
}

MS_U8 HAL_CF_Get_Operation_Type(void)
{
    MS_U32 u32Value= _REG32_R( &_CFCtrl[0].Cf_Status);

    return (MS_U8)((u32Value & CF_OPERATION_TYPE_MASK) >> CF_OPERATION_TYPE_POS);
}

MS_U8 HAL_CF_Get_CwcValid(void)
{
    MS_U32 u32Value= _REG32_R( &_CFCtrl[0].Cf_Platform);

    return (MS_U8)((u32Value & CF_PLATFORM_CWC_VALID_MASK) >> CF_PLATFORM_CWC_VALID_POS);
}

MS_U32 HAL_CF_Read_Input(void)
{
    return _REG32_R( &_CFCtrl[0].Cf_Input);
}

MS_U32 HAL_CF_Read_Output(void)
{
    return _REG32_R( &_CFCtrl[0].Cf_Output);
}

void HAL_CF_Write_Input(MS_U32 u32Cmd)
{
    _REG32_W( &_CFCtrl[0].Cf_Input, u32Cmd);
}

void HAL_CF_Write_Output(MS_U32 u32Cmd)
{
    _REG32_W( &_CFCtrl[0].Cf_Output, u32Cmd);
}

#define _CF_TRANS_STATUS_BUSY_OP    0x4
MS_U8 HAL_CF_Get_CF_IsFinished(void)
{
    if( HAL_CF_Get_Trans_Status()==_CF_TRANS_STATUS_BUSY_OP ){
        return FALSE;
    }
    else{
        return TRUE;
    }
}


// CFKE
//
MS_U8 HAL_CFKE_Get_OperationStatus(void)
{
    // CFKE_STATUS [31:28]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Status);

    return (MS_U8)((u32Value & CFKE_OPERATION_STATUS_MASK) >> CFKE_OPERATION_STATUS_POS);
}
MS_U8 HAL_CFKE_Get_OperationCount(void)
{
    // CFKE_STATUS [27:20]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Status);

    return (MS_U8)((u32Value & CFKE_OPERATION_COUNT_MASK) >> CFKE_OPERATION_COUNT_POS);
}
MS_U8 HAL_CFKE_Get_ErrorCode(void)
{
    // CFKE_STATUS [19:16]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Status);

    return (MS_U8)((u32Value & CFKE_ERROR_CODE_MASK) >> CFKE_ERROR_CODE_POS);
}
MS_U8 HAL_CFKE_Get_OperationType(void)
{
    // CFKE_STATUS [15:12]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Status);

    return (MS_U8)((u32Value & CFKE_OPERATION_TYPE_MASK) >> CFKE_OPERATION_TYPE_POS);
}
MS_U8 HAL_CFKE_Get_DiffAllowNonUnw(void)
{
    // CFKE_STATUS [3]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Status);

    return (MS_U8)((u32Value & CFKE_DIFF_ALLOW_NON_UNW_MASK) >> CFKE_DIFF_ALLOW_NON_UNW_POS);
}
MS_U8 HAL_CFKE_Get_DiffAllowCk(void)
{
    // CFKE_STATUS [2]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Status);

    return (MS_U8)((u32Value & CFKE_DIFF_ALLOW_CK_MASK) >> CFKE_DIFF_ALLOW_CK_POS);
}
MS_U8 HAL_CFKE_Get_DiffContentOutputAllowed(void)
{
    // CFKE_STATUS [1:0]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Status);

    return (MS_U8)((u32Value & CFKE_DIFF_CONTENT_OUTPUT_ALLOWED_MASK) >> CFKE_DIFF_CONTENT_OUTPUT_ALLOWED_POS);
}


MS_U32 HAL_CFKE_Read_Command(void)
{
    return _REG32_R( &_CFKECtrl[0].Cfke_Command);
}
void HAL_CFKE_Write_Command(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Command, u32Cmd);
}


MS_U32 HAL_CFKE_Read_Platform(void)
{
    return _REG32_R( &_CFKECtrl[0].Cfke_Platform);
}

MS_BOOL HAL_CFKE_IsTransactionDone(void)
{
    // CFKE_PLATFORM [30]
    MS_U32 u32Value= _REG32_R( &_CFKECtrl[0].Cfke_Platform);

    return (MS_U8)((u32Value & CFKE_TRANSACTION_DONE_MASK) >> CFKE_TRANSACTION_DONE_POS);
}


MS_U32 HAL_CFKE_Read_UnwPath(void)
{
    return _REG32_R( &_CFKECtrl[0].Cfke_Unw_Path);
}
void HAL_CFKE_Write_UnwPath(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Unw_Path, u32Cmd);
}


MS_U32 HAL_CFKE_Read_Validator0(void)
{
    return _REG32_R( &_CFKECtrl[0].Cfke_Validator_0);
}
void HAL_CFKE_Write_Validator0(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Validator_0, u32Cmd);
}


MS_U32 HAL_CFKE_Read_Validator1(void)
{
    return _REG32_R( &_CFKECtrl[0].Cfke_Validator_1);
}
void HAL_CFKE_Write_Validator1(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Validator_1, u32Cmd);
}


void HAL_CFKE_Write_SlotA(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Slot_A, u32Cmd);
}
void HAL_CFKE_Write_SlotB(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Slot_B, u32Cmd);
}
void HAL_CFKE_Write_SlotC(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Slot_C, u32Cmd);
}
void HAL_CFKE_Write_SlotD(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_Slot_D, u32Cmd);
}

void HAL_CFKE_Write_UserHash(MS_U32 u32Cmd)
{
    _TCF_DBG("[%s][%d] u32Cmd= 0x%08lx\n", __FUNCTION__, __LINE__, u32Cmd);
    _REG32_W( &_CFKECtrl[0].Cfke_User_Hash, u32Cmd);
}


// CRIKL
//
MS_U32 HAL_CRIKL_Read_CfStatus(void)
{
    MS_U32 u32Val = 0;

    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cfstatusout_h)&0xffff)<<16;
    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cfstatusout_l)&0xffff);
    return u32Val;
}
MS_U32 HAL_CRIKL_Read_CfkeStatus(void)
{
    MS_U32 u32Val = 0;

    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cfkestatusout_h)&0xffff)<<16;
    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cfkestatusout_l)&0xffff);
    return u32Val;
}
MS_U32 HAL_CRIKL_Read_CfPlatform(void)
{
    MS_U32 u32Val = 0;

    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cfplatformout_h)&0xffff)<<16;
    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cfplatformout_l)&0xffff);
    return u32Val;
}
MS_U32 HAL_CRIKL_Read_CfFeature(void)
{
    MS_U32 u32Val = 0;

    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cffeatureout_h)&0xffff)<<16;
    u32Val |= (_REG32_R( &_CRIKLCtrl[0].reg_cffeatureout_l)&0xffff);
    return u32Val;
}

void HAL_CRIKL_Set_KTIndex( MS_U8 eScmbCtl, MS_U8 u8DscmbId)
{
    _TCF_DBG("[%s][%d] u16KtIdx= 0x%1x%1x\n", __FUNCTION__, __LINE__, eScmbCtl, u8DscmbId);

    MS_U16 u16KtIdx = 0;
    u16KtIdx |= eScmbCtl << CRI_KT_SCMB_CTL_POS;
    u16KtIdx |= u8DscmbId;
    _REG32_W( &_CRIKLCtrl[0].reg_cri_kt_index, (MS_U32)u16KtIdx);
}
void HAL_CRIKL_Write_KTPara_High_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_kt_para_63_48, (MS_U32)u16HalfWord);
}
void HAL_CRIKL_Write_KTPara_2nd_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_kt_para_47_32, (MS_U32)u16HalfWord);
}
void HAL_CRIKL_Write_KTPara_3rd_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_kt_para_31_16, (MS_U32)u16HalfWord);
}
void HAL_CRIKL_Write_KTPara_Low_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_kt_para_15_00, (MS_U32)u16HalfWord);
}

MS_U16 HAL_CRIKL_Read_KTIndex(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_kt_index))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KTPara_High_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_kt_para_63_48))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KTPara_2nd_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_kt_para_47_32))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KTPara_3rd_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_kt_para_31_16))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KTPara_Low_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_kt_para_15_00))&0xFFFF;
}



void HAL_CRIKL_Set_KSIndex(MS_U8 u8KsIdx)
{
    _TCF_DBG("[%s][%d] u16KsIdx= 0x%02x\n", __FUNCTION__, __LINE__, u8KsIdx);

    MS_U16 u16KsIdx = 0;
    u16KsIdx |= (u8KsIdx&CRI_KS_IDX_VAL_MASK);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_ks_index, (MS_U32)u16KsIdx);
}
void HAL_CRIKL_Write_KSPara_High_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_ks_para_63_48, (MS_U32)u16HalfWord);
}
void HAL_CRIKL_Write_KSPara_2nd_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_ks_para_47_32, (MS_U32)u16HalfWord);
}
void HAL_CRIKL_Write_KSPara_3rd_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_ks_para_31_16, (MS_U32)u16HalfWord);
}
void HAL_CRIKL_Write_KSPara_Low_HalfWord(MS_U16 u16HalfWord)
{
    //_TCF_DBG("[%s][%d] u16HalfWord= 0x%08x\n", __FUNCTION__, __LINE__, u16HalfWord);
    _REG32_W( &_CRIKLCtrl[0].reg_cri_ks_para_15_00, (MS_U32)u16HalfWord);
}

MS_U16 HAL_CRIKL_Read_KSIndex(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_ks_index))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KSPara_High_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_ks_para_63_48))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KSPara_2nd_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_ks_para_47_32))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KSPara_3rd_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_ks_para_31_16))&0xFFFF;
}
MS_U16 HAL_CRIKL_Read_KSPara_Low_HalfWord(void)
{
    return (_REG32_R( &_CRIKLCtrl[0].reg_cri_ks_para_15_00))&0xFFFF;
}

void HAL_CRIKL_Reset_KeyPara(void)
{
    memset( (void*)u16KeyPara, 0x0, sizeof(u16KeyPara));
}
void HAL_CRIKL_Write_KeyPara(MS_BOOL bDscmbKey)
{
    _TCF_DBG("[%s][%d] Setup %s Parameter\n", __FUNCTION__, __LINE__, (bDscmbKey)? "Key Table":"Key Slot");
    _TCF_DBG("[%s][%d] >>> 0x%04X\n", __FUNCTION__, __LINE__, u16KeyPara[3]);
    _TCF_DBG("[%s][%d] >>> 0x%04X\n", __FUNCTION__, __LINE__, u16KeyPara[2]);
    _TCF_DBG("[%s][%d] >>> 0x%04X\n", __FUNCTION__, __LINE__, u16KeyPara[1]);
    _TCF_DBG("[%s][%d] >>> 0x%04X\n", __FUNCTION__, __LINE__, u16KeyPara[0]);

    if(bDscmbKey){
        HAL_CRIKL_Write_KTPara_High_HalfWord( u16KeyPara[3]);
        HAL_CRIKL_Write_KTPara_2nd_HalfWord( u16KeyPara[2]);
        HAL_CRIKL_Write_KTPara_3rd_HalfWord( u16KeyPara[1]);
        HAL_CRIKL_Write_KTPara_Low_HalfWord( u16KeyPara[0]);
    }
    else{
        HAL_CRIKL_Write_KSPara_High_HalfWord( u16KeyPara[3]);
        HAL_CRIKL_Write_KSPara_2nd_HalfWord( u16KeyPara[2]);
        HAL_CRIKL_Write_KSPara_3rd_HalfWord( u16KeyPara[1]);
        HAL_CRIKL_Write_KSPara_Low_HalfWord( u16KeyPara[0]);
    }
}

void HAL_CRIKL_Setup_VendorNo(MS_BOOL bDscmbKey, MS_U8 u8VendorNo)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[3] |= u8VendorNo << CRI_KT_VENDOR_NO_POS;
    }
    else{ // Key Slot
        u16KeyPara[3] |= u8VendorNo << CRI_KS_VENDOR_NO_POS;
    }
}
void HAL_CRIKL_Setup_VendorAtt(MS_BOOL bDscmbKey, MS_U16 u16VendorAtt)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[3] |= ((u16VendorAtt&CRI_KT_VENDOR_ATT_H_VAL_MASK)>>CRI_KT_VENDOR_ATT_L_OFFSET) << CRI_KT_VENDOR_ATT_H_POS;
        u16KeyPara[2] |= (u16VendorAtt&CRI_KT_VENDOR_ATT_L_VAL_MASK) << CRI_KT_VENDOR_ATT_L_POS;
    }
    else{ // Key Slot
        u16KeyPara[3] |= ((u16VendorAtt&CRI_KS_VENDOR_ATT_H_VAL_MASK)>>CRI_KS_VENDOR_ATT_L_OFFSET) << CRI_KS_VENDOR_ATT_H_POS;
        u16KeyPara[2] |= (u16VendorAtt&CRI_KS_VENDOR_ATT_L_VAL_MASK) << CRI_KS_VENDOR_ATT_L_POS;
    }
}
void HAL_CRIKL_Setup_OutputAllowed(MS_BOOL bDscmbKey, MS_U8 u8OutputAllowed)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[2] |= u8OutputAllowed << CRI_KT_OUTPUT_ALLOWED_POS;
    }
    else{ // Key Slot
        u16KeyPara[2] |= u8OutputAllowed << CRI_KS_OUTPUT_ALLOWED_POS;
    }
}

void HAL_CRIKL_Setup_bMultiEng(MS_BOOL bDscmbKey, MS_BOOL bMultiEng)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[2] |= ((bMultiEng)? 1:0) << CRI_KT_MULTI_ENG_POS;
    }
    else{ // Key Slot
        u16KeyPara[2] |= ((bMultiEng)? 1:0) << CRI_KS_MULTI_ENG_POS;
    }
}
void HAL_CRIKL_Setup_bLsb64(MS_BOOL bDscmbKey, MS_BOOL bLsb64)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[2] |= ((bLsb64)? 1:0) << CRI_KT_LSB64_POS;
    }
    else{ // Key Slot
        u16KeyPara[2] |= ((bLsb64)? 1:0) << CRI_KS_LSB64_POS;
    }
}
void HAL_CRIKL_Setup_KeySize(MS_BOOL bDscmbKey, MS_U8 eKeySize)
{
    MS_U8  u8Pos = 0;

    if(bDscmbKey){ // Key Table
        switch(eKeySize){
            case 0: u8Pos = CRI_KT_KEY_SIZE_64_POS;  break;
            case 1: u8Pos = CRI_KT_KEY_SIZE_128_POS; break;
            case 2: u8Pos = CRI_KT_KEY_SIZE_192_POS; break;
            case 3: u8Pos = CRI_KT_KEY_SIZE_256_POS; break;
            default: break;
        }
    }
    else{ // Key Slot
        switch(eKeySize){
            case 0: u8Pos = CRI_KS_KEY_SIZE_64_POS;  break;
            case 1: u8Pos = CRI_KS_KEY_SIZE_128_POS; break;
            case 2: u8Pos = CRI_KS_KEY_SIZE_192_POS; break;
            case 3: u8Pos = CRI_KS_KEY_SIZE_256_POS; break;
            default: break;
        }
    }

    u16KeyPara[2] |= 1 << u8Pos;
}
void HAL_CRIKL_Setup_KeyEngine(MS_BOOL bDscmbKey, MS_U8 u8KeyEng)
{
    if(bDscmbKey){ // Key Table
        if( /*CRIKL_KL_ENG_NPD*/0x80&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KT_ENG_NPD_POS;}
        if( /*CRIKL_KL_ENG_MULTI2*/0x40&u8KeyEng ) { u16KeyPara[1] |= 1 << CRI_KT_ENG_MULTI2_POS;}
        if( /*CRIKL_KL_ENG_CSAV2*/0x20&u8KeyEng )  { u16KeyPara[1] |= 1 << CRI_KT_ENG_CSAV2_POS;}
        if( /*CRIKL_KL_ENG_CSAV3*/0x10&u8KeyEng )  { u16KeyPara[1] |= 1 << CRI_KT_ENG_CSAV3_POS;}
        if( /*CRIKL_KL_ENG_ASA*/0x08&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KT_ENG_ASA_POS;}
        if( /*CRIKL_KL_ENG_DES*/0x04&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KT_ENG_DES_POS;}
        if( /*CRIKL_KL_ENG_TDES*/0x02&u8KeyEng )   { u16KeyPara[1] |= 1 << CRI_KT_ENG_TDES_POS;}
        if( /*CRIKL_KL_ENG_AES*/0x01&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KT_ENG_AES_POS;}
    }
    else{ // Key Slot
        if( /*CRIKL_KL_ENG_NPD*/0x80&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KS_ENG_NPD_POS;}
        if( /*CRIKL_KL_ENG_MULTI2*/0x40&u8KeyEng ) { u16KeyPara[1] |= 1 << CRI_KS_ENG_MULTI2_POS;}
        if( /*CRIKL_KL_ENG_CSAV2*/0x20&u8KeyEng )  { u16KeyPara[1] |= 1 << CRI_KS_ENG_CSAV2_POS;}
        if( /*CRIKL_KL_ENG_CSAV3*/0x10&u8KeyEng )  { u16KeyPara[1] |= 1 << CRI_KS_ENG_CSAV3_POS;}
        if( /*CRIKL_KL_ENG_ASA*/0x08&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KS_ENG_ASA_POS;}
        if( /*CRIKL_KL_ENG_DES*/0x04&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KS_ENG_DES_POS;}
        if( /*CRIKL_KL_ENG_TDES*/0x02&u8KeyEng )   { u16KeyPara[1] |= 1 << CRI_KS_ENG_TDES_POS;}
        if( /*CRIKL_KL_ENG_AES*/0x01&u8KeyEng )    { u16KeyPara[1] |= 1 << CRI_KS_ENG_AES_POS;}
    }
}
void HAL_CRIKL_Setup_Drop(MS_BOOL bDscmbKey, MS_U8 u8Drop)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[1] |= u8Drop << CRI_KT_DROP_POS;
    }
    else{ // Key Slot
        u16KeyPara[1] |= u8Drop << CRI_KS_DROP_POS;
    }
}
void HAL_CRIKL_Setup_Cnt(MS_BOOL bDscmbKey, MS_U8 u8Cnt)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[1] |= u8Cnt << CRI_KT_CNT_POS;
    }
    else{ // Key Slot
        u16KeyPara[1] |= u8Cnt << CRI_KS_CNT_POS;
    }
}
void HAL_CRIKL_Setup_KeyDest(MS_BOOL bDscmbKey, MS_U8 u8KeyDest)
{
    if(bDscmbKey){ // Key Table
        if( /*CRIKL_KL_DEST_AESDMA*/0x4&u8KeyDest ) { u16KeyPara[0] |= 1 << CRI_KT_DEST_AESDMA_POS;}
        if( /*CRIKL_KL_DEST_PVR*/0x2&u8KeyDest )    { u16KeyPara[0] |= 1 << CRI_KT_DEST_PVR_POS;}
        if( /*CRIKL_KL_DEST_CIPHER*/0x1&u8KeyDest ) { u16KeyPara[0] |= 1 << CRI_KT_DEST_CIPHER_POS;}
    }
    else{ // Key Slot
        if( /*CRIKL_KL_DEST_AESDMA*/0x4&u8KeyDest ) { u16KeyPara[0] |= 1 << CRI_KS_DEST_AESDMA_POS;}
        if( /*CRIKL_KL_DEST_PVR*/0x2&u8KeyDest )    { u16KeyPara[0] |= 1 << CRI_KS_DEST_PVR_POS;}
        if( /*CRIKL_KL_DEST_CIPHER*/0x1&u8KeyDest ) { u16KeyPara[0] |= 1 << CRI_KS_DEST_CIPHER_POS;}
    }
}
void HAL_CRIKL_Setup_OpType(MS_BOOL bDscmbKey, MS_U8 u8Op)
{
    if(bDscmbKey){ // Key Table
        switch(u8Op){
            case 0: break;
            case 1: u16KeyPara[0] |= 1 << CRI_KT_DECRYPT_POS; break;
            case 2: u16KeyPara[0] |= 1 << CRI_KT_ENCRYPT_POS; break;
            default: break;
        }
    }
    else{ // Key Slot
        switch(u8Op){
            case 0: break;
            case 1: u16KeyPara[0] |= 1 << CRI_KS_DECRYPT_POS; break;
            case 2: u16KeyPara[0] |= 1 << CRI_KS_ENCRYPT_POS; break;
            default: break;
        }
    }
}
void HAL_CRIKL_Setup_KteSel(MS_BOOL bDscmbKey, MS_U8 eKteSel)
{
    MS_U8  u8Pos = 0;

    if(bDscmbKey){ // Key Table
        switch(eKteSel){
            case 0: u8Pos = CRI_KT_KTE_0; break;
            case 1: u8Pos = CRI_KT_KTE_1; break;
            case 2: u8Pos = CRI_KT_KTE_2; break;
            case 3: u8Pos = CRI_KT_KTE_3; break;
            default: break;
        }
    }
    else{ // Key Slot
        switch(eKteSel){
            case 0: u8Pos = CRI_KS_KTE_0; break;
            case 1: u8Pos = CRI_KS_KTE_1; break;
            case 2: u8Pos = CRI_KS_KTE_2; break;
            case 3: u8Pos = CRI_KS_KTE_3; break;
            default: break;
        }
    }

    u16KeyPara[0] |= 1 << u8Pos;
}
void HAL_CRIKL_Setup_bLock(MS_BOOL bDscmbKey, MS_BOOL bLock)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[0] |= ((bLock)? 1:0) << CRI_KT_LOCK;
    }
    else{ // Key Slot
        u16KeyPara[0] |= ((bLock)? 1:0) << CRI_KS_LOCK;
    }
}
void HAL_CRIKL_Setup_bParaValid(MS_BOOL bDscmbKey, MS_BOOL bParaValid)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[0] |= ((bParaValid)? 1:0) << CRI_KT_PARA_VLA;
    }
    else{ // Key Slot
        u16KeyPara[0] |= ((bParaValid)? 1:0) << CRI_KS_PARA_VLA;
    }
}
void HAL_CRIKL_Setup_bKlEn(MS_BOOL bDscmbKey, MS_BOOL bKlEn)
{
    if(bDscmbKey){ // Key Table
        u16KeyPara[0] |= ((bKlEn)? 1:0) << CRI_KT_KL_EN;
    }
    else{ // Key Slot
        u16KeyPara[0] |= ((bKlEn)? 1:0) << CRI_KS_KL_EN;
    }
}

