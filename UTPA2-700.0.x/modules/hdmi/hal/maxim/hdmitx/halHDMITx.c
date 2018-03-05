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

#define  MHAL_HDMITX_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"

// Internal Definition
#include "regHDMITx.h"
#include "halHDMIUtilTx.h"
#include "halHDMITx.h"
#include "drvHDMITx.h"

// External Definition
#include "drvGPIO.h"
#include "drvSYS.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define GENERAL_PKT_NUM		0x0BU //wilson@kano
#define INFOFRM_PKT_NUM		0x08U //for HDR packet ID = 0x87;  0x06U //wilson@kano


#define HDMITX_VS_INFO_PKT_VER				0x01U
#define HDMITX_VS_INFO_PKT_LEN				0x1BU

#define HDMITX_AVI_INFO_PKT_VER				0x02U
#define HDMITX_AVI_INFO_PKT_LEN				0x0DU

#define HDMITX_SPD_INFO_PKT_VER				0x01U
#define HDMITX_SPD_INFO_PKT_LEN				0x19U

#define HDMITX_AUD_INFO_PKT_VER				0x01U
#define HDMITX_AUD_INFO_PKT_LEN				0x0AU

#define HDMITX_HDR_INFO_PKT_VER             0x01U
#define HDMITX_HDR_INFO_PKT_LEN             0x1BU //wilson@kano: temp solution



#define     IS_STOP_PKT(_X_)            ( (_X_ & E_HDMITX_STOP_PACKET) ? 1 : 0 )
#define     IS_CYCLIC_PKT(_X_)          ( (_X_ & E_HDMITX_CYCLIC_PACKET) ? 1 : 0 )

// HDMI packet cyclic frame count
#define HDMITX_PACKET_NULL_FCNT         0U           ///< 0 ~ 31
#define HDMITX_PACKET_ACR_FCNT          0U           ///< 0 ~ 15
#define HDMITX_PACKET_GC_FCNT           0U           ///< 0 ~ 1
#define HDMITX_PACKET_ACP_FCNT          15U          ///< 0 ~ 31
#define HDMITX_PACKET_ISRC_FCNT         15U          ///< 0 ~ 31

#define HDMITX_PACKET_VS_FCNT           0U          ///< 0 ~ 31
#define HDMITX_PACKET_AVI_FCNT          0U           ///< 0 ~ 31
#define HDMITX_PACKET_SPD_FCNT          1U          ///< 0 ~ 31
#define HDMITX_PACKET_AUD_FCNT          0U           ///< 0 ~ 31
#define HDMITX_PACKET_HDR_FCNT          0U

#define HDMITX_PACKET_SPD_SDI           1U   // Digital STB
#define HDMITX_CSC_SUPPORT_R2Y          1U
#define HDMITX_SUPPORT_P2I              1U
#define HDMITX_SUPPORT_444TO420         1U
#define HDMITX_MAX_PIXEL_CLK            597000000   //Max cupported pixel clock
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//*********************//
//             Video   //
//*********************//

typedef enum
{
    E_HDMITX_VIDEO_INTERLACE_MODE     = 0,
    E_HDMITX_VIDEO_PROGRESSIVE_MODE = 1,
} MDrv_HDMITx_VIDEO_MODE;

typedef enum
{
    E_HDMITX_VIDEO_POLARITY_HIGH  = 0,
    E_HDMITX_VIDEO_POLARITY_LOW = 1,
} MDrv_HDMITx_VIDEO_POLARITY;

typedef struct
{
    MDrv_HDMITx_VIDEO_MODE          i_p_mode;         // interlace / progressive mode
    MDrv_HDMITx_VIDEO_POLARITY   h_polarity;        // Hsync polarity
    MDrv_HDMITx_VIDEO_POLARITY   v_polarity;        // Vsync polarity
    MS_U16                                            vs_width;          // Vsync pulse width
    MS_U16                                            vs_bporch;        // Vsync back-porch
    MS_U16                                            vde_width;        // Vde active width
    MS_U16                                            vs_delayline;    // Vsync line delay
    MS_U16                                            vs_delaypixel;  // Vsync pixel delay
    MS_U16                                            hs_width;          // Hsync pulse width
    MS_U16                                            hs_bporch;        // Hsync back-porch
    MS_U16                                            hde_width;        // Hde active width
    MS_U16                                            hs_delay;          // Hsync delay
    MS_U16                                            vtotal;               // Vsync total
    MS_U16                                            htotal;	         // Hsync total
    MS_U16                                            frame_rate;       // Frame Rate
    MS_U32                                            pixel_clk;        //pixel clock
} MDrv_HDMITx_VIDEO_MODE_INFO_TYPE;

//*********************//
//             Packet  //
//*********************//

typedef enum
{
    E_HDMITX_ACT_GCP_CMD    = 0,
    E_HDMITX_ACT_ACR_CMD    = 1,
    E_HDMITX_ACT_AVI_CMD    = 2,
    E_HDMITX_ACT_AUD_CMD    = 3,
    E_HDMITX_ACT_SPD_CMD    = 4,
    E_HDMITX_ACT_MPG_CMD    = 5,
    E_HDMITX_ACT_VSP_CMD    = 6,
    E_HDMITX_ACT_NUL_CMD    = 7,
    E_HDMITX_ACT_ACP_CMD    = 8,
    E_HDMITX_ACT_ISRC_CMD    = 9,
    E_HDMITX_ACT_GCP_DC_CMD    = 10, // GCP with non-zero CD
    E_HDMITX_ACT_GMP_CMD    = 11, // Gamut Metadata packet
} MDrvHDMITX_PKTS_ACT_CMD;

typedef struct PKT
{
    MS_BOOL User_Define;
    MsHDMITX_PACKET_PROCESS Define_Process;
    MS_U8 Define_FCnt;
}PKT_Behavior;
//*********************//
//             Audio   //
//*********************//
typedef struct
{
    MS_U8     CH_Status3;
    MS_U32   NcodeValue;
} MDrv_HDMITx_AUDIO_FREQ_TYPE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8     gDivider = 0x00;
MS_BOOL   gDivFlag = FALSE;
static MS_U8 gu8ChipVerNum = 0x00;
MS_BOOL     g_bSupportSCDC = FALSE;

stHDMITx_PKT_ATTRIBUTE gbGeneralPktList[GENERAL_PKT_NUM]; //wilson@kano
stHDMITx_PKT_ATTRIBUTE gbInfoFrmPktList[INFOFRM_PKT_NUM]; //wilson@kano

// User defined packet behavior
PKT_Behavior NULL_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior ACR_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior AS_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior GC_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior ACP_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior ISRC1_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior ISRC2_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior DSD_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior HBR_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior GM_PACKET = {FALSE, E_HDMITX_STOP_PACKET, 0};

PKT_Behavior VS_INFORAME = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior AVI_INFORAME = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior SPD_INFORAME = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior AUDIO_INFORAME = {FALSE, E_HDMITX_STOP_PACKET, 0};
PKT_Behavior MPEG_INFORAME = {FALSE, E_HDMITX_STOP_PACKET, 0};
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//*********************//
//             Video   //
//*********************//

// It should be mapped with MsHDMITX_VIDEO_TIMING structure in drvHDMITx.h
MDrv_HDMITx_VIDEO_MODE_INFO_TYPE HDMITxVideoModeTbl[E_HDMITX_RES_MAX]=
{
    //IorPMode;                                             PolarityH;                                          PolarityV;                               VSW;      VbckP;   VDe;       VSDel;  VSDelP;  HSW;      HbckP;   HDe;      HSDel;    VSTotal; HSTotal;
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_LOW,    E_HDMITX_VIDEO_POLARITY_LOW,    0x0002, 0x0021, 0x01E0, 0x000A,     0, 0x0060, 0x0030, 0x0280, 0x0010, 0x020D, 0x0320,  60, 25175000},  // 0: 640x480p
    {E_HDMITX_VIDEO_INTERLACE_MODE,     E_HDMITX_VIDEO_POLARITY_LOW,    E_HDMITX_VIDEO_POLARITY_LOW,    0x0003, 0x000F, 0x01E0, 0x0004,     0, 0x007C, 0x0072, 0x05A0, 0x0026, 0x020D, 0x06B4,  60, 27000000},  // 1: 720x480i
    {E_HDMITX_VIDEO_INTERLACE_MODE,     E_HDMITX_VIDEO_POLARITY_LOW,    E_HDMITX_VIDEO_POLARITY_LOW,    0x0003, 0x0013, 0x0240, 0x0002,     0, 0x007E, 0x008A, 0x05A0, 0x0018, 0x0271, 0x06C0,  50, 27000000},  // 2: 720x576i
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_LOW,    E_HDMITX_VIDEO_POLARITY_LOW,    0x0006, 0x001E, 0x01E0, 0x0009,     0, 0x003E, 0x003C, 0x02D0, 0x0010, 0x020D, 0x035A,  60, 27000000},  // 3: 720x480p
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_LOW,    E_HDMITX_VIDEO_POLARITY_LOW,    0x0005, 0x0027, 0x0240, 0x0005,     0, 0x0040, 0x0044, 0x02D0, 0x000C, 0x0271, 0x0360,  50, 27000000},  // 4: 720x576p
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0014, 0x02D0, 0x0005,     0, 0x0028, 0x00DC, 0x0500, 0x01B8, 0x02EE, 0x07BC,  50, 74250000},  // 5: 1280x720p_50Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0014, 0x02D0, 0x0005,     0, 0x0028, 0x00DC, 0x0500, 0x006E, 0x02EE, 0x0672,  60, 74250000},  // 6: 1280x720p_60Hz
    {E_HDMITX_VIDEO_INTERLACE_MODE,     E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x000F, 0x0438, 0x0002,     0, 0x002C, 0x0094, 0x0780, 0x0210, 0x0465, 0x0A50,  50, 74250000},  // 7: 1920x1080i_50Hz
    {E_HDMITX_VIDEO_INTERLACE_MODE,     E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x000F, 0x0438, 0x0002,     0, 0x002C, 0x0094, 0x0780, 0x0058, 0x0465, 0x0898,  60, 74250000},  // 8: 1920x1080i_60Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0024, 0x0438, 0x0004,     0, 0x002C, 0x0094, 0x0780, 0x027E, 0x0465, 0x0ABE,  24, 74250000},  // 9: 1920x1080p_24Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0024, 0x0438, 0x0004,     0, 0x002C, 0x0094, 0x0780, 0x0210, 0x0465, 0x0A50,  25, 74250000},  // 10: 1920x1080p_25Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0024, 0x0438, 0x0004,     0, 0x002C, 0x0094, 0x0780, 0x0058, 0x0465, 0x0898,  30, 74250000},  // 11: 1920x1080p_30Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0024, 0x0438, 0x0004,     0, 0x002C, 0x0094, 0x0780, 0x0210, 0x0465, 0x0A50,  50, 148500000},  // 12: 1920x1080p_50Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0024, 0x0438, 0x0004,     0, 0x002C, 0x0094, 0x0780, 0x0058, 0x0465, 0x0898,  60, 148500000},  // 13: 1920x1080p_60Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0024, 0x089D, 0x0004,     0, 0x002C, 0x0094, 0x0780, 0x027E, 0x08CA, 0x0ABE,  24, 148500000},  // 14: 1920x2205p_24Hz //3D: 1920x1080p_24Hz x2
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0014, 0x05BE, 0x0005,     0, 0x0028, 0x00DC, 0x0500, 0x01B8, 0x05DC, 0x07BC,  50, 148500000},  // 15: 1280x1470p_50Hz //3D: 1280x720_50Hz x2
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0005, 0x0014, 0x05BE, 0x0005,     0, 0x0028, 0x00DC, 0x0500, 0x006E, 0x05DC, 0x0672,  60, 148500000},  // 16: 1280x1470p_60Hz //3D: 1280x720_60Hz x2
    
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0128, 0x0F00, 0x04FC, 0x08CA, 0x157C,  24, 297000000},  // 17:93: 3840x2160p_24Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0128, 0x0F00, 0x0420, 0x08CA, 0x14A0,  25, 297000000},  // 18:94: 3840x2160p_25Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0128, 0x0F00, 0x00B0, 0x08CA, 0x1130,  30, 297000000},  // 19:95: 3840x2160p_30Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0128, 0x0F00, 0x0420, 0x08CA, 0x14A0,  50, 594000000},  // 20:96: 3840x2160p_50Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0128, 0x0F00, 0x00B0, 0x08CA, 0x1130,  60, 594000000},  // 21:97: 3840x2160p_60Hz

    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0128, 0x1000, 0x03FC, 0x08CA, 0x157C,  24, 297000000},  // 22:98: 4096x2160p_24Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0080, 0x1000, 0x03C8, 0x08CA, 0x14A0,  25, 297000000},  // 23:99: 4096x2160p_25Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0080, 0x1000, 0x0058, 0x08CA, 0x1130,  30, 297000000},  // 24:100: 4096x2160p_30Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008,     0, 0x0058, 0x0080, 0x1000, 0x03C8, 0x08CA, 0x14A0,  50, 594000000},  // 25:101: 4096x2160p_50Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x000A, 0x0048, 0x0870, 0x0008, 	0, 0x0058, 0x0080, 0x1000, 0x0058, 0x08CA, 0x1130,  60, 594000000},  // 26:102: 4096x2160p_60Hz

    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0003, 0x002E, 0x04B0, 0x0001,     0, 0x00C0, 0x0130, 0x0640, 0x0040, 0x04E2, 0x0870,  60, 162000000},  // 27: 1600x1200p_60Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0006, 0x0019, 0x0384, 0x0003,     0, 0x0098, 0x00E8, 0x05A0, 0x0050, 0x03A6, 0x0770,  60, 106500000},  // 28: 1440x900p_60Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0003, 0x0026, 0x0400, 0x0001,     0, 0x0070, 0x00F8, 0x0500, 0x0030, 0x042A, 0x0698,  60, 108000000},  // 29: 1280x1024p_60Hz
    {E_HDMITX_VIDEO_PROGRESSIVE_MODE,   E_HDMITX_VIDEO_POLARITY_HIGH,   E_HDMITX_VIDEO_POLARITY_HIGH,   0x0006, 0x001D, 0x0300, 0x0003,     0, 0x0088, 0x00A0, 0x0400, 0x0024, 0x0326, 0x0540,  60, 65000000},   // 30: 1024x768p_60Hz
};

