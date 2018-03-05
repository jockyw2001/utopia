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
#include "drvHVD_Common.h"

// Internal Definition
#include "drvHVD_def.h"
#include "fwHVD_if.h"
#include "halHVD.h"
#include "regHVD.h"
#include "halVPU.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#ifndef REDLION_LINUX_KERNEL_ENVI
    #if defined(MSOS_TYPE_NOS) //T8, temp define for bring up
        #if defined(__aarch64__)
            #define HVD_CACHE_TO_UNCACHE_CONVERT
        #endif
    #endif
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define RV_VLC_TABLE_SIZE 0x20000

#if HVD_ENABLE_EMBEDDED_FW_BINARY

static MS_U8 u8HVD_FW_Binary[] =
{
    #include "fwHVD.dat"
};
#if HVD_ENABLE_RV_FEATURE
static MS_U8 u8HVD_VLC_Binary[] =
{
    #include "fwHVD_VLC.dat"
};
#endif

#endif

static HVD_AVC_VUI_DISP_INFO g_hvd_VUIINFO;
static MS_U32 u32PTSRptrAddr=0;
static MS_U32 u32PTSWptrAddr=0;
static MS_U32 u32PTSPreWptr=0;
static HVD_PTS_Entry PTSEntry;
static MS_U32 u32PTSByteCnt=0;
static MS_U32 u32BBUWptr=0;
static MS_U8  g_hvd_nal_fill_pair[2][8] = {{0,0,0,0,0,0,0,0}, { 0,0,0,0,0,0,0,0}};
static MS_U32 u32BBUEntryNum=0;
static MS_U32 u32BBUEntryNumTH=0;
static MS_U32 u32RV_VLCTableAddr=0;     // offset from Frame buffer start address
//---------------------------------- Mutex settings -----------------------------------------
#if HAL_HVD_ENABLE_MUTEX_PROTECT
static MS_S32 s32HVDMutexID=-1;
static MS_U8 _u8HVD_Mutex[] = {"HVD_Mutex"};

#define _HAL_HVD_MutexCreate()                                                     \
            if( s32HVDMutexID < 0 )                                                 \
            {                                                                                                       \
                s32HVDMutexID = OSAL_HVD_MutexCreate( _u8HVD_Mutex );        \
            }
#define _HAL_HVD_MutexDelete()                                                 \
            if( s32HVDMutexID >= 0 )                                                 \
            {                                                                                                   \
                OSAL_HVD_MutexDelete(s32HVDMutexID);                           \
                s32HVDMutexID = -1;                                                \
            }
#define  _HAL_HVD_Entry()                                                                       \
            if( s32HVDMutexID >= 0 )                                                 \
            {                                                                                                       \
                if (!OSAL_HVD_MutexObtain(s32HVDMutexID, OSAL_HVD_MUTEX_TIMEOUT))                    \
                {                                                                                                                       \
                    printf("[HAL HVD][%06d] Mutex taking timeout\n", __LINE__);                    \
                }                                                                                                                   \
            }
#define _HAL_HVD_Return(_ret)                                                           \
            {                                                                                                   \
                if( s32HVDMutexID >= 0 )                                                                \
                {                                                                                                       \
                    OSAL_HVD_MutexRelease(s32HVDMutexID);                                       \
                }                                                                                                       \
                return _ret;                                                                                        \
            }
#define _HAL_HVD_Release()                                                           \
            {                                                                                                   \
                if( s32HVDMutexID >= 0 )                                                                \
                {                                                                                                       \
                    OSAL_HVD_MutexRelease(s32HVDMutexID);                                       \
                }                                                                                                       \
            }


#else   // HAL_HVD_ENABLE_MUTEX_PROTECT

#define _HAL_HVD_MutexCreate()
#define _HAL_HVD_MutexDelete()
#define _HAL_HVD_Entry()
#define _HAL_HVD_Return(_ret)      {return _ret;}
#define _HAL_HVD_Release()

#endif      // HAL_HVD_ENABLE_MUTEX_PROTECT

//------------------------------ MIU SETTINGS ----------------------------------
#define _MaskMiuReq_MVD_RW( m )         _HVD_WriteRegBit(MIU0_REG_RQ2_MASK, m, BIT(4))
#define _MaskMiuReq_MVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ0_MASK+1, m, BIT(4))
#define _MaskMiuReq_HVD_RW( m )         _HVD_WriteRegBit(MIU0_REG_RQ2_MASK, m, BIT(2))
#define _MaskMiuReq_HVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ2_MASK, m, BIT(1))

#define _MaskMiu1Req_MVD_RW( m )        _HVD_WriteRegBit(MIU1_REG_RQ2_MASK, m, BIT(4))
#define _MaskMiu1Req_MVD_BBU_R( m )     _HVD_WriteRegBit(MIU1_REG_RQ0_MASK+1, m, BIT(4))
#define _MaskMiu1Req_HVD_RW( m )        _HVD_WriteRegBit(MIU1_REG_RQ2_MASK, m, BIT(2))
#define _MaskMiu1Req_HVD_BBU_R( m )     _HVD_WriteRegBit(MIU1_REG_RQ2_MASK, m, BIT(1))

#define HVD_MVD_RW_ON_MIU1              ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(4)) == BIT(4))
#define HVD_MVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(12)) == BIT(12))
#define HVD_HVD_RW_ON_MIU1              ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(2)) == BIT(2))
#define HVD_HVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(1)) == BIT(1))

#define _HVD_MIU_SetReqMask( miu_clients, mask ) \
   do { \
       if (HVD_##miu_clients##_ON_MIU1 == 0) \
           _MaskMiuReq_##miu_clients( mask ); \
       else \
           _MaskMiu1Req_##miu_clients( mask ); \
   }while(0)

// check RM is supported or not
#define HVD_HW_RUBBER3      (HAL_HVD_Get_HWVersionID()& BIT(14) )
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static volatile HVD_ShareMem *pHVDShareMem  = NULL;
static HVD_Drv_Ctrl *pHVDCtrl_Hal           = NULL;
static HVD_Pre_Ctrl *pHVDPreCtrl_Hal        = NULL;
static MS_U32 u32HVDCmdTimeout              = 0;
static MS_U32 u32VPUClockType               = 216;
static MS_U32 u32HVDClockType               = 216;//144;

#if defined (__aeon__)
static MS_U32 u32HVDRegOSBase = 0xA0200000;
#else
static MS_U32 u32HVDRegOSBase = 0xBF200000;
#endif

#if defined(UDMA_FPGA_ENVI)
static HVD_ShareMem UDMA_pc_HVDShareMem;
static MS_U32 UDMA_fpga_HVDShareMemAddr;
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_U32 _HAL_HVD_GetBBUQNumb(void);


static MS_U16 _HAL_HVD_GetBBUReadptr(void)
{
    MS_U16 u16Ret=0;
    //_HAL_HVD_Entry();
    _HVD_WriteWordMask(  HVD_REG_POLL_NAL_RPTR , 0  ,  HVD_REG_POLL_NAL_RPTR_BIT  );
    _HVD_WriteWordMask(  HVD_REG_POLL_NAL_RPTR , HVD_REG_POLL_NAL_RPTR_BIT  ,  HVD_REG_POLL_NAL_RPTR_BIT  );
    u16Ret = _HVD_Read2Byte(  HVD_REG_NAL_RPTR_HI   );
    //_HAL_HVD_Return( u16Ret);
    return u16Ret;
}

static void _HAL_HVD_SetBBUWriteptr(MS_U16 u16BBUNewWptr )
{
    _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI, u16BBUNewWptr );
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR , HVD_REG_NAL_WPTR_SYNC , HVD_REG_NAL_WPTR_SYNC); // set bit 3
}

static MS_BOOL _HAL_HVD_MBoxSend(MS_U8 u8MBox, MS_U32 u32Msg)
{
    MS_BOOL bResult = TRUE;
    switch(u8MBox)
    {
    case E_HVD_HI_0:
        _HVD_Write4Byte(HVD_REG_HI_MBOX0_L, u32Msg);
        _HVD_WriteWordMask(HVD_REG_HI_MBOX_SET, HVD_REG_HI_MBOX0_SET, HVD_REG_HI_MBOX0_SET);
        break;

    case E_HVD_HI_1:
        _HVD_Write4Byte(HVD_REG_HI_MBOX1_L, u32Msg);
        _HVD_WriteWordMask(HVD_REG_HI_MBOX_SET, HVD_REG_HI_MBOX1_SET, HVD_REG_HI_MBOX1_SET);
        break;
    case E_HVD_VPU_HI_0:
        bResult=HAL_VPU_MBoxSend( VPU_HI_MBOX0  , u32Msg );
        break;

    case E_HVD_VPU_HI_1:
        bResult=HAL_VPU_MBoxSend( VPU_HI_MBOX1  , u32Msg );
        break;
    default:
        bResult = FALSE;
        break;
    }

    return bResult;
}

static MS_BOOL _HAL_HVD_MBoxReady(MS_U8 u8MBox)
{
    MS_BOOL bResult = TRUE;
    switch(u8MBox)
    {
    case E_HVD_HI_0:
        bResult = _HVD_ReadWordBit(HVD_REG_HI_MBOX_RDY, HVD_REG_HI_MBOX0_RDY) ? FALSE : TRUE;
        break;
    case E_HVD_HI_1:
        bResult = _HVD_ReadWordBit(HVD_REG_HI_MBOX_RDY, HVD_REG_HI_MBOX1_RDY) ? FALSE : TRUE;
        break;
    case E_HVD_RISC_0:
        bResult = _HVD_ReadWordBit(HVD_REG_RISC_MBOX_RDY, HVD_REG_RISC_MBOX0_RDY) ? TRUE: FALSE;
        break;
    case E_HVD_RISC_1:
        bResult = _HVD_ReadWordBit(HVD_REG_RISC_MBOX_RDY, HVD_REG_RISC_MBOX1_RDY) ? TRUE: FALSE;
        break;
    case E_HVD_VPU_HI_0:
        bResult = HAL_VPU_MBoxRdy( VPU_HI_MBOX0);
        break;
    case E_HVD_VPU_HI_1:
        bResult = HAL_VPU_MBoxRdy( VPU_HI_MBOX1);
        break;
    case E_HVD_VPU_RISC_0:
        bResult = HAL_VPU_MBoxRdy( VPU_RISC_MBOX0);
        break;
    case E_HVD_VPU_RISC_1:
        bResult = HAL_VPU_MBoxRdy( VPU_RISC_MBOX1);
        break;
    default:
        break;
    }
    return bResult;
}

static MS_BOOL _HAL_HVD_MBoxRead(MS_U8 u8MBox, MS_U32 *u32Msg)
{
    MS_BOOL bResult = TRUE;
    switch(u8MBox)
    {
    case E_HVD_HI_0:
        *u32Msg = _HVD_Read4Byte(HVD_REG_HI_MBOX0_L);
        break;
    case E_HVD_HI_1:
        *u32Msg = _HVD_Read4Byte(HVD_REG_HI_MBOX1_L);
        break;
    case E_HVD_RISC_0:
        *u32Msg = _HVD_Read4Byte(HVD_REG_RISC_MBOX0_L);
        break;
    case E_HVD_RISC_1:
        *u32Msg = _HVD_Read4Byte(HVD_REG_RISC_MBOX1_L);
        break;
    case E_HVD_VPU_RISC_0:
        bResult=HAL_VPU_MBoxRead( VPU_RISC_MBOX0 , u32Msg  );
        break;
    case E_HVD_VPU_RISC_1:
        bResult=HAL_VPU_MBoxRead( VPU_RISC_MBOX1 , u32Msg  );
        break;
    default:
        bResult = FALSE;
        break;
    }
    return bResult;
}

static void _HAL_HVD_MBoxClear(MS_U8 u8MBox)
{
    switch(u8MBox)
    {
    case E_HVD_RISC_0:
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, HVD_REG_RISC_MBOX0_CLR , HVD_REG_RISC_MBOX0_CLR);
        break;
    case E_HVD_RISC_1:
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, HVD_REG_RISC_MBOX1_CLR , HVD_REG_RISC_MBOX1_CLR);
        break;
    case E_HVD_VPU_RISC_0:
        HAL_VPU_MBoxClear( VPU_RISC_MBOX0  );
        break;
    case E_HVD_VPU_RISC_1:
        HAL_VPU_MBoxClear( VPU_RISC_MBOX1  );
        break;
    default:
        break;
    }
}

void HAL_HVD_Dump_HW_Status(MS_U32 numb)
{
    MS_U32 i=0;
    MS_U32 value=0;
    HVD_MSG_DEG("HVD Dump HW status:");
    for(  i=0 ; i <= numb ; i++ )
    {
        _HVD_Write2Byte(HVD_REG_DEBUG_SEL, i);
        value = _HVD_Read2Byte(HVD_REG_DEBUG_DAT_L);
        value |= ((MS_U32)_HVD_Read2Byte(HVD_REG_DEBUG_DAT_H))<<16;
        if( value == 0 )
        {
            break;
        }
        HVD_MSG_DEG(" %lx" , value );
        if(  ((i % 8)+1) ==8)
        {
            HVD_MSG_DEG(" |%lu\n" , i +1  );
        }
    }
    HVD_MSG_DEG("\nHVD Dump HW status End: total number:%lu\n" , i );
}

static MS_U32 _HAL_HVD_GetPC(void)
{
    MS_U32 u32PC=0;
    u32PC = HAL_VPU_GetProgCnt();
//    HVD_MSG_DEG("<gdbg>pc0 =0x%lx\n",u32PC);
    return u32PC;
}

static MS_U32 _HAL_HVD_GetFWState(void)
{
    MS_U32 u32Ret=0;
    if( _HAL_HVD_MBoxRead(HAL_HVD_REG_FW_STATE , &u32Ret ))
    {
        return u32Ret;
    }
    else
    {
        return 0;
    }
}

static MS_U32 _HAL_HVD_GetESWritePtr(void)
{
    MS_U32 data=0;
    if( (pHVDCtrl_Hal->InitParams.u32ModeFlag&E_HVD_INIT_INPUT_MASK )==E_HVD_INIT_INPUT_DRV)
    {
        data=pHVDCtrl_Hal->LastNal.u32NalAddr+pHVDCtrl_Hal->LastNal.u32NalSize;
        if( data > pHVDCtrl_Hal->MemMap.u32BitstreamBufSize)
        {
            data-=pHVDCtrl_Hal->MemMap.u32BitstreamBufSize;
            HVD_MSG_ERR( "HVD HAL: _HAL_HVD_GetESWritePtr(): app should not put this kind of packet\n");
        }
    }
    else
    {
        data =pHVDShareMem->u32ESWritePtr;
    }
    return data;
}

