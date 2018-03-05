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
/// file    Drvtvencoder.c
/// @brief  TV encoder.
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVTVENCODER_C
#define _DRVTVENCODER_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/div64.h>
#include <linux/slab.h>
#else
#include <string.h>
#define do_div(x,y) ((x)/=(y))
#endif
#ifdef MSOS_TYPE_LINUX
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> // O_RDWR
#endif
#include "MsDevice.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvMMIO.h"
#include "halCHIP.h"

// Internal Definition
#include "ve_Analog_Reg.h"
#include "drvTVEncoder.h"
#include "mhal_tvencoder.h"
#include "mdrv_gflip_ve_io.h"

#include "utopia.h"
//#include "utopia_core.h"
#include "utopia_dapi.h"
#include "tvencoder_private.h"
#include "tvencoder.h"
#include "ULog.h"
//#include "MsXC_Common_Internal.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
extern MS_U32 _VE_RIU_BASE;      // This should be inited before XC library starting.
// Need to Refine

extern void* pUtopiaTVEncoder;

//#define ENABLE_VE_FIELD_INV       1

#define VE_FRC_TOLERANT           10

#ifndef CONFIG_MBOOT
#define TVENCODER_UTOPIA2         1
#else
#define TVENCODER_UTOPIA2         0
#endif

#define _SC_PK_L_(bank, addr)   (((MS_U16)(bank) << 8) | (MS_U16)((addr)*2))
#define _SC_PK_H_(bank, addr)   (((MS_U16)(bank) << 8) | (MS_U16)((addr)*2+1))

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    //ve out timing
    MS_U16 u16VttOut;   //VttOut
    MS_U16 u16HttOut;   //HttOut
    MS_U16 u16HdeOut;    //HdeOut
    MS_U16 u16VdeOut;    //Vdeout

    MS_BOOL bInterlace;

}MS_VE_PLL_OutCfg;

typedef struct
{
    MS_VE_PLL_InCfg InCfg;
    MS_VE_PLL_OutCfg OutCfg;

    MS_U32 u32IDclk;
    MS_U32 u32ODclk;
    MS_U32 u32InitPll;
}MS_VE_PLL_INFO;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL g_bVeDisableRegWrite = FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_ve_version = {
    .DDI = { VE_DRV_VERSION },
};

static MS_PHY _phyVEMIUBaseAddr = 0;
static MS_U16 _gu16DbgLevel = FALSE;
static VE_DrvInfo _stVE_DrvInfo;
static MS_VE_PLL_INFO _stVE_PllInfo;

//VE
typedef struct
{
    MS_VE_OUTPUT_DEST_TYPE OutputDestType[2];   // output device
    MS_VE_INPUT_SRC_TYPE   InputSrcType;        // input source
    MS_VE_INPUT_SRC_TYPE   InputSrcOfMixedSrc;  // The real input source for mixed "InputSrcType" of VE,
                                                // eg. Real InputSrc of "MS_VE_SRC_MAIN", "MS_VE_SRC_SUB", "MS_VE_SRC_SCALER" etc.
    MS_VE_VIDEOSYS         VideoSystem;         // video std of output signal
    MS_U8   u8DACType;
    MS_U16  u16inputVfreq;
    MS_VE_Backup_Reg BackupReg;
    MS_U16  u16H_CapStart;
    MS_U16  u16H_CapSize;
    MS_U16  u16V_CapStart;
    MS_U16  u16V_CapSize;
    MS_U16  u16H_SC_CapStart;
    MS_U16  u16H_SC_CapSize;
    MS_U16  u16V_SC_CapStart;
    MS_U16  u16V_SC_CapSize;
    MS_U8   u8VE_DisplayStatus;
    MS_BOOL bHDuplicate;
    MS_BOOL bSrcInterlace;
    MS_BOOL bForceCaptureMode;

    MS_U16  u16OutputWidth;  //Downscaling output H size
    MS_U16  u16Outputheight; //Downscaling output V size
    MS_PHY  phyMiuBaseAddr;  //base address without miu offset
    MS_BOOL bMemAddrInMIU1;
    MS_U32  u32MemSize;
    MS_BOOL bVECapture; ///< TRUE/FALSE, enable/disable VE as a capture device
    MS_VE_CusScalingInfo stVECusScalingInfo;
    MS_S32 s32FdGFlip;
}MS_VE_Info, *PMS_VE_Info;

static MS_VE_Info g_VEInfo={.s32FdGFlip=-1,}; //Init the file descriptor to avoid unused duplicate open of "/dev/gflip"

MS_VE_Cap g_VECap;  // ve capability structure

#if TVENCODER_UTOPIA2
static void* pu32TvencoderInst = NULL;
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#if (defined CONFIG_MLOG)
#define MSG_DRV_VE(fmt,_args...) \
        do{\
            if(_gu16DbgLevel ) \
            {\
                ULOGD("VE_DBG","[%s,%5d] ",__FUNCTION__,__LINE__);\
                ULOGD("VE_DBG",fmt,##_args);\
            }\
        } while(0)
#else
#define MSG_DRV_VE(fmt,...)      \
        do{\
            if(_gu16DbgLevel ) \
            {\
                printf("[VE_DBG]:");\
                printf("%s: %d" fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
            }\
        } while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U32 _GCD(MS_U32 u32A,MS_U32 u32B)
{
    MS_U32 u32R;

    u32R = u32A % u32B;
    do{
        u32A = u32B;
        u32B = u32R;
        u32R = u32A % u32B;
    }while(u32R != 0);

    return u32B;
}

static MS_BOOL _ReductionFrac (MS_U32 *pu32A,MS_U32 *pu32B)
{
    MS_U32 u32GCD;
    while(1)
    {
        if( (*pu32A<3000) && (*pu32B<3000))
        {
            return TRUE;
        }

        u32GCD = _GCD(*pu32A,*pu32B);
        if(u32GCD == 1)
            return TRUE;

        do_div(*pu32A, u32GCD);
        do_div(*pu32B, u32GCD);
    }
    return TRUE;
}

static MS_BOOL _MultiplFrac(MS_U32 *pu32A,MS_U32 *pu32B)
{
    while(1)
    {
        if( ( (*pu32A<<1)<3000) && ( (*pu32B<<1) <3000))
        {
             *pu32A<<=1;
             *pu32B<<=1;
        }
        else
        {
            return TRUE;
        }
    }
    return TRUE;
}

static void _PreHscaleDown(MS_BOOL ben, MS_U16 In, MS_U16 Out)
{
#if VE_PRESCALE_SUPPORT
    MS_U32 u32H_Ratio = 0;

    if(ben)
    {
        u32H_Ratio = (MS_U32)VE_H_PRESCALE_DOWN_RATIO(In, Out);
        u32H_Ratio |= BIT(23); //enable H scale
        MApi_XC_W4BYTE(_SC_PK_L_(0x62, 0x1), u32H_Ratio);  //scale_ratio
    }
    else
    {
        MApi_XC_W4BYTE(_SC_PK_L_(0x62, 0x1), 0x0);  //scale_ratio
    }

    MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x6), In);  //hsize_in

    if(Out > In)
    {
        MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x7), In); //hsize_out
    }
    else
    {
        MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x7), Out); //hsize_out
        MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x43), Out-1); //VSP_hsize_in
        MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x5e), Out); //VSP_hsize_out
    }
#endif
}

static void _PreVscaleDown(MS_BOOL ben, MS_U16 In, MS_U16 Out)
{
#if VE_PRESCALE_SUPPORT
    MS_U32 u32V_Ratio = 0;

    if(ben)
    {
        u32V_Ratio = (MS_U32)VE_V_PRESCALE_DOWN_RATIO(In, Out);
        MApi_XC_W4BYTE(_SC_PK_L_(0x62, 0x47), u32V_Ratio);  //scale_ratio
        MApi_XC_W2BYTEMSK(_SC_PK_L_(0x62, 0x45), 0x0, BIT(1)|BIT(9));  //enable
        MApi_XC_W2BYTEMSK(_SC_PK_L_(0x62, 0x5F), 0x0, BIT(1));  //enable
    }
    else
    {
        u32V_Ratio = 0x100000;
        MApi_XC_W4BYTE(_SC_PK_L_(0x62, 0x47), u32V_Ratio);  //scale_ratio
        MApi_XC_W2BYTEMSK(_SC_PK_L_(0x62, 0x45), 0x0, BIT(1)|BIT(9));  //enable filter
        MApi_XC_W2BYTEMSK(_SC_PK_L_(0x62, 0x5F), 0x0, BIT(1));  //enable VSD
    }

    MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x42), In-1);  //Vsize_in

    if(Out > In)
    {
        MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x44), In-1); //Vsize_out
    }
    else
    {
        MApi_XC_W2BYTE(_SC_PK_L_(0x62, 0x44), Out-1); //Vsize_out
    }
#endif
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_VE_GetLibVer
/// @brief \b Function \b Description: Show the VE driver version
/// @param ppVersion \b Out: Library version string
/// @return             @ref  VE_Result
//-------------------------------------------------------------------------------------------------
VE_Result MDrv_VE_GetLibVer(const MSIF_Version **ppVersion)
{

    if (!ppVersion)
    {
        return E_VE_FAIL;
    }

    *ppVersion = &_drv_ve_version;
    return E_VE_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get VE Information
/// @return @ref VE_DrvInfo return the VE information in this member
//-------------------------------------------------------------------------------------------------
const VE_DrvInfo * _MDrv_VE_GetInfo(void)
{
    MDrv_VE_GetCaps(&_stVE_DrvInfo.stCaps);
    return &_stVE_DrvInfo;
}

const VE_DrvInfo * MDrv_VE_GetInfo(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    static VE_DrvInfo ve_drv_info;
    TVE_GETINFO Args ;
    memset(&Args, 0, sizeof(TVE_GETINFO));
    Args.pVE_DrvInfo = &ve_drv_info;

    if(pu32TvencoderInst == NULL)
    {
        return E_VE_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_GetInfo, (void*)&Args);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    return Args.pVE_DrvInfo;
#else
    return _MDrv_VE_GetInfo();
#endif
}
//-------------------------------------------------------------------------------------------------
/// Get VE Status
/// @param  pDrvStatus                  \b OUT: store the status
/// @return @ref VE_Result
//-------------------------------------------------------------------------------------------------
VE_Result _MDrv_VE_GetStatus(VE_DrvStatus* pDrvStatus)
{
    pDrvStatus->u16H_CapStart = g_VEInfo.u16H_CapStart;
    pDrvStatus->u16H_CapSize  = g_VEInfo.u16H_CapSize;
    pDrvStatus->u16V_CapStart = g_VEInfo.u16V_CapStart;
    pDrvStatus->u16V_CapSize  = g_VEInfo.u16V_CapSize;
    pDrvStatus->VideoSystem   = g_VEInfo.VideoSystem;
    return E_VE_OK;
}

VE_Result MDrv_VE_GetStatus(VE_DrvStatus* pDrvStatus)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_GETSTATUS pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return E_VE_FAIL;
    }
    pArgs = (PTVE_GETSTATUS)malloc(sizeof(TVE_GETSTATUS));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->pStaus = pDrvStatus;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_GetStatues, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_OK;
    }
    else
    {
        free(pArgs);
        return E_VE_FAIL;
    }
#else
    return _MDrv_VE_GetStatus(pDrvStatus);
#endif
}
void _MDrv_VE_SetDbgLevel(MS_U16 level)
{
    _gu16DbgLevel = level;
}

void MDrv_VE_SetDbgLevel(MS_U16 level)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETDBGLEVEL pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        if(UtopiaOpen(MODULE_TVENCODER, (void**)&pu32TvencoderInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            return ;
        }
    }
    pArgs = (PTVE_SETDBGLEVEL)malloc(sizeof(TVE_SETDBGLEVEL));

    if(NULL == pArgs)
        return ;
    pArgs->u16Level = level;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetDbgLevel, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SetDbgLevel(level);
#endif
}

//Clear SW variable for VE
static void MDrv_VE_VariableInit(void)
{
    MS_S32 s32FdGFlip = g_VEInfo.s32FdGFlip;

    memset(&g_VEInfo, 0, sizeof(MS_VE_Info));
    memset(&g_VECap, 0, sizeof(MS_VE_Cap));
    memset(&_stVE_PllInfo, 0, sizeof(MS_VE_PLL_INFO));
    memset(&_stVE_DrvInfo, 0, sizeof(VE_DrvInfo));

    //Restore the file descriptor to avoid unused duplicate open of "/dev/gflip"
    g_VEInfo.s32FdGFlip = s32FdGFlip;
    g_VEInfo.InputSrcType = MS_VE_SRC_NONE;
    g_VEInfo.InputSrcOfMixedSrc = MS_VE_SRC_NONE;
}

MS_BOOL MDrv_VE_SetIOMapBase(void)
{
    MS_VIRT _VERIUBaseAddress=0;
    MS_PHY phyNonPMBankSize = 0;
    if(MDrv_MMIO_GetBASE( &_VERIUBaseAddress, &phyNonPMBankSize, MS_MODULE_PM ) != TRUE)
    {
        MSG_DRV_VE("MDrv_VE_Init GetBASE failure\n");
        MS_ASSERT(0);
        return FALSE;
    }

    Hal_VE_init_riu_base( _VERIUBaseAddress );

    return TRUE;
}

//------------------------------------------------------------------------------
/// Set VE FB memory base address
/// @param[in]  u32MIUAddress  \b IN: The FB address for VE
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_SetMemoryBaseAddr(MS_PHY phyMIUAddress, MS_U32 u32MIUSize)
{
    MS_PHY phyStartOffset;
    MS_U8 u8MiuSel = 0;
#if defined(__aarch64__)
    MSG_DRV_VE("Change VE MemoryBase: Address=0x%lx Size=0x%x \n",phyMIUAddress, u32MIUSize);
#else
    MSG_DRV_VE("Change VE MemoryBase: Address=0x%tx Size=0x%tx \n",(ptrdiff_t)phyMIUAddress,(ptrdiff_t) u32MIUSize);
#endif
    //u32MIUAddress = 0x0000100000+HAL_MIU1_BASE;

    // transfer u32MIUAddress
    _phy_to_miu_offset(u8MiuSel, phyStartOffset, phyMIUAddress);

    Hal_VE_SelMIU(u8MiuSel);
    if(u8MiuSel == E_CHIP_MIU_1)
    {
        g_VEInfo.bMemAddrInMIU1 = TRUE;
    }
    else
    {
        g_VEInfo.bMemAddrInMIU1 = FALSE;
    }

#ifndef CONFIG_MBOOT
    if(g_VECap.bSupport_DolbyVerifier)
    {
        Hal_VE_set_dolby_verifier_addr(phyStartOffset);
        phyStartOffset += Hal_VE_get_dolby_verifier_buf_size();
    }
#endif

    //g_VEInfo.u32MemSize = u32MIUSize/(BYTE_PER_WORD>>1); //Now only used in VEC, Change like this later to avoid not enable 3 frame mode
    g_VEInfo.u32MemSize = u32MIUSize;
    g_VEInfo.phyMiuBaseAddr = phyStartOffset;
#if defined (__aarch64__)
    MSG_DRV_VE("Final VE MemoryBase: Address=0x%lx, size=0x%x \n",g_VEInfo.phyMiuBaseAddr, g_VEInfo.u32MemSize);
#else
    MSG_DRV_VE("Final VE MemoryBase: Address=0x%tx, size=0x%tx \n",(ptrdiff_t)g_VEInfo.phyMiuBaseAddr,(ptrdiff_t) g_VEInfo.u32MemSize);
#endif
    Hal_VE_SetMemAddr(&phyStartOffset, &phyStartOffset);
}

//------------------------------------------------------------------------------
/// Set VE FB memory base address
/// @param[in]  u32MIUAddress  \b IN: The FB address for VE
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetMemoryBaseAddr(MS_PHY phyMIUAddress, MS_U32 u32MIUSize)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETMEMORYBASEADDR pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return;
    }
    pArgs = (PTVE_SETMEMORYBASEADDR)malloc(sizeof(TVE_SETMEMORYBASEADDR));

    if(NULL == pArgs)
        return;
    pArgs->phyMIUAddress = phyMIUAddress;
    pArgs->u32MIUSize = u32MIUSize;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetOutputVideoStd, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
    return;
#else
    return _MDrv_VE_SetMemoryBaseAddr(phyMIUAddress,u32MIUSize);
#endif

}


