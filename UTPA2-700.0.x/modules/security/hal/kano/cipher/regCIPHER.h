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
//  File name: regCIPHER.h
//  Description: CIPHER Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REG_CIPHER_H__
#define __REG_CIPHER_H__


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

#include "MsTypes.h"
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
#if defined (MCU_AEON)
#define REG_CIPHERCTRL_BASE         (0x1A0200 * 2) // bank 0x2A02
#else
#define REG_CIPHERCTRL_BASE         (0xA0B00 * 2) // bank 0x1A0B
#endif
#define REG_IRQCTRL_BASE            (0x01900 * 2) // bank 0x1019
#define REG_MIU_CROSSBAR            (0x61300 * 2) //bank 0x1613

#define REG32_Data   volatile MS_U32

#define DMA_CTRL_REG_SIZE       5UL  //0x02:ctrl  0x03:reserved  0x04:reserved  0x05:except_flag  0x06:cryptoDMA_debug
#define DMA_DIRECT_OUTPUT_SIZE  4UL
#define HASH_RPT_REG_SIZE       10UL
#define DMA_RPT_REG_SIZE        2UL
#define HMAC_KEY_SIZE           4UL
#define HWPA_KEY_SIZE           4UL
#define HWPA_IV_SIZE            4UL

#define RESERVE_SIZE1           3UL
#define RESERVE_SIZE2           21UL

#define HASH_CMD_MSG_SIZE           (64UL) //16*4
#define HASH_CMD_IV_SIZE            (32UL) //8*4
#define HASH_CMD_HMAC_HOSTKEY_SIZE  (16UL)

typedef struct _REG_AESDMACtrl
{
    REG32_Data   SpareCnt_ORG ;
    REG32_Data   Cmd_Queue_ORG;
    REG32_Data   Dma_Ctrl[DMA_CTRL_REG_SIZE]; //0x02:ctrl  0x03:debug data  0x04:reserved  0x05:except_flag  0x06:cryptoDMA_debug
		#define	 REG_DMA_CTRL 			       0x0
        #define	 REG_EXCEPT_FLAG 			   0x3
        #define  REG_EXCEPT_FLAG_SIZE          27
        #define  REG_CRYPTODMA_DEBUG           0x4
		#define  REG_EXCEPT_FLAG_CLEAR         0x80000
        #define  REG_PARSER_LG_PATH_EN         0x400
		#define  REG_DMA_SW_RESET              0x1
        #define  REG_DMA_STR_KEY_VALID         0x80000000
    REG32_Data   Dma_Out[DMA_DIRECT_OUTPUT_SIZE];
    REG32_Data   Hash_Reportp[HASH_RPT_REG_SIZE];
        #define  REG_CIPHER_RPT_THREAD_MSK     0xFFFF
        #define  REG_CIPHER_RPT_OK             0x80000000
        #define  REG_CIPHER_RPT_SHT              31
    REG32_Data   Dma_Reportp[DMA_RPT_REG_SIZE];
    REG32_Data   CryptoDMA2MI_NS;
    REG32_Data   Debug_Port;
    REG32_Data   HMAC_Key[HMAC_KEY_SIZE];
    REG32_Data   RESERVE1[RESERVE_SIZE1];
    REG32_Data   HWPA_Ctrl;
    REG32_Data   HWPA_IV2[HWPA_IV_SIZE];
    REG32_Data   HWPA_Key2[HWPA_KEY_SIZE];
    REG32_Data   Cipher_Status;
    REG32_Data   Parser_Status;
    REG32_Data   RESERVE2[RESERVE_SIZE2];
    REG32_Data   SpareCnt ;
    REG32_Data   Cmd_Queue;


}REG_AESDMACtrl;

