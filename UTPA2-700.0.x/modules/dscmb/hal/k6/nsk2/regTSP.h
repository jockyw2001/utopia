#ifndef _TSP2_REG_H_
#define _TSP2_REG_H_
typedef struct _REG32
{
    volatile MS_U16                 low;
    volatile MS_U16                 _null_l;
    volatile MS_U16                 high;
    volatile MS_U16                 _null_h;
} REG32;

typedef struct _REG16
{
    volatile MS_U16                 data;                                //[jerry] not to name "low"
    volatile MS_U16                 _null;
} REG16;

typedef struct _TSP32
{
    volatile MS_U32                 reg32;
} TSP32;


//#########################################################################
//#### Hardware Capability Macro Start
//#########################################################################

#define TSP_TSIF_NUM                4
#define TSP_PVRENG_NUM              4
#define TSP_PVR_IF_NUM              4
#define TSP_OTVENG_NUM              4
#define STC_ENG_NUM                 4
#define TSP_PCRFLT_NUM              STC_ENG_NUM

#define TSP_PIDFLT_NUM              192
#define TSP_SECFLT_NUM              192
#define TSP_SECBUF_NUM              192

#define TSP_MERGESTREAM_NUM         8

//@NOTE: accroding to width of FW/VQ/SEC buffer base addr , lower / upper bound may be different
#define TSP_FW_BUF_LOW_BUD          0
#define TSP_FW_BUF_UP_BUD           ((1ULL << 32) - 1) // base addr: bits[31:4] , unit: 16-bytes (bits[3:0])
                                                       // base addr = {reg_dma_raddr_msb(8-bits),reg_dma_raddr(16-bits),4'b0(4-bits)}
#define TSP_VQ_BUF_LOW_BUD          0
#define TSP_VQ_BUF_UP_BUD           ((1ULL << 32) - 1) // base addr: bits[31:0] , unit: 1-byte
#define TSP_SEC_BUF_LOW_BUD         0
#define TSP_SEC_BUF_UP_BUD          ((1ULL << 32) - 1) // base addr: bits[31:0] , unit: 1-byte



//#########################################################################
//#### Hardware Capability Macro End
//#########################################################################


// PID Filter
typedef TSP32                       REG_PidFlt;                         // 0x210000

// TSIF
#define TSP_PIDFLT_TSIF_MASK        0x0000E000
#define TSP_PIDFLT_TSIF_SHFT        13

#define TSP_FILTER_DEPTH            16


//#########################################################################
//#### CLKGEN0 Bank:0x100B
//#########################################################################
#define TSP_CLKGEN0_REG(addr)       (*((volatile MS_U16*)(_u32RegBase + 0x1600 + ((addr)<<2))))

    #define REG_CLKGEN0_DC0_SYTNTH                  0x05
        #define REG_CLKGEN0_STC_CW_SEL              0x0002
        #define REG_CLKGEN0_STC_CW_EN               0x0004
        #define REG_CLKGEN0_STC1_CW_SEL             0x0200
        #define REG_CLKGEN0_STC1_CW_EN              0x0400

    #define REG_CLKGEN0_DC0_STC_CW_L                0x06
    #define REG_CLKGEN0_DC0_STC_CW_H                0x07
    #define REG_CLKGEN0_DC0_STC1_CW_L               0x08
    #define REG_CLKGEN0_DC0_STC1_CW_H               0x09

    #define REG_CLKGEN0_S2P_IN_CLK_SRC              0x0C
        #define REG_CLKGEN0_S2P_IN_CLK_SHIFT        0
        #define REG_CLKGEN0_S2P1_IN_CLK_SHIFT       8
        #define REG_CLKGEN0_S2P_IN_CLK_MASK         0x1F
        #define REG_CLKGEN0_S2P_IN_CLK_DISABLE      0x0001
        #define REG_CLKGEN0_S2P_IN_CLK_INVERT       0x0002
        #define REG_CLKGEN0_S2P_IN_CLK_SRC_SHIFT    2
        #define REG_CLKGEN0_S2P_IN_CLK_SRC_MASK     0x7

    #define REG_CLKGEN0_TSO0_CLK                    0x27
        #define REG_CLKGEN0_TSO0_SHIFT              0
    #define REG_CLKGEN0_TS0_CLK                     0x28
        #define REG_CLKGEN0_TS0_SHIFT               0
    #define REG_CLKGEN0_TS1_CLK                     0x28
        #define REG_CLKGEN0_TS1_SHIFT               8
    #define REG_CLKGEN0_TS2_CLK                     0x29
        #define REG_CLKGEN0_TS2_SHIFT               0
    #define REG_CLKGEN0_TS3_CLK                     0x29
        #define REG_CLKGEN0_TS3_SHIFT               8
        #define REG_CLKGEN0_TS_MASK                 0x003F  // 4 bit each
        #define REG_CLKGEN0_TS_DISABLE              0x0001
        #define REG_CLKGEN0_TS_INVERT               0x0002
        #define REG_CLKGEN0_TS_SRC_SHIFT            2
        #define REG_CLKGEN0_TS_SRC_MASK             0x000F
        #define REG_CLKGEN0_TS_SRC_EXT0             0x0000
        #define REG_CLKGEN0_TS_SRC_EXT1             0x0001
        #define REG_CLKGEN0_TS_SRC_EXT2             0x0002
        #define REG_CLKGEN0_TS_SRC_EXT3             0x0003
        #define REG_CLKGEN0_TS_SRC_EXT4             0x0004
        #define REG_CLKGEN0_TS_SRC_EXT5             0x0005
        #define REG_CLKGEN0_TS_SRC_EXT6             0x0006
        #define REG_CLKGEN0_TS_SRC_TSO0             0x0007
        #define REG_CLKGEN0_TS_SRC_TSIO0            0x0008
        //@NOTE Not support internal demod in KANO
        #define REG_CLKGEN0_TS_SRC_DMD0             0x000F

    //get TSP Clk Gen bank
    #define REG_CLKGEN0_TSP_CLK                     0x2A
        #define REG_CLKGEN0_TSP_CLK_MASK            0x001F
        #define REG_CLKGEN0_TSP_SHIFT               0
        #define REG_CLKGEN0_TSP_DISABLE             0x0001
        #define REG_CLKGEN0_TSP_INVERT              0x0002
        //SRC
        #define REG_CLKGEN0_TSP_SRC_SHIFT           2
        #define REG_CLKGEN0_TSP_SRC_MASK            0x0007
        #define REG_CLKGEN0_TSP_SRC_192MHZ          0x0000
        #define REG_CLKGEN0_TSP_SRC_172MHZ          0x0001
        #define REG_CLKGEN0_TSP_SRC_144MHZ          0x0002
        #define REG_CLKGEN0_TSP_SRC_108MHZ          0x0003
        #define REG_CLKGEN0_TSP_SRC_XTAL            0x0007

    //get STC0/1 Clk Gen bank
    #define REG_CLKGEN0_STC0_CLK                    0x2A
        #define REG_CLKGEN0_STC0_MASK               0x0F00
        #define REG_CLKGEN0_STC0_SHIFT              8
    #define REG_CLKGEN0_STC1_CLK                    0x2A
        #define REG_CLKGEN0_STC1_MASK               0xF000
        #define REG_CLKGEN0_STC1_SHIFT              12
        #define REG_CLKGEN0_STC_DISABLE             0x0001
        #define REG_CLKGEN0_STC_INVERT              0x0002
        //SRC
        #define REG_CLKGEN0_STC_SRC_SHIFT           2
        #define REG_CLKGEN0_STC_SRC_MASK            0x0003
        #define REG_CLKGEN0_STC_SRC_SYNTH           0x0000
        #define REG_CLKGEN0_STC_SRC_ONE             0x0001
        #define REG_CLKGEN0_STC_SRC_27M             0x0002
        #define REG_CLKGEN0_STC_SRC_XTAL            0x0003

    #define REG_CLKGEN0_STAMP_CLK                   0x2F
        #define REG_CLKGEN0_STAMP_MASK              0x0F00
        #define REG_CLKGEN0_STAMP_SHIFT             8
        #define REG_CLKGEN0_STAMP_DISABLE           0x0001
        #define REG_CLKGEN0_STAMP_INVERT            0x0002

    #define REG_CLKGEN0_PARSER_CLK                  0x39
        #define REG_CLKGEN0_PARSER_MASK             0x0F00
        #define REG_CLKGEN0_PARSER_SHIFT            8
        #define REG_CLKGEN0_PARSER_DISABLE          0x0001
        #define REG_CLKGEN0_PARSER_INVERT           0x0002

//#########################################################################
//#### CLKGEN2 Bank:0x100A
//#########################################################################
#define TSP_CLKGEN2_REG(addr)       (*((volatile MS_U16*)(_u32RegBase + 0x1400 + ((addr)<<2))))
    #define REG_CLKGEN2_DC0_SYTNTH                  0x4A
        #define REG_CLKGEN2_STC2_CW_SEL             0x0002
        #define REG_CLKGEN2_STC2_CW_EN              0x0004
        #define REG_CLKGEN2_STC3_CW_SEL             0x0200
        #define REG_CLKGEN2_STC3_CW_EN              0x0400

    #define REG_CLKGEN2_DC0_STC2_CW_L               0x4B
    #define REG_CLKGEN2_DC0_STC2_CW_H               0x4C
    #define REG_CLKGEN2_DC0_STC3_CW_L               0x4D
    #define REG_CLKGEN2_DC0_STC3_CW_H               0x4E

    //get STC2/3 Clk Gen bank
    #define REG_CLKGEN2_STC2_CLK                    0x4F
        #define REG_CLKGEN2_STC2_MASK               0x000F
        #define REG_CLKGEN2_STC2_SHIFT              0
    #define REG_CLKGEN2_STC3_CLK                    0x4F
        #define REG_CLKGEN2_STC3_MASK               0x00F0
        #define REG_CLKGEN2_STC3_SHIFT              4
        #define REG_CLKGEN2_STC_DISABLE             0x0001
        #define REG_CLKGEN2_STC_INVERT              0x0002
        //SRC
        #define REG_CLKGEN2_STC_SRC_SHIFT           2
        #define REG_CLKGEN2_STC_SRC_MASK            0x0003
        #define REG_CLKGEN2_STC_SRC_SYNTH           0x0000
        #define REG_CLKGEN2_STC_SRC_ONE             0x0001
        #define REG_CLKGEN2_STC_SRC_27M             0x0002
        #define REG_CLKGEN2_STC_SRC_XTAL            0x0003

//#########################################################################
//#### CHIPTOP Bank:0x101E
//#########################################################################
#define TSP_TOP_REG(addr)           (*((volatile MS_U16*)(_u32RegBase + 0x3c00UL + ((addr)<<2))))
    #define REG_TOP_TS0_MUX                         0x38
        #define REG_TOP_TS0_SHIFT                   0x0
    #define REG_TOP_TS1_MUX                         0x38
        #define REG_TOP_TS1_SHIFT                   0x4
    #define REG_TOP_TS2_MUX                         0x38
        #define REG_TOP_TS2_SHIFT                   0x8
    #define REG_TOP_TS3_MUX                         0x38
        #define REG_TOP_TS3_SHIFT                   0xC


    #define REG_TOP_TSO0_MUX                        0x3A
        #define REG_TOP_TSO0_SHIFT                  0

        #define REG_TOP_TS_SRC_MASK                     0x000F
        #define REG_TOP_TS_SRC_EXT0                     0x0000
        #define REG_TOP_TS_SRC_EXT1                     0x0001
        #define REG_TOP_TS_SRC_EXT2                     0x0002
        #define REG_TOP_TS_SRC_EXT3                     0x0003
        #define REG_TOP_TS_SRC_EXT4                     0x0004
        #define REG_TOP_TS_SRC_EXT5                     0x0005
        #define REG_TOP_TS_SRC_EXT6                     0x0006
        #define REG_TOP_TS_SRC_TSO0                     0x0007
        #define REG_TOP_TS_SRC_TSIO0                    0x0008
        //@NOTE Not support internal demod in KANO
        #define REG_TOP_TS_SRC_DMD0                     0x0008

    #define REG_TOP_TSO4_5_MUX                      0x3B
        #define REG_TOP_TSO4_SHIFT                  0
        #define REG_TOP_TSO4_MASK                   0x0003
        #define REG_TOP_TSO5_SHIFT                  4
        #define REG_TOP_TSO5_MASK                   0x0003

    #define REG_TOP_TS_PADMUX_MODE                  0x02
        #define REG_TOP_TS0MODE_MASK                0x1
        #define REG_TOP_TS0MODE_SHIFT               0
            #define REG_TOP_TS0MODE_PARALLEL        1
        #define REG_TOP_TS1MODE_MASK                0x3
        #define REG_TOP_TS1MODE_SHIFT               1
            #define REG_TOP_TS1MODE_INPUT           1
        #define REG_TOP_TS2MODE_MASK                0x3
        #define REG_TOP_TS2MODE_SHIFT               3
            #define REG_TOP_TS2MODE_PARALLEL        1
            #define REG_TOP_TS2MODE_4WIRED          2
            #define REG_TOP_TS2MODE_3WIRED          3

    #define REG_TOP_TS_OUTPUT_MODE                  0x07
        #define REG_TOP_TS_OUT_MODE_MASK            0x3
        #define REG_TOP_TS_OUT_MODE_SHIFT           14
            #define REG_TOP_TS_OUT_MODE_TSO         1
            #define REG_TOP_TS_OUT_MODE_S2P         2
            #define REG_TOP_TS_OUT_MODE_S2P1        3

    #define REG_TOP_TSP_BOOT_CLK_SEL                0x54
        #define REG_TOP_TSP_BOOT_CLK_SEL_MASK       0x0100
        #define REG_TOP_TSP_BOOT_CLK_SEL_TSP        0x0000

    #define REG_TOP_TSP_3WIRE_MODE                  0x11
        #define REG_TOP_TSP_TS0_3WIRE_EN            0x01
        #define REG_TOP_TSP_TS1_3WIRE_EN            0x02

    #define REG_TOP_TSP_3WIRE_MODE1                 0x7b
        #define REG_TOP_TSP_TS2_3WIRE_EN            0x01
        #define REG_TOP_TSP_TS3_3WIRE_EN            0x02



#define TSP_MMFI_REG(addr)       (*((volatile MS_U16*)(_u32RegBase + 0x27E00 + ((addr)<<2))))
    #define REG_MMFI_TSP_SEL_SRAM                   0x70
        #define REG_MMFI_TSP_SEL_SRAM_EN            0x0002

#define TSP_TSO_REG(addr)        (*((volatile MS_U16*)(_u32RegBase + 0xE0C00 + ((addr)<<2))))
    #define REG_TSO_TSP_CONFIG0                     0x1C
        #define REG_TSO_TSP_S2P_MASK                0x001F
        #define REG_TSO_TSP_S2P_EN                  0x0001
        #define REG_TSO_TSP_S2P_TS_SIN_C0           0x0002
        #define REG_TSO_TSP_S2P_TS_SIN_C1           0x0004
        #define REG_TSO_TSP_S2P_3WIRE               0x0008
        #define REG_TSO_TSP_BYPASS_S2P              0x0010

        #define REG_TSO_TSP_S2P1_MASK               0x1F00
        #define REG_TSO_TSP_S2P1_EN                 0x0100
        #define REG_TSO_TSP_S2P1_TS_SIN_C0          0x0200
        #define REG_TSO_TSP_S2P1_TS_SIN_C1          0x0400
        #define REG_TSO_TSP_S2P1_3WIRE              0x0800
        #define REG_TSO_TSP_BYPASS_S2P1             0x1000


typedef struct _REG_SecFlt
{
    TSP32                           Ctrl;
    // Software Usage Flags
    #define TSP_SECFLT_USER_MASK                    0x00000007
    #define TSP_SECFLT_USER_SHFT                    0
    #define TSP_SECFLT_USER_NULL                    0x0
    #define TSP_SECFLT_USER_SEC                     0x1
    #define TSP_SECFLT_USER_PES                     0x2
    #define TSP_SECFLT_USER_PKT                     0x3
    #define TSP_SECFLT_USER_PCR                     0x4
    #define TSP_SECFLT_USER_TTX                     0x5
/*
    #define TSP_SECFLT_USER_EMM                     0x6
    #define TSP_SECFLT_USER_ECM                     0x7
    #define TSP_SECFLT_USER_OAD                     0x8
 */

    #define TSP_SEC_MATCH_INV                       0x00000008 // HW

    // for
    //     TSP_SECFLT_TYPE_SEC
    //     TSP_SECFLT_TYPE_PES
    //     TSP_SECFLT_TYPE_PKT
    //     TSP_SECFLT_TYPE_TTX
    //     TSP_SECFLT_TYPE_OAD
    #define TSP_SECFLT_MODE_MASK                    0x00000030          // software implementation
    #define TSP_SECFLT_MODE_SHFT                    4
    #define TSP_SECFLT_MODE_CONTI                   0x0                 // SEC
    #define TSP_SECFLT_MODE_ONESHOT                 0x1
    #define TSP_SECFLT_MODE_CRCCHK                  0x2
    // for TSP_SECFLT_TYPE_PCR
    #define TSP_SECFLT_PCRRST                       0x00000010          //[OBSOLETED] PCR


    //[NOTE] update section filter
    // It's not recommended for user updating section filter control register
    // when filter is enable. There may be race condition.
    #define TSP_SECFLT_STATE_MASK                   0x000000C0          // software implementation
    #define TSP_SECFLT_STATE_SHFT                   6
    #define TSP_SECFLT_STATE_OVERFLOW               0x1
    #define TSP_SECFLT_STATE_DISABLE                0x2

    #define TSP_SECFLT_BEMASK                       0x0000FF00          //[Reserved]


    // for
    //     TSP_SECFLT_SEL_BUF
    #define TSP_SECFLT_SECBUF_MASK                  0xFF000000          // [31:26] secbuf id
    #define TSP_SECFLT_SECBUF_SHFT                  24
    #define TSP_SECFLT_SECBUF_MAX                   0xFF                // software usage

    TSP32                           Match[TSP_FILTER_DEPTH/sizeof(TSP32)];
    TSP32                           Mask[TSP_FILTER_DEPTH/sizeof(TSP32)];
/*
    TSP32                           BufStart;
    TSP32                           BufEnd;
    TSP32                           BufRead;
    TSP32                           BufWrite;
    TSP32                           BufCur;
*/
    TSP32                           _x24[(0x38-0x24)/sizeof(TSP32)];    // (0x00211024-0x0021103B)/4

    TSP32                           RmnCnt;
    #define TSP_SECFLT_ALLOC_MASK                   0x80000000
    #define TSP_SECFLT_ALLOC_SHFT                   31
    #define TSP_SECFLT_OWNER_MASK                   0x70000000
    #define TSP_SECFLT_OWNER_SHFT                   24

    #define TSP_SECFLT_MODE_AUTO_CRCCHK             0x00100000 //sec flt mode bits are not enough, arbitrarily occupy here

    #define TSP_SECBUF_RMNCNT_MASK                  0x0000FFFF                                      // TS/PES length
    #define TSP_SECBUF_RMNCNT_SHFT                  0

/*
    // for
    //     TSP_SECFLT_TYPE_ECM
    #define TSP_SECFLT_ECM_IDX_SHFT                 16
    #define TSP_SECFLT_ECM_IDX_MASK                 0x00070000
    #define TSP_SECFLT_ECM_IDX_NULL                 0x00000007                                      // only alow 0 .. 5
 */

    TSP32                           CRC32;
    TSP32                           NMask[TSP_FILTER_DEPTH/sizeof(MS_U32)];
    TSP32                           _x50[(0x80-0x50)/sizeof(TSP32)];    // (0x00211050-0x0021107F)/4
} REG_SecFlt;

typedef struct _REG_SecBuf
{
    TSP32                           Start;
    #define TSP_SECBUF_START_MASK                   0x1FFFFFF0 //section buffers of kaiser and keltic are "4" bits aligment
    #define TSP_SECBUF_OWNER_MASK                   0x60000000
    #define TSP_SECBUF_OWNER_SHFT                   29
    #define TSP_SECBUF_ALLOC_MASK                   0x80000000
    #define TSP_SECBUF_ALLOC_SHFT                   31
    TSP32                           End;
    TSP32                           Read;
    TSP32                           Write;
    TSP32                           Cur;
    TSP32                           _x38[(0xA4-0x38)/sizeof(TSP32)];    // (0x0021103C-0x002110A4)/4
} REG_SecBuf;

typedef struct _REG_Pid
{                                                                       // CPU(byte)    RIU(index)  MIPS(0x1500/2+RIU)*4
    REG_PidFlt                      Flt[TSP_PIDFLT_NUM];                // 0x00210000-0x00210007C
} REG_Pid;

typedef struct _REG_Sec
{                                                                       // CPU(byte)    RIU(index)  MIPS(0x1500/2+RIU)*4
    REG_SecFlt                      Flt[TSP_SECFLT_NUM];
} REG_Sec;


typedef struct _REG_Buf
{
    REG_SecBuf                      Buf[TSP_SECFLT_NUM];
} REG_Buf;


