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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halRASP.c
// @brief  RASP HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#endif
#include "MsCommon.h"
#include "regNDSRASP.h"
#include "halNDSRASP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_U32       _u32RegBase                        = 0;

static REG_RASP*    _RASPReg[RASP_NUM]                 = {NULL , NULL};
static MS_U32       _u32RASP_PidfltBase[RASP_NUM]      = {NULL , NULL};
static MS_U32       _u32RASP_EcmPidfltBase[RASP_NUM]   = {NULL , NULL};
static MS_U32       _u32RASP_EventMaskBase[RASP_NUM]   = {NULL , NULL};
static MS_U32       _u32RASP_PayloadMaskBase[RASP_NUM] = {NULL , NULL};
static MS_U32       _g32RASPHalDbgLv = RASP_DBGLV_DEBUG;

// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_W(reg, value)    {   \
                                        do { (reg)->L = ((value) & 0x0000FFFF);                          \
                                        (reg)->H = ((value) >> 16); } while(0); }

#define _HAL_REG16_W(reg, value)    {    \
                                        (reg)->data = ((value) & 0x0000FFFF); }


#define RASP_NUM_CHECK(idx) if( (MS_U32)idx >= (MS_U32)RASP_NUM ) \
                            { printf("[RASP][ERR] Rasp Engine [%d] not exist ! \n",idx);   \
                             return ; }

#define RASP_NUM_CHECK_RET(idx) if( (MS_U32)idx >= (MS_U32)RASP_NUM ) \
                            { printf("[RASP][ERR] Rasp Engine [%d] not exist ! \n",idx);   \
                             return 0; }

#define HALRASP_DBG(lv, x, args...)   if (lv <= _g32RASPHalDbgLv ) \
                                        { printf("%s, ",__FUNCTION__); \
                                          printf(x, ##args); }
////////////////////////////////////////////////////////////////////////////////
// Local variable
////////////////////////////////////////////////////////////////////////////////


//K2 RASP1 MIU setting
static REG16* _REG_RASP1_MIU;
#define _REG_RASP1_MIU_BASE (0x2460)
#define _RASP1_MIU_EN  (0x4000)

static MS_U32 _gRaspFileIn_Addr[RASP_NUM];
static MS_U32 _gRaspEng_Addr[RASP_NUM];

static MS_U32 _gChipTp_Addr;
static MS_U32 _gClkGen_Addr;
static MS_U32 _gClkGen2_Addr;

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////

void HAL_RFILEIN_WriteReg_Word(MS_U32 rasp_eng, MS_U32 u32RegAddr, MS_U16 u16Data)
{
    MS_U32 u32reg;
    u32reg = (u32RegAddr*4) + _gRaspFileIn_Addr[rasp_eng];
    (*(volatile MS_U16*)(u32reg)) = u16Data;

}

MS_U16 HAL_RFILEIN_ReadReg_Word(MS_U32 rasp_eng, MS_U32 u32RegAddr)
{
    MS_U32 u32reg;
    MS_U16 u16Data;
    u32reg = (u32RegAddr*4) + _gRaspFileIn_Addr[rasp_eng];
    u16Data = (*(volatile MS_U16*)(u32reg));

    return u16Data;
}

void HAL_RFILEIN_WriteReg_DWord(MS_U32 rasp_eng, MS_U32 u32RegAddr, MS_U32 u32Data)
{
    MS_U32 u32reg;

    u32reg = (u32RegAddr*4) + _gRaspFileIn_Addr[rasp_eng];
    (*(volatile MS_U16*)(u32reg)) = (MS_U16)(u32Data&0xffff);
	//printf("(reg,value) = (%x,%x)\n", u32RegAddr, *(volatile MS_U16*)(u32reg) );

    u32reg += 4;
    (*(volatile MS_U16*)(u32reg)) = (MS_U16)((u32Data>>16)&0xffff);
    //printf("(reg,value) = (%x,%x)\n", u32RegAddr+1, *(volatile MS_U16*)(u32reg) );
}

MS_U32 HAL_RFILEIN_ReadReg_DWord(MS_U32 rasp_eng, MS_U32 u32RegAddr)
{
    MS_U32 u32reg,u32Data;
    MS_U16 u16Data1,u16Data2;

    u32reg = (u32RegAddr*4) + _gRaspFileIn_Addr[rasp_eng];
    u16Data1 = (*(volatile MS_U16*)(u32reg));

    u32reg += 4;
    u16Data2 = (*(volatile MS_U16*)(u32reg));
    u32Data = (MS_U32)(u16Data1) + ( ((MS_U32)u16Data2)<<16 );

    return u32Data;
}

void HAL_RASP_WriteReg_Word(MS_U32 rasp_eng, MS_U32 u32RegAddr, MS_U16 u16Data)
{
    MS_U32 u32reg;
    u32reg = (u32RegAddr*4) + _gRaspEng_Addr[rasp_eng];
    (*(volatile MS_U16*)(u32reg)) = u16Data;

    //printf("(reg,value) = (%x,%x)\n",u32reg,u16Data);
}

MS_U16 HAL_RASP_ReadReg_Word(MS_U32 rasp_eng, MS_U32 u32RegAddr)
{
    MS_U32 u32reg;
    MS_U16 u16Data;
    u32reg = (u32RegAddr*4) + _gRaspEng_Addr[rasp_eng];
    u16Data = (*(volatile MS_U16*)(u32reg));

    return u16Data;
}

#if 0
void HAL_NDSRASP_WriteOtherBank_Word(MS_U32 bank, MS_U32 u32RegAddr, MS_U16 u16Data)
{
    MS_U32 u32reg;
	MS_U16 temp;

    u32reg = (u32RegAddr*4) + bank;
	temp = (*(volatile MS_U16*)(u32reg));
	temp |= u16Data;

	(*(volatile MS_U16*)(u32reg)) = temp;

}

void HAL_NDSRASP_MaskOtherBank_Word(MS_U32 bank, MS_U32 u32RegAddr, MS_U16 u16MaskData)
{
    MS_U32 u32reg;
	MS_U16 temp;

    u32reg = (u32RegAddr*4) + bank;
	temp = (*(volatile MS_U16*)(u32reg));
	temp &= u16MaskData;

	(*(volatile MS_U16*)(u32reg)) = temp;

}
#endif

void HAL_RASP_WriteReg_DWord(MS_U32 rasp_eng, MS_U32 u32RegAddr, MS_U32 u32Data)
{
    MS_U32 u32reg;

    u32reg = (u32RegAddr*4) + _gRaspEng_Addr[rasp_eng];
    (*(volatile MS_U16*)(u32reg)) = (MS_U16)(u32Data&0xffff);

    u32reg += 4;
    (*(volatile MS_U16*)(u32reg)) = (MS_U16)((u32Data>>16)&0xffff);
}

MS_U32 HAL_RASP_ReadReg_DWord(MS_U32 rasp_eng, MS_U32 u32RegAddr)
{
    MS_U32 u32reg,u32Data;
    MS_U16 u16Data1,u16Data2;

    u32reg = (u32RegAddr*4) + _gRaspEng_Addr[rasp_eng];
    u16Data1 = (*(volatile MS_U16*)(u32reg));

    u32reg += 4;
    u16Data2 = (*(volatile MS_U16*)(u32reg));
    u32Data = (u16Data1) + ((MS_U32)u16Data2<<16);

    return u32Data;
}

#if 0
static void _delay(void)
{
    volatile MS_U32 i;
    for (i = 0; i< 0xFFFF; i++);
}
#endif

#ifdef REG32__
static MS_U32 _HAL_REG32_R(REG32 *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}
#endif

static MS_U16 _HAL_REG16_R(REG16 *reg)
{
    MS_U16     value;
    value = (reg)->data;
    return value;
}

//--------------------------------------------------------------------------------------------------
// For MISC part
//--------------------------------------------------------------------------------------------------
void HAL_NDSRASP_SetBank(MS_U32 u32BankAddr)
{
    HALRASP_DBG(RASP_DBGLV_ALERT,"u32BankAddr = %x\n",u32BankAddr);
    _u32RegBase                 = u32BankAddr;
    _RASPReg[0]                = (REG_RASP*)(_u32RegBase + RASP0_BANK0_REG_CTRL_BASE);
    _u32RASP_PidfltBase[0]        = _u32RegBase + RASP0_BANK0_PIDFLT_BASE;
    _u32RASP_EventMaskBase[0]     = _u32RegBase + RASP0_BANK1_EVENT_MASK_BASE;
    _u32RASP_PayloadMaskBase[0]   = _u32RegBase + RASP0_BANK1_PAYLOAD_MASK_BASE;
    _u32RASP_EcmPidfltBase[0]     = _u32RegBase + RASP0_BANK1_ECM_PIDFLT_BASE;

    _RASPReg[1]                = (REG_RASP*)(_u32RegBase + RASP1_BANK0_REG_CTRL_BASE);
    _u32RASP_PidfltBase[1]        = _u32RegBase + RASP1_BANK0_PIDFLT_BASE;
    _u32RASP_EventMaskBase[1]     = _u32RegBase + RASP1_BANK1_EVENT_MASK_BASE;
    _u32RASP_PayloadMaskBase[1]   = _u32RegBase + RASP1_BANK1_PAYLOAD_MASK_BASE;
    _u32RASP_EcmPidfltBase[1]     = _u32RegBase + RASP1_BANK1_ECM_PIDFLT_BASE;
     //K2 RASP1 MIU setting
    _REG_RASP1_MIU = (REG16*)(_u32RegBase + _REG_RASP1_MIU_BASE);

     _gRaspFileIn_Addr[0] = _u32RegBase + REG_RASP0_FILE_BASE;
     _gRaspFileIn_Addr[1] = _u32RegBase + REG_RASP1_FILE_BASE;

     _gRaspEng_Addr[0] = _u32RegBase + REG_RASP0_BASE;
     _gRaspEng_Addr[1] = _u32RegBase + REG_RASP1_BASE;

	 _gChipTp_Addr = _u32RegBase + REG_CHIPTOP_RP_BASE;
	 _gClkGen_Addr = _u32RegBase + REG_CLKGEN_RP_BASE;
     _gClkGen2_Addr = _u32RegBase + REG_CLKGEN2_BASE;
}

static void HAL_NDSRASP_Str2MIU_Reset(MS_U32 u32RASPEng)
{
    MS_U16 HWCtrl1;
    HWCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL1);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 | RASP_STR2MIU_RST_WADDR));
    MsOS_DelayTaskUs(10);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 & ~(RASP_STR2MIU_RST_WADDR)) );
}

