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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>

// Common Definition
#include "MsCommon.h"

// Internal Definition
#include "regPARFLASH.h"
#include "halPARFLASH.h"
//#include "drvPARFLASH.h"
//#include "regSerFlash.h"
//#include "halSerFlash.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define WRITE_WORD_MASK(_reg, _val, _mask)  { (*((volatile MS_U16*)(_reg))) = ((*((volatile MS_U16*)(_reg))) & ~(_mask)) | ((MS_U16)(_val) & (_mask)); }

// PFSH
#define PFSH_READ(addr)                     READ_WORD((_hal_pfsh.u32PfshBaseAddr + (addr << 2)))
#define PFSH_WRITE(addr, val)               WRITE_WORD((_hal_pfsh.u32PfshBaseAddr + (addr << 2)), (val))
#define PFSH_WRITE_MASK(addr, val, mask)    WRITE_WORD_MASK((_hal_pfsh.u32PfshBaseAddr + (addr << 2)), (val), (mask))

#define CHIPTOP_WRITE(addr, val)               WRITE_WORD((_hal_pfsh.u32ChipBaseAddr + (addr << 2)), (val))
#define CHIPTOP_WRITE_MASK(addr, val, mask)    WRITE_WORD_MASK(_hal_pfsh.u32ChipBaseAddr + ((addr)<<2), (val), (mask))

#define PIU_READ(addr)                     	READ_WORD((_hal_pfsh.u32PiuBaseAddr + (addr << 2)))
#define PIU_WRITE(addr, val)               	WRITE_WORD((_hal_pfsh.u32PiuBaseAddr + (addr << 2)), (val))
#define PIU_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK(_hal_pfsh.u32PiuBaseAddr + ((addr)<<2), (val), (mask))


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32  u32ChipBaseAddr;     // REG_PFSH_BASE
    MS_U32  u32PfshBaseAddr;     // REG_PFSH_BASE
    MS_U32  u32PiuBaseAddr;      // REG_PIU_BASE
} ST_HAL_PFSH_BASE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8 _u8ParFlashDbgLevel;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static ST_HAL_PFSH_BASE _hal_pfsh = // TODO: review, it would be init in HAL_SerFlash_Config()
{
    .u32ChipBaseAddr = BASEADDR_RIU + BK_CHIPTOP,
    .u32PfshBaseAddr = BASEADDR_RIU + BK_PFSH,
    .u32PiuBaseAddr  = BASEADDR_RIU + BK_PIU,
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _HAL_PARFLASH_Delay(void)
{
    int i;

    for (i = 0; i < 10; i ++)
    {
    // TODO: review, <-@@@ MCU SPECIFIC
#ifdef MCU_AEON
        __asm__ __volatile__("l.nop");
#else // MCU_MIPS_4KE, MCU_MIPS_34K
        asm volatile
        (
            "nop;"
        );
#endif
    }
}

static MS_BOOL _HAL_PARFLASH_WaitReady(void)
{
    MS_U16 u16data = 0;
    MS_U32 u32timeout = 0;

    while (u16data == 0)
    {
        _HAL_PARFLASH_Delay();

        u16data = (PFSH_READ(REG_NORPF_XIU_STATUS) & 0x01);

        if (++u32timeout > 0x10000)
        {
            printf("cmd ready timeout\n");
            return FALSE ; // timeout
        }
    }

    return TRUE ; // Success
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void HAL_PARFLASH_Config(MS_U32 u32PMRegBase, MS_U32 u32RegBaseAddr)
{
    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("%s(0x%08X)\n", __FUNCTION__, (int)u32RegBaseAddr));
    _hal_pfsh.u32ChipBaseAddr = u32RegBaseAddr + BK_CHIPTOP;
    _hal_pfsh.u32PfshBaseAddr = u32RegBaseAddr + BK_PFSH;
    _hal_pfsh.u32PiuBaseAddr  = u32PMRegBase + BK_PIU;

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("u32ChipBaseAddr: 0x%x, u32PfshBaseAddr 0x%x, u32PiuBaseAddr 0x%x\n",
                (unsigned int)_hal_pfsh.u32ChipBaseAddr, (unsigned int)_hal_pfsh.u32PiuBaseAddr, (unsigned int)_hal_pfsh.u32PiuBaseAddr));
}

//--- return 1: Byte mode ----
//--- return 0: Word mode ----
MS_BOOL HAL_PARFLASH_SelectChip(MS_U8 u8cs)
{
    MS_U8 u8temp;

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%d)\n", __FUNCTION__, (int)u8cs));

    PFSH_WRITE_MASK(REG_NORPF_CTRL, BITS(0:0, u8cs), BMASK(0:0));
    u8temp = (MS_U8)(PFSH_READ(REG_NORPF_FLASH_DATA_WIDTH8) & 0x80) >> 7;	

    return ((MS_BOOL)u8temp);
}

