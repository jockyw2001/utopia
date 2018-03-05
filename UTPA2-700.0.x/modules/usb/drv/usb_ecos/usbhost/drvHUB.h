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

#ifndef _CORE_HUB_H_
#define _CORE_HUB_H_
#include <MsTypes.h>

#include "include/drvList.h"
#include "include/drvCompiler.h"
#include "include/drvUSB.h"
#include "include/drvCPE_AMBA.h"
#include "drvUsbHostConfig.h"

#define USB_RT_HUB  (USB_TYPE_CLASS | USB_RECIP_DEVICE)
#define USB_RT_PORT  (USB_TYPE_CLASS | USB_RECIP_OTHER)
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#define HUB_CLEAR_TT_BUFFER  8
#define HUB_RESET_TT    9
#define HUB_GET_TT_STATE  10
#define HUB_STOP_TT    11

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define C_HUB_LOCAL_POWER  0
#define C_HUB_OVER_CURRENT  1

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_PORT_FEAT_CONNECTION  0
#define USB_PORT_FEAT_ENABLE    1
#define USB_PORT_FEAT_SUSPEND    2
#define USB_PORT_FEAT_OVER_CURRENT  3
#define USB_PORT_FEAT_RESET    4
#define USB_PORT_FEAT_POWER    8
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_PORT_FEAT_LOWSPEED    9
#define USB_PORT_FEAT_HIGHSPEED    10
#define USB_PORT_FEAT_C_CONNECTION  16
#define USB_PORT_FEAT_C_ENABLE    17
#define USB_PORT_FEAT_C_SUSPEND    18
#define USB_PORT_FEAT_C_OVER_CURRENT  19
#define USB_PORT_FEAT_C_RESET    20
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_PORT_FEAT_TEST              21 // ??? 11
#define USB_PORT_FEAT_INDICATOR         22 // ??? 12

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_PORT_STAT_CONNECTION  0x0001
#define USB_PORT_STAT_ENABLE    0x0002
#define USB_PORT_STAT_SUSPEND    0x0004
#define USB_PORT_STAT_OVERCURRENT  0x0008
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_PORT_STAT_RESET    0x0010
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_PORT_STAT_POWER    0x0100
#define USB_PORT_STAT_LOW_SPEED    0x0200
#define USB_PORT_STAT_HIGH_SPEED        0x0400
#define USB_PORT_STAT_TEST              0x0800
#define USB_PORT_STAT_INDICATOR         0x1000

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_PORT_STAT_C_CONNECTION  0x0001
#define USB_PORT_STAT_C_ENABLE    0x0002
#define USB_PORT_STAT_C_SUSPEND    0x0004
#define USB_PORT_STAT_C_OVERCURRENT  0x0008
#define USB_PORT_STAT_C_RESET    0x0010

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define HUB_CHAR_LPSM    0x0003 
#define HUB_CHAR_COMPOUND  0x0004 
#define HUB_CHAR_OCPM    0x0018 
#define HUB_CHAR_TTTT           0x0060 
#define HUB_CHAR_PORTIND        0x0080 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
struct usb_port_status {
  U16 wPortStatus;
  U16 wPortChange;
} __attribute__((packed));

struct usb_hub_status {
  U16 wHubStatus;
  U16 wHubChange;
} __attribute__((packed));

#define HUB_STATUS_LOCAL_POWER  0x0001
#define HUB_STATUS_OVERCURRENT  0x0002
#define HUB_CHANGE_LOCAL_POWER  0x0001
#define HUB_CHANGE_OVERCURRENT  0x0002


#define USB_DT_HUB      (USB_TYPE_CLASS | 0x09)
#define USB_DT_HUB_NONVAR_SIZE    7

