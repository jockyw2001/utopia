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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regAESDMA.h
//  Description: AESDMA Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AESDMA_REG_MCU_H_
#define _AESDMA_REG_MCU_H_


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


//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_AESDMACTRL_BASE         0x47940UL //(0x23C00/2 + 0x50) * 4
#define REG_SHARNGCTRL_BASE         0x47800UL //(0x23C00/2 + 0x0) * 4
#define AESDMS_SECURED_BANK_EN 0

#define REG_DMASECURE_CTRL_BASE     0x27A00UL //(0x13D00/2 + 0x00) * 4
#define REG_DMASECURE_BASE          0x27B40UL //(0x13D00/2 + 0x50) * 4
#define REG_AESDMACLK_BASE          0x1460UL  //(0x0A00/2 + 0x18) * 4
//#define REG_AESDMAMBX_BASE          0x17F8  //(0x0B00/2 + 0x7F) * 4
#define REG_AESDMAMBX_BASE          0x7B84UL  //(0x3D00/2 + 0x61) * 4
#define REG_PARSERCTRL_BASE         0x45800UL //(0x22C00/2 + 0x00) * 4
#define REG_CIPHERCTRL_BASE         0x45840UL //(0x22C00/2 + 0x10) * 4
#define REG_AESDMAEXT_BASE          0x45980UL //(0x22C00/2 + 0x60) * 4
#define REG_TZPC_NONPM_BASE         0x47200UL //(0x23900/2 + 0x00) * 4
#define REG_AES_DSCMB6_BASE         0xE7A00UL //(0x73D00/2 + 0x00) * 4
#define REG_AES_DSCMB7_BASE         0xE7B00UL //(0x73D00/2 + 0x40) * 4


//Used for Random Number Gen, Manhattan only
#define REG_ANA_MISC_BASE           0x21840UL //(0x10C00/2 + 0x10) * 4

#define RSA_E_BASE_ADDR             (0x00UL)
#define RSA_N_BASE_ADDR             (0x40UL)
#define RSA_A_BASE_ADDR             (0x80UL)
#define RSA_Z_BASE_ADDR             (0xC0UL)
#define REG_HDCP22_BASE             0xE74C0UL  //(0x73A00/2 + 0x30) * 4
#define REG_HDCP22_SEK              0xE2538UL  //(0x71200/2 + 0x4E) * 4
#define REG_HDCP14_BASE             0xE2438UL  //(0x71200 + 0x1C) * 2

typedef struct _REG32
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32;

typedef struct _REG_CipherKey
{
    REG32                           Key_L;
    REG32                           Key_H;
} REG_CipherKey;

typedef struct _REG_InitVector
{
    REG32                           IV_L;
    REG32                           IV_H;
} REG_InitVector;

