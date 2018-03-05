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
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
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
//  File name: regTSO.h
//  Description: TS I/O Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TSIO_REG_H_
#define _TSIO_REG_H_

//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// Addr                             Address
// Buf                              Buffer
// Clr                              Clear
// CmdQ                             Command queue
// Cnt                              Count
// Ctrl                             Control
// Flt                              Filter
// Hw                               Hardware
// Int                              Interrupt
// Len                              Length
// Ovfw                             Overflow
// Pkt                              Packet
// Rec                              Record
// Recv                             Receive
// Rmn                              Remain
// Reg                              Register
// Req                              Request
// Rst                              Reset
// Scmb                             Scramble
// Sec                              Section
// Stat                             Status
// Sw                               Software
// Ts                               Transport Stream
// MMFI                             Multi Media File In

//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------

#define TSIO_ENGINE_NUM                     (1)
#define TSIO_SERVICE_NUM                    (64)
#define TSIO_LOCKEY_LEN                     (16)
#define TSIO_MAX_SYNCTHRESHOLD              (16)
#define TSIO_MIN_SYNCTHRESHOLD              (1)
#define TSIO_SGDMAIN_PIDFLT_NUM             (512)
#define TSIO_FILTER_NUM                     (32)
#define TSIO_MIU_BUS                        (4)
#define TSIO_SVQ_UNIT_SIZE                  (200)

#define TSIO_LOCDEC_KEY_LEN                 (16)
#define TSIO_PID_NULL                       (0x1FFF)
#define TSIO_SERVICE_NULL                   (0xFF)

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------
#define BITTRAINING_SW_MODE_ENABLE          (0)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_SGDMAIN_PIDFLT_BASE             (0x220000UL)

#define REG_CTRL_BASE_TSIO0                 (0xE3400UL)                            // 0x171A
#define REG_CTRL_BASE_TSIO1                 (0xE3600UL)                            // 0x171B
#define REG_CTRL_BASE_TSIO2                 (0xE3800UL)                            // 0x171C
#define REG_CTRL_BASE_TSIO3                 (0xE7200UL)                            // 0x1739
#define REG_CTRL_BASE_TSIO_LOCDEC           (0xE3C00UL)                            // 0x171E
#define REG_CTRL_BASE_TSIO_PHY              (0xE3E00UL)                            // 0x171F

#define REG_CTRL_BASE_TSO0                  (0xE0C00UL)                            // 0x1706
#define REG_CTRL_BASE_TSO2                  (0xA7200UL)                            // 0x1539
#define REG_CTRL_BASE_TSO3                  (0xE3A00UL)                            // 0x171D

#define REG_CTRL_BASE_CLOCKGEN0             (0x01600UL)                            // 0x100B
#define REG_CTRL_BASE_CHIPTOP               (0x03C00UL)                            // 0x101E
#define REG_CTRL_BASE_STRLD                 (0x05200UL)                            // 0x1029

typedef struct _REG32_TSIO
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32_TSIO;

typedef struct _REG16_TSIO
{
    volatile MS_U16                data;
    volatile MS_U16                _resv;
} REG16_TSIO;


//CLOCKGEN0
typedef struct _REG_Ctrl_CLOCKGEN0
{
    REG16_TSIO                          RESERVED_00_24[37];         //00~24

    REG16_TSIO                          REG_CLKGEN0_TSIO;           //25
    #define REG_CLKGEN0_TSIO_DISABLE_CLOCK                          0x0100
    #define REG_CLKGEN0_TSIO_INVERT_CLOCK                           0x0200
    #define REG_CLKGEN0_TSIO_CLKSOURCE_MASK                         0x1C00
    #define REG_CLKGEN0_TSIO_CLKSOURCE_SHIFT                        10

    REG16_TSIO                          RESERVED_26_29[4];          //26~29

    REG16_TSIO                          REG_CLKGEN0_TSP;            //2A
    #define REG_CLKGEN0_TSP_DISABLE_CLOCK                           0x0001
    #define REG_CLKGEN0_TSP_INVERT_CLOCK                            0x0002
    #define REG_CLKGEN0_TSP_CLKSOURCE_MASK                          0x001C
    #define REG_CLKGEN0_TSP_CLKSOURCE_SHIFT                         2

}REG_Ctrl_CLOCKGEN0;

//CHIP TOP
typedef struct _REG_Ctrl_CHIPTOP
{
    REG16_TSIO                          RESERVED_00_53[84];         //00~53

    REG16_TSIO                          REG_TOP_TSIO;               //54
    #define REG_TOP_TSIO_TSP_BOOT_CLK_SEL                           0x0100

}REG_Ctrl_CHIPTOP;

//STRLD
typedef struct _REG_Ctrl_STRLD
{
    REG16_TSIO                          RESERVED_00_31[50];         //00~31

    REG16_TSIO                          REG_STRLD_32;               //32
    #define TEST_SDLDO_SEL_MASK                                     0x0030
    #define TEST_SDLDO_SEL_SHIFT                                    4
}REG_Ctrl_STRLD;