//atop setting
stHDMITx_ATOP_SETTING HDMITxVideoAtopSetting[HDMITX_COLOR_DEPTH_TYPE_NUM][E_HDMITX_RES_MAX] =
{
    //color depth = 8bit
    {
        {0x03, 0x03, 0x00, 0x03, 0x14, 0x0F, 0x400000},  // 0: 640x480p
        {0x03, 0x03, 0x00, 0x03, 0x14, 0x0F, 0x400000},  // 1: 720x480i
        {0x03, 0x03, 0x00, 0x03, 0x14, 0x0F, 0x400000},  // 2: 720x576i
        {0x03, 0x03, 0x00, 0x03, 0x14, 0x0F, 0x400000},  // 3: 720x480p
        {0x03, 0x03, 0x00, 0x03, 0x14, 0x0F, 0x400000},  // 4: 720x576p
// 74Mhz
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 5: 1280x720p_50Hz
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 6: 1280x720p_60Hz
        #if (defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 7: 1920x1080i_50Hz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 8: 1920x1080i_60Hz
        #else
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 7: 1920x1080i_50Hz
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x2E978D},  // 8: 1920x1080i_60Hz
        #endif
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 9: 1920x1080p_24Hz
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 10: 1920x1080p_25Hz
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 11: 1920x1080p_30Hz
// 148Mhz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 12: 1920x1080p_50Hz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 13: 1920x1080p_60Hz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 14: 1920x2205p_24Hz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 15: 1280x1470p_50Hz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
// 300Mhz
        {0x01, 0x01, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 17:93: 3840x2160p_24Hz
        {0x01, 0x01, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 18:94: 3840x2160p_25Hz
        {0x01, 0x01, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 19:95: 3840x2160p_30Hz
// 600Mhz
        {0x00, 0x00, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 20:96: 3840x2160p_50Hz
        {0x00, 0x00, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 21:97: 3840x2160p_60Hz
// 300Mhz
        {0x01, 0x01, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 22:98: 4096x2160p_24Hz
        {0x01, 0x01, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 23:99: 4096x2160p_25Hz
        {0x01, 0x01, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 24:100: 4096x2160p_30Hz
// 600Mhz
        {0x00, 0x00, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 25:101: 4096x2160p_50Hz
        {0x00, 0x00, 0x00, 0x0C, 0x38, 0x08, 0x1745D1},  // 26:102: 4096x2160p_60Hz

// 150MHz
        {0x01, 0x01, 0x00, 0x0C, 0x38, 0x08, 0x2AAAAA},  // 27: 1600x1200p_60Hz
// 106.5MHz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x40EB71},  // 28: 1440x900p_60Hz
// 108MHz
        {0x01, 0x01, 0x00, 0x03, 0x14, 0x0F, 0x400000},  // 29: 1280x1024p_60Hz
// 65MHz
        {0x02, 0x02, 0x00, 0x03, 0x14, 0x0F, 0x352B52},  // 30: 1024x768p_60Hz
    },

    //color depth  = 10 bit
    {
        {0x03, 0x03, 0x01, 0x03, 0x14, 0x0F, 0x333333},  // 0: 640x480p
        {0x03, 0x03, 0x01, 0x03, 0x14, 0x0F, 0x333333},  // 1: 720x480i
        {0x03, 0x03, 0x01, 0x03, 0x14, 0x0F, 0x333333},  // 2: 720x576i
        {0x03, 0x03, 0x01, 0x03, 0x14, 0x0F, 0x333333},  // 3: 720x480p
        {0x03, 0x03, 0x01, 0x03, 0x14, 0x0F, 0x333333},  // 4: 720x576p
// 74Mhz
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 5: 1280x720p_50Hz
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 6: 1280x720p_60Hz
        #if (defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 7: 1920x1080i_50Hz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 8: 1920x1080i_60Hz
        #else
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 7: 1920x1080i_50Hz
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x25460A},  // 8: 1920x1080i_60Hz
        #endif
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 9: 1920x1080p_24Hz
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 10: 1920x1080p_25Hz
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 11: 1920x1080p_30Hz
// 148Mhz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 12: 1920x1080p_50Hz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 13: 1920x1080p_60Hz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 14: 1920x2205p_24Hz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 15: 1280x1470p_50Hz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x253C82},  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
        
// 300Mhz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 17:93: 3840x2160p_24Hz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 18:94: 3840x2160p_25Hz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 19:95: 3840x2160p_30Hz
// 600Mhz ==> not support !!!
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 20:96: 3840x2160p_50Hz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 21:97: 3840x2160p_60Hz
// 300Mhz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 22:98: 4096x2160p_24Hz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 23:99: 4096x2160p_25Hz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 24:100: 4096x2160p_30Hz
// 600Mhz ==> not support !!!
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 25:101: 4096x2160p_50Hz
        {0x00, 0x00, 0x01, 0x0C, 0x38, 0x08, 0x253C82},  // 26:102: 4096x2160p_60Hz

// 150MHz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x222222},  // 27: 1600x1200p_60Hz
// 106.5MHz
        //{0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x33EBCE},  // 28: 1440x900p_60Hz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x33EF8D},
// 108MHz
        {0x01, 0x01, 0x01, 0x03, 0x14, 0x0F, 0x333333},  // 29: 1280x1024p_60Hz
// 65MHz
        {0x02, 0x02, 0x01, 0x03, 0x14, 0x0F, 0x2A890E},  // 30: 1024x768p_60Hz
    },

    //color depth = 12 bit
    {
        {0x03, 0x03, 0x02, 0x03, 0x14, 0x0F, 0x2AAAAA},  // 0: 640x480p
        {0x03, 0x03, 0x02, 0x03, 0x14, 0x0F, 0x2AAAAA},  // 1: 720x480i
        {0x03, 0x03, 0x02, 0x03, 0x14, 0x0F, 0x2AAAAA},  // 2: 720x576i
        {0x03, 0x03, 0x02, 0x03, 0x14, 0x0F, 0x2AAAAA},  // 3: 720x480p
        {0x03, 0x03, 0x02, 0x03, 0x14, 0x0F, 0x2AAAAA},  // 4: 720x576p
// 74Mhz
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x1F07C1},  // 5: 1280x720p_50Hz
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x1F07C1},  // 6: 1280x720p_60Hz
        #if (defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 7: 1920x1080i_50Hz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 8: 1920x1080i_60Hz
        #else
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x1F07C1},  // 7: 1920x1080i_50Hz
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x1F0FB3},  // 8: 1920x1080i_60Hz
        #endif
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x1F07C1},  // 9: 1920x1080p_24Hz
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x1F07C1},  // 10: 1920x1080p_25Hz
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x1F07C1},  // 11: 1920x1080p_30Hz
// 148Mhz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 12: 1920x1080p_50Hz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 13: 1920x1080p_60Hz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 14: 1920x2205p_24Hz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 15: 1280x1470p_50Hz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
        
// 300Mhz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 17:93: 3840x2160p_24Hz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 18:94: 3840x2160p_25Hz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 19:95: 3840x2160p_30Hz
// 600Mhz ==> not support !!!
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 20:96: 3840x2160p_50Hz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 21:97: 3840x2160p_60Hz
// 300Mhz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 22:98: 4096x2160p_24Hz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 23:99: 4096x2160p_25Hz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 24:100: 4096x2160p_30Hz
// 600Mhz ==> not support !!!
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 25:101: 4096x2160p_50Hz
        {0x00, 0x00, 0x02, 0x0C, 0x38, 0x08, 0x1F07C1},  // 26:102: 4096x2160p_60Hz

// 150MHz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x04, 0x1C71C7},  // 27: 1600x1200p_60Hz
// 106.5MHz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x2B47A0},  // 28: 1440x900p_60Hz
// 108MHz
        {0x01, 0x01, 0x02, 0x0C, 0x38, 0x08, 0x2AAAAA},  // 29: 1280x1024p_60Hz
// 65MHz
        {0x02, 0x02, 0x02, 0x03, 0x14, 0x0F, 0x237237},  // 30: 1024x768p_60Hz
    },

    //color depth = 16 bit
    {
        {0x02, 0x02, 0x03, 0x03, 0x14, 0x0F, 0x400000},  // 0: 640x480p
        {0x02, 0x02, 0x03, 0x03, 0x14, 0x0F, 0x400000},  // 1: 720x480i
        {0x02, 0x02, 0x03, 0x03, 0x14, 0x0F, 0x400000},  // 2: 720x576i
        {0x02, 0x02, 0x03, 0x03, 0x14, 0x0F, 0x400000},  // 3: 720x480p
        {0x02, 0x02, 0x03, 0x03, 0x14, 0x0F, 0x400000},  // 4: 720x576p
// 74Mhz
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 5: 1280x720p_50Hz
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 6: 1280x720p_60Hz
        #if (defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x2E8BA2},  // 7: 1920x1080i_50Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x2E8BA2},  // 8: 1920x1080i_60Hz
        #else
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 7: 1920x1080i_50Hz
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x2E978D},  // 8: 1920x1080i_60Hz
        #endif
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 9: 1920x1080p_24Hz
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 10: 1920x1080p_25Hz
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x2E8BA2},  // 11: 1920x1080p_30Hz
// 148Mhz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x2E8BA2},  // 12: 1920x1080p_50Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x2E8BA2},  // 13: 1920x1080p_60Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x2E8BA2},  // 14: 1920x2205p_24Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x2E8BA2},  // 15: 1280x1470p_50Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x2E8BA2},  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
        
// 300Mhz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 17:93: 3840x2160p_24Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 18:94: 3840x2160p_25Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 19:95: 3840x2160p_30Hz
// 600Mhz ==> Not support !!!
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 20:96: 3840x2160p_50Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 21:97: 3840x2160p_60Hz
// 300Mhz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 22:98: 4096x2160p_24Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 23:99: 4096x2160p_25Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 24:100: 4096x2160p_30Hz
// 600Mhz ==> Not support !!!
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 25:101: 4096x2160p_50Hz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x1745D1},  // 26:102: 4096x2160p_60Hz

// 150MHz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x04, 0x2AAAAA},  // 27: 1600x1200p_60Hz
// 106.5MHz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x40EB71},  // 28: 1440x900p_60Hz
// 108MHz
        {0x00, 0x00, 0x03, 0x0C, 0x38, 0x08, 0x400000},  // 29: 1280x1024p_60Hz
// 65MHz
        {0x01, 0x01, 0x03, 0x03, 0x14, 0x0F, 0x352B52},  // 30: 1024x768p_60Hz
    },
};

#if( defined(HDMITX_SUPPORT_444TO420) && (HDMITX_SUPPORT_444TO420 == 1) )
//For Maxim 420 and Interlace Only HDMITXPHY_0x15[14:12]
MS_U8 HDMITxVideo_OTMDS_ODATA_Ratio[HDMITX_COLOR_DEPTH_TYPE_NUM][E_HDMITX_RES_MAX] =
{
    //color depth = 8bit
    {
        0x03,  // 0: 640x480p
        0x03,  // 1: 720x480i
        0x03,  // 2: 720x576i
        0x03,  // 3: 720x480p
        0x03,  // 4: 720x576p
// 74Mhz
        0x02,  // 5: 1280x720p_50Hz
        0x02,  // 6: 1280x720p_60Hz
        0x02,  // 7: 1920x1080i_50Hz
        0x02,  // 8: 1920x1080i_60Hz
        0x02,  // 9: 1920x1080p_24Hz
        0x02,  // 10: 1920x1080p_25Hz
        0x02,  // 11: 1920x1080p_30Hz
// 148Mhz
        0x02,  // 12: 1920x1080p_50Hz
        0x02,  // 13: 1920x1080p_60Hz
        0x02,  // 14: 1920x2205p_24Hz
        0x02,  // 15: 1280x1470p_50Hz
        0x02,  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
// 300Mhz
        0x02,  // 17:93: 3840x2160p_24Hz
        0x02,  // 18:94: 3840x2160p_25Hz
        0x02,  // 19:95: 3840x2160p_30Hz
// 600Mhz
        0x01,  // 20:96: 3840x2160p_50Hz
        0x01,  // 21:97: 3840x2160p_60Hz
// 300Mhz
        0x02,  // 22:98: 4096x2160p_24Hz
        0x02,  // 23:99: 4096x2160p_25Hz
        0x02,  // 24:100: 4096x2160p_30Hz
// 600Mhz
        0x01,  // 25:101: 4096x2160p_50Hz
        0x01,  // 26:102: 4096x2160p_60Hz
    },

    //color depth  = 10 bit
    {
        0x03,  // 0: 640x480p
        0x03,  // 1: 720x480i
        0x03,  // 2: 720x576i
        0x03,  // 3: 720x480p
        0x03,  // 4: 720x576p
// 74Mhz
        0x02,  // 5: 1280x720p_50Hz
        0x02,  // 6: 1280x720p_60Hz
        0x02,  // 7: 1920x1080i_50Hz
        0x02,  // 8: 1920x1080i_60Hz
        0x02,  // 9: 1920x1080p_24Hz
        0x02,  // 10: 1920x1080p_25Hz
        0x02,  // 11: 1920x1080p_30Hz
// 148Mhz
        0x02,  // 12: 1920x1080p_50Hz
        0x02,  // 13: 1920x1080p_60Hz
        0x02,  // 14: 1920x2205p_24Hz
        0x02,  // 15: 1280x1470p_50Hz
        0x02,  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
// 300Mhz
        0x02,  // 17:93: 3840x2160p_24Hz
        0x02,  // 18:94: 3840x2160p_25Hz
        0x02,  // 19:95: 3840x2160p_30Hz
// 600Mhz ==> not support !!!
        0x00,  // 20:96: 3840x2160p_50Hz
        0x00,  // 21:97: 3840x2160p_60Hz
// 300Mhz
        0x02,  // 22:98: 4096x2160p_24Hz
        0x02,  // 23:99: 4096x2160p_25Hz
        0x02,  // 24:100: 4096x2160p_30Hz
// 600Mhz ==> not support !!!
        0x00,  // 25:101: 4096x2160p_50Hz
        0x00,  // 26:102: 4096x2160p_60Hz
    },

    //color depth = 12 bit
    {
        0x03,  // 0: 640x480p
        0x03,  // 1: 720x480i
        0x03,  // 2: 720x576i
        0x03,  // 3: 720x480p
        0x03,  // 4: 720x576p
// 74Mhz
        0x02,  // 5: 1280x720p_50Hz
        0x02,  // 6: 1280x720p_60Hz
        0x02,  // 7: 1920x1080i_50Hz
        0x02,  // 8: 1920x1080i_60Hz
        0x02,  // 9: 1920x1080p_24Hz
        0x02,  // 10: 1920x1080p_25Hz
        0x02,  // 11: 1920x1080p_30Hz
// 148Mhz
        0x02,  // 12: 1920x1080p_50Hz
        0x02,  // 13: 1920x1080p_60Hz
        0x02,  // 14: 1920x2205p_24Hz
        0x02,  // 15: 1280x1470p_50Hz
        0x02,  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
// 300Mhz
        0x02,  // 17:93: 3840x2160p_24Hz
        0x02,  // 18:94: 3840x2160p_25Hz
        0x02,  // 19:95: 3840x2160p_30Hz
// 600Mhz ==> not support !!!
        0x00,  // 20:96: 3840x2160p_50Hz
        0x00,  // 21:97: 3840x2160p_60Hz
// 300Mhz
        0x02,  // 22:98: 4096x2160p_24Hz
        0x02,  // 23:99: 4096x2160p_25Hz
        0x02,  // 24:100: 4096x2160p_30Hz
// 600Mhz ==> not support !!!
        0x00,  // 25:101: 4096x2160p_50Hz
        0x00,  // 26:102: 4096x2160p_60Hz
    },

    //color depth = 16 bit
    {
        0x02,  // 0: 640x480p
        0x02,  // 1: 720x480i
        0x02,  // 2: 720x576i
        0x02,  // 3: 720x480p
        0x02,  // 4: 720x576p
// 74Mhz
        0x01,  // 5: 1280x720p_50Hz
        0x01,  // 6: 1280x720p_60Hz
        0x01,  // 7: 1920x1080i_50Hz
        0x01,  // 8: 1920x1080i_60Hz
        0x01,  // 9: 1920x1080p_24Hz
        0x01,  // 10: 1920x1080p_25Hz
        0x01,  // 11: 1920x1080p_30Hz
// 148Mhz
        0x01,  // 12: 1920x1080p_50Hz
        0x01,  // 13: 1920x1080p_60Hz
        0x01,  // 14: 1920x2205p_24Hz
        0x01,  // 15: 1280x1470p_50Hz
        0x01,  // 16: 1280x1470p_60Hz
        // the following is 4k2k timing list, if not support, default is 3840x2160p@30
// 300Mhz
        0x01,  // 17:93: 3840x2160p_24Hz
        0x01,  // 18:94: 3840x2160p_25Hz
        0x01,  // 19:95: 3840x2160p_30Hz
// 600Mhz ==> Not support !!!
        0x00,  // 20:96: 3840x2160p_50Hz
        0x00,  // 21:97: 3840x2160p_60Hz
// 300Mhz
        0x01,  // 22:98: 4096x2160p_24Hz
        0x01,  // 23:99: 4096x2160p_25Hz
        0x01,  // 24:100: 4096x2160p_30Hz
// 600Mhz ==> Not support !!!
        0x00,  // 25:101: 4096x2160p_50Hz
        0x00,  // 26:102: 4096x2160p_60Hz
    },
};
#endif

//*********************//
//             Audio   //
//*********************//

MSTHDMITX_REG_TYPE HDMITxAudioOnTbl[] =
{
    {HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, 0x1087, 0x1086}, //[12]: CH status swap[7:0], [7]: enable audio FIFO, [2]:enable CTS Gen, [1]: automatically block start, [0]: audio FIFO not flush
};

MSTHDMITX_REG_TYPE HDMITxAudioOffTbl[] =
{
    {HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, 0x1087, 0x1003}, //[12]: CH status swap[7:0], [7]: disable audio FIFO, [2]:disable CTS Gen, [1]: automatically block start, [0]: audio FIFO flush
};

MSTHDMITX_REG_TYPE HDMITxAudioInitTbl[] =
{
    {HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, 0x1087, 0x1003}, //[12]: CH status swap[7:0], [7]: disable audio FIFO, [2]:disable CTS Gen, [1]: automatically block start, [0]: audio FIFO flush
};

MDrv_HDMITx_AUDIO_FREQ_TYPE TxAudioFreqTbl[E_HDMITX_AUDIO_FREQ_MAX_NUM] =
{
    {0x02, 0x001800},  // No signal, set to 48 KHz
    {0x03, 0x001000},  // 0 ~(32)~ 38 KHz,  4096
    {0x00, 0x001880},  // 38 ~(44.1)~ 46 KHz, 6272
    {0x02, 0x001800},  // 46 ~(48)~ 60 KHz, 6144
    {0x08, 0x003100}, //  60 ~(88.2)~ 92 KHz, 12544
    {0x0a, 0x003000}, //  92 ~(96)~ 140 KHz, 12288
    {0x0c, 0x006200}, //  140 ~(176.4)~ 180 KHz, 25088
    {0x0e, 0x006000},  //  180 ~(192)~ ~~ KHz, 24576
};

//*********************//
//             Packet  //
//*********************//

MS_U8 HDMITX_AviCmrTbl[E_HDMITX_RES_MAX] =
{
    0x48, 0x48, 0x48, 0x48, 0x48, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,            // SDTV C=01(601),M=00(no data) ,R=1000(same)
    0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0x48,
    0xA8, 0x48, 0x48,
};

MS_U8 HDMITX_AviVicTbl[E_HDMITX_RES_MAX] =
{
    1,  6,  21, 2,  17, 19, 4,  20, 5,  32, 33,  34,  31,  16,           // SDTV 480i60,576i50,480p60,576p50,720p50,720p60,1080i50,1080i60,1080p24,1080p25,1080p30, 1080p50, 1080p60
    32, 19, 4,  0,  0,  0,  96, 97, 0,  99, 100, 101, 102,  0,
    0,  0,  0,
};

MS_U8 HDMITX_VendorName[8] =
{
    "MStar   "
};

MS_U8 HDMITX_ProductName[16] =
{
    "HDMI Tx Demo",
};

MS_U8 _gHPDGpioPin = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

#ifdef MS_DEBUG
#define DBG_HDMITX(_f)                  (_f)
#else
#define DBG_HDMITX(_f)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
/*********************************************************************/
/*                                                                                                                     */
/*                                         HDCP22 Relative                                                    */
/*                                                                                                                     */
/*********************************************************************/
void MHal_HDMITx_HDCP2TxInit(MS_BOOL bEnable)
{
    MHal_HDMITx_Mask_Write(HDMITX_HDCP2TX_BASE, 0x0000, 0x11, bEnable ? 0x11 : 0x00); // bit 0: enable hdcp22; bit 4: enable EESS
    if (bEnable)
    {
        MHal_HDMITx_Mask_Write(HDMITX_HDCP2TX_BASE, 0x0000, 0x02, 0x02); //reset hdcp22 FSM
        MHal_HDMITx_Mask_Write(HDMITX_HDCP2TX_BASE, 0x0000, 0x02, 0x00);
    }
}

void MHal_HDMITx_HDCP2TxEnableEncryptEnable(MS_BOOL bEnable)
{
    MHal_HDMITx_Mask_Write(HDMITX_HDCP2TX_BASE, 0x0000, 0x04, bEnable ? 0x04 : 0x00); //bit 2: authentication pass
    MHal_HDMITx_Mask_Write(HDMITX_HDCP2TX_BASE, 0x0000, 0x08, bEnable ? 0x08 : 0x00); //bit 3: enable hdcp22 to issue encryption enable signal
}

