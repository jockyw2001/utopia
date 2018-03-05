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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes.h"
#include "regSEAL.h"
#include "halSEAL.h"
#include "drvSEAL.h"
#include "halCHIP.h"

// For memcpy
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

static int _gSealLocalDelayVar;
#define __delay() {int i; for (i=0; i<100000; i+=2) {i=i-1; _gSealLocalDelayVar+=i;}}

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HAL_SEAL_DBG(x)                 //x
#define HAL_SEAL_ERR(x, args...)        {printf(x, ##args);}\

//Non PM IP group
#define SEAL_NON_PM_GROUP0 \
    E_SEAL_RIU_DBG_PROT_NONPM, \
    E_SEAL_VD_MHEG5_PROT_NONPM,  \
    E_SEAL_HIREG_PROT_NONPM,  \
    E_SEAL_POR_STATUS_PROT_NONPM, \
    E_SEAL_INTR_CPUINT_PROT_NONPM,  \
    E_SEAL_MIU2_PROT_NONPM,  \
    E_SEAL_USB0_PROT_NONPM, \
    E_SEAL_USB1_PROT_NONPM, \
    E_SEAL_UPLL0_PROT_NONPM, \
    E_SEAL_UPLL1_PROT_NONPM,  \
    E_SEAL_BDMA_CH0_PROT_NONPM, \
    E_SEAL_BDMA_CH1_PROT_NONPM, \
    E_SEAL_UART0_PROT_NONPM, \
    E_SEAL_CLKGEN2_PROT_NONPM, \
    E_SEAL_CLKGEN0_PROT_NONPM, \
    E_SEAL_MSPI1_PROT_NONPM

#define SEAL_NON_PM_GROUP1 \
    E_SEAL_UHC1_PROT_NONPM, \
    E_SEAL_ACP_PROT_NONPM, \
    E_SEAL_SPI_PRE_ARB_PROT_NONPM,  \
    E_SEAL_INTR_CTRL_CPU0_1_PROT_NONPM, \
    E_SEAL_INTR_CTRL_CPU1_1_PROT_NONPM, \
    E_SEAL_INTR_CTRL_CPU2_1_PROT_NONPM, \
    E_SEAL_INTR_CTRL_CPU3_1_PROT_NONPM,  \
    E_SEAL_MVD_PROT_NONPM, \
    E_SEAL_MIU_PROT_NONPM, \
    E_SEAL_MSPI0_PROT_NONPM, \
    E_SEAL_MVOP_PROT_NONPM, \
    E_SEAL_TSP0_PROT_NONPM, \
    E_SEAL_TSP1_PROT_NONPM, \
    E_SEAL_HVD1_PROT_NONPM, \
    E_SEAL_SEMAPH_PROT_NONPM, \
    E_SEAL_L3_AXI_PROT_NONPM

#define SEAL_NON_PM_GROUP2 \
    E_SEAL_INTR_CTRL_CPU0_0_PROT_NONPM, \
    E_SEAL_INTR_CTRL_CPU1_0_PROT_NONPM, \
    E_SEAL_INTR_CTRL_CPU2_0_PROT_NONPM,  \
    E_SEAL_INTR_CTRL_CPU3_0_PROT_NONPM, \
    E_SEAL_IDAC_NONPM, \
    E_SEAL_HVD_PROT_NONPM, \
    E_SEAL_EVD_NONPM, \
    E_SEAL_MCU_ARM_PROT_NONPM, \
    E_SEAL_CHIP_PROT_NONPM, \
    E_SEAL_GOP_PROT_NONPM, \
    E_SEAL_USB30PHY_DTOP_M_0_PROT_NONPM, \
    E_SEAL_USB30PHY_ATOP_M_0_PROT_NONPM, \
    E_SEAL_USB30PHY1_DTOP_M_0_PROT_NONPM, \
    E_SEAL_USB30PHY1_ATOP_M_0_NONPM, \
    E_SEAL_UHC0_PROT_NONPM, \
    E_SEAL_PADTOP0_PROT_NONPM

#define SEAL_NON_PM_GROUP3 \
    E_SEAL_EVD_1_PROT_NONPM, \
    E_SEAL_PADTOP1_PROT_NONPM,  \
    E_SEAL_GE0_PROT_NONPM, \
    E_SEAL_SMART_PROT_NONPM, \
    E_SEAL_SMART2_PROT_NONPM, \
    E_SEAL_SATA_GHC_PROT_NONPM, \
    E_SEAL_SATA_P0_PROT_NONPM, \
    E_SEAL_SATA_MISC_PROT_NONPM,  \
    E_SEAL_SC0_PROT_NONPM, \
    E_SEAL_SC1_PROT_NONPM, \
    E_SEAL_SC2_PROT_NONPM,  \
    E_SEAL_SC4_PROT_NONPM,  \
    E_SEAL_CLKGEN1_PROT_NONPM,  \
    E_SEAL_MAILBOX_PROT_NONPM,  \
    E_SEAL_PCM_PROT_NONPM,  \
    E_SEAL_SECMCU51_PROT_NONPM

#define SEAL_NON_PM_GROUP4 \
    E_SEAL_URDMA_PROT_NONPM,  \
    E_SEAL_SEC_R2_PROT_NONPM, \
    E_SEAL_SEC_MAU0_PROT_NONPM,  \
    E_SEAL_SATA_MIUPROT_PROT_NONPM, \
    E_SEAL_UTMISS0_PROT_NONPM, \
    E_SEAL_UTMISS1_PROT_NONPM,  \
    E_SEAL_SATA_PHY_PROT_NONPM,  \
    E_SEAL_UTMI1_PROT_NONPM, \
    E_SEAL_UTMI_PROT_NONPM, \
    E_SEAL_VE_0_PROT_NONPM, \
    E_SEAL_REG_PIU_NONPM_PROT_NONPM, \
    E_SEAL_MVOPSUB_PROT_NONPM,  \
    E_SEAL_VE_1_PROT_NONPM,  \
    E_SEAL_VE_2_PROT_NONPM,  \
    E_SEAL_INTR_CTRL_CPU4_0_PROT_NONPM, \
    E_SEAL_INTR_CTRL_CPU4_1_PROT_NONPM

#define SEAL_NON_PM_GROUP5 \
    E_SEAL_UART1_PROT_NONPM,  \
    E_SEAL_FUART_PROT_NONPM, \
    E_SEAL_GE1_PROT_NONPM,  \
    E_SEAL_GPU_NONPM, \
    E_SEAL_ANA_MISC_EVDPLL_MIUPLL_PROT_NONPM, \
    E_SEAL_ANA_MISC_MIPSPLL_PROT_NONPM,  \
    E_SEAL_ANA_MISC_MPLLTOP_PROT_NONPM, \
    E_SEAL_ANA_MISC_GMAC_PROT_NONPM, \
    E_SEAL_MIU_ATOP_PROT_NONPM, \
    E_SEAL_GPD0_PROT_NONPM, \
    E_SEAL_GPD1_PROT_NONPM, \
    E_SEAL_MFE0_PROT_NONPM, \
    E_SEAL_MFE1_PROT_NONPM, \
    E_SEAL_NFIE0_PROT_NONPM, \
    E_SEAL_NFIE1_PROT_NONPM, \
    E_SEAL_NFIE2_PROT_NONPM

#define SEAL_NON_PM_GROUP6 \
    E_SEAL_OTG0_NONPM, \
    E_SEAL_OTG1_NONPM, \
    E_SEAL_OTG2_NONPM, \
    E_SEAL_OTG3_NONPM, \
    E_SEAL_RASP0_PROT_NONPM, \
    E_SEAL_RASP1_PROT_NONPM, \
    E_SEAL_RASP2_PROT_NONPM,  \
    E_SEAL_RASP3_PROT_NONPM, \
    E_SEAL_SDIO0_PROT_NONPM, \
    E_SEAL_SDIO1_PROT_NONPM, \
    E_SEAL_SDIO2_PROT_NONPM, \
    E_SEAL_MFDEC_PROT_NONPM, \
    E_SEAL_RASP0_FILE_PROT_NONPM, \
    E_SEAL_RASP1_FILE_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY

#define SEAL_NON_PM_GROUP7 \
    E_SEAL_AU_R2_PROT_NONPM, \
    E_SEAL_VIVALDI0_PROT_NONPM, \
    E_SEAL_VIVALDI1_PROT_NONPM, \
    E_SEAL_VIVALDI2_PROT_NONPM, \
    E_SEAL_VIVALDI3_PROT_NONPM, \
    E_SEAL_VIVALDI4_PROT_NONPM, \
    E_SEAL_VIVALDI5_PROT_NONPM, \
    E_SEAL_VIVALDIc_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_CLKGEN_AUR2_0_PROT_NONPM, \
    E_SEAL_PATGEN_AUR2_0_PROT_NONPM, \
    E_SEAL_CLKGEN_AUR2_1_PROT_NONPM, \
    E_SEAL_PATGEN_AUR2_1_PROT_NONPM, \
    E_SEAL_MIIC0_PROT_NONPM, \
    E_SEAL_MIIC1_PROT_NONPM

#define SEAL_NON_PM_GROUP8 \
    E_SEAL_MIIC2_PROT_NONPM, \
    E_SEAL_USBBC0_PROT_NONPM, \
    E_SEAL_USBBC1_PROT_NONPM, \
    E_SEAL_USBBC2_PROT_NONPM, \
    E_SEAL_USB3_BC0_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_MIIC3_PROT_NONPM, \
    E_SEAL_MIIC4_PROT_NONPM, \
    E_SEAL_TSP2_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_GOP0G_0_PROT_NONPM, \
    E_SEAL_GOP0G_1_PROT_NONPM, \
    E_SEAL_GOP0G_ST_PROT_NONPM, \
    E_SEAL_GOP1G_0_PROT_NONPM, \
    E_SEAL_GOP1G_1_PROT_NONPM, \
    E_SEAL_GOP1G_ST_PROT_NONPM

#define SEAL_NON_PM_GROUP9 \
    E_SEAL_GOP2G_0_PROT_NONPM, \
    E_SEAL_GOP2G_1_PROT_NONPM, \
    E_SEAL_GOP2G_ST_PROT_NONPM, \
    E_SEAL_GOP3G_0_PROT_NONPM, \
    E_SEAL_GOP3G_1_PROT_NONPM, \
    E_SEAL_GOP3G_ST_PROT_NONPM, \
    E_SEAL_GOPMIX_PROT_NONPM, \
    E_SEAL_GOP_AFBC_PROT_NONPM, \
    E_SEAL_DAC_PLL_PROT_NONPM, \
    E_SEAL_MCU_ARM_PMU0_PROT_NONPM, \
    E_SEAL_GOPMIX_SD_PROT_NONPM, \
    E_SEAL_GOP5G_0_PROT_NONPM, \
    E_SEAL_GOP5G_1_PROT_NONPM, \
    E_SEAL_GOP5G_ST_PROT_NONPM, \
    E_SEAL_TZPC_NONPM2_NONPM, \
    E_SEAL_GMAC0_PROT_NONPM

