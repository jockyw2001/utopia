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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// @file    madp_mfc.h
/// @brief  MFC Control Interface
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Include standard library
//------------------------------------------------------------------------------
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
//#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------
#include "madp_mfc_spi.h"
#include "mdrv_mfc.h"
#if(CODEBASE_SEL == CODEBASE_LINUX)
#include <stropts.h>
#endif
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
#include "madp.h"
#include "mdrv_mpif_st.h"
#endif
//-------------------------------------------------------------------------------------------------
// Macro
//-------------------------------------------------------------------------------------------------
#define OPT_MFC_MADP_DBG    0


#if (OPT_MFC_MADP_DBG)
#define MFC_PRINT(fmt, args...)      printf("[MAdp_MFC][%05d] " fmt, __LINE__, ## args)
#else
#define MFC_PRINT(fmt, args...)
#endif
#define K_BYTES(k) 		((U32)k<<10)
#define PAGE(p)			((U32)p>>8)

//------------------------------------------------------------------------------
// Definition
//------------------------------------------------------------------------------
#define MFC_IIC_SLAVE_ADDR 0xB4
_FLASHTYPE g_Flash;

//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------
int MFC_fd = 0;

MST_MFC_SYS_INFO_t CMO21=
{
    .u16HStart = 40,
    .u16VStart = 20,
    .u16Width  = 1920,
    .u16Height  = 1200,
    .u16HTotal = 2080,
    .u16VTotal = 1235,

    // set board info
    .u8LVDSChannel =_DUAL,
    .u8LVDSBitNum = _8BITS,
    .u8LVDSTiMode = 1,
    .u8LVDSSwapMsbLsb = 0,
    .u8LVDSSwap_P_N = 0,
    .u8LVDSSwapOddEven = 0,

    // set pannel info
    .u8PanelVfreq = 60,
    .u8PanelChannel = _DUAL,
    .u8PanelLVDSSwapCH =CHANNEL_SWAP(_CH_A, _CH_C, _CH_A, _CH_C),
    .u8PanelBitNum =_8BITS,
    // [6]TI mode, [5]=0:pair shift, [3]Polarity swap, [2]pair swap
    .u8PanelLVDSShiftPair = 0,
    .u8PanelLVDSTiMode = 1,
    .u8PanelLVDSSwapPol = 0,
    .u8PanelLVDSSwapPair = 0,

    .u16MFCMemoryClk        = 1000,//400
    .u16MFCMemoryType   = 0x0705,//0x0545,
    .u8PanelIncVtotalFor50Hz = 1,
    .u8PanelType        = 2,        //TTL, Mini_LVDS, LVDS
    .u8PanelDither      = 0,
    .u8PanelBlankCPVC   = 0,    //Panel Mini LVDS use
    .u8PanelBlankOEC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankTPC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankSTHC   = 0,    //Panel Mini LVDS use
    .u8PanelCSC         = 1,    //LVDS CSC enable/disable
    .u8PanelGAMMA       = 1,
    .u8UseMPIF          = 1,
};

MST_MFC_SYS_INFO_t g_LC420WUF=
{
    .u16HStart = 160,
    .u16VStart = 20,
    .u16Width  = 1920,
    .u16Height  = 1080,
    .u16HTotal = 2200,
    .u16VTotal = 1189,

    // set board info
    .u8LVDSChannel =_DUAL,
    .u8LVDSBitNum = _8BITS,
    .u8LVDSTiMode = 1,
    .u8LVDSSwapMsbLsb = 0,
    .u8LVDSSwap_P_N = 0,
    .u8LVDSSwapOddEven = 1,

    // set pannel info
    .u8PanelVfreq = 120,
    .u8PanelChannel = _QUAD,
    .u8PanelLVDSSwapCH =CHANNEL_SWAP(_CH_D, _CH_B, _CH_C, _CH_A),
    .u8PanelBitNum =_10BITS,
    // [6]TI mode, [5]=0:pair shift, [3]Polarity swap, [2]pair swap
    .u8PanelLVDSShiftPair = 0,
    .u8PanelLVDSTiMode = 1,
    .u8PanelLVDSSwapPol = 0,
    .u8PanelLVDSSwapPair = 0,

    .u16MFCMemoryClk        = 1000,//400
    .u16MFCMemoryType   = 0x0705,//0x0545,
    .u8PanelIncVtotalFor50Hz = 1,
    .u8PanelType        = 2,        //TTL, Mini_LVDS, LVDS
    .u8PanelDither      = 0,
    .u8PanelBlankCPVC   = 0,    //Panel Mini LVDS use
    .u8PanelBlankOEC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankTPC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankSTHC   = 0,    //Panel Mini LVDS use
    .u8PanelCSC         = 1,    //LVDS CSC enable/disable
    .u8PanelGAMMA       = 1,
    .u8UseMPIF          = 1,
};

