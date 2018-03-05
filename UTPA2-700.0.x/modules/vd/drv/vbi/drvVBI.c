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

#define _DRV_VBI_C

// Common Definition
#if defined(MSOS_TYPE_LINUX_KERNEL)
//    #include <linux/slab.h>
    #include <linux/string.h> //for memcpy, memset
#else
    #include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "ULog.h"
// Internal Definition
#include "halVBI.h"
#include "drvVBI.h"
#include "drvMMIO.h"
#include "drvSYS.h"

#include "utopia.h"

#if (VBI_UTOPIA20)
#include "drvVBI_v2.h"
#endif

#include "drvVBI_private.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define _BIT0       BIT(0)
#define _BIT1       BIT(1)
#define _BIT2       BIT(2)
#define _BIT3       BIT(3)
#define _BIT4       BIT(4)
#define _BIT5       BIT(5)
#define _BIT6       BIT(6)
#define _BIT7       BIT(7)
#define _BIT8       BIT(8)
#define _BIT9       BIT(9)
#define _BIT10      BIT(10)
#define _BIT11      BIT(11)
#define _BIT12      BIT(12)
#define _BIT13      BIT(13)
#define _BIT14      BIT(14)
#define _BIT15      BIT(15)

#define UNUSED(x)       (void)((x)=(x))

#define DEFAULT_WAIT_TIME MSOS_WAIT_FOREVER  //ms

static VBI_CB_FN _pFN = NULL;

static MSIF_Version _drv_VBI_version = {
    .MW = { VBI_VER },
};

static VBI_RING_BUFFER rb_handle;
static VBI_DrvInfo     _drvInfo;
static VBI_DrvStatus   _drvStatus;

static VBI_StoreInfo _pStoreInfo = {
    ._bSuspend = FALSE,
    .eInitType = E_VBI_TELETEXT,

    // TTX buffer address & buffer size
    ._ptrVBIRiuBaseAddr = 0xFFFFFFFFUL,
    ._TTXBufAddr = 0xFFFFFFFFUL,
    ._TTXBufLen  = 0,

    // CC buffer address & buffer size
    ._CCBufAddr = 0xFFFFFFFFUL,
    ._CCBufLen  = 0,

    // callback info
    ._cbBufferAddr   = 0xFFFFFFFFUL,
    ._cbBufferLength = 0,
    ._pCbVABuff      = NULL,

    ._bTTXSupported = FALSE,
    ._bVBIInit      = FALSE,
    ._u16VBIDbgSwitch = 0,
};
 
// Mutex & Lock
static MS_S32 _s32VBIMutex = -1;

#if (VBI_UTOPIA20)
static void* pInstantVbi   = NULL;
static void* pAttributeVbi = NULL;
#endif

#define VIDEO_TYPE_CNT          5
#define VIDEO_REGISTER_CNT      8
static MS_U8 video_standard_vbi_settings[VIDEO_TYPE_CNT][VIDEO_REGISTER_CNT + 1] = {
   // type                      41h[7:0]   42h[5:0]    44h[7:0]    4Bh[5:0]    4Dh[7:0]    50h[4:0]    51h[4:0]    53h[4:0]
    {VBI_VIDEO_NTSC,            0x52,      0x1c,       0x39,       0x24,       0x8e,       0xd2,       0x12,       0x90},  // NTSC
    {VBI_VIDEO_PAL_M,           0x52,      0x1c,       0x39,       0x26,       0x8e,       0xd2,       0x12,       0x90},  // PAL-M
    {VBI_VIDEO_PAL_NC,          0x52,      0x1c,       0x39,       0x16,       0x8e,       0xd2,       0x12,       0x90},  // PAL-NC
    {VBI_VIDEO_NTSC443_PAL60,   0x52,      0x23,       0x47,       0x32,       0xb0,       0xd2,       0x12,       0xb2},  // NTSC-443, PAL-60
    {VBI_VIDEO_PAL,             0xb5,      0x23,       0x47,       0x22,       0xb0,       0xd5,       0x15,       0xb2},  // PAL
};
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_VBI_ISR()
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
////////////////////////////////////////////////////////////////////////////////
#if defined (MSOS_TYPE_NOS)
static void _MDrv_VBI_ISR(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data)
#else // Ecos and Linux
static void _MDrv_VBI_ISR( InterruptNum eIntNum )
#endif
{
    #if defined (MSOS_TYPE_NOS)
        UNUSED(pHalReg);
        UNUSED(u32Data);
    #else // Ecos and Linux
        UNUSED(eIntNum);
    #endif

#if defined(MSOS_TYPE_LINUX_KERNEL)
    if(_pStoreInfo._pCbVABuff!=NULL)
#else
    if(_pFN!=NULL && _pStoreInfo._pCbVABuff!=NULL)
#endif
    {
        MS_U32 length = _pStoreInfo._cbBufferLength;

        switch(_drvStatus.eInitType)
        {
            case E_VBI_TELETEXT:
                if(_MDrv_VBI_TTX_GetPackets(_pStoreInfo._cbBufferAddr, &length)==TRUE)
                {
#if defined(MSOS_TYPE_LINUX_KERNEL)
#else
                    (*_pFN)(_pStoreInfo._pCbVABuff, length);
#endif
                }
                break;

            case E_VBI_CC:
                // need to implement
                break;

            default:
                break;
        }
    }

    MsOS_EnableInterrupt(E_INT_IRQ_VBI);
}

/****************************************************************************
 *@brief            : Update TTX Write Pointer
 *@param
 *@return
 *  - TRUE : update write pointer successfully.
 *  - FALSE: update write pointer unsuccessfully.
 ****************************************************************************/
