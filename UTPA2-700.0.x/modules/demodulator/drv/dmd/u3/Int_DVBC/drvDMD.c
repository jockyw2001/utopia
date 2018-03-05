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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDMD.c
/// @brief  DVBC Demodulator Driver Interface
/// @author MStar Semiconductor,Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <math.h>
#else // !MSOS_TYPE_LINUX
#endif // MSOS_TYPE_LINUX

#include "MsCommon.h"
#include "drvDMD.h"
#include "devDVBC.h"


#if   (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DMD_DBG_ENABLE              0


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define DMD_SNR_BIAS                3.5

#ifdef MSOS_TYPE_LINUX_KERNEL
#define DMD_MUTEX_TIMEOUT           MSOS_WAIT_FOREVER
#else
#define DMD_MUTEX_TIMEOUT           2000                                // The maximum timeout by X4CFE_en
#endif

#ifndef MS_NOSAPI
#define DMD_INIT()                 { _s32DmdMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "DMD_Mutex", MSOS_PROCESS_SHARED);   \
                                     if (_s32DmdMutexId < 0)                                        \
                                     {                                                              \
                                         DMD_DBG("%s Failed (%d)\n", __FUNCTION__, __LINE__);       \
                                         return FALSE;                                              \
                                     }                                                              \
                                   }
#define DMD_ENTRY()                if (!MsOS_ObtainMutex(_s32DmdMutexId, DMD_MUTEX_TIMEOUT))        \
                                   {   return FALSE;   }
#define DMD_RETURN(_ret)           { MsOS_ReleaseMutex(_s32DmdMutexId);                             \
                                     return _ret; }// Should be optimized by compiler
#else   // MS_NOSAPI
#define DMD_INIT()                 { }
#define DMD_ENTRY()                { }
#define DMD_RETURN(_ret)           { return _ret; }
#endif  // MS_NOSAPI


#if DMD_DBG_ENABLE
#define DMD_DBG(fmt, args...)       printf(fmt, ##args)
#else
#define DMD_DBG(fmt, args...)       {}
#endif


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MS_S32                       _s32DmdMutexId;
static MS_BOOL                      _bDmdOpen;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
int DVBC_Init(void);
int DVBC_Open(void);
int DVBC_Release(void);
int DVBC_Ioctl(MS_U32 cmd, ...);
#ifdef MS_NOSAPI
MS_BOOL uDVBC_Restart(DVBC_Param *pParam, DVBC_Mode *pMode);
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Initialize demodulator driver
/// @return TRUE(Success), FALSE(Failure)
/// DVBC driver is in @ref E_TASK_PRI_HIGH level
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Init(void)
{
    DMD_DBG("%s\n", __FUNCTION__);

    DVBC_Init();

    DMD_INIT()

    DMD_DBG("%s Successful\n", __FUNCTION__);

    return TRUE;

}


//-------------------------------------------------------------------------------------------------
/// Open a demodulator instance
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Open(void)
{
    DMD_DBG("%s\n", __FUNCTION__);

    DMD_ENTRY();

    if (_bDmdOpen)
    {
        DMD_DBG("%s Failed (%d)\n", __FUNCTION__, __LINE__);
        DMD_RETURN(FALSE);
    }

    // Open SMART device
    if (DVBC_Open() < 0)
    {
        DMD_DBG("%s Failed (%d)\n", __FUNCTION__, __LINE__);
        DMD_RETURN(FALSE);
    }

    _bDmdOpen = TRUE;

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Close a demodulator instance
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Close(void)
{
    DMD_DBG("%s\n", __FUNCTION__);

    DMD_ENTRY();
    if (_bDmdOpen == FALSE)
    {
        DMD_RETURN(FALSE);
    }

    _bDmdOpen = FALSE;

    // Close SMART device and free system resource
    DVBC_Release();

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Set demodulator tuning control
/// @param  pMode                   \b IN:  tuning control
/// @return TRUE(Success), FALSE(Failure)
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_SetMode(DMD_Mode *pMode)
{
    //[TODO] configurate DVBC_Mode if it's different than DMD_Mode
    if (DVBC_Ioctl(DEVDVBC_IOC_SETMODE, pMode))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Reset demodulator
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Reset(void)
{
    DMD_ENTRY();

    DMD_DBG("%s\n", __FUNCTION__);

    if (DVBC_Ioctl(DEVDVBC_IOC_RESET))
    {
        DMD_RETURN(FALSE);
    }

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Restart demodulator by tuning parameter
/// @param  pParam                  \b IN:  tuning parameter
/// @return TRUE(Success), FALSE(Failure)
/// @note
/// [NONOS_SUPPORT] TRUE: Lock
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Restart(DMD_Param *pParam)
{
    DMD_ENTRY();

    DMD_DBG("%s\n", __FUNCTION__);

    if (DVBC_Ioctl(DEVDVBC_IOC_RESET))
    {
        DMD_RETURN(FALSE);
    }

    //[TODO] configurate DVBC_Mode if it's different than DMD_Mode
    if (DVBC_Ioctl(DEVDVBC_IOC_RESTART, pParam))
    {
        DMD_RETURN(FALSE);
    }

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Enable/Disable demodulator external TS output
/// @param  bEnable                 \b IN:  Enable/disable
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_TsOut(MS_BOOL bEnable)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_SETTSOUT, bEnable ? 1 : 0))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Demodulator power on/off
/// @param  bPowerOn                \b IN:  Power on/off
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_PowerOnOff(MS_BOOL bPowerOn)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_SETPOWER, bPowerOn ? 1 : 0))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Set carrier frequency bandwidth
/// @param  u32BW                   \b IN:  Frequency bandwidth (KHz)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_SetBW(MS_U32 u32BW)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_SETBW, u32BW))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get carrier frequency bandwid setting
/// @param  pu32BW                  \b OUT: Pointer of return bandwidth
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetBW(MS_U32 *pu32BW)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_GETBW, pu32BW))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get demodulator lock status
/// @param  pbLock                  \b OUT: Pointer of return lock status
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetLock(MS_BOOL *pbLock)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, pbLock))
    {
        return FALSE;
    }
    return TRUE;
}