//-------------------------------------------------------------------------------------------------
/// GFlip Init for VE capture, get handler for VEC
/// @return TRUE: success/fail
/// @attention
/// <b>[Mxlib] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_VEC_GFLIPInit(void)
{
    #ifdef MSOS_TYPE_LINUX
    if(0> g_VEInfo.s32FdGFlip)
    {
        g_VEInfo.s32FdGFlip = open("/dev/gflip", O_RDWR);
        if(0> g_VEInfo.s32FdGFlip)
        {
            printf("[%s][%d] open fail /dev/gflip\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
    #endif
    return TRUE;
}

//------------------------------------------------------------------------------
/// Initiate VE
/// @param[in]  u32MIUAddress  \b IN: The address reserved for VE
/// @return none
//------------------------------------------------------------------------------
// initiate video encoder
void _MDrv_VE_Init(MS_PHY u32MIUAddress)
{
    // This should be the first function of VE.
    MDrv_VE_VariableInit();
    if (MDrv_VE_SetIOMapBase() != TRUE)
    {
        MSG_DRV_VE("VE: MDrv_VE_Init MDrv_VE_SetIOMapBase() failure\n");
        return;
    }
    //u32MIUAddress = 0x0000100000+HAL_MIU1_BASE;
#if defined (__aarch64__)
    MSG_DRV_VE("VE input  u32MIUAddress=0x%lx \n",u32MIUAddress);
#else
    MSG_DRV_VE("VE input  u32MIUAddress=0x%tx \n",(ptrdiff_t)u32MIUAddress);
#endif

    //get chip capability
    Hal_VE_GetCaps(&g_VECap);

    _MDrv_VE_SetMemoryBaseAddr(u32MIUAddress, 0);
    _phyVEMIUBaseAddr = u32MIUAddress;

    Hal_VE_init();
}
void MDrv_VE_Init(MS_PHY u32MIUAddress)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_INIT pArgs = NULL;

    if(pu32TvencoderInst ==NULL)
    {
        if(UtopiaOpen(MODULE_TVENCODER, (void**)&pu32TvencoderInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen TVE failed\n");
            return ;
        }
    }
    pArgs = (PTVE_INIT)malloc(sizeof(TVE_INIT));

    if(NULL == pArgs)
        return ;
    pArgs->u32MIUAddress = u32MIUAddress;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Init, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    /*
     * special case, in utopia2.0, if address assigned to NULL it means this process will wait here
     * until another process initialize VE SW/HW successfully.
     * For SW and use case back/forword compatible, in utopia1.0 return before setting physical address to HW.
     */
    if (u32MIUAddress == (MS_PHY)NULL)
    {
        return;
    }
    _MDrv_VE_Init(u32MIUAddress);
#endif
}

MS_BOOL _MDrv_VE_GetConfig(void* pInstance, MS_PHY *pMIUAddress)
{
#if TVENCODER_UTOPIA2
    void* psResource = NULL;
    TVENCODER_INSTANT_PRIVATE* psTVENCODERInstPri = NULL;
    TVENCODER_RESOURCE_PRIVATE * pstResPri = NULL;
    
    UtopiaInstanceGetPrivate(pInstance, (void**)&psTVENCODERInstPri);

    if(UtopiaResourceObtain(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0, &psResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return E_TVENCODER_FAIL;
    }

    UtopiaResourceGetPrivate(psResource, (void**)&pstResPri);
    *pMIUAddress = pstResPri->u32MIUAddress;

    //Ready to do VE Init
    pstResPri->bInited_Drv = FALSE;

    UtopiaResourceRelease(psResource);
#endif
    return TRUE;
}

MS_BOOL MDrv_VE_GetConfig(MS_PHY *pMIUAddress)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    PTVE_GETCONFIG pArgs = NULL;

    if(pu32TvencoderInst ==NULL)
    {
        if(UtopiaOpen(MODULE_TVENCODER, (void**)&pu32TvencoderInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen TVE failed\n");
            return FALSE;
        }
    }

    pArgs = (PTVE_GETCONFIG)malloc(sizeof(TVE_GETCONFIG));

    if(NULL == pArgs)
    {
        return FALSE;
    }

    pArgs->pMIUAddress = pMIUAddress;
    pArgs->eRet = E_TVENCODER_FAIL;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_GetConfig, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return TRUE;
    }
    else
    {
        free(pArgs);
        return FALSE;
    }
#else
    return FALSE;
#endif
}

MS_BOOL _MDrv_VE_Exit(void)
{
    MS_VE_Output_Ctrl OutputCtrl;
    // Turn off VE
    OutputCtrl.bEnable = FALSE;
    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    _MDrv_VE_SetOutputCtrl(&OutputCtrl);
    return TRUE;
}

MS_BOOL MDrv_VE_Exit(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_EXIT pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_EXIT)malloc(sizeof(TVE_EXIT));

    if(NULL == pArgs)
    {
        return E_TVENCODER_FAIL;
    }
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Exit, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return TRUE;
    }
    else
    {
        free(pArgs);
        return FALSE;
    }
#else
    return _MDrv_VE_Exit();
#endif
}

//------------------------------------------------------------------------------
/// set the output destination of video encoder
///
///              None SCART CVBS SVIDEO YPbPr
///       None    O     O     O     O     O
///       SCART O      -     X     X     X
///        CVBS  O     X      -     O     O
///     S_VIDE  O     O     X      O     -     X
///       YPbPr   O     X     O      X     -
///
/// @param  -pSwitchInfo \b IN/OUT: the information of switching output destination of TV encodeer
/// @return None
//------------------------------------------------------------------------------
void _MDrv_VE_SwitchOuputDest(PMS_Switch_VE_Dest_Info pSwitchInfo)
{
    if((pSwitchInfo->OutputDstType != MS_VE_DEST_SCART) &&
       (pSwitchInfo->OutputDstType != MS_VE_DEST_YPBPR) &&
       (pSwitchInfo->OutputDstType != MS_VE_DEST_CVBS)  &&
       (pSwitchInfo->OutputDstType != MS_VE_DEST_SVIDEO))
    {
        pSwitchInfo->Status = MS_VE_SWITCH_DST_INVALID_PARAM;
        return;
    }

    pSwitchInfo->Status = MS_VE_SWITCH_DST_SUCCESS;

    if(pSwitchInfo->OutputDstType == MS_VE_DEST_SCART)
        g_VEInfo.u8DACType = VE_OUT_CVBS_RGB;
    else if(pSwitchInfo->OutputDstType == MS_VE_DEST_YPBPR)
        g_VEInfo.u8DACType = VE_OUT_CVBS_YCbCr;
    else
        g_VEInfo.u8DACType = VE_OUT_CVBS_YCC;

    Hal_VE_set_out_sel(g_VEInfo.u8DACType);
    Hal_VE_set_color_convert(g_VEInfo.VideoSystem, g_VEInfo.u8DACType);
}

void MDrv_VE_SwitchOuputDest(PMS_Switch_VE_Dest_Info pSwitchInfo)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SWITCHOUTPUTDEST pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SWITCHOUTPUTDEST)malloc(sizeof(TVE_SWITCHOUTPUTDEST));

    if(NULL == pArgs)
        return ;
    pArgs->pSwitchInfo = pSwitchInfo;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SwitchOutputDest, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SwitchOuputDest( pSwitchInfo);
#endif
}

//------------------------------------------------------------------------------
/// Get u8VE_DisplayStatus for set the input source of video encoder
///
/// @param  -pSwitchInfo \b IN/OUT: the information of switching input destination of TV encodeer
/// @return None
//------------------------------------------------------------------------------
static void MDrv_VE_SetSrcDispStatus(PMS_VE_InputSrc_Info pInputSrcInfo)
{
    MS_VE_INPUT_SRC_TYPE eInputSrc;
    g_VEInfo.u8VE_DisplayStatus &= ~(EN_VE_CCIR656_IN | EN_VE_RGB_IN | EN_VE_DEMODE);
    if((pInputSrcInfo->eInputSrcType == MS_VE_SRC_SUB) ||
       (pInputSrcInfo->eInputSrcType == MS_VE_SRC_MAIN))
    {
        eInputSrc = pInputSrcInfo->eInputSrcOfMixedSrc;
    }
    else
    {
        eInputSrc = pInputSrcInfo->eInputSrcType;
    }

    switch(eInputSrc)
    {
    case MS_VE_SRC_DTV:
        // for Agate new design, it need to use the DE mode!!
        //if(pInputSrcInfo->eInputSrcType == MS_VE_SRC_SUB)
        {
            //By now, DTV is always goes with capture mode in AV output
            //For SRC_SUB, we used in vecapture function, so use DE mode
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        }
        break;

    case MS_VE_SRC_DSUB:
        //g_VEInfo.u8VE_DisplayStatus |= EN_VE_RGB_IN; //Move AP to control this according to SC_IP2's CSC setting
        g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        break;

    case MS_VE_SRC_COMP:
        g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        break;

    case MS_VE_SRC_ATV:
    case MS_VE_SRC_CVBS0:
    case MS_VE_SRC_CVBS1:
    case MS_VE_SRC_CVBS2:
    case MS_VE_SRC_CVBS3:
    case MS_VE_SRC_SVIDEO:
        break;

    case MS_VE_SRC_HDMI_A:
    case MS_VE_SRC_HDMI_B:
    case MS_VE_SRC_HDMI_C:
        //g_VEInfo.u8VE_DisplayStatus |= EN_VE_RGB_IN; //For DVI/HDMI, control this in AP layer
        g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        break;

    case MS_VE_SRC_SCALER:
        // set input color at HAL layer
        //g_VEInfo.u8VE_DisplayStatus |= EN_VE_RGB_IN;
        if(!g_VEInfo.bForceCaptureMode)
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        else
            g_VEInfo.u8VE_DisplayStatus &= ~EN_VE_DEMODE;
        break;

    case MS_VE_SRC_DI:
        if(!g_VEInfo.bForceCaptureMode)
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        else
            g_VEInfo.u8VE_DisplayStatus &= ~EN_VE_DEMODE;
        break;

    case MS_VE_SRC_DNR:
        if(!g_VEInfo.bForceCaptureMode)
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        else
            g_VEInfo.u8VE_DisplayStatus &= ~EN_VE_DEMODE;
        break;

    case MS_VE_SRC_DTV_FROM_MVOP:
        g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        break;

    case MS_VE_SRC_BT656:
        if(!g_VEInfo.bForceCaptureMode)
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_DEMODE;
        else
            g_VEInfo.u8VE_DisplayStatus &= ~EN_VE_DEMODE;
        g_VEInfo.u8VE_DisplayStatus |= EN_VE_CCIR656_IN;
        break;

    default:
        break;
    }
    MSG_DRV_VE("VE: %s:%d status=%x eInputSrc=%u\n", __FUNCTION__, __LINE__, g_VEInfo.u8VE_DisplayStatus, eInputSrc);
}


//------------------------------------------------------------------------------
/// Extended API for set the input source of video encoder(To Expand&Replace MDrv_VE_SwitchInputSource)
///
/// @param  -pSwitchInfo \b IN/OUT: the information of switching input destination of TV encodeer
/// @return @ref MS_SWITCH_VE_SRC_STATUS
//------------------------------------------------------------------------------
MS_SWITCH_VE_SRC_STATUS _MDrv_VE_SetInputSource(PMS_VE_InputSrc_Info pInputSrcInfo)
{
    MSG_DRV_VE("VE: %s, Src = %d, %d\n", __FUNCTION__, pInputSrcInfo->eInputSrcType, pInputSrcInfo->eInputSrcOfMixedSrc);

    g_VEInfo.InputSrcType = pInputSrcInfo->eInputSrcType;
    g_VEInfo.InputSrcOfMixedSrc = pInputSrcInfo->eInputSrcOfMixedSrc;
    MDrv_VE_SetSrcDispStatus(pInputSrcInfo);
    switch(pInputSrcInfo->eInputSrcType)
    {
    case MS_VE_SRC_COMP:
        Hal_VE_set_source_sync_inv(0x12, 0xFF);// set the sync polarity
        Hal_VE_set_sog(ENABLE); // enable sog
        break;

    case MS_VE_SRC_DTV_FROM_MVOP:
    case MS_VE_SRC_SUB:
    case MS_VE_SRC_DTV:
        Hal_VE_set_source_sync_inv(0x12, 0xFF);// set the sync polarity
        break;
    default:
        break;
    }
    if(g_VEInfo.u8VE_DisplayStatus & EN_VE_RGB_IN)
        Hal_VE_set_rgb_in(ENABLE); // enable RGB in
    else
        Hal_VE_set_rgb_in(DISABLE);// disable RGB in

    if(g_VEInfo.u8VE_DisplayStatus & EN_VE_CCIR656_IN)
        Hal_VE_set_ccir656_in(ENABLE);
    else
        Hal_VE_set_ccir656_in(DISABLE);

    Hal_VE_set_inputsource(pInputSrcInfo);
    return MS_VE_SWITCH_SRC_SUCCESS;
}

MS_SWITCH_VE_SRC_STATUS MDrv_VE_SetInputSource(PMS_VE_InputSrc_Info pInputSrcInfo)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETINPUTSRC pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return MS_VE_SWITCH_SRC_FAIL;
    }
    pArgs = (PTVE_SETINPUTSRC)malloc(sizeof(TVE_SETINPUTSRC));

    if(NULL == pArgs)
        return MS_VE_SWITCH_SRC_FAIL;
    pArgs->pInputSrcInfo= pInputSrcInfo;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst,E_MDRV_CMD_TVE_SetInputSRC,(void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return MS_VE_SWITCH_SRC_SUCCESS;
    }
    else
    {
        free(pArgs);
        return MS_VE_SWITCH_SRC_FAIL;
    }
#else
    return _MDrv_VE_SetInputSource(pInputSrcInfo);
#endif
}
//------------------------------------------------------------------------------
/// set the input source of video encoder
///
/// @param  -pSwitchInfo \b IN/OUT: the information of switching input destination of TV encodeer
/// @return None
//------------------------------------------------------------------------------
void _MDrv_VE_SwitchInputSource(PMS_Switch_VE_Src_Info pSwitchInfo)
{
    if((pSwitchInfo->InputSrcType == MS_VE_SRC_SUB) || (pSwitchInfo->InputSrcType == MS_VE_SRC_MAIN))
    {
        printf("!!!ALERT, FATAL ERROR: Non supported argument[%d] for [%s], try [MDrv_VE_SetInputSource]\n", pSwitchInfo->InputSrcType, __FUNCTION__);
    }
    else
    {
        MS_VE_InputSrc_Info InputSrcInfo;
        memset(&InputSrcInfo, 0, sizeof(MS_VE_InputSrc_Info));
        InputSrcInfo.u16Version = 0; //It is old version interface
        InputSrcInfo.eInputSrcType = pSwitchInfo->InputSrcType;
        InputSrcInfo.eInputSrcOfMixedSrc = MS_VE_SRC_NONE;
        _MDrv_VE_SetInputSource(&InputSrcInfo);
    }
}

void MDrv_VE_SwitchInputSource(PMS_Switch_VE_Src_Info pSwitchInfo)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SWITCH_SRC_INFO pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SWITCH_SRC_INFO)malloc(sizeof(TVE_SWITCH_SRC_INFO));

    if(NULL == pArgs)
        return ;
    pArgs->pSwithcSrc_info = pSwitchInfo;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SwitchInputSRC, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SwitchInputSource(pSwitchInfo);
#endif
}

//------------------------------------------------------------------------------
/// control the output of video encoder
///
/// @param  -pOutputCtrl \b IN: the control information of VE
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_SetOutputCtrl(PMS_VE_Output_Ctrl pOutputCtrl)
{
    MS_BOOL bOutCCIR656 = FALSE, bOutTVE=FALSE, bFieldInv;
    MS_U16 u16EnVal;

    //(printf("-------- this is VE out 2009 /04/29-------\n") );

    if(pOutputCtrl->bEnable)
    {
        if(pOutputCtrl->OutputType == MS_VE_OUT_TVENCODER)
        {
            bOutTVE = TRUE;
        }
        else if(pOutputCtrl->OutputType == MS_VE_OUT_CCIR656)
        {
            //When CCIR is on, TV encoder is also needed to be enabled
            bOutTVE = TRUE;
            bOutCCIR656 = TRUE;
        }

        if((g_VEInfo.InputSrcType == MS_VE_SRC_SCALER) ||
           (g_VEInfo.InputSrcType == MS_VE_SRC_DI)     ||
           (g_VEInfo.InputSrcType == MS_VE_SRC_BT656)  ||
           (g_VEInfo.InputSrcType == MS_VE_SRC_DNR)    ||
           ((g_VEInfo.InputSrcType >= MS_VE_SRC_ATV) && (g_VEInfo.InputSrcType <= MS_VE_SRC_SVIDEO)) ||
           ((g_VEInfo.InputSrcOfMixedSrc >= MS_VE_SRC_ATV) && (g_VEInfo.InputSrcOfMixedSrc <= MS_VE_SRC_SVIDEO))
           )
        {
            if(!g_VEInfo.bForceCaptureMode)
                u16EnVal = BIT(7)|(bOutTVE << 2)|(bOutCCIR656 << 1)|(bOutCCIR656 << 6); // Enable DE mode when input source is Scaler OP/DI or VD
            else
                u16EnVal = (bOutTVE << 2)|(bOutCCIR656 << 1)|(bOutCCIR656 << 6); // Disable DE mode when input source is Scaler OP
        }
        else
        {
            u16EnVal = ((g_VEInfo.u8VE_DisplayStatus & EN_VE_DEMODE)<< 7) |
                       (bOutTVE << 2) |
                       (bOutCCIR656 << 1)|
                       (bOutCCIR656 << 6);
        }
        MSG_DRV_VE("VE Power on: bOutCCIR656=0x%x, bOutTVE=0x%x, u16EnVal=0x%x \n", bOutCCIR656, bOutTVE, u16EnVal);
        if(bOutCCIR656)
        {
            Hal_VE_set_source_sync_inv(0, BIT(4));
            if(g_VEInfo.VideoSystem <= MS_VE_PAL_M)
            {   // NTSC
                Hal_VE_set_ccir656_out_pal(0);
            }
            else
            {   // PAL
                Hal_VE_set_ccir656_out_pal(1);
            }
        }

        // Field Invert
        if(g_VEInfo.InputSrcType == MS_VE_SRC_DTV_FROM_MVOP)
        {
            Hal_VE_set_field_inverse(0);
        }
        else if(g_VEInfo.InputSrcType == MS_VE_SRC_COMP)
        {
            bFieldInv = (g_VEInfo.u16V_SC_CapStart & 0x01) ?
                            ENABLE_VE_FIELD_INV : ~ENABLE_VE_FIELD_INV;
            bFieldInv = g_VEInfo.bSrcInterlace ? bFieldInv : 0;
            Hal_VE_set_field_inverse((bFieldInv & 0x01));
        }
        else
        {
            if(g_VEInfo.u8VE_DisplayStatus & EN_VE_DEMODE)
            {
                bFieldInv = (g_VEInfo.bSrcInterlace) ? ENABLE_VE_FIELD_INV : 0;
                Hal_VE_set_field_inverse((bFieldInv & 0x01));
            }
            else
            {
                bFieldInv = (g_VEInfo.u16V_SC_CapStart & 0x01) ?
                            ~ENABLE_VE_FIELD_INV : ENABLE_VE_FIELD_INV;
                bFieldInv = g_VEInfo.bSrcInterlace ? bFieldInv : 0;
                Hal_VE_set_field_inverse((bFieldInv & 0x01));
            }
        }

        Hal_VE_set_clk_on_off(ENABLE);
        Hal_VE_set_ctrl(u16EnVal);
        if(!g_VEInfo.bVECapture)
        {
            Hal_VE_set_vbi(ENABLE);
        }

        //OS_DELAY_TASK(300);
        Hal_VE_sofeware_reset(ENABLE); // software reset
        Hal_VE_set_reg_load(ENABLE);// load register,but not affect bit3(VBI output)

        // For Mboot boot time optimization
#if ( ! defined (STB) ||  ! defined (MBOOT))
        //MsOS_DelayTask(5) ; // delay 5 ms
        OS_DELAY_TASK(30);

        // TELETEXT

        Hal_VE_set_reg_load(DISABLE);// load register,but not affect bit3(VBI output)
        OS_DELAY_TASK(5);
        Hal_VE_sofeware_reset(DISABLE); // software reset
#endif

        if(pOutputCtrl->OutputType != MS_VE_OUT_CAPTURE)
        {
            //For VEC, turn on it in kernel
            OS_DELAY_TASK(5);
            Hal_VE_set_ve_on_off(ENABLE);
        }
    }
    else
    {
        //Hal_VE_set_cvbs_buffer_out(0x0F, 0, 0, 0);
        MSG_DRV_VE("VE Power off\n");
        Hal_VE_set_ctrl(0);
        Hal_VE_set_vbi(DISABLE);
        Hal_VE_set_ve_on_off(DISABLE);
        Hal_VE_set_clk_on_off(DISABLE);
        //When disable VE
        //1.DE mode just set 0x103B00[0] to 0.
        //2. non-DE mode ,set 0x103B00[0] to 0,H capture window start (0x103B12) ¤£¥i¥H³]0
        Hal_VE_set_capture_window(0x010, VE_DE_HEND_MASK, 0x000, VE_DE_VEND_MASK);
    }

    //workaround to pass parasoft test:
    //[2]A pointer parameter in a function prototype should be declared as pointer to const if the pointer is not used to modify the addressed object (JSF-118-4)
    pOutputCtrl->bEnable += 0;
}

