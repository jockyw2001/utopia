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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// [mhal_hdmi.c]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#define  MHAL_HDMI_C

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia_dapi.h"
#include "xc_Analog_Reg.h"

#include "drvXC_IOPort.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_dvi_atop.h"//alex_tung
#include "hwreg_pm_sleep.h"//alex_tung
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "drv_sc_ip.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "mvideo_context.h"
#include "drvXC_HDMI_Internal.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "mhal_hdmi.h"
#include "apiHDMIRx_private.h"

#include "hwreg_mhl.h"

#define PM_REG_READ(reg)            PM_R1BYTE(reg, 7:0)
#define PM_REG_WRITE(reg, data)     PM_W1BYTE(reg, data, 7:0)
#define MST_XTAL_CLOCK_HZ           12000000UL
//==============================================================================
// For MCU access HDCP key SRAM issue.
// Need to read timer once to cover the period of HDCP busy bit's transition.
#define TIMER_0_CNT_REG             0x3028//BIT0-BIT31, RO
//==============================================================================
// HDMI Black Level
#define HDMI_BLACK_LEVEL_HIGH_YC_OFFSET     0x00F0
#define HDMI_BLACK_LEVEL_HIGH_YC_GAIN       0x403B

#define HDMI_BLACK_LEVEL_LOW_YC_OFFSET      0x00E0
#define HDMI_BLACK_LEVEL_LOW_YC_GAIN        0x4045

#define msg_hdmi(x) //x

//==============================================================================
/******************************************************************************/
/// for Patch Flag
/******************************************************************************/
MS_U8 ucBKSV[5] = {0, 0, 0, 0, 0};
//**************************************************************************
//  [Function Name]:
//                  _Hal_HDMI_GetMHLCableDetect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_HDMI_GetMHLCableDetect(void)
{
    return ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_HDMI_GetMHLAttachPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_HDMI_GetMHLAttachPort(MS_U8 ucPortSelect, MS_U8 ucMHLSupportPath)
{
    MS_BOOL bMHLAttach = FALSE;

    if(_Hal_HDMI_GetMHLCableDetect())
    {
        switch(ucPortSelect)
        {
            case INPUT_PORT_DVI0:
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI0)
                {
                    bMHLAttach = TRUE;
                }
                
                break;

            case INPUT_PORT_DVI2:
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI2)
                {
                    bMHLAttach = TRUE;
                }

                break;

            case INPUT_PORT_DVI3:
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI3)
                {
                    bMHLAttach = TRUE;
                }
                
                break;

            default:

                break;
        };
    }
    
    return bMHLAttach;
}

//****************************************************************
// Initial HDCP22 relative register, structure...etc
//****************************************************************
void Hal_HDCP22_PortInit(MS_U8 ucPortIdx)
{
	MS_U32 dwBKOffset = 0;
	MS_U32 dwBKOffset22 = 0;
	
	switch(ucPortIdx)
	{
		default:
		case 0: dwBKOffset = 0x0000; break; // BK 0x110A60
		case 1: dwBKOffset = 0x2900; break; // BK 0x113360
		case 2: dwBKOffset = 0x2B00; break; // BK 0x113560
		case 3: dwBKOffset = 0x2D00; break; // BK 0x113760
	}

	switch(ucPortIdx)
	{
		default:
		case 0: dwBKOffset22 = 0x0000; break; // BK 0x162A00
		case 1: dwBKOffset22 = 0x0100; break; // BK 0x162B00
		case 2: dwBKOffset22 = 0x0200; break; // BK 0x162C00
		case 3: dwBKOffset22 = 0x0300; break; // BK 0x162D00
	}

	printf("[HDCP22] PortInit(), port idx = %d\r\n", ucPortIdx);
	
    W2BYTEMSK(REG_HDCP_19_L + dwBKOffset, BIT(15)|BIT(1), BIT(15)|BIT(1)); //[15]: Enable CPU write; [1]: enable xiu2hdcpkey
    W2BYTEMSK(REG_HDCP_00_L + dwBKOffset, 0, BIT(1)); //[1]: disable SRAM read
	
    //wirte 74Ram for HDCP22 version
    W2BYTEMSK(REG_HDCP_17_L + dwBKOffset, BIT(10), BIT(10)); //HDCP enable for DDC
    W2BYTEMSK(REG_HDCP_19_L + dwBKOffset, BIT(15), BMASK(15:14)); //write enable

    W2BYTEMSK(REG_HDCP_17_L + dwBKOffset, 0x50, BMASK(9:0)); //CPU r/w address
    W2BYTEMSK(REG_HDCP_19_L + dwBKOffset, BIT(5), BIT(5)); //HDCP load address pulse

    W2BYTEMSK(REG_HDCP_18_L + dwBKOffset, 0x04, BMASK(7:0)); //bit2: HDCP22 version
    W2BYTEMSK(REG_HDCP_19_L + dwBKOffset, BIT(4), BIT(4)); //HDCP data write port pulse generate

    //clear RxStatus
    W2BYTEMSK(REG_HDCP22_P0_31_L + dwBKOffset22, 0x00, BMASK(9:0)); //reg_message_length
    W2BYTEMSK(REG_HDCP22_P0_31_L + dwBKOffset22, BIT(15), BIT(15)); //reg_message_length_update

    //clear hdcp_enable cipher bit
    W2BYTEMSK(REG_HDCP22_P0_0E_L + dwBKOffset22, 0, BIT(0));
	
    //clear xiu status
    W2BYTEMSK(REG_HDCP22_P0_34_L + dwBKOffset22, 0x3F, 0x3F);
    W2BYTEMSK(REG_HDCP22_P0_34_L + dwBKOffset22, 0x00, 0x3F);
}

//****************************************************************
// Check If there is any message received;
// @param ucPortIdx	[IN] HDMI port index
//****************************************************************
MS_BOOL Hal_HDCP22_PollingWriteDone(MS_U8 ucPortIdx)
{	
    MS_BOOL bWriteDoneFlag = FALSE;

    switch(ucPortIdx)
    {
        case 0:
            if(R2BYTE(REG_HDCP22_P0_34_L) & BIT(2)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P0_34_L, BIT(4)| BIT(2)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP_71_L, 0);

                bWriteDoneFlag = TRUE;
            }
            break;

        case 1:
            if(R2BYTE(REG_HDCP22_P1_34_L) & BIT(2)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P1_34_L, BIT(4)| BIT(2)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP1_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP1_71_L, 0);

                bWriteDoneFlag = TRUE;
            }
            break;

        case 2:
            if(R2BYTE(REG_HDCP22_P2_34_L) & BIT(2)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P2_34_L, BIT(4)| BIT(2)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP2_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP2_71_L, 0);

                bWriteDoneFlag = TRUE;
            }
            break;

        case 3:
            if(R2BYTE(REG_HDCP22_P3_34_L) & BIT(2)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P3_34_L, BIT(4)| BIT(2)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP3_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP3_71_L, 0);

                bWriteDoneFlag = TRUE;
            }
            break;

        default:

            break;
    };

    return bWriteDoneFlag;
}

//****************************************************************
// Check If Tx already finish its read process
// @param ucPortIdx	[IN] HDMI port index
//****************************************************************
MS_BOOL Hal_HDCP22_PollingReadDone(MS_U8 ucPortIdx)
{	
    MS_BOOL bReadDoneFlag = FALSE;

    switch(ucPortIdx)
    {
        case 0:
            if(R2BYTE(REG_HDCP22_P0_34_L) & BIT(3)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P0_34_L, BIT(3)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP_71_L, 0);

                bReadDoneFlag = TRUE;
            }
            break;

        case 1:
            if(R2BYTE(REG_HDCP22_P1_34_L) & BIT(3)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P1_34_L, BIT(3)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP1_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP1_71_L, 0);

                bReadDoneFlag = TRUE;
            }
            break;

        case 2:
            if(R2BYTE(REG_HDCP22_P2_34_L) & BIT(3)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P2_34_L, BIT(3)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP2_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP2_71_L, 0);

                bReadDoneFlag = TRUE;
            }
            break;

        case 3:
            if(R2BYTE(REG_HDCP22_P3_34_L) & BIT(3)) //check write done
            {
                //clear status
                W2BYTE(REG_HDCP22_P3_34_L, BIT(3)); //clear write done, write start
                W2BYTE(REG_DVI_ATOP3_71_L, BIT(14));
                W2BYTE(REG_DVI_ATOP3_71_L, 0);

                bReadDoneFlag = TRUE;
            }
            break;

        default:

            break;
    };

    return bReadDoneFlag;
}

//****************************************************************
// Enable or Disable Cipher process
// @param ucPortType	[IN] HDCP type, ex: HDMI or Miracast
// @param ucPortIdx	[IN] HDMI port index
// @param bIsEnable	[IN] Enable/Disable Cipher
//****************************************************************
void Hal_HDCP22_EnableCipher(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable)
{
	MS_U32 dwBKOffset22 = 0;

	printf("[HDCP22] EnableCipher(), isEnable = %d\r\n", bIsEnable);

	switch(ucPortIdx)
	{
		default:
		case 0: dwBKOffset22 = 0x0000; break; // BK 0x162A00
		case 1: dwBKOffset22 = 0x0100; break; // BK 0x162B00
		case 2: dwBKOffset22 = 0x0200; break; // BK 0x162C00
		case 3: dwBKOffset22 = 0x0300; break; // BK 0x162D00
	}

	W2BYTEMSK(REG_HDCP22_P0_0E_L + dwBKOffset22, bIsEnable? BIT(0): 0, BIT(0));
}

//****************************************************************
// Fetch message content
// @param ucPortIdx	[IN] HDMI port index
// @param pucRiv	[IN] Data pointer of riv
// @param pucSessionKey	[IN] Data pointer of ks^LC128
//****************************************************************
void Hal_HDCP22_FillCipherKey(MS_U8 ucPortIdx, MS_U8 *pucRiv, MS_U8 *pucSessionKey)
{
#define EN_DBG_CIPHER_KEY 0
	MS_U8 uctemp = 0;
	//MS_U8 ucRegOffset = 0;
	MS_U32 dwBKOffset22 = 0;

	printf("[HDCP22] FillCipherKey\r\n");

	if(ucPortIdx < 10) //to avoid overflow a byte size after multiple with 0x0C
	{
		switch(ucPortIdx)
		{
			default:
			case 0: dwBKOffset22 = 0x0000; break; // BK 0x162A00
			case 1: dwBKOffset22 = 0x0100; break; // BK 0x162B00
			case 2: dwBKOffset22 = 0x0200; break; // BK 0x162C00
			case 3: dwBKOffset22 = 0x0300; break; // BK 0x162D00
		}
	
		//fill riv: size 8 bytes
#if (EN_DBG_CIPHER_KEY == 1)
		printf("\r\n[HDCP22]Riv\r\n");
#endif

		for(uctemp = 0; uctemp < 8; uctemp++)
		{
			MDrv_WriteByteMask(REG_HDCP22_P0_08_L + dwBKOffset22 + 8 - uctemp - 1, *(MS_U8*)(pucRiv + uctemp), BMASK(7:0));

#if (EN_DBG_CIPHER_KEY == 1)
			printf("0x%02X : ", *(MS_U8*)(pucRiv + uctemp));
			printf("(0x%x)0x%02X\r\n", REG_HDCP22_P0_08_L + dwBKOffset22 + 8 - uctemp - 1, MDrv_ReadByte(REG_HDCP22_P0_08_L + dwBKOffset22 + 8 - uctemp - 1));
#endif
		}

		//fill ks^LC128: size 16 bytes
#if (EN_DBG_CIPHER_KEY == 1)
		printf("\r\n[HDCP22]SessionKey\r\n");
#endif

		for(uctemp = 0; uctemp < 16; uctemp++)
		{
			MDrv_WriteByteMask(REG_HDCP22_P0_00_L + dwBKOffset22 + 16 - uctemp - 1, *(MS_U8*)(pucSessionKey + uctemp), BMASK(7:0));

#if (EN_DBG_CIPHER_KEY == 1)
			printf("0x%02X : ", *(MS_U8*)(pucSessionKey + uctemp));
			printf("(0x%x)0x%02X\r\n", REG_HDCP22_P0_00_L + dwBKOffset22 + 16 - uctemp - 1, MDrv_ReadByte(REG_HDCP22_P0_00_L + dwBKOffset22 + 16 - uctemp - 1));
#endif
		}

		//enable cipher
		printf("\r\n[HDCP22]Enable Cipher\r\n");

		W2BYTEMSK(REG_HDCP22_P0_0E_L + dwBKOffset22, BIT(0), BIT(0));
	}
	else
	{
		printf("[HDCP22]Hal_HDCP22_FillCipherKey:: Invalid Port Index!!\r\n");
	}
#undef EN_DBG_CIPHER_KEY
}

//****************************************************************
// Fetch message content
// @param ucPortIdx	[IN] HDMI port index
// @param pucData	[IN] Data pointer to store received message
// @param dwDataLen	[IN] message length
//****************************************************************
void Hal_HDCP22_FetchMsg(MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen)
{
	MS_U32 dwBKOffset = 0;
	MS_U8 ucRegOffset = ( ucPortIdx * 4 );
	MS_U32 ultemp = 0;
	
	switch(ucPortIdx)
	{
		default:
		case 0: dwBKOffset = 0x0000; break; // BK 0x110A60
		case 1: dwBKOffset = 0x2900; break; // BK 0x113360
		case 2: dwBKOffset = 0x2B00; break; // BK 0x113560
		case 3: dwBKOffset = 0x2D00; break; // BK 0x113760
	}

    W2BYTEMSK(REG_HDCP_19_L + dwBKOffset, BIT(1), BIT(15)|BIT(1)); //[15]: Enable CPU read; [1]: enable xiu2hdcpkey
    W2BYTEMSK(REG_HDCP_00_L + dwBKOffset, 0, BIT(1)); //[1]: disable SRAM read

	W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, 0x00, BMASK(9:0)); //start addr

	for(ultemp = 0; ultemp < dwDataLen; ultemp++)
    {
        *(pucData + ultemp) = (R2BYTE(REG_HDCPKEY_0E_L + 2*(ucRegOffset/0x04)) &BMASK(7:0));
    }
}

//****************************************************************
// Receive message and transmitt message to TEE
// @param ucPortIdx	[IN] HDMI port index
//****************************************************************
MS_BOOL Hal_HDCP22_RecvMsg(MS_U8 ucPortIdx, MS_U8 *ucMsgData)
{
    MS_BOOL bReceiveFlag = FALSE;
    MS_U16 ustemp = 0;
    MS_U32 dwMsgLen = 0;
    MS_U32 dwBKOffset22 = 0;

    switch(ucPortIdx)
    {
        default:
        case 0: dwBKOffset22 = 0x0000; break; // BK 0x162A00
        case 1: dwBKOffset22 = 0x0100; break; // BK 0x162B00
        case 2: dwBKOffset22 = 0x0200; break; // BK 0x162C00
        case 3: dwBKOffset22 = 0x0300; break; // BK 0x162D00
    }

    //fetch msg length
    dwMsgLen = R2BYTE(REG_HDCP22_P0_32_L + dwBKOffset22) & BMASK(9:0);

    if(dwMsgLen != 0x00)
    {
        //allocate and init buffer
        for(ustemp = 0; ustemp < HDMI_HDCP22_MESSAGE_LENGTH; ustemp++)
        {
            ucMsgData[ustemp] = 0x00;
        }

        ucMsgData[HDMI_HDCP22_MESSAGE_LENGTH] = dwMsgLen;

        //fetch msg
        Hal_HDCP22_FetchMsg(ucPortIdx, ucMsgData, dwMsgLen);

        //printf("[HDCP22] PrtIdx = %d, msgID = 0x%x, len= %ld\r\n", ucPortIdx, ucMsgData[0], dwMsgLen);

        bReceiveFlag = TRUE;
    }

    return bReceiveFlag;
}

//****************************************************************
// Sending message back to TX
// @param ucPortType	[IN] HDCP type, ex: HDMI or Miracast
// @param ucPortIdx	[IN] HDMI port index
// @param pucData	[IN] received message from HDMI port
// @param dwDataLen	[IN] message lenght
// @param pDummy	[IN] dummy pointer for API layer structure
//****************************************************************
void Hal_HDCP22_SendMsg(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy, MS_U8* ubRecIDListSetDone)
{
	MS_U8 ucRegOffset = 0;
    MS_U32 ultemp = 0;
	MS_U32 dwBKOffset = 0;
	MS_U32 dwBKOffset22 = 0;

	printf("[HDCP22] Send, PrtIdx = %d\r\n", ucPortIdx);
	ucRegOffset = ucPortIdx * 0x04;

    if( (*(pucData) == 0x0C) || (*(pucData) == 0x07) )
    {
        //W2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset, BIT(3)); //clear read done
        Hal_HDCP22_PollingReadDone(ucPortIdx);
    }
	
	switch(ucPortIdx)
	{
		default:
		case 0: dwBKOffset = 0x0000; break; // BK 0x110A60
		case 1: dwBKOffset = 0x2900; break; // BK 0x113360
		case 2: dwBKOffset = 0x2B00; break; // BK 0x113560
		case 3: dwBKOffset = 0x2D00; break; // BK 0x113760
	}
	
	switch(ucPortIdx)
	{
		default:
		case 0: dwBKOffset22 = 0x0000; break; // BK 0x162A00
		case 1: dwBKOffset22 = 0x0100; break; // BK 0x162B00
		case 2: dwBKOffset22 = 0x0200; break; // BK 0x162C00
		case 3: dwBKOffset22 = 0x0300; break; // BK 0x162D00
	}

    W2BYTEMSK(REG_HDCP_19_L + dwBKOffset, BIT(15)|BIT(1), BIT(15)|BIT(1)); //[15]: Enable CPU write; [1]: enable xiu2hdcpkey
    W2BYTEMSK(REG_HDCP_00_L + dwBKOffset, 0, BIT(1)); //[1]: disable SRAM read

	for(ultemp = 0; ultemp < dwDataLen; ultemp++)
	{
		W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp, 0x7FF); //set address
		W2BYTEMSK(REG_HDCPKEY_05_L + ucRegOffset, *(pucData + ultemp), BMASK(7:0));
        W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp, 0x7FF); //set address

        while((R2BYTE(REG_HDCPKEY_0E_L + ucPortIdx * 0x02) &BMASK(7:0)) != *(pucData + ultemp))
        {
            //Wilson: retry, software patch for XIU write fail issue;
            W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp, 0x7FF); //set address
    		W2BYTEMSK(REG_HDCPKEY_05_L + ucRegOffset, *(pucData + ultemp), BMASK(7:0));
            W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp, 0x7FF); //set address
        }
	}

	//patch for CTS
	if(*(pucData) == 0x03) //AKE_Send_Cert
	{
		printf("[HDCP22] check DDC free!!\r\n");

		while((R2BYTE(REG_HDCP_19_L + dwBKOffset) & BIT(10))); //polling DDC free
	}

	W2BYTEMSK(REG_HDCP22_P0_31_L + dwBKOffset22, BIT(15)|dwDataLen, BIT(15)|0x7FF); // write length & length update
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_GetEncryptionFlag()
//  [Description]
//                  Get input is encryption or not
//  [Arguments]:
//
//  [Return]:
//                 TRUE: encryption, FALSE: no encryption
//**************************************************************************
MS_BOOL Hal_HDCP_GetEncryptionFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bEncryptionFlag = FALSE;
    MS_U16 ulDTOPBankOffset = 0;

    switch(enInputPortType)
	{
		default:
		case 0: ulDTOPBankOffset = 0x0000; break; // BK 0x110A60
		case 1: ulDTOPBankOffset = 0x2900; break; // BK 0x113360
		case 2: ulDTOPBankOffset = 0x2B00; break; // BK 0x113560
		case 3: ulDTOPBankOffset = 0x2D00; break; // BK 0x113760
	}

    if(R2BYTE(REG_HDCP_1B_L +ulDTOPBankOffset) &BIT(9))
    {
        bEncryptionFlag = TRUE;

        W2BYTE(REG_HDCP_1B_L +ulDTOPBankOffset, BIT(9));
    }

    return bEncryptionFlag;
}

