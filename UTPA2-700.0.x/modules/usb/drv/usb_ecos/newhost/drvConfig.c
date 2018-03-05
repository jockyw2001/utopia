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

#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvList.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include  "include/drvTimer.h"
#include "include/drvUSBHost.h"
#include "drvUsbd.h"
#define USB_MAX_ALTSETTING    128
#define USB_MAX_ENDPOINTS     30

#define USB_MAX_CONFIG        8
#define USB_ALTSETTING_ALLOC  4
#define USB_MAX_INTERFACES    32
#define MAX_PKT_SIZE(wMaxPacketSize) ((wMaxPacketSize) & 0x07ff)

/**
     * @brief               Parse endpoint descriptor in the buffer
     *
     * @param dev
     * @param ept
     * @param buf
     * @param bufsize
     *
     * @return          The number of bytes it parsed
     */
static int ms_usb_parse_ept(
        struct usb_device *dev,
        struct usb_host_endpoint *pEpt,
        unsigned char *pBuf,
        int iBufSize)
{
    struct usb_descriptor_header *desc = (struct usb_descriptor_header *)pBuf;
    int checked = 0, len;

    if (desc->bDescriptorType != USB_DT_ENDPOINT)
    {
        //diag_printf("pEpt descriptor type is invalid\n");
        return 0;
    }

    if (desc->bLength > iBufSize)
    {
        diag_printf("pEpt descriptor size is too large (%d)\n", desc->bLength);
        return -1;
    }

    len = USB_DT_ENDPOINT_SIZE;
    if (desc->bLength == USB_DT_ENDPOINT_AUDIO_SIZE)
        len = USB_DT_ENDPOINT_AUDIO_SIZE;

    memcpy(&pEpt->desc, pBuf, len);

    pEpt->desc.wMaxPacketSize = pEpt->desc.wMaxPacketSize;

    if (dev->eSpeed == USB_HIGH_SPEED
        && (pEpt->desc.bmAttributes & 0x03) == USB_ENDPOINT_XFER_BULK) {
        if (512 != pEpt->desc.wMaxPacketSize)
        {
            diag_printf("bulk pEpt max packet size is not 512\n");
        }
    }

    pBuf += desc->bLength;
    iBufSize -= desc->bLength;
    checked += desc->bLength;

    //Skip descriptors we don't support
    while ((U32)iBufSize >= sizeof(struct usb_descriptor_header))
    {
        desc = (struct usb_descriptor_header *)pBuf;

        if ((desc->bDescriptorType == USB_DT_ENDPOINT) ||
            (desc->bDescriptorType == USB_DT_INTERFACE) ||
            (desc->bDescriptorType == USB_DT_CONFIG) ||
            (desc->bDescriptorType == USB_DT_DEVICE))
            break;

        pBuf += desc->bLength;
        iBufSize -= desc->bLength;
        checked += desc->bLength;
    }

  return checked;
}

/**
     * @brief               Release memory associated with the interface
     *
     * @param dev
     *
     * @return          None
     */
static void ms_usb_release_intf(struct device_s *dev)
{
    struct usb_interface *pIntf = get_usb_interface(dev);
    int ii;

    if (pIntf->altsetting)
    {
        for (ii = 0; ii < pIntf->num_altsetting; ii++)
        {
            struct usb_host_interface *pAltset = &pIntf->altsetting[ii];

            if (pAltset->endpoint)
                kfree(pAltset->endpoint);
        }
        kfree(pIntf->altsetting);
    }

    kfree(pIntf);
}

/**
     * @brief               Parse interface descriptor in the buffer
     *
     * @param dev
     * @param intf
     * @param buf
     * @param bufsize
     *
     * @return          The number of bytes it parsed
     */
