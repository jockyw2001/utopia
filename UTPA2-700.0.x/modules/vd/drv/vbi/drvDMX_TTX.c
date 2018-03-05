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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define _DRV_DMX_TTX_C

// Common Definition
#if defined(MSOS_TYPE_LINUX_KERNEL)
//    #include <linux/slab.h>
    #include <linux/string.h> //for memcpy, memset
#else
    #include <string.h>
#endif
#include "MsCommon.h"
#include "MsOS.h"
#include "ULog.h"
// Internal Definition
#include "drvDMX_TTX.h"
#include "drvMMIO.h"

#include "utopia.h"

#if (VBI_TTX_UTOPIA20)
#include "drvVBI_v2.h"
#endif

#include "drvVBI_private.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAPI_DMX_DMXID_NULL 0xFF
#define DEFAULT_WAIT_TIME              MSOS_WAIT_FOREVER              // 50 ms

static DMX_CB_FN       _pFN = NULL;

static VBI_RING_BUFFER rb_handle;

static DMX_TTX_StoreInfo _pStoreInfo = {
    ._bTTXInit   = FALSE,
    ._bSuspend   = FALSE,
    ._TTXBufAddr = 0xFFFFFFFFUL,    
    ._TTXBufLen  = 0,
    ._FilterID   = MAPI_DMX_DMXID_NULL,
};

// Mutex & Lock
static MS_S32 _s32DMXMutex = -1;

#if (VBI_TTX_UTOPIA20)
static void* pInstantVbiTTX   = NULL;
static void* pAttributeVbiTTX = NULL;
#endif

