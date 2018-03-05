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
/// file    halLDM.c
/// @brief  local dimming  Driver HAL Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <string.h>
#include "MsCommon.h"
#include "MsTypes.h"
#include "halLDM.h"
#include "regLDM.h"

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define LD_WriteByte(reg,val) WRITE_BYTE((u32VirtLdBaseAddr + ((reg)<<2)),val)

#define LD_Write2Byte(reg,val) WRITE_WORD((u32VirtLdBaseAddr + ((reg)<<2)),val)

#define LD_ReadByte(reg)    (READ_BYTE(u32VirtLdBaseAddr + ((reg)<<2)))

#define LD_Read2Byte(reg)  (READ_WORD(u32VirtLdBaseAddr + ((reg)<<2)))

#define SUBBANK_LD_60HZ  (0x2E) //sub bank 2E for local dimmig 60hz
#define SUBBANK_LD_120HZ (0xCE) //sub bank CE for local dimmig 120hz
#define SUBBANK_FO_BKC9  (0xC9) //sub bank c9 for data select 60/120hz

static MS_U8 u8Subbank = SUBBANK_LD_120HZ;

#define HAL_SUBBANKC9   LD_WriteByte(0,SUBBANK_FO_BKC9)
#define HAL_SUBBANKLD    LD_WriteByte(0,u8Subbank)
#define HAL_SUBBANKOFF  LD_WriteByte(0,0xFF)

static MS_U32 u32VirtLdBaseAddr = BASEADDR_RIU + REG_LD_BASE;

//------------------------------------------------------------------------------
/// Description : Config LDMA MMIO base address
/// @param u32PMBankBaseAddr \b IN:base address of MMIO (PM domain)
/// @param u32NONPMRegBaseAddr \b IN :base address of MMIO
/// @param u8DeviceIndex \b IN: index of HW IP
//------------------------------------------------------------------------------
void HAL_LDM_MMIOConfig(MS_U32 u32NONPMRegBaseAddr, MS_U8 u8ClkHz)
{
    u32VirtLdBaseAddr = u32NONPMRegBaseAddr + REG_LD_BASE;

    if(u8ClkHz == 60)
    {
        HAL_SUBBANKC9;
        LD_Write2Byte(REG_FO_DATA_PATH,0x0100);
        HAL_SUBBANKOFF;
        u8Subbank = SUBBANK_LD_60HZ;
    }
    else if(u8ClkHz == 120)
    {
        //LMDA path sel
        HAL_SUBBANKC9;
        LD_Write2Byte(REG_FO_DATA_PATH,0x0000);
        HAL_SUBBANKOFF;
        u8Subbank = SUBBANK_LD_120HZ;
    }
    else //default set 60HZ timming
    {
        //LMDA path sel
        HAL_SUBBANKC9;
        LD_Write2Byte(REG_FO_DATA_PATH,0x0100);
        HAL_SUBBANKOFF;
        u8Subbank = SUBBANK_LD_60HZ;
    }
    return;
}

void HAL_LDM_SetLDFAddr(MS_U8 u8AddIndex, MS_U32 u32LDFAddr_l,MS_U32 u32LDFAddr_r)
{
    HAL_SUBBANKLD;
    switch(u8AddIndex)
    {
       case 0:
            LD_Write2Byte(REG_LDF_BASEADDR0_LOW_L,u32LDFAddr_l&0xFFFF);
            LD_Write2Byte(REG_LDF_BASEADDR0_HIGH_L,u32LDFAddr_l >> 16);
            LD_Write2Byte(REG_LDF_BASEADDR0_LOW_R,u32LDFAddr_r&0xFFFF);
            LD_Write2Byte(REG_LDF_BASEADDR0_HIGH_R,u32LDFAddr_r >> 16);
            break;
       case 1:
            LD_Write2Byte(REG_LDF_BASEADDR1_LOW_L,u32LDFAddr_l&0xFFFF);
            LD_Write2Byte(REG_LDF_BASEADDR1_HIGH_L,u32LDFAddr_l >> 16);
            LD_Write2Byte(REG_LDF_BASEADDR1_LOW_R,u32LDFAddr_r&0xFFFF);
            LD_Write2Byte(REG_LDF_BASEADDR1_HIGH_R,u32LDFAddr_r >> 16);
            break;
       default:
           break;
    }
    HAL_SUBBANKOFF;
    return ;
}

void HAL_LDM_SetEdge2DAddr(MS_U32 u32Edge2DAddr)
{
    HAL_SUBBANKLD;
    LD_Write2Byte(REG_EDGE_2D_BASEADDR_LOW,u32Edge2DAddr&0xFFFF);
    LD_Write2Byte(REG_EDGE_2D_BASEADDR_HIGH,u32Edge2DAddr >> 16);
    HAL_SUBBANKOFF;
    return ;
}

void HAL_LDM_SetLDBAddr(MS_U8 u8AddIndex, MS_U32 u32LDBAddr_l,MS_U32 u32LDBAddr_r)
{
    HAL_SUBBANKLD;
    switch(u8AddIndex)
    {
       case 0:
            LD_Write2Byte(REG_LDB_BASEADDR0_LOW_L,u32LDBAddr_l&0xFFFF);
            LD_Write2Byte(REG_LDB_BASEADDR0_HIGH_L,u32LDBAddr_l >> 16);
            LD_Write2Byte(REG_LDB_BASEADDR0_LOW_R,u32LDBAddr_r&0xFFFF);
            LD_Write2Byte(REG_LDB_BASEADDR0_HIGH_R,u32LDBAddr_r >> 16);
            break;
       case 1:
            LD_Write2Byte(REG_LDB_BASEADDR1_LOW_L,u32LDBAddr_l&0xFFFF);
            LD_Write2Byte(REG_LDB_BASEADDR1_HIGH_L,u32LDBAddr_l >> 16);
            LD_Write2Byte(REG_LDB_BASEADDR1_LOW_R,u32LDBAddr_r&0xFFFF);
            LD_Write2Byte(REG_LDB_BASEADDR1_HIGH_R,u32LDBAddr_r >> 16);
            break;
       default:
           break;
    }
    HAL_SUBBANKOFF;
    return ;
}