#define SEAL_NON_PM_GROUP10 \
    E_SEAL_GMAC1_PROT_NONPM, \
    E_SEAL_GMAC2_PROT_NONPM, \
    E_SEAL_GMAC3_PROT_NONPM, \
    E_SEAL_GMAC4_PROT_NONPM, \
    E_SEAL_SECGMAC0_PROT_NONPM, \
    E_SEAL_SECGMAC1_PROT_NONPM, \
    E_SEAL_SECGMAC2_PROT_NONPM, \
    E_SEAL_SECGMAC3_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_VE_GAMA_NONPM, \
    E_SEAL_TZPC_MIU0_PROT_NONPM, \
    E_SEAL_TZPC_MIU1_PROT_NONPM, \
    E_SEAL_JPD_PROT_NONPM, \
    E_SEAL_ZDEC_TOP_PROT_NONPM, \
    E_SEAL_CMDQ_PROT_NONPM, \
    E_SEAL_SECURERANGE0_PROT_NONPM

#define SEAL_NON_PM_GROUP11 \
    E_SEAL_SECURERANGE1_PROT_NONPM, \
    E_SEAL_TZPC_NONPM_PROT_NONPM, \
    E_SEAL_TZPC_HOSTID_NONPM_PROT_NONPM, \
    E_SEAL_IMONITOR_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_EMMC_PLL_PROT_NONPM, \
    E_SEAL_MSC_PROT_NONPM, \
    E_SEAL_USB3TOP_PROT_NONPM, \
    E_SEAL_USB30HS_UHC_PROT_NONPM, \
    E_SEAL_USB30HS_USBC_PROT_NONPM, \
    E_SEAL_PCIE_MAC_PROT_NONPM, \
    E_SEAL_PCIE_AXI2MI_BRI_PROT_NONPM, \
    E_SEAL_USB30_SS_MIUPROT0_PROT_NONPM, \
    E_SEAL_USB30_HS0_MIUPROT1_PROT_NONPM, \
    E_SEAL_MIU_ARBB_NONPM, \
    E_SEAL_MIU_ARBB2_NONPM

#define SEAL_NON_PM_GROUP12 \
    E_SEAL_MIU_DIG_E_PROT_NONPM, \
    E_SEAL_MIU_DIG_E2_PROT_NONPM, \
    E_SEAL_MIU_PROTECT_PROT_NONPM, \
    E_SEAL_MIU_PROTECT2_PROT_NONPM, \
    E_SEAL_TSO2_PROT_NONPM, \
    E_SEAL_VIVALDId_PROT_NONPM, \
    E_SEAL_VIVALDIe_PROT_NONPM, \
    E_SEAL_VIVALDIf_PROT_NONPM, \
    E_SEAL_VIVALDI6_PROT_NONPM, \
    E_SEAL_VIVALDI7_PROT_NONPM, \
    E_SEAL_GPU_PLL_PROT_NONPM, \
    E_SEAL_DRM_SECURE_PROT_NONPM, \
    E_SEAL_PCM1_PROT_NONPM, \
    E_SEAL_FI_QUEUE_PROT_NONPM, \
    E_SEAL_CLKDET_PROT_NONPM, \
    E_SEAL_EMACMIUPROT_PROT_NONPM

#define SEAL_NON_PM_GROUP13 \
    E_SEAL_GPU2MIU_MASK_PROT_NONPM, \
    E_SEAL_BYTE_WR_PROT_NONPM, \
    E_SEAL_ONEWAY_PROT_NONPM, \
    E_SEAL_TSP6_PROT_NONPM, \
    E_SEAL_TSP7_PROT_NONPM, \
    E_SEAL_TSO1_NONPM, \
    E_SEAL_CA_MIUCROSSBAR_PROT_NONPM, \
    E_SEAL_EMM_FLT1_PROT_NONPM, \
    E_SEAL_MIU_ARB_NONPM, \
    E_SEAL_MIU_ATOP2_PROT_NONPM, \
    E_SEAL_MIU_ARB2_NONPM, \
    E_SEAL_TSP8_PROT_NONPM, \
    E_SEAL_GE2_NONPM, \
    E_SEAL_GE3_NONPM, \
    E_SEAL_AU_R2_1_NONPM, \
    E_SEAL_DUMMY

#define SEAL_NON_PM_GROUP14 \
    E_SEAL_CALB_PROT_NONPM, \
    E_SEAL_PKA_PROT_NONPM, \
    E_SEAL_PKA_BYTE2WORD_PROT_NONPM, \
    E_SEAL_VIVALDI8_PROT_NONPM, \
    E_SEAL_VIVALDI9_PROT_NONPM, \
    E_SEAL_VIVALDIa_0_PROT_NONPM, \
    E_SEAL_VIVALDIa_1_PROT_NONPM, \
    E_SEAL_VIVALDIa_2_PROT_NONPM, \
    E_SEAL_VIVALDIa_3_PROT_NONPM, \
    E_SEAL_VIVALDIa_4_PROT_NONPM, \
    E_SEAL_VIVALDIa_5_PROT_NONPM, \
    E_SEAL_VIVALDIb_0_PROT_NONPM, \
    E_SEAL_VIVALDIb_1_PROT_NONPM, \
    E_SEAL_VIVALDIb_2_PROT_NONPM, \
    E_SEAL_VIVALDIb_3_PROT_NONPM, \
    E_SEAL_TSP3_PROT_NONPM

#define SEAL_NON_PM_GROUP15 \
    E_SEAL_TSP4_PROT_NONPM, \
    E_SEAL_CA_PWD_PROT_NONPM, \
    E_SEAL_EMM_FLT0_PROT_NONPM, \
    E_SEAL_TSO0_PROT_NONPM, \
    E_SEAL_KC_DESC_PROT_NONPM, \
    E_SEAL_SEC_R2_CPU_PROT_NONPM, \
    E_SEAL_TS_SAMPLE_NONPM, \
    E_SEAL_OTV01_PROT_NONPM, \
    E_SEAL_OTV23_PROT_NONPM, \
    E_SEAL_TSP9_PROT_NONPM, \
    E_SEAL_TSP10_PROT_NONPM, \
    E_SEAL_USBMIUPROT0_PROT_NONPM, \
    E_SEAL_USBMIUPROT1_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_DIG_TOP_PROT_NONPM, \
    E_SEAL_PATGEN_ROOT_PROT_NONPM

#define SEAL_NON_PM_GROUP16 \
    E_SEAL_CODEC_BLOCK_PROT_NONPM, \
    E_SEAL_PATGEN_CODEC_PROT_NONPM, \
    E_SEAL_HI_CODEC_BLOCK_PROT_NONPM, \
    E_SEAL_PATGEN_HI_CODEC_BLOCK_PROT_NONPM, \
    E_SEAL_SC_BLOCK_PROT_NONPM, \
    E_SEAL_PATGEN_SC_PROT_NONPM, \
    E_SEAL_TSP_BLOCK_PROT_NONPM, \
    E_SEAL_PATGEN_TSP_PROT_NONPM, \
    E_SEAL_VIVALDI9_BLOCK_PROT_NONPM, \
    E_SEAL_PATGEN_VIVALDI9_PROT_NONPM, \
    E_SEAL_GPU_BLOCK_PROT_NONPM, \
    E_SEAL_PATGEN_GPU_PROT_NONPM, \
    E_SEAL_PATGEN_GPU_DIE_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_TSIO0_PROT_NONPM

#define SEAL_NON_PM_GROUP17 \
    E_SEAL_TSIO1_PROT_NONPM, \
    E_SEAL_TSIO2_PROT_NONPM, \
    E_SEAL_TSO3_PROT_NONPM, \
    E_SEAL_TSIO_LOC_DEC_PROT_NONPM, \
    E_SEAL_TSIO_PHY_PROT_NONPM, \
    E_SEAL_DVI_DTOP_DUAL_P0_PROT_NONPM, \
    E_SEAL_DVI_RSV_DUAL_P0_PROT_NONPM, \
    E_SEAL_HDCP_DUAL_P0_PROT_NONPM, \
    E_SEAL_HDMI_DUAL_0_PROT_NONPM, \
    E_SEAL_HDMI2_DUAL_0_PROT_NONPM, \
    E_SEAL_HDCPKEY_PROT_NONPM, \
    E_SEAL_COMBO_GP_TOP_PROT_NONPM, \
    E_SEAL_SECURE_TZPC_PROT_NONPM, \
    E_SEAL_COMBO_PHY0_P0_PROT_NONPM, \
    E_SEAL_COMBO_PHY1_P0_PROT_NONPM, \
    E_SEAL_HDMITX_PROT_NONPM

#define SEAL_NON_PM_GROUP18 \
    E_SEAL_HDCPTX_PROT_NONPM, \
    E_SEAL_HDMITX_PROT_NONPM, \
    E_SEAL_HDMITX_VIDEO_PROT_NONPM, \
    E_SEAL_HDMITX_AUDIO_PROT_NONPM, \
    E_SEAL_HDCP22TX_PROT_NONPM, \
    E_SEAL_HDMITX_PHY_PROT_NONPM, \
    E_SEAL_VMX_VMARK_SC0_PROT_NONPM, \
    E_SEAL_VMX_VMARK_SC1_PROT_NONPM, \
    E_SEAL_HDMI3_DUAL_0_PROT_NONPM, \
    E_SEAL_VD_R2_SUBSYS_PROT_NONPM, \
    E_SEAL_HDMITX2_PROT_NONPM, \
    E_SEAL_TSIO3_PROT_NONPM, \
    E_SEAL_X32_NSK_NONPM, \
    E_SEAL_X32_BRIDGE_NONPM, \
    E_SEAL_X32_CIPHERCH0_NONPM, \
    E_SEAL_X32_CIPHERCH1_NONPM

#define SEAL_NON_PM_GROUP19 \
    E_SEAL_X32_TSCEBANK0_NONPM, \
    E_SEAL_X32_TSCEBANK1_NONPM, \
    E_SEAL_X32_CRYPTODMA0_NONPM, \
    E_SEAL_X32_CRYPTODMA1_NONPM, \
    E_SEAL_X32_KEYTABLE_NONPM, \
    E_SEAL_X32_PIDSLOTMTN_PROT_NONPM, \
    E_SEAL_X32_KEYLADDER0_NONPM, \
    E_SEAL_X32_KEYLADDER0_1_NONPM, \
    E_SEAL_X32_KEYLADDER1_NONPM, \
    E_SEAL_X32_KEYLADDER2_NONPM, \
    E_SEAL_X32_PROGPVR0_NONPM, \
    E_SEAL_X32_PROGPVR1_NONPM, \
    E_SEAL_X32_PROGPVR2_NONPM, \
    E_SEAL_X32_PROGPVR3_NONPM, \
    E_SEAL_X32_RSA_NONPM, \
    E_SEAL_X32_MAILBOX_NONPM

