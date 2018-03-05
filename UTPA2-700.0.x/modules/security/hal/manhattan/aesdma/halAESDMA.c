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
// file   halAESDMA.c
// @brief  AESDMA HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "regAESDMA.h"
#include "halAESDMA.h"
#include "halCHIP.h"


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT                _u32RegBase = NULL;
static MS_VIRT                _u32PmRegBase = NULL;
static MS_U32                 SECUREBASECtrl_Buffer;
static REG_AESDMACtrl        *_AESDMACtrl = (REG_AESDMACtrl*)REG_AESDMACTRL_BASE;
static REG_SHARNGCtrl        *_SHARNGCtrl = (REG_SHARNGCtrl*)REG_SHARNGCTRL_BASE;
static REG_AESDMAClk         *_AESDMAClk  = (REG_AESDMAClk*)REG_AESDMACLK_BASE;
static REG_AESDMAMbx         *_AESDMAMbx  = (REG_AESDMAMbx*)REG_AESDMAMBX_BASE;
static REG_PARSERCtrl        *_PARSERCtrl = (REG_PARSERCtrl*)REG_PARSERCTRL_BASE;
static REG_CIPHERCtrl        *_CIPHERCtrl = (REG_CIPHERCtrl*)REG_CIPHERCTRL_BASE;
static REG_DMASECURECtrl     *_DMASECURECtrl = (REG_DMASECURECtrl*)REG_DMASECURE_BASE;
static REG_SECUREBASECtrl    *_SECUREBASECtrl = (REG_SECUREBASECtrl*)REG_DMASECURE_CTRL_BASE;
static REG_AESDMACtrlEx      *_AESDMACtrlEx = (REG_AESDMACtrlEx*)REG_AESDMAEXT_BASE;
static REG_TZPCCtrl          *_TZPCCtrl = (REG_TZPCCtrl*)REG_TZPC_NONPM_BASE;
static REG_HDCP22Data        *_HDCP22Data = (REG_HDCP22Data*)REG_HDCP22_BASE;
static REG_HDCP22SEKCtrl     *_HDCP22SekCtrl = (REG_HDCP22SEKCtrl*)REG_HDCP22_SEK;
static REG_HDCP14SEKCtrl     *_HDCP14SekCtrl = (REG_HDCP14SEKCtrl*)REG_HDCP14_BASE;
static REG_ANAMISCBASECtrl   *_ANAMISCCtrl = (REG_ANAMISCBASECtrl*)REG_ANA_MISC_BASE;
static REG_AES_DSCMB6Ctrl    *_AESDSCMB6Ctrl = (REG_AES_DSCMB6Ctrl*)REG_AES_DSCMB6_BASE;
static REG_AES_DSCMB7Ctrl    *_AESDSCMB7Ctrl = (REG_AES_DSCMB7Ctrl*)REG_AES_DSCMB7_BASE;

#define REG32_W(reg, value)     do {    \
                                    (reg)->H = ((value) >> 16); \
                                    (reg)->L = ((value) & 0x0000FFFF);  \
                                } while(0)

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

MS_U32 _AESDMA_REG32_R(REG32 *reg)
{
    MS_U32     value;

    value = (reg)->H << 16;
    value |= (reg)->L;

    return value;
}

//--------------------------------------------------------------------------------------------------
//  Inline Function
//--------------------------------------------------------------------------------------------------
void HAL_AESDMA_SetBank(MS_VIRT u32NonPmBankAddr, MS_VIRT u32PmBankAddr)
{
    _u32RegBase = u32NonPmBankAddr;
    _u32PmRegBase = u32PmBankAddr;
    _AESDMACtrl = (REG_AESDMACtrl*)(_u32RegBase + REG_AESDMACTRL_BASE);
    _SHARNGCtrl = (REG_SHARNGCtrl*)(_u32RegBase + REG_SHARNGCTRL_BASE);
    _AESDMAClk = (REG_AESDMAClk*)(_u32RegBase + REG_AESDMACLK_BASE);
    _AESDMAMbx = (REG_AESDMAMbx*)(_u32PmRegBase + REG_AESDMAMBX_BASE);
    _PARSERCtrl = (REG_PARSERCtrl*)(_u32RegBase + REG_PARSERCTRL_BASE);
    _CIPHERCtrl = (REG_CIPHERCtrl*)(_u32RegBase + REG_CIPHERCTRL_BASE);
    _DMASECURECtrl = (REG_DMASECURECtrl*)(_u32RegBase + REG_DMASECURE_BASE);
    _SECUREBASECtrl = (REG_SECUREBASECtrl*)(_u32RegBase + REG_DMASECURE_CTRL_BASE);
    _AESDMACtrlEx = (REG_AESDMACtrlEx*)(_u32RegBase + REG_AESDMAEXT_BASE);
    _TZPCCtrl = (REG_TZPCCtrl*)(_u32RegBase + REG_TZPC_NONPM_BASE);
    _HDCP22Data = (REG_HDCP22Data*)(_u32RegBase + REG_HDCP22_BASE);
    _HDCP22SekCtrl = (REG_HDCP22SEKCtrl*)(_u32RegBase + REG_HDCP22_SEK);
    _HDCP14SekCtrl = (REG_HDCP14SEKCtrl*)(_u32RegBase + REG_HDCP14_BASE);
    _ANAMISCCtrl = (REG_ANAMISCBASECtrl*)(_u32RegBase + REG_ANA_MISC_BASE);
    _AESDSCMB6Ctrl = (REG_AES_DSCMB6Ctrl*)(_u32RegBase + REG_AES_DSCMB6_BASE);        
    _AESDSCMB7Ctrl = (REG_AES_DSCMB7Ctrl*)(_u32RegBase + REG_AES_DSCMB7_BASE);       
}

MS_PHY AESDMA_GetMIU1Base(void)
{
    return HAL_MIU1_BASE;
}

MS_PHY AESDMA_GetMIU2Base(void)
{
    //not support
    return FALSE;
}

void AESDMA_Reset(void)
{
    MS_VIRT Reg_AESDMA;
    MS_U32 i = 0;
    Reg_AESDMA = (MS_VIRT)(&_AESDMACtrl[0].Dma_Ctrl);

    REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , 0x00000000); // clear ctrl register
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl)|(AESDMA_CTRL_SW_RST)); // sw rst
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl)&~(AESDMA_CTRL_SW_RST));

    // reset all AESdma register
    for (i = 0 ; i < 19 ; i++)
    {
        REG32_W((REG32 *)(Reg_AESDMA+(i*8)), 0x00000000);
    }
    // reset secure_dma3_ctrl register
    REG32_W((&_DMASECURECtrl[0].Secure_dma3_ctrl), 0x00000000);
    
    // reset hw_parser_ctrl register
    REG32_W((&_PARSERCtrl[0].Parser_Pid1),0x00000000);
    REG32_W((&_PARSERCtrl[0].Parser_Ctrl),0x00000000);
}