void MHal_HDMITx_HDCP2TxFillCipherKey(MS_U8 *pu8Riv, MS_U8 *pu8KsXORLC128)
{
#define DEF_HDCP2CIPHER_DBG 0
#define SIZE_OF_KSXORLC128  16
#define SIZE_OF_RIV         8

    MS_U8 cnt = 0;

#if (DEF_HDCP2CIPHER_DBG == 1)
    printf("Ks^LC128:\r\n");
    for ( cnt = 0; cnt < SIZE_OF_KSXORLC128; cnt++ )
    {
        printf("0x%02X ", *(pu8KsXORLC128 + cnt));
    }
    printf("\r\n");

    printf("Riv:\r\n");
    for ( cnt = 0; cnt < SIZE_OF_RIV; cnt++ )
    {
        printf("0x%02X ", *(pu8Riv + cnt));
    }
    printf("\r\n");
#endif

#undef DEF_HDCP2CIPHER_DBG
    //MHal_HDMITx_Mask_Write(0x172F00, 0x01, 0x0020, 0x0020); //reverse order of cihper key
    //MHal_HDMITx_Mask_Write(0x172B00, 0x01, 0x8000, 0x0000); //disable hdcp 1.4 module

    for ( cnt = 0; cnt < (SIZE_OF_KSXORLC128>>1); cnt++)
        MHal_HDMITx_Write(HDMITX_SECUTZPC_BASE, 0x60 + (SIZE_OF_KSXORLC128 >> 1) - 1 - cnt, *(pu8KsXORLC128 + cnt*2 + 1)|(*(pu8KsXORLC128 + cnt*2)<<8));

    for ( cnt = 0; cnt < (SIZE_OF_RIV>>1); cnt++)
        MHal_HDMITx_Write(HDMITX_SECUTZPC_BASE, 0x68 + (SIZE_OF_RIV >> 1) - 1 - cnt, *(pu8Riv + cnt*2 + 1)|(*(pu8Riv + cnt*2)<<8));
}

void Mhal_HDMITx_SetSCDCCapability(MS_BOOL bFlag)
{
    g_bSupportSCDC = bFlag;
}

void MHal_HDMITx_SetChipVersion(MS_U8 u8ChipVer)
{
    gu8ChipVerNum = u8ChipVer;
}

// HPD: GPIO_PM[11] -> external interrupt[11], register 0x000E00[14]
// DVI disconnet: must power down clock termination resistor: TM_REG[0] = 1, TM_REG[16:15] = 00, TM_REG[35:34] = 00.
// Interrupt helper functoins
//------------------------------------------------------------------------------
/// @brief Disable interrupt
/// @param[in] u32Int interrupter value
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_Int_Disable(MS_U32 u32Int)
{
    //MS_U16 u16reg_val = 0;

    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_0C, (MS_U16)u32Int, (MS_U16)u32Int);
    // [9]: mask FIQ, [8]: mask IRQ
    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_STATUS_0D, (MS_U16)(u32Int>>16), (MS_U16)(u32Int>>16) );

#if 0 // K1 HPD pin doesn't have interrupt function
    if(_gHPDGpioPin != 0xC4) // HPD != I2S_GPIO4
    {
        u16reg_val = (_gHPDGpioPin > 10) ? (1<<(_gHPDGpioPin+4)) : (1<<_gHPDGpioPin);
        if(u32Int & E_HDMITX_IRQ_12) // HPD IRQ is move to PM_Sleep bank
        {
            MHal_HDMITxPM_Mask_Write(PMBK_PMSLEEP_REG_BASE, 0x00, u16reg_val, u16reg_val); // GPIO_PM mask
        }
    }
#endif
}

//------------------------------------------------------------------------------
/// @brief Enable interrupt
/// @param[in] u32Int interrupter value
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_Int_Enable(MS_U32 u32Int)
{
    //MS_U16 u16reg_val = 0;

    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_0C, 0xFFFF, ~u32Int);
    // [9]: mask FIQ, [8]: unmask IRQ
    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_STATUS_0D, 0xFFFF, (~u32Int)>>16 );

#if 0 // K1 HPD pin doesn't have interrupt function
    if(_gHPDGpioPin != 0xC4) // HPD != I2S_GPIO4
    {
        u16reg_val = (_gHPDGpioPin > 10) ? (1<<(_gHPDGpioPin+4)) : (1<<_gHPDGpioPin);
        if(u32Int & E_HDMITX_IRQ_12)
        {
            MHal_HDMITxPM_Mask_Write(PMBK_PMSLEEP_REG_BASE, 0x0F, 1<<_gHPDGpioPin, 1<<_gHPDGpioPin); // GPIO_PM output disable
            MHal_HDMITxPM_Mask_Write(PMBK_PMSLEEP_REG_BASE, 0x00, u16reg_val, 0); // GPIO_PM non-mask
        }
    }
#endif
}

//------------------------------------------------------------------------------
/// @brief MHal_HDMITx_Int_Clear
/// @param[in] u32Int interrupter value
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_Int_Clear(MS_U32 u32Int)
{
    //MS_U16 u16reg_val = 0;

    MHal_HDMITx_Write(HDMITX_MISC_REG_BASE, REG_MISC_STATUS_0E, u32Int);
    MHal_HDMITx_Write(HDMITX_MISC_REG_BASE, REG_MISC_STATUS_0F, (u32Int>>16));

#if 0 // K1 HPD pin doesn't have interrupt function
    if(_gHPDGpioPin != 0xC4) // HPD != I2S_GPIO4
    {
        u16reg_val = (_gHPDGpioPin > 10) ? (1<<(_gHPDGpioPin+4)) : (1<<_gHPDGpioPin);
        if(u32Int & E_HDMITX_IRQ_12)
        {
            MHal_HDMITxPM_Mask_Write(PMBK_PMSLEEP_REG_BASE, 0x04, u16reg_val, u16reg_val);
            MHal_HDMITxPM_Mask_Write(PMBK_PMSLEEP_REG_BASE, 0x04, u16reg_val, 0);
        }
    }
#endif
}

//------------------------------------------------------------------------------
/// @brief MHal_HDMITx_Int_Status
/// @param[in] u32Int interrupter value
/// @return None
//------------------------------------------------------------------------------
MS_U32 MHal_HDMITx_Int_Status(void)
{
    //MS_U16 u16reg_val = 0;
    MS_U32 reg_value=0;

    reg_value |= MHal_HDMITx_Read(HDMITX_MISC_REG_BASE, REG_MISC_STATUS_0E);
    reg_value |= (MHal_HDMITx_Read(HDMITX_MISC_REG_BASE, REG_MISC_STATUS_0F)<<16);

#if 0 // K1 HPD pin doesn't have interrupt function
    if(_gHPDGpioPin != 0xC4) // HPD = I2S_GPIO4
    {
        u16reg_val = (_gHPDGpioPin > 10) ? (1<<(_gHPDGpioPin+4)) : (1<<_gHPDGpioPin);
        reg_value = ((MHal_HDMITxPM_Read(PMBK_PMSLEEP_REG_BASE, 0x0A) & u16reg_val) ? (reg_value|E_HDMITX_IRQ_12):(reg_value&(~E_HDMITX_IRQ_12)));
    }
#endif
    return reg_value;
}


//------------------------------------------------------------------------------
/// @brief This routine is to get HDMI receiver DVI clock and HPD status.
/// @return MsHDMITX_RX_STATUS
//------------------------------------------------------------------------------
MsHDMITX_RX_STATUS MHal_HDMITx_GetRXStatus(void)
{
    MS_BOOL dviclock_s, hpd_s = FALSE;
    MsHDMITX_RX_STATUS state;

    dviclock_s = MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3F) & BIT0 ? FALSE : TRUE;

    hpd_s = mdrv_gpio_get_level(_gHPDGpioPin);

    if((dviclock_s == FALSE) && (hpd_s == FALSE))
        state = E_HDMITX_DVIClock_L_HPD_L;
    else if((dviclock_s == FALSE) && (hpd_s == TRUE))
        state = E_HDMITX_DVIClock_L_HPD_H;
    else if((dviclock_s == TRUE) && (hpd_s == FALSE))
        state = E_HDMITX_DVIClock_H_HPD_L;
    else
        state = E_HDMITX_DVIClock_H_HPD_H;

    return state;
}


//------------------------------------------------------------------------------
/// @brief MHal_HDMITX_SetHDCPConfig
/// @param[in] u32Int HDCP mode
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITX_SetHDCPConfig(MS_U8 HDCP_mode)
{
    MHal_HDMITx_Mask_Write(HDMITX_HDCP_REG_BASE, REG_HDCP_TX_MODE_01, 0x0E00, HDCP_mode << 8);
}


//------------------------------------------------------------------------------
/// @brief MHal_HDMITX_GetM02Bytes
/// @param[in] u16Int index
/// @return M0 2 bytes
//------------------------------------------------------------------------------
MS_U16 MHal_HDMITX_GetM02Bytes(MS_U16 idx)
{
    return (MHal_HDMITx_Read(HDMITX_HDCP_REG_BASE, REG_HDCP_TX_MI_0C + idx));
}


//------------------------------------------------------------------------------
/// @brief MHal_HDMITx_InitSeq
/// @param[in] None
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_InitSeq(void)
{
    MHal_HDMITx_Write(CLKGEN1_REG_BASE, REG_CKG_HDMITx_CLK_28, 0); // enable clk_hdmi_tx_p
    MHal_HDMITx_Mask_Write(CLKGEN0_REG_BASE, REG_CKG0_ODCLK_SOURCE, 0x0010, 0x0010); //ODCLK source from HDMITx, MST253B Only

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_40, 0x0001, 0x0001);//MST253B Only
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3D, 0x0400, 0x0400);//MST253B Only
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_01, 0x0001, 0x0001);//MST253B Only
    //MHal_HDMITx_Mask_Write(0x000E00U, 0x72U, 0x1000, 0x1000);//MST253B Only for HPD
    //MHal_HDMITx_Mask_Write(0x101E00U, 0x6EU, 0x0004, 0x0004);//MST253B Only for DDC

    //set at mboot
    //MHal_HDMITx_Mask_Write(0x101E00, 0x0B, BIT12, BIT12); //enable hdmitx DDC

    //MAC setting
    MHal_HDMITx_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1C, 0x0000);
    MHal_HDMITx_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1D, 0x0000);
    MHal_HDMITx_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1E, 0xFFFF);
    MHal_HDMITx_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1F, 0x0000);

    //sw reset modules
    if(MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E) & 0xE800)
    {
        //MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E, 0xEA00, 0x0200);
    }
    else
    {
        //disable power down
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_39, 0xFFFF, 0xFFFF);
        //[9]:pixel clock [11]:tmds clock [15..13]:enable data out
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E, 0xEA00, 0x0200);
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_00, 0x0017, 0x0017);
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_00, 0x0017, 0x0000);
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_0F, 0x001F, 0x001F);
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_0F, 0x001F, 0x0000);
    }

    //MHal_HDMITx_Write(HDMITX_REG_BASE, REG_HPLL_LOCK_CNT_53, 0x0300); // HPLL lock counter

    //enable PHY setting
    //[0]:enable synth clock; [4]:enable synth clock to a top; [8]:enable tmds clock; [12]: enable atop 40 bit clock
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_10, 0x1101, 0x1101);
    //[9]:pixel clock [11]:tmds clock [15..13]:enable data out
    //MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E, 0xEA00, 0xEA00);

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3F, 0x0010, 0x0010); //reg_atop_nodie_en_disc

    //disable power down
    //MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_39, 0xFFFF, 0xF000);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3A, 0x0070, 0x0000);

    //rterm set by Mboot according to data of EFUSE, rterm 50 Ohm
    //MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_38, 0x01F0, 0x0000);

    //txpll input div 1
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3C, 0x03F3, 0x0051);

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3D, 0x030F, 0x0003); //REG_TXPLL_SEL_CLKIN, REG_TXPLL_ICP_ICTRL
    //MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_34, 0x003F, 0x0003); //REG_ICTRL_PREDRV_MAIN_CLK
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_34, 0x003F, 0x0000); //REG_ICTRL_PREDRV_MAIN_CLK
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_30, 0x003F, 0x0010); //REG_ICTRL_DRV_MAIN_CLK

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_32, 0x3F3F, 0x0000);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_36, 0x3F3F, 0x0000);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_37, 0x3F3F, 0x0000);

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, BIT0, 0x0000); //tmds clock div 2;

    //lane fifo setting
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_16, 0x0007, 0x0004);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_17, 0x0007, 0x0005);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_18, 0x0007, 0x0006);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_19, 0x0007, 0x0007);

    //timing field regen enable (due to scaler can't guarantee field signal and even/odd frame is asynchronize)
    MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_00, 0x0010, 0x0010);

    //for U02: QD980 CTS compatibility issue
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x8000, 0x8000);

}

//------------------------------------------------------------------------------
/// @brief This routine is the initialization for Video module.
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_VideoInit(void)
{
    MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_01, BIT4, BIT4); //enable video engine fifo r/w pointer
    //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_12, 0x000F, BIT3); // [3]: manual mode of pixel-repetition enable
}

//------------------------------------------------------------------------------
/// @brief This routine is the initialization for Audio module.
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_AudioInit(void)
{
    int num;

    num = sizeof(HDMITxAudioInitTbl) / sizeof(MSTHDMITX_REG_TYPE);
    MHal_HDMITx_RegsTbl_Write(HDMITxAudioInitTbl, num);
}


//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI PLL
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_PLLOnOff(MS_BOOL bflag)
{
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3A, 0x0070, bflag ? 0x0000 : 0x0010);
}


void MHal_HDMITx_PKT_User_Define_Clear(void)
{
    MS_U8 i = 0;

    for ( i = 0; i < GENERAL_PKT_NUM; i++ )
    {
        gbGeneralPktList[i].EnableUserDef = FALSE;
        gbGeneralPktList[i].FrmCntNum = 0x00;
        gbGeneralPktList[i].enPktCtrl = E_HDMITX_STOP_PACKET;
        memset(&gbGeneralPktList[i].PktPara, 0x00, sizeof(gbGeneralPktList[i].PktPara));
    }

    for ( i = 0; i < INFOFRM_PKT_NUM; i++ )
    {
        gbInfoFrmPktList[i].EnableUserDef = FALSE;
        gbInfoFrmPktList[i].FrmCntNum = 0x00;
        gbInfoFrmPktList[i].enPktCtrl = E_HDMITX_STOP_PACKET;
        memset(&gbInfoFrmPktList[i].PktPara, 0x00, sizeof(gbInfoFrmPktList[i].PktPara));
    }
}

void MHal_HDMITx_PKT_User_Define(MsHDMITX_PACKET_TYPE packet_type, MS_BOOL def_flag,
        MsHDMITX_PACKET_PROCESS def_process, MS_U8 def_fcnt)
{
    if (packet_type & 0x80) //infoframe packet type
	{
		gbInfoFrmPktList[packet_type & (~0x80)].EnableUserDef = def_flag;
		gbInfoFrmPktList[packet_type & (~0x80)].FrmCntNum = def_fcnt;
		gbInfoFrmPktList[packet_type & (~0x80)].enPktCtrl = def_process;
	}
	else
	{
		gbGeneralPktList[packet_type].EnableUserDef = def_flag;
		gbGeneralPktList[packet_type].FrmCntNum = def_fcnt;
		gbGeneralPktList[packet_type].enPktCtrl = def_process;
	}
}

MS_BOOL MHal_HDMITx_PKT_Content_Define(MsHDMITX_PACKET_TYPE packet_type, MS_U8* data, MS_U8 length)
{
    MS_BOOL bRet = TRUE;
    MS_U8 i, j, *ptr;
	ptr = data;

    switch(packet_type)
	{
	    case E_HDMITX_VS_INFOFRAME:
            for (i=0; i < length; i++)
            {
                j = i>>1;

                if ((REG_PKT_VS_1_27+j > REG_PKT_VS_14_34) || ((REG_PKT_VS_1_27+j == REG_PKT_VS_14_34) && (i % 2 == 1)))
                {
                    //Packet over size, last VS packet PB register is REG_PKT_VS_14_34[7:0]
                    DBG_HDMITX(printf("VS packet over size, length = %d \n", length));
                    break;
                }

                if((i%2)==0)
			        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_1_27+j, 0x00FF, *(ptr+i));
				else
			        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_1_27+j, 0xFF00, (*(ptr+i))<<8);
            }
		break;
	    case E_HDMITX_SPD_INFOFRAME:
            for(i=0;i<length;i++)
            {
                j = i>>1;

                if((REG_PKT_SPD_1_15+j > REG_PKT_SPD_13_21) || ((REG_PKT_SPD_1_15+j == REG_PKT_SPD_13_21) && (i % 2 == 1)))
                {
                    //Packet over size, last SPD packet PB register is REG_PKT_SPD_13_21[7:0]
                    DBG_HDMITX(printf("VS packet over size, length = %d \n", length));
                    break;
                }

                if((i%2)==0)
			        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_SPD_1_15+j, 0x00FF, *(ptr+i));
				else
			        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_SPD_1_15+j, 0xFF00, (*(ptr+i))<<8);
            }
		break;

        case E_HDMITX_AVI_INFOFRAME:
            for ( i = 0; i < length; i++ )
            {
                j = i >> 1;

                if (((REG_PKT_AVI_1_09 + j) > REG_PKT_AVI_7_0F) || (((REG_PKT_AVI_1_09 + j) == REG_PKT_AVI_7_0F) && (i % 2 == 1)))
                {
                    DBG_HDMITX(printf("AVI packet over size, length = %d \n", length));
                    break;
                }

                if ((i % 2) == 0)
                    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09 + j, 0x00FF, *(ptr+i));
                else
                    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09 + j, 0xFF00, *(ptr+i) << 8);
            }
        break;

        //wilson@kano HDR packet
        case E_HDMITX_HDR_INFOFRAME:
            //first 3 bytes will follow spec, (1) type code (2) version (3) length, and reamins will be content
            length = (length > (HDMITX_HDR_INFO_PKT_LEN + 3)) ? (HDMITX_HDR_INFO_PKT_LEN + 3) : length;
            //fill length and version
            MHal_HDMITx_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1F,(*(ptr+2) << 8) | *(ptr+1));

	    if (length >= 3)
            {
                for ( i = 0; i < (length-3); i++ )
                {
                    j = i >> 1;

                    if ( ((REG_HDMI_2_CONFIG_10 + j) > REG_HDMI_2_CONFIG_1D) || (((REG_HDMI_2_CONFIG_10 + j) == REG_HDMI_2_CONFIG_1D) && (i % 2 == 1)) )
                    {
                        DBG_HDMITX(printf("HDR packet over size, length = %d \n", length));
                        break;
                    }

                    if ((i % 2) == 0)
                    {
                        MHal_HDMITx_Mask_Write(HDMITX_2_REG_BASE, (REG_HDMI_2_CONFIG_10 + j), 0x00FF, *(ptr+i+3));
                    }
                    else
                    {
                        MHal_HDMITx_Mask_Write(HDMITX_2_REG_BASE, (REG_HDMI_2_CONFIG_10 + j), 0xFF00, (*(ptr+i+3))<<8);
                    }
                }
            }
        break;

        case E_HDMITX_AUDIO_INFOFRAME:
            {
                //total length should be 192 bits *2 = 384 bits = 48 bytes;

                length = (length < ((192>>3)<<1) ) ? length : ((192>>3)<<1);

                for ( i = 0; i < length; i++ )
                {
                    j = i >> 1;

                    if ((i % 2) == 0)
                    {
                        MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS0_0A + j, 0x00FF, *(ptr+i));
                    }
                    else
                    {
                        MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS0_0A + j, 0xFF00, (*(ptr+i) << 8));
                    }
                }
            }
        break;

	    default:
		    i = 0;
			j = 0;
            bRet = FALSE;
            DBG_HDMITX(printf("Not implemented, packet type = %u\n", packet_type));
	    break;
    }

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx TMDS signal
/// @param[in] bRB_Swap: R/B swap; bTMDS: TMDS flag
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetTMDSOnOff(MS_BOOL bRB_Swap, MS_BOOL bTMDS)
{
    if(bRB_Swap) // R/B channel swap
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_00, BIT13, BIT13);
    else
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_00, BIT13, 0);

    //reg_atop_en_data_out[13..15]:  Enable data channel data output
    if (bTMDS == TRUE)
    {
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_39, 0xFFFF, 0xF000);
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E, 0xE800, 0xE800);
        //TBD: turn off tmds clock

        MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, 0x0001, 0x0001); //flush audio fifo
        MsOS_DelayTask(10);
        MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, 0x0001, 0x0000);
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_39, 0xFFFF, 0xFFFF);
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E, 0xE800, 0x0000);
        //TBD: turn off tmds clock
    }
}