typedef struct _REG_AESDMACtrl
{
    REG32               Dma_Ctrl;                                   //0x50
    #define AESDMA_CTRL_FILEIN_START            0x00000100UL
    #define AESDMA_CTRL_FILEOUT_START           0x00000001UL
    #define AESDMA_CTRL_SW_RST                  0x00000080UL
    #define AESDMA_CTRL_BURST_LENGTH            0x00001000UL

    #define AESDMA_ENG_PS_RELEASE               0x00010000UL
    #define AESDMA_ENG_PS_IN_EN                 0x00100000UL
    #define AESDMA_ENG_PS_OUT_EN                0x00200000UL
    #define AESDMA_ENG_AES_EN                   0x01000000UL
    #define AESDMA_ENG_DES_EN                   0x00040000UL
    #define AESDMA_ENG_TDES_EN                  0x00080000UL
    #define AESDMA_ENG_DESCRYPT                 0x02000000UL          // 0:encrypt, 1:decrypt
    #define AESDMA_ENG_CTR_MODE                 0x10000000UL
    #define AESDMA_ENG_CBC_MODE                 0x20000000UL
    #define AESDMA_ENG_CTS_CBC_MODE             0x40000000UL
    #define AESDMA_ENG_CTS_ECB_MODE             0x00000002UL
    #define AESDMA_ECO_FIX_LAST_BYTE            0x80000000UL          // when (output address + length)/8=1 the last byte maybe no output

    REG32               Dma_Filein_Addr;                            //0x52
    REG32               Dma_Filein_Num;                             //0x54
    REG32               Dma_Fileout_SAddr;                          //0x56
    REG32               Dma_Fileout_EAddr;                          //0x58
    REG32               Dma_PS_Pattern;                             //0x5a
    REG32               Dma_PS_Pattern_Mask;                        //0x5c
    #define AESDMA_CTR_IV_LSB64                 0x01000000UL          //0X5d[8]: reg_ctr_iv_lsb64
    REG32               Dma_Ctrl2;                                  //0x5e
    #define AESDMA_DMA_USE_TDES_EN              0x00000100UL          // DES/TDES path switch
    #define AESDMA_CTRL_BANK_R_H                0x00002000UL
    #define AESDMA_CTRL_BANK_W_H                0x00004000UL
    #define AESDMA_CTRL_BANK_R                  0x00000010UL
    #define AESDMA_CTRL_BANK_W                  0x00000020UL
    #define AESDMA_INT_EN                       0x00000080UL
    #define AESDMA_USE_SECRET_KEY               0x00001000UL        //0x5E[12], Enable Efuse Key1
    #define AESDMA_USE_SECRET_KEY2              0x00001200UL        //0x5E[9]+0x5E[12], Enable Efuse Key2
    #define AESDMA_WADR_ERR_CLR                 0x00000001UL

    REG_CipherKey       Dma_CipherKey_L;                            //0x60
    REG_CipherKey       Dma_CipherKey_H;                            //0x64
    REG_InitVector      Dma_InitVector_L;                           //0x68
    REG_InitVector      Dma_InitVector_H;                           //0x6C
    REG32               Dma_Matched_Btyecnt;                        //0x70
    REG32               Dma_Matched_Pat;                            //0x72
    REG32               Dma_Err_Wadr;                               //0x74
    REG32               Dma_Eng3_Ctrl;                              //0x76
    #define AESDMA_ENG_SPEEDUP                  0x03BF0000UL
    REG32               _xbf808fd8[3];                              //0x78~0x7c
    REG32               Dma_PVR_Status;                             //0x7e
    #define AESDMA_IS_FINISHED                  0x00010000UL
/*
    #define AESDMA_PS_DONE                      0x00000001
    #define AESDMA_PS_STOP                      0x00000002
    #define AESDMA_DMA_DONE                     0x00010000
    #define AESDMA_DMA_PAUSE                    0x00020000
    #define AESDMA_STATES_GROUP                 (AESDMA_PS_DONE     | \
                                                 AESDMA_PS_STOP     | \
                                                 AESDMA_DMA_DONE    | \
                                                 AESDMA_DMA_PAUSE  )
*/
}REG_AESDMACtrl;

#define DmaCtrlSet (AESDMA_ENG_AES_EN       | \
                    AESDMA_ENG_DES_EN       | \
                    AESDMA_ENG_TDES_EN      | \
                    AESDMA_ENG_DESCRYPT     | \
                    AESDMA_ENG_CTR_MODE     | \
                    AESDMA_ENG_CBC_MODE     | \
                    AESDMA_ENG_CTS_CBC_MODE | \
                    AESDMA_ENG_CTS_ECB_MODE)

typedef struct _REG_SHARNGCtrl
{
    REG32               Rng_Ctrl;                                   //0x00
    #define SHARNG_CTRL_RNG_SW_RST              0x00000080UL
	#define MOBF_IN_MIU_READ_EN                 0x00010000UL
	#define MOBF_IN_MIU_WRITE_EN                0x00020000UL
	#define AES_FILE_IN_MIU_NS                  0x00100000UL
	#define AES_FILE_OUT_MIU_NS                 0x00200000UL
	#define MOBF_ONEWAY_EN                      0x01000000UL

    #define SECRET_KEY_IN_NORMAL_BANK           0x08000000UL


    REG32               Rng_Out;                                    //0x02
    REG32               MOBF_KeyR;                                  //0x04
    REG32               MOBF_KeyW;                                  //0x06
    REG32               Sha_Ctrl;                                   //0x08
	#define SHARNG_CTRL_SHA_FIRE_ONCE           0x00000001UL
	#define SHARNG_CTRL_SHA_CLR                 0x00000040UL
	#define SHARNG_CTRL_SHA_RST                 0x00000080UL
	#define SHARNG_CTRL_SHA_INT                 0x00000100UL
	#define SHARNG_CTRL_SHA_SEL_SHA256          0x00000200UL
	#define SHARNG_CTRL_SHA_MSG_INVERSE         0x00040000UL
	#define SHARNG_CTRL_SHA_MSG_BLOCK_NUM       0x00010000UL /* Always be 1 */
    #define SHARNG_CTRL_SPEED_MODE_N		    0x00000010UL
    #define SHARNG_CTRL_REMOVE_SCATTER_GATHER	0x00000800UL

    REG32               Sha_Start;                                  //0x0a
    REG32               Sha_Length;                                 //0x0c
    REG32               Sha_Status;                                 //0x0e
    #define SHARNG_CTRL_SHA_BUSY                0x00020000UL
	#define SHARNG_CTRL_SHA_READY               0x00010000UL

    REG32               Sha_Out[8];                                 //0x10~0x1E
    REG32               Rsa_Ind32_Start;                            //0x20
    #define RSA_INDIRECT_START                  0x00000001UL
    #define RSA_IND32_CTRL_DIRECTION_WRITE      0x00020000UL
    #define RSA_IND32_CTRL_ADDR_AUTO_INC        0x00040000UL
    #define RSA_IND32_CTRL_ACCESS_AUTO_START    0x00080000UL
    REG32               Rsa_Ind32_Addr;                             //0x22
    #define RSA_ADDRESS_MASK                    0x0000FFFFUL
    #define RSA_WDATA_MASK_L                    0xFFFF0000UL
    REG32               Rsa_Ind32_WData;                            //0x24
    #define RSA_WDATA_MASK_H                    0x0000FFFFUL
    #define RSA_RDATA_MASK_L                    0xFFFF0000UL
    REG32               Rsa_Ind32_RData;                            //0x26
    #define RSA_RDATA_MASK_H                    0x0000FFFFUL
    #define RSA_EXP_START                       0x00010000UL
    #define RSA_INT_CLR                         0x00020000UL
    REG32               Rsa_Ctrl;                                   //0x28
    #define RSA_CTRL_RSA_RST                    0x00000001UL
    #define RSA_CTRL_SEL_HW_KEY                 0x00000002UL
    #define RSA_CTRL_SEL_PUBLIC_KEY             0x00000004UL
    #define RSA_CTRL_KEY_LENGTH_MASK            0x00003F00UL
    #define RSA_STATUS_RSA_BUSY                 0x00010000UL
    #define RSA_STATUS_MASK                     0x00FF0000UL
}REG_SHARNGCtrl;