void AESDMA_Set_CipherKey(MS_U32 *cipherkey)
{
    REG32_W((&_AESDMACtrl[0].Dma_CipherKey_L.Key_L) , cipherkey[0]);
    REG32_W((&_AESDMACtrl[0].Dma_CipherKey_L.Key_H) , cipherkey[1]);
    REG32_W((&_AESDMACtrl[0].Dma_CipherKey_H.Key_L) , cipherkey[2]);
    REG32_W((&_AESDMACtrl[0].Dma_CipherKey_H.Key_H) , cipherkey[3]);
}

void AESDMA_Set_InitVector(MS_U32 *pInitVector)
{
    REG32_W((&_AESDMACtrl[0].Dma_InitVector_L.IV_L), pInitVector[0]);
    REG32_W((&_AESDMACtrl[0].Dma_InitVector_L.IV_H), pInitVector[1]);
    REG32_W((&_AESDMACtrl[0].Dma_InitVector_H.IV_L), pInitVector[2]);
    REG32_W((&_AESDMACtrl[0].Dma_InitVector_H.IV_H), pInitVector[3]);
}

void AESDMA_Sel_Key(MS_BOOL keysel)
{
}

void AESDMA_Sel_SecureKey(void)
{
    MS_U32 u32Ctrl = _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2);
    u32Ctrl |= AESDMA_USE_SECRET_KEY;
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl2), u32Ctrl);
}

void AESDMA_Set_FileinDesc(MS_PHY FileinAddr , MS_PHY FileinNum)
{
    REG32_W((&_AESDMACtrl[0].Dma_Filein_Addr), FileinAddr);
    REG32_W((&_AESDMACtrl[0].Dma_Filein_Num), FileinNum);
}

void AESDMA_Set_FileoutDesc(MS_PHY FileoutSAddr , MS_PHY FileoutEAddr)
{
    REG32_W((&_AESDMACtrl[0].Dma_Fileout_SAddr), FileoutSAddr);
    REG32_W((&_AESDMACtrl[0].Dma_Fileout_EAddr), FileoutEAddr);
}

void AESDMA_Start(MS_BOOL AESDMAStart)
{
    if (AESDMAStart)
    {
        REG32_W((&_AESDMACtrl[0].Dma_Ctrl), (_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl) &~ AESDMA_CTRL_FILEOUT_START &~ AESDMA_CTRL_FILEIN_START));
        REG32_W((&_AESDMACtrl[0].Dma_Ctrl), (_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl) | AESDMA_CTRL_FILEOUT_START | AESDMA_CTRL_FILEIN_START));
    }
    else
    {
        REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl)&~(AESDMA_CTRL_FILEOUT_START|AESDMA_CTRL_FILEIN_START));
    }
}

void AESDMA_Set_PS_PTN(MS_U32 MatchPTN)
{
    REG32_W((&_AESDMACtrl[0].Dma_PS_Pattern) , MatchPTN);
}

void AESDMA_Set_PS_Mask(MS_U32 MatchMask)
{
    REG32_W((&_AESDMACtrl[0].Dma_PS_Pattern_Mask) , MatchMask);
}

void AESDMA_Set_PS_ENG(MS_BOOL PSin_en, MS_BOOL PSout_en)
{
    MS_U32 u32PSin, u32PSout;

    u32PSin  = AESDMA_ENG_PS_IN_EN & (PSin_en<<20);
    u32PSout = AESDMA_ENG_PS_OUT_EN & (PSout_en<<21);

    REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl)|(u32PSin|u32PSout));
}

MS_U32 AESDMA_Get_PS_MatchedBytecnt(void)
{
    return _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Matched_Btyecnt);
}

MS_U32 AESDMA_Get_PS_MatchedPTN(void)
{
    return _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Matched_Pat);
}

void AESDMA_Set_PS_Release(void)
{
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl), _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl)|AESDMA_ENG_PS_RELEASE);
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl), _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl) & ~AESDMA_ENG_PS_RELEASE);
}

MS_U32 AESDMA_Get_AESDMA_Status(void)
{
    return _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_PVR_Status);
}

MS_U32 AESDMA_Get_AESDMA_IsFinished(void)
{
    return (_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_PVR_Status) & AESDMA_IS_FINISHED);
}

void AESDMA_Set_CIPHER_ENG(AESDMA_CipherMode eMode, MS_BOOL Descrypt)
{
    MS_U32 u32Cmd = 0;

    switch (eMode)
    {
    case E_AESDMA_CIPHER_ECB:
        u32Cmd |= AESDMA_ENG_AES_EN;
        break;

    case E_AESDMA_CIPHER_CTR:
        u32Cmd |= (AESDMA_ENG_AES_EN | AESDMA_ENG_CTR_MODE);
        break;

    case E_AESDMA_CIPHER_CBC:
        u32Cmd |= (AESDMA_ENG_AES_EN | AESDMA_ENG_CBC_MODE);
        break;

    case E_AESDMA_CIPHER_DES_ECB:
        u32Cmd |= AESDMA_ENG_DES_EN;
        break;

    case E_AESDMA_CIPHER_DES_CTR:
        u32Cmd |= (AESDMA_ENG_DES_EN | AESDMA_ENG_CTR_MODE);
        break;

    case E_AESDMA_CIPHER_DES_CBC:
        u32Cmd |= (AESDMA_ENG_DES_EN | AESDMA_ENG_CBC_MODE);
        break;

    case E_AESDMA_CIPHER_TDES_ECB:
        u32Cmd |= AESDMA_ENG_TDES_EN;
        break;

    case E_AESDMA_CIPHER_TDES_CTR:
        u32Cmd |= (AESDMA_ENG_TDES_EN | AESDMA_ENG_CTR_MODE);
        break;

    case E_AESDMA_CIPHER_TDES_CBC:
        u32Cmd |= (AESDMA_ENG_TDES_EN | AESDMA_ENG_CBC_MODE);
        break;

    case E_AESDMA_CIPHER_CTS_CBC:
        u32Cmd |= (AESDMA_ENG_AES_EN | AESDMA_ENG_CTS_CBC_MODE);
        break;

    case E_AESDMA_CIPHER_CTS_ECB:
        u32Cmd |= (AESDMA_ENG_AES_EN | AESDMA_ENG_CTS_ECB_MODE);
        break;

    case E_AESDMA_CIPHER_DES_CTS_CBC:
        u32Cmd |= (AESDMA_ENG_DES_EN | AESDMA_ENG_CTS_CBC_MODE);
        break;

    case E_AESDMA_CIPHER_DES_CTS_ECB:
        u32Cmd |= (AESDMA_ENG_DES_EN | AESDMA_ENG_CTS_ECB_MODE);
        break;

    case E_AESDMA_CIPHER_TDES_CTS_CBC:
        u32Cmd |= (AESDMA_ENG_TDES_EN | AESDMA_ENG_CTS_CBC_MODE);
        break;

    case E_AESDMA_CIPHER_TDES_CTS_ECB:
        u32Cmd |= (AESDMA_ENG_TDES_EN | AESDMA_ENG_CTS_ECB_MODE);
        break;
    default:
        return;
    }

	if( eMode >= E_AESDMA_CIPHER_DES_ECB )
		REG32_W((&_AESDMACtrl[0].Dma_Ctrl2),_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2) | AESDMA_DMA_USE_TDES_EN);

    u32Cmd |= AESDMA_ENG_DESCRYPT & (Descrypt<<25);
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl),(( _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl) & (~DmaCtrlSet)) | u32Cmd ) );
}

