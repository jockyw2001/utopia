#include "MsTypes.h"
#include "reg_IR_TX.h"

static MS_VIRT _gMIO_MapBase = 0;

MS_U8 HAL_IR_TX_ReadByte(MS_PHY phy64RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(phy64RegAddr << 1) - (phy64RegAddr & 1)];
}

MS_U16 HAL_IR_TX_Read2Byte(MS_PHY phy64RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[phy64RegAddr];
}

MS_BOOL HAL_IR_TX_WriteByte(MS_PHY phy64RegAddr, MS_U8 u8Val)
{
    ((volatile MS_U8*)(_gMIO_MapBase))[(phy64RegAddr << 1) - (phy64RegAddr & 1)] = u8Val;
    return TRUE;
}



MS_BOOL HAL_IR_TX_Write2Byte(MS_PHY phy64RegAddr, MS_U16 u16Val)
{

    MS_PHY addr = phy64RegAddr;
    MS_U16 Val  = u16Val;
    ((volatile MS_U16*)(_gMIO_MapBase))[addr] = Val;
    return TRUE;
}

MS_BOOL HAL_IR_TX_Write4Byte(MS_PHY phy64RegAddr, MS_U32 u32Val)
{
    HAL_IR_TX_Write2Byte(phy64RegAddr, u32Val & 0x0000FFFF);
    HAL_IR_TX_Write2Byte(phy64RegAddr+2, u32Val >> 16);
    return TRUE;
}


MS_BOOL HAL_IR_TX_WriteRegBit(MS_PHY phy64RegAddr, MS_U16 u16Mask)
{

    MS_U16 u16Val = HAL_IR_TX_Read2Byte(phy64RegAddr);
    u16Val = HAL_IR_TX_Read2Byte(phy64RegAddr);
    u16Val = ( u16Val | u16Mask );
    HAL_IR_TX_Write2Byte(phy64RegAddr, u16Val);
    return TRUE;
}


void HAL_IR_TX_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
}


MS_BOOL HAL_IR_TX_SetIRTXMode(void)
{
	return HAL_IR_TX_WriteRegBit(PM_REG_Set_IRTX_MODE, IR_TX_MODE);
}


MS_BOOL HAL_IR_TX_Reset(void)
{
	return HAL_IR_TX_WriteRegBit(IR_TX_STATUS, IR_TX_RESET);
}

MS_BOOL HAL_IR_TX_Trigger(void)
{
	return HAL_IR_TX_WriteRegBit(IR_TX_FLAG, IR_TX_TRIGGER);
}









