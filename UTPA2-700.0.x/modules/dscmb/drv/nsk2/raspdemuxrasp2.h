/*
Copyright ((c)) NDS Limited 2005-2010

P R O P R I E T A R Y  &  C O N F I D E N T I A L

The copyright of this code and related documentation together with
any other associated intellectual property rights are vested in
NDS Limited and may not be used except in accordance with the terms
of the licence that you have entered into with NDS Limited.
Use of this material without an express licence from NDS Limited
shall be an infringement of copyright and any other intellectual
property rights that may be incorporated with this material.

P L E A S E     N O T E :

This header file has been automatically generated from a source CDI
Specification file in XML format.
*/
/*
CDI Issue 7
*/

/*
CDI header file for the demux rasp device.
This file is immutable. Do not edit.
*/

#ifndef __NDS_CDI_RASPDEMUXRASP_H__
#define __NDS_CDI_RASPDEMUXRASP_H__

/* Include the platform-specific IOCTL macros. */
#include "MsCommon.h"


/* Demux RASP notification field definitions. */
#define DEMUX_RASP_TPID_PAYLOAD_UNIT_START_INDICATOR       (0x00000001U)
#define DEMUX_RASP_TPID_CHANGE_ES_TO_NOT_SCRAMBLED         (0x00000002U)
#define DEMUX_RASP_TPID_CHANGE_ES_TO_EVEN_SCRAMBLED        (0x00000004U)
#define DEMUX_RASP_TPID_CHANGE_ES_TO_ODD_SCRAMBLED         (0x00000008U)
#define DEMUX_RASP_TPID_DISCONTINUITY_INDICATOR            (0x00000100U)
#define DEMUX_RASP_TPID_RANDOM_ACCESS_INDICATOR            (0x00000200U)
#define DEMUX_RASP_TPID_ES_PRIORITY_INDICATOR              (0x00000400U)
#define DEMUX_RASP_TPID_PCR_FLAG                           (0x00000800U)
#define DEMUX_RASP_TPID_OPCR_FLAG                          (0x00001000U)
#define DEMUX_RASP_TPID_SPLICING_POINT_FLAG                (0x00002000U)
#define DEMUX_RASP_TPID_TRANSPORT_PRIVATE_DATA             (0x00004000U)
#define DEMUX_RASP_TPID_ADAPTATION_FIELD_EXTENSION_FLAG    (0x00008000U)
#define DEMUX_RASP_GENERAL_FIRST_PACKET                    (0x01000000U)
#define DEMUX_RASP_GENERAL_INTERNAL_TICK                   (0x02000000U)
/* Obsolete:
	The following definitions for DEMUX_RASP_* are obsolete
	as of CDI [version 2] Issue 5.
	The definitions may be removed in a future version
	of the CDI specification and from this header file.
*/
#define DEMUX_RASP_TPID_SECTION_FILTER_MATCH               (0x00000020U)
#define DEMUX_RASP_TPID_PACKET_REPLACEMENT                 (0x00010000U)

/* Demux RASP Event definitions. */
#define DEMUX_RASP_EVENT_THRESHOLD_REACHED               (0x00000001U)
#define DEMUX_RASP_EVENT_OVERFLOW                        (0x00000002U)
#define DEMUX_RASP_EVENT_TP_PAYLOAD_THRESHOLD_REACHED    (0x00000004U)
#define DEMUX_RASP_EVENT_TP_PAYLOAD_OVERFLOW             (0x00000008U)

/* Capabilities Versions definitions. */
#define DEMUX_RASP_CAP_V0    (0x00000001U)
#define DEMUX_RASP_CAP_V1    (0x00000002U)

/* RASP TP Payload PES Notification Fields definitions. */
#define DEMUX_RASP_PES_PTS    (0x00000001U)

/* RASP Encoding Formats definitions. */
#define DEMUX_RASP_ES_ENCODING_FORMAT_MPEG2    (0x00000001U)

/* MPEG2 ES Encoding Format definitions. */
#define DEMUX_RASP_ES_MPEG2_SEQUENCE_START     (0x00000001U)
#define DEMUX_RASP_ES_MPEG2_PICTURE_I_START    (0x00000010U)
#define DEMUX_RASP_ES_MPEG2_PICTURE_P_START    (0x00000020U)
#define DEMUX_RASP_ES_MPEG2_PICTURE_B_START    (0x00000040U)

