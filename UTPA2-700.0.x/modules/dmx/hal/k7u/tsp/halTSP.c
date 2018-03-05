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

#ifdef CONFIG_MSTAR_CLKM
    #include "drvCLKM.h"
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    #include <linux/string.h>
#else
    #include <string.h>
#endif
//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define FPGA_TEST   1

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT                  _u32RegBase               = 0;

static REG_Ctrl                 *_RegCtrl                 = NULL;    // TSP 0/1
static REG_TSP_MULTI_Ctrl       *_RegTspMultiCtrl         = NULL;    // TSP_MULTI
static REG_TSP_MULTI_SYNC_Ctrl  *_RegTspMultiSyncCtrl     = NULL;    // TSP_MULTI_SYNC_0 (0 ~ 3)
static REG_TSP_MULTI_SYNC_Ctrl  *_RegTspMultiSyncCtrl_1   = NULL;    // TSP_MULTI_SYNC_1 (4 ~ 6)
static REG_TSP_SRC_Ctrl         *_RegTspSrcCtrl           = NULL;    // TSP_SRC
static REG_PATH_ENG_Ctrl        *_RegPathCtrl             = NULL;    // Path
static REG_FILE_ENG_Ctrl        *_RegFile0Ctrl            = NULL;    // File #0
static REG_FILE_ENG_Ctrl        *_RegFile1Ctrl            = NULL;    // File #1
static REG_OTHER_Ctrl           *_RegOtherCtrl            = NULL;    // Other/resample
static REG_TOP_Ctrl             *_RegTopCtrl              = NULL;    // Top
static REG_AV_ENG_Ctrl          *_RegAudioCtrl            = NULL;    // Audio
static REG_AV_ENG_Ctrl          *_RegVideoCtrl            = NULL;    // Video
static REG_PVR_ENG_Ctrl         *_RegPvrCtrl              = NULL;    // PVR (1~8)
static REG_PVR_ENG_Ctrl         *_RegPvrCtrl_1            = NULL;    // PVR (9~10)
static REG_STC_ENG_Ctrl         *_RegStcCtrl              = NULL;    // STC
static REG_PCR_ENG_Ctrl         *_RegPcrCtrl              = NULL;    // PCR
static REG_SPS_ENG_Ctrl         *_RegSpsCtrl              = NULL;    // SPS (1~8)
static REG_SPS_ENG_Ctrl         *_RegSpsCtrl_1            = NULL;    // SPS (9~10)
static REG_SPD_ENG_Ctrl         *_RegSpdCtrl              = NULL;    // SPD


// @F_TODO These parameters need to be combined with global variables in Utopia 2.0
static MS_U32               _u32PidFltReg[(TSP_PIDFLT_NUM * sizeof(REG_PidFlt))];
static MS_U32               _u32PidDstReg[(TSP_PIDFLT_NUM * sizeof(REG_PidFlt))];
static MS_U32               _u32PidDst2Reg[(TSP_PIDFLT_NUM * sizeof(REG_PidFlt))];
static MS_U32               _u32SecReg[(TSP_SECFLT_NUM * ((sizeof(REG_SecFlt) - sizeof(((REG_SecFlt*)0)->_x54))/sizeof(TSP32)))];


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

#define REG32_W(reg, value)     { (reg)->low = ((value) & 0x0000FFFF);      \
                                  (reg)->high = ((value) >> 16);            \
                                if(_u32RegDump){_REG32_DUMP(reg, value);}}

#define REG16_W(reg, value)     {(reg)->data = ((value) & 0x0000FFFF);      \
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
    _u32RegBase             = u32BankAddr;

    _RegCtrl                = (REG_Ctrl*)                (u32BankAddr + 0x2A00UL);   // TSP0/1              0x1015/0x1016
    _RegPathCtrl            = (REG_PATH_ENG_Ctrl*)       (u32BankAddr + 0xC2000UL);  // Path                0x1610
    _RegFile0Ctrl           = (REG_FILE_ENG_Ctrl*)       (u32BankAddr + 0xC2200UL);  // File0               0x1611
    _RegFile1Ctrl           = (REG_FILE_ENG_Ctrl*)       (u32BankAddr + 0xC4E00UL);  // File1               0x1627
    _RegPvrCtrl_1           = (REG_PVR_ENG_Ctrl*)        (u32BankAddr + 0xC5600UL);  // PVR 9~10            0x162B
    _RegSpsCtrl_1           = (REG_SPS_ENG_Ctrl*)        (u32BankAddr + 0xC5800UL);  // SPS 9~10            0x162C
    _RegOtherCtrl           = (REG_OTHER_Ctrl*)          (u32BankAddr + 0xE0400UL);  // Other               0x1702
    _RegTopCtrl             = (REG_TOP_Ctrl*)            (u32BankAddr + 0xE0600UL);  // Top                 0x1703
    _RegSpdCtrl             = (REG_SPD_ENG_Ctrl*)        (u32BankAddr + 0xE1800UL);  // SPD                 0x170C
    _RegSpsCtrl             = (REG_SPS_ENG_Ctrl*)        (u32BankAddr + 0xE1A00UL);  // SPS 1~8             0x170D

    _RegAudioCtrl           = (REG_AV_ENG_Ctrl*)         (u32BankAddr + 0x400400UL); // Audio               0x3002
    _RegVideoCtrl           = (REG_AV_ENG_Ctrl*)         (u32BankAddr + 0x400600UL); // Video               0x3003
    _RegPvrCtrl             = (REG_PVR_ENG_Ctrl*)        (u32BankAddr + 0x401400UL); // PVR 1~8             0x300A ~ 0x300D
    _RegStcCtrl             = (REG_STC_ENG_Ctrl*)        (u32BankAddr + 0x401C00UL); // STC                 0x300E
    _RegPcrCtrl             = (REG_PCR_ENG_Ctrl*)        (u32BankAddr + 0x401C80UL); // PCR                 0x300E (offset: 0x20)
    _RegTspMultiCtrl        = (REG_TSP_MULTI_Ctrl*)      (u32BankAddr + 0x402600UL); // TSP_MULTI           0x3013
    _RegTspSrcCtrl          = (REG_TSP_SRC_Ctrl*)        (u32BankAddr + 0x402800UL); // TSP_SRC             0x3014
    _RegTspMultiSyncCtrl    = (REG_TSP_MULTI_SYNC_Ctrl*) (u32BankAddr + 0x402A00UL); // TSP_MULTI_SYNC_0    0x3015
    _RegTspMultiSyncCtrl_1  = (REG_TSP_MULTI_SYNC_Ctrl*) (u32BankAddr + 0x404E00UL); // TSP_MULTI_SYNC_1    0x3027
}

void HAL_TSP_RegDump(MS_BOOL bEnable)
{
    _u32RegDump = bEnable;
}

#if 1
static void _REG32_DUMP(REG32* reg, MS_U32 value )
{
    //`RIU_W(`TSP_REG_BASE + 7'h06, 2'b11, 16'h400a);   //..bank 15 ,7h06 ....reg ,16h400a ......
    //`RIU_W(`TSP_REG_BASE1 + 7'h72, 2'b11, 16'hc000); // ..bank 16 ,7h06 ....reg ,16h400a ......
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
    //`RIU_W(`TSP_REG_BASE + 7'h06, 2'b11, 16'h400a);   //..bank 15 ,7h06 ....reg ,16h400a ......
    //`RIU_W(`TSP_REG_BASE1 + 7'h72, 2'b11, 16'hc000); // ..bank 16 ,7h06 ....reg ,16h400a ......
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
    REG16_SET(&_RegOtherCtrl->CFG_OTHER_16, CFG_OHTER_16_REG_PREVENT_SRAM_COLLISION);

    // @F_TODO check these setting with Stephen
    // TSP_HW_CFG4_WSTAT_CH_EN <--this is bit disable HW sync section buf id with section filter id
    REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_BYTE_ADDR_DMA|TSP_HW_CFG4_ALT_TS_SIZE|TSP_HW_CFG4_WSTAT_CH_EN);

    // Bad initial value of TSP_CTRL1
    // Suppose Standby mode for TSP should NOT be enabled.
    // Enabling TSP standby mode cause TSP section registers (SRAM in AEON) malfunction.
    // Disable it by SW at this stage.
    REG16_CLR(&_RegCtrl->TSP_Ctrl1, TSP_CTRL1_STANDBY);

    MS_U8   u8Idx = 0;

    for(u8Idx = 0; u8Idx < TSP_PVRENG_NUM; u8Idx++)
    {
        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u8Idx < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u8Idx < E_TSP_PVR_PVRENG_8)? u8Idx : (u8Idx - E_TSP_PVR_PVRENG_8);

        // enable PVR record to bypass header
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_RECORD_TS);

        // record null packets for record-all case
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_DIS_NULL_PKT);

        // Fixed PVR eng write pointer to next addr
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_0D, CFG_PVR_0D_REG_PVR_WRITE_POINTER_TO_NEXT_ADDRESS_EN);

        // skip FQ rush data
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_SKIP_PVR_RUSH_DATA);
    }

    for(u8Idx = 0; u8Idx < TSP_PCRFLT_NUM; u8Idx++)
    {
        // skip FQ rush data
        REG16_SET(&_RegPcrCtrl[u8Idx].CFG_PCR_00, CFG_PCR_00_REG_SKIP_PVR_RUSH_DATA);
    }

    for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_AFIFO_NUM; u8Idx++)
    {
        REG16_SET(&_RegAudioCtrl[u8Idx].CFG_AV_00, (CFG_AV_00_REG_DUP_PKT_SKIP | CFG_AV_00_REG_PUSI_THREE_BYTE_MODE));
    }

    for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_VFIFO_NUM; u8Idx++)
    {
        REG16_SET(&_RegVideoCtrl[u8Idx].CFG_AV_00, (CFG_AV_00_REG_DUP_PKT_SKIP | CFG_AV_00_REG_PUSI_THREE_BYTE_MODE));
    }

    //Disable TSP_RM_OVF_GLITCH to fix that section A would occur overflow when section B occured overflow at first.
    REG16_SET(&_RegCtrl->HW2_Config3, TSP_VQ2PINGPONG_EN | TSP_RM_PKT_DEMUX_PIPE /*| TSP_PVR1_ALIGN_EN*/);

    //If SEC CMP adopts pingpong mode (1 + 1 packets) , there is always 1 packet stayed in queue.
    //For example , if it receive 100 packets , the last packet is not in section buffer.
    //Set below bits can force SEC CMP processing SEC packet after receiving a complete 188 bytes.
    REG16_SET(&_RegCtrl->reg160C, TSP_DOUBLE_BUF_DESC);
    REG16_SET(&_RegOtherCtrl->CFG_OTHER_16, CFG_OHTER_16_REG_FIX_PINPON_SYNCP_IN);

    REG16_SET(&_RegCtrl->reg160E, TSP_RM_DMA_GLITCH);

    REG16_SET(&_RegCtrl->TSP_Cfg5, TSP_SYSTIME_MODE);

    REG16_SET(&_RegCtrl->PVRConfig, TSP_MATCH_PID_LD);

    for(u8Idx = 0; u8Idx < TSP_TSIF_NUM; u8Idx++)
    {
        REG16_SET(&_RegPathCtrl[u8Idx].CFG_PATH_08, CFG_PATH_08_REG_BLK_AF_SCRMB_BIT_TSP);

        // Merge stream through FIQ ECO config
        REG16_SET(&_RegPathCtrl[u8Idx].CFG_PATH_06, CFG_PATH_06_REG_ECO_TS_SYNC_OUT_DELAY);
        REG16_SET(&_RegPathCtrl[u8Idx].CFG_PATH_06, CFG_PATH_06_REG_ECO_TS_SYNC_OUT_REVERSE_BLOCK);

        // Fix filter null packet
        REG16_SET(&_RegPathCtrl[u8Idx].CFG_PATH_06, CFG_PATH_06_REG_FIX_FILTER_NULL_PKT);
    }

    // Serial mode config
    REG16_SET(&_RegCtrl->reg15b8, TSP_SERIAL_EXT_SYNC_1T);
    REG16_SET(&_RegCtrl->Hw_PVRCfg, TSP_SYNC_RISING_DETECT);
    REG16_SET(&_RegCtrl->Hw_PVRCfg, TSP_VALID_FALLING_DETECT)

    // Internal Sync Patch
    REG16_SET(&_RegCtrl->Hw_Config4, TSP_HW_CFG4_DATA_CHK_2T);

    for(u8Idx = 0; u8Idx < TSP_TSIF_NUM; u8Idx++)
    {
        REG_FILE_ENG_Ctrl   *RegFileCtrl = (u8Idx < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
        MS_U8   u8FileEng = (u8Idx < E_FILEENG_TSIF4)? u8Idx : (u8Idx - E_FILEENG_TSIF4);

        // Chip after Kirin will lost data when byte timer enable and delay set to 0 setting this bit to fix this issue
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_0A, CFG_FILE_0A_REG_FIX_192_TIMER_0_EN);

        // Fix pass_pkt_timestamp will hang issue
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_0A, CFG_FILE_0A_REG_INIT_STAMP_RSTART_EN);
    }

    // Fix load fw secure issue (dma_start = 1 , polling dma_done , dma_start = 0)
    REG16_SET(&_RegOtherCtrl->CFG_OTHER_14, CFG_OHTER_14_REG_CPU_LOAD_CODE_ONLY_ONE_TIME_BY_TEE);

    // Fix miu_req & miu_last
    REG16_SET(&_RegOtherCtrl->CFG_OTHER_75, CFG_OTHER_75_REG_FIXED_MIU_REQ_FLUSH);

    // Fix miu arbiter last_done_z
    REG16_SET(&_RegTopCtrl->CFG_TOP_24, CFG_TOP_24_REG_MIU_FIXED_LAST_WD_EN_DONE_Z_ABT_ALL);
    REG16_SET(&_RegTopCtrl->CFG_TOP_25, CFG_TOP_25_REG_CHECK_MI2RDY_ABT_ALL);
    REG16_SET(&_RegOtherCtrl->CFG_OTHER_14, CFG_OHTER_14_REG_OR_WRITE_FIX_FOR_NEW_MIU_ARBITER_DISABLE);
}

// ------------ initial config ------------
// Sync Byte: 0x47 , 0x48 .... , 0x4e
// Source id : 0 , 1 , ... , 7
// User can use "HAL_TSP_PktConverter_SetSyncByte()" to change Sync Byte configuration
// , and use "HAL_TSP_PktConverter_SetSrcId()" to change Source id configuration
void HAL_TSP_PktConverter_Init(void)
{
    MS_U8   u8Path, u8Idx;
    MS_U8   u8SyncByte, u8SrcId;

    for(u8Path = 0; u8Path < TSP_TSIF_NUM; ++u8Path)
    {
        HAL_TSP_PktConverter_ForceSync(u8Path, TRUE);   //default: FALSE
        HAL_TSP_PktConverter_SrcIdFlt(u8Path,TRUE);     //default: FALSE

        u8SyncByte = 0x47;
        u8SrcId = 0;

        for(u8Idx = 0; u8Idx < TSP_MERGESTREAM_NUM; ++u8Idx,++u8SyncByte,++u8SrcId)
        {
            HAL_TSP_PktConverter_SetSyncByte(u8Path, u8Idx, &u8SyncByte, TRUE);
            HAL_TSP_PktConverter_SetSrcId(u8Path, u8Idx, &u8SrcId, TRUE);
        }
    }
}

void HAL_TSP_Reset(MS_BOOL bEn)
{
    FILEENG_SEQ eFileEng = E_FILEENG_TSIF0;

    if(bEn)
    {
        // reset CMDQ
        for(eFileEng = 0; eFileEng < TSP_TSIF_NUM; eFileEng++)
        {
            HAL_TSP_Filein_CmdQRst(eFileEng, TRUE);
        }

        REG16_SET(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_WB_DMA_RESET);
        REG16_CLR(&_RegCtrl->TSP_Ctrl, TSP_CTRL_SW_RST);
    }
    else
    {
        REG16_SET(&_RegCtrl->TSP_Ctrl, TSP_CTRL_SW_RST);
        REG16_CLR(&_RegCtrl->Hw_Config0, TSP_HW_CFG0_WB_DMA_RESET);

        for(eFileEng = 0; eFileEng < TSP_TSIF_NUM; eFileEng++)
        {
            HAL_TSP_Filein_CmdQRst(eFileEng, FALSE);
        }
    }
}

