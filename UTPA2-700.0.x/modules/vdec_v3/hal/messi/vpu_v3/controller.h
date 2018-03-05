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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

#ifndef _CONTROL_H_
#define _CONTROL_H_

extern void CTL_main( void *pvParameters );
extern void CTL_Init(void);
extern void CTL_Deinit(void);

#define CTL_VERSION         0x16083109
#define CTL_INFO_ADDR         0x0

// _ctl_info statue
#define CTL_STU_NONE         0
#define CTL_STU_INIT         1
#define CTL_STU_TASK         2

// _ctl_info task_statue[x]
#define CTL_TASK_NONE       0
#define CTL_TASK_CREATE     1  // task has already created by controller
#define CTL_TASK_CMDRDY     2  // task has already inited and ready to get command
#define CTL_TASK_TO_BE_DELETED      3  // task is going to be deteled
#define CTL_TASK_CMD                4

#define VDEC_TAG  0xFE
#define MVD_DECODER 1
#define HVD_DECODER 2

// _ctl_info task_mode
#define CTL_MODE_NORMAL                0
#define CTL_MODE_3DWMV                 1  // 3d wmv
#define CTL_MODE_3DTV                  2  // mpeg2+h.264
#define CTL_MODE_3DTV_PROG             3  // Korea 3DTV forced progressive mode
#define CTL_MODE_ONE_STC               4  // only one STC, sub view sync main stc
#define CTL_MODE_SWITCH_STC            5  // switch target STC , main view sync sub stc and  sub view sync main stc
#define CTL_MODE_3DTV_TWO_PITCH        6  //Korea 3DTV, 2nd pitch enabled for 3DLR
#define CTL_MODE_3DTV_PROG_TWO_PITCH   7  // Korea 3DTV PROG, 2nd pitch enabled for 3DLR
#define CTL_MODE_SEC_MCU               8

#ifdef VDEC3
#define MAX_TASKS 2 // max tasks number
#define GLOBAL_NAL_TABLE_BASE_ADDR 0xA4000
#define EVD_HVD_BBU_NAL_TABLE_SZIE 0x2000
#define NAL_TBL_PRTITION_NUM 2 // This definition should be the same as bbu number
#else
#define MAX_TASKS 2 // max tasks number
#endif

#define VDEC_FW30

//fw allocate dram
#if defined(SUPPORT_NEW_MEM_LAYOUT)
#define DRAM_OFFSET 0x20000     // starts from 0xB0000+0x20000
#elif (SUPPORT_EVD) // Local FPGA verification
#define DRAM_START 0x100000
#else  //For HVD and MVD, or real chip verification
#define DRAM_START 0xA0000
#endif // #if defined(SUPPORT_NEW_MEM_LAYOUT)

#if defined(SUPPORT_NEW_MEM_LAYOUT)
  #if defined(SUPPORT_EVD) && (SUPPORT_EVD == 1)
  #define HEAP_START 0xE0000
  #else
  #define HEAP_START 0xC0000
  #endif
#else
  #define HEAP_START 0xA0000
#endif

#define VSYNC_BRIDGE_OFFSET 0x1FA00
#define VSYNC_BRIDGE_EXT_OFFSET 0x20000

#if defined(SUPPORT_VDEC_STR)
/*
    | STR_FLAG : 16byte | CTL_CMD : 15 set | MAIN_CMD : 120 set | SUB_CMD : 120 set |

    1 set = 16 byte
    total str buffer ~ 4k
*/

#define VDEC_STR_ALIGN  16
#define VDEC_STR_CTL_CMD_RESERVERD  8
#define VDEC_STR_CMD_RESERVERD 120

#if 0
#define VDEC_STR_BUFFER_START      0x2B0000
#define VDEC_STR_MAIN_CTL_CMD_BUF  (VDEC_STR_BUFFER_START + VDEC_STR_ALIGN)
#define VDEC_STR_SUB_CTL_CMD_BUF   (VDEC_STR_MAIN_CTL_CMD_BUF + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_MAIN_CMD_BUF      (VDEC_STR_SUB_CTL_CMD_BUF  + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_SUB_CMD_BUF      (VDEC_STR_MAIN_CMD_BUF + (VDEC_STR_ALIGN * VDEC_STR_CMD_RESERVERD))

