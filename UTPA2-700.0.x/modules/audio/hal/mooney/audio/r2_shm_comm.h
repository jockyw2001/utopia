
#define THRESHOLD_READ_R2_SHM_FAIL_CNT                  100    

/****************************************************************************************************
*                                                                      Each R2 Decoder paramter
****************************************************************************************************/
enum
{
    DTS_TYPE_UNKNOWN    = 0,
    DTS_TYPE_CORE       = 1,
    DTS_TYPE_CORE_XLL   = 2,  
    DTS_TYPE_LBR        = 3,
    DTS_TYPE_XLL        = 4,
};
                
typedef struct
{
    MS_U32  drcMode;           //  0:line  0: RF more
    MS_U32  high_cut;          //0 - 100, 100 indicate 100%
    MS_U32  low_boost;         //0 - 100, 100 indicate 100%
    MS_U32  dmxMode;           //1: LtRt  0:LoRo
    MS_U32  gain;              //
    MS_U32  mul_frame_header;
    MS_U32  DDP_HDMI_bypass;
    MS_U32  AAC_HDMI_bypass;
    MS_U32  TB11_enable;      //Dolby Technical Bulletin 11 (Loudness matching)
    MS_U32  ddp_AD_substream;
    MS_S32  mixer_balance;
    MS_U32  DOLBY_PARAM_Reserved[8];
} AUR2_DOLBY_PARAM;

typedef struct
{
    MS_U32 asf_version;
    MS_U32 nChannels;
    MS_U32 smpRate;
    MS_U32 byteRate;
    MS_U32 blockAlign;
    MS_U32 encOpt;
    MS_U32 parsingByApp;
    MS_U32 bitsPerSmp;
    MS_U32 channelMask;
    MS_U32 drcParamExist;
    MS_U32 drcRmsAmpRef;
    MS_U32 drcRmsAmpTarget;    
    MS_U32 drcPeakAmpRef;
    MS_U32 drcPeakAmpTarget;
    MS_U32 maxPacketSize;
    MS_U32 u32AdvancedEncodeOpt;
    MS_U32 u32AdvancedEncodeOpt2;
    MS_U32 ASF_PARAM_Reserved[10];
} AUR2_ASF_PARAM;

typedef struct
{
    MS_U32 xPcmType;
    MS_U32 nChannels;
    MS_U32 smpRate;
    MS_U32 bitsPerSmp;
    MS_U32 blockSize;
    MS_U32 samplePerBlock;
    MS_U32 XPCM_PARAM_Reserved[5];
} AUR2_XPCM_PARAM;

typedef struct
{
    MS_U32 mNumCodecs;
    MS_U32 mSamples;
    MS_U32 mSampleRate;
    
    MS_U32 Channels[5];
    MS_U32 Regions[5];
    MS_U32 cplStart[5];
    MS_U32 cplQbits[5];
    MS_U32 FrameSize[5];
    MS_U32 COOK_PARAM_Reserved[5];
} AUR2_COOK_PARAM;

typedef struct
{
    MS_U32  drcMode;           //system certification used
    MS_U32  dmxLfeEnable;      //system certification used
    MS_U32  dialnormEnable;    //system certification used
    MS_U32  certMode;          //system certification used
    MS_U32  dmxMode;        
    MS_U32  DTSHD_HDMI_bypass;  
    MS_U32  seamless;          //0: no seamless
    MS_U32  DTS_PARAM_Reserved[10];
} AUR2_DTS_PARAM;

typedef struct
{
    MS_U32  MPEG_soundMode;
    MS_U32  gain;     
    MS_U32  MPEG_PARAM_Reserved[5];
} AUR2_MPEG_PARAM;

/****************************************************************************************************
*                                                                      Each R2 Decoder info
****************************************************************************************************/
typedef struct
{
    MS_U32  decType;          //DDP or DD,  
    MS_U32  frameSize;        //DDP frame ES size
    MS_U32  bsMode;           //DDP bsmode on bitstream 
    MS_U32  aac_Type;
    MS_U32  aac_ES_format;    
    MS_U32  DOLBY_INFO_Reserved[5];    //reserved for new items
} AUR2_DOLBY_INFO;