/* MPEG2 ES Encoding Format definitions. */
#define DEMUX_RASP_ES_MPEG2_PIC_TYPE_FIELD_TOP       (0x01U)
#define DEMUX_RASP_ES_MPEG2_PIC_TYPE_FIELD_BOTTOM    (0x02U)
#define DEMUX_RASP_ES_MPEG2_PIC_TYPE_FRAME           (0x03U)

/* Demux Trigger definitions. */
#define DEMUX_RASP_TRIGGER_BY_TIME         (1U)
#define DEMUX_RASP_TRIGGER_BY_RASP_DATA    (2U)

/// data type unsigned char, data length 1 byte
typedef unsigned char               uint8_t;                              // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short              uint16_t;                             // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned long               uint32_t;                             // 4 bytes

/* DemuxRaspConfiguration structure */
typedef struct DemuxRaspConfiguration_cdi
{
    uint32_t  thresholdType;
    uint32_t  raspDataThreshold;
    uint32_t  timeThreshold;
    uint32_t  tpidIndependentEvents;
} DemuxRaspConfiguration;

/* DemuxRaspTPMask structure */
typedef struct DemuxRaspTPMask_cdi
{
    uint32_t  tpid;
    uint32_t  tpMaskID;
    uint32_t  eventMask;
    uint32_t  payloadMask;
} DemuxRaspTPMask;

/* DemuxRaspTimerTPNumber structure */
typedef struct DemuxRaspTimerTPNumber_cdi
{
    uint32_t  time;
    uint8_t   packetNumber[8];
} DemuxRaspTimerTPNumber;

/* DemuxRaspTP structure */
typedef struct DemuxRaspTP_cdi
{
    uint8_t  packetNumber[8];
    uint8_t  bufferLength;
    uint8_t  *buffer;
} DemuxRaspTP;

/* DemuxRaspReplaceTP structure */

/* Obsolete:
   The following definition for DemuxRaspReplaceTP is obsolete
   as of CDI [version 2] Issue 5.
   The definition may be removed in a future version of the
   CDI specification and from this header file.
*/
typedef struct DemuxRaspReplaceTP_cdi
{
    uint32_t  tpid;
    uint32_t  tpReplacerID;
    uint8_t   fromByte;
    uint8_t   numBytes;
    const void      *replaceData;
} DemuxRaspReplaceTP;

/* DemuxRaspDataBlock structure */
typedef struct DemuxRaspDataBlock_cdi
{
    uint32_t  eventDescriptor;
    uint32_t  tpid;
    uint8_t   packetNumber[8];
    uint32_t  timer;
    uint32_t  streamClock;
} DemuxRaspDataBlock;

/* DemuxRaspRead structure */
typedef struct DemuxRaspRead_cdi
{
    uint32_t            numDataBlocksToRead;
    DemuxRaspDataBlock  *dataBlocks;
    uint32_t            numDataBlocksReturned;
} DemuxRaspRead;

/* DemuxRaspCapsEx structure */
typedef struct DemuxRaspCapsEx_cdi
{
    uint32_t  version;
    void      *capability;
    uint32_t  capabilitySize;
} DemuxRaspCapsEx;

/* Demux RASP features definitions. */
#define DEMUX_RASP_FEATURE_EVERY_SLOT_TIME         (0x01U)
#define DEMUX_RASP_FEATURE_STREAM_CLOCK_STAMP      (0x02U)
#define DEMUX_RASP_FEATURE_LOCAL_CLOCK_STAMP       (0x04U)
#define DEMUX_RASP_FEATURE_TRIGGER_BY_RASP_DATA    (0x08U)
#define DEMUX_RASP_FEATURE_TRIGGER_BY_TIME         (0x10U)
#define DEMUX_RASP_FEATURE_GET_TIMER_IN_TICKS      (0x20U)
#define DEMUX_RASP_FEATURE_TP_PAYLOAD_INDEXING     (0x40U)

/* Demux RASP capability definitions. */

/* Obsolete:
   The following definitions for Demux RASP capability (DEMUX_RASP_CAP_*)
   are obsolete as of CDI [version 2] Issue 5.
   The definition may be removed in a future version of the CDI specification
   and from this header file.
*/
#define DEMUX_RASP_CAP_EVERY_SLOT_TIME         (DEMUX_RASP_FEATURE_EVERY_SLOT_TIME)
#define DEMUX_RASP_CAP_STREAM_CLOCK_STAMP      (DEMUX_RASP_FEATURE_STREAM_CLOCK_STAMP)
#define DEMUX_RASP_CAP_LOCAL_CLOCK_STAMP       (DEMUX_RASP_FEATURE_LOCAL_CLOCK_STAMP)
#define DEMUX_RASP_CAP_TRIGGER_BY_RASP_DATA    (DEMUX_RASP_FEATURE_TRIGGER_BY_RASP_DATA)
#define DEMUX_RASP_CAP_TRIGGER_BY_TIME         (DEMUX_RASP_FEATURE_TRIGGER_BY_TIME)

