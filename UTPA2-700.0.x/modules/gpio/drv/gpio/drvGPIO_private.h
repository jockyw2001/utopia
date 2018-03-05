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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVGPIO_PRIV_H_
#define _DRVGPIO_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @author MStar Semiconductor Inc.
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvGPIO.h"

typedef enum {
    MDrv_CMD_GPIO_Init,
    MDrv_CMD_GPIO_GetStauts,
    MDrv_CMD_GPIO_Set_High,
    MDrv_CMD_GPIO_Set_Low,
    MDrv_CMD_GPIO_Set_Input,
    MDrv_CMD_GPIO_Get_Inout,
    MDrv_CMD_GPIO_Get_Level,
    MDrv_CMD_GPIO_Attach_Interrupt,
    MDrv_CMD_GPIO_Detach_Interrupt,
    MDrv_CMD_GPIO_En_Interrupt,
    MDrv_CMD_GPIO_Dis_Interrupt,
    MDrv_CMD_GPIO_Dis_Interrupt_All,
    MDrv_CMD_GPIO_En_Interrupt_All,
    MDrv_CMD_GPIO_Interrupt_Action,
} eGpioIoctlOpt;


typedef struct _GPIO_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}GPIO_RESOURCE_PRIVATE;

typedef struct _GPIO_GET_STS_PARAM
{
    GPIO_Status *pSts;
}GPIO_GET_STS_PARAM, *PGPIO_GET_STS_PARAM;

typedef struct _GPIO_SET_HIGH_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_SET_HIGH_PARAM, *PGPIO_SET_HIGH_PARAM;

typedef struct _GPIO_SET_LOW_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_SET_LOW_PARAM, *PGPIO_SET_LOW_PARAM;

typedef struct _GPIO_SET_INPUT_PARAM
{
		MS_GPIO_NUM gpio_num;
}GPIO_SET_INPUT_PARAM, *PGPIO_SET_INPUT_PARAM;

typedef struct _GPIO_GET_INOUT_PARAM
{
    MS_U32 u32InOut;
    MS_GPIO_NUM gpio_num;
}GPIO_GET_INOUT_PARAM, *PGPIO_GET_INOUT_PARAM;

typedef struct _GPIO_GET_LEVEL_PARAM
{
    MS_U32 u32Level;
    MS_GPIO_NUM gpio_num;
}GPIO_GET_LEVEL_PARAM, *PGPIO_GET_LEVEL_PARAM;

typedef struct _GPIO_ATTACH_INTERRUPT_PARAM
{
		MS_GPIO_NUM gpio_num;
		GPIO_Edge gpio_edge_type;
		void* GPIO_Callback;
}GPIO_ATTACH_INTERRUPT_PARAM, *PGPIO_ATTACH_INTERRUPT_PARAM;

typedef struct _GPIO_DETACH_INTERRUPT_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_DETACH_INTERRUPT_PARAM, *PGPIO_DETACH_INTERRUPT_PARAM;

typedef struct _GPIO_ENABLE_INTERRUPT_PARAM
{
	MS_GPIO_NUM gpio_num;
}GPIO_ENABLE_INTERRUPT_PARAM, *PGPIO_ENABLE_INTERRUPT_PARAM;

typedef struct _GPIO_DISABLE_INTERRUPT_PARAM
{
	MS_GPIO_NUM gpio_num;
}GPIO_DISABLE_INTERRUPT_PARAM, *PGPIO_DISABLE_INTERRUPT_PARAM;


typedef union _GPIO_PRIVATE_PARAM
{
	 GPIO_GET_STS_PARAM							privatGpioSts;
	 GPIO_SET_HIGH_PARAM						privateGpioSetHigh;
	 GPIO_SET_LOW_PARAM							privateGpioSetLow;
	 GPIO_SET_INPUT_PARAM						privateGpioInput;
	 GPIO_GET_INOUT_PARAM						privateGpioGetInout;
	 GPIO_GET_LEVEL_PARAM						privateGpioGetLevel;
	 GPIO_ATTACH_INTERRUPT_PARAM		privateGpioAttachInterrupt;
	 GPIO_DETACH_INTERRUPT_PARAM		privateGpioDetachInterrupt;
	 GPIO_ENABLE_INTERRUPT_PARAM		privateGpioEnableInterrupt;
	 GPIO_DISABLE_INTERRUPT_PARAM		privateGpioDisableInterrupt;
}GPIO_PRIVATE_PARAM;

