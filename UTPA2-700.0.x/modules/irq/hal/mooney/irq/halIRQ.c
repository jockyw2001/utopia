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

#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX) 

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsOS.h"
#include "halIRQTBL.h"
#include "regCHIP.h"
#include "drvIRQ.h"
#include "halIRQ.h"
#include "regIRQ.h"

#if defined(MSOS_TYPE_NUTTX) 
#include "debug.h"
#endif

#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

#if defined (__mips__)
#define mtspr(spr, value)  printf("[NIY] mtspr in line:%s %d\n",__FILE__, __LINE__);
#define mfspr(spr)         printf("[NIY] mfspr in line:%s %d\n",__FILE__, __LINE__);

inline MS_U32 __mhal_lsbit_index(MS_U32 _value_)
{
    MS_U32  index = 1;

    while((_value_&0x01) == 0x00)
    {
        _value_ = (_value_ >> 1);
        index++;
        if(index == 32)
        {
            index = 0;
            break;
        }
    }

    return index;
    //printf(const char * fmt, ...)("[NIY] __mhal_lsbit_index in line: %s %d\n",__FILE__, __LINE__);
}


#define __mhal_interrupt_disable(_old_) (_old_=_old_)

#define __mhal_interrupt_restore(_old_) (_old_=_old_)

#elif defined (__arm__) || defined(__aarch64__)
#define mtspr(spr, value)  printf("[NIY] mtspr in line:%s %d\n",__FILE__, __LINE__);
#define mfspr(spr)         printf("[NIY] mfspr in line:%s %d\n",__FILE__, __LINE__);

inline MS_U32 __mhal_lsbit_index(MS_U32 _value_)
{
    MS_U32  index = 1;

    while((_value_&0x01) == 0x00)
    {
        _value_ = (_value_ >> 1);
        index++;
        if(index == 32)
        {
            index = 0;
            break;
        }
    }

    return index;
    //printf(const char * fmt, ...)("[NIY] __mhal_lsbit_index in line: %s %d\n",__FILE__, __LINE__);
}


#define __mhal_interrupt_disable(_old_) (_old_=_old_)

#define __mhal_interrupt_restore(_old_) (_old_=_old_)
#else
#define mtspr(spr, value) \
    __asm__ __volatile__ ("l.mtspr\t\t%0,%1,0" : : "r" (spr), "r" (value))

#define mfspr(spr) \
    ({ \
        unsigned long value; \
        __asm__ __volatile__ ("l.mfspr\t\t%0,%1,0" : "=r" (value) : "r" (spr) : "memory"); \
        value; \
    })

#define __mhal_lsbit_index(_value_)                                         \
    ({                                                                      \
    unsigned long _index_;                                                  \
    __asm__ __volatile__ ("l.ff1\t\t%0,%1" : "=r" (_index_) : "r" (_value_));\
    _index_;                                                                \
    })


#define GRP_BITS                (11)
#define SPR_SR                  ((0 << GRP_BITS) + 17)
#define SPR_SR_TEE              0x00000002  // Tick timer Exception Enable
#define SPR_SR_IEE              0x00000004  // Interrupt Exception Enable

#define __mhal_interrupt_disable(_old_)                                     \
    MST_MACRO_START                                                         \
    _old_ = mfspr(SPR_SR);                                                  \
    mtspr(SPR_SR, (_old_) & ~(SPR_SR_IEE | SPR_SR_TEE));                    \
    MST_MACRO_END