typedef enum
{
    E_CIPHER_NDS_Region_fail,
    E_CIPHER_NDS_keyoff,
    E_CIPHER_sha_write_scr,
    E_CIPHER_sha_read_scr,
    E_CIPHER_sha_error_flag,
    E_CIPHER_except_read_scr,
    E_CIPHER_except_key,
    E_CIPHER_except_CAVid,
    E_CIPHER_except_des,
    E_CIPHER_except_NDS,
    E_CIPHER_except_chain,
    E_CIPHER_except_algo,
    E_CIPHER_except_key_HID,
    E_CIPHER_except_key_cbc_mac,
    E_CIPHER_except_file_dqmem,
    E_CIPHER_except_hash_dqmem,
    E_CIPHER_disable_masterkey0,
    E_CIPHER_disable_masterkey1,
    E_CIPHER_disable_cck,
    E_CIPHER_disable_reg_key_0,
    E_CIPHER_disable_reg_key_1,
    E_CIPHER_disable_reg_key_2,
    E_CIPHER_disable_reg_key_3,
    E_CIPHER_dma_forbid_qmem2dram,
    E_CIPHER_dma_forbid_cryptodma_keyslot_qmem,
    E_CIPHER_sha_forbid_qmem2dram,
    E_CIPHER_sha_forbid_cryptodma_keyslot_qmem,
	E_CIPHER_tdes_key_error,
	E_CIPHER_write_address_error,
	E_CIPHER_except_str,
	E_CIPHER_except_sha_str

}REG_Cipher_ExceptFlag;

typedef enum
{
    E_CIPHER_DEBUG_HASH_BUSY  = 0x40,
    E_CIPHER_DEBUG_DMA_BUSY   = 0x80,

}REG_CIPHER_Debug;


typedef struct _CMD_DmaFormat0
{
    MS_U32          CL:6;
        #define     CMD_LEN_MASK            0x3FUL

    MS_U32          DIV:1;

    MS_U32          DK:1;

    MS_U32          OVT:1;

    MS_U32          SB:3;
        #define     CMD_DMA_SB_SEL_CLR          0x0UL
        #define     CMD_DMA_SB_SEL_IV1          0x1UL
        #define     CMD_DMA_SB_SEL_IV2          0x2UL

    MS_U32          Residue:3;
    	#define     CMD_DMA_RESIDUE_CLR         0x0UL
    	#define     CMD_DMA_RESIDUE_CTS         0x1UL
    	#define     CMD_DMA_RESIDUE_SCTE52      0x2UL

  //jamietest  MS_U32          Decrypt:1;

    MS_U32          SubAlgo:4;
        #define     CMD_DMA_SUBALGO_ECB          0x0UL
        #define     CMD_DMA_SUBALGO_CBC          0x1UL
        #define     CMD_DMA_SUBALGO_CTR          0x2UL
        #define     CMD_DMA_SUBALGO_CBC_MAC      0x3UL  // [NOTE] This sub algorithm is AES only
		#define     CMD_DMA_SUBALGO_CTR64        0x4UL
        #define     CMD_DMA_SUBALGO_CMAC_KEY     0x5UL
        #define     CMD_DMA_SUBALGO_CMAC_ALGO    0x6UL

    MS_U32          Algo:4;
        #define     CMD_DMA_ALGO_AES                0x1UL
        #define     CMD_DMA_ALGO_DES                0x2UL
        #define     CMD_DMA_ALGO_TDES               0x3UL
        #define     CMD_DMA_ALGO_M6                 0x4UL
        #define     CMD_DMA_ALGO_M6_CCBC_DIS        0x5UL
        #define     CMD_DMA_ALGO_M6_KE56            0x6UL
        #define     CMD_DMA_ALGO_M6_KE56_CCBC_DIS   0x7UL
        #define     CMD_DMA_RC4                     0x8UL

    MS_U32          KeySel:4; //jamietest KeySel:3

        // bit [3]
        #define     CMD_DMA_KSEL_CLR_SK             0x8UL

        // bit [2:0]
        #define     CMD_DMA_KSEL_SK0                0x0UL //session key 0
        #define     CMD_DMA_KSEL_SK1                0x1UL
        #define     CMD_DMA_KSEL_SK2                0x2UL
        #define     CMD_DMA_KSEL_SK3                0x3UL
        #define     CMD_DMA_KSEL_MK0                0x4UL //SCK3
        #define     CMD_DMA_KSEL_MK1                0x5UL //SCK4
        #define     CMD_DMA_KSEL_CCCK               0x6UL //SCK6
		#define     CMD_DMA_KSEL_TRNG               0x7UL //PM Key

	MS_U32          CMD:3;
        #define     CMD_DMA_ENCRYPT                 0x2UL
        #define     CMD_DMA_DECRYPT                 0x3UL
        #define     CMD_DMA_MASK                    0x2FUL

    MS_U32          HID:2;                      	//[NOTE] Set by HW only
        #define     CMD_ACPU                    	0x0UL
        #define     CMD_R2                      	0x1UL
        #define     CMD_BGC51                   	0x2UL

}CMD_DmaFormat0;


