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

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/drv_api.h>
#include <cyg/io/io.h>
#include <cyg/io/devtab.h>
#include <cyg/io/disk.h>

#include "drvSCSI.h"
#include "drvMassStor.h"
#include "drvUSB.h"
//#include <pkgconf/devs_usb.h>

extern void RemoveUSBDiskPort(U8 uPort, U8 uLunNum);
//#define USB_DISK_DBG	1

typedef struct {
    int        num;
    cyg_uint32 size;
    int		uPort;
    int     DevId; 
} usb_disk_info_t;

// ----------------------------------------------------------------------------
#ifndef __lint__ //NOTE: lint will complain for the following device tables..
static bool usb_disk_init(struct cyg_devtab_entry *tab);

static Cyg_ErrNo usb_disk_read(disk_channel *chan,
                                 void         *buf,
                                 cyg_uint32    len,
                                 cyg_uint32    block_num);

static Cyg_ErrNo usb_disk_write(disk_channel *chan,
                                  const void   *buf,
                                  cyg_uint32    len,
                                  cyg_uint32    block_num);

static Cyg_ErrNo usb_disk_get_config(disk_channel *chan,
                                       cyg_uint32    key,
                                       const void   *xbuf,
                                       cyg_uint32   *len);

static Cyg_ErrNo usb_disk_set_config(disk_channel *chan,
                                       cyg_uint32    key,
                                       const void   *xbuf,
                                       cyg_uint32   *len);

static Cyg_ErrNo usb_disk_lookup(struct cyg_devtab_entry  **tab,
                                   struct cyg_devtab_entry   *sub_tab,
                                   const char                *name);

static struct DISK_FUNS(usb_disk_funs,
                 &usb_disk_read,
                 &usb_disk_write,
                 &usb_disk_get_config,
                 &usb_disk_set_config
);

//USB_2
// We define 10 empty block device entries for our USB storage device.
// Initially, these entries can not be used until the USB module connect it.
#if 1
#define USB_DISK_INSTANCE(_number_,_port_,_name_)        \
    static usb_disk_info_t usb_disk_info##_port_##_number_ = {         \
        -1,                                   \
        0,                                    \
        -1,                                   \
        -1                                    \
    };                            \
    disk_controller usb_disk_controller##_port_##_number_ = {          \
    priv:       NULL,                         \
    init:       false,                          \
    busy:       false                           \
    };                      \
    DISK_CHANNEL(usb_disk_channel##_port_##_number_,     \
    usb_disk_funs,                                             \
    usb_disk_info##_port_##_number_,                          \
    usb_disk_controller##_port_##_number_,                   \
    true           \
    );                             \
    BLOCK_DEVTAB_ENTRY(usb_disk_io##_port_##_number_, \
    _name_,                                              \
    0,                                                   \
    &cyg_io_disk_devio,                      \
    &usb_disk_init,                                \
    &usb_disk_lookup,                           \
    &usb_disk_channel##_port_##_number_              \
    );
#else
#define USB_DISK_INSTANCE(_number_,_port_,_name_)        \
    static usb_disk_info_t usb_disk_info##_port_##_number_ = {         \
    num: _number_,                                   \
    size: 0,                                         \
    uPort: _port_ ,                                   \
    DevId: -1                                        \
    };                            \
    disk_controller usb_disk_controller##_port_##_number_ = {          \
    priv:       NULL,                         \
    init:       false,                          \
    busy:       false                           \
    };                      \
    DISK_CHANNEL(usb_disk_channel##_port_##_number_,     \
    usb_disk_funs,                                             \
    usb_disk_info##_port_##_number_,                          \
    usb_disk_controller##_port_##_number_,                   \
    true           \
    );                             \
    BLOCK_DEVTAB_ENTRY(usb_disk_io##_port_##_number_, \
    _name_,                                              \
    0,                                                   \
    &cyg_io_disk_devio,                      \
    &usb_disk_init,                                \
    &usb_disk_lookup,                           \
    &usb_disk_channel##_port_##_number_              \
    );
#endif

USB_DISK_INSTANCE(0, 0, "/dev/sda/");
USB_DISK_INSTANCE(1, 0, "/dev/sdb/");
USB_DISK_INSTANCE(2, 0, "/dev/sdc/");
USB_DISK_INSTANCE(3, 0, "/dev/sdd/");
USB_DISK_INSTANCE(4, 0, "/dev/sde/");
USB_DISK_INSTANCE(5, 0, "/dev/sdf/");
USB_DISK_INSTANCE(6, 0, "/dev/sdg/");
USB_DISK_INSTANCE(7, 0, "/dev/sdh/");

