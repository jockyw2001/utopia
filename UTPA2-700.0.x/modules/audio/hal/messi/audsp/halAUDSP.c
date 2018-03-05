//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "ULog.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "drvBDMA.h"
#include "drvSERFLASH.h"

#include "regCHIP.h"

#include "halAUDSP.h"
#include "drvAUDIO.h"
#include "../audio/regAUDIO.h"
#include "../audio/halAUDIO.h"
#include "../audio/halSIF.h"
#include "../audio/halADVAUDIO.h"
#include "../audio/ddr_config.h"
#include "DecoderInfo.h"

#define LOAD_CODE_DEBUG  0

#define HALAUDSP_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MBOOT //mboot Speed up
  #define DSP_VERIFY_DSP_CODE     0   // 0: don't verify dsp code (for speed)
                                    // 1: verify dsp code (for debug)
  #define DSP_IDMA_CHK_READY      1   // 0: don't check IDMA ready (for speed)
                                    // 1: check IDMA ready (for debug)
#else
  #define DSP_VERIFY_DSP_CODE     1   // 0: don't verify dsp code (for speed)
                                    // 1: verify dsp code (for debug)
  #define DSP_IDMA_CHK_READY      1   // 0: don't check IDMA ready (for speed)
                                    // 1: check IDMA ready (for debug)
#endif

#define AU_NULL  0

LOAD_CODE_INFO g_loadcodeinfo;
MS_BOOL        g_bDSPLoadCode = FALSE;

#ifndef MSOS_TYPE_NOS
void* MDrv_MPool_PA2KSEG1(void* pAddrPhys);
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define LOU8(MS_U16Val)  ( (MS_U8)(MS_U16Val) )
#define HIU8(MS_U16Val)  ( (MS_U8)((MS_U16Val) >> 8) )

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_BOOL g_bAudio_loadcode_from_dram;
extern AUDIO_SHARED_VARS2 * g_AudioVars2;

extern MS_S32  _s32AUDIOMutexIDMA;

