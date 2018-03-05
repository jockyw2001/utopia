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
#define _HAL_EMMFLT_C

////////////////////////////////////////////////////////////////////////////////
/// @file halEMMflt.c
/// @author MStar Semiconductor Inc.
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "drvEMMFlt.h"
#include "halEMMflt.h"
#include "regEMMflt.h"
#include "../../include/drvNSK2Type.h"
#include "MsIRQ.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////
//0x113c for emm filter..

//0x100B
#define CLKGEN0_REG(addr)       (*((volatile MS_U16*)(_gCLKGEN0_Addr + ((addr)<<2))))

//0x100A
#define CLKGEN2_REG(addr)       (*((volatile MS_U16*)(_gCLKGEN2_Addr + ((addr)<<2))))

//bank 0x101E
#define CHIP_REG(addr)          (*((volatile MS_U16*)(_gCHIPTOP_Addr + ((addr)<<2))))

//bank 0x1137
#define PVR0_REG(addr)          (*((volatile MS_U16*)(_gPVR0_Addr + ((addr)<<2))))

//bank 0x19xx
#define OTP_REG(addr)           (*((volatile MS_U32*)(_gOTP_Addr + addr )))

//bank 0x1713
#define OTP_CTRL_REG(addr)      (*((volatile MS_U32*)(_gOTP_CTRL_Addr + (addr<<2) )))

#define EMM_IRQ_INT     E_INT_FIQ_TSP_SPARE_CA2PM_8

static MS_U32 _g32EMMHalDbgLv = EMM_DBGLV_INFO;