USB_DISK_INSTANCE(0, 1, "/dev/sdi/");
USB_DISK_INSTANCE(1, 1, "/dev/sdj/");
USB_DISK_INSTANCE(2, 1, "/dev/sdk/");
USB_DISK_INSTANCE(3, 1, "/dev/sdl/");
USB_DISK_INSTANCE(4, 1, "/dev/sdm/");
USB_DISK_INSTANCE(5, 1, "/dev/sdn/");
USB_DISK_INSTANCE(6, 1, "/dev/sdo/");
USB_DISK_INSTANCE(7, 1, "/dev/sdp/");

USB_DISK_INSTANCE(0, 2, "/dev/sdq/");
USB_DISK_INSTANCE(1, 2, "/dev/sdr/");
USB_DISK_INSTANCE(2, 2, "/dev/sds/");
USB_DISK_INSTANCE(3, 2, "/dev/sdt/");
USB_DISK_INSTANCE(4, 2, "/dev/sdu/");
USB_DISK_INSTANCE(5, 2, "/dev/sdv/");
USB_DISK_INSTANCE(6, 2, "/dev/sdw/");
USB_DISK_INSTANCE(7, 2, "/dev/sdx/");

#ifdef _USB_DISK_DEBUG
void dump_data(cyg_uint8 *buf, cyg_uint32 len)
{
    int i, j=0;

    while(j<len)
    {
        diag_printf("[ ");
        for (i=0; i<16; i++)
            diag_printf("%x ", buf[j+i]);
        diag_printf(" ]\n");
        j+=i;
    }    

}
#endif

static bool
usb_disk_init(struct cyg_devtab_entry *tab)
{
	disk_channel      *chan       = (disk_channel *) tab->priv;
	cyg_disk_info_t *info = chan->info;

    info->connected = false;
    chan->valid = false;

	if (chan->init)
		return true;


	//if (!(chan->callbacks->disk_init)(tab))
	//	return false;

	// Initially, we declare that we are not connected
        info->connected = false;
        chan->valid     = false;

	return true;
}

static Cyg_ErrNo
usb_disk_lookup(struct cyg_devtab_entry **tab,
                  struct cyg_devtab_entry  *sub_tab,
                  const char               *name)
{
    disk_channel *chan = (disk_channel *) (*tab)->priv;
    return (chan->callbacks->disk_lookup(tab, sub_tab, name));
}

static Cyg_ErrNo
usb_disk_read(disk_channel *chan,
                void         *buf,
                cyg_uint32    len,
                cyg_uint32    block_num)
{
	usb_disk_info_t *usb_info = (usb_disk_info_t *)chan->dev_priv;

    if ((usb_info->uPort == -1) ||
        (usb_info->num == -1) ||
        (usb_info->DevId == -1) )
    {
        diag_printf("Err: usb_disk_read: device not exist \n");
        return ENODEV;
    }
        
    if (bSCSI_Read_10(usb_info->uPort, usb_info->num, block_num,
					len , (cyg_uint8*)buf) )
    {
        #ifdef _USB_DISK_DEBUG
            diag_printf("[%s][%d] ==============================\n", __FUNCTION__, __LINE__);
            diag_printf("[%s][%d] %d %d %d %d 0x%08x\n", __FUNCTION__, __LINE__,
                usb_info->uPort, usb_info->num, block_num, len , (int)buf);
            dump_data(buf, len* 512);
            diag_printf("[%s][%d] ==============================\n", __FUNCTION__, __LINE__);
        #endif
    
		return ENOERR;
    }    
	else

		return -EIO;
}

static Cyg_ErrNo
usb_disk_write(disk_channel *chan,
                 const void   *buf,
                 cyg_uint32    len,
                 cyg_uint32    block_num)
{
	usb_disk_info_t *usb_info = (usb_disk_info_t *)chan->dev_priv;

    if ((usb_info->uPort == -1) ||
        (usb_info->num == -1) ||
        (usb_info->DevId == -1) )
    {
        diag_printf("Err: usb_disk_read: device not exist \n");
        return ENODEV;
    }
    
    //kevinhuang, no write temporarily coz current FATFS will contaminate FS on the disk
	if (bSCSI_Write_10(usb_info->uPort, usb_info->num, block_num,
					len, (cyg_uint8*) buf) )
		return ENOERR;
	else
		return -EIO;
}

static Cyg_ErrNo
usb_disk_get_config(disk_channel *chan,
                      cyg_uint32    key,
                      const void   *xbuf,
                      cyg_uint32   *len)
{


    return -EINVAL;
}