#ifndef MSOS_TYPE_NUTTX
extern AUDIO_TEE_INFO_SHARE_MEM   *pAudioTeeInfoShm;
#endif


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 g_u8DspCodeTypeLoaded = 0;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_DspLoadCode()
/// @brief \b Function \b Description:  load CM/PM/cache/prefetch DSP code
/// @param <IN>        \b \b u8Type    :      -- DSP load code type
///                                        DSP_segment -- select DSP load code seg
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_DspLoadCode(MS_U8 u8Type)
{
#ifndef MSOS_TYPE_NUTTX
   REE_TO_TEE_MBX_MSG_TYPE msg_type = REE_TO_TEE_MBX_MSG_NULL;
#endif
    MS_PHY MIU_addr;
    AUDIO_ALG_INFO *pCodeTableinfo=NULL;
    LOAD_CODE_ARCH_TYPE eArchType = LOAD_INVALID;

   HALAUDSP_CHECK_SHM_INIT;

    switch(u8Type)
    {

        case AU_DEC_SYSTEM:
            break;

        case AU_SE_SYSTEM:
            pCodeTableinfo = &SE_SYSTEM_LoadCodeTable[0];
            eArchType = LOAD_SYSTEM;
            break;

        case AU_DVB_DEC_NONE:
        case AU_DVB2_NONE:
            pCodeTableinfo = &DEC_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_MPEG:
        case AU_DVB2_STANDARD_MPEG_AD:
            pCodeTableinfo = &MPEG_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_AAC:
        case AU_DVB_STANDARD_MS10_DDT:
        case AU_DVB2_STANDARD_GAAC_AD:
            pCodeTableinfo = &GAAC_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_MS10_DDC:
        case AU_DVB_STANDARD_AC3:
        case AU_DVB_STANDARD_AC3P:
        case AU_DVB2_STANDARD_AC3_AD:
        case AU_DVB2_STANDARD_AC3P_AD:
            pCodeTableinfo = &AC3P_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_MP3:
        case AU_DVB2_STANDARD_MP3:
            pCodeTableinfo = &MP3_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_WMA:
        case AU_DVB_STANDARD_WMA_PRO:
            pCodeTableinfo = &WMA_PRO_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_XPCM:
        case AU_DVB2_STANDARD_XPCM:
            pCodeTableinfo = &XPCM_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_RA8LBR:
            pCodeTableinfo = &RA8LBR_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
           break;

        case AU_DVB_STANDARD_FLAC:
            pCodeTableinfo = &FLAC_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_VORBIS:
            pCodeTableinfo = &VORBIS_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_DTS:
        case AU_DVB2_STANDARD_DTS:
            pCodeTableinfo = &DTS_DMP_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_DTSLBR:
            pCodeTableinfo = &DTS_LBR_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_DRA:
            pCodeTableinfo = &DRAD_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_AMR_NB:
            pCodeTableinfo = &AMR_NB_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB_STANDARD_AMR_WB:
            pCodeTableinfo = &AMR_WB_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_SIF_PALSUM:
            pCodeTableinfo = &PALSUM_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;
            
        case AU_SIF_BTSC:
            pCodeTableinfo = &BTSC_LoadCodeTable[0];
            eArchType = LOAD_DECODER;
            break;

        case AU_DVB2_ADVSND_TSHD:
            pCodeTableinfo = &TSHD_LoadCodeTable[0];
            eArchType = LOAD_ADVSOUND;
            break;

        case AU_DVB2_ADVSND_SRS:
            pCodeTableinfo = &TSXT_LoadCodeTable[0];
            eArchType = LOAD_ADVSOUND;
            break;

        case AU_DVB2_ADVSND_PURESND:
            pCodeTableinfo = &PURESND_LoadCodeTable[0];
            eArchType = LOAD_ADVSOUND;
            break;

        case AU_DVB2_ADVSND_DBX:
            pCodeTableinfo = &DBX_LoadCodeTable[0];
            eArchType = LOAD_ADVSOUND;
            break;

        default : 
            eArchType = LOAD_INVALID;
            return FALSE;
    };

#ifndef MSOS_TYPE_NUTTX
    AUDIO_TEE_INFO_SHM_CHECK_NULL;
    REE_TO_TEE_MBX_MSG_SET_PARAM(0, (MS_U8)u8Type);
    REE_TO_TEE_MBX_MSG_SET_PARAM_COUNT(1);
    msg_type = REE_TO_TEE_MBX_MSG_HAL_AUDSP_DspLoadCode;
        msg_type = msg_type | REE_TO_TEE_MBX_MSG_TYPE_SE;

    if ( TEE_TO_REE_MBX_ACK_MSG_NO_TEE != (mbx_msg_ack_status = HAL_AUDIO_SendMBXMsg(msg_type)))
    {
        if (mbx_msg_ack_status != TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS)
            return FALSE;
        HAL_AUDSP_SetDspCodeTypeLoaded(u8Type);
        return TRUE;
    }
#endif

    if(pCodeTableinfo == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=======NULL return==========\r\n");
        return FALSE;                 
    }
    else if(pCodeTableinfo->cm_len == 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n========cm length =0=========\r\n");
        return FALSE;     
    }

    HAL_AUDSP_SetDspLoadCodeInfo(pCodeTableinfo, DSP_SE);
    HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x01);

//======== Load DSP PM ========
#if(LOAD_CODE_DEBUG)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PM addr: 0x%x\r\n", pCodeTableinfo->pm_addr);
    #if defined (__aarch64__)	 
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PM buf addr: 0x%lx\r\n", (MS_VIRT)pCodeTableinfo->pm_buf);
    #else
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PM buf addr: 0x%x\r\n", (MS_VIRT)pCodeTableinfo->pm_buf);
    #endif
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PM size: 0x%x\r\n",pCodeTableinfo->pm_len);
#endif

    if(!HAL_AUDSP_DspLoadCodeSegment(pCodeTableinfo->pm_addr, pCodeTableinfo->pm_buf+6, pCodeTableinfo->pm_len, DSP_SE))
        return FALSE;
    if(!HAL_AUDSP_DspVerifySegmentCode(pCodeTableinfo->pm_addr, pCodeTableinfo->pm_buf+6, pCodeTableinfo->pm_len, DSP_SE))
        return FALSE;

 //======== Load DSP Prefetch ======== 
