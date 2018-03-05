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
/*******************************************************************************
    Shared library to log data to a circular buffer and output the data to
    multiple devices.
 *******************************************************************************/
#if defined (MSOS_TYPE_LINUX)
#include <stdio.h>

#include <pthread.h>

#include <errno.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "frhsl_datalink.h"
#include "frhsl_ringbuffer.h"
#include "frhsl_cfg.h"
#include "apiHSL.h"
#include "frhsl_RAWLOB.h"
#include "MsCommon.h"
#include "drvWDT.h"

// large part of the data are bookmarks (8 * BUFFER_SEGMENTS_COUNT)
#define SYS_STATE_BUFSIZE (1024)


#define DUMP_LP_LEN 41
static MS_U8 s_DumpBufferLP[] =         {0x17,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x12, 0x03, 0xFF, 0x00, 0x00, 0x11, 0x02, 0x00, 0x11, 0x3B, 0xD0, 0x10, 0x00, 0x00, 0x17};
static MS_U8 s_CriticalDumpBufferLP[] = {0x17,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x13, 0x0F, 0x09, 0x00, 0x00,
        0x12, 0x03, 0xFF, 0x00, 0x00, 0x11, 0x02, 0x00, 0x11, 0x59, 0x86, 0x10, 0x00, 0x00, 0x17};

ST_LIBRARY_SHORT_INFO gDynamicLibraryInfo[HSL_MAX_LIBRARY_COUNT];
MS_U16 gDynamicLibraryCount = 0;
ST_THREAD_INFO gThreadInfo[HSL_MAX_THREAD_COUNT];
MS_U16 gThreadCount = 0;

pthread_t gTestThreads[MAX_SELFTEST_THREAD_CNT];
MS_BOOL gRunLoadTest = true;

void *HslSelfTestThreadFun(void* pArg)
{
    struct timeval timeStart, timeNow;
    int i = 0;
    ST_HSL_SELFTEST_CMD command;
    ST_HSL_SELFTEST_CMD* pCommand = &command;
    memcpy(pCommand, pArg, sizeof(ST_HSL_SELFTEST_CMD));

    gettimeofday(&timeStart, 0);
    timeNow = timeStart;

    while( gRunLoadTest &&
            ((timeNow.tv_sec-timeStart.tv_sec)*1000000 + (timeNow.tv_usec-timeStart.tv_usec) < (pCommand->mRunTime*1000000)) )
    {
        for(i=0; i < pCommand->mCntSimpleLP; i++)
        {
            M_FrHslPrintf0 (0x4890, HSL_DBG_SN_ALL_DEBUG, "HSL SelfTest printf 0 arg:");
        }
        for(i=0; i < pCommand->mCntPrintf1p; i++)
        {
            M_FrHslPrintf1 (0x37D05, HSL_DBG_SN_ALL_DEBUG, "HSL SelfTest printf 1 arg: %X", 0xFF);
        }
        for(i=0; i < pCommand->mCntArray16; i++)
        {
            MS_U16 sndArray[pCommand->mParamArray16Size];
            memset(sndArray, 0x16, sizeof(sndArray));
            M_FrHslArray16 (0xAD99, HSL_DBG_SN_ALL_DEBUG, "HSL SelfTest array16: %X", pCommand->mParamArray16Size, sndArray);
        }
        for(i=0; i< pCommand->mCntArray32; i++)
        {
            MS_U32 sndArray[pCommand->mParamArray32Size];
            memset(sndArray, 0x32, sizeof(sndArray));
            M_FrHslArray32 (0xAB54, HSL_DBG_SN_ALL_DEBUG, "HSL SelfTest array32: %X", pCommand->mParamArray32Size, sndArray, 0);
        }
        for(i=0; i < pCommand->mCntTypedef; i++)
        {
            ST_TEST_STRUCT testStruct;
            testStruct.a = 0x1234;
            testStruct.b = 0x5678;
            testStruct.c = 0xABCD;
            M_FrHslTypedef (0xA4A4,  HSL_DBG_SN_ALL_DEBUG, "HSL SelfTest typedef: %d", ST_TEST_STRUCT, &testStruct );
        }
        usleep(pCommand->mThreadDelay*1000); // convert the parameter from usec into msec
        gettimeofday(&timeNow, 0);
    }
    return 0;
}

const MS_U8 syncBuf[] = {HSL_RAWLOB_SYNC_MARKER};

MS_U32 GetSystemTime(void)
{
	return MDrv_TIMER_GetCounter (E_TIMER_1);
}

// PENDING WRITE LINKED LIST OPERATIONS

/* Should only be called from the listAppendValue */
stListNode* listFindNextFreeNode( stHslController *_hsl )
{
    stListNode *retVal = 0;
    if( _hsl != 0)
    {
        int i = 0;
        if( _hsl->pendingWriteHead == 0 )
        {
            retVal = &(_hsl->pendingWriteList[0]);
        }
        else
        {
            for( i=0; (i<PENDING_WRITE_LIST_LEN) && (_hsl->pendingWriteList[i].u32bufferIndex || _hsl->pendingWriteList[i].u32timestamp); i++ ){};
            retVal = &(_hsl->pendingWriteList[i]);
        }
    }
    return retVal;
}

