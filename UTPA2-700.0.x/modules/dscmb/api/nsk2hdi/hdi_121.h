/*
=
                                        Hdi_121.h
  Description			:	This is the header file for CA Hardware Driver High-Level Interface for NSK2 chips
=
====================================================================================================*/

#ifndef H_HDI_121
#define H_HDI_121

#include "ndstypes.h"


/* ***** Defines ***** */
#define NSK2HDX_OK						                    0U
#define NSK2HDX_FAIL						                2U   /* should not be changed */
#define NSK2HDX_INVALID_REQUEST                             3U

#define NSK2HDX_MAX_STREAMS	      20U

/****************************************/
/*	  	  Driver Initialization  		*/
/****************************************/

//NDS_STATUS32 NSK2HDX_GetCaDriverVersion(NDS_ULONG *version_no/* RPC IN OUT */);
NDS_STATUS32 NSK2HDX_GetCaDriverVersion(char **version_no/* RPC IN OUT */); // barry replace with this one


NDS_STATUS32 NSK2HDX_SetScreaderHwInfo(NDS_ULONG *screader_ids/* RPC IN OUT */, NDS_ULONG number_of_screaders);

NDS_STATUS32 NSK2HDX_SetEmmHwInfo(NDS_ULONG num_emm_buffers);

NDS_STATUS32 NSK2HDX_SetEcmHwInfo(NDS_ULONG num_ecm_buffers);

/****************************************/
/*	  	    Smart Card Driver   		*/
/****************************************/

NDS_STATUS32 NSK2HDX_CardInserted(NDS_ULONG screader_id);

NDS_STATUS32 NSK2HDX_CardRemoved(NDS_ULONG screader_id);

NDS_STATUS32 NSK2HDX_CardError(NDS_ULONG screader_id);

#define NSK2HDX_CARD_INSERTED      0U
#define NSK2HDX_CARD_REMOVED       1U

NDS_STATUS32 NSK2HDX_CheckCardPresence(NDS_ULONG screader_id, NDS_ULONG *card_presence/* RPC IN OUT */);

/*  VCC LEVEL  */
#define NSK2HDX_VCC_3V    0U
#define NSK2HDX_VCC_5V    1U

NDS_STATUS32 NSK2HDX_SetVccLevel(NDS_ULONG screader_id, NDS_UBYTE vcc_level);

/*  VCC SWITCH */
#define NSK2HDX_VCC_ON    0U
#define NSK2HDX_VCC_OFF   1U

NDS_STATUS32 NSK2HDX_SetVcc(NDS_ULONG screader_id, NDS_UBYTE vcc_switch);

NDS_STATUS32 NSK2HDX_WarmResetCard(NDS_ULONG screader_id, NDS_UBYTE *atr_data/* RPC IN OUT SIZECONST(32) */, NDS_UBYTE *atr_len/* RPC IN OUT */, NDS_UBYTE *first_history_byte/* RPC IN OUT */);

#define NSK2HDX_ISO_INVERSE_CONVENTION   0U
#define NSK2HDX_ISO_DIRECT_CONVENTION    1U

#define NSK2HDX_SC_PROTOCOL_T_0           0U

#define NSK2HDX_SC_CLOCK_1                1U  /* 4.5  Mhz */
#define NSK2HDX_SC_CLOCK_3                3U  /* 6.75 Mhz */
#define NSK2HDX_SC_CLOCK_5                5U  /* 13.5 Mhz */
#define NSK2HDX_SC_CLOCK_6                6U  /* 20   Mhz */
#define NSK2HDX_SC_BR_1                   1U  /* baud rate of 9600 */
#define NSK2HDX_SC_BR_2                   2U  /* baud rate of 19K */
#define NSK2HDX_SC_BR_3                   3U  /* baud rate of 38K */
#define NSK2HDX_SC_BR_4                   4U  /* baud rate of 76K */
#define NSK2HDX_SC_BR_5                   5U  /* baud rate of 153K */
#define NSK2HDX_SC_BR_F                   0x0FU  /* baud rate of 1M */


typedef struct _NSK2HDX_UARTCONFIGURATION
{
	NDS_UBYTE card_clock;
	NDS_UBYTE card_baud_rate;
	NDS_UBYTE nak_retries;
	NDS_UBYTE parity_retries;
	NDS_UBYTE work_wait_time;
	NDS_UBYTE guard_time;
	NDS_UBYTE convention;
	NDS_UBYTE protocol;
	NDS_UBYTE spare[4];

} NSK2HDX_UARTCONFIGURATION;

