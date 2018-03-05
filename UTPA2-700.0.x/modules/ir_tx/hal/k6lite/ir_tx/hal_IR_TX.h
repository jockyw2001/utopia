

void HAL_IR_TX_SetIOMapBase(MS_VIRT virtBase);

MS_BOOL HAL_IR_TX_WriteRegBit(MS_PHY phy64RegAddr, MS_U8 u8Mask);
MS_BOOL HAL_IR_TX_WriteByte(MS_PHY phy64RegAddr, MS_U8 u8Val);
MS_BOOL HAL_IR_TX_Write2Byte(MS_PHY phy64u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_IR_TX_Write4Byte(MS_PHY phy64u32RegAddr, MS_U32 u32Val);

MS_U8 HAL_IR_TX_ReadByte(MS_PHY phy64RegAddr);
MS_U16 HAL_IR_TX_Read2Byte(MS_PHY phy64RegAddr);

MS_BOOL HAL_IR_TX_Reset(void);

MS_BOOL HAL_IR_TX_SetIRTXMode(void);
MS_BOOL HAL_IR_TX_Reset(void);
MS_BOOL HAL_IR_TX_Trigger(void);