static int ms_usb_parse_interface(
        struct usb_device *dev,
        struct usb_interface *pIntf,
        unsigned char *pBuf,
        int bufsize)
{
    int i, retval, checked = 0;
    struct usb_descriptor_header *pDesc;
    struct usb_host_interface *pIf;
    struct usb_interface_descriptor  *pIntf_desc;

    pIntf->act_altsetting = 0;
    pIntf->num_altsetting = 0;
    pIntf->max_altsetting = USB_ALTSETTING_ALLOC; //Use 4 interface altsettings each time
    ms_device_initialize(&pIntf->dev);
    pIntf->dev.release_func = ms_usb_release_intf;

    pIntf->altsetting = (struct usb_host_interface *)
        kmalloc(sizeof(*pIntf->altsetting) * pIntf->max_altsetting, GFP_KERNEL);

    if (!pIntf->altsetting)
    {
        diag_printf("failed to alloc altsetting\n");
        return -1;
    }

    while (bufsize > 0)
    {
        if (pIntf->num_altsetting >= pIntf->max_altsetting) //Allocate more memory for altset
        {
            struct usb_host_interface *pInft_tmp;
            int cur_max_altsetting;

            cur_max_altsetting = pIntf->max_altsetting;
            pIntf->max_altsetting += USB_ALTSETTING_ALLOC;
            if (pIntf->max_altsetting > USB_MAX_ALTSETTING)
            {
                diag_printf("altsettings are over (%d)\n", USB_MAX_ALTSETTING);
                return -1;
            }

            pInft_tmp = (struct usb_host_interface *)
                    kmalloc(sizeof(*pInft_tmp) * pIntf->max_altsetting, GFP_KERNEL);
            if (pInft_tmp == NULL)
            {
                diag_printf("failed to alloc mem for altsetting\n");
                return -1;
            }
            memcpy(pInft_tmp, pIntf->altsetting, sizeof(*pIntf->altsetting) * cur_max_altsetting);
            kfree(pIntf->altsetting);
            pIntf->altsetting = pInft_tmp;
        }

        pIf = pIntf->altsetting + pIntf->num_altsetting;
        pIf->endpoint = NULL;
        pIntf->num_altsetting++;

        memcpy(pIf, pBuf, USB_DT_INTERFACE_SIZE);

        pBuf += pIf->desc.bLength;
        checked += pIf->desc.bLength;
        bufsize -= pIf->desc.bLength;

        //skip descriptors we can't handle
        while ((U32)bufsize >= sizeof(struct usb_descriptor_header))
        {
            pDesc = (struct usb_descriptor_header *)pBuf;

            if ((pDesc->bDescriptorType == USB_DT_INTERFACE) ||
                (pDesc->bDescriptorType == USB_DT_ENDPOINT) ||
                (pDesc->bDescriptorType == USB_DT_CONFIG) ||
                (pDesc->bDescriptorType == USB_DT_DEVICE))
                break;

            pBuf += pDesc->bLength;
            checked += pDesc->bLength;
            bufsize -= pDesc->bLength;
        }

        pDesc = (struct usb_descriptor_header *)pBuf;
        if (((U32)bufsize >= sizeof(struct usb_descriptor_header)) &&
            ((pDesc->bDescriptorType == USB_DT_CONFIG) ||
            (pDesc->bDescriptorType == USB_DT_DEVICE)))
            return checked;

        if ( (pIf->desc.bNumEndpoints > 0) && (pIf->desc.bNumEndpoints <= USB_MAX_ENDPOINTS) )
        {
            pIf->endpoint = (struct usb_host_endpoint *)
                        kmalloc(sizeof(struct usb_host_endpoint) *
                                pIf->desc.bNumEndpoints, GFP_KERNEL);
            if (!pIf->endpoint)
            {
                diag_printf("fail to alloc mem for ept\n");
                return -1;
            }

            memset(pIf->endpoint, 0, sizeof(struct usb_host_endpoint) *
                    pIf->desc.bNumEndpoints);

            for (i = 0; i < pIf->desc.bNumEndpoints; i++)
            {
                retval = ms_usb_parse_ept(dev, pIf->endpoint + i, pBuf, bufsize);
                if (retval < 0)
                    return retval;

                pBuf += retval;
                checked += retval;
                bufsize -= retval;
            }
        }

        //parsing endpoint done
        pIntf_desc = (struct usb_interface_descriptor *)pBuf;
        if (bufsize < USB_DT_INTERFACE_SIZE
            || pIntf_desc->bDescriptorType != USB_DT_INTERFACE
            || !pIntf_desc->bAlternateSetting)
            return checked;
    }

    return checked;
}

/**
     * @brief               Parse configuration descriptor in the buffer
     *
     * @param dev
     * @param config
     * @param buf
     *
     * @return          The number of bytes it parsed
     */
int ms_usb_parse_config(
        struct usb_device *dev,
        struct usb_host_config *pConfig,
        char *pBuf)
{
    int i, Desc_size;
    int retval = -EINVAL;
    struct usb_descriptor_header *pDesc;

    memcpy(&pConfig->desc, pBuf, USB_DT_CONFIG_SIZE);
    pConfig->desc.wTotalLength = pConfig->desc.wTotalLength;
    Desc_size = pConfig->desc.wTotalLength;

    //for (i = 0; i < USB_MAX_INTERFACES; ++i)
    //  pConfig->interface[i] = (struct usb_interface)NULL;

    if (pConfig->desc.bNumInterfaces > USB_MAX_INTERFACES)
    {
        diag_printf("too many interfaces %d", pConfig->desc.bNumInterfaces);
        return -EINVAL;
    }

    for (i = 0; i < pConfig->desc.bNumInterfaces; i++)
    {
        pConfig->interface[i] = (struct usb_interface *) kmalloc(sizeof(struct usb_interface), GFP_KERNEL);
        if (!pConfig->interface[i])
        {
            diag_printf("fail to alloc mem for interface\n");
            retval = -ENOMEM;
            goto error_exit;
        }
        memset(pConfig->interface[i], 0x00, sizeof(struct usb_interface));
    }

    pBuf += pConfig->desc.bLength;
    Desc_size -= pConfig->desc.bLength;

    for (i = 0; i < pConfig->desc.bNumInterfaces; i++)
    {
        while ((U32)Desc_size >= sizeof(struct usb_descriptor_header))
        {
            pDesc = (struct usb_descriptor_header *)pBuf;

            if ((pDesc->bDescriptorType == USB_DT_ENDPOINT) ||
                (pDesc->bDescriptorType == USB_DT_INTERFACE) ||
                (pDesc->bDescriptorType == USB_DT_CONFIG) ||
                (pDesc->bDescriptorType == USB_DT_DEVICE))
                break;

            pBuf += pDesc->bLength;
            Desc_size -= pDesc->bLength;
        }

        retval = ms_usb_parse_interface(dev, pConfig->interface[i], (unsigned char*) pBuf, Desc_size);
        if (retval < 0)
            return retval;

        pBuf += retval;
        Desc_size -= retval;
    }

    return Desc_size;

error_exit:
    for (i = 0; i < USB_MAX_INTERFACES; ++i)
        kfree(pConfig->interface[i]);

    return retval;
}