#define __mhal_interrupt_restore(_old_)                                     \
    mtspr(SPR_SR, (~(SPR_SR_IEE|SPR_SR_TEE) & mfspr(SPR_SR) ) |             \
                  ( (SPR_SR_IEE|SPR_SR_TEE) & (_old_) ))
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define COUNTOF( array )    (sizeof(array) / sizeof((array)[0]))
//#define E_INTERRUPT_FIQ     E_INTERRUPT_02
//#define E_INTERRUPT_IRQ     E_INTERRUPT_03

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef void (*IRQCb)(MS_U32 u32Vector);

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static IRQCb irq_table[E_IRQ_FIQ_ALL] = {0};
// static MS_U32 _u32FIQ, _u32IRQ, _u32FIQExp, _u32IRQExp, _u32MIO_MapBase = 0;
static MS_U32 _u32FIQ_Msk, _u32IRQ_Msk, _u32FIQExp_Msk, _u32IRQExp_Msk;

#if defined(MCU_AEON)
static MS_U32  _u32MIO_MapBase = 0xfa200000;
#elif defined(__arm__) || defined(__aarch64__)
#ifdef CONFIG_MBOOT
static MS_U32 _u32MIO_MapBase = 0x1f200000;
#else
static MS_U32 _u32MIO_MapBase = 0xfd200000;
#endif
#else
static MS_U32  _u32MIO_MapBase= 0xbf200000;
#endif

static MS_BOOL _bInIRQ = FALSE;
static MS_BOOL _bInFIQ = FALSE;

//-------------------------------------------------------------------------------------------------
//  External Functions
//-------------------------------------------------------------------------------------------------

#define REG16_R(u32RegAddr) ((*((volatile MS_U16*)(_u32MIO_MapBase+ ((u32RegAddr)<< 1)))) & 0xFFFF)
#define REG16_W(u32RegAddr, u32Value) (*((volatile MS_U32*)(_u32MIO_MapBase+ ((u32RegAddr)<< 1))))= ((u32Value) & 0xFFFF)

/*
static MS_U16 REG16_R(MS_U32 u32RegAddr_in)
{
    MS_U32 u32RegAddr1 = (u32RegAddr_in << 1);
    MS_U32 u32RegAddr = (_u32MIO_MapBase+ (u32RegAddr1));
    MS_U16 u16RegValue = (*((volatile MS_U16*)(u32RegAddr)) & 0xFFFF);

    printf("[%s][%d] 0x%08x, 0x%08x\n", __FUNCTION__, __LINE__, u32RegAddr, u16RegValue);
    return u16RegValue;
}

static MS_U16 REG16_W(MS_U32 u32RegAddr_in, MS_U32 u32Value)
{
    MS_U32 u32RegAddr1 = (u32RegAddr_in << 1);
    MS_U32 u32RegAddr = (_u32MIO_MapBase+ (u32RegAddr1));
    *((volatile MS_U16*)(u32RegAddr)) = ((u32Value) & 0xFFFF);

    // printf("[%s][%d] 0x%08x, 0x%08x\n", __FUNCTION__, __LINE__, u32RegAddr, u32Value);
    // REG16_R(u32RegAddr_in);
}
*/

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _HAL_IRQ_Enable(MS_U32 u32Vector, int enable);

static MS_U16 _IRQ_Read2Byte(MS_U32 u32RegAddr)
{
    return REG16_R(u32RegAddr);
}

static MS_U32 _IRQ_Read4Byte(MS_U32 u32RegAddr)
{
    return (_IRQ_Read2Byte(u32RegAddr) | _IRQ_Read2Byte(u32RegAddr+2) << 16);
}

#if 0
static void _IRQ_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (u32RegAddr & 1)
    {
        REG16_W(u32RegAddr, (REG16_R(u32RegAddr) & ~(0xFF00))| (u8Val<< 8));
    }
    else
    {
        REG16_W(u32RegAddr, (REG16_R(u32RegAddr) & ~(0x00FF))| (u8Val));
    }
}
#endif

static void _IRQ_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    REG16_W(u32RegAddr, u16Val);
}

static void _IRQ_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    _IRQ_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    _IRQ_Write2Byte(u32RegAddr+2, u32Val >> 16);
}

