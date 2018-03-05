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

#define _HAL_VBI_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"

// Internal Definition
#include "halVBI.h"
#include "regVBI.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define INTERFACE extern

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAKEWORD(a,b)               ((((MS_U16)((MS_U8) (a)))<<8) | ((MS_U16)((MS_U8) (b))))

#define R1BYTE(Addr, u8mask)            \
    (READ_BYTE (_ptrVBIRiuBaseAddr + ((Addr) << 1) - ((Addr) & 1)) & (u8mask))

#define W1BYTE(Addr, u8Val, u8mask)     \
    (WRITE_BYTE(_ptrVBIRiuBaseAddr + ((Addr) << 1) - ((Addr) & 1), (R1BYTE(Addr, 0xFF) & ~(u8mask)) | ((u8Val) & (u8mask))))

///////////////////////////////////////////////////////////////
#define MDrv_WriteByte( Reg, u8Val )                                                 \
     do {                                                                     \
     (WRITE_BYTE(_ptrVBIRiuBaseAddr + ((Reg) << 1) - ((Reg) & 1), u8Val));    \
          }while(0)

#define MDrv_WriteWord( Reg, u16Val )                                                 \
     do {                                                                     \
     (WRITE_WORD(_ptrVBIRiuBaseAddr + ((Reg) << 1), u16Val));              \
     }while(0)
     
#define MDrv_WriteByteMask( Reg, u8Val, u8Mask )                           \
    do {                                                                     \
    (WRITE_BYTE(_ptrVBIRiuBaseAddr + ((Reg) << 1) - ((Reg) & 1), (R1BYTE((Reg), 0xFF) & ~(u8Mask)) | ((u8Val) & (u8Mask))));    \
        }while(0)

#define MDrv_ReadByte( Reg) (READ_BYTE (_ptrVBIRiuBaseAddr + ((Reg) << 1) - ((Reg) & 1)))

#define _BIT0       BIT(0)
#define _BIT1       BIT(1)
#define _BIT2       BIT(2)
#define _BIT3       BIT(3)
#define _BIT4       BIT(4)
#define _BIT5       BIT(5)
#define _BIT6       BIT(6)
#define _BIT7       BIT(7)
#define _BIT8       BIT(8)
#define _BIT9       BIT(9)
#define _BIT10      BIT(10)
#define _BIT11      BIT(11)
#define _BIT12      BIT(12)
#define _BIT13      BIT(13)
#define _BIT14      BIT(14)
#define _BIT15      BIT(15)

typedef enum
{
    VBI_FIELD_EVEN,
    VBI_FIELD_ODD
} EN_VBI_FIELD;

#define BK_VBI_E7_BUG 1
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _ptrVBIRiuBaseAddr;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_U8 HAL_VBI_ReadByte(MS_U32 u32RegAddr)
{
    return (MDrv_ReadByte(u32RegAddr));
}

MS_U16 HAL_VBI_Read2Byte(MS_U32 u32RegAddr)
{
    return ((MDrv_ReadByte(u32RegAddr)) + ((MDrv_ReadByte(u32RegAddr + 1))<<8));
}

void HAL_VBI_WriteByte(MS_U32 u32RegAddr, MS_U8 val)
{
    MDrv_WriteByte(u32RegAddr, val);
}

void HAL_VBI_Write2Byte(MS_U32 u32RegAddr, MS_U16 val)
{
    MDrv_WriteByte(u32RegAddr, (val & 0xFF));
    MDrv_WriteByte(u32RegAddr + 1, (val>>8));
}

void HAL_VBI_WriteWord(MS_U32 u32RegAddr, MS_U16 val)
{
    if (u32RegAddr & 0x01)
        printf("[VBI][%s] ERROR, not alignment address", __FUNCTION__);
    
    MDrv_WriteWord(u32RegAddr, val);
}

