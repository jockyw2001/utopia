////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
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
// file   halPVR_IframeLUT.c
// @brief  Secure PVR Iframe LUT HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "drvSYS.h"
#include "regPVR_IframeLUT.h"
#include "halPVR_IframeLUT.h"

#define K6LITEU01         (MDrv_SYS_GetChipRev() < 0x1)

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

static REG_Ctrl0*   _RegCtrl0                         = NULL;    // PVR_IframeLUT LUT
static REG_Ctrl1*   _RegCtrl1[4]                      = {NULL};  // PVR_IframeLUT FSC


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#define MIU_BUS (4)

//-------------------------------------------------------------------------------------------------
//  Debug Message
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_HAL_PVR_IframeLUT_DBG_LEVEL_NONE,   // no debug message shown
    E_HAL_PVR_IframeLUT_DBG_LEVEL_ERR,    // only shows error message that can't be recover
    E_HAL_PVR_IframeLUT_DBG_LEVEL_WARN,   // error case can be recover, like retry
    E_HAL_PVR_IframeLUT_DBG_LEVEL_EVENT,  // event that is okay but better known, ex: timestamp ring, file circular, etc.
    E_HAL_PVR_IframeLUT_DBG_LEVEL_INFO,   // information for internal parameter
    E_HAL_PVR_IframeLUT_DBG_LEVEL_FUNC,   // Function trace and input parameter trace
    E_HAL_PVR_IframeLUT_DBG_LEVEL_TRACE,  // debug trace
} EN_HAL_PVR_IframeLUT_DBGMSG_LEVEL;

typedef enum
{
    E_HAL_PVR_IframeLUT_DBG_MODEL_NONE,   // @temporarily , need to refine
    E_HAL_PVR_IframeLUT_DBG_MODEL_ALL,
} EN_HAL_PVR_IframeLUT_DBGMSG_MODEL;

//#define HAL_PVR_IframeLUT_DBGMSG(_level,_model,_f) do {if(_u32PVR_IframeLUTDbgLevel >= (_level)&&((_u32PVR_IframeLUTDbgModel&_model)!=0)) (_f);} while(0)
//static MS_U32  _u32PVR_IframeLUTDbgLevel = E_HAL_PVR_IframeLUT_DBG_LEVEL_ERR;
//static MS_U32  _u32PVR_IframeLUTDbgModel = E_HAL_PVR_IframeLUT_DBG_MODEL_ALL;


//[NOTE] Jerry
// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_PVR_IframeLUT_W(reg, value)    do {  (reg)->low  = ((value) & 0x0000FFFF);                          \
                                                      (reg)->high = ((value) >> 16); } while(0)

#define _HAL_REG16_PVR_IframeLUT_W(reg, value)    { (reg)->data = (MS_U16)((value) & 0xFFFF); }

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) &   (bit))
#define SET_FLAG(flag, bit)        ((flag) |=  (bit))
#define RESET_FLAG(flag, bit)      ((flag) &=  (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag) |   (bit))
#define RESET_FLAG1(flag, bit)     ((flag) &   (~(bit)))


#define _REG16_SET_PVR_IframeLUT(reg, value);    _HAL_REG16_PVR_IframeLUT_W(reg, SET_FLAG1(_HAL_REG16_PVR_IframeLUT_R(reg), value));
//#define _REG32_SET_PVR_IframeLUT(reg, value);    _HAL_REG32_PVR_IframeLUT_W(reg, SET_FLAG1(_HAL_REG32_PVR_IframeLUT_R(reg), value));
#define _REG16_CLR_PVR_IframeLUT(reg, value);    _HAL_REG16_PVR_IframeLUT_W(reg, RESET_FLAG1(_HAL_REG16_PVR_IframeLUT_R(reg), value));
//#define _REG32_CLR_PVR_IframeLUT(reg, value);    _HAL_REG32_PVR_IframeLUT_W(reg, RESET_FLAG1(_HAL_REG32_PVR_IframeLUT_R(reg), value));