#define HALEMM_DBG(lv, x, args...)   if (lv <= _g32EMMHalDbgLv ) \
                                        {printf(x, ##args);}


#define ConnectionCheck(x) { if(x>EMMENG_NUMBER)  \
                            { printf("only one emm allow\n"); \
                            return FALSE;} }


////////////////////////////////////////////////////////////////////////////////
// Local variable
////////////////////////////////////////////////////////////////////////////////

//static MS_U32 _gEMMflt_BankAddr = 0;
static MS_U32 _gEMMflt_Addr[EMMENG_NUMBER];
static MS_U32 _gBasicAddr = 0;
static MS_U32 _gOTP_Addr = 0;
static MS_U32 _gOTP_CTRL_Addr = 0;
static MS_U32 _gPVR0_Addr = 0;
static MS_U32 _gCLKGEN0_Addr = 0;
static MS_U32 _gCLKGEN2_Addr = 0;
static MS_U32 _gCHIPTOP_Addr = 0;

////////////////////////////////////////////////////////////////////////////////
// Global variable
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function Declaration
////////////////////////////////////////////////////////////////////////////////
void PrintSetting(void);

////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////

static void HAL_EMMFLT_WriteReg_Word(MS_U32 connection, MS_U32 u32RegAddr, MS_U16 u16Data)
{
    MS_U32 u32reg;
    u32reg = (u32RegAddr*4) + _gEMMflt_Addr[connection];
    (*(volatile MS_U16*)(u32reg)) = u16Data;
}

static MS_U16 HAL_EMMFLT_ReadReg_Word(MS_U32 connection, MS_U32 u32RegAddr)
{
    MS_U32 u32reg;
    MS_U16 u16Data;
    u32reg = (u32RegAddr*4) + _gEMMflt_Addr[connection];
    u16Data = (*(volatile MS_U16*)(u32reg));

    return u16Data;
}

static void HAL_EMMFLT_WriteReg_Dword(MS_U32 connection, MS_U32 u32RegAddr, MS_U32 u32Data)
{
    MS_U32 u32reg;

    u32reg = (u32RegAddr*4) + _gEMMflt_Addr[connection];
    (*(volatile MS_U16*)(u32reg)) = (MS_U16)(u32Data&0xffff);

    u32reg += 4;
    (*(volatile MS_U16*)(u32reg)) = (MS_U16)((u32Data>>16)&0xffff);
}

static MS_U32 HAL_EMMFLT_ReadReg_Dword(MS_U32 connection, MS_U32 u32RegAddr)
{
    MS_U32 u32reg,u32Data;
    MS_U16 u16Data1,u16Data2;

    u32reg = (u32RegAddr*4) + _gEMMflt_Addr[connection];
    u16Data1 = (*(volatile MS_U16*)(u32reg));

    u32reg += 4;
    u16Data2 = (*(volatile MS_U16*)(u32reg));
    u32Data = (u16Data1) + ((MS_U32)u16Data2<<16);

    return u32Data;
}

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////

void HAL_EMMFLT_SetBank(MS_U32 u32Base)
{
    MS_U16 u16I,u16J;
    MS_U16 u16Data;

    HALEMM_DBG(EMM_DBGLV_INFO, "%s: u32Base = 0x%x\n", __FUNCTION__, u32Base);

    _gBasicAddr = u32Base;

    _gEMMflt_Addr[0] = _gBasicAddr + REG_EMMFLT_BASE1;
    _gEMMflt_Addr[1] = _gBasicAddr + REG_EMMFLT_BASE2;

    _gOTP_Addr  = _gBasicAddr + REG_OTP_BASE;
    _gOTP_CTRL_Addr  = _gBasicAddr + REG_OTP_CTRL_BASE;
    _gPVR0_Addr  = _gBasicAddr + REG_PVR0_BASE;
    _gCHIPTOP_Addr= _gBasicAddr + REG_CHIPTOP_BASE;
    _gCLKGEN0_Addr= _gBasicAddr + REG_CLKGEN0_BASE;
    _gCLKGEN2_Addr= _gBasicAddr + REG_CLKGEN2_BASE;


    HALEMM_DBG(EMM_DBGLV_INFO,"_gBasicAddr = %x, _gEMMflt_Addr[0] = %x, _gEMMflt_Addr[1] = %x\n",_gBasicAddr,_gEMMflt_Addr[0],_gEMMflt_Addr[1]);
    HALEMM_DBG(EMM_DBGLV_INFO,"_gCHIPTOP_Addr = %x, _gCLKGEN0_Addr = %x\n",_gCHIPTOP_Addr,_gCLKGEN0_Addr);
    HALEMM_DBG(EMM_DBGLV_INFO,"int ctrl addr = %x\n", (_gBasicAddr+ (0x101900<<1) ));

    for( u16J=0; u16J<EMMENG_NUMBER; u16J++ )
    {
        for(u16I=0; u16I<=REG_EMM_TSIF_LOCKED_CNT_STATUS; u16I++)
        {
            u16Data = HAL_EMMFLT_ReadReg_Word(u16J,u16I);
            HALEMM_DBG(EMM_DBGLV_ARRAY, " %x = %x \n",u16I,u16Data);
        }
    }

}

#define FileInTest

static void HAL_EMMFLT_FileInSet(void)
{
    //MS_U32 u32Addr;
    MS_U16 u16Data,i;

    HALEMM_DBG(EMM_DBGLV_INFO, "CHIP_REG(0x2) = %x\n",CHIP_REG(0x2));
    HALEMM_DBG(EMM_DBGLV_INFO, "CHIP_REG(0x3A) = %x\n",CHIP_REG(0x3A));

#if 0
    HALEMM_DBG(0, "CHIP_REG(0x2A) = %x\n",CHIP_REG(0x2A));
    HALEMM_DBG(0, "CHIP_REG(0x28) = %x\n",CHIP_REG(0x28));
    HALEMM_DBG(0, "CHIP_REG(0x29) = %x\n",CHIP_REG(0x29));


    if(CHIP_REG(0x29) != 0)
    {
        CHIP_REG(0x29) = 0;
    }
#endif

    for( i=0 ; i<EMMENG_NUMBER ; i++ )
    {
        u16Data = HAL_EMMFLT_ReadReg_Word(i,REG_EMM_CTRL0_L);

#if 0 //file in and bypass
    HALEMM_DBG(EMM_DBGLV_INFO, "File in and bypass\n");
    u16Data |= (EMM_FW_FILEIN | EMM_FLT_BYPASS);
#else //file in only
    HALEMM_DBG(EMM_DBGLV_INFO, "File in only\n");
    u16Data |= (EMM_FW_FILEIN);
#endif

        HAL_EMMFLT_WriteReg_Word(i,REG_EMM_CTRL0_L, u16Data);
    }


}

static void HAL_EMMFLT_LiveInSet(void)
{
    MS_U16 u16Data,i;

    for( i=0 ; i<EMMENG_NUMBER ; i++ )
    {
        u16Data = HAL_EMMFLT_ReadReg_Word(i,REG_EMM_CTRL0_L);
        u16Data &= ~(EMM_FW_FILEIN);

        HAL_EMMFLT_WriteReg_Word(i,REG_EMM_CTRL0_L, u16Data);
    }

}

MS_U32 HAL_EMMFLT_SrcSelect(MS_U32 connection, MS_U32 u32SrcFrom, MS_U32 u32SrcType)
{

    //CLKGEN0 bank, [0x29]_bit[11:8]=4'h8
    //CLKGEN0_REG(0x29) = ( CLKGEN0_REG(0x29) & 0xf0ff ) | 0x800 ;

    HALEMM_DBG(EMM_DBGLV_INFO,"Enter %s.....\n",__FUNCTION__);

    //[0x1137]_[0x1e]_[9] = 0
    //[0x101e]_[0x3a]_[6:4] = 3'b100: from demod0
    //[0x101e]_[0x43]_[15:0] = 16'h8000: reg_miu_wc_bypass
    //[0x101e]_[0x44]_[15:0] = 16'h0003: reg_miu_wc_bypass
    //[0x101e]_[0x02]_[7:0] = 8'h11  (reg_ts1_mode = 1, reg_ts0_mode=1)
    //[0x100b]_[0x2f]_[12:8] = 5'b100_00  :  clk_ts6_p
    //[0x100b]_[0x2a]_[4:0]  = 5'b010_00  :  clk_tsp
    //[0x1015]_[0x7a]_[15:0] = 16'h0002  software reset


    MS_U32 addr;
    MS_U16 u16Reg;

#if 1
	addr = _gBasicAddr + (0x101200*2 + 0x18*4);		// MIU_EN
    (*((volatile MS_U16*)(addr))) = 0xffff;

	addr = _gBasicAddr + (0x161300*2 + 0x00*4);		//$ enable MIUCrossbar
    (*((volatile MS_U16*)(addr))) = 0x000f;
#endif


    MsOS_DelayTask(1);
    PrintSetting();


    //connection needs to modify for K3
    if(u32SrcFrom == EMM_SRC_FILEIN)
    {
        HAL_EMMFLT_FileInSet();
    }
    else //EMM_LIVEIN
    {
        HAL_EMMFLT_LiveInSet();

        if(u32SrcType == EMM_SRC_TS0)
        {
            HALEMM_DBG(EMM_DBGLV_INFO,"[%s][%d] EMM_SRC_TS0\n",__FUNCTION__,__LINE__);

            //clock setting to TS0
            u16Reg = CLKGEN0_REG(0x26);
            CLKGEN0_REG(0x26) = 0;

            //pad set to TS0
            u16Reg = CHIP_REG(0x39);
            u16Reg &= (~0xf00<<(connection*4));
            u16Reg |= (EMM_FROM_TS0<<(8+connection*4));
            CHIP_REG(0x39) = u16Reg;


            u16Reg = CLKGEN2_REG(0x04+connection);
            u16Reg &= (~0x001f);
            u16Reg |= (EMM_FROM_TS0<<2);
            CLKGEN2_REG(0x04+connection) = u16Reg;
        }
        else if(u32SrcType == EMM_SRC_TS1)
        {
            HALEMM_DBG(EMM_DBGLV_INFO,"[%s][%d] EMM_SRC_TS1\n",__FUNCTION__,__LINE__);

            //clock setting to TS1
            u16Reg = CLKGEN0_REG(0x26);
            CLKGEN0_REG(0x26) = 0x404;

            //pad set to TS1
            u16Reg = CHIP_REG(0x39);
            u16Reg &= ~(0xf00<<(connection*4));
            u16Reg |= (EMM_FROM_TS1<<(8+connection*4));
            CHIP_REG(0x39) = u16Reg;


            u16Reg = CLKGEN2_REG(0x04+connection);
            u16Reg &= (~0x001f);
            u16Reg |= (EMM_FROM_TS1<<2);
            CLKGEN2_REG(0x04+connection) = u16Reg;
        }
        else if(u32SrcType == EMM_SRC_TS3)
        {
            //u16Reg = CHIP_REG(0x2);
            //u16Reg |= 0x18;
            //CHIP_REG(0x2) = u16Reg;

            //pad set to TS3
            u16Reg = CHIP_REG(0x39);
            u16Reg &= ~(0xf00<<(connection*4));
            u16Reg |= (EMM_FROM_TS3<<(8+connection*4));
            CHIP_REG(0x39) = u16Reg;


            u16Reg = CLKGEN2_REG(0x04+connection);
            u16Reg &= (~0x003f);
            u16Reg |= (EMM_FROM_TS3<<2);
            CLKGEN2_REG(0x04+connection) = u16Reg;
        }
        else if(u32SrcType == EMM_SRC_TS5)
        {
            // set emm flt clk src
            // clear
            u16Reg = CLKGEN2_REG(0x04 + connection);
            u16Reg &= (~0x001f);
            // set
            u16Reg |= (EMM_FROM_TS5 << 2);
            CLKGEN2_REG(0x04 + connection) = u16Reg;

            // set emm flt ts mux
            // clear
            u16Reg = CHIP_REG(0x39);
            u16Reg &= ~(0xf00<<(connection*4));
            // set
            u16Reg |= (EMM_FROM_TS5 << (8 + (connection * 4)));
            CHIP_REG(0x39) = u16Reg;
        }

    }

    return TRUE;
}


void PrintSetting(void)
{

    MS_U32 addr;
    MS_U16 u16Reg;


	#if 0

	KERES EMM PADDING
	//a.	reg_ts0_mode : bank_101e_0x02_[2:0]    = 3'd001;		//TS0 Mode
    u16Reg = CHIP_REG(0x02);
    u16Reg = (u16Reg & ~0x0007) | 0x1;
    CHIP_REG(0x02) = u16Reg;

	//b.	reg_ckg_ts4 :	   bank_100b_0x26_[3:0]    = 4'b0;
	//clk source for ts4: ts0_clk
	u16Reg = CLKGEN0_REG(0x26);
    u16Reg = (u16Reg & ~0x000f) | 0x0;
    CLKGEN0_REG(0x26) = u16Reg;

	//c.	reg_ckg_ts5 :	   bank_100b_0x26_[11:8]  = 4'b0;
	//clk source for ts5: ts0_clk
	u16Reg = CLKGEN0_REG(0x26);
    u16Reg = (u16Reg & ~0x0f00) | 0x0;
    CLKGEN0_REG(0x26) = u16Reg;

	//d.	reg_emmflt0_mux:  bank_101e_0x39_[10:8]    = 3'd000
	//Source Selections for EMMFLT Channel 0: come from PAD_TS0
	u16Reg = CHIP_REG(0x39);
    u16Reg = (u16Reg & ~0x0700) | 0x0;
    CHIP_REG(0x39) = u16Reg;

	//e.	reg_emmflt1_mux:  bank_101e_0x39_[14:12]	= 3'd000
	//Source Selections for EMMFLT Channel 1: come from PAD_TS0
	u16Reg = CHIP_REG(0x39);
    u16Reg = (u16Reg & ~0x7000) | 0x0;
    CHIP_REG(0x39) = u16Reg;

	#endif

    u16Reg = CHIP_REG(0x2);
    HALEMM_DBG(EMM_DBGLV_DEBUG, "CHIP 0x2 = %x\n", u16Reg);

    u16Reg = CHIP_REG(0x39);
    HALEMM_DBG(EMM_DBGLV_DEBUG, "CHIP 0x39 = %x\n", u16Reg);

    u16Reg = CLKGEN0_REG(0x26);
    HALEMM_DBG(EMM_DBGLV_DEBUG, "CLKGEN0 0x26 = %x\n", u16Reg);


    addr = _gBasicAddr + (0x101500*2 + 0x7a*4);
    u16Reg = (*((volatile MS_U16*)(addr)));
    HALEMM_DBG(EMM_DBGLV_DEBUG, "(TSP0, 0x101500*2 + 0x7a*4) = %x\n", u16Reg);

	#if 0
    addr = _gBasicAddr + (0x113700*2 + 0x1e*4);
    u16Reg = (*((volatile MS_U16*)(addr)));
    HALEMM_DBG(EMM_DBGLV_DEBUG, "(0x113700*2 + 0x1e*4) = %x\n", u16Reg);
	#endif
}


MS_U32 HAL_EMMFLT_Init(void)
{
    MS_U16 u16Data;
    MS_U32 i;

    for( i=0 ; i<EMMENG_NUMBER ; i++ )
    {
        u16Data = HAL_EMMFLT_ReadReg_Word(i,REG_EMM_RESET);

        u16Data |= EMM_RESET_UNLOCK;
        HAL_EMMFLT_WriteReg_Word(i,REG_EMM_RESET, u16Data);
    }

    for( i=0 ; i<EMMENG_NUMBER ; i++ )
    {
        u16Data = HAL_EMMFLT_ReadReg_Word(i,REG_EMM_STR2MIU_EN);
        u16Data |= EMM_STR2MIU_EN;
        HAL_EMMFLT_WriteReg_Word(i, REG_EMM_STR2MIU_EN, u16Data); //string 2 miu enable

        HALEMM_DBG(EMM_DBGLV_INFO, "enable stream to miu \n");

        //designer suggest setting....still needs to confirm

		HAL_EMMFLT_WriteReg_Word(i, REG_EMM_TS_IF2_CTRL, 0x80e1);
        //HAL_EMMFLT_WriteReg_Word(i, REG_EMM_TS_IF2_CTRL, 0x8fe1);		//DEBUG
    }

    return TRUE;
}

MS_U32 HAL_EMMFLT_GetHwBufCnt(MS_U32 *pHwBufCnt)
{
    *pHwBufCnt = EMMFLT_HWBUF_NUM;
    return TRUE;
}

//does connection needs to be increased.
MS_U32 HAL_EMMFLT_SWReset(MS_U32 connection)
{
    MS_U16 u16Data;

    u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_RESET);
    u16Data &= (~EMM_RESET_UNLOCK);

    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_RESET, u16Data);

    u16Data |= EMM_RESET_UNLOCK;
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_RESET, u16Data);

    return TRUE;
}

