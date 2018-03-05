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
// file   halPVR.c
// @brief  PVR HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "halCHIP.h"
#include "regTSP.h"
#include "halTSP.h"
#include "drvSYS.h"


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
MS_VIRT              _u32RegBase                       = 0;

static REG_Ctrl*    _RegCtrl                          = NULL;
static REG_Ctrl2*   _RegCtrl2                         = NULL;
static REG_Ctrl3*   _RegCtrl3                         = NULL;
static REG_Ctrl4*   _RegCtrl4                         = NULL;
static REG_Ctrl5*   _RegCtrl5                         = NULL;
static REG_Ctrl6*   _RegCtrl6                         = NULL;
static REG_Ctrl7*   _RegCtrl7                         = NULL;
static REG_Ctrl8*   _RegCtrl8                         = NULL;



// @F_TODO These parameters need to be combined with global variables in Utopia 2.0
static MS_U32                       _u32PidFltReg[(TSP_PIDFLT_NUM * sizeof(REG_PidFlt))];
static MS_U32                       _u32PidDstReg[(TSP_PIDFLT_NUM * sizeof(REG_PidFlt))];
static MS_U32                       _u32SecReg[(TSP_SECFLT_NUM * ((sizeof(REG_SecFlt) - sizeof(((REG_SecFlt*)0)->_x50))/sizeof(TSP32)))];


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSP_HCMD_BufRst(MS_U32 u32Value);

static void _delay(MS_U32 usec)
{
    MsOS_DelayTaskUs_Poll(usec);
}

static MS_U32 REG32_R(REG32 *reg)
{
    MS_U32              value = 0;
    value  = (reg)->low;
    value |= (reg)->high << 16;
    return value;
}


static MS_U16 REG16_R(REG16 *reg)
{
    MS_U16              value = 0;
    value = (reg)->data;
    return value;
}

static MS_BOOL _u32RegDump = 0;
static void _REG32_DUMP(REG32 * reg, MS_U32 value );
static void _REG16_DUMP(REG16 * reg, MS_U16 value);

#if 0
#define REG32_W(reg, value);    { (reg)->low = ((value) & 0x0000FFFF);                          \
                                  (reg)->high = ((value) >> 16);\
                                  if(_u32RegDump)\
    {printf("wriu bank:0x%04X addr:0x%02X value:0x%04X\n", (((MS_U32)(&(reg)->low) - _u32RegBase)>>(1+8)), ((((MS_U32)(&(reg)->low) - _u32RegBase)>>1)&0xFF)>>1,(value) & 0x0000FFFF);\
     printf("wriu bank:0x%04X addr:0x%02X value:0x%04X\n\n", (((MS_U32)(&(reg)->high) - _u32RegBase)>>(1+8)),((((MS_U32)(&(reg)->high) - _u32RegBase)>>1)&0xFF)>>1, (value) >> 16);}}

#define REG16_W(reg, value);    {(reg)->data = ((value) & 0x0000FFFF);\
                                  if(_u32RegDump)\
    {printf("wriu bank:0x%04X addr:0x%02X value:0x%04X\n\n", (((MS_U32)reg) - _u32RegBase)>>(1+8), ((((MS_U32)reg - _u32RegBase)>>1)&0xFF)>>1, (value) & 0x0000FFFF);}}
#endif
#define REG32_W(reg, value)    { (reg)->low = ((value) & 0x0000FFFF);                          \
                                  (reg)->high = ((value) >> 16);\
                                if(_u32RegDump){_REG32_DUMP(reg, value);}}

#define REG16_W(reg, value)    {(reg)->data = ((value) & 0x0000FFFF);\
                                if(_u32RegDump){_REG16_DUMP(reg, value);}}


#define _AND_(flag, bit)            ((flag) &   (bit) )

#define _SET_(flag, bit)            ((flag) |   (bit) )
#define _CLR_(flag, bit)            ((flag) & (~(bit)))

#define REG16_SET(reg, value)    REG16_W(reg, _SET_(REG16_R(reg), value))
#define REG32_SET(reg, value)    REG32_W(reg, _SET_(REG32_R(reg), value))
#define REG16_CLR(reg, value)    REG16_W(reg, _CLR_(REG16_R(reg), value))
#define REG32_CLR(reg, value)    REG32_W(reg, _CLR_(REG32_R(reg), value))

#define REG16_MSK_W(reg, mask, value)    REG16_W((reg), _CLR_(REG16_R(reg), (mask)) | _AND_((value), (mask)))
#define REG32_MSK_W(reg, mask, value)    REG32_W((reg), _CLR_(REG32_R(reg), (mask)) | _AND_((value), (mask)))

#define MIU_BUS (4)


//-------------------------------------------------------------------------------------------------
//  Debug Message
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_HAL_TSP_DBG_LEVEL_NONE,   // no debug message shown
    E_HAL_TSP_DBG_LEVEL_ERR,    // only shows error message that can't be recover
    E_HAL_TSP_DBG_LEVEL_WARN,   // error case can be recover, like retry
    E_HAL_TSP_DBG_LEVEL_EVENT,  // event that is okay but better known, ex: timestamp ring, file circular, etc.
    E_HAL_TSP_DBG_LEVEL_INFO,   // information for internal parameter
    E_HAL_TSP_DBG_LEVEL_FUNC,   // Function trace and input parameter trace
    E_HAL_TSP_DBG_LEVEL_TRACE,  // debug trace
} EN_HAL_TSP_DBGMSG_LEVEL;

typedef enum
{
    E_HAL_TSP_DBG_MODEL_NONE,         // @temporarily , need to refine
    E_HAL_TSP_DBG_MODEL_ALL,
} EN_HAL_TSP_DBGMSG_MODEL;

#define HAL_TSP_DBGMSG(_level,_model,_f) do {if(_u32TSPDbgLevel >= (_level)&&((_u32TSPDbgModel&_model)!=0)) (_f);} while(0)
static MS_U32  _u32TSPDbgLevel = E_HAL_TSP_DBG_LEVEL_ERR;
static MS_U32  _u32TSPDbgModel = E_HAL_TSP_DBG_MODEL_ALL;


//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
void TSP32_IdrW(TSP32 *preg, MS_U32 value)
{
    MS_U32 tempDump = _u32RegDump;
    if(_u32RegDump)
    {
        //test_chip_top.write_ind32('h00221004, 32'hffffffff);  //indirect .... (address,data)
        _u32RegDump = 0;
        printf("test_chip_top.write_ind32(\'h%08lx, 32\'h%08lx);\n", (long unsigned int)preg, (long unsigned int)value);
    }
    REG32_W(&_RegCtrl->Idr_Addr, (MS_VIRT)preg);
    REG32_W(&_RegCtrl->Idr_Write, value);
    REG16_W(&_RegCtrl->Idr_Ctrl, TSP_IDR_START | TSP_IDR_WRITE);
    _u32RegDump = tempDump;
}

MS_U32 TSP32_IdrR(TSP32 *preg)
{
    MS_U32 tempDump = _u32RegDump;
    if(_u32RegDump)
    {
        _u32RegDump = 0;
    }
    REG32_W(&_RegCtrl->Idr_Addr,  (MS_VIRT)preg);
    REG16_W(&_RegCtrl->Idr_Ctrl, TSP_IDR_START | TSP_IDR_READ);
    _u32RegDump = tempDump;
    return REG32_R(&_RegCtrl->Idr_Read);
}

void HAL_TSP_SetBank(MS_VIRT u32BankAddr)
{
    _u32RegBase = u32BankAddr;
    _RegCtrl    = (REG_Ctrl*)(u32BankAddr + 0x2A00UL);        //TSP0 0x1015, TSP1 0x1016
    _RegCtrl2   = (REG_Ctrl2*)(u32BankAddr  + 0xE0400UL);     //TSP3 0x1702,
    _RegCtrl3   = (REG_Ctrl3*)(u32BankAddr  + 0xE0600UL);     //TSP4 0x1703
    _RegCtrl4   = (REG_Ctrl4*)(u32BankAddr  + 0xC2000UL);     //TSP6 0x1610
    _RegCtrl5   = (REG_Ctrl5*)(u32BankAddr  + 0xC2200UL);     //TSP7 0x1611
    _RegCtrl6   = (REG_Ctrl6*)(u32BankAddr  + 0xC4E00UL);     //TSP8 0x1627
    _RegCtrl7   = (REG_Ctrl7*)(u32BankAddr  + 0xE1800UL);     //TSP9 0x170C
    _RegCtrl8   = (REG_Ctrl8*)(u32BankAddr  + 0xE1A00UL);     //TSP10 0x170D
}

void HAL_TSP_RegDump(MS_BOOL bEnable)
{
    _u32RegDump = bEnable;
}
#if 1
static void _REG32_DUMP(REG32* reg, MS_U32 value )
{
    //`RIU_W(`TSP_REG_BASE + 7'h06, 2'b11, 16'h400a);   //..bank 15 ,7’h06 ....reg ,16’h400a ......
    //`RIU_W(`TSP_REG_BASE1 + 7'h72, 2'b11, 16'hc000); // ..bank 16 ,7’h06 ....reg ,16’h400a ......
    //`RIU_W((24'h103800>>1) +  7'h56 ,  2'b11, 16'h0000); //....bank 038
    MS_U32 bank = ((MS_VIRT)(&(reg)->low) - _u32RegBase)>>(1+8);
    MS_U32 addr_low = ((((MS_VIRT)(&(reg)->low) - _u32RegBase)>>1)&0xFF)>>1;
    MS_U32 addr_high = ((((MS_VIRT)(&(reg)->high) - _u32RegBase)>>1)&0xFF)>>1;
    MS_U32 val_low  = (value) & 0x0000FFFF;
    MS_U32 val_high  = ((value) & 0xFFFF0000 )>>16;

    if(bank == 0x15)
    {
        printf("`RIU_W(TSP_REG_BASE + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr_low, (long unsigned int)val_low);
        printf("`RIU_W(TSP_REG_BASE + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr_high, (long unsigned int)val_high);
    }
    else if(bank == 0x16)
    {
        printf("`RIU_W(TSP_REG_BASE1 + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr_low, (long unsigned int)val_low);
        printf("`RIU_W(TSP_REG_BASE1 + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr_high, (long unsigned int)val_high);
    }
    else if(bank == 0x38)
    {
        printf("`RIU_W((24\'h103800>>1) + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr_low, (long unsigned int)val_low);
        printf("`RIU_W((24\'h103800>>1) + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr_high, (long unsigned int)val_high);
    }
}
static void _REG16_DUMP(REG16* reg, MS_U16 value )
{
    //`RIU_W(`TSP_REG_BASE + 7'h06, 2'b11, 16'h400a);   //..bank 15 ,7’h06 ....reg ,16’h400a ......
    //`RIU_W(`TSP_REG_BASE1 + 7'h72, 2'b11, 16'hc000); // ..bank 16 ,7’h06 ....reg ,16’h400a ......
    //`RIU_W((24'h103800>>1) +  7'h56 ,  2'b11, 16'h0000); //....bank 038
    MS_U32 bank = ((MS_VIRT)(&(reg)) - _u32RegBase)>>(1+8);
    MS_U32 addr = ((((MS_VIRT)(&(reg)) - _u32RegBase)>>1)&0xFF)>>1;
    MS_U32 val  = (value) & 0x0000FFFF;

    if(bank == 0x15)
    {
        printf("`RIU_W(TSP_REG_BASE + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr, (long unsigned int)val);
    }
    else if(bank == 0x16)
    {
        printf("`RIU_W(TSP_REG_BASE1 + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr, (long unsigned int)val);
    }
    else if(bank == 0x38)
    {
        printf("`RIU_W((24\'h103800>>1) + 7\'h%02lx, 2\'b11, 16\'h%04lx);\n", (long unsigned int)addr, (long unsigned int)val);
    }
}
#endif

void HAL_TSP_HwPatch(void)
{
    //For sram
    REG16_SET(&_RegCtrl->PktChkSizeFilein, TSP_HW_STANDBY_MODE);
    REG16_SET(&_RegCtrl3->CFG3_35, HW4_CFG35_PREVENT_SRAM_COLLISION);

    // @F_TODO check these setting with Stephen
    // TSP_HW_CFG4_WSTAT_CH_EN <--this is bit disable HW sync section buf id with section filter id
    REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_BYTE_ADDR_DMA|TSP_HW_CFG4_ALT_TS_SIZE|TSP_HW_CFG4_WSTAT_CH_EN);

    // Bad initial value of TSP_CTRL1
    // Suppose Standby mode for TSP should NOT be enabled.
    // Enabling TSP standby mode cause TSP section registers (SRAM in AEON) malfunction.
    // Disable it by SW at this stage.
    REG16_CLR(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_STANDBY);

    //enable PVR record to bypass header
    REG16_SET(&_RegCtrl->reg15b4, TSP_PVR_PID_BYPASS|TSP_PVR_PID_BYPASS2);
    REG16_SET(&_RegCtrl2->CFG_16, CFG_16_PID_BYPASS3_REC);
    REG16_SET(&_RegCtrl2->CFG_23, CFG_23_PID_BYPASS4_REC);

    REG16_SET(&_RegCtrl->reg15b8, TSP_REMOVE_DUP_AV_PKT);

    REG16_SET(&_RegCtrl->HW2_Config3, TSP_VQ2PINGPONG_EN | TSP_RM_PKT_DEMUX_PIPE /*| TSP_PVR1_ALIGN_EN*/);

    //Disable all live pathes block mechanism
    REG16_SET(&_RegCtrl->reg160C, TSP_DOUBLE_BUF_DESC/*| TSP_VQTX0_BLOCK_DIS|TSP_VQTX2_BLOCK_DIS|TSP_VQTX3_BLOCK_DIS*/);
    REG16_SET(&_RegCtrl->reg160E, TSP_RM_DMA_GLITCH);

    REG16_SET(&_RegCtrl3->CFG3_35, HW4_CFG35_PUSI_3BYTE_MODE); //Enable audio 3 byte mode

    REG16_SET(&_RegCtrl->TSP_Cfg5, TSP_SYSTIME_MODE);
    REG16_SET(&_RegCtrl->PVRConfig, TSP_MATCH_PID_LD);
    REG16_SET(&_RegCtrl3->CFG3_35, HW4_CFG35_BLK_AD_SCMBTIS_TSP);

    //Disable pvr1 & pvr2 block mechanism
    //DisableAV FIFO block mechanism for live path
    //REG32_SET(&_RegCtrl->PVR2_Config, TSP_PVR2_PVR_ALIGN_EN|TSP_PVR1_BLOCK_DIS|TSP_PVR2_BLOCK_DIS|TSP_V_BLOCK_DIS|TSP_A_BLOCK_DIS|TSP_AD_BLOCK_DIS); // by angie

    //fix load fw secure issue (dma_start = 1 , polling dma_done , dma_start = 0)
    REG16_SET(&_RegCtrl3->CFG3_16, CFG3_16_FIXED_DMA_RSTART_OTP_ONEWAY_LOAD_FW);

    //Internal Sync Patch
    REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_DATA_CHK_2T);

    //Fixed filein_192+timer_en+byte_time=0 Error
    REG16_SET(&_RegCtrl3->CFG3_34, CFG3_34_FIX_192_TIMER_0_EN);

    //Fixed filein_192 timestamp & LPCR ring back first issue
    //REG16_SET(&_RegCtrl6->CFG6_2A, FIXED_TIMESTAMP_RING_BACK_EN | FIXED_LPCR_RING_BACK_EN);

    //ECO bit for miu flush
    REG16_SET(&_RegCtrl6->CFG6_2A, FIXED_DMA_WADDR_NEXT_OVF | FIXED_VQ_MIU_REQ_FLUSH);

    //drop start code error
    REG16_SET(&_RegCtrl6->CFG6_2A, TSP_DROP_ERR_START_CODE | TSP_DROP_TEI_ERR_START_CODE);

    REG16_SET(&_RegCtrl6->CFG6_2A,TSP_FIQ_DMA_FLUSH_EN | TSP_FIND_LOSS_SYNC_PID_RVU);

    //FIQ ECO and block-reverse
    REG16_SET(&_RegCtrl6->CFG6_2B, TSP_ECO_FIQ_INPUT | TSP_ECO_TS_SYNC_OUT_DELAY | TSP_ECO_TS_SYNC_OUT_REVERSE_BLOCK);

    //serial mode config
    REG16_SET(&_RegCtrl->Hw_PVRCfg, TSP_SYNC_RISING_DETECT | TSP_VALID_FALLING_DETECT);
}

// ------------ initial config ------------
// Sync Byte: 0x47 , 0x48 .... , 0x4e
// Source id : 0 , 1 , ... , 7
// User can use "HAL_TSP_PktConverter_SetSyncByte()" to change Sync Byte configuration
// , and use "HAL_TSP_PktConverter_SetSrcId()" to change Source id configuration
void HAL_TSP_PktConverter_Init(void)
{
    MS_U8   u8Path,u8Id;
    MS_U8   u8SyncByte;

    for(u8Path = 0; u8Path < TSP_TSIF_NUM; ++u8Path)
    {
        HAL_TSP_PktConverter_ForceSync(u8Path,TRUE);//default: FALSE

        u8SyncByte = 0x47;
        for(u8Id = 0; u8Id < TSP_MERGESTREAM_NUM; ++u8Id,++u8SyncByte)
        {
            HAL_TSP_PktConverter_SetSyncByte(u8Path, u8Id, &u8SyncByte, TRUE);
            HAL_TSP_PktConverter_SetSrcId(u8Path, u8Id, &u8Id, TRUE);
        }
    }
}

void HAL_TSP_Reset(MS_BOOL bEn)
{
    //MS_U16              reg;

    if (bEn)
    {
    #if 0

        // WB DMA source won't be reset by SW_RST bit so we use HWPATCH to make it's source to default
        REG16_CLR(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_PVR_CMD_QUEUE_ENABLE);
        REG16_CLR(&_RegCtrl2->CFG_00, CFG_00_RST_CMDQ_FILEIN_TSIF1);

    #endif
        // reset CMDQ for tsif 0~3
        REG16_SET(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_CMDQ_RESET);
        REG16_SET(&_RegCtrl2->CFG_00, CFG_00_RST_CMDQ_FILEIN_TSIF1);
        REG16_SET(&_RegCtrl2->CFG_05, CFG_05_RST_CMDQ_FILEIN_TSIF2);
        REG16_SET(&_RegCtrl2->CFG_0A, CFG_0A_RST_CMDQ_FILEIN_TSIF3);
        REG16_SET(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_WB_DMA_RESET);
        REG16_CLR(&_RegCtrl->TSP_Ctrl, TSP_CTRL_SW_RST);
    }
    else
    {
        REG16_SET(&_RegCtrl->TSP_Ctrl, TSP_CTRL_SW_RST);

        REG16_CLR(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_WB_DMA_RESET);
        // set CMDQ for tsif 0~3
        REG16_CLR(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_CMDQ_RESET);
        REG16_CLR(&_RegCtrl2->CFG_00, CFG_00_RST_CMDQ_FILEIN_TSIF1);
        REG16_CLR(&_RegCtrl2->CFG_05, CFG_05_RST_CMDQ_FILEIN_TSIF2);
        REG16_CLR(&_RegCtrl2->CFG_0A, CFG_0A_RST_CMDQ_FILEIN_TSIF3);
    }
}


void HAL_TSP_Path_Reset(MS_U32 tsIf,MS_BOOL bEn)
{
    switch(tsIf)
    {
        case 0: if(bEn)
                {
                    REG16_SET(&_RegCtrl->reg160C,TSP_TIMESTAMP_RESET);
                    REG16_SET(&_RegCtrl2->CFG_10,CFG_10_RESET_PDFLT0);
                    REG16_SET(&_RegCtrl2->CFG_12,CFG_12_REG_REST_RBF0 | CFG_12_REG_REST_PDBF0);
                }
                else
                {
                    REG16_CLR(&_RegCtrl2->CFG_12,CFG_12_REG_REST_RBF0 | CFG_12_REG_REST_PDBF0);
                    REG16_CLR(&_RegCtrl2->CFG_10,CFG_10_RESET_PDFLT0);
                    REG16_CLR(&_RegCtrl->reg160C,TSP_TIMESTAMP_RESET);
                }
                break;
        case 1: if(bEn)
                {
                    REG16_SET(&_RegCtrl2->CFG_00,CFG_00_RST_TS_FIN1);
                    REG16_SET(&_RegCtrl2->CFG_10,CFG_10_RESET_PDFLT1);
                    REG16_SET(&_RegCtrl2->CFG_12,CFG_12_REG_REST_RBF1 | CFG_12_REG_REST_PDBF1);
                }
                else
                {
                    REG16_CLR(&_RegCtrl2->CFG_12,CFG_12_REG_REST_RBF1 | CFG_12_REG_REST_PDBF1);
                    REG16_CLR(&_RegCtrl2->CFG_10,CFG_10_RESET_PDFLT1);
                    REG16_CLR(&_RegCtrl2->CFG_00,CFG_00_RST_TS_FIN1);
                }
                break;
        case 2: if(bEn)
                {
                    REG16_SET(&_RegCtrl2->CFG_05,CFG_05_RST_TS_FIN2);
                    REG16_SET(&_RegCtrl2->CFG_10,CFG_10_RESET_PDFLT2);
                    REG16_SET(&_RegCtrl2->CFG_12,CFG_12_REG_REST_RBF2 | CFG_12_REG_REST_PDBF2);
                }
                else
                {
                    REG16_CLR(&_RegCtrl2->CFG_12,CFG_12_REG_REST_RBF2 | CFG_12_REG_REST_PDBF2);
                    REG16_CLR(&_RegCtrl2->CFG_10,CFG_10_RESET_PDFLT2);
                    REG16_CLR(&_RegCtrl2->CFG_05,CFG_05_RST_TS_FIN2);
                }
                break;
        default: break;
    }
}
/*****************/

MS_BOOL HAL_TSP_GetClockSetting(EN_TSP_HAL_CLK_TYPE eClkType, MS_U8 u8Index, ST_TSP_HAL_CLK_STATUS *pstClkStatus)
{
    switch(eClkType)
    {
        case E_TSP_HAL_TSP_CLK:
            pstClkStatus->bEnable = !(TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK) & REG_CLKGEN0_TSP_DISABLE);
            pstClkStatus->bInvert = !!(TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK) & REG_CLKGEN0_TSP_INVERT);
            pstClkStatus->u8ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK) & REG_CLKGEN0_TSP_CLK_MASK) >> REG_CLKGEN0_TSP_SRC_SHIFT;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

void HAL_TSP_Power(MS_BOOL bEn)
{
    if(bEn)
    {

        //disable MCM
        REG16_SET(&_RegCtrl3->CFG3_34, CFG3_34_TSP2MI_REQ_MCM_DISABLE); //TSP
        REG16_SET(&_RegCtrl3->CFG3_16, CFG3_16_MMFI1_REQ_MCM_DISABLE); // disable MMFI1 MCM, and never enable it, only for Curry

        // Enable TSP Clk
        TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK) & ~REG_CLKGEN0_TSP_CLK_MASK)
                | (REG_CLKGEN0_TSP_SRC_192MHZ << REG_CLKGEN0_TSP_SRC_SHIFT);
        // Enable STC1,2 Clk
        // STC1
        TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK) & ~REG_CLKGEN0_STC0_MASK)
                | (REG_CLKGEN0_STC_SRC_SYNTH << (REG_CLKGEN0_STC_SRC_SHIFT+REG_CLKGEN0_STC0_SHIFT));
        // STC2
        TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK) & ~REG_CLKGEN0_STC1_MASK)
                | (REG_CLKGEN0_STC_SRC_SYNTH << (REG_CLKGEN0_STC_SRC_SHIFT+REG_CLKGEN0_STC1_SHIFT));

        // Stamp
        TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK) & ~REG_CLKGEN0_STAMP_MASK);

        // Parser
        TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK) = ((TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK) & ~REG_CLKGEN0_PARSER_MASK) | REG_CLKGEN0_PARSER_192);

        // Enable TSIF => Disable TSIF
        // FixME Enable flowset would enable TSx clk so we don't enable TSx clk
        //TS0
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) & ~REG_CLKGEN0_TS_MASK);

        //TS1
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS1_SHIFT));

        //TS2
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS2_SHIFT));
        // TSP Boot clk sel
        TSP_TOP_REG(REG_TOP_TSP_BOOT_CLK_SEL) = (TSP_TOP_REG(REG_TOP_TSP_BOOT_CLK_SEL) & ~REG_TOP_TSP_BOOT_CLK_SEL_MASK) | REG_TOP_TSP_BOOT_CLK_SEL_TSP;

        // TSP SRAM sel
        TSP_MMFI_REG(REG_MMFI_TSP_SEL_SRAM) = TSP_MMFI_REG(REG_MMFI_TSP_SEL_SRAM) | REG_MMFI_TSP_SEL_SRAM_EN;

        REG16_SET(&_RegCtrl3->CFG3_35, HW4_CFG35_PREVENT_SRAM_COLLISION);
    }
    else
    {
        // Disable TSP Clk
        // [2016.03.10] Disable TSP clk for power problem. If AESDMA share clk with TSP. Need to do tsp init before using AESDMA.
        TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK),(REG_CLKGEN0_TSP_DISABLE << REG_CLKGEN0_TSP_SHIFT));

        // Disable STC Clk
        //STC0
        TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK),(REG_CLKGEN0_STC_DISABLE << REG_CLKGEN0_STC0_SHIFT));
        //STC1
        TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK),(REG_CLKGEN0_STC_DISABLE << REG_CLKGEN0_STC1_SHIFT));

        // Stamp
        TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK) = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK),(REG_CLKGEN0_STAMP_DISABLE << REG_CLKGEN0_STAMP_SHIFT));

        // Parser
        TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK) = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK),(REG_CLKGEN0_PARSER_DISABLE << REG_CLKGEN0_PARSER_SHIFT));

        // Disable TSIF clk
        //TS0
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS0_SHIFT));
        //TS1
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS1_SHIFT));
        //TS2
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS2_SHIFT));

        //enable MCM
        REG16_CLR(&_RegCtrl3->CFG3_34, CFG3_34_TSP2MI_REQ_MCM_DISABLE);//TSP
    }
}

void HAL_TSP_CPU(MS_BOOL bEn)
{
    if (bEn)
    {
        REG16_SET(&_RegCtrl->TSP_Ctrl, TSP_CTRL_CPU_EN);
    }
    else
    {
        REG16_CLR(&_RegCtrl->TSP_Ctrl, TSP_CTRL_CPU_EN);
    }
}
void HAL_TSP_ResetCPU(MS_BOOL bReset)
{
    // @NOTE TRUE for stop cpu clock
    if (bReset)
    {
        REG16_CLR(&_RegCtrl->TSP_Ctrl, TSP_CTRL_CPU_EN);
    }
    else
    {
        REG16_SET(&_RegCtrl->TSP_Ctrl, TSP_CTRL_CPU_EN);
    }
}