#if(LOAD_CODE_DEBUG)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PreFetch PM addr: 0x%x\r\n", pCodeTableinfo->prefetch_addr);
    #if defined (__aarch64__)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PreFetch PM buf addr: 0x%lx\r\n", (MS_VIRT)pCodeTableinfo->prefetch_buf);
    #else
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PreFetch PM buf addr: 0x%x\r\n", (MS_VIRT)pCodeTableinfo->prefetch_buf);
    #endif
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    PreFetch PM size: 0x%x\r\n", pCodeTableinfo->prefetch_len);
#endif

    if(pCodeTableinfo->prefetch_len != 0)
    {
        MIU_addr = pCodeTableinfo->prefetch_addr * 3 + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
#if defined (__aarch64__)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    MIU of PreFetch: 0x%08lx\r\n", (unsigned int)(MS_VIRT)MIU_addr);
#else
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    MIU of PreFetch: 0x%08x\r\n", (unsigned int)(MS_VIRT)MIU_addr);
#endif
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((pCodeTableinfo->prefetch_buf+6)), pCodeTableinfo->prefetch_len);
    }

//======== Load DSP Cache ======== 
#if(LOAD_CODE_DEBUG)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    Cache PM addr: 0x%x\r\n", pCodeTableinfo->cache_addr);
    #if defined (__aarch64__) 		
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    Cache PM buf addr: 0x%lx\r\n", (MS_VIRT)pCodeTableinfo->cache_buf);
    #else
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    Cache PM buf addr: 0x%x\r\n", (MS_VIRT)pCodeTableinfo->cache_buf);
    #endif
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    Cache PM size: 0x%x\r\n", pCodeTableinfo->cache_len);
#endif

    if(pCodeTableinfo->cache_len != 0)
    {
        MIU_addr = pCodeTableinfo->cache_addr * 3 + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
#if defined (__aarch64__)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    MIU of Cache: 0x%08lx\r\n", (unsigned int)(MS_VIRT)MIU_addr);
#else
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    MIU of Cache: 0x%08x\r\n", (unsigned int)(MS_VIRT)MIU_addr);
#endif
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((pCodeTableinfo->cache_buf+6)), pCodeTableinfo->cache_len);
    }

//======== Load DSP CM ======== 
#if(LOAD_CODE_DEBUG)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    CM addr: 0x%x\r\n", pCodeTableinfo->cm_addr);
    #if defined (__aarch64__)
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    CM buf addr: 0x%lx\r\n", (MS_VIRT)pCodeTableinfo->cm_buf);
    #else	
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    CM buf addr: 0x%x\r\n", (MS_VIRT)pCodeTableinfo->cm_buf);
    #endif
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "    CM size: 0x%x\r\n", pCodeTableinfo->cm_len);
#endif

    if(eArchType==LOAD_SYSTEM)
    {
     // First 2 words will be loaded later    
        if(!HAL_AUDSP_DspLoadCodeSegment(0x02, pCodeTableinfo->cm_buf+12, pCodeTableinfo->cm_len-6, DSP_SE))
            return FALSE;
        if(!HAL_AUDSP_DspVerifySegmentCode(0x02, pCodeTableinfo->cm_buf+12, pCodeTableinfo->cm_len-6, DSP_SE))
            return FALSE;
        
    // When Load  SE-DSP  system code , write address 0  to reset DSP    
        if(!HAL_AUDSP_DspLoadCodeSegment(0x0001,  mst_codec_pm1+6+3, 21, DSP_SE)) return FALSE;
        if(!HAL_AUDSP_DspVerifySegmentCode(0x0001,  mst_codec_pm1+6+3, 21, DSP_SE)) return FALSE;
        if(!HAL_AUDSP_DspLoadCodeSegment(0x0000,  mst_codec_pm1+6, 3, DSP_SE)) return FALSE;
        if(!HAL_AUDSP_DspVerifySegmentCode(0x0000,  mst_codec_pm1+6, 3, DSP_SE)) return FALSE;   
    }
    else
    {
        if(!HAL_AUDSP_DspLoadCodeSegment(pCodeTableinfo->cm_addr, pCodeTableinfo->cm_buf+6, pCodeTableinfo->cm_len, DSP_SE))
            return FALSE;
        if(!HAL_AUDSP_DspVerifySegmentCode(pCodeTableinfo->cm_addr, pCodeTableinfo->cm_buf+6, pCodeTableinfo->cm_len, DSP_SE))
            return FALSE;
    }
        
    MsOS_FlushMemory();

    HAL_AUDSP_SetDspCodeTypeLoaded(u8Type);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "HAL_AUDSP_DspLoadCode finished(type=%s(0x%x))\r\n", pCodeTableinfo->AlgName, u8Type);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DspLoadCodeSegment()