//=========================================================//
// Function : Hal_HDMI_Check_MHL_Attach
// Description: return if current attach interface is MHL
//=========================================================//
MS_BOOL Hal_HDMI_Check_MHL_Attach(void)
{
    return FALSE;
}

//=========================================================//
// Function : Hal_HDMI_Func_Caps
// Description: report which patch function hdmi needs
//=========================================================//
MS_U16 Hal_HDMI_Func_Caps(void)
{
    return 0;
}

/******************************************************************************/
/// for HDCP
/******************************************************************************/
//=========================================================//
// Function : Hal_HDCP_clearflag
// Description: clear hdcp status, must during hpd low
//=========================================================//
void Hal_HDCP_clearflag(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000; break; // BK 0x110A60
        case INPUT_PORT_DVI1:  u16bank_offset = 0x2900;  break; // BK 0x113360
        case INPUT_PORT_DVI3:  u16bank_offset = 0x2B00;  break; // BK 0x113560
        case INPUT_PORT_DVI2:  u16bank_offset = 0x2D00;  break; // BK 0x113760
    }
    HDCP_W2BYTE(REG_HDCP_01_L+u16bank_offset, 0xFF00 );
}

//=========================================================//
// Function : Hal_HDCP_getstatus
// Description: get HDCP status
//=========================================================//
MS_U16 Hal_HDCP_getstatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16hdcp_status, u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000; break; // BK 0x110A60
        case INPUT_PORT_DVI1:  u16bank_offset = 0x2900;  break; // BK 0x113360
        case INPUT_PORT_DVI3:  u16bank_offset = 0x2B00;  break; // BK 0x113560
        case INPUT_PORT_DVI2:  u16bank_offset = 0x2D00;  break; // BK 0x113760
    }
    u16hdcp_status = HDCP_R2BYTE(REG_HDCP_01_L + u16bank_offset);

    return u16hdcp_status;
}

/******************************************************************************/
/// for HDMI
/******************************************************************************/
//=========================================================//
// Function : Hal_HDMI_pullhpd
// Description: Pull DVI Hot-Plug-Detection pin
//=========================================================//
void Hal_HDMI_pullhpd(MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse, MS_U8 ucMHLSupportPath)
{
    if ( !bInverse )
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0://A
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(4)|BIT(0) : 0, BIT(4)|BIT(0));
                break;
            case INPUT_PORT_DVI1://B
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(5)|BIT(1) : 0, BIT(5)|BIT(1));
                break;
            case INPUT_PORT_DVI2://C
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI2)
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, BIT(0) , BIT(8)|BIT(0) );
					PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                }
                else
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(1): 0, BIT(1));
                    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(6)|BIT(2) : 0, BIT(6)|BIT(2));
                }
                break;
            case INPUT_PORT_DVI3://D
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI3)
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, BIT(8) , BIT(8)|BIT(0) );
					PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                }
                else
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(9): 0, BIT(9));
                    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(7)|BIT(3) : 0, BIT(7)|BIT(3));
                }
				break;
            default: // For system first power on with the unknow HDMI port.
                {
					PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? LBMASK : 0, LBMASK);
					if(ucMHLSupportPath != E_INPUT_NOT_SUPPORT_MHL)
					{
					    PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
					}
				    if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI3)
				    {
                        PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(1): 0, BIT(1));
					    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(6)|BIT(2) : 0, BIT(6)|BIT(2));
				    }
					else
					{
                        PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(9): 0, BIT(9));
					    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(7)|BIT(3) : 0, BIT(7)|BIT(3));
					}
            	}
				break;
        }
    }
    else
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0://A
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(4), BIT(4)|BIT(0));
                break;
            case INPUT_PORT_DVI1://B
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(5), BIT(5)|BIT(1));
                break;
            case INPUT_PORT_DVI2://C
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI2)
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, BIT(0) , BIT(8)|BIT(0) );
					PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                }
                else
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(1): 0, BIT(1));
                    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(6), BIT(6)|BIT(2));
                }
                break;
            case INPUT_PORT_DVI3://D
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI3)
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, BIT(8) , BIT(8)|BIT(0) );
					PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                }
                else
                {
                    PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(9): 0, BIT(9));
                    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(7), BIT(7)|BIT(3));
                }
				break;
            default: // For system first power on with the unknow HDMI port.
            	{
					PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BMASK(7:4), LBMASK);
					if(ucMHLSupportPath != E_INPUT_NOT_SUPPORT_MHL)
					{
					    PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
					}
				    if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI3)
				    {
                        PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(1): 0, BIT(1));
					    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(6) , BIT(6)|BIT(2));
				    }
					else
					{
                        PM_W2BYTE(REG_PM_SLEEP_73_L, bHighLow? BIT(9): 0, BIT(9));
					    PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(7) , BIT(7)|BIT(3));
					}
            	}
				break;
        }
    }
}

