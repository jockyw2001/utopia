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
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_xc_io.h
// @brief  XC KMD Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_XC_IO_H
#define _MDRV_XC_IO_H

//=============================================================================
// Includs
//=============================================================================
#include "mdrv_xc_st.h"
//=============================================================================
// Defines
//=============================================================================
//IO Ctrl defines:
#define MDRV_XC_IOC_SET_HDR_TYPE_NR                  (0)
#define MDRV_XC_IOC_GET_HDR_TYPE_NR                  (1)
#define MDRV_XC_IOC_SET_INPUT_SOURCE_TYPE_NR         (2)
#define MDRV_XC_IOC_GET_INPUT_SOURCE_TYPE_NR         (3)
#define MDRV_XC_IOC_SET_OPEN_METADATA_INFO_NR        (4)
#define MDRV_XC_IOC_SET_3DLUT_INFO_NR                (5)
#define MDRV_XC_IOC_SET_SHMEM_INFO_NR                (6)
#define MDRV_XC_IOC_ENABLE_HDR_NR                    (7)
#define MDRV_XC_IOC_SET_HDR_WINDOW_NR                (8)
#define MDRV_XC_IOC_CONFIG_AUTO_DOWNLOAD_NR          (9)
#define MDRV_XC_IOC_WRITE_AUTO_DOWNLOAD_NR           (10)
#define MDRV_XC_IOC_FIRE_AUTO_DOWNLOAD_NR            (11)
#define MDRV_XC_IOC_GET_AUTO_DOWNLOAD_CAPS_NR        (12)
#define MDRV_XC_IOC_SET_COLOR_FORMAT_NR              (13)
#define MDRV_XC_IOC_MLOAD_INIT_NR                    (14)
#define MDRV_XC_IOC_MLOAD_ENABLE_NR                  (15)
#define MDRV_XC_IOC_MLOAD_GETSTATUS_NR               (16)
#define MDRV_XC_IOC_MLOAD_WRT_CMD_NR                 (17)
#define MDRV_XC_IOC_MLOAD_FIRE_NR                    (18)
#define MDRV_XC_IOC_MLOAD_WRT_CMD_NONXC_NR           (19)
#define MDRV_XC_IOC_MLOAD_SET_IP_TRIG_P_NR           (20)
#define MDRV_XC_IOC_MLOAD_GET_IP_TRIG_P_NR           (21)
#define MDRV_XC_IOC_MLOAD_SET_TRIG_SYNC_NR           (22)
#define MDRV_XC_IOC_DS_GET_DSFORCE_INDEX_SUPPORT_NR  (23)
#define MDRV_XC_IOC_DS_SET_DSINDEX_SOURCE_SELECT_NR  (24)
#define MDRV_XC_IOC_DS_SET_DSFORCE_INDEX_NR          (25)
#define MDRV_XC_IOC_DS_SET_DYNAMICSCALING_NR         (26)
#define MDRV_XC_IOC_DS_SET_DYNAMICSCALING_FLAG_NR    (27)
#define MDRV_XC_IOC_DS_GET_DS_STATUS_NR              (28)
#define MDRV_XC_IOC_DS_ENABLE_IPM_TUNE_AFTER_DS_NR   (29)
#define MDRV_XC_IOC_DS_WRITE_SWDS_CMD_NR             (30)
#define MDRV_XC_IOC_DS_ADD_NULL_CMD_NR               (31)
#define MDRV_XC_IOC_DS_SET_CLIENT_NR                 (32)
#define MDRV_XC_IOC_DS_GET_SUPPORT_INDEX_NUM_NR      (33)
#define MDRV_XC_IOC_CFDCONCTRL_NR                    (34)
#define MDRV_XC_IOC_DLCCHANGECURVE_NR                (35)
#define MDRV_XC_IOC_DLCONOFFINFO_NR                  (36)
#define MDRV_XC_IOC_DLCBLECHANGECURVE_NR             (37)
#define MDRV_XC_IOC_DLCBLEONOFFINFO_NR               (38)
#define MDRV_XC_IOC_DLCGETHISTOGRAMINFO_NR           (39)
#define MDRV_XC_IOC_DLCSETDLCINITINFO_NR             (40)
#define MDRV_XC_IOC_DLCSETTMOINFO_NR                 (41)
#define MDRV_XC_IOC_DLCSETHDRINFO_NR                 (42)
#define MDRV_XC_IOC_DS_WRITE_SWDS_NONXC_CMD_NR       (43)
#define MDRV_XC_IOC_DS_GET_INDEX_NR                  (44)
#define MDRV_XC_IOC_DS_FIRE_INDEX_NR                 (45)
#define MDRV_XC_IOC_SET_DSHDRINFO_NR                 (46)
#define MDRV_XC_IOC_DS_WRITE_SWDS_CMD_MASK_NR        (47)
#define MDRV_XC_IOC_GET_CFD_SUPPORT_STATUS_NR        (48)
#define MDRV_XC_IOC_SET_SWDR_INFO_NR                 (49)
#define MDRV_XC_IOC_GET_SWDR_INFO_NR                 (50)
#define MDRV_XC_IOC_MAX_NR                           (51)