#if defined(__arm__) || defined(__aarch64__)
static void _HAL_IRQ_FIQHnd(void)
#else
static void _HAL_IRQ_FIQHnd(MHAL_SavedRegisters *pHalReg, MS_U32 u32Vector)
#endif
{
    MS_U32 status;
    MS_U32 index;

    _bInFIQ = TRUE;
    status = _IRQ_Read4Byte(REG_FIQ_FINAL_STATUS);

    index = __mhal_lsbit_index(status);
    if (index)
    {

        _IRQ_Write4Byte(REG_C_FIQ_CLR + 0, status);
        _IRQ_Write4Byte(REG_C_FIQ_CLR + 0, 0);

        do
        {
            status &= ~(1 << --index);
            index += (MS_U32)E_FIQL_START;
            if (irq_table[index])
            {
                _HAL_IRQ_Enable(index, DISABLE);
                irq_table[index](HWIdx2IntEnum[index]);
            }
            index = __mhal_lsbit_index(status);
        } while (index);
    }

    status = _IRQ_Read4Byte(REG_C_FIQ_EXP_FINAL_STATUS);

    index = __mhal_lsbit_index(status);
    if (index)
    {
        _IRQ_Write4Byte(REG_C_FIQ_EXP_CLR, status);
        _IRQ_Write4Byte(REG_C_FIQ_EXP_CLR, 0);
        do {
            status &= ~(1 << --index);
            index += (MS_U32)E_FIQEXPL_START;
            if (irq_table[index])
            {
                _HAL_IRQ_Enable(index, DISABLE);
                irq_table[index](HWIdx2IntEnum[index]);
            }
            index = __mhal_lsbit_index(status);
        } while (index);
    }
    _bInFIQ = FALSE;
}

#if defined(__arm__) || defined(__aarch64__)
static void _HAL_IRQ_IRQHnd(void)
#else
static void _HAL_IRQ_IRQHnd(MHAL_SavedRegisters *pHalReg, MS_U32 u32Vector)
#endif
{
    MS_U32 status;
    MS_U32 index;

    _bInIRQ = TRUE;
    status = _IRQ_Read4Byte(REG_IRQ_FINAL_STATUS);
    index = __mhal_lsbit_index(status);
    if (index)
    {
        do {
            status &= ~(1 << --index);
            index += (MS_U32)E_IRQL_START;
            if (irq_table[index])
            {
                _HAL_IRQ_Enable(index, DISABLE);
                //fix Uart Rx interrupt can't work
                irq_table[index](HWIdx2IntEnum[index]);
            }
            index = __mhal_lsbit_index(status);
        } while (index);
    }

    status = _IRQ_Read4Byte(REG_C_IRQ_EXP_FINAL_STATUS);
    index = __mhal_lsbit_index(status);
    if (index)
    {
        do {
            status &= ~(1 << --index);
            index += (MS_U32)E_IRQEXPL_START;
            if (irq_table[index])
            {
                _HAL_IRQ_Enable(index, DISABLE);
                irq_table[index](HWIdx2IntEnum[index]);
            }
            index = __mhal_lsbit_index(status);
        } while (index);
    }
    _bInIRQ = FALSE;
}