MS_BOOL HAL_TSP_LoadFW(MS_U32 u32FwPhyAddr, MS_U32 u32FwSize)
{

#define _TSP_QMEM_I_MASK            0xffffc000 //code: 0x2000, data: 0x1000, total: 0x3000
#define _TSP_QMEM_I_ADDR_HIT        0x00000000
#define _TSP_QMEM_I_ADDR_MISS       0xffffffff
#define _TSP_QMEM_D_MASK            0xffffc000
#define _TSP_QMEM_D_ADDR_HIT        0x00000000
#define _TSP_QMEM_D_ADDR_MISS       0xffffffff
#define _TSP_QMEM_SIZE              0x1000 // 16K bytes, 32bit aligment  //0x4000  this is 4 byte address

    REG32_W(&_RegCtrl->Cpu_Base, 0); // 16 bytes address unit

    MS_U32              u32DnldCtrl  = 0;
    MS_U32              u32DnldCtrl1 = 0;

    u32DnldCtrl  =      (u32FwPhyAddr >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT;
    u32DnldCtrl1 =      u32DnldCtrl >> 16;

    REG16_W(&_RegCtrl->Dnld_Ctrl_Addr, (MS_U16)(u32DnldCtrl & TSP_DNLD_ADDR_MASK)); // oneway register
    REG16_MSK_W(&_RegCtrl->Dnld_AddrH, TSP_DMA_RADDR_MSB_MASK, (MS_U16)u32DnldCtrl1);
    REG16_W(&_RegCtrl->Dnld_Ctrl_Size, _TSP_QMEM_SIZE);
    REG16_CLR(&_RegCtrl->TSP_Ctrl, TSP_CTRL_DNLD_START| TSP_CTRL_DNLD_DONE);
    REG16_SET(&_RegCtrl->TSP_Ctrl, TSP_CTRL_DNLD_START);

    //@TODO temprarily comment because of Secure Protect
    #if 1
    while (!(REG16_R(&_RegCtrl->TSP_Ctrl) & TSP_CTRL_DNLD_DONE))
    {
    }
    #endif


    REG16_CLR(&_RegCtrl->TSP_Ctrl, TSP_CTRL_DNLD_START| TSP_CTRL_DNLD_DONE);

    REG32_W(&_RegCtrl->Qmem_Imask, _TSP_QMEM_I_MASK);
    REG32_W(&_RegCtrl->Qmem_Ibase, _TSP_QMEM_I_ADDR_HIT);
    REG32_W(&_RegCtrl->Qmem_Dmask, _TSP_QMEM_D_MASK);
    REG32_W(&_RegCtrl->Qmem_Dbase, _TSP_QMEM_D_ADDR_HIT);

#undef _TSP_QMEM_I_MASK
#undef _TSP_QMEM_I_ADDR_HIT
#undef _TSP_QMEM_I_ADDR_MISS
#undef _TSP_QMEM_D_MASK
#undef _TSP_QMEM_D_ADDR_HIT
#undef _TSP_QMEM_D_ADDR_MISS
#undef _TSP_QMEM_SIZE

    return TRUE;
}

void    HAL_TSP_RestoreFltState(void)
{
    MS_U32              u32Size;
    TSP32 *             pRegEnd;
    TSP32 *             pReg;
    int                 i, j;

    for (i = 0; i < TSP_PIDFLT_NUM; i++)
    {
        TSP32_IdrW(&(_REGPid0->Flt[i]), _u32PidFltReg[i]);
        TSP32_IdrW(&(_REGPid1->Flt[i]), _u32PidDstReg[i]);
    }

    u32Size = ((MS_VIRT)&(((REG_SecFlt*)0)->_x50))/sizeof(TSP32);

    for (i = 0; i < TSP_SECFLT_NUM; i++)
    {
       pReg = (TSP32*)&(_REGSec->Flt[i]);
       pRegEnd = pReg + u32Size;
       j = 0;
       for ( ; pReg < pRegEnd; pReg++)
       {
           TSP32_IdrW(pReg, _u32SecReg[i*u32Size+j]);
           j++;
       }
    }

}

void HAL_TSP_PktBuf_Reset(MS_U32 pktDmxId, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(pktDmxId)
        {
            case 0:
                REG16_SET(&_RegCtrl2->CFG_12, CFG_12_REG_REST_PDBF0);
                break;
            case 1:
                REG16_SET(&_RegCtrl2->CFG_12, CFG_12_REG_REST_PDBF1);
                break;
            case 2:
                REG16_SET(&_RegCtrl2->CFG_12, CFG_12_REG_REST_PDBF2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(pktDmxId)
        {
            case 0:
                REG16_CLR(&_RegCtrl2->CFG_12, CFG_12_REG_REST_PDBF0);
                break;
            case 1:
                REG16_CLR(&_RegCtrl2->CFG_12, CFG_12_REG_REST_PDBF1);
                break;
            case 2:
                REG16_CLR(&_RegCtrl2->CFG_12, CFG_12_REG_REST_PDBF2);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_RecvBuf_Reset(MS_U32 pktDmxId, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(pktDmxId)
        {
            case 0:
                REG16_SET(&_RegCtrl2->CFG_12, CFG_12_REG_REST_RBF0);
                break;
            case 1:
                REG16_SET(&_RegCtrl2->CFG_12, CFG_12_REG_REST_RBF1);
                break;
            case 2:
                REG16_SET(&_RegCtrl2->CFG_12, CFG_12_REG_REST_RBF2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(pktDmxId)
        {
            case 0:
                REG16_CLR(&_RegCtrl2->CFG_12, CFG_12_REG_REST_RBF0);
                break;
            case 1:
                REG16_CLR(&_RegCtrl2->CFG_12, CFG_12_REG_REST_RBF1);
                break;
            case 2:
                REG16_CLR(&_RegCtrl2->CFG_12, CFG_12_REG_REST_RBF2);
                break;
            default:
                break;
        }
    }
}

MS_BOOL HAL_TSP_SetTSIF(MS_U16 u16TSIF, TSP_TSIF_CFG u16Cfg, MS_BOOL bFileIn)
{
    if(bFileIn)
    {
        HAL_TSP_TSIF_FileEn((FILEENG_SEQ)u16TSIF, TRUE); // this returns true only we don't check the return value
        HAL_TSP_TSIF_LiveEn(u16TSIF, FALSE);             // this returns true only we don't check the return value
    }
    else
    {
        HAL_TSP_TSIF_FileEn((FILEENG_SEQ)u16TSIF, FALSE);
        HAL_TSP_TSIF_LiveEn(u16TSIF, TRUE);
    }

    if(bFileIn != TRUE)
    {
        HAL_TSP_TSIF_BitSwap(u16TSIF, ((u16Cfg&E_TSP_TSIF_CFG_BITSWAP)?TRUE:FALSE));
        HAL_TSP_TSIF_ExtSync(u16TSIF, ((u16Cfg&E_TSP_TSIF_CFG_EXTSYNC)?TRUE:FALSE));
        HAL_TSP_TSIF_Parl   (u16TSIF, ((u16Cfg&E_TSP_TSIF_CFG_PARA   )?TRUE:FALSE));
        HAL_TSP_TSIF_3Wire  (u16TSIF, ((u16Cfg&E_TSP_TSIF_CFG_3WIRE  )?TRUE:FALSE));
    }

    return TRUE;
}

MS_BOOL HAL_TSP_TSIF_LiveEn(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(tsIf)
        {
            case 0:
                REG16_SET(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TSIF0_ENABLE);
                break;
            case 1:
                REG16_SET(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TSIF1_ENABLE);
                break;
            case 2:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_TS_IF2_EN);
                break;
            default:
                return FALSE;
        }
    }
    else
    {
        switch(tsIf)
        {
            case 0:
                REG16_CLR(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TSIF0_ENABLE);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TSIF1_ENABLE);
                break;
            case 2:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_TS_IF2_EN);
                break;
            default:
                return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL HAL_TSP_TSIF_SelPad(MS_U32 tsIf, TSP_TS_PAD eTSPad) // @FIXME modify this parameter to enum plz
{
    MS_U32 clk_src = REG_CLKGEN0_TS_SRC_EXT0;
    MS_U32 pad_src = REG_TOP_TS_SRC_EXT0;

    //@NOTE
    //EX3~6 are serial mode and ts2_padmax_mode must be 2 or 3.


    switch (eTSPad)
    {
        default:
        case E_TSP_TS_PAD_EXT0:
            clk_src = REG_CLKGEN0_TS_SRC_EXT0;
            pad_src = REG_TOP_TS_SRC_EXT0;
            break;
        case E_TSP_TS_PAD_EXT1:
            clk_src = REG_CLKGEN0_TS_SRC_EXT1;
            pad_src = REG_TOP_TS_SRC_EXT1;
            break;
        case E_TSP_TS_PAD_INTER0:
            clk_src = REG_CLKGEN0_TS_SRC_EXT0;
            pad_src = REG_TOP_TS_SRC_EXT0;
            printf("[%s][%d]Warning KANO not support Internal Demod\n",__FUNCTION__,__LINE__);
            break;
    }
    //@FIXME use enum instead of constant
    switch (tsIf)
    {
        case 0:
            TSP_TOP_REG(REG_TOP_TS0_MUX)          =  (TSP_TOP_REG(REG_TOP_TS0_MUX) & ~REG_TOP_TS_SRC_MASK) | (pad_src<<REG_TOP_TS0_SHIFT);
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK)  = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) & ~REG_CLKGEN0_TS_MASK) | (clk_src<<(REG_CLKGEN0_TS0_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case 1:
            TSP_TOP_REG(REG_TOP_TS1_MUX)          =  (TSP_TOP_REG(REG_TOP_TS1_MUX) & ~(REG_TOP_TS_SRC_MASK<<REG_TOP_TS1_SHIFT))
                                                        | (pad_src<<REG_TOP_TS1_SHIFT);
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK)  = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS1_SHIFT))
                                                        | (clk_src<<(REG_CLKGEN0_TS1_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case 2:
            TSP_TOP_REG(REG_TOP_TS2_MUX)          =  (TSP_TOP_REG(REG_TOP_TS2_MUX) & ~(REG_TOP_TS_SRC_MASK<<REG_TOP_TS2_SHIFT))
                                                        | (pad_src<<REG_TOP_TS2_SHIFT);
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK)  = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS2_SHIFT))
                                                        | (clk_src<<(REG_CLKGEN0_TS2_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

void HAL_TSO_SetTSOOutMUX(MS_BOOL bSet)
{
    return ;
}

MS_BOOL HAL_TSP_TsOutPadCfg(TSP_TS_PAD eOutPad, TSP_TS_PAD_MUX_MODE eOutPadMode, TSP_TS_PAD eInPad, TSP_TS_PAD_MUX_MODE eInPadMode, MS_BOOL bEnable)
{
    return FALSE;
}

static MS_BOOL _TSP_Hal_TSPAD2RelatedReg_Mapping(TSP_TS_PAD eTSPad, MS_U32* pu32PADSrc, MS_U32* pu32CLKSrc)
{
    switch (eTSPad)
    {
        case E_TSP_TS_PAD_EXT0:
            *pu32CLKSrc = REG_CLKGEN0_TS_SRC_EXT0;
            *pu32PADSrc = REG_TOP_TS_SRC_EXT0;
            break;
        case E_TSP_TS_PAD_EXT1:
            *pu32CLKSrc = REG_CLKGEN0_TS_SRC_EXT1;
            *pu32PADSrc = REG_TOP_TS_SRC_EXT1;
            break;
        case E_TSP_TS_PAD_INTER0:
            *pu32CLKSrc = REG_CLKGEN0_TS_SRC_DMD0;
            *pu32PADSrc = REG_TOP_TS_SRC_DMD0;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_TSO_TSIF_SelPad(MS_U32 u32TSOEng, TSP_TS_PAD eTSPad)
{
    MS_U32 u32CLKSrc = REG_CLKGEN0_TS_SRC_EXT0;
    MS_U32 u32PADSrc = REG_TOP_TS_SRC_EXT0;
    _TSP_Hal_TSPAD2RelatedReg_Mapping(eTSPad, &u32PADSrc, &u32CLKSrc);

    switch(u32TSOEng)
    {
        case 0:
            TSP_TOP_REG(REG_TOP_TSO0_MUX) = (TSP_TOP_REG(REG_TOP_TSO0_MUX) & ~REG_TOP_TS_SRC_MASK) | u32PADSrc;
            TSP_CLKGEN0_REG(REG_CLKGEN0_TSO0_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSO0_CLK) & ~REG_CLKGEN0_TS_MASK)
                | (u32CLKSrc << (REG_CLKGEN0_TSO0_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            return TRUE;

        default:
            return FALSE;
    }
}


MS_BOOL HAL_TSP_TSIF_SelPad_ClkInv(MS_U32 tsIf , MS_BOOL bClkInv)
{
    if (bClkInv)
    {
        switch (tsIf)
        {
            case 0:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) |= ((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS0_SHIFT));
                break;
            case 1:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) |= ((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS1_SHIFT));
                break;
            case 2:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) |= ((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS2_SHIFT));
                break;
            default:
                return FALSE;
        }
    }
    else
    {
        switch (tsIf)
        {
            case 0:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) &= ~((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS0_SHIFT));
                break;
            case 1:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) &= ~((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS1_SHIFT));
                break;
            case 2:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) &= ~((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS2_SHIFT));
                break;
            default:
                return FALSE;
        }
    }
    return TRUE;
}

MS_BOOL HAL_TSP_TSIF_SelPad_ClkDis(MS_U32 tsIf , MS_BOOL bClkDis)
{
    if (bClkDis)
    {
        switch (tsIf)
        {
            case 0:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) |= ((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS0_SHIFT));
                break;
            case 1:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) |= ((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS1_SHIFT));
                break;
            case 2:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) |= ((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS2_SHIFT));
                break;
            default:
                return FALSE;
        }
    }
    else
    {
        switch (tsIf)
        {
            case 0:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) &= ~((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS0_SHIFT));
                break;
            case 1:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) &= ~((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS1_SHIFT));
                break;
            case 2:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) &= ~((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS2_SHIFT));
                break;
            default:
                return FALSE;
        }
    }
    return TRUE;
}

// @NOTE tsif0 and tsif2 can do filein and livein simulatenously,so tsif0 tsif2's output are both live TS Data
MS_BOOL HAL_TSP_TSIF_FileEn(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_SET(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_PVR_CMD_QUEUE_ENABLE);    // for wishbone DMA (load firmware or playback)
                REG16_SET(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_DATA_PORT_SEL);        //Tsif0 output is live TS
                REG16_SET(&_RegCtrl->TSP_Ctrl, TSP_CTRL_TSFILE_EN);                 //filein enable
                break;
            case E_FILEENG_TSIF1:
                REG16_SET(&_RegCtrl2->CFG_01, CFG_01_TSP_FILE_SEGMENT1);            // for wishbone DMA (load firmware or playback) we don't use this fileEng for FW
                REG16_SET(&_RegCtrl2->CFG_00, CFG_00_TSP_FILE_SEGMENT_TSIF1);
                REG16_SET(&_RegCtrl2->CFG_01, CFG_01_TS_DATA_PORT_SEL1);
                REG16_SET(&_RegCtrl2->CFG_00, CFG_00_TSP_FILE_IN_TSIF1_EN);         //filein enable
                break;
            case E_FILEENG_TSIF2:
                REG16_SET(&_RegCtrl2->CFG_06, CFG_06_TSP_FILE_SEGMENT2);
                REG16_SET(&_RegCtrl2->CFG_05, CFG_05_TSP_FILE_SEGMENT_TSIF2);
                REG16_SET(&_RegCtrl2->CFG_06, CFG_06_TS_DATA_PORT_SEL2);
                REG16_SET(&_RegCtrl2->CFG_05, CFG_05_TSP_FILEIN_TSIF2);
                break;
            default:
                return FALSE;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_PVR_CMD_QUEUE_ENABLE);
                REG16_CLR(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_DATA_PORT_SEL);
                REG16_CLR(&_RegCtrl->TSP_Ctrl, TSP_CTRL_TSFILE_EN);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl2->CFG_01, CFG_01_TSP_FILE_SEGMENT1);
                REG16_CLR(&_RegCtrl2->CFG_00, CFG_00_TSP_FILE_SEGMENT_TSIF1);
                REG16_CLR(&_RegCtrl2->CFG_01, CFG_01_TS_DATA_PORT_SEL1);
                REG16_CLR(&_RegCtrl2->CFG_00, CFG_00_TSP_FILE_IN_TSIF1_EN);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl2->CFG_06, CFG_06_TSP_FILE_SEGMENT2);
                REG16_CLR(&_RegCtrl2->CFG_05, CFG_05_TSP_FILE_SEGMENT_TSIF2);
                REG16_CLR(&_RegCtrl2->CFG_05, CFG_05_TSP_FILEIN_TSIF2);
                REG16_CLR(&_RegCtrl2->CFG_06, CFG_06_TS_DATA_PORT_SEL2);
                break;
            default:
                return FALSE;
        }
    }

    return TRUE;
}

void HAL_TSP_TSIF_BitSwap(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(tsIf)
        {
            case 0:
                REG16_SET(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TS_DATA0_SWAP);
                break;
            case 1:
                REG16_SET(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TS_DATA1_SWAP);
                break;
            case 2:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_TS_DATA2_SWAP);
                break;
            default:
                return;
        }
    }
    else
    {
        switch(tsIf)
        {
            case 0:
                REG16_CLR(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TS_DATA0_SWAP);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->Hw_PVRCfg, TSP_HW_CFG4_TS_DATA1_SWAP);
                break;
            case 2:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_TS_DATA2_SWAP);
                break;
            default:
                return;
        }
    }
}

void HAL_TSP_TSIF_ExtSync(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(bEnable)
    {
        printf("External Sync\n");

        switch(tsIf)
        {
            case 0:
                REG16_SET(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_TSIF0_EXTSYNC);
                break;
            case 1:
                REG16_SET(&_RegCtrl->Hw_Config2, TSP_HW_CFG2_TSIF1_EXTSYNC);
                break;
            case 2:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_EXT_SYNC_SEL2);
                break;
            default:
                return;
        }
    }
    else
    {
        printf("Internal Sync\n");

        switch(tsIf)
        {
            case 0:
                REG16_CLR(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_TSIF0_EXTSYNC);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->Hw_Config2, TSP_HW_CFG2_TSIF1_EXTSYNC);
                break;
            case 2:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_EXT_SYNC_SEL2);
                break;
            default:
                return;
        }
    }
}

//void HAL_TSP_TSIF_Full_Block(MS_U32 tsIf, MS_BOOL bEnable)
void HAL_TSP_Filein_Bypass(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(eFileEng)
        {
            case 0:
                REG16_SET(&(_RegCtrl->TSP_Ctrl2), TSP_AV_DIRECT_STOP);
                break;
            case 1:
                REG16_SET(&(_RegCtrl->TSP_Ctrl2), TSP_AV_DIRECT_STOP1);
                break;
            case 2:
                REG16_SET(&(_RegCtrl->TSP_Ctrl2), TSP_AV_DIRECT_STOP2);
                break;
            default:
                return;
        }

        REG16_MSK_W(&_RegCtrl3->CFG3_10,CFG3_10_PS_MODE_SRC_MASK,(eFileEng << CFG3_10_PS_MODE_SRC_SHIFT));
    }
    else
    {
        switch(eFileEng)
        {
            case 0:
                REG16_CLR(&(_RegCtrl->TSP_Ctrl2), TSP_AV_DIRECT_STOP);
                break;
            case 1:
                REG16_CLR(&(_RegCtrl->TSP_Ctrl2), TSP_AV_DIRECT_STOP1);
                break;
            case 2:
                REG16_CLR(&(_RegCtrl->TSP_Ctrl2), TSP_AV_DIRECT_STOP2);
                break;
            default:
                return;
        }

    }
}

void HAL_TSP_TSIF_Parl(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(tsIf)
        {
            case 0:
                REG16_SET(&(_RegCtrl->Hw_Config0), TSP_HW_CFG0_TSIF0_PARL);
                break;
            case 1:
                REG16_SET(&(_RegCtrl->Hw_Config2), TSP_HW_CFG2_TSIF1_PARL);
                break;
            case 2:
                REG32_SET(&(_RegCtrl->PVR2_Config), TSP_P_SEL2);
                break;
            default:
                return;
        }
    }
    else
    {
        switch(tsIf)
        {
            case 0:
                REG16_CLR(&(_RegCtrl->Hw_Config0), TSP_HW_CFG0_TSIF0_PARL);
                break;
            case 1:
                REG16_CLR(&(_RegCtrl->Hw_Config2), TSP_HW_CFG2_TSIF1_PARL);
                break;
            case 2:
                REG32_CLR(&(_RegCtrl->PVR2_Config), TSP_P_SEL2);
                break;
            default:
                return;
        }
    }
}

void HAL_TSP_PAD_3Wire(MS_U32 u32Pad, MS_BOOL bEnable)
{
#if 0 //@NOTE: Kano do NOT need to set
    if(bEnable)
    {
        switch(u32Pad)
        {
            case 0:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) | REG_TOP_TSP_TS0_3WIRE_EN;
                break;
            case 1:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) | REG_TOP_TSP_TS1_3WIRE_EN;
                break;
            case 2:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1)| REG_TOP_TSP_TS2_3WIRE_EN;
                break;
            case 3:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1) | REG_TOP_TSP_TS3_3WIRE_EN;
                break;

            default:
                return;
        }
    }
    else
    {
        switch(u32Pad)
        {
            case 0:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) & ~REG_TOP_TSP_TS0_3WIRE_EN;
                break;
            case 1:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE) & ~REG_TOP_TSP_TS1_3WIRE_EN;
                break;
            case 2:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1) & ~REG_TOP_TSP_TS2_3WIRE_EN;
                break;
            case 3:
                TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1) = TSP_TOP_REG(REG_TOP_TSP_3WIRE_MODE1) & ~REG_TOP_TSP_TS3_3WIRE_EN;
                break;
            default:
                return;
        }
    }
#endif
}

void HAL_TSP_TSIF_3Wire(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(tsIf)
        {
            case 0:
                REG16_SET(&(_RegCtrl3->CFG3_37), HW4_CFG37_3WIRE_SERIAL_MODE_TS0);
                break;
            case 1:
                REG16_SET(&(_RegCtrl3->CFG3_37), HW4_CFG37_3WIRE_SERIAL_MODE_TS1);
                break;
            case 2:
                REG16_SET(&(_RegCtrl3->CFG3_37), HW4_CFG37_3WIRE_SERIAL_MODE_TS2);
                break;
            default:
                return;
        }
    }
    else
    {
        switch(tsIf)
        {
            case 0:
                REG16_CLR(&(_RegCtrl3->CFG3_37), HW4_CFG37_3WIRE_SERIAL_MODE_TS0);
                break;
            case 1:
                REG16_CLR(&(_RegCtrl3->CFG3_37), HW4_CFG37_3WIRE_SERIAL_MODE_TS1);
                break;
            case 2:
                REG16_CLR(&(_RegCtrl3->CFG3_37), HW4_CFG37_3WIRE_SERIAL_MODE_TS2);
                break;
            default:
                return;
        }
    }

}

void HAL_TSP_PktDmx_CCDrop(MS_U32 pktDmxId, MS_BOOL bEn)
{
    if(bEn)
    {
        REG16_SET(&_RegCtrl3->CFG3_0C, ((1 << pktDmxId) << CFG3_0C_PKTDMX_CC_DROP_SHIFT) & CFG3_0C_PKTDMX_CC_DROP_MSAK);
    }
    else
    {
        REG16_CLR(&_RegCtrl3->CFG3_0C, ((1 << pktDmxId) << CFG3_0C_PKTDMX_CC_DROP_SHIFT) & CFG3_0C_PKTDMX_CC_DROP_MSAK);
    }
}

void HAL_TSP_ReDirect_File(MS_U32 reDir, MS_U32 tsIf, MS_BOOL bEn)
{
   //@NOTE  Not support in KANO(K7)
   #if 0
    MS_U16 u16Src = 0;

    if((reDir > 0) || (tsIf > 1))
        return;

    if(bEn)
    {
        u16Src = (MS_U16)(tsIf + 1) << CFG_01_PDFLT2_FILE_SRC_SHIFT;
    }

    REG16_MSK_W(&_RegCtrl2->CFG_01, CFG_01_PDFLT2_FILE_SRC, u16Src);
   #endif
}

void HAL_TSP_TRACE_MARK_En(MS_U32 u32Tsif,TSP_DST_SEQ eFltType,MS_BOOL bEn)
{
    if(bEn)
    {
        switch(u32Tsif)
        {
            case 0:
                switch (eFltType)
                {
                    case E_TSP_DST_FIFO_VIDEO:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_V_EN);
                        break;
                    case E_TSP_DST_FIFO_VIDEO3D:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_V3D_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_A_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO2:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_AB_EN);
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                switch (eFltType)
                {
                    case E_TSP_DST_FIFO_VIDEO:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_V_EN);
                        break;
                    case E_TSP_DST_FIFO_VIDEO3D:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_V3D_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_A_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO2:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_AB_EN);
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                switch (eFltType)
                {
                    case E_TSP_DST_FIFO_VIDEO:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_V_EN);
                        break;
                    case E_TSP_DST_FIFO_VIDEO3D:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_V3D_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_A_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO2:
                        REG16_SET(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_AB_EN);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32Tsif)
        {
            case 0:
                switch (eFltType)
                {
                    case E_TSP_DST_FIFO_VIDEO:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_V_EN);
                        break;
                    case E_TSP_DST_FIFO_VIDEO3D:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_V3D_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_A_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO2:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX0_TRACE_MARK_AB_EN);
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                switch (eFltType)
                {
                    case E_TSP_DST_FIFO_VIDEO:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_V_EN);
                        break;
                    case E_TSP_DST_FIFO_VIDEO3D:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_V3D_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_A_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO2:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX1_TRACE_MARK_AB_EN);
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                switch (eFltType)
                {
                    case E_TSP_DST_FIFO_VIDEO:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_V_EN);
                        break;
                    case E_TSP_DST_FIFO_VIDEO3D:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_V3D_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_A_EN);
                        break;
                    case E_TSP_DST_FIFO_AUDIO2:
                        REG16_CLR(&_RegCtrl3->CFG3_2A, CFG3_2A_PKTDMX2_TRACE_MARK_AB_EN);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_BD_AUD_En(MS_U32 u32BD,MS_BOOL bEn)
{

    if(bEn == TRUE)
    {
        switch(u32BD)
        {
            case 0:
                REG16_SET(&_RegCtrl->reg15b4, TSP_BD_AUD_EN);
                break;
            case 1:
                REG16_SET(&_RegCtrl->reg15b4, TSP_BD2_AUD_EN);
                break;
            default:
                printf("Not support !!\n");
                break;
        }
    }
    else
    {
        switch(u32BD)
        {
            case 0:
                REG16_CLR(&_RegCtrl->reg15b4, TSP_BD_AUD_EN);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->reg15b4, TSP_BD2_AUD_EN);
                break;
            default:
                printf("Not support !!\n");
                break;

        }
    }

}

void HAL_TSP_Filein_PktSize(FILEENG_SEQ eFileEng, MS_U32 u32PktSize)
{
    MS_U32 u32PktLen = u32PktSize;

    if(u32PktSize == 130)
    {
        u32PktLen = 188;
    }
    else if(u32PktSize == 134)
    {
        u32PktLen = 192;
    }

    if(u32PktSize == 130 || u32PktSize == 134) //RVU
    {
        MS_U16 u16value = 0;

        if(u32PktSize == 134)
        {
            switch(eFileEng)
            {
                case E_FILEENG_TSIF0:
                    u16value = HW4_CFG36_PKT130_TIMESTAMP_EN0;
                    break;
                case E_FILEENG_TSIF1:
                    u16value = HW4_CFG36_PKT130_TIMESTAMP_EN1;
                    break;
                case E_FILEENG_TSIF2:
                    u16value = CFG4_54_RVU_TIMESTAMP_EN2;
                    break;
                default:
                    printf("Not support !!\n");
                    break;
            }
        }

        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                u16value |= HW4_CFG36_PKT130_PSI_EN0 | HW4_CFG36_PKT130_EN0;
                REG16_SET(&_RegCtrl3->CFG3_36, u16value);
                break;
            case E_FILEENG_TSIF1:
                u16value |= HW4_CFG36_PKT130_PSI_EN1 | HW4_CFG36_PKT130_EN1;
                REG16_SET(&_RegCtrl3->CFG3_36, u16value);
                break;
            case E_FILEENG_TSIF2:
                u16value |= CFG4_54_RVU_PSI_EN2 | CFG4_54_RVU_EN2;
                REG16_SET(&_RegCtrl4->CFG4_54, u16value);
                break;
            default:
                return;
        }

    }

    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_ALT_TS_SIZE);
            REG16_W(&_RegCtrl->PktChkSizeFilein, (REG16_R(&_RegCtrl->PktChkSizeFilein) & ~TSP_PKT_SIZE_MASK) | (TSP_PKT_SIZE_MASK & (u32PktLen-1)));
            break;
        case E_FILEENG_TSIF1:
            REG16_W(&_RegCtrl2->CFG_02, (REG16_R(&_RegCtrl2->CFG_02) & ~CFG_02_PKT_CHK_SIZE_FIN1) | (CFG_02_PKT_CHK_SIZE_FIN1 & (u32PktLen-1)));
            break;
        case E_FILEENG_TSIF2:
            REG16_W(&_RegCtrl2->CFG_07, (REG16_R(&_RegCtrl2->CFG_07) & ~CFG_07_PKT_CHK_SIZE_FILEIN2) | (CFG_07_PKT_CHK_SIZE_FILEIN2 & (u32PktLen-1)));
            break;
        default:
            break;
    }
}

void HAL_TSP_Filein_Addr(FILEENG_SEQ eFileEng, MS_U32 addr)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG32_W(&_RegCtrl->TsDma_Addr, addr);
            break;
        case E_FILEENG_TSIF1:
            REG32_W(&_RegCtrl2->CFG_30_31, addr);
            break;
        case E_FILEENG_TSIF2:
            REG32_W(&_RegCtrl2->CFG_35_36, addr);
            break;
        default:
            break;
    }
}

void HAL_TSP_Filein_Size(FILEENG_SEQ eFileEng, MS_U32 size)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG32_W(&_RegCtrl->TsDma_Size, size);
            break;
        case E_FILEENG_TSIF1:
            REG32_W(&_RegCtrl2->CFG_32_33, size);
            break;
        case E_FILEENG_TSIF2:
            REG32_W(&_RegCtrl2->CFG_37_38, size);
            break;
        default:
            break;
    }
}

void HAL_TSP_Filein_Start(FILEENG_SEQ eFileEng)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_SET(&_RegCtrl->TsDma_Ctrl, TSP_TSDMA_CTRL_START);
            break;
        case E_FILEENG_TSIF1:
            REG16_SET(&_RegCtrl2->CFG_34, CFG_34_REG_TSP_FILEIN_CTRL_TSIF1_START);
            break;
        case E_FILEENG_TSIF2:
            REG16_SET(&_RegCtrl2->CFG_39, CFG_39_FILEIN_CTRL_TSIF2_START);
            break;
        default:
            break;
    }
}

MS_BOOL HAL_TSP_File_Pause(FILEENG_SEQ eFileEng)
{
    switch (eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_SET(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_FILEIN_PAUSE);
            return TRUE;
        case E_FILEENG_TSIF1:
            REG16_SET(&_RegCtrl3->CFG3_21, CFG3_21_TSIF1_FILE_PAUSE);
            return TRUE;
        case E_FILEENG_TSIF2:
            REG16_SET(&_RegCtrl3->CFG3_21, CFG3_21_TSIF2_FILE_PAUSE);
            return TRUE;
        default:
            return FALSE;
    }
}

MS_BOOL HAL_TSP_File_Resume(FILEENG_SEQ eFileEng)
{
    switch (eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_CLR(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_FILEIN_PAUSE);
            return TRUE;
        case E_FILEENG_TSIF1:
            REG16_CLR(&_RegCtrl3->CFG3_21, CFG3_21_TSIF1_FILE_PAUSE);
            return TRUE;
        case E_FILEENG_TSIF2:
            REG16_CLR(&_RegCtrl3->CFG3_21, CFG3_21_TSIF2_FILE_PAUSE);
            return TRUE;
        default:
            return FALSE;
    }
}

void HAL_TSP_Filein_Init_Trust_Start(FILEENG_SEQ eFileEng)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_SET(&_RegCtrl->TsDma_Ctrl, (TSP_TSDMA_INIT_TRUST | TSP_TSDMA_CTRL_START));
            break;
        case E_FILEENG_TSIF1:
            REG16_SET(&_RegCtrl2->CFG_34, (CFG_34_REG_TSP_FILEIN_INIT_TRUST_TSIF1 | CFG_34_REG_TSP_FILEIN_CTRL_TSIF1_START));
            break;
        case E_FILEENG_TSIF2:
            REG16_SET(&_RegCtrl2->CFG_39, (CFG_39_FILEIN_INIT_TRUST_TSIF2 | CFG_39_FILEIN_CTRL_TSIF2_START));
            break;
        default:
            break;
    }
}