MS_U32 AESDMA_QueryCipherMode(AESDMA_CipherMode eMode)
{
    MS_U32 u32Result = 1;

    switch (eMode)
    {
    case E_AESDMA_CIPHER_ECB:
    case E_AESDMA_CIPHER_CTR:
    case E_AESDMA_CIPHER_CBC:
    case E_AESDMA_CIPHER_DES_ECB:
    case E_AESDMA_CIPHER_DES_CTR:
    case E_AESDMA_CIPHER_DES_CBC:
    case E_AESDMA_CIPHER_TDES_ECB:
    case E_AESDMA_CIPHER_TDES_CTR:
    case E_AESDMA_CIPHER_TDES_CBC:
    case E_AESDMA_CIPHER_CTS_CBC:
    case E_AESDMA_CIPHER_CTS_ECB:
    case E_AESDMA_CIPHER_DES_CTS_CBC:
    case E_AESDMA_CIPHER_DES_CTS_ECB:
    case E_AESDMA_CIPHER_TDES_CTS_CBC:
    case E_AESDMA_CIPHER_TDES_CTS_ECB:
        u32Result = 0;
        break;
    default:
        break;
    }
    return u32Result;

}

void AESDMA_Set_MIU_Path(MS_BOOL MIU_R, MS_BOOL MIU_W)
{
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl2), RESET_FLAG1(_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2), AESDMA_CTRL_BANK_R));
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl2), RESET_FLAG1(_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2), AESDMA_CTRL_BANK_W));

    REG32_W((&_AESDMACtrl[0].Dma_Ctrl2), SET_FLAG1(_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2), (AESDMA_CTRL_BANK_R & ((MS_U32)MIU_R << 4))));
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl2), SET_FLAG1(_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2), (AESDMA_CTRL_BANK_W & ((MS_U32)MIU_W << 5))));
}

void AESDMA_Set_MIU2_Path(MS_BOOL MIU_R, MS_BOOL MIU_W)
{
    // Not support
}

void AESDMA_Enable_Int(void)
{
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl2) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2)|AESDMA_INT_EN);
}

void AESDMA_Disable_Int(void)
{
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl2) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2)&~AESDMA_INT_EN);
}

void AESDMA_Clear_Int(void)
{
    //
    // Since ISR Clear bit for Normal and Secured bank is independently, it must be reset individually
    //

#if AESDMS_SECURED_BANK_EN    
    //
    // For Secured Bank
    //
    REG32_W((&_AESDSCMB6Ctrl[0].Reg0001) , _AESDMA_REG32_R(&_AESDSCMB6Ctrl[0].Reg0001)|AESDMA_ISR_CLR);
    REG32_W((&_AESDSCMB6Ctrl[0].Reg0001) , _AESDMA_REG32_R(&_AESDSCMB6Ctrl[0].Reg0001)&~AESDMA_ISR_CLR);
#else        
    //
    // For Normal Bank
    //
    REG32_W((&_AESDSCMB7Ctrl[0].Reg0001) , _AESDMA_REG32_R(&_AESDSCMB7Ctrl[0].Reg0001)|AESDMA_ISR_CLR);
    REG32_W((&_AESDSCMB7Ctrl[0].Reg0001) , _AESDMA_REG32_R(&_AESDSCMB7Ctrl[0].Reg0001)&~AESDMA_ISR_CLR);
#endif  //#if AESDMS_SECURED_BANK_EN
}

void AESDMA_HW_Patch(void)
{
    REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl)|AESDMA_ECO_FIX_LAST_BYTE);
    //REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl)|AESDMA_CTRL_BURST_LENGTH);
    //REG32_W((&_AESDMACtrl[0].Dma_Eng3_Ctrl) , _AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Eng3_Ctrl)|AESDMA_ENG_SPEEDUP); 
}

void AESDMA_Enable_Clk(void)
{
    MS_U32 u32Val;

    //1. clk_aesdma, 0x100A_19[3:0] = 0
    u32Val = _AESDMA_REG32_R(&_AESDMAClk[0].Reg_Gate_Clk_AESDMA);
    u32Val &= ~AESDMA_CLK_XTAL;  //Clear Clk setting.
    u32Val |= AESDMA_CLK_172M;   //Set Clk to 172M
    u32Val &= ~AESDMA_CLK_OFF_EN;//Enable Clk
    REG32_W((&_AESDMAClk[0].Reg_Gate_Clk_AESDMA) , u32Val);

#if 0    
    //
    // Due to the security concern
    // Set this CLK at SBOOT
    //
#if defined (__aarch64__)    
    //Enable clk_rc: 0x1239_0x03 = 0x004F
#else
    //2. clk_rc
    u32Val = _AESDMA_REG32_R(&_TZPCCtrl[0].Reg0203);
    u32Val &= 0x0000FFFF;   //Clear 0x3
    u32Val |= 0x004F0000;   //0x1239_0x03 = 0x004F
    REG32_W((&_TZPCCtrl[0].Reg0203) , u32Val);
#endif
#endif

    //3. Enable Rng
    u32Val = _AESDMA_REG32_R(&_SECUREBASECtrl[0].Reg0001);
    u32Val |= REG_RNG_EN;
    REG32_W((&_SECUREBASECtrl[0].Reg0001) , u32Val);
    
    //4. 0x110C_11[9], reg_mipspll_occclk2_en, For Random number Gen, Manhattan only
    u32Val = _AESDMA_REG32_R(&_ANAMISCCtrl[0].Reg0001);
    u32Val |= REG_MIPSPLL_OCCCLK2_EN;
    REG32_W((&_ANAMISCCtrl[0].Reg0001) , u32Val);    
}

