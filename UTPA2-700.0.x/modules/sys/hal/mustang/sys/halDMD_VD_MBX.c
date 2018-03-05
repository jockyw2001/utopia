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

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "halDMD_VD_MBX.h"
#include "drvMMIO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined(MCU_AEON)
    #define BASEADDR_RIU            0xA0000000
#elif defined(__arm__)
    #define BASEADDR_RIU            0x1F000000
#else
    #define BASEADDR_RIU            0xBF000000
#endif

#define RIU_MACRO_START             do {
#define RIU_MACRO_END               } while (0)

// Address bus of RIU is 16 bits.

#define RIU_READ_BYTE(addr)         (READ_BYTE(_hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr)))
#define RIU_READ_2BYTE(addr)        (READ_WORD(_hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr)))
#define RIU_WRITE_BYTE(addr, val)   {WRITE_BYTE(_hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr), val)}
#define RIU_WRITE_2BYTE(addr, val)  {WRITE_WORD(_hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr), val)}

// Standard Form

#define RIU_ReadByte( u32Reg )      RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define RIU_Read2Byte( u32Reg )     (RIU_READ_2BYTE((u32Reg) << 1))

#define RIU_ReadRegBit( u32Reg, u8Mask )    (RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1)) & (u8Mask))

#define RIU_WriteRegBit( u32Reg, bEnable, u8Mask )                                      \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) << 1) - ((u32Reg) & 1)) , (bEnable) ? (RIU_READ_BYTE((((u32Reg) << 1) - ((u32Reg) & 1))  ) | (u8Mask)) :                           \
                                (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Mask)));                            \
    RIU_MACRO_END

#define RIU_WriteByte( u32Reg, u8Val )                                                  \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);                           \
    RIU_MACRO_END

#define RIU_Write2Byte( u32Reg, u16Val )                                                \
    RIU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                                            \
    {                                                                                                                \
        RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                     \
        RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        RIU_WRITE_2BYTE( ((u32Reg)<<1) ,  u16Val);                                     \
    }                                                                                   \
    RIU_MACRO_END

#define RIU_WriteByteMask( u32Reg, u8Val, u8Msk )                                       \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END

#define DMD_MBX_TIMEOUT 200
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32  u32DMD_VD_MBX_BaseAddr;
    MS_BOOL bBaseAddrInitialized;
    MS_U8   u8DMD_VD_MBX_Type;
} hal_DMD_VD_MBX_t;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static hal_DMD_VD_MBX_t _hal_DMD_VD_MBX =
{
    .u32DMD_VD_MBX_BaseAddr = BASEADDR_RIU,
    .bBaseAddrInitialized = 0,
    .u8DMD_VD_MBX_Type = -1,
};
static MS_U8 MBX_SetType;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_SYS_DMD_VD_MBX_Init(void)
{
    MS_U32  u32NonPMBank;
    MS_U32  u32NonPMBankSize;
    if (!MDrv_MMIO_GetBASE( &u32NonPMBank, &u32NonPMBankSize, MS_MODULE_AVD))
    {
        printf("IOMap failure to get MAP_NONPM_BANK\n");
        return FALSE;
    }

    _hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr=u32NonPMBank;
    _hal_DMD_VD_MBX.bBaseAddrInitialized = 1;
    _hal_DMD_VD_MBX.u8DMD_VD_MBX_Type = -1;
    #ifdef MS_DEBUG
    printf("HAL_SYS_DMD_VD_MBX_Init %lx\n",u32NonPMBank);
    #endif
    return TRUE;
}

MS_U8 HAL_SYS_DMD_VD_MBX_GetType(void)
{
    #ifdef MS_DEBUG
    printf("HAL_SYS_DMD_VD_MBX_GetType %d\n",MBX_SetType);
    #endif
    //return RIU_ReadByte(0x1E3E);
    return MBX_SetType;
}

void HAL_SYS_DMD_VD_MBX_SetType(MS_U8 u8Value)
{
    MBX_SetType =  u8Value;
    //RIU_WriteByte(0x1E3E, u8Value);
}