static void HAL_NDSRASP_ECM2MIU_Reset(MS_U32 u32RASPEng)
{
    MS_U16 HWCtrl3;
    HWCtrl3 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL3);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 | RASP_ECM2MIU_RST_WADDR));
    MsOS_DelayTaskUs(10);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 & ~(RASP_ECM2MIU_RST_WADDR)) );
}

static void HAL_NDSRASP_PAYLD2MIU_Reset(MS_U32 u32RASPEng)
{
    MS_U16 HWCtrl3;
    HWCtrl3 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL3);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 | RASP_PAYLD2MIU_RST_WADDR));
    MsOS_DelayTaskUs(10);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 & ~(RASP_PAYLD2MIU_RST_WADDR)) );
}

void HAL_NDSRASP_Init(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x\n", u32RASPEng);

    RASP_NUM_CHECK(u32RASPEng);
#if 1
    MS_U32 addr;
	addr = _u32RegBase + (0x161300*2 + 0x00*4);		//$ enable MIUCrossbar
    (*((volatile MS_U16*)(addr))) = 0x000f;

    addr = _u32RegBase + (0x161300*2 + 0x1b*4);		//$ enable gate clock
    (*((volatile MS_U16*)(addr))) = 0x0003;
#endif


    MS_U16 HWCtrl0;
    //Clear all ctrl
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, 0x0);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, 0x0);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, 0x0);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, 0x0);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL4, 0x0);

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_CORPT_PKTSIZE, (RASP_PKT_SIZE_188<<RASP_PKT_SHIFT) );

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, RASP_SW_RESET);

    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL0);

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 | RASP_RISING_SYNC_DETECT|RASP_FALLING_VALID_DETECT) );

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, RASP_SERIAL_EXT_SYNC_1T);

#if 0
    //need to change....
    if(u32RASPEng == 1)
    {
        //K2 RASP1 MIU setting
        _HAL_REG16_W((REG16*)_REG_RASP1_MIU, SET_FLAG1(_HAL_REG16_R((REG16*)_REG_RASP1_MIU), _RASP1_MIU_EN));
    }
#endif

    MS_U16 i;
    for(i=0;i<=RASP_PIDFLT_NUM;i++)
        HAL_NDSRASP_SetPidflt(u32RASPEng, i, 0x1fff);


    HAL_NDSRASP_Str2MIU_Reset(u32RASPEng);

    MS_U16 HWCtrl1;

    HWCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL1);
    HWCtrl1 |= (RASP_ALT_TS_SIZE_EN | RASP_STR2MIU_EN | RASP_PINGPONG_EN);
    //HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, HWCtrl1);

	HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL0);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 | RASP_TSIF2_ENABLE) );

    HAL_NDSRASP_SetECM_Init_1(u32RASPEng);

}

void HAL_NDSRASP_Exit(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "uRASPEng = %x\n", u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);

    MS_U16 HWCtrl0;
    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL0);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 & ~RASP_SW_RESET));

    //need to change....
    if(u32RASPEng == 1)
    {
        //K2 RASP1 MIU setting
        _HAL_REG16_W((REG16*)_REG_RASP1_MIU, RESET_FLAG1(_HAL_REG16_R((REG16*)_REG_RASP1_MIU), _RASP1_MIU_EN));
    }
}

void HAL_NDSRASP_Pvr_Enable(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "uRASPEng = %x\n", u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);

    HAL_NDSRASP_Str2MIU_Reset(u32RASPEng);

    MS_U16 HWCtrl1;

    HWCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL1);
    HWCtrl1 |= (RASP_ALT_TS_SIZE_EN | RASP_STR2MIU_EN | RASP_PINGPONG_EN);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, HWCtrl1);

    HAL_NDSRASP_Str2MIU_Reset(u32RASPEng);

    //MS_U16 HWCtrl0;
    //HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL0);
    //HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 | RASP_TSIF2_ENABLE) );
}


void HAL_NDSRASP_ECM_Enable(MS_U32 u32RASPEng , MS_BOOL enable )
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x, enable = %d\n", u32RASPEng,enable);
    RASP_NUM_CHECK(u32RASPEng);

    MS_U16 HWCtrl2,HWCtrl3;
    HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL2);
    HWCtrl3 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL3);

    if (enable)
    {
        HAL_NDSRASP_ECM2MIU_Reset(u32RASPEng);

        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 | RASP_ECM_BURST_LEN) );

        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 | RASP_ECM2MIU_EN) );
    }
    else
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 & (~RASP_ECM2MIU_EN)) );
    }
}

void HAL_NDSRASP_Payload_Enable(MS_U32 u32RASPEng ,MS_BOOL enable )
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x, enable = %d\n", u32RASPEng,enable);
    RASP_NUM_CHECK(u32RASPEng);

    MS_U16 HWCtrl2,HWCtrl3;
    HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL2);
    HWCtrl3 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL3);

#if 1
    if (enable)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 | RASP_PAYLD2MIU_RST_WADDR) );
        MsOS_DelayTaskUs(10);
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 & (~RASP_PAYLD2MIU_RST_WADDR)) );

        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 | RASP_PAYLOAD_BURST_LEN) );

        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 | RASP_PAYLD2MIU_EN | RASP_PAYLD2MIU_PINGPONE) );
    }
    else
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 & (~RASP_PAYLD2MIU_EN)) );
    }
#else
    if (enable)
    {
        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3,
        SET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3), RASP_PAYLD2MIU_RST_WADDR));
        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3,
        RESET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3), RASP_PAYLD2MIU_RST_WADDR));

        //timo
        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL2,
        SET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL2), RASP_PAYLOAD_BURST_LEN));

        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3,
        SET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3), RASP_PAYLD2MIU_EN));
        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3,
        SET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3), RASP_PAYLD2MIU_PINGPONE));
    }
    else
    {
        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3,
                RESET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3), RASP_PAYLD2MIU_EN));

    }
#endif
}




void HAL_NDSRASP_Stop(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x\n", u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);
    MS_U16 HWCtrl1;
    HWCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL1);

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 & (~RASP_STR2MIU_EN)) );
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 & (~RASP_TSIF2_ENABLE)) );
}

void HAL_NDSRASP_Pause(MS_U32 u32RASPEng , MS_BOOL bPause)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x, bPause = %d\n", u32RASPEng,bPause);
    RASP_NUM_CHECK(u32RASPEng);
    MS_U16 HWCtrl1;
    HWCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL1);

    if (bPause)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 & (~RASP_STR2MIU_EN)) );
    }
    else
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 | RASP_STR2MIU_RST_WADDR) );
        MsOS_DelayTaskUs(10);
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 & (~RASP_STR2MIU_RST_WADDR)) );
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 | RASP_STR2MIU_EN) );
    }
}

void HAL_NDSRASP_Rec_PID(MS_U32 u32RASPEng , MS_BOOL bSet)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x, bSet = %d\n", u32RASPEng,bSet);
    RASP_NUM_CHECK(u32RASPEng);
    MS_U16 HWCtrl1;
    HWCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL1);
    if (bSet)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 | RASP_REC_PID) );
    }
    else
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (HWCtrl1 & (~RASP_REC_PID)) );
    }
}

void HAL_NDSRASP_SWReset(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x\n", u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);

    MS_U16 HWCtrl0;

    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0);
    HAL_RASP_WriteReg_Word(u32RASPEng,REG_RASP_HW_CTRL0, (HWCtrl0|RASP_SW_RESET) );
    MsOS_DelayTaskUs(10);
    HAL_RASP_WriteReg_Word(u32RASPEng,REG_RASP_HW_CTRL0, HWCtrl0 );
}

void HAL_NDSRASP_SetCorptData(MS_U32 u32RASPEng , MS_U16 u16From ,MS_U16 u16To ,  MS_U8 u8Data)
{
    RASP_NUM_CHECK(u32RASPEng);
#if 1
    MS_U16 CorptData = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_CORPT_PKTSIZE);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_CORRUPTION, (u16From&RASP_FROMTO_MASK) | ((u16To&RASP_FROMTO_MASK)<< RASP_TO_SHIFT) );
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_CORPT_PKTSIZE, ((CorptData & 0xff00) | u8Data) );
#else
    // set corrupt data //
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_CorptFromTo, ((u16To << RASP_TO_SHIFT )&RASP_FROMTO_MASK)|(u16From&RASP_FROMTO_MASK) );
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_CorptData_PktSize2,
                   _HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_CorptData_PktSize2)|u8Data);
#endif
}