MS_U32 HAL_EMMFLT_SetDbgLevel(MS_U32 u32Level)
{
    _g32EMMHalDbgLv = u32Level;
    HALEMM_DBG(EMM_DBGLV_DEBUG, "%s level: %x\n", __FUNCTION__, u32Level);
    return TRUE;
}


MS_U32 HAL_EMMFLT_SetOutputType(MS_U32 connection, MS_U32 u32Type)
{
    MS_U16 u16Data;

    HALEMM_DBG(EMM_DBGLV_INFO, "SetOutputType conn = %x, Type = %x \n",connection,u32Type);
    if(u32Type == EMM_OUT_NORMAL)
    {
        u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_GENERAL_CTRL_L);
        u16Data &= ~(__BIT2);
        HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_GENERAL_CTRL_L, u16Data);
    }
    else if(u32Type == EMM_OUT_184BYTES)
    {

    }
    else if(u32Type == EMM_OUT_PACKETNUM)
    {
        u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_GENERAL_CTRL_L);
        u16Data |= (__BIT2);
        HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_GENERAL_CTRL_L, u16Data);
    }
    return TRUE;
}

MS_U32 HAL_EMMFLT_Enable_Int(void)
{
    MS_U32 i;

    HALEMM_DBG(EMM_DBGLV_INFO, "EMMFLT_Enable_Int\n");
    for( i=0 ; i<EMMENG_NUMBER ; i++ )
    {
        //HAL_EMMFLT_WriteReg_Word(REG_EMM_CA_INT,0);
        //HAL_EMMFLT_WriteReg_Word( i, 0x0075, 0x3);
        HAL_EMMFLT_WriteReg_Word( i, REG_EMM_CA_INT, (EMM_ONEPAKCET_INT | EMM_OVERFLOW_INT));
    }
    return TRUE;
}