//@NOTE TSP 0~1
typedef struct _REG_Ctrl
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
    // Type                         Name                                Index(word)     CPU(byte)     MIPS(0x1500/2+index)*4
    REG16                           _xbf202a00;                              // 0xbf802a00   0x00
    REG32                           Str2mi_head2pvr1;                        // 0xbf802a04   0x01
    #define TSP_HW_PVR1_BUF_HEAD2_MASK              0x0FFFFFFF

    REG32                           Str2mi_mid2pvr1;                         // 0xbf802a0c   0x03 ,wptr & mid share same register
    #define TSP_HW_PVR1_BUF_MID2_MASK               0x0FFFFFFF

    REG32                           Str2mi_tail2pvr1;                        // 0xbf802a14   0x05
    #define TSP_HW_PVR1_BUF_TAIL2_MASK              0x0FFFFFFF

    REG32                           Pcr_L;                                   // 0xbf802a1c  0x07
    #define TSP_PCR64_L32_MASK                      0xFFFFFFFF

    REG32                           Pcr_H;                                   // 0xbf802a24  0x09
    #define TSP_PCR64_H32_MASK                      0xFFFFFFFF               // PCR64 Middle 64

    REG16                           Mobf_Filein_Idx;                         // 0xbf802a2c   0x0b
    #define TSP_MOBF_FILEIN_MASK                    0x0000001F

    REG32                           _xbf202a2c;                              // 0xbf802a30   0x0c

    REG32                           PVR2_Config;                             // 0xbf802a38   0x0e
    #define TSP_PVR2_LPCR1_WLD                      0x00000001
    #define TSP_PVR2_LPCR1_RLD                      0x00000002
    #define TSP_PVR2_STR2MIU_DSWAP                  0x00000004
    #define TSP_PVR2_STR2MIU_EN                     0x00000008
    #define TSP_PVR2_STR2MIU_RST_WADR               0x00000010
    #define TSP_PVR2_STR2MIU_BT_ORDER               0x00000020
    #define TSP_PVR2_STR2MIU_PAUSE                  0x00000040
    #define TSP_PVR2_REG_PINGPONG_EN                0x00000080
    #define TSP_PVR2_PVR_ALIGN_EN                   0x00000100
    #define TSP_PVR2_DMA_FLUSH_EN                   0x00000200
    #define TSP_PVR2_PKT192_EN                      0x00000400
    #define TSP_PVR2_BURST_LEN_MASK                 0x00001800
    #define TSP_PVR2_BURST_LEN_SHIFT                11
    #define TSP_REC_DATA2_INV                       0x00002000
    #define TSP_V_BLOCK_DIS                         0x00004000
    #define TSP_V3d_BLOCK_DIS                       0x00008000
    #define TSP_A_BLOCK_DIS                         0x00010000
    #define TSP_AD_BLOCK_DIS                        0x00020000
    #define TSP_PVR1_BLOCK_DIS                      0x00040000
    #define TSP_PVR2_BLOCK_DIS                      0x00080000
    #define TSP_TS_IF2_EN                           0x00100000
    #define TSP_TS_DATA2_SWAP                       0x00200000
    #define TSP_P_SEL2                              0x00400000
    #define TSP_EXT_SYNC_SEL2                       0x00800000
    #define TSP_BYPASS_TSIF2                        0x01000000
    #define TSP_TEI_SKIP_PKT2                       0x02000000
    #define TSP_AC_BLOCK_DIS                        0x04000000
    #define TSP_ADD_BLOCK_DIS                       0x08000000
    #define TSP_CLR_LOCKED_PKT_CNT                  0x20000000
    #define TSP_CLR_PKT_CNT                         0x40000000
    #define TSP_CLR_PVR_OVERFLOW                    0x80000000

    REG32                           PVR2_LPCR1;                             // 0xbf802a40   0x10

    #define TSP_STR2MI2_ADDR_MASK  0x0FFFFFFF
    REG32                           Str2mi_head1_pvr2;                      // 0xbf802a48   0x12
    REG32                           Str2mi_mid1_wptr_pvr2;                  // 0xbf802a50   0x14
    REG32                           Str2mi_tail1_pvr2;                      // 0xbf802a58   0x16
    REG32                           Str2mi_head2_pvr2;                      // 0xbf802a60   0x18
    REG32                           Str2mi_mid2_pvr2;                       // 0xbf802a68   0x1a, PVR2 mid address & write point
    REG32                           Str2mi_tail2_pvr2;                      // 0xbf802a70   0x1c
    REG32                           Hw_SyncByte2;                           // 0xbf802a78   0x1e
    #define TSP_HW_CFG2_PACKET_SYNCBYTE2_MASK       0x000000FF
    #define TSP_HW_CFG2_PACKET_SYNCBYTE2_SHFT       0
    #define TSP_HW_CFG2_PACKET_SIZE2_MASK           0x0000FF00
    #define TSP_HW_CFG2_PACKET_SIZE2_SHFT           8
    #define TSP_HW_CFG2_PACKET_CHK_SIZE2_MASK       0x00FF0000
    #define TSP_HW_CFG2_PACKET_CHK_SIZE2_SHFT       16

    REG32                           Pkt_CacheW0;                            // 0xbf802a80   0x20

    REG32                           Pkt_CacheW1;                            // 0xbf802a88   0x22

    REG32                           Pkt_CacheW2;                            // 0xbf802a90   0x24

    REG32                           Pkt_CacheW3;                            // 0xbf802a98   0x26

    REG32                           Pkt_CacheIdx;                           // 0xbf802aa0   0x28

    REG32                           Pkt_DMA;                                // 0xbf802aa8   0x2a
    #define TSP_SEC_DMAFIL_NUM_MASK                 0x000000FF
    #define TSP_SEC_DMAFIL_NUM_SHIFT                0
    #define TSP_SEC_DMASRC_OFFSET_MASK              0x0000FF00
    #define TSP_SEC_DMASRC_OFFSET_SHIFT             8
    #define TSP_SEC_DMADES_LEN_MASK                 0x00FF0000
    #define TSP_SEC_DMADES_LEN_SHIFT                16

    REG16                           Hw_Config0;                             // 0xbf802ab0   0x2c
    #define TSP_HW_CFG0_DATA_PORT_SEL               0x0001                  //TSIF0 data port output select. 0: select live TS to be TSIF output 1: select data port to be TSIF output
    #define TSP_HW_CFG0_TSIFO_SERL                  0x0000
    #define TSP_HW_CFG0_TSIF0_PARL                  0x0002
    #define TSP_HW_CFG0_TSIF0_EXTSYNC               0x0004
    #define TSP_HW_CFG0_TSIF0_TS_BYPASS             0x0008
    #define TSP_HW_CFG0_TSIF0_VPID_BYPASS           0x0010
    #define TSP_HW_CFG0_TSIF0_APID_BYPASS           0x0020
    #define TSP_HW_CFG0_WB_DMA_RESET                0x0040
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_MASK        0xFF00
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_SHIFT       8

    REG16                           Hw_PktSize0;                             // 0xbf802ab4   0x2d
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_MASK    0x00FF
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_SHIFT   0
    #define TSP_HW_CFG0_PACKET_CHK_SIZE_MASK        0xFF00
    #define TSP_HW_CFG0_PACKET_CHK_SIZE_SHFT        8

    REG16                           STC_Config;                             // 0xbf802ab8   0x2e
    #define TSP_STC_CFG_SET_TIME_BASE_64b_3         0x0001
    #define TSP_STC_CFG_CNT64b_3_EN                 0x0002
    #define TSP_STC_CFG_CNT64b_3_LD                 0x0004
    #define TSP_STC_CFG_SET_TIME_BASE_64b_4         0x0010
    #define TSP_STC_CFG_CNT64b_4_EN                 0x0020
    #define TSP_STC_CFG_CNT64b_4_LD                 0x0040

    REG16                           TSP_DBG_PORT;                           // 0xbf802ab8   0x2f
    #define TSP_DNG_DATA_PORT_MASK                  0x00FF
    #define TSP_DNG_DATA_PORT_SHIFT                 0

    REG32                           Pcr_L_CmdQ;                             // 0xbf802ac0   0x30
    REG16                           Pcr_H_CmdQ;                             // 0xbf802ac8   0x32
    #define TSP_REG_PCR_CMDQ_H                      0x0001

    REG16                           Vd_Pid_Hit;                             // 0xbf802acc   0x33
    #define TSP_VPID_MASK                           0x1FFF

    REG16                           Aud_Pid_Hit;                            // 0xbf802ad0   0x34
    #define TSP_APID_MASK                           0x1FFF

    REG16                           Pkt_Info;                               // 0xbf802ad4   0x35
    #define TSP_PKT_PID_8_12_CP_MASK                0x001F
    #define TSP_PKT_PID_8_12_CP_SHIFT               0
    #define TSP_PKT_PRI_MASK                        0x0020
    #define TSP_PKT_PRI_SHIFT                       5
    #define TSP_PKT_PLST_MASK                       0x0040
    #define TSP_PKT_PLST_SHIFT                      6
    #define TSP_PKT_ERR                             0x0080
    #define TSP_PKT_ERR_SHIFT                       7

    REG16                           Pkt_Info2;                              // 0xbf802ad8   0x36
    #define TSP_PKT_INFO_CC_MASK                    0x000F
    #define TSP_PKT_INFO_CC_SHFT                    0
    #define TSP_PKT_INFO_ADPCNTL_MASK               0x0030
    #define TSP_PKT_INFO_ADPCNTL_SHFT               4
    #define TSP_PKT_INFO_SCMB                       0x00C0
    #define TSP_PKT_INFO_SCMB_SHFT                  6
    #define TSP_PKT_PID_0_7_CP_MASK                 0xFF00
    #define TSP_PKT_PID_0_7_CP_SHIFT                8

    REG16                           AVFifoSts;                              // 0xbf802adc   0x37
    #define TSP_VFIFO3D_EMPTY                       0x0001
    #define TSP_VFIFO3D_EMPTY_SHFT                  0
    #define TSP_VFIFO3D_FULL                        0x0002
    #define TSP_VFIFO3D_FULL_SHFT                   1
    #define TSP_VFIFO3D_LEVEL                       0x000C
    #define TSP_VFIFO3D_LEVEL_SHFT                  2
    #define TSP_VFIFO_EMPTY                         0x0010
    #define TSP_VFIFO_EMPTY_SHFT                    4
    #define TSP_VFIFO_FULL                          0x0020
    #define TSP_VFIFO_FULL_SHFT                     5
    #define TSP_VFIFO_LEVEL                         0x00C0
    #define TSP_VFIFO_LEVEL_SHFT                    6
    #define TSP_AFIFO_EMPTY                         0x0100
    #define TSP_AFIFO_EMPTY_SHFT                    8
    #define TSP_AFIFO_FULL                          0x0200
    #define TSP_AFIFO_FULL_SHFT                     9
    #define TSP_AFIFO_LEVEL                         0x0C00
    #define TSP_AFIFO_LEVEL_SHFT                    10
    #define TSP_AFIFOB_EMPTY                        0x1000
    #define TSP_AFIFOB_EMPTY_SHFT                   12
    #define TSP_AFIFOB_FULL                         0x2000
    #define TSP_AFIFOB_FULL_SHFT                    13
    #define TSP_AFIFOB_LEVEL                        0xC000
    #define TSP_AFIFOB_LEVEL_SHFT                   14

    REG32                           SwInt_Stat;                             // 0xbf802ae0   0x38
    #define TSP_SWINT_INFO_SEC_MASK                 0x000000FF
    #define TSP_SWINT_INFO_SEC_SHFT                 0
    #define TSP_SWINT_INFO_ENG_MASK                 0x0000FF00
    #define TSP_SWINT_INFO_ENG_SHFT                 8
    #define TSP_SWINT_STATUS_CMD_MASK               0x7FFF0000
    #define TSP_SWINT_STATUS_CMD_SHFT               16
    #define TSP_SWINT_STATUS_SEC_RDY                0x0001
    #define TSP_SWINT_STATUS_REQ_RDY                0x0002
    #define TSP_SWINT_STATUS_SEC_RDY_CRCERR         0x0003
    #define TSP_SWINT_STATUS_BUF_OVFLOW             0x0006
    #define TSP_SWINT_STATUS_SEC_CRCERR             0x0007
    #define TSP_SWINT_STATUS_SEC_ERROR              0x0008
    #define TSP_SWINT_STATUS_SYNC_LOST              0x0010
    #define TSP_SWINT_STATUS_PKT_OVRUN              0x0020
    #define TSP_SWINT_STATUS_DEBUG                  0x0030
    #define TSP_SWINT_CMD_DMA_PAUSE                 0x0100
    #define TSP_SWINT_CMD_DMA_RESUME                0x0200
    #define TSP_SWINT_STATUS_SEC_GROUP              0x000F
    #define TSP_SWINT_STATUS_GROUP                  0x00FF
    #define TSP_SWINT_CMD_GROUP                     0x7F00
    #define TSP_SWINT_CMD_STC_UPD                   0x0400
    #define TSP_SWINT_CTRL_FIRE                     0x80000000

    REG32                           TsDma_Addr;                             // 0xbf802ae8   0x3a

    REG32                           TsDma_Size;                             // 0xbf802af0   0x3c

    REG16                           TsDma_Ctrl;                             // 0xbf802af8   0x3e
    #define TSP_TSDMA_CTRL_START                    0x0001
    #define TSP_TSDMA_FILEIN_DONE                   0x0002
    #define TSP_TSDMA_INIT_TRUST                    0x0004
    #define TSP_TSDMA_STAT_ABORT                    0x0080

    REG16                           TsDma_mdQ;                          // 0xbf802af8   0x3f
    #define TSP_CMDQ_CNT_MASK                       0x001F
    #define TSP_CMDQ_CNT_SHFT                       0
    #define TSP_CMDQ_FULL                           0x0040
    #define TSP_CMDQ_EMPTY                          0x0080
    #define TSP_CMDQ_SIZE                           16
    #define TSP_CMDQ_WR_LEVEL_MASK                  0x0300
    #define TSP_CMDQ_WR_LEVEL_SHFT                  8

    REG32                           MCU_Cmd;                            // 0xbf802b00   0x40
    #define TSP_MCU_CMD_MASK                        0x0000FFFF
    #define TSP_MCU_CMD_NULL                        0x00000000
    #define TSP_MCU_CMD_READ                        0x00000001
    #define TSP_MCU_CMD_WRITE                       0x00000002
    #define TSP_MCU_CMD_ALIVE                       0x00000100
    #define TSP_MCU_CMD_DBG                         0x00000200
    #define TSP_MCU_CMD_BUFRST                      0x00000400
    #define TSP_MCU_CMD_SECRDYINT_DISABLE           0x00000800
    #define TSP_MCU_CMD_SEC_CC_CHECK_DISABLE        0x00001000
    #define TSP_MCU_CMD_INFO                        0x00008000
        #define INFO_FW_VERSION                         0x0001
        #define INFO_FW_DATE                            0x0002

    REG16                           PktSize1;                          // 0xbf802b08   0x42
    #define TSP_HW_CFG2_PACKET_CHK_SIZE1_MASK       0x00FF
    #define TSP_HW_CFG2_PACKET_CHK_SIZE1_SHFT       0
    #define TSP_HW_CFG2_PACKET_SYNCBYTE1_MASK       0xFF00
    #define TSP_HW_CFG2_PACKET_SYNCBYTE1_SHFT       8

    REG16                           Hw_Config2;                         // 0xbf802b0C   0x43
    #define TSP_HW_CFG2_PACKET_SIZE1_MASK           0x00FF
    #define TSP_HW_CFG2_PACKET_SIZE1_SHFT           0
    #define TSP_HW_CFG2_TSIF1_SERL                  0x0000
    #define TSP_HW_CFG2_TSIF1_PARL                  0x0100
    #define TSP_HW_CFG2_TSIF1_EXTSYNC               0x0200
    #define TSP_HW_CFG2_TSIF1_TS_BYPASS             0x1000

    REG16                           Hw_PVRCfg;                          // 0xbf802b10   0x44
    #define TSP_HW_CFG4_SECDMA_PRI_HIGH             0x0001
    #define TSP_HW_CFG4_PVR_ENABLE                  0x0002
    #define TSP_HW_CFG4_PVR_ENDIAN_BIG              0x0004              // 1: record TS to MIU with big endian, 0: record TS to MIU with little endian
    #define TSP_HW_CFG4_TSIF1_ENABLE                0x0008              // 1: enable ts interface 1 and vice versa
    #define TSP_HW_CFG4_PVR_FLUSH                   0x0010              // 1: str2mi_wadr <- str2mi_miu_head
    #define TSP_HW_CFG4_PVRBUF_BYTEORDER_BIG        0x0020              // Byte order of 8-byte recoding buffer to MIU.
    #define TSP_HW_CFG4_PVR_PAUSE                   0x0040
    #define TSP_HW_CFG4_MEMTSDATA_ENDIAN_BIG        0x0080              // 32-bit data byte order read from 8x64 FIFO when playing file.
    #define TSP_HW_CFG4_TSIF0_ENABLE                0x0100              // 1: enable ts interface 0 and vice versa
    #define TSP_SYNC_RISING_DETECT                  0x0200              // Reset bit count when data valid signal of TS interface is low.
    #define TSP_VALID_FALLING_DETECT                0x0400              // Reset bit count on the rising sync signal of TS interface.
    #define TSP_HW_CFG4_TS_DATA0_SWAP               0x0800              // Set 1 to swap the bit order of TS0 DATA bus
    #define TSP_HW_CFG4_TS_DATA1_SWAP               0x1000              // Set 1 to swap the bit order of TS1 DATA bus
    #define TSP_HW_TSP2OUTAEON_INT_EN               0x4000              // Set 1 to force interrupt to outside AEON
    #define TSP_HW_HK_INT_FORCE                     0x8000              // Set 1 to force interrupt to HK_MCU

    REG16                           Hw_Config4;                         // 0xbf802b14   0x45
    #define TSP_HW_CFG4_ALT_TS_SIZE                 0x0001              // enable TS packets in 204 mode
    #define TSP_HW_CFG4_PS_AUDC_EN                  0x0002              // program stream audiodC enable
    #define TSP_HW_CFG4_BYTE_ADDR_DMA               0x000D              // prevent from byte enable bug, bit1~3 must enable togather
    #define TSP_HW_DMA_MODE_MASK                    0x0030              // Section filter DMA mode, 2'b00: Single.2'b01: Burst 2 bytes.2'b10: Burst 4 bytes.2'b11: Burst 8 bytes.
    #define TSP_HW_DMA_MODE_SHIFT                   4
    #define TSP_HW_CFG4_WSTAT_CH_EN                 0x0040
    #define TSP_HW_CFG4_PS_VID_EN                   0x0080              // program stream video enable
    #define TSP_HW_CFG4_PS_AUD_EN                   0x0100              // program stream audio enable
    #define TSP_HW_CFG4_PS_AUDB_EN                  0x0200              // program stream audioB enable
    #define TSP_HW_CFG4_APES_ERR_RM_EN              0x0400              // Set 1 to enable removing APES error packet
    #define TSP_HW_CFG4_VPES_ERR_RM_EN              0x0800              // Set 1 to enable removing VPES error packet
    #define TSP_HW_CFG4_SEC_ERR_RM_EN               0x1000              // Set 1 to enable removing section error packet
    #define TSP_HW_CFG4_PS_AUDD_EN                  0x2000              // program stream audioD enable
    #define TSP_HW_CFG4_DATA_CHK_2T                 0x8000              // Set 1 to enable the patch of internal sync in "tsif"

    REG32                           NOEA_PC;                            // 0xbf802b18   0x46

    REG16                           Idr_Ctrl;                           // 0xbf802b20   0x48
    #define TSP_IDR_START                           0x0001
    #define TSP_IDR_READ                            0x0000
    #define TSP_IDR_WRITE                           0x0002
    #define TSP_IDR_WR_ENDIAN_BIG                   0x0004
    #define TSP_IDR_WR_ADDR_AUTO_INC                0x0008              // Set 1 to enable address auto-increment after finishing read/write
    #define TSP_IDR_WDAT0_TRIG_EN                   0x0010              // WDAT0_TRIG_EN
    #define TSP_IDR_MCUWAIT                         0x0020
    #define TSP_IDR_SOFT_RST                        0x0080              // Set 1 to soft-reset the IND32 module
    #define TSP_IDR_AUTO_INC_VAL_MASK               0x0F00
    #define TSP_IDR_AUTO_INC_VAL_SHIFT              8

    REG32                           Idr_Addr;                           // 0xbf802b24   0x49
    REG32                           Idr_Write;                          // 0xbf802b2c   0x4b
    REG32                           Idr_Read;                           // 0xbf802b34   0x4d

    REG16                           Fifo_Status;                        // 0xbf802b3c   0x4f
    #define TSP_V3D_FIFO_DISCON                     0x0010
    #define TSP_V3D_FIFO_OVERFLOW                   0x0020
    #define TSP_VD_FIFO_DISCON                      0x0200
    #define TSP_VD_FIFO_OVERFLOW                    0x0800
    #define TSP_AUB_FIFO_OVERFLOW                   0x1000
    #define TSP_AU_FIFO_OVERFLOW                    0x2000

    // only 25 bits supported in PVR address. 8 bytes address
    #define TSP_STR2MI2_ADDR_MASK                   0x0FFFFFFF
    REG32                           TsRec_Head;                         // 0xbf802b40   0x50
    REG32                           TsRec_Mid_PVR1_WPTR;                // 0xbf802b48   0x52, PVR1 mid address & write point
    REG32                           TsRec_Tail;                         // 0xbf802b50   0x54
    REG32                           _xbf802b58[2];                      // 0xbf802b58 ~ 0xbf802b60   0x56~0x59

    REG16                           reg15b4;                            // 0xbf802b68   0x5a
    #define TSP_VQ_DMAW_PROTECT_EN                  0x0001
    #define TSP_SEC_CB_PVR2_DAMW_PROTECT_EN         0x0002
    #define TSP_PVR_PID_BYPASS                      0x0008              // Set 1 to bypass PID in record
    #define TSP_PVR_PID_BYPASS2                     0x0010              // Set 1 to bypass PID in record2
    #define TSP_BD_AUD_EN                           0x0020              // Set 1 to enable the BD audio stream recognization ( core /extend audio stream)
    #define TSP_BD2_AUD_EN                          0x0200              // Set 1 to enable the BD audio stream recognization ( core /extend audio stream)
    #define TSP_AVFIFO_RD_EN                        0x0080              // 0: AFIFO and VFIFO read are connected to MVD and MAD,  1: AFIFO and VFIFO read are controlled by registers (0x15B5[2:0])
    #define TSP_AVFIFO_RD                           0x0100              // If AVFIFO_RD_EN is 1, set to 1, then set to 0 would issue a read strobe to AFIFO or VFIFO
    #define TSP_NMATCH_DISABLE                      0x0800
    #define TSP_PVR_INVERT                          0x1000              // Set 1 to enable data payload invert for PVR record
    #define TSP_PLY_FILE_INV_EN                     0x2000              // Set 1 to enable data payload invert in pidflt0 file path
    #define TSP_PLY_TS_INV_EN                       0x4000              // Set 1 to enable data payload invert in pidflt0 TS path
    #define TSP_FILEIN_BYTETIMER_ENABLE             0x8000              // Set 1 to enable byte timer in ts_if0 TS path

    REG16                           reg15b8;                            // 0xbf802b6C   0x5b
    #define TSP_PVR1_PINGPONG                       0x0001              // Set 1 to enable MIU addresses with pinpon mode
    #define TSP_VQ_STATUS_SEL                       0x0002
    #define TSP_TEI_SKIPE_PKT_PID0                  0x0004              // Set 1 to skip error packets in pidflt0 TS path
    #define TSP_TEI_SKIPE_PKT_PID4                  0x0008              // Set 1 to skip error packets in pidflt4 TS path
    #define TSP_TEI_SKIPE_PKT_PID1                  0x0010              // Set 1 to skip error packets in pidflt1 TS path
    #define TSP_TEI_SKIPE_PKT_PID3                  0x0020              // Set 1 to skip error packets in pidflt3 TS path
    #define TSP_REMOVE_DUP_AV_PKT                   0x0040              // Set 1 to remove duplicate A/V packet
    #define TSP_64bit_PCR2_ld                       0x0080
    #define TSP_cnt_33b_ld                          0x0100
    #define TSP_FORCE_SYNCBYTE                      0x0200              // Set 1 to force sync byte (8'h47) in ts_if0 and ts_if1 path.
    #define TSP_SERIAL_EXT_SYNC_1T                  0x0400              // Set 1 to detect serial-in sync without 8-cycle mode
    #define TSP_BURST_LEN_MASK                      0x1800              // 00,01:    burst length = 4; 10,11: burst length = 1
    #define TSP_BURST_LEN_SHIFT                     11
    #define TSP_MATCH_PID_SRC_MASK                  0xE000              // Select the source of pid filter number with hit pid and match pid number with scramble information, 00 : from pkt_demux0, 01 : from pkt_demux_file, 10 : from pkt_demux1, 11 : from pkt_demux2
    #define TSP_MATCH_PID_SRC_SHIFT                 13
    #define TSP_MATCH_PID_SRC_PKTDMX0               0
    #define TSP_MATCH_PID_SRC_PKTDMX1               1
    #define TSP_MATCH_PID_SRC_PKTDMX2               2
    #define TSP_MATCH_PID_SRC_PKTDMX3               3

    REG32                           TSP_MATCH_PID_NUM;                  // 0xbf802b70   0x5c

    REG32                           TSP_IWB_WAIT;                       // 0xbf802b78   0x5e  // Wait count settings for IWB when TSP CPU i-cache is enabled.

    REG32                           Cpu_Base;                           // 0xbf802b80   0x60
    #define TSP_CPU_BASE_ADDR_MASK                  0x0FFFFFFF

    REG32                           Qmem_Ibase;                         // 0xbf802b88   0x62

    REG32                           Qmem_Imask;                         // 0xbf802b90   0x64

    REG32                           Qmem_Dbase;                         // 0xbf802b98   0x66

    REG32                           Qmem_Dmask;                         // 0xbf802ba0   0x68

    REG32                           TSP_Debug;                          // 0xbf802ba8   0x6a
    #define TSP_DEBUG_MASK                          0x00FFFFFF

    REG32                           _xbf802bb0;                         // 0xbf802bb0   0x6c

    REG32                           TsFileIn_RPtr;                      // 0xbf802bb8   0x6e
    #define TSP_FILE_RPTR_MASK                      0x0FFFFFFF
    REG32                           TsFileIn_Timer;                     // 0xbf802bc0   0x70
    #define TSP_FILE_TIMER_MASK                     0x00FFFFFF
    REG32                           TsFileIn_Head;                      // 0xbf802bc8   0x72
    #define TSP_FILE_ADDR_MASK                      0x0FFFFFFF
    REG32                           TsFileIn_Mid;                       // 0xbf802bd0   0x74

    REG32                           TsFileIn_Tail;                      // 0xbf802bd8   0x76

    REG16                           Dnld_Ctrl_Addr;                     // 0xbf802be0   0x78
    #define TSP_DNLD_ADDR_MASK                      0xFFFF
    #define TSP_DNLD_ADDR_SHFT                      0
    #define TSP_DNLD_ADDR_ALI_SHIFT                 4                   // Bit [11:4] of DMA_RADDR[19:0]

    REG16                           Dnld_Ctrl_Size;                     // 0xbf802be4   0x79
    #define TSP_DNLD_NUM_MASK                       0xFFFF
    #define TSP_DNLD_NUM_SHFT                       0

    REG16                           TSP_Ctrl;                           // 0xbf802be8   0x7a
    #define TSP_CTRL_CPU_EN                         0x0001
    #define TSP_CTRL_SW_RST                         0x0002
    #define TSP_CTRL_DNLD_START                     0x0004
    #define TSP_CTRL_DNLD_DONE                      0x0008              // See 0x78 for related information
    #define TSP_CTRL_TSFILE_EN                      0x0010
    #define TSP_CTRL_R_PRIO                         0x0020
    #define TSP_CTRL_W_PRIO                         0x0040
    #define TSP_CTRL_ICACHE_EN                      0x0100
    #define TSP_CTRL_CPU2MI_R_PRIO                  0x0400
    #define TSP_CTRL_CPU2MI_W_PRIO                  0x0800
    #define TSP_CTRL_I_EL                           0x0000
    #define TSP_CTRL_I_BL                           0x1000
    #define TSP_CTRL_D_EL                           0x0000
    #define TSP_CTRL_D_BL                           0x2000
    #define TSP_CTRL_NOEA_QMEM_ACK_DIS              0x4000
    #define TSP_CTRL_MEM_TS_WORDER                  0x8000

    REG16                           TSP_SyncByte;                       // 0xbf802bec   0x7b
    #define TSP_SYNC_BYTE_MASK                      0x00FF
    #define TSP_SYNC_BYTE_SHIFT                     0

    REG16                           PKT_CNT;                            // 0xbf802bf0   0x7c
    #define TSP_PKT_CNT_MASK                        0x00FF

    REG16                           DBG_SEL;                            // 0xbf802bf4   0x7d
    #define TSP_DBG_SEL_MASK                        0xFFFF
    #define TSP_DBG_SEL_SHIFT                       0

    REG16                           HwInt_Stat;                         // 0xbf802bf8   0x7e
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
    #define TSP_HWINT_EN_MASK                       0x00FF          // Tsp2hk_int enable bits.
    #define TSP_HWINT_EN_SHIFT                      0
    #define TSP_HWINT_TSP_PVR_TAIL0_EN              0x0001          // currently not used in ISR
    #define TSP_HWINT_TSP_PVR_MID0_EN               0x0002          // currently not used in ISR
    #define TSP_HWINT_HW_PVR0_EN_MASK               (TSP_HWINT_TSP_PVR_TAIL0_EN | TSP_HWINT_TSP_PVR_MID0_EN)
    #define TSP_HWINT_TSP_HK_INT_FORCE_EN           0x0004          // currently not used in ISR
    #define TSP_HWINT_TSP_FILEIN_MID_INT_EN         0x0008          // currently not used in ISR
    #define TSP_HWINT_TSP_FILEIN_TAIL_INT_EN        0x0010          // currently not used in ISR
    #define TSP_HWINT_TSP_SW_INT_EN                 0x0020
    #define TSP_HWINT_TSP_DMA_READ_DONE_EN          0x0040          // currently not used in ISR
    #define TSP_HWINT_TSP_AV_PKT_ERR_EN             0x0080          // currently not used in ISR
    #define TSP_HWINT_TSP_SUPPORT_ALL               (TSP_HWINT_TSP_SW_INT_EN)
    #define TSP_HWINT_ALL                           TSP_HWINT_TSP_SUPPORT_ALL

    #define TSP_HWINT_STATUS_MASK                   0xFF00
    #define TSP_HWINT_STATUS_SHIFT                  8
    #define TSP_HWINT_TSP_PVR_TAIL0_STATUS          0x0100
    #define TSP_HWINT_TSP_PVR_MID0_STATUS           0x0200
    #define TSP_HWINT_HW_PVR0_STATUS_MASK           (TSP_HWINT_TSP_PVR_TAIL0_STATUS | TSP_HWINT_TSP_PVR_MID0_STATUS)
    #define TSP_HWINT_TSP_HK_INT_FORCE_STATUS       0x0400
    #define TSP_HWINT_TSP_FILEIN_MID_INT_STATUS     0x0800
    #define TSP_HWINT_TSP_FILEIN_TAIL_INT_STATUS    0x1000
    #define TSP_HWINT_TSP_SW_INT_STATUS             0x2000
    #define TSP_HWINT_TSP_DMA_READ_DONE             0x4000
    #define TSP_HWINT_TSP_AV_PKT_ERR                0x8000

    // following mask is merged with bank 15 7e(LOW BYTE) and bank 16 6c(HIGH BYTE)
    #define TSP_HWINT_HW_PVR_ALL_MASK               (TSP_HWINT_HW_PVR0_STATUS_MASK | TSP_HWINT_HW_PVR1_MASK) //@FIXME this is for all pvr interrupt but PVR 3 and 4 is not added

    REG16                           TSP_Ctrl1;                          // 0xbf802bfc   0x7f
    // 0x7f: TSP_CTRL1: hidden in HwInt_Stat
    #define TSP_CTRL1_FILEIN_TIMER_ENABLE           0x0001
    #define TSP_CTRL1_TSP_FILE_NON_STOP             0x0002              //Set 1 to enable TSP file data read without timer check
    #define TSP_CTRL1_FILEIN_PAUSE                  0x0004              //Set 1 to pause file-in engine fetch data
    #define TSP_CTRL1_FILE_CHECK_WP                 0x0008
    #define TSP_CTRL1_FILE_WP_SEL_MASK              0x0030
    #define TSP_CTRL1_FILE_WP_FI                    0x0010
    #define TSP_CTRL1_FILE_WP_PVR                   0x0020
    #define TSP_CTRL1_STANDBY                       0x0080
    #define TSP_CTRL1_INT2NOEA                      0x0100
    #define TSP_CTRL1_INT2NOEA_FORCE                0x0200
    #define TSP_CTRL1_FORCE_XIU_WRDY                0x0400
    #define TSP_CTRL1_CMDQ_RESET                    0x0800
    #define TSP_CTRL1_DLEND_EN                      0x1000          // Set 1 to enable little-endian mode in TSP CPU
    #define TSP_CTRL1_PVR_CMD_QUEUE_ENABLE          0x2000
    #define TSP_CTRL1_FILEIN_RADDR_READ             0x4000
    #define TSP_CTRL1_DMA_RST                       0x8000

    //----------------------------------------------
    // 0xBF802C00 MIPS direct access
    //----------------------------------------------
    REG32                           MCU_Data0;                          // 0xbf802c00   0x00
    #define TSP_MCU_DATA_ALIVE                      TSP_MCU_CMD_ALIVE

    REG32                           PVR1_LPcr1;                         // 0xbf802c08   0x02

    REG32                           LPcr2;                              // 0xbf802c10   0x04

    REG16                           reg160C;                            // 0xbf802c18   0x06
    #define TSP_PVR1_LPCR1_WLD                      0x0001              // Set 1 to load LPCR1 value (Default: 0)
    #define TSP_PVR1_LPCR1_RLD                      0x0002              // Set 1 to read LPCR1 value (Default: 1)
    #define TSP_LPCR2_WLD                           0x0004              // Set 1 to load LPCR2 value (Default: 0)
    #define TSP_LPCR2_RLD                           0x0008              // Set 1 to read LPCR2 value (Default: 1)
    #define TSP_RECORD192_EN                        0x0010              // 160C bit(5)enable TS packets with 192 bytes on record mode
    #define TSP_FILEIN192_EN                        0x0020              // 160C bit(5)enable TS packets with 192 bytes on file-in mode
    #define TSP_ORZ_DMAW_PROT_EN                    0x0080              // 160C bit(7) open RISC DMA write protection
    #define TSP_CLR_PIDFLT_BYTE_CNT                 0x0100              // Clear pidflt0_file byte counter
    #define TSP_DOUBLE_BUF_DESC                     0x4000              // 160d bit(6) remove buffer limitation, Force pinpong buffer to flush
    #define TSP_TIMESTAMP_RESET                     0x8000              // 160d bit(7) reset timestamp

    REG16                           reg160E;                            // 0xbf802c1C   0x07
    #define TSP_VQTX0_BLOCK_DIS                     0x0001
    #define TSP_VQTX1_BLOCK_DIS                     0x0002
    #define TSP_VQTX2_BLOCK_DIS                     0x0004
    #define TSP_VQTX3_BLOCK_DIS                     0x0008
    #define TSP_DIS_MIU_RQ                          0x0010              // Disable miu R/W request for reset TSP usage
    #define TSP_RM_DMA_GLITCH                       0x0080              // Fix sec_dma overflow glitch
    #define TSP_RESET_VFIFO                         0x0100              // Reset VFIFO -- ECO Done
    #define TSP_RESET_AFIFO                         0x0200              // Reset AFIFO -- ECO Done
    #define TSP_RESET_AFIFO3                        0x0400              // Reset AFIFOC -- ECO Done
    #define TSP_CLR_ALL_FLT_MATCH                   0x0800              // Set 1 to clean all flt_match in a packet
    #define TSP_RESET_AFIFO2                        0x1000
    #define TSP_RESET_VFIFO3D                       0x2000
    #define TSP_PVR_WPRI_HIGH                       0x4000
    #define TSP_OPT_ORACESS_TIMING                  0x8000

    REG16                           PktChkSizeFilein;                   // 0xbf802c20   0x08
    #define TSP_PKT_SIZE_MASK                       0x00ff
    #define TSP_PKT192_BLK_DIS_FIN                  0x0100              // Set 1 to disable file-in timestamp block scheme
    #define TSP_AV_CLR                              0x0200              // Clear AV FIFO overflow flag and in/out counter
    #define TSP_HW_STANDBY_MODE                     0x0400              // Set 1 to disable all SRAM in TSP for low power mode automatically
    #define TSP_RESET_AFIFO4                        0x4000              // Reset AFIFOC -- ECO Done

    REG16                           TSP_Cfg5;                           // 0xbf802c24   0x09
    #define TSP_PREVENT_OVF_META                    0x0001
    #define TSP_OVF_META_SEL                        0x0004
    #define TSP_SYSTIME_MODE                        0x0008
    #define TSP_SEC_DMA_BURST_EN                    0x0080              // Enable Section DMA burst

    REG16                           Dnld_AddrH;                         // 0xbf802c28   0x0a
    #define TSP_DMA_RADDR_MSB_MASK                  0x00FF
    #define TSP_DMA_RADDR_MSB_SHIFT                 0

    REG16                           TSP_Ctrl2;                          // 0xbf802c2c   0x0b
    #define TSP_CMQ_WORD_EN                         0x0040              // Set 1 to access CMDQ related registers in word.
    #define TSP_AV_DIRECT_STOP                      0x0080              //Set 1 to enable A/V fifo full pull back tsif0 file in
    #define TSP_AV_DIRECT_STOP1                     0x0100              //Set 1 to enable A/V fifo full pull back tsif1 file in
    #define TSP_AV_DIRECT_STOP2                     0x0200              //Set 1 to enable A/V fifo full pull back tsif2 file in
    #define TSP_AV_DIRECT_STOP3                     0x0400              //Set 1 to enable A/V fifo full pull back tsif3 file in
    #define TSP_TS_OUT_EN                           0x1000              // TS_CB out enable. for Serial input to parallel output
    #define TSP_PS_VID_3D_EN                        0x2000              //Set 1 to enable video 3D path in program stream mode

    REG32                           TsPidScmbStatTsin;                  // 0xbf802c30   0x0c

    REG32                           TsPidScmbStatFile;                  // 0xbf802c38   0x0e

    REG32                           PCR64_2_L;                          // 0xbf802c40   0x10

    REG32                           PCR64_2_H;                          // 0xbf802c48   0x12

    #define TSP_DMAW_BND_MASK                       0xFFFFFFFFUL
    REG32                           DMAW_LBND0;                         // 0xbf802c50   0x14    //sec1 protect

    REG32                           DMAW_UBND0;                         // 0xbf802c58   0x16

    REG32                           DMAW_LBND1;                         // 0xbf802c60   0x18    //sec2 protect

    REG32                           DMAW_UBND1;                         // 0xbf802c68   0x1A

    REG32                           HW2_CFG6;                           // 0xbf802c68   0x1C

    REG32                           HW2_CFG5;                           // 0xbf802c68   0x1E

    REG32                           VQ0_BASE;                           // 0xbf802c80   0x20

    REG16                           VQ0_SIZE;                           // 0xbf802c84   0x22
    #define TSP_VQ0_SIZE_208PK_MASK                 0xFFFF
    #define TSP_VQ0_SIZE_208PK_SHIFT                0

    REG16                           VQ0_CTRL;                           // 0xbf802c88   0x23
    #define TSP_VQ0_WR_THRESHOLD_MASK               0x000F
    #define TSP_VQ0_WR_THRESHOLD_SHIFT              0
    #define TSP_VQ0_PRIORTY_THRESHOLD_MASK          0x00F0
    #define TSP_VQ0_PRIORTY_THRESHOL_SHIFT          4
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_MASK          0x0F00
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_SHIFT         8
    #define TSP_VQ0_RESET                           0x1000
    #define TSP_VQ0_OVERFLOW_INT_EN                 0x4000              // Enable the interrupt for overflow happened on Virtual Queue path
    #define TSP_VQ0_CLR_OVERFLOW_INT                0x8000              // Clear the interrupt and the overflow flag

    REG16                           VQ_PIDFLT_CTRL;                    // 0xbf802c90   0x24

    #define TSP_REQ_VQ_RX_THRESHOLD_MASKE           0x000E
    #define TSP_REQ_VQ_RX_THRESHOLD_SHIFT           1
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN1            0x0000
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN2            0x0002
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN4            0x0004
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN8            0x0006
    #define TSP_PIDFLT0_OVF_INT_EN                  0x0040
    #define TSP_PIDFLT0_CLR_OVF_INT                 0x0080
    #define TSP_PIDFLT1_OVF_INT_EN                  0x0100
    #define TSP_PIDFLT1_CLR_OVF_INT                 0x0200
    #define TSP_PIDFLT2_OVF_INT_EN                  0x0400
    #define TSP_PIDFLT2_CLR_OVF_INT                 0x0800

    REG16                           _xbf202c94 ;                        // 0xbf802c94   0x25

    REG16                           MOBF_PVR1_Index[2];                 // 0xbf3a2c98   0x26
    #define TSP_MOBF_PVR1_INDEX_MASK               0x0000001F
    #define TSP_MOBF_PVR1_INDEX_SHIFT              0

    REG16                           MOBF_PVR2_Index[2];                 // 0xbf3a2cA0   0x28
    #define TSP_MOBF_PVR2_INDEX_MASK               0x0000001F
    #define TSP_MOBF_PVR2_INDEX_SHIFT              0

    REG32                           DMAW_LBND2;                         // 0xbf802ca8   0x2a    //PVR protect
    #define TSP_PVR_MASK            0x0FFFFFFFUL

    REG32                           DMAW_UBND2;                         // 0xbf802cb0   0x2c

    REG32                           DMAW_LBND3;                         // 0xbf802cb8   0x2e    //PVR 2 protect

    REG32                           DMAW_UBND3;                         // 0xbf802cc0   0x30

    REG32                           DMAW_LBND4;                         // 0xbf802cc8   0x32    //PVR 3 protect

    REG32                           DMAW_UBND4;                         // 0xbf802cd0   0x34

    REG32                           ORZ_DMAW_LBND;                      // 0xbf802cd8   0x36    //CPU protect
    #define TSP_ORZ_DMAW_LBND_MASK                  0xffffffffUL        //protect address is base on MIU unit (16byte aligment)
    REG32                           ORZ_DMAW_UBND;                      // 0xbf802ce0   0x38
    #define TSP_ORZ_DMAW_UBND_MASK                  0xffffffffUL

    REG16                           PIDFLT_PCR0;                        // 0xbf802ce8   0x3a
    #define TSP_PIDFLT_PCR0_PID_MASK                0x1fff
    #define TSP_PIDFLT_PCR0_EN                      0x8000


    REG16                           PIDFLT_PCR1;                        // 0xbf802ce8   0x3b
    #define TSP_PIDFLT_PCR1_PID_MASK                0x1fff
    #define TSP_PIDFLT_PCR1_EN                      0x8000

    REG32                           HWPCR0_L;                           // 0xbf802cf0   0x3c
    REG32                           HWPCR0_H;                           // 0xbf802cf8   0x3e

    REG32                           CA_CTRL;                            // 0xbf802d00   0x40
    #define TSP_CA_CTRL_MASK                        0xffffffff
    #define TSP_CA0_INPUT_TSIF0_LIVEIN              0x00000001
    #define TSP_CA0_INPUT_TSIF0_FILEIN              0x00000002
    #define TSP_CA0_INPUT_TSIF1                     0x00000004
    #define TSP_CA0_AVPAUSE                         0x00000008
    #define TSP_CA0_OUTPUT_PKTDMX0_LIVE             0x00000010
    #define TSP_CA0_OUTPUT_PKTDMX0_FILE             0x00000020
    #define TSP_CA0_OUTPUT_PKTDMX1                  0x00000040          //pkt_demux1
    #define TSP_CA0_INPUT_TSIF2                     0x00001000
    #define TSP_CA0_OUTPUT_PKTDMX2                  0x00002000          //pkt_demux2
    #define TSP_CA2_INPUT_TSIF2                     0x00100000
    #define TSP_CA2_OUTPUT_REC2                     0x00200000          //pkt_demux2
    #define TSP_CA2_INPUT_TSIF0_LIVEIN              0x01000000
    #define TSP_CA2_INPUT_TSIF0_FILEIN              0x02000000
    #define TSP_CA2_INPUT_TSIF1                     0x04000000
    #define TSP_CA2_OUTPUT_PLAY_LIVE                0x10000000
    #define TSP_CA2_OUTPUT_PLAY_FILE                0x20000000
    #define TSP_CA2_OUTPUT_REC1                     0x40000000          //pkt_demux1

    REG16                           OneWay;                             // 0xbf802d08   0x42 ,
    #define TSP_ONEWAY_CAREC_DISABLE                0x0001
    #define TSP_ONEWAY_PVR                          0x0002
    #define TSP_ONEWAY_PVR1                         0x0004
    #define TSP_ONEWAY_FW                           0x0008
    #define TSP_ONEWAY_QMEM                         0x0010
    #define TSP_ONEWAY_PVR2                         0x0020
    #define TSP_ONEWAY_FIQ                          0x0040

    REG16                           _xbf202d0C;                         // 0xbf802d0C   0x43

    REG32                           HWPCR1_L;                           // 0xbf802d10   0x44
    REG32                           HWPCR1_H;                           // 0xbf802d18   0x46

    REG16                           IND32_CMD;                          // 0xbf802d20   0x48

    REG32                           IND32_ADDR;                         // 0xbf802d24   0x49, Indirect address to TSP CPU

    REG32                           IND32_WDATA;                        // 0xbf802d2C   0x4B, Indirect write data to TSP CPUr

    REG32                           IND32_RDATA;                        // 0xbf802d34   0x4D, IND32_WDATA

    REG16                           _xbf202d3c;                         // 0xbf802d3C   0x4F

    REG16                           FIFO_Src;                           // 0xbf802d40   0x50
    #define TSP_AUD_SRC_MASK                        0x0007
    #define TSP_AUD_SRC_SHIFT                       0
    #define TSP_AUDB_SRC_MASK                       0x0038
    #define TSP_AUDB_SRC_SHIFT                      3
    #define TSP_VID_SRC_MASK                        0x01C0
    #define TSP_VID_SRC_SHIFT                       6
    #define TSP_VID3D_SRC_MASK                      0x0E00
    #define TSP_VID3D_SRC_SHIFT                     9
    #define TSP_PVR1_SRC_MASK                       0x7000
    #define TSP_PVR1_SRC_SHIFT                      12
    #define TSP_PVR2_SRC_MASK_L                     0x8000
    #define TSP_PVR2_SRC_SHIFT_L                    15

    REG16                           PCR_Cfg;                           // 0xbf802d44   0x51
    #define TSP_PVR2_SRC_MASK_H                     0x0003
    #define TSP_PVR2_SRC_SHIFT_H                    0
    #define TSP_AUDC_SRC_MASK                       0x001C
    #define TSP_AUDC_SRC_SHIFT                      2
    #define TSP_AUDD_SRC_MASK                       0x00E0
    #define TSP_AUDD_SRC_SHIFT                      5
    #define TSP_TEI_SKIP_PKT_PCR0                   0x0100
    #define TSP_PCR0_RESET                          0x0200
    #define TSP_PCR0_INT_CLR                        0x0400
    #define TSP_PCR0_READ                           0x0800
    #define TSP_TEI_SKIP_PKT_PCR1                   0x1000
    #define TSP_PCR1_RESET                          0x2000
    #define TSP_PCR1_INT_CLR                        0x4000
    #define TSP_PCR1_READ                           0x8000

    REG32                           STC_DIFF_BUF;                       // 0xbf802d48   0x52

    REG32                           STC_DIFF_BUF_H;                     // 0xbf802d50   0x54
    #define TSP_STC_DIFF_BUF_H_MASK                 0x0000000F
    #define TSP_STC_DIFF_BUF_H_AHIFT                0

    REG32                           VQ1_Base;                           // 0xbf802d58   0x56

    REG32                           _xbf202d60_6C[2];                   // 0xbf802d60   0x58~0x5B

    REG16                           VQ1_Size;                           // 0xbf802d70   0x5C
    #define TSP_VQ1_SIZE_208PK_MASK                 0xffff
    #define TSP_VQ1_SIZE_208PK_SHIFT                0

    REG16                           VQ1_Config;                         // 0xbf802d74   0x5d
    #define TSP_VQ1_WR_THRESHOLD_MASK               0x000F
    #define TSP_VQ1_WR_THRESHOLD_SHIFT              0
    #define TSP_VQ1_PRI_THRESHOLD_MASK              0x00F0
    #define TSP_VQ1_PRI_THRESHOLD_SHIFT             4
    #define TSP_VQ1_FORCEFIRE_CNT_1K_MASK           0x0F00
    #define TSP_VQ1_FORCEFIRE_CNT_1K_SHIFT          8
    #define TSP_VQ1_RESET                           0x1000
    #define TSP_VQ1_OVF_INT_EN                      0x4000
    #define TSP_VQ1_CLR_OVF_INT                     0x8000

    REG32                           VQ2_Base;                           // 0xbf802d78   0x5E

    REG32                           TS_WatchDog_Cnt;                    // 0xbf802d80   0x60
    #define TSP_TS_WATCH_DOG_MASK                   0xFFFF0000
    #define TSP_TS_WATCH_DOG_SHIFT                  16

    REG32                           Bist_Fail;                          // 0xbf802d88   0x62
    #define TSP_BIST_FAIL_STATUS_MASK               0x00FF0000
    #define TSP_BIST_FAIL_STATUS_SRAM1P192x8_MASK   0x00070000
    #define TSP_BIST_FAIL_STATUS_SRAM2P512x32w8     0x00080000
    #define TSP_BIST_FAIL_STATUS_SRAM2P16x128_MASK  0x00600000
    #define TSP_BIST_FAIL_STATUS_SRAM1P2048x32w8    0x00800000
    #define TSP_BIST_FAIL_STATUS_SRAM1P1024x32w8    0x01000000
    #define TSP_BIST_FAIL_STATUS_SRAM1P512x20       0x00200000

    REG16                           VQ2_Size;                         // 0xbf802d90   0x64
    #define TSP_VQ2_SIZE_208PK_MASK                 0xffff
    #define TSP_VQ2_SIZE_208PK_SHIFT                0

    REG16                           VQ2_Config;                         // 0xbf802d90   0x65
    #define TSP_VQ2_WR_THRESHOLD_MASK               0x000F
    #define TSP_VQ2_WR_THRESHOLD_SHIFT              0
    #define TSP_VQ2_PRI_THRESHOLD_MASK              0x00F0
    #define TSP_VQ2_PRI_THRESHOLD_SHIFT             4
    #define TSP_VQ2_FORCEFIRE_CNT_1K_MASK           0x0F00
    #define TSP_VQ2_FORCEFIRE_CNT_1K_SHIFT          8
    #define TSP_VQ2_RESET                           0x1000
    #define TSP_VQ2_OVF_INT_EN                      0x4000
    #define TSP_VQ2_CLR_OVF_INT                     0x8000

    REG32                           VQ_STATUS;                          // 0xbf802d98   0x66
    #define TSP_VQ_STATUS_MASK                      0xFFFFFFFF
    #define TSP_VQ_STATUS_SHIFT                     0
    #define TSP_VQ0_STATUS_READ_EVER_FULL           0x00001000
    #define TSP_VQ0_STATUS_READ_EVER_OVERFLOW       0x00002000
    #define TSP_VQ0_STATUS_EMPTY                    0x00004000
    #define TSP_VQ0_STATUS_READ_BUSY                0x00008000
    #define TSP_VQ1_STATUS_READ_EVER_FULL           0x00010000
    #define TSP_VQ1_STATUS_READ_EVER_OVERFLOW       0x00020000
    #define TSP_VQ1_STATUS_EMPTY                    0x00040000
    #define TSP_VQ1_STATUS_READ_BUSY                0x00080000
    #define TSP_VQ2_STATUS_READ_EVER_FULL           0x00100000
    #define TSP_VQ2_STATUS_READ_EVER_OVERFLOW       0x00200000
    #define TSP_VQ2_STATUS_EMPTY                    0x00400000
    #define TSP_VQ2_STATUS_READ_BUSY                0x00800000
    #define TSP_VQ0_STATUS_TX_OVERFLOW              0x10000000
    #define TSP_VQ1_STATUS_TX_OVERFLOW              0x20000000
    #define TSP_VQ2_STATUS_TX_OVERFLOW              0x40000000

    REG32                           DM2MI_WAddr_Err;                    // 0xbf802da0   0x68  , DM2MI_WADDR_ERR0

    REG32                           ORZ_DMAW_WAddr_Err;                 // 0xbf802da8   0x6a  , ORZ_WADDR_ERR0

    REG16                           HwInt2_Stat;                        // 0xbf802dB0   0x6c

        /*
            [7] : PVR2 meet_tail  or PVR2_meet_mid
            [6] : vq0, vq1, vq2, vq3 overflow interrupt
            [5] : all DMA write address not in the protect zone interrupt
            [4] : PVR_cb meet the mid or PVR_cb meet the tail
            [3] : pcr filter 0 update finish
            [2] : pcr filter 1 update finish
            [1] : OTV HW interrupt
            [0] : reserved
        */
    #define TSP_HWINT2_EN_MASK                              0x00FF
    #define TSP_HWINT2_EN_SHIFT                             0
    #define TSP_HWINT2_OTV_EN                               0x0002
    #define TSP_HWINT2_PCR1_UPDATE_END_EN                   0x0004
    #define TSP_HWINT2_PCR0_UPDATE_END_EN                   0x0008
    #define TSP_HWINT2_ALL_DMA_WADDR_NOT_IN_PROCT_Z_EN      0x0020
    #define TSP_HWINT2_VQ0_VQ1_VQ2_VQ3_OVERFLOW_EN          0x0040
    #define TSP_HWINT2_PVR2_MID_TAIL_STATUS_EN              0x0080
    #define TSP_HWINT_PVR                                   (TSP_HWINT2_PVR2_MID_TAIL_STATUS_EN) //@FIXME check what is this doing
    #define TSP_HWINT2_SUPPORT_ALL                          (TSP_HWINT2_PCR0_UPDATE_END_EN|TSP_HWINT2_PCR1_UPDATE_END_EN|TSP_HWINT2_OTV_EN)
    #define TSP_HWINT2_ALL                                  TSP_HWINT2_SUPPORT_ALL

    #define TSP_HWINT2_STATUS_MASK                          0xFF00
    #define TSP_HWINT2_STATUS_SHIFT                         8
    #define TSP_HWINT2_OTV                                  0x0200
    #define TSP_HWINT2_PCR1_UPDATE_END                      0x0400
    #define TSP_HWINT2_PCR0_UPDATE_END                      0x0800
    #define TSP_HWINT2_ALL_DMA_WADDR_NOT_IN_PROCT_Z         0x2000
    #define TSP_HWINT2_VQ0_VQ1_VQ2_VQ3_OVERFLOW             0x4000
    #define TSP_HWINT2_PVR2_MID_TAIL_STATUS                 0x8000

    REG32                           SwInt2_Stat;                        // 0xbf802dB4   0x6d

    REG16                           HwInt3_Stat;                        // 0xbf802dBC   0x6f

        /*
            [7:2] : reserved
            [1] : pcr filter 3 update finish
            [0] : pcr filter 2 update finish
        */
    #define TSP_HWINT3_EN_MASK                              0x00FF
    #define TSP_HWINT3_EN_SHIFT                             0
    #define TSP_HWINT3_PCR2_UPDATE_END_EN                   0x0001
    #define TSP_HWINT3_PCR3_UPDATE_END_EN                   0x0002
    #define TSP_HWINT3_SUPPORT_ALL                          (TSP_HWINT3_PCR2_UPDATE_END_EN|TSP_HWINT3_PCR3_UPDATE_END_EN)
    #define TSP_HWINT3_ALL                                  TSP_HWINT3_SUPPORT_ALL

    #define TSP_HWINT3_STATUS_MASK                          0xFF00
    #define TSP_HWINT3_STATUS_SHIFT                         8
    #define TSP_HWINT3_PCR2_UPDATE_END                      0x0100
    #define TSP_HWINT3_PCR3_UPDATE_END                      0x0200

    REG32                           TimeStamp_FileIn;                   // 0xbf802dC0   0x70

    REG16                           HW2_Config3;                        // 0xbf802dC8   0x72
    #define TSP_PVR_DMAW_PROTECT_EN                 0x0001
    #define TSP_WADDR_ERR_SRC_SEL_MASK              0x0006
    #define TSP_WADDR_ERR_SRC_SEL_SHIFT             1
    #define TSP_WADDR_ERR_SRC_PVR                   0x0000
    #define TSP_WADDR_ERR_SRC_VQ                    0x0002
    #define TSP_WADDR_ERR_SRC_SEC_CB                0x0004
    #define TSP_RM_OVF_GLITCH                       0x0008
    #define TSP_FILEIN_RADDR_READ                   0x0010
    #define TSP_DUP_PKT_CNT_CLR                     0x0040
    #define TSP_DMA_FLUSH_EN                        0x0080 //PVR1, PVR2 dma flush
    #define TSP_REC_AT_SYNC_DIS                     0x0100
    #define TSP_PVR1_ALIGN_EN                       0x0200
    #define TSP_REC_FORCE_SYNC_EN                   0x0400
    #define TSP_RM_PKT_DEMUX_PIPE                   0x0800
    #define TSP_VQ_EN                               0x4000
    #define TSP_VQ2PINGPONG_EN                      0x8000

    REG16                           PVRConfig;                        // 0xbf802dCC  0x73
    #define TSP_PVR1_REC_ALL_EN                     0x0001
    #define TSP_PVR2_REC_ALL_EN                     0x0002
    #define TSP_REC_NULL                            0x0004
    #define TSP_REC_ALL_OLD                         0x0008
    #define TSP_MATCH_PID_SEL_MASK                  0x0700
    #define TSP_MATCH_PID_SEL_SHIFT                 8
    #define TSP_MATCH_PID_LD                        0x8000

    REG32                           VQ3_Base;                     //0x74~75

    REG16                           VQ3_Size;                     // 0x76
    #define TSP_VQ3_SIZE_208PK_MASK                 0xffff
    #define TSP_VQ3_SIZE_208PK_SHIFT                0

    REG16                           VQ3_Config;                   //0x77
    #define TSP_VQ3_WR_THRESHOLD_MASK               0x000F
    #define TSP_VQ3_WR_THRESHOLD_SHIFT              0
    #define TSP_VQ3_PRI_THRESHOLD_MASK              0x00F0
    #define TSP_VQ3_PRI_THRESHOLD_SHIFT             4
    #define TSP_VQ3_FORCEFIRE_CNT_1K_MASK           0x0F00
    #define TSP_VQ3_FORCEFIRE_CNT_1K_SHIFT          8
    #define TSP_VQ3_RESET                           0x1000
    #define TSP_VQ3_OVF_INT_EN                      0x4000
    #define TSP_VQ3_CLR_OVF_INT                     0x8000

    REG32                           VQ_RX_Status;                 // 0xbf802de0   0x78
    #define VQ_RX_ARBITER_MODE_MASK                 0x0000000F
    #define VQ_RX_ARBITER_MODE_SHIFT                0
    #define VQ_RX0_PRI_MASK                         0x000000F0
    #define VQ_RX0_PRI_SHIFT                        4
    #define VQ_RX1_PRI_MASK                         0x00000F00
    #define VQ_RX1_PRI_SHIFT                        8
    #define VQ_RX2_PRI_MASK                         0x0000F000
    #define VQ_RX2_PRI_SHIFT                        12

    REG32                           _xbf802de8;                      // 0xbf802dC0   0x7a

    REG32                           MCU_Data1;                       // 0xbf802dC0   0x7c
} REG_Ctrl;