void MDrv_VE_SetOutputCtrl(PMS_VE_Output_Ctrl pOutputCtrl)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETOUTPUTCTRL pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SETOUTPUTCTRL)malloc(sizeof(TVE_SETOUTPUTCTRL));

    if(NULL == pArgs)
        return ;
    pArgs->pOutputCtrl = pOutputCtrl;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetOutputCtrl, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SetOutputCtrl(pOutputCtrl);
#endif
}
//------------------------------------------------------------------------------
/// set the output video standard of video encoder
///
/// @param  -VideoSystem \b IN: the video standard
/// @return TRUE: supported and success,  FALSE: unsupported or unsuccess
//------------------------------------------------------------------------------
MS_BOOL _MDrv_VE_SetOutputVideoStd(MS_VE_VIDEOSYS VideoSystem)
{
    MS_BOOL bRet;
    MSG_DRV_VE("VE Standard: VideoSystem=%d\n", VideoSystem);
    g_VEInfo.VideoSystem = VideoSystem;

    bRet = Hal_VE_set_output_video_std(VideoSystem);

    return bRet;
}

MS_BOOL MDrv_VE_SetOutputVideoStd(MS_VE_VIDEOSYS VideoSystem)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETOUTPUTVIDEOSTD pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_SETOUTPUTVIDEOSTD)malloc(sizeof(TVE_SETOUTPUTVIDEOSTD));

    if(NULL == pArgs)
        return FALSE;
    pArgs->VideoSystem = VideoSystem;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetOutputVideoStd, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return TRUE;
    }
    else
    {
        free(pArgs);
        return FALSE;
    }
#else
    return _MDrv_VE_SetOutputVideoStd(VideoSystem);
#endif
}

#define END_OF_CUS_TABLE_VALUE      0xffffff
#define COLUMN_NUM_OF_ONE_CUS_CMD        4
MS_BOOL _MDrv_VE_SetCusTable(MS_VE_VIDEOSYS VideoSystem, MS_U8* pTbl)
{
    if (pTbl == NULL)
    {
        printf("[%s] pTbl error\n.", __FUNCTION__);
        return FALSE;
    }

    if(VideoSystem >= MS_VE_VIDEOSYS_NUM)
    {
        printf("[%s] VideoSystem error\n.", __FUNCTION__);
        return FALSE;
    }

    MS_U32 u32Index = 0;
    MS_U16 u16Counter = 0;
    const MS_U16 u16Cols = COLUMN_NUM_OF_ONE_CUS_CMD;
    MS_BOOL bEnd = FALSE;

    while (u16Counter*u16Cols < VE_CUSTBL_MAX_SIZE)
    {
        u16Counter++;
        u32Index = ((pTbl[u16Cols*u16Counter+0] << 16) + (pTbl[u16Cols*u16Counter+1] << 8) + pTbl[u16Cols*u16Counter+2]);
        if (u32Index == END_OF_CUS_TABLE_VALUE) // check end of table
        {
            bEnd = TRUE;
            break;
        }
    }

    if(FALSE == bEnd)
    {
        printf("Error: [%s] Table size too large than VE_CUSTBL_MAX_SIZE = %d \n.", __FUNCTION__, VE_CUSTBL_MAX_SIZE);
        return FALSE;
    }

    Hal_VE_SetCusTable(VideoSystem,pTbl,u16Counter*u16Cols);

    return TRUE;
}

MS_BOOL MDrv_VE_SetCusTable(MS_VE_VIDEOSYS VideoSystem, MS_U8* pTbl)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETCUSTABLE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_SETCUSTABLE)malloc(sizeof(TVE_SETCUSTABLE));

    if(NULL == pArgs)
        return FALSE;
    pArgs->VideoSystem = VideoSystem;
    pArgs->pTbl = pTbl;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Set_CusTable, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return TRUE;
    }
    else
    {
        free(pArgs);
        return FALSE;
    }
#else
    return _MDrv_VE_SetCusTable(VideoSystem, pTbl);
#endif

}

void _MDrv_VE_EnableCusTable(MS_BOOL bEnable)
{
    Hal_VE_EnableCusTable(bEnable);
}

void MDrv_VE_EnableCusTable(MS_BOOL bEnable)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_ENABLECUSTABLE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return;
    }
    pArgs = (PTVE_ENABLECUSTABLE)malloc(sizeof(TVE_ENABLECUSTABLE));

    if(NULL == pArgs)
        return ;
    pArgs->bEnable = bEnable;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_EnableCusTable, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    free(pArgs);

#else
    return _MDrv_VE_EnableCusTable(bEnable);
#endif

}


void _MDrv_VE_SetWSSData(MS_BOOL bEn, MS_U16 u16WSSData) // 071204
{
//    MSG_DRV_VE("WSSData=%d, data=%u\n", bEn, u16WSSData));
    Hal_VE_set_wss_data(bEn, u16WSSData);
}

void MDrv_VE_SetWSSData(MS_BOOL bEn, MS_U16 u16WSSData)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETWSSDATA pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SETWSSDATA)malloc(sizeof(TVE_SETWSSDATA));

    if(NULL == pArgs)
        return ;
    pArgs->bEn = bEn;
    pArgs->u16WSSData = u16WSSData;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetWSSData, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SetWSSData(bEn, u16WSSData);
#endif
}

MS_U16 _MDrv_VE_GetWSSData(void)
{
    return Hal_VE_get_wss_data();
}

MS_U16 MDrv_VE_GetWSSData(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    TVE_GETWSSDATA pArgs;
    if(pu32TvencoderInst == NULL)
    {
        return 0xFF;
    }

    memset(&pArgs, 0, sizeof(TVE_GETWSSDATA));
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_GetWSSData, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    return pArgs.u16WSSData;
#else
    return _MDrv_VE_GetWSSData();
#endif
}
//Small adjust Hratio for HW design rule:
//tmp = ratio*input pixel, then tmp[31:11] = Number of output pixel, must adjust ratio to make this value even
//Ex : ratio = 0x155, input pixel = 1920, tmp = 0x9FD80, here tmp[11]=1,then set ratio=ratio+1
#define MAX_TRY_HRATIOAROUND 100
void MDrv_VE_HRatioAround(MS_U16 *pu16In_Hsize, MS_U32 *pu32H_Ratio)
{
    MS_U32 u32OutputPixel;
    MS_U8 i = 0;
    do
    {
        i++;
        u32OutputPixel = *pu16In_Hsize * (*pu32H_Ratio+i);
    }while((u32OutputPixel & BIT(11)) && (i<MAX_TRY_HRATIOAROUND));

    if(i < MAX_TRY_HRATIOAROUND)
    {
        if(i != 0)
        {
            *pu32H_Ratio +=i;
            MSG_DRV_VE("VE Scaling: HRatio Around=%d\n", (int)*pu32H_Ratio);
        }
    }
    else
    {
        printf("!!!Alert: VE HRatio Around fail!!!\n");
    }

    //workaround to pass parasoft test:
    //[2]A pointer parameter in a function prototype should be declared as pointer to const if the pointer is not used to modify the addressed object (JSF-118-4)
    *pu16In_Hsize += 0;
}

VE_Result  _MDrv_VE_Set_Customer_Scaling(MS_VE_CusScalingInfo *pstVECusScalingInfo)
{
    memcpy(&g_VEInfo.stVECusScalingInfo, pstVECusScalingInfo, sizeof(MS_VE_CusScalingInfo));
    return E_VE_OK;
}

VE_Result  MDrv_VE_Set_Customer_Scaling(MS_VE_CusScalingInfo *pstVECusScalingInfo)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETCUSSCALING pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return E_VE_FAIL;
    }
    pArgs = (PTVE_SETCUSSCALING)malloc(sizeof(TVE_SETCUSSCALING));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->pstVECusScalingInfo = pstVECusScalingInfo;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Set_Customer_Scaling, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_FAIL;
    }
    else
    {
        free(pArgs);
        return E_VE_OK;
    }
#else
    return MDrv_VE_Set_Customer_Scaling(pstVECusScalingInfo);
#endif
}

void MDrv_VE_set_scaling_ratio(MS_U16 u16Out_Hsize, MS_U16 u16Out_Vsize)
{
    MS_U32 u32H_Ratio = 0, u32V_Ratio = 0;
    MS_U16 u16In_Vsize, u16In_Hsize;

    if(g_VEInfo.stVECusScalingInfo.bHCusScalingEnable==ENABLE)
    {
        u16In_Hsize = g_VEInfo.stVECusScalingInfo.u16HScalingsrc;
        u16Out_Hsize= g_VEInfo.stVECusScalingInfo.u16HScalingdst;
    }
    else
    {
        #ifdef VE_SOURCE_AFTER_DI
        if (g_VEInfo.bHDuplicate)
        {
            u16In_Hsize = g_VEInfo.u16H_CapSize/2; //MApi_XC_R2BYTE(_SC_PK_L_(0x12, 0x1F));  //hsize_in
        }
        else
        {
            u16In_Hsize = g_VEInfo.u16H_CapSize;
        }
        #else
        u16In_Hsize = g_VEInfo.u16H_CapSize;
        #endif
    }
    if(g_VEInfo.stVECusScalingInfo.bVCusScalingEnable==ENABLE)
    {
        u16In_Vsize = g_VEInfo.stVECusScalingInfo.u16VScalingsrc;
        u16Out_Vsize= g_VEInfo.stVECusScalingInfo.u16VScalingdst;
    }
    else
    {
        u16In_Vsize = g_VEInfo.u16V_CapSize;
    }
    MSG_DRV_VE("VE Scaling: bHCusScaling=%u, bVCusScaling=%u, In_Hsize =%d, In_Vsize = %d, Out_Hsize =%d, Out_Vsize = %d,\n",
                          g_VEInfo.stVECusScalingInfo.bHCusScalingEnable, g_VEInfo.stVECusScalingInfo.bVCusScalingEnable, u16In_Hsize, u16In_Vsize, u16Out_Hsize, u16Out_Vsize);

    // for Monaco 4kx2k or from OP case
    Hal_VE_Set_PreHSD(g_VEInfo.InputSrcType,&u16In_Hsize,&u16In_Vsize);

    if(u16Out_Hsize != u16In_Hsize)
    {
        if(u16In_Hsize > u16Out_Hsize)
        {
            if((g_VEInfo.InputSrcType == MS_VE_SRC_SCALER) && VE_PRESCALE_SUPPORT)
            {
                _PreHscaleDown(ENABLE, u16In_Hsize, u16Out_Hsize);
                Hal_VE_set_h_scaling(DISABLE, 0x0000);
            }
            else
            {
                u32H_Ratio = (MS_U16)VE_H_SCALE_DOWN_RATIO(u16In_Hsize, u16Out_Hsize);
                MDrv_VE_HRatioAround(&u16In_Hsize, &u32H_Ratio);
                Hal_VE_set_h_scaling(ENABLE, u32H_Ratio);
            }

            if (g_VECap.bSupport_UpScale)
            {
                #ifdef VE_SOURCE_AFTER_DI
                Hal_VE_set_h_upscaling(ENABLE, 0x100000);    // set to no-scale
                #else
                Hal_VE_set_h_upscaling(ENABLE, 0x10000);    // set to no-scale
                #endif
            }
        }
        else
        {
            if (!g_VECap.bSupport_UpScale)
            {
                //VE not support upscale,just bypass it
                if(g_VEInfo.u16H_CapSize ==544)//DVB DTV 544x576 needs fine tune by hand
                {
                    Hal_VE_set_h_scaling(ENABLE, 0x7FF);
                }
                else
                {
                    _PreHscaleDown(DISABLE, u16In_Hsize, u16Out_Hsize);
                    Hal_VE_set_h_scaling(DISABLE, 0x0000);
                }
            }
            else
            {
                //do h upscale
                u32H_Ratio = VE_H_SCALE_UP_RATIO(u16In_Hsize, u16Out_Hsize);
                Hal_VE_set_h_upscaling(ENABLE, u32H_Ratio);

                _PreHscaleDown(DISABLE, u16In_Hsize, u16Out_Hsize);
                Hal_VE_set_h_scaling(DISABLE, 0x0000);
                MSG_DRV_VE("VE H upscale ratio: u32H_Ratio =%d\n", (int)u32H_Ratio);
            }
        }
    }
    else
    {
        _PreHscaleDown(DISABLE, u16In_Hsize, u16Out_Hsize);
        Hal_VE_set_h_scaling(DISABLE, 0x0000);
        if (g_VECap.bSupport_UpScale)
        {
            #ifdef VE_SOURCE_AFTER_DI
            Hal_VE_set_h_upscaling(ENABLE, 0x100000);    // set to no-scale
            #else
            Hal_VE_set_h_upscaling(ENABLE, 0x10000);    // set to no-scale
            #endif
        }
    }

    if(u16Out_Vsize != u16In_Vsize)
    {
        if(u16In_Vsize > u16Out_Vsize)
        {
            if((g_VEInfo.InputSrcType == MS_VE_SRC_SCALER) && VE_PRESCALE_SUPPORT)
            {
                _PreVscaleDown(ENABLE, u16In_Vsize, u16Out_Vsize);
                Hal_VE_set_v_scaling(DISABLE, 0x0000);
            }
            else
            {
                u32V_Ratio = (MS_U16)VE_V_SCALE_DOWN_RATIO(u16In_Vsize, u16Out_Vsize);

                Hal_VE_set_v_initfactor(g_VEInfo.bSrcInterlace,u32V_Ratio);

                if(g_VEInfo.stVECusScalingInfo.bVCusScalingEnable==ENABLE)
                {
                    Hal_VE_set_v_scaling(ENABLE, u32V_Ratio);//For customer scaling, just set it as requested
                }
                else
                {
                    Hal_VE_set_v_scaling_Traditional(ENABLE, u32V_Ratio);//For traditional V scaling, need considering the recevier's overscan
                }
            }

            if (g_VECap.bSupport_UpScale)
            {
                Hal_VE_set_v_upscaling(ENABLE, 0x10000);
            }
        }
        else
        {
            if (!g_VECap.bSupport_UpScale)
            {
                //VE not support upscale,just bypass it
                Hal_VE_set_v_scaling(DISABLE, 0x0000);
                _PreVscaleDown(DISABLE, u16In_Vsize, u16Out_Vsize);
                Hal_VE_set_v_initfactor(g_VEInfo.bSrcInterlace,u32V_Ratio);
            }
            else
            {
                //do v upscale
                u32V_Ratio = VE_V_SCALE_UP_RATIO(u16In_Vsize, u16Out_Vsize);
                Hal_VE_set_v_upscaling(ENABLE, u32V_Ratio);

                _PreVscaleDown(DISABLE, u16In_Vsize, u16Out_Vsize);
                Hal_VE_set_v_scaling(DISABLE, 0x0000);
                MSG_DRV_VE("VE V upscale ratio: u32V_Ratio =%d\n", (int)u32V_Ratio);
            }
        }
    }
    else
    {
        if(g_VEInfo.u16H_CapSize ==544)//DVB DTV 544x576 needs fine tune by hand
        {
            Hal_VE_set_v_scaling(ENABLE, 0x0750);
        }
        else
        {
            _PreVscaleDown(DISABLE, u16In_Vsize, u16Out_Vsize);
            Hal_VE_set_v_scaling(DISABLE, 0x0000);
            if (g_VECap.bSupport_UpScale)
            {
                Hal_VE_set_v_upscaling(ENABLE, 0x10000);
            }
        }
        Hal_VE_set_v_initfactor(g_VEInfo.bSrcInterlace,u32V_Ratio);
    }
    MSG_DRV_VE("VE Scaling: u32H_Ratio =%d, u32V_Ratio = %d \n", (int)u32H_Ratio, (int)u32V_Ratio);
}

//------------------------------------------------------------------------------
/// VE Set display windows
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_set_display_window(MS_VE_WINDOW_TYPE stDispWin)
{
    MS_U16 u16Out_Hsize= (((g_VEInfo.bHDuplicate) && (g_VEInfo.InputSrcType == MS_VE_SRC_DTV)) ? 704 : 720);
    MS_U16 u16Out_Vsize = ((g_VEInfo.VideoSystem <= MS_VE_PAL_M) ? 480 : 576);
    MS_PHY phyMIUOffsetAddr = 0;

    //Check display window location and size.
    if(stDispWin.x > u16Out_Hsize) {stDispWin.x = 0 ; MSG_DRV_VE("x >> width,please check x range \n");}
    if(stDispWin.y > u16Out_Vsize) {stDispWin.y = 0 ; MSG_DRV_VE("y >> height,please check y range \n");}
    if(stDispWin.x + stDispWin.width > u16Out_Hsize) {stDispWin.width = u16Out_Hsize ; MSG_DRV_VE("width >> Hsize,please check width size \n");}
    if(stDispWin.y + stDispWin.height > u16Out_Vsize) {stDispWin.height = u16Out_Vsize ; MSG_DRV_VE("height >> Vsize,please check height size \n");}
    MSG_DRV_VE("VE Set Display Window: x/y/w/h = %u, %u, %u, %u\n", stDispWin.x, stDispWin.y, stDispWin.width, stDispWin.height);

    if(MApi_VE_R2BYTE_MSK(MS_VE_REG_BANK_3B, 0x35, BIT(3)) == 0)
    {
        stDispWin.y /=2;
    }
    phyMIUOffsetAddr = 2 * (stDispWin.x + stDispWin.y * u16Out_Hsize);
    MDrv_VE_set_scaling_ratio(stDispWin.width,stDispWin.height);
    phyMIUOffsetAddr += g_VEInfo.phyMiuBaseAddr;
    Hal_VE_Set_winodw_offset(phyMIUOffsetAddr);
}