void HAL_TSP_Power(MS_BOOL bEn)
{
#ifdef CONFIG_MSTAR_CLKM
    MS_U8   u8Idx = 0;
    char    ClkName[20] = "";
    char    Postfix[10] = "";
    MS_S32  s32Handle;
#endif

    if(bEn)
    {
    #ifdef CONFIG_MSTAR_CLKM

        // Enable TSP Clk
        s32Handle = Drv_Clkm_Get_Handle("g_clk_tsp");
        Drv_Clkm_Set_Clk_Source(s32Handle,"CLK_TSP_FAST");

        // Enable STC Clk
        for(u8Idx = 0; u8Idx < STC_ENG_NUM; u8Idx++)
        {
            strcpy(ClkName, "g_clk_stc");
            sprintf(Postfix, "%d", u8Idx);
            strcat(ClkName, Postfix);

            s32Handle = Drv_Clkm_Get_Handle(ClkName);
            Drv_Clkm_Set_Clk_Source(s32Handle, "CLK_STC0_BUF");
        }

        // Stamp
        s32Handle = Drv_Clkm_Get_Handle("g_clk_stamp");
        Drv_Clkm_Set_Clk_Source(s32Handle,"CLK_STAMP_NORMAL");

        // Parser
        s32Handle = Drv_Clkm_Get_Handle("g_clk_parser");
        Drv_Clkm_Set_Clk_Source(s32Handle,"CLK_PARSER_NORMAL");

        // Enable TSIF => Disable TSIF
        // FixME Enable flowset would enable TSx clk so we don't enable TSx clk
        for(u8Idx = 0; u8Idx < TSP_TSIF_NUM; u8Idx++)
        {
            strcpy(ClkName, "g_clk_ts");
            sprintf(Postfix, "%d", u8Idx);
            strcat(ClkName, Postfix);

            s32Handle = Drv_Clkm_Get_Handle(ClkName);
            Drv_Clkm_Set_Clk_Source(s32Handle, "CLK_TS0_PAD0");
        }

        //TSO0
        s32Handle = Drv_Clkm_Get_Handle("g_clk_tso_in");
        Drv_Clkm_Set_Clk_Source(s32Handle,"CLK_TSOIN0_PAD0");

    #else

        // Enable TSP Clk
        TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK) & ~REG_CLKGEN0_TSP_CLK_MASK)
                | (REG_CLKGEN0_TSP_SRC_192MHZ << REG_CLKGEN0_TSP_SRC_SHIFT);

        // Enable STC Clk
        TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK) & ~REG_CLKGEN0_STC0_MASK)
                | (REG_CLKGEN0_STC_SRC_SYNTH << (REG_CLKGEN0_STC_SRC_SHIFT+REG_CLKGEN0_STC0_SHIFT));

        TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK) & ~REG_CLKGEN0_STC1_MASK)
                | (REG_CLKGEN0_STC_SRC_SYNTH << (REG_CLKGEN0_STC_SRC_SHIFT+REG_CLKGEN0_STC1_SHIFT));

        TSP_CLKGEN2_REG(REG_CLKGEN2_STC2_CLK) = (TSP_CLKGEN2_REG(REG_CLKGEN2_STC2_CLK) & ~REG_CLKGEN2_STC2_MASK)
                | (REG_CLKGEN2_STC_SRC_SYNTH << (REG_CLKGEN2_STC_SRC_SHIFT+REG_CLKGEN2_STC2_SHIFT));

        TSP_CLKGEN2_REG(REG_CLKGEN2_STC3_CLK) = (TSP_CLKGEN2_REG(REG_CLKGEN2_STC3_CLK) & ~REG_CLKGEN2_STC3_MASK)
                | (REG_CLKGEN2_STC_SRC_SYNTH << (REG_CLKGEN2_STC_SRC_SHIFT+REG_CLKGEN2_STC3_SHIFT));

        TSP_CLKGEN1_REG(REG_CLKGEN1_STC4_CLK) = (TSP_CLKGEN1_REG(REG_CLKGEN1_STC4_CLK) & ~REG_CLKGEN1_STC4_MASK)
                | (REG_CLKGEN1_STC_SRC_SYNTH << (REG_CLKGEN1_STC_SRC_SHIFT+REG_CLKGEN1_STC4_SHIFT));

        TSP_CLKGEN1_REG(REG_CLKGEN1_STC5_CLK) = (TSP_CLKGEN1_REG(REG_CLKGEN1_STC5_CLK) & ~REG_CLKGEN1_STC5_MASK)
                | (REG_CLKGEN1_STC_SRC_SYNTH << (REG_CLKGEN1_STC_SRC_SHIFT+REG_CLKGEN1_STC5_SHIFT));

        TSP_CLKGEN1_REG(REG_CLKGEN1_STC6_CLK) = (TSP_CLKGEN1_REG(REG_CLKGEN1_STC6_CLK) & ~REG_CLKGEN1_STC6_MASK)
                | (REG_CLKGEN1_STC_SRC_SYNTH << (REG_CLKGEN1_STC_SRC_SHIFT+REG_CLKGEN1_STC6_SHIFT));

        TSP_CLKGEN1_REG(REG_CLKGEN1_STC7_CLK) = (TSP_CLKGEN1_REG(REG_CLKGEN1_STC7_CLK) & ~REG_CLKGEN1_STC7_MASK)
                | (REG_CLKGEN1_STC_SRC_SYNTH << (REG_CLKGEN1_STC_SRC_SHIFT+REG_CLKGEN1_STC7_SHIFT));

        // Stamp
        TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK) & ~REG_CLKGEN0_STAMP_MASK);

        // Parser
        TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK) & ~REG_CLKGEN0_PARSER_MASK);

        // Enable TSIF => Disable TSIF
        // FixME Enable flowset would enable TSx clk so we don't enable TSx clk
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS0_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS1_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS2_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS3_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS4_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS5_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) & ~(REG_CLKGEN0_TS_MASK << REG_CLKGEN0_TS6_SHIFT));

        //TSO0
        TSP_CLKGEN0_REG(REG_CLKGEN0_TSO0_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSO0_CLK) & ~REG_CLKGEN0_TS_MASK);

    #endif

        // TSP Boot clk sel
        TSP_TOP_REG(REG_TOP_TSP_BOOT_CLK_SEL) = (TSP_TOP_REG(REG_TOP_TSP_BOOT_CLK_SEL) & ~REG_TOP_TSP_BOOT_CLK_SEL_MASK) | REG_TOP_TSP_BOOT_CLK_SEL_TSP;

        // TSP SRAM sel
        TSP_MMFI_REG(REG_MMFI_TSP_SEL_SRAM) = TSP_MMFI_REG(REG_MMFI_TSP_SEL_SRAM) | REG_MMFI_TSP_SEL_SRAM_EN;

        REG16_SET(&_RegOtherCtrl->CFG_OTHER_16, CFG_OHTER_16_REG_PREVENT_SRAM_COLLISION);

        // Disable MCM
        REG16_SET(&_RegOtherCtrl->CFG_OTHER_13, CFG_OHTER_13_REG_TSP2MI_REQ_MCM_DISABLE)
    }
    else
    {
        // Enable MCM
        REG16_CLR(&_RegOtherCtrl->CFG_OTHER_13, CFG_OHTER_13_REG_TSP2MI_REQ_MCM_DISABLE);

    #ifdef CONFIG_MSTAR_CLKM

        MS_S32 s32Handle;

        // Disable TSP Clk
        // [2016.03.10] Disable TSP clk for power problem. If AESDMA share clk with TSP. Need to do tsp init before using AESDMA.
        s32Handle = Drv_Clkm_Get_Handle("g_clk_tsp");
        Drv_Clkm_Clk_Gate_Disable(s32Handle);

        // Disable STC Clk
        for(u8Idx = 0; u8Idx < STC_ENG_NUM; u8Idx++)
        {
            strcpy(ClkName, "g_clk_stc");
            sprintf(Postfix, "%d", u8Idx);
            strcat(ClkName, Postfix);

            s32Handle = Drv_Clkm_Get_Handle(ClkName);
            Drv_Clkm_Clk_Gate_Disable(s32Handle);
        }

        // Stamp
        s32Handle = Drv_Clkm_Get_Handle("g_clk_stamp");
        Drv_Clkm_Clk_Gate_Disable(s32Handle);

        // Parser
        s32Handle = Drv_Clkm_Get_Handle("g_clk_parser");
        Drv_Clkm_Clk_Gate_Disable(s32Handle);

        // Disable TSIF
        for(u8Idx = 0; u8Idx < TSP_TSIF_NUM; u8Idx++)
        {
            strcpy(ClkName, "g_clk_ts");
            sprintf(Postfix, "%d", u8Idx);
            strcat(ClkName, Postfix);

            s32Handle = Drv_Clkm_Get_Handle(ClkName);
            Drv_Clkm_Clk_Gate_Disable(s32Handle);
        }

        //TSO0
        s32Handle = Drv_Clkm_Get_Handle("g_clk_tso_in");
        Drv_Clkm_Clk_Gate_Disable(s32Handle);

    #else
        // Disable TSP Clk
        // [2016.03.10] Disable TSP clk for power problem. If AESDMA share clk with TSP. Need to do tsp init before using AESDMA.
        TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TSP_CLK),(REG_CLKGEN0_TSP_DISABLE << REG_CLKGEN0_TSP_SHIFT));

        // Disable STC Clk
        TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_STC0_CLK),(REG_CLKGEN0_STC_DISABLE << REG_CLKGEN0_STC0_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_STC1_CLK),(REG_CLKGEN0_STC_DISABLE << REG_CLKGEN0_STC1_SHIFT));
        TSP_CLKGEN2_REG(REG_CLKGEN2_STC2_CLK)   = _SET_(TSP_CLKGEN2_REG(REG_CLKGEN2_STC2_CLK),(REG_CLKGEN2_STC_DISABLE << REG_CLKGEN2_STC2_SHIFT));
        TSP_CLKGEN2_REG(REG_CLKGEN2_STC3_CLK)   = _SET_(TSP_CLKGEN2_REG(REG_CLKGEN2_STC3_CLK),(REG_CLKGEN2_STC_DISABLE << REG_CLKGEN2_STC3_SHIFT));
        TSP_CLKGEN1_REG(REG_CLKGEN1_STC4_CLK)   = _SET_(TSP_CLKGEN1_REG(REG_CLKGEN1_STC4_CLK),(REG_CLKGEN1_STC_DISABLE << REG_CLKGEN1_STC4_SHIFT));
        TSP_CLKGEN1_REG(REG_CLKGEN1_STC5_CLK)   = _SET_(TSP_CLKGEN1_REG(REG_CLKGEN1_STC5_CLK),(REG_CLKGEN1_STC_DISABLE << REG_CLKGEN1_STC5_SHIFT));
        TSP_CLKGEN1_REG(REG_CLKGEN1_STC6_CLK)   = _SET_(TSP_CLKGEN1_REG(REG_CLKGEN1_STC6_CLK),(REG_CLKGEN1_STC_DISABLE << REG_CLKGEN1_STC6_SHIFT));
        TSP_CLKGEN1_REG(REG_CLKGEN1_STC7_CLK)   = _SET_(TSP_CLKGEN1_REG(REG_CLKGEN1_STC7_CLK),(REG_CLKGEN1_STC_DISABLE << REG_CLKGEN1_STC7_SHIFT));

        // Stamp
        TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK) = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_STAMP_CLK),(REG_CLKGEN0_STAMP_DISABLE << REG_CLKGEN0_STAMP_SHIFT));

        // Parser
        TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK) = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_PARSER_CLK),(REG_CLKGEN0_PARSER_DISABLE << REG_CLKGEN0_PARSER_SHIFT));

        // Disable TSIF clk
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS0_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS1_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS2_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS3_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS4_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS5_SHIFT));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TS6_SHIFT));

        //TSO0
        TSP_CLKGEN0_REG(REG_CLKGEN0_TSO0_CLK)   = _SET_(TSP_CLKGEN0_REG(REG_CLKGEN0_TSO0_CLK),(REG_CLKGEN0_TS_DISABLE << REG_CLKGEN0_TSO0_SHIFT));

    #endif

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

MS_BOOL HAL_TSP_LoadFW(MS_U32 u32FwPhyAddr, MS_U32 u32FwSize)
{

#define _TSP_QMEM_I_MASK            0xffff8000 //code: 0x2000, data: 0x1000, total: 0x3000
#define _TSP_QMEM_I_ADDR_HIT        0x00000000
#define _TSP_QMEM_I_ADDR_MISS       0xffffffff
#define _TSP_QMEM_D_MASK            0xffff8000
#define _TSP_QMEM_D_ADDR_HIT        0x00000000
#define _TSP_QMEM_D_ADDR_MISS       0xffffffff
#define _TSP_QMEM_SIZE              0x2000 // 32K bytes, 32bit aligment  //0x8000  this is 4 byte address

    REG32_W(&_RegCtrl->Cpu_Base, 0); // 16 bytes address unit

    MS_U32              u32DnldCtrl  = 0;
    MS_U32              u32DnldCtrl1 = 0;

    // Check MIU select
    MS_U8               u8MiuSel = 0;
    MS_PHY              phyMiuOffsetFWBuf = 0;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetFWBuf, u32FwPhyAddr);
    REG16_MSK_W(&_RegTopCtrl->CFG_TOP_08, CFG_TOP_08_REG_MIU_SEL_FILEIN_MASK, u8MiuSel); //Filein #0

    u32DnldCtrl  =      (phyMiuOffsetFWBuf >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT;
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
        TSP32_IdrW(&(_REGPid2->Flt[i]), _u32PidDst2Reg[i]);
    }

    u32Size = ((MS_VIRT)&(((REG_SecFlt*)0)->_x54))/sizeof(TSP32);

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
    if(tsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
        return FALSE;
    }

    if(bEnable)
    {
        REG16_SET(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_TS_IF_EN);
    }
    else
    {
        REG16_CLR(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_TS_IF_EN);
    }

    return TRUE;
}

MS_BOOL HAL_TSP_TSIF_SelPad(MS_U32 tsIf, TSP_TS_PAD eTSPad)
{
    if(tsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
        return FALSE;
    }

    MS_U32 clk_src = REG_CLKGEN0_TS_SRC_EXT0;
    MS_U32 pad_src = CFG_TSP_SRC_MUX_PAD_TS0;
#ifdef CONFIG_MSTAR_CLKM
    MS_S32 handle;
    MS_U8  u8NameIdx = 0;
    char* u8ClkSrcNames[] =
    {
        "CLK_TS0_PAD0",
        "CLK_TS0_PAD1",
        "CLK_TS0_PAD2",
        "CLK_TS0_PAD3",
        "CLK_TS0_PAD4",
        "CLK_TS0_PAD5",
        "CLK_TS0_PAD6",
        "CLK_TS0_PADTSO",
        "CLK_TS0_PADTSIO"
    /*
        "CLK_TS0_PADINTER0",
        "CLK_TS0_PADINTER1"
    */
    };
#endif

    //@NOTE
    //EX3~6 are serial mode and ts2_padmax_mode must be 2 or 3.

    switch (eTSPad)
    {
        default:
        case E_TSP_TS_PAD_EXT0:
            clk_src = REG_CLKGEN0_TS_SRC_EXT0;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS0;
            break;
        case E_TSP_TS_PAD_EXT1:
            clk_src = REG_CLKGEN0_TS_SRC_EXT1;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS1;
            break;
        case E_TSP_TS_PAD_EXT2:
            clk_src = REG_CLKGEN0_TS_SRC_EXT2;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS2;
            break;
        case E_TSP_TS_PAD_EXT3:
            clk_src = REG_CLKGEN0_TS_SRC_EXT3;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS3;
            break;
        case E_TSP_TS_PAD_EXT4:
            clk_src = REG_CLKGEN0_TS_SRC_EXT4;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS4;
            break;
        case E_TSP_TS_PAD_EXT5:
            clk_src = REG_CLKGEN0_TS_SRC_EXT5;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS5;
            break;
        case E_TSP_TS_PAD_EXT6:
            clk_src = REG_CLKGEN0_TS_SRC_EXT6;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS6;
            break;
        case E_TSP_TS_PAD_EXT7:
            clk_src = REG_CLKGEN0_TS_SRC_EXT7;
            pad_src = CFG_TSP_SRC_MUX_PAD_TS7;
            break;
        case E_TSP_TS_PAD_TSOUT0:
            clk_src = REG_CLKGEN0_TS_SRC_TSO0;
            pad_src = CFG_TSP_SRC_MUX_PAD_TSO0;
            break;
        case E_TSP_TS_PAD_TSOUT1:
            clk_src = REG_CLKGEN0_TS_SRC_TSO1;
            pad_src = CFG_TSP_SRC_MUX_PAD_TSO1;
            break;
        case E_TSP_TS_PAD_TSIOOUT0:
            clk_src = REG_CLKGEN0_TS_SRC_TSIO0;
            pad_src = CFG_TSP_SRC_MUX_PAD_TSIO;
            break;
    }

    if(tsIf & 0x1)  // Odd
    {
        REG16_MSK_W(&_RegTspSrcCtrl->CFG_TSP_SRC_00_03[(tsIf >> 1)], CFG_TSP_SRC_MUX_ODD_MASK, (pad_src << CFG_TSP_SRC_MUX_ODD_SHIFT));
    }
    else            // Even
    {
        REG16_MSK_W(&_RegTspSrcCtrl->CFG_TSP_SRC_00_03[(tsIf >> 1)], CFG_TSP_SRC_MUX_EVEN_MASK, (pad_src << CFG_TSP_SRC_MUX_EVEN_SHIFT));
    }

#ifdef CONFIG_MSTAR_CLKM
    char    TspClkName[20] = "g_clk_ts";
    char    postfix[10] = "";

    sprintf(postfix, "%d", tsIf);
    strcat(TspClkName, postfix);

    handle = Drv_Clkm_Get_Handle(TspClkName);
    u8NameIdx = clk_src - REG_CLKGEN0_TS_SRC_EXT0;
    Drv_Clkm_Set_Clk_Source(handle, u8ClkSrcNames[u8NameIdx]);
#else
    switch (tsIf)
    {
        case TSP_TSIF0:
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS0_SHIFT)) | (clk_src<<(REG_CLKGEN0_TS0_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case TSP_TSIF1:
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS1_SHIFT)) | (clk_src<<(REG_CLKGEN0_TS1_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case TSP_TSIF2:
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS2_SHIFT)) | (clk_src<<(REG_CLKGEN0_TS2_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case TSP_TSIF3:
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS3_SHIFT)) | (clk_src<<(REG_CLKGEN0_TS3_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case TSP_TSIF4:
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS4_SHIFT)) | (clk_src<<(REG_CLKGEN0_TS4_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case TSP_TSIF5:
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS5_SHIFT)) | (clk_src<<(REG_CLKGEN0_TS5_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        case TSP_TSIF6:
            TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) & ~(REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS6_SHIFT)) | (clk_src<<(REG_CLKGEN0_TS6_SHIFT+REG_CLKGEN0_TS_SRC_SHIFT));
            break;
        default:
            return FALSE;
    }
#endif

    return TRUE;
}

void HAL_TSO_SetTSOOutMUX(MS_BOOL bSet)
{
    //@NOTE: K7U don't have to implement (this function is used for TSO 1.0)
}

MS_BOOL HAL_TSP_TSO_TSIF_SelPad(MS_U32 u32TSOEng, TSP_TS_PAD eTSPad)
{
    //@NOTE: K7U don't have to implement (this function is used for TSO 1.0)
    return TRUE;
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
            case 3:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) |= ((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS3_SHIFT));
                break;
            case 4:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) |= ((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS4_SHIFT));
                break;
            case 5:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) |= ((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS5_SHIFT));
                break;
            case 6:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) |= ((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS6_SHIFT));
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
            case 3:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) &= ~((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS3_SHIFT));
                break;
            case 4:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) &= ~((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS4_SHIFT));
                break;
            case 5:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) &= ~((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS5_SHIFT));
                break;
            case 6:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) &= ~((REG_CLKGEN0_TS_INVERT)<<(REG_CLKGEN0_TS6_SHIFT));
                break;
            default:
                return FALSE;
        }
    }
    return TRUE;
}

MS_BOOL HAL_TSP_TSIF_SelPad_ClkDis(MS_U32 tsIf , MS_BOOL bClkDis)
{
    if(tsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
        return FALSE;
    }

#ifdef CONFIG_MSTAR_CLKM
    MS_S32  s32Handle;
    char    TspClkName[20] = "g_clk_ts";
    char    postfix[10] = "";

    sprintf(postfix, "%d", tsIf);
    strcat(TspClkName, postfix);
#endif

    if (bClkDis)
    {
    #ifdef CONFIG_MSTAR_CLKM
        s32Handle = Drv_Clkm_Get_Handle(TspClkName);
        Drv_Clkm_Clk_Gate_Disable(s32Handle);
    #else
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
            case 3:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) |= ((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS3_SHIFT));
                break;
            case 4:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) |= ((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS4_SHIFT));
                break;
            case 5:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) |= ((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS5_SHIFT));
                break;
            case 6:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) |= ((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS6_SHIFT));
                break;
            default:
                return FALSE;
        }
    #endif
    }
    else
    {
    #ifdef CONFIG_MSTAR_CLKM
        MS_U16 u16ClkSrc;
        MS_U8  u8NameIdx = 0;
        char* u8ClkSrcNames[] =
        {
            "CLK_TS0_PAD0",
            "CLK_TS0_PAD1",
            "CLK_TS0_PAD2",
            "CLK_TS0_PAD3",
            "CLK_TS0_PAD4",
            "CLK_TS0_PAD5",
            "CLK_TS0_PAD6",
            "CLK_TS0_PADTSO",
            "CLK_TS0_PADTSIO"
        };

        switch (tsIf)
        {
            case 0:
                u16ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS0_CLK) & (REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS0_SHIFT)) >> (REG_CLKGEN0_TS0_SHIFT + REG_CLKGEN0_TS_SRC_SHIFT);
                break;
            case 1:
                u16ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS1_CLK) & (REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS1_SHIFT)) >> (REG_CLKGEN0_TS1_SHIFT + REG_CLKGEN0_TS_SRC_SHIFT);
                break;
            case 2:
                u16ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS2_CLK) & (REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS2_SHIFT)) >> (REG_CLKGEN0_TS2_SHIFT + REG_CLKGEN0_TS_SRC_SHIFT);
                break;
            case 3:
                u16ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) & (REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS3_SHIFT)) >> (REG_CLKGEN0_TS3_SHIFT + REG_CLKGEN0_TS_SRC_SHIFT);
                break;
            case 4:
                u16ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) & (REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS4_SHIFT)) >> (REG_CLKGEN0_TS4_SHIFT + REG_CLKGEN0_TS_SRC_SHIFT);
                break;
            case 5:
                u16ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) & (REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS5_SHIFT)) >> (REG_CLKGEN0_TS5_SHIFT + REG_CLKGEN0_TS_SRC_SHIFT);
                break;
            case 6:
                u16ClkSrc = (TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) & (REG_CLKGEN0_TS_MASK<<REG_CLKGEN0_TS6_SHIFT)) >> (REG_CLKGEN0_TS6_SHIFT + REG_CLKGEN0_TS_SRC_SHIFT);
                break;
            default:
                return FALSE;
        }

        if(u16ClkSrc > REG_CLKGEN0_TS_SRC_TSIO0)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[PVR ERROR][%s][%d] Pad not support !!\n",__FUNCTION__,__LINE__));
            return FALSE;
        }

        s32Handle = Drv_Clkm_Get_Handle(TspClkName);
        u8NameIdx = u16ClkSrc - REG_CLKGEN0_TS_SRC_EXT0;
        Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcNames[u8NameIdx]);
    #else
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
            case 3:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS3_CLK) &= ~((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS3_SHIFT));
                break;
            case 4:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS4_CLK) &= ~((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS4_SHIFT));
                break;
            case 5:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS5_CLK) &= ~((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS5_SHIFT));
                break;
            case 6:
                TSP_CLKGEN0_REG(REG_CLKGEN0_TS6_CLK) &= ~((REG_CLKGEN0_TS_DISABLE)<<(REG_CLKGEN0_TS6_SHIFT));
                break;
            default:
                return FALSE;
        }
    #endif
    }

    return TRUE;
}