void HAL_TSP_Filein_Abort(FILEENG_SEQ eFileEng, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_SET(&_RegCtrl2->CFG_04, CFG_04_TSP_FILEIN_ABORT_ECO_TSIF0);
                break;
            case E_FILEENG_TSIF1:
                REG16_SET(&_RegCtrl2->CFG_04, CFG_04_TSP_FILEIN_ABORT_ECO_TSIF1);
                break;
            case E_FILEENG_TSIF2:
                REG16_SET(&_RegCtrl2->CFG_04, CFG_04_TSP_FILEIN_ABORT_ECO_TSIF2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl2->CFG_04, CFG_04_TSP_FILEIN_ABORT_ECO_TSIF0);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl2->CFG_04, CFG_04_TSP_FILEIN_ABORT_ECO_TSIF1);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl2->CFG_04, CFG_04_TSP_FILEIN_ABORT_ECO_TSIF2);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_Filein_CmdQRst(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_SET(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_CMDQ_RESET);
                break;
            case E_FILEENG_TSIF1:
                REG16_SET(&_RegCtrl2->CFG_00, CFG_00_RST_CMDQ_FILEIN_TSIF1);
                break;
            case E_FILEENG_TSIF2:
                REG16_SET(&_RegCtrl2->CFG_05, CFG_05_RST_CMDQ_FILEIN_TSIF2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_CMDQ_RESET);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl2->CFG_00, CFG_00_RST_CMDQ_FILEIN_TSIF1);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl2->CFG_05, CFG_05_RST_CMDQ_FILEIN_TSIF2);
                break;
            default:
                break;
        }
    }
}

MS_U32 HAL_TSP_Filein_CmdQSlot(FILEENG_SEQ eFileEng)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            return (TSP_CMDQ_SIZE - ((REG16_R(&_RegCtrl->TsDma_mdQ) & TSP_CMDQ_CNT_MASK) >> TSP_CMDQ_CNT_SHFT));
        case E_FILEENG_TSIF1:
            return (CFG_3F_REG_TSIF1_CMD_QUEUE_SIZE - (REG16_R(&_RegCtrl2->CFG_3F) & CFG_3F_REG_TSIF1_CMD_QUEUE_WR_CNT));
        case E_FILEENG_TSIF2:
            return (CFG_40_REG_TSIF2_CMD_QUEUE_SIZE - (REG16_R(&_RegCtrl2->CFG_40) & CFG_40_REG_TSIF2_CMD_QUEUE_WR_CNT));
        default:
            return 0;
    }
}

MS_U32 HAL_TSP_Filein_CmdQCnt(FILEENG_SEQ eFileEng)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            return ((REG16_R(&_RegCtrl->TsDma_mdQ) & TSP_CMDQ_CNT_MASK) >> TSP_CMDQ_CNT_SHFT);
        case E_FILEENG_TSIF1:
            return (REG16_R(&_RegCtrl2->CFG_3F) & CFG_3F_REG_TSIF1_CMD_QUEUE_WR_CNT);
        case E_FILEENG_TSIF2:
            return (REG16_R(&_RegCtrl2->CFG_40) & CFG_40_REG_TSIF2_CMD_QUEUE_WR_CNT);
        default:
            return 0;
    }
}

MS_U32 HAL_TSP_Filein_CmdQLv(FILEENG_SEQ eFileEng)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            return ((REG16_R(&_RegCtrl->TsDma_Ctrl) & TSP_CMDQ_WR_LEVEL_MASK) >> TSP_CMDQ_WR_LEVEL_SHFT);
        case E_FILEENG_TSIF1:
            return ((REG16_R(&_RegCtrl2->CFG_3F) & CFG_3F_REG_TSIF1_CMD_QUEUE_WR_LEVEL) >> CFG_3F_REG_TSIF1_CMD_QUEUE_LEVEL_SHIFT);
        case E_FILEENG_TSIF2:
            return ((REG16_R(&_RegCtrl2->CFG_40) & CFG_40_REG_TSIF2_CMD_QUEUE_WR_LEVEL) >> CFG_3F_REG_TSIF1_CMD_QUEUE_LEVEL_SHIFT);
        default:
            return 0;
    }
}

void HAL_TSP_Filein_ByteDelay(FILEENG_SEQ eFileEng, MS_U32 delay, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG32_W(&_RegCtrl->TsFileIn_Timer, delay & TSP_FILE_TIMER_MASK);
                REG16_SET(&_RegCtrl->reg15b4, TSP_FILEIN_BYTETIMER_ENABLE);
                break;
            case E_FILEENG_TSIF1:
                REG16_W(&_RegCtrl2->CFG_03, delay & CFG_03_TSP_FILE_TIMER1);
                REG16_SET(&_RegCtrl2->CFG_01, CFG_01_TIMER_EN1);
                break;
            case E_FILEENG_TSIF2:
                REG16_W(&_RegCtrl2->CFG_08, delay & CFG_08_TSP_FILE_TIMER2);
                REG16_SET(&_RegCtrl2->CFG_06, CFG_06_TSP_TIMER_EN2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl->reg15b4, TSP_FILEIN_BYTETIMER_ENABLE);
                REG32_W(&_RegCtrl->TsFileIn_Timer, 0x0000);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl2->CFG_01, CFG_01_TIMER_EN1);
                REG16_W(&_RegCtrl2->CFG_03, 0x0000);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl2->CFG_06, CFG_06_TSP_TIMER_EN2);
                REG16_W(&_RegCtrl2->CFG_08, 0x0000);
                break;
            default:
                break;
        }
    }
}

MS_U32 HAL_TSP_Filein_Status(FILEENG_SEQ eFileEng)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            return !(REG16_R(&_RegCtrl->TsDma_Ctrl) & TSP_TSDMA_FILEIN_DONE);
        case E_FILEENG_TSIF1:
            return !(REG16_R(&_RegCtrl2->CFG_34) & CFG_34_REG_TSP_FILEIN_CTRL_TSIF1_DONE);
        case E_FILEENG_TSIF2:
            return !(REG16_R(&_RegCtrl2->CFG_39) & CFG_39_FILEIN_CTRL_TSIF2_DONE);
        default:
            return 0;
    }
}

// Only used by [HW test code]
/*
MS_BOOL HAL_TSP_Filein_Done_Status(FILEENG_SEQ eFileEng)
{
    return !HAL_TSP_Filein_Status(eFileEng);
}
*/

//To do : only tsif0 has pause functionality in Kaiser
TSP_HAL_FileState HAL_TSP_Filein_GetState(FILEENG_SEQ eFileEng)
{
    TSP_HAL_FileState       state = E_TSP_HAL_FILE_STATE_INVALID;
    MS_U32                  u32Status = HAL_TSP_Filein_Status(eFileEng);

    // @FIXME in kaiser u01 only tsif0 file eng has pause function
    switch (eFileEng)
    {
        case E_FILEENG_TSIF0:
            if (REG16_R(&_RegCtrl->TSP_Ctrl1) & TSP_CTRL1_FILEIN_PAUSE )
            {
                state = E_TSP_HAL_FILE_STATE_PAUSE;
            }
            else if (u32Status)
            {
                state = E_TSP_HAL_FILE_STATE_BUSY;
            }
            else
            {
                state = E_TSP_HAL_FILE_STATE_IDLE;
            }
            break;
        case E_FILEENG_TSIF1:
            if (REG16_R(&_RegCtrl3->CFG3_21) & CFG3_21_TSIF1_FILE_PAUSE)
            {
                state = E_TSP_HAL_FILE_STATE_PAUSE;
            }
            else if (u32Status)
            {
                state = E_TSP_HAL_FILE_STATE_BUSY;
            }
            else
            {
                state = E_TSP_HAL_FILE_STATE_IDLE;
            }
            break;
        case E_FILEENG_TSIF2:
            if (REG16_R(&_RegCtrl3->CFG3_21) & CFG3_21_TSIF2_FILE_PAUSE)
            {
                state = E_TSP_HAL_FILE_STATE_PAUSE;
            }
            else if (u32Status)
            {
                state = E_TSP_HAL_FILE_STATE_BUSY;
            }
            else
            {
                state = E_TSP_HAL_FILE_STATE_IDLE;
            }
            break;
        default:
            state = E_TSP_HAL_FILE_STATE_INVALID;
            break;
    }
    return state;
}

void HAL_TSP_Filein_PacketMode(FILEENG_SEQ eFileEng,MS_BOOL bSet)
{
    if(bSet)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_SET(&_RegCtrl->reg160C, TSP_FILEIN192_EN);
                break;
            case E_FILEENG_TSIF1:
                REG16_SET(&_RegCtrl2->CFG_01, CFG_01_PKT192_EN1);
                break;
            case E_FILEENG_TSIF2:
                REG16_SET(&_RegCtrl2->CFG_06, CFG_06_TSP_PKT192_EN2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl->reg160C, TSP_FILEIN192_EN);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl2->CFG_01, CFG_01_PKT192_EN1);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl2->CFG_06, CFG_06_TSP_PKT192_EN2);
                break;
            default:
                break;
        }
    }

}

void HAL_TSP_Filein_BlockTimeStamp(FILEENG_SEQ eFileEng, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl->PktChkSizeFilein, TSP_PKT192_BLK_DIS_FIN);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl2->CFG_01, CFG_01_PKT192_BLK_DISABLE1);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl2->CFG_06, CFG_06_TSP_PKT192_BLK_DISABLE2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_SET(&_RegCtrl->PktChkSizeFilein, TSP_PKT192_BLK_DIS_FIN);
                break;
            case E_FILEENG_TSIF1:
                REG16_SET(&_RegCtrl2->CFG_01, CFG_01_PKT192_BLK_DISABLE1);
                break;
            case E_FILEENG_TSIF2:
                REG16_SET(&_RegCtrl2->CFG_06, CFG_06_TSP_PKT192_BLK_DISABLE2);
                break;
                break;
            default:
                break;
        }
    }
}

static void _HAL_TSP_FILEIN_ResetPktTimeStamp(FILEENG_SEQ eFileEng, MS_U32 u32InitTimeStamp)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_SET(&_RegCtrl3->CFG3_17, CFG3_17_INIT_TIMESTAMP_TSIF_0);
            REG32_W(&_RegCtrl6->CFG6_50_51, u32InitTimeStamp);
            REG16_CLR(&_RegCtrl3->CFG3_17, CFG3_17_INIT_TIMESTAMP_TSIF_0);
            break;
        case E_FILEENG_TSIF1:
            REG16_SET(&_RegCtrl3->CFG3_17, CFG3_17_INIT_TIMESTAMP_TSIF_1);
            REG32_W(&_RegCtrl6->CFG6_52_53, u32InitTimeStamp);
            REG16_CLR(&_RegCtrl3->CFG3_17, CFG3_17_INIT_TIMESTAMP_TSIF_1);
            break;
        case E_FILEENG_TSIF2:
            REG16_SET(&_RegCtrl3->CFG3_17, CFG3_17_INIT_TIMESTAMP_TSIF_2);
            REG32_W(&_RegCtrl6->CFG6_54_55, u32InitTimeStamp);
            REG16_CLR(&_RegCtrl3->CFG3_17, CFG3_17_INIT_TIMESTAMP_TSIF_2);
            break;
        default:
            break;
    }
}

void HAL_TSP_Filein_SetTimeStamp(FILEENG_SEQ eFileEng, MS_U32 u32Stamp)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_SET(&_RegCtrl->reg160C, TSP_LPCR2_WLD);
            REG32_W(&_RegCtrl->LPcr2, u32Stamp);
            REG16_CLR(&_RegCtrl->reg160C, TSP_LPCR2_WLD);
            break;
        case E_FILEENG_TSIF1:
            REG16_SET(&_RegCtrl2->CFG_01, CFG_01_LPCR2_WLD1);
            REG32_W(&_RegCtrl2->CFG_50_51, u32Stamp);
            REG16_CLR(&_RegCtrl2->CFG_01, CFG_01_LPCR2_WLD1);
            break;
        case E_FILEENG_TSIF2:
            REG16_SET(&_RegCtrl2->CFG_06, CFG_06_LPCR2_WLD2);
            REG32_W(&_RegCtrl2->CFG_52_53, u32Stamp);
            REG16_CLR(&_RegCtrl2->CFG_06, CFG_06_LPCR2_WLD2);
            break;
        default:
            break;
    }

    _HAL_TSP_FILEIN_ResetPktTimeStamp(eFileEng, 0);
}

void HAL_TSP_Filein_SetTimeStampClk(FILEENG_SEQ eFileEng, TSP_HAL_TimeStamp_Clk eTimeStampClk)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            if(eTimeStampClk==E_TSP_HAL_TIMESTAMP_CLK_27M)
            {
                REG16_SET(&_RegCtrl3->CFG3_0F, CFG3_0F_TSIF0_C27M);
            }
            else
            {
                REG16_CLR(&_RegCtrl3->CFG3_0F, CFG3_0F_TSIF0_C27M);
            }
            break;
        case E_FILEENG_TSIF1:
            if(eTimeStampClk==E_TSP_HAL_TIMESTAMP_CLK_27M)
            {
                REG16_SET(&_RegCtrl3->CFG3_0F, CFG3_0F_TSIF1_C27M);
            }
            else
            {
                REG16_CLR(&_RegCtrl3->CFG3_0F, CFG3_0F_TSIF1_C27M);
            }
            break;
        case E_FILEENG_TSIF2:
            if(eTimeStampClk==E_TSP_HAL_TIMESTAMP_CLK_27M)
            {
                REG16_SET(&_RegCtrl3->CFG3_0F, CFG3_0F_TSIF2_C27M);
            }
            else
            {
                REG16_CLR(&_RegCtrl3->CFG3_0F, CFG3_0F_TSIF2_C27M);
            }
            break;
        default:
            break;
    }
}

MS_U32 HAL_TSP_Filein_GetTimeStamp(FILEENG_SEQ eFileEng)
{
    MS_U32 u32Stamp = 0;
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG16_CLR(&_RegCtrl->reg160C, TSP_LPCR2_RLD);
            u32Stamp = REG32_R(&_RegCtrl->LPcr2);
            REG16_SET(&_RegCtrl->reg160C, TSP_LPCR2_RLD);
            break;
        case E_FILEENG_TSIF1:
            REG16_CLR(&_RegCtrl2->CFG_00, CFG_00_LPCR2_LOAD_TSIF1);
            u32Stamp = REG32_R(&_RegCtrl2->CFG_50_51);
            REG16_SET(&_RegCtrl2->CFG_00, CFG_00_LPCR2_LOAD_TSIF1);
            break;
        case E_FILEENG_TSIF2:
            REG16_CLR(&_RegCtrl2->CFG_05, CFG_05_LPCR2_LOAD_TSIF2);
            u32Stamp = REG32_R(&_RegCtrl2->CFG_52_53);
            REG16_SET(&_RegCtrl2->CFG_05, CFG_05_LPCR2_LOAD_TSIF2);
            break;
        default:
            u32Stamp = 0;
            break;
    }
    return u32Stamp;
}

MS_U32 HAL_TSP_Filein_PktTimeStamp(FILEENG_SEQ eFileEng)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            return REG32_R(&_RegCtrl->TimeStamp_FileIn);
        case E_FILEENG_TSIF1:
            return REG32_R(&_RegCtrl2->CFG_42_43);
        case E_FILEENG_TSIF2:
            return REG32_R(&_RegCtrl2->CFG_44_45);
        default:
            break;
    }
    return 0;
}

void HAL_TSP_Filein_GetCurAddr(FILEENG_SEQ eFileEng, MS_PHY *pu32Addr)
{
    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            *pu32Addr = (MS_PHY)(REG32_R(&_RegCtrl->TsFileIn_RPtr) & TSP_FILE_RPTR_MASK);
            break;
        case E_FILEENG_TSIF1:
            *pu32Addr = (MS_PHY)(REG32_R(&_RegCtrl2->CFG_6A_6B) & CFG_6A_6B_TSP2MI_RADDR_S_TSIF1);
            break;
        case E_FILEENG_TSIF2:
            *pu32Addr = (MS_PHY)(REG32_R(&_RegCtrl2->CFG_6C_6D) & CFG_6C_6D_TSP2MI_RADDR_S_TSIF2);
            break;
        default:
            break;
    }

}

void HAL_TSP_Filein_WbFsmRst(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_SET(&_RegCtrl3->CFG3_53, CFG3_53_WB_FSM_REST);
                break;
            case E_FILEENG_TSIF1:
                REG16_SET(&_RegCtrl6->CFG6_2B, TSP_RESET_WB_DMA_FSM_TSIF1);
                break;
            case E_FILEENG_TSIF2:
                REG16_SET(&_RegCtrl6->CFG6_2B, TSP_RESET_WB_DMA_FSM_TSIF2);
                break;
            default:
                HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[%s][%s][%d] UnSupported File Eng: %uld !\n",__FILE__,__FUNCTION__,__LINE__,(MS_U32)eFileEng));
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl3->CFG3_53, CFG3_53_WB_FSM_REST);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl6->CFG6_2B, TSP_RESET_WB_DMA_FSM_TSIF1);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl6->CFG6_2B, TSP_RESET_WB_DMA_FSM_TSIF2);
                break;
            default:
                HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[%s][%s][%d] UnSupported File Eng: %uld !\n",__FILE__,__FUNCTION__,__LINE__,(MS_U32)eFileEng));
                break;
        }
    }
}


void HAL_TSP_Filein_MOBF_Enable(FILEENG_SEQ eFileEng, MS_BOOL bEnable, MS_U32 u32Key)
{
    if(bEnable)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_W(&_RegCtrl->Mobf_Filein_Idx, (u32Key & TSP_MOBF_FILEIN_MASK));
                break;
            case E_FILEENG_TSIF1:
                REG16_W(&_RegCtrl2->CFG_75, (u32Key & CFG_75_FI_MOBF_INDEC_TSIF1_MASK));
                break;
            case E_FILEENG_TSIF2:
                REG16_W(&_RegCtrl2->CFG_76, (u32Key & CFG_76_FI_MOBF_INDEC_TSIF2_MASK));
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_W(&_RegCtrl->Mobf_Filein_Idx, 0);
                break;
            case E_FILEENG_TSIF1:
                REG16_W(&_RegCtrl2->CFG_75, 0);
                break;
            case E_FILEENG_TSIF2:
                REG16_W(&_RegCtrl2->CFG_76, 0);
                break;
            default:
                break;
        }
    }
}

MS_U32 HAL_TSP_FltSrc2PktDmx_Mapping(TSP_PIDFLT_SRC ePidFltSrc)
{
    switch (ePidFltSrc)
    {
        case E_TSP_PIDFLT_LIVE0:
            return TSP_PIDFLT_TSIF0;
        case E_TSP_PIDFLT_LIVE1:
            return TSP_PIDFLT_TSIF2;
        case E_TSP_PIDFLT_LIVE2:
            return TSP_PIDFLT_TSIF1;
        case E_TSP_PIDFLT_FILE0:
            return TSP_PIDFLT_TSIF1;
        case E_TSP_PIDFLT_FILE1:
            return TSP_PIDFLT_TSIF2;
        case E_TSP_PIDFLT_FILE2:
            return TSP_PIDFLT_TSIF0;
        default:
            printf("[TSP_ERR][%s][%d] Wrong filter source!!!\n",__FUNCTION__, __LINE__);
            break;
    }

    return 0;
}

void HAL_TSP_PidFlt_SetFltIn(MS_U32 fltId, MS_U32 u32FltIn)
{
    REG_PidFlt *pPidFlt = PPIDFLT0(fltId);
    TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_IN_MASK) | ((u32FltIn << TSP_PIDFLT_TSIF_SHFT) & TSP_PIDFLT_IN_MASK));
}

void HAL_TSP_PidFlt_SetFltOut(MS_U32 fltId, MS_U32 u32FltOut)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);
    TSP32_IdrW(pPidFlt, ((TSP32_IdrR(pPidFlt) & ~(TSP_PIDFLT_OUT_MASK)) | (u32FltOut & TSP_PIDFLT_OUT_MASK)));
}

void HAL_TSP_PidFlt_SetSrcID(MS_U32 fltId, MS_U32 u32SrcID)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);
    TSP32_IdrW(pPidFlt, ((TSP32_IdrR(pPidFlt) & ~(TSP_PIDFLT_SRCID_MASK)) | ((u32SrcID << TSP_PIDFLT_SRCID_SHIFT) & TSP_PIDFLT_SRCID_MASK)));
}

void HAL_TSP_PidFlt_SetSecFlt(MS_U32 fltId, MS_U32 u32SecFltId)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);
    TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_SECFLT_MASK) | ((u32SecFltId << TSP_PIDFLT_SECFLT_SHFT) & TSP_PIDFLT_SECFLT_MASK));
}

void HAL_TSP_PidFlt_SetPvrFlt(MS_U32 fltId, MS_U32 u32PVREng, MS_BOOL bEn)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);
    if(bEn)
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_PVRFLT_MASK) | (((1 << u32PVREng) << TSP_PIDFLT_PVRFLT_SHFT) & TSP_PIDFLT_PVRFLT_MASK));
    }
    else
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_PVRFLT_MASK));
    }
}

void HAL_TSP_PidFlt_SetFltRushPass(MS_U32 fltId, MS_U8 u8Enable)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);
    if(u8Enable)
        TSP32_IdrW( pPidFlt, TSP32_IdrR(pPidFlt) | (TSP_PID_FLT_PKTPUSH_PASS));
    else
        TSP32_IdrW( pPidFlt, TSP32_IdrR(pPidFlt) & ~(TSP_PID_FLT_PKTPUSH_PASS));
}

void HAL_TSP_PidFlt_SetPid(MS_U32 fltId, MS_U32 u32Pid)
{
    REG_PidFlt *pPidFlt = PPIDFLT0(fltId);
    TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_PID_MASK) | ((u32Pid << TSP_PIDFLT_PID_SHFT) & TSP_PIDFLT_PID_MASK));
}

//-------------------------------------------------------------------------------------------------
// For section filter part
//-------------------------------------------------------------------------------------------------

void HAL_TSP_SecFlt_BurstLen(MS_U32 burstMode)
{
     REG16_SET(&_RegCtrl->TSP_Cfg5, TSP_SEC_DMA_BURST_EN);
     REG16_MSK_W(&_RegCtrl->Hw_Config4, TSP_HW_DMA_MODE_MASK, (burstMode << TSP_HW_DMA_MODE_SHIFT));
}

void HAL_TSP_SecFlt_SetType(REG_SecFlt *pSecFlt, MS_U32 u32FltType)
{
    TSP32_IdrW(&pSecFlt->Ctrl, (TSP32_IdrR(&pSecFlt->Ctrl) & ~TSP_SECFLT_USER_MASK) | (u32FltType << TSP_SECFLT_USER_SHFT));
}


void HAL_TSP_SecFlt_SetMode(REG_SecFlt *pSecFlt, MS_U32 u32SecFltMode)
{
    TSP32_IdrW(&pSecFlt->Ctrl, (TSP32_IdrR(&pSecFlt->Ctrl) & ~TSP_SECFLT_MODE_MASK) | ((u32SecFltMode << TSP_SECFLT_MODE_SHFT) & TSP_SECFLT_MODE_MASK));
}


// match mask --> 0 will compare
void HAL_TSP_SecFlt_SetMask(REG_SecFlt *pSecFlt, MS_U8 *pu8Mask)
{
    MS_U32 i,j;
    MS_U32 u32Temp;

    for (i = 0; i < (TSP_FILTER_DEPTH/sizeof(MS_U32)); i++)
    {
        j = (i<< 2);
        u32Temp = (pu8Mask[j]) | (pu8Mask[j+ 1] << 8  ) | (pu8Mask[j+ 2] << 16  )| (pu8Mask[j+ 3] << 24);
        TSP32_IdrW((TSP32 *)&pSecFlt->Mask[i], u32Temp);
    }
}


// not match mask --> 1 will compare
void HAL_TSP_SecFlt_SetNMask(REG_SecFlt *pSecFlt, MS_U8 *pu8NMask)
{
    MS_U32 i,j;
    MS_U32 u32Temp;

    for (i = 0; i < (TSP_FILTER_DEPTH/sizeof(MS_U32)); i++)
    {
        j = (i<< 2);
        u32Temp = (pu8NMask[j]) | (pu8NMask[j+ 1] << 8  ) | (pu8NMask[j+ 2] << 16  )| (pu8NMask[j+ 3] << 24);
        TSP32_IdrW((TSP32 *)&pSecFlt->NMask[i], u32Temp);
    }
}


void HAL_TSP_SecFlt_SetMatch(REG_SecFlt *pSecFlt, MS_U8 *pu8Match)
{
    MS_U32 i,j;
    MS_U32 u32Temp;

    for (i = 0; i < (TSP_FILTER_DEPTH/sizeof(MS_U32)); i++)
    {
        j = (i<< 2);
        u32Temp =  (pu8Match[j]) | (pu8Match[j+ 1] << 8  ) | (pu8Match[j+ 2] << 16  )| (pu8Match[j+ 3] << 24);
        TSP32_IdrW((TSP32 *)&pSecFlt->Match[i], u32Temp);
    }
}


//[LIMIT] Is impossible to identify the pidfilter is assigned a secfilter/buffer or not
void HAL_TSP_SecFlt_SelSecBuf(REG_SecFlt *pSecFlt, MS_U16 u16BufId)
{
    TSP32_IdrW( &pSecFlt->Ctrl, (TSP32_IdrR(&pSecFlt->Ctrl) & ~TSP_SECFLT_SECBUF_MASK)
            | ((u16BufId << TSP_SECFLT_SECBUF_SHFT) & TSP_SECFLT_SECBUF_MASK) );
}


MS_U16 HAL_TSP_SecFlt_GetSecBuf(REG_SecFlt *pSecFlt)
{
    return ((TSP32_IdrR(&pSecFlt->Ctrl) & TSP_SECFLT_SECBUF_MASK) >> TSP_SECFLT_SECBUF_SHFT);
}

MS_U32 HAL_TSP_PidFlt_GetFltOutput(REG_PidFlt *pPidFlt)
{
    return (TSP32_IdrR(pPidFlt) & TSP_PIDFLT_OUT_MASK);
}

void HAL_TSP_SecFlt_DropEnable(MS_BOOL bSet)
{
// don't have to implement no reference
}


// @FIXME: Is it secflt or secbuf?
void HAL_TSP_SecFlt_ResetState(REG_SecFlt* pSecFlt)
{
    TSP32_IdrW(&pSecFlt->Ctrl, TSP32_IdrR(&pSecFlt->Ctrl) & ~(TSP_SECFLT_STATE_MASK));
}


void HAL_TSP_SecFlt_ClrCtrl(REG_SecFlt *pSecFlt)
{
    TSP32_IdrW(&pSecFlt->Ctrl, 0);
}

MS_U32 HAL_TSP_SecFlt_GetState(REG_SecFlt *pSecFlt)
{
    return ((TSP32_IdrR(&pSecFlt->Ctrl) & TSP_SECFLT_STATE_MASK) >> TSP_SECFLT_STATE_SHFT);
}


MS_U32 HAL_TSP_SecFlt_GetMode(REG_SecFlt *pSecFlt)
{
    return ((TSP32_IdrR(&pSecFlt->Ctrl) & TSP_SECFLT_MODE_MASK) >> TSP_SECFLT_MODE_SHFT);
}


void HAL_TSP_SecFlt_PcrReset(REG_SecFlt *pSecFlt)
{
    TSP32_IdrW(&pSecFlt->Ctrl, TSP32_IdrR(&pSecFlt->Ctrl) | TSP_SECFLT_PCRRST);
}


MS_BOOL HAL_TSP_SecFlt_TryAlloc(REG_SecFlt* pSecFlt, MS_U16 u16TSPId)
{
    MS_U32              reg;

    //_TSP_HW_Lock();

    // rmn counter doesn't need 32bit (check 13818) so we use some of it to store owner and alloc info
    reg = TSP32_IdrR(&pSecFlt->RmnCnt) & (TSP_SECFLT_OWNER_MASK | TSP_SECFLT_ALLOC_MASK);
    if (reg & TSP_SECFLT_ALLOC_MASK)
    {
        //_TSP_HW_Unlock();
        return FALSE;
    }
    reg |= TSP_SECFLT_ALLOC_MASK | ((u16TSPId<<TSP_SECFLT_OWNER_SHFT) & TSP_SECFLT_OWNER_MASK);
    TSP32_IdrW(&pSecFlt->RmnCnt, reg);

    //_TSP_HW_Unlock();
    return TRUE;
}


void HAL_TSP_SecFlt_SetAutoCRCChk(REG_SecFlt *pSecFlt, MS_BOOL bSet)
{
    if(bSet)
    {
        TSP32_IdrW(&pSecFlt->Ctrl, TSP32_IdrR(&pSecFlt->Ctrl) | TSP_SECFLT_MODE_AUTO_CRCCHK);
    }
    else
    {
        TSP32_IdrW(&pSecFlt->Ctrl, TSP32_IdrR(&pSecFlt->Ctrl) & ~TSP_SECFLT_MODE_AUTO_CRCCHK);
    }
}


void HAL_TSP_SecFlt_Free(REG_SecFlt* pSecFlt)
{
    MS_U32              reg;

    reg = TSP32_IdrR(&pSecFlt->RmnCnt) & ~(TSP_SECFLT_OWNER_MASK | TSP_SECFLT_ALLOC_MASK);
    TSP32_IdrW(&pSecFlt->RmnCnt, reg);
}

void HAL_TSP_SecFlt_ResetRmnCnt(REG_SecFlt* pSecFlt)
{
    TSP32_IdrW(&pSecFlt->RmnCnt, TSP32_IdrR(&pSecFlt->RmnCnt) & ~(TSP_SECBUF_RMNCNT_MASK));
}

//-------------------------------------------------------------------------------------------------
//  For section buffer part
//-------------------------------------------------------------------------------------------------
void HAL_TSP_SecBuf_SetBuf(REG_SecBuf *pSecBuf, MS_U32 u32StartAddr, MS_U32 u32BufSize)
{
    MS_U32              owner;
    // To avoid SW read hidden HW byte enable information.
    owner = TSP32_IdrR(&pSecBuf->Start); // @FIXME local variable but not used?

    TSP32_IdrW(&pSecBuf->Start, u32StartAddr);
    TSP32_IdrW(&pSecBuf->End, u32StartAddr + u32BufSize);
}


void HAL_TSP_SecBuf_SetRead(REG_SecBuf *pSecBuf, MS_U32 u32ReadAddr)
{
    TSP32_IdrW(&pSecBuf->Read, u32ReadAddr);
}


