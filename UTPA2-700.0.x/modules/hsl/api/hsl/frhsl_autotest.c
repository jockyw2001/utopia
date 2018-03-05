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
#if defined (MSOS_TYPE_LINUX)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
// ----------------------------------------------------------
// The Little-Endian and Big-Endian is ignor in the firmware,
// The PC-side client should take care of the tranfer data endian.
// ----------------------------------------------------------


/* According to earlier standards */
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <termios.h>

#include "apiHSL.h"
#include "frhsl_datalink.h"
#include "frhsl_autotest.h"
#include "apiHSL_SHM.h"
#include "frhsl_cfg.h"

#ifdef HSL_TCPIP
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LENGTH_OF_LISTEN_QUEUE 50 //length of listen queue in server
#define DBG_OUTPUT_TCPIP 0
#define DBG_OUTPUT_FILE 0

#define HSL_DBG_OUTPUT_FILE     "/utopia_test/HSL_Log"
#define MSG_RESPONSE_OVERFLOW   "Invalid AT response length specified which exceeds %u char"

#endif

//#define HSL_FILE_MSG(FILE_NAME, fmt, arg...) printf(fmt, ##arg)
#define HSL_FILE_MSG(FILE_NAME, fmt, arg...)


FILE *OutputFp;
cHslSyncMem hslSyncMem;

static void CmdRecvStart(p_HslController _hsl);

// =============================
// For AutoTest
// =============================

static void* InitThread(void* pHsl)
{
    p_HslController hsl = (p_HslController)pHsl;
    CmdRecvStart(hsl) ;
    return NULL;
}

#define SET_RESP_DWORD(buffer, index, value) \
        *((MS_U32*)(buffer+LINK_HEADER_SIZE+4*index)) = value



void CmdSend(stHslController* const _hsl, const void *message, int length)
{
    send(_hsl->hClientSocket,message,length,0);
}

MS_BOOL SendMessage(p_HslController _hsl, void *a_pBody, int a_bodySize)
{
    CmdInfo   rb_cmd;

    rb_cmd.u32Pid = 0x12345678;     // test
    rb_cmd.u16Cmd = (MS_U16)HSL_CMD_SEND_DATA;
    rb_cmd.u32Len = a_bodySize;
    rb_cmd.pu8Buf = a_pBody;

    return FrHslCommand(_hsl, &rb_cmd);
}

MS_BOOL RespCardStatus(p_HslController _hsl, MS_U8 *a_buffer)
{
    /* TP: Need to add some error notification for the pc.
     * At the moment if anything goes wrong this function
     * aborts and quietly returns FALSE causing a timeout
     * on the PC. Let's assume that a response of (0,0)
     * will mean a problem with retrieving data from the RB.
     */

    CmdInfo rb_cmd;
    MS_BOOL bRetn = FALSE;

    MS_U32 totalSize = 0;
    MS_U32 usedSize  = 0;

    // -----------------------------------------------
    // get Total size
    // -----------------------------------------------
    rb_cmd.u32Pid = 0x12345678;          // test -- is it still needed (TP)?

    rb_cmd.u16Cmd = HSL_CMD_RB_TOTAL_SIZE;
    rb_cmd.pu8Buf = NULL;
    rb_cmd.u32Len = 0;  // put the size in here

    if( FrHslCommand(_hsl, &rb_cmd) )
    {
        totalSize = rb_cmd.u32Len;

        // -----------------------------------------------
        // get Used size
        // -----------------------------------------------
        rb_cmd.u16Cmd = HSL_CMD_RB_EXIST_DATA_SIZE;
        rb_cmd.u32Len = 0;  // put the size in here
        if( FrHslCommand(_hsl, &rb_cmd) )
        {
            usedSize = rb_cmd.u32Len;
            bRetn = TRUE;
        }
    }

    // -----------------------------------------------
    // detect error
    // -----------------------------------------------
    if((usedSize > totalSize) && (usedSize != 0xffffffff))
    {
        // body.usedSize = body.totalSize / 2; // TP: why was this necessary?
        HSL_FILE_MSG(OutputFp, "Ring buffer internal error\n");
        bRetn = FALSE;
    }

    // -----------------------------------------------
    // send Response
    // -----------------------------------------------
    if( bRetn != TRUE )
    {
        usedSize = totalSize = 0; // clear any rubbish
    }

    a_buffer[0] = (MS_U8)EN_LINK_CMD_RESP_CARD_STATUS;
    // seqNo should remain the same as in the received message

    SET_RESP_DWORD(a_buffer, 0, usedSize);
    SET_RESP_DWORD(a_buffer, 1, totalSize);

    bRetn &= SendMessage(_hsl, a_buffer, LINK_HEADER_SIZE+LINK_DATA_RESPONSE_CARD_STATUS);

    //HSL_FILE_MSG(OutputFp, "Used_Size:%lx RingBuf_Size:%lx\n", body.usedSize, body.totalSize);
    // HSL_FILE_MSG(OutputFp, "CardStatus=%x/%x\n", usedSize, totalSize);

    return bRetn;
}