//------------------------------------------------------------------------------
/// @brief This routine return on/off status of HDMI Tx TMDS signal
/// @return On/Off
//------------------------------------------------------------------------------
MS_U32 MHal_HDMITx_GetTMDSStatus(void)
{
    MS_U16 usTMDSOutput = 0;
    MS_U16 usTMDSPD = 0;
    MS_U32 uiTMDSOn = 0;
    
    usTMDSOutput = MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E);
    usTMDSPD = MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_39);
    
    if( ((usTMDSOutput&0xE800) == 0xE800) && ((usTMDSPD &0x0FFF) == 0x0000) )
    {
        uiTMDSOn = TRUE;
    }
    else
    {
        uiTMDSOn = FALSE;
    }

    return uiTMDSOn;
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx video output
/// @param[in] bVideo: Video flag; bCSC: CSC flag, b709format = BT.709-5
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetVideoOnOff(MS_BOOL bVideo, MS_BOOL bCSC, MS_BOOL b709format)
{
    if (bVideo == TRUE)
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, BIT0, 0x0000); // disable test pattern
    }
    else
    {
        MS_U8 i = 0x00;

        DBG_HDMITX(printf("MDrv_HDMITx_SetVideoOnOff: csc flag= %d \n", bCSC));

        // enable test pattern
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_3A, 0x0000);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_3B, 0x0000);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_3C, 0x03FF); // whole-blue
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, BIT0|BIT1, BIT0|BIT1);

        for ( i = 0; i < 6; i++ )
            MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_3D + i, 0x0000);
    }
}

//------------------------------------------------------------------------------
/// @brief This routine sets video color formatt
/// @param[in] bCSC: CSC flag, YUV422 12 bit, b709format = BT.709-5
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetColorFormat(MS_BOOL bCSC, MS_BOOL bHdmi422b12, MS_BOOL b709format)
{
    if (bCSC) // YUV -> RGB
    {
        DBG_HDMITX(printf("YUV -> RGB \n"));
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, BIT0, BIT0); //bypass
    }
    else // bypass
    {
        DBG_HDMITX(printf("bypass YUV -> RGB \n"));
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, BIT0, 0x00); //bypass
    }

    // YUV422 12 bits output
    MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_17, BIT15, bHdmi422b12 ? BIT15 : 0x00);
}

//------------------------------------------------------------------------------
/// @brief This routine get CSC capability
/// @param[in]
/// @return False : not support R2Y. Ture : ok.
//------------------------------------------------------------------------------
MS_BOOL MHal_HDMITx_CSC_Support_R2Y(void* pDatatIn)
{
    pDatatIn = pDatatIn;
    return HDMITX_CSC_SUPPORT_R2Y;
}

//------------------------------------------------------------------------------
/// @brief This routine sets color domain and color range transform
/// @param[in]
/// @return False : not support this command. Ture : ok.
//------------------------------------------------------------------------------
MS_BOOL MHal_HDMITx_ColorandRange_Transform(MsHDMITX_VIDEO_COLOR_FORMAT incolor, MsHDMITX_VIDEO_COLOR_FORMAT outcolor, MsHDMITX_YCC_QUANT_RANGE inrange, MsHDMITX_YCC_QUANT_RANGE outrange)
{
    MS_BOOL bReturn = FALSE;

    if( (incolor == outcolor) && (inrange == outrange) )
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
        bReturn = TRUE;
        DBG_HDMITX(printf("Input equals output \n"));
    }
    else if(incolor == E_HDMITX_VIDEO_COLOR_RGB444)
    {
        if(inrange == E_HDMITX_YCC_QUANT_LIMIT)//LR
        {
            if(((outcolor == E_HDMITX_VIDEO_COLOR_YUV444)||(outcolor == E_HDMITX_VIDEO_COLOR_YUV422)) && (outrange == E_HDMITX_YCC_QUANT_LIMIT))
            { //LR -> LY
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x00A0);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x020C);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x1E24);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x1FD0);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x00DA);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x02DC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x004A);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x1F88);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x1E6C);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x020C);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("LR->LY! \n"));
            }
            else if(((outcolor == E_HDMITX_VIDEO_COLOR_YUV444)||(outcolor == E_HDMITX_VIDEO_COLOR_YUV422)) && (outrange == E_HDMITX_YCC_QUANT_FULL))
            { //LR -> FY
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x0CB0);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x0256);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x1DE1);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x1FC9);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x00FE);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x0357);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x0056);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x1F77);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x1E33);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x0256);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("LR->FY! \n"));
            }
            else if((outcolor == E_HDMITX_VIDEO_COLOR_RGB444)&& (outrange == E_HDMITX_YCC_QUANT_FULL))
            { //LR -> FR
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x0C10);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("LR->FR! \n"));
            }
            else
            {
                DBG_HDMITX(printf("LR->LR! \n"));
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
                DBG_HDMITX(printf("In/Out Same Color! \n"));
                bReturn = TRUE;
            }

        }
        else if(inrange == E_HDMITX_YCC_QUANT_FULL)//FR
        {
            if(((outcolor == E_HDMITX_VIDEO_COLOR_YUV444)||(outcolor == E_HDMITX_VIDEO_COLOR_YUV422)) && (outrange == E_HDMITX_YCC_QUANT_LIMIT))
            { //FR -> LY
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x00E0);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x01C0);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x1E69);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x1FD7);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x00BA);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x0273);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x003F);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x1F99);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x1EA6);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x01C0);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("FR->LY! \n"));
            }
            else if(((outcolor == E_HDMITX_VIDEO_COLOR_YUV444)||(outcolor == E_HDMITX_VIDEO_COLOR_YUV422)) && (outrange == E_HDMITX_YCC_QUANT_FULL))
            { //FR -> FY
                //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x00A1);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x00A0);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x0200);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x1E2F);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x1FD1);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x00DA);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x02DC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x004A);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x1F8B);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x1E75);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x0200);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("FR->FY! \n"));
            }
            else if((outcolor == E_HDMITX_VIDEO_COLOR_RGB444)&& (outrange == E_HDMITX_YCC_QUANT_LIMIT))
            { //FR -> LR
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x5040);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x0381);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x036D);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x0381);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("FR->LR! \n"));
            }
            else
            {
                DBG_HDMITX(printf("FR->FR! \n"));
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
                DBG_HDMITX(printf("In/Out Same Color! \n"));
                bReturn = TRUE;
            }
        }
        else
        {
            //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
            DBG_HDMITX(printf("Not Support This! \n"));
        }
    }
    else if((incolor == E_HDMITX_VIDEO_COLOR_YUV444) || (incolor == E_HDMITX_VIDEO_COLOR_YUV422))
    {
        if(inrange == E_HDMITX_YCC_QUANT_LIMIT)//LY
        {
            if((outcolor == E_HDMITX_VIDEO_COLOR_RGB444) && (outrange == E_HDMITX_YCC_QUANT_FULL))
            {//LY->FR
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x001A);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x0731);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x1DDD);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x1F25);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x0879);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("LY->FR! \n"));
            }
            else if((outcolor == E_HDMITX_VIDEO_COLOR_RGB444) && (outrange == E_HDMITX_YCC_QUANT_LIMIT))
            {//LY->LR
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x000A);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x0629);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x0400);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x1E2B);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x0400);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x1F44);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x0400);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x0742);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("May over range! \n"));
                DBG_HDMITX(printf("LY->LR! \n"));
            }
            else if(((outcolor == E_HDMITX_VIDEO_COLOR_YUV444)||(outcolor == E_HDMITX_VIDEO_COLOR_YUV422)) && (outrange == E_HDMITX_YCC_QUANT_FULL))
            {//LY->FY
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x00BA);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x0491);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x0491);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("LY->FY! \n"));
            }
            else
            {
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
                DBG_HDMITX(printf("In/Out Same Color! \n"));
                DBG_HDMITX(printf("LY->LY! \n"));
                bReturn = TRUE;
            }

        }
        else if(inrange == E_HDMITX_YCC_QUANT_FULL)//FY
        {
            if((outcolor == E_HDMITX_VIDEO_COLOR_RGB444) && (outrange == E_HDMITX_YCC_QUANT_FULL))
            {//FY->FR
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x000A);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x064D);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x0400);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x1E21);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x0400);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x1F40);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x0400);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x076C);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("FY->FR! \n"));
            }
            else if((outcolor == E_HDMITX_VIDEO_COLOR_RGB444) && (outrange == E_HDMITX_YCC_QUANT_LIMIT))
            {//FY->LR
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x504A);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x0565);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x036D);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x1E66);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x036D);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x1F5C);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x036D);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x065B);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("FY->LR! \n"));
            }
            else if(((outcolor == E_HDMITX_VIDEO_COLOR_YUV444)||(outcolor == E_HDMITX_VIDEO_COLOR_YUV422)) && (outrange == E_HDMITX_YCC_QUANT_LIMIT))
            {//FY->LY
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x7FFF, 0x00EA);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_21, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_22, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_23, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_24, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_25, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_26, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_27, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_28, 0x1FFF, 0x0000);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_29, 0x1FFF, 0x04AC);
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0001);
                bReturn = TRUE;
                DBG_HDMITX(printf("FY->LY! \n"));
            }
            else
            {
                MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
                DBG_HDMITX(printf("In/Out Same Color! \n"));
                DBG_HDMITX(printf("FY->FY! \n"));
                bReturn = TRUE;
            }
        }
        else
        {
            //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
            DBG_HDMITX(printf("Not Support This! \n"));
        }
    }
    else
    {
        //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_20, 0x0001, 0x0000);
        DBG_HDMITX(printf("Not Support This! \n"));
    }

    //if(bReturn == TRUE)
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_17, BIT15, (outcolor == E_HDMITX_VIDEO_COLOR_YUV422) ? BIT15 : 0);

    return bReturn;
}

//------------------------------------------------------------------------------
/// @brief This routine will set or stop all HDMI packet generation
/// @param[in] bflag  True: Enable packet gen, False : Disable packet gen
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_EnablePacketGen(MS_BOOL bflag)
{
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_HDMI_CONFIG1_00, BIT2, bflag ? BIT2:0);
}

//------------------------------------------------------------------------------
/// @brief This routine sets HDMI/DVI mode
/// @param[in] bflag
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetHDMImode(MS_BOOL bflag, MsHDMITX_VIDEO_COLORDEPTH_VAL cd_val)
{
    MS_U8 ucRegVal = 0x00;

    DBG_HDMITX(printf("[%s][%d]HDMI mode = %d, Color Depth = %d \n", __FUNCTION__, __LINE__, bflag, bflag));

    if (bflag) // HDMI mode
    {
        switch(cd_val)
        {
            case E_HDMITX_VIDEO_CD_NoID:
            case E_HDMITX_VIDEO_CD_24Bits:
            default:
                ucRegVal = 0x00;
            break;

            case E_HDMITX_VIDEO_CD_30Bits:
                ucRegVal = 0x40;
            break;

            case E_HDMITX_VIDEO_CD_36Bits:
                ucRegVal = 0x80;
            break;

            case E_HDMITX_VIDEO_CD_48Bits:
                ucRegVal = 0xC0;
            break;
        }

        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_HDMI_CONFIG1_00, 0x00FF, ucRegVal|BIT2); // [7:6]: DC_mode, [2]: packet enable, [0]: HDMI/DVI
    }
    else // DVI
    {
        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_HDMI_CONFIG1_00, 0x00FF, BIT0); // [7:6]: DC_mode, [2]: packet enable, [0]: HDMI/DVI
    }
}


//------------------------------------------------------------------------------
/// @brief This routine sets audio on/off
/// @param[in] bflag
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetAudioOnOff(MS_BOOL bflag)
{
    MS_U8 num;

    if(bflag) // audio on
    {
        num = sizeof(HDMITxAudioOnTbl)/sizeof(MSTHDMITX_REG_TYPE);
        MHal_HDMITx_RegsTbl_Write(HDMITxAudioOnTbl, num);
    }
    else // audio off
    {
        num = sizeof(HDMITxAudioOffTbl)/sizeof(MSTHDMITX_REG_TYPE);
        MHal_HDMITx_RegsTbl_Write(HDMITxAudioOffTbl, num);
    }
}

//------------------------------------------------------------------------------
/// @brief This routine sets audio sampling freq.
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetAudioFrequency(MsHDMITX_AUDIO_FREQUENCY afidx,
        MsHDMITX_AUDIO_CHANNEL_COUNT achidx, MsHDMITX_AUDIO_CODING_TYPE actidx
)
{
    // HDMI audio channel setting
    if(achidx == E_HDMITX_AUDIO_CH_2) // 2 channels
    {
        MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, BIT4|BIT5|BIT6, BIT5|BIT6); //[6:5]: audio FIFO depth ch1234, [4]=1'b0: 2 channels
    }
    else // 8 channels
    {
        MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, BIT4, BIT4); //[4]=1'b1: 8 channels
    }

    // Audio channel status
    #if 1 //NOTE:: kano: move channel status from address 0x00 to  address 0x0A
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS0_0A, ((actidx == E_HDMITX_AUDIO_PCM) ? 0 : BIT1) ); // [1]: PCM / non-PCM
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS1_0B, (TxAudioFreqTbl[afidx].CH_Status3 << 8) | (achidx << 4)); //[11:8]: audio sampling frequncy; [7:4]: audio channel count
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS2_0C, 0x0000);
    #else
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS0_00, ((actidx == E_HDMITX_AUDIO_PCM) ? 0 : BIT1) ); // [1]: PCM / non-PCM
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS1_01, 0);
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS2_02, (achidx<<4)); // [7:4]: audio channel count
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS3_03, TxAudioFreqTbl[afidx].CH_Status3); // [3:0]: audio sampling frequncy
    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS4_04, 0);
    #endif
    // ACR N code
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACR_2_06, 0x0F00, (TxAudioFreqTbl[afidx].NcodeValue & 0x0F0000));
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACR_3_07, 0xFFFF, (TxAudioFreqTbl[afidx].NcodeValue & 0x00FFFF));

    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_N_PKT_61, 0x000F, (TxAudioFreqTbl[afidx].NcodeValue & 0x0F0000) >> 8);
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_N_PKT_60, 0xFFFF, (TxAudioFreqTbl[afidx].NcodeValue & 0x00FFFF));
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACR_CFG_08, 0x0100, 0x0100); //enable cts * 2
}

//------------------------------------------------------------------------------
/// @brief This routine sets audio source format.
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetAudioSourceFormat(MsHDMITX_AUDIO_SOURCE_FORMAT fmt)
{
    MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, BIT9|BIT8, fmt << 8);
}

//------------------------------------------------------------------------------
/// @brief This routine Get Audio CTS value
/// @return CTS
//------------------------------------------------------------------------------
MS_U32 MHal_HDMITx_GetAudioCTS(void)
{
    MS_U32 ret;

    ret = ((MHal_HDMITx_Read(HDMITX_REG_BASE, REG_PKT_ACR_2_06) & 0x000F) << 16) | MHal_HDMITx_Read(HDMITX_REG_BASE, REG_PKT_ACR_1_05);
    return ret;
}

//------------------------------------------------------------------------------
/// @brief This routine Mute Audio FIFO
/// @param[in] bflag: True: mute audio, False: unmute audio
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_MuteAudioFIFO(MS_BOOL bflag)
{
    MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, 0x0001, (MS_U16)bflag);
}

//------------------------------------------------------------------------------
/// @brief This routine sets HDMI Tx HDCP encryption On/Off
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetHDCPOnOff(MS_BOOL hdcp_flag, MS_BOOL hdmi_flag)
{
    if(hdcp_flag) // HDCP on
    {
        if (hdmi_flag) // HDMI EESS
            MHal_HDMITx_Mask_Write(HDMITX_HDCP_REG_BASE, REG_HDCP_TX_MODE_01, 0x0C00, 0x0400);
        else // DVI OESS
            MHal_HDMITx_Mask_Write(HDMITX_HDCP_REG_BASE, REG_HDCP_TX_MODE_01, 0x0C00, 0x0000);
        // HDCP encryption
        MHal_HDMITx_Mask_Write(HDMITX_HDCP_REG_BASE, REG_HDCP_TX_COMMAND_02, 0x0008, 0x0008);
    }
    else // HDCP off
        MHal_HDMITx_Mask_Write(HDMITX_HDCP_REG_BASE, REG_HDCP_TX_COMMAND_02, 0x0008, 0x0000);
}

