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

//#include <MsCommon.h> // NUSED

//#include  "include/drvPorts.h" // NUSED
#include  "include/drvConfig.h"
//#include  "include/drvCompiler.h" // NUSED
//#include  "include/drvErrno.h" // NUSED
//#include "include/drvTimer.h" // NUSED
//#include  "include/drvList.h" // NUSED
#include  "include/drvKernel.h"
#include  "include/drvBitops.h"
//#include "include/drvCPE_AMBA.h" // NUSED

// USB related header files
//#include "include/drvUSBHost.h" // NUSED
#include "drvUSBCore.h"
#include "drvUsbd.h"
#include "drvUSBHwCtl.h"
#include "drvEHCI.h"
//#include "drvUSB.h" // NUSED
/* applying drvUsbHostConfig.h (inside drvUsbd.h <- drvHub.h) */

extern int  ms_usb_hub_init(void);
extern int  ms_usb_hub_uninit(void);
//extern void ms_qh_ehci_reinit(struct usb_hcd *pHcd, int fix);

//int USB_core_lock=0; // NUSED
//int Timer_task_lock=0; // NUSED

char usb_product_string_buf[64];
char usb_manufacturer_string_buf[64];
char usb_serialnumber_string_buf[64];

static int ms_generic_probe (struct device_s *pDev)
{
    return 0;
}
static int ms_generic_remove (struct device_s *pDev)
{
    return 0;
}

static struct device_driver ms_usb_generic_driver = {
    "usb_gdrv",
    &usb_bus_type,
    {NULL,NULL},
    {&ms_usb_generic_driver.devices,&ms_usb_generic_driver.devices},
    ms_generic_probe,
    ms_generic_remove,
};

static int usb_generic_driver_data;
#if 0 // NUSED
void ms_usb_show_string(struct usb_device *pDev, char *pStringId, int iIdx)
{
    char *pCbuf;

    if (!iIdx)
        return;

    if (!(pCbuf = (char*) kmalloc(256, GFP_KERNEL)))
        return;

    if (ms_usb_string(pDev, iIdx, pCbuf, 256) > 0)
    {
        diag_printf("<%s> %s: %s\n",__FUNCTION__, pStringId, pCbuf);
    }
    kfree(pCbuf);
}
#endif
const struct usb_device_id *
ms_usb_match_id(struct usb_interface *pUintf, const struct usb_device_id *pDevId)
{
    struct usb_host_interface *ms_intf;
    struct usb_device *pUdev;

    if (pDevId == NULL)
        return NULL;

    ms_intf = &pUintf->altsetting [pUintf->act_altsetting];
    pUdev = interface_to_usbdev(pUintf);

    for (; pDevId->idVendor || pDevId->bDeviceClass || pDevId->bInterfaceClass ||
         pDevId->match_id_info; pDevId++)
    {
        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_VENDOR) &&
            pDevId->idVendor != pUdev->descriptor.idVendor)
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_PRODUCT) &&
            pDevId->idProduct != pUdev->descriptor.idProduct)
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_DEV_LO) &&
            (pDevId->bcdDevice_lo >= pUdev->descriptor.bcdDevice))
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_DEV_HI) &&
            (pDevId->bcdDevice_hi <= pUdev->descriptor.bcdDevice))
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_DEV_CLASS) &&
            (pDevId->bDeviceClass != pUdev->descriptor.bDeviceClass))
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_DEV_SUBCLASS) &&
            (pDevId->bDeviceSubClass!= pUdev->descriptor.bDeviceSubClass))
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_DEV_PROTOCOL) &&
            (pDevId->bDeviceProtocol != pUdev->descriptor.bDeviceProtocol))
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_INT_CLASS) &&
            (pDevId->bInterfaceClass != ms_intf->desc.bInterfaceClass))
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_INT_SUBCLASS) &&
            (pDevId->bInterfaceSubClass != ms_intf->desc.bInterfaceSubClass))
            continue;

        if ((pDevId->match_id_flags & USBDEV_MATCH_ID_INT_PROTOCOL) &&
            (pDevId->bInterfaceProtocol != ms_intf->desc.bInterfaceProtocol))
            continue;

        return pDevId;
    }

    return NULL;
}