/********************************************************************************/
/*                                                        external functions                                                       */
/********************************************************************************/

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_DMX_TTX_UpdateWritePointer(void)
{
    MS_S32 NumberOfFreePackets;
    MS_U32 addr;

    if(_pStoreInfo._FilterID==MAPI_DMX_DMXID_NULL)
    {
        return FALSE;
    }

    if(_pFN==NULL)
    {
        return FALSE;
    }

    addr = (*_pFN)(_pStoreInfo._FilterID);
    rb_handle.WritePacketNumber = (addr - _pStoreInfo._TTXBufAddr)/VBI_TTX_DATA_LENGTH;
    rb_handle.WritePacketNumber = rb_handle.WritePacketNumber%_pStoreInfo._TTXBufLen;

    if(rb_handle.pkt_received==TRUE)
    {
        NumberOfFreePackets = (MS_S32)(rb_handle.WritePacketNumber - rb_handle.ReadPacketNumber);
        if (NumberOfFreePackets < 0)
            NumberOfFreePackets += _pStoreInfo._TTXBufLen;

        NumberOfFreePackets = (MS_S32)(_pStoreInfo._TTXBufLen - NumberOfFreePackets);

        if (NumberOfFreePackets < 2)
        {
            rb_handle.NoOfPacketBufferOverflows++;
            rb_handle.PacketBufferOverflow = TRUE;
            return FALSE;
        }
    }

    if(rb_handle.WritePacketNumber == 0)
    {
        if(rb_handle.pkt_received )
        {
            rb_handle.WritePacketNumber = _pStoreInfo._TTXBufLen - 1;
        }
    }
    else if(rb_handle.WritePacketNumber != rb_handle.ReadPacketNumber)
    {
        rb_handle.WritePacketNumber--;
        rb_handle.pkt_received = TRUE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void _MDrv_DMX_TTX_RingBuffer_Reset(void)
{
    MS_U32 addr =0;
    memset((MS_U8 *) MS_PA2KSEG1(_pStoreInfo._TTXBufAddr), 0x20, _pStoreInfo._TTXBufLen*VBI_TTX_DATA_LENGTH);

    OS_OBTAIN_MUTEX(_s32DMXMutex, DEFAULT_WAIT_TIME);

    if(_pFN!=NULL)
    {
        addr = (*_pFN)(_pStoreInfo._FilterID);
        rb_handle.ReadPacketNumber = (addr - _pStoreInfo._TTXBufAddr)/VBI_TTX_DATA_LENGTH;
    }
    else
        rb_handle.ReadPacketNumber = 0;

    rb_handle.ReadPacketNumber = rb_handle.ReadPacketNumber%_pStoreInfo._TTXBufLen;
    rb_handle.WritePacketNumber = rb_handle.ReadPacketNumber;
    rb_handle.pkt_received = FALSE;

    rb_handle.PacketBufferOverflow = FALSE;
    rb_handle.NoOfPacketBufferOverflows = 0;
    OS_RELEASE_MUTEX(_s32DMXMutex);

}

MS_BOOL _MDrv_DMX_TTX_Init(EN_DMX_TTX_CMD cmd, MS_PHY bufferAddr, MS_U16 packetCount)
{
    static char _dmxTTXSyncMutexName[] = "OS_DMX_TTX_MUTEX";

    if(cmd == DMX_TTX_INIT)
    {
        _s32DMXMutex = //OS_CREATE_MUTEX(DMX_TTX);
                MsOS_CreateMutex(E_MSOS_FIFO, _dmxTTXSyncMutexName, MSOS_PROCESS_PRIVATE);

        _pStoreInfo._TTXBufAddr = bufferAddr;
        _pStoreInfo._TTXBufLen = packetCount;
        _pStoreInfo._FilterID = MAPI_DMX_DMXID_NULL;

        _pStoreInfo._bTTXInit = TRUE;
    }
    
    _MDrv_DMX_TTX_RingBuffer_Reset();

    return TRUE;
}

MS_BOOL _MDrv_DMX_TTX_Exit(EN_DMX_TTX_CMD cmd)
{
    _pStoreInfo._bTTXInit = FALSE;

    if(cmd == DMX_TTX_EXIT)
    {
        if (_s32DMXMutex != -1)
        {
            OS_OBTAIN_MUTEX(_s32DMXMutex, DEFAULT_WAIT_TIME);
            OS_RELEASE_MUTEX(_s32DMXMutex);
            OS_DELETE_MUTEX(_s32DMXMutex);
        }
    }
                    
    return TRUE;
}

void _MDrv_DMX_TTX_SetFilterID(MS_U8 fid)
{
    _pStoreInfo._FilterID = fid;
}

void _MDrv_DMX_TTX_SetCB(DMX_CB_FN fn)
{
    _pFN = fn;
}

MS_BOOL _MDrv_DMX_TTX_PacketBufferIsEmpty(void)
{
    if(_MDrv_DMX_TTX_UpdateWritePointer()==FALSE)   // if overflow or slicer is not ready, define it is buffer empty
        return TRUE;
    if (rb_handle.WritePacketNumber - rb_handle.ReadPacketNumber) /* no of packets */
        return FALSE;

    return TRUE;
}

MS_BOOL _MDrv_DMX_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length)
{
    MS_U8 *pData = (MS_U8 *) MS_PA2KSEG1(dataAddr);
    MS_BOOL ret = FALSE;

    if((*length)==0)
        return FALSE;

    OS_OBTAIN_MUTEX(_s32DMXMutex, DEFAULT_WAIT_TIME);

    if (_MDrv_DMX_TTX_PacketBufferIsEmpty()==FALSE)
    {
        MS_PHY packetAddress;
        MS_U32 data_length;
        MS_U32 data_length2;

        packetAddress = _pStoreInfo._TTXBufAddr + (rb_handle.ReadPacketNumber) * VBI_TTX_DATA_LENGTH;

        if(rb_handle.ReadPacketNumber > rb_handle.WritePacketNumber)
        {
             // HEAD <--- 2 ---> W ------R <--- 1 ---> TAIL

            // copy region 1
            data_length = VBI_TTX_DATA_LENGTH*((_pStoreInfo._TTXBufLen - 1) - rb_handle.ReadPacketNumber);

            if(data_length > (*length))
                data_length = (*length);

            memcpy(pData, (MS_U8 *) MS_PA2KSEG1(packetAddress), data_length);

            // input buffer is full
            if((*length)==data_length)
            {
                rb_handle.ReadPacketNumber += data_length/VBI_TTX_DATA_LENGTH;
                ret = TRUE;
                goto GET_PACKETS_EXIT;
            }

            // copy region 2
            data_length2 = VBI_TTX_DATA_LENGTH*(rb_handle.WritePacketNumber);

            // calculate the left buffer size
            if(data_length2 > ((*length) - data_length))
                data_length2 = ((*length) - data_length);

            memcpy(pData + data_length, (MS_U8 *) MS_PA2KSEG1(_pStoreInfo._TTXBufAddr), data_length2);

            rb_handle.ReadPacketNumber = data_length2/VBI_TTX_DATA_LENGTH;
            (*length) = data_length + data_length2;

            ret = TRUE;
        }
        else
        {
            // R <--- 1 ---> W

            // copy region 1
            data_length = VBI_TTX_DATA_LENGTH*(rb_handle.WritePacketNumber - rb_handle.ReadPacketNumber);

            if(data_length > (*length))
                data_length = (*length);

            memcpy(pData, (MS_U8 *) MS_PA2KSEG1(packetAddress), data_length);

            rb_handle.ReadPacketNumber += data_length/VBI_TTX_DATA_LENGTH;
            (*length) = data_length;

            ret = TRUE;
        }
    }

GET_PACKETS_EXIT:
    OS_RELEASE_MUTEX(_s32DMXMutex);
    return ret;
}

MS_BOOL _MDrv_DMX_TTX_GetPacket(MS_PHY *packetAddress)
{
    MS_BOOL ret = FALSE;

    OS_OBTAIN_MUTEX(_s32DMXMutex, DEFAULT_WAIT_TIME);

    if (_MDrv_DMX_TTX_PacketBufferIsEmpty()==FALSE)
    {
        *packetAddress = _pStoreInfo._TTXBufAddr + rb_handle.ReadPacketNumber*VBI_TTX_DATA_LENGTH;

        rb_handle.ReadPacketNumber++;

        // TSP, 0 -> 1 -> 2 ->3 -> N-1 -> 0 -> 1 ->2 -> 3 ...
        if ( rb_handle.ReadPacketNumber >= _pStoreInfo._TTXBufLen )
        {
            rb_handle.ReadPacketNumber = 0;
        }
        
        ret = TRUE;
    }
    
    OS_RELEASE_MUTEX(_s32DMXMutex);
    return ret;
}

MS_BOOL _MDrv_DMX_TTX_PacketBufferIsOverflow( void )
{
    return rb_handle.PacketBufferOverflow;
}

MS_U16 _MDrv_DMX_TTX_PacketBufferGetNoOfOverflows( void )
{
    return rb_handle.NoOfPacketBufferOverflows;
}

MS_BOOL _MDrv_DMX_TTX_Suspend(void)
{
    if((_pStoreInfo._bSuspend != FALSE) || (_s32DMXMutex == -1))
    {
        return FALSE;
    }

    _pStoreInfo._bSuspend   = TRUE;
    _MDrv_DMX_TTX_Exit(DMX_TTX_SUSPEND);
    return TRUE;
}

MS_BOOL _MDrv_DMX_TTX_Resume(void)
{
    if((_pStoreInfo._bSuspend != TRUE) || (_s32DMXMutex == -1))
    {
        return FALSE;
    }

    _pStoreInfo._bSuspend = FALSE;
    _MDrv_DMX_TTX_Init(DMX_TTX_RESUME, _pStoreInfo.bufferAddr, _pStoreInfo.packetCount);
    return TRUE;
}

MS_U32 _MDrv_DMX_TTX_SetPowerState(EN_POWER_MODE u16PowerState)
{
    MS_U16 _ret = FALSE;

    switch (u16PowerState)
    {
        case E_POWER_SUSPEND:
            _ret = MDrv_DMX_TTX_Suspend();
            break;

        case E_POWER_RESUME:
            _ret = MDrv_DMX_TTX_Resume();
            break;

        case E_POWER_MECHANICAL:
        case E_POWER_SOFT_OFF:
        default:
            ULOGE("VBI", "[%s] %d Power state not support!!\n", __FUNCTION__, __LINE__);
            break;
    }

    return ((TRUE == _ret)? UTOPIA_STATUS_SUCCESS: UTOPIA_STATUS_FAIL);
}


void MDrv_DMX_TTX_RingBuffer_Reset(void)
{
#if (VBI_TTX_UTOPIA20)
    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
                                            
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_RingBuffer_Reset,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_RingBuffer_Reset fail\n");  
        return;
    }
#else
    _MDrv_DMX_TTX_RingBuffer_Reset();
#endif
}

