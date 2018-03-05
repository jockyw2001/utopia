

#define      Cmd_NopNop      0
#define      Cmd_WriteKey    1
#define      Cmd_InvKey      2
#define      Cmd_ReadSW      3
#define      Cmd_EnaKey      4

#define      Field_lsad      0
#define      Field_esa       1
#define      Field_lsas      2
#define      Field_switch    3
#define      Field_iv        4

#define      WMux_en         0
#define      WMux_cavid      1
#define      WMux_tsid       2
#define      WMux_even_idx   3
#define      WMux_odd_idx    4
#define      WMux_clr_idx    5
#define      WMux_dualpath   6

#define      AES             0  //Engine encoded
#define      DVBCSA2         1  //Engine encoded
#define      DES             2  //Engine encoded
#define      TDES            3  //Engine encoded
#define      MULTI2          4  //Engine encoded
#define      DVBCSA2CONF     5  //Engine encoded
#define      DVBCSA3         6  //Engine encoded

#define      ENTROPY64       0  // key entropy encoded
#define      ENTROPY128      1  // key entropy encoded

#define      NDS             1  //CAVid encoded
#define      NGA             2
#define      NAGRA           2  //CAVid encoded
#define      VIACCESS        3  //CAVid encoded
#define      IRDETO          4  //CAVid encoded
#define      VERIMATRIX      5  //CAVid encoded

#define      CPCM_MDI_CBC    0  //SubAlgo encoded
#define      CPCM_MDI_RCBC   1  //SubAlgo encoded
#define      CPCM_MDD_CBC    2  //SubAlgo encoded
#define      CPCM_MDD_RCBC   3  //SubAlgo encoded
#define      LEADING_CLR     4  //SubAlgo encoded
#define      ECB             5  //SubAlgo encoded
#define      CBC             6  //SubAlgo encoded
#define      CTR             7  //SubAlgo encoded
#define      OFB             8  //SubAlgo encoded

#define      CLR             0  //Res, SB
#define      CTS             1  //Res
#define      SCTE52          2  //Res
#define      XORIV1          3  //Res, SB
#define      OC_M            4  //Res
#define      XORIV2          5  //Res, SB

#define      CLR_SCB         2
#define      KEEP_SCB        0

#define      FIELD_LSAD       0
#define      FIELD_ESA        1
#define      FIELD_LSAS       2
#define      SOURCE_KEY0      0
#define      SOURCE_KEY1      1
#define      SOURCE_KEY2      2
#define      SOURCE_KEY3      3
#define      SOURCE_KEY4      4
#define      SOURCE_PRIVATE   6
#define      SOURCE_VGK       7
#define      SOURCE_PRIVATE1  8
#define      SOURCE_PRIVATE2  9

