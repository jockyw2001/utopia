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
// file   halTSIO.c
// @brief  TS I/O HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "regTSIO.h"
#include "halTSIO.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_Ctrl_TSIO0* _TSIOCtrl0 = NULL;
static REG_Ctrl_TSIO1* _TSIOCtrl1 = NULL;
static REG_Ctrl_TSIO2* _TSIOCtrl2 = NULL;
static REG_Ctrl_TSIO3* _TSIOCtrl3 = NULL;
static REG_Ctrl_TSIO_LOCDEC* _TSIOCtrlLOCDEC = NULL;
static REG_Ctrl_TSO0* _TSOCtrl0 = NULL;
static REG_Ctrl_TSO2* _TSOCtrl2 = NULL;
static REG_Ctrl_TSO3* _TSOCtrl3 = NULL;
static REG_Ctrl_CHIPTOP* _TSIOCHIPTOP = NULL;
static REG_Ctrl_CLOCKGEN0* _TSIO_CLKGEN0 = NULL;
static REG_Ctrl_TSIO_PHY* _TSIOCtrlPHY = NULL;
static REG_Ctrl_TSP8* _TSPCtrl8 = NULL;
static REG_Ctrl_STRLD* _TSIO_STRLD = NULL;

static MS_VIRT   _u32TSIORegBase = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Message
//-------------------------------------------------------------------------------------------------
#define HAL_TSIO_DBGMSG(_level,_f) do {if(_u32TSIODbgLevel >= (_level)) (_f);} while(0)
static MS_U32  _u32TSIODbgLevel = E_HAL_TSIO_DBG_LEVEL_WARN;


#define _HAL_REG32_W(reg, value)    do { (reg)->L = ((value) & 0x0000FFFF);             \
                                         (reg)->H = ((value) >> 16); } while(0)

#define _HAL_REG16_W(reg, value)    { (reg)->data = (MS_U16)((value) & 0xFFFF); }

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))


#define _REG16_SET(reg, value);    _HAL_REG16_W(reg, SET_FLAG1(_HAL_REG16_R(reg), value));
#define _REG32_SET(reg, value);    _HAL_REG32_W(reg, SET_FLAG1(_HAL_REG32_R(reg), value));
#define _REG16_CLR(reg, value);    _HAL_REG16_W(reg, RESET_FLAG1(_HAL_REG16_R(reg), value));
#define _REG32_CLR(reg, value);    _HAL_REG32_W(reg, RESET_FLAG1(_HAL_REG32_R(reg), value));
#define _REG16_SET_MASK(reg, value, mask, shift); _HAL_REG16_W(reg, (_HAL_REG16_R(reg)& ~mask) | ((value << shift) & mask));
#define _REG32_SET_MASK(reg, value, mask, shift); _HAL_REG32_W(reg, (_HAL_REG32_R(reg)& ~mask) | ((value << shift) & mask));
#define _TSIO_SGDMAIN_PIDFLT_ADDR(FltId)  (REG_SGDMAIN_PIDFLT_BASE + ((FltId) << 2))

//--------------------------------------------------------------------------------------------------
//  Analog phase table
//--------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8Phase_Digital;
    MS_U8 u8Delay1T;
    MS_U8 u8Corse;
    MS_U8 u8Fine;
}ANALOG_PHASE_SETTING;

static MS_U8 u8Phasetable = 0;

#if 1 //new
ANALOG_PHASE_SETTING SETTING[ANALOG_PHASE_TABLE][ANALOG_PHASE_NUM] =
{
    //200M
    {
    { 0, 0, 0, 0},
    { 1, 0, 0, 4},
    { 2, 0, 0, 4},
    { 3, 0, 0, 4},
    { 4, 0, 1, 0},
    { 5, 0, 1, 0},
    { 6, 0, 1, 4},
    { 7, 0, 1, 4},
    { 8, 0, 1, 4},
    { 9, 0, 2, 0},
    {10, 0, 2, 0},
    {11, 0, 2, 4},
    {12, 0, 2, 4},
    {13, 0, 2, 4},
    {14, 0, 3, 0},
    {15, 0, 3, 0},
    {16, 0, 3, 4},
    {17, 0, 3, 4},
    {18, 0, 3, 4},
    {19, 1, 0, 0},
    {20, 1, 0, 0},
    {21, 1, 0, 4},
    {22, 1, 0, 4},
    {23, 1, 0, 4},
    {24, 1, 1, 0},
    {25, 1, 1, 0},
    {26, 1, 1, 4},
    {27, 1, 1, 4},
    {28, 1, 1, 4},
    {29, 1, 2, 0},
    {30, 1, 2, 0},
    {31, 1, 2, 4},
    {32, 1, 2, 4},
    {33, 1, 2, 4},
    {34, 1, 3, 0},
    {35, 1, 3, 0},
    {36, 1, 3, 4},
    {37, 1, 3, 4},
    {38, 1, 3, 4},
    {39, 0, 0, 0},
    },

    //100M
    {
    { 0, 0, 0, 0},
    { 1, 0, 0, 4},
    { 2, 0, 0, 4},
    { 3, 0, 1, 4},
    { 4, 0, 2, 0},
    { 5, 0, 2, 0},
    { 6, 0, 2, 4},
    { 7, 0, 2, 4},
    { 8, 0, 3, 4},
    { 9, 0, 4, 0},
    {10, 0, 4, 0},
    {11, 0, 4, 4},
    {12, 0, 4, 4},
    {13, 0, 5, 4},
    {14, 0, 6, 0},
    {15, 0, 6, 0},
    {16, 0, 6, 4},
    {17, 0, 6, 4},
    {18, 0, 7, 4},
    {19, 1, 0, 0},
    {20, 1, 0, 0},
    {21, 1, 0, 4},
    {22, 1, 0, 4},
    {23, 1, 1, 4},
    {24, 1, 2, 0},
    {25, 1, 2, 0},
    {26, 1, 2, 4},
    {27, 1, 2, 4},
    {28, 1, 3, 4},
    {29, 1, 4, 0},
    {30, 1, 4, 0},
    {31, 1, 4, 4},
    {32, 1, 4, 4},
    {33, 1, 5, 4},
    {34, 1, 6, 0},
    {35, 1, 6, 0},
    {36, 1, 6, 4},
    {37, 1, 6, 4},
    {38, 1, 7, 4},
    {39, 0, 0, 0},
    },
    //50M
    {
    { 0, 0, 0, 0},
    { 1, 0, 1, 4},
    { 2, 0, 2, 4},
    { 3, 0, 3, 4},
    { 4, 0, 4, 0},
    { 5, 0, 4, 0},
    { 6, 0, 5, 4},
    { 7, 0, 6, 4},
    { 8, 0, 7, 4},
    { 9, 0, 8, 0},
    {10, 0, 8, 0},
    {11, 0, 9, 4},
    {12, 0, 10, 4},
    {13, 0, 11, 4},
    {14, 0, 12, 0},
    {15, 0, 12, 0},
    {16, 0, 13, 4},
    {17, 0, 14, 4},
    {18, 0, 15, 4},
    {19, 1, 0, 0},
    {20, 1, 0, 0},
    {21, 1, 1, 4},
    {22, 1, 2, 4},
    {23, 1, 3, 4},
    {24, 1, 4, 0},
    {25, 1, 4, 0},
    {26, 1, 5, 4},
    {27, 1, 6, 4},
    {28, 1, 7, 4},
    {29, 1, 8, 0},
    {30, 1, 8, 0},
    {31, 1, 9, 4},
    {32, 1, 10, 4},
    {33, 1, 11, 4},
    {34, 1, 12, 0},
    {35, 1, 12, 0},
    {36, 1, 13, 4},
    {37, 1, 14, 4},
    {38, 1, 15, 4},
    {39, 0, 0, 0},
    },
    //25M
    {
    { 0, 0, 0, 0},
    { 1, 0, 1, 4},
    { 2, 0, 3, 4},
    { 3, 0, 5, 4},
    { 4, 0, 8, 0},
    { 5, 0, 8, 0},
    { 6, 0, 9, 4},
    { 7, 0, 11, 4},
    { 8, 0, 13, 4},
    { 9, 0, 16, 0},
    {10, 0, 16, 0},
    {11, 0, 17, 4},
    {12, 0, 19, 4},
    {13, 0, 21, 4},
    {14, 0, 24, 0},
    {15, 0, 24, 0},
    {16, 0, 25, 4},
    {17, 0, 27, 4},
    {18, 0, 29, 4},
    {19, 1, 0, 0},
    {20, 1, 0, 0},
    {21, 1, 1, 4},
    {22, 1, 3, 4},
    {23, 1, 5, 4},
    {24, 1, 8, 0},
    {25, 1, 8, 0},
    {26, 1, 9, 4},
    {27, 1, 11, 4},
    {28, 1, 13, 4},
    {29, 1, 16, 0},
    {30, 1, 16, 0},
    {31, 1, 17, 4},
    {32, 1, 19, 4},
    {33, 1, 21, 4},
    {34, 1, 24, 0},
    {35, 1, 24, 0},
    {36, 1, 25, 4},
    {37, 1, 27, 4},
    {38, 1, 29, 4},
    {39, 0, 0, 0},
    },
};
#else
//old phase table
ANALOG_PHASE_SETTING SETTING[ANALOG_PHASE_TABLE][ANALOG_PHASE_NUM] =
{
    //200M
    {
    { 0, 0, 0, 0},
    { 1, 0, 0, 0},
    { 2, 0, 0, 5},
    { 3, 0, 0, 5},
    { 4, 0, 0, 7},
    { 5, 0, 1, 0},
    { 6, 0, 1, 0},
    { 7, 0, 1, 5},
    { 8, 0, 1, 5},
    { 9, 0, 1, 7},
    {10, 0, 2, 0},
    {11, 0, 2, 0},
    {12, 0, 2, 5},
    {13, 0, 2, 5},
    {14, 0, 2, 7},
    {15, 0, 3, 0},
    {16, 0, 3, 0},
    {17, 0, 3, 5},
    {18, 0, 3, 5},
    {19, 0, 3, 7},
    {20, 1, 0, 0},
    {21, 1, 0, 0},
    {22, 1, 0, 5},
    {23, 1, 0, 5},
    {24, 1, 0, 7},
    {25, 1, 1, 0},
    {26, 1, 1, 0},
    {27, 1, 1, 5},
    {28, 1, 1, 5},
    {29, 1, 1, 7},
    {30, 1, 2, 0},
    {31, 1, 2, 0},
    {32, 1, 2, 5},
    {33, 1, 2, 5},
    {34, 1, 2, 7},
    {35, 1, 3, 0},
    {36, 1, 3, 0},
    {37, 1, 3, 5},
    {38, 1, 3, 5},
    {39, 1, 3, 7},
    },

    //100M
    {
    { 0, 0, 0, 0},
    { 1, 0, 0, 5},
    { 2, 0, 0, 7},
    { 3, 0, 1, 0},
    { 4, 0, 1, 5},
    { 5, 0, 2, 0},
    { 6, 0, 2, 5},
    { 7, 0, 2, 7},
    { 8, 0, 3, 0},
    { 9, 0, 3, 5},
    {10, 0, 4, 0},
    {11, 0, 4, 5},
    {12, 0, 4, 7},
    {13, 0, 5, 0},
    {14, 0, 5, 5},
    {15, 0, 6, 0},
    {16, 0, 6, 5},
    {17, 0, 6, 7},
    {18, 0, 7, 0},
    {19, 0, 7, 5},
    {20, 1, 0, 0},
    {21, 1, 0, 5},
    {22, 1, 0, 7},
    {23, 1, 1, 0},
    {24, 1, 1, 5},
    {25, 1, 2, 0},
    {26, 1, 2, 5},
    {27, 1, 2, 7},
    {28, 1, 3, 0},
    {29, 1, 3, 5},
    {30, 1, 4, 0},
    {31, 1, 4, 5},
    {32, 1, 4, 7},
    {33, 1, 5, 0},
    {34, 1, 5, 5},
    {35, 1, 6, 0},
    {36, 1, 6, 5},
    {37, 1, 6, 7},
    {38, 1, 7, 0},
    {39, 1, 7, 5},
    },
    //50M
    {
    { 0, 0, 0, 0},
    { 1, 0, 0, 7},
    { 2, 0, 1, 5},
    { 3, 0, 2, 5},
    { 4, 0, 3, 5},
    { 5, 0, 4, 0},
    { 6, 0, 4, 7},
    { 7, 0, 5, 5},
    { 8, 0, 6, 5},
    { 9, 0, 7, 5},
    {10, 0, 8, 0},
    {11, 0, 8, 7},
    {12, 0, 9, 5},
    {13, 0, 10, 5},
    {14, 0, 11, 5},
    {15, 0, 12, 0},
    {16, 0, 12, 7},
    {17, 0, 13, 5},
    {18, 0, 14, 5},
    {19, 0, 15, 5},
    {20, 1, 0, 0},
    {21, 1, 0, 7},
    {22, 1, 1, 5},
    {23, 1, 2, 5},
    {24, 1, 3, 5},
    {25, 1, 4, 0},
    {26, 1, 4, 7},
    {27, 1, 5, 5},
    {28, 1, 6, 5},
    {29, 1, 7, 5},
    {30, 1, 8, 0},
    {31, 1, 8, 7},
    {32, 1, 9, 5},
    {33, 1, 10, 5},
    {34, 1, 11, 5},
    {35, 1, 12, 0},
    {36, 1, 12, 7},
    {37, 1, 13, 5},
    {38, 1, 14, 5},
    {39, 1, 15, 5},
    },
    //25M
    {
    { 0, 0, 0, 0},
    { 1, 0, 1, 7},
    { 2, 0, 3, 5},
    { 3, 0, 5, 0},
    { 4, 0, 6, 5},
    { 5, 0, 8, 0},
    { 6, 0, 9, 7},
    { 7, 0, 11, 5},
    { 8, 0, 13, 0},
    { 9, 0, 14, 5},
    {10, 0, 16, 0},
    {11, 0, 17, 7},
    {12, 0, 19, 5},
    {13, 0, 21, 0},
    {14, 0, 22, 5},
    {15, 0, 24, 0},
    {16, 0, 25, 7},
    {17, 0, 27, 5},
    {18, 0, 29, 0},
    {19, 0, 30, 5},
    {20, 1, 0, 0},
    {21, 1, 1, 7},
    {22, 1, 3, 5},
    {23, 1, 5, 0},
    {24, 1, 6, 5},
    {25, 1, 8, 0},
    {26, 1, 9, 7},
    {27, 1, 11, 5},
    {28, 1, 13, 0},
    {29, 1, 14, 5},
    {30, 1, 16, 0},
    {31, 1, 17, 7},
    {32, 1, 19, 5},
    {33, 1, 21, 0},
    {34, 1, 22, 5},
    {35, 1, 24, 0},
    {36, 1, 25, 7},
    {37, 1, 27, 5},
    {38, 1, 29, 0},
    {39, 1, 30, 5},
    },
};
#endif
//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
static MS_U32 _HAL_REG32_R(REG32_TSIO *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

static MS_U16 _HAL_REG16_R(REG16_TSIO *reg)
{
    MS_U16              value = 0;
    value = (reg)->data;
    return value;
}

//=======================================================
// General API
//=======================================================
void HAL_TSIO_SetBank(MS_VIRT u32BankAddr)
{
    _u32TSIORegBase = u32BankAddr;
    _TSIOCtrl0 = (REG_Ctrl_TSIO0*)(_u32TSIORegBase+ REG_CTRL_BASE_TSIO0);   // 0x171A
    _TSIOCtrl1 = (REG_Ctrl_TSIO1*)(_u32TSIORegBase+ REG_CTRL_BASE_TSIO1);   // 0x171B
    _TSIOCtrl2 = (REG_Ctrl_TSIO2*)(_u32TSIORegBase+ REG_CTRL_BASE_TSIO2);   // 0x171C
    _TSIOCtrl3 = (REG_Ctrl_TSIO3*)(_u32TSIORegBase+ REG_CTRL_BASE_TSIO3);   // 0x1739
    _TSIOCtrlLOCDEC = (REG_Ctrl_TSIO_LOCDEC*)(_u32TSIORegBase+ REG_CTRL_BASE_TSIO_LOCDEC);  // 0x171E
    _TSOCtrl0 = (REG_Ctrl_TSO0*)(_u32TSIORegBase+ REG_CTRL_BASE_TSO0);   //0x1706
    _TSOCtrl2 = (REG_Ctrl_TSO2*)(_u32TSIORegBase+ REG_CTRL_BASE_TSO2);   // 0x1539
    _TSOCtrl3 = (REG_Ctrl_TSO3*)(_u32TSIORegBase+ REG_CTRL_BASE_TSO3);   // 0x171D
    _TSIOCHIPTOP = (REG_Ctrl_CHIPTOP*)(_u32TSIORegBase+ REG_CTRL_BASE_CHIPTOP);   // 0x101E
    _TSIO_CLKGEN0 = (REG_Ctrl_CLOCKGEN0*)(_u32TSIORegBase+ REG_CTRL_BASE_CLOCKGEN0);   // 0x100B
    _TSIOCtrlPHY = (REG_Ctrl_TSIO_PHY*)(_u32TSIORegBase+ REG_CTRL_BASE_TSIO_PHY);   // 0x171F
    _TSPCtrl8 = (REG_Ctrl_TSP8 *)(_u32TSIORegBase+ 0xC4E00UL);     // TSP8  0x1627
    _TSIO_STRLD = (REG_Ctrl_STRLD*)(_u32TSIORegBase+ REG_CTRL_BASE_STRLD); //STRLD 0x1029
}

void HAL_TSIO_DbgLevel(EN_HAL_TSIO_DBGMSG_LEVEL eLevel)
{
    _u32TSIODbgLevel = eLevel;
}

void HAL_TSIO_ClkOpen(MS_BOOL bEnable)
{
    if (bEnable)
    {
        //tsp clk
        _REG16_CLR(&(_TSIO_CLKGEN0->REG_CLKGEN0_TSP), REG_CLKGEN0_TSP_DISABLE_CLOCK);
        _REG16_SET_MASK(&(_TSIO_CLKGEN0->REG_CLKGEN0_TSP), 0, REG_CLKGEN0_TSP_CLKSOURCE_MASK, REG_CLKGEN0_TSP_CLKSOURCE_SHIFT);

        //TSP boot clk set
        _REG16_CLR(&(_TSIOCHIPTOP->REG_TOP_TSIO), REG_TOP_TSIO_TSP_BOOT_CLK_SEL);

        //ckg tsp tsio
        _REG16_CLR(&(_TSIOCtrl0->CKG_TSP_TSIO), TSIO0_CKG_TSP_TSIO);

        //tsio clk
        _REG16_CLR(&(_TSIO_CLKGEN0->REG_CLKGEN0_TSIO), REG_CLKGEN0_TSIO_DISABLE_CLOCK);
        _REG16_SET_MASK(&(_TSIO_CLKGEN0->REG_CLKGEN0_TSIO), 0, REG_CLKGEN0_TSIO_CLKSOURCE_MASK, REG_CLKGEN0_TSIO_CLKSOURCE_SHIFT);
    }
    else
    {
        //tsio clk
        _REG16_SET(&(_TSIO_CLKGEN0->REG_CLKGEN0_TSIO), REG_CLKGEN0_TSIO_DISABLE_CLOCK);
    }
}
void HAL_TSIO_PowerInit(void)
{
    //_HAL_REG16_W(&(_TSIOCtrl0->AD_TX_CONFIG0),0x0000); // fpga default value
    //_HAL_REG16_W(&(_TSIOCtrl0->AD_RX_CONFIG0),0x0000); // fpga default value
    //_REG16_SET(&(_TSIOCtrl0->AD_TX_CONFIG0),TSIO0_PD_SMC_LDO_FPGA); // fpga setting
    //_HAL_REG16_W(&(_TSIOCtrl0->MAX_PERIOD_OF_PKT_SYNC),0x0000); // for test
}

void HAL_TSIO_Reset(void)
{
    _REG16_CLR(&(_TSIOCtrl0->SW_RSTZ), TSIO0_SW_RSTZ);//low active
    _REG16_SET(&(_TSIOCtrl0->SW_RSTZ), TSIO0_SW_RSTZ);
}

void HAL_TSIO_Tx2Rx_InsertCTS(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_INSERT_CTS_IN_TX);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_INSERT_CTS_IN_TX);
    }
}