static MS_BOOL _MDrv_VBI_TTX_UpdateWritePointer(void)
{
    MS_S32 NumberOfFreePackets;

    if(VBI_TTX_CheckCircuitReady()==FALSE)
    {
        return FALSE;
    }

    rb_handle.WritePacketNumber = VBI_TTX_GetPacketCount();

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

    if(rb_handle.WritePacketNumber==1)
    {
        if(rb_handle.pkt_received)
        {
            rb_handle.WritePacketNumber = _pStoreInfo._TTXBufLen;
        }
        else
        {
            rb_handle.WritePacketNumber = 0;
        }
    }
    else
    {
        if(rb_handle.WritePacketNumber==0)
        {
            rb_handle.ReadPacketNumber = 0;
            return FALSE;
        }

        rb_handle.WritePacketNumber--;
        rb_handle.pkt_received = TRUE;
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_VBI_GetStatus(VBI_DrvStatus *pDrvStatus)
{
    memcpy(pDrvStatus, &_drvStatus, sizeof(VBI_DrvStatus));
    return TRUE;
}

MS_BOOL _MDrv_VBI_Init(EN_VBI_CMD cmd, VBI_INIT_TYPE type)
{
    MS_PHY phyNonPMBankSize;
    static char _vbiSyncMutexName[] = "OS_VBI_MUTEX";
        
    if(cmd == VBI_INIT)
    {
        if (type == E_VBI_WSS)
        {
            MS_VIRT ptrVBIRiuBaseAddrForWSS = 0;
            
            // get MMIO base
            if (MDrv_MMIO_GetBASE( &ptrVBIRiuBaseAddrForWSS, &phyNonPMBankSize, MS_MODULE_VBI) != TRUE)
            {
                return FALSE;
            }
            else
            {
                //printf("MDrv_PNL_Init u32PnlRiuBaseAddr = %lx\n", gu32PnlRiuBaseAddr);
            }

            VBI_WSSInit(ptrVBIRiuBaseAddrForWSS);

            return TRUE;
        }
            	
        _s32VBIMutex = //OS_CREATE_MUTEX(VBI);
                    MsOS_CreateMutex(E_MSOS_FIFO, _vbiSyncMutexName, MSOS_PROCESS_PRIVATE);
    
        if(_s32VBIMutex < 0)
        {
            return FALSE;
        }
    
        // get MMIO base
        if(MDrv_MMIO_GetBASE( &_pStoreInfo._ptrVBIRiuBaseAddr, &phyNonPMBankSize, MS_MODULE_VBI ) != TRUE)
        {
            OS_DELETE_MUTEX(_s32VBIMutex);
            return FALSE;
        }
        else
        {
            //ULOGD("VBI", "MDrv_PNL_Init u32PnlRiuBaseAddr = %lx\n", gu32PnlRiuBaseAddr);
        }
    
        if(type==E_VBI_TELETEXT)
        {
            _pStoreInfo._bTTXSupported = MDrv_SYS_Query(E_SYS_QUERY_TTXNICAM_SUPPORTED);
            if(_pStoreInfo._bTTXSupported == TRUE)
            {            
                VBI_TTXInit(_pStoreInfo._ptrVBIRiuBaseAddr);
            }
            else
            {
                ULOGE("VBI", "[VBI ERROR] TELETEXT not support in this chip !!!\n");
                return FALSE;
            }
        }
        else
        {
            ;   // should call CC VBI init
        }
    
        _drvStatus.eInitType = type;    
        _pStoreInfo._bVBIInit = TRUE;
        _pStoreInfo._bSuspend = FALSE;
    
        
#if defined (MSOS_TYPE_NOS)
        // irq init
        MsOS_AttachInterrupt(E_INT_IRQ_VBI, (InterruptCb)_MDrv_VBI_ISR );
        MsOS_EnableInterrupt(E_INT_IRQ_VBI);
#else // Ecos and Linux
        MsOS_AttachInterrupt(E_INT_IRQ_VBI, _MDrv_VBI_ISR );
        MsOS_EnableInterrupt(E_INT_IRQ_VBI);
#endif
    }
    else
    {
        if(type==E_VBI_TELETEXT)
        {
            _pStoreInfo._bTTXSupported = MDrv_SYS_Query(E_SYS_QUERY_TTXNICAM_SUPPORTED);
            if(_pStoreInfo._bTTXSupported == TRUE)
            {            
                VBI_TTXInit(_pStoreInfo._ptrVBIRiuBaseAddr);
            }
            else
            {
                ULOGE("VBI", "[VBI ERROR] TELETEXT not support in this chip !!!\n");
                return FALSE;
            }
        }
        else
        {
            ;   // should call CC VBI init
        }
            
        MsOS_EnableInterrupt(E_INT_IRQ_VBI);
    }
    
    return TRUE;
}

MS_BOOL _MDrv_VBI_Exit(EN_VBI_CMD cmd)
{
    _pStoreInfo._bVBIInit = FALSE;
    if(cmd == VBI_EXIT) // vbi exit
    {
        if (_s32VBIMutex != -1)
        {
            OS_OBTAIN_MUTEX(_s32VBIMutex, DEFAULT_WAIT_TIME);
            OS_RELEASE_MUTEX(_s32VBIMutex);
            OS_DELETE_MUTEX(_s32VBIMutex);
        }
    
        MsOS_DisableInterrupt(E_INT_IRQ_VBI);
        MsOS_DetachInterrupt(E_INT_IRQ_VBI);
    }
    else // vbi suspend
    {
        MsOS_DisableInterrupt(E_INT_IRQ_VBI);
    }
    
    return TRUE;
}

void _MDrv_VBI_RegisterCB(VBI_CB_FN pFN, MS_PHY bufferAddr, MS_U32 length)
{
    _pFN = pFN;
    _pStoreInfo._cbBufferAddr = bufferAddr;
    _pStoreInfo._cbBufferLength = (length/VBI_TTX_DATA_LENGTH)*VBI_TTX_DATA_LENGTH;
    _pStoreInfo._pCbVABuff = (MS_U8 *) MS_PA2KSEG1(_pStoreInfo._cbBufferAddr);
}

void _MDrv_VBI_RingBuffer_Reset(void)
{
    memset((MS_U8 *) MS_PA2KSEG1(_pStoreInfo._TTXBufAddr), 0x20, _pStoreInfo._TTXBufLen*VBI_TTX_DATA_LENGTH);

    OS_OBTAIN_MUTEX(_s32VBIMutex, DEFAULT_WAIT_TIME);

    rb_handle.ReadPacketNumber = VBI_TTX_GetPacketCount();
    rb_handle.WritePacketNumber = 0;
    rb_handle.pkt_received = FALSE;
    rb_handle.PacketBufferOverflow = FALSE;
    rb_handle.NoOfPacketBufferOverflows = 0;

    OS_RELEASE_MUTEX(_s32VBIMutex);
}

void _MDrv_VBI_InitializeTTXSlicer(MS_PHY bufferAddr, MS_U16 packetCount)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        ULOGE("VBI", "TTX not support\n");
        return;
    }

    VBI_TTX_InitSlicer(bufferAddr, packetCount);
    _pStoreInfo._TTXBufAddr = bufferAddr;
    _pStoreInfo._TTXBufLen = packetCount;
    _MDrv_VBI_RingBuffer_Reset();
}

void _MDrv_VBI_EnableTTXSlicer(MS_BOOL bEnable)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return;
    }

    VBI_TTX_EnableSlicer(bEnable);
}