/// @brief \b Function \b Description: This routine is used to load DSP code
/// @param <IN>        \b dsp_addr    :
/// @param <IN>        \b dspCode_buf    :
/// @param <IN>        \b dspCode_buflen    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  BOOL    :    TRUE --DSP Load code okay
///                                    FALSE--DSP Load code fail
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_DspLoadCodeSegment(MS_U32 dsp_addr, MS_U8  *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select)
{
    MS_U32 i,j;
    MS_U32 idma_wrbase_addr_l, dsp_brg_data_l, dsp_brg_data_h, bdma_mode_addr;

    HALAUDSP_CHECK_SHM_INIT;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    g_bDSPLoadCode = TRUE;

    if(DSP_select == DSP_DEC)
    {
        idma_wrbase_addr_l = REG_DEC_IDMA_WRBASE_ADDR_L;
        dsp_brg_data_l = REG_DEC_DSP_BRG_DATA_L;
        dsp_brg_data_h = REG_DEC_DSP_BRG_DATA_H;
        bdma_mode_addr = REG_DEC_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x00);
    }
    else
    {
        idma_wrbase_addr_l = REG_SE_IDMA_WRBASE_ADDR_L;
        dsp_brg_data_l = REG_SE_DSP_BRG_DATA_L;
        dsp_brg_data_h = REG_SE_DSP_BRG_DATA_H;
        bdma_mode_addr = REG_SE_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x01);
    }

    HAL_AUDIO_WriteMaskReg(bdma_mode_addr, 0x8080, 0x0000); // disable bdma

    if(dspCode_buflen>0)
    {
        /* set iDMA addr */

        HAL_AUDIO_WriteReg(idma_wrbase_addr_l, dsp_addr);

        AUDIO_DELAY1MS(1);

        for( i=0; i<dspCode_buflen; i+=3)
        {
            HAL_AUDIO_WriteByte(dsp_brg_data_l,*(dspCode_buf+i+1));
            HAL_AUDIO_WriteByte(dsp_brg_data_h,*(dspCode_buf+i+2));

            for(j=0;j<2;j++);//for delay only

            HAL_AUDIO_WriteByte(dsp_brg_data_l,*(dspCode_buf+i));
            HAL_AUDIO_WriteByte(dsp_brg_data_h,0x00);

            if(DSP_select == DSP_DEC)
            {
                if (HAL_AUDSP_CheckDecIdmaReady(AUD_CHK_DSP_WRITE_RDY)==FALSE)
                {
                    g_bDSPLoadCode = FALSE;
                    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);
                    return FALSE;
                }
            }
            else
            {
                if (HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_WRITE_RDY)==FALSE)
                {
                    g_bDSPLoadCode = FALSE;
                    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);
                    return FALSE;
                }
            }
        }
    }

    g_bDSPLoadCode = FALSE;

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    return TRUE;
}

MS_BOOL HAL_AUDSP_DspVerifySegmentCode(MS_U32 dsp_addr, MS_U8 *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select)
{
    MS_U32 i;
    MS_U8 dat[3];
    MS_U32 idma_rdbase_addr_l, idma_ctrl0, idma_rddata_h_0, idma_rddata_h_1, idma_rddata_l, bdma_mode_addr;

#if (DSP_VERIFY_DSP_CODE==0)
    return TRUE;                        //don't verify just return;
#endif

    HALAUDSP_CHECK_SHM_INIT;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    g_bDSPLoadCode = TRUE;

    if ( DSP_select == DSP_DEC )
    {
        idma_rdbase_addr_l = REG_DEC_IDMA_RDBASE_ADDR_L;
        idma_ctrl0 = REG_DEC_IDMA_CTRL0;
        idma_rddata_h_0 = REG_DEC_IDMA_RDDATA_H_0;
        idma_rddata_h_1 = REG_DEC_IDMA_RDDATA_H_1;
        idma_rddata_l = REG_DEC_IDMA_RDDATA_L;
        bdma_mode_addr = REG_DEC_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x00);
    }
    else
    {
        idma_rdbase_addr_l = REG_SE_IDMA_RDBASE_ADDR_L;
        idma_ctrl0 = REG_SE_IDMA_CTRL0;
        idma_rddata_h_0 = REG_SE_IDMA_RDDATA_H_0;
        idma_rddata_h_1 = REG_SE_IDMA_RDDATA_H_1;
        idma_rddata_l = REG_SE_IDMA_RDDATA_L;
        bdma_mode_addr = REG_SE_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x01);
    }

    //HAL_AUDIO_WriteMaskByte(0x3CEC, 0x40, 0x00);
    HAL_AUDIO_WriteMaskReg(bdma_mode_addr, 0x8080, 0x0000); // disable bdma (enable idma)
    HAL_AUDIO_WriteReg(idma_rdbase_addr_l, dsp_addr);
    AUDIO_DELAY1MS(1);

    for (i=0; i<dspCode_buflen; i+=3)
    {
        HAL_AUDIO_WriteMaskByte(idma_ctrl0, 0x08, 0x08 );
#if (DSP_IDMA_CHK_READY == 1)
     if(DSP_select == DSP_DEC)
        {
         if (HAL_AUDSP_CheckDecIdmaReady(AUD_CHK_DSP_READ_RDY)==FALSE)
             {
                g_bDSPLoadCode = FALSE;

                OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

                return FALSE;
             }
        }
        else
        {
            if (HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_READ_RDY)==FALSE)
            {
                g_bDSPLoadCode = FALSE;

                OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

                return FALSE;
            }
        }