//TSIO0
typedef struct _REG_Ctrl_TSIO0
{
    REG16_TSIO                          SW_RSTZ;                    //00
    #define TSIO0_SW_RSTZ                                           0x0001
    #define TSIO0_SW_RST_ANA_TX                                     0x0002
    #define TSIO0_SW_RST_ANA_RX                                     0x0004

    REG16_TSIO                          TX_CONFIG0;                 //01
    #define TSIO0_STUFF_SVID_MASK                                   0x003F
    #define TSIO0_STUFF_SVID_SHIFT                                  0
    #define TSIO0_VALID_LOCKED_CLR                                  0x0040
    #define TSIO0_PKT_OVERFLOW_CLR                                  0x0080
    #define TSIO0_TSIO_TX_IDLE_CLR                                  0x0100
    #define TSIO0_TX_DIRECT_RX_8                                    0x0200
    #define TSIO0_CTS_DISABLE                                       0x0400
    #define TSIO0_INSERT_CTS_IN_TX                                  0x0800
    #define TSIO0_INSERT_EVEN_KEY_IN_TX                             0x1000
    #define TSIO0_INSERT_LE_EN_IN_TX                                0x2000
    #define TSIO0_TURN_OFF_MCM_TSIO                                 0x4000
    #define TSIO0_BYOASS_TSIOTX_FIFO                                0x8000

    REG16_TSIO                          TX_STATUS;                  //02

    REG16_TSIO                          ECO0;                       //03

    REG16_TSIO                          ECO1;                       //04

    REG16_TSIO                          CKG_TSP_TSIO;               //05
    #define TSIO0_CKG_TSP_TSIO                                      0x0001

    REG16_TSIO                          TX_SOC_DEFINE_VAL[4];       //06~09

    REG16_TSIO                          TX_CONFIG1;                 //0a
    #define TSIO0_STUFF_WHEN_LOSE_LOCK_EN                           0x0001

    REG16_TSIO                          RESERVED_0B_0F[5];          //0b~0f

    //C&C: h10~h3f
    REG16_TSIO                          TX_CC_CNTRL;                //10
    #define TSIO0_TX_CC_INT_EN                                      0x0002
    #define TSIO0_TX_CC_CLR                                         0x0004
    #define TSIO0_TX_CC_INT_CLR                                     0x0008
    #define TSIO0_TX_CC_START                                       0x8000

    REG16_TSIO                          TX_CC_CNTRL2;               //11
    #define TSIO0_TX_ACPU_ST                                        0x0001
    #define TSIO0_TX_ACPU_RW                                        0x0002
    #define TSIO0_TX_ACPU_CONT                                      0x0004
    #define TSIO0_TX_ACPU_ADDR_MASK                                 0xFF00
    #define TSIO0_TX_ACPU_ADDR_SHIFT                                8

    REG32_TSIO                          TX_CC_WDATA;                //12~13

    REG16_TSIO                          TX_CC_SIZE;                 //14
    #define TSIO0_CC_SIZE_MASK                                      0x01FF
    #define TSIO0_CC_SIZE_SHIFT                                     0

    REG16_TSIO                          RESERVED_15_17[3];          //15~17

    REG16_TSIO                          TX_CC_STATUS;               //18
    #define TSIO0_TX_CC_STATUS_TX_ACPU_ADDR_MASK                    0x00FF
    #define TSIO0_TX_CC_STATUS_TX_ACPU_ADDR_SHIFT                   0
    #define TSIO0_TX_CC_STATUS_TX_CC_DONE                           0x0100
    #define TSIO0_TX_CC_STATUS_INT_TX_CC                            0x8000

    REG32_TSIO                          TX_CC_RDATA;                //19~1a

    REG16_TSIO                          RESERVED_1B_1F[5];          //1b~1f

    REG16_TSIO                          RX_CC_CNTRL;                //20
    #define TSIO0_RX_CC_INT_EN                                      0x0002
    #define TSIO0_RX_CC_CLR                                         0x0004
    #define TSIO0_RX_CC_INT_CLR                                     0x0008
    #define TSIO0_RX_ERR_INT_EN_MASK                                0x0FF0
    #define TSIO0_RX_ERR_INT_EN_SHIFT                               4
    #define TSIO0_RX_CC_RECEIVE_EN                                  0x8000

    REG16_TSIO                          RX_CC_CNTRL2;               //21
    #define TSIO0_RX_ACPU_ST                                        0x0001
    #define TSIO0_RX_ACPU_RW                                        0x0002
    #define TSIO0_RX_ACPU_CONT                                      0x0004
    #define TSIO0_RX_ACPU_ADDR_MASK                                 0x0FF0
    #define TSIO0_RX_ACPU_ADDR_SHIFT                                4
    #define TSIO0_RX_ACPU_ADDR_CONT_TRI                             0x8000

    REG16_TSIO                          RESERVED_22_27[6];          //22~27

    REG16_TSIO                          RX_CC_STATUS;               //28
    #define TSIO0_RX_CC_DONE                                        0x0001
    #define TSIO0_FLAG_MID_IN_IDLE                                  0x0010
    #define TSIO0_FLAG_LAST_IN_IDLE                                 0x0020
    #define TSIO0_FLAG_FIRST_IN_MID                                 0x0040
    #define TSIO0_RX_CC_PROTOCOL                                    0x0100
    #define TSIO0_RX_CC_MISSED                                      0x0200
    #define TSIO0_RX_CC_OVERFLOW                                    0x0400
    #define TSIO0_ERR_FLAG_MASK                                     0x0770
    #define TSIO0_INT_RX_CC                                         0x1000

    REG32_TSIO                          RX_CC_RDATA;                //29~2a

    REG16_TSIO                          RX_CC_SIZE;                 //2b
    #define TSIO0_RX_CC_SIZE_MASK                                   0x01FF
    #define TSIO0_RX_CC_SIZE_SHIFT                                  0

    REG16_TSIO                          RESERVED_2C_3F[20];         //2c~3f

    //Power up handler: h40~h5f
    REG16_TSIO                          PUH_CONFIG0;                //40

    REG16_TSIO                          PUH_CONFIG1;                //41
    #define TSIO0_PUH_CONFIG1_VCC_POWER_GOOD_SET_HIGHT              0x0010
    #define TSIO0_PUH_CONFIG1_VCC_POWER_GOOD_SET_LOW                0x0020
    #define TSIO0_PUH_CONFIG1_TSIO_RX_DATA_VALID_SET_HIGH           0x0040

    REG16_TSIO                          MIN_PERIOD_OF_IDLE;         //42

    REG16_TSIO                          MAX_PERIOD_OF_IDLE;         //43

    REG16_TSIO                          MIN_PERIOD_OF_RST_PWR;      //44

    REG16_TSIO                          MAX_PERIOD_OF_RST_PWR;      //45

    REG16_TSIO                          MIN_PERIOD_OF_RST_CLK_PRE;  //46

    REG16_TSIO                          MAX_PERIOD_OF_RST_CLK_PRE;  //47

    REG16_TSIO                          MIN_PERIOD_OF_RST_CLK;      //48

    REG16_TSIO                          MAX_PERIOD_OF_RST_CLK;      //49

    REG16_TSIO                          RESERVED_4A_4F[6];          //4a~4f

    REG16_TSIO                          MIN_PERIOD_OF_SC_TRAINING;  //50

    REG16_TSIO                          MAX_PERIOD_OF_SC_TRAINING;  //51

    REG16_TSIO                          MIN_PERIOD_OF_SOC_TRAINING; //52

    REG16_TSIO                          MAX_PERIOD_OF_SOC_TRAINING; //53

    REG16_TSIO                          MIN_PERIOD_OF_PKT_SYNC;     //54

    REG16_TSIO                          MAX_PERIOD_OF_PKT_SYNC;     //55

    REG16_TSIO                          RETRY_THRESHOLD;            //56

    REG16_TSIO                          DELAY_CNT_SMALL;            //57

    REG16_TSIO                          DELAY_CNT_MID;              //58

    REG16_TSIO                          PUH_STATUS0;                //59
    #define TSIO0_PUH_STATUS0_OC_FLAG_TX_CH0                        0x0001
    #define TSIO0_PUH_STATUS0_OC_FLAG_TX_CH1                        0x0002
    #define TSIO0_PUH_STATUS0_NO_SUPPORT                            0x0004
    #define TSIO0_PUH_STATUS0_PKT_SYNC_TIMEOUT                      0x0008
    #define TSIO0_PUH_STATUS0_POWER_STATUS_MASK                     0x00F0
    #define TSIO0_PUH_STATUS0_POWER_STATUS_SHIFT                    4
    #define TSIO0_PUH_STATUS0_RETRY_TIMES                           0x0100
    #define TSIO0_PUH_STATUS0_OPERATION                             0x0200
    #define TSIO0_PUH_STATUS0_POWER_OFF                             0x0400
    #define TSIO0_PUH_STATUS0_POWER_ON                              0x0800

    REG16_TSIO                          PUH_CONFIG2;                //5a
    #define TSIO0_PUH_CONFIG2_OC_FLAG_TX_CH0_INT_CLR                0x0001
    #define TSIO0_PUH_CONFIG2_OC_FLAG_TX_CH0_INT_EN                 0x0002
    #define TSIO0_PUH_CONFIG2_OC_FLAG_TX_CH1_INT_CLR                0x0004
    #define TSIO0_PUH_CONFIG2_OC_FLAG_TX_CH1_INT_EN                 0x0008
    #define TSIO0_PUH_CONFIG2_NO_SUPPORT_CLR                        0x0010
    #define TSIO0_PUH_CONFIG2_NO_SUPPORT_EN                         0x0020
    #define TSIO0_PUH_CONFIG2_PKT_SYNC_TIMEOUT_CLR                  0x0040
    #define TSIO0_PUH_CONFIG2_PKT_SYNC_TIMEOUT_EN                   0x0080
    #define TSIO0_PUH_CONFIG2_RETRY_TIMES_CLR                       0x0100
    #define TSIO0_PUH_CONFIG2_RETRY_TIMES_EN                        0x0200
    #define TSIO0_PUH_CONFIG2_OPERATION_START_CLR                   0x0400
    #define TSIO0_PUH_CONFIG2_OPERATION_START_EN                    0x0800
    #define TSIO0_PUH_CONFIG2_POWER_OFF_CLR                         0x1000
    #define TSIO0_PUH_CONFIG2_POWER_OFF_EN                          0x2000
    #define TSIO0_PUH_CONFIG2_POWER_ON_CLR                          0x4000
    #define TSIO0_PUH_CONFIG2_POWER_ON_EN                           0x8000

    REG16_TSIO                          RESERVED_5B_5F[5];          //5b~5f

    //AD_IF:h60~h6b
    REG16_TSIO                          AD_TX_CONFIG0;              //60
    #define TSIO0_TX_DATA_PN_SWAP                                   0x0001
    #define TSIO0_CLK_PN_SWAP                                       0x0002
    #define TSIO0_CH_SWAP                                           0x0004
    #define TSIO0_TX_BIG_ENDIAN                                     0x0008
    #define TSIO0_TX2ATOP_STATUS_CLR                                0x0010
    #define TSIO0_BYPASS_AD_OUT_FIFO                                0x0020
    #define TSIO0_PRBS_TX_TSIO                                      0x0040
    #define TSIO0_PBRS_TX_ATOP                                      0x0080
    #define TSIO0_PD_SMC_LDO_FPGA                                   0x0100
    #define TSIO0_TX_BYTE_SWAP                                      0x0200

    REG16_TSIO                          AD_RX_CONFIG0;              //61

    REG16_TSIO                          AD_TX_ST;                   //62

    REG16_TSIO                          PRBS_CONFIG0;               //63

    REG16_TSIO                          DP_PHY_PRBS_ERRCNT;         //64

    REG16_TSIO                          PRBS_CONFIG1;               //65

    REG16_TSIO                          RESERVED_66_6B[6];          //66~6b

    //test mode:h6c~6f
    REG16_TSIO                          PUH_TEST_CONFIG;            //6c

    REG16_TSIO                          BTRAIN_TEST_CONFIG0;        //6d

    REG16_TSIO                          BTRAIN_TEST_CONFIG1;        //6e

    REG16_TSIO                          BTRAIN_TEST_CONFIG2;        //6f

    //bit training status: h70~h7f
    REG16_TSIO                          PH_INFO_0;                  //70

    REG16_TSIO                          PH_INFO_1;                  //71

    REG16_TSIO                          PH_INFO_2;                  //72
    #define TSIO0_PH_INFO_2_REG_PHASE_MASK                          0x003F
    #define TSIO0_PH_INFO_2_REG_PHASE_SHIFT                         0

    REG16_TSIO                          EL_INFO[3];                 //73~75

    REG16_TSIO                          CH_INFO;                    //76

    REG16_TSIO                          ATOP_IN;                    //77

    REG16_TSIO                          FT_STATUS;                  //78

    REG16_TSIO                          BTRAIN_ST_0;                //79
    #define TSIO0_BTRAIN_ST_0_SW_MODE_DONE                          0x0001
    #define TSIO0_BTRAIN_ST_0_SW_MODE_DONE_CH                       0x0002
    #define TSIO0_BTRAIN_ST_0_SW_REMAP_MODE                         0x0004
    #define TSIO0_BTRAIN_ST_0_CHG_PH_START_REMAP                    0x0008
    #define TSIO0_BTRAIN_ST_0_CHG_PH_START_REMAP_SHIFT              3

    REG16_TSIO                          RESERVED_7A_7F[6];          //7a~7f
} REG_Ctrl_TSIO0;