MS_BOOL _MDrv_VBI_IsVPS_Ready(void)
{
    MS_U8 regval;
        
    regval = VBI_TTX_GetHardware_Indication();

    if(regval & _BIT4)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL _MDrv_VBI_IsTTX_Ready(void)
{
    MS_U8 regval;
    
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return FALSE;
    }
       
    regval = VBI_TTX_GetHardware_Indication();

    if(regval & _BIT1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL _MDrv_VBI_IsWSS_Ready(void)
{
    MS_U8 regval;
    
    regval = VBI_TTX_GetHardware_Indication();

    if(regval & _BIT6)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U16 _MDrv_VBI_GetWSS_Data(void)
{        
    return VBI_GetWSS_Data();
}

void _MDrv_VBI_GetRawVPS_Data(MS_U8 *byte0, MS_U8 *byte1, MS_U8 *byte2, MS_U8 *byte3)
{
    VBI_GetVPS_Data(byte0, byte1, byte2, byte3);
}

void _MDrv_VBI_GetVPS_Data(MS_U8 *lowerWord, MS_U8 *higherWord)
{
    MS_U8 ucLoop;
    MS_U8 ucVpsDataH, ucVpsDataL;
    MS_U8 VpsByte1, VpsByte2, VpsByte3, VpsByte4;

    VBI_GetVPS_Data(&VpsByte1, &VpsByte2, &VpsByte3, &VpsByte4);

    ucVpsDataH = 0x00;
    ucVpsDataL = 0x00;

     for (ucLoop = _BIT7; ucLoop!=_BIT1; ucLoop>>=1)	// extract byte 14[2:7] as ucVpsDataL[5:0]
    {
        ucVpsDataL >>= 1;
        if (VpsByte2 & ucLoop)
            ucVpsDataL |= _BIT7;
    } // for

    for (ucLoop = _BIT1; ucLoop!=0; ucLoop>>=1)		// extract byte 11[0:1] as ucVpsDataL[7:6]
    {
        ucVpsDataL >>= 1;
        if (VpsByte4 & ucLoop)
            ucVpsDataL |= _BIT7;
    } // for

    for (ucLoop = _BIT1; ucLoop!=0; ucLoop>>=1)		// extract byte 14[0:1] as ucVpsDataH[1:0]
    {
        ucVpsDataH >>= 1;
        if (VpsByte2 & ucLoop)
            ucVpsDataH |= _BIT3;
    } // for

    for (ucLoop = _BIT7; ucLoop!=_BIT5; ucLoop>>=1)	// extract byte-13[6:7] as ucVpsDataH[3:2]
    {
        ucVpsDataH >>= 1;
        if (VpsByte1 & ucLoop)
            ucVpsDataH |= _BIT3;
    } // for

    *lowerWord = ucVpsDataL;
    *higherWord = ucVpsDataH;
}

void _MDrv_VBI_SetVideoStandard(VBI_VIDEO_STANDARD eStandard)
{           
    switch(eStandard)
    {
        case VBI_VIDEO_SECAM:
            VBI_Set_Secam_VideoStandard();
            break;

        case VBI_VIDEO_PAL_NC:
            VBI_Set_PalNC_VideoStandard();
            break;

        default:
            ULOGD("VBI", "[%s] not support standard 0x%x, use PAL instead\n", __FUNCTION__, (unsigned int)eStandard);
            VBI_Set_Pal_VideoStandard();
            break;
    }
}

MS_BOOL _MDrv_VBI_TTX_PacketBufferIsEmpty(void)
{
    if(_MDrv_VBI_TTX_UpdateWritePointer()==FALSE)   // if overflow or slicer is not ready, define it is buffer empty
        return TRUE;

    if (rb_handle.WritePacketNumber - rb_handle.ReadPacketNumber) /* no of packets */
        return FALSE;

    return TRUE;
}

MS_BOOL _MDrv_VBI_TTX_CheckCircuitReady(void)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return FALSE;
    }
    return VBI_TTX_CheckCircuitReady();
}

MS_U16 _MDrv_VBI_TTX_GetPacketCount(void)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return 0;
    }
    return VBI_TTX_GetPacketCount();
}

MS_BOOL _MDrv_VBI_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length)
{
    MS_U8 *pData = (MS_U8 *) MS_PA2KSEG1(dataAddr);
    MS_BOOL ret = FALSE;

    if((*length)==0)
    {
        return FALSE;
    }

    OS_OBTAIN_MUTEX(_s32VBIMutex, DEFAULT_WAIT_TIME);

    if(_MDrv_VBI_TTX_PacketBufferIsEmpty()==FALSE)
    {
        MS_PHY packetAddress;
        MS_U32 data_length;
        MS_U32 data_length2;

        packetAddress = _pStoreInfo._TTXBufAddr + (rb_handle.ReadPacketNumber) * VBI_TTX_DATA_LENGTH;

        if(rb_handle.ReadPacketNumber > rb_handle.WritePacketNumber)
        {
            // HEAD <--- 2 ---> W ------R <--- 1 ---> TAIL

            // copy region 1
            data_length = VBI_TTX_DATA_LENGTH*(_pStoreInfo._TTXBufLen - rb_handle.ReadPacketNumber);

            if(data_length > (*length))
            {
                data_length = (*length);
            }

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
            {
                data_length2 = ((*length) - data_length);
            }

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
            {
                data_length = (*length);
            }

            memcpy(pData, (MS_U8 *) MS_PA2KSEG1(packetAddress), data_length);

            rb_handle.ReadPacketNumber += data_length/VBI_TTX_DATA_LENGTH;
            (*length) = data_length;

            ret = TRUE;
        }
    }

GET_PACKETS_EXIT:
    OS_RELEASE_MUTEX(_s32VBIMutex);
    return ret;
}

MS_BOOL _MDrv_VBI_TTX_GetPacket(MS_PHY *packetAddress)
{
    MS_BOOL ret = FALSE;

    OS_OBTAIN_MUTEX(_s32VBIMutex, DEFAULT_WAIT_TIME);

    if (_MDrv_VBI_TTX_PacketBufferIsEmpty()==FALSE)
    {
        rb_handle.ReadPacketNumber++;

        // Venus, 0 -> 1 -> 2 ->3 -> N-1 -> N -> 1 ->2 -> 3 ...
        if ( rb_handle.ReadPacketNumber > _pStoreInfo._TTXBufLen )
        {
            rb_handle.ReadPacketNumber = 1;
        }

        *packetAddress = _pStoreInfo._TTXBufAddr + (rb_handle.ReadPacketNumber - 1) * VBI_TTX_DATA_LENGTH;

        ret = TRUE;
    }

    OS_RELEASE_MUTEX(_s32VBIMutex);
    return ret;
}

MS_BOOL _MDrv_VBI_TTX_PacketBufferIsOverflow( void )
{
    return rb_handle.PacketBufferOverflow;
}

MS_U16 _MDrv_VBI_TTX_PacketBufferGetNoOfOverflows( void )
{
    return rb_handle.NoOfPacketBufferOverflows;
}

void _MDrv_VBI_TTX_EnableLine(MS_U16 StartLine, MS_U16 EndLine)
{
    VBI_TTX_EnableLine(StartLine, EndLine);
}

MS_BOOL _MDrv_VBI_SyncMemory( MS_U32 u32Start, MS_U32 u32Size )
{
    MsOS_ReadMemory();
    return MsOS_Dcache_Flush(u32Start, u32Size);
}

void _MDrv_VBI_CC_InitSlicer(MS_VIRT u32RiuAddr, MS_PHY bufferAddr, MS_U16 packetCount)
{
    MS_PHY phyNonPMBankSize;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &_pStoreInfo._ptrVBIRiuBaseAddr, &phyNonPMBankSize, MS_MODULE_VBI ) != TRUE)
    {
        return;
    }
    else
    {
        //ULOGD("VBI", "MDrv_PNL_Init u32PnlRiuBaseAddr = %lx\n", _gu32PnlRiuBaseAddr);
    }

    if(bufferAddr != 0 && packetCount != 0)
    {
        if(_pStoreInfo._ptrVBIRiuBaseAddr != 0)
        {
            u32RiuAddr = _pStoreInfo._ptrVBIRiuBaseAddr;
        }
        
        _pStoreInfo._CCBufAddr = bufferAddr;
        _pStoreInfo._CCBufLen  = packetCount;
        
        VBI_CC_Init(u32RiuAddr, bufferAddr, packetCount);
    }
}

void _MDrv_VBI_CC_InitYPbYr(MS_U8 cvbs_no)
{
    VBI_CC_YPbPr_Init(cvbs_no);
}