void HAL_VBI_WriteByteMask(MS_U32 u32RegAddr, MS_U8 val, MS_U8 mask)
{
    MDrv_WriteByteMask(u32RegAddr, val, mask);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void VBI_TTXInit(MS_VIRT ptrAddr)
{
    _ptrVBIRiuBaseAddr = ptrAddr;

    // close caption slicer threshold mode
    HAL_VBI_WriteByte(BK_VBI_40, 0x01);

    // close caption line start 1 (lower 3 bits) = 0
    // close caption lin end 1 = 0
    HAL_VBI_WriteByte(BK_VBI_41, 0x00);

    // close caption line start 2 = 0
    // close caption CRI zero crossing type :  positive edge
    // close caption clock run-in amplitude upper threshold (upper 2 bits) = 0b01
    HAL_VBI_WriteByte(BK_VBI_50, 0x60);

    // close caption line end 2 = 0
    // close caption multi-line acquisition mode : 1
    // close caption zero crossing mode : normal.
    // close caption SYNC Found enable mode : 1
    HAL_VBI_WriteByte(BK_VBI_51, 0xA0);

    // teletext clock run-in amplitude accumulation start point. : 0b00010001
    // For eye-height testing
    HAL_VBI_WriteByte(BK_VBI_77, 0x11);

    // teletext clock run-in amplitude accumulation start point. : 0b00011010
    // For SuperVHS decode issue
    //HAL_VBI_WriteByte(TT_CLK_RUN_IN_START_POINT, 0x1A);

    // teletext VBI line start 1 (odd field) : 0b00100
    // teletext VBI line end (lower 3 bits) 0b000
    HAL_VBI_WriteByte(BK_VBI_7C, 0x04);

    // teletext data line end 1 (odd field) : 0b10110
    // teletext slicer read mode : 0b1
    // teletext framing code error bond value : 0b0 fully match framing code.
    // teletext framing code windows mode : 0b0
    HAL_VBI_WriteByte(BK_VBI_7D, 0x36);

    // teletext data line start 2 (even field) : 0b00100
    // teletext slicer threshold fixing mode : 0b0 adjust automatically according to TtSidDetSel
    // teletext slicer level mode : 0b0 original mode.
    // teletext initial packet counter : 0b1 packet counter increases when teletext packet is detected without upper-bound.
    HAL_VBI_WriteByte(BK_VBI_7E, 0x84);

    // teletext data line end 2 (even field) : 0b10110
    // teletext single line point mode : 0b11 Enable ttslptrmode, start from the line when previous line is no teletext.
    // teletext base address source selecion : 0b1
    HAL_VBI_WriteByte(BK_VBI_7F, 0xF6);

    HAL_VBI_WriteByte(BK_VBI_81, 0x52);
    HAL_VBI_WriteByte(BK_VBI_86, 0xD6);
    HAL_VBI_WriteByte(BK_VBI_89, 0xC2);
    HAL_VBI_WriteByte(BK_VBI_8A, 0x42);
    HAL_VBI_WriteByte(BK_VBI_8B, 0x24);
    HAL_VBI_WriteByte(BK_VBI_8D, 0xA5);
    HAL_VBI_WriteByte(BK_VBI_90, 0x70);
    HAL_VBI_WriteByte(BK_VBI_C4, 0x32);
    HAL_VBI_WriteByte(BK_VBI_CB, 0xC4);
    HAL_VBI_WriteByte(BK_VBI_CC, 0xBD);

    // For VPS detect speed up
    HAL_VBI_WriteByte(BK_VBI_B4, 0x42);
    HAL_VBI_WriteByte(BK_VBI_B5, 0x61);
    HAL_VBI_WriteByte(BK_VBI_BB, 0x06);

    HAL_VBI_WriteByte(BK_VBI_70,0x80); // enable VPS/WSS
}

void VBI_WSSInit(MS_VIRT ptrAddr)
{
    MS_U8 tmp = 0;
    _ptrVBIRiuBaseAddr = ptrAddr;

    tmp = HAL_VBI_ReadByte(BK_VBI_BF);
    tmp &= ~(_BIT6 | _BIT7);
    HAL_VBI_WriteByte(BK_VBI_BF, tmp);

    HAL_VBI_WriteByte(BK_VBI_70, 0x80); // enable VPS/WSS
}

MS_BOOL VBI_TTX_CheckCircuitReady(void)
{
    if( !(HAL_VBI_ReadByte( SLICERREADY ) & _BIT7) )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_U16 VBI_TTX_GetPacketCount(void)
{
    return (MS_U16) HAL_VBI_Read2Byte(VBI_PKTCNT_L);
}

MS_U16 VBI_GetWSS_Count(void)
{
    return (MS_U16) (HAL_VBI_ReadByte(VBI_WSS_COUNT) & 0x07);
}

MS_U16 VBI_GetVPS_Count(void)
{
    return (MS_U16) (HAL_VBI_ReadByte(VBI_VPS_COUNT)>>4);
}

void VBI_TTX_InitSlicer(MS_PHY addr, MS_U16 packetCount)
{
    if((addr >> 3) >= (1 << TTX_BUF_BIT))
    {
        printf("[VBI][%s] ERROR, buffer address out of bound\n", __FUNCTION__);
        //MS_ASSERT(0);
    }
        
    addr = addr >> 3; /* 8 byte-aligned */

    /* Initial VBI Buffer Start Address */
    HAL_VBI_Write2Byte(VBI_BASEADDR_L, addr);
    HAL_VBI_WriteByte(VBI_BASEADDR_H, addr >> 16);
    if((addr>>24) & 0x0f)
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, (HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~0x0f))|((addr>>24) & 0x0f));
    else
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~0x0f));

    /* Initial VBI Buffer Field Number */
    HAL_VBI_Write2Byte(VBI_BUF_LEN, packetCount);   /* no need to minus 1, follow Venus design */

    /* change dram access mode
      * 0x371F, BIT7 must be always 1 (otherwise the DMA access related function could be error) */
    HAL_VBI_WriteByte(TTDEC_COMMAND, _BIT1|_BIT7);   /* put header packet into VBI without decoder */
}