//TSIO1
typedef struct _REG_Ctrl_TSIO1
{
    REG16_TSIO                          SVID_SRCID[64];             //00~3f
    #define TSIO1_SOURCE_ID_MASK                                    0x00FF
    #define TSIO1_SOURCE_ID_SHIFT                                   0
    #define TSIO1_SERVICE_ID_MASK                                   0x3F00
    #define TSIO1_SERVICE_ID_SHIFT                                  8
    #define TSIO1_ENABLE                                            0x8000

    REG16_TSIO                          SVID_INFO[8];               //40~47
    #define TSIO1_LIVE_0_ENABLE                                     0x0001
    #define TSIO1_LIVE_1_ENABLE                                     0x0004
    #define TSIO1_LIVE_2_ENABLE                                     0x0010
    #define TSIO1_LIVE_3_ENABLE                                     0x0040
    #define TSIO1_LIVE_4_ENABLE                                     0x0100
    #define TSIO1_LIVE_5_ENABLE                                     0x0400
    #define TSIO1_LIVE_6_ENABLE                                     0x1000
    #define TSIO1_LIVE_7_ENABLE                                     0x4000

    REG16_TSIO                          RESERVED_48_4F[8];          //48~4f

    REG16_TSIO                          RX_CONFIG0;                 //50
    #define TSIO1_DECRYPT_DISABLE                                   0x0001
    #define TSIO1_SECURE_FLAG_DBG                                   0x0002
    #define TSIO1_BIT_TRAINING_BYPASS                               0x0004
    #define TSIO1_TX_DIRECT_RX_16                                   0x0008
    #define TSIO1_STUFF_RM_CLR                                      0x0010
    #define TSIO1_BYPASS_TSIORX_FIFO                                0x0080
    #define TSIO1_BEST_PH_OFFSET_MASK                               0x0700
    #define TSIO1_BEST_PH_OFFSET_SHIFT                              8
    #define TSIO1_BEST_PH_ADD                                       0x0800
    #define TSIO1_BEST_PH_MINUS                                     0x1000
    #define TSIO1_MAX_CDR_CNT_MASK                                  0xE000
    #define TSIO1_MAX_CDR_CNT_SHIFT                                 13

    REG16_TSIO                          PKT_SYNC_CTRL;              //51
    #define TSIO1_PKT_SYNC_EN                                       0x0001
    #define TSIO1_PKT_SYNC_CLR                                      0x0010

    REG16_TSIO                          MATCH_PATTERN;              //52
    #define TSIO1_MATCH_PATTERN_MASK                                0x00FF
    #define TSIO1_MATCH_PATTERN_SHIFT                               0

    REG16_TSIO                          TSIO_SYNC_THRESHOLD;        //53
    #define TSIO1_TSIO_N_SYNC_MASK                                  0x000F
    #define TSIO1_TSIO_N_SYNC_SHIFT                                 0
    #define TSIO1_TSIO_SYNC_CNT_MASK                                0x00F0
    #define TSIO1_TSIO_SYNC_CNT_SHIFT                               4

    REG32_TSIO                          MISSED_NUMOFBYTES;          //54~55

    REG16_TSIO                          PKT_SYNC_STATUS;            //56
    #define TSIO1_ANAFIFO_OVERFLOW                                  0x0001
    #define TSIO1_MISSED_MUCH_FLAG                                  0x0002
    #define TSIO1_EVER_LOSE_LOCK                                    0x0004
    #define TSIO1_PKT_SYNC_RX_DONE                                  0x0080
    #define TSIO1_STUFF_RM_OVERFLOW                                 0x8000

    REG32_TSIO                          TSIO_BIST_FAIL;             //57~58

    REG16_TSIO                          LOSE_LOCK_CNT;              //59

    REG16_TSIO                          RX_SOC_DEFINE_VAL[4];       //5a~5d

    REG16_TSIO                          RXANA_TO_PVR;               //5e
    #define TSIO1_RXANA_TO_PVR_EN                                   0x0001
    #define TSIO1_RXANA_WINDOW_INDEX_MASK                           0x00F0
    #define TSIO1_RXANA_WINDOW_INDEX_SHIFT                          4

    REG16_TSIO                          RESERVED_5F;                //5f

    //bit training: h60~h70
    REG16_TSIO                          CHECK_PH_TIME;              //60

    REG16_TSIO                          CHG_PH_STABLE_TIME;         //61
    #define TSIO1_CHG_PH_STABLE_TIME_GUARD_PH_MASK                  0x0F00
    #define TSIO1_CHG_PH_STABLE_TIME_GUARD_PH_SHIFT                 8
    #define TSIO1_CHG_PH_STABLE_TIME_GUARD_PH_LOAD                  0x1000

    REG32_TSIO                          DYN_ADJ_TIMER_PERIOD;       //62~63

    REG16_TSIO                          BTRAIN_CONFIG0;             //64
    #define TSIO1_SW_JUMP_PH_CH                                     0x4000

    REG16_TSIO                          BTRAIN_CONFIG1;             //65

    REG16_TSIO                          BTRAIN_CONFIG2;             //66

    REG16_TSIO                          BTRAIN_CONFIG3;             //67
    #define TSIO1_SW_PH_CHNAGE                                      0x0001
    #define TSIO1_SW_PH_INCR                                        0x0002
    #define TSIO1_SW_PH_DECR                                        0x0004
    #define TSIO1_SW_PH_STEP_MODE                                   0x0008
    #define TSIO1_SW_CH_PH_REMAP                                    0x0010
    #define TSIO1_SW_REMAP_WHEN_STUCK                               0x0020
    #define TSIO1_SW_EARLY                                          0x0040
    #define TSIO1_SW_LATE                                           0x0080
    #define TSIO1_SW_CHG_PH_DONE                                    0x0100
    #define TSIO1_BTRAIN_INT_CLR                                    0x0200
    #define TSIO1_DONE_EVER_CLR                                     0x0400
    #define TSIO1_CHANNEL_SEL_MASK                                  0x1800
    #define TSIO1_CHANNEL_SEL_SHIFT                                 11
    #define TSIO1_EL_INFO_SEL                                       0x2000
    #define TSIO1_PH_INFO_SEL                                       0x4000
    #define TSIO1_DEBUG_REG_LOAD                                    0x8000

    REG16_TSIO                          BTRAIN_INT_EN;              //68
    #define TSIO1_BTRAIN_INT_EN                                     0x0001

    REG16_TSIO                          BTRAIN_INT_STATUS;          //69
    #define TSIO1_GUARD_REACH_MIN                                   0x0001

    REG32_TSIO                          MAX_DYN_CDR_EL_TIMER;       //6a~6b

    REG16_TSIO                          DYN_CDR_EL_CNT;             //6c

    REG16_TSIO                          BTRAIN_CONFIG4;             //6d
    #define TSIO1_BTRAIN_CONFIG4_SW_MODE_CH_READ_ST_DIS             0x0001
    #define TSIO1_BTRAIN_CONFIG4_DISABLE_BITTRAIN                   0x0020

    REG16_TSIO                          FT_PRBS_TIMEOUT;            //6e

    REG16_TSIO                          BTRAIN_CONFIG5;             //6f
    #define TSIO1_BTRAIN_CONFIG5_MAX_EYE_REGION_MASK                0x003F
    #define TSIO1_BTRAIN_CONFIG5_MAX_EYE_REGION_SHIFT               0

    //unpack : h70
    REG16_TSIO                          UNPACK_CTRL;                //70
    #define TSIO1_BYPASS_ERRPKT_TSBLK_LIVEDMA                       0x0001
    #define TSIO1_BYPASS_ERRPKT_WITHOUT_SERVICEID                   0x0002
    #define TSIO1_BYPASS_ERRPKT_DMAEND                              0x0004
    #define TSIO1_DMAEND_DISABLE                                    0x0010
    #define TSIO1_CLR_ALL_ERR_FLAG                                  0x0080
    #define TSIO1_UNPACK_CTS_DISABLE                                0x0100

    REG32_TSIO                          UNPACK_STATUS;              //71~72

    REG16_TSIO                          UNDECLARE_SVID[4];          //73~76

    REG16_TSIO                          TSIORX_INT_EN;              //77

    REG16_TSIO                          DESYNC_CTRL;                //78

    REG16_TSIO                          RESERVED_79_7A[2];          //79~7a

    REG32_TSIO                          TSIO_INT_STATUS;            //7b~7c

    REG32_TSIO                          TSIO_DEBUG;                 //7d~7e
    #define D45_PH_INCR                                             0x00800000
    #define D45_PH_DECR                                             0x00400000
    #define D45_GUARD_PH_SMALL                                      0x00080000
    #define D45_GUARD_PH_LARGE                                      0x00040000

    REG16_TSIO                          TSIO_DBG_SEL;               //7f

} REG_Ctrl_TSIO1;