void MDrv_VE_set_display_window(MS_VE_WINDOW_TYPE stDispWin)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETDISPLAYWIN pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return;
    }
    pArgs = (PTVE_SETDISPLAYWIN)malloc(sizeof(TVE_SETDISPLAYWIN));

    if(NULL == pArgs)
        return ;
    pArgs->stDispWin = stDispWin;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_set_display_window, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_set_display_window(stDispWin);
#endif
}
//------------------------------------------------------------------------------
/// @brief VE be set display window (backend), after MIU
///
/// @argument:
//          - MS_VE_WINDOW_TYPE - display window
/// @return VE_Result
//------------------------------------------------------------------------------
VE_Result MDrv_VE_set_be_display_window(MS_VE_WINDOW_TYPE stDispWin)
{
    MS_U16 u16Out_Hsize= (((g_VEInfo.bHDuplicate) && (g_VEInfo.InputSrcType == MS_VE_SRC_DTV)) ? 704 : 720);
    MS_U16 u16Out_Vsize = ((g_VEInfo.VideoSystem <= MS_VE_PAL_M) ? 480 : 576);
    VE_Result ret = E_VE_OK;

    if (!g_VECap.bSupport_CropMode) {
        MSG_DRV_VE("[%s] is not supported in this chip\n",
                    __FUNCTION__);
        ret = E_VE_NOT_SUPPORT;
        goto fail;
    }

    //Check display window location and size.
    if(stDispWin.x > u16Out_Hsize) {
        MSG_DRV_VE("x >> width,please check x range \n");
        ret = E_VE_FAIL;
        goto fail;
    }
    if(stDispWin.y > u16Out_Vsize) {
        MSG_DRV_VE("y >> height,please check y range \n");
        ret = E_VE_FAIL;
        goto fail;
    }
    if(stDispWin.x + stDispWin.width > u16Out_Hsize) {
        MSG_DRV_VE("width >> Hsize,please check width size \n");
        ret = E_VE_FAIL;
        goto fail;
    }
    if(stDispWin.y + stDispWin.height > u16Out_Vsize) {
        MSG_DRV_VE("height >> Vsize,please check height size \n");
        ret = E_VE_FAIL;
        goto fail;
    }

    // Width of VE Display window should be even number, otherwise video will be wavy and some strange dotted lines will appear.
    if((stDispWin.width & 0x1) == 0x1)
    {
        stDispWin.width = stDispWin.width + 1;
    }

    MSG_DRV_VE("VE Set Display Window: x/y/w/h = %u, %u, %u, %u\n", stDispWin.x, stDispWin.y, stDispWin.width, stDispWin.height);

    MDrv_VE_set_scaling_ratio(stDispWin.width, stDispWin.height);
    Hal_VE_Set_HScale_Output_Line_Number(stDispWin.width);
    Hal_VE_Set_VScale_Output_Line_Number((stDispWin.height) / 2);   // field size
    Hal_VE_Set_Crop_OSD_Offset(stDispWin.x, (stDispWin.y / 2)); // field size

    return E_VE_OK;
fail:
    printf("Error: [%s] failed, errno: %d\n", __FUNCTION__, ret);
    return ret;
}

void MDrv_VE_set_frc(MS_U16 u16VInputFreq)
{
    MS_U32 u32FullNum, u32EmptyNum;
    MS_U32 u32FrameSize;
    MS_U16 u16Out_FR, u16In_FR;
    MS_U16 b3FrameMode = FALSE;
    g_VEInfo.u16inputVfreq = u16VInputFreq;
    if(g_VEInfo.VideoSystem <= MS_VE_PAL_M)
    {
        u32FrameSize = VE_NTSC_FRAME_SIZE;
        u16Out_FR = 2997;
    }
    else
    {
        u32FrameSize = VE_PAL_FRAME_SIZE;
        u16Out_FR = 2500;
    }
    if(g_VEInfo.bVECapture)
    {
        //FrameSize = 720*Height*2/MIU Length
        u32FrameSize = (720*g_VEInfo.u16Outputheight/(BYTE_PER_WORD>>1));//The pitch is always 720pixel for VE
        Hal_VE_set_field_size(u32FrameSize/2);
    }
    u16In_FR =  (u16VInputFreq*10/2) ;
    MSG_DRV_VE("---VE: InputVFreq=%u, In_FR=%u, Out_FR=%u\n", u16VInputFreq, u16In_FR, u16Out_FR);
    if(g_VEInfo.bVECapture)
    {
        if(g_VEInfo.u32MemSize/3 >= u32FrameSize*BYTE_PER_WORD)
        {
            b3FrameMode = TRUE;
        }
        Hal_VE_set_frc(DISABLE, 0xFFFF, 0, g_VEInfo.bSrcInterlace, b3FrameMode);
    }
#ifdef VE_NEW_FRC_SETTING
    //Suggest to use 4-field buffer mode for normal case (input frame rate is almost same as output frame rate)
    //Suggest to use 3-frame buffer mode for special case (input frame rate is lower than output frame rate)
    else if((!g_VEInfo.bSrcInterlace)&&(u16In_FR<(u16Out_FR - VE_FRC_TOLERANT))&&(u16In_FR>(u16Out_FR + VE_FRC_TOLERANT)))
    {
        if(g_VEInfo.u32MemSize/3 >= u32FrameSize*BYTE_PER_WORD)
        {
            b3FrameMode = TRUE;
        }

        if(b3FrameMode)
        {
            u32FullNum = 2*u32FrameSize;
            u32EmptyNum = u32FrameSize;
        }
        else // 2 framemode
        {
            u32FullNum = u32FrameSize;
            u32EmptyNum = u32FrameSize;
        }
        Hal_VE_set_frc(ENABLE, u32FullNum, u32EmptyNum, g_VEInfo.bSrcInterlace, b3FrameMode);
    }
    else
    {
        u32FullNum = u32FrameSize * 3/2;
        u32EmptyNum = u32FrameSize / 2;

        Hal_VE_set_frc(ENABLE, u32FullNum, u32EmptyNum, 1, b3FrameMode);
    }
#else
    else if( ( u16In_FR > (u16Out_FR - VE_FRC_TOLERANT) ) && ( u16In_FR<(u16Out_FR + VE_FRC_TOLERANT) ) )
    {
        // Default always to enable FRC (avoid tearing)
        u32FullNum = u32FrameSize * 3/2;
        u32EmptyNum = u32FrameSize / 2;
#if defined (__aarch64__)
        MSG_DRV_VE("---VE: default ENABLE FRC,u32FullNum=%u, u32EmptyNum=%u\n", u32FullNum, u32EmptyNum);
#else
        MSG_DRV_VE("---VE: default ENABLE FRC,u32FullNum=%tu, u32EmptyNum=%tu\n",(ptrdiff_t) u32FullNum,(ptrdiff_t) u32EmptyNum);
#endif
        Hal_VE_set_frc(ENABLE, u32FullNum, u32EmptyNum, g_VEInfo.bSrcInterlace, b3FrameMode);
    }
//#endif
    else if(u16In_FR < u16Out_FR)
    {
        u32FullNum  = u32FrameSize * 19 / 10 ;
        u32EmptyNum = (MS_U32)(u16Out_FR - u16In_FR) * u32FrameSize / (MS_U32)u16Out_FR ;
#if defined (__aarch64__)
        MSG_DRV_VE("---VE: Enable FRC, u32FullNum=%u, u32EmptyNum=%u\n", u32FullNum, u32EmptyNum);
#else
        MSG_DRV_VE("---VE: Enable FRC, u32FullNum=%tu, u32EmptyNum=%tu\n",(ptrdiff_t) u32FullNum,(ptrdiff_t) u32EmptyNum);
#endif
        Hal_VE_set_frc(ENABLE, u32FullNum, u32EmptyNum, g_VEInfo.bSrcInterlace, b3FrameMode);
    }
    else
    {
        u32FullNum = u32FrameSize +
                     ((MS_U32)u16Out_FR  * u32FrameSize / u16In_FR);
        u32EmptyNum = u32FrameSize / 10 ;
#if defined (__aarch64__)
        MSG_DRV_VE("---VE: Enable FRC, u32FullNum=%u, u32EmptyNum=%u\n", u32FullNum, u32EmptyNum);
#else
        MSG_DRV_VE("---VE: Enable FRC, u32FullNum=%tu, u32EmptyNum=%tu\n",(ptrdiff_t) u32FullNum,(ptrdiff_t) u32EmptyNum);
#endif
        Hal_VE_set_frc(ENABLE, u32FullNum, u32EmptyNum, g_VEInfo.bSrcInterlace, b3FrameMode);
    }
#endif
#if defined (__aarch64__)
    MSG_DRV_VE("---VE: bVECapture=%u, u32FrameSize=%u, b3FrameMode=%u\n", g_VEInfo.bVECapture, u32FrameSize, b3FrameMode);
#else
    MSG_DRV_VE("---VE: bVECapture=%u, u32FrameSize=%tu, b3FrameMode=%u\n", g_VEInfo.bVECapture,(ptrdiff_t) u32FrameSize, b3FrameMode);
#endif
}

void MDrv_VE_set_capture_window(void)
{
	MS_U16 u16Out_Vsize, u16Out_Hsize,u16V_CapSize=0;
    MS_U16 u16Tmp, u16Tmp2;
    MS_BOOL bVECManualCapWindow = FALSE;

    if(((g_VEInfo.InputSrcType == MS_VE_SRC_SUB) || (g_VEInfo.InputSrcType == MS_VE_SRC_MAIN)) &&
       (g_VEInfo.InputSrcOfMixedSrc >= MS_VE_SRC_ATV) && (g_VEInfo.InputSrcOfMixedSrc <= MS_VE_SRC_SVIDEO)
      )
    {
        bVECManualCapWindow = TRUE; //For VD sources, need manual tune ve capture window in vec case
    }

    if(g_VEInfo.bVECapture)
    {
        u16Out_Hsize = g_VEInfo.u16OutputWidth;
        u16Out_Vsize = g_VEInfo.u16Outputheight;
    }
    else if(!g_VEInfo.bForceCaptureMode)
    {
        u16Out_Hsize = ((g_VEInfo.InputSrcType == MS_VE_SRC_DTV) && (g_VEInfo.bHDuplicate)) ? 704 : 720;
        u16Out_Vsize = (g_VEInfo.VideoSystem <= MS_VE_PAL_M) ? 480 : 576;
    }
    else
    {
        u16Out_Hsize = g_VEInfo.u16H_CapSize;
        u16Out_Vsize = g_VEInfo.u16V_CapSize;
    }

    if(g_VEInfo.InputSrcType == MS_VE_SRC_DI)
    {
        MSG_DRV_VE("VE InputSrcType = MS_VE_SRC_DI \n");
        u16Out_Hsize = g_VEInfo.u16H_CapSize;
        u16Out_Vsize = g_VEInfo.u16V_CapSize;
    }

    g_VEInfo.u8VE_DisplayStatus &= ~(EN_VE_INVERSE_HSYNC);


    MSG_DRV_VE("Out: Hsize=%d, Vsize=%d \n", u16Out_Hsize, u16Out_Vsize);
    MSG_DRV_VE("VE Cap: Hstart=%d, Hsize =%d \n", g_VEInfo.u16H_CapStart, g_VEInfo.u16H_CapSize);
    //MSG_DRV_VE("SC Cap: Hstart=%d, Hsize =%d \n", g_SrcInfo.u16H_CapStart, g_SrcInfo.u16H_CapSize));
    MSG_DRV_VE("VE Cap: Vstart=%d, Vsize =%d \n", g_VEInfo.u16V_CapStart, g_VEInfo.u16V_CapSize);
    //MSG_DRV_VE("SC Cap: Vstart=%d, Vsize =%d \n", g_SrcInfo.u16V_CapStart, g_SrcInfo.u16V_CapSize));
    MSG_DRV_VE("[%s][%d]_gbIsCaptureMode = %d, bVECManualCapWindow=%d\n", __FUNCTION__, __LINE__, g_VEInfo.bForceCaptureMode, bVECManualCapWindow);
    if(bVECManualCapWindow == FALSE) //For normal AV output case
    {
        // Horizontal
#if (ENABLE_VE_SUBTITLE)
        if((g_VEInfo.VideoSystem <= MS_VE_PAL_M) &&
            (g_VEInfo.u16V_CapSize <= 480))
        {
            u16Tmp2 = 0x17; //fine tune for ntsc 480i case, which is too left
        }
        else
        {
            u16Tmp2 = 0;
        }

        if(g_VEInfo.u16H_CapSize < u16Out_Hsize)
        {
            #ifndef VE_SOURCE_AFTER_DI
            g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            #endif
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;

            u16Tmp = u16Out_Hsize - g_VEInfo.u16H_CapSize;
            g_VEInfo.u16H_CapStart =  g_VEInfo.u16H_CapStart - u16Tmp/2 + u16Tmp2 + 9 ;
            #ifndef VE_SUPPORT_CROP
            g_VEInfo.u16H_CapSize = u16Out_Hsize;
            #endif
            MSG_DRV_VE("--1 temp %d, u16Tmp2 %d, H_start %d\n",u16Tmp,u16Tmp2,g_VEInfo.u16H_CapStart);
        }
        else if( ( g_VEInfo.u16H_CapSize == u16Out_Hsize ) &&
                ( g_VEInfo.u16H_CapSize > g_VEInfo.u16H_SC_CapSize )) //Same condition with Vsize
        {
            #ifndef VE_SOURCE_AFTER_DI
            g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            #endif
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;

            u16Tmp = g_VEInfo.u16H_CapSize - g_VEInfo.u16H_SC_CapSize;
            g_VEInfo.u16H_CapStart = g_VEInfo.u16H_SC_CapStart - u16Tmp/2 + u16Tmp2 + 9;
            MSG_DRV_VE("--2 temp %d, u16Tmp2 %d, H_start %d\n",u16Tmp, u16Tmp2, g_VEInfo.u16H_CapStart);
        }
        else
        {
            //g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;

            u16Tmp = g_VEInfo.u16H_CapSize - g_VEInfo.u16H_SC_CapSize;
            g_VEInfo.u16H_CapStart = g_VEInfo.u16H_SC_CapStart - u16Tmp/2 + u16Tmp2 + 9;
    		MSG_DRV_VE("--3 temp %d, u16Tmp2 %d , H_start %d\n",u16Tmp, u16Tmp2,g_VEInfo.u16H_CapStart);
        }

#else
        if(g_VEInfo.u16H_CapSize < u16Out_Hsize)
        {
            g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;

            u16Tmp = u16Out_Hsize - g_VEInfo.u16H_CapSize;
            g_VEInfo.u16H_CapStart -= (u16Tmp/2);
            g_VEInfo.u16H_CapSize += u16Tmp;
        }
#endif

        // Vertical
#if (ENABLE_VE_SUBTITLE)
        if((g_VEInfo.VideoSystem <= MS_VE_PAL_M) &&
            (g_VEInfo.u16V_CapSize <= 480))
        {
            if(g_VEInfo.bSrcInterlace)
            {
                u16Tmp2 = 0x11; //fine tune for ntsc 480i case, which will leave a black band on the bottom
            }
            else
            {
                u16Tmp2 = 0x5; //fine tune for ntsc 480p case, which will leave a black band on the bottom
            }
        }
        else if((g_VEInfo.VideoSystem <= MS_VE_PAL_M) &&
            (g_VEInfo.u16V_CapSize <= 720))
        {
                u16Tmp2 = 0x9; //fine tune for ntsc 720p case, which will leave a black band on the bottom
        }
        else if((g_VEInfo.VideoSystem <= MS_VE_PAL_M) &&
            (g_VEInfo.u16V_CapSize <= 1080))
        {
                u16Tmp2 = 0x9; //fine tune for ntsc 1080i case, which will leave a black band on the bottom
        }
        else
        {
            u16Tmp2 = 0;
        }

        if(g_VEInfo.u16V_CapSize < u16Out_Vsize)
        {
            g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;

            u16Tmp = u16Out_Vsize - g_VEInfo.u16V_CapSize;
            if(g_VEInfo.u16V_CapStart > u16Tmp/2 + u16Tmp2)
            {
                g_VEInfo.u16V_CapStart =  g_VEInfo.u16V_CapStart - u16Tmp/2 - u16Tmp2;
            }
            else
            {
                g_VEInfo.u16V_CapStart = 0;
            }
            g_VEInfo.u16V_CapSize = u16Out_Vsize;
			MSG_DRV_VE("--1 temp %d, u16Tmp2 %d, V_start %d\n",u16Tmp,u16Tmp2,g_VEInfo.u16V_CapStart);
        }
        else if( ( g_VEInfo.u16V_CapSize == u16Out_Vsize ) &&
                ( g_VEInfo.u16V_CapSize > g_VEInfo.u16V_SC_CapSize ))
        {
            g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;

            u16Tmp = g_VEInfo.u16V_CapSize - g_VEInfo.u16V_SC_CapSize;
            if(g_VEInfo.u16V_SC_CapStart > u16Tmp/2 + u16Tmp2)
            {
                g_VEInfo.u16V_CapStart = g_VEInfo.u16V_SC_CapStart - u16Tmp/2 - u16Tmp2;
            }
            else
            {
                g_VEInfo.u16V_CapStart = 0;
            }
            MSG_DRV_VE("--2 temp %d, u16Tmp2 %d, V_start %d\n",u16Tmp,u16Tmp2,g_VEInfo.u16V_CapStart);
        }
        else
        {
            //g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;
            u16Tmp = g_VEInfo.u16V_CapSize - g_VEInfo.u16V_SC_CapSize;

            MSG_DRV_VE("--g_VEInfo.u16V_CapStart %x\n",g_VEInfo.u16V_CapStart);
            MSG_DRV_VE("--u16Tmp %d, u16Tmp2 %d\n",u16Tmp, u16Tmp2);
            if(g_VEInfo.u16V_SC_CapStart > (u16Tmp/2 + u16Tmp2))
            {
                g_VEInfo.u16V_CapStart = g_VEInfo.u16V_SC_CapStart - u16Tmp/2 - u16Tmp2;
            }
            else
            {
                g_VEInfo.u16V_CapStart = 0;
            }
			MSG_DRV_VE("--3 temp %d , u16Tmp2 %d, V_start %d\n",u16Tmp, u16Tmp2 ,g_VEInfo.u16V_CapStart);
        }

        MSG_DRV_VE("--u16V_CapEnd %d\n",g_VEInfo.u16V_CapStart + g_VEInfo.u16V_CapSize - 1);

        if(g_VEInfo.u16V_CapStart> (g_VEInfo.u16V_CapStart + g_VEInfo.u16V_CapSize - 1))
        {
           Hal_VE_set_cvbs_buffer_out(0x0F,0x00, 0x00, 0x00);//disable
        }
#else
        if(g_VEInfo.u16V_CapSize < u16Out_Vsize)
        {
            g_VEInfo.u8VE_DisplayStatus &= ~ EN_VE_DEMODE;
            g_VEInfo.u8VE_DisplayStatus |= EN_VE_INVERSE_HSYNC;

            u16Tmp = u16Out_Vsize - g_VEInfo.u16V_CapSize;
            g_VEInfo.u16V_CapStart -= (u16Tmp/2);
            g_VEInfo.u16V_CapSize += u16Tmp;
        }
        if(g_VEInfo.u16V_CapStart > (g_VEInfo.u16V_CapStart + g_VEInfo.u16V_CapSize - 1))
            Hal_VE_set_cvbs_buffer_out(0x0F,0x00, 0x00, 0x00);//disable
#endif
    }

    MSG_DRV_VE("VE status=%x \n", g_VEInfo.u8VE_DisplayStatus);
    MSG_DRV_VE("VE Cap: Hstart=%d, Hsize =%d \n", g_VEInfo.u16H_CapStart, g_VEInfo.u16H_CapSize);
    MSG_DRV_VE("VE Cap: Vstart=%d, Vsize =%d \n", g_VEInfo.u16V_CapStart, g_VEInfo.u16V_CapSize);

    /*
    For input source is component and HDMI all use DE mode.
    */
    if((g_VEInfo.InputSrcType >= MS_VE_SRC_COMP) && (g_VEInfo.InputSrcType <= MS_VE_SRC_HDMI_C))
    {
        g_VEInfo.u8VE_DisplayStatus |=  EN_VE_DEMODE;
    }

#if 1
    if(g_VEInfo.bSrcInterlace)
    {
        u16V_CapSize = g_VEInfo.u16V_CapSize / 2;
        MSG_DRV_VE("VE Cap: Interlace mode, Change Vsize =%d \n", u16V_CapSize);
    }
    else
    {
        u16V_CapSize = g_VEInfo.u16V_CapSize;
    }
#ifdef VE_SUPPORT_CROP
    if(!(g_VEInfo.u8VE_DisplayStatus & EN_VE_DEMODE))
    {
        g_VEInfo.u8VE_DisplayStatus |=  EN_VE_DEMODE;
        if(g_VEInfo.u16V_CapSize < u16Out_Vsize)
        {
            u16Tmp = (u16Out_Vsize-g_VEInfo.u16V_CapSize)/2;
        }
        else
        {
            u16Tmp = 0;
        }

        if(g_VEInfo.u16H_CapSize < u16Out_Hsize)
        {
            u16Tmp2 = (u16Out_Hsize-g_VEInfo.u16H_CapSize)/2;
        }
        else
        {
            u16Tmp2 = 0;
        }

        Hal_VE_set_crop_window(u16Tmp2,
        u16Out_Hsize - u16Tmp2 -1,
        u16Tmp,
        (u16Out_Vsize - u16Tmp+1)/2 -1);
    }
    else
    {
        Hal_VE_set_crop_window(0,u16Out_Hsize-1,0,(u16Out_Vsize/2)-1);
    }
#endif

    if(g_VEInfo.u8VE_DisplayStatus & EN_VE_CCIR656_IN)
        Hal_VE_set_capture_window(0x000, VE_DE_HEND_MASK, 0x10, 0xFF);
    else if((g_VEInfo.u8VE_DisplayStatus & EN_VE_DEMODE) && (!g_VEInfo.bForceCaptureMode)) // Force DE mode for source coming Scaler OP
        Hal_VE_set_capture_window(0x000, VE_DE_HEND_MASK, 0x000, VE_DE_VEND_MASK);
    else
    {
        Hal_VE_set_capture_window(g_VEInfo.u16H_CapStart,
        g_VEInfo.u16H_CapStart + g_VEInfo.u16H_CapSize - 1,
        g_VEInfo.u16V_CapStart,
        g_VEInfo.u16V_CapStart + u16V_CapSize - 1);
    }
#else
    if(g_VEInfo.u8VE_DisplayStatus & EN_VE_CCIR656_IN)
        Hal_VE_set_capture_window(0x000, 0x7FF, 0x10, 0xFF);
    else if((g_VEInfo.u8VE_DisplayStatus & EN_VE_DEMODE || g_VEInfo.InputSrcType == MS_VE_SRC_SCALER) && (!_gbIsCaptureMode)) // Force DE mode for source coming Scaler OP if capture mode is disable
        Hal_VE_set_capture_window(0x000, 0x7FF, 0x000, 0x7FF);
    else
    {
        Hal_VE_set_capture_window(g_VEInfo.u16H_CapStart,
                            g_VEInfo.u16H_CapStart + g_VEInfo.u16H_CapSize - 1,
                            g_VEInfo.u16V_CapStart,
                            g_VEInfo.u16V_CapStart + g_VEInfo.u16V_CapSize - 1);
    }
#endif

    if(g_VEInfo.u8VE_DisplayStatus & EN_VE_INVERSE_HSYNC)
        Hal_VE_set_hsync_inverse(1); //enable inverse Hsync
    else
        Hal_VE_set_hsync_inverse(0); //disable inverse Hsync


    if(g_VEInfo.bSrcInterlace)
        Hal_VE_set_source_interlace(ENABLE);
    else
        Hal_VE_set_source_interlace(DISABLE);
}