MS_U8 _MDrv_VBI_CC_SetDataRate(MS_U8 *ptable)
{
    if(ptable != NULL)
    {
        return (VBI_CC_DataRateSet(ptable));
    }

    return FALSE;
}

MS_BOOL _MDrv_VBI_CC_SetSCWindowLen(MS_U8 u8Len)
{
    return VBI_CC_SetSCWindowLen(u8Len);
}

MS_BOOL _MDrv_VBI_CC_SetStandard(VBI_VIDEO_STANDARD eStandard)
{
    MS_U8 i=0;

    // Get the video system's type
    for(i=0; i<VIDEO_TYPE_CNT; i++)
    {
        if(video_standard_vbi_settings[i][0] == eStandard) 
            break;
    }

    if(i >= VIDEO_TYPE_CNT)
    {
        return FALSE;
    }

    VBI_CC_DataRateSet(video_standard_vbi_settings[i]);
    VBI_CC_SetSCWindowLen(video_standard_vbi_settings[i][8]); // BK_VBI_53
    return TRUE;
}

MS_U32 _MDrv_VBI_CC_GetInfo(MS_U32 selector)
{
    // Functionality select
    switch (selector)
    {
        case VBI_CC_PACKET_COUNT:
            return  ((MS_U32)VBI_CC_GetPacketCnt());

            break;

        case VBI_CC_BYTE_FOUND_INDICATION:
            return  ((MS_U32)VBI_CC_GetByteFoundIndication());

            break;

        case VBI_CC_DATA_GET:
            return  ((MS_U32)VBI_CC_GetPacket());   // odd_filed_data + even_field_data

            break;

        default:
            ;
    }

    return  (MS_U32)VBI_ERRORCODE_SUCCESS;
}

void _MDrv_VBI_CC_SetFrameCnt(MS_U8 cnt)
{
    VBI_SetCCFrameCnt(cnt);
}

void _MDrv_VBI_CC_EnableSlicer(MS_BOOL bEnable)
{
    VBI_CC_EnableSlicer(bEnable);
}

void _MDrv_VBI_CC_EnableLine(MS_U16 StartLine, MS_U16 EndLine, MS_U8 mode)
{
    // set line
    VBI_CC_SetCCLine(StartLine, EndLine, mode);
}

void _MDrv_VBI_WSS_SetVpsByteNum(MS_U8 cnt)
{
    VBI_SetWssVpsByteNum(cnt);
}

MS_BOOL _MDrv_VBI_Suspend(void)
{
    if((_pStoreInfo._bSuspend != FALSE) || (_s32VBIMutex == -1))
    {
        return FALSE;
    }
    _pStoreInfo._bSuspend = TRUE;
    _pStoreInfo.eInitType = _drvStatus.eInitType;

    VBI_RegStateStore();
    _MDrv_VBI_Exit(VBI_SUSPEND);

    return TRUE;
}

MS_BOOL _MDrv_VBI_Resume(void)
{
    if((_pStoreInfo._bSuspend != TRUE) || (_s32VBIMutex == -1))
    {
        return FALSE;
    }
    _pStoreInfo._bSuspend = FALSE;

    _MDrv_VBI_Init(VBI_RESUME, _pStoreInfo.eInitType);

    if((_pStoreInfo.eInitType == E_VBI_CC) && ((_pStoreInfo._CCBufAddr != 0xFFFFFFFFUL) && (_pStoreInfo._CCBufLen != 0)))
        VBI_CC_Init(_pStoreInfo._ptrVBIRiuBaseAddr, _pStoreInfo._CCBufAddr, _pStoreInfo._CCBufLen);

    if((_pStoreInfo.eInitType == E_VBI_TELETEXT) && ((_pStoreInfo._TTXBufAddr != 0xFFFFFFFFUL) && (_pStoreInfo._TTXBufLen != 0)))
    {
        _MDrv_VBI_InitializeTTXSlicer(_pStoreInfo._TTXBufAddr, _pStoreInfo._TTXBufLen);
    }

    VBI_RegStateRestore();

    return TRUE;
}

MS_U32 _MDrv_VBI_SetPowerState(EN_POWER_MODE u16PowerState)
{
    MS_U16 _ret = FALSE;

    switch (u16PowerState)
    {
        case E_POWER_SUSPEND:
            _ret = _MDrv_VBI_Suspend();
            break;

        case E_POWER_RESUME:
            _ret = _MDrv_VBI_Resume();
            break;

        case E_POWER_MECHANICAL:
        case E_POWER_SOFT_OFF:
        default:
            ULOGE("VBI", "[%s] %d Power state not support!!\n", __FUNCTION__, __LINE__);
            break;
    }

    return ((TRUE == _ret)? UTOPIA_STATUS_SUCCESS: UTOPIA_STATUS_FAIL);
}

MS_BOOL _MDrv_VBI_ProtectMemory(MS_BOOL bEnable, MS_PHY phyAddr, MS_U32 u32Size)
{
    return VBI_ProtectMemory(bEnable, phyAddr, u32Size);
}

/****************************************************************************
 *@brief            : Get VBI Library Version
 *@param ppVersion  : [OUT] library version
 *@return
 *  - TRUE : get library version successfully.
 *  - FALSE: get library version unsuccessfully.
 ****************************************************************************/
MS_BOOL MDrv_VBI_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_VBI_version;

    return TRUE;
}

/****************************************************************************
 *@brief            : Get VBI Information
 *@param
 *@return
 *@see VBI_DrvInfo
 ****************************************************************************/
const VBI_DrvInfo * MDrv_VBI_GetInfo(void)
{
    return &_drvInfo;
}

/****************************************************************************
 *@brief            : Get VBI Driver Status
 *@param pDrvStatus : [OUT] driver status
 *@return
 *  - TRUE : get status successfully.
 *  - FALSE: get status unsuccessfully.
 ****************************************************************************/
MS_BOOL MDrv_VBI_GetStatus(VBI_DrvStatus *pDrvStatus)
{
    if(_pStoreInfo._bVBIInit == FALSE)
    {
        return FALSE;
    }
    
#if (VBI_UTOPIA20)
    VBI_GETSTATUS_PARAM GetStatus;
    memset(&GetStatus, 0 , sizeof(VBI_GETSTATUS_PARAM));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
    
    GetStatus.pDrvStatus = pDrvStatus;
        
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_GetStatus,(void*)&GetStatus) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_GetStatus fail\n");
        return FALSE;
    }
    return TRUE;
#else
    return _MDrv_VBI_GetStatus(pDrvStatus);
#endif
}

/****************************************************************************
 *@brief                : Set VBI Debug Level
 *@param u16DbgSwitch   : [IN] debug level
 *@return
 *  - TRUE : set debug level successfully.
 *  - FALSE: set debug level unsuccessfully.
 ****************************************************************************/
MS_BOOL MDrv_VBI_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    _pStoreInfo._u16VBIDbgSwitch = u16DbgSwitch;

    return TRUE;
}

/****************************************************************************
 *@brief            : Initialize VBI
 *@param type       : [IN] VBI type (Teletext/CC)
 *@return
 *  - TRUE : initialize VBI successfully.
 *  - FALSE: initialize VBI unsuccessfully.
 ****************************************************************************/
