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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>

#include "drvDMD_ATSC.h"

#include "MsTypes.h"
#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
#include "drvDMD_common.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define DMD_ATSC_CHIP_GALILEO       0x01

#if defined(galileo) 
#define DMD_ATSC_CHIP_VERSION       DMD_ATSC_CHIP_GALILEO
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define HAL_EXTERN_ATSC_DBINFO(y)   y

#define REG_IIC_BYPASS       0x0910
#define SRAM_BASE            0x8000
#define REG_MCU_RST          0x0B32
#define ATSC_OUTER_REG_BASE  0x2600

#define LOAD_CODE_I2C_BLOCK_NUM  0x80

#define EXTERN_ATSC_OUTER_STATE           0x80
#define EXTERN_ATSC_VSB_TRAIN_SNR_LIMIT   0x05
#define EXTERN_ATSC_FEC_ENABLE            0x1F

#define VSB_ATSC           0x04
#define QAM256_ATSC        0x02

#define I2C_SLAVE_ID            ((pRes->sDMD_ATSC_InitData.u8I2CSlaveBus<<8) | pRes->sDMD_ATSC_InitData.u8I2CSlaveAddr)

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

const MS_U8 EXTERN_ATSC_table[] = {
    #include "DMD_EXTERN_ATSC.dat"
};

static MS_U16 u16Lib_size = sizeof(EXTERN_ATSC_table);

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

extern MS_U8 u8DMD_ATSC_DMD_ID;

extern DMD_ATSC_ResData *psDMD_ATSC_ResData;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _I2C_READ_BYTE(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_BOOL bRet = TRUE;
    MS_U8 u8MsbData[6] = {0};
    
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 5, u8MsbData);
    bRet &= pRes->sDMD_ATSC_InitData.I2C_ReadBytes(I2C_SLAVE_ID, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, u8MsbData);

    return bRet;
}

static MS_BOOL _I2C_WRITE_BYTE(MS_U16 u16Addr, MS_U8 u8Data)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_BOOL bRet = TRUE;
    MS_U8 u8MsbData[6] = {0};
    
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, u8MsbData);
    u8MsbData[0] = 0x10;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 6, u8MsbData);
    u8MsbData[0] = 0x34;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, u8MsbData);

    return bRet;
}

static MS_BOOL _I2C_WRITE_BYTES(MS_U16 u16Addr, MS_U8 *u8Data, MS_U8 u8Len)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_BOOL bRet = TRUE;
    MS_U16 index;
    MS_U8 Data[0x80+5];

    Data[0] = 0x10;
    Data[1] = 0x00;
    Data[2] = 0x00;
    Data[3] = (u16Addr >> 8) &0xff;
    Data[4] = u16Addr &0xff;

    for(index = 0; index < u8Len ; index++)
    {
         Data[5+index] = u8Data[index];
    }

    Data[0] = 0x35;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, Data);
    Data[0] = 0x10;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, Data);
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, (5 + u8Len), Data);
    Data[0] = 0x34;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, Data);

    return bRet;    
}

static MS_BOOL _I2C_Channel_Set(MS_U8 ch_num)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_BOOL bRet = TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    
    //Exit
    Data[0] = 0x34;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);
    Data[0]=(ch_num & 0x01)? 0x36 : 0x45;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);
    
    //Init
    Data[0] = 0x53;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 5, Data);
    Data[0]=(ch_num & 0x04)? 0x80 : 0x81;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);
    
    if ((ch_num==4)||(ch_num==5)||(ch_num==1))
        Data[0] = 0x82;
    else
        Data[0] = 0x83;
    
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);

    if ((ch_num==4)||(ch_num==5))
        Data[0]=0x85;
    else
        Data[0] = 0x84;

    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);
    Data[0]=(ch_num & 0x01)? 0x51 : 0x53;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);
    Data[0]=(ch_num & 0x01)? 0x37 : 0x7F;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);
    Data[0] = 0x35;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);
    Data[0] = 0x71;
    bRet &= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, NULL, 1, Data);

    return bRet;
}

static MS_BOOL _I2C_Channel_Change(MS_U8 ch_num)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_BOOL bRet = TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    
    Data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet&= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, Data);
    Data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet&= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, Data);
    Data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet&= pRes->sDMD_ATSC_InitData.I2C_WriteBytes(I2C_SLAVE_ID, 0, 0, 1, Data);

    return bRet;
}

static void _HAL_EXTERN_ATSC_InitClk(void)
{
    _I2C_Channel_Set(0);

    _I2C_Channel_Change(3);
}