MS_BOOL HAL_TSP_TSIF_FileEn(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return FALSE;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bEnable)
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_TSP_FILE_SEGMENT);
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TS_DATA_PORT_SEL);
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_TSP_FILE_IN);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_TSP_FILE_SEGMENT);
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TS_DATA_PORT_SEL);
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_TSP_FILE_IN);
    }

    return TRUE;
}

void HAL_TSP_TSIF_BitSwap(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(tsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
        return;
    }

    if(bEnable)
    {
        REG16_SET(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_TS_DATA_SWAP);
    }
    else
    {
        REG16_CLR(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_TS_DATA_SWAP);
    }
}

void HAL_TSP_TSIF_ExtSync(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(tsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
        return;
    }

    if(bEnable)
    {
        REG16_SET(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_EXT_SYNC_SEL);
    }
    else
    {
        REG16_CLR(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_EXT_SYNC_SEL);
    }
}

void HAL_TSP_Filein_Bypass(FILEENG_SEQ eFileEng, MS_BOOL bBypass)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bBypass)
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_PS_MODE_BLOCK);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_PS_MODE_BLOCK);
    }
}

void HAL_TSP_TSIF_Parl(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(tsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
        return;
    }

    if(bEnable)
    {
        REG16_SET(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_P_SEL);
    }
    else
    {
        REG16_CLR(&_RegPathCtrl[tsIf].CFG_PATH_00, CFG_PATH_00_REG_P_SEL);
    }
}

void HAL_TSP_PAD_3Wire(MS_U32 u32Pad, MS_BOOL bEnable)
{
    //@NOTE: After Kano , do NOT need to set
}

void HAL_TSP_TSIF_3Wire(MS_U32 tsIf, MS_BOOL bEnable)
{
    if(tsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
        return;
    }

    if(bEnable)
    {
        REG16_SET(&_RegOtherCtrl->CFG_OTHER_13, (CFG_OTHER_13_REG_3WIRE_SERIAL_MODE_EN << tsIf));
    }
    else
    {
        REG16_CLR(&_RegOtherCtrl->CFG_OTHER_13, (CFG_OTHER_13_REG_3WIRE_SERIAL_MODE_EN << tsIf));
    }
}

void HAL_TSP_Filein_PktSize(FILEENG_SEQ eFileEng, MS_U32 u32PktSize)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);
    MS_U16  u16PktLen = u32PktSize, u16value = 0;


    if(u32PktSize == 130)
    {
        u16PktLen = 188;
    }
    else if(u32PktSize == 134)
    {
        u16PktLen = 192;
    }

    if(u32PktSize == 130 || u32PktSize == 134) //RVU
    {
        u16value = CFG_FILE_0E_REG_RVU_EN | CFG_FILE_0E_REG_RVU_PSI_EN;

        if(u32PktSize == 134)
        {
            u16value |= CFG_FILE_0E_REG_RVU_TIMESTAMP_EN;
        }

        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_0E, u16value);
    }
    else
    {
        u16value = CFG_FILE_0E_REG_RVU_EN | CFG_FILE_0E_REG_RVU_PSI_EN | CFG_FILE_0E_REG_RVU_TIMESTAMP_EN;
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_0E, u16value);
    }

    // file-in init trust count value
    REG16_MSK_W(&RegFileCtrl[u8FileEng].CFG_FILE_0A, CFG_FILE_0A_REG_INIT_TRUST_SYNC_CNT_VALUE_MASK, u16PktLen << CFG_FILE_0A_REG_INIT_TRUST_SYNC_CNT_VALUE_SHIFT);
    REG16_MSK_W(&RegFileCtrl[u8FileEng].CFG_FILE_08, CFG_FILE_08_REG_CHK_PKT_SIZE_MASK, (u16PktLen-1) << CFG_FILE_08_REG_CHK_PKT_SIZE_SHIFT);
}

void HAL_TSP_Filein_Addr(FILEENG_SEQ eFileEng, MS_U32 addr)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    // Check MIU select
    MS_U8   u8MiuSel;
    MS_PHY  phyMiuOffsetFileinAddr;
    MS_U16  u8Shift = eFileEng * TSP_MIU_SEL_BITS_LEN;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetFileinAddr, addr);
    REG16_MSK_W(&_RegTopCtrl->CFG_TOP_08, (CFG_TOP_08_REG_MIU_SEL_FILEIN_MASK << u8Shift), (u8MiuSel << (CFG_TOP_08_REG_MIU_SEL_FILEIN_SHIFT + u8Shift)));
    REG32_W(&RegFileCtrl[u8FileEng].CFG_FILE_02_03, phyMiuOffsetFileinAddr);
}

void HAL_TSP_Filein_Size(FILEENG_SEQ eFileEng, MS_U32 size)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    REG32_W(&RegFileCtrl[u8FileEng].CFG_FILE_04_05, size);
}

void HAL_TSP_Filein_Start(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_06, CFG_FILE_06_REG_FILEIN_RSTART);
}

MS_BOOL HAL_TSP_File_Pause(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return FALSE;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TSIF_PAUSE);
    return TRUE;
}

MS_BOOL HAL_TSP_File_Resume(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return FALSE;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TSIF_PAUSE);
    return TRUE;
}

#if FPGA_TEST

    MS_BOOL HAL_TSP_Filein_Done_Status(FILEENG_SEQ eFileEng)
    {
        return !HAL_TSP_Filein_Status(eFileEng);
    }

    void HAL_TSP_Filein_Init_Trust_Start(FILEENG_SEQ eFileEng)
    {
        if(eFileEng >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
            return;
        }

        REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
        MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_06, (CFG_FILE_06_REG_FILEIN_RSTART | CFG_FILE_06_REG_FILEIN_INIT_TRUST));
    }

    void HAL_TSP_FILEIN_Address_Protect_En(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
    {
        if(eFileEng >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
            return;
        }

        if(bEnable)
        {
            REG16_SET(&_RegOtherCtrl->CFG_OTHER_19, (CFG_OTHER_19_REG_FILEIN0_DMAR_PROTECT_EN << eFileEng));
        }
        else
        {
            REG16_CLR(&_RegOtherCtrl->CFG_OTHER_19, (CFG_OTHER_19_REG_FILEIN0_DMAR_PROTECT_EN << eFileEng));
        }
    }

    void HAL_TSP_FILEIN_Address_Protect(FILEENG_SEQ eFileEng,MS_PHY u32UpBoundAddr, MS_PHY u32LowBoundAddr)
    {
        if(eFileEng >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
            return;
        }

        MS_U8   u8MiuSel;
        MS_PHY  phyMiuOffsetLB, phyMiuOffsetUB;

        _phy_to_miu_offset(u8MiuSel, phyMiuOffsetLB, u32LowBoundAddr);
        _phy_to_miu_offset(u8MiuSel, phyMiuOffsetUB, u32UpBoundAddr);

        if(u8MiuSel < E_CHIP_MIU_NUM)
        {
            MS_U32  u32LBnd = (MS_U32)((phyMiuOffsetLB >> MIU_BUS) & REG_TOP_ProtectCtrl_BND_MASK);
            MS_U32  u32UBnd = (MS_U32)((phyMiuOffsetUB >> MIU_BUS) & REG_TOP_ProtectCtrl_BND_MASK);

            REG32_W(&_RegTopCtrl->CFG_TOP_30_4B[eFileEng].L_BND, u32LBnd);
            REG32_W(&_RegTopCtrl->CFG_TOP_30_4B[eFileEng].U_BND, u32UBnd);
        }
    }

#endif

void HAL_TSP_Filein_Abort(FILEENG_SEQ eFileEng, MS_BOOL bEn)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bEn)
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TSP_FILEIN_ABORT);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TSP_FILEIN_ABORT);
    }
}

void HAL_TSP_Filein_CmdQRst(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bEnable)
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_RST_CMDQ_FILEIN);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_RST_CMDQ_FILEIN);
    }
}

MS_U32 HAL_TSP_Filein_CmdQSlot(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return 0;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    return 16 - ((REG16_R(&RegFileCtrl[u8FileEng].CFG_FILE_07) & CFG_FILE_07_CMD_WR_CNT_TSIF1_MASK) >> CFG_FILE_07_CMD_WR_CNT_TSIF1_SHIFT);
}

MS_U32 HAL_TSP_Filein_CmdQCnt(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return 0;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    return ((REG16_R(&RegFileCtrl[u8FileEng].CFG_FILE_07) & CFG_FILE_07_CMD_WR_CNT_TSIF1_MASK) >> CFG_FILE_07_CMD_WR_CNT_TSIF1_SHIFT);
}

MS_U32 HAL_TSP_Filein_CmdQLv(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return 0;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    return ((REG16_R(&RegFileCtrl[u8FileEng].CFG_FILE_07) & CFG_FILE_07_CMD_WR_LEVEL_TSIF1_MASK) >> CFG_FILE_07_CMD_WR_LEVEL_TSIF1_SHIFT);
}

void HAL_TSP_Filein_ByteDelay(FILEENG_SEQ eFileEng, MS_U32 delay, MS_BOOL bEnable)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bEnable)
    {
        REG16_W(&RegFileCtrl[u8FileEng].CFG_FILE_09, delay);
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TIMER_EN);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_TIMER_EN);
        REG16_W(&RegFileCtrl[u8FileEng].CFG_FILE_09, 0x0000);
    }
}

MS_U32 HAL_TSP_Filein_Status(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return 0;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    return !(REG16_R(&RegFileCtrl[u8FileEng].CFG_FILE_06) & CFG_FILE_06_REG_FILEIN_DONE);
}

TSP_HAL_FileState HAL_TSP_Filein_GetState(FILEENG_SEQ eFileEng)
{
    TSP_HAL_FileState state     = E_TSP_HAL_FILE_STATE_INVALID;
    MS_U32            u32Status = HAL_TSP_Filein_Status(eFileEng);

    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return E_TSP_HAL_FILE_STATE_INVALID;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(REG16_R(&RegFileCtrl[u8FileEng].CFG_FILE_01) & CFG_FILE_01_REG_TSIF_PAUSE)
    {
        state = E_TSP_HAL_FILE_STATE_PAUSE;
    }
    else if(u32Status)
    {
        state = E_TSP_HAL_FILE_STATE_BUSY;
    }
    else
    {
        state = E_TSP_HAL_FILE_STATE_IDLE;
    }

    return state;
}

void HAL_TSP_Filein_PacketMode(FILEENG_SEQ eFileEng, MS_BOOL bSet)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bSet)
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_PKT192_EN);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_PKT192_EN);
    }
}

void HAL_TSP_Filein_BlockTimeStamp(FILEENG_SEQ eFileEng, MS_BOOL bEn)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bEn)
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_PKT192_BLK_DISABLE);
    }
    else
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_PKT192_BLK_DISABLE);
    }
}

void HAL_TSP_Filein_SetTimeStamp(FILEENG_SEQ eFileEng, MS_U32 u32Stamp)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_LPCR2_WLD);
    REG32_W(&RegFileCtrl[u8FileEng].CFG_FILE_10_11, u32Stamp);
    REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_LPCR2_WLD);

    // Fix pass_pkt_timestamp will hang issue by TSP8,0x60[3] : reg_init_timestamp_restart_en
    // _HAL_TSP_FILEIN_ResetPktTimeStamp(eFileEng, 0);
}

void HAL_TSP_Filein_SetTimeStampClk(FILEENG_SEQ eFileEng, TSP_HAL_TimeStamp_Clk eTimeStampClk)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(eTimeStampClk == E_TSP_HAL_TIMESTAMP_CLK_27M)
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_LPCR_FREG_27M_90K);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_01, CFG_FILE_01_REG_LPCR_FREG_27M_90K);
    }
}

MS_U32 HAL_TSP_Filein_GetTimeStamp(FILEENG_SEQ eFileEng)
{
    MS_U32 u32Stamp = 0;

    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return 0;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_LPCR2_LOAD);
    u32Stamp = REG32_R(&RegFileCtrl[u8FileEng].CFG_FILE_12_13);
    REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_LPCR2_LOAD);

    return u32Stamp;
}

MS_U32 HAL_TSP_Filein_PktTimeStamp(FILEENG_SEQ eFileEng)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return 0;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    return REG32_R(&RegFileCtrl[u8FileEng].CFG_FILE_14_15);
}

void HAL_TSP_Filein_GetCurAddr(FILEENG_SEQ eFileEng, MS_PHY *pu32Addr)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    *pu32Addr = (MS_PHY)(REG32_R(&RegFileCtrl[u8FileEng].CFG_FILE_16_17) & CFG_FILE_16_17_REG_TSP2MI_RADDR_TSIF_MASK);
}

void HAL_TSP_Filein_WbFsmRst(FILEENG_SEQ eFileEng, MS_BOOL bEnable)
{
    if(eFileEng >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FileIn Engine : 0x%x !!\n",__FUNCTION__,__LINE__,eFileEng));
        return;
    }

    REG_FILE_ENG_Ctrl   *RegFileCtrl = (eFileEng < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
    MS_U8   u8FileEng = (eFileEng < E_FILEENG_TSIF4)? eFileEng : (eFileEng - E_FILEENG_TSIF4);

    if(bEnable)
    {
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_WB_FSM_RESET);
    }
    else
    {
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_00, CFG_FILE_00_REG_WB_FSM_RESET);
    }
}

void HAL_TSP_Filein_MOBF_Enable(FILEENG_SEQ eFileEng, MS_BOOL bEnable, MS_U32 u32Key)
{
    // not support
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
            return TSP_PIDFLT_TSIF3;
        case E_TSP_PIDFLT_LIVE3:
            return TSP_PIDFLT_TSIF1;
        case E_TSP_PIDFLT_FILE0:
            return TSP_PIDFLT_TSIF1;
        case E_TSP_PIDFLT_FILE1:
            return TSP_PIDFLT_TSIF3;
        case E_TSP_PIDFLT_FILE2:
            return TSP_PIDFLT_TSIF0;
        case E_TSP_PIDFLT_FILE3:
            return TSP_PIDFLT_TSIF2;
        case E_TSP_PIDFLT_LIVE4:
        case E_TSP_PIDFLT_FILE4:
            return TSP_PIDFLT_TSIF4;
        case E_TSP_PIDFLT_LIVE5:
        case E_TSP_PIDFLT_FILE5:
            return TSP_PIDFLT_TSIF5;
        case E_TSP_PIDFLT_LIVE6:
        case E_TSP_PIDFLT_FILE6:
            return TSP_PIDFLT_TSIF6;
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
    REG_PidFlt *pPidFlt = PPIDFLT0(fltId);
    TSP32_IdrW(pPidFlt, ((TSP32_IdrR(pPidFlt) & ~(TSP_PIDFLT_SRCID_MASK)) | ((u32SrcID << TSP_PIDFLT_SRCID_SHIFT) & TSP_PIDFLT_SRCID_MASK)));
}

void HAL_TSP_PidFlt_SetSecFlt(MS_U32 fltId, MS_U32 u32SecFltId)
{
    REG_PidFlt *pPidFlt = PPIDFLT2(fltId);
    TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_SECFLT_MASK) | ((u32SecFltId << TSP_PIDFLT_SECFLT_SHFT) & TSP_PIDFLT_SECFLT_MASK));
}

void HAL_TSP_PidFlt_SetPvrFlt(MS_U32 fltId, MS_U32 u32PVREng, MS_BOOL bEn)
{
    REG_PidFlt *pPidFlt = PPIDFLT1(fltId);

    if(bEn)
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_PVRFLT_MASK) | (((1 << u32PVREng) << TSP_PIDFLT_PVRFLT_SHIFT) & TSP_PIDFLT_PVRFLT_MASK));
    }
    else
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_PVRFLT_MASK));
    }
}

void HAL_TSP_PidFlt_SetFltRushPass(MS_U32 fltId, MS_U8 u8Enable)
{
    REG_PidFlt *pPidFlt = PPIDFLT0(fltId);

    if(u8Enable)
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) | (TSP_PID_FLT_PKTPUSH_PASS));
    else
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) & ~(TSP_PID_FLT_PKTPUSH_PASS));
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
        j = (i << 2);
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

    TSP32_IdrW( &pSecFlt->Ctrl_1, !!(u16BufId & 0x100));
}

MS_U16 HAL_TSP_SecFlt_GetSecBuf(REG_SecFlt *pSecFlt)
{
    MS_U16  u16RetVal = (TSP32_IdrR(&pSecFlt->Ctrl) & TSP_SECFLT_SECBUF_MASK) >> TSP_SECFLT_SECBUF_SHFT;

    if(TSP32_IdrR(&pSecFlt->Ctrl_1) & TSP_SECFLT_SECBUF_H_MASK)
    {
        u16RetVal |= 0x100;
    }

    return u16RetVal;
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

    // rmn counter doesn't need 32bit (check 13818) so we use some of it to store owner and alloc info
    reg = TSP32_IdrR(&pSecFlt->RmnCnt) & (TSP_SECFLT_OWNER_MASK | TSP_SECFLT_ALLOC_MASK);
    if (reg & TSP_SECFLT_ALLOC_MASK)
    {
        return FALSE;
    }
    reg |= TSP_SECFLT_ALLOC_MASK | ((u16TSPId<<TSP_SECFLT_OWNER_SHFT) & TSP_SECFLT_OWNER_MASK);
    TSP32_IdrW(&pSecFlt->RmnCnt, reg);

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
    // Check MIU select
    MS_U8   u8MiuSel;
    MS_PHY  phyMiuOffsetSecBuf;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetSecBuf, u32StartAddr);
    REG16_MSK_W(&_RegTopCtrl->CFG_TOP_09, CFG_TOP_09_REG_MIU_SEL_SEC_MASK, (u8MiuSel << CFG_TOP_09_REG_MIU_SEL_SEC_SHIFT));
    TSP32_IdrW(&pSecBuf->Start, phyMiuOffsetSecBuf);
    TSP32_IdrW(&pSecBuf->End, phyMiuOffsetSecBuf + u32BufSize);
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
    //@NOTE: K7U don't have to implement
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
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        return;
    }

    if(bEnable)
    {
        REG32_SET(&_RegPcrCtrl[pcrFltId].CFG_PCR_01_02, CFG_PCR_01_02_REG_PIDFLT_PCR_ENPCR);
    }
    else
    {
        REG32_CLR(&_RegPcrCtrl[pcrFltId].CFG_PCR_01_02, CFG_PCR_01_02_REG_PIDFLT_PCR_ENPCR);
    }
}

void HAL_TSP_PcrFlt_SetPid(MS_U32 pcrFltId, MS_U32 u32Pid)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        return;
    }

    REG32_MSK_W(&_RegPcrCtrl[pcrFltId].CFG_PCR_01_02, CFG_PCR_01_02_REG_PIDFLT_PCR_PID_MASK, u32Pid);
}