struct usb_hub_descriptor {
  unsigned char  bDescLength;
  unsigned char  bDescriptorType;
  unsigned char  bNbrPorts;
  U16 wHubCharacteristics;
  unsigned char  bPwrOn2PwrGood;
  unsigned char  bHubContrCurrent;
  unsigned char  DeviceRemovable[(USB_MAXCHILDREN + 1 + 7) / 8 + 1]; // add 1 to prevent warning message
  unsigned char  PortPwrCtrlMask[(USB_MAXCHILDREN + 1 + 7) / 8 + 1];
} __attribute__((packed));

struct usb_device;

struct usb_tt {
  struct usb_device  *hub;  
  int      multi;  

  spinlock_t    lock;
  struct list_head  clear_list;  
  //struct work_struct      kevent;
};

struct usb_tt_clear {
  struct list_head  clear_list;
  U32    tt;
  U16      devinfo;
};

extern void usb_hub_tt_clear_buffer (struct usb_device *dev, int pipe);

struct usb_hub {
  struct usb_interface  *intf;    
  struct urb    *urb;    

  char      (*buffer)[(USB_MAXCHILDREN + 1 + 7) / 8];
  dma_addr_t    buffer_dma;  
  union {
    struct usb_hub_status  hub;
    struct usb_port_status  port;
  }status;  

  int      error;    
  int      nerrors;  

  struct list_head  hub_list;  
  struct list_head  event_list;  

  struct usb_hub_descriptor descriptor;  
  //struct semaphore  khubd_sem;
  struct usb_tt    tt;    
};

extern void hub_poll(void);
extern BOOL has_hub_events(U16 *pPortStatus);
#define HUB_RESET_TRIES    5
#define HUB_PROBE_TRIES    2
#define HUB_ROOT_RESET_TIME  50  
#define HUB_SHORT_RESET_TIME  10
#define HUB_LONG_RESET_TIME  200
#define HUB_RESET_TIMEOUT  500
#define HUB_DEBOUNCE_STEP      25
#define HUB_DEBOUNCE_STABLE   4 // # of counts

struct s_vPortRange
{
    U32 vPortStart;
    U32 vPortEnd;
};

struct s_cpe_dev_info
{
    U32 devid;
    char devname[24];
};

struct s_gVar4UsbPort
{
char name[24];
MS_U8 portNum;
struct s_vPortRange vPortRange;
char bus_name[24];
char product_desc[24];
struct s_cpe_dev_info cpe_dev_info;
#ifndef  MS_NOSAPI
MS_U8 *u8pHubStackBuffer;
#endif

struct cpe_dev cpe_ehci_dev;

struct list_head *p_hub_event; // for UsbTask()
struct usb_hcd *p_UsbHcd; // for MDrv_UsbDeviceConnect()
};
static __inline__ struct s_gVar4UsbPort *cpe_to_gVar(struct cpe_dev *cpe_ptr)
{
  const struct cpe_dev *__mptr = (cpe_ptr);
  return (struct s_gVar4UsbPort *)( (char *)__mptr - (char *)offsetof(struct s_gVar4UsbPort, cpe_ehci_dev) );
}

struct s_RegMap
{
    MS_U32 iFlag;
    MS_U32 baseUTMI;
    MS_U32 baseUHC;
    MS_U32 baseUSBC;
    MS_U8 uhcIRQ;
    MS_U8 usbcIRQ;
};

struct s_UsbcInfo
{
    MS_U8 portNum;
    MS_U8 int_pol;
    MS_U16 intEn;
    MS_U16 intSts;
    MS_U8 eventFlag;
    MS_U8 eventType;
};

struct s_ChipUsbHostDef
{
    MS_U8 chipID;
    char name[24];
    MS_U8 nRootHub;
    struct s_RegMap reg[NUM_OF_ROOT_HUB];
    //struct cpe_dev *p_cpe_ehci_dev[NUM_OF_ROOT_HUB];    
    struct s_gVar4UsbPort *p_roothub[NUM_OF_ROOT_HUB];
    struct s_UsbcInfo usbc_ip[NUM_OF_ROOT_HUB];
};


#endif /* __LINUX_HUB_H */

