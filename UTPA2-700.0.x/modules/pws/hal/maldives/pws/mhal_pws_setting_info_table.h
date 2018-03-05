#ifndef _PWS_SETTING_INFO_TABLE_H_
#define _PWS_SETTING_INFO_TABLE_H_

#include "MsTypes.h"

typedef   struct   {
    MS_U32     u32RegAddr;
    MS_U8     u8RegMask;
    /*MS_U16     u16OffOnFlag;*/
    MS_BOOL     bInvert;
    const char regName[32];//*regName;
}PWS_TABLE_INFO;

static const PWS_TABLE_INFO pws_setting_info_table[] =
{
  //video_atop
  {0x102503, 0x40/*, 0xFE3F*/ /*off=0x00, on=0x01*/, TRUE, "EN_MUX_RGB"},
  {0x102509, 0x04/*, 0xFE3F*/ /*off=0x01, on=0x00*/, FALSE, "PD_ICLP_RGB"},
  {0x102509, 0x08/*, 0xB97D*/ /*off=0x01, on=0x00*/, FALSE, "PD_ICLP_VDY"},
  {0x102509, 0x10/*, 0xB9FD*/ /*off=0x01, on=0x00*/, FALSE, "PD_ICLP_VDC"},
  {0x10250a, 0x04/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOG_DAC"},
  {0x10250A, 0x08/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOG_MAIN"},
  {0x10257E, 0x01/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOG_MUX"},
  {0x10250A, 0x10/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOGOFF_DAC"},
  {0x10250A, 0x20/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOGOFF_MAIN"},
  {0x10257e, 0x02/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOGOFF_MUX"},
  {0x102508, 0x04/*, 0x383D*/ /*off=0x01, on=0x00*/, FALSE, "PD_BG"},
  {0x102508, 0x08/*, 0x383D*/ /*off=0x01, on=0x00*/, FALSE, "PD_REF"},
  {0x102508, 0x01/*, 0xFE3D*/ /*off=0x01, on=0x00*/, FALSE, "PD_REF_RGB"},
  {0x102508, 0x10/*, 0xB97D*/ /*off=0x01, on=0x00*/, FALSE, "PD_REF_YUV"},
  {0x102505, 0x01/*, 0xB97D*/ /*off=0x00, on=0x01*/, TRUE, "EN_MUX_VD_Y"},
  {0x102505, 0x02/*, 0xB9FD*/ /*off=0x00, on=0x01*/, TRUE, "EN_MUX_VD_C"},
  {0x102570, 0x02/*, 0xFFFD*/ /*off=0x00, on=0x01*/, TRUE, "EN_VDLPF_C"},
  {0x102570, 0x01/*, 0xB97D*/ /*off=0x00, on=0x01*/, TRUE, "EN_VDLPF_Y"},
  {0x102590, 0x08/*, 0x3FFF*/ /*off=0x00, on=0x01*/, TRUE, "EN_IDAC1"},
  {0x103d00, 0x10/*, 0x3FFF*/ /*off=0x00, on=0x01*/, TRUE, "EN_IDAC_REF"},
  {0x10250b, 0x01/*, 0xFEFF*/ /*off=0x01, on=0x00*/, FALSE, "PD_HSYNC0_COMP"},
  {0x102509, 0x02/*, 0xB83D*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCPLA_REG"},
  {0x10250A, 0x02/*, 0xB97D*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCPLB_REG"},
  {0x102508, 0x20/*, 0xFE3D*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCR"},
  {0x102508, 0x40/*, 0xFE3F*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCG"},
  {0x102508, 0x80/*, 0xFE3F*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCB"},
  {0x102509, 0x40/*, 0xB97D*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCY"},
  {0x102580, 0x40/*, 0xFF7F*/ /*off=0x01, on=0x00*/, FALSE, "PD_FBLANK"},
  {0x1025BC, 0x40/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCA_PHDAC_R"},
  {0x1025BC, 0x80/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCA_PHDAC_G"},
  {0x1025BD, 0x01/*, 0xFEBF*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCA_PHDAC_B"},
  {0x1025BD, 0x04/*, 0xFF7D*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCB_PHDAC_R"},
  {0x1025BD, 0x08/*, 0xFF7F*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCB_PHDAC_G"},
  {0x1025BD, 0x10/*, 0xFF7F*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCB_PHDAC_B"},
  {0x1025BD, 0x20/*, 0xF97D*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCB_PHDAC_Y"},
  {0x1025FC, 0x01/*, 0xB83D*/ /*off=0x01, on=0x00*/, FALSE, "reg_pd_ldo_lv"},
  {0x1025FC, 0x10/*, 0xB83D*/ /*off=0x01, on=0x00*/, FALSE, "reg_pd_ldo25_adca"},
  {0x1025FD, 0x01/*, 0xB83D*/ /*off=0x01, on=0x00*/, FALSE, "reg_pd_ldo25_adcb"},
  {0x1025FF, 0x10/*, 0xB83D*/ /*off=0x01, on=0x00*/, FALSE, "reg_pd_ldo25_ana"},
  {0x102571, 0x01/*, 0xFFFD*/ /*off=0x00, on=0x01*/, TRUE, "EN_VDLPF_X"},
  //demod_atop
  {0x112840, 0x10/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_REF"},
  {0x112818, 0x04/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_CLPBUF"},
  {0x112818, 0x01/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_ADC"},
  {0x112879, 0xf0/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "DMLDO_PD(2p5/2p6/1p1x2)"},
  {0x11286A, 0x80/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_PD"},
  {0x11286B, 0x20/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "PD_DMPLL_CLK"},
  {0x112860, 0x02/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_ADC_CLK_PD"},
  {0x112860, 0x04/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV2_PD"},
  {0x112860, 0x08/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV3_PD"},
  {0x112860, 0x10/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV4_PD"},
  {0x112860, 0x20/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV8_PD"},
  {0x112860, 0x40/*, 0x03FF*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV10_PD"},
};

static const PWS_TABLE_INFO pws_init_setting_info_table[] =
{
  {0x102592, 0x08/*, 0xFFFF*/ /*off=0x00, on=0x01*/, TRUE, "EN_IDAC2"},
  {0x1025BD, 0x02/*, 0xFFFF*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCA_PHDAC_Y"},
  {0x10250a, 0x80/*, 0xFFFF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOGX_MAIN"},
  {0x10250a, 0x40/*, 0xFFFF*/ /*off=0x01, on=0x00*/, FALSE, "PD_SOGX_DAC"},
  {0x10250b, 0x10/*, 0xFFFF*/ /*off=0x01, on=0x00*/, FALSE, "PD_REF_X"},
  {0x10250b, 0x20/*, 0xFFFF*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCX"},
  {0x1025BC, 0x02/*, 0xFFFF*/ /*off=0x01, on=0x00*/, FALSE, "PD_ADCB_PHDAC_X"},
  {0x102507, 0x04/*, 0xFFFF*/ /*off=0x00, on=0x01*/, TRUE, "EN_MUX_VD_X"},
  {0x11281E, 0x80/*, 0xFFFF*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_VCALBUF"},
};


#endif