typedef void (*IOCTL_GPIO_GET_STS)(GPIO_Status*);
typedef void (*IOCTL_GPIO_INIT)(void);
typedef void (*IOCTL_GPIO_SET_H)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_SET_L)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_SET_INPUT)(MS_GPIO_NUM);
typedef int  (*IOCTL_GPIO_GET_INOUT)(MS_GPIO_NUM);
typedef int  (*IOCTL_GPIO_GET_LEVEL)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_ATTACH_INTERRUPT)(MS_GPIO_NUM,GPIO_Edge,void* GPIO_Callback);
typedef GPIO_Result (*IOCTL_GPIO_DETACH_INTERRUPT)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_ENALBE_INTERRUPT)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_DISABLE_INTERRUPT)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_DISABLE_INTERRUPT_ALL)(void);
typedef void (*IOCTL_GPIO_ENABLE_INTERRUPT_ALL)(void);
typedef void (*IOCTL_GPIO_INTERRUPT_ACTION)(void);

typedef struct _GPIO_INSTANT_PRIVATE
{
	IOCTL_GPIO_GET_STS								fpGpioGetSts;
	IOCTL_GPIO_INIT  									fpGpioInit;
	IOCTL_GPIO_SET_H									fpGpioSetH;
	IOCTL_GPIO_SET_L									fpGpioSetL;
	IOCTL_GPIO_SET_INPUT							fpGpioSetInput;
	IOCTL_GPIO_GET_INOUT							fpGpioGetInout;
	IOCTL_GPIO_GET_LEVEL							fpGpioGetLevel;
	IOCTL_GPIO_ATTACH_INTERRUPT				fpGpioAttachInterrupt;
	IOCTL_GPIO_DETACH_INTERRUPT				fpGpioDetachInterrupt;
	IOCTL_GPIO_DISABLE_INTERRUPT				fpGpioDisableInterrupt;
	IOCTL_GPIO_ENALBE_INTERRUPT				fpGpioEnableInterrupt;
	IOCTL_GPIO_DISABLE_INTERRUPT_ALL		fpGpioDisableInterruptAll;
	IOCTL_GPIO_ENABLE_INTERRUPT_ALL		fpGpioEnableInterruptAll;
	IOCTL_GPIO_INTERRUPT_ACTION				fpGpioInterruptAction;
}GPIO_INSTANT_PRIVATE;

void GPIORegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPIOOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPIOClose(void* pInstance);
MS_U32 GPIOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 GPIOStr(MS_U32 u32PowerState, void* pModule);

void MDrv_GPIO_GetStatus_U2K(GPIO_Status *pStatus);
void mdrv_gpio_init_U2K(void);
void mdrv_gpio_set_high_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_set_low_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_set_input_U2K(MS_GPIO_NUM gpio);
int mdrv_gpio_get_inout_U2K(MS_GPIO_NUM gpio);
int mdrv_gpio_get_level_U2K(MS_GPIO_NUM gpio);
GPIO_Result mdrv_gpio_attach_interrupt_U2K(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback);
GPIO_Result mdrv_gpio_detach_interrupt_U2K(MS_GPIO_NUM gpio_num);
GPIO_Result mdrv_gpio_enable_interrupt_U2K(MS_GPIO_NUM gpio);
GPIO_Result mdrv_gpio_disable_interrupt_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_disable_interrupt_all_U2K(void);
void mdrv_gpio_enable_interrupt_all_U2K(void);
void mdrv_gpio_interrupt_action_U2K(void);

#ifdef __cplusplus
}
#endif
#endif // _DRVGPIO_PRIV_H_