//=========================================================//
// Function : Hal_HDCP_initproductionkey
// Description:
//=========================================================//
// Aaron comment:  HDCP OESS mode in U3
void Hal_HDCP_initproductionkey( MS_U8 * pu8HdcpKeyData )
{
    MS_U16 i, delay, u16bank_offset=0, u16loop;
    MS_U8 * pu8HdcpKeyData_bk;

    pu8HdcpKeyData_bk = pu8HdcpKeyData;

    // write BKSV/Bcaps into 4 HDCP engines
    for(u16loop=0; u16loop<4; u16loop++)
    {
        switch(u16loop)
        {
            default:
            case 0: u16bank_offset = 0x0000; break; // BK 0x110A60
            case 1: u16bank_offset = 0x2900; break; // BK 0x113360
            case 2: u16bank_offset = 0x2B00; break; // BK 0x113560
            case 3: u16bank_offset = 0x2D00; break; // BK 0x113760
        }

        pu8HdcpKeyData = pu8HdcpKeyData_bk;
        // Bksv
        HDCP_W2BYTEMSK(REG_HDCP_17_L+u16bank_offset, BIT(10), BIT(10)); // [10]: HDCP enable for DDC
        HDCP_W2BYTEMSK(REG_HDCP_19_L+u16bank_offset, BIT(15), BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM

        HDCP_W2BYTEMSK(REG_HDCP_17_L+u16bank_offset, 0x00, BMASK(9:0)); // address
        HDCP_W2BYTEMSK(REG_HDCP_19_L+u16bank_offset, BIT(5), BIT(5)); // trigger latch address
        for ( i = 0; i < 5; i++ )
        {
            ucBKSV[i] = *pu8HdcpKeyData;
            HDCP_W2BYTEMSK(REG_HDCP_18_L+u16bank_offset, *pu8HdcpKeyData, BMASK(7:0)); // data
            HDCP_W2BYTEMSK(REG_HDCP_19_L+u16bank_offset, BIT(4), BIT(4)); // trigger latch data
            delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
            pu8HdcpKeyData += 1;
            while(R2BYTE(REG_HDCP_19_L+u16bank_offset) & BIT(7)); // wait write ready
        }

        // Bcaps = 0x80
        HDCP_W2BYTEMSK(REG_HDCP_17_L+u16bank_offset, 0x40, BMASK(9:0)); // address
        HDCP_W2BYTEMSK(REG_HDCP_19_L+u16bank_offset, BIT(5), BIT(5)); // trigger latch address

        HDCP_W2BYTEMSK(REG_HDCP_18_L+u16bank_offset, 0x80, BMASK(7:0)); // data
        HDCP_W2BYTEMSK(REG_HDCP_19_L+u16bank_offset, BIT(4), BIT(4)); // trigger latch data
        delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
        while(R2BYTE(REG_HDCP_19_L+u16bank_offset) & BIT(7)); // wait write ready
        HDCP_W2BYTEMSK(REG_HDCP_19_L+u16bank_offset, 0, BIT(15)|BIT(14)); // [15]: CPU write disable, [14]: 0: 74 RAM, 1 :HDCP RAM

        // [10:8]: 3'b111 determine Encrp_En during Vblank in DVI mode; [5]:HDCP enable; [0]: EESS mode deglitch Vsync mode
        HDCP_W2BYTEMSK(REG_HDCP_00_L+u16bank_offset, (BIT(10)|BIT(9)|BIT(8)|BIT(5)|BIT(0)), (BIT(10)|BIT(9)|BIT(8)|BIT(5)|BIT(0)));
    }

    // HDCP key
    HDCP_W2BYTEMSK(REG_HDCP_19_L, BIT(15)|BIT(14), BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM
    // burst write from address 0x00
    HDCP_W2BYTEMSK(REG_HDCP_17_L, 0x00, BMASK(9:0)); // address
    HDCP_W2BYTEMSK(REG_HDCP_19_L, BIT(5), BIT(5)); // trigger latch address

    for ( i = 0; i < 284; i++ )
    {
        HDCP_W2BYTEMSK(REG_HDCP_18_L, *pu8HdcpKeyData, BMASK(7:0)); // data
        HDCP_W2BYTEMSK(REG_HDCP_19_L, BIT(4), BIT(4)); // trigger latch data
        delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
        pu8HdcpKeyData += 1;
        while(R2BYTE(REG_HDCP_19_L) & BIT(7)); // wait write ready
    }

    HDCP_W2BYTEMSK(REG_HDCP_19_L, 0, BIT(15)|BIT(14)); // [15]: CPU write disable, [14]: 0: 74 RAM, 1 :HDCP RAM
    // to resolve the coverity issue
    u16loop = delay;
}

//=========================================================//
// Function : Hal_HDCP_ddc_en
// Description:
//=========================================================//
void Hal_HDCP_ddc_en(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable)
{
    MS_U16 u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000; break; // BK 0x110A60
        case INPUT_PORT_DVI1:  u16bank_offset = 0x2900;  break; // BK 0x113360
        case INPUT_PORT_DVI3:  u16bank_offset = 0x2B00;  break; // BK 0x113560
        case INPUT_PORT_DVI2:  u16bank_offset = 0x2D00;  break; // BK 0x113760
    }
    W2BYTEMSK(REG_HDCP_17_L+u16bank_offset, (bEnable ? BIT(10):0), BIT(10)); // HDCP enable for ddc
}

//=========================================================//
// Function : Hal_HDCP_Vsync_end_en
// Description:
//=========================================================//
// Aaron comment: doesn't need in U3
void Hal_HDCP_Vsync_end_en( MS_BOOL bEnalbe )
{
     //do nothing in U3
}

//=========================================================//
// Function : Hal_MHL_TMDS_pkt_reset
// Description: reset the MHL packet status
//=========================================================//
void Hal_MHL_TMDS_pkt_reset(E_MUX_INPUTPORT enInputPortType, HDMI_REST_t breset)
{
    // reset off-line AVMUTE status

    if(breset & REST_AVMUTE)
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0:
                W2BYTEMSK(REG_HDMI2_11_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_HDMI2_11_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI1:
                W2BYTEMSK(REG_HDMI2_11_L, BIT(8), BIT(8));
                W2BYTEMSK(REG_HDMI2_11_L, 0, BIT(8));
                break;
            case INPUT_PORT_DVI3:
                W2BYTEMSK(REG_HDMI2_12_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_HDMI2_12_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI2:
                W2BYTEMSK(REG_HDMI2_12_L, BIT(8), BIT(8));
                W2BYTEMSK(REG_HDMI2_12_L, 0, BIT(8));
                break;
            default:
                break;
        }
    }

    W2BYTEMSK(REG_MHL_TMDS_5F_L, breset, HBMASK);
    MsOS_DelayTask(1);
    W2BYTEMSK(REG_MHL_TMDS_5F_L, 0x0000, HBMASK);
}

//=========================================================//
// Function : Hal_HDMI_hw_reset
// Description: reset the HDMI packet status
//=========================================================//
void Hal_HDMI_pkt_reset(E_MUX_INPUTPORT enInputPortType, HDMI_REST_t breset)
{
    if (Hal_HDMI_Check_MHL_Attach())
    {
        Hal_MHL_TMDS_pkt_reset(enInputPortType, breset);
        return;
    }

    // reset off-line AVMUTE status
    if(breset & REST_AVMUTE)
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0:
                W2BYTEMSK(REG_HDMI2_11_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_HDMI2_11_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI1:
                W2BYTEMSK(REG_HDMI2_11_L, BIT(8), BIT(8));
                W2BYTEMSK(REG_HDMI2_11_L, 0, BIT(8));
                break;
            case INPUT_PORT_DVI3:
                W2BYTEMSK(REG_HDMI2_12_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_HDMI2_12_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI2:
                W2BYTEMSK(REG_HDMI2_12_L, BIT(8), BIT(8));
                W2BYTEMSK(REG_HDMI2_12_L, 0, BIT(8));
                break;
            default:
                break;
        }
    }

    W2BYTEMSK(REG_HDMI_5F_L, breset, HBMASK);
    MsOS_DelayTask(1);
    W2BYTEMSK(REG_HDMI_5F_L, 0x0000, HBMASK);
}

void Hal_HDMI_exit(E_MUX_INPUTPORT enInputPortType)
{
    // Disable all DDC
    W2BYTEMSK(REG_DDC_4A_L, 0 , 0x80);
    W2BYTEMSK(REG_DDC_4B_L, 0 , 0x8080);
    W2BYTEMSK(REG_DDC_4C_L, 0 , 0x80);

    // Disable audio
    Hal_HDMI_audio_output(DISABLE);

    // HPD low
    Hal_HDMI_hpd_en(DISABLE);

    // Disable HDCP
    Hal_HDCP_ddc_en(enInputPortType, DISABLE);

}

//=========================================================//
// Function : Hal_HDMI_init
// Description:
//=========================================================//
void Hal_HDMI_init(MS_BOOL bImmeswitchSupport, MS_U8 ucMHLSupportPath)// void )
{
    MS_U8 i=0;
    MS_U8 u8reg_offset=0x40;
    MS_BOOL bCalibrationFlag = FALSE;

    // DVI setting
    PM_W2BYTE(REG_PM_SLEEP_4A_L, 0xF000, 0xF000); // power off DVI off-line detection clock

    if(bImmeswitchSupport)
    {
        // Power up all clocks for DVI/HDMI/HDCP
        W2BYTE(REG_DVI_ATOP_06_L, 0x0000); // enable DVI0 all clock power
        W2BYTE(REG_DVI_ATOP1_06_L, 0x0000); // enable DVI1 all clock power
        W2BYTE(REG_DVI_ATOP2_06_L, 0x0000); // enable DVI2 all clock power
        W2BYTE(REG_DVI_ATOP3_06_L, 0x0000); // enable DVI3 all clock power

        PM_W2BYTE(REG_PM_SLEEP_4A_L, 0x0000, 0xFFFF); // DM and DVI clock power on
        PM_W2BYTE(REG_PM_SLEEP_4B_L, 0x0000, 0x00FF); // DVI clock power on //Fix Coverity issue.
        PM_W2BYTE(REG_PM_SLEEP_4B_L, 0x0000, 0xFF00); // DVI clock power on //Fix Coverity issue.

        W2BYTE(REG_DVI_ATOP_60_L, 0x0000); // enable DVI0 PLL power
        W2BYTE(REG_DVI_ATOP1_60_L, 0x0000); // enable DVI1 PLL power
        W2BYTE(REG_DVI_ATOP2_60_L, 0x0000); // enable DVI2 PLL power
        W2BYTE(REG_DVI_ATOP3_60_L, 0x0000); // enable DVI3 PLL power

        MDrv_WriteByteMask(REG_DVI_ATOP_7F_L, 0x00, 0x3F); //power on for Port A DVI DPLPHI/DPLPHQ
        MDrv_WriteByteMask(REG_DVI_ATOP1_74_L, 0x00, 0x3F); //power on for Port B DVI DPLPHI/DPLPHQ
        MDrv_WriteByteMask(REG_DVI_ATOP3_74_L, 0x00, 0x3F); //power on for Port C DVI DPLPHI/DPLPHQ
        MDrv_WriteByteMask(REG_DVI_ATOP2_74_L, 0x00, 0x3F); //power on for Port D DVI DPLPHI/DPLPHQ

        // IMMESWITCH enhance setting
        W2BYTEMSK(REG_HDMI2_20_L, BIT(8), BIT(8)); // [8]: 0: disable 1:enable (mask ISLAND_EN while power saving mode)

        for(i=0;i<4;i++)
        {
            W2BYTEMSK(REG_DVI_PS_01_L+(i*u8reg_offset), BMASK(3:2), BMASK(3:0)); //[3:2]: turn off B/G.
            W2BYTE(REG_DVI_PS_02_L+(i*u8reg_offset), 0x0070); //[15:4]: Hsync front on num, [3:0]: Hsync back off num.
            W2BYTE(REG_DVI_PS_03_L+(i*u8reg_offset), 0x0801); //[15:4]: Vsync front on num, [3:0]: Vsync back off num.
            W2BYTEMSK(REG_DVI_PS_04_L+(i*u8reg_offset), 0x0000, BMASK(11:8)); //REG_DVI_PS_04_H[3:0]=0: DVI VDE period change tolerance.
            W2BYTEMSK(REG_DVI_PS_06_L+(i*u8reg_offset), BIT(11), BIT(11)); //[11]: Turn on offline power only on Vblanking
            W2BYTE(REG_DVI_PS_0A_L+(i*u8reg_offset), 0x0C30); //[11:10]: HDCP Hsync/Vsync re-gen, [8:4]: learn frame count for H/V regen.
        }

        W2BYTEMSK(REG_DVI_DTOP_0B_L, BIT(1), BMASK(1:0)); // [1:0]: DVI select channel2(R) as small DE
        W2BYTEMSK(REG_DVI_DTOP1_0B_L, BIT(1), BMASK(1:0)); // [1:0]: DVI1 select channel 2(R) small DE
        W2BYTEMSK(REG_DVI_DTOP2_0B_L, BIT(1), BMASK(1:0)); // [1:0]: DVI2 select channel 2(R) small DE
        W2BYTEMSK(REG_DVI_DTOP3_0B_L, BIT(1), BMASK(1:0)); // [1:0]: DVI3 select channel 2(R) small DE
        W2BYTEMSK(REG_DVI_DTOP_19_L, BIT(5), BMASK(5:4)); // [1:0]: DVI select channel2(R) as align DE
        W2BYTEMSK(REG_DVI_DTOP1_19_L, BIT(5), BMASK(5:4)); // [1:0]: DVI1 select channel2(R) as align DE
        W2BYTEMSK(REG_DVI_DTOP2_19_L, BIT(5), BMASK(5:4)); // [1:0]: DVI2 select channel2(R) as align DE
        W2BYTEMSK(REG_DVI_DTOP3_19_L, BIT(5), BMASK(5:4)); // [1:0]: DVI3 select channel2(R) as align DE

        // DVI slowly update
        W2BYTEMSK( REG_DVI_DTOP_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
        W2BYTEMSK( REG_DVI_DTOP1_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP1_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
        W2BYTEMSK( REG_DVI_DTOP2_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP2_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
        W2BYTEMSK( REG_DVI_DTOP3_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP3_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly

    }
    else
    {
        #ifdef DVI_OFFLINE_CLK_DETECTION
        PM_W2BYTE(REG_PM_SLEEP_4A_L, 0x0000, 0xF000); // power on DVI off-line detection clock
        PM_W2BYTE(REG_PM_SLEEP_4B_L, 0x0000, 0xFFFF); // DVI clock power on
        #endif

        // Power up Xtal clocks and DVI detection clock for DVI clock detection
        W2BYTEMSK(REG_DVI_ATOP_06_L, 0x0000, 0x8001);
        W2BYTEMSK(REG_DVI_ATOP1_06_L, 0x0000, 0x8001);
        W2BYTEMSK(REG_DVI_ATOP2_06_L, 0x0000, 0x8001);
        W2BYTEMSK(REG_DVI_ATOP3_06_L, 0x0000, 0x8001);

        // DVI slowly update
        W2BYTEMSK( REG_DVI_DTOP_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP_2A_L, 0xE3E3); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
        W2BYTEMSK( REG_DVI_DTOP1_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP1_2A_L, 0xE3E3); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
        W2BYTEMSK( REG_DVI_DTOP2_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP2_2A_L, 0xE3E3); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
        W2BYTEMSK( REG_DVI_DTOP3_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
        W2BYTE( REG_DVI_DTOP3_2A_L, 0xE3E3); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
    }

    //MHL port Hot Plug Control by CBUS
    PM_W2BYTE(REG_PM_SLEEP_73_L, 0, BIT(14)| BIT(9)| BIT(6)| BIT(1)); // [14]: reg_hpluga_gpio_en, [9]: reg_hpluga_pu1k, [6]: reg_hplugc_gpio_en, [1]: reg_hpluga_pu1k 
    
    if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI0)
    {
        PM_W2BYTE(REG_PM_SLEEP_73_L, BIT(8), BIT(8)| BIT(0)); // [8]: reg_hpluga_mhl_en
    }
    else if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI2)
    {
        PM_W2BYTE(REG_PM_SLEEP_73_L, BIT(0), BIT(8)| BIT(0)); // [0]: reg_hplugc_mhl_en
    }

    // power down CHCK
    W2BYTEMSK(REG_DVI_ATOP_6F_L, 0, BIT(0));
    W2BYTEMSK(REG_DVI_ATOP1_6F_L, 0, BIT(0));
    W2BYTEMSK(REG_DVI_ATOP2_6F_L, 0, BIT(0));
    W2BYTEMSK(REG_DVI_ATOP3_6F_L, 0, BIT(0));

    //R-term setting
    W2BYTEMSK( REG_DVI_DTOP_0C_L, BIT(9), BIT(9)); //override enable
    W2BYTEMSK( REG_DVI_DTOP_0C_L, BIT(3)|BIT(2), BMASK(4:0)); //initial value = 0x0C

    // enable Vsync glitch filter
    W2BYTEMSK(REG_DVI_DTOP_1E_L, BIT(4), BIT(4));  // enable DVI port0 Vsync glitch filter
    W2BYTEMSK(REG_DVI_DTOP1_1E_L, BIT(4), BIT(4));  // enable DVI port1 Vsync glitch filter
    W2BYTEMSK(REG_DVI_DTOP2_1E_L, BIT(4), BIT(4));  // enable DVI port2 Vsync glitch filter
    W2BYTEMSK(REG_DVI_DTOP3_1E_L, BIT(4), BIT(4));  // enable DVI port3 Vsync glitch filter

    //by pass phace dac spread control
    W2BYTEMSK(REG_DVI_DTOP_23_L, (HDMI_WB_DETECT_UNLOCK_THRESHOLD << 8)| HDMI_WB_DETECT_LOCK_THRESHOLD, BMASK(15:0));
    W2BYTEMSK(REG_DVI_DTOP1_23_L, (HDMI_WB_DETECT_UNLOCK_THRESHOLD << 8)| HDMI_WB_DETECT_LOCK_THRESHOLD, BMASK(15:0));
    W2BYTEMSK(REG_DVI_DTOP2_23_L, (HDMI_WB_DETECT_UNLOCK_THRESHOLD << 8)| HDMI_WB_DETECT_LOCK_THRESHOLD, BMASK(15:0));
    W2BYTEMSK(REG_DVI_DTOP3_23_L, (HDMI_WB_DETECT_UNLOCK_THRESHOLD << 8)| HDMI_WB_DETECT_LOCK_THRESHOLD, BMASK(15:0));

    // EQ value setting
    W2BYTEMSK(REG_DVI_DTOP_20_L, BIT(13)| ((HDMI_R_CHANNEL_EQ_VALUE << 8)| (HDMI_G_CHANNEL_EQ_VALUE << 4)| HDMI_B_CHANNEL_EQ_VALUE), BIT(13)| BMASK(11:0));
    W2BYTEMSK(REG_DVI_DTOP1_20_L, BIT(13)| ((HDMI_R_CHANNEL_EQ_VALUE << 8)| (HDMI_G_CHANNEL_EQ_VALUE << 4)| HDMI_B_CHANNEL_EQ_VALUE), BIT(13)| BMASK(11:0));
    W2BYTEMSK(REG_DVI_DTOP2_20_L, BIT(13)| ((HDMI_R_CHANNEL_EQ_VALUE << 8)| (HDMI_G_CHANNEL_EQ_VALUE << 4)| HDMI_B_CHANNEL_EQ_VALUE), BIT(13)| BMASK(11:0));
    W2BYTEMSK(REG_DVI_DTOP3_20_L, BIT(13)| ((HDMI_R_CHANNEL_EQ_VALUE << 8)| (HDMI_G_CHANNEL_EQ_VALUE << 4)| HDMI_B_CHANNEL_EQ_VALUE), BIT(13)| BMASK(11:0));
    
    W2BYTEMSK(REG_DVI_DTOP_1E_L, 0x00, BMASK(3:0)); // [3:0]: Timer for HDMI report
    W2BYTEMSK(REG_DVI_DTOP1_1E_L, 0x00, BMASK(3:0)); // [3:0]: Timer for HDMI report
    W2BYTEMSK(REG_DVI_DTOP2_1E_L, 0x00, BMASK(3:0)); // [3:0]: Timer for HDMI report
    W2BYTEMSK(REG_DVI_DTOP3_1E_L, 0x00, BMASK(3:0)); // [3:0]: Timer for HDMI report

    //MHL port Hot Plug Control by CBUS
    //PM_W2BYTE(REG_PM_SLEEP_73_L, BIT(0), BIT(0)); // [0]: reg_hplugc_mhl_en

    //set TIMER V Divider for div report
    if(MST_XTAL_CLOCK_HZ==12000000UL)
    {
        W2BYTEMSK(REG_DVI_DTOP_1E_L, BIT(3), BMASK(3:0));  // timer baased on Xtal = 12Mhz
        W2BYTEMSK(REG_DVI_DTOP1_1E_L, BIT(3), BMASK(3:0));  // timer baased on Xtal = 12Mhz
        W2BYTEMSK(REG_DVI_DTOP2_1E_L, BIT(3), BMASK(3:0));  // timer baased on Xtal = 12Mhz
        W2BYTEMSK(REG_DVI_DTOP3_1E_L, BIT(3), BMASK(3:0));  // timer baased on Xtal = 12Mhz
    }

    //Hal_HDCP_ddc_en(ENABLE);  // HDCP enable for ddc
    // Enable DVI function: REG_DVI_ATOP_00_L[2] should be always enable if one of HDMI ports is used.
    W2BYTEMSK(REG_DVI_ATOP_00_L, BIT(2), BIT(2)); // enable DVI0
    W2BYTEMSK(REG_DVI_ATOP1_00_L, BIT(2), BIT(2)); // enable DVI1
    W2BYTEMSK(REG_DVI_ATOP2_00_L, BIT(2), BIT(2)); // enable DVI2
    W2BYTEMSK(REG_DVI_ATOP3_00_L, BIT(2), BIT(2)); // enable DVI3

	W2BYTEMSK(REG_DVI_ATOP3_0A_L, BIT(7), BMASK(7:5)); // gcr pll clock recovery lock mode (portC)

    // Enable DVI phase accumulator extension and clock stable gate
    W2BYTEMSK( REG_DVI_DTOP_04_L, 0, BIT(1)|BIT(0)); // [1]:Enable of acc8lsb extension ; [0]:Enable of acc8lsb extension select
    W2BYTEMSK( REG_DVI_DTOP_21_L, 0, BIT(1)); // [1]:Enable the function that clock gen gate signal controlled by TMDS clock stable
    W2BYTEMSK( REG_DVI_DTOP_04_L+0x2900, 0, BIT(1)|BIT(0)); // [1]:Enable of acc8lsb extension ; [0]:Enable of acc8lsb extension select
    W2BYTEMSK( REG_DVI_DTOP_21_L+0x2900, 0, BIT(1)); // [1]:Enable the function that clock gen gate signal controlled by TMDS clock stable
    W2BYTEMSK( REG_DVI_DTOP_04_L+0x2B00, 0, BIT(1)|BIT(0)); // [1]:Enable of acc8lsb extension ; [0]:Enable of acc8lsb extension select
    W2BYTEMSK( REG_DVI_DTOP_21_L+0x2B00, 0, BIT(1)); // [1]:Enable the function that clock gen gate signal controlled by TMDS clock stable
    W2BYTEMSK( REG_DVI_DTOP_04_L+0x2D00, 0, BIT(1)|BIT(0)); // [1]:Enable of acc8lsb extension ; [0]:Enable of acc8lsb extension select
    W2BYTEMSK( REG_DVI_DTOP_21_L+0x2D00, 0, BIT(1)); // [1]:Enable the function that clock gen gate signal controlled by TMDS clock stable

    // Enable DVI clock tolerance: 4*0.7 = 2.8 MHz
    W2BYTEMSK(REG_DVI_DTOP_13_L, (0x2<<12)|0xC80,  BMASK(14:0)); //the comp value is reg_comp_tol * 4
    W2BYTEMSK(REG_DVI_DTOP_13_L+0x2900, (0x2<<12)|0xC80,  BMASK(14:0)); //the comp value is reg_comp_tol * 4
    W2BYTEMSK(REG_DVI_DTOP_13_L+0x2B00, (0x2<<12)|0xC80,  BMASK(14:0)); //the comp value is reg_comp_tol * 4
    W2BYTEMSK(REG_DVI_DTOP_13_L+0x2D00, (0x2<<12)|0xC80,  BMASK(14:0)); //the comp value is reg_comp_tol * 4
    W2BYTEMSK(REG_DVI_DTOP_12_L, (0xF<<12),  BMASK(15:12)); //0x110A25=0xB0, DVI clock detection counter tolerance set to 7
    W2BYTEMSK(REG_DVI_DTOP_12_L+0x2900, (0xF<<12),  BMASK(15:12)); //0x110A25=0xB0, DVI clock detection counter tolerance set to 7
    W2BYTEMSK(REG_DVI_DTOP_12_L+0x2B00, (0xF<<12),  BMASK(15:12)); //0x110A25=0xB0, DVI clock detection counter tolerance set to 7
    W2BYTEMSK(REG_DVI_DTOP_12_L+0x2D00, (0xF<<12),  BMASK(15:12)); //0x110A25=0xB0, DVI clock detection counter tolerance set to 7

    //DVIPLL auto protection setting
    //PORTA
    W2BYTEMSK(REG_DVI_ATOP_65_L, 0,  BMASK(13:12));//Voltage threshold setting: 0x0 = 1.1v, 0x1 = 1.15v, 0x2 = 1.2v
    //PORTB
    W2BYTEMSK(REG_DVI_ATOP1_65_L, 0,  BMASK(13:12));//Voltage threshold setting: 0x0 = 1.1v, 0x1 = 1.15v, 0x2 = 1.2v
    //PORTD
    W2BYTEMSK(REG_DVI_ATOP2_65_L, 0,  BMASK(13:12));//Voltage threshold setting: 0x0 = 1.1v, 0x1 = 1.5v, 0x2 = 1.2v
    //PORTC
    W2BYTEMSK(REG_DVI_ATOP3_65_L, 0,  BMASK(13:12));//Voltage threshold setting: 0x0 = 1.1v, 0x1 = 1.5v, 0x2 = 1.2v

    // [5:4] = 2'b 01, Audio FIFO 1/4 full
    W2BYTEMSK(REG_HDMI_05_L, BIT(4), BIT(5)|BIT(4));
    // [15]: output black level, [14]: DC info, [13]: Audio flat bit check, [11:10]: Blanking when AVMUTE, [9]: HDMI/DVI mode HW detect
    W2BYTEMSK(REG_HDMI_06_L, (0x6E<<8), HBMASK);
    // [1:0]: enable HDMI deep color mode
    W2BYTEMSK(REG_HDMI_07_L, 0x03, LBMASK);
    // [2]: Deep color FIFO type 1
    W2BYTEMSK(REG_HDMI_08_L, BIT(2), BIT(2));
    // [7]: Frame repetition manual mode, [1]: auto non-PCM detection
    W2BYTEMSK( REG_HDMI_64_L, BIT(1)|BIT(7), LBMASK);

    //****** For MHL PP mode
    // [5:4] = 2'b 01, Audio FIFO 1/4 full
    W2BYTEMSK(REG_MHL_TMDS_05_L, BIT(4), BIT(5)|BIT(4));
    // [15]: output black level, [14]: DC info, [13]: Audio flat bit check, [11:10]: Blanking when AVMUTE, [9]: HDMI/DVI mode HW detect
    W2BYTEMSK(REG_MHL_TMDS_06_L, (0x6E<<8), HBMASK);
    // [1:0]: enable HDMI deep color mode
    W2BYTEMSK(REG_MHL_TMDS_07_L, 0x03, LBMASK);
    // [2]: Deep color FIFO type 1
    W2BYTEMSK(REG_MHL_TMDS_08_L, BIT(2), BIT(2));
    // [7]: Frame repetition manual mode, [1]: auto non-PCM detection
    W2BYTEMSK( REG_MHL_TMDS_64_L, BIT(1)|BIT(7), LBMASK);
    //****** End of for MHL PP mode

    // [7]: enable CTS/N synth,  [0]: CTS[19:0]
    W2BYTEMSK( REG_HDMI2_01_L, BIT(0)|BIT(7), LBMASK);
    // enable deep color clock 2x
    W2BYTEMSK( REG_HDMI2_03_L, BMASK(7:4), BMASK(7:4));

    // pixel clock from atop pixel clock and HDMI 3D settings
    W2BYTEMSK( REG_HDMI2_20_L, BMASK(7:6)|BIT(4)|BIT(0), BMASK(7:6)|BIT(4)|BIT(0)); // [7]: 0,TMDS 1,pixel clock ; [6] : 0,deep color 1,reg, [4]: enable AVG scale down FIFO, [0]: Bypass 422 to 444

    // turn off all the interrupt
    W2BYTE(REG_HDMI_63_L, 0xFFFF); // HDMI interrupt clear
    W2BYTE(REG_HDMI_63_L, 0);
    W2BYTE(REG_HDMI_60_L, 0xFFFF);  //REG_HDMI_INT_MASK

    //****** for MHL PP mode
    // turn off all the interrupt
    W2BYTE(REG_MHL_TMDS_63_L, 0xFFFF); // HDMI interrupt clear
    W2BYTE(REG_MHL_TMDS_63_L, 0);
    W2BYTE(REG_MHL_TMDS_60_L, 0xFFFF);  //REG_HDMI_INT_MASK
    //****** End of for MHL PP mode

    W2BYTEMSK(REG_HDMI2_27_L, BMASK(15:8), BMASK(15:8)); // HDMI2 interrupt clear
    W2BYTEMSK(REG_HDMI2_27_L, 0, BMASK(15:8));
    W2BYTEMSK(REG_HDMI2_26_L, BMASK(15:8), BMASK(15:8)); // HDMI2 interrupt mask

    // Setting pixel clock comparator for 3D timing which is large than 166MHz
    W2BYTE(REG_HDMI2_33_L, 0x0215); // 50MHz count value
    W2BYTE(REG_HDMI2_34_L, 0x042A); // 100MHz count value
    W2BYTE(REG_HDMI2_35_L, 0x06EA); // 166MHz count value

    // Disable data Rterm HW control
    W2BYTEMSK(REG_DVI_ATOP1_01_L, 0, BIT(0)); // Port B
    W2BYTEMSK(REG_DVI_ATOP2_01_L, 0, BIT(0)); // Port D
    W2BYTEMSK(REG_DVI_ATOP3_01_L, 0, BIT(0)); // Port C

    // Always set pull down 100K
    W2BYTEMSK(REG_PM_MHL_CBUS_20, BIT(7), BIT(7));
    
	bCalibrationFlag = Hal_HDMI_Calibration(0);
	msg_hdmi(printf("PortA: Calibration : %d",bCalibrationFlag));
    bCalibrationFlag = Hal_HDMI_Calibration(1);
	msg_hdmi(printf("PortB: Calibration : %d",bCalibrationFlag));
    bCalibrationFlag = Hal_HDMI_Calibration(2);
    msg_hdmi(printf("PortD: Calibration : %d",bCalibrationFlag));
	bCalibrationFlag = Hal_HDMI_Calibration(3);
    msg_hdmi(printf("PortC: Calibration : %d",bCalibrationFlag));

	W2BYTEMSK(REG_DVI_ATOP_63_L, BIT(9), BIT(9)); //EN_LDO
    W2BYTEMSK(REG_DVI_ATOP1_63_L, BIT(9), BIT(9)); //EN_LDO
    W2BYTEMSK(REG_DVI_ATOP2_63_L, BIT(9), BIT(9)); //EN_LDO

    // switch to hysteresis clock path
    W2BYTEMSK(REG_DVI_ATOP_32_L, BIT(12), BMASK(13:12)); // Port A
    W2BYTEMSK(REG_DVI_ATOP1_32_L, BIT(12), BMASK(13:12)); // Port B
    W2BYTEMSK(REG_DVI_ATOP2_32_L, BIT(12), BMASK(13:12)); // Port D
    W2BYTEMSK(REG_DVI_ATOP3_32_L, BIT(12), BMASK(13:12)); // Port C

    //if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ)
    {
        //port A
        //MDrv_WriteByteMask(REG_DVI_DTOP_00_L, BIT(4), BIT(4)); // enable EQ new mode
        //MDrv_WriteByteMask(REG_DVI_EQ_02_H, BIT(3), BIT(3));
        //MDrv_WriteByteMask(REG_DVI_EQ_04_L, 0x31, 0xFF); //EQ 1 and EQ 0
        //MDrv_WriteByteMask(REG_DVI_EQ_04_H, 0x01, 0x0F); //EQ 3
        //MDrv_WriteByte(REG_DVI_EQ_10_L, 0x80); // [7]: angle overwirte mode, [1]:disable manual EQ mode
        //MDrv_WriteByte(REG_DVI_EQ_10_H, 0xA2); // [7]: check good phase and error rate, [5:4]: 40 phases, [1]: overwrite enable for comparion two eq levels
        //MDrv_WriteByte(REG_DVI_EQ_11_L, 0x03); // [3:0]:reg_eq2_ov_value
        //MDrv_WriteByte(REG_DVI_EQ_11_H, 0x31); // [7:4]:reg_eq1_ov_value, [3:0]:reg_eq0_ov_value
        //MDrv_WriteByteMask(REG_DVI_EQ_12_H, 0x30, 0xF0); // [7:4]: Continuous good phases threshold = 3
        //MDrv_WriteByte(REG_DVI_EQ_17_L, 0x2F); // [7:6]: 64ms timeout, [5]: Time out enable, [3]: waive enable, [2]: 2nd eye, [1]: unused DE, [0]: DE de-glitch
        //MDrv_WriteByte(REG_DVI_EQ_17_H, 0x3F); // de-glitch length = 0x3F(pixels)
        //MDrv_WriteByteMask(REG_DVI_DTOP_30_H, BIT(4), BIT(4)); // Auto EQ trigger only > 100MHz
        //MDrv_WriteByteMask(REG_DVI_EQ_02_H, 0x10, 0x10); // make EQ phase smoothly when pixel rate is under 100MHz
        //MDrv_WriteByte(REG_DVI_DTOP_2F_L, 0x89); // enable auto EQ trigger

        //port B
        //MDrv_WriteByteMask(REG_DVI_DTOP1_00_L, BIT(4), BIT(4)); // enable EQ new mode
        //MDrv_WriteByteMask(REG_DVI_EQ1_02_H, BIT(3), BIT(3));
        //MDrv_WriteByteMask(REG_DVI_EQ1_04_L, 0x31, 0xFF); //EQ 1 and EQ 0
        //MDrv_WriteByteMask(REG_DVI_EQ1_04_H, 0x01, 0x0F); //EQ 3
        //MDrv_WriteByte(REG_DVI_EQ1_10_L, 0x80); // [7]: angle overwirte mode, [1]:disable manual EQ mode
        //MDrv_WriteByte(REG_DVI_EQ1_10_H, 0xA2); // [7]: check good phase and error rate, [5:4]: 40 phases, [1]: overwrite enable for comparion two eq levels
        //MDrv_WriteByte(REG_DVI_EQ1_11_L, 0x03); // [3:0]:reg_eq2_ov_value
        //MDrv_WriteByte(REG_DVI_EQ1_11_H, 0x31); // [7:4]:reg_eq1_ov_value, [3:0]:reg_eq0_ov_value
        //MDrv_WriteByteMask(REG_DVI_EQ1_12_H, 0x30, 0xF0); // [7:4]: Continuous good phases threshold = 3
        //MDrv_WriteByte(REG_DVI_EQ1_17_L, 0x2F); // [7:6]: 64ms timeout, [5]: Time out enable, [3]: waive enable, [2]: 2nd eye, [1]: unused DE, [0]: DE de-glitch
        //MDrv_WriteByte(REG_DVI_EQ1_17_H, 0x3F); // de-glitch length = 0x3F(pixels)
        //MDrv_WriteByteMask(REG_DVI_DTOP1_30_H, BIT(4), BIT(4)); // Auto EQ trigger only > 100MHz
        //MDrv_WriteByteMask(REG_DVI_EQ1_02_H, 0x10, 0x10); // make EQ phase smoothly when pixel rate is under 100MHz
        //MDrv_WriteByte(REG_DVI_DTOP1_2F_L, 0x89); // enable auto EQ trigger

        //port C
        //MDrv_WriteByteMask(REG_DVI_DTOP3_00_L, BIT(4), BIT(4)); // enable EQ new mode
        //MDrv_WriteByteMask(REG_DVI_EQ3_02_H, BIT(3), BIT(3));
        //MDrv_WriteByteMask(REG_DVI_EQ3_04_L, 0x31, 0xFF); //EQ 1 and EQ 0
        //MDrv_WriteByteMask(REG_DVI_EQ3_04_H, 0x01, 0x0F); //EQ 3
        //MDrv_WriteByte(REG_DVI_EQ3_10_L, 0x80); // [7]: angle overwirte mode, [1]:disable manual EQ mode
        //MDrv_WriteByte(REG_DVI_EQ3_10_H, 0xA2); // [7]: check good phase and error rate, [5:4]: 40 phases, [1]: overwrite enable for comparion two eq levels
        //MDrv_WriteByte(REG_DVI_EQ3_11_L, 0x03); // [3:0]:reg_eq2_ov_value
        //MDrv_WriteByte(REG_DVI_EQ3_11_H, 0x31); // [7:4]:reg_eq1_ov_value, [3:0]:reg_eq0_ov_value
        //MDrv_WriteByteMask(REG_DVI_EQ3_12_H, 0x30, 0xF0); // [7:4]: Continuous good phases threshold = 3
        //MDrv_WriteByte(REG_DVI_EQ3_17_L, 0x2F); // [7:6]: 64ms timeout, [5]: Time out enable, [3]: waive enable, [2]: 2nd eye, [1]: unused DE, [0]: DE de-glitch
        //MDrv_WriteByte(REG_DVI_EQ3_17_H, 0x3F); // de-glitch length = 0x3F(pixels)
        //MDrv_WriteByteMask(REG_DVI_DTOP3_30_H, BIT(4), BIT(4)); // Auto EQ trigger only > 100MHz
        //MDrv_WriteByteMask(REG_DVI_EQ3_02_H, 0x10, 0x10); // make EQ phase smoothly when pixel rate is under 100MHz
        //MDrv_WriteByte(REG_DVI_DTOP3_2F_L, 0x89); // enable auto EQ trigger

        //port D
        //MDrv_WriteByteMask(REG_DVI_DTOP2_00_L, BIT(4), BIT(4)); // enable EQ new mode
        //MDrv_WriteByteMask(REG_DVI_EQ2_02_H, BIT(3), BIT(3));
        //MDrv_WriteByteMask(REG_DVI_EQ2_04_L, 0x31, 0xFF); //EQ 1 and EQ 0
        //MDrv_WriteByteMask(REG_DVI_EQ2_04_H, 0x01, 0x0F); //EQ 3
        //MDrv_WriteByte(REG_DVI_EQ2_10_L, 0x80); // [7]: angle overwirte mode, [1]:disable manual EQ mode
        //MDrv_WriteByte(REG_DVI_EQ2_10_H, 0xA2); // [7]: check good phase and error rate, [5:4]: 40 phases, [1]: overwrite enable for comparion two eq levels
        //MDrv_WriteByte(REG_DVI_EQ2_11_L, 0x03); // [3:0]:reg_eq2_ov_value
        //MDrv_WriteByte(REG_DVI_EQ2_11_H, 0x31); // [7:4]:reg_eq1_ov_value, [3:0]:reg_eq0_ov_value
        //MDrv_WriteByteMask(REG_DVI_EQ2_12_H, 0x30, 0xF0); // [7:4]: Continuous good phases threshold = 3
        //MDrv_WriteByte(REG_DVI_EQ2_17_L, 0x2F); // [7:6]: 64ms timeout, [5]: Time out enable, [3]: waive enable, [2]: 2nd eye, [1]: unused DE, [0]: DE de-glitch
        //MDrv_WriteByte(REG_DVI_EQ2_17_H, 0x3F); // de-glitch length = 0x3F(pixels)
        //MDrv_WriteByteMask(REG_DVI_DTOP2_30_H, BIT(4), BIT(4)); // Auto EQ trigger only > 100MHz
        //MDrv_WriteByteMask(REG_DVI_EQ2_02_H, 0x10, 0x10); // make EQ phase smoothly when pixel rate is under 100MHz
        //MDrv_WriteByte(REG_DVI_DTOP2_2F_L, 0x89); // enable auto EQ trigger
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the MHL DE
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 Hal_MHL_TMDS_GetHDE(void)
{
    return R2BYTEMSK(REG_MHL_TMDS_65_L, 0x3FFF);
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI DE
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 Hal_HDMI_GetHDEInfo(MS_U8 ucHDMIInfoSource)
{
    if(ucHDMIInfoSource)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_GetHDE();
    }

    return R2BYTEMSK(REG_HDMI_65_L, 0x3FFF);
}

//=========================================================//
// Function : Hal_HDMI_set_EQ
// Description:
//=========================================================//
void Hal_HDMI_Set_EQ(E_MUX_INPUTPORT enInputPortType, MS_HDMI_EQ enEq, MS_U8 u8EQValue)
{
    MS_U16 u16bank_offset;
    MS_U16 temp;
    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000; break; // BK 0x110A00
        case INPUT_PORT_DVI1:  u16bank_offset = 0x2900;  break; // BK 0x113300
        case INPUT_PORT_DVI3:  u16bank_offset = 0x2B00;  break; // BK 0x113500
        case INPUT_PORT_DVI2:  u16bank_offset = 0x2D00;  break; // BK 0x113700
    }

    temp = u8EQValue & 0x0F;

    switch (enEq)
    {
        case MS_HDMI_EQ_2M:
            W2BYTEMSK(REG_DVI_DTOP_20_L+u16bank_offset, 0x0555, 0x0FFF);
            break;
        case MS_HDMI_EQ_10M:
            W2BYTEMSK(REG_DVI_DTOP_20_L+u16bank_offset, 0x0AAA, 0x0FFF);
            break;
        case MS_HDMI_EQ_15M:
            W2BYTEMSK(REG_DVI_DTOP_20_L+u16bank_offset, 0x0CCC, 0x0FFF);
            break;
        case MS_HDMI_EQ_MAX:
            W2BYTEMSK(REG_DVI_DTOP_20_L+u16bank_offset, 0xFFFF, 0x0FFF);
            break;
        case MS_HDMI_EQ_USER_DEFINE:
            W2BYTEMSK(REG_DVI_DTOP_20_L+u16bank_offset, (temp)|(temp<<4)|(temp<<8), 0x0FFF);
            break;
        default:
            W2BYTEMSK(REG_DVI_DTOP_20_L+u16bank_offset, 0x0AAA, 0x0FFF);
            break;
    }

}

//=========================================================//
// Function : Hal_HDMI_Audio_MUTE_Enable
// Description:
//=========================================================//
void Hal_HDMI_Audio_MUTE_Enable(MS_U16 u16MuteEvent, MS_U16 u16MuteMask)
{
    //Reserved
}

//=========================================================//
// Function : Hal_HDMI_Audio_Status_Clear
// Description:
//=========================================================//
void Hal_HDMI_Audio_Status_Clear(void)
{
    //Reserved
}

//=========================================================//
// Function : Hal_MHL_TMDS_gcontrol_info
// Description:
//#[15:12]: PP_VAL[3:0]
//#[11:8]: CD_VAL[3:0]
//#[7:5]: PRE_LPP[2:0]
//#[4:2]: LAST_PP[2:0]
//#[1]: Default_Phase
//#[0]: AVMUTE
//=========================================================//
MS_U16 Hal_MHL_TMDS_gcontrol_info(HDMI_GControl_INFO_t gcontrol)
{
    MS_U16 u16regvalue;

    u16regvalue = R2BYTE(REG_MHL_TMDS_15_L);
    switch(gcontrol)
    {
        default:
        case G_CTRL_AVMUTE://#[0]: AVMUTE
            u16regvalue &= BIT(0);
            break;
        case G_Ctrl_DEFAULT_PHASE://#[1]: Default_Phase
            u16regvalue &= BIT(1);
            break;
        case G_Ctrl_LAST_PP://#[4:2]: LAST_PP[2:0]
            u16regvalue &= (BIT(4)|BIT(3)|BIT(2));
            break;
        case G_Ctrl_PRE_LPP://#[7:5]: PRE_LPP[2:0]
            u16regvalue &= (BIT(7)|BIT(6)|BIT(5));
            break;
        case G_Ctrl_CD_VAL://#[11:8]: CD_VAL[3:0]
            u16regvalue &= (BIT(11)|BIT(10)|BIT(9)|BIT(8));
            break;
        case G_Ctrl_PP_VAL://#[15:12]: PP_VAL[3:0]
            u16regvalue &= (BIT(15)|BIT(14)|BIT(13)|BIT(12));
            break;
        case G_Ctrl_ALL:
            u16regvalue &=(LBMASK|HBMASK);
            break;
    }

    return u16regvalue;
}

//=========================================================//
// Function : Hal_HDMI_gcontrol_info
// Description:
//#[15:12]: PP_VAL[3:0]
//#[11:8]: CD_VAL[3:0]
//#[7:5]: PRE_LPP[2:0]
//#[4:2]: LAST_PP[2:0]
//#[1]: Default_Phase
//#[0]: AVMUTE
//=========================================================//
MS_U16 Hal_HDMI_gcontrol_info(HDMI_GControl_INFO_t gcontrol, MS_U8 ucHDMIInfoSource)
{
    MS_U16 u16regvalue;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_gcontrol_info(gcontrol);
    }

    u16regvalue = R2BYTE(REG_HDMI_15_L);
    switch(gcontrol)
    {
        default:
        case G_CTRL_AVMUTE://#[0]: AVMUTE
            u16regvalue &= BIT(0);
            break;
        case G_Ctrl_DEFAULT_PHASE://#[1]: Default_Phase
            u16regvalue &= BIT(1);
            break;
        case G_Ctrl_LAST_PP://#[4:2]: LAST_PP[2:0]
            u16regvalue &= (BIT(4)|BIT(3)|BIT(2));
            break;
        case G_Ctrl_PRE_LPP://#[7:5]: PRE_LPP[2:0]
            u16regvalue &= (BIT(7)|BIT(6)|BIT(5));
            break;
        case G_Ctrl_CD_VAL://#[11:8]: CD_VAL[3:0]
            u16regvalue &= (BIT(11)|BIT(10)|BIT(9)|BIT(8));
            break;
        case G_Ctrl_PP_VAL://#[15:12]: PP_VAL[3:0]
            u16regvalue &= (BIT(15)|BIT(14)|BIT(13)|BIT(12));
            break;
        case G_Ctrl_ALL:
            u16regvalue &=(LBMASK|HBMASK);
            break;
    }

    return u16regvalue;
}

// T3 doesn't have HPLL
//=========================================================//
// Function : Hal_HDMI_pll_ctrl1/2
// Description:
// bread = 1-> read or write
//=========================================================//
MS_U16 Hal_HDMI_pll_ctrl1(HDMI_PLL_CTRL_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    MS_U16 u16regvalue = 0;
#if 0
    if(bread)
        u16regvalue = R2BYTE(REG_HDMI_5C_H);

    switch(pllctrl)
    {
        case PLL_CTRL_KN:                      //#KN[1:0]    15:14
            if(bread)
                u16regvalue &= (BIT(15)|BIT(14));
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & (BIT(15)|BIT(14))), (BIT(15)|BIT(14)));
            break;
        case PLL_CTRL_RCTRL:               //#RCTRL    13:12
            if(bread)
                u16regvalue &= (BIT(13)|BIT(12)|BIT(11));
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & (BIT(13)|BIT(12)|BIT(11))), (BIT(13)|BIT(12)|BIT(11)));
            break;
        case PLL_CTRL_ICTRL:               //#ICTRL    11:8
            if(bread)
                u16regvalue &= (BIT(10)|BIT(9)|BIT(8));
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & (BIT(10)|BIT(9)|BIT(8))), (BIT(10)|BIT(9)|BIT(8)));
            break;
        case PLL_CTRL_MANUAL_DIV:   //#MANUAL_PLL_DIV     7
            if(bread)
                u16regvalue &= BIT(7);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(7)), BIT(7));
            break;
        case PLL_CTRL_PORST:               //#PLL_PORST    6
            if(bread)
                u16regvalue &= BIT(6);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(6)), BIT(6));

            break;
        case PLL_CTRL_RESET_TP:        //#RESET_TP    5
            if(bread)
                u16regvalue &= BIT(5);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(5)), BIT(5));
            break;
        case PLL_CTRL_RESET_TF:        //#RESET_TF    4
            if(bread)
                u16regvalue &= BIT(4);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(4)), BIT(4));
            break;
        case PLL_CTRL_RESET_TI:        //#RESET_TI    3
            if(bread)
                u16regvalue &= BIT(3);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(3)), BIT(3));
            break;
        case PLL_CTRL_VCO_OFFSET:   //#VCO_OFFSET    2
            if(bread)
                u16regvalue &= BIT(2);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(2)), BIT(2));
            break;
        case PLL_CTRL_RESET:               //#PLL_RESET    1
            if(bread)
                u16regvalue &= BIT(1);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(1)), BIT(1));
            break;
        case PLL_CTRL_PWRDOWN:        //#PWRDOWN    0
            if(bread)
                u16regvalue &= BIT(0);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(0)), BIT(0));
            break;
        case PLL_CTRL_L:
            if(bread)
                u16regvalue &= LBMASK;
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & LBMASK), LBMASK);
            break;
        case PLL_CTRL_H:
            if(bread)
                u16regvalue &= HBMASK;
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & HBMASK), HBMASK);
            break;
    }
    //if write, return 0
    if(!bread)
        u16regvalue = 0;