void HAL_TSIO_Tx2Rx_Direct8(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_TX_DIRECT_RX_8);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_TX_DIRECT_RX_8);
    }
}

void HAL_TSIO_Tx2Rx_Direct16(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl1->RX_CONFIG0), TSIO1_TX_DIRECT_RX_16);
        _REG16_SET(&(_TSIOCtrl0->PUH_CONFIG1), TSIO0_PUH_CONFIG1_VCC_POWER_GOOD_SET_HIGHT);
        _REG16_SET(&(_TSIOCtrl0->PUH_CONFIG1), TSIO0_PUH_CONFIG1_TSIO_RX_DATA_VALID_SET_HIGH);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl1->RX_CONFIG0), TSIO1_TX_DIRECT_RX_16);
        _REG16_CLR(&(_TSIOCtrl0->PUH_CONFIG1), TSIO0_PUH_CONFIG1_VCC_POWER_GOOD_SET_HIGHT);
        _REG16_CLR(&(_TSIOCtrl0->PUH_CONFIG1), TSIO0_PUH_CONFIG1_TSIO_RX_DATA_VALID_SET_HIGH);
    }
}

void HAL_TSIO_Tx2Rx_Loopback(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_18), 0x20,TSIO_PHY_18_TEST_TSIO_MASK, TSIO_PHY_18_TEST_TSIO_SHIFT); // loopback

    }
    else
    {
        _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_18), 0,TSIO_PHY_18_TEST_TSIO_MASK, TSIO_PHY_18_TEST_TSIO_SHIFT);
    }
}

void HAL_TSIO_StuffingSID_Set(MS_U8 u8Stuffing_sid)
{
    _REG16_SET_MASK(&(_TSIOCtrl0->TX_CONFIG0), u8Stuffing_sid, TSIO0_STUFF_SVID_MASK, TSIO0_STUFF_SVID_SHIFT);
}

void HAL_TSIO_Decrypt_Set(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_CLR(&(_TSIOCtrl1->RX_CONFIG0), TSIO1_DECRYPT_DISABLE);
    }
    else
    {
        _REG16_SET(&(_TSIOCtrl1->RX_CONFIG0), TSIO1_DECRYPT_DISABLE);
    }

    //_REG16_SET(&(_TSIOCtrl1->RX_CONFIG0), TSIO1_BIT_TRAINING_BYPASS);
    _REG16_SET(&(_TSIOCtrl1->PKT_SYNC_CTRL),TSIO1_PKT_SYNC_EN);
}

void HAL_TSIO_Threshold_Set(MS_U8 u8Threshold)
{
    _REG16_SET_MASK(&(_TSIOCtrl1->TSIO_SYNC_THRESHOLD), (u8Threshold-1), TSIO1_TSIO_N_SYNC_MASK, TSIO1_TSIO_N_SYNC_SHIFT);
    _REG16_SET_MASK(&(_TSIOCtrl1->TSIO_SYNC_THRESHOLD), (u8Threshold-1), TSIO1_TSIO_SYNC_CNT_MASK, TSIO1_TSIO_SYNC_CNT_SHIFT);
}

void HAL_TSIO_BypassMerge_Set(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _REG16_CLR(&(_TSIOCtrl3->TSIO_PKT_MERGE_CONFIG), TSIO3_NO_NEED_MERGE_BYPASS_SM);
        _REG16_CLR(&(_TSIOCtrl3->TSIO_BYPASS_MERGE_DISABLE), TSIO3_BYPASS_MERGE_DIABLE);
    }
    else
    {
        _REG16_SET(&(_TSIOCtrl3->TSIO_PKT_MERGE_CONFIG), TSIO3_NO_NEED_MERGE_BYPASS_SM);
        _REG16_SET(&(_TSIOCtrl3->TSIO_BYPASS_MERGE_DISABLE), TSIO3_BYPASS_MERGE_DIABLE);
    }
}

