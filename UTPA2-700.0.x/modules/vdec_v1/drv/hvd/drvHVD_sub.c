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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvHVD_sub.c
/// @brief  SubHVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(SUPPORT_HVD_SUB)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "drvHVD_Common.h"
#include "drvHVD.h"
#include "drvHVD_def.h"
#include "drvHVD_sub.h"
#include "drvHVD_sub_def.h"

#if defined(CHIP_T2)
#include "halSVD.h"
#include "regSVD.h"
#include "fwSVD_if.h"
#else
    #include "halHVD.h"
    #include "halHVD_sub.h"
    #include "fwHVD_if.h"
    #include "halVPU.h"
#endif

#if HVD_ENABLE_AUTO_SET_REG_BASE
#include "drvMMIO.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// Drv memory usage
#if defined(REDLION_LINUX_KERNEL_ENVI)
    #define HVD_DTV_VIDEO_DELAY         0 // ms
#else
    #define HVD_DTV_VIDEO_DELAY         0 // ms
#endif
#define HVD_FW_CMD_TIMEOUT_DEFAULT      100 // ms
#define HVD_FW_EXIT_ACTION_TIMEOUT      3500 // ms

#define HVD_CTRL_INIT_FINISHED          BIT(0)    // deflaut: 0
#define HVD_CTRL_PROCESSING             BIT(1)    // deflaut: 0
#define HVD_CTRL_DATA_END               BIT(2)    // deflaut: 0
#define HVD_CTRL_DISPLAY_CTRL           BIT(3)    // deflaut: 0
#define HVD_CTRL_DISP_INFO_RDY          BIT(4)    // deflaut: 0
#define HVD_CTRL_CODE_MIU_1             BIT(5)    // deflaut: 0
#define HVD_CTRL_ES_MIU_1               BIT(6)    // deflaut: 0
#define HVD_CTRL_FRM_MIU_1              BIT(7)    // deflaut: 0
#define HVD_CTRL_DRVPROCC_MIU_1         BIT(8)    // deflaut: 0

typedef enum
{
    E_HVD_CHECK_CMD_NONE = 0, //decode -> decode_finish
    E_HVD_CHECK_CMD_INIT,
    E_HVD_CHECK_CMD_TRIGGER_DISP,
    E_HVD_CHECK_CMD_SEEK2PTS,
    E_HVD_CHECK_CMD_MAX,
} HVD_Check_Cmd;

#define HVD_MIU_PROTECT_HVD     BIT(0)
#define HVD_MIU_PROTECT_MVD     BIT(1)
#define HVD_MIU_PROTECT_VPU     BIT(2)

#define _DRV_HVD_Entry()        {gSubHVDCtrl.u32CtrlMode |= HVD_CTRL_PROCESSING;}
#define _DRV_HVD_Return(_ret)   {gSubHVDCtrl.u32CtrlMode &= (~HVD_CTRL_PROCESSING); return (_ret);}

#define _DRV_HVD_Initedn()      {if( !bHVDIsInited ){ return ; }}
#define _DRV_HVD_Inited(_ret)   {if( !bHVDIsInited ){ return (_ret); }}
#define _DRV_HVD_Rsting(_ret)   {if( bHVDIsIniting ){ return (_ret); }}
#define _DRV_HVD_Ctrl(x)        ( gSubHVDCtrl.u32CtrlMode & (x) )
#define _DRV_HVD_SetCtrl(x)     { gSubHVDCtrl.u32CtrlMode |= (x); }

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
HVD_Drv_Ctrl    gSubHVDCtrl;
MS_U32 u32SubUartCtrl=(E_HVD_UART_CTRL_ERR|E_HVD_UART_CTRL_MUST);
MS_U32 u32SubInitSysTimeBase=0;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static HVD_DrvInfo DrvInfo;
static MS_BOOL bHVDIsInited=FALSE;
static MS_BOOL bHVDIsIniting=FALSE;
static HVD_ISR_Ctrl gHVDISRCtrl={ FALSE, FALSE, 0  , NULL };
static HVD_BBU_Info gHVDPacket={0,0,0,0,0,0, FALSE};
static unsigned char u8_4Lines[64];
//MS_U8 pu8HVD_DMAcpy_pool[20];

static HVD_Result _DRV_HVD_Sub_InitVariables(HVD_MemCfg *pStMemCfg, HVD_Init_Params *pStInitParams);
static HVD_Result _DRV_HVD_Sub_Check_Cmd(HVD_Check_Cmd etype);
static HVD_Result _DRV_HVD_Sub_InitFW_AVC(void);
static HVD_Result _DRV_HVD_Sub_Check_FW_Version(void);
static HVD_Result _DRV_HVD_Sub_InitFW(void);

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
void _HVD_Sub_Chip_Flush_Memory(void)
{
    unsigned char *pu8;
    volatile unsigned char tmp;

    // Transfer the memory to noncache memory
    // Flush the data from pipe and buffer in MIU
    if( gSubHVDCtrl.u32DummyWriteBuf != 0 )
    {
        pu8 = ((unsigned char *)(((unsigned int)gSubHVDCtrl.u32DummyWriteBuf   )  ));
#if 1
        pu8[0] = pu8[16] = pu8[32] = pu8[48] = 1;
#else
        // TODO: this patch is necessary for T3_nos_mips & simpleplayer.
        {
            MS_U32 cnt=0;
            for (cnt = 0; cnt < 128; cnt++)
            {
                *pu8 = 1;
                pu8++;
            }
        }
#endif
    }
    else
    {
        pu8 = ((unsigned char *)(((unsigned int)u8_4Lines)  ));
        pu8[0] = pu8[16] = pu8[32] = pu8[48] = 1;
    }

    // Flush the data in the EC bridge buffer
    //mb();
    HAL_MEMORY_BARRIER();

    // final read
    tmp = pu8[0] ;
    tmp = pu8[16] ;
    tmp = pu8[32] ;
    tmp = pu8[48] ;
}

void _HVD_Sub_Chip_Read_Memory(void)
{
    volatile unsigned char *pu8;
    volatile unsigned int t;

    // Transfer the memory to noncache memory
    pu8 = (volatile unsigned char *)(((unsigned int)gSubHVDCtrl.MemMap.u32CodeBufVAddr));

    t = pu8[0];
    t = pu8[64];
}

static HVD_Result _DRV_HVD_Sub_InitVariables(HVD_MemCfg *pStMemCfg, HVD_Init_Params *pStInitParams)
{
    HVD_memset((void *) &gSubHVDCtrl, 0, sizeof(HVD_Drv_Ctrl));

    if (sizeof(HVD_Init_Params) == sizeof(HVD_Init_Settings))
    {
        HVD_memcpy((void *) &gSubHVDCtrl.InitParams,
                   pStInitParams,
                   sizeof(HVD_Init_Params));
    }
    else
    {
        HVD_SUB_MSG_ERR("HVD Err: HVD struct define is diff: HVD_Init_Params(%lu) vs HVD_Init_Settings(%lu)\n",
                    (MS_U32)sizeof(HVD_Init_Params),
                    (MS_U32)sizeof(HVD_Init_Settings));
        return E_HVD_FAIL;
    }

    // fill memory
#if 0
    HVD_memcpy( (void*)&gSubHVDCtrl.MemMap ,  pStMemCfg  , sizeof(HVD_Mem_Map)  );
#else
    if (pStMemCfg->u32MIU1BaseAddr == 0)
    {
#if HVD_ENABLE_MSOS_MIU1_BASE
        gSubHVDCtrl.MemMap.u32MIU1BaseAddr=  HAL_MIU1_BASE;
#else
        gSubHVDCtrl.MemMap.u32MIU1BaseAddr=  HVD_MIU1_BASE_ADDRESS;
#endif
    }
    else
    {
        gSubHVDCtrl.MemMap.u32MIU1BaseAddr = pStMemCfg->u32MIU1BaseAddr;
    }

    gSubHVDCtrl.MemMap.eFWSourceType       = (HVD_FWInputSourceType) pStMemCfg->eFWSourceType;
    gSubHVDCtrl.MemMap.u32FWBinaryVAddr    = pStMemCfg->u32FWBinaryVAddr;
    gSubHVDCtrl.MemMap.u32FWBinaryAddr     = (MS_U32) pStMemCfg->u32FWBinaryAddr;
    gSubHVDCtrl.MemMap.u32FWBinarySize     = pStMemCfg->u32FWBinarySize;
    gSubHVDCtrl.MemMap.u32VLCBinaryVAddr   = pStMemCfg->u32VLCBinaryVAddr;
    gSubHVDCtrl.MemMap.u32VLCBinaryAddr    = (MS_U32) pStMemCfg->u32VLCBinaryAddr;
    gSubHVDCtrl.MemMap.u32VLCBinarySize    = pStMemCfg->u32VLCBinarySize;

    gSubHVDCtrl.MemMap.u32CodeBufVAddr     = pStMemCfg->u32CodeBufVAddr;
    gSubHVDCtrl.MemMap.u32CodeBufAddr      = (MS_U32) pStMemCfg->u32CodeBufAddr;
    gSubHVDCtrl.MemMap.u32CodeBufSize      = pStMemCfg->u32CodeBufSize;
    gSubHVDCtrl.MemMap.u32FrameBufVAddr    = pStMemCfg->u32FrameBufVAddr;
    gSubHVDCtrl.MemMap.u32FrameBufAddr     = (MS_U32) pStMemCfg->u32FrameBufAddr;
    gSubHVDCtrl.MemMap.u32FrameBufSize     = pStMemCfg->u32FrameBufSize;
    gSubHVDCtrl.MemMap.u32BitstreamBufVAddr= pStMemCfg->u32BitstreamBufVAddr;
    gSubHVDCtrl.MemMap.u32BitstreamBufAddr = (MS_U32) pStMemCfg->u32BitstreamBufAddr ;
    gSubHVDCtrl.MemMap.u32BitstreamBufSize = pStMemCfg->u32BitstreamBufSize ;
    gSubHVDCtrl.MemMap.u32DrvProcessBufVAddr = pStMemCfg->u32DrvProcessBufVAddr;
    gSubHVDCtrl.MemMap.u32DrvProcessBufAddr = (MS_U32)pStMemCfg->u32DrvProcessBufAddr;
    gSubHVDCtrl.MemMap.u32DrvProcessBufSize = pStMemCfg->u32DrvProcessBufSize;
#endif
    // get non-cache address
#if 0
    if( pStMemCfg->u32CPUNonCacheMask == 0 )
    {
        #if defined (__aeon__)
            #if defined (CHIP_T2)
            gSubHVDCtrl.u32CPUNonCacheMask=0x80000000;
            #else
            gSubHVDCtrl.u32CPUNonCacheMask=0xC0000000;
            #endif
        #else   // MIPS
        gSubHVDCtrl.u32CPUNonCacheMask=0xA0000000;
        #endif
    }
    else
    {
        gSubHVDCtrl.u32CPUNonCacheMask = pStMemCfg->u32CPUNonCacheMask;
    }
    #if defined( MSOS_TYPE_LINUX) || defined(REDLION_LINUX_KERNEL_ENVI)
    gSubHVDCtrl.u32CPUNonCacheMask=0;
    #endif
    // Virtual address to physical
#if 0
    gSubHVDCtrl.MemMap.u32CodeBufAddr=   HVD_VA2PA( pStMemCfg->u32CodeBufVAddr  ) ;
    gSubHVDCtrl.MemMap.u32FrameBufAddr=   HVD_VA2PA( pStMemCfg->u32FrameBufVAddr  ) ;
    gSubHVDCtrl.MemMap.u32BitstreamBufAddr=   HVD_VA2PA( pStMemCfg->u32BitstreamBufVAddr  ) ;
    gSubHVDCtrl.MemMap.u32DrvProcessBufAddr = HVD_VA2PA( pStMemCfg->u32DrvProcessBufVAddr  ) ;
#endif
    // remove non-cache
    gSubHVDCtrl.MemMap.u32BitstreamBufAddr &= (~gSubHVDCtrl.u32CPUNonCacheMask);
    gSubHVDCtrl.MemMap.u32CodeBufAddr &= (~gSubHVDCtrl.u32CPUNonCacheMask);
    gSubHVDCtrl.MemMap.u32FrameBufAddr &= (~gSubHVDCtrl.u32CPUNonCacheMask);
    gSubHVDCtrl.MemMap.u32DrvProcessBufAddr &= (~gSubHVDCtrl.u32CPUNonCacheMask);
#endif
    HVD_SUB_MSG_INFO("HVD mmap: MIU1base:%lx FW(%d %lx %lx %lx)vlc(%lx %lx %lx) (Va,Pa,Siz)Co(%lx %lx %lx)Fr(%lx %lx %lx)Bit(%lx %lx %lx) Drv(%lx %lx %lx)\n",
                 gSubHVDCtrl.MemMap.u32MIU1BaseAddr,
                 gSubHVDCtrl.MemMap.eFWSourceType,
                 gSubHVDCtrl.MemMap.u32FWBinaryVAddr,
                 gSubHVDCtrl.MemMap.u32FWBinaryAddr,
                 gSubHVDCtrl.MemMap.u32FWBinarySize,
                 gSubHVDCtrl.MemMap.u32VLCBinaryVAddr,
                 gSubHVDCtrl.MemMap.u32VLCBinaryAddr,
                 gSubHVDCtrl.MemMap.u32VLCBinarySize,
                 pStMemCfg->u32CodeBufVAddr,
                 gSubHVDCtrl.MemMap.u32CodeBufAddr,
                 gSubHVDCtrl.MemMap.u32CodeBufSize,
                 pStMemCfg->u32FrameBufVAddr,
                 gSubHVDCtrl.MemMap.u32FrameBufAddr,
                 gSubHVDCtrl.MemMap.u32FrameBufSize,
                 pStMemCfg->u32BitstreamBufVAddr,
                 gSubHVDCtrl.MemMap.u32BitstreamBufAddr,
                 gSubHVDCtrl.MemMap.u32BitstreamBufSize,
                 pStMemCfg->u32DrvProcessBufVAddr,
                 gSubHVDCtrl.MemMap.u32DrvProcessBufAddr,
                 gSubHVDCtrl.MemMap.u32DrvProcessBufSize);

    gSubHVDCtrl.InitParams.u16DecoderClock = HAL_HVD_Sub_GetCorretClock(pStInitParams->u16DecoderClock);
    {
        MS_U32 offset = HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_DUMMY_WRITE_ADDR);

        if (offset != 0)
        {
            gSubHVDCtrl.u32DummyWriteBuf = gSubHVDCtrl.MemMap.u32CodeBufVAddr + offset;
        }
    }

#if HVD_ENABLE_STOP_ACCESS_OVER_256
    gSubHVDCtrl.bCannotAccessMIU256 = TRUE;
#endif

    gSubHVDCtrl.bAutoRmLastZeroByte = TRUE;
    gSubHVDCtrl.u32CmdTimeout = HVD_FW_CMD_TIMEOUT_DEFAULT;

    if (pStInitParams->u8TurboInit & E_HVD_TURBOINIT_FW_RELOAD)
    {
        gSubHVDCtrl.bTurboFWMode = TRUE;
    }

#if !(defined(CHIP_T2) || defined(CHIP_T7))
    gSubHVDCtrl.Settings.u32MiuBurstLevel = (MS_U32)E_HVD_BURST_CNT_DISABLE;
#endif

    return E_HVD_OK;
}

static HVD_Result _DRV_HVD_Sub_Check_Cmd(HVD_Check_Cmd eCmd)
{
    MS_U32 u32Tmp0 = 0;
    MS_U32 u32Tmp1 = 0;

    switch (eCmd)
    {
        case E_HVD_CHECK_CMD_INIT:
        {
            if (!(gSubHVDCtrl.InitParams.u8TurboInit & E_HVD_TURBOINIT_CHECK))
            {
                // Check mode
                u32Tmp0 = gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK;

                if (!MDrv_HVD_Sub_GetCaps((HVD_Codec) u32Tmp0))
                {
                    HVD_SUB_MSG_ERR("Check Cmd Err: Init: HW not support type:%d\n", eCmd);
                    return E_HVD_RET_UNSUPPORTED;
                }

                u32Tmp0 = gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK;

                if (u32Tmp0 == HVD_INIT_MAIN_LIVE_STREAM)
                {
                    if ((gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_INPUT_MASK) == HVD_INIT_INPUT_DRV)
                    {
                        HVD_SUB_MSG_ERR("Check Cmd Err: Init: Live stream mode can not use Drv input\n");
                        return E_HVD_RET_INVALID_PARAMETER;
                    }
                    if(!(gSubHVDCtrl.InitParams.u32ModeFlag & (HVD_INIT_UTOPIA_ENVI|HVD_INIT_DBG_FW)))
                    {
                        // need not to check this under debug mode
                        if ((gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_START_CODE_MASK) == HVD_INIT_START_CODE_REMOVED)
                        {
                            HVD_SUB_MSG_ERR("Check Cmd Err: Init: Live stream mode must have start code\n");
                            return E_HVD_RET_INVALID_PARAMETER;
                        }
                        if (gSubHVDCtrl.InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_NON)
                        {
                            HVD_SUB_MSG_ERR("Check Cmd Err: Init: Live stream mode can not use none ATS sync mode\n");
                            return E_HVD_RET_INVALID_PARAMETER;
                        }
                    }
                }
                else if ((u32Tmp0 == HVD_INIT_MAIN_FILE_RAW)
                      || (u32Tmp0 == HVD_INIT_MAIN_FILE_TS))
                {
                    if (!(gSubHVDCtrl.InitParams.u32ModeFlag & (HVD_INIT_UTOPIA_ENVI | HVD_INIT_DBG_FW)))
                    {
                        if((gSubHVDCtrl.InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_DTS)
                        && (gSubHVDCtrl.InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_PTS)
                        && (gSubHVDCtrl.InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_NON))
                        {
                            HVD_SUB_MSG_ERR("Check Cmd Err: Init: sync mode is not set. use default value:%d\n",
                                        (MS_U16)E_HVD_SYNC_TBL_TYPE_PTS);
                            gSubHVDCtrl.InitParams.u8SyncType = E_HVD_SYNC_TBL_TYPE_PTS;
                        }
                    }
                }
                else if ((u32Tmp0 != HVD_INIT_MAIN_FILE_RAW)
                      && (u32Tmp0 != HVD_INIT_MAIN_FILE_TS))
                {
                    HVD_SUB_MSG_ERR("Check Cmd Err: Init: main type can not be recognized:%ld\n", u32Tmp0);
                    return E_HVD_RET_INVALID_PARAMETER;
                }

                // check memory map
                if (gSubHVDCtrl.MemMap.u32MIU1BaseAddr == 0)
                {
                    HVD_SUB_MSG_ERR("Check Cmd Err: Init: MIU 1 Base addr should not be zero\n");
                    return E_HVD_RET_INVALID_PARAMETER;
                }

                if ((gSubHVDCtrl.MemMap.u32CodeBufAddr& 0x7FF) != 0)
                {
                    HVD_SUB_MSG_ERR("Check Cmd Err: Init: Error HVD code address(0x%lx) must 2048 byte alignment\n",
                                gSubHVDCtrl.MemMap.u32CodeBufAddr);
                    return E_HVD_RET_INVALID_PARAMETER;
                }
            }

            u32Tmp0 = gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK;
            u32Tmp1 = gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_INPUT_MASK;

            if (u32Tmp0 != HVD_INIT_MAIN_LIVE_STREAM)
            {
                if (u32Tmp1 == HVD_INIT_INPUT_DRV)
                {
                    // check if driver process buffer is in bitstream buffer.
                    MS_U32 tmpAddr = gSubHVDCtrl.MemMap.u32DrvProcessBufAddr;
                    if (gSubHVDCtrl.MemMap.u32DrvProcessBufAddr == 0)
                    {
                        HVD_SUB_MSG_ERR("Check Cmd Err: Init: Drv process buffer address should not be zero\n");
                        gSubHVDCtrl.bNoDrvProccBuf = TRUE;
                        //return E_HVD_RET_INVALID_PARAMETER;
                    }
                    if ((gSubHVDCtrl.MemMap.u32BitstreamBufAddr > tmpAddr)
                    || (tmpAddr > (gSubHVDCtrl.MemMap.u32BitstreamBufAddr+gSubHVDCtrl.MemMap.u32BitstreamBufSize)))
                    {
                        HVD_SUB_MSG_ERR("Check Cmd Warn: Init: The driver process buffer shall be located in the bitstream buffer under file mode(TS or MP4) and Drv input.\n");
                        gSubHVDCtrl.bNoDrvProccBuf = TRUE;
                        //return E_HVD_RET_INVALID_PARAMETER;
                    }
                    if (gSubHVDCtrl.MemMap.u32DrvProcessBufSize
                    < (HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB)<<3) + 8))
                    {
                        HVD_SUB_MSG_ERR("Check Cmd Warn: Init: File mode(TS or MP4) and Drv input must set the process buffer size and must be larger than %lu Bytes:%ld\n",
                                    gSubHVDCtrl.MemMap.u32DrvProcessBufSize,
                                    (MS_U32)(HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB)<<3) + 8) );
                        gSubHVDCtrl.bNoDrvProccBuf = TRUE;
                        //return E_HVD_RET_OUTOF_MEMORY;
                    }
                    if (gSubHVDCtrl.MemMap.u32DrvProcessBufSize > 1)
                    {
                        tmpAddr = gSubHVDCtrl.MemMap.u32DrvProcessBufAddr + gSubHVDCtrl.MemMap.u32DrvProcessBufSize -1;
                    }
                    else
                    {
                        tmpAddr = gSubHVDCtrl.MemMap.u32DrvProcessBufAddr;
                    }
                    if ((gSubHVDCtrl.MemMap.u32BitstreamBufAddr > tmpAddr)
                    || (tmpAddr > (gSubHVDCtrl.MemMap.u32BitstreamBufAddr + gSubHVDCtrl.MemMap.u32BitstreamBufSize)))
                    {
                        HVD_SUB_MSG_ERR("Check Cmd Warn: Init: The driver process buffer shall be located in the bitstream buffer under file mode(TS or PS) and Drv input.\n");
                        gSubHVDCtrl.bNoDrvProccBuf = TRUE;
                        //return E_HVD_RET_INVALID_PARAMETER;
                    }
                }
                else
                {
                    ;// TODO: check if the TSP MIU sel is the same with the MIU lacation of process buffer.
                }
            }
            break;
        }
        case E_HVD_CHECK_CMD_SEEK2PTS:
        case E_HVD_CHECK_CMD_TRIGGER_DISP:
#if 0
if( MDrv_HVD_GetPlayMode( E_HVD_GMODE_IS_SYNC_ON )  )
{
    HVD_SUB_MSG_ERR("Check Cmd Err: Cmd type:%d Sync Active: %lx\n", (MS_U16)eCmd ,  (MS_S32)MDrv_HVD_GetPlayMode( E_HVD_GMODE_IS_SYNC_ON ) );
    return E_HVD_RET_ILLEGAL_ACCESS;
}
#endif
            break;
        default:
            return E_HVD_OK;
    }
    return E_HVD_OK;
}

HVD_Result _DRV_HVD_Sub_InitShareMem(void)
{
    return (HVD_Result)HAL_HVD_Sub_InitShareMem();
}

HVD_Result _DRV_HVD_Sub_InitRegCPU(void)
{
    // check MIU select
    if( !(gSubHVDCtrl.InitParams.u8TurboInit & E_HVD_TURBOINIT_CHECK) )
    {
        HAL_HVD_Sub_CheckMIUSel( gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_UTOPIA_ENVI );
    }
    return (HVD_Result)HAL_HVD_Sub_InitRegCPU();
}

HVD_Result _DRV_HVD_Sub_RstVariables(void)
{
    MS_BOOL bBitMIU1 = FALSE;
    MS_BOOL bCodeMIU1 = FALSE;
    // Init control flg
    gSubHVDCtrl.u32CtrlMode=0;
    if( gSubHVDCtrl.MemMap.u32CodeBufAddr >= gSubHVDCtrl.MemMap.u32MIU1BaseAddr )
    {
        //gSubHVDCtrl.MemMap.u32CodeBufAddr-=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
        //gSubHVDCtrl.MemMap.u32FrameBufAddr-=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
        gSubHVDCtrl.u32CtrlMode |= HVD_CTRL_CODE_MIU_1;
        bCodeMIU1=TRUE;
    }
    if( gSubHVDCtrl.MemMap.u32BitstreamBufAddr >= gSubHVDCtrl.MemMap.u32MIU1BaseAddr  )
    {
        //gSubHVDCtrl.MemMap.u32BitstreamBufAddr-=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
        gSubHVDCtrl.u32CtrlMode |= HVD_CTRL_ES_MIU_1;
        bBitMIU1=TRUE;
    }
    if( gSubHVDCtrl.MemMap.u32FrameBufAddr>= gSubHVDCtrl.MemMap.u32MIU1BaseAddr  )
    {
        //gSubHVDCtrl.MemMap.u32BitstreamBufAddr-=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
        gSubHVDCtrl.u32CtrlMode |= HVD_CTRL_FRM_MIU_1;
    }
    if( gSubHVDCtrl.MemMap.u32DrvProcessBufAddr>= gSubHVDCtrl.MemMap.u32MIU1BaseAddr )
    {
        //gSubHVDCtrl.MemMap.u32BitstreamBufAddr-=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
        gSubHVDCtrl.u32CtrlMode |= HVD_CTRL_DRVPROCC_MIU_1;
    }

    if(!gSubHVDCtrl.bNoDrvProccBuf)
    {
        // init nal table buffer start address.
        if( ( gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
        {
            if( bBitMIU1 != bCodeMIU1 )
            {
                if( gSubHVDCtrl.MemMap.u32DrvProcessBufSize>
                    ( HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) <<3) + 8))
                {
                    gSubHVDCtrl.u32BBUTblInBitstreamBufAddr = HVD_BBU_ST_ADDR_IN_BITSTREAMBUF;
                    if( (( gSubHVDCtrl.u32BBUTblInBitstreamBufAddr + gSubHVDCtrl.MemMap.u32DrvProcessBufAddr) % 8) != 0)
                    {
                        gSubHVDCtrl.u32BBUTblInBitstreamBufAddr = gSubHVDCtrl.u32BBUTblInBitstreamBufAddr + gSubHVDCtrl.MemMap.u32DrvProcessBufAddr + 7;
                        gSubHVDCtrl.u32BBUTblInBitstreamBufAddr -= (gSubHVDCtrl.u32BBUTblInBitstreamBufAddr ) % 8;
                        gSubHVDCtrl.u32BBUTblInBitstreamBufAddr -= gSubHVDCtrl.MemMap.u32DrvProcessBufAddr;
                        HVD_SUB_MSG_ERR( "HVD Err: Nal table in bitstream buffer start address is not aligned. old:%lx new:%lx" ,  (MS_U32)HVD_BBU_ST_ADDR_IN_BITSTREAMBUF  , gSubHVDCtrl.u32BBUTblInBitstreamBufAddr  );
                    }
                }
                else
                {
                    HVD_SUB_MSG_ERR( "HVD Err: Driver process buffer size is not enough for driver input path. input:%lx required:%lx" ,  (MS_U32)gSubHVDCtrl.MemMap.u32DrvProcessBufSize  , ( HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) <<3) + 8)  );
                    return E_HVD_RET_OUTOF_MEMORY;
                }
            }
        }

        // init AVI NULL packet pattern && RM flush pattern
        if( ( gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
        {
            MS_U8 *pNULLPattern=NULL;
            gSubHVDCtrl.u32NULLPacketAddr = gSubHVDCtrl.MemMap.u32DrvProcessBufAddr;
            //pNULLPattern=(MS_U8 *)(((MS_U32)pu8HVD_DMAcpy_pool)  );
            //HVD_memset( (void*)pNULLPattern, 0 , sizeof( pu8HVD_DMAcpy_pool ) );
            // TODO: use other non-cachable VA addr
            pNULLPattern=(MS_U8 *)(((MS_U32)gSubHVDCtrl.MemMap.u32CodeBufVAddr)  );
            HVD_memset( (void*)pNULLPattern, 0 , 12 );
            switch( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) )
            {
            case HVD_INIT_HW_AVC:
            case HVD_INIT_HW_AVS:
                if(  gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_START_CODE_MASK )
                {   // start code removed
                    gSubHVDCtrl.u32NULLPacketSize = 8;
                    if( gSubHVDCtrl.MemMap.u32DrvProcessBufSize < gSubHVDCtrl.u32NULLPacketSize )
                    {
                        return E_HVD_RET_OUTOF_MEMORY;
                    }
                    HVD_memset( (void*)pNULLPattern, 0xAA , gSubHVDCtrl.u32NULLPacketSize);
                    pNULLPattern[4] = 0X55;
                }
                else // start code remained
                {
                    gSubHVDCtrl.u32NULLPacketSize = 12;
                    if( gSubHVDCtrl.MemMap.u32DrvProcessBufSize < gSubHVDCtrl.u32NULLPacketSize )
                    {
                        return E_HVD_RET_OUTOF_MEMORY;
                    }
                    HVD_memset( (void*)pNULLPattern, 0xAA , gSubHVDCtrl.u32NULLPacketSize);
                    pNULLPattern[0] = 0;
                    pNULLPattern[1] = 0;
                    pNULLPattern[2] = 1;
                    pNULLPattern[3] = 0xFF;
                    pNULLPattern[8] = 0X55;
                    //pNULLPattern[gSubHVDCtrl.u32NULLPacketSize-1] = 0x80;
                }

                if( bBitMIU1 != bCodeMIU1 )
                {
                    #if HVD_ENABLE_BDMA_2_BITSTREAMBUF
                    _HVD_Sub_Chip_Flush_Memory();
                    HVD_dmacpy(gSubHVDCtrl.MemMap.u32DrvProcessBufAddr, gSubHVDCtrl.MemMap.u32CodeBufAddr, gSubHVDCtrl.u32NULLPacketSize);
                    #else
                    HVD_memcpy((void*)(gSubHVDCtrl.MemMap.u32DrvProcessBufVAddr), pNULLPattern, gSubHVDCtrl.u32NULLPacketSize);
                    _HVD_Sub_Chip_Flush_Memory();
                    #endif
                }
                else
                {
                    HVD_memcpy((void*)(gSubHVDCtrl.MemMap.u32DrvProcessBufVAddr), pNULLPattern, gSubHVDCtrl.u32NULLPacketSize);
                    _HVD_Sub_Chip_Flush_Memory();                
                }

                break;

            case HVD_INIT_HW_RM:
                // RM has no NULL packet
                gSubHVDCtrl.u32NULLPacketSize = 0;
                gSubHVDCtrl.u32NULLPacketAddr =0;
                #if HVD_ENABLE_RV_FEATURE
                    gSubHVDCtrl.u32RV_FlushPacketAddr = gSubHVDCtrl.MemMap.u32DrvProcessBufAddr;
                    gSubHVDCtrl.u32RV_FlushPacketSize = 8;
                    if( gSubHVDCtrl.MemMap.u32DrvProcessBufSize < gSubHVDCtrl.u32RV_FlushPacketSize )
                    {
                        return E_HVD_RET_OUTOF_MEMORY;
                    }
                    HVD_memset( (void*)pNULLPattern, 0xFF , gSubHVDCtrl.u32RV_FlushPacketSize);
                    {
                    #if HVD_ENABLE_BDMA_2_BITSTREAMBUF
                        _HVD_Sub_Chip_Flush_Memory();
                        HVD_dmacpy(  gSubHVDCtrl.MemMap.u32DrvProcessBufAddr ,  gSubHVDCtrl.MemMap.u32CodeBufAddr ,  gSubHVDCtrl.u32RV_FlushPacketSize);
                    #else
                        HVD_memcpy((void*)(gSubHVDCtrl.MemMap.u32DrvProcessBufVAddr), pNULLPattern, gSubHVDCtrl.u32RV_FlushPacketSize);
                        _HVD_Sub_Chip_Flush_Memory();
                    #endif
                    }
                #endif
                break;


            default:
                gSubHVDCtrl.u32NULLPacketSize = 0;
                gSubHVDCtrl.u32NULLPacketAddr =0;
                break;
            }
        }
        else
        {
            gSubHVDCtrl.u32NULLPacketSize = 0;
            gSubHVDCtrl.u32NULLPacketAddr =0;
        }
    }

    // reset other driver control variables
    gSubHVDCtrl.u32StepDecodeCnt=0;
    gSubHVDCtrl.u32LastESRptr=0;
    gSubHVDCtrl.u32BBUPacketCnt=0;
    gSubHVDCtrl.u32BBUWptr_Fired=0;
    gSubHVDCtrl.u32LastErrCode=0;
    gSubHVDCtrl.bIsDispInfoChg=0;
    HVD_memset( (void*)&(gSubHVDCtrl.LastNal) ,  0   , sizeof(HVD_Nal_Entry)   );
    HVD_memset( (void*)&(gSubHVDCtrl.LivingStatus) ,  0   , sizeof(HVD_Alive_Status)   );

    // Init HAL variables
    return (HVD_Result) HAL_HVD_Sub_InitVariables((MS_U32)(&gSubHVDCtrl));
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: _DRV_HVD_Sub_SetSyncMode()
/// @brief \b Function \b Description:  specify the way to sync video time stamp and STC.
/// @param -eMode \b IN : sync type
/// @param -u32Arg \b IN : only work under ( eMode == E_HVD_SYNC_ATS) , video delay toward Audio time stamp. It's minimun value is 0, and maximun value is 4500 ( unit:ms)
/// @return -The result of command set sync type
//-----------------------------------------------------------------------------
HVD_Result _DRV_HVD_Sub_SetSyncMode( HVD_Sync_Tbl_Type eMode)
{
    HVD_Result eRet = E_HVD_OK;
    _DRV_HVD_Entry();
    eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_TYPE, eMode);
    _DRV_HVD_Return(eRet );
}

static HVD_Result _DRV_HVD_Sub_InitFW_AVC(void)
{
    MS_U32 u32InitMode = gSubHVDCtrl.InitParams.u32ModeFlag;

    // common settings
    if (!(gSubHVDCtrl.InitParams.u8TurboInit & E_HVD_TURBOINIT_DISPLAY))
    {
        _DRV_HVD_Sub_SetSyncMode((HVD_Sync_Tbl_Type)gSubHVDCtrl.InitParams.u8SyncType);

        if (gSubHVDCtrl.InitParams.u8MinFrmGap)
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_MIN_FRAME_GAP,
                           gSubHVDCtrl.InitParams.u8MinFrmGap);
        }

        if (gSubHVDCtrl.InitParams.u32MaxDecTick)
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_MAX_DEC_TICK,
                           gSubHVDCtrl.InitParams.u32MaxDecTick);
        }

        if (gSubHVDCtrl.InitParams.u16Pitch)
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_PITCH, gSubHVDCtrl.InitParams.u16Pitch);
        }

        if (gSubHVDCtrl.InitParams.bSyncEachFrm)
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_EACH_FRM, TRUE);
        }

        if (gSubHVDCtrl.InitParams.bFastDisplay)
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_FAST_DISP, TRUE);
        }

        if (gSubHVDCtrl.InitParams.bDynamicScaling)
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_DYNAMIC_SCALE, TRUE);
        }

        if (gSubHVDCtrl.InitParams.bUserData)
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_DPO_CC, TRUE);
        }
    }

    if (gSubHVDCtrl.InitParams.u8TimeUnit)
    {
        HAL_HVD_Sub_SetCmd(E_HVD_CMD_TIME_UNIT_TYPE,
                       gSubHVDCtrl.InitParams.u8TimeUnit);
    }

    // specific settings
    switch (u32InitMode & HVD_INIT_MAIN_MASK)
    {
        case HVD_INIT_MAIN_FILE_RAW:
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_DIS_VDEAD, TRUE);
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_IGNORE_ERR_REF, TRUE);

            if (!(gSubHVDCtrl.InitParams.u8TurboInit & E_HVD_TURBOINIT_DISPLAY))
            {
                HAL_HVD_Sub_SetCmd(E_HVD_CMD_REPEAT_LAST_FIELD, TRUE);
            }

            HAL_HVD_Sub_SetCmd(E_HVD_CMD_PARSER_BYPASS, TRUE);

            //HAL_HVD_Sub_SetCmd( E_HVD_CMD_PITCH , 1952 );
            // svd only
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_ES_FULL_STOP, TRUE);
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_BBU_RESIZE, 95);
            //HAL_HVD_Sub_SetCmd( E_HVD_CMD_PLAY , 0);
            break;
        }
        case HVD_INIT_MAIN_FILE_TS:
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_DIS_VDEAD, TRUE);
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_IGNORE_ERR_REF, TRUE);

            if (!(gSubHVDCtrl.InitParams.u8TurboInit & E_HVD_TURBOINIT_DISPLAY))
            {
                HAL_HVD_Sub_SetCmd(E_HVD_CMD_REPEAT_LAST_FIELD, TRUE);
            }
            //HAL_HVD_Sub_SetCmd( E_HVD_CMD_PITCH , 1952 );
            //HAL_HVD_Sub_SetCmd( E_HVD_CMD_SYNC_EACH_FRM ,  TRUE );
            // svd ONLY
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_ES_FULL_STOP, TRUE);

            //HAL_HVD_Sub_SetCmd( E_HVD_CMD_PLAY , 0);
            break;
        }
        case HVD_INIT_MAIN_LIVE_STREAM:
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_FORCE_RESET_HW, TRUE);

            if (gSubHVDCtrl.InitParams.u8TimeUnit)
            {
                HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_VIDEO_DELAY,
                               (MS_U32) HVD_DTV_VIDEO_DELAY);
            }
            else
            {
                HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_VIDEO_DELAY,
                               (MS_U32) HVD_DTV_VIDEO_DELAY*90);
            }
            //HAL_HVD_Sub_SetCmd( E_HVD_CMD_PITCH , 1984 );
            //HAL_HVD_Sub_SetCmd( E_HVD_CMD_MAX_DEC_TICK ,  6400000); //for GP only
            break;
        }
        default:
            break;
    }

    // svd only
    //HAL_HVD_Sub_SetCmd( E_HVD_CMD_FRAME_BUF_RESIZE , (gSubHVDCtrl.MemMap.u32BitstreamBufSize+gSubHVDCtrl.MemMap.u32CodeBufSize+gSubHVDCtrl.MemMap.u32FrameBufSize) >>20);
    return E_HVD_OK;
}

