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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Mfc Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MDRV_MFC_DEFINE_H_
#define _MDRV_MFC_DEFINE_H_

//------------------------------------------------------------------------------
// Macro
//------------------------------------------------------------------------------
#define K_BYTES(k) 		((U32)k<<10)
#define PAGE(p)			((U32)p>>8)
#define MAKEWORD(value1, value2)  (((U16)(value1)) * 0x100) + (value2)
#define LimitCheck(wValue, ucMode)	((wValue%ucMode)?(wValue/ucMode+1)*ucMode:wValue)
#define IsMultipCheck(wValue, ucMode)	((wValue%ucMode)?0:1)
#define CalculateDecimal(dwX,dwY)         ((((U32)dwX) + (dwY / 2)) / dwY)
#define Min(a,b)            (((a) < (b)) ? (a) : (b))
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#define diff(a, b)          (((a) > (b)) ? (a-b):(b-a))

//------------------------------------------------------------------------------
//  Definition
//------------------------------------------------------------------------------
#define _ENABLE      1
#define _DISABLE     0

#define _15MS           (HZ/66)

#define _BIT0			(0x0001)
#define _BIT1			(0x0002)
#define _BIT2			(0x0004)
#define _BIT3			(0x0008)
#define _BIT4			(0x0010)
#define _BIT5			(0x0020)
#define _BIT6			(0x0040)
#define _BIT7			(0x0080)
#define _BIT8			(0x0100)
#define _BIT9			(0x0200)
#define _BIT10			(0x0400)
#define _BIT11			(0x0800)
#define _BIT12			(0x1000)
#define _BIT13			(0x2000)
#define _BIT14			(0x4000)
#define _BIT15			(0x8000)

#define _bit0_(val)  ((U8)(val & _BIT0))
#define _bit1_(val)  ((U8)(val & _BIT1))
#define _bit2_(val)  ((U8)(val & _BIT2))
#define _bit3_(val)  ((U8)(val & _BIT3))
#define _bit4_(val)  ((U8)(val & _BIT4))
#define _bit5_(val)  ((U8)(val & _BIT5))
#define _bit6_(val)  ((U8)(val & _BIT6))
#define _bit7_(val)  ((U8)(val & _BIT7))
#define _bit8_(val)  ((U8)(val & _BIT8))
#define _bit9_(val)  ((U8)(val & _BIT9))
#define _bit10_(val) ((U8)(val & _BIT10))
#define _bit11_(val) ((U8)(val & _BIT11))
#define _bit12_(val) ((U8)(val & _BIT12))
#define _bit13_(val) ((U8)(val & _BIT13))
#define _bit14_(val) ((U8)(val & _BIT14))
#define _bit15_(val) ((U8)(val & _BIT15))

#define _END_OF_TBL_ 	-1 // end of register table

#if 0  // why add here?
#define MFC_ENABLE_LVDS_SSC 0
#define MFC_LVDS_SSC_SPAN_DEFAULT	200 //((gLgModelType==0)?300:200)
#define MFC_LVDS_SSC_STEP_DEFAULT	100


//--MFC MIU SSC setting
#define MFC_ENABLE_MIU_SSC    0
#define MFC_MIU_SSC_SPAN_DEFAULT 200 //((gLgModelType==0)?250:200)
#define MFC_MIU_SSC_STEP_DEFAULT 100
#endif

////////////////////////////////////////////////////////////////////////////////////
// Panel Type
////////////////////////////////////////////////////////////////////////////////////
// LVDS
#define PNL_WUXGA_60Hz     			0x00
#define PNL_WXGA_60HZ      			0x01

// AUO
#define PNL_AU17_EN05_SXGA_60HZ		0x10
#define PNL_AU20_WXGA_60HZ			0x11
#define PNL_AU23_WXGA_60HZ			0x12
#define PNL_AU26_WXGA_60HZ			0x13
#define PNL_AU20_M201_WSXGA_60HZ	0x14
#define PNL_T420XW01_WXGA_120HZ     0x15

