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

#include "MsCommon.h"
#include "halTCF.h"
#include "regTCF.h"

//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define _TCF_DBG(fmt, args...)  printf("\033[32m""[DBG]"fmt"\033[m", ## args)
#define _TCF_ERR(fmt, args...)  printf("\033[31m""[ERR]"fmt"\033[m", ## args)


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
static MS_VIRT              _u32NonPmBankAddr = NULL;
static REG_CFCtrl           *_CFCtrl  = (REG_CFCtrl*)REG_CF_BASE;
static REG_CFBCtrl          *_CFBCtrl = (REG_CFBCtrl*)REG_CFB_BASE;


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
void HAL_CF_SetBank(MS_VIRT u32NonPmBankAddr)
{
    _u32NonPmBankAddr = u32NonPmBankAddr;
    /// CF
    _CFCtrl  = (REG_CFCtrl*)  (_u32NonPmBankAddr + REG_CF_BASE);
    /// CFB
    _CFBCtrl = (REG_CFBCtrl*) (_u32NonPmBankAddr + REG_CFB_BASE);
}

void HAL_CF_Clk(MS_BOOL bEnable)
{
    REG32 *Reg32_Clk_CF = (REG32*)(_u32NonPmBankAddr + REG_CLK_CF_BASE + REG_CLK_CF);
    REG32 *Reg32_Clk_cftop_p_sel = (REG32*)(_u32NonPmBankAddr + REG_CLK_GEN0_BASE + REG_CLK_CFTOP_P_SEL);

    if(bEnable)
    {
        _TCF_DBG("[%s] Enable CF Clock!!\n", __FUNCTION__);
        _REG32_W( Reg32_Clk_CF, (_REG32_R( Reg32_Clk_CF)&~ENABLE_48_MHZ_CF_CLK_MASK) | ENABLE_48_MHZ_CF_CLK);
    }
    else
    {
        _TCF_DBG("[%s] Disable CF Clock!!\n", __FUNCTION__);
        _REG32_W( Reg32_Clk_CF, (_REG32_R( Reg32_Clk_CF)|ENABLE_48_MHZ_CF_CLK_MASK));
    }

#if 1//CLK_54M
    //Set CF top clock to 54MHz, for functional mode
    _REG32_W( Reg32_Clk_cftop_p_sel, (_REG32_R( Reg32_Clk_cftop_p_sel)&~ENABLE_CFTOP_P_SEL_MASK) | ENABLE_CFTOP_P_54M_CLK);
    _REG32_W( &_CFBCtrl[0].Cfb_TcfProductionMode, 0);
#else
    //Set CF top clock to 12MHz, for production mode
    _REG32_W( Reg32_Clk_cftop_p_sel, (_REG32_R( Reg32_Clk_cftop_p_sel)&~ENABLE_CFTOP_P_SEL_MASK) | ENABLE_CFTOP_P_12M_CLK);
    _REG32_W( &_CFBCtrl[0].Cfb_TcfProductionMode, 1);
#endif
    // TODO: TSP Clock checking
    //REG32 *Reg32_Clk_TSP= (REG32*)(_u32NonPmBankAddr + REG_CLK_TSP_BASE + REG_CLK_TSP);
    //_REG32_W( Reg32_Clk_TSP, (_REG32_R( Reg32_Clk_TSP)&~SET_192_MHZ_TSP_CLK_MASK) | SET_192_MHZ_TSP_CLK );  // change to 172
}

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
    _pstStatus->_u8UseNvmKey      = ((u32Value & CF_USE_NVMKEY_MASK) >> CF_USE_NVMKEY_POS);
    _pstStatus->_eOperationType  = ((u32Value & CF_OPERATION_TYPE_MASK) >> CF_OPERATION_TYPE_POS);
    _pstStatus->_eDecmSrc        = ((u32Value & CF_DECM_SOURCE_MASK) >> CF_DECM_SOURCE_POS);
    _pstStatus->_eOutputUsage    = ((u32Value & CF_OUTPUT_USAGE_MASK) >> CF_OUTPUT_USAGE_POS);
    _pstStatus->_u8ProductRange  = ((u32Value & CF_PRODUCT_RANGE_MASK) >> CF_PRODUCT_RANGE_POS);
    _pstStatus->_u8ProductOffset = ((u32Value & CF_PRODUCT_OFFSET_MASK) >> CF_PRODUCT_OFFSET_POS);
}

