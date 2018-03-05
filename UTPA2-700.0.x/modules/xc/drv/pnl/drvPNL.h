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
/// @file   drvPNL.h
/// @brief  Panel Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PNL_H_
#define _DRV_PNL_H_

#ifdef _DRVPNL_C_
#define DRV_PNL_INTERFACE
#else
#define DRV_PNL_INTERFACE extern
#endif

#include "ULog.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define PNL_LIB_VERSION         "000.000.000.000"

//clock hz define, the unit is 0.1hz
#define DCLK_1MHZ_X10            0010000000
#define DCLK_25MHZ_X10           0250000000
#define DCLK_50MHZ_X10           0500000000
#define DCLK_75MHZ_X10           0750000000
#define DCLK_100MHZ_X10          1000000000
#define DCLK_150MHZ_X10          1500000000

#define KERNEL_DRIVER_PATCH 1
#ifdef CONFIG_MS_DEBUG_XC_LOG
#ifndef PNL_LOG_DBG
#define PNL_LOG_DBG
#endif
#endif
// OS related
//#define PNL_LOG_DBG     1
#define KERNEL_DRIVER_PNL_STR_PATCH 0

// Debug related
#ifdef PNL_LOG_DBG
#if (defined CONFIG_MLOG)
#define PNL_DBG(_dbgSwitch_, _fmt, _args...)      \
    { \
        if((_dbgSwitch_ & _u16PnlDbgSwitch) != 0) \
        {\
            ULOGD("PNL_DBG","[%s,%5d] ",__FUNCTION__,__LINE__);\
            ULOGD("PNL_DBG","PNL:"_fmt, ##_args); \
        }\
    }
#else
#define PNL_DBG(_dbgSwitch_, _fmt, _args...)      \
    { \
        if((_dbgSwitch_ & _u16PnlDbgSwitch) != 0) \
        {\
            printf("PNL_DBG:"_fmt, ##_args); \
        }\
    }
#endif
#else
#define PNL_DBG(_dbgSwitch_, _fmt, _args...)      { }
#endif

#define PNL_ASSERT(_cnd, _fmt, _args...)                                                                            \
                                    if (!(_cnd)) {                                                                  \
                                        MS_DEBUG_MSG(printf("PNL ASSERT: %s %d:"_fmt, __FILE__, __LINE__, ##_args));              \
                                        /* while(1); */                                                             \
                                    }

// for Utopia 2.0 to Utopia 1.0 compatibility.
DRV_PNL_INTERFACE void* pu32PNLInst_private;
DRV_PNL_INTERFACE void* g_pPNLResource[E_PNL_POOL_ID_NUM];

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_PNL_FAIL = 0,
    E_PNL_OK = 1,
    E_PNL_GET_BASEADDR_FAIL,            ///< get base address failed when initialize panel driver
    E_PNL_OBTAIN_MUTEX_FAIL,            ///< obtain mutex timeout when calling this function
} PNL_Result;

typedef enum
{
    E_DRVPNL_GAMMA_10BIT         = 1,
    E_DRVPNL_GAMMA_12BIT         = 2,
} DRVPNL_GAMMA_TYPE;

typedef enum
{
    E_DRVPNL_GAMMA_8BIT_MAPPING  = 1,      ///< mapping 1024 to 256 gamma entries
    E_DRVPNL_GAMMA_10BIT_MAPPING = 2,         ///< mapping 1024 to 1024 gamma entries
} DRVPNL_GAMMA_MAPPEING_MODE;

/// Define Panel MISC control index
/// please enum use BIT0 = 0x01, BIT1 = 0x02, BIT2 = 0x04, BIT3 = 0x08, BIT4 = 0x10,
typedef enum
{
    E_DRVPNL_MISC_MFC_ENABLE = 0x0001,

    E_DRVPNL_MISC_MFC_MCP    = 0x0010,
    E_DRVPNL_MISC_MFC_ABChannel = 0x0020,
    E_DRVPNL_MISC_MFC_ACChannel = 0x0040,
    E_DRVPNL_MISC_MFC_ENABLE_60HZ = 0x0080,
    E_DRVPNL_MISC_MFC_ENABLE_240HZ = 0x0100,
    E_DRVPNL_MISC_4K2K_ENABLE_60HZ = 0x0200,
    E_DRVPNL_MISC_SKIP_T3D_CONTROL = 0x0400,
} DRVPNL_MISC;

typedef struct
{
    MS_U8 u8SupportGammaType;    ///< refer to APIPNL_GAMMA_TYPE
    MS_U8 u8SupportGammaMapLv;   ///< refero to APIPNL_GAMMA_MAPPEING_MODE
} ST_PNL_DRV_INFO;

typedef struct
{
    MS_BOOL bPanel_Initialized;     ///< panel initialized or not
    MS_BOOL bPanel_Enabled;         ///< panel enabled or not, if enabled, you can see OSD/Video
} PNL_DrvStatus;

typedef enum
{
    E_PNL_TYPE_TTL,                      // 0
    E_PNL_TYPE_LVDS,                     // 1
    E_PNL_TYPE_RSDS,                     // 2
    E_PNL_TYPE_MINILVDS,                 // 3 <--  E_XC_PNL_LPLL_MINILVDS_6P_2L,
    E_PNL_TYPE_ANALOG_MINILVDS,          // 4
    E_PNL_TYPE_DIGITAL_MINILVDS,         // 5
    E_PNL_TYPE_MFC,                      // 6
    E_PNL_TYPE_DAC_I,                    // 7
    E_PNL_TYPE_DAC_P,                    // 8
    E_PNL_TYPE_PDPLVDS,                  // 9 ///< For PDP(Vsync use Manually MODE)

    E_PNL_TYPE_EXT,                      // 10
    E_PNL_LPLL_EPI34_8P = E_PNL_TYPE_EXT,// 10
    E_PNL_LPLL_EPI28_8P,                 // 11
    E_PNL_LPLL_EPI34_6P,                 // 12
    E_PNL_LPLL_EPI28_6P,                 // 13

//  E_PNL_LPLL_MINILVDS_6P_2L,                   ///< replace this with E_PNL_TYPE_MINILVDS
    E_PNL_LPLL_MINILVDS_5P_2L,           // 14
    E_PNL_LPLL_MINILVDS_4P_2L,           // 15
    E_PNL_LPLL_MINILVDS_3P_2L,           // 16
    E_PNL_LPLL_MINILVDS_6P_1L,           // 17
    E_PNL_LPLL_MINILVDS_5P_1L,           // 18
    E_PNL_LPLL_MINILVDS_4P_1L,           // 19
    E_PNL_LPLL_MINILVDS_3P_1L,           // 20

    E_PNL_TYPE_HS_LVDS,                  // 21
    E_PNL_TYPE_HF_LVDS,                  // 22

    E_PNL_TYPE_TTL_TCON,                 // 23
    E_PNL_LPLL_MINILVDS_2CH_3P_8BIT,     // 24 // 2 channel, 3 pari, 8 bits
    E_PNL_LPLL_MINILVDS_2CH_4P_8BIT,     // 25 // 2 channel, 4 pari, 8 bits
    E_PNL_LPLL_MINILVDS_2CH_5P_8BIT,    // 26 //2 channel, 5 pari, 8 bits
    E_PNL_LPLL_MINILVDS_2CH_6P_8BIT,    // 27 //2 channel, 6 pari, 8 bits

    E_PNL_LPLL_MINILVDS_1CH_3P_8BIT,    // 28 //1 channel, 3 pair, 8 bits
    E_PNL_LPLL_MINILVDS_1CH_4P_8BIT,    // 29 //1 channel, 4 pair, 8 bits
    E_PNL_LPLL_MINILVDS_1CH_5P_8BIT,    // 30 //1 channel, 5 pair, 8 bits
    E_PNL_LPLL_MINILVDS_1CH_6P_8BIT,    // 31 //1 channel, 6 pair, 8 bits

    E_PNL_LPLL_MINILVDS_2CH_3P_6BIT,    // 32 //2 channel, 3 pari, 6 bits
    E_PNL_LPLL_MINILVDS_2CH_4P_6BIT,    // 33 //2 channel, 4 pari, 6 bits
    E_PNL_LPLL_MINILVDS_2CH_5P_6BIT,    // 34 //2 channel, 5 pari, 6 bits
    E_PNL_LPLL_MINILVDS_2CH_6P_6BIT,    // 35 //2 channel, 6 pari, 6 bits

    E_PNL_LPLL_MINILVDS_1CH_3P_6BIT,    // 36 //1 channel, 3 pair, 6 bits
    E_PNL_LPLL_MINILVDS_1CH_4P_6BIT,    // 37 //1 channel, 4 pair, 6 bits
    E_PNL_LPLL_MINILVDS_1CH_5P_6BIT,    // 38 //1 channel, 5 pair, 6 bits
    E_PNL_LPLL_MINILVDS_1CH_6P_6BIT,    // 39 //1 channel, 6 pair, 6 bits

    E_PNL_LPLL_HDMI_BYPASS_MODE,        //40  // HDMI Bypass Mode

    E_PNL_LPLL_EPI34_2P,                         /// 41
    E_PNL_LPLL_EPI34_4P,                         /// 42
    E_PNL_LPLL_EPI28_2P,                         /// 43
    E_PNL_LPLL_EPI28_4P,                         /// 44

    E_PNL_LPLL_VBY1_10BIT_4LANE,    ///45
    E_PNL_LPLL_VBY1_10BIT_2LANE,    ///46
    E_PNL_LPLL_VBY1_10BIT_1LANE,    ///47
    E_PNL_LPLL_VBY1_8BIT_4LANE,     ///48
    E_PNL_LPLL_VBY1_8BIT_2LANE,     ///49
    E_PNL_LPLL_VBY1_8BIT_1LANE,     ///50

    E_PNL_LPLL_VBY1_10BIT_8LANE,     ///51
    E_PNL_LPLL_VBY1_8BIT_8LANE,     ///52

    E_PNL_LPLL_EPI28_12P,           /// 53

    E_PNL_LPLL_HS_LVDS_BYPASS_MODE, ///54
    E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE, ///55
    E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE, ///56
    E_PNL_LPLL_EPI24_12P,           ///57
    E_PNL_LPLL_VBY1_10BIT_16LANE,   ///58
    E_PNL_LPLL_VBY1_8BIT_16LANE,    ///59
    E_PNL_LPLL_USI_T_8BIT_12P,      ///60
    E_PNL_LPLL_USI_T_10BIT_12P,     ///61
    E_PNL_LPLL_ISP_8BIT_12P,        ///62
    E_PNL_LPLL_ISP_8BIT_6P_D,       ///63
} PNL_TYPE;

typedef enum
{
    E_PNL_LPLL_VIDEO = 0,
    E_PNL_LPLL_OSD,

}PNL_LPLL_TYPE_SEL;

typedef enum
{
    E_PNL_MODE_SINGLE = 0,          ///< single channel
    E_PNL_MODE_DUAL = 1,            ///< dual channel

    E_PNL_MODE_QUAD = 2,            ///< quad channel
    E_PNL_MODE_QUAD_LR = 3,            ///< quad channel

} PNL_MODE;

typedef enum
{
    E_PNL_OUTPUT_NO_OUTPUT = 0,     ///< even called g_IPanel.Enable(TRUE), still no physical output
    E_PNL_OUTPUT_CLK_ONLY,          ///< after called g_IPanel.Enable(TRUE), will output clock only
    E_PNL_OUTPUT_DATA_ONLY,         ///< after called g_IPanel.Enable(TRUE), will output data only
    E_PNL_OUTPUT_CLK_DATA,          ///< after called g_IPanel.Enable(TRUE), will output clock and data
    E_PNL_OUTPUT_MAX,          ///< after called g_IPanel.Enable(TRUE), will output clock and data
} PNL_OUTPUT_MODE;

typedef struct
{
    // Output timing info
    MS_U16  u16VTotal;            ///< Output vertical total
    MS_U16  u16DEVStart;          ///< Output DE vertical start
    MS_U16  u16DEVEnd;            ///< Output DE Vertical end
    MS_U16  u16VSyncStart;        ///< Output VSync start
    MS_U16  u16VSyncEnd;          ///< Output VSync end
    MS_U16  u16HTotal;            ///< Output horizontal total
    MS_U16  u16DEHStart;          ///< Output DE horizontal start
    MS_U16  u16DEHEnd;            ///< Output DE horizontal end
    MS_U16  u16HSyncWidth;        ///< Output HSync width
    MS_BOOL bIsPanelManualVysncMode; ///< enable manuel V sync control
    MS_BOOL bInterlaceOutput;     ///< enable Scaler Interlace output
} PNL_TimingInfo;

typedef struct __attribute__((packed))
{
    const char *pPanelName;                ///<  PanelName
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy0;
#endif
    // Output timing
    MS_U16 u16HStart;               ///< DE H start
    MS_U16 u16VStart;               ///< DE V start
    MS_U16 u16Width;                ///< DE H width
    MS_U16 u16Height;               ///< DE V height
    MS_U16 u16HTotal;               ///< H total
    MS_U16 u16VTotal;               ///< V total

    MS_U16 u16DefaultVFreq;         ///< Panel output Vfreq., used in free run

    // Later need to refine to use Min/Max SET for PDP panel, but for LCD, it maybe no need to check the Min/Max SET
    //MS_U16 u16DefaultHTotal, u16DefaultVTotal;
    //MS_U16 u16MinHTotal, u16DefaultHTotal, u16MaxHTotal;
    //MS_U16 u16MinVTotal, u16DefaultVTotal, u16MaxVTotal;
    //MS_U32 u32MinDCLK, u32MaxDCLK;
    MS_U32 u32MinSET, u32MaxSET;

    // output type
    PNL_TYPE eLPLL_Type;            ///< 0: LVDS type, 1: RSDS type
    PNL_TYPE eLPLL_Type_Ext;        ///< 0: Ext LVDS type, 1: RSDS type
    PNL_MODE eLPLL_Mode;            ///< 0: single mode, 1: dual mode

    // sync
    MS_U8  u8HSyncWidth;            ///< H sync width
    MS_U16 u16VSyncStart;           ///< V sync start = Vtotal - backporch - VSyncWidth
    MS_U8  u8VSyncWidth;            ///< V sync width
    MS_BOOL bManuelVSyncCtrl;       ///< enable manuel V sync control

    // output control
    MS_U16 u16OCTRL;                ///< Output control such as Swap port, etc.
    MS_U16 u16OSTRL;                ///< Output control sync as Invert sync/DE, etc.
    MS_U16 u16ODRV;                 ///< Driving current
    MS_U16 u16DITHCTRL;             ///< Dither control

    // MOD
    MS_U16 u16MOD_CTRL0;            ///< MOD_REG(0x40), PANEL_DCLK_DELAY:8, PANEL_SWAP_LVDS_CH:6, PANEL_SWAP_LVDS_POL:5, PANEL_LVDS_TI_MODE:2,
    MS_U16 u16MOD_CTRL9;            ///< MOD_REG(0x49), PANEL_SWAP_EVEN_ML:14, PANEL_SWAP_EVEN_RB:13, PANEL_SWAP_ODD_ML:12, PANEL_SWAP_ODD_RB:11, [7,6] : output formate selction 10: 8bit, 01: 6bit :other 10bit
    MS_U16 u16MOD_CTRLA;            ///< MOD_REG(0x4A), PANEL_INV_HSYNC:12, PANEL_INV_DCLK:4, PANEL_INV_VSYNC:3, PANEL_INV_DE:2, PANEL_DUAL_PORT:1, PANEL_SWAP_PORT:0,
    MS_U8  u8MOD_CTRLB;             ///< MOD_REG(0x4B), [1:0]ti_bitmode=00(10bit)

    // Other
    MS_U16 u16LVDSTxSwapValue;      ///< swap PN setting
    MS_U8  u8PanelNoiseDith;        ///<  PAFRC mixed with noise dither disable
    MS_U32 u32PNL_MISC;
    MS_U16 u16OutputCFG0_7;
    MS_U16 u16OutputCFG8_15;
    MS_U16 u16OutputCFG16_21;

    // panel on/off timing
    MS_U16 u16PanelOnTiming1;                 ///<  time between panel & data while turn on power
    MS_U16 u16PanelOnTiming2;                 ///<  time between data & back light while turn on power
    MS_U16 u16PanelOffTiming1;                 ///<  time between back light & data while turn off power
    MS_U16 u16PanelOffTiming2;                 ///<  time between data & panel while turn off power

    MS_U8 u16PanelDimmingCtl;                  ///<  Initial Dimming Value
    MS_U8 u16PanelMaxPWMVal;                 ///<  Max Dimming Value
    MS_U8 u16PanelMinPWMVal;                  ///<  Min Dimming Value

                                                                                        ///<  not support Manuel VSync Start/End now
                                                                                        ///<  VOP_02[10:0] VSync start = Vtt - VBackPorch - VSyncWidth
                                                                                        ///<  VOP_03[10:0] VSync end = Vtt - VBackPorch
    MS_U8 u16PanelHSyncBackPorch;    ///<  PANEL_HSYNC_BACK_PORCH, no register setting, provide value for query only
    MS_U8 u16PanelVBackPorch;              ///<  define PANEL_VSYNC_BACK_PORCH
    MS_U16 u16PanelAspectRatio;           ///<  Panel Aspect Ratio, provide information to upper layer application for aspect ratio setting.

    MS_U16 u16PanelOutTimingMode;        ///<Define which panel output timing change mode is used to change VFreq for same panel

    MS_U16 u16Ext_LpllType;              ///< Ext LPLL type is used by OSDC

    MS_U8  u8OutputOrderType;
    MS_U16 u16OutputOrder0_3;
    MS_U16 u16OutputOrder4_7;
    MS_U16 u16OutputOrder8_11;
    MS_U16 u16OutputOrder12_13;

    MS_BOOL bVideo_HW_Training_En;       ///< Enable Video HW training mode
    MS_BOOL bOSD_HW_Training_En;         ///< Enable OSD HW training mode
} PNL_InitData;



//HW LVDS Reserved Bit to L/R flag Info
typedef struct
{
    MS_U32 u32pair; // pair 0: BIT0, pair 1: BIT1, pair 2: BIT2, pair 3: BIT3, pair 4: BIT4, etc ...
    MS_U16 u16channel; // channel A: BIT0, channel B: BIT1,
    MS_BOOL bEnable;
} PNL_DrvHW_LVDSResInfo;

typedef struct
{
    MS_U16 u16ExpectSwingLevel;
    MS_U8 u8ModCaliPairSel;
    MS_U8 u8ModCaliTarget;
    MS_S8 s8ModCaliOffset;
    MS_BOOL bPVDD_2V5;
}PNL_ModCali_InitData;

typedef enum
{
/*
__________________________________________________________________________________________________________________________________________
|Vby1 Output Format|D0|D1|D2|D3|D4|D5|D6|D7|D8|D9|D10|D11|D12|D13|D14|D15|D16|D17|D18|D19|D20|D21|D22|D23|D24|D25|D26|D27|D28|D29|D30|D31|
|------------------|--|--|--|--|--|--|--|--|--|--|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| ARGB 1           |R0|R1|R2|R3|R4|R5|R6|R7|G0|G1|G2 |G3 |G4 |G5 |G6 |G7 |B0 |B1 |B2 |B3 |B4 |B5 |B6 |B7 |A1 |A0 |A2 |A3 |A4 |A5 |A6 |A7 |
|------------------|--|--|--|--|--|--|--|--|--|--|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| ARGB 2           |R2|R3|R4|R5|R6|R7|A6|A7|G2|G3|G4 |G5 |G6 |G7 |A4 |A5 |B2 |B3 |B4 |B5 |B6 |B7 |A2 |A3 |A0 |A1 |B0 |B1 |G0 |G1 |R0 |R1 |
------------------------------------------------------------------------------------------------------------------------------------------
*/
    E_PNL_OSDC_OUTPUT_FORMAT_VBY1_ARGB1,
    E_PNL_OSDC_OUTPUT_FORMAT_VBY1_ARGB2,
} E_PNL_OSDC_OUTPUT_FORMAT;


#define IsVBY1(x)   ( (((x)>= E_PNL_LPLL_VBY1_10BIT_4LANE)&&((x)<= E_PNL_LPLL_VBY1_8BIT_8LANE)) || ((x)== E_PNL_LPLL_VBY1_10BIT_16LANE)||((x)== E_PNL_LPLL_VBY1_8BIT_16LANE) )
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
DRV_PNL_INTERFACE MS_U16 _u16PnlDbgSwitch;

// must have functions
DRV_PNL_INTERFACE const MS_U8 * MDrv_PNL_GetLibVer(void);                  ///< Get version (without Mutex protect)
DRV_PNL_INTERFACE ST_PNL_DRV_INFO MDrv_PNL_GetInfo(void *pInstance);              ///< Get supported gamma type from driver (without Mutex protect)

// initial functions
PNL_Result MDrv_PNL_PreInit(void *pInstance, PNL_OUTPUT_MODE eParam);
PNL_Result MDrv_PNL_Init(void *pInstance);       ///< initialize panel driver
PNL_Result MDrv_PNL_Close(void *pInstance);                                                            ///< close panel driver and release resources

//DRV_PNL_INTERFACE void MDrv_PNL_TCONMAP_DumpTable(MS_U8 *pTCONTable, MS_U8 u8Tcontype);
//DRV_PNL_INTERFACE void MDrv_PNL_TCONMAP_Power_Sequence(MS_U8 *pTCONTable, MS_BOOL bEnable);
void MDrv_PNL_TCON_DumpSCRegTab(void *pInstance, MS_U8* pu8TconTab);
void MDrv_PNL_TCON_DumpMODRegTab(void *pInstance, MS_U8* pu8TconTab);
void MDrv_PNL_TCON_DumpGENRegTab(void *pInstance, MS_U8* pu8TconTab);


DRV_PNL_INTERFACE void MDrv_PNL_TCON_Count_Reset(void *pInstance, MS_BOOL bEnable);
DRV_PNL_INTERFACE void MDrv_PNL_TCON_Init(void *pInstance);
DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_GetDataFromRegister(void *pInstance, PNL_TimingInfo *PNLTiming);

// control & setting functions
DRV_PNL_INTERFACE PNL_Result MDrv_PNL_En(void *pInstance, MS_BOOL bPanelOn, MS_BOOL bCalEn);                            ///< bPanelOn = TRUE to show up OSD & video

#define MDrv_PNL_MOD_Calibration  MHal_PNL_MOD_Calibration
DRV_PNL_INTERFACE PNL_Result MDrv_PNL_MOD_Calibration(void *pInstance);

DRV_PNL_INTERFACE PNL_Result MDrv_PNL_SetGammaTbl(void *pInstance, DRVPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3], DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode); ///< set gamma table
DRV_PNL_INTERFACE PNL_Result MDrv_PNL_GetGammaTbl(void *pInstance, DRVPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3],DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode); ///< get gamma table
DRV_PNL_INTERFACE PNL_Result MDrv_PNL_SetGammaValue(void *pInstance, MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue);
DRV_PNL_INTERFACE PNL_Result MDrv_PNL_SetSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable);
DRV_PNL_INTERFACE void MDrv_PNL_MFC(void *pInstance, MS_BOOL bIsMFC);
DRV_PNL_INTERFACE void MDrv_PNL_SetOutputPattern(void *pInstance, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue);
DRV_PNL_INTERFACE void MDrv_PNL_SetOutputType(void *pInstance, PNL_OUTPUT_MODE eOutputMode, PNL_TYPE eLPLL_Type);
DRV_PNL_INTERFACE PNL_Result MDrv_PNL_SetPanelType(void *pInstance, PNL_InitData *pstPanelInitData);