#if (DMD_VD_MBX_CHIP_VERSION == DMD_VD_MBX_CHIP_T3)
// ATV
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WaitReady(void)
{
    MS_U32 u32StartTime=MsOS_GetSystemTime();
    while (MsOS_GetSystemTime()-u32StartTime < 10)
    {
        if (!RIU_ReadByte(DemodCmd)) return TRUE;
        MsOS_DelayTaskUs(500);
    }
    #ifdef MS_DEBUG
    printf("HAL_SYS_DMD_VD_MBX_ATV_WaitReady Fail\n");
    #endif
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_ReadByte(MS_U32 u32Reg, MS_U8 *u8Value)
{
    if (HAL_SYS_DMD_VD_MBX_ATV_WaitReady())
    {
        RIU_WriteByte(DemodAdrL, u32Reg&0xFF);
        RIU_WriteByte(DemodAdrH, (u32Reg>>8)&0xFF);
        RIU_WriteByte(DemodCmd, DemodCmdRdReg);

        if (HAL_SYS_DMD_VD_MBX_ATV_WaitReady())
        {
            *u8Value = RIU_ReadByte(DemodData);
            return TRUE;
        }
    }
    #ifdef MS_DEBUG
    printf("HAL_SYS_DMD_VD_MBX_ATV_ReadByte Fail\n");
    #endif
    *u8Value = 0;
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WriteByte(MS_U32 u32Reg, MS_U8 u8Val)
{
    if (HAL_SYS_DMD_VD_MBX_ATV_WaitReady())
    {
        RIU_WriteByte(DemodAdrL, u32Reg&0xFF);
        RIU_WriteByte(DemodAdrH, (u32Reg>>8)&0xFF);
        RIU_WriteByte(DemodData, u8Val);
        RIU_WriteByte(DemodCmd, DemodCmdWrReg);
        return TRUE;
    }
    return FALSE;
}
#else
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WaitReady(void)
{
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_ReadByte(MS_U32 u32Reg, MS_U8 *u8Value)
{
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WriteByte(MS_U32 u32Reg, MS_U8 u8Val)
{
    return FALSE;
}
#endif

// DVBT & DVBC
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WaitReady(void)
{
    MS_U32 u32StartTime=MsOS_GetSystemTime();
    while (RIU_ReadByte(MBRegBase + 0x00)) // wait VDMCU ready
    {
        if (MsOS_Timer_DiffTimeFromNow(u32StartTime)>DMD_MBX_TIMEOUT)
        {
            printf("HAL_SYS_DMD_VD_MBX_DVB_WaitReady Timeout\n");
            return FALSE;
        }
    }
    return TRUE;
}

void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void)
{
    MS_U32 u32StartTime=MsOS_GetSystemTime();
    while(RIU_ReadByte(MBRegBase + 0x00) != 0xFF)           // wait MB_CNTL set done
    {
        if (MsOS_Timer_DiffTimeFromNow(u32StartTime)>DMD_MBX_TIMEOUT)
        {
            printf("HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake Timeout\n");
            break;
        }
    }
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadByte(MS_U16 u16Addr, MS_U8 *u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x00, 0x01);                 // MB_CNTL set read mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                         // assert interrupt to VD MCU51
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                         // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    *u8Value = RIU_ReadByte(MBRegBase + 0x03);             // REG_DATA get
    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteByte(MS_U32 u16Addr, MS_U8 u8Data)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x03, u8Data);               // REG_DATA
    RIU_WriteByte(MBRegBase + 0x00, 0x02);                 // MB_CNTL set write mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                         // assert interrupt to VD MCU51
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                         // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear
    return TRUE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadDspReg(MS_U32 u16Addr, MS_U8 *u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));    // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);           // ADDR_L
    RIU_WriteByte(MBRegBase + 0x00, 0x03);                  // MB_CNTL set read mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                 // assert interrupt to DMD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                 // de-assert interrupt to DMD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    *u8Value = RIU_ReadByte(MBRegBase + 0x03);              // REG_DATA get
    RIU_WriteByte(MBRegBase + 0x00, 0x00);                  // MB_CNTL clear

    return TRUE;
}


MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteDspReg(MS_U32 u16Addr, MS_U8 u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));        // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);               // ADDR_L
    RIU_WriteByte(MBRegBase + 0x03, u8Value);                    // REG_DATA
    RIU_WriteByte(MBRegBase + 0x00, 0x04);                      // MB_CNTL set write mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                     // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                     // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    RIU_WriteByte(MBRegBase + 0x00, 0x00);                      // MB_CNTL clear

    return TRUE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_ReadReg(MS_U16 u16Addr, MS_U8 *u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x00, 0x05);                 // MB_CNTL set read mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                // assert interrupt to DMD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                // de-assert interrupt to DMD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    *u8Value = RIU_ReadByte(MBRegBase + 0x03);             // REG_DATA get
    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

//-----------------------------------------------------------------------
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_WriteReg(MS_U32 u16Addr, MS_U8 u8Data)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x03, u8Data);               // REG_DATA
    RIU_WriteByte(MBRegBase + 0x00, 0x06);                 // MB_CNTL set write mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

// ATSC
void HAL_SYS_DMD_VD_MBX_ATSC_WriteByte(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag;

    RIU_WriteByte(MBRegBase + 0x00, (u16Addr&0xff));
    RIU_WriteByte(MBRegBase + 0x01, (u16Addr>>8));
    RIU_WriteByte(MBRegBase + 0x10, u8Data);
    RIU_WriteByte(MBRegBase + 0x1E, 0x01);

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                     // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                     // de-assert interrupt to VD MCU51

    for ( u8CheckCount=0; u8CheckCount < 10 ; u8CheckCount++ )
    {
        u8CheckFlag = RIU_ReadByte(MBRegBase + 0x1E);
        if ((u8CheckFlag&0x01)==0)
             break;
        MsOS_DelayTask(1);
    }
}

MS_U8 HAL_SYS_DMD_VD_MBX_ATSC_ReadByte(MS_U16 u16Addr)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag;
    MS_U8 u8Value;

    RIU_WriteByte(MBRegBase + 0x00, (u16Addr&0xff));
    RIU_WriteByte(MBRegBase + 0x01, (u16Addr>>8));
    RIU_WriteByte(MBRegBase + 0x1E, 0x02);

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                     // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                     // de-assert interrupt to VD MCU51

    for ( u8CheckCount=0; u8CheckCount < 10 ; u8CheckCount++ )
    {
        u8CheckFlag = RIU_ReadByte(MBRegBase + 0x1E);
        if ((u8CheckFlag&0x02)==0)
        {
            u8Value = RIU_ReadByte(MBRegBase + 0x10);
            return u8Value;
        }
        MsOS_DelayTask(1);
    }

    return 0;
}