//TSIO2
typedef struct _REG_Ctrl_TSIO2
{
    REG16_TSIO                          TSIO_PVR_CONFIG;            //00
    #define TSIO2_PVR_STR2MI_EN                                     0x0001
    #define TSIO2_PVR_STR2MI_RST_WADR                               0x0002
    #define TSIO2_PVR_STR2MI_PAUSE                                  0x0004
    #define TSIO2_PVR_BURST_LEN_MASK                                0x0018
    #define TSIO2_PVR_BURST_LEN_SHIFT                               3
    #define TSIO2_PVR_SRAM_AD_EN                                    0x0020
    #define TSIO2_PVR_STR2MI_WP_LD                                  0x0040
    #define TSIO2_PVR_CLR                                           0x0080
    #define TSIO2_PVR_DMA_FLUSH_EN                                  0x0100
    #define TSIO2_PVR_MIU_HIGHPRI                                   0x0200
    #define TSIO2_PVR_WRITE_POINTER_TO_NEXT_ADDR_EN                 0x0400
    #define TSIO2_PVR_DMAW_PROTECT_EN                               0x0800
    #define TSIO2_PVR_CLR_NO_HIT_INT                                0x1000
    #define TSIO2_FLUSH_DATA_TSIO_PVR_STATUS                        0x2000

    REG32_TSIO                          TSIO_PVR_DMAW_WADDR_ERR;    //01~02

    REG16_TSIO                          RESERVED_03;                //03

    REG32_TSIO                          TSIO_PVR_STR2MI_WADR_R;     //04~05

    REG16_TSIO                          TSIO_PVR_FIFO_STATUS;       //06

    REG32_TSIO                          TSIO_PVR_DMAW_LBND;         //07~08

    REG32_TSIO                          TSIO_PVR_DMAW_UBND;         //09~0a

    REG16_TSIO                          TSIO_PVR2MI_SEL;            //0b

    REG16_TSIO                          RESERVED_0C_1F[20];         //0c~1f

    REG16_TSIO                          PVR_CTRL;                   //20

    REG16_TSIO                          PVR_CTRL_STATUS;            //21

    REG16_TSIO                          RESERVED_22;                //22

    REG16_TSIO                          SW_RST_SG_TSIO;             //23
    #define TSIO2_SW_RST_SG_TSIO_ROUNDROBIN                         0x0200
    #define TSIO2_SW_RST_SG_TSIO_SG_FSM                             0x0400
    #define TSIO2_SW_RST_SG_TSIO_VC_TABLE                           0x0800
    #define TSIO2_SW_RST_SG_TSIO_VCDMA_READ_MIU                     0x1000
    #define TSIO2_SW_RST_SG_TSIO_VC_FSM                             0x2000

    REG16_TSIO                          RESERVED_24_3D[26];         //24~3d

    REG32_TSIO                          SGDMAOUT_DBG;               //3e~3f
    #define TSIO2_SGDMAOUT_DBG_MASK                                 0x00FFFFFF
    #define TSIO2_SGDMAOUT_DBG_SHIFT                                0
    #define TSIO2_SGDMAOUT_DBG_SEL_MASK                             0xFF000000
    #define TSIO2_SGDMAOUT_DBG_SEL_SHIFT                            24

    REG16_TSIO                          VCFSM_START;                //40
    #define TSIO2_VCFSM_START                                       0x0001

    REG16_TSIO                          ACPU_ACTIVE;                //41
    #define TSIO2_ACPU_ACTIVE                                       0x0001

    REG16_TSIO                          ACPU_CMD;                   //42
    #define TSIO2_ACPU_CMD_VC_ID_MASK                               0x003F
    #define TSIO2_ACPU_CMD_VC_ID_SHIFT                              0
    #define TSIO2_ACPU_CMD_RD_POSITION_MASK                         0x01C0
    #define TSIO2_ACPU_CMD_RD_POSITION_SHIFT                        6
    #define TSIO2_ACPU_CMD_CLR                                      0x0200
    #define TSIO2_ACPU_CMD_RW                                       0x0400
    #define TSIO2_ACPU_CMD_ACTIVE                                   0x0800
    #define TSIO2_ACPU_CMD_RD_ADDR_LSB                              0x1000

    REG32_TSIO                          ACPU_WDATA;                 //43~44

    REG16_TSIO                          ACPU_FLAG;                  //45
    #define TSIO2_ACPU_FLAG_INTERRUPT_ENABLE                        0x0010
    #define TSIO2_ACPU_FLAG_MOBF_MASK                               0x03E0
    #define TSIO2_ACPU_FLAG_MOBF_SHIFT                              5
    #define TSIO2_ACPU_FLAG_SECURE_FLAG                             0x0400
    #define TSIO2_ACPU_FLAG_DMA_END_MASK                            0x0800
    #define TSIO2_ACPU_FLAG_DMA_END_CONTINUE_DMA                    0x1000

    REG32_TSIO                          ACPU_RDATA;                 //46~47

    REG16_TSIO                          SGDMA_OUT_CTRL;             //48
    #define TSIO2_SGDMA_OUT_INT_CLR                                 0x0001
    #define TSIO2_SGDMA_OUT_PAUSE                                   0x0002
    #define TSIO2_SGDMA_OUT_DBG_SEL_MASK                            0x00FC
    #define TSIO2_SGDMA_OUT_DBG_SEL_SHIFT                           2
    #define TSIO2_SGDMA_OUT_INT_MASK                                0x0100
    #define TSIO2_VCDMA_MIU_PRIORITY                                0x0200
    #define TSIO2_SGDMA_OUT_VC_INT_TRIGGER                          0x0400

    REG16_TSIO                          SGDMA_OUT_DBG;              //49

    REG16_TSIO                          SGDMA_OUT_VC_INT[4];        //4a~4d

    REG16_TSIO                          SGDMA_OUT_INFO;             //4e
    #define TSIO2_SGDMA_OUT_VC_INT_VC_ID_MASK                       0x003F
    #define TSIO2_SGDMA_OUT_VC_INT_VC_ID_SHIFT                      0
    #define TSIO2_SGDMA_OUT_VC_INT_CLR                              0x0040
    #define TSIO2_SGDMA_OUT_VC_INT_MASK                             0x0080
    #define TSIO2_PACE_DBG_VCID_MASK                                0x3F00
    #define TSIO2_PACE_DBG_VCID_SHIFT                               8
    #define TSIO2_PACE_DBG_CLR                                      0x4000
    #define TSIO2_PACE_DBG_EN                                       0x8000

    REG16_TSIO                          RESERVED_4F;                //4f

    REG16_TSIO                          SGDMA_OUT_VC_STATUS_SEL;    //50
    #define TSIO2_SGDMA_OUT_VC_STATUS_SEL_MASK                      0x00FC
    #define TSIO2_SGDMA_OUT_VC_STATUS_SEL_SHIFT                     2

    REG16_TSIO                          SGDMA_OUT_VC_STATUS;        //51
    #define TSIO2_SGDMA_OUT_VC_STATUS_LASTNODE                      0x0200
    #define TSIO2_SGDMA_OUT_VC_STATUS_NODEINT                       0x0400
    #define TSIO2_SGDMA_OUT_VC_STATUS_DROP                          0x0800
    #define TSIO2_SGDMA_OUT_VC_STATUS_DMAEND                        0x1000

    REG16_TSIO                          SGDMA_OUT_CONFIG0;          //52

    REG16_TSIO                          SGDMA_OUT_CONFIG1;          //53

    REG16_TSIO                          RESERVED_54_5F[12];         //54~5f

    REG32_TSIO                          TSIO_PVR_STR2MI_HEAD_DEBUG; //60~61

    REG32_TSIO                          TSIO_PVR_STR2MI_TAIL_DEBUG; //62~63

    REG16_TSIO                          TSIO_RXANA_TO_PVR_DEBUG;    //64
    #define TSIO2_TSIO_RXANA_TO_PVR_DEBG                            0x0001

    REG16_TSIO                          RESERVED_65_7F[27];         //65~7f
} REG_Ctrl_TSIO2;