DRV_PNL_INTERFACE void MDrv_PNL_SetSSC_Rdeviation(void *pInstance, MS_U16 u16Rdeviation);
DRV_PNL_INTERFACE void MDrv_PNL_SetSSC_Fmodulation(void *pInstance, MS_U16 u16Fmodulation);
DRV_PNL_INTERFACE void MDrv_PNL_SetSSC_En(void *pInstance, MS_BOOL bEnable);

DRV_PNL_INTERFACE void MDrv_PNL_SetOSDSSC_Rdeviation(void *pInstance, MS_U16 u16Rdeviation);
DRV_PNL_INTERFACE void MDrv_PNL_SetOSDSSC_Fmodulation(void *pInstance, MS_U16 u16Fmodulation);
DRV_PNL_INTERFACE void MDrv_PNL_SetOSDSSC_En(void *pInstance, MS_BOOL bEnable);

DRV_PNL_INTERFACE void MDrv_Mod_Calibration_Init(void *pInstance, PNL_ModCali_InitData *pstModCaliInitData);
DRV_PNL_INTERFACE void MDrv_BD_LVDS_Output_Type(void *pInstance, MS_U16 Type);
DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_Control_Out_Swing(void *pInstance, MS_U16 u16Swing_Level);
DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_SkipTimingChange_GetCaps(void *pInstance);
DRV_PNL_INTERFACE void  MDrv_PNL_PreSetModeOn(void *pInstance, MS_BOOL bSetMode);