typedef struct _CMD_DmaFormat1
{
    MS_U32          TID:16;
    MS_U32          CaVid:5;
    MS_U32          IntM:1;
        #define     CMD_INT_MODE_NONE       0x0UL
        #define     CMD_INT_MODE_EN         0x1UL
        //#define     CMD_INT_MODE_WAIT_CLR   0x2   // Block next operation until interrupt is cleared
    MS_U32          NL:1;
        #define     CMD_NEW_LINE            0x1UL

    MS_U32          RR:1;  	// Report Mode
        #define     CMD_DMA_RPT_MODE_REG        0x0UL
        #define     CMD_DMA_RPT_MODE_DRAM       0x1UL

    MS_U32          Dest:1;     // File output Dest
        #define     CMD_DMA_OUTPUT_DRAM         0x0UL
        #define     CMD_DMA_OUTPUT_REG          0x1UL   //[NOTE] if HID = R2 , the output will be written into DQRAM

    MS_U32          SD:2;       // Source Data
	    #define     CMD_DMA_DATA_REG            0x0UL
        #define     CMD_DMA_DATA_DRAM           0x1UL
        #define     CMD_DMA_DATA_R2             0x2UL //[NOTE] bit[15] = 0, IQMEM. bit[15] = 1, DQMEM
        #define     CMD_DMA_DATA_HW_INPUT       0x3UL //jamietest
    MS_U32          DIBS:1;     // Data Input Byte Swap
    MS_U32          DOBS:1;     // Output Data Byte swap
    MS_U32          DestKL:1;     // [NOTE] if DestKL = 0 ,Write output data to DRAM, Regfile or QMEM by setting, DestKL = 1 output to SRAM of KL
        #define     CMD_DMA_OUTPUT_DEST         0x0UL
        #define     CMD_DMA_OUTPUT_SRAM_KL      0x1UL
    MS_U32          COS:1;      // Output Data Swap
    MS_U32          CIS:1;      // Input Data Swap

}CMD_DmaFormat1;

typedef struct _CMD_DmaFormat2
{
    MS_U32          OBF_IDX_WRITE:5;
    MS_U32          CTROT:4;
    MS_U32          DMP:1;
    MS_U32          TK:1;
    MS_U32          TsInSb:2;
    MS_U32          BPS:1;
    MS_U32          TsSbMk:1;
    MS_U32          TsSbPn:2;
        #define     CMD_PARSER_SCB10            0x2UL
        #define     CMD_PARSER_SCB11            0x3UL
    MS_U32          InSb:1;
    MS_U32          RmSb:1;
    MS_U32          CR:1;
    MS_U32          IT:1;
    MS_U32          AT:1;
    MS_U32          P192:1;
    MS_U32          TS:1;
    MS_U32          HDCP:1;
    MS_U32          HP:1;
    MS_U32          OBF_IDX_READ:5;

    MS_U32          NS:1;
        #define     CMD_DMA_SECURE_IP       0x0UL
        #define     CMD_DMA_NON_SECURE_IP   0x1UL

}CMD_DmaFormat2;