#endif

    return u16regvalue;
}

MS_U16 Hal_HDMI_pll_ctrl2(HDMI_PLL_CTRL2_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    MS_U16 u16regvalue = 0;
#if 0
    if(bread)
        u16regvalue = R2BYTE(REG_HDMI_5D_H);

    switch(pllctrl)
    {

        case PLL_CTRL2_DDIV:               //#DDIV[3:0]    15:12
            if(bread)
                u16regvalue &= (BIT(15)|BIT(14)|BIT(13)|BIT(12));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(15)|BIT(14)|BIT(13)|BIT(12))), (BIT(15)|BIT(14)|BIT(13)|BIT(12)));
            break;
        case PLL_CTRL2_FBDIV:               //#FBDIV[3:0]    11:8
            if(bread)
                u16regvalue &= (BIT(11)|BIT(10)|BIT(9)|BIT(8));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(11)|BIT(10)|BIT(9)|BIT(8))), (BIT(11)|BIT(10)|BIT(9)|BIT(8)));
            break;
        case PLL_CTRL2_KP:               //#KP[3:0]    7:4
            if(bread)
                u16regvalue &= (BIT(7)|BIT(6)|BIT(5)|BIT(4));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(7)|BIT(6)|BIT(5)|BIT(4))), (BIT(7)|BIT(6)|BIT(5)|BIT(4)));
            break;
        case PLL_CTRL2_KM:               //#KM[3:0]    3:0
            if(bread)
                u16regvalue &= (BIT(3)|BIT(2)|BIT(1)|BIT(0));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(3)|BIT(2)|BIT(1)|BIT(0))), (BIT(3)|BIT(2)|BIT(1)|BIT(0)));
            break;
        case PLL_CTRL2_L:
            if(bread)
                u16regvalue &= LBMASK;
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16value & LBMASK), LBMASK);
            break;
        case PLL_CTRL2_H:
            if(bread)
                u16regvalue &= HBMASK;
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16value & HBMASK), HBMASK);
            break;


    }
    //if write, return 0
    if(!bread)
        u16regvalue = 0;