//TSP 3
typedef struct _REG_Ctrl2                                                               //TSP3 0x1702
{
    REG16    CFG_00;                                                                      // 0x00
        #define    CFG_00_TSP_FILE_IN_TSIF1_EN                                  0x0001    //Set 1: Enable FILE_input
        #define    CFG_00_MEM_TS_DATA_ENDIAN_TSIF1                              0x0002    //Set 1 to swap the byte order of TSIF1 DMA DATA bus
        #define    CFG_00_TSP_FILE_SEGMENT_TSIF1                                0x0004
        #define    CFG_00_FILEIN_RADDR_READ_TSIF1                               0x0008    //Read file DMA read address
        #define    CFG_00_MEM_TS_W_ORDER_TSIF1                                  0x0010    //Set 1 to swap the word order of TSIF1 MIU DATA bus
        #define    CFG_00_DIS_MIU_RQ_TSIF1                                      0x0020    //Disable the MIU request
        #define    CFG_00_RST_TS_FIN1                                           0x0040    //reset TSIF1
        #define    CFG_00_RST_FILEIN_TSIF1                                      0x0080    //reset the TSIF1 file in path
        #define    CFG_00_RST_CMDQ_FILEIN_TSIF1                                 0x0100    //reset the file in TSIF1 command queue
        #define    CFG_00_WB_RST_FILEIN_TSIF1                                   0x0200    //reset DMA to TSIF FSM in TSP clock Domain
        #define    CFG_00_RST_WB_DMA_FILEIN_TSIF1                               0x0400    //reset TSIF1 DMA in TSP clock Domain
        #define    CFG_00_FILE2MI_PRI_TSIF1                                     0x0800    //Set 1: Higher MIU ABT read priority
        #define    CFG_00_RST_READ_DMA_1                                        0x1000    //reset TSIF1 DMA in MIU clock Domain
        #define    CFG_00_LPCR2_LOAD_TSIF1                                      0x2000    //Load lpcr2 from TSIF1 90k counter
        #define    CFG_00_LPCR2_LOAD_BUF1                                       0x4000    //Load lpcr2 from pdflt1_buffer 90k counter
        #define    CFG_00_LPCR2_LOAD_BUF0                                       0x8000    //Load lpcr2 from pdflt0_buffer 90k counter
    REG16    CFG_01;                                                                      // 0x01
        #define    CFG_01_TSP_FILE_SEGMENT1                                     0x0001
        #define    CFG_01_TIMER_EN1                                             0x0002    //1: enable byte delay timer for TSIF1 filein path 0: packet delay timer
        #define    CFG_01_PKT192_EN1                                            0x0004    //Set 1 to enable TS packets with 192 bytes on file-in mode. (Blocking TS packets by timestamp)
        #define    CFG_01_PKT192_BLK_DISABLE1                                   0x0008    //Set 1 to disable file-in timestamp block scheme
        #define    CFG_01_LPCR2_WLD1                                            0x0010    //Set PCR to TSIF1 90k counter
        #define    CFG_01_TS_DATA_PORT_SEL1                                     0x0020    //TSIF1 data port output select. 0: select live TS to be TSIF output 1: select data port to be TSIF output
        #define    CFG_01_PDFLT2_FILE_SRC                                       0x00c0    //00:disable 01:tsif0 file in port 10:tsif1 file in port 11:disable
        #define    CFG_01_PDFLT2_FILE_SRC_SHIFT                                 6

        #define    CFG_01_PCR0_SRC_MASK                                         0x0f00
        #define    CFG_01_PCR0_SRC_SHIFT                                        8
        #define    CFG_01_PCR0_SRC_TSIF0                                        0x0
        #define    CFG_01_PCR0_SRC_TSIF1                                        0x1
        #define    CFG_01_PCR0_SRC_TSIF2                                        0x2
        #define    CFG_01_PCR0_SRC_TSIF3                                        0x3
        #define    CFG_01_PCR0_SRC_TSIF4                                        0x4
        #define    CFG_01_PCR0_SRC_TSIF5                                        0x5
        #define    CFG_01_PCR0_SRC_PKT_MERGE0                                   0x8
        #define    CFG_01_PCR0_SRC_PKT_MERGE1                                   0x9
        #define    CFG_01_PCR0_SRC_MM_FILEIN0                                   0xa
        #define    CFG_01_PCR0_SRC_MM_FILEIN1                                   0xb
        #define    CFG_01_PCR0_SRC_FIQ0                                         0xc
        #define    CFG_01_PCR0_SRC_FIQ1                                         0xd

        #define    CFG_01_PCR1_SRC_MASK                                         0xf000
        #define    CFG_01_PCR1_SRC_SHIFT                                        12
        #define    CFG_01_PCR1_SRC_TSIF0                                        0x0
        #define    CFG_01_PCR1_SRC_TSIF1                                        0x1
        #define    CFG_01_PCR1_SRC_TSIF2                                        0x2
        #define    CFG_01_PCR1_SRC_TSIF3                                        0x3
        #define    CFG_01_PCR1_SRC_TSIF4                                        0x4
        #define    CFG_01_PCR1_SRC_TSIF5                                        0x5
        #define    CFG_01_PCR1_SRC_PKT_MERGE0                                   0x8
        #define    CFG_01_PCR1_SRC_PKT_MERGE1                                   0x9
        #define    CFG_01_PCR1_SRC_MM_FILEIN0                                   0xa
        #define    CFG_01_PCR1_SRC_MM_FILEIN1                                   0xb
        #define    CFG_01_PCR1_SRC_FIQ0                                         0xc
        #define    CFG_01_PCR1_SRC_FIQ1                                         0xd
    REG16    CFG_02;
        #define    CFG_02_PKT_CHK_SIZE_FIN1                                     0x00ff    //(Packet Size - 1) for sync detection in TSIF1
        #define    CFG_02_PKT_DEMUX_SIZE_1                                      0xff00    //(Packet Size - 1) for sync detection in pkt_demux1
        #define    CFG_02_PKT_DEMUX_SIZE_1_SHIFT                                8
    REG16    CFG_03;
        #define    CFG_03_TSP_FILE_TIMER1                                       0xffff    //Bit [15:0] of timer threshold for TS file playback data fetch from MIU.
    REG16    CFG_04;
        #define    CFG_04_TSP_FILEIN_ABORT_ECO_TSIF0                            0x0001
        #define    CFG_04_TSP_FILEIN_ABORT_ECO_TSIF1                            0x0002
        #define    CFG_04_TSP_FILEIN_ABORT_ECO_TSIF2                            0x0004
        #define    CFG_04_TSP_FILEIN_ABORT_ECO_TSIF3                            0x0008
    REG16    CFG_05;
        #define    CFG_05_TSP_FILEIN_TSIF2                                      0x0001    //Set 1 to swap the word order of TSIF2 MIU DATA bus
        #define    CFG_05_MEM_TS_DATA_EDIAN_TSIF2                               0x0002    //Set 1 to swap the byte order of TSIF2 DMA DATA bus
        #define    CFG_05_TSP_FILE_SEGMENT_TSIF2                                0x0004    //set 0 to enable file in alignment mdoe
        #define    CFG_05_FILEIN_RDDR_READ_TSIF2                                0x0008    //Read file DMA read address
        #define    CFG_05_MEM_TS_W_ORDER_TSIF2                                  0x0010    //Set 1: Enable FILE_input
        #define    CFG_05_DIS_MIU_RQ_TSIF2                                      0x0020    //Disable the MIU request
        #define    CFG_05_RST_TS_FIN2                                           0x0040    //reset TSIF2
        #define    CFG_05_RST_FILEIN_TSIF2                                      0x0080    //reset the TSIF2 file in path
        #define    CFG_05_RST_CMDQ_FILEIN_TSIF2                                 0x0100    //reset the file in TSIF2 command queue
        #define    CFG_05_WB_RST_FILEIN_TSIF2                                   0x0200    //reset DMA to TSIF FSM in TSP clock Domain
        #define    CFG_05_RST_WB_DMA_FILEIN_TSIF2                               0x0400    //reset TSIF2 DMA in TSP clock Domain
        #define    CFG_05_FILE2MI_PRI_TSIF2                                     0x0800    //Set 1: Higher MIU ABT read priority
        #define    CFG_05_RST_READ_DMA_2                                        0x1000    //reset TSIF1 DMA in MIU clock Domain
        #define    CFG_05_LPCR2_LOAD_TSIF2                                      0x2000    //Load lpcr2 from TSIF2 90k counter
        #define    CFG_05_LPCR2_LOAD_BUF2                                       0x4000    //Load lpcr2 from pdflt2_buffer 90k counter
    REG16    CFG_06;
        #define    CFG_06_TSP_FILE_SEGMENT2                                     0x0001    //set 0 to enable file in alignment mdoe
        #define    CFG_06_TSP_TIMER_EN2                                         0x0002    //1: enable byte delay timer for TSIF2 filein path 0: packet delay timer
        #define    CFG_06_TSP_PKT192_EN2                                        0x0004    //Set 1 to enable TS packets with 192 bytes on file-in mode. (Blocking TS packets by timestamp)
        #define    CFG_06_TSP_PKT192_BLK_DISABLE2                               0x0008    //Set 1 to disable file-in timestamp block scheme
        #define    CFG_06_LPCR2_WLD2                                            0x0010    //Set PCR to TSIF2 90k counter
        #define    CFG_06_TS_DATA_PORT_SEL2                                     0x0020    //TSIF2 data port output select. 0: select live TS to be TSIF output  1: select data port to be TSIF output
        #define    CFG_06_PIDFLT5_FILE_SRC                                      0x00C0    //pdflt5 file in source 00:disable 01:tsif2 file in port 10:tsif3 file in port 11:disable
        #define    CFG_06_PIDFLT5_FILE_SRC_SHIFT                                6
        #define    CFG_06_PCR0_ID_SEL                                           0x0700    //pkt merge multi-stream id select 0: stream 0 1: stream 1 2: stream 2 3: stream 3
        #define    CFG_06_PCR0_ID_SEL_SHFIT                                     8
        #define    CFG_06_PCR1_ID_SEL                                           0x3800    //pkt merge multi-stream id select 0: stream 0 1: stream 1 2: stream 2 3: stream 3
        #define    CFG_06_PCR1_ID_SEL_SHFIT                                     11
    REG16    CFG_07;
        #define    CFG_07_PKT_CHK_SIZE_FIN2                                     0x00ff    //(Packet Size ¡V 1) for sync detection in TSIF2
        #define    CFG_07_PKTDMX_SIZE2                                          0xff00    //(Packet Size ¡V 1) for sync detection in pkt_demux2
        #define    CFG_07_PKTDMX_SIZE2_SHIFT                                    8
    REG16    CFG_08;
        #define    CFG_08_TSP_FILE_TIMER2                                       0x00ff
    REG16    CFG_09;                                                                      // reserved
    REG16    CFG_0A;
        #define    CFG_0A_TSP_FILE_IN_TSIF3                                     0x0001    //Set 1: Enable FILE_input
        #define    CFG_0A_MEM_TS_DATA_EDIAN_TSIF3                               0x0002    //Set 1 to swap the byte order of TSIF3 DMA DATA bus
        #define    CFG_0A_TSP_FILE_SEGMENT_TSIF3                                0x0004    //set 0 to enable file in alignment mdoe
        #define    CFG_0A_FILEIN_RADDR_READ_TSIF3                               0x0008    //Read file DMA read address
        #define    CFG_0A_MEM_TS_W_ORDER_TSIF3                                  0x0010    //Set 1: Enable FILE_input
        #define    CFG_0A_DIS_MIU_RQ_TSIF3                                      0x0020    //Set 1 to swap the byte order of TSIF3 DMA DATA bus
        #define    CFG_0A_RST_TS_FIN3                                           0x0040    //set 0 to enable file in alignment mdoe
        #define    CFG_0A_RST_FILEIN_TSIF3                                      0x0080    //Read file DMA read address
        #define    CFG_0A_RST_CMDQ_FILEIN_TSIF3                                 0x0100    //reset the file in TSIF3 command queue
        #define    CFG_0A_WB_RST_FILEIN_TSIF3                                   0x0200    //reset DMA to TSIF FSM in TSP clock Domain
        #define    CFG_0A_RST_WB_DMA_FILEIN_TSIF3                               0x0400    //reset TSIF3 DMA in TSP clock Domain
        #define    CFG_0A_FILE2MI_PRI_TSIF3                                     0x0800    //Set 1: Higher MIU ABT read priority
        #define    CFG_0A_RST_READ_DMA_3                                        0x1000    //reset TSIF3 DMA in MIU clock Domain
        #define    CFG_0A_LPCR2_LOAD_TSIF3                                      0x2000    //Load lpcr2 from TSIF3 90k counter
        #define    CFG_0A_LPCR2_LOAD_BUF3                                       0x4000    //Load lpcr2 from pdflt3_buffer 90k counter
    REG16    CFG_0B;
        #define    CFG_0B_TSP_FILE_SEGMENT3                                     0x0001    //set 0 to enable file in alignment mdoe
        #define    CFG_0B_TIMER_EN3                                             0x0002    //1: enable byte delay timer for TSIF3 filein path 0: packet delay timer
        #define    CFG_0B_PKT192_EN3                                            0x0004    //Set 1 to enable TS packets with 192 bytes on file-in mode. (Blocking TS packets by timestamp)
        #define    CFG_0B_PKT192_BLK_DISABLE3                                   0x0008    //Set 1 to disable file-in timestamp block scheme
        #define    CFG_0B_LPCR2_WLD3                                            0x0010    //Set PCR to TSIF3 90k counter
        #define    CFG_0B_TS_DATA_PORT_SEL3                                     0x0020    //TSIF3 data port output select. 0: select live TS to be TSIF output 1: select data port to be TSIF output
        #define    CFG_0B_P_SEL3                                                0x0040    //select parallel TS interface for TSIF3
        #define    CFG_0B_EXT_SYNC_SEL3                                         0x0080    //select exteranl sync for ts_if3
        #define    CFG_0B_TS_IF3_EN                                             0x0100    //set 1 tsif3 live in enable
        #define    CFG_0B_TS_DATA3_SWAP                                         0x0200    //tsif3 live in bit order swap
    REG16    CFG_0C;
        #define    CFG_0C_PKT_CHK_SIZE_FIN3                                     0x00ff    //(Packet Size ¡V 1) for sync detection in TSIF3
        #define    CFG_0C_PKT_DMX_SIZE3                                         0xff00    //(Packet Size ¡V 1) for sync detection in pkt_demux3
        #define    CFG_0C_PKT_DMX_SIZE3_SHIFT                                   8

    REG16    CFG_0D;
        #define    CFG_0D_TSP_FILE_TIMER3                                       0xffff    //Bit [15:0] of timer threshold for TS file playback data fetch from MIU.

    REG16    CFG_0E;
        #define    CFG_0E_PKT_DEMUX_SIZE_0                                      0x00ff    //(Packet Size - 1) for sync detection in pkt_demux0
        #define    CFG_0E_PKT_SIZE3                                             0xff00    //(Packet Size ¡V 1) for sync detection in pkt_flt3
        #define    CFG_0E_PKT_SIZE3_SHIFT                                       8

    REG16    CFG_0F;
        #define    CFG_0F_PKT_CHK_SIZE3                                         0x00ff    //(Packet Size ¡V 1) for sync detection in TSIF3.
        #define    CFG_0F_SYNC_BYTE3                                            0xff00    //Sync byte for TSIF3
        #define    CFG_0F_SYNC_BYTE3_SHIFT                                      8

    REG16    CFG_10;
        #define    CFG_10_RESET_PDFLT0                                          0x0001    //reset Pdflt0
        #define    CFG_10_RESET_PDFLT1                                          0x0002    //reset Pdflt1
        #define    CFG_10_RESET_PDFLT2                                          0x0004    //reset Pdflt2
        #define    CFG_10_RESET_PDFLT3                                          0x0008    //reset Pdflt3
    REG16    CFG_11;
        #define    CFG_11_RECEIVE_BUF0_SRC                                      0x0003    //Receive BUF0 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
        #define    CFG_11_RECEIVE_BUF0_SRC_SHIFT                                0
        #define    CFG_11_RECEIVE_BUF1_SRC                                      0x000c    //Receive BUF1 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
        #define    CFG_11_RECEIVE_BUF1_SRC_SHIFT                                2
        #define    CFG_11_RECEIVE_BUF2_SRC                                      0x0030    //Receive BUF2 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
        #define    CFG_11_RECEIVE_BUF2_SRC_SHIFT                                4
        #define    CFG_11_RECEIVE_BUF3_SRC                                      0x00c0    //Receive BUF3 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
        #define    CFG_11_RECEIVE_BUF3_SRC_SHIFT                                6
    REG16    CFG_12;
        #define    CFG_12_TIMESTAMP_SEL_PVR1                                    0x0001    //PVR1 timestamp sel 0:local timestamp 1:stream timestamp
        #define    CFG_12_TIMESTAMP_SEL_PVR2                                    0x0002    //PVR2 timestamp sel 0:local timestamp 1:stream timestamp
        #define    CFG_12_TIMESTAMP_SEL_PVR3                                    0x0004    //PVR3 timestamp sel 0:local timestamp 1:stream timestamp
        #define    CFG_12_TIMESTAMP_SEL_PVR4                                    0x0008    //PVR4 timestamp sel 0:local timestamp 1:stream timestamp

        #define    CFG_12_REG_REST_RBF0                                         0x0010    //reset Receive buffer0
        #define    CFG_12_REG_REST_RBF1                                         0x0020    //reset Receive buffer1
        #define    CFG_12_REG_REST_RBF2                                         0x0040    //reset Receive buffer2
        #define    CFG_12_REG_REST_RBF3                                         0x0080    //reset Receive buffer2

        #define    CFG_12_REG_REST_PDBF0                                        0x0400    //reset Pdflt_buf0
        #define    CFG_12_REG_REST_PDBF1                                        0x0800    //reset Pdflt_buf1
        #define    CFG_12_REG_REST_PDBF2                                        0x1000    //reset Pdflt_buf2
        #define    CFG_12_REG_REST_PDBF3                                        0x2000    //reset Pdflt_buf3
    REG16    CFG_13;
        #define    CFG_13_LPCR_WLD0                                             0x0001    //Set PCR to pdflt_buf0 90k counter
        #define    CFG_13_LPCR_EN0                                              0x0002    //Enable Pdflt_buf0 90k counter
        #define    CFG_13_LPCR_WLD1                                             0x0004    //Set PCR to pdflt_buf1 90k counter
        #define    CFG_13_LPCR_EN1                                              0x0008    //Enable Pdflt_buf1 90k counter
        #define    CFG_13_LPCR_WLD2                                             0x0010    //Set PCR to pdflt_bu
        #define    CFG_13_LPCR_EN2                                              0x0020    //Enable Pdflt_buf1 90k counter
        #define    CFG_13_LPCR_WLD3                                             0x0040    //Set PCR to pdflt_bu
        #define    CFG_13_LPCR_EN3                                              0x0080    //Enable Pdflt_buf1 90k counter
        #define    CFG_13_REG_RESET_ABT0                                        0x4000    //reset pkt_merge0
        #define    CFG_13_REG_RESET_ABT1                                        0x8000    //reset pkt_merge1
    REG16    CFG_14;
        #define    CFG_14_ABT_PORT0_SRC                                         0x0007    //pkt_merge0 input Stream source selection 0: tsif0 stream 1: tsif1 stream 2: tsif2 stream 3: tsif3 stream 4: tsif4 stream 5: tsif5 stream
        #define    CFG_14_ABT_PORT0_SRC_SHIFT                                   0
        #define    CFG_14_ABT_PORT1_SRC                                         0x0038    //pkt_merge0 input Stream source selection 1: tsif0 stream 1: tsif1 stream 2: tsif2 stream 3: tsif3 stream 4: tsif4 stream 5: tsif5 stream
        #define    CFG_14_ABT_PORT1_SRC_SHIFT                                   3
        #define    CFG_14_ABT_PORT2_SRC                                         0x01c0    //pkt_merge0 input Stream source selection 2: tsif0 stream 1: tsif1 stream 2: tsif2 stream 3: tsif3 stream 4: tsif4 stream 5: tsif5 stream
        #define    CFG_14_ABT_PORT2_SRC_SHIFT                                   6
        #define    CFG_14_ABT_PORT3_SRC                                         0x1E00    //pkt_merge0 input Stream source selection 2: tsif0 stream 1: tsif1 stream 2: tsif2 stream 3: tsif3 stream 4: tsif4 stream 5: tsif5 stream
        #define    CFG_14_ABT_PORT3_SRC_SHIFT                                   9
    REG16    CFG_15;
        #define    CFG_15_RBUF_FULL_LEVEL                                       0x0038
        #define    CFG_15_PVR3_SRC                                              0x0e00    //PVR3 input path sel 000: pkt_demux0 001: pkt_demux1 010: pkt_demux2 011: pkt_demux3 100: pkt_demux4 101: pkt_demux5
        #define    CFG_15_PVR3_SRC_SHIFT                                        9
        #define    CFG_15_PVR4_SRC                                              0x7000    //PVR4input path sel 000: pkt_demux0 001: pkt_demux1 010: pkt_demux2 011: pkt_demux3 100: pkt_demux4 101: pkt_demux5
        #define    CFG_15_PVR4_SRC_SHIFT                                        12

    REG16    CFG_16;
        #define    CFG_16_PVR3_REG_PINGPONG_EN                                  0x0001    //set 1 to enable the pingpong buffer of PVR3
        #define    CFG_16_PVR3_STR2MI_EN                                        0x0002    //set 1 to enable PVR3
        #define    CFG_16_PVR3_STR2MI_RST_WADR                                  0x0004    //set 1 to reset the PVR3 write pointer to the head address
        #define    CFG_16_PVR3_STR2MI_PAUSE                                     0x0008    //set 1 to pause PVR3
        #define    CFG_16_PVR3_PKT192_EN                                        0x0010    //set 1 to enable 192 mode of PVR3
        #define    CFG_16_PVR3_BURST_LEN_MASK                                   0x0060    //the PVR3 dma burst length 00 : burst 8 01 : burst 4 10/11 : burst 1
        #define    CFG_16_PVR3_BURST_LEN_SHIFT                                  5
        #define    CFG_16_PVR3_LPCR1_WLD                                        0x0080    //set 1 to write the value of lpcr1 from the register to the lpcr1_buf for PVR3
        #define    CFG_16_PVR3_PVR_ALIGN_EN                                     0x0100    //set 1 to enable the function of alignment of PVR3
        #define    CFG_16_PVR3_STR2MI_DSWAP                                     0x0200    //set 1 to swap the bit order of stream2miu data bus of PVR3
        #define    CFG_16_PVR3_STR2MI_BT_ORDER                                  0x0400    //Byte order of 16-byte recoding buffer to MIU of PVR3 0: Little endian. 1: Big endian
        #define    CFG_16_REC_DATA3_INV_EN                                      0x0800    //Set 1 to enable data payload invert for PVR record
        #define    CFG_16_PVR3_BLOCK_DIS                                        0x1000    //set 1 to disable the PVR3 fifo blocking mechanism
        #define    CFG_16_PID_BYPASS3_REC                                       0x2000    //0: record PES 1: record 188/192
        #define    CFG_16_REC_ALL3                                              0x4000    //set 1 to record all
        #define    CFG_16_PVR3_LPCR1_RLD                                        0x8000    //set 1 to read the value of lpcr1 from the register to the lpcr1_buf for PVR3
    REG32    CFG_17_18;
        #define    CFG_17_18_PVR3_STR2MI_HEAD                                   0xffffffff    //[31:27] : reserved [26:0] : MIU start address1 of TS recoding buffer for PVR3
    REG32    CFG_19_1A;
        #define    CFG_19_1A_PVR3_STR2MI_MID                                    0xffffffff    //[31:27] : reserved [26:0] : MIU middle address1 of TS recoding buffer for PVR3.
    REG32    CFG_1B_1C;
        #define    CFG_1B_1C_PVR3_STR2MI_TAIL                                   0xffffffff    //[31:27] : reserved [26:0] : MIU tail address1 of TS recoding buffer for PVR3.
    REG32    CFG_1D_1E;
        #define    CFG_1D_1E_PVR3_STR2MI_HEAD2                                  0xffffffff    //[31:27] : reserved [26:0] : MIU start address2 of TS recoding buffer for PVR3
    REG32    CFG_1F_20;
        #define    CFG_1F_20_PVR3_STR2MI_MID2                                   0xffffffff    //[31:27] : reserved [26:0] : MIU middle address2 of TS recoding buffer for PVR3.
    REG32    CFG_21_22;
        #define    CFG_21_22_PVR3_STR2MI_TAIL2                                  0xffffffff    //[31:27] : reserved [26:0] : MIU tail address2 of TS recoding buffer for PVR3.

    REG16    CFG_23;
        #define    CFG_23_PVR4_REG_PINGPONG_EN                                  0x0001    //set 1 to enable the pingpong buffer of PVR4
        #define    CFG_23_PVR4_STR2MI_EN                                        0x0002    //set 1 to enable PVR4
        #define    CFG_23_PVR4_STR2MI_RST_WADR                                  0x0004    //set 1 to reset the PVR4 write pointer to the head address
        #define    CFG_23_PVR4_STR2MI_PAUSE                                     0x0008    //set 1 to pause PVR4
        #define    CFG_23_PVR4_PKT192_EN                                        0x0010    //set 1 to enable 192 mode of PVR4
        #define    CFG_23_PVR4_BURST_LEN_MASK                                   0x0060    //the PVR4 dma burst length 00 : burst 8 01 : burst 4 10/11 : burst 1
        #define    CFG_23_PVR4_BURST_LEN_SHIFT                                  5
        #define    CFG_23_PVR4_LPCR1_WLD                                        0x0080    //set 1 to write the value of lpcr1 from the register to the lpcr1_buf for PVR4
        #define    CFG_23_PVR4_PVR_ALIGN_EN                                     0x0100    //set 1 to enable the function of alignment of PVR4
        #define    CFG_23_PVR4_STR2MI_DSWAP                                     0x0200    //set 1 to swap the bit order of stream2miu data bus of PVR4
        #define    CFG_23_PVR4_STR2MI_BT_ORDER                                  0x0400    //Byte order of 16-byte recoding buffer to MIU of PVR4 0: Little endian. 1: Big endian
        #define    CFG_23_REC_DATA4_INV_EN                                      0x0800    //Set 1 to enable data payload invert for PVR record
        #define    CFG_23_PVR4_BLOCK_DIS                                        0x1000    //set 1 to disable the PVR4 fifo blocking mechanism
        #define    CFG_23_PID_BYPASS4_REC                                       0x2000    //0: record PES 1: record 188/192
        #define    CFG_23_REC_ALL4                                              0x4000    //set 1 to record all
        #define    CFG_23_PVR4_LPCR1_RLD                                        0x8000    //set 1 to read the value of lpcr1 from the register to the lpcr1_buf for PVR4

    REG32    CFG_24_25;
        #define    CFG_24_25_PVR4_STR2MI_HEAD                                   0xffffffff    //[31:27] : reserved [26:0] : MIU start address1 of TS recoding buffer for PVR4
    REG32    CFG_26_27;
        #define    CFG_26_27_PVR4_STR2MI_MID                                    0xffffffff    //[31:27] : reserved [26:0] : MIU middle address1 of TS recoding buffer for PVR4
    REG32    CFG_28_29;
        #define    CFG_28_29_PVR4_STR2MI_TAIL                                   0xffffffff    //[31:27] : reserved [26:0] : MIU tail address1 of TS recoding buffer for PVR4
    REG32    CFG_2A_2B;
        #define    CFG_2A_2B_PVR4_STR2MI_HEAD2                                  0xffffffff    //[31:27] : reserved [26:0] : MIU start address2 of TS recoding buffer for PVR4
    REG32    CFG_2C_2D;
        #define    CFG_2C_2D_PVR4_STR2MI_MID2                                   0xffffffff    //[31:27] : reserved [26:0] : MIU middle address2 of TS recoding buffer for PVR4
    REG32    CFG_2E_2F;
        #define    CFG_2E_2F_PVR4_STR2MI_TAIL2                                  0xffffffff    //[31:27] : reserved [26:0] : MIU tail address2 of TS recoding buffer for PVR4

    REG32    CFG_30_31;
        #define    CFG_30_31_REG_TSP_FILEIN_RADDR_TSIF1                         0xffffffff    //Read start address [23:0] (byte unit) in tsif1 and command queue mode
    REG32    CFG_32_33;
        #define    CFG_32_33_REG_TSP_FILEIN_RNUM_TSIF1                          0xffffffff    //Read number [23:0] (byte unit) in tsif1 and command queue mode
    REG16    CFG_34;
        #define    CFG_34_REG_TSP_FILEIN_CTRL_TSIF1_START                       0x0001        //bit[0] Set 1 to start tsif1 in command
        #define    CFG_34_REG_TSP_FILEIN_CTRL_TSIF1_DONE                        0x0002        //bit[1] 1 : FileIn done
        #define    CFG_34_REG_TSP_FILEIN_INIT_TRUST_TSIF1                       0x0004        //bit[2] filein_init_trust for tsif1
        #define    CFG_34_REG_TSP_FILEIN_CTRL_TSIF1_ABORT                       0x0010        //bit[4] Set 1 to abort tsif1 in command queue mode
    REG32    CFG_35_36;
        #define    CFG_35_36_TSP_FILEIN_RADDR_TSIF2                             0xffffffff    //[31:24] : reserved [23:0] : Read start address [15:0] [23:16](byte unit) in tsif2 and command queue mode
    REG32    CFG_37_38;
        #define    CFG_37_38_TSP_FILEIN_RNUM_TSIF2                              0xffffffff    //[31:24] : reserved [23:0] : Read number [15:0] [23:16] (byte unit) in tsif2 and command queue mode
    REG16    CFG_39;
        #define    CFG_39_FILEIN_CTRL_TSIF2_START                               0x0001        //bit[0] Set 1 to start tsif2 in command
        #define    CFG_39_FILEIN_CTRL_TSIF2_DONE                                0x0002        //bit[1] 1: FileIn done
        #define    CFG_39_FILEIN_INIT_TRUST_TSIF2                               0x0004        //bit[2] filein_init_trust for tsif2
        #define    CFG_39_FILEIN_CTRL_TSIF2_ABORT                               0x0010        //bit[4] Set 1 to abort tsif2 in command queue mode
    REG32    CFG_3A_3B;
        #define    CFG_3A_3B_TSP_FILEIN_RADDR_TSIF3                             0xffffffff    //[31:24] : reserved [23:0] : Read start address [15:0] [23:16](byte unit) in tsif2 and command queue mode
    REG32    CFG_3C_3D;
        #define    CFG_3C_3D_TSP_FILEIN_RNUM_TSIF3                              0xffffffff    //[31:24] : reserved [23:0] : Read number [15:0] [23:16] (byte unit) in tsif2 and command queue mode
    REG16    CFG_3E;
        #define    CFG_3E_FILEIN_CTRL_TSIF3_START                               0x0001        //bit[0] Set 1 to start tsif2 in command
        #define    CFG_3E_FILEIN_CTRL_TSIF3_DONE                                0x0002        //bit[1] 1: FileIn done
        #define    CFG_3E_FILEIN_INIT_TRUST_TSIF3                               0x0004        //bit[2] filein_init_trust for tsif3
        #define    CFG_3E_FILEIN_CTRL_TSIF3_ABORT                               0x0010        //bit[4] Set 1 to abort tsif2 in command queue mode
    REG16    CFG_3F;
        #define    CFG_3F_REG_TSIF1_CMD_QUEUE_WR_CNT                            0x001f
        #define    CFG_3F_REG_TSIF1_CMD_QUEUE_FIFO_FULL                         0x0040
        #define    CFG_3F_REG_TSIF1_CMD_QUEUE_FIFO_EMPTY                        0x0080
        #define    CFG_3F_REG_TSIF1_CMD_QUEUE_WR_LEVEL                          0x0300
        #define    CFG_3F_REG_TSIF1_CMD_QUEUE_LEVEL_SHIFT                       8
        #define    CFG_3F_REG_TSIF1_CMD_QUEUE_SIZE                              16
    REG16    CFG_40;
        #define    CFG_40_REG_TSIF2_CMD_QUEUE_WR_CNT                            0x001f
        #define    CFG_40_REG_TSIF2_CMD_QUEUE_FIFO_FULL                         0x0040
        #define    CFG_40_REG_TSIF2_CMD_QUEUE_FIFO_EMPTY                        0x0080
        #define    CFG_40_REG_TSIF2_CMD_QUEUE_WR_LEVEL                          0x0300
        #define    CFG_40_REG_TSIF2_CMD_QUEUE_LEVEL_SHIFT                       8
        #define    CFG_40_REG_TSIF2_CMD_QUEUE_SIZE                              16
    REG16    CFG_41;
        #define    CFG_41_REG_TSIF3_CMD_QUEUE_WR_CNT                            0x001f
        #define    CFG_41_REG_TSIF3_CMD_QUEUE_FIFO_FULL                         0x0040
        #define    CFG_41_REG_TSIF3_CMD_QUEUE_FIFO_EMPTY                        0x0080
        #define    CFG_41_REG_TSIF3_CMD_QUEUE_WR_LEVEL                          0x0300
        #define    CFG_41_REG_TSIF3_CMD_QUEUE_LEVEL_SHIFT                       8
        #define    CFG_41_REG_TSIF3_CMD_QUEUE_SIZE                              16
    REG32    CFG_42_43;
        #define    CFG_42_43_REG_TIMESTAMP_TSP_FILEIN_TSIF1                     0xffffffff    //tsif1 pkt timestamp
    REG32    CFG_44_45;
        #define    CFG_44_45_REG_TIMESTAMP_TSP_FILEIN_TSIF2                     0xffffffff    //tsif2 pkt timestamp
    REG32    CFG_46_47;
        #define    CFG_46_47_REG_TIMESTAMP_TSP_FILEIN_TSIF3                     0xffffffff    //tsif3 pkt timestamp
    REG16    CFG_48;
        #define    CFG_48_REG_INT0                                              0xffff
    REG16    CFG_49;
    REG16    CFG_4A_4F[6];
    REG32    CFG_50_51;
        #define    CFG_50_51_LPCR2_TSIF1_RD                                     0xffffffff    //tsif1 90k counter value
    REG32    CFG_52_53;
        #define    CFG_52_53_LPCR2_TSIF2_RD                                     0xffffffff    //tsif2 90k counter value
    REG32    CFG_54_55;
        #define    CFG_54_55_LPCR2_TSIF3_RD                                     0xffffffff    //tsif3 90k counter value
    REG32    CFG_56_57;
        #define    CFG_56_57_LPCR2_BUF0_RD                                      0xffffffff    // pdflt_buf0 90k counter value
    REG32    CFG_58_59;
        #define    CFG_58_59_LPCR2_BUF1_RD                                      0xffffffff    // pdflt_buf1 90k counter value
    REG32    CFG_5A_5B;
        #define    CFG_5A_5B_LPCR2_BUF2_RD                                      0xffffffff    // pdflt_buf2 90k counter value
    REG32    CFG_5C_5D;
        #define    CFG_5C_5D_LPCR2_BUF3_RD                                      0xffffffff    // pdflt_buf3 90k counter value
    REG32    CFG_5E_5F;
        #define    CFG_5E_5F_LPCR2_BUF4_RD                                      0xffffffff    // pdflt_buf4 90k counter value
    REG32    CFG_60_61;
        #define    CFG_60_61_LPCR2_BUF5_RD                                      0xffffffff    // pdflt_buf5 90k counter value
    REG32    CFG_62_63;
        #define    CFG_62_63_LPCR2_PVR3_RD                                      0xffffffff    // PVR3 90k counter value
    REG32    CFG_64_65;                                                                       // Reserved
        #define    CFG_64_65_LPCR2_PVR4_RD                                      0xffffffff    // PVR4 90k counter value
    REG32    CFG_66_67;
        #define    CFG_66_67_PVR3_STR2MI_WADR_R                                 0xffffffff    // PVR3 write point
    REG32    CFG_68_69;
        #define    CFG_68_69_PVR4_STR2MI_WADR_R                                 0xffffffff    // PVR4 write point
    REG32    CFG_6A_6B;
        #define    CFG_6A_6B_TSP2MI_RADDR_S_TSIF1                               0x0fffffff    // tsif1 DMA read point
    REG32    CFG_6C_6D;
        #define    CFG_6C_6D_TSP2MI_RADDR_S_TSIF2                               0x0fffffff    // tsif2 DMA read point
    REG32    CFG_6E_6F;
        #define    CFG_6E_6F_TSP2MI_RADDR_S_TSIF3                               0x0fffffff    // tsif3 DMA read point
    REG16    CFG_70;
        #define    CFG_70_MATCHECED_VPID_3D_MASK                                0x1fff
        #define    CFG_70_CHANGE_VPID_3D                                        0x4000
    REG16    CFG_71;
        #define    CFG_71_MATCHECED_APID_B_MASK                                 0x1fff
        #define    CFG_71_CHANGE_APID_B                                         0x4000
    REG16    CFG_72;
        #define    CFG_72_MERGE_FIFO_STATUS                                     0x1fff
    REG16    CFG_73;
        #define    CFG_73_PVR_STATUS_PVR3_FIFO_MASK                             0x000f
        #define    CFG_73_PVR_STATUS_PVR3_FIFO_SHIFT                            0
        #define    CFG_73_PVR_STATUS_PVR3_EVER_OVERFLOW_MASK                    0x0001
        #define    CFG_73_PVR_STATUS_PVR3_EVER_OVERFLOW_SHIFT                   4
        #define    CFG_73_PVR_STATUS_PVR3_STR2MI_INT_MASK                       0x0006
        #define    CFG_73_PVR_STATUS_PVR3_STR2MI_INT_SHIFT                      5
        #define    CFG_73_PVR_STATUS_PVR4_FIFO_MASK                             0x000f
        #define    CFG_73_PVR_STATUS_PVR4_FIFO_SHIFT                            8
        #define    CFG_73_PVR_STATUS_PVR4_EVER_OVERFLOW_MASK                    0x0001
        #define    CFG_73_PVR_STATUS_PVR4_EVER_OVERFLOW_SHIFT                   12
        #define    CFG_73_PVR_STATUS_PVR4_STR2MI_INT_MASK                       0x0006
        #define    CFG_73_PVR_STATUS_PVR4_STR2MI_INT_SHIFT                      13
    REG16    CFG_74;
        #define    CFG_74_MATCHECED_APID_C_MASK                                 0x1fff
        #define    CFG_74_CHANGE_APID_C                                         0x4000
    REG16    CFG_75;
        #define    CFG_75_FI_MOBF_INDEC_TSIF1_MASK                              0x0000001F
    REG16    CFG_76;
        #define    CFG_76_FI_MOBF_INDEC_TSIF2_MASK                              0x0000001F
    REG16    CFG_77;
        #define    CFG_77_FI_MOBF_INDEC_TSIF3_MASK                              0x0000001F
    REG16    CFG_78_7B[4];
        #define    CFG_78_PVR3_INDEX                                            0x0000001F
    REG16    CFG_7C;
        #define    CFG_7C_MATCHECED_APID_D_MASK                                 0x1fff
        #define    CFG_7C_CHANGE_APID_D                                         0x4000
    REG16    CFG_7D;
        #define    CFG_7D_MATCHECED_VPID_3_MASK                                 0x1fff
        #define    CFG_7D_CHANGE_VPID_3                                         0x4000
    REG16    CFG_7E;
        #define    CFG_7E_MATCHECED_VPID_4_MASK                                 0x1fff
        #define    CFG_7E_CHANGE_VPID_4                                         0x4000

} REG_Ctrl2;

