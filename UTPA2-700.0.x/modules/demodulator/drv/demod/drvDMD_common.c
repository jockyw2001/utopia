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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDMD_common.c
/// @brief  DMD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "drvSYS.h"
#include "drvDMD_common.h"
#include "halDMD_INTERN_common.h"
#include "drvDMD_VD_MBX.h"
#include "ULog.h"
#include "drvMSPI.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SPI_DEVICE_BUFFER_SIZE           256
//////////////MSPI Define//////////////////
#define READ_INDEX  0
#define WRITE_INDEX 1
#define CMD_INDEX 0
#define ADDR_INDEX 1
//SSPI COMMAND
#define RIU_W_CMD      0x1A
#define RIU_W1_CMD     0x1D
#define RIU_R_CMD       0x18
#define RIU_R1_CMD      0x1C
#define RIU_RT_CMD      0x11
#define RIU_R1T_CMD      0x15
#define MIU_W_CMD      0x25
#define MIU_R_CMD   0x20
#define MIU_ST_CMD       0x21
#define CFG_W_CMD       0x05
#define CFG_R_CMD       0x00


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/*
#ifdef CONFIG_AMAZON
extern s_MDRV_DMD_INTERFACE_FUNCTION  sMdrvDmdInterfaceFunc;
#endif
*/
MS_U8   u8DMD_I2C_SLAVE_BUS;
MS_U8   u8DMD_I2C_SLAVE_ADDR;
MS_BOOL   bIS_EXTERN_DMD;
s_I2C_Interface_func   sI2cInterfaceFunc;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_PreInit(void)
{
    DMD_DBG(ULOGD("DEMOD","MDrv_DMD_PreInit\n"));
    /*
    #ifdef CONFIG_AMAZON
    sMdrvDmdInterfaceFunc.MDrv_DMD_I2C_Channel_Change  = MDrv_DMD_I2C_Channel_Change;
    sMdrvDmdInterfaceFunc.MDrv_DMD_I2C_Channel_Set        = MDrv_DMD_I2C_Channel_Set;
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg                     = MDrv_DMD_ReadReg;
    sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg                     = MDrv_DMD_WriteReg;
    sMdrvDmdInterfaceFunc.MDrv_DMD_WriteRegs                   = MDrv_DMD_WriteRegs; 
    sMdrvDmdInterfaceFunc.Log10Approx                                 = Log10Approx;
    #endif
    */
    HAL_DMD_RegInit();

    //HAL_DMD_TS1_Tristate(TRUE);
    MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);

    return TRUE;
}

void MDrv_DMD_RFAGC_Tristate(MS_BOOL bEnable)
{
    HAL_DMD_RFAGC_Tristate(bEnable);
}

void MDrv_DMD_IFAGC_Tristate(MS_BOOL bEnable)
{
    HAL_DMD_IFAGC_Tristate(bEnable);
}

MS_BOOL MDrv_DMD_TS_GetClockRate(void *fTS_CLK)
{
    // TODO: TS output is only valid when demod is locked
//    return HAL_DMD_TS_GetClockRate(fTS_CLK);
return true;
}

MS_BOOL MDrv_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array)
{
    return HAL_DMD_TSO_Clk_Control(u8cmd_array);
}

MS_BOOL MDrv_DMD_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult = FALSE;  
    if (bIS_EXTERN_DMD == FALSE)
        bResult = MDrv_SYS_DMD_VD_MBX_ReadReg(u32Reg, u8Value);
    else //external demod
        bResult = HAL_DMD_IIC_ReadByte((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), u32Reg, u8Value);
    return bResult;
}
MS_BOOL MDrv_DMD_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult = FALSE;  
    if (bIS_EXTERN_DMD == FALSE)
        bResult = MDrv_SYS_DMD_VD_MBX_WriteReg(u32Reg, u8Value);
    else //external demod
        bResult = HAL_DMD_IIC_WriteByte((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), u32Reg, u8Value);
    return bResult;  
}

MS_BOOL MDrv_DMD_WriteRegs(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length)
{
    MS_BOOL bResult = FALSE;  
    if (bIS_EXTERN_DMD == FALSE)
        bResult = MDrv_SYS_DMD_VD_MBX_WriteReg(u32Reg, (*u8Value));
    else //external demod
        bResult = HAL_DMD_IIC_WriteBytes((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), u32Reg, u8Value, u8Length);
    return bResult;  

}

MS_BOOL MDrv_DMD_I2C_Channel_Change(MS_U8 ch_num)
{
    MS_BOOL bResult = FALSE;  
    bResult = HAL_DMD_I2C_Channel_Change((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), ch_num);

    return bResult;  

}

MS_BOOL MDrv_DMD_I2C_Channel_Set(MS_U8  ch_num)
{
    MS_BOOL bResult = FALSE;  

    bResult = HAL_DMD_I2C_Channel_Set((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), ch_num);
    return bResult;  

}