#endif
    return u16regvalue;
}


void Hal_HDMI_hpd_en(MS_BOOL benable)
{
    MS_U16 u16value;

    if(benable) // 0 is output enable
        u16value= 0x0000;
    else
        u16value= 0xFFFF;

    //PM_W2BYTE(REG_PM_SLEEP_27_L, u16value & BMASK(7:4), BMASK(7:4));
    PM_W2BYTE(REG_PM_SLEEP_27_L, u16value, 0x00FF);
}

//=========================================================//
// Function : Hal_MHL_TMDS_packet_info
// Description:
//=========================================================//
MS_U16 Hal_MHL_TMDS_packet_info(void)
{
    return R2BYTE(REG_MHL_TMDS_01_L);
}

//=========================================================//
// Function : Hal_HDMI_packet_info
// Description:
//=========================================================//
MS_U16 Hal_HDMI_packet_info(MS_U8 ucHDMIInfoSource)
{
    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_packet_info();
    }

    return R2BYTE(REG_HDMI_01_L);
}

//=========================================================//
// Function : Hal_MHL_TMDS_packet_ctrl
// Description:
// HDMI_PACKET_INFO_t
//=========================================================//
void Hal_MHL_TMDS_packet_ctrl(MS_U16 u16pkt)
{
    W2BYTE(REG_MHL_TMDS_01_L, u16pkt);
}

//=========================================================//
// Function : Hal_HDMI_packet_info
// Description:
// HDMI_PACKET_INFO_t
//=========================================================//
void Hal_HDMI_packet_ctrl(MS_U8 ucHDMIInfoSource, MS_U16 u16pkt)
{
    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        Hal_MHL_TMDS_packet_ctrl(u16pkt);
        return;
    }

    W2BYTE(REG_HDMI_01_L, u16pkt);
}

//=========================================================//
// Function : Hal_MHL_TMDS_avi_infoframe_info
// Description: avi info frame byte 1~13
//=========================================================//
MS_U8 Hal_MHL_TMDS_avi_infoframe_info(MS_U8 u8byte)
{
    MS_U16 u16Reg;

    if(u8byte<1) //u8byte = 0 is invalide
        u8byte = 1;

    u16Reg = R2BYTE(REG_MHL_TMDS_40_L + ((u8byte - 1)/2)*2);
    return (MS_U8)(((u8byte-1)&0x01) ? (u16Reg>>8):(u16Reg&0xFF));
}

//=========================================================//
// Function : Hal_HDMI_avi_infoframe_info_ver
// Description: avi info frame version
//=========================================================//
EN_AVI_INFOFRAME_VERSION Hal_HDMI_avi_infoframe_info_ver(MS_U8 ucHDMIInfoSource)
{
    EN_AVI_INFOFRAME_VERSION enPktVersion= E_AVI_INFOFRAME_VERSION_NON;

    if(ucHDMIInfoSource > 0)
    {

    }
  
    if (R2BYTEMSK(REG_HDMI_46_L, BMASK(9:8)) >> 8 == 0x00)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION1;
    }
    else if (R2BYTEMSK(REG_HDMI_46_L, BMASK(9:8)) >> 8 == 0x01)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION2;
    }
    else if (R2BYTEMSK(REG_HDMI_46_L, BMASK(9:8)) >> 8 == 0x02)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION3;
    }
    else if (R2BYTEMSK(REG_HDMI_46_L, BMASK(9:8)) >> 8 == 0x03)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION_NON;
    }
    
    return enPktVersion;
}

//=========================================================//
// Function : Hal_HDMI_avi_infoframe_info
// Description: avi info frame byte 1~13
//=========================================================//
MS_U8 Hal_HDMI_avi_infoframe_info(MS_U8 ucHDMIInfoSource, MS_U8 u8byte)
{
    MS_U16 u16Reg;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_avi_infoframe_info(u8byte);
    }

    if(u8byte<1) //u8byte = 0 is invalide
        u8byte = 1;

    u16Reg = R2BYTE(REG_HDMI_40_L + ((u8byte - 1)/2)*2);
    return (MS_U8)(((u8byte-1)&0x01) ? (u16Reg>>8):(u16Reg&0xFF));
}

//=========================================================//
// Function : Hal_MHL_TMDS_get_packet_value
// Description:
//=========================================================//
MS_BOOL Hal_MHL_TMDS_get_packet_value(MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pu8Value)
{
    MS_U32 u32Base_add = 0;

    switch(u8state)
    {
        case PKT_MPEG:
            //1~5, 57
            if(u8byte_idx > 5)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_57_L;

            break;
        case PKT_AUI:
            //1~5, 54
            if(u8byte_idx > 5)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_54_L;

            break;
        case PKT_SPD:
            //1~25, 47
            if(u8byte_idx > 25)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_47_L;

            break;
        case PKT_AVI:
            //1~13, 40
            if(u8byte_idx > 13)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_40_L;

            break;
        case PKT_GC:
            //2, 15[15:0]
            if(u8byte_idx > 2)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_15_L;

            break;
        case PKT_ASAMPLE:
            break;
        case PKT_ACR:
            break;
        case PKT_VS:
            //0~28, 30
            u8byte_idx ++;
            if(u8byte_idx > 29)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_30_L;

            break;
        case PKT_NULL:
            break;
        case PKT_ISRC2:
            //0~31, 20
            u8byte_idx ++;
            if(u8byte_idx > 32)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_20_L;

            break;
        case PKT_ISRC1:
            //1, 1F
            if(u8byte_idx > 1)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_1F_L;

            break;
        case PKT_ACP:
            //0~15, 17
            u8byte_idx ++;
            if(u8byte_idx > 16)
                return FALSE;
            u32Base_add = REG_MHL_TMDS_17_L;

            break;
        case PKT_ONEBIT_AUD:
            break;
        default:
            break;
    }



    *pu8Value = MDrv_ReadByte(u32Base_add + u8byte_idx);

    return TRUE;
}

//=========================================================//
// Function : Hal_HDMI_get_packet_value
// Description:
//=========================================================//
MS_BOOL Hal_HDMI_get_packet_value(MS_U8 ucHDMIInfoSource, MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pu8Value)
{
    MS_U32 u32Base_add = 0;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_get_packet_value(u8state, u8byte_idx, pu8Value);
    }

    switch(u8state)
    {
        case PKT_MPEG:
            //1~5, 57
            if(u8byte_idx > 5)
                return FALSE;
            u32Base_add = REG_HDMI_57_L;

            break;
        case PKT_AUI:
            //1~5, 54
            if(u8byte_idx > 5)
                return FALSE;
            u32Base_add = REG_HDMI_54_L;

            break;
        case PKT_SPD:
            //1~25, 47
            if(u8byte_idx > 25)
                return FALSE;
            u32Base_add = REG_HDMI_47_L;

            break;
        case PKT_AVI:
            //1~13, 40
            if(u8byte_idx > 13)
                return FALSE;
            u32Base_add = REG_HDMI_40_L;

            break;
        case PKT_GC:
            //2, 15[15:0]
            if(u8byte_idx > 2)
                return FALSE;
            u32Base_add = REG_HDMI_15_L;

            break;
        case PKT_ASAMPLE:
            break;
        case PKT_ACR:
            break;
        case PKT_VS:
            //0~28, 30
            u8byte_idx ++;
            if(u8byte_idx > 29)
                return FALSE;
            u32Base_add = REG_HDMI_30_L;

            break;
        case PKT_NULL:
            break;
        case PKT_ISRC2:
        	//0~31, 20
            u8byte_idx ++;
            if(u8byte_idx > 32)
                return FALSE;
            u32Base_add = REG_HDMI_20_L;

            break;
        case PKT_ISRC1:
            //1, 1F
            if(u8byte_idx > 1)
                return FALSE;
            u32Base_add = REG_HDMI_1F_L;

            break;
        case PKT_ACP:
            //0~15, 17
            u8byte_idx ++;
            if(u8byte_idx > 16)
                return FALSE;
            u32Base_add = REG_HDMI_17_L;

            break;
        case PKT_ONEBIT_AUD:
            break;
        default:
            break;
    }

    *pu8Value = MDrv_ReadByte(u32Base_add + u8byte_idx);

    return TRUE;
}

//=========================================================//
// Function : Hal_MHL_TMDS_audio_content_protection_info
// Description: ACP_HDR1[7:0]
//=========================================================//
MS_U8 Hal_MHL_TMDS_audio_content_protection_info(void)
{
   return R2BYTEMSK(REG_MHL_TMDS_16_L, LBMASK);
}

//=========================================================//
// Function : Hal_HDMI_acp_info
// Description: ACP_HDR1[7:0]
//=========================================================//
MS_U8 Hal_HDMI_audio_content_protection_info(MS_U8 ucHDMIInfoSource)
{
    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_audio_content_protection_info();
    }

    return R2BYTEMSK(REG_HDMI_16_L, LBMASK);
}

//=========================================================//
// Function : Hal_MHL_TMDS_audio_payload_info
// Description: ACP packet payload byte 0~15
//=========================================================//
MS_U8 Hal_MHL_TMDS_audio_payload_info(MS_U8 u8byte)
{
    if(u8byte%2)
    {
        return ((R2BYTEMSK(REG_MHL_TMDS_17_L+(u8byte/2)*2, HBMASK)) >> 8);
    }
    else
    {
        return R2BYTEMSK(REG_MHL_TMDS_17_L+(u8byte/2)*2, LBMASK);
    }
}

//=========================================================//
// Function : Hal_HDMI_audio_payload_info
// Description: ACP packet payload byte 0~15
//=========================================================//
MS_U8 Hal_HDMI_audio_payload_info(MS_U8 ucHDMIInfoSource, MS_U8 u8byte)
{
    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_audio_payload_info(u8byte);
    }

    if(u8byte%2)
    {
        return ((R2BYTEMSK(REG_HDMI_17_L+(u8byte/2)*2, HBMASK)) >> 8);
    }
    else
    {
        return R2BYTEMSK(REG_HDMI_17_L+(u8byte/2)*2, LBMASK);
    }
}

//=========================================================//
// Function : Hal_MHL_TMDS_err_status_update
// Description: ACP packet payload byte 0~15
//=========================================================//
MS_U8 Hal_MHL_TMDS_err_status_update(MS_U8 u8value, MS_BOOL bread)
{
    MS_U8 u8regvalue = 0xFF;

    if(bread)
    {
        u8regvalue = R2BYTEMSK(REG_MHL_TMDS_04_L, LBMASK);
    }
    else
    {
        W2BYTEMSK(REG_MHL_TMDS_04_L, u8value, LBMASK);
    }
    return u8regvalue;
}

//=========================================================//
// Function : Hal_HDMI_err_status_update
// Description: ACP packet payload byte 0~15
//=========================================================//
MS_U8 Hal_HDMI_err_status_update(MS_U8 ucHDMIInfoSource, MS_U8 u8value, MS_BOOL bread)
{
    MS_U8 u8regvalue = 0xFF;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_err_status_update(u8value, bread);
    }

    if(bread)
    {
        u8regvalue = R2BYTEMSK(REG_HDMI_04_L, LBMASK);
    }
    else
    {
        W2BYTEMSK(REG_HDMI_04_L, u8value, LBMASK);
    }
    return u8regvalue;
}
/******************************************************************************/
/// for HDMI AUDIO
/******************************************************************************/
//=========================================================//
// Function : Hal_HDMI_audio_output
// Description:
//=========================================================//
void Hal_HDMI_audio_output( MS_BOOL bEnable )
{
#if 0 // not need in T7
    if ( bEnable )
    {
        W2BYTEMSK( REG_HDMI_08_L, (BIT(2) | BIT(0))<<8, (BIT(2) | BIT(0))<<8);
    }
    else
    {
        W2BYTEMSK( REG_HDMI_08_L, 0x0000, HBMASK);
    }
#endif
}

//=========================================================//
// Function : Hal_MHL_TMDS_audio_channel_status
// Description: channel status byte 0~4
//=========================================================//
MS_U8 Hal_MHL_TMDS_audio_channel_status(MS_U8 u8byte)
{
    if(u8byte%2)
    {
        return ((R2BYTEMSK(REG_MHL_TMDS_5A_L+(u8byte/2)*2, HBMASK)) >> 8);
    }
    else
    {
        return R2BYTEMSK(REG_MHL_TMDS_5A_L+(u8byte/2)*2, LBMASK);
    }
}

//=========================================================//
// Function : Hal_HDMI_audio_channel_status
// Description: channel status byte 0~4
//=========================================================//
MS_U8 Hal_HDMI_audio_channel_status(MS_U8 ucHDMIInfoSource, MS_U8 u8byte)
{
    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_audio_channel_status(u8byte);
    }

    if(u8byte%2)
    {
        return ((R2BYTEMSK(REG_HDMI_5A_L+(u8byte/2)*2, HBMASK)) >> 8);
    }
    else
    {
        return R2BYTEMSK(REG_HDMI_5A_L+(u8byte/2)*2, LBMASK);
    }
}

/******************************************************************************/
/// for DVI
/******************************************************************************/
// Aaron comment: doesn't need in U3 because IC already has auto mode
//=========================================================//
// Function : Hal_DVI_clk_lessthan70mhz_usingportc
// Description:
//=========================================================//
MS_BOOL Hal_DVI_clk_lessthan70mhz_usingportc( MS_BOOL bIsPortC )
{
    return 0xFF;
}