/**
     * @brief               Release memory associated with the interface
     *
     * @param dev
     *
     * @return          None
     */
void ms_usb_destroy_config(struct usb_device *dev)
{
    int config_idx, i;

    if (dev->config == NULL)
        return;

    if (dev->pRawDescriptors)
    {
        for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
            kfree(dev->pRawDescriptors[i]);

        kfree(dev->pRawDescriptors);
    }

    for (config_idx = 0; config_idx < dev->descriptor.bNumConfigurations; config_idx++)
    {
        struct usb_host_config *pConfig = &dev->config[config_idx];

        if (!pConfig->interface)
            break;
    }

    kfree(dev->config);
}


/**
     * @brief               Retrieve configuration descriptor from the device
     *
     * @param dev
     *
     * @return            Function status
     */
int ms_usb_get_config(struct usb_device *dev)
{
    int retval;
    U32 cfg_num, len;
    unsigned char *pBuf = NULL;
    unsigned char *pBuf_tmp;
    struct usb_config_descriptor *pDesc;
    U8  uCfg_count;

    uCfg_count = dev->descriptor.bNumConfigurations;
    if ( (uCfg_count == 0) || (uCfg_count > USB_MAX_CONFIG) )
    {
        diag_printf("number of config is invalid\n");
        return -EINVAL;
    }

    dev->config = (struct usb_host_config *)
            kmalloc(dev->descriptor.bNumConfigurations *
            sizeof(struct usb_host_config), GFP_KERNEL);

    if (!dev->config)
    {
        diag_printf("fail to alloc mem for config");
        return -ENOMEM;
    }

    memset(dev->config, 0, dev->descriptor.bNumConfigurations *
            sizeof(struct usb_host_config));

    dev->pRawDescriptors = (char **)kmalloc(sizeof(char *) *
            dev->descriptor.bNumConfigurations, GFP_KERNEL);

    if (!dev->pRawDescriptors)
    {
        diag_printf("fail to alloc mem for descriptors\n");
        return -ENOMEM;
    }

    pBuf = (unsigned char*) kmalloc(8, GFP_KERNEL);
    if (!pBuf)
    {
        diag_printf("unable to allocate memory for configuration descriptors\n");
        return -ENOMEM;
    }

    pDesc = (struct usb_config_descriptor *)pBuf;

    for (cfg_num = 0; cfg_num < dev->descriptor.bNumConfigurations; cfg_num++)
    {
        retval = ms_usb_get_descriptor(dev, USB_DT_CONFIG, cfg_num, pBuf, 8);
        if (retval < 0)
        {
            diag_printf("fail to get config descriptor %d\n", retval);
            goto error_exit;
        }

        len = pDesc->wTotalLength;

        pBuf_tmp = (unsigned char*) kmalloc(len, GFP_KERNEL);
        if (pBuf_tmp == NULL)
        {
            diag_printf("fail to alloc mem for config desc\n");
            retval = -ENOMEM;
            goto error_exit;
        }

        retval = ms_usb_get_descriptor(dev, USB_DT_CONFIG, cfg_num, pBuf_tmp, len);
        if (retval < 0)
        {
            diag_printf("fail to get config descriptor %d\n", retval);
            kfree(pBuf_tmp);
            goto error_exit;
        }

        dev->pRawDescriptors[cfg_num] = (char*) pBuf_tmp;

        retval = ms_usb_parse_config(dev, &dev->config[cfg_num], (char*) pBuf_tmp);
        if (retval < 0)
        {
            retval = -EINVAL;
            goto error_exit;
        }
    }

    if (pBuf)
        kfree(pBuf);
    return 0;

error_exit:
    if (pBuf)
        kfree(pBuf);

    dev->descriptor.bNumConfigurations = cfg_num; //modify the number of config
    return retval;
}