void AESDMA_Disable_Clk(void)
{
    REG32_W((&_AESDMAClk[0].Reg_Gate_Clk_AESDMA) , _AESDMA_REG32_R(&_AESDMAClk[0].Reg_Gate_Clk_AESDMA) | AESDMA_CLK_OFF_EN);
}

MS_U8 AESDMA_Rand(void)
{
   MS_U8 u8RN = 0x00;
   u8RN = (MS_U8)_AESDMA_REG32_R(&_SECUREBASECtrl[0].Reg0203)&0xFF;

   return u8RN;
}

void SHA_Reset(void)
{
    MS_VIRT Reg_SHARNG;
    MS_U32 i = 0;
    Reg_SHARNG = (MS_VIRT)(&_SHARNGCtrl[0].Sha_Ctrl);
    REG32_W((&_SHARNGCtrl[0].Sha_Ctrl) , SHARNG_CTRL_SHA_RST); // 0->1
	REG32_W((&_SHARNGCtrl[0].Sha_Ctrl) , (_AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Ctrl)&(~SHARNG_CTRL_SHA_RST))); // 1->0

	// reset all SHA & RNG register
    for (i = 0 ; i < 7 ; i++)
    {
        REG32_W((REG32 *)(Reg_SHARNG+(i*8)), 0x00000000);
    }
}

MS_U32 SHA_Get_Status(void)
{
    return _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Status);
}

void SHA_SelMode(AESDMA_SHAMode eMode)
{
    MS_U32 u32Cmd = 0;

    switch (eMode)
    {
    case E_AESDMA_SHA1:
        u32Cmd |= (SHARNG_CTRL_SHA_MSG_BLOCK_NUM);
        break;

    case E_AESDMA_SHA256:
        u32Cmd |= (SHARNG_CTRL_SHA_SEL_SHA256 | SHARNG_CTRL_SHA_MSG_BLOCK_NUM);
        break;

    default:
        return;
    }

    //Turn off speedup mode
    //u32Cmd |= SHARNG_CTRL_SPEED_MODE_N;

    REG32_W((&_SHARNGCtrl[0].Sha_Ctrl), _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Ctrl) | u32Cmd);
}

void SHA_SetAddress(MS_PHY u32Addr)
{
    REG32_W((&_SHARNGCtrl[0].Sha_Start), u32Addr);
}

void SHA_SetLength(MS_U32 u32Size)
{
    REG32_W((&_SHARNGCtrl[0].Sha_Length), u32Size);
}

void SHA_Start(MS_BOOL SHAStart)
{
	if(SHAStart)
	{
		REG32_W((&_SHARNGCtrl[0].Sha_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Ctrl)&~(SHARNG_CTRL_SHA_FIRE_ONCE));
		REG32_W((&_SHARNGCtrl[0].Sha_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Ctrl)|(SHARNG_CTRL_SHA_FIRE_ONCE));
	}
	else
	{
		REG32_W((&_SHARNGCtrl[0].Sha_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Ctrl)&~(SHARNG_CTRL_SHA_FIRE_ONCE));
	}
}

void SHA_Out(MS_VIRT u32Buf)
{
    MS_U32 Index, u32Size, u32offset;
    MS_U32 eMode = _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Ctrl)&SHARNG_CTRL_SHA_SEL_SHA256;

    if(eMode == SHARNG_CTRL_SHA_SEL_SHA256)
    {
        u32Size = DIGEST_SIZE_SHA256;
        u32offset = 0;
    }
    else
    {
        u32Size = DIGEST_SIZE_SHA1;
        u32offset = 3;
    }

    for( Index = 0; Index < u32Size; Index++ )
    {
        *((MS_U32 *)u32Buf + Index) = _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Out[Index + u32offset]);
    }
    //Set "1" to idle state after reg_read_sha_ready = 1
    REG32_W((&_SHARNGCtrl[0].Sha_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Sha_Ctrl)|(SHARNG_CTRL_SHA_CLR));
}


//-------------------------------------------------------------------------------------------------
/// Enable mobf one way function
/// @param  NULL