//=========================================================//
// Function : Hal_DVI_clk_stable_usingportc
// Description:
//=========================================================//
MS_BOOL Hal_DVI_clk_stable_usingportc( MS_BOOL bIsPortC )
{
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Returne Phase status
/// @param  bClockLessThan70MHz                \b IN: u8Channel
//-------------------------------------------------------------------------------------------------
MS_U16 Hal_DVI_ChannelPhaseStatus( MS_U8 u8Channel)
{
    switch (u8Channel)
    {
        case 0: // Channl R
            return R2BYTE(REG_DVI_DTOP_01_L);
            break;
        case 1: // Channl G
            return R2BYTE(REG_DVI_DTOP_02_L);
            break;
        case 2:
            return R2BYTE(REG_DVI_DTOP_03_L);
            break;
        default:
            return 0;
            break;
    }
}

//=========================================================//
// Function : Hal_DVI_Accumulator_Monitor
// Description:
//=========================================================//
void Hal_DVI_Accumulator_Monitor( void )
{

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrvXC_MVideo_Context._u8MhlSupportInfo==E_INPUT_SUPPORT_MHL_PORT_DVI2)
    {
        if(R2BYTE(REG_DVI_DTOP3_16_L) & BIT(9)) // clk stable
        {
            if((R2BYTE(REG_DVI_DTOP3_01_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP3_01_L) == 0x7FF7) ||
            (R2BYTE(REG_DVI_DTOP3_02_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP3_02_L) == 0x7FF7) ||
            (R2BYTE(REG_DVI_DTOP3_03_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP3_03_L) == 0x7FF7))
            {
                W2BYTEMSK(REG_DVI_DTOP3_0E_L, BIT(4), BIT(4)); // clear accumulator
                W2BYTEMSK(REG_DVI_DTOP3_0E_L, 0, BIT(4) );
            }
        }
	}
    else if(pXCResourcePrivate->stdrvXC_MVideo_Context._u8MhlSupportInfo==E_INPUT_SUPPORT_MHL_PORT_DVI3)
    {
        if(R2BYTE(REG_DVI_DTOP2_16_L) & BIT(9)) // clk stable
        {
            if((R2BYTE(REG_DVI_DTOP2_01_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP2_01_L) == 0x7FF7) ||
            (R2BYTE(REG_DVI_DTOP2_02_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP2_02_L) == 0x7FF7) ||
            (R2BYTE(REG_DVI_DTOP2_03_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP2_03_L) == 0x7FF7))
            {
                W2BYTEMSK(REG_DVI_DTOP2_0E_L, BIT(4), BIT(4)); // clear accumulator
                W2BYTEMSK(REG_DVI_DTOP2_0E_L, 0, BIT(4) );
            }
        }
	}

}

//=========================================================//
// Function : Hal_DVI_adc_adjust
// Description:
//=========================================================//
void Hal_DVI_adc_adjust( MS_BOOL bClockLessThan70MHz )
{
    // do nothing
}

//=========================================================//
// Function : Hal_DVI_clklose_det
// Description:
//=========================================================//
MS_BOOL Hal_DVI_clklose_det(E_MUX_INPUTPORT enInputPortType) // 4M clock
{
    MS_U16 u16regvalue, u16bank_offset;
    MS_BOOL bIsLose;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x110A00
        case INPUT_PORT_DVI1:  u16bank_offset = 0x2900;  break; // BK 0x113300
        case INPUT_PORT_DVI3:  u16bank_offset = 0x2B00;  break; // BK 0x113500
        case INPUT_PORT_DVI2:  u16bank_offset = 0x2D00;  break; // BK 0x113700
    }

    u16regvalue = R2BYTEMSK(REG_DVI_DTOP_17_L+u16bank_offset, HBMASK);
    bIsLose = ((u16regvalue & BIT(15)) ? TRUE : FALSE);

    return bIsLose;
}

void Hal_DVI_IMMESWITCH_PS_SW_Path(void)
{
    MS_U8 i, u8reg_offset = 0;
    MS_U16 u16bank_offset =0;

    for(i=0;i<4;i++)
    {
        switch(i)
        {
            case 0:  u16bank_offset = 0x0000; u8reg_offset = 0x00; break;
            case 1:  u16bank_offset = 0x2900; u8reg_offset = 0x40; break;
            case 2:  u16bank_offset = 0x2B00; u8reg_offset = 0x80; break;
            case 3:  u16bank_offset = 0x2D00; u8reg_offset = 0xC0; break;
            default:
                break;
        }
        // Input clock is stable but DE is idel or unstable
        if( (R2BYTE(REG_DVI_DTOP_16_L+u16bank_offset) & BIT(10)) && (R2BYTE(REG_DVI_DTOP_31_L+u16bank_offset) & 0x30) )
        {
            // Turn off Ch2 as DE align out and cycle align delay
            W2BYTEMSK(REG_DVI_PS_01_L+u8reg_offset, BIT(8), BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
            MsOS_DelayTask(1);
            W2BYTEMSK(REG_DVI_PS_01_L+u8reg_offset, BIT(9)|BIT(8), BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
        }
        // Not turn on power saving mode and DE is stable, but PS still stay in saving state
        if( !(R2BYTE(REG_DVI_PS_00_L+u8reg_offset) & BIT(0)) && (R2BYTE(REG_DVI_DTOP_31_L+u16bank_offset) & BIT(6)) && (R2BYTE(REG_DVI_PS_12_L+u8reg_offset) & BIT(4)) )
        {
            W2BYTEMSK(REG_DVI_PS_00_L+u8reg_offset, BIT(9), BIT(9)); //[9]: PS FSM reset
            MsOS_DelayTask(1);
            W2BYTEMSK(REG_DVI_PS_00_L+u8reg_offset, 0, BIT(9));
        }
    }

}

/******************************************************************************/
///DVI soft reset
/******************************************************************************/
void Hal_DVI_sw_reset(MS_U16 u16Reset)
{
}

//=========================================================//
// Function : Hal_DVI_irq_info
// Description:
//=========================================================//
MS_U16 Hal_DVI_irq_info(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U16 u16reg_val = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_val = R2BYTEMSK(REG_DVI_ATOP_70_L, irqbit); break;
        case INPUT_PORT_DVI1:  u16reg_val = R2BYTEMSK(REG_DVI_ATOP1_70_L, irqbit); break;
        case INPUT_PORT_DVI3:  u16reg_val = R2BYTEMSK(REG_DVI_ATOP2_70_L, irqbit); break;
        case INPUT_PORT_DVI2:  u16reg_val = R2BYTEMSK(REG_DVI_ATOP_72_L, irqbit); break;
    }

    return u16reg_val;
}

//=========================================================//
// Function : Hal_DVI_irq_clear
// Description:
//=========================================================//
void Hal_DVI_irq_mask(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U32 u16reg_add = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_add = REG_DVI_ATOP_70_L;  break;
        case INPUT_PORT_DVI1:  u16reg_add = REG_DVI_ATOP1_70_L;  break;
        case INPUT_PORT_DVI3:  u16reg_add = REG_DVI_ATOP2_70_L;  break;
        case INPUT_PORT_DVI2:  u16reg_add = REG_DVI_ATOP_72_L;  break;
    }

    W2BYTEMSK(u16reg_add, (irqbit <<8), (IRQ_ALL_BIT<<8));
}

//=========================================================//
// Function : Hal_DVI_irq_forcemode
// Description:
//=========================================================//
void Hal_DVI_irq_forcemode(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U32 u16reg_add = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_add = REG_DVI_ATOP_71_L;  break;
        case INPUT_PORT_DVI1:  u16reg_add = REG_DVI_ATOP1_71_L;  break;
        case INPUT_PORT_DVI3:  u16reg_add = REG_DVI_ATOP2_71_L;  break;
        case INPUT_PORT_DVI2:  u16reg_add = REG_DVI_ATOP_73_L;  break;
    }

    W2BYTEMSK(u16reg_add, irqbit, IRQ_ALL_BIT);
}

//=========================================================//
// Function : Hal_DVI_irq_clear
// Description:
//=========================================================//
void Hal_DVI_irq_clear(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U32 u16reg_add = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_add = REG_DVI_ATOP_71_L;  break;
        case INPUT_PORT_DVI1:  u16reg_add = REG_DVI_ATOP1_71_L;  break;
        case INPUT_PORT_DVI3:  u16reg_add = REG_DVI_ATOP2_71_L;  break;
        case INPUT_PORT_DVI2:  u16reg_add = REG_DVI_ATOP_73_L;  break;
    }

    W2BYTEMSK(u16reg_add, (irqbit<<8), (IRQ_ALL_BIT<<8));
}

//=========================================================//
// Function : Hal_DVI_ClkPullLow
// Description:
//=========================================================//
void Hal_DVI_ClkPullLow(MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType)
{
#if 0//defined(MSTAR_IMMESWITCH)

    // DVI clock is always set to high becasue of Instaport
    PM_W2BYTE(REG_PM_SLEEP_4B_L, FALSE ? BMASK(11:8) : 0, BMASK(11:8));

#else

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0://A
            PM_W2BYTE(REG_PM_SLEEP_4B_L, bPullLow ? BIT(8) : 0, BMASK(8:8));
            break;
        case INPUT_PORT_DVI1://B
            PM_W2BYTE(REG_PM_SLEEP_4B_L, bPullLow ? BIT(9) : 0, BMASK(9:9));
            break;
        case INPUT_PORT_DVI3://D
            PM_W2BYTE(REG_PM_SLEEP_4B_L, bPullLow ? BIT(10) : 0, BMASK(10:10));
            break;
        case INPUT_PORT_DVI2://C
            PM_W2BYTE(REG_PM_SLEEP_4B_L, bPullLow ? BIT(11) : 0, BMASK(11:11));
            break;
        default: // For system first power on with the unknow HDMI port.
            PM_W2BYTE(REG_PM_SLEEP_4B_L, bPullLow ? BMASK(11:8) : 0, BMASK(11:8));
            break;
    }

#endif // #if defined(MSTAR_IMMESWITCH)
}

//=========================================================//
// Function : Hal_DVI_SwitchSrc
// Description:
//=========================================================//
void Hal_DVI_SwitchSrc(E_MUX_INPUTPORT enInputPortType, MS_U8 ucHDMIInfoSource)
{
    if(ucHDMIInfoSource)
    {

    }

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0: // A
            W2BYTEMSK(REG_DVI_ATOP_6A_L, 0, BMASK(1:0));
            break;

        case INPUT_PORT_DVI1: // B
            W2BYTEMSK(REG_DVI_ATOP_6A_L, 1, BMASK(1:0));
            break;

        case INPUT_PORT_DVI3: // D
            W2BYTEMSK(REG_DVI_ATOP_6A_L, 2, BMASK(1:0));
            break;

        case INPUT_PORT_DVI2: // C
            W2BYTEMSK(REG_DVI_ATOP_6A_L, 3, BMASK(1:0));
            break;
        default:
            //ASSERT("Not support!\n");
            break;
    }
}

void HAL_HDMI_DDCRam_SelectPort(E_XC_DDCRAM_PROG_TYPE eDDCRamType)
{
    switch(eDDCRamType)
    {
        case E_XC_PROG_DVI0_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI0),REG_DDC_SRAM_SEL_MASK);
            break;
        case E_XC_PROG_DVI1_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI1),REG_DDC_SRAM_SEL_MASK);
            break;
        case E_XC_PROG_DVI2_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI2),REG_DDC_SRAM_SEL_MASK);
            break;
        case E_XC_PROG_DVI3_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI3),REG_DDC_SRAM_SEL_MASK);
            break;
        case E_XC_PROG_VGA_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_ADC),BMASK(14:14));
            break;
        default:
            //ASSERT("Not support!\n");
            break;
    }
}

void HAL_HDMI_DDCRAM_SetPhyAddr(XC_DDCRAM_PROG_INFO *pstDDCRam_Info)
{
	UNUSED(pstDDCRam_Info);
}


//=========================================================//
// Function : Hal_DVI_GetPowerSavingStatus
// Description: Get power saving mode status
// return:  TRUE: stable, FALSE: unstable
//=========================================================//
MS_BOOL Hal_DVI_GetPowerSavingStatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U8 u8offset;
    MS_BOOL result;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u8offset = 0x00;  break;
        case INPUT_PORT_DVI1:  u8offset = 0x40;  break;
        case INPUT_PORT_DVI3:  u8offset = 0x80;  break;
        case INPUT_PORT_DVI2:  u8offset = 0xC0;  break;
    }

    result = (R2BYTEMSK(REG_DVI_PS_00_L+u8offset, BIT(0)) & BIT(0)) ? TRUE : FALSE;

    return result;
}

//=========================================================//
// Function : Hal_DVI_GetDEStableStatus
// Description: Get DE stable status for Immeswitch power saving mode
// return:  TRUE: stable, FALSE: unstable
//=========================================================//
MS_BOOL Hal_DVI_GetDEStableStatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16bank_offset;
    MS_BOOL result;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x110A00
        case INPUT_PORT_DVI1:  u16bank_offset = 0x2900;  break; // BK 0x113300
        case INPUT_PORT_DVI3:  u16bank_offset = 0x2B00;  break; // BK 0x113500
        case INPUT_PORT_DVI2:  u16bank_offset = 0x2D00;  break; // BK 0x113700
    }

    result = (R2BYTEMSK(REG_DVI_DTOP_31_L+u16bank_offset, 0x0070) & BIT(6)) ? TRUE : FALSE;

    return result;
}

//=========================================================//
// Function : Hal_DVI_EnhanceImmeswitch
// Description: Enable / Disable Immeswitch enhancement
//=========================================================//
void Hal_DVI_EnhanceImmeswitch(E_MUX_INPUTPORT enInputPortType, MS_BOOL bflag)
{
    MS_U8 u8offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u8offset = 0x00;  break;
        case INPUT_PORT_DVI1:  u8offset = 0x40;  break;
        case INPUT_PORT_DVI3:  u8offset = 0x80;  break;
        case INPUT_PORT_DVI2:  u8offset = 0xC0;  break;
    }

    W2BYTEMSK(REG_DVI_PS_01_L+u8offset, bflag ? BMASK(9:8) : 0, BMASK(9:8)); // [9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
}

//=========================================================//
// Function : Hal_DVI_ForceAllPortsEnterPS
// Description: Force all DVI ports enter power saving mode
//=========================================================//
void Hal_DVI_ForceAllPortsEnterPS(void)
{
    // DVI slowly update
    W2BYTEMSK( REG_DVI_DTOP_29_L, 0, BIT(15)); // [15]: turn off slowly updated
    W2BYTEMSK( REG_DVI_DTOP_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
    W2BYTEMSK( REG_DVI_DTOP1_29_L, 0, BIT(15)); // [15]: turn off slowly updated
    W2BYTEMSK( REG_DVI_DTOP1_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
    W2BYTEMSK( REG_DVI_DTOP2_29_L, 0, BIT(15)); // [15]: turn off slowly updated
    W2BYTEMSK( REG_DVI_DTOP2_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
    W2BYTEMSK( REG_DVI_DTOP3_29_L, 0, BIT(15)); // [15]: turn off slowly updated
    W2BYTEMSK( REG_DVI_DTOP3_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
    // DVI power saving mode
    W2BYTEMSK(REG_DVI_PS_00_L, BIT(0), BIT(0)); // turn on DVI power saving mode
    W2BYTEMSK(REG_DVI_PS1_00_L, BIT(0), BIT(0)); // turn on DVI1 power saving mode
    W2BYTEMSK(REG_DVI_PS2_00_L, BIT(0), BIT(0)); // turn on DVI2 power saving mode
    W2BYTEMSK(REG_DVI_PS3_00_L, BIT(0), BIT(0)); // turn on DVI3 power saving mode
    // Turn off Ch2 as DE align out and cycle align delay
    W2BYTEMSK(REG_DVI_PS_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
    W2BYTEMSK(REG_DVI_PS1_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
    W2BYTEMSK(REG_DVI_PS2_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
    W2BYTEMSK(REG_DVI_PS3_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                HDMI 1.4 new feature:
//                                1. 3D format
//                                2. 4K x 2K format
//                                3. ARC
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// HDMI_Video_Format: Vendor Specifc Info-frame, PB4[7:5]
//   000: No additional HDMI video format is presented in this packet
//   001: Extended resolution format (e.g. 4Kx2K video) present
//   010: 3D format indication present
//   011~111: Reserved
E_HDMI_ADDITIONAL_VIDEO_FORMAT Hal_MHL_TMDS_Check_Additional_Format(void)
{
    E_HDMI_ADDITIONAL_VIDEO_FORMAT val;

    if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(7:5)) == 0)
        val = E_HDMI_NO_ADDITIONAL_FORMAT;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(7:5)) == BIT(5))
        val = E_HDMI_4Kx2K_FORMAT;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(7:5)) == BIT(6))
        val = E_HDMI_3D_FORMAT;
    else
        val = E_HDMI_RESERVED;

    return val;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                HDMI 1.4 new feature:
//                                1. 3D format
//                                2. 4K x 2K format
//                                3. ARC
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// HDMI_Video_Format: Vendor Specifc Info-frame, PB4[7:5]
//   000: No additional HDMI video format is presented in this packet
//   001: Extended resolution format (e.g. 4Kx2K video) present
//   010: 3D format indication present
//   011~111: Reserved
E_HDMI_ADDITIONAL_VIDEO_FORMAT Hal_HDMI_Check_Additional_Format(MS_U8 ucHDMIInfoSource)
{
    E_HDMI_ADDITIONAL_VIDEO_FORMAT val;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_Check_Additional_Format();
    }

    if (R2BYTEMSK(REG_HDMI_34_L, BMASK(7:5)) == 0)
        val = E_HDMI_NO_ADDITIONAL_FORMAT;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(7:5)) == BIT(5))
        val = E_HDMI_4Kx2K_FORMAT;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(7:5)) == BIT(6))
        val = E_HDMI_3D_FORMAT;
    else
        val = E_HDMI_RESERVED;

    return val;
}