MS_U32 HAL_TSP_SecBuf_GetStart(REG_SecBuf *pSecBuf)
{
    return TSP32_IdrR(&pSecBuf->Start);
}


MS_U32 HAL_TSP_SecBuf_GetEnd(REG_SecBuf *pSecBuf)
{
    return TSP32_IdrR(&pSecBuf->End);
}


MS_U32 HAL_TSP_SecBuf_GetBufCur(REG_SecBuf *pSecBuf)
{
    //BufCur is control by firmware
    //we use Cur pointer to receive the newest data
    //and use write pointer to guarantee that the data between
    //read and write pointer is correct, so that user won't get
    //unverified data.
    return TSP32_IdrR(&pSecBuf->Cur);
}


void HAL_TSP_SecBuf_Reset(REG_SecBuf *pSecBuf)
{
    MS_U32              start;

    start = TSP32_IdrR(&pSecBuf->Start);

    TSP32_IdrW(&pSecBuf->Cur,    start);
    TSP32_IdrW(&pSecBuf->Read,   start);
    TSP32_IdrW(&pSecBuf->Write,  start);

    start = ( (MS_VIRT)pSecBuf - (MS_VIRT)REG_SECBUF_BASE ) / sizeof(REG_SecBuf)  ;
    HAL_TSP_HCMD_BufRst(start); // @FIXME seems we don't need to do this
}


MS_U32  HAL_TSP_SecBuf_GetRead(REG_SecBuf *pSecBuf)
{
    return TSP32_IdrR((TSP32*)&pSecBuf->Read);
}


MS_U32  HAL_TSP_SecBuf_GetWrite(REG_SecBuf *pSecBuf)
{
    return TSP32_IdrR((TSP32*)&pSecBuf->Write);
}


MS_BOOL HAL_TSP_SecBuf_TryAlloc(REG_SecBuf *pSecBuf, MS_U16 u16TSPId)
{
    // @TODO make sure the owner and alloc info is necessary or not.
    MS_U32              reg;

    //_TSP_HW_Lock();

    reg = TSP32_IdrR(&pSecBuf->Start) & (TSP_SECBUF_OWNER_MASK | TSP_SECBUF_ALLOC_MASK);
    //if (reg & TSP_SECBUF_ALLOC_MASK)
    //{
    //    //_TSP_HW_Unlock();
    //    return FALSE;
    //}
    reg |= TSP_SECBUF_ALLOC_MASK | ((u16TSPId<<TSP_SECBUF_OWNER_SHFT) & TSP_SECBUF_OWNER_MASK);
    //TSP32_IdrW(&pSecBuf->Start, reg);

    //_TSP_HW_Unlock();
    return TRUE;
}


void HAL_TSP_SecBuf_Free(REG_SecBuf *pSecBuf)
{
    // @TODO ref to HAL_TSP_SecBuf_TryAlloc
    TSP32_IdrW(&pSecBuf->Start, 0x0);
}

MS_U32 HAL_TSP_PidFlt_GetPid(REG_PidFlt* pPidFlt)
{
    return ((TSP32_IdrR(pPidFlt) & TSP_PIDFLT_PID_MASK) >> TSP_PIDFLT_PID_SHFT);
}

void HAL_TSP_PcrFlt_Enable(MS_U32 pcrFltId, MS_BOOL bEnable)
{
    switch(pcrFltId)
    {
        case 0:
            if(bEnable)
            {
                REG16_SET(&_RegCtrl->PIDFLT_PCR0, TSP_PIDFLT_PCR0_EN);
            }
            else
            {
                REG16_CLR(&_RegCtrl->PIDFLT_PCR0, TSP_PIDFLT_PCR0_EN);
            }
            break;
        case 1:
            if(bEnable)
            {
                REG16_SET(&_RegCtrl->PIDFLT_PCR1, TSP_PIDFLT_PCR1_EN);
            }
            else
            {
                REG16_CLR(&_RegCtrl->PIDFLT_PCR1, TSP_PIDFLT_PCR1_EN);
            }
            break;
        default:
            break;
    }
}

void HAL_TSP_PcrFlt_SetPid(MS_U32 pcrFltId, MS_U32 u32Pid)
{
    switch(pcrFltId)
    {
        case 0:
            REG16_MSK_W(&_RegCtrl->PIDFLT_PCR0, TSP_PIDFLT_PCR0_PID_MASK, u32Pid);
            break;
        case 1:
            REG16_MSK_W(&_RegCtrl->PIDFLT_PCR1, TSP_PIDFLT_PCR1_PID_MASK, u32Pid);
            break;
        default:
            break;
    }

}

MS_U32 HAL_TSP_PcrFlt_GetPid(MS_U32 pcrFltId)
{
    switch(pcrFltId)
    {
        case 0:
            return (REG16_R(&_RegCtrl->PIDFLT_PCR0) & TSP_PIDFLT_PCR0_PID_MASK);
        case 1:
            return (REG16_R(&_RegCtrl->PIDFLT_PCR1) & TSP_PIDFLT_PCR1_PID_MASK);
        default:
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[PVR ERROR][%s][%d] PCR flt id not support !!\n",__FUNCTION__,__LINE__));
            return PVR_PIDFLT_DEF;
    }
}

void HAL_TSP_PcrFlt_SetSrc(MS_U32 pcrFltId, TSP_PCR_SRC src)
{
    switch(pcrFltId)
    {
        case 0:
            //src 0
            REG16_MSK_W(&_RegCtrl2->CFG_01, CFG_01_PCR0_SRC_MASK, src << CFG_01_PCR0_SRC_SHIFT);
            break;
        case 1:
            //src 1
            REG16_MSK_W(&_RegCtrl2->CFG_01, CFG_01_PCR1_SRC_MASK, src << CFG_01_PCR1_SRC_SHIFT);
            break;
        default:
            break;
    }
}

//[Jason]
void HAL_TSP_PcrFlt_GetSrc(MS_U32 pcrFltId, TSP_PCR_SRC *pPcrSrc)
{
    //printf("[Jason][%s][%d] pcrFltId = %x\n", __FUNCTION__, __LINE__, pcrFltId);

    switch(pcrFltId)
    {
        case 0:
            //src 0
            *pPcrSrc = (REG16_R(&_RegCtrl2->CFG_01) & CFG_01_PCR0_SRC_MASK) >> CFG_01_PCR0_SRC_SHIFT;
            break;
        case 1:
            //src 1
            *pPcrSrc = (REG16_R(&_RegCtrl2->CFG_01) & CFG_01_PCR1_SRC_MASK) >> CFG_01_PCR1_SRC_SHIFT;
            break;
        default:
            break;
    }
}

TSP_PCR_SRC HAL_TSP_FltSrc2PCRSrc_Mapping(TSP_PIDFLT_SRC ePidFltSrc)
{
    TSP_PCR_SRC ePcrSrc = E_TSP_PCR_SRC_INVALID;
    switch(ePidFltSrc)
    {
        case E_TSP_PIDFLT_LIVE0:
            ePcrSrc = E_TSP_PCR_SRC_TSIF0;
            break;
        case E_TSP_PIDFLT_LIVE1:
            ePcrSrc = E_TSP_PCR_SRC_TSIF2;
            break;
        case E_TSP_PIDFLT_LIVE2:
            ePcrSrc = E_TSP_PCR_SRC_TSIF1;
            break;
        case E_TSP_PIDFLT_FILE0:
            ePcrSrc = E_TSP_PCR_SRC_TSIF1;
            break;
        case E_TSP_PIDFLT_FILE1:
            ePcrSrc = E_TSP_PCR_SRC_TSIF2;
            break;
        case E_TSP_PIDFLT_FILE2:
            ePcrSrc = E_TSP_PCR_SRC_TSIF0;
            break;
        default:
            printf("[TSP_ERR][%s][%d] Wrong Flt Src type!!!\n",__FUNCTION__,__LINE__);
            break;
    }
    return ePcrSrc;

}


void HAL_TSP_PcrFlt_GetPcr(MS_U32 pcrFltId, MS_U32 *pu32Pcr_H, MS_U32 *pu32Pcr)
{
    switch(pcrFltId)
    {
        case 0:
            REG16_SET(&_RegCtrl->PCR_Cfg, TSP_PCR0_READ);
            *pu32Pcr = REG32_R(&_RegCtrl->HWPCR0_L);
            *pu32Pcr_H = REG32_R(&_RegCtrl->HWPCR0_H) & 0x1;
            REG16_CLR(&_RegCtrl->PCR_Cfg, TSP_PCR0_READ);
            break;
        case 1:
            REG16_SET(&_RegCtrl->PCR_Cfg, TSP_PCR1_READ);
            *pu32Pcr = REG32_R(&_RegCtrl->HWPCR1_L);
            *pu32Pcr_H = REG32_R(&_RegCtrl->HWPCR1_H) & 0x1;
            REG16_CLR(&_RegCtrl->PCR_Cfg, TSP_PCR1_READ);
            break;
        default:
            break;
    }
}

void HAL_TSP_PcrFlt_Reset(MS_U32 pcrFltId)
{
    switch(pcrFltId)
    {
        case 0:
            REG16_CLR(&_RegCtrl->PCR_Cfg, TSP_PCR0_RESET);
            REG16_SET(&_RegCtrl->PCR_Cfg, TSP_PCR0_RESET);
            break;
        case 1:
            REG16_CLR(&_RegCtrl->PCR_Cfg, TSP_PCR1_RESET);
            REG16_SET(&_RegCtrl->PCR_Cfg, TSP_PCR1_RESET);
            break;
        default:
            break;
    }
}


void HAL_TSP_PcrFlt_ClearInt(MS_U32 pcrFltId)
{
    switch(pcrFltId)
    {
        case 0:
            //REG16_CLR(&_RegCtrl->SwInt_Stat1_L,TSP_HWINT2_PCR0_UPDATE_END);
            REG16_W(&_RegCtrl->SwInt_Stat1_L,
            (REG16_R(&_RegCtrl->SwInt_Stat1_L) & (~TSP_HWINT2_STATUS_MASK)) |
            (~TSP_HWINT2_PCR0_UPDATE_END & TSP_HWINT2_STATUS_MASK));
            break;
        case 1:
            //REG16_CLR(&_RegCtrl->SwInt_Stat1_L,TSP_HWINT2_PCR1_UPDATE_END);
            REG16_W(&_RegCtrl->SwInt_Stat1_L,
            (REG16_R(&_RegCtrl->SwInt_Stat1_L) & (~TSP_HWINT2_STATUS_MASK)) |
            (~TSP_HWINT2_PCR1_UPDATE_END & TSP_HWINT2_STATUS_MASK));
            break;
        default:
            break;
    }
}

MS_U32 HAL_TSP_PcrFlt_GetIntMask(MS_U32 pcrFltId)
{
    switch(pcrFltId)
    {
        case 0:
            return (TSP_HWINT2_PCR0_UPDATE_END_EN << 8);
        case 1:
            return (TSP_HWINT2_PCR1_UPDATE_END_EN << 8);
        default:
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP ERROR][%s][%d] PCR flt id not support !!\n",__FUNCTION__,__LINE__));
            return 0;
    }
}

void HAL_TSP_STC_Init(void)
{
    /////////////Set STC control by HK////////////////
    // select synth from chip top : bit 1 -> 0 -> controlled by HK
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~REG_CLKGEN0_STC_CW_SEL;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~REG_CLKGEN0_STC1_CW_SEL;

    // set HK STC synth CW
     //if CLK_MPLL_SYN is 432MHz, set 0x28000000;if CLK_MPLL_SYN is 216MHz, set 0x14000000
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_L) = 0x0000;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_H) = 0x2800;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_L) = 0x0000;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_H) = 0x2800;

    // set STC synth
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC_CW_EN;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC1_CW_EN);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC1_CW_EN;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC1_CW_EN);

#if 0  // we don't use TSP CPU to control STC anymmore, so we don't have to do the following
    /////////////Set STC control by TSP////////////////
    // select synth from TSP      : bit 1 -> 1 -> controlled by TSP
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC_CW_SEL;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC1_CW_SEL;

    // set TSP STC synth CW
    //if CLK_MPLL_SYN is 432MHz, set 0x28000000;if CLK_MPLL_SYN is 216MHz, set 0x14000000
    TSP32_IdrW((TSP32 *)(0x0021024c<<1), 0x28000000);
    TSP32_IdrW((TSP32 *)(0x00210280<<1), 0x28000000); //STC1

    // t2 , t3 had no 0x0021025c, it was add after t4, eanble synthesizer
    TSP32_IdrW((TSP32 *)(0x0021025c<<1), TSP32_IdrR((TSP32 *)(0x0021025c<<1))|0x01);
    TSP32_IdrW((TSP32 *)(0x0021025c<<1), TSP32_IdrR((TSP32 *)(0x0021025c<<1))& ~0x01);
    TSP32_IdrW((TSP32 *)(0x0021025c<<1), TSP32_IdrR((TSP32 *)(0x0021025c<<1))|0x02); //STC1
    TSP32_IdrW((TSP32 *)(0x0021025c<<1), TSP32_IdrR((TSP32 *)(0x0021025c<<1))& ~0x02);
#endif
}

void HAL_TSP_GetSTCSynth(MS_U32 Eng, MS_U32* u32Sync)
{
    switch (Eng)
    {
        case 0:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_L);
            *u32Sync |= TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_H) << 16 ;
            break;
        case 1:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_L);
            *u32Sync |= TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_H) << 16 ;
            break;
    }
}

void HAL_TSP_SetSTCSynth(MS_U32 Eng, MS_U32 u32Sync)
{
    switch (Eng)
    {
        case 0:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~REG_CLKGEN0_STC_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |=   REG_CLKGEN0_STC_CW_EN;
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);
            break;
        case 1:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~REG_CLKGEN0_STC1_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC1_CW_EN);
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |=   REG_CLKGEN0_STC1_CW_EN;
            TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC1_CW_EN);
            break;
    }
}

void HAL_TSP_STC64_Mode_En(MS_BOOL bEnable)
{
    if (bEnable)
    {
        REG16_SET(&_RegCtrl->TSP_Cfg5, TSP_SYSTIME_MODE);
    }
    else
    {
        REG16_CLR(&_RegCtrl->TSP_Cfg5, TSP_SYSTIME_MODE);
    }
}

void HAL_TSP_STC64_Set(MS_U32 Eng, MS_U32 stcH, MS_U32 stcL)
{
    switch (Eng)
    {
        case 0:
            REG32_W(&_RegCtrl->Pcr_L, stcL);
            REG32_W(&_RegCtrl->Pcr_H, stcH);
            break;
        case 1:
            REG32_W(&_RegCtrl->PCR64_2_L, stcL);
            REG32_W(&_RegCtrl->PCR64_2_H, stcH);
            break;
    }
}

void HAL_TSP_STC64_Get(MS_U32 Eng, MS_U32* pStcH, MS_U32* pStcL)
{
    switch (Eng)
    {
        case 0:
            REG16_CLR(&_RegCtrl->reg15b8, TSP_cnt_33b_ld);
            *pStcH = REG32_R(&_RegCtrl->Pcr_H);
            *pStcL = REG32_R(&_RegCtrl->Pcr_L);
            REG16_SET(&_RegCtrl->reg15b8, TSP_cnt_33b_ld);
            break;
        case 1:
            REG16_CLR(&_RegCtrl->reg15b8, TSP_64bit_PCR2_ld);
            *pStcH = REG32_R(&_RegCtrl->PCR64_2_H);
            *pStcL = REG32_R(&_RegCtrl->PCR64_2_L);
            REG16_SET(&_RegCtrl->reg15b8, TSP_64bit_PCR2_ld);
            break;
    }
}

void HAL_TSP_STC33_CmdQSet(MS_U32 stcH, MS_U32 stcL)
{
    // @TODO ask designer for the difference between 64bit STC and 33 Bit STC
    // and it's hw limit (like: cmdQ delay)
    REG16_W(&_RegCtrl->Pcr_H_CmdQ, stcH & TSP_REG_PCR_CMDQ_H);
    REG32_W(&_RegCtrl->Pcr_L_CmdQ, stcL);
}

void HAL_TSP_STC33_CmdQGet(MS_U32* pStcH, MS_U32* pStcL)
{
    REG16_CLR(&_RegCtrl->reg15b8, TSP_cnt_33b_ld);
    *pStcH = REG16_R(&_RegCtrl->Pcr_H_CmdQ) & TSP_REG_PCR_CMDQ_H;
    *pStcL = REG32_R(&_RegCtrl->Pcr_L_CmdQ);
    REG16_SET(&_RegCtrl->reg15b8, TSP_cnt_33b_ld);
}

void HAL_TSP_FIFO_SetSrc(TSP_DST_SEQ eFltType, MS_U32 pktDmxId)
{
    switch (eFltType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            REG16_MSK_W(&_RegCtrl->FIFO_Src, TSP_VID_SRC_MASK, ((MS_U16)pktDmxId) << TSP_VID_SRC_SHIFT);
            break;
        case E_TSP_DST_FIFO_VIDEO3D :
            REG16_MSK_W(&_RegCtrl->FIFO_Src, TSP_VID3D_SRC_MASK, ((MS_U16)pktDmxId) << TSP_VID3D_SRC_SHIFT);
            break;
        case E_TSP_DST_FIFO_AUDIO :
            REG16_MSK_W(&_RegCtrl->FIFO_Src, TSP_AUD_SRC_MASK, ((MS_U16)pktDmxId) << TSP_AUD_SRC_SHIFT);
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            REG16_MSK_W(&_RegCtrl->FIFO_Src, TSP_AUDB_SRC_MASK, ((MS_U16)pktDmxId)  << TSP_AUDB_SRC_SHIFT);
            break;
        default:
            break;
    }
}

void HAL_TSP_FIFO_GetSrc(TSP_DST_SEQ eFltType, TSP_SRC_SEQ *pktDmxId)
{
    switch (eFltType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            *pktDmxId = ((REG16_R(&_RegCtrl->FIFO_Src)) & TSP_VID_SRC_MASK) >> TSP_VID_SRC_SHIFT;
            break;
        case E_TSP_DST_FIFO_VIDEO3D:
            *pktDmxId = ((REG16_R(&_RegCtrl->FIFO_Src)) & TSP_VID3D_SRC_MASK) >> TSP_VID3D_SRC_SHIFT;
            break;
        case E_TSP_DST_FIFO_AUDIO :
            *pktDmxId = ((REG16_R(&_RegCtrl->FIFO_Src)) & TSP_AUD_SRC_MASK) >> TSP_AUD_SRC_SHIFT;
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            *pktDmxId = ((REG16_R(&_RegCtrl->FIFO_Src)) & TSP_AUDB_SRC_MASK) >> TSP_AUDB_SRC_SHIFT;
            break;
        default:
            break;
    }
}

void HAL_TSP_FIFO_ClearAll()
{
    // clear ALL FIFO !!!
    REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_VID_EN);
    REG16_CLR(&_RegCtrl->TSP_Ctrl2, TSP_PS_VID_3D_EN);
    REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUD_EN);
    REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUDB_EN);
    REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUDC_EN);
    REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUDD_EN);

}


void HAL_TSP_FIFO_Connect(MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        REG16_SET(&_RegCtrl->reg15b4,TSP_AVFIFO_RD_EN);
    }
    else
    {
        REG16_CLR(&_RegCtrl->reg15b4,TSP_AVFIFO_RD_EN);
    }

}


MS_U16 HAL_TSP_FIFO_ReadPkt(void)
{
    return (REG16_R(&_RegCtrl->PKT_CNT) & TSP_PKT_CNT_MASK);
}




void HAL_TSP_FIFO_ReadSrc(TSP_DST_SEQ eFltType)
{
    switch (eFltType)
    {
        case E_TSP_DST_FIFO_VIDEO:
            REG16_MSK_W(&_RegCtrl3->CFG3_2C, CFG3_2C_AVFIFO_READ_SEL_MASK, ((MS_U16)CFG3_2C_AVFIFO_READ_SEL_V) << CFG3_2C_AVFIFO_READ_SEL_SHIFT);
            break;
        case E_TSP_DST_FIFO_AUDIO:
            REG16_MSK_W(&_RegCtrl3->CFG3_2C, CFG3_2C_AVFIFO_READ_SEL_MASK, ((MS_U16)CFG3_2C_AVFIFO_READ_SEL_A) << CFG3_2C_AVFIFO_READ_SEL_SHIFT);
            break;
        case E_TSP_DST_FIFO_AUDIO2:
            REG16_MSK_W(&_RegCtrl3->CFG3_2C, CFG3_2C_AVFIFO_READ_SEL_MASK, ((MS_U16)CFG3_2C_AVFIFO_READ_SEL_AB) << CFG3_2C_AVFIFO_READ_SEL_SHIFT);
            break;
        case E_TSP_DST_FIFO_VIDEO3D:
            REG16_MSK_W(&_RegCtrl3->CFG3_2C, CFG3_2C_AVFIFO_READ_SEL_MASK, ((MS_U16)CFG3_2C_AVFIFO_READ_SEL_V3D) << CFG3_2C_AVFIFO_READ_SEL_SHIFT);
            break;
        default:
            break;
    }
}




//@NOTE for TS mode
//@TODO need to rename (TS enable or PKTDMX_BYPASS)
void HAL_TSP_Flt_Bypass(TSP_DST_SEQ eFltType, MS_BOOL bEn) // @NOTE this function only for Tsif0 fileEng other fileEng has no by pass capability
{
    if(bEn)
    {
        switch (eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO:
                REG16_SET(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_TSIF0_VPID_BYPASS);
                break;
            case E_TSP_DST_FIFO_AUDIO:
                REG16_SET(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_TSIF0_APID_BYPASS);
                break;
            case E_TSP_DST_FIFO_AUDIO2:
                REG16_SET(&_RegCtrl3->CFG3_2D, CFG3_2D_APID_B_BYPASS);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_SET(&_RegCtrl3->CFG3_2D, CFG3_2D_VPID_3D_BYPASS);
                break;
            default:
                break;
        }
    }
    else
    {
        switch (eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO:
                REG16_CLR(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_TSIF0_VPID_BYPASS);
                break;
            case E_TSP_DST_FIFO_AUDIO:
                REG16_CLR(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_TSIF0_APID_BYPASS);
                break;
            case E_TSP_DST_FIFO_AUDIO2:
                REG16_CLR(&_RegCtrl3->CFG3_2D, CFG3_2D_APID_B_BYPASS);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_CLR(&_RegCtrl3->CFG3_2D, CFG3_2D_VPID_3D_BYPASS);
                break;
            default:
                break;

        }
    }
}


void HAL_TSP_PS_SRC(MS_U32 tsIf)
{
    REG16_MSK_W(&_RegCtrl3->CFG3_10, CFG3_10_PS_MODE_SRC_MASK,(((MS_U16)tsIf)<< CFG3_10_PS_MODE_SRC_SHIFT));
}



//PS MODE
//NEED TO rename
void HAL_TSP_FIFO_Bypass(TSP_DST_SEQ eFltType, MS_BOOL bEn) // @NOTE this function only for Tsif0 fileEng other fileEng has no by pass capability
{
    if(bEn)
    {
        switch (eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO:
                REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_VID_EN);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_SET(&_RegCtrl->TSP_Ctrl2, TSP_PS_VID_3D_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO:
                REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUD_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO2:
                REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUDB_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch (eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_VID_EN);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_CLR(&_RegCtrl->TSP_Ctrl2, TSP_PS_VID_3D_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUD_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_PS_AUDB_EN);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_FIFO_Bypass_Src(FILEENG_SEQ eFileEng, TSP_DST_SEQ eFltType)
{
   // (K6 HW CL) Kano doesn't support
   // PS mode source sel need to be independent
}

MS_U32 HAL_TSP_FIFO_PidHit(TSP_DST_SEQ eFltType)
{
    switch (eFltType)
    {
    case E_TSP_DST_FIFO_VIDEO :
        return REG16_R(&_RegCtrl->Vd_Pid_Hit) & TSP_VPID_MASK;
    case E_TSP_DST_FIFO_VIDEO3D :
        return REG16_R(&_RegCtrl2->CFG_70) & CFG_70_MATCHECED_VPID_3D_MASK;
    case E_TSP_DST_FIFO_AUDIO :
        return REG16_R(&_RegCtrl->Aud_Pid_Hit) & TSP_APID_MASK;
    case E_TSP_DST_FIFO_AUDIO2 :
        return REG16_R(&_RegCtrl2->CFG_71) & CFG_71_MATCHECED_APID_B_MASK;
    default:
        return 0x1FFF;
    }
}

void HAL_TSP_FIFO_Reset(TSP_DST_SEQ eFltType, MS_BOOL bReset)
{
    if (bReset)
    {
        switch (eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_SET(&_RegCtrl->reg160E, TSP_RESET_VFIFO);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_SET(&_RegCtrl->reg160E, TSP_RESET_VFIFO3D);
                break;

            case E_TSP_DST_FIFO_AUDIO :
                REG16_SET(&_RegCtrl->reg160E, TSP_RESET_AFIFO);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_SET(&_RegCtrl->reg160E, TSP_RESET_AFIFO2);
                break;
            default :
                break;
        }
    }
    else
    {
        switch (eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_CLR(&_RegCtrl->reg160E, TSP_RESET_VFIFO);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_CLR(&_RegCtrl->reg160E, TSP_RESET_VFIFO3D);
                break;

            case E_TSP_DST_FIFO_AUDIO :
                REG16_CLR(&_RegCtrl->reg160E, TSP_RESET_AFIFO);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_CLR(&_RegCtrl->reg160E, TSP_RESET_AFIFO2);
                break;
            default :
                break;
        }
    }

    _delay(1);
}

void HAL_TSP_FIFO_Skip_Scrmb(TSP_DST_SEQ eFltType,MS_BOOL bSkip)
{
    if(bSkip)
    {
        switch(eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO:
                REG16_SET(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_V_EN);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_SET(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_V3D_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO:
                REG16_SET(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_A_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO2:
                REG16_SET(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_AB_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO:
                REG16_CLR(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_V_EN);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG16_CLR(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_V3D_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO:
                REG16_CLR(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_A_EN);
                break;
            case E_TSP_DST_FIFO_AUDIO2:
                REG16_CLR(&_RegCtrl3->CFG3_34, CFG3_34_MASK_SRC_AB_EN);
                break;
            default:
                break;
        }

    }
}


void HAL_TSP_FIFO_BlockDis(TSP_DST_SEQ eFltType, MS_BOOL bDisable)
{
    if(bDisable)
    {
        switch(eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_V_BLOCK_DIS);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_V3d_BLOCK_DIS);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_A_BLOCK_DIS);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_AD_BLOCK_DIS);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFltType)
        {
            case E_TSP_DST_FIFO_VIDEO:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_V_BLOCK_DIS);
                break;
            case E_TSP_DST_FIFO_VIDEO3D:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_V3d_BLOCK_DIS);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_A_BLOCK_DIS);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_AD_BLOCK_DIS);
                break;
            default:
                break;
        }
    }
}

MS_BOOL HAL_TSP_FIFO_IsReset(TSP_DST_SEQ eFltType)
{
    MS_U32 u32Matched = 0;
    switch (eFltType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            u32Matched = REG16_R(&_RegCtrl->reg160E) & TSP_RESET_VFIFO;
            break;
        case E_TSP_DST_FIFO_VIDEO3D:
            u32Matched = REG16_R(&_RegCtrl->reg160E) & TSP_RESET_VFIFO3D;
            break;
        case E_TSP_DST_FIFO_AUDIO :
            u32Matched = REG16_R(&_RegCtrl->reg160E) & TSP_RESET_AFIFO;
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            u32Matched = REG16_R(&_RegCtrl->reg160E) & TSP_RESET_AFIFO2;
            break;
        default :
            return FALSE;
    }
    return (u32Matched) ? TRUE: FALSE;
}

MS_U32  HAL_TSP_FIFO_GetStatus(TSP_DST_SEQ eFltType)
{
    #define E_TSP_FIFO_STATUS_ISRESET    0x80000000
    #define E_TSP_FIFO_STATUS_EMPTY      0x00000001
    #define E_TSP_FIFO_STATUS_OVERFLOW   0x00000002
    #define E_TSP_FIFO_STATUS_LEVEL      0x0000000C
    #define TSP_FIFO_STATUS_LEVEL_SHFT            2   //shift the value get from HAL_TSP_FIFO_Level to the u32Status level position
    // @NOTE please follow K2 like series to return this value
    // since API layer didn't define for each column
    // we only defined that 0x80000000 stands for this fifo reset is High
    //                               0x00000001 stands for this fifo is empty
    //                               0x00000002 stands for this fifo is overflow
    //                               0x0000000C is a mask for this fifo level
    // this is exclusive usage
    MS_U32 u32Status = 0;
    if (HAL_TSP_FIFO_IsReset(eFltType))
    {
        u32Status |= E_TSP_FIFO_STATUS_ISRESET;
    }
    if (HAL_TSP_FIFO_Empty(eFltType))
    {
        u32Status |= E_TSP_FIFO_STATUS_EMPTY;
    }
    if (HAL_TSP_FIFO_Overflow(eFltType))
    {
        u32Status |= E_TSP_FIFO_STATUS_OVERFLOW;
    }

    u32Status |= ((HAL_TSP_FIFO_Level(eFltType)<<TSP_FIFO_STATUS_LEVEL_SHFT)&E_TSP_FIFO_STATUS_LEVEL);

    return u32Status;
}

MS_U32 HAL_TSP_FIFO_Level(TSP_DST_SEQ eFltType)
{
    switch (eFltType)
    {
    case E_TSP_DST_FIFO_VIDEO :
        return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_VFIFO_LEVEL)  >> TSP_VFIFO_LEVEL_SHFT;
    case E_TSP_DST_FIFO_VIDEO3D :
        return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_VFIFO3D_LEVEL)  >> TSP_VFIFO3D_LEVEL_SHFT;
    case E_TSP_DST_FIFO_AUDIO :
        return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_AFIFO_LEVEL)  >> TSP_AFIFO_LEVEL_SHFT;
    case E_TSP_DST_FIFO_AUDIO2 :
        return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_AFIFOB_LEVEL)  >> TSP_AFIFOB_LEVEL_SHFT;
    default :
        return 0;
    }
}