int ms_usb_probe_interface(struct device_s *pDev)
{
    struct usb_interface * pUintf = get_usb_interface(pDev);
    struct usb_driver * pUdriver = get_usb_driver(pDev->driver);
    const struct usb_device_id *pUdev_id;
    int s32Error = -ENODEV;

    if (!pUdriver->probe_func)
        return s32Error;

    pUdev_id = ms_usb_match_id (pUintf, pUdriver->match_id_table);
    if (pUdev_id)
    {
        ms_usbhost_debug ("<%s> - got id\n", __FUNCTION__);
        s32Error = pUdriver->probe_func (pUintf, pUdev_id);
    }
    if (!s32Error)
        pUintf->driver = pUdriver;

    return s32Error;
}

void ms_usb_driver_release_interface(struct usb_driver *pUdriver, struct usb_interface *iface)
{
    // this should never happen, don't release something that's not ours
    if (iface->driver && iface->driver != pUdriver)
        return;

    #if 0 // ???
    iface->driver = NULL;
    ms_usb_set_interface(interface_to_usbdev(iface),
        iface->altsetting[0].desc.bInterfaceNumber,
        0);
    #endif
    ms_usb_set_intfdata(iface, NULL);
    iface->driver = NULL;
}

int ms_usb_unbind_interface(struct device_s *pDev)
{
    struct usb_interface *pUintf = get_usb_interface(pDev);
    struct usb_driver *pUdriver = get_usb_driver(pDev->driver);

    ms_usb_disable_interface(interface_to_usbdev(pUintf), pUintf);

    if (pUintf->driver && pUintf->driver->discon_func)
        pUintf->driver->discon_func(pUintf);

    ms_usb_driver_release_interface(pUdriver, pUintf);

    return 0;
}

int ms_usb_register(struct usb_driver *n_driver)
{
    int s32Retval = 0;

    memcpy(n_driver->driver.name, n_driver->name, sizeof(n_driver->name));
    n_driver->driver.bus = &usb_bus_type;
    n_driver->driver.probe = ms_usb_probe_interface;
    n_driver->driver.remove = ms_usb_unbind_interface;

    s32Retval = ms_driver_register(&n_driver->driver);

    if (!s32Retval)
    {
        ms_usbhost_debug("regtered new driver %s", n_driver->name);
    }
    else
    {
        ms_usbhost_err("problem %d when registering driver %s",
            s32Retval, n_driver->name);
    }

    return s32Retval;
}

void ms_usb_deregister(struct usb_driver *pUdriver)
{
    ms_usbhost_msg("deregistering driver %s\n", pUdriver->name);
    ms_driver_unregister (&pUdriver->driver);
}

struct usb_interface *ms_usb_ifnum_to_if(struct usb_device *pUdev, int ifnum)
{
    int i;

    for (i = 0; i < pUdev->actconfig->desc.bNumInterfaces; i++)
    if (pUdev->actconfig->interface[i]->altsetting[0]
        .desc.bInterfaceNumber == ifnum)
      return pUdev->actconfig->interface[i];