void HAL_CF_Cf_Status(HAL_CF_CF_STATUS *_pstCfStatus)
{
    MS_U32 u32Value= _REG32_R( &_CFCtrl[0].Cf_Status);
    u32Value= _REG32_R( &_CFCtrl[0].Cf_Status);
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
    if( HAL_CF_Get_Trans_Status()==_CF_TRANS_STATUS_BUSY_OP )
    {
        return FALSE;
    }
    else{
        return TRUE;
    }
}



MS_U8 HAL_CFB_Init(void)
{
    MS_U32 u32Value= 0;

    u32Value = _REG32_R( &_CFBCtrl[0].Cfb_Use_Case);
    u32Value = u32Value | ( REG_CFB_INT_CLR | REG_INITIAL_SM | REG_RESET_CFB );
    _TCF_DBG("[%s][%d] Reg_0[6:4]= b'%u%u%u\n", __FUNCTION__, __LINE__, (MS_U8)(u32Value&REG_CFB_INT_CLR)>>6, (MS_U8)(u32Value&REG_INITIAL_SM)>>5, (MS_U8)(u32Value&REG_RESET_CFB)>>4);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value);
    MsOS_DelayTask(10);
    u32Value = (u32Value | (REG_INITIAL_SM)) & (~REG_CFB_INT_CLR) & (~REG_RESET_CFB);
    _TCF_DBG("[%s][%d] Reg_0[6:4]= b'%u%u%u\n", __FUNCTION__, __LINE__, (MS_U8)(u32Value&REG_CFB_INT_CLR)>>6, (MS_U8)(u32Value&REG_INITIAL_SM)>>5, (MS_U8)(u32Value&REG_RESET_CFB)>>4);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value);

    return TRUE;
}
MS_U8 HAL_CFB_Enable(void)
{
    MS_U32 u32Value= 0;

    // REG_RESET_CFB= 0, REG_INITIAL_SM= 0
    u32Value = _REG32_R( &_CFBCtrl[0].Cfb_Use_Case);
    _TCF_DBG("[%s][%d] Reg_0[5:4]= b'%u%u\n", __FUNCTION__, __LINE__, (MS_U8)(u32Value&REG_INITIAL_SM)>>5, (MS_U8)(u32Value&REG_RESET_CFB)>>4);
    u32Value = u32Value & (~REG_RESET_CFB);
    u32Value = u32Value & (~REG_INITIAL_SM);
    _TCF_DBG("[%s][%d] Reg_0[5:4]= b'%u%u\n", __FUNCTION__, __LINE__, (MS_U8)(u32Value&REG_INITIAL_SM)>>5, (MS_U8)(u32Value&REG_RESET_CFB)>>4);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value);

    return TRUE;
}
MS_U8 HAL_CFB_Reset(void)
{
    MS_U32 u32Value= 0;

    u32Value = _REG32_R( &_CFBCtrl[0].Cfb_Use_Case);
    _TCF_DBG("[%s][%d] Reg_0[5:4]= b'%u%u\n", __FUNCTION__, __LINE__, (MS_U8)(u32Value&REG_INITIAL_SM)>>5, (MS_U8)(u32Value&REG_RESET_CFB)>>4);

    // Set REG_RESET_CFB= 1, REG_INITIAL_SM= 1
    u32Value = u32Value | (REG_RESET_CFB);
    u32Value = u32Value | (REG_INITIAL_SM);
    _TCF_DBG("[%s][%d] Reg_0[5:4]= b'%u%u\n", __FUNCTION__, __LINE__, (MS_U8)(u32Value&REG_INITIAL_SM)>>5, (MS_U8)(u32Value&REG_RESET_CFB)>>4);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value);

    // Set REG_RESET_CFB= 0, REG_INITIAL_SM= 1
    u32Value = u32Value & (~REG_RESET_CFB);
    u32Value = u32Value | (REG_INITIAL_SM);
    _TCF_DBG("[%s][%d] Reg_0[5:4]= b'%u%u\n", __FUNCTION__, __LINE__, (MS_U8)(u32Value&REG_INITIAL_SM)>>5, (MS_U8)(u32Value&REG_RESET_CFB)>>4);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value);

    return TRUE;
}