//------------------------------------------------------------------------------
/// @brief This routine calculate check sum of infoframes.
/// @param[in] packet_type packet type
/// @return checksum
//------------------------------------------------------------------------------
MS_U8 MHal_HDMITx_InfoFrameCheckSum(MsHDMITX_PACKET_TYPE packet_type)
{
    MS_U8 ucSumVal = 0;
    MS_U8 i = 0;
    MS_U8 j = 0;
    MS_U16 wRegVal = 0;

    switch (packet_type)
    {
        case E_HDMITX_VS_INFOFRAME:
            ucSumVal += (E_HDMITX_VS_INFOFRAME + HDMITX_VS_INFO_PKT_VER + HDMITX_VS_INFO_PKT_LEN);

            for ( i = 0; i < (HDMITX_VS_INFO_PKT_LEN + 1) >> 1; i++ )
            {
                wRegVal = MHal_HDMITx_Read(HDMITX_REG_BASE, REG_PKT_VS_1_27 + i);

                for ( j = 0; j < 2; j++ )
                {
                    if ((i == 13) && (j==1))
                    {
                        ucSumVal += 0x00;
                    }
                    else
                    {
                        ucSumVal += (j % 2 == 0) ? ((MS_U8)(wRegVal & 0x00FF)) : ((MS_U8)((wRegVal & 0xFF00) >> 8));
                    }
                }
            }
        break;

        case E_HDMITX_AVI_INFOFRAME:
            ucSumVal += (E_HDMITX_AVI_INFOFRAME + HDMITX_AVI_INFO_PKT_VER + HDMITX_AVI_INFO_PKT_LEN);

            for ( i = 0; i < ((HDMITX_AVI_INFO_PKT_LEN + 1) >> 1); i++ )
            {
                wRegVal = MHal_HDMITx_Read(HDMITX_REG_BASE, REG_PKT_AVI_1_09 + i);

                for ( j = 0; j < 2; j++ )
                {
                    if ((i == 1) && (j == 0)) // SC[1:0]
                    {
                        ucSumVal += (HDMITX_AVI_INFO_PKT_VER >= 0x02U) ? ((MS_U8)(wRegVal & 0x00FF)) : ((MS_U8)(wRegVal & 0x00FF) & 0x03);
                    }
                    else if ((i == 1) && (j == 1)) // VIC[6:0]
                    {
                        ucSumVal += ((HDMITX_AVI_INFO_PKT_VER >= 0x02U) ? ((MS_U8)((wRegVal & 0xFF00) >> 8) & 0x7F) : 0x00);
                    }
                    else if ((i == 2) && (j == 0)) // PR[3:0]
                    {
                        ucSumVal += (MS_U8)(wRegVal & 0x00FF); //bit 4:7 used for HDR; //ucSumVal += ((MS_U8)(wRegVal & 0x00FF) & 0x0F);
                    }
                    #if 0 //not resevered byte anymore; should be counted
                    else if ((i == 2) && (j == 1)) // reserved
                    {
                        ucSumVal += 0x00;
                    }
                    #endif
                    else if (( i == ((HDMITX_AVI_INFO_PKT_LEN + 1) >> 1) - 1)&&(j == 1))
                    {
                        ucSumVal += 0x00;
                    }
                    else
                    {
                        ucSumVal += (j % 2 == 0) ? ((MS_U8)(wRegVal & 0x00FF)) : ((MS_U8)((wRegVal & 0xFF00) >> 8));
                    }
                }
            }
        break;

        case E_HDMITX_SPD_INFOFRAME:
            ucSumVal += (E_HDMITX_SPD_INFOFRAME + HDMITX_SPD_INFO_PKT_VER + HDMITX_SPD_INFO_PKT_LEN);

            for ( i = 0; i < ((HDMITX_SPD_INFO_PKT_LEN+ 1) >> 1); i++ )
            {
                wRegVal = MHal_HDMITx_Read(HDMITX_REG_BASE, REG_PKT_SPD_1_15 + i);

                for ( j = 0; j < 2; j++ )
                {
                    if ((i == 12) && (j == 0))
                    {
                        ucSumVal += (MS_U8)(wRegVal & 0x00FF);
                    }
                    else if ((i == 12) && (j == 1)) //reserved
                    {
                        ucSumVal += 0x00;
                    }
                    else
                    {
                        ucSumVal += (j % 2 == 0) ? ((MS_U8)(wRegVal & 0x00FF)) : ((MS_U8)((wRegVal & 0xFF00) >> 8));
                    }
                }
            }
        break;

        case E_HDMITX_AUDIO_INFOFRAME:
            ucSumVal += (E_HDMITX_AUDIO_INFOFRAME + HDMITX_AUD_INFO_PKT_VER + HDMITX_AUD_INFO_PKT_LEN);

            for ( i = 0; i < (((HDMITX_AUD_INFO_PKT_LEN >> 1) + 1) >> 1); i++ )
            {
                wRegVal = MHal_HDMITx_Read(HDMITX_REG_BASE, REG_PKT_AUD_1_11 + i);

                if (i == 1)
                {
                    for ( j = 0; j < 2; j++ )
                        ucSumVal += (j % 2 == 0) ? ((MS_U8)(wRegVal & 0x00FF)) : ((MS_U8)((wRegVal & 0xFF00) >> 8));
                }

                if (i == 0)
                {
                    for ( j = 0; j < 2; j++ )
                        ucSumVal += ( (j % 2 == 0) ? ((MS_U8)(wRegVal & 0x00FF) & 0xF7) : (((MS_U8)(wRegVal & 0xFF00) >> 8) & 0x1F));
                }

                if (i == 2)
                {
                    for ( j = 0; j < 2; j++ )
                        ucSumVal +=  ((j % 2 == 0) ? ((MS_U8)(wRegVal & 0x00FF) & 0xFB) : 0x00);
                }
            }
        break;

        case E_HDMITX_MPEG_INFOFRAME:
            //TBD
        break;

        case E_HDMITX_HDR_INFOFRAME:
            {
                MS_U8 u8PktLen = 0x00;

                u8PktLen = (MHal_HDMITx_Read(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1F) & 0xFF00) >> 8;
                ucSumVal += (E_HDMITX_HDR_INFOFRAME + (MS_U8)(MHal_HDMITx_Read(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1F) & 0x00FF) + u8PktLen);

                for ( i = 0; i < ((u8PktLen + 1) >> 1); i++ )
                {
                    wRegVal = MHal_HDMITx_Read(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_10 + i);

                    for ( j = 0; j < 2; j++ )
                    {
                        if ((i == 0x0D) && (j % 2 == 0x00))
                            ucSumVal += (j % 2 == 0) ? (MS_U8)(wRegVal & 0x00FF) : 0x00;
                        else
                            ucSumVal += (j % 2 == 0) ? ((MS_U8)(wRegVal & 0x00FF)) : ((MS_U8)((wRegVal & 0xFF00) >> 8));
                    }
                }
            }
        break;

        default:
        break;
    }

    return (MS_U8)((~ucSumVal) + 0x01);

}

//------------------------------------------------------------------------------
/// @brief This routine sets video output mode (color/repetition/regen)
/// @param[in] idx: gHDMITxInfo.output_video_timing
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetVideoOutputMode(MsHDMITX_VIDEO_TIMING idx, MS_BOOL bflag, MsHDMITX_VIDEO_COLORDEPTH_VAL cd_val, MsHDMITX_ANALOG_TUNING *pInfo, MS_U8 ubSSCEn)
{
    //MS_U16 reg_value=0;
    MS_U8 ubTimingChanged = TRUE;
    MS_U8 ucCDIdx = 0;
    MS_U16 wHfront = 0;
    MS_U16 wVfront = 0;
    MS_BOOL bIsHDMI20 = FALSE;
    MS_BOOL bIs420Fmt = FALSE;
    MS_BOOL bIsRPMode = FALSE;
    MsHDMITX_VIDEO_TIMING eSize_idx = idx;
    MS_U32 uiTMDSCLK = 0;

//wilson@kano:TBD
    printf("video idx = 0x%X, color depth = 0x%X\r\n", idx, cd_val);
    // Deep color FIFO reset
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_HDMI_CONFIG1_00, BIT10, BIT10);
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_HDMI_CONFIG1_00, BIT10, 0);
    MsOS_DelayTask(10);



    ucCDIdx = (cd_val == 0)? 0x00 : (cd_val - 4);

    // Interlace mode
    #if (defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))
    if (HDMITxVideoModeTbl[idx].i_p_mode == E_HDMITX_VIDEO_INTERLACE_MODE)
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_2D, 0x0008, 0x0008);
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_00, 0x017F, 0x005F);
        //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_17, 0x4000, 0x4000); //reg_interlace_mode_sel
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, 0x0004, 0x0004); // PG interlace enable

        if(idx == E_HDMITX_RES_720x480i)
        {
            MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x0040, 0x0040); //video clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideo_OTMDS_ODATA_Ratio[ucCDIdx][idx] << 12);
        }
        else if(idx == E_HDMITX_RES_720x576i)
        {
            MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x0040, 0x0040); //video clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideo_OTMDS_ODATA_Ratio[ucCDIdx][idx] << 12);
        }
        else if(idx == E_HDMITX_RES_1920x1080i_50Hz)
        {
            MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x01C0); //video,pixel,tmds clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0001); //PHY tmds clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideo_OTMDS_ODATA_Ratio[ucCDIdx][idx] << 12);
        }
        else if(idx == E_HDMITX_RES_1920x1080i_60Hz)
        {
            MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x01C0); //video,pixel,tmds clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0001); //PHY tmds clock div 2
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideo_OTMDS_ODATA_Ratio[ucCDIdx][idx] << 12);
        }
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_2D, 0x0008, 0x0008);
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_00, 0x017F, 0x004E);
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_17, 0x4000, 0x0000); //reg_interlace_mode_sel
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, 0x0004, 0x0000); // PG interlace disable

        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x0000); //video clock div 2
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 12);
    }
    #else
    if (HDMITxVideoModeTbl[idx].i_p_mode == E_HDMITX_VIDEO_INTERLACE_MODE)
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_00, 0x017F, 0x005F);
        //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_17, 0x4000, 0x4000); //reg_interlace_mode_sel
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x0040, 0x0040); //video clock div 2
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, 0x0004, 0x0004); // PG interlace enable
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_00, 0x017F, 0x004E);
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_17, 0x4000, 0x0000); //reg_interlace_mode_sel
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x0040, 0x0000); //video clock div 2
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, 0x0004, 0x0000); // PG interlace disable
    }
    #endif

    if ((gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME &(~0x80)].PktPara.AVIInfoPktPara.enColorFmt != E_HDMITX_VIDEO_COLOR_YUV420) &&
        (
            (idx == E_HDMITX_RES_3840x2160p_60Hz) || (idx == E_HDMITX_RES_3840x2160p_50Hz) || \
            (idx == E_HDMITX_RES_4096x2160p_50Hz) || (idx == E_HDMITX_RES_4096x2160p_60Hz) || \
            (
                (
                    (idx == E_HDMITX_RES_1280x1470p_60Hz) || \
                    (idx == E_HDMITX_RES_3840x2160p_24Hz) || \
                    (idx == E_HDMITX_RES_3840x2160p_25Hz) || \
                    (idx == E_HDMITX_RES_3840x2160p_30Hz) || \
                    (idx == E_HDMITX_RES_4096x2160p_24Hz) || \
                    (idx == E_HDMITX_RES_4096x2160p_25Hz) || \
                    (idx == E_HDMITX_RES_4096x2160p_30Hz)
                    
                ) && 
                (
                    (cd_val != E_HDMITX_VIDEO_CD_24Bits) && (cd_val != E_HDMITX_VIDEO_CD_NoID)
                )
            )
         ))
        bIsHDMI20 = TRUE;

    if ((gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME &(~0x80)].PktPara.AVIInfoPktPara.enColorFmt == E_HDMITX_VIDEO_COLOR_YUV420) &&
        ((idx == E_HDMITX_RES_3840x2160p_60Hz) || (idx == E_HDMITX_RES_3840x2160p_50Hz) || (idx == E_HDMITX_RES_4096x2160p_50Hz) || (idx == E_HDMITX_RES_4096x2160p_60Hz)) &&
        (cd_val != E_HDMITX_VIDEO_CD_24Bits) && (cd_val != E_HDMITX_VIDEO_CD_NoID))
        bIsHDMI20 = TRUE;

    if (g_bSupportSCDC == TRUE)
    {
        //for 2.0
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_46, 0x0100, bIsHDMI20 ? 0x0100 : 0x00);
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_52, BIT0, bIsHDMI20 ? BIT0 : 0x00);

        //scdc
        if (bIsHDMI20 == TRUE)
            Mhal_HDMITx_SCDCSetTmdsConfig(TRUE, TRUE);
        else
            Mhal_HDMITx_SCDCSetTmdsConfig(FALSE, FALSE);
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_46, 0x0100, 0x00);
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_52, BIT0, 0x00);
    }

    
    uiTMDSCLK = MHal_HDMITx_GetPixelClk_ByTiming(idx, gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME &(~0x80)].PktPara.AVIInfoPktPara.enColorFmt, cd_val);
    uiTMDSCLK /= 1000;

    #if (defined(HDMITX_SUPPORT_444TO420) && (HDMITX_SUPPORT_444TO420 == 1))
    if (gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME &(~0x80)].PktPara.AVIInfoPktPara.enColorFmt == E_HDMITX_VIDEO_COLOR_YUV420)
    {
        if(cd_val > E_HDMITX_VIDEO_CD_24Bits)
        {
            switch (idx)
            {
                case E_HDMITX_RES_3840x2160p_50Hz:
                    idx = E_HDMITX_RES_3840x2160p_25Hz;
                    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x0000); //video, pixel,tmds clock div 2 for 420
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 12);
                break;

                case E_HDMITX_RES_3840x2160p_60Hz:
                    idx = E_HDMITX_RES_3840x2160p_30Hz;
                    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x0000); //video, pixel,tmds clock div 2 for 420
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 12);
                break;

                case E_HDMITX_RES_4096x2160p_50Hz:
                    idx = E_HDMITX_RES_4096x2160p_25Hz;
                    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x0000); //video, pixel,tmds clock div 2 for 420
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 12);
                break;

                case E_HDMITX_RES_4096x2160p_60Hz:
                    idx = E_HDMITX_RES_4096x2160p_30Hz;
                    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x0000); //video, pixel,tmds clock div 2 for 420
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
                    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 12);
                break;

                default:
                    if( idx < E_HDMITX_RES_1920x1080p_50Hz)
                    {
                        printf("[HDMITX] Invalid Combination of Color Format & Video Timing, Keep Origional Timing Setting!!\r\n");
                    }
                    else
                    {
                        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x01C0); //video, pixel,tmds clock div 2 for 420
                        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0001); //PHY tmds clock div 2
                        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideo_OTMDS_ODATA_Ratio[ucCDIdx][idx] << 12);
                    }
                break;
            }
        }
        else
        {
            if( idx < E_HDMITX_RES_1920x1080p_50Hz)
            {
                printf("[HDMITX] Invalid Combination of Color Format & Video Timing, Keep Origional Timing Setting!!\r\n");
            }
            else
            {
                MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x01C0); //video, pixel,tmds clock div 2 for 420
                MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0001); //PHY tmds clock div 2
                MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideo_OTMDS_ODATA_Ratio[ucCDIdx][idx] << 12);
            }
        }

        bIs420Fmt = TRUE;
        //MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x0040, 0x0040); //video clock div 2 for 420

    }
    else if (HDMITxVideoModeTbl[idx].i_p_mode != E_HDMITX_VIDEO_INTERLACE_MODE)//Not 420
    {
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x01C0, 0x0000); //video, pixel,tmds clock div 2 for 420
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, 0x0001, 0x0000); //PHY tmds clock div 2
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 12);
    }
    #else
    if (gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME &(~0x80)].PktPara.AVIInfoPktPara.enColorFmt == E_HDMITX_VIDEO_COLOR_YUV420)
    {
        switch (idx)
        {
            case E_HDMITX_RES_3840x2160p_50Hz:
                idx = E_HDMITX_RES_3840x2160p_25Hz;
            break;

            case E_HDMITX_RES_3840x2160p_60Hz:
                idx = E_HDMITX_RES_3840x2160p_30Hz;
            break;

            case E_HDMITX_RES_4096x2160p_50Hz:
                idx = E_HDMITX_RES_4096x2160p_25Hz;
            break;

            case E_HDMITX_RES_4096x2160p_60Hz:
                idx = E_HDMITX_RES_4096x2160p_30Hz;
            break;

            default:
                printf("[HDMITX] Invalid Combination of Color Format & Video Timing, Keep Origional Timing Setting!!\r\n");
            break;
        }

        bIs420Fmt = TRUE;
        MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_1B, 0x0040, 0x0040); //video clock div 2 for 420
    }
    #endif

    //Timing Changed Judgement
    if( (MHal_HDMITx_Read(HDMITX_VIDEO_REG_BASE, REG_VE_STATUS_15) & 0x3FFF) == HDMITxVideoModeTbl[idx].vtotal)
    {
        if( MHal_HDMITx_Read(HDMITX_VIDEO_REG_BASE, REG_VE_STATUS_16) == HDMITxVideoModeTbl[idx].htotal)
        {
            MS_U32 uiSynthSet = 0;
            uiSynthSet |= (MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_03) & 0x0FFF);
            uiSynthSet = uiSynthSet << 16;
            uiSynthSet |= MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_02);


            if(uiSynthSet == HDMITxVideoAtopSetting[ucCDIdx][idx].SynthSSCSet)
            {
                ubTimingChanged = FALSE;
            }
        }
    }

    if((idx == E_HDMITX_RES_720x480i) || (idx == E_HDMITX_RES_720x576i))
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_39, 0x00FF, 0x008B);
        MHal_HDMITx_Mask_Write(CLKGEN0_REG_BASE, REG_CKG0_ODCLK_SOURCE, 0x000F, 0x0001);//Repetition times

        bIsRPMode = TRUE;
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_39, 0x00FF, 0x00AC);
        MHal_HDMITx_Mask_Write(CLKGEN0_REG_BASE, REG_CKG0_ODCLK_SOURCE, 0x000F, 0x0000);//Repetition times

        bIsRPMode = FALSE;
    }

    //enable H, VSync regen
    MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_01, 0x8001, 0x8001);
    MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_01, 0x0002, HDMITxVideoModeTbl[idx].h_polarity ? 0x0002 : 0x0000);
    MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_01, 0x0004, HDMITxVideoModeTbl[idx].v_polarity ? 0x0004 : 0x0000);

    //deep color setting
    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_HDMI_CONFIG1_00, 0x00C0, ucCDIdx << 6);

    //HDMITx phy clock
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_10, 0xCC0C, 0x0000);

    #if( (!(defined(HDMITX_SUPPORT_444TO420) && (HDMITX_SUPPORT_444TO420 == 1))) && (!(defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))) )
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_15, 0x7000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 12);
    #endif

    //sythesizer setting
    MHal_HDMITx_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_02, (MS_U16)(HDMITxVideoAtopSetting[ucCDIdx][idx].SynthSSCSet & 0x0000FFFF));
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_03, 0x0FFF, (MS_U16)((HDMITxVideoAtopSetting[ucCDIdx][idx].SynthSSCSet & 0x0FFF0000) >> 16));

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_79, 0x1003, 0x0000);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_26, 0x0100, 0x0000);

    //atop
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_38, 0x0003, HDMITxVideoAtopSetting[ucCDIdx][idx].MUX_DIVSEL_POST);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3C, 0xC000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_POST << 14);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3C, 0x3000, HDMITxVideoAtopSetting[ucCDIdx][idx].TXPLL_DIVSEL_PIXEL << 12);