MS_BOOL RespHslData(p_HslController _hsl, MS_U8 *a_buffer)
{
    MS_BOOL bRetn = FALSE;
    int     blength;

    CmdInfo rb_cmd;

    // -----------------------------------------------
    // recv "Request Size"
    // -----------------------------------------------
    blength = recv(_hsl->hClientSocket, a_buffer+LINK_HEADER_SIZE, LINK_DATA_GET_HSL_DATA, 0);
    if (blength == 0)
    {
        HSL_FILE_MSG(OutputFp,"Client connection has been closed.\n");
        return FALSE;
    }

    MS_U32 reqSize = *((MS_U32*)(a_buffer+LINK_HEADER_SIZE));
    //HSL_FILE_MSG(OutputFp, "RequestSize : %lx\n", reqSize);

    // -----------------------------------------------
    // get Used size
    // -----------------------------------------------

    rb_cmd.u16Cmd = HSL_CMD_RB_EXIST_DATA_SIZE;
    rb_cmd.u32Len = 0;  // put the size in here
    bRetn = FrHslCommand(_hsl, &rb_cmd);
    if( !bRetn )
    {
        return FALSE;
    }
    MS_U32 usedSize = rb_cmd.u32Len;

    // -----------------------------------------------
    // Decide bytes send back
    // -----------------------------------------------
    if( reqSize > usedSize)
    {
        reqSize = usedSize;
        HSL_FILE_MSG(OutputFp, "RealSize : %lx\n", reqSize);
    }

    // -----------------------------------------------
    // Send "Response CMD"
    // -----------------------------------------------

    //TODO: Check if the requested data is still available in the ringbuffer
    /* (TP) By the look of it it is always attempted to send exactly the requested amount
     * of data from the ringbuffer. Could see no query to the RB if such amount is available.
     * will leave it like this for now but it must be addressed before release.
     */

    a_buffer[0] = (MS_U8)EN_LINK_CMD_RESP_HSL_DATA; // only need to change the cmd id in this case
    SET_RESP_DWORD(a_buffer, 0, reqSize);

    bRetn = SendMessage(_hsl, a_buffer, LINK_HEADER_SIZE+LINK_DATA_RESPONSE_HSL_CMD);

    if( bRetn)
    {
        // -----------------------------------------------
        // pop Data
        // -----------------------------------------------
        // HSL_FILE_MSG(OutputFp, "Start to send packet type 4, request size:%x.\n", reqStru.reqSize);
        rb_cmd.u32Pid = 0;     // test
        rb_cmd.u16Cmd = HSL_CMD_GET_HSL_DATA;
        rb_cmd.u32Len = reqSize;
        rb_cmd.pu8Buf = NULL;

        bRetn = FrHslCommand(_hsl, &rb_cmd);
    }

    return bRetn;
}