//TSP 4
typedef struct _REG_Ctrl3
{
    REG16   CFG3_00_09[10];                                                                         // 0x00 ~ 0x09
    REG16   CFG3_0A;                                                                                // 0x0A
        #define CFG3_0A_REG_PDFLT_BF0_CAVID                                     0x001F
        #define CFG3_0A_REG_PDFLT_BF1_CAVID                                     0x03E0
        #define CFG3_0A_REG_PDFLT_BF2_CAVID                                     0x7C00
    REG16   CFG3_0B;                                                                                // 0x0B
    REG16   CFG3_0C;                                                                                // 0x0C
        #define CFG3_0C_RBF0_PASS_MODE                                          0x0001
        #define CFG3_0C_RBF1_PASS_MODE                                          0x0002
        #define CFG3_0C_RBF2_PASS_MODE                                          0x0004
        #define CFG3_0C_RBF3_PASS_MODE                                          0x0008

        #define CFG3_0C_PKTDMX_CC_DROP_MSAK                                     0x03C0
        #define CFG3_0C_PKTDMX_CC_DROP_SHIFT                                    0x0006
        #define CFG3_0C_PIDFLT0_DUP_CC_SKIP                                     0x0040
        #define CFG3_0C_PIDFLT1_DUP_CC_SKIP                                     0x0080
        #define CFG3_0C_PIDFLT2_DUP_CC_SKIP                                     0x0100
        #define CFG3_0C_PIDFLT3_DUP_CC_SKIP                                     0x0200

    REG16   CFG3_0D;                                                                                // 0x0D
        #define CFG3_0D_PIDFLT0_ADP_DUP_CC_SKIP                                 0x0001
        #define CFG3_0D_PIDFLT1_ADP_DUP_CC_SKIP                                 0x0002
        #define CFG3_0D_PIDFLT2_ADP_DUP_CC_SKIP                                 0x0004
        #define CFG3_0D_PIDFLT3_ADP_DUP_CC_SKIP                                 0x0008
    REG16   CFG3_0E;                                                                                // 0x0E
        #define CFG3_0E_PDFBUF_FULL_SEL                                         0x0007
        #define CFG3_0E_PKT_MERGE_TIMESTAMP_SRC_SEL                             0x01F8              //reg_pkt_merge_timestamp_src_sel=>
        #define CFG3_0E_PIDBUF0_TIMESTAMP_27M                                   0x0008              //pdflt buffer 0 timestamp sel 1: 27m 0: 90k
        #define CFG3_0E_PIDBUF1_TIMESTAMP_27M                                   0x0010              //pdflt buffer 1 timestamp sel 1: 27m 0: 90k
        #define CFG3_0E_PIDBUF2_TIMESTAMP_27M                                   0x0020              //pdflt buffer 2 timestamp sel 1: 27m 0: 90k
        #define CFG3_0E_PIDBUF3_TIMESTAMP_27M                                   0x0040              //pdflt buffer 3 timestamp sel 1: 27m 0: 90k
        #define CFG3_0E_STREAM2MIU1_C27M                                        0x0200              //reg_stream2miu1_c90k_sel=>Stream2miu1  timestamp sel 1: 27m 0: 90k
        #define CFG3_0E_STREAM2MIU2_C27M                                        0x0400              //reg_stream2miu2_c90k_sel=>Stream2miu2  timestamp sel 1: 27m 0: 90k
        #define CFG3_0E_STREAM2MIU3_C27M                                        0x0800              //reg_stream2miu3_c90k_sel=>Stream2miu3  timestamp sel 1: 27m 0: 90k
        #define CFG3_0E_STREAM2MIU4_C27M                                        0x1000              //reg_stream2miu4_c90k_sel=>Stream2miu4  timestamp sel 1: 27m 0: 90k
    REG16   CFG3_0F;                                                                                // 0x0F
        #define CFG3_0F_TSIF0_C27M                                              0x0001              //reg_tsif0_c90k_sel=>Tsif0  timestamp sel 1: 27m 0: 90k
        #define CFG3_0F_TSIF1_C27M                                              0x0002              //reg_tsif1_c90k_sel=>Tsif1  timestamp sel 1: 27m 0: 90k
        #define CFG3_0F_TSIF2_C27M                                              0x0004              //reg_tsif2_c90k_sel=>Tsif2  timestamp sel 1: 27m 0: 90k
        #define CFG3_0F_TSIF3_C27M                                              0x0008              //reg_tsif3_c90k_sel=>Tsif3  timestamp sel 1: 27m 0: 90k
    REG16   CFG3_10;                                                                                // 0x10
        #define CFG3_10_TSO0_SRC                                                0x0007              //reg_tso0_src
        #define CFG3_10_TSO0_SRC_SHIFT                                          0
        #define CFG3_10_TSO0_SRC_PKTDMX0                                        0x0001
        #define CFG3_10_TSO0_SRC_PKTDMX1                                        0x0002
        #define CFG3_10_TSO0_SRC_PKTDMX2                                        0x0004
        #define CFG3_10_TSO1_SRC                                                0x0038              //reg_tso1_src
        #define CFG3_10_TSO1_SRC_SHIFT                                          3
        #define CFG3_10_TSO1_SRC_PKTDMX0                                        0x0001
        #define CFG3_10_TSO1_SRC_PKTDMX1                                        0x0002
        #define CFG3_10_TSO1_SRC_PKTDMX2                                        0x0004
        #define CFG3_10_TSO0_BLOCK_DIS                                          0x1000              //reg_tso0_block_dis
        #define CFG3_10_TSO1_BLOCK_DIS                                          0x2000              //reg_tso1_block_dis
        #define CFG3_10_PS_MODE_SRC_MASK                                        0x01C0
        #define CFG3_10_PS_MODE_SRC_SHIFT                                       6

    REG16   CFG3_11;                                                                                // 0x11
    REG32   CFG3_12_13;                                                                             // reg_dmaw_lbnd4
    REG32   CFG3_14_15;                                                                             //reg_dmaw_ubnd4
    REG16   CFG3_16;                                                                                // 0x16
        #define CFG3_16_FIXED_DMA_RSTART_OTP_ONEWAY_LOAD_FW                     0x8000
    REG16   CFG3_17;                                                                                // 0x17
        #define CFG3_17_INIT_TIMESTAMP_TSIF_0                                   0x0040
        #define CFG3_17_INIT_TIMESTAMP_TSIF_1                                   0x0080
        #define CFG3_17_INIT_TIMESTAMP_TSIF_2                                   0x0100
        #define CFG3_17_INIT_TIMESTAMP_TSIF_3                                   0x0200
    REG16   CFG3_18_1D[6];                                                                          // 0x18 ~ 0x1D
    REG16   CFG3_1E;                                                                                // 0X1E
        #define CFG3_1E_TSIF0_SPD_RESET                                         0x0001              //Tsif0 SPD rest
        #define CFG3_1E_TSIF1_SPD_RESET                                         0x0002              //Tsif1 SPD rest
        #define CFG3_1E_TSIF2_SPD_RESET                                         0x0004              //Tsif2 SPD rest
        #define CFG3_1E_TSIF3_SPD_RESET                                         0x0008              //Tsif3 SPD rest
    REG16   CFG3_1F;                                                                                // 0x1F
    REG16   CFG3_20;                                                                                // 0x20
        #define CFG3_20_PIDFLT0_CLR_REPLACE_EN_MASK                             0x000F              //reg_pdflt0_clear_replace_en=>clear pdflt 0  cc replace function flag
        #define CFG3_20_PIDFLT1_CLR_REPLACE_EN_MASK                             0x00F0              //reg_pdflt1_clear_replace_en=>clear pdflt 0  cc replace function flag
        #define CFG3_20_PIDFLT2_CLR_REPLACE_EN_MASK                             0x0F00              //reg_pdflt2_clear_replace_en=>clear pdflt 0  cc replace function flag
        #define CFG3_20_PIDFLT3_CLR_REPLACE_EN_MASK                             0xF000              //reg_pdflt3_clear_replace_en=>clear pdflt 0  cc replace function flag
    REG16   CFG3_21;                                                                                // 0x21
        #define CFG3_21_TSIF0_FILE_PAUSE                                        0x0100              // Set 1 to inform TSIF(file-in engine) back-end pipe is full
        #define CFG3_21_TSIF1_FILE_PAUSE                                        0x0200              // and don't transmit data
        #define CFG3_21_TSIF2_FILE_PAUSE                                        0x0400
        #define CFG3_21_TSIF3_FILE_PAUSE                                        0x0800
    REG16   CFG3_22;
        #define CFG3_22_PVR1_PKT_MEET_SIZE_L_MASK                               0xFFFF              //PVR1 callback PKT Meet Size
    REG16   CFG3_23;
        #define CFG3_23_PVR1_PKT_MEET_SIZE_H_MASK                               0x00FF
        #define CFG3_23_PVR1_STR2MI_CNT_CLR                                     0x0100
        #define CFG3_23_PVR1_STR2MI_CNT_INTMODE                                 0x0200
        #define CFG3_23_PVR1_STR2MI_SYNC_INTMODE                                0x0400
    REG16   CFG3_24;
        #define CFG3_24_PVR2_PKT_MEET_SIZE_L_MASK                               0xFFFF              //PVR2 callback PKT Meet Size
    REG16   CFG3_25;
        #define CFG3_25_PVR2_PKT_MEET_SIZE_H_MASK                               0x00FF
        #define CFG3_25_PVR2_STR2MI_CNT_CLR                                     0x0100
        #define CFG3_25_PVR2_STR2MI_CNT_INTMODE                                 0x0200
        #define CFG3_25_PVR2_STR2MI_SYNC_INTMODE                                0x0400
    REG16   CFG3_26;
        #define CFG3_26_PVR3_PKT_MEET_SIZE_L_MASK                               0xFFFF              //PVR3 callback PKT Meet Size
    REG16   CFG3_27;
        #define CFG3_27_PVR3_PKT_MEET_SIZE_H_MASK                               0x00FF
        #define CFG3_27_PVR3_STR2MI_CNT_CLR                                     0x0100
        #define CFG3_27_PVR3_STR2MI_CNT_INTMODE                                 0x0200
        #define CFG3_27_PVR3_STR2MI_SYNC_INTMODE                                0x0400
    REG16   CFG3_28_29[2];                                                                          // 0x28 ~ 0x29
    REG16   CFG3_2A;
        #define CFG3_2A_PKTDMX0_TRACE_MARK_V_EN                                 0x0001
        #define CFG3_2A_PKTDMX0_TRACE_MARK_V3D_EN                               0x0002
        #define CFG3_2A_PKTDMX0_TRACE_MARK_A_EN                                 0x0004
        #define CFG3_2A_PKTDMX0_TRACE_MARK_AB_EN                                0x0008
        #define CFG3_2A_PKTDMX0_TRACE_MARK_AC_EN                                0x0010
        #define CFG3_2A_PKTDMX1_TRACE_MARK_V_EN                                 0x0020
        #define CFG3_2A_PKTDMX1_TRACE_MARK_V3D_EN                               0x0040
        #define CFG3_2A_PKTDMX1_TRACE_MARK_A_EN                                 0x0080
        #define CFG3_2A_PKTDMX1_TRACE_MARK_AB_EN                                0x0100
        #define CFG3_2A_PKTDMX1_TRACE_MARK_AC_EN                                0x0200
        #define CFG3_2A_PKTDMX2_TRACE_MARK_V_EN                                 0x0400
        #define CFG3_2A_PKTDMX2_TRACE_MARK_V3D_EN                               0x0800
        #define CFG3_2A_PKTDMX2_TRACE_MARK_A_EN                                 0x1000
        #define CFG3_2A_PKTDMX2_TRACE_MARK_AB_EN                                0x2000
        #define CFG3_2A_PKTDMX2_TRACE_MARK_AC_EN                                0x4000
    REG16   CFG3_2B;
        #define CFG3_2B_PKTDMX3_TRACE_MARK_V_EN                                 0x0001
        #define CFG3_2B_PKTDMX3_TRACE_MARK_V3D_EN                               0x0002
        #define CFG3_2B_PKTDMX3_TRACE_MARK_A_EN                                 0x0004
        #define CFG3_2B_PKTDMX3_TRACE_MARK_AB_EN                                0x0008
        #define CFG3_2B_PKTDMX3_TRACE_MARK_AC_EN                                0x0010
        #define CFG3_2B_PKTDMX4_TRACE_MARK_V_EN                                 0x0020
        #define CFG3_2B_PKTDMX4_TRACE_MARK_V3D_EN                               0x0040
        #define CFG3_2B_PKTDMX4_TRACE_MARK_A_EN                                 0x0080
        #define CFG3_2B_PKTDMX4_TRACE_MARK_AB_EN                                0x0100
        #define CFG3_2B_PKTDMX4_TRACE_MARK_AC_EN                                0x0200
    REG16   CFG3_2C;
        #define CFG3_2C_PDFLT0_NDS_TEST_MODE                                    0x0001
        #define CFG3_2C_PDFLT1_NDS_TEST_MODE                                    0x0002
        #define CFG3_2C_PDFLT2_NDS_TEST_MODE                                    0x0004
        #define CFG3_2C_PDFLT3_NDS_TEST_MODE                                    0x0008
        #define CFG3_2C_AVFIFO_READ_SEL_MASK                                    0x01C0
        #define CFG3_2C_AVFIFO_READ_SEL_SHIFT                                   6
        #define CFG3_2C_AVFIFO_READ_SEL_V                                       0
        #define CFG3_2C_AVFIFO_READ_SEL_A                                       1
        #define CFG3_2C_AVFIFO_READ_SEL_AB                                      2
        #define CFG3_2C_AVFIFO_READ_SEL_V3D                                     3
        #define CFG3_2C_AVFIFO_READ_SEL_AC                                      4
        #define CFG3_2C_AVFIFO_READ_SEL_AD                                      5
        #define CFG3_2C_AVFIFO_READ_SEL_V3                                      6
        #define CFG3_2C_AVFIFO_READ_SEL_V4                                      7
        #define CFG3_2C_DEBUG_WR_SRC_SEL_MASK                                   0x0E00
    REG16   CFG3_2D;
        #define CFG3_2D_FIXED_RM_PINPONG_SYCN_IN_ECO                            0x0001              // fixed_rm_pinpong_sycn_in_eco
        #define CFG3_2D_VPID_3D_BYPASS                                          0x0002              // reg_vpid_3d_bypass
        #define CFG3_2D_APID_B_BYPASS                                           0x0004              // reg_apid_b_bypass
        #define CFG3_2D_APID_C_BYPASS                                           0x0008              // reg_apid_b_bypass
        #define CFG3_2D_APID_D_BYPASS                                           0x0010              // reg_apid_b_bypass
        #define CFG3_2D_PKTDMX0_TRACE_MARK_AD_EN                                0x0020              // reg_pkt_demux0_trace_mark_ad_en=>set 1 enable trace mark function in pktdemux0 audio d path
        #define CFG3_2D_PKTDMX1_TRACE_MARK_AD_EN                                0x0040              // reg_pkt_demux1_trace_mark_ad_en=>set 1 enable trace mark function in pktdemux1 audio d path
        #define CFG3_2D_PKTDMX2_TRACE_MARK_AD_EN                                0x0080              // reg_pkt_demux2_trace_mark_ad_en=>set 1 enable trace mark function in pktdemux2 audio d path
        #define CFG3_2D_PKTDMX3_TRACE_MARK_AD_EN                                0x0100              // reg_pkt_demux3_trace_mark_ad_en=>set 1 enable trace mark function in pktdemux3 audio d path
        #define CFG3_2D_FILTER_NULL_PKT                                         0x0800
    REG16   CFG3_2E;
        #define CFG3_2E_VPID_3_BYPASS                                           0x0001              // reg_vpid_3_bypass
        #define CFG3_2E_PKTDMX0_TRACE_MARK_V3_EN                                0x0002              // reg_pkt_demux0_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux0 video 3 path
        #define CFG3_2E_PKTDMX1_TRACE_MARK_V3_EN                                0x0004              // reg_pkt_demux1_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux1 video 3 path
        #define CFG3_2E_PKTDMX2_TRACE_MARK_V3_EN                                0x0008              // reg_pkt_demux2_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux2 video 3 path
        #define CFG3_2E_PKTDMX3_TRACE_MARK_V3_EN                                0x0010              // reg_pkt_demux3_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux3 video 3 path
    REG16   CFG3_2F;
        #define CFG3_2F_VPID_4_BYPASS                                           0x0001              // reg_vpid_4_bypass
        #define CFG3_2F_PKTDMX0_TRACE_MARK_V4_EN                                0x0002              // reg_pkt_demux0_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux0 video 4 path
        #define CFG3_2F_PKTDMX1_TRACE_MARK_V4_EN                                0x0004              // reg_pkt_demux1_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux1 video 4 path
        #define CFG3_2F_PKTDMX2_TRACE_MARK_V4_EN                                0x0008              // reg_pkt_demux2_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux2 video 4 path
        #define CFG3_2F_PKTDMX3_TRACE_MARK_V4_EN                                0x0010              // reg_pkt_demux3_trace_mark_v3_en=>set 1 enable trace mark function in pktdemux3 video 4 path
    REG16   CFG3_30;                                                                                // 0x30 reserved
    REG16   CFG3_31;                                                                                // 0x31
        #define CFG3_31_PVR1_MEET_SIZE_CNT_R_MASK                               0x00FF
        #define CFG3_31_PVR2_MEET_SIZE_CNT_R_MASK                               0xFF00
    REG16   CFG3_32;                                                                                // 0x32
        #define CFG3_31_PVR3_MEET_SIZE_CNT_R_MASK                               0x00FF
    REG16   CFG3_33;
        #define TSP_AFIFOC_EMPTY                                                0x0002
        #define TSP_AFIFOC_EMPTY_SHFT                                           1
        #define TSP_AFIFOC_FULL                                                 0x0004
        #define TSP_AFIFOC_FULL_SHFT                                            2
        #define TSP_AFIFOC_LEVEL                                                0x0018
        #define TSP_AFIFOC_LEVEL_SHFT                                           3
        #define TSP_AFIFOD_EMPTY                                                0x1000
        #define TSP_AFIFOD_EMPTY_SHFT                                           12
        #define TSP_AFIFOD_FULL                                                 0x4000
        #define TSP_AFIFOD_FULL_SHFT                                            13
        #define TSP_AFIFOD_LEVEL                                                0xC000
        #define TSP_AFIFOD_LEVEL_SHFT                                           14
    REG16   CFG3_34;                                                                                // 0x34
        #define CFG3_34_DUP_PKT_SKIP_V                                          0x0001              //reg_dup_pkt_skip_v
        #define CFG3_34_DUP_PKT_SKIP_V3D                                        0x0002              //reg_dup_pkt_skip_v3d
        #define CFG3_34_DUP_PKT_SKIP_A                                          0x0004              //reg_dup_pkt_skip_a
        #define CFG3_34_DUP_PKT_SKIP_AB                                         0x0008              //reg_dup_pkt_skip_ab
        #define CFG3_34_DUP_PKT_SKIP_AC                                         0x0010              //reg_dup_pkt_skip_ac
        #define CFG3_34_DUP_PKT_SKIP_AD                                         0x0020              //reg_dup_pkt_skip_ad
        #define CFG3_34_MASK_SRC_V_EN                                           0x0100              //mask_scr_vid_en
        #define CFG3_34_MASK_SRC_V3D_EN                                         0x0200              //mask_scr_v3d_en
        #define CFG3_34_MASK_SRC_A_EN                                           0x0400              //mask_scr_aud_en
        #define CFG3_34_MASK_SRC_AB_EN                                          0x0800              //mask_scr_aud_b_en
        #define CFG3_34_MASK_SRC_AC_EN                                          0x1000              //mask_scr_aud_c_en
        #define CFG3_34_MASK_SRC_AD_EN                                          0x2000              //mask_scr_aud_d_en
        #define CFG3_34_FIX_192_TIMER_0_EN                                      0x4000              //reg_fix_192_timer_0_en
        #define CFG3_34_TSP2MI_REQ_MCM_DISABLE                                  0x8000              //reg_tsp2mi_req_mcm_disable
    REG16   CFG3_35;                                                                                // 0x35
        #define HW4_CFG35_BLK_AD_SCMBTIS_TSP                                    0x0001
        #define HW4_CFG35_PUSI_3BYTE_MODE                                       0x0002
        #define HW4_CFG35_PKT_MERGE_AUTO_RST                                    0x0004
        #define HW4_CFG35_AES_OUT_BT_ORDER                                      0x0008
        #define HW4_CFG35_AES_IN_BT_ORDER                                       0x0010
        #define HW4_CFG35_PREVENT_PID_TABLE_SRAM_COLLISION                      0x0020
        #define HW4_CFG35_RW_CONDITION_0                                        0x0040
        #define HW4_CFG35_RW_CONDITION_1                                        0x0080
        #define HW4_CFG35_PUSI_UPDATE_SCMB_BIT                                  0x0100
        #define HW4_CFG35_BYPASS_TIMESTAMP_SEL1                                 0x0200
        #define HW4_CFG35_BYPASS_TIMESTAMP_SEL2                                 0x0400
        #define HW4_CFG35_BYPASS_TIMESTAMP_SEL3                                 0x0800
        #define HW4_CFG35_BYPASS_TIMESTAMP_SEL4                                 0x1000
        #define HW4_CFG35_CLR_SRAM_COLLISION                                    0x2000
        #define HW4_CFG35_PREVENT_SRAM_COLLISION                                0x4000
        #define HW4_CFG35_BYPASS_FILEIN_TO_FIQ                                  0x8000
    REG16   CFG3_36;
        #define HW4_CFG36_RVU_PSI_EN0                                           0x0001              //rvu setting
        #define HW4_CFG36_RVU_TEI_EN0                                           0x0002
        #define HW4_CFG36_RVU_ERR_CLR0                                          0x0004
        #define HW4_CFG36_RVU_EN0                                               0x0008
        #define HW4_CFG36_RVU_TIMESTAMP_EN0                                     0x0010
        #define HW4_CFG36_RVU_PID_12_TIE_0_EN0                                  0x0020
        #define HW4_CFG36_PAYLOAD_128_MODE_EN0                                  0x0040
        #define HW4_CFG36_RVU_PSI_EN1                                           0x0100
        #define HW4_CFG36_RVU_TEI_EN1                                           0x0200
        #define HW4_CFG36_RVU_ERR_CLR1                                          0x0400
        #define HW4_CFG36_RVU_EN1                                               0x0800
        #define HW4_CFG36_RVU_TIMESTAMP_EN1                                     0x1000
    REG16   CFG3_37;
        #define HW4_CFG37_3WIRE_SERIAL_MODE_TS0                                 0x0001
        #define HW4_CFG37_3WIRE_SERIAL_MODE_TS1                                 0x0002
        #define HW4_CFG37_3WIRE_SERIAL_MODE_TS2                                 0x0004
        #define HW4_CFG37_3WIRE_SERIAL_MODE_TS3                                 0x0008
        #define HW4_CFG37_NON_188_CNT_MODE                                      0x0100
        #define HW4_CFG37_MASK_SCR_PVR1_EN                                      0x0200
        #define HW4_CFG37_MASK_SCR_PVR2_EN                                      0x0400
        #define HW4_CFG37_MASK_SCR_PVR3_EN                                      0x0800
        #define HW4_CFG37_MASK_SCR_PVR4_EN                                      0x1000
        #define HW4_CFG37_RST_CC_MODE                                           0x2000
        #define HW4_CFG37_DIS_CNTR_INC_BY_PL                                    0x4000 // 1=without payload 0=with payload ???
    REG16   CFG3_38;
        #define HW4_CFG38_LOAD_SPS_KEY1                                         0x0001
        #define HW4_CFG38_LOAD_SPS_KEY2                                         0x0002
        #define HW4_CFG38_LOAD_SPS_KEY3                                         0x0004
        #define HW4_CFG38_LOAD_SPS_KEY4                                         0x0008
        #define HW4_CFG38_PKT192_SPS_EN1                                        0x0010
        #define HW4_CFG38_PKT192_SPS_EN2                                        0x0020
        #define HW4_CFG38_PKT192_SPS_EN3                                        0x0040
        #define HW4_CFG38_PKT192_SPS_EN4                                        0x0080
        #define HW4_CFG38_CA_PVR1_SEL_MASK                                      0x0300
        #define HW4_CFG38_CA_PVR1_SEL_SHIFT                                     8
        #define HW4_CFG38_CA_PVR2_SEL_MASK                                      0x0C00
        #define HW4_CFG38_CA_PVR2_SEL_SHIFT                                     10
        #define HW4_CFG38_CA_PVR3_SEL_MASK                                      0x3000
        #define HW4_CFG38_CA_PVR3_SEL_SHIFT                                     12
        #define HW4_CFG38_CA_PVR4_SEL_MASK                                      0xC000
        #define HW4_CFG38_CA_PVR4_SEL_SHIFT                                     14
    REG16   CFG3_39;
        #define HW4_CFG39_FLUSH_PVR_DATA                                        0x0001
        #define HW4_CFG39_FLUSH_PVR1_DATA                                       0x0002
        #define HW4_CFG39_FLUSH_PVR2_DATA                                       0x0004
        #define HW4_CFG39_FLUSH_PVR3_DATA                                       0x0008
    REG16   CFG3_3A;
        #define HW4_CFG3A_LOAD_SPD_KEY0                                         0x0001
        #define HW4_CFG3A_LOAD_SPD_KEY1                                         0x0002
        #define HW4_CFG3A_LOAD_SPD_KEY2                                         0x0004
        #define HW4_CFG3A_LOAD_SPD_KEY3                                         0x0008
        #define HW4_CFG3A_LOAD_SPD_KEY4                                         0x0010
        #define HW4_CFG3A_LOAD_SPD_KEY5                                         0x0020
    REG16   CFG3_3B_3F[5];
    REG16   CFG3_40;
            #define HW4_CFG40_HW_SEMAPHORE0_MASK                                0xFFFF
    REG16   CFG3_41;
            #define HW4_CFG41_HW_SEMAPHORE1_MASK                                0xFFFF
    REG16   CFG3_42;
            #define HW4_CFG42_HW_SEMAPHORE2_MASK                                0xFFFF
    REG16   CFG3_43;
        #define HW4_CFG43_SRC_AES_PVR_KEY_MASK                                  0x0007
        #define HW4_CFG43_SRC_AES_PVR1_KEY                                      0x0000
        #define HW4_CFG43_SRC_AES_PVR2_KEY                                      0x0001
        #define HW4_CFG43_SRC_AES_PVR3_KEY                                      0x0002
        #define HW4_CFG43_SRC_AES_PVR4_KEY                                      0x0003
        #define HW4_CFG43_SRC_AES_FI_KEY_MASK                                   0x0038
        #define HW4_CFG43_SRC_AES_FI_KEY_SHIFT                                  3
        #define HW4_CFG43_SRC_AES_FI0_KEY                                       0x0000
        #define HW4_CFG43_SRC_AES_FI1_KEY                                       0x0001
        #define HW4_CFG43_SRC_AES_FI2_KEY                                       0x0002
        #define HW4_CFG43_SRC_AES_FI3_KEY                                       0x0003
    REG32   CFG3_44_45;                                                         //pause time0 for PVR1+ FIQ application
    REG32   CFG3_46_47;                                                         //pause time1 for PVR2+ FIQ application
    REG32   CFG3_48_49;                                                         //pause time2 for PVR3+ FIQ application
    REG32   CFG3_4A_4B;                                                         //pause time3 for PVR4+ FIQ application
    REG16   CFG3_4C;
        #define CFG3_4C_TSP_VFIFO3_EMPTY                                        0x0001
        #define CFG3_4C_TSP_VFIFO3_EMPTY_SHFT                                   0
        #define CFG3_4C_TSP_VFIFO3_FULL                                         0x0002
        #define CFG3_4C_TSP_VFIFO3_FULL_SHFT                                    1
        #define CFG3_4C_TSP_VFIFO3_LEVEL                                        0x000C
        #define CFG3_4C_TSP_VFIFO3_LEVEL_SHFT                                   2
        #define CFG3_4C_TSP_VD3_FIFO_DISCON                                     0x0100
        #define CFG3_4C_TSP_VD3_FIFO_OVERFLOW                                   0x0200
    REG16   CFG3_4D;
        #define CFG3_4D_TSP_VFIFO4_EMPTY                                        0x0001
        #define CFG3_4D_TSP_VFIFO4_EMPTY_SHFT                                   0
        #define CFG3_4D_TSP_VFIFO4_FULL                                         0x0002
        #define CFG3_4D_TSP_VFIFO4_FULL_SHFT                                    1
        #define CFG3_4D_TSP_VFIFO4_LEVEL                                        0x000C
        #define CFG3_4D_TSP_VFIFO4_LEVEL_SHFT                                   2
        #define CFG3_4D_TSP_VD4_FIFO_DISCON                                     0x0100
        #define CFG3_4D_TSP_VD4_FIFO_OVERFLOW                                   0x0200
    REG16   CFG3_4E_4F[2];
    REG32   CFG3_50_51;
    REG16   CFG3_52;
        #define CFG3_52_SPD_TSIF0_BYPASS                                        0x0001
        #define CFG3_52_SPD_TSIF1_BYPASS                                        0x0002
        #define CFG3_52_SPD_TSIF2_BYPASS                                        0x0004
        #define CFG3_52_SPD_TSIF3_BYPASS                                        0x0008
    REG16   CFG3_53;
        #define CFG3_53_WB_FSM_RESET                                            0x0001
    REG16   CFG3_54_57[4];
    REG16   CFG3_58_5F[8];
    REG16   CFG3_60_67[8];                                                      //AES KEY PVR
    REG16   CFG3_68_6F[8];                                                      //AES KEY FILEIN
    REG16   CFG3_70_71[2];                                                      //BIST fail status
    REG16   CFG3_72;
        #define CFG3_72_PIDFLT_PCR0_SRC_ID_MASK                                 0x000F
        #define CFG3_72_PIDFLT_PCR0_SRC_ID_SHIFT                                0
        #define CFG3_72_PIDFLT_PCR1_SRC_ID_MASK                                 0x0F00
        #define CFG3_72_PIDFLT_PCR1_SRC_ID_SHIFT                                8
    REG16  CFG3_73;
        #define CFG3_73_PVR1_DMAW_PROTECT_EN                                    0x0001
        #define CFG3_73_PVR2_DMAW_PROTECT_EN                                    0x0002
        #define CFG3_73_PVR3_DMAW_PROTECT_EN                                    0x0004
        #define CFG3_73_PVR4_DMAW_PROTECT_EN                                    0x0008
        #define CFG3_73_FILEIN0_DMAR_PROTECT_EN                                 0x0010
        #define CFG3_73_FILEIN1_DMAR_PROTECT_EN                                 0x0020
        #define CFG3_73_FILEIN2_DMAR_PROTECT_EN                                 0x0040
        #define CFG3_73_FILEIN3_DMAR_PROTECT_EN                                 0x0080
        #define CFG3_73_MMFI0_DMAR_PROTECT_EN                                   0x0400
        #define CFG3_73_MMFI1_DMAR_PROTECT_EN                                   0x0800
        #define CFG3_73_FILEIN0_ILLEGAL_ADDR_0                                  0x1000
        #define CFG3_73_FILEIN1_ILLEGAL_ADDR_0                                  0x2000
        #define CFG3_73_FILEIN2_ILLEGAL_ADDR_0                                  0x4000
        #define CFG3_73_FILEIN3_ILLEGAL_ADDR_0                                  0x8000
    REG16  CFG3_74;
        #define CFG3_74_MMFI0_ILLEGAL_ADDR_0                                    0x0004
        #define CFG3_74_MMFI1_ILLEGAL_ADDR_0                                    0x0008
        #define CFG3_74_FILEIN0_ILLEGAL_MIU_NS_EN                               0x0010
        #define CFG3_74_FILEIN1_ILLEGAL_MIU_NS_EN                               0x0020
        #define CFG3_74_FILEIN2_ILLEGAL_MIU_NS_EN                               0x0040
        #define CFG3_74_FILEIN3_ILLEGAL_MIU_NS_EN                               0x0080
        #define CFG3_74_MMFI0_ILLEGAL_MIU_NS_EN                                 0x0400
        #define CFG3_74_MMFI1_ILLEGAL_MIU_NS_EN                                 0x0800
        #define CFG3_74_DIS_FILEIN0_ADDR_LEN_BY_TEE                             0x1000
        #define CFG3_74_DIS_FILEIN1_ADDR_LEN_BY_TEE                             0x2000
        #define CFG3_74_DIS_FILEIN2_ADDR_LEN_BY_TEE                             0x4000
        #define CFG3_74_DIS_FILEIN3_ADDR_LEN_BY_TEE                             0x8000
    REG16  CFG3_75;
        #define CFG3_75_DIS_MMFI0_ADDR_LEN_BY_TEE                               0x0004
        #define CFG3_75_DIS_MMFI1_ADDR_LEN_BY_TEE                               0x0008
    REG16  CFG3_76_7B[6];
    REG16  CFG3_7C;
        #define CFG3_7C_PRIVILEGE_FLAG                                          0x0001
} REG_Ctrl3;