static MS_U32 _HAL_HVD_GetESReadPtr(MS_BOOL bDbug)
{
    MS_U32 data=0;

    if(((pHVDCtrl_Hal->InitParams.u32ModeFlag&E_HVD_INIT_INPUT_MASK)==E_HVD_INIT_INPUT_DRV)
    || (TRUE == bDbug))
    {
        // set reg_poll_nal_rptr 0
        _HVD_WriteWordMask(HVD_REG_ESB_RPTR, 0, HVD_REG_ESB_RPTR_POLL);
        // set reg_poll_nal_rptr 1
        _HVD_WriteWordMask(HVD_REG_ESB_RPTR, HVD_REG_ESB_RPTR_POLL, HVD_REG_ESB_RPTR_POLL);
        // read reg_nal_rptr_hi
#if 0
        if( HVD_HW_RUBBER3 )
        {
            data=_HVD_Read2Byte(  HVD_REG_ESB_RPTR ) & 0xFF80;
            data>>=7;
            data |= _HVD_Read2Byte(  HVD_REG_ESB_RPTR_H ) << 9;
        }
        else    // rubber2
#endif
    //_HAL_HVD_Entry();
        data=_HVD_Read2Byte(  HVD_REG_ESB_RPTR ) & 0xFFC0;
        data>>=6;
        data |= _HVD_Read2Byte(  HVD_REG_ESB_RPTR_H ) << 10;
    //_HAL_HVD_Release();
    // patch for XDemux
    #if 0
    /*
        if( (pHVDCtrl_Hal->InitParams.u32ModeFlag&E_HVD_INIT_INPUT_MASK )==E_HVD_INIT_INPUT_DRV)
        {
            MS_U32 u32ESWptr=_HAL_HVD_GetESWritePtr();
            MS_U32 u32ESWptrtmp=data<<3;
            if( ( pHVDCtrl_Hal->u32LastESRptr < u32ESWptr )
                && ( u32ESWptrtmp > u32ESWptr ) )
            {
                HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  u32ESWptrtmp , pHVDCtrl_Hal->u32LastESRptr, u32ESWptr  );
            }
        }
    */
        if( data >= 1)
        {
            data -=1;
        }
        else
        {
            data=(pHVDCtrl_Hal->MemMap.u32BitstreamBufSize>>3)-1;
        }

        data<<=3;// unit

    #else
        data<<=3;// unit
        if( (pHVDCtrl_Hal->InitParams.u32ModeFlag&E_HVD_INIT_INPUT_MASK )==E_HVD_INIT_INPUT_DRV)
        {
            MS_U32 u32ESWptr=_HAL_HVD_GetESWritePtr();
            if( ( pHVDCtrl_Hal->u32LastESRptr < u32ESWptr )
                && ( data > u32ESWptr ) )
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  data , pHVDCtrl_Hal->u32LastESRptr, u32ESWptr  );
                data = u32ESWptr;
            }
            else if( ( pHVDCtrl_Hal->u32LastESRptr == u32ESWptr )
                && ( data > u32ESWptr ) )
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  data , pHVDCtrl_Hal->u32LastESRptr, u32ESWptr  );
                data = u32ESWptr;
            }
            else if(  (_HAL_HVD_GetBBUQNumb() ==0) &&  (( data - u32ESWptr )< 32) && (( _HAL_HVD_GetFWState() & E_HVD_FW_STATE_MASK )==E_HVD_FW_PLAY))
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  data , pHVDCtrl_Hal->u32LastESRptr, u32ESWptr  );
                data = u32ESWptr;
            }
            else if (((data > u32ESWptr) && (pHVDCtrl_Hal->u32LastESRptr > data))
                && ((data - u32ESWptr) < 32)
                && (pHVDCtrl_Hal->u32FlushRstPtr == 1))
            {
                //HVD_MSG_INFO("444HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  u32Data , pCtrl->u32LastESRptr, u32ESWptr  );
                data = u32ESWptr;
            }
        }
    #endif
        // remove illegal pointer
        #if 1
        if( (pHVDCtrl_Hal->MemMap.u32DrvProcessBufSize !=0) &&  (pHVDCtrl_Hal->MemMap.u32DrvProcessBufAddr !=0) )
        {
            MS_U32 u32PacketStaddr = data + pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr;
            if(  ( (pHVDCtrl_Hal->MemMap.u32DrvProcessBufAddr <= u32PacketStaddr ) &&
                   (u32PacketStaddr < (pHVDCtrl_Hal->MemMap.u32DrvProcessBufAddr + pHVDCtrl_Hal->MemMap.u32DrvProcessBufSize)) )  )
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is located in drv process buffer(%lx %lx)\n" ,  data , pHVDCtrl_Hal->u32LastESRptr,  pHVDCtrl_Hal->MemMap.u32DrvProcessBufAddr  ,   pHVDCtrl_Hal->MemMap.u32DrvProcessBufSize  );
                data = pHVDCtrl_Hal->u32LastESRptr;
            }
        }
        #endif
    }
    else
    {
        data =pHVDShareMem->u32ESReadPtr;
    }

    pHVDCtrl_Hal->u32LastESRptr = data;
    //return data;
    return  data;
}

static MS_BOOL _HAL_HVD_SetCMDArg(MS_U32 u32Arg)
{
    MS_U16 u16TimeOut = 0xFFFF;
    MS_BOOL bResult = FALSE;

    //HVD_MSG_DEG("Send argument 0x%lx to HVD \n", u32Arg);
    while(--u16TimeOut)
    {
        if( _HAL_HVD_MBoxReady(HAL_HVD_CMD_MBOX)&&
            _HAL_HVD_MBoxReady(HAL_HVD_CMD_ARG_MBOX))
        {
            bResult = _HAL_HVD_MBoxSend(HAL_HVD_CMD_ARG_MBOX, u32Arg);
            break;
        }
    }
    return bResult;
}

static MS_BOOL _HAL_HVD_SetCMD(MS_U32 u32Cmd)
{
    MS_U16 u16TimeOut = 0xFFFF;
    MS_BOOL bResult = FALSE;

    //HVD_MSG_DEG("Send CMD 0x%lx to HVD \n", u32Cmd);
    while(--u16TimeOut)
    {
        if(_HAL_HVD_MBoxReady(HAL_HVD_CMD_MBOX))
        {
            bResult = _HAL_HVD_MBoxSend(HAL_HVD_CMD_MBOX, u32Cmd);
            break;
        }
    }
    return bResult;
}

static HVD_Return _HAL_HVD_SendCmd( MS_U32 u32Cmd , MS_U32 u32CmdArg)
{
    MS_U32 u32timeout= HVD_GetSysTime_ms() +u32HVDCmdTimeout ;
    while( !_HAL_HVD_SetCMDArg( u32CmdArg )  )
    {
        if( HVD_GetSysTime_ms()  >  u32timeout )
        {
            HVD_MSG_ERR( "HVD cmd:%lx ;cmd arg timeout:%lx\n" , u32Cmd , u32CmdArg  );
            return E_HVD_RETURN_TIMEOUT;
        }
        if( u32Cmd == E_HVD_CMD_STOP  )
        {
            _HAL_HVD_MBoxSend(HAL_HVD_CMD_MBOX, E_HVD_CMD_STOP);
            _HAL_HVD_MBoxSend(HAL_HVD_CMD_ARG_MBOX, 0);
            HVD_MSG_ERR( "HVD cmd force stop:%lx ;cmd arg:%lx\n" , u32Cmd , u32CmdArg  );
            return E_HVD_RETURN_SUCCESS;
        }
        //_HAL_HVD_GetPC();
        HAL_HVD_Dump_FW_Status();
        HAL_HVD_Dump_HW_Status(HVD_U32_MAX);
    }
    u32timeout= HVD_GetSysTime_ms() +u32HVDCmdTimeout ;
    while( !_HAL_HVD_SetCMD( u32Cmd )  )
    {
        if( HVD_GetSysTime_ms()  >  u32timeout )
        {
            HVD_MSG_ERR( " cmd timeout: %lx\n" , u32Cmd );
            return E_HVD_RETURN_TIMEOUT;
        }
        //_HAL_HVD_GetPC();
        HAL_HVD_Dump_FW_Status();
        HAL_HVD_Dump_HW_Status(HVD_U32_MAX);
    }
    return E_HVD_RETURN_SUCCESS;
}

static void _HAL_HVD_SetMIUProtectMask(MS_BOOL bEnable)
{
#if HAL_HVD_ENABLE_MIU_PROTECT
    _HVD_MIU_SetReqMask(MVD_RW, bEnable);
    _HVD_MIU_SetReqMask(MVD_BBU_R, bEnable);
    _HVD_MIU_SetReqMask(HVD_RW, bEnable);
    _HVD_MIU_SetReqMask(HVD_BBU_R, bEnable);
    HAL_VPU_MIU_RW_Protect( bEnable );
    //HVD_Delay_ms(1);
#endif
    return;
}

static void _HAL_HVD_RstMVDParser(void)
{
    _HAL_HVD_SetMIUProtectMask(TRUE);
//    _HVD_WriteRegBit(MVD_REG_STAT_CTRL, 1, MVD_REG_CTRL_RST|MVD_REG_CTRL_INIT);
    _HVD_WriteWordMask(MVD_REG_STAT_CTRL, MVD_REG_CTRL_RST|MVD_REG_DISCONNECT_MIU, MVD_REG_CTRL_RST|MVD_REG_DISCONNECT_MIU);
    HVD_Delay_ms(1); // Delay 2ms to make MVD reset complete
    _HVD_WriteWordMask(MVD_REG_STAT_CTRL, 0, MVD_REG_CTRL_RST|MVD_REG_DISCONNECT_MIU);
    _HAL_HVD_SetMIUProtectMask(FALSE);
    return;
}

static void _HAL_SVD_Release(void)
{
    // release SW reset
    _HVD_WriteWordMask(HVD_REG_RESET, 0, HVD_REG_RESET_SWRST);

    // release cpu rst
    HAL_VPU_SwRstRelse();
}

static void _HAL_HVD_SetBufferAddr(void)
{
    MS_U16 u16Reg       = 0;
    MS_U32 u32StAddr    = 0;
    MS_BOOL bBitMIU1    = FALSE;
    MS_BOOL bCodeMIU1   = FALSE;

    // nal table settngs
    if(pHVDCtrl_Hal->MemMap.u32CodeBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
    {
        bCodeMIU1 = TRUE;
    }
    if(pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
    {
        bBitMIU1 = TRUE;
    }
    if(bBitMIU1 != bCodeMIU1)
    {
        u32StAddr = pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr +
                    pHVDCtrl_Hal->u32BBUTblInBitstreamBufAddr;

        if(u32StAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
        }
    }
    else
    {
        u32StAddr = pHVDCtrl_Hal->MemMap.u32CodeBufAddr +
                    HVD_BBU_DRAM_ST_ADDR;

        if(u32StAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
        }
    }
    HVD_MSG_DEG("_HAL_HVD_SetBufferAddr: nal StAddr:%lx \n", u32StAddr);
    _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L, (MS_U16)(u32StAddr >> 3));
    _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H, (MS_U16)(u32StAddr >> 19));
    // -1 is for NAL_TAB_LEN counts from zero.
    _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN, (MS_U16)(u32BBUEntryNum - 1));

    // ES buffer
    u32StAddr = pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr;
    if(u32StAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
    {
        u32StAddr -= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
    }
    HVD_MSG_DEG("_HAL_HVD_SetBufferAddr: ESb StAddr:%lx \n", u32StAddr);
    _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L, HVD_LWORD(u32StAddr >> 3));
    _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H, HVD_HWORD(u32StAddr >> 3));

    _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L,
                    HVD_LWORD(pHVDCtrl_Hal->MemMap.u32BitstreamBufSize >> 3));
    _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H,
                    HVD_HWORD(pHVDCtrl_Hal->MemMap.u32BitstreamBufSize >> 3));

    // others
    u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU);
    if((pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_TSP)
    {
        u16Reg |= HVD_REG_BBU_TSP_INPUT;
    }
    else
    {
        u16Reg &= ~HVD_REG_BBU_TSP_INPUT;
    }
    u16Reg &= ~HVD_REG_BBU_PASER_MASK;
    if((pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM)   // RM
    {
        u16Reg |= HVD_REG_BBU_PASER_DISABLE;   // force BBU to remove nothing, RM only
    }
    else    // AVS or AVC
    {
        if((pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_START_CODE_MASK) == E_HVD_INIT_START_CODE_REMOVED)
        {
            u16Reg |= HVD_REG_BBU_PASER_ENABLE_03;
        }
        else    // start code remained
        {
            u16Reg |= HVD_REG_BBU_PASER_ENABLE_ALL;
        }
    }
    u16Reg |= HVD_REG_BBU_AUTO_NAL_TAB;
    _HVD_Write2Byte(HVD_REG_MIF_BBU, u16Reg);

    // MIF offset
#if 0
    {
        MS_U16 offaddr=0;
        u32StAddr = pHVDCtrl_Hal->MemMap.u32CodeBufAddr;
        if( u32StAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr  )
        {
            u32StAddr-=pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
        }
        HVD_MSG_DEG("_HAL_HVD_SetBufferAddr: MIF offset:%lx \n" , u32StAddr);
        offaddr = (MS_U16)((u32StAddr )>>20);
        offaddr &= BMASK(  HVD_REG_MIF_OFFSET_L_BITS :0 );//0x1FF;   // 9 bits(L + H)
        u16Reg=_HVD_Read2Byte(HVD_REG_MIF_BBU) ;
        u16Reg&= ~HVD_REG_MIF_OFFSET_H ;
        u16Reg&=~(BMASK( HVD_REG_MIF_OFFSET_L_BITS :0)) ;
        if(offaddr & BIT( HVD_REG_MIF_OFFSET_L_BITS ) )
        {
            u16Reg |= HVD_REG_MIF_OFFSET_H;
        }
        _HVD_Write2Byte(HVD_REG_MIF_BBU,
                       (u16Reg | (offaddr & BMASK(HVD_REG_MIF_OFFSET_L_BITS:0))));
    }
#endif
}

static MS_U32 _HAL_HVD_GetESLevel(void)
{
    MS_U32 u32Wptr = 0;
    MS_U32 u32Rptr = 0;
    MS_U32 u32CurMBX=0;
    MS_U32 u32ESsize =0;
    MS_U32 u32Ret=E_HVD_ESB_LEVEL_NORMAL;

    u32Wptr = _HAL_HVD_GetESWritePtr();
    u32Rptr = _HAL_HVD_GetESReadPtr(FALSE);
    u32ESsize = pHVDCtrl_Hal->MemMap.u32BitstreamBufSize;
    if(u32Rptr >= u32Wptr)
    {
        u32CurMBX = u32Rptr - u32Wptr;
    }
    else
    {
        u32CurMBX = u32ESsize - (u32Wptr - u32Rptr);
    }

    if(  u32CurMBX == 0)
    {
        u32Ret = E_HVD_ESB_LEVEL_UNDER;
    }
    else if (u32CurMBX < HVD_FW_AVC_ES_OVER_THRESHOLD)
    {
        u32Ret = E_HVD_ESB_LEVEL_OVER;
    }
    else
    {
        u32CurMBX = u32ESsize - u32CurMBX;
        if (u32CurMBX < HVD_FW_AVC_ES_UNDER_THRESHOLD)
        {
            u32Ret = E_HVD_ESB_LEVEL_UNDER;
        }
    }
    return u32Ret;
}

MS_BOOL _HAL_HVD_SwCPURst(void)
{
    MS_U16  u16Timeout = 1000;

    _HAL_HVD_SetMIUProtectMask(TRUE);

    // re-setup clock.
    HAL_HVD_PowerCtrl(TRUE);

    HAL_VPU_SwRst();
    _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_SWRST , HVD_REG_RESET_SWRST);

    while(u16Timeout)
    {
        if( (_HVD_Read2Byte(HVD_REG_RESET) & (HVD_REG_RESET_SWRST_FIN))
           == (HVD_REG_RESET_SWRST_FIN))
        {
            break;
        }
        u16Timeout--;
    }

    _HAL_HVD_SetMIUProtectMask(FALSE);

    if( !u16Timeout )
    {
        HVD_MSG_ERR("_HAL_HVD_SwCPURst timeout \n");
    }

    return (u16Timeout>0) ? TRUE : FALSE;
}