MST_MFC_SYS_INFO_t g_LC230WUF =
{
    .u16HStart = 160,
    .u16VStart = 20,
    .u16Width  = 1920,
    .u16Height  = 1080,
    .u16HTotal = 2200,
    .u16VTotal = 1189,

    // set board info
    .u8LVDSChannel =_DUAL,
    .u8LVDSBitNum = _8BITS,
    .u8LVDSTiMode = 1,
    .u8LVDSSwapMsbLsb = 0,
    .u8LVDSSwap_P_N = 0,
    .u8LVDSSwapOddEven = 1,

    // set pannel info
    .u8PanelVfreq = 120,
    .u8PanelChannel = _QUAD,
    .u8PanelLVDSSwapCH =CHANNEL_SWAP(_CH_D, _CH_B, _CH_C, _CH_A),
    .u8PanelBitNum =_8BITS,
    // [6]TI mode, [5]=0:pair shift, [3]Polarity swap, [2]pair swap
    .u8PanelLVDSShiftPair = 0,
    .u8PanelLVDSTiMode = 1,
    .u8PanelLVDSSwapPol = 1,
    .u8PanelLVDSSwapPair = 0,

    .u16MFCMemoryClk        = 1000,//400
    .u16MFCMemoryType   = 0x0705,//0x0545,
    .u8PanelIncVtotalFor50Hz = 1,
    .u8PanelType        = 2,        //TTL, Mini_LVDS, LVDS
    .u8PanelDither      = 0,
    .u8PanelBlankCPVC   = 0,    //Panel Mini LVDS use
    .u8PanelBlankOEC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankTPC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankSTHC   = 0,    //Panel Mini LVDS use
    .u8PanelCSC         = 1,    //LVDS CSC enable/disable
    .u8PanelGAMMA       = 1,
    .u8UseMPIF          = 1,
};
/*
MST_MFC_SYS_INFO_t LC420WUD =
{
    .u16HStart = 160,
    .u16VStart = 20,
    .u16Width  = 1920,
    .u16Height  = 1080,
    .u16HTotal = 2200,
    .u16VTotal = 1125,

    // set board info
    .u8LVDSChannel =_DUAL,
    .u8LVDSBitNum = _8BITS,
    .u8LVDSTiMode = 1,
    .u8LVDSSwapMsbLsb = 0,
    .u8LVDSSwap_P_N = 0,
    .u8LVDSSwapOddEven = 1,

    // set pannel info
    .u8PanelVfreq = 120,
    .u8PanelChannel = _QUAD,
    .u8PanelLVDSSwapCH =CHANNEL_SWAP(_CH_B, _CH_D, _CH_A, _CH_C),
    .u8PanelBitNum =_10BITS,
    // [6]TI mode, [5]=0:pair shift, [3]Polarity swap, [2]pair swap
    .u8PanelLVDSShiftPair = 0,
    .u8PanelLVDSTiMode = 1,
    .u8PanelLVDSSwapPol = 0,
    .u8PanelLVDSSwapPair = 0,

    .u16MFCMemoryClk        = 1000,//400
    .u16MFCMemoryType   = 0x0705,//0x0545,
    .u8PanelIncVtotalFor50Hz = 1,
    .u8PanelType        = 2,        //TTL, Mini_LVDS, LVDS
    .u8PanelDither      = 0,
    .u8PanelBlankCPVC   = 0,    //Panel Mini LVDS use
    .u8PanelBlankOEC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankTPC    = 0,    //Panel Mini LVDS use
    .u8PanelBlankSTHC   = 0,    //Panel Mini LVDS use
    .u8PanelCSC         = 1,    //LVDS CSC enable/disable
    .u8PanelGAMMA       = 1,
    .u8UseMPIF          = 1,
};
*/
//MST_MFC_SYS_INFO_t *mfc_sysinfo = &LC420WUD;
//MST_MFC_SYS_INFO_t *mfc_sysinfo = &CMO21;
//MST_MFC_SYS_INFO_t *mfc_sysinfo = &LC230WUF;
//MST_MFC_SYS_INFO_t *mfc_sysinfo = &LC420WUF;