MS_U32 HAL_TSP_PcrFlt_GetPid(MS_U32 pcrFltId)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        return PVR_PIDFLT_DEF;
    }

    return (REG32_R(&_RegPcrCtrl[pcrFltId].CFG_PCR_01_02) & CFG_PCR_01_02_REG_PIDFLT_PCR_PID_MASK);
}

void HAL_TSP_PcrFlt_SetSrc(MS_U32 pcrFltId, TSP_PCR_SRC src)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        return;
    }

    REG16_MSK_W(&_RegPcrCtrl[pcrFltId].CFG_PCR_00, CFG_PCR_00_REG_PCR_SRC_MASK, src << CFG_PCR_00_REG_PCR_SRC_SHIFT);
}

void HAL_TSP_PcrFlt_SetSrcId(MS_U32 pcrFltId, MS_U32 u32SrcId)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        return;
    }

    REG32_MSK_W(&_RegPcrCtrl[pcrFltId].CFG_PCR_01_02, CFG_PCR_01_02_REG_PIDFLT_PCR_SOURCE_MASK, u32SrcId << CFG_PCR_01_02_REG_PIDFLT_PCR_SOURCE_SHIFT);
}

void HAL_TSP_PcrFlt_GetSrc(MS_U32 pcrFltId, TSP_PCR_SRC *pPcrSrc)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        *pPcrSrc = E_TSP_PCR_SRC_INVALID;
        return;
    }

    *pPcrSrc = (REG16_R(&_RegPcrCtrl[pcrFltId].CFG_PCR_00) & CFG_PCR_00_REG_PCR_SRC_MASK) >> CFG_PCR_00_REG_PCR_SRC_SHIFT;
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
            ePcrSrc = E_TSP_PCR_SRC_TSIF3;
            break;
        case E_TSP_PIDFLT_LIVE3:
            ePcrSrc = E_TSP_PCR_SRC_TSIF1;
            break;
        case E_TSP_PIDFLT_FILE0:
            ePcrSrc = E_TSP_PCR_SRC_TSIF1;
            break;
        case E_TSP_PIDFLT_FILE1:
            ePcrSrc = E_TSP_PCR_SRC_TSIF3;
            break;
        case E_TSP_PIDFLT_FILE2:
            ePcrSrc = E_TSP_PCR_SRC_TSIF0;
            break;
        case E_TSP_PIDFLT_FILE3:
            ePcrSrc = E_TSP_PCR_SRC_TSIF2;
            break;
        case E_TSP_PIDFLT_LIVE4:
        case E_TSP_PIDFLT_FILE4:
            ePcrSrc = E_TSP_PCR_SRC_TSIF4;
            break;
        case E_TSP_PIDFLT_LIVE5:
        case E_TSP_PIDFLT_FILE5:
            ePcrSrc = E_TSP_PCR_SRC_TSIF5;
            break;
        case E_TSP_PIDFLT_LIVE6:
        case E_TSP_PIDFLT_FILE6:
            ePcrSrc = E_TSP_PCR_SRC_TSIF6;
            break;
        default:
            printf("[TSP_ERR][%s][%d] Wrong Flt Src type!!!\n",__FUNCTION__,__LINE__);
            break;
    }
    return ePcrSrc;

}

void HAL_TSP_PcrFlt_GetPcr(MS_U32 pcrFltId, MS_U32 *pu32Pcr_H, MS_U32 *pu32Pcr)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        *pu32Pcr_H = 0;
        *pu32Pcr = 0;
        return;
    }

    REG16_SET(&_RegPcrCtrl[pcrFltId].CFG_PCR_00, CFG_PCR_00_REG_PCR_READ);
    *pu32Pcr = REG32_R(&_RegPcrCtrl[pcrFltId].CFG_PCR_03_04);
    *pu32Pcr_H = REG16_R(&_RegPcrCtrl[pcrFltId].CFG_PCR_05) & CFG_PCR_05_REG_PCR_VALID_BIT32_MASK;
    REG16_CLR(&_RegPcrCtrl[pcrFltId].CFG_PCR_00, CFG_PCR_00_REG_PCR_READ);
}

void HAL_TSP_PcrFlt_Reset(MS_U32 pcrFltId)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        return;
    }

    REG16_CLR(&_RegPcrCtrl[pcrFltId].CFG_PCR_00, CFG_PCR_00_REG_PCR_RESET);
    REG16_SET(&_RegPcrCtrl[pcrFltId].CFG_PCR_00, CFG_PCR_00_REG_PCR_RESET);
}

void HAL_TSP_PcrFlt_ClearInt(MS_U32 pcrFltId)
{
    switch(pcrFltId)
    {
        case 0:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR0_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR0_UPDATE_END);
            break;
        case 1:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR1_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR1_UPDATE_END);
            break;
        case 2:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR2_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR2_UPDATE_END);
            break;
        case 3:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR3_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR3_UPDATE_END);
            break;
        case 4:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR4_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR4_UPDATE_END);
            break;
        case 5:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR5_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR5_UPDATE_END);
            break;
        case 6:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR6_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR6_UPDATE_END);
            break;
        case 7:
            REG16_SET(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR7_UPDATE_END);
            REG16_CLR(&_RegCtrl->HwInt3_Stat,TSP_HWINT3_PCR7_UPDATE_END);
            break;
        default:
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Eng ID : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
            break;
    }
}

MS_U32 HAL_TSP_PcrFlt_GetIntMask(MS_U32 pcrFltId)
{
    if(pcrFltId >= TSP_PCRFLT_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PCR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,pcrFltId));
        return 0;
    }

    return (TSP_HWINT3_PCR0_UPDATE_END_EN << (pcrFltId + 16));
}

void HAL_TSP_STC_Init(void)
{
    /////////////Set STC control by HK////////////////
    // select synth from chip top : bit 1 -> 0 -> controlled by HK
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~REG_CLKGEN0_STC_CW_SEL;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~REG_CLKGEN0_STC1_CW_SEL;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~REG_CLKGEN2_STC2_CW_SEL;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~REG_CLKGEN2_STC3_CW_SEL;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~REG_CLKGEN1_STC4_CW_SEL;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~REG_CLKGEN1_STC5_CW_SEL;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~REG_CLKGEN1_STC6_CW_SEL;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~REG_CLKGEN1_STC7_CW_SEL;

    // set HK STC synth CW
     //if CLK_MPLL_SYN is 432MHz, set 0x28000000;if CLK_MPLL_SYN is 216MHz, set 0x14000000
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_L)  = 0x0000;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_H)  = 0x2800;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_L) = 0x0000;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC1_CW_H) = 0x2800;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC2_CW_L) = 0x0000;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC2_CW_H) = 0x2800;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC3_CW_L) = 0x0000;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC3_CW_H) = 0x2800;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC4_CW_L) = 0x0000;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC4_CW_H) = 0x2800;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC5_CW_L) = 0x0000;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC5_CW_H) = 0x2800;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC6_CW_L) = 0x0000;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC6_CW_H) = 0x2800;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC7_CW_L) = 0x0000;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC7_CW_H) = 0x2800;

    // set STC synth
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC_CW_EN;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC1_CW_EN);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC1_CW_EN;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC1_CW_EN);
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC2_CW_EN);
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) |= REG_CLKGEN2_STC2_CW_EN;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC2_CW_EN);
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC3_CW_EN);
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) |= REG_CLKGEN2_STC3_CW_EN;
    TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC3_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC4_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) |= REG_CLKGEN1_STC4_CW_EN;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC4_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC5_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) |= REG_CLKGEN1_STC5_CW_EN;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC5_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC6_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) |= REG_CLKGEN1_STC6_CW_EN;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC6_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC7_CW_EN);
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) |= REG_CLKGEN1_STC7_CW_EN;
    TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC7_CW_EN);

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
        case 2:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC2_CW_L);
            *u32Sync |= TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC2_CW_H) << 16 ;
            break;
        case 3:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC3_CW_L);
            *u32Sync |= TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC3_CW_H) << 16 ;
            break;
        case 4:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC4_CW_L);
            *u32Sync |= TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC4_CW_H) << 16 ;
            break;
        case 5:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC5_CW_L);
            *u32Sync |= TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC5_CW_H) << 16 ;
            break;
        case 6:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC6_CW_L);
            *u32Sync |= TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC6_CW_H) << 16 ;
            break;
        case 7:
            // get HK STC synth CW
            *u32Sync  = TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC7_CW_L);
            *u32Sync |= TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC7_CW_H) << 16 ;
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
        case 2:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~REG_CLKGEN2_STC2_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC2_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC2_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC2_CW_EN);
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) |=   REG_CLKGEN2_STC2_CW_EN;
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC2_CW_EN);
            break;
        case 3:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~REG_CLKGEN2_STC3_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC3_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_STC3_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC3_CW_EN);
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) |=   REG_CLKGEN2_STC3_CW_EN;
            TSP_CLKGEN2_REG(REG_CLKGEN2_DC0_SYTNTH) &= ~(REG_CLKGEN2_STC3_CW_EN);
            break;
        case 4:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~REG_CLKGEN1_STC4_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC4_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC4_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC4_CW_EN);
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) |=   REG_CLKGEN1_STC4_CW_EN;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC4_CW_EN);
            break;
        case 5:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~REG_CLKGEN1_STC5_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC5_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_STC5_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC5_CW_EN);
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) |=   REG_CLKGEN1_STC5_CW_EN;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC0_SYTNTH) &= ~(REG_CLKGEN1_STC5_CW_EN);
            break;
        case 6:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~REG_CLKGEN1_STC6_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC6_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC6_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC6_CW_EN);
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) |=   REG_CLKGEN1_STC6_CW_EN;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC6_CW_EN);
            break;
        case 7:
            //set STC controller : HK or TSP CPU
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~REG_CLKGEN1_STC7_CW_SEL;

            // set HK STC synth CW
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC7_CW_L) = u32Sync & 0xFFFF;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_STC7_CW_H) = u32Sync >> 16;

            // set STC synth : toggle update bit
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC7_CW_EN);
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) |=   REG_CLKGEN1_STC7_CW_EN;
            TSP_CLKGEN1_REG(REG_CLKGEN1_DC1_SYTNTH) &= ~(REG_CLKGEN1_STC7_CW_EN);
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
        default:
            if(Eng >= STC_ENG_NUM)
            {
                HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong STC Engine : 0x%x !!\n",__FUNCTION__,__LINE__,Eng));
                return;
            }

            MS_U32 u32SetBitMask = CFG_STC_19_1A_REG_SET_BASE_STC << ((Eng - 2) << 2);

            REG32_W(&_RegStcCtrl->CFG_STC_00_17[Eng - 2].STC_L, stcL);
            REG32_W(&_RegStcCtrl->CFG_STC_00_17[Eng - 2].STC_H, stcH);
            REG32_SET(&_RegStcCtrl->CFG_STC_19_1A, u32SetBitMask);
            REG32_CLR(&_RegStcCtrl->CFG_STC_19_1A, u32SetBitMask);
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
        default:
            if(Eng >= STC_ENG_NUM)
            {
                HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong STC Engine : 0x%x !!\n",__FUNCTION__,__LINE__,Eng));
                *pStcH = 0;
                *pStcL = 0;
                return;
            }

            MS_U32 u32LdBitMask = CFG_STC_19_1A_REG_STC_LD << ((Eng - 2) << 2);

            REG32_CLR(&_RegStcCtrl->CFG_STC_19_1A, u32LdBitMask);
            *pStcH = REG32_R(&_RegStcCtrl->CFG_STC_00_17[Eng - 2].STC_H);
            *pStcL = REG32_R(&_RegStcCtrl->CFG_STC_00_17[Eng - 2].STC_L);
            REG32_SET(&_RegStcCtrl->CFG_STC_19_1A, u32LdBitMask);
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
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        REG16_MSK_W(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_INPUT_SRC_MASK, ((MS_U16)pktDmxId) << CFG_AV_02_REG_INPUT_SRC_SHIFT);
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        REG16_MSK_W(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_INPUT_SRC_MASK, ((MS_U16)pktDmxId) << CFG_AV_02_REG_INPUT_SRC_SHIFT);
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_FIFO_GetSrc(TSP_DST_SEQ eFltType, TSP_SRC_SEQ *pktDmxId)
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        *pktDmxId = ((REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02)) & CFG_AV_02_REG_INPUT_SRC_MASK) >> CFG_AV_02_REG_INPUT_SRC_SHIFT;
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        *pktDmxId = ((REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02)) & CFG_AV_02_REG_INPUT_SRC_MASK) >> CFG_AV_02_REG_INPUT_SRC_SHIFT;
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        *pktDmxId = E_TSP_SRC_INVALID;
    }
}

void HAL_TSP_FIFO_BD_AUD_En(TSP_DST_SEQ eAudioType, MS_BOOL bMainChEn, MS_BOOL bEn)
{
    if((eAudioType >= E_TSP_DST_FIFO_AUDIO) && (eAudioType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAudioType - E_TSP_DST_FIFO_AUDIO;

        if(bEn)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_REG_BD_AUD_EN);

            if(bMainChEn)
            {
                REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_REG_MAIN_CHANNEL);
            }
            else
            {
                REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_REG_MAIN_CHANNEL);
            }
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_REG_BD_AUD_EN);
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_REG_MAIN_CHANNEL);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_BD_AUD_En(MS_U32 u32BD, MS_BOOL bEn)
{
    if(bEn)
    {
        switch(u32BD)
        {
            case 0:
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO, TRUE, TRUE);
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO2, FALSE, TRUE);
                break;
            case 1:
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO3, TRUE, TRUE);
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO4, FALSE, TRUE);
                break;
            case 2:
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO5, TRUE, TRUE);
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO6, FALSE, TRUE);
                break;
            default:
                HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
                break;
        }
    }
    else
    {
        switch(u32BD)
        {
            case 0:
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO, FALSE, FALSE);
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO2, FALSE, FALSE);
                break;
            case 1:
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO3, FALSE, FALSE);
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO4, FALSE, FALSE);
                break;
            case 2:
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO5, FALSE, FALSE);
                HAL_TSP_FIFO_BD_AUD_En(E_TSP_DST_FIFO_AUDIO6, FALSE, FALSE);
                break;
            default:
                HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
                break;
        }
    }
}

#if FPGA_TEST

    void HAL_TSP_FIFO_ClearAll()
    {
        // clear ALL FIFO !!!
        MS_U8   u8Idx = 0;

        for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_AFIFO_NUM; u8Idx++)
        {
            REG16_CLR(&_RegAudioCtrl[u8Idx].CFG_AV_00, CFG_AV_00_REG_PS_MODE);
        }

        for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_VFIFO_NUM; u8Idx++)
        {
            REG16_CLR(&_RegVideoCtrl[u8Idx].CFG_AV_00, CFG_AV_00_REG_PS_MODE);
        }
    }

    void HAL_TSP_FIFO_ReadEn(TSP_DST_SEQ eFltType, MS_BOOL bEn)
    {
        if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
        {
            MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

            if(bEn)
            {
                REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_06, CFG_AV_06_REG_PES_RSEL);
            }
            else
            {
                REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_06, CFG_AV_06_REG_PES_RSEL);
            }
        }
        else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
        {
            MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

            if(bEn)
            {
                REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_06, CFG_AV_06_REG_PES_RSEL);
            }
            else
            {
                REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_06, CFG_AV_06_REG_PES_RSEL);
            }
        }
        else
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        }
    }

    MS_U16 HAL_TSP_FIFO_ReadPkt(TSP_DST_SEQ eFltType)
    {
        if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
        {
            MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

            return REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_07) & CFG_AV_07_REG_DEBUG_FIFO_DATA_MASK;
        }
        else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
        {
            MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

            return REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_07) & CFG_AV_07_REG_DEBUG_FIFO_DATA_MASK;
        }
        else
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
            return 0;
        }
    }

    void HAL_TSP_FIFO_Connect(TSP_DST_SEQ eFltType, MS_BOOL bEn)
    {
        if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
        {
            MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

            if(bEn)
            {
                REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_06, CFG_AV_06_REG_FIFO_RD);
            }
            else
            {
                REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_06, CFG_AV_06_REG_FIFO_RD);
            }
        }
        else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
        {
            MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

            if(bEn)
            {
                REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_06, CFG_AV_06_REG_FIFO_RD);
            }
            else
            {
                REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_06, CFG_AV_06_REG_FIFO_RD);
            }
        }
        else
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        }
    }

    MS_U32 HAL_TSP_FIFO_PidHit(TSP_DST_SEQ eFltType)
    {
        if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
        {
            MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

            return REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_08) & CFG_AV_08_REG_PID_MATCH_MASK;
        }
        else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
        {
            MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

            return REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_08) & CFG_AV_08_REG_PID_MATCH_MASK;
        }
        else
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
            return PVR_PIDFLT_DEF;
        }
    }

    void HAL_TSP_TRACE_MARK_En(TSP_DST_SEQ eFltType,MS_BOOL bEn)
    {
        if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
        {
            MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

            if(bEn)
            {
                REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_TRACE_MARK_EN);
            }
            else
            {
                REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_TRACE_MARK_EN);
            }
        }
        else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
        {
            MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

            if(bEn)
            {
                REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_TRACE_MARK_EN);
            }
            else
            {
                REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_TRACE_MARK_EN);
            }
        }
        else
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        }
    }

#endif

//@NOTE for TS mode
//@TODO need to rename (TS enable or PKTDMX_BYPASS)
void HAL_TSP_Flt_Bypass(TSP_DST_SEQ eFltType, MS_BOOL bEn)
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        if(bEn)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_PID_BYPASS);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_PID_BYPASS);
        }
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        if(bEn)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_PID_BYPASS);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_PID_BYPASS);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

//PS MODE
//NEED TO rename
void HAL_TSP_FIFO_Bypass(TSP_DST_SEQ eFltType, MS_BOOL bEn) // @NOTE this function only for Tsif0 fileEng other fileEng has no by pass capability
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        if(bEn)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_PS_MODE);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_PS_MODE);
        }
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        if(bEn)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_PS_MODE);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_PS_MODE);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_FIFO_Bypass_Src(FILEENG_SEQ eFileEng, TSP_DST_SEQ eFltType)
{
    // (K6 HW CL)
    // PS mode source sel need to be independent
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        REG16_MSK_W(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_01, CFG_AV_01_REG_PS_MODE_SRC_MASK, ((eFileEng + 1) << CFG_AV_01_REG_PS_MODE_SRC_SHIFT));
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        REG16_MSK_W(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_01, CFG_AV_01_REG_PS_MODE_SRC_MASK, ((eFileEng + 1) << CFG_AV_01_REG_PS_MODE_SRC_SHIFT));
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_FIFO_Reset(TSP_DST_SEQ eFltType, MS_BOOL bReset)
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        if(bReset)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_01, CFG_AV_01_REG_RESET_FIFO_PARSER);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_01, CFG_AV_01_REG_RESET_FIFO_PARSER);
        }
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        if(bReset)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_01, CFG_AV_01_REG_RESET_FIFO_PARSER);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_01, CFG_AV_01_REG_RESET_FIFO_PARSER);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }

    _delay(1);
}

void HAL_TSP_FIFO_Skip_Scrmb(TSP_DST_SEQ eFltType,MS_BOOL bSkip)
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        if(bSkip)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_MASK_SCR_EN);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_MASK_SCR_EN);
        }
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        if(bSkip)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_MASK_SCR_EN);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_MASK_SCR_EN);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_FIFO_BlockDis(TSP_DST_SEQ eFltType, MS_BOOL bDisable)
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        if(bDisable)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_BLOCK_DISABLE);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_BLOCK_DISABLE);
        }
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        if(bDisable)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_BLOCK_DISABLE);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_BLOCK_DISABLE);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