MS_BOOL MDrv_VBI_Init(VBI_INIT_TYPE type)
{
#if (VBI_UTOPIA20)
    VBI_INIT_TYPE_PARAM InitType;
    memset(&InitType, 0 , sizeof(VBI_INIT_TYPE_PARAM));
    
    if (NULL == pInstantVbi)
    {        
        if(UtopiaOpen(MODULE_VBI, (void**)&pInstantVbi, 0, pAttributeVbi) ==  UTOPIA_STATUS_FAIL)
        {
            ULOGE("VBI", "UtopiaOpen VBI fail\n");
            return FALSE;
        }
    }
    else
    {
        ULOGE("VBI", "VBI already Opened\n");
        return FALSE;
    }

    InitType.cmd = VBI_INIT;
    InitType.type = type;
    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_Init,(void*)&InitType) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_Init fail\n");
        return FALSE;
    }
    return TRUE;
#else
    return _MDrv_VBI_Init(VBI_INIT, type);
#endif
}

/****************************************************************************
 *@brief            : VBI Exit
 *@return
 *  - TRUE : VBI exit successfully.
 *  - FALSE: VBI exit unsuccessfully.
 ****************************************************************************/
MS_BOOL MDrv_VBI_Exit(void)
{    
#if (VBI_UTOPIA20)   
    VBI_CMD Cmd;
    memset(&Cmd, 0 , sizeof(VBI_CMD));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
        
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_Exit,(void*)&Cmd) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_Exit fail\n");
        return FALSE;
    } 
    return TRUE;
#else
    return _MDrv_VBI_Exit(VBI_EXIT);
#endif
}


/****************************************************************************
 *@brief            : Set Callback to VBI driver, the CB will be called if VBI interrupt is catched
 *@param pFN : [IN] call back function pointer.
 *@param bufferAddr : [IN] the physical address of buffer which will store VBI data and pass pointer to CB
 *@param length : [IN] the data lebgth of th input buffer. Note: it should be the multiple of VBI_TTX_DATA_LENGTH
 *@return
 ****************************************************************************/
void MDrv_VBI_RegisterCB(VBI_CB_FN pFN, MS_PHY bufferAddr, MS_U32 length)
{
#if (VBI_UTOPIA20)
    VBI_REGISTER_CB RegisterCB;
    memset(&RegisterCB, 0 , sizeof(VBI_REGISTER_CB));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
    
    RegisterCB.pFN        = pFN;
    RegisterCB.bufferAddr = bufferAddr; 
    RegisterCB.length     = length;
                
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_RegisterCB,(void*)&RegisterCB) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_RegisterCB fail\n");
    }
#else
    _MDrv_VBI_RegisterCB(pFN, bufferAddr, length);
#endif
}

/****************************************************************************
 *@brief            : Reset TTX Ring Buffer
 *@param
 *@return
 ****************************************************************************/
void MDrv_VBI_RingBuffer_Reset(void)
{
#if (VBI_UTOPIA20)
    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_RingBuffer_Reset, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_RingBuffer_Reset fail\n");
    }
#else
    _MDrv_VBI_RingBuffer_Reset();
#endif
}

/****************************************************************************
 *@brief            : Initialize TTX Slicer
 *@param bufferAddr : [IN] TTX buffer address
 *@param packetCount: [IN] TTX packet count
 *@return
 ****************************************************************************/
void MDrv_VBI_InitializeTTXSlicer(MS_PHY bufferAddr, MS_U16 packetCount)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return;
    }
    
#if (VBI_UTOPIA20)
    VBI_INITIALIZER_TTX_SLICER InitializerTTXSlicer;
    memset(&InitializerTTXSlicer, 0 , sizeof(VBI_INITIALIZER_TTX_SLICER));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
    
    InitializerTTXSlicer.bufferAddr  = bufferAddr;
    InitializerTTXSlicer.packetCount = packetCount;
    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_InitializeTTXSlicer,(void*)&InitializerTTXSlicer) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_InitializeTTXSlicer fail\n");
    }
#else
    _MDrv_VBI_InitializeTTXSlicer(bufferAddr, packetCount);
#endif
}

/****************************************************************************
 *@brief            : Enable TTX Slicer
 *@param bEnable    : [IN] enable or disable TTX slicer
 *@return
 ****************************************************************************/
void MDrv_VBI_EnableTTXSlicer(MS_BOOL bEnable)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return;
    }
    
#if (VBI_UTOPIA20)
    VBI_ENABLE_TTX_SLICER EnableTTXSlicer;
    memset(&EnableTTXSlicer, 0 , sizeof(VBI_ENABLE_TTX_SLICER));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
    
    EnableTTXSlicer.bEnable = bEnable;
        
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_EnableTTXSlicer,(void*)&EnableTTXSlicer) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_EnableTTXSlicer fail\n");
    }
#else
    _MDrv_VBI_EnableTTXSlicer(bEnable);
#endif
}

/****************************************************************************
 *@brief            : VPS Is Ready or Not
 *@param
 *@return
 *  - TRUE : VPS is ready.
 *  - FALSE: VPS is not ready.
 ****************************************************************************/
MS_BOOL MDrv_VBI_IsVPS_Ready(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_IsVPS_Ready,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_IsVPS_Ready fail\n");        
        return FALSE;
    }    
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_IsVPS_Ready();
#endif
}

/****************************************************************************
 *@brief            : TTX Is Ready or Not
 *@param
 *@return
 *  - TRUE : TTX is ready.
 *  - FALSE: TTX is not ready.
 ****************************************************************************/
MS_BOOL MDrv_VBI_IsTTX_Ready(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_IsTTX_Ready,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_IsTTX_Ready fail\n");        
        return FALSE;
    }    
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_IsTTX_Ready();
#endif
}

/****************************************************************************
 *@brief            : WSS Is Ready or Not
 *@param
 *@return
 *  - TRUE : WSS is ready.
 *  - FALSE: WSS is not ready.
 ****************************************************************************/
MS_BOOL MDrv_VBI_IsWSS_Ready(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_IsWSS_Ready,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_IsWSS_Ready fail\n");        
        return FALSE;
    }    
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_IsWSS_Ready();
#endif
}

/****************************************************************************
 *@brief            : Get WSS Data
 *@param
 *@return           : return WSS data
 ****************************************************************************/
MS_U16 MDrv_VBI_GetWSS_Data(void)
{        
#if (VBI_UTOPIA20)
    VBI_GET_DATA GetData;
    memset(&GetData, 0 , sizeof(VBI_GET_DATA));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                        
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_GetWSS_Data,(void*)&GetData) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_GetWSS_Data fail\n");        
        return FALSE;
    }    
    return GetData.u16data;
#else
    return _MDrv_VBI_GetWSS_Data();
#endif
}

/****************************************************************************
 *@brief            : Get VPS Data
 *@param lowerWord  : [OUT] VPS lower data
 *@param higherWord : [OUT] VPS higher data
 *@return
 ****************************************************************************/
void MDrv_VBI_GetVPS_Data(MS_U8 *lowerWord, MS_U8 *higherWord)
{
#if (VBI_UTOPIA20)
    VBI_GET_VPS_DATA GetVPSData;
    memset(&GetVPSData, 0 , sizeof(VBI_GET_VPS_DATA));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
    
    GetVPSData.lowerWord  = lowerWord;
    GetVPSData.higherWord = higherWord;
                            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_GetVPS_Data,(void*)&GetVPSData) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_GetVPS_Data fail\n");        
        return;
    }    