MS_U32 ProcessCmd(p_HslController _hsl, MS_U8 *a_cmdBuffer)
{
    MS_U32 errCode = EN_CMD_EC_OK;
    int i = 0;

    // At this point we assume that the PC has sent all the data in
    // and cast the buffer onto a command structure
    ST_HSL_CMD_REQ* p_command = (ST_HSL_CMD_REQ*)a_cmdBuffer;

    HSL_FILE_MSG(OutputFp,"CmdId=%X, Ver=%X\n", p_command->cmdId, p_command->ver);

    if( p_command->ver != LINK_PROTOCOL_VERSION )
    {
      errCode = EN_CMD_EC_VERSION;
    }
    else
    {
      switch( p_command->cmdId )
      {
          case EN_CMD_TEST_NONE:
              errCode = EN_CMD_EC_CMD_UNKNOWN;
              break;
          case EN_CMD_TEST_AT:
              if( !StoreShareMemory(&hslSyncMem, p_command->U_Cmd.atCmd) )
              {
                errCode = EN_CMD_EC_AT_FAILURE;
                PackAtCmdEchoIntoHslCmdResp(false, p_command->U_Cmd.atCmd);
              }
              else
              {
                PackAtCmdEchoIntoHslCmdResp(true, p_command->U_Cmd.atCmd);
              }
              break;
          case EN_CMD_TEST_LOAD:
          {
              HSL_FILE_MSG(OutputFp, "Load Test request received\n");
              gRunLoadTest = true;
              for(i=0; i<p_command->U_Cmd.loadTestCmd.mThreadNo; i++)
              {
                  if( 0 != pthread_create(&gTestThreads[i], 0, HslSelfTestThreadFun, (void*)(&p_command->U_Cmd.loadTestCmd) ) )
                  {
                      HSL_FILE_MSG(OutputFp, "Failed to create thread! Stop the load test\n");
                      errCode = EN_CMD_EC_THREAD_FAIL;
                      gRunLoadTest = false;
                      break;
                  }
                  else
                  {
                      HSL_FILE_MSG(OutputFp, "Load test thread %d created\n", i);
                  }
              }

          }
              break;
          default:
            errCode = EN_CMD_EC_CMD_UNKNOWN;
            break;
      }
    }

    return errCode;
}

/** Wrapper function to echo back input AT command back to PC.
    This is done to permit to get an exact timestamp on when the command was processed.  */
void PackAtCmdEchoIntoHslCmdResp(MS_BOOL a_result, const char* a_atCmd)
{
    AtCmdEcho obj;
    obj.result = a_result;
    memset(obj.atCmd, '\0', sizeof(obj.atCmd));
    strncpy(obj.atCmd, a_atCmd, sizeof(obj.atCmd) - 1);
    PackIntoHslCmdResp(EN_RESP_TEST_AT_ECHO, (const void*)&obj);
}


/** Wrapper function for sending AT command response back to PC */
void PackAtRespIntoHslCmdResp(const char* a_atResp)
{
    PackIntoHslCmdResp(EN_RESP_TEST_AT, (const void*)a_atResp);
}


/** Wrapper function to transport over log point id 0x000A the Hsl Command response
 * The implementation depends closely on the definition of ST_HSL_CMD_RESP; Any
 * update to that structure would need to be reflected here. */
void PackIntoHslCmdResp(EN_RESPONSE_ID a_respId, const void* a_body)
{
    MS_U16 packedStructLength = offsetof(ST_HSL_CMD_RESP, U_Resp); /* This gets the total size up to the start of the union. */

    ST_HSL_CMD_RESP container;

    container.respId = a_respId;

    switch (a_respId)
    {
    case EN_RESP_TEST_AT:
    {
        MS_U32 strLength = strlen((const char*)a_body) + 1; /* +1 for the null char as this is a string */
        if (strLength > RSP_MAX_AT)
        {
            snprintf(container.U_Resp.atResponse, strlen(MSG_RESPONSE_OVERFLOW)+10,MSG_RESPONSE_OVERFLOW, RSP_MAX_AT);
            packedStructLength += (MS_U16)(strlen(container.U_Resp.atResponse) + 1);
        }
        else
        {
            strncpy(container.U_Resp.atResponse, (char*) a_body,strLength);
            packedStructLength += (MS_U16)strLength;
        }
    }
    break;

    case EN_RESP_TEST_AT_ECHO:
    {
        const AtCmdEcho* cmdEchoPtr = (const AtCmdEcho*) a_body;
        container.U_Resp.atCmdEcho.result = (MS_BOOL) (cmdEchoPtr->result);

        MS_U32 strLength = strlen((const char*)cmdEchoPtr->atCmd) + 1; /* +1 for the null char as this is a string */
        if (strLength > CMD_MAX_AT)
        {
            snprintf(container.U_Resp.atCmdEcho.atCmd, strlen(MSG_RESPONSE_OVERFLOW)+10,MSG_RESPONSE_OVERFLOW, CMD_MAX_AT);
            packedStructLength += (MS_U16)(strlen(container.U_Resp.atCmdEcho.atCmd) + 1);
        }
        else
        {
            strncpy(container.U_Resp.atCmdEcho.atCmd, (char*) cmdEchoPtr->atCmd,strLength);
            packedStructLength += (MS_U16)strLength;
        }

    }
    break;

    case EN_RESP_TEST_CMD_LAST:
        break;

    default:
        break;
    }

    if ((packedStructLength % 2) == 1)
    {
        packedStructLength += (MS_U16)1;  // This ensures we don't lose one character as we transfer in MS_U16 blocks over HSL.
    }

    HSL_FILE_MSG(OutputFp,"Length=%d\n", packedStructLength);

    /* Using hard coded log point ID for this structure; Do not modify this log point ID as tools
     * on PC depend on it. */
    M_FrHslArray16(0x000A, HSL_DBG_UTOPIA_ALL_DEBUG, "ST_HSL_CMD_RESP: %#04X", packedStructLength/sizeof(MS_U16), &container);

}