// CMO
#define PNL_V420H1_WUXGA_120HZ    	0x20
#define PNL_CMO26_WUXGA_60HZ    	0x21
#define PNL_CMO22_WSXGA_60HZ        0x22
#define PNL_CMO22_WSXGA_120HZ      0x23
#define PNL_CMO23_WSXGA_120HZ      	0x24
#define PNL_CMO22_WUXGA_120HZ      0x25
#define PNL_CM21_WUXGA_60HZ			0x26
#define PNL_CMO23_WUXGA_120HZ_LR        0x27 //2Chip


// LPL
#define PNL_LC320WXN_WXGA_60HZ     	0x30
#define PNL_LC370WUN_WUXGA_60HZ     0x31
#define PNL_LC370WUE_WUXGA_60HZ     0x32
#define PNL_LC420WUN_WUXGA_60HZ     0x33
#define PNL_LC420WUE_WUXGA_60HZ     0x34
#define PNL_LC470WUN_WUXGA_60HZ     0x35
#define PNL_LC470WUE_WUXGA_60HZ     0x36

#define PNL_LC230WX3_WXGA_120HZ     0x40
#define PNL_LC320WX1_WXGA_120HZ     0x41
#define PNL_LC320WX5_WXGA_120HZ     0x42
#define PNL_LC420WX4_WXGA_120HZ     0x43
#define PNL_LC420WX7_WXGA_120HZ     0x44
#define PNL_LC420WUD_WUXGA_120HZ    0x45
#define PNL_LC470WUD_WUXGA_120HZ    0x46
#define PNL_LC370WUD_GIP_WUXGA_120HZ 0x47
#define PNL_LC320WXD_WXGA_120HZ       0x48
#define PNL_LC150OLED_WXGA_120HZ       0x49
#define PNL_LC150OLED_WXGA_60HZ       0x4a
#define PNL_LCEAll                                     0x4b
#define PNL_LC420WUF_WUXGA_120HZ	0x4c
#define PNL_LC230WUF_WUXGA_120HZ    0x4d //LG 23" FHD 120 
#define PNL_LC230WUF_WUXGA_120HZ_LR    0x4e //LG 23" FHD 120 LR //2Chip

#define PDP_LG32_XGA_60HZ     		0x60
#define PDP_LG32_WXGA_60HZ     		0x61
#define PDP_LG42_XGA_60HZ     		0x62
#define PDP_LG42_WUXGA_60HZ     	0x63

// Innolux
#define PNL_INN22_WSXGA_120HZ 		0x70
#define PNL_INN19_WXGA_120HZ        0x71
#define PNL_INN26_WXGA_120HZ        0x72

// CPT
#define PNL_CPT32_WXGA_120HZ     	0x80

//SHARP
#define PNL_SHARP37_WXGA_120HZ		0x81
#define PNL_TVBOX_720P_60HZ         0x82
#define PNL_TVBOX_1080P_60HZ        0x83

////////////////////////////////////////////////////////////////////////////////////
//MFC MIU setting
////////////////////////////////////////////////////////////////////////////////////
#define _2BK                    	0x00
#define _4BK                    	0x01
#define BK_MASK						0x01

#define _8COL                    	0x00
#define _9COL                    	0x02
#define _10COL                    	0x04
#define _11COL                    	0x06
#define COL_MASK                   	0x06

#define	_16BITS 					0x00
#define	_32BITS 					0x08
#define	MEM_BIT_MASK 				0x08

#define _1PICS						0x00
#define _2PICS						0x10
#define	PICS_MASK 					0x30

#define _SDR 						0x80
#define _DDR1 						0xC0
#define _DDR2  						0x40
#define _DDR3  						0x00

#define DDR_MASK 					0xC0