#define SEAL_NON_PM_GROUP20 \
    E_SEAL_X32_OTP_CTRL_NONPM, \
    E_SEAL_X32_OTP_PUB0_NONPM, \
    E_SEAL_X32_OTP_PUB1_NONPM, \
    E_SEAL_X32_OTP_PUB2_NONPM, \
    E_SEAL_X32_OTP_PUB3_NONPM, \
    E_SEAL_X32_NI_NONPM, \
    E_SEAL_X32_AKL0_NONPM, \
    E_SEAL_X32_AKL1_NONPM, \
    E_SEAL_X32_XIUGEN_NONPM, \
    E_SEAL_X32_RNG_PROT_NONPM, \
    E_SEAL_X32_CA_MPROT_NONPM, \
    E_SEAL_X32_CA_RPROT_NONPM, \
    E_SEAL_X32_GMAC0_NONPM, \
    E_SEAL_X32_GMAC1_NONPM, \
    E_SEAL_X32_GMAC2_NONPM, \
    E_SEAL_X32_EMAC0_NONPM

#define SEAL_NON_PM_GROUP21 \
    E_SEAL_X32_EMAC1_NONPM, \
    E_SEAL_X32_EMAC2_NONPM, \
    E_SEAL_X32_GPUAPB0_PROT_NONPM, \
    E_SEAL_X32_GPUAPB1_PROT_NONPM, \
    E_SEAL_X32_GPUAPB2_PROT_NONPM, \
    E_SEAL_X32_PCIE_MAC_PROT_NONPM, \
    E_SEAL_X32_USB3XHCI_PROT_NONPM, \
    E_SEAL_X32_SATA_MAC_PROT_NONPM, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY

#define SEAL_NON_PM_GROUP22 \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY

#define SEAL_NON_PM_GROUP23 \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_TSP_NON_SECURE_PROT_NONPM, \
    E_SEAL_TSP_FILEIN_ILLEGAL_PROT_NONPM, \
    E_SEAL_TSP_SEC_PROT_NONPM, \
    E_SEAL_TSP_OR_PROT_NONPM, \
    E_SEAL_TSP_VQ_PROT_NONPM, \
    E_SEAL_TSP_PVR4_PROT_NONPM, \
    E_SEAL_DEMOD_DMDMCU51_MIU_PROT_NONPM, \
    E_SEAL_ARM2MIU_SECURE_PROT_NONPM, \
    E_SEAL_ARM2MIU_NON_SECURE_PROT_NONPM, \
    E_SEAL_PM51_TO_MIU_PROT_NONPM, \
    E_SEAL_CRYPTODMA_SEC_51_PROT_NONPM, \
    E_SEAL_CRYPTODMA_SEC_R2_PROT_NONPN, \
    E_SEAL_CRYPTODMA_GEN_NON_SEcure_PROT_NONPM, \
    E_SEAL_CRYPTODMA_GEN_SECURE_PROT_NONPM

//PM IP group
#define SEAL_PM_GROUP0 \
    E_SEAL_RIU_DBG_PROT_PM, \
    E_SEAL_MENULOAD_PROT_PM, \
    E_SEAL_DUMMY,\
    E_SEAL_GDMA_PROT_PM, \
    E_SEAL_DDC_PROT_PM, \
    E_SEAL_PM_POR_PROT_PM, \
    E_SEAL_ISP_PROT_PM, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_MIIC_PM0_PROT_PM, \
    E_SEAL_MSPI0_PROT_PM, \
    E_SEAL_PM_SLEEP_PROT_PM, \
    E_SEAL_PM_GPIO_PROT_PM, \
    E_SEAL_MCU_PROT_PM, \
    E_SEAL_PM_CEC_PROT_PM, \
    E_SEAL_PM_RTC0_PROT_PM


#define SEAL_PM_GROUP1 \
    E_SEAL_PM_RTC1_PROT_PM, \
    E_SEAL_PM_SAR_PROT_PM, \
    E_SEAL_DUMMY, \
    E_SEAL_FSP_PROT_PM, \
    E_SEAL_QSPI_PROT_PM, \
    E_SEAL_PM_RTC2_PROT_PM, \
    E_SEAL_PM_TOP_PROT_PM, \
    E_SEAL_DUMMY, \
    E_SEAL_STR_DATA_PM_PROT_PM, \
    E_SEAL_ONEWAY_PROT_PM, \
    E_SEAL_IRQ_PROT_PM, \
    E_SEAL_CACHE_PROT_PM, \
    E_SEAL_XDMIU_PROT_PM, \
    E_SEAL_DUMMY, \
    E_SEAL_WDT_PROT_PM, \
    E_SEAL_TIMER0_PROT_PM

#define SEAL_PM_GROUP2 \
    E_SEAL_TIMER1_PROT_PM, \
    E_SEAL_TIMER2_PROT_PM, \
    E_SEAL_ALBANY0_PROT_PM, \
    E_SEAL_ALBANY1_PROT_PM, \
    E_SEAL_ALBANY2_PROT_PM, \
    E_SEAL_DUMMY, \
    E_SEAL_REG_PIU_MISC_0_PROT_PM, \
    E_SEAL_IR_PROT_PM, \
    E_SEAL_SEC_PROT_PM, \
    E_SEAL_PM_PATGEN_PROT_PM, \
    E_SEAL_PM_DIG_TOP_PROT_PM, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY

#define SEAL_PM_GROUP3 \
    E_SEAL_SCDC_0_PROT_PM, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY, \
    E_SEAL_DUMMY

//processor group
#define SEAL_PROCESSOR_GROUP0 \
    E_SEAL_DBBUS, \
    E_SEAL_MCU51, \
    E_SEAL_CPU2, \
    E_SEAL_VD_R2, \
    E_SEAL_SECURE_R2, \
    E_SEAL_SC, \
    E_SEAL_CMDQ, \
    E_SEAL_HEMCU ,\
    E_SEAL_PROCESSOR_NUM,\

//lock group
#define SEAL_LOCK_GROUP_DUMMY \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY

#define SEAL_LOCK_GROUP0 \
    E_SEAL_SC_WP_DIPW, \
    E_SEAL_SC_RP_DIPW, \
    E_SEAL_SC_WP_DIPR, \
    E_SEAL_SC_RP_DIPR, \
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_0, \
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_1, \
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_2, \
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_3, \
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_0, \
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_1, \
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_2, \
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_3, \
    E_SEAL_SECURE_R2_WR_PROTN_0, \
    E_SEAL_SECURE_R2_WR_PROTN_1, \
    E_SEAL_SECURE_R2_WR_PROTN_2, \
    E_SEAL_SECURE_R2_WR_PROTN_3

#define SEAL_LOCK_GROUP1 \
    E_SEAL_CIPHERENG_WP_SYSKEY, \
    E_SEAL_CIPHERENG_RP_SYSKEY, \
    E_SEAL_DSCRMB_WP_RIV0, \
    E_SEAL_DSCRMB_RP_RIV0, \
    E_SEAL_DSCRMB_WP_RIV1, \
    E_SEAL_DSCRMB_RP_RIV1, \
    E_SEAL_DSCRMB_WP_WDATA, \
    E_SEAL_DSCRMB_RP_WDATA, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_MVOP_WP_TLB, \
    E_SEAL_AUDIO_PAS_PROTN

#define SEAL_LOCK_GROUP2 \
    E_SEAL_DSCRMB_RP_RDATA, \
    E_SEAL_CRI_KL_WP_N, \
    E_SEAL_PCIERC_IBWP_PROTN, \
    E_SEAL_PCIERC_OBWP_PROTN, \
    E_SEAL_RNG_PROTECT, \
    E_SEAL_RSA_PROTECT, \
    E_SEAL_SHA_PROTECT, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_EVD_R2_WR_PROTN_0, \
    E_SEAL_EVD_R2_WR_PROTN_1, \
    E_SEAL_EVD_R2_WR_PROTN_2, \
    E_SEAL_EVD_R2_WR_PROTN_3, \
    E_SEAL_HVD_R2_WR_PROTN_0, \
    E_SEAL_HVD_R2_WR_PROTN_1, \
    E_SEAL_HVD_R2_WR_PROTN_2, \
    E_SEAL_HVD_R2_WR_PROTN_3

#define SEAL_LOCK_GROUP3 \
    E_SEAL_HVD_ES0_BUF, \
    E_SEAL_HVD_ES1_BUF, \
    E_SEAL_HVD_ES2_BUF, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_MFE0_ES_BUF, \
    E_SEAL_MVD_WR_PROTN_0, \
    E_SEAL_MVD_WR_PROTN_1, \
    E_SEAL_MVD_WR_PROTN_2, \
    E_SEAL_EVD_0_WR_PROTN_0, \
    E_SEAL_EVD_1_WR_PROTN_0, \
    E_SEAL_MHEG5_WR_PROTN_0, \
    E_SEAL_AUDIO_DSP_ES_PROTN, \
    E_SEAL_AUDIO_DSP_CACHE_PROTN, \
    E_SEAL_EVD_0_WR_PROTN_1, \
    E_SEAL_EVD_1_WR_PROTN_1, \
    E_SEAL_VP9_TOP

#define SEAL_LOCK_GROUP4 \
    E_SEAL_TSO_WP_TSOFI, \
    E_SEAL_TSO_RP_TSOFI, \
    E_SEAL_MMFI_WP_MMFI0, \
    E_SEAL_MMFI_RP_MMFI0, \
    E_SEAL_MMFI_WP_MMFI1, \
    E_SEAL_MMFI_RP_MMFI1, \
    E_SEAL_TSP0_WP_PVR, \
    E_SEAL_TSP0_RP_PVR, \
    E_SEAL_TSP0_WP_PVR1, \
    E_SEAL_TSP0_RP_PVR1, \
    E_SEAL_TSP0_WP_FILEIN, \
    E_SEAL_TSP0_RP_FILEIN, \
    E_SEAL_TSP0_WP_QMEM, \
    E_SEAL_TSP0_RP_QMEM, \
    E_SEAL_TSP0_WP_FW, \
    E_SEAL_TSP0_RP_FW

#define SEAL_LOCK_GROUP5 \
    E_SEAL_VE_WP, \
    E_SEAL_VE_RP, \
    E_SEAL_SC_WP_OD, \
    E_SEAL_SC_RP_OD, \
    E_SEAL_SC_WP_SCM_M, \
    E_SEAL_SC_RP_SCM_M, \
    E_SEAL_SC_WP_SCM_S, \
    E_SEAL_SC_RP_SCM_S, \
    E_SEAL_SC_WP_PDW0, \
    E_SEAL_SC_RP_PDW0, \
    E_SEAL_SC_WP_PDW1, \
    E_SEAL_SC_RP_PDW1, \
    E_SEAL_SC_WP_OPW, \
    E_SEAL_SC_RP_OPW, \
    E_SEAL_GOPD_PROTN, \
    E_SEAL_AUDIO_AL_PROTN

