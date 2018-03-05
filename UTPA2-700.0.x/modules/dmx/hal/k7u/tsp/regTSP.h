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
    volatile MS_U16                 data;   //[jerry] not to name "low"
    volatile MS_U16                 _null;
} REG16;

typedef struct _TSP32
{
    volatile MS_U32                 reg32;
} TSP32;


//#########################################################################
//#### Hardware Capability Macro Start
//#########################################################################

#define TSP_TSIF_NUM                7
#define TSP_PVRENG_NUM              10
#define TSP_PVR_IF_NUM              7
#define TSP_OTVENG_NUM              8
#define STC_ENG_NUM                 8
#define TSP_PCRFLT_NUM              STC_ENG_NUM

#define TSP_PIDFLT_NUM              768
#define TSP_SECFLT_NUM              512
#define TSP_SECBUF_NUM              512

#define TSP_MERGESTREAM_NUM         32

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

    #define REG_CLKGEN0_TSO0_CLK                    0x27    // TSO #0
        #define REG_CLKGEN0_TSO0_SHIFT              0

    #define REG_CLKGEN0_TS0_CLK                     0x28
        #define REG_CLKGEN0_TS0_SHIFT               0
    #define REG_CLKGEN0_TS1_CLK                     0x28
        #define REG_CLKGEN0_TS1_SHIFT               8
    #define REG_CLKGEN0_TS2_CLK                     0x29
        #define REG_CLKGEN0_TS2_SHIFT               0
    #define REG_CLKGEN0_TS3_CLK                     0x29
        #define REG_CLKGEN0_TS3_SHIFT               8
    #define REG_CLKGEN0_TS4_CLK                     0x6B
        #define REG_CLKGEN0_TS4_SHIFT               0
    #define REG_CLKGEN0_TS5_CLK                     0x6B
        #define REG_CLKGEN0_TS5_SHIFT               8
    #define REG_CLKGEN0_TS6_CLK                     0x6C
        #define REG_CLKGEN0_TS6_SHIFT               0

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
        #define REG_CLKGEN0_TS_SRC_EXT7             0x0007
        #define REG_CLKGEN0_TS_SRC_EXT8             0x0008
        #define REG_CLKGEN0_TS_SRC_TSO0             0x0009
        #define REG_CLKGEN0_TS_SRC_TSO1             0x000A
        #define REG_CLKGEN0_TS_SRC_TSIO0            0x000B
        #define REG_CLKGEN0_TS_SRC_CILINK           0x000C
        //@NOTE Not support internal demod in K7U

    //get TSP Clk Gen bank
    #define REG_CLKGEN0_TSP_CLK                     0x2A
        #define REG_CLKGEN0_TSP_CLK_MASK            0x001F
        #define REG_CLKGEN0_TSP_SHIFT               0
        #define REG_CLKGEN0_TSP_DISABLE             0x0001
        #define REG_CLKGEN0_TSP_INVERT              0x0002
        //SRC
        #define REG_CLKGEN0_TSP_SRC_SHIFT           2
        #define REG_CLKGEN0_TSP_SRC_MASK            0x0007
        #define REG_CLKGEN0_TSP_SRC_288MHZ          0x0000
        #define REG_CLKGEN0_TSP_SRC_240MHZ          0x0001
        #define REG_CLKGEN0_TSP_SRC_216MHZ          0x0002
        #define REG_CLKGEN0_TSP_SRC_192MHZ          0x0003
        #define REG_CLKGEN0_TSP_SRC_172MHZ          0x0004
        #define REG_CLKGEN0_TSP_SRC_144MHZ          0x0005
        #define REG_CLKGEN0_TSP_SRC_108MHZ          0x0006
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
        #define REG_CLKGEN0_STC_SRC_ONE             0x0001  // reserved
        #define REG_CLKGEN0_STC_SRC_27M             0x0002
        #define REG_CLKGEN0_STC_SRC_XTAL            0x0003  // reserved

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
//#### CLKGEN1 Bank:0x1033
//#########################################################################
#define TSP_CLKGEN1_REG(addr)       (*((volatile MS_U16*)(_u32RegBase + 0x6600 + ((addr)<<2))))
    #define REG_CLKGEN1_DC0_SYTNTH                  0x5A
        #define REG_CLKGEN1_STC4_CW_SEL             0x0002
        #define REG_CLKGEN1_STC4_CW_EN              0x0004
        #define REG_CLKGEN1_STC5_CW_SEL             0x0200
        #define REG_CLKGEN1_STC5_CW_EN              0x0400

    #define REG_CLKGEN1_DC0_STC4_CW_L               0x5B
    #define REG_CLKGEN1_DC0_STC4_CW_H               0x5C
    #define REG_CLKGEN1_DC0_STC5_CW_L               0x5D
    #define REG_CLKGEN1_DC0_STC5_CW_H               0x5E

    #define REG_CLKGEN1_STC4_CLK                    0x5F
        #define REG_CLKGEN1_STC4_MASK               0x000F
        #define REG_CLKGEN1_STC4_SHIFT              0
    #define REG_CLKGEN1_STC5_CLK                    0x5F
        #define REG_CLKGEN1_STC5_MASK               0x00F0
        #define REG_CLKGEN1_STC5_SHIFT              4
        #define REG_CLKGEN1_STC_DISABLE             0x0001
        #define REG_CLKGEN1_STC_INVERT              0x0002
        //SRC
        #define REG_CLKGEN1_STC_SRC_SHIFT           2
        #define REG_CLKGEN1_STC_SRC_MASK            0x0003
        #define REG_CLKGEN1_STC_SRC_SYNTH           0x0000
        #define REG_CLKGEN1_STC_SRC_ONE             0x0001  // reserved
        #define REG_CLKGEN1_STC_SRC_27M             0x0002
        #define REG_CLKGEN1_STC_SRC_XTAL            0x0003  // reserved

    #define REG_CLKGEN1_DC1_SYTNTH                  0x6A
        #define REG_CLKGEN1_STC6_CW_SEL             0x0002
        #define REG_CLKGEN1_STC6_CW_EN              0x0004
        #define REG_CLKGEN1_STC7_CW_SEL             0x0200
        #define REG_CLKGEN1_STC7_CW_EN              0x0400

    #define REG_CLKGEN1_DC1_STC6_CW_L               0x6B
    #define REG_CLKGEN1_DC1_STC6_CW_H               0x6C
    #define REG_CLKGEN1_DC1_STC7_CW_L               0x6D
    #define REG_CLKGEN1_DC1_STC7_CW_H               0x6E

    #define REG_CLKGEN1_STC6_CLK                    0x6F
        #define REG_CLKGEN1_STC6_MASK               0x000F
        #define REG_CLKGEN1_STC6_SHIFT              0
    #define REG_CLKGEN1_STC7_CLK                    0x6F
        #define REG_CLKGEN1_STC7_MASK               0x00F0
        #define REG_CLKGEN1_STC7_SHIFT              4


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
        #define REG_CLKGEN2_STC_SRC_ONE             0x0001  // reserved
        #define REG_CLKGEN2_STC_SRC_27M             0x0002
        #define REG_CLKGEN2_STC_SRC_XTAL            0x0003  // reserved