#if 0
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_34, 0x3F00, HDMITxVideoAtopSetting[ucCDIdx][idx].ICTRL_PREDRV_MAIN_L012 << 8);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_35, 0x3F00, HDMITxVideoAtopSetting[ucCDIdx][idx].ICTRL_PREDRV_MAIN_L012 << 8);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_35, 0x003F, HDMITxVideoAtopSetting[ucCDIdx][idx].ICTRL_PREDRV_MAIN_L012);
#else //RD suggestion
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_34, 0x3F00, 0x0000);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_35, 0x3F00, 0x0000);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_35, 0x003F, 0x0000);
#endif

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_30, 0x3F00, HDMITxVideoAtopSetting[ucCDIdx][idx].ICTRL_DRV_MAIN_L012 << 8);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_31, 0x3F00, HDMITxVideoAtopSetting[ucCDIdx][idx].ICTRL_DRV_MAIN_L012 << 8);
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_31, 0x003F, HDMITxVideoAtopSetting[ucCDIdx][idx].ICTRL_DRV_MAIN_L012);

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3A, 0x000F, HDMITxVideoAtopSetting[ucCDIdx][idx].PD_RT);


    // Timing regeneration
    MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_17, BIT10|BIT13, 0x0000/*BIT10|BIT13*/); // 0: delay from DE end; 1: delay from end of h, v sync

    #if( (!(defined(HDMITX_SUPPORT_444TO420) && (HDMITX_SUPPORT_444TO420 == 1))) && (!(defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))) )
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_02, HDMITxVideoModeTbl[idx].vs_width);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_05, HDMITxVideoModeTbl[idx].vs_delayline);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_06, HDMITxVideoModeTbl[idx].vs_delaypixel);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_07, HDMITxVideoModeTbl[idx].hs_width >> (bIs420Fmt ? 0x01 : 0x00));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_0A, HDMITxVideoModeTbl[idx].hs_delay >> (bIs420Fmt ? 0x01 : 0x00));

    //for PG
    wHfront = HDMITxVideoModeTbl[idx].hs_delay;
    wVfront = HDMITxVideoModeTbl[idx].vs_delayline;

    //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, 0x0003, 0x0003);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_31, (bIs420Fmt||bIsRPMode) ? (HDMITxVideoModeTbl[idx].hde_width/4 - 1) : (HDMITxVideoModeTbl[idx].hde_width/2 - 1));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_32, (bIs420Fmt||bIsRPMode) ?
        ((HDMITxVideoModeTbl[idx].htotal - HDMITxVideoModeTbl[idx].hde_width)/4 - 1) : ((HDMITxVideoModeTbl[idx].htotal - HDMITxVideoModeTbl[idx].hde_width)/2 - 1));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_33, (bIs420Fmt||bIsRPMode) ? (wHfront/4 - 1) : (wHfront/2 - 1));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_34, (bIs420Fmt||bIsRPMode) ? (HDMITxVideoModeTbl[idx].hs_width/4 - 1) : (HDMITxVideoModeTbl[idx].hs_width/2 - 1));

    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_35, HDMITxVideoModeTbl[idx].vde_width - 1);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_36, (HDMITxVideoModeTbl[idx].i_p_mode == E_HDMITX_VIDEO_INTERLACE_MODE) ?
        ((HDMITxVideoModeTbl[idx].vtotal - HDMITxVideoModeTbl[idx].vde_width)/2 - 1) : (HDMITxVideoModeTbl[idx].vtotal - HDMITxVideoModeTbl[idx].vde_width - 1));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_37, wVfront - 1);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_38, HDMITxVideoModeTbl[idx].vs_width - 1);

    #else//444->420 || P2I

    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_02, HDMITxVideoModeTbl[eSize_idx].vs_width);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_05, HDMITxVideoModeTbl[eSize_idx].vs_delayline);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_06, HDMITxVideoModeTbl[eSize_idx].vs_delaypixel);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_07, HDMITxVideoModeTbl[eSize_idx].hs_width >> (bIs420Fmt ? 0x01 : 0x00));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_0A, HDMITxVideoModeTbl[eSize_idx].hs_delay >> (bIs420Fmt ? 0x01 : 0x00));

    //for PG
    wHfront = HDMITxVideoModeTbl[eSize_idx].hs_delay;
    wVfront = HDMITxVideoModeTbl[eSize_idx].vs_delayline;

    //MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_30, 0x0003, 0x0003);
    #if (defined(HDMITX_SUPPORT_444TO420) && (HDMITX_SUPPORT_444TO420 == 1))
    if( ( (eSize_idx == E_HDMITX_RES_3840x2160p_60Hz) || (eSize_idx == E_HDMITX_RES_3840x2160p_50Hz) || \
          (eSize_idx == E_HDMITX_RES_4096x2160p_50Hz) || (eSize_idx == E_HDMITX_RES_4096x2160p_60Hz)
        ) && bIs420Fmt && (cd_val > E_HDMITX_VIDEO_CD_24Bits)
       )//4K60 420
    {
        if( MHal_HDMITx_Read(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_31) != (HDMITxVideoModeTbl[eSize_idx].hde_width/4 - 1) )
        {
            if(ubTimingChanged == FALSE)
            {
                ubTimingChanged = TRUE;
            }
        }

        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_31, (HDMITxVideoModeTbl[eSize_idx].hde_width/4 - 1) );
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_32, ((HDMITxVideoModeTbl[eSize_idx].htotal - HDMITxVideoModeTbl[eSize_idx].hde_width)/4 - 1) );
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_33, (wHfront/4 - 1) );
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_34, (HDMITxVideoModeTbl[eSize_idx].hs_width/4 - 1) );
    }
    else
    {
        if(bIsRPMode)
        {
            if( MHal_HDMITx_Read(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_31) != (HDMITxVideoModeTbl[eSize_idx].hde_width/4 - 1) )
            {
                if(ubTimingChanged == FALSE)
                {
                    ubTimingChanged = TRUE;
                }
            }
        }
        else
        {
            if( MHal_HDMITx_Read(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_31) != (HDMITxVideoModeTbl[eSize_idx].hde_width/2 - 1) )
            {
                if(ubTimingChanged == FALSE)
                {
                    ubTimingChanged = TRUE;
                }
            }
        }

        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_31, (bIsRPMode) ? (HDMITxVideoModeTbl[eSize_idx].hde_width/4 - 1) : (HDMITxVideoModeTbl[eSize_idx].hde_width/2 - 1));
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_32, (bIsRPMode) ?
            ((HDMITxVideoModeTbl[eSize_idx].htotal - HDMITxVideoModeTbl[eSize_idx].hde_width)/4 - 1) : ((HDMITxVideoModeTbl[eSize_idx].htotal - HDMITxVideoModeTbl[eSize_idx].hde_width)/2 - 1));
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_33, (bIsRPMode) ? (wHfront/4 - 1) : (wHfront/2 - 1));
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_34, (bIsRPMode) ? (HDMITxVideoModeTbl[eSize_idx].hs_width/4 - 1) : (HDMITxVideoModeTbl[eSize_idx].hs_width/2 - 1));
    }
    #else//Not 444->420
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_31, (bIs420Fmt||bIsRPMode) ? (HDMITxVideoModeTbl[idx].hde_width/4 - 1) : (HDMITxVideoModeTbl[idx].hde_width/2 - 1));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_32, (bIs420Fmt||bIsRPMode) ?
        ((HDMITxVideoModeTbl[idx].htotal - HDMITxVideoModeTbl[idx].hde_width)/4 - 1) : ((HDMITxVideoModeTbl[idx].htotal - HDMITxVideoModeTbl[idx].hde_width)/2 - 1));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_33, (bIs420Fmt||bIsRPMode) ? (wHfront/4 - 1) : (wHfront/2 - 1));
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_34, (bIs420Fmt||bIsRPMode) ? (HDMITxVideoModeTbl[idx].hs_width/4 - 1) : (HDMITxVideoModeTbl[idx].hs_width/2 - 1));
    #endif

    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_35, HDMITxVideoModeTbl[eSize_idx].vde_width - 1);

    #if(defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_36, (HDMITxVideoModeTbl[eSize_idx].vtotal - HDMITxVideoModeTbl[eSize_idx].vde_width - 1) );
    #else
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_36, (HDMITxVideoModeTbl[idx].i_p_mode == E_HDMITX_VIDEO_INTERLACE_MODE) ?
        ((HDMITxVideoModeTbl[idx].vtotal - HDMITxVideoModeTbl[idx].vde_width)/2 - 1) : (HDMITxVideoModeTbl[idx].vtotal - HDMITxVideoModeTbl[idx].vde_width - 1));
    #endif
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_37, wVfront - 1);
    MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_38, HDMITxVideoModeTbl[eSize_idx].vs_width - 1);

    //P2I Setting
    #if(defined(HDMITX_SUPPORT_P2I) && (HDMITX_SUPPORT_P2I == 1))
    if(HDMITxVideoModeTbl[idx].i_p_mode == E_HDMITX_VIDEO_INTERLACE_MODE)
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_50, 0x000D, 0x000D);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_52, wVfront);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_53, HDMITxVideoModeTbl[eSize_idx].vs_width);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_54, bIsRPMode ? wHfront/2 : wHfront);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_55, wHfront ? HDMITxVideoModeTbl[eSize_idx].hs_width/2 : HDMITxVideoModeTbl[eSize_idx].hs_width);
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_50, 0x000D, 0x0000);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_52, wVfront);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_53, HDMITxVideoModeTbl[eSize_idx].vs_width);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_54, bIsRPMode ? wHfront/2 : wHfront);
        MHal_HDMITx_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_55, wHfront ? HDMITxVideoModeTbl[eSize_idx].hs_width/2 : HDMITxVideoModeTbl[eSize_idx].hs_width);
    }
    #endif

    //444 -> 420
    #if (defined(HDMITX_SUPPORT_444TO420) && (HDMITX_SUPPORT_444TO420 == 1))
    if(bIs420Fmt)
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_50, 0x0002, 0x0002);
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_VIDEO_REG_BASE, REG_VE_CONFIG_50, 0x0002, 0x0000);
    }
    #endif///444->420

    #endif//!444->420 && !P2I

    MHal_HDMITx_EnableSSC(ubSSCEn, uiTMDSCLK);

    // fifo reset
    if(ubTimingChanged)
    {
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_0F, 0x0008, 0x0008);
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_0F, 0x0008, 0);
    }
}

//------------------------------------------------------------------------------
/// @brief This routine will power on or off HDMITx clock (power saving)
/// @param[in] bEnable: TRUE/FALSE
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_Power_OnOff(MS_BOOL bEnable)
{
    if (bEnable)
    {
        MHal_HDMITx_Mask_Write(CLKGEN1_REG_BASE, REG_CKG_HDMITx_CLK_28, BIT0, 0);
    }
    else
    {
        MHal_HDMITx_Mask_Write(CLKGEN1_REG_BASE, REG_CKG_HDMITx_CLK_28, BIT0, BIT0);
    }
}


MS_BOOL MHal_HDMITx_RxBypass_Mode(MsHDMITX_INPUT_FREQ freq, MS_BOOL bflag)
{
    //wilson@kano:TBD
    #if 1
    return FALSE;
    #else
    printf("[HDMITx] Bypass Mode = 0x%d\r\n", bflag);
    if (bflag == TRUE)
    {
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY1_REG_BASE, REG_COMBOPHY1_CONFIG_3C, BIT0, BIT0); //reg_atop_en_clko_vcodiv8_syn

        //Note: change frequency tolerance if need
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY0_CONFIG_4C, 0xE000, 0xA000); //[13..15] reg_hdmi_clk_thr1 tolerance
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY0_CONFIG_4B, 0xE000, 0xA000); //[13..15] reg_hdmi_clk_thr1 tolerance
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY0_CONFIG_4A, 0xE000, 0xA000); //[13..15] reg_hdmi_clk_thr1 tolerance
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY0_CONFIG_49, 0xE000, 0xA000); //[13..15] reg_hdmi_clk_thr1 tolerance
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY0_CONFIG_09, 0xE000, 0xA000); //[13..15] reg_hdmi_clk_thr1 tolerance
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY0_CONFIG_22, 0xE000, 0xA000); //[13..15] reg_hdmi_clk_thr1 tolerance
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY0_CONFIG_49, 0x1FFF, 0x0C6A); //change 250Mhz -> 297Mhz

        //Enable Fifo and select input source
        MHal_HDMITx_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_10, 0x1919); //selct clock from Rx;

#if 0
        if (MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_7A) & 0x00C0) //for HDMI 2.0 timing bypass mode
        {
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, BIT0, BIT0); //tmds clock div 2;
        }
#endif

        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_16, 0x000E, 0x0004); //[1:0]: Lane 0 to fifo; [2]: enable
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_17, 0x000E, 0x0005); //[1:0]: Lane 1 to fifo; [2]: enable
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_18, 0x000E, 0x0006); //[1:0]: Lane 2 to fifo; [2]: enable
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_19, 0x000E, 0x0007); //[1:0]: Lane 3 to fifo; [2]: enable

        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_79, 0x1000, 0x1000); //bit repetition HW mode;

        //synthesizer setting //TBD
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_10, 0x0001, 0x0001);
        //ssc_set = 0x140000
        MHal_HDMITx_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_02, 0x0000); //ssc_set [15..0]
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_03, 0x00FF, 0x0014);//ssc_set [7..0]

        #if 0 //with SSC clock
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_05, 0xFFF0, ssc_step);
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_06, 0x3000, ssc_span);
        #else //without SSC clock
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_05, 0xFFF0, 0x0000);
            MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_06, 0x3FFF, 0x0000);
        #endif

        //ATOP setting
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_2E, 0xEA00, 0xEA00); //[9]:en_clk_pixel;[11]:en_clk_tmds;[13..15]:en_data_out
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_39, 0xFFFF, 0x0000); //[11..0]: disable power down;[15:12]:pre-emphasis
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_3A, 0x007F, 0x000F); //[3:0]:rterm turn off;[6:4]:disable power down
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_79, 0x2001, 0x2001); //[0]:turn on HW mode; [13]: Rx to tmds bypass
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_46, 0x0200, 0x0200); //[9] hdmi20 hw config mode;
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_7E, 0x0200, 0x0200); //[9]: freq range tolerance up direction;
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_60, 0x0001, 0x0001); //[0]: freq range reset follow rx big change;
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY1_REG_BASE, REG_COMBOPHY1_CONFIG_3C, BIT0, 0x0000);
        MHal_HDMITx_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_10, 0x1101); //selct clock from Rx;
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, BIT0, 0x0000); //tmds clock div 2;
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_79, 0x2001, 0x0000); //[0]:turn on HW mode; [13]: Rx to tmds bypass
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_46, 0x0200, 0x0000); //[9] hdmi20 hw config mode;
    }

    return TRUE;

    #endif
}


/// @brief This routine will disable TMDS clock, data, and DDC... bypass mode
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MHal_HDMITx_Disable_RxBypass(void)
{
#if 1
    return FALSE;
#else
    MHal_HDMITx_Mask_Write(HDMIRX_COMBOPHY0_REG_BASE, REG_COMBOPHY1_CONFIG_3C, BIT0, 0x0000);
    MHal_HDMITx_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_10, 0x1101); //selct clock from Rx;
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_11, BIT0, 0x0000); //tmds clock div 2;
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_79, 0x2001, 0x0000); //[0]:turn on HW mode; [13]: Rx to tmds bypass
    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_46, 0x0200, 0x0000); //[9] hdmi20 hw config mode;

    return TRUE;
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine will set GPIO pin for HPD
/// @param[in] u8pin: GPIO0 ~ 12
/// @return None
//------------------------------------------------------------------------------
void MHal_HDMITx_SetHPDGpioPin(MS_U8 u8pin)
{
    printf("_gHPDGpioPin = 0x%X\r\n", u8pin);
    _gHPDGpioPin = u8pin;
}

//------------------------------------------------------------------------------
/// @brief This routine return CHIP capability of DVI mode
/// @return TRUE, FALSE
//------------------------------------------------------------------------------
MS_BOOL MHal_HDMITx_IsSupportDVIMode(void)
{
    return TRUE;
}

// *************  For customer NDS **************//

void MHal_HDMITx_Set_AVI_InfoFrame(MsHDMITX_PACKET_PROCESS packet_process, MsHDMITX_AVI_CONTENT_TYPE content_type, MS_U16 *data)
{
    MS_U16 tmp_value=0;

    if (IS_STOP_PKT(packet_process))   // Stop sending packet
    {
        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, 0x0005, 0x0000);  // Stop AVI packet
    }
    else
    {
        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09, 0x0080, 0x80); // EIA version 2
        switch(content_type)
        {
            case E_HDMITX_AVI_PIXEL_FROMAT:
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09, 0x0060, *data);
				break;
            case E_HDMITX_AVI_ASPECT_RATIO:
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09, 0x3F1F, *data);
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_2_0A, 0x0003, *(data+1));
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_4_0C, 0xFFFF, *(data+2));
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_5_0D, 0xFFFF, *(data+3));
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_6_0E, 0xFFFF, *(data+4));
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_7_0F, 0xFFFF, *(data+5));
                break;
            case E_HDMITX_AVI_COLORIMETRY:
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09, 0xC000, *data);
                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_2_0A, 0x0030, *(data+1));
                break;
            default:
                break;
        }

        tmp_value = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_AVI_INFOFRAME);  // Checksum
        // cyclic packet
        if (IS_CYCLIC_PKT(packet_process))
            MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, ( (tmp_value<<8) | (HDMITX_PACKET_AVI_FCNT<<3) | 0x0005));  // send AVI packet
        // single packet
        else
        {
            MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, (tmp_value<<8) | 0x0001);
            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1<<E_HDMITX_ACT_AVI_CMD);
        }
    }
}