MS_U32 HAL_EMMFLT_GetIntNumber(void)
{
    return EMM_IRQ_INT;
}

MS_BOOL HAL_EMMFLT_GetIntMode(void)
{
    //seprated interrupt.
    return FALSE;
}

MS_U32 HAL_EMMFLT_SetTidMode(MS_U32 connection, MS_U8 u8TidValue, EMM_TIDMODE_e eTIDMODE)
{

    MS_U32 u32Data;

    ConnectionCheck(connection);

    u32Data = HAL_EMMFLT_ReadReg_Dword(connection,REG_EMM_TID_MODE_L);

    u32Data &= ~((MS_U32)E_TIDMODE_RESERVED<<u8TidValue);
    u32Data |= ((MS_U32)eTIDMODE<<u8TidValue);


    HAL_EMMFLT_WriteReg_Dword(connection,REG_EMM_TID_MODE_L,u32Data);
    HALEMM_DBG(EMM_DBGLV_INFO, "%s SetTidMode value : %x\n", __FUNCTION__, u32Data);
    return TRUE;
}

MS_U32 HAL_EMMFLT_SetIRDMode(MS_U32 connection,MS_U8 u8IRDNum, MS_U8 u8CompareMode)
{
    MS_U16 u16Mask,u16Data;

    ConnectionCheck(connection);
    if(u8CompareMode > REG_EMM_CTRL_MAX)
    {
        return HAL_EMMFLT_INVALID_REQUEST;
    }

    if(u8IRDNum > REG_EMM_IRD_MAX)
    {
        return HAL_EMMFLT_INVALID_REQUEST;
    }

    HALEMM_DBG(EMM_DBGLV_INFO, "%s u8IRDNum= %d, u8CompareMode = %d\n", __FUNCTION__, u8IRDNum, u8CompareMode );
    u16Mask = (EMM_IRD_REG_MASK << (u8IRDNum*2));

    u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_CTRL_ID);

    HALEMM_DBG(EMM_DBGLV_INFO, "REG_EMM_CTRL_ID = %x\n",u16Data);
    u16Data &= ~u16Mask;


    u16Data |= (u8CompareMode << (u8IRDNum*2));
    HALEMM_DBG(EMM_DBGLV_INFO, "%s u16Data= %x \n", __FUNCTION__, u16Data);
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_CTRL_ID, u16Data);

    return TRUE;
}