void VBI_TTX_EnableSlicer(MS_BOOL bEnable)
{
    if(bEnable)
        HAL_VBI_WriteByte(TT_ENABLE,  HAL_VBI_ReadByte(TT_ENABLE) | _BIT0); // enable TT VBI slicer
    else
        HAL_VBI_WriteByte(TT_ENABLE, HAL_VBI_ReadByte(TT_ENABLE) & ~(_BIT0)); // enable TT VBI slicer
}

MS_U8 VBI_TTX_GetHardware_Indication(void)
{
    return HAL_VBI_ReadByte(BK_VBI_AF);
}

void VBI_GetVPS_Data(MS_U8 *byte1, MS_U8 *byte2, MS_U8 *byte3, MS_U8 *byte4)
{
    *byte1 = HAL_VBI_ReadByte(BK_VBI_AD);
    *byte2 = HAL_VBI_ReadByte(BK_VBI_AE);
    *byte3 = HAL_VBI_ReadByte(BK_VBI_A6);
    *byte4 = HAL_VBI_ReadByte(BK_VBI_A7);
}

#define NTSC_WSS_CRC_CHECK 0

MS_U16 VBI_GetWSS_Data(void)
{
    MS_U8 wWssWordH;
    MS_U8 wWssWordL;

    wWssWordL = HAL_VBI_ReadByte(BK_VBI_CD);
    wWssWordH = HAL_VBI_ReadByte(BK_VBI_CE);
    
#if NTSC_WSS_CRC_CHECK
        
    MS_U32 idx = 0;
    MS_U8 u8Op = 0;
    MS_U8 u8CrcCode = 0x3f;
    MS_U8 u8CrcCheck = ((HAL_VBI_ReadByte(BK_VBI_CF) & 0xf) << 2) | (wWssWordH >> 6);
    MS_U16 u16Data = MAKEWORD(wWssWordH, wWssWordL);

    if(HAL_VBI_ReadByte(BK_AFEC_CD) & 0x10) // not NTSC
    {
        return MAKEWORD(wWssWordH, wWssWordL);
    }
    
    for(idx = 0; idx <14; idx++)
    {
        MS_U8 u8CrcTmp = u8CrcCode;
        u8CrcCode = 0;
        
        u8Op = (u8CrcTmp ^ (MS_U8)u16Data) & 0x1;
        u8CrcCode = (u8CrcTmp >> 1) & 0xf;                        // CRC[3:0]
        u8CrcCode |= (((u8CrcTmp >> 5) ^ u8Op) << 4);             // CRC[4]
        u8CrcCode |=  u8Op << 5;                                  // CRC[5]
        u16Data >>= 1;
    }
    
    if(u8CrcCheck == u8CrcCode)
    {
        return MAKEWORD(wWssWordH, wWssWordL);
    }
    else
    {
        return 0;
    }
#else
    return MAKEWORD(wWssWordH, wWssWordL);
#endif
}