typedef struct _REG_DMASECURECtrl
{
    REG32               Secure_file_st;                             //0x50
    //#define AESDMA_SECURE_FILEIN_START          0x00000001
    REG32               reserved52;                                 //0x52
    REG32               reserved54;                                 //0x54
    REG32               reserved56;                                 //0x56
    REG32               reserved58;                                 //0x58
    REG32               reserved5a;                                 //0x5a
    REG32               reserved5c;                                 //0x5c
    REG32               reserved5e;                                 //0x5e
    REG_CipherKey       Secure_CipherKey_L;                         //0x60
    REG_CipherKey       Secure_CipherKey_H;                         //0x64
    REG_InitVector      Secure_InitVector_L;                        //0x68
    REG_InitVector      Secure_InitVector_H;                        //0x6C
    REG32               reserved70;                                 //0x70
    REG32               reserved72;                                 //0x72
    REG32               reserved74;                                 //0x74
    REG32               reserved76;                                 //0x76
    REG32               Secure_dma3_ctrl;                           //0x78
    #define AESDMA_SECURE_PROTECT_S             0x00010000UL
    #define AESDMA_SECURE_PROTECT_SHA_S         0x00020000UL
    #define AESDMA_SECURE_PROTECT_RSA_S         0x00040000UL
    #define AESDMA_USE_SECRET_KEY0              0x00200000UL
    #define AESDMA_USE_SECRET_KEY1              0x00400000UL
    REG32               reserved7a;                                 //0x7a
    REG32               reserved7c;                                 //0x7c
    REG32               Secure_dma3_status;                         //0x7e
    //#define AESDMA_DONE_S                       0x00010000
}REG_DMASECURECtrl;

typedef struct _REG_CKG_AESDMA
{
    REG32               Reg_Gate_Clk_AESDMA;                        //0x18
    #define AESDMA_CLK_OFF_EN                   0x00010000UL        //0x19[0]
    #define AESDMA_CLK_INV                      0x00020000UL        //0x19[1]
    #define AESDMA_CLK_172M                     0x00000000UL        //0x19[3:2]
    #define AESDMA_CLK_144M                     0x00040000UL        //0x19[3:2]
    #define AESDMA_CLK_108M                     0x00080000UL        //0x19[3:2]
    #define AESDMA_CLK_XTAL                     0x000C0000UL        //0x19[3:2]

}REG_AESDMAClk;

typedef struct _REG_MBX_AESDMA
{
    REG32               Reg_Et_Rpd;                                 //0x61
    #define AESDMA_SEM_USED                     0x00000100UL
    #define AESDMA_CLK_USED                     0x00000200UL
    #define TSP_CLK_USED                        0x00000400UL
}REG_AESDMAMbx;


