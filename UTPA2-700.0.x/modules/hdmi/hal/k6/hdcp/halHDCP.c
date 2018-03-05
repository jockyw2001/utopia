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
// file   halHDCP.c
// @brief  HDCP HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************/
/*                                                                                                                     */
/*                                                   Includes                                                      */
/*                                                                                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>
#include "MsCommon.h"
#include "MsTypes.h"
#include "regHDCP.h"
#include "halHDCP.h"
#include "drvCPU.h"

#ifndef HAL_HDCP_C
#define HAL_HDCP_C

/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/
#define DEF_HDCP_TX_FUNC_EN     1

#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define HalHDCPLogInfo(format, args...)       ULOGI("HDCP", format, ##args)
#define HalHDCPLogWarning(format, args...)    ULOGW("HDCP", format, ##args)
#define HalHDCPLogDebug(format, args...)      ULOGD("HDCP", format, ##args)
#define HalHDCPLogError(format, args...)      ULOGE("HDCP", format, ##args)
#define HalHDCPLogFatal(format, args...)      ULOGF("HDCP", format, ##args)

#else

#define HalHDCPLogInfo(format, args...)       printf(format, ##args)
#define HalHDCPLogWarning(format, args...)    printf(format, ##args)
#define HalHDCPLogDebug(format, args...)      printf(format, ##args)
#define HalHDCPLogError(format, args...)      printf(format, ##args)
#define HalHDCPLogFatal(format, args...)      printf(format, ##args)

#endif


#define DEF_SIZE_OF_KSXORLC128  16
#define DEF_SIZE_OF_RIV         8
#define DEF_SIZE_OF_HDCP1X_KEY  304

MS_VIRT _gHDCPRegBase = 0x00U;
MS_VIRT _gHDCPPMRegBase = 0x00U;

#define HDCPREG(bank, addr)     (*((volatile MS_U16 *)((_gHDCPRegBase + (bank << 1U)) + (addr << 2U))))
#define HDCPPMREG(bank, addr)   (*((volatile MS_U16 *)((_gHDCPPMRegBase + (bank << 1U)) + (addr << 2U))))

#define DEF_HDCP14_M0_SIZE          64U //bytes

/*********************************************************************/
/*                                                                                                                     */
/*                                                    Global                                                        */
/*                                                                                                                     */
/*********************************************************************/
MS_U8 gu8Hdcp1xKey[DEF_SIZE_OF_HDCP1X_KEY]= {0x00};
static MS_BOOL gbIsKmNewMode = FALSE;

/*********************************************************************/
/*                                                                                                                     */
/*                                                    Functions                                                    */
/*                                                                                                                     */
/*********************************************************************/
/*********************************************************************/
/*                                                                                                                     */
/*                                                    Internal                                                      */
/*                                                                                                                     */
/*********************************************************************/

MS_U16 MHalHdcpRegRead(MS_U32 bank, MS_U16 address)
{
    return HDCPREG(bank, address);
}

void MHalHdcpRegWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_data)
{
    HDCPREG(bank, address) = reg_data;
}

void MHalHdcpRegMaskWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_mask, MS_U16 reg_data)
{
    MS_U16 reg_value;

    reg_value = (HDCPREG(bank, address) & (~reg_mask)) | (reg_data & reg_mask);
    HDCPREG(bank, address) = reg_value;
}

MS_U16 MHalHdcpPMRegRead(MS_U32 bank, MS_U16 address)
{
    return HDCPPMREG(bank, address);
}

void MHalHdcpPMRegWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_data)
{
    HDCPPMREG(bank, address) = reg_data;
}

void MHalHdcpPMRegMaskWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_mask, MS_U16 reg_data)
{
    MS_U16 reg_value;

    reg_value = (HDCPPMREG(bank, address) & (~reg_mask)) | (reg_data & reg_mask);
    HDCPPMREG(bank, address) = reg_value;
}

/*********************************************************************/
/*                                                                                                                     */
/*                                                    External                                                     */
/*                                                                                                                     */
/*********************************************************************/
void MHal_HDCP_HDCP14TxInitHdcp(MS_U8 u8PortIdx)
{
    //Kano has only 1 Tx port
    u8PortIdx &= 0x0F;

    //TBD: get bank offset by port index
    MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0001, 0x8000, 0x8000); // Enable HDCP encryption
    MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x001C, 0x0000); //[4]: 1: km new mode; 0: km old mode
}

