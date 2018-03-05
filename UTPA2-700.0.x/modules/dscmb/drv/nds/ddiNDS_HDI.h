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
/****************************************************************************/
/*                                                                          */
/*       CAHDIVER.H                                                         */
/*       ----------                                                         */
/*                                                                          */
/*  Description: Include file between CA HDI Interface and Jupiter Verifier */
/*                                                                          */
/*  $Revision:   2.0  $$Author :  $$Date:   07 Jan 1999 19:50:10  $         */
/****************************************************************************/

#ifndef __N3_282_H__
#define __N3_282_H__




#ifndef H_NDSTYPES
typedef unsigned char               NDS_BYTE;
typedef unsigned short              NDS_USHORT;
typedef unsigned long               NDS_ULONG;
typedef NDS_USHORT NDS_STATUS;
#endif

typedef unsigned char CAHDI_BOOLEAN;

#define CAHDI_TRUE  ( (unsigned char) 1 )
#define CAHDI_FALSE ( (unsigned char) 0 )



/*******************/
/* Value of status */
/*******************/
#define CA_OK                                   0
#define CA_REQUEST_NOT_SUPPORTED_BY_DRIVER      1
#define CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW  2
#define CA_GLOBAL_HARDWARE_FAILURE              3
#define CA_ECM_PID_FILTER_ALREADY_DEFINED       4
#define CA_DRIVER_INVALID_DATA_STREAM           5
#define CA_DRIVER_INVALID_LOGICAL_STREAM_ID     6
// #define CA_INSUFICIENT_BUFFER                   7
#define CA_INFO_NOT_AVAILABLE                           7
#define CA_INSUFFICIENT_BUFFER                          8
#define CA_SCHEME_NOT_SUPPORTED                         9
#define CA_FUNCTIONALITY_NOT_AVAILABLE_UNTIL_RESTART    10
#define CA_FUNCTIONALITY_PERMANENTLY_NOT_AVAILABLE      11
#define CA_FEATURE_NOT_SUPPORTED                        12

#define MAX_PIDS                                8

/****************************************************************************/
/*                                                                          */
/*                    XCORECA_CbModuleType                                  */
/*                                                                          */
/****************************************************************************/

typedef struct {
   NDS_BYTE         ca_module_type;
   NDS_BYTE         ca_module_version;
   NDS_USHORT       uart_module_version;
   NDS_BYTE         *ecm_buffers_ptr;
   NDS_BYTE         *emm_buffers_ptr;
   NDS_BYTE         num_emm_buffers;
   NDS_BYTE         num_ecm_buffer_pairs;
   CAHDI_BOOLEAN    auto_odd_to_even_ecm_filter;
   NDS_BYTE         num_descramblers;
   NDS_ULONG        hw_module_id;
   NDS_BYTE         reserved[9];
}  CA_MODULE_TYPE;

extern NDS_STATUS XCORECA_CbModuleType (int num_of_modules,
                                        CA_MODULE_TYPE *ca_module_type);

/* Define values of ca_module_version (or icam version) */
#define ICAM_1   0x1
#define ICAM_2   0x2
#define LSI      0x11

extern NDS_STATUS XHDICA_GetLogicalStreamId (int request_type,
                                             NDS_ULONG x_connection,
                                             NDS_BYTE *hw_module_id);
#define HDICA_GET_EMM_STREAM         1
#define HDICA_GET_ECM_STREAM         2
#define HDICA_GET_DESCRAMBLER_STREAM 3

/****************************************************************************/
/*                                                                          */
/*                      ECM INTERFACE                                       */
/*                                                                          */
/****************************************************************************/
/*  HDICA_ECM_RECEPTION_MASK */
#define HDICA_ECM_RECEPTION_DISABLE 0x00
#define HDICA_ECM_RECEPTION_ENABLE  0x80

#define HDICA_ECM_DISABLE_RECEPTION 0x00   /* it's multiply because of mismatch in the document */
#define HDICA_ECM_ENABLE_RECEPTION  0x80


extern NDS_STATUS XHDICA_DefineEcmPid (NDS_ULONG x_connection,
                                       NDS_BYTE ecm_buf_pair_num,
                                       NDS_USHORT pid,
                                       NDS_BYTE odd_ecm_table_id,
                                       NDS_BYTE even_ecm_table_id);

