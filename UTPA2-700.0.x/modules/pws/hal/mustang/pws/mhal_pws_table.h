#ifndef _PWSTABLE_H_
#define _PWSTABLE_H_

#define PWS_TBL_VERSION  "0.0_2015/2/4"
#define PWS_First_Items 0
#define OutOfRange_PWS_Items 63

static const MS_U16 pws_table[] =
{
  0xFE3F, /*EN_MUX_RGB*/
  0xFE3F, /*PD_ICLP_RGB*/
  0xB979, /*PD_ICLP_VDY*/
  0xB9F9, /*PD_ICLP_VDC*/
  0xFEBF, /*PD_SOG_DAC*/
  0xFEBF, /*PD_SOG_MAIN*/
  0xFEBF, /*PD_SOG_MUX*/
  0xFEBF, /*PD_SOGOFF_DAC*/
  0xFEBF, /*PD_SOGOFF_MAIN*/
  0xFEBF, /*PD_SOGOFF_MUX*/
  0x3839, /*PD_BG*/
  0x3839, /*PD_REF*/
  0xFE3D, /*PD_REF_RGB*/
  0xB979, /*PD_REF_YUV*/
  0xB979, /*EN_MUX_VD_Y*/
  0xB9F9, /*EN_MUX_VD_C*/
  0xFFFD, /*EN_VDLPF_C*/
  0xB979, /*EN_VDLPF_Y*/
  0x3FFB, /*EN_IDAC1*/
  0x3FFB, /*EN_IDAC_REF*/
  0xFEFF, /*PD_HSYNC0_COMP*/
  0xB839, /*PD_ADCPLA_REG*/
  0xB979, /*PD_ADCPLB_REG*/
  0xFEBF, /*GCR_PGDIG_D_PD*/
  0xFE3D, /*PD_ADCR*/
  0xFE3F, /*PD_ADCG*/
  0xFE3F, /*PD_ADCB*/
  0xB979, /*PD_ADCY*/
  0xFF7F, /*PD_FBLANK*/
  0xFEBF, /*PD_ADCA_PHDAC_R*/
  0xFEBF, /*PD_ADCA_PHDAC_G*/
  0xFEBF, /*PD_ADCA_PHDAC_B*/
  0xFF7D, /*PD_ADCB_PHDAC_R*/
  0xFF7F, /*PD_ADCB_PHDAC_G*/
  0xFF7F, /*PD_ADCB_PHDAC_B*/
  0xF97D, /*PD_ADCB_PHDAC_Y*/
  0xB839, /*reg_pd_ldo_lv*/
  0xB839, /*reg_pd_ldo25_adca*/
  0xB839, /*reg_pd_ldo25_adcb*/
  0xB839, /*reg_pd_ldo25_ana*/
  0xFFFB, /*PD_SOGX_MAIN*/
  0xFFFB, /*PD_SOGX_DAC*/
  0xFFFB, /*PD_REF_X*/
  0xFFFB, /*PD_ADCX*/
  0xFFFB, /*PD_ADCB_PHDAC_X*/
  0xFFFB, /*EN_MUX_VD_X*/
  0xFFF9, /*EN_VDLPF_X*/
  0x03FF, /*PD_REF*/
  0x03FF, /*PD_CLPBUF*/
  0x03FF, /*PD_ADCI*/
  0x03FF, /*PD_DMLDO2P6I*/
  0x03FF, /*PD_DMLDO2P6Q*/
  0x03FF, /*PD_DMLDO2P5I*/
  0x03FF, /*PD_DMLDO1P1_CLK*/
  0x03FF, /*PD_DMLDO1P1_CMP*/
  0x03FF, /*MPLL_PD*/
  0x03FF, /*PD_DMPLL_CLK*/
  0x03FF, /*MPLL_ADC_CLK_PD*/
  0x03FF, /*MPLL_DIV2_PD*/
  0x03FF, /*MPLL_DIV3_PD*/
  0x03FF, /*MPLL_DIV4_PD*/
  0x03FF, /*MPLL_DIV8_PD*/
  0x03FF, /*MPLL_DIV10_PD*/
  0x47FF, /*IFAGC_T_ENABLE*/
};

#define PWS_Init_First_Items 0
#define OutOfRange_PWS_Init_Items 6

static const MS_U16 pws_init_table[] =
{
  0xFFFF, /*EN_IDAC2*/
  0xFFFF, /*PD_ADCA_PHDAC_Y*/
  0xFFFF, /*PD_VCALBUF*/
  0xFFFF, /*PD_ADCQ*/
  0xFFFF, /*PD_DMLDO2P5Q*/
  0xFFFF, /*IFAGC_S_ENABLE*/
};


#endif