#define SEAL_LOCK_GROUP6 \
    E_SEAL_GE0_SB_PROTN, \
    E_SEAL_GE0_DB_PROTN, \
    E_SEAL_GE1_SB_PROTN, \
    E_SEAL_GE1_DB_PROTN, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY

#define SEAL_LOCK_GROUP7 \
    E_SEAL_TSP_WP_FILEIN1, \
    E_SEAL_TSP_RP_FILEIN1, \
    E_SEAL_TSP_WP_PVR3, \
    E_SEAL_TSP_RP_PVR3, \
    E_SEAL_TSP_WP_FIQ0, \
    E_SEAL_TSP_RP_FIQ0, \
    E_SEAL_TSP_WP_PVR4, \
    E_SEAL_TSP_RP_PVR4, \
    E_SEAL_TSP_WP_FILEIN2, \
    E_SEAL_TSP_RP_FILEIN2, \
    E_SEAL_TSP_WP_FILEIN3, \
    E_SEAL_TSP_RP_FILEIN3, \
    E_SEAL_TSP_WP_FILEIN4, \
    E_SEAL_TSP_RP_FILEIN4, \
    E_SEAL_TSP_WP_FILEIN5, \
    E_SEAL_TSP_RP_FILEIN5

#define SEAL_LOCK_GROUP8 \
    E_SEAL_USB_MIUPROT_WP_N, \
    E_SEAL_EMAC_MIUPROT_WP_N, \
    E_SEAL_PCIE_MIUPROT_WP_N, \
    E_SEAL_AU_HDMI_DMA_WP_N, \
    E_SEAL_TSP_WP_FIQ1, \
    E_SEAL_TSP_RP_FIQ1, \
    E_SEAL_TSP_WP_ORZ, \
    E_SEAL_TSP_RP_ORZ, \
    E_SEAL_TSP_WP_VQ, \
    E_SEAL_TSP_RP_VQ, \
    E_SEAL_TSP_WP_SEC, \
    E_SEAL_TSP_RP_SEC, \
    E_SEAL_TSP_WP_FIQ2, \
    E_SEAL_TSP_RP_FIQ2, \
    E_SEAL_TSP_WP_FIQ3, \
    E_SEAL_TSP_RP_FIQ3

#define SEAL_LOCK_GROUP9 \
    E_SEAL_EVD_LITE_0_WR_PROTN_0, \
    E_SEAL_EVD_LITE_1_WR_PROTN_1, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY

#define SEAL_LOCK_GROUP10 \
    E_SEAL_FCIE_WR_PROT_ENABLE_0, \
    E_SEAL_FCIE_WR_PROT_ENABLE_1, \
    E_SEAL_FCIE_WR_PROT_ENABLE_2, \
    E_SEAL_FCIE_WR_PROT_ENABLE_3, \
    E_SEAL_HDCP_RIU_R_PROTN, \
    E_SEAL_HDCP_XIU_R_PROTN, \
    E_SEAL_HDCP_RIU_W_PROTN, \
    E_SEAL_HDCP_XIU_W_PROTN, \
    E_SEAL_FCIE_WR_PROT_0_LOCK, \
    E_SEAL_FCIE_WR_PROT_1_LOCK, \
    E_SEAL_FCIE_WR_PROT_2_LOCK, \
    E_SEAL_FCIE_WR_PROT_3_LOCK, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY, \
    E_SEAL_LOCK_DUMMY


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
const eSeal_IP NonPmIpTbl[SEAL_NONPM_TBL_IP_NUM] =
{
    SEAL_NON_PM_GROUP0, SEAL_NON_PM_GROUP1, SEAL_NON_PM_GROUP2,
    SEAL_NON_PM_GROUP3, SEAL_NON_PM_GROUP4, SEAL_NON_PM_GROUP5,
    SEAL_NON_PM_GROUP6, SEAL_NON_PM_GROUP7, SEAL_NON_PM_GROUP8,
    SEAL_NON_PM_GROUP9, SEAL_NON_PM_GROUP10, SEAL_NON_PM_GROUP11,
    SEAL_NON_PM_GROUP12, SEAL_NON_PM_GROUP13, SEAL_NON_PM_GROUP14,
    SEAL_NON_PM_GROUP15, SEAL_NON_PM_GROUP16, SEAL_NON_PM_GROUP17,
    SEAL_NON_PM_GROUP18, SEAL_NON_PM_GROUP19, SEAL_NON_PM_GROUP20,
    SEAL_NON_PM_GROUP21, SEAL_NON_PM_GROUP22, SEAL_NON_PM_GROUP23
};

const eSeal_IP PmIpTbl[SEAL_PM_TBL_IP_NUM] =
{
    SEAL_PM_GROUP0,
    SEAL_PM_GROUP1,
    SEAL_PM_GROUP2,
    SEAL_PM_GROUP3
};

const eSeal_ProcessorId ProcessorTbl[SEAL_TBL_PROCESSOR_NUM] =
{
    SEAL_PROCESSOR_GROUP0
};

const eSeal_Lock LockTbl[SEAL_TBL_LOCK_NUM] =
{
    SEAL_LOCK_GROUP10, /*0x64*/
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP_DUMMY,
    SEAL_LOCK_GROUP0, /*0x74*/
    SEAL_LOCK_GROUP1,
    SEAL_LOCK_GROUP2,
    SEAL_LOCK_GROUP3,
    SEAL_LOCK_GROUP4,
    SEAL_LOCK_GROUP5,
    SEAL_LOCK_GROUP6,
    SEAL_LOCK_GROUP7,
    SEAL_LOCK_GROUP8,
    SEAL_LOCK_GROUP9
};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _gSEAL_NonPmMapBase = 0;
static MS_VIRT _gSEAL_PmMapBase = 0;
static SRAM_TZPC_NSGroup *_gSEAL_NS0GroupBase;
static SRAM_TZPC_NSGroup *_gSEAL_NS1GroupBase;

MS_U8 DefaultNonSecClient[16] =
{
    0x1, 0x3, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0
};

MS_U8 DefaultSecClient[16] =
{
    0x0, 0x3, 0xFF, 0xFF,
    0xFF, 0xFF, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0
};