extern NDS_STATUS XHDICA_DefineEcmFilter (NDS_ULONG x_connection,
                                          NDS_BYTE ecm_filter_num,
                                          NDS_BYTE ecm_filter_control);

/*  HDICA_ECM_FILTER_CONTROL_ODD_RECEPTION_MASK */
#define HDICA_ECM_FILTER_CONTROL_DISABLE_ODD_RECEPTION  0x00
#define HDICA_ECM_FILTER_CONTROL_ENABLE_ODD_RECEPTION   0x20

/*  HDICA_ECM_FILTER_CONTROL_EVEN_RECEPTION_MASK */
#define HDICA_ECM_FILTER_CONTROL_DISABLE_EVEN_RECEPTION 0x00
#define HDICA_ECM_FILTER_CONTROL_ENABLE_EVEN_RECEPTION  0x40

extern NDS_STATUS XHDICA_ReleaseEcmBuffer (NDS_ULONG x_connection,
                                           NDS_BYTE ecm_filter_num,
                                           NDS_BYTE ecm_polarity,
                                           NDS_BYTE *ecm_buf);
extern NDS_STATUS XHDICA_ReleaseAllEcmBuffers (NDS_ULONG x_connection);

/* CORECA_EcmReceived Return Status */
#define CA_ECM_CHECKSUM_ERROR           1
#define CA_INVALID_BUFFER_NUMBER        2
#define CA_ECM_WAS_NOT_USED             3

 /* ECM_RECEIVED_STATUS */
#define ECM_OK          0
#define ECM_OVERFLOW    1

   /* ECM BUFFER */
#define HDICA_EVEN_BUFFER     1
#define HDICA_ODD_BUFFER      2


typedef struct {
   NDS_BYTE rcv_status; /* ECM_OK or ECM_OVERFLOW */
   NDS_BYTE ecm_filter_num;
   NDS_BYTE ecm_polarity; /* HDICA_ODD_BUFFER or HDICA_EVEN_BUFFER */
   NDS_BYTE *ecm_buf_ptr; /* pointer to the ECM payload*/
}XECM_MSG;


extern NDS_STATUS XCORECA_EcmReceived (NDS_ULONG x_connection,
                                       XECM_MSG *ecm_msg,
                                       NDS_ULONG hi_counter,
                                       NDS_ULONG lo_counter);

/****************************************************************************/
/*                                                                          */
/*                      EMM INTERFACE                                       */
/*                                                                          */
/****************************************************************************/
extern NDS_STATUS XHDICA_SetEmmPid (NDS_ULONG x_connection,
                                    NDS_USHORT pid,
                                    NDS_BYTE emm_filter_control);

/*  HDICA_EMM_RECEPTION_MASK */
#define HDICA_EMM_RECEPTION_DISABLE 0x00
#define HDICA_EMM_RECEPTION_ENABLE  0x80

#define HDICA_EMM_DISABLE_RECEPTION 0x00   /* it's multiply because of mismatch in the document */
#define HDICA_EMM_ENABLE_RECEPTION  0x80

/*  HDICA_TABLE_ID_FILTERING_MASK    */
#define HDICA_TABLE_ID_FILTERING_IGNORE 0x00
#define HDICA_TABLE_ID_FILTERING_USE    0x40

extern NDS_STATUS XHDICA_EmmSetTableIdFilter (NDS_ULONG x_connection,
                                              NDS_BYTE table_id[4]);

/* TABLE ID FILTER CONTROL NDS_BYTE */
#define IGNORE_THIS_ID  0x00
#define RECEIVE_ALL     0x01
#define FILTER_BY_NDC   0x10

typedef struct {
   NDS_BYTE reg1_address[4];
   NDS_BYTE reg1_mask[4];
   NDS_BYTE reg2_address[4];
   NDS_BYTE reg2_mask[4];
   NDS_BYTE reg3_address[4];
   NDS_BYTE reg3_mask[4];
} EMM_REG;

extern NDS_STATUS XHDICA_SetPrivateEmmFilter (NDS_ULONG x_connection,
                                             EMM_REG *emm_reg);