//@TODO There is FIQ Bank in TSP6 bank
//TSP 6
typedef struct _REG_Ctrl4
{
    REG16   CFG4_00_53[84];
    REG16   CFG4_54;
        #define CFG4_54_RVU_PSI_EN2                                             0x0001
        #define CFG4_54_RVU_TEI_EN2                                             0x0002
        #define CFG4_54_RVU_ERR_CLR2                                            0x0004
        #define CFG4_54_RVU_EN2                                                 0x0008
        #define CFG4_54_RVU_TIMESTAMP_EN2                                       0x0010
        #define CFG4_54_RVU_PID_12_TIE_0_EN2                                    0x0020
        #define CFG4_54_PAYLOAD_128_MODE_EN2                                    0x0040

        #define CFG4_54_RVU_PSI_EN3                                             0x0100
        #define CFG4_54_RVU_TEI_EN3                                             0x0200
        #define CFG4_54_RVU_ERR_CLR3                                            0x0400
        #define CFG4_54_RVU_EN3                                                 0x0800
        #define CFG4_54_RVU_TIMESTAMP_EN3                                       0x1000
        #define CFG4_54_RVU_PID_12_TIE_0_EN3                                    0x2000
        #define CFG4_54_PAYLOAD_128_MODE_EN3                                    0x4000
    REG16   CFG4_55;
        #define CFG4_55_RVU_PSI_EN4                                             0x0001
        #define CFG4_55_RVU_TEI_EN4                                             0x0002
        #define CFG4_55_RVU_ERR_CLR4                                            0x0004
        #define CFG4_55_RVU_EN4                                                 0x0008
        #define CFG4_55_RVU_TIMESTAMP_EN4                                       0x0010
        #define CFG4_55_RVU_PID_12_TIE_0_EN4                                    0x0020
        #define CFG4_55_PAYLOAD_128_MODE_EN4                                    0x0040
}REG_Ctrl4;