typedef struct
{
    MS_U32  cdMode;         //  
    MS_U32  dts_ES_frame_size;
    MS_U32  dts_ES_Type;
    MS_U32  dts_PCM_frame_size;
    MS_U32  DTS_INFO_Reserved[5];    //reserved for new items
} AUR2_DTS_INFO;

typedef struct
{
    MS_U32  MPEG_Layer;
    MS_U32  MPEG_stereoMode;
    MS_U32  MPEG_Header;  
    //MS_U32  gain;     
    MS_U32  MPEG_INFO_Reserved[5];    //reserved for new items
} AUR2_MPEG_INFO;

typedef struct
{
    MS_U32  SampleRate;       //input ES sampling rate
    MS_U32  spdif_info_flag;         //0: disable(init), 1: bypass/convert, 2: transcoder, 3: pcm
    MS_U32  hdmi_info_flag;          //0: disable(init), 1: bypass/convert, 2: transcoder, 3: pcm
    MS_U32  COMMON_INFO_Reserved[5];    //reserved for new items
} AUR2_COMMON_INFO;
/****************************************************************************************************
*                                                                 Common R2 Decoder paramter
****************************************************************************************************/
typedef struct
{
    /* each codec support max output channel */
    MS_U32     ch_cap_MEPG;
    MS_U32     ch_cap_AC3;
    MS_U32     ch_cap_AAC;
    MS_U32     ch_cap_DTS;
    MS_U32     ch_cap_DTS_HD;
    MS_U32     ch_cap_DTS_LBR;
    MS_U32     ch_cap_WMA;
    MS_U32     ch_cap_DRA;
    MS_U32     ch_cap_COOK;
    MS_U32     ch_cap_XPCM;
    MS_U32     ch_cap_VORBIS;
} AUR2_ADEC_Ch_INFO_SHM;