//------------------------------------------------------------------------------
/// Load bin code to SPI Flash
/// @param  pBin \b IN: Pointer to bin code
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
BOOL MAdp_MFC_SetSPIMode(U8 cmd)
{
	U8	 u8addr;
    U8   u8Data[5];
    BOOL ret;

    if (cmd == SpiEnter)
    {
        u8Data[0] = 0x4D;
        u8Data[1] = 0x53;
        u8Data[2] = 0x54;
        u8Data[3] = 0x41;
        u8Data[4] = 0x52;
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
        ret = MAdp_IIC_Write(MFC_MADP_IIC_CHANNEL_ID, MFC_SPI_SLAVE_ADDR, &u8addr, 0, u8Data, 5);
#else
		ret = MDrv_MFC_IIC_Write(MFC_MADP_IIC_CHANNEL_ID, MFC_SPI_SLAVE_ADDR, &u8addr, 0, u8Data, 5);
#endif
        if (!ret)
        {
            return FALSE;
        }
    }
    else
    {
        u8Data[0] = cmd;
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
        ret = MAdp_IIC_Write(MFC_MADP_IIC_CHANNEL_ID, MFC_SPI_SLAVE_ADDR, u8Data, 0, u8Data, 1);
#else
		ret = MDrv_MFC_IIC_Write(MFC_MADP_IIC_CHANNEL_ID, MFC_SPI_SLAVE_ADDR, u8Data, 0, u8Data, 1);
#endif
        if (!ret)
        {
            return FALSE;
        }
    }
    return TRUE;
}
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
void MAdp_MFC_SpiManufacturerID(void)
{
	spiRDID(&g_Flash);
	//printf("\r\ntconSpiManufacturerID() = %x", g_Flash.ManufacturerID);
}
#endif
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
void MAdp_MFC_SpiBlockErase(void)
{
	// [2]BP0 = 0,   1,   0,   1,
	// [3]BP1 = 0,   0,   1,   1,
	// Protect: 0, 1/4, 1/2, All,
	//printf("\r\ntconSpiBlockErase(Start)");
	spiWRSR(g_Flash.ManufacturerID, 0x00);
	spiBlockErase(g_Flash.ManufacturerID, 0, 0);
	spiWRSR(g_Flash.ManufacturerID, WPEN|BP1|BP0);
	spiWaitReady(RDY);
	//printf("\r\ntconSpiBlockErase(End)");
}
#endif
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
void MAdp_MFC_CheckSpiBlank(void)
{
	BOOL bRet = 1;
	U8 u8Data[256];
	U16 wPage=0, i;

	//printf("\r\nCheckSpiBlank(Start)");
	for (wPage=0; wPage<PAGE(K_BYTES(16)); wPage++)
	{
		spiPageReadByte(wPage, 0, 0xFF, u8Data);
		for (i=0; i<256; i++)
		{
			//printf("[%x]", u8Data[i]);
			if (u8Data[i] != 0xFF)
				bRet = 0;
			if (bRet==0)
				break;
		}
		if (bRet==0)
			break;
	}
	spiWaitReady(RDY);
	//printf("\r\nCheckSpiBlank(End) = ");
	//return bRet;
}
#endif
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
void MAdp_MFC_SpiProgram(U8 *pAddr, U32 len)
{
	U16 wPage, wChkSum;

	g_Flash.ProgramCKS = 0;
	//printf("\r\ntconSpiProgram(Start) = ");
	spiWRSR(g_Flash.ManufacturerID, 0x00);

	if (g_Flash.ManufacturerID == FV_SST)
	{
		printf("AAI");
		g_Flash.ProgramCKS = spiAAIWriteByte(0, 0, len , pAddr);
	}
	else
	{
		for (wPage=0; wPage<PAGE(len); wPage++)
		{
			wChkSum = spiPageWriteByte(wPage, 0, 0xFF, pAddr);
			g_Flash.ProgramCKS += wChkSum;
			pAddr += 0x100;
		}
		if (len&0xFF)
		{
			wChkSum = spiPageWriteByte(wPage, 0, (len&0xFF)-1, pAddr);
			g_Flash.ProgramCKS += wChkSum;
		}
	}
	if (g_Flash.ManufacturerID == FV_SST)
		spiWaitReady(AAI|RDY);
	else
		spiWaitReady(RDY);

	spiWRSR(g_Flash.ManufacturerID, 0x8C);
	//printf("\r\ntconSpiProgram(End) Check Sum = %x", g_Flash.ProgramCKS);
}
#endif
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
void MAdp_MFC_SpiVerify(U32 len)
{
	U8 u8Value[256];
	U16 u16Page, i;

	g_Flash.VerifyCKS = 0;
	//printf("\r\ntconSpiVerify(Start)=%d",len);
	for (u16Page=0; u16Page<PAGE(len); u16Page++)
	{
		spiPageReadByte(u16Page, 0, 0xFF, u8Value);
		for (i=0; i<256; i++)
			g_Flash.VerifyCKS += u8Value[i];
	}
	if(len&0xFF)
	{
		spiPageReadByte(u16Page, 0, (len&0xFF)-1, u8Value);
		for (i=0; i < (len&0xFF); i++)
			g_Flash.VerifyCKS += u8Value[i];
	}
	spiWaitReady(RDY);
	//printf("\r\ntconSpiVerify(End) CheckSum = %x", g_Flash.VerifyCKS);
	//return g_Flash.VerifyCKS;
}
#endif
#if(CODEBASE_SEL != CODEBASE_UTOPIA)
void MAdp_MFC_SpiLoadCode(void)
{
    U8 *pAddr;
	U32 Len;
	pAddr = MFC_BIN;
    Len = sizeof(MFC_BIN);

	MAdp_MFC_SetSPIMode(SpiEnter);
	MAdp_MFC_SpiManufacturerID();
	MAdp_MFC_SpiBlockErase();
	MAdp_MFC_CheckSpiBlank();
	MAdp_MFC_SpiProgram(pAddr, Len);
	MAdp_MFC_SpiVerify(Len);
	MAdp_MFC_SetSPIMode(SpiReset);
}
#else
void MAdp_MFC_SpiLoadCode(void)
{
	;
}
#endif