//wilson@kano
//**************************************************************************
//  [Function Name]:
//                      MHal_HDMITx_SetGCPParameter()
//  [Description]:
//                      setting General Control packet attribute
//  [Arguments]:
//                      [stGC_PktPara] stPktPara
//  [Return]:
//                      void
//
//**************************************************************************
void MHal_HDMITx_SetGCPParameter(stGC_PktPara stPktPara)
{
    gbGeneralPktList[E_HDMITX_GC_PACKET].PktPara.GCPktPara.enAVMute = stPktPara.enAVMute;
    gbGeneralPktList[E_HDMITX_GC_PACKET].PktPara.GCPktPara.enColorDepInfo = stPktPara.enColorDepInfo;
}

//**************************************************************************
//  [Function Name]:
//                      MHal_HDMITx_SetPktAttribute()
//  [Description]:
//                      configure settings to corresponding packet
//  [Arguments]:
//                      [MsHDMITX_PACKET_TYPE] enPktType
//                      [MS_BOOL] bEnUserDef
//                      [MS_U8] u8FrmCntNum
//                      [MsHDMITX_PACKET_PROCESS] enPktCtrl
//  [Return]:
//                      void
//
//**************************************************************************
void MHal_HDMITx_SetPktAttribute(MsHDMITX_PACKET_TYPE enPktType, MS_BOOL bEnUserDef, MS_U8 u8FrmCntNum, MsHDMITX_PACKET_PROCESS enPktCtrl)
{
	if (enPktType & 0x80) //infoframe packet type
	{
		gbInfoFrmPktList[enPktType & (~0x80)].EnableUserDef = bEnUserDef;
		gbInfoFrmPktList[enPktType & (~0x80)].FrmCntNum = u8FrmCntNum;
		gbInfoFrmPktList[enPktType & (~0x80)].enPktCtrl = enPktCtrl;
	}
	else
	{
		gbGeneralPktList[enPktType].EnableUserDef = bEnUserDef;
		gbGeneralPktList[enPktType].FrmCntNum = u8FrmCntNum;
		gbGeneralPktList[enPktType].enPktCtrl = enPktCtrl;
	}
}

//**************************************************************************
//  [Function Name]:
//                      MHal_HDMITx_SetAVIInfoParameter()
//  [Description]:
//                      Assign content to AVI Infoframe packet
//  [Arguments]:
//                      [stAVIInfo_PktPara] stPktPara
//  [Return]:
//                      void
//
//**************************************************************************
void MHal_HDMITx_SetAVIInfoParameter(stAVIInfo_PktPara stPktPara)
{
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.A0Value = stPktPara.A0Value;
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enableAFDoverWrite = stPktPara.enableAFDoverWrite;
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enColorFmt = stPktPara.enColorFmt;
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enColorimetry = stPktPara.enColorimetry;
    gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enExtColorimetry = stPktPara.enExtColorimetry;
    gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enYCCQuantRange = stPktPara.enYCCQuantRange;
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enVidTiming = stPktPara.enVidTiming;
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enAFDRatio = stPktPara.enAFDRatio;
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enScanInfo = stPktPara.enScanInfo;
	gbInfoFrmPktList[E_HDMITX_AVI_INFOFRAME & (~0x80)].PktPara.AVIInfoPktPara.enAspectRatio = stPktPara.enAspectRatio;
}

//**************************************************************************
//  [Function Name]:
//                      MHal_HDMITx_SetVSInfoParameter()
//  [Description]:
//                      Assign content to VendorSpecific Infoframe packet
//  [Arguments]:
//                      [stVSInfo_PktPara] stPketPara
//  [Return]:
//                      void
//
//**************************************************************************
void MHal_HDMITx_SetVSInfoParameter(stVSInfo_PktPara stPktPara)
{
	gbInfoFrmPktList[E_HDMITX_VS_INFOFRAME & (~0x80)].PktPara.VSInfoPktPara.en3DStruct = stPktPara.en3DStruct;
	gbInfoFrmPktList[E_HDMITX_VS_INFOFRAME & (~0x80)].PktPara.VSInfoPktPara.en4k2kVIC = stPktPara.en4k2kVIC;
	gbInfoFrmPktList[E_HDMITX_VS_INFOFRAME & (~0x80)].PktPara.VSInfoPktPara.enVSFmt = stPktPara.enVSFmt;
}

//**************************************************************************
//  [Function Name]:
//                      MHal_HDMITx_SetVSInfoParameter()
//  [Description]:
//                      Assign content to VendorSpecific Infoframe packet
//  [Arguments]:
//                      [stVSInfo_PktPara] stPketPara
//  [Return]:
//                      void
//
//**************************************************************************
void MHal_HDMITx_SetAudioInfoParameter(stAUDInfo_PktPara stPktPara)
{
	gbInfoFrmPktList[E_HDMITX_AUDIO_INFOFRAME & (~0x80)].PktPara.AUDInfoPktPara.enAudChCnt = stPktPara.enAudChCnt;
	gbInfoFrmPktList[E_HDMITX_AUDIO_INFOFRAME & (~0x80)].PktPara.AUDInfoPktPara.enAudType = stPktPara.enAudType;
	gbInfoFrmPktList[E_HDMITX_AUDIO_INFOFRAME & (~0x80)].PktPara.AUDInfoPktPara.enAudFreq = stPktPara.enAudFreq;
}