MS_U32 HAL_EMMFLT_SetEmmDataIDx(MS_U32 connection, MS_U8 u8IRDNum, MS_U8 *pu8Data)
{
    MS_U32 u32IRDAddr,u32EMMAddr;
    MS_U32 u32Mapping[] = {REG_EMM_DATA_ID1_L, REG_EMM_DATA_ID2_L,
                           REG_EMM_DATA_ID3_L, REG_EMM_DATA_ID4_L,
                           REG_EMM_DATA_ID5_L, REG_EMM_DATA_ID6_L,
                           REG_EMM_DATA_ID7_L, REG_EMM_DATA_ID8_L, };


    ConnectionCheck(connection);

    u32EMMAddr = ((MS_U32)pu8Data[0]<<24) + ((MS_U32)pu8Data[1]<<16) +
                 ((MS_U32)pu8Data[2]<<8) + ((MS_U32)pu8Data[3]) ;


    HALEMM_DBG(EMM_DBGLV_INFO, "%s u8IRDNum= %x, u32EMMAddr = %x\n", __FUNCTION__, u8IRDNum, u32EMMAddr);

    if(u8IRDNum >= REG_EMM_IRD_MAX)
    {
        return HAL_EMMFLT_INVALID_REQUEST;
    }


    u32IRDAddr = u32Mapping[u8IRDNum];

    HAL_EMMFLT_WriteReg_Dword(connection,u32IRDAddr,u32EMMAddr);

    return TRUE;
}

MS_U32 HAL_EMMFLT_SetEmmMaskIDx(MS_U32 connection,MS_U8 u8IRDNum, MS_U8 *pu8Data)
{
    MS_U32 u32IRDAddr,u32EMMMask;
    MS_U32 u32Mapping[] = {REG_EMM_MASK_ID1_L, REG_EMM_MASK_ID2_L,
                           REG_EMM_MASK_ID3_L, REG_EMM_MASK_ID4_L,
                           REG_EMM_MASK_ID5_L, REG_EMM_MASK_ID6_L,
                           REG_EMM_MASK_ID7_L, REG_EMM_MASK_ID8_L, };


    ConnectionCheck(connection);

    u32EMMMask = ((MS_U32)pu8Data[0]<<24) + ((MS_U32)pu8Data[1]<<16) +
                 ((MS_U32)pu8Data[2]<<8) + ((MS_U32)pu8Data[3]) ;
    HALEMM_DBG(EMM_DBGLV_INFO, "%s u8IRDNum= %x, u32EMMMask = %x\n", __FUNCTION__, u8IRDNum, u32EMMMask);

    if(u8IRDNum >= REG_EMM_IRD_MAX)
    {
        return HAL_EMMFLT_INVALID_REQUEST;
    }


    u32IRDAddr = u32Mapping[u8IRDNum];

    HAL_EMMFLT_WriteReg_Dword(connection,u32IRDAddr,u32EMMMask);

    return TRUE;
}

MS_U32 HAL_EMMFLT_DisableEMM(MS_U32 connection)
{
    ConnectionCheck(connection);

    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_PID, 0);
    return TRUE;
}

MS_U32 HAL_EMMFLT_SetEmmPID(MS_U32 connection,MS_U16 u16EmmPID)
{
    MS_U16 u16Data;

    HALEMM_DBG(EMM_DBGLV_INFO, "EMMFLT_SetEmmPID conn = %x, u16EmmPID = %x \n",connection,u16EmmPID);
    PrintSetting();

    ConnectionCheck(connection);
#if 1
    u16Data = 0;
    u16Data = u16EmmPID;
#else
    u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_PID);

    u16Data = ( u16Data & ~(REG_EMM_ENABLE_TID | REG_EMM_ENABLE_PID) ) | u16EmmPID;
#endif
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_PID, u16Data);
    return TRUE;
}

MS_U32 HAL_EMMFLT_SetEmmTID(MS_U32 connection,MS_U16 u16EmmTID)
{
    HALEMM_DBG(EMM_DBGLV_INFO, "EMMFLT_SetEmmTID conn = %x, u16EmmTID = %x \n",connection,u16EmmTID);
    ConnectionCheck(connection);
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_TID, u16EmmTID);
    return TRUE;
}

MS_U32 HAL_EMMFLT_EnableEmmTID(MS_U32 connection, MS_BOOL bEnable)
{
    MS_U16 u16Data;

    HALEMM_DBG(EMM_DBGLV_INFO, "EMMFLT_EnableEmmTID conn = %x, bEnable = %x \n",connection,bEnable);
    ConnectionCheck(connection);
    u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_PID);

    if(TRUE == bEnable)
    {
        u16Data |= REG_EMM_ENABLE_TID;
    }
    else
    {
        u16Data &= (~REG_EMM_ENABLE_TID);
    }

    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_PID, u16Data);

    return TRUE;
}