//#########################################################################
//#### CHIPTOP Bank:0x101E
//#########################################################################
#define TSP_TOP_REG(addr)           (*((volatile MS_U16*)(_u32RegBase + 0x3c00UL + ((addr)<<2))))

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
    #define TSP_SECFLT_SECBUF_MASK                  0xFF000000          // [31:24] secbuf id (secbuf id low field)
    #define TSP_SECFLT_SECBUF_SHFT                  24
    #define TSP_SECFLT_SECBUF_MAX                   0x1FF               // software usage

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

    TSP32                           Ctrl_1;
    #define TSP_SECFLT_SECBUF_H_MASK                0x00000001          // secbuf id high field (bit[8])
    #define TSP_SECFLT_SECBUF_H_SHFT                0

    TSP32                           _x54[(0x80-0x54)/sizeof(TSP32)];    // (0x00211054-0x0021107F)/4

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

    REG32                           Pcr_L;                                   // 0xbf802a1c  0x07_0x08
    #define TSP_PCR64_L32_MASK                      0xFFFFFFFF
    REG32                           Pcr_H;                                   // 0xbf802a24  0x09_0x0a
    #define TSP_PCR64_H32_MASK                      0xFFFFFFFF               // PCR64 Middle 64

    REG16                           Mobf_Filein_Idx;                         // 0xbf802a2c   0x0b
    #define TSP_MOBF_FILEIN_MASK                    0x0000001F

    REG32                           _xbf202a2c;                              // 0xbf802a30   0x0c_0x0d

    REG32                           PVR2_Config;                             // 0xbf802a38   0x0e_0x0f
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

    REG32                           PVR2_LPCR1;                             // 0xbf802a40   0x10_0x11
    #define TSP_STR2MI2_ADDR_MASK  0x0FFFFFFF

    REG32                           Str2mi_head1_pvr2;                      // 0xbf802a48   0x12_0x13
    REG32                           Str2mi_mid1_wptr_pvr2;                  // 0xbf802a50   0x14_0x15
    REG32                           Str2mi_tail1_pvr2;                      // 0xbf802a58   0x16_0x17
    REG32                           Str2mi_head2_pvr2;                      // 0xbf802a60   0x18_0x19
    REG32                           Str2mi_mid2_pvr2;                       // 0xbf802a68   0x1a_0x1b, PVR2 mid address & write point
    REG32                           Str2mi_tail2_pvr2;                      // 0xbf802a70   0x1c_0x1d
    REG32                           Hw_SyncByte2;                           // 0xbf802a78   0x1e_0x1f
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
    #define TSP_SWINT_INFO_SEC_MASK                 0x00000FFF
    #define TSP_SWINT_INFO_SEC_SHFT                 0
    #define TSP_SWINT_INFO_ENG_MASK                 0x0000F000
    #define TSP_SWINT_INFO_ENG_SHFT                 12
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
    #define TSP_MATCH_PID_SRC_PKTDMX4               4
    #define TSP_MATCH_PID_SRC_PKTDMX5               5
    #define TSP_MATCH_PID_SRC_PKTDMX6               6

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
            6: DMA read done
            5: HK_INT_FORCE.            // it's trigger bit is at bank 15 39 bit[15]
            4: STR2MI_WADR meets STR2MI_MID.
            3: STR2MI_WADR meets STR2MI_TAIL.
            2: dma_status1
            1: dma_status2
            0: dma_status3
        */
    #define TSP_HWINT_EN_MASK                       0x00FF          // Tsp2hk_int enable bits.
    #define TSP_HWINT_EN_SHIFT                      0
    #define TSP_HWINT_DMA_STATUS3_EN                0x0001          // currently not used in ISR
    #define TSP_HWINT_DMA_STATUS2_EN                0x0002          // currently not used in ISR
    #define TSP_HWINT_DMA_STATUS1_EN                0x0004          // currently not used in ISR
    #define TSP_HWINT_TSP_FILEIN_MID_INT_EN         0x0008          // currently not used in ISR
    #define TSP_HWINT_TSP_FILEIN_TAIL_INT_EN        0x0010          // currently not used in ISR
    #define TSP_HWINT_TSP_SW_INT_EN                 0x0020
    #define TSP_HWINT_TSP_DMA_READ_DONE_EN          0x0040          // currently not used in ISR
    #define TSP_HWINT_PVR                           (TSP_HWINT_TSP_FILEIN_MID_INT_EN | TSP_HWINT_TSP_FILEIN_TAIL_INT_EN)
    #define TSP_HWINT_TSP_SUPPORT_ALL               (TSP_HWINT_TSP_SW_INT_EN)
    #define TSP_HWINT_ALL                           TSP_HWINT_TSP_SUPPORT_ALL

    #define TSP_HWINT_STATUS_MASK                   0xFF00
    #define TSP_HWINT_STATUS_SHIFT                  8
    #define TSP_HWINT_DMA_STATUS3_STATUS            0x0100
    #define TSP_HWINT_DMA_STATUS2_STATUS            0x0200
    #define TSP_HWINT_DMA_STATUS1_STATUS            0x0400
    #define TSP_HWINT_TSP_FILEIN_MID_INT_STATUS     0x0800
    #define TSP_HWINT_TSP_FILEIN_TAIL_INT_STATUS    0x1000
    #define TSP_HWINT_TSP_SW_INT_STATUS             0x2000
    #define TSP_HWINT_TSP_DMA_READ_DONE             0x4000

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
            [5] : OTV HW interrupt
            [4] : all DMA write address not in the protect zone interrupt
            [3] : vq0~vq6 overflow interrupt
            [2] : aud_err
            [1] : vid_err
            [0] : reg_hk_int_force (it's trigger bit is at bank 15 44 bit[15])
        */
    #define TSP_HWINT2_EN_MASK                              0x00FF
    #define TSP_HWINT2_EN_SHIFT                             0
    #define TSP_HWINT2_TSP_HK_INT_FORCE_EN                  0x0001      // currently not used in ISR
    #define TSP_HWINT2_TSP_VID_PKT_ERR_EN                   0x0002      // currently not used in ISR
    #define TSP_HWINT2_TSP_AUD_PKT_ERR_EN                   0x0004      // currently not used in ISR
    #define TSP_HWINT2_VQ0_TO_VQ6_OVERFLOW_EN               0x0008      // currently not used in ISR
    #define TSP_HWINT2_ALL_DMA_WADDR_NOT_IN_PROCT_Z_EN      0x0010      // currently not used in ISR
    #define TSP_HWINT2_OTV_EN                               0x0020
    #define TSP_HWINT2_SUPPORT_ALL                          0x0000
    #define TSP_HWINT2_ALL                                  TSP_HWINT2_SUPPORT_ALL

    #define TSP_HWINT2_STATUS_MASK                          0xFF00
    #define TSP_HWINT2_STATUS_SHIFT                         8
    #define TSP_HWINT2_TSP_HK_INT_FORCE_STATUS              0x0100
    #define TSP_HWINT2_TSP_VID_PKT_ERR                      0x0200
    #define TSP_HWINT2_TSP_AUD_PKT_ERR                      0x0400
    #define TSP_HWINT2_VQ0_TO_VQ6_OVERFLOW                  0x0800
    #define TSP_HWINT2_ALL_DMA_WADDR_NOT_IN_PROCT_Z         0x1000
    #define TSP_HWINT2_OTV                                  0x2000

    REG32                           SwInt2_Stat;                        // 0xbf802dB4   0x6d
    REG16                           HwInt3_Stat;                        // 0xbf802dBC   0x6f
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
    #define TSP_HWINT3_EN_MASK                              0x00FF
    #define TSP_HWINT3_EN_SHIFT                             0
    #define TSP_HWINT3_PCR0_UPDATE_END_EN                   0x0001
    #define TSP_HWINT3_PCR1_UPDATE_END_EN                   0x0002
    #define TSP_HWINT3_PCR2_UPDATE_END_EN                   0x0004
    #define TSP_HWINT3_PCR3_UPDATE_END_EN                   0x0008
    #define TSP_HWINT3_PCR4_UPDATE_END_EN                   0x0010
    #define TSP_HWINT3_PCR5_UPDATE_END_EN                   0x0020
    #define TSP_HWINT3_PCR6_UPDATE_END_EN                   0x0040
    #define TSP_HWINT3_PCR7_UPDATE_END_EN                   0x0080
    #define TSP_HWINT3_SUPPORT_ALL                          0x00FF
    #define TSP_HWINT3_ALL                                  TSP_HWINT3_SUPPORT_ALL

    #define TSP_HWINT3_STATUS_MASK                          0xFF00
    #define TSP_HWINT3_STATUS_SHIFT                         8
    #define TSP_HWINT3_PCR0_UPDATE_END                      0x0100
    #define TSP_HWINT3_PCR1_UPDATE_END                      0x0200
    #define TSP_HWINT3_PCR2_UPDATE_END                      0x0400
    #define TSP_HWINT3_PCR3_UPDATE_END                      0x0800
    #define TSP_HWINT3_PCR4_UPDATE_END                      0x1000
    #define TSP_HWINT3_PCR5_UPDATE_END                      0x2000
    #define TSP_HWINT3_PCR6_UPDATE_END                      0x4000
    #define TSP_HWINT3_PCR7_UPDATE_END                      0x8000

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
    #define TSP_DMA_FLUSH_EN                        0x0080        //PVR1, PVR2 dma flush
    #define TSP_REC_AT_SYNC_DIS                     0x0100
    #define TSP_PVR1_ALIGN_EN                       0x0200
    #define TSP_REC_FORCE_SYNC_EN                   0x0400
    #define TSP_RM_PKT_DEMUX_PIPE                   0x0800
    #define TSP_VQ_EN                               0x4000
    #define TSP_VQ2PINGPONG_EN                      0x8000

    REG16                           PVRConfig;                    // 0xbf802dCC  0x73
    #define TSP_PVR1_REC_ALL_EN                     0x0001
    #define TSP_PVR2_REC_ALL_EN                     0x0002
    #define TSP_REC_NULL                            0x0004
    #define TSP_REC_ALL_OLD                         0x0008
    #define TSP_MATCH_PID_SEL_MASK                  0x1F00
    #define TSP_MATCH_PID_SEL_SHIFT                 8
    #define TSP_MATCH_PID_LD                        0x8000

    REG32                           VQ3_Base;                     // 0x74~75
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


// TSP_MULTI
#include "regTSP_MULTI.h"

// TSP_SRC
#include "regTSP_SRC.h"

// LIVE-IN
#include "regPATH.h"

// FILE-IN
#include "regFILE.h"

// SPS/SPD
#include "regSPS_SPD.h"

// PVR
#include "regPVR.h"

// AV
#include "regAV.h"

// STC / PCR
#include "regSTC.h"

// Other
#include "regOTHER.h"

// TOP
#include "regTOP.h"

#endif