MS_BOOL MDrv_DMX_TTX_Init(MS_PHY bufferAddr, MS_U16 packetCount)
{    
#if (VBI_TTX_UTOPIA20)
    VBI_INITIALIZER_TTX_SLICER InitializerTTXSlicer;   
    memset(&InitializerTTXSlicer, 0 , sizeof(VBI_INITIALIZER_TTX_SLICER));
    
    if (NULL == pInstantVbiTTX)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_VBI, (void**)&pInstantVbiTTX, 0, pAttributeVbiTTX))
        {
            ULOGE("VBI", "UtopiaOpen VBI fail\n");
            return FALSE;
        }
    }
    else
    {
        ULOGE("VBI", "VBI_TTX already Opened\n");
        return FALSE;
    }

    InitializerTTXSlicer.cmd = DMX_TTX_INIT;
    InitializerTTXSlicer.bufferAddr  = bufferAddr;
    InitializerTTXSlicer.packetCount = packetCount;
                                                
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_Init,(void*)&InitializerTTXSlicer) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_Init fail\n");  
        return FALSE;
    }
    return TRUE;
#else
    return _MDrv_DMX_TTX_Init(DMX_TTX_INIT, bufferAddr, packetCount);
#endif
}

MS_BOOL MDrv_DMX_TTX_Exit(void)
{

#if (VBI_TTX_UTOPIA20) 
    DMX_TTX_CMD Cmd;
    memset(&Cmd, 0 , sizeof(DMX_TTX_CMD));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbiTTX=NULL fail\n");
        return FALSE;
    }

    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_Exit,(void*)&Cmd) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_CMD_DMX_TTX_Exit fail\n");
        return FALSE;
    }
    return TRUE;