//TSP 7
typedef struct _REG_Ctrl5
{
    REG16   CFG5_00;
    REG16   CFG5_01;
    REG16   CFG5_02;
    REG16   CFG5_03;
    REG16   CFG5_04;
    REG16   Drop_Dis_PKT_Cnt_0;
    REG16   Drop_Dis_PKT_Cnt_1;
    REG16   Drop_Dis_PKT_Cnt_2;
    REG16   Drop_Dis_PKT_Cnt_3;
    REG16   CFG5_09;
    REG16   CFG5_0A;
    REG16   CFG5_0B;
    REG16   CFG5_0C;
    REG16   Locked_PKT_Cnt;                                                    //0x0D :   reg_locked_pkt_cnt
    REG16   Av_PKT_Cnt;                                                        //0x0E :   aud_pkt /vid_pkt
    REG16   CFG5_0F_16[8];
    REG16   Av_PKT_Cnt1;                                                       //0x17 :   aud_b_pkt /vid_3d_pkt
    REG16   CFG5_18;
    REG16   Err_PKT_Cnt;                                                       //0x19 :   reg_err_pkt_cnt
    REG16   CFG5_1A_1C[3];
    REG16   Input_PKT_Cnt;                                                     //0x1D :  reg_input_pkt_cnt
    REG16   CFG5_1E_6F[82];
    REG16   CFG5_70;
        #define CFG5_70_ERR_PKT_SRC_SEL_SHIFT                                   0
        #define CFG5_70_ERR_PKT_SRC_SEL_MASK                                    0x0007
        #define CFG5_70_INPUT_PKT_SRC_SEL_SHIT                                  3
        #define CFG5_70_INPUT_PKT_SRC_SEL_MASK                                  0x0038
    REG16   CFG5_71;
        #define CFG5_71_ERR_PKT_CNT_0_LOAD                                      0x0001
        #define CFG5_71_ERR_PKT_CNT_1_LOAD                                      0x0002
        #define CFG5_71_ERR_PKT_CNT_2_LOAD                                      0x0004
        #define CFG5_71_ERR_PKT_CNT_3_LOAD                                      0x0008
        #define CFG5_71_INPUT_PKT_CNT_0_LOAD                                    0x0100
        #define CFG5_71_INPUT_PKT_CNT_1_LOAD                                    0x0200
        #define CFG5_71_INPUT_PKT_CNT_2_LOAD                                    0x0400
        #define CFG5_71_INPUT_PKT_CNT_3_LOAD                                    0x0800
    REG16   CFG5_72;
        #define CFG5_72_ERR_PKT_CNT_0_CLR                                       0x0001
        #define CFG5_72_ERR_PKT_CNT_1_CLR                                       0x0002
        #define CFG5_72_ERR_PKT_CNT_2_CLR                                       0x0004
        #define CFG5_72_ERR_PKT_CNT_3_CLR                                       0x0008
        #define CFG5_72_INPUT_PKT_CNT_0_CLR                                     0x0100
        #define CFG5_72_INPUT_PKT_CNT_1_CLR                                     0x0200
        #define CFG5_72_INPUT_PKT_CNT_2_CLR                                     0x0400
        #define CFG5_72_INPUT_PKT_CNT_3_CLR                                     0x0800
    REG16   Av_PKT_Cnt2;                                                        //0x73 :   vid_3_pkt /vid_4_pkt
    REG16   CFG5_74;
        #define CFG5_74_V3_PKT_CNT_LOAD                                         0x0001
        #define CFG5_74_V3_PKT_CNT_CLR                                          0x0002
        #define CFG5_74_DROP_PKT_CNT_V3_LOAD                                    0x0004
        #define CFG5_74_DROP_PKT_CNT_V3_CLR                                     0x0008
        #define CFG5_74_DIS_PKT_CNT_V3_LOAD                                     0x0010
        #define CFG5_74_DIS_PKT_CNT_V3_CLR                                      0x0020
    REG16   CFG5_75;
        #define CFG5_75_V4_PKT_CNT_LOAD                                         0x0001
        #define CFG5_75_V4_PKT_CNT_CLR                                          0x0002
        #define CFG5_75_DROP_PKT_CNT_V4_LOAD                                    0x0004
        #define CFG5_75_DROP_PKT_CNT_V4_CLR                                     0x0008
        #define CFG5_75_DIS_PKT_CNT_V4_LOAD                                     0x0010
        #define CFG5_75_DIS_PKT_CNT_V4_CLR                                      0x0020
    REG16   CFG5_76;
    REG16   CFG5_77;
        #define CFG5_77_PIDFLT_SRC_SEL2_SHIFT                                   0      //pkt dmx 2
        #define CFG5_77_PIDFLT_SRC_SEL2_MASK                                    0x0007
        #define CFG5_77_PIDFLT_SRC_SEL3_SHIFT                                   3      //pkt dmx 3
        #define CFG5_77_PIDFLT_SRC_SEL3_MASK                                    0x0038
    REG16   CFG5_78;
        #define CFG5_78_AUDC_SRC_MASK                                           0x0007
        #define CFG5_78_AUDC_SRC_SHIFT                                          0
        #define CFG5_78_AUDD_SRC_MASK                                           0x0038
        #define CFG5_78_AUDD_SRC_SHIFT                                          3
        #define CFG5_78_PIDFLT_SRC_SEL_MMFI0_SHIFT                              6      // MMFI0
        #define CFG5_78_PIDFLT_SRC_SEL_MMFI0_MASK                               0x01C0
        #define CFG5_78_PIDFLT_SRC_SEL_MMFI1_SHIFT                              9      // MMFI1
        #define CFG5_78_PIDFLT_SRC_SEL_MMFI1_MASK                               0x0E00
        #define CFG5_78_VID_4_SRC_MASK                                          0x7000
        #define CFG5_78_VID_4_SRC_SHIFT                                         12
    REG16   CFG5_79;
    REG16   CFG5_7A;
        #define CFG5_7A_LOCKED_PKT_CNT_0_LOAD                                   0x0001
        #define CFG5_7A_LOCKED_PKT_CNT_1_LOAD                                   0x0002
        #define CFG5_7A_LOCKED_PKT_CNT_2_LOAD                                   0x0004
        #define CFG5_7A_LOCKED_PKT_CNT_3_LOAD                                   0x0008
        #define CFG5_7A_A_PKT_CNT_LOAD                                          0x0100
        #define CFG5_7A_V_PKT_CNT_LOAD                                          0x0200
        #define CFG5_7A_AD_PKT_CNT_LOAD                                         0x0400
        #define CFG5_7A_V3D_PKT_CNT_LOAD                                        0x0800
        #define CFG5_7A_ADC_PKT_CNT_LOAD                                        0x1000
        #define CFG5_7A_ADD_PKT_CNT_LOAD                                        0x2000
    REG16   CFG5_7B;
        #define CFG5_7B_DROP_PKT_CNT_V_LOAD                                     0x0001
        #define CFG5_7B_DROP_PKT_CNT_V3D_LOAD                                   0x0002
        #define CFG5_7B_DROP_PKT_CNT_A_LOAD                                     0x0004
        #define CFG5_7B_DROP_PKT_CNT_AD_LOAD                                    0x0008
        #define CFG5_7B_DROP_PKT_CNT_ADC_LOAD                                   0x0010
        #define CFG5_7B_DROP_PKT_CNT_ADD_LOAD                                   0x0020
        #define CFG5_7B_DIS_PKT_CNT_V_LOAD                                      0x0100
        #define CFG5_7B_DIS_PKT_CNT_V3D_LOAD                                    0x0200
        #define CFG5_7B_DIS_PKT_CNT_A_LOAD                                      0x0400
        #define CFG5_7B_DIS_PKT_CNT_AD_LOAD                                     0x0800
        #define CFG5_7B_DIS_PKT_CNT_ADC_LOAD                                    0x1000
        #define CFG5_7B_DIS_PKT_CNT_ADD_LOAD                                    0x2000
    REG16   CFG5_7C;
        #define CFG5_7C_LOCKED_PKT_CNT_0_CLR                                    0x0001
        #define CFG5_7C_LOCKED_PKT_CNT_1_CLR                                    0x0002
        #define CFG5_7C_LOCKED_PKT_CNT_2_CLR                                    0x0004
        #define CFG5_7C_LOCKED_PKT_CNT_3_CLR                                    0x0008
        #define CFG5_7C_A_PKT_CNT_CLR                                           0x0100
        #define CFG5_7C_V_PKT_CNT_CLR                                           0x0200
        #define CFG5_7C_AD_PKT_CNT_CLR                                          0x0400
        #define CFG5_7C_V3D_PKT_CNT_CLR                                         0x0800
        #define CFG5_7C_ADC_PKT_CNT_CLR                                         0x1000
        #define CFG5_7C_ADD_PKT_CNT_CLR                                         0x2000
    REG16   CFG5_7D;
        #define CFG5_7D_DROP_PKT_CNT_V_CLR                                      0x0001
        #define CFG5_7D_DROP_PKT_CNT_V3D_CLR                                    0x0002
        #define CFG5_7D_DROP_PKT_CNT_A_CLR                                      0x0004
        #define CFG5_7D_DROP_PKT_CNT_AD_CLR                                     0x0008
        #define CFG5_7D_DROP_PKT_CNT_ADC_CLR                                    0x0010
        #define CFG5_7D_DROP_PKT_CNT_ADD_CLR                                    0x0020
        #define CFG5_7D_DIS_PKT_CNT_V_CLR                                       0x0100
        #define CFG5_7D_DIS_PKT_CNT_V3D_CLR                                     0x0200
        #define CFG5_7D_DIS_PKT_CNT_A_CLR                                       0x0400
        #define CFG5_7D_DIS_PKT_CNT_AD_CLR                                      0x0800
        #define CFG5_7D_DIS_PKT_CNT_ADC_CLR                                     0x1000
        #define CFG5_7D_DIS_PKT_CNT_ADD_CLR                                     0x2000
    REG16   CFG5_7E;
        #define CFG5_7E_AUDA_SRC_MASK                                           0x0007
        #define CFG5_7E_AUDA_SRC_SHIFT                                          0
        #define CFG5_7E_VID_SRC_MASK                                            0x0038
        #define CFG5_7E_VID_SRC_SHIFT                                           3
        #define CFG5_7E_AUDB_SRC_MASK                                           0x01C0
        #define CFG5_7E_AUDB_SRC_SHIFT                                          6
        #define CFG5_7E_VID_3D_SRC_MASK                                         0x0E00
        #define CFG5_7E_VID_3D_SRC_SHIFT                                        9
        #define CFG5_7E_VID_3_SRC_MASK                                          0x7000
        #define CFG5_7E_VID_3_SRC_SHIFT                                         12
        #define AV_PKT_SRC_PKTDMX0                      0x0000
        #define AV_PKT_SRC_PKTDMX1                      0x0001
        #define AV_PKT_SRC_PKTDMX2                      0x0002
        #define AV_PKT_SRC_PKTDMX3                      0x0003
        #define AV_PKT_SRC_PKTDMX4                      0x0004
        #define AV_PKT_SRC_PKTDMX5                      0x0005
        #define AV_PKT_SRC_MMFI0                        0x0006
        #define AV_PKT_SRC_MMFI1                        0x0007

    REG16   CFG5_7F;
        #define CFG5_7F_DROP_PKT_MODE                                           0x0002 //choose the source of the reg_pkt_cnt   0: dis_cont_pkt      1: drop_pkt_cnt
        #define CFG5_7F_PIDFLT_SRC_SEL_SHIFT                                    2      //pkt dmx 0
        #define CFG5_7F_PIDFLT_SRC_SEL_MASK                                     0x001C
        #define CFG5_7F_TSIF_SRC_SEL_SHIFT                                      5
        #define CFG5_7F_TSIF_SRC_SEL_MASK                                       0x00E0
        #define TSIF_SRC_SEL_TSIF0                                              0x000
        #define TSIF_SRC_SEL_TSIF1                                              0x001
        #define TSIF_SRC_SEL_TSIF2                                              0x002
        #define TSIF_SRC_SEL_TSIF3                                              0x003

        #define CFG5_7F_AV_PKT_SRC_SEL                                          0x0100 //choose the source of the Av_PKT_Cnt   0 : vid_pkt_cnt/vid_3d_pkt_cnt      1 : aud_pkt_cnt/aud_b_pkt_cnt
        #define CFG5_7F_CLR_SRC_SHIFT                                           9
        #define CFG5_7F_CLR_SRC_MASK                                            0x0E00
        #define CFG5_7F_CLR_SRC_PKTDMX0                                         0x0000
        #define CFG5_7F_CLR_SRC_PKTDMX1                                         0x0001
        #define CFG5_7F_CLR_SRC_PKTDMX2                                         0x0002
        #define CFG5_7F_CLR_SRC_PKTDMX3                                         0x0003
        #define CFG5_7F_CLR_SRC_PKTDMX4                                         0x0004
        #define CFG5_7F_CLR_SRC_PKTDMX5                                         0x0005
        #define CFG5_7F_CLR_SRC_MMFI0                                           0x0006
        #define CFG5_7F_CLR_SRC_MMFI1                                           0x0007

        #define CFG5_7F_PIDFLT_SRC_SEL1_SHIFT                                   13      //pkt dmx 1
        #define CFG5_7F_PIDFLT_SRC_SEL1_MASK                                    0xE000
        #define DIS_DROP_CNT_V                                                  0
        #define DIS_DROP_CNT_V3D                                                1
        #define DIS_DROP_CNT_A                                                  2
        #define DIS_DROP_CNT_AD                                                 3
        #define DIS_DROP_CNT_ADC                                                4
        #define DIS_DROP_CNT_ADD                                                5
        #define DIS_DROP_CNT_V3                                                 6
        #define DIS_DROP_CNT_V4                                                 7

} REG_Ctrl5;

