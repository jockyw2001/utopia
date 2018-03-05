/*
Copyright (c) NDS Limited 2010

P R O P R I E T A R Y & C O N F I D E N T I A L

The copyright of this code and related documentation together with
any other associated intellectual property rights are vested in
NDS Limited and may not be used except in accordance with the terms
of the license that you have entered into with NDS Limited.
Use of this material without an express license from NDS Limited
shall be an infringement of copyright and any other intellectual
property rights that may be incorporated with this material.
*/

#ifndef NSK2HDI_HEADER_H_
#define NSK2HDI_HEADER_H_

/**
* @mainpage NSK2 API - HDI Bootloader API for NSK2 Chips
*
* @author Reuben Sumner, Julia Rabinovich
* @date 28/06/2011
* @version 4.01
*
* @file nsk2hdi_bl.h

* @brief NSK2 API - HDI Bootloader API for NSK2 Chips

* This file contains the definitions and functions of the NSK2 Bootloader supplement

*
*/

#include "ndstypes.h"
#include "nsk_282.h"
#include "nsk_3014.h"
#include "hdi_121.h"


//#define DebugEnable

#define NSK2HDI_PVRENG      0   //DMX_PVR_EGN0
#define NSK2HDI_SPSPVRENG   0   //E_DSCMB_SPSPVR_ENG0

#define NSK21_Change



//typedef (void *)NSK2Util_AllocateMemory (MS_U32 u32Size, NSKUTIL_MEMORY_TYPE_e eMemType)
typedef void *  (*P_NSK2_CbAllocateMemory)(MS_U32 u32Size, MS_BOOL bCached);
typedef MS_BOOL (*P_NSK2_CbFreeMemory)(void *pAddress, MS_BOOL bCached);
typedef MS_U32  (*P_NSK2_CbEcmGetType)(MS_U32 x_conn);

//typedef MS_BOOL (*P_NSK2_CbDMXOpenStart)(NDS_ULONG xconn, NDS_USHORT es_pid, NDS_UBYTE  es_type, MS_U8 *DmxIdSect);


typedef struct
{
    MS_BOOL                 bHarmonizer;
    P_NSK2_CbAllocateMemory cbAllocMemory;
    P_NSK2_CbFreeMemory     cbFreeMemory;
    P_NSK2_CbEcmGetType     cbEcmGetType;
} HDIPara_Init_t;

MS_U32 MApi_NSK2HDI_Initial(HDIPara_Init_t *pInit);
MS_BOOL MApi_NSK2_IsHarmonizer(void);
void *MApi_NSK2_AllocateMemory(MS_U32 u32Size, MS_BOOL bCached);
MS_BOOL MApi_NSK2_FreeMemory(void *pAddress, MS_BOOL bCached);
MS_BOOL MApi_NSK2_DTV_ClosePidNo(MS_U8 PidNo);
MS_U32 MApi_NSK2_EcmGetFilterType(MS_U32 x_conn);

NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_Open (NSK2HDI_DEVICE_ID CaSoCGroup_ID,
                                             NSK2HDI_HANDLE    *CaSoCGroup_handle);

NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_Close (NSK2HDI_HANDLE CaSoCGroup_handle);

NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_EnumerateChildren( NSK2HDI_HANDLE       CaSoCGroup_handle,
                                                          NDS_ULONG            *child_device_num,
                                                          NSK2HDI_CHILD_DEVICE *child_device);

NSK2HDI_STATUS NSKHDI_CaSoCGroupDevice_RegisterEnumerationChangeCB (NSK2HDI_HANDLE                   CaSoCGroup_handle,
                                                                    HDINSK2_ENUMERATION_CHANGE_CB_FN *cb_routine,
                                                                    void                             *cookie);

NSK2HDI_STATUS NSKHDI_CmChannelTable_Open (NSK2HDI_DEVICE_ID CmChannelTable_ID,
                                           NSK2HDI_HANDLE    *CmChannelTable_handle);

