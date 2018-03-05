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
////////////////////////////////////////////////////////////////////////////////

#include "../../../drv/usbhost/include/drvGlobal.h"
#include "../../../drv/usbhost/include/_drvUSB.h"

U16 HAL_USB_GetChipID(void)
{
    return CHIPID_MONET;
}

void HAL_USB_BC_Enable(U8 uPort, BOOLEAN bEnable)
{
    static U8 PortBCStaus[4] = {1, 1, 1, 1}; //Set port BC enabled at first time
    U32 utmi_base, bc_base;

    if (uPort == 0)
    {
        utmi_base = 0x103A80;
        bc_base = 0x123600;
    }
    else if (uPort == 1)
    {
        utmi_base = 0x103A00;
        bc_base = 0x123620;
    }
    else if (uPort == 2)
    {
        utmi_base = 0x103900;
        bc_base = 0x123640;
    }
    else if (uPort == 3)
    {
        utmi_base = 0x122080;
        bc_base = 0x123660;
    }
    else
    {
        printf("HAL_USB_BC_Enable: invalid port number %d\n", uPort);
        return;
    }

    if (bEnable)
    {
        if ( !PortBCStaus[uPort] )
        {
            printf("USB BC mode enabled\n");
            MDrv_WriteRegBit(utmi_base+0x01, ENABLE, 0x40); //IREF_PDN=1¡¦b1. (utmi+0x01[6] )
            MDrv_WriteRegBit(bc_base+0x03, ENABLE, 0x40); // [6]= reg_host_bc_en
            MDrv_WriteRegBit(bc_base+0x0C, ENABLE, 0x40); // [6]= reg_into_host_bc_sw_tri
            MDrv_Write2Byte(bc_base, 0x0); // [15:0] = bc_ctl_ov_en
            MDrv_WriteRegBit(bc_base+0x0A, ENABLE, 0x80); // [7]=reg_bc_switch_en
            PortBCStaus[uPort] = 1;
        }
    }
    else
    {
        if ( PortBCStaus[uPort] )
        {
            printf("USB BC mode disabled\n");
            MDrv_WriteRegBit(bc_base+0x0C, DISABLE, 0x40); // [6]= reg_into_host_bc_sw_tri
            MDrv_WriteRegBit(bc_base+0x03, DISABLE, 0x40); // [6]= reg_host_bc_en
            MDrv_WriteRegBit(utmi_base+0x01, DISABLE, 0x40); //IREF_PDN=1¡¦b1. (utmi+0x01[6] )
            PortBCStaus[uPort] = 0;
        }
    }
}