BOOL MAdp_MFC_SpiReset(void)
{
	return MAdp_MFC_SetSPIMode(SpiReset);
}

BOOL MAdp_MFC_PLATFORM_INFO_SET(PMST_MFC_SYS_INFO_t mfcSysInfo)
{
    MDrv_MFC_SetSerialDebug(TRUE);

	MDrv_MFC_WriteByte(0x1E0E, 0x08); // Disable OD clock
	MDrv_MFC_WriteByte(0x2F00, 0xE1); // Disable OP double

	// [7]=1:TI mode, [2]=1:10bits, [1]=1:Single
	MDrv_MFC_WriteByte(0x2300,
						  ((mfcSysInfo->u8LVDSTiMode?_BIT7:0)|
						   (mfcSysInfo->u8LVDSBitNum==_10BITS?_BIT2:0)|
						   (mfcSysInfo->u8LVDSChannel==_SINGLE?_BIT1:0)));

	MDrv_MFC_WriteByte(0x2328,
						  ((mfcSysInfo->u8LVDSSwapMsbLsb?_BIT7:0)|
						   (mfcSysInfo->u8LVDSSwap_P_N?_BIT6:0)|
						   (mfcSysInfo->u8LVDSSwapOddEven?_BIT4:0)));

    MDrv_MFC_WriteByte(0x2F1E, mfcSysInfo->u8PanelType);
	MDrv_MFC_WriteByte(0x233A, (0xB0|(mfcSysInfo->u8LVDSBitNum==_6BITS?_BIT3:0)));


	MDrv_MFC_WriteByte(0x2F02, (U8)((mfcSysInfo->u16VTotal-1) & 0x00FF));
    MDrv_MFC_WriteByte(0x2F03, (U8)((mfcSysInfo->u16VTotal-1) >> 8));
    MDrv_MFC_WriteByte(0x2F04, (U8)((mfcSysInfo->u16HTotal-1) & 0x00FF));
    MDrv_MFC_WriteByte(0x2F05, (U8)((mfcSysInfo->u16HTotal-1) >> 8));
	MDrv_MFC_WriteByte(0x2F14, (U8)((mfcSysInfo->u16VStart) & 0x00FF));
    MDrv_MFC_WriteByte(0x2F15, (U8)((mfcSysInfo->u16VStart) >> 8));
    MDrv_MFC_WriteByte(0x2F16, (U8)((mfcSysInfo->u16VStart+mfcSysInfo->u16Height-1) & 0x00FF));
    MDrv_MFC_WriteByte(0x2F17, (U8)((mfcSysInfo->u16VStart+mfcSysInfo->u16Height-1) >> 8));
    MDrv_MFC_WriteByte(0x2F18, (U8)((mfcSysInfo->u16HStart) & 0x00FF));
    MDrv_MFC_WriteByte(0x2F19, (U8)((mfcSysInfo->u16HStart) >> 8));
    MDrv_MFC_WriteByte(0x2F1A, (U8)((mfcSysInfo->u16HStart+mfcSysInfo->u16Width-1) & 0x00FF));
    MDrv_MFC_WriteByte(0x2F1B, (U8)((mfcSysInfo->u16HStart+mfcSysInfo->u16Width-1) >> 8));
    MDrv_MFC_WriteByte(0x2F1C, mfcSysInfo->u8PanelVfreq);

	MDrv_MFC_WriteByte(0x2F20, mfcSysInfo->u8PanelChannel);
	MDrv_MFC_WriteByte(0x2F22, mfcSysInfo->u8PanelDither);

    MDrv_MFC_WriteByte(0x321F, mfcSysInfo->u8PanelLVDSSwapCH);
    if (mfcSysInfo->u8PanelBitNum==_8BITS)
	    MDrv_MFC_WriteByte(0x320A, 0x08); // lvds TI 8bit(h5[3:2]=10)
    else if (mfcSysInfo->u8PanelBitNum==_6BITS)
		MDrv_MFC_WriteByte(0x320A, 0x0C); // lvds TI 6bit(h5[3:2]=11)
  	else
    	MDrv_MFC_WriteByte(0x320A, 0x00);
  	// [6]TI mode, [5]=0:pair shift, [3]Polarity swap, [2]pair swap
	MDrv_MFC_WriteByte(0x320B, ((mfcSysInfo->u8PanelLVDSTiMode?_BIT6:0)|
								(mfcSysInfo->u8PanelLVDSShiftPair?0:_BIT5)|
								(mfcSysInfo->u8PanelLVDSSwapPol?_BIT3:0)|
								(mfcSysInfo->u8PanelLVDSSwapPair?_BIT2:0)));


	MDrv_MFC_WriteByte(0x2F2E, 0x07);
	MDrv_MFC_WriteByte(0x2F00, 0xE9);// Enable OP double

    MDrv_MFC_SetSerialDebug(FALSE);
	return TRUE;
}