    return NULL;
}
#if 0 // NUSED
BOOL ms_usb_find_dev_for_driver(struct device_driver *pDriver, struct usb_device **pudev, struct usb_hcd *hcd)
{
    struct list_head *entry;
    struct device_s *pDev;

    list_for_loop(entry, &pDriver->devices)
    {
        const struct list_head *_mptr = entry;
        pDev = (struct device_s *) ( (char *)_mptr - (char *)offsetof(struct device_s, driver_list) );

        *pudev = (struct usb_device *) ((char *)pDev - (char *)offsetof( struct usb_device, dev));
        if ((*pudev)->parent == NULL) // skip root hub, 2012.02.29
            continue;

        if ((*pudev)->bus->hcpriv == hcd)
            return TRUE;
    }
    diag_printf(" \n\nusb_find_dev_for_driver> NOT FOUND!!!\n\n");
    return FALSE;
}
#endif
//NSTL_060406 Sony MSAC-US5 MS Reader
BOOL ms_usb_get_connected_dev_state(int *pdevstate,
        unsigned char *pDevClass, struct usb_device *pusbdev, BOOL *pIntfDrvMatched)
{
    BOOL status = FALSE;
    struct usb_interface *pusbintf = NULL;
    *pIntfDrvMatched = FALSE;

    if (!pusbdev)
        return status;

    status = TRUE;
    *pdevstate = pusbdev->eState;
    if (pusbdev->eState < USB_STATE_ADDRESS)
        return status;

    if (pusbdev->descriptor.bDeviceClass)
    {
        *pDevClass = pusbdev->descriptor.bDeviceClass;
    }
    else if (pusbdev->actconfig && pusbdev->actconfig->desc.bNumInterfaces)
    {
        pusbintf = ms_usb_ifnum_to_if(pusbdev, 0);

        if(pusbintf)
        {
            *pDevClass = pusbintf->altsetting->desc.bInterfaceClass;
            *pIntfDrvMatched = (pusbintf->driver != NULL) ? TRUE : FALSE;
        }
    }

    return status;
}

static int ms_usb_device_match (struct device_s *pDev, struct device_driver *pDrv)
{
    struct usb_interface *pUintf;
    struct usb_driver *usb_drv;
    const struct usb_device_id *pUdev_id;

    /* check for generic driver, which we don't match any device with */
    if (pDrv == &ms_usb_generic_driver)
        return 0;

    pUintf = get_usb_interface(pDev);
    usb_drv = get_usb_driver(pDrv);
    pUdev_id = usb_drv->match_id_table;

    pUdev_id = ms_usb_match_id (pUintf, usb_drv->match_id_table);
    if (pUdev_id)
        return 1; //found id

    return 0; //id not found
}

void ms_usb_release_dev(struct device_s *pDev)
{
    struct usb_device *pUdev;
    int retv;

    pUdev = ms_to_usb_device(pDev);

    //if (pUdev->bus && pUdev->bus->bus_ops && pUdev->bus->bus_ops->ms_deallo_hcd_dev)
    //    pUdev->bus->bus_ops->ms_deallo_hcd_dev(pUdev);
    if ((retv = ms_hcd_free_dev(pUdev)) != ENOERR) {
        ms_usbhost_err("free device error %d\n", retv);
    }
    ms_usb_destroy_config(pUdev);
    kfree (pUdev);
}

struct usb_device *ms_usb_alloc_dev(struct usb_device *parent, struct usb_bus *bus)
{
    struct usb_device *pUdev;
    int retv;

    pUdev = (struct usb_device*) kmalloc(sizeof(*pUdev), GFP_KERNEL);
    if (!pUdev)
        return NULL;

    memset(pUdev, 0, sizeof(*pUdev));

    ms_device_initialize(&pUdev->dev);
    pUdev->dev.release_func = ms_usb_release_dev;
    /* the only way to escape from USB_STATE_NOTATTACHED */
    pUdev->eState = USB_STATE_ATTACHED;

    if (!parent)
        pUdev->devpath [0] = '0';
    pUdev->bus = bus;
    pUdev->parent = parent;

    //if (pUdev->bus->bus_ops->ms_alloc_hcd_dev)
    //    pUdev->bus->bus_ops->ms_alloc_hcd_dev(pUdev);
    if ((retv = ms_hcd_alloc_dev(pUdev)) != ENOERR) {
        ms_usbhost_err("alloc device error %d\n", retv);
    }

    return pUdev;
}

struct usb_device *ms_usb_get_dev (struct usb_device *pUdev)
{
    struct device_s *pDev_t;

    if (!pUdev)
        return NULL;

    pDev_t = ms_get_device(&pUdev->dev);
    if (pDev_t)
        return ms_to_usb_device(pDev_t);
    else
        return NULL;
}

void ms_usb_put_dev(struct usb_device *pUdev)
{
    if (pUdev)
    {
        if (osapi_atomic_dec_and_test(&((&pUdev->dev)->dev_refcnt)))
        {
            //release allocated buffer for saving device's configurations and device info
            ms_device_unregister(&pUdev->dev);
        }
    }
}