#else
    _MDrv_VBI_GetVPS_Data(lowerWord, higherWord);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get Complete VPS Data.
/// @ingroup VBI_TTX_ToBeModified
/// @param  pBuffer    \b IN/OUT: VPS buffer.
/// @param  dataLen   \b IN: buffer size.
/// @return None
//-------------------------------------------------------------------------------------------------
#define VPS_PACKET_LEN  15
static MS_U8 _u8VPSData[VPS_PACKET_LEN];
MS_BOOL MDrv_VBI_GetCompleteVPS_Data(MS_U8** pBuffer, MS_U32* dataLen)
{    
    VBI_GET_RAW_VPS_DATA GetRawVPSData;
    memset(&GetRawVPSData, 0 , sizeof(VBI_GET_RAW_VPS_DATA));

#if (VBI_UTOPIA20)

    if (NULL == pInstantVbi)
    {
        printf("pInstantVbi=NULL fail\n");
        return FALSE;
    }

    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_GetRawVPS_Data, (void*)&GetRawVPSData) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_VBI_GetCompleteVPS_Data fail\n"); 
		return FALSE;
    }    

#else
    _MDrv_VBI_GetRawVPS_Data(&GetRawVPSData.byte0, &GetRawVPSData.byte1, &GetRawVPSData.byte2, &GetRawVPSData.byte3);
#endif
    

    if(MDrv_VBI_IsVPS_Ready())
    {
        memset(_u8VPSData, 0, VPS_PACKET_LEN);
        _u8VPSData[0x0D - 1] = GetRawVPSData.byte0;
        _u8VPSData[0x0E - 1] = GetRawVPSData.byte1;
        _u8VPSData[0x05 - 1] = GetRawVPSData.byte2;
        _u8VPSData[0x0B - 1] = GetRawVPSData.byte3;

        *dataLen = VPS_PACKET_LEN;
        *pBuffer = _u8VPSData;

        return TRUE;        
    }
    else
    {
	    return FALSE;
	}
}
/****************************************************************************
 *@brief            : Set Video Standard
 *@param eStandard  : [IN] type (NTSC/PAL/SECAM)
 *@see VBI_VIDEO_STANDARD
 *@return
 ****************************************************************************/
void MDrv_VBI_SetVideoStandard(VBI_VIDEO_STANDARD eStandard)
{           
#if (VBI_UTOPIA20)
    VBI_SET_VIDEO_STANDARD SetVideoStandard;
    memset(&SetVideoStandard, 0 , sizeof(VBI_SET_VIDEO_STANDARD));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
    
    SetVideoStandard.eStandard = eStandard;
                                
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_SetVideoStandard,(void*)&SetVideoStandard) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_SetVideoStandard fail\n");        
    }    
#else
    _MDrv_VBI_SetVideoStandard(eStandard);
#endif
}

/****************************************************************************
 *@brief            : Check if the TTX packet in VBI buffer is empty
 *@param
 *@return
 *  - TRUE : is empty
 *  - FALSE: not empty
 ****************************************************************************/
MS_BOOL MDrv_VBI_TTX_PacketBufferIsEmpty(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                                            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_PacketBufferIsEmpty,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_TTX_PacketBufferIsEmpty fail\n");  
        return FALSE;
    }        
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_TTX_PacketBufferIsEmpty();
#endif
}

/****************************************************************************
 *@brief            : Check TTX circuit ready
 *@param
 *@return
 *  - TRUE : TTX circuit is ready.
 *  - FALSE: TTX circuit is not ready.
 ****************************************************************************/
MS_BOOL MDrv_VBI_TTX_CheckCircuitReady(void)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return FALSE;
    }

#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_CheckCircuitReady,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_TTX_CheckCircuitReady fail\n");  
        return FALSE;
    }        
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_TTX_CheckCircuitReady();
#endif
}

/****************************************************************************
 *@brief            : Get TTX Packet Count
 *@param
 *@return           : TTX packet count
 ****************************************************************************/
MS_U16 MDrv_VBI_TTX_GetPacketCount(void)
{
    if(_pStoreInfo._bTTXSupported == FALSE)
    {
        return 0;
    }    
    
#if (VBI_UTOPIA20)
    VBI_GET_DATA GetData;
    memset(&GetData, 0 , sizeof(VBI_GET_DATA));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                                            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_GetPacketCount,(void*)&GetData) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_TTX_GetPacketCount fail\n");  
        return FALSE;
    }        
    return GetData.u16data;
#else
    return _MDrv_VBI_TTX_GetPacketCount();
#endif
}

/****************************************************************************
 *@brief            : Get TTX Packets
 *@param dataAddr   : [IN] data address
 *@param length     : [OUT] packet length
 *@return
 *  - TRUE : get TTX packets successfully.
 *  - FALSE: get TTX packets unsuccessfully.
 ****************************************************************************/
MS_BOOL MDrv_VBI_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length)
{
#if (VBI_UTOPIA20)
    VBI_TTX_GET_PACKETS GetPackets;
    memset(&GetPackets, 0 , sizeof(VBI_TTX_GET_PACKETS));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
    
    GetPackets.dataAddr= dataAddr; 
    GetPackets.length = length;   
    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_GetPackets,(void*)&GetPackets) != UTOPIA_STATUS_SUCCESS)
    {
        //ULOGE("VBI", "Ioctl MDrv_VBI_TTX_GetPackets fail\n");  
        return FALSE;
    }  
    return TRUE;
#else
    return _MDrv_VBI_TTX_GetPackets(dataAddr, length);
#endif
}

/****************************************************************************
 *@brief                : Get TTX Packet
 *@param packetAddress  : [OUT] packet address
 *@return
 *  - TRUE : get TTX packet successfully.
 *  - FALSE: get TTX packet unsuccessfully.
 ****************************************************************************/
MS_BOOL MDrv_VBI_TTX_GetPacket(MS_PHY *packetAddress)
{
#if (VBI_UTOPIA20)
    VBI_TTX_GET_PACKET GetPacket;
    memset(&GetPacket, 0 , sizeof(VBI_TTX_GET_PACKET));
    
    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                        
    GetPacket.packetAddress = packetAddress;
                                                    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_GetPacket,(void*)&GetPacket) != UTOPIA_STATUS_SUCCESS)
    {
        //ULOGE("VBI", "Ioctl MDrv_VBI_TTX_GetPackets fail\n");  
        return FALSE;
    }  
    return TRUE;
#else
    return _MDrv_VBI_TTX_GetPacket(packetAddress);
#endif
}

/***********************************************************************
 * FUNCTION: MDrv_VBI_TTX_PacketBufferIsOverflow
 *
 * DESCRIPTION:
 *   Check if there is a packet buffer overflow. If there is an overflow,
 *   the the packet buffer should be cleared from the reading task.
 *
 * RETURN:
 *   TRUE if there is packet buffer overflow,
 *   NULL otherwise.
 ***********************************************************************/
MS_BOOL MDrv_VBI_TTX_PacketBufferIsOverflow( void )
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                                
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_PacketBufferIsOverflow,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_TTX_PacketBufferIsOverflow fail\n");  
        return FALSE;
    }  
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_TTX_PacketBufferIsOverflow();
#endif
}

