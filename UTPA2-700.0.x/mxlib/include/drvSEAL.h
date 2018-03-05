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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvSEAL.h
/// @brief  SEAL Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SEAL SEAL interface
    \ingroup  G_PERIPHERAL

    \brief

    This drvSEAL.h provided function to protect security range, to prevent anyone
	who want to access this range.

    <b>Features</b>
    - SEAL initialize
    - SEAL secure range set
    - SEAL secure Master set
    - SEAL secure Slave set

    <b> Operation Code Flow: </b> \n
    -# SEAL initialize
    -# Set secure range
    -# set secure master
	-# set secure slave
	-# set Buffer lock

    \defgroup G_SEAL_INIT Initialization Task relative
     \ingroup  G_SEAL
     \defgroup G_SEAL_COMMON Common Task relative
     \ingroup  G_SEAL
     \defgroup G_SEAL_CONTROL Control relative
     \ingroup  G_SEAL
     \defgroup G_SEAL_INT  Interrupt relative
     \ingroup  G_SEAL
     \defgroup G_SEAL_ToBeModified GPIO api to be modified
     \ingroup  G_SEAL
     \defgroup G_SEAL_ToBeRemove GPIO api to be removed
     \ingroup  G_SEAL
*/

#ifndef _DRV_SEAL_H_
#define _DRV_SEAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "utopia.h"
#include "UFO.h"

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#define BIT0  0x0001UL
#define BIT1  0x0002UL
#define BIT2  0x0004UL
#define BIT3  0x0008UL
#define BIT4  0x0010UL
#define BIT5  0x0020UL
#define BIT6  0x0040UL
#define BIT7  0x0080UL
#define BIT8  0x0100UL
#define BIT9  0x0200UL
#define BIT10 0x0400UL
#define BIT11 0x0800UL
#define BIT12 0x1000UL
#define BIT13 0x2000UL
#define BIT14 0x4000UL
#define BIT15 0x8000UL

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define SEAL_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'S','E','A','L'},                  /* IP__                                             */  \
    {'0','0'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','0'},                          /* 00 ~ 99                                          */  \
    {'0','0','2','6','4','8','8','5'},  /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_SEAL_MIU_DEV0,
    E_SEAL_MIU_DEV1,
    E_SEAL_MIU_DEV2,
    E_SEAL_MIU_DEV3,
    E_SEAL_MIU_NUM,
}eSeal_MiuDev;

typedef enum
{
#if defined(UFO_PUBLIC_HEADER_500)
    E_SEAL_SECURE_WRITE    = BIT0,
    E_SEAL_SECURE_READ     = BIT1,
    E_SEAL_NONSECURE_WRITE = BIT2,
    E_SEAL_NONSECURE_READ  = BIT3,
#else
    E_SEAL_SECURE_READ     = BIT0,
    E_SEAL_SECURE_WRITE    = BIT1,
    E_SEAL_NONSECURE_READ  = BIT2,
    E_SEAL_NONSECURE_WRITE = BIT3,
#endif
}eSeal_SecureAttribute;

typedef enum
{
    E_SEAL_SECURE_RANGE_ID0,
    E_SEAL_SECURE_RANGE_ID1,
    E_SEAL_SECURE_RANGE_ID2,
    E_SEAL_SECURE_RANGE_ID3,
    E_SEAL_SECURE_RANGE_ID4,
    E_SEAL_SECURE_RANGE_ID5,
    E_SEAL_SECURE_RANGE_ID6,
    E_SEAL_SECURE_RANGE_ID7,
#if defined(UFO_PUBLIC_HEADER_300)||defined(UFO_PUBLIC_HEADER_700)
    E_SEAL_SECURE_RANGE_ID8,
    E_SEAL_SECURE_RANGE_ID9,
    E_SEAL_SECURE_RANGE_ID10,
    E_SEAL_SECURE_RANGE_ID11,
    E_SEAL_SECURE_RANGE_ID12,
    E_SEAL_SECURE_RANGE_ID13,
    E_SEAL_SECURE_RANGE_ID14,
    E_SEAL_SECURE_RANGE_ID15,
#endif
    E_SEAL_SECURE_RANGE_NUM,
}eSeal_SecureRangeId;

typedef enum
{
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300)||defined(UFO_PUBLIC_HEADER_700)
    E_SEAL_PROCESSOR_DUMMY = -1,
    E_SEAL_DBBUS = 0,
    E_SEAL_MCU51,
    E_SEAL_CPU2,
    E_SEAL_VD_R2,
    E_SEAL_SECURE_R2,
    E_SEAL_SC,
    E_SEAL_CMDQ,
    E_SEAL_HEMCU,
    E_SEAL_SECURE_51,
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_700)
    E_SEAL_SC_BE,
#endif
#else
    E_SEAL_DBBUS,
    E_SEAL_MCU51,
    E_SEAL_CPU2,
    E_SEAL_VD_R2,
    E_SEAL_SECURE_R2,
    E_SEAL_SC,
    E_SEAL_CMDQ,
    E_SEAL_HEMCU,
#endif
    E_SEAL_PROCESSOR_NUM,
}eSeal_ProcessorId ;