#else                    
    return _MDrv_DMX_TTX_Exit(DMX_TTX_EXIT);
#endif

}

void MDrv_DMX_TTX_SetFilterID(MS_U8 fid)
{
#if (VBI_TTX_UTOPIA20)
    DMX_TTX_SET_FILTERID DMXSetFilterID;
    memset(&DMXSetFilterID, 0 , sizeof(DMX_TTX_SET_FILTERID));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
        
    DMXSetFilterID.fid = fid;
                                                    
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_SetFilterID,(void*)&DMXSetFilterID) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_SetFilterID fail\n");  
        return;
    }
#else
    _MDrv_DMX_TTX_SetFilterID(fid);
#endif
}

void MDrv_DMX_TTX_SetCB(DMX_CB_FN fn)
{
#if (VBI_TTX_UTOPIA20)
    DMX_TTX_SET_CB DMXSetCB;
    memset(&DMXSetCB, 0 , sizeof(DMX_TTX_SET_CB));
    
    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
            
    DMXSetCB.fn = fn;
                                                        
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_SetCB,(void*)&DMXSetCB) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_SetCB fail\n");  
        return;
    }
#else
    _MDrv_DMX_TTX_SetCB(fn);
#endif
}

MS_BOOL MDrv_DMX_TTX_PacketBufferIsEmpty(void)
{
#if (VBI_TTX_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                                                            
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_PacketBufferIsEmpty,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_PacketBufferIsEmpty fail\n");  
        return FALSE;
    }
    return CheckTureFalse.bCheck;
#else
    return _MDrv_DMX_TTX_PacketBufferIsEmpty();
#endif
}

MS_BOOL MDrv_DMX_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length)
{
#if (VBI_TTX_UTOPIA20)
    VBI_TTX_GET_PACKETS GetPackets;
    memset(&GetPackets, 0 , sizeof(VBI_TTX_GET_PACKETS));
    
    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
            
    GetPackets.dataAddr = dataAddr;
    GetPackets.length   = length;
                                                                
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_GetPackets,(void*)&GetPackets) != UTOPIA_STATUS_SUCCESS)
    {
        //ULOGE("VBI", "Ioctl MDrv_DMX_TTX_GetPackets fail\n");  
        return FALSE;
    }
    return TRUE;
#else
    return _MDrv_DMX_TTX_GetPackets(dataAddr, length);
#endif
}