/***********************************************************************
 * FUNCTION: MDrv_VBI_TTX_PacketBufferGetNoOfOverflows
 *
 * DESCRIPTION:
 *   Resuren the nomber of packet buffer overflows since the last reset
 *   or creation.
 *
 * RETURN:
 *   The number of packet buffer overflows.
 ***********************************************************************/
MS_U16 MDrv_VBI_TTX_PacketBufferGetNoOfOverflows( void )
{
#if (VBI_UTOPIA20)
    VBI_GET_DATA GetData;
    memset(&GetData, 0 , sizeof(VBI_GET_DATA));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                                
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows,(void*)&GetData) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_TTX_PacketBufferGetNoOfOverflows fail\n");  
        return FALSE;
    }  
    return GetData.u16data;
#else
    return _MDrv_VBI_TTX_PacketBufferGetNoOfOverflows();
#endif
}

/****************************************************************************
 *@brief            : Set TTX Enable Line
 *@param StartLine  : [IN] TTX start line
 *@param EndLine    : [IN] TTX end line
 *@return
 ****************************************************************************/
void MDrv_VBI_TTX_EnableLine(MS_U16 StartLine, MS_U16 EndLine)
{
#if (VBI_UTOPIA20)
    VBI_TTX_ENABLE_LINE EnableLine;
    memset(&EnableLine, 0 , sizeof(VBI_TTX_ENABLE_LINE));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
    
    EnableLine.StartLine = StartLine;
    EnableLine.EndLine   = EndLine;
    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_TTX_EnableLine,(void*)&EnableLine) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_TTX_EnableLine fail\n");  
        return;
    }  
#else
    _MDrv_VBI_TTX_EnableLine(StartLine, EndLine);
#endif
}

/****************************************************************************
 *@brief            : Fresh memory to avoid cache coherence issue
 *@param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
 *@param  u32Size  \b IN: size (must be 16-B aligned)
 *@return TRUE : succeed
 *@return FALSE : fail due to invalide parameter
 ****************************************************************************/
MS_BOOL MDrv_VBI_SyncMemory( MS_U32 u32Start, MS_U32 u32Size )
{
#if (VBI_UTOPIA20)
    VBI_SYNC_MEMORY SyncMemory;
    memset(&SyncMemory, 0 , sizeof(VBI_SYNC_MEMORY));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
        
    SyncMemory.u32Start = u32Start;
    SyncMemory.u32Size  = u32Size;
        
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_SyncMemory,(void*)&SyncMemory) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_SyncMemory fail\n");  
        return FALSE;
    }
    return TRUE;
#else
    return _MDrv_VBI_SyncMemory(u32Start, u32Size);
#endif
}

/****************************************************************************
 *@brief            : Initialize CC Slicer
 *@param u32RiuAddr : [IN] VBI CC RIU address
 *@param bufferAddr : [IN] VBI CC buffer address
 *@param packetCount: [IN] VBI CC packet count
 *@return
 ****************************************************************************/
void MDrv_VBI_CC_InitSlicer(MS_VIRT u32RiuAddr, MS_PHY bufferAddr, MS_U16 packetCount)
{
#if (VBI_UTOPIA20)
    VBI_CC_INIT_SLICER CCInitSlicer;
    memset(&CCInitSlicer, 0 , sizeof(VBI_CC_INIT_SLICER));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
            
    CCInitSlicer.u32RiuAddr  = u32RiuAddr;
    CCInitSlicer.bufferAddr  = bufferAddr;
    CCInitSlicer.packetCount = packetCount;
            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_InitSlicer,(void*)&CCInitSlicer) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_InitSlicer fail\n");  
        return;
    }
#else
    _MDrv_VBI_CC_InitSlicer(u32RiuAddr, bufferAddr, packetCount);
#endif
}

/****************************************************************************
 *@brief            : Initialize CC CVBS number
 *@param cvbs_no    : [IN] CVBS number
 *@return
 ****************************************************************************/
void MDrv_VBI_CC_InitYPbYr(MS_U8 cvbs_no)
{
#if (VBI_UTOPIA20)
    VBI_CC_INIT_YPBYR CCInitYPbYr;
    memset(&CCInitYPbYr, 0 , sizeof(VBI_CC_INIT_YPBYR));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
                
    CCInitYPbYr.cvbs_no = cvbs_no;
                
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_InitYPbYr,(void*)&CCInitYPbYr) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_InitYPbYr fail\n");  
        return;
    }
#else
    _MDrv_VBI_CC_InitYPbYr(cvbs_no);
#endif
}
/****************************************************************************
 *@brief            : Set CC Data Rate
 *@param ptable     : [OUT] data rate table
 *@return           : data rate or 0 when failed
 ****************************************************************************/
MS_U8 MDrv_VBI_CC_SetDataRate(MS_U8 *ptable)
{
#if (VBI_UTOPIA20)
    VBI_CC_SETDATARATE CCSetDataRate;
    memset(&CCSetDataRate, 0 , sizeof(VBI_CC_SETDATARATE));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                    
    CCSetDataRate.ptable = ptable;
                    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_SetDataRate,(void*)&CCSetDataRate) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_SetDataRate fail\n");  
        return FALSE;
    }
    return TRUE;
#else
    return _MDrv_VBI_CC_SetDataRate(ptable);
#endif
}

/****************************************************************************
 *@brief            : Set CC standard
 *@param eStandard  : [IN] video standard
 *@return           : TRUE or FALSE
 ****************************************************************************/
MS_BOOL MDrv_VBI_CC_SetVideoStandard(VBI_VIDEO_STANDARD eStandard)
{
#if (VBI_UTOPIA20)
    VBI_SET_VIDEO_STANDARD ccStandard;
    memset(&ccStandard, 0 , sizeof(VBI_SET_VIDEO_STANDARD));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                    
    ccStandard.eStandard = eStandard;
    ccStandard.bRet = FALSE;        
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_SetVideoStandard,(void*)&ccStandard) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_CMD_VBI_CC_SetVideoStandard fail\n");  
        return FALSE;
    }
    
    if(ccStandard.bRet == FALSE)
        ULOGE("VBI", "[%s] not support standard 0x%x\n", __FUNCTION__, (unsigned int)eStandard);

    return ccStandard.bRet;
#else
    return _MDrv_VBI_CC_SetStandard(eStandard);
#endif
}

/****************************************************************************
 *@brief            : Get CC Information
 *@param selector   : [IN] CC function select
 *@return           : packet count or indication
 ****************************************************************************/
MS_U32 MDrv_VBI_CC_GetInfo(MS_U32 selector)
{
#if (VBI_UTOPIA20)
    VBI_CC_GETINFO GetInfo;
    memset(&GetInfo, 0 , sizeof(VBI_CC_SETDATARATE));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                        
    GetInfo.selector = selector;

    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_GetInfo,(void*)&GetInfo) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_GetInfo fail\n");  
        return FALSE;
    }
    return GetInfo.info;
#else
    return _MDrv_VBI_CC_GetInfo(selector);
#endif
}

/****************************************************************************
 *@brief            : Set CC Frame Count
 *@param cnt        : [IN] frame count
 *@return
 ****************************************************************************/