HVD_Result _DRV_HVD_Sub_InitFW_RM(void)
{
    MS_U32 u32InitMode = gSubHVDCtrl.InitParams.u32ModeFlag;

    // common settings
    if( !(gSubHVDCtrl.InitParams.u8TurboInit & E_HVD_TURBOINIT_DISPLAY)  )
    {
        _DRV_HVD_Sub_SetSyncMode( (HVD_Sync_Tbl_Type)gSubHVDCtrl.InitParams.u8SyncType);
        if( gSubHVDCtrl.InitParams.u16Pitch)
        {
            HAL_HVD_Sub_SetCmd( E_HVD_CMD_PITCH ,  gSubHVDCtrl.InitParams.u16Pitch);
        }
        if( gSubHVDCtrl.InitParams.bSyncEachFrm)
        {
            HAL_HVD_Sub_SetCmd( E_HVD_CMD_SYNC_EACH_FRM ,  TRUE);
        }
        if( gSubHVDCtrl.InitParams.bFastDisplay)
        {
            HAL_HVD_Sub_SetCmd( E_HVD_CMD_FAST_DISP ,  TRUE);
        }
        if( gSubHVDCtrl.InitParams.bDynamicScaling)
        {
            HAL_HVD_Sub_SetCmd( E_HVD_CMD_DYNAMIC_SCALE ,  TRUE);
        }
    }
    if( gSubHVDCtrl.InitParams.u8TimeUnit)
    {
        HAL_HVD_Sub_SetCmd( E_HVD_CMD_TIME_UNIT_TYPE ,  gSubHVDCtrl.InitParams.u8TimeUnit);
    }
    // specific settings
    switch( u32InitMode & HVD_INIT_MAIN_MASK )
    {
    case HVD_INIT_MAIN_FILE_RAW:
        break;
    case HVD_INIT_MAIN_FILE_TS:
    case HVD_INIT_MAIN_LIVE_STREAM:
    default:
        break;
    }

    // svd only
    //HAL_HVD_Sub_SetCmd( E_HVD_CMD_FRAME_BUF_RESIZE , (gSubHVDCtrl.MemMap.u32BitstreamBufSize+gSubHVDCtrl.MemMap.u32CodeBufSize+gSubHVDCtrl.MemMap.u32FrameBufSize) >>20);

    // fix FFx4 display error
    HAL_HVD_Sub_SetCmd( E_HVD_CMD_FORCE_RESET_HW , TRUE );

    return E_HVD_OK;
}

static HVD_Result _DRV_HVD_Sub_Check_FW_Version(void)
{
    // check FW version ID
    if (!(gSubHVDCtrl.InitParams.u8TurboInit & E_HVD_TURBOINIT_CHECK))
    {
#if (HVD_HW_VERSION == HVD_HW_SVD)
        if (HVD_FW_VERSION != HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID))
        {
            HVD_SUB_MSG_ERR("HVD: fw interface and binary is not the same. fw ID: interface:%lx binary:%lx\n",
                        (MS_U32)HVD_FW_VERSION,
                        HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID));
            return E_HVD_FAIL;
        }
        // check if release required
        if (!(gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_DBG_FW))
        {
            // check debug version
            if ((HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID) & HVD_FW_VER_RELEASE_VERSION_MASK) < HVD_FW_VER_RELEASE_VERSION)
            {
                HVD_SUB_MSG_MUST("HVD: FW is not release verion: binary:%lx\n",
                             HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID));
                //return E_HVD_FAIL;
            }
        }
#else

        if( (HVD_FW_IF_VERSION>>16) != (HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_IF_VERSION_ID)>>16) )
        {
            HVD_SUB_MSG_ERR("HVD: fw interface and binary is not the same. fw ID: interface:%lx binary:%lx\n",
                        (MS_U32)HVD_FW_IF_VERSION,
                        HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_IF_VERSION_ID));
            return E_HVD_FAIL;
        }

        // check if release required
        if (!(gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_DBG_FW))
        {
            // check debug version
            if (!(HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID) & HVD_FW_VER_RELEASE_VERSION))
            {
                HVD_SUB_MSG_MUST("HVD: FW is not release verion: binary:%lx\n",
                             HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID));
                //return E_HVD_FAIL;
            }

            if (HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID) & HVD_FW_VER_DUMP_DEBUG_MSG)
            {
                HVD_SUB_MSG_MUST("HVD: FW release verion, but it has debug message. binary:%lx\n",
                             HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_VERSION_ID));
                //return E_HVD_FAIL;
            }
        }
#endif
    }
    return E_HVD_OK;
}

static HVD_Result _DRV_HVD_Sub_InitFW(void)
{
    HVD_Result eRet = E_HVD_OK;

    eRet = _DRV_HVD_Sub_Check_FW_Version();
    if (eRet != E_HVD_OK)
    {
        return eRet;
    }

    switch (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK)
    {
        case HVD_INIT_HW_AVC:
            eRet = _DRV_HVD_Sub_InitFW_AVC();
            break;
        case HVD_INIT_HW_AVS:
            eRet = _DRV_HVD_Sub_InitFW_AVC();
            break;
        case HVD_INIT_HW_RM:
            eRet = _DRV_HVD_Sub_InitFW_RM();
            break;
        default:
            break;
    }
    return eRet;
}

MS_BOOL _DRV_HVD_Sub_IsAllBufferEmpty( void )
{
    MS_U32 playbacktype = gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK ;
    if( playbacktype == HVD_INIT_MAIN_FILE_RAW)
    {
        if(HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_Q_NUMB) != 0)
        {
            return FALSE;
        }
    }
    else //if( playbacktype == HVD_INIT_MAIN_FILE_TS ) TSP input
    {
        #if 0
        if( HAL_HVD_Sub_GetData( E_HVD_GDATA_ES_READ_PTR) != HAL_HVD_Sub_GetData( E_HVD_GDATA_ES_WRITE_PTR) )
        {
            return FALSE;
        }
        #endif
    }
    {   // other Queues
        if( ( (HAL_HVD_Sub_GetData(E_HVD_GDATA_DEC_Q_NUMB) == 0) &&
               (HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_Q_NUMB) == 0) )
               &&  (HAL_HVD_Sub_GetData(E_HVD_GDATA_VPU_IDLE_CNT) > HVD_FW_IDLE_THRESHOLD)   )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

#if defined(REDLION_LINUX_KERNEL_ENVI)
MS_S32 _DRV_HVD_Sub_ISRHandler(void)
#else
void _DRV_HVD_Sub_ISRHandler(void)
#endif
{
    if( gHVDISRCtrl.pfnISRCallBack != NULL )
    {
        gHVDISRCtrl.bInISR = TRUE;
        gHVDISRCtrl.u32ISRInfo = HAL_HVD_Sub_GetData( E_HVD_GDATA_HVD_ISR_STATUS );
        if( gHVDISRCtrl.u32ISRInfo )
        {
            HAL_HVD_Sub_Enable_ISR(FALSE);
            gHVDISRCtrl.pfnISRCallBack();
            HAL_HVD_Sub_Enable_ISR(TRUE);
        }
    }
    else
    {
        HVD_SUB_MSG_ERR( "DRV HVD Err: ISR callback is NULL.\n"  );
    }
    gHVDISRCtrl.bInISR = FALSE;
    HAL_HVD_Sub_SetClearISR();
    OSAL_HVD_ISR_Enable();
    #if defined(REDLION_LINUX_KERNEL_ENVI)
    return 1;
    #endif
}


HVD_Result _DRV_HVD_Sub_RecoverySettings(void)
{
// TODO: complete this function. and consider more cases. step decoder, display?
    HVD_Result eRst=E_HVD_OK;

    switch( gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK )
    {
    case HVD_INIT_MAIN_LIVE_STREAM:
// temp solution
    // must before play().
        if( (gSubHVDCtrl.Settings.DispInfoTH.u32FrmrateLowBound != 0 )||
            (gSubHVDCtrl.Settings.DispInfoTH.u32FrmrateUpBound   != 0)  ||
            (gSubHVDCtrl.Settings.DispInfoTH.u32MvopLowBound     != 0)  ||
            (gSubHVDCtrl.Settings.DispInfoTH.u32MvopUpBound       != 0)   )
        {
            HAL_HVD_Sub_SetData(  E_HVD_SDATA_DISP_INFO_TH ,  (MS_U32)(&gSubHVDCtrl.Settings.DispInfoTH) );
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_UPDATE_DISP_THRESHOLD, 0);
        }
        if( gSubHVDCtrl.Settings.u32IsrEvent )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_ISR_TYPE, gSubHVDCtrl.Settings.u32IsrEvent);
        }
        HAL_HVD_Sub_Enable_ISR(gSubHVDCtrl.Settings.bEnISR);
  // play()
        HAL_HVD_Sub_SetCmd(E_HVD_CMD_PLAY, 0);
        if(  gSubHVDCtrl.Settings.u8SkipMode )
        {
            if(E_HVD_OK != (eRst = MDrv_HVD_Sub_SetSkipDecMode((HVD_Skip_Decode)(gSubHVDCtrl.Settings.u8SkipMode))))
            {
                HVD_SUB_MSG_ERR( "_DRV_HVD_Sub_RecoverySettings : Set Skip Mode fail!!.\n"  );
                return eRst;
            }
        }
        if(  gSubHVDCtrl.Settings.bIsShowErrFrm )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_ERR_FRM, TRUE);
        }
        if(  gSubHVDCtrl.Settings.u8FrcMode )
        {
            if(E_HVD_OK != (eRst = MDrv_HVD_Sub_SetFrcMode((HVD_FrmRateConv_Mode)(gSubHVDCtrl.Settings.u8FrcMode))))
            {
                HVD_SUB_MSG_ERR( "_DRV_HVD_Sub_RecoverySettings : Set Frc Mode fail!!.\n"  );
                return eRst;
            }
        }
        if(  gSubHVDCtrl.Settings.bIsErrConceal )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_ERR_CONCEAL, TRUE);
        }
        if(  gSubHVDCtrl.Settings.bAutoFreeES )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_AUTO_FREE_ES, TRUE);
        }
        if(  gSubHVDCtrl.Settings.bDisDeblocking )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_DIS_DBF, TRUE);
        }
        if(  gSubHVDCtrl.Settings.bDisQuarterPixel )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_DIS_QUART_PIXEL, TRUE);
        }

        if(  gSubHVDCtrl.Settings.bIsSyncOn )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_ACTIVE, TRUE);
        }
        if(  gSubHVDCtrl.Settings.u32SyncTolerance )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_TOLERANCE, gSubHVDCtrl.Settings.u32SyncTolerance);
        }
        if(  gSubHVDCtrl.Settings.u32SyncRepeatTH )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_THRESHOLD, gSubHVDCtrl.Settings.u32SyncRepeatTH);
        }
        if(  gSubHVDCtrl.Settings.u32SyncVideoDelay )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_VIDEO_DELAY, gSubHVDCtrl.Settings.u32SyncVideoDelay);
        }
        if(  gSubHVDCtrl.Settings.u32SyncFreeRunTH )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_FREERUN_THRESHOLD, gSubHVDCtrl.Settings.u32SyncFreeRunTH);
        }
#if !(defined(CHIP_T2) || defined(CHIP_T7))
        if( gSubHVDCtrl.Settings.u32MiuBurstLevel != 0xFFFFFFFF )
        {
            HAL_HVD_Sub_SetCmd(E_HVD_CMD_MIU_BURST_CNT, gSubHVDCtrl.Settings.u32MiuBurstLevel);
        }
#endif
        //HAL_HVD_Sub_SetCmd(E_HVD_CMD_BLUE_SCREEN, FALSE);
        gSubHVDCtrl.bStepDecoding=0;
        break;

    case HVD_INIT_MAIN_FILE_RAW:
    default:
        gSubHVDCtrl.bStepDecoding=0;
        HAL_HVD_Sub_SetCmd(E_HVD_CMD_PLAY, 0);
        break;
    }
    return eRst;
}