NDS_STATUS32 NSK2HDX_SetUartParams(NDS_ULONG screader_id, NSK2HDX_UARTCONFIGURATION *conf/* RPC IN OUT */);
NDS_STATUS32 NSK2HDX_GetUartParams(NDS_ULONG screader_id, NSK2HDX_UARTCONFIGURATION *conf/* RPC IN OUT */);

#define NSK2HDX_DIRECTION_FROM_CARD     0U

#define NSK2HDX_DIRECTION_TO_CARD       1U

NDS_STATUS32 NSK2HDX_CardIoCommand(NDS_ULONG screader_id, NDS_UBYTE *to_card/* RPC IN OUT SIZEPARAM(to_card_len) */, NDS_ULONG to_card_len, NDS_UBYTE *from_card/* RPC SIZECONST(256) */, NDS_ULONG *from_card_len/* RPC IN OUT */, NDS_UBYTE direction, NDS_USHORT timeout);

#define NSK2HDX_UART_COMMAND_IO_C7_ON     	0x04U
#define NSK2HDX_UART_COMMAND_IO_C4_ON       0x08U
#define NSK2HDX_UART_COMMAND_IO_C8_ON     	0x10U

#define NSK2HDX_UART_COMMAND_C7_ON     	    0x80U
#define NSK2HDX_UART_COMMAND_C4_ON 	        0x20U
#define NSK2HDX_UART_COMMAND_C8_ON     	    0x40U

NDS_STATUS32 NSK2HDX_SetUartCommand(NDS_ULONG screader_id, NDS_UBYTE pin_mask);
NDS_STATUS32 NSK2HDX_GetUartCommand(NDS_ULONG screader_id, NDS_UBYTE *pin_mask/* RPC IN OUT */);

/****************************************/
/*		EMM Filtering and Reception		*/
/****************************************/

/* values for filter_type */
#define NSK2HDX_LABEL_01            0x01U
#define NSK2HDX_LABEL_02            0x02U
#define NSK2HDX_LABEL_03            0x03U

/* values for table_id_flag */
#define NSK2HDX_DISABLE_TID           0x00U
#define NSK2HDX_ENABLE_TID			  0x01U
#define NSK2HDX_ENABLE_TID_NDS        0x02U

typedef struct _NSK2_EMM_FILTER_DEF
{
    NDS_UBYTE   filter_type;
    NDS_UBYTE   filter_address[4];
    NDS_UBYTE   filter_address_mask[4];

} NSK2HDX_EMM_FILTER_DEF;

typedef struct _NSK2_EMM_FILTER
{
    NDS_ULONG		size_of_struct;
    NDS_USHORT		emm_pid;
    NDS_ULONG		num_of_filters;
    NDS_UBYTE		table_id[16];
    NDS_UBYTE		table_id_flag[16];
    NSK2HDX_EMM_FILTER_DEF filter_def[8];

} NSK2HDX_EMM_FILTER;

NDS_STATUS32 NSK2HDX_RequestEmm(NDS_ULONG x_connection, NSK2HDX_EMM_FILTER *emmfl/* RPC IN OUT */);

NDS_STATUS32 NSK2HDX_EmmArrived(NDS_ULONG x_connection, NDS_UBYTE *emm_buf/* RPC SIZECONST(256) */);

NDS_STATUS32 NSK2HDX_EmmBufferOverflow(NDS_ULONG x_connection);

NDS_STATUS32 NSK2HDX_EmmProcessingFinished(NDS_UBYTE *emm_buf/* RPC SIZECONST(256) */);

NDS_STATUS32 NSK2HDX_StopEmm(NDS_ULONG x_connection);

/****************************************/
/*		ECM Filtering and Reception		*/
/****************************************/

#define NSK2HDX_ODD_ECM      0U
#define NSK2HDX_EVEN_ECM     1U


NDS_STATUS32 NSK2HDX_RequestEcm(NDS_ULONG x_connection, NDS_USHORT ecm_pid);

NDS_STATUS32 NSK2HDX_EcmArrived(NDS_ULONG x_connection, NDS_USHORT ecm_pid, NDS_UBYTE *ecm_buf/* RPC SIZECONST(256) */, NDS_UBYTE polarity);

NDS_STATUS32 NSK2HDX_GetPacketCounter(NDS_ULONG x_connection, NDS_ULONG packet_counter_length, NDS_UBYTE *packet_counter/* RPC IN OUT SIZEPARAM(packet_counter_length) */);

NDS_STATUS32 NSK2HDX_EcmProcessingFinished(NDS_USHORT ecm_pid, NDS_UBYTE *ecm_buf/* RPC SIZECONST(256) */, NDS_UBYTE polarity);

NDS_STATUS32 NSK2HDX_StopEcm(NDS_ULONG x_connection, NDS_USHORT ecm_pid);

#endif