#ifndef MSOS_TYPE_LINUX_KERNEL
//-------------------------------------------------------------------------------------------------
/// Get carrier SNR indicator
/// @param  pu32SNR                 \b OUT: Pointer of return SNR (db)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetSNR(MS_U32 *pu32SNR)
{
    MS_S16              s16NoisePower;
    float              vk;

    *pu32SNR = 0;

    MS_BOOL             bLock;
    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }

    if (DVBC_Ioctl(DEVDVBC_IOC_GETNOISEPOWER, &s16NoisePower))
    {
        return FALSE;
    }
    if (s16NoisePower == 0)
    {
        s16NoisePower = 1;
    }
    if (s16NoisePower < 0)// negative
    {
        s16NoisePower = -s16NoisePower;
    }

    vk = 10 * log10(65536.f/((double)s16NoisePower)) - DMD_SNR_BIAS;
    *pu32SNR = (vk > 0.0) ? (MS_U32)(vk) : 0;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get carrier BER indicator
/// @param  pfBER                   \b OUT: Pointer of return BER (1/65535)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetBER(float *pfBER)
{
    MS_BOOL             bLock;
    MS_U16              u16WindowLen;
    MS_U32              u32ErrorBit;

    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }

    if (DVBC_Ioctl(DEVDVBC_IOC_GETWINDOWLEN, &u16WindowLen))
    {
        return FALSE;
    }
    if (DVBC_Ioctl(DEVDVBC_IOC_GETERRORBIT, &u32ErrorBit))
    {
        return FALSE;
    }
    // 0 <= fBER <= 1
    *pfBER = (double)(u32ErrorBit)/((double)(u16WindowLen) * 128 * 188 * 8);
    if (*pfBER > 1.0)
    {
        *pfBER = 1.0;
    }
    // .32 format

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get carrier PWR indicator
/// @param  ps32PWR                 \b OUT: Pointer of return PWR (0-255)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetPWR(MS_S32 *ps32PWR)
{
    MS_U32              u32RfGain, u32IfGain;
    double              fPower, fTemp;

    *ps32PWR = 0;

#if 0 // no need to be locked
    MS_BOOL             bLock;
    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }
#endif

    if (DVBC_Ioctl(DEVDVBC_IOC_GETRFGAIN, &u32RfGain))
    {
        return FALSE;
    }
    if (DVBC_Ioctl(DEVDVBC_IOC_GETIFGAIN, &u32IfGain))
    {
        return FALSE;
    }
    fTemp = -20 * log10((double)u32RfGain/0x10000) - 20 * log10((double)u32IfGain/0x10000) - 63.4;
    if (fTemp >= -45.1)
    {
        fPower = -30 + ( fTemp + 45.1 ) * 2.5974;
    }
    else if (fTemp >= -47.4)
    {
        fPower = -50 + ( fTemp + 47.4 ) * 8.69565;
    }
    else if (fTemp >= -52.3)
    {
        fPower = -55 + ( fTemp + 52.3 ) * 1.02;
    }
    else //if (fTemp < -52.3)
    {
        fPower = -55 + ( fTemp + 52.3 ) * 3.846154;
    }

    *ps32PWR = (MS_S32)(fPower);


    return TRUE;
}
#endif


//-------------------------------------------------------------------------------------------------
/// Get demodulator tuning parameter
/// @param  pParam                  \b OUT: Pointer of return paramter
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetParam(DMD_Param *pParam)
{
    MS_BOOL             bLock;

    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }

    if (DVBC_Ioctl(DEVDVBC_IOC_GETPARAM, pParam))
    {
        return FALSE;
    }

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Config demodulator register parameter
/// @param  pRegParam                \b OUT: Pointer of return paramter
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Dmd_Config(MS_U8 *pRegParam)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_CONFIG, pRegParam))
    {
        return FALSE;
    }

	return TRUE;
}

#endif // DEMOD_MSDVBC