void HAL_TSIO_SIDBypassSMCMergeEnable(MS_U8 u8Sid, MS_BOOL bEnable)
{
    MS_U16 u16SvidBypassSMCEnableBit[4];

    u16SvidBypassSMCEnableBit[0] = _HAL_REG16_R(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[0]));
    u16SvidBypassSMCEnableBit[1] = _HAL_REG16_R(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[1]));
    u16SvidBypassSMCEnableBit[2] = _HAL_REG16_R(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[2]));
    u16SvidBypassSMCEnableBit[3] = _HAL_REG16_R(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[3]));

    MS_U64 u64Mask = 0;
    MS_U64 u64Data = u16SvidBypassSMCEnableBit[0] + ((MS_U64)u16SvidBypassSMCEnableBit[1] << 16)
        + ((MS_U64)u16SvidBypassSMCEnableBit[2] << 32) + ((MS_U64)u16SvidBypassSMCEnableBit[3] << 48);

    if (bEnable)
    {
        u64Mask = (MS_U64)(0x1) << u8Sid;
        u64Data |= u64Mask;
    }
    else
    {
        u64Mask = ~((MS_U64)(0x1) << u8Sid);
        u64Data &= u64Mask;
    }

    u16SvidBypassSMCEnableBit[0] = (u64Data & 0x000000000000FFFF);
    u16SvidBypassSMCEnableBit[1] = (u64Data & 0x00000000FFFF0000)>>16;
    u16SvidBypassSMCEnableBit[2] = (u64Data & 0x0000FFFF00000000)>>32;
    u16SvidBypassSMCEnableBit[3] = (u64Data & 0xFFFF000000000000)>>48;

    _HAL_REG16_W(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[0]), u16SvidBypassSMCEnableBit[0]);
    _HAL_REG16_W(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[1]), u16SvidBypassSMCEnableBit[1]);
    _HAL_REG16_W(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[2]), u16SvidBypassSMCEnableBit[2]);
    _HAL_REG16_W(&(_TSIOCtrl3->TSIO_SVID_BYPASS_SMC_EN[3]), u16SvidBypassSMCEnableBit[3]);
}

void HAL_TSIO_CTS_En(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _REG16_CLR(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_CTS_DISABLE);
    }
    else
    {
        _REG16_SET(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_CTS_DISABLE);
    }
}

//=======================================================
// SGDMA_IN API
//=======================================================
void HAL_TSIO_SGDMAIN_Reset(void)
{
    _REG16_CLR(&(_TSOCtrl3->SW_RSTZ),TSO3_SW_RSTZ); //low active
    _REG16_SET(&(_TSOCtrl3->SW_RSTZ),TSO3_SW_RSTZ);
}

void HAL_TSIO_SGDMAIN_LastPktBuf_Set(MS_U32 u32Head, MS_U32 u32Tail)
{
    _HAL_REG32_W(&(_TSOCtrl3->LAST_PKT_STR2MI_HEAD), (u32Head >> TSIO_MIU_BUS)&0xFFFFFFFF);
    _HAL_REG32_W(&(_TSOCtrl3->LAST_PKT_STR2MI_TAIL), (u32Tail >> TSIO_MIU_BUS)&0xFFFFFFFF);

    _REG16_SET(&(_TSOCtrl3->LAST_PKT),TSO3_LAST_PKT_STR2MI_RST_WADR);
    _REG16_CLR(&(_TSOCtrl3->LAST_PKT),TSO3_LAST_PKT_STR2MI_RST_WADR);

    _REG16_SET(&(_TSOCtrl3->LAST_PKT),TSO3_LAST_PKT_STR2MI_EN);
}

void HAL_TSIO_SGDMAIN_SGVQBuf_Set(MS_U32 u32BufAddr, MS_U32 u32BufSize)
{
    _HAL_REG32_W(&(_TSOCtrl3->SGVQ1_BASE), (u32BufAddr >> TSIO_MIU_BUS)&0xFFFFFFFF);
    _HAL_REG16_W(&(_TSOCtrl3->SGVQ1_SIZE_200BYTE), (u32BufSize / TSIO_SVQ_UNIT_SIZE));

    _REG16_SET(&(_TSOCtrl3->SGVQ1_TX_CONFIG), TSO3_TX_CONFIG_TX_RESET);
    _REG16_CLR(&(_TSOCtrl3->SGVQ1_TX_CONFIG), TSO3_TX_CONFIG_TX_RESET);
    _REG16_SET(&(_TSOCtrl3->SGVQ1_TX_CONFIG), TSO3_TX_CONFIG_SVQ_TX_ENABLE);
}

void HAL_TSIO_SGDMAIN_VC_Open(MS_U32 u32Addr, MS_U8 u8Chid, MS_U8 u8Sid, MS_BOOL bTSmode,
    MS_BOOL bPriority, MS_BOOL bFlush, MS_BOOL bNewPkt)
{
    //Set CMD
    _REG16_SET_MASK(&(_TSOCtrl3->ACPU_CMD), u8Chid, TSO3_ACPU_CMD_ACPU_VC_ID_MASK, TSO3_ACPU_CMD_ACPU_VC_ID_SHIFT);
    if (bNewPkt)
    {
        _REG16_SET(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_START_A_NEW_PACKAGE);
    }
    else
    {
        _REG16_CLR(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_START_A_NEW_PACKAGE);
    }
    _REG16_SET(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_ACPU_RW);
    _REG16_SET(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_ACPU_ACTIVE);
    if (bPriority)
    {
        _REG16_SET(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_M_PRIORITY);
    }
    else
    {
        _REG16_CLR(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_M_PRIORITY);
    }

    //Set FLAG
    if (bTSmode)
    {
        _REG16_SET(&(_TSOCtrl3->ACPU_FLAG), TSO3_ACPU_FLAG_PKT_MODE_188);
    }
    else
    {
        _REG16_CLR(&(_TSOCtrl3->ACPU_FLAG), TSO3_ACPU_FLAG_PKT_MODE_188);
    }
    if (bFlush)
    {
        _REG16_SET(&(_TSOCtrl3->ACPU_FLAG), TSO3_ACPU_FLAG_LAST_NODE_FLUSH);
    }
    else
    {
        _REG16_CLR(&(_TSOCtrl3->ACPU_FLAG), TSO3_ACPU_FLAG_LAST_NODE_FLUSH);
    }
    _REG16_SET(&(_TSOCtrl3->ACPU_FLAG), TSO3_ACPU_FLAG_INTERRUPT_ENABLE);
    _REG16_SET_MASK(&(_TSOCtrl3->ACPU_FLAG), u8Sid, TSO3_ACPU_FLAG_SERVICE_ID_MASK, TSO3_ACPU_FLAG_SERVICE_ID_SHIFT);

    //Write Addr
    _HAL_REG32_W(&(_TSOCtrl3->ACPU_WDATA), u32Addr);

    //Trigger
    _REG16_SET(&(_TSOCtrl3->ACPU_ACTIVE), TSO3_ACPU_ACTIVE);
}

void HAL_TSIO_SGDMAIN_VC_Close(MS_U8 u8Chid)
{
    //Set CMD
    _REG16_SET_MASK(&(_TSOCtrl3->ACPU_CMD), u8Chid, TSO3_ACPU_CMD_ACPU_VC_ID_MASK, TSO3_ACPU_CMD_ACPU_VC_ID_SHIFT);
    _REG16_SET(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_ACPU_RW);
    _REG16_CLR(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_ACPU_ACTIVE);

    //Trigger
    _REG16_SET(&(_TSOCtrl3->ACPU_ACTIVE), TSO3_ACPU_ACTIVE);
}

void HAL_TSIO_SGDMAIN_Global_Pacing(void)
{
    //golobal tick unit, 192 = 1us
    _REG16_SET_MASK(&(_TSOCtrl3->GLOBAL_TICK_COUNT_SET), 192, TSO3_GLOBAL_TICK_COUNT_SET_MASK, TSO3_GLOBAL_TICK_COUNT_SET_SHIFT);
}

void HAL_TSIO_SGDMAIN_VC_Pacing(MS_U8 u8Chid, MS_BOOL bTSmode, MS_U32 Pacing)
{
    if (Pacing == 0 || u8Chid >= TSIO_SERVICE_NUM)
    {
        return;
    }

    MS_U16 u16Wdata = 0;
    if (bTSmode)
    {
        u16Wdata = (1000000/(Pacing /188 /8));
    }
    else
    {
        u16Wdata = (1000000/(Pacing /192 /8));
    }

    _REG16_SET_MASK(&(_TSOCtrl3->TICK_COUNT_SET[u8Chid]), u16Wdata, TSO3_TICK_COUNT_SET_MASK, TSO3_TICK_COUNT_SET_SHIFT);
}

void HAL_TSIO_SGDMAIN_Start(void)
{
    _REG16_SET(&(_TSOCtrl3->SGCTRL_START), TSO3_SGCTRL_START);
}

void HAL_TSIO_SGDMAIN_IntRead(MS_U8 u8Chid, MS_U16 *u16Status)
{
    MS_BOOL bINT = false;
    MS_U64 VC_INT = (MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[0]))+
        ((MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[1])) << 16)+
        ((MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[2])) << 32)+
        ((MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[3])) << 48);

    bINT = (VC_INT & ((MS_U64)(0x1) << u8Chid)) >> u8Chid;
    *u16Status = bINT;
}

void HAL_TSIO_SGDMAIN_IntClr(MS_U8 u8Chid)
{
    _REG16_SET_MASK(&(_TSOCtrl3->PACE_DBG), u8Chid, TSO3_SGDMA_IN_VC_INT_VC_ID_MASK, TSO3_SGDMA_IN_VC_INT_VC_ID_SHIFT);
    _REG16_SET(&(_TSOCtrl3->PACE_DBG), TSO3_SGDMA_IN_VC_INT_CLR);
    _REG16_SET(&(_TSOCtrl3->SGDMA_IN), TSO3_SGDMA_IN_VC_INT_TRIGGER);
}

MS_U64 HAL_TSIO_SGDMAIN_IntAll(void)
{
    MS_U64 VC_INT = (MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[0]))+
        ((MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[1])) << 16)+
        ((MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[2])) << 32)+
        ((MS_U64)_HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_VC_INT[3])) << 48);

    return VC_INT;
}

void HAL_TSIO_SGDMAIN_IntClrAll(void)
{
    _REG16_SET(&(_TSOCtrl3->SGDMA_IN), TSO3_SGDMA_IN_INT_CLR);
    _REG16_CLR(&(_TSOCtrl3->SGDMA_IN), TSO3_SGDMA_IN_INT_CLR);
}

MS_U32 HAL_TSIO_SGDMAIN_ReadSram(MS_U8 u8Chid, HAL_SGDMA_SRAM eSram)
{
    MS_U32 u32Data = 0;
    if (eSram >= HAL_SGDMA_SRAM_MAX)
    {
        return 0;
    }

    //Set cmd
    _REG16_SET_MASK(&(_TSOCtrl3->ACPU_CMD), u8Chid, TSO3_ACPU_CMD_ACPU_VC_ID_MASK, TSO3_ACPU_CMD_ACPU_VC_ID_SHIFT);
    _REG16_SET_MASK(&(_TSOCtrl3->ACPU_CMD), eSram, TSO3_ACPU_CMD_ACPU_POSITION_MASK, TSO3_ACPU_CMD_ACPU_POSITION_SHIFT);
    _REG16_CLR(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_ACPU_RW);
    _REG16_CLR(&(_TSOCtrl3->ACPU_CMD), TSO3_ACPU_CMD_ACPU_ACTIVE);

     //Trigger
    _REG16_SET(&(_TSOCtrl3->ACPU_ACTIVE), TSO3_ACPU_ACTIVE);

    u32Data = _HAL_REG32_R(&(_TSOCtrl3->ACPU_RDATA));
    return u32Data;
}