MS_U32 HAL_EMMFLT_ResetInt(MS_U32 connection)
{
    MS_U16 u16Data;

    HALEMM_DBG(EMM_DBGLV_ERR, "EMMFLT_ResetInt conn = %x\n",connection);

    u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_CA_INT);

    u16Data |= EMM_RESET_INT;

	HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_CA_INT, u16Data);

    return TRUE;
}

#if 0
MS_U32 HAL_EMMFLT_GetIntReg(MS_U16 *pu16Data)
{
    MS_U16 u16Data = 0,u16I;
    *pu16Data = 0;

    for( u16I=0 ; u16I<EMMENG_NUMBER ; u16I++)
    {
        u16Data = HAL_EMMFLT_ReadReg_Word( u16I, REG_EMM_CA_INT );
        *pu16Data = u16Data & (EMM_ONEPAKCET_INT | EMM_OVERFLOW_INT);
    }

    return TRUE;
}
#endif

MS_U32 HAL_EMMFLT_GetIntStatus(MS_U16 *pu16EMMInt, MS_U16 *pu16IntStat, MS_U8 *pu8conflag)
{
    MS_U16 u16Data,i;

    for( i=0; i<EMMENG_NUMBER ; i++)
    {
        pu8conflag[i] = FALSE;
        u16Data = HAL_EMMFLT_ReadReg_Word(i,REG_EMM_CA_INT);
        HALEMM_DBG(EMM_DBGLV_DEBUG, "%s i= %x, u16Data = %x\n", __FUNCTION__, i, u16Data);

        //that means the first emm has interrupt....
        if( (u16Data & EMMFLT_EMM_OVERFLOW_INT) || (u16Data & EMMFLT_EMM_INT) )
        {
            pu8conflag[i]  =  TRUE;
            pu16IntStat[i] = (u16Data & EMM_INT_MASK) ;
            pu16EMMInt[i]  =  u16Data;
        }
    }
    HALEMM_DBG(EMM_DBGLV_DEBUG, "Int status = %x\n",u16Data);

    return TRUE;
}

MS_U32 HAL_EMMFLT_GetCurrentBufIndex(MS_U32 connection)
{
    MS_U32 index = HAL_EMMFLT_ReadReg_Word(connection, REG_EMM_INT_STAT);

    //MS_U32 index = (HAL_EMMFLT_ReadReg_Word(connection, REG_EMM_CA_INT)>>8);
    HALEMM_DBG(EMM_DBGLV_INFO, "current index = %x\n",index);
    return index;
}

MS_U32 HAL_EMMFLT_ReqDstBufSize(MS_U32 *p32DstBufSize, MS_U32 *p32AlignBytes, MS_U8 *p8BufBum)
{
    *p32DstBufSize = EMMFLT_HWBUF_SIZE;
    *p32AlignBytes = EMMFLT_BUF_ALIGNMENT;
    *p8BufBum = EMMENG_NUMBER;

    HALEMM_DBG(EMM_DBGLV_INFO, "ReqDstBufSize HWBUF_SIZE = %x, BUF_ALIGNMENT = %x, EMMENG_NUMBER = %x\n",EMMFLT_HWBUF_SIZE, EMMFLT_BUF_ALIGNMENT,EMMENG_NUMBER);
    return TRUE;
}

MS_U32 HAL_EMMFLT_SetDstBufInfo(MS_U32 connection, MS_U32 u32BufAddr, MS_U32 u32BufSize, MS_U32 *p32BufAddrs)
{
    MS_U32 u32I;
    MS_U16 u16Reg;

    ConnectionCheck(connection);
    HALEMM_DBG(EMM_DBGLV_INFO, "%s u32BufAddr= %x, u32BufSize= %x \n", __FUNCTION__, u32BufAddr,u32BufSize);

    HAL_EMMFLT_WriteReg_Dword(connection,REG_EMM_STR2MIU_HEAD1_L, (u32BufAddr>>4));
    HAL_EMMFLT_WriteReg_Dword(connection,REG_EMM_STR2MIU_TAIL1_L, ((u32BufAddr+u32BufSize)>>4));
    HAL_EMMFLT_WriteReg_Dword(connection,REG_EMM_STR2MIU_MID1_L,  ((u32BufAddr+u32BufSize)>>4));


    u16Reg = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_STR2MIU_EN);
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_STR2MIU_EN, (u16Reg | 0x4) );

    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_STR2MIU_EN,u16Reg);

    for(u32I = 0; u32I<MAX_EMMFLT_NUM ; u32I++)
    {
        p32BufAddrs[u32I] = u32BufAddr + (u32I*U01_NDS_EMMFLT_BUF_SIZE);
        HALEMM_DBG(EMM_DBGLV_INFO, "%s p32BufAddr[%x] = %x, \n", __FUNCTION__, u32I, p32BufAddrs[u32I]);
    }

    return TRUE;
}


//does connection needs to be increased.
MS_U32 HAL_EMMFLT_PacketCount(MS_U32 connection,MS_U8 *pCount)
{
    MS_U16 PacketCnt;
    PacketCnt = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_PACKET_CNT);
    HALEMM_DBG(EMM_DBGLV_INFO, "REG_EMM_PACKET_CNT = %x\n",PacketCnt);
    *pCount = (MS_U8)((PacketCnt>>8)&0xff);
    return TRUE;
}



