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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "drvBDMA.h"
#include "drvSERFLASH.h"

#include "regCHIP.h"

#include "halAUDSP.h"
#include "drvAUDIO.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "halADVAUDIO.h"
#include "ddr_config.h"
#include "r2_shm_comm.h"
#include "decR2_shm.h"
#if ASND_R2_SUPPORT
#include "sndR2_shm.h"
#endif

#ifdef CONFIG_MBOOT//Mboot Mini system
    #include "dspcode_s/aucode_mboot_r2.c"
#else
    #include "dspcode_s/aucode_adec_r2.c"
#if ASND_R2_SUPPORT
    #include "dspcode_s/aucode_asnd_r2.c"
#endif
#endif



#if defined(MSOS_TYPE_NUTTX)
extern int lib_lowprintf(const char *format, ...);
#define DBG_PREFIX lib_lowprintf
#else
#define DBG_PREFIX printf
#endif

#if (defined ANDROID)
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#ifndef LOGI // android 4.1 rename LOGx to ALOGx
#define LOGI ALOGI
#endif

#ifndef LOGE // android 4.1 rename LOGx to ALOGx
#define LOGE ALOGE
#endif

#define HALAUDSP_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
#define HALAUDSP_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)
#else
#define HALAUDSP_PRINT(fmt, args...)    DBG_PREFIX("[[utopia]]      " fmt, ## args)
#define HALAUDSP_ERROR(fmt, args...)    DBG_PREFIX("[[utopia]]      " fmt, ## args)
#endif

#define HALAUDSP_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            HALAUDSP_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            HALAUDSP_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define DBG_AUDSP_LOAD(args...)   //DBG_PREFIX(args)
#define DBG_AUDSP(args...)        //DBG_PREFIX(args)
#define DBG_AUDSP_ERROR(args...)  //DBG_PREFIX(args);

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

#ifdef CONFIG_MBOOT
    #define FLASH_COPY(x,y,z,i,j)   TRUE
    #define FLASH_CHKDONE()         TRUE
#else
    #define FLASH_COPY(x,y,z,i,j)   MDrv_SERFLASH_CopyHnd(x,y,z,i,j)
    #define FLASH_CHKDONE()         MDrv_FLASH_CheckWriteDone()
#endif

LOAD_CODE_INFO g_loadcodeinfo;
MS_BOOL        g_bDSPLoadCode = FALSE;

#ifndef CONFIG_MBOOT
AUDIO_ALG_INFO dsp_info[]=
{
    // ---------- Decoder2 DSP Code ----------
    //AU_DVB_SYS_NONE[0]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DVB2_NONE",
        AU_DVB2_NONE,
    },
    // SE System
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "SE SYSTEM",
        AU_SE_SYSTEM,
    },
    //DDE
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "dde",
        AU_DVB2_STANDARD_DDE,
    },
    //BTSC
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "SIF BTSC",
        AU_SIF_BTSC,
    },
    // PALSUM
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "SIF PALSUM",
        AU_SIF_PALSUM,
    },
    // audio_adv_sndeff_info
    //DBX
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ADVSND DBX",
        AU_DVB2_ADVSND_DBX,
    },
     //PURESND
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ADVSND PURESND",
        AU_DVB2_ADVSND_PURESND,
    },
    //DPS
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ADVSND BONGIOVI_DPS",
        AU_DVB2_ADVSND_BONGIOVI_DPS,
    },
    //DTSE
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DTSE",
        AU_DVB2_ENCODE_DTSE,
    },
    //DDE
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DDE",
        AU_DVB2_ENCODE_DDE,
    },
    //MS10-DDE
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "MS10-DDE",
        AU_DVB2_ENCODE_MS10_DDE,
    },

};
#else //=======================CONFIG_MBOOT=========================================//
AUDIO_ALG_INFO dsp_info[]=
{
    // ---------- Decoder2 DSP Code ----------
    //AU_DVB_SYS_NONE[0]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DVB2_NONE",
        AU_DVB2_NONE,
    },
    // SE System
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "SE SYSTEM",
        AU_SE_SYSTEM,
    },
};
#endif