void HAL_NDSRASP_SetCorptFlt(MS_U32 u32RASPEng , MS_U16 u16Fltid, MS_BOOL Enable)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x, u16Fltid = %d(0x%x), Enable = %d\n", u32RASPEng,u16Fltid,u16Fltid,Enable);
    RASP_NUM_CHECK(u32RASPEng);

#if 1
    MS_U16 PidFlt = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_PIDFLT_N(u16Fltid));
    if(Enable)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PIDFLT_N(u16Fltid), (PidFlt | RASP_PID_PKT_CORPT_EN));
    }
    else
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PIDFLT_N(u16Fltid), (PidFlt & (~RASP_PID_PKT_CORPT_EN)) );
    }
#else
    MS_U32 _u32PidfltReg = _u32RASP_PidfltBase[u32RASPEng] + (u16Fltid*0x04);

    if(Enable)
{
        _HAL_REG16_W((REG16 *)_u32PidfltReg, SET_FLAG1(_HAL_REG16_R((REG16 *)_u32PidfltReg), RASP_PID_PKT_CORPT_EN));
    }
    else
{
        _HAL_REG16_W((REG16 *)_u32PidfltReg, RESET_FLAG1(_HAL_REG16_R((REG16 *)_u32PidfltReg), RASP_PID_PKT_CORPT_EN));
    }
#endif

}

void HAL_NDSRASP_SetPidflt(MS_U32 u32RASPEng , MS_U16 u16Fltid , MS_U16 u16Pid)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x, u16Fltid = %d(0x%x), u16Pid = %x\n", u32RASPEng,u16Fltid,u16Fltid,u16Pid);
    RASP_NUM_CHECK(u32RASPEng);
    HAL_RASP_WriteReg_Word(u32RASPEng,REG_RASP_PIDFLT_N(u16Fltid),u16Pid);
}

void HAL_NDSRASP_GetPidflt(MS_U32 u32RASPEng , MS_U16 u16Fltid , MS_U16 *u16Pid)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x, u16Fltid = %d(0x%x)\n", u32RASPEng,u16Fltid,u16Fltid);
    RASP_NUM_CHECK(u32RASPEng);
    *u16Pid = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_PIDFLT_N(u16Fltid));
    HALRASP_DBG(RASP_DBGLV_INFO, "u16Pid = 0x%x\n", *u16Pid);
}

void HAL_NDSRASP_SetEcmPidflt(MS_U32 u32RASPEng , MS_U16 u16Fltid , MS_U16 u16Pid)
{

    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x, u16Fltid = %d(0x%x) u16Pid = %x\n", u32RASPEng,u16Fltid,u16Fltid,u16Pid);
    RASP_NUM_CHECK(u32RASPEng);

    MS_U16 LockCtrlReg;
    u16Pid = u16Pid | 0xE000;  //

    if(u16Fltid <= 3)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_PID_N(u16Fltid), u16Pid);
        LockCtrlReg = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_ECM_LOCK_CTRL);
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_LOCK_CTRL, (LockCtrlReg | (0x1<<(4*u16Fltid)) ) );
    }
    else if(u16Fltid == 4)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_PID_4, u16Pid);
        LockCtrlReg = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_ECM45_LOCK_CTRL);
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM45_LOCK_CTRL, (LockCtrlReg | (0x1) ) );
    }
    else if(u16Fltid == 5)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_PID_5, u16Pid);
        LockCtrlReg = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_ECM45_LOCK_CTRL);
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM45_LOCK_CTRL, (LockCtrlReg | (0x1<<4) ) );
    }

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_TID, 0x8180); //odd,even
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_LOCK_CTRL, 0x7777);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM45_LOCK_CTRL, 0x77);

}

void HAL_NDSRASP_ReadEcmPidflt(MS_U32 u32RASPEng , MS_U16 u16Fltid , MS_U16 *pu16Pid)
{

    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x, u16Fltid = %d(0x%x)\n", u32RASPEng,u16Fltid,u16Fltid);
    RASP_NUM_CHECK(u32RASPEng);

    if(u16Fltid <= 3)
        *pu16Pid = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_ECM_PID_N(u16Fltid));
    else if(u16Fltid == 4)
        *pu16Pid = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_ECM_PID_4);
    else if(u16Fltid == 5)
        *pu16Pid = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_ECM_PID_5);

}


void HAL_NDSRASP_SetECM_Init_1(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x\n", u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);
#if 1
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_CA_INT, 0x0007);
    //MsOS_DelayTaskUs(10);
    //HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_INT_STATE, 0xff);
#else
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_Ecm_reg_18,0X0007);
#endif
}

void HAL_NDSRASP_SetECM_Init_2(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x\n", u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);
#if 1
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_TID, 0x8180); //odd,even
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_LOCK_CTRL, 0x7777);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM45_LOCK_CTRL, 0x77);
#else
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_Ecm_reg_16 ,0X8180);
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_Ecm_reg_17 ,0X7777);
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_Ecm_reg_32 ,0X0077);
#endif
}

MS_U16 HAL_NDSRASP_SetECM_GetHW(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x\n", u32RASPEng);
    RASP_NUM_CHECK_RET(u32RASPEng);

    return HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_ECM_INT_STATE);
}

void HAL_NDSRASP_SetECM_ResetHW(MS_U32 u32RASPEng, MS_U16 IntStatus)
{
    RASP_NUM_CHECK(u32RASPEng);

    //HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_INT_STATE, 0xFFFF);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_INT_STATE, IntStatus);
    //MsOS_DelayTaskUs(10);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_ECM_INT_STATE, 0x0000);
}

void HAL_NDSRASP_SetEventMask(MS_U32 u32RASPEng , MS_U16 u16Fltid, MS_U32 u32Mask)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x, u16Fltid = %d(0x%x) u32Mask = %x\n",u32RASPEng,u16Fltid,u16Fltid,u32Mask);
    RASP_NUM_CHECK(u32RASPEng);
#if 1
    HAL_RASP_WriteReg_DWord(u32RASPEng,REG_RASP_EVENT_MASK_N(u16Fltid),u32Mask);
#else
    MS_U32 _u32EventMaskReg = _u32RASP_EventMaskBase[u32RASPEng] + (u16Fltid*0x08);
    _HAL_REG32_W((REG32 *)_u32EventMaskReg, u32Mask);
#endif
}

void HAL_NDSRASP_SetPayloadMask(MS_U32 u32RASPEng , MS_U16 u16Fltid, MS_U32 u32Mask)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x, u16Fltid = %d(0x%x) u32Mask = %x\n", u32RASPEng,u16Fltid,u16Fltid,u32Mask);
    RASP_NUM_CHECK(u32RASPEng);
#if 1
    HAL_RASP_WriteReg_DWord(u32RASPEng,REG_RASP_PAYLOAD_MASK_N(u16Fltid),u32Mask);
#else
    MS_U32 _u32PayloadMaskReg = _u32RASP_PayloadMaskBase[u32RASPEng] + (u16Fltid*0x08);
    _HAL_REG32_W((REG32 *)_u32PayloadMaskReg, u32Mask);
#endif
}


void HAL_NDSRASP_SetPayloadPacketMode(MS_U32 u32RASPEng, MS_BOOL bDisable192)
{
	HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x\n", u32RASPEng);
	RASP_NUM_CHECK(u32RASPEng);

    MS_U16 HW_CTRL3;
    HW_CTRL3 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL3);

	if (bDisable192)
	{
	    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HW_CTRL3 | RASP_PAYLD2_PKT_192_DIS /*| RASP_ECMPKT_192_DIS */) );
	}

	else
	{
		HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HW_CTRL3 & ~(RASP_PAYLD2_PKT_192_DIS /* |RASP_ECMPKT_192_DIS */)) );
	}

}

void HAL_NDSRASP_SetECMPacketMode(MS_U32 u32RASPEng, MS_BOOL bDisable192)
{
	HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x\n", u32RASPEng);
	RASP_NUM_CHECK(u32RASPEng);

    MS_U16 HW_CTRL3;
    HW_CTRL3 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL3);

	if (bDisable192)
	{
	    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HW_CTRL3 | RASP_ECMPKT_192_DIS /*| RASP_ECMPKT_192_DIS */) );
	}

	else
	{
		HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HW_CTRL3 & ~(RASP_ECMPKT_192_DIS /* |RASP_ECMPKT_192_DIS */)) );
	}

}


#if 0

void HAL_NDSRASP_SetStr2Miu_StartAddr(MS_U32 u32RASPEng , MS_U32 u32StartAddr0, MS_U32 u32StartAddr1)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Str2miu_Head,MIU(u32StartAddr0));
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Str2miu_Head2,MIU(u32StartAddr1));
}

void HAL_NDSRASP_SetStr2Miu_MidAddr(MS_U32 u32RASPEng , MS_U32 u32MidAddr0, MS_U32 u32MidAddr1)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Str2miu_Mid,MIU(u32MidAddr0));
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Str2miu_Mid2,MIU(u32MidAddr1));
}

void HAL_NDSRASP_SetStr2Miu_EndAddr(MS_U32 u32RASPEng , MS_U32 u32EndAddr0, MS_U32 u32EndAddr1)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Str2miu_Tail,MIU(u32EndAddr0));
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Str2miu_Tail2,MIU(u32EndAddr1));
}

void HAL_NDSRASP_SetPayload_StartAddr(MS_U32 u32RASPEng , MS_U32 u32StartAddr0, MS_U32 u32StartAddr1)
{
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Payload2miu_Head,MIU(u32StartAddr0));
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Payload2miu_Head2,MIU(u32StartAddr1));
}