MS_BOOL HAL_TSP_FIFO_Overflow(TSP_DST_SEQ eFltType)
{
    switch (eFltType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_VFIFO_FULL)  >> TSP_VFIFO_FULL_SHFT;
        case E_TSP_DST_FIFO_VIDEO3D :
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_VFIFO3D_FULL)  >> TSP_VFIFO3D_FULL_SHFT;
        case E_TSP_DST_FIFO_AUDIO :
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_AFIFO_FULL)  >> TSP_AFIFO_FULL_SHFT;
        case E_TSP_DST_FIFO_AUDIO2 :
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_AFIFOB_FULL)  >> TSP_AFIFOB_FULL_SHFT;
        default :
            return FALSE;
    }
}

MS_BOOL HAL_TSP_FIFO_Empty(TSP_DST_SEQ eFltType)
{
    switch (eFltType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_VFIFO_EMPTY)  >> TSP_VFIFO_EMPTY_SHFT;
        case E_TSP_DST_FIFO_VIDEO3D:
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_VFIFO3D_EMPTY)  >> TSP_VFIFO3D_EMPTY_SHFT;
        case E_TSP_DST_FIFO_AUDIO :
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_AFIFO_EMPTY)  >> TSP_AFIFO_EMPTY_SHFT;
        case E_TSP_DST_FIFO_AUDIO2 :
            return (REG16_R(&_RegCtrl->AVFifoSts) & TSP_AFIFOB_EMPTY)  >> TSP_AFIFOB_EMPTY_SHFT;
        default :
            return FALSE;
    }
}

static MS_BOOL _HAL_TSP_VQ_TxConfig(MS_U32 vqId)
{
    // reg_vq_wr_threshold = 0x8
    // reg_vq_forcefire_cnt_1k= 0xC

    switch(vqId)
    {
        case 0:
            REG16_W(&_RegCtrl->VQ0_CTRL, (REG16_R(&_RegCtrl->VQ0_CTRL) & (~TSP_VQ0_WR_THRESHOLD_MASK)) | ((0x8 << TSP_VQ0_WR_THRESHOLD_SHIFT) & TSP_VQ0_WR_THRESHOLD_MASK));
            REG16_W(&_RegCtrl->VQ0_CTRL, (REG16_R(&_RegCtrl->VQ0_CTRL) & (~TSP_VQ0_FORCE_FIRE_CNT_1K_MASK)) | ((0xC << TSP_VQ0_FORCE_FIRE_CNT_1K_SHIFT) & TSP_VQ0_FORCE_FIRE_CNT_1K_MASK));
            break;
        case 1:
            REG16_W(&_RegCtrl->VQ1_Config, (REG16_R(&_RegCtrl->VQ1_Config) & (~TSP_VQ1_WR_THRESHOLD_MASK)) | ((0x8 << TSP_VQ1_WR_THRESHOLD_SHIFT) & TSP_VQ1_WR_THRESHOLD_MASK));
            REG16_W(&_RegCtrl->VQ1_Config, (REG16_R(&_RegCtrl->VQ1_Config) & (~TSP_VQ1_FORCEFIRE_CNT_1K_MASK)) | ((0xC << TSP_VQ1_FORCEFIRE_CNT_1K_SHIFT) & TSP_VQ1_FORCEFIRE_CNT_1K_MASK));
            break;
        case 2:
            REG16_W(&_RegCtrl->VQ2_Config, (REG16_R(&_RegCtrl->VQ2_Config) & (~TSP_VQ2_WR_THRESHOLD_MASK)) | ((0x8 << TSP_VQ2_WR_THRESHOLD_SHIFT) & TSP_VQ2_WR_THRESHOLD_MASK));
            REG16_W(&_RegCtrl->VQ2_Config, (REG16_R(&_RegCtrl->VQ2_Config) & (~TSP_VQ2_FORCEFIRE_CNT_1K_MASK)) | ((0xC << TSP_VQ2_FORCEFIRE_CNT_1K_SHIFT) & TSP_VQ2_FORCEFIRE_CNT_1K_MASK));
            break;
        default:
            return FALSE;
    }
    return TRUE;
}


MS_BOOL HAL_TSP_SetVQ( MS_PHYADDR u32BaseAddr, MS_U32 u32BufLen)
{
    MS_U32 vqId;
    MS_U32 u32VQ_PktNum = 0;
    MS_U32 u32Addr = u32BaseAddr;
    MS_U32 u32OneBufSize = 0;

    u32OneBufSize = ((u32BufLen >> MIU_BUS) / VQ_NUM) << MIU_BUS; //miu alignment
    u32VQ_PktNum = u32OneBufSize / VQ_PACKET_UNIT_LEN;

    for(vqId = 0; vqId < VQ_NUM; vqId ++)
    {
        if(TRUE != _HAL_TSP_VQ_TxConfig(vqId))
        {
            return FALSE;
        }

        // in kaiser we needs to set 6 VQ and the base unit is 208 so total vq size should be N*208*6
        if (TRUE != HAL_TSP_VQ_Buffer(vqId, u32Addr, u32VQ_PktNum))
        {
            return FALSE;
        }
        u32Addr += u32OneBufSize;
    }

    HAL_TSP_VQ_Enable(TRUE);
    return TRUE;

}