// 3D_Structure: Vendor Specifc Info-frame, PB5[7:4]
//   0000: Frame packing
//   0001: Field alternative
//   0010: Line alternative
//   0011: Side-by-Side(Full)
//   0100: L+depth
//   0101: L+depth+graphics+graphics-depth
//   0110: Top-and-Bottom
//   0111: Reserved
//   1000: Side-by-Side(Half)
//   1001 ~ 1111: Reserved
E_XC_3D_INPUT_MODE Hal_MHL_TMDS_Get_3D_Structure(void)
{
    E_XC_3D_INPUT_MODE val;

    if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == 0)
        val = E_XC_3D_INPUT_FRAME_PACKING;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == BIT(12))
        val = E_XC_3D_INPUT_FIELD_ALTERNATIVE;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == BIT(13))
        val = E_XC_3D_INPUT_LINE_ALTERNATIVE;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == (BIT(13)|BIT(12)))
        val = E_XC_3D_INPUT_SIDE_BY_SIDE_FULL;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == BIT(14))
        val = E_XC_3D_INPUT_L_DEPTH;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == (BIT(14)|BIT(12)))
        val = E_XC_3D_INPUT_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == (BIT(14)|BIT(13)))
        val = E_XC_3D_INPUT_TOP_BOTTOM;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:12)) == BIT(15))
        val = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
    else
        val = E_XC_3D_INPUT_MODE_NONE;

    return val;
}

// 3D_Structure: Vendor Specifc Info-frame, PB5[7:4]
//   0000: Frame packing
//   0001: Field alternative
//   0010: Line alternative
//   0011: Side-by-Side(Full)
//   0100: L+depth
//   0101: L+depth+graphics+graphics-depth
//   0110: Top-and-Bottom
//   0111: Reserved
//   1000: Side-by-Side(Half)
//   1001 ~ 1111: Reserved
E_XC_3D_INPUT_MODE Hal_HDMI_Get_3D_Structure(MS_U8 ucHDMIInfoSource)
{
    E_XC_3D_INPUT_MODE val;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_Get_3D_Structure();
    }

    if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == 0)
        val = E_XC_3D_INPUT_FRAME_PACKING;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == BIT(12))
        val = E_XC_3D_INPUT_FIELD_ALTERNATIVE;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == BIT(13))
        val = E_XC_3D_INPUT_LINE_ALTERNATIVE;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == (BIT(13)|BIT(12)))
        val = E_XC_3D_INPUT_SIDE_BY_SIDE_FULL;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == BIT(14))
        val = E_XC_3D_INPUT_L_DEPTH;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == (BIT(14)|BIT(12)))
        val = E_XC_3D_INPUT_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == (BIT(14)|BIT(13)))
        val = E_XC_3D_INPUT_TOP_BOTTOM;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:12)) == BIT(15))
        val = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
    else
        val = E_XC_3D_INPUT_MODE_NONE;

    return val;
}

// 3D_Ext_Data: Vendor Specifc Info-frame, PB6[7:4]
//   0000 ~ 0011 : Horizontal sub-sampling
//   0100: Quincunx matrix - Odd/Left  picture, Odd/Right  picture
//   0101: Quincunx matrix - Odd/Left  picture, Even/Right picture
//   0110: Quincunx matrix - Even/Left picture, Odd/Right  picture
//   0111: Quincunx matrix - Even/Left picture, Even/Right picture
//   1000 ~ 1111: Reserved
E_HDMI_3D_EXT_DATA_T Hal_MHL_TMDS_Get_3D_Ext_Data(void)
{
    E_HDMI_3D_EXT_DATA_T e_3d_extdata;
    MS_U16 u16temp;

    u16temp = R2BYTEMSK(REG_MHL_TMDS_35_L, BMASK(7:4));

    switch(u16temp)
    {
        case 0x0000:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_0;
            break;
        case 0x0010:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_1;
            break;
        case 0x0020:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_2;
            break;
        case 0x0030:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_3;
            break;
        case 0x0040:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_0;
            break;
        case 0x0050:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_1;
            break;
        case 0x0060:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_2;
            break;
        case 0x0070:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_3;
            break;
        default:
            e_3d_extdata = E_3D_EXT_DATA_MODE_MAX;
            break;
    }
    return e_3d_extdata;
}

// 3D_Ext_Data: Vendor Specifc Info-frame, PB6[7:4]
//   0000 ~ 0011 : Horizontal sub-sampling
//   0100: Quincunx matrix - Odd/Left  picture, Odd/Right  picture
//   0101: Quincunx matrix - Odd/Left  picture, Even/Right picture
//   0110: Quincunx matrix - Even/Left picture, Odd/Right  picture
//   0111: Quincunx matrix - Even/Left picture, Even/Right picture
//   1000 ~ 1111: Reserved
E_HDMI_3D_EXT_DATA_T Hal_HDMI_Get_3D_Ext_Data(MS_U8 ucHDMIInfoSource)
{
    E_HDMI_3D_EXT_DATA_T e_3d_extdata;
    MS_U16 u16temp;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_Get_3D_Ext_Data();
    }

    u16temp = R2BYTEMSK(REG_HDMI_35_L, BMASK(7:4));

    switch(u16temp)
    {
        case 0x0000:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_0;
            break;
        case 0x0010:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_1;
            break;
        case 0x0020:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_2;
            break;
        case 0x0030:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_3;
            break;
        case 0x0040:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_0;
            break;
        case 0x0050:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_1;
            break;
        case 0x0060:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_2;
            break;
        case 0x0070:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_3;
            break;
        default:
            e_3d_extdata = E_3D_EXT_DATA_MODE_MAX;
            break;
    }
    return e_3d_extdata;
}

// 3D_Meta_Field: Vendor Specifc Info-frame, PB5[3], PB7 ~ PB7+N
//   - 3D_Meta_Present: PB5[3]
//   - 3D_Metadata_Type: PB7[7:5]
//       000: parallax information defined in ISO23002-3
//       001 ~ 111: reserved
//   - 3D_Metadata_Length(N, N <= 31-8 = 23): PB7[4:0]
//   - 3D_Metadata[] : PB8 ~ PB7+N
void Hal_MHL_TMDS_Get_3D_Meta_Field(sHDMI_3D_META_FIELD *pdata)
{
    MS_U8 i;
    MS_U16 u16temp;

    pdata->b3D_Meta_Present = R2BYTEMSK(REG_MHL_TMDS_34_L, BIT(11)) > 0 ? TRUE : FALSE;
    u16temp = R2BYTEMSK(REG_MHL_TMDS_35_L, BMASK(15:13));
    switch(u16temp)
    {
        case 0x0000:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_PARALLAX_ISO23002_3;
            break;
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_RESERVE;
            break;
        default:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_MAX;
            break;
    }
    pdata->u83D_Metadata_Length = (MS_U8)(R2BYTEMSK(REG_MHL_TMDS_35_L, BMASK(12:8)) >> 8);
    for(i=0;i<pdata->u83D_Metadata_Length;i++)
    {
        pdata->u83D_Metadata[i] = R2BYTEMSK(REG_MHL_TMDS_36_L+i, BMASK(7:0));
    }
}

// 3D_Meta_Field: Vendor Specifc Info-frame, PB5[3], PB7 ~ PB7+N
//   - 3D_Meta_Present: PB5[3]
//   - 3D_Metadata_Type: PB7[7:5]
//       000: parallax information defined in ISO23002-3
//       001 ~ 111: reserved
//   - 3D_Metadata_Length(N, N <= 31-8 = 23): PB7[4:0]
//   - 3D_Metadata[] : PB8 ~ PB7+N
void Hal_HDMI_Get_3D_Meta_Field(MS_U8 ucHDMIInfoSource, sHDMI_3D_META_FIELD *pdata)
{
    MS_U8 i;
    MS_U16 u16temp;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        Hal_MHL_TMDS_Get_3D_Meta_Field(pdata);
        return;
    }

    pdata->b3D_Meta_Present = R2BYTEMSK(REG_HDMI_34_L, BIT(11)) > 0 ? TRUE : FALSE;
    u16temp = R2BYTEMSK(REG_HDMI_35_L, BMASK(15:13));
    switch(u16temp)
    {
        case 0x0000:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_PARALLAX_ISO23002_3;
            break;
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_RESERVE;
            break;
        default:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_MAX;
            break;
    }
    pdata->u83D_Metadata_Length = (MS_U8)(R2BYTEMSK(REG_HDMI_35_L, BMASK(12:8)) >> 8);
    for(i=0;i<pdata->u83D_Metadata_Length;i++)
    {
        pdata->u83D_Metadata[i] = R2BYTEMSK(REG_HDMI_36_L+i, BMASK(7:0));
    }
}

// VIC_CODE: Auxiliary Video Information Info-frame, PB4[6:0]
MS_U8 Hal_MHL_TMDS_Get_VIC_Code(void)
{
    MS_U8 val;

    val = R2BYTEMSK(REG_MHL_TMDS_41_L, BMASK(14:8)) >> 8;
    return val;
}

// VIC_CODE: Auxiliary Video Information Info-frame, PB4[6:0]
MS_U8 Hal_HDMI_Get_VIC_Code(MS_U8 ucHDMIInfoSource)
{
    MS_U8 val;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_Get_VIC_Code();
    }

    val = R2BYTEMSK(REG_HDMI_41_L, BMASK(14:8)) >> 8;
    return val;
}

// 4Kx2K VIC code: Vendor Specifc Info-frame, PB5[7:0]
//   0x01: 1920(x2)x1080(x2) @ 29.97/30Hz
//   0x02: 1920(x2)x1080(x2) @ 25Hz
//   0x03: 1920(x2)x1080(x2) @ 23.976/24Hz
//   0x04: 2048(x2)x1080(x2) @ 24Hz
//   0x00, 0x05 ~ 0xFF: Reserved
E_HDMI_VIC_4Kx2K_CODE Hal_MHL_TMDS_Get_4Kx2K_VIC_Code(void)
{
    E_HDMI_VIC_4Kx2K_CODE val;

    if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:8)) >> 8 == 0x01)
        val = E_VIC_4Kx2K_30Hz;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:8)) >> 8 == 0x02)
        val = E_VIC_4Kx2K_25Hz;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:8)) >> 8 == 0x03)
        val = E_VIC_4Kx2K_24Hz;
    else if (R2BYTEMSK(REG_MHL_TMDS_34_L, BMASK(15:8)) >> 8 == 0x04)
        val = E_VIC_4Kx2K_24Hz_SMPTE;
    else
        val = E_VIC_RESERVED;

    return val;
}

// 4Kx2K VIC code: Vendor Specifc Info-frame, PB5[7:0]
//   0x01: 1920(x2)x1080(x2) @ 29.97/30Hz
//   0x02: 1920(x2)x1080(x2) @ 25Hz
//   0x03: 1920(x2)x1080(x2) @ 23.976/24Hz
//   0x04: 2048(x2)x1080(x2) @ 24Hz
//   0x00, 0x05 ~ 0xFF: Reserved
E_HDMI_VIC_4Kx2K_CODE Hal_HDMI_Get_4Kx2K_VIC_Code(MS_U8 ucHDMIInfoSource)
{
    E_HDMI_VIC_4Kx2K_CODE val;

    if(ucHDMIInfoSource > 0)
    {

    }

    if (Hal_HDMI_Check_MHL_Attach())
    {
        return Hal_MHL_TMDS_Get_4Kx2K_VIC_Code();
    }

    if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:8)) >> 8 == 0x01)
        val = E_VIC_4Kx2K_30Hz;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:8)) >> 8 == 0x02)
        val = E_VIC_4Kx2K_25Hz;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:8)) >> 8 == 0x03)
        val = E_VIC_4Kx2K_24Hz;
    else if (R2BYTEMSK(REG_HDMI_34_L, BMASK(15:8)) >> 8 == 0x04)
        val = E_VIC_4Kx2K_24Hz_SMPTE;
    else
        val = E_VIC_RESERVED;

    return val;
}

// It must set to non-bypass(0) when input source is detected as HDMI 3D format
// 0: no bypass, 1: bypass
void Hal_HDMI_Set_YUV422to444_Bypass(MS_BOOL btrue)
{
    W2BYTEMSK(REG_HDMI2_20_L, btrue, BIT(0));
}

// Check whether input pixel rate is large 166MHz or not.
MS_BOOL Hal_HDMI_Is_Input_Large_166MHz(MS_U8 ucHDMIInfoSource)
{
    MS_BOOL bflag = FALSE;
    MS_U16 reg_val;

    if(ucHDMIInfoSource > 0)
    {

    }

    reg_val = R2BYTEMSK(REG_HDMI2_36_L, BMASK(12:0));
    if ((reg_val/128)*12 >= 166)
        bflag = TRUE;

    return bflag;
}

// If input format is 3D or 4Kx2K and pixel rate large than 166MHz, it need to do the horizontal scaling down.
void Hal_HDMI_AVG_ScaleringDown(MS_U8 ucHDMIInfoSource, MS_BOOL btrue)
{
    if(ucHDMIInfoSource > 0)
    {

    }

    if(btrue)
    {
        return; //Einstein support 300Mhz
        //W2BYTEMSK(REG_HDMI2_20_L, BIT(5), BIT(5)|BIT(3)|BIT(2)); // [5]: 1: WR_Clock_Freq/2, [3:2]: 2'b 00: AVG and FIFO enable
    }
    else
    {
        W2BYTEMSK(REG_HDMI2_20_L, BIT(2), BIT(5)|BIT(3)|BIT(2)); // [5]: 0: WR_Clock_Freq, [3:2]: 2'b 01: AVG bypass and FIFO enable
    }
}

// HDMI ARC pin control
//     - enInputPortType: INPUT_PORT_DVI0 / INPUT_PORT_DVI1 / INPUT_PORT_DVI2 / INPUT_PORT_DVI3
//     - bEnable: ARC enable or disable
//     - bDrivingHigh: ARC driving current high or low
void Hal_HDMI_ARC_PINControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0: // A
            W2BYTEMSK(REG_HDMI2_15_L, bEnable ? BIT(0) : BMASK(2:1), BMASK(2:0));
            break;
        case INPUT_PORT_DVI1: // B
            W2BYTEMSK(REG_HDMI2_15_L, bEnable ? BIT(3) : BMASK(5:4), BMASK(5:3));
            break;
        case INPUT_PORT_DVI3: // D
            W2BYTEMSK(REG_HDMI2_15_L, bEnable ? BIT(6) : BMASK(8:7), BMASK(8:6));
            break;
        case INPUT_PORT_DVI2: // C
            W2BYTEMSK(REG_HDMI2_15_L, bEnable ? BIT(9) : BMASK(11:10), BMASK(11:9));
            break;
        default:
            break;
    }

    // Driving current
    W2BYTEMSK(REG_HDMI2_15_L, bDrivingHigh ? BIT(12) : 0, BIT(12));
}