static MS_BOOL _HAL_EXTERN_ATSC_Download(void)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_BOOL bRet = TRUE;
    MS_U16 index;
    MS_U16 SRAM_Address;
    #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
    MS_U32 SRAM_SSPI_Address;
    #endif
    MS_U8  u8TmpData;
    MS_U16 u16AddressOffset = 0x6B80;
    MS_U8  u8OrgSPISetting = 0x00;
    
    if (pRes->sDMD_ATSC_PriData.bDownloaded)
    {
        // Enable MCU Reset
        bRet&=_I2C_WRITE_BYTE(REG_MCU_RST, 0x03);
        
        // Disable MCU Reset
        bRet&=_I2C_WRITE_BYTE(REG_MCU_RST, 0x00);
    
        #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
        MsOS_DelayTask(20);
        #else
        pRes->sDMD_ATSC_InitData.DelayMS(20);
        #endif
        
        return bRet;
    }
    
    // Enable MCU Reset
    bRet&=_I2C_WRITE_BYTE(REG_MCU_RST, 0x03);
    
    bRet&=_I2C_Channel_Change(0);
    
    // Disable Watch-Dog
    bRet&=_I2C_WRITE_BYTE(0x3008, 0x00);
    bRet&=_I2C_WRITE_BYTE(0x3009, 0x00);
    bRet&=_I2C_WRITE_BYTE(0x300A, 0x00);
    bRet&=_I2C_WRITE_BYTE(0x300B, 0x00);
    
    bRet&=_I2C_Channel_Change(3);
    
    // Enable SRAM XDATA mapping
    bRet&=_I2C_WRITE_BYTE(0x10E1, 0x20); // Start address
    bRet&=_I2C_WRITE_BYTE(0x10E0, 0x3F); // End address
    bRet&=_I2C_WRITE_BYTE(0x10E6, 0x08);
    
    if (pRes->sDMD_ATSC_InitData.bIsUseSspiLoadCode == TRUE)
    {
        #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
        //turn on all pad in
        bRet&=_I2C_WRITE_BYTE(0x0950, 0x00);
        bRet&=_I2C_WRITE_BYTE(0x0951, 0x00);
        
        if (pRes->sDMD_ATSC_InitData.bIsSspiUseTsPin == TRUE)
        {
            printf("##### MSPI USE TS PIN : ENABLE #####\n");
            bRet = _I2C_READ_BYTE(0x0976, &u8TmpData);
            u8OrgSPISetting = u8TmpData;
            u8TmpData &= ~0x03;
            u8TmpData |= 0x02;
            bRet = _I2C_WRITE_BYTE(0x0976, u8TmpData);
        }
        #endif
    }
    
    bRet&=_I2C_Channel_Change(0);
    
    if (pRes->sDMD_ATSC_InitData.bIsUseSspiLoadCode == FALSE)
    {
        for (index = 0; index < u16Lib_size; )
        {
            SRAM_Address = SRAM_BASE + index;
            if (index+LOAD_CODE_I2C_BLOCK_NUM-1<u16Lib_size)
            {
                bRet&= _I2C_WRITE_BYTES(SRAM_Address, (MS_U8 *)(EXTERN_ATSC_table+index), LOAD_CODE_I2C_BLOCK_NUM);
                index=index+LOAD_CODE_I2C_BLOCK_NUM-1;
            }
            else
            {
                bRet&=_I2C_WRITE_BYTES(SRAM_Address, (MS_U8 *)(EXTERN_ATSC_table+index), u16Lib_size-index);
                index=u16Lib_size;
            }
        }
        
        HAL_EXTERN_ATSC_DBINFO(printf("ATSC firmware code size = 0x[%x]\n", u16Lib_size));
        
        if(u16Lib_size >= 0x6b80)
        {
            HAL_EXTERN_ATSC_DBINFO(printf("Firmware code size over 0x6B80!!!\n"));
            bRet = FALSE;
        }
    }
    #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
    else
    {    
        printf("##### _HAL_EXTERN_ATSC_Download by MSPI #####\n");
        
        MDrv_DMD_SSPI_Init(0);
        
        SRAM_SSPI_Address = 0x80000000;
        
        MDrv_DMD_SSPI_RIU_Read8(0x0974, &u8TmpData);
        u8TmpData |= 0x10;
        MDrv_DMD_SSPI_RIU_Write8(0x0974, u8TmpData);
        
        MDrv_DMD_SSPI_MIU_Writes(SRAM_SSPI_Address, (MS_U8 *)EXTERN_ATSC_table, u16Lib_size);
        
        MDrv_DMD_SSPI_RIU_Read8(0x0974, &u8TmpData);
        u8TmpData &= ~0x10;
        MDrv_DMD_SSPI_RIU_Write8(0x0974, u8TmpData);        
    }
    #endif
    
    //Set IF value
    SRAM_Address = SRAM_BASE + u16AddressOffset + 0x0F;//BACKEND_IF_KHZ_0
    u8TmpData = (pRes->sDMD_ATSC_InitData.u16IF_KHZ) & 0xFF;
    bRet&=_I2C_WRITE_BYTE(SRAM_Address, u8TmpData);

    SRAM_Address = SRAM_BASE + u16AddressOffset + 0x10;//BACKEND_IF_KHZ_1
    u8TmpData = (pRes->sDMD_ATSC_InitData.u16IF_KHZ) >> 8;
    bRet&=_I2C_WRITE_BYTE(SRAM_Address, u8TmpData);
                                            
    //Set TS mode
    SRAM_Address = SRAM_BASE + u16AddressOffset + 0x0C;//TS_SERIAL MODE
    u8TmpData = (pRes->sDMD_ATSC_InitData.u5TsConfigByte_DivNum << 3) |
                (pRes->sDMD_ATSC_InitData.u1TsConfigByte_ClockInv << 2) |
                (pRes->sDMD_ATSC_InitData.u1TsConfigByte_DataSwap << 1) |
                (pRes->sDMD_ATSC_InitData.u1TsConfigByte_SerialMode);
    bRet&=_I2C_WRITE_BYTE(SRAM_Address, u8TmpData);
    //Set IQSwap mode
    SRAM_Address = SRAM_BASE + u16AddressOffset + 0x0E;//BACKEND_DMD_IQ_SWAP
    u8TmpData = pRes->sDMD_ATSC_InitData.bIQSwap;
    bRet&=_I2C_WRITE_BYTE(SRAM_Address, u8TmpData);
    
    bRet&=_I2C_Channel_Change(3);
    
    //Set I&Q pad
    if (pRes->sDMD_ATSC_InitData.bIsQPad)
    {
        _I2C_WRITE_BYTE(0x0A33, 0x2E);
        _I2C_WRITE_BYTE(0x0A2E, 0x01);
        
        HAL_EXTERN_ATSC_DBINFO(printf("select Q pad source\n"));
    }
    else
    {
        _I2C_WRITE_BYTE(0x0A33, 0x1E);
        _I2C_WRITE_BYTE(0x0A2E, 0x00);
        
        HAL_EXTERN_ATSC_DBINFO(printf("select I pad source\n"));
    }
    
    if (pRes->sDMD_ATSC_InitData.bIsUseSspiLoadCode == TRUE)
    {
        #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
        if (pRes->sDMD_ATSC_InitData.bIsSspiUseTsPin == TRUE)
        {
            printf("##### MSPI USE TS PIN : DISABLE #####\n");
            bRet = _I2C_READ_BYTE(0x0976, &u8TmpData);
            u8TmpData = u8OrgSPISetting;
            bRet = _I2C_WRITE_BYTE(0x0976, u8TmpData);
        }
        #endif
    }
    
    // Disable SRAM XDATA mapping
    bRet&=_I2C_WRITE_BYTE(0x10E6, 0x00);
    
    // Set program SRAM start address
    bRet&=_I2C_WRITE_BYTE(0x1004, 0x00);
    bRet&=_I2C_WRITE_BYTE(0x1005, 0x00);
    bRet&=_I2C_WRITE_BYTE(0x1000, 0x00);
    
    // Set program SRAM end address
    bRet&=_I2C_WRITE_BYTE(0x1006, 0x7F);
    bRet&=_I2C_WRITE_BYTE(0x1007, 0xFF);
    bRet&=_I2C_WRITE_BYTE(0x1002, 0x00);
    
    // Boot from SRAM
    bRet&=_I2C_WRITE_BYTE(0x1018, 0x01);
    
    // Disable MCU Reset
    bRet&=_I2C_WRITE_BYTE(REG_MCU_RST, 0x00);
    
    pRes->sDMD_ATSC_PriData.bDownloaded = true;
    
    #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
    MsOS_DelayTask(20);
    #else
    pRes->sDMD_ATSC_InitData.DelayMS(20);
    #endif
    
    return TRUE;
}

static void _HAL_EXTERN_ATSC_FWVERSION(void)
{
    MS_U8 data1,data2,data3;
    
    _I2C_READ_BYTE(0x0995, &data1);//REG_BASE[DMDTOP_WR_DBG_04]
    _I2C_READ_BYTE(0x0B54, &data2);//REG_BASE[DMDTOP_WR_DBG_15]
    _I2C_READ_BYTE(0x0B55, &data3);//REG_BASE[DMDTOP_WR_DBG_16]

    HAL_EXTERN_ATSC_DBINFO(printf("EXTERN_ATSC_FW_VERSION:%x.%x.%x\n", data1, data2, data3));
}

static MS_BOOL _HAL_EXTERN_ATSC_Exit(void)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data = 0xFF;
    
    //Reset FSM
    if (_I2C_WRITE_BYTE(0x0990, 0x00)==FALSE) return FALSE;
    
    while (!u8Data)
    {
        if (_I2C_READ_BYTE(0x0992, &u8Data)==FALSE) return FALSE;
    }
    
    // Disable TS pad
    bRet = _I2C_READ_BYTE(0x095A, &u8Data);
    u8Data = 0x00;
    bRet = _I2C_WRITE_BYTE(0x095A, u8Data);
    
    // [0][4] ifagc/rfagc disable
    bRet = _I2C_READ_BYTE(0x0a30, &u8Data);
    u8Data &= ~0x11;
    bRet = _I2C_WRITE_BYTE(0x0a30, u8Data);  
    
    return bRet;
}

static MS_BOOL _HAL_EXTERN_ATSC_SoftReset(void)
{
    MS_U8 u8Data = 0xFF;
    
    //Reset FSM
    if (_I2C_WRITE_BYTE(0x0990, 0x00)==FALSE) return FALSE;
    
    while (u8Data != 0x0)
    {
        if (_I2C_READ_BYTE(0x0992, &u8Data)==FALSE) return FALSE;
    }
    
    //Execute demod top reset
    _I2C_READ_BYTE(0x0B20, &u8Data);
    _I2C_WRITE_BYTE(0x0B20, (u8Data|0x01));
    return _I2C_WRITE_BYTE(0x0B20, (u8Data&(~0x01)));
}

static MS_BOOL _HAL_EXTERN_ATSC_SetVsbMode(void)
{
    return _I2C_WRITE_BYTE(0x0990, 0x08);
}

static MS_BOOL _HAL_EXTERN_ATSC_Set64QamMode(void)
{
    _I2C_WRITE_BYTE(0x0C1B, 0x00); // Clear QAM&SR force mode
    
    if (_I2C_WRITE_BYTE(0x0994, 0x00)==FALSE) return FALSE;
    return _I2C_WRITE_BYTE(0x0990, 0x04);
}

static MS_BOOL _HAL_EXTERN_ATSC_Set256QamMode(void)
{
    _I2C_WRITE_BYTE(0x0C1B, 0x00); // Clear QAM&SR force mode
    
    if (_I2C_WRITE_BYTE(0x0994, 0x01)==FALSE) return FALSE;
    return _I2C_WRITE_BYTE(0x0990, 0x04);
}

static MS_BOOL _HAL_EXTERN_ATSC_SetModeClean(void)
{
    _I2C_WRITE_BYTE(0x0C1B, 0x00); // Clear QAM&SR force mode
    
    return _I2C_WRITE_BYTE(0x0990, 0x00);
}