BOOL MAdp_MFC_WaitReady(void)
{
	U8 u8ReadValue, u8count = 0;
	while(u8count++ < 5)
	{
		mfcSleepMs(100);
		u8ReadValue=0x00;


                MDrv_MFC_SetSerialDebug(TRUE);
		u8ReadValue = MDrv_MFC_ReadByte(0x2F2E);
		printf("\n\n\n\n\n\n\n[0x2F2E]=[%x]\n\n\n\n\n", u8ReadValue);
		MDrv_MFC_SetSerialDebug(FALSE);


		if(u8ReadValue == 0x03)
			break;
	}
	return (u8count > 10) ? 0 : 1;
}

//------------------------------------------------------------------------------
/// Init MFC
/// @return None
//------------------------------------------------------------------------------
void MAdp_MFC_Init(PMST_MFC_SYS_INFO_t mfcSysInfo, U8 u8IsS7M)
{
    //Init MFC
    MDrv_MFC_SetSerialDebug(TRUE);
    S7M = u8IsS7M;
    if(S7M) //for S7M
    {
    	MDrv_MFC_Init(mfcSysInfo);
    }
	else//for BCM
	{
		//MAdp_MFC_SpiLoadCode();
		//MAdp_MFC_WaitReady();

	    // addr 0x2F2E
	    // 0x01: URSA will init with default data
	    // 0x07: URSA will set the panel info from Saturn6.
    	#if 0
    		MDrv_MFC_WriteByte(0x2F2E, 0x01);
    	#else
    		MAdp_MFC_PLATFORM_INFO_SET(mfcSysInfo);
    		MDrv_MFC_WriteByte(0x2F2E, 0x07);
    	#endif
    	//for removing OSD garbage
	    //MDrv_MFC_WriteByteMask(0x2052, 0, 0x01);
	}
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_OnOff(BOOL bOnOff)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_OnOff( bOnOff);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_Compensation(U8 u8Blur, U8 u8Judder, U8 u8MFC)
{
    MDrv_MFC_SetSerialDebug(TRUE);
    MDrv_MFC_COMPENSATION(u8Blur, u8Judder, u8MFC);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_True_Motion_Demo(U8 u8TrueMotionDemo)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_TrueMotionDemo(u8TrueMotionDemo);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_Video_Block(U8 u8Ctrl, BOOL bCtrl)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_VideoBlock(u8Ctrl , bCtrl);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_LVDSOnOff(BOOL bOnOff)
{//0:On	1:Off
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_LVDSPowerOnOFF(bOnOff);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_BypassWindow(BOOL bOnOff, U8 u8WinId, U16 u16Hstart, U16 u16Vstart, U16 u16Hsize, U16 u16Vsize)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_Set_Bypass_Window( bOnOff,  u8WinId,  u16Hstart,  u16Vstart,  u16Hsize,  u16Vsize);
    MDrv_MFC_SetSerialDebug(FALSE);
}

U16 MAdp_MFC_GetFirmwareVersion(void)
{
    U16 ret;

     MDrv_MFC_SetSerialDebug(TRUE);
    ret = MDrv_MFC_GetSWVersion();
    MDrv_MFC_SetSerialDebug(FALSE);

    return ret;
}

U16 MAdp_MFC_GetFirmwareVersionFromBin(void)
{
    U16 ret;

    MDrv_MFC_SetSerialDebug(TRUE);
	ret = MDrv_MFC_GetBinVersion();
    MDrv_MFC_SetSerialDebug(FALSE);

    return ret;
}

void MAdp_MFC_SetSpreadSpectrum(U16 u16KHz, U16 u16Percent, BOOL bEnable, BOOL bMiuLVDS)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	//1:MIU SSC		0:LVDS SSC
	MDrv_MFC_SetSSC( u16KHz, u16Percent, bEnable, bMiuLVDS);
    MDrv_MFC_SetSerialDebug(FALSE);

}

void MAdp_MFC_OnOffDebugBlock(BOOL bOnOff)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_DebugBlock(bOnOff);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_DemoBarControl(BOOL bOnOFF, BOOL bDirection, U8 u8Width, U8 u8Color)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_DemoBarControl(bOnOFF, bDirection, u8Width, u8Color);
    MDrv_MFC_SetSerialDebug(FALSE);
}