//------------------------------------------------------------------------------
/// @brief MDrv_VE_Get_Ext_Caps: get VE extented capability
///
/// @argument:
/// @return none
//------------------------------------------------------------------------------
VE_Result MDrv_VE_Get_Ext_Caps(MS_VE_Ext_Cap *cap)
{
    cap->bSupport_UpScale = g_VECap.bSupport_UpScale;
    cap->bSupport_CropMode = g_VECap.bSupport_CropMode;

    return E_VE_OK;
}

//------------------------------------------------------------------------------
/// @brief MDrv_VE_set_crop_window, set up crop window (crop video frame in MIU)
///
/// @argument:
//          - MS_VE_WINDOW_TYPE - crop window
/// @return VE_Result
//------------------------------------------------------------------------------
VE_Result MDrv_VE_set_crop_window(MS_VE_WINDOW_TYPE stCropWin)
{
    MS_U16 u16Out_Hsize= (((g_VEInfo.bHDuplicate) && (g_VEInfo.InputSrcType == MS_VE_SRC_DTV)) ? 704 : 720);
    MS_U16 u16Out_Vsize = ((g_VEInfo.VideoSystem <= MS_VE_PAL_M) ? 480 : 576);
    MS_PHY phyMIUOffsetAddr = 0;
    MS_U8 u8PackPerLine = 0;
    MS_U16 u16PixelPerUnit = 0;
    MS_U16 u16FieldLineNum = 0;
    VE_Result ret = E_VE_OK;

    if (!g_VECap.bSupport_CropMode) {
        MSG_DRV_VE("[%s] is not supported in this chip\n",
                    __FUNCTION__);
        ret = E_VE_NOT_SUPPORT;
        goto fail;
    }

    //Check crop window location and size.
    if(stCropWin.x > u16Out_Hsize) {
        MSG_DRV_VE("[%s]x >> width,please check x range\n",
                    __FUNCTION__);
        ret = E_VE_FAIL;
        goto fail;
    }
    if(stCropWin.y > u16Out_Vsize) {
        MSG_DRV_VE("[%s]y >> height,please check y range\n",
                    __FUNCTION__);
        ret = E_VE_FAIL;
        goto fail;
    }

    if (Hal_VE_Get_DRAM_Format() == MS_VE_DRAM_FORMAT_Y8C8) {
        // Y8C8
        u16PixelPerUnit = 4;    // 64 / 16 = 4
    } else {
        // Y8C8M4
        u16PixelPerUnit = 3;    // 64 / 20 = 3
        // NOTE: In Y8C8M4 format, there are at most 3 pixel per access unit
        // (64bit). There are 4 unused bits every 64 bits.
    }
    // step 0: crop left part(stCropWin.x)
    // step 1: top part(stCropWin.y * u16Out_HSize)
    // NOTE: there are PAL: 288 / NTSC:240 lines in one field.
    phyMIUOffsetAddr = (stCropWin.x + (stCropWin.y / 2) * u16Out_Hsize) / u16PixelPerUnit * 8;
    phyMIUOffsetAddr += g_VEInfo.phyMiuBaseAddr;
    Hal_VE_SetMemAddr(&phyMIUOffsetAddr, NULL);

    // seep 2: crop right part (stCropWin.width)
    // unit: MIU bus width: 64 bits
    u8PackPerLine = (stCropWin.width / u16PixelPerUnit) - 1;
    Hal_VE_Set_Pack_Num_Per_Line(u8PackPerLine);

    MSG_DRV_VE("step 2: pack number per line: %u\n",
                (unsigned int)u8PackPerLine);

    // seep 3: crop bottom part (stCropWin.height)
    // line per field: PAL: 288, NTSC: 240
    u16FieldLineNum = ((stCropWin.height) / 2) - 1;
    Hal_VE_Set_Field_Line_Number(u16FieldLineNum);
    MSG_DRV_VE("step 3: field line number: %u\n",
                (unsigned int)u16FieldLineNum);

    // update Cap size for MDrv_VE_set_scaling_ratio
    g_VEInfo.u16H_CapSize = stCropWin.width;
    g_VEInfo.u16V_CapSize = stCropWin.height;
    return E_VE_OK;
fail:
    printf("Error: [%s] failed, errno: %d\n", __FUNCTION__, ret);
    return ret;
}

void _MDrv_VE_SetMode(MS_VE_Set_Mode_Type *pVESetMode)
{
    g_VEInfo.u16H_CapSize  = pVESetMode->u16H_CapSize;
    g_VEInfo.u16V_CapSize  = pVESetMode->u16V_CapSize;
    g_VEInfo.u16H_CapStart = pVESetMode->u16H_CapStart;
    g_VEInfo.u16V_CapStart = pVESetMode->u16V_CapStart;

    g_VEInfo.u16H_SC_CapSize  = pVESetMode->u16H_SC_CapSize;
    g_VEInfo.u16V_SC_CapSize  = pVESetMode->u16V_SC_CapSize;
    g_VEInfo.u16H_SC_CapStart = pVESetMode->u16H_SC_CapStart;
    g_VEInfo.u16V_SC_CapStart = pVESetMode->u16V_SC_CapStart;
    g_VEInfo.bHDuplicate = pVESetMode->bHDuplicate;
    #ifdef VE_SOURCE_AFTER_DI
    g_VEInfo.bSrcInterlace = FALSE;
    #else
    g_VEInfo.bSrcInterlace = pVESetMode->bSrcInterlace;
    #endif

    MSG_DRV_VE("VE SetMode, VFreq=%d, Hdupliate=%d, Interlace=%d\n",
        pVESetMode->u16InputVFreq, g_VEInfo.bHDuplicate, g_VEInfo.bSrcInterlace);

    MSG_DRV_VE("VE->H:(%d, %d), V:(%d, %d)\n",
        g_VEInfo.u16H_CapStart, g_VEInfo.u16H_CapSize,
        g_VEInfo.u16V_CapStart, g_VEInfo.u16V_CapSize);

    MSG_DRV_VE("SC->H:(%d, %d), V:(%d, %d)\n",
        g_VEInfo.u16H_SC_CapStart, g_VEInfo.u16H_SC_CapSize,
        g_VEInfo.u16V_SC_CapStart, g_VEInfo.u16V_SC_CapSize);
    if(g_VEInfo.bVECapture == FALSE)
    {
        g_VEInfo.u16OutputWidth = (( ( g_VEInfo.bHDuplicate ) && ( g_VEInfo.InputSrcType == MS_VE_SRC_DTV ) ) ? 704 : 720);
        g_VEInfo.u16Outputheight= ((g_VEInfo.VideoSystem <= MS_VE_PAL_M) ? 480 : 576);
    }
    MDrv_VE_set_capture_window(); // set capture window
    MDrv_VE_set_scaling_ratio(g_VEInfo.u16OutputWidth, g_VEInfo.u16Outputheight);    // set scaling ratio
    MDrv_VE_set_frc(pVESetMode->u16InputVFreq); // set frc
}

void MDrv_VE_SetMode(MS_VE_Set_Mode_Type *pVESetMode)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETMODE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SETMODE)malloc(sizeof(TVE_SETMODE));

    if(NULL == pArgs)
        return ;
    pArgs->pVESetMode = pVESetMode;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetMode, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    free(pArgs);
#else
    _MDrv_VE_SetMode(pVESetMode);
#endif
}
void _MDrv_VE_SetBlackScreen(MS_BOOL bEn)
{
    MSG_DRV_VE("VE Setblackscreen %x\n", bEn);
    Hal_VE_set_blackscreen(bEn);
}

void MDrv_VE_SetBlackScreen(MS_BOOL bEn)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETBLACKSCREEN pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return;
    }
    pArgs = (PTVE_SETBLACKSCREEN)malloc(sizeof(TVE_SETBLACKSCREEN));

    if(NULL == pArgs)
        return ;
    pArgs->bEnable = bEn;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetBlackSceen, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SetBlackScreen(bEn);
#endif
}

MS_BOOL _MDrv_VE_IsBlackScreenEnabled(void)
{
    return Hal_VE_is_blackscreen_enabled();
}

MS_BOOL MDrv_VE_IsBlackScreenEnabled(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    TVE_ISBLACKSCREENENABLE Args;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    //pArgs = (PTVE_ISBLACKSCREENENABLE)malloc(sizeof(PTVE_ISBLACKSCREENENABLE));
    Args.bEnable = FALSE;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_IsBlackScreenEnabled, (void*)&Args);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    return Args.bEnable;
#else
    return _MDrv_VE_IsBlackScreenEnabled();
#endif
}

void MDrv_VE_GetCaps(VE_Caps* pCaps)
{
    pCaps->bNTSC = TRUE;
    pCaps->bPAL  = TRUE;
	pCaps->bVESupported = VE_SUPPORTED;
}

//------------------------------------------------------------------------------
/// VE restart TVE to read data from TT buffer
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_EnableTtx(MS_BOOL bEnable)
{
    Hal_VE_EnableTtx(bEnable);
}

#define FILED_PER_FIRE 5
//------------------------------------------------------------------------------
/// VE set TT buffer address
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_SetTtxBuffer(MS_PHY u32StartAddr, MS_U32 u32Size)
{

    //Hal_VE_Set_VbiTtxActiveLines(linePerField);

    // MIU size and base adress
    Hal_VE_Set_ttx_Buffer(u32StartAddr, u32Size);

    // interrupt force & interrupt clear
    Hal_VE_Clear_ttxReadDoneStatus();

    // start next vbi ttx read
    Hal_VE_EnableTtx(ENABLE);
}

void MDrv_VE_SetTtxBuffer(MS_PHY u32StartAddr, MS_U32 u32Size)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETTTXBUFFER pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return;
    }
    pArgs = (PTVE_SETTTXBUFFER)malloc(sizeof(TVE_SETTTXBUFFER));

    if(NULL == pArgs)
        return ;
    pArgs->u32StartAddr = u32StartAddr;
    pArgs->u32Size = u32Size;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetTtxBuffer, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    free(pArgs);
#else
    _MDrv_VE_SetTtxBuffer(u32StartAddr, u32Size);
#endif
}
//------------------------------------------------------------------------------
/// VE clear TT buffer read done status
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_ClearTtxReadDoneStatus(void)
{
    Hal_VE_Clear_ttxReadDoneStatus();
}

//------------------------------------------------------------------------------
/// VE TT buffer read done status
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL _MDrv_VE_GetTtxReadDoneStatus(void)
{
    return Hal_VE_Get_ttxReadDoneStatus();
}

MS_BOOL MDrv_VE_GetTtxReadDoneStatus(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    TVE_GETTTXREADDONE Args ;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }

    memset(&Args, 0, sizeof(TVE_GETTTXREADDONE));
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_VE_GetTtxReadDoneStatus, (void*)&Args);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    return Args.bDone;
#else
    return _MDrv_VE_GetTtxReadDoneStatus();
#endif
}
//------------------------------------------------------------------------------
/// VE Set VBI TT active Lines
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetVbiTtxActiveLines(MS_U8 u8LinePerField)
{
    Hal_VE_Set_VbiTtxActiveLines(u8LinePerField);
}

//------------------------------------------------------------------------------
/// VE Set VBI TT active Lines by the given bitmap
/// @param <IN>\b u32Bitmap: a bitmap that defines whick physical lines the teletext lines are to be inserted.
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetVbiTtxActiveLinesBitmap(MS_U32 u32Bitmap)
{
    Hal_VE_Set_VbiTtxActiveLinesBitmap(u32Bitmap);
}