static void _HAL_IRQ_Enable(MS_U32 u32Vector, int enable)
{
    MS_U32 reg = E_IRQ_FIQ_INVALID;
    MS_U32 mask;
    MS_U32 old = 0;

    if ((MS_U32)u32Vector <= COUNTOF(irq_table))
    {
        if ( (u32Vector >= E_IRQL_START) && (u32Vector <= E_IRQH_END) )
        {
            u32Vector -= E_IRQL_START;
            reg = REG_C_IRQ_MASK;
        }
        else if ( (u32Vector >= E_FIQL_START) && (u32Vector <= E_FIQH_END) )
        {
            u32Vector -= E_FIQL_START;
            reg = REG_C_FIQ_MASK;
        }
        else if ( (u32Vector >= E_IRQEXPL_START) && (u32Vector <= E_IRQEXPH_END) )
        {
            u32Vector -= E_IRQEXPL_START;
            reg = REG_C_IRQ_EXP_MASK;
        }
        else if ( (u32Vector >= E_FIQEXPL_START) && (u32Vector <= E_FIQEXPH_END) )
        {
            u32Vector -= E_FIQEXPL_START;
            reg = REG_C_FIQ_EXP_MASK;
        }

        if( E_IRQ_FIQ_INVALID == reg )
        {
            //printf("_HAL_IRQ_Enable: unknow vector\n");
            return;
        }

        __mhal_interrupt_disable(old);
        mask = _IRQ_Read4Byte(reg);
        u32Vector = (1 << u32Vector);

        if (enable)
            mask &= ~u32Vector;
        else
            mask |= u32Vector;

        _IRQ_Write4Byte(reg, mask);
        __mhal_interrupt_restore(old);

    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void HAL_IRQ_Set_IOMap(MS_U32 u32Base)
{
     _u32MIO_MapBase = u32Base;
}

#if defined(MSOS_TYPE_NUTTX) 
int _HAL_IRQ_FIQHnd_Nuttx(int irq, void *context)
{
    _HAL_IRQ_FIQHnd((MHAL_SavedRegisters *)context, irq);
    return 1;
}

int _HAL_IRQ_IRQHnd_Nuttx(int irq, void *context)
{
    _HAL_IRQ_IRQHnd((MHAL_SavedRegisters *)context, irq);
    return 1;
}
#endif

void HAL_IRQ_Init(void)
{
    HAL_InitIrqTable();
#if defined(__arm__) || defined(__aarch64__)
    MsOS_CPU_AttachInterrupt(E_INTERRUPT_FIQ, _HAL_IRQ_FIQHnd, E_INTERRUPT_FIQ);
    MsOS_CPU_AttachInterrupt(E_INTERRUPT_IRQ, _HAL_IRQ_IRQHnd, E_INTERRUPT_IRQ);
#else
#if defined(MSOS_TYPE_NUTTX) 
    MsOS_CPU_AttachInterrupt(E_INTERRUPT_FIQ, _HAL_IRQ_FIQHnd_Nuttx, E_INTERRUPT_FIQ);
    MsOS_CPU_AttachInterrupt(E_INTERRUPT_IRQ, _HAL_IRQ_IRQHnd_Nuttx, E_INTERRUPT_IRQ);
    MsOS_CPU_UnMaskInterrupt(E_INTERRUPT_FIQ);
    MsOS_CPU_UnMaskInterrupt(E_INTERRUPT_IRQ);
#else
    MsOS_CPU_AttachInterrupt(E_INTERRUPT_FIQ, (mhal_isr_t) _HAL_IRQ_FIQHnd, E_INTERRUPT_FIQ);
    MsOS_CPU_AttachInterrupt(E_INTERRUPT_IRQ, (mhal_isr_t) _HAL_IRQ_IRQHnd, E_INTERRUPT_IRQ);
#endif
#endif
    HAL_IRQ_DetechAll();
}

void HAL_IRQ_Attach(MS_U32 u32Vector, void *pIntCb)
{
    MS_U32 u32VectorIndex = 0;

    u32VectorIndex = (MS_U32)IntEnum2HWIdx[u32Vector];

    if ((MS_U32)u32VectorIndex <= COUNTOF(irq_table))
        irq_table[u32VectorIndex] = (IRQCb)pIntCb;
    else
        printf("%s error vector: %x\n", __FUNCTION__, (unsigned int)u32VectorIndex);
}

void HAL_IRQ_DetechAll()
{
    MS_U16 u16Cnt= 0;
    for (; u16Cnt <= COUNTOF(irq_table); u16Cnt++)
        irq_table[u16Cnt] = 0;
}

void HAL_IRQ_Detech(MS_U32 u32Vector)
{
    MS_U32 u32VectorIndex = 0;

    u32VectorIndex = (MS_U32)IntEnum2HWIdx[u32Vector];

    if ((MS_U32)u32VectorIndex <= COUNTOF(irq_table))
        irq_table[u32VectorIndex] = 0;
    else
        printf("%s error vector: %x\n", __FUNCTION__, (unsigned int)u32Vector);
}

void HAL_IRQ_MaskAll(MS_BOOL bMask)
{
    if (bMask)
    {
        _u32FIQ_Msk = _IRQ_Read4Byte(REG_C_FIQ_MASK);
        _u32IRQ_Msk = _IRQ_Read4Byte(REG_C_IRQ_MASK);
        _u32FIQExp_Msk = _IRQ_Read4Byte(REG_C_FIQ_EXP_MASK);
        _u32IRQExp_Msk = _IRQ_Read4Byte(REG_C_IRQ_EXP_MASK);
        _IRQ_Write4Byte(REG_C_FIQ_MASK, 0xFFFFFFFF);
        _IRQ_Write4Byte(REG_C_IRQ_MASK, 0xFFFFFFFF);
        _IRQ_Write4Byte(REG_C_FIQ_EXP_MASK, 0xFFFFFFFF);
        _IRQ_Write4Byte(REG_C_IRQ_EXP_MASK, 0xFFFFFFFF);
    }
    else
    {
        _IRQ_Write4Byte(REG_C_FIQ_MASK, 0);
        _IRQ_Write4Byte(REG_C_IRQ_MASK, 0);
        _IRQ_Write4Byte(REG_C_FIQ_EXP_MASK, 0);
        _IRQ_Write4Byte(REG_C_IRQ_EXP_MASK, 0);
    }
}

void HAL_IRQ_Restore()
{
    _IRQ_Write4Byte(REG_C_FIQ_MASK, _u32FIQ_Msk);
    _IRQ_Write4Byte(REG_C_IRQ_MASK, _u32IRQ_Msk);
    _IRQ_Write4Byte(REG_C_FIQ_EXP_MASK, _u32FIQExp_Msk);
    _IRQ_Write4Byte(REG_C_IRQ_EXP_MASK, _u32IRQExp_Msk);
}

void HAL_IRQ_Mask(MS_U32 u32Vector)
{
    MS_U32 u32VectorIndex = 0;

    u32VectorIndex = (MS_U32)IntEnum2HWIdx[u32Vector];
    _HAL_IRQ_Enable(u32VectorIndex, DISABLE);
}

void HAL_IRQ_UnMask(MS_U32 u32Vector)
{
    MS_U32 u32VectorIndex = 0;

    u32VectorIndex = (MS_U32)IntEnum2HWIdx[u32Vector];
    _HAL_IRQ_Enable(u32VectorIndex, ENABLE);
}

void HAL_IRQ_NotifyCpu(IRQ_CPU_TYPE type)
{
    type = type;
    printf("[%s][%d] has not implemented yet\n", __FUNCTION__, __LINE__);
#if 0
    switch (type)
    {
        case E_IRQ_CPU0_2_CPU1:
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU0, BIT(0));
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU0, 0);
            break;
        case E_IRQ_CPU0_2_CPU2:
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU0, BIT(1));
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU0, 0);
            break;
        case E_IRQ_CPU1_2_CPU0:
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU1, BIT(0));
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU1, 0);
            break;
        case E_IRQ_CPU1_2_CPU2:
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU1, BIT(1));
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU1, 0);
            break;
        case E_IRQ_CPU2_2_CPU0:
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU2, BIT(0));
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU2, 0);
            break;
        case E_IRQ_CPU2_2_CPU1:
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU2, BIT(1));
            _IRQ_WriteByte(REG_SEND_IRQ_FROM_CPU2, 0);
            break;
        default:
            break;
    }