//**************************************************************************
//  [Function Name]:
//                      MHal_HDMITx_SendPacket()
//  [Description]:
//                      configure  packet content and process according to user define or defalut setting
//  [Arguments]:
//                      [MsHDMITX_PACKET_TYPE] enPktType
//  [Return]:
//                      void
//
//**************************************************************************
void MHal_HDMITx_SendPacket(MsHDMITX_PACKET_TYPE enPktType, MsHDMITX_PACKET_PROCESS packet_process)
{
	if (enPktType & 0x80) //info frame packet
	{
		MS_U8 ucInfoPktType = enPktType & (~0x80);
		MS_U8 ucChkSum = 0;

        gbInfoFrmPktList[ucInfoPktType].enPktCtrl = packet_process;

		switch (enPktType)
		{
			case E_HDMITX_VS_INFOFRAME:
				if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_STOP_PACKET)
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_CFG_35, 0x0005, 0x0000);
				}
				else
				{
					if (gbInfoFrmPktList[ucInfoPktType].EnableUserDef == TRUE)
					{
						ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_VS_INFOFRAME);

						if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
						{
							MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_VS_CFG_35, (ucChkSum << 8) | 0x0001);
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_VSP_CMD);
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_VS_CFG_35, ((ucChkSum<<8) | (gbGeneralPktList[ucInfoPktType].FrmCntNum << 3) | 0x0005));
						}
					}
					else
					{
						//fill IEEE HDMI tag
						MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_1_27, 0xFFFF, 0x0C03);

						//check content
						if (gbInfoFrmPktList[ucInfoPktType].PktPara.VSInfoPktPara.enVSFmt == E_HDMITX_VIDEO_VS_3D)
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_2_28, 0xE000, E_HDMITX_VIDEO_VS_3D << 13);  // video format
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_3_29, 0x00FF, (gbInfoFrmPktList[ucInfoPktType].PktPara.VSInfoPktPara.en3DStruct) << 4);  // 3D structure
						}
						else if (gbInfoFrmPktList[ucInfoPktType].PktPara.VSInfoPktPara.enVSFmt == E_HDMITX_VIDEO_VS_4k_2k)
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_2_28, 0xE000, E_HDMITX_VIDEO_VS_4k_2k << 13);  // video format
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_3_29, 0x00FF, (gbInfoFrmPktList[ucInfoPktType].PktPara.VSInfoPktPara.en4k2kVIC));	// 4k2k vic
						}
						else
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_2_28, 0xE000, 0);  // video format
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_VS_3_29, 0x00FF, 0);
						}

						ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_VS_INFOFRAME);

						if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_VS_CFG_35, (ucChkSum << 8) | 0x0001);
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_VSP_CMD);
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_VS_CFG_35, ((ucChkSum << 8) | (HDMITX_PACKET_VS_FCNT << 3) | 0x0005));
						}
					}
				}
			break;

    		case E_HDMITX_AVI_INFOFRAME:
				if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_STOP_PACKET)
				{
				    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, 0x0005, 0x0000);
				}
				else
				{
                    if (gbInfoFrmPktList[ucInfoPktType].EnableUserDef == TRUE)
                    {
                        ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_AVI_INFOFRAME);

    					if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
    					{
    						MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, (ucChkSum << 8) | 0x0001);
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_AVI_CMD);
    					}
    					else
    					{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, ((ucChkSum << 8) | (gbInfoFrmPktList[ucInfoPktType].FrmCntNum << 3) | 0x0005));
    					}
                    }
                    else
                    {
    					MS_U8 ucPktVal = 0;

    					//Y2, Y1, Y0: RGB, YCbCr 422, 444, 420
    					ucPktVal = (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enColorFmt << 5);// | 0x10;
    					//A0 field
    					ucPktVal |= ((gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.A0Value == 0x01) ? 0x10 : 0x00);
    					//S1, S0 field
    					ucPktVal |= (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enScanInfo);

    					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09, 0x00FF, (MS_U16)ucPktVal); //MDrv_WriteByte(REG_HDMITX_09_L, ucPktVal);

    					//C1, C0, M1, M0
    					if ((gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming >= E_HDMITX_RES_720x480i) &&
    						(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming <= E_HDMITX_RES_720x576p))
    					{
    						ucPktVal = HDMITX_AviCmrTbl[gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming];
    						ucPktVal |= (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enAspectRatio << 4);
    					}
    					else
    					{
    						//HD timing is always 16:9
    						ucPktVal = HDMITX_AviCmrTbl[gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming];
    					}

    					//R3, R2, R1, R0: active porting aspect ration
    					if (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enableAFDoverWrite == TRUE)
    					{
    						ucPktVal |= (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enAFDRatio & 0x0F);
    					}

                        //ucPktVal |= ((MS_U8)gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enExtColorimetry == 0) ? 0 : 0xC0; //set [C1, C0] = [1, 1]

                        if(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enColorimetry != E_HDMITX_COLORIMETRY_MAX)
                        {
                            ucPktVal = (ucPktVal & 0x3F) | ( ((MS_U8)gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enColorimetry & 0x03) << 6);
                        }

    					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_1_09, 0xFF00, ucPktVal << 8); //MDrv_WriteByte(REG_HDMITX_09_H, ucPktVal);

                        //EC0, EC1, EC2
                        ucPktVal = (MS_U8)gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enExtColorimetry;
                        ucPktVal = (ucPktVal > 6) ? 6 : ucPktVal; //BT2020 RGB & BT2020 YCbCr share same value 6; 7 is reserved;
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_2_0A, 0x0070, ucPktVal << 4); //MDrv_WriteByteMask(REG_HDMITX_0A_L, ucPktVal << 4, 0x70);

                                        if(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enColorFmt == E_HDMITX_VIDEO_COLOR_RGB444)
                                        {
                                            //Q1, Q0
                                            if(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enYCCQuantRange == E_HDMITX_YCC_QUANT_LIMIT)
                                            {
                                                ucPktVal = 1;
                                            }
                                            else if(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enYCCQuantRange == E_HDMITX_YCC_QUANT_FULL)
                                            {
                                                ucPktVal = 2;
                                            }
                                            else
                                            {
                                                ucPktVal = 0;
                                            }

                                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_2_0A, 0x000C, ucPktVal << 2);
                                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_3_0B, 0x00C0, 0x00);
                                        }
                                        else
                                        {
                                            //YQ1, YQ0
                                            if(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enYCCQuantRange == E_HDMITX_YCC_QUANT_LIMIT)
                                            {
                                                ucPktVal = 0;
                                            }
                                            else if(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enYCCQuantRange == E_HDMITX_YCC_QUANT_FULL)
                                            {
                                                ucPktVal = 1;
                                            }
                                            else
                                            {
                                                ucPktVal = 3;
                                            }

                                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_2_0A, 0x000C, 0x00);
                                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_3_0B, 0x00C0, ucPktVal << 6);
                                        }

    					//VIC code: VIC code shoud +1 if aspect ration is 16:9
    					ucPktVal = HDMITX_AviVicTbl[gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming];

    					if (((gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming >= E_HDMITX_RES_720x480i) &&
    						(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming <= E_HDMITX_RES_720x576p)) &&
    						(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enAspectRatio == E_HDMITX_VIDEO_AR_16_9))
    					{
    						ucPktVal += 1;
    					}
                        else if (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enAspectRatio == E_HDMITX_VIDEO_AR_21_9)
                        {
                            MS_U8 AR21_9MappingTbl[14][2] = {
                                    {60, 65},
                                    {61, 66},
                                    {62, 67},
                                    {19, 68},
                                    { 4, 69},
                                    {41, 70},
                                    {47, 71},
                                    {32, 72},
                                    {33, 73},
                                    {34, 74},
                                    {31, 75},
                                    {16, 76},
                                    {64, 77},
                                    {63, 78}
                                };

                            if ((ucPktVal >= 93) && (ucPktVal <= 97))//3840*2160p@24 ~ 3840*2160@60
                            {
                                ucPktVal += 10;
                            }
                            else if ((gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming >= E_HDMITX_RES_3840x2160p_24Hz) &&\
                                (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming <= E_HDMITX_RES_3840x2160p_30Hz))
                            {
                                ucPktVal += (103 + (gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming - E_HDMITX_RES_3840x2160p_24Hz));
                            }
                            else if ((ucPktVal > 78) && (ucPktVal <= 92))
                            {
                                //do nothing;
                            }
                            else
                            {
                                MS_U8 i = 0;
                                MS_BOOL bValidVIC = FALSE;

                                for ( i = 0; i < 14; i++ )
                                {
                                    if (AR21_9MappingTbl[i][0] == ucPktVal)
                                    {
                                        ucPktVal = AR21_9MappingTbl[i][1];
                                        bValidVIC = TRUE;
                                        break;
                                    }
                                }

                                if (!bValidVIC)
                                {
                                    printf("%s :: Invalid VIC Code for 21:9 Aspect Ratio!!!\r\n", __FUNCTION__);
                                }
                            }
                        }

    					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_2_0A, 0x7F00, ucPktVal << 8); //MDrv_WriteByte(REG_HDMITX_0A_H, (ucPktVal & 0x7F));

    					//check repetition
    					if ((HDMITxVideoModeTbl[gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming].i_p_mode == E_HDMITX_VIDEO_INTERLACE_MODE) &&
    						(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enVidTiming <= E_HDMITX_RES_720x576i))
    					{
    						MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_3_0B, 0x000F, 0x0001); //MDrv_WriteByteMask(REG_HDMITX_0B_L, 0x01, 0x0F);
    					}
    					else
    					{
    						MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_3_0B, 0x000F, 0x0000); //MDrv_WriteByteMask(REG_HDMITX_0B_L, 0x00, 0x0F);
    					}

                        //YQ1, YQ0
                        //MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_3_0B, 0x00C0, (MS_U8)(gbInfoFrmPktList[ucInfoPktType].PktPara.AVIInfoPktPara.enYCCQuantRange << 6));

    					//AVI version
    					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AVI_7_0F, 0x0300, (MS_U16)(HDMITX_AVI_INFO_PKT_VER << 8));
    					//MDrv_WriteByteMask(REG_HDMITX_0F_H, HDMITX_AVI_INFO_PKT_VER, 0x03);

    					ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_AVI_INFOFRAME);

    					if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
    					{
    						MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, (ucChkSum << 8) | 0x0001);
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_AVI_CMD);
    					}
    					else
    					{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AVI_CFG_10, ((ucChkSum << 8) | (HDMITX_PACKET_AVI_FCNT << 3) | 0x0005));
							//MDrv_Write2Byte(REG_HDMITX_10_L, 0x05 | (ucChkSum << 8) | (HDMITX_PACKET_AVI_FCNT << 3));
    					}
                    }
				}
			break;

    		case E_HDMITX_SPD_INFOFRAME:
				if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_STOP_PACKET)
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_SPD_CFG_22, 0x0005, 0x0000); //MDrv_WriteByteMask(REG_HDMITX_22_L, 0x00, 0x05);
				}
				else
				{
					if (gbInfoFrmPktList[ucInfoPktType].EnableUserDef == TRUE)
					{
						ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_SPD_INFOFRAME);

						if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
						{
							MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_SPD_CFG_22, (ucChkSum << 8) | 0x0001);
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_SPD_CMD);
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_SPD_CFG_22, ((ucChkSum << 8) | (gbInfoFrmPktList[ucInfoPktType].FrmCntNum << 3) | 0x0005));
							//MDrv_Write2Byte(REG_HDMITX_22_L, 0x05 | (ucChkSum << 8) | (gbInfoFrmPktList[ucInfoPktType].FrmCntNum << 3));
						}
					}
					else
					{
						MS_U8 i = 0;
                        MS_U8 ucPktVal = 0;

                        for (i = 0; i < ((HDMITX_SPD_INFO_PKT_LEN + 1) >> 1); i++)
                        {
                            if (i < 4)  // vendor name
                            {
                                ucPktVal = (HDMITX_VendorName[2*i+1]<<8) | HDMITX_VendorName[2*i];
                                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_SPD_1_15+i, 0x7F7F, ucPktVal);
                            }
                            else if ((i >= 4) && (i < 12))  // product description
                            {
                                ucPktVal = (HDMITX_ProductName[2*(i-4)+1]<<8) | HDMITX_ProductName[2*(i-4)];
                                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_SPD_5_19+(i-4), 0x7F7F, ucPktVal);
                            }
                            else  // source device information
                            {
                                MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_SPD_13_21, 0x00FF, HDMITX_PACKET_SPD_SDI);
                            }
                        }

                        #if 0
						for ( i = 0; i < (HDMITX_SPD_INFO_PKT_LEN - 1); i++ )
						{
							if ( i < 8 )
								MDrv_WriteByte((REG_HDMITX_15_L + i), gbHDMITX_VendorName[i]);
							else
								MDrv_WriteByte((REG_HDMITX_15_L + i), gbHDMITX_ProductName[i-8]);
						}

						MDrv_WriteByte(REG_HDMITX_21_L, 0x01); //SPD infoframe, byte25: source information: 0x01 = Digital STB
                        #endif

						ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_SPD_INFOFRAME);

						if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
						{
							MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_SPD_CFG_22, (ucChkSum << 8) | 0x0001);
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_SPD_CMD);
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_SPD_CFG_22, ((ucChkSum << 8) | (HDMITX_PACKET_SPD_FCNT << 3) | 0x0005));
							//MDrv_Write2Byte(REG_HDMITX_22_L, 0x05 | (ucChkSum << 8) | (HDMITX_PACKET_SPD_FCNT << 3));
						}
					}
				}

			break;

    		case E_HDMITX_AUDIO_INFOFRAME:
				if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_STOP_PACKET)
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AUD_CFG_14, 0x0005, 0x0000);
				}
				else
				{
					// Modified for HDMI CTS test -
                    //     - Audio Coding Type (CT3~CT0) is 0x0 then continue else then FAIL
                    //     - Sampling Frequency (SF2~ SF0) is zero then continue else then FAIL.
                    //     - Sample Size (SS1~ SS0) is zero then continue else then FAIL.
                    //tmp_value = (gHDMITxInfo.output_audio_frequncy << 10) | 0x11;  // audio sampling frequency, PCM and 2 channel.
                    //MHal_HDMITx_Mask_Write(REG_HDMITX_BANK1, REG_PKT_AUD_1_11, 0x1FFF, tmp_value);

					if (gbInfoFrmPktList[ucInfoPktType].PktPara.AUDInfoPktPara.enAudChCnt == E_HDMITX_AUDIO_CH_2) // 2-channel
					{
					    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AUD_1_11, (E_HDMITX_AUDIO_CH_2 - 1) & 0x07); // 2 channels
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AUD_2_12, 0xFF00, 0); // Channel allocation
					}
					else //8- channel
					{
					    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AUD_1_11, (E_HDMITX_AUDIO_CH_8 - 1)&0x07); // 8 channels
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AUD_2_12, 0xFF00, 0x1F00); // Channel allocation
					}
					//clear LFEP value
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_AUD_3_13, 0x0001, 0x0000); //Fix LFEP defalut value in Kappa.

					ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_AUDIO_INFOFRAME);

					if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
					{
						MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AUD_CFG_14, (ucChkSum << 8) | 0x0001);
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_AUD_CMD);
					}
					else
					{
						if (gbInfoFrmPktList[ucInfoPktType].EnableUserDef == TRUE)
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AUD_CFG_14, ( (ucChkSum << 8) | (gbInfoFrmPktList[ucInfoPktType].FrmCntNum << 3) | 0x0005));
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_AUD_CFG_14, ( (ucChkSum << 8) | (HDMITX_PACKET_AUD_FCNT << 3) | 0x0005));
						}
					}
				}

                //NOTE:: Kano move channel status from 0x00 to 0x0A
                if (gbInfoFrmPktList[ucInfoPktType].EnableUserDef == FALSE)
                {
                    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS0_0A, ((gbInfoFrmPktList[ucInfoPktType].PktPara.AUDInfoPktPara.enAudType == E_HDMITX_AUDIO_PCM) ? 0 : BIT1)); // [1]: PCM / non-PCM
                    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS1_0B, (TxAudioFreqTbl[gbInfoFrmPktList[ucInfoPktType].PktPara.AUDInfoPktPara.enAudFreq].CH_Status3 << 8) | (gbInfoFrmPktList[ucInfoPktType].PktPara.AUDInfoPktPara.enAudChCnt << 4)); //[11:8]: audio sampling frequncy; [7:4]: audio channel count
                    MHal_HDMITx_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CH_STATUS2_0C, 0x0000);
                }
                // Audio sampling frequency
                // 1           1        0        0        32 kHz
                // 0           0        0        0        44.1 kHz
                // 0           0        0        1        88.2 kHz
                // 0           0        1        1        176.4 kHz
                // 0           1        0        0        48 kHz
                // 0           1        0        1        96 kHz
                // 0           1        1        1        192 kHz
                // 1           0        0        1        768 kHz
			break;

    		case E_HDMITX_MPEG_INFOFRAME:
				//TBD
			break;

            case E_HDMITX_HDR_INFOFRAME:
                if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_STOP_PACKET)
				{
					MHal_HDMITx_Mask_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1E, 0x0001, 0x0000);
				}
				else
				{
				    if (gbInfoFrmPktList[ucInfoPktType].EnableUserDef == TRUE)
					{
					    ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_HDR_INFOFRAME);

						if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
						{
							MHal_HDMITx_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1E, (ucChkSum << 8) | 0x0001); //[15..8]: chk_sum; [0]:hdr_send_cmd
                            MHal_HDMITx_Mask_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_00, 0xFFFF, 0x0001); //[0]:reg_act_hdr_cmd
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1E, ((ucChkSum<<8) | (gbGeneralPktList[ucInfoPktType].FrmCntNum << 3) | 0x0005));
						}
                    }
                    else
                    {
                        //TBD
                        ucChkSum = MHal_HDMITx_InfoFrameCheckSum(E_HDMITX_HDR_INFOFRAME);

                        if (gbInfoFrmPktList[ucInfoPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
						{
							MHal_HDMITx_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1E, (ucChkSum << 8) | 0x0001); //[15..8]: chk_sum; [0]:hdr_send_cmd
                            MHal_HDMITx_Mask_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_00, 0xFFFF, 0x0001); //[0]:reg_act_hdr_cmd
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_2_REG_BASE, REG_HDMI_2_CONFIG_1E, ((ucChkSum<<8) | (HDMITX_PACKET_HDR_FCNT << 3) | 0x0005));
						}

                    }
                }
            break;

			default:
				printf("hal_HDMITx_SendPacket():: Invalid Packet Type!!\r\n");
			break;
		}
	}
	else //general packet
	{
	    gbGeneralPktList[enPktType].enPktCtrl = packet_process;

		switch (enPktType)
		{
			case E_HDMITX_NULL_PACKET:
				if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_STOP_PACKET)
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_NUL_CFG_02, 0x0005, 0x0000);
				}
				else
				{
					if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
					{
						MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_NUL_CFG_02, 0x00FF, 0x0001);
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_NUL_CMD);
					}
					else
					{
						if (gbGeneralPktList[enPktType].EnableUserDef == TRUE)
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_NUL_CFG_02, 0x00FF, ((gbGeneralPktList[enPktType].FrmCntNum << 3) |0x0005) );
						}
						else
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_NUL_CFG_02, 0x00FF, ((HDMITX_PACKET_NULL_FCNT << 3) |0x0005) );
						}
					}
				}
			break;

			case E_HDMITX_ACR_PACKET:
				if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_STOP_PACKET) //stop packet
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACR_CFG_08, 0x000F, 0x0008);
				}
				else
				{
					if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
					{
						MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACR_CFG_08, 0x00FF, 0x0009);
						MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_ACR_CMD);
					}
					else //cyclic
					{
						if (gbGeneralPktList[enPktType].EnableUserDef == TRUE)
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACR_CFG_08, 0x00FF, ((gbGeneralPktList[enPktType].FrmCntNum << 4) |0x05));
						}
						else
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACR_CFG_08, 0x00FF, ((HDMITX_PACKET_ACR_FCNT << 4) |0x05));
						}
					}
				}
			break;

    		case E_HDMITX_AS_PACKET:
				if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_STOP_PACKET)
				{
					MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, BIT7|BIT0, BIT0); // [7]: disable audio FIFO, [0]:audio FIFO flush
				}
				else
				{
					MHal_HDMITx_Mask_Write(HDMITX_AUDIO_REG_BASE, REG_AE_CONFIG_05, BIT7|BIT0, BIT7); // [7]: enable audio FIFO, [0]:audio FIFO not flush
				}
			break;

    		case E_HDMITX_GC_PACKET:
				if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_STOP_PACKET) //stop packet
				{
				    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_GC_CFG_03, 0x000F, (gbGeneralPktList[enPktType].PktPara.GCPktPara.enAVMute << 1) | 0x0000);
				}
				else
				{
					//fill color depth information
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_GC12_04, 0x010F, gbGeneralPktList[enPktType].PktPara.GCPktPara.enColorDepInfo); // [8]: default phase = 0, [3:0]: Color depth

					if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
					{
					    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_GC_CFG_03, 0x007F, (gbGeneralPktList[enPktType].PktPara.GCPktPara.enAVMute << 1)| 0x21); // [6]: 0, DC and non-DC info send together
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_GCP_CMD);
						//MDrv_WriteByteMask(REG_HDMITX_03_L, 0x21, 0x7F);
					}
					else //cyclic
					{
						if (gbGeneralPktList[enPktType].EnableUserDef == TRUE)
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_GC_CFG_03, 0x007F, ((gbGeneralPktList[enPktType].PktPara.GCPktPara.enAVMute << 1) | (gbGeneralPktList[enPktType].FrmCntNum << 4) | 0x29));
						}
						else
						{
						    MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_GC_CFG_03, 0x007F, ((gbGeneralPktList[enPktType].PktPara.GCPktPara.enAVMute << 1) | (HDMITX_PACKET_GC_FCNT << 4) | 0x29));
						}
					}
				}
			break;

    		case E_HDMITX_ACP_PACKET:
				if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_STOP_PACKET) //stop packet
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACP_CFG_40, 0x0005, 0x0000);
				}
				else
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ACP_0_38, 0xFFFF, 0x0000); //acp type is 0x00

					if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
					{
					    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ACP_CFG_40, 0x0001);
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_ACP_CMD);
					}
					else //cyclic
					{
						if (gbGeneralPktList[enPktType].EnableUserDef == TRUE)
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ACP_CFG_40, ((gbGeneralPktList[enPktType].FrmCntNum << 3) | 0x0005));
						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ACP_CFG_40, (HDMITX_PACKET_ACP_FCNT | 0x0005));
						}
					}
				}
			break;

    		case E_HDMITX_ISRC1_PACKET:
				if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_STOP_PACKET) //stop packet
				{
					MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, 0x0005, 0x0000);
				}
				else
				{
					if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
					{
						MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, 0x0001);
                        MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_ISRC_CMD);
					}
					else //cyclic
					{
						if (gbGeneralPktList[enPktType].EnableUserDef == TRUE)
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, ((gbGeneralPktList[enPktType].FrmCntNum << 3) | 0x0005));						}
						else
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, ((HDMITX_PACKET_ISRC_FCNT << 3) | 0x0005));
						}
					}
				}
			break;

    		case E_HDMITX_ISRC2_PACKET:
				//check ISRC cnt value
				if (MHal_HDMITx_Read(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51) & 0x8000)
				{
					if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_STOP_PACKET) //stop packet
					{
						MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, 0x0005, 0x0000);
					}
					else
					{
					    MS_U8 u8ISRCCntVal = 0x80;

						if (gbGeneralPktList[enPktType].enPktCtrl == E_HDMITX_SEND_PACKET) //single
						{
						    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, (u8ISRCCntVal << 8) | 0x0001 ); // 0x80: ISRC_CONT = 1, ISRC1 & ISRC2
                            MHal_HDMITx_Mask_Write(HDMITX_REG_BASE, REG_ACT_HDMI_PKTS_CMD_01, 0x03FF, 1 << E_HDMITX_ACT_ISRC_CMD);
						}
						else //cyclic
						{
							if (gbGeneralPktList[enPktType].EnableUserDef == TRUE)
							{
							    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, ((u8ISRCCntVal << 8) | (gbGeneralPktList[enPktType].FrmCntNum << 3) | 0x0005));
							}
							else
							{
							    MHal_HDMITx_Write(HDMITX_REG_BASE, REG_PKT_ISRC_CFG_51, ((u8ISRCCntVal << 8) | (HDMITX_PACKET_ISRC_FCNT << 3) | 0x0005));
							}
						}
					}
				}
			break;

    		case E_HDMITX_DSD_PACKET:
				//TBD
			break;

    		case E_HDMITX_HBR_PACKET:
				//TBD
			break;

    		case E_HDMITX_GM_PACKET:
				//TBD
			break;

			default:
				printf("hal_HDMITx_SendPacket():: Invalid Packet Type!!\r\n");
			break;
		}
	}
}

void MHal_HDMITx_EnableSSC(MS_BOOL bEnable, MS_U32 uiTMDSCLK)
{
    //Maxim does not implement SSC yet.
    #if 0
    #define HDMITX_MPLL_CLK                        432 //432MHz
    #define HDMITX_SSC_CLK                          30 //30KHz
    #define HDMITX_SSC_DEVIATION              1 // 0.1%
    #define HDMITX_SSC_DEVIATION_DIVIDER      1000
    #define HDMITX_SSC_THREAD_LEVEL1      150 ///1080p 8bits
    #define HDMITX_SSC_THREAD_LEVEL2      300 ///4K30 8bits
    #define HDMITX_SSC_SPAN_REG                REG_HDMITxPHY_CONFIG_06
    #define HDMITX_SSC_STEP_REG                 REG_HDMITxPHY_CONFIG_07
    #define HDMITX_SSC_SUB_DIVIDER_REG   REG_HDMITxPHY_CONFIG_01

    MS_U32 ub2x19times = 524288;// 2^ 19
    MS_U32 dSYNCLK = 1;
    MS_U32 ub432MHz = HDMITX_MPLL_CLK;
    MS_U32 ubSSCClk = HDMITX_SSC_CLK;
    MS_U32 dSSc_Deviation= HDMITX_SSC_DEVIATION;
    MS_U32 ubSYNSet = 0;
    MS_U32 dSSC_Span = 0;
    MS_U32 dSSC_Step = 0;
    MS_U32 dPixel_Clk = 0;

    if(!bEnable)
    {
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_SPAN_REG, 0x3FFF, 0 );//Span
        MHal_HDMITx_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_STEP_REG, 0);//Step
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_SUB_DIVIDER_REG, 0x7000, 0 );//Sub-Divider
        return;
    }

    ubSYNSet = MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_02);
    ubSYNSet |= ( (MHal_HDMITx_Read(HDMITX_PHY_REG_BASE, REG_HDMITxPHY_CONFIG_03) & 0xFF) << 16);

    if(ubSYNSet != 0)
        dSYNCLK = ((ub432MHz*ub2x19times)/ubSYNSet);

    if(ubSSCClk != 0)
        dSSC_Span = (dSYNCLK * 250) / (ubSSCClk); //Span = SYN_CLK_KHz / (SSC_CLK_KHz * 4)

    if( (dSSC_Span != 0) && (HDMITX_SSC_DEVIATION_DIVIDER != 0) )
        dSSC_Step = (((ubSYNSet/ dSSC_Span)/HDMITX_SSC_DEVIATION_DIVIDER) * dSSc_Deviation) ; //Step = SYN_SET * deviation / Span

    printf("ubSYNSet=%x, dSYNCLK=%d, dSSC_Span=%d, dSSC_Step=%d\r\n", ubSYNSet, dSYNCLK, dSSC_Span, dSSC_Step);

    MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_SPAN_REG, 0x3FFF, ( ((int)dSSC_Span) & 0x3FFF) );//Span
    MHal_HDMITx_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_STEP_REG, ( ((int)dSSC_Step) & 0xFFFF) );//Step

    //Read tmds clock
    #if 0
    MHal_HDMITx_Mask_Write(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_20, 0x3F, 0x3F);
    dPixel_Clk = ((MHal_HDMITx_Read(HDMITX_MISC_REG_BASE, REG_MISC_CONFIG_21) << 1) * 12 / 128);
    #else
    dPixel_Clk = uiTMDSCLK;
    #endif
    printf("dPixel_Clk=%d\r\n", dPixel_Clk);

    if(dPixel_Clk < HDMITX_SSC_THREAD_LEVEL1)
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_SUB_DIVIDER_REG, 0x7000, 0x0000 );//Sub-Divider
    else if(dPixel_Clk < HDMITX_SSC_THREAD_LEVEL2)
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_SUB_DIVIDER_REG, 0x7000, 0x1000 );//Sub-Divider
    else
        MHal_HDMITx_Mask_Write(HDMITX_PHY_REG_BASE, HDMITX_SSC_SUB_DIVIDER_REG, 0x7000, 0x3000 );//Sub-Divider

    #endif
}

MS_U32 MHal_HDMITx_GetPixelClk_ByTiming(MsHDMITX_VIDEO_TIMING idx, MsHDMITX_VIDEO_COLOR_FORMAT color_fmt, MsHDMITX_VIDEO_COLORDEPTH_VAL color_depth)
{
    MS_U32 dwTMDSDataRate = 0;
    MS_U8 ubBitNum = 8;
    
    switch(color_depth)
    {
        case E_HDMITX_VIDEO_CD_NoID:
        case E_HDMITX_VIDEO_CD_24Bits:
        {
            ubBitNum = 8;
        }
        break;

        case E_HDMITX_VIDEO_CD_30Bits:
        {
            ubBitNum = 10;
        }
        break;

        case E_HDMITX_VIDEO_CD_36Bits:
        {
            ubBitNum = 12;
        }
        break;

        case E_HDMITX_VIDEO_CD_48Bits:
        {
            ubBitNum = 16;
        }
        break;

        default:
        {
            ubBitNum = 8;
        }
        break;
    }

    dwTMDSDataRate = (HDMITxVideoModeTbl[idx].pixel_clk/1000) * ubBitNum / 8;
    
    if(color_fmt == E_HDMITX_VIDEO_COLOR_YUV420)
        dwTMDSDataRate = dwTMDSDataRate/2;

    //printf("Time_ID = %d, C_FMT = %d, C_DEP = %d, PXL_CLK = %d\r\n", idx, color_fmt, color_depth, dwTMDSDataRate);
    return dwTMDSDataRate;
}

MS_U32 MHal_HDMITx_GetMaxPixelClk(void)
{
    return HDMITX_MAX_PIXEL_CLK/1000;
}