MS_U16 HAL_TSIO_SGDMAIN_ReadDbg(HAL_SGDMAIN_DBG eDbg)
{
    MS_U16 u16Data = 0;

    if (eDbg >= HAL_SGDMAIN_DBG_MAX)
    {
        return 0;
    }

    _REG16_SET_MASK(&(_TSOCtrl3->SGDMA_IN), eDbg, TSO3_SGDMA_IN_DBG_SEL_MASK, TSO3_SGDMA_IN_DBG_SEL_SHIFT);
    u16Data = _HAL_REG16_R(&(_TSOCtrl3->SGDMA_IN_DBG));
    return u16Data;
}

void HAL_TSIO_SGDMAIN_PidFltEnable(MS_U8 u8Sid, MS_BOOL bEnable)
{
    MS_U16 u16PidFltEnableBit[4];

    u16PidFltEnableBit[0] = _HAL_REG16_R(&(_TSOCtrl2->SG_PDFLT_SVID_EN[0]));
    u16PidFltEnableBit[1] = _HAL_REG16_R(&(_TSOCtrl2->SG_PDFLT_SVID_EN[1]));
    u16PidFltEnableBit[2] = _HAL_REG16_R(&(_TSOCtrl2->SG_PDFLT_SVID_EN[2]));
    u16PidFltEnableBit[3] = _HAL_REG16_R(&(_TSOCtrl2->SG_PDFLT_SVID_EN[3]));

    MS_U64 u64Mask = 0;
    MS_U64 u64Data = u16PidFltEnableBit[0] + ((MS_U64)u16PidFltEnableBit[1] << 16)
        + ((MS_U64)u16PidFltEnableBit[2] << 32) + ((MS_U64)u16PidFltEnableBit[3] << 48);

    if (bEnable)
    {
        u64Mask = (MS_U64)(0x1) << u8Sid;
        u64Data |= u64Mask;
    }
    else
    {
        u64Mask = ~((MS_U64)(0x1) << u8Sid);
        u64Data &= u64Mask;
    }

    u16PidFltEnableBit[0] = (u64Data & 0x000000000000FFFF);
    u16PidFltEnableBit[1] = (u64Data & 0x00000000FFFF0000)>>16;
    u16PidFltEnableBit[2] = (u64Data & 0x0000FFFF00000000)>>32;
    u16PidFltEnableBit[3] = (u64Data & 0xFFFF000000000000)>>48;

    _HAL_REG16_W(&(_TSOCtrl2->SG_PDFLT_SVID_EN[0]), u16PidFltEnableBit[0]);
    _HAL_REG16_W(&(_TSOCtrl2->SG_PDFLT_SVID_EN[1]), u16PidFltEnableBit[1]);
    _HAL_REG16_W(&(_TSOCtrl2->SG_PDFLT_SVID_EN[2]), u16PidFltEnableBit[2]);
    _HAL_REG16_W(&(_TSOCtrl2->SG_PDFLT_SVID_EN[3]), u16PidFltEnableBit[3]);
}

static void _HAL_TSIO_SGDMAIN_PageTableIndW(MS_U32 u32Addr, MS_U32 u32Wdata)
{
    // addr
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_ADDR_L), (u32Addr & 0x0000FFFF));
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_ADDR_H), (u32Addr >> 16));

    // Wdata
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_WDATA_L), (u32Wdata & 0x0000FFFF));
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_WDATA_H), (u32Wdata >> 16));

    // Wen
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_EN), TSO0_PDTABLE_W_EN);
}

static MS_U16 _HAL_TSIO_SGDMAIN_PageTableIndR_H(MS_U32 u32Addr)
{
    // addr
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_ADDR_L), (u32Addr & 0x0000FFFF));
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_ADDR_H), (u32Addr >> 16));

    // Ren
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_EN), TSO0_PDTABLE_R_EN);

    // Rdata
    return _HAL_REG16_R(&(_TSOCtrl2->SG_PDTABLE_RDATA_H));
}

static MS_U16 _HAL_TSIO_SGDMAIN_PageTableIndR(MS_U32 u32Addr)
{
    // addr
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_ADDR_L), (u32Addr & 0x0000FFFF));
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_ADDR_H), (u32Addr >> 16));

    // Ren
    _HAL_REG16_W(&(_TSOCtrl0->PDTABLE_EN), TSO0_PDTABLE_R_EN);

    // Rdata
    return _HAL_REG16_R(&(_TSOCtrl2->SG_PDTABLE_RDATA));
}

void HAL_TSIO_SGDMAIN_Flt_SetPid(MS_U16 u16FltId, MS_U16 u16PID)
{
    MS_U32 u32Temp = 0;

    u32Temp = TSO0_ONE_MASK + (MS_U32)(u16PID & TSO0_PID_MASK);
    u32Temp |= (MS_U32)((_HAL_TSIO_SGDMAIN_PageTableIndR_H(_TSIO_SGDMAIN_PIDFLT_ADDR(u16FltId))&TSO2_PDTABLE_RDATA_H_MASK)<<16);
    _HAL_TSIO_SGDMAIN_PageTableIndW(_TSIO_SGDMAIN_PIDFLT_ADDR(u16FltId), u32Temp);
}

void HAL_TSIO_SGDMAIN_Flt_SetSid(MS_U16 u16FltId, MS_U8 u8Sid)
{
    MS_U32 u32Temp = 0;

    u32Temp = (((MS_U32)u8Sid<<TSO0_SVID_SHIFT) & TSO0_SVID_MASK);
    u32Temp |= (MS_U32)_HAL_TSIO_SGDMAIN_PageTableIndR(_TSIO_SGDMAIN_PIDFLT_ADDR(u16FltId));
    _HAL_TSIO_SGDMAIN_PageTableIndW(_TSIO_SGDMAIN_PIDFLT_ADDR(u16FltId), u32Temp);
}

void HAL_TSIO_SGDMAIN_Pause(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSOCtrl3->SGDMA_IN), TSO3_SGDMA_IN_PAUSE);
    }
    else
    {
        _REG16_CLR(&(_TSOCtrl3->SGDMA_IN), TSO3_SGDMA_IN_PAUSE);
    }
}

//=======================================================
// SGDMA_OUT API
//=======================================================
void HAL_TSIO_SGDMAOUT_Init(void)
{
    _REG16_CLR(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_STR2MI_EN);
    _REG16_SET(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_STR2MI_RST_WADR);
    _REG16_CLR(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_STR2MI_RST_WADR);
    _REG16_SET_MASK(&(_TSIOCtrl2->TSIO_PVR_CONFIG), 1, TSIO2_PVR_BURST_LEN_MASK, TSIO2_PVR_BURST_LEN_SHIFT); // for fixed ccp error
    _REG16_SET(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_DMA_FLUSH_EN); // for fixed ccp error
    _REG16_SET(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_STR2MI_EN);
}

void HAL_TSIO_SGDMAOUT_VC_Open(MS_U32 u32Addr, MS_U8 u8Chid, MS_BOOL bSecurity)
{
    //Set cmd
    _REG16_SET_MASK(&(_TSIOCtrl2->ACPU_CMD), u8Chid, TSIO2_ACPU_CMD_VC_ID_MASK, TSIO2_ACPU_CMD_VC_ID_SHIFT);
    _REG16_SET(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_CLR);
    _REG16_SET(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_RW);
    _REG16_SET(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_ACTIVE);

    //Set flag
    _REG16_SET(&(_TSIOCtrl2->ACPU_FLAG), TSIO2_ACPU_FLAG_INTERRUPT_ENABLE);
    _REG16_SET_MASK(&(_TSIOCtrl2->ACPU_FLAG), 0x18, TSIO2_ACPU_FLAG_MOBF_MASK, TSIO2_ACPU_FLAG_MOBF_SHIFT);
    if (bSecurity)
    {
        _REG16_SET(&(_TSIOCtrl2->ACPU_FLAG), TSIO2_ACPU_FLAG_SECURE_FLAG);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl2->ACPU_FLAG), TSIO2_ACPU_FLAG_SECURE_FLAG);
    }
    _REG16_SET(&(_TSIOCtrl2->ACPU_FLAG), TSIO2_ACPU_FLAG_DMA_END_CONTINUE_DMA);

    //Write Addr
    _HAL_REG32_W(&(_TSIOCtrl2->ACPU_WDATA), u32Addr);

    //Trigger
    _REG16_SET(&(_TSIOCtrl2->ACPU_ACTIVE), TSIO2_ACPU_ACTIVE);
}

void HAL_TSIO_SGDMAOUT_VC_Close(MS_U8 u8Chid)
{
    //Set cmd
    _REG16_SET_MASK(&(_TSIOCtrl2->ACPU_CMD), u8Chid, TSIO2_ACPU_CMD_VC_ID_MASK, TSIO2_ACPU_CMD_VC_ID_SHIFT);
    _REG16_SET(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_RW);
    _REG16_SET(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_CLR);
    _REG16_CLR(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_ACTIVE);

     //Trigger
    _REG16_SET(&(_TSIOCtrl2->ACPU_ACTIVE), TSIO2_ACPU_ACTIVE);
}

void HAL_TSIO_SGDMAOUT_Start(void)
{
    _REG16_SET(&(_TSIOCtrl2->VCFSM_START), TSIO2_VCFSM_START);
}

void HAL_TSIO_SGDMAOUT_IntRead(MS_U8 u8Chid, MS_U16 *u16Status)
{
    MS_BOOL bINT = false;
    MS_U64 VC_INT = (MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[0]))+
        ((MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[1])) << 16)+
        ((MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[2])) << 32)+
        ((MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[3])) << 48);

    bINT = (VC_INT & ((MS_U64)(0x1) << u8Chid)) >> u8Chid;
    if (bINT)
    {
        _REG16_SET_MASK(&(_TSIOCtrl2->SGDMA_OUT_VC_STATUS_SEL), u8Chid, TSIO2_SGDMA_OUT_VC_STATUS_SEL_MASK, TSIO2_SGDMA_OUT_VC_STATUS_SEL_SHIFT);
        *u16Status = _HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_STATUS));
    }
    else
    {
        *u16Status = 0;
    }
}

void HAL_TSIO_SGDMAOUT_IntRead_2(MS_U8 u8Chid, MS_U16 *u16Status)
{
    _REG16_SET_MASK(&(_TSIOCtrl2->SGDMA_OUT_VC_STATUS_SEL), u8Chid, TSIO2_SGDMA_OUT_VC_STATUS_SEL_MASK, TSIO2_SGDMA_OUT_VC_STATUS_SEL_SHIFT);
    *u16Status = _HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_STATUS));
}

void HAL_TSIO_SGDMAOUT_IntClr(MS_U8 u8Chid)
{
    _REG16_SET_MASK(&(_TSIOCtrl2->SGDMA_OUT_INFO), u8Chid, TSIO2_SGDMA_OUT_VC_INT_VC_ID_MASK, TSIO2_SGDMA_OUT_VC_INT_VC_ID_SHIFT);
    _REG16_SET(&(_TSIOCtrl2->SGDMA_OUT_INFO), TSIO2_SGDMA_OUT_VC_INT_CLR);
    _REG16_SET(&(_TSIOCtrl2->SGDMA_OUT_CTRL), TSIO2_SGDMA_OUT_VC_INT_TRIGGER);
}

MS_U64 HAL_TSIO_SGDMAOUT_IntAll(void)
{
    MS_U64 VC_INT = (MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[0]))+
        ((MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[1])) << 16)+
        ((MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[2])) << 32)+
        ((MS_U64)_HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_VC_INT[3])) << 48);

    return VC_INT;
}

void HAL_TSIO_SGDMAOUT_IntClrAll(void)
{
    _REG16_SET(&(_TSIOCtrl2->SGDMA_OUT_CTRL), TSIO2_SGDMA_OUT_INT_CLR);
    _REG16_CLR(&(_TSIOCtrl2->SGDMA_OUT_CTRL), TSIO2_SGDMA_OUT_INT_CLR);
}