static HVD_Result _DRV_HVD_Sub_Rst(MS_BOOL bErrHandle)
{
    HVD_Result eRst = E_HVD_RET_ILLEGAL_ACCESS;

    if (bErrHandle == TRUE)
    {
        HVD_SUB_MSG_ERR("HVD Not supported reset(TRUE) yet\n");
        return E_HVD_RET_INVALID_PARAMETER;
    }

    bHVDIsIniting = TRUE;

    HAL_HVD_Sub_Enable_ISR(FALSE);

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    eRst = _DRV_HVD_Sub_RstVariables();

    if (eRst != E_HVD_OK)
    {
        goto DRV_HVD_Rst_Failed;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    eRst = _DRV_HVD_Sub_InitShareMem();

    if (eRst != E_HVD_OK)
    {
        goto DRV_HVD_Rst_Failed;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    eRst = _DRV_HVD_Sub_InitRegCPU();

    if (eRst != E_HVD_OK)
    {
        goto DRV_HVD_Rst_Failed;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    eRst = _DRV_HVD_Sub_InitFW();

    if (eRst != E_HVD_OK)
    {
        goto DRV_HVD_Rst_Failed;
    }

    eRst = E_HVD_OK;

    HAL_HVD_Sub_Enable_ISR(gSubHVDCtrl.Settings.bEnISR);

    bHVDIsIniting = FALSE;
    return eRst;

    DRV_HVD_Rst_Failed :
    HAL_HVD_Sub_Enable_ISR(gSubHVDCtrl.Settings.bEnISR);
    bHVDIsIniting = FALSE;

    return eRst;
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

// System
//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_PowerCtrl()
/// @brief \b Function \b Description:  Enable/Disable HVD HW clock
/// @param -bEnable \b IN :  Enable/Disable HVD HW clock
///                 -FALSE(0): Disable
///                 -TRUE(1): Enable
//-----------------------------------------------------------------------------
void MDrv_HVD_Sub_PowerCtrl( MS_BOOL bEnable )
{
    HVD_SUB_MSG_TRACE();
    HAL_HVD_Sub_MVD_PowerCtrl( bEnable);
    HAL_HVD_Sub_PowerCtrl(bEnable);
    HAL_VPU_PowerCtrl(bEnable);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetOSRegBase()
/// @brief \b Function \b Description:  Set system register base
/// @param -u32RegBaseAddr \b IN :  system register base
//-----------------------------------------------------------------------------
void MDrv_HVD_Sub_SetOSRegBase( MS_U32 u32RegBaseAddr )
{
    HAL_HVD_Sub_InitRegBase( u32RegBaseAddr );
}

// Action

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_Init()
/// @brief \b Function \b Description:  HVD driver initialization
/// @param -pStMemCfg \b IN :  pointer to the memory config of HVD driver
/// @param -pStInitSettings \b IN :  Initialization of HVD driver
/// @return -The result of initialization process
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_Init(HVD_MemCfg *pStMemCfg, HVD_Init_Settings *pStInitSettings)
{
    HVD_Result eRst = E_HVD_FAIL;

    HVD_SUB_MSG_TRACE();

#if HVD_ENABLE_TIME_MEASURE
    u32SubInitSysTimeBase = HVD_GetSysTime_ms();
#endif

    if ((pStMemCfg == NULL) || (pStInitSettings == NULL))
    {
        HVD_SUB_MSG_ERR("HVD Err: Init params are invalid\n");
        return E_HVD_RET_INVALID_PARAMETER;
    }

#if HVD_ENABLE_AUTO_SET_REG_BASE
    {
        MS_U32 u32NonPMBankSize = 0, u32RiuBaseAdd = 0;

        if (!MDrv_MMIO_GetBASE(&u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
        {
            HVD_SUB_MSG_ERR("HVD Err: MMIO_GetBASE failure\n");
            return eRst;
        }
        else
        {
            HVD_SUB_MSG_INFO("HVD:1 u32RiuBaseAdd = %lx\n", u32RiuBaseAdd);
            HAL_HVD_Sub_InitRegBase(u32RiuBaseAdd);
        }
    }
#endif

    HVD_SUB_MSG_INFO("HVD: system call type: Mutex:%d clock:%d delay:%d asm_sync:%d\n",
                 HVD_ENABLE_MUTEX_PROTECT,
                 HVD_SYSTEM_CLOCK_TYPE,
                 HVD_SYSTEM_DELAY_MS_TYPE,
                 HVD_MEMORY_BARRIER_TYPE);

    if (u32SubUartCtrl & E_HVD_UART_CTRL_INFO)
    {
        MS_U32 u32delaytime = 5;
        MS_U32 u32FirstTime = HVD_GetSysTime_ms();
        MS_U32 u32SecondTime= 0;

        HVD_Delay_ms(u32delaytime);
        u32SecondTime = HVD_GetSysTime_ms();

        HVD_SUB_MSG_INFO("HVD: MSOS API check: 1st:%lu 2nd:%lu delay:%lu dif:%lu\n",
                     u32FirstTime,
                     u32SecondTime,
                     u32delaytime,
                     u32SecondTime - u32FirstTime);
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    if (bHVDIsInited == TRUE)
    {
        HVD_SUB_MSG_ERR("HVD Warn: re-init HVD Driver\n");

#if HVD_ENABLE_REINIT_FAILED
        eRst = E_HVD_RET_RE_INIT;
        return eRst;
#endif
    }

    // disable ISR first
    HAL_HVD_Sub_Enable_ISR(FALSE);

    eRst = _DRV_HVD_Sub_InitVariables(pStMemCfg,
              (HVD_Init_Params *) pStInitSettings);

    if (eRst != E_HVD_OK)
    {
        return eRst;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    _DRV_HVD_Entry();

    eRst = _DRV_HVD_Sub_Check_Cmd(E_HVD_CHECK_CMD_INIT);

    if (eRst != E_HVD_OK)
    {
        _DRV_HVD_Return(eRst);
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    eRst = _DRV_HVD_Sub_Rst(FALSE);

    if (eRst != E_HVD_OK)
    {
        _DRV_HVD_Return(eRst);
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_SUB_MSG_MUST("HVD Time Measure:%d (%s %d) \n",
                 HVD_GetSysTime_ms() - u32SubInitSysTimeBase,
                 __FUNCTION__,
                 __LINE__);
#endif

    gSubHVDCtrl.u32CtrlMode |= HVD_CTRL_INIT_FINISHED;
    bHVDIsInited = TRUE;
    eRst = E_HVD_OK;

    HVD_SUB_MSG_INFO("HVD driver Init successfully.\n");

    _DRV_HVD_Return(eRst);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_Rst()
/// @brief \b Function \b Description:  Reset HVD driver
/// @param -bErrHandle \b IN :  reset option HVD driver
///                 -FALSE(0): Reset HVD to clear mode.( do not recovery SPS)
///                 -TRUE(1): Reset HVD to clear mode, and recovery SPS.
/// @return -The result of reset process
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_Rst(MS_BOOL bErrHandle)
{
    HVD_Result eRet=E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    if( bErrHandle == TRUE )
    {
        HVD_SUB_MSG_ERR( "HVD Not supported reset(TRUE) yet\n");
        return E_HVD_RET_INVALID_PARAMETER;
    }
    if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) != HVD_INIT_MAIN_LIVE_STREAM)
    {
        HVD_SUB_MSG_ERR( "HVD rst() only support live stream mode\n");
        return E_HVD_RET_INVALID_PARAMETER;
    }
    eRet = _DRV_HVD_Sub_Rst(bErrHandle);
    if( eRet != E_HVD_OK )
    {
        return (eRet);
    }
    eRet = _DRV_HVD_Sub_RecoverySettings();
    if( eRet != E_HVD_OK )
    {
        return ( eRet);
    }
    gSubHVDCtrl.u32CtrlMode |= HVD_CTRL_INIT_FINISHED;
    bHVDIsInited=TRUE;
    return (eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_Play()
/// @brief \b Function \b Description:  Play HVD
/// @return -The result of command play
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_Play(void)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();

    _DRV_HVD_Inited(eRet);

    _DRV_HVD_Entry();

#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if( MDrv_HVD_Sub_GetPlayState() != E_HVD_GSTATE_PLAY || gSubHVDCtrl.bStepDecoding )
#endif
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_PLAY, 0);
        if( eRet != E_HVD_OK )
        {
            _DRV_HVD_Return(eRet );
        }
    }
    // step display off
    eRet = (HVD_Result)HAL_HVD_Sub_SetData(E_HVD_SDATA_TRIGGER_DISP, 0);
    if( eRet == E_HVD_OK  )
    {
        gSubHVDCtrl.u32CtrlMode &= ~HVD_CTRL_DISPLAY_CTRL;
    }
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_Exit()
/// @brief \b Function \b Description:  Stop HVD and release resource.
/// @return -The result of command stop
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_Exit(void)
{
    MS_U32 u32Timer=0;
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Rsting(eRet);
    bHVDIsInited=FALSE;
    do
    {
        u32Timer++;
        if(u32Timer>=HVD_FW_EXIT_ACTION_TIMEOUT)
        {
            eRet = E_HVD_RET_TIMEOUT;
            return eRet;
        }
        else
        {
            HVD_Delay_ms(1);
        }
    }while(_DRV_HVD_Ctrl(HVD_CTRL_PROCESSING));
    // release ISR callback
    HAL_HVD_Sub_Enable_ISR(FALSE);
    OSAL_HVD_ISR_Disable();
    if( gHVDISRCtrl.bRegISR )
    {
        OSAL_HVD_ISR_Detach();
    }
    // close HVD FW
    eRet = (HVD_Result)HAL_HVD_Sub_DeInit();
    // reset internal control variables
    HVD_memset( (void*)&gSubHVDCtrl  , 0 , sizeof(HVD_Drv_Ctrl) );
    HVD_memset( (void*)&gHVDISRCtrl  , 0 , sizeof(HVD_ISR_Ctrl) );
    // TODO: check it is really stop
    //_DRV_HVD_Return(eRet );
    return eRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_Pause()
/// @brief \b Function \b Description:  Pause HVD
/// @return -The result of command pause
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_Pause(void)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_PAUSE, 0);
    if( eRet !=  E_HVD_OK)
    {
        _DRV_HVD_Return(eRet );
    }
    // step display off
    eRet = (HVD_Result)HAL_HVD_Sub_SetData(E_HVD_SDATA_TRIGGER_DISP, 0);
    if( eRet == E_HVD_OK  )
    {
        gSubHVDCtrl.u32CtrlMode &=~HVD_CTRL_DISPLAY_CTRL;
    }
    _DRV_HVD_Return(eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_Flush()
/// @brief \b Function \b Description:  Flush queue and buffer
/// @param -bShowLast \b IN :  TRUE / FALSE
///                 -FALSE(0): show current displayed frame
///                 -TRUE(1): show last decoded frame
/// @return -The result of command flush
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_Flush(MS_BOOL bShowLast)
{
    MS_U32 u32Times = 0;
    MS_BOOL bPlayback=FALSE;
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();

    // pause first
    if(  MDrv_HVD_Sub_GetPlayState() != E_HVD_GSTATE_PAUSE )
    {
        bPlayback = TRUE;
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_PAUSE, 0);
        if( eRet !=  E_HVD_OK)
        {
            _DRV_HVD_Return(eRet );
        }
        // check flush done
        while( 1 )
        {
            if(  MDrv_HVD_Sub_GetPlayState() == E_HVD_GSTATE_PAUSE )
            {
                break;
            }
            u32Times++;
            HVD_Delay_ms(1);
            if( u32Times > 100 )
            {
                HVD_SUB_MSG_INFO("HVD info: pause in flush() not finished.\n");
                break;
            }
        }
    }
    // step display off
    eRet = (HVD_Result)HAL_HVD_Sub_SetData(E_HVD_SDATA_TRIGGER_DISP, 0);
    if( eRet == E_HVD_OK  )
    {
        gSubHVDCtrl.u32CtrlMode &=~HVD_CTRL_DISPLAY_CTRL;
    }

#if HVD_ENABLE_RV_FEATURE
    if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_RM )
    {
        if ( gSubHVDCtrl.LastNal.bRVBrokenPacket )
        {
            HVD_SUB_MSG_INFO("HVD Info: push dummy packet for broken by us packet\n");
            gHVDPacket.u32Length=gSubHVDCtrl.u32RV_FlushPacketSize;
            gHVDPacket.u32Staddr=gSubHVDCtrl.u32RV_FlushPacketAddr-gSubHVDCtrl.MemMap.u32BitstreamBufAddr;
            gHVDPacket.u32OriPktAddr=gHVDPacket.u32Staddr;
            gHVDPacket.bRVBrokenPacket = FALSE;
            eRet=( HVD_Result )HAL_HVD_Sub_PushPacket( (HVD_BBU_Info*)&gHVDPacket );
        }
    }
#endif

    // fire all packet to FW
    if( HAL_HVD_Sub_GetData( E_HVD_GDATA_BBU_WRITE_PTR_FIRED ) != HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_WRITE_PTR) )
    {
        HAL_HVD_Sub_UpdateESWptr_Fire();
    }
    // send command
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_FLUSH, (MS_U32)bShowLast);
    if( eRet !=  E_HVD_OK)
    {
        _DRV_HVD_Return(eRet );
    }
    // check flush done only for raw file mode
    if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
    {
        while( 1 )
        {
            //_DRV_HVD_PushDummy();
            if( _DRV_HVD_Sub_IsAllBufferEmpty() )
            {
                eRet = E_HVD_OK;
                break;
            }
            u32Times++;
            HVD_Delay_ms(1);
            if ( u32Times > 200 )
            {
                HVD_SUB_MSG_ERR("HVD Err: Flush() timeout failed: BBU:%lu Dec:%lu Disp:%lu Idle:%lu ESr:%lx ESw:%lx\n" ,
                    HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_Q_NUMB),
                    HAL_HVD_Sub_GetData(E_HVD_GDATA_DEC_Q_NUMB),
                    HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_Q_NUMB),
                    HAL_HVD_Sub_GetData(E_HVD_GDATA_VPU_IDLE_CNT),
                    HAL_HVD_Sub_GetData(E_HVD_GDATA_ES_READ_PTR),
                    HAL_HVD_Sub_GetData(E_HVD_GDATA_ES_WRITE_PTR) );
                eRet = E_HVD_FAIL;
                break;
            }
        }
        // reset byte_cnt
        HAL_HVD_Sub_RstPTSCtrlVariable();
    }
    HAL_HVD_Sub_FlushRstShareMem();

    // MediaCodec need this to let scan mode to be normal.
    if( 0)//bPlayback )
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_PLAY, 0);
        if( eRet != E_HVD_OK )
        {
            _DRV_HVD_Return(eRet );
        }
    }
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_StepDisp()
/// @brief \b Function \b Description:  Trigger HVD to show one frame
/// @return -The result of command trigger display
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_StepDisp(void)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    // check command
    eRet=_DRV_HVD_Sub_Check_Cmd( E_HVD_CHECK_CMD_TRIGGER_DISP );
    if( eRet != E_HVD_OK )
    {
        _DRV_HVD_Return(eRet );
    }

    // step display on
    if( _DRV_HVD_Ctrl(HVD_CTRL_DISPLAY_CTRL) )
    {
        if( MDrv_HVD_Sub_IsFrameShowed() )
        {
            eRet = (HVD_Result)HAL_HVD_Sub_SetData(E_HVD_SDATA_TRIGGER_DISP, 1);
        }
        else
        {
            HVD_SUB_MSG_INFO("HVD Warn: Previous Step Display command is not finished\n");
            eRet =E_HVD_OK;
        }
    }
    else
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetData(E_HVD_SDATA_TRIGGER_DISP, 1);
        if( eRet == E_HVD_OK  )
        {
            _DRV_HVD_SetCtrl(HVD_CTRL_DISPLAY_CTRL);
        }
    }
    // enter play mode
    if( 1)//HAL_HVD_GetData(E_HVD_GDATA_FW_STATE) == E_HVD_FW_PAUSE )
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_PLAY, 0);
        if( eRet !=  E_HVD_OK)
        {
            _DRV_HVD_Return(eRet );
        }
    }
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_StepDecode()
/// @brief \b Function \b Description:  Step decode one frame
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_StepDecode(void)
{
    MS_U32 u32Times=0;
    MS_U32 FWState=0;
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    // check if step decoding
    if( gSubHVDCtrl.bStepDecoding)
    {
        if( gSubHVDCtrl.u32StepDecodeCnt == HAL_HVD_Sub_GetData(E_HVD_GDATA_DECODE_CNT) )
        {
            eRet = E_HVD_RET_NOTREADY;
            _DRV_HVD_Return(eRet );
        }
    }
    // Pause first
    FWState = HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_STATE);
    if( FWState != (MS_U32)E_HVD_FW_PAUSE )
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_PAUSE, 0);
        if( eRet !=  E_HVD_OK)
        {
            _DRV_HVD_Return(eRet );
        }
        while( FWState != ((MS_U32)E_HVD_FW_PAUSE)
            && u32Times <= 10000)
        {
            FWState = HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_STATE);
            u32Times++;
            HVD_Delay_ms(1);
        }
        if( HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_STATE) != E_HVD_FW_PAUSE )
        {
            eRet = E_HVD_FAIL;
            _DRV_HVD_Return(eRet );
        }
    }
    gSubHVDCtrl.bStepDecoding=TRUE;
    // get decode cnt
    gSubHVDCtrl.u32StepDecodeCnt = HAL_HVD_Sub_GetData(E_HVD_GDATA_DECODE_CNT);
    // step decode
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_STEP_DECODE, 0);
    _DRV_HVD_Return(eRet );
}

// set command, action, status, input
//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_PushQueue()
/// @brief \b Function \b Description:  push one entry into the decoding table(BBU table).
/// @param -pInfo \b IN :  Pointer to the information of input packet.
/// @return -The result of command push queue
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_PushQueue(HVD_Packet_Info* pInfo)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    MS_BOOL bNULLPacket=FALSE;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();

    if( pInfo ==NULL  )
    {
        eRet=E_HVD_RET_INVALID_PARAMETER;
        _DRV_HVD_Return(eRet );
    }
    else if( pInfo->u32Length >= 0x200000) // HW bbu tag limitation
    {
        HVD_SUB_MSG_ERR(  "HVD Err: input packet size(0x%lx) larger than max packet size(0x%lx)\n " , pInfo->u32Length  , 0x200000UL  );
        eRet=E_HVD_RET_INVALID_PARAMETER;
        _DRV_HVD_Return(eRet );
    }
    else if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_INPUT_MASK) == HVD_INIT_INPUT_TSP )
    {
        HVD_SUB_MSG_ERR(  "HVD Err: Init mode is TSP input and PushQueue() is not supported in TSP input."  );
        eRet=E_HVD_RET_ILLEGAL_ACCESS;
        _DRV_HVD_Return(eRet );
    }
    else if( ((gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) != HVD_INIT_HW_RM) &&
       (pInfo->u32Staddr >= gSubHVDCtrl.MemMap.u32BitstreamBufSize) )
    {
        eRet=E_HVD_RET_INVALID_PARAMETER;
        _DRV_HVD_Return(eRet );
    }

    //HVD_memcpy((void*)(&gHVDPacket), (void*)pInfo, sizeof(HVD_Packet_Info));
    gHVDPacket.u32ID_H=pInfo->u32ID_H;
    gHVDPacket.u32ID_L=pInfo->u32ID_L;
    gHVDPacket.u32Length=pInfo->u32Length;
    gHVDPacket.u32TimeStamp=pInfo->u32TimeStamp;
    if( pInfo->u32Staddr & HVD_RV_BROKEN_BY_US_MASK )
    {
        gHVDPacket.bRVBrokenPacket= TRUE;
        gHVDPacket.u32OriPktAddr=pInfo->u32Staddr & (~HVD_RV_BROKEN_BY_US_MASK);
        gHVDPacket.u32Staddr=pInfo->u32Staddr& (~HVD_RV_BROKEN_BY_US_MASK);
    }
    else
    {
        gHVDPacket.bRVBrokenPacket= FALSE;
        gHVDPacket.u32OriPktAddr=pInfo->u32Staddr;
        gHVDPacket.u32Staddr=pInfo->u32Staddr;
    }

    // invalid packet
    if(  (gHVDPacket.u32TimeStamp != HVD_U32_MAX) && (gHVDPacket.u32Length==0))
    {
        HVD_SUB_MSG_INFO(  "HVD Err: Invalid Packet(size:0x%lx PTS:0x%lx)\n " , gHVDPacket.u32Length  , gHVDPacket.u32TimeStamp );
        eRet=E_HVD_RET_INVALID_PARAMETER;
        _DRV_HVD_Return(eRet );
    }
    // AVI NULL packet
#if HVD_ENABLE_AUTO_AVI_NULL_PACKET
    else if((gHVDPacket.u32TimeStamp == HVD_U32_MAX) && (gHVDPacket.u32Length==0))
    {
        if( gSubHVDCtrl.bNoDrvProccBuf)
        {
            HVD_SUB_MSG_INFO(  "HVD Err: AVI Null Packet(size:0x%lx PTS:0x%lx), but do not have enough driver process buffer(0x%lx)\n " , pInfo->u32Length  , gHVDPacket.u32TimeStamp , gSubHVDCtrl.MemMap.u32DrvProcessBufSize);
            eRet=E_HVD_RET_ILLEGAL_ACCESS;
            _DRV_HVD_Return(eRet );
        }
        else
        {
            bNULLPacket=TRUE;
            gHVDPacket.u32Length = gSubHVDCtrl.u32NULLPacketSize;
            gHVDPacket.u32Staddr= gSubHVDCtrl.u32NULLPacketAddr - gSubHVDCtrl.MemMap.u32BitstreamBufAddr;
        }
    }