DRV_PNL_INTERFACE void MDrv_PNL_HWLVDSReservedtoLRFlag(void *pInstance, PNL_DrvHW_LVDSResInfo lvdsresinfo);
DRV_PNL_INTERFACE void MDrv_PNL_OverDriver_Init(void *pInstance, MS_PHY u32OD_MSB_Addr, MS_PHY u32OD_LSB_Addr, MS_U8 u8ODTbl[1056]);
DRV_PNL_INTERFACE void MDrv_PNL_OverDriver_Enable(void *pInstance, MS_BOOL bEnable);
DRV_PNL_INTERFACE MS_U32 MDrv_PNL_CalculateLPLLSETbyDClk(void *pInstance, MS_U64 ldHz, MS_BOOL bHighAccurate);

#define MDrv_MOD_PVDD_Power_Setting  MHal_MOD_PVDD_Power_Setting
DRV_PNL_INTERFACE void MDrv_MOD_PVDD_Power_Setting(void *pInstance, MS_BOOL bIs2p5);

#define MDrv_PNL_MISC_Control  MHal_PNL_MISC_Control
DRV_PNL_INTERFACE void MDrv_PNL_MISC_Control(void *pInstance, MS_U32 u32PNL_MISC);

DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_Is_SupportFRC(void *pInstance);
DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_Is_SupportTCON(void *pInstance);
DRV_PNL_INTERFACE void MDrv_FRC_MOD_ForcePairSwap(void *pInstance, MS_U32 u32Polarity);
MS_U16 MDrv_PNL_Get_DEVstart(void *pInstance);
MS_U16 MDrv_PNL_Get_DEHstart(void *pInstance);
MS_U16 MDrv_PNL_GetDefaultVfreq(void *pInstance, MS_U16 u16Vfreq);
MS_BOOL MDrv_PNL_isYUVOutput(void *pInstance);
MS_BOOL MDrv_PNL_GetDACOut(void *pInstance);