#define SET_SECURE_CLIENT(client) (memcpy((void*)(&(client)), (void*)DefaultSecClient, 16))
#define SET_NONSECURE_CLIENT(client) (memcpy((void*)(&(client)), (void*)DefaultNonSecClient, 16))

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_SEAL_NonPmReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gSEAL_NonPmMapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_PmReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_SEAL_PmReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gSEAL_PmMapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmRead2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SEAL_NonPmRead2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gSEAL_NonPmMapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_PmRead2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SEAL_PmRead2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gSEAL_PmMapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmRead4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_SEAL_NonPmRead4Byte(MS_U32 u32RegAddr)
{
    return (HAL_SEAL_NonPmRead2Byte(u32RegAddr) | HAL_SEAL_NonPmRead2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmRead6Byte
/// @brief \b Function  \b Description: read 6 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U64
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U64 HAL_SEAL_NonPmRead6Byte(MS_U32 u32RegAddr)
{
    MS_U64 u64HitAddr;

    u64HitAddr = 0;
    u64HitAddr |= (MS_U64)HAL_SEAL_NonPmRead2Byte(u32RegAddr);
    u64HitAddr |= (MS_U64)HAL_SEAL_NonPmRead2Byte(u32RegAddr + 2) << 16;
    u64HitAddr |= (MS_U64)(HAL_SEAL_NonPmRead2Byte(u32RegAddr + 4)) << 32;

    return u64HitAddr;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmWriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SEAL_NonPmWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SEAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gSEAL_NonPmMapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_PmWriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SEAL_PmWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SEAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gSEAL_PmMapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmWrite2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SEAL_NonPmWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        HAL_SEAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gSEAL_NonPmMapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_PmWrite2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SEAL_PmWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        HAL_SEAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gSEAL_PmMapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmWrite4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SEAL_NonPmWrite4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        HAL_SEAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmWrite6Byte
/// @brief \b Function  \b Description: write 6 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u64Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SEAL_NonPmWrite6Byte(MS_U32 u32RegAddr, MS_U64 u64Val)
{
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u64Val & 0xFFFF);
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr + 2,(u64Val >> 16) & 0xFFFF);
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr + 4,(u64Val >> 32) & 0xFFFF);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmWriteRegBit
/// @brief \b Function  \b Description: set or reset one bit in 1-byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Mask: 1-byte mask
/// @param <IN>         \b bEnabl: decide if set or reset bits
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SEAL_NonPmWriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable)
{
    MS_U8 u8Val = HAL_SEAL_NonPmReadByte(u32RegAddr);
    if (!u32RegAddr)
    {
        HAL_SEAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SEAL_NonPmReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_SEAL_NonPmWriteByte(u32RegAddr, u8Val);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_NonPmWrite2BytesBit
/// @brief \b Function  \b Description: set or reset one bit in 2-byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Mask: 2-byte mask
/// @param <IN>         \b bEnabl: decide if set or reset bits
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SEAL_NonPmWrite2BytesBit(MS_U32 u32RegOffset, MS_BOOL bEnable, MS_U16 u16Mask)
{
    MS_U16 val = HAL_SEAL_NonPmRead2Byte(u32RegOffset);
    val = (bEnable) ? (val | u16Mask) : (val & ~u16Mask);
    HAL_SEAL_NonPmWrite2Byte(u32RegOffset, val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_GetNonPmIpIdx
/// @brief \b Function  \b Description: Get the Non-PM IP index with respect to HW wired
/// @param <IN>         \b u32RegAddr: Non-PM IP index with respect to SW enum
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_S32: Non-PM IP index with respect to HW wired
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_S32 HAL_SEAL_GetNonPmIpIdx(eSeal_IP eNonPmIP)
{
    MS_S32 s32HwIdx;

    for (s32HwIdx = 0; s32HwIdx < SEAL_NONPM_TBL_IP_NUM; s32HwIdx++)
    {
        if(eNonPmIP == NonPmIpTbl[s32HwIdx])
        {
            return s32HwIdx;
        }
    }

    return (-1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_GetPmIpIdx
/// @brief \b Function  \b Description: Get the PM IP index with respect to HW wired
/// @param <IN>         \b u32RegAddr: PM IP index with respect to SW enum
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_S32: PM IP index with respect to HW wired
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_S32 HAL_SEAL_GetPmIpIdx(eSeal_IP ePmIP)
{
    MS_S32 s32HwIdx;

    for (s32HwIdx = 0; s32HwIdx < SEAL_PM_TBL_IP_NUM; s32HwIdx++)
    {
        if(ePmIP == PmIpTbl[s32HwIdx])
        {
            return s32HwIdx;
        }
    }

    return (-1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_GetProcessorIdx
/// @brief \b Function  \b Description: Get the processor index with respect to HW wired
/// @param <IN>         \b u32RegAddr: Processor index with respect to SW enum
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_S32: Processor index with respect to HW wired
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_S32 HAL_SEAL_GetProcessorIdx(eSeal_ProcessorId eProcessorId)
{
    MS_S32 s32HwIdx;

    for (s32HwIdx = 0; s32HwIdx < SEAL_TBL_PROCESSOR_NUM; s32HwIdx++)
    {
        if(eProcessorId == ProcessorTbl[s32HwIdx])
        {
            return s32HwIdx;
        }
    }

    return (-1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_GetLockIdx
/// @brief \b Function  \b Description: Get the lock index with respect to HW wired
/// @param <IN>         \b u32RegAddr: Lock index with respect to SW enum
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_S32: Lock index with respect to HW wired
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_S32 HAL_SEAL_GetLockIdx(eSeal_Lock eLockId)
{
    MS_S32 s32HwIdx;

    for (s32HwIdx = 0; s32HwIdx < SEAL_TBL_LOCK_NUM; s32HwIdx++)
    {
        if(eLockId == LockTbl[s32HwIdx])
        {
            return s32HwIdx;
        }
    }

    return (-1);
}

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_SetIOMapBase
/// @brief \b Function  \b Description: set io-base
/// @param <IN>         \b u32NonPmBaseAddr: Non-PM io-base address
/// @param <IN>         \b u32PmBaseAddr: PM io-base address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SEAL_SetIOMapBase(MS_U32 u32NonPmBaseAddr, MS_U32 u32PmBaseAddr)
{
    MS_U32 u32RegAddr0;
#ifndef CONFIG_OPTEE
    MS_U32 u32RegAddr1;
#endif
    MS_U8  u8Val;
    MS_U16 u16Val;

    _gSEAL_NonPmMapBase = u32NonPmBaseAddr;
    _gSEAL_PmMapBase = u32PmBaseAddr;
    _gSEAL_NS0GroupBase = (SRAM_TZPC_NSGroup *)MsOS_PA2KSEG1(HAL_MIU0_BASE);
    _gSEAL_NS1GroupBase = (SRAM_TZPC_NSGroup *)MsOS_PA2KSEG1(HAL_MIU1_BASE);

    HAL_SEAL_DBG(printf("SEAL _gSEAL_NonPmMapBase= %lx\n", _gSEAL_NonPmMapBase));
    HAL_SEAL_DBG(printf("SEAL _gSEAL_PmMapBase= %lx\n", _gSEAL_PmMapBase));
    HAL_SEAL_DBG(printf("SEAL _gSEAL_NS0GroupBase= %lx\n", _gSEAL_NS0GroupBase));
    HAL_SEAL_DBG(printf("SEAL _gSEAL_NSG1roupBase= %lx\n", _gSEAL_NS1GroupBase));

    // Force RIU bank be controled by TZPC
    u32RegAddr0 = REG_TZPC_PROTECT_CTL;
    u8Val = HAL_SEAL_NonPmReadByte(u32RegAddr0);
    u8Val = u8Val | (1 << 2);
    HAL_SEAL_NonPmWriteByte(u32RegAddr0, u8Val);

    // Mask ARM decode error
    u32RegAddr0 = REG_TZPC_MASK ;
    u8Val = HAL_SEAL_NonPmReadByte(u32RegAddr0);
    u8Val = u8Val | REG_TZPC_RESP_MASK_MIU ;
    HAL_SEAL_NonPmWriteByte(u32RegAddr0, u8Val);

#if (defined(MCU_AEON))
    // Enable SEC_R2 to program NS sram
    u16Val = 0x0041;
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#else
    // Enable ARM to program NS sram
    u16Val = 0x0179;
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#endif

#ifndef CONFIG_OPTEE
    // Map NS sram on PA:0x0
    u32RegAddr0 = REG_TZPC_MIU0_BASE_ADDR_LOW;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, 0x0000);
    u32RegAddr0 = REG_TZPC_MIU0_BASE_ADDR_HIGH;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, 0x0000);

    u32RegAddr0 = REG_TZPC_MIU1_BASE_ADDR_LOW;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, 0x0000);
    u32RegAddr0 = REG_TZPC_MIU1_BASE_ADDR_HIGH;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, 0x0000);

    // Enable accessibility of NS sram
    u32RegAddr0 = REG_TZPC_MIU0_CTL;
    u32RegAddr1 = REG_TZPC_MIU1_CTL;

    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_TOP_EN;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr1);
    u16Val |= REG_TZPC_MIU_TOP_EN;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr1, u16Val);

    __delay();

    // Reset NS Sram
    HAL_SEAL_ResetSram();

    // Set some secure IP with secure accessibility
    HAL_Seal_SecureMasterSet(E_SEAL_ARM2MIU_SECURE_PROT_NONPM, 1);
    HAL_Seal_SecureMasterSet(E_SEAL_CRYPTODMA_SEC_R2_PROT_NONPN, 1);
    HAL_Seal_SecureMasterSet(E_SEAL_CRYPTODMA_GEN_SECURE_PROT_NONPM, 1);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SEAL_ResetSram
/// @brief \b Function  \b Description: reset NS sram to 0
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SEAL_ResetSram(void)
{
    MS_U32 u32RegAddr0;
    MS_U16 u16Val;
    MS_U16 u16Client;

#if (defined(MCU_AEON))
    // Enable SEC_R2 to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#else
    // Enable ARM to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#endif
    __delay();

    for(u16Client = 0; u16Client < SEAL_NONPM_TBL_IP_NUM; u16Client++)
    {
        SET_NONSECURE_CLIENT(_gSEAL_NS0GroupBase->client[u16Client]);
        SET_NONSECURE_CLIENT(_gSEAL_NS1GroupBase->client[u16Client]);

       // Flush data from internal sram to outer sram
       MsOS_FlushMemory();
    }
    __delay();

#if (defined(MCU_AEON))
    // Disable SEC_R2 to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#else
    // Disable ARM to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#endif
    __delay();

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureRangeSet
/// @brief \b Function  \b Description: set the secure range on MIU
/// @param <IN>         \b u8SecureRangeId : Secure range ID
/// @param <IN>         \b u64StartAddr : Start address of secure range
/// @param <IN>         \b u64EndAddr : End address of secure range
/// @param <IN>         \b u32Attribute : Attribute of secure range
/// @param <OUT>        \b OUT :
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_SecureRangeSet(
    MS_U8 u8SecureRangeId,
    MS_U64 u64StartAddr,
    MS_U64 u64EndAddr,
    MS_U32 u32Attribute
)
{
    MS_U32 u32RegStartAddr = 0;
    MS_U32 u32RegEndAddr = 0;
    MS_U32 u32RegAttribute = 0;
    MS_U32 u32RegDetEn = 0;
    MS_U16 u16Val = 0;
    MS_U64 u64PreStartAddr = 0;
    MS_U64 u64PreEndAddr = 0;
    MS_U32 u32RangeOffset = 0x08 * u8SecureRangeId;

    if (u8SecureRangeId >= E_HAL_SEAL_SECURE_RANGE_NUM)
    {
        return FALSE;
    }

    if(u64StartAddr < HAL_MIU1_BASE)
    {
        u32RegDetEn = REG_SECURE0_DETECT_ENABLE;
        u32RegStartAddr = REG_SECURE0_RANGE0_START_ADDR + u32RangeOffset;
        u32RegEndAddr = REG_SECURE0_RANGE0_END_ADDR + u32RangeOffset;
        u32RegAttribute = REG_SECURE0_RANGE0_ATTRIBUTE + u32RangeOffset;
    }
    else
    {
        u64StartAddr -= HAL_MIU1_BASE;
        u64EndAddr -= HAL_MIU1_BASE;

        u32RegDetEn = REG_SECURE1_DETECT_ENABLE;
        u32RegStartAddr = REG_SECURE1_RANGE0_START_ADDR + u32RangeOffset;
        u32RegEndAddr = REG_SECURE1_RANGE0_END_ADDR + u32RangeOffset;
        u32RegAttribute = REG_SECURE1_RANGE0_ATTRIBUTE + u32RangeOffset;
    }

    if (u32Attribute == 0x0 || (u64StartAddr == 0 && u64EndAddr == 0) || (u32Attribute == (E_SEAL_NONSECURE_READ | E_SEAL_NONSECURE_WRITE | E_SEAL_SECURE_READ | E_SEAL_SECURE_WRITE)))
    {
        // Disable secure range detection
        u16Val = HAL_SEAL_NonPmReadByte(u32RegDetEn);
        u16Val &= ~(1 << u8SecureRangeId);
        HAL_SEAL_NonPmWriteByte(u32RegDetEn, u16Val);

        // Set start address
        HAL_SEAL_NonPmWrite4Byte(u32RegStartAddr, 0x0);

        // Set end address
        HAL_SEAL_NonPmWrite4Byte(u32RegEndAddr, 0x0);

        // Trigger addr change
        u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAttribute);
        u16Val |= SECURE_RANGE_ADDR_DYN_CH;
        HAL_SEAL_NonPmWrite2Byte(u32RegAttribute, u16Val);
    }
    else
    {
        // kastor's range value is 4kb shifted
        u64StartAddr = u64StartAddr >> 12;
        u64EndAddr = u64EndAddr >> 12;

        // 20-bit value in kastor
        u64PreStartAddr = HAL_SEAL_NonPmRead4Byte(u32RegStartAddr) & 0x0FFFFF;
        u64PreEndAddr = HAL_SEAL_NonPmRead4Byte(u32RegEndAddr) & 0x0FFFFF;

        // if range does not change, we assume user only wants to change attrbuite
        // we do this to prevent nuttx dying after disable det_en bit when DRAMOBF is on
        if (u64StartAddr == u64PreStartAddr && u64EndAddr == u64PreEndAddr)
        {
            // do nothing
        }
        else
        {
            // disable secure range detection
            u16Val = HAL_SEAL_NonPmReadByte(u32RegDetEn);
            u16Val &= ~(1 << u8SecureRangeId);
            HAL_SEAL_NonPmWriteByte(u32RegDetEn, u16Val);

            // Set start address
            HAL_SEAL_NonPmWrite4Byte(u32RegStartAddr, (MS_U32)u64StartAddr);

            // Set end address
            HAL_SEAL_NonPmWrite4Byte(u32RegEndAddr, (MS_U32)u64EndAddr);

            // Trigger addr change
            u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAttribute);
            u16Val |= SECURE_RANGE_ADDR_DYN_CH;
            HAL_SEAL_NonPmWrite2Byte(u32RegAttribute, u16Val);

            // Enable secure range detection
            u16Val = HAL_SEAL_NonPmRead2Byte(u32RegDetEn);
            u16Val |= (1 << u8SecureRangeId);
            HAL_SEAL_NonPmWrite2Byte(u32RegDetEn, u16Val);
        }
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureRangeQuery
/// @brief \b Function  \b Description: Query the attribute of secure range
/// @param <IN>         \b u8SecureRangeId : Secure range ID
/// @param <OUT>        \b pAttribute : Attribute of secure range
/// @param <OUT>        \b pStartAddr : Start address of secure range
/// @param <OUT>        \b pEndAddr : End address of secure range
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_SecureRangeQuery(
    MS_U8 u8MiuDev,
    MS_U8 u8SecureRangeId,
    MS_U32 *pAttribute,
    MS_U64 *pStartAddr,
    MS_U64 *pEndAddr)
{
    MS_U32 u32RegStartAddr = 0;
    MS_U32 u32RegEndAddr = 0;
    MS_U32 u32RangeOffset = 0x08 * u8SecureRangeId;

    if (u8SecureRangeId >= E_HAL_SEAL_SECURE_RANGE_NUM)
    {
        return FALSE;
    }
    if (u8MiuDev >= E_HAL_MIU_NUM)
    {
        return FALSE;
    }

    if(u8MiuDev == E_HAL_MIU_DEV0)
    {
        u32RegStartAddr = REG_SECURE0_RANGE0_START_ADDR + u32RangeOffset;
        u32RegEndAddr = REG_SECURE0_RANGE0_END_ADDR + u32RangeOffset;
	}
	else
	{
        u32RegStartAddr = REG_SECURE1_RANGE0_START_ADDR + u32RangeOffset;
        u32RegEndAddr = REG_SECURE1_RANGE0_END_ADDR + u32RangeOffset;
	}

    // Get secure range attribute
    *pAttribute = 0x0; // kastor has no range attribute

    // Get start address
    *pStartAddr = (HAL_SEAL_NonPmRead4Byte(u32RegStartAddr) & 0x0FFFFF) << 12; // 20-bit value, 4kb shifted

    // Get end address
    *pEndAddr = (HAL_SEAL_NonPmRead4Byte(u32RegEndAddr) & 0x0FFFFF) << 12;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_IMI_RangeSet
/// @brief \b Function  \b Description: set the secure range on internel memory interface
/// @param <IN>         \b u32StartAddr : Start address of secure range on IMI
/// @param <IN>         \b u32EndAddr : End address of secure range on IMI
/// @param <IN>         \b bEnable : Enable/Disable secure range on IMI
/// @param <OUT>        \b OUT :
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_IMI_RangeSet(MS_U32 u32StartAddr, MS_U32 u32EndAddr, MS_BOOL bEnable)
{
    MS_U32 u32RegStartAddr = 0;
    MS_U32 u32RegEndAddr = 0;

    u32RegStartAddr = REG_IMI_RANGE_START_ADDR;
    u32RegEndAddr = REG_IMI_RANGE_END_ADDR;

    if(bEnable == 1)
    {
        // Set start address
        HAL_SEAL_NonPmWrite4Byte(u32RegStartAddr, u32StartAddr);

        // Set end address
        HAL_SEAL_NonPmWrite4Byte(u32RegEndAddr, u32EndAddr);
    }
    else
    {
        // Reset start address
        HAL_SEAL_NonPmWrite4Byte(u32RegStartAddr, 0x0);

        // Reset end address
        HAL_SEAL_NonPmWrite4Byte(u32RegEndAddr,0x0);
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_GetMiuHitFlag
/// @brief \b Function  \b Description: Get the information about hitted flag on MIU
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <OUT>        \b None:
/// @param <RET>        \b MS_BOOL: Hitted flag
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_GetMiuHitFlag(MS_U8 u8MiuDev)
{
    MS_U32 u32RegLogAddr;
    MS_U16 u16Val;

    u32RegLogAddr = (u8MiuDev) ? REG_SECURE1_HITTED_STATUS : REG_SECURE0_HITTED_STATUS;

    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegLogAddr);

    if(REG_SECURE_HITTED_FALG & u16Val)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_GetHitRangeID
/// @brief \b Function  \b Description: Get the information about the ID of hitted secure range on MIU
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <OUT>        \b None:
/// @param <RET>        \b MS_U8: ID of hitted secure range
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_Seal_GetMiuHitRangeID(MS_U8 u8MiuDev)
{
    MS_U32 u32RegLogAddr;
    MS_U16 u16Val;
    MS_U16 u8ret;

    u32RegLogAddr = (u8MiuDev) ? REG_SECURE1_HITTED_RANGE_ID : REG_SECURE0_HITTED_RANGE_ID;

    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegLogAddr);

    u8ret =(MS_U8)GET_HIT_RANGE_ID(u16Val);

    return u8ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_GetHitClientID
/// @brief \b Function  \b Description: Get the information about the ID of hittd MIU client ID on MIU
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <OUT>        \b None:
/// @param <RET>        \b MS_U8: hittd MIU client ID
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_Seal_GetMiuHitClientID(MS_U8 u8MiuDev)
{
    MS_U32 u32RegLogAddr;
    MS_U16 u16Val;
    MS_U16 u8ret;

    u32RegLogAddr = (u8MiuDev) ? REG_SECURE1_HITTED_STATUS : REG_SECURE0_HITTED_STATUS;

    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegLogAddr);
    u8ret =(MS_U8)GET_HIT_CLIENT_ID(u16Val);

    return u8ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_GetSecureFlag
/// @brief \b Function  \b Description: Get the information about the hitted client on MIU be secure or not
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <OUT>        \b None:
/// @param <RET>        \b MS_BOOL: Secure flag
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_GetMiuSecureFlag(MS_U8 u8MiuDev)
{
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_GetWriteFlag
/// @brief \b Function  \b Description: Get the information about the violation on MIU be write or read access
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <OUT>        \b None:
/// @param <RET>        \b MS_BOOL: Write flag
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_GetMiuWriteFlag(MS_U8 u8MiuDev)
{
    MS_U32 u32RegLogAddr;
    MS_U16 u16Val;

    u32RegLogAddr = (u8MiuDev) ? REG_SECURE1_HITTED_STATUS : REG_SECURE0_HITTED_STATUS;

    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegLogAddr);

    if(REG_SECURE_HITTED_IS_WRITE & u16Val)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_GetHitAddr
/// @brief \b Function  \b Description: Get the information about the hitted address on MIU
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <OUT>        \b None:
/// @param <RET>        \b MS_U32: Hitted address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U64 HAL_Seal_GetMiuHitAddr(MS_U8 u8MiuDev)
{
    MS_U32 u32RegAddr;
    MS_U64 u64Val;

    u32RegAddr = (u8MiuDev) ? REG_SECURE1_HITTED_ADDR : REG_SECURE0_HITTED_ADDR;
    u64Val = HAL_SEAL_NonPmRead6Byte(u32RegAddr);
    u64Val = u64Val >> HITTED_ADDRESS_OFFSET;

    return u64Val;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_ClearMiuHitLog
/// @brief \b Function  \b Description: Clear the hitted log on MIU
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <OUT>        \b None:
/// @param <RET>        \b RET:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_ClearMiuHitLog(MS_U8 u8MiuDev)
{
    MS_U32 u32RegLogAddr;

    u32RegLogAddr = (u8MiuDev) ? REG_SECURE1_HITTED_STATUS : REG_SECURE0_HITTED_STATUS;

    HAL_SEAL_NonPmWrite2BytesBit(u32RegLogAddr, TRUE, REG_SECURE_HITTED_LOG_CLR);
    HAL_SEAL_NonPmWrite2BytesBit(u32RegLogAddr, FALSE, REG_SECURE_HITTED_LOG_CLR);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureRangeLock
/// @brief \b Function  \b Description: One-way setting to lock the specific secure range
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <IN>         \b u8SecureRangeId: Secure range ID
/// @param <OUT>        \b None:
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_SecureRangeLock(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId)
{
    MS_U32 u32RegAddr;
    MS_U8 u8Val;

    u32RegAddr = (u8MiuDev) ? REG_SECURE1_LOCK : REG_SECURE0_LOCK;
    u8Val = HAL_SEAL_NonPmReadByte(u32RegAddr);
    u8Val = u8Val | (1 << u8SecureRangeId);
    HAL_SEAL_NonPmWriteByte(u32RegAddr, u8Val);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureProcessorSet
/// @brief \b Function  \b Description: Define the specific processor be secure or not
/// @param <IN>         \b u8ProcessorId: Processor ID
/// @param <IN>         \b bSecure: 1: non-secure 0:secure
/// @param <OUT>        \b None:
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_NonSecureProcessorSet(MS_U8 u8ProcessorId, MS_BOOL bNonSecure)
{
    MS_U32 u32RegAddr;
    MS_U16 u16Val;
    MS_S32 s32HwIdx;

    s32HwIdx = HAL_SEAL_GetProcessorIdx(u8ProcessorId);

    if(s32HwIdx == -1)
    {
       HAL_SEAL_ERR("Processor ID:%d doesn't support !\n", u8ProcessorId);
       return FALSE;
    }

    // special case for HEMCU
    if(u8ProcessorId == E_SEAL_HEMCU)
    {
        if(bNonSecure)
        {
            HAL_SEAL_NonPmWriteRegBit(REG_TZPC_NONSECURE_HEMCU, BIT3, ENABLE);
        }
        else
        {
            HAL_SEAL_NonPmWriteRegBit(REG_TZPC_NONSECURE_HEMCU, BIT3, DISABLE);
        }
        return TRUE;
    }


    u32RegAddr = REG_TZPC_NONSECURE_PROCESSOR;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);

    if(bNonSecure)
    {
        u16Val = u16Val | (1 << (s32HwIdx * 2));
    }
    else
    {
        u16Val = u16Val & ~(1 << (s32HwIdx * 2));
    }

    HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureProcessorQuery
/// @brief \b Function  \b Description: Query the specific processor be secure or not
/// @param <IN>         \b u8ProcessorId: Processor ID
/// @param <OUT>        \b pSecure: 1: non-secure 0:secure
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_NonSecureProcessorQuery(MS_U8 u8ProcessorId, MS_BOOL *pNonSecure)
{
    MS_U32 u32RegAddr;
    MS_U16 u16Val;
    MS_U16 s32HwIdx;

    s32HwIdx = HAL_SEAL_GetProcessorIdx(u8ProcessorId);

    if(s32HwIdx == -1)
    {
       HAL_SEAL_ERR("Processor ID:%d doesn't support !\n", u8ProcessorId);
       return FALSE;
    }

    // special case for HEMCU
    if(u8ProcessorId == E_SEAL_HEMCU)
    {
        u32RegAddr = REG_TZPC_NONSECURE_HEMCU;
        u16Val = (MS_U8)HAL_SEAL_NonPmReadByte(u32RegAddr);

        if(u16Val & BIT3)
        {
            *pNonSecure = 1;
        }
        else
        {
            *pNonSecure = 0;
        }
        return TRUE;
    }

    u32RegAddr = REG_TZPC_NONSECURE_PROCESSOR;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);

    if(u16Val & (1 << (s32HwIdx * 2)))
    {
        *pNonSecure = 1;
    }
    else
    {
        *pNonSecure = 0;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureSlaveSet
/// @brief \b Function  \b Description: Define the specific slave IP be secure or not
/// @param <IN>         \b u32SlaveId: Slave ID
/// @param <IN>         \b bSecure: 0: non-secure 1:secure
/// @param <OUT>        \b None:
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_SecureSlaveSet(MS_U32 u32SlaveId, MS_BOOL bSecure)
{
    MS_U32 u32RegAddr;
    MS_U16 u16Val;
    MS_S32 s32HwIdx;
    MS_U8 u8GroupId;
    MS_U8 u8ClientId;
    MS_BOOL bIsPm;

    bIsPm = 0;
    s32HwIdx = HAL_SEAL_GetNonPmIpIdx(u32SlaveId);

    //Slave ID not in Non-PM group, check if u32SlaveId in PM group
    if(s32HwIdx == -1)
    {
        s32HwIdx = HAL_SEAL_GetPmIpIdx(u32SlaveId);
        bIsPm = 1;

        //Slave ID not in Non-PM nor PM group
        if(s32HwIdx == -1)
        {
            HAL_SEAL_ERR("Slave ID:%d doesn't support !\n", (int)u32SlaveId);
            return FALSE;
        }
    }

    if(bIsPm == 0)
    {
        u8GroupId = (MS_U8)(s32HwIdx / SEAL_NONPM_GROUP_IP_NUM);
        u8ClientId = (MS_U8)(s32HwIdx % SEAL_NONPM_GROUP_IP_NUM);
        u32RegAddr = (MS_U32)(REG_TZPC_NONPM_SECURE_SLAVE + (u8GroupId * 2));

        if(bSecure == 1)
        {
            u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
            u16Val = u16Val & (~(1 << u8ClientId));
            HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
        }
        else
        {
            u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
            u16Val = u16Val | (1 << u8ClientId);
            HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
        }
    }
    else
    {
        u8GroupId = (MS_U8)(s32HwIdx / SEAL_PM_GROUP_IP_NUM);
        u8ClientId = (MS_U8)(s32HwIdx % SEAL_PM_GROUP_IP_NUM);
        u32RegAddr = (MS_U32)(REG_TZPC_PM_SECURE_SLAVE + (u8GroupId * 2));

        if(bSecure == 1)
        {
            u16Val = HAL_SEAL_PmRead2Byte(u32RegAddr);
            u16Val = u16Val & (~(1 << u8ClientId));
            HAL_SEAL_PmWrite2Byte(u32RegAddr, u16Val);
        }
        else
        {
            u16Val = HAL_SEAL_PmRead2Byte(u32RegAddr);
            u16Val = u16Val | (1 << u8ClientId);
            HAL_SEAL_PmWrite2Byte(u32RegAddr, u16Val);
        }
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureSlaveQuery
/// @brief \b Function  \b Description: Query the specific slave IP be secure or not
/// @param <IN>         \b u32SlaveId: Slave ID
/// @param <IN>         \b pSecure: 0: non-secure 1:secure
/// @param <OUT>        \b None:
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_SecureSlaveQuery(MS_U32 u32SlaveId, MS_BOOL *pSecure)
{
    MS_U32 u32RegAddr;
    MS_U16 u16Val;
    MS_S32 s32HwIdx;
    MS_U8 u8GroupId;
    MS_U8 u8ClientId;
    MS_BOOL bIsPm;

    bIsPm = 0;
    s32HwIdx = HAL_SEAL_GetNonPmIpIdx(u32SlaveId);

    //Slave ID not in Non-PM group, check if u32SlaveId in PM group
    if(s32HwIdx == -1)
    {
        s32HwIdx = HAL_SEAL_GetPmIpIdx(u32SlaveId);
        bIsPm = 1;

        //Slave ID not in Non-PM nor PM group
        if(s32HwIdx == -1)
        {
            HAL_SEAL_ERR("Slave ID:%d doesn't support !\n", (int)u32SlaveId);
            return FALSE;
        }
    }

    if(bIsPm == 0)
    {
        u8GroupId = (MS_U8)(s32HwIdx / SEAL_NONPM_GROUP_IP_NUM);
        u8ClientId = (MS_U8)(s32HwIdx % SEAL_NONPM_GROUP_IP_NUM);
        u32RegAddr = (MS_U32)(REG_TZPC_NONPM_SECURE_SLAVE + (u8GroupId * 2));

        u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
    }
    else
    {
        u8GroupId = (MS_U8)(s32HwIdx / SEAL_PM_GROUP_IP_NUM);
        u8ClientId = (MS_U8)(s32HwIdx % SEAL_PM_GROUP_IP_NUM);
        u32RegAddr = (MS_U32)(REG_TZPC_PM_SECURE_SLAVE + (u8GroupId * 2));

        u16Val = HAL_SEAL_PmRead2Byte(u32RegAddr);
    }

    if(u16Val & (1 << u8ClientId))
    {
        *pSecure = 0;
    }
    else
    {
        *pSecure = 1;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureMasterSet
/// @brief \b Function  \b Description: Define the specific master IP be secure or not
/// @param <IN>         \b u32SlaveId: Master ID
/// @param <IN>         \b bSecure: 0: non-secure 1:secure
/// @param <OUT>        \b None:
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_SecureMasterSet(MS_U32 u32MasterId, MS_BOOL bSecure)
{
    MS_U32 u32RegAddr0;
    MS_U16 u16Val;
    MS_S32 s32HwIdx;

    s32HwIdx = HAL_SEAL_GetNonPmIpIdx(u32MasterId);

    if(s32HwIdx == -1)
    {
        HAL_SEAL_ERR("Master ID:%d doesn't support !\n", (int)u32MasterId);
        return FALSE;
    }

#if (defined(MCU_AEON))
    // Enable SEC_R2 to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#else
    // Enable ARM to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#endif
    __delay();

    if(bSecure == 1)
    {
        SET_SECURE_CLIENT(_gSEAL_NS0GroupBase->client[s32HwIdx]);
        SET_SECURE_CLIENT(_gSEAL_NS1GroupBase->client[s32HwIdx]);
        __delay();
    }
    else
    {
        SET_NONSECURE_CLIENT(_gSEAL_NS0GroupBase->client[s32HwIdx]);
        SET_NONSECURE_CLIENT(_gSEAL_NS1GroupBase->client[s32HwIdx]);
        __delay();
    }

    // Flush data from internal sram to outer sram
    MsOS_FlushMemory();
    __delay();

#if (defined(MCU_AEON))
    // Disable SEC_R2 to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#else
    // Disable ARM to program NS sram
    u32RegAddr0 = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);

    u32RegAddr0 = REG_TZPC_MIU1_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr0);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr0, u16Val);
#endif
    __delay();

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_SecureMasterQuery
/// @brief \b Function  \b Description: Query the specific master IP be secure or not
/// @param <IN>         \b u32SlaveId: Master ID
/// @param <IN>         \b pSecure: 0: non-secure 1:secure
/// @param <OUT>        \b None:
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_SecureMasterQuery(MS_U32 u32MasterId, MS_BOOL *pSecure)
{
    MS_U32 u32RegAddr;
    MS_U16 u16Val;
    MS_S32 s32HwIdx;

    s32HwIdx = HAL_SEAL_GetNonPmIpIdx(u32MasterId);

    if(s32HwIdx == -1)
    {
        HAL_SEAL_ERR("PM Master ID:%d doesn't support !\n", (int)u32MasterId);
        return FALSE;
    }

    // Invalid data in internal sram to outer sram
    MsOS_ReadMemory();

#if (defined(MCU_AEON))
    // Enable SEC_R2 to program NS sram
    u32RegAddr = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
#else
    // Enable ARM to program NS sram
    u32RegAddr = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
    u16Val |= REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
#endif
    __delay();

    if(_gSEAL_NS0GroupBase->client[s32HwIdx].data[0] == 0x00)
    {
        *pSecure = 1;
    }
    else
    {
        *pSecure = 0;
    }

#if (defined(MCU_AEON))
    // Disable SEC_R2 to program NS sram
    u32RegAddr = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
#else
    // Disable ARM to program NS sram
    u32RegAddr = REG_TZPC_MIU0_ID0;
    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
    u16Val &= ~REG_TZPC_MIU_ID_ENABLE;
    HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
#endif
    __delay();

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_BufferLock
/// @brief \b Function  \b Description: Lock the specific buffer
/// @param <IN>         \b u8SecureRangeId: Secure buffer ID
/// @param <IN>         \b bLock: 0: unlock 1:lock
/// @param <OUT>        \b None:
/// @param <RET>        \b RET
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_BufferLock(MS_U8 u8BufferLockId, MS_BOOL bLock)
{
    MS_U32 u32RegAddr;
    MS_U16 u16Val;
    MS_S32 s32HwIdx;
    MS_U8 u8GroupId;
    MS_U8 u8ClientId;

    s32HwIdx = HAL_SEAL_GetLockIdx(u8BufferLockId);

    //Check if u8SecureBufferId exist
    if(s32HwIdx == -1)
    {
        HAL_SEAL_ERR("Secure buffer lock ID:%d doesn't support !\n", u8BufferLockId);
        return FALSE;
    }

    u8GroupId = (MS_U8)(s32HwIdx / SEAL_LOCK_NUM_PERGROUP);
    u8ClientId = (MS_U8)(s32HwIdx % SEAL_LOCK_NUM_PERGROUP);
    u32RegAddr = (MS_U32)(REG_TZPC_BUFFER_LOCK + (u8GroupId * 2));

    if(bLock == 1)
    {
        u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
        u16Val = u16Val & (~(1 << u8ClientId));
        HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
    }
    else
    {
        u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
        u16Val = u16Val | (1 << u8ClientId);
        HAL_SEAL_NonPmWrite2Byte(u32RegAddr, u16Val);
    }

    return TRUE;
}


MS_BOOL HAL_Seal_QueryBufferLocked(eSeal_Lock eLockId, MS_BOOL* bLocked)
{
    MS_U32 u32RegAddr;
    MS_U16 u16Val;
    MS_S32 s32HwIdx;
    MS_U8 u8GroupId;
    MS_U8 u8ClientId;

    s32HwIdx = HAL_SEAL_GetLockIdx(eLockId);

    //Check if u8SecureBufferId exist
    if(s32HwIdx == -1)
    {
        HAL_SEAL_ERR("Secure buffer lock ID:%u doesn't exist !\n", (unsigned int)eLockId);
        return FALSE;
    }

    u8GroupId = (MS_U8)(s32HwIdx / SEAL_LOCK_NUM_PERGROUP);
    u8ClientId = (MS_U8)(s32HwIdx % SEAL_LOCK_NUM_PERGROUP);
    u32RegAddr = (MS_U32)(REG_TZPC_BUFFER_LOCK + (u8GroupId * 2));

    u16Val = HAL_SEAL_NonPmRead2Byte(u32RegAddr);
    *bLocked = (u16Val & (1 << u8ClientId)) ? FALSE : TRUE;

    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_MaskInterruptFlag
/// @brief \b Function  \b Description: mask/unmask irq mask bit
/// @param <IN>         \b u8MiuDev: 0: MIU0 1:MIU1
/// @param <IN>         \b bMask: FLASE: unmask ; TRUE: mask
/// @param <OUT>        \b None:
/// @param <RET>        \b RET:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_MaskInterruptFlag(MS_U8 u8MiuDev, MS_BOOL bMask)
{
    MS_U32 u32RegLogAddr;

    u32RegLogAddr = (u8MiuDev) ? REG_SECURE1_HITTED_STATUS : REG_SECURE0_HITTED_STATUS;

    HAL_SEAL_NonPmWrite2BytesBit(u32RegLogAddr, bMask, REG_SECURE_HITTED_IRQ_MASK);
    HAL_SEAL_NonPmWrite2BytesBit(u32RegLogAddr, bMask, REG_SECURE_HITTED_IRQ_MASK);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_Seal_CheckSecureRange
/// @brief \b Function  \b Description: Is Overlap Secure Range
/// @param <IN>         \b phyStartAddr : start address
/// @param <IN>         \b u32Length : protect length
/// @param <IN>         \b eCheckSecureRangeType : check SecureRange type
/// @param <OUT>        \b None:
/// @param <RET>        \b RET:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_Seal_CheckSecureRange(MS_PHY phyStartAddr, MS_U32 u32Length, eSeal_CheckSecureRangeType eCheckSecureRangeType)
{
    MS_U8  u8MiuSel;
    MS_U8  u8Val;
    MS_U8  RangeNum;
    MS_U64 u64Offset;
    MS_U32 u32RegStartAddr;
    MS_U32 u32RegEndAddr;
    MS_U32 u32RegAttribute;
    MS_U32 u32RegDetEn;
    MS_U64 u64StartAddr;
    MS_U64 u64EndAddr;

    _phy_to_miu_offset(u8MiuSel, u64Offset, phyStartAddr);

    switch(u8MiuSel)
    {
        case E_CHIP_MIU_0:
            u32RegDetEn = REG_SECURE0_DETECT_ENABLE;
            u32RegStartAddr = REG_SECURE0_RANGE0_START_ADDR;
            u32RegEndAddr = REG_SECURE0_RANGE0_END_ADDR;
            u32RegAttribute = REG_SECURE0_RANGE0_ATTRIBUTE;
            break;
        case E_CHIP_MIU_1:
            u32RegDetEn = REG_SECURE1_DETECT_ENABLE;
            u32RegStartAddr = REG_SECURE1_RANGE0_START_ADDR;
            u32RegEndAddr = REG_SECURE1_RANGE0_END_ADDR;
            u32RegAttribute = REG_SECURE1_RANGE0_ATTRIBUTE;
            break;
        default:
            printf("%s: Wrong paramater-MiuSel %d \n", __FUNCTION__, u8MiuSel);
            return TRUE;
    }

    u8Val = HAL_SEAL_NonPmReadByte(u32RegDetEn);

    for(RangeNum = E_HAL_SEAL_SECURE_RANGE_ID0; RangeNum < E_HAL_SEAL_SECURE_RANGE_NUM; RangeNum++, u32RegStartAddr+=RANGE_ADDR_OFFSET, u32RegEndAddr+=RANGE_ADDR_OFFSET, u32RegAttribute+=RANGE_ADDR_OFFSET)
    {
        if( u8Val & (1<<RangeNum)) // Already set secure range
        {
            //check overlap
            u64StartAddr = HAL_SEAL_NonPmRead4Byte(u32RegStartAddr);
            u64EndAddr = HAL_SEAL_NonPmRead4Byte(u32RegEndAddr);
            //shift 4k align
            u64StartAddr = u64StartAddr << 12;
            u64EndAddr = u64EndAddr << 12;

            switch(eCheckSecureRangeType)
            {
                case E_SEAL_OVERLAP:
                    if ( u64StartAddr <= u64Offset && u64EndAddr > u64Offset )
                        return TRUE;
                    else if ( u64StartAddr > u64Offset && u64StartAddr < (u64Offset + u32Length))
                        return TRUE;
                    break;
                case E_SEAL_CONTAIN:
                    if ( u64StartAddr <= u64Offset && u64EndAddr >= (u64Offset + u32Length) )
                        return TRUE;
                    break;
                default:
                    printf("%s: Wrong paramater-eCheckSecureRangeType %d \n", __FUNCTION__, eCheckSecureRangeType);
                    return TRUE;
            }
        }
    }
    return FALSE;
}

#ifdef MSOS_TYPE_OPTEE
MS_BOOL HAL_Seal_ChangeIPSecureDMAAbillity(MS_U32 u32ModuleID, MS_U32 u32ModuleParameter, MS_U32 u32IsSecure)
{
    switch(u32ModuleID)
    {
        case MODULE_XC:
            HAL_Seal_SecureMasterSet(E_SEAL_MSC_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_SC0_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_SC1_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_SC2_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_SC3_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_SC4_PROT_NONPM, u32IsSecure);
            break;
        case MODULE_VDEC_EX:
        case MODULE_VDEC:
            switch(u32ModuleParameter)
            {
                case E_SEAL_IPGROUP_VDEC_R2:
                    HAL_Seal_SecureMasterSet(E_SEAL_VD_MHEG5_PROT_NONPM, u32IsSecure);
                    HAL_Seal_SecureMasterSet(E_SEAL_VD_EVD_R2_NONPM, u32IsSecure);
                    break;
                case E_SEAL_IPGROUP_VDEC_DECODER:
                    HAL_Seal_SecureMasterSet(E_SEAL_MVD_PROT_NONPM, u32IsSecure);
                    HAL_Seal_SecureMasterSet(E_SEAL_HVD_PROT_NONPM, u32IsSecure);
                    HAL_Seal_SecureMasterSet(E_SEAL_EVD_NONPM, u32IsSecure);
                    HAL_Seal_SecureMasterSet(E_SEAL_MAU_EVD_NONPM, u32IsSecure);
                    HAL_Seal_SecureMasterSet(E_SEAL_HIREG_EVD_PROT_NONPM, u32IsSecure);
                    HAL_Seal_SecureMasterSet(E_SEAL_MAU_EVD_LV2_0_NONPM, u32IsSecure);
                    HAL_Seal_SecureMasterSet(E_SEAL_MFDEC_PROT_NONPM, u32IsSecure);
                    break;
                default:
                    printf("%s: Unknown module paramater\n", __FUNCTION__);
                    break;
            }
            break;
        case MODULE_MVOP:
            HAL_Seal_SecureMasterSet(E_SEAL_MVOPSUB_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_MVOP_PROT_NONPM, u32IsSecure);
            break;
        case MODULE_AUDIO:
            HAL_Seal_SecureMasterSet(E_SEAL_VIVALDI2_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_VIVALDI3_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_VIVALDI7_PROT_NONPM, u32IsSecure);
            break;
        case MODULE_TVENCODER:
            HAL_Seal_SecureMasterSet(E_SEAL_VE_0_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_VE_1_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_VE_2_PROT_NONPM, u32IsSecure);
            break;
        case MODULE_DIP:
            HAL_Seal_SecureMasterSet(E_SEAL_MSC_PROT_NONPM, u32IsSecure);
            break;
        case MODULE_DMX:
            HAL_Seal_SecureMasterSet(E_SEAL_TSP0_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP1_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP_BLOCK_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP_SPARE_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP2_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP3_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP4_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP5_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP6_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP7_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP8_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP9_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP10_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP_FILEIN_ILLEGAL_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP_SEC_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP_OR_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP_VQ_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_TSP_PVR4_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_OTV01_PROT_NONPM, u32IsSecure);
            HAL_Seal_SecureMasterSet(E_SEAL_OTV23_PROT_NONPM, u32IsSecure);
            break;
        default:
            printf("%s, Unsupported ModuleID\n", __FUNCTION__);
            break;
    }
    return TRUE;
}

MS_BOOL HAL_Seal_SetSecureRange(MS_PHY u64phy, MS_U32 u32Length, MS_U32 u32IsSecure)
{
    MS_U8  u8MiuSel;
    MS_U64 u64Offset;
    MS_U32 u32RegDetEn;
    MS_U8 i;
    MS_U8 u8Val;
    MS_U32 u32RegStartAddr;
    MS_U64 u64StartAddr;

    _phy_to_miu_offset(u8MiuSel, u64Offset, u64phy);

    switch(u8MiuSel)
    {
        case E_CHIP_MIU_0:
        {
            u32RegDetEn = REG_SECURE0_DETECT_ENABLE;
            break;
        }
        case E_CHIP_MIU_1:
        {
            u32RegDetEn = REG_SECURE1_DETECT_ENABLE;
            break;
        }
        default:
        {
            return FALSE;
        }
    }

    u8Val = HAL_SEAL_NonPmReadByte(u32RegDetEn);

    if(u8MiuSel == E_CHIP_MIU_0)
    {
        u32RegStartAddr = REG_SECURE0_RANGE0_START_ADDR;
    }
    else if( u8MiuSel == E_CHIP_MIU_1 )
    {
        u32RegStartAddr = REG_SECURE1_RANGE0_START_ADDR;
    }
    else
    {
        return FALSE;
    }

    if (u32IsSecure) // setup a new securerange
    {
        for (i=E_HAL_SEAL_SECURE_RANGE_ID0; i<E_HAL_SEAL_SECURE_RANGE_NUM; i++, u32RegStartAddr+=RANGE_ADDR_OFFSET)
        {
            u64StartAddr = HAL_SEAL_NonPmRead6Byte(u32RegStartAddr);
            u64StartAddr = u64StartAddr << 12;
            if ((MS_U32)u64StartAddr == (MS_U32)u64Offset && (u8Val & (1<<i)))
            {
                printf("Secure range overlaps\n");
                return TRUE;
            }
        }

        for (i=E_HAL_SEAL_SECURE_RANGE_ID0; i<E_HAL_SEAL_SECURE_RANGE_NUM; i++)
        {
            if ( !(u8Val & (1<<i)) ) // got a free range
            {
                return HAL_Seal_SecureRangeSet(i, u64phy, u64phy+u32Length, E_SEAL_SECURE_READ | E_SEAL_SECURE_WRITE);
            }
        }

        return FALSE; // no free range
    }
    else // disable existing securerange
    {
        for (i=E_HAL_SEAL_SECURE_RANGE_ID0; i<E_HAL_SEAL_SECURE_RANGE_NUM; i++, u32RegStartAddr+=RANGE_ADDR_OFFSET)
        {
            u64StartAddr = HAL_SEAL_NonPmRead6Byte(u32RegStartAddr);
            u64StartAddr = u64StartAddr << 12;
            if ((MS_U32)u64StartAddr == (MS_U32)u64Offset && (u8Val & (1<<i)))
            {
                return HAL_Seal_SecureRangeSet(i, u64phy, u64phy+u32Length, 0x0);
            }
        }

        return FALSE; // no such range
    }
}
#endif

MS_BOOL HAL_SEAL_POWER_SUSPEND(void)
{
    return TRUE;
}

MS_BOOL HAL_SEAL_POWER_RESUME(void)
{
    return TRUE;
}
