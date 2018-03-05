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
//#include "include/drvPorts.h" // NUSED
//#include "include/drvConfig.h" // NUSED
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvList.h" // NUSED
#include "include/drvKernel.h"
#include "drvUsbHostConfig.h"

/**
     * @brief               binding generic device to driver
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
void ms_device_bind_driver(struct device_s * ms_gdev)
{
    ms_usbhost_debug("bound device '%s' to driver '%s'\n",
        ms_gdev->bus_id,ms_gdev->driver->name);
    ms_insert_list_before(&ms_gdev->driver_list,&ms_gdev->driver->devices);
}

/**
     * @brief               generic device and driver matching
     *
     * @param           struct device_s * ms_gdev
     * @param           struct device_driver * ms_drv
     *
     * @return          error code
     */
static int ms_bus_match(struct device_s * ms_gdev, struct device_driver * ms_drv)
{
    int ms_err = -ENODEV;

    // Call BUS provided match function to match device ID witch specific comparing method
    if (ms_gdev->bus_type->match_func(ms_gdev,ms_drv))
    {
        ms_gdev->driver = ms_drv;
        // Match device ==> call driver's probe function
        if (ms_drv->probe)
        {
            if ((ms_err = ms_drv->probe(ms_gdev)))
            {
                ms_gdev->driver = NULL;
                return ms_err;
            }
        }
        ms_device_bind_driver(ms_gdev);
        ms_err = 0;
    }

    return ms_err;
}

/**
     * @brief               generic device attaching to bus
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          error code
     */
static int ms_device_attach(struct device_s * ms_gdev)
{
    struct ms_bus_type * ms_bus = ms_gdev->bus_type;
    struct list_head * entry;
    struct device_driver  * ms_drv;
    struct list_head *_mtr;

    if (ms_gdev->driver)
    {
        ms_device_bind_driver(ms_gdev);
        return 1;
    }

    if (ms_bus->match_func)
    {
        list_for_loop(entry,&ms_bus->drivers_list)
        {
        _mtr = entry ;
        ms_drv = (struct device_driver *)((char *)_mtr - (char *)offsetof(struct device_driver, bus_list) );
        if (!ms_bus_match(ms_gdev,ms_drv))
            return 1;
        }
    }

    return 0;
}

/**
     * @brief               generic driver attaching to bus
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          none
     */
void ms_driver_attach(struct device_driver * ms_drv)
{
    struct ms_bus_type * ms_bus = ms_drv->bus;
    struct list_head * entry;
    struct device_s * ms_gdev;
    const struct list_head *_mtr;

    if (!ms_bus->match_func)
        return;

    list_for_loop(entry,&ms_bus->devices_list)
    {
        _mtr = entry ;
        ms_gdev = (struct device_s *)((char *)_mtr - (char *)offsetof(struct device_s, bus_list) );
        if (!ms_gdev->driver)
        {
            ms_bus_match(ms_gdev,ms_drv);
        }
    }
}

/**
     * @brief               generic device release its driver
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
void ms_device_release_driver(struct device_s * ms_gdev)
{
    struct device_driver * ms_drv = ms_gdev->driver;

    if (ms_drv)
    {
        ms_list_remove_and_init(&ms_gdev->driver_list);
        if (ms_drv->remove)
            ms_drv->remove(ms_gdev);
        ms_gdev->driver = NULL;
    }
}

/**
     * @brief               generic driver release its binding device
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          none
     */
static void ms_driver_detach(struct device_driver * ms_drv)
{
    struct list_head * entry, * next;
    struct device_s *ms_gdev;
    struct list_head *_mptr;

    list_for_loop_ex(entry,next,&ms_drv->devices)
    {
        // struct device * dev = container_of(entry,struct device,driver_list);
        _mptr = entry;
        ms_gdev = (struct device_s *)( (char *)_mptr - (char *)offsetof(struct device_s, driver_list) );
        ms_device_release_driver(ms_gdev);
    }
}

/**
     * @brief               adding generic device to bus
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          error code
     */
int ms_bus_add_device(struct device_s * ms_gdev)
{
    int ms_err = 0;

    if (ms_gdev->bus_type)
    {
        ms_usbhost_debug("bus USB: add device %s\n",ms_gdev->bus_id);
        ms_insert_list_before(&ms_gdev->bus_list,&ms_gdev->bus_type->devices_list);
        ms_device_attach(ms_gdev);
    }
    return ms_err;
}

/**
     * @brief               generic device removing from bus
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
void ms_bus_remove_device(struct device_s * ms_gdev)
{
    if (ms_gdev->bus_type)
    {
        ms_usbhost_debug("bus USB: remove device %s\n",ms_gdev->bus_id);
        ms_device_release_driver(ms_gdev);
        ms_list_remove_and_init(&ms_gdev->bus_list);
    }
}

/**
     * @brief               adding generic driver to bus
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          error code
     */
int ms_bus_add_driver(struct device_driver * ms_drv)
{
    struct ms_bus_type * ms_bus = ms_drv->bus;
    int ms_err = 0;

    if (ms_bus)
    {
        ms_usbhost_debug("bus USB: add driver %s\n",ms_drv->name);

        // Add driver to bus driver list
        ms_insert_list_before(&ms_drv->bus_list,&ms_bus->drivers_list);
        ms_driver_attach(ms_drv);
    }
    return ms_err;
}

/**
     * @brief               generic driver removing from bus
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          none
     */
void ms_bus_remove_driver(struct device_driver * ms_drv)
{
    if (ms_drv->bus)
    {
        ms_usbhost_debug("bus USB: remove driver %s\n",ms_drv->name);
        ms_driver_detach(ms_drv);
        ms_list_remove_and_init(&ms_drv->bus_list);
    }
}