stListNode* listAppendValue(stHslController *_hsl, MS_U32 newValue)
{
    stListNode *currentNode = 0;
    stListNode *lastNode = 0;
    if(_hsl!=0)
    {
        pthread_mutex_lock( &(_hsl->pendingWriteMutex) );
        //assert(_hsl->pendingWriteLength < PENDING_WRITE_LIST_LEN );
        currentNode = listFindNextFreeNode(_hsl);
        assert( currentNode != 0 );
        currentNode->u32bufferIndex = newValue;
        if(_hsl->pendingWriteHead == 0 )
        {
            _hsl->pendingWriteHead = currentNode;
            currentNode->prev = 0;
        }
        else
        {
        // find the tail
            lastNode = _hsl->pendingWriteHead;
            while(lastNode->next)
            {
                lastNode = lastNode->next;
            }
            lastNode->next = currentNode;
            currentNode->prev = lastNode;
        }
        currentNode->next = 0;
        _hsl->pendingWriteLength++;
        pthread_mutex_unlock( &(_hsl->pendingWriteMutex) );
    }
    return currentNode;
}

void listRemoveNode(stHslController *_hsl, stListNode *node)
{
    if(_hsl!=0)
    {
        pthread_mutex_lock( &(_hsl->pendingWriteMutex) );
        //assert( _hsl->pendingWriteHead != 0 );

        if(node == _hsl->pendingWriteHead)
        {
            _hsl->pendingWriteHead = node->next;
        }

        if(node->prev)
        {
        	node->prev->next = node->next;
        }
        if(node->next)
        {
        	node->next->prev = node->prev;
        }
        node->u32bufferIndex = 0;
        node->u32timestamp = 0;
        node->prev = 0;
        node->next = 0;
        _hsl->pendingWriteLength--;
        pthread_mutex_unlock( &(_hsl->pendingWriteMutex) );
    }
}

MS_U32 listGetFirstValue( stHslController *_hsl )
{
    MS_U32 retVal = 0;
    if(_hsl!=0)
    {
        retVal = 0;
        pthread_mutex_lock( &(_hsl->pendingWriteMutex) );
        if( _hsl->pendingWriteHead == 0 )
        {
            retVal = 0;
        }
        else
        {
            retVal = _hsl->pendingWriteHead->u32bufferIndex;
        }
        pthread_mutex_unlock( &(_hsl->pendingWriteMutex) );
    }
    return retVal;
}