static Cyg_ErrNo
usb_disk_set_config(disk_channel *chan,
                      cyg_uint32    key,
                      const void   *xbuf,
                      cyg_uint32   *len)
{
   Cyg_ErrNo                   result  = ENOERR;
    // cyg_mmc_bus_device*    disk    = (cyg_mmc_bus_device*) chan->dev_priv;
//    cyg_disk_info_t *info = chan->info;

    switch(key)
    {
    case CYG_IO_SET_CONFIG_DISK_MOUNT:
        // There will have been a successful lookup(), so there's
        // little point in checking the disk again.
        // if ((sd_chk_ready()!=_DEVICE_READY)&&
        //     result=-EINVAL;
        //else device is OK

        break;

    case CYG_IO_SET_CONFIG_DISK_UMOUNT:
        if (0 == chan->info->mounts)
        {
            //cyg_mmc_bus* bus = disk->mmc_bus_dev;

            // If this is the last unmount of the card, mark it as
            // disconnected. If the user then removes the card and
            // plugs in a new one everything works cleanly.
            // disk->mmc_connected = false;
            // info->connected = false;
            //result = (chan->callbacks->disk_disconnected)(chan);

            // * deselect it too if it had been selected * //
            // * We don't care about any error really. This device is going away. * //
            //mmc_bus_select_card( bus, NULL );
        }
        break;
    }

    return result;
    // return -EINVAL;

}
#endif