void MHal_HDCP_HDCP14TxLoadKey(MS_U8* pu8KeyData, MS_BOOL bUseKmNewMode)
{
    gbIsKmNewMode = bUseKmNewMode;
    if (pu8KeyData != NULL)
        memcpy(gu8Hdcp1xKey, pu8KeyData, DEF_SIZE_OF_HDCP1X_KEY);
}

void MHal_HDCP_HDCP14TxSetAuthPass(MS_U8 u8PortIdx)
{
    //Kano has only 1 Tx port
    u8PortIdx &= 0x0F;

    //TBD: get bank offset by port index
    MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x000F, 0x000C);
}

void MHal_HDCP_HDCP14TxEnableENC_EN(MS_U8 u8PortIdx, MS_BOOL bEnable)
{
    //Kano has only 1 Tx port
    u8PortIdx &= 0x0F;

    if (bEnable == TRUE)
        MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x0008, 0x0008);
    else
        MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x0008, 0x0000);
}

void MHal_HDCP_HDCP14TxProcessAn(MS_U8 u8PortIdx, MS_BOOL bUseInternalAn, MS_U8* pu8An)
{
    MS_U8 i = 0;
    //Kano has only 1 Tx port
    u8PortIdx &= 0x0F;

    if (bUseInternalAn == TRUE)
    {
        MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0001, 0x0100, 0x0100);
        MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x0002, 0x0002);
        MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x0002, 0x0000);

        //MsOS_DelayTask(1);

        for ( i = 0; i < 4; i++ )
        {
            *(pu8An + 2*i) = MHalHdcpRegRead(DEF_HDCP14_TX_REG_BANK, 0x0008 + i) & 0x00FF;
            *(pu8An + 2*i + 1) = (MHalHdcpRegRead(DEF_HDCP14_TX_REG_BANK, 0x0008 + i) & 0xFF00) >> 8;
        }
    }
    else
    {
        MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0001, 0x0100, 0x0000);

        for ( i = 0; i < 4; i++ )
        {
            MHalHdcpRegWrite(DEF_HDCP14_TX_REG_BANK, 0x0008 + i, ((*(pu8An + 2*i + 1) << 8) | (*(pu8An + 2*i))));
        }
    }
}


void MHal_HDCP_HDCP14TxGetAKSV(MS_U8 u8PortIdx, MS_U8* pu8Aksv)
{
    MS_U8 u8ByteCnt = 0;

    //Kano has only 1 Tx port
    u8PortIdx &= 0x0F;

    for (u8ByteCnt = 0; u8ByteCnt < 5; u8ByteCnt++ )
    {
        *(pu8Aksv + u8ByteCnt) = gu8Hdcp1xKey[u8ByteCnt];
    }
}


MS_BOOL MHal_HDCP_HDCP14TxCompareRi(MS_U8 u8PortIdx, MS_U8* pu8SinkRi)
{
    MS_BOOL bRet = FALSE;
    MS_U16 u16SrcRi = 0x0000;
    //Kano has only 1 Tx port
    u8PortIdx &= 0x0F;

    do
    {
        u16SrcRi = MHalHdcpRegRead(DEF_HDCP14_TX_REG_BANK, 0x0000);

        if (u16SrcRi == *(MS_U16*)pu8SinkRi)
        {
            bRet = TRUE;
            break;
        }

        u16SrcRi = MHalHdcpRegRead(DEF_HDCP14_TX_REG_BANK, 0x0000);

        if (u16SrcRi == *(MS_U16*)pu8SinkRi)
        {
            bRet = TRUE;
            break;
        }

    } while (FALSE);

    return bRet;
}


void MHal_HDCP_HDCP14TxConfigMode(MS_U8 u8PortIdx, MS_U8 u8Mode)
{
    //Kano has only 1 Tx port
    u8PortIdx &= 0x0F;

    MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0001, 0x0E00, u8Mode << 8);
}