void MDrv_VBI_CC_SetFrameCnt(MS_U8 cnt)
{
#if (VBI_UTOPIA20)
    VBI_CC_SET_FRAMECNT CCSetFrameCnt;
    memset(&CCSetFrameCnt, 0 , sizeof(VBI_CC_SET_FRAMECNT));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
                            
    CCSetFrameCnt.cnt = cnt;
    
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_SetFrameCnt,(void*)&CCSetFrameCnt) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_SetFrameCnt fail\n");  
        return;
    }
#else
    _MDrv_VBI_CC_SetFrameCnt(cnt);
#endif
}

/****************************************************************************
 *@brief            : Enable CC Slicer
 *@param bEnable    : [IN] enable or disable CC slicer
 *@return
 ****************************************************************************/
void MDrv_VBI_CC_EnableSlicer(MS_BOOL bEnable)
{
#if (VBI_UTOPIA20)
    VBI_CC_ENABLE_SLICER CCEnableSlicer;
    memset(&CCEnableSlicer, 0 , sizeof(VBI_CC_ENABLE_SLICER));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
                                
    CCEnableSlicer.bEnable = bEnable;
        
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_EnableSlicer,(void*)&CCEnableSlicer) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_EnableSlicer fail\n");  
        return;
    }
#else
    _MDrv_VBI_CC_EnableSlicer(bEnable);
#endif
}

/****************************************************************************
 *@brief            : Enable CC Line
 *@param StartLine  : [IN] start line
 *@param EndLine    : [IN] end line
 *@param mode       : [IN] NTSC/PAL/SECAM mode
 *@return
 ****************************************************************************/
void MDrv_VBI_CC_EnableLine(MS_U16 StartLine, MS_U16 EndLine, MS_U8 mode)
{
#if (VBI_UTOPIA20)
    VBI_CC_ENABLE_LINE CCEnableLine;
    memset(&CCEnableLine, 0 , sizeof(VBI_CC_ENABLE_LINE));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
                                    
    CCEnableLine.StartLine = StartLine;
    CCEnableLine.EndLine   = EndLine;
    CCEnableLine.mode      = mode;
            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_EnableLine,(void*)&CCEnableLine) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_EnableLine fail\n");  
        return;
    }
#else
    _MDrv_VBI_CC_EnableLine(StartLine, EndLine, mode);
#endif
}

/****************************************************************************
 *@brief            : Set CC SC window length
 *@param u8Len      : [IN] windows length
 *@return           : TRUE / FALSE
 ****************************************************************************/
MS_BOOL MDrv_VBI_CC_SetSCWindowLen(MS_U8 u8Len)
{
#if (VBI_UTOPIA20)
    VBI_CC_SET_SC_WND_LEN SCWndLen;
    memset(&SCWndLen, 0 , sizeof(VBI_CC_SET_SC_WND_LEN));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }
                                    
    SCWndLen.u8Len  = u8Len;
    SCWndLen.bRet   = FALSE;
            
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_CC_SetSCWindowLen,(void*)&SCWndLen) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_CC_SetSCWindowLen fail\n");  
        return FALSE;
    }
    
    return SCWndLen.bRet;
#else
    return _MDrv_VBI_CC_SetSCWindowLen(u8Len);
#endif
}
/****************************************************************************
 *@brief            : Set WSS/VPS Byte Number
 *@param cnt        : [IN] byte number
 *@return
 ****************************************************************************/
void MDrv_VBI_WSS_SetVpsByteNum(MS_U8 cnt)
{
#if (VBI_UTOPIA20)
    VBI_VBI_WSS_VPSBYTENUM WSSSetVpsByteCnt;
    memset(&WSSSetVpsByteCnt, 0 , sizeof(VBI_VBI_WSS_VPSBYTENUM));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return;
    }
                                        
    WSSSetVpsByteCnt.cnt = cnt;
                
    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_WSS_SetVpsByteNum,(void*)&WSSSetVpsByteCnt) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_WSS_SetVpsByteNum fail\n");  
        return;
    }
#else
    _MDrv_VBI_WSS_SetVpsByteNum(cnt);
#endif
}

/****************************************************************************
*@brief            : Suspend VBI driver
*@return TRUE - Success
*@return FALSE - Failure
*@note
*@Save VBI driver states to DRAM
****************************************************************************/
MS_BOOL MDrv_VBI_Suspend(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }

    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_Suspend,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_Suspend fail\n");
        return FALSE;
    }
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_Suspend();
#endif
}

/****************************************************************************
*@brief            :  Resume VBI driver
*@return TRUE - Success
*@return FALSE - Failure
*@note
*@Save VBI driver states to DRAM
*@Restore VBI driver states from DRAM
****************************************************************************/
MS_BOOL MDrv_VBI_Resume(void)
{
#if (VBI_UTOPIA20)
    VBI_BOOL CheckTureFalse;
    memset(&CheckTureFalse, 0 , sizeof(VBI_BOOL));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }

    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_Resume,(void*)&CheckTureFalse) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_Resume fail\n");
        return FALSE;
    }    
    return CheckTureFalse.bCheck;
#else
    return _MDrv_VBI_Resume();
#endif
}

/****************************************************************************
*@brief            : Set VBI suspend & resume
*@param u16PowerState              \b IN: VBI power state
*@return TRUE - Success
*@return FALSE - Failure
*@note
****************************************************************************/
MS_U32 MDrv_VBI_SetPowerState(EN_POWER_MODE u16PowerState)
{
#if (VBI_UTOPIA20)
    VBI_GET_RESULT GetResult;
    memset(&GetResult, 0 , sizeof(VBI_GET_RESULT));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }

    GetResult.u16PowerState = u16PowerState;

    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_SetPowerState,(void*)&GetResult) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_SetPowerState fail\n");
        return FALSE;
    }
    return GetResult.u32result;
#else
    return _MDrv_VBI_SetPowerState();
#endif
}

/****************************************************************************
*@brief                     : restrict vbi memory buffer range
*@phyAddr                   : start address of the range
*@u32Size                   : size of the range
*@return TRUE - Success
*@return FALSE - Failure
****************************************************************************/
MS_BOOL MDrv_VBI_ProtectMemory(MS_BOOL bEnable, MS_PHY phyAddr, MS_U32 u32Size)
{
    return VBI_ProtectMemory(bEnable, phyAddr, u32Size);

#if (VBI_UTOPIA20)
    VBI_PROTECT_MEMORY ProtectMemory;
    memset(&ProtectMemory, 0 , sizeof(VBI_PROTECT_MEMORY));

    if (NULL == pInstantVbi)
    {
        ULOGE("VBI", "pInstantVbi=NULL fail\n");
        return FALSE;
    }

    ProtectMemory.bEnable = bEnable;
    ProtectMemory.phyAddr = phyAddr;
    ProtectMemory.u32Size = u32Size;

    if(UtopiaIoctl(pInstantVbi, MDrv_CMD_VBI_ProtectMemory,(void*)&ProtectMemory) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("VBI", "Ioctl MDrv_VBI_ProtectMemory fail\n");
        return FALSE;
    }
    return ProtectMemory.bRet;
#else
    return _MDrv_VBI_ProtectMemory(bEnable, phyAddr, u32Size);
#endif
}

#undef _DRV_VBI_C