void HAL_NDSRASP_SetPayload_MidAddr(MS_U32 u32RASPEng , MS_U32 u32MidAddr0, MS_U32 u32MidAddr1)
{
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Payload2miu_Mid,MIU(u32MidAddr0));
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Payload2miu_Mid2,MIU(u32MidAddr1));
}

void HAL_NDSRASP_SetPayload_EndAddr(MS_U32 u32RASPEng , MS_U32 u32EndAddr0, MS_U32 u32EndAddr1)
{
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Payload2miu_Tail,MIU(u32EndAddr0));
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Payload2miu_Tail2,MIU(u32EndAddr1));
}

void HAL_NDSRASP_SetECM_StartAddr(MS_U32 u32RASPEng , MS_U32 u32StartAddr)
{
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Ecm2miu_Head,MIU(u32StartAddr));
}

void HAL_NDSRASP_SetECM_MidAddr(MS_U32 u32RASPEng , MS_U32 u32MidAddr)
{
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Ecm2miu_Mid,MIU(u32MidAddr));
}

void HAL_NDSRASP_SetECM_EndAddr(MS_U32 u32RASPEng , MS_U32 u32EndAddr)
{
    RASP_NUM_CHECK(u32RASPEng);
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_Ecm2miu_Tail,MIU(u32EndAddr));
}
#endif

MS_U32 HAL_NDSRASP_GetWritePtr(MS_U32 u32RASPEng)
{
    MS_U32 WritePtr;

    RASP_NUM_CHECK_RET(u32RASPEng);
    MS_U16 HWCtrl4;

    HWCtrl4 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL4);
#if 1
    HAL_RASP_WriteReg_Word(u32RASPEng,REG_RASP_HW_CTRL4, (HWCtrl4 | RASP_TS_STR2MI_WP_LD_DIS) );
    WritePtr = (HAL_RASP_ReadReg_DWord(u32RASPEng,REG_RASP_STR2MIU_MID1_L) << 4 );
    HAL_RASP_WriteReg_Word(u32RASPEng,REG_RASP_HW_CTRL4, (HWCtrl4 & (~RASP_TS_STR2MI_WP_LD_DIS)) );
#else
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4,
        SET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4) , RASP_TS_STR2MI_WP_LD_DIS));

    WritePtr = ((_HAL_REG32_R(&_RASPReg[u32RASPEng][0].RASP_Str2miu_Mid)) << 4);

    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4,
        RESET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4) , RASP_TS_STR2MI_WP_LD_DIS));
#endif
    //HALRASP_DBG(RASP_DBGLV_INFO, "RASPEng = %x, WritePtr = %x\n", u32RASPEng,WritePtr);
    return WritePtr;
}

MS_U32 HAL_NDSRASP_GetPayloadWritePtr(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    return (HAL_RASP_ReadReg_DWord(u32RASPEng,REG_RASP_PAYLOAD2MIU_MID1_L) << 4 );
}

MS_U32 HAL_NDSRASP_GetECMWritePtr(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    return (HAL_RASP_ReadReg_DWord(u32RASPEng,REG_RASP_ECM2MIU_MID1_L) << 4 );
}


//change the BOOL bSet to MS_U32, the packet size maybe more than two choice
void HAL_NDSRASP_SetStrPacketMode(MS_U32 u32RASPEng , MS_BOOL bSet)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);

    MS_U16 u16RegCtrl4,u16RegCtrl1;
    u16RegCtrl4 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL4);
    if (bSet)
    {
        u16RegCtrl4 |= RASP_STREAM_192_EN;
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL4, u16RegCtrl4);

        //set the burst length.
        u16RegCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL1);
        u16RegCtrl1 = (u16RegCtrl1 & (~RASP_BURST_LEN_MASK)) | ( 0x02 <<RASP_BURST_LEN_SHIFT) ;
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, u16RegCtrl1);
    }
    else
    {
        u16RegCtrl4 &= (~RASP_STREAM_192_EN);
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL4, u16RegCtrl4);

    }
}


void HAL_NDSRASP_PowerCtrl(MS_U32 u32RASPEng , MS_BOOL bSet)
{

}

MS_U32 HAL_NDSRASP_GetPVRTimeStamp(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    //return _HAL_REG32_R(&_RASPReg[u32RASPEng][0].RASP_StrLPCR1);
    return HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_STR_LPCR1_L);
}

MS_U32 HAL_NDSRASP_GetECMTimeStamp(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    return HAL_RASP_ReadReg_DWord(u32RASPEng,REG_RASP_ECM_LPCR1_L);
}

MS_U32 HAL_NDSRASP_GetPayloadTimeStamp(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    //return _HAL_REG32_R(&_RASPReg[u32RASPEng][0].RASP_PayLPCR1);
    return HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_PAY_LPCR1_L);
}

void HAL_NDSRASP_SetPVRTimeStamp(MS_U32 u32RASPEng , MS_U32 u32Stamp)
{
    RASP_NUM_CHECK(u32RASPEng);

    HAL_RASP_WriteReg_DWord(u32RASPEng, REG_RASP_STR_LPCR1_BUF_L, u32Stamp);
    MS_U16 HWCtrl4;

    HWCtrl4 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL4);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL4, (HWCtrl4 | RASP_STREAM_LPCR_WLD));
    MsOS_DelayTaskUs(10);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL4, (HWCtrl4 & (~RASP_STREAM_LPCR_WLD)));
}

void HAL_NDSRASP_SetECMTimeStamp(MS_U32 u32RASPEng , MS_U32 u32Stamp)
{
    RASP_NUM_CHECK(u32RASPEng);
    HAL_RASP_WriteReg_DWord(u32RASPEng, REG_RASP_ECM_LPCR1_BUF_L, u32Stamp);

    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_EcmLPCR1Buf , u32Stamp);

    MS_U16 HWCtrl3;

    HWCtrl3 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL3);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 | RASP_ECM2MIU_LPCR_WT));
    MsOS_DelayTaskUs(10);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HWCtrl3 & (~RASP_ECM2MIU_LPCR_WT)));
}

void HAL_NDSRASP_SetPayloadTimeStamp(MS_U32 u32RASPEng , MS_U32 u32Stamp)
{
    RASP_NUM_CHECK(u32RASPEng);
#if 1
    MS_U16 HW_CTRL3;
    HAL_RASP_WriteReg_DWord(u32RASPEng, REG_RASP_PAY_LPCR1_BUF_L, u32Stamp);

    HW_CTRL3 = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL3);

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HW_CTRL3 | RASP_PAYLD2MIU_LPCR_WT) );
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL3, (HW_CTRL3 & (~RASP_PAYLD2MIU_LPCR_WT)) );

#else
    _HAL_REG32_W(&_RASPReg[u32RASPEng][0].RASP_PayLPCR1Buf , u32Stamp);

    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3,
        SET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3) , RASP_PAYLD2MIU_LPCR_WT));
    _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3,
        RESET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL3) , RASP_PAYLD2MIU_LPCR_WT));
#endif
}



void HAL_NDSRASP_SetWaterMark(MS_U32 u32RASPEng)
{
}

#if 0	//EEGII: REFINED API ==>HAL_NDSRASP_EVENT_SetThreshold, HAL_NDSRASP_SetTime_Timeout
void HAL_NDSRASP_SetTimerWaterMark(MS_U32 u32RASPEng, MS_U16 u16TimerWaterMask)
{
    MS_U16 HWCtrl2;

    HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL2);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 | (u16TimerWaterMask<<RASP_INT_TIMER_SHIFT)) );
}

void HAL_NDSRASP_SetEventWaterMark(MS_U32 u32RASPEng, MS_U16 u16EventWaterMask)
{
    MS_U16 HWCtrl2;

    HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL2);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 | (u16EventWaterMask<<RASP_INT_EVENT_SHIFT)) );
}
#endif

void HAL_NDSRASP_SetTSIF(MS_U32 u32RASPEng , MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP)
{
    RASP_NUM_CHECK(u32RASPEng);
    MS_U16 HWCtrl0,HWCtrl1;

    HALRASP_DBG(RASP_DBGLV_DEBUG, "RASPEng = 0x%x,  bPara = %d, bExtSync = %d, bDataSWP = %d\n",u32RASPEng, bPara, bExtSync, bDataSWP);

    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0);
    HWCtrl1 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL1);

    if (bPara)
    {
        HWCtrl0 |= RASP_TSIF2_PARA_SEL;
    }
    else
    {
        HWCtrl0 &= (~RASP_TSIF2_PARA_SEL);
        HWCtrl0 |= (RASP_RISING_SYNC_DETECT | RASP_FALLING_VALID_DETECT);

        HWCtrl1 |= 0x0400;
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, HWCtrl1);

        MS_U16 PktSet;
        PktSet = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_PKT_SET);
        PktSet |= 0x0080;
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PKT_SET, PktSet);
    }

    if (bExtSync)
    {
        HWCtrl0 |= RASP_TSIF2_EXT_SYNC;
    }
    else
    {
        HWCtrl0 &= (~RASP_TSIF2_EXT_SYNC);
    }

    if (bDataSWP)
    {
        HWCtrl0 |= RASP_TSIF2_DATA_SWP;
    }
    else
    {
        HWCtrl0 &= (~RASP_TSIF2_DATA_SWP);
    }

    HALRASP_DBG(RASP_DBGLV_DEBUG, "RASPEng = 0x%x,  HWCtrl0 = %x, HWCtrl1 = %x\n",u32RASPEng, HWCtrl0, HWCtrl1);

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, HWCtrl0);
}