typedef struct
{
    /* decoder info */
    MS_U32  capability;               // update once
    MS_U32  decStatus;                // update per frame 
    MS_U32  dec_error_id;             // update per frame 
    MS_U32  mmFileReqSize;
    MS_U32  bootMagicID;              // identify whether this structure is initialized or not ?      // update once
    
    MS_U32  smpRate;                  // update per frame 
    MS_U32  spdifTx_smpRate;          // update per frame 
    MS_U32  hdmiTx_smpRate;           // update per frame 
    MS_U32  bitRate;                  // update per frame 
    MS_U32  synthRate;                // update per frame
    MS_U32  dec_ch_mode;              // update per frame 
    MS_U32  dec_ch_mapping;           // update per frame
                                            //        [15:8]: reserved
                                            //        [7]: right front
                                            //        [6]: left front
                                            //        [5]: LFE
                                            //        [4]: right surround
                                            //        [3]: left surround
                                            //        [2]: right
                                            //        [1]: left
                                            //        [0]: center 
    
    /* buffer level */
    MS_U32 PCM_Level;                 // udpate when change
    MS_U32 spdifbuf_Level;            // udpate when change
    
    /* AV sync info */
    MS_U32 STC;                       // [32:1]  skip bit 0
    MS_U32 PTS;                       // [32:1]  skip bit 0       // update per frame 
    MS_U32 currPTS;                   // [32:1]  skip bit 0   // update when change
    MS_S32 Td;                          // [32:1]  STC - APTS       // update when change

    MS_U32 MMTs_SyncFlag;             // update when change
    MS_U32 wait1stPtsCnt;             // update per sample
    MS_U32 waitStcCnt;                // update per sample
    MS_U32 smpFlushCnt;               // update per sample
    
    MS_U32 avSyncState;               // update per sample
    MS_U32 freeRunType;               // update per sample
    MS_U32 avSyncOffset;              // update per sample     

    MS_U32 ptsTagWRIdx;               // update per sample
    MS_U32 ptsTagRDIdx;               // update per sample
    
    /* dec status */
    MS_U32 playState;                 // update per sample
    MS_U32 decType;                   // update per sample
    MS_U32 playSmpFlag;               // update per sample
    MS_U32 decCallingCnt;             // update per sample
    MS_U32 receiveStopCnt;            // update per sample
    MS_U32 pcmBufEmptyCnt;

    /* frame info */
    MS_U32 ok_frmCnt;                 // update per frame 
    MS_U32 err_frmCnt;                // update per frame 
    MS_U32 skip_frmCnt;               // update per frame 
    MS_U32 rept_frmCnt;               // update per frame 
    MS_U32 syncMiss_Cnt;              // update per frame 
    
    /* buffer pointer */
    MS_U32 PCM_wrPtr;             //  byte pointer        // update when change       //range : 0 ~ PCM1_DRAM_SIZE
    MS_U32 pcmAddr;
    MS_U32 pcmSize;
    
    /* uniplayer */
    MS_U32 uni_decDone_cnt;                           
    MS_U32 uni_decDone_pcmAddr;                   //range:  0 ~ PCM1_DRAM_SIZE
    MS_U32 uni_decDone_pcmSize;                   //unit : bytes

    MS_U32 spdifbuf_wrPtr;                                //  byte pointer        // update when change       //range : 0 ~ PCM1_DRAM_SIZE
    MS_U32 spdifbuf_Addr;
    MS_U32 spdifbuf_Size;

    MS_U32 encRawBuf_wrPtr;	
    MS_U32 encRawBuf_Addr;	
    MS_U32 encRawBuf_Size;		

    MS_U32 hdmibuf_Level;                 // udpate when change
    MS_U32 hdmibuf_wrPtr;			 //  byte pointer		 // update when change		 //range : 0 ~ PCM1_DRAM_SIZE
    MS_U32 hdmibuf_Addr;
    MS_U32 hdmibuf_Size;    
    MS_U32 hdmi_hbr_mode;         //1 or 0

    /* PTS latency */
    MS_U32 pts_latency;

    AUR2_DOLBY_INFO  dolby_info;   
    AUR2_DTS_INFO    dts_info;
    AUR2_MPEG_INFO   mpeg_info;
    AUR2_COMMON_INFO    common_info;

    MS_U32 unsupport_type;
    MS_U32 syncword_count;
    MS_U32 dec_pcm_channel;

    MS_U32 PCM_DMX_Level;
    
    MS_U32 ADEC_INFO_SHM_Reserved[19];    //reserved for new items
} AUR2_ADEC_INFO_SHM;       // audio R2 report info to MCU / MM_AEON

//for spdif_type
    #define SPDIF_NONE      0
    #define SPDIF_PCM       1
    #define SPDIF_AUTO      2
//for OMX_SPDIF_Ctrl param
    #define OMX_SPDIF_MODE_EN_BIT               _BIT_0_ 
    #define OMX_SPDIF_PLAY_BIT                  _BIT_1_

