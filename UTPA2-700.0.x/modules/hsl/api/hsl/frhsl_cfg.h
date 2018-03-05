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
/***************************************************************************
 *
 * Copyright ?2008-2010 MStar Semiconductor, Inc. All Rights Reserved
 *
 **************************************************************************/
/** \file
 * \brief This is the header file to configure the FrHsl module.
 * This file contains default definitions of platform-specific macros to
 * customise the HSL hardware, interface and logging routines.  All macros
 * defined here can be overridden/'overloaded' in a platform-specific header file.
 */
#if defined (MSOS_TYPE_LINUX)
#ifndef FRHSLCFG_H
#define FRHSLCFG_H


#include "MsTypes.h"
#include <unistd.h>
#include <pthread.h>

#include <dlfcn.h>


#ifdef  __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Constants
*******************************************************************************/

#define ENABLE_ALL_GROUP_IDS            1
#define DISABLE_ALL_GROUP_IDS           0

/*******************************************************************************
** Macros
*******************************************************************************/

/** Macro that can be redefined to allow code to be added at the
 *  start of the HSL flow control interrupt.
 *  Macro that can be redefined to allow code to be added at the
 *  start of the HSL flow control interrupt.
 */
#if !defined (M_FrHslFlowControlIntBegin)
# define M_FrHslFlowControlIntBegin()
#endif

/** Macro that can be redefined to allow code to be added at the
 *  end of the HSL flow control interrupt.
 *  Macro that can be redefined to allow code to be added at the
 *  end of the HSL flow control interrupt.
 */
#if !defined (M_FrHslFlowControlIntEnd)
# define M_FrHslFlowControlIntEnd()
#endif


/*******************************************************************************
* Alternative macros
*******************************************************************************/


/** \def FR_HSL_V2_UNITIALISED_LOG_ID
 *  Log point id assigned when using MDF to capture a new log point.
 *  Log point id assigned when using MDF to capture a new log point.
 *  This value should be used when adding a log point with a new MDF
 *  compatible log point id but using HSL version 2 macros.
 *  e.g. M_FrHslPrintf1(FR_HSL_V2_UNITIALISED_LOG_ID, NEW_MDF_COMPAT_ID,"NewMdfCompaqId: %#.4x", arg1);
 *  NEW_MDF_COMPAT_ID is the new log point added for MDF to be able to decode it.
 */
#define FR_HSL_V2_UNITIALISED_LOG_ID ((MS_U16)0x0000)

/** \def FR_HSL_V2_UNPROCESSED_LOG_ID
 *  Log point id assigned when an HSL version 2 statement has not been processed by silox.exe.
 *  Log point id assigned when an HSL version 2 statement has not been processed by silox.exe.
 *  This ID is used as part of stub functions which will print a default HSL log message when
 *  new HSL log points have been added but have not been processed by silox.exe.
 */
#define FR_HSL_V2_UNPROCESSED_LOG_ID ((MS_U16)0x0002)


// large part of the data are bookmarks (8 * BUFFER_SEGMENTS_COUNT)
#define SYS_STATE_BUFSIZE (1024)

#define TX_BUFFER_SIZE (5*1024)
#define MAX_OUTPUT_THREADS 1
// MS_U8 used to store the next bm number - max count 255
#define BUFFER_SEGMENTS_COUNT 10
#define BUFFER_DUMP_SEGMENTS 3
#define BUFFER_FREE_SPACE_PERCENTAGE 15
#define PENDING_WRITE_LIST_LEN 255
#define ASSERT_RT_CLOCK_FAIL_TW 1000

/*******************************************************************************
** Global Data
*******************************************************************************/
typedef struct ST_HSL_CONTROLLER* p_HslController;

typedef struct ST_LIST_NODE
{
    MS_U32 u32bufferIndex;
    MS_U32 u32timestamp;
    struct ST_LIST_NODE *prev;
    struct ST_LIST_NODE *next;
} stListNode;

typedef struct
{
    MS_U32 address;
    MS_U32 timerWrapDelta;
} stRBbookmark;