//For BCM
U8 MAdp_MFC_GetModelType(void)
{
	U8 ret = 0;

    MDrv_MFC_SetSerialDebug(TRUE);
	ret = MDrv_MFC_ReadByte(0x1E48);
	//0: Venus, 1: Mini LVDS 42'' M+S normal
	//2: Mini LVDS 47'' M+S normal, 3:Mini LVDS 37'' M+S GIP
    MDrv_MFC_SetSerialDebug(FALSE);

	return ret;
}

void MAdp_MFC_ReadHVTotal(U16* pHTotal, U16* pVTotal )
{

    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_GetHVTotal( pHTotal,  pVTotal);
    MDrv_MFC_SetSerialDebug(FALSE);

}

U8 MAdp_MFC_IsStable(void)
{
    U8 ret = TRUE;

    MDrv_MFC_SetSerialDebug(TRUE);
	ret = MDrv_MFC_IsStable();
    MDrv_MFC_SetSerialDebug(FALSE);

    return ret;
}

void MAdp_MFC_SetLVDSVesaJeida(BOOL bVesaJeida)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_SetLVDSVesaJeida(bVesaJeida);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_SetFrameRate(U8 u8frameRate)
{

    MDrv_MFC_SetSerialDebug(TRUE);
	if(S7M)
		MDrv_MFC_SetFrameRate(u8frameRate);
	else
	{
		if(u8frameRate == 60)	//60Hz
		{
			MDrv_MFC_WriteByte(0x1E4D, 0x3c);
			MDrv_MFC_WriteByteMask(0x1E4C, 1, 0x01);
		}
		else//120Hz
		{
			MDrv_MFC_WriteByte(0x1E4D, 0x78);
			MDrv_MFC_WriteByteMask(0x1E4C, 1, 0x01);
			//OSA_SuspendTask(20);
			mfcSleepMs(100);
			MDrv_MFC_WriteByteMask(0x1E4C, 0, 0x01);
		}
	}
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_SetSlowFrameLock(BOOL bEnable)
{

    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_SlowFrameLock(bEnable);
    MDrv_MFC_SetSerialDebug(FALSE);

}

void MAdp_MFC_ControlFrameLockMode(U8 u8type)
{

    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_FrameLockMode(u8type);
    MDrv_MFC_SetSerialDebug(FALSE);

}

void MAdp_MFC_RESET(void)
{

    MDrv_MFC_SetSerialDebug(TRUE);
	if(S7M) MDrv_MFC_Reset();
    MDrv_MFC_SetSerialDebug(FALSE);

}

void MAdp_MFC_UpdateSW(void)
{

    MDrv_MFC_SetSerialDebug(TRUE);
	if(S7M)
		MDrv_MFC_UpdateSW();
	else
		MAdp_MFC_SpiLoadCode();
	//Remember to call	"void MAdp_MFC_Init(PMST_MFC_SYS_INFO_t mfcSysInfo, U8 u8IsS7M)"
    MDrv_MFC_SetSerialDebug(FALSE);

}

void MAdp_MFC_SetReverseControl(U8 u8Mode)
{//0:MIRROR_OFF		1:MIRROR_H_MODE		2:MIRROR_V_MODE		3:MIRROR_HV_MODE
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_SetReverseMode(u8Mode);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_SetLVDSBitNum(U8 u8LVDSBitNum)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_SetLVDSBit(u8LVDSBitNum);
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_SetODCTable(void)
{
    MDrv_MFC_SetSerialDebug(TRUE);
	MDrv_MFC_SetODCTable();
    MDrv_MFC_SetSerialDebug(FALSE);
}

void MAdp_MFC_SetVFreq(U16 u16Inputfreq, BOOL enableFPLL)
{
    MDrv_MFC_SetSerialDebug(TRUE);
    MDrv_MFC_SetVFreq(u16Inputfreq, enableFPLL);
    MDrv_MFC_SetSerialDebug(FALSE);
}

