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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvUART.c
/// @brief  UART Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include <string.h>
#include "MsIRQ.h"
#include "drvMMIO.h"

#include "ULog.h"

// Internal Definition
#include "regUART.h"
#include "drvUART.h"
#include "halUART.h"
#include "utopia.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_UART "UART"

#define UART_DBG_FUNC()             if (_uartDbgLevel >= E_UART_DBGLV_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_UART, "\t====   %s   ====\n", __FUNCTION__));}
#define UART_DBG_INFO(x, args...)   if (_uartDbgLevel >= E_UART_DBGLV_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_UART, x, ##args));}
#define UART_DBG_ERR(x, args...)    if (_uartDbgLevel >= E_UART_DBGLV_ERR_ONLY) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_UART, x, ##args));}

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
#if (defined(MCU_AEON))
struct devio aeon_uart_io =
{
    .open   = HAL_UART_Aeon_Open,
    .close  = HAL_UART_Aeon_Close,
    .read   = HAL_UART_Aeon_Read,
    .write  = HAL_UART_Aeon_Write,
    .poll   = HAL_UART_Aeon_Poll,
    .ioctl  = HAL_UART_Aeon_Ioctl,
    .event  = 0,
};
#endif

struct devio piu_uart_io =
{
    .open   = HAL_UART_PIU_Open,
    .close  = HAL_UART_PIU_Close,
    .read   = HAL_UART_PIU_Read,
    .write  = HAL_UART_PIU_Write,
    .poll   = HAL_UART_PIU_Poll,
    .ioctl  = HAL_UART_PIU_Ioctl,
    .event  = 0,
};

#undef putchar
#undef kbhit
#undef getchar
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#define HAL_UART_CAPS               (UART_CAPS_AEON | UART_CAPS_PIU)

// Uart driver info
static UART_DrvInfo                 _uartInfo =
                                    {
                                        .Init = FALSE,
                                        .Caps = HAL_UART_CAPS,
                                    };

static UART_DrvStatus               _uartStatus =
                                    {
                                        .HwBusy = FALSE,
                                    };

static MSIF_Version _uartVersion = {
    .DDI = { UART_DRV_VERSION },
};

static UART_DbgLv _uartDbgLevel = E_UART_DBGLV_ERR_ONLY;

// UART Driver parameter
#define UART_MAX 4
#if defined(CONFIG_FPGA)
#define DEFAULT_BAUDRATE 38400
#else
#define DEFAULT_BAUDRATE 115200
#endif

static UART_DEVICE_TYPE _eDefaultUart = E_UART_PIU_UART0;
static MS_U32          _u32DefaultUartBaudrate = DEFAULT_BAUDRATE;
static MS_U32          _hDefaultUart ;
static MS_BOOL         _bInitFlag =FALSE;

typedef struct _UART_DrvDisc
{
UART_DEVICE_TYPE     Uart_dev;
MS_U32               baudrate;
MS_U32               UartHandle;
ms_uart_rx_callback  rx_cb;
MS_BOOL              enable;
}UART_DrvDisc;

static UART_DrvDisc _UartDisc[UART_MAX]=
{
    {E_UART_PIU_UART0,DEFAULT_BAUDRATE,0,NULL,FALSE},
    {E_UART_PIU_UART1,DEFAULT_BAUDRATE,0,NULL,FALSE},
    {E_UART_PIU_UART2,DEFAULT_BAUDRATE,0,NULL,FALSE},
    {E_UART_PIU_FUART0,DEFAULT_BAUDRATE,0,NULL,FALSE}
};

#if UART_TYPE_EYWA