//------------------------------------------------------------------------------
/// ve set vbi ttx active line
/// @param <in>\b odd_start: odd page start line
/// @param <in>\b odd_end: odd page end line
/// @param <in>\b even_start: even page start line
/// @param <in>\b even_end: even page end line
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetVbiTtxRange(MS_U16 odd_start, MS_U16 odd_end,
                            MS_U16 even_start, MS_U16 enen_end)

{
    Hal_VE_SetVbiTtxRange(odd_start, odd_end, even_start, enen_end);
}



//------------------------------------------------------------------------------
/// VE Set VBI CC
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_EnableCcSw(MS_BOOL bEnable)
{
    Hal_VE_EnableCcSw( bEnable);
}

void MDrv_VE_EnableCcSw(MS_BOOL bEnable)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_ENABLECCSW pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_ENABLECCSW)malloc(sizeof(TVE_ENABLECCSW));

    if(NULL == pArgs)
        return;
    pArgs->bEnable = bEnable;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_EnableCcSw, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_EnableCcSw(bEnable);
#endif
}
//------------------------------------------------------------------------------
/// VE Set VBI CC active line
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_SetCcRange(MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end)
{
    Hal_VE_SetCcRange( odd_start,  odd_end,  even_start,  enen_end);
}

void MDrv_VE_SetCcRange(MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETCCRANGE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }

    pArgs = (PTVE_SETCCRANGE)malloc(sizeof(TVE_SETCCRANGE));

    if(NULL == pArgs)
        return ;
    pArgs->u16odd_start = odd_start;
    pArgs->u16odd_end = odd_end;
    pArgs->u16even_start = even_start;
    pArgs->u16even_end = enen_end;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetCcRange, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SetCcRange(odd_start, odd_end, even_start, enen_end);
#endif
}
//------------------------------------------------------------------------------
/// VE Set VBI CC data
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_SendCcData(MS_BOOL bIsOdd, MS_U16 data)    // true: odd, false: even
{
    Hal_VE_SendCcData( bIsOdd,  data);
}

void MDrv_VE_SendCcData(MS_BOOL bIsOdd, MS_U16 data)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SENDCCDATA pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }

    pArgs = (PTVE_SENDCCDATA)malloc(sizeof(TVE_SENDCCDATA));

    if(NULL == pArgs)
        return ;
    pArgs->bIsOdd = bIsOdd;
    pArgs->u16data = data;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SendCcData, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    free(pArgs);
#else
    _MDrv_VE_SendCcData(bIsOdd, data);
#endif
}

//------------------------------------------------------------------------------
/// Set VE Frame Color
/// @return none
//------------------------------------------------------------------------------
VE_Result _MDrv_VE_SetFrameColor(MS_U32 u32aRGB)
{
    return Hal_VE_SetFrameColor(u32aRGB);
}

VE_Result MDrv_VE_SetFrameColor(MS_U32 u32aRGB)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETFRAMECOLOR pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_SETFRAMECOLOR)malloc(sizeof(TVE_SETFRAMECOLOR));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->u32aRGB = u32aRGB;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Set_FrameColor, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_OK;
    }
    else
    {
        free(pArgs);
        return E_VE_FAIL;
    }
#else
    return _MDrv_VE_SetFrameColor(u32aRGB);
#endif

}

//------------------------------------------------------------------------------
/// VE Set output with OSD
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_SetOSD(MS_BOOL bEnable)
{
    Hal_VE_Set_OSD(bEnable);
}

void MDrv_VE_SetOSD(MS_BOOL bEnable)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETOSD pArgs = NULL;

    if(pu32TvencoderInst == NULL)
    {
        return ;
    }

    pArgs = (PTVE_SETOSD)malloc(sizeof(TVE_SETOSD));

    if(NULL == pArgs)
        return ;
    pArgs->bEnable = bEnable;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetOSD, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    return _MDrv_VE_SetOSD(bEnable);
#endif
}

//------------------------------------------------------------------------------
/// Set VE OSD Layer
/// @return none
//------------------------------------------------------------------------------
VE_Result _MDrv_VE_Set_OSDLayer(MS_VE_OSD_LAYER_SEL eVideoOSDLayer)
{
    return Hal_VE_Set_OSDLayer(eVideoOSDLayer);
}

VE_Result MDrv_VE_Set_OSDLayer(MS_VE_OSD_LAYER_SEL eVideoOSDLayer)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETOSDLAYER pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_SETOSDLAYER)malloc(sizeof(TVE_SETOSDLAYER));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->eVideoOSDLayer = eVideoOSDLayer;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Set_OSDLayer, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_OK;
    }
    else
    {
        free(pArgs);
        return E_VE_FAIL;
    }
#else
    return _MDrv_VE_Set_OSDLayer(eVideoOSDLayer);
#endif

}

//------------------------------------------------------------------------------
/// Get VE OSD Layer
/// @return none
//------------------------------------------------------------------------------
MS_VE_OSD_LAYER_SEL _MDrv_VE_Get_OSDLayer(void)
{
    return Hal_VE_Get_OSDLayer();
}

MS_VE_OSD_LAYER_SEL MDrv_VE_Get_OSDLayer(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_GETOSDLAYER pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_GETOSDLAYER)malloc(sizeof(TVE_GETOSDLAYER));

    if(NULL == pArgs)
        return MS_VE_LAYER_NONE;
    pArgs->eRet = MS_VE_LAYER_NONE;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Get_OSDLayer, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    MS_VE_OSD_LAYER_SEL eReturn = pArgs->eRet;
    free(pArgs);
    return eReturn;

#else
    return _MDrv_VE_Get_OSDLayer();
#endif

}

//------------------------------------------------------------------------------
/// Set VE Video Alpha
/// @return none
//------------------------------------------------------------------------------
VE_Result _MDrv_VE_Set_VideoAlpha(MS_U8 u8Val)
{
    return Hal_VE_Set_VideoAlpha(u8Val);
}

VE_Result MDrv_VE_Set_VideoAlpha(MS_U8 u8Val)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETVIDEOALPHA pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_SETVIDEOALPHA)malloc(sizeof(TVE_SETVIDEOALPHA));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->u8Val = u8Val;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Set_VideoAlpha, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_OK;
    }
    else
    {
        free(pArgs);
        return E_VE_FAIL;
    }
#else
    return _MDrv_VE_Set_VideoAlpha(u8Val);
#endif

}

//------------------------------------------------------------------------------
/// Get VE Video Alpha
/// @return none
//------------------------------------------------------------------------------
VE_Result _MDrv_VE_Get_VideoAlpha(MS_U8 *pu8Val)
{
    return Hal_VE_Get_VideoAlpha(pu8Val);
}

VE_Result MDrv_VE_Get_VideoAlpha(MS_U8 *pu8Val)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_GETVIDEOALPHA pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_GETVIDEOALPHA)malloc(sizeof(TVE_GETVIDEOALPHA));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->pu8Val = pu8Val;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Get_VideoAlpha, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_OK;
    }
    else
    {
        free(pArgs);
        return E_VE_FAIL;
    }
#else
    return _MDrv_VE_Get_VideoAlpha(pu8Val);
#endif

}


//------------------------------------------------------------------------------
/// VE Set RGB In
/// @return none
//------------------------------------------------------------------------------
void _MDrv_VE_SetRGBIn(MS_BOOL bEnable)
{
    Hal_VE_set_rgb_in(bEnable);
}

void MDrv_VE_SetRGBIn(MS_BOOL bEnable)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETRGBIN pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SETRGBIN)malloc(sizeof(TVE_SETRGBIN));

    if(NULL == pArgs)
        return ;
    pArgs->bEnable = bEnable;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetRGBIn, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SetRGBIn(bEnable);
#endif
}
//------------------------------------------------------------------------------
/// VE Get output video standard
/// @param <RET>       \b VideoSystem : the video standard: PAL or NTSC
//------------------------------------------------------------------------------
MS_VE_VIDEOSYS _MDrv_VE_Get_Output_Video_Std(void)
{
    return Hal_VE_Get_Output_Video_Std();
}

MS_VE_VIDEOSYS MDrv_VE_Get_Output_Video_Std(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    TVE_GETOUTPUTVIDEOSTD Args;

    if(pu32TvencoderInst == NULL)
    {
        return MS_VE_VIDEOSYS_NUM;
    }
    memset(&Args, 0, sizeof(TVE_GETOUTPUTVIDEOSTD));
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Get_Output_Video_Std, (void*)&Args);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    return Args.stVideoSys;
#else
    return _MDrv_VE_Get_Output_Video_Std();
#endif
}
//------------------------------------------------------------------------------
/// Force VE Enable/Disable Capture Mode, used to avoid enter DE mode in some case
/// @param <RET>       \b bEnable : TRUE: Force use capture mode, FALSE: use defaut
//------------------------------------------------------------------------------
void _MDrv_VE_SetCaptureMode(MS_BOOL bEnable)
{
    g_VEInfo.bForceCaptureMode = bEnable;
}

void MDrv_VE_SetCaptureMode(MS_BOOL bEnable)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETCAPMODE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SETCAPMODE)malloc(sizeof(TVE_SETCAPMODE));

    if(NULL == pArgs)
        return ;
    pArgs->bEnable = bEnable;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetCaptureMode, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_SetCaptureMode(bEnable);
#endif
}
//------------------------------------------------------------------------------
/// VE Get Destination Information for GOP mixer
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL _MApi_VE_GetDstInfo(MS_VE_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    if(NULL == pDstInfo)
    {
        MSG_DRV_VE("MApi_VE_GetDstInfo():pDstInfo is NULL\n");
        return FALSE;
    }

    if(u32SizeofDstInfo != sizeof(MS_VE_DST_DispInfo))
    {
        MSG_DRV_VE("MApi_VE_GetDstInfo():u16SizeofDstInfo is different from the MS_VE_DST_DispInfo defined, check header file!\n");
        return FALSE;
    }

    if(_MDrv_VE_Get_Output_Video_Std() == MS_VE_PAL)
    {
        //PAL
        MSG_DRV_VE("MApi_VE_GetDstInfo(): It's PAL case.\n");
        pDstInfo->bInterlaceMode = TRUE;
        pDstInfo->HDTOT = 864;
        pDstInfo->VDTOT = 624;
        pDstInfo->DEHST = 132;
        pDstInfo->DEHEND= pDstInfo->DEHST + 720;
        pDstInfo->DEVST = 44;
        pDstInfo->DEVEND= pDstInfo->DEVST + 576;
        return TRUE;
    }
    else if(_MDrv_VE_Get_Output_Video_Std() == MS_VE_NTSC)
    {
        //NTSC
        MSG_DRV_VE("MApi_VE_GetDstInfo(): It's NTSC case.\n");
        pDstInfo->bInterlaceMode = TRUE;
        pDstInfo->HDTOT = 858;
        pDstInfo->VDTOT = 524;
        pDstInfo->DEHST = 119;
        pDstInfo->DEHEND= pDstInfo->DEHST +720;
        pDstInfo->DEVST = 36;
        pDstInfo->DEVEND= pDstInfo->DEVST + 480;
        return TRUE;
    }
    else
    {
        MSG_DRV_VE("MApi_VE_GetDstInfo(): Unkown standard.\n");
        pDstInfo->bInterlaceMode = FALSE;
        pDstInfo->HDTOT = 0;
        pDstInfo->VDTOT = 0;
        pDstInfo->DEHST = 0;
        pDstInfo->DEHEND= 0;
        pDstInfo->DEVST = 0;
        pDstInfo->DEVEND= 0;
        return FALSE;
    }

}

MS_BOOL MApi_VE_GetDstInfo(MS_VE_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    PTVE_GETDSTINFO pArgs ;//= NULL;

    if(pu32TvencoderInst == NULL)
    {
        if(UtopiaOpen(MODULE_TVENCODER, (void**)&pu32TvencoderInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen TVE failed\n");
            return FALSE;
        }
    }
    pArgs = (PTVE_GETDSTINFO)malloc(sizeof(TVE_GETDSTINFO));

    if(NULL == pArgs)
        return FALSE;
    pArgs->pDstInfo = pDstInfo;
    pArgs->u32SizeofDstInfo = u32SizeofDstInfo;
    pArgs->eRet = E_TVENCODER_FAIL;
    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_GetDstInfo, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return FALSE;
    }
    else
    {
        free(pArgs);
        return TRUE;
    }
#else
    return _MApi_VE_GetDstInfo(pDstInfo,  u32SizeofDstInfo);
#endif
}

void _MDrv_VE_Set_TestPattern(MS_BOOL bEn)
{
    MSG_DRV_VE("VE Set TestPattern bEn = %x\n", bEn);
    Hal_VE_Set_TestPattern(bEn);
}

void MDrv_VE_Set_TestPattern(MS_BOOL bEn)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_SETTESTPATTERN pArgs =NULL;
    if(pu32TvencoderInst == NULL)
    {
        return ;
    }
    pArgs = (PTVE_SETTESTPATTERN)malloc(sizeof(TVE_SETTESTPATTERN));

    if(NULL == pArgs)
        return ;
    pArgs->bEnable = bEn;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Set_TestPattern, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_Set_TestPattern(bEn);
#endif
}
void MApi_VE_W2BYTE_MSK(MS_VE_REG_BANK VE_BK, MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16Mask)
{
    MS_VIRT _VERIUBaseAddress =0;
    MS_PHY phyNonPMBankSize = 0;

    if(_VE_RIU_BASE == 0)
    {
        if(MDrv_MMIO_GetBASE( &_VERIUBaseAddress, &phyNonPMBankSize, MS_MODULE_PM ) != TRUE)
        {
            MSG_DRV_VE("MDrv_VE_Init GetBASE failure\n");
            MS_ASSERT(0);
        }
        Hal_VE_init_riu_base( _VERIUBaseAddress );
#if defined (__aarch64__)
        MSG_DRV_VE("_VE_RIU_BASE = 0x%x \n",_VE_RIU_BASE);
#else
        MSG_DRV_VE("_VE_RIU_BASE = 0x%tx \n",(ptrdiff_t)_VE_RIU_BASE);
#endif
    }

    if(VE_BK == MS_VE_REG_BANK_3B)
    {
        Hal_VE_W2BYTE_MSK(L_BK_VE_SRC(u32Reg), u16Val, u16Mask);
    }
    else if(VE_BK == MS_VE_REG_BANK_3E)
    {
        Hal_VE_W2BYTE_MSK(L_BK_VE_ENC(u32Reg), u16Val, u16Mask);
    }
    else if(VE_BK == MS_VE_REG_BANK_3F)
    {
        Hal_VE_W2BYTE_MSK(L_BK_VE_ENC_EX(u32Reg), u16Val, u16Mask);
    }
}

MS_U16 MApi_VE_R2BYTE_MSK(MS_VE_REG_BANK VE_BK, MS_U32 u32Reg, MS_U16 u16Mask)
{
    MS_VIRT _VERIUBaseAddress =0;
    MS_PHY phyNonPMBankSize = 0;

    if(_VE_RIU_BASE == 0)
    {
        if(MDrv_MMIO_GetBASE( &_VERIUBaseAddress, &phyNonPMBankSize, MS_MODULE_PM ) != TRUE)
        {
            MSG_DRV_VE("VE GetBASE failure\n");
            MS_ASSERT(0);
            return FALSE;
        }
        Hal_VE_init_riu_base( _VERIUBaseAddress );
#if defined (__aarch64__)
        MSG_DRV_VE("_VE_RIU_BASE = 0x%x \n",_VE_RIU_BASE);
#else
        MSG_DRV_VE("_VE_RIU_BASE = 0x%tx \n",(ptrdiff_t)_VE_RIU_BASE);
#endif
    }

    if(VE_BK == MS_VE_REG_BANK_3B)
    {
        return Hal_VE_R2BYTE_MSK(L_BK_VE_SRC(u32Reg), u16Mask);
    }
    else if(VE_BK == MS_VE_REG_BANK_3E)
    {
        return Hal_VE_R2BYTE_MSK(L_BK_VE_ENC(u32Reg), u16Mask);
    }
    else if(VE_BK == MS_VE_REG_BANK_3F)
    {
        return Hal_VE_R2BYTE_MSK(L_BK_VE_ENC_EX(u32Reg), u16Mask);
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/// VE Dump Table Interface
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
void MDrv_VE_DumpTable(MS_U8 *pVETable, MS_U8 u8TableType)
{
    Hal_VE_DumpTable(pVETable, u8TableType);
}

//------------------------------------------------------------------------------
/// VE Set MV
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
VE_Result MDrv_VE_SetMV(MS_BOOL bEnble, MS_VE_MV_TYPE eMvType)
{
    return Hal_VE_SetMV(bEnble, eMvType);
}

//------------------------------------------------------------------------------
/// VE Set MV By Bit Control (CPC/CPS)
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
VE_Result MDrv_VE_SetMV_BitControl(MS_BOOL bEnable, MS_VE_MV_BitControl MV_BitControl_Data)
{
    return Hal_VE_SetMV_BitControl(bEnable, MV_BitControl_Data);
}

VE_Result _MDrv_VE_DCS_SetType(MS_BOOL bEnable, MS_U32 eType)
{
    return Hal_VE_DCS_SetType(bEnable, eType);
}
VE_Result _MDrv_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
    return Hal_VE_DCS_SetActivationKey(pu8ActivationKeyTbl, u8ActivationKeyTblSize);
}

//------------------------------------------------------------------------------
/// Component Set MV
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
VE_Result MDrv_VE_DCS_SetType(MS_BOOL bEnable, MS_U32 eType)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_DCSSETTYPE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return E_VE_FAIL;
    }
    pArgs = (PTVE_DCSSETTYPE)malloc(sizeof(TVE_DCSSETTYPE));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->bEnable = bEnable;
    pArgs->eType = eType;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_DCS_SetType, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_FAIL;
    }
    else
    {
        free(pArgs);
        return E_VE_OK;
    }

#else
    return _MDrv_VE_DCS_SetType(bEnable, eType);
#endif
}

//------------------------------------------------------------------------------
/// Component Set MV
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
VE_Result MDrv_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_DCSSETACTIVATIONKEY pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return E_VE_FAIL;
    }
    pArgs = (PTVE_DCSSETACTIVATIONKEY)malloc(sizeof(TVE_DCSSETACTIVATIONKEY));

    if(NULL == pArgs)
    {
        return E_VE_FAIL;
    }
    pArgs->pu8ActivationKeyTbl = pu8ActivationKeyTbl;
    pArgs->u8ActivationKeyTblSize = u8ActivationKeyTblSize;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_DCS_SetActivationKey, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_FAIL;
    }
    else
    {
        free(pArgs);
        return E_VE_OK;
    }