MS_BOOL listIsEmpty(stHslController *_hsl)
{
    MS_BOOL retVal = true;
    if(_hsl!=0)
    {
    pthread_mutex_lock( &(_hsl->pendingWriteMutex) );
    retVal = (_hsl->pendingWriteHead == 0);
    pthread_mutex_unlock( &(_hsl->pendingWriteMutex) );
    }
    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//    GENERAL MESSAGE PROCESSING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    Function:
        sendBytesToStream
    Description:
        Output bytes of data from a message in the buffer, only the bytes less than outputLen will be output
    INPUT:
        MS_U8*: output buffer
        MS_U32: total bytes need to output
    OUTPUT:
        Data to a device - currently UART.
    RETURNS:
        MS_BOOL: the whole bytes in message was written out or not
 *******************************************************************************/
MS_BOOL bufferSendBytes(p_HslController _hsl, const MS_U8* const pu8MessageBuffer, const MS_U32 u32totalBytes)
{
    PUTCS(pu8MessageBuffer, u32totalBytes, FR_HSL_FILE_DESCRIPTOR, _hsl);
    return FALSE;
}

/*******************************************************************************
    Function:
            bufferWriteThread
    Description:
            output Data to UART
 *******************************************************************************/
void* bufferWriteThread(void* data)
{
    //TODO!: Write a new simplified ring buffer output procedure
    struct timeval now;
    struct timespec timeout;
    MS_U32 txSize;
    MS_U32 txPos;
    MS_U8  txBuffer[TX_BUFFER_SIZE];
    MS_U32 txBufPos;
    MS_U32 dumpReadPos;
    MS_U32 dumpInfoPos=0;
    MS_U8* dumpInfoBuf=NULL;
    MS_BOOL resetBufEnd = FALSE;

    p_HslController _hsl = (p_HslController)data;

    pthread_mutex_lock(&(_hsl->outputCondMutex));
    while( !(_hsl->thread_finalize) )
    {

        gettimeofday(&now, 0);
        timeout.tv_sec = now.tv_sec + 1;
        timeout.tv_nsec = now.tv_usec;

        /* Checks to the pthread_cond_timedwait return immediately with an exception of the ETIMEDOUT
         * Any other check will lose the blocking behaviour and will result in a tight loop executing
         * the default case (add timerwraps) with high frequency and consuming lots of system's resources.
         * Please see 'man pthread_cond_timedwait' for details.
         */

        if( (_hsl->pSystemState->bufFlags != BUFFER_FLAG_NONE) ||
                (ETIMEDOUT != pthread_cond_timedwait( &(_hsl->outputCond), &(_hsl->outputCondMutex), &timeout )) )
        {
            if( _hsl->pSystemState->bufFlags & BUFFER_FLAG_EMERGENCY_DUMP )
            {
                pthread_cond_wait(&(_hsl->outputCond), &(_hsl->outputCondMutex));
            }

            if( _hsl->pSystemState->bufFlags & BUFFER_FLAG_TX )
            {
               /* Additional info about the usage of bufReadPos below:   what coverity misses is that:
                -   Every write access to this variable is protected by holding both of the: bufferMutex and outputCondMutex
                -   Every read is protected by holding at least one of them.
                This provides enough locking to ensure that the value is always consistent between threads yet provides enough
                freedom for multiple threads to operate concurrently if they only require reading. */
                txPos = _hsl->pSystemState->bufReadPos;
                if(_hsl->pSystemState->dumpFlag != BUFFER_FLAG_NONE)
                {
                    switch(_hsl->pSystemState->dumpFlag)
                    {
                    case BUFFER_FLAG_DUMP:
                        dumpInfoPos = DUMP_LP_LEN;
                        dumpInfoBuf = s_DumpBufferLP;
                        break;
                    case BUFFER_FLAG_EMERGENCY_DUMP:
                        dumpInfoPos = DUMP_LP_LEN;
                        dumpInfoBuf = s_CriticalDumpBufferLP;
                        break;
                    default:
                        dumpInfoPos = 0;
                        dumpInfoBuf = NULL;
                        break;
                    }
                }
                while( _hsl->pSystemState->bufBytesToSend > 0 )
                {
                    txBufPos = 0;
                    while((txBufPos < TX_BUFFER_SIZE-1) && (_hsl->pSystemState->bufBytesToSend > 0))
                    {
                        // is it time to send a sync marker?
                        if( _hsl->pSystemState->syncMarker == 0 )
                        {
                            txBuffer[txBufPos] = HSL_RAWLOB_SYNC_MARKER;
                            txBufPos++;
                            --(_hsl->pSystemState->bufBytesToSend);
                            _hsl->pSystemState->syncMarker = HSL_RAWLOB_SYNC_MARKER_INTERVAL;
                        }

                        // does the remaining data fill the space to the nearest sync marker?
                        if( _hsl->pSystemState->bufBytesToSend >= _hsl->pSystemState->syncMarker )
                        {
                            txSize = _hsl->pSystemState->syncMarker;
                        }
                        else
                        {
                            txSize = _hsl->pSystemState->bufBytesToSend;
                        }


                        if(txSize > 0)
                        {
                            // need to inject a data dump information logpoint?
                            if(dumpInfoPos > 0)
                            {
                                if(txSize > dumpInfoPos)
                                {
                                    txSize = dumpInfoPos;
                                }

                                dumpReadPos = DUMP_LP_LEN-dumpInfoPos;

                                if( dumpReadPos + txSize <= DUMP_LP_LEN )
                                {
                                    memcpy(txBuffer+txBufPos, dumpInfoBuf+dumpReadPos, txSize);
                                }

                                dumpInfoPos -= txSize;

                            }
                            else
                            {
                                if( (txPos + txSize) >= _hsl->pSystemState->bufUsedEnd )
                                {
                                    txSize = _hsl->pSystemState->bufUsedEnd - txPos;
                                }

                                if( txBufPos + txSize >= TX_BUFFER_SIZE )
                                {
                                    txSize = TX_BUFFER_SIZE - txBufPos;
                                }

                                // data TX
                                memcpy(txBuffer+txBufPos, (MS_U8*)txPos, txSize);

                                txPos += txSize;
                                /* IMPORTANT !!!
                                 * The txPos (and effectively the bufReadPos) can only wrap around if the bufReadLimit has
                                 * wrapped around previously. This is to avoid a corner case when the read limit sits right at the end
                                 * of the buffer and after catching up with it, the read pos is updated to the beginning of the buffer
                                 * resulting in the complete OLD buffer contents to be sent again.
                                 */
                                if( (txPos >= _hsl->pSystemState->bufUsedEnd) && (_hsl->pSystemState->bufReadLimit < _hsl->pSystemState->bufReadPos) )
                                {
                                    // mark the buffer's end for restoration once we've read past the previous virtual one
                                    resetBufEnd = TRUE;
                                    txPos = _hsl->pSystemState->bufStart;
                                }
                            }
                            // update tx markers
                            _hsl->pSystemState->syncMarker = (_hsl->pSystemState->syncMarker - txSize);
                            _hsl->pSystemState->bufBytesToSend -= txSize;
                            txBufPos += txSize;
                        }
                    }
                    bufferSendBytes(_hsl, txBuffer, txBufPos);
                }

                // data successfully sent, update read position.
                pthread_mutex_lock(&(_hsl->bufferMutex));
                _hsl->pSystemState->bufReadPos = txPos;
                _hsl->pSystemState->bufFlags &= ~BUFFER_FLAG_TX;
                _hsl->pSystemState->dumpFlag = BUFFER_FLAG_NONE;
                if( resetBufEnd )
                {
                    _hsl->pSystemState->bufUsedEnd = _hsl->pSystemState->bufEnd;
                    resetBufEnd = FALSE;
                }
                pthread_mutex_unlock(&(_hsl->bufferMutex));
            }
            if( _hsl->pSystemState->bufFlags & BUFFER_FLAG_DUMP )
            {
                // dump data
                pthread_mutex_lock(&(_hsl->bufferMutex));
                _hsl->pSystemState->dumpFlag = BUFFER_FLAG_DUMP;
                bufferDumpOldestDataSegment(_hsl);
                _hsl->pSystemState->bufFlags &= ~BUFFER_FLAG_DUMP;
                pthread_mutex_unlock(&(_hsl->bufferMutex));
            }
        }
        else
        {
            // no data to be allocated but it will add some timer wraps
            pthread_mutex_unlock(&(_hsl->outputCondMutex));
            if( bufferAllocateWrite(_hsl, 0,0) )
            {
                // coverity thinks that the return value should be checked....
                continue;
            }
            pthread_mutex_lock(&(_hsl->outputCondMutex));
        }
    }
    pthread_mutex_unlock(&(_hsl->outputCondMutex));
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////
//    THREAD FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    Function:
    Description:
    INPUT:
    OUTPUT:
 *******************************************************************************/
MS_BOOL deleteThread(hslThread aThread)
{
    MS_U32 rc=false;

    if (0 == pthread_detach(aThread.threadId))
    {
        rc = true;
    }

    return rc;
}

/*******************************************************************************
    Function:
    Description:
    INPUT:
    OUTPUT:
 *******************************************************************************/
MS_BOOL initThread(hslThread* aThread)
{
    MS_BOOL rc = false;

    if (0 == pthread_create( &(aThread->threadId), NULL, aThread->funcPtr, aThread->funcParam))
    {
        rc = true;
    }

    return rc;
}

////////////////////////////////////////////////////////////////////////////////
//    MUTEX FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    Function:
        initMutex
    Description:
    INPUT:
    OUTPUT:
 *******************************************************************************/
MS_BOOL initMutex(pthread_mutex_t* pMutex)
{
    MS_BOOL rc = false;

    if (0 == pthread_mutex_init(pMutex, NULL))
    {
        rc = true;
    }

    return rc;
}

/*******************************************************************************
    Function:
    Description:
    INPUT:
    OUTPUT:
 *******************************************************************************/
MS_BOOL deleteMutex(pthread_mutex_t* pMutex)
{
    MS_BOOL rc = false;

    if (0 == pthread_mutex_destroy(pMutex))
    {
        rc = true;
    }

    return rc;
}

////////////////////////////////////////////////////////////////////////////////
//    CONDITION FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    Function:
        initCondition
    Description:
        Initilises a condition variable.
    INPUT:
        pCond - pointer to a condition varaible.
    OUTPUT:
        None.
    RETURNS:
        rc - indicates success or failure of initialisation
 *******************************************************************************/
MS_BOOL initCondition(pthread_cond_t* pCond)
{
    MS_BOOL rc = false;

    if (0 == pthread_cond_init(pCond, NULL))
    {
        rc = true;
    }

    return rc;
}

/*******************************************************************************
    Function:
        deleteCondition
    Description:
        Deletes a condition variable.
    INPUT:
        pCond - pointer to a condition varaible.
    OUTPUT:
        None.
    RETURNS:
        rc - indicates success or failure of initialisation
 *******************************************************************************/
MS_BOOL deleteCondition(pthread_cond_t* pCond)
{
    MS_BOOL rc = false;

    if (0 == pthread_cond_destroy(pCond))
    {
        rc = true;
    }

    return rc;
}

////////////////////////////////////////////////////////////////////////////////
//    INIT FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    Function:
        initBuffer
    Description:
        Initialises the data structure for the ring buffer and partitions the
        memory to contain the system state and the ring buffer data.
    INPUT:
        size - size of the memory available.
        physAddr - starting address of the memory in RAM (needs to be remapped for
        this process).
    OUTPUT:
        None.
 *******************************************************************************/
MS_BOOL initBufferMemory(stHslController* const _hsl, const MS_U32 size, const MS_PHYADDR physAddr)
{
    MS_U32 virtualAddress;
    int i;
    if ( size != 0 )
    {
        pthread_mutex_lock(&(_hsl->outputCondMutex));
        pthread_mutex_lock(&(_hsl->bufferMutex));

        if(physAddr != 0)
        {
            // save intial params. and buffer ptrs and sizes to system state structure
            // required to store system state in ringbuffer so need to allocate 2K.
            // from buffer of 512K.

            // convert physical address to one mapped into process memory space.
            virtualAddress = MsOS_MPool_PA2VA(physAddr);
        }
        else
        {
            virtualAddress = (MS_U32)malloc(size);
            assert(virtualAddress!=0);
        }

        // initialise memory
        memset( (MS_U8*)virtualAddress, 0, size);

        // locate system state structure at beginning of allocated memory
        _hsl->pSystemState = (stSystemState*)(virtualAddress);

        if( physAddr != 0 )
        {
            _hsl->pSystemState->dynamicAlloc = 0;
        }
        else
        {
            _hsl->pSystemState->dynamicAlloc = 1;
        }
        // store pre-determined system state buffer size
        _hsl->pSystemState->ssize = SYS_STATE_BUFSIZE;

        // initialise rest of system state structure
        _hsl->pSystemState->sbufStart = virtualAddress;
        _hsl->pSystemState->sbufEnd = virtualAddress + SYS_STATE_BUFSIZE - 1;

        // initialise the ringbuffer pointers
        _hsl->pSystemState->bufSize = size - SYS_STATE_BUFSIZE;
        _hsl->pSystemState->bufStart = virtualAddress + SYS_STATE_BUFSIZE;
        _hsl->pSystemState->bufEnd = virtualAddress + size - 1;
        _hsl->pSystemState->bufUsedEnd = _hsl->pSystemState->bufEnd;
        _hsl->pSystemState->bufWritePos = _hsl->pSystemState->bufStart + 1; // leave one byte for the initial timer wrap
        _hsl->pSystemState->bufReadPos = _hsl->pSystemState->bufStart;
        _hsl->pSystemState->bufReadLimit = _hsl->pSystemState->bufWritePos;

        _hsl->pSystemState->syncMarker = 0; // must be zero since the first byte should be sync byte

        // the following values should remain constant throughout the execution
        // calculated once on initialization
        _hsl->pSystemState->bufMinFreeBytes = _hsl->pSystemState->bufSize * BUFFER_FREE_SPACE_PERCENTAGE / 100;
        _hsl->pSystemState->bufSegmentSize = _hsl->pSystemState->bufSize / BUFFER_SEGMENTS_COUNT + 1;


        // initialize bookmarks
        _hsl->pSystemState->nextBookmark = 0;
        _hsl->pSystemState->bufBookmarks[0].address = _hsl->pSystemState->bufStart;
        _hsl->pSystemState->bufBookmarks[0].timerWrapDelta = 0;

        // initialize flags
        _hsl->pSystemState->bufFlags = BUFFER_FLAG_NONE;
        _hsl->pSystemState->flushOnConnect = 0; // don't flush just after the reset
        _hsl->pSystemState->dumpFlag = BUFFER_FLAG_NONE; // no dump at the beginning

        // initialize timing
        _hsl->pSystemState->hslLastTwTime = GetSystemTime();
        *((MS_U8*)(_hsl->pSystemState->bufStart)) = HSL_RAWLOB_MSG_TYPE_TW_CHAN0;

        _hsl->thread_finalize = false;

        for(i=0; i<PENDING_WRITE_LIST_LEN; i++)
        {
            _hsl->pendingWriteList[i].u32bufferIndex = 0;
            _hsl->pendingWriteList[i].u32timestamp = 0;
        }
        pthread_mutex_unlock(&(_hsl->bufferMutex));
        pthread_mutex_unlock(&(_hsl->outputCondMutex));

        return true;
    }
    else
    {
        return false;
    }
}

MS_BOOL deleteBufferMemory(stHslController* const _hsl)
{
    if(_hsl->pSystemState->dynamicAlloc)
    {
        free((void*)(_hsl->pSystemState));
    }
    return true;
}
////////////////////////////////////////////////////////////////////////////////
//    EXTERNALLY EXPOSED FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    Function:
        finalizeRingBuffer
    Description:
        flush output and kill any remaining threads
    INPUT:
        None.
    OUTPUT:
        None.
 *******************************************************************************/
MS_BOOL bufferFinalize(stHslController* const _hsl)
{
    MS_BOOL rc = false;

    //    free(pDropInfo);

    // tell output thread to stop output
    _hsl->thread_finalize = true;

    // delete condition variable
    if(deleteThread(_hsl->logThreads[0]))
    {
        if(deleteCondition(&(_hsl->outputCond)))
        {
            if(deleteMutex(&(_hsl->outputCondMutex)))
            {
                if(deleteMutex(&(_hsl->inputMutex)))
                {
                    if(deleteMutex(&(_hsl->bufferMutex)))
                    {
                        rc = deleteBufferMemory(_hsl);
                    }
                }
            }
        }
    }
    return rc;
}

MS_U32 bufferCalculateFreeSpace(stHslController* const _hsl)
{
    MS_U32 retVal = 0;
    if( _hsl->pSystemState->bufWritePos >= _hsl->pSystemState->bufReadPos )
    {
        retVal = _hsl->pSystemState->bufReadPos - _hsl->pSystemState->bufStart + _hsl->pSystemState->bufUsedEnd - _hsl->pSystemState->bufWritePos;
    }
    else
    {
        retVal = _hsl->pSystemState->bufReadPos - _hsl->pSystemState->bufWritePos;
    }
    return retVal;
}

/*******************************************************************************
    Function:
        bufferInitialize
    Description:
        Initialises the library - allocates the memory for the buffer, initialises
        the mutex and the output thread.
    INPUT:
        physAddr    - start addres in memory of buffer.
        size    - size in bytes of buffer.
    OUTPUT:
        None.
 *******************************************************************************/
MS_BOOL bufferInitialize(stHslController* const _hsl, const MS_PHYADDR physAddr, const MS_U32 size)
{
    MS_BOOL rc = false;

    if (initBufferMemory(_hsl, size, physAddr))
    {
        if (initMutex(&(_hsl->bufferMutex)))
        {
            if (initMutex(&(_hsl->inputMutex)))
            {
                if (initMutex(&(_hsl->outputCondMutex)))
                {
                    if (initCondition(&(_hsl->outputCond)))
                    {
                        rc = initThread(&(_hsl->logThreads[0]));
                    }
                }
            }
        }
    }

    return rc;
}

/*******************************************************************************
        Function:
                bufferDumpOldestDataSegment
        Description:
                Removes the oldest data from the buffer in order to prepare space
                for new data
        INPUT:
                None.
        OUTPUT:
                None.
 *******************************************************************************/
MS_BOOL bufferDumpOldestDataSegment(stHslController* const _hsl)
{
    // copy the data between current read marker and the nearest bookmark
    // to the dump buffer and add a warning logpoint at the end.
    // after that move the read marker N bookmarks forward.

    //find the nearest bookmark
    int i = (_hsl->pSystemState->bufReadPos - _hsl->pSystemState->bufStart) / _hsl->pSystemState->bufSegmentSize;
    if(i>0)
    {
        i--;
    }
    _hsl->pSystemState->flushOnConnect = 1;
    while( _hsl->pSystemState->bufBookmarks[i].address <= _hsl->pSystemState->bufReadPos)
    {
        ++i;
    }

    // move the read pointer forward.
    i = ( i + BUFFER_DUMP_SEGMENTS) % BUFFER_SEGMENTS_COUNT;

    if( _hsl->pSystemState->bufBookmarks[i].address < _hsl->pSystemState->bufUsedEnd )
    {
        _hsl->pSystemState->bufReadPos = _hsl->pSystemState->bufBookmarks[i].address;
    }
    else
    {
        // need to wrap around the end of the buffer
        _hsl->pSystemState->bufReadPos = _hsl->pSystemState->bufStart;
    }

    // Add a log point to inform the user about lost data
    // The log point will be injected at current READ position
    return TRUE;
}

/*******************************************************************************
    Function:
        getStartAddress
    Description:
        Populates a data structure showing next available writing positions.
    INPUT:
        dataSize    - size in bytes of data to be written to buffer.
        s_dataInfo    - data structure containing pointers to write positions, size
            of data to be written and flag indicating a wrap condition.
    OUTPUT:
        None.
 *******************************************************************************/
MS_BOOL bufferAllocateWrite(stHslController* const _hsl, const MS_U32 dataSize, stListNode** const s_dataInfo)
{
    MS_BOOL rc = false;
    MS_U32 currentTime;
    MS_U32 twDelta;
    MS_U32 u32freeSpace;
    MS_U32 u32tmpReadPtr;

    pthread_mutex_lock(&(_hsl->inputMutex));
    pthread_mutex_lock(&(_hsl->bufferMutex));
    //    pthread_mutex_lock(&dumpMutex);
    // memory buffer exists ?
    if ( !_hsl->thread_finalize )
    {
        // first things first - read the timestamp
        currentTime = GetSystemTime();

        // update the read limits first
        if( _hsl->pSystemState->bufReadLimit != _hsl->pSystemState->bufWritePos )
        {
            u32tmpReadPtr = listGetFirstValue(_hsl);
            if( u32tmpReadPtr == 0 ) // this means no writes are pending
            {
                _hsl->pSystemState->bufReadLimit = _hsl->pSystemState->bufWritePos;
            }
            else
            {
                _hsl->pSystemState->bufReadLimit = u32tmpReadPtr;
            }
        }

        // calculate the number of missing timer wraps and save the timestamp
        if( currentTime < _hsl->pSystemState->hslLastTwTime )
        {
            // the system clock has wrapped around
            twDelta = 0x10000 - (_hsl->pSystemState->hslLastTwTime >> 16) + (currentTime >> 16);
        }
        else
        {
            twDelta = (currentTime >> 16) - (_hsl->pSystemState->hslLastTwTime >> 16);
        }
        /* WARNING: as it happens, sometimes the read from the hardware clock is incorrect
         * which may result in large values of twDelta leading to the buffer being unable
         * to accomodate so much data in one write.
         * In a normal operation the value of twDelta should not exceed about 200.
         * The following assert allows for a large slip in time readings but will still
         * detect a major failure.
         *
         * It has been also observed that on some targets the hardware clock is not configured
         * and started correctly. As a result all time readings are the same and as time
         * measurement is an essential feature of hsl it will assert when a disfunctional clock
         * is detected.
         */
        assert(_hsl->pSystemState->hslLastTwTime != currentTime);
        assert(twDelta < ASSERT_RT_CLOCK_FAIL_TW);

        _hsl->pSystemState->hslLastTwTime = currentTime;

        if( _hsl->pSystemState->bufWritePos >= _hsl->pSystemState->bufReadPos )
        {
            // now see if we need to wrap around - when writing we always use the physical end of the buffer
            if( (_hsl->pSystemState->bufEnd - _hsl->pSystemState->bufWritePos) < (twDelta + dataSize) )
            {
                _hsl->pSystemState->bufUsedEnd = _hsl->pSystemState->bufWritePos;
                _hsl->pSystemState->bufWritePos = _hsl->pSystemState->bufStart;

                // set the last bookmark: it should always be aligned with the end of readable data
                _hsl->pSystemState->bufBookmarks[BUFFER_SEGMENTS_COUNT-1].address = _hsl->pSystemState->bufUsedEnd;
                _hsl->pSystemState->nextBookmark = 0;
            }
        }

        // if the free space drops below a threshold set a flag and notify the "garbage collector"
        // an additional mutex required to enable releasing blocking output thread and wait here
        u32freeSpace = bufferCalculateFreeSpace(_hsl);
        while( u32freeSpace < twDelta + dataSize )
        {
            _hsl->pSystemState->bufFlags |= BUFFER_FLAG_EMERGENCY_DUMP;
            pthread_mutex_unlock(&(_hsl->bufferMutex));
            pthread_mutex_lock(&(_hsl->outputCondMutex));
            pthread_mutex_lock(&(_hsl->bufferMutex));
            if(u32freeSpace < twDelta + dataSize) // must check again due to small probability of a complete buffer flush
            {
                _hsl->pSystemState->dumpFlag = BUFFER_FLAG_EMERGENCY_DUMP;
                bufferDumpOldestDataSegment(_hsl);
            }
            _hsl->pSystemState->bufFlags &= ~BUFFER_FLAG_EMERGENCY_DUMP;
            u32freeSpace = bufferCalculateFreeSpace(_hsl);
            pthread_cond_signal(&(_hsl->outputCond));
            pthread_mutex_unlock(&(_hsl->outputCondMutex));
        }
        if( u32freeSpace < _hsl->pSystemState->bufMinFreeBytes )
        {
            _hsl->pSystemState->bufFlags |= BUFFER_FLAG_DUMP;
            pthread_cond_signal( &(_hsl->outputCond) );
        }
        // write the missing timer wraps
        memset( (MS_U8*)(_hsl->pSystemState->bufWritePos), HSL_RAWLOB_MSG_TYPE_TW_CHAN0, twDelta );
        _hsl->pSystemState->bufWritePos += twDelta;

        // if we have a valid pointer to data info save the pending write data
        if( s_dataInfo )
        {
            // save the position of the pending write
            *s_dataInfo = listAppendValue(_hsl, _hsl->pSystemState->bufWritePos);
            (*s_dataInfo)->u32timestamp = currentTime;
        }
        else
        {
        	_hsl->pSystemState->bufReadLimit = _hsl->pSystemState->bufWritePos + dataSize;
        }
        _hsl->pSystemState->bufWritePos += dataSize;

        // did we pass a bookmark threshold? if yes save the position
        if(  _hsl->pSystemState->bufWritePos >= _hsl->pSystemState->bufStart + (_hsl->pSystemState->bufSegmentSize * (_hsl->pSystemState->nextBookmark+1)) )
        {
            _hsl->pSystemState->bufBookmarks[_hsl->pSystemState->nextBookmark].address = _hsl->pSystemState->bufWritePos;
            ++(_hsl->pSystemState->nextBookmark);
        }

        rc = true;
    }

    //    pthread_mutex_unlock(&dumpMutex);
    pthread_mutex_unlock(&(_hsl->bufferMutex));
    pthread_mutex_unlock(&(_hsl->inputMutex));

    return rc;
}

void bufferAcknowledgeWrite( stHslController* const _hsl, stListNode* const s_dataInfo )
{
    // no need to lock, the read limit will be updated on next allocate call
    listRemoveNode(_hsl, s_dataInfo);
}

/*******************************************************************************
    Function:
        sendToCmdQue
    Description:
        send the command to ringbuffer's command queue.
    INPUT:
        pCmd    - command queue header.
    OUTPUT:
        True/False.
 *******************************************************************************/
MS_BOOL bufferProcessCommand(stHslController* const _hsl, CmdInfo* const pCmd)
{
    switch( pCmd->u16Cmd)
    {
    case HSL_CMD_RB_TOTAL_SIZE:
    {
        pCmd->u32Len = _hsl->pSystemState->bufSize;
        pCmd->u32Len += _hsl->pSystemState->bufSize / HSL_RAWLOB_SYNC_MARKER_INTERVAL;
        /* NEED TO ADD 1KB TO THE REPORTED TOTAL BUFFER SIZE DUE TO AN ERROR IN HSLS
         * CAUSING A FALSE DETECTION ON A FIFO OVERFLOW
         */
        pCmd->u32Len += 1024;
    }
    break;

    case HSL_CMD_RB_EXIST_DATA_SIZE:
    {
        pthread_mutex_lock( &(_hsl->bufferMutex) );
        if( _hsl->pSystemState->bufReadLimit >= _hsl->pSystemState->bufReadPos )
        {
            pCmd->u32Len = _hsl->pSystemState->bufReadLimit - _hsl->pSystemState->bufReadPos;
        }
        else
        {
            pCmd->u32Len = (_hsl->pSystemState->bufUsedEnd - _hsl->pSystemState->bufReadPos) + (_hsl->pSystemState->bufReadLimit - _hsl->pSystemState->bufStart);
        }
        if(_hsl->pSystemState->dumpFlag != BUFFER_FLAG_NONE)
        {
            pCmd->u32Len += DUMP_LP_LEN;
        }
        pCmd->u32Len += (HSL_RAWLOB_SYNC_MARKER_INTERVAL - _hsl->pSystemState->syncMarker + pCmd->u32Len) / HSL_RAWLOB_SYNC_MARKER_INTERVAL;
        //pCmd->u32Len += pSystemState->syncMarker / (pCmd->u32Len % HSL_RAWLOB_SYNC_MARKER_INTERVAL);
        pthread_mutex_unlock( &(_hsl->bufferMutex) );
    }
    break;

    case HSL_CMD_SEND_DATA:
    {
        // mutex required to prevent inserting data in the middle of an HSL stream
        pthread_mutex_lock(&(_hsl->outputCondMutex));

        PUTCS(pCmd->pu8Buf, pCmd->u32Len, FR_HSL_FILE_DESCRIPTOR, _hsl);
        FFLUSH(FR_HSL_FILE_DESCRIPTOR);

        pthread_mutex_unlock(&(_hsl->outputCondMutex));
    }
    break;

    case HSL_CMD_RESET:
    {
        pthread_mutex_lock(&(_hsl->outputCondMutex));

        // Reset Sync Marker since the first byte needs to be a Sync Marker
        _hsl->pSystemState->bufBytesToSend = 0;
        _hsl->pSystemState->syncMarker = 0;

        if(_hsl->pSystemState->flushOnConnect > 0)
        {
            pthread_mutex_lock(&(_hsl->bufferMutex)); // TODO: review if there are no possible deadlocks
            _hsl->pSystemState->bufUsedEnd = _hsl->pSystemState->bufEnd;
            _hsl->pSystemState->bufWritePos = _hsl->pSystemState->bufStart + 1; // leave one byte for the initial timer wrap
            _hsl->pSystemState->bufReadPos = _hsl->pSystemState->bufStart;
            _hsl->pSystemState->bufReadLimit = _hsl->pSystemState->bufWritePos;

            _hsl->pSystemState->syncMarker = 0; // must be zero since the first byte should be sync byte
            // initialize bookmarks

            _hsl->pSystemState->nextBookmark = 0;
            _hsl->pSystemState->bufBookmarks[0].address = _hsl->pSystemState->bufStart;
            _hsl->pSystemState->bufBookmarks[0].timerWrapDelta = 0;

            // initialize flags
            _hsl->pSystemState->bufFlags = BUFFER_FLAG_NONE;
            _hsl->pSystemState->dumpFlag = BUFFER_FLAG_NONE;

            // initialize timing
            _hsl->pSystemState->hslLastTwTime = GetSystemTime();
            *((MS_U8*)(_hsl->pSystemState->bufStart)) = HSL_RAWLOB_MSG_TYPE_TW_CHAN0;
            _hsl->pSystemState->flushOnConnect = 1;

            pthread_mutex_unlock(&(_hsl->bufferMutex));
        }
        else
        {
            _hsl->pSystemState->flushOnConnect = 1;
        }
        pthread_mutex_unlock(&(_hsl->outputCondMutex));

        // Dump base addresses of known loaded dynamic libraries
        int i = 0;
        for(i=0; i<gDynamicLibraryCount; i++)
        {
            gDynamicLibraryInfo[i].rec_no = i+1;
            gDynamicLibraryInfo[i].rec_total = gDynamicLibraryCount;
            M_FrHslTypedef (0x80AB, HSL_DBG_SN_PROFILING, "Dynamic library base address: %d", ST_LIBRARY_SHORT_INFO, &gDynamicLibraryInfo[i]);
        }
        for(i=0; i<gThreadCount; i++)
        {
            M_FrHslTypedef (0x8DA6, HSL_DBG_SN_PROFILING, "Thread name: %d", ST_THREAD_INFO, &gThreadInfo[i]);
        }
    }
    break;

    case HSL_CMD_GET_HSL_DATA:
    {
        pthread_mutex_lock(&(_hsl->outputCondMutex));
        pthread_mutex_lock(&(_hsl->bufferMutex));
        _hsl->pSystemState->bufFlags |= BUFFER_FLAG_TX;
        _hsl->pSystemState->bufBytesToSend = pCmd->u32Len;
        pthread_cond_signal(&(_hsl->outputCond));
        pthread_mutex_unlock(&(_hsl->bufferMutex));
        pthread_mutex_unlock(&(_hsl->outputCondMutex));
    }
    break;

    default:
    {
        return FALSE;
    }
    break;
    }
    return TRUE;
}

#endif