void VBI_Set_PalNC_VideoStandard(void)
{
    HAL_VBI_WriteByte(BK_VBI_82, 0x10);
    HAL_VBI_WriteByte(BK_VBI_83, 0xB9);
    HAL_VBI_WriteByte(BK_VBI_89, HAL_VBI_ReadByte(BK_VBI_89)|_BIT6);
    // for VPS
    HAL_VBI_WriteByte(BK_VBI_89, HAL_VBI_ReadByte(BK_VBI_89)&(~(_BIT5)));
    HAL_VBI_WriteByte(BK_VBI_99, 0x8C);
    HAL_VBI_WriteByte(BK_VBI_9A, 0x01);
}

void VBI_Set_Secam_VideoStandard(void)
{
    HAL_VBI_WriteByte(BK_VBI_89, HAL_VBI_ReadByte(BK_VBI_89)|(_BIT6));
    // for VPS
    HAL_VBI_WriteByte(BK_VBI_89, HAL_VBI_ReadByte(BK_VBI_89)|_BIT5);
    HAL_VBI_WriteByte(BK_VBI_99, 0x6D);
    HAL_VBI_WriteByte(BK_VBI_9A, 0x9A);
}

void VBI_Set_Pal_VideoStandard(void)
{
    HAL_VBI_WriteByte(BK_VBI_82, 0x8E);
    HAL_VBI_WriteByte(BK_VBI_83, 0x6B);
    HAL_VBI_WriteByte(BK_VBI_89, HAL_VBI_ReadByte(BK_VBI_89)|_BIT6);
    // for VPS
    HAL_VBI_WriteByte(BK_VBI_89, HAL_VBI_ReadByte(BK_VBI_89)&(~(_BIT5)));
    HAL_VBI_WriteByte(BK_VBI_99, 0x8C);
    HAL_VBI_WriteByte(BK_VBI_9A, 0x01);
}

void VBI_TTX_EnableInterrupt(MS_BOOL bEnable)
{
    if(bEnable)
    {
        // TTX, VPS and WSS
        HAL_VBI_WriteByteMask(VBI_INTERRUPT_MASK, 0, _BIT4|_BIT1|_BIT0);
    }
    else
    {
        // TTX, VPS and WSS
        HAL_VBI_WriteByteMask(VBI_INTERRUPT_MASK, _BIT4|_BIT1|_BIT0, _BIT4|_BIT1|_BIT0);
    }
}

MS_U8 VBI_TTX_ReadIRQ(void)
{
    return HAL_VBI_ReadByte(VBI_INTERRUPT_STATUS);
}

void VBI_TTX_ClearIRQ(void)
{
    HAL_VBI_WriteByteMask(VBI_INTERRUPT_CLEAR, _BIT4|_BIT1|_BIT0, _BIT4|_BIT1|_BIT0);
    HAL_VBI_WriteByteMask(VBI_INTERRUPT_CLEAR, 0, _BIT4|_BIT1|_BIT0);
}

void VBI_TTX_EnableLine(MS_U16 StartLine, MS_U16 EndLine)
{
#define EVEN_FIELD_OFFSET   313

    EN_VBI_FIELD eField = VBI_FIELD_ODD;

    if(StartLine >= EVEN_FIELD_OFFSET)
        eField = VBI_FIELD_EVEN;

    switch(eField)
    {
        case VBI_FIELD_EVEN:
            HAL_VBI_WriteByteMask(BK_VBI_7E, StartLine - EVEN_FIELD_OFFSET, 0x1F);
            HAL_VBI_WriteByteMask(BK_VBI_7F, EndLine - EVEN_FIELD_OFFSET, 0x1F);
            break;

        case VBI_FIELD_ODD:
            HAL_VBI_WriteByteMask(BK_VBI_7C, StartLine, 0x1F);
            HAL_VBI_WriteByteMask(BK_VBI_7D, EndLine, 0x1F);
            break;
    }
}