#endif
        dat[1] = HAL_AUDIO_ReadByte(idma_rddata_h_0);
        dat[2] = HAL_AUDIO_ReadByte(idma_rddata_h_1);
        dat[0] = HAL_AUDIO_ReadByte(idma_rddata_l);

        //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "@@%x\n", (dat[2] << 16) | (dat[1] << 8) | (dat[0]));

        if ( (dat[0]!=dspCode_buf[i]) || (dat[1]!=dspCode_buf[i+1]) ||
                (dat[2]!=dspCode_buf[i+2]))
        {
#if defined (CONFIG_UTOPIA_ANDROID_L)
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "check data %x\n", i);
#else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "check data %x\n", (unsigned int)i);
#endif
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "dat0 %X <===> ",dspCode_buf[i]);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "%x \n", dat[0]);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "dat1 %X <===> ",dspCode_buf[i+1]);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "%x \n", dat[1]);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "dat2 %x <===> ",dspCode_buf[i+2]);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "%X \n", dat[2]);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "  Dsp code verify error!!\r\n");

            g_bDSPLoadCode = FALSE;

            OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

            return FALSE;
        }
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "  Dsp code verify ok!!\n\r");

    g_bDSPLoadCode = FALSE;

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_CheckDecIdmaReady()
/// @brief \b Function \b Description:  This routine is used to check if the Dec-DSP IDMA is ready or not.
/// @param <IN>        \b IdmaChk_type    :
///                                    0x10 : check write ready
///                                    0x80 : check read  ready
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL    : TRUE--IDMA is ready
///                                      FALSE--IDMA not ready
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_CheckDecIdmaReady(MS_U8 u8IdmaChk_type )
{
    MS_U8  j = 0;

    while(j<200)
    {
        j++;
        if( (HAL_AUDIO_ReadByte(REG_DEC_IDMA_CTRL0)& u8IdmaChk_type) == 0 )
            return TRUE;
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "DSP DEC Idma check ready fail!(%d)\r\n",j);
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_CheckSeIdmaReady()
/// @brief \b Function \b Description:  This routine is used to check if the Se-DSP IDMA is ready or not.
/// @param <IN>        \b IdmaChk_type    :
///                                    0x10 : check write ready
///                                    0x80 : check read  ready
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL    : TRUE--IDMA is ready
///                                      FALSE--IDMA not ready
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_CheckSeIdmaReady(MS_U8 u8IdmaChk_type)
{
    MS_U8  j = 0;

    while(j<200)
    {
        j++;
        if( (HAL_AUDIO_ReadByte(REG_SE_IDMA_CTRL0)& u8IdmaChk_type) == 0 )
            return TRUE;
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "DSP SE Idma check ready fail!(%d)\r\n",j);
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_SetDspCodeTypeLoaded()
/// @brief \b Function \b Description:  This function is used to set the DSP code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDSP_SetDspCodeTypeLoaded(MS_U8 u8Type)
{
    g_u8DspCodeTypeLoaded=u8Type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_GetDspCodeTypeLoaded()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDSP_GetDspCodeTypeLoaded(void)
{
    return g_u8DspCodeTypeLoaded;
}

void HAL_AUDSP_SetDspLoadCodeInfo(AUDIO_ALG_INFO *pau_info, MS_U8 DSP_select)
{
    g_loadcodeinfo.pau_info = pau_info;
    g_loadcodeinfo.DSP_select= DSP_select;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_GetDspCodeTypeLoaded()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
LOAD_CODE_INFO* HAL_AUDSP_GetDspLoadCodeInfo(void)
{
    return &g_loadcodeinfo;
}