void ms_usb_disconnect(struct usb_device **pdev)
{
    struct usb_device  *pUdev = *pdev;
    //struct usb_bus    *pUbus; // NUSED
    int      i, devnum = 0;
    struct usb_hcd *pHcd = (struct usb_hcd*) pUdev->bus->hcpriv;

    if (!pUdev)
    {
        ms_usbhost_debug ("%s nodev\n", __FUNCTION__);
        return;
    }
    //pUbus = pUdev->bus;
    //if (!pUbus)
    //{
    //    ms_usbhost_debug ("%s nobus\n", __FUNCTION__);
    //    return;
    //}

    //*pdev = NULL; // moved

    //pUdev->eState = USB_STATE_NOTATTACHED;
    ms_usb_set_device_state(pUdev, USB_STATE_NOTATTACHED); // patch from Linux 2.6.28

    ms_usbhost_msg ("USB disconnect, address %d\n", pUdev->u32DevNum);
    if (pUdev->parent)
        devnum = pUdev->parent->u32DevNum;

    /* If EHC halted while roothub dev disconnect, restart EHC by set
     * its run bit. The periodic schedule urb reclaiming will reference
     * the frame index, so we should not halt the EHC while disconnect.
     */
    if (devnum == 1)
    {
        /* Make sure the EHC is halted already */
        ms_forceHcdRun(pHcd);
    }

    /* Free up all the children before we remove this device */
    for (i = 0; i < USB_MAXCHILDREN; i++)
    {
        struct usb_device **child = pUdev->children + i;
        if (*child)
            ms_usb_disconnect(child);
    }

    /* the following code merged into ms_usb_disable_device()
     * based on Linux USB driver
     */
    //ms_usbhost_debug("%s - unregistering interface %s\n", pUdev->dev.dev_name, pUdev->dev.bus_id);
    //
    //if (pUdev->actconfig)
    //{
    //    for (i = 0; i < pUdev->actconfig->desc.bNumInterfaces; i++)
    //    {
    //        struct usb_interface  *pUiface;
    //
    //        /* remove this interface */
    //        pUiface = pUdev->actconfig->interface[i];
    //        ms_device_unregister(&pUiface->dev);
    //    }
    //}

    /* deallocate hcd/hardware state ... and nuke all pending urbs */
    ms_usb_disable_device(pUdev, 0);
    /* synchronize unlinks, may this works only in SMP case */
    osapi_spin_lock_irq(&hcd_urb_unlink_lock);
    osapi_spin_unlock_irq(&hcd_urb_unlink_lock);

    /* patch code sequence by Linux */
    ms_usbhost_debug("unregistering device\n");
    ms_device_unregister(&pUdev->dev);

    /* Free the device number and remove the /proc/bus/usb entry */
    if (pUdev->u32DevNum > 0)
    {
        //ms_clear_bit(pUdev->u32DevNum, pUdev->bus->devmap.usb_devicemap, unsigned long);
        ms_devmap_clear_bit(pUdev->u32DevNum, pUdev->bus->devmap.usb_devicemap);
	pUdev->u32DevNum = -1;
    }

    osapi_spin_lock_irq(&device_state_lock);
    *pdev = NULL;
    osapi_spin_unlock_irq(&device_state_lock);

    #ifdef ENABLE_ROOTHUB_DISCONN_REINIT // new patch to release qh and recover async qh
    if (devnum == 1) // root hub is always assigned address 1
    {
	ms_roothub_disconn_reinit(pHcd);
    }
    #endif
}

void ms_usb_choose_address(struct usb_device *pUdev)
{
    int s32Devnum;
    // TODO:  FIXME needs locking for SMP!!

    s32Devnum = find_next_zero_bit(pUdev->bus->devmap.usb_devicemap, 128, pUdev->bus->devnum_next);
    if (s32Devnum >= 128)
        s32Devnum = find_next_zero_bit(pUdev->bus->devmap.usb_devicemap, 128, 1);

    pUdev->bus->devnum_next = ( s32Devnum >= 127 ? 1 : s32Devnum + 1);

    if (s32Devnum < 128)
    {
        //ms_set_bit(s32Devnum, pUdev->bus->devmap.usb_devicemap, unsigned long);
        ms_devmap_set_bit(s32Devnum, pUdev->bus->devmap.usb_devicemap);
        pUdev->u32DevNum = s32Devnum;
    }
}