#endif
    // the else are all normal cases.
    if( MDrv_HVD_Sub_GetBBUVacancy() != 0)
    {
        {
            MS_U32 u32ESRptr=HAL_HVD_Sub_GetData(E_HVD_GDATA_ES_READ_PTR );
            MS_U32 u32ESWptr=HAL_HVD_Sub_GetData(E_HVD_GDATA_ES_WRITE_PTR);
            if( ( u32ESWptr >= u32ESRptr ) &&
                 ( gHVDPacket.u32Staddr + gHVDPacket.u32Length >= gSubHVDCtrl.MemMap.u32BitstreamBufSize )  )
            {
                HVD_SUB_MSG_ERR(  "HVD Warn: input packet (%lx %lx %lx) may cause bitstream buffer overflow(%lx %lx) %lx\n " , gHVDPacket.u32Staddr , gHVDPacket.u32Length  ,  gHVDPacket.u32Staddr+gHVDPacket.u32Length  , u32ESRptr ,u32ESWptr , gSubHVDCtrl.MemMap.u32BitstreamBufSize );
            }
            if( ( u32ESWptr < u32ESRptr ) &&
                 ( gHVDPacket.u32Staddr + gHVDPacket.u32Length >= u32ESRptr )  )
            {
                HVD_SUB_MSG_ERR(  "HVD Warn: input packet (%lx %lx %lx) may overwrite undecoded data(%lx %lx)\n " , gHVDPacket.u32Staddr , gHVDPacket.u32Length  ,  gHVDPacket.u32Staddr+gHVDPacket.u32Length  , u32ESRptr ,u32ESWptr );
            }
            if( HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_Q_NUMB) && (!bNULLPacket) &&
                 (gSubHVDCtrl.MemMap.u32DrvProcessBufSize != 0) &&
                 ( gSubHVDCtrl.MemMap.u32BitstreamBufAddr<= gSubHVDCtrl.MemMap.u32DrvProcessBufAddr  ) &&
                 ( gSubHVDCtrl.MemMap.u32DrvProcessBufAddr < (gSubHVDCtrl.MemMap.u32BitstreamBufAddr + gSubHVDCtrl.MemMap.u32BitstreamBufSize)) )
            {
                    MS_U32 u32Lower = gSubHVDCtrl.MemMap.u32DrvProcessBufAddr - gSubHVDCtrl.MemMap.u32BitstreamBufAddr;
                    MS_U32 u32Upper = u32Lower+gSubHVDCtrl.MemMap.u32DrvProcessBufSize;
                    if( ( (u32Lower <= gHVDPacket.u32Staddr) && (gHVDPacket.u32Staddr < u32Upper)) ||
                         ( (u32Lower <= (gHVDPacket.u32Staddr + gHVDPacket.u32Length) ) && ( (gHVDPacket.u32Staddr+gHVDPacket.u32Length) < u32Upper)) ||
                         ( (gHVDPacket.u32Staddr < u32Lower) && (u32Upper<=(gHVDPacket.u32Staddr+gHVDPacket.u32Length)) )   )
                    {
                        HVD_SUB_MSG_ERR(  "HVD Warn: input packet (%lx %lx %lx) is located in HVD driver process buffer(%lx %lx)\n " , gHVDPacket.u32Staddr , gHVDPacket.u32Length  ,  gHVDPacket.u32Staddr+gHVDPacket.u32Length  , u32Lower ,u32Upper );
                    }
            }
         //for debug
            if( 0)
            {
                 HVD_SUB_MSG_INFO(  "HVD : %lu (%lu %lu) ID:%lx input packet (%lx %lx %lx) (%lx %lx %lx) (%lu %lu %lu)\n " ,
                    gSubHVDCtrl.u32BBUPacketCnt , MDrv_HVD_Sub_GetDataErrCnt() , MDrv_HVD_Sub_GetDecErrCnt(),
                    gHVDPacket.u32ID_L , gHVDPacket.u32Staddr+gHVDPacket.u32Length , gHVDPacket.u32Length, gHVDPacket.u32Staddr,
                    u32ESRptr, gSubHVDCtrl.MemMap.u32BitstreamBufSize  ,u32ESWptr ,
                    MDrv_HVD_Sub_GetBBUVacancy() ,HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_READ_PTR) ,HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_WRITE_PTR) );
            }
        }
        #if !(defined(UDMA_FPGA_ENVI))
        if( ((gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) != HVD_INIT_HW_RM)
        && (gSubHVDCtrl.bAutoRmLastZeroByte == TRUE)
        && ( (!gSubHVDCtrl.bCannotAccessMIU256) ||
             (( gSubHVDCtrl.bCannotAccessMIU256 )  && ( gSubHVDCtrl.MemMap.u32BitstreamBufAddr < gSubHVDCtrl.MemMap.u32MIU1BaseAddr))) )
        {
            MS_U32 ModifyCnt=0;
            while(1)//for( ModifyCnt=0 ; ModifyCnt<3;ModifyCnt++ )
            {
                if( gHVDPacket.u32Length )
                {
                    MS_U8 *pByte=NULL;
                    MS_U32 u32tmp=0;
                    u32tmp = ( gHVDPacket.u32Staddr + gHVDPacket.u32Length-1);
                    if( u32tmp >= gSubHVDCtrl.MemMap.u32BitstreamBufSize )
                    {
                        u32tmp-=gSubHVDCtrl.MemMap.u32BitstreamBufSize;
                    }
                    u32tmp+=gSubHVDCtrl.MemMap.u32BitstreamBufVAddr;
                    pByte = (MS_U8 *)u32tmp;
                    if( *pByte == 0 )
                    {
                        /*
                       if( ModifyCnt == 2 )
                       {
                            gHVDPacket.u32Length+=ModifyCnt;
                       }
                       else
                       */
                       {
                        ModifyCnt++;
                            gHVDPacket.u32Length--;
                       }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if( ModifyCnt != 0)
            {
                //HVD_SUB_MSG_INFO("HVD remove last zero byte:%lu\n" , ModifyCnt);
            }
            if( gHVDPacket.u32Length == 0 )
            {
                HVD_SUB_MSG_ERR(  "HVD Warn: Packet with all zero bytes(staddr:0x%lx remove zero bytes:%lu)\n " ,  gHVDPacket.u32Staddr , ModifyCnt);
                eRet=E_HVD_OK;
                _DRV_HVD_Return(eRet );
            }
        }
        #endif
        /*
        {
            MS_U8 *pByte=NULL;
            pByte = (MS_U8 *)((gSubHVDCtrl.MemMap.u32BitstreamBufVAddr) + gHVDPacket.u32Staddr );
            HVD_SUB_MSG_INFO("HVD remove last zero byte:%02x%02x%02x%02x\n" ,
                *pByte  , *(pByte+1) , *(pByte+2) , *(pByte+3) );
        }
        */
        eRet=( HVD_Result )HAL_HVD_Sub_PushPacket( (HVD_BBU_Info*)&gHVDPacket );

    }
    else
    {
        HVD_SUB_MSG_DEG("Push queue full\n");
        eRet=E_HVD_RET_QUEUE_FULL;
    }
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_PushQueue_Fire()
/// @brief \b Function \b Description:  fire all waiting entry into the decoding table(BBU table).
/// @return -The result of command push queue fire
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_PushQueue_Fire(void)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    HAL_HVD_Sub_UpdateESWptr_Fire();
    eRet= E_HVD_OK;
    _DRV_HVD_Return( eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_DecodeIFrame()
/// @brief \b Function \b Description:  Decode I frame only under driver input path.
/// @param -u32SrcSt \b IN :  The physical address if user has input packet.
/// @param -u32SrcSize \b IN :  The packet size if user has input packet.
/// @return -The result of command decode I frame.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_DecodeIFrame(MS_PHYADDR u32SrcSt , MS_U32 u32SrcSize)
{
    MS_U32 timer = 300;
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();

    eRet= MDrv_HVD_Sub_Pause();
    if( eRet != E_HVD_OK)
    {
        _DRV_HVD_Return(eRet );
    }
    // skip decode I
    eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SKIP_DEC, E_HVD_SKIP_DECODE_I);
    if( eRet != E_HVD_OK )
    {
        _DRV_HVD_Return(eRet );
    }
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_I_DIRECT, TRUE);
    if( eRet != E_HVD_OK )
    {
        _DRV_HVD_Return(eRet );
    }
    eRet = MDrv_HVD_Sub_StepDecode();
    if( eRet != E_HVD_OK)
    {
        _DRV_HVD_Return(eRet );
    }
    if( E_HVD_INIT_INPUT_DRV == (gSubHVDCtrl.InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) )
    {
        HVD_Packet_Info packet;
        if( u32SrcSize ==0 )
        {
            eRet = E_HVD_RET_INVALID_PARAMETER;
            HVD_SUB_MSG_ERR( "HVD err: decode I frame input packet size is zero\n" );
            _DRV_HVD_Return(eRet );
        }
        packet.u32Staddr = u32SrcSt - gSubHVDCtrl.MemMap.u32BitstreamBufAddr ;
        packet.u32Length = u32SrcSize;
        if( ( packet.u32Staddr + packet.u32Length )  >  gSubHVDCtrl.MemMap.u32BitstreamBufSize )
        {
            eRet = E_HVD_RET_INVALID_PARAMETER;
            HVD_SUB_MSG_ERR( "HVD err: decode I frame memory overflow, the packet end address is over ES buffer end address\n" );
            _DRV_HVD_Return(eRet );
        }
        packet.u32TimeStamp = 0xFFFFFFFF;
        packet.u32ID_L=0;
        packet.u32ID_H=0;
        MDrv_HVD_Sub_PushQueue( &packet );
        HAL_HVD_Sub_UpdateESWptr_Fire();
    }
    while( timer )
    {
        HVD_Delay_ms(1);
        if( MDrv_HVD_Sub_IsStepDecodeDone() )
        {
            break;
        }
        timer--;
    }
    if( timer ==0)
    {
        eRet= E_HVD_FAIL;
        HVD_SUB_MSG_ERR( "HVD err: decode I frame time out, not enough data\n" );
        _DRV_HVD_Return( eRet);
    }
    eRet= E_HVD_OK;
    _DRV_HVD_Return( eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDataEnd()
/// @brief \b Function \b Description:  Upper layer set this to inform driver that there are no more data will be pushed.
/// @param -bEnd \b IN :  Enable/ Disable
///                 -FALSE(0): normal status( default )
///                 -TRUE(1): ending status
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDataEnd( MS_BOOL bEnd )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    if( bEnd )
    {
        gSubHVDCtrl.u32CtrlMode|= HVD_CTRL_DATA_END;
    }
    else
    {
        gSubHVDCtrl.u32CtrlMode&=~ HVD_CTRL_DATA_END;
    }
    //_DRV_HVD_PushDummy_Rst();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_PLAYBACK_FINISH, bEnd);
    _DRV_HVD_Return( eRet);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDispErrFrm()
/// @brief \b Function \b Description:  Enable/ Disable to decode and show error(broken) frames
/// @param -bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): hide error frames
///                 -TRUE(1): show error frames
/// @return -The result of command set display error frames
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDispErrFrm(MS_BOOL bEnable)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_ERR_FRM, bEnable);
    gSubHVDCtrl.Settings.bIsShowErrFrm=bEnable;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDispRepeatField()
/// @brief \b Function \b Description:  Enable/ Disable to show last field when FW needs to show repeated field
/// @param -bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): disable this mode
///                 -TRUE(1): enable this mode
/// @return -The result of command set display repeated field
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDispRepeatField(MS_BOOL bEnable)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_REPEAT_LAST_FIELD, bEnable);
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetSkipDecMode()
/// @brief \b Function \b Description:  set the decoding frame type.
/// @param -eDecType \b IN : decoding frame type
/// @return -The result of command set skip decode mode
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetSkipDecMode(HVD_Skip_Decode eDecType)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if( (HVD_Skip_Decode)HAL_HVD_Sub_GetData( E_HVD_GDATA_SKIP_MODE ) != eDecType )
#endif
    {
        eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SKIP_DEC, eDecType);
        if( eRet != E_HVD_OK )
        {
            _DRV_HVD_Return(eRet );
        }
        if( eDecType == E_HVD_SKIP_DECODE_I  )
        {
            eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_I_DIRECT, TRUE);
        }
        else
        {
            eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_I_DIRECT, FALSE);
        }
    }
    gSubHVDCtrl.Settings.u8SkipMode=(MS_U8)eDecType;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDispSpeed()
/// @brief \b Function \b Description:  specify the display speed type.
/// @param -eSpeed \b IN : display speed type
/// @return -The result of command set display speed type
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDispSpeed(HVD_Drv_Disp_Speed eSpeed)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    HVD_SUB_MSG_DEG(" MDrv_HVD_SetDispSpeed:%d\n " , (MS_S16)eSpeed);
#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if( HAL_HVD_Sub_GetData( E_HVD_GDATA_DISPLAY_DURATION ) != eSpeed )
#endif
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_SPEED, eSpeed);
    }
#if HVD_ENABLE_WAIT_CMD_FINISHED
    if( eRet == E_HVD_OK )
    {
        MS_U32 timer = HVD_DRV_CMD_WAIT_FINISH_TIMEOUT;
        while( timer )
        {
            if( HAL_HVD_Sub_GetData( E_HVD_GDATA_DISPLAY_DURATION ) == eSpeed )
            {
                break;
            }
            HVD_Delay_ms(1);
            timer--;
        }
    }
#endif
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetSyncActive()
/// @brief \b Function \b Description:  Enable/disable the sync of video time stamp and STC.
/// @param -bEnable \b IN : Enable/ Disable
///                 -FALSE(0): Disable sync mode
///                 -TRUE(1): Enable sync mode
/// @return -The result of command set sync active
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetSyncActive( MS_BOOL bEnable)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if( HAL_HVD_Sub_GetData( E_HVD_GDATA_IS_SYNC_ON ) != bEnable )
#endif
    {
        eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_ACTIVE, bEnable);
    }
#if HVD_ENABLE_WAIT_CMD_FINISHED
    if( eRet == E_HVD_OK )
    {
        MS_U32 timer = HVD_DRV_CMD_WAIT_FINISH_TIMEOUT;
        while( timer )
        {
            if( HAL_HVD_Sub_GetData( E_HVD_GDATA_IS_SYNC_ON ) == bEnable )
            {
                break;
            }
            HVD_Delay_ms(1);
            timer--;
        }
    }
#endif
    gSubHVDCtrl.Settings.bIsSyncOn=bEnable;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDropMode()
/// @brief \b Function \b Description:  specify the way to drop decoded frames.
/// @param -eMode \b IN : Drop display type.
/// @param -u32Arg \b IN : The argument of eMode
///     -( eMode == E_HVD_DROP_DISPLAY_AUTO)  , Enable: (u32Arg = TRUE); Disable: (u32Arg = FALSE)
///     -( eMode == E_HVD_DROP_DISPLAY_ONCE)  , u32Arg = not zero
/// @return -The result of command set display speed type
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDropMode( HVD_Drop_Disp eMode , MS_U32 u32Arg)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();

    if( eMode  == E_HVD_DROP_DISPLAY_AUTO )
    {
        eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DROP_DISP_AUTO, u32Arg);
    }
    else if( eMode  == E_HVD_DROP_DISPLAY_ONCE )
    {
        eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DROP_DISP_ONCE, u32Arg);
    }
    else
    {
        eRet= E_HVD_RET_INVALID_PARAMETER;
    }
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_RstPTS()
/// @brief \b Function \b Description:  Reset HVD sync table
/// @param -u32PTS \b IN : PTS base
/// @return -The result of command reset PTS
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_RstPTS(MS_U32 u32PTS)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_RESET_PTS, u32PTS);
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetFrcMode()
/// @brief \b Function \b Description:  set the frame rate convert mode.
/// @param -eMode \b IN : mode type
/// @return -The result of command set frame rate convert mode
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetFrcMode(HVD_FrmRateConv_Mode eMode )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_FRC_MODE, eMode);
    gSubHVDCtrl.Settings.u8FrcMode=(MS_U8)eMode;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetSyncTolerance()
/// @brief \b Function \b Description:  Set the tolerance of FW reporting sync reach.
/// @param -u32Arg \b IN : tolerance.
/// @return -The result of command set sync tolerance
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetSyncTolerance(MS_U32 u32Arg )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_TOLERANCE, u32Arg);
    gSubHVDCtrl.Settings.u32SyncTolerance=u32Arg;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetSyncVideoDelay()
/// @brief \b Function \b Description:  Set the video delay from STC when sync mode active.
/// @param -u32Arg \b IN : The video delay. unit:ms
/// @return -The result of command set sync video delay
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetSyncVideoDelay( MS_U32 u32Arg)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) ==
         HVD_INIT_MAIN_LIVE_STREAM   )
    {
        if( gSubHVDCtrl.InitParams.u8TimeUnit )
        {
            eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_VIDEO_DELAY, (MS_U32)(u32Arg+HVD_DTV_VIDEO_DELAY)  );
        }
        else
        {
            eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_VIDEO_DELAY, u32Arg+((MS_U32)HVD_DTV_VIDEO_DELAY*90)  );
        }
    }
    else
    {
        eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_VIDEO_DELAY, u32Arg);
    }
    gSubHVDCtrl.Settings.u32SyncVideoDelay=u32Arg;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetSyncFreeRunTH()
/// @brief \b Function \b Description:  Set the tolerance of FW reporting sync reach.
/// @param -u32Arg \b IN : theashold.
///                     - 0 : use FW default value
///                     - 0xFFFFFFFF : never free run, FW always do sync action.
/// @return -The result of command set sync tolerance
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetSyncFreeRunTH(MS_U32 u32Arg )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_FREERUN_THRESHOLD, u32Arg);
    gSubHVDCtrl.Settings.u32SyncFreeRunTH=u32Arg;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetSyncRepeatTH()
/// @brief \b Function \b Description:  Set the repeat threashold under sync mode.
/// @param -u32Arg \b IN : repeat times. 0x01 ~ 0xFF
///                 0xff - repeat current frame until STC catch up PTS.
/// @return -The result of command set sync repeat threashold
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetSyncRepeatTH( MS_U32 u32Arg)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet= (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SYNC_THRESHOLD, u32Arg);
    gSubHVDCtrl.Settings.u32SyncRepeatTH=u32Arg;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetErrConceal()
/// @brief \b Function \b Description:  Enable/Disable error concealment.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable error concealment.
///                 -TRUE(1): Enable error concealment.
/// @return -The result of command set sync tolerance
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetErrConceal(MS_BOOL bEnable )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_ERR_CONCEAL, bEnable);
    gSubHVDCtrl.Settings.bIsErrConceal=bEnable;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDbgLevel()
/// @brief \b Function \b Description:  Set debug level
/// @param -elevel \b IN : debug level
//-----------------------------------------------------------------------------
void MDrv_HVD_Sub_SetDbgLevel( HVD_Uart_Level elevel )
{
    if( elevel == E_HVD_UART_LEVEL_FW )
    {
        HAL_HVD_Sub_UartSwitch2FW(TRUE);
    }
    #if 0
    else
    {
        HAL_HVD_Sub_UartSwitch2FW(FALSE);
    }
    #endif
    switch( elevel )
    {
    case E_HVD_UART_LEVEL_ERR:
        u32SubUartCtrl =E_HVD_UART_CTRL_ERR;
        break;
    case E_HVD_UART_LEVEL_INFO:
        u32SubUartCtrl =E_HVD_UART_CTRL_INFO|E_HVD_UART_CTRL_ERR;
        break;
    case E_HVD_UART_LEVEL_DBG:
        u32SubUartCtrl =E_HVD_UART_CTRL_DBG|E_HVD_UART_CTRL_ERR|E_HVD_UART_CTRL_INFO;
        break;
    case E_HVD_UART_LEVEL_TRACE:
        u32SubUartCtrl =E_HVD_UART_CTRL_TRACE|E_HVD_UART_CTRL_ERR|E_HVD_UART_CTRL_INFO|E_HVD_UART_CTRL_DBG;
        break;
    case E_HVD_UART_LEVEL_FW:
        //u32SubUartCtrl =E_HVD_UART_CTRL_FW|E_HVD_UART_CTRL_ERR|E_HVD_UART_CTRL_INFO|E_HVD_UART_CTRL_DBG|E_HVD_UART_CTRL_TRACE;
        //break;
    case E_HVD_UART_LEVEL_NONE:
    default:
        u32SubUartCtrl=E_HVD_UART_CTRL_DISABLE;
        break;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SeekToPTS()
/// @brief \b Function \b Description: Let FW decode to the input PTS by using full decoding speed. If FW reaches (equal to or larger than) input PTS, FW will pause automatically. This function can not work under MDrv_HVD_SetSyncActive(TRUE).
/// @param -u32PTS \b IN : specific PTS.
///                 -0: disable this mode. FW will go back to previous status (play or pause).
///                 -any not zero: enable this mode
/// @return -The result of command seek to specific PTS.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SeekToPTS(MS_U32 u32PTS )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet=_DRV_HVD_Sub_Check_Cmd( E_HVD_CHECK_CMD_SEEK2PTS );
    if( eRet != E_HVD_OK )
    {
        _DRV_HVD_Return(eRet );
    }
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_JUMP_TO_PTS, u32PTS);
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SkipToPTS()
/// @brief \b Function \b Description: Let FW skip to the input PTS by using full decoding speed and start decode again after input PTS.
/// @param -u32PTS \b IN : specific PTS.
///                 -0: disable this mode. FW will go back to previous status (play or pause).
///                 -any not zero: enable this mode
/// @return -The result of command seek to specific PTS.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SkipToPTS(MS_U32 u32PTS )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SKIP_TO_PTS, u32PTS);
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetFreezeImg()
/// @brief \b Function \b Description: Let FW stop updating frames when vsync, but decoding process is still going.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable freeze image.
///                 -TRUE(1): Enable freeze image.
/// @return -The result of command freeze image.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetFreezeImg(MS_BOOL bEnable )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_FREEZE_IMG, bEnable);
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetBlueScreen()
/// @brief \b Function \b Description: Let FW stop updating frames when vsync and keep blue screen , but decoding process is still going.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable blue screen.
///                 -TRUE(1): Enable blue screen.
/// @return -The result of command set blue screen.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetBlueScreen(MS_BOOL bEnable )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_BLUE_SCREEN, bEnable);
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDispOneField()
/// @brief \b Function \b Description: Let FW only show one field(top field only).
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable display one field.
///                 -TRUE(1): Enable display one field.
/// @return -The result of command display one field.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDispOneField(MS_BOOL bEnable )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    if( bEnable )
    {   // force to show top field only.
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_ONE_FIELD, E_HVD_FIELD_CTRL_TOP);
    }
    else
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_DISP_ONE_FIELD, E_HVD_FIELD_CTRL_OFF);
    }
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetISREvent()
/// @brief \b Function \b Description: Set the ISR event type sended by HVD fw.
/// @param -u32Event \b IN : event types
/// @param -fnISRHandler \b IN : function pointer to a interrupt handler.
/// @return -The result of command set ISR event.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetISREvent(MS_U32 u32Event, HVD_InterruptCb fnISRHandler)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    if( u32Event == E_HVD_ISR_NONE )
    {
        gSubHVDCtrl.Settings.bEnISR = FALSE;
        HAL_HVD_Sub_Enable_ISR(FALSE);
        OSAL_HVD_ISR_Disable();
        OSAL_HVD_ISR_Detach();
        gHVDISRCtrl.bRegISR = FALSE;
        gHVDISRCtrl.pfnISRCallBack = NULL;
        gSubHVDCtrl.Settings.u32IsrEvent = (MS_U32)E_HVD_ISR_NONE;
        eRet = E_HVD_OK;
    }
    else
    {
        if(fnISRHandler != NULL)
        {
#if 1 //If ISR handler has been registerred, we only need to reset event flag.
            if( gHVDISRCtrl.bRegISR )
            {
                gHVDISRCtrl.pfnISRCallBack = (HVD_ISRCallBack)fnISRHandler;
                eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_ISR_TYPE, u32Event);
                if( eRet != E_HVD_OK)
                {
                    _DRV_HVD_Return(eRet);
                }
                gSubHVDCtrl.Settings.u32IsrEvent = u32Event;
                eRet = E_HVD_OK;
            }
            else
            {
                gHVDISRCtrl.pfnISRCallBack = (HVD_ISRCallBack)fnISRHandler;
                OSAL_HVD_ISR_Attach( (void*)_DRV_HVD_Sub_ISRHandler );
                OSAL_HVD_ISR_Enable();
                HVD_MSG_INFO("attach ISR number:%d\n" , HVD_ISR_VECTOR);
                eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_ISR_TYPE, u32Event);
                if(eRet != E_HVD_OK)
                {
                    _DRV_HVD_Return(eRet );
                }
                gSubHVDCtrl.Settings.u32IsrEvent = u32Event;
                gSubHVDCtrl.Settings.bEnISR = TRUE;
                HAL_HVD_Sub_Enable_ISR(TRUE);
                gHVDISRCtrl.bRegISR=TRUE;
                eRet = E_HVD_OK;
            }
