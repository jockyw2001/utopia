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
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_HWI2C_H_
#define _HAL_HWI2C_H_

////////////////////////////////////////////////////////////////////////////////
/// @file halHWI2C.h
/// @author MStar Semiconductor Inc.
/// @brief MIIC control functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
//v: value n: shift n bits
//v: value n: shift n bits
#define _LShift(v, n)       ((v) << (n))
#define _RShift(v, n)       ((v) >> (n))

#define HIGH_BYTE(val)      (MS_U8)_RShift((val), 8)
#define LOW_BYTE(val)       ((MS_U8)((val) & 0xFF))

#define __BIT(x)    ((MS_U8)_LShift(1, x))
#define __BIT0       __BIT(0)
#define __BIT1       __BIT(1)
#define __BIT2       __BIT(2)
#define __BIT3       __BIT(3)
#define __BIT4       __BIT(4)
#define __BIT5       __BIT(5)
#define __BIT6       __BIT(6)
#define __BIT7       __BIT(7)

#define HWI2C_SET_RW_BIT(bRead, val) ((bRead) ? ((val) | __BIT0) : ((val) & ~__BIT0))

#define HAL_HWI2C_PORTS         4
#define HAL_HWI2C_PORT0         0
#define HAL_HWI2C_PORT1         1
#define HAL_HWI2C_PORT2         2
#define HAL_HWI2C_PORT3         3

typedef enum _HAL_HWI2C_STATE
{
	E_HAL_HWI2C_STATE_IDEL = 0,
	E_HAL_HWI2C_STATE_START,
	E_HAL_HWI2C_STATE_WRITE,
	E_HAL_HWI2C_STATE_READ,
	E_HAL_HWI2C_STATE_INT,
	E_HAL_HWI2C_STATE_WAIT,
	E_HAL_HWI2C_STATE_STOP
} HAL_HWI2C_STATE;

typedef enum _HAL_HWI2C_PORT
{
    E_HAL_HWI2C_PORT0_0 = 0, //disable port 0
    E_HAL_HWI2C_PORT0_1,
    E_HAL_HWI2C_PORT0_2,
    E_HAL_HWI2C_PORT0_3,
    E_HAL_HWI2C_PORT0_4,
    E_HAL_HWI2C_PORT0_5,
    E_HAL_HWI2C_PORT0_6,
    E_HAL_HWI2C_PORT0_7,

    E_HAL_HWI2C_PORT1_0,  //disable port 1
    E_HAL_HWI2C_PORT1_1,
    E_HAL_HWI2C_PORT1_2,
    E_HAL_HWI2C_PORT1_3,
    E_HAL_HWI2C_PORT1_4,
    E_HAL_HWI2C_PORT1_5,
    E_HAL_HWI2C_PORT1_6,
    E_HAL_HWI2C_PORT1_7,

    E_HAL_HWI2C_PORT2_0,  //disable port 2
    E_HAL_HWI2C_PORT2_1,
    E_HAL_HWI2C_PORT2_2,
    E_HAL_HWI2C_PORT2_3,
    E_HAL_HWI2C_PORT2_4,
    E_HAL_HWI2C_PORT2_5,
    E_HAL_HWI2C_PORT2_6,
    E_HAL_HWI2C_PORT2_7,

    E_HAL_HWI2C_PORT3_0, //disable port 3
    E_HAL_HWI2C_PORT3_1,
    E_HAL_HWI2C_PORT3_2,
    E_HAL_HWI2C_PORT3_3,
    E_HAL_HWI2C_PORT3_4,
    E_HAL_HWI2C_PORT3_5,
    E_HAL_HWI2C_PORT3_6,
    E_HAL_HWI2C_PORT3_7,

    E_HAL_HWI2C_PORT_NOSUP
}HAL_HWI2C_PORT;

typedef enum _HAL_HWI2C_CLKSEL
{
    E_HAL_HWI2C_CLKSEL_HIGH = 0,
    E_HAL_HWI2C_CLKSEL_NORMAL,
    E_HAL_HWI2C_CLKSEL_SLOW,
    E_HAL_HWI2C_CLKSEL_VSLOW,
    E_HAL_HWI2C_CLKSEL_USLOW,
    E_HAL_HWI2C_CLKSEL_UVSLOW,
    E_HAL_HWI2C_CLKSEL_VHIGH,
    E_HAL_HWI2C_CLKSEL_NOSUP
}HAL_HWI2C_CLKSEL;