//TSIO3
typedef struct _REG_Ctrl_TSIO3
{
    REG32_TSIO                          TSIO_FILTER[32];            //00~3f
    #define TSIO3_FILTER_ENABLE                                     0x00000001
    #define TSIO3_FILTER_SERVICE_ID_MASK                            0x0000007E
    #define TSIO3_FILTER_SERVICE_ID_SHIFT                           1
    #define TSIO3_FILTER_PID_MASK                                   0x000FFF10
    #define TSIO3_FILTER_PID_SHIFT                                  7
    #define TSIO3_FILTER_USE_SERVICE_ID_IN_FILTERING                0x00100000
    #define TSIO3_FILTER_USE_PID_IN_FILTERING                       0x00200000
    #define TSIO3_FILTER_DATA_TO_SM                                 0x00400000
    #define TSIO3_FILTER_DATA_TO_TSIO_RX                            0x00800000

    REG16_TSIO                          TSIO_WHITE_LIST;            //40
    #define TSIO3_WHITE_LIST_SM_ENABLE                              0x0001
    #define TSIO3_WHITE_LIST_RX_ENABLE                              0x0002

    REG16_TSIO                          TSIO3_CONFIG0;              //41
    #define TSIO3_BULK_ALWAYS_TO_SM                                 0x0001


    REG16_TSIO                          TSIO_PKT_MERGE_CONFIG;      //42
    #define TSIO3_NO_NEED_MERGE_BYPASS_SM                           0x0001
    #define TSIO3_CLR_PKT_LENGTH_FLAG                               0x0020

    REG16_TSIO                          TSIO_PKT_LENGTH;            //43
    #define TSIO3_PKT_LENGTH_MORE_212                               0x0001
    #define TSIO3_PKT_LENGTH_LESS_212                               0x0002

    REG16_TSIO                          TSIO_ONEWAY_MERGE;          //44
    #define TSIO3_TSIO_ONE_WAY_MERGRE                               0x0001

    REG16_TSIO                          TSIO_BYPASS_MERGE_DISABLE;  //45
    #define TSIO3_BYPASS_MERGE_DIABLE                               0x0001

    REG16_TSIO                          TSIO_SVID_BYPASS_SMC_EN[4]; //46~49

    REG16_TSIO                          RESERVED_4A_7F[54];         //4A~7f
} REG_Ctrl_TSIO3;

//TSIO_LOCDEC
typedef struct _REG_Ctrl_TSIO_LOCDEC
{
    REG16_TSIO                          SW_KEY[8];                  //00~07

    REG32_TSIO                          SW_KEY_PROPERTY;            //08~09

    REG16_TSIO                          CMD;                        //0a
    #define TSIO_LOCDEC_CMD_SERVICE_ID_MASK                         0x003F
    #define TSIO_LOCDEC_CMD_SERVICE_ID_SHIFT                        0
    #define TSIO_LOCDEC_CMD_KEY_SEL                                 0x0100
    #define TSIO_LOCDEC_CMD_CMD                                     0x0200
    #define TSIO_LOCDEC_CMD_CMD_FPGA                                0x0400

    REG16_TSIO                          KT_GO;                      //0b
    #define TSIO_LOCDEC_KT_GO                                       0x0001

    REG16_TSIO                          KT_DONE;                    //0c
    #define TSIO_LOCDEC_KT_DONE                                     0x0001

    REG16_TSIO                          SCBFIXRULE;                 //0d
    #define TSIO_LOCDEC_SCBFIXRULE                                  0x0001

    REG16_TSIO                          XIU;                        //0e
    #define TSIO_LOCDEC_XIU_ID_MASK                                 0x000F
    #define TSIO_LOCDEC_XIU_ID_SHIFT                                0
    #define TSIO_LOCDEC_XIU_NS                                      0x0010

    REG16_TSIO                          SECURE;                     //0f
    #define TSIO_LOCDEC_TO_SECURE                                   0x0001
    #define TSIO_LOCDEC_WRITE_SECURE_FORBID                         0x0010

    REG16_TSIO                          BYPASS_STATUS;              //10
    #define TSIO_LOCDEC_BPSTATUS_KEY_ENABLE_BIT                     0x0001
    #define TSIO_LOCDEC_BPSTATUS_EVEN_KEY_VLD                       0x0002
    #define TSIO_LOCDEC_BPSTATUS_EVEN_KEY_DECRYPT                   0x0004
    #define TSIO_LOCDEC_BPSTATUS_EVEN_KEY_ENTROPY                   0x0008
    #define TSIO_LOCDEC_BPSTATUS_EVEN_KEY_USAGE                     0x0010
    #define TSIO_LOCDEC_BPSTATUS_ODD_KEY_VLD                        0x0020
    #define TSIO_LOCDEC_BPSTATUS_ODD_KEY_DECRYPT                    0x0040
    #define TSIO_LOCDEC_BPSTATUS_ODD_KEY_ENTROPY                    0x0080
    #define TSIO_LOCDEC_BPSTATUS_ODD_KEY_USAGE                      0x0100
    #define TSIO_LOCDEC_BPSTATUS_AF_ONLY                            0x0200
    #define TSIO_LOCDEC_BPSTATUS_AF_LENGTH                          0x0400
    #define TSIO_LOCDEC_BPSTATUS_TS_MODE                            0x0800
    #define TSIO_LOCDEC_BPSTATUS_BULK_MODE                          0x1000
    #define TSIO_LOCDEC_BPSTATUS_BYPASS_EN                          0x2000
    #define TSIO_LOCDEC_BPSTATUS_BYPASS_VLD                         0x8000

    REG32_TSIO                          SCPU_CTRL;                  //11~12

    REG16_TSIO                          RESERVED_13;                //13

    REG16_TSIO                          KEY_ENABLE_BIT[4];          //14~17

    REG16_TSIO                          ODD_KEY_VLD[4];             //18~1b

    REG16_TSIO                          EVEN_KEY_VLD[4];            //1c~1f

    REG32_TSIO                          CHK_BANK_VERSION;           //20~21

    REG16_TSIO                          FORCESCB_BIT[4];            //22~25

    REG16_TSIO                          RESERVED_26_2F[10];         //26~2f

    REG16_TSIO                          DEBUG;                      //30
    #define TSIO_LOCDEC_DEBUG_ID_MASK                               0x003F
    #define TSIO_LOCDEC_DEBUG_ID_SHIFT                              0
    #define TSIO_LOCDEC_DEBUG_CLR                                   0x0080
    #define TSIO_LOCDEC_PKT_CNT_CLR                                 0x0100
    #define TSIO_LOCDEC_BYPASS_CLR                                  0x0200

    REG16_TSIO                          KT_ERROR;                   //31
    #define TSIO_LOCDEC_KT_ERROR_NO_ENABLE                          0x0001
    #define TSIO_LOCDEC_KT_ERROR_NO_KEY                             0x0002
    #define TSIO_LOCDEC_KT_ERROR_ERR_ALGO                           0x0010
    #define TSIO_LOCDEC_KT_ERROR_ERR_ENTROPY                        0x0020
    #define TSIO_LOCDEC_KT_ERROR_ERR_ENCDEC                         0x0040

    REG16_TSIO                          KT_WARNING;                 //32
    #define TSIO_LOCDEC_KT_WARNING_ERR_SYNC                         0x0001
    #define TSIO_LOCDEC_KT_WARNING_NONE_212                         0x0002

    REG16_TSIO                          RESERVED_33_37[5];          //33~37

    REG16_TSIO                          PKT_CNT;                    //38
    #define TSIO_LOCDEC_DEC_PKT_CNT_MASK                            0x00FF
    #define TSIO_LOCDEC_DEC_PKT_CNT_SHIFT                           0
    #define TSIO_LOCDEC_CLR_PKT_CNT_MASK                            0xFF00
    #define TSIO_LOCDEC_CLR_PKT_CNT_SHIFT                           8

    REG16_TSIO                          INPUT_PKT_CNT;              //39
    #define TSIO_LOCDEC_INPUT_PKT_CNT_MASK                          0x00FF
    #define TSIO_LOCDEC_INPUT_PKT_CNT_SHIFT                         0

    REG16_TSIO                          RESERVED_3A_3F[6];          //3a~3f

    REG16_TSIO                          NO_EN_IRQ_EN[4];            //40~43

    REG16_TSIO                          ERROR_IRQ_EN[4];            //44~47

    REG16_TSIO                          RESERVED_48_4F[8];          //48~4f

    REG16_TSIO                          NO_EN_IRQ_STA[4];           //50~53

    REG16_TSIO                          ERROR_IRQ_STA[4];           //54~57

    REG16_TSIO                          WARNING_STA[4];             //58~5b

    REG16_TSIO                          RESERVED_5C_5F[4];          //5c~5f

    REG16_TSIO                          FPGA_READ[8];               //60~67

    REG16_TSIO                          SLOT_SECURE[4];             //68~6b

    REG16_TSIO                          SLOT_PRIVILEGE[4];          //6c~6f

    REG16_TSIO                          RESERVED_70_7F[16];         //70~7f
} REG_Ctrl_TSIO_LOCDEC;