static MS_BOOL _HAL_EXTERN_ATSC_Set_QAM_SR(void)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_U8 data1,data2,data3;
    
    data1 = (pRes->sDMD_ATSC_PriData.eLastType-1) | 0x80;
    data2 = pRes->sDMD_ATSC_PriData.u16SymRate & 0xFF;
    data3 = (pRes->sDMD_ATSC_PriData.u16SymRate>>8) & 0xFF;
    
    _I2C_WRITE_BYTE(0x0C1B, data1);
    _I2C_WRITE_BYTE(0x0C1C, data2);
    _I2C_WRITE_BYTE(0x0C1D, data3);
    
    printf("QAM type 0x%x SR %d\n",pRes->sDMD_ATSC_PriData.eLastType-1,pRes->sDMD_ATSC_PriData.u16SymRate);
    
    return _I2C_WRITE_BYTE(0x0990, 0x04);
}

static DMD_ATSC_DEMOD_TYPE _HAL_EXTERN_ATSC_Check8VSB64_256QAM(void)
{
    MS_U8 mode=0;

    _I2C_READ_BYTE(0x2900, &mode); //mode check

    if ((mode&VSB_ATSC) == VSB_ATSC) return DMD_ATSC_DEMOD_ATSC_VSB;
    else if ((mode & QAM256_ATSC) == QAM256_ATSC) return DMD_ATSC_DEMOD_ATSC_256QAM;
    else return DMD_ATSC_DEMOD_ATSC_64QAM;
}

