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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regURDMA.h
//  Description: FAST UART DMA Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FAST_UART_DMA_H__
#define __FAST_UART_DMA_H__

//#if FAST_UART_DMA_ENABLE

//#include "fast_uart_dma_param.h" //halURDMA.h


#define Struct_Register __attribute__((packed))


/* Warning: Little Endian */
typedef struct dma_interface
{
    union
    {
        volatile MS_U16 reg00; /* 00h */
        struct
        {
            volatile MS_U16 sw_rst          :   1;  /* BIT0 */
            volatile MS_U16 urdma_mode      :   1;  /* BIT1 */
            volatile MS_U16 tx_urdma_en     :   1;  /* BIT2 */
            volatile MS_U16 rx_urdma_en     :   1;  /* BIT3 */
            volatile MS_U16 tx_endian       :   1;  /* BIT4 */
            volatile MS_U16 rx_endian       :   1;  /* BIT5 */
			volatile MS_U16 tx_sw_rst       :   1;  /* BIT6 */
            volatile MS_U16 rx_sw_rst       :   1;  /* BIT7 */
			volatile MS_U16 reserve00       :   3;  /* BIT8 ~ BIT10 */
            volatile MS_U16 rx_op_mode      :   1;  /* BIT11 */
            volatile MS_U16 tx_busy         :   1;  /* BIT12 */
            volatile MS_U16 rx_busy         :   1;  /* BIT13 */
            volatile MS_U16 reserve01       :   2;  /* BIT14 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space00;
#endif
    union
    {
        volatile MS_U16 reg01; /* 02h */
        struct
        {
            volatile MS_U16 intr_threshold  :   12; /* BIT0 ~ BIT11 */
            volatile MS_U16 reserve02       :   4;  /* BIT12 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space01;
#endif
    union
    {
        volatile MS_U16 reg02; /* 04h */
        struct
        {
            volatile MS_U16 tx_buf_base_h   :   8;  /* BIT0 ~ BIT7 */
            volatile MS_U16 reserve03       :   8;  /* BIT8 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space02;
#endif
    union
    {
        volatile MS_U16 reg03; /* 06h */
        struct
        {
            volatile MS_U16 tx_buf_base_l   :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space03;
#endif
    union
    {
        volatile MS_U16 reg04; /* 08h */
        struct
        {
            volatile MS_U16 tx_buf_size     :   13;  /* BIT0 ~ BIT12 */
            volatile MS_U16 reserve04       :   3;   /* BIT13 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space04;
#endif
    union
    {
        volatile MS_U16 reg05; /* 0Ah */
        struct
        {
            volatile MS_U16 tx_buf_rptr     :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space05;
#endif
    union
    {
        volatile MS_U16 reg06; /* 0Ch */
        struct
        {
            volatile MS_VIRT tx_buf_wptr     :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space06;
#endif
    union
    {
        volatile MS_U16 reg07; /* 0Eh */
        struct
        {
            volatile MS_U16 tx_timeout      :   4;   /* BIT0 ~ BIT3 */
            volatile MS_U16 reserve05       :   12;  /* BIT4 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space07;
#endif
    union
    {
        volatile MS_U16 reg08; /* 10h */
        struct
        {
            volatile MS_U16 rx_buf_base_h   :   8;  /* BIT0 ~ BIT7 */
            volatile MS_U16 reserve06       :   8;  /* BIT8 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space08;
#endif
    union
    {
        volatile MS_U16 reg09; /* 12h */
        struct
        {
            volatile MS_U16 rx_buf_base_l   :   16; /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space09;
#endif
    union
    {
        volatile MS_U16 reg0a; /* 14h */
        struct
        {
            volatile MS_U16 rx_buf_size     :   13; /* BIT0 ~ BIT12 */
            volatile MS_U16 reserve07       :   3;  /* BIT13 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space0a;
#endif
    union
    {
        volatile MS_U16 reg0b; /* 16h */
        struct
        {
            volatile MS_U16 rx_buf_wptr     :   16; /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space0b;
#endif
    union
    {
        volatile MS_U16 reg0c; /* 18h */
        struct
        {
            volatile MS_U16 rx_timeout      :   4;  /* BIT0 ~ BIT3 */
            volatile MS_U16 reserve08       :   12; /* BIT4 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space0c;
#endif
    union
    {
        volatile MS_U16 reg0d;  /* 1Ah */
        struct
        {
            volatile MS_U16 rx_intr_clr     :   1; /* BIT0 */
            volatile MS_U16 rx_intr1_en     :   1; /* BIT1 */
            volatile MS_U16 rx_intr2_en     :   1; /* BIT2 */
            volatile MS_U16 reserve09       :   1; /* BIT3 */
            volatile MS_U16 rx_intr1        :   1; /* BIT4 */
            volatile MS_U16 rx_intr2        :   1; /* BIT5 */
            volatile MS_U16 reserve0a       :   1; /* BIT6 */
            volatile MS_U16 rx_mcu_intr     :   1; /* BIT7 */
            volatile MS_U16 tx_intr_clr     :   1; /* BIT8 */
            volatile MS_U16 tx_intr_en      :   1; /* BIT9 */
            volatile MS_U16 reserve0b       :   5; /* BIT10 ~ BIT14 */
            volatile MS_U16 tx_mcu_intr     :   1; /* BIT15 */
        } Struct_Register;
    } Struct_Register;
} Struct_Register dma_interface_t;

//#endif /* #if FAST_UART_DMA_ENABLE */

#endif /* __FAST_UART_DMA_H__ */