void HAL_NDSRASP_GetTSIF(MS_U32 u32RASPEng , MS_BOOL* pbPara, MS_BOOL* pbExtSync, MS_BOOL* pbDataSWP)
{
    RASP_NUM_CHECK(u32RASPEng);
    MS_U16 HWCtrl0;

    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0);

    *pbPara = (HWCtrl0 & RASP_TSIF2_PARA_SEL)? 1:0;

    *pbExtSync = (HWCtrl0 & RASP_TSIF2_EXT_SYNC)? 1:0;

    *pbDataSWP = (HWCtrl0 & RASP_TSIF2_DATA_SWP)? 1:0;
}

#define REG16(addr) (*(volatile unsigned short *) (addr))	//temporary hardcode

//bank 0x101E
#define CHIP_REG(addr)          (*((volatile MS_U16*)(_gChipTp_Addr + ((addr)<<2))))

//0x100A
#define CLKGEN2_REG(addr)       (*((volatile MS_U16*)(_gClkGen2_Addr + ((addr)<<2))))

//0x100B
#define CLKGEN0_REG(addr)       (*((volatile MS_U16*)(_gClkGen_Addr + ((addr)<<2))))



MS_BOOL HAL_NDSRASP_Livein_Config(MS_U32 rasp_eng, RASP_LIVEIN_SRC_e InputSrc)
{
    MS_U16 u16Reg;

    HALRASP_DBG(RASP_DBGLV_DEBUG,"InputSrc = %d\n",InputSrc);
    //pad set to TS0
    u16Reg = CHIP_REG(0x39);
    u16Reg &= ~(0xf<<(rasp_eng*4));
    u16Reg |= (InputSrc<<(rasp_eng*4));
    CHIP_REG(0x39) = u16Reg;


    u16Reg = CLKGEN2_REG(0x02+rasp_eng);
    u16Reg &= (~0x001f);
    u16Reg |= (InputSrc<<2);
    CLKGEN2_REG(0x02+rasp_eng) = u16Reg;


    u16Reg = CLKGEN0_REG(0x29);
    u16Reg &= ~(0x3F00);
    u16Reg |= (InputSrc<<10);
    CLKGEN0_REG(0x26) = u16Reg;

    HALRASP_DBG(RASP_DBGLV_DEBUG, "RASPEng = 0x%x,  CHIP_REG(0x39) = %x, CLKGEN2_REG = %x\n",rasp_eng, CHIP_REG(0x39), CLKGEN2_REG(0x02+rasp_eng));

    return TRUE;
}

void HAL_NDSRASP_SetFileinStart(MS_U32 u32RASPEng , MS_BOOL bEn)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);
    RASP_NUM_CHECK(u32RASPEng);
    //jamietest, FileIn Enable is always 0 on Keres

    MS_U16 HWCtrl0;

    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 & (~RASP_FILEIN_EN)) );

    if (bEn)
    {
        //Set RASP buffer full level to 0b100
        HAL_NDSRASP_SetBufferBlockLevel(u32RASPEng, 0x4);
    }
    else
    {
        HAL_NDSRASP_SetBufferBlockLevel(u32RASPEng, 0x0);
    }
}

MS_BOOL HAL_NDSRASP_GetFileinStart(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    if(HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0) & RASP_FILEIN_EN)
        return TRUE;
    else
        return FALSE;
}

MS_U16  HAL_NDSRASP_INT_GetHW(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    return HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_INT_STATUS);
}

void HAL_NDSRASP_INT_ClrHW(MS_U32 u32RASPEng, MS_U16 u16Mask)
{
    RASP_NUM_CHECK(u32RASPEng);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_INT_CLR, u16Mask);
    MsOS_DelayTaskUs(1);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_INT_CLR, 0x0000);
}

void  HAL_NDSRASP_INT_Enable(MS_U32 u32RASPEng, MS_U16 u16Mask)
{
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_INT_ENABLE, u16Mask );
}

void  HAL_NDSRASP_INT_Disable(MS_U32 u32RASPEng, MS_U16 u16Mask)
{
    MS_U16 IntEn;

    IntEn = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_INT_ENABLE);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_INT_ENABLE, (IntEn & (~u16Mask)) );
}


void HAL_NDSRASP_EVENT_EventWM_Enable(MS_U32 u32RASPEng, MS_BOOL enable)
{
    MS_U16 HWCtrl0,HWCtrl2;

    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0);
    HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL2);

    if (enable)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 | RASP_REC_EVENT_FIFO_EN) );
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 | RASP_INT_EVENT_EN) );
    }
    else
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 & (~RASP_REC_EVENT_FIFO_EN)) );
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 & (~RASP_INT_EVENT_EN)) );
    }
}

MS_BOOL HAL_NDSRASP_EVENT_SetEventThreshold(MS_U32 u32RASPEng, MS_U32 u32threshold)
{
    MS_U16 HWCtrl2;
	HALRASP_DBG(RASP_DBGLV_ERR, "Rasp ID [0x%x]\n", u32RASPEng);

	RASP_NUM_CHECK_RET(u32RASPEng);

	if (u32threshold > 31)
	{
		HALRASP_DBG(RASP_DBGLV_ERR, "THRESHOLD Value exceeds maximum value\n");
		return FALSE;
	}

	HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL2);

	HWCtrl2 &= (~RASP_INT_EVENT_MASK);
	u32threshold <<= RASP_INT_EVENT_SHIFT;
	u32threshold &= RASP_INT_EVENT_MASK;

	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, HWCtrl2 | u32threshold );
	return TRUE;

}

void HAL_NDSRASP_EVENT_TimeWM_Enable(MS_U32 u32RASPEng, MS_BOOL enable)
{

    MS_U16 HWCtrl2;
    //MS_U16 HWCtrl0;

    //HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0);
    HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL2);

    if (enable)
    {
        //HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 | RASP_REC_EVENT_FIFO_EN) );
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 | RASP_INT_TIMER_EN) );
    }
    else
    {
        //HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 & (~RASP_REC_EVENT_FIFO_EN)) );
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, (HWCtrl2 & (~RASP_INT_TIMER_EN)) );
    }

}

MS_BOOL HAL_NDSRASP_EVENT_SetTimerThreshold(MS_U32 u32RASPEng, MS_U16 u16timeout)
{
    MS_U16 HWCtrl2;
	HALRASP_DBG(RASP_DBGLV_ERR, "Rasp ID [0x%x]\n", u32RASPEng);

	RASP_NUM_CHECK_RET(u32RASPEng);

	if (u16timeout > 3)
	{
		HALRASP_DBG(RASP_DBGLV_ERR, "TIMEOUT Value exceeds maximum value\n");
		return FALSE;
	}

	HWCtrl2 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL2);

	HWCtrl2 &= (~RASP_INT_TIMER_MASK);
	u16timeout <<= RASP_INT_TIMER_SHIFT;
	u16timeout &= RASP_INT_TIMER_MASK;

	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL2, HWCtrl2 | u16timeout );
	return TRUE;

}

MS_U16 HAL_NDSRASP_EVENT_GetEventNum(MS_U32 u32RASPEng)
{
    MS_U16 FifoStatus = 0 ;

    RASP_NUM_CHECK_RET(u32RASPEng);
    FifoStatus = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_EVENTLOG_STATUS);

	//Current amount of stored evens in the FIFO
    return (FifoStatus & RASP_EVENT_FIFO_NUM_MASK);
}

MS_BOOL HAL_NDSRASP_EVENT_GetEventDescriptor(MS_U32 u32RASPEng,MS_U32 *DataArray , MS_U32 ArrSize )
{
    MS_U16 FifoStatus = 0;

    FifoStatus = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_EVENTLOG_STATUS);
    HALRASP_DBG(RASP_DBGLV_INFO, "FifoStatus = [%x]\n", (MS_U32)FifoStatus);

    if (FifoStatus & RASP_EVENT_FIFO_NUM_MASK)
    {
		HAL_NDSRASP_EVENT_EFrame_Read(u32RASPEng);	// read event from event FIFO

        if (ArrSize >= 5 )
        {
        	// [NOTE] NDS structure for 20 bytes..
            DataArray[0] = HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_EVENT_DESCR_L);
            DataArray[1] = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_EVENT_PKT_PID);
            DataArray[2] = HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_EVENT_PKT_NUM_L);
            DataArray[3] = HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_EVENT_PKT_TIMER_L);
            DataArray[4] = HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_EVENT_PKT_PCR_L);
            //DataArray[5] = FifoStatus;
            return TRUE;
        }
        else
        {
             HALRASP_DBG(RASP_DBGLV_ERR, "Even descriptor size not enough.\n");
             return FALSE;
        }
    }
    else
    {
    	HALRASP_DBG(RASP_DBGLV_ERR, "No event in Event FIFO.\n");
         return FALSE;
    }

    return FALSE;
}

//Reset Event Counter in Event FIFO
MS_BOOL HAL_NDSRASP_EVENT_RstCounter(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);

    MS_U32 addr;

	addr = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_PKT_SET);
    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PKT_SET, (addr & ~RASP_PKT_RESET_NUMBER));
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PKT_SET, (addr | RASP_PKT_RESET_NUMBER));
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PKT_SET, (addr & ~RASP_PKT_RESET_NUMBER));

    return TRUE;
}

//Reset Event Timer in Event FIFO
MS_BOOL HAL_NDSRASP_EVENT_RstTimer(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);

	MS_U32 addr;

	addr = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_PKT_SET);
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PKT_SET, (addr & ~RASP_PKT_RESET_TIMER));
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PKT_SET, (addr | RASP_PKT_RESET_TIMER));
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_PKT_SET, (addr & ~RASP_PKT_RESET_TIMER));

    return TRUE;
}