MS_BOOL HAL_TSP_FIFO_IsReset(TSP_DST_SEQ eFltType)
{
    MS_U16 u16Matched = 0;

    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        u16Matched = REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_01) & CFG_AV_01_REG_RESET_FIFO_PARSER;
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        u16Matched = REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_01) & CFG_AV_01_REG_RESET_FIFO_PARSER;
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        return FALSE;
    }

    return (u16Matched) ? TRUE: FALSE;
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
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        return (REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_06) & CFG_AV_06_REG_FIFO_WR_LEVEL_MASK)  >> CFG_AV_06_REG_FIFO_WR_LEVEL_SHIFT;
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        return (REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_06) & CFG_AV_06_REG_FIFO_WR_LEVEL_MASK)  >> CFG_AV_06_REG_FIFO_WR_LEVEL_SHIFT;
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        return 0;
    }
}

MS_BOOL HAL_TSP_FIFO_Overflow(TSP_DST_SEQ eFltType)
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        return (REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_06) & CFG_AV_06_REG_FIFO_FULL_MASK)  >> CFG_AV_06_REG_FIFO_FULL_SHIFT;
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        return (REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_06) & CFG_AV_06_REG_FIFO_FULL_MASK)  >> CFG_AV_06_REG_FIFO_FULL_SHIFT;
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        return FALSE;
    }
}

MS_BOOL HAL_TSP_FIFO_Empty(TSP_DST_SEQ eFltType)
{
    if((eFltType >= E_TSP_DST_FIFO_AUDIO) && (eFltType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eFltType - E_TSP_DST_FIFO_AUDIO;

        return (REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_06) & CFG_AV_06_REG_FIFO_EMPTY_MASK)  >> CFG_AV_06_REG_FIFO_EMPTY_SHIFT;
    }
    else if(eFltType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eFltType - E_TSP_DST_FIFO_VIDEO;

        return (REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_06) & CFG_AV_06_REG_FIFO_EMPTY_MASK)  >> CFG_AV_06_REG_FIFO_EMPTY_SHIFT;
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        return FALSE;
    }
}

static MS_BOOL _HAL_TSP_VQ_TxConfig(void)
{
    // reg_vq_wr_threshold = 0x8
    // reg_vq_forcefire_cnt_1k= 0xC
    REG16_MSK_W(&_RegOtherCtrl->CFG_OTHER_70, CFG_OTHER_70_REG_VQ_WR_THRESHOLD_MASK, (0x8 << CFG_OTHER_70_REG_VQ_WR_THRESHOLD_SHIFT));
    REG16_MSK_W(&_RegOtherCtrl->CFG_OTHER_70, CFG_OTHER_70_REG_VQ_FORCEFIRE_CNT_1K_MASK, (0xC << CFG_OTHER_70_REG_VQ_FORCEFIRE_CNT_1K_SHIFT));

    return TRUE;
}

MS_BOOL HAL_TSP_SetVQ(MS_PHYADDR u32BaseAddr, MS_U32 u32BufLen)
{
    MS_U32  vqId = 0;
    MS_U32  u32VQ_PktNum = 0;
    MS_U32  u32Addr = 0;
    MS_U32  u32OneBufSize = 0;

    // Check MIU select
    MS_U8   u8MiuSel = 0;

    _phy_to_miu_offset(u8MiuSel, u32Addr, u32BaseAddr);
    REG16_MSK_W(&_RegTopCtrl->CFG_TOP_09, CFG_TOP_09_REG_MIU_SEL_VQ_MASK, (u8MiuSel << CFG_TOP_09_REG_MIU_SEL_VQ_SHIFT));

    u32OneBufSize = ((u32BufLen >> MIU_BUS) / VQ_NUM) << MIU_BUS; //miu alignment
    u32VQ_PktNum = u32OneBufSize / VQ_PACKET_UNIT_LEN;

    _HAL_TSP_VQ_TxConfig();

    for(vqId = 0; vqId < VQ_NUM; vqId ++)
    {
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
    if(vqId >= VQ_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong VQ Engine : 0x%x !!\n",__FUNCTION__,__LINE__,vqId));
        return FALSE;
    }

    REG32_W(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_base, (u32BaseAddr >> MIU_BUS));
    REG16_W(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_size_208byte, u32VQ_PktNum);

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
    if(vqId >= VQ_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong VQ Engine : 0x%x !!\n",__FUNCTION__,__LINE__,vqId));
        return;
    }

    if(bEn)
    {
        REG16_SET(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_ctrl, REG_OTHER_VQ_TX_REG_VQ_RESET);
    }
    else
    {
        REG16_CLR(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_ctrl, REG_OTHER_VQ_TX_REG_VQ_RESET);
    }
}

#if FPGA_TEST

    MS_BOOL HAL_TSP_VQ_Block_Dis(MS_U32 vqId, MS_BOOL bDis)
    {
        if(vqId >= VQ_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong VQ Engine : 0x%x !!\n",__FUNCTION__,__LINE__,vqId));
            return FALSE;
        }

        if(bDis)
        {
            REG16_SET(&_RegOtherCtrl->CFG_OTHER_75, (CFG_OTHER_75_REG_VQ_TX_BLOCK_DISABLE << vqId));
        }
        else
        {
            REG16_CLR(&_RegOtherCtrl->CFG_OTHER_75, (CFG_OTHER_75_REG_VQ_TX_BLOCK_DISABLE << vqId));
        }

        return TRUE;
    }

    void HAL_TSP_VQ_OverflowInt_En(MS_U32 vqId, MS_BOOL bEn)
    {
        if(vqId >= VQ_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong VQ Engine : 0x%x !!\n",__FUNCTION__,__LINE__,vqId));
            return;
        }

        if(bEn)
        {
            REG16_SET(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_ctrl, REG_OTHER_VQ_TX_REG_VQ_OVERFLOW_INT_EN);
        }
        else
        {
            REG16_CLR(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_ctrl, REG_OTHER_VQ_TX_REG_VQ_OVERFLOW_INT_EN);
        }
    }

    void HAL_TSP_VQ_OverflowInt_Clr(MS_U32 vqId)
    {
        if(vqId >= VQ_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong VQ Engine : 0x%x !!\n",__FUNCTION__,__LINE__,vqId));
            return;
        }

        REG16_SET(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_ctrl, REG_OTHER_VQ_TX_REG_VQ_CLR_OVERFLOW_INT);
        REG16_CLR(&_RegOtherCtrl->CFG_OTHER_30_67[vqId].reg_vq_ctrl, REG_OTHER_VQ_TX_REG_VQ_CLR_OVERFLOW_INT);
    }

#endif

void HAL_PVR_Init(MS_U32 u32PVREng, MS_U32 pktDmxId)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_PINGPONG_EN);
    REG16_MSK_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_10, CFG_PVR_10_REG_INPUT_SRC_MASK, (((MS_U16)pktDmxId) << CFG_PVR_10_REG_INPUT_SRC_SHIFT));
}

void HAL_PVR_Exit(MS_U32 u32PVREng)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    //reset pvr control registers
    REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_PINGPONG_EN);
    REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_10, CFG_PVR_10_REG_INPUT_SRC_MASK);

    //reset write address
    REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_RST_WADR);
    REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_RST_WADR);

    //reset time-stamp
    HAL_PVR_SetPVRTimeStamp(u32PVREng, 0);
}

void HAL_PVR_Start(MS_U32 u32PVREng)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    //reset write address
    REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_RST_WADR);
    REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_RST_WADR);

    //enable string to miu
    REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_EN);
}

void HAL_PVR_Stop(MS_U32 u32PVREng)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    //disable string to miu
    REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_EN);
}

void HAL_PVR_Pause(MS_U32 u32PVREng, MS_BOOL bPause)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    if(bPause)
    {
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_PAUSE);
    }
    else
    {
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_STR2MI_PAUSE);
    }
}

void HAL_PVR_RecPid(MS_U32 u32PVREng, MS_BOOL bSet)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    if(bSet)
    {
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_RECORD_ALL);
    }
    else
    {
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_RECORD_ALL);
    }
}

#if FPGA_TEST

    void HAL_PVR_Alignment_Enable(MS_U32 u32PVREng, MS_BOOL bEnable)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        if(bEnable)
        {
            REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_ALIGN_EN);
        }
        else
        {
            REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_ALIGN_EN);
        }
    }

    void HAL_PVR_FlushData(MS_U32 u32PVREng)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_0D, CFG_PVR_0D_REG_FLUSH_PVR_DATA);
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_0D, CFG_PVR_0D_REG_FLUSH_PVR_DATA);
    }

    void HAL_PVR_Skip_Scrmb(MS_U32 u32PVREng, MS_BOOL bSkip)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        if(bSkip)
        {
            REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_MASK_SCR_PVR_EN);
        }
        else
        {
            REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_MASK_SCR_PVR_EN);
        }
    }

    void HAL_PVR_Block_Dis(MS_U32 u32PVREng, MS_BOOL bDisable)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        if(bDisable)
        {
            REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_PVR_BLOCK_DISABLE);
        }
        else
        {
            REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_PVR_BLOCK_DISABLE);
        }
    }

    void HAL_PVR_BurstLen(MS_U32 u32PVREng, MS_U16 u16BurstMode)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        REG16_MSK_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_BURST_LEN_MASK, (u16BurstMode << CFG_PVR_00_REG_PVR_BURST_LEN_SHIFT));
    }

    void HAL_PVR_TimeStamp_Stream_En(MS_U32 u32pktDmxId, MS_BOOL bEnable)
    {
        // not support
    }

    void HAL_PVR_MOBF_Enable(MS_U32 u32PVREng, MS_BOOL bEnable, MS_U32 u32Key)
    {
        // not support
    }

    void HAL_PVR_TimeStamp_Sel(MS_U32 u32PVREng, MS_BOOL bLocal_Stream)
    {
        if (u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        if (bLocal_Stream)  //Stream
        {
            REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_0D, CFG_PVR_0D_REG_PVR_TIMESTAMP_SRC_SEL);
        }
        else                //local
        {
            REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_0D, CFG_PVR_0D_REG_PVR_TIMESTAMP_SRC_SEL);
        }
    }

    void HAL_PVR_SetPVRTimeStamp_Stream(MS_U32 u32PVREng , MS_U32 u32Stamp)
    {
        // not supported
    }

    void HAL_PVR_PauseTime_En(MS_U32 u32PVREng,MS_BOOL bEnable)
    {
        if (u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        // bypass_timestamp_sel: 1 , timestamp don't need to consider PVR pause time
        //                       0 , timestamp needs to consider PVR pause time
        if(bEnable)
        {
            REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_26, CFG_PVR_26_REG_BYPASS_TIMESTAMP_SEL);
        }
        else
        {
            REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_26, CFG_PVR_26_REG_BYPASS_TIMESTAMP_SEL);
        }
    }

    void HAL_PVR_SetPauseTime(MS_U32 u32PVREng, MS_U32 u32PauseTime)
    {
        if (u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_22_23, u32PauseTime);
    }

    void HAL_PVR_RecNull(MS_U32 u32PVREng, MS_BOOL bSet)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        if(bSet)
        {
            REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_DIS_NULL_PKT);
        }
        else
        {
            REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_20, CFG_PVR_20_REG_DIS_NULL_PKT);
        }
    }

    void HAL_PVR_SetBuf(MS_U32 u32PVREng , MS_U32 u32StartAddr0, MS_U32 u32BufSize0, MS_U32 u32StartAddr1, MS_U32 u32BufSize1)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
        MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

        MS_U32 u32EndAddr0 = u32StartAddr0 + u32BufSize0;
        MS_U32 u32EndAddr1 = u32StartAddr1 + u32BufSize1;

        // head1
        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_01_02, (u32StartAddr0 >> MIU_BUS));
        // end1
        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_05_06, (u32EndAddr0 >> MIU_BUS));
        // mid1
        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_03_04, (u32StartAddr0 >> MIU_BUS));

        // head2
        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_07_08, (u32StartAddr1 >> MIU_BUS));
        // end2
        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_0B_0C, (u32EndAddr1 >> MIU_BUS));
        // mid2
        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_09_0A, (u32StartAddr1 >> MIU_BUS));
    }

#endif

void HAL_PVR_SetStr2Miu_StartAddr(MS_U32 u32PVREng, MS_U32 u32StartAddr0, MS_U32 u32StartAddr1)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    // Check MIU select
    MS_U8   u8MiuSel;
    MS_PHY  phyMiuOffsetPvrBuf0, phyMiuOffsetPvrBuf1;
    MS_U8   u8Shift = u32PVREng * TSP_MIU_SEL_BITS_LEN;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetPvrBuf0, u32StartAddr0);
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetPvrBuf1, u32StartAddr1);

    // Select MIU
    REG32_MSK_W(&_RegTopCtrl->CFG_TOP_05_06, (CFG_TOP_05_06_REG_MIU_SEL_PVR_MASK << u8Shift), (u8MiuSel << (CFG_TOP_05_06_REG_MIU_SEL_PVR_SHIFT + u8Shift)));

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    // head1
    REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_01_02, (phyMiuOffsetPvrBuf0 >> MIU_BUS));
    // head2
    REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_07_08, (phyMiuOffsetPvrBuf1 >> MIU_BUS));
}

void HAL_PVR_SetStr2Miu_MidAddr(MS_U32 u32PVREng, MS_U32 u32MidAddr0, MS_U32 u32MidAddr1)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    // Check MIU select
    MS_U8   u8MiuSel;
    MS_PHY  phyMiuOffsetPvrBuf0, phyMiuOffsetPvrBuf1;
    MS_U8   u8Shift = u32PVREng * TSP_MIU_SEL_BITS_LEN;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetPvrBuf0, u32MidAddr0);
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetPvrBuf1, u32MidAddr1);

    // Select MIU
    REG32_MSK_W(&_RegTopCtrl->CFG_TOP_05_06, (CFG_TOP_05_06_REG_MIU_SEL_PVR_MASK << u8Shift), (u8MiuSel << (CFG_TOP_05_06_REG_MIU_SEL_PVR_SHIFT + u8Shift)));


    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    // mid1
    REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_03_04, (phyMiuOffsetPvrBuf0 >> MIU_BUS));
    // mid2
    REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_09_0A, (phyMiuOffsetPvrBuf1 >> MIU_BUS));
}

void HAL_PVR_SetStr2Miu_EndAddr(MS_U32 u32PVREng, MS_U32 u32EndAddr0, MS_U32 u32EndAddr1)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    // Check MIU select
    MS_U8   u8MiuSel;
    MS_PHY  phyMiuOffsetPvrBuf0, phyMiuOffsetPvrBuf1;
    MS_U8   u8Shift = u32PVREng * TSP_MIU_SEL_BITS_LEN;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetPvrBuf0, u32EndAddr0);
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetPvrBuf1, u32EndAddr1);

    // Select MIU
    REG32_MSK_W(&_RegTopCtrl->CFG_TOP_05_06, (CFG_TOP_05_06_REG_MIU_SEL_PVR_MASK << u8Shift), (u8MiuSel << (CFG_TOP_05_06_REG_MIU_SEL_PVR_SHIFT + u8Shift)));


    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    // end1
    REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_05_06, (u32EndAddr0 >> MIU_BUS));
    // end2
    REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_0B_0C, (u32EndAddr1 >> MIU_BUS));
}

MS_U32 HAL_PVR_GetWritePtr(MS_U32 u32PVREng)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return 0;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    return (REG32_R(&RegPvrCtrl[u8PvrEng].CFG_PVR_13_14) << MIU_BUS);
}


void HAL_PVR_GetEngSrc(MS_U32 u32EngDst, TSP_SRC_SEQ *eSrc)
{
    if(u32EngDst >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32EngDst));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32EngDst < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32EngDst < E_TSP_PVR_PVRENG_8)? u32EngDst : (u32EngDst - E_TSP_PVR_PVRENG_8);

    *eSrc = (REG16_R(&RegPvrCtrl[u8PvrEng].CFG_PVR_10) & CFG_PVR_10_REG_INPUT_SRC_MASK) >> CFG_PVR_10_REG_INPUT_SRC_SHIFT;
}

//   kaiser Only!!!
FILEENG_SEQ HAL_TSP_FilePath2Tsif_Mapping(MS_U32 u32FileEng)
{
    switch (u32FileEng)
    {
        case 0: //File in Eng 0
            return E_FILEENG_TSIF1;
        case 1: //File in Eng 1
            return E_FILEENG_TSIF3;
        case 2: //File in Eng 2
            return E_FILEENG_TSIF0;
        case 3: //File in Eng 3
            return E_FILEENG_TSIF2;
        case 4:
            return E_FILEENG_TSIF4;
        case 5:
            return E_FILEENG_TSIF5;
        case 6:
            return E_FILEENG_TSIF6;
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
            return E_TSP_SRC_PKTDMX2;
        case 1:
            return E_TSP_SRC_PKTDMX3;
        case 2:
            return E_TSP_SRC_PKTDMX2;
        case 3:
            return E_TSP_SRC_PKTDMX3;
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
                u32PhyTSIF = TSP_TSIF3; // @NOTE tsif3 is used for playback0 file-in
            else
                u32PhyTSIF = TSP_TSIF2;
            break;
        case E_TSP_HAL_TSIF_2:
            if(bFileIn)
                u32PhyTSIF = TSP_TSIF0;
            else
                u32PhyTSIF = TSP_TSIF3;
            break;
        case E_TSP_HAL_TSIF_3:
            if(bFileIn)
                u32PhyTSIF = TSP_TSIF2;
            else
                u32PhyTSIF = TSP_TSIF1;
            break;
        case E_TSP_HAL_TSIF_4:
            u32PhyTSIF = TSP_TSIF4;
            break;
        case E_TSP_HAL_TSIF_5:
            u32PhyTSIF = TSP_TSIF5;
            break;
        case E_TSP_HAL_TSIF_6:
            u32PhyTSIF = TSP_TSIF6;
            break;
        case E_TSP_HAL_TSIF_PVR0:
            u32PhyTSIF = TSP_TSIF2;
            break;
        case E_TSP_HAL_TSIF_PVR1:
            u32PhyTSIF = TSP_TSIF3;
            break;
        case E_TSP_HAL_TSIF_PVR2:
            u32PhyTSIF = TSP_TSIF2;
            break;
        case E_TSP_HAL_TSIF_PVR3:
            u32PhyTSIF = TSP_TSIF3;
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
        case E_TSP_SRC_PKTDMX3:
            ePidFltSrc = E_TSP_PIDFLT_FILE1;
            break;
        case E_TSP_SRC_PKTDMX4:
            ePidFltSrc = E_TSP_PIDFLT_LIVE4;
            break;
        case E_TSP_SRC_PKTDMX5:
            ePidFltSrc = E_TSP_PIDFLT_LIVE5;
            break;
        case E_TSP_SRC_PKTDMX6:
            ePidFltSrc = E_TSP_PIDFLT_LIVE6;
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
            case 2:
                return TSP_PIDFLT_OUT_VFIFO3;
            case 3:
                return TSP_PIDFLT_OUT_VFIFO4;
            case 4:
                return TSP_PIDFLT_OUT_VFIFO5;
            case 5:
                return TSP_PIDFLT_OUT_VFIFO6;
            case 6:
                return TSP_PIDFLT_OUT_VFIFO7;
            case 7:
                return TSP_PIDFLT_OUT_VFIFO8;
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
            case 4:
                return TSP_PIDFLT_OUT_AFIFO5;
            case 5:
                return TSP_PIDFLT_OUT_AFIFO6;
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
            case 2:
                return TSP_PIDFLT_OUT_PVR3;
            case 3:
                return TSP_PIDFLT_OUT_PVR4;
            case 4:
                return TSP_PIDFLT_OUT_PVR5;
            case 5:
                return TSP_PIDFLT_OUT_PVR6;
            case 6:
                return TSP_PIDFLT_OUT_PVR7;
            case 7:
                return TSP_PIDFLT_OUT_PVR8;
            case 8:
                return TSP_PIDFLT_OUT_PVR9;
            case 9:
                return TSP_PIDFLT_OUT_PVR10;

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

MS_U32 HAL_TSP_Tsif2Fq_Mapping(MS_U32 u32Tsif)
{
    MS_U32  u32Fq = 0;

    switch (u32Tsif)
    {
        case 0:
            u32Fq = 0;
            break;
        case 1:
            u32Fq = 2;
            break;
        case 2:
            u32Fq = 3;
            break;
        case 3:
            u32Fq = 1;
            break;
        case 4 ... 10: //exclude 7
            u32Fq = u32Tsif;
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
        case 4:
            return E_TSP_TS_PAD_EXT4;
        case 5:
            return E_TSP_TS_PAD_EXT5;
        case 6:
            return E_TSP_TS_PAD_EXT6;
        case 7:
            return E_TSP_TS_PAD_EXT7;
        default:
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[PVR ERROR][%s][%d] mapping 3WirePad:%u error\n",__FUNCTION__,__LINE__,u8Pad3WireId));
            return E_TSP_TS_PAD_INVALID;
    }
}

void HAL_PVR_SetStrPacketMode(MS_U32 u32PVREng, MS_BOOL bSet)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    if(bSet)
    {
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_PKT192_EN);
    }
    else
    {
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_PKT192_EN);
    }
}