//does connection needs to be increased.
MS_U32 HAL_EMMFLT_PacketAct(MS_U32 connection)
{
#if 0
    MS_U16 u16Rg;
    u16Rg = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_CA_INT);
    u16Rg = (0x10<<8);
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_CA_INT,u16Rg);
#else
    HALEMM_DBG(EMM_DBGLV_INFO, "HAL_EMMFLT_PacketAct = %x\n",connection);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_INT_STAT,EMM_RECIEVE_ACT);
    //HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_CA_INT,(EMM_RECIEVE_ACT<<8) + 0x6);
#endif

    //MS_U16 u16Data;
    //MsOS_DelayTaskUs(10);

    //u16Data = HAL_EMMFLT_ReadReg_Word(REG_EMM_INT_STAT);

    //HALEMM_DBG(0, "HAL_EMMFLT_PacketAct = %x\n",u16Data);

    return TRUE;
}


MS_U32 HAL_EMMFLT_IntCtrl(MS_U8 u8En)
{
    MS_U16 u16Data;

    HALEMM_DBG(EMM_DBGLV_INFO, "EMMFLT_IntCtrl u8En = %x\n",u8En);

    u16Data = HAL_EMMFLT_ReadReg_Word(0,REG_EMM_CA_INT);

    if(1 == u8En ) //mask interrupt
    {
        u16Data &= (~EMM_RESET_INT);
        u16Data &= ~(EMM_ONEPAKCET_INT | EMM_OVERFLOW_INT);
    }
    else //unmask interrupt....
    {
        u16Data &= (~EMM_RESET_INT);
        u16Data |= (EMM_ONEPAKCET_INT | EMM_OVERFLOW_INT);
    }

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_CA_INT, u16Data);
    return TRUE;
}


MS_U32 HAL_EMMFLT_SetDebugMode(MS_U16 u16Mode)
{
    MS_U16 u16Reg,u16I;

    for(u16I=0; u16I<EMMENG_NUMBER; u16I++)
    {
        u16Reg = HAL_EMMFLT_ReadReg_Word(u16I,REG_EMM_TS_IF2_CTRL);

        u16Reg &= ~(REG_EMM_DGB_SEL);
        u16Reg |= (u16Mode<<8);

        u16Reg |= (0x1<<13);
        HAL_EMMFLT_WriteReg_Word(u16I,REG_EMM_TS_IF2_CTRL, u16Reg);
    }
    return TRUE;
}

MS_U32 HAL_EMMFLT_GetDebugStatus(MS_U32 *pRegValue)
{
    *pRegValue = HAL_EMMFLT_ReadReg_Dword(0,REG_EMM_TS_IF2_DEBUG_L);
    return TRUE;
}


//#define PureFileInOut

MS_U32 HAL_EMMFLT_GeneralCtrl(MS_U32 connection)
{

#if 0
    u16Data = HAL_EMMFLT_ReadReg_Word(REG_EMM_STR2MIU_EN);
    u16Data |= REG_STR2MIU_RST_WADR;

    HALEMM_DBG(0, "REG_EMM_STR2MIU_EN = %x\n",(u16Data | REG_STR2MIU_RST_WADR));

    HAL_EMMFLT_WriteReg_Word(REG_EMM_STR2MIU_EN, (u16Data | REG_STR2MIU_RST_WADR) );

    MsOS_DelayTaskUs(1);

    u16Data &= ~(REG_STR2MIU_RST_WADR);
    HALEMM_DBG(0, "REG_EMM_STR2MIU_EN = %x\n",u16Data);
    HAL_EMMFLT_WriteReg_Word(REG_EMM_STR2MIU_EN, u16Data);
#endif

#ifdef PureFileInOut
    MS_U16 u16Data;
    u16Data = HAL_EMMFLT_ReadReg_Word(connection,REG_EMM_PID);
    u16Data &= 0x3fff;
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_PID, u16Data);
#endif

    //HAL_EMMFLT_WriteReg_Word(REG_EMM_GENERAL_CTRL_L, 0x0004);

	/*	select parallel TS interface for TS interface 2 in emm_flt0
	select exteranl sync for ts_if2 in emm_flt0
	Set 1 to enable the patch of internal sync in ¡§tsif¡¨ in emm_flt0
	set 1 to enable ts_if2 in emm_flt0		*/
	//HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_TS_IF2_CTRL, 0x80e1);

    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_GENERAL_CTRL_L, 0x0000);
    HAL_EMMFLT_WriteReg_Word(connection,REG_EMM_GENERAL_CTRL_H, 0x0010);	//Bit[23:20]: packet number enable bits for TS3-TS0

    HALEMM_DBG(EMM_DBGLV_ERR, "fire EMM filter\n");

    return TRUE;
}

MS_U32 HAL_EMMFLT_ConnectCheck(MS_U32 connection)
{

    ConnectionCheck(connection);
    return TRUE;
}

MS_U32 HAL_EMMFLT_HWSimulation(void)
{
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_GENERAL_CTRL_L, 0x0004);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_GENERAL_CTRL_H, 0x0010);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_CA_INT, 0x0006);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_TS_IF2_CTRL, 0x80e1);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_PID, 0xC000);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_TID, 0x0000);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_TID_MODE_L, 0xFFFF);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_TID_MODE_H, 0x0000);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_EN, 0x0002); //string 2 miu enable

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_HEAD1_L, 0x0000);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_HEAD1_H, 0x0000);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_TAIL1_L, 0x006C);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_TAIL1_H, 0x0000);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_MID1_L, 0x0010);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_MID1_H, 0x0000);


    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_HEAD2_L, 0x1000);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_HEAD2_H, 0x0000);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_TAIL2_L, 0x106C);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_TAIL2_H, 0x0000);

    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_MID2_L, 0x1010);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_STR2MIU_MID2_H, 0x0000);

    return TRUE;
}