typedef struct _CMD_DmaFormat3
{
    MS_U32          Rsv1:4;
    MS_U32          CRS:1;
    MS_U32          MP:1;
    MS_U32          DIV2:1;
    MS_U32          DK2:1;
    MS_U32          Rsv2:15;
    MS_U32          KeySel2:4;
    MS_U32          Decrypt2:1;
    MS_U32          IK:1;
    MS_U32          AP:1;
    MS_U32          Rsv3:2;

}CMD_DmaFormat3;


typedef struct _CMD_HashFormat0
{
    MS_U32          CL:6;
    MS_U32          IntM:1;

    MS_U32          IWC_Sel:1;
        #define     CMD_HASH_IWC_RPT        0x0UL
        #define     CMD_HASH_IWC_CMDQ       0x1UL

    MS_U32          OVT:1;
    MS_U32          HOS:1; //reverse hash output

    MS_U32          Inv16:1; //invere byte order per 16 byte
    MS_U32          KPAD:1;
        #define     CMD_HASH_IKP    0x0UL
        #define     CMD_HASH_OKP    0x1UL
    MS_U32          HMAC:1;
        #define     CMD_HASH_NONE   0x0UL
        #define     CMD_HASH_HMAC   0x1UL


    MS_U32          HMAC_KeySel:4;

        // bit [3:0]
        #define     CMD_HMAC_KSEL_HK                 0x0UL
        #define     CMD_HMAC_KSEL_STRN               0x1UL
        #define     CMD_HMAC_KSEL_MK0                0x2UL
        #define     CMD_HMAC_KSEL_MK1                0x3UL
        #define     CMD_HMAC_KSEL_CCCK               0x4UL
        #define     CMD_HMAC_KSEL_SK0                0x5UL
        #define     CMD_HMAC_KSEL_SK1                0x6UL
        #define     CMD_HMAC_KSEL_SK2                0x7UL
        #define     CMD_HMAC_KSEL_SK3                0x8UL

    MS_U32          DK:1;                                 //HMAC command key, clear seesion key
        #define     CMD_HMAC_KSEL_CLR_SK             0x1UL  //HMAC KSEL = 5-8
        #define     CMD_HMAC_KSEL_DIR_HK             0x1UL  //HMAC KSEL = 0

    MS_U32          Dest:2;
        #define     CMD_HASH_OUTPUT_RPT     0x0UL
        #define     CMD_HASH_OUTPUT_R2      0x2UL  //IQMEM or DQMEM
        #define     CMD_HASH_OUTPUT_DRAM    0x3UL

    MS_U32          Src:2;
        #define     CMD_HASH_SRC_DRAM       0x0UL
        #define     CMD_HASH_SRC_PRV_RPT    0x1UL  //use previous hash operation result in hasg_rpt_reg2~9
        #define     CMD_HASH_SRC_R2         0x2UL  //[NOTE] bit[15] = 0, IQMEM. bit[15] = 1, DQMEM
        #define     CMD_HASH_SRC_REG        0x3UL
    MS_U32          AutoPad:1;
    MS_U32          InitHashSel:2;
        #define     CMD_HASH_IV_FIPS        0x0UL
        #define     CMD_HASH_IV_CMD         0x1UL
        #define     CMD_HASH_IV_PRV_RPT     0x2UL

    MS_U32          HashSel:2;  //jamietest
        #define     CMD_HASH_SHA1           0x0UL
        #define     CMD_HASH_SHA256         0x1UL
        #define     CMD_HASH_MD5            0x2UL

    MS_U32          CMD:3;
        #define     CMD_HASH_START          0x4UL
		#define     CMD_HASH_START_RR       0x6UL

    MS_U32          HID:2;
        #define     CMD_ACPU                    0x0UL
        #define     CMD_R2                      0x1UL
        #define     CMD_BGC51                   0x2UL

}Hash_DmaFormat0;