MS_BOOL MDrv_DMD_SSPI_Init(MS_U8  u8DeviceNum)
{

	
#ifndef MSOS_TYPE_LINUX_KERNEL
    if(MDrv_MSPI_Init_Ext(u8DeviceNum) == E_MSPI_OK)
        return TRUE;
    else
        return FALSE;
#endif
     return true;

}

MS_BOOL MDrv_DMD_SSPI_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    MS_BOOL bRet = TRUE;
    MS_U8 Wdata[5];
    
    Wdata[0] = MIU_W_CMD;
    Wdata[1] = u32Addr & 0xFF;
    Wdata[2] = (u32Addr >> 8) & 0xFF;
    Wdata[3] = (u32Addr >> 16)& 0xFF;
    Wdata[4] = (u32Addr >> 24);
    //ULOGD("DEMOD","MDrv_SS_MIU_Writes\n");
#ifndef MSOS_TYPE_LINUX_KERNEL
    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    // send write address
    MDrv_MSPI_Write(Wdata,sizeof(Wdata));
    // send data
    MDrv_MSPI_Write(pdata,u16Size);
    MDrv_MSPI_SlaveEnable(FALSE);
#endif
    return bRet;
}

MS_BOOL MDrv_DMD_SSPI_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    
    MS_BOOL bRet = TRUE;
    MS_U8 Rdata[SPI_DEVICE_BUFFER_SIZE];
    MS_U16 dataLen, i, j=0;

#ifndef MSOS_TYPE_LINUX_KERNEL
    do
    {
        dataLen = (u16Size>16?16:u16Size);//(len>24?24:len);
        
        Rdata[0] = MIU_R_CMD;
        Rdata[1] = u32Addr & 0xFF;
        Rdata[2] = (u32Addr >> 8) & 0xFF;
        Rdata[3] = (u32Addr >> 16)& 0xFF;
        Rdata[4] = (u32Addr >> 24);
        Rdata[5] = dataLen+1;
        //ULOGD("DEMOD","MDrv_SS_MIU_Reads, addr=0x%lx, dataLen=%d\n", u32Addr, dataLen);

        // send read command to read data

        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,6);
        MDrv_MSPI_SlaveEnable(FALSE);

        // read operation
        Rdata[0] = MIU_ST_CMD;
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,1);
        //ULOGD("DEMOD","Read Operation\n");
        MDrv_MSPI_Read(Rdata, dataLen+1);
        MDrv_MSPI_SlaveEnable(FALSE);

        if(Rdata[0] != 0x0A)
        {
            ULOGD("DEMOD","MDrv_SS_MIU_Reads fail, status=0x%x\n", Rdata[0] );
            return false;
        }

        for (i=1; i<dataLen+1; i++, j++) 
        {
            pdata[j] = Rdata[i];
            //ULOGD("DEMOD","%x, ", pdata[j]);
        }

        u16Size -= dataLen;
        u32Addr += dataLen;
        //ULOGD("DEMOD","u16Size=%d,  u32Addr=0x%lx\n", u16Size, u32Addr);
    }while(u16Size);
#endif
    return bRet;
}

MS_BOOL MDrv_DMD_SSPI_RIU_Write8(MS_U16 u16Addr, MS_U8 data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 Wdata[4];

    Wdata[0] = RIU_W1_CMD;
    Wdata[1] = u16Addr & 0xFF;
    Wdata[2] = (u16Addr >> 8) & 0xFF;
    Wdata[3] = data;
    //ULOGD("DEMOD","MDrv_SS_RIU_Write\n");
#ifndef MSOS_TYPE_LINUX_KERNEL
    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    
    // send write address & data
    MDrv_MSPI_Write(Wdata,4);
    
    MDrv_MSPI_SlaveEnable(FALSE);
#endif
    return bRet;
}

MS_BOOL MDrv_DMD_SSPI_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata)
{
    MS_BOOL bRet = TRUE;
    MS_U8 Rdata[5];
    
    Rdata[0] = RIU_R1T_CMD;
    Rdata[1] = u16Addr & 0xFF;
    Rdata[2] = (u16Addr >> 8) & 0xFF;
    Rdata[3] = 0x00;
    //ULOGD("DEMOD","MDrv_SS_RIU_Read8\n");
#ifndef MSOS_TYPE_LINUX_KERNEL
    MDrv_MSPI_SlaveEnable(TRUE);
    // send read command to read data
    MDrv_MSPI_Write(Rdata,4);
    // read operation
    //ULOGD("DEMOD","Read Operation\n");
    MDrv_MSPI_Read(pdata, 1);
    MDrv_MSPI_SlaveEnable(FALSE);
#endif
    
    return bRet;

}