/// @note : disable read mobf key, and could not enable read mobf key anymore
//-------------------------------------------------------------------------------------------------
void MOBF_OneWay(void)
{
	REG32_W((&_SHARNGCtrl[0].Rng_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl)|(MOBF_ONEWAY_EN));
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF encrypt key
/// @param  u32WriteKey                \b IN: Encrypt Key

/// @note
//-------------------------------------------------------------------------------------------------
void MOBF_WriteKey(MS_U32 u32WriteKey)
{
	REG32_W((&_SHARNGCtrl[0].MOBF_KeyW) , u32WriteKey);
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF decrypt key
/// @param  u32ReadKey                \b IN: Decrypt Key

/// @note
//-------------------------------------------------------------------------------------------------
void MOBF_ReadKey(MS_U32 u32ReadKey)
{
	REG32_W((&_SHARNGCtrl[0].MOBF_KeyR) , u32ReadKey);
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF in MIU write port
/// @param  bEnable                \b IN: Enable/Disable

/// @note : MOBF Encrypt
//-------------------------------------------------------------------------------------------------
void MOBF_WriteEnable(MS_BOOL bEnable)
{
	if(bEnable)
	{
		REG32_W((&_SHARNGCtrl[0].Rng_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl)&(~MOBF_IN_MIU_WRITE_EN));
	}
	else
	{
		REG32_W((&_SHARNGCtrl[0].Rng_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl)&(~MOBF_IN_MIU_WRITE_EN));
        REG32_W((&_SHARNGCtrl[0].Rng_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl)|(MOBF_IN_MIU_WRITE_EN));
	}
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF in MIU read port
/// @param  bEnable                \b IN: Enable/Disable

/// @note : MOBF Decrypt
//-------------------------------------------------------------------------------------------------
void MOBF_ReadEnable(MS_BOOL bEnable)
{
	if(bEnable)
	{
		REG32_W((&_SHARNGCtrl[0].Rng_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl)&(~MOBF_IN_MIU_READ_EN));
	}
	else
	{
		REG32_W((&_SHARNGCtrl[0].Rng_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl)&(~MOBF_IN_MIU_READ_EN));
        REG32_W((&_SHARNGCtrl[0].Rng_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl)|(MOBF_IN_MIU_READ_EN));
	}
}

//-------------------------------------------------------------------------------------------------
/// Disable TDES
///

/// @note : MOBF Decrypt
//-------------------------------------------------------------------------------------------------
void MOBF_DisableTDES(void)
{
	REG32_W((&_AESDMACtrl[0].Dma_Ctrl2),_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl2) & (~AESDMA_DMA_USE_TDES_EN));
}

void AESDMA_Parser_Set_Mode(AESDMA_ParserMode eMode)
{
    MS_U32 u32Cmd = 0;

    switch (eMode)
    {
    case E_AESDMA_PARSER_TS_PKT192:
        u32Cmd |= (TS_MODE | PKT192_MODE);
        break;

    case E_AESDMA_PARSER_TS_PKT192_CLEAR:
        u32Cmd |= (TS_MODE | PKT192_MODE | CLEAR_MODE);
        break;

    case E_AESDMA_PARSER_TS_PKT188:
        u32Cmd |= (TS_MODE);
        u32Cmd &= (~PKT192_MODE);
        break;

    case E_AESDMA_PARSER_TS_PKT188_CLEAR:
        u32Cmd |= (TS_MODE | CLEAR_MODE);
        u32Cmd &= (~PKT192_MODE);
        break;

    case E_AESDMA_PARSER_HDCP20_PKT192:
        u32Cmd |= (HDCP20_MODE | PKT192_MODE);
        break;

    case E_AESDMA_PARSER_HDCP20_PKT192_CLEAR:
        u32Cmd |= (HDCP20_MODE | PKT192_MODE | CLEAR_MODE);
        break;

    case E_AESDMA_PARSER_HDCP20_PKT188:
        u32Cmd |= (HDCP20_MODE);
        u32Cmd &= (~PKT192_MODE);
        break;

    case E_AESDMA_PARSER_HDCP20_PKT188_CLEAR:
        u32Cmd |= (HDCP20_MODE | CLEAR_MODE);
        u32Cmd &= (~PKT192_MODE);
        break;

    default:
        return;
    }

    u32Cmd |= (AUTO_MODE | INIT_TRUST);
    REG32_W((&_PARSERCtrl[0].Parser_Ctrl),(( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~ParserCtrlSet)) | u32Cmd ) );
}


void AESDMA_Parser_Insert_Scrmb(MS_BOOL bEnable)
{
    if(bEnable)
	{
	    REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~INSERT_SCRMB)) );
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) | (INSERT_SCRMB)) );
    }
    else
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~INSERT_SCRMB)) );
    }
}

void AESDMA_Parser_Remove_Scrmb(MS_BOOL bEnable)
{
    if(bEnable)
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~REMOVE_SCRMB)) );
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) | (REMOVE_SCRMB)) );
    }
    else
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~REMOVE_SCRMB)) );
    }
}

void AESDMA_Parser_Mask_Scrmb(MS_BOOL bEnable)
{
    if(bEnable)
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~TS_SCRMB_MASK)) );
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) | (TS_SCRMB_MASK)) );
    }
    else
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~TS_SCRMB_MASK)) );
    }
}

void AESDMA_Parser_Bypass_Pid(MS_BOOL bEnable)
{
    if(bEnable)
    {
        REG32_W((&_PARSERCtrl[0].Parser_Pid1),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) & (~BYPASS_PID)) );
        REG32_W((&_PARSERCtrl[0].Parser_Pid1),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) | (BYPASS_PID)) );
    }
    else
    {
        REG32_W((&_PARSERCtrl[0].Parser_Pid1),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) & (~BYPASS_PID)) );
    }
}

void AESDMA_Parser_Set_ScrmbPattern(AESDMA_ScrmbPattern ePattern)
{
    MS_U32 u32Cmd = 0;

    switch (ePattern)
    {
    case E_AESDMA_PARSER_SCRMB_10:
        u32Cmd |= (SCRMB_PATTERN10);
        break;

    case E_AESDMA_PARSER_SCRMB_11:
        u32Cmd |= (SCRMB_PATTERN11);
        break;

    default:
        u32Cmd &= ~(SCRMB_PATTERN11);
        break;
    }

    REG32_W((&_PARSERCtrl[0].Parser_Ctrl),(( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~SCRMB_INITVALUE)) | u32Cmd ) );

}

void AESDMA_Parser_Set_AddedScrmbPattern(AESDMA_ScrmbPattern ePattern)
{
    MS_U32 u32Cmd = 0;

    switch (ePattern)
    {
    case E_AESDMA_PARSER_SCRMB_10:
        u32Cmd |= (SCRMB_PATTERN10_ADD);
        break;

    case E_AESDMA_PARSER_SCRMB_11:
        u32Cmd |= (SCRMB_PATTERN11_ADD);
        break;

    default:
        return;
    }

    REG32_W((&_PARSERCtrl[0].Parser_Pid1),(( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) & (~SCRMB_INITVALUE_ADD)) | u32Cmd ) );

}

void AESDMA_Parser_Set_Pid(MS_U8 u8Index, MS_U16 u16Pid)
{
    if(u8Index==0)
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),(( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~PARSER_PID0_MASK)) | (u16Pid<<16) ) );
    }
    else if(u8Index==1)
    {
        REG32_W((&_PARSERCtrl[0].Parser_Pid1),(( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) & (~PARSER_PID1_MASK)) | u16Pid ) );
    }
    else
    {
        printf("Edison do not support other PID\n");
    }
}

MS_U8 AESDMA_Parser_Query_PidCount(void)
{
    return PARSER_PID_COUNT;
}

void AESDMA_Parser_Enable_HWParser(MS_BOOL bEnable)
{
#if AESDMS_SECURED_BANK_EN
    //
    // Set HW Parser controlled by DSCRMB3
    //
    REG32_W((&_AESDSCMB6Ctrl[0].Reg0001) , _AESDMA_REG32_R(&_AESDSCMB6Ctrl[0].Reg0001)|AESDMA_DSCRMB3_HW_PASER_EN);
#else
    //
    // Set HW Parser controlled by DSCRMB5
    //
    REG32_W((&_AESDSCMB6Ctrl[0].Reg0001) , _AESDMA_REG32_R(&_AESDSCMB6Ctrl[0].Reg0001)&~(AESDMA_DSCRMB3_HW_PASER_EN));
#endif  //#if AESDMS_SECURED_BANK_EN

    if(bEnable)
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~HW_PARSER_MODE))  );
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) | (HW_PARSER_MODE))  );
        REG32_W((&_PARSERCtrl[0].Parser_Pid1),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) | (ENABLE_LG_PATCH))  );
    }
    else
    {
        REG32_W((&_PARSERCtrl[0].Parser_Ctrl),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Ctrl) & (~HW_PARSER_MODE))  );
        REG32_W((&_PARSERCtrl[0].Parser_Pid1),( _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) & (~ENABLE_LG_PATCH))  );
    }

    //make sure CC_CTRL_BY_ACPU is set 0
    REG32_W((&_CIPHERCtrl[0].Cipher_Ctrl),( _AESDMA_REG32_R(&_CIPHERCtrl[0].Cipher_Ctrl) & (~CC_CTRL_BY_ACPU)) );
}