NSK2HDI_STATUS NSKHDI_CmChannelTable_Close (NSK2HDI_HANDLE CmChannelTable_handle);

NSK2HDI_STATUS NSKHDI_CmChannelTable_EnumerateChildren( NSK2HDI_HANDLE       CmChannelTable_handle,
                                                        NDS_ULONG            *child_device_num,
                                                        NSK2HDI_CHILD_DEVICE *child_device);

NSK2HDI_STATUS NSKHDI_CmChannelGroup_Open (NSK2HDI_DEVICE_ID CmChannelGroup_ID,
                                           NSK2HDI_HANDLE    *CmChannelGroup_handle);

NSK2HDI_STATUS NSKHDI_CmChannelGroup_Close (NSK2HDI_HANDLE CmChannelGroup_handle);

NSK2HDI_STATUS NSKHDI_CmChannelGroup_GetProperties( NSK2HDI_HANDLE  CmChannelGroup_handle,
                                                    NDS_ULONG       request_id,
                                                    NDS_ULONG       *desc_size,
                                                    NDS_UBYTE       *desc);

NSK2HDI_STATUS NSKHDI_CmChannelDevice_Open (NSK2HDI_DEVICE_ID CmChannelGroup_ID,
                                            NSK2HDI_HANDLE    *CmChannele_handle);

NSK2HDI_STATUS NSKHDI_CmChannelDevice_Close (NSK2HDI_HANDLE CmChannele_handle);

NSK2HDI_STATUS NSKHDI_CmChannelDevice_Configure ( NSK2HDI_HANDLE  CmChannele_handle,
                                                  NDS_ULONG       lda_desc_size,
                                                  const NDS_UBYTE *lda_desc,
                                                  NDS_ULONG       esa_desc_size,
                                                  const NDS_UBYTE *esa_desc,
                                                  NDS_ULONG       lsa_desc_size,
                                                  const NDS_UBYTE *lsa_desc);

NSK2HDI_STATUS NSKHDI_CmChannelDevice_WriteTransportKey ( NSK2HDI_HANDLE  CmChannelDevice_handle,
                                                          NDS_ULONG       scb_in_out_size,
                                                          NDS_UBYTE       *scb_in,
                                                          NDS_UBYTE       *scb_out,
                                                          NDS_ULONG       lda_desc_size,
                                                          const NDS_UBYTE *lda_desc,
                                                          NDS_ULONG       esa_desc_size,
                                                          const NDS_UBYTE *esa_desc,
                                                          NDS_ULONG       lsa_desc_size,
                                                          const NDS_UBYTE *lsa_desc);

NSK2HDI_STATUS NSKHDI_CmChannelDevice_InvalidateKeys (NSK2HDI_HANDLE CmChannelDevice_handle);

NSK2HDI_STATUS NSKHDI_CmChannelDevice_GetCookie(NSK2HDI_HANDLE CmChannelDevice_handle,
                                                void **cookie);

NSK2HDI_STATUS NSKHDI_CaPIDProxy_Open (NDS_ULONG      xconn,
                                       NSK2HDI_HANDLE *CaPIDProxy_handle);

NSK2HDI_STATUS NSKHDI_CaPIDProxy_Close (NSK2HDI_HANDLE CaPIDProxy_handle);

NSK2HDI_STATUS NSKHDI_CaPIDProxy_SetPrimaryStreamPID (NSK2HDI_HANDLE CaPIDProxy_handle,
                                                      NDS_USHORT     es_pid);

NSK2HDI_STATUS NSKHDI_CaPIDProxy_SetPrimaryStreamCmChannel (NSK2HDI_HANDLE CaPIDProxy_handle,
                                                            void           *CmChannel_cookie);

NSK2HDI_STATUS NSKHDI_CaPIDProxy_RemovePrimaryStreamCmChannel (NSK2HDI_HANDLE CaPIDProxy_handle);