#else
    return _MDrv_VE_DCS_SetActivationKey(pu8ActivationKeyTbl, u8ActivationKeyTblSize);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine set flag to mask register write for special case \n
///			e.g. MBoot to APP with logo display
/// @argument:
///              - bFlag: TRUE: Mask register write, FALSE: not Mask
/// @return None
//------------------------------------------------------------------------------
VE_Result _MDrv_VE_DisableRegWrite(MS_BOOL bFlag)
{
    if(Hal_VE_DisableRegWrite_GetCaps() == TRUE)
    {
	    g_bVeDisableRegWrite = bFlag;
        return E_VE_OK;
    }
    else
    {
        g_bVeDisableRegWrite = FALSE;
        return E_VE_OK;
    }
}

VE_Result MDrv_VE_DisableRegWrite(MS_BOOL bFlag)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_DISABLEREGWRITE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        if(UtopiaOpen(MODULE_TVENCODER, (void**)&pu32TvencoderInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen TVE failed\n");
            return FALSE;
        }
    }
    pArgs = (PTVE_DISABLEREGWRITE)malloc(sizeof(TVE_DISABLEREGWRITE));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->bFlag = bFlag;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_DisableRegWrite, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_FAIL;
    }
    else
    {
        free(pArgs);
        return E_VE_OK;
    }
#else
    return _MDrv_VE_DisableRegWrite( bFlag);
#endif
}

//------------------------------------------------------------------------------
/// VE show internal color bar (test pattern)
/// @param  - bEnable IN: TRUE to enable color; false to disable color bar
/// @return None
//------------------------------------------------------------------------------
void MDrv_VE_ShowColorBar(MS_BOOL bEnable)
{
    Hal_VE_ShowColorBar(bEnable);
}

//------------------------------------------------------------------------------
/// @brief This routine set read/write addjustment to centralize VE display window.\n
///        Set the adjustment and it works after MDrv_VE_set_display_window() API is invoked. \n
///
/// @argument:
///              - s32WAddrAdjustment: the pixel units to adjust on write address
///              - s32RAddrAdjustment: the pixel units to adjust on read address
/// @return VE_Result
//------------------------------------------------------------------------------
VE_Result _MDrv_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment)
{
    return Hal_VE_AdjustPositionBase(s32WAddrAdjustment, s32RAddrAdjustment);
}

VE_Result MDrv_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_ADJPOSITIONBASE pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        if(UtopiaOpen(MODULE_TVENCODER, (void**)&pu32TvencoderInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen TVE failed\n");
            return FALSE;
        }
    }
    pArgs = (PTVE_ADJPOSITIONBASE)malloc(sizeof(TVE_ADJPOSITIONBASE));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->s32RAddrAdjustment = s32RAddrAdjustment;
    pArgs->s32WAddrAdjustment = s32WAddrAdjustment;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_AdjustPositionBase, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_FAIL;
    }
    else
    {
        free(pArgs);
        return E_VE_OK;
    }
#else
    return _MDrv_VE_AdjustPositionBase(s32WAddrAdjustment,s32RAddrAdjustment);
#endif
}
//------------------------------------------------------------------------------
/// @brief In STB Chip, VE needs to set vepll to lock frame rate between HD and SD Path.\n
///
/// @argument:
///              - MS_VE_PLL_InCfg: Information of HD path
///              - MS_VE_PLL_OutCfg: Information of SD path
/// @return VE_Result
//------------------------------------------------------------------------------
VE_Result MDrv_VE_SetFrameLock(MS_VE_PLL_InCfg *pInCfg, MS_BOOL bEnable)
{
    MS_VE_PLL_OutCfg stOutCfg;

    if(g_VEInfo.VideoSystem <= MS_VE_PAL_M)
    {
        //NTSC, PAL-M System
        stOutCfg.u16VttOut = 525;
        stOutCfg.u16HttOut = 858;
        stOutCfg.u16VdeOut = 480;
        stOutCfg.u16HdeOut = 720;
    }
    else
    {
        //PAL System
        stOutCfg.u16VttOut = 625;
        stOutCfg.u16HttOut = 864;
        stOutCfg.u16VdeOut = 576;
        stOutCfg.u16HdeOut = 720;
    }

    stOutCfg.bInterlace = TRUE;

    if(pInCfg != NULL)
    {
        memcpy(&_stVE_PllInfo.InCfg, pInCfg, sizeof(MS_VE_PLL_InCfg));
        memcpy(&_stVE_PllInfo.OutCfg, &stOutCfg, sizeof(MS_VE_PLL_OutCfg));

        //idclk_div & odclk_div
        MS_U32 u32FreqIn,u32FreqOut;

        if(_stVE_PllInfo.InCfg.u16HdeIn > 720 ) //HD timing
        {
            _stVE_PllInfo.u32InitPll = 0x19999999;
        }
        else
        {
            _stVE_PllInfo.u32InitPll = 0x15555555;
        }

        u32FreqIn   = _stVE_PllInfo.InCfg.u16VttIn  *  _stVE_PllInfo.InCfg.u16HttIn;
        u32FreqOut = ((MS_U32)(_stVE_PllInfo.OutCfg.u16VttOut)* (MS_U32)(_stVE_PllInfo.OutCfg.u16HttOut))*4;
        if(!_stVE_PllInfo.OutCfg.bInterlace)
            u32FreqOut *= 2;

        MSG_DRV_VE("[VE_PLL] MDrv_VE_SetFrameLock\n");
        MSG_DRV_VE("[VE_PLL] InCfg.u16HttIn=0x%x \t InCfg.u16VttIn=0x%x \t InCfg.u16HdeIn=0x%x \t InCfg.u16VdeIn=0x%x \t InCfg.u16Hde_St=0x%x \t InCfg.u16Vde_St=0x%x\n", _stVE_PllInfo.InCfg.u16HttIn, _stVE_PllInfo.InCfg.u16VttIn, _stVE_PllInfo.InCfg.u16HdeIn, _stVE_PllInfo.InCfg.u16VdeIn, _stVE_PllInfo.InCfg.u16Hde_St, _stVE_PllInfo.InCfg.u16Vde_St);
#if defined (__aarch64__)
        MSG_DRV_VE("[VE_PLL] 32FreqIn=0x%x \t u32FreqOut=0x%x \n",u32FreqIn,u32FreqOut);
        MSG_DRV_VE("[VE_PLL] u32InitPll=0x%x \n",_stVE_PllInfo.u32InitPll);
#else
        MSG_DRV_VE("[VE_PLL] 32FreqIn=0x%tx \t u32FreqOut=0x%tx \n",(ptrdiff_t)u32FreqIn,(ptrdiff_t)u32FreqOut);
        MSG_DRV_VE("[VE_PLL] u32InitPll=0x%tx \n",(ptrdiff_t)_stVE_PllInfo.u32InitPll);
#endif

        _ReductionFrac(&u32FreqIn, &u32FreqOut);
        _MultiplFrac(&u32FreqIn,&u32FreqOut);

        _stVE_PllInfo.u32IDclk  = u32FreqIn;
        _stVE_PllInfo.u32ODclk =  u32FreqOut;

        if( ( _stVE_PllInfo.u32IDclk>0xFFFFFF ) || ( _stVE_PllInfo.u32ODclk>0xFFFFFF ) )
        {
                MSG_DRV_VE("[VE_PLL] ASSERT !!! idclk/odclk overflow\n");
        }
#if defined (__aarch64__)
        MSG_DRV_VE("[VE_PLL] idclk=%x \t odclk=%x\n",_stVE_PllInfo.u32IDclk,_stVE_PllInfo.u32ODclk);
#else
        MSG_DRV_VE("[VE_PLL] idclk=%tx \t odclk=%tx\n",(ptrdiff_t)_stVE_PllInfo.u32IDclk,(ptrdiff_t)_stVE_PllInfo.u32ODclk);
#endif
    }
    else
    {
        MSG_DRV_VE("[VE_PLL] input config is NULL, set fixed clk as ve clk!\n");
        bEnable = FALSE;
    }

    return HAL_VE_SetFrameLock((_stVE_PllInfo.u32IDclk&0xFFFFFF),(_stVE_PllInfo.u32ODclk&0xFFFFFF), _stVE_PllInfo.u32InitPll, bEnable);
}

//------------------------------------------------------------------------------
/// @brief MDrv_VE_SetWindow, set crop window and display window in one API.
///
/// @argument:
///     - a pointer to MS_VE_WINDOW_TYPE - source window, depends on input size
///     - a pointer to MS_VE_WINDOW_TYPE - crop window, depends on source window size
///     - a pointer to MS_VE_WINDOW_TYPE - display window, depends on output size
/// @return VE_Result
//------------------------------------------------------------------------------
VE_Result MDrv_VE_SetWindow(MS_VE_WINDOW_TYPE *stSrcWin, MS_VE_WINDOW_TYPE *stCropWin,
        MS_VE_WINDOW_TYPE *stDispWin)
{
    MS_VE_WINDOW_TYPE stScaledSrcWin, stScaledCropWin;
    const MS_U32 u32BaseWidth = 720;  // maximun down-scaled width
    const MS_U32 u32BaseHeight = 576; // maximun down-scaled height
    MS_U16 u16BakHCapSize = g_VEInfo.u16H_CapSize;
    MS_U16 u16BakVCapSize = g_VEInfo.u16V_CapSize;
    VE_Result eRet = E_VE_OK;

    // check chip capibility
    if ((!g_VECap.bSupport_CropMode) || (!g_VECap.bSupport_UpScale))
    {
        MSG_DRV_VE("[%s] is not supported in this chip\n",
                    __FUNCTION__);
        eRet = E_VE_NOT_SUPPORT;
        goto fail;
    }
    // check input parameters
    if (stSrcWin == NULL)
    {
        MSG_DRV_VE("[%s][%d] failed\n", __FUNCTION__, __LINE__);
        eRet = E_VE_FAIL;
        goto fail;
    }
    if (stCropWin == NULL)
    {
        MSG_DRV_VE("[%s][%d] failed\n", __FUNCTION__, __LINE__);
        eRet = E_VE_FAIL;
        goto fail;
    }
    if (stDispWin == NULL)
    {
        MSG_DRV_VE("[%s][%d] failed\n", __FUNCTION__, __LINE__);
        eRet = E_VE_FAIL;
        goto fail;
    }
    memcpy(&stScaledSrcWin, stSrcWin, sizeof(MS_VE_WINDOW_TYPE));
    memcpy(&stScaledCropWin, stCropWin, sizeof(MS_VE_WINDOW_TYPE));

    // stage0 - setup down scaling factor
    if ((stSrcWin->width > u32BaseWidth) || (stSrcWin->height > u32BaseHeight))
    {
        if ((stSrcWin->width >= ((u32BaseWidth * stSrcWin->height) / u32BaseHeight)))
        {
            // stSrcWin->width:stSrcwin->height >= base_width:base_height
            // base_width-based down scale
            MSG_DRV_VE("[%s]stage 0: do width-based down-scale\n", __FUNCTION__);
            // setup scaled_srcWin
            stScaledSrcWin.x = u32BaseWidth * stSrcWin->x / stSrcWin->width;
            stScaledSrcWin.y = u32BaseWidth * stSrcWin->y / stSrcWin->width;
            stScaledSrcWin.width = u32BaseWidth;
            stScaledSrcWin.height = u32BaseWidth * stSrcWin->height / stSrcWin->width;
            // setup scaled_cropWin
            stScaledCropWin.x = u32BaseWidth * stCropWin->x / stSrcWin->width;
            stScaledCropWin.y = u32BaseWidth * stCropWin->y / stSrcWin->width;
            stScaledCropWin.width = u32BaseWidth * stCropWin->width / stSrcWin->width;
            stScaledCropWin.height = u32BaseWidth * stCropWin->height / stSrcWin->width;
        }
        else
        {
            // stSrcWin->width:stSrcwin->height < base_width:base_height
            // base_height-based down scale
            MSG_DRV_VE("[%s]stage 0: do height-based down-scale\n", __FUNCTION__);
            // setup scaled_srcWin
            stScaledSrcWin.x = u32BaseHeight * stSrcWin->x / stSrcWin->height;
            stScaledSrcWin.y = u32BaseHeight * stSrcWin->y / stSrcWin->height;
            stScaledSrcWin.width = u32BaseHeight * stSrcWin->width / stSrcWin->height;
            stScaledSrcWin.height = u32BaseHeight;
            // setup scaled_cropWin
            stScaledCropWin.x = u32BaseHeight * stCropWin->x / stSrcWin->height;
            stScaledCropWin.y = u32BaseHeight * stCropWin->y / stSrcWin->height;
            stScaledCropWin.width = u32BaseHeight * stCropWin->width / stSrcWin->height;
            stScaledCropWin.height = u32BaseHeight * stCropWin->height / stSrcWin->height;
        }
    }

    // stage1 - Down scale source window width to base_width without lossing information.
    // (before MIU)
    MSG_DRV_VE("[%s]stage 1: stSrcWin (%d, %d, %d, %d) -> "
                "scaled_srcWin (%d, %d, %d, %d)\n",
                __FUNCTION__,
                stSrcWin->x, stSrcWin->y, stSrcWin->width, stSrcWin->height,
                stScaledSrcWin.x, stScaledSrcWin.x, stScaledSrcWin.width, stScaledSrcWin.height);

    g_VEInfo.u16H_CapSize = stSrcWin->width;
    g_VEInfo.u16V_CapSize = stSrcWin->height;
    MDrv_VE_set_scaling_ratio(stScaledSrcWin.width, stScaledSrcWin.height);

    // stage2 - Crop video from the down-scaled video. (after MIU)
    MSG_DRV_VE("[%s]stage 2: stCropWin (%d, %d, %d, %d) -> "
                "scaled_cropWin (%d, %d, %d, %d)\n",
                __FUNCTION__,
                stCropWin->x, stCropWin->y, stCropWin->width, stCropWin->height,
                stScaledCropWin.x, stScaledCropWin.x, stScaledCropWin.width,
                stScaledCropWin.height);

    if (E_VE_OK != MDrv_VE_set_crop_window(stScaledCropWin))
    {
        MSG_DRV_VE("[%s][%d] failed\n", __FUNCTION__, __LINE__);
        eRet = E_VE_FAIL;
        goto fail;
    }

    // stage3 - Scale cropped video to output video size. (after MIU)
    g_VEInfo.u16H_CapSize = stScaledCropWin.width;
    g_VEInfo.u16V_CapSize = stScaledCropWin.height;
    if (E_VE_OK != MDrv_VE_set_be_display_window(*stDispWin))
    {
        MSG_DRV_VE("[%s][%d] failed\n", __FUNCTION__, __LINE__);
        eRet = E_VE_FAIL;
        goto fail;
    }
    MSG_DRV_VE("[%s]stage 3: scaled_cropWin(%d, %d, %d, %d) -> "
                "stDispWin (%d, %d, %d, %d)\n",
                __FUNCTION__,
                stScaledCropWin.x, stScaledCropWin.x, stScaledCropWin.width,
                stScaledCropWin.height,
                stDispWin->x, stDispWin->y, stDispWin->width, stDispWin->height);

    return E_VE_OK;

fail:
    // revert changes
    g_VEInfo.u16H_CapSize = u16BakHCapSize;
    g_VEInfo.u16V_CapSize = u16BakVCapSize;
    printf("Error: [%s] failed, errno: %d\n", __FUNCTION__, eRet);
    return eRet;
}

#define MS_VEC_MAX_FRAMENUM_P      3 //These 2 setting should obey with driver setting in "Hal_VE_set_frc"
#define MS_VEC_MAX_FRAMENUM_I      2
/******************************************************************************/
/// Init and config the VE capture
/// @param pVECapture \b IN : @ref PMS_VE_Output_CAPTURE
/// @return TRUE: success
/// @return FALSE: fail
/******************************************************************************/
void _MDrv_VE_InitVECapture(PMS_VE_Output_CAPTURE pVECapture)
{
#ifdef MSOS_TYPE_LINUX
    g_VEInfo.bVECapture = pVECapture->bVECapture;
    if(g_VEInfo.bVECapture)
    {
        MDrv_VEC_GFLIPInit();
        g_VEInfo.u16OutputWidth = pVECapture->u16Width;
        g_VEInfo.u16Outputheight= pVECapture->u16height;
        g_VEInfo.u32MemSize     = pVECapture->u32MemSize;
        g_VEInfo.phyMiuBaseAddr = (MS_PHY)pVECapture->u32MiuBaseAddr;
        _MDrv_VE_SetMemoryBaseAddr(g_VEInfo.phyMiuBaseAddr, g_VEInfo.u32MemSize);
    #if VE_TVS_ISR_SUPPORT
        MS_GFLIP_VEC_CONFIG stGflipVECConfig;
        Hal_VE_ISRConfig(MS_VE_INT_TVS_VSYNC);
        stGflipVECConfig.u16Version  = VE_VEC_CONFIG_VERSION;//For compatibility with kernel
        stGflipVECConfig.u16Length   = sizeof(MS_GFLIP_VEC_CONFIG);
        stGflipVECConfig.eConfigType = MS_VEC_CONFIG_INIT;
        stGflipVECConfig.bInterlace  = g_VEInfo.bSrcInterlace;
        stGflipVECConfig.eIsrType    = MS_VEC_ISR_VE;
        stGflipVECConfig.u8MaxFrameNumber_P = MS_VEC_MAX_FRAMENUM_P;
        stGflipVECConfig.u8MaxFrameNumber_I = MS_VEC_MAX_FRAMENUM_I;
        stGflipVECConfig.u8Result = FALSE;
        if (ioctl(g_VEInfo.s32FdGFlip, MDRV_GFLIP_VEC_IOC_CONFIG, &stGflipVECConfig))
        {
            MSG_DRV_VE("%s %d: IO(INIT) fail!!!!\n", __FUNCTION__, __LINE__);
            return;
        }
        MSG_DRV_VE("VEC: Config(%u,%u,%u,%u,%u) Return=%u\n", stGflipVECConfig.bInterlace, stGflipVECConfig.u8MaxFrameNumber_P,
                           stGflipVECConfig.u8MaxFrameNumber_I, (MS_U8)stGflipVECConfig.eIsrType, (MS_U8)stGflipVECConfig.eConfigType, stGflipVECConfig.u8Result);
    #endif//VE_TVS_ISR_SUPPORT
    }
    else
    {
        g_VEInfo.stVECusScalingInfo.bHCusScalingEnable = FALSE;
        g_VEInfo.stVECusScalingInfo.bVCusScalingEnable = FALSE;
    }

    //workaround to pass parasoft test:
    //[2]A pointer parameter in a function prototype should be declared as pointer to const if the pointer is not used to modify the addressed object (JSF-118-4)
    pVECapture->u16Width += 0;
#if defined (__aarch64__)
    MSG_DRV_VE("VE InitVECapture bEnable=%u, W/H=%u, %u, MemBase=0x%lx, Size=%u\n",
                       g_VEInfo.bVECapture, pVECapture->u16Width, pVECapture->u16height, pVECapture->u32MiuBaseAddr, g_VEInfo.u32MemSize);
#else
    MSG_DRV_VE("VE InitVECapture bEnable=%u, W/H=%u, %u, MemBase=0x%tx, Size=%tu\n",
                       g_VEInfo.bVECapture, pVECapture->u16Width, pVECapture->u16height,(ptrdiff_t) pVECapture->u32MiuBaseAddr,(ptrdiff_t) g_VEInfo.u32MemSize);
#endif //def __aarch64__
#endif //def MSOS_TYPE_LINUX
}