void RSA_ClearInt(void)
{
    //RSA interrupt clear
    REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_RData) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_RData)|(RSA_INT_CLR));
}

void RSA_Reset(void)
{
    //RSA Rst
    REG32_W((&_SHARNGCtrl[0].Rsa_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl)|(RSA_CTRL_RSA_RST));
    REG32_W((&_SHARNGCtrl[0].Rsa_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl)&(~RSA_CTRL_RSA_RST));

    //add polling RSA status before load data to SRAM
    while (( RSA_GetStatus() & RSA_STATUS_RSA_BUSY ) != 0);
}

void RSA_Ind32Ctrl(MS_U8 u8dirction)
{
    //[1] reg_ind32_direction 0: Read. 1: Write
    if(u8dirction==1)
    {
        REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Start) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Start)|(RSA_IND32_CTRL_DIRECTION_WRITE));
    }
    else
    {
        REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Start) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Start)&(~RSA_IND32_CTRL_DIRECTION_WRITE));
    }

    REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Start) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Start)|(RSA_IND32_CTRL_ADDR_AUTO_INC|RSA_IND32_CTRL_ACCESS_AUTO_START));

}

void RSA_LoadSram(MS_U32 *u32Buf, RSA_IND32Address eMode)
{
    MS_U32 u32Cmd = 0;
    MS_U32 i = 0;
    MS_U32 WData_H = 0;
    MS_U32 WData_L = 0;

    switch (eMode)
    {
    case E_RSA_ADDRESS_E:
        u32Cmd |= (RSA_E_BASE_ADDR);
        break;

    case E_RSA_ADDRESS_N:
        u32Cmd |= (RSA_N_BASE_ADDR);
        break;

    case E_RSA_ADDRESS_A:
        u32Cmd |= (RSA_A_BASE_ADDR);
        break;

    default:
        return;
    }

    REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Addr) , ((_AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Addr)&(~RSA_ADDRESS_MASK))|u32Cmd));
    REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Start) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Start)|(RSA_INDIRECT_START));

    for( i = 0; i < 64; i++ )
    {
        if( eMode == E_RSA_ADDRESS_E )
        {
            WData_L = (((*(u32Buf+i))>>8)&0xFF00)|(((*(u32Buf+i))>>24)&0xFF);
            WData_H = (((*(u32Buf+i))>>8)&0xFF)|(((*(u32Buf+i))<<8)&0xFF00);
        }
        else
        {
            WData_L = (((*(u32Buf-i))>>8)&0xFF00)|(((*(u32Buf-i))>>24)&0xFF);
            WData_H = (((*(u32Buf-i))>>8)&0xFF)|(((*(u32Buf-i))<<8)&0xFF00);
        }

        REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Addr) , ((_AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Addr)&(~RSA_WDATA_MASK_L))|(WData_L<<16)));
        REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_WData) , ((_AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_WData)&(~RSA_WDATA_MASK_H))|WData_H));
    }

}

void RSA_SetKeyLength(MS_U32 u32keylen)
{
    //[13:8] n_len_e: key length
    REG32_W((&_SHARNGCtrl[0].Rsa_Ctrl) , (_AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl)&(~RSA_CTRL_KEY_LENGTH_MASK))|(u32keylen<<8));
}

void RSA_SetKeyType(MS_U8 u8hwkey, MS_U8 u8pubkey)
{
    //[1] hw_key_e : 0 : software key, 1: hardware key
    //[2] e_pub_e : 0: pvivate key, 1: public key
    if(u8hwkey==1)
    {
        REG32_W((&_SHARNGCtrl[0].Rsa_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl)|(RSA_CTRL_SEL_HW_KEY));
    }
    else
    {
        REG32_W((&_SHARNGCtrl[0].Rsa_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl)&(~RSA_CTRL_SEL_HW_KEY));
    }

    if(u8pubkey==1)
    {
        REG32_W((&_SHARNGCtrl[0].Rsa_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl)|(RSA_CTRL_SEL_PUBLIC_KEY));
    }
    else
    {
        REG32_W((&_SHARNGCtrl[0].Rsa_Ctrl) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl)&(~RSA_CTRL_SEL_PUBLIC_KEY));
    }

}

void RSA_ExponetialStart(void)
{
    //RSA exp start
    REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_RData) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_RData)|(RSA_EXP_START));
}

MS_U32 RSA_GetStatus(void)
{
    return _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ctrl) & RSA_STATUS_MASK;
}

void RSA_FileOutStart(void)
{
    //RSA ind32_start
    REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Start) , _AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Start)|(RSA_INDIRECT_START));
}

void RSA_SetFileOutAddr(MS_U32 u32offset)
{
    MS_PHY u32Addr = 0;
    u32Addr = RSA_Z_BASE_ADDR + u32offset;

    REG32_W((&_SHARNGCtrl[0].Rsa_Ind32_Addr) , ((_AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_Addr)&(~RSA_ADDRESS_MASK))|u32Addr));
}

MS_U32 RSA_FileOut(void)
{
    MS_U32 u32out_L = 0;
    MS_U32 u32out_H = 0;

    u32out_L = (_AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_WData)&RSA_RDATA_MASK_L)>>16;
    u32out_H = (_AESDMA_REG32_R(&_SHARNGCtrl[0].Rsa_Ind32_RData)&RSA_RDATA_MASK_H)<<16;

    return (u32out_H|u32out_L);
}

MS_U32 RSA_Get_RSA_IsFinished(void)
{
    return (RSA_GetStatus()&RSA_STATUS_RSA_BUSY);
}