int ms_usb_set_address(struct usb_device *pUdev)
{
    int s32Retval;

    if (pUdev->u32DevNum == 0)
        return -EINVAL;
    if ((pUdev->eState != USB_STATE_DEFAULT) && (pUdev->eState != USB_STATE_ADDRESS))
        return -EINVAL;

    s32Retval = ms_usb_control_cmd(pUdev, usb_snddefctrl(pUdev), USB_REQ_SET_ADDRESS,
                    0, pUdev->u32DevNum, 0, NULL, 0, HZ * USB_CTRL_SET_TIMEOUT);

    if (s32Retval == 0)
        //pUdev->eState = USB_STATE_ADDRESS;
        ms_usb_set_device_state(pUdev, USB_STATE_ADDRESS);
    return s32Retval;
}

#if USB_IF_EHSET_SUPPORT
extern int ms_usb_test_proc_vid(U16 vid, U16 pid);
extern void ms_usb_high_speed_test_proc(struct usb_device *udev, int flag);
#endif
#define NEW_DEVICE_RETRYS  2
#define SET_ADDRESS_RETRYS  2

int ms_usb_new_device(struct usb_device *pUdev, struct device_s *parent)
{
    static U8 u8MaxPacketSize = 8;
    int s32Err = -EINVAL;
    int i;
    int j;

    //diag_printf("usb_new_device\n");

    ms_usb_generic_driver.bus = &usb_bus_type;
    pUdev->dev.parent = parent;
    pUdev->dev.driver = &ms_usb_generic_driver;
    pUdev->dev.bus_type = &usb_bus_type;
    pUdev->dev.driver_data = &usb_generic_driver_data;
    strcpy(pUdev->dev.dev_name,"usbdev");

    ms_usb_get_dev(pUdev);

    if (pUdev->dev.bus_id[0] == 0)
        USB_sprintf(&pUdev->dev.bus_id[0], "%d-%s", (int)pUdev->bus->busnum, pUdev->devpath);
    pUdev->dev.dma_mask = parent->dma_mask;

    switch (pUdev->eSpeed)
    {
        case USB_HIGH_SPEED:
            i = 64;
            break;
        case USB_FULL_SPEED:
            i = u8MaxPacketSize;
            break;
        case USB_LOW_SPEED:
            i = 8;
            break;

        default:
            goto fail;
    }
    pUdev->pEpMaxPacketIn [0] = i;
    pUdev->pEpMaxPacketOut[0] = i;


    for (i = 0; i < NEW_DEVICE_RETRYS; ++i)
    {
        for (j = 0; j < SET_ADDRESS_RETRYS; ++j)
        {
            s32Err = ms_usb_set_address(pUdev);
            if (s32Err >= 0)
                break;

            diag_printf("Device state = %s, (i,j) = (%d, %d)\n",
                pUdev->eState == 3 ? "default" :
                pUdev->eState == 4 ? "address": "unknown", i, j);
            //if (0) // unpatch from Linux
            //{
            //    struct usb_hcd *hcd = pUdev->bus->hcpriv;
            //    if (hcd->isBadDevice)
            //    {
            //        diag_printf("  @ usb_new_device(): set address fail to set gIsBadDevice=1\n");
            //        break;
            //    }
            //}

            wait_ms(200);
        }

        if (s32Err < 0) {
          ms_usbhost_err("device not accepting address %d, error %d\n", (int)pUdev->u32DevNum, s32Err);
          goto fail;
        }

        wait_ms(10);  /* Let the SET_ADDRESS settle */
        s32Err = ms_usb_get_descriptor(pUdev, USB_DT_DEVICE, 0, &pUdev->descriptor, 8);
        if (s32Err >= 8 || s32Err == -ENODEV) //Received 8 byress?
            break;
        wait_ms(100);
    }

    if (s32Err < 8)
    {
        ms_usbhost_err("device descriptor read/8, error %d\n", s32Err);
        goto fail;
    }

    if (pUdev->eSpeed == USB_FULL_SPEED)
    {
        pUdev->pEpMaxPacketIn [0] = pUdev->descriptor.bMaxPacketSize0;
        pUdev->pEpMaxPacketOut[0] = pUdev->descriptor.bMaxPacketSize0;
        u8MaxPacketSize = pUdev->descriptor.bMaxPacketSize0;
    }

    s32Err = ms_usb_get_dev_descriptor(pUdev);
    if (s32Err < (signed)sizeof(pUdev->descriptor))
    {
        ms_usbhost_err("device descriptor read/all, error %d\n", s32Err);
        goto fail;
    }

    // annouce the VID/PID to the world
    if (pUdev->parent) // not root hub
        diag_printf("usb new device VID = %4x, PID = %4x\n", pUdev->descriptor.idVendor, pUdev->descriptor.idProduct);

#if USB_IF_EHSET_SUPPORT // embedded host electrical test procedure
      if (pUdev->parent ) // not root hub
      {
          //dev->descriptor.idVendor = 0x1A0A; // test only, marked for fomal release
          //dev->descriptor.idProduct = 0x0102;
          //diag_printf("<usb_new_device> force VID = %x, PID = %x\n", pUdev->descriptor.idVendor, pUdev->descriptor.idProduct);
          if (ms_usb_test_proc_vid(pUdev->descriptor.idVendor, pUdev->descriptor.idProduct) == 1)
              ms_usb_high_speed_test_proc(pUdev, 1);
      }
#endif

    s32Err = ms_usb_get_config(pUdev);
    if (s32Err < 0)
    {
        ms_usbhost_err("unable to get device %d configuration (error=%d)\n",
            (int)pUdev->u32DevNum, s32Err);
        goto fail;
    }

    if (pUdev->descriptor.bNumConfigurations != 1)
    {
        diag_printf("configuration #%d chosen from %d choices\n",
            pUdev->config[0].desc.bConfigurationValue,
            pUdev->descriptor.bNumConfigurations);
    }
    // Add string
    if (pUdev->descriptor.iManufacturer)
    {
        int retv;

        retv = ms_usb_string(pUdev, pUdev->descriptor.iManufacturer, usb_manufacturer_string_buf, 64);
        if (retv > 0)
            pUdev->pManufacturerString = usb_manufacturer_string_buf;
        else
            pUdev->pManufacturerString = NULL;
    }
    else
        pUdev->pManufacturerString = NULL;
    if (pUdev->pManufacturerString)
        ms_usbhost_msg("USB device manufacturer string (index %d) = %s\n", pUdev->descriptor.iManufacturer, pUdev->pManufacturerString);
    if (pUdev->descriptor.iProduct)
    {
        int retv;

        retv = ms_usb_string(pUdev, pUdev->descriptor.iProduct, usb_product_string_buf, 64);
        if (retv > 0)
            pUdev->pProductString = usb_product_string_buf;
        else
            pUdev->pProductString = NULL;
    }
    else
        pUdev->pProductString = NULL;
    if (pUdev->pProductString)
        ms_usbhost_msg("USB device product string (index %d) = %s\n", pUdev->descriptor.iProduct, pUdev->pProductString);
    if (pUdev->descriptor.iSerialNumber)
    {
        int retv;

        retv = ms_usb_string(pUdev, pUdev->descriptor.iSerialNumber, usb_serialnumber_string_buf, 64);
        if (retv > 0)
            pUdev->pSerialNumberString = usb_serialnumber_string_buf;
        else
            pUdev->pSerialNumberString = NULL;
    }
    else
        pUdev->pSerialNumberString = NULL;
    if (pUdev->pSerialNumberString)
        ms_usbhost_msg("USB device serial number string (index %d) = %s\n", pUdev->descriptor.iSerialNumber, pUdev->pSerialNumberString);

    //if(pUdev->descriptor.bDeviceClass == USB_CLASS_COMM)
    //    s32Err = ms_usb_set_config(pUdev, pUdev->config[1].desc.bConfigurationValue);
    //else
        s32Err = ms_usb_set_config(pUdev, pUdev->config[0].desc.bConfigurationValue);

    if (s32Err)
    {
        ms_usbhost_err("failed to set device %d default configuration (error=%d)\n",
            (int)pUdev->u32DevNum, s32Err);
        goto fail;
    }

#if USB_IF_EHSET_SUPPORT // embedded host electrical test procedure
    if (pUdev->parent ) // not root hub
    {
        if (ms_usb_test_proc_vid(pUdev->descriptor.idVendor, pUdev->descriptor.idProduct) == 2)
            ms_usb_high_speed_test_proc(pUdev, 2);
    }
#endif

    s32Err = ms_device_add (&pUdev->dev);
    if (s32Err)
        goto fail;

    for (i = 0; i < pUdev->actconfig->desc.bNumInterfaces; i++)
    {
        struct usb_interface *iface = pUdev->actconfig->interface[i];
        struct usb_interface_descriptor *i_desc;

        i_desc = &iface->altsetting [iface->act_altsetting].desc;
        iface->dev.parent = &pUdev->dev;
        iface->dev.driver = NULL;
        iface->dev.bus_type = &usb_bus_type;
        iface->dev.dma_mask = parent->dma_mask;

        strcpy(iface->dev.dev_name,"intf");
        USB_sprintf(&iface->dev.bus_id[0], "%d-%s:%d",
           (int)pUdev->bus->busnum, pUdev->devpath,
           i_desc->bInterfaceNumber);
        ms_usbhost_msg("%s %s - registering interface %s\n", pUdev->dev.dev_name, pUdev->dev.bus_id, iface->dev.bus_id);
        ms_device_add (&iface->dev);
        // Unsupport usb driverfs
    }
    return 0;
fail:
    //pUdev->eState = USB_STATE_DEFAULT;
    ms_usb_set_device_state(pUdev, USB_STATE_NOTATTACHED); // patch from Linux 3.1.10
    //ms_clear_bit(pUdev->u32DevNum, pUdev->bus->devmap.usb_devicemap, unsigned long);
    ms_devmap_clear_bit(pUdev->u32DevNum, pUdev->bus->devmap.usb_devicemap);
    pUdev->u32DevNum = -1;
    ms_usb_put_dev(pUdev);
    return s32Err;
}