static Eywa_FastUart_struct            _EywaPacket =
									{
										.data_type = E_EYWA_INVALID,
										.data_type_inv = E_EYWA_INV_INVALID,
										.len = EYWA_PAYLOAD_LEN,
										.payload = {0},
										.parity = EYWA_DEFAULT_PARITY
									};
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
UART_Result MDrv_UART_SetIOMapBase(void)
{
    MS_VIRT u32BaseAddr = 0;
	MS_PHY u32BaseSize = 0;
	MS_VIRT u32PMBaseAddr = 0;
	MS_PHY u32PMBaseSize = 0;

    if(!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_UART))
    {
        UART_DBG_ERR("Get IOMAP Base faill!\n");
        return E_UART_FAIL;
    }
	if(!MDrv_MMIO_GetBASE(&u32PMBaseAddr, &u32PMBaseSize, MS_MODULE_PM))
    {
        UART_DBG_ERR("Get IOMAP Base faill!\n");
        return E_UART_FAIL;
    }
    UART_DBG_INFO("Get IOMAP ID:%u Base:%lx!\n", MS_MODULE_UART, u32BaseAddr);
	UART_DBG_INFO("Get PM IOMAP ID:%u Base:%lx!\n", MS_MODULE_PM, u32PMBaseAddr);
    HAL_UART_SetIOMapBase(u32BaseAddr, u32PMBaseAddr);

    return E_UART_OK;
}

void MDrv_UART_Init(UART_DEVICE_TYPE uart_dev, MS_U32 baudrate)
{
    if ( E_UART_FAIL == MDrv_UART_SetIOMapBase())
    {
        return;
    }

#if (defined(MCU_AEON))
    _uart_dev[E_UART_AEON].device.magic = DEVICE_MAGIC ;
    _uart_dev[E_UART_AEON].device.refcnt = 0 ;
    _uart_dev[E_UART_AEON].device.flags = 0 ;
    _uart_dev[E_UART_AEON].device.devio = &aeon_uart_io ;
    _uart_dev[E_UART_AEON].regbase = UART_AEON_ADDR ;
    _uart_dev[E_UART_AEON].uarttype = UART_TYPE_AEON ;
    #if defined(__AEONR2__)
    _uart_dev[E_UART_AEON_R2].device.magic = DEVICE_MAGIC ;
    _uart_dev[E_UART_AEON_R2].device.refcnt = 0 ;
    _uart_dev[E_UART_AEON_R2].device.flags = 1 ;
    _uart_dev[E_UART_AEON_R2].device.devio = &aeon_uart_io ;
    _uart_dev[E_UART_AEON_R2].regbase = UART_AEON_ADDR ;
    _uart_dev[E_UART_AEON_R2].uarttype = UART_TYPE_AEON ;
    #endif

    _uart_dev[E_UART_SECURE_R2].device.magic = DEVICE_MAGIC ;
    _uart_dev[E_UART_SECURE_R2].device.refcnt = 0 ;
    _uart_dev[E_UART_SECURE_R2].device.flags = 2 ;
    _uart_dev[E_UART_SECURE_R2].device.devio = &aeon_uart_io ;
    _uart_dev[E_UART_SECURE_R2].regbase = UART_AEON_ADDR ;
    _uart_dev[E_UART_SECURE_R2].uarttype = UART_TYPE_AEON ;
#endif

#if defined (UART0_PIU_ADDR)
    _uart_dev[E_UART_PIU_UART0].device.magic = DEVICE_MAGIC ;
    _uart_dev[E_UART_PIU_UART0].device.refcnt = 0 ;
    _uart_dev[E_UART_PIU_UART0].device.flags = 0 ;
    _uart_dev[E_UART_PIU_UART0].device.devio = &piu_uart_io ;
    _uart_dev[E_UART_PIU_UART0].regbase = UART0_PIU_ADDR ;
    _uart_dev[E_UART_PIU_UART0].uarttype = UART_TYPE_PIU ;
#endif

#if defined (UART1_PIU_ADDR)
    _uart_dev[E_UART_PIU_UART1].device.magic = DEVICE_MAGIC ;
    _uart_dev[E_UART_PIU_UART1].device.refcnt = 0 ;
    _uart_dev[E_UART_PIU_UART1].device.flags = 0 ;
    _uart_dev[E_UART_PIU_UART1].device.devio = &piu_uart_io ;
    _uart_dev[E_UART_PIU_UART1].regbase = UART1_PIU_ADDR ;
    _uart_dev[E_UART_PIU_UART1].uarttype = UART_TYPE_PIU ;
#endif

#if defined (UART2_PIU_ADDR)
    _uart_dev[E_UART_PIU_UART2].device.magic = DEVICE_MAGIC ;
    _uart_dev[E_UART_PIU_UART2].device.refcnt = 0 ;
    _uart_dev[E_UART_PIU_UART2].device.flags = 0 ;
    _uart_dev[E_UART_PIU_UART2].device.devio = &piu_uart_io ;
    _uart_dev[E_UART_PIU_UART2].regbase = UART2_PIU_ADDR ;
    _uart_dev[E_UART_PIU_UART2].uarttype = UART_TYPE_PIU ;
#endif

#if defined (FUART_PIU_ADDR)
    _uart_dev[E_UART_PIU_FUART0].device.magic = DEVICE_MAGIC ;
    _uart_dev[E_UART_PIU_FUART0].device.refcnt = 0 ;
    _uart_dev[E_UART_PIU_FUART0].device.flags = 0 ;
    _uart_dev[E_UART_PIU_FUART0].device.devio = &piu_uart_io ;
    _uart_dev[E_UART_PIU_FUART0].regbase = FUART_PIU_ADDR ;
    _uart_dev[E_UART_PIU_FUART0].uarttype = UART_TYPE_PIU ;
#endif

    // default initialize just init once & open uart0 for DefaultUart
	_bInitFlag = TRUE;
    // default initialize
    _hDefaultUart = mdrv_uart_open(uart_dev) ; // default uart port
    mdrv_uart_set_baudrate( _hDefaultUart, baudrate) ; // set default baud rate

    // Driver Initialization
    _uartInfo.Init = TRUE;
    _uartStatus.HwBusy = FALSE;
}