typedef struct _CMD_HashFormat1
{
    MS_U32          TID:16;
    MS_U32          CaVid:5;
    MS_U32          OBF_IDX_WRITE:5;
    MS_U32          OBF_IDX_READ:5;
    MS_U32          NS:1;
        #define     CMD_HASH_SECURE_IP       0x0UL
        #define     CMD_HASH_NON_SECURE_IP   0x1UL

}Hash_DmaFormat1;

typedef struct _CMD_HashFormat2
{
    MS_U32          Rsv:31;
    MS_U32          RR:1;

}CMD_HashFormat2;


//=====================================================================
//            RSA Register Definition
//=====================================================================

#define REG_RSA_BASE         (0x31800UL)

#define REG_RSA_COMMON          ( REG_RSA_BASE + 0x0UL )
#define REG_RSA_ONEWAY_SYSREG   ( REG_RSA_BASE + 0x1UL )
#define REG_RSA_CTRL            ( REG_RSA_BASE + 0x2UL )
#define REG_RSA_IND32_ADDR      ( REG_RSA_BASE + 0x3UL )
#define REG_RSA_IND32_WDATA     ( REG_RSA_BASE + 0x4UL )

#define REG_RSA_SEC_RANGE_START(_a_)   ( REG_RSA_BASE + ((_a_)* 2) + 0x5 ) // a = 0 ~ 5
#define REG_RSA_SEC_RANGE_END(_a_)     ( REG_RSA_BASE + ((_a_)* 2) + 0x6 )

#define REG_RSA_KEY_SEL         ( REG_RSA_BASE + 0x32UL )
#define REG_RSA_WDATA           ( REG_RSA_BASE + 0x39UL )
#define REG_RSA_LV_PROTECT_EN   ( REG_RSA_BASE + 0x3aUL )
#define REG_RSA_MAILBOX(_a_)    ( REG_RSA_BASE + 0x40UL + (_a_) )

//=====================================================================
//            MISC Register Definition
//=====================================================================
#define REG_RNG_BASE        (0x143800UL)//(0xA1C00 * 2)
#define REG_OTP_PUB0_BASE   (0xC5800UL)//(0x62C00 * 2)
#define REG_OTP_PUB1_BASE   (0xC5A00UL)//(0x62D00 * 2)
#define REG_OTP_PUB2_BASE   (0xC5C00UL)//(0x62E00 * 2)

#define REG_RNG_TRNG_ACPU           (REG_RNG_BASE + 0x13 * 4)

#define REG_RNG_TRNG_ACK_ACPU 0x80000000UL
#define REG_RNG_TRNG_VALID_ACPU_MASK 0x00010000UL
#define REG_RNG_TRNG_OUT_ACPU_MASK 0x0000FFFFUL

#define REG_RNG_TRNG_SCPU           (REG_RNG_BASE + 0x12 * 4)

#define REG_RNG_TRNG_ACK_SCPU 0x80000000UL
#define REG_RNG_TRNG_VALID_SCPU_MASK 0x00010000UL
#define REG_RNG_TRNG_OUT_SCPU_MASK 0x0000FFFFUL

//=====================================================================
//            IRQ Register Definition
//=====================================================================
#define REG_HST0_FIQ_STATUS_63_48    (REG_IRQCTRL_BASE + (0xfUL * 4))
#define REG_HST0_FIQ_MASK_63_48      (REG_IRQCTRL_BASE + (0x7UL * 4))

#define REG_HTS0_FIQ_MASK_CRYPTODMA  (0xFEFFUL)
#define REG_HTS0_FIQ_CRYPTODMA       (0x0100UL)   //FIQ 56 is CryptoDMA

#endif // #ifndef __REG_CIPHER_H__