typedef struct
{
    /* DTV avSync offset */
    MS_S32 dtv_mpg_avDelay;
    MS_S32 dtv_ac3_avDelay;
    MS_S32 dtv_aac_avDelay;

    /* DTV asoc avSync offset */
    MS_S32 dtv_asoc_mpg_avDelay;
    MS_S32 dtv_asoc_ac3_avDelay;
    MS_S32 dtv_asoc_aac_avDelay;

    /* external synthsizer control */
    MS_U32 extSynthsizer_en;
    MS_U32 extSynthsizer_value;

    /* AD volume */
    MS_U32 adMix_mainVol;
    MS_U32 adMix_asocVol;

    /* MM ctrl flag */
    MS_U32 mmTs_ctrl_flag;        //  MMTS control flag, ex: bit0: syncSTC ...
    MS_U32 mmUni_needDecodeFrmCnt;
    MS_U32 OMX_SPDIF_Ctrl;        //        [0]: SPDIF mode, [1]: SPDIF play        
    MS_U32 OMX_SPDIF_PCM_Level;
    MS_U32 fast_forward;
    MS_U32 spdif_type;   //NONE, PCM, AUTO
    MS_U32 hdmi_type;       //NONE, PCM, AUTO
    /* specific decoder param */
    AUR2_DOLBY_PARAM    dolby_param;
    AUR2_ASF_PARAM      asf_param;
    AUR2_XPCM_PARAM     xpcm_param;
    AUR2_COOK_PARAM     cook_param;
    AUR2_DTS_PARAM      dts_param; 
    AUR2_MPEG_PARAM     mpeg_param; 
    
    /* ES limiter */
    MS_U32     ES_Limiter_EN;
    MS_U32     ES_Limiter_Threshold;

    /* PCM limiter */
    MS_U32     PCM_Limiter_EN;
    MS_U32     PCM_Limiter_Threshold;
    
    /* general decoder control */
    MS_U32     mmFileReqSize;
    MS_U32     bootMagicID;               // identify whether this structure is initialized or not ?
    MS_U32     decCtrl;                   // decode control
                                          //        [10]: STC from TSP0/1  0: from TSP 0, 1: from TSP 1.
                                          //        [9]: Slow forward 0.5x
                                          //        [8]: Parser Spec flag  1:11172-1  , 0: 13818-1
                                          //        [7]: PES Bypass flag
                                          //        [6]: PUSI disable flag
                                          //        [5]: MMTS PTS report:  1 : report PTS with MSB, 0: report PTS without MSB
                                          //        [4]: MM TS sync STC
                                          //        [3]: Fast forward 2x
                                          //        [2]: decoder mute
                                          //        [1:0]: sound mode 
    MS_U32     DPGA_Disable_flag;   
    MS_U32     hashKey;                                       
    MS_U32     arib_mmd_mode;
    MS_U32     force_mmuni_mch_pcm;
    MS_U32     vorbis_header_size;
    MS_U32     TTS_On_flag;
    MS_U32     OMX_SPDIF_PAUSE;
    MS_U32     ADEC_PARAM_SHM_Reserved[19];    //reserved for new items
} AUR2_ADEC_PARAM_SHM;        // MCU set param to audio R2

typedef struct
{
    MS_U32 ES_Level;                  // update when change
    MS_U32 ES_rdPtr;              //  byte pointer        // update when change       //range : 0 ~ ES1_DRAM_SIZE
    MS_U32 ES_accumCnt;                // update per frame
    MS_U32 ES_INFO_Reserved[5];    //reserved for new items
} AUR2_ADEC_ES_INFO_SHM;

typedef struct
{
    MS_U32 MM_inputPTS;       //   input PTS
    MS_U32 ES_wrPtr;              //  byte pointer        //range : ES1_DRAM_ADDR ~ (ES1_DRAM_ADDR + ES1_DRAM_SIZE)    
    MS_U32 ES_PARAM_Reserved[5];    //reserved for new items
} AUR2_ADEC_ES_PARAM_SHM;

/****************************************************************************************************
*                                                                 Common R2 encoder paramter
****************************************************************************************************/
typedef struct
{
    MS_U32 acmod;
    MS_U32 lfe_on;
    MS_U32 sampleRate;

    /* pcm info from decoder */
    MS_U32 mchPcm_dram_offset;
    MS_U32 mchPcm_dram_size;
    MS_U32 mchPcm_dram_wrPtr;

    /* meta data from decoder */
    MS_U32 metaData_dram_offset;
    MS_U32 metaData_dram_size;
    MS_U32 metaData_dram_wrPtr;
    MS_U32 AENC_PARAM_Reserved[10];    //reserved for new items
} AUR2_AENC_PARAM;

/****************************************************************************************************
*                                                                 each Sound effect param
****************************************************************************************************/