MS_U32 HAL_PVR_GetPVRTimeStamp(MS_U32 u32PVREng)
{
    MS_U32 u32lpcr = 0;

    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return 0;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_LPCR1_RLD);
    u32lpcr = REG32_R(&RegPvrCtrl[u8PvrEng].CFG_PVR_17_18);
    REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_LPCR1_RLD);

    return u32lpcr;
}

void HAL_PVR_SetPVRTimeStamp(MS_U32 u32PVREng, MS_U32 u32Stamp)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    //reset pvr control registers
    REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_LPCR1_WLD);
    REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_15_16, u32Stamp);
    REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_00, CFG_PVR_00_REG_PVR_LPCR1_WLD);
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
    REG16_CLR(&_RegCtrl->DBG_SEL, TSP_DBG_SEL_MASK);
    REG16_SET(&_RegCtrl->DBG_SEL, ((u16Sel << TSP_DBG_SEL_SHIFT) & TSP_DBG_SEL_MASK));

    return REG32_R(&_RegCtrl->TSP_Debug);
}

// [ToDo...]
void HAL_TSP_INT_Enable(MS_U32 u32Mask)
{
    // u32Mask [0:7] for bank 0x15 7e bit[0:7]
        /*
            6: DMA read done
            5: HK_INT_FORCE. (it's trigger bit is at bank 15 39 bit[15])
            4: STR2MI_WADR meets STR2MI_MID.
            3: STR2MI_WADR meets STR2MI_TAIL.
            2: dma_status1
            1: dma_status2
            0: dma_status3
        */
    REG16_SET(&_RegCtrl->HwInt_Stat, (TSP_HWINT_EN_MASK & u32Mask));

    // u32Mask [8:15] for bank 0x16 6c bit[0:7]
        /*
            [5] : OTV HW interrupt
            [4] : all DMA write address not in the protect zone interrupt
            [3] : vq0~vq6 overflow interrupt
            [2] : aud_err
            [1] : vid_err
            [0] : reg_hk_int_force (it's trigger bit is at bank 15 44 bit[15])
        */
    REG16_SET(&_RegCtrl->HwInt2_Stat, (TSP_HWINT2_EN_MASK & (u32Mask >> 8)));

     // u32Mask [16:23] for bank 0x16 6f bit[0:7]
        /*
            [7] : pcr filter 7 update finish
            [6] : pcr filter 6 update finish
            [5] : pcr filter 5 update finish
            [4] : pcr filter 4 update finish
            [3] : pcr filter 3 update finish
            [2] : pcr filter 2 update finish
            [1] : pcr filter 1 update finish
            [0] : pcr filter 0 update finish
        */
    REG16_SET(&_RegCtrl->HwInt3_Stat, (TSP_HWINT3_EN_MASK & (u32Mask >> 16)));
}


// [ToDo...]
void HAL_TSP_INT_Disable(MS_U32 u32Mask)
{
    REG16_W(&_RegCtrl->HwInt_Stat,
        (REG16_R(&_RegCtrl->HwInt_Stat) & ~(TSP_HWINT_EN_MASK & (u32Mask))));

    REG16_W(&_RegCtrl->HwInt2_Stat,
        (REG16_R(&_RegCtrl->HwInt2_Stat) & ~(TSP_HWINT2_EN_MASK & (u32Mask >> 8))));

    REG16_W(&_RegCtrl->HwInt3_Stat,
        (REG16_R(&_RegCtrl->HwInt3_Stat) & ~(TSP_HWINT3_EN_MASK & (u32Mask >> 16))));
}


// [ToDo...]
void HAL_TSP_INT_ClrHW(MS_U32 u32Mask)
{
    REG16_SET(&_RegCtrl->HwInt_Stat, ((u32Mask << TSP_HWINT_STATUS_SHIFT) & TSP_HWINT_STATUS_MASK));
    REG16_CLR(&_RegCtrl->HwInt_Stat, ((u32Mask << TSP_HWINT_STATUS_SHIFT) & TSP_HWINT_STATUS_MASK));

    REG16_SET(&_RegCtrl->HwInt2_Stat, (((u32Mask >> 8) << TSP_HWINT2_STATUS_SHIFT) & TSP_HWINT2_STATUS_MASK));
    REG16_CLR(&_RegCtrl->HwInt2_Stat, (((u32Mask >> 8) << TSP_HWINT2_STATUS_SHIFT) & TSP_HWINT2_STATUS_MASK));

    REG16_SET(&_RegCtrl->HwInt3_Stat, (((u32Mask >> 16) << TSP_HWINT3_STATUS_SHIFT) & TSP_HWINT3_STATUS_MASK));
    REG16_CLR(&_RegCtrl->HwInt3_Stat, (((u32Mask >> 16) << TSP_HWINT3_STATUS_SHIFT) & TSP_HWINT3_STATUS_MASK));
}

// [ToDo...]
MS_U32 HAL_TSP_INT_GetHW(void)
{
    MS_U32 status;

    status = (MS_U32)(((REG16_R(&_RegCtrl->HwInt3_Stat) & TSP_HWINT3_STATUS_MASK) >> TSP_HWINT3_STATUS_SHIFT) << 16);


    status |= ((MS_U32)(((REG16_R(&_RegCtrl->HwInt2_Stat) & TSP_HWINT2_STATUS_MASK) >> TSP_HWINT2_STATUS_SHIFT) << 8));


    status |= ((MS_U32)((REG16_R(&_RegCtrl->HwInt_Stat) & TSP_HWINT_STATUS_MASK) >> TSP_HWINT_STATUS_SHIFT));

    return (status & 0x00FFFFFF);
}

void HAL_TSP_INT_ClrSW(void)
{
    REG32_W(&_RegCtrl->SwInt_Stat, 0);
}

MS_U32 HAL_TSP_INT_GetSW(void)
{
    return REG32_R(&_RegCtrl->SwInt_Stat);
}

#if FPGA_TEST

    void HAL_TSP_Set_RcvBuf_Src(MS_U32 bufIdx, MS_U32 inputSrc)
    {
        //@NOTE: K7U don't have to implement (deleted...)
    }

    void HAL_TSP_SPD_Bypass_En(MS_BOOL bByPassEn)
    {
        MS_U8   u8FileEng;

        if(bByPassEn)
        {
            for(u8FileEng = 0; u8FileEng < E_FILEENG_TSIF4; u8FileEng++)
            {
                REG16_SET(&_RegFile0Ctrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_SPD_TSIF_BYPASS);
            }

            for(u8FileEng = 0; u8FileEng <= (E_FILEENG_TSIF6 - E_FILEENG_TSIF4); u8FileEng++)
            {
                REG16_SET(&_RegFile1Ctrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_SPD_TSIF_BYPASS);
            }
        }
        else
        {
            for(u8FileEng = 0; u8FileEng < E_FILEENG_TSIF4; u8FileEng++)
            {
                REG16_CLR(&_RegFile0Ctrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_SPD_TSIF_BYPASS);
            }

            for(u8FileEng = 0; u8FileEng <= (E_FILEENG_TSIF6 - E_FILEENG_TSIF4); u8FileEng++)
            {
                REG16_CLR(&_RegFile1Ctrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_SPD_TSIF_BYPASS);
            }
        }
    }

    void HAL_TSP_FileIn_SPDConfig(MS_U32 tsIf, MS_BOOL CTR_mode)
    {
        if(tsIf >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
            return;
        }

        if(CTR_mode)
        {
            REG16_SET(&_RegSpdCtrl[tsIf].CFG_SPD_05, CFG_SPD_05_REG_CTR_MODE_SPD_FILEIN);   //set CTR mode enable
            REG16_W(&_RegSpdCtrl[tsIf].CFG_SPD_00_03[0], 0x0000);                           //set counter IV
            REG16_W(&_RegSpdCtrl[tsIf].CFG_SPD_00_03[1], 0x0000);
            REG16_W(&_RegSpdCtrl[tsIf].CFG_SPD_00_03[2], 0x0000);
            REG16_W(&_RegSpdCtrl[tsIf].CFG_SPD_00_03[3], 0x0000);
            REG16_W(&_RegSpdCtrl[tsIf].CFG_SPD_04, CFG_SPD_04_CTR_IV_SPD_MAX_1K);           //set counter IV max vld
            REG16_SET(&_RegSpdCtrl[tsIf].CFG_SPD_05, CFG_SPD_05_REG_LOAD_INIT_COUNTER_SPD); //load counter IV
        }
        else
        {
            REG16_CLR(&_RegSpdCtrl[tsIf].CFG_SPD_05, CFG_SPD_05_REG_CTR_MODE_SPD_FILEIN);
        }

        REG16_MSK_W(&_RegOtherCtrl->CFG_OTHER_1F, CFG_OTHER_1F_REG_SRC_AES_FILEIN_KEY_MASK, (tsIf << CFG_OTHER_1F_REG_SRC_AES_FILEIN_KEY_SHIFT));
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[0], 0x0000);    //file-in SPD key
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[1], 0x0000);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[2], 0x0000);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[3], 0x0000);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[4], 0x1111);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[5], 0x1111);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[6], 0x1111);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_28_2F[7], 0x1111);

        REG_FILE_ENG_Ctrl   *RegFileCtrl = (tsIf < E_FILEENG_TSIF4)? _RegFile0Ctrl : _RegFile1Ctrl;
        MS_U8   u8FileEng = (tsIf < E_FILEENG_TSIF4)? tsIf : (tsIf - E_FILEENG_TSIF4);

        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_LOAD_SPD_KEY);
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_SPD_TSIF_BYPASS);
        REG16_SET(&RegFileCtrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_TSIF_SPD_RST);
        REG16_CLR(&RegFileCtrl[u8FileEng].CFG_FILE_0F, CFG_FILE_0F_REG_TSIF_SPD_RST);
    }

#endif

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

void HAL_TSP_PktDmx_RmDupAVPkt(MS_BOOL bEnable)
{
    MS_U8   u8Idx = 0;

    if(bEnable)
    {
        for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_AFIFO_NUM; u8Idx++)
        {
            REG16_SET(&_RegAudioCtrl[u8Idx].CFG_AV_00, CFG_AV_00_REG_DUP_PKT_SKIP);
        }

        for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_VFIFO_NUM; u8Idx++)
        {
            REG16_SET(&_RegVideoCtrl[u8Idx].CFG_AV_00, CFG_AV_00_REG_DUP_PKT_SKIP);
        }
    }
    else
    {
        for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_AFIFO_NUM; u8Idx++)
        {
            REG16_CLR(&_RegAudioCtrl[u8Idx].CFG_AV_00, CFG_AV_00_REG_DUP_PKT_SKIP);
        }

        for(u8Idx = 0; u8Idx < E_TSP_HAL_CAP_VAL_VFIFO_NUM; u8Idx++)
        {
            REG16_CLR(&_RegVideoCtrl[u8Idx].CFG_AV_00, CFG_AV_00_REG_DUP_PKT_SKIP);
        }
    }
}

void HAL_TSP_TEI_RemoveErrorPkt(TSP_HAL_TEI_RmPktType eHalPktType, MS_BOOL bEnable)
{
    if(eHalPktType <= E_TSP_HAL_TEI_REMOVE_AUDIO6_PKT)
    {
        MS_U8   u8AudioFltIdx = eHalPktType - E_TSP_HAL_TEI_REMOVE_AUDIO_PKT;

        if(bEnable)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_PES_ERR_RM_EN);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_00, CFG_AV_00_REG_PES_ERR_RM_EN);
        }
    }
    else if((eHalPktType >= E_TSP_HAL_TEI_REMOVE_VIDEO_PKT) && (eHalPktType <= E_TSP_HAL_TEI_REMOVE_VIDEO8_PKT))
    {
        MS_U8   u8VideoFltIdx = eHalPktType - E_TSP_HAL_TEI_REMOVE_VIDEO_PKT;

        if(bEnable)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_PES_ERR_RM_EN);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_00, CFG_AV_00_REG_PES_ERR_RM_EN);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

#if FPGA_TEST

    void HAL_TSP_TEI_SKIP(MS_U32 tsIf, MS_BOOL bEnable)
    {
        if(tsIf >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,tsIf));
            return;
        }

        if(bEnable)
        {
            REG16_SET(&_RegPathCtrl[tsIf].CFG_PATH_08, CFG_PATH_08_REG_TEI_SKIP_PKT);
        }
        else
        {
            REG16_CLR(&_RegPathCtrl[tsIf].CFG_PATH_08, CFG_PATH_08_REG_TEI_SKIP_PKT);
        }
    }

#endif

void HAL_TSP_OR_Address_Protect_En(MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        REG16_SET(&_RegCtrl->reg160C, TSP_ORZ_DMAW_PROT_EN);
    }
    else
    {
        REG16_CLR(&_RegCtrl->reg160C, TSP_ORZ_DMAW_PROT_EN);
    }
}

void HAL_TSP_OR_Address_Protect(MS_PHY u32UpBoundAddr, MS_PHY u32LowBoundAddr)
{
    MS_U8 u8MiuSel;
    MS_PHY phyMiuOffsetLB, phyMiuOffsetUB;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetLB, u32LowBoundAddr);
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetUB, u32UpBoundAddr);

    if(u8MiuSel < E_CHIP_MIU_NUM)
    {
        REG32_W(&_RegCtrl->ORZ_DMAW_LBND,(MS_U32)((phyMiuOffsetLB >> MIU_BUS) & TSP_ORZ_DMAW_LBND_MASK));
        REG32_W(&_RegCtrl->ORZ_DMAW_UBND,(MS_U32)((phyMiuOffsetUB >> MIU_BUS) & TSP_ORZ_DMAW_UBND_MASK));
    }
}

void HAL_TSP_SEC_Address_Protect_En(MS_BOOL bEn)
{
    if(bEn == TRUE)
    {
        REG16_SET(&_RegCtrl->reg15b4, TSP_SEC_CB_PVR2_DAMW_PROTECT_EN);
    }
    else
    {
        REG16_CLR(&_RegCtrl->reg15b4, TSP_SEC_CB_PVR2_DAMW_PROTECT_EN);
    }
}

void HAL_TSP_SEC_Address_Protect(MS_U8 u8SecID, MS_PHY u32UpBoundAddr, MS_PHY u32LowBoundAddr)
{
    MS_U8 u8MiuSel;
    MS_PHY phyMiuOffsetLB, phyMiuOffsetUB;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetLB, u32LowBoundAddr);
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetUB, u32UpBoundAddr);

    if(u8MiuSel < E_CHIP_MIU_NUM)
    {
        MS_U32  u32LBnd = (MS_U32)((phyMiuOffsetLB >> MIU_BUS) & TSP_DMAW_BND_MASK);
        MS_U32  u32UBnd = (MS_U32)((phyMiuOffsetUB >> MIU_BUS) & TSP_DMAW_BND_MASK);

        switch(u8SecID)
        {
            case 0:
                REG32_W(&_RegCtrl->DMAW_LBND0, u32LBnd);
                REG32_W(&_RegCtrl->DMAW_UBND0, u32UBnd);
                break;
            case 1:
                REG32_W(&_RegCtrl->DMAW_LBND1, u32LBnd);
                REG32_W(&_RegCtrl->DMAW_UBND1, u32UBnd);
                break;
            default:
                break;
        }
    }
}

void HAL_TSP_PVR_Address_Protect_En(MS_U32 u32PVREng, MS_BOOL bEnable)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    if(bEnable)
    {
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_0D, CFG_PVR_0D_REG_PVR_DMAW_PROTECT_EN);
    }
    else
    {
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_0D, CFG_PVR_0D_REG_PVR_DMAW_PROTECT_EN);
    }
}

void HAL_TSP_PVR_Address_Protect(MS_U32 u32PVREng, MS_PHY u32UpBoundAddr, MS_PHY u32LowBoundAddr)
{
    if(u32PVREng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
        return;
    }

    MS_U8 u8MiuSel;
    MS_PHY phyMiuOffsetLB, phyMiuOffsetUB;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetLB, u32LowBoundAddr);
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetUB, u32UpBoundAddr);

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32PVREng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32PVREng < E_TSP_PVR_PVRENG_8)? u32PVREng : (u32PVREng - E_TSP_PVR_PVRENG_8);

    if(u8MiuSel < E_CHIP_MIU_NUM)
    {
        MS_U32  u32LBnd = (MS_U32)(phyMiuOffsetLB >> MIU_BUS);
        MS_U32  u32UBnd = (MS_U32)(phyMiuOffsetUB >> MIU_BUS);

        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_1C_1D, u32LBnd);
        REG32_W(&RegPvrCtrl[u8PvrEng].CFG_PVR_1E_1F, u32UBnd);
    }
}

void HAL_TSP_MMFI_Address_Protect_En(MS_U32 u32MMFIEng,MS_BOOL bEnable)
{
    if(u32MMFIEng >= 2)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong MMFI Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32MMFIEng));
        return;
    }

    if(bEnable)
    {
        REG16_SET(&_RegOtherCtrl->CFG_OTHER_18, (CFG_OTHER_18_REG_MMFI0_DMAR_PROTECT_EN << u32MMFIEng));
    }
    else
    {
        REG16_CLR(&_RegOtherCtrl->CFG_OTHER_18, (CFG_OTHER_18_REG_MMFI0_DMAR_PROTECT_EN << u32MMFIEng));
    }
}

void HAL_TSP_MMFI_Address_Protect(MS_U32 u32MMFIEng, MS_PHY u32UpBoundAddr, MS_PHY u32LowBoundAddr)
{
    if(u32MMFIEng >= 2)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong MMFI Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32MMFIEng));
        return;
    }

    MS_U8 u8MiuSel;
    MS_PHY phyMiuOffsetLB, phyMiuOffsetUB;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetLB, u32LowBoundAddr);
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetUB, u32UpBoundAddr);

    if(u8MiuSel < E_CHIP_MIU_NUM)
    {
        MS_U32  u32LBnd = (MS_U32)((phyMiuOffsetLB >> MIU_BUS) & REG_TOP_ProtectCtrl_BND_MASK);
        MS_U32  u32UBnd = (MS_U32)((phyMiuOffsetUB >> MIU_BUS) & REG_TOP_ProtectCtrl_BND_MASK);

        REG32_W(&_RegTopCtrl->CFG_TOP_50_57[u32MMFIEng].L_BND, u32LBnd);
        REG32_W(&_RegTopCtrl->CFG_TOP_50_57[u32MMFIEng].U_BND, u32UBnd);
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
         _u32PidDst2Reg[i] = TSP32_IdrR(&(_REGPid2->Flt[i]));
    }

    u32Size = ((MS_VIRT)&(((REG_SecFlt*)0)->_x54))/sizeof(TSP32);
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
    MS_U32 u32Data = 0;
    MS_BOOL brtn = FALSE;
    //printf("\033[31m [%s] Mode = %lX , Config0 =%lX , Config1 = %lX \033[m\n",__FUNCTION__,u32Cmd,u32Config0,u32Config1);  // Eason test
    switch (u32Cmd)
    {
        case HAL_DMX_CMD_RUN_DISABLE_SEC_CC_CHECK:
            REG32_W(&_RegCtrl->MCU_Data1, u32Config0);
            REG32_W(&_RegCtrl->MCU_Cmd, TSP_MCU_CMD_SEC_CC_CHECK_DISABLE);
            _delay(10); // supposed TSP is able to respond in 10us
            u32Data = REG32_R(&_RegCtrl->MCU_Data1);
            REG32_W(&_RegCtrl->MCU_Cmd, 0); // write null to clear
            brtn = (u32Data == TSP_MCU_CMD_SEC_CC_CHECK_DISABLE)? TRUE : FALSE;
            break;
        default:
            break;
    }
    return brtn;
}