#ifndef MSOS_TYPE_NOS
void* MDrv_MPool_PA2KSEG1(void* pAddrPhys);
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define AUDIO_HAL_ERR(x, args...)        //{printf(x, ##args);}
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
static AUDIO_DSP_CODE_TYPE g_DspCodeTypeLoaded = AU_DVB_NONE;

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
/// @brief \b Function \b Name: MDrv_AUDIO_DspLoadCodeKernel()
/// @brief \b Function \b Description:  load CM/PM/cache/prefetch DSP code
/// @param <IN>        \b \b u8Type    :      -- DSP load code type
///                                        DSP_segment -- select DSP load code seg
///                                     DSP_select -- select DSP1 or DSP2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_DspLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType)
{
#ifndef MSOS_TYPE_NUTTX
   REE_TO_TEE_MBX_MSG_TYPE msg_type = REE_TO_TEE_MBX_MSG_NULL;
#endif
    //MS_U8   DSP_select= DSP_SE;
    //MS_U32 DEC_R2_ADDR, SND_R2_ADDR;
    AUDIO_ALG_INFO *pau_info=NULL;
    MS_U8   DSP_select=0;
    MS_U32  MIU_addr;
    int ii;

    HALAUDSP_CHECK_SHM_INIT;

    DBG_AUDSP_LOAD("@@@@@@@@@@    ====== HAL_AUDSP_DspLoadCode: 0x%x ======\r\n",u8Type);

    if (IS_AUDIO_DSP_CODE_TYPE_DEC1_SYSTEM(dspCodeType))
    {
        return  TRUE;    // Decoder in DEC-R2
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_DEC_SYSTEM(dspCodeType)) //AU_DEC_SYSTEM
    {
        return  TRUE;    // Decoder in DEC-R2
    }
    else if ((!IS_AUDIO_DSP_CODE_TYPE_DEC2_SYSTEM(dspCodeType)) ||
             (!IS_AUDIO_DSP_CODE_TYPE_SE_SYSTEM(dspCodeType)) ||
             (!IS_AUDIO_DSP_CODE_TYPE_SIF(dspCodeType)) ||
             (!IS_AUDIO_DSP_CODE_TYPE_ADVSND(dspCodeType)) ||
             (!IS_AUDIO_DSP_CODE_TYPE_SE_ENC(dspCodeType)))
    {
        for (ii=0; ii< sizeof(dsp_info)/sizeof(AUDIO_ALG_INFO);ii++)
        {
            if (dspCodeType == dsp_info[ii].dspCodeType)
                break;
        }
        if (ii < (sizeof(dsp_info)/sizeof(AUDIO_ALG_INFO)))
        {
            pau_info = &dsp_info[ii];
            DSP_select = DSP_SE;
        }
        else
        {
            DBG_AUDSP_ERROR(AUDIO_DEBUG_LEVEL_ERROR, "    [HAL_AUDSP_DspLoadCode]:======  Loading the wrong DSP code type!======\r\n");
            return FALSE;
        }
    }

#ifndef MSOS_TYPE_NUTTX
    AUDIO_TEE_INFO_SHM_CHECK_NULL;
    REE_TO_TEE_MBX_MSG_SET_PARAM(0, (dspCodeType&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM_COUNT(1);
    msg_type = REE_TO_TEE_MBX_MSG_HAL_AUDSP_DspLoadCode;

    msg_type = msg_type | REE_TO_TEE_MBX_MSG_TYPE_SE;

    if ( TEE_TO_REE_MBX_ACK_MSG_NO_TEE != (mbx_msg_ack_status = HAL_AUDIO_SendMBXMsg(msg_type)))
    {
        if (mbx_msg_ack_status != TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS)
            return FALSE;
        HAL_AUDSP_SetDspCodeTypeLoaded(dspCodeType);
        return TRUE;
    }
#endif


  // Load  SE-DSP  system code
    if (IS_AUDIO_DSP_CODE_TYPE_SE_SYSTEM(dspCodeType))
    {
        HAL_AUR2_WriteByte(REG_DECR2_RESET_CTRL, 0x00);               // STOP DEC-R2
        HAL_AUR2_WriteMaskReg(REG_DECR2_SYSTEM_START, 0xFFFF, 0x0000); // clear DEC-R2 start cmd register
#if ASND_R2_SUPPORT
        HAL_AUR2_WriteByte(REG_SNDR2_RESET_CTRL, 0x00);               // STOP SND-R2
        HAL_AUR2_WriteMaskReg(REG_SNDR2_SYSTEM_START, 0xFFFF, 0x0000); //clear SND-R2 start cmd register
#endif
        HAL_AUDIO_WriteMaskReg(0x2DDC, 0xFFFF, 0x0000); //clear  SE-DSP start cmd register
        AUDIO_DELAY1MS(1);

        HAL_AUR2_WriteMaskReg(REG_SDR_SWITCH_CTRL, 0xFFFF, 0x3213);    //use SDR2 address

      #ifdef  CONFIG_MBOOT
      //============ Load R2 code ===============
      // Memory sequence : Mboot-R2(DEC-R2)
      //====================================
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);

        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((mst_codec_r2)), DEC_R2_SHM_DDR_OFFSET);
        memset((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE), 0, ADEC__R2_DDR_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      (void*)(mst_codec_r2+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      MST_CODEC_R2_AUDIO_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        HAL_DEC_R2_init_SHM_param();
        AUDIO_DELAY1MS(1);
        MsOS_FlushMemory();

        HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
      #else
      //============ Load R2 code ===============
      // Memory sequence : DEC-R2 ==> SND-R2 ==> SE-DSP
      //====================================

      // Load DEC-R2
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((mst_codec_r2)), DEC_R2_SHM_DDR_OFFSET);
        memset((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE), 0, ADEC__R2_DDR_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      (void*)(mst_codec_r2+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      MST_CODEC_R2_AUDIO_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        HAL_DEC_R2_init_SHM_param();
        AUDIO_DELAY1MS(1);
        MsOS_FlushMemory();

#if ASND_R2_SUPPORT
     // Load SND-R2
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV)+ADEC__R2_DDR_SIZE;
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((mst_snd_r2)), SND_R2_SHM_DDR_OFFSET);
        memset((void*)(MS_PA2KSEG1(MIU_addr)+SND_R2_SHM_DDR_OFFSET+SND_R2_SHM_DDR_SIZE), 0, ASND__R2_DDR_SIZE - SND_R2_SHM_DDR_OFFSET - SND_R2_SHM_DDR_SIZE);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)+SND_R2_SHM_DDR_OFFSET+SND_R2_SHM_DDR_SIZE),
                      (void*)(mst_snd_r2+SND_R2_SHM_DDR_OFFSET+SND_R2_SHM_DDR_SIZE),
                      MST_SND_R2_AUDIO_SIZE - SND_R2_SHM_DDR_OFFSET - SND_R2_SHM_DDR_SIZE);
        AUDIO_DELAY1MS(1);
        MsOS_FlushMemory();

        HAL_SND_R2_EnableR2(TRUE);               // Enable SND-R2 after load R2 code