void HAL_CFB_Set_Use_Case(MS_U8 u8UseCase)
{
    MS_U32 u32Value= 0;

    u32Value = _REG32_R( &_CFBCtrl[0].Cfb_Use_Case);
    u32Value = (u32Value&(~REG_USE_CASE_MASK)) | (u8UseCase&REG_USE_CASE_MASK);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value );
}

void HAL_CFB_Set_Otp_Key(MS_U8 u8OtpKeyIdx)
{
    MS_U32 u32Value= 0;

    u32Value = _REG32_R( &_CFBCtrl[0].Cfb_Use_Case);
    u32Value = (u32Value&(~REG_TCF_KEY_SEL_MASK)) | ((u8OtpKeyIdx<<8)&REG_TCF_KEY_SEL_MASK);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value);
}

void HAL_CFB_Set_CACWC_Src(MS_U8 u8CAcwcSrc)
{
    MS_U32 u32Value= 0;

    u32Value = _REG32_R( &_CFBCtrl[0].Cfb_Use_Case);
    u32Value = (u32Value&(~REG_CFB_CACWC_SEL)) | ((u8CAcwcSrc<<16)&REG_CFB_CACWC_SEL);
    _REG32_W( &_CFBCtrl[0].Cfb_Use_Case, u32Value);
}

void HAL_CFB_SET_CACWC(MS_U32 *pCACWC)
{
    _REG32_W( &_CFBCtrl[0].Cfb_Cacwc0, pCACWC[3]);
    _REG32_W( &_CFBCtrl[0].Cfb_Cacwc1, pCACWC[2]);
    _REG32_W( &_CFBCtrl[0].Cfb_Cacwc2, pCACWC[1]);
    _REG32_W( &_CFBCtrl[0].Cfb_Cacwc3, pCACWC[0]);
}

void HAL_CFB_SET_EPK(MS_U32 *pEPK)
{
    _REG32_W( &_CFBCtrl[0].Cfb_Epk0, pEPK[3]);
    _REG32_W( &_CFBCtrl[0].Cfb_Epk1, pEPK[2]);
    _REG32_W( &_CFBCtrl[0].Cfb_Epk2, pEPK[1]);
    _REG32_W( &_CFBCtrl[0].Cfb_Epk3, pEPK[0]);
}

void HAL_CFB_SET_EFUV(MS_U32 *pEFUV)
{
    _REG32_W( &_CFBCtrl[0].Cfb_Efuv0, pEFUV[3]);
    _REG32_W( &_CFBCtrl[0].Cfb_Efuv1, pEFUV[2]);
    _REG32_W( &_CFBCtrl[0].Cfb_Efuv2, pEFUV[1]);
    _REG32_W( &_CFBCtrl[0].Cfb_Efuv3, pEFUV[0]);
}

MS_U16 HAL_CFB_GetCwc_Dst(MS_U8 eCwcDst)
{
    switch (eCwcDst)
    {
        case E_CFB_DST_KT:
            return E_HAL_CFB_DST_KT;
        case E_CFB_DST_DMA_SK0:
            return E_HAL_CFB_DST_DMA_SK0;
        case E_CFB_DST_DMA_SK1:
            return E_HAL_CFB_DST_DMA_SK1;
        case E_CFB_DST_DMA_SK2:
            return E_HAL_CFB_DST_DMA_SK2;
        case E_CFB_DST_DMA_SK3:
            return E_HAL_CFB_DST_DMA_SK3;
        case E_CFB_DST_TSIO:
            return E_HAL_CFB_DST_TSIO;
        default:
            return E_HAL_CFB_DST_KT;
    }
}

