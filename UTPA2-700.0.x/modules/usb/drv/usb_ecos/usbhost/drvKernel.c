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

#include <MsCommon.h>
#include <cyg/hal/hal_if.h>
#include "include/drvKernel.h"
#include "include/drvTimer.h"
#include "include/drvPorts.h"
#include "include/drvCPE_AMBA.h"

extern int usb_core_init(void);
extern struct bus_type usb_bus_type;

//int CPU_Int_State = INT_DISABLED;

extern void init_cache_memory(void);

S32      g_USBWaitFlg;
S32      g_USBWaitP2Flg;

void init_OS_Resource(void)
{

	g_USBWaitFlg = MsOS_CreateEventGroup("USB_Event");

    //   g_SCSISem = MsOS_CreateSemaphore(1, E_MSOS_FIFO, "USB_Sem");
//      _s32MutexUSB = MsOS_CreateMutex(E_MSOS_FIFO, "USB Mutex");


}

void free_OS_Resource(void)
{
	BOOL err;

	err = MsOS_DeleteEventGroup(g_USBWaitFlg);
	USB_ASSERT((err == TRUE ),"Delete Bulk Flag Fail...\n");

//	err = MsOS_DeleteSemaphore(g_SCSISem);
//        err=MsOS_DeleteMutex(_s32MutexUSB);
//	USB_ASSERT((err == TRUE ),"Delete USB mutex Fail...\n");
}

void init_OS_Port2_Resource(void)
{

	g_USBWaitP2Flg = MsOS_CreateEventGroup("USB_Event");

    //   g_SCSISem = MsOS_CreateSemaphore(1, E_MSOS_FIFO, "USB_Sem");
//      _s32MutexUSB = MsOS_CreateMutex(E_MSOS_FIFO, "USB Mutex");


}

void free_OS_Port2_Resource(void)
{
	BOOL err;

	err = MsOS_DeleteEventGroup(g_USBWaitP2Flg);
	USB_ASSERT((err == TRUE ),"Delete Port2 Bulk Flag Fail...\n");

//	err = MsOS_DeleteSemaphore(g_SCSISem);
//        err=MsOS_DeleteMutex(_s32MutexUSB);
//	USB_ASSERT((err == TRUE ),"Delete USB mutex Fail...\n");
}

void exit_sys(void)
{
	free_OS_Resource();
        free_OS_Port2_Resource();
}

void busyloop_delay(U32 count)
{
   U32 i,j;

   for (i=0; i<255; i++)
      for (j=0; j<count; j++) {
    }
}

#if 0
//The following code has serious bug.  Jonas_20110519
int find_next_zero_bit_le (void *p, int size, int offset)
{
  int i,j;
  int iOffset,jOffset;
  char *tmp = (char *)p;

  iOffset = offset%8;
  jOffset = offset/8;

  // Skip offset bits
  for(i=iOffset;i<8;i++)
    tmp[i] >>= 1;

  for(j=jOffset;j<size/8;j++)
  {
    for(i=iOffset;i<8;i++)
    {
      if ( ( tmp[j] & 0x01 ) == 0 )
      {
        return j*8+i;
      }
      tmp[j] >>= 1;
    }
  }
  // not found return (maximum size) back
  return size;
}

#else

#define BITOP_WORD(nr)		((nr) / BITS_PER_LONG)

U32 find_next_zero_bit(const U32 *addr, U32 size, U32 offset)
{
	const U32 *p = addr + BITOP_WORD(offset);
	U32 result = offset & ~(BITS_PER_LONG-1);
	U32 tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_LONG;
	if (offset) {
		tmp = *(p++);
		tmp |= ~0UL >> (BITS_PER_LONG - offset);
		if (size < BITS_PER_LONG)
			goto found_first;
		if (~tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	while (size & ~(BITS_PER_LONG-1)) {
		if (~(tmp = *(p++)))
			goto found_middle;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp |= ~0UL << size;
	if (tmp == ~0UL)	
		return result + size;	
found_middle:
	return result + ffz(tmp);
}


U32 find_next_zero_bit_le(U32 *addr, U32 size, U32 offset)
{
	return find_next_zero_bit(addr, size, offset);
}

#endif

void udelay(U32 us)
{
   HAL_DELAY_US(us);
}

void init_sys(void)
{
    #if 1
	init_cache_memory();
	INIT_LIST_HEAD(&usb_bus_type.devices_list);
	INIT_LIST_HEAD(&usb_bus_type.drivers_list);
	INIT_LIST_HEAD(&CPE_AMBA_bus_type.devices_list);
	INIT_LIST_HEAD(&CPE_AMBA_bus_type.drivers_list);
    #endif

	sti();

    #if  1
	// Init timer and init software timer link list
	init_timers();
    #endif

    #if 1
	// Init USB core (including hub)
	usb_core_init();
    #endif
}

void wait_ms(int x)
{
    if (x==0) return;
    MsOS_DelayTask(x);
}

void init_OS_Resource_EX(S32 *USBWaitFlg)
{
    *USBWaitFlg = MsOS_CreateEventGroup("USB_Event");
}