extern NDS_STATUS XHDICA_ReleaseEmmBuffer(NDS_ULONG x_connection,
                                         NDS_BYTE *emm_buf);

/* CORECA_EmmReceivedMsg Return Status */
#define CA_EMM_WAS_NOT_USED             3

/* EMM_RECEIVED_STATUS */
#define  EMM_OK          0
#define  EMM_OVERFLOW    1

typedef struct
{
   NDS_BYTE  receive_status;
   NDS_BYTE *emm_buf;
} XEMM_MSG;

extern NDS_STATUS XCORECA_EmmReceived (NDS_ULONG x_connection,
                                       XEMM_MSG *emm_msg);


/****************************************************************************/
/*                                                                          */
/*                Descrambling Elementary Streams                           */
/*                                                                          */
/****************************************************************************/
extern NDS_STATUS XHDICA_DefineDescramblerEs (NDS_ULONG x_connection,
                                              NDS_BYTE descrambler_num,
                                              NDS_USHORT pid,
                                              NDS_BYTE filter_control);
#define HDICA_DESCRAMBLER_CONTROL_ENABLE_DESCRAMBLING   0x80
#define HDICA_DESCRAMBLER_CONTROL_DISABLE_DESCRAMBLING  0x00

#define HDICA_DESCRAMBLE_TRANSPORT_PACKET_LEVEL 0x00
#define HDICA_DESCRAMBLE_PES_PACKET_LEVEL       0x40

extern NDS_STATUS XHDICA_SetEvenKey (NDS_ULONG x_connection,
                                     NDS_BYTE descrambler_num,
                                     NDS_BYTE *key_ptr);
extern NDS_STATUS XHDICA_SetOddKey  (NDS_ULONG x_connection,
                                     NDS_BYTE descrambler_num,
                                     NDS_BYTE *key_ptr);

extern NDS_STATUS XHDICA_DefineDescramblerType(NDS_ULONG x_connection,
                                        NDS_BYTE descrambler_type);

#define CSA     0
#define NSA     1
#define DES     2
#define AES     3


/****************************************************************************/
/*                                                                          */
/*                Smart Card Interface                                      */
/*                                                                          */
/****************************************************************************/

#define SC_INSERTED        0
#define SC_REMOVED         1
#define SC_COMM_OK         2
#define SC_COMM_OVERFLOW   3
#define SC_PARITY          4
#define SC_ERROR           5

extern NDS_STATUS CORECA_SmartCardComm (NDS_BYTE smart_card_comm_msg);


extern NDS_STATUS HDICA_SetCardId(NDS_ULONG card_id);
extern NDS_STATUS HDICA_SetIrdId(NDS_ULONG ird_id);
extern NDS_STATUS HDICA_SetCardClockDivisor(NDS_BYTE clock_divisor);

/*  CLOCK_DIVISOR */
#define HIGHEST_CLOCK  1
#define MEDIUM_CLOCK   2
#define LOWEST_CLOCK   3

#define DIVIDE_BY_2    1
#define DIVIDE_BY_4    2
#define DIVIDE_BY_6    3
#define DIVIDE_BY_8    4

extern NDS_STATUS HDICA_SetVccLevel(NDS_BYTE vcc_level);

/*  VCC LEVEL  */
#define HDICA_VCC_3V    0
#define HDICA_VCC_5V    1


extern NDS_STATUS HDICA_SetConvention (NDS_BYTE convention);

/* TRANSMISSION_CONVENTION */
#define HDICA_CONVENTION_INVERSE    0x00
#define HDICA_CONVENTION_DIRECT     0x01

extern NDS_STATUS HDICA_SetUartBaudRate (NDS_BYTE baud_rate);

#define HDICA_BAUD_RATE_9600    1
#define HDICA_BAUD_RATE_19200   2
#define HDICA_BAUD_RATE_38400   3
#define HDICA_BAUD_RATE_76800   4
#define HDICA_BAUD_RATE_153600  5
#define HDICA_BAUD_RATE_223200  6  /* F= 512; D= 32 */

extern NDS_STATUS HDICA_SetVcc (NDS_BYTE vcc_switch);

#define HDICA_VCC_ON    0
#define HDICA_VCC_OFF   1

extern NDS_STATUS HDICA_SetVpp (NDS_BYTE vpp_switch);