#endif
}

MS_BOOL HAL_IRQ_InISR()
{
    return (_bInIRQ || _bInFIQ);
}

#endif // #if defined (MSOS_TYPE_NOS)

#if defined(MSOS_TYPE_LINUX_KERNEL)
#include "linux/interrupt.h"
#include "MsCommon.h"
#include "halIRQTBL.h"

typedef struct
{
    MS_BOOL         bUsed;
    //MS_BOOL         bRunning;
    InterruptCb     pInterruptCb;
} CHIP_ISR_Info;

static CHIP_ISR_Info        _MsOS_ISR_Info[MS_IRQ_MAX];

static irqreturn_t _HAL_IRQHandler_Wrapper(int irq, void *desc);

//
//  Interrupt Service Routine
//
static                          DEFINE_SPINLOCK(_HAL_IRQ_Mutex);
#define IRQ_MUTEX_LOCK()        spin_lock(&_HAL_IRQ_Mutex)
#define IRQ_MUTEX_UNLOCK()      spin_unlock(&_HAL_IRQ_Mutex)
static MS_BOOL g_bISRInit = FALSE;

//Bottom half interrupt handler define
#define BH_SOFTIRQ      1
#define BH_TASKLET      2
#define BH_WORKQUEUE    3
#define BH_THREADED     4