struct device* MDrv_UART_GetDev(UART_DEVICE_TYPE uart_dev)
{
    if((uart_dev>=UART_DEV_MAX) || (uart_dev>=E_UART_OFF))
        return (struct device *)0;
    return &_uart_dev[uart_dev].device;
}

const UART_DrvInfo* MDrv_UART_GetInfo(void)
{
    return (&_uartInfo);
}

UART_Result MDrv_UART_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return E_UART_FAIL;
    }

    *ppVersion = &_uartVersion;

    return E_UART_OK;
}

UART_Result MDrv_UART_GetStatus(UART_DrvStatus *pStatus)
{
    memcpy(pStatus, &_uartStatus, sizeof(UART_DrvStatus));
    return E_UART_OK;
}


UART_Result MDrv_UART_SetRxMode(MS_BOOL bEnable, void *rx_buf, MS_U16 buf_len, ms_uart_rx_callback rx_cb)
{
    if (bEnable)
    {
        mdrv_uart_rx_enable(_hDefaultUart);
    }
    else
    {
        mdrv_uart_rx_disable(_hDefaultUart);
    }

    if (rx_buf && buf_len)
    {
        mdrv_uart_set_rx_buffer(_hDefaultUart, (void *)rx_buf, buf_len);
    }
    else
    {
        mdrv_uart_set_rx_buffer(_hDefaultUart, (void *)NULL, 0);
    }

    if (rx_cb)
    {
        mdrv_uart_set_rx_callback(_hDefaultUart, rx_cb);
    }
    else
    {
        mdrv_uart_set_rx_callback(_hDefaultUart, NULL);
    }
    return E_UART_OK;
}


void MDrv_UART_SetDbgLevel(MS_U8 level)
{
    _uartDbgLevel = (UART_DbgLv)level;
}

UART_Result MDrv_UART_SetPMRxEnable(MS_BOOL bEnable)
{
	return HAL_UART_SetPMRxEnable(bEnable);
}

void MDrv_UART_PutChar(MS_U8 u8Ch)
{
    MS_U8 u8Str[2] ;

    _uartStatus.HwBusy = TRUE;
    u8Str[0] = u8Ch ;
    u8Str[1] = 0 ;
    mdrv_uart_write( _hDefaultUart, u8Str, 0x01) ;
    _uartStatus.HwBusy = FALSE;
}