extern struct us_data *Mass_stor_us[];
U8 MDrv_USB_MscLookupHostID(char *str)
{
    cyg_devtab_entry_t  *t;    
    disk_channel    	*chan;
    usb_disk_info_t     *p_usb_info;
    U8 host_id = 0xff;
    
    for (t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
    {
    	chan = (disk_channel *) t->priv;
        if (chan == NULL)
            continue;
        p_usb_info = (usb_disk_info_t *) chan->dev_priv;
        if (p_usb_info == NULL)
            continue;

        if ((p_usb_info->DevId != (-1)) && (strcmp(str, t->name) == 0)) // find the table entry
        {
            host_id = Mass_stor_us[p_usb_info->uPort]->host_id;
            break;
        }
    }
    if (host_id == 0xff)
        diag_printf("Not found %s on any USB host port\n", str);
    return (host_id);
}

char *MDrv_USB_MscLookupProductString(char *str)
{
    cyg_devtab_entry_t  *t;    
    disk_channel    	*chan;
    usb_disk_info_t     *p_usb_info;
    char *pstr;

    pstr = NULL;    
    for (t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
    {
    	chan = (disk_channel *) t->priv;
        if (chan == NULL)
            continue;
        p_usb_info = (usb_disk_info_t *) chan->dev_priv;
        if (p_usb_info == NULL)
            continue;

        if ((p_usb_info->DevId != (-1)) && (strcmp(str, t->name) == 0)) // find the table entry
        {
            pstr = Mass_stor_us[p_usb_info->uPort]->pusb_dev->product;
            break;
        }
    }
    if (pstr == NULL)
        diag_printf("Product string not found\n");
    return (pstr);
}

BOOL ConnectUSBDisk(U8 uPort, U8 uLunNum)
{
    struct LUN_Device   *LunDevice = Mass_stor_us[uPort]->Mass_stor_device;
    // Cyg_ErrNo		    err;
	cyg_devtab_entry_t  *dev_h;
	// cyg_io_handle_t     io_h;
	disk_channel    	*chan;
	usb_disk_info_t     *p_usb_info;
	cyg_disk_identify_t ident;
	cyg_devtab_entry_t  *t;
    char				szUSBDevName[12]={0};
    //int  ret;

	for (t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
	{
		chan = (disk_channel *) t->priv;
                if (chan == NULL)
                    continue;
	        p_usb_info = (usb_disk_info_t *) chan->dev_priv;
                if (p_usb_info == NULL)
                    continue;
                if (p_usb_info->DevId == ((uPort+1)*MAX_USB_STOR_LUNS + uLunNum) )
                {
                    diag_printf("Warning : find one previous device here !!\n");
                    //call RemoveUSBDiskPort()??? 
                    RemoveUSBDiskPort(uPort, uLunNum);
                    p_usb_info->DevId = -1;
                }
	}

	for (t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
	{
        
		chan = (disk_channel *) t->priv;
                if (chan == NULL)
                    continue;
      	        p_usb_info = (usb_disk_info_t *) chan->dev_priv;
                if (p_usb_info == NULL)
                    continue;

                if (p_usb_info->DevId == (-1))
                {
                    p_usb_info->DevId = (uPort+1)*MAX_USB_STOR_LUNS + uLunNum;
                    break;
                }
	}

    if (t == &__DEVTAB_END__)
    {
        diag_printf("Error! Can't find available disk name. Exceed maximum support number!\n");        
		return FALSE;
    }
    
	chan = (disk_channel *) t->priv;
	chan->info->connected = true;

    #if 0
    szUSBDevName[7] = t->name[7];
    diag_printf("cyg_io_lookup %s ..", szUSBDevName);

	err = cyg_io_lookup(szUSBDevName, &io_h);
	if (err != ENOERR)
	{
        diag_printf(" FALSE !!\n");
		return FALSE;
	}
    diag_printf(" OK\n");

	dev_h = (cyg_devtab_entry_t *) io_h;
    #endif
    
    dev_h = t;
    
	chan = (disk_channel*) dev_h->priv;
	p_usb_info = (usb_disk_info_t *) chan->dev_priv;
	p_usb_info->uPort = uPort;
	p_usb_info->num = uLunNum;

	ident.serial[0]       = '\0';
	ident.firmware_rev[0] = '\0';
	ident.model_num[0]    = '\0';
   // ident.lba_sectors_num = usb_info->size / 512;
    ident.lba_sectors_num = LunDevice[uLunNum].u32BlockTotalNum;
	ident.cylinders_num   = 0;
	ident.heads_num       = 0;
	ident.sectors_num     = 0;
    ident.phys_block_size = LunDevice[uLunNum].u32BlockSize;
    ident.max_transfer    = LunDevice[uLunNum].u32BlockSize;//  512;

    //diag_printf("Connect USB disk %s\n", szUSBDevName);
    diag_printf("Connect USB disk %s\n", dev_h->name);
    diag_printf("Port:%d Lun:%d Id:%d\n", p_usb_info->uPort, p_usb_info->num, p_usb_info->DevId);
	if (!(chan->callbacks->disk_init)(dev_h))
		return FALSE;

	if (ENOERR != (chan->callbacks->disk_connected)(dev_h, &ident))
		return FALSE;

    if ( _DrvUSB_CBFun != NULL )
    {
        if (strlen(dev_h->name) < 12)
            strcpy(szUSBDevName, dev_h->name);
        _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_STOR, szUSBDevName);
    }
    //Test
    //ret = mount("/dev/sda/1", "/", "fatfs");
    //diag_printf("mount status : %d \n", ret); 
    diag_printf("@@ Host ID %x\n", MDrv_USB_MscLookupHostID(szUSBDevName));

	return TRUE;
}

VOID RemoveUSBDiskPort(U8 uPort, U8 uLunNum)
{
    // Cyg_ErrNo		    err;
	cyg_devtab_entry_t  *dev_h, *t;
	// cyg_io_handle_t     io_h;
	disk_channel    	*chan;
	cyg_disk_info_t     *info;
	usb_disk_info_t     *p_usb_info;
    char				szUSBDevName[12]={0};
    ///struct LUN_Device*  LunDevice = Mass_stor_us[uPort]->Mass_stor_device;
   

#if 0
	//UTL_snprintf(szUSBDevName, 12, "/dev/sd%c/", 'a' + uLunNum);
    szUSBDevName[7]= 'a' + (uPort * MAX_USB_STOR_LUNS) + uLunNum;

	err = cyg_io_lookup(szUSBDevName, &io_h);
	if (err != ENOERR)
		return;
	dev_h = (cyg_devtab_entry_t *) io_h;
#endif    

	for (t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
	{
        
		chan = (disk_channel *) t->priv;
                if (chan == NULL)
                    continue;
	        p_usb_info = (usb_disk_info_t *) chan->dev_priv;
                if (p_usb_info == NULL)
                    continue;

                if (p_usb_info->DevId == ((uPort+1)*MAX_USB_STOR_LUNS + uLunNum))
                {
                    if ((p_usb_info->uPort != uPort) || (p_usb_info->num != uLunNum))
                    {
                        diag_printf("Warning: RemoveUSBDiskPort not match !!\n");
                        diag_printf("[Port lun]: [%d %d]->[%d %d]\n", uPort,uLunNum,p_usb_info->uPort,p_usb_info->num);     
                    }
                    diag_printf("RemoveUSBDiskPort:%d %d %d\n",p_usb_info->uPort,p_usb_info->num,p_usb_info->DevId);
                    break;
                }
	}

    if (t == &__DEVTAB_END__)
    {
        diag_printf("RemoveUSBDiskPort: Can't find match Port:%d Lun:%d\n", uPort, uLunNum);        
		return;
    }

	dev_h = t;
	chan = (disk_channel *) dev_h->priv;
	info = chan->info;
	p_usb_info = (usb_disk_info_t *) chan->dev_priv;
	p_usb_info->DevId = -1;
	p_usb_info->uPort = -1;
	p_usb_info->num = -1;

    //diag_printf("Remove USB disk %s\n", szUSBDevName);
    diag_printf("Remove USB disk %s\n", dev_h->name);
	chan->callbacks->disk_disconnected(chan);

    if ( _DrvUSB_CBFun != NULL )
    {
        if (strlen(dev_h->name) < 12)
            strcpy(szUSBDevName, dev_h->name);
        _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_STOR, szUSBDevName);
    }    

	//info->connected = false;
	//chan->valid     = false;
}