//reserved for secure protect function
/*
void AESDMA_Secure_SetCipherKey(MS_U32 *cipherkey)
{
    REG32_W((&_DMASECURECtrl[0].Secure_CipherKey_L.Key_L) , cipherkey[0]);
    REG32_W((&_DMASECURECtrl[0].Secure_CipherKey_L.Key_H) , cipherkey[1]);
    REG32_W((&_DMASECURECtrl[0].Secure_CipherKey_H.Key_L) , cipherkey[2]);
    REG32_W((&_DMASECURECtrl[0].Secure_CipherKey_H.Key_H) , cipherkey[3]);
}

void AESDMA_Secure_SetInitVector(MS_U32 *pInitVector)
{
    REG32_W((&_DMASECURECtrl[0].Secure_InitVector_L.IV_L), pInitVector[0]);
    REG32_W((&_DMASECURECtrl[0].Secure_InitVector_L.IV_H), pInitVector[1]);
    REG32_W((&_DMASECURECtrl[0].Secure_InitVector_H.IV_L), pInitVector[2]);
    REG32_W((&_DMASECURECtrl[0].Secure_InitVector_H.IV_H), pInitVector[3]);
}

void AESDMA_SecureProtect(MS_BOOL bEnable)
{
    if(bEnable)
	{
		REG32_W((&_DMASECURECtrl[0].Secure_dma3_ctrl) , _AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_dma3_ctrl)|(AESDMA_SECURE_PROTECT_S));
	}
	else
	{
        REG32_W((&_DMASECURECtrl[0].Secure_dma3_ctrl) , _AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_dma3_ctrl)&(~AESDMA_SECURE_PROTECT_S));
	}
}

void AESDMA_Set_FileOutEnable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        REG32_W((&_AESDMACtrl[0].Dma_Ctrl), (_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl) |AESDMA_CTRL_FILEIN_START));
    }
    else
    {
        REG32_W((&_AESDMACtrl[0].Dma_Ctrl), (_AESDMA_REG32_R(&_AESDMACtrl[0].Dma_Ctrl) &~AESDMA_CTRL_FILEIN_START));
    }
}

void AESDMA_Secure_Start(MS_BOOL AESDMAStart)
{
    if (AESDMAStart)
    {
        REG32_W((&_DMASECURECtrl[0].Secure_file_st), (_AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_file_st) &~AESDMA_SECURE_FILEIN_START));
        REG32_W((&_DMASECURECtrl[0].Secure_file_st), (_AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_file_st) | AESDMA_SECURE_FILEIN_START));
    }
    else
    {
        REG32_W((&_DMASECURECtrl[0].Secure_file_st), (_AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_file_st) &~AESDMA_SECURE_FILEIN_START));
    }
}

MS_U32 AESDMA_Get_AESDMA_SecureIsFinished(void)
{
    return (_AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_dma3_status) & AESDMA_DONE_S);
}

*/

//-------------------------------------------------------------------------------------------------
/// Get status of Secret Key in Normal bank
/// @param  N/A
/// @note : Get Status
//-------------------------------------------------------------------------------------------------
MS_U32 AESDMA_IsSecretKeyInNormalBank(void)
{
    return (_AESDMA_REG32_R(&_SHARNGCtrl[0].Rng_Ctrl) & (SECRET_KEY_IN_NORMAL_BANK));
}

//-------------------------------------------------------------------------------------------------
/// Enable Two keys (Odd/Even) in using HW auto select key.
/// @param  bEnable                \b IN: Enable/Disable