void MDrv_UART_PutString(char *pu8Str)
{
    _uartStatus.HwBusy = TRUE;

    mdrv_uart_write( _hDefaultUart, (const MS_U8 *)pu8Str, strlen(pu8Str)) ;

    _uartStatus.HwBusy = FALSE;
}



#if 0 //[TODO] - jerry //[SVP] add for NDS/SVP RPC
#define UART_GETSTR_TIMEOUT         1000
#define UART_GETCHAR_DELAY          5 // 5ms (min:2ms)
#endif

MS_U8 MDrv_UART_GetChar(void)
{
    MS_U8 u8Str[2] ;

    _uartStatus.HwBusy = TRUE;
#if 0 //[TODO] - jerry //[SVP] add for NDS/SVP RPC
    MS_U16 len = 0;
    #if defined(MSOS_TYPE_ECOS) //[SVP] add for NDS SVP RPC
    while (1)
    {
    #endif
        len = mdrv_uart_read( _hDefaultUart, u8Str, 0x01) ;
    #if defined(MSOS_TYPE_ECOS) //[SVP] add for NDS SVP RPC
        if (len) break;

//        MsOS_DelayTask(1);
    }
    #endif
#else
    mdrv_uart_read( _hDefaultUart, u8Str, 0x01) ;
#endif

    u8Str[1] = 0 ;
    _uartStatus.HwBusy = FALSE;

    return u8Str[0] ;
}


#if 0 //[TODO] - jerry //[SVP] add for NDS/SVP RPC
MS_U8 MDrv_UART_GetString(char *pu8Str, MS_U32 max_len)
{
    MS_U32      start_time = 0;
    MS_U16      len, ret_len = 0;

    _uartStatus.HwBusy = TRUE;

    start_time = MsOS_GetSystemTime();
    while (max_len > 0)
    {
        len = mdrv_uart_read( _hDefaultUart, pu8Str, max_len);
        if (len) // len <= max_len
        {
            max_len -= len;
            pu8Str += len;
            ret_len += len;
            start_time = MsOS_GetSystemTime();
        }
        if ((MsOS_GetSystemTime() - start_time) > UART_GETSTR_TIMEOUT)
        {
            break;
        }
        MsOS_DelayTask(UART_GETCHAR_DELAY);
    }

    if (max_len > 0)
    {
        *(MS_U8*)pu8Str = 0x00;
    }

    _uartStatus.HwBusy = FALSE;

    return ret_len;
}
#endif


void MDrv_UART_GetFile(MS_U8 *pu8Buf, MS_U32 u32Len)
{
    _uartStatus.HwBusy = TRUE;
    mdrv_uart_read( _hDefaultUart, pu8Buf, u32Len) ;
    _uartStatus.HwBusy = FALSE;
}

MS_U32 mdrv_uart_open(UART_DEVICE_TYPE uart_dev)
{
    struct device *dev;

    _uartStatus.HwBusy = TRUE;
    if( _uart_dev[uart_dev].device.magic != DEVICE_MAGIC )
        return 0 ;
    dev = &_uart_dev[uart_dev].device;
    _uartStatus.HwBusy = FALSE;
    if((uart_dev >= E_UART_PIU_UART0)&&(uart_dev <= E_UART_PIU_FUART0))
    {
        _UartDisc[uart_dev-3].enable = TRUE;
        _UartDisc[uart_dev-3].Uart_dev = uart_dev;
        _UartDisc[uart_dev-3].UartHandle = ((MS_U32)(dev->devio->open((device_t)dev, 0)));
        return _UartDisc[uart_dev-3].UartHandle;
    }
    else
        return ((MS_U32)(dev->devio->open((device_t)dev, 0)));
}