MS_U32 HAL_TSIO_SGDMAOUT_ReadSram(MS_U8 u8Chid, HAL_SGDMA_SRAM eSram, MS_BOOL bLSB)
{
    MS_U32 u32Data = 0;
    if (eSram >= HAL_SGDMA_SRAM_MAX)
    {
        return 0;
    }

    //Set cmd
    _REG16_SET_MASK(&(_TSIOCtrl2->ACPU_CMD), u8Chid, TSIO2_ACPU_CMD_VC_ID_MASK, TSIO2_ACPU_CMD_VC_ID_SHIFT);
    _REG16_SET_MASK(&(_TSIOCtrl2->ACPU_CMD), eSram, TSIO2_ACPU_CMD_RD_POSITION_MASK, TSIO2_ACPU_CMD_RD_POSITION_SHIFT);
    _REG16_CLR(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_RW);
    _REG16_CLR(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_ACTIVE);
    if (bLSB)
    {
        _REG16_SET(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_RD_ADDR_LSB);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl2->ACPU_CMD), TSIO2_ACPU_CMD_RD_ADDR_LSB);
    }

     //Trigger
    _REG16_SET(&(_TSIOCtrl2->ACPU_ACTIVE), TSIO2_ACPU_ACTIVE);

    u32Data = _HAL_REG32_R(&(_TSIOCtrl2->ACPU_RDATA));
    return u32Data;
}

MS_U16 HAL_TSIO_SGDMAOUT_ReadDbg(HAL_SGDMAOUT_DBG eDbg)
{
    MS_U16 u16Data = 0;

    if (eDbg >= HAL_SGDMAOUT_DBG_MAX)
    {
        return 0;
    }

    _REG16_SET_MASK(&(_TSIOCtrl2->SGDMA_OUT_CTRL), eDbg, TSIO2_SGDMA_OUT_DBG_SEL_MASK, TSIO2_SGDMA_OUT_DBG_SEL_SHIFT);
    u16Data = _HAL_REG16_R(&(_TSIOCtrl2->SGDMA_OUT_DBG));
    return u16Data;
}

void HAL_TSIO_SGDMAOUT_Pause(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl2->SGDMA_OUT_CTRL), TSIO2_SGDMA_OUT_PAUSE);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl2->SGDMA_OUT_CTRL), TSIO2_SGDMA_OUT_PAUSE);
    }
}

//=======================================================
// SVID table API
//=======================================================
void HAL_TSIO_SVIDTBL_Set(MS_U8 u8Tblid, MS_U8 u8Sid, MS_U8 u8Sourceid, MS_BOOL bLive)
{
    if (u8Tblid >= TSIO_SERVICE_NUM)
    {
        return;
    }

    _REG16_SET_MASK(&(_TSIOCtrl1->SVID_SRCID[u8Tblid]), u8Sourceid, TSIO1_SOURCE_ID_MASK, TSIO1_SOURCE_ID_SHIFT);
    _REG16_SET_MASK(&(_TSIOCtrl1->SVID_SRCID[u8Tblid]), u8Sid, TSIO1_SERVICE_ID_MASK, TSIO1_SERVICE_ID_SHIFT);

    MS_U8 u8Addr = u8Tblid / 8;
    MS_U16 u16Shift = 0 ;
    switch ((u8Tblid%8))
    {
        case 0:
            u16Shift = TSIO1_LIVE_0_ENABLE;
            break;
        case 1:
            u16Shift = TSIO1_LIVE_1_ENABLE;
            break;
        case 2:
            u16Shift = TSIO1_LIVE_2_ENABLE;
            break;
        case 3:
            u16Shift = TSIO1_LIVE_3_ENABLE;
            break;
        case 4:
            u16Shift = TSIO1_LIVE_4_ENABLE;
            break;
        case 5:
            u16Shift = TSIO1_LIVE_5_ENABLE;
            break;
        case 6:
            u16Shift = TSIO1_LIVE_6_ENABLE;
            break;
        case 7:
            u16Shift = TSIO1_LIVE_7_ENABLE;
        default:
            break;
    }

    if (bLive)
    {
        _REG16_SET(&(_TSIOCtrl1->SVID_INFO[u8Addr]), u16Shift);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl1->SVID_INFO[u8Addr]), u16Shift);
    }
    _REG16_SET(&(_TSIOCtrl1->SVID_SRCID[u8Tblid]), TSIO1_ENABLE);
}

//=======================================================
// CC API
//=======================================================

void HAL_TSIO_CC_RxClr(void)
{
    _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_CLR);
    _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_CLR);
}

void HAL_TSIO_CC_RxIntClr(void)
{
    _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_RECEIVE_EN);
    _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_INT_CLR);
    _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_INT_CLR);
}

void HAL_TSIO_CC_RxEnable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_INT_EN);
        _REG16_SET_MASK(&(_TSIOCtrl0->RX_CC_CNTRL),0xFF,TSIO0_RX_ERR_INT_EN_MASK,TSIO0_RX_ERR_INT_EN_SHIFT);
        _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_RECEIVE_EN);
    }
    else
    {
        _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_CLR);
        _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_CLR);
        _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL), TSIO0_RX_CC_RECEIVE_EN);
    }
}

MS_BOOL HAL_TSIO_CC_RxInt(void)
{
    MS_BOOL bRet = FALSE;
    if ((_HAL_REG16_R(&(_TSIOCtrl0->RX_CC_STATUS)) & TSIO0_INT_RX_CC) == TSIO0_INT_RX_CC)
    {
        bRet = TRUE;
    }
    return bRet;
}

MS_U16 HAL_TSIO_CC_RxErrStatus(void)
{
    return (_HAL_REG16_R(&(_TSIOCtrl0->RX_CC_STATUS)) & TSIO0_ERR_FLAG_MASK);
}

MS_U16 HAL_TSIO_CC_RxReadSize(void)
{
    return (_HAL_REG16_R(&(_TSIOCtrl0->RX_CC_SIZE)) & TSIO0_RX_CC_SIZE_MASK);
}

MS_U32 HAL_TSIO_CC_RxReadData(MS_U8 u8Addr)
{
    _REG16_SET_MASK(&(_TSIOCtrl0->RX_CC_CNTRL2),u8Addr,TSIO0_RX_ACPU_ADDR_MASK,TSIO0_RX_ACPU_ADDR_SHIFT);
    _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_ST);
    _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_ST);

    return _HAL_REG32_R(&(_TSIOCtrl0->RX_CC_RDATA));
}

MS_U32 HAL_TSIO_CC_RxReadData_Fast(MS_U8 u8Addr)
{
    _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_ADDR_CONT_TRI);
    _REG16_SET_MASK(&(_TSIOCtrl0->RX_CC_CNTRL2),u8Addr,TSIO0_RX_ACPU_ADDR_MASK,TSIO0_RX_ACPU_ADDR_SHIFT);
    _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_CONT);

    return _HAL_REG32_R(&(_TSIOCtrl0->RX_CC_RDATA));
}

void HAL_TSIO_CC_RxReadData_FastEnable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET_MASK(&(_TSIOCtrl0->RX_CC_CNTRL2),0,TSIO0_RX_ACPU_ADDR_MASK,TSIO0_RX_ACPU_ADDR_SHIFT);
        _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_CONT);
        _REG16_SET(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_ST);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_CONT);
        _REG16_CLR(&(_TSIOCtrl0->RX_CC_CNTRL2),TSIO0_RX_ACPU_ST);
    }
}

void HAL_TSIO_CC_TxClr(void)
{
    _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_CLR);
    _REG16_CLR(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_CLR);
}

void HAL_TSIO_CC_TxIntClr(void)
{
    _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_INT_CLR);
    _REG16_CLR(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_INT_CLR);
}

void HAL_TSIO_CC_TxEnable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_INT_EN);
        _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_START);
    }
    else
    {
        _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_CLR);
        _REG16_CLR(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_CLR);
        _REG16_CLR(&(_TSIOCtrl0->TX_CC_CNTRL), TSIO0_TX_CC_START);
    }
}

void HAL_TSIO_CC_TxWriteData(MS_U8 u8Addr, MS_U32 u32Data)
{
    _HAL_REG32_W(&(_TSIOCtrl0->TX_CC_WDATA), u32Data);
    _REG16_SET_MASK(&(_TSIOCtrl0->TX_CC_CNTRL2),u8Addr,TSIO0_TX_ACPU_ADDR_MASK,TSIO0_TX_ACPU_ADDR_SHIFT);
    _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_RW);
    _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_ST);
    _REG16_SET_MASK(&(_TSIOCtrl0->TX_CC_CNTRL2),0x00,TSIO0_TX_ACPU_ADDR_MASK,TSIO0_TX_ACPU_ADDR_SHIFT);
    _REG16_CLR(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_ST);
}

void HAL_TSIO_CC_TxWriteData_Fast(MS_U32 u32Data)
{
    _HAL_REG32_W(&(_TSIOCtrl0->TX_CC_WDATA), u32Data);
}

void HAL_TSIO_CC_TxWriteData_FastEnable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET_MASK(&(_TSIOCtrl0->TX_CC_CNTRL2),0,TSIO0_TX_ACPU_ADDR_MASK,TSIO0_TX_ACPU_ADDR_SHIFT);
        _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_CONT);
        _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_RW);
        _REG16_SET(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_ST);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_CONT);
        _REG16_CLR(&(_TSIOCtrl0->TX_CC_CNTRL2),TSIO0_TX_ACPU_ST);
    }
}

void HAL_TSIO_CC_TxWriteSize(MS_U16 u16Size)
{
    _REG16_SET_MASK(&(_TSIOCtrl0->TX_CC_SIZE),u16Size,TSIO0_CC_SIZE_MASK,TSIO0_CC_SIZE_SHIFT);
}

MS_BOOL HAL_TSIO_CC_TxInt(void)
{
    MS_BOOL bRet = FALSE;
    if ((_HAL_REG16_R(&(_TSIOCtrl0->TX_CC_STATUS)) & TSIO0_TX_CC_STATUS_INT_TX_CC) == TSIO0_TX_CC_STATUS_INT_TX_CC)
    {
        bRet = TRUE;
    }
    return bRet;
}

//=======================================================
// Loc Decrypt API
//=======================================================

void HAL_TSIO_LOCDEC_Xiu_Lock(MS_U8 u8Xiu)
{
    _REG16_SET_MASK(&(_TSIOCtrlLOCDEC->XIU),u8Xiu,TSIO_LOCDEC_XIU_ID_MASK,TSIO_LOCDEC_XIU_ID_SHIFT);
}

void HAL_TSIO_LOCDEC_Xiu_Unlock(void)
{
    _REG16_SET_MASK(&(_TSIOCtrlLOCDEC->XIU),0,TSIO_LOCDEC_XIU_ID_MASK,TSIO_LOCDEC_XIU_ID_SHIFT);
}

void HAL_TSIO_LOCDEC_ScbFixRule(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrlLOCDEC->SCBFIXRULE),TSIO_LOCDEC_SCBFIXRULE);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlLOCDEC->SCBFIXRULE),TSIO_LOCDEC_SCBFIXRULE);
    }
}

MS_BOOL HAL_TSIO_LOCDEC_Key_Set( MS_U8 u8Sid, MS_U8 *pKey, MS_U16 u16KeySize, MS_BOOL bOddKey)
{
    MS_U16 u16Data = 0;
    MS_S16 index = 0;
    MS_U8 addr = 0;

    if (pKey == NULL || u16KeySize != TSIO_LOCDEC_KEY_LEN)
    {
        return FALSE;
    }

    for (index = TSIO_LOCDEC_KEY_LEN-1; index >= 0; index-=2)
    {
        u16Data = (pKey[index-1]<<8) + pKey[index];
        _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SW_KEY[addr]),u16Data);
        addr++;
    }

    _REG16_SET_MASK(&(_TSIOCtrlLOCDEC->CMD),u8Sid,TSIO_LOCDEC_CMD_SERVICE_ID_MASK,TSIO_LOCDEC_CMD_SERVICE_ID_SHIFT);

    if (bOddKey)
    {
        _REG16_SET(&(_TSIOCtrlLOCDEC->CMD), TSIO_LOCDEC_CMD_KEY_SEL);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlLOCDEC->CMD), TSIO_LOCDEC_CMD_KEY_SEL);
    }

    _REG16_SET(&(_TSIOCtrlLOCDEC->CMD), TSIO_LOCDEC_CMD_CMD);
    _REG16_SET(&(_TSIOCtrlLOCDEC->KT_GO), TSIO_LOCDEC_KT_GO);

    return TRUE;
}