void MDrv_PNL_CalExtLPLLSETbyDClk(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 u8LPLL_Type, MS_U64 ldHz);

MS_BOOL MDrv_PNL_EnableInternalTermination(void *pInstance, MS_BOOL bEnable);
DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_VBY1_Handshake(void *pInstance);
DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_VBY1_OC_Handshake(void *pInstance);
DRV_PNL_INTERFACE PNL_Result MDrv_PNL_SetOutputInterlaceTiming(void *pInstance, MS_BOOL bEnable);
DRV_PNL_INTERFACE void MDrv_PNL_GetOutputInterlaceTiming(void *pInstance, MS_BOOL* bIsInterlaceOutput);
DRV_PNL_INTERFACE void MDrv_PNL_SetOSDCOutputType(void *pInstance, MS_U16 eLPLL_Type, MS_U8 eOC_OutputFormat);
DRV_PNL_INTERFACE MS_U32 MDrv_PNL_Get_Semaphore(void *pInstance,E_PNL_POOL_ID eID);
DRV_PNL_INTERFACE MS_U32 MDrv_PNL_Release_Semaphore(void *pInstance,E_PNL_POOL_ID eID);
DRV_PNL_INTERFACE MS_U32 MDrv_PNL_GetDeviceNum(void);
DRV_PNL_INTERFACE MS_U16 MDrv_PNL_GetPanelVStart(void);
DRV_PNL_INTERFACE MS_BOOL MDrv_PNL_Check_VBY1_Handshake_Status(void *pInstance);
DRV_PNL_INTERFACE void MDrv_PNL_MOD_PECurrent_Setting(void *pInstance, MS_U16 u16Current_Level, MS_U16 u16Channel_Select);
DRV_PNL_INTERFACE void MDrv_PNL_VBY1_Hardware_TrainingMode_En(void *pInstance, MS_BOOL bIsVideoMode ,MS_BOOL bEnable);
DRV_PNL_INTERFACE const char* MDrv_PNL_GetName(void);

#endif // _DRV_PNL_H_

