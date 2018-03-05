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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#if defined(REDLION_LINUX_KERNEL_ENVI)
#include "drvHVD_Common.h"
#else
#include "MsCommon.h"
#endif
// Internal Definition
#include "regVPU.h"
#include "halVPU.h"
#include "halCHIP.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define VPU_MIU1BASE_ADDR    0x40000000   //Notice: this define must be comfirm with designer

#define VPU_MSG_ERR(format, args...)  //printf(format, ##args)
#define VPU_MSG_DEG(format, args...)  //printf(format, ##args)
#define VPU_MSG_INFO(format, args...)  //printf(format, ##args)

//------------------------------ MIU SETTINGS ----------------------------------
#define _MaskMiuReq_VPU_D_RW( m )       _VPU_WriteRegBit(MIU0_REG_RQ0_MASK, m, BIT(6))
#define _MaskMiuReq_VPU_Q_RW( m )       _VPU_WriteRegBit(MIU0_REG_RQ0_MASK, m, BIT(6))
#define _MaskMiuReq_VPU_I_R( m )        _VPU_WriteRegBit(MIU0_REG_RQ0_MASK+1, m, BIT(0))

#define _MaskMiu1Req_VPU_D_RW( m )      _VPU_WriteRegBit(MIU1_REG_RQ0_MASK, m, BIT(6))
#define _MaskMiu1Req_VPU_Q_RW( m )      _VPU_WriteRegBit(MIU1_REG_RQ0_MASK, m, BIT(6))
#define _MaskMiu1Req_VPU_I_R( m )       _VPU_WriteRegBit(MIU1_REG_RQ0_MASK+1, m, BIT(0))