void HAL_EMMFLT_IntStatusTest(void)
{
    MS_U16 CA_INT, INT_STAT;

    CA_INT = HAL_EMMFLT_ReadReg_Word(0,REG_EMM_CA_INT);
    HALEMM_DBG(EMM_DBGLV_INFO, "REG_EMM_CA_INT = %x\n",CA_INT);

    INT_STAT = HAL_EMMFLT_ReadReg_Word(0,REG_EMM_INT_STAT);
    HALEMM_DBG(EMM_DBGLV_INFO, "REG_EMM_INT_STAT = %x\n",INT_STAT);


    if(CA_INT == 0x2)
    {
        HAL_EMMFLT_WriteReg_Word(0,REG_EMM_INT_STAT,EMM_RECIEVE_ACT);
        HAL_EMMFLT_WriteReg_Word(0,REG_EMM_CA_INT,(EMM_RECIEVE_ACT<<8) + 0x6);
    }
    else if(CA_INT == 0x6)
    {
        HALEMM_DBG(EMM_DBGLV_INFO, "Buffer overflow\n");
    }

}

MS_U32 HAL_EMMFLT_SetExtendConfig(MS_U32 x_connection, MS_U16 type, MS_U16 extendbytes, MS_U16 syncbyte)
{
    MS_U16 Reg;

    Reg = 0x204;
    Reg = (Reg&(~0x1f0)) | ((extendbytes&0x1f)<<4);
    printf("EMM HW Config 0 is %x\n",Reg);
    HAL_EMMFLT_WriteReg_Word(x_connection,REG_EMM_HW_CONFIG0, 0x02C4);


    Reg = HAL_EMMFLT_ReadReg_Word(x_connection,REG_EMM_SYNC_BYTES);

    Reg = ((extendbytes + 188)<<0x8) | (syncbyte&0xff);
    printf("sync byte reg is %x\n",Reg);
    HAL_EMMFLT_WriteReg_Word(x_connection,REG_EMM_SYNC_BYTES, Reg);

    return TRUE;
}

MS_U32 HAL_EMMFLT_Bypass(void)
{
    MS_U16 Reg;
    Reg = HAL_EMMFLT_ReadReg_Word(0,REG_EMM_CTRL0_L);
    HAL_EMMFLT_WriteReg_Word(0,REG_EMM_CTRL0_L, (Reg | EMM_FLT_BYPASS) );

    return TRUE;
}

MS_U32 HAL_EMMFLT_En192Output(MS_U32 x_connection)
{
    MS_U16 Reg;

    Reg = HAL_EMMFLT_ReadReg_Word(x_connection,REG_EMM_CTRL0_L);
    HAL_EMMFLT_WriteReg_Word(x_connection,REG_EMM_CTRL0_L, (Reg & ~(EMM_PACKET256_EN | EMM_PVR_EN)) );

    Reg = HAL_EMMFLT_ReadReg_Word(x_connection,REG_EMM_STR2MIU_CTRL);
    HAL_EMMFLT_WriteReg_Word(x_connection,REG_EMM_STR2MIU_CTRL, (Reg | REG_PKT192_EN) );

    return TRUE;
}

MS_U32 HAL_EMMFLT_InputMode(MS_U32 x_connection, MS_BOOL bSerial)
{
    MS_U16 Reg_TS_IF2_CTRL = 0, Reg_STR2MIU_CTRL = 0;

    Reg_TS_IF2_CTRL = HAL_EMMFLT_ReadReg_Word(x_connection, REG_EMM_TS_IF2_CTRL );
    Reg_TS_IF2_CTRL &= ~(REG_SIM_C0_CONFIG | REG_SIM_C1_CONFIG | REG_P_SEL2 | REG_EXT_SYNC_SEL2 | REG_SERIAL_EXT_SYNC_1T); // bit 2,3,5,6,12

    Reg_STR2MIU_CTRL = HAL_EMMFLT_ReadReg_Word(x_connection, REG_EMM_STR2MIU_CTRL );

    if( bSerial == TRUE ) //input with serial mode....
    {
        Reg_TS_IF2_CTRL |= (REG_SIM_C0_CONFIG | REG_SIM_C1_CONFIG | REG_EXT_SYNC_SEL2 | REG_SERIAL_EXT_SYNC_1T);  // bit[3..2]=11, bit[6..5]=10,bit[12]=1
        Reg_STR2MIU_CTRL |= REG_RECORD_AT_SYNC_DIS;  // bit[10]=1
    }
    else  //input with parallel mode....
    {
        Reg_TS_IF2_CTRL |= (REG_P_SEL2 | REG_EXT_SYNC_SEL2);  // bit[3..2]=00, bit[6..5]=11,bit[12]=0
        Reg_STR2MIU_CTRL &= ~(REG_RECORD_AT_SYNC_DIS);  // bit[10]=0
    }

    HAL_EMMFLT_WriteReg_Word( x_connection, REG_EMM_TS_IF2_CTRL, Reg_TS_IF2_CTRL );
    HAL_EMMFLT_WriteReg_Word( x_connection, REG_EMM_STR2MIU_CTRL, Reg_STR2MIU_CTRL );
    return TRUE;
}