#define BOTTOM_HALF     BH_THREADED

void CHIP_InitISR(void)
{
    IRQ_MUTEX_LOCK();
    if (g_bISRInit)
    {
        IRQ_MUTEX_UNLOCK();
        return;
    }
    g_bISRInit = TRUE;
    HAL_InitIrqTable();
    //_HAL_IRQ_Tlb_Init();
    memset(_MsOS_ISR_Info, 0, MS_IRQ_MAX*sizeof(CHIP_ISR_Info));
    IRQ_MUTEX_UNLOCK();
}

MS_BOOL CHIP_EnableIRQ(InterruptNum eIntNum)
{
    MS_U16 u16VectorIndex = 0;
    int irq;

    IRQ_MUTEX_LOCK();

    u16VectorIndex = (MS_U16)IntEnum2HWIdx[eIntNum];
    irq = (int)u16VectorIndex + MSTAR_INT_BASE;

    if (!g_bISRInit || !_MsOS_ISR_Info[u16VectorIndex].bUsed)
    {
        IRQ_MUTEX_UNLOCK();
        return FALSE;   //IRQ number was not register(Attach).
    }
    IRQ_MUTEX_UNLOCK();

    /* do not enable irq if already enableld */
    if ((irq_to_desc(irq)->irq_data.state_use_accessors & IRQD_IRQ_DISABLED))
        enable_irq(irq);

    return TRUE;
}

MS_BOOL CHIP_DisableIRQ(InterruptNum eIntNum)
{
    MS_U16 u16VectorIndex = 0;
    int irq;

    IRQ_MUTEX_LOCK();

    u16VectorIndex = (MS_U16)IntEnum2HWIdx[eIntNum];
    irq = (int)u16VectorIndex + MSTAR_INT_BASE;

    if (!g_bISRInit || !_MsOS_ISR_Info[u16VectorIndex].bUsed)
    {
        IRQ_MUTEX_UNLOCK();
        return FALSE;   //IRQ number was not register(Attach).
    }
    IRQ_MUTEX_UNLOCK();

    disable_irq_nosync(irq);
    return TRUE;
}

typedef struct
{
    int     id;
    bool        id_auto;
}platform_utopia_device;


static platform_utopia_device _devUTOPIA =
{
    .id = 6,
    .id_auto = 0,
};
static irqreturn_t _irq_top(int eIntNum, void* dev_id)
{
    return IRQ_WAKE_THREAD;
}