#define HDICA_VPP_ON    0
#define HDICA_VPP_OFF   1

extern NDS_STATUS HDICA_SetGuardTime (NDS_BYTE guard_time);

extern NDS_STATUS HDICA_GetInputByte (NDS_BYTE *input_byte);
extern NDS_STATUS HDICA_PutOutputByte (NDS_BYTE output_byte);
extern NDS_STATUS HDICA_ResetCard (NDS_BYTE reset_switch);

#define INACTIVE    0
#define ACTIVE      1
#define RESET_CYCLE 2

extern NDS_STATUS HDICA_GetUartStatus (NDS_BYTE *uart_status);

/* HDICA_UART_STATUS_TX_READY_MASK */
#define HDICA_UART_STATUS_TX_IS_READY           0x01    /* bit 0 */
#define HDICA_UART_STATUS_TX_IS_NOT_READY       0x00

/* HDICA_UART_RX_READY_MASK */
#define HDICA_UART_STATUS_RX_IS_READY           0x02    /* bit 1 */
#define HDICA_UART_STATUS_RX_IS_NOT_READY       0x00

/* HDICA_UART_STATUS_PARITY_ERROR_LAST_BYTE_RCVD_MASK */
#define HDICA_UART_STATUS_RCV_PARITY_ERROR      0x04    /* bit 2 */
#define HDICA_UART_STATUS_RCV_NO_PARITY_ERROR   0x00

/* HDICA_UART_STATUS_RCV_BUFFER_OVERFLOW_MASK */
#define HDICA_UART_STATUS_OVERFLOW              0x08    /* bit 3 */
#define HDICA_UART_STATUS_NO_OVERFLOW           0x00

/* HDICA_UART_STATUS_PARITY_ERROR_LAST_BYTE_TRANSMITTED_MASK */
#define HDICA_UART_STATUS_TX_PARITY_ERROR       0x10    /* bit 4 */
#define HDICA_UART_STATUS_TX_NO_PARITY_ERROR    0x00

/* HDICA_UART_STATUS_CONVENTION_MASK */
#define HDICA_UART_STATUS_CONVENTION_INVERSE    0x00
#define HDICA_UART_STATUS_CONVENTION_DIRECT     0x20    /* bit 5 */

/* HDICA_UART_STATUS_INTERRUPT_PENDING_MASK */
#define HDICA_UART_STATUS_INTERRUPT_PENDING     0x40    /* bit 6 */
#define HDICA_UART_STATUS_INTERRUPT_NOT_PENDING 0x00

/* HDICA_UART_STATUS_CARD_DETECT_MASK */
#define HDICA_UART_STATUS_CARD_INSERTED         0x80    /* bit 7 */
#define HDICA_UART_STATUS_CARD_NOT_INSERTED     0x00

extern NDS_STATUS HDICA_SetCaValue (NDS_BYTE ca_value);
extern NDS_STATUS XHDICA_SetCaValue (NDS_ULONG x_connection , NDS_BYTE ca_value);
extern NDS_STATUS HDICA_SetUartCommand (NDS_BYTE pin_mask);

extern NDS_STATUS HDICA_SendAndReceiveBytesSession (NDS_BYTE *xmt_ptr,
                              NDS_BYTE      xmit_length,
                              CAHDI_BOOLEAN   null_filter,
                              CAHDI_BOOLEAN   use_flow_control,
                              NDS_BYTE      *rcv_ptr,
                              NDS_BYTE      rcv_length
                              );

extern NDS_STATUS HDICA_ReceiveBytesSession (CAHDI_BOOLEAN null_filter,
                                      CAHDI_BOOLEAN use_flow_control,
                                      NDS_BYTE    *rcv_ptr,
                                      NDS_BYTE    rcv_length);

extern NDS_STATUS HDICA_AbortSmartCardCommSession (void);

/****************************************************************************/
/*                                                                          */
/*                Recording control interface                               */
/*                                                                          */
/****************************************************************************/
#define CA_OP_BASE_KEY  0x00
#define CA_OP_READING   0x01
#define CA_OP_WRITING   0x02

extern NDS_STATUS XHDICA_ECMCorruptionSettings (NDS_BYTE CorruptDataByte,
                                           NDS_BYTE FromByte);