#define VDEC_STR_MAIN_WORK         VDEC_STR_BUFFER_START
#define VDEC_STR_SUB_WORK          VDEC_STR_BUFFER_START+0x1
#define VDEC_STR_MAIN_RESUME       VDEC_STR_BUFFER_START+0x2
#define VDEC_STR_SUB_RESUME        VDEC_STR_BUFFER_START+0x3
#define VDEC_STR_MAIN_CTL_CMD_COUNT    VDEC_STR_BUFFER_START+0x4
#define VDEC_STR_SUB_CTL_CMD_COUNT     VDEC_STR_BUFFER_START+0x5
#define VDEC_STR_MAIN_CMD_COUNT        VDEC_STR_BUFFER_START+0x6
#define VDEC_STR_SUB_CMD_COUNT         VDEC_STR_BUFFER_START+0x8  //0x7 for VDEC_UNMUTE_BYTE
#else

#define VDEC_STR_BUFFER_DUAL_OFFSET     0x2B0000
#define VDEC_STR_BUFFER_SINGLE_OFFSET     0x1D0000

#define VDEC_STR_MAIN_CTL_CMD_OFFSET  (VDEC_STR_ALIGN)
#define VDEC_STR_SUB_CTL_CMD_OFFSET   (VDEC_STR_MAIN_CTL_CMD_OFFSET + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_MAIN_CMD_OFFSET      (VDEC_STR_SUB_CTL_CMD_OFFSET  + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_SUB_CMD_OFFSET      (VDEC_STR_MAIN_CMD_OFFSET + (VDEC_STR_ALIGN * VDEC_STR_CMD_RESERVERD))

#define VDEC_STR_MAIN_WORK_OFFSET         0x0
#define VDEC_STR_SUB_WORK_OFFSET          0x1
#define VDEC_STR_MAIN_RESUME_OFFSET       0x2
#define VDEC_STR_SUB_RESUME_OFFSET        0x3
#define VDEC_STR_MAIN_CTL_CMD_COUNT_OFFSET    0x4
#define VDEC_STR_SUB_CTL_CMD_COUNT_OFFSET     0x5
#define VDEC_STR_MAIN_CMD_COUNT_OFFSET        0x6
#define VDEC_STR_SUB_CMD_COUNT_OFFSET         0x8  //0x7 for VDEC_UNMUTE_BYTE

#endif



#define VDEC_STR_CMD     4
#define VDEC_STR_ARG0    8
#define VDEC_STR_ARG1    9
#define VDEC_STR_ARG2    10
#define VDEC_STR_ARG3    11
#define VDEC_STR_ARG4    12
#define VDEC_STR_ARG5    13

#define VDEC_STR_MVD 1
#define VDEC_STR_HVD 2

#define VDEC_UNMUTE_BYTE  7

#endif
/* Structure defination */
struct _ctl_info {
    const unsigned int readonly[4];       // CTL_INFO_ADDR + 0x00 read only for tag.
    unsigned int vpu_clk;                 // CTL_INFO_ADDR + 0x10 reserved for driver to fw message.(VDEC CPU clock)
    unsigned int ctl_interface;           // CTL_INFO_ADDR + 0x14 driver interface(read only)
    unsigned int heap_size[MAX_TASKS];    // CTL_INFO_ADDR + 0x18 heap size available for each task
    unsigned int verion;                  // CTL_INFO_ADDR + 0x20
    unsigned int statue;                  // CTL_INFO_ADDR + 0x24
    unsigned int last_ctl_cmd;            // CTL_INFO_ADDR + 0x28
    unsigned int last_ctl_arg;            // CTL_INFO_ADDR + 0x2C
    unsigned int task_statue[MAX_TASKS];  // CTL_INFO_ADDR + 0x30 fixed to 4 elements for alignment
    unsigned int task_single;             // CTL_INFO_ADDR + 0x40
    unsigned short task_mode[MAX_TASKS];  // CTL_INFO_ADDR + 0x44 0:normal 1:3d WMV 2:korea 3d TV
    unsigned int burst_mode;              // CTL_INFO_ADDR + 0x48 0:normal 1:burst cmd
    unsigned char task_hvd;               // CTL_INFO_ADDR + 0x4c
    unsigned char task_mvd;               // CTL_INFO_ADDR + 0x4d
    unsigned short u16TaskFeature;        // CTL_INFO_ADDR + 0x4e
    unsigned int u32Reserved;             // CTL_INFO_ADDR + 0x50 reserved
    unsigned int u32TaskShareInfoAddr[4]; // CTL_INFO_ADDR + 0x54 offset from FW beginning
    unsigned int u32VsyncBridgeAddr;      // CTL_INFO_ADDR + 0x64
    unsigned int FB_ADDRESS;              // CTL_INFO_ADDR + 0x68 , this value is offset of miu, unit is byte
    unsigned int FB_Total_SIZE;           // CTL_INFO_ADDR + 0x6C , unit is byte
    unsigned int FB_Used_SIZE;            // CTL_INFO_ADDR + 0x70 , unit is byte
#ifdef VDEC3
    unsigned int nal_tbl_hvd;
    unsigned int nal_tbl_evd;
    unsigned int nal_tbl0_hvd_vp8;
    unsigned int nal_tbl1_hvd_vp8;
#endif
#ifdef VDEC3_FB
    unsigned int u32FrameBufAddr;         // frame buffer base address
    unsigned int u32FrameBufSize;         // frame buffer size for all tasks
    unsigned char u8FrameBufSegment;      // select one enumeration from Split_FB
    unsigned char bFrameBufUsed[4];       // record if each segment is used.
#endif
    unsigned char u8UseIMITaskId; //indicate which task is using IMI
    unsigned char u8HicodecType;  //Kano, 0:Hicodec 1:Hicodec_Lite
    unsigned char reserved;
    unsigned int  u32VsyncBridgeAddrExt;  // record the vsync bridge extension address
    unsigned int  u32DolbyVisionXCShmAddr;   // record the dolby vision XC share memory address for transfer DM/composer
    unsigned char STCindex[MAX_TASKS];
    unsigned char STCMode[MAX_TASKS];
    unsigned char task_evd;
#if defined(SUPPORT_VDEC_STR)
    unsigned int  u32StrAddrOffset;
#endif

} ;