typedef enum
{
    E_SEAL_NONE                                =0,
    E_SEAL_DUMMY                               =1,
    E_SEAL_RIU_DBG_PROT_NONPM                  =2,
    E_SEAL_MSPI0_PROT_NONPM                    =3,
    E_SEAL_MSPI1_PROT_NONPM                    =4,
    E_SEAL_VD_MHEG5_PROT_NONPM                 =5,
    E_SEAL_MAU1_PROT_NONPM                     =6,
    E_SEAL_HIREG_PROT_NONPM                    =7,
    E_SEAL_POR_STATUS_PROT_NONPM               =8,
    E_SEAL_INTR_CPUINT_PROT_NONPM              =9,
    E_SEAL_MIU2_PROT_NONPM                     =10,
    E_SEAL_USB0_PROT_NONPM                     =11,
    E_SEAL_USB1_PROT_NONPM                     =12,
    E_SEAL_BDMA_CH0_PROT_NONPM                 =13,
    E_SEAL_BDMA_CH1_PROT_NONPM                 =14,
    E_SEAL_UART0_PROT_NONPM                    =15,
    E_SEAL_CLKGEN0_PROT_NONPM                  =16,
    E_SEAL_DSCRMB_PROT_NONPM                   =17,
    E_SEAL_UHC1_PROT_NONPM                     =18,
    E_SEAL_MHEG5_PROT_NONPM                    =19,
    E_SEAL_MVD_PROT_NONPM                      =20,
    E_SEAL_MIU_PROT_NONPM                      =21,
    E_SEAL_MVOPSUB_PROT_NONPM                  =22,
    E_SEAL_MVOP_PROT_NONPM                     =23,
    E_SEAL_TSP0_PROT_NONPM                     =24,
    E_SEAL_TSP1_PROT_NONPM                     =25,
    E_SEAL_JPD_PROT_NONPM                      =26,
    E_SEAL_SEMAPH_PROT_NONPM                   =27,
    E_SEAL_MAU0_PROT_NONPM                     =28,
    E_SEAL_ECBRIDGE_PROT_NONPM                 =29,
    E_SEAL_INTR_CTRL_PROT_NONPM                =30,
    E_SEAL_HDMI2_PROT_NONPM                    =31,
    E_SEAL_HVD_PROT_NONPM                      =32,
    E_SEAL_TSP2_PROT_NONPM                     =33,
    E_SEAL_MIPS_PROT_NONPM                     =34,
    E_SEAL_CHIP_PROT_NONPM                     =35,
    E_SEAL_GOP_PROT_NONPM                      =36,
    E_SEAL_EMAC0_PROT_NONPM                    =37,
    E_SEAL_EMAC1_PROT_NONPM                    =38,
    E_SEAL_EMAC2_PROT_NONPM                    =39,
    E_SEAL_EMAC3_PROT_NONPM                    =40,
    E_SEAL_UHC0_PROT_NONPM                     =41,
    E_SEAL_ADC_ATOP_PROT_NONPM                 =42,
    E_SEAL_ADC_DTOP_PROT_NONPM                 =43,
    E_SEAL_HDMI_PROT_NONPM                     =44,
    E_SEAL_GE0_PROT_NONPM                      =45,
    E_SEAL_SMART_PROT_NONPM                    =46,
    E_SEAL_CI_PROT_NONPM                       =47,
    E_SEAL_CHIPGPIO_PROT_NONPM                 =48,
    E_SEAL_VP6_PROT_NONPM                      =49,
    E_SEAL_LDM_DMA0_PROT_NONPM                 =50,
    E_SEAL_LDM_DMA1_PROT_NONPM                 =51,
    E_SEAL_SC0_PROT_NONPM                      =52,
    E_SEAL_SC1_PROT_NONPM                      =53,
    E_SEAL_SC2_PROT_NONPM                      =54,
    E_SEAL_SC3_PROT_NONPM                      =55,
    E_SEAL_SC4_PROT_NONPM                      =56,
    E_SEAL_CLKGEN1_PROT_NONPM                  =57,
    E_SEAL_MAILBOX_PROT_NONPM                  =58,
    E_SEAL_MIIC_PROT_NONPM                     =59,
    E_SEAL_PCM_PROT_NONPM                      =60,
    E_SEAL_VDMCU51_IF_PROT_NONPM               =61,
    E_SEAL_DMDMCU51_IF_PROT_NONPM              =62,
    E_SEAL_URDMA_PROT_NONPM                    =63,
    E_SEAL_AFEC_PROT_NONPM                     =64,
    E_SEAL_COMB_PROT_NONPM                     =65,
    E_SEAL_VBI_PROT_NONPM                      =66,
    E_SEAL_SCM_PROT_NONPM                      =67,
    E_SEAL_UTMI2_PROT_NONPM                    =68,
    E_SEAL_PATGEN_PROT_NONPM                   =69,
    E_SEAL_UTMI1_PROT_NONPM                    =70,
    E_SEAL_UTMI_PROT_NONPM                     =71,
    E_SEAL_VE_0_PROT_NONPM                     =72,
    E_SEAL_REG_PIU_NONPM_PROT_NONPM            =73,
    E_SEAL_VE_1_PROT_NONPM                     =74,
    E_SEAL_VE_2_PROT_NONPM                     =75,
    E_SEAL_MPIF_PROT_NONPM                     =76,
    E_SEAL_GPD_PROT_NONPM                      =77,
    E_SEAL_UART1_PROT_NONPM                    =78,
    E_SEAL_UART2_PROT_NONPM                    =79,
    E_SEAL_FUART_PROT_NONPM                    =80,
    E_SEAL_GE1_PROT_NONPM                      =81,
    E_SEAL_G3D_PROT_NONPM                      =82,
    E_SEAL_DVI_ATOP_PROT_NONPM                 =83,
    E_SEAL_DVI_DTOP_PROT_NONPM                 =84,
    E_SEAL_DVIEQ_PROT_NONPM                    =85,
    E_SEAL_HDCP_PROT_NONPM                     =86,
    E_SEAL_NR_HSD_PROT_NONPM                   =87,
    E_SEAL_ANA_MISC_PROT_NONPM                 =88,
    E_SEAL_MIU_ATOP_PROT_NONPM                 =89,
    E_SEAL_NR_PROT_NONPM                       =90,
    E_SEAL_DI_PROT_NONPM                       =91,
    E_SEAL_MFE0_PROT_NONPM                     =92,
    E_SEAL_MFE1_PROT_NONPM                     =93,
    E_SEAL_ADC_DTOPB_PROT_NONPM                =94,
    E_SEAL_NFIE0_PROT_NONPM                    =95,
    E_SEAL_NFIE1_PROT_NONPM                    =96,
    E_SEAL_NFIE2_PROT_NONPM                    =97,
    E_SEAL_ON0_PROT_NONPM                      =98,
    E_SEAL_ON1_PROT_NONPM                      =99,
    E_SEAL_MIIC0_PROT_NONPM                    =100,
    E_SEAL_MIIC1_PROT_NONPM                    =101,
    E_SEAL_MIIC2_PROT_NONPM                    =102,
    E_SEAL_MIIC3_PROT_NONPM                    =103,
    E_SEAL_CLKGEN_DMD_PROT_NONPM               =104,
    E_SEAL_DEMOD_0_PROT_NONPM                  =105,
    E_SEAL_DEMOD_1_PROT_NONPM                  =106,
    E_SEAL_DEMOD_2_PROT_NONPM                  =107,
    E_SEAL_DEMOD_3_PROT_NONPM                  =108,
    E_SEAL_DEMOD_4_PROT_NONPM                  =109,
    E_SEAL_DEMOD_5_PROT_NONPM                  =110,
    E_SEAL_DEMOD_6_PROT_NONPM                  =111,
    E_SEAL_DEMOD_7_PROT_NONPM                  =112,
    E_SEAL_DMD_ANA_MISC_PROT_NONPM             =113,
    E_SEAL_AUR20_PROT_NONPM                    =114,
    E_SEAL_VIVALDI0_PROT_NONPM                 =115,
    E_SEAL_VIVALDI1_PROT_NONPM                 =116,
    E_SEAL_VIVALDI2_PROT_NONPM                 =117,
    E_SEAL_VIVALDI3_PROT_NONPM                 =118,
    E_SEAL_VIVALDI4_PROT_NONPM                 =119,
    E_SEAL_VIVALDI5_PROT_NONPM                 =120,
    E_SEAL_AUR21_PROT_NONPM                    =121,
    E_SEAL_AUR22_PROT_NONPM                    =122,
    E_SEAL_DVI_ATOP_1_PROT_NONPM               =123,
    E_SEAL_DVI_DTOP_1_PROT_NONPM               =124,
    E_SEAL_DVIEQ_1_PROT_NONPM                  =125,
    E_SEAL_HDCP_1_PROT_NONPM                   =126,
    E_SEAL_DVI_ATOP_2_PROT_NONPM               =127,
    E_SEAL_DVI_DTOP_2_PROT_NONPM               =128,
    E_SEAL_DVIEQ_2_PROT_NONPM                  =129,
    E_SEAL_HDCP_2_PROT_NONPM                   =130,
    E_SEAL_DVI_PS_PROT_NONPM                   =131,
    E_SEAL_DVI_DTOP_3_PROT_NONPM               =132,
    E_SEAL_DVIEQ_3_PROT_NONPM                  =133,
    E_SEAL_HDCP_3_PROT_NONPM                   =134,
    E_SEAL_USB2_PROT_NONPM                     =135,
    E_SEAL_UHC2_PROT_NONPM                     =136,
    E_SEAL_DRM_SECURE_PROT_NONPM               =137,
    E_SEAL_DSCRMB2_PROT_NONPM                  =138,
    E_SEAL_DSCRMB3_PROT_NONPM                  =139,
    E_SEAL_GPD0_PROT_NONPM                     =140,
    E_SEAL_GPD1_PROT_NONPM                     =141,
    E_SEAL_GOP4G_0_PROT_NONPM                  =142,
    E_SEAL_GOP4G_1_PROT_NONPM                  =143,
    E_SEAL_GOP4G_ST_PROT_NONPM                 =144,
    E_SEAL_GOP2G_0_PROT_NONPM                  =145,
    E_SEAL_GOP2G_1_PROT_NONPM                  =146,
    E_SEAL_GOP2G_ST_PROT_NONPM                 =147,
    E_SEAL_GOP1G_0_PROT_NONPM                  =148,
    E_SEAL_GOP1G_1_PROT_NONPM                  =149,
    E_SEAL_GOP1G_ST_PROT_NONPM                 =150,
    E_SEAL_GOP1GX_0_PROT_NONPM                 =151,
    E_SEAL_GOP1GX_1_PROT_NONPM                 =152,
    E_SEAL_GOP1GX_ST_PROT_NONPM                =153,
    E_SEAL_GOPD_PROT_NONPM                     =154,
    E_SEAL_SPARE0_PROT_NONPM                   =155,
    E_SEAL_SPARE1_PROT_NONPM                   =156,
    E_SEAL_CA9PAT_PROT_NONPM                   =157,
    E_SEAL_USB30PHY_DTOP_PROT_NONPM            =158,
    E_SEAL_USB30PHY_ATOP_PROT_NONPM            =159,
    E_SEAL_UTMI3_PROT_NONPM                    =160,
    E_SEAL_USB3INDCTL_PROT_NONPM               =161,
    E_SEAL_USB3TOP_PROT_NONPM                  =162,
    E_SEAL_ALBANY0_PROT_NONPM                  =163,
    E_SEAL_ALBANY1_PROT_NONPM                  =164,
    E_SEAL_SEC_R2_PROT_NONPM                   =165,
    E_SEAL_SEC_MAU0_PROT_NONPM                 =166,
    E_SEAL_MOBF_PROT_NONPM                     =167,
    E_SEAL_DC_SCL_PROT_NONPM                   =168,
    E_SEAL_JPD1_PROT_NONPM                     =169,
    E_SEAL_JPD2_PROT_NONPM                     =170,
    E_SEAL_JPD3_PROT_NONPM                     =171,
    E_SEAL_CMDQ_PROT_NONPM                     =172,
    E_SEAL_MSC_PROT_NONPM                      =173,
    E_SEAL_GPUAPB_PROT_NONPM                   =174,
    E_SEAL_X32_USB3XHCI_PROT_NONPM             =175,
    E_SEAL_USBBC0_PROT_NONPM                   =176,
    E_SEAL_USBBC1_PROT_NONPM                   =177,
    E_SEAL_USBBC2_PROT_NONPM                   =178,
    E_SEAL_USB3_BC0_PROT_NONPM                 =179,
    E_SEAL_MHL_TMDS_PROT_NONPM                 =180,
    E_SEAL_HDCPKEY_PROT_NONPM                  =181,
    E_SEAL_ACP_PROT_NONPM                      =182,
    E_SEAL_SPARE2_PROT_NONPM                   =183,
    E_SEAL_SPARE3_PROT_NONPM                   =184,
    E_SEAL_TSP_DUMMY_PROT_NONPM                =185,
    E_SEAL_CODEC_DUMMY_PROT_NONPM              =186,
    E_SEAL_MHEG5_DUMMY_PROT_NONPM              =187,
    E_SEAL_DSCRMB4_PROT_NONPM                  =188,
    E_SEAL_MENULOAD_PROT_PM                    =189,
    E_SEAL_GDMA_PROT_PM                        =190,
    E_SEAL_DDC_PROT_PM                         =191,
    E_SEAL_ISP_PROT_PM                         =192,
    E_SEAL_FSP_PROT_PM                         =193,
    E_SEAL_QSPI_PROT_PM                        =194,
    E_SEAL_PM_SLEEP_PROT_PM                    =195,
    E_SEAL_PM_GPIO_PROT_PM                     =196,
    E_SEAL_MCU_PROT_PM                         =197,
    E_SEAL_PM_CEC_PROT_PM                      =198,
    E_SEAL_PM_RTC_PROT_PM                      =199,
    E_SEAL_PM_RTC2_PROT_PM                     =200,
    E_SEAL_PM_SAR_PROT_PM                      =201,
    E_SEAL_PM_AV_LINK_PROT_PM                  =202,
    E_SEAL_PM_TOP_PROT_PM                      =203,
    E_SEAL_MHL_CBUS_PROT_PM                    =204,
    E_SEAL_EFUSE_PROT_PM                       =205,
    E_SEAL_IRQ_PROT_PM                         =206,
    E_SEAL_CACHE_PROT_PM                       =207,
    E_SEAL_XDMIU_PROT_PM                       =208,
    E_SEAL_PM_MISC_PROT_PM                     =209,
    E_SEAL_PM_MHL_CBUS_PROT_PM                 =210,
    E_SEAL_WDT_PROT_PM                         =211,
    E_SEAL_TIMER0_PROT_PM                      =212,
    E_SEAL_TIMER1_PROT_PM                      =213,
    E_SEAL_SEC_KEY_PROT_PM                     =214,
    E_SEAL_DID_KEY_PROT_PM                     =215,
    E_SEAL_REG_PIU_MISC_0_PROT_PM              =216,
    E_SEAL_IR_PROT_PM                          =217,
    E_SEAL_PM_SPARE0_PROT_PM                   =218,
    E_SEAL_PM_SPARE1_PROT_PM                   =219,
    E_SEAL_FUART1_PROT_NONPM                   =220,
    E_SEAL_URDMA1_PROT_NONPM                   =221,
    E_SEAL_UPLL0_PROT_NONPM                    =222,
    E_SEAL_UPLL1_PROT_NONPM                    =223,
    E_SEAL_UART3_PROT_NONPM                    =224,
    E_SEAL_CLKGEN2_PROT_NONPM                  =225,
    E_SEAL_VDMCU51_1_IF_PROT_NONPM             =226,
    E_SEAL_ADC_ATOPB_PROT_NONPM                =227,
    E_SEAL_UART4_PROT_NONPM                    =228,
    E_SEAL_ANA_MISC_GMAC_PROT_NONPM            =229,
    E_SEAL_USB30PHY_DTOP_M_0_PROT_NONPM        =230,
    E_SEAL_USB30PHY_DTOP_M_1_PROT_NONPM        =231,
    E_SEAL_USB3_PROT_NONPM                     =232,
    E_SEAL_TSO_PROT_NONPM                      =233,
    E_SEAL_SDIO0_PROT_NONPM                    =234,
    E_SEAL_MIU_ARB256_PROT_NONPM               =235,
    E_SEAL_HDMITX_MISC_PROT_NONPM              =236,
    E_SEAL_MIIC4_PROT_NONPM                    =237,
    E_SEAL_MIIC5_PROT_NONPM                    =238,
    E_SEAL_HDMITX_VIDEO_PROT_NONPM             =239,
    E_SEAL_HDMITX_AUDIO_PROT_NONPM             =240,
    E_SEAL_USB30PHY_ATOP_M_0_PROT_NONPM        =241,
    E_SEAL_USB30PHY_ATOP_M_1_PROT_NONPM        =242,
    E_SEAL_UTMISS0_PROT_NONPM                  =243,
    E_SEAL_UTMISS1_PROT_NONPM                  =244,
    E_SEAL_UHC3_PROT_NONPM                     =245,
    E_SEAL_SDIO1_PROT_NONPM                    =246,
    E_SEAL_SDIO2_PROT_NONPM                    =247,
    E_SEAL_USBBC3_PROT_NONPM                   =248,
    E_SEAL_USB3_BC1_PROT_NONPM                 =249,
    E_SEAL_SECURERANGE0_PROT_NONPM             =250,
    E_SEAL_SECURERANGE1_PROT_NONPM             =251,
    E_SEAL_TZPC_NONPM_PROT_NONPM               =252,
    E_SEAL_NFIE3_PROT_NONPM                    =253,
    E_SEAL_DSCRMB5_PROT_NONPM                  =254,
    E_SEAL_EMMC_PLL_PROT_NONPM                 =255,
    E_SEAL_VIVALDI6_PROT_NONPM                 =256,
    E_SEAL_VIVALDI7_PROT_NONPM                 =257,
    E_SEAL_GMAC0_PROT_NONPM                    =258,
    E_SEAL_GMAC1_PROT_NONPM                    =259,
    E_SEAL_GMAC2_PROT_NONPM                    =260,
    E_SEAL_GMAC3_PROT_NONPM                    =261,
    E_SEAL_GMAC4_PROT_NONPM                    =262,
    E_SEAL_PCM2_PROT_NONPM                     =263,
    E_SEAL_TSP3_PROT_NONPM                     =264,
    E_SEAL_HEVC0_PROT_NONPM                    =265,
    E_SEAL_HEVC1_PROT_NONPM                    =266,
    E_SEAL_DYN_SCL_PROT_NONPM                  =267,
    E_SEAL_X32_MVD1_PROT_NONPM                 =268,
    E_SEAL_X32_MVD2_PROT_NONPM                 =269,
    E_SEAL_X32_MVD3_PROT_NONPM                 =270,
    E_SEAL_X32_MVD4_PROT_NONPM                 =271,
    E_SEAL_CODEC_MRQ_PROT_NONPM                =272,
    E_SEAL_MIU3_PROT_NONPM                     =273,
    E_SEAL_MIU_ATOP2_PROT_NONPM                =274,
    E_SEAL_X32_HVD0_PROT_NONPM                 =275,
    E_SEAL_X32_HVD1_PROT_NONPM                 =276,
    E_SEAL_X32_HVD2_PROT_NONPM                 =277,
    E_SEAL_X32_HVD3_PROT_NONPM                 =278,
    E_SEAL_X32_HVD4_PROT_NONPM                 =279,
    E_SEAL_X32_HVD5_PROT_NONPM                 =280,
    E_SEAL_X32_HVD6_PROT_NONPM                 =281,
    E_SEAL_X32_HVD7_PROT_NONPM                 =282,
    E_SEAL_X32_EVD0_PROT_NONPM                 =283,
    E_SEAL_X32_EVD1_PROT_NONPM                 =284,
    E_SEAL_X32_EVD2_PROT_NONPM                 =285,
    E_SEAL_X32_EVD3_PROT_NONPM                 =286,
    E_SEAL_X32_EVD4_PROT_NONPM                 =287,
    E_SEAL_X32_EVD5_PROT_NONPM                 =288,
    E_SEAL_X32_EVD6_PROT_NONPM                 =289,
    E_SEAL_X32_EVD7_PROT_NONPM                 =290,
    E_SEAL_X32_EVD8_PROT_NONPM                 =291,
    E_SEAL_X32_GPUAPB0_PROT_NONPM              =292,
    E_SEAL_X32_GPUAPB1_PROT_NONPM              =293,
    E_SEAL_X32_GPUAPB2_PROT_NONPM              =294,
    E_SEAL_TIMER2_PROT_PM                      =295,
    E_SEAL_ALBANY0_PROT_PM                     =296,
    E_SEAL_ALBANY1_PROT_PM                     =297,
    E_SEAL_ALBANY2_PROT_PM                     =298,
    E_SEAL_NORPF_PROT_NONPM                    =299,
    E_SEAL_PM_PROT_NONPM                       =300,
    E_SEAL_ON_PROT_NONPM                       =301,
    E_SEAL_SWCD_PROT_NONPM                     =302,
    E_SEAL_ALBANY2_PROT_NONPM                  =303,
    E_SEAL_TZPC_PROT_PM                        =304,
    E_SEAL_SPARE4_PROT_NONPM                   =305,
    E_SEAL_SPARE5_PROT_NONPM                   =306,
    E_SEAL_JPD4_PROT_NONPM                     =307,
    E_SEAL_RIU_DBG_PROT_PM                     =308,
    E_SEAL_UHC4_PROT_NONPM                     =309,
    E_SEAL_USB4_PROT_NONPM                     =310,
    E_SEAL_UTMI4_PROT_NONPM                    =311,
    E_SEAL_USBBC4_PROT_NONPM                   =312,
    E_SEAL_UTMISS_PROT_NONPM                   =313,
    E_SEAL_VDR2_PROT_NONPM                     =314,
    E_SEAL_INTR_CTRL1_PROT_NONPM               =315,
    E_SEAL_L3_AXI_PROT_NONPM                   =316,
    E_SEAL_MCU_ARM_PROT_NONPM                  =317,
    E_SEAL_VDMCU51_1_IF_NONPM                  =318,
    E_SEAL_SC_GP1_NONPM                        =319,
    E_SEAL_CHIPGPIO1_NONPM                     =320,
    E_SEAL_GPU_NONPM                           =321,
    E_SEAL_ANA_MISC2_NONPM                     =322,
    E_SEAL_AU_MAU_NONPM                        =323,
    E_SEAL_AU_GDMA_NONPM                       =324,
    E_SEAL_USB3_NONPM                          =325,
    E_SEAL_OTG0_NONPM                          =326,
    E_SEAL_OTG1_NONPM                          =327,
    E_SEAL_OTG2_NONPM                          =328,
    E_SEAL_OTG3_NONPM                          =329,
    E_SEAL_SECURERANGE2_NONPM                  =330,
    E_SEAL_TZPC_NONPM2_NONPM                   =331,
    E_SEAL_TSO1_NONPM                          =332,
    E_SEAL_MSC1_NONPM                          =333,
    E_SEAL_EVD_NONPM                           =334,
    E_SEAL_MIU_ARB_NONPM                       =335,
    E_SEAL_TSP4_NONPM                          =336,
    E_SEAL_DVI_ATOP3_NONPM                     =337,
    E_SEAL_HDMI_P4_NONPM                       =338,
    E_SEAL_HDMI2_P4_NONPM                      =339,
    E_SEAL_DVI_ATOP_P4_NONPM                   =340,
    E_SEAL_DVI_DTOP_P4_NONPM                   =341,
    E_SEAL_DVI_EQ_P4_NONPM                     =342,
    E_SEAL_HDCP_P4_NONPM                       =343,
    E_SEAL_DVI_POWERSAVE_P4_NONPM              =344,
    E_SEAL_MIU3_NONPM                          =345,
    E_SEAL_MIU_ATOP3_NONPM                     =346,
    E_SEAL_MIU_ARB2_NONPM                      =347,
    E_SEAL_MIU_ARB3_NONPM                      =348,
    E_SEAL_MIU4_NONPM                          =349,
    E_SEAL_MIU_ATOP4_NONPM                     =350,
    E_SEAL_MIU_ARB4_NONPM                      =351,
    E_SEAL_GE2_NONPM                           =352,
    E_SEAL_GE3_NONPM                           =353,
    E_SEAL_HDCP22_P0_NONPM                     =354,
    E_SEAL_HDCP22_P1_NONPM                     =355,
    E_SEAL_HDCP22_P2_NONPM                     =356,
    E_SEAL_HDCP22_P3_NONPM                     =357,
    E_SEAL_HDCP22_P4_NONPM                     =358,
    E_SEAL_AU_R2_1_NONPM                       =359,
    E_SEAL_AU_MAU_1_NONPM                      =360,
    E_SEAL_AU_GDMA_1_NONPM                     =361,
    E_SEAL_VD_EVD_R2_NONPM                     =362,
    E_SEAL_MAU_EVD_NONPM                       =363,
    E_SEAL_MAU1_LV2_0_NONPM                    =364,
    E_SEAL_MAU1_LV2_1_NONPM                    =365,
    E_SEAL_MAU_EVD_LV2_0_NONPM                 =366,
    E_SEAL_MAU_EVD_LV2_1_NONPM                 =367,
    E_SEAL_SEC_MAU_LV2_0_NONPM                 =368,
    E_SEAL_SEC_MAU_LV2_1_NONPM                 =369,
    E_SEAL_TSP5_NONPM                          =370,
    E_SEAL_X32_USB3XHCI_NONPM                  =371,
    E_SEAL_X32_GPUAPB0_NONPM                   =372,
    E_SEAL_X32_GPUAPB1_NONPM                   =373,
    E_SEAL_X32_GPUAPB2_NONPM                   =374,
    E_SEAL_SECURERANGE3_PROT_NONPM             =375,
    E_SEAL_HIREG_EVD_PROT_NONPM                =376,
    E_SEAL_PM_RTC0_PROT_PM                     =377,
    E_SEAL_PM_RTC1_PROT_PM                     =378,
    E_SEAL_PM_CEC1_PROT_PM                     =379,
    E_SEAL_PM_CEC2_PROT_PM                     =380,
    E_SEAL_PM_CEC3_PROT_PM                     =381,
    E_SEAL_MOD2_PROT_NONPM                     =382,
    E_SEAL_GOPG4_0_PROT_NONPM                  =383,
    E_SEAL_GOPG4_1_PROT_NONPM                  =384,
    E_SEAL_GOPG4_ST_PROT_NONPM                 =385,
    E_SEAL_USB30_MIU_ARB_PROT_NONPM            =386,
    E_SEAL_ZDEC_PROT_NONPM                     =387,
    E_SEAL_PATGEN_CODEC_PROT_NONPM             =388,
    E_SEAL_PATGEN_VIV_PROT_NONPM               =389,
    E_SEAL_PATGEN_CPU_PROT_NONPM               =390,
    E_SEAL_PATGEN_GPU_PROT_NONPM               =391,
    E_SEAL_PATGEN_DEMOD_PROT_NONPM             =392,
    E_SEAL_PATGEN_TSP_PROT_NONPM               =393,
    E_SEAL_PATGEN_DVI_PROT_NONPM               =394,
    E_SEAL_PATGEN_EVD_PROT_NONPM               =395,
    E_SEAL_PATGEN_SC0_PROT_NONPM               =396,
    E_SEAL_PATGEN_SC1_PROT_NONPM               =397,
    E_SEAL_PATGEN_SC2_PROT_NONPM               =398,
    E_SEAL_COMBO_PHY0_P0_PROT_NONPM            =399,
    E_SEAL_COMBO_PHY1_P0_PROT_NONPM            =400,
    E_SEAL_COMBO_PHY0_P1_PROT_NONPM            =401,
    E_SEAL_COMBO_PHY1_P1_PROT_NONPM            =402,
    E_SEAL_COMBO_PHY0_P2_PROT_NONPM            =403,
    E_SEAL_COMBO_PHY1_P2_PROT_NONPM            =404,
    E_SEAL_COMBO_PHY0_P3_PROT_NONPM            =405,
    E_SEAL_COMBO_PHY1_P3_PROT_NONPM            =406,
    E_SEAL_DVI_DTOP_DUAL_P0_PROT_NONPM         =407,
    E_SEAL_DVI_RSV_DUAL_P0_PROT_NONPM          =408,
    E_SEAL_HDCP_DUAL_P0_PROT_NONPM             =409,
    E_SEAL_DVI_DTOP_DUAL_P1_PROT_NONPM         =410,
    E_SEAL_DVI_RSV_DUAL_P1_PROT_NONPM          =411,
    E_SEAL_HDCP_DUAL_P1_PROT_NONPM             =412,
    E_SEAL_DVI_DTOP_DUAL_P2_PROT_NONPM         =413,
    E_SEAL_DVI_RSV_DUAL_P2_PROT_NONPM          =414,
    E_SEAL_HDCP_DUAL_P2_PROT_NONPM             =415,
    E_SEAL_DVI_DTOP_DUAL_P3_PROT_NONPM         =416,
    E_SEAL_DVI_RSV_DUAL_P3_PROT_NONPM          =417,
    E_SEAL_HDCP_DUAL_P3_PROT_NONPM             =418,
    E_SEAL_HDMI_DUAL_0_PROT_NONPM              =419,
    E_SEAL_HDMI2_DUAL_0_PROT_NONPM             =420,
    E_SEAL_HDMI_DUAL_1_PROT_NONPM              =421,
    E_SEAL_HDMI2_DUAL_1_PROT_NONPM             =422,
    E_SEAL_COMBO_GP_TOP_PROT_NONPM             =423,
    E_SEAL_USB30HS_UHC_PROT_NONPM              =424,
    E_SEAL_USB30HS1_UHC_PROT_NONPM             =425,
    E_SEAL_USB30HS_USBC_PROT_NONPM             =426,
    E_SEAL_USB30HS1_USBC_PROT_NONPM            =427,
    E_SEAL_USB30PHY1_DTOP_M_0_PROT_NONPM       =428,
    E_SEAL_USB30PHY1_ATOP_M_0_NONPM            =429,
    E_SEAL_MSPI_MCARD_PROT_NONPM               =430,
    E_SEAL_VP9_TOP_PROT_NONPM                  =431,
    E_SEAL_GPU_PLL_PROT_NONPM                  =432,
    E_SEAL_VIVALDI8_PROT_NONPM                 =433,
    E_SEAL_VIVALDI9_PROT_NONPM                 =434,
    E_SEAL_VIVALDIa_PROT_NONPM                 =435,
    E_SEAL_VIVALDIb_PROT_NONPM                 =436,
    E_SEAL_PM_PATGEN_PROT_PM                   =437,
    E_SEAL_SCDC_0_PROT_PM                      =438,
    E_SEAL_SCDC_1_PROT_PM                      =439,
    E_SEAL_SCDC_2_PROT_PM                      =440,
    E_SEAL_SCDC_3_PROT_PM                      =441,
    E_SEAL_SECURE_TZPC_PROT_NONPM              =442,
    E_SEAL_UP110_PORT_NONPM                    =443,
    E_SEAL_UP111_PORT_NONPM                    =444,
    E_SEAL_IDAC_NONPM                          =445,
    E_SEAL_DIPW_NONPM                          =446,
    E_SEAL_VE_DISC_NONPM                       =447,
    E_SEAL_VE_GAMA_NONPM                       =448,
    E_SEAL_GOP0G_0_PROT_NONPM                  =449,
    E_SEAL_GOP0G_1_PROT_NONPM                  =450,
    E_SEAL_GOP0G_ST_PROT_NONPM                 =451,
    E_SEAL_GOP3G_0_PROT_NONPM                  =452,
    E_SEAL_GOP3G_1_PROT_NONPM                  =453,
    E_SEAL_GOP3G_ST_PROT_NONPM                 =454,
    E_SEAL_PM_SPARE2_PROT_PM                   =455,
    E_SEAL_PM_SPARE3_PROT_PM                   =456,
    E_SEAL_HDMITX_PROT_NONPM                   =457,
    E_SEAL_HDCPTX_PROT_NONPM                   =458,
    E_SEAL_SDIO_PLL_PROT_NONPM                 =459,
    E_SEAL_TSP4_PROT_NONPM                     =460,
    E_SEAL_TSP5_PROT_NONPM                     =461,
    E_SEAL_MIIC_PM0_PROT_PM                    =462,
    E_SEAL_TZPC_PM_PROT_PM                     =463,
    E_SEAL_TS_SAMPLE_NONPM                     =464,
    E_SEAL_CLKGEN_SC_FE                        =465,
    E_SEAL_CLKGEN_SC_BE                        =466,
    E_SEAL_CLKGEN_SC_GP2                       =467,
    E_SEAL_SC_GPLUS                            =468,
    E_SEAL_SRAM_LDO                            =469,
    E_SEAL_VIVALDIa_0_PROT_NONPM               =470,
    E_SEAL_VIVALDIa_1_PROT_NONPM               =471,
    E_SEAL_VIVALDIa_2_PROT_NONPM               =472,
    E_SEAL_VIVALDIa_3_PROT_NONPM               =473,
    E_SEAL_MHL_ECBUS_PROT_NONPM                =474,
    E_SEAL_CBUS_AUDIO_PROT_NONPM               =475,
    E_SEAL_MHL_ECBUS_PHY_PROT_NONPM            =476,
    E_SEAL_L2_CACHE_PROT_NONPM                 =477,
    E_SEAL_PADTOP0_PROT_NONPM                  =478,
    E_SEAL_PADTOP1_PROT_NONPM                  =479,
    E_SEAL_MIPS_PATGEN_PROT_NONPM              =480,
    E_SEAL_ANA_MISC_MIPSPLL_PROT_NONPM         =481,
    E_SEAL_ANA_MISC_HDMI_PROT_NONPM            =482,
    E_SEAL_ANA_MISC_DSP_PROT_NONPM             =483,
    E_SEAL_RASP0_PROT_NONPM                    =484,
    E_SEAL_RASP1_PROT_NONPM                    =485,
    E_SEAL_RASP2_PROT_NONPM                    =486,
    E_SEAL_RASP3_PROT_NONPM                    =487,
    E_SEAL_HDNITX_MISC_PROT_NONPM              =488,
    E_SEAL_GOPMIX_PROT_NONPM                   =489,
    E_SEAL_DAC_PLL_PROT_NONPM                  =490,
    E_SEAL_SECEMAC0_PROT_NONPM                 =491,
    E_SEAL_SECEMAC1_PROT_NONPM                 =492,
    E_SEAL_SECEMAC2_PROT_NONPM                 =493,
    E_SEAL_SECEMAC3_PROT_NONPM                 =494,
    E_SEAL_SECEMAC4_PROT_NONPM                 =495,
    E_SEAL_VE_VTRACK_NONPM                     =496,
    E_SEAL_VE_P2I_NONPM                        =497,
    E_SEAL_RASP0_FILE_PROT_NONPM               =498,
    E_SEAL_RASP1_FILE_PROT_NONPM               =499,
    E_SEAL_MIU_KEY_PROT_NONPM                  =500,
    E_SEAL_PCM1_PROT_NONPM                     =501,
    E_SEAL_FI_QUEUE_PROT_NONPM                 =502,
    E_SEAL_CH34_MOD_PROT_NONPM                 =503,
    E_SEAL_BYTE_WR_PROT_NONPM                  =504,
    E_SEAL_ONEWAY_PROT_NONPM                   =505,
    E_SEAL_TSP6_PROT_NONPM                     =506,
    E_SEAL_TSP7_PROT_NONPM                     =507,
    E_SEAL_CA_MIUCROSSBAR_PROT_NONPM           =508,
    E_SEAL_EMM_FLT1_PROT_NONPM                 =509,
    E_SEAL_CA_PWD_PROT_NONPM                   =510,
    E_SEAL_EMM_FLT0_PROT_NONPM                 =511,
    E_SEAL_TSO0_PROT_NONPM                     =512,
    E_SEAL_KC_DESC_PROT_NONPM                  =513,
    E_SEAL_SEC_R2_CPU_PROT_NONPM               =514,
    E_SEAL_X32_NSK_NONPM                       =515,
    E_SEAL_X32_BRIDGE_NONPM                    =516,
    E_SEAL_X32_CIPHERCH0_NONPM                 =517,
    E_SEAL_X32_CIPHERCH1_NONPM                 =518,
    E_SEAL_X32_TSCEBANK0_NONPM                 =519,
    E_SEAL_X32_TSCEBANK1_NONPM                 =520,
    E_SEAL_X32_CRYPTODMA0_NONPM                =521,
    E_SEAL_X32_CRYPTODMA1_NONPM                =522,
    E_SEAL_X32_KEYTABLE_NONPM                  =523,
    E_SEAL_X32_KEYLADDER0_NONPM                =524,
    E_SEAL_X32_KEYLADDER1_NONPM                =525,
    E_SEAL_X32_KEYLADDER2_NONPM                =526,
    E_SEAL_X32_PROGPVR0_NONPM                  =527,
    E_SEAL_X32_PROGPVR1_NONPM                  =528,
    E_SEAL_X32_PROGPVR2_NONPM                  =529,
    E_SEAL_X32_PROGPVR3_NONPM                  =530,
    E_SEAL_X32_RSA_NONPM                       =531,
    E_SEAL_X32_MAILBOX_NONPM                   =532,
    E_SEAL_X32_OTP_CTRL_NONPM                  =533,
    E_SEAL_X32_OTP_PUB0_NONPM                  =534,
    E_SEAL_X32_OTP_PUB1_NONPM                  =535,
    E_SEAL_X32_OTP_PUB2_NONPM                  =536,
    E_SEAL_X32_OTP_PUB3_NONPM                  =537,
    E_SEAL_X32_NI_NONPM                        =538,
    E_SEAL_X32_AKL0_NONPM                      =539,
    E_SEAL_X32_AKL1_NONPM                      =540,
    E_SEAL_X32_XIUGEN_NONPM                    =541,
    E_SEAL_ONEWAY_PROT_PM                      =542,
    E_SEAL_SEC_PROT_PM                         =543,
    E_SEAL_UART5_PROT_NONPM                    =544,
    E_SEAL_SPARE6_PROT_NONPM                   =545,
    E_SEAL_SPARE7_PROT_NONPM                   =546,
    E_SEAL_USBC0_PROT_NONPM                    =547,
    E_SEAL_USBC1_PROT_NONPM                    =548,
    E_SEAL_USBC2_PROT_NONPM                    =549,
    E_SEAL_USBC3_PROT_NONPM                    =550,
    E_SEAL_SPI2FCIE_PROT_NONPM                 =551,
    E_SEAL_GPU_PLL_PROT_NONPN                  =552,
    E_SEAL_VIVALDIc_PROT_NONPM                 =553,
    E_SEAL_VIVALDId_PROT_NONPM                 =554,
    E_SEAL_DMD_MCU2_PROT_NONPM                 =555,
    E_SEAL_MIU_ARB_SC_NONPM                    =556,
    E_SEAL_MIU_ARB2_SC_NONPM                   =557,
    E_SEAL_VIVALDIb_0_PROT_NONPM               =558,
    E_SEAL_VIVALDIb_1_PROT_NONPM               =559,
    E_SEAL_VIVALDIb_2_PROT_NONPM               =560,
    E_SEAL_VIVALDIb_3_PROT_NONPM               =561,
    E_SEAL_PM_POR_PROT_PM                      =562,
    E_SEAL_TSP8_PROT_NONPM                     =563,
    E_SEAL_MCM_DIG_PROT_NONPM                  =564,
    E_SEAL_MCM_CODEC_PROT_NONPM                =565,
    E_SEAL_MCM_TSP_PROT_NONPM                  =566,
    E_SEAL_MCM_VIVALdi9_PROT_NONPM             =567,
    E_SEAL_MCM_SC_PROT_NONPM                   =568,
    E_SEAL_MCM_DMD_PROT_NONPM                  =569,
    E_SEAL_OTV23_PROT_NONPM                    =570,
    E_SEAL_OTV01_PROT_NONPM                    =571,
    E_SEAL_EVD_1_PROT_NONPM                    =572,
    E_SEAL_SMART2_PROT_NONPM                   =573,
    E_SEAL_SECMCU51_PROT_NONPM                 =574,
    E_SEAL_ANA_MISC_EVDPLL_MIUPLL_PROT_NONPM   =575,
    E_SEAL_ANA_MISC_MPLLTOP_PROT_NONPM         =576,
    E_SEAL_HI_VDR2_PROT_NONPM                  =577,
    E_SEAL_HI_VDR2_HIREG_PROT_NONPM            =578,
    E_SEAL_AU_R2_PROT_NONPM                    =579,
    E_SEAL_GOP_AFBC_PROT_NONPM                 =580,
    E_SEAL_MCU_ARM_PMU0_PROT_NONPM             =581,
    E_SEAL_GOPMIX_SD_PROT_NONPM                =582,
    E_SEAL_SPI_PRE_ARB_PROT_NONPM              =583,
    E_SEAL_SECGMAC0_PROT_NONPM                 =584,
    E_SEAL_SECGMAC1_PROT_NONPM                 =585,
    E_SEAL_SECGMAC2_PROT_NONPM                 =586,
    E_SEAL_SECGMAC3_PROT_NONPM                 =587,
    E_SEAL_SECGMAC4_PROT_NONPM                 =588,
    E_SEAL_PCIE_MAC_PROT_NONPM                 =589,
    E_SEAL_PCIE_AXI2MI_BRI_PROT_NONPM          =590,
    E_SEAL_USB30_SS_MIUPROT0_PROT_NONPM        =591,
    E_SEAL_USB30_HS0_MIUPROT1_PROT_NONPM       =592,
    E_SEAL_USB30_HS1_MIUPROT2_PROT_NONPM       =593,
    E_SEAL_CLKDET_PROT_NONPM                   =594,
    E_SEAL_EMACMIUPROT_PROT_NONPM              =595,
    E_SEAL_CALB_PROT_NONPM                     =596,
    E_SEAL_VIVALDIa_4_PROT_NONPM               =597,
    E_SEAL_VIVALDIa_5_PROT_NONPM               =598,
    E_SEAL_TSP9_PROT_NONPM                     =599,
    E_SEAL_TSP10_PROT_NONPM                    =600,
    E_SEAL_USBMIUPROT0_PROT_NONPM              =601,
    E_SEAL_USBMIUPROT1_PROT_NONPM              =602,
    E_SEAL_USBMIUPROT2_PROT_NONPM              =603,
    E_SEAL_DIG_TOP_PROT_NONPM                  =604,
    E_SEAL_PATGEN_ROOT_PROT_NONPM              =605,
    E_SEAL_CODEC_BLOCK_PROT_NONPM              =606,
    E_SEAL_HI_CODEC_BLOCK_PROT_NONPM           =607,
    E_SEAL_PATGEN_HI_CODEC_BLOCK_PROT_NONPM    =608,
    E_SEAL_SC_BLOCK_PROT_NONPM                 =609,
    E_SEAL_PATGEN_SC_PROT_NONPM                =610,
    E_SEAL_TSP_BLOCK_PROT_NONPM                =611,
    E_SEAL_VIVALDI9_BLOCK_PROT_NONPM           =612,
    E_SEAL_PATGEN_VIVALDI9_PROT_NONPM          =613,
    E_SEAL_GPU_BLOCK_PROT_NONPM                =614,
    E_SEAL_HI_CODEC_LITE_BLOCK_PROT_NONPM      =615,
    E_SEAL_PATGEN_HI_CODEC_BLOCK_LITE_NONPM    =616,
    E_SEAL_PATGEN_GPU_DIE_PROT_NONPM           =617,
    E_SEAL_HDCP22TX_PROT_NONPM                 =618,
    E_SEAL_HDMITX_PHY_PROT_NONPM               =619,
    E_SEAL_VMX_VMARK_SC0_PROT_NONPM            =620,
    E_SEAL_VMX_VMARK_SC1_PROT_NONPM            =621,
    E_SEAL_HDMI3_DUAL_0_PROT_NONPM             =622,
    E_SEAL_HDMITX2_PROT_NONPM                  =623,
    E_SEAL_X32_RNG_PROT_NONPM                  =624,
    E_SEAL_X32_GPUAPB_PROT_NONPM               =625,
    E_SEAL_X32_PCIE_MAC_PROT_NONPM             =626,
    E_SEAL_CRYPTODMA_GEN_SECURE_PROT_NONPM     =627,
    E_SEAL_CRYPTODMA_GEN_NON_SEcure_PROT_NONPM =628,
    E_SEAL_CRYPTODMA_SEC_R2_PROT_NONPM         =629,
    E_SEAL_CRYPTODMA_SEC_51_PROT_NONPM         =630,
    E_SEAL_PM51_TO_MIU_PROT_NONPM              =631,
    E_SEAL_ARM2MIU_NON_SECURE_PROT_NONPM       =632,
    E_SEAL_ARM2MIU_SECURE_PROT_NONPM           =633,
    E_SEAL_DEMOD_DMDMCU51_MIU_PROT_NONPM       =634,
    E_SEAL_ANA_MISC_MPOP_PROT_NONPM            =635,
    E_SEAL_CRYPTODMA_SEC_R2_PROT_NONPN         =636,
    E_SEAL_MSPI0_PROT_PM                       =637,
    E_SEAL_BYTE2WORD_PROT_PM                   =638,
    E_SEAL_FSC1_PROT_PM                        =639,
    E_SEAL_OTP_LDO_PROT_PM                     =640,
    E_SEAL_MFSC_PROT_NONPM                     =641,
    E_SEAL_PATGEN_FSC_PROT_NONPM               =642,
    E_SEAL_USB0_MIUPROT_PROT_NONPM             =643,
    E_SEAL_USB1_MIUPROT_PROT_NONPM             =644,
    E_SEAL_USB2_MIUPROT_PROT_NONPM             =645,
    E_SEAL_USB3_MIUPROT_PROT_NONPM             =646,
    E_SEAL_VIVALDIe_PROT_NONPM                 =647,
    E_SEAL_VIVALDIf_PROT_NONPM                 =648,
    E_SEAL_MIU_ARB3_SC_NONPM                   =649,
    E_SEAL_MIU_ARB_FRC_NONPM                   =650,
    E_SEAL_MIU_ARB2_FRC_NONPM                  =651,
    E_SEAL_DSCRMB6_PROT_NONPM                  =652,
    E_SEAL_DSCRMB7_PROT_NONPM                  =653,
    E_SEAL_CRI_KL_PROT_NONPM                   =654,
    E_SEAL_CERT_KL_PROT_NONPM                  =655,
    E_SEAL_X32_CRI_APB_PROT_NONPM              =656,
    E_SEAL_X32_CERT_PROT_NONPM                 =657, // E_SEAL_X32_CERT_APB_PROT_NONPM ?
    E_SEAL_COMB1_PROT_NONPM                    =658,
    E_SEAL_COMB2_PROT_NONPM                    =659,
    E_SEAL_COMB3_PROT_NONPM                    =660,
    E_SEAL_COMB4_PROT_NONPM                    =661,
    E_SEAL_COMB5_PROT_NONPM                    =662,
    E_SEAL_PDW0_PROT_NONPM                     =663,
    E_SEAL_PDW1_PROT_NONPM                     =664,
    E_SEAL_DIP_PROT_NONPM                      =665,
    E_SEAL_STR_PROT_PM                         =666,
    E_SEAL_TSP_SPARE_PROT_NONPM                =667,
    E_SEAL_CODEC_SPARE_PROT_NONPM              =668,
    E_SEAL_MHEG5_SPARE_PROT_NONPM              =669,
    E_SEAL_CCI400_PROT_NONPM                   =670,
    E_SEAL_ARM_CA72_PROT_NONPM                 =671,
    E_SEAL_SCPLL_PROT_NONPM                    =672,
    E_SEAL_TOP_SPARE0_PROT_NONPM               =673,
    E_SEAL_TOP_SPARE1_PROT_NONPM               =674,
    E_SEAL_MIPSPLL_2_PROT_NONPM                =675,
    E_SEAL_SC_GPLUS_TOP_PROT_NONPM             =676,
    E_SEAL_DSCRMB23_PROT_NONPM                 =677,
    E_SEAL_DSCRMB24_PROT_NONPM                 =678,
    E_SEAL_MFT_PROT_NONPM                      =679,
    E_SEAL_CMDQ_SUB_PROT_NONPM                 =680,
    E_SEAL_PATGEN_DEMOD1_PROT_NONPM            =681,
    E_SEAL_DEMOD_8_PROT_NONPM                  =682,
    E_SEAL_DEMOD_9_PROT_NONPM                  =683,
    E_SEAL_MIU_DIG_E_PROT_NONPM                =684,
    E_SEAL_MIU_DIG_E2_PROT_NONPM               =685,
    E_SEAL_MIU_DIG_E3_PROT_NONPM               =686,
    E_SEAL_MIU_DIG_E4_PROT_NONPM               =687,
    E_SEAL_DMD_TOP_1_PROT_NONPM                =688,
    E_SEAL_TSO2_PROT_NONPM                     =689,
    E_SEAL_GPU2MIU_MASK_PROT_NONPM             =690,
    E_SEAL_MIU_ARBB_NONPM                      =691,
    E_SEAL_MIU_ARBB2_NONPM                     =692,
    E_SEAL_MIU_ARBB3_NONPM                     =693,
    E_SEAL_MIU_ARBB4_NONPM                     =694,
    E_SEAL_DSCRMB25_PROT_NONPM                 =695,
    E_SEAL_DSCRMB26_PROT_NONPM                 =696,
    E_SEAL_DSCRMB27_PROT_NONPM                 =697,
    E_SEAL_MIU_ARB3_FRC_NONPM                  =698,
    E_SEAL_VIVALDIa_6_PROT_NONPM               =699,
    E_SEAL_HDMI3_DUAL_1_PROT_NONPM             =700,
    E_SEAL_X32_PKA_APB_PROT_NONPM              =701,
    E_SEAL_SC_GPLUS_TOP                        =702,
    E_SEAL_ZDEC_TOP_PROT_NONPM                 =703,
    E_SEAL_GS_PROT_NONPM                       =704,
    E_SEAL_STB_DC_PROT_NONPM                   =705,
    E_SEAL_SVP3_PROT_NONPM                     =706,
    E_SEAL_PM_DIG_TOP_PROT_PM                  =707,
    E_SEAL_BLK_DIG_PROT_NONPM                  =708,
    E_SEAL_PATGEN_DIG_PROT_NONPM               =709,
    E_SEAL_BLK_CODEC_PROT_NONPM                =710,
    E_SEAL_BLK_DMD0_PROT_NONPM                 =711,
    E_SEAL_BLK_HICODEC_PROT_NONPM              =712,
    E_SEAL_BLK_SC_PROT_NONPM                   =713,
    E_SEAL_BLK_TSP_PROT_NONPM                  =714,
    E_SEAL_BLK_VIVALDI9_PROT_NONPM             =715,
    E_SEAL_INTR_CTRL_CPU0_1_PROT_NONPM         =716,
    E_SEAL_INTR_CTRL_CPU1_1_PROT_NONPM         =717,
    E_SEAL_INTR_CTRL_CPU2_1_PROT_NONPM         =718,
    E_SEAL_INTR_CTRL_CPU3_1_PROT_NONPM         =719,
    E_SEAL_HVD1_PROT_NONPM                     =720,
    E_SEAL_INTR_CTRL_CPU0_0_PROT_NONPM         =721,
    E_SEAL_INTR_CTRL_CPU1_0_PROT_NONPM         =722,
    E_SEAL_INTR_CTRL_CPU2_0_PROT_NONPM         =723,
    E_SEAL_INTR_CTRL_CPU3_0_PROT_NONPM         =724,
    E_SEAL_SATA_GHC_PROT_NONPM                 =725,
    E_SEAL_SATA_P0_PROT_NONPM                  =726,
    E_SEAL_SATA_MISC_PROT_NONPM                =727,
    E_SEAL_SATA_MIUPROT_PROT_NONPM             =728,
    E_SEAL_SATA_PHY_PROT_NONPM                 =729,
    E_SEAL_INTR_CTRL_CPU4_0_PROT_NONPM         =730,
    E_SEAL_INTR_CTRL_CPU4_1_PROT_NONPM         =731,
    E_SEAL_MFDEC_PROT_NONPM                    =732,
    E_SEAL_CLKGEN_AUR2_0_PROT_NONPM            =733, // riu_clkgen_au_r2_20_prot_ns
    E_SEAL_PATGEN_AUR2_0_PROT_NONPM            =734,
    E_SEAL_CLKGEN_AUR2_1_PROT_NONPM            =735, // riu_clkgen_au_r2_21_prot_ns
    E_SEAL_PATGEN_AUR2_1_PROT_NONPM            =736,
    E_SEAL_GOP5G_0_PROT_NONPM                  =737,
    E_SEAL_GOP5G_1_PROT_NONPM                  =738,
    E_SEAL_GOP5G_ST_PROT_NONPM                 =739,
    E_SEAL_TZPC_MIU0_PROT_NONPM                =740,
    E_SEAL_TZPC_MIU1_PROT_NONPM                =741,
    E_SEAL_TZPC_HOSTID_NONPM_PROT_NONPM        =742,
    E_SEAL_IMONITOR_PROT_NONPM                 =743,
    E_SEAL_MIU_PROTECT_PROT_NONPM              =744,
    E_SEAL_MIU_PROTECT2_PROT_NONPM             =745,
    E_SEAL_PKA_PROT_NONPM                      =746,
    E_SEAL_PKA_BYTE2WORD_PROT_NONPM            =747,
    E_SEAL_TSIO0_PROT_NONPM                    =748,
    E_SEAL_TSIO1_PROT_NONPM                    =749,
    E_SEAL_TSIO2_PROT_NONPM                    =750,
    E_SEAL_TSIO3_PROT_NONPM                    =751,
    E_SEAL_TSO3_PROT_NONPM                     =752,
    E_SEAL_TSIO_LOC_DEC_PROT_NONPM             =753,
    E_SEAL_TSIO_PHY_PROT_NONPM                 =754,
    E_SEAL_VD_R2_SUBSYS_PROT_NONPM             =755,
    E_SEAL_X32_PIDSLOTMTN_PROT_NONPM           =756,
    E_SEAL_X32_KEYLADDER0_1_NONPM              =757,
    E_SEAL_X32_CA_MPROT_NONPM                  =758,
    E_SEAL_X32_CA_RPROT_NONPM                  =759,
    E_SEAL_X32_GMAC0_NONPM                     =760,
    E_SEAL_X32_GMAC1_NONPM                     =761,
    E_SEAL_X32_GMAC2_NONPM                     =762,
    E_SEAL_X32_EMAC0_NONPM                     =763,
    E_SEAL_X32_EMAC1_NONPM                     =764,
    E_SEAL_X32_EMAC2_NONPM                     =765,
    E_SEAL_X32_SATA_MAC_PROT_NONPM             =766,
    E_SEAL_TSP_PVR4_PROT_NONPM                 =767,
    E_SEAL_TSP_VQ_PROT_NONPM                   =768,
    E_SEAL_TSP_OR_PROT_NONPM                   =769,
    E_SEAL_TSP_SEC_PROT_NONPM                  =770,
    E_SEAL_TSP_FILEIN_ILLEGAL_PROT_NONPM       =771,
    E_SEAL_TSP_NON_SECURE_PROT_NONPM           =772,
    E_SEAL_STR_DATA_PM_PROT_PM                 =773,
    E_SEAL_MIU_BWFLAG_PROT_NONPM               =774,
    E_SEAL_MIU_BWFLAG2_PROT_NONPM              =775,
    E_SEAL_X32_CPUAPB0_NONPM                   =776,
    E_SEAL_PAD_MUX_NONPM                       =777,
    E_SEAL_ROM_CRC_NONPM                       =778,
    E_SEAL_SECURE_HDMITX_NONPM                 =779,
    E_SEAL_DEMOD_MIU_ARB_PROT_NONPM            =780,
    E_SEAL_DEMOD_BLOCK_PROT_NONPM              =781,
    E_SEAL_SECPVR_LUT_PROT_NONPM               =782,
    E_SEAL_PVR_FSC0_PROT_NONPM                 =783,
    E_SEAL_PVR_FSC1_PROT_NONPM                 =784,
    E_SEAL_PVR_FSC2_PROT_NONPM                 =785,
    E_SEAL_PVR_FSC3_PROT_NONPM                 =786,
    E_SEAL_X32_CFB_0_PROT_NONPM                =787,
    E_SEAL_X32_CFB_1_PROT_NONPM                =788,
    E_SEAL_AEON_I_PROT_NONPM                   =789,
    E_SEAL_DSP_I_PROT_NONPM                    =790,
    E_SEAL_SECU_51_I_PROT_NONPM                =791,
    E_SEAL_AU_R2_1_I_PROT_NONPM                =792,
    E_SEAL_AU_R2_0_I_PROT_NONPM                =793,
    E_SEAL_VD_R2_I_PROT_NONPM                  =794,
    E_SEAL_SECU_R2_I_PROT_NONPM                =795,
    E_SEAL_DMD51_I_PROT_NONPM                  =796,
    E_SEAL_ARM_I_NON_SECURE_PROT_NONPM         =797,
    E_SEAL_ARM_I_SECURE_PROT_NONPM             =798,
    E_SEAL_PM51_I_PROT_NONPM                   =799,
    E_SEAL_X32_SATA_MAC_01_PROT_NONPM          =800,
    E_SEAL_X32_SATA_MAC_02_PROT_NONPM          =801,
    E_SEAL_X32_SATA_MAC_03_PROT_NONPM          =802,
    E_SEAL_AEON_D_PROT_NONPM                   =803,
    E_SEAL_DSP_D_PROT_NONPM                    =804,
    E_SEAL_SECU_51_D_PROT_NONPM                =805,
    E_SEAL_AU_R2_1_D_PROT_NONPM                =806,
    E_SEAL_AU_R2_0_D_PROT_NONPM                =807,
    E_SEAL_VD_R2_D_PROT_NONPM                  =808,
    E_SEAL_SECU_R2_D_PROT_NONPM                =809,
    E_SEAL_DMD51_D_PROT_NONPM                  =810,
    E_SEAL_ARM_D_NON_SECURE_PROT_NONPM         =811,
    E_SEAL_ARM_D_SECURE_PROT_NONPM             =812,
    E_SEAL_PM51_D_PROT_NONPM                   =813,
    E_SEAL_IR_TX_PROT_PM                       =814,
    E_SEAL_IR_TX_MEM_PROT_PM                   =815,
    E_SEAL_IR2_PROT_PM                         =816,
    E_SEAL_MAILBOX_PQ_PROT_NONPM               =817,
    E_SEAL_BAT_PROT_NONPM                      =818,
    E_SEAL_SC_EXT_PROT_NONPM                   =819,
    E_SEAL_MSC_EXT_PROT_NONPM                  =820,
    E_SEAL_DWIN0_PROT_NONPM                    =821,
    E_SEAL_DWIN1_PROT_NONPM                    =822,
    E_SEAL_DIP_0_PROT_NONPM                    =823,
    E_SEAL_DIP_1_PROT_NONPM                    =824,
    E_SEAL_DDI_0_PROT_NONPM                    =825,
    E_SEAL_DDI_1_PROT_NONPM                    =826,
    E_SEAL_WFIMON_PROT_PM                      =827,
    E_SEAL_ANA_MISC_MIPSPLL1_PROT_NONPM        =828,
    E_SEAL_ANA_MISC_MIPSPLL2_PROT_NONPM        =829,
    E_SEAL_X32_MAILBOX_1_NONPM                 =830,
    E_SEAL_ANA_MISC_LPF_PROT_NONPM             =831,
    E_SEAL_ANA_MISC_DDR_SCAL_PROT_NONPM        =832,
    E_SEAL_GEMAC0_PROT_NONPM                   =833,
    E_SEAL_GEMAC1_PROT_NONPM                   =834,
    E_SEAL_GEMAC2_PROT_NONPM                   =835,
    E_SEAL_GEMAC3_PROT_NONPM                   =836,
    E_SEAL_GEMAC4_PROT_NONPM                   =837,
    E_SEAL_CH34_MOD1_PROT_NONPM                =838,
    E_SEAL_X32_GEMAC0_NONPM                    =839,
    E_SEAL_X32_GEMAC1_NONPM                    =840,
    E_SEAL_X32_GEMAC2_NONPM                    =841,
    E_SEAL_BLK_DIG_PM_PROT_PM                  =842,
	E_SEAL_IP_NUM                              =843
}eSeal_IP;