extern NDS_STATUS XHDICA_EnableECMCorruption (NDS_ULONG x_connection,
                                              NDS_USHORT pid,
                                              NDS_BYTE enable_corruption);
extern NDS_STATUS XHDICA_SecondaryScramblingKey (NDS_ULONG x_connection,
                                                 NDS_BYTE operation,
                                                 NDS_BYTE* key_ptr);






//extern NDS_STATUS CORECA_EmmReceived( XEMM_MSG *emm_msg );
//extern NDS_STATUS CORECA_EcmReceived( XECM_MSG *ecm_msg );

/****************************************************************************/
/*                                                                          */
/*                Control Word Encryption Interface                         */
/*                                                                          */
/****************************************************************************/
#define ESD_CLEAR_CW_LENGTH             8
#define ESD_ENCRYPTED_CW_LENGTH         8
#define ESD_ENCRYPTED_CKEY_LENGTH       16
#define ESD_ENCRYPTED_ACKEY_LENGTH      16

/* The following are descriptor tags for ExtendedSetDescrambler */
#define ESD_CLEAR_CW            0x01
#define ESD_ENCRYPTED_CW        0x02
#define ESD_EKEY                0x03 /* encrypted key - Ekpair/Eknopair used for CW encryption feature */
#define ESD_VGS_CHAINING_KEY    0x08
#define ESD_VGS_AC_KEY          0x09
#define ESD_VGS_OPTIONS         0x10

/* The following are descriptor tags for the chip properties structure */
#define CHIP_TYPE_DSCR_TAG              0x11
#define CHIP_ID_DSCR_TAG                0x12
#define HASH_KEY_DSCR_TAG               0x13 /* encrypted key - Ekpair/Eknopair used for CW encryption feature */
#define KEY_LADDER_DEPTH_DSCR_TAG       0x14
#define ENABLE_FLAGS_DSCR_TAG           0x15

/* The following are descriptor tags for the descrambling key structure */
#define CLEAR_CW_DSCR_TAG               0x01
#define ENCRYPTED_CW_DSCR_TAG           0x02
#define ENCRYPTED_KEY_DSCR_TAG          0x03
#define ENCRYPTION_SCHEME_DSCR_TAG      0x04

/* The following are Encryption Scheme Values Definition */
#define ENS_3DES_DED_2KEY               2
#define ENS_3DES_DED_2KEY_TWIST         3
#define ENS_3DES_DED_2KEY_HALF_TWIST    4
#define ENS_AES_128_BITS                0x10

#define ESD_VGS_CONFORMANCE     0x80

/*
#define CA_INFO_NOT_AVAILABLE                           7
#define CA_INSUFFICIENT_BUFFER                          8
#define CA_SCHEME_NOT_SUPPORTED                         9
#define CA_FUNCTIONALITY_NOT_AVAILABLE_UNTIL_RESTART    10
#define CA_FUNCTIONALITY_PERMANENTLY_NOT_AVAILABLE      11
#define CA_FEATURE_NOT_SUPPORTED                        12
*/

/* The following define how Block Clear CWS should be performed */
#define BLOCK_FOREVER                   0
#define BLOCK_TILL_NEXT_RESET           1

#define KEY_IN_BOX                      1
#define FLASH_ID                        2

#define CTRL_ENCRYPTION_SUPPORTED               (1<< 0)
#define CTRL_ENCRYPTION_PERMANENTLY_FORCED      (1<< 1)
#define CTRL_ENCRYPTION_FORCED_UNTIL_RESET      (1<< 2)
#define JTAG_PASSWORD_ENABLED                   (1<< 3)
#define BOOTRON_ENABLED                         (1<< 4)
#define DATA_ENCRYPTION_SUPPORTED               (1<< 5)
#define SPARE_1                                 (1<< 6)
#define SPARE_2                                 (1<< 7)

#define ENCRYPT_IN_PARAMS_DSCR_TAG 0x0a
#define MAX_AUX_LEN 10

enum {
    TID_ECM_EVEN = 0x80,
    TID_ECM_ODD,
};

enum {
    POLARITY_CLEAR = 0,
    POLARITY_ODD,
    POLARITY_EVEN,
};