//	It reads event FIFO into offset 0x50-0x0x59. Per read, event buffer level decreased.
MS_BOOL HAL_NDSRASP_EVENT_EFrame_Read(MS_U32 u32RASPEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);

	MS_U32 addr;

	addr = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_HW_CTRL1);
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (addr & ~RASP_EVENT_FIFO_READ));
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (addr | RASP_EVENT_FIFO_READ));
	HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL1, (addr & ~RASP_EVENT_FIFO_READ));

    return TRUE;
}

void HAL_NDSRASP_SetDataSwap(MS_U32 u32RASPEng, MS_BOOL bEn)
{
    MS_U16 HWCtrl0;

    HWCtrl0 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL0);

    if(bEn)
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 | RASP_TSIF2_DATA_SWP) );
    }
    else
    {
        HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL0, (HWCtrl0 & (~RASP_TSIF2_DATA_SWP)) );
    }
}


//TODO:what this function for
MS_BOOL HAL_NDSRASP_SetStream_47_48(MS_U32 u32RASPEng, MS_U32 StreamSel)
{
    if(0x47 == StreamSel)
    {
        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4,
            RESET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4), RASP_AUTO_STREAM_47_48)); // '0' for 47, '1' for 48
        return TRUE;
    }
    else if(0x48 == StreamSel)
    {
        _HAL_REG16_W(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4,
            SET_FLAG1(_HAL_REG16_R(&_RASPReg[u32RASPEng][0].RASP_HW_CTRL4), RASP_AUTO_STREAM_47_48));// '0' for 47, '1' for 48
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U32 HAL_NDSRASP_GetPktTimer(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    return HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_PKT_TIMER_L);
}

MS_U32 HAL_NDSRASP_GetPktNum(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    return HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_PKT_NUM_L);
}

MS_U32 HAL_NDSRASP_GetEcmPktNum(MS_U32 u32RASPEng)
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    return HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_ECM_PACKET_NUM_L);
}

void HAL_NDSRASP_SetBufferBlockLevel(MS_U32 u32RASPEng, MS_U32 bufBlockLv)
{
    MS_U16 HWCtrl4;

    HWCtrl4 = HAL_RASP_ReadReg_Word(u32RASPEng,REG_RASP_HW_CTRL4);

    HWCtrl4 &= (~RASP_TS_FF_FULL_SEL_MASK);
    HWCtrl4 |= (bufBlockLv<<RASP_TS_FF_FULL_SEL_SHFT);

    HAL_RASP_WriteReg_Word(u32RASPEng, REG_RASP_HW_CTRL4, HWCtrl4 );
}

MS_U32 HAL_NDSRASP_GetEVENT_Status(MS_U32 u32RASPEng, MS_U16 *pEventLog , MS_U32 *pEventDescr) // [NOTE] NDS structure for 20 bytes..
{
    RASP_NUM_CHECK_RET(u32RASPEng);
    *pEventLog = HAL_RASP_ReadReg_Word(u32RASPEng, REG_RASP_EVENTLOG_STATUS);
    *pEventDescr = HAL_RASP_ReadReg_DWord(u32RASPEng, REG_RASP_EVENT_DESCR_L);
    return TRUE;
}

MS_BOOL HAL_NDSRASP_FileIn_Init(MS_U32 u32RASPEng, MS_U8 u8PacketLength)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);
    MS_U16 i;
    MS_U32 addr;


    //for(i=0 ; i<2; i++)
    i = u32RASPEng;
    {
        // reset
        HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_RESET, REG_RFILEIN_RESET_ALL);
        MsOS_DelayTaskUs(10);
        HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_RESET, 0x0000);


 		//188 mode, Filein enable
		addr = HAL_RFILEIN_ReadReg_Word(i, REG_RFILEIN_CTRL_0);
		addr &= ~REG_RFILEIN_PKT_CHK_SIZE;

		if (188 == u8PacketLength)
		{
			addr |= (RASP_RFILEIN_PORT_SEL_FILE|RASP_RFILEIN_ALIGN_EN|RASP_RFILEIN_INPUT_EN /*| REG_RFILEIN_PKT_CHK_SIZE_188*/);	//test, moving it to different api: NDS_RASP_SetFileIn_PktSize
		}

		else if (192 == u8PacketLength)
		{
			addr |= (RASP_RFILEIN_PORT_SEL_FILE|RASP_RFILEIN_ALIGN_EN|RASP_RFILEIN_INPUT_EN /*| REG_RFILEIN_PKT_CHK_SIZE_192*/);
		}

		HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_CTRL_0, addr);

		HALRASP_DBG(RASP_DBGLV_INFO, "offset CTRL_0 [0x%x]\n", (MS_U32)HAL_RFILEIN_ReadReg_Word(i, REG_RFILEIN_CTRL_0) );

        //auto flush
        HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_CTRL_1, HAL_RFILEIN_ReadReg_Word(i,REG_RFILEIN_CTRL_1) | REG_RFILEIN_FLUSH_AUTO);
    }

    // MIU Xbar
    //addr = 0xbf000000 + (0x161300*2 + 0x00*4);
    //(*((volatile MS_U16*)(addr))) = 0x0F;


    return TRUE;
}


MS_BOOL HAL_NDSRASP_LiveIn_Init(MS_U32 u32RASPEng, MS_U8 u8PacketLength)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x \n",u32RASPEng);
    MS_U16 i;
    MS_U32 addr;


    for(i=0 ; i<2; i++)
    {
        // reset
        HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_RESET, REG_RFILEIN_RESET_ALL);
        MsOS_DelayTaskUs(10);
        HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_RESET, 0x0000);


 		//188 mode, Filein disable
		addr = HAL_RFILEIN_ReadReg_Word(i, REG_RFILEIN_CTRL_0);
		addr &= ~(RASP_RFILEIN_PORT_SEL_FILE|RASP_RFILEIN_ALIGN_EN|RASP_RFILEIN_INPUT_EN /* | REG_RFILEIN_PKT_CHK_SIZE*/);	//disable filein

		#if 0
		if (188==u8PacketLength){
			addr |= REG_RFILEIN_PKT_CHK_SIZE_188;		//188
		}
		else if (192==u8PacketLength){
			addr |= REG_RFILEIN_PKT_CHK_SIZE_192;
		}
		#endif

		HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_CTRL_0, addr);

        //auto flush
        HAL_RFILEIN_WriteReg_Word(i, REG_RFILEIN_CTRL_1, HAL_RFILEIN_ReadReg_Word(i,REG_RFILEIN_CTRL_1) | REG_RFILEIN_FLUSH_AUTO);

    }

    // MIU Xbar
    //addr = 0xbf000000 + (0x161300*2 + 0x00*4);
    //(*((volatile MS_U16*)(addr))) = 0x0F;

    return TRUE;
}

MS_U32 HAL_RASP_SetFileIn_Config(MS_U32 RaspEng, MS_U32 StartAddr, MS_U32 FileInSize)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, StartAddr = %x, FileInSize = %x\n", RaspEng, StartAddr, FileInSize);
    RASP_NUM_CHECK_RET(RaspEng);


    HAL_NDSRASP_FileIn_Init(RaspEng, 188);

    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_ADDR_L, (MS_U16) (StartAddr&0xFFFF));
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_ADDR_H, (MS_U16) ((StartAddr>>16)&0xFFFF));
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_SIZE_L, (MS_U16) (FileInSize&0xFFFF));
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_SIZE_H, (MS_U16) ((FileInSize>>16)&0xFFFF));
    return TRUE;
}

MS_U32 HAL_RASP_FileIn_Start(MS_U32 RaspEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "%s: RaspEng = 0x%x\n", __FUNCTION__, RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_START, RASP_FILEIN_START);
    return TRUE;
}

MS_U32 HAL_RASP_SetFileIn_Timer(MS_U32 RaspEng, MS_U16 u16Timer)
{

    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, u16Timer = %x\n", RaspEng,u16Timer);
    RASP_NUM_CHECK_RET(RaspEng);

	/*FileIn speed. By Byte delay*/
    MS_U16 u16Reg;
    u16Reg = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CTRL_0);
    u16Reg |= 0x0400;
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_0, u16Reg);

    //file timer
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_TIMER, u16Timer);

    return TRUE;
}


//TODO: this API not necessary, flowset and filein_packetmode can do it
MS_U32 HAL_RASP_SetFileIn_PktSize(MS_U32 RaspEng, MS_U16 PktSize)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, PktSize = %x\n", RaspEng,PktSize);
    RASP_NUM_CHECK_RET(RaspEng);
	MS_U16 u16Reg;

    u16Reg = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CTRL_0);

    if(PktSize == 188)
    {
        // 188-Mode
        HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_0, u16Reg | REG_RFILEIN_PKT_CHK_SIZE_188 ); //hard code first
    }
    else if(PktSize == 192)
    {
        // 192-Mode
        HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_0, u16Reg | REG_RFILEIN_PKT_CHK_SIZE_192); //hard code first
    }

    return TRUE;
}