typedef struct _REG_Ctrl_TSIO_PHY
{
    REG16_TSIO                          TSIO_PHY_00;                //0x00
    #define TSIO_PHY_00_EN_RX_TERM                                  0x0001
    #define TSIO_PHY_00_EN_TX_SKEWCLK                               0x0002
    #define TSIO_PHY_00_GCR_CKEN_RX                                 0x0004
    #define TSIO_PHY_00_GCR_CKEN_RX_GUARD                           0x0008
    #define TSIO_PHY_00_GCR_CKEN_TX                                 0x0010
    #define TSIO_PHY_00_EN_REG                                      0x0020
    #define TSIO_PHY_00_EN_TX_SKEW                                  0x0040
    #define TSIO_PHY_00_REG_REF_SEL                                 0x0080
    #define TSIO_PHY_00_TX_CAL_EN                                   0x0100
    #define TSIO_PHY_00_TX_CAL_SRC                                  0x0200
    #define TSIO_PHY_00_PDN_TSIO_RX_PREAMP                          0x0400
    #define TSIO_PHY_00_PDN_TSIO_RX_TED                             0x0800
    #define TSIO_PHY_00_PD_IB_TSIO                                  0x1000
    #define TSIO_PHY_00_PD_RX_REF                                   0x2000
    #define TSIO_PHY_00_PD_TX_OCP                                   0x4000
    #define TSIO_PHY_00_START_RX_CAL                                0x8000

    REG16_TSIO                          TSIO_PHY_01;                //0x01

    REG16_TSIO                          TSIO_PHY_02;                //0x02
    #define TSIO_PHY_02_TEST_REG_MASK                               0x0300
    #define TSIO_PHY_02_TEST_REG_SHIFT                              8

    REG16_TSIO                          TSIO_PHY_03;                //0x03

    REG16_TSIO                          RESERVED_04_05[2];          //0x04 ~0x05

    REG16_TSIO                          TSIO_PHY_06;                //0x06
    #define TSIO_PHY_06_GCR_RX_CA_DA_OV_MASK                        0x007F
    #define TSIO_PHY_06_GCR_RX_CA_DA_OV_SHIFT                       0
    #define TSIO_PHY_06_GCR_RX_CA_DB_OV_MASK                        0x7F00
    #define TSIO_PHY_06_GCR_RX_CA_DB_OV_SHIFT                       8

    REG16_TSIO                          RESERVED_07_0A[4];          //0x07 ~0x0A

    REG16_TSIO                          TSIO_PHY_0B;                //0x0B
    #define TSIO_PHY_0B_RX_CAL_DATA_A_MASK                          0x003F
    #define TSIO_PHY_0B_RX_CAL_DATA_A_SHIFT                         0
    #define TSIO_PHY_0B_RX_CAL_DATA_B_MASK                          0x3F00
    #define TSIO_PHY_0B_RX_CAL_DATA_B_SHIFT                         8
    #define TSIO_PHY_0B_RX_CAL_END                                  0x8000
    #define TSIO_PHY_0B_RX_CAL_END_SHIFT                            15

    REG16_TSIO                          RESERVED_0C_0E[3];          //0x0C ~0x0E

    REG16_TSIO                          TSIO_PHY_0F;                //0x0F
    #define TSIO_PHY_0F_HW_CAL_BIAS_CON_MASK                        0x001F
    #define TSIO_PHY_0F_HW_CAL_BIAS_CON_SHIFT                       0

    REG16_TSIO                          RESERVED_10_11[2];          //0x10~0x11

    REG16_TSIO                          TSIO_PHY_12;                //0x12

    REG16_TSIO                          TSIO_PHY_13;                //0x13

    REG16_TSIO                          RESERVED_14_17[4];          //0x14~0x17

    REG16_TSIO                          TSIO_PHY_18;                //0x18
    #define TSIO_PHY_18_TEST_TSIO_MASK                              0x00FF
    #define TSIO_PHY_18_TEST_TSIO_SHIFT                             0

    REG16_TSIO                          RESERVED_19_1A[2];          //0x19~0x1A

    REG16_TSIO                          TSIO_PHY_1B;                //0x1B

    REG16_TSIO                          RESERVED_1C_1F[4];          //0x1C~0x1F

    REG16_TSIO                          TSIO_PHY_20;                //0x20
    #define TSIO_PHY_20_LPLL_PD                                     0x0001
    #define TSIO_PHY_20_LPLL_DMY_SKEW2                              0x0002
    #define TSIO_PHY_20_LPLL_DMY_SKEW3                              0x0004
    #define TSIO_PHY_20_LPLL_DMY_SKEW4                              0x0008
    #define TSIO_PHY_20_LPLL_PHDAC_RST                              0x0010
    #define TSIO_PHY_20_LPLL_PHDAC_SELECT                           0x0020
    #define TSIO_PHY_20_LPLL_PHDAC_UPDATE                           0x0040
    #define TSIO_PHY_20_LPLL_DUAL_LP_EN                             0x0080
    #define TSIO_PHY_20_LPLL_SEL_SKEW1_DELAYlt                      0x0100
    #define TSIO_PHY_20_LPLL_SEL_SKEW2_DELAYlt                      0x0200
    #define TSIO_PHY_20_LPLL_SEL_SKEW3_DELAYlt                      0x0400
    #define TSIO_PHY_20_LPLL_SEL_SKEW4_DELAYlt                      0x0800
    #define TSIO_PHY_20_LPLL_SKEW_EN_FIXCLK                         0x1000
    #define TSIO_PHY_20_LPLL_SKEW_EN_SKEWCLK                        0x2000
    #define TSIO_PHY_20_LPLL_SYN_CLKIN                              0x4000
    #define TSIO_PHY_20_LPLL_XTAL_LV                                0x8000

    REG16_TSIO                          TSIO_PHY_21;                //0x21
    #define TSIO_PHY_21_EN_DDR                                      0x0001
    #define TSIO_PHY_21_EN_MINI                                     0x0002
    #define TSIO_PHY_21_LPLL_RX_CLKIN_10                            0x0004
    #define TSIO_PHY_21_LPLL_EN_FT_BACKUP                           0x0008

    REG16_TSIO                          TSIO_PHY_22;                //0x22
    #define TSIO_PHY_22_LPLL_INPUT_DIV_FIRST_MASK                   0x0003
    #define TSIO_PHY_22_LPLL_INPUT_DIV_FIRST_SHIFT                  0
    #define TSIO_PHY_22_LPLL_LOOP_DIV_FIRST_MASK                    0x000C
    #define TSIO_PHY_22_LPLL_LOOP_DIV_FIRST_SHIFT                   2
    #define TSIO_PHY_22_LPLL_LOOP_DIV_SECOND_MASK                   0x00F0
    #define TSIO_PHY_22_LPLL_LOOP_DIV_SECOND_SHIFT                  4
    #define TSIO_PHY_22_LPLL_ICTRL_MASK                             0x0700
    #define TSIO_PHY_22_LPLL_ICTRL_SHIFT                            8
    #define TSIO_PHY_22_LPLL_SKEW_DIV_MASK                          0x7000
    #define TSIO_PHY_22_LPLL_SKEW_DIV_SHIFT                         12
    #define TSIO_PHY_22_LPLL_SW_DEBUG_EN                            0x8000

    REG16_TSIO                          TSIO_PHY_23;                //0x23
    #define TSIO_PHY_23_LPLL_SKEW1_FINE_MASK                        0x000F
    #define TSIO_PHY_23_LPLL_SKEW1_FINE_SHIFT                       0
    #define TSIO_PHY_23_LPLL_SKEW4_FINE_MASK                        0x00F0
    #define TSIO_PHY_23_LPLL_SKEW4_FINE_SHIFT                       4
    #define TSIO_PHY_23_LPLL_SKEW2_FINE_MASK                        0x0F00
    #define TSIO_PHY_23_LPLL_SKEW2_FINE_SHIFT                       8
    #define TSIO_PHY_23_LPLL_SKEW3_FINE_MASK                        0xF000
    #define TSIO_PHY_23_LPLL_SKEW3_FINE_SHIFT                       12

    REG16_TSIO                          TSIO_PHY_24;                //0x24
    #define TSIO_PHY_24_LPLL_SKEW1_COARSE_MASK                      0x001F
    #define TSIO_PHY_24_LPLL_SKEW1_COARSE_SHIFT                     0
    #define TSIO_PHY_24_LPLL_SKEW2_COARSE_MASK                      0x1F00
    #define TSIO_PHY_24_LPLL_SKEW2_COARSE_SHIFT                     8

    REG16_TSIO                          TSIO_PHY_25;                //0x25
    #define TSIO_PHY_24_LPLL_SKEW4_COARSE_MASK                      0x001F
    #define TSIO_PHY_24_LPLL_SKEW4_COARSE_SHIFT                     0
    #define TSIO_PHY_24_LPLL_SKEW3_COARSE_MASK                      0x1F00
    #define TSIO_PHY_24_LPLL_SKEW3_COARSE_SHIFT                     8

    REG16_TSIO                          RESERVED_26_2F[10];         //0x26~0x2F

    REG32_TSIO                          TSIO_PHY_30_31;             //0x30~31
    #define TSIO_PHY_30_31_LPLL_SYNTH_SET_MASK                      0x00FFFFFF
    #define TSIO_PHY_30_31_LPLL_SYNTH_SET_SHIFT                     0

}REG_Ctrl_TSIO_PHY;