//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_VIRT              _u32LutRegBase                       = 0;

#define PVR_IframeLUT_WRITE_POINTER_GET(reg)   *((volatile MS_U32*)(_u32LutRegBase + ((0x50480UL + reg)<< 2)))
    #define REG_LUT_WP_PVR1                         0x78
    #define REG_LUT_WP_PVR2                         0x79
    #define REG_LUT_WP_PVR3                         0x7A
    #define REG_LUT_WP_PVR4                         0x7B

#if 0
static MS_U32 _HAL_REG32_PVR_IframeLUT_R(REG32PVR_IframeLUT *reg)
{
    MS_U32     value = 0;
    value  = (reg)->high << 16;
    value |= (reg)->low;
    return value;
}
#endif

static MS_U16 _HAL_REG16_PVR_IframeLUT_R(REG16PVR_IframeLUT *reg)
{
    MS_U16              value = 0;
    value = (reg)->data;
    return value;
}

static void _HAL_PVR_IframeLUT_SetBank(MS_VIRT u32BankAddr)
{
    _u32LutRegBase = u32BankAddr;

    _RegCtrl0    = (REG_Ctrl0*)(u32BankAddr  + 0xE6C00UL);     // PVR_IframeLUT LUT 0x1736
    _RegCtrl1[0] = (REG_Ctrl1*)(u32BankAddr  + 0xE6E00UL);     // PVR_IframeLUT FSC 0x1737
    _RegCtrl1[1] = (REG_Ctrl1*)(u32BankAddr  + 0xE6E80UL);     // PVR_IframeLUT FSC 0x1737
    _RegCtrl1[2] = (REG_Ctrl1*)(u32BankAddr  + 0xE6F00UL);     // PVR_IframeLUT FSC 0x1737
    _RegCtrl1[3] = (REG_Ctrl1*)(u32BankAddr  + 0xE6F80UL);     // PVR_IframeLUT FSC 0x1737
}

static MS_BOOL _HAL_PVR_IframeLUT_SetBuf(MS_U32 u32PVREng , MS_U32 u32StartAddr0, MS_U32 u32BufSize0)
{
    MS_U32 u32EndAddr0 = u32StartAddr0 + u32BufSize0;
    switch(u32PVREng)
    {
        case 0:
            //head1
            _HAL_REG32_PVR_IframeLUT_W(&_RegCtrl0->CFG0_10_11,   u32StartAddr0 >> MIU_BUS);
            //end1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_14_15), u32EndAddr0 >> MIU_BUS);
            //mid1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_12_13), u32EndAddr0 >> (MIU_BUS + 1));
            break;
        case 1:
            //head1
            _HAL_REG32_PVR_IframeLUT_W(&_RegCtrl0->CFG0_16_17,   u32StartAddr0 >> MIU_BUS);
            //end1
            _HAL_REG32_PVR_IframeLUT_W(&_RegCtrl0->CFG0_1A_1B,   u32EndAddr0 >> MIU_BUS);
            //mid1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_18_19), u32EndAddr0 >> (MIU_BUS + 1));
            break;

        case 2:
            //head1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_1C_1D), u32StartAddr0 >> MIU_BUS);
            //end1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_20_21), u32EndAddr0 >> MIU_BUS);
            //mid1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_1E_1F), u32EndAddr0 >> (MIU_BUS + 1));
            break;

        case 3:
            //head1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_22_23), u32StartAddr0 >> MIU_BUS);
            //end1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_26_27), u32EndAddr0 >> MIU_BUS);
            //mid1
            _HAL_REG32_PVR_IframeLUT_W(&(_RegCtrl0->CFG0_24_25), u32EndAddr0 >> (MIU_BUS + 1));
            break;

        default:
            return FALSE;
    }

    // set the addr to LUT
    _REG16_SET_PVR_IframeLUT(&_RegCtrl0->CFG0_00, CFG0_00_REG_TABLE_ID_AUTO_SET);
    _REG16_CLR_PVR_IframeLUT(&_RegCtrl0->CFG0_00, CFG0_00_REG_TABLE_ID_AUTO_SET);
    return TRUE;
}

