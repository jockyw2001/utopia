////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
//****************************************************
//   ADC initial table CodeGen Version 1.0
// Mooney_ADC_Driver_LDO
// 7/3/2016 21:28
//****************************************************

#ifndef _DRVADCTBL_H_
#define _DRVADCTBL_H_

//   ADC initial table CodeGen Version 1.0
#define DRV_ADC_REG(reg) ((reg>>8)&0xFF), (reg&0xFF)
#define REG_ADDR_SIZE       2
#define REG_MASK_SIZE       1
#define REG_DATA_SIZE       2
typedef enum
{
    ADC_TABLE_INIT_All,
    ADC_TABLE_INIT_NUMS
} ADC_INIT_TYPE;

typedef enum
{
    ADC_TABLE_FreeRunEn_All,
    ADC_TABLE_FreeRunEn_NUMS
} ADC_FreeRunEn_TYPE;

typedef enum
{
    ADC_TABLE_FreeRunDis_All,
    ADC_TABLE_FreeRunDis_NUMS
} ADC_FreeRunDis_TYPE;

typedef enum
{
    ADC_TABLE_PorstEn_All,
    ADC_TABLE_PorstEn_NUMS
} ADC_PorstEn_TYPE;

typedef enum
{
    ADC_TABLE_PorstDis_All,
    ADC_TABLE_PorstDis_NUMS
} ADC_PorstDis_TYPE;

typedef enum
{
    ADC_TABLE_MUX_RGB0_Sync,
    ADC_TABLE_MUX_RGB0_Data,
    ADC_TABLE_MUX_RGB1_Sync,
    ADC_TABLE_MUX_RGB1_Data,
    ADC_TABLE_MUX_RGB2_Sync,
    ADC_TABLE_MUX_RGB2_Data,
    ADC_TABLE_MUX_CVBSY0,
    ADC_TABLE_MUX_CVBSY1,
    ADC_TABLE_MUX_CVBSY2,
    ADC_TABLE_MUX_CVBSY3,
    ADC_TABLE_MUX_G0,
    ADC_TABLE_MUX_G1,
    ADC_TABLE_MUX_G2,
    ADC_TABLE_MUX_CVBSC0,
    ADC_TABLE_MUX_CVBSC1,
    ADC_TABLE_MUX_CVBSC2,
    ADC_TABLE_MUX_CVBSC3,
    ADC_TABLE_MUX_R0,
    ADC_TABLE_MUX_R1,
    ADC_TABLE_MUX_R2,
    ADC_TABLE_MUX_DVI0,
    ADC_TABLE_MUX_DVI1,
    ADC_TABLE_MUX_DVI2,
    ADC_TABLE_MUX_DVI3,
    ADC_TABLE_MUX_NUMS
} ADC_MUX_TYPE;

typedef enum
{
    ADC_TABLE_SOURCE_RGB,
    ADC_TABLE_SOURCE_YUV,
    ADC_TABLE_SOURCE_ATV,
    ADC_TABLE_SOURCE_INT_ATV,
    ADC_TABLE_SOURCE_SVIDEO,
    ADC_TABLE_SOURCE_SCART,
    ADC_TABLE_SOURCE_CVBS,
    ADC_TABLE_SOURCE_DVI,
    ADC_TABLE_SOURCE_MVOP,
    ADC_TABLE_SOURCE_RGB_SC,
    ADC_TABLE_SOURCE_RGB_AV,
    ADC_TABLE_SOURCE_RGB_DVI,
    ADC_TABLE_SOURCE_RGB_MV,
    ADC_TABLE_SOURCE_YUV_SC,
    ADC_TABLE_SOURCE_YUV_AV,
    ADC_TABLE_SOURCE_YUV_DVI,
    ADC_TABLE_SOURCE_YUV_MV,
    ADC_TABLE_SOURCE_MVOP_ATV,
    ADC_TABLE_SOURCE_MVOP_SV,
    ADC_TABLE_SOURCE_MVOP_SC,
    ADC_TABLE_SOURCE_MVOP_AV,
    ADC_TABLE_SOURCE_MVOP_DVI,
    ADC_TABLE_SOURCE_DVI_CVBS,
    ADC_TABLE_SOURCE_DVI_SV,
    ADC_TABLE_SOURCE_DVI_SC,
    ADC_TABLE_SOURCE_NUMS
} ADC_SOURCE_TYPE;