typedef enum _HAL_HWI2C_CLK
{
    E_HAL_HWI2C_CLK_DIV4 = 1, //750K@12MHz
    E_HAL_HWI2C_CLK_DIV8,     //375K@12MHz
    E_HAL_HWI2C_CLK_DIV16,    //187.5K@12MHz
    E_HAL_HWI2C_CLK_DIV32,    //93.75K@12MHz
    E_HAL_HWI2C_CLK_DIV64,    //46.875K@12MHz
    E_HAL_HWI2C_CLK_DIV128,   //23.4375K@12MHz
    E_HAL_HWI2C_CLK_DIV256,   //11.71875K@12MHz
    E_HAL_HWI2C_CLK_DIV512,   //5.859375K@12MHz
    E_HAL_HWI2C_CLK_DIV1024,  //2.9296875K@12MHz
    E_HAL_HWI2C_CLK_NOSUP
}HAL_HWI2C_CLK;

typedef enum {
    E_HAL_HWI2C_READ_MODE_DIRECT,                       ///< first transmit slave address + reg address and then start receive the data */
    E_HAL_HWI2C_READ_MODE_DIRECTION_CHANGE,             ///< slave address + reg address in write mode, direction change to read mode, repeat start slave address in read mode, data from device
    E_HAL_HWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START,  ///< slave address + reg address in write mode + stop, direction change to read mode, repeat start slave address in read mode, data from device
    E_HAL_HWI2C_READ_MODE_MAX
} HAL_HWI2C_ReadMode;

typedef enum _HAL_HWI2C_DMA_ADDRMODE
{
    E_HAL_HWI2C_DMA_ADDR_NORMAL = 0,
    E_HAL_HWI2C_DMA_ADDR_10BIT,
    E_HAL_HWI2C_DMA_ADDR_MAX,
}HAL_HWI2C_DMA_ADDRMODE;

typedef enum _HAL_HWI2C_DMA_MIUPRI
{
    E_HAL_HWI2C_DMA_PRI_LOW = 0,
    E_HAL_HWI2C_DMA_PRI_HIGH,
    E_HAL_HWI2C_DMA_PRI_MAX,
}HAL_HWI2C_DMA_MIUPRI;

typedef enum _HAL_HWI2C_DMA_MIUCH
{
    E_HAL_HWI2C_DMA_MIU_CH0 = 0,
    E_HAL_HWI2C_DMA_MIU_CH1,
    E_HAL_HWI2C_DMA_MIU_MAX,
}HAL_HWI2C_DMA_MIUCH;

typedef struct _HAL_HWI2C_PinCfg
{
    MS_U32  u32Reg;    /// register
    MS_U8   u8BitPos;  /// bit position
    MS_BOOL bEnable;   /// enable or disable
}HAL_HWI2C_PinCfg;

typedef struct _HAL_HWI2C_PortCfg //Synchronize with drvHWI2C.h
{
    MS_U32                  u32DmaPhyAddr;  /// DMA physical address
    HAL_HWI2C_DMA_ADDRMODE  eDmaAddrMode;   /// DMA address mode
    HAL_HWI2C_DMA_MIUPRI    eDmaMiuPri;     /// DMA miu priroity
    HAL_HWI2C_DMA_MIUCH     eDmaMiuCh;      /// DMA miu channel
    MS_BOOL                 bDmaEnable;     /// DMA enable

    HAL_HWI2C_PORT          ePort;          /// number
    HAL_HWI2C_CLKSEL        eSpeed;         /// clock speed
    HAL_HWI2C_ReadMode      eReadMode;      /// read mode
    MS_BOOL                 bEnable;        /// enable

}HAL_HWI2C_PortCfg;

/// I2C Configuration for initialization
typedef struct _HAL_HWI2C_CfgInit //Synchronize with drvHWI2C.h
{
    HAL_HWI2C_PortCfg   sCfgPort[4];    /// port cfg info
    HAL_HWI2C_PinCfg    sI2CPin;        /// pin info
    HAL_HWI2C_CLKSEL    eSpeed;         /// speed
    HAL_HWI2C_PORT      ePort;          /// port
    HAL_HWI2C_ReadMode  eReadMode;      /// read mode

}HAL_HWI2C_CfgInit;