MS_BOOL HAL_TSIO_LOCDEC_KeySetDone(void)
{
    return (_HAL_REG16_R(&(_TSIOCtrlLOCDEC->KT_DONE)) & TSIO_LOCDEC_KT_DONE);
}

void HAL_TSIO_LOCDEC_KeyEnable(MS_U8 u8Sid, MS_BOOL bEnable)
{
    MS_U16 u16KeyEnableBit[4];

    u16KeyEnableBit[0] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[0]));
    u16KeyEnableBit[1] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[1]));
    u16KeyEnableBit[2] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[2]));
    u16KeyEnableBit[3] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[3]));

    MS_U64 u64Mask = 0;
    MS_U64 u64Data = u16KeyEnableBit[0] + ((MS_U64)u16KeyEnableBit[1] << 16)
        + ((MS_U64)u16KeyEnableBit[2] << 32) + ((MS_U64)u16KeyEnableBit[3] << 48);

    if (bEnable)
    {
        u64Mask = (MS_U64)(0x1) << u8Sid;
        u64Data |= u64Mask;
    }
    else
    {
        u64Mask = ~((MS_U64)(0x1) << u8Sid);
        u64Data &= u64Mask;
    }

    u16KeyEnableBit[0] = (u64Data & 0x000000000000FFFF);
    u16KeyEnableBit[1] = (u64Data & 0x00000000FFFF0000)>>16;
    u16KeyEnableBit[2] = (u64Data & 0x0000FFFF00000000)>>32;
    u16KeyEnableBit[3] = (u64Data & 0xFFFF000000000000)>>48;

    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[0]), u16KeyEnableBit[0]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[1]), u16KeyEnableBit[1]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[2]), u16KeyEnableBit[2]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->KEY_ENABLE_BIT[3]), u16KeyEnableBit[3]);
}

void HAL_TSIO_LOCDEC_PrivilegeSlotEnable(MS_U8 u8Sid, MS_BOOL bEnable)
{
    MS_U16 u16PrivilegeBit[4];

    u16PrivilegeBit[0] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[0]));
    u16PrivilegeBit[1] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[1]));
    u16PrivilegeBit[2] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[2]));
    u16PrivilegeBit[3] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[3]));

    MS_U64 u64Mask = 0;
    MS_U64 u64Data = u16PrivilegeBit[0] + ((MS_U64)u16PrivilegeBit[1] << 16)
        + ((MS_U64)u16PrivilegeBit[2] << 32) + ((MS_U64)u16PrivilegeBit[3] << 48);

    if (bEnable)
    {
        u64Mask = (MS_U64)(0x1) << u8Sid;
        u64Data |= u64Mask;
    }
    else
    {
        u64Mask = ~((MS_U64)(0x1) << u8Sid);
        u64Data &= u64Mask;
    }

    u16PrivilegeBit[0] = (u64Data & 0x000000000000FFFF);
    u16PrivilegeBit[1] = (u64Data & 0x00000000FFFF0000)>>16;
    u16PrivilegeBit[2] = (u64Data & 0x0000FFFF00000000)>>32;
    u16PrivilegeBit[3] = (u64Data & 0xFFFF000000000000)>>48;

    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[0]), u16PrivilegeBit[0]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[1]), u16PrivilegeBit[1]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[2]), u16PrivilegeBit[2]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_PRIVILEGE[3]), u16PrivilegeBit[3]);
}

void HAL_TSIO_LOCDEC_SecureSlotEnable(MS_U8 u8Sid, MS_BOOL bEnable)
{
    MS_U16 u16SecureBit[4];

    u16SecureBit[0] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_SECURE[0]));
    u16SecureBit[1] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_SECURE[1]));
    u16SecureBit[2] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_SECURE[2]));
    u16SecureBit[3] = _HAL_REG16_R(&(_TSIOCtrlLOCDEC->SLOT_SECURE[3]));

    MS_U64 u64Mask = 0;
    MS_U64 u64Data = u16SecureBit[0] + ((MS_U64)u16SecureBit[1] << 16)
        + ((MS_U64)u16SecureBit[2] << 32) + ((MS_U64)u16SecureBit[3] << 48);

    if (bEnable)
    {
        u64Mask = (MS_U64)(0x1) << u8Sid;
        u64Data |= u64Mask;
    }
    else
    {
        u64Mask = ~((MS_U64)(0x1) << u8Sid);
        u64Data &= u64Mask;
    }

    u16SecureBit[0] = (u64Data & 0x000000000000FFFF);
    u16SecureBit[1] = (u64Data & 0x00000000FFFF0000)>>16;
    u16SecureBit[2] = (u64Data & 0x0000FFFF00000000)>>32;
    u16SecureBit[3] = (u64Data & 0xFFFF000000000000)>>48;

    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_SECURE[0]), u16SecureBit[0]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_SECURE[1]), u16SecureBit[1]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_SECURE[2]), u16SecureBit[2]);
    _HAL_REG16_W(&(_TSIOCtrlLOCDEC->SLOT_SECURE[3]), u16SecureBit[3]);
}

void HAL_TSIO_LOCDEC_SetSWKeyProperty(MS_U32 keyProperty)
{
    _HAL_REG32_W(&(_TSIOCtrlLOCDEC->SW_KEY_PROPERTY), keyProperty);
}

//=======================================================
// Get Infomation API
//=======================================================
MS_U16 HAL_TSIO_GetLoselockcnt(void)
{
    MS_U16 u16rdata = 0;
    u16rdata = _HAL_REG16_R(&(_TSIOCtrl1->LOSE_LOCK_CNT));
    return u16rdata;
}

//=======================================================
// Bit training API
//=======================================================
void HAL_TSIO_Bittraining_init(void)
{
    ANALOG_PHASE_SETTING *Setting = &SETTING[u8Phasetable][0];
    _REG16_SET_MASK(&(_TSIOCtrl1->CHG_PH_STABLE_TIME), ANALOG_PHASE_GAP, TSIO1_CHG_PH_STABLE_TIME_GUARD_PH_MASK, TSIO1_CHG_PH_STABLE_TIME_GUARD_PH_SHIFT);
    MsOS_DelayTask(1);
    _REG16_SET(&(_TSIOCtrl1->CHG_PH_STABLE_TIME),TSIO1_CHG_PH_STABLE_TIME_GUARD_PH_LOAD);
    MsOS_DelayTask(1);
    _REG16_CLR(&(_TSIOCtrl1->CHG_PH_STABLE_TIME),TSIO1_CHG_PH_STABLE_TIME_GUARD_PH_LOAD);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl1->BTRAIN_CONFIG3), 0x8000);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl1->BTRAIN_CONFIG0), 0x8010);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl1->BTRAIN_CONFIG0), 0x0010);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl1->BTRAIN_CONFIG3), 0x8010);
    _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_PH_INFO_SEL);
    MsOS_DelayTask(1);
    //_REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG4), TSIO1_BTRAIN_CONFIG4_DISABLE_BITTRAIN);
    //====skew1====
    //Coarse
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_24), Setting[11].u8Corse,
                    TSIO_PHY_24_LPLL_SKEW1_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW1_COARSE_SHIFT);
    MsOS_DelayTask(1);
    //Fine
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_23), Setting[11].u8Fine,
                    TSIO_PHY_23_LPLL_SKEW1_FINE_MASK, TSIO_PHY_23_LPLL_SKEW1_FINE_SHIFT);
    MsOS_DelayTask(1);
    //Delay1t
    if (Setting[11].u8Delay1T)
    {
        _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW1_DELAYlt);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW1_DELAYlt);
    }
    MsOS_DelayTask(1);
}

void HAL_TSIO_Bittraining_init_dynamic(void)
{
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_18), 0x20,TSIO_PHY_18_TEST_TSIO_MASK, TSIO_PHY_18_TEST_TSIO_SHIFT); // loopback
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl0->AD_TX_CONFIG0), 0x0288);
    _HAL_REG16_W(&(_TSIOCtrl0->PRBS_CONFIG0), 0x1007);
    _HAL_REG16_W(&(_TSIOCtrl0->AD_RX_CONFIG0), 0x0000);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl1->BTRAIN_CONFIG0), 0x9010);
    _HAL_REG16_W(&(_TSIOCtrl1->BTRAIN_CONFIG0), 0x1010);
    _HAL_REG16_W(&(_TSIOCtrl1->BTRAIN_CONFIG3), 0x8010);
    MsOS_DelayTask(1);
}

MS_BOOL HAL_TSIO_Bittraining_Getremapstate(void)
{
    MS_U16 u16Data = 0;
    MS_BOOL bRet = 0;
    u16Data = _HAL_REG16_R(&(_TSIOCtrl0->BTRAIN_ST_0));

    bRet = ((u16Data & TSIO0_BTRAIN_ST_0_CHG_PH_START_REMAP) >> TSIO0_BTRAIN_ST_0_CHG_PH_START_REMAP_SHIFT);
    return bRet;
}

void HAL_TSIO_Bittraining_Setvalue(MS_BOOL bEarly, MS_BOOL bLate)
{
    _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_DEBUG_REG_LOAD);
    //_REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_SW_CH_PH_REMAP);
    if (bEarly)
    {
        _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_SW_EARLY);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_SW_EARLY);
    }
    if (bLate)
    {
        _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_SW_LATE);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_SW_LATE);
    }
    MsOS_DelayTaskUs(1);
    _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_SW_CHG_PH_DONE);
    MsOS_DelayTaskUs(1);
    _REG16_CLR(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_SW_CHG_PH_DONE);
    MsOS_DelayTaskUs(1);
}

MS_U8 HAL_TSIO_Bittraining_GetPhase(MS_U8 u8Phase)
{
    MS_U16 u16Data = 0;
    MS_U8 u8Ret = 0;

    //0: phase, 1: eraly, 2: late
    _REG16_SET_MASK(&(_TSIOCtrl1->BTRAIN_CONFIG3), u8Phase, TSIO1_CHANNEL_SEL_MASK, TSIO1_CHANNEL_SEL_SHIFT);
    MsOS_DelayTask(1);
    u16Data = _HAL_REG16_R(&(_TSIOCtrl0->PH_INFO_2));
    u8Ret = (u16Data & TSIO0_PH_INFO_2_REG_PHASE_MASK) >> TSIO0_PH_INFO_2_REG_PHASE_SHIFT;
    return u8Ret;
}

void HAL_TSIO_Bittraining_GetElinfo(MS_BOOL bEarly)
{
    MS_U16 u16Data[3] = {0};

    if (bEarly)
    {
        _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_EL_INFO_SEL);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_EL_INFO_SEL);
    }
    MsOS_DelayTask(1);

    u16Data[0] = _HAL_REG16_R(&(_TSIOCtrl0->EL_INFO[0]));
    u16Data[1] = _HAL_REG16_R(&(_TSIOCtrl0->EL_INFO[1]));
    u16Data[2] = _HAL_REG16_R(&(_TSIOCtrl0->EL_INFO[2]));

    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE, printf("[0x75][0x74][0x73] = [%04X][%04X][%04X]\n",
        u16Data[2], u16Data[1], u16Data[0]));
}

void HAL_TSIO_Bittraining_GetPhaseinfo(void)
{
    MS_U16 u16Data[4] = {0};

    MsOS_DelayTask(1);
    u16Data[0] = _HAL_REG16_R(&(_TSIOCtrl0->PH_INFO_0));
    u16Data[1] = _HAL_REG16_R(&(_TSIOCtrl0->PH_INFO_1));
    u16Data[2] = _HAL_REG16_R(&(_TSIOCtrl0->PH_INFO_2));
    u16Data[3] = _HAL_REG16_R(&(_TSIOCtrl0->BTRAIN_ST_0));

    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE, printf("[0x70][0x71][0x72][0x79] = [%04X][%04X][%04X][%04X]\n",
        u16Data[0], u16Data[1], u16Data[2], u16Data[3]));
}