#endif
        HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
      #endif

#if 0  //secure boot(Reserved)
        HAL_AUR2_WriteMaskReg(REG_SDR_SWITCH_CTRL, 0xFFFF, 0x3210);    //use SDR1 address

      #ifdef  CONFIG_MBOOT
      //============ Load R2 code ===============
      // Memory sequence : Mboot-R2(DEC-R2)
      //====================================
        DEC_R2_ADDR = (MS_U32) MsOS_MPool_VA2PA((MS_VIRT) &mst_codec_r2);

        HAL_AUR2_WriteReg(REG_DECR2_ICMEM_BASE_LO, (MS_U16)(DEC_R2_ADDR & 0xFFFF));
        HAL_AUR2_WriteReg(REG_DECR2_ICMEM_BASE_HI, (MS_U16)((DEC_R2_ADDR >> 16) & 0xFFFF));
        HAL_AUR2_WriteReg(REG_DECR2_DCMEM_BASE_LO, (MS_U16)(DEC_R2_ADDR & 0xFFFF));
        HAL_AUR2_WriteReg(REG_DECR2_DCMEM_BASE_HI, (MS_U16)((DEC_R2_ADDR >> 16) & 0xFFFF));
        AUDIO_DELAY1MS(1);
        HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
      #else
      //============ Load R2 code ===============
      // Memory sequence : DEC-R2 ==> SND-R2 ==> SE-DSP
      //====================================
        DEC_R2_ADDR =  (MS_U32) MsOS_MPool_VA2PA((MS_VIRT) &mst_codec_r2);
        SND_R2_ADDR =  (MS_U32) MsOS_MPool_VA2PA((MS_VIRT) &mst_snd_r2);

        HAL_AUR2_WriteReg(REG_DECR2_ICMEM_BASE_LO, (MS_U16)(DEC_R2_ADDR & 0xFFFF));
        HAL_AUR2_WriteReg(REG_DECR2_ICMEM_BASE_HI, (MS_U16)((DEC_R2_ADDR >> 16) & 0xFFFF));
        HAL_AUR2_WriteReg(REG_DECR2_DCMEM_BASE_LO, (MS_U16)(DEC_R2_ADDR & 0xFFFF));
        HAL_AUR2_WriteReg(REG_DECR2_DCMEM_BASE_HI, (MS_U16)((DEC_R2_ADDR >> 16) & 0xFFFF));
        HAL_AUR2_WriteReg(REG_SNDR2_ICMEM_BASE_LO, (MS_U16)(SND_R2_ADDR & 0xFFFF));
        HAL_AUR2_WriteReg(REG_SNDR2_ICMEM_BASE_HI, (MS_U16)((SND_R2_ADDR >> 16) & 0xFFFF));
        HAL_AUR2_WriteReg(REG_SNDR2_DCMEM_BASE_LO, (MS_U16)(SND_R2_ADDR & 0xFFFF));
        HAL_AUR2_WriteReg(REG_SNDR2_DCMEM_BASE_HI, (MS_U16)((SND_R2_ADDR >> 16) & 0xFFFF));
        AUDIO_DELAY1MS(1);
        HAL_SND_R2_EnableR2(TRUE);               // Enable SND-R2 after load R2 code
        HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