void *ms_usb_buffer_alloc (
  struct usb_device *pUdev,
  size_t size,
  int mem_flags,
  dma_addr_t *dma
)
{
  if (!pUdev || !pUdev->bus /*|| !pUdev->bus->bus_ops || !pUdev->bus->bus_ops->ms_alloc_hcd_buffer*/)
    return 0;
  //return pUdev->bus->bus_ops->ms_alloc_hcd_buffer (pUdev->bus, size, mem_flags, dma);
  return ms_hcd_buffer_alloc(pUdev->bus, size, mem_flags, dma);
}

void ms_usb_buffer_free (
  struct usb_device *pUdev,
  size_t size,
  void *addr,
  dma_addr_t dma
)
{
  if (!pUdev || !pUdev->bus /*|| !pUdev->bus->bus_ops || !pUdev->bus->bus_ops->ms_free_hcd_buffer*/)
        return;
  if (!addr)
        return;
  //pUdev->bus->bus_ops->ms_free_hcd_buffer (pUdev->bus, size, addr, dma);
  ms_hcd_buffer_free(pUdev->bus, size, addr, dma);
}


struct ms_bus_type usb_bus_type = {
  {&usb_bus_type.drivers_list,&usb_bus_type.drivers_list},
  {&usb_bus_type.devices_list,&usb_bus_type.devices_list},
  ms_usb_device_match,
};

int ms_usb_core_init(void)
{
    ms_usb_hub_init();
    ms_driver_register(&ms_usb_generic_driver);

    return 0;
}

int ms_usb_core_uninit(void)
{
    ms_usb_hub_uninit();
    ms_driver_unregister(&ms_usb_generic_driver);

    return 0;
}