typedef enum
{
    E_SEAL_LOCK_NONE                           =0,
    E_SEAL_LOCK_DUMMY                          =1,
    E_SEAL_VD_R2_INST_BUF                      =2,
    E_SEAL_HK_R2_INST_BUF                      =3,
    E_SEAL_AUDIO_DEC_R2_INST_BUF               =4,
    E_SEAL_AUDIO_DEC_ES_BUF                    =5,
    E_SEAL_AUDIO_ENC_R2_INST_BUF               =6,
    E_SEAL_AUDIO_ENC_ES_BUF                    =7,
    E_SEAL_HVD_ES0_BUF                         =8,
    E_SEAL_HVD_ES1_BUF                         =9,
    E_SEAL_HVD_ES2_BUF                         =10,
    E_SEAL_MFE0_ES_BUF                         =11,
    E_SEAL_MFE1_ES_BUF                         =12,
    E_SEAL_TSP_AEON_INS_BUF                    =13,
    E_SEAL_PVR_BUF                             =14,
    E_SEAL_SECURE_RANGE                        =15,
    E_SEAL_SC_BUF                              =16,
    E_SEAL_GE_BUF                              =17,
    E_SEAL_SC_WP_DIPW                          =18,
    E_SEAL_SC_RP_DIPW                          =19,
    E_SEAL_SC_WP_DIPR                          =20,
    E_SEAL_SC_RP_DIPR                          =21,
    E_SEAL_CIPHERENG_WP_SYSKEY                 =22,
    E_SEAL_CIPHERENG_RP_SYSKEY                 =23,
    E_SEAL_DSCRMB_WP_RIV0                      =24,
    E_SEAL_DSCRMB_RP_RIV0                      =25,
    E_SEAL_DSCRMB_WP_RIV1                      =26,
    E_SEAL_DSCRMB_RP_RIV1                      =27,
    E_SEAL_DSCRMB_WP_WDATA                     =28,
    E_SEAL_DSCRMB_RP_WDATA                     =29,
    E_SEAL_DSCRMB_RP_RDATA                     =30,
    E_SEAL_RNG_PROTECT                         =31,
    E_SEAL_RSA_PROTECT                         =32,
    E_SEAL_SHA_PROTECT                         =33,
    E_SEAL_MVD_WR_PROTN_0                      =34,
    E_SEAL_MVD_WR_PROTN_1                      =35,
    E_SEAL_MVD_WR_PROTN_2                      =36,
    E_SEAL_EVD_0_WR_PROTN_0                    =37,
    E_SEAL_EVD_1_WR_PROTN_0                    =38,
    E_SEAL_MHEG5_WR_PROTN_0                    =39,
    E_SEAL_TSO_WP_TSOFI                        =40,
    E_SEAL_TSO_RP_TSOFI                        =41,
    E_SEAL_MMFI_WP_MMFI0                       =42,
    E_SEAL_MMFI_RP_MMFI0                       =43,
    E_SEAL_MMFI_WP_MMFI1                       =44,
    E_SEAL_MMFI_RP_MMFI1                       =45,
    E_SEAL_TSP0_WP_PVR                         =46,
    E_SEAL_TSP0_RP_PVR                         =47,
    E_SEAL_TSP0_WP_PVR1                        =48,
    E_SEAL_TSP0_RP_PVR1                        =49,
    E_SEAL_TSP0_WP_FILEIN                      =50,
    E_SEAL_TSP0_RP_FILEIN                      =51,
    E_SEAL_TSP0_WP_QMEM                        =52,
    E_SEAL_TSP0_RP_QMEM                        =53,
    E_SEAL_TSP0_WP_FW                          =54,
    E_SEAL_TSP0_RP_FW                          =55,
    E_SEAL_VE_WP                               =56,
    E_SEAL_VE_RP                               =57,
    E_SEAL_SC_WP_OD                            =58,
    E_SEAL_SC_RP_OD                            =59,
    E_SEAL_SC_WP_SCM_M                         =60,
    E_SEAL_SC_RP_SCM_M                         =61,
    E_SEAL_SC_WP_SCM_S                         =62,
    E_SEAL_SC_RP_SCM_S                         =63,
    E_SEAL_SC_WP_PDW0                          =64,
    E_SEAL_SC_RP_PDW0                          =65,
    E_SEAL_SC_WP_PDW1                          =66,
    E_SEAL_SC_RP_PDW1                          =67,
    E_SEAL_SC_WP_OPW                           =68,
    E_SEAL_SC_RP_OPW                           =69,
    E_SEAL_GOPD_PROTN                          =70,
    E_SEAL_GE0_SB_PROTN                        =71,
    E_SEAL_GE0_DB_PROTN                        =72,
    E_SEAL_GE1_SB_PROTN                        =73,
    E_SEAL_GE1_DB_PROTN                        =74,
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_0             =75,
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_1             =76,
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_2             =77,
    E_SEAL_AUDIO_LCL_R2_WR_PROTN_3             =78,
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_0             =79,
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_1             =80,
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_2             =81,
    E_SEAL_AUDIO_SCL_R2_WR_PROTN_3             =82,
    E_SEAL_SECURE_R2_WR_PROTN_0                =83,
    E_SEAL_SECURE_R2_WR_PROTN_1                =84,
    E_SEAL_SECURE_R2_WR_PROTN_2                =85,
    E_SEAL_SECURE_R2_WR_PROTN_3                =86,
    E_SEAL_EVD_R2_WR_PROTN_0                   =87,
    E_SEAL_EVD_R2_WR_PROTN_1                   =88,
    E_SEAL_EVD_R2_WR_PROTN_2                   =89,
    E_SEAL_EVD_R2_WR_PROTN_3                   =90,
    E_SEAL_HVD_R2_WR_PROTN_0                   =91,
    E_SEAL_HVD_R2_WR_PROTN_1                   =92,
    E_SEAL_HVD_R2_WR_PROTN_2                   =93,
    E_SEAL_HVD_R2_WR_PROTN_3                   =94,
    E_SEAL_AUDIO_DSP_ES_PROTN                  =95,
    E_SEAL_AUDIO_DSP_CACHE_PROTN               =96,
    E_SEAL_EVD_0_WR_PROTN_1                    =97,
    E_SEAL_EVD_1_WR_PROTN_1                    =98,
    E_SEAL_DSCRMB_WP_RIV2                      =99,
    E_SEAL_DSCRMB_RP_RIV2                      =100,
    E_SEAL_DSCRMB_WP_RIV3                      =101,
    E_SEAL_DSCRMB_RP_RIV3                      =102,
    E_SEAL_SC2_WP_SCM_M                        =103,
    E_SEAL_SC2_RP_SCM_M                        =104,
    E_SEAL_VP9_TOP                             =105,
    E_SEAL_TSP0_WP_FILEIN1                     =106,
    E_SEAL_TSP0_RP_FILEIN1                     =107,
    E_SEAL_TSP0_WP_PVR2                        =108,
    E_SEAL_TSP0_RP_PVR2                        =109,
    E_SEAL_TSP0_WP_FIQ                         =110,
    E_SEAL_TSP0_RP_FIQ                         =111,
    E_SEAL_MVOP_WP_TLB                         =112,
    E_SEAL_AUDIO_PAS_PROTN                     =113,
    E_SEAL_AUDIO_AL_PROTN                      =114,
    E_SEAL_TSP_WP_PVR3                         =115,
    E_SEAL_TSP_RP_PVR3                         =116,
    E_SEAL_TSP_WP_FILEIN2                      =117,
    E_SEAL_TSP_RP_FILEIN2                      =118,
    E_SEAL_TSP_WP_FILEIN3                      =119,
    E_SEAL_TSP_RP_FILEIN3                      =120,
    E_SEAL_TSP_WP_FILEIN4                      =121,
    E_SEAL_TSP_RP_FILEIN4                      =122,
    E_SEAL_TSP_WP_FILEIN5                      =123,
    E_SEAL_TSP_RP_FILEIN5                      =124,
    E_SEAL_CRI_KL_WP_N                         =125,
    E_SEAL_PCIERC_IBWP_PROTN                   =126,
    E_SEAL_PCIERC_OBWP_PROTN                   =127,
    E_SEAL_TSP_WP_FILEIN1                      =128,
    E_SEAL_TSP_RP_FILEIN1                      =129,
    E_SEAL_TSP_WP_FIQ0                         =130,
    E_SEAL_TSP_RP_FIQ0                         =131,
    E_SEAL_TSP_WP_PVR4                         =132,
    E_SEAL_TSP_RP_PVR4                         =133,
    E_SEAL_TSP_WP_FIQ1                         =134,
    E_SEAL_TSP_RP_FIQ1                         =135,
    E_SEAL_TSP_WP_ORZ                          =136,
    E_SEAL_TSP_RP_ORZ                          =137,
    E_SEAL_USB_MIUPROT_WP_N                    =138,
    E_SEAL_EMAC_MIUPROT_WP_N                   =139,
    E_SEAL_PCIE_MIUPROT_WP_N                   =140,
    E_SEAL_AU_HDMI_DMA_WP_N                    =141,
    E_SEAL_TSP_WP_VQ                           =142,
    E_SEAL_TSP_RP_VQ                           =143,
    E_SEAL_TSP_WP_SEC                          =144,
    E_SEAL_TSP_RP_SEC                          =145,
    E_SEAL_TSP_WP_FIQ2                         =146,
    E_SEAL_TSP_RP_FIQ2                         =147,
    E_SEAL_TSP_WP_FIQ3                         =148,
    E_SEAL_TSP_RP_FIQ3                         =149,
    E_SEAL_EVD_LITE_0_WR_PROTN_0               =150,
    E_SEAL_EVD_LITE_1_WR_PROTN_1               =151,
    E_SEAL_GOPG_WP_N                           =152,
    E_SEAL_DIP_PROTN                           =153,
    E_SEAL_HDGEN_WP_N                          =154,
    E_SEAL_HDGEN_RP_N                          =155,
    E_SEAL_HDGEN_DCS_MV_WP_N                   =156,
    E_SEAL_HDGEN_DCS_MV_RP_N                   =157,
    E_SEAL_VE_DCS_MV_WP_N                      =158,
    E_SEAL_VE_DCS_MV_RP_N                      =159,
    E_SEAL_LOCK_ECO                            =160,
    E_SEAL_VTRACK_PROTN                        =161,
    E_SEAL_FCIE_WR_PROT_ENABLE_0               =162,
    E_SEAL_FCIE_WR_PROT_ENABLE_1               =163,
    E_SEAL_FCIE_WR_PROT_ENABLE_2               =164,
    E_SEAL_FCIE_WR_PROT_ENABLE_3               =165,
    E_SEAL_HDCP_RIU_R_PROTN                    =166,
    E_SEAL_HDCP_XIU_R_PROTN                    =167,
    E_SEAL_HDCP_RIU_W_PROTN                    =168,
    E_SEAL_HDCP_XIU_W_PROTN                    =169,
    E_SEAL_FCIE_WR_PROT_0_LOCK                 =170,
    E_SEAL_FCIE_WR_PROT_1_LOCK                 =171,
    E_SEAL_FCIE_WR_PROT_2_LOCK                 =172,
    E_SEAL_FCIE_WR_PROT_3_LOCK                 =173,
    E_SEAL_LOCK_NUM                            =174
}eSeal_Lock;

