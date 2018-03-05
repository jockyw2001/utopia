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

#include "MsCommon.h"
#include "include/drvConfig.h"
#include "include/drvCompiler.h"
#include "include/drvPorts.h"
#include "include/drvIO.h"
#include "include/drvPCIMEM.h"
#include "include/drvList.h"
#include "include/drvTimer.h"
#include "include/drvKernel.h"
// USB related implemented header files
#include "include/drvUSB.h"
#include "drvHCD.h"
#include "include/drvCPE_EHCI.h"

#include "drvEHCI.h"

#ifdef EHCI_VERBOSE_DEBUG

static __inline__ char * edstring(U32 ed_type,char *tmp)
{
  switch (ed_type) {
  case PIPE_CONTROL:
        strcpy(tmp,"ctrl");
        break;
  case PIPE_BULK:
        strcpy(tmp, "bulk");
        break;
  case PIPE_INTERRUPT:
        strcpy(tmp,"intr");
        break;
  default:
        strcpy(tmp,"isoc");
        break;
  };
  return tmp;
}

#define pipestring(pipe, tmp_str) edstring(usb_pipetype(pipe),tmp_str)

/* debug| print the main components of an URB
 * small: 0) header + data packets 1) just header
 */
static void urb_print (struct urb * urb, char * str, int small)
{
  U32 pipe= urb->pipe;
  char tmp_str[10];

  if (!urb->dev || !urb->dev->bus) {
    dbg("%s URB: no dev", str);
    return;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  dbg("[urb_print]%s %p dev=%d ep=%d%s-%s flags=%x",
        str,
        urb,
        usb_pipedevice (pipe),
        usb_pipeendpoint (pipe),
        usb_pipeout (pipe)? "out" : "in",
        pipestring (pipe,tmp_str),
        urb->transfer_flags);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  dbg("[urb_print] len=%d/%d stat=%d",
        urb->actual_length,
        urb->transfer_buffer_length,
        urb->status);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!small) {
    int i, len;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
    if (usb_pipecontrol (pipe)) {
      printk (KERN_DEBUG __func__ ": setup(8):");
      for (i = 0; i < 8 ; i++)
        printk (" %02x", ((unsigned char *) urb->setup_packet) [i]);
      printk ("\n");
    }
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
    if (urb->transfer_buffer_length > 0 && urb->transfer_buffer) {
      printk (KERN_DEBUG __func__ ": data(%d/%d):",
        urb->actual_length,
        urb->transfer_buffer_length);
      len = usb_pipeout (pipe)?
            urb->transfer_buffer_length: urb->actual_length;
      for (i = 0; i < 16 && i < len; i++)
        printk (" %02x", ((unsigned char *) urb->transfer_buffer) [i]);
      printk ("%s stat:%d\n", i < len? "...": "", urb->status);
    }
  }
}

#endif