MS_BOOL MDrv_DMX_TTX_GetPacket(MS_PHY *packetAddress)
{
#if (VBI_TTX_UTOPIA20)
    VBI_TTX_GET_PACKET GetPacket;
    memset(&GetPacket, 0 , sizeof(VBI_TTX_GET_PACKET));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                
    GetPacket.packetAddress = packetAddress;
                                                                    
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_GetPacket,(void*)&GetPacket) != UTOPIA_STATUS_SUCCESS)
    {
        //ULOGE("VBI", "Ioctl MDrv_DMX_TTX_GetPacket fail\n");  
        return FALSE;
    }
    return TRUE;
#else
    return _MDrv_DMX_TTX_GetPacket(packetAddress);
#endif
}

/***********************************************************************
 * FUNCTION: MDrv_DMX_TTX_PacketBufferIsOverflow
 *
 * DESCRIPTION:
 *   Check if there is a packet buffer overflow. If there is an overflow,
 *   the the packet buffer should be cleared from the reading task.
 *
 * RETURN:
 *   TRUE if there is packet buffer overflow,
 *   NULL otherwise.
 ***********************************************************************/
MS_BOOL MDrv_DMX_TTX_PacketBufferIsOverflow( void )
{
#if (VBI_TTX_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                                                                                
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_PacketBufferIsOverflow,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_PacketBufferIsOverflow fail\n");  
        return FALSE;
    }
    return CheckTureFalse.bCheck;
#else
    return _MDrv_DMX_TTX_PacketBufferIsOverflow();
#endif
}

/***********************************************************************
 * FUNCTION: MDrv_DMX_TTX_PacketBufferGetNoOfOverflows
 *
 * DESCRIPTION:
 *   Resuren the nomber of packet buffer overflows since the last reset
 *   or creation.
 *
 * RETURN:
 *   The number of packet buffer overflows.
 ***********************************************************************/
MS_U16 MDrv_DMX_TTX_PacketBufferGetNoOfOverflows( void )
{
#if (VBI_TTX_UTOPIA20)
    VBI_GET_DATA GetData;
    memset(&GetData, 0 , sizeof(VBI_GET_DATA));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
            
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows,(void*)&GetData) != UTOPIA_STATUS_SUCCESS)
    {
        //ULOGE("VBI", "Ioctl MDrv_DMX_TTX_PacketBufferGetNoOfOverflows fail\n");  
        return FALSE;
    }
    return GetData.u16data;
#else
    return _MDrv_DMX_TTX_PacketBufferGetNoOfOverflows();
#endif
}

/****************************************************************************
*@brief            : Suspend DMX_TTX driver
*@return TRUE - Success
*@return FALSE - Failure
*@note
*@Save DMX_TTX driver states to DRAM
****************************************************************************/
MS_BOOL MDrv_DMX_TTX_Suspend(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
    
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_Suspend,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_Suspend fail\n");
        return FALSE;
    }
    return CheckTureFalse.bCheck;
#else
    return _MDrv_DMX_TTX_Suspend();
#endif
}

/****************************************************************************
*@brief            :  Resume DMX_TTX driver
*@return TRUE - Success
*@return FALSE - Failure
*@note
*@Save DMX_TTX driver states to DRAM
*@Restore DMX_TTX driver states from DRAM
****************************************************************************/
MS_BOOL MDrv_DMX_TTX_Resume(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
    
    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_Resume,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_Resume fail\n");
        return FALSE;
    }
    return CheckTureFalse.bCheck;
#else
    return _MDrv_DMX_TTX_Resume();
#endif
}

/****************************************************************************
*@brief            : Set DMX_TTX suspend & resume
*@param u16PowerState              \b IN: DMX_TTX power state
*@return TRUE - Success
*@return FALSE - Failure
*@note
****************************************************************************/
MS_U32 MDrv_DMX_TTX_SetPowerState(EN_POWER_MODE u16PowerState)
{
#if (VBI_UTOPIA20)
    VBI_GET_RESULT GetResult;
    memset(&GetResult, 0 , sizeof(VBI_GET_RESULT));

    if (NULL == pInstantVbiTTX)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }

    GetResult.u16PowerState = u16PowerState;

    if(UtopiaIoctl(pInstantVbiTTX, MDrv_CMD_DMX_TTX_SetPowerState,(void*)&GetResult) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_DMX_TTX_SetPowerState fail\n");
        return FALSE;
    }
    return GetResult.u32result;
#else
    return _MDrv_VBI_SetPowerState();
#endif
}

#undef _DRV_DMX_TTX_C