//=========================================================//
// Function : Hal_HDMI_Calibration
// Description:
//=========================================================//
MS_BOOL Hal_HDMI_Calibration( MS_U8 Port )
{
    MS_BOOL Calibration_Flag = TRUE;
    MS_U8 i=0;
    MS_U16 Clk_flag= 0;

    switch(Port)
    {
        case 0: //PortA
            for(i=0;i<3;i++)
            {
                Clk_flag = PM_R2BYTE(REG_PM_SLEEP_3F_L, 0x8000);
		        //printf("PACLK:0x%x\r\n",PM_R2BYTE(REG_PM_SLEEP_3F_L, 0xFFFF));
                if(Clk_flag==0) return FALSE;
            }
               W2BYTE(REG_DVI_ATOP_62_L, 0x0000);//Clear Calibration, [8:6]: RGB calibration enable.
               W2BYTE(REG_DVI_ATOP_67_L, 0x0008);//switch to mpl
               W2BYTE(REG_DVI_ATOP_64_L, 0x0A92);//clock on, clear garbage
               W2BYTE(REG_DVI_ATOP_65_L, 0x0A00);
               MsOS_DelayTask(1);
               W2BYTE(REG_DVI_ATOP_67_L, 0x0000);//Reset VCO
               W2BYTE(REG_DVI_ATOP_64_L, 0x0100);//Set DVI_PL VCO = 0
               MsOS_DelayTask(1);
               W2BYTE(REG_DVI_ATOP_62_L, 0x01C0);//Enable Calibration
               W2BYTE(REG_DVI_ATOP_67_L, 0x0008);//switch to mpl
               MsOS_DelayTask(1);
               W2BYTE(REG_DVI_ATOP_64_L, 0x0A92);//clock on, clear garbage
               MsOS_DelayTask(1);
            if(!(R2BYTE(REG_DVI_ATOP_68_L)&(0x70)))//check calibration end flags
            {
               W2BYTE(REG_DVI_ATOP_62_L, 0x0000);
               Calibration_Flag = FALSE;
            }

            Clk_flag = PM_R2BYTE(REG_PM_SLEEP_3F_L, 0x8000);
            if(Clk_flag==0)
            {
	           W2BYTE(REG_DVI_ATOP_62_L, 0x0000);
               Calibration_Flag = FALSE;
	        }

            W2BYTE(REG_DVI_ATOP_67_L, 0x0000);
            W2BYTE(REG_DVI_ATOP_64_L, 0x0000);
			W2BYTE(REG_DVI_ATOP_65_L, 0x0000);
            break;

        case 1://PortB
            for(i=0;i<3;i++)
            {
                Clk_flag = PM_R2BYTE(REG_PM_SLEEP_42_L, 0x8000);
		        //printf("PBCLK:0x%x\r\n",PM_R2BYTE(REG_PM_SLEEP_42_L, 0xFFFF));
                if(Clk_flag==0) return FALSE;
            }
	        W2BYTE(REG_DVI_ATOP1_62_L, 0x0000);//Clear Calibration
            W2BYTE(REG_DVI_ATOP1_67_L, 0x0008);//switch to mpl
            W2BYTE(REG_DVI_ATOP1_64_L, 0x0A92);//clock on, clear garbage
            W2BYTE(REG_DVI_ATOP1_65_L, 0x0A00);
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP1_67_L, 0x0000);//Reset VCO
            W2BYTE(REG_DVI_ATOP1_64_L, 0x0100);//Set DVI_PL VCO = 0
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP1_62_L, 0x01C0);//Enable Calibration
            W2BYTE(REG_DVI_ATOP1_67_L, 0x0008);//switch to mpl
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP1_64_L, 0x0A92);//clock on, clear garbage
            MsOS_DelayTask(1);
            if(!(R2BYTE(REG_DVI_ATOP1_68_L)&(0x70)))
            {
                W2BYTE(REG_DVI_ATOP1_62_L, 0x0000);
                Calibration_Flag = FALSE;
            }

            Clk_flag = PM_R2BYTE(REG_PM_SLEEP_42_L, 0x8000);
            if(Clk_flag==0)
            {
	            W2BYTE(REG_DVI_ATOP1_62_L, 0x0000);
                Calibration_Flag = FALSE;
	        }

            W2BYTE(REG_DVI_ATOP1_67_L, 0x0000);
            W2BYTE(REG_DVI_ATOP1_64_L, 0x0000);
			W2BYTE(REG_DVI_ATOP1_65_L, 0x0000);
            break;

        case 2://PortD
            for(i=0;i<3;i++)
            {
                Clk_flag = PM_R2BYTE(REG_PM_SLEEP_45_L, 0x8000);
                //printf("PADLK:0x%x\r\n",PM_R2BYTE(REG_PM_SLEEP_45_L, 0xFFFF));
                if(Clk_flag==0) return FALSE;
            }
            W2BYTE(REG_DVI_ATOP2_62_L, 0x0000);//Clear Calibration
            W2BYTE(REG_DVI_ATOP2_67_L, 0x0008);//switch to mpl
            W2BYTE(REG_DVI_ATOP2_64_L, 0x0A92);//clock on, clear garbage
            W2BYTE(REG_DVI_ATOP2_65_L, 0x0A00);
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP2_67_L, 0x0000);//Reset VCO
            W2BYTE(REG_DVI_ATOP2_64_L, 0x0100);//Set DVI_PL VCO = 0
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP2_62_L, 0x01C0);//Enable Calibration
            W2BYTE(REG_DVI_ATOP2_67_L, 0x0008);//switch to mpl
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP2_64_L, 0x0A92);//clock on, clear garbage
            MsOS_DelayTask(1);
            if(!(R2BYTE(REG_DVI_ATOP2_68_L)&(0x70)))
            {
                W2BYTE(REG_DVI_ATOP2_62_L, 0x0000);
                Calibration_Flag = FALSE;
            }

            Clk_flag = PM_R2BYTE(REG_PM_SLEEP_45_L, 0x8000);
            if(Clk_flag==0)
            {
	            W2BYTE(REG_DVI_ATOP2_62_L, 0x0000);
                Calibration_Flag = FALSE;
	        }

            W2BYTE(REG_DVI_ATOP2_67_L, 0x0000);
            W2BYTE(REG_DVI_ATOP2_64_L, 0x0000);
			W2BYTE(REG_DVI_ATOP2_65_L, 0x0A00);
            break;

		case 3://PortC
            for(i=0;i<3;i++)
            {
                 Clk_flag = PM_R2BYTE(REG_PM_SLEEP_48_L, 0x8000);
		        //printf("PCCLK:0x%x\r\n",PM_R2BYTE(REG_PM_SLEEP_48_L, 0xFFFF));
		         if(Clk_flag==0) return FALSE;
            }
	        W2BYTE(REG_DVI_ATOP3_62_L, 0x0000);//Clear Calibration
            W2BYTE(REG_DVI_ATOP3_67_L, 0x0008);//switch to mpl
            W2BYTE(REG_DVI_ATOP3_64_L, 0x0A92);//clock on, clear garbage
            W2BYTE(REG_DVI_ATOP3_65_L, 0x0A00);
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP3_67_L, 0x0000);//Reset VCO
            W2BYTE(REG_DVI_ATOP3_64_L, 0x0100);//Set DVI_PL VCO = 0
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP3_62_L, 0x01C0);//Enable Calibration
            W2BYTE(REG_DVI_ATOP3_67_L, 0x0008);//switch to mpl
            MsOS_DelayTask(1);
            W2BYTE(REG_DVI_ATOP3_64_L, 0x0A92);//clock on, clear garbage
            MsOS_DelayTask(1);
            if(!(R2BYTE(REG_DVI_ATOP3_68_L)&(0x70)))
            {
                W2BYTE(REG_DVI_ATOP3_62_L, 0x0000);
                Calibration_Flag = FALSE;
            }

			Clk_flag = PM_R2BYTE(REG_PM_SLEEP_48_L, 0x8000);
			if(Clk_flag==0)
			{
			    W2BYTE(REG_DVI_ATOP3_62_L, 0x0000);
			    Calibration_Flag = FALSE;
			}

            W2BYTE(REG_DVI_ATOP3_67_L, 0x0000);
            W2BYTE(REG_DVI_ATOP3_64_L, 0x0000);
			W2BYTE(REG_DVI_ATOP3_65_L, 0x0000);
            break;
    }

    return Calibration_Flag;
}

/******************************************************************************/
///DVI soft reset
/******************************************************************************/
//[6]: soft-reset hdmi
//[5]: soft-reset hdcp
//[4]: soft-reset dvi
void Hal_DVI_Software_Reset(E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    MS_U32 u32RegAdr;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0://A
            u32RegAdr = REG_DVI_ATOP_07_L;
            break;
        case INPUT_PORT_DVI1://B
            u32RegAdr = REG_DVI_ATOP1_07_L;
            break;
        case INPUT_PORT_DVI3://D
            u32RegAdr = REG_DVI_ATOP2_07_L;
            break;
        case INPUT_PORT_DVI2://C
            u32RegAdr = REG_DVI_ATOP3_07_L;
            break;
        default:
            printf("enInputPortType=0x%X error at %s %u\n", enInputPortType, __FUNCTION__, __LINE__);
            return;
            break;
    }
    //printf("daniel_test u32RegAdr=0x%lX\n", u32RegAdr );

    W2BYTE(u32RegAdr, u16Reset);
    //OS_DELAY_TASK(1); // delay 1 usec if possible
    /*{ // For system delay
        MS_U32 t;
        t = MsOS_GetSystemTime();
        t = MsOS_GetSystemTime();
    }*/
    W2BYTE(u32RegAdr, 0x0000);
#else
    // Do nothing
#endif
}

MS_U16 Hal_HDMI_GetTMDSFreq(void)
{
    return 0;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_DVI_HF_adjust()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void Hal_DVI_HF_adjust(void)
{
    // Do nothing
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_StablePolling()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void Hal_HDMI_StablePolling(MS_U8 ucMHLSupportPath, stHDMI_POLLING_INFO *stHDMIPollingInfo)
{
    MS_U8 ucDivValue = 0;
    MS_U16 usClockRate = 0;
    
    // port A
    if(R2BYTE(REG_DVI_DTOP_16_L) & BIT(9)) // clk stable
    {
        if(!stHDMIPollingInfo[0].bTimingStableFlag)
        {
            if(!_Hal_HDMI_GetMHLAttachPort(INPUT_PORT_DVI0, ucMHLSupportPath))
            {
                ucDivValue = (R2BYTE(REG_DVI_ATOP_61_L) &BMASK(9:8)) >> 8;

                if(ucDivValue == 0)
                {
                    W2BYTEMSK(REG_DVI_ATOP_32_L, (HDMI_HF_SETTING_VALUE_HIGH << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 1)
                {
                    W2BYTEMSK(REG_DVI_ATOP_32_L, (HDMI_HF_SETTING_VALUE_MEDIUM << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 2)
                {
                    W2BYTEMSK(REG_DVI_ATOP_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_MEDIUM, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else
                {
                    W2BYTEMSK(REG_DVI_ATOP_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_LOW, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
            }

            usClockRate = (R2BYTE(REG_DVI_DTOP_17_L) & BMASK(11:0)) *12 /128;

            if(usClockRate > 280)
            {
                W2BYTEMSK(REG_DVI_DTOP_31_L, 0x0500, BMASK(15:8));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_31_L, 0x0C00, BMASK(15:8));
            }

            stHDMIPollingInfo[0].bTimingStableFlag = TRUE;
        }

        if(!(R2BYTE(REG_DVI_DTOP_31_L) &BIT(6)))
        {
            W2BYTEMSK(REG_DVI_DTOP_24_L, BIT(14)| BIT(6), BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP_25_L, BIT(6), BIT(6));
            W2BYTEMSK(REG_DVI_DTOP_24_L, 0, BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP_25_L, 0, BIT(6));
        }
    }
    else if(stHDMIPollingInfo[0].bTimingStableFlag)
    {
        stHDMIPollingInfo[0].bTimingStableFlag = FALSE;
    }

    // port B
    if(R2BYTE(REG_DVI_DTOP1_16_L) & BIT(9)) // clk stable
    {
        if(!stHDMIPollingInfo[1].bTimingStableFlag)
        {
            if(!_Hal_HDMI_GetMHLAttachPort(INPUT_PORT_DVI1, ucMHLSupportPath))
            {
                ucDivValue = (R2BYTE(REG_DVI_ATOP1_61_L) &BMASK(9:8)) >> 8;

                if(ucDivValue == 0)
                {
                    W2BYTEMSK(REG_DVI_ATOP1_32_L, (HDMI_HF_SETTING_VALUE_HIGH << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 1)
                {
                    W2BYTEMSK(REG_DVI_ATOP1_32_L, (HDMI_HF_SETTING_VALUE_MEDIUM << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 2)
                {
                    W2BYTEMSK(REG_DVI_ATOP1_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_MEDIUM, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else
                {
                    W2BYTEMSK(REG_DVI_ATOP1_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_LOW, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
            }

            usClockRate = (R2BYTE(REG_DVI_DTOP1_17_L) & BMASK(11:0)) *12 /128;

            if(usClockRate > 280)
            {
                W2BYTEMSK(REG_DVI_DTOP1_31_L, 0x0500, BMASK(15:8));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP1_31_L, 0x0C00, BMASK(15:8));
            }

            stHDMIPollingInfo[1].bTimingStableFlag = TRUE;
        }

        if(!(R2BYTE(REG_DVI_DTOP1_31_L) &BIT(6)))
        {
            W2BYTEMSK(REG_DVI_DTOP1_24_L, BIT(14)| BIT(6), BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP1_25_L, BIT(6), BIT(6));
            W2BYTEMSK(REG_DVI_DTOP1_24_L, 0, BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP1_25_L, 0, BIT(6));
        }
    }
    else if(stHDMIPollingInfo[1].bTimingStableFlag)
    {
        stHDMIPollingInfo[1].bTimingStableFlag = FALSE;
    }

    // port C
    if(R2BYTE(REG_DVI_DTOP3_16_L) & BIT(9)) // clk stable
    {
        if(!stHDMIPollingInfo[3].bTimingStableFlag)
        {
            if(!_Hal_HDMI_GetMHLAttachPort(INPUT_PORT_DVI3, ucMHLSupportPath))
            {
                ucDivValue = (R2BYTE(REG_DVI_ATOP3_61_L) &BMASK(9:8)) >> 8;

                if(ucDivValue == 0)
                {
                    W2BYTEMSK(REG_DVI_ATOP3_32_L, (HDMI_HF_SETTING_VALUE_HIGH << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP3_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 1)
                {
                    W2BYTEMSK(REG_DVI_ATOP3_32_L, (HDMI_HF_SETTING_VALUE_MEDIUM << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP3_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 2)
                {
                    W2BYTEMSK(REG_DVI_ATOP3_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP3_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_MEDIUM, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else
                {
                    W2BYTEMSK(REG_DVI_ATOP3_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP3_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_LOW, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
            }

            usClockRate = (R2BYTE(REG_DVI_DTOP3_17_L) & BMASK(11:0)) *12 /128;

            if(usClockRate > 280)
            {
                W2BYTEMSK(REG_DVI_DTOP3_31_L, 0x0500, BMASK(15:8));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP3_31_L, 0x0C00, BMASK(15:8));
            }

            stHDMIPollingInfo[3].bTimingStableFlag = TRUE;
        }

        if(!(R2BYTE(REG_DVI_DTOP3_31_L) &BIT(6)))
        {
            W2BYTEMSK(REG_DVI_DTOP3_24_L, BIT(14)| BIT(6), BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP3_25_L, BIT(6), BIT(6));
            W2BYTEMSK(REG_DVI_DTOP3_24_L, 0, BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP3_25_L, 0, BIT(6));
        }
    }
    else if(stHDMIPollingInfo[3].bTimingStableFlag)
    {
        stHDMIPollingInfo[3].bTimingStableFlag = FALSE;
    }

    // port D
    if(R2BYTE(REG_DVI_DTOP2_16_L) & BIT(9)) // clk stable
    {
        if(!stHDMIPollingInfo[2].bTimingStableFlag)
        {
            if(!_Hal_HDMI_GetMHLAttachPort(INPUT_PORT_DVI2, ucMHLSupportPath))
            {
                ucDivValue = (R2BYTE(REG_DVI_ATOP2_61_L) &BMASK(9:8)) >> 8;

                if(ucDivValue == 0)
                {
                    W2BYTEMSK(REG_DVI_ATOP2_32_L, (HDMI_HF_SETTING_VALUE_HIGH << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 1)
                {
                    W2BYTEMSK(REG_DVI_ATOP2_32_L, (HDMI_HF_SETTING_VALUE_MEDIUM << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_HIGH, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else if(ucDivValue == 2)
                {
                    W2BYTEMSK(REG_DVI_ATOP2_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_MEDIUM, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
                else
                {
                    W2BYTEMSK(REG_DVI_ATOP2_32_L, (HDMI_HF_SETTING_VALUE_LOW << 8), BMASK(9:8));
                    W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4)| HDMI_I_CONTROL_VALUE_LOW, BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
                }
            }

            usClockRate = (R2BYTE(REG_DVI_DTOP2_17_L) & BMASK(11:0)) *12 /128;

            if(usClockRate > 280)
            {
                W2BYTEMSK(REG_DVI_DTOP2_31_L, 0x0500, BMASK(15:8));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP2_31_L, 0x0C00, BMASK(15:8));
            }

            stHDMIPollingInfo[2].bTimingStableFlag = TRUE;
        }

        if(!(R2BYTE(REG_DVI_DTOP2_31_L) &BIT(6)))
        {
            W2BYTEMSK(REG_DVI_DTOP2_24_L, BIT(14)| BIT(6), BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP2_25_L, BIT(6), BIT(6));
            W2BYTEMSK(REG_DVI_DTOP2_24_L, 0, BIT(14)| BIT(6));
            W2BYTEMSK(REG_DVI_DTOP2_25_L, 0, BIT(6));
        }
    }
    else if(stHDMIPollingInfo[2].bTimingStableFlag)
    {
        stHDMIPollingInfo[2].bTimingStableFlag = FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_GetSourceVersionType()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_U8 Hal_HDMI_GetSourceVersionType(E_MUX_INPUTPORT enInputPortType)
{
    return 0;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_Set5VDetectGPIOSelect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void Hal_HDMI_Set5VDetectGPIOSelect(MS_U32 ul5VDetectGPIOIndex)
{
    ul5VDetectGPIOIndex = 0;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_CheckHDCP14()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL Hal_HDMI_CheckHDCP14(void)
{
    MS_U8 ucBKSV_temp[5];
    MS_U8 i, j, ucOneCount = 0, ucZeroCount = 0;
	MS_BOOL HDCP14_flag = FALSE;
    
    for(i = 0; i < 5; i++)
    {
        ucBKSV_temp[i] = ucBKSV[i];
		for(j = 0; j < 8; j++)
		{
		    if(ucBKSV_temp[i] & BIT(0))
            {
                ucOneCount++;
            }
	    	else
	        {
	            ucZeroCount++;
	        }
			ucBKSV_temp[i] = ucBKSV_temp[i] >> 1;
		}
    }

	if((ucOneCount == 20) && (ucZeroCount == 20))
    {
        HDCP14_flag = TRUE;
    }
	return HDCP14_flag;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_WriteX74()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL Hal_HDCP_WriteX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ubOffset, MS_U8 ubData)
{
    if(enInputPortType > 0)
    {
        ubOffset = 0;
        ubData = 0;
    }

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_ReadX74()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 Hal_HDCP_ReadX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ubOffset)
{
    if(enInputPortType > 0)
    {
        ubOffset = 0;
    }

    return 0;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_SetReady()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_SetReady(E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsReady)
{
    if(enInputPortType > 0)
    {
        bIsReady = FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_WriteKSVList()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_WriteKSVList(E_MUX_INPUTPORT enInputPortType, MS_U8* pucData, MS_U32 ulDataLen)
{
    if(enInputPortType > 0)
    {
        pucData = NULL;
        ulDataLen = 0;
    }
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_ClearStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_ClearStatus(E_MUX_INPUTPORT enInputPortType, MS_U16 usInt)
{
    MS_U16 ulDTOPBankOffset = 0;

    switch(enInputPortType)
	{
		default:
		case 0: ulDTOPBankOffset = 0x0000; break; // BK 0x110A60
		case 1: ulDTOPBankOffset = 0x2900; break; // BK 0x113360
		case 2: ulDTOPBankOffset = 0x2B00; break; // BK 0x113560
		case 3: ulDTOPBankOffset = 0x2D00; break; // BK 0x113760
	}

    HDCP_W2BYTE(REG_HDCP_01_L +ulDTOPBankOffset, usInt << 8);
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_WriteDoneInterruptEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_WriteDoneInterruptEnable(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnableIRQ)
{
    switch(enInputPortType)
    {
        case 0:
            W2BYTEMSK(REG_HDCP_15_L, bEnableIRQ? 0: BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_ATOP_70_L, bEnableIRQ? 0: BIT(14), BIT(14));
            W2BYTEMSK(REG_HDCP22_P0_35_L, bEnableIRQ? 0: BIT(2), BIT(2));
            break;

        case 1:
            W2BYTEMSK(REG_HDCP1_15_L, bEnableIRQ? 0: BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_ATOP1_70_L, bEnableIRQ? 0: BIT(14), BIT(14));
            W2BYTEMSK(REG_HDCP22_P1_35_L, bEnableIRQ? 0: BIT(2), BIT(2));
            break;

        case 2:
            W2BYTEMSK(REG_HDCP2_15_L, bEnableIRQ? 0: BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_ATOP2_70_L, bEnableIRQ? 0: BIT(14), BIT(14));
            W2BYTEMSK(REG_HDCP22_P2_35_L, bEnableIRQ? 0: BIT(2), BIT(2));
            break;

        case 3:
            W2BYTEMSK(REG_HDCP3_15_L, bEnableIRQ? 0: BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_ATOP3_70_L, bEnableIRQ? 0: BIT(14), BIT(14));
            W2BYTEMSK(REG_HDCP22_P3_35_L, bEnableIRQ? 0: BIT(2), BIT(2));
            break;

        default:

            break;
    };
}