MS_U16 mdrv_uart_close(MS_VIRT hUart)
{
    struct device *dev;
    MS_U8 i = 0;
    MS_VIRT defaulthUart = (MS_VIRT)_hDefaultUart;
    ms_uart_dev_t phUart = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    if (hUart == 0)
    {
        phUart = (ms_uart_dev_t) defaulthUart;
    }
    if (phUart)
    {
        dev = &(phUart->device);
        if (dev->devio->close)
        {
            dev->devio->close((device_t)dev);
        }
        for(i=0;i<UART_MAX;i++)
        {
            if(hUart ==_UartDisc[i].UartHandle)
            {
                _UartDisc[i].enable = FALSE;
                break;
            }
        }
    }
    _uartStatus.HwBusy = FALSE;

    return -1;
}

MS_U16 mdrv_uart_set_rx_callback(MS_VIRT hUart, ms_uart_rx_callback rx_cb)
{
    struct device *dev;
	ms_uart_dev_t phUart = (ms_uart_dev_t) hUart;
    MS_U8 i=0;

    ULOGE(TAG_UART,"[Lwc Debug] %s : %d\n",__FUNCTION__,__LINE__);
    if(_bInitFlag) //bakup rx call back fun
    {
        for(i=0;i<UART_MAX;i++)
        {
            if(_UartDisc[i].UartHandle == hUart)
            {
                _UartDisc[i].rx_cb=rx_cb;
                break;
            }
        }
    }
    _uartStatus.HwBusy = TRUE;
    if (hUart)
    {
        dev =&(phUart->device);
        if (dev->devio->ioctl)
        {
            return dev->devio->ioctl((device_t)dev, IOCTL_SET_RXCALLBACK, (MS_VIRT)&rx_cb);
        }
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_set_rx_callback_halreg(MS_VIRT hUart, ms_uart_rx_callback_halreg rx_cb)
{
    struct device *dev;

	ms_uart_dev_t phUart = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    if (hUart)
    {
        dev = &(phUart->device);
        if (dev->devio->ioctl)
        {
            return dev->devio->ioctl((device_t)dev, IOCTL_SET_RXCALLBACK_HALREG, (MS_VIRT)&rx_cb);
        }
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_set_tx_buffer(MS_VIRT hUart, void *tx_buffer, MS_U16 len)
{
    _uartStatus.HwBusy = TRUE;

	ms_uart_dev_t phUart = (ms_uart_dev_t) hUart;

    if (hUart)
    {
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_LockInterrupt();
#endif
        phUart->tx_buf = tx_buffer;
        phUart->tx_buf_in = 0;
        phUart->tx_buf_out = 0;
        phUart->tx_buf_len = len;
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_UnLockInterrupt();
#endif
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_set_rx_buffer(MS_VIRT hUart, void *rx_buffer, MS_U16 len)
{
    _uartStatus.HwBusy = TRUE;

	ms_uart_dev_t phUart = (ms_uart_dev_t) hUart;

    if (hUart)
    {
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_LockInterrupt();
#endif
        phUart->rx_buf = rx_buffer;
        phUart->rx_buf_in = 0;
        phUart->rx_buf_out = 0;
        phUart->rx_buf_len = len;
#ifdef MSOS_TYPE_NOS
        MsOS_CPU_UnLockInterrupt();
#endif
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_rx_enable(MS_VIRT hUart)
{
	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    if (hUart)
    {
        phUART->rx_flags &= ~RX_FLAG_DISABLED;
        return 0;
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_tx_enable(MS_VIRT hUart)
{
	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    if (hUart)
    {
        phUART->tx_flags &= ~TX_FLAG_DISABLED;
        return 1;
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_rx_disable(MS_VIRT hUart)
{
	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    if (hUart)
    {
        phUART->rx_flags |= RX_FLAG_DISABLED;
        return 1;
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_tx_disable(MS_VIRT hUart)
{
	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    if (hUart)
    {
        phUART->tx_flags |= TX_FLAG_DISABLED;
        return 1;
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_write(MS_VIRT hUart, const MS_U8 *buf, MS_U16 len)
{
    struct device *dev;

	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    dev = &(phUART->device);
    if (dev->devio->write && !( ((ms_uart_dev_t)hUart)->tx_flags & TX_FLAG_DISABLED))
    {
        return dev->devio->write((device_t)dev, (const char *)buf, (MS_U32)len, 0);
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_read(MS_VIRT hUart, MS_U8 *buf, MS_U32 len)
{
    struct device *dev;

	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    dev = &(phUART->device);
    if (dev->devio->read && !( ((ms_uart_dev_t)hUart)->rx_flags & RX_FLAG_DISABLED))
    {
        return dev->devio->read((device_t)dev, (char *)buf, len, 0);
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

MS_U16 mdrv_uart_poll(MS_VIRT hUart, int types)
{
    struct device *dev;

	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    _uartStatus.HwBusy = TRUE;
    dev = &(phUART->device);
    if (dev->devio->poll)
    {
        return dev->devio->poll((device_t)dev, types);
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}


MS_U16 mdrv_uart_set_baudrate(MS_VIRT hUart, MS_U32 baudrate)
{
    struct device *dev;

	ms_uart_dev_t phUART = (ms_uart_dev_t) hUart;

    MS_U8 i=0;

    for(i=0;i<UART_MAX;i++)
    {
        if(hUart == _UartDisc[i].UartHandle)
        {
            _UartDisc[i].baudrate = baudrate;
            break;
        }
    }
    _uartStatus.HwBusy = TRUE;
    if (hUart)
    {
        dev = &(phUART->device);
        if (dev->devio->ioctl)
        {
            return dev->devio->ioctl((device_t)dev, IOCTL_SET_BAUDRATE, (MS_VIRT)&baudrate);
        }
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

UART_DEVICE_TYPE mdrv_uart_get_connection(UART_PORT_TYPE uart_port)
{
    UART_DEVICE_TYPE enUartDeviceType;

    _uartStatus.HwBusy = TRUE;
    enUartDeviceType = HAL_UART_Get_Connection(uart_port);
    _uartStatus.HwBusy = FALSE;

    return enUartDeviceType;
}

MS_U16 mdrv_uart_connect(UART_PORT_TYPE uart_port, UART_DEVICE_TYPE uart_dev)
{
    MS_U16 u16Ret;

    _uartStatus.HwBusy = TRUE;
    u16Ret = HAL_UART_Connect(uart_port, uart_dev);
    _uartStatus.HwBusy = FALSE;
    return u16Ret;
}

MS_U16 mdrv_uart_connect_mux(UART_DEVICE_TYPE uart_dev, UART_PORT_TYPE uart_port, UART_PAD_TYPE uart_pad)
{
	MS_U16 u16Ret = TRUE;

	_uartStatus.HwBusy = TRUE;

	u16Ret &= HAL_UART_Connect(uart_port, uart_dev);
	if (u16Ret == FALSE)
	{
		ULOGE(TAG_UART, "Error: %s, uart_connect fail\n", __FUNCTION__);
	}

	//check uart_port and relative uart_port_pad
	u16Ret &= HAL_UART_SetPadMux(uart_pad);
	if (u16Ret == FALSE)
	{
		ULOGE(TAG_UART, "Error: %s, uart set port pad fail\n", __FUNCTION__);
	}

	_uartStatus.HwBusy = FALSE;

	return u16Ret;
}

MS_U16 mdrv_uart_invert(UART_PORT_TYPE uart_port, MS_BOOL bInv)
{
    MS_U16 u16Ret;

    _uartStatus.HwBusy = TRUE;
    u16Ret = HAL_UART_Invert(uart_port, bInv);
    _uartStatus.HwBusy = FALSE;

    return u16Ret;
}

#if UART_TYPE_EYWA

MS_U8 Eywa_Cal8BitsChecksum(MS_U8 *buf, MS_U32 len)
{
	MS_S32 checkSum = 0;
	MS_U8 i;

	for( i = 0 ; i < len; i++ )
    {
        checkSum += buf[i];
    }

    return (MS_U8)( ( -checkSum ) & 0xFF );
}


void Eywa_UART_RecvHandler(MS_U8 c)
{
	//MS_U8 inx=0;
	//MDrv_WDT_Init((WDT_DbgLv)0);
	//MDrv_TIMER_Count(E_TIMER_0,1);
	//_EywaPacket.payload[inx] = c;
	ULOGD(TAG_UART, "[Eywa]packet.payload = %x\n", c);

	#if 0
	if( c==EYWA_PACKET_ID && _EywaPacket.count == 0 )
	//if( c == EYWA_PACKET_ID )
	{
		_EywaPacket.data_type = E_EYWA_TOUCHPAD;
		_EywaPacket.data_type_inv = E_EYWA_INV_TOUCHPAD;
		_EywaPacket.len = 8;
		_EywaPacket.payload[0] = c;
		_EywaPacket.parity = EYWA_DEFAULT_PARITY;
		_EywaPacket.count = 7;
		ULOGD(TAG_UART, "[Eywa]packet.payload[0] = %x\n", _EywaPacket.payload[0]);
	}
	else
	{
		if( _EywaPacket.count > 0 )
		{
			inx = 8 - _EywaPacket.count;
			_EywaPacket.payload[inx] = c;
			ULOGD(TAG_UART, "[Eywa]packet.payload[%d] = %x\n", inx, _EywaPacket.payload[inx]);
			_EywaPacket.count -= 1;
		}
#if 0
		if(_EywaPacket.payload[7])
		{
			ULOGD(TAG_UART, "[Eywa]packet.data_type = %x\n", _EywaPacket.data_type);
			ULOGD(TAG_UART, "[Eywa]packet.data_type_inv = %x\n", _EywaPacket.data_type_inv);
			ULOGD(TAG_UART, "[Eywa]packet.len = %x\n", _EywaPacket.len);
			for(inx = 0; inx < 8; inx++)
			{
				ULOGD(TAG_UART, "[Eywa]packet.payload[%d] = %x\n", inx, _EywaPacket.payload[inx]);
			}
			ULOGD(TAG_UART, "[Eywa]packet.parity = %x\n", _EywaPacket.parity);
		}
#endif
	}
#endif
	MsOS_EnableInterrupt(E_INT_IRQ_UART1);
	//printf("duration = %d ms\n",MDrv_TIMER_GetMs(E_TIMER_0));

}


void Eywa_UART_Init(void)
{
	MS_U32 g_EywaUART;
      MS_U16 inx = 0;
      MS_U8 p_buf[8];

	ULOGD(TAG_UART, "[Eywa]Uart init\n");

	//second UART mode select PAD_GPIO4/PAD_GPIO3
	*(volatile MS_U32*)(0xbf200000+(0x0F02*4)) |= 0x0300;

	mdrv_uart_connect(E_UART_PORT1, E_UART_PIU_UART1);

	g_EywaUART = mdrv_uart_open(E_UART_PIU_UART1);
	mdrv_uart_set_baudrate(g_EywaUART,57600);
	//mdrv_uart_tx_enable(g_EywaUART);
	//mdrv_uart_rx_enable(g_EywaUART);
	//mdrv_uart_set_rx_buffer(g_EywaUART,p_buf,sizeof(p_buf));
	//mdrv_uart_set_rx_callback(g_EywaUART, Eywa_UART_RecvHandler);

		while(1)
		{
			mdrv_uart_read(g_EywaUART,p_buf,sizeof(p_buf));
			if(p_buf[0]==0x52)
			{
				//_EywaPacket.payload[0] = p_buf;
				for(inx = 0; inx < 8; inx++)
				{
					//mdrv_uart_read(g_EywaUART,p_buf,sizeof(p_buf));
					_EywaPacket.payload[inx] = p_buf[inx];
				}

				if(_EywaPacket.payload[7] == Eywa_Cal8BitsChecksum(_EywaPacket.payload,7))
				{
					_EywaPacket.data_type = E_EYWA_TOUCHPAD;
					_EywaPacket.data_type_inv = E_EYWA_INV_TOUCHPAD;
					_EywaPacket.len = 8;
					_EywaPacket.parity = EYWA_DEFAULT_PARITY;


					ULOGD(TAG_UART, "[Eywa]packet.data_type = %x\n", _EywaPacket.data_type);
					ULOGD(TAG_UART, "[Eywa]packet.data_type_inv = %x\n", _EywaPacket.data_type_inv);
					ULOGD(TAG_UART, "[Eywa]packet.len = %x\n", _EywaPacket.len);
					for(inx = 0; inx < 8; inx++)
		  			{
		  				ULOGD(TAG_UART, "[Eywa]packet.payload[%d] = %x\n", inx, _EywaPacket.payload[inx]);
		  			}
					ULOGD(TAG_UART, "[Eywa]packet.parity = %x\n", _EywaPacket.parity);
				}
				else
				{
					ULOGE(TAG_UART, "[Eywa]packet checksum error!!\n");
				}

			}
		}

}
#endif


//#if !(defined(CHIP_JANUS) && defined (MSOS_TYPE_ECOS))
#if !defined (MSOS_TYPE_ECOS) && !defined (MSOS_TYPE_UCOS)
int putchar(int c)
{
    char x;

    _uartStatus.HwBusy = TRUE;
    if (_hDefaultUart == DEVICE_NULL)
    {
        return -1;
    }
    if (c == '\n')
    {
        x = '\r';
        mdrv_uart_write(_hDefaultUart, (MS_U8 *)&x, 1);
    }
    x = c;
    mdrv_uart_write(_hDefaultUart, (MS_U8 *)&x, 1);
    _uartStatus.HwBusy = FALSE;

    return c;
}
#endif

int kbhit(void)
{
    _uartStatus.HwBusy = TRUE;
    if (_hDefaultUart != DEVICE_NULL)
    {
        return mdrv_uart_poll(_hDefaultUart, POLLIN);
    }
    _uartStatus.HwBusy = FALSE;

    return 0;
}

//#if !(defined(CHIP_JANUS) && defined (MSOS_TYPE_ECOS))
#if !defined (MSOS_TYPE_ECOS) && !defined (MSOS_TYPE_UCOS)
int getchar(void)
{
    char x = 0;

    _uartStatus.HwBusy = TRUE;
    if ((_hDefaultUart != DEVICE_NULL) && (mdrv_uart_read(_hDefaultUart, (MS_U8 *)&x, 1) == 1))
    {
        return (int) (unsigned char) x;
    }
    _uartStatus.HwBusy = FALSE;

    return -1;
}
#endif


MS_U32 MDrv_UART_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_U8 i=0;
    if (u16PowerState == E_POWER_SUSPEND)
    {
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        //default uart re_init
        _bInitFlag = FALSE;
        //disable interrupt
        for(i=0;i<UART_MAX;i++)
        {
            if(_UartDisc[i].enable ==TRUE)
            {
                if(_UartDisc[i].rx_cb != NULL)
                    mdrv_uart_set_rx_callback(_UartDisc[i].UartHandle,NULL);
            }
        }
    }
    else if (u16PowerState == E_POWER_RESUME)
    {

         if (_prev_u16PowerState == E_POWER_SUSPEND)
         {
            MDrv_UART_Init(_eDefaultUart, _u32DefaultUartBaudrate);
            for(i=0;i<UART_MAX;i++)
            {
                if(_UartDisc[i].enable ==TRUE)
                {
                   _UartDisc[i].UartHandle=mdrv_uart_open(_UartDisc[i].Uart_dev);

                   if(_UartDisc[i].UartHandle)
                   {
                        mdrv_uart_set_baudrate(_UartDisc[i].UartHandle, _UartDisc[i].baudrate); // set default baud rate
                        mdrv_uart_set_rx_callback(_UartDisc[i].UartHandle,_UartDisc[i].rx_cb);
                   }
                }
            }
            _prev_u16PowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
             ULOGE(TAG_UART, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
             u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        ULOGE(TAG_UART, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

void MDrv_UART_SetUARTSecurityBank(UART_DEVICE_TYPE uart_dev)
{
    return HAL_UART_SetUARTSecurityBank(uart_dev);
}