void HAL_CFB_Set_Cwc_Cfg(HAL_CFB_CWCCFG *pstCwcCfg)
{
    MS_U32 u32Value= 0;
    MS_U16 u16CwcDst = HAL_CFB_GetCwc_Dst(pstCwcCfg->_u8CwcDst);

    u32Value = _REG32_R( &_CFBCtrl[0].Cfb_Tcf_Key_Otp);

    // [31:24]: reg_tcf_cwc_pid (8)
    u32Value = (u32Value&(~REG_TCF_CWC_PID))  | ( ((pstCwcCfg->_u8CwcPid)<<24) & REG_TCF_CWC_PID );
    // [21:20]: reg_tcf_cwc_fld (2)
    u32Value = (u32Value&(~REG_TCF_CWC_FLD))  | ( ((pstCwcCfg->_u8CwcFld)<<20) & REG_TCF_CWC_FLD );
    // [19:18]: reg_tcf_cwc_fscb (2)
    u32Value = (u32Value&(~REG_TCF_CWC_FSCB)) | ( ((pstCwcCfg->_u8CwcFscb)<<18) & REG_TCF_CWC_FSCB );
    // [17:16]: reg_tcf_cwc_scb (2)
    u32Value = (u32Value&(~REG_TCF_CWC_SCB))  | ( ((pstCwcCfg->_u8CwcScb)<<16) & REG_TCF_CWC_SCB );
    // [11:8]: reg_tcf_cwc_tsid (4)
    u32Value = (u32Value&(~REG_TCF_CWC_TSID)) | ( ((pstCwcCfg->_u8CwcTsid)<<8) & REG_TCF_CWC_TSID );

    _REG32_W( &_CFBCtrl[0].Cfb_Tcf_Key_Otp, u32Value);
    _REG32_W( &_CFBCtrl[0].Cfb_Key_Dst, u16CwcDst);
}

MS_BOOL HAL_CFB_Is_Ready(void)
{
    if( (_REG32_R( &_CFBCtrl[0].Cfb_Status)&REG_CFB_READY) == REG_CFB_READY ) // ready
    {
        return TRUE;
    }
    else
    {
        printf(">>>\tNOT Ready!!\n");
        return FALSE;
    }
}

MS_BOOL HAL_CFB_Is_Done(void)
{
    if( (_REG32_R( &_CFBCtrl[0].Cfb_Status)&REG_CFB_DONE) == REG_CFB_DONE ) // done
    {
        return TRUE;
    }
    else
    {
        //printf(">>>\tNOT Done!!\n");
        return FALSE;
    }
}

//////

MS_BOOL HAL_CFB_DBG_KT_Response(void)
{
    MS_U8 u8Val = _REG32_R( &_CFBCtrl[0].Cfb_Status);

    printf("\033[32m""\n[%s][DBG] control word has been transtered to key table:\n""\033[m", __FUNCTION__);
    if( (u8Val&REG_TCF_CWC_WR_DONE_LTH)>>6 )
        printf("\tTRUE\n");
    else
        printf("\tFalse!\n");

    printf("\033[32m""[%s][DBG] key table response:\n""\033[m", __FUNCTION__);
    switch( (u8Val&REG_TCF_CWC_WR_RESP_LTH)>>8 )
    {
        case 0x1F:
            printf("\tno_slot (5'b11111)\n");
            break;
        case 0x19:
            printf("\tno_allow_wkey (5'b11001)\n");
            break;
        case 0x02:
            printf("\tis_bad_key (2'b10)\n");
            break;
        case 0x00:
            printf("\tok (5'b0000)\n");
            break;
        default:
            break;
    }

    return TRUE;
}

MS_U8 HAL_CFB_DBG_CFB_FSM(void)
{
    MS_U8 u8Val = (_REG32_R( &_CFBCtrl[0].Cfb_State))&REG_CF_STATE;
    printf("\033[32m""\n[%s][DBG] cfb FSM: 0x%X\n""\033[m", __FUNCTION__, u8Val);

    return u8Val;
}