MS_BOOL HAL_TSP_CAPVR_SPSEnable(MS_U32 u32Eng, MS_U16 u16CaPvrMode, MS_BOOL bEnable)
{
    if(u32Eng >= TSP_PVRENG_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32Eng));
        return FALSE;
    }

    REG_PVR_ENG_Ctrl    *RegPvrCtrl = (u32Eng < E_TSP_PVR_PVRENG_8)? _RegPvrCtrl : _RegPvrCtrl_1;
    MS_U8   u8PvrEng = (u32Eng < E_TSP_PVR_PVRENG_8)? u32Eng : (u32Eng - E_TSP_PVR_PVRENG_8);

    if(bEnable)
    {
        if(u16CaPvrMode != 0)
        {
            REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_10, CFG_PVR_10_REG_REC_CA_UPPER_PATH);
        }

        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_1B, CFG_PVR_1B_REG_PKT_192_SPS_EN);
    }
    else
    {
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_10, CFG_PVR_10_REG_REC_CA_UPPER_PATH);
        REG16_CLR(&RegPvrCtrl[u8PvrEng].CFG_PVR_1B, CFG_PVR_1B_REG_PKT_192_SPS_EN);
    }

    return TRUE;
}

//@TODO
#if FPGA_TEST

    void HAL_TSP_PVR_SPSConfig(MS_U32 u32PVREng, MS_BOOL CTR_mode)
    {
        if(u32PVREng >= TSP_PVRENG_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32PVREng));
            return;
        }

        REG_PVR_ENG_Ctrl    *RegPvrCtrl = NULL;
        REG_SPS_ENG_Ctrl    *RegSpsCtrl = NULL;
        MS_U8               u8PvrEng = 0;

        if(u32PVREng < E_TSP_PVR_PVRENG_8)
        {
            u8PvrEng = u32PVREng;
            RegPvrCtrl = _RegPvrCtrl;
            RegSpsCtrl = _RegSpsCtrl;
        }
        else
        {
            u8PvrEng = u32PVREng - E_TSP_PVR_PVRENG_8;
            RegPvrCtrl = _RegPvrCtrl_1;
            RegSpsCtrl = _RegSpsCtrl_1;
        }

        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_1B, CFG_PVR_1B_REG_PKT_192_SPS_EN);
        REG16_MSK_W(&_RegOtherCtrl->CFG_OTHER_1F, CFG_OTHER_1F_REG_SRC_AES_PVR_KEY_MASK, ((MS_U16)u32PVREng << CFG_OTHER_1F_REG_SRC_AES_PVR_KEY_SHIFT));
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[0], 0x0000);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[1], 0x0000);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[2], 0x0000);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[3], 0x0000);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[4], 0x1111);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[5], 0x1111);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[6], 0x1111);
        REG16_W(&_RegOtherCtrl->CFG_OTHER_20_27[7], 0x1111);
        REG16_SET(&RegPvrCtrl[u8PvrEng].CFG_PVR_1B, CFG_PVR_1B_REG_LOAD_SPS_KEY);

        if(CTR_mode)
        {
            REG16_SET(&RegSpsCtrl[u8PvrEng].CFG_SPS_05, CFG_SPS_05_REG_CTR_MODE_SPS_PVR);         //set CTR mode
            REG16_W(&RegSpsCtrl[u8PvrEng].CFG_SPS_00_03[0], 0x0000);                              //set counter IV
            REG16_W(&RegSpsCtrl[u8PvrEng].CFG_SPS_00_03[1], 0x0000);
            REG16_W(&RegSpsCtrl[u8PvrEng].CFG_SPS_00_03[2], 0x0000);
            REG16_W(&RegSpsCtrl[u8PvrEng].CFG_SPS_00_03[3], 0x0000);
            REG16_W(&RegSpsCtrl[u8PvrEng].CFG_SPS_04, CFG_SPS_04_CTR_IV_SPS_MAX_1K);              //set counter IV max vld
            REG16_SET(&RegSpsCtrl[u8PvrEng].CFG_SPS_05, CFG_SPS_05_REG_LOAD_INIT_COUNTER_SPS);    //load counter IV
        }
        else
        {
            REG16_CLR(&RegSpsCtrl[u8PvrEng].CFG_SPS_05, CFG_SPS_05_REG_CTR_MODE_SPS_PVR);
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
        case 4:
            u16PktDmxSrc = _SET_(u16PktDmxSrc, TSP_MATCH_PID_SRC_PKTDMX4 << TSP_MATCH_PID_SRC_SHIFT);
            break;
        case 5:
            u16PktDmxSrc = _SET_(u16PktDmxSrc, TSP_MATCH_PID_SRC_PKTDMX5 << TSP_MATCH_PID_SRC_SHIFT);
            break;
        case 6:
            u16PktDmxSrc = _SET_(u16PktDmxSrc, TSP_MATCH_PID_SRC_PKTDMX6 << TSP_MATCH_PID_SRC_SHIFT);
            break;
        default:
            break;
    }

    REG16_W(&_RegCtrl->reg15b8, u16PktDmxSrc);

    REG16_W(&_RegCtrl->PVRConfig, _SET_(REG16_R(&_RegCtrl->PVRConfig), TSP_MATCH_PID_LD)); // Set 1 to load match_pid_num

    // get status
    REG16_W(&_RegCtrl->PVRConfig, _CLR_(REG16_R(&_RegCtrl->PVRConfig), TSP_MATCH_PID_LD)); // freeze

    u16WordId =  _CLR_(REG16_R(&_RegCtrl->PVRConfig), TSP_MATCH_PID_SEL_MASK);

    if (u32GroupId < 24)
    {
        u16WordId = _SET_(u16WordId, u32GroupId << TSP_MATCH_PID_SEL_SHIFT);
        REG16_MSK_W(&_RegCtrl->PVRConfig,TSP_MATCH_PID_SEL_MASK, u16WordId);
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
MS_BOOL HAL_TSP_PktConverter_SetSyncByte(MS_U8 u8Path, MS_U8 u8Idx, MS_U8 *pu8SyncByte, MS_BOOL bSet)
{
    if(u8Path >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u8Path));
        return FALSE;
    }

    REG16   *SynCReg =  (u8Path < TSP_TSIF4)?
                        &_RegTspMultiSyncCtrl->CFG_TSP_MULTI_SYNC_00_7F[u8Path].reg_sync_byte[u8Idx >> 1]
                       :&_RegTspMultiSyncCtrl_1->CFG_TSP_MULTI_SYNC_00_7F[(u8Path - TSP_TSIF4)].reg_sync_byte[u8Idx >> 1];
    MS_U16  u16Mask = (u8Idx & 0x1)? CFG_TSP_MULTI_SYNC_BYTE_ODD_MASK : CFG_TSP_MULTI_SYNC_BYTE_EVEN_MASK;
    MS_U16  u16Shift = (u8Idx & 0x1)? CFG_TSP_MULTI_SYNC_BYTE_ODD_SHIFT : CFG_TSP_MULTI_SYNC_BYTE_EVEN_SHIFT;

    if(bSet)
    {
        REG16_MSK_W(SynCReg, u16Mask, (*pu8SyncByte << u16Shift));
    }
    else
    {
        *pu8SyncByte = (MS_U8)((REG16_R(SynCReg) & u16Mask) >> u16Shift);
    }

    return TRUE;
}

MS_BOOL HAL_TSP_PktConverter_SetSrcId(MS_U8 u8Path, MS_U8 u8Idx, MS_U8 *pu8SrcId, MS_BOOL bSet)
{
    if(u8Path >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u8Path));
        return FALSE;
    }

    REG16   *SrcIdReg = (u8Path < TSP_TSIF4)?
                        &_RegTspMultiSyncCtrl->CFG_TSP_MULTI_SYNC_00_7F[u8Path].reg_source_id[u8Idx >> 1]
                       :&_RegTspMultiSyncCtrl_1->CFG_TSP_MULTI_SYNC_00_7F[(u8Path - TSP_TSIF4)].reg_source_id[u8Idx >> 1];
    MS_U16  u16Mask = (u8Idx & 0x1)? CFG_TSP_MULTI_SRC_ID_ODD_MASK : CFG_TSP_MULTI_SRC_ID_EVEN_MASK;
    MS_U16  u16Shift = (u8Idx & 0x1)? CFG_TSP_MULTI_SRC_ID_ODD_SHIFT : CFG_TSP_MULTI_SRC_ID_EVEN_SHIFT;

    if(bSet)
    {
        REG16_MSK_W(SrcIdReg, u16Mask, (*pu8SrcId << u16Shift));
        REG16_SET(SrcIdReg, (CFG_TSP_MULTI_SRC_ID_MULTI_SYNC_BYTE_EN << u16Shift)); // enable bit
    }
    else
    {
        *pu8SrcId = (MS_U8)((REG16_R(SrcIdReg) & u16Mask) >> u16Shift);
    }

    return TRUE;
}

static void HAL_TSP_PktConverter_SetMXLPktHeaderLen(MS_U8 u8Path, MS_U8 u8PktHeaderLen)
{
    if(u8Path >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u8Path));
        return;
    }

    REG16 *PktConverterReg = &_RegPathCtrl[u8Path].CFG_PATH_05;
    REG16_MSK_W(PktConverterReg, CFG_PATH_05_MXL_PKT_HEADER_MASK, (u8PktHeaderLen << CFG_PATH_05_MXL_PKT_HEADER_SHIFT));
}

static void HAL_TSP_PktConverter_SetSyncBytePosition(MS_U8 u8Path, MS_U8 u8SyncBytePos)
{
    if(u8Path >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u8Path));
        return;
    }

    REG16 *PktConverterReg = &_RegPathCtrl[u8Path].CFG_PATH_05;
    REG16_MSK_W(PktConverterReg, CFG_PATH_05_SYNC_BYTE_POSITION_MASK, (u8SyncBytePos << CFG_PATH_05_SYNC_BYTE_POSITION_SHIFT));
}

MS_BOOL HAL_TSP_PktConverter_PktMode(MS_U8 u8Path, TSP_HAL_PKT_MODE ePktMode)
{
    if(u8Path >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u8Path));
        return FALSE;
    }

    REG16 *PktConverterReg = &_RegPathCtrl[u8Path].CFG_PATH_05;

    switch(ePktMode)
    {
        case E_TSP_HAL_PKT_MODE_NORMAL:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_NORMAL_MODE);
            break;
        case E_TSP_HAL_PKT_MODE_CI:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_CIPLUS_MODE);
            break;
        case E_TSP_HAL_PKT_MODE_OPEN_CABLE:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_OPENCABLE_MODE);
            break;
        case E_TSP_HAL_PKT_MODE_ATS:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_ATS_MODE);
            break;
        case E_TSP_HAL_PKT_MODE_MXL_192:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_MXL_MODE);
            HAL_TSP_PktConverter_SetMXLPktHeaderLen(u8Path,4);
            break;
        case E_TSP_HAL_PKT_MODE_MXL_196:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_MXL_MODE);
            HAL_TSP_PktConverter_SetMXLPktHeaderLen(u8Path,8);
            break;
        case E_TSP_HAL_PKT_MODE_MXL_200:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_MXL_MODE);
            HAL_TSP_PktConverter_SetMXLPktHeaderLen(u8Path,12);
            break;
        case E_TSP_HAL_PKT_MODE_ND:
            REG16_MSK_W(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_MODE_MASK, CFG_PATH_05_PKT_CONVERTER_ND_MODE);
            HAL_TSP_PktConverter_SetMXLPktHeaderLen(u8Path,1);
            HAL_TSP_PktConverter_SetSyncBytePosition(u8Path, 1);
            break;
        default:
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong PktConverter Packet Mode!!!\n",__FUNCTION__,__LINE__));
            return FALSE;
    }

    return TRUE;
}

void HAL_TSP_PktConverter_ForceSync(MS_U8 u8Path, MS_BOOL bEnable)
{
    if(u8Path >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u8Path));
        return;
    }

    REG16 *PktConverterReg = &_RegPathCtrl[u8Path].CFG_PATH_05;

    if(bEnable)
    {
        REG16_SET(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_FORCE_SYNCBYTE); // Set 1 to force sync byte be 0x47
    }
    else
    {
        REG16_CLR(PktConverterReg, CFG_PATH_05_PKT_CONVERTER_FORCE_SYNCBYTE);
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

void HAL_TSP_PktConverter_SrcIdFlt(MS_U8 u8Path, MS_BOOL bEnable)
{
    if(u8Path >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u8Path));
        return;
    }

    REG16 *PktConverterReg = &_RegPathCtrl[u8Path].CFG_PATH_05;

    if(bEnable)
    {
        REG16_SET(PktConverterReg, CFG_PATH_05_SRC_ID_FLT_EN);
    }
    else
    {
        REG16_CLR(PktConverterReg, CFG_PATH_05_SRC_ID_FLT_EN);
    }
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
            return E_TSP_SRC_PKTDMX3;
        case E_TSP_HAL_FLOW_LIVE3:
            return E_TSP_SRC_PKTDMX1;
        case E_TSP_HAL_FLOW_FILE0:
            return E_TSP_SRC_PKTDMX1;
        case E_TSP_HAL_FLOW_FILE1:
            return E_TSP_SRC_PKTDMX3;
        case E_TSP_HAL_FLOW_FILE2:
            return E_TSP_SRC_PKTDMX0;
        case E_TSP_HAL_FLOW_FILE3:
            return E_TSP_SRC_PKTDMX2;
        case E_TSP_HAL_FLOW_LIVE4:
        case E_TSP_HAL_FLOW_FILE4:
            return E_TSP_SRC_PKTDMX4;
        case E_TSP_HAL_FLOW_LIVE5:
        case E_TSP_HAL_FLOW_FILE5:
            return E_TSP_SRC_PKTDMX5;
        case E_TSP_HAL_FLOW_LIVE6:
        case E_TSP_HAL_FLOW_FILE6:
            return E_TSP_SRC_PKTDMX6;
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
    //@NOTE: K7U don't have to implement
}

void HAL_TSP_Debug_LockPktCnt_Load(MS_U32 u32TsIf, MS_BOOL bEn)
{
    if(u32TsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32TsIf));
        return;
    }

    if(bEn)
    {
        REG16_SET(&_RegPathCtrl[u32TsIf].CFG_PATH_02, CFG_PATH_02_REG_LOCKED_PKT_CNT_LOAD);
    }
    else
    {
        REG16_CLR(&_RegPathCtrl[u32TsIf].CFG_PATH_02, CFG_PATH_02_REG_LOCKED_PKT_CNT_LOAD);
    }
}

MS_U16 HAL_TSP_Debug_LockPktCnt_Get(MS_U32 u32TsIf, MS_BOOL bLock)
{
    if(u32TsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32TsIf));
        return 0;
    }

    if(bLock)
    {
        REG16_CLR(&_RegPathCtrl[u32TsIf].CFG_PATH_02, CFG_PATH_02_REG_UNLOCKED_PKT_CNT_MODE);
    }
    else
    {
        REG16_SET(&_RegPathCtrl[u32TsIf].CFG_PATH_02, CFG_PATH_02_REG_UNLOCKED_PKT_CNT_MODE);
    }

    return REG16_R(&_RegPathCtrl[u32TsIf].CFG_PATH_03);
}

void HAL_TSP_Debug_LockPktCnt_Clear(MS_U32 u32TsIf)
{
    if(u32TsIf >= TSP_TSIF_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32TsIf));
        return;
    }

    REG16_SET(&_RegPathCtrl[u32TsIf].CFG_PATH_02, CFG_PATH_02_REG_LOCKED_PKT_CNT_CLR);
    REG16_CLR(&_RegPathCtrl[u32TsIf].CFG_PATH_02, CFG_PATH_02_REG_LOCKED_PKT_CNT_CLR);
}

void HAL_TSP_Debug_ClrSrcSel(TSP_SRC_SEQ eClrSrc)
{
    //@NOTE: K7U don't have to implement
}

void HAL_TSP_Debug_AvPktCnt_Src(TSP_DST_SEQ eAvType, TSP_SRC_SEQ ePktDmxId)
{
    //@NOTE: K7U don't have to implement
}

void HAL_TSP_Debug_AvPktCnt_Load(TSP_DST_SEQ eAvType, MS_BOOL bEn)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        if(bEn)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_LOAD);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_LOAD);
        }
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        if(bEn)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_LOAD);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_LOAD);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

MS_U16 HAL_TSP_Debug_AvPktCnt_Get(TSP_DST_SEQ eAvType)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        return REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_05);
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        return REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_05);
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        return 0;
    }
}

void HAL_TSP_Debug_AvPktCnt_Clear(TSP_DST_SEQ eAvType)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_CLR);
        REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_CLR);
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_CLR);
        REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_PKT_CNT_CLR);
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_Debug_DropDisPktCnt_Src(TSP_DST_SEQ eAvType,TSP_SRC_SEQ ePktDmxId)
{
    //@NOTE: K7U don't have to implement
}

void HAL_TSP_Debug_DropPktCnt_Load(TSP_DST_SEQ eAvType, MS_BOOL bEn)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        if(bEn)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_LOAD);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_LOAD);
        }
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        if(bEn)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_LOAD);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_LOAD);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_Debug_DisPktCnt_Load(TSP_DST_SEQ eAvType,MS_BOOL bEn,MS_BOOL bPayload)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        if(bPayload)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_INC_BY_PL);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_INC_BY_PL);
        }

        if(bEn)
        {
            REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_LOAD);
        }
        else
        {
            REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_LOAD);
        }
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        if(bPayload)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_INC_BY_PL);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_INC_BY_PL);
        }

        if(bEn)
        {
            REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_LOAD);
        }
        else
        {
            REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_LOAD);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

MS_U16 HAL_TSP_Debug_DropDisPktCnt_Get(TSP_DST_SEQ eAvType, MS_BOOL bDrop)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        if(bDrop)
        {
            return REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_03);
        }
        else
        {
            return REG16_R(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_04);
        }
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        if(bDrop)
        {
            return REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_03);
        }
        else
        {
            return REG16_R(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_04);
        }
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
        return 0;
    }
}

void HAL_TSP_Debug_DropPktCnt_Clear(TSP_DST_SEQ eAvType)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_CLR);
        REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_CLR);
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_CLR);
        REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DROP_PKT_CNT_CLR);
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

void HAL_TSP_Debug_DisPktCnt_Clear(TSP_DST_SEQ eAvType)
{
    if((eAvType >= E_TSP_DST_FIFO_AUDIO) && (eAvType <= E_TSP_DST_FIFO_AUDIO6))
    {
        MS_U8   u8AudioFltIdx = eAvType - E_TSP_DST_FIFO_AUDIO;

        REG16_SET(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_CLR);
        REG16_CLR(&_RegAudioCtrl[u8AudioFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_CLR);
    }
    else if(eAvType <= E_TSP_DST_FIFO_VIDEO8)
    {
        MS_U8   u8VideoFltIdx = eAvType - E_TSP_DST_FIFO_VIDEO;

        REG16_SET(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_CLR);
        REG16_CLR(&_RegVideoCtrl[u8VideoFltIdx].CFG_AV_02, CFG_AV_02_REG_DIS_CNTR_CLR);
    }
    else
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[AV ERROR][%s][%d] Type not support !!\n",__FUNCTION__,__LINE__));
    }
}