#define _1M							0x0000
#define _2M							0x0100
#define _4M							0x0200
#define _8M							0x0300
#define _16M						0x0400
#define _32M						0x0500
#define _64M						0x0600
#define _128M						0x0700

#define	SIZE_MASK 					0x0700
#define COL_BK_MASK					0x07
#define DDR_PICS_MASK				0xF0

#define DDR1_08M_16BITS_1           (_DDR1| _8M|_16BITS|_1PICS|_9COL|_4BK) // DDR 16x1 bits bus,
#define DDR1_08M_16BITS_2           (_DDR1| _8M|_16BITS|_2PICS|_9COL|_4BK) // DDR 16x2 bits bus,
#define DDR2_16M_16BITS_1      		(_DDR2|_16M|_16BITS|_1PICS|_9COL|_4BK) // DDR2 16x1 bits bus,
#define DDR2_16M_16BITS_2      		(_DDR2|_16M|_16BITS|_2PICS|_9COL|_4BK) // DDR2 16x2 bits bus,
#define DDR2_32M_16BITS_1     		(_DDR2|_32M|_16BITS|_1PICS|_10COL|_4BK) // DDR2 16x1 bits bus,
#define DDR2_32M_16BITS_2     		(_DDR2|_32M|_16BITS|_2PICS|_10COL|_4BK) // DDR2 16x2 bits bus,
#define DDR2_64M_16BITS_2     		(_DDR2|_64M|_16BITS|_2PICS|_10COL|_4BK) // DDR2 64Mbytes, x2Pic
#define DDR2_128M_16BITS_1     		(_DDR2|_128M|_16BITS|_1PICS|_10COL|_4BK) // DDR2,128Mbytes
#define DDR3_128M_16BITS_1     		(_DDR3|_128M|_16BITS|_1PICS|_10COL|_4BK) // DDR3,128Mbytes

////////////////////////////////////////////////////////////////////////////////////
//MFC IP color space mode setting
////////////////////////////////////////////////////////////////////////////////////
#define IP_YC_8BIT				0	
#define IP_YC_10BIT				1
#define IP_RGB_8BIT				2
#define IP_RGB_10BIT			3
#define IP_YC_10BIT_COMP		4
#define IP_RGB_10BIT_COMP		6


////////////////////////////////////////////////////////////////////////////////////
//MFC OD mode setting
////////////////////////////////////////////////////////////////////////////////////
// REG_2820[3:1] od_mode, REG_2820[0] od_en,
#define OD_MODE_OFF					0
#define OD_MODE_ON					1

#define	OD_MODE_444					(0x00|OD_MODE_ON)  //
#define	OD_MODE_565					(0x02|OD_MODE_ON)
#define	OD_MODE_Y_8					(0x04|OD_MODE_ON)
#define	OD_MODE_333					(0x06|OD_MODE_ON)
#define	OD_MODE_666					(0x08|OD_MODE_ON)
#define	OD_MODE_COM					(0x0A|OD_MODE_ON)
#define	OD_MODE_555					(0x0C|OD_MODE_ON)  //BW enough (default)
#define	OD_MODE_888					(0x0E|OD_MODE_ON)  //Bw unsupport
#define	OD_MODE_444_COMPRESS		(0x00|OD_MODE_COM)
#define	OD_MODE_555_COMPRESS		(0x10|OD_MODE_COM)
#define	OD_MODE_666_COMPRESS		(0x20|OD_MODE_COM)


////////////////////////////////////////////////////////////////////////////////////
//MFC Gamma setting
////////////////////////////////////////////////////////////////////////////////////
#define GAMMA_OFF					0
#define GAMMA_ON					1
#define GAMMA_MLOAD					2

#define HK_XDATA_ON_DRAM 			0