/// @note : Hardware Parser Decrypt
//-------------------------------------------------------------------------------------------------
void AESDMA_Parser_Enable_Two_Keys(MS_BOOL bEnable)
{
    if(bEnable)
    {
        REG32_W((&_PARSERCtrl[0].Parser_Pid1), _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) | (SCRMB_ENABLE_TWO_KEY) );
    }
    else
    {
        REG32_W((&_PARSERCtrl[0].Parser_Pid1), _AESDMA_REG32_R(&_PARSERCtrl[0].Parser_Pid1) & (~SCRMB_ENABLE_TWO_KEY) );
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Odd keys in using HW auto select key.
/// @param  cipherkey
/// @note : Hardware Parser Decrypt
//-------------------------------------------------------------------------------------------------
void AESDMA_Set_CipherOddKey(MS_U32 *cipherkey)
{
    REG32_W((&_AESDMACtrlEx[0].Dma_CipherKey_L.Key_L) , cipherkey[0]);
    REG32_W((&_AESDMACtrlEx[0].Dma_CipherKey_L.Key_H) , cipherkey[1]);
    REG32_W((&_AESDMACtrlEx[0].Dma_CipherKey_H.Key_L) , cipherkey[2]);
    REG32_W((&_AESDMACtrlEx[0].Dma_CipherKey_H.Key_H) , cipherkey[3]);
}

//-------------------------------------------------------------------------------------------------
/// Set Odd IVs in using HW auto select key.
/// @param  cipherkey
/// @note : Hardware Parser Decrypt
//-------------------------------------------------------------------------------------------------
void AESDMA_Set_OddInitVector(MS_U32 *pInitVector)
{
    REG32_W((&_AESDMACtrlEx[0].Dma_InitVector_L.IV_L), pInitVector[0]);
    REG32_W((&_AESDMACtrlEx[0].Dma_InitVector_L.IV_H), pInitVector[1]);
    REG32_W((&_AESDMACtrlEx[0].Dma_InitVector_H.IV_L), pInitVector[2]);
    REG32_W((&_AESDMACtrlEx[0].Dma_InitVector_H.IV_H), pInitVector[3]);
}

//-------------------------------------------------------------------------------------------------
/// Set Even keys in using HW auto select key.
/// @param  cipherkey
/// @note : Hardware Parser Decrypt
//-------------------------------------------------------------------------------------------------
void AESDMA_Set_CipherEvenKey(MS_U32 *cipherkey)
{
    AESDMA_Set_CipherKey(cipherkey);
}

//-------------------------------------------------------------------------------------------------
/// Set Even IVs in using HW auto select key.
/// @param  cipherkey
/// @note : Hardware Parser Decrypt
//-------------------------------------------------------------------------------------------------
void AESDMA_Set_EvenInitVector(MS_U32 *pInitVector)
{
    AESDMA_Set_InitVector(pInitVector);
}

//-------------------------------------------------------------------------------------------------
/// Set HDCP Data
/// @param  u8Idx                \b IN: Index
/// @param  pu8Data              \b IN: Content Keys
/// @param  pu8privData          \b IN: RIV
/// @note : Set HDCP2.2
//-------------------------------------------------------------------------------------------------
void HDCP_ProcessCipher (MS_U8 u8Idx, MS_U8 *pu8Data, MS_U8* pu8privData)
{
    REG_HDCP22Data *pData = NULL;
    REG_HDCP22SEKCtrl *pCtrl = NULL;
    MS_U16 offset = 0;
    MS_U16 BKOffset = 0;

    MS_U32 *cipherkey = (MS_U32 *)pu8Data;
    MS_U32 *riv = (MS_U32 *)pu8privData;

    offset = u8Idx * (sizeof(REG_HDCP22Data) >> 1);
    BKOffset = u8Idx * 0x300;

    pData = (REG_HDCP22Data*)((MS_U16*)_HDCP22Data + offset);
    pCtrl = (REG_HDCP22SEKCtrl*)((MS_U16*)_HDCP22SekCtrl + BKOffset);

    //Content Keys
    REG32_W((&pData[0].Hdcp22_ContentKey[0]) , cipherkey[0]);
    REG32_W((&pData[0].Hdcp22_ContentKey[1]) , cipherkey[1]);
    REG32_W((&pData[0].Hdcp22_ContentKey[2]) , cipherkey[2]);
    REG32_W((&pData[0].Hdcp22_ContentKey[3]) , cipherkey[3]);

    //RIV
    REG32_W((&pData[0].Hdcp22_Riv[0]) , riv[0]);
    REG32_W((&pData[0].Hdcp22_Riv[1]) , riv[1]);

    //Set SKE successful
    REG32_W((&pCtrl[0].SEK), (_AESDMA_REG32_R(&pCtrl[0].SEK) | HDCP_SEK_BIT0));
}

//-------------------------------------------------------------------------------------------------
/// Get the status of HW supports HDCP2.2
/// @param  N/A
/// @note : Support HDCP2.2
//-------------------------------------------------------------------------------------------------
void HDCP_GetHdcpCipherState(MS_U8 u8Idx, MS_U8 *pu8State)
{
#define HDCP_SUPPORT_EN 0x00000001
    REG_HDCP22SEKCtrl *pCtrl = NULL;
    MS_U16 offset = 0;
    MS_U16 BKOffset = 0;

    offset = u8Idx * (sizeof(REG_HDCP22Data) >> 1);
    BKOffset = u8Idx * 0x300;

    pCtrl = (REG_HDCP22SEKCtrl*)((MS_U16*)_HDCP22SekCtrl + BKOffset);

    *pu8State = (MS_U8)(_AESDMA_REG32_R(&pCtrl[0].SEK) & HDCP_SUPPORT_EN);
}

//-------------------------------------------------------------------------------------------------
/// Reset AESDMA
/// @param  N/A
/// @note : Clear Registers, not to do sw reset
//-------------------------------------------------------------------------------------------------
void AESDMA_NormalReset(void)
{
    MS_VIRT Reg_AESDMA;
    MS_U32 i = 0;
    Reg_AESDMA = (MS_VIRT)(&_AESDMACtrl[0].Dma_Ctrl);

    REG32_W((&_AESDMACtrl[0].Dma_Ctrl) , 0x00000000); // clear ctrl register

    // reset all AESdma register
    for (i = 0 ; i < 19 ; i++)
    {
        REG32_W((REG32 *)(Reg_AESDMA+(i*8)), 0x00000000);
    }
    // reset secure_dma3_ctrl register
    REG32_W((&_DMASECURECtrl[0].Secure_dma3_ctrl), 0x00000000);
    
    // reset hw_parser_ctrl register
    REG32_W((&_PARSERCtrl[0].Parser_Pid1),0x00000000);
    REG32_W((&_PARSERCtrl[0].Parser_Ctrl),0x00000000);
}

void AESDMA_Set_SecureBankEnable (MS_BOOL bEnable)
{
#ifdef MSOS_TYPE_NUTTX
    if ( bEnable == TRUE)
    {        
        REG32_W((&_DMASECURECtrl[0].Secure_dma3_ctrl) , _AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_dma3_ctrl) |(AESDMA_SECURE_PROTECT_S | AESDMA_SECURE_PROTECT_SHA_S | AESDMA_SECURE_PROTECT_RSA_S));
    }
    else
    {        
        REG32_W((&_DMASECURECtrl[0].Secure_dma3_ctrl) , _AESDMA_REG32_R(&_DMASECURECtrl[0].Secure_dma3_ctrl) &~(AESDMA_SECURE_PROTECT_S | AESDMA_SECURE_PROTECT_SHA_S | AESDMA_SECURE_PROTECT_RSA_S));
    }
#endif
}

void HAL_AESDMA_HDMI_GetM0(AESDMA_HDMI_INPUT_PORT enInputPortType, MS_U8 *pu8Data)
{
    MS_U16 u16bank_offset;
    MS_U32 m0[2];
    MS_U8* buf = (MS_U8*)m0;
    int i;

    switch(enInputPortType)
    {
        default:
        case E_AESDMA_HDMI_INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x171200
        case E_AESDMA_HDMI_INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171500
        case E_AESDMA_HDMI_INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171800
        case E_AESDMA_HDMI_INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171B00
    }

    // read 8 bytes
    for(i = 0; i < 2; ++i)
    {
        m0[i] = _AESDMA_REG32_R(&((REG_HDCP14SEKCtrl*)((MS_U16*)_HDCP14SekCtrl + u16bank_offset))[i].REG_0001); 
    }
    for (i = 0; i < 8; ++i)
    {
        pu8Data[i] = buf[i];
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Index of KSlot
/// @param  u32Index
/// @param  bEnable                \b IN: TRUE: For Odd Key/FALSE: For Even Key
/// @note : AESDMA AES CTR64
//-------------------------------------------------------------------------------------------------
void AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey)
{
}

//-------------------------------------------------------------------------------------------------
/// Enable AES CTR64
/// @param  bEnable                \b IN: Enable/Disable
/// @note : AESDMA AES CTR64
//-------------------------------------------------------------------------------------------------
void AESDMA_SET_CTR64(MS_BOOL bEnable)
{
}

//-------------------------------------------------------------------------------------------------
/// Select Efuse key
/// @param  u8KeyIdx                \b IN: the index of efuse key
/// @note : Select Efuse Key
//-------------------------------------------------------------------------------------------------
void AESDMA_SetSecureKeyIdx (MS_U8 u8KeyIdx)
{
}

MS_BOOL AESDMA_POWER_SUSPEND(void)
{
    SECUREBASECtrl_Buffer =  _AESDMA_REG32_R(&_SECUREBASECtrl[0].Reg0001);
    return TRUE;
}

MS_BOOL AESDMA_POWER_RESUME(void)
{
    REG32_W((&_SECUREBASECtrl[0].Reg0001), SECUREBASECtrl_Buffer);
    return TRUE;
}