extern NDS_STATUS XHDICA_GetChipProperties(NDS_ULONG hw_module_id,
                                    unsigned int chip_prop_length,
                                    unsigned int *actual_chip_prop_length,
                                    NDS_BYTE *chip_properties,
                                    void *auxiliary_data);

extern NDS_STATUS XHDICA_SetControlWord(NDS_ULONG x_connection,
                                 NDS_USHORT descrambler_num,
                                 unsigned int control_data_length,
                                 NDS_BYTE *control_data,
                                 unsigned int polarity_control);

extern NDS_STATUS XHDICA_GetSerializationInfo(unsigned int info_type,
                                       unsigned int serializaion_data_len,
                                       unsigned int *actual_serialization_data_len,
                                       NDS_BYTE *serialization_data);

/****************************************************************************/
/*                                                                          */
/*                VGS Interface                                             */
/*                                                                          */
/****************************************************************************/
NDS_STATUS XHDICA_GetResponseToChallenge(NDS_BYTE *nonce,
                                         NDS_BYTE nonce_length,
                                         int descrambling_key_length,
                                         NDS_BYTE *descrambling_key_descrip,
                                         NDS_BYTE *response,
                                         NDS_BYTE *response_length);

/****************************************************************************/
/*                                                                          */
/*                Data Encryption Interface                                 */
/*                                                                          */
/****************************************************************************/
unsigned short XHDICA_EncryptData(void *encrypt_in_params,
                              unsigned int *out_data_buf_len,
                              NDS_BYTE *out_data_buf);


//////------- NSK ------------//////
unsigned short HDIHAL_WriteNSKReg(unsigned long instance,
                                    unsigned long offset,
                                    unsigned long length,
                                    unsigned long *reg_data_out);

unsigned short HDIHAL_ReadNSKReg(unsigned long instance,
                                    unsigned long offset,
                                    unsigned long length,
                                    unsigned long *reg_data_in);


unsigned short HDIHAL_WriteNSKData(unsigned long instance,
                                    unsigned long offset,
                                    unsigned long length,
                                    unsigned char *data_out);


unsigned short HDIHAL_ReadNSKData(unsigned long instance,
                                    unsigned long offset,
                                    unsigned long length,
                                    unsigned char *data_in);

/*unsigned short HALHDI_InterruptServiceRoutine(
                                    unsigned long instance,
                                    unsigned long interrupt_source);


unsigned short HALHDI_InitSVP_HW(unsigned long instance);*/


/////-------- SVP --------//////
unsigned short SVPHDI_ReadSerialData
(
    unsigned int offset,
    unsigned int length,
    unsigned char* data
);


unsigned short SVPHDI_AllocateCmChannel
(
    unsigned long xconn,
    unsigned int esa_select,
    unsigned int esa_subselect,
    unsigned int* cm_channel_handle
);


unsigned short SVPHDI_DeallocateCmChannel
(
    unsigned int cm_channel_handle
);


unsigned short SVPHDI_AddEsPidCmChannel
(
    unsigned int cm_channel_handle,
    unsigned short es_pid
);


unsigned short SVPHDI_RemoveEsPidCmChannel
(
    unsigned int cm_channel_handle,
    unsigned short es_pid
);


unsigned short SVPHDI_EnableCmChannel
(
    unsigned int cm_channel_handle
);


unsigned short SVPHDI_DisableCmChannel
(
    unsigned int cm_channel_handle
);

unsigned short SVPHDI_WriteKey
(
    unsigned long instance,
    unsigned int cm_channel_handle,
    unsigned char scb,
    unsigned int scrambling_settings_len,
    unsigned char* scrambling_settings
);


unsigned short SVPHDI_GetContentModuleProperties
(
    unsigned int cm_prop_length,
    unsigned int* actual_cm_prop_len,
    unsigned char* cm_properties
);




/////---- no support now -----
unsigned short SVPHDI_SetSecondaryVideoPid
(
    unsigned int cm_channel_handle,
    unsigned short primary_pid,
    unsigned short secondary_pid
);


unsigned short SVPHDI_ReleaseSecondaryVideoPid
(
    unsigned int cm_channel_handle,
    unsigned short secondary_pid
);