//pbMode: Fill flash commend mode
void HAL_PARFLASH_Init(MS_BOOL *pbMode)
{
    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    //set SPI clock
    /*PIU_WRITE_MASK(REG_PIU_SPI_CLK_SRC, PSCS_CLK_SRC_SEL_XTAL,  PSCS_CLK_SRC_SEL_MASK); // PIU_REG16(REG_PIU_SPI_CLK_SRC) &= ~(1<<5); // Xtal
   	 PIU_WRITE_MASK(REG_PIU_SPI_CLK_SRC, PSCS_CLK_SEL_36MHZ,     PSCS_CLK_SEL_MASK);     // PIU_REG16(REG_PIU_SPI_CLK_SRC)  = (PIU_REG16(REG_PIU_SPI_CLK_SRC) & ~0x1F) | 0x08; //36 Mhz
   	 PIU_WRITE_MASK(REG_PIU_SPI_CLK_SRC, PSCS_CLK_SRC_SEL_CLK,   PSCS_CLK_SRC_SEL_MASK); // PIU_REG16(REG_PIU_SPI_CLK_SRC) |= (1<<5); // clk_sel
	*/
    CHIPTOP_WRITE(REG_PCMCONFIG, 0); //disable CI pad
    CHIPTOP_WRITE_MASK(REG_PCMISGPIO, BITS(5:0, 0), BMASK(5:0)); 
    CHIPTOP_WRITE_MASK(REG_ALLPAD_IN, BITS(15:15, 0), BMASK(15:15)); //Disable all pad in
    CHIPTOP_WRITE_MASK(REG_PF_MODE, BITS(4:4, 0), BMASK(4:4)); 

    *pbMode = HAL_PARFLASH_SelectChip(0);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("Byte mode %d\n", *pbMode));

    PFSH_WRITE_MASK(REG_NORPF_WEB_END, BITS(7:0, 0xB3), BMASK(7:0)); // RIU[REG_NORPF_WEB_END] = (RIU[REG_NORPF_WEB_END] & 0xFF00) | 0xB3; // d -> b, finish WEB eariler for the verilog model
}

MS_BOOL HAL_PARFLASH_PrepareCmdWrite(MS_U8 u8cmdnum, MS_U32 *pu32cmdadr, MS_U16* pu16cmddata)
{
    MS_U8 u8ii;
    MS_U16 u16temp, u16regaddr, u16regdata;

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%d,%p,%p)\n", __FUNCTION__, (int)u8cmdnum, pu32cmdadr,pu16cmddata));

    //write commend cycles
    u16regaddr = REG_NORPF_WRITE_ADDR0_L;
    u16regdata = REG_NORPF_WRITE_DATA0;
    for(u8ii = 0; u8ii < u8cmdnum; u8ii++)
    {
        u16temp = (MS_U16)(pu32cmdadr[u8ii] & 0xFFFF);
        PFSH_WRITE(u16regaddr++, u16temp);
        u16temp = (MS_U16)((pu32cmdadr[u8ii] & 0xFFFF0000) >> 16);
        PFSH_WRITE(u16regaddr++, u16temp);

        PFSH_WRITE(u16regdata++, pu16cmddata[u8ii]);
    }

    return TRUE ;
}

MS_BOOL HAL_PARFLASH_LastCmdTrig(MS_U8 u8runs, MS_U32 u32lastaddr, MS_U16 u16lastdata)
{
    MS_U16 u16temp;

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%d, 0x%08x, 0x%04x)\n", __FUNCTION__, (int)u8runs, (unsigned int)u32lastaddr, (unsigned short)u16lastdata));

    if(u8runs > PFSH_MAX_CMDRUN)
        return FALSE;

    u16temp = PFSH_READ(REG_NORPF_WRITE_RUN) | 0x10; //direct mode
    u16temp = (u16temp & ~0x07) | (MS_U16)(u8runs - 1); // set runs, direct mode
    PFSH_WRITE(REG_NORPF_WRITE_RUN, u16temp);

    u16temp = (MS_U16)(u32lastaddr & 0xFFFF);
    PFSH_WRITE(REG_NORPF_XIU_ADDR_L, u16temp);
    u16temp = (MS_U16)((u32lastaddr & 0xFFFF0000) >> 16);
    PFSH_WRITE(REG_NORPF_XIU_ADDR_H, u16temp);
    PFSH_WRITE(REG_NORPF_XIU_WDATA, u16lastdata);

    // XIU Write Trigger
    PFSH_WRITE(REG_NORPF_XIU_CTRL, 0xA);
    PFSH_WRITE(REG_NORPF_XIU_CTRL, 0xB);

    if(_HAL_PARFLASH_WaitReady() == FALSE)
        return FALSE;

    return TRUE;
}

MS_BOOL HAL_PARFLASH_Cmd_Write(MS_U8 u8runs, MS_U32 *pu32cmdadr, MS_U16* pu16cmddata)
{
    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%d,0x%x,0x%x)\n", __FUNCTION__, (int)u8runs, (unsigned int)(*pu32cmdadr), (unsigned int)(*pu16cmddata)));

    if(u8runs > PFSH_MAX_CMDRUN)
        return FALSE;

    if(!HAL_PARFLASH_PrepareCmdWrite(u8runs - 1, pu32cmdadr, pu16cmddata))
        return FALSE;

    if(!HAL_PARFLASH_LastCmdTrig(u8runs, pu32cmdadr[u8runs - 1], pu16cmddata[u8runs - 1]))
        return FALSE;

    return TRUE ;
}

MS_BOOL HAL_PARFLASH_Read(MS_U32 u32addr, MS_U16* pu16data)
{
    *pu16data = 0;

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(0x%08x,%p)\n", __FUNCTION__, (unsigned int)u32addr, (void*)pu16data));

    // XIU Read Trigger
    PFSH_WRITE(REG_NORPF_XIU_ADDR_L, (MS_U16)(u32addr & 0xFFFF));
    PFSH_WRITE(REG_NORPF_XIU_ADDR_H, (MS_U16)((u32addr & 0xFFFF0000) >> 16));
    PFSH_WRITE(REG_NORPF_XIU_CTRL, 0xC);
    PFSH_WRITE(REG_NORPF_XIU_CTRL, 0xD);

    if(_HAL_PARFLASH_WaitReady() == FALSE)
        return FALSE;

    *pu16data = PFSH_READ(REG_NORPF_XIU_RDATA_L);

    return TRUE;
}