MS_U8 TTX_DMA_CMD_MAPPING[]=
{
	DMA_HEADER,
	DMA_PACKET1_TO_25,
	DMA_PACKET26_28_29,
	DMA_PACKET27,
	DMA_BTT,
	DMA_AIT,
};

#if defined(__mips__)
static void _VBI_WaitDMAReady(void)
{
    MS_U8 i;

    for(i = 0; i<25; i++)
        __asm__ __volatile__ ("nop");

    while(1)
    {
        if(HAL_VBI_ReadByte(DMA_COMMAND) & DMA_READY)
        {
            break;
        }
    }
}
#elif defined(__aeon__)
static void _VBI_WaitDMAReady(void)
{
    MS_U8 i;

    for(i = 0; i<10; i++)
        __asm__ __volatile__ ("l.nop 0");

    while(1)
    {
        if(HAL_VBI_ReadByte(DMA_COMMAND) & DMA_READY)
        {
            break;
        }
    }
}
#elif defined(__arm__) ||  defined (__aarch64__)
static void _VBI_WaitDMAReady(void)
{
}
#else
    #error "Not support CPU!!"
#endif

void VBI_TTX_DMA_CopyPacket(MS_PHY src_addr, MS_PHY dest_addr, MS_U8 aPacketType)
{
    src_addr = src_addr>>3;


    HAL_VBI_Write2Byte(DMASRC_ADR_L, (src_addr) & 0xFFFF);
    HAL_VBI_WriteByte(DMASRC_ADR_H, (src_addr>>16) & 0xFF);

    if((src_addr>>24) & 0x01)
    {
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, HAL_VBI_ReadByte(VBI_BIT24_ADDR)|DMASRC_ADDR24);
    }
    else
    {
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~DMASRC_ADDR24));
    }
    dest_addr = dest_addr>>3;
    HAL_VBI_Write2Byte(DMADES_ADR_L, (dest_addr) & 0xFFFF);
    HAL_VBI_WriteByte(DMADES_ADR_H, (dest_addr>>16)  & 0xFF);

    if((dest_addr>>24) & 0x01)
    {
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, HAL_VBI_ReadByte(VBI_BIT24_ADDR)|DMADES_ADDR24);
    }
    else
    {
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~DMADES_ADDR24));
    }
    HAL_VBI_WriteByte(DMA_FUNC, TTX_DMA_CMD_MAPPING[aPacketType]);    // DMA Function 05 : AIT PAGE X/1~X/22

    HAL_VBI_Write2Byte(DMAQW_CNT_L, 5);     // DMA Counter (64Bit)
    HAL_VBI_WriteByte (DMA_COMMAND, DMA_FIRE);    // Fire
    _VBI_WaitDMAReady();
}