//TSP 8
typedef struct _REG_Ctrl6
{
    REG16   SyncByte_tsif0[4];                                                  //0x00~0x03
        #define TSP_SYNC_BYTE_MAASK0                                            0x00FF //byte 0
        #define TSP_SYNC_BYTE_MAASK1                                            0xFF00 //byte 1
        #define TSP_SYNC_BYTE_SHIFT0                                            0
        #define TSP_SYNC_BYTE_SHIFT1                                            8
    REG16   SourceId_tsif0[2];                                                  //0x04~0x05
        #define TSP_SRCID_MASK0                                                 0x000F //soruce 0
        #define TSP_SRCID_MASK1                                                 0x00F0 //soruce 1
        #define TSP_SRCID_MASK2                                                 0x0F00 //soruce 2
        #define TSP_SRCID_MASK3                                                 0xF000 //soruce 3
        #define TSP_SRCID_SHIFT0                                                0
        #define TSP_SRCID_SHIFT1                                                4
        #define TSP_SRCID_SHIFT2                                                8
        #define TSP_SRCID_SHIFT3                                                12
    REG16   SyncByte_tsif1[4];                                                  //0x06~0x09
    REG16   SourceId_tsif1[2];                                                  //0x0a~0x0b
    REG16   SyncByte_tsif2[4];                                                  //0x0c~0x0f
    REG16   SourceId_tsif2[2];                                                  //0x10~0x11
    REG16   SyncByte_tsif3[4];                                                  //0x12~0x15
    REG16   SourceId_tsif3[2];                                                  //0x16~0x17
    REG16   CFG6_18_23[12];                                                     //0x18~0x23
    REG16   pkt_converter[4];                                                   //0x24~0x27
        #define TSP_PKT_CONVERTER_MODE_MASK                                     0x0007
        #define TSP_PKT_188Mode                                                 0
        #define TSP_PKT_CIMode                                                  1
        #define TSP_PKT_OpenCableMode                                           2
        #define TSP_PKT_ATSMode                                                 3
        #define TSP_PKT_MxLMode                                                 4
        #define TSP_PKT_NagraDongleMode                                         5
        #define TSP_PKT_FORCE_SYNC_47                                           0x0008
        #define TSP_BYPASS_PKT_CONVERTER                                        0x0010
        #define TSP_BYPASS_SRC_ID_PARSER                                        0x0020
        #define TSP_SRC_ID_FLT_EN                                               0x0040
        #define TSP_MXL_PKT_HEADER_MASK                                         0x0F80 //add pkt num
        #define TSP_MXL_PKT_HEADER_SHIFT                                        7
    REG16   CFG6_28_29[2];
    REG16   CFG6_2A;
        #define CLR_PKT_CONVERTER_OVERFLOW                                      0x0001
        #define TSP_TSIF0_TSO_BLK_EN                                            0x0002
        #define TSP_TSIF0_TS1_BLK_EN                                            0x0004
        #define TSP_TSIF0_TS2_BLK_EN                                            0x0008
        #define TSP_TSIF0_TS3_BLK_EN                                            0x0010
        #define FIXED_TIMESTAMP_RING_BACK_EN                                    0x0080
        #define FIXED_LPCR_RING_BACK_EN                                         0x0400
        #define FIXED_VQ_MIU_REG_FLUSH                                          0x2000
    REG16   CFG6_2B;
        #define TSP_RESET_WB_DMA_FSM_TSIF1                                      0x0001
        #define TSP_RESET_WB_DMA_FSM_TSIF2                                      0x0002
        #define TSP_RESET_WB_DMA_FSM_TSIF3                                      0x0004
        #define TSP_RESET_WB_DMA_FSM_TSIF4                                      0x0008
        #define TSP_ECO_FIQ_INPUT                                               0x0100
        #define TSP_ECO_TS_SYNC_OUT_DELAY                                       0x0200
        #define TSP_ECO_TS_SYNC_OUT_REVERSE_BLOCK                               0x0400
        #define TSP_FIX_FILTER_NULL_PKT                                         0x4000
    REG16   CFG6_2C_2F[4];
    REG32   CFG6_30_31;                                                         // filein0 lower DMA read bound
    REG32   CFG6_32_33;                                                         // filein0 upper DMA read bound
    REG32   CFG6_34_35;                                                         // filein1 lower DMA read bound
    REG32   CFG6_36_37;                                                         // filein1 upper DMA read bound
    REG32   CFG6_38_39;                                                         // filein2 lower DMA read bound
    REG32   CFG6_3A_3B;                                                         // filein2 upper DMA read bound
    REG32   CFG6_3C_3D;                                                         // filein3 lower DMA read bound
    REG32   CFG6_3E_3F;                                                         // filein3 upper DMA read bound
        #define TSP_FILEIN_DMAR_BND_MASK                                        0x0FFFFFFFUL
    REG16   CFG6_40_47[8];                                                      // @Not used
    REG32   CFG6_48_49;                                                         // mmfi0 lower DMA read bound
    REG32   CFG6_4A_4B;                                                         // mmfi0 upper DMA read bound
    REG32   CFG6_4C_4D;                                                         // mmfi1 lower DMA read bound
    REG32   CFG6_4E_4F;                                                         // mmfi1 upper DMA read bound
        #define TSP_MMFI_DMAR_BND_MASK                                          0x0FFFFFFFUL
    REG32   CFG6_50_51;                                                         // initial packet timestamp value (tsif0)
    REG32   CFG6_52_53;                                                         // initial packet timestamp value (tsif1)
    REG32   CFG6_54_55;                                                         // initial packet timestamp value (tsif2)
    REG32   CFG6_56_57;                                                         // initial packet timestamp value (tsif3)
    REG16   CFG6_58_5B[4];                                                      // @Not used
    REG32   CFG6_5C_5D;                                                         // initial packet timestamp value (mmfi0)
    REG32   CFG6_5E_5F;                                                         // initial packet timestamp value (mmfi1)
    REG16   CFG6_60;
        #define TSP_INIT_TIMESTAMP_RESTART_EN      0x0008
} REG_Ctrl6;