#else
            HAL_HVD_Sub_Enable_ISR(FALSE);
            if( gHVDISRCtrl.bRegISR )
            {
                OSAL_HVD_ISR_Detach();
                gHVDISRCtrl.bRegISR=FALSE;
            }
            gHVDISRCtrl.pfnISRCallBack = (HVD_ISRCallBack)fnISRHandler;
            OSAL_HVD_ISR_Attach( (void*)_DRV_HVD_Sub_ISRHandler );
            OSAL_HVD_ISR_Enable();
/*
            {
                MS_U32 i=0;
                for( i=0;i<68;i++ )
                {
                    MsOS_AttachInterrupt( i , (void*)_DRV_HVD_Sub_ISRHandler );//(InterruptCb)fnISRHandler  );
                    MsOS_EnableInterrupt(i);
                }
            }
*/
            HVD_SUB_MSG_INFO("attach ISR number:%d\n" , HVD_ISR_VECTOR);
            eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_ISR_TYPE, u32Event);
            gSubHVDCtrl.Settings.u32IsrEvent = u32Event;
            if( eRet != E_HVD_OK)
            {
                _DRV_HVD_Return(eRet );
            }
            HAL_HVD_Sub_Enable_ISR(TRUE);
            gHVDISRCtrl.bRegISR=TRUE;
            eRet = E_HVD_OK;
#endif
        }
        else
        {
            HVD_SUB_MSG_ERR( "HVD DrvErr: SetISREvent with NULL pointer. ISR type:%lu\n", u32Event);
            eRet = E_HVD_RET_INVALID_PARAMETER;
        }
    }
    _DRV_HVD_Return(eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetEnableISR()
/// @brief \b Function \b Description: Enable/Disable HVD interrupt.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable interrupt.
///                 -TRUE(1): Enable interrupt.
/// @return -The result of command set enable ISR.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_SetEnableISR(MS_BOOL bEnable)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    HAL_HVD_Sub_Enable_ISR(bEnable);
    gSubHVDCtrl.Settings.bEnISR=bEnable;
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetForceISR()
/// @brief \b Function \b Description: test the HVD interrupt function. Force FW send one interrupt.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Clear force interrupt status from HK.
///                 -TRUE(1): force one interrupt from HK.
/// @return -The result of command set force ISR.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_SetForceISR(MS_BOOL bEnable)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    HAL_HVD_Sub_SetForceISR( bEnable);
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetMVOPDone()
/// @brief \b Function \b Description: tell FW MVOP is ready for futher decode.
/// @return -The result of command.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_SetMVOPDone(void)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    //_DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_BLUE_SCREEN, FALSE);
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetVirtualBox()
/// @brief \b Function \b Description:  Set DS width and Height to F/W
/// @param -u16Width \b IN :  frame width
/// @param -u16Height \b IN :  frame height
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    HAL_HVD_Sub_SetData(E_HVD_SDATA_VIRTUAL_BOX_WIDTH, u16Width);
    HAL_HVD_Sub_SetData(E_HVD_SDATA_VIRTUAL_BOX_HEIGHT, u16Height);
    eRet = E_HVD_OK;
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDynScalingParam()
/// @brief \b Function \b Description: Pass scalar parameters to decoder
/// @return -The result of command.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDynScalingParam(  void *pStAddr , MS_U32 u32Size )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    MS_U32 addr = 0;

    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    if( (pStAddr == NULL ) || (u32Size==0)  )
    {
        eRet = E_HVD_RET_INVALID_PARAMETER;
        _DRV_HVD_Inited(eRet);
    }
    else
    {
        // 1. copy data input data array
        addr = MDrv_HVD_Sub_GetDynamicScalingInfo(E_HVD_DS_INFO_ADDR);
        HVD_memcpy(addr, pStAddr, u32Size);

        // 2. while till FW finish it.
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_SCALER_INFO_NOTIFY, 0);
    }
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetDispInfoTH()
/// @brief \b Function \b Description:  Set the upper and lower limitation of a valid SPS.
/// @param -DispInfoTH \b IN : tolerance.
/// @return -The result of command set display inforation threshold
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetDispInfoTH(HVD_Disp_Info_Threshold* DispInfoTH )
{
    HVD_Disp_Info_TH DispInfoTHTmp;
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;

    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    if( DispInfoTH ==NULL)
    {
        _DRV_HVD_Return(eRet);
    }
    DispInfoTHTmp.u32FrmrateLowBound=DispInfoTH->u32FrmrateLowBound;
    DispInfoTHTmp.u32FrmrateUpBound=DispInfoTH->u32FrmrateUpBound;
    DispInfoTHTmp.u32MvopLowBound=DispInfoTH->u32MvopLowBound;
    DispInfoTHTmp.u32MvopUpBound=DispInfoTH->u32MvopUpBound;
    HAL_HVD_Sub_SetData(  E_HVD_SDATA_DISP_INFO_TH ,  (MS_U32)(&DispInfoTHTmp) );
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_UPDATE_DISP_THRESHOLD, 0);
    HVD_memcpy((void*)&(gSubHVDCtrl.Settings.DispInfoTH), (void*)(DispInfoTH), sizeof(HVD_Disp_Info_Threshold) );
    _DRV_HVD_Return(eRet );
}

// check status

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsISROccured()
/// @brief \b Function \b Description:  Check if the ISR is occured or not.
/// @return - Is occured or not
/// @retval     -FALSE(0): interrupt is not occured.
/// @retval     -TRUE(1): interrupt has been occured.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsISROccured(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    return HAL_HVD_Sub_IsISROccured();
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsDispFinish()
/// @brief \b Function \b Description:  Check this file mode display is finish or not. only work after MDrv_HVD_SetDataEnd(TRUE)
/// @return - Is finish or not
/// @retval     -FALSE(0): Not finish or Not in file mode playback
/// @retval     -TRUE(1): Display Finished.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsDispFinish(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_LIVE_STREAM)
    {
        return FALSE;
    }

    if( gSubHVDCtrl.u32CtrlMode & HVD_CTRL_DATA_END)
    {
        if( _DRV_HVD_Sub_IsAllBufferEmpty() )
        {
            return TRUE;
        }
        else
        {
            //_DRV_HVD_PushDummy();
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsFrameShowed()
/// @brief \b Function \b Description:  Is HVD Frame showed after TriggerDisp(TRUE) or first frame showed
/// @return - Is frame showed or not
/// @retval     -FALSE(0): New Framed showed
/// @retval     -TRUE(1): Not showed
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsFrameShowed(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( !HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_1ST_FRM_RDY)  )
    {// 1st frame not ready or AV sync not ready
        return FALSE;
    }
    else
    {// 1st frame showed or AV sync ready
        if( _DRV_HVD_Ctrl( HVD_CTRL_DISPLAY_CTRL))
        {   // control display
            return (MS_BOOL)HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_FRAME_SHOWED);
        }
        else
        { // cotrol decode
            return TRUE;
        }
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsStepDecodeDone()
/// @brief \b Function \b Description:  Is HVD step decode done after step decode command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send corresponding step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsStepDecodeDone(void)
{
    MS_BOOL bRet=FALSE;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( gSubHVDCtrl.bStepDecoding)
    {
        if( gSubHVDCtrl.u32StepDecodeCnt != HAL_HVD_Sub_GetData(E_HVD_GDATA_DECODE_CNT) )
        {
            gSubHVDCtrl.u32StepDecodeCnt = 0;
            bRet= TRUE;
            gSubHVDCtrl.bStepDecoding=FALSE;
        }
    }
    return bRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_CheckDispInfoRdy()
/// @brief \b Function \b Description:  check display info ready and correct or not
/// @return -The result of command check display infor ready or not
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_CheckDispInfoRdy(void)
{
    MS_U32 u32ErrCode=0;
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);

    if( !( gSubHVDCtrl.u32CtrlMode & HVD_CTRL_DISP_INFO_RDY) )
    {
        if( !HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_DISP_INFO_CHANGE) )
        {
            return E_HVD_RET_NOTREADY;
        }
        else
        {
            gSubHVDCtrl.bIsDispInfoChg=TRUE;
            _DRV_HVD_SetCtrl(HVD_CTRL_DISP_INFO_RDY);
        }
    }
    // check if FW report error
    u32ErrCode = HAL_HVD_Sub_GetData(E_HVD_GDATA_ERROR_CODE);
    if( u32ErrCode != 0 )
    {
        if( u32ErrCode == E_HVD_ERR_OUT_OF_MEMORY)
        {
            HVD_SUB_MSG_ERR("HVD Err: FW error that Out of Memory:%lx. Allocated frame buffer size is smaller than required.\n" , u32ErrCode );
            gSubHVDCtrl.bIsDispInfoChg=FALSE;
            return E_HVD_RET_OUTOF_MEMORY;
        }
    }
    //>> move to FW
    #if 0
    if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_AVC )
    {
        MS_U32 u32MaxFS = 0, u32FS = 0;
        MS_U8 u8LevelIdc = HAL_HVD_GetData(E_HVD_GDATA_AVC_LEVEL_IDC);
        MS_U32 u32Width, u32Height;
        HVD_Display_Info *pDispInfo  = (HVD_Display_Info *)HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_INFO_ADDR);
        u32Height = pDispInfo->u16VerSize;
        u32Width = pDispInfo->u16VerSize;
        u32FS = (u32Width / 16) * (u32Height / 16);

        switch(u8LevelIdc)
        {
            case 10:
                u32MaxFS = 99;
                break;
            case 11:
            case 12:
            case 13:
            case 20:
                u32MaxFS = 396;
                break;
            case 21:
                u32MaxFS = 792;
                break;
            case 22:
            case 30:
                u32MaxFS = 1620;
                break;
            case 31:
                u32MaxFS = 3600;
                break;
            case 32:
                u32MaxFS = 5120;
                break;
            case 40:
            case 41:
                u32MaxFS = 8192;
                break;
            case 42:
                u32MaxFS = 8704;
                break;
            case 50:
                u32MaxFS = 22080;
                break;
            case 51:
                u32MaxFS = 36864;
                break;
            default:
                u32MaxFS = 0xFFFFFFFF;
                break;
        }

        if(u32FS > u32MaxFS)
        {
            HVD_SUB_MSG_DEG("HVD Err : SPS over spec,level = %d u32FS = %lu u32MaxFS = %lu\n", u8LevelIdc ,  u32FS, u32MaxFS);
            gSubHVDCtrl.bIsDispInfoChg=FALSE;
            return E_HVD_RET_UNSUPPORTED;
        }
    }
    #endif
    return E_HVD_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsDispInfoChg()
/// @brief \b Function \b Description:  check display info is changed or not
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): not changed
/// @retval     -TRUE(1): changed
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsDispInfoChg(void)
{
    MS_BOOL bRet=FALSE;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    bRet = (MS_BOOL)HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_DISP_INFO_CHANGE);
    if( bRet )
    {
        gSubHVDCtrl.bIsDispInfoChg = TRUE;
        _DRV_HVD_SetCtrl(HVD_CTRL_DISP_INFO_RDY);
    }
    #if 0 // debug use
    {
        static MS_U32 u32DecodeCnt=0;
        static MS_U32 u32IdleCnt=0;
        MS_U32 tmp =0 ;
        tmp = MDrv_HVD_Sub_GetDecodeCnt();
        if( u32DecodeCnt != tmp )
        {
            printf( "%lu decode cnt:%lx PTS:%lx\n" , (MS_U32)bRet ,tmp ,  MDrv_HVD_GetPTS() );
            u32DecodeCnt =tmp;
        }
        else
        {
            u32IdleCnt++;
        }
        if( u32IdleCnt > 1000)
        {
            printf( "HVD: seems IDLE: %lu decode cnt:%lx PTS:%lx\n" , (MS_U32)bRet  ,tmp ,  MDrv_HVD_Sub_GetPTS() );
            u32IdleCnt=0;
        }
    }
    #endif
    return bRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsIdle()
/// @brief \b Function \b Description:  check decoder is idle or not
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): decoder is not idle
/// @retval     -TRUE(1): decoder is idle
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsIdle(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( HAL_HVD_Sub_GetData(E_HVD_GDATA_VPU_IDLE_CNT) > HVD_FW_IDLE_THRESHOLD )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsSyncStart()
/// @brief \b Function \b Description:  check decoder starts to do sync action(drop or repeat) or not.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): decoder is not doing sync action
/// @retval     -TRUE(1): decoder is doing sync action
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsSyncStart(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( (MS_BOOL)MDrv_HVD_Sub_GetPlayMode(E_HVD_GMODE_IS_SYNC_ON) == FALSE)
    {
        return FALSE;
    }
    if( HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_SYNC_START)  )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsSyncReach()
/// @brief \b Function \b Description: The distance of Video time stamp and STC is closed enough.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): The distance is not shorter than sync tolerance.
/// @retval     -TRUE(1): The distance is shorter than sync tolerance.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsSyncReach(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( (MS_BOOL)MDrv_HVD_Sub_GetPlayMode(E_HVD_GMODE_IS_SYNC_ON) == FALSE)
    {
        return FALSE;
    }

    if( HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_SYNC_REACH)  )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsLowDelay()
/// @brief \b Function \b Description:  check if current stream has low delay flag in SPS.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Low delay flag not found.
/// @retval     -TRUE(1): Low delay flag found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsLowDelay(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( HAL_HVD_Sub_GetData(E_HVD_GDATA_AVC_LOW_DELAY)  )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsIFrmFound()
/// @brief \b Function \b Description:  check if I frame found after Init() or flush().
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): I frame is not found.
/// @retval     -TRUE(1): I frame has been found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsIFrmFound(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_I_FRM_FOUND)  )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_Is1stFrmRdy()
/// @brief \b Function \b Description:  check if first frame showed on screen after Init() or flush().
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): First frame is not showed.
/// @retval     -TRUE(1): First frame is showed.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_Is1stFrmRdy(void)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_1ST_FRM_RDY)  )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsAllBufferEmpty()
/// @brief \b Function \b Description:  check if all of the buffers(display, decoded, BBU, bitstream) are empty.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Not Empty.
/// @retval     -TRUE(1): Empty.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_IsAllBufferEmpty(void)
{
    //HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if( _DRV_HVD_Sub_IsAllBufferEmpty() )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    #if 0
    if( u32Times > 200 )
    {
        HVD_SUB_MSG_INFO("HVD Info: Flush() timeout failed:BBU:%lu Dec:%lu Disp:%lu Idle:%lu ESr:%lx ESw:%lx  \n" ,
            HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_Q_NUMB) ,
            HAL_HVD_Sub_GetData(E_HVD_GDATA_DEC_Q_NUMB),
            HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_Q_NUMB),
            HAL_HVD_Sub_GetData(E_HVD_GDATA_VPU_IDLE_CNT),
            HAL_HVD_Sub_GetData( E_HVD_GDATA_ES_READ_PTR),
            HAL_HVD_Sub_GetData( E_HVD_GDATA_ES_WRITE_PTR) );
        eRet = E_HVD_FAIL;
        break;
    }
    return TRUE;
    #endif
}

// get infomation

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetBBUVacancy()
/// @brief \b Function \b Description:  get the vacancy of BBU queue.
/// @return - TRUE / FALSE
/// @retval     -0: queue is full.
/// @retval     -not zero: queue is not full.
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetBBUVacancy(void)
{
    MS_U32 u32BBUQnumb=0;
    MS_U32 u32BBUTotal=0;
    MS_U32 u32PTSQnumb=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    u32BBUTotal = HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_BBU_TOTAL_TBL_ENTRY);
    u32BBUQnumb = HAL_HVD_Sub_GetData(E_HVD_GDATA_BBU_Q_NUMB);
    if( u32BBUTotal <= u32BBUQnumb )
    {
        //HVD_SUB_MSG_ERR("HVD err: GetBBUVacancy has error that total number(%lx) is smaller than current number(%lx)\n" , u32BBUTotal, u32BBUQnumb);
        u32BBUQnumb=0;
    }
    else
    {
        u32BBUQnumb = u32BBUTotal - u32BBUQnumb;
    }
    if( u32BBUQnumb )
    {
        u32BBUTotal=HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_PTS_TOTAL_ENTRY_NUMB) - 1; // add -1 for full case
        u32PTSQnumb=HAL_HVD_Sub_GetData(E_HVD_GDATA_PTS_Q_NUMB);
        if( u32BBUTotal <= u32PTSQnumb  )
        {
            HVD_SUB_MSG_DEG("HVD Warn: GetBBUVacancy has error that total PTS Q number(%lx) is smaller than current PTS Q number(%lx)\n" , u32BBUTotal, u32PTSQnumb);
            u32BBUQnumb=0;
        }
    }
    return u32BBUQnumb;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetDispInfo()
/// @brief \b Function \b Description:  Get video display information
/// @param -pinfo \b OUT : pointer to video display information.
/// @return -The result of command get display information
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_GetDispInfo( HVD_Disp_Info *pinfo )
{
    MS_U32 u32Seqtimes=10;
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);

    if( !(gSubHVDCtrl.u32CtrlMode & HVD_CTRL_DISP_INFO_RDY))
    {
        if( MDrv_HVD_Sub_CheckDispInfoRdy() == E_HVD_RET_NOTREADY )
        {
            return E_HVD_RET_NOTREADY;
        }
    }
    if( pinfo == NULL )
    {
        return E_HVD_RET_INVALID_PARAMETER;
    }

    _DRV_HVD_Entry();

    HVD_GETDISPINFO_START:
    HAL_HVD_Sub_SetData(E_HVD_SDATA_GET_DISP_INFO_START, 0);
    HVD_memcpy((void*)pinfo, (void*)HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_INFO_ADDR)  ,  sizeof(HVD_Display_Info));
    if( HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_DISP_INFO_UNCOPYED) )
    {
        u32Seqtimes--;
        if( u32Seqtimes > 0)
        {
            goto HVD_GETDISPINFO_START;
        }
        else
        {
            HVD_SUB_MSG_ERR("HVD Err: GetDispInfo Timeout:%d\n" , (MS_S16)HAL_HVD_Sub_GetData(E_HVD_GDATA_IS_DISP_INFO_UNCOPYED) );
            _DRV_HVD_Return( E_HVD_RET_TIMEOUT);
        }
    }