/******************************************************************************/
// API for VBI Slicer Initialization::
// Initializing VBI Slicer HW
/******************************************************************************/
void VBI_CC_Init(MS_VIRT ptrRiuAddr, MS_PHY phyAddr, MS_U16 u8Len)
{
    MS_U8 u8Tmp;

    // VBI RIU base
    _ptrVBIRiuBaseAddr = ptrRiuAddr;

    // U8 u8Bank; <- remove the bank change code
    if((phyAddr >> 3) >= (1 << CC_BUF_BIT))
    {
        printf("[VBI][%s] ERROR, buffer address out of bound\n", __FUNCTION__);
        //MS_ASSERT(0);
    }
    //reset and enable closed caption
    // u8Bank = XBYTE[BK_SELECT_00]; <- remove the bank change code
    // XBYTE[BK_SELECT_00] = REG_BANK_VBI; <- remove the bank change code
    HAL_VBI_WriteByte(BK_VBI_46, 0x00); //disable VBI

    /* --- setup CC Bytes buffer --- */
    /* set cc base address */
    //printf("\n init adr=0x%x, %d", u32Addr, u8Len);
    u8Tmp = HAL_VBI_ReadByte(BK_VBI_1D);
    u8Tmp &= ~(0xf0);
    u8Tmp |= (((phyAddr >> 27)&0x0f) << 4);
    HAL_VBI_WriteByte(BK_VBI_1D, u8Tmp); // CcBaseAddr_24, 25
    HAL_VBI_WriteByte(BK_VBI_5D, (phyAddr >> 19) & 0xFF); // CcBaseAddr_23_16 (default value is 0xFF so it needs to specify to 0x00)
    HAL_VBI_WriteByte(BK_VBI_5E, (phyAddr >> 11) & 0xFF); // CcBaseAddr_15_8 (high order)
    HAL_VBI_WriteByte(BK_VBI_5F, (phyAddr >> 3) & 0xFF);  // CCBaseAddr_7_0 (low order)

    /* set cc buffer length */
    HAL_VBI_WriteByte(BK_VBI_5C, u8Len);

    // CJ
    HAL_VBI_WriteByte(BK_AFEC_6B, HAL_VBI_ReadByte(BK_AFEC_6B) & 0xF7);

    // Set a constraint for CC patterns in case of wrong encoder's behavior
    HAL_VBI_WriteByte(BK_VBI_4A, (HAL_VBI_ReadByte(BK_VBI_4A)&0xf0)|0x03);

    HAL_VBI_WriteByte(BK_VBI_46, 0x01); //enable VBI

    // disable vbi software reset
    u8Tmp = HAL_VBI_ReadByte(BK_VBI_70);
    u8Tmp &= ~(0x08);
    HAL_VBI_WriteByte(BK_VBI_70, u8Tmp);
}

/******************************************************************************/
/// API to turn on VBI from YPbPr Initialization::
/// Initializing VBI Slicer HW
/******************************************************************************/
void VBI_CC_YPbPr_Init(MS_U8 cvbs_no)
{
    // Enable VD parts
    HAL_VBI_WriteByte(L_BK_ADC_ATOP(0x00), 0x09);                 // enable VD & YPbPr
    HAL_VBI_WriteByte(L_BK_ADC_ATOP(0x02), 0xf0|cvbs_no);  // (select VD_ymux for CVBS input from Y)
    HAL_VBI_WriteByte(L_BK_ADC_ATOP(0x04), 0x00);                 // enable analog blocks, 04, 05
    HAL_VBI_WriteByte(H_BK_ADC_ATOP(0x04), 0x00);
    HAL_VBI_WriteByte(L_BK_ADC_ATOP(0x05), 0x00);
    HAL_VBI_WriteByte(L_BK_ADC_ATOP(0x06), 0x00);                 // enable ADC clocks
    HAL_VBI_WriteByte(L_BK_ADC_ATOP(0x12), 0x01);                 // VD pll =2X (16Fsc)

    // enable VD clocks, setup VD AFEC, AFEC and VBI are just set as AV mode
    HAL_VBI_WriteByte(H_BK_CHIPTOP(0x16), (HAL_VBI_ReadByte(H_BK_CHIPTOP(0x16)) & (0x0F)));
    HAL_VBI_WriteByte(L_BK_CHIPTOP(0x17), 0x06);
}