MS_U16 HAL_TSIO_Bittraining_CheckPrbsErrcnt(void)
{
    _HAL_REG16_W(&(_TSIOCtrl0->PRBS_CONFIG0), 0x1000);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl0->PRBS_CONFIG0), 0x1007);
    MsOS_DelayTask(1);

    return _HAL_REG16_R(&(_TSIOCtrl0->DP_PHY_PRBS_ERRCNT));
}

void HAL_TSIO_Int_Bittraining_Enable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl1->BTRAIN_INT_EN), TSIO1_BTRAIN_INT_EN);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl1->BTRAIN_INT_EN), TSIO1_BTRAIN_INT_EN);
    }
}

void HAL_TSIO_Int_Bittraining_Clear(void)
{
    _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_BTRAIN_INT_CLR);
    MsOS_DelayTask(1);
    _REG16_CLR(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_BTRAIN_INT_CLR);
    MsOS_DelayTask(1);
}

MS_BOOL HAL_TSIO_Int_Bittraining_Read(void)
{
    MS_U16 u16Data = 0;
    MS_BOOL bRet = FALSE;
    u16Data = _HAL_REG16_R(&(_TSIOCtrl1->BTRAIN_INT_STATUS));

    if (u16Data & TSIO1_GUARD_REACH_MIN)
    {
        bRet = TRUE;
    }

    return bRet;
}

MS_U32 HAL_TSIO_DBG_Read(MS_U16 u16Dbg)
{
    MS_U32 u32Data = 0;
    _HAL_REG16_W(&(_TSIOCtrl1->TSIO_DBG_SEL), u16Dbg);
    u32Data = _HAL_REG32_R(&(_TSIOCtrl1->TSIO_DEBUG));
    return u32Data;
}

//=======================================================
// Interrupt API
//=======================================================
MS_BOOL HAL_TSIO_Int_Operation_Read(void)
{
    MS_U16 u16Data = 0;
    MS_BOOL bRet = FALSE;
    u16Data = _HAL_REG16_R(&(_TSIOCtrl0->PUH_STATUS0));

    if (u16Data & TSIO0_PUH_STATUS0_OPERATION)
    {
        bRet = TRUE;
    }

    return TRUE;
}

void HAL_TSIO_Int_Operation_Enable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _REG16_SET(&(_TSIOCtrl0->PUH_CONFIG2), TSIO0_PUH_CONFIG2_OPERATION_START_EN);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrl0->PUH_CONFIG2), TSIO0_PUH_CONFIG2_OPERATION_START_EN);
    }
}

void HAL_TSIO_Int_Operation_Clear(void)
{
    _REG16_SET(&(_TSIOCtrl0->PUH_CONFIG2), TSIO0_PUH_CONFIG2_OPERATION_START_CLR);
    _REG16_CLR(&(_TSIOCtrl0->PUH_CONFIG2), TSIO0_PUH_CONFIG2_OPERATION_START_CLR);
}

//=======================================================
// Analog phase API
//=======================================================

void HAL_TSIO_Analogphase_Pllreset(void)
{
    _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PD);
    MsOS_DelayTask(1);
    _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PD);
    MsOS_DelayTask(1);
}

static MS_U8 _HAL_TSIO_Analogphase_Tableswitch(HAL_TSIO_CLK eClk)
{
    MS_U8 u8Ret = 0;

    switch (eClk)
    {
        default:
        case HAL_TSIO_CLK_200M:
            u8Ret = 0;
            break;

        case HAL_TSIO_CLK_100M:
            u8Ret = 1;
            break;

        case HAL_TSIO_CLK_50M:
            u8Ret = 2;
            break;

        case HAL_TSIO_CLK_25M:
            u8Ret = 3;
            break;
    }
    return u8Ret;
}

void HAL_TSIO_Analogphase_Init(HAL_TSIO_CLK eClk, MS_BOOL bLoopback)
{
    MS_BOOL bBit15 = FALSE;
    MS_U8 u8DA_OV = 0;
    MS_U8 u8DB_OV = 0;

    u8Phasetable = _HAL_TSIO_Analogphase_Tableswitch(eClk);

    _REG16_CLR(&(_TSIOCtrl0->SW_RSTZ), TSIO0_SW_RST_ANA_RX);

#if (BITTRAINING_SW_MODE_ENABLE == 1)
    _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_22),TSIO_PHY_22_LPLL_SW_DEBUG_EN);
#endif
    MsOS_DelayTask(1);
    //toggle to force phase jump, will not bit training done until SW exit
    if (bLoopback)
    {
        _HAL_REG16_W(&(_TSIOCtrl0->PUH_CONFIG1), 0x0869); //for loopback
    }
    else
    {
        _HAL_REG16_W(&(_TSIOCtrl0->PUH_CONFIG1), 0x0809);
    }
    MsOS_DelayTask(1);
    _REG16_SET(&(_TSIOCtrl0->SW_RSTZ), TSIO0_SW_RSTZ);
    MsOS_DelayTask(1);
    _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_BTRAIN_INT_CLR);
    MsOS_DelayTask(1);
    _REG16_CLR(&(_TSIOCtrl1->BTRAIN_CONFIG3), TSIO1_BTRAIN_INT_CLR);
    MsOS_DelayTask(1);
    _REG16_SET(&(_TSIOCtrl1->BTRAIN_INT_EN), TSIO1_BTRAIN_INT_EN);
#if (BITTRAINING_SW_MODE_ENABLE == 1)
    _REG16_SET(&(_TSIOCtrl1->BTRAIN_CONFIG0), TSIO1_SW_JUMP_PH_CH);
    MsOS_DelayTask(1);
    _REG16_CLR(&(_TSIOCtrl1->BTRAIN_CONFIG0), TSIO1_SW_JUMP_PH_CH);
    MsOS_DelayTask(1);
#endif
    //_REG16_SET(&(_TSIOCtrl0->AD_TX_CONFIG0), TSIO0_CH_SWAP);
    //MsOS_DelayTask(1);

    //TSIO_ATOP RX calibration
    _HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_00), 0x807E);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_00), 0x007E);
    MsOS_DelayTask(1);
    do
    {
        bBit15 = (_HAL_REG16_R(&(_TSIOCtrlPHY->TSIO_PHY_0B)) & TSIO_PHY_0B_RX_CAL_END) >> TSIO_PHY_0B_RX_CAL_END_SHIFT;
        MsOS_DelayTask(1);
    }while(!bBit15);
    u8DA_OV = (_HAL_REG16_R(&(_TSIOCtrlPHY->TSIO_PHY_0B)) & TSIO_PHY_0B_RX_CAL_DATA_A_MASK) >> TSIO_PHY_0B_RX_CAL_DATA_A_SHIFT;
    u8DB_OV = (_HAL_REG16_R(&(_TSIOCtrlPHY->TSIO_PHY_0B)) & TSIO_PHY_0B_RX_CAL_DATA_B_MASK) >> TSIO_PHY_0B_RX_CAL_DATA_B_SHIFT;
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_06), (0x40|u8DA_OV), TSIO_PHY_06_GCR_RX_CA_DA_OV_MASK, TSIO_PHY_06_GCR_RX_CA_DA_OV_SHIFT);
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_06), (0x40|u8DB_OV), TSIO_PHY_06_GCR_RX_CA_DB_OV_MASK, TSIO_PHY_06_GCR_RX_CA_DB_OV_SHIFT);

    //TSIO
    //_HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_00), 0x007E);
    //MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_0F), 0x0010);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl0->MAX_PERIOD_OF_PKT_SYNC), 0x0000); // for test
    MsOS_DelayTask(1);

    //LPLL
    _HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_20), 0x0000);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_21), 0x0002);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_22), 0x8138);
    MsOS_DelayTask(1);
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_22), u8Phasetable, TSIO_PHY_22_LPLL_SKEW_DIV_MASK,TSIO_PHY_22_LPLL_SKEW_DIV_SHIFT);
    MsOS_DelayTask(1);
    _REG32_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_30_31), 0x19EB85, TSIO_PHY_30_31_LPLL_SYNTH_SET_MASK, TSIO_PHY_30_31_LPLL_SYNTH_SET_SHIFT);
    MsOS_DelayTask(1);

    //TSIO
    _REG16_SET(&(_TSOCtrl0->TSO_CONFIG5), TSO0_TSIO_MODE);
    _REG16_SET(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_INSERT_CTS_IN_TX);
    _REG16_SET(&(_TSIOCtrl0->TX_CONFIG0), TSIO0_TURN_OFF_MCM_TSIO);
    _HAL_REG16_W(&(_TSIOCtrl0->MAX_PERIOD_OF_SOC_TRAINING), 0x0000);

#if (BITTRAINING_SW_MODE_ENABLE == 1)
    if (bLoopback)
    {
        _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_18), 0x20,TSIO_PHY_18_TEST_TSIO_MASK, TSIO_PHY_18_TEST_TSIO_SHIFT); // loopback
        MsOS_DelayTask(1);
        _HAL_REG16_W(&(_TSIOCtrl0->PUH_CONFIG1), 0x0859); //for loopback
        MsOS_DelayTask(1);
    }
#else
    //====skew====
    switch(eClk)
    {
        default:
        case HAL_TSIO_CLK_200M:
            //Coarse (200MHz)
            _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_24), 0x0002,
                            TSIO_PHY_24_LPLL_SKEW1_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW1_COARSE_SHIFT);
            break;
        case HAL_TSIO_CLK_100M:
            //Coarse (100MHz)
            _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_24), 0x0004,
                    TSIO_PHY_24_LPLL_SKEW1_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW1_COARSE_SHIFT);
            break;
        case HAL_TSIO_CLK_50M:
            //Coarse (50MHz)
            _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_24), 0x0008,
                    TSIO_PHY_24_LPLL_SKEW1_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW1_COARSE_SHIFT);
            break;
        case HAL_TSIO_CLK_25M:
            //Coarse (25MHz)
            _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_24), 0x0010,
                    TSIO_PHY_24_LPLL_SKEW1_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW1_COARSE_SHIFT);
            break;
    }
    MsOS_DelayTaskUs(1);
#endif
    _REG16_SET(&(_TSIOCtrl1->RX_CONFIG0), TSIO1_DECRYPT_DISABLE);

    //enable operation start interrupt
    _REG16_SET(&(_TSIOCtrl0->PUH_CONFIG2), TSIO0_PUH_CONFIG2_OPERATION_START_EN);

    //phase DAC power up 10%
    _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_00), TSIO_PHY_00_REG_REF_SEL);
    MsOS_DelayTask(1);
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_02), 3, TSIO_PHY_02_TEST_REG_MASK, TSIO_PHY_02_TEST_REG_SHIFT);
    MsOS_DelayTask(1);

}

void HAL_TSIO_Analogphase_Exit(void)
{
    //_HAL_REG16_W(&(_TSIOCtrlPHY->TSIO_PHY_00), 0x107E);
    //MsOS_DelayTask(1);

    _REG16_CLR(&(_TSIOCtrl0->SW_RSTZ), TSIO0_SW_RSTZ);//low active
    _REG16_SET(&(_TSIOCtrl0->SW_RSTZ), TSIO0_SW_RST_ANA_RX);
    MsOS_DelayTask(1);
}