NSK2HDI_STATUS NSKHDI_CaPIDProxy_SetSecondaryStream (NSK2HDI_HANDLE CaPIDProxy_handle,
                                                     void           *CmChannel_cookie,
                                                     NDS_USHORT     es_pid,
                                                     NDS_ULONG      merge_mode);

NSK2HDI_STATUS NSKHDI_CaPIDProxy_RemoveSecondaryStream (NSK2HDI_HANDLE CaPIDProxy_handle);


NSK2HDI_STATUS NSKHDI_OtpDevice_Open (NSK2HDI_DEVICE_ID   Otp_ID,
                                      NSK2HDI_HANDLE      *Otp_handle);

NSK2HDI_STATUS NSKHDI_OtpDevice_Close (NSK2HDI_HANDLE Otp_handle);

NSK2HDI_STATUS NSKHDI_OtpDevice_GetProperties( NSK2HDI_HANDLE  Otp_handle,
                                               NDS_ULONG       request_id,
                                               NDS_ULONG       *desc_size,
                                               NDS_UBYTE       *desc);

NSK2HDI_STATUS NSKHDI_NskDevice_Open (NSK2HDI_DEVICE_ID   Nsk_ID,
                                      NSK2HDI_HANDLE      *Nsk_handle);

NSK2HDI_STATUS NSKHDI_NskDevice_Close (NSK2HDI_HANDLE Nsk_handle);

NSK2HDI_STATUS NSKHDI_NskDevice_Read8 (NSK2HDI_HANDLE      Nsk_handle,
                                       NDS_ULONG           register_offset,
                                       NDS_ULONG           data_size,
                                       NDS_UBYTE           *data);

NSK2HDI_STATUS NSKHDI_NskDevice_Write8 (NSK2HDI_HANDLE     Nsk_handle,
                                         NDS_ULONG          register_offset,
                                         NDS_ULONG          data_size,
                                         const NDS_UBYTE    *data);

NSK2HDI_STATUS NSKHDI_NskDevice_Read32 (NSK2HDI_HANDLE     Nsk_handle,
                                        NDS_ULONG          register_offset,
                                        NDS_ULONG          data_size,
                                        NDS_ULONG          *data);

NSK2HDI_STATUS NSKHDI_NskDevice_Write32 (NSK2HDI_HANDLE     Nsk_handle,
                                         NDS_ULONG          register_offset,
                                         NDS_ULONG          data_size,
                                         const NDS_ULONG    *data);

NSK2HDI_STATUS NSKHDI_NskDevice_ExecuteCmd (NSK2HDI_HANDLE     Nsk_handle,
                                            NDS_ULONG          command,
                                            NDS_ULONG          control,
                                            NDS_ULONG          register_offset,
                                            NDS_ULONG          data_size,
                                            const NDS_UBYTE    *data);

NSK2HDI_STATUS NSKHDI_NskDevice_RegisterEventCB ( NSK2HDI_HANDLE          Nsk_handle,
                                                  HDINSK2_NSKEVENT_CB_FN  *cb_routine,
                                                  void                    *cookie);

NSK2HDI_STATUS NSKHDI_NskDevice_AcknowledgeKte(NSK2HDI_HANDLE Nsk_handle);

NSK2HDI_STATUS NSKHDI_NskDevice_BasicInitializationComplete( NSK2HDI_HANDLE Nsk_handle, NDS_ULONG stage);

NSK2HDI_STATUS NSKHDI_NskDevice_UpdateNVCounter(NSK2HDI_HANDLE Nsk_handle);

NSK2HDI_STATUS NSKHDI_NskDevice_WriteOTPKey(NSK2HDI_HANDLE     Nsk_handle,
                                            NDS_ULONG          desc_size,
                                            const NDS_UBYTE    *desc);

NSK2HDI_STATUS NSKHDI_NskDevice_SetOneShotGenInBit(NSK2HDI_HANDLE     Nsk_handle,
                                                   NDS_ULONG          bits);