#if HVD_ENABLE_RV_FEATURE
    if(HVD_INIT_HW_RM == (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
    {//RM is always displayed by 60 frames per sec.
        pinfo->u32FrameRate = 60000;
    }
#endif

    if( gSubHVDCtrl.bIsDispInfoChg )
    {
        HVD_SUB_MSG_DEG("pinfo->u32FrameRate = %lu\n", pinfo->u32FrameRate);
        HVD_SUB_MSG_DEG("pinfo->u8Interlace = %x \n", (MS_S16)pinfo->u8Interlace);
        HVD_SUB_MSG_DEG("pinfo->u16HorSize = %u \n", pinfo->u16HorSize);
        HVD_SUB_MSG_DEG("pinfo->u16VerSize = %u \n", pinfo->u16VerSize);
        HVD_SUB_MSG_DEG("pinfo->u16Crop R/L = %u, %u \n", pinfo->u16CropRight, pinfo->u16CropLeft);
        HVD_SUB_MSG_DEG("pinfo->u16Crop B/T = %u, %u \n", pinfo->u16CropBottom, pinfo->u16CropTop);
        HVD_SUB_MSG_DEG("pinfo->u8AspectRate = %u \n", (MS_S16)pinfo->u8AspectRate);
        HVD_SUB_MSG_DEG("pinfo->u16SarWidth = %u \n", pinfo->u16SarWidth);
        HVD_SUB_MSG_DEG("pinfo->u16SarHeight = %u \n", pinfo->u16SarHeight);
        HVD_SUB_MSG_DEG("pinfo->u16Pitch = %u \n", pinfo->u16Pitch);
        HVD_SUB_MSG_DEG("pinfo->u8ColourPrimaries = %u \n", pinfo->u8ColourPrimaries);
        gSubHVDCtrl.bIsDispInfoChg=FALSE;
    }

    _DRV_HVD_Return( E_HVD_OK);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetPTS()
/// @brief \b Function \b Description:  get the pts of current displayed video frame. unit: ms
/// @return - PTS
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetPTS(void)
{
    MS_U32 u32PTS = 0;
    _DRV_HVD_Inited(FALSE);
    u32PTS = HAL_HVD_Sub_GetData(E_HVD_GDATA_PTS);
    return u32PTS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetNextPTS()
/// @brief \b Function \b Description:  get the pts of next displayed video frame. unit: ms
/// @return - PTS
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetNextPTS(void)
{
    MS_U32 u32PTS = 0;
    _DRV_HVD_Inited(FALSE);
    u32PTS = HAL_HVD_Sub_GetData(E_HVD_GDATA_NEXT_PTS);
    return u32PTS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetDataErrCnt()
/// @brief \b Function \b Description:  get accumulated data Error Count
/// @return -data error count
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetDataErrCnt(void)
{
    _DRV_HVD_Inited(FALSE);
    return HAL_HVD_Sub_GetData(E_HVD_GDATA_DATA_ERROR_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetDecErrCnt()
/// @brief \b Function \b Description:  get accumulated decode Error Count
/// @return -decode error count
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetDecErrCnt(void)
{
    _DRV_HVD_Inited(FALSE);
    return HAL_HVD_Sub_GetData(E_HVD_GDATA_DEC_ERROR_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetESWritePtr()
/// @brief \b Function \b Description:  Get Elementary Stream buffer write point
/// @return - ES buffer write point offset from bitstream buffer base
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetESWritePtr(void)
{
    _DRV_HVD_Inited(FALSE);
    return HAL_HVD_Sub_GetData(E_HVD_GDATA_ES_WRITE_PTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_GetESReadPtr()
/// @brief \b Function \b Description:  Get Elementary Stream buffer read point
/// @return - ES buffer read point offset from bitstream buffer base
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetESReadPtr(void)
{
    _DRV_HVD_Inited(FALSE);
    return HAL_HVD_Sub_GetData(E_HVD_GDATA_ES_READ_PTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetErrCode()
/// @brief \b Function \b Description:  get error code
/// @return - error code number
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetErrCode(void)
{
// TODO: define driver error code  for upper layer
    MS_U32 u32Ret=0;
    _DRV_HVD_Inited(FALSE);
    u32Ret = HAL_HVD_Sub_GetData(E_HVD_GDATA_ERROR_CODE);
    HAL_HVD_Sub_SetData(E_HVD_SDATA_ERROR_CODE , 0);
    gSubHVDCtrl.u32LastErrCode = u32Ret;
    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetPlayMode()
/// @brief \b Function \b Description:  Get current play mode status.
/// @param -eMode \b IN : Mode type.
/// @return - mode status
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetPlayMode(HVD_Get_Mode_Status eMode)
{
    MS_U32 u32Ret=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    switch( eMode )
    {
    case E_HVD_GMODE_IS_SHOW_ERR_FRM:
    case E_HVD_GMODE_IS_REPEAT_LAST_FIELD:
    case E_HVD_GMODE_IS_ERR_CONCEAL:
    case E_HVD_GMODE_IS_SYNC_ON:
    case E_HVD_GMODE_IS_PLAYBACK_FINISH:
    case E_HVD_GMODE_SYNC_MODE:
    case E_HVD_GMODE_SKIP_MODE:
    case E_HVD_GMODE_DROP_MODE:
    case E_HVD_GMODE_DISPLAY_SPEED:
    case E_HVD_GMODE_FRC_MODE:
        // TODO: add isr type here
    case E_HVD_GMODE_ISR_TYPE:
        u32Ret=HAL_HVD_Sub_GetData( (HVD_GetData)((MS_U32)eMode+ (MS_U32)E_HVD_GDATA_IS_SHOW_ERR_FRM));
        break;
    case E_HVD_GMODE_IS_STEP_DISPLAY:
        u32Ret = _DRV_HVD_Ctrl(HVD_CTRL_DISPLAY_CTRL);
        break;
    case E_HVD_GMODE_STREAM_TYPE:
        u32Ret = gSubHVDCtrl.InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK;
        break;
    default:
        break;
    }
    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetPlayState()
/// @brief \b Function \b Description:  get current play state
/// @return - play state
//-----------------------------------------------------------------------------
HVD_Get_Play_State MDrv_HVD_Sub_GetPlayState(void)
{
    MS_U32 u32FWstate=0;
    HVD_Get_Play_State eRet=E_HVD_GSTATE_INIT;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    u32FWstate=HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_STATE);
    u32FWstate &= E_HVD_FW_STATE_MASK;
    switch( u32FWstate )
    {
    case E_HVD_FW_INIT:
        eRet = E_HVD_GSTATE_INIT;
        break;
    case E_HVD_FW_PLAY:
        eRet = E_HVD_GSTATE_PLAY;
        break;
    case E_HVD_FW_PAUSE:
        eRet = E_HVD_GSTATE_PAUSE;
        break;
    case E_HVD_FW_STOP:
        eRet = E_HVD_GSTATE_STOP;
        break;
    }
    return eRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetDecodeCnt()
/// @brief \b Function \b Description:  get accumulated decoded frame Count
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetDecodeCnt(void)
{
    //HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    return HAL_HVD_Sub_GetData(E_HVD_GDATA_DECODE_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetActiveFormat()
/// @brief \b Function \b Description:  Get current AFD ID
/// @return - AFD ID, 0xFF:invalid value
//-----------------------------------------------------------------------------
MS_U8 MDrv_HVD_Sub_GetActiveFormat(void)
{
    HVD_Display_Info *pDispInfo = NULL;

    HVD_SUB_MSG_TRACE();

    _DRV_HVD_Inited(~0);

    if (!(gSubHVDCtrl.u32CtrlMode & HVD_CTRL_DISP_INFO_RDY))
    {
        return 0;
    }

    pDispInfo = (HVD_Display_Info *) HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_INFO_ADDR);

    if (pDispInfo != NULL)
    {
        return pDispInfo->u8AFD;
    }
    else
    {
        return ~0;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetInfo()
/// @brief \b Function \b Description:  Get information of HVD driver.
/// @return - driver information
//-----------------------------------------------------------------------------
const HVD_DrvInfo* MDrv_HVD_Sub_GetInfo( void )
{
    DrvInfo.bAVC=MDrv_HVD_Sub_GetCaps( E_HVD_AVC);
    DrvInfo.bAVS=MDrv_HVD_Sub_GetCaps( E_HVD_AVS);
    DrvInfo.bRM=MDrv_HVD_Sub_GetCaps( E_HVD_RM);
    DrvInfo.FWversion=HVD_FW_VERSION;
    return (&DrvInfo);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetStatus()
/// @brief \b Function \b Description:  Get status of HVD driver
/// @param -pstatus \b OUT : driver status
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Low delay flag not found.
/// @retval     -TRUE(1): Low delay flag found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_GetStatus( HVD_DrvStatus *pstatus)
{
    if( pstatus ==  NULL )
    {
        return FALSE;
    }
    pstatus->bInit = gSubHVDCtrl.u32CtrlMode&HVD_CTRL_INIT_FINISHED;
    pstatus->bBusy = gSubHVDCtrl.u32CtrlMode&HVD_CTRL_PROCESSING;
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetFrmInfo()
/// @brief \b Function \b Description:  Get current displayed or decoded frame information of HVD driver
/// @param -eType \b IN : Type of frame information
/// @param -pInfo \b OUT : frame information
/// @return -The result of command get frame information
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_GetFrmInfo( HVD_Get_Frm_Info_Type eType  ,  HVD_Frame_Info *pInfo)
{
    HVD_Frm_Information *pFrmInfo =NULL;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(E_HVD_FAIL);
    if( pInfo ==  NULL )
    {
        return E_HVD_FAIL;
    }
    if( eType == E_HVD_GFRMINFO_DISPLAY )
    {
        pFrmInfo =(HVD_Frm_Information *)HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_FRM_INFO);
    }
    else if(  eType == E_HVD_GFRMINFO_DECODE )
    {
        pFrmInfo =(HVD_Frm_Information *)HAL_HVD_Sub_GetData(E_HVD_GDATA_DEC_FRM_INFO);
    }

    if(  pFrmInfo != NULL  )
    {
        pInfo->u32LumaAddr = (MS_PHYADDR)(pFrmInfo->u32LumaAddr);
        pInfo->u32ChromaAddr = (MS_PHYADDR)(pFrmInfo->u32ChromaAddr);
        if( gSubHVDCtrl.u32CtrlMode & HVD_CTRL_FRM_MIU_1 )
        {
            pInfo->u32LumaAddr+=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
            pInfo->u32ChromaAddr+=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
        }
        pInfo->u32TimeStamp= pFrmInfo->u32TimeStamp;
        pInfo->u32ID_L= pFrmInfo->u32ID_L;
        pInfo->u32ID_H= pFrmInfo->u32ID_H;
        pInfo->u16Pitch = pFrmInfo->u16Pitch;
        pInfo->u16Height = pFrmInfo->u16Height;
        pInfo->u16Width = pFrmInfo->u16Width;
        pInfo->eFrmType= (HVD_Frm_Type)(pFrmInfo->u8FrmType);
        pInfo->eFieldType=(HVD_FieldType)(pFrmInfo->u8FieldType);
        if( (pInfo->u16Pitch ==0) && (pInfo->u16Width == 0) && (pInfo->u16Height ==0) )
        {
            return E_HVD_FAIL;
        }
    }
    else
    {
        return E_HVD_FAIL;
    }
    return E_HVD_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetISRInfo()
/// @brief \b Function \b Description:  Get information of HVD driver interrupt
/// @param -eType \b OUT : ISR information
/// @return -the result of get ISR information
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_GetISRInfo(MS_U32* eType)
{
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    if(TRUE == gHVDISRCtrl.bInISR)
    {
        gHVDISRCtrl.u32ISRInfo |= HAL_HVD_Sub_GetData( E_HVD_GDATA_HVD_ISR_STATUS );
        *eType = gHVDISRCtrl.u32ISRInfo;
    }
    else
    {
        *eType = HAL_HVD_Sub_GetData( E_HVD_GDATA_HVD_ISR_STATUS );
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_CalLumaSum()
/// @brief \b Function \b Description:  Get the sum of luma data in a frame.
/// @param -eType \b IN : Type of frame information
/// @return -the sum
/// @retval     -0xFFFFFFFF: error occer.
/// @retval     -not zero: the sum.
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_CalLumaSum(  HVD_Get_Frm_Info_Type eType  )
{
    HVD_Frm_Information *pFrmInfo =NULL;
    MS_U32 u32Ret=HVD_U32_MAX;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(u32Ret);
    if( eType == E_HVD_GFRMINFO_DISPLAY )
    {
        pFrmInfo =(HVD_Frm_Information *)HAL_HVD_Sub_GetData(E_HVD_GDATA_DISP_FRM_INFO);
    }
    else if(  eType == E_HVD_GFRMINFO_DECODE )
    {
        pFrmInfo =(HVD_Frm_Information *)HAL_HVD_Sub_GetData(E_HVD_GDATA_DEC_FRM_INFO);
    }

    if(  pFrmInfo != NULL  )
    {
        MS_U32 u32tmp=0;
        MS_U32 u32PitchCnt=0;
        MS_U32 u32HeightCnt=0;
        volatile MS_U8* pLumaData=NULL;
        // PA2VA
        u32tmp = gSubHVDCtrl.MemMap.u32FrameBufAddr;
        if( gSubHVDCtrl.u32CtrlMode & HVD_CTRL_FRM_MIU_1 )
        {
            u32tmp -=gSubHVDCtrl.MemMap.u32MIU1BaseAddr;
        }
        pLumaData = (volatile MS_U8*)(gSubHVDCtrl.MemMap.u32FrameBufVAddr+(pFrmInfo->u32LumaAddr -u32tmp)  );
        for( u32HeightCnt=0 ; u32HeightCnt < pFrmInfo->u16Height ; u32HeightCnt++  )
        {
            for( u32PitchCnt =0   ; u32PitchCnt < pFrmInfo->u16Pitch  ;u32PitchCnt++ )
            {
                if( u32PitchCnt < pFrmInfo->u16Pitch )
                {
                    u32Ret+=(MS_U32)(*pLumaData);
                }
                pLumaData++;
            }
        }
    }
    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetUserData_Wptr()
/// @brief \b Function \b Description:  Get write pointer of user data.
/// @return -the information of write pointer of user data.
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetUserData_Wptr( void )
{
    MS_U32 u32Ret=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    u32Ret = (MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_WPTR);
    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetUserData_Packet()
/// @brief \b Function \b Description:  Get information of user data packet.
/// @param -u32Idx \b IN : the pointer of required user data packet( valid range is 0 ~ 11 )
/// @param -u32Size \b OUT : the size of required user data packet
/// @return -the offset of user data packet form code buffer start address
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetUserData_Packet( MS_U32 u32Idx , MS_U32* u32Size )
{
    MS_U32 u32Ret=0;
    MS_U32 tmp=0;
    MS_U8* pIdx=NULL;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    *u32Size=0;
    tmp = (MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_IDX_TBL_SIZE);
    if(  u32Idx >= tmp )
    {
        HVD_SUB_MSG_ERR("HVD ERR: input user data index(%lu) is larger than max index(%lu)\n"  , u32Idx   , tmp  );
        return 0;
    }
    tmp = (MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_IDX_TBL_ADDR);
    if(  tmp == 0 )
    {
        HVD_SUB_MSG_INFO("HVD FW ERR: user data packet idx tbl base addr(%lx) is zero\n"   , tmp  );
        return 0;
    }
    pIdx = (MS_U8*)(tmp + u32Idx);
    tmp = (MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_PACKET_TBL_SIZE);
    if( (*pIdx) >= tmp )
    {
        HVD_SUB_MSG_INFO("HVD FW ERR: user data packet tbl ptr(%lu) is larger than max size(%lu)\n"  , (MS_U32)(*pIdx)   , tmp  );
        return 0;
    }
    tmp = (MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_PACKET_TBL_ADDR);
    if(  tmp == 0 )
    {
        HVD_SUB_MSG_INFO("HVD FW ERR: user data packet packet tbl base offset(%lx) is zero\n"   , tmp  );
        return 0;
    }
    u32Ret= tmp ;//+ gSubHVDCtrl.MemMap.u32CodeBufAddr;
    tmp = (MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_PACKET_SIZE);
    if(  tmp == 0 )
    {
        HVD_SUB_MSG_INFO("HVD FW ERR: user data packet packet size(%lu) is zero\n"   , tmp  );
        return 0;
    }
    *u32Size=tmp;
    u32Ret +=  (MS_U32)(*pIdx) * tmp;
    return u32Ret;
}

// VDEC Interal control
//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GenPattern()
/// @brief \b Function \b Description:  Generate spcific pattern to support some special function.
/// @param -eType \b IN : the virtual address of spcific pattern
/// @param -u32VAddr \b IN : the virtual address of spcific pattern
/// @param -u32Size \b IN, OUT :
///                             IN: the input array size.
///                             OUT: the used array size.
/// @return -The result of command generate spcific pattern
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_GenPattern( HVD_PatternType eType ,  MS_U32 u32VAddr  ,  MS_U32* u32Size  )
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    MS_U8* pDummyData = NULL;
    MS_U32 u32MinPatternSize=0;

    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();

#if defined(CHIP_T2)

    {// dummy pattern
        // Driver input need not to push dummy pattern
        if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
        {
            *u32Size = 0;
            eRet = E_HVD_OK;
            _DRV_HVD_Return(eRet );
        }

        // TSP input process
        if( u32VAddr == 0)
        {
            *u32Size = 6;
            HVD_SUB_MSG_ERR("HVD ERR: Dummy Pattern address shall not be zero\n" );
            eRet = E_HVD_RET_INVALID_PARAMETER;
            _DRV_HVD_Return(eRet );
        }
        switch( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) )
        {
        case HVD_INIT_HW_AVC:
            u32MinPatternSize =6;
            break;
        case HVD_INIT_HW_AVS:
        case HVD_INIT_HW_RM:
        default:
            u32MinPatternSize = 0;
            break;
        }
        if( *u32Size < u32MinPatternSize )
        {
            HVD_SUB_MSG_ERR("HVD ERR: Dummy Pattern must have at least %lu bytes, input:%lu\n" , u32MinPatternSize , (MS_U32)(*u32Size)  );
            *u32Size = u32MinPatternSize;
            eRet = E_HVD_RET_OUTOF_MEMORY;
            _DRV_HVD_Return(eRet );
        }

        pDummyData = (MS_U8*)(u32VAddr);
        HVD_memset( (void*)pDummyData, 0xff , *u32Size);

        switch( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) )
        {
        case HVD_INIT_HW_AVC:
            pDummyData[0] = 0;
            pDummyData[1] = 0;
            pDummyData[2] = 1;
            pDummyData[3] = 0xFF;
            pDummyData[*u32Size-1] = 0x80;
            break;
        case HVD_INIT_HW_AVS:
        case HVD_INIT_HW_RM:
        default:
            *u32Size = u32MinPatternSize;
            break;
        }
    }

#else // HVD chip set
    if( eType == E_HVD_PATTERN_FLUSH )
    {// flush pattern
        // Driver input need not to push flush pattern
        if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
        {
            *u32Size = 0;
            eRet = E_HVD_OK;
            _DRV_HVD_Return(eRet );
        }

        // TSP input process
        if( u32VAddr == 0)
        {
            *u32Size = 8+144;
            HVD_SUB_MSG_ERR("HVD ERR: Flush Pattern address shall not be zero\n" );
            eRet = E_HVD_RET_INVALID_PARAMETER;
            _DRV_HVD_Return(eRet );
        }
        switch( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) )
        {
        case HVD_INIT_HW_AVC:
            u32MinPatternSize =8+144;
            break;
        case HVD_INIT_HW_AVS:
            u32MinPatternSize=8+144;
            break;
        case HVD_INIT_HW_RM:
        default:
            u32MinPatternSize = 0;
            break;
        }
        if( *u32Size < u32MinPatternSize )
        {
            HVD_SUB_MSG_ERR("HVD ERR: Flush Pattern must have at least %lu bytes, input:%lu\n" , u32MinPatternSize , (MS_U32)(*u32Size)  );
            *u32Size = u32MinPatternSize;
            eRet = E_HVD_RET_OUTOF_MEMORY;
            _DRV_HVD_Return(eRet );
        }
        *u32Size = u32MinPatternSize;

        pDummyData = (MS_U8*)(u32VAddr);
        HVD_memset( (void*)pDummyData, 0x0 , *u32Size);

        switch( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) )
        {
        case HVD_INIT_HW_AVC:
            pDummyData[0] = 0;
            pDummyData[1] = 0;
            pDummyData[2] = 1;
            pDummyData[3] = 0xFF;
            pDummyData[4] = 0xAA;
            pDummyData[5] = 0x55;
            pDummyData[6] = 0xAA;
            pDummyData[7] = 0x55;
            break;
        case HVD_INIT_HW_AVS:
            pDummyData[0] = 0;
            pDummyData[1] = 0;
            pDummyData[2] = 1;
            pDummyData[3] = 0xB4;
            pDummyData[4] = 0xAA;
            pDummyData[5] = 0x55;
            pDummyData[6] = 0xAA;
            pDummyData[7] = 0x55;
            break;
        case HVD_INIT_HW_RM:
        default:
            break;
        }

    }
    else if( eType == E_HVD_PATTERN_FILEEND )
    {// dummy pattern
        // Driver input need not to push dummy pattern
        if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
        {
            *u32Size = 0;
            eRet = E_HVD_OK;
            _DRV_HVD_Return(eRet );
        }

        // TSP input process
        if( u32VAddr == 0)
        {
            *u32Size = 8+144;
            HVD_SUB_MSG_ERR("HVD ERR: Dummy Pattern address shall not be zero\n" );
            eRet = E_HVD_RET_INVALID_PARAMETER;
            _DRV_HVD_Return(eRet );
        }
        switch( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) )
        {
        case HVD_INIT_HW_AVC:
            u32MinPatternSize =8+144;
            break;
        case HVD_INIT_HW_AVS:
            u32MinPatternSize=8+144;
            break;
        case HVD_INIT_HW_RM:
        default:
            u32MinPatternSize = 0;
            break;
        }
        if( *u32Size < u32MinPatternSize )
        {
            HVD_SUB_MSG_ERR("HVD ERR: Dummy Pattern must have at least %lu bytes, input:%lu\n" , u32MinPatternSize , (MS_U32)(*u32Size)  );
            *u32Size = u32MinPatternSize;
            eRet = E_HVD_RET_OUTOF_MEMORY;
            _DRV_HVD_Return(eRet );
        }

        pDummyData = (MS_U8*)(u32VAddr);

        HVD_memset( (void*)pDummyData, 0x0 , *u32Size);

        switch( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_HW_MASK) )
        {
        case HVD_INIT_HW_AVC:
            pDummyData[0] = 0;
            pDummyData[1] = 0;
            pDummyData[2] = 1;
            pDummyData[3] = 0xFF;
            pDummyData[4] = 0xFF;
            pDummyData[5] = 0xFF;
            pDummyData[6] = 0xFF;
            pDummyData[7] = 0xFF;
            break;
        case HVD_INIT_HW_AVS:
            pDummyData[0] = 0;
            pDummyData[1] = 0;
            pDummyData[2] = 1;
            pDummyData[3] = 0xB4;
            pDummyData[4] = 0xAA;
            pDummyData[5] = 0x66;
            pDummyData[6] = 0xAA;
            pDummyData[7] = 0x66;
            break;
        case HVD_INIT_HW_RM:
        default:
            *u32Size = u32MinPatternSize;
            break;
        }
    }
#endif
    eRet = E_HVD_OK;
    _DRV_HVD_Return( eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetPatternInfo()
/// @brief \b Function \b Description:  Get driver specific data information
/// @param -eType \b IN : the type of specific data information
/// @return -the information of choosed type
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetPatternInfo(   HVD_Pattern_Info eType )
{
    MS_U32 eRet=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    switch( eType )
    {
    case E_HVD_FLUSH_PATTERN_SIZE:
        if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
        {
            eRet =0;
        }
        else if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_TS)
        {
#if defined(CHIP_T2)
            eRet =(MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_MAX_DUMMY_FIFO);
#else
            eRet =8+144;
#endif
        }
        break;

    case E_HVD_DUMMY_HW_FIFO:
        if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW )
        {
            eRet =0;
        }
        else if( (gSubHVDCtrl.InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_TS)
        {
            eRet =(MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_MAX_DUMMY_FIFO);
        }
        break;
    }
    return eRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetDynamicScalingInfo()
/// @brief \b Function \b Description:  Get information of Dynamic Scaling
/// @param -eType \b IN : the type of specific information
/// @return -the information of choosed type
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetDynamicScalingInfo(   HVD_DynamicScaling_Info eType )
{
    MS_U32 u32Ret=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    switch( eType )
    {
    case E_HVD_DS_BUF_MIUSEL:
        if( gSubHVDCtrl.MemMap.u32CodeBufAddr >= gSubHVDCtrl.MemMap.u32MIU1BaseAddr )
        {
            u32Ret=TRUE;
        }
        else
        {
            u32Ret=FALSE;
        }
        break;
    case E_HVD_DS_BUF_ADDR:
        u32Ret=(MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_DS_BUF_ADDR);
        u32Ret+=gSubHVDCtrl.MemMap.u32CodeBufAddr;
        break;
    case E_HVD_DS_BUF_SIZE:
        u32Ret=(MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_DS_BUF_SIZE);
        break;
    case E_HVD_DS_VECTOR_DEPTH:
        u32Ret=(MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_DS_VECTOR_DEPTH);
        break;
    case E_HVD_DS_INFO_ADDR:
        u32Ret=(MS_U32)HAL_HVD_Sub_GetData(E_HVD_GDATA_FW_DS_INFO_ADDR);
        u32Ret+=gSubHVDCtrl.MemMap.u32CodeBufAddr;
        break;
    }
    return u32Ret;
}

#if 0
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_DynamicScaling_Info
/// @brief \b Enum \b Description:  The information type of dynamic scaling.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_DS_CAPABILITY,         ///< the capability of dynamic sacling. Ret: TRUE /FALSE
    E_HVD_DS_BUF_ADDR,         ///< the buffer start physcial address.
    E_HVD_DS_BUF_SIZE,            ///< the buffer size.
    E_HVD_DS_VECTOR_DEPTH,            ///< the depth of scaling vector.
} HVD_DynamicScaling_Info;


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_GetDynamicScalingInfo()
/// @brief \b Function \b Description:  Get driver information of dynamic scaling
/// @param -eType \b IN : the type of specific dynamic scaling information
/// @return -the information of choosed type
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_GetDynamicScalingInfo(   HVD_DynamicScaling_Info eType )
{
    MS_U32 u32Ret=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);

    switch( eType )
    {
    case E_HVD_DS_CAPABILITY:
        HAL_HVD_Sub_GetData(  E_HVD_GDATA_DS_CAPABILITY );
        break;
    case E_HVD_DS_BUF_ADDR:
        HAL_HVD_Sub_GetData(  E_HVD_GDATA_DS_BUF_ADDR );
        break;
    case E_HVD_DS_BUF_SIZE:
        HAL_HVD_Sub_GetData(  E_HVD_GDATA_DS_BUF_SIZE );
        break;
    case E_HVD_DS_VECTOR_DEPTH:
        HAL_HVD_Sub_GetData(  E_HVD_GDATA_DS_VECTOR_DEPTH );
        break;
    default:
        break;
    }
    return u32Ret;
}
#endif
// debug

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetData()
/// @brief \b Function \b Description:  Get target data from HVD driver
/// @param -eType \b IN : the type of the target data
/// @return -the value of the target data
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetData(HVD_GData_Type eType)
{
    MS_U32 u32Ret=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(FALSE);
    switch( eType )
    {
    case E_HVD_GDATA_TYPE_DISP_CNT:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_DISP_CNT );
        break;
    case E_HVD_GDATA_TYPE_SKIP_CNT:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_SKIP_CNT );
        break;
    case E_HVD_GDATA_TYPE_DROP_CNT:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_DROP_CNT );
        break;
    case E_HVD_GDATA_TYPE_IDLE_CNT:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_VPU_IDLE_CNT );
        break;
    case E_HVD_GDATA_TYPE_VSYNC_CNT:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_VSYNC_CNT );
        break;
    case E_HVD_GDATA_TYPE_MAIN_LOOP_CNT:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_MAIN_LOOP_CNT );
        break;
    case E_HVD_GDATA_TYPE_AVC_LEVEL_IDC:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_AVC_LEVEL_IDC );
        break;
    case E_HVD_GDATA_TYPE_DISP_Q_SIZE:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_DISP_Q_NUMB );
        break;
    case E_HVD_GDATA_TYPE_ES_LEVEL:
        u32Ret = (MS_U32)(HVD_ES_Level)HAL_HVD_Sub_GetData(  E_HVD_GDATA_ES_LEVEL );
        break;
    case E_HVD_GDATA_TYPE_AVC_VUI_DISP_INFO:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_AVC_VUI_DISP_INFO );
        break;
    case E_HVD_GDATA_TYPE_DISP_STC:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_DISP_STC );
        break;
    case E_HVD_GDATA_TYPE_USERDATA_IDX_TBL_SIZE:
        u32Ret = HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_IDX_TBL_SIZE);
        break;
    case E_HVD_GDATA_TYPE_USERDATA_PACKET_SIZE:
        u32Ret = HAL_HVD_Sub_GetData(E_HVD_GDATA_USERDATA_PACKET_SIZE);
        break;

    default:
        break;

    }
    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetMem_Dbg()
/// @brief \b Function \b Description:  Get any data from any memory address
/// @param -u32Addr \b IN : the memory address of the target data
/// @return -the value of the memory
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_Sub_GetMem_Dbg(MS_U32 u32Addr)
{
    MS_U32 u32Ret=0;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(u32Ret);
    switch(u32Addr)
    {
    case 1:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_BBU_READ_PTR );
        break;
    case 2:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_VPU_PC_CNT );
        break;
    case 3:
        u32Ret = HAL_HVD_Sub_GetData(  E_HVD_GDATA_BBU_WRITE_PTR );
        break;
    }
    return u32Ret;
//        break;
//    return HAL_HVD_GetData_Dbg(u32Addr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_DbgDumpStatus()
/// @brief \b Function \b Description:  Dump specific information to standard output.
/// @param -eFlag \b IN : the type of information.
//-----------------------------------------------------------------------------
void MDrv_HVD_Sub_DbgDumpStatus(HVD_DumpStatus eFlag)
{
    _DRV_HVD_Initedn();
    if( eFlag & E_HVD_DUMP_STATUS_DRV )
    {
        ;
    }
    if( eFlag & E_HVD_DUMP_STATUS_FW )
    {
        HAL_HVD_Sub_Dump_FW_Status();
    }
    if( eFlag & E_HVD_DUMP_STATUS_HW )
    {
        HAL_HVD_Sub_Dump_HW_Status(HVD_U32_MAX);
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetMem_Dbg()
/// @brief \b Function \b Description:  set any data into any memory address
/// @param -u32Addr \b IN : the memory address of the target destination
/// @param -u32Arg \b IN : the value of input content
//-----------------------------------------------------------------------------
void MDrv_HVD_Sub_SetMem_Dbg(MS_U32 u32Addr , MS_U32 u32Arg)
{
    HVD_SUB_MSG_TRACE();
    HAL_HVD_Sub_SetData_Dbg(u32Addr ,u32Arg );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetCmd_Dbg()
/// @brief \b Function \b Description:  set any FW debug command
/// @param -u32Cmd \b IN : specify the FW command ID.
/// @param -u32Arg \b IN : specify the argument of FW command.
/// @return -the result of debug command
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetCmd_Dbg( MS_U32 u32Cmd , MS_U32 u32Arg)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    eRet = (HVD_Result)HAL_HVD_Sub_SetCmd((HVD_User_Cmd)u32Cmd , u32Arg);
    _DRV_HVD_Return(eRet );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetSettings_Pro()
/// @brief \b Function \b Description:  set any FW debug command
/// @param -eType \b IN : specify the type of setting.
/// @param -u32Arg \b IN : specify the argument of the setting.
/// @return -the result of set professional settings.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetSettings_Pro( HVD_SSettings_Type eType , MS_U32 u32Arg)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_User_Cmd eCmd = E_HVD_CMD_INVALID_CMD;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    _DRV_HVD_Entry();
    switch( eType )
    {
        case E_HVD_SSET_TIME_UNIT:
            eCmd = E_HVD_CMD_TIME_UNIT_TYPE;
            break;
        case E_HVD_SSET_PITCH:
            eCmd = E_HVD_CMD_PITCH;
            break;
        case E_HVD_SSET_SYNC_EACH_FRM:
            eCmd = E_HVD_CMD_SYNC_EACH_FRM;
            break;
        case E_HVD_SSET_MAX_DEC_TICK:
            eCmd = E_HVD_CMD_MAX_DEC_TICK;
            break;
        case E_HVD_SSET_AUTO_FREE_ES:
            eCmd = E_HVD_CMD_AUTO_FREE_ES;
            gSubHVDCtrl.Settings.bAutoFreeES=u32Arg;
            break;
        case E_HVD_SSET_MIN_FRAME_GAP:
            eCmd = E_HVD_CMD_MIN_FRAME_GAP;
            break;
        case E_HVD_SSET_DISABLE_DEBLOCKING:
            eCmd = E_HVD_CMD_DIS_DBF;
            if( u32Arg > 2 )
            {
                u32Arg =1;
            }
            gSubHVDCtrl.Settings.bDisDeblocking=u32Arg;
            break;
        case E_HVD_SSET_DISABLE_QUARTER_PIXEL:
            eCmd = E_HVD_CMD_DIS_QUART_PIXEL;
            if( u32Arg > 2 )
            {
                u32Arg =1;
            }
            gSubHVDCtrl.Settings.bDisQuarterPixel=u32Arg;
            break;
        case E_HVD_SSET_MIU_BURST_CNT_LEVEL:
#if !(defined(CHIP_T2) || defined(CHIP_T7))
            gSubHVDCtrl.Settings.u32MiuBurstLevel=u32Arg;
            eCmd = E_HVD_CMD_MIU_BURST_CNT;
#else
            eCmd = E_HVD_CMD_INVALID_CMD;
#endif
            break;
        default:
            break;
    }
    if( eCmd != E_HVD_CMD_INVALID_CMD)
    {
        eRet = (HVD_Result)HAL_HVD_Sub_SetCmd(eCmd , u32Arg);
    }
    else
    {
        eRet = E_HVD_RET_INVALID_PARAMETER;
    }
    _DRV_HVD_Return(eRet );
}

#if 1

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_GetCaps()
/// @brief \b Function \b Description:  check if HW support this format
/// @param -u32Type \b IN : specify the format type
/// @return - TRUE/ FALSE
/// @retval     -FALSE(0): not supported by HW
/// @retval     -TRUE(1): supported by HW
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_Sub_GetCaps( HVD_Codec u32Type)
{
    MS_U32 verID=HAL_HVD_Sub_Get_HWVersionID();
    verID = verID;
    u32Type = u32Type;
    //AVCH264DBG(printf("HVD rev id: 0x%x \n\n", verID );
    #if ( HVD_HW_VERSION == HVD_HW_HVD)
    verID=verID >> 12;
    switch( u32Type )
    {
    case E_HVD_AVC:
    case E_HVD_AVS:
    case E_HVD_RM:
        if( (verID & BIT(u32Type)) == 0 )
        {
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
    #else
    if( u32Type != E_HVD_AVC )
    {
        return FALSE;
    }
    #endif
    return TRUE;
}
#endif

#if 0 //need check for hvd sub
MS_BOOL MDrv_HVD_LinkWeakSymbolPatch(void)
{
    return TRUE;
}
#endif

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetAutoRmLstZeroByte
/// @brief \b Function \b Description: Turn on/off auto remove last zero byte
/// @param -bOn \b IN : Turn on/off auto remove last zero byte
/// @return -the result of turn on/off auto remove last zero byte
///\b NOTE: The default mode after initialization is On.
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetAutoRmLstZeroByte(MS_BOOL bOn)
{
    HVD_Result eRet = E_HVD_RET_ILLEGAL_ACCESS;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRet);
    gSubHVDCtrl.bAutoRmLastZeroByte = bOn;
    return E_HVD_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_IsAlive
/// @brief \b Function \b Description:Check Whether HVD is alive or not(check cnt)
/// @return -the result of HVD alive status(E_HVD_OK/E_HVD_RET_NOT_RUNNING)
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_IsAlive(void)
{
    _DRV_HVD_Inited(E_HVD_FAIL);
#if defined(CHIP_T2)
    return E_HVD_OK;
#else
    if((gSubHVDCtrl.LivingStatus.u32DecCnt == HAL_HVD_Sub_GetData(E_HVD_GDATA_DECODE_CNT))
    && (gSubHVDCtrl.LivingStatus.u32SkipCnt == HAL_HVD_Sub_GetData(E_HVD_GDATA_SKIP_CNT))
    && (gSubHVDCtrl.LivingStatus.u32IdleCnt == HAL_HVD_Sub_GetData(E_HVD_GDATA_VPU_IDLE_CNT))
    && (gSubHVDCtrl.LivingStatus.u32MainLoopCnt == HAL_HVD_Sub_GetData(E_HVD_GDATA_MAIN_LOOP_CNT)))
    {
        return E_HVD_RET_NOT_RUNNING;
    }
    else
    {
        gSubHVDCtrl.LivingStatus.u32DecCnt = HAL_HVD_Sub_GetData(E_HVD_GDATA_DECODE_CNT);
        gSubHVDCtrl.LivingStatus.u32SkipCnt = HAL_HVD_Sub_GetData(E_HVD_GDATA_SKIP_CNT);
        gSubHVDCtrl.LivingStatus.u32IdleCnt = HAL_HVD_Sub_GetData(E_HVD_GDATA_VPU_IDLE_CNT);
        gSubHVDCtrl.LivingStatus.u32MainLoopCnt = HAL_HVD_Sub_GetData(E_HVD_GDATA_MAIN_LOOP_CNT);
    }
    return E_HVD_OK;
#endif
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_Sub_SetBalanceBW
/// @brief \b Function \b Description: bandwidth adjustment
/// @param -qp_cnt \b IN : QP threshold for overtime counter
/// @param -db_cnt \b IN : Deblocking threshod for overtime counter
/// @param -upper \b IN : upper bound for overtime counter
/// @return -the result of command E_HVD_CMD_BALANCE_BW
//-----------------------------------------------------------------------------
HVD_Result MDrv_HVD_Sub_SetBalanceBW(MS_U8 qp_cnt, MS_U8 db_cnt, MS_U8 upper)
{
    HVD_Result eRst=E_HVD_FAIL;
    HVD_SUB_MSG_TRACE();
    _DRV_HVD_Inited(eRst);
    _DRV_HVD_Entry();
    eRst = (HVD_Result)HAL_HVD_Sub_SetCmd(E_HVD_CMD_BALANCE_BW, qp_cnt | (db_cnt << 8) | (upper << 16));
    _DRV_HVD_Return( eRst);
}
#endif //defined(SUPPORT_HVD_SUB)