void HAL_IRQ_Attach(MS_U32 u32Vector, void *pIntCb, MS_U32 u32IntType )
{
    MS_U16 u16VectorIndex = 0;
    int irq_flags = SA_INTERRUPT;
    int err = 0, irq;

    IRQ_MUTEX_LOCK();

    u16VectorIndex = (MS_U16)IntEnum2HWIdx[u32Vector];
    irq = (int)u16VectorIndex + MSTAR_INT_BASE;

    if (!g_bISRInit || _MsOS_ISR_Info[u16VectorIndex].bUsed)
    {
        if (!g_bISRInit)
            printk("[Utopia2K] IRQ structure did not be initialized\n");
        else
            printk("[Utopia2K] IRQ %d has been registered.\n", (unsigned int)u32Vector);
        IRQ_MUTEX_UNLOCK();
        return;
    }

    _MsOS_ISR_Info[u16VectorIndex].bUsed = TRUE;
    _MsOS_ISR_Info[u16VectorIndex].pInterruptCb = (InterruptCb)pIntCb;

    IRQ_MUTEX_UNLOCK();

#if   (BOTTOM_HALF == BH_SOFTIRQ)
#elif (BOTTOM_HALF == BH_TASKLET)
#elif (BOTTOM_HALF == BH_WORKQUEUE)
#elif (BOTTOM_HALF == BH_THREADED)

    if(u32IntType !=  IRQF_SHARED )
    {
        irq_flags |= IRQF_ONESHOT;

        err = request_threaded_irq( irq,
                                NULL,
                                _HAL_IRQHandler_Wrapper,
                                irq_flags,
                                HWIdx2IRQname[u16VectorIndex],
                                NULL);
    }else
    {
        irq_flags = IRQF_SHARED | IRQF_ONESHOT;

        err = request_threaded_irq( irq,
                                _irq_top,
                                _HAL_IRQHandler_Wrapper,
                                irq_flags,
                                HWIdx2IRQname[u16VectorIndex],
                                &_devUTOPIA);

    }
    if (0 != err)
    {
        printk("[Utopia2K] request_threaded_irq Fail\n");
        return;
    }

    if(!(irq_to_desc(irq)->irq_data.state_use_accessors & IRQD_IRQ_DISABLED))
        disable_irq_nosync(irq);
#endif

    return;
}

void HAL_IRQ_Detech(MS_U32 u32Vector)
{
    MS_U16 u16VectorIndex = 0;
    int irq;

    IRQ_MUTEX_LOCK();
    u16VectorIndex = (MS_U16)IntEnum2HWIdx[u32Vector];
    irq = (int)u16VectorIndex + MSTAR_INT_BASE;

    if (!g_bISRInit || !_MsOS_ISR_Info[u16VectorIndex].bUsed)
    {
        IRQ_MUTEX_UNLOCK();
        return;
    }
    IRQ_MUTEX_UNLOCK();

    free_irq(irq, NULL);

    IRQ_MUTEX_LOCK();
    _MsOS_ISR_Info[u16VectorIndex].bUsed = FALSE;
    _MsOS_ISR_Info[u16VectorIndex].pInterruptCb = NULL;
    IRQ_MUTEX_UNLOCK();

    return;
}

static irqreturn_t _HAL_IRQHandler_Wrapper(int irq, void *desc)
{
    InterruptCb pfnIntCb;

	disable_irq_nosync(irq);

    IRQ_MUTEX_LOCK();
    if (!_MsOS_ISR_Info[(irq-MSTAR_INT_BASE)].bUsed || !_MsOS_ISR_Info[(irq-MSTAR_INT_BASE)].pInterruptCb)
    {
        IRQ_MUTEX_UNLOCK();
        return IRQ_HANDLED;
    }
    pfnIntCb = _MsOS_ISR_Info[(irq-MSTAR_INT_BASE)].pInterruptCb;
    IRQ_MUTEX_UNLOCK();

    pfnIntCb(irq);

    return IRQ_HANDLED;
}

#endif