void MHal_HDCP_HDCP14TxGenerateCipher(MS_U8 u8PortIdx, MS_U8* pu8Bksv)
{
#define DEF_HDCP1X_KEY_OFFSET 8U

    MS_U8 u8Lm[7] = {0};
    MS_U8 u8ByteCnt = 0;
    MS_U8 u8BitCnt = 0;
    MS_U8 u8LmCnt = 0;
    MS_U8 u8CarryBit = 0;
    MS_U8 u8Seed = 0;
    MS_U8 u8Tmp = 0;
    MS_U16 u16Offset = 0;

    for (u8ByteCnt = 0; u8ByteCnt < 5; u8ByteCnt++)
    {
        for (u8BitCnt = 0; u8BitCnt < 8; u8BitCnt++)
        {
            if (*(pu8Bksv + u8ByteCnt) & (1 << u8BitCnt))
            {
                u8CarryBit = 0;
                u16Offset = (u8ByteCnt * 8 + u8BitCnt) * 7 + DEF_HDCP1X_KEY_OFFSET;

                for (u8LmCnt = 0; u8LmCnt < 7; u8LmCnt++)
                {
                    u8Seed = (u8LmCnt + gu8Hdcp1xKey[7]) % 7;
                    u8Tmp = gu8Hdcp1xKey[u16Offset + u8LmCnt] ^ gu8Hdcp1xKey[u8Seed];

                    u8Lm[u8LmCnt] = u8Lm[u8LmCnt] + u8Tmp + u8CarryBit;
                    if (((u8CarryBit == 0) && (u8Lm[u8LmCnt] >= u8Tmp)) || ((u8CarryBit == 1) && (u8Lm[u8LmCnt] > u8Tmp)))
                        u8CarryBit = 0;
                    else
                        u8CarryBit = 1;
                }
            }
        }
    }

    u8Tmp = gu8Hdcp1xKey[288];

    for (u8LmCnt = 0; u8LmCnt < 7; u8LmCnt++)
    {
        if (u8LmCnt < 6)
            u8Lm[u8LmCnt] = u8Lm[u8LmCnt] ^ u8Tmp;
        else
            u8Lm[u8LmCnt] = u8Lm[u8LmCnt] ^ (~u8Tmp);

        if (u8LmCnt % 2 != 0)
            MHalHdcpRegWrite(DEF_HDCP14_TX_REG_BANK, 0x0004 + (u8LmCnt/2), (u8Lm[u8LmCnt] << 8) | u8Lm[u8LmCnt - 1]);
    }

    MHalHdcpRegWrite(DEF_HDCP14_TX_REG_BANK, 0x0007, (u8Tmp << 8) | u8Lm[6]);

#undef DEF_HDCP1X_KEY_OFFSET

}

MS_BOOL MHal_HDCP_HDCP14TxProcessR0(MS_U8 u8PortIdx)
{
    MS_U8 u8Cnt = 0;

    MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x000F, 0x0000);
    MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x000F, 0x0001);
    MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, 0x0002, 0x000F, 0x0000);

    while (u8Cnt-- && !(MHalHdcpRegRead(DEF_HDCP14_TX_REG_BANK, 0x0002) & 0x0100));

    return ((u8Cnt == 0) ? FALSE : TRUE);
}

void MHal_HDCP_HDCP14TxGetM0(MS_U8 u8PortIdx, MS_U8* pu8M0)
{
#define DEF_HDCP1X_M0_SIZE 8
    unsigned char u8DataCnt = 0;
    MS_U16 u16BKOffset = 0x00;
    MS_U16 u16RegVal = 0x00;

    u8PortIdx &= 0x0F;

    for ( u8DataCnt = 0; u8DataCnt < (DEF_HDCP1X_M0_SIZE>>1); u8DataCnt++ )
    {
        u16RegVal = MHalHdcpRegRead(DEF_HDCP14_TX_REG_BANK + u16BKOffset, 0x0C + u8DataCnt);
        *(pu8M0 + 2*u8DataCnt) = (MS_U8)(u16RegVal & 0x00FF);
        *(pu8M0 + 2*u8DataCnt + 1) = (MS_U8)((u16RegVal & 0xFF00)>>8);

    }
#undef DEF_HDCP1X_M0_SIZE
}

void MHal_HDCP_HDCP14GetM0(MS_U8 u8PortIdx, MS_U8 *pu8Data)
{
    MS_U8 cnt = 0x00;
    MS_U16 u16BKOffset = 0x00;

    u8PortIdx &= 0x0F;
    //Kano only has 1 Tx port

    for ( cnt = 0; cnt < (DEF_HDCP14_M0_SIZE >> 4); cnt++ )
    {
        MS_U16 u16tmpData = 0x00;

        u16tmpData = MHalHdcpRegRead(DEF_HDCP14_RX_REG_BANK + u16BKOffset, 0x0E + cnt);

        *(pu8Data + cnt*2) = (MS_U8)(u16tmpData & 0x00FF);
        *(pu8Data + cnt*2 + 1) = (MS_U8)((u16tmpData & 0xFF00) >> 8);
    }
}