//
// General API
//

MS_BOOL HAL_PVR_IframeLUT_Init(MS_VIRT u32BankAddr)
{/*
    if(K6LITEU01)
    {
        printf("HAL_PVR_IframeLUT_Init fail: Not Support in K6LITEU01\n");
        return FALSE;
    }*/

    // check if DSCMB inited
    MS_U8 u8RegReceiveBufSDrc = *((volatile MS_U8*)(u32BankAddr + 0xE0400UL + 4 * 0x11));     // TSP3  0x1702, offset 0x11
    // check reg_receive_buf0_src ~ reg_receive_buf3_src = 0b10 Dscrmb
    // 0b1010,1010 = 0xAA
    if(0xAA != u8RegReceiveBufSDrc)
    {
        printf("HAL_PVR_IframeLUT_Init fail!\n");
        return FALSE;
    }

    _HAL_PVR_IframeLUT_SetBank(u32BankAddr);

    // solve endian problem
    // from 32-bytes big-endian
    // to   16-bytes little-endian | 16-bytes little-endian
    _REG16_SET_PVR_IframeLUT(&_RegCtrl0->CFG0_0B, CFG0_0B_REG_LUT2MI_DSWAP_BT_ORDER);

    return TRUE;
}

MS_BOOL HAL_PVR_IframeLUT_Open(MS_U32 u32PVREng, MS_U32 u32StartAddr0, MS_U32 u32BufSize0, HAL_PVR_IframeLUT_VDEC_TYPE eVdec, MS_U32 u32TSCEPath)
{
    if(!_HAL_PVR_IframeLUT_SetBuf(u32PVREng, u32StartAddr0, u32BufSize0))
    {
        printf("_HAL_PVR_IframeLUT_SetBuf fail\n");
        return FALSE;
    }
    if(!HAL_PVR_IframeLUT_SetVdecType(u32PVREng, eVdec))
    {
        printf("HAL_PVR_IframeLUT_SetVdec fail\n");
        return FALSE;
    }

    switch(u32TSCEPath)
    {
        case 0:
            _REG16_SET_PVR_IframeLUT(&_RegCtrl0->CFG0_0B, CFG0_0B_REG_LUT2MI_EN0); // upper path
            break;
        case 1:
            _REG16_SET_PVR_IframeLUT(&_RegCtrl0->CFG0_0B, CFG0_0B_REG_LUT2MI_EN1); // lower path
            break;
        case 2:
            _REG16_SET_PVR_IframeLUT(&_RegCtrl0->CFG0_0B, CFG0_0B_REG_LUT2MI_EN2); // mid path
            break;
        default:
            return FALSE;
    }

    switch(u32PVREng)
    {
        case 0:
            // enable LUT for PVR eng 0
            _REG16_SET_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN0);
            break;
        case 1:
            // enable LUT for PVR eng 1
            _REG16_SET_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN1);
            break;
        case 2:
            // enable LUT for PVR eng 2
            _REG16_SET_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN2);
            break;
        case 3:
            // enable LUT for PVR eng 3
            _REG16_SET_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN3);
            break;
        default:
            return FALSE;
    }

    _REG16_SET_PVR_IframeLUT(&_RegCtrl1[u32PVREng]->CFG1_01,  0x0FE0);
    return TRUE;
}