MS_BOOL HAL_RASP_IsFileIn_Done(MS_U32 RaspEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);

    MS_U16 u16FileInStatus = 0;
    MS_U16 u16CMDQStatus = 0;

    // trigger
    u16FileInStatus = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_START);
    u16FileInStatus &= 0x1;

    u16CMDQStatus = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CMDQ_STATUS);
	HALRASP_DBG(RASP_DBGLV_INFO, "FileInStat[%x], CMDQStat[%x]\n", (MS_U32)u16FileInStatus, (MS_U32)u16CMDQStatus);

    if( (u16FileInStatus == 0) && (u16CMDQStatus & RFILEIN_CMD_FIFO_EMPTY) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

MS_BOOL HAL_RASP_FileIn_IsCMDQ_Full (MS_U32 RaspEng)

{
	HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
	MS_U16 u16CMDQStatus = 0;

	u16CMDQStatus = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CMDQ_STATUS);
    if( u16CMDQStatus & RFILEIN_CMD_FIFO_FULL )
	{
        return TRUE;
    }
    else
	{
        return FALSE;
    }

}

MS_BOOL HAL_RASP_FileIn_IsCMDQ_Empty (MS_U32 RaspEng)

{
	HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
	MS_U16 u16CMDQStatus = 0;

	u16CMDQStatus = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CMDQ_STATUS);
    if( u16CMDQStatus & RFILEIN_CMD_FIFO_EMPTY )
	{
        return TRUE;
    }
    else
	{
        return FALSE;
    }
}

MS_BOOL HAL_RASP_FileIn_GetCmdQueueLevel (MS_U32 RaspEng, MS_U8 *CmdQLvl)

{
	HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
	MS_U16 u16CMDQStatus = 0;

	u16CMDQStatus = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CMDQ_STATUS);
	HALRASP_DBG(RASP_DBGLV_INFO, "CMDQStatus = %x\n", (MS_U32)u16CMDQStatus);

    u16CMDQStatus &= RFILEIN_CMD_WR_LEVEL_MASK;
    u16CMDQStatus >>= 8;

	*CmdQLvl = (MS_U8) u16CMDQStatus;
	return TRUE;
}

MS_BOOL HAL_RASP_FileIn_GetEmptyNum (MS_U32 RaspEng, MS_U8 *CmdQLvl)

{
	HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
	MS_U16 u16CMDQStatus = 0;

	u16CMDQStatus = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CMDQ_STATUS);
    u16CMDQStatus &= RFILEIN_CMD_WR_COUNT_MASK;

	*CmdQLvl = (MS_U8) (16 - u16CMDQStatus);	//we have 16slots in CMDQ
	return TRUE;
}



MS_U32 HAL_RASP_FileIn_Flush(MS_U32 RaspEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);

    MS_U16 u16Reg;

    // flush data
    u16Reg = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CTRL_1);

    u16Reg &= ~REG_RFILEIN_FLUSH;
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, u16Reg);

	u16Reg |= REG_RFILEIN_FLUSH;
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, u16Reg);

	u16Reg &= ~REG_RFILEIN_FLUSH;
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, u16Reg);

    return TRUE;
}

MS_U32 HAL_RASP_FileIn_BlockTimeStamp(MS_U32 RaspEng, MS_BOOL enbl)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);

    MS_U16 u16Reg, u16Reg2, u16Reg3, u16Reg4;

    u16Reg = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CTRL_0);

	if(enbl)
	{
	    u16Reg |= RASP_RFILEIN_PKT192_EN;	//enable 192, but timestamp blocked
		u16Reg &= ~RASP_RFILEIN_PKT192_BLK_DIS;	//enable timestamp block scheme

		// patch, 20150519
		u16Reg2 = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CTRL_1);
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16Reg2|REG_RFILEIN_LPCR2_LOAD|REG_RFILEIN_TIMER_192FIX) );	//4 please remain it as set, HW patch

		//set RST_PKT_TSTAMP to reset packet timestamp to zero, and check
		u16Reg2 = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_CTRL_1);
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16Reg2 | REG_RFILEIN_RST_PKT_TSTAMP) );

		u16Reg3 = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_STREAM_TIMESTAMP_L);
		u16Reg4 = HAL_RFILEIN_ReadReg_Word(RaspEng, REG_RFILEIN_STREAM_TIMESTAMP_H);
		if ( (u16Reg3 == 0x0) && (u16Reg4 == 0x0) )
		{
			printf("==========bank, 0x2f[0], 0x30[0]\n");
			//if equal to zero, reset RST_PKT_TSTAMP register
			HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16Reg2 & ~REG_RFILEIN_RST_PKT_TSTAMP) );
		}
		else
		{
			printf("==========bank, 0x2f[%x], 0x30[%x], FAIL\n", (MS_U32)u16Reg3, (MS_U32)u16Reg4);
		}
		// patch ends, 20150519

	}
	else
	{
		u16Reg &= ~RASP_RFILEIN_PKT192_EN;	//disable 192
		u16Reg |= RASP_RFILEIN_PKT192_BLK_DIS;	//disable timestamp block scheme
	}
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_0, u16Reg);

    return TRUE;
}

MS_U32 HAL_RASP_FileIn_SetPlaybackTimeStamp(MS_U32 RaspEng , MS_U32 u32Stamp)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);
	MS_U16 HWCtrl4;

    HAL_RFILEIN_WriteReg_DWord(RaspEng, REG_RFILEIN_LPCR2_BUF_L, u32Stamp);
	HWCtrl4 = HAL_RFILEIN_ReadReg_Word(RaspEng,REG_RFILEIN_CTRL_1);

	//send initial value to lpcr2 timer
    HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (HWCtrl4 | REG_RFILEIN_LPCR2_WLD) );
    MsOS_DelayTaskUs(10);

	//Read lpcr2 initial timer before start
	HAL_RASP_FileIn_GetPlaybackTimeStamp(RaspEng);

	//start lpcr2 timer
	HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (HWCtrl4 & ~REG_RFILEIN_LPCR2_WLD) );
	MsOS_DelayTaskUs(10);

	return TRUE;
}

MS_U32 HAL_RASP_FileIn_GetPlaybackTimeStamp(MS_U32 RaspEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);

	MS_U16 u16value;
	MS_U32 u32TimeStamp;

	u16value = HAL_RFILEIN_ReadReg_Word(RaspEng,REG_RFILEIN_CTRL_1);

	//update 90k ctr for read
	HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16value | REG_RFILEIN_LPCR2_LOAD));

	//latch 90k ctr
	HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16value & (~REG_RFILEIN_LPCR2_LOAD) ));

	//get counter
    u32TimeStamp = HAL_RFILEIN_ReadReg_DWord(RaspEng, REG_RFILEIN_LPCR2_RD_L);

	HALRASP_DBG(RASP_DBGLV_INFO, "u32TimeStamp = 0x%x\n", u32TimeStamp);
	return u32TimeStamp;

}


MS_U32 HAL_RASP_FileIn_Timer(MS_U32 RaspEng, MS_BOOL bEnFileinTimer, MS_U16 u16TimerValue)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, TimerValue = 0x%x\n", RaspEng, (MS_U32)u16TimerValue);
    RASP_NUM_CHECK_RET(RaspEng);

	MS_U16 u16value, u16value2;

	u16value = HAL_RFILEIN_ReadReg_Word(RaspEng,REG_RFILEIN_CTRL_0);

	if (TRUE == bEnFileinTimer)
	{
		//enable delay timer
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_0, (u16value | RASP_RFILEIN_TIMER_EN));

		//set timer value
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_TIMER, u16TimerValue );

		//patch, since Kris
		u16value2 = HAL_RFILEIN_ReadReg_Word(RaspEng,REG_RFILEIN_CTRL_1);
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, u16value2 | REG_RFILEIN_TIMER_192FIX );
	}
	else
	{
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_0, (u16value & ~RASP_RFILEIN_TIMER_EN));
	}

    return TRUE;

}

MS_U32 HAL_RASP_FileIn_Reset_TStamp(MS_U32 RaspEng, MS_BOOL bResetPktTimeStamp)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x\n", RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);

	MS_U16 u16value;

	u16value = HAL_RFILEIN_ReadReg_Word(RaspEng,REG_RFILEIN_CTRL_1);

	if (TRUE == bResetPktTimeStamp)
	{
		//@ IMPORTANT: Initialize timestamp latch buffer to zero
		//@ IMPORTANT: In normal scenario, timestamp latch buffer is updated by Filein packet timestamp

		//HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16value & ~REG_RFILEIN_RST_PKT_TSTAMP));
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16value | REG_RFILEIN_RST_PKT_TSTAMP));
		//HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16value & ~REG_RFILEIN_RST_PKT_TSTAMP));
	}
	else
	{
		HAL_RFILEIN_WriteReg_Word(RaspEng, REG_RFILEIN_CTRL_1, (u16value & ~REG_RFILEIN_RST_PKT_TSTAMP));
	}

    return TRUE;

}


/**************************************************************************************************************************/


MS_U32 HAL_RASP_SetPVRDstBuf(MS_U32 RaspEng, MS_U32 u32StartAddr0, MS_U32 u32EndAddr0, MS_U32 u32StartAddr1, MS_U32 u32EndAddr1)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, u32StartAddr0 = %x, u32EndAddr0 = %x\n", RaspEng,u32StartAddr0,u32EndAddr0);
    RASP_NUM_CHECK_RET(RaspEng);

    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_STR2MIU_HEAD1_L, MIU(u32StartAddr0));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_STR2MIU_MID1_L, MIU(u32EndAddr0));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_STR2MIU_TAIL1_L, MIU(u32EndAddr0));

    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_STR2MIU_HEAD2_L, MIU(u32StartAddr1));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_STR2MIU_MID2_L, MIU(u32EndAddr1));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_STR2MIU_TAIL2_L, MIU(u32EndAddr1));

    MsOS_DelayTaskUs(10);
    HAL_NDSRASP_Str2MIU_Reset(RaspEng);
    return TRUE;
}

