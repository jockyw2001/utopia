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

//#undef DEBUG

#include <MsCommon.h>
#include  "include/drvPorts.h"
#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"

#include "drvBase.h"


void device_bind_driver(struct device_s * dev)
{
	pr_debug("bound device '%s' to driver '%s'\n",
				dev->bus_id,dev->driver->name);
	list_add_tail(&dev->driver_list,&dev->driver->devices);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int bus_match(struct device_s * dev, struct device_driver * drv)
{
	int error = -ENODEV;

	// Call BUS provided match function to match device ID witch specific comparing method
	if (dev->bus->match(dev,drv)) {
		dev->driver = drv;
		// Match device ==> call driver's probe function
		if (drv->probe) {
			if ((error = drv->probe(dev))) {
				dev->driver = NULL;
				return error;
			}
		}
		device_bind_driver(dev);
		error = 0;
	}

	return error;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int device_attach(struct device_s * dev)
{
	struct bus_type * bus = dev->bus;
	struct list_head * entry;
	struct device_driver  * drv;
	struct list_head *_mtr;

	if (dev->driver) {
		device_bind_driver(dev);
		return 1;
	}

	if (bus->match) {
		list_for_each(entry,&bus->drivers_list) {
			_mtr = entry ;
			drv = (struct device_driver *)((char *)_mtr - (char *)offsetof(struct device_driver, bus_list) );
			if (!bus_match(dev,drv))
				return 1;
		}
	}

  return 0;
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void driver_attach(struct device_driver * drv)
{
	struct bus_type * bus = drv->bus;
	struct list_head * entry;
	struct device_s * dev;
	const struct list_head *_mtr;

	if (!bus->match)
		return;

	list_for_each(entry,&bus->devices_list) {
		_mtr = entry ;
		dev = (struct device_s *)((char *)_mtr - (char *)offsetof(struct device_s, bus_list) );
		if (!dev->driver) {
			bus_match(dev,drv);
		}
	}
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void device_release_driver(struct device_s * dev)
{
	struct device_driver * drv = dev->driver;

	if (drv) {
		list_del_init(&dev->driver_list);
		if (drv->remove)
			drv->remove(dev);
		dev->driver = NULL;
	}
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void driver_detach(struct device_driver * drv)
{
	struct list_head * entry, * next;
	struct device_s *dev;
	struct list_head *_mptr;

	list_for_each_safe(entry,next,&drv->devices) {
		// struct device * dev = container_of(entry,struct device,driver_list);
		_mptr = entry;
		dev = (struct device_s *)( (char *)_mptr - (char *)offsetof(struct device_s, driver_list) );
		device_release_driver(dev);
	}

}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int bus_add_device(struct device_s * dev)
{
	int error = 0;

	if (dev->bus) {
		pr_debug("bus %s: add device %s\n",dev->bus->name,dev->bus_id);
		list_add_tail(&dev->bus_list,&dev->bus->devices_list);
		device_attach(dev);
	}
	return error;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void bus_remove_device(struct device_s * dev)
{
	if (dev->bus) {
		pr_debug("bus %s: remove device %s\n",dev->bus->name,dev->bus_id);
		device_release_driver(dev);
		list_del_init(&dev->bus_list);
	}
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int bus_add_driver(struct device_driver * drv)
{
	struct bus_type * bus = drv->bus;
	int error = 0;

	if (bus) {
		pr_debug("bus %s: add driver %s\n",bus->name,drv->name);

		// Add driver to bus driver lsit
		list_add_tail(&drv->bus_list,&bus->drivers_list);
		driver_attach(drv);
	}
	return error;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void bus_remove_driver(struct device_driver * drv)
{
	if (drv->bus) {
		pr_debug("bus %s: remove driver %s\n",drv->bus->name,drv->name);
		driver_detach(drv);
		list_del_init(&drv->bus_list);
	}
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int bus_register(struct bus_type * bus)
{
	pr_debug("bus type '%s' registered\n",bus->name);
	return 0;
}



void bus_unregister(struct bus_type * bus)
{
	pr_debug("bus %s: unregistering\n",bus->name);
}