MS_BOOL HAL_TSP_VQ_Buffer(MS_U32 vqId, MS_PHYADDR u32BaseAddr, MS_U32 u32VQ_PktNum)
{
    switch(vqId)
    {
        case 0:
            REG32_W(&_RegCtrl->VQ0_BASE, (u32BaseAddr >> MIU_BUS));
            REG16_W(&_RegCtrl->VQ0_SIZE, u32VQ_PktNum);
            break;
        case 1:
            REG32_W(&_RegCtrl->VQ1_Base, (u32BaseAddr >> MIU_BUS));
            REG16_W(&_RegCtrl->VQ1_Size,  u32VQ_PktNum);
            break;
        case 2:
            REG32_W(&_RegCtrl->VQ2_Base, (u32BaseAddr >> MIU_BUS));
            REG16_W(&_RegCtrl->VQ2_Size, u32VQ_PktNum);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_TSP_VQ_Block_Dis(MS_U32 vqId,MS_BOOL bDis)
{
    if(bDis == TRUE)
    {
        switch(vqId)
        {
            case 0:
                REG16_SET(&_RegCtrl->reg160E, TSP_VQTX0_BLOCK_DIS);
                break;
            case 1:
                REG16_SET(&_RegCtrl->reg160E, TSP_VQTX1_BLOCK_DIS);
                break;
            case 2:
                REG16_SET(&_RegCtrl->reg160E, TSP_VQTX2_BLOCK_DIS);
                break;
            default:
                return FALSE;
        }
    }
    else
    {
        switch(vqId)
        {
            case 0:
                REG16_CLR(&_RegCtrl->reg160E, TSP_VQTX0_BLOCK_DIS);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->reg160E, TSP_VQTX1_BLOCK_DIS);
                break;
            case 2:
                REG16_CLR(&_RegCtrl->reg160E, TSP_VQTX2_BLOCK_DIS);
                break;
            default:
                return FALSE;
        }
    }

    return TRUE;
}

void HAL_TSP_VQ_Enable(MS_BOOL bEn)
{
    if (bEn)
    {
          REG16_SET(&_RegCtrl->HW2_Config3, TSP_VQ_EN|TSP_VQ2PINGPONG_EN);
    }
    else
    {
          REG16_CLR(&_RegCtrl->HW2_Config3, TSP_VQ_EN|TSP_VQ2PINGPONG_EN);
    }
}
void HAL_TSP_VQ_Reset(MS_U32 vqId, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(vqId)
        {
            case 0:
                REG16_SET(&_RegCtrl->VQ0_CTRL, TSP_VQ0_RESET);
                break;
            case 1:
                REG16_SET(&_RegCtrl->VQ1_Config, TSP_VQ1_RESET);
                break;
            case 2:
                REG16_SET(&_RegCtrl->VQ2_Config, TSP_VQ2_RESET);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(vqId)
        {
            case 0:
                REG16_CLR(&_RegCtrl->VQ0_CTRL, TSP_VQ0_RESET);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->VQ1_Config, TSP_VQ1_RESET);
                break;
            case 2:
                REG16_CLR(&_RegCtrl->VQ2_Config, TSP_VQ2_RESET);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_VQ_OverflowInt_En(MS_U32 vqId, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(vqId)
        {
            case 0:
                REG16_SET(&_RegCtrl->VQ0_CTRL, TSP_VQ0_OVERFLOW_INT_EN);
                break;
            case 1:
                REG16_SET(&_RegCtrl->VQ1_Config, TSP_VQ1_OVF_INT_EN);
                break;
            case 2:
                REG16_SET(&_RegCtrl->VQ2_Config, TSP_VQ2_OVF_INT_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(vqId)
        {
            case 0:
                REG16_CLR(&_RegCtrl->VQ0_CTRL, TSP_VQ0_OVERFLOW_INT_EN);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->VQ1_Config, TSP_VQ1_OVF_INT_EN);
                break;
            case 2:
                REG16_CLR(&_RegCtrl->VQ2_Config, TSP_VQ2_OVF_INT_EN);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_VQ_OverflowInt_Clr(MS_U32 vqId, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(vqId)
        {
            case 0:
                REG16_SET(&_RegCtrl->VQ0_CTRL, TSP_VQ0_CLR_OVERFLOW_INT);
                break;
            case 1:
                REG16_SET(&_RegCtrl->VQ1_Config, TSP_VQ1_CLR_OVF_INT);
                break;
            case 2:
                REG16_SET(&_RegCtrl->VQ2_Config, TSP_VQ2_CLR_OVF_INT);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(vqId)
        {
            case 0:
                REG16_CLR(&_RegCtrl->VQ0_CTRL, TSP_VQ0_CLR_OVERFLOW_INT);
                break;
            case 1:
                REG16_CLR(&_RegCtrl->VQ1_Config, TSP_VQ1_CLR_OVF_INT);
                break;
            case 2:
                REG16_CLR(&_RegCtrl->VQ2_Config, TSP_VQ2_CLR_OVF_INT);
                break;
            default:
                break;
        }
    }
}

void HAL_PVR_Init(MS_U32 u32PVREng, MS_U32 pktDmxId)
{
    switch(u32PVREng)
    {
        case 0:
            // PVR 1
            REG16_SET(&(_RegCtrl->reg15b8), TSP_PVR1_PINGPONG);
            REG16_W(&(_RegCtrl->FIFO_Src), (REG16_R(&(_RegCtrl->FIFO_Src)) & ~TSP_PVR1_SRC_MASK) | (((MS_U16)pktDmxId) << TSP_PVR1_SRC_SHIFT));
            break;
        case 1:
            // PVR 2
            REG32_SET(&(_RegCtrl->PVR2_Config), TSP_PVR2_REG_PINGPONG_EN);
            REG16_W(&(_RegCtrl->FIFO_Src), (REG16_R(&(_RegCtrl->FIFO_Src)) & ~TSP_PVR2_SRC_MASK_L) | ((((MS_U16)pktDmxId) & 0x01) << TSP_PVR2_SRC_SHIFT_L));
            REG16_W(&(_RegCtrl->PCR_Cfg), (REG16_R(&(_RegCtrl->PCR_Cfg)) & ~TSP_PVR2_SRC_MASK_H) | ((((MS_U16)pktDmxId) >> 1) << TSP_PVR2_SRC_SHIFT_H));
            break;
        default:
            break;
    }
}

void HAL_PVR_Exit(MS_U32 u32PVREng)
{
    switch(u32PVREng)
    {
        case 0:
            //reset pvr control registers
            REG16_CLR(&(_RegCtrl->reg15b8), TSP_PVR1_PINGPONG);
            REG16_CLR(&(_RegCtrl->FIFO_Src), TSP_PVR1_SRC_MASK);

            //reset write address
            REG16_SET(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_FLUSH);
            REG16_CLR(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_FLUSH);
            break;
        case 1:
            //reset pvr control registers
            REG32_CLR(&(_RegCtrl->PVR2_Config), TSP_PVR2_REG_PINGPONG_EN);
            REG16_CLR(&(_RegCtrl->FIFO_Src), TSP_PVR2_SRC_MASK_L);
            REG16_CLR(&(_RegCtrl->PCR_Cfg), TSP_PVR2_SRC_MASK_H);

            //reset write address
            REG32_SET(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_RST_WADR);
            REG32_CLR(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_RST_WADR);
            break;
        default:
            break;
    }

    //reset time-stamp
    HAL_PVR_SetPVRTimeStamp(u32PVREng,0);

}

void HAL_PVR_Start(MS_U32 u32PVREng)
{
    switch(u32PVREng)
    {
        case 0:
            //reset write address
            REG16_SET(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_FLUSH);
            REG16_CLR(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_FLUSH);

            //enable string to miu
            REG16_SET(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_ENABLE);
            break;
        case 1:
            //reset write address
            REG32_SET(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_RST_WADR);
            REG32_CLR(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_RST_WADR);

            //enable string to miu
            REG32_SET(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_EN);
            break;
        default:
            break;
    }
}

void HAL_PVR_Stop(MS_U32 u32PVREng)
{
    switch(u32PVREng)
    {
        case 0:
            REG16_CLR(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_ENABLE);
            break;
        case 1:
            REG32_CLR(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_EN);
            break;
        default:
            break;
    }
}

void HAL_PVR_Pause(MS_U32 u32PVREng , MS_BOOL bPause)
{
    if(bPause)
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_SET(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_PAUSE);
                break;
            case 1:
                REG32_SET(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_PAUSE);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_CLR(&(_RegCtrl->Hw_PVRCfg), TSP_HW_CFG4_PVR_PAUSE);
                break;
            case 1:
                REG32_CLR(&(_RegCtrl->PVR2_Config), TSP_PVR2_STR2MIU_PAUSE);
                break;
            default:
                break;
        }
    }
}

void HAL_PVR_RecPid(MS_U32 u32PVREng , MS_BOOL bSet)
{
    if(bSet)
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_SET(&(_RegCtrl->reg15b4), TSP_PVR_PID_BYPASS);
                REG16_CLR(&(_RegCtrl->PVRConfig), TSP_PVR1_REC_ALL_EN);
                break;
            case 1:
                REG16_SET(&(_RegCtrl->reg15b4), TSP_PVR_PID_BYPASS2);
                REG16_CLR(&(_RegCtrl->PVRConfig), TSP_PVR2_REC_ALL_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_CLR(&(_RegCtrl->reg15b4), TSP_PVR_PID_BYPASS);
                REG16_SET(&(_RegCtrl->PVRConfig), TSP_PVR1_REC_ALL_EN);
                break;
            case 1:
                REG16_CLR(&(_RegCtrl->reg15b4), TSP_PVR_PID_BYPASS2);
                REG16_SET(&(_RegCtrl->PVRConfig), TSP_PVR2_REC_ALL_EN);
                break;
            default:
                break;
        }
    }
}

void HAL_PVR_RecNull(MS_BOOL bSet)
{
    if(bSet == TRUE)
    {
        REG16_SET(&(_RegCtrl->PVRConfig), TSP_REC_NULL);
    }
    else
    {
        REG16_CLR(&(_RegCtrl->PVRConfig), TSP_REC_NULL);
    }
}

void HAL_PVR_SetBuf(MS_U32 u32PVREng , MS_U32 u32StartAddr0, MS_U32 u32BufSize0, MS_U32 u32StartAddr1, MS_U32 u32BufSize1)
{
    MS_U32 u32EndAddr0 = u32StartAddr0 + u32BufSize0;
    MS_U32 u32EndAddr1 = u32StartAddr1 + u32BufSize1;
    //MS_U32  u32Temp;
    switch(u32PVREng)
    {
        case 0:
            //head1
            REG32_W(&_RegCtrl->TsRec_Head, (u32StartAddr0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            //end1
            REG32_W(&(_RegCtrl->TsRec_Tail), (u32EndAddr0 >> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            //mid1
            REG32_W(&(_RegCtrl->TsRec_Mid_PVR1_WPTR), (u32StartAddr0>>MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            //head2
            REG32_W(&_RegCtrl->Str2mi_head2pvr1, (u32StartAddr1>> MIU_BUS) & TSP_HW_PVR1_BUF_HEAD2_MASK);
            //end2
            REG32_W(&(_RegCtrl->Str2mi_tail2pvr1), (u32EndAddr1 >> MIU_BUS) &TSP_HW_PVR1_BUF_TAIL2_MASK);
            //mid2
            REG32_W(&(_RegCtrl->Str2mi_mid2pvr1), (u32StartAddr1>>MIU_BUS) & TSP_HW_PVR1_BUF_MID2_MASK);
            break;
        case 1:
            //head1
            REG32_W(&_RegCtrl->Str2mi_head1_pvr2, (u32StartAddr0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            //end1
            REG32_W(&_RegCtrl->Str2mi_tail1_pvr2, (u32EndAddr0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            //mid1
            REG32_W(&(_RegCtrl->Str2mi_mid1_wptr_pvr2), (u32StartAddr0>>MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            //head2
            REG32_W(&_RegCtrl->Str2mi_head2_pvr2, (u32StartAddr1>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            //end2
            REG32_W(&_RegCtrl->Str2mi_tail2_pvr2, (u32EndAddr1>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            //mid2
            REG32_W(&(_RegCtrl->Str2mi_mid2_pvr2), (u32StartAddr1>>MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            break;
        default:
            break;
    }
}

void HAL_PVR_SetStr2Miu_StartAddr(MS_U32 u32PVREng , MS_U32 u32StartAddr0, MS_U32 u32StartAddr1)
{
    //MS_U32  u32Temp;
    switch(u32PVREng)
    {
        case 0:
            //head1
            REG32_W(&_RegCtrl->TsRec_Head, (u32StartAddr0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            //head2
            REG32_W(&_RegCtrl->Str2mi_head2pvr1, (u32StartAddr1>> MIU_BUS) & TSP_HW_PVR1_BUF_HEAD2_MASK);
            break;
        case 1:
            //head1
            REG32_W(&_RegCtrl->Str2mi_head1_pvr2, (u32StartAddr0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            //head2
            REG32_W(&_RegCtrl->Str2mi_head2_pvr2, (u32StartAddr1>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            break;
        default:
            break;
    }
}

void HAL_PVR_SetStr2Miu_MidAddr(MS_U32 u32PVREng , MS_U32 u32MidAddr0, MS_U32 u32MidAddr1)
{
    switch(u32PVREng)
    {
        case 0:
            //mid1
            REG32_W(&(_RegCtrl->TsRec_Mid_PVR1_WPTR), (u32MidAddr0>>4) & TSP_STR2MI2_ADDR_MASK);

            //mid2
            REG32_W(&(_RegCtrl->Str2mi_mid2pvr1), (u32MidAddr1>>4) & TSP_HW_PVR1_BUF_MID2_MASK);
            break;
        case 1:
            //mid1
            REG32_W(&(_RegCtrl->Str2mi_mid1_wptr_pvr2), (u32MidAddr0>>4) & TSP_STR2MI2_ADDR_MASK);

            //mid2
            REG32_W(&(_RegCtrl->Str2mi_mid2_pvr2), (u32MidAddr1>>4) & TSP_STR2MI2_ADDR_MASK);
            break;
        default:
            break;
    }
}

void HAL_PVR_SetStr2Miu_EndAddr(MS_U32 u32PVREng , MS_U32 u32EndAddr0, MS_U32 u32EndAddr1)
{
    switch(u32PVREng)
    {
        case 0:
            //end1
            REG32_W(&(_RegCtrl->TsRec_Tail), (u32EndAddr0 >> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            //end2
            REG32_W(&(_RegCtrl->Str2mi_tail2pvr1), (u32EndAddr1 >> MIU_BUS) &TSP_HW_PVR1_BUF_TAIL2_MASK);
            break;
        case 1:
            //end1
            REG32_W(&_RegCtrl->Str2mi_tail1_pvr2, (u32EndAddr0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            //end2
            REG32_W(&_RegCtrl->Str2mi_tail2_pvr2, (u32EndAddr1>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            break;
        default:
            break;
    }
}

MS_U32 HAL_PVR_GetWritePtr(MS_U32 u32PVREng)
{
    switch(u32PVREng)
    {
        case 0:
            return (REG32_R(&_RegCtrl->TsRec_Mid_PVR1_WPTR) << MIU_BUS);
            break;
        case 1:
            return (REG32_R(&_RegCtrl->Str2mi_mid1_wptr_pvr2) << MIU_BUS);
            break;
        default:
            break;
    }
    return 0;
}


void HAL_PVR_GetEngSrc(MS_U32 u32EngDst, TSP_SRC_SEQ *eSrc)
{
    MS_U16 u16Value = 0;

    switch(u32EngDst)
    {
        case 0:
            // PVR 1
            *eSrc = ((REG16_R(&(_RegCtrl->FIFO_Src)) & TSP_PVR1_SRC_MASK) >> TSP_PVR1_SRC_SHIFT);
            break;
        case 1:
            // PVR 2
            u16Value = (REG16_R(&(_RegCtrl->FIFO_Src)) & TSP_PVR2_SRC_MASK_L)>> TSP_PVR2_SRC_SHIFT_L;
            u16Value |= ((REG16_R(&(_RegCtrl->PCR_Cfg)) & TSP_PVR2_SRC_MASK_H) << 1);
            *eSrc = (TSP_SRC_SEQ)u16Value;
            break;
        default:
            break;
    }
}


//   kaiser Only!!!
FILEENG_SEQ HAL_TSP_FilePath2Tsif_Mapping(MS_U32 u32FileEng)
{
    switch (u32FileEng)
    {
        case 0: //File in Eng 0
            return E_FILEENG_TSIF1;
        case 1: //File in Eng 1
            return E_FILEENG_TSIF2;
        case 2: //File in Eng 2
            return E_FILEENG_TSIF0;
        default:
            return E_FILEENG_INVALID;
    }
}

// @NOTE for backward competible when calling flowset pvr
//   kaiser Only!!!
TSP_SRC_SEQ HAL_TSP_Eng2PktDmx_Mapping(MS_U32 u32Eng)
{
    switch(u32Eng)
    {
        case 0:
            return E_TSP_SRC_PKTDMX0;
        case 1:
            return E_TSP_SRC_PKTDMX1;
        case 2:
            return E_TSP_SRC_PKTDMX2;
        default:
            return E_TSP_SRC_INVALID;
    }
}

//   kaiser Only!!!
MS_U32 HAL_TSP_TsifMapping(TSP_HAL_TSIF eTSIF, MS_BOOL bFileIn)
{
    MS_U32  u32PhyTSIF = 0;
    switch (eTSIF)
    {
        case E_TSP_HAL_TSIF_0:
            if(bFileIn)
                u32PhyTSIF = TSP_TSIF1; // @NOTE tsif1 is used for playback0 file-in
            else
                u32PhyTSIF = TSP_TSIF0;
            break;
        case E_TSP_HAL_TSIF_1:
            if(bFileIn)
                u32PhyTSIF = TSP_TSIF2;
            else
                u32PhyTSIF = TSP_TSIF2;
            break;
        case E_TSP_HAL_TSIF_2:
            if(bFileIn)
                u32PhyTSIF = TSP_TSIF0;
            else
                u32PhyTSIF = TSP_TSIF1;
            break;
        case E_TSP_HAL_TSIF_PVR0:
            u32PhyTSIF = TSP_TSIF0;
            break;
        case E_TSP_HAL_TSIF_PVR1:
            u32PhyTSIF = TSP_TSIF1;
            break;
        case E_TSP_HAL_TSIF_PVR2:
            u32PhyTSIF = TSP_TSIF2;
            break;
       default:
            printf("[PVR ERROR][%s][%d] mapping TSIF error\n",__FUNCTION__,__LINE__);
            break;
    }

    return u32PhyTSIF;
}


//   kaiser Only!!!
TSP_PIDFLT_SRC HAL_TSP_PktDmx2FltSrc_Mapping(TSP_SRC_SEQ eSrc)
{
    // @NOTE we map hw pkt dmx setting to api layer flow at this function

    TSP_PIDFLT_SRC ePidFltSrc = E_TSP_PIDFLT_INVALID;

    switch (eSrc)
    {
        case E_TSP_SRC_PKTDMX0:
            ePidFltSrc = E_TSP_PIDFLT_LIVE0;
            break;
        case E_TSP_SRC_PKTDMX1:
            ePidFltSrc = E_TSP_PIDFLT_FILE0;
            break;
        case E_TSP_SRC_PKTDMX2:
            ePidFltSrc = E_TSP_PIDFLT_LIVE1;
            break;
        default:
            // @TODO add assert
            printf ("[TSP_ERR][%s][%d] Wrong Engine Source!!\n", __FUNCTION__,__LINE__);
            break;
    }

    return ePidFltSrc;
}

FILEENG_SEQ HAL_TSP_GetDefaultFileinEng(void)
{
    return E_FILEENG_TSIF1;
}

MS_U32 HAL_TSP_PidFltDstMapping(TSP_PIDFLT_DST eDstType, MS_U32 u32Eng)
{
    if(eDstType == E_TSP_PIDFLT_DST_VIDEO)
    {
        switch(u32Eng)
        {
            case 0:
                return TSP_PIDFLT_OUT_VFIFO;
            case 1:
                return TSP_PIDFLT_OUT_VFIFO3D;
            default:
                printf("[TSP ERROR][%s][%d] mapping Vfifo eng error\n", __FUNCTION__, __LINE__);
                return 0;
        }
    }
    else if(eDstType == E_TSP_PIDFLT_DST_AUDIO)
    {
        switch(u32Eng)
        {
            case 0:
                return TSP_PIDFLT_OUT_AFIFO;
            case 1:
                return TSP_PIDFLT_OUT_AFIFO2;
            case 2:
                return TSP_PIDFLT_OUT_AFIFO3;
            case 3:
                return TSP_PIDFLT_OUT_AFIFO4;
            default:
                printf("[TSP ERROR][%s][%d] mapping Afifo eng error\n", __FUNCTION__, __LINE__);
                return 0;
        }
    }
    else if(eDstType == E_TSP_PIDFLT_DST_PVR)
    {
        switch(u32Eng)
        {
            case 0:
                return TSP_PIDFLT_OUT_PVR1;
            case 1:
                return TSP_PIDFLT_OUT_PVR2;
            default:
                printf("[TSP ERROR][%s][%d] mapping PVR eng error\n", __FUNCTION__, __LINE__);
                return 0;
        }
    }
    else
    {
        printf("[TSP ERROR][%s][%d] pid filter destination type error\n", __FUNCTION__, __LINE__);
        return 0;
    }
}

MS_U32 HAL_TSP_Tsif2Fq_Mapping(MS_U32 u32Tsif)   //TODO: fiq mux
{
    MS_U32  u32Fq = 0;

    switch (u32Tsif)
    {
        case 0:
        case 1:
        case 2:
            u32Fq = 0;
            break;
       default:
            printf("[PVR ERROR][%s][%d] mapping TSIF:%u error\n",__FUNCTION__,__LINE__,(unsigned int)u32Tsif);
            break;
    }

    return u32Fq;
}

TSP_TS_PAD HAL_TSP_3WirePadMapping(MS_U8 u8Pad3WireId)
{
    switch(u8Pad3WireId)
    {
        case 0:
            return E_TSP_TS_PAD_EXT3;
        case 1:
            return E_TSP_TS_PAD_EXT4;
        case 2:
            return E_TSP_TS_PAD_EXT5;
        case 3:
            return E_TSP_TS_PAD_EXT6;
        default:
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[PVR ERROR][%s][%d] mapping 3WirePad:%u error\n",__FUNCTION__,__LINE__,u8Pad3WireId));
            return E_TSP_TS_PAD_INVALID;
    }
}

void HAL_PVR_SetStrPacketMode(MS_U32 u32PVREng , MS_BOOL bSet)
{
    if(bSet)
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_SET(&_RegCtrl->reg160C, TSP_RECORD192_EN);
                break;
            case 1:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_PVR2_PKT192_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_CLR(&_RegCtrl->reg160C, TSP_RECORD192_EN);
                break;
            case 1:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_PVR2_PKT192_EN);
                break;
            default:
                break;
        }
    }
}

MS_U32 HAL_PVR_GetPVRTimeStamp(MS_U32 u32PVREng)
{
    MS_U32 u32lpcr = 0;
    switch(u32PVREng)
    {
        case 0:
            REG16_CLR(&_RegCtrl->reg160C, TSP_PVR1_LPCR1_RLD);

            u32lpcr = REG32_R(&_RegCtrl->PVR1_LPcr1);

            REG16_SET(&_RegCtrl->reg160C, TSP_PVR1_LPCR1_RLD);

            return u32lpcr;
        case 1:
            REG32_CLR(&_RegCtrl->PVR2_Config, TSP_PVR2_LPCR1_RLD);

            u32lpcr = REG32_R(&_RegCtrl->PVR2_LPCR1);

            REG32_SET(&_RegCtrl->PVR2_Config, TSP_PVR2_LPCR1_RLD);
            return u32lpcr;
        default:
            break;
    }
    return 0;
}

void HAL_PVR_SetPVRTimeStamp(MS_U32 u32PVREng , MS_U32 u32Stamp)
{
    switch(u32PVREng)
    {
        case 0:
            REG16_SET(&_RegCtrl->reg160C, TSP_PVR1_LPCR1_WLD);

            REG32_W(&_RegCtrl->PVR1_LPcr1, u32Stamp);

            REG16_CLR(&_RegCtrl->reg160C, TSP_PVR1_LPCR1_WLD);
            break;
        case 1:
            REG32_SET(&_RegCtrl->PVR2_Config, TSP_PVR2_LPCR1_WLD);

            REG32_W(&_RegCtrl->PVR2_LPCR1, u32Stamp);

            REG32_CLR(&_RegCtrl->PVR2_Config, TSP_PVR2_LPCR1_WLD);
            break;
        default:
            break;
    }
}

void HAL_PVR_SetPVRTimeStamp_Stream(MS_U32 u32PVREng , MS_U32 u32Stamp)
{
    switch(u32PVREng)
    {
        case 0:
            REG16_SET((&_RegCtrl2->CFG_13), CFG_13_LPCR_WLD0);

            REG32_W(&_RegCtrl2->CFG_56_57, u32Stamp);

            REG16_CLR((&_RegCtrl2->CFG_13), CFG_13_LPCR_WLD0);
            break;
        case 1:
            REG16_SET((&_RegCtrl2->CFG_13), CFG_13_LPCR_WLD1);

            REG32_W(&_RegCtrl2->CFG_58_59, u32Stamp);

            REG16_CLR((&_RegCtrl2->CFG_13), CFG_13_LPCR_WLD1);
            break;
        default:
            break;
    }
}

void HAL_PVR_Alignment_Enable(MS_U32 u32PVREng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_SET(&_RegCtrl->HW2_Config3, TSP_PVR1_ALIGN_EN);
                break;
            case 1:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_PVR2_PVR_ALIGN_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_CLR(&_RegCtrl->HW2_Config3, TSP_PVR1_ALIGN_EN);
                break;
            case 1:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_PVR2_PVR_ALIGN_EN);
                break;
            default:
                break;
        }
    }
}

void HAL_PVR_FlushData(MS_U32 u32PVREng)
{
    switch(u32PVREng)
    {
        case 0:
            REG16_SET(&_RegCtrl3->CFG3_39,HW4_CFG39_FLUSH_PVR_DATA);
            REG16_CLR(&_RegCtrl3->CFG3_39,HW4_CFG39_FLUSH_PVR_DATA);
            break;
        case 1:
            REG16_SET(&_RegCtrl3->CFG3_39,HW4_CFG39_FLUSH_PVR1_DATA);
            REG16_CLR(&_RegCtrl3->CFG3_39,HW4_CFG39_FLUSH_PVR1_DATA);
            break;
        default:
            break;
    }
}

void HAL_PVR_Skip_Scrmb(MS_U32 u32PVREng,MS_BOOL bSkip)
{
    if(bSkip)
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_SET(&_RegCtrl3->CFG3_37, HW4_CFG37_MASK_SCR_PVR1_EN);
                break;
            case 1:
                REG16_SET(&_RegCtrl3->CFG3_37, HW4_CFG37_MASK_SCR_PVR2_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_CLR(&_RegCtrl3->CFG3_37, HW4_CFG37_MASK_SCR_PVR1_EN);
                break;
            case 1:
                REG16_CLR(&_RegCtrl3->CFG3_37, HW4_CFG37_MASK_SCR_PVR2_EN);
                break;
            default:
                break;
        }
    }
}


void HAL_PVR_Block_Dis(MS_U32 u32PVREng,MS_BOOL bDisable)
{
    if(bDisable)
    {
        switch(u32PVREng)
        {
            case 0:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_PVR1_BLOCK_DIS);
                break;
            case 1:
                REG32_SET(&_RegCtrl->PVR2_Config, TSP_PVR2_BLOCK_DIS);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_PVR1_BLOCK_DIS);
                break;
            case 1:
                REG32_CLR(&_RegCtrl->PVR2_Config, TSP_PVR2_BLOCK_DIS);
                break;
            default:
                break;
        }

    }
}

void HAL_PVR_BurstLen(MS_U32 u32PVREng,MS_U16 u16BurstMode)
{
    switch(u32PVREng)
    {
        case 0:
            REG16_MSK_W(&_RegCtrl->reg15b8, TSP_BURST_LEN_MASK, (u16BurstMode << TSP_BURST_LEN_SHIFT));
            break;
        case 1:
            REG32_MSK_W(&_RegCtrl->PVR2_Config, TSP_PVR2_BURST_LEN_MASK, (u16BurstMode << TSP_PVR2_BURST_LEN_SHIFT));
            break;
        default:
            break;
    }
}

void HAL_PVR_TimeStamp_Sel(MS_U32 u32PVREng, MS_BOOL bLocal_Stream)
{
    if (u32PVREng>=TSP_PVRENG_NUM)
    {
        return;
    }

    if (bLocal_Stream) //Stream
    {
        REG16_SET((&_RegCtrl2->CFG_12), CFG_12_TIMESTAMP_SEL_PVR1 << (u32PVREng));
    }
    else //local
    {
        REG16_CLR((&_RegCtrl2->CFG_12), CFG_12_TIMESTAMP_SEL_PVR1 << (u32PVREng));
    }
}

void HAL_PVR_PauseTime_En(MS_U32 u32PVREng,MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_CLR(&_RegCtrl3->CFG3_35, HW4_CFG35_BYPASS_TIMESTAMP_SEL1);
                break;
            case 1:
                REG16_CLR(&_RegCtrl3->CFG3_35, HW4_CFG35_BYPASS_TIMESTAMP_SEL2);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_SET(&_RegCtrl3->CFG3_35, HW4_CFG35_BYPASS_TIMESTAMP_SEL1);
                break;
            case 1:
                REG16_SET(&_RegCtrl3->CFG3_35, HW4_CFG35_BYPASS_TIMESTAMP_SEL2);
                break;
            default:
                break;
        }
    }
}

void HAL_PVR_SetPauseTime(MS_U32 u32PVREng,MS_U32 u32PauseTime)
{
    switch(u32PVREng)
    {
        case 0:
            REG32_W(&_RegCtrl3->CFG3_44_45, u32PauseTime);
            break;
        case 1:
            REG32_W(&_RegCtrl3->CFG3_46_47, u32PauseTime);
            break;
        default:
            break;
    }
}

void HAL_PVR_TimeStamp_Stream_En(MS_U32 u32pktDmxId, MS_BOOL bEnable)
{
    if (bEnable)
    {
        REG16_SET((&_RegCtrl2->CFG_13), CFG_13_LPCR_EN0 << (u32pktDmxId*2));
    }
    else
    {
        REG16_CLR((&_RegCtrl2->CFG_13), CFG_13_LPCR_EN0 << (u32pktDmxId*2));
    }
}

void HAL_PVR_MOBF_Enable(MS_U32 u32PVREng, MS_BOOL bEnable, MS_U32 u32Key)
{
    switch(u32PVREng)
    {
        case 0:
            REG16_W(&_RegCtrl->MOBF_PVR1_Index[0], (u32Key & TSP_MOBF_PVR1_INDEX_MASK));
            break;
        case 1:
            REG16_W(&_RegCtrl->MOBF_PVR2_Index[0], (u32Key & TSP_MOBF_PVR2_INDEX_MASK));
            break;
        default:
            break;
    }
}

/*
void HAL_PVR_SetTSIF(MS_U32 u32PVREng , MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP)
{
    if (bPara)
    {
        REG16_SET(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_PARA_SEL);
    }
    else
    {
        REG16_CLR(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_PARA_SEL);
    }

    if (bExtSync)
    {
        REG16_SET(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_EXT_SYNC);
    }
    else
    {
        REG16_CLR(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_EXT_SYNC);
    }

    if (bDataSWP)
    {
        REG16_SET(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_DATA_SWP);
    }
    else
    {
        REG16_CLR(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_DATA_SWP);
    }
}

void HAL_PVR_RecAtSync_Dis(MS_U32 u32PVREng, MS_BOOL bDis)
{
    if(bDis)
    {
        REG16_SET(&(_PVRReg[u32PVREng]->PVR_Ctrl1), PVR_REC_AT_SYNC_DIS);
    }
    else
    {
        REG16_CLR(&(_PVRReg[u32PVREng]->PVR_Ctrl1), PVR_REC_AT_SYNC_DIS);
    }
}

void HAL_PVR_SetDataSwap(MS_U32 u32PVREng, MS_BOOL bEn)
{
    if(bEn)
    {
        REG16_SET(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_DATA_SWP);
    }
    else
    {
        REG16_CLR(&(_PVRReg[u32PVREng]->PVR_Ctrl0), PVR_TSIF_DATA_SWP);
    }
}
*/

MS_U32 HAL_TSP_HCMD_GetInfo(MS_U32 u32Type)
{
    MS_U32              u32Data;

//reserved
    switch (u32Type)
    {

    case INFO_FW_VERSION:
        REG32_W(&_RegCtrl->MCU_Data0, INFO_FW_VERSION);
    break;
    case INFO_FW_DATE:
        REG32_W(&_RegCtrl->MCU_Data0, INFO_FW_DATE);
        break;
    default:
        REG32_W(&_RegCtrl->MCU_Data0, INFO_FW_VERSION);
        break;

    }

    REG32_W(&_RegCtrl->MCU_Cmd, TSP_MCU_CMD_INFO);

    _delay(10); // supposed TSP is able to respond in 10us
    u32Data = REG32_R(&_RegCtrl->MCU_Data1);
    REG32_W(&_RegCtrl->MCU_Cmd, 0); // write null to clear

    return u32Data;
}

MS_BOOL HAL_TSP_HCMD_BufRst(MS_U32 u32Value)
{
 //   MS_U32              u32Data;

    REG32_W(&_RegCtrl->MCU_Data0 , u32Value);
    REG32_W(&_RegCtrl->MCU_Cmd   , TSP_MCU_CMD_BUFRST);
    _delay(10); // supposed TSP is able to respond in 10us

    return TRUE;
}

MS_U32 HAL_TSP_HCMD_Read(MS_U32 u32Addr)
{
    MS_U32              u32Data;

    REG32_W(&_RegCtrl->MCU_Data0, u32Addr);
    REG32_W(&_RegCtrl->MCU_Cmd, TSP_MCU_CMD_READ);

    _delay(10); // supposed TSP is able to respond in 10us
    u32Data = REG32_R(&_RegCtrl->MCU_Data1);
    REG32_W(&_RegCtrl->MCU_Cmd, 0); // write null to clear

    return u32Data;
}


MS_BOOL HAL_TSP_HCMD_Write(MS_U32 u32Addr, MS_U32 u32Value)
{
    MS_U32              u32Data;

    REG32_W(&_RegCtrl->MCU_Data0, u32Addr);
    REG32_W(&_RegCtrl->MCU_Data1, u32Value);
    REG32_W(&_RegCtrl->MCU_Cmd, TSP_MCU_CMD_WRITE);

    _delay(10); // supposed TSP is able to respond in 10us
    u32Data = REG32_R(&_RegCtrl->MCU_Data1);
    REG32_W(&_RegCtrl->MCU_Cmd, 0); // write null to clear

    return TRUE;
}


MS_BOOL HAL_TSP_HCMD_Alive(void)
{
    MS_U32              u32Data;

    REG32_W(&_RegCtrl->MCU_Data1, 0);
    REG32_W(&_RegCtrl->MCU_Cmd, TSP_MCU_CMD_ALIVE);  //@TODO check FW HCMD
    _delay(10); // supposed TSP is able to respond in 10us
    u32Data = REG32_R(&_RegCtrl->MCU_Data1);
    REG32_W(&_RegCtrl->MCU_Cmd, 0); // write null to clear

    return (u32Data == TSP_MCU_DATA_ALIVE)? TRUE : FALSE;
}

void HAL_TSP_HCMD_SET(MS_U32 mcu_cmd, MS_U32 mcu_data0, MS_U32 mcu_data1)
{
    REG32_W(&_RegCtrl->MCU_Data0, mcu_data0);
    REG32_W(&_RegCtrl->MCU_Data1, mcu_data1);
    REG32_W(&_RegCtrl->MCU_Cmd,   mcu_cmd);
}

void HAL_TSP_HCMD_GET(MS_U32* pmcu_cmd, MS_U32* pmcu_data0, MS_U32* pmcu_data1)
{
    *pmcu_cmd   = REG32_R(&_RegCtrl->MCU_Cmd);
    *pmcu_data0 = REG32_R(&_RegCtrl->MCU_Data0);
    *pmcu_data1 = REG32_R(&_RegCtrl->MCU_Data1);
}

void HAL_TSP_HCMD_SecRdyInt_Disable(MS_U32 FltId ,MS_BOOL bDis)
{
    MS_U32              u32Data = bDis ;

    REG32_W(&_RegCtrl->MCU_Data0, FltId);
    REG32_W(&_RegCtrl->MCU_Data1,u32Data);
    REG32_W(&_RegCtrl->MCU_Cmd, TSP_MCU_CMD_SECRDYINT_DISABLE); // @TODO add HCMD list here
    _delay(10); // supposed TSP is able to respond in 10us
    REG32_W(&_RegCtrl->MCU_Cmd, 0); // write null to clear

    return ;
}

MS_U32 HAL_TSP_HCMD_Dbg(MS_U32 u32Enable)
{
    MS_U32              u32Data;

    REG32_W(&_RegCtrl->MCU_Data0, u32Enable);
    REG32_W(&_RegCtrl->MCU_Cmd, TSP_MCU_CMD_DBG);

    _delay(10); // supposed TSP is able to respond in 10us
    u32Data = REG32_R(&_RegCtrl->MCU_Data1);
    REG32_W(&_RegCtrl->MCU_Cmd, 0); // write null to clear

    return REG32_R(&_RegCtrl->MCU_Data1);
}

MS_U32 HAL_TSP_GetDBGStatus(MS_U16 u16Sel)
{
    REG16_CLR(&_RegCtrl->PKT_CNT, TSP_DBG_SEL_MASK);
    REG16_SET(&_RegCtrl->PKT_CNT, ((u16Sel << TSP_DBG_SEL_SHIFT) & TSP_DBG_SEL_MASK));

    return REG32_R(&_RegCtrl->TSP_Debug);
}

void HAL_TSP_INT_Enable(MS_U32 u32Mask)
{
    // Low  byte for bank 0x15 7e bit[0:7]
        /*
            7: audio/video packet error
            6: DMA read done
            5: HK_INT_FORCE.            // it's trigure bit is at bank 15 44 bit[15]
            4: TSP_FILE_RP meets TSP_FILE_TAIL.
            3: TSP_FILE_RP meets TSP_FILE_MID.
            2: HK_INT_FORCE.            // it's trigure bit is at bank 15 39 bit[15]
            1: STR2MI_WADR meets STR2MI_MID.
            0: STR2MI_WADR meets STR2MI_TAIL."
        */
    REG16_SET(&_RegCtrl->HwInt_Stat, TSP_INT_EN_MASK & u32Mask);

    // High byte for bank 0x16 6c bit[0:7]
        /*
            [7] : PVR2 meet_tail  or PVR2_meet_mid
            [6] : vq0, vq1, vq2, vq3 overflow interrupt
            [5] : all DMA write address not in the protect zone interrupt
            [4] : PVR_cb meet the mid or PVR_cb meet the tail
            [3] : pcr filter 0 update finish
            [2] : pcr filter 1 update finish
            [1: 0] : reserved
        */
    REG16_SET(&_RegCtrl->SwInt_Stat1_L, (TSP_HWINT2_EN_MASK & (u32Mask >> 8)) | TSP_HWINT2_STATUS_MASK);
}


void HAL_TSP_INT_Disable(MS_U32 u32Mask)
{
    REG16_CLR(&_RegCtrl->HwInt_Stat, TSP_INT_EN_MASK & u32Mask);
    //REG16_CLR(&_RegCtrl->SwInt_Stat1_L, TSP_HWINT2_EN_MASK & (u32Mask >> 8));
    REG16_W(&_RegCtrl->SwInt_Stat1_L,
    (REG16_R(&_RegCtrl->SwInt_Stat1_L) & ~(TSP_HWINT2_EN_MASK & (u32Mask >> 8))) |
        TSP_HWINT2_STATUS_MASK);
}


void HAL_TSP_INT_ClrHW(MS_U32 u32Mask)
{
    REG16_CLR(&_RegCtrl->HwInt_Stat, (u32Mask & 0x00FF) << 8);
    //REG16_CLR(&_RegCtrl->SwInt_Stat1_L, u32Mask & 0xFF00);
    REG16_W(&_RegCtrl->SwInt_Stat1_L,
    (REG16_R(&_RegCtrl->SwInt_Stat1_L) & (~TSP_HWINT2_STATUS_MASK)) |
        (~(u32Mask & 0xFF00) & TSP_HWINT2_STATUS_MASK));
}


MS_U32 HAL_TSP_INT_GetHW(void)
{
    MS_U32              status;

    status = (MS_U32)(((REG16_R(&_RegCtrl->SwInt_Stat1_L) & TSP_HWINT2_STATUS_MASK) >> TSP_HWINT2_STATUS_SHIFT)<<8);

    status |= (MS_U32)((REG16_R(&_RegCtrl->HwInt_Stat) & TSP_HWINT_STATUS_MASK) >> TSP_HWINT_STATUS_SHIFT);

    return (status & 0xFFFF);
}


void HAL_TSP_INT_ClrSW(void)
{
    REG32_W(&_RegCtrl->SwInt_Stat, 0);
}


MS_U32 HAL_TSP_INT_GetSW(void)
{
    return REG32_R(&_RegCtrl->SwInt_Stat);
}

void HAL_TSP_Set_RcvBuf_Src(MS_U32 bufIdx, MS_U32 inputSrc)
{
    switch (bufIdx)
    {
        case 0:
            REG16_MSK_W(&_RegCtrl2->CFG_11, CFG_11_RECEIVE_BUF0_SRC, inputSrc<<(bufIdx * 2));
             break;
        case 1:
            REG16_MSK_W(&_RegCtrl2->CFG_11, CFG_11_RECEIVE_BUF1_SRC, inputSrc<<(bufIdx * 2));
            break;
        case 2:
            REG16_MSK_W(&_RegCtrl2->CFG_11, CFG_11_RECEIVE_BUF2_SRC, inputSrc<<(bufIdx * 2));
            break;
        default:
            break;
    }
}

MS_U32 HAL_RASP_Set_Source(MS_U32 u32RASPEng, MS_U32 pktDmxId)
{
    return 0;
}

MS_U32 HAL_RASP_Get_Source(MS_U32 u32RASPEng, TSP_SRC_SEQ *eSrc)
{
    return 0;
}

void HAL_TSP_SPD_Bypass_En(MS_BOOL bByPassEn)
{
    if (bByPassEn)
    {
        REG16_SET(&_RegCtrl3->CFG3_52, CFG3_52_SPD_TSIF0_BYPASS | CFG3_52_SPD_TSIF1_BYPASS | CFG3_52_SPD_TSIF2_BYPASS | CFG3_52_SPD_TSIF3_BYPASS);
    }
    else
    {
        REG16_CLR(&_RegCtrl3->CFG3_52, CFG3_52_SPD_TSIF0_BYPASS | CFG3_52_SPD_TSIF1_BYPASS | CFG3_52_SPD_TSIF2_BYPASS | CFG3_52_SPD_TSIF3_BYPASS);
    }
}

//@TODO
#if 1
void HAL_TSP_FileIn_SPDConfig(MS_U32 tsif, MS_BOOL CTR_mode)
{
    if(CTR_mode == TRUE)
    {
        printf("SPD CTR mode = %p\n",&(_RegCtrl7[tsif].CFG7_05));
        REG16_SET(&(_RegCtrl7[tsif].CFG7_05), CFG7_05_CTR_MODE_SPD_FILEIN); //set CTR mode enable
        REG16_W(&(_RegCtrl7[tsif].CFG7_00_03[0]), 0x0000);                  //set counter IV
        REG16_W(&(_RegCtrl7[tsif].CFG7_00_03[1]), 0x0000);
        REG16_W(&(_RegCtrl7[tsif].CFG7_00_03[2]), 0x0000);
        REG16_W(&(_RegCtrl7[tsif].CFG7_00_03[3]), 0x0000);
        REG16_W(&(_RegCtrl7[tsif].CFG7_04), CFG7_04_CTR_IV_SPD_MAX_1K);     //set counter IV max vld
        REG16_SET(&(_RegCtrl7[tsif].CFG7_05), CFG7_05_LOAD_INIT_CNT_SPD);   //load counter IV
    }

    switch(tsif)
    {
        case 0: REG16_MSK_W(&_RegCtrl3->CFG3_43, HW4_CFG43_SRC_AES_FI_KEY_MASK, HW4_CFG43_SRC_AES_FI0_KEY << HW4_CFG43_SRC_AES_FI_KEY_SHIFT);
                REG16_W(&_RegCtrl3->CFG3_68_6F[0], 0x0000);             //file-in SPD key
                REG16_W(&_RegCtrl3->CFG3_68_6F[1], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[2], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[3], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[4], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[5], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[6], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[7], 0x1111);
                REG16_SET(&_RegCtrl3->CFG3_3A,HW4_CFG3A_LOAD_SPD_KEY0);
                REG16_CLR(&_RegCtrl3->CFG3_52,CFG3_52_SPD_TSIF0_BYPASS);//bypass SPD
                REG16_SET(&_RegCtrl3->CFG3_1E,CFG3_1E_TSIF0_SPD_RESET); //TSIF SPD reset
                REG16_CLR(&_RegCtrl3->CFG3_1E,CFG3_1E_TSIF0_SPD_RESET);
                break;
        case 1: REG16_MSK_W(&_RegCtrl3->CFG3_43, HW4_CFG43_SRC_AES_FI_KEY_MASK, HW4_CFG43_SRC_AES_FI1_KEY << HW4_CFG43_SRC_AES_FI_KEY_SHIFT);
                /*
                REG16_W(&_RegCtrl3->CFG3_68_6F[0], 0x2222);             //file-in SPD key
                REG16_W(&_RegCtrl3->CFG3_68_6F[1], 0x2222);
                REG16_W(&_RegCtrl3->CFG3_68_6F[2], 0x2222);
                REG16_W(&_RegCtrl3->CFG3_68_6F[3], 0x2222);
                REG16_W(&_RegCtrl3->CFG3_68_6F[4], 0x3333);
                REG16_W(&_RegCtrl3->CFG3_68_6F[5], 0x3333);
                REG16_W(&_RegCtrl3->CFG3_68_6F[6], 0x3333);
                REG16_W(&_RegCtrl3->CFG3_68_6F[7], 0x3333);
                */
                REG16_W(&_RegCtrl3->CFG3_68_6F[0], 0x0000);             //file-in SPD key
                REG16_W(&_RegCtrl3->CFG3_68_6F[1], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[2], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[3], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[4], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[5], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[6], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[7], 0x1111);
                REG16_SET(&_RegCtrl3->CFG3_3A,HW4_CFG3A_LOAD_SPD_KEY1);
                REG16_CLR(&_RegCtrl3->CFG3_52,CFG3_52_SPD_TSIF1_BYPASS);//bypass SPD
                REG16_SET(&_RegCtrl3->CFG3_1E,CFG3_1E_TSIF1_SPD_RESET); //TSIF SPD reset
                REG16_CLR(&_RegCtrl3->CFG3_1E,CFG3_1E_TSIF1_SPD_RESET);
                break;
        case 2: REG16_MSK_W(&_RegCtrl3->CFG3_43, HW4_CFG43_SRC_AES_FI_KEY_MASK, HW4_CFG43_SRC_AES_FI2_KEY << HW4_CFG43_SRC_AES_FI_KEY_SHIFT);
                /*
                REG16_W(&_RegCtrl3->CFG3_68_6F[0], 0x4444);             //file-in SPD key
                REG16_W(&_RegCtrl3->CFG3_68_6F[1], 0x4444);
                REG16_W(&_RegCtrl3->CFG3_68_6F[2], 0x4444);
                REG16_W(&_RegCtrl3->CFG3_68_6F[3], 0x4444);
                REG16_W(&_RegCtrl3->CFG3_68_6F[4], 0x5555);
                REG16_W(&_RegCtrl3->CFG3_68_6F[5], 0x5555);
                REG16_W(&_RegCtrl3->CFG3_68_6F[6], 0x5555);
                REG16_W(&_RegCtrl3->CFG3_68_6F[7], 0x5555);
                */
                REG16_W(&_RegCtrl3->CFG3_68_6F[0], 0x0000);             //file-in SPD key
                REG16_W(&_RegCtrl3->CFG3_68_6F[1], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[2], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[3], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_68_6F[4], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[5], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[6], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_68_6F[7], 0x1111);
                REG16_SET(&_RegCtrl3->CFG3_3A,HW4_CFG3A_LOAD_SPD_KEY2);
                REG16_CLR(&_RegCtrl3->CFG3_52,CFG3_52_SPD_TSIF2_BYPASS);//bypass SPD
                REG16_SET(&_RegCtrl3->CFG3_1E,CFG3_1E_TSIF2_SPD_RESET); //TSIF SPD reset
                REG16_CLR(&_RegCtrl3->CFG3_1E,CFG3_1E_TSIF2_SPD_RESET);
                break;
        default:printf("Not Support !!\n");
                break;
    }
}
#endif

// for TSO
void HAL_TSP_PidFlt_SetTSOFlt(MS_U32 fltId, MS_U32 u32TSOEng, MS_BOOL bEn)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);
    if(bEn)
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_TSOFLT_MASK) | (((1 << u32TSOEng) << TSP_PIDFLT_TSOFLT_SHFT) & TSP_PIDFLT_TSOFLT_MASK));
    }
    else
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~(((1 << u32TSOEng) << TSP_PIDFLT_TSOFLT_SHFT) & TSP_PIDFLT_TSOFLT_MASK)));
    }
}

MS_BOOL HAL_TSP_GetCaps(TSP_HAL_CAP_TYPE eCap, MS_U32 *pu32CapInfo)
{
    switch (eCap)
    {
        case E_TSP_HAL_CAP_TYPE_PIDFLT_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_PIDFLT_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_SECFLT_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_SECFLT_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_SECBUF_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_SECBUF_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_RECENG_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_RECENG_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_RECFLT_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_RECFLT_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_TSIF_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_TSIF_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_DEMOD_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_DEMOD_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_TSPAD_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_TSPAD_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_VQ_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_VQ_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_CAFLT_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_CAFLT_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_CAKEY_NUM :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_CAKEY_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_SECBUF_ALIGN :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_SECBUF_ALIGN;
            break;
        case E_TSP_HAL_CAP_TYPE_PVR_ALIGN:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_PVR_ALIGN;
            break;
        case E_TSP_HAL_CAP_TYPE_FW_ALIGN :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_FW_ALIGN;
            break;
        case E_TSP_HAL_CAP_TYPE_VQ_ALIGN :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_VQ_ALIGN;
            break;
        case E_TSP_HAL_CAP_TYPE_VQ_PITCH :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_VQ_PITCH;
            break;
        case E_TSP_HAL_CAP_TYPE_MMFI_AUDIO_FILTER_NUM:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_MMFI_AUDIO_FILTER_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_MMFI_V3D_FILTER_NUM:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_MMFI_V3D_FILTER_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_HW_TYPE:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_HW_TYPE;
            break;
        case E_TSP_HAL_CAP_TYPE_RECFLT_IDX :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_RECFLT_IDX;
            break;
        case E_TSP_HAL_CAP_TYPE_PCRFLT_START_IDX :
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_PCRFLT_START_IDX;
            break;
        case E_TSP_HAL_CAP_TYPE_VFIFO_NUM:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_VFIFO_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_AFIFO_NUM:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_AFIFO_NUM;
            break;
        case E_TSP_HAL_CAP_TYPE_HWPCR_SUPPORT:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_HWPCR_SUPPORT;
            break;
        case E_TSP_HAL_CAP_FIQ_NUM:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_FIQ_NUM;
            break;
        case E_TSP_HAL_CAP_FW_BUF_SIZE:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_FW_BUF_SIZE;
            break;
        case E_TSP_HAL_CAP_FW_BUF_RANGE:
            *(pu32CapInfo) = TSP_FW_BUF_LOW_BUD;
            *(pu32CapInfo + 1) = TSP_FW_BUF_UP_BUD;
            break;
        case E_TSP_HAL_CAP_VQ_BUF_RANGE:
            *(pu32CapInfo) = TSP_VQ_BUF_LOW_BUD;
            *(pu32CapInfo + 1) = TSP_VQ_BUF_UP_BUD;
            break;
        case E_TSP_HAL_CAP_SEC_BUF_RANGE:
            *(pu32CapInfo) = TSP_SEC_BUF_LOW_BUD;
            *(pu32CapInfo + 1) = TSP_SEC_BUF_UP_BUD;
            break;
        default:
            *(pu32CapInfo) = E_TSP_HAL_CAP_VAL_NULL;
            return FALSE;
    }
    return TRUE;
}

static TSP_TS_PAD _HAL_TSP_PadMapping(MS_U16 u16Pad)
{
    MS_U16 u16Ts2_Mode = (TSP_TOP_REG(REG_TOP_TS_PADMUX_MODE) >> REG_TOP_TS2MODE_SHIFT) & REG_TOP_TS2MODE_MASK;

    switch (u16Pad)
    {
        case REG_TOP_TS_SRC_EXT0:
            return E_TSP_TS_PAD_EXT0;
        case REG_TOP_TS_SRC_EXT1:
            return E_TSP_TS_PAD_EXT1;
        case REG_TOP_TS_SRC_EXT2:
            return E_TSP_TS_PAD_EXT2;
        case REG_TOP_TS_SRC_EXT3:
            if(u16Ts2_Mode == REG_TOP_TS2MODE_3WIRED)
            {
                return E_TSP_TS_PAD_EXT0_3WIRE;
            }
            else
            {
                return E_TSP_TS_PAD_EXT3;
            }
        case REG_TOP_TS_SRC_EXT4:
            if(u16Ts2_Mode == REG_TOP_TS2MODE_3WIRED)
            {
                return E_TSP_TS_PAD_EXT1_3WIRE;
            }
            else
            {
                return E_TSP_TS_PAD_EXT4;
            }
        case REG_TOP_TS_SRC_EXT5:
            if(u16Ts2_Mode == REG_TOP_TS2MODE_3WIRED)
            {
                return E_TSP_TS_PAD_EXT2_3WIRE;
            }
            else
            {
                return E_TSP_TS_PAD_EXT5;
            }
        case REG_TOP_TS_SRC_EXT6:
            if(u16Ts2_Mode == REG_TOP_TS2MODE_3WIRED)
            {
                return E_TSP_TS_PAD_EXT3_3WIRE;
            }
            else
            {
                return E_TSP_TS_PAD_EXT6;
            }
        case REG_TOP_TS_SRC_TSO0:
            return E_TSP_TS_PAD_TSOUT0;

        default:
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[%s][%s][%d] UnSupported Pad Enum: %d !\n",__FILE__,__FUNCTION__,__LINE__,u16Pad));
            return E_TSP_TS_PAD_INVALID;
    }
}

void HAL_TSP_GetTSIF_Status(MS_U32 u32TsIfId, TSP_TS_PAD* pePad, MS_U16* pu16Clk, MS_BOOL* pbClkInv, MS_BOOL* pbExtSync, MS_BOOL* pbParl)
{
    MS_U16 u16pad, u16clk;
    *pbExtSync = FALSE;
    *pbParl = FALSE;
    *pbClkInv = FALSE;
    *pePad = 0;
    *pu16Clk = 0;

    switch (u32TsIfId)
    {
        case 0:
            u16pad = TSP_TOP_REG(REG_TOP_TS0_MUX) & ((REG_TOP_TS_SRC_MASK) << REG_TOP_TS0_SHIFT);
            u16pad >>= REG_TOP_TS0_SHIFT;
            *pePad = _HAL_TSP_PadMapping(u16pad);
            u16clk = TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) & ((REG_CLKGEN0_TS_SRC_MASK)<<(REG_CLKGEN0_TS0_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            u16clk >>= (REG_CLKGEN0_TS0_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT);
            *pu16Clk = u16clk;
            *pbExtSync = ((REG16_R(&_RegCtrl->Hw_Config0) & TSP_HW_CFG0_TSIF0_EXTSYNC) == TSP_HW_CFG0_TSIF0_EXTSYNC);
            *pbParl = ((REG16_R(&_RegCtrl->Hw_Config0) & TSP_HW_CFG0_TSIF0_PARL) == TSP_HW_CFG0_TSIF0_PARL);
            *pbClkInv =(((TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) >> REG_CLKGEN0_TS0_SHIFT)& REG_CLKGEN0_TS_INVERT) == REG_CLKGEN0_TS_INVERT);
            break;
        case 1:
            u16pad = TSP_TOP_REG(REG_TOP_TS1_MUX) & ((REG_TOP_TS_SRC_MASK) << REG_TOP_TS1_SHIFT);
            u16pad >>= REG_TOP_TS1_SHIFT;
            *pePad = _HAL_TSP_PadMapping(u16pad);
            u16clk = TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) & ((REG_CLKGEN0_TS_SRC_MASK)<<(REG_CLKGEN0_TS1_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            u16clk >>= (REG_CLKGEN0_TS1_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT);
            *pu16Clk = u16clk;
            *pbExtSync = ((REG16_R(&_RegCtrl->Hw_Config2) & TSP_HW_CFG2_TSIF1_EXTSYNC) == TSP_HW_CFG2_TSIF1_EXTSYNC);
            *pbParl = ((REG16_R(&_RegCtrl->Hw_Config2) & TSP_HW_CFG2_TSIF1_PARL) == TSP_HW_CFG2_TSIF1_PARL);
            *pbClkInv =(((TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) >> REG_CLKGEN0_TS1_SHIFT)& REG_CLKGEN0_TS_INVERT) == REG_CLKGEN0_TS_INVERT);
            break;
        case 2:
            u16pad = TSP_TOP_REG(REG_TOP_TS2_MUX) & ((REG_TOP_TS_SRC_MASK) << REG_TOP_TS2_SHIFT);
            u16pad >>= REG_TOP_TS2_SHIFT;
            *pePad = _HAL_TSP_PadMapping(u16pad);
            u16clk = TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) & ((REG_CLKGEN0_TS_SRC_MASK)<<(REG_CLKGEN0_TS2_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            u16clk >>= (REG_CLKGEN0_TS2_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT);
            *pu16Clk = u16clk;
            *pbExtSync = ((REG32_R(&_RegCtrl->PVR2_Config) & TSP_EXT_SYNC_SEL2) == TSP_EXT_SYNC_SEL2);
            *pbParl = ((REG32_R(&_RegCtrl->PVR2_Config) & TSP_P_SEL2) == TSP_P_SEL2);
            *pbClkInv =(((TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) >> REG_CLKGEN0_TS2_SHIFT)& REG_CLKGEN0_TS_INVERT) == REG_CLKGEN0_TS_INVERT);
            break;
        default:
            break;
    }

}

void HAL_TSP_PktDmx_RmDupAVPkt(MS_BOOL bEnable)
{
    if(bEnable)
    {
            REG16_SET(&_RegCtrl->reg15b8, TSP_REMOVE_DUP_AV_PKT);
    }
    else
    {
            REG16_CLR(&_RegCtrl->reg15b8, TSP_REMOVE_DUP_AV_PKT);
    }
}

void HAL_TSP_TEI_RemoveErrorPkt(TSP_HAL_TEI_RmPktType eHalPktType, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch (eHalPktType)
       {
          case E_TSP_HAL_TEI_REMOVE_AUDIO_PKT:
              REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_APES_ERR_RM_EN);
              break;
          case E_TSP_HAL_TEI_REMOVE_VIDEO_PKT:
              REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_VPES_ERR_RM_EN);
              break;
          default:
              break;
        }
    }
    else
    {
       switch (eHalPktType)
       {
          case E_TSP_HAL_TEI_REMOVE_AUDIO_PKT:
              REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_APES_ERR_RM_EN);
              break;
          case E_TSP_HAL_TEI_REMOVE_VIDEO_PKT:
              REG16_CLR(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_VPES_ERR_RM_EN);
              break;
          default:
              break;
        }
    }
}

void HAL_TSP_TEI_SKIP(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch (tsIf)
       {
          case 0:
              REG16_SET(&_RegCtrl->reg15b8, TSP_TEI_SKIPE_PKT_PID0);
              break;
          case 1:
              REG16_SET(&_RegCtrl->reg15b8, TSP_TEI_SKIPE_PKT_PID1);
              break;
          case 2:
              REG32_SET(&_RegCtrl->PVR2_Config, TSP_TEI_SKIP_PKT2);
              break;
          default:
              break;
        }
    }
    else
    {
       switch (tsIf)
       {
          case 0:
              REG16_CLR(&_RegCtrl->reg15b8, TSP_TEI_SKIPE_PKT_PID0);
              break;
          case 1:
              REG16_CLR(&_RegCtrl->reg15b8, TSP_TEI_SKIPE_PKT_PID1);
              break;
          case 2:
              REG32_CLR(&_RegCtrl->PVR2_Config, TSP_TEI_SKIP_PKT2);
              break;
          default:
              break;
        }
    }

}

void HAL_TSP_DisPKTCnt_Clear(TSP_DST_SEQ eFltType)
{
    switch (eFltType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V_CLR);
            break;
        case E_TSP_DST_FIFO_VIDEO3D:
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V3D_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V3D_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_A_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_A_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_AD_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_AD_CLR);
            break;
        default :
            printf("[%s][%s][%d] Clear AVPktNum Error\n",__FILE__,__FUNCTION__,__LINE__);
            break;
    }
}

static MS_PHY _HAL_TSP_MIU_OFFSET(MS_PHY Phyaddr)
{
    #ifdef HAL_MIU2_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU2_BASE)
        return (MS_PHY)HAL_MIU2_BASE;
    else
    #endif  //HAL_MIU2_BUS_BASE
    #ifdef HAL_MIU1_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU1_BASE)
        return (MS_PHY)HAL_MIU1_BASE;
    else
    #endif //HAL_MIU1_BUS_BASE
        return (MS_PHY)HAL_MIU0_BASE;
}

void HAL_TSP_OR_Address_Protect_En(MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        REG16_SET(&_RegCtrl->reg160C,TSP_ORZ_DMAW_PROT_EN);
    }
    else
    {
        REG16_CLR(&_RegCtrl->reg160C,TSP_ORZ_DMAW_PROT_EN);
    }
}

void HAL_TSP_OR_Address_Protect(MS_PHY u32AddrH, MS_PHY u32AddrL)
{
    MS_PHY  phyMiuOffset = _HAL_TSP_MIU_OFFSET(u32AddrL);

    REG32_W(&_RegCtrl->ORZ_DMAW_LBND,(MS_U32)(((u32AddrL - phyMiuOffset) >> MIU_BUS) & TSP_ORZ_DMAW_LBND_MASK));
    REG32_W(&_RegCtrl->ORZ_DMAW_UBND,(MS_U32)(((u32AddrH - phyMiuOffset) >> MIU_BUS) & TSP_ORZ_DMAW_UBND_MASK));
}

void HAL_TSP_SEC_Address_Protect_En(MS_BOOL bEn)
{

    if(bEn == TRUE)
    {
        REG16_SET(&_RegCtrl->reg15b4,TSP_SEC_CB_PVR2_DAMW_PROTECT_EN);
    }
    else
    {
        REG16_CLR(&_RegCtrl->reg15b4,TSP_SEC_CB_PVR2_DAMW_PROTECT_EN);
    }
}

void HAL_TSP_SEC_Address_Protect(MS_U8 u8SecID, MS_PHY u32AddrH, MS_PHY u32AddrL)
{
    MS_PHY  phyMiuOffset = _HAL_TSP_MIU_OFFSET(u32AddrL);
    MS_U32  u32LBnd = (MS_U32)(((u32AddrL - phyMiuOffset) >> MIU_BUS) & TSP_DMAW_BND_MASK);
    MS_U32  u32UBnd = (MS_U32)(((u32AddrH - phyMiuOffset) >> MIU_BUS) & TSP_DMAW_BND_MASK);

    switch(u8SecID)
    {
        case 0:
            REG32_W(&_RegCtrl->DMAW_LBND0,u32LBnd);
            REG32_W(&_RegCtrl->DMAW_UBND0,u32UBnd);
            break;
        case 1:
            REG32_W(&_RegCtrl->DMAW_LBND1,u32LBnd);
            REG32_W(&_RegCtrl->DMAW_UBND1,u32UBnd);
            break;
        default:
            break;
    }
}

void HAL_TSP_PVR_Address_Protect_En(MS_U32 u32PVREng,MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_SET(&_RegCtrl3->CFG3_73, CFG3_73_PVR1_DMAW_PROTECT_EN);
                break;
            case 1:
                REG16_SET(&_RegCtrl3->CFG3_73, CFG3_73_PVR2_DMAW_PROTECT_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32PVREng)
        {
            case 0:
                REG16_CLR(&_RegCtrl3->CFG3_73, CFG3_73_PVR1_DMAW_PROTECT_EN);
                break;
            case 1:
                REG16_CLR(&_RegCtrl3->CFG3_73, CFG3_73_PVR2_DMAW_PROTECT_EN);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_PVR_Address_Protect(MS_U32 u32PVREng,MS_PHY u32AddrH, MS_PHY u32AddrL)
{
    MS_PHY  phyMiuOffset = _HAL_TSP_MIU_OFFSET(u32AddrL);
    MS_U32  u32LBnd = (MS_U32)(((u32AddrL - phyMiuOffset) >> MIU_BUS) & TSP_PVR_MASK);
    MS_U32  u32UBnd = (MS_U32)(((u32AddrH - phyMiuOffset) >> MIU_BUS) & TSP_PVR_MASK);

    switch(u32PVREng)
    {
        case 0:
            REG32_W(&_RegCtrl->DMAW_LBND2, u32LBnd);
            REG32_W(&_RegCtrl->DMAW_UBND2, u32UBnd);
            break;
        case 1:
            REG32_W(&_RegCtrl->DMAW_LBND3, u32LBnd);
            REG32_W(&_RegCtrl->DMAW_UBND3, u32UBnd);
            break;
        default:
            break;
    }
}

void HAL_TSP_FILEIN_Address_Protect_En(FILEENG_SEQ eFileEng,MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_SET(&_RegCtrl3->CFG3_73, CFG3_73_FILEIN0_DMAR_PROTECT_EN);
                break;
            case E_FILEENG_TSIF1:
                REG16_SET(&_RegCtrl3->CFG3_73, CFG3_73_FILEIN1_DMAR_PROTECT_EN);
                break;
            case E_FILEENG_TSIF2:
                REG16_SET(&_RegCtrl3->CFG3_73, CFG3_73_FILEIN2_DMAR_PROTECT_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(eFileEng)
        {
            case E_FILEENG_TSIF0:
                REG16_CLR(&_RegCtrl3->CFG3_73, CFG3_73_FILEIN0_DMAR_PROTECT_EN);
                break;
            case E_FILEENG_TSIF1:
                REG16_CLR(&_RegCtrl3->CFG3_73, CFG3_73_FILEIN1_DMAR_PROTECT_EN);
                break;
            case E_FILEENG_TSIF2:
                REG16_CLR(&_RegCtrl3->CFG3_73, CFG3_73_FILEIN2_DMAR_PROTECT_EN);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_FILEIN_Address_Protect(FILEENG_SEQ eFileEng,MS_PHY u32AddrH, MS_PHY u32AddrL)
{
    MS_PHY  phyMiuOffset = _HAL_TSP_MIU_OFFSET(u32AddrL);
    MS_U32  u32LBnd = (MS_U32)(((u32AddrL - phyMiuOffset) >> MIU_BUS) & TSP_FILEIN_DMAR_BND_MASK);
    MS_U32  u32UBnd = (MS_U32)(((u32AddrH - phyMiuOffset) >> MIU_BUS) & TSP_FILEIN_DMAR_BND_MASK);

    switch(eFileEng)
    {
        case E_FILEENG_TSIF0:
            REG32_W(&_RegCtrl6->CFG6_30_31, u32LBnd);
            REG32_W(&_RegCtrl6->CFG6_32_33, u32UBnd);
            break;
        case E_FILEENG_TSIF1:
            REG32_W(&_RegCtrl6->CFG6_34_35, u32LBnd);
            REG32_W(&_RegCtrl6->CFG6_36_37, u32UBnd);
            break;
        case E_FILEENG_TSIF2:
            REG32_W(&_RegCtrl6->CFG6_38_39, u32LBnd);
            REG32_W(&_RegCtrl6->CFG6_3A_3B, u32UBnd);
            break;
        default:
            break;
    }
}

void HAL_TSP_MMFI_Address_Protect_En(MS_U32 u32MMFIEng,MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(u32MMFIEng)
        {
            case 0:
                REG16_SET(&_RegCtrl3->CFG3_73, CFG3_73_MMFI0_DMAR_PROTECT_EN);
                break;
            case 1:
                REG16_SET(&_RegCtrl3->CFG3_73, CFG3_73_MMFI1_DMAR_PROTECT_EN);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u32MMFIEng)
        {
            case 0:
                REG16_CLR(&_RegCtrl3->CFG3_73, CFG3_73_MMFI0_DMAR_PROTECT_EN);
                break;
            case 1:
                REG16_CLR(&_RegCtrl3->CFG3_73, CFG3_73_MMFI1_DMAR_PROTECT_EN);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_MMFI_Address_Protect(MS_U32 u32MMFIEng,MS_PHY u32AddrH, MS_PHY u32AddrL)
{
    MS_PHY  phyMiuOffset = _HAL_TSP_MIU_OFFSET(u32AddrL);
    MS_U32  u32LBnd = (MS_U32)(((u32AddrL - phyMiuOffset) >> MIU_BUS) & TSP_MMFI_DMAR_BND_MASK);
    MS_U32  u32UBnd = (MS_U32)(((u32AddrH - phyMiuOffset) >> MIU_BUS) & TSP_MMFI_DMAR_BND_MASK);

    switch(u32MMFIEng)
    {
        case 0:
            REG32_W(&_RegCtrl6->CFG6_48_49, u32LBnd);
            REG32_W(&_RegCtrl6->CFG6_4A_4B, u32UBnd);
            break;
        case 1:
            REG32_W(&_RegCtrl6->CFG6_4C_4D, u32LBnd);
            REG32_W(&_RegCtrl6->CFG6_4E_4F, u32UBnd);
            break;
        default:
            break;
    }
}


void HAL_TSP_SaveFltState(void)
{
    MS_U32              u32Size;
    TSP32 *             pRegEnd;
    TSP32 *             pReg;
    int                 i, j;

    for (i = 0; i < TSP_PIDFLT_NUM; i++)
    {
         _u32PidFltReg[i] = TSP32_IdrR(&(_REGPid0->Flt[i]));
         _u32PidDstReg[i] = TSP32_IdrR(&(_REGPid1->Flt[i]));
    }

    u32Size = ((MS_VIRT)&(((REG_SecFlt*)0)->_x50))/sizeof(TSP32);
    for (i = 0; i < TSP_SECFLT_NUM; i++)
    {
         pReg = (TSP32*)&(_REGSec->Flt[i]);
         pRegEnd = pReg + u32Size;
         j = 0;
         for ( ; pReg < pRegEnd; pReg++)
         {
             _u32SecReg[i*u32Size+j] = TSP32_IdrR(pReg);
             j++;
         }
    }
}


MS_BOOL HAL_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1, MS_U32* pData)
{
    //@TODO not implement yet
    return TRUE;
}

MS_BOOL HAL_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_BOOL bEnable)
{
    printf("[%s] not support!!\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL HAL_TSP_CAPVR_SPSEnable(MS_U32 u32Eng, MS_U16 u16CaPvrMode, MS_BOOL bEnable)
{
    MS_U16 u16value = REG16_R(&_RegCtrl3->CFG3_38);

    switch(u32Eng)
    {
        case 0:
            u16value &= ~(HW4_CFG38_CA_PVR1_SEL_MASK|HW4_CFG38_PKT192_SPS_EN1);
            break;
        case 1:
            u16value &= ~(HW4_CFG38_CA_PVR2_SEL_MASK|HW4_CFG38_PKT192_SPS_EN2);
            break;
        default:
            return FALSE;
    }

    if(bEnable)
    {
        switch(u32Eng)
        {
            case 0:
                u16value |= (u16CaPvrMode << HW4_CFG38_CA_PVR1_SEL_SHIFT);
                u16value |= HW4_CFG38_PKT192_SPS_EN1;
                break;
            case 1:
                u16value |= (u16CaPvrMode << HW4_CFG38_CA_PVR2_SEL_SHIFT);
                u16value |= HW4_CFG38_PKT192_SPS_EN2;
                break;
            default:
                return FALSE;
        }
    }

    REG16_W(&_RegCtrl3->CFG3_38, u16value);

    return TRUE;

}

//@TODO
#if 1
void HAL_TSP_PVR_SPSConfig(MS_U32 u32Eng, MS_BOOL CTR_mode)
{
    switch(u32Eng)
    {
        case 0: REG16_SET(&_RegCtrl3->CFG3_38,HW4_CFG38_PKT192_SPS_EN1);
                REG16_MSK_W(&_RegCtrl3->CFG3_43, HW4_CFG43_SRC_AES_PVR_KEY_MASK, HW4_CFG43_SRC_AES_PVR1_KEY);
                REG16_W(&_RegCtrl3->CFG3_60_67[0], 0x0000);//PVR SPS key
                REG16_W(&_RegCtrl3->CFG3_60_67[1], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_60_67[2], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_60_67[3], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_60_67[4], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_60_67[5], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_60_67[6], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_60_67[7], 0x1111);
                REG16_SET(&_RegCtrl3->CFG3_38,HW4_CFG38_LOAD_SPS_KEY1);
                break;
        case 1: REG16_SET(&_RegCtrl3->CFG3_38,HW4_CFG38_PKT192_SPS_EN2);
                REG16_MSK_W(&_RegCtrl3->CFG3_43, HW4_CFG43_SRC_AES_PVR_KEY_MASK, HW4_CFG43_SRC_AES_PVR2_KEY);
                /*
                REG16_W(&_RegCtrl3->CFG3_60_67[0], 0x2222);//PVR SPS key
                REG16_W(&_RegCtrl3->CFG3_60_67[1], 0x2222);
                REG16_W(&_RegCtrl3->CFG3_60_67[2], 0x2222);
                REG16_W(&_RegCtrl3->CFG3_60_67[3], 0x2222);
                REG16_W(&_RegCtrl3->CFG3_60_67[4], 0x3333);
                REG16_W(&_RegCtrl3->CFG3_60_67[5], 0x3333);
                REG16_W(&_RegCtrl3->CFG3_60_67[6], 0x3333);
                REG16_W(&_RegCtrl3->CFG3_60_67[7], 0x3333);
                */
                REG16_W(&_RegCtrl3->CFG3_60_67[0], 0x0000);//PVR SPS key
                REG16_W(&_RegCtrl3->CFG3_60_67[1], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_60_67[2], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_60_67[3], 0x0000);
                REG16_W(&_RegCtrl3->CFG3_60_67[4], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_60_67[5], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_60_67[6], 0x1111);
                REG16_W(&_RegCtrl3->CFG3_60_67[7], 0x1111);
                REG16_SET(&_RegCtrl3->CFG3_38,HW4_CFG38_LOAD_SPS_KEY2);
                break;
        default: printf("Not Support !!\n");
                 break;
    }

    if(CTR_mode == TRUE)
    {
        printf("SPS CTR mode = %p\n",&(_RegCtrl8[u32Eng].CFG8_05));
        REG16_SET(&(_RegCtrl8[u32Eng].CFG8_05),CFG8_05_CTR_MODE_SPS_PVR1);   //set CTR mode
        REG16_W(&(_RegCtrl8[u32Eng].CFG8_00_03[0]), 0x0000);                 //set counter IV
        REG16_W(&(_RegCtrl8[u32Eng].CFG8_00_03[1]), 0x0000);
        REG16_W(&(_RegCtrl8[u32Eng].CFG8_00_03[2]), 0x0000);
        REG16_W(&(_RegCtrl8[u32Eng].CFG8_00_03[3]), 0x0000);
        REG16_W(&(_RegCtrl8[u32Eng].CFG8_04), CFG8_04_CTR_IV_SPS_MAX_1K);    //set counter IV max vld
        REG16_SET(&(_RegCtrl8[u32Eng].CFG8_05),CFG8_05_LOAD_INIT_CNT_SPS1);  //load counter IV
    }
}
#endif

// u32GroupId: 0 -> filter0  ~ filter31
// u32GroupId: 1 -> filter32 ~ filter63
// u32GroupId: 2 -> filter64 ~ filter95
// u32GroupId: 3 -> filter96 ~ filter127
// u32GroupId: 4 -> filter128 ~ filter159
// u32GroupId: 5 -> filter160 ~ filter191


MS_BOOL HAL_DSCMB_GetStatus(MS_U32 u32PktDmx, MS_U32 u32GroupId, MS_U32 u32PidFltId, MS_U32 *pu32ScmbSts)
{
    MS_U16              u16PktDmxSrc;
    MS_U16              u16WordId;
    MS_U32              u32PIDFltMask = u32PidFltId;

    if(u32PidFltId != 0xFFFFFFFF)
    {
        u32PIDFltMask = (1 << (u32PidFltId & 0x1F));
    }

    u16PktDmxSrc = _CLR_(REG16_R(&_RegCtrl->reg15b8), TSP_MATCH_PID_SRC_MASK);

    switch(u32PktDmx)
    {
        case 0:
            u16PktDmxSrc = _SET_(u16PktDmxSrc, TSP_MATCH_PID_SRC_PKTDMX0 << TSP_MATCH_PID_SRC_SHIFT);
            break;
        case 1:
            u16PktDmxSrc = _SET_(u16PktDmxSrc, TSP_MATCH_PID_SRC_PKTDMX1 << TSP_MATCH_PID_SRC_SHIFT);
            break;
        case 2:
            u16PktDmxSrc = _SET_(u16PktDmxSrc, TSP_MATCH_PID_SRC_PKTDMX2 << TSP_MATCH_PID_SRC_SHIFT);
            break;
        case 3:
            u16PktDmxSrc = _SET_(u16PktDmxSrc, TSP_MATCH_PID_SRC_PKTDMX3 << TSP_MATCH_PID_SRC_SHIFT);
            break;
        default:
            break;
    }

    REG16_W(&_RegCtrl->reg15b8, u16PktDmxSrc);

    REG16_W(&_RegCtrl->PVRConfig, _SET_(REG16_R(&_RegCtrl->PVRConfig), TSP_MATCH_PID_LD)); // Set 1 to load match_pid_num

    // get status
    REG16_W(&_RegCtrl->PVRConfig, _CLR_(REG16_R(&_RegCtrl->PVRConfig), TSP_MATCH_PID_LD)); // freeze

    u16WordId =  _CLR_(REG16_R(&_RegCtrl->PVRConfig), TSP_MATCH_PID_SEL_MASK);

    if (u32GroupId < 6)
    {
        u16WordId = _SET_(u16WordId, u32GroupId << TSP_MATCH_PID_SEL_SHIFT);
        REG16_MSK_W(&_RegCtrl->PVRConfig,TSP_MATCH_PID_SEL_MASK,u16WordId);
    }
    else
    {
        printf("[TSP_ERR][%s][%d] Unsupported u32GroupId (%lu)!!!\n",__FUNCTION__, __LINE__, (long unsigned int)u32GroupId);
    }

    *pu32ScmbSts = _AND_(REG32_R(&_RegCtrl->TsPidScmbStatTsin), u32PIDFltMask);

    REG16_W(&_RegCtrl->PVRConfig, _SET_(REG16_R(&_RegCtrl->PVRConfig), TSP_MATCH_PID_LD)); // re-enable

    if(u32PIDFltMask != 0xFFFFFFFF)
    {
        *pu32ScmbSts = ((*pu32ScmbSts > 0) ? 1: 0);
    }

    return TRUE;
}

// -------------------------------------------------------------
// Merge Stream
// -------------------------------------------------------------
MS_BOOL HAL_TSP_PktConverter_SetSyncByte(MS_U8 u8Path, MS_U8 u8Id, MS_U8 *pu8SyncByte, MS_BOOL bSet)
{
    REG16 *SynReg=0;
    MS_U16 u16Mask = 0x00FF, u16Sync = 0, u16Shift = 0;

    switch(u8Path)
    {
        case TSP_TSIF0:
            SynReg = &(_RegCtrl6[0].SyncByte_tsif0[u8Id>>1]);
            break;
        case TSP_TSIF1:
            SynReg = &(_RegCtrl6[0].SyncByte_tsif1[u8Id>>1]);
            break;
        case TSP_TSIF2:
            SynReg = &(_RegCtrl6[0].SyncByte_tsif2[u8Id>>1]);
            break;
        default:
            return FALSE;
    }


    if(u8Id & 0x1)
    {
        u16Shift = 8;
    }

    if(bSet == TRUE)
    {
        u16Sync = (MS_U16)(*pu8SyncByte & 0xFF);
        REG16_W(SynReg,((REG16_R(SynReg) & ~(u16Mask << u16Shift)) | (u16Sync << u16Shift)));
    }
    else
    {
        u16Sync = (REG16_R(SynReg) & (u16Mask << u16Shift)) >> u16Shift;
        *pu8SyncByte = (MS_U8)u16Sync;
    }

    return TRUE;

}

MS_BOOL HAL_TSP_PktConverter_SetSrcId(MS_U8 u8Path, MS_U8 u8Id, MS_U8 *pu8SrcId, MS_BOOL bSet)
{
    REG16 *SrcIdReg =0;
    MS_U16 u16SrcId = 0, u16Mask = 0x000F, u16Shift = 0;

    switch(u8Path)
    {
        case TSP_TSIF0:
            SrcIdReg = &(_RegCtrl6[0].SourceId_tsif0[u8Id>>2]);
            break;
        case TSP_TSIF1:
            SrcIdReg = &(_RegCtrl6[0].SourceId_tsif1[u8Id>>2]);
            break;
        case TSP_TSIF2:
            SrcIdReg = &(_RegCtrl6[0].SourceId_tsif2[u8Id>>2]);
            break;
        default:
            return FALSE;
    }

    switch(u8Id & 0x3)
    {
        case 0x1:
            u16Shift = 4;
            u16SrcId <<= 4;
            u16Mask <<= 4;
            break;
        case 0x2:
            u16Shift = 8;
            u16SrcId <<= 8;
            u16Mask <<= 8;
            break;
        case 0x3:
            u16Shift = 12;
            u16SrcId <<= 12;
            u16Mask <<= 12;
            break;
    }

    if(bSet == TRUE)
    {
        u16SrcId = (MS_U16)(*pu8SrcId & 0xFF);
        REG16_W(SrcIdReg,((REG16_R(SrcIdReg) & ~(u16Mask << u16Shift)) | (u16SrcId << u16Shift)));
    }
    else
    {
        u16SrcId = (REG16_R(SrcIdReg) & (u16Mask << u16Shift)) >> u16Shift;
        *pu8SrcId = (MS_U8)u16SrcId;
    }

    return TRUE;
}

static void HAL_TSP_PktConverter_SetMXLPktHeaderLen(MS_U8 u8Path, MS_U8 u8PktHeaderLen)
{
    REG16 *PktConverterReg = &_RegCtrl6->pkt_converter[u8Path];

    REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_MXL_PKT_HEADER_MASK)) | ((u8PktHeaderLen << TSP_MXL_PKT_HEADER_SHIFT) & TSP_MXL_PKT_HEADER_MASK)));
}

MS_BOOL HAL_TSP_PktConverter_PktMode(MS_U8 u8Path, TSP_HAL_PKT_MODE ePktMode)
{
    REG16 *PktConverterReg = &_RegCtrl6->pkt_converter[u8Path];

    switch(ePktMode)
    {
        case E_TSP_HAL_PKT_MODE_NORMAL:
            REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_PKT_CONVERTER_MODE_MASK)) | TSP_PKT_188Mode));
            break;
        case E_TSP_HAL_PKT_MODE_CI:
            REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_PKT_CONVERTER_MODE_MASK)) | TSP_PKT_CIMode));
            break;
        case E_TSP_HAL_PKT_MODE_OPEN_CABLE:
            REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_PKT_CONVERTER_MODE_MASK)) | TSP_PKT_OpenCableMode));
            break;
        case E_TSP_HAL_PKT_MODE_ATS:
            REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_PKT_CONVERTER_MODE_MASK)) | TSP_PKT_ATSMode));
            break;
        case E_TSP_HAL_PKT_MODE_MXL_192:
            REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_PKT_CONVERTER_MODE_MASK)) | TSP_PKT_MxLMode));
            HAL_TSP_PktConverter_SetMXLPktHeaderLen(u8Path,4);
            break;
        case E_TSP_HAL_PKT_MODE_MXL_196:
            REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_PKT_CONVERTER_MODE_MASK)) | TSP_PKT_MxLMode));
            HAL_TSP_PktConverter_SetMXLPktHeaderLen(u8Path,8);
            break;
        case E_TSP_HAL_PKT_MODE_MXL_200:
            REG16_W(PktConverterReg,((REG16_R(PktConverterReg) & ~(TSP_PKT_CONVERTER_MODE_MASK)) | TSP_PKT_MxLMode));
            HAL_TSP_PktConverter_SetMXLPktHeaderLen(u8Path,12);
            break;
        default:
            printf("[TSP_ERR][%s][%d] Wrong PktConverter Packet Mode!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
    }

    return TRUE;
}