void HAL_TSIO_Analogphase_Set(MS_BOOL bSelect, MS_U8 u8Phase, MS_U8 u8Gap)
{
    MS_U8 u8Phase_pre = 0;
    MS_U8 u8Phase_next = 0;
    ANALOG_PHASE_SETTING *Setting = &SETTING[u8Phasetable][0];
    static MS_BOOL bReset = TRUE;
    static MS_U32 u32Count = 0;

    if (u8Gap >= ANALOG_PHASE_GAP)
    {
        u8Gap = ANALOG_PHASE_GAP;
    }

    if (u8Phase < u8Gap)
    {
        u8Phase_pre = ANALOG_PHASE_MAX + u8Phase - u8Gap + 1;
    }
    else
    {
        u8Phase_pre = u8Phase - u8Gap;
    }

    u8Phase_next = u8Phase + u8Gap;
    if (u8Phase_next > ANALOG_PHASE_MAX)
    {
        u8Phase_next = u8Phase_next - ANALOG_PHASE_MAX - 1;
    }

    //phdac reset
    if (bReset)
    {
        HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE, printf("phdac Reset\n"));
        _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_RST);
        MsOS_DelayTaskUs(1);
        _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_RST);
        MsOS_DelayTaskUs(1);
        bReset = FALSE;
    }

    MS_U8 u8Skew_phase = 11;
    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE,printf("Count   [%d]\n",u32Count++));
    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE,printf("Phase   [%d, %d, %d, %d]\n",u8Skew_phase, u8Phase_pre, u8Phase, u8Phase_next));
    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE,printf("Select  [%d, %d, %d, %d]\n",bSelect, bSelect, bSelect, bSelect));
    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE,printf("Delay1t [%d, %d, %d, %d]\n",Setting[u8Skew_phase].u8Delay1T,
        Setting[u8Phase_pre].u8Delay1T, Setting[u8Phase].u8Delay1T, Setting[u8Phase_next].u8Delay1T));
    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE,printf("Coarse  [%d, %d, %d, %d]\n",Setting[u8Skew_phase].u8Corse,
        Setting[u8Phase_pre].u8Corse, Setting[u8Phase].u8Corse, Setting[u8Phase_next].u8Corse));
    HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE,printf("Fine    [%d, %d, %d, %d]\n",Setting[u8Skew_phase].u8Fine,
        Setting[u8Phase_pre].u8Fine, Setting[u8Phase].u8Fine, Setting[u8Phase_next].u8Fine));
#if 1
    if (bSelect)
    {
        _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_SELECT);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_SELECT);
    }
#endif
    //select sw patch
    //_REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_SELECT);
    MsOS_DelayTaskUs(1);

    //====skew1====
    //Coarse
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_24), Setting[u8Skew_phase].u8Corse,
                    TSIO_PHY_24_LPLL_SKEW1_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW1_COARSE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Fine
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_23), Setting[u8Skew_phase].u8Fine,
                    TSIO_PHY_23_LPLL_SKEW1_FINE_MASK, TSIO_PHY_23_LPLL_SKEW1_FINE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Delay1t
    if (Setting[u8Skew_phase].u8Delay1T)
    {
        _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW1_DELAYlt);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW1_DELAYlt);
    }
    MsOS_DelayTaskUs(1);

    //====skew2====
    //Coarse
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_24), Setting[u8Phase_pre].u8Corse,
                    TSIO_PHY_24_LPLL_SKEW2_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW2_COARSE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Fine
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_23), Setting[u8Phase_pre].u8Fine,
                    TSIO_PHY_23_LPLL_SKEW2_FINE_MASK, TSIO_PHY_23_LPLL_SKEW2_FINE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Delay1t
    if (Setting[u8Phase_pre].u8Delay1T)
    {
        _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW2_DELAYlt);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW2_DELAYlt);
    }
    MsOS_DelayTaskUs(1);


    //====skew3====
    //Coarse
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_25), Setting[u8Phase].u8Corse,
                    TSIO_PHY_24_LPLL_SKEW3_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW3_COARSE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Fine
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_23), Setting[u8Phase].u8Fine,
                    TSIO_PHY_23_LPLL_SKEW3_FINE_MASK, TSIO_PHY_23_LPLL_SKEW3_FINE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Delay1t
    if (Setting[u8Phase].u8Delay1T)
    {
        _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW3_DELAYlt);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW3_DELAYlt);
    }
    MsOS_DelayTaskUs(1);

    //====skew4====
    //Coarse
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_25), Setting[u8Phase_next].u8Corse,
                    TSIO_PHY_24_LPLL_SKEW4_COARSE_MASK, TSIO_PHY_24_LPLL_SKEW4_COARSE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Fine
    _REG16_SET_MASK(&(_TSIOCtrlPHY->TSIO_PHY_23), Setting[u8Phase_next].u8Fine,
                    TSIO_PHY_23_LPLL_SKEW4_FINE_MASK, TSIO_PHY_23_LPLL_SKEW4_FINE_SHIFT);
    MsOS_DelayTaskUs(1);
    //Delay1t
    if (Setting[u8Phase_next].u8Delay1T)
    {
        _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW4_DELAYlt);
    }
    else
    {
        _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_SEL_SKEW4_DELAYlt);
    }
    MsOS_DelayTaskUs(1);

    //select sw patch
    //_REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_SELECT);
    //MsOS_DelayTask(1);

    //update
    _REG16_SET(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_UPDATE);
    MsOS_DelayTaskUs(1);
    _REG16_CLR(&(_TSIOCtrlPHY->TSIO_PHY_20), TSIO_PHY_20_LPLL_PHDAC_UPDATE);
    MsOS_DelayTaskUs(1);
}

void HAL_TSIO_Analogphase_Start(void)
{
    _HAL_REG16_W(&(_TSIOCtrl0->PUH_CONFIG0), 0x0062);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl0->PUH_CONFIG0), 0x0066);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl0->PUH_CONFIG0), 0x0000);
    MsOS_DelayTask(1);
}

MS_U16 HAL_TSIO_Analogphase_Read(MS_BOOL bEarly)
{
    MS_U16 u16Data = 0;
    MS_U16 u16ReadCount = 150;
    MS_U16 u16index = 0;
    MS_U16 u16Count = 0;
    MS_U16 u16Temp = 0;

    for (u16index = 0; u16index < u16ReadCount; u16index++)
    {
        if (bEarly)
        {
            u16Temp = _HAL_REG16_R(&(_TSIOCtrlPHY->TSIO_PHY_12));
            if (u16Temp)
            {
                u16Count++;
            }
            u16Data |= u16Temp;
        }
        else
        {
            u16Temp = _HAL_REG16_R(&(_TSIOCtrlPHY->TSIO_PHY_13));
            if (u16Temp)
            {
                u16Count++;
            }
            u16Data |= u16Temp;
        }
    }

    if (u16Data)
    {
        if (bEarly)
        {
            HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE, printf("Early error count = %d \n", u16Count));
        }
        else
        {
            HAL_TSIO_DBGMSG(E_HAL_TSIO_DBG_LEVEL_TRACE, printf("Late error count = %d \n", u16Count));
        }
    }

    return u16Data;
}

MS_U16 HAL_TSIO_Analogphase_GetPuhstatus(void)
{
    MS_U16 u16Data = 0;

    u16Data = (_HAL_REG16_R(&(_TSIOCtrl0->PUH_STATUS0)) & TSIO0_PUH_STATUS0_POWER_STATUS_MASK)
        >> TSIO0_PUH_STATUS0_POWER_STATUS_SHIFT;

    return u16Data;
}


MS_U16 HAL_TSIO_Analogphase_PrbsRead(void)
{
    MS_U16 u16Data = 0;

    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl0->PRBS_CONFIG0), 0x1000);
    MsOS_DelayTask(1);
    _HAL_REG16_W(&(_TSIOCtrl0->PRBS_CONFIG0), 0x1007);
    MsOS_DelayTask(1);
    u16Data = _HAL_REG16_R(&(_TSIOCtrl0->DP_PHY_PRBS_ERRCNT));

    return u16Data;
}

//=======================================================
// TSIO Filter API (use for bypass smc)
//=======================================================
void HAL_TSIO_Filter_Enable(MS_U16 u16FltId, MS_BOOL bEnable)
{
    if (u16FltId >= TSIO_FILTER_NUM)
        return;

    if (bEnable)
    {
        //_REG32_SET(&(_TSIOCtrl3->TSIO_FILTER[u16FltId]), TSIO3_FILTER_USE_PID_IN_FILTERING);
        _REG32_SET(&(_TSIOCtrl3->TSIO_FILTER[u16FltId]), TSIO3_FILTER_USE_SERVICE_ID_IN_FILTERING);
        _REG32_SET(&(_TSIOCtrl3->TSIO_FILTER[u16FltId]), TSIO3_FILTER_DATA_TO_TSIO_RX);
        _REG32_SET(&(_TSIOCtrl3->TSIO_FILTER[u16FltId]), TSIO3_FILTER_ENABLE);
    }
    else
    {
        _REG32_CLR(&(_TSIOCtrl3->TSIO_FILTER[u16FltId]), TSIO3_FILTER_ENABLE);
    }
}

void HAL_TSIO_Filter_SetPid(MS_U16 u16FltId, MS_U16 u16Pid)
{
    if (u16FltId >= TSIO_FILTER_NUM)
        return;

    _REG32_SET_MASK(&(_TSIOCtrl3->TSIO_FILTER[u16FltId]), u16Pid, TSIO3_FILTER_PID_MASK, TSIO3_FILTER_PID_SHIFT);
}

void HAL_TSIO_Filter_SetSid(MS_U16 u16FltId, MS_U8 u8Sid)
{
    if (u16FltId >= TSIO_FILTER_NUM)
        return;

    _REG32_SET_MASK(&(_TSIOCtrl3->TSIO_FILTER[u16FltId]), u8Sid, TSIO3_FILTER_SERVICE_ID_MASK, TSIO3_FILTER_SERVICE_ID_SHIFT);
}

//=======================================================
// Other API
//=======================================================
void HAL_TSIO_RxRecord_Start(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG16_W(&(_TSIOCtrl1->RXANA_TO_PVR), 0x2);
        _REG16_SET(&(_TSIOCtrl2->TSIO_RXANA_TO_PVR_DEBUG), TSIO2_TSIO_RXANA_TO_PVR_DEBG);
    }
    else
    {
        _HAL_REG16_W(&(_TSIOCtrl1->RXANA_TO_PVR), 0x0);
        _REG16_CLR(&(_TSIOCtrl2->TSIO_RXANA_TO_PVR_DEBUG), TSIO2_TSIO_RXANA_TO_PVR_DEBG);
    }
}

void HAL_TSIO_RxRecord_Setbuf(MS_U32 u32Headaddr, MS_U32 u32Tailaddr)
{
    _REG16_SET(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_STR2MI_EN);
    _HAL_REG32_W(&(_TSIOCtrl2->TSIO_PVR_STR2MI_HEAD_DEBUG), u32Headaddr);
    _HAL_REG32_W(&(_TSIOCtrl2->TSIO_PVR_STR2MI_TAIL_DEBUG), u32Tailaddr);
    _REG16_SET(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_STR2MI_RST_WADR);
    _REG16_CLR(&(_TSIOCtrl2->TSIO_PVR_CONFIG), TSIO2_PVR_STR2MI_RST_WADR);
}

void HAL_TSIO_TSP_BLK_Set(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _REG16_SET(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO0_BLK_EN);
        _REG16_SET(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO1_BLK_EN);
        _REG16_SET(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO2_BLK_EN);
        _REG16_SET(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO3_BLK_EN);
        _REG16_SET(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO4_BLK_EN);
        _REG16_SET(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO5_BLK_EN);
    }
    else
    {
        _REG16_CLR(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO0_BLK_EN);
        _REG16_CLR(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO1_BLK_EN);
        _REG16_CLR(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO2_BLK_EN);
        _REG16_CLR(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO3_BLK_EN);
        _REG16_CLR(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO4_BLK_EN);
        _REG16_CLR(&(_TSPCtrl8->HW8_CONFIG3), TSP_TSIF0_TSIO5_BLK_EN);
    }
}

void HAL_TSIO_STRLD_SET(MS_U8 u8index)
{
    MS_U8 u8Set = 0;

    switch (u8index)
    {
        case 0:
            u8Set = 2; //1.75v
            break;

        case 1:
            u8Set = 0; //1.8v
            break;

        case 2:
            u8Set = 1; //1.85v
            break;

        case 3:
            u8Set = 3; //1.9v
            break;

        default:
            return;
    }

    _REG16_SET_MASK(&(_TSIO_STRLD->REG_STRLD_32), u8Set, TEST_SDLDO_SEL_MASK, TEST_SDLDO_SEL_SHIFT);
}