typedef enum
{
    ADC_TABLE_AdcCal_SW_UG,
    ADC_TABLE_AdcCal_Fix_UG,
    ADC_TABLE_AdcCal_BG_Fix_UG,
    ADC_TABLE_AdcCal_NUMS
} ADC_AdcCal_TYPE;

typedef enum
{
    ADC_TABLE_AdcCal_AV_ALL,
    ADC_TABLE_AdcCal_AV_NUMS
} ADC_AdcCal_AV_TYPE;

typedef enum
{
    ADC_TABLE_AdcCal_SV_ALL,
    ADC_TABLE_AdcCal_SV_NUMS
} ADC_AdcCal_SV_TYPE;

typedef enum
{
    ADC_TABLE_FREQ_SECTION1,
    ADC_TABLE_FREQ_SECTION2,
    ADC_TABLE_FREQ_SECTION3,
    ADC_TABLE_FREQ_SECTION4,
    ADC_TABLE_FREQ_SECTION5,
    ADC_TABLE_FREQ_SECTION6,
    ADC_TABLE_FREQ_SECTION7,
    ADC_TABLE_FREQ_SECTION8,
    ADC_TABLE_FREQ_SECTION9,
    ADC_TABLE_FREQ_SECTION10,
    ADC_TABLE_FREQ_SECTION11,
    ADC_TABLE_FREQ_SECTION12,
    ADC_TABLE_FREQ_SECTION13,
    ADC_TABLE_FREQ_SECTION14,
    ADC_TABLE_FREQ_SECTION15,
    ADC_TABLE_FREQ_SECTION16,
    ADC_TABLE_FREQ_SECTION17,
    ADC_TABLE_SetMode_NUMS
} ADC_SetMode_TYPE;

typedef enum
{
    ADC_TABLE_CVBSO_OFF,
    ADC_TABLE_CVBSO_SV_ON,
    ADC_TABLE_CVBSO_SV_OFF,
    ADC_TABLE_CVBSO_CVBS_ON,
    ADC_TABLE_CVBSO_CVBS_OFF,
    ADC_TABLE_CVBSO_VIF_DAC,
    ADC_TABLE_CVBSO_VIF_VIF,
    ADC_TABLE_CVBSO_NUMS
} ADC_CVBSO_TYPE;

typedef enum
{
    ADC_TABLE_CVBSO_MUX_CVBS0,
    ADC_TABLE_CVBSO_MUX_CVBS1,
    ADC_TABLE_CVBSO_MUX_CVBS2,
    ADC_TABLE_CVBSO_MUX_CVBS3,
    ADC_TABLE_CVBSO_MUX_SV_ON,
    ADC_TABLE_CVBSO_MUX_SV_OFF,
    ADC_TABLE_CVBSO_MUX_DAC,
    ADC_TABLE_CVBSO_MUX_R0,
    ADC_TABLE_CVBSO_MUX_R1,
    ADC_TABLE_CVBSO_MUX_R2,
    ADC_TABLE_CVBSO_MUX_G0,
    ADC_TABLE_CVBSO_MUX_G1,
    ADC_TABLE_CVBSO_MUX_G2,
    ADC_TABLE_CVBSO_MUX_NUMS
} ADC_CVBSO_MUX_TYPE;

typedef enum
{
    ADC_TABLE_PowerDown_NUMS
} ADC_PowerDown_TYPE;

typedef struct
{
    MS_U16 FreqHLimit;
    MS_U16 FreqLLimit;
} ADC_FREQ_RANGE;

#undef _DRVADCTBL_H_
#endif