/******************************************************************************/
/// API to Set CC data rate::
/// Set CC's data rate
/// @Param  u8Mode \b IN video system mode
/// @return TRUE:: Successfully set
///         FALSE:: Fail
/******************************************************************************/
MS_U8 VBI_CC_DataRateSet(MS_U8 *ptable)
{
    MS_U8 j=1;

    // Set the VBI registers
    HAL_VBI_WriteByte(BK_VBI_41, ptable[j++]);

    HAL_VBI_WriteByte(BK_VBI_42, HAL_VBI_ReadByte(BK_VBI_42)&0xC0);
    HAL_VBI_WriteByte(BK_VBI_42, HAL_VBI_ReadByte(BK_VBI_42)|ptable[j++]);

    HAL_VBI_WriteByte(BK_VBI_44, ptable[j++]);

    HAL_VBI_WriteByte(BK_VBI_4B, HAL_VBI_ReadByte(BK_VBI_4B)&0xC0);
    HAL_VBI_WriteByte(BK_VBI_4B, HAL_VBI_ReadByte(BK_VBI_4B)|ptable[j++]);

    HAL_VBI_WriteByte(BK_VBI_4D, ptable[j++]);

    HAL_VBI_WriteByte(BK_VBI_50, HAL_VBI_ReadByte(BK_VBI_50)&0xE0);
    HAL_VBI_WriteByte(BK_VBI_50, HAL_VBI_ReadByte(BK_VBI_50)|ptable[j++]);

    HAL_VBI_WriteByte(BK_VBI_51, HAL_VBI_ReadByte(BK_VBI_51)&0xE0);
    HAL_VBI_WriteByte(BK_VBI_51, HAL_VBI_ReadByte(BK_VBI_51)|ptable[j++]);

    return TRUE;
}

/******************************************************************************/
/// API to get packet count:
///
/******************************************************************************/
MS_U8 VBI_CC_GetPacketCnt(void)
{
    return (HAL_VBI_ReadByte(BK_VBI_5B)&0x1F);
}

/******************************************************************************/
/// API to get odd/even byte-found-identification:
/// 2 bits
/******************************************************************************/
MS_U8 VBI_CC_GetByteFoundIndication(void)
{
    return ((HAL_VBI_ReadByte(BK_VBI_56)&0xC0) >> 6);
}

/******************************************************************************/
/// API to get packet data:
///
/// @return (U32) (Odd_1st_Byte + Odd_2nd_Byte + Even_1st_Byte + Even_2nd_Byte)
/******************************************************************************/
MS_U32 VBI_CC_GetPacket(void)
{
    MS_U32 u32Tmp;

    u32Tmp  = ((MS_U32)HAL_VBI_ReadByte(BK_VBI_57)) << 24;
    u32Tmp |= ((MS_U32)HAL_VBI_ReadByte(BK_VBI_58)) << 16;
    u32Tmp |= ((MS_U32)HAL_VBI_ReadByte(BK_VBI_59)) << 8;
    u32Tmp |= ((MS_U32)HAL_VBI_ReadByte(BK_VBI_5A)) << 0;

    return (u32Tmp);
}

/******************************************************************************/
/// API to set CC frame count:
///
/******************************************************************************/
void VBI_SetCCFrameCnt(MS_U8 cnt)
{
    MS_U8 u8Tmp=0;

    u8Tmp = HAL_VBI_ReadByte(BK_VBI_46);
    u8Tmp &= ~(_BIT1 | _BIT2 | _BIT3 | _BIT4 | _BIT5);
    u8Tmp |= ((cnt&0x1F) << 1);

    HAL_VBI_WriteByte(BK_VBI_46, u8Tmp);
}

/******************************************************************************/
/// API to enable CC slicer:
///
/******************************************************************************/
void VBI_CC_EnableSlicer(MS_BOOL bEnable)
{
    if(bEnable)
    {
        HAL_VBI_WriteByte(BK_VBI_46, HAL_VBI_ReadByte(BK_VBI_46) | _BIT0);
    }
    else
    {
        HAL_VBI_WriteByte(BK_VBI_46, HAL_VBI_ReadByte(BK_VBI_46) & 0xFE);
    }
}