typedef enum
{
    E_SEAL_OVERLAP                           =0,
    E_SEAL_CONTAIN                           =1
} eSeal_CheckSecureRangeType;

typedef enum
{
    E_SEAL_IPGROUP_VDEC_R2,
    E_SEAL_IPGROUP_VDEC_DECODER,
} eSeal_IPGroup;

typedef struct
{
    MS_BOOL bIsHit;
    MS_U8   u8MiuDev;
    MS_U8   u8SecureRangeId;
    MS_U8   u8ClientId;
    MS_BOOL bIsSecure;
    MS_BOOL bIsWrite;
    MS_U64  u64HitAddr;
}Seal_PortectInfo;

typedef struct
{
    MS_U8   u8MiuDev;
    MS_U8   u8SecureRangeId;
    MS_U64  u64StartAddr;
    MS_U64  u64EndAddr;
    MS_U32  u32Attribute;
}Seal_SecureRangeInfo;

typedef void (*SEAL_CB_FUNC)(InterruptNum eIntNum);
//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_INIT
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEAL_Init(void);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_SecureRangeSet(MS_U8 u8SecureRangeId ,MS_U64 u64StartAddr, MS_U64 u64EndAddr, MS_U32 u32Attribute);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_SecureRangeQuery(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId, Seal_SecureRangeInfo *pSecureRangeInfo);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_IMI_RangeSet(MS_U32 u32StartAddr, MS_U32 u32EndAddr, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_GetHittedInfo(MS_U8 u8MiuDev, Seal_PortectInfo *pInfo);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_SecureRangeLock(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_NonSecureProcessorSet(MS_U8 u8ProcessorId, MS_BOOL bNonEnable);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_NonSecureProcessorQuery(MS_U8 u8ProcessorId, MS_BOOL *bNonSecure);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_SecureSlaveSet(MS_U32 u32SlaveId, MS_BOOL bSecure);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_SecureSlaveQuery(MS_U32 u32SlaveId, MS_BOOL *pSecure);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_SecureMasterSet(MS_U32 u32MasterId, MS_BOOL bSecure);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_SecureMasterQuery(MS_U32 u32MasterId, MS_BOOL *pSecure);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_Seal_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_CONTROL
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_BufferLock(MS_U8 u8BufferLockId, MS_BOOL bLock);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_INT
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_ENInterrupt(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_INT
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_AttachCallbackFunc(SEAL_CB_FUNC pSEALCBFunc);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @ingroup G_SEAL_INT
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Seal_DispatchCallbackFunc(void);

MS_BOOL MDrv_Seal_CheckSecureRange(MS_PHY phyStartAddr, MS_U32 u32Length, eSeal_CheckSecureRangeType eCheckSecureRangeType);

MS_BOOL MDrv_Seal_QueryBufferLocked(eSeal_Lock eLockId, MS_BOOL* bLocked);

#ifdef MSOS_TYPE_OPTEE
MS_BOOL MDrv_Seal_ChangeIPSecureDMAAbillity(MS_U32 u32ModuleID, MS_U32 u32ModuleParameter, MS_U32 u32IsSecure);
MS_BOOL MDrv_Seal_SetSecureRange(MS_PHY u64phy, MS_U32 u32Length, MS_U32 u32IsSecure);
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_SEAL_H_