#ifdef VDEC3
#define VDEC_BBU_ID_MASK     0xFF000000
#define VDEC_BBU_ID_SHIFT            24

#define MAX_VDEC_VBBU_ENTRY_COUNT 254

typedef struct
{
    unsigned int u32Offset;             ///< Packet offset from bitstream buffer base address. unit: byte.
    unsigned int u32Length;             ///< Packet size. unit: byte. ==> Move _VDEC_EX_ReparseVP8Packet to FW
    unsigned long long u64TimeStamp;    ///< Packet time stamp.
    unsigned int u32ID_L;               ///< Packet ID low part.
    unsigned int u32ID_H;               ///< Packet ID high part.
    unsigned char u8Reserved[8];        ///< Revserved space and for 16-byte alignment
} VDEC_VBBU_Entry;

typedef struct
{
    unsigned int u32WrPtr;
    unsigned int u32RdPtr;
    unsigned char u8Reserved[8];
    VDEC_VBBU_Entry stEntry[MAX_VDEC_VBBU_ENTRY_COUNT + 1];
} VDEC_VBBU;

#endif

typedef struct
{
    unsigned int u32HVD_PENDING_RELEASE_ST_ADDR;//[0][0]:0x0D98 [0][1]:0x0DA4 [1][0]:0x0DB0 [1][1]:0x0DBC
    unsigned int u32HVD_PENDING_RELEASE_SIZE;   //[0][0]:0x0D9C [0][1]:0x0DA8 [1][0]:0x0DB4 [1][1]:0x0DC0
    unsigned int u32HVD_COLLISION_NUM;          //[0][0]:0x0DA0 [0][1]:0x0DAC [1][0]:0x0DB8 [1][1]:0x0DC4
} PENDING_RELEASE_QUEUE;

typedef struct
{
    unsigned int u32HVD_DISPCMDQ_DRAM_ST_ADDR;//0x0FA8 // for VDEC3 display command queue usage
    unsigned int u32HVD_STREAM_DISPCMDQ_RD;   //0x0FAC // stream display command queue read ptr for VDEC3 display command queue usage
    unsigned int u32HVD_STREAM_DISPCMDQ_WD;   //0x0FB0 // stream display command queue write ptr for VDEC3 display command queue usage
    unsigned int u32HVD_CMDQ_DRAM_ST_ADDR;    //0x0FB4 // for VDEC3 dram command queue usage
    unsigned int u32HVD_STREAM_CMDQ_RD;       //0x0FB8 // stream command queue read ptr for VDEC3 dram command queue usage
    unsigned int u32HVD_STREAM_CMDQ_WD;       //0x0FBC // stream command queue write ptr for VDEC3 dram command queue usage
} CMD_QUEUE;

extern struct _ctl_info *g_ctl_ptr;
extern volatile char g_ctl_Version[] __attribute__ ((section(".tail_version"), aligned(16)));


#endif // _CONTROL_H_