static MS_BOOL _HAL_EXTERN_ATSC_Vsb_QAM_AGCLock(void)
{
    MS_U8 data1 = 0;
    
    _I2C_READ_BYTE(0x3E1D, &data1);//AGC_LOCK
    
    if (data1&0x01)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HAL_EXTERN_ATSC_Vsb_PreLock(void)
{
    MS_U8 data1 = 0;
    
    _I2C_READ_BYTE(0x0993, &data1);

    if ((data1&0x02) == 0x02)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HAL_EXTERN_ATSC_Vsb_FSync_Lock(void)
{
    MS_U8 data1 = 0;

    _I2C_READ_BYTE(0x2824, &data1);

    if ((data1&0x10) == 0x10)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HAL_EXTERN_ATSC_Vsb_CE_Lock(void)
{
    MS_U8 data1 = 0;
    
    if((data1&0x10) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HAL_EXTERN_ATSC_Vsb_FEC_Lock(void)
{
    MS_U8 data1 = 0, data2 = 0, data3 = 0, data4 = 0, data5 = 0;
    MS_U8 data6 = 0, data7 = 0;

    _I2C_READ_BYTE(0x0992, &data1);
    _I2C_READ_BYTE(0x1917, &data2);//AD_NOISE_PWR_TRAIN1
    _I2C_READ_BYTE(0x0993, &data3);
    _I2C_READ_BYTE(0x2601, &data4);//FEC_EN_CTL
    _I2C_READ_BYTE(0x2767, &data5);//EQEXT_REG_BASE//addy
    _I2C_READ_BYTE(0x0D01, &data6);
    _I2C_READ_BYTE(0x0D40, &data7);

	if (data1==EXTERN_ATSC_OUTER_STATE && (data2<=EXTERN_ATSC_VSB_TRAIN_SNR_LIMIT || data5 <= EXTERN_ATSC_VSB_TRAIN_SNR_LIMIT) &&
	    (data3&0x02)==0x02 && (data4&EXTERN_ATSC_FEC_ENABLE)==EXTERN_ATSC_FEC_ENABLE && ((data6&0x10) == 0x10) && ((data7&0x01) == 0x01))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HAL_EXTERN_ATSC_QAM_PreLock(void)
{
    MS_U8 data1=0;
    
    _I2C_READ_BYTE(0x1415, &data1);//TR_LOCK
    
    if((data1&0x10) == 0x10)
    {
        HAL_EXTERN_ATSC_DBINFO(printf("    QAM preLock OK  \n"));
        return TRUE;
    }
    else
    {
        HAL_EXTERN_ATSC_DBINFO(printf("    QAM preLock NOT OK   \n"));
        return FALSE;
    }
}

static MS_BOOL _HAL_EXTERN_ATSC_QAM_Main_Lock(void)
{
    MS_U8 data1 = 0, data4 = 0, data5 = 0, data6 = 0;
    MS_U8 data2 = 0, data3 = 0;

    _I2C_READ_BYTE(0x0992, &data1);
    _I2C_READ_BYTE(0x2601, &data4);//FEC_EN_CTL
    _I2C_READ_BYTE(0x0D01, &data5);
    _I2C_READ_BYTE(0x0D40, &data6);
    _I2C_READ_BYTE(0x1415, &data3);//TR_LOCK
    _I2C_READ_BYTE(0x2618, &data2);//boundary detected
    
    if (data1==EXTERN_ATSC_OUTER_STATE && (data2&0x01)==0x01 &&
        data4==EXTERN_ATSC_FEC_ENABLE && (data3&0x10)==0x10 &&
        ((data5&0x10) == 0x10) && ((data6&0x01) == 0x01))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_U8 _HAL_EXTERN_ATSC_ReadIFAGC(void)
{
    MS_U8 data = 0;
    
    _I2C_READ_BYTE(0x13FD, &data);

    return data;
}

static void _HAL_EXTERN_ATSC_CheckSignalCondition(DMD_ATSC_SIGNAL_CONDITION* pstatus)
{
    DMD_ATSC_DEMOD_TYPE eMode;
    MS_U8 u8NoisePowerH=0;
    static MS_U8 u8NoisePowerH_Last = 0xff;

    eMode = _HAL_EXTERN_ATSC_Check8VSB64_256QAM();
    
    _I2C_READ_BYTE(0x1915, &u8NoisePowerH);

    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB) //VSB mode//SNR=10*log10((1344<<10)/noisepower)
    {
        if (!_HAL_EXTERN_ATSC_Vsb_FEC_Lock()) u8NoisePowerH=0xFF;
        else if (abs(u8NoisePowerH_Last-u8NoisePowerH) > 5)
            u8NoisePowerH_Last = u8NoisePowerH;
        else u8NoisePowerH = u8NoisePowerH_Last;

        if (u8NoisePowerH > 0xBE) //SNR<14.5
            *pstatus=DMD_ATSC_SIGNAL_NO;
        else if (u8NoisePowerH > 0x4D) //SNR<18.4
            *pstatus=DMD_ATSC_SIGNAL_WEAK;
        else if (u8NoisePowerH > 0x23) //SNR<21.8
            *pstatus=DMD_ATSC_SIGNAL_MODERATE;
        else if (u8NoisePowerH > 0x0A) //SNR<26.9
            *pstatus=DMD_ATSC_SIGNAL_STRONG;
        else
            *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
    }
    else //QAM MODE
    {
        if (!_HAL_EXTERN_ATSC_QAM_Main_Lock()) u8NoisePowerH=0xFF;
        else if (abs(u8NoisePowerH_Last-u8NoisePowerH) > 5)
            u8NoisePowerH_Last = u8NoisePowerH;
        else u8NoisePowerH = u8NoisePowerH_Last;

        if (eMode == DMD_ATSC_DEMOD_ATSC_256QAM) //256QAM//SNR=10*log10((2720<<10)/noisepower)
        {
            if (u8NoisePowerH > 0x13) //SNR<27.5
                *pstatus=DMD_ATSC_SIGNAL_NO;
            else if (u8NoisePowerH > 0x08) //SNR<31.2
                *pstatus=DMD_ATSC_SIGNAL_WEAK;
            else if (u8NoisePowerH > 0x06) //SNR<32.4
                *pstatus=DMD_ATSC_SIGNAL_MODERATE;
            else if (u8NoisePowerH > 0x04) //SNR<34.2
                *pstatus=DMD_ATSC_SIGNAL_STRONG;
            else
                *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
        }
        else //64QAM//SNR=10*log10((2688<<10)/noisepower)
        {
            if (u8NoisePowerH > 0x4C) //SNR<21.5
                *pstatus=DMD_ATSC_SIGNAL_NO;
            else if (u8NoisePowerH > 0x1F) //SNR<25.4
                *pstatus=DMD_ATSC_SIGNAL_WEAK;
            else if (u8NoisePowerH > 0x11) //SNR<27.8
                *pstatus=DMD_ATSC_SIGNAL_MODERATE;
            else if (u8NoisePowerH > 0x07) //SNR<31.4
                *pstatus=DMD_ATSC_SIGNAL_STRONG;
            else
                *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
        }
    }
}

static MS_U8 _HAL_EXTERN_ATSC_ReadSNRPercentage(void)
{
    DMD_ATSC_DEMOD_TYPE eMode;
    MS_U8 u8NoisePowerH = 0, u8NoisePowerL = 0;
    MS_U16 u16NoisePower;

    eMode = _HAL_EXTERN_ATSC_Check8VSB64_256QAM();
    
    _I2C_READ_BYTE(0x1914, &u8NoisePowerL);
    _I2C_READ_BYTE(0x1915, &u8NoisePowerH);

    u16NoisePower = (u8NoisePowerH<<8) | u8NoisePowerL;

    if(eMode == DMD_ATSC_DEMOD_ATSC_VSB) //VSB mode//SNR=10*log10((1344<<10)/noisepower)
    {
        if (!_HAL_EXTERN_ATSC_Vsb_FEC_Lock())
            return 0;//SNR=0;
        else if (u16NoisePower<=0x008A)//SNR>=40dB
            return 100;//SNR=MAX_SNR;
        else if (u16NoisePower<=0x0097)//SNR>=39.6dB
            return 99;//
        else if (u16NoisePower<=0x00A5)//SNR>=39.2dB
            return 98;//
        else if (u16NoisePower<=0x00B5)//SNR>=38.8dB
            return 97;//
        else if (u16NoisePower<=0x00C7)//SNR>=38.4dB
            return 96;//
        else if (u16NoisePower<=0x00DA)//SNR>=38.0dB
            return 95;//
        else if (u16NoisePower<=0x00EF)//SNR>=37.6dB
            return 94;//
        else if (u16NoisePower<=0x0106)//SNR>=37.2dB
            return 93;//
        else if (u16NoisePower<=0x0120)//SNR>=36.8dB
            return 92;//
        else if (u16NoisePower<=0x013B)//SNR>=36.4dB
            return 91;//
        else if (u16NoisePower<=0x015A)//SNR>=36.0dB
            return 90;//
        else if (u16NoisePower<=0x017B)//SNR>=35.6dB
            return 89;//
        else if (u16NoisePower<=0x01A0)//SNR>=35.2dB
            return 88;//
        else if (u16NoisePower<=0x01C8)//SNR>=34.8dB
            return 87;//
        else if (u16NoisePower<=0x01F4)//SNR>=34.4dB
            return 86;//
        else if (u16NoisePower<=0x0224)//SNR>=34.0dB
            return 85;//
        else if (u16NoisePower<=0x0259)//SNR>=33.6dB
            return 84;//
        else if (u16NoisePower<=0x0293)//SNR>=33.2dB
            return 83;//
        else if (u16NoisePower<=0x02D2)//SNR>=32.8dB
            return 82;//
        else if (u16NoisePower<=0x0318)//SNR>=32.4dB
            return 81;//
        else if (u16NoisePower<=0x0364)//SNR>=32.0dB
            return 80;//
        else if (u16NoisePower<=0x03B8)//SNR>=31.6dB
            return 79;//
        else if (u16NoisePower<=0x0414)//SNR>=31.2dB
            return 78;//
        else if (u16NoisePower<=0x0479)//SNR>=30.8dB
            return 77;//
        else if (u16NoisePower<=0x04E7)//SNR>=30.4dB
            return 76;//
        else if (u16NoisePower<=0x0560)//SNR>=30.0dB
            return 75;//
        else if (u16NoisePower<=0x05E5)//SNR>=29.6dB
            return 74;//
        else if (u16NoisePower<=0x0677)//SNR>=29.2dB
            return 73;//
        else if (u16NoisePower<=0x0716)//SNR>=28.8dB
            return 72;//
        else if (u16NoisePower<=0x07C5)//SNR>=28.4dB
            return 71;//
        else if (u16NoisePower<=0x0885)//SNR>=28.0dB
            return 70;//
        else if (u16NoisePower<=0x0958)//SNR>=27.6dB
            return 69;//
        else if (u16NoisePower<=0x0A3E)//SNR>=27.2dB
            return 68;//
        else if (u16NoisePower<=0x0B3B)//SNR>=26.8dB
            return 67;//
        else if (u16NoisePower<=0x0C51)//SNR>=26.4dB
            return 66;//
        else if (u16NoisePower<=0x0D81)//SNR>=26.0dB
            return 65;//
        else if (u16NoisePower<=0x0ECF)//SNR>=25.6dB
            return 64;//
        else if (u16NoisePower<=0x103C)//SNR>=25.2dB
            return 63;//
        else if (u16NoisePower<=0x11CD)//SNR>=24.8dB
            return 62;//
        else if (u16NoisePower<=0x1385)//SNR>=24.4dB
            return 61;//
        else if (u16NoisePower<=0x1567)//SNR>=24.0dB
            return 60;//
        else if (u16NoisePower<=0x1778)//SNR>=23.6dB
            return 59;//
        else if (u16NoisePower<=0x19BB)//SNR>=23.2dB
            return 58;//
        else if (u16NoisePower<=0x1C37)//SNR>=22.8dB
            return 57;//
        else if (u16NoisePower<=0x1EF0)//SNR>=22.4dB
            return 56;//
        else if (u16NoisePower<=0x21EC)//SNR>=22.0dB
            return 55;//
        else if (u16NoisePower<=0x2531)//SNR>=21.6dB
            return 54;//
        else if (u16NoisePower<=0x28C8)//SNR>=21.2dB
            return 53;//
        else if (u16NoisePower<=0x2CB7)//SNR>=20.8dB
            return 52;//
        else if (u16NoisePower<=0x3108)//SNR>=20.4dB
            return 51;//
        else if (u16NoisePower<=0x35C3)//SNR>=20.0dB
            return 50;//
        else if (u16NoisePower<=0x3AF2)//SNR>=19.6dB
            return 49;//
        else if (u16NoisePower<=0x40A2)//SNR>=19.2dB
            return 48;//
        else if (u16NoisePower<=0x46DF)//SNR>=18.8dB
            return 47;//
        else if (u16NoisePower<=0x4DB5)//SNR>=18.4dB
            return 46;//
        else if (u16NoisePower<=0x5534)//SNR>=18.0dB
            return 45;//
        else if (u16NoisePower<=0x5D6D)//SNR>=17.6dB
            return 44;//
        else if (u16NoisePower<=0x6670)//SNR>=17.2dB
            return 43;//
        else if (u16NoisePower<=0x7052)//SNR>=16.8dB
            return 42;//
        else if (u16NoisePower<=0x7B28)//SNR>=16.4dB
            return 41;//
        else if (u16NoisePower<=0x870A)//SNR>=16.0dB
            return 40;//
        else if (u16NoisePower<=0x9411)//SNR>=15.6dB
            return 39;//
        else if (u16NoisePower<=0xA25A)//SNR>=15.2dB
            return 38;//
        else if (u16NoisePower<=0xB204)//SNR>=14.8dB
            return 37;//
        else if (u16NoisePower<=0xC331)//SNR>=14.4dB
            return 36;//
        else if (u16NoisePower<=0xD606)//SNR>=14.0dB
            return 35;//
        else if (u16NoisePower<=0xEAAC)//SNR>=13.6dB
            return 34;//
        else// if (u16NoisePower>=0xEAAC)//SNR<13.6dB
            return 33;//
    }
    else //QAM MODE
    {
        if(eMode == DMD_ATSC_DEMOD_ATSC_256QAM) //256QAM//SNR=10*log10((2720<<10)/noisepower)
        {
            if (!_HAL_EXTERN_ATSC_QAM_Main_Lock())
                return 0;//SNR=0;
            else if (u16NoisePower<=0x0117)//SNR>=40dB
                return 100;//
            else if (u16NoisePower<=0x0131)//SNR>=39.6dB
                return 99;//
            else if (u16NoisePower<=0x014F)//SNR>=39.2dB
                return 98;//
            else if (u16NoisePower<=0x016F)//SNR>=38.8dB
                return 97;//
            else if (u16NoisePower<=0x0193)//SNR>=38.4dB
                return 96;//
            else if (u16NoisePower<=0x01B9)//SNR>=38.0dB
                return 95;//
            else if (u16NoisePower<=0x01E4)//SNR>=37.6dB
                return 94;//
            else if (u16NoisePower<=0x0213)//SNR>=37.2dB
                return 93;//
            else if (u16NoisePower<=0x0246)//SNR>=36.8dB
                return 92;//
            else if (u16NoisePower<=0x027E)//SNR>=36.4dB
                return 91;//
            else if (u16NoisePower<=0x02BC)//SNR>=36.0dB
                return 90;//
            else if (u16NoisePower<=0x02FF)//SNR>=35.6dB
                return 89;//
            else if (u16NoisePower<=0x0349)//SNR>=35.2dB
                return 88;//
            else if (u16NoisePower<=0x039A)//SNR>=34.8dB
                return 87;//
            else if (u16NoisePower<=0x03F3)//SNR>=34.4dB
                return 86;//
            else if (u16NoisePower<=0x0455)//SNR>=34.0dB
                return 85;//
            else if (u16NoisePower<=0x04C0)//SNR>=33.6dB
                return 84;//
            else if (u16NoisePower<=0x0535)//SNR>=33.2dB
                return 83;//
            else if (u16NoisePower<=0x05B6)//SNR>=32.8dB
                return 82;//
            else if (u16NoisePower<=0x0643)//SNR>=32.4dB
                return 81;//
            else if (u16NoisePower<=0x06DD)//SNR>=32.0dB
                return 80;//
            else if (u16NoisePower<=0x0787)//SNR>=31.6dB
                return 79;//
            else if (u16NoisePower<=0x0841)//SNR>=31.2dB
                return 78;//
            else if (u16NoisePower<=0x090D)//SNR>=30.8dB
                return 77;//
            else if (u16NoisePower<=0x09EC)//SNR>=30.4dB
                return 76;//
            else if (u16NoisePower<=0x0AE1)//SNR>=30.0dB
                return 75;//
            else if (u16NoisePower<=0x0BEE)//SNR>=29.6dB
                return 74;//
            else if (u16NoisePower<=0x0D15)//SNR>=29.2dB
                return 73;//
            else if (u16NoisePower<=0x0E58)//SNR>=28.8dB
                return 72;//
            else if (u16NoisePower<=0x0FBA)//SNR>=28.4dB
                return 71;//
            else if (u16NoisePower<=0x113E)//SNR>=28.0dB
                return 70;//
            else if (u16NoisePower<=0x12E8)//SNR>=27.6dB
                return 69;//
            else if (u16NoisePower<=0x14BB)//SNR>=27.2dB
                return 68;//
            else if (u16NoisePower<=0x16BB)//SNR>=26.8dB
                return 67;//
            else if (u16NoisePower<=0x18ED)//SNR>=26.4dB
                return 66;//
            else if (u16NoisePower<=0x1B54)//SNR>=26.0dB
                return 65;//
            else if (u16NoisePower<=0x1DF7)//SNR>=25.6dB
                return 64;//
            else if (u16NoisePower<=0x20DB)//SNR>=25.2dB
                return 63;//
            else if (u16NoisePower<=0x2407)//SNR>=24.8dB
                return 62;//
            else if (u16NoisePower<=0x2781)//SNR>=24.4dB
                return 61;//
            else if (u16NoisePower<=0x2B50)//SNR>=24.0dB
                return 60;//
            else if (u16NoisePower<=0x2F7E)//SNR>=23.6dB
                return 59;//
            else if (u16NoisePower<=0x3413)//SNR>=23.2dB
                return 58;//
            else if (u16NoisePower<=0x3919)//SNR>=22.8dB
                return 57;//
            else if (u16NoisePower<=0x3E9C)//SNR>=22.4dB
                return 56;//
            else if (u16NoisePower<=0x44A6)//SNR>=22.0dB
                return 55;//
            else if (u16NoisePower<=0x4B45)//SNR>=21.6dB
                return 54;//
            else if (u16NoisePower<=0x5289)//SNR>=21.2dB
                return 53;//
            else if (u16NoisePower<=0x5A7F)//SNR>=20.8dB
                return 52;//
            else if (u16NoisePower<=0x633A)//SNR>=20.4dB
                return 51;//
            else if (u16NoisePower<=0x6CCD)//SNR>=20.0dB
                return 50;//
            else if (u16NoisePower<=0x774C)//SNR>=19.6dB
                return 49;//
            else if (u16NoisePower<=0x82CE)//SNR>=19.2dB
                return 48;//
            else if (u16NoisePower<=0x8F6D)//SNR>=18.8dB
                return 47;//
            else if (u16NoisePower<=0x9D44)//SNR>=18.4dB
                return 46;//
            else if (u16NoisePower<=0xAC70)//SNR>=18.0dB
                return 45;//
            else if (u16NoisePower<=0xBD13)//SNR>=17.6dB
                return 44;//
            else if (u16NoisePower<=0xCF50)//SNR>=17.2dB
                return 43;//
            else if (u16NoisePower<=0xE351)//SNR>=16.8dB
                return 42;//
            else if (u16NoisePower<=0xF93F)//SNR>=16.4dB
                return 41;//
            else// if (u16NoisePower>=0xF93F)//SNR<16.4dB
                return 40;//
        }
        else //64QAM//SNR=10*log10((2688<<10)/noisepower)
        {
            if (!_HAL_EXTERN_ATSC_QAM_Main_Lock())
                return 0;//SNR=0;
            else if (u16NoisePower<=0x0113)//SNR>=40dB
                return 100;//
            else if (u16NoisePower<=0x012E)//SNR>=39.6dB
                return 99;//
            else if (u16NoisePower<=0x014B)//SNR>=39.2dB
                return 98;//
            else if (u16NoisePower<=0x016B)//SNR>=38.8dB
                return 97;//
            else if (u16NoisePower<=0x018E)//SNR>=38.4dB
                return 96;//
            else if (u16NoisePower<=0x01B4)//SNR>=38.0dB
                return 95;//
            else if (u16NoisePower<=0x01DE)//SNR>=37.6dB
                return 94;//
            else if (u16NoisePower<=0x020C)//SNR>=37.2dB
                return 93;//
            else if (u16NoisePower<=0x023F)//SNR>=36.8dB
                return 92;//
            else if (u16NoisePower<=0x0277)//SNR>=36.4dB
                return 91;//
            else if (u16NoisePower<=0x02B3)//SNR>=36.0dB
                return 90;//
            else if (u16NoisePower<=0x02F6)//SNR>=35.6dB
                return 89;//
            else if (u16NoisePower<=0x033F)//SNR>=35.2dB
                return 88;//
            else if (u16NoisePower<=0x038F)//SNR>=34.8dB
                return 87;//
            else if (u16NoisePower<=0x03E7)//SNR>=34.4dB
                return 86;//
            else if (u16NoisePower<=0x0448)//SNR>=34.0dB
                return 85;//
            else if (u16NoisePower<=0x04B2)//SNR>=33.6dB
                return 84;//
            else if (u16NoisePower<=0x0525)//SNR>=33.2dB
                return 83;//
            else if (u16NoisePower<=0x05A5)//SNR>=32.8dB
                return 82;//
            else if (u16NoisePower<=0x0630)//SNR>=32.4dB
                return 81;//
            else if (u16NoisePower<=0x06C9)//SNR>=32.0dB
                return 80;//
            else if (u16NoisePower<=0x0770)//SNR>=31.6dB
                return 79;//
            else if (u16NoisePower<=0x0828)//SNR>=31.2dB
                return 78;//
            else if (u16NoisePower<=0x08F1)//SNR>=30.8dB
                return 77;//
            else if (u16NoisePower<=0x09CE)//SNR>=30.4dB
                return 76;//
            else if (u16NoisePower<=0x0AC1)//SNR>=30.0dB
                return 75;//
            else if (u16NoisePower<=0x0BCA)//SNR>=29.6dB
                return 74;//
            else if (u16NoisePower<=0x0CED)//SNR>=29.2dB
                return 73;//
            else if (u16NoisePower<=0x0E2D)//SNR>=28.8dB
                return 72;//
            else if (u16NoisePower<=0x0F8B)//SNR>=28.4dB
                return 71;//
            else if (u16NoisePower<=0x110A)//SNR>=28.0dB
                return 70;//
            else if (u16NoisePower<=0x12AF)//SNR>=27.6dB
                return 69;//
            else if (u16NoisePower<=0x147D)//SNR>=27.2dB
                return 68;//
            else if (u16NoisePower<=0x1677)//SNR>=26.8dB
                return 67;//
            else if (u16NoisePower<=0x18A2)//SNR>=26.4dB
                return 66;//
            else if (u16NoisePower<=0x1B02)//SNR>=26.0dB
                return 65;//
            else if (u16NoisePower<=0x1D9D)//SNR>=25.6dB
                return 64;//
            else if (u16NoisePower<=0x2078)//SNR>=25.2dB
                return 63;//
            else if (u16NoisePower<=0x239A)//SNR>=24.8dB
                return 62;//
            else if (u16NoisePower<=0x270A)//SNR>=24.4dB
                return 61;//
            else if (u16NoisePower<=0x2ACE)//SNR>=24.0dB
                return 60;//
            else if (u16NoisePower<=0x2EEF)//SNR>=23.6dB
                return 59;//
            else if (u16NoisePower<=0x3376)//SNR>=23.2dB
                return 58;//
            else if (u16NoisePower<=0x386D)//SNR>=22.8dB
                return 57;//
            else if (u16NoisePower<=0x3DDF)//SNR>=22.4dB
                return 56;//
            else if (u16NoisePower<=0x43D7)//SNR>=22.0dB
                return 55;//
            else if (u16NoisePower<=0x4A63)//SNR>=21.6dB
                return 54;//
            else if (u16NoisePower<=0x5190)//SNR>=21.2dB
                return 53;//
            else if (u16NoisePower<=0x596E)//SNR>=20.8dB
                return 52;//
            else if (u16NoisePower<=0x620F)//SNR>=20.4dB
                return 51;//
            else if (u16NoisePower<=0x6B85)//SNR>=20.0dB
                return 50;//
            else if (u16NoisePower<=0x75E5)//SNR>=19.6dB
                return 49;//
            else if (u16NoisePower<=0x8144)//SNR>=19.2dB
                return 48;//
            else if (u16NoisePower<=0x8DBD)//SNR>=18.8dB
                return 47;//
            else if (u16NoisePower<=0x9B6A)//SNR>=18.4dB
                return 46;//
            else if (u16NoisePower<=0xAA68)//SNR>=18.0dB
                return 45;//
            else if (u16NoisePower<=0xBAD9)//SNR>=17.6dB
                return 44;//
            else if (u16NoisePower<=0xCCE0)//SNR>=17.2dB
                return 43;//
            else if (u16NoisePower<=0xE0A4)//SNR>=16.8dB
                return 42;//
            else if (u16NoisePower<=0xF650)//SNR>=16.4dB
                return 41;//
            else// if (u16NoisePower>=0xF650)//SNR<16.4dB
                return 40;//
        }
    }
}

static MS_U16 _HAL_EXTERN_ATSC_ReadPKTERR(void)
{
    MS_U16 data = 0;
    MS_U8 reg = 0, reg_frz = 0;
    DMD_ATSC_DEMOD_TYPE eMode;

    eMode = _HAL_EXTERN_ATSC_Check8VSB64_256QAM();
    
    _I2C_READ_BYTE(0x0D03, &reg_frz);
    _I2C_WRITE_BYTE(0x0D03, reg_frz|0x03);

    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB)
    {
        if (!_HAL_EXTERN_ATSC_Vsb_FEC_Lock()) data = 0;
        else
        {
            _I2C_READ_BYTE(0x0D67, &reg);
            data = reg;
            _I2C_READ_BYTE(0x0D66, &reg);
            data = (data << 8) | reg;
        }
    }
    else
    {
        if (!_HAL_EXTERN_ATSC_QAM_Main_Lock()) data = 0;
        else
        {
            _I2C_READ_BYTE(0x0D67, &reg);
            data = reg;
            _I2C_READ_BYTE(0x0D66, &reg);
            data = (data << 8) | reg;
        }
    }
    
    reg_frz=reg_frz&(~0x03);
    _I2C_WRITE_BYTE(0x0D03, reg_frz);

    return data;
}

static MS_BOOL _HAL_EXTERN_ATSC_ReadBER(float *pBer)
{
    MS_BOOL status = true;
    MS_U8 reg = 0, reg_frz = 0;
    MS_U16 BitErrPeriod;
    MS_U32 BitErr;
    DMD_ATSC_DEMOD_TYPE eMode;

    eMode = _HAL_EXTERN_ATSC_Check8VSB64_256QAM();

    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB)
    {
        if (!_HAL_EXTERN_ATSC_Vsb_FEC_Lock()) *pBer = 0;
        else
        {
            // bank 0D 0x03 [1:0] reg_bit_err_num_freeze
            _I2C_READ_BYTE(0x0D03, &reg_frz);
            _I2C_WRITE_BYTE(0x0D03, reg_frz|0x03);
            
            // bank 0D 0x46 [7:0] reg_bit_err_sblprd_7_0
            //         0x47 [15:8] reg_bit_err_sblprd_15_8
            _I2C_READ_BYTE(0x0D47, &reg);
            BitErrPeriod = reg;
            _I2C_READ_BYTE(0x0D46, &reg);
            BitErrPeriod = (BitErrPeriod << 8)|reg;
            
            // bank 0D 0x6a [7:0] reg_bit_err_num_7_0
            //         0x6b [15:8] reg_bit_err_num_15_8
            // bank 0D 0x6c [7:0] reg_bit_err_num_23_16
            //         0x6d [15:8] reg_bit_err_num_31_24
            status &= _I2C_READ_BYTE(0x0D6d, &reg);
            BitErr = reg;
            status &= _I2C_READ_BYTE(0x0D6c, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _I2C_READ_BYTE(0x0D6b, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _I2C_READ_BYTE(0x0D6a, &reg);
            BitErr = (BitErr << 8)|reg;
            
            // bank 0D 0x03 [1:0] reg_bit_err_num_freeze
            reg_frz=reg_frz&(~0x03);
            _I2C_WRITE_BYTE(0x0D03, reg_frz);
            
            if (BitErrPeriod == 0 )    //protect 0
                BitErrPeriod = 1;
            if (BitErr <=0 )
                *pBer = 0.5f / ((float)BitErrPeriod*8*187*128);
            else
                *pBer = (float)BitErr / ((float)BitErrPeriod*8*187*128);
        }
    }
    else
    {
        if (!_HAL_EXTERN_ATSC_QAM_Main_Lock()) *pBer = 0;
        else
        {
            // bank 0D 0x03 [1:0] reg_bit_err_num_freeze
            _I2C_READ_BYTE(0x0D03, &reg_frz);
            _I2C_WRITE_BYTE(0x0D03, reg_frz|0x03);
            
            // bank 0D 0x46 [7:0] reg_bit_err_sblprd_7_0
            //         0x47 [15:8] reg_bit_err_sblprd_15_8
            _I2C_READ_BYTE(0x0D47, &reg);
            BitErrPeriod = reg;
            _I2C_READ_BYTE(0x0D46, &reg);
            BitErrPeriod = (BitErrPeriod << 8)|reg;
            
            // bank 0D 0x6a [7:0] reg_bit_err_num_7_0
            //         0x6b [15:8] reg_bit_err_num_15_8
            // bank 0D 0x6c [7:0] reg_bit_err_num_23_16
            //         0x6d [15:8] reg_bit_err_num_31_24
            status &= _I2C_READ_BYTE(0x0D6d, &reg);
            BitErr = reg;
            status &= _I2C_READ_BYTE(0x0D6c, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _I2C_READ_BYTE(0x0D6b, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _I2C_READ_BYTE(0x0D6a, &reg);
            BitErr = (BitErr << 8)|reg;
            
            // bank 0D 0x03 [1:0] reg_bit_err_num_freeze
            reg_frz=reg_frz&(~0x03);
            _I2C_WRITE_BYTE(0x0D03, reg_frz);
            
            if (BitErrPeriod == 0 )    //protect 0
                BitErrPeriod = 1;
            if (BitErr <=0 )
                *pBer = 0.5f / ((float)BitErrPeriod*7*122*128);
            else
                *pBer = (float)BitErr / ((float)BitErrPeriod*7*122*128);
        }
    }
    
    return status;
}

static MS_S16 _HAL_EXTERN_ATSC_ReadFrequencyOffset(void)
{
    DMD_ATSC_DEMOD_TYPE eMode;
    MS_U8 u8PTK_LOOP_FF_R3=0, u8PTK_LOOP_FF_R2=0;
    MS_U8 u8PTK_RATE_2=0;
    MS_U8 u8AD_CRL_LOOP_VALUE0=0, u8AD_CRL_LOOP_VALUE1=0;
    MS_U8 u8MIX_RATE_0=0, u8MIX_RATE_1=0, u8MIX_RATE_2=0;
    MS_S16 PTK_LOOP_FF;
    MS_S16 AD_CRL_LOOP_VALUE;
    MS_S16 MIX_RATE;
    MS_S16 FreqOffset = 0; //kHz
    
    eMode = _HAL_EXTERN_ATSC_Check8VSB64_256QAM();
    
    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB) //VSB mode//
    {
        _I2C_WRITE_BYTE(0x297E, 0x01);
        _I2C_WRITE_BYTE(0x29E6, 0xff);
        _I2C_READ_BYTE(0x297C, &u8PTK_LOOP_FF_R2);
        _I2C_READ_BYTE(0x297D, &u8PTK_LOOP_FF_R3);
        _I2C_WRITE_BYTE(0x297E, 0x00);
        _I2C_WRITE_BYTE(0x29E6, 0xff);
        
        PTK_LOOP_FF = (u8PTK_LOOP_FF_R3<<8) | u8PTK_LOOP_FF_R2;
        FreqOffset  = (float)(-PTK_LOOP_FF*0.04768);
        
        _I2C_READ_BYTE(0x2982, &u8PTK_RATE_2);
        
        if (u8PTK_RATE_2 == 0x07)
            FreqOffset = FreqOffset-100;
        else if (u8PTK_RATE_2 == 0x08)
            FreqOffset = FreqOffset-500;
    }
    else //QAM MODE
    {
        _I2C_READ_BYTE(0x2C04, &u8AD_CRL_LOOP_VALUE0);
        _I2C_READ_BYTE(0x2C05, &u8AD_CRL_LOOP_VALUE1);
        
        AD_CRL_LOOP_VALUE = (u8AD_CRL_LOOP_VALUE1<<8) | u8AD_CRL_LOOP_VALUE0;
        
        _I2C_READ_BYTE(0x2904, &u8MIX_RATE_0);
        _I2C_READ_BYTE(0x2905, &u8MIX_RATE_1);
        _I2C_READ_BYTE(0x2906, &u8MIX_RATE_2);
        
        MIX_RATE = (u8MIX_RATE_2<<12)|(u8MIX_RATE_1<<4)|(u8MIX_RATE_0>>4);
        
        if (eMode == DMD_ATSC_DEMOD_ATSC_256QAM) //256QAM//
        {
            FreqOffset = (float)(AD_CRL_LOOP_VALUE*0.0025561); //5.360537E6/2^21*1000
        }
        else if (eMode == DMD_ATSC_DEMOD_ATSC_64QAM)//64QAM//
        {
            FreqOffset = (float)(AD_CRL_LOOP_VALUE*0.00241134); //5.056941E6/2^21*1000
        }
        
        FreqOffset = FreqOffset+(float)(MIX_RATE-0x3D70)/2.62144; //(0.001/25*2^20/16)
    }
    
    return FreqOffset;
}

static MS_BOOL _HAL_EXTERN_ATSC_SetSerialControl(MS_U8 u8TsConfigData)
{
    return TRUE;
}

static MS_BOOL _HAL_EXTERN_ATSC_IIC_Bypass_Mode(MS_BOOL bEnable)
{
    MS_U8 u8Retry = 10;
    MS_BOOL bRet = FALSE;

    while ((u8Retry--) && (bRet == FALSE))
    {
        if (bEnable)
        {
            bRet = _I2C_WRITE_BYTE(REG_IIC_BYPASS, 0x10);// IIC by-pass mode on
        }
        else
        {
            bRet = _I2C_WRITE_BYTE(REG_IIC_BYPASS, 0x00);// IIC by-pass mode off
        }

    }
    
    return bRet;
}

static MS_BOOL _HAL_EXTERN_ATSC_SW_SSPI_GPIO(MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data;

    if(bEnable == TRUE) //switch to GPIO
    {
        bRet = _I2C_READ_BYTE(0x0951, &u8Data);
        u8Data &= ~0x01;
        bRet = _I2C_WRITE_BYTE(0x0951, u8Data);   // [8] reg_turn_off_pad = 0 
    
    
        bRet = _I2C_READ_BYTE(0x0976, &u8Data);
        u8Data &= ~0x01;
        bRet = _I2C_WRITE_BYTE(0x0976, u8Data);   // [0] reg_en_sspi_pad = 0 
    }
    else
    {
        bRet = _I2C_READ_BYTE(0x0951, &u8Data);
        u8Data |= 0x01;
        bRet = _I2C_WRITE_BYTE(0x0951, u8Data);   // [8] reg_turn_off_pad = 1 
    
    
        bRet = _I2C_READ_BYTE(0x0976, &u8Data);
        u8Data |= 0x01;
        bRet = _I2C_WRITE_BYTE(0x0976, u8Data);   // [0] reg_en_sspi_pad = 1 
    }

    return bRet; 
}

static MS_BOOL _HAL_EXTERN_ATSC_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data;

    bRet = _I2C_READ_BYTE(0x09CA, &u8Data);
    if (u8Pin == 0)
    {
        if ((u8Data & 0x01) == 0x01)
            *bLevel = TRUE;
        else
            *bLevel = FALSE;
    }
    else if(u8Pin == 1)
    {
        if ((u8Data & 0x02) == 0x02)
            *bLevel = TRUE;
        else
            *bLevel = FALSE;
    }
    else if(u8Pin == 2)
    {
        if ((u8Data & 0x04) == 0x04)
            *bLevel = TRUE;
        else
            *bLevel = FALSE;
    }
    else if(u8Pin == 5)
    {
        if ((u8Data & 0x20) == 0x20)
            *bLevel = TRUE;
        else
            *bLevel = FALSE;
    }
    else
    {
        HAL_EXTERN_ATSC_DBINFO(printf("Can not use this pin as GPIO[%d]\n", u8Pin));
        
        bRet = FALSE;
        
        return bRet;
    }
    
    return bRet;
}

static MS_BOOL _HAL_EXTERN_ATSC_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data;

    bRet = _I2C_READ_BYTE(0x09C6, &u8Data);
    if (u8Pin == 0)
    {
        if (bLevel == FALSE)
            u8Data &= ~0x01;
        else
            u8Data |= 0x01;
    }
    else if (u8Pin == 1)
    {
        if (bLevel == FALSE)
            u8Data &= ~0x02;
        else
            u8Data |= 0x02;
    }
    else if (u8Pin == 2)
    {
        if (bLevel == FALSE)
            u8Data &= ~0x04;
        else
            u8Data |= 0x04;
    }
    else if (u8Pin == 5)
    {
        if (bLevel == FALSE)
            u8Data &= ~0x20;
        else
            u8Data |= 0x20;
    }
    else
    {
        HAL_EXTERN_ATSC_DBINFO(printf("Can not use this pin as GPIO[%d]\n", u8Pin));
        
        bRet = FALSE;
        
        return bRet;
    }

    bRet = _I2C_WRITE_BYTE(0x09C6, u8Data);

    return bRet;
}

static MS_BOOL _HAL_EXTERN_ATSC_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data;

    bRet = _I2C_READ_BYTE(0x09C8, &u8Data);
    if (u8Pin == 0)
    {
        if (bEnableOut == TRUE)
            u8Data &= ~0x01;
        else
            u8Data |= 0x01;
    }
    else if (u8Pin == 1)
    {
        if (bEnableOut == TRUE)
            u8Data &= ~0x02;
        else
            u8Data |= 0x02;
    }
    else if (u8Pin == 2)
    {
        if (bEnableOut == TRUE)
            u8Data &= ~0x04;
        else
            u8Data |= 0x04;
    }
    else if (u8Pin == 5)
    {
        if (bEnableOut == TRUE)
            u8Data &= ~0x20;
        else
            u8Data |= 0x20;
    }
    else
    {
        HAL_EXTERN_ATSC_DBINFO(printf("Can not use this pin as GPIO[%d]\n", u8Pin));
        
        bRet = FALSE;
        
        return bRet;
    }

    bRet = _I2C_WRITE_BYTE(0x09C8, u8Data);
    
    return bRet;
}

static MS_BOOL _HAL_EXTERN_ATSC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return _I2C_READ_BYTE(u16Addr, pu8Data);
}

static MS_BOOL _HAL_EXTERN_ATSC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return _I2C_WRITE_BYTE(u16Addr, u8Data);
}

static MS_BOOL _HAL_EXTERN_ATSC_DoIQSwap(MS_BOOL bIsQPad)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;

    pRes->sDMD_ATSC_InitData.bIsQPad = bIsQPad;
    
    //Set I&Q pad
    if (pRes->sDMD_ATSC_InitData.bIsQPad)
    {
        _I2C_WRITE_BYTE(0x0A33, 0x2E);
        _I2C_WRITE_BYTE(0x0A2E, 0x01);
        
        HAL_EXTERN_ATSC_DBINFO(printf("select Q pad source\n"));
    }
    else
    {
        _I2C_WRITE_BYTE(0x0A33, 0x1E);
        _I2C_WRITE_BYTE(0x0A2E, 0x00);
        
        HAL_EXTERN_ATSC_DBINFO(printf("select I pad source\n"));
    }
    
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_EXTERN_ATSC_IOCTL_CMD(DMD_ATSC_HAL_COMMAND eCmd, void *pArgs)
{
    MS_BOOL bResult = TRUE;
    
    switch(eCmd)
    {
    case DMD_ATSC_HAL_CMD_Exit:
        bResult = _HAL_EXTERN_ATSC_Exit();
        break;
    case DMD_ATSC_HAL_CMD_InitClk:
        _HAL_EXTERN_ATSC_InitClk();
        break;
    case DMD_ATSC_HAL_CMD_Download:
        bResult = _HAL_EXTERN_ATSC_Download();
        break;
    case DMD_ATSC_HAL_CMD_FWVERSION:
        _HAL_EXTERN_ATSC_FWVERSION();
        break;
    case DMD_ATSC_HAL_CMD_SoftReset:
        bResult = _HAL_EXTERN_ATSC_SoftReset();
        break;
    case DMD_ATSC_HAL_CMD_SetVsbMode:
        bResult = _HAL_EXTERN_ATSC_SetVsbMode();
        break;
    case DMD_ATSC_HAL_CMD_Set64QamMode:
        bResult = _HAL_EXTERN_ATSC_Set64QamMode();
        break;
    case DMD_ATSC_HAL_CMD_Set256QamMode:
        bResult = _HAL_EXTERN_ATSC_Set256QamMode();
        break;
    case DMD_ATSC_HAL_CMD_SetModeClean:
        bResult = _HAL_EXTERN_ATSC_SetModeClean();
        break;
    case DMD_ATSC_HAL_CMD_Set_QAM_SR:
        bResult = _HAL_EXTERN_ATSC_Set_QAM_SR();
        break;
    case DMD_ATSC_HAL_CMD_Active:
        break;
    case DMD_ATSC_HAL_CMD_Check8VSB64_256QAM:
        *((DMD_ATSC_DEMOD_TYPE *)pArgs) = _HAL_EXTERN_ATSC_Check8VSB64_256QAM();
        break;
    case DMD_ATSC_HAL_CMD_AGCLock:
        bResult = _HAL_EXTERN_ATSC_Vsb_QAM_AGCLock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_PreLock:
        bResult = _HAL_EXTERN_ATSC_Vsb_PreLock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_FSync_Lock:
        bResult = _HAL_EXTERN_ATSC_Vsb_FSync_Lock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_CE_Lock:
        bResult = _HAL_EXTERN_ATSC_Vsb_CE_Lock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_FEC_Lock:
        bResult = _HAL_EXTERN_ATSC_Vsb_FEC_Lock();
        break;
    case DMD_ATSC_HAL_CMD_QAM_PreLock:
        bResult = _HAL_EXTERN_ATSC_QAM_PreLock();
        break;
    case DMD_ATSC_HAL_CMD_QAM_Main_Lock:
        bResult = _HAL_EXTERN_ATSC_QAM_Main_Lock();
        break;
    case DMD_ATSC_HAL_CMD_ReadIFAGC:
        *((MS_U16 *)pArgs) = _HAL_EXTERN_ATSC_ReadIFAGC();
        break;
    case DMD_ATSC_HAL_CMD_CheckSignalCondition:
        _HAL_EXTERN_ATSC_CheckSignalCondition((DMD_ATSC_SIGNAL_CONDITION *)pArgs);
        break;
    case DMD_ATSC_HAL_CMD_ReadSNRPercentage:
        *((MS_U8 *)pArgs) = _HAL_EXTERN_ATSC_ReadSNRPercentage();
        break;
    case DMD_ATSC_HAL_CMD_ReadPKTERR:
        *((MS_U16 *)pArgs) = _HAL_EXTERN_ATSC_ReadPKTERR();
        break;
    case DMD_ATSC_HAL_CMD_GetPreViterbiBer:
        break;
    case DMD_ATSC_HAL_CMD_GetPostViterbiBer:
        bResult = _HAL_EXTERN_ATSC_ReadBER((float *)pArgs);
        break;
    case DMD_ATSC_HAL_CMD_ReadFrequencyOffset:
        *((MS_S16 *)pArgs) = _HAL_EXTERN_ATSC_ReadFrequencyOffset();
        break;
    case DMD_ATSC_HAL_CMD_TS_INTERFACE_CONFIG:
        bResult = _HAL_EXTERN_ATSC_SetSerialControl(*((MS_U8 *)pArgs));
        break;
    case DMD_ATSC_HAL_CMD_IIC_Bypass_Mode:
        bResult = _HAL_EXTERN_ATSC_IIC_Bypass_Mode(*((MS_BOOL *)pArgs));
        break;
    case DMD_ATSC_HAL_CMD_SSPI_TO_GPIO:
        bResult = _HAL_EXTERN_ATSC_SW_SSPI_GPIO(*((MS_BOOL *)pArgs));
        break;
    case DMD_ATSC_HAL_CMD_GPIO_GET_LEVEL:        
        bResult = _HAL_EXTERN_ATSC_GPIO_GET_LEVEL((*((DMD_ATSC_GPIO_PIN_DATA *)pArgs)).u8Pin, &((*((DMD_ATSC_GPIO_PIN_DATA *)pArgs)).bLevel));
        break;
    case DMD_ATSC_HAL_CMD_GPIO_SET_LEVEL:
        bResult = _HAL_EXTERN_ATSC_GPIO_SET_LEVEL((*((DMD_ATSC_GPIO_PIN_DATA *)pArgs)).u8Pin, (*((DMD_ATSC_GPIO_PIN_DATA *)pArgs)).bLevel);
        break;
    case DMD_ATSC_HAL_CMD_GPIO_OUT_ENABLE:
        bResult = _HAL_EXTERN_ATSC_GPIO_OUT_ENABLE((*((DMD_ATSC_GPIO_PIN_DATA *)pArgs)).u8Pin, (*((DMD_ATSC_GPIO_PIN_DATA *)pArgs)).bIsOut);
        break;
    case DMD_ATSC_HAL_CMD_GET_REG:
        bResult = _HAL_EXTERN_ATSC_GetReg((*((DMD_ATSC_REG_DATA *)pArgs)).u16Addr, &((*((DMD_ATSC_REG_DATA *)pArgs)).u8Data));
        break;
    case DMD_ATSC_HAL_CMD_SET_REG:
        bResult = _HAL_EXTERN_ATSC_SetReg((*((DMD_ATSC_REG_DATA *)pArgs)).u16Addr, (*((DMD_ATSC_REG_DATA *)pArgs)).u8Data);
        break;
	case DMD_ATSC_HAL_CMD_DoIQSwap:
        bResult = _HAL_EXTERN_ATSC_DoIQSwap(*((MS_BOOL *)pArgs));
        break;
    default:
        break;
    }
    
    return bResult;
}

static MS_U8 _DEFAULT_CMD(void)
{
    return TRUE;
}

MS_BOOL MDrv_DMD_ATSC_Initial_Hal_Interface(void)  __attribute__((weak, alias ("_DEFAULT_CMD")));