// use 'x' as magic number
#define MDRV_XC_IOC_MAGIC      ('x')
#define MDRV_XC_IOC_SET_HDR_TYPE                     _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_HDR_TYPE_NR, EN_KDRV_XC_HDR_TYPE)
#define MDRV_XC_IOC_GET_HDR_TYPE                     _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_GET_HDR_TYPE_NR, EN_KDRV_XC_HDR_TYPE)
#define MDRV_XC_IOC_SET_INPUT_SOURCE_TYPE            _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_INPUT_SOURCE_TYPE_NR, EN_KDRV_XC_INPUT_SOURCE_TYPE)
#define MDRV_XC_IOC_GET_INPUT_SOURCE_TYPE            _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_GET_INPUT_SOURCE_TYPE_NR, EN_KDRV_XC_INPUT_SOURCE_TYPE)
#define MDRV_XC_IOC_SET_OPEN_METADATA_INFO           _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_OPEN_METADATA_INFO_NR, ST_KDRV_XC_OPEN_METADATA_INFO)
#define MDRV_XC_IOC_SET_3DLUT_INFO                   _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_3DLUT_INFO_NR, ST_KDRV_XC_3DLUT_INFO)
#define MDRV_XC_IOC_SET_SHMEM_INFO                   _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_SHMEM_INFO_NR, ST_KDRV_XC_SHARE_MEMORY_INFO)
#define MDRV_XC_IOC_ENABLE_HDR                       _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_ENABLE_HDR_NR, MS_BOOL)
#define MDRV_XC_IOC_SET_HDR_WINDOW                   _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_HDR_WINDOW_NR, ST_KDRV_XC_WINDOW_INFO)
#define MDRV_XC_IOC_CONFIG_AUTO_DOWNLOAD             _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_CONFIG_AUTO_DOWNLOAD_NR, ST_KDRV_XC_AUTODOWNLOAD_CONFIG_INFO)
#define MDRV_XC_IOC_WRITE_AUTO_DOWNLOAD              _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_WRITE_AUTO_DOWNLOAD_NR, ST_KDRV_XC_AUTODOWNLOAD_DATA_INFO)
#define MDRV_XC_IOC_FIRE_AUTO_DOWNLOAD               _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_FIRE_AUTO_DOWNLOAD_NR, EN_KDRV_XC_AUTODOWNLOAD_CLIENT)
#define MDRV_XC_IOC_GET_AUTO_DOWNLOAD_CAPS           _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_GET_AUTO_DOWNLOAD_CAPS_NR, ST_KDRV_XC_AUTODOWNLOAD_CLIENT_SUPPORTED_CAPS)
#define MDRV_XC_IOC_SET_COLOR_FORMAT                 _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_COLOR_FORMAT_NR, EN_KDRV_XC_HDR_COLOR_FORMAT)
#define MDRV_XC_IOC_MLOAD_INIT                       _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_INIT_NR, ST_KDRV_MLOAD_INIT)
#define MDRV_XC_IOC_MLOAD_ENABLE                     _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_ENABLE_NR, ST_KDRV_MLOAD_ENABLE)
#define MDRV_XC_IOC_MLOAD_GETSTATUS                  _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_GETSTATUS_NR, ST_KDRV_MLOAD_GETSTATUS)
#define MDRV_XC_IOC_MLOAD_WRT_CMD                    _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_WRT_CMD_NR, ST_KDRV_MLOAD_WRT_CMD)
#define MDRV_XC_IOC_MLOAD_FIRE                       _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_FIRE_NR, ST_KDRV_MLOAD_FIRE)
#define MDRV_XC_IOC_MLOAD_WRT_CMD_NONXC              _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_WRT_CMD_NONXC_NR, ST_KDRV_MLOAD_WRT_CMD_NONXC)
#define MDRV_XC_IOC_MLOAD_SET_IP_TRIG_P              _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_SET_IP_TRIG_P_NR, ST_KDRV_MLOAD_SET_IP_TRIG_P)
#define MDRV_XC_IOC_MLOAD_GET_IP_TRIG_P              _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_GET_IP_TRIG_P_NR, ST_KDRV_MLOAD_GET_IP_TRIG_P)
#define MDRV_XC_IOC_MLOAD_SET_TRIG_SYNC              _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_MLOAD_SET_TRIG_SYNC_NR, EN_MLOAD_TRIG_SYNC)
#define MDRV_XC_IOC_CFDCONCTRL                       _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_CFDCONCTRL_NR, ST_KDRV_XC_CFD_CONTROL_INFO)
#define MDRV_XC_IOC_DLCCHANGECURVE                   _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DLCCHANGECURVE_NR, ST_KDRV_XC_DLC_INFO)
#define MDRV_XC_IOC_DLCONOFFINFO                     _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DLCONOFFINFO_NR, MS_BOOL)
#define MDRV_XC_IOC_DLCBLECHANGECURVE                _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DLCBLECHANGECURVE_NR, ST_KDRV_XC_DLC_BLE_INFO)
#define MDRV_XC_IOC_DLCBLEONOFFINFO                  _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DLCBLEONOFFINFO_NR, MS_BOOL)
#define MDRV_XC_IOC_DLCGETHISTOGRAMINFO              _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DLCGETHISTOGRAMINFO_NR, MS_U16)
#define MDRV_XC_IOC_DLCSETDLCINITINFO                _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DLCSETDLCINITINFO_NR, ST_KDRV_XC_DLC_INIT_INFO)
#define MDRV_XC_IOC_DLCSETTMOINFO                    _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DLCSETTMOINFO_NR, ST_KDRV_XC_TMO_INFO)
#define MDRV_XC_IOC_DS_GET_DSFORCE_INDEX_SUPPORT     _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_GET_DSFORCE_INDEX_SUPPORT_NR, ST_KDRV_DS_GET_DSForceIndexSupported)
#define MDRV_XC_IOC_DS_SET_DSINDEX_SOURCE_SELECT     _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_SET_DSINDEX_SOURCE_SELECT_NR, ST_KDRV_DS_SET_DSIndexSourceSelect)
#define MDRV_XC_IOC_DS_SET_DSFORCE_INDEX             _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_SET_DSFORCE_INDEX_NR, ST_KDRV_DS_SET_DSForceIndex)
#define MDRV_XC_IOC_DS_SET_DYNAMICSCALING            _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_SET_DYNAMICSCALING_NR, ST_KDRV_DS_SetDynamicScaling)
#define MDRV_XC_IOC_DS_SET_DYNAMICSCALING_FLAG       _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_SET_DYNAMICSCALING_FLAG_NR, ST_KDRV_DS_SET_DynamicScalingFlag)
#define MDRV_XC_IOC_DS_GET_DS_STATUS                 _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_GET_DS_STATUS_NR, ST_KDRV_DS_GetDynamicScalingStatus)
#define MDRV_XC_IOC_DS_ENABLE_IPM_TUNE_AFTER_DS      _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_ENABLE_IPM_TUNE_AFTER_DS_NR, ST_KDRV_DS_EnableIPMTuneAfterDS)
#define MDRV_XC_IOC_DS_WRITE_SWDS_CMD                _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_WRITE_SWDS_CMD_NR, ST_KDRV_DS_WriteSWDSCommand)
#define MDRV_XC_IOC_DS_ADD_NULL_CMD                  _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_ADD_NULL_CMD_NR, ST_KDRV_DS_Add_NullCommand)
#define MDRV_XC_IOC_DS_SET_CLIENT                    _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_SET_CLIENT_NR, ST_KDRV_DS_SET_CLIENT)
#define MDRV_XC_IOC_DS_GET_SUPPORT_INDEX_NUM         _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_GET_SUPPORT_INDEX_NUM_NR, ST_KDRV_DS_GET_SUPPORT_INDEX_NUM)
#define MDRV_XC_IOC_DS_WRITE_SWDS_NONXC_CMD          _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_WRITE_SWDS_NONXC_CMD_NR, ST_KDRV_DS_WriteSWDSCommandNonXC)
#define MDRV_XC_IOC_DS_GET_INDEX                     _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_GET_INDEX_NR, ST_KDRV_DS_GET_INDEX_INFO)
#define MDRV_XC_IOC_DS_FIRE_INDEX                    _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_FIRE_INDEX_NR, ST_KDRV_DS_FIRE_INDEX_INFO)
#define MDRV_XC_IOC_SET_DSHDRINFO                    _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_DSHDRINFO_NR, ST_KDRV_XC_DS_HDRInfo)
#define MDRV_XC_IOC_DS_WRITE_SWDS_CMD_MASK           _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_DS_WRITE_SWDS_CMD_MASK_NR, ST_KDRV_DS_WriteSWDSCommand_Mask)
#define MDRV_XC_IOC_GET_CFD_SUPPORT_STATUS           _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_GET_CFD_SUPPORT_STATUS_NR, MS_BOOL)
#define MDRV_XC_IOC_SET_SWDR_INFO                    _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_SET_SWDR_INFO_NR, ST_KDRV_XC_SWDR_INFO)
#define MDRV_XC_IOC_GET_SWDR_INFO                    _IOWR(MDRV_XC_IOC_MAGIC, MDRV_XC_IOC_GET_SWDR_INFO_NR, ST_KDRV_XC_SWDR_INFO)
#endif //_MDRV_XC_IO_H