MS_U32 HAL_RASP_SetPayloadDstBuf(MS_U32 RaspEng, MS_U32 u32StartAddr0, MS_U32 u32MidAddr0, MS_U32 u32EndAddr0, MS_U32 u32StartAddr1, MS_U32 u32MidAddr1, MS_U32 u32EndAddr1)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, u32StartAddr0 = %x, u32EndAddr0 = %x\n",RaspEng,u32StartAddr0,u32EndAddr0);
    RASP_NUM_CHECK_RET(RaspEng);

    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_PAYLOAD2MIU_HEAD1_L, MIU(u32StartAddr0));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_PAYLOAD2MIU_MID1_L, MIU(u32MidAddr0));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_PAYLOAD2MIU_TAIL1_L, MIU(u32EndAddr0));

    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_PAYLOAD2MIU_HEAD2_L, MIU(u32StartAddr1));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_PAYLOAD2MIU_MID2_L, MIU(u32MidAddr1));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_PAYLOAD2MIU_TAIL2_L, MIU(u32EndAddr1));

	MsOS_DelayTaskUs(10);
	HAL_NDSRASP_PAYLD2MIU_Reset(RaspEng);
    return TRUE;
}

MS_U32 HAL_RASP_SetECMDstBuf(MS_U32 RaspEng, MS_U32 u32StartAddr0, MS_U32 u32MidAddr0, MS_U32 u32EndAddr0)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, u32StartAddr0 = %x, u32EndAddr0 = %x\n",RaspEng,u32StartAddr0,u32EndAddr0);
    RASP_NUM_CHECK_RET(RaspEng);

    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_ECM2MIU_HEAD1_L, MIU(u32StartAddr0));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_ECM2MIU_MID1_L, MIU(u32MidAddr0));
    HAL_RASP_WriteReg_DWord(RaspEng, REG_RASP_ECM2MIU_TAIL1_L, MIU(u32EndAddr0));

	MsOS_DelayTaskUs(10);
    //HAL_NDSRASP_ECM2MIU_Reset(RaspEng);
    return TRUE;
}


MS_U32 HAL_RASP_RecOpenPid(MS_U32 RaspEng, MS_U16 Pid, MS_U8 RaspFltId)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "RaspEng = 0x%x, Pid = %x, RaspFltId = %x\n", RaspEng,Pid,RaspFltId);
    RASP_NUM_CHECK_RET(RaspEng);

    MS_U16 u16Reg;

    // enable record....
    u16Reg = HAL_RASP_ReadReg_Word(RaspEng, REG_RASP_HW_CTRL1);

    if(u16Reg & RASP_REC_PID)
    {
        //already enable...skip...
    }
    else
    {
        u16Reg |= RASP_REC_PID;
        HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_HW_CTRL1, u16Reg);
    }

    HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_PIDFLT_N(RaspFltId), Pid);

    return TRUE;
}

MS_U32 HAL_RASP_DisableRec(MS_U32 RaspEng)
{
    MS_U16 u16Reg;

    // dsiable record....
    u16Reg = HAL_RASP_ReadReg_Word(RaspEng, REG_RASP_HW_CTRL1);
    u16Reg &= (~RASP_REC_PID);
    HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_HW_CTRL1, u16Reg);

    return TRUE;
}

//ask Stephen what is the real trigger start register...
MS_BOOL HAL_NDSRASP_SetFileinConfig(MS_U32 RaspEng, MS_BOOL bFileIn, MS_U16 BlockLevel)
{
    HALRASP_DBG(RASP_DBGLV_INFO,"RaspEng = %x, bFileIn = %d, BlockLevel = %x\n",RaspEng, bFileIn, BlockLevel);
    RASP_NUM_CHECK_RET(RaspEng);

    MS_U16 u16RegCtrl0, u16RegBlockLevel;

    // read control 0....
    u16RegCtrl0 = HAL_RASP_ReadReg_Word(RaspEng, REG_RASP_HW_CTRL0);
	u16RegBlockLevel = HAL_RASP_ReadReg_Word(RaspEng, REG_RASP_HW_CTRL4);

    if(bFileIn)
    {
    	u16RegCtrl0 &= (~RASP_FILEIN_EN);

        u16RegBlockLevel = (u16RegBlockLevel & ~RASP_TS_FF_FULL_SEL_MASK) | (BlockLevel<<RASP_TS_FF_FULL_SEL_SHFT);
        HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_HW_CTRL4, u16RegBlockLevel);
    }
    else
    {
        //u16RegCtrl0 |= RASP_FILEIN_EN;
		u16RegCtrl0 &= (~RASP_FILEIN_EN);	// always 0 on keres

        u16RegBlockLevel = (u16RegBlockLevel & ~RASP_TS_FF_FULL_SEL_MASK);
        HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_HW_CTRL4, u16RegBlockLevel);
    }

    HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_HW_CTRL0, u16RegCtrl0);



    return TRUE;
}

MS_U32 HAL_RASP_Close(MS_U32 RaspEng)
{
    HALRASP_DBG(RASP_DBGLV_INFO, "u32RASPEng = 0x%x\n", RaspEng);
    RASP_NUM_CHECK_RET(RaspEng);

    MS_U16 u16RegCtrl0;

    // read control 0....
    u16RegCtrl0 = HAL_RASP_ReadReg_Word(RaspEng, REG_RASP_HW_CTRL0);
    u16RegCtrl0 |= RASP_SW_RESET;
    HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_HW_CTRL0, u16RegCtrl0);

    MsOS_DelayTaskUs(10);
    u16RegCtrl0 &= (~RASP_SW_RESET);

    HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_HW_CTRL0, u16RegCtrl0);

    if(RaspEng == 1)
    {
        //K2 RASP1 MIU setting
        _HAL_REG16_W((REG16*)_REG_RASP1_MIU, RESET_FLAG1(_HAL_REG16_R((REG16*)_REG_RASP1_MIU), _RASP1_MIU_EN));
    }

    return TRUE;
}

void HAL_NDSRASP_SetDbgLevel(MS_U32 level)
{
    _g32RASPHalDbgLv = level;
}


MS_U32 HAL_NDSRASP_SetExtendConfig(MS_U32 RaspEng, MS_U16 type, MS_U16 extendbytes, MS_U16 syncbyte)
{
    MS_U16 Reg;

    Reg = 0x204;
    Reg = (Reg&(~0x1f0)) | ((extendbytes&0x1f)<<4);
    printf("RASP HW Config 0 is %x\n",Reg);
    HAL_RFILEIN_WriteReg_DWord(RaspEng, 5, 0x2C4);


    //Reg = ((extendbytes + 188)<<0x8) | (syncbyte&0xff);
    printf("sync byte reg is %x\n",syncbyte);
    HAL_RFILEIN_WriteReg_DWord(RaspEng, 0xb, (syncbyte&0xff));

#if 0
    Reg = ((extendbytes + 188) + (syncbyte<<0x8));
    HAL_RASP_WriteReg_Word(RaspEng, REG_RASP_CONFIG_TSIF2, Reg);
#endif
    return TRUE;
}

#ifdef MSOS_TYPE_LINUX_KERNEL
EXPORT_SYMBOL(HAL_NDSRASP_SetPayload_MidAddr);
EXPORT_SYMBOL(HAL_NDSRASP_INT_Enable);
EXPORT_SYMBOL(HAL_NDSRASP_SetECM_StartAddr);
EXPORT_SYMBOL(HAL_NDSRASP_SetPayload_EndAddr);
EXPORT_SYMBOL(HAL_NDSRASP_EVENT_TimeWM_Enable);
EXPORT_SYMBOL(HAL_NDSRASP_GetPktTimer);
EXPORT_SYMBOL(HAL_NDSRASP_SetPayloadMask);
EXPORT_SYMBOL(HAL_NDSRASP_SetTimerWaterMark);
EXPORT_SYMBOL(HAL_NDSRASP_GetPidflt);
EXPORT_SYMBOL(HAL_NDSRASP_SetECM_MidAddr);
EXPORT_SYMBOL(HAL_NDSRASP_Payload_Enable);
EXPORT_SYMBOL(HAL_NDSRASP_SetEventMask);
EXPORT_SYMBOL(HAL_NDSRASP_SetEventWaterMark);
EXPORT_SYMBOL(HAL_NDSRASP_SetPayload_StartAddr);
EXPORT_SYMBOL(HAL_NDSRASP_SetCorptFlt);
EXPORT_SYMBOL(HAL_NDSRASP_EVENT_EventWM_Enable);
EXPORT_SYMBOL(HAL_NDSRASP_SetCorptData);
EXPORT_SYMBOL(HAL_NDSRASP_SWReset);
EXPORT_SYMBOL(HAL_NDSRASP_EVENT_GetEventNum);
EXPORT_SYMBOL(HAL_NDSRASP_EVENT_GetEventDescriptor);
EXPORT_SYMBOL(HAL_NDSRASP_ECM_Enable);
EXPORT_SYMBOL(HAL_NDSRASP_INT_ClrHW);
EXPORT_SYMBOL(HAL_NDSRASP_GetPktNum);
EXPORT_SYMBOL(HAL_NDSRASP_GetEcmPktNum);
EXPORT_SYMBOL(HAL_NDSRASP_SetECM_EndAddr);
EXPORT_SYMBOL(HAL_NDSRASP_INT_GetHW);
EXPORT_SYMBOL(HAL_NDSRASP_SetEcmPidflt);
#endif