typedef struct _REG_PARSERCtrl
{
    REG32               Parser_Ctrl;                                //0x00
    #define HDCP20_MODE                         0x00000001UL
    #define PKT192_MODE                         0x00000002UL
    #define AUTO_MODE                           0x00000004UL
    #define TRIGGER_MODE                        0x00000008UL
    #define INIT_TRUST                          0x00000010UL
    #define CLEAR_MODE                          0x00000020UL
    #define REMOVE_SCRMB                        0x00000040UL
    #define INSERT_SCRMB                        0x00000080UL
    #define SCRMB_PATTERN10                     0x00000400UL
    #define SCRMB_PATTERN11                     0x00000600UL
    #define SCRMB_INITVALUE                     0x00000600UL
    #define HW_PARSER_MODE                      0x00000100UL
    #define TS_SCRMB_MASK                       0x00000800UL
    #define TS_MODE                             0x00008000UL
    #define PARSER_PID0_MASK                    0x1FFF0000UL
    REG32               Parser_Pid1;                                //0x02
    #define PARSER_PID1_MASK                    0x00001FFFUL
    #define BYPASS_PID                          0x00080000UL
    #define SCRMB_PATTERN10_ADD                 0x00200000UL
    #define SCRMB_PATTERN11_ADD                 0x00300000UL
    #define SCRMB_INITVALUE_ADD                 0x00300000UL
    #define SCRMB_ENABLE_TWO_KEY                0x00400000UL
    #define ENABLE_LG_PATCH                     0x01000000UL
    REG32               Parser_Status;                              //0x04

}REG_PARSERCtrl;

typedef struct _REG_CIPHERCtrl
{
    REG32               Cipher_Ctrl;                                //0x00
    #define CC_CTRL_BY_ACPU                     0x00000080UL
    REG32               Cipher_len;                                 //0x02
    REG32               Parser_Status;                              //0x04

}REG_CIPHERCtrl;

#define ParserCtrlSet (HDCP20_MODE       | \
                       PKT192_MODE       | \
                       AUTO_MODE         | \
                       TRIGGER_MODE      | \
                       INIT_TRUST        | \
                       CLEAR_MODE        | \
                       HW_PARSER_MODE    | \
                       TS_MODE             )

typedef struct _REG_AESDMACtrlEx
{
    REG_CipherKey       Dma_CipherKey_L;                            //0x60
    REG_CipherKey       Dma_CipherKey_H;                            //0x64
    REG_InitVector      Dma_InitVector_L;                           //0x68
    REG_InitVector      Dma_InitVector_H;                           //0x6C
}REG_AESDMACtrlEx;

typedef struct _REG_HDCP22Data
{
    REG32               Hdcp22_ContentKey[4];
    REG32               Hdcp22_Riv[2];
}REG_HDCP22Data;

typedef struct _REG_HDCP22SEKCtrl
{
    REG32               SEK;                                        //0x9C
    #define HDCP_SEK_BIT0                              0x00000001
}REG_HDCP22SEKCtrl;

typedef struct _REG_GENERAL_STRUCT
{
    REG32               Reg0001;                                    //0x00
    REG32               Reg0203;                                    //0x02
    REG32               Reg0405;                                    //0x04
    REG32               Reg0607;                                    //0x06
    REG32               Reg0809;                                    //0x08
}REG_GENERAL_STRUCT;

typedef struct _REG_HDCP14SEKCtrl
{
    REG32               REG_0001;                                        //0x0E
    #define HDCP14_SEK_BIT0                            0x00000001
}REG_HDCP14SEKCtrl;

typedef REG_GENERAL_STRUCT REG_TZPCCtrl;                            //BANK: 0x1239
#define REG_TZPC2NONPM_RNG_SOURCE_EN        0x000F0000UL              //0x03[3:0]

typedef REG_GENERAL_STRUCT REG_SECUREBASECtrl;                      //BANK: 0x113D
#define REG_RNG_EN                          0x00000080UL              //0x00[7]

typedef REG_GENERAL_STRUCT REG_ANAMISCBASECtrl;                     //BANK: 0x110C_10
#define REG_MIPSPLL_OCCCLK2_EN              0x02000000UL              //0x11[9]

typedef REG_GENERAL_STRUCT REG_AES_DSCMB6Ctrl;                      //BANK: 0x173D_0
#define AESDMA_DSCRMB3_HW_PASER_EN          0x04000000UL              //0x01[10] = 1:DESCRMB3/0:DESCRMB5, [11:10] reg_who_use_hwparser
typedef REG_GENERAL_STRUCT REG_AES_DSCMB7Ctrl;                      //BANK: 0x173D_40
#define AESDMA_ISR_CLR                      0x00010000UL              //0x01[0]

#endif // #ifndef _AESDMA_REG_MCU_H_