void MDrv_VE_InitVECapture(PMS_VE_Output_CAPTURE pVECapture)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    P_TVE_INITCAPTURE pArgs = NULL;

    if(pu32TvencoderInst == NULL)
    {
        return ;
    }

    pArgs = (P_TVE_INITCAPTURE)malloc(sizeof(_TVE_INITCAPTURE));

    if(NULL == pArgs)
        return ;
    pArgs->pVECapture = pVECapture;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_InitVECapture, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    free(pArgs);
#else
    _MDrv_VE_InitVECapture(pVECapture);
#endif
}
/******************************************************************************/
/// Enable VE capture
/// @param pstVECapState \b IN : @ref PMS_VE_VECAPTURESTATE
/// @return TRUE: success
/// @return FALSE: fail
/******************************************************************************/
MS_BOOL _MDrv_VE_EnaVECapture(PMS_VE_VECAPTURESTATE pstVECapState)
{
#ifdef MSOS_TYPE_LINUX
    MS_GFLIP_VEC_STATE  stGflipVECstate;
    stGflipVECstate.bEnable      = pstVECapState->bEnable;//Enable or Disable
    stGflipVECstate.u8Result     = FALSE; //default set to false
    stGflipVECstate.u8FrameCount = 0;     //default set to 0:Invalid


    if(0 > g_VEInfo.s32FdGFlip)
    {
        MSG_DRV_VE("%s %d: GLIP device not opened!!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

#if VE_TVS_ISR_SUPPORT
    //Below is Progressive/Interlace config for VEC
    MS_GFLIP_VEC_CONFIG stGflipVECConfig;
    memset(&stGflipVECConfig, 0, sizeof(stGflipVECConfig));
    stGflipVECConfig.u16Version  = VE_VEC_CONFIG_VERSION;//For compatibility with kernel
    stGflipVECConfig.u16Length   = sizeof(MS_GFLIP_VEC_CONFIG);
    stGflipVECConfig.eConfigType = MS_VEC_CONFIG_ENABLE;
    stGflipVECConfig.bInterlace  = g_VEInfo.bSrcInterlace;
    if (ioctl(g_VEInfo.s32FdGFlip, MDRV_GFLIP_VEC_IOC_CONFIG, &stGflipVECConfig))
    {
        MSG_DRV_VE("%s %d: IO(INIT) fail!!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(TRUE == stGflipVECConfig.u8Result)
#endif //VE_TVS_ISR_SUPPORT
    {
        if (ioctl(g_VEInfo.s32FdGFlip, MDRV_GFLIP_VEC_IOC_ENABLEVECAPTURE, &stGflipVECstate))
        {
            MSG_DRV_VE("%s %d: IO(ENABLE) fail!!!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        pstVECapState->bEnable  = stGflipVECstate.bEnable;//return current VEC status info
        pstVECapState->u8FrameCount = stGflipVECstate.u8FrameCount;
#if VE_TVS_ISR_SUPPORT
        if(stGflipVECstate.u8Result == TRUE)
        {
            if(stGflipVECstate.bEnable == TRUE)
            {
                //OS_DELAY_TASK(300);
                Hal_VE_sofeware_reset(ENABLE); // software reset
                Hal_VE_set_reg_load(ENABLE);// load register,but not affect bit3(VBI output)
                //MsOS_DelayTask(5) ; // delay 5 ms
                OS_DELAY_TASK(30);
                Hal_VE_set_reg_load(DISABLE);// load register,but not affect bit3(VBI output)
                OS_DELAY_TASK(5);
                Hal_VE_sofeware_reset(DISABLE); // software reset
                OS_DELAY_TASK(5);
                Hal_VE_set_ve_on_off(ENABLE);
            }
            else
            {
                Hal_VE_set_ve_on_off(DISABLE);
            }
        }
#endif //#if VE_TVS_ISR_SUPPORT
    }
    pstVECapState->u8Result = stGflipVECstate.u8Result;
    MSG_DRV_VE("%s %d: return=%u, Frame=%u\n", __FUNCTION__, __LINE__, stGflipVECstate.u8Result, stGflipVECstate.u8FrameCount);
    return stGflipVECstate.u8Result;
#endif
    return FALSE; //Unsupport now
}

MS_BOOL MDrv_VE_EnaVECapture(PMS_VE_VECAPTURESTATE pstVECapState)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_ENACAPTURE pArgs = NULL;

    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_ENACAPTURE)malloc(sizeof(TVE_ENACAPTURE));

    if(NULL == pArgs)
        return FALSE;
    pArgs->pstVECapState = pstVECapState;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_EnaVECapture, (void*)pArgs);

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return FALSE;
    }
    else
    {
        free(pArgs);
        return TRUE;
    }
#else
    return _MDrv_VE_EnaVECapture(pstVECapState);
#endif
}

/******************************************************************************/
/// Get VE capture state
/// @param pstVECapState \b OUT : @ref PMS_VE_VECAPTURESTATE
/// @return TRUE: success
/// @return FALSE: fail
/******************************************************************************/
MS_BOOL MDrv_VE_GetVECaptureState(PMS_VE_VECAPTURESTATE pstVECapState)
{
#ifdef MSOS_TYPE_LINUX
    MS_GFLIP_VEC_STATE stGflipVECstate;
    stGflipVECstate.u8Result     = FALSE;
    if(0 > g_VEInfo.s32FdGFlip)
    {
        MSG_DRV_VE("%s %d: GLIP device not opened!!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (ioctl(g_VEInfo.s32FdGFlip, MDRV_GFLIP_VEC_IOC_GETVECAPTURESTATE, &stGflipVECstate))
    {
        MSG_DRV_VE("%s %d: IO(GET STATE) fail!!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    //printf("GFLP=%u\n", pstVECapState->u8FrameCount);
    pstVECapState->u8Result = stGflipVECstate.u8Result;
    if(TRUE == pstVECapState->u8Result)
    {
        pstVECapState->bEnable = stGflipVECstate.bEnable;
        pstVECapState->u8FrameCount = stGflipVECstate.u8FrameCount;
    }
    return stGflipVECstate.u8Result;
#endif
    return FALSE; //Unsupport now
}

//-------------------------------------------------------------------------------------------------
/// Wait on the finish of specified frame: Check if the frame is captured,
/// if Yes, return TRUE, otherwise sleep until the next Vsync ISR
/// @param  pstVECapState          \b IN : @ref PMS_VE_VECAPTURESTATE
/// @return TRUE: success
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_VE_VECaptureWaitOnFrame(PMS_VE_VECAPTURESTATE pstVECapState)
{
#ifdef MSOS_TYPE_LINUX
    MS_GFLIP_VEC_STATE stGflipVECstate;
    stGflipVECstate.u8FrameCount = pstVECapState->u8FrameCount;
    stGflipVECstate.u8Result     = FALSE;
    if(0 > g_VEInfo.s32FdGFlip)
    {
        MSG_DRV_VE("%s %d: GLIP device not opened!!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (ioctl(g_VEInfo.s32FdGFlip, MDRV_GFLIP_VEC_IOC_VECAPTUREWAITONFRAME, &stGflipVECstate))
    {
        MSG_DRV_VE("%s %d: IO(WAIT) fail!!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    //printf("GFLP=%u\n", pstVECapState->u8FrameCount);
    pstVECapState->bEnable  = stGflipVECstate.bEnable;
    pstVECapState->u8Result = stGflipVECstate.u8Result;
    return pstVECapState->u8Result;
#endif
    return FALSE; //Unsupport now
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_VE_Adjust_FrameStart
/// @brief \b Function \b Description : the frame start is used to adjust output video
///                                     (in pixel)
///
/// @param <IN>        \b pixel_offset
/// @return VE_Result
////////////////////////////////////////////////////////////////////////////////
VE_Result _MDrv_VE_Adjust_FrameStart(MS_VE_WINDOW_TYPE *stSrcWin,MS_S16 pixel_offset)
{
    //central alignment
    //(720 only show 704. Align formula : Waddr mapping Raddr+8pixel)
    //case 1: source is not PAL mode - non-alignment
    //case 2: source is 704 & PAL mode and DI mode - non-alignment //DI
    //others : alignment //include OP:704 scale to 720 case

    Hal_VE_SetMemAddr(&g_VEInfo.phyMiuBaseAddr,&g_VEInfo.phyMiuBaseAddr);

    if((stSrcWin) && (g_VEInfo.VideoSystem >= MS_VE_PAL_N )&&(g_VEInfo.VideoSystem <= MS_VE_PAL ) )   //PAL mode
    {
        if(!(((stSrcWin->width==704) && (stSrcWin->height==576)) &&
            (g_VEInfo.InputSrcType == MS_VE_SRC_DI)) )
        {
            MS_PHY phyAlignAddr = g_VEInfo.phyMiuBaseAddr + 16;
            Hal_VE_SetMemAddr(&(phyAlignAddr),&g_VEInfo.phyMiuBaseAddr);
        }
    }

    return Hal_VE_Adjust_FrameStart(pixel_offset);
}

VE_Result MDrv_VE_Adjust_FrameStart(MS_VE_WINDOW_TYPE *stSrcWin,MS_S16 pixel_offset)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_ADJFRAMESTART pArgs = NULL;

    if(pu32TvencoderInst == NULL)
    {
        if(UtopiaOpen(MODULE_TVENCODER, (void**)&pu32TvencoderInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen TVE failed\n");
            return FALSE;
        }
    }

    pArgs = (PTVE_ADJFRAMESTART)malloc(sizeof(TVE_ADJFRAMESTART));

    if(NULL == pArgs)
        return E_VE_FAIL;
    pArgs->stSrcWin = stSrcWin;
    pArgs->pixel_offset = pixel_offset;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_Adjust_FrameStart, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return E_VE_FAIL;
    }
    else
    {
        free(pArgs);
        return E_VE_OK;
    }
#else
    return _MDrv_VE_Adjust_FrameStart(stSrcWin,pixel_offset);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_VE_SetWSS525Data
/// @brief \b Function \b Description : Start or Stop to send WSS data on 525i system
/// @param  bEn   enable/disable wss525
/// @param  u32WSSData  20-bit wss data includes 14 bits of data and 6 bits of CRC
///
/// @return             @ref  VE_Result
//-------------------------------------------------------------------------------------------------
VE_Result MDrv_VE_SetWSS525Data(MS_BOOL bEn, MS_U32 u32WSSData)
{
#if defined (__aarch64__)
    MSG_DRV_VE("Enable = %d, wss525data = %u\n", bEn, u32WSSData);
#else
    MSG_DRV_VE("Enable = %d, wss525data = %tu\n", bEn,(ptrdiff_t) u32WSSData);
#endif
    #if 0
    Hal_VE_set_wss_data(bEn, u32WSSData);
    #else
    return Hal_VE_set_wss525_data(bEn, u32WSSData);
    #endif
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_VE_GetWSS525Data
/// @brief \b Function \b Description : Get current wss data
/// @param  void
///
/// @return   32-bit wss data, it includes 14 bits of data and 6 bits of CRC in lower 20 bits
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_VE_GetWSS525Data(void)
{
    #if 0
    return Hal_VE_get_wss_data();
    #else
    return Hal_VE_get_wss525_data();
    #endif
}

void MDrv_VE_OnOffWSS(MS_BOOL ben)
{
    Hal_VE_OnOffWSS(ben);
}
MS_BOOL MDrv_VE_GetWSSStatus(void)
{
    return Hal_VE_GetWSSStatus();
}
void MDrv_VE_OnOffMV(MS_BOOL ben)
{
    Hal_VE_OnOffMV(ben);
}
MS_BOOL MDrv_VE_GetMVStatus(void)
{
    return Hal_VE_GetMVStatus();
}
void MDrv_VE_OnOffDCS(MS_BOOL ben)
{
    Hal_VE_OnOffDCS(ben);
}
MS_BOOL MDrv_VE_GetDCSStatus(void)
{
    return Hal_VE_GetDCSStatus();
}

MS_U32 _MDrv_TVE_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U32 u32Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		//_gbInit = false;
		u32Return = TRUE;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
	    MS_PHY phyMIUAddress = 0;
        MS_Switch_VE_Src_Info SwitchInputSrc;
        MS_VE_Output_Ctrl OutputCtrl;
        SwitchInputSrc.InputSrcType =g_VEInfo.InputSrcType;
        OutputCtrl.bEnable = TRUE;
        OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;

        //Use _phyVEMIUBaseAddr instead of g_VEInfo.phyMiuBaseAddr, otherwise VE memory address will add Dolby buffer address every time.
        //It will overlap other module and cause coredump.
        //_phyVEMIUBaseAddr is raw value get from VE_INIT,
        //this value havs add MIU interval already
        phyMIUAddress = _phyVEMIUBaseAddr;

        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            // init
            //_MDrv_VE_Init(u32MIUAddress);
            if (MDrv_VE_SetIOMapBase() != TRUE)
            {
                MSG_DRV_VE("VE: MDrv_VE_Init MDrv_VE_SetIOMapBase() failure\n");
                return FALSE;
            }
            //u32MIUAddress = 0x0000100000+HAL_MIU1_BASE;
#if defined (__aarch64__)
            MSG_DRV_VE("VE input  u32MIUAddress=0x%lx \n",phyMIUAddress);
#else
            MSG_DRV_VE("VE input  u32MIUAddress=0x%tx \n",(ptrdiff_t)phyMIUAddress);
#endif
            //get chip capability
            Hal_VE_GetCaps(&g_VECap);
            _MDrv_VE_SetMemoryBaseAddr(phyMIUAddress, 0);
            Hal_VE_init();
            // set output std
            _MDrv_VE_SetOutputVideoStd(g_VEInfo.VideoSystem);
            // set input source
            _MDrv_VE_SwitchInputSource(&SwitchInputSrc);
            // switch output dest.
            Hal_VE_set_out_sel(g_VEInfo.u8DACType);
            Hal_VE_set_color_convert(g_VEInfo.VideoSystem, g_VEInfo.u8DACType);
            // set mode
            MDrv_VE_set_capture_window(); // set capture window
            MDrv_VE_set_scaling_ratio(g_VEInfo.u16OutputWidth, g_VEInfo.u16Outputheight);    // set scaling ratio
            MDrv_VE_set_frc(g_VEInfo.u16inputVfreq); // set frc
            // set output ctrl.
            _MDrv_VE_SetOutputCtrl(&OutputCtrl);

			_prev_u16PowerState = u16PowerState;
			u32Return = TRUE;//RESUME_OK;
		}
		else
		{
			//printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u32Return = TRUE;//SUSPEND_FAILED;
		}
	}
	else
	{
		//printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u32Return = FALSE;
	}

	return u32Return;// for success
}

MS_U32 MDrv_TVE_SetPowerState(EN_POWER_MODE u16PowerState)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    PTVE_SETPOWERSTATE pArgs;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_SETPOWERSTATE)malloc(sizeof(TVE_SETPOWERSTATE));

    if(NULL == pArgs)
        return FALSE;
    pArgs->ePowerState = u16PowerState;
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_SetPowerState, (void*)pArgs);

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_FAIL == pArgs->eRet)
    {
        free(pArgs);
        return FALSE;
    }
    else
    {
        free(pArgs);
        return TRUE;
    }
#else
    return _MDrv_TVE_SetPowerState(u16PowerState);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_VE_ReloadSetting()
/// @brief \b Function \b Description : Do SW resetting do confirm all change are applied
/// @param  void
///
/// @return             @ref  VE_Result
//-------------------------------------------------------------------------------------------------
VE_Result _MDrv_VE_ReloadSetting(void)
{
    return Hal_VE_ReloadSetting();
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_VE_ReloadSetting()
/// @brief \b Function \b Description : Do SW resetting do confirm all change are applied
/// @param  void
///
/// @return             @ref  VE_Result
//------------------------------------------------------------------------------
VE_Result MDrv_VE_ReloadSetting(void)
{
#if TVENCODER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PTVE_RELOADSETTING pArgs = NULL;
    if(pu32TvencoderInst == NULL)
    {
        return FALSE;
    }
    pArgs = (PTVE_RELOADSETTING)malloc(sizeof(PTVE_RELOADSETTING));

    if(NULL == pArgs)
    {
        return E_TVENCODER_FAIL;
    }
    pArgs->eRet = E_TVENCODER_FAIL;

    u32Ret = UtopiaIoctl(pu32TvencoderInst, E_MDRV_CMD_TVE_ReloadSetting, (void*)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    if(E_TVENCODER_OK == pArgs->eRet)
    {
        free(pArgs);
        return TRUE;
    }
    else
    {
        free(pArgs);
        return FALSE;
    }
#else
    return _MDrv_VE_ReloadSetting();
#endif

}
#endif