void VBI_CC_SetCCLine(MS_U16 StartLine, MS_U16 EndLine, MS_U8 val) // val -> 0:NTSC, 1:PAL
{
#define CC_EVEN_FIELD_OFFSET        263 // (525 / 2 + 1)
#define VBI_CC_NTSC_LINE_OFFSET     3
#define VBI_CC_NTSC_DEFAULT_LINE    21
    
    EN_VBI_FIELD eField = VBI_FIELD_ODD;
    MS_U8 u8Tmp;

    if(StartLine >= CC_EVEN_FIELD_OFFSET)
        eField = VBI_FIELD_EVEN;

    if(val == 0)
    {
        val = VBI_CC_NTSC_LINE_OFFSET;    // offset
    }
    else
    {
        val = 0;    // offset
    }

    switch(eField)
    {
        case VBI_FIELD_EVEN:
            HAL_VBI_WriteByteMask(BK_VBI_50, (StartLine - CC_EVEN_FIELD_OFFSET), 0x1F);
            HAL_VBI_WriteByteMask(BK_VBI_51, (EndLine - CC_EVEN_FIELD_OFFSET), 0x1F);
            break;

        case VBI_FIELD_ODD:

            // start line
            u8Tmp = HAL_VBI_ReadByte(BK_VBI_40);
            u8Tmp &= ~(_BIT4 | _BIT5);
            u8Tmp |= ((StartLine & 0x18) << 1);
            HAL_VBI_WriteByteMask(BK_VBI_40, u8Tmp, 0x30);
            
            u8Tmp = HAL_VBI_ReadByte(BK_VBI_41);
            u8Tmp &= ~(_BIT5 | _BIT6 | _BIT7);
            u8Tmp |= ((StartLine & 0x07) << 5);
            HAL_VBI_WriteByteMask(BK_VBI_41, u8Tmp, 0xE0);

            // end line
            HAL_VBI_WriteByteMask(BK_VBI_41, EndLine, 0x1F);
            break;
    }
}
/******************************************************************************/
/// API to set CC SC window length
///
/******************************************************************************/
MS_BOOL VBI_CC_SetSCWindowLen(MS_U8 u8Len)
{
    HAL_VBI_WriteByte(BK_VBI_53, u8Len);
    return TRUE;
}
/******************************************************************************/
/// API to set WSS VPS byte number:
///
/******************************************************************************/
void VBI_SetWssVpsByteNum(MS_U8 cnt)
{
    MS_U8 u8Tmp=0;

    u8Tmp = HAL_VBI_ReadByte(BK_VBI_B8);
    u8Tmp &= ~(_BIT0 | _BIT1 | _BIT2 | _BIT3);
    u8Tmp |= (cnt&0x0f);

    HAL_VBI_WriteByte(BK_VBI_B8, u8Tmp);
}

/******************************************************************************/
/// API to enable memory protect (the memory range vbi can access)
///
/******************************************************************************/
MS_BOOL VBI_ProtectMemory(MS_BOOL bEnable, MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_U32 u32MiuAddr = (MS_U32)phyAddr >> 3;
    MS_U32 u32MiuAddrMax = ( ( (MS_U32)phyAddr + u32Size ) >> 3 );
    
    if(bEnable)
    {
        HAL_VBI_WriteByte(BK_VBI_DE, u32MiuAddr & 0xFF);
        HAL_VBI_WriteByte(BK_VBI_DF, (u32MiuAddr >> 8) & 0xFF);        
        HAL_VBI_WriteByte(BK_VBI_E0, (u32MiuAddr >> 16) & 0xFF);        
        
        HAL_VBI_WriteByte(BK_VBI_E1, u32MiuAddrMax & 0xFF);
        HAL_VBI_WriteByte(BK_VBI_E2, (u32MiuAddrMax >> 8) & 0xFF);        
        HAL_VBI_WriteByte(BK_VBI_E3, (u32MiuAddrMax >> 16) & 0xFF);        
        
#if BK_VBI_E7_BUG
        HAL_VBI_WriteWord(BK_VBI_E6, ( (u32MiuAddr >> 24) & 0x0F ) | 
                                     ( ( (u32MiuAddrMax >> 24) & 0x0F ) << 4));
#else
        HAL_VBI_WriteByte(BK_VBI_E7, ( (u32MiuAddr >> 24) & 0x0F ) | 
                                     ( ( (u32MiuAddrMax >> 24) & 0x0F ) << 4));        
#endif        
        HAL_VBI_WriteByteMask(BK_VBI_E4, _BIT1, _BIT1);
    }
    else
    {
        HAL_VBI_WriteByteMask(BK_VBI_E4, 0, _BIT1);
    }

    return TRUE;
}
#undef _HAL_VBI_C