// -----------------------------------------------
// Process HSL command
// If error occured and can not be recovered, return false
// This connection should be closed
// -----------------------------------------------
MS_BOOL RespHslCmd(p_HslController _hsl, MS_U8 *a_buffer)
{
    int     bLength     = 0;
    int     bReceived   = 0;
    MS_U8   cmdBuffer[LINK_MAX_COMMAND];
    MS_BOOL bRetn       = FALSE;
    MS_U32  errCode     = EN_CMD_EC_OK;

    //----------------------------------
    // Read "len" field first
    //----------------------------------
    bLength = recv(_hsl->hClientSocket, a_buffer+LINK_HEADER_SIZE, LINK_DATA_SEND_HSL_CMD, 0);
    if (bLength == 0)
    {
        HSL_FILE_MSG(OutputFp,"Client connection has been closed.\n");
        return FALSE;
    }
    else
    {
        bRetn = TRUE;
    }

    //----------------------------------
    // Read all payload after "len"
    //----------------------------------
    MS_U32 reqLen = *((MS_U32*)(a_buffer+LINK_HEADER_SIZE));
    if( reqLen > LINK_MAX_COMMAND )
    {
        // No recovery - in case of failure everything should shutdown
        errCode = EN_CMD_EC_DATA_SIZE;
        HSL_FILE_MSG(OutputFp, "Command received, wrong size: %u is not < %u\n", (unsigned int)reqLen, LINK_MAX_COMMAND);
    }
    else
    {
        // Read the whole message - may come in one or more pieces
        bReceived = 0;
        while( bReceived < reqLen )
        {
            //TODO!: add timeout handling
            bLength = recv(_hsl->hClientSocket, (char*)(cmdBuffer+bReceived), reqLen-bReceived, 0);
            bReceived += bLength;
        }
        errCode = ProcessCmd(_hsl, cmdBuffer);
    }

    a_buffer[0] = (MS_U8)EN_LINK_CMD_RESP_HSL_CMD;
    SET_RESP_DWORD(a_buffer, 0, errCode);

    // bRetn = (errCode == EN_CMD_EC_OK); // All or nothing, in case of error close the connection
    bRetn &= SendMessage(_hsl, a_buffer, LINK_HEADER_SIZE+LINK_DATA_RESPONSE_HSL_CMD);
    HSL_FILE_MSG(OutputFp, "After processing a command: err code: %u, result: %d\n", (unsigned int)errCode, (int)bRetn);
    return bRetn;
}


MS_BOOL RespFilterMx(p_HslController _hsl, MS_U8 * a_buffer)
{
    int     bLength     = 0;
    int     bReceived   = 0;
    MS_U8   cmdBuffer[CMD_MAX_FILTER];
    MS_BOOL bRetn       = FALSE;
    MS_U32  errCode     = EN_CMD_EC_OK;

    //----------------------------------
    // Read "len" field first
    //----------------------------------
    bLength = recv(_hsl->hClientSocket, a_buffer+LINK_HEADER_SIZE, LINK_DATA_SEND_HSL_CMD, 0);
    if (bLength == 0)
    {
        HSL_FILE_MSG(OutputFp,"Client connection has been closed.\n");
        return FALSE;
    }

    //----------------------------------
    // Read all payload after "len"
    //----------------------------------
    MS_U32 reqLen = *((MS_U32*)(a_buffer+LINK_HEADER_SIZE));
    if( reqLen > CMD_MAX_FILTER )
    {
        // No recovery - in case of failure everything should shutdown
        errCode = EN_CMD_EC_DATA_SIZE;
    }
    else
    {
        // Read the whole message - may come in one or more pieces
        bReceived = 0;
        while( bReceived < reqLen )
        {
            bLength = recv(_hsl->hClientSocket, (char*)(cmdBuffer+bReceived), reqLen-bReceived, 0);
            bReceived += bLength;
        }
        //errCode = ProcessCmd(cmdBuffer);
        KiTtiProcessSetsFilter((KiSetsFilterMatrix*)cmdBuffer);
    }

    a_buffer[0] = (MS_U8)EN_LINK_CMD_RSP_FILTER_MX;
    SET_RESP_DWORD(a_buffer, 0, errCode);

    HSL_FILE_MSG(OutputFp,"reqLen=%lx, errCode=%lx\n", reqLen, errCode);

    bRetn = (errCode == EN_CMD_EC_OK); // All or nothing, in case of error close the connection
    bRetn &= SendMessage(_hsl, a_buffer, LINK_HEADER_SIZE+LINK_DATA_RESPONSE_HSL_CMD);
    return bRetn;
}