typedef struct _REG_Ctrl_TSO0 //for sgdmain pidflt
{
    REG16_TSIO                          RESERVED_00_43[68];         //0x00~0x43

    REG16_TSIO                          TSO_CONFIG5;                //0x44
    #define TSO0_TSIO2OPIF                                          0x0800
    #define TSO0_TSIO_MODE                                          0x0400

    REG16_TSIO                          PDTABLE_ADDR_L;             //45 ind R/W of L addr to pdtable
    REG16_TSIO                          PDTABLE_ADDR_H;             //46 ind R/W of H addr to pdtable

    REG16_TSIO                          PDTABLE_WDATA_L;            //47 ind R/W of L addr to pdtable
    REG16_TSIO                          PDTABLE_WDATA_H;            //48 ind R/W of L addr to pdtable
    #define TSO0_PID_MASK                                           0x00001FFF
    #define TSO0_PID_SHIFT                                          0
    #define TSO0_ONE_MASK                                           0x0000E000
    #define TSO0_ONE_SHIFT                                          13
    #define TSO0_SVID_MASK                                          0x003F0000
    #define TSO0_SVID_SHIFT                                         16
    #define TSO0_ZERO_MASK                                          0xFFC00000
    #define TSO0_ZERO_SHIFT                                         22

    REG16_TSIO                          PDTABLE_RDATA;              //49 ind of Rdata from pdtable

    REG16_TSIO                          PDTABLE_EN;                 //4A
    #define TSO0_PDTABLE_W_EN                                       0x0001
    #define TSO0_PDTABLE_R_EN                                       0x0002

    REG16_TSIO                          RESERVED_4b_7F[53];         //4B~7F
}REG_Ctrl_TSO0;

typedef struct _REG_Ctrl_TSO2 //Part of SGDMA_in
{
    REG16_TSIO                          RESERVED_00_50[81];         //00~50

    REG16_TSIO                          SG_PDFLT_SVID_EN[4];        //51~54
    REG16_TSIO                          SG_PDTABLE_RDATA;           //55 ind of Rdata from pdtable
    REG16_TSIO                          SG_PDTABLE_RDATA_H;         //56 ind of Rdata from pdtable
    #define TSO2_PDTABLE_RDATA_H_MASK           0x003F

    REG16_TSIO                          RESERVED_57_7F[41];         //57~7F
}REG_Ctrl_TSO2;