MS_BOOL HAL_PVR_IframeLUT_Close(MS_U32 u32PVREng)
{
    switch(u32PVREng)
    {
        case 0:
            // disable LUT for PVR eng 0
            _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN0);
            break;
        case 1:
            // disable LUT for PVR eng 1
            _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN1);
            break;
        case 2:
            // disable LUT for PVR eng 2
            _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN2);
            break;
        case 3:
            // disable LUT for PVR eng 3
            _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN3);
            break;
         default:
            return FALSE;
    }

    _REG16_SET_PVR_IframeLUT(&_RegCtrl1[u32PVREng]->CFG1_01,  0x0FE0);

    //_REG16_SET_PVR_IframeLUT(&_RegCtrl0->CFG0_00,  CFG0_00_REG_SW_RSTB);
    //_REG16_CLR_PVR_IframeLUT(&_RegCtrl0->CFG0_00,  CFG0_00_REG_SW_RSTB);
    return TRUE;
}

MS_BOOL HAL_PVR_IframeLUT_SetVdecType(MS_U32 u32PVREng , HAL_PVR_IframeLUT_VDEC_TYPE eVdec)
{
    switch(eVdec)
    {
        case E_HAL_PVR_IframeLUT_MPEG:
            _HAL_REG16_PVR_IframeLUT_W(&_RegCtrl1[u32PVREng]->CFG1_03,   CFG1_03_REG_PVR_CODEC_MPGE);
            break;
        case E_HAL_PVR_IframeLUT_H264:
            _HAL_REG16_PVR_IframeLUT_W(&_RegCtrl1[u32PVREng]->CFG1_03,   CFG1_03_REG_PVR_CODEC_H264);
            break;
        case E_HAL_PVR_IframeLUT_HEVC:
            _HAL_REG16_PVR_IframeLUT_W(&_RegCtrl1[u32PVREng]->CFG1_03,   CFG1_03_REG_PVR_CODEC_HEVC);
            break;
        case E_HAL_PVR_IframeLUT_AVS:
            _HAL_REG16_PVR_IframeLUT_W(&_RegCtrl1[u32PVREng]->CFG1_03,   CFG1_03_REG_PVR_CODEC_AVS );
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

MS_U32 HAL_PVR_IframeLUT_GetWritePtr(MS_U32 u32PVREng)
{
    switch(u32PVREng)
    {
        case 0:
            return (PVR_IframeLUT_WRITE_POINTER_GET(REG_LUT_WP_PVR1) << MIU_BUS);
        case 1:
            return (PVR_IframeLUT_WRITE_POINTER_GET(REG_LUT_WP_PVR2) << MIU_BUS);
        case 2:
            return (PVR_IframeLUT_WRITE_POINTER_GET(REG_LUT_WP_PVR3) << MIU_BUS);
        case 3:
            return (PVR_IframeLUT_WRITE_POINTER_GET(REG_LUT_WP_PVR4) << MIU_BUS);
        default:
            return 0;
    }
}

MS_BOOL HAL_PVR_IframeLUT_Exit(void)
{
    // disable TSCE path
    _REG16_CLR_PVR_IframeLUT(&_RegCtrl0->CFG0_0B, CFG0_0B_REG_LUT2MI_EN0); // upper path
    _REG16_CLR_PVR_IframeLUT(&_RegCtrl0->CFG0_0B, CFG0_0B_REG_LUT2MI_EN1); // lower path
    _REG16_CLR_PVR_IframeLUT(&_RegCtrl0->CFG0_0B, CFG0_0B_REG_LUT2MI_EN2); // mid path

    // disable channel
    _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN0);
    _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN1);
    _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN2);
    _REG16_CLR_PVR_IframeLUT(&(_RegCtrl0->CFG0_00), CFG0_00_REG_CHANNEL_EN3);

    _REG16_SET_PVR_IframeLUT(&_RegCtrl0->CFG0_00,  CFG0_00_REG_SW_RSTB);
    _REG16_CLR_PVR_IframeLUT(&_RegCtrl0->CFG0_00,  CFG0_00_REG_SW_RSTB);

    return TRUE;
}