////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
void HAL_HWI2C_ExtraDelay(MS_U32 u32Us);
void HAL_HWI2C_SetIOMapBase(MS_VIRT u32Base);
MS_U8 HAL_HWI2C_ReadByte(MS_VIRT u32RegAddr);
MS_U16 HAL_HWI2C_Read2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_HWI2C_Read4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_HWI2C_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_HWI2C_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_HWI2C_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_HWI2C_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask);
MS_U8 HAL_HWI2C_ReadBytePM(MS_U32 u32RegAddr);
MS_U16 HAL_HWI2C_Read2BytePM(MS_U32 u32RegAddr);
MS_U32 HAL_HWI2C_Read4BytePM(MS_U32 u32RegAddr);
MS_BOOL HAL_HWI2C_WriteBytePM(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_HWI2C_Write2BytePM(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_HWI2C_Write4BytePM(MS_U32 u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_HWI2C_WriteRegBitPM(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_WriteByteMaskPM(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask);

MS_BOOL HAL_HWI2C_Init_Chip(void);
MS_BOOL HAL_HWI2C_IsMaster(void);
MS_BOOL HAL_HWI2C_Master_Enable(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_SetPortRegOffset(HAL_HWI2C_PORT ePort, MS_U32* pu32Offset);
MS_BOOL HAL_HWI2C_GetPortIdxByOffset(MS_U16 u16Offset, MS_U8* pu8Port);
MS_BOOL HAL_HWI2C_GetPortIdxByPort(HAL_HWI2C_PORT ePort, MS_U8* pu8Port);
MS_BOOL HAL_HWI2C_SelectPort(HAL_HWI2C_PORT ePort);
MS_BOOL HAL_HWI2C_SetClk(MS_U16 u16PortOffset, HAL_HWI2C_CLKSEL eClkSel);

MS_BOOL HAL_HWI2C_Start(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_Stop(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_ReadRdy(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_SendData(MS_U16 u16PortOffset, MS_U8 u8Data);
MS_U8 HAL_HWI2C_RecvData(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_Get_SendAck(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_NoAck(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_Ack(MS_U16 u16PortOffset);
MS_U8 HAL_HWI2C_GetState(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_Is_Idle(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_Is_INT(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_Clear_INT(MS_U16 u16PortOffset);
MS_BOOL HAL_HWI2C_Reset(MS_U16 u16PortOffset, MS_BOOL bReset);
MS_BOOL HAL_HWI2C_Send_Byte(MS_U16 u16PortOffset, MS_U8 u8Data);
MS_BOOL HAL_HWI2C_Recv_Byte(MS_U16 u16PortOffset, MS_U8 *pData);

MS_BOOL HAL_HWI2C_DMA_Init(MS_U16 u16PortOffset, HAL_HWI2C_PortCfg* pstPortCfg);
MS_BOOL HAL_HWI2C_DMA_Enable(MS_U16 u16PortOffset, MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_DMA_ReadBytes(MS_U16 u16PortOffset, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
MS_BOOL HAL_HWI2C_DMA_WriteBytes(MS_U16 u16PortOffset, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);

void HAL_HWI2C_Init_ExtraProc(void);

void HAL_HWI2C_EnableRetry(MS_BOOL bEnable);

#if 0
MS_BOOL HAL_HWI2C_EnINT(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnDMA(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnClkStretch(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnTimeoutINT(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnFilter(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnPushSda(MS_BOOL bEnable);

MS_BOOL HAL_HWI2C_DMA_SetINT(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_DMA_Reset(MS_BOOL bReset);
MS_BOOL HAL_HWI2C_DMA_MiuReset(MS_BOOL bReset);
MS_BOOL HAL_HWI2C_DMA_SetMiuPri(HAL_HWI2C_DMA_MIUPRI eMiuPri);
MS_BOOL HAL_HWI2C_DMA_SetMiuAddr(MS_U32 u32MiuAddr);
MS_BOOL HAL_HWI2C_DMA_Trigger(void);
MS_BOOL HAL_HWI2C_DMA_ReTrigger(void);
MS_BOOL HAL_HWI2C_DMA_SetReadMode(HAL_HWI2C_ReadMode eReadMode);
MS_BOOL HAL_HWI2C_DMA_SetRdWrt(MS_BOOL bRdWrt);
MS_BOOL HAL_HWI2C_DMA_SetMiuChannel(HAL_HWI2C_DMA_MIUCH eMiuCh);
MS_BOOL HAL_HWI2C_DMA_TxfrDone(void);
MS_BOOL HAL_HWI2C_DMA_IsTxfrDone(void);
MS_BOOL HAL_HWI2C_DMA_SetTxfrCmd(MS_U8 u8CmdLen, MS_U8* pu8CmdBuf);
MS_BOOL HAL_HWI2C_DMA_SetCmdLen(MS_U8 u8CmdLen);
MS_BOOL HAL_HWI2C_DMA_SetDataLen(MS_U32 u32DataLen);
MS_U32 HAL_HWI2C_DMA_GetTxfrCnt(void);
MS_BOOL HAL_HWI2C_DMA_SetSlaveAddr(MS_U16 u16SlaveAddr);
HAL_HWI2C_DMA_ADDRMODE HAL_HWI2C_DMA_GetAddrMode(void);
MS_BOOL HAL_HWI2C_DMA_SetAddrMode(HAL_HWI2C_DMA_ADDRMODE eAddrMode);
#endif

#endif  //_MHAL_HWI2C_H_