MS_BOOL _HAL_HVD_LoadVLCTable(HVD_FWInputSourceType eType  )
{
#if HVD_ENABLE_RV_FEATURE
    if(  eType == E_HVD_FW_INPUT_SOURCE_FLASH )
    {
    #if HVD_ENABLE_BDMA_FW_FLASH_2_SDRAM
        HVD_MSG_DEG("HVD Loading VLC outF2D: dest:0x%lx source:%lx size:%lx\n",  (MS_U32)pHVDCtrl_Hal->MemMap.u32FrameBufAddr+u32RV_VLCTableAddr ,
            ((MS_U32)pHVDCtrl_Hal->MemMap.u32VLCBinaryAddr) , (MS_U32)pHVDCtrl_Hal->MemMap.u32VLCBinarySize);
        if( pHVDCtrl_Hal->MemMap.u32VLCBinarySize != 0  )
        {
            SPIDMA_Dev cpyflag=E_SPIDMA_DEV_MIU1;
            if( pHVDCtrl_Hal->MemMap.u32FrameBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr )
            {
                cpyflag = E_SPIDMA_DEV_MIU1;
            }
            else
            {
                cpyflag = E_SPIDMA_DEV_MIU0;
            }
            if(  !HVD_FLASHcpy( (MS_U32)pHVDCtrl_Hal->MemMap.u32FrameBufAddr+u32RV_VLCTableAddr , pHVDCtrl_Hal->MemMap.u32VLCBinaryAddr ,  pHVDCtrl_Hal->MemMap.u32VLCBinarySize , cpyflag ) )
            {
                HVD_MSG_ERR("HVD ERR: HVD_BDMAcpy VLC table Flash 2 DRAM failed: dest:0x%lx src:0x%lx size:0x%lx flag:%lu\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr  , pHVDCtrl_Hal->MemMap.u32FWBinaryAddr  , pHVDCtrl_Hal->MemMap.u32FWBinarySize  , (MS_U32)cpyflag  );
                return FALSE;
            }
        }
        else
        {
            HVD_MSG_ERR("HVD ERR: During copy VLC from Flash to Dram, the source size of FW is zero\n");
            return FALSE;
        }
    #else
        HVD_MSG_ERR("HVD ERR: driver not enable to use BDMA copy VLC from flash 2 sdram.\n");
        return FALSE;
    #endif
    }
    else
    {
        if(  eType == E_HVD_FW_INPUT_SOURCE_DRAM)
        {
            if( (pHVDCtrl_Hal->MemMap.u32VLCBinaryVAddr!= 0) && ( pHVDCtrl_Hal->MemMap.u32VLCBinarySize!= 0 ) )
            {
                HVD_MSG_DEG("HVD Loading VLC outD2D: dest:0x%lx source:%lx size:%lx\n",  pHVDCtrl_Hal->MemMap.u32FrameBufVAddr+u32RV_VLCTableAddr ,
                    ((MS_U32)pHVDCtrl_Hal->MemMap.u32VLCBinaryVAddr) , (MS_U32)pHVDCtrl_Hal->MemMap.u32VLCBinarySize);
                #if HVD_ENABLE_BDMA_2_BITSTREAMBUF
                    BDMA_Result bdmaRlt;
                    MS_U32 u32DstAdd=0 , u32SrcAdd=0 , u32tabsize=0;
                    u32DstAdd = pHVDCtrl_Hal->MemMap.u32FrameBufVAddr+u32RV_VLCTableAddr;
                    u32SrcAdd = pHVDCtrl_Hal->MemMap.u32VLCBinaryVAddr;
                    u32tabsize = pHVDCtrl_Hal->MemMap.u32VLCBinarySize;
                    //bdmaRlt = MDrv_BDMA_MemCopy(u32SrcAdd, u32DstAdd, SLQ_TBL_SIZE);
                    HAL_HVD_FlushMemory();
                    bdmaRlt = HVD_dmacpy( u32DstAdd, u32SrcAdd,  u32tabsize);
                    if (E_BDMA_OK != bdmaRlt)
                    {
                        HVD_MSG_ERR("HVD Err:MDrv_BDMA_MemCopy fail in %s(), ret=%x!\n", __FUNCTION__, bdmaRlt);
                    }
                #else
                HVD_memcpy( (void*)( pHVDCtrl_Hal->MemMap.u32FrameBufVAddr+u32RV_VLCTableAddr ),
                    (void*)(((MS_U32)pHVDCtrl_Hal->MemMap.u32VLCBinaryVAddr)) , pHVDCtrl_Hal->MemMap.u32VLCBinarySize);
                #endif
            }
            else
            {
                HVD_MSG_ERR("HVD ERR: During copy VLC from out Dram to Dram, the source size or virtual address of VLC is zero\n");
                return FALSE;
            }
        }
        else
        {
        #if HVD_ENABLE_EMBEDDED_FW_BINARY
#ifdef HVD_CACHE_TO_UNCACHE_CONVERT
            MS_U8 *pu8HVD_VLC_Binary;
                pu8HVD_VLC_Binary = (MS_U8 *)((MS_U32)u8HVD_VLC_Binary | 0xA0000000);
            HVD_MSG_DEG("HVD Loading VLC inD2D: dest:0x%lx source:%lx size:%lx\n", pHVDCtrl_Hal->MemMap.u32FrameBufVAddr+u32RV_VLCTableAddr,
                ((MS_U32)pu8HVD_VLC_Binary) , (MS_U32)sizeof(u8HVD_VLC_Binary) );
            HVD_memcpy( (void*)( pHVDCtrl_Hal->MemMap.u32FrameBufVAddr+u32RV_VLCTableAddr ),
                (void*)((MS_U32)pu8HVD_VLC_Binary) , sizeof(u8HVD_VLC_Binary)  );
#else
            HVD_MSG_DEG("HVD Loading VLC inD2D: dest:0x%lx source:%lx size:%lx\n", pHVDCtrl_Hal->MemMap.u32FrameBufVAddr+u32RV_VLCTableAddr,
                ((MS_U32)u8HVD_VLC_Binary) , (MS_U32)sizeof(u8HVD_VLC_Binary) );
            HVD_memcpy( (void*)( pHVDCtrl_Hal->MemMap.u32FrameBufVAddr+u32RV_VLCTableAddr ),
                (void*)((MS_U32)u8HVD_VLC_Binary) , sizeof(u8HVD_VLC_Binary)  );
#endif
        #else
            HVD_MSG_ERR("HVD ERR: driver not enable to use embedded VLC binary.\n");
            return FALSE;
        #endif
        }
    }
#endif
    return TRUE;
}

MS_BOOL _HAL_HVD_SetRegCPU(void)
{
    MS_U32 u32FirmVer = 0;
    MS_U32 u32Timeout = 20000;
    MS_BOOL bNeedReloadFW = TRUE;

    HVD_MSG_DEG("HVD HW ver id: 0x%04lx\n", HAL_HVD_Get_HWVersionID()  );

    if(!_HAL_HVD_SwCPURst())
    {
        HVD_MSG_ERR("HVD reset failed...\n");
        return FALSE;
    }
#if HVD_ENABLE_TIME_MEASURE
    HVD_MSG_MUST( "HVD Time Measure:%d (%s %d) \n" , HVD_GetSysTime_ms()  -  u32InitSysTimeBase , __FUNCTION__, __LINE__  );
#endif

    //Check whether need to reload fw or not
    if((TRUE == pHVDCtrl_Hal->bTurboFWMode)
    && (FALSE == HAL_VPU_IsNeedReload(E_VPU_DECODER_HVD)))
    {
        bNeedReloadFW = FALSE;
    }

    if(TRUE == bNeedReloadFW)
    {
        //If we need to reload fw, need to reset vpu fw decoder type first.
        HAL_VPU_SetFWDecoder(E_VPU_DECODER_NONE);

        // load binary
        if(  pHVDCtrl_Hal->MemMap.eFWSourceType == E_HVD_FW_INPUT_SOURCE_FLASH )
        {
        #if HVD_ENABLE_BDMA_FW_FLASH_2_SDRAM
            HVD_MSG_DEG("HVD Loading FW outF2D: dest:0x%lx source:%lx size:%lx\n",  (MS_U32)pHVDCtrl_Hal->MemMap.u32CodeBufAddr ,
                ((MS_U32)pHVDCtrl_Hal->MemMap.u32FWBinaryAddr) , (MS_U32)pHVDCtrl_Hal->MemMap.u32FWBinarySize);
            if( pHVDCtrl_Hal->MemMap.u32FWBinarySize != 0  )
            {
                SPIDMA_Dev cpyflag=E_SPIDMA_DEV_MIU1;
                if( pHVDCtrl_Hal->MemMap.u32CodeBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr )
                {
                    cpyflag = E_SPIDMA_DEV_MIU1;
                }
                else
                {
                    cpyflag = E_SPIDMA_DEV_MIU0;
                }
                if(  !HVD_FLASHcpy( pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32FWBinaryAddr ,  pHVDCtrl_Hal->MemMap.u32FWBinarySize , cpyflag ) )
                {
                    HVD_MSG_ERR("HVD ERR: HVD_BDMAcpy Flash 2 DRAM failed: dest:0x%lx src:0x%lx size:0x%lx flag:%lu\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr  , pHVDCtrl_Hal->MemMap.u32FWBinaryAddr  , pHVDCtrl_Hal->MemMap.u32FWBinarySize  , (MS_U32)cpyflag  );
                    return FALSE;
                }
            }
            else
            {
                HVD_MSG_ERR("HVD ERR: During copy FW from Flash to Dram, the source size of FW is zero\n");
                return FALSE;
            }
        #else
            HVD_MSG_ERR("HVD ERR: driver not enable to use BDMA copy FW Bin from flash 2 sdram.\n");
            return FALSE;
        #endif
        }
        else
        {
            if(  pHVDCtrl_Hal->MemMap.eFWSourceType == E_HVD_FW_INPUT_SOURCE_DRAM)
            {
                if( (pHVDCtrl_Hal->MemMap.u32FWBinaryVAddr != 0) && ( pHVDCtrl_Hal->MemMap.u32FWBinarySize != 0 ) )
                {
                    HVD_MSG_DEG("HVD Loading FW outD2D: dest:0x%lx source:%lx size:%lx\n",  pHVDCtrl_Hal->MemMap.u32CodeBufVAddr ,
                        ((MS_U32)pHVDCtrl_Hal->MemMap.u32FWBinaryVAddr) , (MS_U32)pHVDCtrl_Hal->MemMap.u32FWBinarySize);
                    HVD_memcpy( (void*)( pHVDCtrl_Hal->MemMap.u32CodeBufVAddr ),
                        (void*)(((MS_U32)pHVDCtrl_Hal->MemMap.u32FWBinaryVAddr)) , pHVDCtrl_Hal->MemMap.u32FWBinarySize);
                }
                else
                {
                    HVD_MSG_ERR("HVD ERR: During copy FW from out Dram to Dram, the source size or virtual address of FW is zero\n");
                    return FALSE;
                }
            }
            else
            {
            #if HVD_ENABLE_EMBEDDED_FW_BINARY
#ifdef HVD_CACHE_TO_UNCACHE_CONVERT
                MS_U8 * pu8HVD_FW_Binary;
                pu8HVD_FW_Binary = (MS_U8 *)((MS_U32)u8HVD_FW_Binary | 0xA0000000);
                HVD_MSG_DEG("HVD Loading FW inD2D: dest:0x%lx source:%lx size:%lx\n",  pHVDCtrl_Hal->MemMap.u32CodeBufVAddr ,
                    ((MS_U32)pu8HVD_FW_Binary) , (MS_U32)sizeof(u8HVD_FW_Binary) );
                HVD_memcpy( (void*)( pHVDCtrl_Hal->MemMap.u32CodeBufVAddr ),
                    (void*)((MS_U32)pu8HVD_FW_Binary), sizeof(u8HVD_FW_Binary) );
#else
                HVD_MSG_DEG("HVD Loading FW inD2D: dest:0x%lx source:%lx size:%lx\n",  pHVDCtrl_Hal->MemMap.u32CodeBufVAddr ,
                    ((MS_U32)u8HVD_FW_Binary) , (MS_U32)sizeof(u8HVD_FW_Binary) );
                HVD_memcpy( (void*)( pHVDCtrl_Hal->MemMap.u32CodeBufVAddr ),
                    (void*)((MS_U32)u8HVD_FW_Binary), sizeof(u8HVD_FW_Binary) );
#endif
            #else
                HVD_MSG_ERR("HVD ERR: driver not enable to use embedded FW binary.\n");
                return FALSE;
            #endif
            }
        }

        if( ((pHVDCtrl_Hal->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK )
           == E_HVD_INIT_HW_RM  )
        {
            if( _HAL_HVD_LoadVLCTable( pHVDCtrl_Hal->MemMap.eFWSourceType ) == FALSE)
            {
                return FALSE;
            }
        }
        HAL_HVD_FlushMemory();
        //HVD_MSG_DEG("HVD FW data compare: dest:0x%lx %lx %lx source:%lx %lx %lx\n",
        //    *(MS_U32*)(pHVDCtrl_Hal->MemMap.u32CodeBufVAddr+160) ,*(MS_U32*)(pHVDCtrl_Hal->MemMap.u32CodeBufVAddr+164),*(MS_U32*)(pHVDCtrl_Hal->MemMap.u32CodeBufVAddr+168),
        //    *(MS_U32*)(u8HVD_FW_Binary+160) ,*(MS_U32*)(u8HVD_FW_Binary+164),*(MS_U32*)(u8HVD_FW_Binary+168));

        //When complete loading fw, set vpu fw decoder type
        HAL_VPU_SetFWDecoder(E_VPU_DECODER_HVD);

        HVD_MSG_DEG("HVD Load FW done\n" );
    }

    {
        MS_U32 u32Addr = 0;
        u32Addr = pHVDCtrl_Hal->MemMap.u32CodeBufAddr;
/* //From JANUS and the later chip, need not set the offset when VPU setting.
        if( u32Addr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr  )
        {
            u32Addr-=pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
        }
*/
        HVD_MSG_DEG("_HAL_HVD_SetRegCPU: VPU settings:%lx \n" , u32Addr);
        HAL_VPU_CPUSetting(u32Addr);
    }

    //HVD4, from JANUS and later chip
    switch( ((pHVDCtrl_Hal->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK) )
    {
    case E_HVD_INIT_HW_AVS:
        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_HK_AVS_MODE , HVD_REG_RESET_HK_AVS_MODE|HVD_REG_RESET_HK_RM_MODE  );
        break;
    case E_HVD_INIT_HW_RM:
        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_HK_RM_MODE , HVD_REG_RESET_HK_AVS_MODE|HVD_REG_RESET_HK_RM_MODE  );
        if( pHVDCtrl_Hal->InitParams.pRVFileInfo->RV_Version  )// RV 9,10
        {
            _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_HK_RV9_DEC_MODE , HVD_REG_RESET_HK_RV9_DEC_MODE  );
        }
        else    // RV 8
        {
            _HVD_WriteWordMask(HVD_REG_RESET, 0 , HVD_REG_RESET_HK_RV9_DEC_MODE  );
        }
        break;
    //case E_HVD_INIT_HW_AVC:
    default:
        _HVD_WriteWordMask(HVD_REG_RESET, 0 , HVD_REG_RESET_HK_AVS_MODE|HVD_REG_RESET_HK_RM_MODE  );
        break;
    }

    //T8: use miu128bit
    HVD_MSG_DEG("(be)Miu128 bits Status = %x <<<<<<<\n",_HVD_Read2Byte(HVD_REG_RESET));
    _HVD_Write2Byte(HVD_REG_RESET, (_HVD_Read2Byte(HVD_REG_RESET) | HVD_REG_RESET_MIU_128));
    HVD_MSG_DEG("(af)Miu128 bits Status = %x <<<<<<<\n",_HVD_Read2Byte(HVD_REG_RESET));

    _HAL_HVD_SetBufferAddr();
    _HAL_HVD_RstMVDParser();

    // release sw and cpu rst
    _HAL_SVD_Release();
    HVD_MSG_DEG("HVD CPU/HW release done\n" );

    while(u32Timeout)
    {
        u32FirmVer = HAL_HVD_GetData(E_HVD_GDATA_FW_INIT_DONE);
        if( u32FirmVer != 0)
        {
            u32FirmVer =HAL_HVD_GetData(E_HVD_GDATA_FW_VERSION_ID);
            break;
        }
        u32Timeout--;
        HVD_Delay_ms(1);
    }
    if(u32Timeout > 0)
    {
        HVD_MSG_DEG("HVD firmware version binary:0x%lx if:0x%lx\n", u32FirmVer , (MS_U32)HVD_FW_VERSION);
    }
    else
    {
        _HAL_HVD_GetPC();
        HVD_MSG_ERR("Cannot get HVD firmware version !!%x %lx \n" , (MS_S16)_HVD_Read2Byte(HVD_REG_RESET) , HAL_HVD_GetData(E_HVD_GDATA_FW_VERSION_ID));
        return FALSE;
    }
#if HVD_ENABLE_TIME_MEASURE
    HVD_MSG_MUST( "HVD Time Measure:%d (%s %d) \n" , HVD_GetSysTime_ms()  -  u32InitSysTimeBase , __FUNCTION__, __LINE__  );
#endif

    return TRUE;
}

HVD_Return _HAL_HVD_UpdatePTSTable(HVD_BBU_Info* pInfo)
{
    MS_U32 u32PTSWptr=HVD_U32_MAX;
    MS_U32 u32PTSRptr=HVD_U32_MAX;
    MS_U32 u32DestAddr=0;

    // update R & W ptr
    u32PTSRptr = HAL_VPU_MemRead(u32PTSRptrAddr);

    if (0xFFFFFFFF == u32PTSRptr)
    {
        return E_HVD_RETURN_FAIL;
    }

    //HVD_MSG_DEG("HVD PTS table RPtr:%lx Wptr:%lx\n" ,u32PTSRptr , HAL_VPU_MemRead( u32PTSWptrAddr ) );

    if( u32PTSRptr >= MAX_PTS_TABLE_SIZE )
    {
        HVD_MSG_ERR("HVD ERR: PTS table Read Ptr(%lx) > max table size(%lx) \n" ,u32PTSRptr ,(MS_U32)MAX_PTS_TABLE_SIZE);
        return E_HVD_RETURN_FAIL;
    }
    // check queue is full or not
    u32PTSWptr = u32PTSPreWptr + 1;
    u32PTSWptr %= MAX_PTS_TABLE_SIZE;
    if( u32PTSWptr == u32PTSRptr )
    {
        HVD_MSG_ERR("HVD ERR: PTS table full. Read Ptr(%lx) == new Write ptr(%lx) ,Pre Wptr(%lx) \n" ,u32PTSRptr,u32PTSWptr , u32PTSPreWptr );
        return E_HVD_RETURN_FAIL;
    }
    // add one PTS entry
    PTSEntry.u32ByteCnt =u32PTSByteCnt&HVD_BYTE_COUNT_MASK;
    PTSEntry.u32ID_L=pInfo->u32ID_L;
    PTSEntry.u32ID_H=pInfo->u32ID_H;
    PTSEntry.u32PTS=pInfo->u32TimeStamp;
#if defined(UDMA_FPGA_ENVI)
    u32DestAddr=(pHVDCtrl_Hal->MemMap.u32CodeBufAddr  ) +HVD_PTS_TABLE_ST_OFFSET+(u32PTSPreWptr*sizeof(HVD_PTS_Entry));
    HVD_UDMA_memcpy(  (void*)u32DestAddr , &PTSEntry  ,  sizeof(HVD_PTS_Entry ) );
#else
    u32DestAddr=(pHVDCtrl_Hal->MemMap.u32CodeBufVAddr  ) +HVD_PTS_TABLE_ST_OFFSET+(u32PTSPreWptr*sizeof(HVD_PTS_Entry));
    HVD_memcpy(  (void*)u32DestAddr , &PTSEntry  ,  sizeof(HVD_PTS_Entry ) );
    HAL_HVD_FlushMemory();
#endif

    // update Write ptr
    if( !HAL_VPU_MemWrite(  u32PTSWptrAddr ,  u32PTSWptr) )
    {
        HVD_MSG_ERR("HVD ERR: PTS table SRAM write failed\n"  );
        return E_HVD_RETURN_FAIL;
    }

    u32PTSPreWptr=u32PTSWptr;

    return E_HVD_RETURN_SUCCESS;
}

HVD_Return _HAL_HVD_UpdateESWptr(MS_U32 nal_offset , MS_U32 nal_len)
{
    //---------------------------------------------------
    // item format in nal table:
    // reserved |borken| nal_offset | nal_len
    //  13 bits |1bit  |  29 bits   | 21 bits   (total 8 bytes)
    //---------------------------------------------------
    MS_U32 addr=0;
    MS_U32 u32BBUNewWptr = 0;
    MS_U8 item[8];

//    MS_U8 pbuf[HVD_MAX_PACKET_SIZE]; // temp buffer
    u32BBUNewWptr=u32BBUWptr;
    u32BBUNewWptr++;
    u32BBUNewWptr%=u32BBUEntryNum;

    // prepare nal entry
    item[0] = nal_len & 0xff;
    item[1] = (nal_len >> 8)  & 0xff;
    item[2] = ((nal_len >> 16) & 0x1f ) | ((nal_offset<<5) & 0xe0);
    item[3] = (nal_offset>>3) & 0xff;
    item[4] = (nal_offset>>11) & 0xff;
    item[5] = (nal_offset>>19) & 0xff;
    item[6] = (nal_offset>>27) & 0x07; //including broken bit
    item[7] = 0;

    // add nal entry
#if defined(UDMA_FPGA_ENVI)
    if(u32BBUWptr%2==0)
    {
        g_hvd_nal_fill_pair[0][0] = item[0];
        g_hvd_nal_fill_pair[0][1] = item[1];
        g_hvd_nal_fill_pair[0][2] = item[2];
        g_hvd_nal_fill_pair[0][3] = item[3];
        g_hvd_nal_fill_pair[0][4] = item[4];
        g_hvd_nal_fill_pair[0][5] = item[5];
        g_hvd_nal_fill_pair[0][6] = item[6];
        g_hvd_nal_fill_pair[0][7] = item[7];
        g_hvd_nal_fill_pair[1][0] = 0;
        g_hvd_nal_fill_pair[1][1] = 0;
        g_hvd_nal_fill_pair[1][2] = 0;
        g_hvd_nal_fill_pair[1][3] = 0;
        g_hvd_nal_fill_pair[1][4] = 0;
        g_hvd_nal_fill_pair[1][5] = 0;
        g_hvd_nal_fill_pair[1][6] = 0;
        g_hvd_nal_fill_pair[1][7] = 0;
    }
    else
    {
        g_hvd_nal_fill_pair[1][0] = item[0];
        g_hvd_nal_fill_pair[1][1] = item[1];
        g_hvd_nal_fill_pair[1][2] = item[2];
        g_hvd_nal_fill_pair[1][3] = item[3];
        g_hvd_nal_fill_pair[1][4] = item[4];
        g_hvd_nal_fill_pair[1][5] = item[5];
        g_hvd_nal_fill_pair[1][6] = item[6];
        g_hvd_nal_fill_pair[1][7] = item[7];
    }
    addr = pHVDCtrl_Hal->MemMap.u32CodeBufAddr + HVD_BBU_DRAM_ST_ADDR + ((u32BBUWptr-(u32BBUWptr%2))<<3);
    HVD_UDMA_memcpy((void*)addr, (void*)g_hvd_nal_fill_pair, 16);
#else
    addr = (pHVDCtrl_Hal->MemMap.u32CodeBufVAddr)+ HVD_BBU_DRAM_ST_ADDR + (u32BBUWptr<<3);
    HVD_memcpy((void*)addr, (void*)item, 8);
    HAL_HVD_FlushMemory();
    //HVD_MSG_DEG( "in UpdateESWptr:%lx %lx %lx\n"  , addr , pHVDCtrl_Hal->MemMap.u32CodeBufVAddr  , u32BBUWptr );
#endif
    // add nal ptr
    //_HAL_HVD_SetBBUWriteptr( HVD_LWORD(u32BBUNewWptr) );
    u32BBUWptr = u32BBUNewWptr;

    return E_HVD_RETURN_SUCCESS;
}

static MS_U32 _HAL_HVD_GetVUIDispInfo(void)
{
    if( (pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_AVC)
    {
        MS_U32 VUIstaddr = 0;
        MS_U16 u16Count=0;
        MS_U32 *pData = (MS_U32 *)(&g_hvd_VUIINFO);
        HAL_HVD_ReadMemory();
        VUIstaddr=pHVDShareMem->u32AVC_VUIDispInfo_Addr;
        for(u16Count =0; u16Count < sizeof(HVD_AVC_VUI_DISP_INFO); u16Count+=4)
        {
            *pData = HAL_VPU_MemRead(VUIstaddr + u16Count);
            pData++;
        }
    }
    else
    {
        HVD_memset( &g_hvd_VUIINFO , 0 , sizeof(HVD_AVC_VUI_DISP_INFO) );
    }
    return (MS_U32 )(&g_hvd_VUIINFO);
}

MS_U32 _HAL_HVD_GetBBUQNumb(void)
{
    MS_U32 u32ReadPtr=0;
    MS_U32 eRet=0;

    u32ReadPtr =_HAL_HVD_GetBBUReadptr();
	//HVD_MSG_DEG("_HAL_HVD_GetBBUQNumb:%lx %lx %lx\n" , u32ReadPtr , u32BBUWptr ,(MS_U32)u32BBUEntryNum );
    if( u32BBUWptr >= u32ReadPtr  )
    {
        eRet = u32BBUWptr - u32ReadPtr;
    }
    else
    {
        eRet = u32BBUEntryNum -( u32ReadPtr -u32BBUWptr  );
    }
    return eRet;
}

MS_U32 _HAL_HVD_GetPTSQNumb(void)
{
    MS_U32 u32ReadPtr=0;
    MS_U32 eRet=0;

    u32ReadPtr = HAL_VPU_MemRead(u32PTSRptrAddr);

    if (0xFFFFFFFF == u32ReadPtr)
    {
        return 0;
    }

    if( u32ReadPtr >= MAX_PTS_TABLE_SIZE )
    {
        HVD_MSG_ERR("HVD ERR: GetPTSQNumb: PTS table Read Ptr(%lx) > max table size(%lx) \n" ,u32ReadPtr ,(MS_U32)MAX_PTS_TABLE_SIZE);
        return 0;
    }
    //HVD_MSG_DEG("_HAL_HVD_GetBBUQNumb:%lx %lx %lx\n" , u32ReadPtr , u32BBUWptr ,(MS_U32)u32BBUEntryNum );
    if( u32PTSPreWptr >= u32ReadPtr  )
    {
        eRet = u32PTSPreWptr - u32ReadPtr;
    }
    else
    {
        eRet = MAX_PTS_TABLE_SIZE -( u32ReadPtr -u32PTSPreWptr  );
    }

    return eRet;
}

#define _HVD_DQ
#ifdef _HVD_DQ
#define HVD_DISPQ_PREFETCH_COUNT        2
static MS_U16 _u16DispQPtr = 0;
static HVD_Frm_Information* _HAL_HVD_GetNextDispFrame(void)
{
    MS_U16 u16QNum = pHVDShareMem->u16DispQNumb;
    MS_U16 u16QPtr = pHVDShareMem->u16DispQPtr;
    static volatile HVD_Frm_Information *pHvdFrm = NULL;

    if (u16QNum > HVD_DISPQ_PREFETCH_COUNT) u16QNum = HVD_DISPQ_PREFETCH_COUNT;

    //printf("Q: %d %d\n", u16QNum, u16QPtr);
    //search the next frame to display
    while (u16QNum != 0)
    {
        pHvdFrm = (volatile HVD_Frm_Information *)&pHVDShareMem->DispQueue[u16QPtr];

        //printf("Q2: %ld\n", pHVDShareMem->DispQueue[u16QPtr].u32Status);
        if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
        {
            _u16DispQPtr = u16QPtr;
            pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW; /////Change its state!!
            HVD_MSG_DEG("FrameDone: %d, pHvdFrm=0x%lx, timestamp=%ld\n", u16QPtr,
                (MS_U32)pHvdFrm, pHVDShareMem->DispQueue[u16QPtr].u32TimeStamp);
            HVD_MSG_INFO("<<< halHVD pts,idH = %lu, %lu [%x]\n", pHvdFrm->u32TimeStamp, pHvdFrm->u32ID_H, u16QPtr);    //STS output
            return (HVD_Frm_Information *)pHvdFrm;
        }
        u16QNum--;
        //go to next frame in the dispQ
        u16QPtr++;
        if (u16QPtr == pHVDShareMem->u16DispQSize) u16QPtr = 0; //wrap to the begin
    }
    return NULL;
}
#endif

void HAL_HVD_FlushMemory(void)
{
    MsOS_FlushMemory();
}

void HAL_HVD_ReadMemory(void)
{
    MsOS_ReadMemory();
}

void HAL_HVD_CheckMIUSel(MS_BOOL bChange)
{
#if 1
    return;
#else
#if defined(CHIP_U3)
    if( pHVDCtrl_Hal->InitParams.bDynamicScaling )
    {
        if( pHVDCtrl_Hal->MemMap.u32DynSacalingBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr )
        {
            if( !((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(13)) == BIT(13))  )
            {
                HVD_MSG_ERR("HVD Drv Err: dynamic scaling address(%lx) is at MIU1, but MIU sel is set(VPU qdma WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(13))  );
                if( bChange )
                {
                    // VPU qdma WR
                    _HVD_WriteWordMask(MIU0_REG_SEL2 , BIT(13), BIT(13));
                }
            }
        }
        else
        {
            if( ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(13)) == BIT(13))  )
            {
                HVD_MSG_ERR("HVD Drv Err: dynamic scaling address(%lx) is at MIU0, but MIU sel is set(VPU qdma WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(13))  );
                if( bChange )
                {
                    // VPU qdma WR
                    _HVD_WriteWordMask(MIU0_REG_SEL2 , BIT(13), BIT(13));
                }
            }
        }
    }
#endif

    if( pHVDCtrl_Hal->MemMap.u32CodeBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr )
    {
    #if defined(CHIP_U3)
        if( !((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(9)) == BIT(9))  )
        {
            HVD_MSG_ERR("HVD Drv Err: code start address(%lx) is at MIU1, but MIU sel is set(VPU d-cache WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(9)));
            if( bChange )
            {
                // VPU d-cache WR
                _HVD_WriteWordMask(MIU0_REG_SEL2 , BIT(9), BIT(9));
            }
        }
        if( !((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(13)) == BIT(13))  )
        {
            HVD_MSG_ERR("HVD Drv Err: code start address(%lx) is at MIU1, but MIU sel is set(VPU qdma WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(13))  );
            if( bChange )
            {
                // VPU qdma WR
                _HVD_WriteWordMask(MIU0_REG_SEL2 , BIT(13), BIT(13));
            }
        }
        if( !((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(13)) == BIT(13))  )
        {
            HVD_MSG_ERR("HVD Drv Err: code start address(%lx) is at MIU1, but MIU sel is set(VPU i-cache WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(13)) );
            if( bChange )
            {
                // VPU i-cache WR
                _HVD_WriteWordMask(MIU0_REG_SEL0 , BIT(13), BIT(13));
            }
        }
    #endif
    #if defined(CHIP_T3)
        if( !((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(7)) == BIT(7))  )
        {
            HVD_MSG_ERR("HVD Drv Err: code start address(%lx) is at MIU1, but MIU sel is set(VPU d-cache WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr ,  (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(7)) );
            if( bChange )
            {
                // VPU d-cache WR
                _HVD_WriteWordMask(MIU0_REG_SEL0 , BIT(7), BIT(7));
            }
        }
        if( !((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(8)) == BIT(8))  )
        {
            HVD_MSG_ERR("HVD Drv Err: code start address(%lx) is at MIU1, but MIU sel is set(VPU qdma WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr ,  (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(8)) );
            if( bChange )
            {
                // VPU qdma WR
                _HVD_WriteWordMask(MIU0_REG_SEL0 , BIT(8), BIT(8));
            }
        }
        if( !((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(9)) == BIT(9))  )
        {
            HVD_MSG_ERR("HVD Drv Err: code start address(%lx) is at MIU1, but MIU sel is set(VPU i-cache WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr,  (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(9)) );
            if( bChange )
            {
                // VPU i-cache WR
                _HVD_WriteWordMask(MIU0_REG_SEL2 , BIT(9), BIT(9));
            }
        }
    #endif
    }
    if( pHVDCtrl_Hal->MemMap.u32FrameBufAddr>= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr )
    {
    #if defined(CHIP_U3)
        if( !((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(12)) == BIT(12))  )
        {
            HVD_MSG_ERR("HVD Drv Err: Frame Buf address(%lx) is at MIU1, but MIU sel is set(HVD RW) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(12)));
            if( bChange )
            {
                // HVD RW
                _HVD_WriteWordMask(MIU0_REG_SEL0 , BIT(12), BIT(12));
            }
        }
    #endif
    #if defined(CHIP_T3)
        if( !((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(10)) == BIT(10))  )
        {
            HVD_MSG_ERR("HVD Drv Err: Frame Buf address(%lx) is at MIU1, but MIU sel is set(HVD RW) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(10))  );
            if( bChange )
            {
                // HVD RW
                _HVD_WriteWordMask(MIU0_REG_SEL0 , BIT(10), BIT(10));
            }
        }
        if( !((_HVD_Read2Byte(MIU0_REG_SEL3) & BIT(4)) == BIT(4))  )
        {
            HVD_MSG_ERR("HVD Drv Err: Frame Buf address(%lx) is at MIU1, but MIU sel is set(MVD WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL3) & BIT(4)) );
            if( bChange )
            {
                // MVD WR
                _HVD_WriteWordMask(MIU0_REG_SEL3 , BIT(4), BIT(4));
            }
        }
    #endif
    }
    if( pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr>= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr )
    {
    #if defined(CHIP_U3)
        if( !((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(0)) == BIT(0))  )
        {
            HVD_MSG_ERR("HVD Drv Err: Bitstream Buf address(%lx) is at MIU1, but MIU sel is set(HVD BBU R) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(0)) );
            if( bChange )
            {
                // HVD BBU R
                _HVD_WriteWordMask(MIU0_REG_SEL2 , BIT(0), BIT(0));
            }
        }
        if( !((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(6)) == BIT(6))  )
        {
            HVD_MSG_ERR("HVD Drv Err: Bitstream Buf address(%lx) is at MIU1, but MIU sel is set(MVD WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr, (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(6)) );
            if( bChange )
            {
                // MVD WR
                _HVD_WriteWordMask(MIU0_REG_SEL2 , BIT(6), BIT(6));
            }
        }
    #endif
    #if defined(CHIP_T3)
        if( !((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(11)) == BIT(11))  )
        {
            HVD_MSG_ERR("HVD Drv Err: Bitstream Buf address(%lx) is at MIU1, but MIU sel is set(HVD BBU R) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr ,  (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(11)) );
            if( bChange )
            {
                // HVD BBU R
                _HVD_WriteWordMask(MIU0_REG_SEL0 , BIT(11), BIT(11));
            }
        }
        if( !((_HVD_Read2Byte(MIU0_REG_SEL3) & BIT(5)) == BIT(5))  )
        {
            HVD_MSG_ERR("HVD Drv Err: Bitstream Buf address(%lx) is at MIU1, but MIU sel is set(MVD BBU WR) to MIU0. (MIU1 base:%lx) (reg:%lx)\n" , pHVDCtrl_Hal->MemMap.u32CodeBufAddr , pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr , (MS_U32)(_HVD_Read2Byte(MIU0_REG_SEL3) & BIT(5)) );
            if( bChange )
            {
                // MVD BBU WR
                _HVD_WriteWordMask(MIU0_REG_SEL3 , BIT(5), BIT(5));
            }
        }
    #endif
    }
#endif
}

MS_U32 HAL_HVD_Get_HWVersionID(void)
{
    return _HVD_Read2Byte(HVD_REG_REV_ID);
}

void HAL_HVD_PowerCtrl(MS_BOOL bEnable)
{
    if( bEnable)
    {
        _HVD_WriteByteMask(REG_TOP_HVD, ~TOP_CKG_HVD_DIS , TOP_CKG_HVD_DIS );
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, ~TOP_CLK_HVD_VP8_DIS, TOP_CLK_HVD_VP8_DIS);
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, ~TOP_CLK_MIU_HVD_MASK, TOP_CLK_MIU_HVD_MASK);
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, ~TOP_CLK_RM_MIU_HVD_MASK, TOP_CLK_RM_MIU_HVD_MASK);
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, ~TOP_CLK_MIU_HVD_VP8_MASK, TOP_CLK_MIU_HVD_VP8_MASK);
    }
    else
    {
        _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_DIS , TOP_CKG_HVD_DIS );
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, TOP_CLK_HVD_VP8_DIS, TOP_CLK_HVD_VP8_DIS);
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, TOP_CLK_MIU_HVD_MASK, TOP_CLK_MIU_HVD_MASK);
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, TOP_CLK_RM_MIU_HVD_MASK, TOP_CLK_RM_MIU_HVD_MASK);
        _HVD_WriteWordMask(REG_TOP_HVD_CLK, TOP_CLK_MIU_HVD_VP8_MASK, TOP_CLK_MIU_HVD_VP8_MASK);
    }
    // fix to not inverse
    _HVD_WriteByteMask(REG_TOP_HVD, ~TOP_CKG_HVD_INV , TOP_CKG_HVD_INV );
    switch( u32HVDClockType )
    {
    case 216:
        _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_216MHZ ,  TOP_CKG_HVD_CLK_MASK);
        break;
    case 172:
        _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_172MHZ ,  TOP_CKG_HVD_CLK_MASK);
        break;
    case 160:
        _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_160MHZ ,  TOP_CKG_HVD_CLK_MASK);
        break;
    case 144:
        _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_144MHZ ,  TOP_CKG_HVD_CLK_MASK);
        break;
    default:
        _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_160MHZ ,  TOP_CKG_HVD_CLK_MASK);
        break;
    }
    return;
}

void HAL_HVD_InitRegBase(MS_U32 u32RegBase)
{
    u32HVDRegOSBase = u32RegBase;
    HAL_VPU_InitRegBase( u32RegBase );
}

void HAL_HVD_SetPreCtrlVariables(MS_U32 drvprectrl)
{
    HVD_Pre_Ctrl *pHVDPreCtrl_in = (HVD_Pre_Ctrl*)drvprectrl;
    pHVDPreCtrl_Hal = pHVDPreCtrl_in;
}

HVD_Return HAL_HVD_InitVariables(MS_U32 drvctrl)
{
    HVD_Drv_Ctrl *pHVDCtrl_in = (HVD_Drv_Ctrl*)drvctrl;

    // local variables
    u32PTSPreWptr   = 0;
    u32PTSRptrAddr  = 0;
    u32PTSWptrAddr  = 0;
    u32PTSByteCnt   = 0;
    u32BBUWptr      = 0;

    HVD_memset((void *) g_hvd_nal_fill_pair, 0, 16);

    // global variables
    u32HVDCmdTimeout = pHVDCtrl_in->u32CmdTimeout;
    pHVDCtrl_Hal = pHVDCtrl_in;
    //u32VPUClockType = (MS_U32)pHVDCtrl_in->InitParams.u16DecoderClock;
    //u32HVDClockType = (MS_U32)pHVDCtrl_in->InitParams.u16DecoderClock;
    // Create mutex
    _HAL_HVD_MutexCreate();

    // fill HVD init variables
#if defined(UDMA_FPGA_ENVI)
    pHVDShareMem = &UDMA_pc_HVDShareMem;
    UDMA_fpga_HVDShareMemAddr = pHVDCtrl_in->MemMap.u32CodeBufVAddr + HVD_SHARE_MEM_ST_OFFSET ;
#else
    pHVDShareMem = (volatile HVD_ShareMem *)((pHVDCtrl_in->MemMap.u32CodeBufVAddr) + HVD_SHARE_MEM_ST_OFFSET);
    HVD_MSG_DEG("pHVDShareMem = %lx\n", (MS_U32)pHVDShareMem);
#endif

#if HVD_ENABLE_RV_FEATURE
    if( ((pHVDCtrl_Hal->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM )
    {
        u32BBUEntryNum = RVD_BBU_DRAM_TBL_ENTRY;
        u32BBUEntryNumTH = RVD_BBU_DRAM_TBL_ENTRY_TH;
        if (pHVDCtrl_Hal->MemMap.u32FrameBufSize > RV_VLC_TABLE_SIZE)
        {
            u32RV_VLCTableAddr = pHVDCtrl_Hal->MemMap.u32FrameBufSize - RV_VLC_TABLE_SIZE;
            pHVDCtrl_Hal->MemMap.u32FrameBufSize -= RV_VLC_TABLE_SIZE;
        }
        else
        {
            HVD_MSG_ERR("HAL_HVD_InitVariables failed: frame buffer size too small. FB:%lx min:%lx\n",
                       (MS_U32)pHVDCtrl_Hal->MemMap.u32FrameBufSize, (MS_U32)RV_VLC_TABLE_SIZE);
            return E_HVD_RETURN_INVALID_PARAMETER;
        }
    }
    else
#endif
    {
        u32BBUEntryNum = HVD_BBU_DRAM_TBL_ENTRY;
        u32BBUEntryNumTH = HVD_BBU_DRAM_TBL_ENTRY_TH;
        u32RV_VLCTableAddr = 0;
    }

    if( (( (pHVDCtrl_in->MemMap.u32CodeBufVAddr )<=  (MS_U32)pHVDShareMem)&& ( (MS_U32)pHVDShareMem <= ((pHVDCtrl_in->MemMap.u32CodeBufVAddr )+ pHVDCtrl_in->MemMap.u32CodeBufSize)))
         || (( (pHVDCtrl_in->MemMap.u32BitstreamBufVAddr)<=  (MS_U32)pHVDShareMem)&& ( (MS_U32)pHVDShareMem <= ((pHVDCtrl_in->MemMap.u32BitstreamBufVAddr )+ pHVDCtrl_in->MemMap.u32BitstreamBufSize)))
         || (( (pHVDCtrl_in->MemMap.u32FrameBufVAddr) <=  (MS_U32)pHVDShareMem)&& ( (MS_U32)pHVDShareMem <= ((pHVDCtrl_in->MemMap.u32FrameBufVAddr) + pHVDCtrl_in->MemMap.u32FrameBufSize)))  )
    {
        HVD_MSG_DEG("input memory: %lx %lx %lx %lx\n",
                    pHVDCtrl_in->MemMap.u32CodeBufAddr,
                    pHVDCtrl_in->MemMap.u32FrameBufAddr,
                    pHVDCtrl_in->MemMap.u32BitstreamBufAddr,
                    pHVDCtrl_in->MemMap.u32MIU1BaseAddr);
        return E_HVD_RETURN_SUCCESS;
    }
    else
    {
        HVD_MSG_ERR("HAL_HVD_InitVariables failed: %lx %lx %lx %lx %lx\n",
                    (MS_U32)pHVDShareMem,
                    pHVDCtrl_in->MemMap.u32CodeBufVAddr,
                    pHVDCtrl_in->MemMap.u32FrameBufVAddr,
                    pHVDCtrl_in->MemMap.u32BitstreamBufVAddr,
                    pHVDCtrl_in->MemMap.u32MIU1BaseAddr);
        return E_HVD_RETURN_INVALID_PARAMETER;
    }
}

HVD_Return HAL_HVD_InitShareMem(void)
{
    MS_U32 u32Addr = 0;

    HVD_memset((volatile void *)pHVDShareMem, 0, sizeof(HVD_ShareMem));
    u32Addr = pHVDCtrl_Hal->MemMap.u32FrameBufAddr;
    if (u32Addr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
    {
        u32Addr -= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
    }
    pHVDShareMem->u32FrameRate = pHVDCtrl_Hal->InitParams.u32FrameRate;
    pHVDShareMem->u32FrameRateBase = pHVDCtrl_Hal->InitParams.u32FrameRateBase;
    pHVDShareMem->u32FrameBufAddr = u32Addr;
    pHVDShareMem->u32FrameBufSize = pHVDCtrl_Hal->MemMap.u32FrameBufSize;
    pHVDShareMem->DispInfo.u16DispWidth = 1;
    pHVDShareMem->DispInfo.u16DispHeight = 1;
    pHVDShareMem->u32CodecType = pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK;
    pHVDShareMem->u32CPUClock = u32VPUClockType;
    pHVDShareMem->u32UserCCIdxWrtPtr = 0xFFFFFFFF;
    pHVDShareMem->DispFrmInfo.u32TimeStamp = 0xFFFFFFFF;

    // PreSetControl
    if (pHVDPreCtrl_Hal->bOnePendingBuffer)
    {
        pHVDShareMem->u32PreSetControl |= PRESET_ONE_PENDING_BUFFER;
    }

    if (pHVDPreCtrl_Hal->bFrameRateHandling)
    {
        pHVDShareMem->u32PreSetControl |= PRESET_FRAMERATE_HANDLING;
        pHVDShareMem->u32PreSetFrameRate = pHVDPreCtrl_Hal->u32PreSetFrameRate;
    }

    //Chip info
    pHVDShareMem->u16ChipID = E_MSTAR_CHIP_A3;
    pHVDShareMem->u16ChipECONum = 0;

    HVD_MSG_DEG("[DBG] pHVDCtrl_Hal->InitParams.u32ModeFlag = 0x%lx\n", pHVDCtrl_Hal->InitParams.u32ModeFlag);

    if ((pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK) == E_HVD_INIT_MAIN_FILE_RAW)
    {
        pHVDShareMem->u8SrcMode = E_HVD_SRC_MODE_FILE;
    }
    else if ((pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK) == E_HVD_INIT_MAIN_FILE_TS)
    {
        pHVDShareMem->u8SrcMode = E_HVD_SRC_MODE_TS_FILE;
    }
    else
    {
        pHVDShareMem->u8SrcMode = E_HVD_SRC_MODE_DTV;
    }

#if 1 //From T4 and the later chips, QDMA can support the address more than MIU1 base.
    if(pHVDCtrl_Hal->MemMap.u32CodeBufAddr >= HAL_MIU1_BASE)
    {
        pHVDShareMem->u32FWBaseAddr = (pHVDCtrl_Hal->MemMap.u32CodeBufAddr-HAL_MIU1_BASE) | 0x40000000; //Bit30 sel miu0/1
    }
    else
    {
        pHVDShareMem->u32FWBaseAddr = pHVDCtrl_Hal->MemMap.u32CodeBufAddr;
    }
    //printf("<DBG>QDMA Addr = %lx <<<<<<<<<<<<<<<<<<<<<<<<\n",pHVDShareMem->u32FWBaseAddr);
#else
    u32Addr = pHVDCtrl_Hal->MemMap.u32CodeBufAddr;
    if (u32Addr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
    {
        u32Addr -= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
    }
    pHVDShareMem->u32FWBaseAddr = u32Addr;
#endif
    // RM only
#if HVD_ENABLE_RV_FEATURE
    if( (((pHVDCtrl_Hal->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM)
    && (pHVDCtrl_Hal->InitParams.pRVFileInfo != NULL) )
    {
        MS_U32 i = 0;
        for(i = 0; i < HVD_RM_INIT_PICTURE_SIZE_NUMBER; i++)
        {
            pHVDShareMem->pRM_PictureSize[i].u16Width = pHVDCtrl_Hal->InitParams.pRVFileInfo->ulPicSizes_w[i];
            pHVDShareMem->pRM_PictureSize[i].u16Height = pHVDCtrl_Hal->InitParams.pRVFileInfo->ulPicSizes_h[i];
        }
        pHVDShareMem->u8RM_Version = (MS_U8)pHVDCtrl_Hal->InitParams.pRVFileInfo->RV_Version;
        pHVDShareMem->u8RM_NumSizes = (MS_U8)pHVDCtrl_Hal->InitParams.pRVFileInfo->ulNumSizes;
        u32Addr = pHVDCtrl_Hal->MemMap.u32FrameBufAddr + u32RV_VLCTableAddr;
        if (u32Addr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
        {
            u32Addr -= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr;
        }
        pHVDShareMem->u32RM_VLCTableAddr = u32Addr;
    }
#endif
#if defined(UDMA_FPGA_ENVI)
    HVD_UDMA_memcpy((void *)UDMA_fpga_HVDShareMemAddr,
                    &UDMA_pc_HVDShareMem,
                    sizeof(HVD_Display_Info));
#endif
    HAL_HVD_FlushMemory();
    return E_HVD_RETURN_SUCCESS;
}

HVD_Return HAL_HVD_InitRegCPU( void)
{
    MS_BOOL bInitRet=FALSE;

    //_HAL_HVD_Entry();
    // Init VPU
    {
        VPU_Init_Params VPUInitParams = { E_VPU_CLOCK_216MHZ , FALSE , -1, VPU_DEFAULT_MUTEX_TIMEOUT, FALSE};
        switch( u32VPUClockType )
        {
        case 216:
            VPUInitParams.eClockSpeed=E_VPU_CLOCK_216MHZ;
            break;
        case 192:
            VPUInitParams.eClockSpeed=E_VPU_CLOCK_192MHZ;
            break;
        case 160:
            VPUInitParams.eClockSpeed=E_VPU_CLOCK_160MHZ;
            break;
        case 144:
            VPUInitParams.eClockSpeed=E_VPU_CLOCK_144MHZ;
            break;
        default:
            break;
        }
    #if HAL_HVD_ENABLE_MUTEX_PROTECT
        VPUInitParams.s32VPUMutexID = s32HVDMutexID;
    #endif

        if(pHVDCtrl_Hal->MemMap.u32CodeBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr)
        {
            VPUInitParams.bInMIU1 = TRUE;
        }
        else
        {
            VPUInitParams.bInMIU1 = FALSE;
        }

        HAL_VPU_Init(&VPUInitParams);
    }
    HAL_HVD_MVD_PowerCtrl( TRUE );
    #if 0
    // check MVD power on
    if( _HVD_Read2Byte(REG_TOP_MVD) & (TOP_CKG_MHVD_DIS) )
    {
        HVD_MSG_INFO( "HVD warning: MVD is not power on before HVD init.\n" );
        _HVD_WriteWordMask(REG_TOP_MVD,  0 , TOP_CKG_MHVD_DIS   );
        HVD_Delay_ms(1);
    }
    // Check VPU power on
    if( _HVD_Read2Byte(REG_TOP_VPU) & (TOP_CKG_VPU_DIS) )
    {
        HVD_MSG_INFO( "HVD warning: VPU is not power on before HVD init.\n" );
        _HVD_WriteWordMask(REG_TOP_VPU,  0 , TOP_CKG_VPU_DIS   );
        HVD_Delay_ms(1);
    }
    // check HVD power on
    if( _HVD_Read2Byte(REG_TOP_HVD) & (TOP_CKG_HVD_DIS) )
    {
        HVD_MSG_INFO( "HVD warning: HVD is not power on before HVD init.\n" );
        HAL_HVD_PowerCtrl(TRUE);
        HVD_Delay_ms(1);
    }
    #endif
    bInitRet = _HAL_HVD_SetRegCPU();
    if( !bInitRet )
    {
        //_HAL_HVD_Return( E_HVD_RETURN_FAIL);
        return E_HVD_RETURN_FAIL;
    }
    bInitRet=HAL_HVD_RstPTSCtrlVariable();
    if( !bInitRet )
    {
        //_HAL_HVD_Return( E_HVD_RETURN_FAIL);
        return E_HVD_RETURN_FAIL;
    }
    //_HAL_HVD_Return( E_HVD_RETURN_SUCCESS);
    return E_HVD_RETURN_SUCCESS;
}

HVD_Return HAL_HVD_SetData( HVD_SetData u32type , MS_U32 u32Data)
{
    HVD_Return eRet = E_HVD_RETURN_SUCCESS;
    //_HAL_HVD_Entry();
    switch(u32type)
    {
// share memory
    // switch
    case E_HVD_SDATA_FRAMEBUF_ADDR:
        pHVDShareMem->u32FrameBufAddr = u32Data;
        break;
    case E_HVD_SDATA_FRAMEBUF_SIZE:
        pHVDShareMem->u32FrameBufSize= u32Data;
        break;
    case E_HVD_SDATA_RM_PICTURE_SIZES:
        HVD_memcpy( (volatile void*)pHVDShareMem->pRM_PictureSize, (void*)((HVD_PictureSize*)u32Data) ,  HVD_RM_INIT_PICTURE_SIZE_NUMBER * sizeof( HVD_PictureSize )   );
        break;
    case E_HVD_SDATA_ERROR_CODE:
        pHVDShareMem->u16ErrCode= (MS_U16)u32Data;
        break;
    case E_HVD_SDATA_DISP_INFO_TH:
        HVD_memcpy((volatile void*)&(pHVDShareMem->DispThreshold), (void*)((HVD_DISP_THRESHOLD*)u32Data), sizeof(HVD_DISP_THRESHOLD) );
        break;
// SRAM

// Mailbox
    case E_HVD_SDATA_TRIGGER_DISP:    // HVD HI mbox 0
        if( u32Data != 0)
        {
            _HAL_HVD_MBoxSend( HAL_HVD_REG_DISP_CTL  ,  1 );
        }
        else
        {
            _HAL_HVD_MBoxSend( HAL_HVD_REG_DISP_CTL  ,  0 );
        }
        break;

    case E_HVD_SDATA_GET_DISP_INFO_DONE:
        _HAL_HVD_MBoxClear( HAL_HVD_REG_DISP_INFO_CHANGE );
        break;

    case E_HVD_SDATA_GET_DISP_INFO_START:
        _HAL_HVD_MBoxClear( HAL_HVD_REG_DISP_INFO_COPYED );
        break;

    case E_HVD_SDATA_VIRTUAL_BOX_WIDTH:
        pHVDShareMem->u32VirtualBoxWidth = u32Data;
        break;

    case E_HVD_SDATA_VIRTUAL_BOX_HEIGHT:
        pHVDShareMem->u32VirtualBoxHeight = u32Data;
        break;

#ifdef _HVD_DQ
    case E_HVD_SDATA_DISPQ_STATUS_VIEW:
        if (pHVDShareMem->DispQueue[u32Data].u32Status == E_HVD_DISPQ_STATUS_INIT)
        {
            //printf("DispFrame DqPtr: %d\n", u32Data);
            pHVDShareMem->DispQueue[u32Data].u32Status = E_HVD_DISPQ_STATUS_VIEW;
        }
        break;

    case E_HVD_SDATA_DISPQ_STATUS_DISP:
        if (pHVDShareMem->DispQueue[u32Data].u32Status == E_HVD_DISPQ_STATUS_VIEW)
        {
            //printf("DispFrame DqPtr: %d\n", u32Data);
            pHVDShareMem->DispQueue[u32Data].u32Status = E_HVD_DISPQ_STATUS_DISP;
        }
        break;

    case E_HVD_SDATA_DISPQ_STATUS_FREE:
        if (pHVDShareMem->DispQueue[u32Data].u32Status == E_HVD_DISPQ_STATUS_VIEW)
        {
            pHVDShareMem->DispQueue[u32Data].u32Status = E_HVD_DISPQ_STATUS_FREE;
        }
        break;
#endif //#ifdef _HVD_DQ

    default:
        break;
    }

#if defined(UDMA_FPGA_ENVI)
    if(  u32type & E_HVD_SDATA_SHARE_MEM  )
    {
        HVD_UDMA_memcpy(  (void*)UDMA_fpga_HVDShareMemAddr , &UDMA_pc_HVDShareMem  ,  sizeof(HVD_Display_Info ) );
    }
#endif
    HAL_HVD_FlushMemory();

    //_HAL_HVD_Return( eRet);
    return eRet;
}

MS_S64 HAL_HVD_GetData_EX(HVD_GetData eType)
{
    MS_S64 s64Ret = 0;

    HAL_HVD_ReadMemory();

    switch (eType)
    {
        // report
        case E_HVD_GDATA_PTS_STC_DIFF:
            s64Ret = pHVDShareMem->s64PtsStcDiff;
            break;

        default:
            break;
    }

    return s64Ret;
}

MS_U32 HAL_HVD_GetData( HVD_GetData eType )
{
    MS_U32 u32Ret=0;
    //_HAL_HVD_Entry();
#if defined(UDMA_FPGA_ENVI)
    if( eType & E_HVD_SDATA_SHARE_MEM )
    {
        HVD_UDMA_memcpy( &UDMA_pc_HVDShareMem  , (void*)UDMA_fpga_HVDShareMemAddr   ,  sizeof(HVD_Display_Info ) );
    }
#endif
    HAL_HVD_ReadMemory();
    switch( eType )
    {
// share memory
    // switch
    case E_HVD_GDATA_DISP_INFO_ADDR:
        u32Ret = (MS_U32)(&pHVDShareMem->DispInfo);
        break;
    // report
    case E_HVD_GDATA_PTS:
        u32Ret=pHVDShareMem->DispFrmInfo.u32TimeStamp;
        break;
    case E_HVD_GDATA_DECODE_CNT:
        u32Ret=pHVDShareMem->u32DecodeCnt;
        break;
    case E_HVD_GDATA_DATA_ERROR_CNT:
        u32Ret=pHVDShareMem->u32DataErrCnt;
        break;
    case E_HVD_GDATA_DEC_ERROR_CNT:
        u32Ret=pHVDShareMem->u32DecErrCnt;
        break;
    case E_HVD_GDATA_ERROR_CODE:
        u32Ret=(MS_U32)(pHVDShareMem->u16ErrCode);
        break;
    case E_HVD_GDATA_VPU_IDLE_CNT:
        u32Ret=pHVDShareMem->u32VPUIdleCnt;
        break;
    case E_HVD_GDATA_DISP_FRM_INFO:
        u32Ret=(MS_U32)(&(pHVDShareMem->DispFrmInfo));
        break;
    case E_HVD_GDATA_DEC_FRM_INFO:
        u32Ret=(MS_U32)(&(pHVDShareMem->DecoFrmInfo));
        break;
    case E_HVD_GDATA_ES_LEVEL:
        u32Ret=(MS_U32)(_HAL_HVD_GetESLevel());
        break;

    // user data
    case E_HVD_GDATA_USERDATA_WPTR:
        u32Ret=(MS_U32)(pHVDShareMem->u32UserCCIdxWrtPtr);
        break;
    case E_HVD_GDATA_USERDATA_IDX_TBL_ADDR:
        u32Ret=(MS_U32)(pHVDShareMem->u8UserCCIdx);
        break;
    case E_HVD_GDATA_USERDATA_PACKET_TBL_ADDR:
        u32Ret=(MS_U32)(pHVDShareMem->u32UserCCBase);
        break;
    case E_HVD_GDATA_USERDATA_PACKET_SIZE:
        u32Ret=(MS_U32)(sizeof(DTV_BUF_type));
        break;
    case E_HVD_GDATA_USERDATA_IDX_TBL_SIZE:
        u32Ret=(MS_U32)(USER_CC_IDX_SIZE);
        break;
    case E_HVD_GDATA_USERDATA_PACKET_TBL_SIZE:
        u32Ret=(MS_U32)(USER_CC_DATA_SIZE);
        break;
    // report - modes
    case E_HVD_GDATA_IS_SHOW_ERR_FRM:
        u32Ret=pHVDShareMem->ModeStatus.bIsShowErrFrm;
        break;
    case E_HVD_GDATA_IS_REPEAT_LAST_FIELD:
        u32Ret=pHVDShareMem->ModeStatus.bIsRepeatLastField;
        break;
    case E_HVD_GDATA_IS_ERR_CONCEAL:
        u32Ret=pHVDShareMem->ModeStatus.bIsErrConceal;
        break;
    case E_HVD_GDATA_IS_SYNC_ON:
        u32Ret=pHVDShareMem->ModeStatus.bIsSyncOn;
        break;
    case E_HVD_GDATA_IS_PLAYBACK_FINISH:
        u32Ret=pHVDShareMem->ModeStatus.bIsPlaybackFinish;
        break;
    case E_HVD_GDATA_SYNC_MODE:
        u32Ret=pHVDShareMem->ModeStatus.u8SyncType;
        break;
    case E_HVD_GDATA_SKIP_MODE:
        u32Ret=pHVDShareMem->ModeStatus.u8SkipMode;
        break;
    case E_HVD_GDATA_DROP_MODE:
        u32Ret=pHVDShareMem->ModeStatus.u8DropMode;
        break;
    case E_HVD_GDATA_DISPLAY_DURATION:
        u32Ret=pHVDShareMem->ModeStatus.s8DisplaySpeed;
        break;
    case E_HVD_GDATA_FRC_MODE:
        u32Ret=pHVDShareMem->ModeStatus.u8FrcMode;
        break;
    case E_HVD_GDATA_NEXT_PTS:
        u32Ret=pHVDShareMem->u32NextPTS;
        break;
#ifdef _HVD_DQ
    case E_HVD_GDATA_DISP_Q_SIZE:
        u32Ret=pHVDShareMem->u16DispQSize;
        break;
    case E_HVD_GDATA_DISP_Q_PTR:
        u32Ret=(MS_U32)_u16DispQPtr;//pHVDShareMem->u16DispQPtr;
        break;
    case E_HVD_GDATA_NEXT_DISP_FRM_INFO:
        u32Ret=(MS_U32)_HAL_HVD_GetNextDispFrame();
        break;
#endif
    case E_HVD_GDATA_REAL_FRAMERATE:
        u32Ret=pHVDShareMem->u32RealFrameRate;
        break;
    case E_HVD_GDATA_IS_ORI_INTERLACE_MODE:
        u32Ret=(MS_U32)pHVDShareMem->DispInfo.u8IsOriginInterlace;
        break;
    case E_HVD_GDATA_FRM_PACKING_SEI_DATA:
        u32Ret=(MS_U32)pHVDShareMem->u32Frm_packing_arr_data_addr;
        break;
	case E_HVD_GDATA_TYPE_FRAME_MBS_ONLY_FLAG:
        u32Ret=(MS_U32)pHVDShareMem->u8FrameMbsOnlyFlag;
        break;

    // internal control
    case E_HVD_GDATA_IS_1ST_FRM_RDY:
        u32Ret=pHVDShareMem->bIs1stFrameRdy;
        break;
    case E_HVD_GDATA_IS_I_FRM_FOUND:
        u32Ret=pHVDShareMem->bIsIFrmFound;
        break;
    case E_HVD_GDATA_IS_SYNC_START:
        u32Ret=pHVDShareMem->bIsSyncStart;
        break;
    case E_HVD_GDATA_IS_SYNC_REACH:
        u32Ret=pHVDShareMem->bIsSyncReach;
        break;
    case E_HVD_GDATA_FW_VERSION_ID:
        u32Ret=pHVDShareMem->u32FWVersionID;
        break;
    case E_HVD_GDATA_FW_IF_VERSION_ID:
        u32Ret=pHVDShareMem->u32FWIfVersionID;
        break;
    case E_HVD_GDATA_BBU_Q_NUMB:
        u32Ret=_HAL_HVD_GetBBUQNumb();
        break;
    case E_HVD_GDATA_DEC_Q_NUMB:
        u32Ret=pHVDShareMem->u16DecQNumb;
        break;
    case E_HVD_GDATA_DISP_Q_NUMB:
        u32Ret=pHVDShareMem->u16DispQNumb;
        break;
    case E_HVD_GDATA_PTS_Q_NUMB:
        u32Ret=_HAL_HVD_GetPTSQNumb();
        break;
    case E_HVD_GDATA_FW_INIT_DONE:
        u32Ret=pHVDShareMem->bInitDone;
        break;

    // debug
    case E_HVD_GDATA_SKIP_CNT:
        u32Ret=pHVDShareMem->u32SkipCnt;
        break;
    case E_HVD_GDATA_GOP_CNT:
        u32Ret=pHVDShareMem->u32DropCnt;
        break;
    case E_HVD_GDATA_DISP_CNT:
        u32Ret= pHVDShareMem->u32DispCnt;
        break;
    case E_HVD_GDATA_DROP_CNT:
        u32Ret= pHVDShareMem->u32DropCnt;
        break;
    case E_HVD_GDATA_DISP_STC:
        u32Ret= pHVDShareMem->u32DispSTC;
        break;
    case E_HVD_GDATA_VSYNC_CNT:
        u32Ret= pHVDShareMem->u32VsyncCnt;
        break;
    case E_HVD_GDATA_MAIN_LOOP_CNT:
        u32Ret= pHVDShareMem->u32MainLoopCnt;
        break;

    // AVC
    case E_HVD_GDATA_AVC_LEVEL_IDC:
        u32Ret=pHVDShareMem->u16AVC_SPS_LevelIDC;
        break;
    case E_HVD_GDATA_AVC_LOW_DELAY:
        u32Ret=pHVDShareMem->u8AVC_SPS_LowDelayHrdFlag;
        break;
    case E_HVD_GDATA_AVC_VUI_DISP_INFO:
        u32Ret=_HAL_HVD_GetVUIDispInfo();
        break;

// SRAM

// Mailbox
    case E_HVD_GDATA_FW_STATE:       // HVD RISC MBOX 0 (esp. FW init done)
        u32Ret=_HAL_HVD_GetFWState();
        break;
    case E_HVD_GDATA_IS_DISP_INFO_UNCOPYED:
        u32Ret=_HAL_HVD_MBoxReady( HAL_HVD_REG_DISP_INFO_COPYED );
        break;
    case E_HVD_GDATA_IS_DISP_INFO_CHANGE:   // HVD RISC MBOX 1 (rdy only)
        u32Ret=_HAL_HVD_MBoxReady( HAL_HVD_REG_DISP_INFO_CHANGE );
        break;
    case E_HVD_GDATA_HVD_ISR_STATUS:   // HVD RISC MBOX 1 (value only)
        _HAL_HVD_MBoxRead(HAL_HVD_REG_ISR_HVD , &u32Ret );
        break;
    case E_HVD_GDATA_VPU_ISR_STATUS:   // VPU RISC MBOX 1 (value only)
        _HAL_HVD_MBoxRead(HAL_HVD_REG_ISR_VPU , &u32Ret );
        break;
    case E_HVD_GDATA_IS_FRAME_SHOWED:    // HVD HI mbox 0 ( showed: rdy cleared ; not show: rdy enable )
        if(  _HAL_HVD_MBoxReady( HAL_HVD_REG_DISP_CTL ) )
        {
            u32Ret = TRUE;
        }
        else
        {
            u32Ret = FALSE;
        }
        break;
    case E_HVD_GDATA_ES_READ_PTR:    //
        u32Ret= _HAL_HVD_GetESReadPtr(FALSE);
        break;
    case E_HVD_GDATA_ES_WRITE_PTR:   //
        u32Ret= _HAL_HVD_GetESWritePtr();
        break;
    case E_HVD_GDATA_BBU_READ_PTR:
        u32Ret= _HAL_HVD_GetBBUReadptr();
        break;
    case E_HVD_GDATA_BBU_WRITE_PTR:
        u32Ret= u32BBUWptr;
        break;
    case E_HVD_GDATA_BBU_WRITE_PTR_FIRED:
        u32Ret= pHVDCtrl_Hal->u32BBUWptr_Fired;
        break;

    case E_HVD_GDATA_VPU_PC_CNT:
        u32Ret= _HAL_HVD_GetPC();
        break;


// FW def
    case E_HVD_GDATA_FW_MAX_DUMMY_FIFO:  // AVC: 256Bytes AVS: 2kB RM:???
        u32Ret=HVD_MAX3(  HVD_FW_AVC_DUMMY_FIFO , HVD_FW_AVS_DUMMY_FIFO   , HVD_FW_RM_DUMMY_FIFO  );
        break;

    case E_HVD_GDATA_FW_AVC_MAX_VIDEO_DELAY:
        u32Ret=HVD_FW_AVC_MAX_VIDEO_DELAY;
        break;
    case E_HVD_GDATA_FW_BBU_TOTAL_TBL_ENTRY:
        u32Ret=u32BBUEntryNumTH ;
        break;
    case E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB:
        u32Ret=u32BBUEntryNum ;
        break;
    case E_HVD_GDATA_FW_PTS_TOTAL_ENTRY_NUMB:
        u32Ret=MAX_PTS_TABLE_SIZE;
        break;
    case E_HVD_GDATA_FW_DUMMY_WRITE_ADDR:
        u32Ret=HVD_DUMMY_WRITE_ADDR ;
        break;
    case E_HVD_GDATA_FW_DS_BUF_ADDR:
        u32Ret=HVD_DYNAMIC_SCALING_ADDR ;
        break;
    case E_HVD_GDATA_FW_DS_BUF_SIZE:
        //u32Ret = HVD_DYNAMIC_SCALING_SIZE;
        // ----------------------- yi-chun.pan: for Dynamic Scaling 3k/6k issue ----------20111213---
        // ----------------------- modify DRV and AP(SN/MM) first, and then update fw --------------
        u32Ret = (1024 * 3);
        break;
    case E_HVD_GDATA_FW_DS_VECTOR_DEPTH:
        u32Ret=HVD_DYNAMIC_SCALING_DEPTH ;
        break;
    case E_HVD_GDATA_FW_DS_INFO_ADDR:
        u32Ret=HVD_SCALER_INFO_ADDR;
        break;

    default:
        break;
    }
    //_HAL_HVD_Return( u32Ret);
    return u32Ret;
}

HVD_Return HAL_HVD_SetCmd(HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg)
{
    HVD_Return eRet = E_HVD_RETURN_SUCCESS;
    MS_U32 u32Cmd = (MS_U32) eUsrCmd;

    _HAL_HVD_Entry();

    // check if old SVD cmds
    if (u32Cmd < E_HVD_CMD_SVD_BASE)
    {
        HVD_MSG_ERR("HVD Err: Old SVD FW cmd(%lx %lx) used in HVD.\n" , u32Cmd, u32CmdArg);

        _HAL_HVD_Return(E_HVD_RETURN_INVALID_PARAMETER);
    }

    HVD_MSG_DEG("HVD DBG: Send cmd:0x%lx Arg:0x%lx\n", u32Cmd, u32CmdArg);

    eRet = _HAL_HVD_SendCmd(u32Cmd, u32CmdArg);

    _HAL_HVD_Return(eRet);
}


HVD_Return HAL_HVD_DeInit(void)
{
    HVD_Return eRet=E_HVD_RETURN_FAIL;
    //MS_U32 u32FWState=0;
    //MS_U32 Timer=50;    // ms
#if HVD_ENABLE_TIME_MEASURE
    MS_U32 ExitTimeCnt=0;
    ExitTimeCnt = HVD_GetSysTime_ms();
#endif
    eRet=HAL_HVD_SetCmd(E_HVD_CMD_PAUSE, 0);
    if (E_HVD_RETURN_SUCCESS != eRet)
        HVD_MSG_ERR("HVDERR %s(%d) HVD fail to PAUSE %d\n", __FUNCTION__, __LINE__, eRet);

    eRet=HAL_HVD_SetCmd(E_HVD_CMD_STOP, 0);
    _HAL_HVD_MutexDelete();
/*
    while(Timer)
    {
        HVD_Delay_ms(1);
        u32FWState=HAL_HVD_GetData( E_HVD_GDATA_FW_STATE  );
        switch( (pHVDCtrl_Hal->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK )
        {
        case E_HVD_INIT_HW_AVC:
            if(  u32FWState == E_HVD_FW_STOP_DONE)
            {
                Timer=1;
            }
            break;
        case E_HVD_INIT_HW_AVS:
            if(  u32FWState == E_HVD_FW_STOP)
            {
                Timer=1;
            }
            break;
        default:
            break;
        }
        Timer--;
    };
*/
#if HVD_ENABLE_TIME_MEASURE
    HVD_MSG_DEG( "HVD Stop Time(Wait FW):%d\n"   ,  HVD_GetSysTime_ms()-ExitTimeCnt );
#endif
    //check MAU idle before reset VPU
    {
        MS_U32 mau_idle_cnt = 100;// ms
        while(mau_idle_cnt)
        {
            if(TRUE == HAL_VPU_MAU_IDLE())
            {
                break;
            }
            mau_idle_cnt--;
            HVD_Delay_ms(1);
        }

        if(mau_idle_cnt == 0)
        {
            HVD_MSG_ERR("MAU idle time out~~~~~\n");
        }
    }
    HVD_MSG_DEG("%s(%d) HVD hold CPU\n", __FUNCTION__, __LINE__);
    HAL_VPU_SwRst(); //CPU hold
    {
        MS_U16  u16Timeout = 1000;
        _HAL_HVD_SetMIUProtectMask(TRUE);
        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_SWRST , HVD_REG_RESET_SWRST);
        while(u16Timeout)
        {
            if( (_HVD_Read2Byte(HVD_REG_RESET) & (HVD_REG_RESET_SWRST_FIN))
               == (HVD_REG_RESET_SWRST_FIN))
            {
                break;
            }
            u16Timeout--;
        }

        _HAL_HVD_RstMVDParser();

        //HAL_VPU_PowerCtrl(FALSE);
        HAL_VPU_DeInit();
        HAL_VPU_SwRelseMAU();
        HAL_HVD_PowerCtrl(FALSE);
        HAL_HVD_MVD_PowerCtrl( FALSE );

        _HAL_HVD_SetMIUProtectMask(FALSE);
    }
    //HAL_HVD_MVD_PowerCtrl( FALSE );
    return eRet;
}

HVD_Return HAL_HVD_PushPacket(HVD_BBU_Info *pInfo)
{
    HVD_Return eRet = E_HVD_RETURN_UNSUPPORTED;
    MS_U32 u32Addr = 0;

    eRet = _HAL_HVD_UpdatePTSTable(pInfo);

    if (E_HVD_RETURN_SUCCESS != eRet)
    {
        return eRet;
    }

#ifdef _HVD_DQ
    //printf(">>> halHVD pts,idH = %lu, %lu\n", pInfo->u32TimeStamp, pInfo->u32ID_H);    //STS input
#endif

    //T9: for 128 bit memory. BBU need to get 2 entry at a time.
    eRet = _HAL_HVD_UpdateESWptr(0, 0);

    if (E_HVD_RETURN_SUCCESS != eRet)
    {
        return eRet;
    }

    u32Addr = pInfo->u32Staddr;

    if (pInfo->bRVBrokenPacket)
    {
        u32Addr = pInfo->u32Staddr | BIT(HVD_RV_BROKENBYUS_BIT);
    }

    eRet = _HAL_HVD_UpdateESWptr(u32Addr, pInfo->u32Length);

    if (E_HVD_RETURN_SUCCESS != eRet)
    {
        return eRet;
    }

    u32PTSByteCnt += pInfo->u32Length;

    // do not add local pointer
    if ((pHVDCtrl_Hal->MemMap.u32DrvProcessBufSize != 0) && (pHVDCtrl_Hal->MemMap.u32DrvProcessBufAddr != 0))
    {
        MS_U32 u32PacketStAddr = pInfo->u32Staddr + pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr;

        if (!((pHVDCtrl_Hal->MemMap.u32DrvProcessBufAddr <= u32PacketStAddr) &&
              (u32PacketStAddr < (pHVDCtrl_Hal->MemMap.u32DrvProcessBufAddr + pHVDCtrl_Hal->MemMap.u32DrvProcessBufSize))))
        {
            pHVDCtrl_Hal->LastNal.u32NalAddr = pInfo->u32Staddr;
            pHVDCtrl_Hal->LastNal.u32NalSize = pInfo->u32Length;
        }
        else
        {
            //null packet
            pHVDCtrl_Hal->LastNal.u32NalAddr = pInfo->u32OriPktAddr;
            pHVDCtrl_Hal->LastNal.u32NalSize = 0;
        }
    }
    else
    {
        pHVDCtrl_Hal->LastNal.u32NalAddr = pInfo->u32Staddr;
        pHVDCtrl_Hal->LastNal.u32NalSize = pInfo->u32Length;
    }

    pHVDCtrl_Hal->LastNal.bRVBrokenPacket = pInfo->bRVBrokenPacket;
    pHVDCtrl_Hal->u32BBUPacketCnt++;

    return eRet;
}

void HAL_HVD_EnableISR(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, 0, HVD_REG_RISC_ISR_MSK);
    }
    else
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, HVD_REG_RISC_ISR_MSK, HVD_REG_RISC_ISR_MSK);
    }
}

void HAL_HVD_SetForceISR(MS_BOOL bEnable)
{
    if( bEnable )
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, HVD_REG_RISC_ISR_FORCE, HVD_REG_RISC_ISR_FORCE);
    }
    else
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, 0, HVD_REG_RISC_ISR_FORCE);
    }
}