NSK2HDI_STATUS NSKHDI_M2MTable_Open (NSK2HDI_DEVICE_ID M2MTable_ID,
                                     NSK2HDI_HANDLE    *M2MTable_handle);

NSK2HDI_STATUS NSKHDI_M2MTable_Close (NSK2HDI_HANDLE M2MTable_handle);

NSK2HDI_STATUS NSKHDI_M2MTable_EnumerateChildren( NSK2HDI_HANDLE       M2MTable_handle,
                                                  NDS_ULONG            *child_device_num,
                                                  NSK2HDI_CHILD_DEVICE *child_device);


NSK2HDI_STATUS NSKHDI_DMADevice_Open (NSK2HDI_DEVICE_ID DMADevice_ID,
                                      NSK2HDI_HANDLE    *DMADevice_handle);

NSK2HDI_STATUS NSKHDI_DMADevice_Close (NSK2HDI_HANDLE DMADevice_handle);

NSK2HDI_STATUS NSKHDI_DMADevice_GetProperties( NSK2HDI_HANDLE  DMADevice_handle,
                                               NDS_ULONG       request_id,
                                               NDS_ULONG       *desc_size,
                                               NDS_UBYTE       *desc);

NSK2HDI_STATUS NSKHDI_DMADevice_WriteData( NSK2HDI_HANDLE  DMADevice_handle,
                                           void            *M2MChannel_cookie,
                                           NDS_ULONG       desc_size,
                                           const NDS_UBYTE *desc,
                                           NDS_ULONG       data_size,
                                           NDS_UBYTE       *in_data,
                                           NDS_UBYTE       *out_data);


NSK2HDI_STATUS NSKHDI_M2MChannelGroup_Open (NSK2HDI_DEVICE_ID M2MChannelGroup_ID,
                                            NSK2HDI_HANDLE    *M2MChannelGroup_handle);

NSK2HDI_STATUS NSKHDI_M2MChannelGroup_Close (NSK2HDI_HANDLE M2MChannelGroup_handle);

NSK2HDI_STATUS NSKHDI_M2MChannelGroup_GetProperties( NSK2HDI_HANDLE  M2MChannelGroup_handle,
                                                     NDS_ULONG       request_id,
                                                     NDS_ULONG       *desc_size,
                                                     NDS_UBYTE       *desc);

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_Open (NSK2HDI_DEVICE_ID M2MChannelGroup_ID,
                                             NSK2HDI_HANDLE    *M2MChannel_handle);

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_Close (NSK2HDI_HANDLE M2MChannel_handle);

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_GetCookie(NSK2HDI_HANDLE M2MChannele_handle,
                                                 void **cookie);

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_Configure (NSK2HDI_HANDLE  M2MChannele_handle,
                                                  NDS_ULONG       desc_size,
                                                  const NDS_UBYTE *desc);

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_WriteM2MKey(NSK2HDI_HANDLE  M2MChannele_handle,
                                                   NDS_ULONG       desc_size,
                                                   const NDS_UBYTE *desc);

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_NSKWriteM2MKey(NSK2HDI_HANDLE  M2MChannele_handle,
                                                      NDS_ULONG       desc_size,
                                                      const NDS_UBYTE *desc);


MS_U32 NSKHDI_GetHandle(NSK2HDI_HANDLE Handle);

MS_U32 NSKHDI_IVDataTrans(MS_U8 NSK2HDI_Algo, MS_U8 *pu8IV1, MS_U8 *pu8IV2, MS_U32 *pu32HWIV);

MS_U32 NSKHDI_AddDscmbConnect(NDS_ULONG  xconn, NDS_USHORT es_pid, MS_U32     u32FltID);
MS_U32 NSKHDI_RemoveDscmbConnect(NDS_ULONG xconn, NDS_USHORT es_pid);
void NSK2HDI_ParamInit(void);

#endif /*NSK2HDI_HEADER_H_*/