////////////////////////////////////////////////////////////////////////////////////
//MFC OP Two chip function
////////////////////////////////////////////////////////////////////////////////////
#define TwoChip_OFF		 	0
#define TwoChip_Master		1		
#define TwoChip_Slave		2
//////////////////////////////////////////////////////////////////////////////////
// MFC Chip function selection
//////////////////////////////////////////////////////////////////////////////////
#define PNL_LCEAll                  0x4b
#define PANEL_TYPE_SEL				PNL_LC420WUD_WUXGA_120HZ
#define CODESIZE_SEL				CODESIZE_ALL//CODESIZE_SMALL
#define IP_MODE 					IP_YC_10BIT
#define MOD_POWER_ON_AFTER_INIT 	0 //Old is 1
#define GAMMA_FUNCTION              GAMMA_OFF 
#define OD_MODE_SEL     			OD_MODE_OFF
#define TwoChip_Func  				TwoChip_OFF
#define ENABLE_MPIF_DEVICE     		0
#define Cable_effect 				2

//////////////////////////////////////////////////////////////////////////////////
// MFC Crystal frequence
//////////////////////////////////////////////////////////////////////////////////
#define PWM_CRYSTAL_CLOCK   		12000000ul
#define PERIOD_MASK 	0x03FFFF
#define CRYSTAL_CLOCK	12000000ul//24000000ul
#define MST_CLOCK_HZ	CRYSTAL_CLOCK
#define MST_CLOCK_KHZ	(MST_CLOCK_HZ / 1000)
#define MST_CLOCK_MHZ	(MST_CLOCK_KHZ / 1000)

#define LPLL_INPUT_DIVIDER_1ST    	0x00   // 00 : divide 1
#define LPLL_INPUT_DIVIDER_2ND    	0x00   // 00 : divide 1

#if (CRYSTAL_CLOCK==12000000ul)
	#define MPLL_CLOCK_MHZ    			216 // XTAL * MM = 12000 * 18 = 216000
#elif(CRYSTAL_CLOCK==24000000ul)
	#define MPLL_CLOCK_MHZ    			216 // XTAL * MM = 24000 * 9  = 216000
#else
	#define MPLL_CLOCK_MHZ    			215 // XTAL * MM = 14318 * 15 = 214770
#endif
#define MPLL_CLOCK_X2_KHZ    		2000ul*MPLL_CLOCK_MHZ

#define LM_FACTOR    				32 
#define LM_FACTOR_TCON60    		16

#define A_FACTOR     				(7/2)
#define PANEL_DCLK_FACTOR       	(524288ul * LM_FACTOR  * MPLL_CLOCK_MHZ / 7*2)

#define A_FACTOR_TCON  				4 // For RSDS, Bruce 20070607
#define PANEL_DCLK_FACTOR_TCON     	(524288ul * LM_FACTOR  * MPLL_CLOCK_MHZ / A_FACTOR_TCON)
#define PANEL_DCLK_FACTOR_TCON_60   (524288ul * LM_FACTOR_TCON60  * MPLL_CLOCK_MHZ / A_FACTOR_TCON)

//--MFC Output dot clock = (XTAL * MM) * (524288 * LM * K) / (A * SET)
// XTAL = external XTAL frequency
// MM = PLL divider MM, SREG D2[4:0]
// LM = PLL divider LM, SREG D3[4:0]
// K  = PLL divider K,  SREG D4[4:3]
// SET = PLL divider SET, SREG D5, D6, D7

// A = PLL divider A,
// LVDS single channel = 7
// LVDS dual channel = 3.5
// RSDS/TTL = 4

/////////////////////////////////////////
// MCU
/////////////////////////////////////////
#define WATCH_DOG_TIMER             0 // 10
#define MCU_XTAL_CLK_HZ              CRYSTAL_CLOCK
#define MCU_XTAL_CLK_KHZ             (MCU_XTAL_CLK_HZ / 1000)
#define MCU_XTAL_CLK_MHZ             (MCU_XTAL_CLK_KHZ / 1000)

//////////////////////////////////////////////////
// Chip
//////////////////////////////////////////////////
#define U01        0
#define U02        1

#endif