//TSP9
typedef struct _REG_Ctrl7
{
    REG16   CFG7_00_03[4];                                                      //SPD CTR mode counter IV
    REG16   CFG7_04;                                                            //SPD CTR mode IV MAX (FILEIN)
        #define CFG7_04_CTR_IV_SPD_MAX_1K                                       0x0001
        #define CFG7_04_CTR_IV_SPD_MAX_2K                                       0x0002
        #define CFG7_04_CTR_IV_SPD_MAX_4K                                       0x0004
        #define CFG7_04_CTR_IV_SPD_MAX_8K                                       0x0008
        #define CFG7_04_CTR_IV_SPD_MAX_16K                                      0x0010
        #define CFG7_04_CTR_IV_SPD_MAX_32K                                      0x0020
        #define CFG7_04_CTR_IV_SPD_MAX_64K                                      0x0040
        #define CFG7_04_CTR_IV_SPD_MAX_128K                                     0x0080
    REG16   CFG7_05;                                                            //SPD CTR mode control (FILEIN)
        #define CFG7_05_CTR_MODE_SPD_FILEIN                                     0x0001
        #define CFG7_05_UPDATE_CTR_MODE_CNT_IV_SPD_FILEIN                       0x0002
        #define CFG7_05_LOAD_INIT_CNT_SPD                                       0x0004
        #define CFG7_05_SPD_ONEWAY                                              0x8000
    REG16   CFG7_06_0F[10];
} REG_Ctrl7;

//TSP10
typedef struct _REG_Ctrl8
{
    REG16   CFG8_00_03[4];                                                      //SPS CTR mode counter IV
    REG16   CFG8_04;                                                            //SPS CTR mode IV MAX (PVR 1)
        #define CFG8_04_CTR_IV_SPS_MAX_1K                                       0x0001
        #define CFG8_04_CTR_IV_SPS_MAX_2K                                       0x0002
        #define CFG8_04_CTR_IV_SPS_MAX_4K                                       0x0004
        #define CFG8_04_CTR_IV_SPS_MAX_8K                                       0x0008
        #define CFG8_04_CTR_IV_SPS_MAX_16K                                      0x0010
        #define CFG8_04_CTR_IV_SPS_MAX_32K                                      0x0020
        #define CFG8_04_CTR_IV_SPS_MAX_64K                                      0x0040
        #define CFG8_04_CTR_IV_SPS_MAX_128K                                     0x0080
    REG16   CFG8_05;                                                            //SPS CTR mode control (PVR 1)
        #define CFG8_05_CTR_MODE_SPS_PVR1                                       0x0001
        #define CFG8_05_UPDATE_CTR_MODE_CNT_IV_SPS_PVR1                         0x0002
        #define CFG8_05_LOAD_INIT_CNT_SPS1                                      0x0004
        #define CFG8_05_SPS_ONEWAY1                                             0x8000
    REG16   CFG8_06_0F[10];
} REG_Ctrl8;

typedef struct _REG_Ctrl8_1
{
    REG16   CFG8_40;                                                            //reg_hw10_config0
        #define CFG8_40_REG_VID3_SRC_MASK                                       0x0007
        #define CFG8_40_REG_VID3_SRC_SHIFT                                      0
        #define CFG8_40_VPES3_ERR_RM_EN                                         0x0008
        #define CFG8_40_DUP_PKT_SKIP_V3                                         0x0010
        #define CFG8_40_MASK_SCR_V3_EN                                          0x0020
        #define CFG8_40_PS_VID3_EN                                              0x0040
        #define CFG8_40_V3_BLOCK_DIS                                            0x0080
        #define CFG8_40_RESET_VFIFO_3                                           0x0100
    REG16   CFG8_41;                                                            //reg_hw10_config1
        #define CFG8_41_REG_VID4_SRC_MASK                                       0x0007
        #define CFG8_41_REG_VID4_SRC_SHIFT                                      0
        #define CFG8_41_VPES4_ERR_RM_EN                                         0x0008
        #define CFG8_41_DUP_PKT_SKIP_V4                                         0x0010
        #define CFG8_41_MASK_SCR_V4_EN                                          0x0020
        #define CFG8_41_PS_VID4_EN                                              0x0040
        #define CFG8_41_V4_BLOCK_DIS                                            0x0080
        #define CFG8_41_RESET_VFIFO_4                                           0x0100
    REG16   CFG8_42;
        //reg_pcr2_src
        #define CFG8_42_PCR2_SRC_MASK                                           0x000F
        #define CFG8_42_PCR2_SRC_SHIFT                                          0
        #define CFG8_42_PCR2_SRC_TSIF0                                          0x0
        #define CFG8_42_PCR2_SRC_TSIF1                                          0x1
        #define CFG8_42_PCR2_SRC_TSIF2                                          0x2
        #define CFG8_42_PCR2_SRC_TSIF3                                          0x3
        #define CFG8_42_PCR2_SRC_TSIF4                                          0x4
        #define CFG8_42_PCR2_SRC_TSIF5                                          0x5
        #define CFG8_42_PCR2_SRC_PKT_MERGE0                                     0x8
        #define CFG8_42_PCR2_SRC_PKT_MERGE1                                     0x9
        #define CFG8_42_PCR2_SRC_MM_FILEIN0                                     0xa
        #define CFG8_42_PCR2_SRC_MM_FILEIN1                                     0xb
        #define CFG8_42_PCR2_SRC_FIQ0                                           0xc
        #define CFG8_42_PCR2_SRC_FIQ1                                           0xd
        //reg_tei_skip_pkt_pcr2
        #define CFG8_42_REG_TEI_SKIP_PKT_PCR2                                   0x0010
        //reg_pcr2_id_sel
        #define CFG8_42_REG_PCR2_ID_SEL_MASK                                    0x00E0
        #define CFG8_42_REG_PCR2_ID_SEL_SHIFT                                   5
        //reg_pcr2_reset
        #define CFG8_42_REG_PCR2_RESET                                          0x0100
        //reg_pcr2_read
        #define CFG8_42_REG_PCR2_READ                                           0x0200
    REG16   CFG8_43;
        //reg_pcr3_src
        #define CFG8_43_PCR3_SRC_MASK                                           0x000F
        #define CFG8_43_PCR3_SRC_SHIFT                                          0
        #define CFG8_43_PCR3_SRC_TSIF0                                          0x0
        #define CFG8_43_PCR3_SRC_TSIF1                                          0x1
        #define CFG8_43_PCR3_SRC_TSIF2                                          0x2
        #define CFG8_43_PCR3_SRC_TSIF3                                          0x3
        #define CFG8_43_PCR3_SRC_TSIF4                                          0x4
        #define CFG8_43_PCR3_SRC_TSIF5                                          0x5
        #define CFG8_43_PCR3_SRC_PKT_MERGE0                                     0x8
        #define CFG8_43_PCR3_SRC_PKT_MERGE1                                     0x9
        #define CFG8_43_PCR3_SRC_MM_FILEIN0                                     0xa
        #define CFG8_43_PCR3_SRC_MM_FILEIN1                                     0xb
        #define CFG8_43_PCR3_SRC_FIQ0                                           0xc
        #define CFG8_43_PCR3_SRC_FIQ1                                           0xd
        //reg_tei_skip_pkt_pcr3
        #define CFG8_43_REG_TEI_SKIP_PKT_PCR3                                   0x0010
        //reg_pcr3_id_sel
        #define CFG8_43_REG_PCR3_ID_SEL_MASK                                    0x00E0
        #define CFG8_43_REG_PCR3_ID_SEL_SHIFT                                   5
        //reg_pcr3_reset
        #define CFG8_43_REG_PCR3_RESET                                          0x0100
        //reg_pcr3_read
        #define CFG8_43_REG_PCR3_READ                                           0x0200
    REG16   CFG8_44;                                                            //reg_pidflt_pcr2
        #define CFG8_44_PIDFLT_PCR2_PID_MASK                                    0x1FFF
        #define CFG8_44_PIDFLT_PCR2_EN                                          0x8000
    REG16   CFG8_45;                                                            //reg_pidflt_pcr3
        #define CFG8_45_PIDFLT_PCR3_PID_MASK                                    0x1FFF
        #define CFG8_45_PIDFLT_PCR3_EN                                          0x8000
    REG16   CFG8_46;
        //reg_pidflt_pcr2_scr_id
        #define CFG8_46_REG_PIDFLT_PCR2_SRC_ID_MASK                             0x000F
        #define CFG8_46_REG_PIDFLT_PCR2_SRC_ID_SHIFT                            0
        //reg_pidflt_pcr3_scr_id
        #define CFG8_46_REG_PIDFLT_PCR3_SRC_ID_MASK                             0x0F00
        #define CFG8_46_REG_PIDFLT_PCR3_SRC_ID_SHIFT                            8
    REG16   CFG8_47;                                                            //reg_hw10_config2
        //reg_ps_mode_src_a
        #define CFG8_47_REG_PS_MODE_SRC_A_MASK                                  0x0007
        #define CFG8_47_REG_PS_MODE_SRC_A_SHIFT                                 0
        //reg_ps_mode_src_ad
        #define CFG8_47_REG_PS_MODE_SRC_AD_MASK                                 0x0070
        #define CFG8_47_REG_PS_MODE_SRC_AD_SHIFT                                4
        //reg_ps_mode_src_ac
        #define CFG8_47_REG_PS_MODE_SRC_AC_MASK                                 0x0700
        #define CFG8_47_REG_PS_MODE_SRC_AC_SHIFT                                8
        //reg_ps_mode_src_add
        #define CFG8_47_REG_PS_MODE_SRC_ADD_MASK                                0x7000
        #define CFG8_47_REG_PS_MODE_SRC_ADD_SHIFT                               12
    REG16   CFG8_48;                                                            //reg_hw10_config3
        //reg_ps_mode_src_v
        #define CFG8_48_REG_PS_MODE_SRC_V_MASK                                  0x0007
        #define CFG8_48_REG_PS_MODE_SRC_V_SHIFT                                 0
        //reg_ps_mode_src_v3d
        #define CFG8_48_REG_PS_MODE_SRC_V3D_MASK                                0x0070
        #define CFG8_48_REG_PS_MODE_SRC_V3D_SHIFT                               4
        //reg_ps_mode_src_v3
        #define CFG8_48_REG_PS_MODE_SRC_V3_MASK                                 0x0700
        #define CFG8_48_REG_PS_MODE_SRC_V3_SHIFT                                8
        //reg_ps_mode_src_v4
        #define CFG8_48_REG_PS_MODE_SRC_V4_MASK                                 0x7000
        #define CFG8_48_REG_PS_MODE_SRC_V4_SHIFT                                12
    REG16   CFG8_49_4F[7];
    REG32   CFG8_50_51;                                                         //reg_synth0
    REG32   CFG8_52_53;                                                         //reg_synth1
    REG32   CFG8_54_55;                                                         //reg_pcr64_3_riu(Low)
    REG32   CFG8_56_57;                                                         //reg_pcr64_3_riu(High)
    REG32   CFG8_58_59;                                                         //reg_synth2
    REG32   CFG8_5A_5B;                                                         //reg_pcr64_4_riu(Low)
    REG32   CFG8_5C_5D;                                                         //reg_pcr64_4_riu(High)
    REG32   CFG8_5E_5F;                                                         //reg_synth3
    REG32   CFG8_60_61;                                                         //reg_pcr2_valid
    REG16   CFG8_62;                                                            //reg_pcr2_valid_33
    REG32   CFG8_63_64;                                                         //reg_pcr3_valid
    REG16   CFG8_65;                                                            //reg_pcr3_valid_33

} REG_Ctrl8_1;


#endif