void MHal_HDCP_SetBank(MS_U32 u32NonPmBankAddr, MS_U32 u32PmBankAddr)
{
    HalHDCPLogInfo("u32NonPmBankAddr = 0x%X, u32PmBankAddr = 0x%X\r\n", (unsigned int)u32NonPmBankAddr, (unsigned int)u32PmBankAddr);
    _gHDCPRegBase = u32NonPmBankAddr;
    _gHDCPPMRegBase = u32PmBankAddr;
}

void MHal_HDCP_HDCP2TxInit(MS_U8 u8PortIdx, MS_BOOL bEnable)
{
    MS_U16 u16BKOffset = 0x00;

    MHalHdcpRegMaskWrite(DEF_HDCP22_TX_REG_BANK + u16BKOffset, 0x0000, 0x11, bEnable ? 0x11 : 0x00); // bit 0: enable hdcp22; bit 4: enable EESS
    if (bEnable)
    {
        MHalHdcpRegMaskWrite(DEF_HDCP22_TX_REG_BANK + u16BKOffset, 0x0000, 0x02, 0x02); //reset hdcp22 FSM
        MHalHdcpRegMaskWrite(DEF_HDCP22_TX_REG_BANK + u16BKOffset, 0x0000, 0x02, 0x00);
    }
}

void MHal_HDCP_HDCP2TxEnableEncrypt(MS_U8 u8PortIdx, MS_BOOL bEnable)
{
    MS_U16 u16BKOffset = 0x00;

    MHalHdcpRegMaskWrite(DEF_HDCP22_TX_REG_BANK + u16BKOffset, 0x0000, 0x04, bEnable ? 0x04 : 0x00); //bit 2: authentication pass
    MHalHdcpRegMaskWrite(DEF_HDCP22_TX_REG_BANK + u16BKOffset, 0x0000, 0x08, bEnable ? 0x08 : 0x00); //bit 3: enable hdcp22 to issue encryption enable signal
}

void MHal_HDCP_HDCP2TxFillCipherKey(MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8KsXORLC128)
{
    MS_U8 cnt = 0;
    MS_U16 u16BKOffset = 0x00;
    //MS_U16 u16RegOffset = 0x00;

    //Kano only has 1 Tx port

    //Ks^LC128
    for ( cnt = 0; cnt < (DEF_SIZE_OF_KSXORLC128>>1); cnt++)
        MHalHdcpRegWrite(DEF_HDCP22_TX_KEY_REG_BANK + u16BKOffset, 0x60 + (DEF_SIZE_OF_KSXORLC128 >> 1) - 1 - cnt, *(pu8KsXORLC128 + cnt*2 + 1)|(*(pu8KsXORLC128 + cnt*2)<<8));

    //Riv
    for ( cnt = 0; cnt < (DEF_SIZE_OF_RIV>>1); cnt++)
        MHalHdcpRegWrite(DEF_HDCP22_TX_KEY_REG_BANK + u16BKOffset, 0x68 + (DEF_SIZE_OF_RIV >> 1) - 1 - cnt, *(pu8Riv + cnt*2 + 1)|(*(pu8Riv + cnt*2)<<8));

}

void MHal_HDCP_HDCP2TxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State)
{
    MS_U16 u16BKOffset = 0x00;
    //MS_U16 u16RegOffset = 0x00;

    //Kano only has 1 Tx port

    *pu8State = MHalHdcpRegRead(DEF_HDCP22_TX_REG_BANK + u16BKOffset, 0x00) & 0x0C;
}