void HAL_LDM_SetMIUPackOffset(MS_U8 u8Channel,MS_U8 u8PackOffset )
{
    if(u8PackOffset > 0x1F) //bit 0 ~bit4
    {
        printf("fun:%s @@error:Set MIU PackOffset Outof Range!!!\n",__FUNCTION__);
        return ;
    }
    HAL_SUBBANKLD;
    switch(u8Channel)
    {
       case LDMA0: //DMA0
            LD_WriteByte(REG_PACK_OFFSET0,u8PackOffset);
            break;
       case LDMA1: //DMA1
            LD_WriteByte(REG_PACK_OFFSET1,u8PackOffset);
            break;
       default:
           break;
    }
    HAL_SUBBANKOFF;
    return ;
}
void HAL_LDM_SetMIUPackLength(MS_U8 u8Channel,MS_U8 u8PackLength)
{
    if(u8PackLength > 0x1F) //bit 0 ~bit4
    {
        printf("fun:%s @@error:Set MIU PackLength Outof Range!!!\n",__FUNCTION__);
        return ;
    }

    HAL_SUBBANKLD;
    switch(u8Channel)
    {
       case LDMA0: //DMA0
            LD_WriteByte(REG_PACK_LENGTH0,(LD_Read2Byte(REG_PACK_LENGTH0)&(~0x1F))|u8PackLength);
            break;
       case LDMA1: //DMA1
            LD_WriteByte(REG_PACK_LENGTH1,(LD_Read2Byte(REG_PACK_LENGTH1)&(~0x1F))|u8PackLength);
            break;
       default:
           break;
    }

    HAL_SUBBANKOFF;
    return ;
}
void HAL_LDM_SetYoffEnd(MS_U8 u8Channel,MS_U8 u8YoffEnd)
{
    if(u8YoffEnd > 0x3F) //bit 0~bit5
    {
        printf("fun:%s @@error:Set YoffEnd Outof Range!!!\n",__FUNCTION__);
        return ;
    }
    HAL_SUBBANKLD;
    switch(u8Channel)
    {
        case LDMA0: //DMA0
             LD_WriteByte(REG_DMA_YOFF_END_0,u8YoffEnd);
             break;
        case LDMA1: //DMA1
             LD_WriteByte(REG_DMA_YOFF_END_1,u8YoffEnd);
             break;
        default:
            break;
    }
    HAL_SUBBANKOFF;
    return ;
}
void HAL_LDM_SetDmaEnable(MS_U8 u8Channel, MS_BOOL Enable)
{
    HAL_SUBBANKLD;
    switch(u8Channel)
    {
        case LDMA0: //DMA0
            if(Enable)
            {
                LD_Write2Byte(REG_PACK_LENGTH0,LD_Read2Byte(REG_PACK_LENGTH0)|REG_DMA0_ENABLE_BIT);
            }
            else
            {
                LD_Write2Byte(REG_PACK_LENGTH1,LD_Read2Byte(REG_PACK_LENGTH1)&(~REG_DMA1_ENABLE_BIT));
            }
            break;
        case LDMA1: //DMA1
            if(Enable)
            {
                LD_Write2Byte(REG_PACK_LENGTH1,LD_Read2Byte(REG_PACK_LENGTH1)|REG_DMA0_ENABLE_BIT);
            }
            else
            {
                LD_Write2Byte(REG_PACK_LENGTH1,LD_Read2Byte(REG_PACK_LENGTH1)&(~REG_DMA0_ENABLE_BIT));
            }
            break;
        default:
            break;
    }
    HAL_SUBBANKOFF;
    return ;
}

void HAL_LDM_SetBlHeightDMA(MS_U8 u8Height)
{
    HAL_SUBBANKLD;
    LD_Write2Byte(REG_BL_WIDTH_DMA,(LD_Read2Byte(REG_BL_WIDTH_DMA)&(0xC0FF))|(u8Height<<8));
    HAL_SUBBANKOFF;
    return ;
}
void HAL_LDM_SetBlWidthDMA(MS_U8 u8Width)
{
    HAL_SUBBANKLD;
    LD_Write2Byte(REG_BL_WIDTH_DMA,(LD_Read2Byte(REG_BL_WIDTH_DMA)&(0xFFC0))|u8Width);
    HAL_SUBBANKOFF;
    return ;
}

void HAL_LDM_SetLEDBufBaseOffset(MS_U32 u32DataOffset)
{
    HAL_SUBBANKLD;
    LD_Write2Byte(REG_BASEADDR_OFFSET,u32DataOffset&0xFFFF);
    LD_Write2Byte(REG_BASEADDR_OFFSET+1,u32DataOffset >> 16);
    HAL_SUBBANKOFF;
    return ;
}

//step6: enable local dimming
void HAL_LDM_Enable(MS_BOOL Enable)
{
    HAL_SUBBANKLD;
    if(Enable)
    {
        LD_Write2Byte(REG_LD_ENABLE,0x0001);
    }
    else
    {
        LD_Write2Byte(REG_LD_ENABLE,0x0000);
    }
    HAL_SUBBANKOFF;
    return ;
}