void HAL_HVD_SetClearISR(void)
{
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, HVD_REG_RISC_ISR_CLR , HVD_REG_RISC_ISR_CLR);
}

MS_BOOL HAL_HVD_IsISROccured(void)
{
    return (MS_BOOL)(_HVD_Read2Byte( HVD_REG_RISC_MBOX_RDY )&  HVD_REG_RISC_ISR_VALID );
}

MS_BOOL HAL_HVD_IsEnableISR(void)
{
    if (_HVD_Read2Byte(HVD_REG_RISC_MBOX_CLR) & HVD_REG_RISC_ISR_MSK)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_BOOL HAL_HVD_IsAlive(void)
{
    if (pHVDCtrl_Hal)
    {
        if ((pHVDCtrl_Hal->LivingStatus.u32DecCnt == HAL_HVD_GetData(E_HVD_GDATA_DECODE_CNT))
            && (pHVDCtrl_Hal->LivingStatus.u32SkipCnt == HAL_HVD_GetData(E_HVD_GDATA_SKIP_CNT))
            && (pHVDCtrl_Hal->LivingStatus.u32IdleCnt == HAL_HVD_GetData(E_HVD_GDATA_VPU_IDLE_CNT))
            && (pHVDCtrl_Hal->LivingStatus.u32MainLoopCnt == HAL_HVD_GetData(E_HVD_GDATA_MAIN_LOOP_CNT)))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_HVD_RstPTSCtrlVariable(void)
{
    if(  (pHVDCtrl_Hal->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV  )
    {
#if defined(UDMA_FPGA_ENVI)
        HVD_UDMA_memcpy(  (void*)UDMA_fpga_HVDShareMemAddr , &UDMA_pc_HVDShareMem  ,  sizeof(HVD_Display_Info ) );
#endif
        HAL_HVD_ReadMemory();

        u32PTSRptrAddr=pHVDShareMem->u32PTStableRptrAddr;
        u32PTSWptrAddr=pHVDShareMem->u32PTStableWptrAddr;
        u32PTSByteCnt = pHVDShareMem->u32PTStableByteCnt;
        u32PTSPreWptr=HAL_VPU_MemRead( u32PTSWptrAddr );
        HVD_MSG_DEG( "HVD hal:PTS table: WptrAddr:%lx RptrAddr:%lx ByteCnt:%lx PreWptr:%lx\n"  ,  u32PTSWptrAddr  ,  u32PTSRptrAddr ,u32PTSByteCnt ,u32PTSPreWptr );
    }
    return TRUE;
}

MS_BOOL HAL_HVD_FlushRstShareMem(void)
{
    HVD_memset(  &pHVDShareMem->DecoFrmInfo  ,  0  , sizeof(HVD_Frm_Information)   );
    HAL_HVD_FlushMemory();
    return TRUE;
}


void HAL_HVD_UartSwitch2FW( MS_BOOL bEnable )
{
    if( bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_UART_SEL0, REG_TOP_UART_SEL_VD_MHEG5, REG_TOP_UART_SEL_0_MASK);
    }
    else
    {
    #if defined (__aeon__)
        _HVD_WriteWordMask(REG_TOP_UART_SEL0, REG_TOP_UART_SEL_MHEG5, REG_TOP_UART_SEL_0_MASK);
    #else // defined (__mips__)
        _HVD_WriteWordMask(REG_TOP_UART_SEL0, REG_TOP_UART_SEL_PIU_0, REG_TOP_UART_SEL_0_MASK);
    #endif
    }
}

MS_U32 HAL_HVD_GetData_Dbg( MS_U32 u32Addr )
{
    return 0;
}

void HAL_HVD_SetData_Dbg( MS_U32 u32Addr , MS_U32 u32Data)
{
    return ;
}

MS_U16 HAL_HVD_GetCorretClock(MS_U16 u16Clock)
{
    //if( u16Clock == 0 )
    return 216;//140;
    //if(  )
}

void HAL_HVD_UpdateESWptr_Fire(void)
{
    MS_BOOL bBitMIU1 = FALSE;
    MS_BOOL bCodeMIU1 = FALSE;
    if( pHVDCtrl_Hal->MemMap.u32CodeBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr  )
    {
        bCodeMIU1=TRUE;
    }
    if( pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr >= pHVDCtrl_Hal->MemMap.u32MIU1BaseAddr  )
    {
        bBitMIU1=TRUE;
    }
    if( bBitMIU1  != bCodeMIU1 )
    {
    #if HVD_ENABLE_BDMA_2_BITSTREAMBUF
        BDMA_Result bdmaRlt;
        MS_U32 u32DstAdd=0 , u32SrcAdd=0 , u32tabsize=0;
        u32DstAdd = pHVDCtrl_Hal->MemMap.u32BitstreamBufAddr+pHVDCtrl_Hal->u32BBUTblInBitstreamBufAddr;
        u32SrcAdd = pHVDCtrl_Hal->MemMap.u32CodeBufAddr + HVD_BBU_DRAM_ST_ADDR;
        u32tabsize = u32BBUEntryNum << 3;
        //bdmaRlt = MDrv_BDMA_MemCopy(u32SrcAdd, u32DstAdd, SLQ_TBL_SIZE);
        HAL_HVD_FlushMemory();
        bdmaRlt = HVD_dmacpy( u32DstAdd, u32SrcAdd,  u32tabsize);
        if (E_BDMA_OK != bdmaRlt)
        {
            HVD_MSG_ERR("HVD Err:MDrv_BDMA_MemCopy fail in %s(), ret=%x!\n", __FUNCTION__, bdmaRlt);
        }
    #else
        MS_U32 u32DstAdd=0 , u32SrcAdd=0 , u32tabsize=0;
        u32DstAdd = pHVDCtrl_Hal->MemMap.u32BitstreamBufVAddr+pHVDCtrl_Hal->u32BBUTblInBitstreamBufAddr;
        u32SrcAdd = pHVDCtrl_Hal->MemMap.u32CodeBufVAddr + HVD_BBU_DRAM_ST_ADDR;
        u32tabsize = u32BBUEntryNum << 3;
        HVD_memcpy( u32DstAdd, u32SrcAdd,  u32tabsize);
        HAL_HVD_FlushMemory();
    #endif
    }
//    HVD_MSG_INFO( "HVD Push packet fire:%lu st:%lx size:%lx BBU:%lu %lu\n", pHVDCtrl_Hal->u32BBUPacketCnt ,  pHVDCtrl_Hal->LastNal.u32NalAddr   , pHVDCtrl_Hal->LastNal.u32NalSize  ,   (MS_U32)_HAL_HVD_GetBBUReadptr()  , u32BBUWptr   );
    _HAL_HVD_SetBBUWriteptr( HVD_LWORD(u32BBUWptr) );
    pHVDCtrl_Hal->u32BBUWptr_Fired = u32BBUWptr;
}

void HAL_HVD_MVD_PowerCtrl(MS_BOOL bEnable)
{
    if( bEnable )
    {
        _HVD_WriteWordMask(REG_TOP_MVD,  0 , TOP_CKG_MHVD_DIS   );
        _HVD_WriteWordMask(REG_TOP_MVD2,  0 , TOP_CKG_MHVD2_DIS   );
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_MVD,  TOP_CKG_MHVD_DIS , TOP_CKG_MHVD_DIS   );
        _HVD_WriteWordMask(REG_TOP_MVD2,  TOP_CKG_MHVD2_DIS , TOP_CKG_MHVD2_DIS   );
    }
}

void HAL_HVD_Dump_FW_Status(void)
{
    MS_U32 tmp1=0;
    MS_U32 tmp2=0;
    HAL_HVD_ReadMemory();

    _HAL_HVD_MBoxRead( HAL_HVD_CMD_MBOX  , &tmp1 );
    _HAL_HVD_MBoxRead( HAL_HVD_CMD_ARG_MBOX  , &tmp2 );
    HVD_MSG_DEG("=====HVD Dump Systime:%lu FW Ver:%lx status: %lx Err Code: %lx PC: %lx =====\n" , HVD_GetSysTime_ms() , pHVDShareMem->u32FWVersionID , _HAL_HVD_GetFWState() , (MS_U32)pHVDShareMem->u16ErrCode , HAL_VPU_GetProgCnt() );

    HVD_MSG_DEG("Time: STC:%lu DispT:%lu Dec:%lu PTS(skip,seek):%lu;  Last Cmd:%lx _Arg:%lx _Rdy1:%lx _Rdy2:%lx\n" ,
        pHVDShareMem->u32DispSTC , pHVDShareMem->DispFrmInfo.u32TimeStamp , pHVDShareMem->DecoFrmInfo.u32TimeStamp , pHVDShareMem->u32CurrentPts,
        tmp1  ,   tmp2   ,  (MS_U32)_HAL_HVD_MBoxReady(HAL_HVD_CMD_MBOX) , (MS_U32)_HAL_HVD_MBoxReady(HAL_HVD_CMD_ARG_MBOX) );

    HVD_MSG_DEG("Flag: InitDone:%d SPS_change:%d IFrm:%d 1stFrmRdy:%d Sync_Start:%d _Reach:%d \n" ,
        pHVDShareMem->bInitDone , _HAL_HVD_MBoxReady( HAL_HVD_REG_DISP_INFO_CHANGE )  ,  pHVDShareMem->bIsIFrmFound , pHVDShareMem->bIs1stFrameRdy ,
        pHVDShareMem->bIsSyncStart , pHVDShareMem->bIsSyncReach );

    HVD_MSG_DEG("Queue: BBU:%lu Dec:%d Disp:%d ESR:%lu ESRfromFW:%lu ESW:%lu ESLevel:%lu\n" ,
        _HAL_HVD_GetBBUQNumb() , pHVDShareMem->u16DecQNumb , pHVDShareMem->u16DispQNumb ,
        _HAL_HVD_GetESReadPtr(TRUE), pHVDShareMem->u32ESReadPtr, _HAL_HVD_GetESWritePtr() , _HAL_HVD_GetESLevel() );

    HVD_MSG_DEG("Counter: Dec:%lu Disp:%lu Err_Data:%lu _Dec:%lu Skip:%lu Drop:%lu Idle:%lu Main:%lu Vsync:%lu\n" ,
        pHVDShareMem->u32DecodeCnt , pHVDShareMem->u32DispCnt , pHVDShareMem->u32DataErrCnt ,
        pHVDShareMem->u32DecErrCnt , pHVDShareMem->u32SkipCnt , pHVDShareMem->u32DropCnt ,
        pHVDShareMem->u32VPUIdleCnt ,  pHVDShareMem->u32MainLoopCnt,  pHVDShareMem->u32VsyncCnt);
    HVD_MSG_DEG("Mode: ShowErr:%d RepLastField:%d SyncOn:%d FileEnd:%d Skip:%d Drop:%d DispSpeed:%d FRC:%d BlueScreen:%d FreezeImg:%d 1Field:%d\n" ,
        pHVDShareMem->ModeStatus.bIsShowErrFrm , pHVDShareMem->ModeStatus.bIsRepeatLastField ,
        pHVDShareMem->ModeStatus.bIsSyncOn , pHVDShareMem->ModeStatus.bIsPlaybackFinish ,
        pHVDShareMem->ModeStatus.u8SkipMode , pHVDShareMem->ModeStatus.u8DropMode ,
        pHVDShareMem->ModeStatus.s8DisplaySpeed , pHVDShareMem->ModeStatus.u8FrcMode ,
        pHVDShareMem->ModeStatus.bIsBlueScreen , pHVDShareMem->ModeStatus.bIsFreezeImg ,
        pHVDShareMem->ModeStatus.bShowOneField);
    HVD_MSG_DEG("====================================\n"  );
}

void HAL_HVD_GetBBUEntry( MS_U32 u32Idx , MS_U32* u32NalOffset , MS_U32* u32NalSize  )
{
    MS_U8* addr=NULL;
    if( u32Idx >= u32BBUEntryNum )
    {
        return;
    }
    addr = (MS_U8*)((pHVDCtrl_Hal->MemMap.u32CodeBufVAddr)+ HVD_BBU_DRAM_ST_ADDR + (u32Idx<<3));
    *u32NalSize = *(addr +2) & 0x1f;
    *u32NalSize <<=8;
    *u32NalSize |= *(addr +1) & 0xff;
    *u32NalSize <<=8;
    *u32NalSize |= *(addr) & 0xff;

    *u32NalOffset = ((MS_U32)(*(addr+2) & 0xe0)) >> 5;
    *u32NalOffset |= ((MS_U32)(*(addr+3) & 0xff)) << 3;
    *u32NalOffset |= ((MS_U32)(*(addr+4) & 0xff)) << 11;
    *u32NalOffset |= ((MS_U32)(*(addr+5) & 0xff)) << 19;
}

void HAL_HVD_Dump_BBUs(  MS_U32 u32StartIdx, MS_U32 u32EndIdx, MS_BOOL bShowEmptyEntry )
{
    MS_U32 u32CurIdx=0;
    MS_BOOL bFinished=FALSE;
    MS_U32 u32NalOffset=0 ;
    MS_U32 u32NalSize=0;
    if( (u32StartIdx >= u32BBUEntryNum) || (u32EndIdx >= u32BBUEntryNum) )
    {
        return;
    }
    u32CurIdx = u32StartIdx;
    do
    {
        if( u32CurIdx  == u32EndIdx )
        {
            bFinished =TRUE;
        }
        HAL_HVD_GetBBUEntry( u32CurIdx ,  &u32NalOffset , &u32NalSize   );
        if( (bShowEmptyEntry ==FALSE) ||
             ( bShowEmptyEntry &&  (u32NalOffset ==0) &&  (u32NalSize ==0) ))
        {
            HVD_MSG_DEG(  "HVD BBU Entry: Idx:%lu Offset:%lx Size:%lx\n",  u32CurIdx  , u32NalOffset   , u32NalSize );
        }
        u32CurIdx++;
        if( u32CurIdx >= u32BBUEntryNum )
        {
            u32CurIdx%=u32BBUEntryNum;
        }
    }while(bFinished == TRUE);
}

// This function is used by MJPEG driver
MS_BOOL HAL_HVD_LoadCode(MS_U32 u32DestAddr, MS_U32 u32Size, MS_U32 u32BinAddr, MS_U8 u8FwSrcType)
{
#if HVD_ENABLE_BDMA_FW_FLASH_2_SDRAM
    MS_U32 u32MIU1Base=0;
#ifdef HAL_MIU1_BASE
    u32MIU1Base = HAL_MIU1_BASE;
#else
    u32MIU1Base=0x08000000;
#endif
#endif

    if (u8FwSrcType == E_HVD_FW_INPUT_SOURCE_FLASH)
    {
        #if HVD_ENABLE_BDMA_FW_FLASH_2_SDRAM
        if(u32Size != 0)
        {
            SPIDMA_Dev cpyflag = E_SPIDMA_DEV_MIU1;
            if (u32DestAddr >= u32MIU1Base)
            {
                cpyflag = E_SPIDMA_DEV_MIU1;
            }
            else
            {
                cpyflag = E_SPIDMA_DEV_MIU0;
            }

            if( ! HVD_FLASHcpy(u32DestAddr, u32BinAddr, u32Size, cpyflag))
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
        #else
        return FALSE;
        #endif
    }
    else
    if (u8FwSrcType == E_HVD_FW_INPUT_SOURCE_DRAM)
    {
        if(u32BinAddr != 0 && u32Size != 0)
        {
            HVD_memcpy((void*)(u32DestAddr), (void*)(u32BinAddr), u32Size);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        #if HVD_ENABLE_EMBEDDED_FW_BINARY
        HVD_memcpy( (void*)(u32DestAddr), (void*)(u8HVD_FW_Binary) , sizeof(u8HVD_FW_Binary));
        #else
        return FALSE;
        #endif
    }
    //HAL_HVD_FlushMemory();
    MAsm_CPU_Sync();
    MsOS_FlushMemory();

    //Init shareMem
    pHVDShareMem = (volatile HVD_ShareMem*)(u32DestAddr + HVD_SHARE_MEM_ST_OFFSET);
    HVD_memset( (volatile void*)pHVDShareMem, 0, sizeof( HVD_ShareMem ) );
    //Set Chip Id
    pHVDShareMem->u16ChipID = E_MSTAR_CHIP_A3;
    pHVDShareMem->u16ChipECONum = 0;
    HAL_HVD_FlushMemory();

    return TRUE;
}

void HAL_HVD_SetMiuBurstLevel(HVD_MIU_Burst_Cnt_Ctrl eMiuBurstCntCtrl)
{
    if (pHVDCtrl_Hal)
    {
        pHVDCtrl_Hal->Settings.u32MiuBurstLevel = (MS_U32) eMiuBurstCntCtrl;
    }
}

#ifdef _HVD_DQ
void* HAL_HVD_GetShmMemAddr(void)
{
    return (void*)pHVDShareMem;
}
#endif

#define HVD_HW_MAX_PIXEL (1920*1088*51000ULL) //FullHD@50p
MS_BOOL HAL_HVD_GetFrmRateIsSupported(MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate)
{
    HVD_MSG_DEG("%s w:%d, h:%d, fr:%ld, MAX:%lld\n", __FUNCTION__, u16HSize, u16VSize, u32FrmRate, HVD_HW_MAX_PIXEL);
    return (((MS_U64)u16HSize*(MS_U64)u16VSize*(MS_U64)u32FrmRate) <= HVD_HW_MAX_PIXEL);
}