// this structure need to save in ring buffer
// after crashing, it can be used to output the remaining message
// make sure the size of this structure MUST less than SYS_STATE_BUFSIZE
typedef struct ST_RB_SYSTEM_STATE
{
    MS_U32 ssize;               // size of system state reserved RAM area
    MS_U32 sbufStart;           // system state RAM start address
    MS_U32 sbufEnd;             // system state RAM address end

    MS_U32  bufSize;            // size of ringbuffer
    MS_U32  bufStart;           // ring buffer start address
    MS_U32  bufEnd;             // ring buffer end address

    // runtime constants
    MS_U32  bufMinFreeBytes;     // defines a threshold for triggering data dumping
    MS_U32  bufSegmentSize;

    // access to the following is protected by the bufferMutex
    MS_U32  bufUsedEnd;         // end of the currently used buffer space (adjusted
    MS_U32  bufWritePos;        // current write position in ring buffer
    MS_U32  bufReadPos;         // position of output pointer
    MS_U32  bufReadLimit;       // last byte of valid fully written data update after write finishes
    MS_U32  bufFlags;
    MS_U32  hslLastTwTime;      // timestamp of the last timerwrap

    // controlled by outputCondMutex
    MS_U32  bufBytesToSend;     // amount of data to be transfered from the ring buffer
    stRBbookmark bufBookmarks[BUFFER_SEGMENTS_COUNT];
    MS_U8    dumpFlag;            // marks a necessity to insert a log point informing about a data dump into the output stream.
    MS_U8   syncMarker;         // countdown for writing sync marker
    MS_U8   nextBookmark;
    MS_U8    flushOnConnect;        // indicates whether the complete buffer contents should be flushed on connection
    MS_U8    dynamicAlloc;        // indicates the type of allocation and a necessity to release memory on exit. (0 static/1 dynamic)
} stSystemState;

// manages the data associated with a specific thread instance.
typedef void * (*start_routine)(void *);

typedef enum
{
    read_thread=0,
    write_thread,
    output_thread,
    prMS_S32_thread
}THREADTYPE;

typedef struct
{
    MS_S32          streamId;
    pthread_t       threadId;
    MS_S32          priority;
    THREADTYPE  typeId;
    start_routine   funcPtr;
    void*               funcParam; // ptr to array of bytes to log.
} hslThread;


typedef struct ST_HSL_CONTROLLER
{
    // system state pointer
    stSystemState* pSystemState;                // location of structure for system state
    MS_U32 u32Port;  // port on which connections will be accepted
    MS_U8 u8Instance;
    MS_BOOL bInit;

    // Global variables
    //TODO!: Initialize output threads
    hslThread logThreads[MAX_OUTPUT_THREADS];
/*    =
    {
            {0, 0, 0, output_thread, bufferWriteThread, NULL}
    }; */

    stListNode pendingWriteList[PENDING_WRITE_LIST_LEN];
    stListNode *pendingWriteHead; //TODO: initialize pointers with zeros
    MS_U16 pendingWriteLength;

    volatile MS_BOOL thread_finalize;

    pthread_mutex_t  bufferMutex;            // mutex for accessing the ring buffer
    pthread_mutex_t  inputMutex;             // mutex synchronising the input threads
    pthread_mutex_t  outputCondMutex;        // mutex for I/O sync
    pthread_cond_t   outputCond;             // ring buffer output sync condition

    pthread_mutex_t pendingWriteMutex;       // mutex for the pending write linked list;

    pthread_t threadCmdRecvStart;
    pthread_mutex_t socketReadMutex;   // mutex variable for socket read

    MS_BOOL g_AutoTestRun;
    int hServerSocket, hClientSocket;

} stHslController;

/*******************************************************************************
** Global Function Prototypes
*******************************************************************************/

void FrHslResetConnection(stHslController* const _hsl);
MS_BOOL FrHslInitialiseBegin(void);
MS_BOOL FrHslInitialiseEnd(void);
MS_BOOL FrHslStop(void);
void FrHslWritePayload (const MS_BOOL bLast, const MS_U16 u16Payload, MS_U8 *pu8Buff);
void FrHslWriteHeader(const MS_U8 u8MsgType, const MS_U16 u16ID, MS_U8 *pu8Buff, MS_U32 u32Time);
MS_BOOL FrHslCommand(p_HslController _hsl, CmdInfo* const pCmd);

#define HSL_MAX_LIBRARY_COUNT 50
#define HSL_MAX_THREAD_COUNT 200
#define HSL_MAX_THREAD_NAME 17
#define HSL_MAX_LIBRARY_NAME 100

typedef struct LibraryShortInfoTag
{
    MS_U16 rec_no;
    MS_U16 rec_total;
    void*  base_address;
    void*  on_load_addr;
    char   name[HSL_MAX_LIBRARY_NAME];
} ST_LIBRARY_SHORT_INFO;
extern ST_LIBRARY_SHORT_INFO gDynamicLibraryInfo[HSL_MAX_LIBRARY_COUNT];
extern MS_U16 gDynamicLibraryCount;

typedef struct ThreadInfoTag
{
    MS_U32  thread_ID;
    char   thread_name[HSL_MAX_THREAD_NAME];
} ST_THREAD_INFO;
extern ST_THREAD_INFO gThreadInfo[HSL_MAX_THREAD_COUNT];
extern MS_U16 gThreadCount;

#define MAX_SELFTEST_THREAD_CNT 20
extern pthread_t gTestThreads[MAX_SELFTEST_THREAD_CNT];
extern MS_BOOL gRunLoadTest;
void *HslSelfTestThreadFun(void* pArg);

#ifdef  __cplusplus
}
#endif

#endif
/* END OF FILE */
#endif