void HAL_TSP_PktConverter_ForceSync(MS_U8 u8Path, MS_BOOL bEnable)
{
    REG16 *PktConverterReg = &_RegCtrl6->pkt_converter[u8Path];

    if(bEnable)
    {
        REG16_W(PktConverterReg, _SET_(REG16_R(PktConverterReg), TSP_PKT_FORCE_SYNC_47)); // Set 1 to force sync byte be 0x47
    }
    else
    {
        REG16_W(PktConverterReg, _CLR_(REG16_R(PktConverterReg), TSP_PKT_FORCE_SYNC_47));
    }
}

void HAL_TSP_PidFlt_SetSrcId(MS_U32 fltId, MS_U32 u32SrcId)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);
    TSP32_IdrW(pPidFlt, ((TSP32_IdrR(pPidFlt) & ~(TSP_PIDFLT_SRCID_MASK)) | ((u32SrcId << TSP_PIDFLT_SRCID_SHIFT) & TSP_PIDFLT_SRCID_MASK)));
}

void HAL_TSP_PcrFlt_SetSrcId(MS_U32 pcrFltId, MS_U32 u32SrcId)
{
    switch(pcrFltId)
    {
        case 0:
            //src 0
            REG16_MSK_W(&_RegCtrl3->CFG3_72,CFG3_72_PIDFLT_PCR_SCR_ID_MASK,(u32SrcId << CFG3_72_PIDFLT_PCR0_SCR_ID_SHIFT));
            break;
        case 1:
            //src 1
            REG16_MSK_W(&_RegCtrl3->CFG3_72,CFG3_72_PIDFLT_PCR_SCR_ID_MASK<<CFG3_72_PIDFLT_PCR1_SCR_ID_SHIFT,(u32SrcId << CFG3_72_PIDFLT_PCR1_SCR_ID_SHIFT));

            break;
        default:
            break;
    }
}