#if FPGA_TEST

    void HAL_TSP_Debug_ErrPktCnt_Src(MS_U32 u32TsIf)
    {
        //@NOTE: K7U don't have to implement
    }

    void HAL_TSP_Debug_ErrPktCnt_Load(MS_U32 u32TsIf, MS_BOOL bEn)
    {
        if(u32TsIf >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32TsIf));
            return;
        }

        if(bEn)
        {
            REG16_SET(&_RegPathCtrl[u32TsIf].CFG_PATH_0C, CFG_PATH_0C_REG_ERR_PKT_CNTR_LOAD);
        }
        else
        {
            REG16_CLR(&_RegPathCtrl[u32TsIf].CFG_PATH_0C, CFG_PATH_0C_REG_ERR_PKT_CNTR_LOAD);
        }
    }

    MS_U16 HAL_TSP_Debug_ErrPktCnt_Get(MS_U32 u32TsIf)
    {
        if(u32TsIf >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32TsIf));
            return 0;
        }

        return REG16_R(&_RegPathCtrl[u32TsIf].CFG_PATH_0B);
    }

    void HAL_TSP_Debug_ErrPktCnt_Clear(MS_U32 u32TsIf)
    {
        if(u32TsIf >= TSP_TSIF_NUM)
        {
            HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong Path Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32TsIf));
            return;
        }

        REG16_SET(&_RegPathCtrl[u32TsIf].CFG_PATH_0C, CFG_PATH_0C_REG_ERR_PKT_CNTR_CLR);
        REG16_CLR(&_RegPathCtrl[u32TsIf].CFG_PATH_0C, CFG_PATH_0C_REG_ERR_PKT_CNTR_CLR);
    }

    void HAL_TSP_Debug_InputPktCnt_Src(MS_U32 u32TsIf)
    {
        //@NOTE: K7U don't have to implement (deleted...)
    }

    void HAL_TSP_Debug_InputPktCnt_Load(MS_U32 u32TsIf,MS_BOOL bEn)
    {
        //@NOTE: K7U don't have to implement (deleted...)
    }

    MS_U16 HAL_TSP_Debug_InputPktCnt_Get(void)
    {
        //@NOTE: K7U don't have to implement (deleted...)
        return 0;
    }

    void HAL_TSP_Debug_InputPktCnt_Clear(MS_U32 u32Tsif)
    {
        //@NOTE: K7U don't have to implement (deleted...)
    }

#endif

MS_BOOL HAL_TSP_FQ_SetMuxSwitch(MS_U32 u32FQEng, MS_U32 u32FQSrc)
{
    // not support
    return TRUE;
}

MS_U32 HAL_TSP_FQ_GetMuxSwitch(MS_U32 u32FQEng)
{
    // not support
    return 0;
}

MS_BOOL HAL_TSP_FQ_FLT_NULL_PKT(MS_U32 u32FQEng, MS_BOOL bFltNull)
{
    if(u32FQEng >= TSP_FQ_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FQ Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32FQEng));
        return FALSE;
    }

    if(bFltNull)
    {
        REG16_SET(&_RegPathCtrl[u32FQEng].CFG_PATH_06, CFG_PATH_06_REG_FILTER_NULL_PKT);
    }
    else
    {
        REG16_CLR(&_RegPathCtrl[u32FQEng].CFG_PATH_06, CFG_PATH_06_REG_FILTER_NULL_PKT);
    }

    return TRUE;
}

MS_BOOL HAL_TSP_FQ_MuxOutPathSrc(MS_U32 u32FQEng, TSP_FQ_MUX_OUT_SRC *peSrc, MS_BOOL bSet)
{
    if(u32FQEng >= TSP_FQ_NUM)
    {
        HAL_TSP_DBGMSG(E_HAL_TSP_DBG_LEVEL_ERR, E_HAL_TSP_DBG_MODEL_ALL, printf("[TSP_ERR][%s][%d] Wrong FQ Engine : 0x%x !!\n",__FUNCTION__,__LINE__,u32FQEng));
        return FALSE;
    }

    if(bSet)
    {
        REG16_MSK_W(&_RegPathCtrl[u32FQEng].CFG_PATH_09, CFG_PATH_09_REG_FIQ_MUX_OUT_PATH_SRC_MASK, ((MS_U16)*peSrc << CFG_PATH_09_REG_FIQ_MUX_OUT_PATH_SRC_SHIFT));
    }
    else
    {
        *peSrc = (TSP_FQ_MUX_OUT_SRC)((REG16_R(&_RegPathCtrl[u32FQEng].CFG_PATH_09) & CFG_PATH_09_REG_FIQ_MUX_OUT_PATH_SRC_MASK) >> CFG_PATH_09_REG_FIQ_MUX_OUT_PATH_SRC_SHIFT);
    }

    return TRUE;
}

void HAL_TSP_CLK_GATING(TSP_HAL_GATING ePath, MS_U32 u32Eng, MS_BOOL bEn)
{
#if 0 //@NOTE: temporarily comment
    if(bEn)
    {
        switch(ePath)
        {
            case E_TSP_HAL_GATING_PATH0:
            case E_TSP_HAL_GATING_PATH1:
            case E_TSP_HAL_GATING_PATH2:
            case E_TSP_HAL_GATING_PATH3:
            case E_TSP_HAL_GATING_PATH4:
            case E_TSP_HAL_GATING_PATH5:
                if(u32Eng > TSP_TSIF_NUM)
                {
                    printf("[%s][%s][%d] UnSupported TSIF : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_SET(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_PATH0 << u32Eng);
                break;

            case E_TSP_HAL_GATING_TSP_ENG:
                REG16_SET(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_TSP_ENG);
                break;
            case E_TSP_HAL_GATING_FIQ:
                REG16_SET(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_FIQ);
                break;

            case E_TSP_HAL_GATING_PVR1:
            case E_TSP_HAL_GATING_PVR2:
            case E_TSP_HAL_GATING_PVR3:
            case E_TSP_HAL_GATING_PVR4:
                if(u32Eng > TSP_PVRENG_NUM)
                {
                    printf("[%s][%s][%d] UnSupported PVR eng : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_SET(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_PVR1 << u32Eng);
                break;

            case E_TSP_HAL_MIU_CLK_GATING_PATH0:
            case E_TSP_HAL_MIU_CLK_GATING_PATH1:
            case E_TSP_HAL_MIU_CLK_GATING_PATH2:
            case E_TSP_HAL_MIU_CLK_GATING_PATH3:
            case E_TSP_HAL_MIU_CLK_GATING_PATH4:
            case E_TSP_HAL_MIU_CLK_GATING_PATH5:
                if(u32Eng > TSP_TSIF_NUM)
                {
                    printf("[%s][%s][%d] UnSupported TSIF : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_SET(&_RegCtrl6->CFG6_7F,CFG6_7F_REG_MIU_CLK_GATING_PATH0 << u32Eng);
                break;

            case E_TSP_HAL_MIU_CLK_GATING_TSP_ENG:
                REG16_SET(&_RegCtrl6->CFG6_7F,CFG6_7F_REG_MIU_CLK_GATING_TSP_ENG);
                break;

            case E_TSP_HAL_GATING_FIQ0:
            case E_TSP_HAL_GATING_FIQ1:
            case E_TSP_HAL_GATING_FIQ2:
            case E_TSP_HAL_GATING_FIQ3:
            case E_TSP_HAL_GATING_FIQ4:
            case E_TSP_HAL_GATING_FIQ5:
                if(u32Eng > TSP_FQ_NUM)
                {
                    printf("[%s][%s][%d] UnSupported FIQ : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_SET(&_RegCtrl6->CFG6_6D,CFG6_6D_REG_CLK_GATING_FIQ0 << u32Eng);
                break;

            case E_TSP_HAL_MIU_CLK_GATING_FIQ0:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ1:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ2:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ3:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ4:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ5:
                if(u32Eng > TSP_FQ_NUM)
                {
                    printf("[%s][%s][%d] UnSupported FIQ : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_SET(&_RegCtrl6->CFG6_6D,CFG6_6D_REG_MIU_CLK_GATING_FIQ0 << u32Eng);
                break;
            default :
                printf("[%s][%s][%d] UnSupported Type : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)ePath);
                break;
        }
    }
    else
    {
        switch(ePath)
        {
            case E_TSP_HAL_GATING_PATH0:
            case E_TSP_HAL_GATING_PATH1:
            case E_TSP_HAL_GATING_PATH2:
            case E_TSP_HAL_GATING_PATH3:
            case E_TSP_HAL_GATING_PATH4:
            case E_TSP_HAL_GATING_PATH5:
                if(u32Eng > TSP_TSIF_NUM)
                {
                    printf("[%s][%s][%d] UnSupported TSIF : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_CLR(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_PATH0 << u32Eng);
                break;

            case E_TSP_HAL_GATING_TSP_ENG:
                REG16_CLR(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_TSP_ENG);
                break;
            case E_TSP_HAL_GATING_FIQ:
                REG16_CLR(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_FIQ);
                break;

            case E_TSP_HAL_GATING_PVR1:
            case E_TSP_HAL_GATING_PVR2:
            case E_TSP_HAL_GATING_PVR3:
            case E_TSP_HAL_GATING_PVR4:
                if(u32Eng > TSP_PVRENG_NUM)
                {
                    printf("[%s][%s][%d] UnSupported PVR eng : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_CLR(&_RegCtrl6->CFG6_79,CFG6_79_REG_CLK_GATING_PVR1 << u32Eng);
                break;

            case E_TSP_HAL_MIU_CLK_GATING_PATH0:
            case E_TSP_HAL_MIU_CLK_GATING_PATH1:
            case E_TSP_HAL_MIU_CLK_GATING_PATH2:
            case E_TSP_HAL_MIU_CLK_GATING_PATH3:
            case E_TSP_HAL_MIU_CLK_GATING_PATH4:
            case E_TSP_HAL_MIU_CLK_GATING_PATH5:
                if(u32Eng > TSP_TSIF_NUM)
                {
                    printf("[%s][%s][%d] UnSupported TSIF : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }

                REG16_CLR(&_RegCtrl6->CFG6_7F,CFG6_7F_REG_MIU_CLK_GATING_PATH0 << u32Eng);
                break;

            case E_TSP_HAL_MIU_CLK_GATING_TSP_ENG:
                REG16_CLR(&_RegCtrl6->CFG6_7F,CFG6_7F_REG_MIU_CLK_GATING_TSP_ENG);
                break;

            case E_TSP_HAL_GATING_FIQ0:
            case E_TSP_HAL_GATING_FIQ1:
            case E_TSP_HAL_GATING_FIQ2:
            case E_TSP_HAL_GATING_FIQ3:
            case E_TSP_HAL_GATING_FIQ4:
            case E_TSP_HAL_GATING_FIQ5:
                if(u32Eng > TSP_FQ_NUM)
                {
                    printf("[%s][%s][%d] UnSupported FIQ : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_CLR(&_RegCtrl6->CFG6_6D,CFG6_6D_REG_CLK_GATING_FIQ0 << u32Eng);
                break;

            case E_TSP_HAL_MIU_CLK_GATING_FIQ0:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ1:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ2:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ3:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ4:
            case E_TSP_HAL_MIU_CLK_GATING_FIQ5:
                if(u32Eng > TSP_FQ_NUM)
                {
                    printf("[%s][%s][%d] UnSupported FIQ : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)u32Eng);
                    return;
                }
                REG16_CLR(&_RegCtrl6->CFG6_6D,CFG6_6D_REG_MIU_CLK_GATING_FIQ1 << u32Eng);
                break;

            default :
                printf("[%s][%s][%d] UnSupported Type : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)ePath);
                break;
        }
    }
#endif
}

void HAL_TSP_Module_Reset(TSP_HAL_RESET_CTRL ePath, MS_U32 u32Idx, MS_BOOL bEn)
{
#if 0 //@NOTE: temporarily comment
    if(bEn)
    {
        switch(ePath)
        {
            case E_TSP_HAL_RESET_CTRL_PKT_CONVERTER0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7A,CFG6_7A_REG_CLK_RESET_PKT_CONVERTER0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_FIQ0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7A,CFG6_7A_REG_CLK_RESET_FIQ0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_VQ_TX0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_VQ_TX0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_VQ_RX:
                REG16_SET(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_VQ_RX);
                break;
            case E_TSP_HAL_RESET_CTRL_VQ_TOP:
                REG16_SET(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_VQ_TOP);
                break;
            case E_TSP_HAL_RESET_CTRL_PKT_DEMUX0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_PKT_DEMUX0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_PVR1:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7C,CFG6_7C_REG_RESET_PVR1 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR1:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7C,CFG6_7C_REG_RESET_TIMESTAMP_SEL_PVR1 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_SP_D0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7C,CFG6_7C_REG_RESET_SP_D0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7D,CFG6_7D_REG_RESET_FILTER_NULL_PKT0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7D,CFG6_7D_REG_RESET_DIRECTV_130_188_0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7E,CFG6_7E_REG_RESET_SRC_ID_PARSER0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_PCRFLT_0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_7E,CFG6_7E_REG_RESET_PCRFLT_0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_PATH0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_PATH0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_OTV:
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_OTV);
                break;
            case E_TSP_HAL_RESET_DEBUG_TABLE:
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_DEBUG_TABLE);
                break;
            case E_TSP_HAL_RESET_DMA_ENG:
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_DMA_ENG);
                break;
            case E_TSP_HAL_RESET_SEC_CMP:
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_SEC_CMP);
                break;
            case E_TSP_HAL_RESET_SECFLT_REG:
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_SECFLT_REG);
                break;
            case E_TSP_HAL_RESET_SEC:
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_SEC);
                break;
            case E_TSP_HAL_RESET_PID_TABLE:
                REG16_SET(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_PID_TABLE);
                break;

            default :
                printf("[%s][%s][%d] UnSupported Type : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)ePath);
                break;
        }
    }
    else
    {
        switch(ePath)
        {
            case E_TSP_HAL_RESET_CTRL_PKT_CONVERTER0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7A,CFG6_7A_REG_CLK_RESET_PKT_CONVERTER0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_FIQ0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7A,CFG6_7A_REG_CLK_RESET_FIQ0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_VQ_TX0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_VQ_TX0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_VQ_RX:
                REG16_CLR(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_VQ_RX);
                break;
            case E_TSP_HAL_RESET_CTRL_VQ_TOP:
                REG16_CLR(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_VQ_TOP);
                break;
            case E_TSP_HAL_RESET_CTRL_PKT_DEMUX0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7B,CFG6_7B_REG_RESET_PKT_DEMUX0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_PVR1:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7C,CFG6_7C_REG_RESET_PVR1 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR1:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7C,CFG6_7C_REG_RESET_TIMESTAMP_SEL_PVR1 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_SP_D0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7C,CFG6_7C_REG_RESET_SP_D0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7D,CFG6_7D_REG_RESET_FILTER_NULL_PKT0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7D,CFG6_7D_REG_RESET_DIRECTV_130_188_0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7E,CFG6_7E_REG_RESET_SRC_ID_PARSER0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_CTRL_PCRFLT_0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_7E,CFG6_7E_REG_RESET_PCRFLT_0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_PATH0:
                if(u32Idx > TSP_TSIF_NUM)
                    return;
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_PATH0 << u32Idx);
                break;
            case E_TSP_HAL_RESET_OTV:
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_OTV);
                break;
            case E_TSP_HAL_RESET_DEBUG_TABLE:
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_DEBUG_TABLE);
                break;
            case E_TSP_HAL_RESET_DMA_ENG:
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_DMA_ENG);
                break;
            case E_TSP_HAL_RESET_SEC_CMP:
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_SEC_CMP);
                break;
            case E_TSP_HAL_RESET_SECFLT_REG:
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_SECFLT_REG);
                break;
            case E_TSP_HAL_RESET_SEC:
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_SEC);
                break;
            case E_TSP_HAL_RESET_PID_TABLE:
                REG16_CLR(&_RegCtrl6->CFG6_6C,CFG6_6C_REG_RESET_PID_TABLE);
                break;

            default :
                printf("[%s][%s][%d] UnSupported Type : %u\n\n",__FILE__,__FUNCTION__,__LINE__, (unsigned int)ePath);
                break;
        }
    }
#endif
}

void HAL_TSP_PidFlt_SetLutEn(MS_U32 fltId, MS_BOOL bEn)
{
    REG_PidFlt *pPidFlt = PPIDFLT0(fltId);

    if(bEn)
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) | TSP_PIDFLT_OUT_LUT));
    }
    else
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_OUT_LUT));
    }
}

void HAL_TSP_PidFlt_SetFqLutEn(MS_U32 fltId, MS_U32 u32FqLutEng, MS_BOOL bEn)
{
    REG_PidFlt *pPidFlt = PPIDFLT2(fltId);

    if(bEn)
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_FIQ_LUT_MASK) | (((1 << u32FqLutEng) << TSP_PIDFLT_FIQ_LUT_SHIFT) & TSP_PIDFLT_FIQ_LUT_MASK));
    }
    else
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_FIQ_LUT_MASK));
    }
}

void HAL_TSP_PidFlt_SetMultiPvrEn(MS_U32 fltId, MS_U32 u32MultiPvrEng, MS_U32 u32MultiPvrChId, MS_BOOL bEn)
{
    REG_PidFlt *pPidFlt = PPIDFLT2(fltId);

    if(bEn)
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_MULTI_PVR_MASK) | (((1 << u32MultiPvrChId) << TSP_PIDFLT_MULTI_PVR_SHIFT) & TSP_PIDFLT_MULTI_PVR_MASK));
    }
    else
    {
        TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~TSP_PIDFLT_MULTI_PVR_MASK));
    }
}

void HAL_TSP_FQ_MMFI_MIU_Sel(TSP_HAL_MIU_SEL_TYPE eType, MS_U8 u8Eng, MS_PHY phyBufStart)
{
    MS_U8   u8MiuSel = 0;
    MS_PHY  phyMiuOffsetFileinAddr = 0;
    MS_U8   u8Shift = 0;

    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetFileinAddr, phyBufStart);

    switch(eType)
    {
        case E_TSP_HAL_MIU_SEL_MMFI:
            u8Shift = u8Eng * TSP_MIU_SEL_BITS_LEN;
            REG16_MSK_W(&_RegTopCtrl->CFG_TOP_09, (CFG_TOP_09_REG_MIU_SEL_MMFI_MASK << u8Shift), (u8MiuSel << (CFG_TOP_09_REG_MIU_SEL_MMFI_SHIFT + u8Shift)));
            break;
        case E_TSP_HAL_MIU_SEL_FQ:
            if(u8Eng < TSP_FQ_MUX_START_ID)
            {
                u8Shift = u8Eng * TSP_MIU_SEL_BITS_LEN;
                REG16_MSK_W(&_RegTopCtrl->CFG_TOP_07, (CFG_TOP_07_REG_MIU_SEL_FIQ_MASK << u8Shift), (u8MiuSel << (CFG_TOP_07_REG_MIU_SEL_FIQ_SHIFT + u8Shift)));
            }
            else
            {
                u8Shift = (u8Eng - TSP_FQ_MUX_START_ID) * TSP_MIU_SEL_BITS_LEN;
                REG16_MSK_W(&_RegTopCtrl->CFG_TOP_09, (CFG_TOP_09_REG_MIU_SEL_FIQ_MUX_MASK << u8Shift), (u8MiuSel << (CFG_TOP_09_REG_MIU_SEL_FIQ_MUX_SHIFT + u8Shift)));
            }
            break;
        default:
            break;
    }
}