#endif
      #endif
    }

    HAL_AUDSP_SetDspCodeTypeLoaded(dspCodeType);
    DBG_AUDSP_LOAD("HAL_AUDSP_DspLoadCode finished(type=(0x%x))\r\n" , u8dspCodeType);

    return TRUE;
}

MS_BOOL HAL_AUDSP_DspLoadCode2(MS_U8 DSP_select, void * info)
{
    return TRUE;
}

MS_BOOL HAL_AUDSP_DspLoadSystemCode2(MS_U8 DSP_select, AUDIO_ALG_INFO * pau_info)
{
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
    g_bDSPLoadCode = TRUE;
    return TRUE;     // secure boot: MCU can't use IDMA
}

MS_BOOL HAL_AUDSP_DspVerifySegmentCode(MS_U32 dsp_addr, MS_U8 *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select)
{
    g_bDSPLoadCode = TRUE;
    return TRUE;     // secure boot: MCU can't use IDMA
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
    return FALSE;  // secure boot: MCU can't use IDMA
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

    DBG_AUDSP_ERROR("XBox check data trans fail!(%d)\r\n",j);
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
void HAL_AUDSP_SetDspCodeTypeLoaded(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    g_DspCodeTypeLoaded = dspCodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_GetDspCodeTypeLoaded()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_AUDSP_GetDspCodeTypeLoaded(void)
{
    return g_DspCodeTypeLoaded;
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
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_DECR2LoadCode()
/// @brief \b Function \b Description:  Load DEC-R2 code to DDR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDSP_DECR2LoadCode(void)
{
    MS_U32 MIU_addr;

    printf("======audio: start DEC-R2 load code======\n");

    HAL_AUR2_WriteByte(REG_DECR2_RESET_CTRL, 0x00);               // STOP DEC-R2
    HAL_AUR2_WriteMaskReg(REG_DECR2_SYSTEM_START, 0xFFFF, 0x0000); //clear DEC-R2 start cmd register
    AUDIO_DELAY1MS(1);

  //============ Load R2 code ===============
  // Memory sequence : DEC-R2 ==> SND-R2 ==> SE-DSP
  //====================================

  // Load DEC-R2
    MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);

    memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((mst_codec_r2)), DEC_R2_SHM_DDR_OFFSET);
    memset((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE), 0, ADEC__R2_DDR_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
    memcpy((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                  (void*)(mst_codec_r2+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                  MST_CODEC_R2_AUDIO_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
    HAL_DEC_R2_init_SHM_param();
    AUDIO_DELAY1MS(1);
    MsOS_FlushMemory();

    HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
    HAL_MAD2_SetMcuCmd(0xF3);

    printf("======audio: end DEC-R2 load code======\n");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_CheckDecIdmaReady()
/// @brief \b Function \b Description:  This routine is used to check if the XBox data transport is ready or not.
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL    : TRUE--IDMA is ready
///                                      FALSE--IDMA not ready
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_Check_XBox_TransData_Ready(void)
{
    MS_U8  j = 0;
    while(j<200)
    {
        j++;
        if( (HAL_AUDIO_ReadByte(REG_DSP_XBOX_MISC)& 0x08) == 0x00 )  //bit[3] = 1 ,busy
        return TRUE;
    }
    DBG_AUDSP_ERROR("XBox check data trans fail!(%d)\r\n",j);
    return FALSE;
}

