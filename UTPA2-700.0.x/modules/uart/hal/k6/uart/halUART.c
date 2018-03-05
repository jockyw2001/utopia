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
#include "MsCommon.h"
#include "drvUART.h"

// Internal Definition
#include "regUART.h"
#include "halUART.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define UART_HAL_ERR(x, args...)        //{printf(x, ##args);}

#define _BITMASK(loc_msb, loc_lsb) ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))
#define BITMASK(x) _BITMASK(1?x, 0?x)
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

struct uart_dev_struct _uart_dev[UART_DEV_MAX];

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _gMIO_MapBase = 0;
static MS_VIRT _gMIO_PM_MapBase = 0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_U8 _HAL_REG_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

MS_BOOL _HAL_REG_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        UART_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

MS_BOOL _HAL_REG_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask)
{
    if (!u32RegAddr)
    {
        UART_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = (_HAL_REG_ReadByte(u32RegAddr) & ~(u8Mask)) | (u8Val & u8Mask);
    _HAL_REG_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

MS_U8 _HAL_REG_PM_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_PM_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

MS_BOOL _HAL_REG_PM_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        UART_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_PM_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

MS_BOOL _HAL_REG_PM_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask)
{
    if (!u32RegAddr)
    {
        UART_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = (_HAL_REG_PM_ReadByte(u32RegAddr) & ~(u8Mask)) | (u8Val & u8Mask);
    _HAL_REG_PM_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void HAL_UART_SetIOMapBase(MS_VIRT u32Base, MS_VIRT u32PMBase)
{
    _gMIO_MapBase = u32Base;
    _gMIO_PM_MapBase = u32PMBase;
    UART_HAL_ERR("UART IOMap base:%8lx\n", u32Base);
    UART_HAL_ERR("PM   IOMap base:%8lx\n", u32PMBase);
}

MS_VIRT HAL_UART_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}


MS_BOOL HAL_UART_SetPMRxEnable(MS_BOOL bEnable)
{
    return _HAL_REG_PM_WriteByteMask(REG_PM_UART_CTRL, (bEnable? _uart_rx_enable:0), _uart_rx_enable);
}

UART_DEVICE_TYPE HAL_UART_Get_Connection(UART_PORT_TYPE uart_port)
{
    UART_DEVICE_TYPE src;
    int dev;

    switch (uart_port)
    {
        case E_UART_PORT0:
            dev = (_HAL_REG_ReadByte(0x1EA6) & BITMASK(3:0));
            break;
        case E_UART_PORT1:
            dev = (_HAL_REG_ReadByte(0x1EA6) & BITMASK(7:4)) >> 4;
            break;
        case E_UART_PORT2:
            dev = (_HAL_REG_ReadByte(0x1EA7) & BITMASK(3:0));
            break;
        case E_UART_PORT3:
            dev = (_HAL_REG_ReadByte(0x1EA7) & BITMASK(7:4)) >> 4;
            break;
        case E_UART_PORT4:
            dev = (_HAL_REG_ReadByte(0x1EA8) & BITMASK(3:0));
            break;
        default:
            MS_ASSERT(0);
            return E_UART_INVALID;
    }
    switch (dev)
    {
        case UART_SEL_HK_R2:
            src = E_UART_AEON_R2;
            break;

        case UART_SEL_VD_MHEG5:
            src = E_UART_VDEC;
            break;

        case UART_SEL_TSP:
            src = E_UART_TSP;
            break;

        case UART_SEL_PIU_UART0:
            src = E_UART_PIU_UART0;
            break;

        case UART_SEL_PIU_UART1:
            src = E_UART_PIU_UART1;
            break;

        case UART_SEL_PIU_UART2:
            src = E_UART_PIU_UART2;
            break;

        case UART_SEL_PIU_FAST_UART:
            src = E_UART_PIU_FUART0;
            break;

        case UART_SEL_VD_MCU51_TXD0:
            src = E_UART_VD51_UART0;
            break;

        case UART_SEL_VD_MCU51_TXD1:
            src = E_UART_VD51_UART1;
            break;
        case UART_SEL_DMD_MCU51_TXD0:
            src = E_UART_DMD51_UART0;
            break;

        case UART_SEL_DMD_MCU51_TXD1:
            src = E_UART_DMD51_UART1;
            break;

        case UART_SEL_AUDIO_R2:
            src = E_UART_AEON_AUDIO_R2;
            break;

        case UART_SEL_SECURE_R2:
            src = E_UART_SECURE_R2;
            break;

        case UART_SEL_OFF:
            src = E_UART_OFF;
            break;

        default:
            MS_ASSERT(0);
            return E_UART_INVALID;
    }
    return src;

}

MS_U16 HAL_UART_Connect(UART_PORT_TYPE uart_port, UART_DEVICE_TYPE uart_dev)
{
    MS_U16 src;

    switch (uart_dev)
    {
        case E_UART_OFF:
            src = UART_SEL_OFF;
            break;

        case E_UART_AEON_R2:
            src = UART_SEL_HK_R2;
            break;

        case E_UART_VDEC:
            src = UART_SEL_VD_MHEG5;
            break;

        case E_UART_TSP:
            src = UART_SEL_TSP;
            break;

        case E_UART_PIU_UART0:
            src = UART_SEL_PIU_UART0;
            break;

        case E_UART_PIU_UART1:
            src = UART_SEL_PIU_UART1;
            break;

        case E_UART_PIU_UART2:
            src = UART_SEL_PIU_UART2;
            break;

        case E_UART_PIU_FUART0:
            src = UART_SEL_PIU_FAST_UART;
            break;

        case E_UART_VD51_UART0:
            src = UART_SEL_VD_MCU51_TXD0;
            break;

        case E_UART_VD51_UART1:
            src = UART_SEL_VD_MCU51_TXD1;
            break;
        case E_UART_DMD51_UART0:
            src = UART_SEL_DMD_MCU51_TXD0;
            break;

        case E_UART_DMD51_UART1:
            src = UART_SEL_DMD_MCU51_TXD1;
            break;

        case E_UART_AEON_AUDIO_R2:
            src = UART_SEL_AUDIO_R2;
            break;

        case E_UART_SECURE_R2:
            src = UART_SEL_SECURE_R2;
            break;
        default:
            MS_ASSERT(0);
            return -1;  // TODO: update for new return code
    }
    switch (uart_port)
    {
        case E_UART_PORT0:
            _HAL_REG_WriteByteMask(0x1EA6, (src << 0), BITMASK(3:0));
            break;
        case E_UART_PORT1:
            _HAL_REG_WriteByteMask(0x1EA6, (src << 4), BITMASK(7:4));
            break;
        case E_UART_PORT2:
            _HAL_REG_WriteByteMask(0x1EA7, (src << 0), BITMASK(3:0));
            break;
        case E_UART_PORT3:
            _HAL_REG_WriteByteMask(0x1EA7, (src << 4), BITMASK(7:4));
            break;
        case E_UART_PORT4:
            _HAL_REG_WriteByteMask(0x1EA8, (src << 0), BITMASK(3:0));
            break;
        default:
            MS_ASSERT(0);
            return -1;  // TODO: update for new return code
    }
    return 0;

}


MS_BOOL HAL_UART_SetPadMux(UART_PAD_TYPE uart_pad)
{
    MS_U8 u8Value;

    if (/*(uart_pad>=E_UART_PAD_1_0) &&*/ (uart_pad<=E_UART_PAD_1_3))
    {
        switch (uart_pad)
        {
            case E_UART_PAD_1_0:
                u8Value = 0;
                break;
            case E_UART_PAD_1_1:
                u8Value = CHIP_SecondUART_PAD_1;
                break;
            case E_UART_PAD_1_2:
                u8Value = CHIP_SecondUART_PAD_2;
                break;
            case E_UART_PAD_1_3:
                u8Value = CHIP_SecondUART_PAD_3;
                break;
            default:
                return FALSE;
        }
        _HAL_REG_WriteByteMask(CHIP_REG_SecondUART, u8Value, CHIP_SecondUART_PAD_MASK);
        return TRUE;
    }
    else if ((uart_pad>=E_UART_PAD_2_0) && (uart_pad<=E_UART_PAD_2_3))
    {
        switch (uart_pad)
        {
            case E_UART_PAD_2_0:
                u8Value = 0;
                break;
            case E_UART_PAD_2_1:
                u8Value = CHIP_ThirdUART_PAD_1;
                break;
            case E_UART_PAD_2_2:
                u8Value = CHIP_ThirdUART_PAD_2;
                break;
            case E_UART_PAD_2_3:
                u8Value = CHIP_ThirdUART_PAD_3;
                break;
            default:
                return FALSE;
        }
        _HAL_REG_WriteByteMask(CHIP_REG_ThirdUART, u8Value, CHIP_ThirdUART_PAD_MASK);
        return TRUE;
    }
    else if ((uart_pad>=E_UART_PAD_3_0) && (uart_pad<=E_UART_PAD_3_3))
    {
        switch (uart_pad)
        {
            case E_UART_PAD_3_0:
                u8Value = 0;
                break;
            case E_UART_PAD_3_1:
                u8Value = CHIP_FourthUART_PAD_1;
                break;
            case E_UART_PAD_3_2:
                u8Value = CHIP_FourthUART_PAD_2;
                break;
            case E_UART_PAD_3_3:
                u8Value = CHIP_FourthUART_PAD_3;
                break;
            default:
                return FALSE;
        }
        _HAL_REG_WriteByteMask(CHIP_REG_FourthUART, u8Value, CHIP_FourthUART_PAD_MASK);
        return TRUE;
    }
    else if ((uart_pad>=E_UART_PAD_4_0) && (uart_pad<=E_UART_PAD_4_3))
    {
        switch (uart_pad)
        {
            case E_UART_PAD_4_0:
                u8Value = 0;
                break;
            case E_UART_PAD_4_1:
                u8Value = CHIP_FastUART_PAD_1;
                break;
            case E_UART_PAD_4_2:
                u8Value = CHIP_FastUART_PAD_2;
                break;
            case E_UART_PAD_4_3:
                u8Value = CHIP_FastUART_PAD_3;
                break;
            default:
                return FALSE;
        }
        _HAL_REG_WriteByteMask(CHIP_REG_FastUART, u8Value, CHIP_FastUART_PAD_MASK);
        return TRUE;
    }
    return FALSE;
}

MS_U16 HAL_UART_Invert(UART_PORT_TYPE uart_port, MS_BOOL bInv)
{

    switch (uart_port)
    {
        case E_UART_PORT0:
            _HAL_REG_WriteByteMask(0x1EAB, bInv*BIT0, BITMASK(0:0));
            break;
        case E_UART_PORT1:
            _HAL_REG_WriteByteMask(0x1EAB, bInv*BIT1, BITMASK(1:1));
            break;
        case E_UART_PORT2:
            _HAL_REG_WriteByteMask(0x1EAB, bInv*BIT2, BITMASK(2:2));
            break;
        case E_UART_PORT3:
            _HAL_REG_WriteByteMask(0x1EAB, bInv*BIT3, BITMASK(3:3));
            break;
        case E_UART_PORT4:
            _HAL_REG_WriteByteMask(0x1EAB, bInv*BIT4, BITMASK(4:4));
            break;
        default:
            MS_ASSERT(0);
            return -1;  // TODO: update for new return code
    }
    return 0;

}

#if defined(MCU_AEON)
int HAL_UART_Aeon_Close(device_t dev)
{
    ms_uart_dev_t u;

    u = UART_DEVICE(dev);
    if (u->device.refcnt > 0)
    {
        u->device.refcnt--;
        if (u->device.refcnt == 0)
        {
            u->rx_flags |= RX_FLAG_DISABLED;
            u->tx_flags |= TX_FLAG_DISABLED;
        }
    }
    return 0;
}

int HAL_UART_Aeon_Write(device_t dev, const char *buf, MS_U32 len, int off)
{
    int nr_xmit;
    int tx_buf_in;
    ms_uart_dev_t u;

    UNUSED(off);

    u = UART_DEVICE(dev);

    nr_xmit = 0;

    if (!u->tx_buf)
    {
        while (len-- > 0)
        {
            while (!(AEON_REG8(UART_LSR) & UART_LSR_THRE));
            AEON_REG8(UART_TX) = *buf++;
            nr_xmit++;
        }
    }
    else
    {
try_again:
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_LockInterrupt();
#endif
        if (u->tx_flags & TX_FLAG_DIRECT)
        {
            MS_ASSERT(u->tx_buf_out == u->tx_buf_in);

            while (len-- > 0)
            {
                if (!(AEON_REG8(UART_LSR) & UART_LSR_THRE))
                {
                    break;
                }
                AEON_REG8(UART_TX) = *buf++;
                nr_xmit++;
            }
            u->tx_flags &= ~TX_FLAG_DIRECT;
        }
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_UnLockInterrupt();
#endif

        while (len > 0)
        {
            tx_buf_in = u->tx_buf_in + 1;
            if (tx_buf_in >= u->tx_buf_len)
            {
                tx_buf_in = 0;
            }
            if (u->tx_buf_out == tx_buf_in)
            {
                goto try_again;
            }
            u->tx_buf[u->tx_buf_in] = *buf++;
            u->tx_buf_in = tx_buf_in;
            nr_xmit++;
            len--;
        }
    }
    return nr_xmit;
}

int HAL_UART_Aeon_Read(device_t dev, char *buf, MS_U32 len, int off)
{
    ms_uart_dev_t u;
    int nr_recv;

    UNUSED(off);

    u = UART_DEVICE(dev);

    nr_recv = 0;
    if (!u->rx_buf)
    {

#ifdef MSOS_TYPE_NOS
        //int picmr;

        //picmr = mfspr(SPR_PICMR);
        //mtspr(SPR_PICMR, picmr & ~(1 << 19));
        MsOS_CPU_MaskInterrupt(E_INTERRUPT_UART);
#endif

        while (len-- > 0)
        {
            while (!(AEON_REG8(UART_LSR) & UART_LSR_DR));
            buf[nr_recv++] = AEON_REG8(UART_RX);
        }

#ifdef MSOS_TYPE_NOS
        //mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (picmr & (1 << 19)));
        MsOS_CPU_UnMaskInterrupt(E_INTERRUPT_UART);
#endif
    }
    else
    {
        while (len-- > 0)
        {
            while (u->rx_buf_in != u->rx_buf_out)
            {
                buf[nr_recv++] = u->rx_buf[u->rx_buf_out];
                u->rx_buf_out++;
                if (u->rx_buf_out >= u->rx_buf_len)
                {
                    u->rx_buf_out = 0;
                }
            }
        }
    }
    return nr_recv;
}

int HAL_UART_Aeon_Poll(device_t dev, int types)
{
    ms_uart_dev_t u;

    u = UART_DEVICE(dev);

    if (types & POLLIN)
    {
        if (u->rx_buf)
        {
            if (u->rx_buf_in != u->rx_buf_out)
            {
                return 1;
            }
        }
        else
        {
            if (AEON_REG8(UART_LSR) & UART_LSR_DR)
            {
                return 1;
            }
        }
    }
    if (types & POLLOUT)
    {
        if (u->tx_buf)
        {
            if (((u->tx_buf_in + 1) % u->tx_buf_len) != u->tx_buf_out)
            {
                return 1;
            }
        }
        else
        {
            if (AEON_REG8(UART_LSR) & UART_LSR_THRE)
            {
                return 1;
            }
        }
    }
    return 0;
}

int HAL_UART_Aeon_Set_Baudrate(ms_uart_dev_t u, MS_U32 baudrate)
{
    MS_U32 divisor;
    MS_U32 Fre = 0;

    UNUSED(u);

    /* Set baud rate */
    //divisor = mdrv_clkgen_get(CKG_MCU) / (16 * baudrate);
    switch((_HAL_REG_ReadByte(0x337A)>>2)&0x3)
    {
        case 0x0:
            Fre = 288000000;
            break;

        case 0x1:
            Fre = 240000000;
            break;

        case 0x2:
            Fre = 216000000;
            break;

        case 0x3:
            Fre = 12000000; //XTAL Clock
            break;

        default:
            //printf("Frequency error \n");
            break;
    }

    divisor = ((Fre) / (16 * baudrate));

    AEON_REG8(UART_LCR) |= UART_LCR_DLAB;
    AEON_REG8(UART_DLL) = divisor & 0xFF;
    AEON_REG8(UART_DLM) = (divisor >> 8) & 0xFF;
    AEON_REG8(UART_LCR) &= ~(UART_LCR_DLAB);
    return 0;
}

#ifdef MSOS_TYPE_NOS
void HAL_UART_Aeon_Isr ( MHAL_SavedRegisters *pHalReg, MS_U32 u32Data )
{
    ms_uart_dev_t   u;
    int             c;
    int             iir;

    UNUSED(pHalReg);

    u = UART_DEVICE(u32Data);

    iir = AEON_REG8(UART_IIR);
    if (!(iir & UART_IIR_NO_INT))
    {
        switch (iir & UART_IIR_ID)
        {
        case UART_IIR_RDI:
            while (AEON_REG8(UART_LSR) & UART_LSR_DR)
            {
                c = AEON_REG8(UART_RX);
                if (!(u->rx_flags & RX_FLAG_DISABLED))
                {
                    if (u->rx_buf)
                    {
                        u->rx_buf[u->rx_buf_in++] = c;
                        if (u->rx_buf_in >= u->rx_buf_len)
                        {
                            u->rx_buf_in = 0;
                        }
                        if (u->rx_buf_in == u->rx_buf_out)
                        {
                            u->rx_flags |= RX_FLAG_OVERFLOW;
                        }
                    }
                    if (u->rx_callback)
                    {
                        u->rx_callback(c);
                    }
                }
            }
            // fall through

        case UART_IIR_THRI:
            if (u->tx_buf_in == u->tx_buf_out)
            {
                u->tx_flags |= TX_FLAG_DIRECT;
            }
            else
            {
                while ((AEON_REG8(UART_LSR) & UART_LSR_THRE)
                    && (u->tx_buf_in != u->tx_buf_out))
                {
                    c = u->tx_buf[u->tx_buf_out++];
                    if (!(u->tx_flags & TX_FLAG_DISABLED))
                    {
                        AEON_REG8(UART_TX) = c;
                        if (u->tx_buf_out >= u->tx_buf_len)
                        {
                            u->tx_buf_len = 0;
                        }
                    }
                }
            }
        }
    }
}

void HAL_UART_Aeon_Isr_HalReg ( MHAL_SavedRegisters *pHalReg, MS_U32 u32Data )
{
    ms_uart_dev_t   u;
    int             c;
    int             iir;

    UNUSED(pHalReg);

    u = UART_DEVICE(u32Data);

    iir = AEON_REG8(UART_IIR);
    if (!(iir & UART_IIR_NO_INT))
    {
        switch (iir & UART_IIR_ID)
        {
        case UART_IIR_RDI:
            while (AEON_REG8(UART_LSR) & UART_LSR_DR)
            {
                c = AEON_REG8(UART_RX);
                if (!(u->rx_flags & RX_FLAG_DISABLED))
                {
                    if (u->rx_buf)
                    {
                        u->rx_buf[u->rx_buf_in++] = c;
                        if (u->rx_buf_in >= u->rx_buf_len)
                        {
                            u->rx_buf_in = 0;
                        }
                        if (u->rx_buf_in == u->rx_buf_out)
                        {
                            u->rx_flags |= RX_FLAG_OVERFLOW;
                        }
                    }
                    if (u->rx_callback_halreg)
                    {
                        u->rx_callback_halreg(c, (MS_U32)pHalReg);
                    }
                }
            }
            // fall through

        case UART_IIR_THRI:
            if (u->tx_buf_in == u->tx_buf_out)
            {
                u->tx_flags |= TX_FLAG_DIRECT;
            }
            else
            {
                while ((AEON_REG8(UART_LSR) & UART_LSR_THRE)
                    && (u->tx_buf_in != u->tx_buf_out))
                {
                    c = u->tx_buf[u->tx_buf_out++];
                    if (!(u->tx_flags & TX_FLAG_DISABLED))
                    {
                        AEON_REG8(UART_TX) = c;
                        if (u->tx_buf_out >= u->tx_buf_len)
                        {
                            u->tx_buf_len = 0;
                        }
                    }
                }
            }
        }
    }
}

#endif

int HAL_UART_Aeon_Set_Rx_Callback(ms_uart_dev_t u, void (*rx_cb)(int c))
{
    if (rx_cb)
    {
        u->rx_callback = rx_cb;
        AEON_REG8(UART_IER) |= UART_IER_RDI;
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_AttachInterrupt(E_INTERRUPT_UART, HAL_UART_Aeon_Isr, (MS_U32) u);
        MsOS_CPU_UnMaskInterrupt(E_INTERRUPT_UART);
#endif
    }
    else
    {
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_MaskInterrupt(E_INTERRUPT_UART);
#endif
        AEON_REG8(UART_IER) &= ~UART_IER_RDI;
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_DetachInterrupt(E_INTERRUPT_UART);
#endif
        u->rx_callback = 0;
    }
    return 0;
}

int HAL_UART_Aeon_Set_Rx_Callback_HalReg(ms_uart_dev_t u, void (*rx_cb)(int c, MS_U32 pHalReg))
{
    if (rx_cb)
    {
        u->rx_callback_halreg = rx_cb;
        AEON_REG8(UART_IER) |= UART_IER_RDI;
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_AttachInterrupt(E_INTERRUPT_UART, HAL_UART_Aeon_Isr, (MS_U32) u);
        MsOS_CPU_UnMaskInterrupt(E_INTERRUPT_UART);
#endif
    }
    else
    {
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_MaskInterrupt(E_INTERRUPT_UART);
#endif
        AEON_REG8(UART_IER) &= ~UART_IER_RDI;
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_DetachInterrupt(E_INTERRUPT_UART);
#endif
        u->rx_callback_halreg = 0;
    }
    return 0;
}

int HAL_UART_Aeon_Ioctl(device_t dev, int request, unsigned long arg)
{
    ms_uart_dev_t u;

    u = UART_DEVICE(dev);
    switch (request)
    {
        case IOCTL_SET_BAUDRATE:
            return HAL_UART_Aeon_Set_Baudrate(u, *(MS_U32 *)arg);
        case IOCTL_SET_RXCALLBACK:
            return HAL_UART_Aeon_Set_Rx_Callback(u, *(ms_uart_rx_callback *)arg);
        case IOCTL_SET_RXCALLBACK_HALREG:
            return HAL_UART_Aeon_Set_Rx_Callback_HalReg(u, *(ms_uart_rx_callback_halreg*)arg);
        default:
            MS_ASSERT(0);
    }
    return -1;
}

int HAL_UART_Aeon_Open(device_t dev, int mode)
{
    ms_uart_dev_t u;

    UNUSED(mode);
    u = UART_DEVICE(dev);
    if (u->device.refcnt == 0)
    {
        u->regbase = 0x90000000;
        u->rx_flags = 0;
        u->tx_flags = 0;

        AEON_REG8(UART_IER) = 0;

        /* Reset receiver and transmiter */
        AEON_REG8(UART_FCR) = UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT | UART_FCR_TRIGGER_14;

        /* Interrupt Enable Register */
        AEON_REG8(UART_IER) = 0;

        /* Set 8 bit char, 1 stop bit, no parity */
        AEON_REG8(UART_LCR) = UART_LCR_WLEN8 & ~(UART_LCR_STOP2 | UART_LCR_PARITY);
    }
    u->device.refcnt++;

    return (int)(dev);
}
#endif

int HAL_UART_PIU_Close(device_t dev)
{
    ms_uart_dev_t u;

    u = UART_DEVICE(dev);
    if (u->device.refcnt > 0)
    {
        u->device.refcnt--;
        if (u->device.refcnt == 0)
        {
            u->rx_flags |= RX_FLAG_DISABLED;
            u->tx_flags |= TX_FLAG_DISABLED;
        }
    }
    return 0;
}

int HAL_UART_PIU_Write(device_t dev, const char *buf, MS_U32 len, int off)
{
    int nr_xmit;
    int tx_buf_in;
    ms_uart_dev_t u;

    UNUSED(off);

    u = UART_DEVICE(dev);

    nr_xmit = 0;

    if (!u->tx_buf)
    {
        while (len-- > 0)
        {
            while (!(UART_REG8(UART_LSR) & UART_LSR_THRE));
            UART_REG8(UART_TX) = *buf++;
            nr_xmit++;
        }
    }
    else
    {
try_again:
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_LockInterrupt();
#endif
        if (u->tx_flags & TX_FLAG_DIRECT)
        {
            MS_ASSERT(u->tx_buf_out == u->tx_buf_in);

            while (len-- > 0)
            {
                if (!(UART_REG8(UART_LSR) & UART_LSR_THRE))
                {
                    break;
                }
                UART_REG8(UART_TX) = *buf++;
                nr_xmit++;
            }
            u->tx_flags &= ~TX_FLAG_DIRECT;
        }
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_UnLockInterrupt();
#endif

        while (len > 0)
        {
            tx_buf_in = u->tx_buf_in + 1;
            if (tx_buf_in >= u->tx_buf_len)
            {
                tx_buf_in = 0;
            }
            if (u->tx_buf_out == tx_buf_in)
            {
                goto try_again;
            }
            u->tx_buf[u->tx_buf_in] = *buf++;
            u->tx_buf_in = tx_buf_in;
            nr_xmit++;
            len--;
        }
    }
    return nr_xmit;
}

int HAL_UART_PIU_Read(device_t dev, char *buf, MS_U32 len, int off)
{
    ms_uart_dev_t u;
    int nr_recv;

    UNUSED(off);

    u = UART_DEVICE(dev);

#if !defined(MSOS_TYPE_ECOS)
    if (u->rx_callback || u->rx_callback_halreg)
    {
        return -1;
    }
#endif
    nr_recv = 0;
    if (!u->rx_buf)
    {
        while (len-- > 0)
        {
            while (!(UART_REG8(UART_LSR) & UART_LSR_DR));
            buf[nr_recv++] = UART_REG8(UART_RX);
        }
    }
    else
    {
        while (nr_recv < len)
        {
            if (u->rx_buf_in != u->rx_buf_out)
            {
                buf[nr_recv++] = u->rx_buf[u->rx_buf_out];
                u->rx_buf_out++;
                if (u->rx_buf_out >= u->rx_buf_len)
                {
                    u->rx_buf_out = 0;
                }
            }
        }
    }
    return nr_recv;
}

int HAL_UART_PIU_Poll(device_t dev, int types)
{
    ms_uart_dev_t u;

    u = UART_DEVICE(dev);

    if (types & POLLIN)
    {
        if (u->rx_buf)
        {
            if (u->rx_buf_in != u->rx_buf_out)
            {
                return 1;
            }
        }
        else
        {
            if (UART_REG8(UART_LSR) & UART_LSR_DR)
            {
                return 1;
            }
        }
    }
    if (types & POLLOUT)
    {
        if (u->tx_buf)
        {
            if (((u->tx_buf_in + 1) % u->tx_buf_len) != u->tx_buf_out)
            {
                return 1;
            }
        }
        else
        {
            if (UART_REG8(UART_LSR) & UART_LSR_THRE)
            {
                return 1;
            }
        }
    }
    return 0;
}

int HAL_UART_PIU_Set_Baudrate(ms_uart_dev_t u, MS_U32 baudrate)
{
    MS_U32 divisor;
    MS_U32 uart_clock = 0;
    MS_U8 uart_clock_option = -1;
    UNUSED(u);

    switch (u->regbase)
    {
    case UART0_PIU_ADDR:
        uart_clock_option = _HAL_REG_ReadByte(UART0_CLK_REG);
        break;

    case UART1_PIU_ADDR:
        uart_clock_option = _HAL_REG_ReadByte(UART1_CLK_REG);
        break;

    case UART2_PIU_ADDR:
        uart_clock_option = _HAL_REG_ReadByte(UART2_CLK_REG);
        break; 

    case FUART_PIU_ADDR:
        uart_clock_option = _HAL_REG_ReadByte(FUART_CLK_REG);
        break;

    default:
        uart_clock_option = 0xC;
        break;
    }


    switch (uart_clock_option)
    {
    case 0x0:
        uart_clock = UART_CLK_FREQ_172M;
        break;

    case 0x4:
        uart_clock = UART_CLK_FREQ_160M;
        break;
        
    case 0x8:
        uart_clock = UART_CLK_FREQ_144M;
        break;

    case 0x10:
        uart_clock = UART_CLK_FREQ_108M;
        break;

    case 0xC:
    default:
        uart_clock = UART_CLK_FREQ;
        break;
    }

    //i.    Set "reg_mcr_loopback";
    UART_REG8(UART_MCR) |= 0x10;

    //    Clear FIFO Buffer
    UART_REG8(UART_FCR) |= 0x07;

    //ii.    Poll "reg_usr_busy" till 0;
    while(UART_REG8(UART_USR) & 0x01);

    /* Set baud rate */
    //divisor = mdrv_clkgen_get(CKG_UART) / (16 * baudrate);
    divisor = ((uart_clock) / (16 * baudrate));

    UART_REG8(UART_LCR) |= UART_LCR_DLAB;
    UART_REG8(UART_DLL) = divisor & 0xFF;
    UART_REG8(UART_DLM) = (divisor >> 8) & 0xFF;
    UART_REG8(UART_LCR) &= ~(UART_LCR_DLAB);

    UART_REG8(UART_MCR) &= ~0x10;
    return 0;
}

static void _HAL_UART_PIU_Isr(UART_DEVICE_TYPE uart_dev)//ISR Body
{

    ms_uart_dev_t   u;

    int             c;
    int             iir;
    u = &_uart_dev[uart_dev];
    iir = UART_REG8(UART_IIR);
    if (!(iir & UART_IIR_NO_INT))
    {
        switch (iir & UART_IIR_ID)
        {
        case UART_IIR_RDI:
            while (UART_REG8(UART_LSR) & UART_LSR_DR)
            {
                c = UART_REG8(UART_RX);
                if (!(u->rx_flags & RX_FLAG_DISABLED))
                {
                    if (u->rx_buf)
                    {
                        u->rx_buf[u->rx_buf_in++] = c;
                        if (u->rx_buf_in >= u->rx_buf_len)
                        {
                            u->rx_buf_in = 0;
                        }
                        if (u->rx_buf_in == u->rx_buf_out)
                        {
                            u->rx_flags |= RX_FLAG_OVERFLOW;
                        }
                    }
                    if (u->rx_callback)
                    {
                        u->rx_callback(c);
                    }
                }
            }
            // fall through

        case UART_IIR_THRI:
            if (u->tx_buf_in == u->tx_buf_out)
            {
                u->tx_flags |= TX_FLAG_DIRECT;
            }
            else
            {
                while ((UART_REG8(UART_LSR) & UART_LSR_THRE)
                    && (u->tx_buf_in != u->tx_buf_out))
                {
                    c = u->tx_buf[u->tx_buf_out++];
                    if (!(u->tx_flags & TX_FLAG_DISABLED))
                    {
                        UART_REG8(UART_TX) = c;
                        if (u->tx_buf_out >= u->tx_buf_len)
                        {
                            u->tx_buf_len = 0;
                        }
                    }
                }
            }
        }
    }

}


void HAL_UART0_PIU_Isr(MS_U32 *pHalReg, MS_U32 u32Data)
{
       UNUSED(pHalReg);

       UNUSED(u32Data);

       _HAL_UART_PIU_Isr(E_UART_PIU_UART0);

       MsOS_EnableInterrupt(E_INT_IRQ_UART0);
}

void HAL_UART1_PIU_Isr(MS_U32 *pHalReg, MS_U32 u32Data)
{
        UNUSED(pHalReg);

        UNUSED(u32Data);

        _HAL_UART_PIU_Isr(E_UART_PIU_UART1);

        MsOS_EnableInterrupt(E_INT_IRQ_UART1);
}

int HAL_UART_PIU_Set_Rx_Callback(ms_uart_dev_t u, void (*rx_cb)(int c))
{
//#define IRQ_UART0   (MDRV_IRQ_BEGIN + 0)
    if (rx_cb)
    {
        u->rx_callback = rx_cb;
        UART_REG8(UART_IER) |= UART_IER_RDI;

        if( u->regbase == UART1_PIU_ADDR )
        {
           MsOS_AttachInterrupt(E_INT_IRQ_UART1, (InterruptCb)HAL_UART1_PIU_Isr);
           MsOS_EnableInterrupt(E_INT_IRQ_UART1);
        }
        else
        {
            MsOS_AttachInterrupt(E_INT_IRQ_UART0, (InterruptCb)HAL_UART0_PIU_Isr);
            MsOS_EnableInterrupt(E_INT_IRQ_UART0);
        }

    }
    else
    {
        if( u->regbase == UART1_PIU_ADDR )
        {
           MsOS_DisableInterrupt(E_INT_IRQ_UART1);
           UART_REG8(UART_IER) &= ~UART_IER_RDI;
           MsOS_DetachInterrupt(E_INT_IRQ_UART1);
        }
        else
        {
            MsOS_DisableInterrupt(E_INT_IRQ_UART0);
            UART_REG8(UART_IER) &= ~UART_IER_RDI;
            MsOS_DetachInterrupt(E_INT_IRQ_UART0);
        }
        u->rx_callback = 0;
    }
    return 0;
}

void _HAL_UART_PIU_Isr_HalReg(UART_DEVICE_TYPE uart_dev, MS_U32 *pHalReg, MS_U32 u32Data)
{
    ms_uart_dev_t   u;
    int             c;
    int             iir;

    //UNUSED(pHalReg);
    UNUSED(u32Data);

    //u = UART_DEVICE(u32Data);
    u = &_uart_dev[uart_dev];

    iir = UART_REG8(UART_IIR);
    if (!(iir & UART_IIR_NO_INT))
    {
        switch (iir & UART_IIR_ID)
        {
        case UART_IIR_RDI:
            while (UART_REG8(UART_LSR) & UART_LSR_DR)
            {
                c = UART_REG8(UART_RX);
                if (!(u->rx_flags & RX_FLAG_DISABLED))
                {
                    if (u->rx_buf)
                    {
                        u->rx_buf[u->rx_buf_in++] = c;
                        if (u->rx_buf_in >= u->rx_buf_len)
                        {
                            u->rx_buf_in = 0;
                        }
                        if (u->rx_buf_in == u->rx_buf_out)
                        {
                            u->rx_flags |= RX_FLAG_OVERFLOW;
                        }
                    }
                    if (u->rx_callback_halreg)
                    {
                        u->rx_callback_halreg(c, (MS_VIRT)pHalReg);
                    }
                }
            }
            // fall through

        case UART_IIR_THRI:
            if (u->tx_buf_in == u->tx_buf_out)
            {
                u->tx_flags |= TX_FLAG_DIRECT;
            }
            else
            {
                while ((UART_REG8(UART_LSR) & UART_LSR_THRE)
                    && (u->tx_buf_in != u->tx_buf_out))
                {
                    c = u->tx_buf[u->tx_buf_out++];
                    if (!(u->tx_flags & TX_FLAG_DISABLED))
                    {
                        UART_REG8(UART_TX) = c;
                        if (u->tx_buf_out >= u->tx_buf_len)
                        {
                            u->tx_buf_len = 0;
                        }
                    }
                }
            }
        }
    }
}


void HAL_UART0_PIU_Isr_HalReg(MS_U32 *pHalReg, MS_U32 u32Data)
{

    _HAL_UART_PIU_Isr_HalReg(E_UART_PIU_UART0, pHalReg, u32Data);

    MsOS_EnableInterrupt(E_INT_IRQ_UART0);
}

void HAL_UART1_PIU_Isr_HalReg(MS_U32 *pHalReg, MS_U32 u32Data)
{

    _HAL_UART_PIU_Isr_HalReg(E_UART_PIU_UART1, pHalReg, u32Data);

    MsOS_EnableInterrupt(E_INT_IRQ_UART1);
}



int HAL_UART_PIU_Set_Rx_Callback_HalReg(ms_uart_dev_t u, void (*rx_cb)(int c, MS_U32 pHalReg))
{
//#define IRQ_UART0   (MDRV_IRQ_BEGIN + 0)
    if (rx_cb)
    {
        u->rx_callback_halreg = rx_cb;
        UART_REG8(UART_IER) |= UART_IER_RDI;

        if( u->regbase == UART1_PIU_ADDR )
        {
           MsOS_AttachInterrupt(E_INT_IRQ_UART1, (InterruptCb)HAL_UART1_PIU_Isr_HalReg);
           MsOS_EnableInterrupt(E_INT_IRQ_UART1);
        }
        else
        {
            MsOS_AttachInterrupt(E_INT_IRQ_UART0, (InterruptCb)HAL_UART0_PIU_Isr_HalReg);
            MsOS_EnableInterrupt(E_INT_IRQ_UART0);
        }
    }
    else
    {
        //mdrv_irq_mask(IRQ_UART0);
        MsOS_DisableInterrupt(E_INT_IRQ_UART0);
        UART_REG8(UART_IER) &= ~UART_IER_RDI;
        //mdrv_irq_detach(IRQ_UART0);
        MsOS_DetachInterrupt(E_INT_IRQ_UART0);
        u->rx_callback_halreg = 0;
    }
    return 0;
}

int HAL_UART_PIU_Ioctl(device_t dev, int request, unsigned long arg)
{
    ms_uart_dev_t u;

    u = UART_DEVICE(dev);
    switch (request)
    {
        case IOCTL_SET_BAUDRATE:
            return HAL_UART_PIU_Set_Baudrate(u, *(MS_U32 *)arg);
        case IOCTL_SET_RXCALLBACK:
            return HAL_UART_PIU_Set_Rx_Callback(u, *(ms_uart_rx_callback *)arg);
        case IOCTL_SET_RXCALLBACK_HALREG:
            return HAL_UART_PIU_Set_Rx_Callback_HalReg(u, *(ms_uart_rx_callback_halreg*)arg);
        default:
            MS_ASSERT(0);
    }
    return -1;
}

int HAL_UART_PIU_Open(device_t dev, int mode)
{
    ms_uart_dev_t u;

    UNUSED(mode);
    u = UART_DEVICE(dev);
    if (u->device.refcnt == 0)
    {
        u->rx_flags = 0;
        u->tx_flags = 0;

        // Disable all interrupts
        UART_REG8(UART_IER) = 0x00;

        // card_reset_reg must set to 1 for rx
        UART_REG8(8 * 2) = 0x10;

        // Reset receiver and transmiter
        UART_REG8(UART_FCR) = UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT | UART_FCR_TRIGGER_1;

        // Set 8 bit char, 1 stop bit, no parity
        UART_REG8(UART_LCR) = UART_LCR_WLEN8 & ~(UART_LCR_STOP2 | UART_LCR_PARITY);
    }
    u->device.refcnt++;

    return (int)(dev);
}

void HAL_UART_SetUARTSecurityBank(UART_DEVICE_TYPE uart_dev)
{
    printf("not realize HAL_UART_SetUARTSecurityBank  yet\n");
    return;
}