void MHal_HDCP_HDCP2RxProcessCipher(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey)
{
    MS_U8 cnt = 0;
    MS_U16 u16BKOffset = 0x00;
    MS_U16 u16RegOffset = 0x00;

    //Kano only has 1 Rx port

    HalHDCPLogDebug("%s:: PortIdx : BKOffset : RegOffset = 0x%X : 0x%X : 0x%X\r\n", __FUNCTION__, u8PortIdx, u16BKOffset, u16RegOffset);

    //Ks^LC128
    for ( cnt = 0; cnt < (DEF_SIZE_OF_KSXORLC128>>1); cnt++)
        MHalHdcpRegWrite(DEF_HDCP22_RX_KEY_REG_BANK + u16BKOffset, u16RegOffset + 0x30 + (DEF_SIZE_OF_KSXORLC128 >> 1) - 1 - cnt, *(pu8ContentKey + cnt*2 + 1)|(*(pu8ContentKey + cnt*2)<<8));

    //Riv
    for ( cnt = 0; cnt < (DEF_SIZE_OF_RIV>>1); cnt++)
        MHalHdcpRegWrite(DEF_HDCP22_RX_KEY_REG_BANK + u16BKOffset, u16RegOffset + 0x38 + (DEF_SIZE_OF_RIV >> 1) - 1 - cnt, *(pu8Riv + cnt*2 + 1)|(*(pu8Riv + cnt*2)<<8));

    //Set SKE successful
    MHalHdcpRegMaskWrite(DEF_HDCP22_RX_REG_BANK + u16BKOffset, 0x4E, 0x0001, 0x0001);
}

void MHal_HDCP_HDCP2RxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State)
{
    MS_U16 u16BKOffset = 0x00;
    //MS_U16 u16RegOffset = 0x00;

    ////Kano only has 1 Rx port

    *pu8State = MHalHdcpRegRead(DEF_HDCP22_RX_REG_BANK + u16BKOffset, 0x4E) & 0x01;
}

MS_U32 MHal_HDCP_HDCP1TxEncrytionStatus(MS_U8 u8PortIdx, MS_U8 u8SetStatusFlag, MS_U32 u32SetStatus)
{
    MS_U32 u32GetStatus = 0;

    if(u8SetStatusFlag) // Set HDCP1 encrytion status
    {
        MHalHdcpRegMaskWrite(DEF_HDCP14_TX_REG_BANK, REG_HDCP14_TX_02_L, BIT(3), u32SetStatus? BIT(3): 0); //bit 3: enable hdcp14 to issue encryption enable signal
    }

    // Get HDCP1 encrytion status
    if(MHalHdcpRegRead(DEF_HDCP14_TX_REG_BANK, REG_HDCP14_TX_02_L) &BIT(3))
    {
        u32GetStatus = TRUE;
    }

    return u32GetStatus;
}

MS_U32 MHal_HDCP_HDCP2TxEncrytionStatus(MS_U8 u8PortIdx, MS_U8 u8SetStatusFlag, MS_U32 u32SetStatus)
{
    MS_U32 u32GetStatus = 0;

    if(u8SetStatusFlag) // Set HDCP2 encrytion status
    {
        MHalHdcpRegMaskWrite(DEF_HDCP22_TX_REG_BANK, REG_HDCP22_TX_00_L, BIT(3), u32SetStatus? BIT(3): 0); //bit 3: enable hdcp22 to issue encryption enable signal
    }

    // Get HDCP2 encrytion status
    if(MHalHdcpRegRead(DEF_HDCP22_TX_REG_BANK, REG_HDCP22_TX_00_L) &BIT(3))
    {
        u32GetStatus = TRUE;
    }

    return u32GetStatus;
}

MS_U32 MHal_HDCP_HDCPTxHDMIStatus(MS_U8 u8PortIdx, MS_U8 u8SetStatusFlag, MS_U32 u32SetStatus)
{
    MS_U32 u32GetStatus = 0;

    if(u8SetStatusFlag) // Set HDNI status
    {
        MHalHdcpRegMaskWrite(DEF_HDMITX_PHY_REG_BANK, REG_HDMITX_PHY_39_L, 0xFFFF, u32SetStatus? 0xF000: 0xFFFF);
        MHalHdcpRegMaskWrite(DEF_HDMITX_PHY_REG_BANK, REG_HDMITX_PHY_2E_L, 0xE800, u32SetStatus? 0xE800: 0x0000);
    }

    // Get HDNI status
    if((MHalHdcpRegRead(DEF_HDMITX_PHY_REG_BANK, REG_HDMITX_PHY_2E_L) &0xE800) == 0xE800)
    {
        u32GetStatus = TRUE;
    }

    return u32GetStatus;
}

#endif //#ifndef HAL_HDCP_C