// -----------------------------------------------
// Thread Start Function
// -----------------------------------------------
static void CmdRecvStart(p_HslController _hsl)
{
    struct sockaddr_in  servaddr;
    struct sockaddr_in  cliaddr;
    socklen_t       socklen;
    int         blength;
    struct          timeval tv;
    int           iResult;

    MS_BOOL       bRetn = FALSE;
    //string output;
    //long timestamp;

    // -----------------------------------------------
    // Initial
    // -----------------------------------------------

    assert(_hsl!=0);

    blength = 0;
    bRetn = true;

    socklen = sizeof(cliaddr);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family       = AF_INET;
    servaddr.sin_port         = htons(_hsl->u32Port);
    servaddr.sin_addr.s_addr  = htons(INADDR_ANY);

#if DBG_OUTPUT_FILE
    OutputFp = fopen(HSL_DBG_OUTPUT_FILE,"w+");
    fclose(OutputFp);
#endif

    // -----------------------------------------------
    // listen port
    // -----------------------------------------------
    HSL_FILE_MSG(OutputFp,"HSL: create socket!\n");
    _hsl->hServerSocket = socket(AF_INET,SOCK_STREAM,0);
    if (_hsl->hServerSocket < 0)
    {
        HSL_FILE_MSG(OutputFp,"create socket error!\n");
        return;
    }

    HSL_FILE_MSG(OutputFp, "HSL: bind to port %u!\n", (unsigned int)_hsl->u32Port);
    iResult = bind(_hsl->hServerSocket, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (iResult < 0)
    {
        HSL_FILE_MSG(OutputFp, "HSL: bind to port %u failure!\n",  (unsigned int)_hsl->u32Port);
        return;
    }

    HSL_FILE_MSG(OutputFp,"HSL: call listen!\n");
    if (listen(_hsl->hServerSocket,LENGTH_OF_LISTEN_QUEUE) < 0)
    {
        HSL_FILE_MSG(OutputFp,"HSL: call listen failure!\n");
        return;
    }

    HSL_FILE_MSG(OutputFp,"HSL: Start to accept socket connection.\n");

    if(_hsl->u8Instance == 0 )
    {
        if( false == InitShareMemory(&hslSyncMem, true))
        {
            HSL_FILE_MSG(OutputFp,"HSL: InitShareMemory() error!\n");
            return;
        }
        HSL_FILE_MSG(OutputFp,"HSL: InitShareMemory() successful!\n");
    }

    HSL_FILE_MSG(OutputFp,"HSL: check if flash ok, before while(1)\n");

    // -----------------------------------------------
    // set timeout
    // -----------------------------------------------

    tv.tv_sec = LINK_CONNECTION_TIMEOUT;
    tv.tv_usec = 0;
    if(setsockopt(_hsl->hServerSocket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0)
    {
        HSL_FILE_MSG(OutputFp,"HSL: setsockopt error!\n");
        return;
    }

    // -----------------------------------------------
    // process data
    // -----------------------------------------------
    MS_U8 socketBuffer[LINK_BUFFER_SIZE];
    pthread_mutex_lock(&(_hsl->socketReadMutex));

    while(_hsl->g_AutoTestRun)
    {
        // -----------------------------------------------
        // accept()
        // -----------------------------------------------
        HSL_FILE_MSG(OutputFp, "HSL: Waiting for an access to the RCV loop\n");
        while(_hsl->g_AutoTestRun)
        {
            _hsl->hClientSocket = accept(_hsl->hServerSocket, (struct sockaddr*)&cliaddr, &socklen);
            if( _hsl->hClientSocket >= 0)
            {
                FrHslResetConnection(_hsl); // when new connection come, reset it
                bRetn = TRUE;
                break;
            }
            else
            {
                HSL_FILE_MSG(OutputFp, "HSL: Socket not connected: errno=%d ", errno);
                if( errno == 11 )
                {
                    HSL_FILE_MSG(OutputFp, "(Try again)\n");
                }
                else
                {
                    HSL_FILE_MSG(OutputFp, "\n");
                }
            }
            errno = 0;
        }

        HSL_FILE_MSG(OutputFp,"HSL: Accepted connection from client,IP:%s,Port:%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

        while((_hsl->hClientSocket >= 0) && (_hsl->g_AutoTestRun))
        {
            // -----------------------------------------------
            // Receive packet header: Command(1 byte) and Sequence Number (1 byte)
            // -----------------------------------------------

            // The return value will be 0 when the peer has performed an orderly shutdown and errno == 0
            blength = recv(_hsl->hClientSocket, socketBuffer, LINK_HEADER_SIZE, 0);
            if (blength > 0)
            {
//                HSL_FILE_MSG(OutputFp, "HSL: Received %d bytes\n", blength);
                bRetn = TRUE;
            }
            else if(blength == 0)
            {
                HSL_FILE_MSG(OutputFp, "HSL: Connection closed by client\n");
                bRetn = FALSE;
            }
            else // blength <= 0
            {
                HSL_FILE_MSG(OutputFp, "HSL: Socket error: %d\n", errno);
                bRetn = FALSE;
            }

            if(bRetn != FALSE)
            {
                MS_U8 cmdId = socketBuffer[0];

                switch(cmdId)
                {
                case EN_LINK_CMD_GET_CARD_STATUS:
                    bRetn = RespCardStatus(_hsl, socketBuffer);
                    break;

                case EN_LINK_CMD_GET_HSL_DATA:
                    bRetn = RespHslData(_hsl, socketBuffer);
                    break;

                case EN_LINK_CMD_SEND_HSL_CMD:
                    bRetn = RespHslCmd(_hsl, socketBuffer);
                    break;

                case EN_LINK_CMD_SND_FILTER_MX:
                    bRetn = RespFilterMx(_hsl ,socketBuffer);
                    break;

                default:
                    //TODO: need to think if any feedback to the pc is required
                    bRetn = false;
                    HSL_FILE_MSG(OutputFp, "HSL: ERROR: Unknown command!\n");
                    break;
                }
//                HSL_FILE_MSG(OutputFp,"HSL: Finished processing a command\n");
            }
            else
            {
                HSL_FILE_MSG(OutputFp,"HSL: Connection closed or HSL transport layer failure\n");
                break; // ToDo: uncomment it, if teraHSL can create new clifd
                // TP: if the protocol is broken for some reason there is no point
                // in keeping the connection alive. If TeraHSL cannot cope with it
                // it's should not compromise the implementation on the target.
            }
        }
        if (_hsl->hClientSocket >=0)
        {
            HSL_FILE_MSG(OutputFp,"HSL: Closing client socket %d\n", _hsl->hClientSocket);
            close(_hsl->hClientSocket);
        }
        else
        {
            printf("HSL: ERROR: Invalid id when attempting to close a socket (%d)\n", _hsl->hClientSocket);
        }
    }

    pthread_mutex_unlock(&(_hsl->socketReadMutex));
    close(_hsl->hServerSocket);
    FreeShareMemory(&hslSyncMem);

    return;
}

MS_BOOL FrHsl_AutoTest_Init(stHslController* const _hsl)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    _hsl->g_AutoTestRun = true;
    if(!pthread_create(&(_hsl->threadCmdRecvStart), &attr, InitThread, _hsl))
        return true;
    else
        return false;
}

void FrHsl_AutoTest_End(stHslController* const _hsl)
{
    _hsl->g_AutoTestRun = false;
    if( 0 != pthread_join(_hsl->threadCmdRecvStart, 0))
    {
        HSL_FILE_MSG(OutputFp, "ERROR: Failed to shutdown the HSL receive thread!\n");
    }
}

#endif
/* END OF FILE */