typedef struct _REG_Ctrl_TSO3 //SGDMA_in
{
    REG16_TSIO                          TSO3_CTRL;                  //00
    #define TSO3_MEM_TS_W_ORDER                                     0x0001
    #define TSO3_FILE_PAUSE                                         0x0002
    #define TSO3_RADDR_LD                                           0x0004
    #define TSO3_DIS_MIU_RQ                                         0x0008
    #define TSO3_BYTE_TIMER_EN                                      0x0010
    #define TSO3_MEM_TS_DATA_ENDIAN                                 0x0020
    #define TSO3_SGFI2MI_PRIORITY                                   0x0040

    REG16_TSIO                          BYTE_TIMER;                 //01
    #define TSO3_BYTE_TIMER_MASK                                    0x00FF
    #define TSO3_BYTE_TIMER_SHIFT                                   0

    REG16_TSIO                          SGCTRL;                     //02
    #define TSO3_SGCTRL_RSTART                                      0x0001
    #define TSO3_SGCTRL_RDONE                                       0x0002
    #define TSO3_SGCTRL_BYTECNT_MASK                                0xFF00
    #define TSO3_SGCTRL_BYTECNT_SHIFT                               8

    REG16_TSIO                          RESERVED_03_07[5];          //03~07

    REG32_TSIO                          SGVQ1_BASE;                 //08~09

    REG16_TSIO                          SGVQ1_SIZE_200BYTE;         //0a

    REG16_TSIO                          SGVQ1_TX_CONFIG;            //0b
    #define TSO3_TX_CONFIG_WR_THRESHOLD_MASK                        0x000F
    #define TSO3_TX_CONFIG_WR_THRESHOLD_SHIFT                       0
    #define TSO3_TX_CONFIG_PRIORITY_THRESHOLD_MASK                  0x00F0
    #define TSO3_TX_CONFIG_PRIORITY_THRESHOLD_SHIFT                 4
    #define TSO3_TX_CONFIG_FORCEFIRE_CNT_MASK                       0x0F00
    #define TSO3_TX_CONFIG_FORCEFIRE_CNT_SHIFT                      8
    #define TSO3_TX_CONFIG_TX_RESET                                 0x1000
    #define TSO3_TX_CONFIG_OVERFLOW_INT_EN                          0x2000
    #define TSO3_TX_CONFIG_OVERFLOW_CLR                             0x4000
    #define TSO3_TX_CONFIG_SVQ_TX_ENABLE                            0x8000

    REG16_TSIO                          SGVQ1_TX_CONFIG_2;          //0c
    #define TSO3_TX_CONFIG_2_FORCEFIRE_CNT_EXT_MASK                 0x0003
    #define TSO3_TX_CONFIG_2_FORCEFIRE_CNT_EXT_SHIFT                0
    #define TSO3_TX_CONFIG_2_DISABLE_FORCEFIRE                      0x0004
    #define TSO3_TX_CONFIG_2_FIX_MIU_REG_FLUSH_VQ                   0x0008

    REG16_TSIO                          SGVQ_RX_CONFIG;             //0d
    #define TSO3_SGVQ_RX_CONFIG_MODE_MASK                           0x0003
    #define TSO3_SGVQ_RX_CONFIG_MODE_SHIFT                          0
    #define TSO3_SGVQ_RX_CONFIG_RD_THRESHOLD_MASK                   0x001C
    #define TSO3_SGVQ_RX_CONFIG_RD_THRESHOLD_SHIFT                  2
    #define TSO3_SGVQ_RX_CONFIG_ARBITOR_MODE_MASK                   0x0060
    #define TSO3_SGVQ_RX_CONFIG_ARBITOR_MODE_SHIFT                  5
    #define TSO3_SGVQ_RX_CONFIG_SRAM_SD_EN                          0x0080
    #define TSO3_SGVQ_RX_CONFIG_SVQ_FORCE_RESET                     0x0100
    #define TSO3_SGVQ_RX_CONFIG_SVQ_MIU_NS                          0x0200
    #define TSO3_SGVQ_RX_CONFIG_SVQ_MOBF_INDEX_MASK                 0x7C00
    #define TSO3_SGVQ_RX_CONFIG_SVQ_MOBF_INDEX_SHIFT                10
    #define TSO3_SGVQ_RX_CONFIG_SVQ_DYNAMIC_PRI                     0x8000

    REG16_TSIO                          SGVQ_STATUS;                //0e

    REG16_TSIO                          RESERVED_0F;                //0f

    REG16_TSIO                          PRE_HEADER_1_CONFIG[4];     //10~13

    REG16_TSIO                          DBG_SEL;                    //14
    #define TSO3_DBG_SEL_MASK                                       0x00FF
    #define TSO3_DBG_SEL_SHIFT                                      0

    REG16_TSIO                          LAST_PKT;                   //15
    #define TSO3_LAST_PKT_STR2MI_EN                                 0x0001
    #define TSO3_LAST_PKT_STR2MI_RST_WADR                           0x0002
    #define TSO3_LAST_PKT_STR2MI_PAUSE                              0x0004
    #define TSO3_LAST_PKT_BURST_LEN_MASK                            0x0018
    #define TSO3_LAST_PKT_BURST_LEN_SHIFT                           3
    #define TSO3_LAST_PKT_SRAM_SD_EN                                0x0020
    #define TSO3_LAST_PKT_FLUSH_DATA                                0x0040
    #define TSO3_LAST_PKT_STR2MI_WP_LD                              0x0080
    #define TSO3_LAST_PKT_CLR                                       0x0100
    #define TSO3_LAST_PKT_DMA_FLUSH_EN                              0x0200
    #define TSO3_LAST_PKT_MIU_HIGHPRI                               0x0400
    #define TSO3_LAST_PKT_WRITE_POINTER_TO_NEXT_ADDR_EN             0x0800
    #define TSO3_LAST_PKT_DMAW_PROTECT_EN                           0x1000
    #define TSO3_LAST_PKT_CLR_NO_HIT_INT                            0x2000
    #define TSO3_LAST_PKT_FLUSH_DATA_LAST_PKT_STATUS                0x4000

    REG32_TSIO                          LAST_PKT_STR2MI_HEAD;       //16~17

    REG32_TSIO                          LAST_PKT_STR2MI_TAIL;       //18~19

    REG32_TSIO                          LAST_PKT_DMAW_WADDR_ERR;    //1a~1b

    REG16_TSIO                          LAST_PKT_STR2MI_MOBF_INDEX0;//1c
    #define TSO3_LAST_PKT_STR2MI_MOBF_INDEX0_MASK                   0x001F
    #define TSO3_LAST_PKT_STR2MI_MOBF_INDEX0_SHIFT                  0

    REG32_TSIO                          LAST_PKT_STR2MI_WADR_R;     //1d~1e

    REG16_TSIO                          LAST_PKT_STATUS;            //1f
    #define TSO3_LAST_PKT_FIFO_STATUS_MASK                          0x000F
    #define TSO3_LAST_PKT_FIFO_STATUS_SHIFT                         0
    #define TSO3_ABT_STATUS_MASK                                    0x03F0
    #define TSO3_ABT_STATUS_SHIFT                                   4

    REG32_TSIO                          SG_DEBUG_PORT;              //20~21
    #define TSO3_SG_DEBUG_PORT_MASK                                 0x00FFFFFF
    #define TSO3_SG_DEBUG_PORT_SHIFT                                1

    REG16_TSIO                          REG_ABT;                    //22
    #define TSO3_MIU_RR_PRI_ABT                                     0x0001
    #define TSO3_DIS_MIU_RQ_ABT                                     0x0002
    #define TSO3_MERGE_EN_ABT                                       0x0004

    REG16_TSIO                          SW_RSTZ;                    //23
    #define TSO3_SW_RSTZ                                            0x0001
    #define TSO3_SW_RST_SG_DMA_READ                                 0x0002
    #define TSO3_SW_RST_SG_DMA_READ_MIU                             0x0004
    #define TSO3_SW_RST_SG_LAST_PKT                                 0x0008
    #define TSO3_SW_RST_SG_LAST_PKT_MIU                             0x0010
    #define TSO3_SW_RST_SG_PINGPONG_FIFO                            0x0020
    #define TSO3_SW_RST_SG_TSIF                                     0x0040
    #define TSO3_SW_RST_SG_VQ_TX                                    0x0080
    #define TSO3_SW_RST_SG_VQ_TX_MIU                                0x0100
    #define TSO3_SW_RST_SG_TSO_ROUNDROBIN                           0x0200
    #define TSO3_SW_RST_SG_TSO_SGDMA_CTRL                           0x0400
    #define TSO3_SW_RST_SG_TSO_VC_TABLE                             0x0800
    #define TSO3_SW_RST_SG_TSO_VCDMA_READ_MIU                       0x1000
    #define TSO3_SW_RST_SG_TSO_PACE_TRANSFER                        0x2000

    REG32_TSIO                          LAST_PKT_DMAW_LBND;         //24~25

    REG32_TSIO                          LAST_PKT_DMAW_UBND;         //26~27

    REG16_TSIO                          RESERVED_28_29[2];          //28~29

    REG16_TSIO                          SGDMA_IN_CONFIG0;           //2a

    REG16_TSIO                          SGDMA_IN_CONFIG1;           //2b

    REG16_TSIO                          SID_DISABLE[4];             //2c~2f

    REG16_TSIO                          SGCTRL_START;               //30
    #define TSO3_SGCTRL_START                                       0x0001

    REG16_TSIO                          ACPU_ACTIVE;                //31
    #define TSO3_ACPU_ACTIVE                                        0x0001

    REG16_TSIO                          ACPU_CMD;                   //32
    #define TSO3_ACPU_CMD_ACPU_VC_ID_MASK                           0x003F
    #define TSO3_ACPU_CMD_ACPU_VC_ID_SHIFT                          0
    #define TSO3_ACPU_CMD_ACPU_POSITION_MASK                        0x01C0
    #define TSO3_ACPU_CMD_ACPU_POSITION_SHIFT                       6
    #define TSO3_ACPU_CMD_START_A_NEW_PACKAGE                       0x0200
    #define TSO3_ACPU_CMD_ACPU_RW                                   0x0400
    #define TSO3_ACPU_CMD_ACPU_ACTIVE                               0x0800
    #define TSO3_ACPU_CMD_M_PRIORITY                                0x1000

    REG32_TSIO                          ACPU_WDATA;                 //33~34

    REG16_TSIO                          ACPU_FLAG;                  //35
    #define TSO3_ACPU_FLAG_MODE_INFO_MIU_SEL_MASK                   0x0003
    #define TSO3_ACPU_FLAG_MODE_INFO_MIU_SEL_SHIFT                  0
    #define TSO3_ACPU_FLAG_LAST_NODE_FLUSH                          0x0004
    #define TSO3_ACPU_FLAG_PKT_MODE_188                             0x0008
    #define TSO3_ACPU_FLAG_INTERRUPT_ENABLE                         0x0010
    #define TSO3_ACPU_FLAG_MOBF_MASK                                0x03E0
    #define TSO3_ACPU_FLAG_MOBF_SHIFT                               5
    #define TSO3_ACPU_FLAG_SERVICE_ID_MASK                          0xFC00
    #define TSO3_ACPU_FLAG_SERVICE_ID_SHIFT                         10

    REG32_TSIO                          ACPU_RDATA;                 //36~37

    REG16_TSIO                          SGDMA_IN;                   //38
    #define TSO3_SGDMA_IN_INT_CLR                                   0x0001
    #define TSO3_SGDMA_IN_PAUSE                                     0x0002
    #define TSO3_SGDMA_IN_DBG_SEL_MASK                              0x00FC
    #define TSO3_SGDMA_IN_DBG_SEL_SHIFT                             2
    #define TSO3_SGDMA_IN_INT_MASK                                  0x0100
    #define TSO3_SGDMA_IN_VCDMA_MIU_PRIORITY                        0x0200
    #define TSO3_SGDMA_IN_VC_INT_TRIGGER                            0x0400

    REG16_TSIO                          SGDMA_IN_DBG;               //39

    REG16_TSIO                          SGDMA_IN_VC_INT[4];         //3a~3d

    REG16_TSIO                          PACE_DBG;                   //3e
    #define TSO3_SGDMA_IN_VC_INT_VC_ID_MASK                         0x003F
    #define TSO3_SGDMA_IN_VC_INT_VC_ID_SHIFT                        0
    #define TSO3_SGDMA_IN_VC_INT_CLR                                0x0040
    #define TSO3_SGDMA_IN_VC_INT_MASK                               0x0080
    #define TSO3_PACE_DBG_VCID_MASK                                 0x3F00
    #define TSO3_PACE_DBG_VCID_SHIFT                                8
    #define TSO3_PACE_DBG_CLR                                       0x4000
    #define TSO3_PACE_DBG_EN                                        0x8000

    REG16_TSIO                          GLOBAL_TICK_COUNT_SET;      //3f
    #define TSO3_GLOBAL_TICK_COUNT_SET_MASK                         0x00FF
    #define TSO3_GLOBAL_TICK_COUNT_SET_SHIFT                        0

    REG16_TSIO                          TICK_COUNT_SET[64];         //40~7f
    #define TSO3_TICK_COUNT_SET_MASK                                0x03FF
    #define TSO3_TICK_COUNT_SET_SHIFT                               0
}REG_Ctrl_TSO3;

typedef struct _REG_Ctrl_TSP8 //TSP8
{
    REG16_TSIO                          RESERVED_00_62[99];         //00~62
    REG16_TSIO                          HW8_CONFIG3;      //63
    #define TSP_TSIF0_TSIO0_BLK_EN                                  0x0001
    #define TSP_TSIF0_TSIO1_BLK_EN                                  0x0002
    #define TSP_TSIF0_TSIO2_BLK_EN                                  0x0004
    #define TSP_TSIF0_TSIO3_BLK_EN                                  0x0008
    #define TSP_TSIF0_TSIO4_BLK_EN                                  0x0010
    #define TSP_TSIF0_TSIO5_BLK_EN                                  0x0020
    REG16_TSIO                          RESERVED_64_7F[28];         //64~7f
}REG_Ctrl_TSP8;

#endif // _TSIO_REG_H_