unsigned short SVPHDI_StartPacketInjection
(
    unsigned long xconn,
    unsigned char* transport_packet,
    unsigned int injection_type,
    unsigned int injection_frequency
);


unsigned short SVPHDI_InjectSinglePacket
(
    unsigned long xconn,
    unsigned char* transport_packet
);


unsigned short SVPHDI_ContinousReplacePacket
(
    unsigned long xconn,
    unsigned short replace_pid,
    unsigned char* transport_packet
);


unsigned short SVPHDI_TerminatePacketWrite
(
    unsigned long xconn,
    unsigned short pid
);

unsigned short HDIHAL_WriteDescrambler
(
    unsigned long xconn,
    unsigned short pid,
    unsigned char scb,
    unsigned char flags
);



/****************************************************************************/
/*                                                                                   */
/*                XTV RASP Low Level Driver Interdace                                */
/*                                                                                   */
/****************************************************************************/

typedef enum _MS_XBOOL
{
    _MS_XFALSE = 0,
    _MS_XTRUE  = 1
} _MS_XBOOL;


typedef struct
{
    unsigned long        Event_Descriptor;
    unsigned short       Pid;
    unsigned short       Reserved;
    unsigned long        PacketNum;
    unsigned long        Timer;
    unsigned long        PCR;

}RASP_EVENT;

typedef struct
{
    unsigned long     Supported_Event;
    unsigned long             Event_Timer_Rate;
    unsigned short            Num_Rasps;
    unsigned short            Max_Event_Num_In_Fifo;
    unsigned short            Num_of_Payloaad_Buffers;
    unsigned short            Num_of_Filters;
    _MS_XBOOL             Event_Generation_Every_Slot_Time;
    _MS_XBOOL             PCR_Stamp;
    _MS_XBOOL             Local_Clock_Stamp;
    _MS_XBOOL             Trigger_by_Evnet;
    _MS_XBOOL             Trigger_by_Time ;

}RASP_CAPS;


typedef unsigned short  (*RASP_EVENT_NOTIFY_FOUNCTION)
(
    unsigned long      Xconn,
    unsigned short          NumEvent,
    RASP_EVENT      RaspEvents[]
);

typedef unsigned short  (*RASP_OVERFLOW_NOTIFY_FUNCTION)
(
    unsigned long      Xconn
);

unsigned short  RASPLDI_GetCapabilities(RASP_CAPS *RaspCaps) ;
unsigned short  RASPLDI_GetEventMask
(
    unsigned long         Xconn,
    unsigned char             FilterNum,
    unsigned short            *Pid,
    unsigned long     *EventMask,
    unsigned long     *PayLoadMask
) ;

unsigned short  RASPLDI_GetTimerAndPacketNumber
(
    unsigned long      Xconn,
    unsigned long           *Time,
    unsigned long           *PacketNumber
) ;

unsigned short  RASPLDI_GetTSPayload
(
    unsigned long      Xconn,
    unsigned long           PacketNum,
    unsigned char          Buffer[188]
);

unsigned short  RASPLDI_Reset(unsigned long  Xconn) ;

unsigned short  RASPLDI_SetEventNotifyFunction
(
    unsigned long      Xconn,
    RASP_EVENT_NOTIFY_FOUNCTION     *RaspEventCB
);

unsigned short  RASPLDI_SetOverflowNotifyFunction
(
    unsigned long     Xconn,
    RASP_OVERFLOW_NOTIFY_FUNCTION      *RaspOvfCB
) ;


unsigned short  RASPLDI_Configure
(
    unsigned long        Xconn,
    unsigned long        TriggerExecutor,
    unsigned short       EventHighMark,
    unsigned short       TimeHighMark,
    unsigned long        PidIndEvent
);


unsigned short  RASPLDI_SetEventMask
(
    unsigned long        Xconn,
    unsigned char            FilterNum,
    unsigned short            Pid,
    unsigned long     EventMask,
    unsigned long     PayloadMask
);


unsigned short  RASPLDI_ReplacePackets
(
    unsigned long       Xconn,
    unsigned short           Pid,
    unsigned char           FromByte,
    unsigned char           NumBytes,
    unsigned char          *ReplaceData
);


unsigned short  RASPLDI_Start(unsigned long  Xconn);


#endif