void HAL_TSP_Reset_TSIF_MergeSetting(MS_U8 u8Path)
{
    MS_U8   u8Id;
    MS_U8   u8SyncByte;

    u8SyncByte = 0x47;
    for(u8Id = 0; u8Id < TSP_MERGESTREAM_NUM; u8Id++,u8SyncByte++)
    {
        HAL_TSP_PktConverter_SetSyncByte(u8Path, u8Id, &u8SyncByte, TRUE);
        HAL_TSP_PktConverter_SetSrcId(u8Path, u8Id, &u8Id, TRUE);
    }
    HAL_TSP_PktConverter_PktMode(u8Path,E_TSP_HAL_PKT_MODE_NORMAL);
}


//-------------------------------------------------------------------------------------------------
//  For Debug Table
//-------------------------------------------------------------------------------------------------

TSP_SRC_SEQ HAL_TSP_Debug_Flow2PktDmx_Mapping(TSP_HAL_FLOW eFlow)
{
    switch (eFlow)
    {
        case E_TSP_HAL_FLOW_LIVE0:
            return E_TSP_SRC_PKTDMX0;
        case E_TSP_HAL_FLOW_LIVE1:
            return E_TSP_SRC_PKTDMX2;
        case E_TSP_HAL_FLOW_LIVE2:
            return E_TSP_SRC_PKTDMX1;
        case E_TSP_HAL_FLOW_FILE0:
            return E_TSP_SRC_PKTDMX1;
        case E_TSP_HAL_FLOW_FILE1:
            return E_TSP_SRC_PKTDMX2;
        case E_TSP_HAL_FLOW_FILE2:
            return E_TSP_SRC_PKTDMX0;
        case E_TSP_HAL_FLOW_MMFI0:
            return E_TSP_SRC_MMFI0;
        case E_TSP_HAL_FLOW_MMFI1:
            return E_TSP_SRC_MMFI1;
        default:
            printf("[TSP_ERR][%s][%d] UnSupported Debug Flow : %d !!!\n",__FUNCTION__, __LINE__,eFlow);
            return E_TSP_SRC_INVALID;
    }
}

void HAL_TSP_Debug_LockPktCnt_Src(MS_U32 u32TsIf)
{
    MS_U16 u16TSIF = 0;;

    switch(u32TsIf)
    {
        case 0:
            u16TSIF = TSIF_SRC_SEL_TSIF0;
            break;
        case 1:
            u16TSIF = TSIF_SRC_SEL_TSIF1;
            break;
        case 2:
            u16TSIF = TSIF_SRC_SEL_TSIF2;
            break;
        default:
            printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
            break;
    }

    REG16_MSK_W(&_RegCtrl5->CFG5_7F,CFG5_7F_TSIF_SRC_SEL_MASK,(u16TSIF << CFG5_7F_TSIF_SRC_SEL_SHIFT));
}

void HAL_TSP_Debug_LockPktCnt_Load(MS_U32 u32TsIf,MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        switch(u32TsIf)
        {
            case 0:
                REG16_SET(&_RegCtrl5->CFG5_7A,CFG5_7A_LOCKED_PKT_CNT_0_LOAD);
                break;
            case 1:
                REG16_SET(&_RegCtrl5->CFG5_7A,CFG5_7A_LOCKED_PKT_CNT_1_LOAD);
                break;
            case 2:
                REG16_SET(&_RegCtrl5->CFG5_7A,CFG5_7A_LOCKED_PKT_CNT_2_LOAD);
                break;
            default:
                printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
                break;
        }
    }
    else
    {
        switch(u32TsIf)
        {
            case 0:
                REG16_CLR(&_RegCtrl5->CFG5_7A,CFG5_7A_LOCKED_PKT_CNT_0_LOAD);
                break;
            case 1:
                REG16_CLR(&_RegCtrl5->CFG5_7A,CFG5_7A_LOCKED_PKT_CNT_1_LOAD);
                break;
            case 2:
                REG16_CLR(&_RegCtrl5->CFG5_7A,CFG5_7A_LOCKED_PKT_CNT_2_LOAD);
                break;
            default:
                printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
                break;
        }
    }

}

MS_U16 HAL_TSP_Debug_LockPktCnt_Get(MS_U32 u32TsIf, MS_BOOL bLock)
{
    if(bLock)  // 188 mode
    {
        REG16_CLR(&_RegCtrl3->CFG3_37,HW4_CFG37_NON_188_CNT_MODE);
    }
    else       // Non 188 mode
    {
       REG16_SET(&_RegCtrl3->CFG3_37,HW4_CFG37_NON_188_CNT_MODE);
    }

    return REG16_R(&_RegCtrl5->Locked_PKT_Cnt);
}

void HAL_TSP_Debug_LockPktCnt_Clear(MS_U32 u32Tsif)
{
    switch (u32Tsif)
    {
        case 0 :
            REG16_SET(&_RegCtrl5->CFG5_7C,CFG5_7C_LOCKED_PKT_CNT_0_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7C,CFG5_7C_LOCKED_PKT_CNT_0_CLR);
            break;
        case 1:
            REG16_SET(&_RegCtrl5->CFG5_7C,CFG5_7C_LOCKED_PKT_CNT_1_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7C,CFG5_7C_LOCKED_PKT_CNT_1_CLR);
            break;
        case 2:
            REG16_SET(&_RegCtrl5->CFG5_7C,CFG5_7C_LOCKED_PKT_CNT_2_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7C,CFG5_7C_LOCKED_PKT_CNT_2_CLR);
            break;
        default :
            printf("[%s][%s][%d] UnSupported TSIF : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Tsif);
            break;
    }
}

void HAL_TSP_Debug_ClrSrcSel(TSP_SRC_SEQ eClrSrc)
{
    switch (eClrSrc)
    {
        case E_TSP_SRC_PKTDMX0:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F, CFG5_7F_CLR_SRC_MASK, CFG5_7F_CLR_SRC_PKTDMX0 << CFG5_7F_CLR_SRC_SHIFT);
            break;
        case E_TSP_SRC_PKTDMX1:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F, CFG5_7F_CLR_SRC_MASK, CFG5_7F_CLR_SRC_PKTDMX1 << CFG5_7F_CLR_SRC_SHIFT);
            break;
        case E_TSP_SRC_PKTDMX2:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F, CFG5_7F_CLR_SRC_MASK, CFG5_7F_CLR_SRC_PKTDMX2 << CFG5_7F_CLR_SRC_SHIFT);
            break;
        case E_TSP_SRC_PKTDMX3:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F, CFG5_7F_CLR_SRC_MASK, CFG5_7F_CLR_SRC_PKTDMX3 << CFG5_7F_CLR_SRC_SHIFT);
            break;
        case E_TSP_SRC_MMFI0:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F, CFG5_7F_CLR_SRC_MASK, CFG5_7F_CLR_SRC_MMFI0 << CFG5_7F_CLR_SRC_SHIFT);
            break;
        case E_TSP_SRC_MMFI1:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F, CFG5_7F_CLR_SRC_MASK, CFG5_7F_CLR_SRC_MMFI1 << CFG5_7F_CLR_SRC_SHIFT);
            break;
        default:
            printf("[%s][%s][%d] UnSupported Clear Source : %d !\n",__FILE__,__FUNCTION__,__LINE__,eClrSrc);
            break;
    }
}

void HAL_TSP_Debug_AvPktCnt_Src(TSP_DST_SEQ eAvType, TSP_SRC_SEQ ePktDmxId)
{
    MS_U16 u16AvSrc = 0;

    switch(ePktDmxId)
    {
        case E_TSP_SRC_PKTDMX0:
            u16AvSrc = AV_PKT_SRC_PKTDMX0;
            break;
        case E_TSP_SRC_PKTDMX1:
            u16AvSrc = AV_PKT_SRC_PKTDMX1;
            break;
        case E_TSP_SRC_PKTDMX2:
            u16AvSrc = AV_PKT_SRC_PKTDMX2;
            break;
        case E_TSP_SRC_PKTDMX3:
            u16AvSrc = AV_PKT_SRC_PKTDMX3;
            break;
        case E_TSP_SRC_MMFI0:
            u16AvSrc = AV_PKT_SRC_MMFI0;
            break;
        case E_TSP_SRC_MMFI1:
            u16AvSrc = AV_PKT_SRC_MMFI1;
            break;
        default:
            printf("[%s][%s][%d] UnSupported AV Source : %d!\n",__FILE__,__FUNCTION__,__LINE__,ePktDmxId);
            break;
    }

    switch (eAvType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            REG16_MSK_W(&_RegCtrl5->CFG5_7E, CFG5_7E_VID_SRC_MASK, u16AvSrc << CFG5_7E_VID_SRC_SHIFT);
            break;
        case E_TSP_DST_FIFO_VIDEO3D :
            REG16_MSK_W(&_RegCtrl5->CFG5_7E, CFG5_7E_VID_3D_SRC_MASK, u16AvSrc << CFG5_7E_VID_3D_SRC_SHIFT);
            break;
        case E_TSP_DST_FIFO_AUDIO :
            REG16_MSK_W(&_RegCtrl5->CFG5_7E, CFG5_7E_AUDA_SRC_MASK, u16AvSrc << CFG5_7E_AUDA_SRC_SHIFT);
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            REG16_MSK_W(&_RegCtrl5->CFG5_7E, CFG5_7E_AUDB_SRC_MASK, u16AvSrc << CFG5_7E_AUDB_SRC_SHIFT);
            break;
        default:
            printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
            break;
    }
}

void HAL_TSP_Debug_AvPktCnt_Load(TSP_DST_SEQ eAvType, MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        switch (eAvType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_SET(&_RegCtrl5->CFG5_7A,CFG5_7A_V_PKT_CNT_LOAD);
                break;
            case E_TSP_DST_FIFO_VIDEO3D :
                REG16_SET(&_RegCtrl5->CFG5_7A,CFG5_7A_V3D_PKT_CNT_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG16_SET(&_RegCtrl5->CFG5_7A,CFG5_7A_A_PKT_CNT_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_SET(&_RegCtrl5->CFG5_7A,CFG5_7A_AD_PKT_CNT_LOAD);
                break;
            default :
                printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
                break;
        }
    }
    else
    {
        switch (eAvType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_CLR(&_RegCtrl5->CFG5_7A,CFG5_7A_V_PKT_CNT_LOAD);
                break;
            case E_TSP_DST_FIFO_VIDEO3D :
                REG16_CLR(&_RegCtrl5->CFG5_7A,CFG5_7A_V3D_PKT_CNT_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG16_CLR(&_RegCtrl5->CFG5_7A,CFG5_7A_A_PKT_CNT_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_CLR(&_RegCtrl5->CFG5_7A,CFG5_7A_AD_PKT_CNT_LOAD);
                break;
            default :
                printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
                break;
        }
    }
}

MS_U16 HAL_TSP_Debug_AvPktCnt_Get(TSP_DST_SEQ eAvType)
{
    switch (eAvType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            REG16_CLR(&_RegCtrl5->CFG5_7F,CFG5_7F_AV_PKT_SRC_SEL);
            return REG16_R(&_RegCtrl5->Av_PKT_Cnt);
        case E_TSP_DST_FIFO_VIDEO3D:
            REG16_CLR(&_RegCtrl5->CFG5_7F,CFG5_7F_AV_PKT_SRC_SEL);
            return REG16_R(&_RegCtrl5->Av_PKT_Cnt1);
        case E_TSP_DST_FIFO_AUDIO :
            REG16_SET(&_RegCtrl5->CFG5_7F,CFG5_7F_AV_PKT_SRC_SEL);
            return REG16_R(&_RegCtrl5->Av_PKT_Cnt);
        case E_TSP_DST_FIFO_AUDIO2 :
            REG16_SET(&_RegCtrl5->CFG5_7F,CFG5_7F_AV_PKT_SRC_SEL);
            return REG16_R(&_RegCtrl5->Av_PKT_Cnt1);
        default :
            printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
            return 0;
    }
}

void HAL_TSP_Debug_AvPktCnt_Clear(TSP_DST_SEQ eAvType)
{
    switch (eAvType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            REG16_SET(&_RegCtrl5->CFG5_7C,CFG5_7C_V_PKT_CNT_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7C,CFG5_7C_V_PKT_CNT_CLR);
            break;
        case E_TSP_DST_FIFO_VIDEO3D :
            REG16_SET(&_RegCtrl5->CFG5_7C,CFG5_7C_V3D_PKT_CNT_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7C,CFG5_7C_V3D_PKT_CNT_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO :
            REG16_SET(&_RegCtrl5->CFG5_7C,CFG5_7C_A_PKT_CNT_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7C,CFG5_7C_A_PKT_CNT_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            REG16_SET(&_RegCtrl5->CFG5_7C,CFG5_7C_AD_PKT_CNT_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7C,CFG5_7C_AD_PKT_CNT_CLR);
            break;
        default :
            printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
            break;
    }
}

void HAL_TSP_Debug_DropDisPktCnt_Src(TSP_DST_SEQ eAvType,TSP_SRC_SEQ ePktDmxId)
{
    MS_U16 u16AvType = 0;

    switch(eAvType)
    {
        case E_TSP_DST_FIFO_VIDEO:
            u16AvType = DIS_DROP_CNT_V;
            break;
        case E_TSP_DST_FIFO_VIDEO3D:
            u16AvType = DIS_DROP_CNT_V3D;
            break;
        case E_TSP_DST_FIFO_AUDIO:
            u16AvType = DIS_DROP_CNT_A;
            break;
        case E_TSP_DST_FIFO_AUDIO2:
            u16AvType = DIS_DROP_CNT_AD;
            break;
        default:
            printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
            break;
    }

    switch(ePktDmxId)
    {
        case E_TSP_SRC_PKTDMX0:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F,CFG5_7F_PIDFLT_SRC_SEL_MASK,(u16AvType<<CFG5_7F_PIDFLT_SRC_SEL_SHIFT));
            break;
        case E_TSP_SRC_PKTDMX1:
            REG16_MSK_W(&_RegCtrl5->CFG5_7F,CFG5_7F_PIDFLT_SRC_SEL1_MASK,(u16AvType<<CFG5_7F_PIDFLT_SRC_SEL1_SHIFT));
            break;
        case E_TSP_SRC_PKTDMX2:
            REG16_MSK_W(&_RegCtrl5->CFG5_77,CFG5_77_PIDFLT_SRC_SEL2_MASK,(u16AvType<<CFG5_77_PIDFLT_SRC_SEL2_SHIFT));
            break;
        case E_TSP_SRC_MMFI0:
            REG16_MSK_W(&_RegCtrl5->CFG5_78,CFG5_78_PIDFLT_SRC_SEL_MMFI0_MASK,(u16AvType<<CFG5_78_PIDFLT_SRC_SEL_MMFI0_SHIFT));
            break;
        case E_TSP_SRC_MMFI1:
            REG16_MSK_W(&_RegCtrl5->CFG5_78,CFG5_78_PIDFLT_SRC_SEL_MMFI1_MASK,(u16AvType<<CFG5_78_PIDFLT_SRC_SEL_MMFI1_SHIFT));
            break;
        default:
            printf("[%s][%s][%d] UnSupported AV FIFO Source : %d !\n",__FILE__,__FUNCTION__,__LINE__,ePktDmxId);
            break;
    }
}

void HAL_TSP_Debug_DropPktCnt_Load(TSP_DST_SEQ eAvType,MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        switch (eAvType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_V_LOAD);
                break;
            case E_TSP_DST_FIFO_VIDEO3D :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_V3D_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_A_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_AD_LOAD);
                break;
            default :
                printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
                break;
        }
    }
    else
    {
        switch (eAvType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_V_LOAD);
                break;
            case E_TSP_DST_FIFO_VIDEO3D :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_V3D_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_A_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DROP_PKT_CNT_AD_LOAD);
                break;
            default :
                printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
                break;
        }
    }
}

void HAL_TSP_Debug_DisPktCnt_Load(TSP_DST_SEQ eAvType,MS_BOOL bEn,MS_BOOL bPayload)
{
    if(bPayload)
    {
        REG16_SET(&_RegCtrl3->CFG3_37,HW4_CFG37_DIS_CNTR_INC_BY_PL);
    }
    else
    {
        REG16_CLR(&_RegCtrl3->CFG3_37,HW4_CFG37_DIS_CNTR_INC_BY_PL);
    }

    if(bEn == TRUE)
    {
        switch (eAvType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_V_LOAD);
                break;
            case E_TSP_DST_FIFO_VIDEO3D :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_V3D_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_A_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_SET(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_AD_LOAD);
                break;
            default :
                printf("[%s][%s][%d] UnSupported AV Type : %d \n",__FILE__,__FUNCTION__,__LINE__,eAvType);
                break;
        }
    }
    else
    {
        switch (eAvType)
        {
            case E_TSP_DST_FIFO_VIDEO :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_V_LOAD);
                break;
            case E_TSP_DST_FIFO_VIDEO3D :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_V3D_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_A_LOAD);
                break;
            case E_TSP_DST_FIFO_AUDIO2 :
                REG16_CLR(&_RegCtrl5->CFG5_7B,CFG5_7B_DIS_PKT_CNT_AD_LOAD);
                break;
            default :
                printf("[%s][%s][%d] UnSupported AV Type : %d \n",__FILE__,__FUNCTION__,__LINE__,eAvType);
                break;
        }
    }
}

MS_U16 HAL_TSP_Debug_DropDisPktCnt_Get(TSP_SRC_SEQ ePktDmxId, MS_BOOL bDrop)
{
    if(bDrop)
    {
        REG16_SET(&_RegCtrl5->CFG5_7F,CFG5_7F_DROP_PKT_MODE);
    }
    else
    {
        REG16_CLR(&_RegCtrl5->CFG5_7F,CFG5_7F_DROP_PKT_MODE);
    }

    switch (ePktDmxId)
    {
        case E_TSP_SRC_PKTDMX0:
            return REG16_R(&_RegCtrl5->Drop_Dis_PKT_Cnt_0);
        case E_TSP_SRC_PKTDMX1:
            return REG16_R(&_RegCtrl5->Drop_Dis_PKT_Cnt_1);
        case E_TSP_SRC_PKTDMX2:
            return REG16_R(&_RegCtrl5->Drop_Dis_PKT_Cnt_2);
        case E_TSP_SRC_PKTDMX3:
            return REG16_R(&_RegCtrl5->Drop_Dis_PKT_Cnt_3);
        case E_TSP_SRC_MMFI0:
            return REG16_R(&_RegCtrl5->CFG5_0B);
        case E_TSP_SRC_MMFI1:
            return REG16_R(&_RegCtrl5->CFG5_0C);
        default :
            printf("[%s][%s][%d] UnSupported AV FIFO Source : %d\n",__FILE__,__FUNCTION__,__LINE__,ePktDmxId);
            return 0;
    }
}

void HAL_TSP_Debug_DropPktCnt_Clear(TSP_DST_SEQ eAvType)
{
    switch (eAvType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_V_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_V_CLR);
            break;
        case E_TSP_DST_FIFO_VIDEO3D :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_V3D_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_V3D_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_A_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_A_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_AD_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DROP_PKT_CNT_AD_CLR);
            break;
        default :
            printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
            break;
    }
}

void HAL_TSP_Debug_DisPktCnt_Clear(TSP_DST_SEQ eAvType)
{
    switch (eAvType)
    {
        case E_TSP_DST_FIFO_VIDEO :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V_CLR);
            break;
        case E_TSP_DST_FIFO_VIDEO3D :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V3D_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_V3D_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_A_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_A_CLR);
            break;
        case E_TSP_DST_FIFO_AUDIO2 :
            REG16_SET(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_AD_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_7D,CFG5_7D_DIS_PKT_CNT_AD_CLR);
            break;
        default :
            printf("[%s][%s][%d] UnSupported AV Type : %d !\n",__FILE__,__FUNCTION__,__LINE__,eAvType);
            break;
    }
}

void HAL_TSP_Debug_ErrPktCnt_Src(MS_U32 u32TsIf)
{
    MS_U16 u16TSIF = 0;;

    switch(u32TsIf)
    {
        case 0:
            u16TSIF = TSIF_SRC_SEL_TSIF0;
            break;
        case 1:
            u16TSIF = TSIF_SRC_SEL_TSIF1;
            break;
        case 2:
            u16TSIF = TSIF_SRC_SEL_TSIF2;
            break;
        default:
            printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
            break;
    }

    REG16_MSK_W(&_RegCtrl5->CFG5_70,CFG5_70_ERR_PKT_SRC_SEL_MASK,(u16TSIF << CFG5_70_ERR_PKT_SRC_SEL_SHIFT));
}

void HAL_TSP_Debug_ErrPktCnt_Load(MS_U32 u32TsIf,MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        switch(u32TsIf)
        {
            case 0:
                REG16_SET(&_RegCtrl5->CFG5_71,CFG5_71_ERR_PKT_CNT_0_LOAD);
                break;
            case 1:
                REG16_SET(&_RegCtrl5->CFG5_71,CFG5_71_ERR_PKT_CNT_1_LOAD);
                break;
            case 2:
                REG16_SET(&_RegCtrl5->CFG5_71,CFG5_71_ERR_PKT_CNT_2_LOAD);
                break;
            default:
                printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
                break;
        }
    }
    else
    {
        switch(u32TsIf)
        {
            case 0:
                REG16_CLR(&_RegCtrl5->CFG5_71,CFG5_71_ERR_PKT_CNT_0_LOAD);
                break;
            case 1:
                REG16_CLR(&_RegCtrl5->CFG5_71,CFG5_71_ERR_PKT_CNT_1_LOAD);
                break;
            case 2:
                REG16_CLR(&_RegCtrl5->CFG5_71,CFG5_71_ERR_PKT_CNT_2_LOAD);
                break;
            default:
                printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
                break;
        }
    }

}

MS_U16 HAL_TSP_Debug_ErrPktCnt_Get(void)
{
    return REG16_R(&_RegCtrl5->Err_PKT_Cnt);
}

void HAL_TSP_Debug_ErrPktCnt_Clear(MS_U32 u32Tsif)
{
    switch (u32Tsif)
    {
        case 0 :
            REG16_SET(&_RegCtrl5->CFG5_72,CFG5_72_ERR_PKT_CNT_0_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_72,CFG5_72_ERR_PKT_CNT_0_CLR);
            break;
        case 1:
            REG16_SET(&_RegCtrl5->CFG5_72,CFG5_72_ERR_PKT_CNT_1_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_72,CFG5_72_ERR_PKT_CNT_1_CLR);
            break;
        case 2 :
            REG16_SET(&_RegCtrl5->CFG5_72,CFG5_72_ERR_PKT_CNT_2_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_72,CFG5_72_ERR_PKT_CNT_2_CLR);
            break;
        default :
            printf("[%s][%s][%d] UnSupported TSIF : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Tsif);
            break;
    }
}

void HAL_TSP_Debug_InputPktCnt_Src(MS_U32 u32TsIf)
{
    MS_U16 u16TSIF = 0;;

    switch(u32TsIf)
    {
        case 0:
            u16TSIF = TSIF_SRC_SEL_TSIF0;
            break;
        case 1:
            u16TSIF = TSIF_SRC_SEL_TSIF1;
            break;
        case 2:
            u16TSIF = TSIF_SRC_SEL_TSIF2;
            break;
        default:
            printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
            break;
    }

    REG16_MSK_W(&_RegCtrl5->CFG5_70,CFG5_70_INPUT_PKT_SRC_SEL_MASK,(u16TSIF << CFG5_70_INPUT_PKT_SRC_SEL_SHIT));
}

void HAL_TSP_Debug_InputPktCnt_Load(MS_U32 u32TsIf,MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        switch(u32TsIf)
        {
            case 0:
                REG16_SET(&_RegCtrl5->CFG5_71,CFG5_71_INPUT_PKT_CNT_0_LOAD);
                break;
            case 1:
                REG16_SET(&_RegCtrl5->CFG5_71,CFG5_71_INPUT_PKT_CNT_1_LOAD);
                break;
            case 2:
                REG16_SET(&_RegCtrl5->CFG5_71,CFG5_71_INPUT_PKT_CNT_2_LOAD);
                break;
            default:
                printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
                break;
        }
    }
    else
    {
        switch(u32TsIf)
        {
            case 0:
                REG16_CLR(&_RegCtrl5->CFG5_71,CFG5_71_INPUT_PKT_CNT_0_LOAD);
                break;
            case 1:
                REG16_CLR(&_RegCtrl5->CFG5_71,CFG5_71_INPUT_PKT_CNT_1_LOAD);
                break;
            case 2:
                REG16_CLR(&_RegCtrl5->CFG5_71,CFG5_71_INPUT_PKT_CNT_2_LOAD);
                break;
            default:
                printf("[%s][%s][%d] UnSupported TSIF : %u\n",__FILE__,__FUNCTION__,__LINE__,(unsigned int)u32TsIf);
                break;
        }
    }
}

MS_U16 HAL_TSP_Debug_InputPktCnt_Get(void)
{
    return REG16_R(&_RegCtrl5->Input_PKT_Cnt);
}

void HAL_TSP_Debug_InputPktCnt_Clear(MS_U32 u32Tsif)
{
    switch (u32Tsif)
    {
        case 0 :
            REG16_SET(&_RegCtrl5->CFG5_72,CFG5_72_INPUT_PKT_CNT_0_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_72,CFG5_72_INPUT_PKT_CNT_0_CLR);
            break;
        case 1:
            REG16_SET(&_RegCtrl5->CFG5_72,CFG5_72_INPUT_PKT_CNT_1_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_72,CFG5_72_INPUT_PKT_CNT_1_CLR);
            break;
        case 2 :
            REG16_SET(&_RegCtrl5->CFG5_72,CFG5_72_INPUT_PKT_CNT_2_CLR);
            REG16_CLR(&_RegCtrl5->CFG5_72,CFG5_72_INPUT_PKT_CNT_2_CLR);
            break;
        default :
            printf("[%s][%s][%d] UnSupported TSIF : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Tsif);
            break;
    }

}

MS_BOOL HAL_TSP_FQ_SetMuxSwitch(MS_U32 u32FQEng, MS_U32 u32FQSrc)
{
    MS_U32 u32Src = 0xFF;

    if(u32FQEng != 0)
        return FALSE;

    switch(u32FQSrc)
    {
        case 0:                                 // @NOTE : mapping for Playback0 (LIVE0)
            u32Src = TSP_FIQ_SRC_PATH0;
            break;
        case 1:                                 // @NOTE : mapping for Playback1 (LIVE1)
            u32Src = TSP_FIQ_SRC_PATH1;
            break;
        case 2:                                 // @NOTE : mapping for Playback2 (LIVE2)
            u32Src = TSP_FIQ_SRC_PATH2;
            break;
        default:
            return FALSE;
    }

    REG16_MSK_W(&_RegCtrl6->CFG6_60, TSP_FIQ_MUX_MASK, (MS_U16)u32Src);

    return TRUE;
}

MS_U32 HAL_TSP_FQ_GetMuxSwitch(MS_U32 u32FQEng)
{
    MS_U32 u32Src = 0xFF;
    MS_U32 u32Ret = 0xFF;

    if(u32FQEng != 0)
        return FALSE;

    u32Src = (MS_U32)(REG16_R(&_RegCtrl6->CFG6_60) & TSP_FIQ_MUX_MASK);

    switch(u32Src)
    {
        case TSP_FIQ_SRC_PATH0:
            u32Ret = 0;                        // @NOTE : mapping for Playback0 (LIVE0)
            break;
        case TSP_FIQ_SRC_PATH1:
            u32Ret = 1;                        // @NOTE : mapping for Playback2 (LIVE2)
            break;
        case TSP_FIQ_SRC_PATH2:
            u32Ret = 2;                        // @NOTE : mapping for Playback1 (LIVE1)
            break;
        default:
            return FALSE;
    }
    return u32Ret;
}

MS_BOOL HAL_TSP_FQ_FLT_NULL_PKT(MS_U32 u32FQEng, MS_BOOL bFltNull)
{
    // not implement yet
    return TRUE;
}