#define VPU_D_RW_ON_MIU1                ((_VPU_ReadByte(MIU0_REG_SEL0) & BIT(6)) == BIT(6))
#define VPU_Q_RW_ON_MIU1                ((_VPU_ReadByte(MIU0_REG_SEL0) & BIT(6)) == BIT(6))
#define VPU_I_R_ON_MIU1                 ((_VPU_ReadByte(MIU0_REG_SEL0+1) & BIT(0)) == BIT(0)) //g08
#define _VPU_MIU_SetReqMask( miu_clients, mask ) \
   do { \
       if (miu_clients##_ON_MIU1 == 0) \
           _MaskMiuReq_##miu_clients( mask ); \
       else \
           _MaskMiu1Req_##miu_clients( mask ); \
   }while(0)

#if ENABLE_VPU_MUTEX_PROTECTION
static MS_S32 s32VPUMutexID = -1;
static MS_U8 _u8VPU_Mutex[] = {"VPU_Mutex"};
#define _HAL_VPU_MutexCreate() \
            if( s32VPUMutexID < 0 ) \
            { \
                s32VPUMutexID = MsOS_CreateMutex(E_MSOS_FIFO, (char *)_u8VPU_Mutex, MSOS_PROCESS_SHARED); \
            }
#define _HAL_VPU_MutexDelete() \
            if( s32VPUMutexID >= 0 ) \
            { \
                MsOS_DeleteMutex(s32VPUMutexID); \
                s32VPUMutexID = -1; \
            }
#define _HAL_VPU_Entry() \
            if( s32VPUMutexID >= 0 ) \
            { \
                if (!MsOS_ObtainMutex(s32VPUMutexID, VPU_DEFAULT_MUTEX_TIMEOUT)) \
                { \
                    printf("[HAL VPU][%06d] Mutex taking timeout\n", __LINE__); \
                } \
            }
#define _HAL_VPU_Return(_ret) \
            { \
                if( s32VPUMutexID >= 0 ) \
                { \
                    MsOS_ReleaseMutex(s32VPUMutexID); \
                } \
                return _ret; \
            }
#define _HAL_VPU_Release() \
            { \
                if( s32VPUMutexID >= 0 ) \
                { \
                    MsOS_ReleaseMutex(s32VPUMutexID); \
                } \
            }
#else
#define _HAL_VPU_MutexCreate()
#define _HAL_VPU_MutexDelete()
#define _HAL_VPU_Entry()
#define _HAL_VPU_Return(_ret)       {return _ret;}
#define _HAL_VPU_Release()
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if defined (__aeon__)
static MS_U32 u32VPURegOSBase=0xA0000000;
#else
static MS_U32 u32VPURegOSBase=0xBF200000;
#endif
static VPU_DecoderType _ePreDecoder = E_VPU_DECODER_NONE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void HAL_VPU_InitRegBase(MS_U32 u32RegBase)
{
    u32VPURegOSBase = u32RegBase;
}

MS_BOOL HAL_VPU_Init(VPU_Init_Params* InitParams)
{
    MS_BOOL bRet = TRUE;
    // enable module
    HAL_VPU_ClockInv(InitParams->bClockInv);
    HAL_VPU_ClockSpeed(InitParams->eClockSpeed);
    HAL_VPU_PowerCtrl( TRUE );
#if 1 //Create VPU's own mutex
    _HAL_VPU_MutexCreate();
#else
    s32VPUMutexID = InitParams->s32VPUMutexID;
    u32VPUMutexTimeOut = InitParams->u32VPUMutexTimeout;
#endif
    return bRet;
}

MS_BOOL HAL_VPU_DeInit(void)
{
    MS_BOOL bRet = TRUE;
    HAL_VPU_PowerCtrl( FALSE );
    _HAL_VPU_MutexDelete();
    return bRet;
}

void HAL_VPU_PowerCtrl(MS_BOOL bEnable)
{
    if( bEnable )
    {
        _VPU_WriteWordMask( REG_TOP_VPU , 0 , TOP_CKG_VPU_DIS );
    }
    else
    {
        _VPU_WriteWordMask( REG_TOP_VPU , TOP_CKG_VPU_DIS , TOP_CKG_VPU_DIS );
    }
}

void HAL_VPU_ClockSpeed(MS_U32 u32type)
{
    // setup clock.
    switch( u32type )
    {
        case VPU_CLOCK_216MHZ:
        case VPU_CLOCK_192MHZ:
        case VPU_CLOCK_160MHZ:
        case VPU_CLOCK_144MHZ:
        _VPU_WriteWordMask( REG_TOP_VPU , u32type , TOP_CKG_VPU_CLK_MASK );
        break;
    default:
        _VPU_WriteWordMask( REG_TOP_VPU , VPU_CLOCK_216MHZ , TOP_CKG_VPU_CLK_MASK );
        break;
    }
}

void HAL_VPU_ClockInv(MS_BOOL bEnable)
{
    if( TRUE )
    {
        _VPU_WriteWordMask( REG_TOP_VPU , 0 , TOP_CKG_VPU_INV );
    }
    else
    {
        _VPU_WriteWordMask( REG_TOP_VPU , TOP_CKG_VPU_INV , TOP_CKG_VPU_INV );
    }
}

void HAL_VPU_MIU_RW_Protect(MS_BOOL bEnable)
{
    _VPU_MIU_SetReqMask(  VPU_D_RW  ,  bEnable );
    _VPU_MIU_SetReqMask(  VPU_Q_RW  ,  bEnable );
    _VPU_MIU_SetReqMask(  VPU_I_R  ,  bEnable );
    VPU_Timer_delay_ms(1);
}

///-----------------------------------------------------------------------------
/// config AVCH264 CPU
/// @param u32StAddr \b IN: CPU binary code base address in DRAM.
/// @param u8dlend_en \b IN: endian
///     - 1, little endian
///     - 0, big endian
///-----------------------------------------------------------------------------
MS_BOOL HAL_VPU_CPUSetting(MS_U32 u32StAddr)
{
    MS_BOOL bRet = TRUE;
    MS_U32 u32Offset = 0;
    MS_U16 tempreg = 0;

    u32Offset = (u32StAddr >= HAL_MIU1_BASE) ? (u32StAddr-HAL_MIU1_BASE) : u32StAddr ;

    _VPU_Write2Byte(VPU_REG_SPI_BASE,  0xC000);
    _VPU_WriteWordMask( VPU_REG_CPU_SETTING , 0 , VPU_REG_CPU_SPI_BOOT );
    _VPU_WriteWordMask( VPU_REG_CPU_SETTING , 0 , VPU_REG_CPU_SDRAM_BOOT );
    _VPU_Write2Byte(VPU_REG_DQMEM_MASK_L,  0xc000);
    _VPU_Write2Byte(VPU_REG_DQMEM_MASK_H,  0xffff);
    _VPU_Write2Byte(VPU_REG_IO2_BASE,  0x8000);
    _VPU_Write2Byte(VPU_REG_DQMEM_BASE_L,  0x0000);
    _VPU_Write2Byte(VPU_REG_DQMEM_BASE_H, 0x2000);

    if(u32StAddr >= HAL_MIU1_BASE)
    {
        // Data sram base Unit: byte address
        _VPU_Write2Byte(VPU_REG_DCU_SDR_BASE_L, (MS_U16)((u32Offset | VPU_MIU1BASE_ADDR) & 0x0000ffff)) ;
        _VPU_Write2Byte(VPU_REG_DCU_SDR_BASE_H, (MS_U16)(((u32Offset | VPU_MIU1BASE_ADDR)>>16) & 0xffff));

        // Instruction sram base Unit: byte address
        _VPU_Write2Byte(VPU_REG_ICU_SDR_BASE_L, (MS_U16)(u32Offset & 0x0000ffff)) ;
        _VPU_Write2Byte(VPU_REG_ICU_SDR_BASE_H, (MS_U16)((u32Offset >>16) & 0xffff));
    }
    else
    {
        // Data sram base Unit: byte address
        _VPU_Write2Byte(VPU_REG_DCU_SDR_BASE_L, (MS_U16)(u32Offset & 0x0000ffff)) ;
        _VPU_Write2Byte(VPU_REG_DCU_SDR_BASE_H, (MS_U16)((u32Offset>>16) & 0xffff));

        // Instruction sram base Unit: byte address
        _VPU_Write2Byte(VPU_REG_ICU_SDR_BASE_L, (MS_U16)(u32Offset & 0x0000ffff)) ;
        _VPU_Write2Byte(VPU_REG_ICU_SDR_BASE_H, (MS_U16)((u32Offset>>16) & 0xffff));
    }

    tempreg = _VPU_Read2Byte(VPU_REG_CONTROL_SET);
    tempreg |= VPU_REG_IO2_EN;
    tempreg |= VPU_REG_QMEM_SPACE_EN;
    _VPU_Write2Byte(VPU_REG_CONTROL_SET, tempreg);

    return bRet;
}

///-----------------------------------------------------------------------------
/// H.264 SW reset
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
///-----------------------------------------------------------------------------
MS_BOOL HAL_VPU_SwRst(void)
{
    MS_U16 tempreg=0, tempreg1=0;
    MS_BOOL bRet = TRUE;

    HAL_VPU_MIU_RW_Protect(TRUE);

    tempreg1 = _VPU_Read2Byte(MAU1_CPU_RST);
    tempreg1 |= MAU1_REG_SW_RESET;
    _VPU_Write2Byte(MAU1_CPU_RST, tempreg1);

#if defined(UDMA_FPGA_ENVI)
    tempreg = _VPU_Read2Byte(VPU_REG_RESET);
    _VPU_Write2Byte(VPU_REG_RESET, (tempreg& 0xfffd));
#endif

    tempreg = _VPU_Read2Byte(VPU_REG_CPU_SETTING);
    tempreg &= ~VPU_REG_CPU_R2_EN;
    tempreg &= ~VPU_REG_CPU_SW_RSTZ;
    tempreg &= ~VPU_REG_CPU_MIU_SW_RSTZ;
    _VPU_Write2Byte(VPU_REG_CPU_SETTING, tempreg);

    VPU_Timer_delay_ms(1);
    HAL_VPU_MIU_RW_Protect(FALSE);
    return bRet;
}

///-----------------------------------------------------------------------------
/// CPU reset release
///-----------------------------------------------------------------------------
void HAL_VPU_SwRstRelse(void)
{
    MS_U16 tempreg=0, tempreg1=0;

    tempreg = _VPU_Read2Byte(VPU_REG_CPU_SETTING);
    tempreg |= VPU_REG_CPU_R2_EN;
    tempreg |= VPU_REG_CPU_SW_RSTZ;
    tempreg |= VPU_REG_CPU_MIU_SW_RSTZ;
    _VPU_Write2Byte(VPU_REG_CPU_SETTING, tempreg);

    tempreg1 = _VPU_Read2Byte(MAU1_CPU_RST);
    tempreg1 &= ~MAU1_REG_SW_RESET;
    _VPU_Write2Byte(MAU1_CPU_RST, tempreg1);

    //MsOS_DelayTask(1);
}

void HAL_VPU_SwRelseMAU(void)
{
    MS_U16 tempreg=0;

    tempreg = _VPU_Read2Byte(MAU1_CPU_RST);
    tempreg &= ~MAU1_REG_SW_RESET;
    _VPU_Write2Byte(MAU1_CPU_RST, tempreg);
}

//todo: hvd PTS R/W pointer need upate from sram to dram
MS_U32 HAL_VPU_MemRead(MS_U32 u32Addr)
{
    MS_U32 u32value = 0;

    return u32value;
}

MS_BOOL HAL_VPU_MemWrite(MS_U32 u32Addr , MS_U32 u32value)
{
    MS_BOOL bRet=TRUE;

    return bRet;
}

///-----------------------------------------------------------------------------
/// Check AVCH264 Ready or not
/// @return TRUE or FALSE
///     - TRUE, MailBox is free
///     - FALSE, MailBox is busy
/// @param u8MBox \b IN: MailBox to check
///     - AVCH264_HI_MBOX0,
///     - AVCH264_HI_MBOX1,
///     - AVCH264_RISC_MBOX0,
///     - AVCH264_RISC_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL HAL_VPU_MBoxRdy(MS_U32 u32type)
{
    MS_BOOL bResult = FALSE;
    switch(u32type)
    {
        case VPU_HI_MBOX0:
            bResult = (_VPU_Read2Byte(VPU_REG_HI_MBOX_RDY)&VPU_REG_HI_MBOX0_RDY)?FALSE:TRUE;
            break;
        case VPU_HI_MBOX1:
            bResult = (_VPU_Read2Byte(VPU_REG_HI_MBOX_RDY)&VPU_REG_HI_MBOX1_RDY)?FALSE:TRUE;
            break;
        case VPU_RISC_MBOX0:
            bResult = (_VPU_Read2Byte(VPU_REG_RISC_MBOX_RDY)&VPU_REG_RISC_MBOX0_RDY)?TRUE:FALSE;
            break;
        case VPU_RISC_MBOX1:
            bResult = (_VPU_Read2Byte(VPU_REG_RISC_MBOX_RDY)&VPU_REG_RISC_MBOX1_RDY)?TRUE:FALSE;
            break;
        default:
            break;
    }
    return bResult;
}


///-----------------------------------------------------------------------------
/// Read message from AVCH264
/// @return TRUE or FALSE
///     - TRUE, success
///     - FALSE, failed
/// @param u8MBox \b IN: MailBox to read
///     - AVCH264_RISC_MBOX0
///     - AVCH264_RISC_MBOX1
/// @param u32Msg \b OUT: message read
///-----------------------------------------------------------------------------
MS_BOOL HAL_VPU_MBoxRead(MS_U32 u32type, MS_U32 *u32Msg)
{
    MS_BOOL bResult = TRUE;
    switch(u32type)
    {
        case VPU_HI_MBOX0:
            *u32Msg = ((MS_U32)(_VPU_Read2Byte(VPU_REG_HI_MBOX0_H)) << 16) |
                      ((MS_U32)(_VPU_Read2Byte(VPU_REG_HI_MBOX0_L)));
            break;
        case VPU_HI_MBOX1:
            *u32Msg = ((MS_U32)(_VPU_Read2Byte(VPU_REG_HI_MBOX1_H)) << 16) |
                      ((MS_U32)(_VPU_Read2Byte(VPU_REG_HI_MBOX1_L)));
            break;
        case VPU_RISC_MBOX0:
            *u32Msg = ((MS_U32)(_VPU_Read2Byte(VPU_REG_RISC_MBOX0_H)) << 16) |
                      ((MS_U32)(_VPU_Read2Byte(VPU_REG_RISC_MBOX0_L)));
            break;
        case VPU_RISC_MBOX1:
            *u32Msg = ((MS_U32)(_VPU_Read2Byte(VPU_REG_RISC_MBOX1_H)) << 16) |
                      ((MS_U32)(_VPU_Read2Byte(VPU_REG_RISC_MBOX1_L)));
            break;

        default:
            *u32Msg=0;
            bResult = FALSE;
            break;
    }
    return bResult;
}

///-----------------------------------------------------------------------------
/// Mailbox from AVCH264 clear bit resest
///-----------------------------------------------------------------------------
void HAL_VPU_MBoxClear(MS_U32 u32type)
{
    switch(u32type)
    {
        case VPU_RISC_MBOX0:
            _VPU_WriteWordMask(  VPU_REG_RISC_MBOX_CLR , VPU_REG_RISC_MBOX0_CLR  , VPU_REG_RISC_MBOX0_CLR  );
            break;
        case VPU_RISC_MBOX1:
            _VPU_WriteWordMask(  VPU_REG_RISC_MBOX_CLR , VPU_REG_RISC_MBOX1_CLR  , VPU_REG_RISC_MBOX1_CLR  );
            break;
        default:
            break;
    }
}

///-----------------------------------------------------------------------------
/// Send message to AVCH264
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
/// @param u8MBox \b IN: MailBox
///     - AVCH264_HI_MBOX0,
///     - AVCH264_HI_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL HAL_VPU_MBoxSend(MS_U32 u32type, MS_U32 u32Msg)
{
    MS_BOOL bResult = TRUE;
    switch(u32type)
    {
        case VPU_HI_MBOX0:
            _VPU_Write4Byte( VPU_REG_HI_MBOX0_L  ,  u32Msg );
            _VPU_WriteWordMask(  VPU_REG_HI_MBOX_SET , VPU_REG_HI_MBOX0_SET  , VPU_REG_HI_MBOX0_SET  );
            break;
        case VPU_HI_MBOX1:
            _VPU_Write4Byte( VPU_REG_HI_MBOX1_L  ,  u32Msg );
            _VPU_WriteWordMask(  VPU_REG_HI_MBOX_SET , VPU_REG_HI_MBOX1_SET  , VPU_REG_HI_MBOX1_SET  );
            break;
        default:
            bResult = FALSE;
            break;
    }
    return bResult;
}

MS_U32 HAL_VPU_GetProgCnt(void)
{
    MS_U16 expc_l=0;
    MS_U16 expc_h=0;

    expc_l = _VPU_Read2Byte(VPU_REG_EXPC_L) & 0xFFFF;
    expc_h = _VPU_Read2Byte(VPU_REG_EXPC_H) & 0xFFFF;

    return (((MS_U32)expc_h) << 16) | (MS_U32)expc_l;
}

/// Drivers can query if they need to reload f/w
MS_BOOL HAL_VPU_IsNeedReload(VPU_DecoderType eDecoder)
{
    //printf("Pre=%x, Cur=%x\n", _ePreDecoder, eDecoder);
    return (_ePreDecoder != eDecoder);
}

/// Drivers should set its eDecoderType to VPU after initialization succeed.
void HAL_VPU_SetFWDecoder(VPU_DecoderType eDecoder)
{
    _ePreDecoder = eDecoder;
}

/// Driver can query the current eDecoderType
VPU_DecoderType HAL_VPU_GetFWDecoder(void)
{
    return _ePreDecoder;
}


MS_BOOL HAL_VPU_MAU_IDLE(void)
{
    if(((_VPU_Read2Byte(MAU1_ARB0_DBG0) & MAU1_FSM_CS_MASK) == MAU1_FSM_CS_IDLE)
    && ((_VPU_Read2Byte(MAU1_ARB1_DBG0) & MAU1_FSM_CS_MASK) == MAU1_FSM_CS_IDLE))
    {
        return TRUE;
    }

    return FALSE;
}