/* DemuxRaspCaps structure */

/* Obsolete:
   The following definition for DemuxRaspCaps is obsolete
   as of CDI [version 2] Issue 5.
   The definition may be removed in a future version of the
   CDI specification and from this header file.
*/
typedef struct DemuxRaspCaps_cdi
{
    uint32_t  raspEvents;
    uint32_t  clockRate;
    uint16_t  numConnections;
    uint16_t  numRaspDataBlocks;
    uint16_t  numTPs;
    uint16_t  numTPMasks;
    uint16_t  numTPreplacers;
    uint8_t   functionalityMask;
} DemuxRaspCaps;

typedef DemuxRaspCaps DemuxRaspCapsV0;

/* DemuxRaspCapsV1 structure */
typedef struct DemuxRaspCapsV1_cdi
{
    uint32_t  raspEvents;
    uint32_t  clockRate;
    uint16_t  numConnections;
    uint16_t  numRaspDataBlocks;
    uint16_t  numTPs;
    uint16_t  numTPMasks;
    uint8_t   features;
} DemuxRaspCapsV1;

/* TP Payload Data Block Version definitions. */
#define DEMUX_RASP_TP_PAYLOAD_DATABLOCK_V0    (0x00000001U)

/* RASP TP Payload Feature Definitions definitions. */
#define DEMUX_RASP_TP_PAYLOAD_FEATURE_LOCAL_CLOCK_STAMP       (0x00000001U)
#define DEMUX_RASP_TP_PAYLOAD_FEATURE_TRIGGER_BY_RASP_DATA    (0x00000002U)
#define DEMUX_RASP_TP_PAYLOAD_FEATURE_TRIGGER_BY_TIME         (0x00000004U)
#define DEMUX_RASP_TP_PAYLOAD_FEATURE_ES_MPEG2_FRAME_FIELD    (0x00000008U)

/* DemuxRaspTPPayloadCaps structure */
typedef struct DemuxRaspTPPayloadCaps_cdi
{
    uint32_t  features;
    uint32_t  pesEvents;
    uint32_t  esEncodingFormats;
    uint16_t  numRaspTPPayloadDataBlocks;
    uint16_t  numTPPayloadMasks;
    uint8_t   tpPayloadDataBlockVersion;
} DemuxRaspTPPayloadCaps;

/* DemuxRaspTPPayloadESCaps structure */
typedef struct DemuxRaspTPPayloadESCaps_cdi
{
    uint32_t  esEncodingFormat;
    uint32_t  esEvents;
} DemuxRaspTPPayloadESCaps;

/* DemuxRaspTPPayloadConfiguration structure */
typedef struct DemuxRaspTPPayloadConfiguration_cdi
{
    uint32_t  thresholdType;
    uint32_t  raspDataThreshold;
    uint32_t  timeThreshold;
    uint32_t  tpidIndependentEvents;
} DemuxRaspTPPayloadConfiguration;

/* DemuxRaspTPPayloadMask structure */
typedef struct DemuxRaspTPPayloadMask_cdi
{
    uint32_t  tpid;
    uint32_t  tpPayloadMaskID;
    uint32_t  pesEventMask;
    uint32_t  esEncodingFormat;
    uint32_t  esEventMask;
} DemuxRaspTPPayloadMask;

/* tag_PTS_do_not_use_in_client structure */
typedef struct tag_PTS_do_not_use_in_client_cdi
{
    uint32_t  high;
    uint32_t  low;
} tag_PTS_do_not_use_in_client;

typedef tag_PTS_do_not_use_in_client PTS;


/* DemuxRaspTPPayloadDataBlockV0 structure */
typedef struct DemuxRaspTPPayloadDataBlockV0_cdi
{
    uint32_t  pesEventDescriptor;
    uint32_t  esEventDescriptor;
    uint32_t  tpid;
    uint8_t   packetNumber[8];
    uint32_t  timer;
    PTS       pts;
    uint8_t   pictureType;
} DemuxRaspTPPayloadDataBlockV0;

/* DemuxRaspTPPayloadRead structure */
typedef struct DemuxRaspTPPayloadRead_cdi
{
    uint32_t  numDataBlocksToRead;
    void      *dataBlocks;
    uint32_t  numDataBlocksReturned;
} DemuxRaspTPPayloadRead;



#endif
/* End of File. */

