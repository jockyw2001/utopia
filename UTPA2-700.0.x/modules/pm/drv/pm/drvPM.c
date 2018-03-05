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
/// file    drvPM.c
/// @brief  PM Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Internal Definition
#define PM_LOCK_SUPPORT             1   // 1: Enable 0: Disable

// Common Definition
#include <string.h> //for memcpy, memset
#ifdef MSOS_TYPE_LINUX
#include <fcntl.h>
#include <sys/ioctl.h>
#include "drv_pm_io.h"
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvPM.h"

#include "halPM.h"
#include "regPM.h"

#include "drvMBX.h"
#include "drvMMIO.h"
#if PM_LOCK_SUPPORT
#include "drvSEM.h"
#endif

#include "ULog.h"

#define PM_MBX_QUEUESIZE 8

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if PM_LOCK_SUPPORT //-----------------------------------------------------------------------------
#define RES_HK51ID    0x01UL
#define RES_AeonID    0x02UL
#define RES_MipsID    0x03UL
#define PMSEMID       0x04UL

#define TAG_PM "PM"

#define _PM_ENTRY(u16PMResID)       do{                \
   while (MDrv_SEM_Get_Resource(PMSEMID, u16PMResID) == FALSE); \
}while(0)

#define _PM_RETURN(u16PMResId)     do{    \
     MDrv_SEM_Free_Resource(PMSEMID, u16PMResId); \
}while(0)
#else //-------------------------------------------------------------------------------------------
#define _PM_ENTRY(u16PMResID)                  do{ }while(0)
#define _PM_RETURN(u16PMResId)             do{ }while(0)
#endif //------------------------------------------------------------------------------------------

#define PM_MBX_QUEUESIZE         8
#define PM_MBX_TIMEOUT           5000

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define LOWBYTE(u16)    ((MS_U8)(u16))
#define HIGHBYTE(u16)   ((MS_U8)((u16) >> 8))

#ifdef MSOS_TYPE_NOS
    extern void mhal_interrupt_lock(void);
    extern void mhal_interrupt_unlock(void);
#endif

#if defined(__mips__) || defined(__arm__)
#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#define _VA2PA(x) (MS_U32)MsOS_VA2PA((x))

#else
#define _PA2VA(x) x
#define _VA2PA(x) x
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX
static MS_S32 _s32FdPM = -1;
#endif
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static PM_AckFlags gPmAckFlags;
static PM_PWM_AckFlags gPmPWMAckFlags;

static MSIF_Version _drv_pm_version = {
    .DDI = { PM_DRV_VERSION },
};
static PM_DrvInfo _sPmDrvInfo;
static PM_DbgLv _gPmDbgLevel = E_PM_DBGLV_ERR_ONLY;

#define EFUSE_SIZE          8
#define MASTER_KEY_SIZE     16
#define CHIPID_SIZE         4
#define MemAddr_SIZE        4

//static MS_U8 gPmMasterKey[MASTER_KEY_SIZE];
static MS_U8 gPmDeviceID[EFUSE_SIZE];
static MS_U8 gPmChipID[CHIPID_SIZE];
static MS_U8 gPmMemAddr[MemAddr_SIZE];

#if PM_LOCK_SUPPORT
#ifdef __mips__
static MS_U16 _gu16PMResID = RES_MipsID;
#else
static MS_U16 _gu16PMResID = RES_AeonID;
#endif
#endif

static IRRecord_Callback gfIRRecordCallback = NULL;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define PM_DBG_FUNC()             if (_gPmDbgLevel >= E_PM_DBGLV_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_PM, "\t====   %s   ====\n", __FUNCTION__));}
#define PM_DBG_INFO(x, args...)   if (_gPmDbgLevel >= E_PM_DBGLV_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_PM, x, ##args));}
#define PM_DBG_ERR(x, args...)    if (_gPmDbgLevel >= E_PM_DBGLV_ERR_ONLY) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_PM, x, ##args));}
#define PM_DBG_STR(x)     //(x)

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// PM MailBox Handler
/// @param  pData   \b IN: mailbox data configuration
/// @return @ref MBX_Result
//-------------------------------------------------------------------------------------------------
MBX_Result _MDrv_PM_MailBoxHandler(void *pData)
{
    MBX_Msg MB_Command;

   MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;

    PM_DBG_FUNC();

    mbxResult = MDrv_MBX_RecvMsg(E_MBX_CLASS_PM_WAIT, &MB_Command, PM_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);
    if (E_MBX_ERR_TIME_OUT == mbxResult)
    {
        return mbxResult;
    }

    if (E_MBX_SUCCESS == mbxResult)
    {
        if ((MB_Command.u8Ctrl == 0) || (MB_Command.u8Ctrl == 1))
        {
            PM_DBG_INFO("Get PM command: 0x%02x.\n", MB_Command.u8Index);
            switch (MB_Command.u8Index)
            {
                case PM_CMDIDX_ACK_51ToAEON:
                    switch(MB_Command.u8Parameters[0])
                    {
                        case PM_CMDIDX_INIT:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_INIT);
                            break;
                        case PM_CMDIDX_GET_STATUS:
                            *((PM_DrvStatus*)pData) = (PM_DrvStatus)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_STATUS);
                            break;
                        case PM_CMDIDX_GET_LIBVER:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_LIBVER);
                            break;
                        case PM_CMDIDX_POWER_DOWN:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_POWERDOWN);
                            break;
                        case PM_CMDIDX_RTC_INIT:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_RTCINIT);
                            break;
                        case PM_CMDIDX_RTC_SETCOUNTER:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_RTCSETCOUNTER);
                            break;
                        case PM_CMDIDX_RTC_GETCOUNTER:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_RTCGETCOUNTER);
                            break;
                        case PM_CMDIDX_RTC_SETMATCHCOUNTER:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_RTCSETMATCHCOUNTER);
                            break;
                        case PM_CMDIDX_RTC_GETMATCHCOUNTER:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_RTCGETMATCHCOUNTER);
                            break;
                        case PM_CMDIDX_GET_INFO:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_INFO);
                            break;
                        case PM_CMDIDX_GET_MASTERKEY:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_GETMASTERKEY);
                            break;
                        case PM_CMDIDX_GET_DEVICEID:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_GETDEVICEID);
                            gPmDeviceID[0] = MB_Command.u8Parameters[2];
                            gPmDeviceID[1] = MB_Command.u8Parameters[3];
                            gPmDeviceID[2] = MB_Command.u8Parameters[4];
                            gPmDeviceID[3] = MB_Command.u8Parameters[5];
                            gPmDeviceID[4] = MB_Command.u8Parameters[6];
                            gPmDeviceID[5] = MB_Command.u8Parameters[7];
                            break;
                        case PM_CMDIDX_GET_CHIPID:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_GETCHIPID);
                            gPmChipID[0] = MB_Command.u8Parameters[2];
                            gPmChipID[1] = MB_Command.u8Parameters[3];
                            gPmChipID[2] = MB_Command.u8Parameters[4];
                            gPmChipID[3] = MB_Command.u8Parameters[5];
                            break;
                        case PM_CMDIDX_GPIO_INIT:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_GPIOINIT);
                            break;
                        case PM_CMDIDX_GET_PM_MEMADDR:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_GETPMMEMADDR);
                            gPmMemAddr[0] = MB_Command.u8Parameters[2];
                            gPmMemAddr[1] = MB_Command.u8Parameters[3];
                            gPmMemAddr[2] = MB_Command.u8Parameters[4];
                            gPmMemAddr[3] = MB_Command.u8Parameters[5];
                            break;
                        case PM_CMDIDX_GETEXTRASRAMDATA:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_GETEXTRASRAMDATA);
                            break;
                        case PM_CMDIDX_SETEXTRASRAMDATA:
                            *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                            gPmAckFlags &= (PM_AckFlags)(~E_PM_ACKFLG_WAIT_SETEXTRASRAMDATA);
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
    }

    return mbxResult;
}

void _MDrv_PM_DummyMiuFifo(void)
{
    static PM_Dummy PmDummy;
    MS_U8 index;

    for(index = 0; index<16; index++)
    {
        PmDummy.u8Reserved[index] = index;
		//Coverity Fixed
		if( PmDummy.u8Reserved[index] != index )
		{
			PmDummy.u8Reserved[index] = index;
		}
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Check if runtime_pm is running
/// @return TRUE : Running
/// @return FALSE : Not running
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_isRunning(void)
{
    MS_BOOL bRet;
    //PM_DBG_ERR("MDrv_PM_isRunning is not support\n");
    bRet = HAL_PM_isRunning();
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// attach interrupt request
//-------------------------------------------------------------------------------------------------
void MDrv_PM_InterruptRequest( void )
{
    PM_DBG_ERR("MDrv_PM_InterruptRequest is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Load firmware data
/// @param  pPmCode   \b IN: PM code data
/// @param  u16Len    \b IN: Size of PM code data
//-------------------------------------------------------------------------------------------------
void MDrv_PM_LoadFw( MS_U8 *pPmCode, MS_U16 u16Len )
{
    PM_DBG_ERR("MDrv_PM_LoadFw is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Write 1 byte data to M4 Lite register
/// @param  u16Addr   \b IN: register address
/// @param  u8Data    \b IN: 1 byte value to write
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_RegWrite( MS_U16 u16Addr, MS_U8 u8Data )
{
    PM_DBG_ERR("MDrv_PM_RegWrite is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Read 1 byte data from M4 Lite register
/// @param  u16Addr       \b IN: register address
/// @return unsinged char value: 1 byte value for read
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PM_RegRead( MS_U16 u16Addr )
{
    PM_DBG_ERR("MDrv_PM_RegRead is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// set 1 bit data to M4 Lite register
/// @param  u16Addr       \b IN: register address
/// @param  bBit          \b IN: bit value to set
/// @param  u8BitPos      \b IN: which bit to set
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RegWriteBit(MS_U16 u16Addr, MS_U8 bBit, MS_U8 u8BitPos )
{
    PM_DBG_ERR("MDrv_PM_RegWriteBit is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// get 1 bit data from M4 Lite register
/// @param  u16Addr       \b IN: register address
/// @param  u8BitPos      \b IN: which bit to read
/// @return unsinged char value: 8 bits (1 byte) value for read with bit mask
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PM_RegReadBit(MS_U16 u16Addr, MS_U8 u8BitPos)
{
    PM_DBG_ERR("MDrv_PM_RegReadBit is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Write 2 byte data to M4 Lite register
/// @param  u16RegIndex       \b IN: register address
/// @param  u16Value          \b IN: 2 byte value to write
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RegWrite2byte( MS_U16 u16RegIndex, MS_U16 u16Value )
{
    PM_DBG_ERR("MDrv_PM_RegWrite2byte is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Write 3 byte data to M4 Lite register
/// @param  u16Regndex        \b IN: register address
/// @param  u32Value          \b IN: 4 byte value to write
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RegWrite3byte( MS_U16 u16Regndex, MS_U32 u32Value )
{
    PM_DBG_ERR("MDrv_PM_RegWrite3byte is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// PM Sleep Control Function
/// @param  u8opcode       \b IN: control command
/// @param  u8Data         \b IN: 8 bits data
/// @param  u16Data        \b IN: 16 bits data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_Control( MS_U8 u8opcode, MS_U8 u8Data, MS_U16 u16Data )
{
    PM_DBG_ERR("MDrv_PM_Control is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// PM Control Write Function
/// @param  u8Opcode          \b IN: control command
/// @param  u8CmdType         \b IN: 8 bits data
/// @param  u8DataNum         \b IN: 8 bits data
/// @param  pu8Data           \b IN: 8 bits data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_CtrlWrite( MS_U8 u8Opcode, MS_U8 u8CmdType, MS_U8 u8DataNum, MS_U8 *pu8Data )
{
    PM_DBG_ERR("MDrv_PM_CtrlWrite is not support\n");
    return FALSE;
}


//-------------------------------------------------------------------------------------------------
/// PM Control Read Function
/// @param  u8Opcode          \b IN: control command
/// @param  u8CmdType         \b IN: 8 bits data
/// @param  u8DataNum         \b IN: 8 bits data
/// @param  pu8Data           \b IN: 8 bits data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_CtrlRead( MS_U8 u8Opcode, MS_U8 u8CmdType, MS_U8 u8DataNum, MS_U8 *pu8Data )
{
    PM_DBG_ERR("MDrv_PM_CtrlRead is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Write array data to memory of MICOM Module
/// @param  pu8Data          \b IN: Data write to PM
/// @param  u16MemLen        \b IN: length of data BYTE write to PM Memory
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_MemoryWrite(MS_U8 *pu8Data, MS_U16 u16MemLen)
{
    PM_DBG_ERR("MDrv_PM_MemoryWrite is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Read array data from memory of MICOM Module
/// @param  pu8Data          \b IN: Data write to PM
/// @param  u16MemLen        \b IN: length of data BYTE read from PM Memory
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_MemoryRead(MS_U8 *pu8Data, MS_U16 u16MemLen)
{
    PM_DBG_ERR("MDrv_PM_MemoryRead is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Enable RTC interrupt
/// @param  bEnable          \b IN: TRUE for enable, FALSE for disable
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCEnableInterrupt(MS_BOOL bEnable)
{
    _PM_ENTRY(_gu16PMResID);
    HAL_PM_WriteRegBit(REG_PMRTC_CTRL, bEnable? 0:1, PMRTC_CTRL_INT_MASK);
    HAL_PM_WriteRegBit(REG_PMRTC_CTRL, bEnable? 0:1, PMRTC_CTRL_INT_CLEAR);
    _PM_RETURN(_gu16PMResID);
    //PM_DBG_ERR("MDrv_PM_RTCEnableInterrupt is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Set rtc match time
/// @param  u32PmSysTime          \b IN: RTC match time
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCSetMatchTime(MS_U32 u32PmSysTime)
{
    _MDrv_PM_DummyMiuFifo();
    _PM_ENTRY(_gu16PMResID);
    HAL_PM_RtcSetMatchCounter(0, u32PmSysTime);
    _PM_RETURN(_gu16PMResID);
    //PM_DBG_ERR("MDrv_PM_RTCSetMatchTime is not support\n");
}


/*add by owen.qin begin*/
void MDrv_PM_RTC2EnableInterrupt(MS_BOOL bEnable)
{

    _PM_ENTRY(_gu16PMResID);
    HAL_PM_WriteRegBit(REG_PMRTC1_CTRL, bEnable? 0:1, PMRTC_CTRL_INT_MASK);
    HAL_PM_WriteRegBit(REG_PMRTC1_CTRL, bEnable? 0:1, PMRTC_CTRL_INT_CLEAR);
    _PM_RETURN(_gu16PMResID);

}

void MDrv_PM_RTC2SetMatchTime(MS_U32 u32PmSysTime)
{

    _MDrv_PM_DummyMiuFifo();
    _PM_ENTRY(_gu16PMResID);
    HAL_PM_RtcSetMatchCounter(1, u32PmSysTime);
    _PM_RETURN(_gu16PMResID);

}
/*add by owen.qin end*/

//-------------------------------------------------------------------------------------------------
/// Get rtc match time
/// @return unsinged long value: RTC match time
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PM_RTCGetMatchTime(void)
{

    MS_U32 pu32PmSysTime;

    _MDrv_PM_DummyMiuFifo();
    _PM_ENTRY(_gu16PMResID);
    pu32PmSysTime = HAL_PM_RtcGetMatchCounter(0);
    _PM_RETURN(_gu16PMResID);
    //PM_DBG_ERR("MDrv_PM_RTCGetMatchTime is not support\n");
    return pu32PmSysTime;
}

//-------------------------------------------------------------------------------------------------
/// Set rtc system time
/// @param  u32PmSysTime          \b IN: RTC system time
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCSetSystemTime(MS_U32 u32PmSysTime)
{
    _MDrv_PM_DummyMiuFifo();
    _PM_ENTRY(_gu16PMResID);
   HAL_PM_RtcSetCounter(0, u32PmSysTime);
    _PM_RETURN(_gu16PMResID);
    //PM_DBG_ERR("MDrv_PM_RTCSetSystemTime is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Get rtc system time
/// @return unsinged long value: RTC system time
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PM_RTCGetSystemTime(void)
{
    _MDrv_PM_DummyMiuFifo();
    MS_U32 pu32PmSysTime;
    _PM_ENTRY(_gu16PMResID);
    pu32PmSysTime = HAL_PM_RtcGetCounter(0);
    _PM_RETURN(_gu16PMResID);
    //PM_DBG_ERR("MDrv_PM_RTCGetSystemTime is not support\n");
    return pu32PmSysTime;
}

//-------------------------------------------------------------------------------------------------
/// RTC initialize
/// @param  u32CtrlWord          \b IN: Crystal clock frequency
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCInit(MS_U32 u32CtrlWord)
{
    PM_DBG_ERR("MDrv_PM_RTCInit is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// IR initialize
/// @param  irclk_mhz          \b IN: Crystal clock frequency
/// @param  irRegCfg           \b IN: IR register configuration
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IRInit(MS_U8 irclk_mhz, PM_IrRegCfg *irRegCfg)
{
    PM_DBG_ERR("MDrv_PM_IRInit is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Keypad initialize
/// @param  sarRegCfg          \b IN: SAR register configuration
/// @param  sarParamCfg        \b IN: SAR data configuration
//-------------------------------------------------------------------------------------------------
void MDrv_PM_KeypadInit(PM_SarRegCfg *sarRegCfg, PM_SarParamCfg *sarParamCfg)
{
    PM_DBG_ERR("MDrv_PM_KeypadInit is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Adjust internal clock
//-------------------------------------------------------------------------------------------------
void MDrv_PM_CalibrateRC(void)
{
    PM_DBG_ERR("MDrv_PM_CalibrateRC is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Check if pmsleep firmware is downloaded
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_isDownloaded(void)
{
    PM_DBG_ERR("MDrv_PM_isDownloaded is not support\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Set pmsleep firmware is downloaded
//-------------------------------------------------------------------------------------------------
void MDrv_PM_Set_Download(void)
{
    PM_DBG_ERR("MDrv_PM_Set_Download is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Attach different IRQ services
/// @param  irq          \b IN: IRQ type
/// @param  isr          \b IN: pointer of service funciton
/// @param  data         \b IN: interrupt/exception data
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqAttach(PM_IRQ_TYPE irq, PM_IsrCb_Type isr, MS_U32 data)
{
    PM_DBG_ERR("MDrv_PM_IrqAttach is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Detach specific IRQ services
/// @param  irq          \b IN: IRQ type
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqDetach(PM_IRQ_TYPE irq)
{
    PM_DBG_ERR("MDrv_PM_IrqDetach is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Enable specific irq request
/// @param  irq          \b IN: IRQ type
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqUnmask(PM_IRQ_TYPE irq)
{
   PM_DBG_ERR("MDrv_PM_IrqUnmask is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Disable specific irq request
/// @param  irq          \b IN: IRQ type
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqMask(PM_IRQ_TYPE irq)
{
    PM_DBG_ERR("MDrv_PM_IrqMask is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// Disable all PM irq requests
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqMaskAll(void)
{
    PM_DBG_ERR("MDrv_PM_IrqMask is not support\n");
}

//-------------------------------------------------------------------------------------------------
/// PM driver initialize
/// @param  pPmWakeCfg   \b IN: PM wake-up configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_Init(PM_WakeCfg *pPmWakeCfg)
{
    MS_VIRT virtSrcAddr;
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;
    MS_U16 u16WakeSrc = 0;
    
    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_INIT;

    virtSrcAddr = (MS_VIRT)pPmWakeCfg;
    PM_WakeCfg *PmWakeCfg = (PM_WakeCfg*)_PA2VA((MS_VIRT)pPmWakeCfg);   
        
    if(PmWakeCfg->bPmWakeEnableIR == 1)     u16WakeSrc |= (1<<0);
    if(PmWakeCfg->bPmWakeEnableSAR == 1)    u16WakeSrc |= (1<<1);
    if(PmWakeCfg->bPmWakeEnableGPIO0 == 1)  u16WakeSrc |= (1<<2);
    if(PmWakeCfg->bPmWakeEnableGPIO1 == 1)  u16WakeSrc |= (1<<3);
    if(PmWakeCfg->bPmWakeEnableUART1 == 1)  u16WakeSrc |= (1<<4);
    if(PmWakeCfg->bPmWakeEnableSYNC == 1)   u16WakeSrc |= (1<<5);
    if(PmWakeCfg->bPmWakeEnableESYNC == 1)  u16WakeSrc |= (1<<6);
    if(PmWakeCfg->bPmWakeEnableRTC0 == 1)   u16WakeSrc |= (1<<7);
    if(PmWakeCfg->bPmWakeEnableRTC1 == 1)   u16WakeSrc |= (1<<8);
    if(PmWakeCfg->bPmWakeEnableDVI0 == 1)   u16WakeSrc |= (1<<9);
    if(PmWakeCfg->bPmWakeEnableDVI2 == 1)   u16WakeSrc |= (1<<10);
    if(PmWakeCfg->bPmWakeEnableCEC == 1)    u16WakeSrc |= (1<<11);
    if(PmWakeCfg->bPmWakeEnableAVLINK == 1) u16WakeSrc |= (1<<12);
    if(PmWakeCfg->bPmWakeEnableMHL == 1)    u16WakeSrc |= (1<<13);
    if(PmWakeCfg->bPmWakeEnableWOL == 1)    u16WakeSrc |= (1<<14);

    HAL_PM_SetWakupDevice(u16WakeSrc);
    
    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_INIT;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    // Waiting for PM51 Power Management initialization done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_INIT) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_Init Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_Init OK\n");
    }

    return PmResult;
}

//-------------------------------------------------------------------------------------------------
/// PM driver get status
/// @param  pDrvStatus   \b IN: PM driver status configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetStatus(PM_DrvStatus *pDrvStatus)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    return PmResult;

}

//-------------------------------------------------------------------------------------------------
/// PM driver get library version
/// @param  pPmLibVer   \b IN: PM driver library version configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetLibVer(const MSIF_Version **ppVersion)
{
    PM_Result PmResult = E_PM_FAIL;

    if (!ppVersion)
        return E_PM_FAIL;

    *ppVersion = &_drv_pm_version;
    PmResult = E_PM_OK;

    return PmResult;
}

//-------------------------------------------------------------------------------------------------
/// PM driver power down
/// @param  pPmPowerDownCfg   \b IN: PM driver power down configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PowerDown(PM_PowerDownCfg *pPmPowerDownCfg)
{
    MS_VIRT virtSrcAddr;
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_POWERDOWN;

    virtSrcAddr = (MS_VIRT)pPmPowerDownCfg;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_POWER_DOWN;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_POWERDOWN) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PowerDown Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PowerDown OK\n");
    }


    return PmResult;
}

PM_Result MDrv_PM_GetMasterKey(MS_U8 *pPmPowerDownCfg)
{
    MS_VIRT virtSrcAddr;
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_GETMASTERKEY;

    virtSrcAddr = (MS_VIRT)pPmPowerDownCfg;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_GET_MASTERKEY;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_GETMASTERKEY) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_GetMasterKey Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_GetMasterKey OK\n");
    }

    return PmResult;
}

PM_Result MDrv_PM_GetDeviceID(MS_U8 *DeviceID)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_GETDEVICEID;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_GET_DEVICEID;
        MB_Command.u8ParameterCount = 0;

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_GETDEVICEID) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_GetDeviceID Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;

        DeviceID[0] = gPmDeviceID[0];
        DeviceID[1] = gPmDeviceID[1];
        DeviceID[2] = gPmDeviceID[2];
        DeviceID[3] = gPmDeviceID[3];
        DeviceID[4] = gPmDeviceID[4];
        DeviceID[5] = gPmDeviceID[5];

        PM_DBG_INFO("MDrv_PM_GetDeviceID OK\n");
    }

    return PmResult;
}

PM_Result MDrv_PM_GetPMMemAddr(MS_U8 *PmMemAddr)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_GETPMMEMADDR;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_GET_PM_MEMADDR;
        MB_Command.u8ParameterCount = 0;

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {

            //Error Handling here:
        }
    }

    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_GETPMMEMADDR) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;

        PM_DBG_ERR("MDrv_PM_GetPMMemAddr Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;

        PmMemAddr[0] = gPmMemAddr[0];
        PmMemAddr[1] = gPmMemAddr[1];
        PmMemAddr[2] = gPmMemAddr[2];
        PmMemAddr[3] = gPmMemAddr[3];
        PM_DBG_INFO("PmMemAddr[0] = 0x%x\n",PmMemAddr[0]);
        PM_DBG_INFO("PmMemAddr[1] = 0x%x\n",PmMemAddr[1]);
        PM_DBG_INFO("PmMemAddr[2] = 0x%x\n",PmMemAddr[2]);
        PM_DBG_INFO("PmMemAddr[3] = 0x%x\n",PmMemAddr[3]);

        PM_DBG_INFO("MDrv_PM_GetPMMemAddr OK\n");
    }

    return PmResult;
}

PM_Result MDrv_PM_GetChipID(MS_U8 *ChipParam)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_GETCHIPID;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_GET_CHIPID;
        MB_Command.u8ParameterCount = 0;

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_GETCHIPID) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_GetChipID Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;

        ChipParam[0] = gPmChipID[0];
        ChipParam[1] = gPmChipID[1];
        ChipParam[2] = gPmChipID[2];
        ChipParam[3] = gPmChipID[3];

        PM_DBG_INFO("MDrv_PM_GetChipID OK\n");
    }

    return PmResult;
}

//-------------------------------------------------------------------------------------------------
/// PM driver get information
/// @return @ref PM_DrvInfo
//-------------------------------------------------------------------------------------------------
const PM_DrvInfo* MDrv_PM_GetInfo(void)
{
    PM_DBG_FUNC();

    PM_Result PmResult = E_PM_FAIL;

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_VIRT virtSrcAddr;

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_INFO;

    virtSrcAddr = (MS_VIRT)(&_sPmDrvInfo);

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_GET_INFO;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {

        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);

    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_INFO) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_GetInfo Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_GetInfo OK\n");
    }

    //TODO
    return (&_sPmDrvInfo);
}

PM_Result MDrv_PM_GPIOInit(MS_U16 u16GPIOIndex)
{
    u16GPIOIndex &= 0x1FCF; //Mask reserved GPIO 4, 5 and unused bits

    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_GPIOINIT;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_GPIO_INIT;
        MB_Command.u8ParameterCount = 2;
        MB_Command.u8Parameters[0] =  (MS_U8)(u16GPIOIndex>>8);
        MB_Command.u8Parameters[1] =  (MS_U8)(u16GPIOIndex&0x00FFUL);
        mbxResult = MDrv_MBX_SendMsg(&MB_Command);
        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }

    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
            //Error Handling here:
            return E_PM_FAIL;
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_GPIOINIT) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_GPIOInit Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_GPIOInit OK\n");
    }

    return PmResult;
}
//-------------------------------------------------------------------------------------------------
/// Set debug level for debug message
/// @param  eLevel   \b IN: E_PM_DBGLV_NONE/E_PM_DBGLV_ERR_ONLY/E_PM_DBGLV_INFO/E_PM_DBGLV_ALL
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_SetDbgLevel(PM_DbgLv eLevel)
{
    PM_DBG_INFO("%s level: %u\n", __FUNCTION__, eLevel);

    _gPmDbgLevel = eLevel;
    return E_PM_OK;
}

//-------------------------------------------------------------------------------------------------
/// Rtc Initialize
/// @param  pPmRtcParam   \b IN: PM Rtc Parameters
/// @return @ref PM_Result
/// @note   Please call MDrv_PM_GetInfo() first to init _sPmDrvInfo
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcInit(PM_RtcParam *pPmRtcParam)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    //MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    //MBX_Msg MB_Command;
    //MS_U32 u32SrcAddr;
    PM_RtcParam *pPmRtcVA = (PM_RtcParam*)_PA2VA((MS_VIRT)pPmRtcParam);

    PM_DBG_FUNC();


    if(pPmRtcVA->u8PmRtcIndex > _sPmDrvInfo.u8PmSupRtcIdxMax)
    {
        PM_DBG_ERR("FAIL: Only support Max %d RTC(s)\n", _sPmDrvInfo.u8PmSupRtcIdxMax);
        return PmResult;
    }
#if 1
    MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base faill!\n");
        return PmResult;
    }

        HAL_PM_SetIOMapBase(virtBaseAddr);
#endif

    _PM_ENTRY(_gu16PMResID);
    HAL_PM_RtcInit(pPmRtcVA->u8PmRtcIndex, pPmRtcVA->u32RtcCtrlWord);
    _PM_RETURN(_gu16PMResID);
    //HAL_PM_RtcInit(pPmRtcParam->u8PmRtcIndex, pPmRtcParam->u32RtcCtrlWord);
    PmResult = E_PM_OK;

   #if 0
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_RTCINIT;

    u32SrcAddr = (MS_U32)pPmRtcParam;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_RTC_INIT;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_RTCINIT) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_RtcInit Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_RtcInit OK\n");
    }
 #endif

    return PmResult;
}

//-------------------------------------------------------------------------------------------------
/// Disable Rtc Init
/// @param  none  \b IN: none
/// @return @ref 
/// @note   
//-------------------------------------------------------------------------------------------------
void MDrv_PM_Rtc_DisableInit(E_PM_RTC eRtc)
{
    _PM_ENTRY(_gu16PMResID);
    HAL_PM_RTC_DisableInit(eRtc);
    _PM_RETURN(_gu16PMResID);
    
}

//-------------------------------------------------------------------------------------------------
/// Rtc set counter
/// @param  pPmRtcParam   \b IN: PM Rtc Parameters
/// @return @ref PM_Result
/// @note   Please call MDrv_PM_GetInfo() first to init _sPmDrvInfo
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcSetCounter(PM_RtcParam *pPmRtcParam)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    //MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    //MBX_Msg MB_Command;
    //MS_U32 u32SrcAddr;
    PM_RtcParam *pPmRtcVA = (PM_RtcParam*)_PA2VA((MS_VIRT)pPmRtcParam);

    PM_DBG_FUNC();

    if(pPmRtcVA->u8PmRtcIndex > _sPmDrvInfo.u8PmSupRtcIdxMax)
    {
        PM_DBG_ERR("FAIL: Only support Max %d RTC(s)\n", _sPmDrvInfo.u8PmSupRtcIdxMax);
        return PmResult;
    }

    _PM_ENTRY(_gu16PMResID);
    HAL_PM_RtcSetCounter(pPmRtcVA->u8PmRtcIndex, pPmRtcVA->u32RtcSetCounter);
    _PM_RETURN(_gu16PMResID);
    PmResult=E_PM_OK;

    #if 0
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_RTCSETCOUNTER;

    u32SrcAddr = (MS_U32)pPmRtcParam;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_RTC_SETCOUNTER;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_RTCSETCOUNTER) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_RtcSetCounter Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_RtcSetCounter OK\n");
    }
    #endif

    return PmResult;
}

//-------------------------------------------------------------------------------------------------
/// Rtc get counter
/// @param  pPmRtcParam   \b IN: PM Rtc Parameters
/// @return @ref PM_Result
/// @note   Please call MDrv_PM_GetInfo() first to init _sPmDrvInfo
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcGetCounter(PM_RtcParam *pPmRtcParam)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    //MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    //MBX_Msg MB_Command;
    //MS_U32 u32SrcAddr;
    PM_RtcParam *pPmRtcVA = (PM_RtcParam*)_PA2VA((MS_VIRT)pPmRtcParam);
    PM_DBG_FUNC();

    if(pPmRtcVA->u8PmRtcIndex > _sPmDrvInfo.u8PmSupRtcIdxMax)
    {
        PM_DBG_ERR("FAIL: Only support Max %d RTC(s)\n", _sPmDrvInfo.u8PmSupRtcIdxMax);
        return PmResult;
    }

    _PM_ENTRY(_gu16PMResID);
    pPmRtcVA->u32RtcGetCounter = HAL_PM_RtcGetCounter(pPmRtcVA->u8PmRtcIndex);
    _PM_RETURN(_gu16PMResID);
    PmResult = E_PM_OK;

    #if 0
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_RTCGETCOUNTER;

    u32SrcAddr = (MS_U32)pPmRtcParam;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_RTC_GETCOUNTER;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_RTCGETCOUNTER) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_RtcGetCounter Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_RtcGetCounter OK\n");
    }

    #endif

    return PmResult;
}

//-------------------------------------------------------------------------------------------------
/// Rtc set match counter
/// @param  pPmRtcParam   \b IN: PM Rtc Parameters
/// @return @ref PM_Result
/// @note   Please call MDrv_PM_GetInfo() first to init _sPmDrvInfo
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcSetMatchCounter(PM_RtcParam *pPmRtcParam)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();

    //MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    //MBX_Msg MB_Command;
    //MS_U32 u32SrcAddr;
    PM_RtcParam *pPmRtcVA = (PM_RtcParam*)_PA2VA((MS_VIRT)pPmRtcParam);
    PM_DBG_FUNC();

    if(pPmRtcVA->u8PmRtcIndex > _sPmDrvInfo.u8PmSupRtcIdxMax)
    {
        PM_DBG_ERR("FAIL: Only support Max %d RTC(s)\n", _sPmDrvInfo.u8PmSupRtcIdxMax);
        return PmResult;
    }

    _PM_ENTRY(_gu16PMResID);
    HAL_PM_RtcSetMatchCounter(pPmRtcVA->u8PmRtcIndex, pPmRtcVA->u32RtcSetMatchCounter);
    _PM_RETURN(_gu16PMResID);
    PmResult = E_PM_OK;

    #if 0
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_RTCSETMATCHCOUNTER;

    u32SrcAddr = (MS_U32)pPmRtcParam;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_RTC_SETMATCHCOUNTER;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_RTCSETMATCHCOUNTER) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_RtcSetMatchCounter Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_RtcSetMatchCounter OK\n");
    }
    #endif

    return PmResult;
}


//-------------------------------------------------------------------------------------------------
/// Rtc get match counter
/// @param  pPmRtcParam   \b IN: PM Rtc Parameters
/// @return @ref PM_Result
/// @note   Please call MDrv_PM_GetInfo() first to init _sPmDrvInfo
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcGetMatchCounter(PM_RtcParam *pPmRtcParam)
{
    PM_Result PmResult = E_PM_FAIL;

    _MDrv_PM_DummyMiuFifo();


    //MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    //MBX_Msg MB_Command;
    //MS_U32 u32SrcAddr;
    PM_RtcParam *pPmRtcVA = (PM_RtcParam*)_PA2VA((MS_VIRT)pPmRtcParam);
    PM_DBG_FUNC();

    if(pPmRtcVA->u8PmRtcIndex > _sPmDrvInfo.u8PmSupRtcIdxMax)
    {
        PM_DBG_ERR("FAIL: Only support Max %d RTC(s)\n", _sPmDrvInfo.u8PmSupRtcIdxMax);
        return PmResult;
    }

    _PM_ENTRY(_gu16PMResID);
    pPmRtcVA->u32RtcGetMatchCounter = HAL_PM_RtcGetMatchCounter(pPmRtcVA->u8PmRtcIndex);
    _PM_RETURN(_gu16PMResID);
    PmResult = E_PM_OK;

    #if 0
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_RTCGETMATCHCOUNTER;

    u32SrcAddr = (MS_U32)pPmRtcParam;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_RTC_GETMATCHCOUNTER;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    do
    {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_RTCGETMATCHCOUNTER) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_RtcGetMatchCounter Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_RtcGetMatchCounter OK\n");
    }
    #endif

    return PmResult;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_PM_SetSPIOffsetForMCU
/// @brief \b Function  \b Description: MCU Reset and set code offset in spi
/// @param pStatus      \b Out: non
////////////////////////////////////////////////////////////////////////////////
void MDrv_PM_SetSPIOffsetForMCU(MS_U32 BANK)
{
    MS_VIRT virtBaseAddr;
	MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base faill!\n");
        return ;
    }

        HAL_PM_SetIOMapBase(virtBaseAddr);

    HAL_PM_SetSPIOffsetForMCU(BANK);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_PM_SetSRAMOffsetForMCU
/// @brief \b Function  \b Description: MCU Reset on MCU SRAM.
/// @param pStatus      \b Out: non
////////////////////////////////////////////////////////////////////////////////
void MDrv_PM_SetSRAMOffsetForMCU(void)
{
    MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base faill!\n");
        return ;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);

    HAL_PM_SetSRAMOffsetForMCU();

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_PM_SetDRAMOffsetForMCU
/// @brief \b Function  \b Description: MCU Reset and set code offset in DRAM.
/// @return TRUE : succeed
/// @return FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_PM_SetDRAMOffsetForMCU(MS_U32 u32Offset)
{
    MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
	MS_BOOL bRet;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base faill!\n");
        return FALSE;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);

    bRet = HAL_PM_SetDRAMOffsetForMCU(u32Offset);
	
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_PM_PowerOnMode
/// @brief \b Function  \b Description: Return power on mode
/// @param pStatus      \b Out: PM_PowerOnMode
////////////////////////////////////////////////////////////////////////////////
PM_PowerOnMode MDrv_PM_PowerOnMode(void)
{
    PM_PowerOnMode PowerOnMode = E_PM_POWERON_ACON;
	MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base faill!\n");
        return PowerOnMode;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);
    PowerOnMode =(PM_PowerOnMode) HAL_PM_PowerOnMode();
    return PowerOnMode ;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_PM_GetWakeupSource
/// @brief \b Function  \b Description: Return wakeup source
/// @param pStatus      \b Out: PM_WakeupSource
////////////////////////////////////////////////////////////////////////////////
PM_WakeupSource MDrv_PM_GetWakeupSource(void)
{
    PM_WakeupSource WakeupSource = E_PM_WAKEUPSRC_NONE;
	MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base fail!\n");
        return WakeupSource;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);
    WakeupSource=(PM_WakeupSource) HAL_PM_GetWakeupSource();
    return WakeupSource;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_PM_GetWakeupKey
/// @brief \b Function  \b Description: Return wakeup key
/// @param     \b Out: MS_U8 wakeup key value
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_PM_GetWakeupKey(void)
{
    MS_U8 u8WakeupKey = 0x00;
    MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base fail!\n");
        return u8WakeupKey;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);
    u8WakeupKey=(MS_U8) HAL_PM_GetWakeupKey();
    return u8WakeupKey;
}

//-------------------------------------------------------------------------------------------------
/// PM Get Extra Sram Data
/// @param  pPmExtraSramData   \b IN: PM get extra sram data configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------

PM_Result MDrv_PM_GetExtraSramData(PM_ExtraSramData *pPmExtraSramData)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_VIRT virtSrcAddr;

    MS_U32 MBX_Resendcnt = 0;
    PM_ExtraSramData *pPmExtraSramDataVA = (PM_ExtraSramData*)_PA2VA((MS_VIRT)pPmExtraSramData);

    if (pPmExtraSramDataVA->u16Offset + pPmExtraSramDataVA->u16DataLen > 512)
    {
        PM_DBG_ERR("Excess Extra SRAM size (512 bytes)\n");
        return E_PM_FAIL;
    }

    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_GETEXTRASRAMDATA;

    virtSrcAddr = (MS_VIRT)pPmExtraSramData;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_GETEXTRASRAMDATA;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    // Waiting for PM51 Power Management initialization done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_GETEXTRASRAMDATA) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_GetExtraSramData Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_GetExtraSramData OK\n");
    }

    return PmResult;
}

//-------------------------------------------------------------------------------------------------
/// PM Set Extra Sram Data
/// @param  pPmExtraSramData   \b IN: PM set extra sram data configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------

PM_Result MDrv_PM_SetExtraSramData(PM_ExtraSramData *pPmExtraSramData)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_VIRT virtSrcAddr;

    MS_U32 MBX_Resendcnt = 0;
    PM_ExtraSramData *pPmExtraSramDataVA = (PM_ExtraSramData*)_PA2VA((MS_VIRT)pPmExtraSramData);

    if (pPmExtraSramDataVA->u16Offset + pPmExtraSramDataVA->u16DataLen > 512)
    {
        PM_DBG_ERR("Excess Extra SRAM size (512 bytes)\n");
        return E_PM_FAIL;
    }

    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmAckFlags |= E_PM_ACKFLG_WAIT_SETEXTRASRAMDATA;

    virtSrcAddr = (MS_VIRT)pPmExtraSramData;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_SETEXTRASRAMDATA;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    // Waiting for PM51 Power Management initialization done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
          {
            //Error Handling here:
          }
       }
    }
    while((gPmAckFlags & E_PM_ACKFLG_WAIT_SETEXTRASRAMDATA) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_SetExtraSramData Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_SetExtraSramData OK\n");
    }

    return PmResult;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_PM_Disable51
/// @brief \b Function  \b Description: Disable PM51
/// @return @ref PM_Result
////////////////////////////////////////////////////////////////////////////////
PM_Result MDrv_PM_Disable51(void)
{
#if defined(__mips__) || defined(__arm__)


    MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize = 0;

    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
    {
        PM_DBG_ERR("%s Get IOMAP Base faill!\n", __FUNCTION__);
        return E_PM_FAIL;
    }
    HAL_PM_SetIOMapBase(virtBaseAddr);
    HAL_PM_Disable51();
#endif
    return E_PM_OK;
}



//-------------------------------------------------------------------------------------------------
/// PM PWM MailBox Handler
/// @param  pData   \b IN: mailbox data configuration
/// @return @ref MBX_Result
//-------------------------------------------------------------------------------------------------
MBX_Result _MDrv_PM_PWM_MailBoxHandler(void *pData)
{
    MBX_Msg MB_Command;

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;

    PM_DBG_FUNC();

    mbxResult = MDrv_MBX_RecvMsg(E_MBX_CLASS_PM_PWM_WAIT, &MB_Command, PM_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);
    if (E_MBX_ERR_TIME_OUT == mbxResult)
    {
        return mbxResult;
    }

    if (E_MBX_SUCCESS == mbxResult)
    {
        if ((MB_Command.u8Ctrl == 0) || (MB_Command.u8Ctrl == 1))
        {
            PM_DBG_INFO("Get PM PWM command: 0x%02x.\n", MB_Command.u8Index);
            if(MB_Command.u8Index == PM_CMDIDX_ACK_51ToAEON )
            {
                PM_DBG_INFO("\r\n PM_CMDIDX_ACK_51ToAEON.\n");
                PM_DBG_INFO("GetPM_CMDIDX_PWM_INIT 0x%02x.\n", PM_CMDIDX_PWM_INIT);
                PM_DBG_INFO("Get MB_Command.u8Parameters[0] 0x%02x.\n", MB_Command.u8Parameters[0]);
                PM_DBG_INFO("Get MB_Command.u8Parameters[1] 0x%02x.\n", MB_Command.u8Parameters[1]);
                switch(MB_Command.u8Parameters[0])
                {
                    case PM_CMDIDX_PWM_INIT:
                        *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                        gPmPWMAckFlags &= (PM_PWM_AckFlags)(~E_PM_PWM_ACKFLG_WAIT_INIT);
                        break;
                    case PM_CMDIDX_PWM_CONFIG:
                        *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                        gPmPWMAckFlags &= (PM_PWM_AckFlags)(~E_PM_PWM_ACKFLG_WAIT_CONF);
                        break;
                    case PM_CMDIDX_PWM_IRRECORD_TRANSMIT :
                        *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                        gPmPWMAckFlags &= (PM_PWM_AckFlags)(~E_PM_PWM_ACKFLG_WAIT_IRRECORD_TRANSMIT);
                        break;
                    case PM_CMDIDX_PWM_IRRECORD_RECEIVED_REQUIRE :
                        *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                        gPmPWMAckFlags &= (PM_PWM_AckFlags)(~E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_REQUIRE);
                        break;
                    case PM_CMDIDX_PWM_IRRECORD_RECEIVED_COMPLETE :
                        *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                        gPmPWMAckFlags &= (PM_PWM_AckFlags)(~E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_COMPLETE);
                        break;
                    case PM_CMDIDX_PWM_IRRECORD_RECEIVED_SETCALLBACK:
                        *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                        gPmPWMAckFlags &= (PM_PWM_AckFlags)(~E_PM_PWM_ACKFLG_WAIT_IRRECORD_SETCALLBACK);
                        break;
                    case PM_CMDIDX_PWM_IRRECORD_RECEIVED_EXIT:
                        *((PM_Result*)pData) = (PM_Result)MB_Command.u8Parameters[1];
                        gPmPWMAckFlags &= (PM_PWM_AckFlags)(~E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_EXIT);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return mbxResult;
}
//-------------------------------------------------------------------------------------------------
/// PM PWM MailBox Handler
/// @param  pData           \b IN: mailbox data configuration
/// @param  pbAddToQueue    \b IN: add to queue
/// @return @ref MBX_Result
//-------------------------------------------------------------------------------------------------
static void _PM_PWM_MailBoxCallbackHandler( MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    MS_U32 u32Memoryadrr =0 ;
    MS_U16 u16datalen =0;
    MS_U8 i=0;

    PM_DBG_FUNC();

    *pbAddToQueue = FALSE; //no need to add to queue

    if(!pMsg) return;

    if(pMsg->u8MsgClass != E_MBX_CLASS_PM_PWM_NOWAIT)
    {
        PM_DBG_ERR("Invalid Message Class!\n");
        return;
    }

    // check control type
    if ((pMsg->u8Ctrl != 4) && (pMsg->u8Ctrl != 5))
    {
        PM_DBG_ERR("Control: 0x%02x\n", pMsg->u8Ctrl);
        PM_DBG_INFO("Not Implemented!\n");
        return;
    }
    PM_DBG_INFO("Get IR record command: 0x%02x\n", pMsg->u8Index);
    PM_DBG_INFO("Parameter[0]=%d\n",  pMsg->u8Parameters[0]);
    PM_DBG_INFO("Parameter[1]=%d\n",  pMsg->u8Parameters[1]);
    PM_DBG_INFO("Parameter[2]=%d\n",  pMsg->u8Parameters[2]);
    PM_DBG_INFO("Parameter[3]=%d\n",  pMsg->u8Parameters[3]);
    PM_DBG_INFO("Parameter[4]=%d\n",  pMsg->u8Parameters[4]);
    PM_DBG_INFO("Parameter[5]=%d\n",  pMsg->u8Parameters[5]);

    for(i=0; i<4; i++)
    {
        u32Memoryadrr = (u32Memoryadrr<<8) | (MS_U8)(pMsg->u8Parameters[i]);
    }
    for(i=0; i<2; i++)
    {
        u16datalen = (u16datalen<<8)|(MS_U8)(pMsg->u8Parameters[i+4]);
    }
    if (pMsg->u8Index == PM_CMDIDX_PWM_IRRECORD_RECEIVED_COMPLETE)
    {
        if(gfIRRecordCallback)
        {
            gfIRRecordCallback(u32Memoryadrr, u16datalen);
        }
    }

    return;
}

//-------------------------------------------------------------------------------------------------
/// PM  PWM driver initialize
/// @param  u8PWM  \b IN: PM pwm initialize configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_Init(MS_U8 u8PWM)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_VIRT virtSrcAddr;

    MS_U32 MBX_Resendcnt = 0;

    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmPWMAckFlags |= E_PM_PWM_ACKFLG_WAIT_INIT;


    mbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_PWM_WAIT,PM_MBX_QUEUESIZE);
    if(mbxResult!=E_MBX_SUCCESS)
    {
        PM_DBG_ERR("Mailbox registered fails!\n");
        return E_PM_FAIL;
    }
    virtSrcAddr = (MS_VIRT)u8PWM;
    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_PWM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_PWM_INIT;
        MB_Command.u8ParameterCount = 1;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr&0x000000FFUL);

        mbxResult = MDrv_MBX_SendMsg(&MB_Command);
        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }

    // Waiting for PM51 PWM  initialization done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_PWM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
                //Error Handling here:
                return E_PM_FAIL;
          }
       }
    }
    while((gPmPWMAckFlags & E_PM_PWM_ACKFLG_WAIT_INIT)
           &&(mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PWM_Init Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PWM_Init OK\n");
    }

    return PmResult;

}


//-------------------------------------------------------------------------------------------------
/// PM  PWM driver config
/// @param  pPmPWMCfg   \b IN: PM PWM configuration
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_Config(PWMSimIR_CFG *pPWMSimIRCfg)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;

    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmPWMAckFlags |= E_PM_PWM_ACKFLG_WAIT_CONF;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_PWM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_PWM_CONFIG;
        MB_Command.u8ParameterCount = 8;
        MB_Command.u8Parameters[0] =  (MS_U8)pPWMSimIRCfg->u8IREncodeMode;
        MB_Command.u8Parameters[1] =  (MS_U8)pPWMSimIRCfg->u8Customercode1;
        MB_Command.u8Parameters[2] =  (MS_U8)pPWMSimIRCfg->u8Customercode2;
        MB_Command.u8Parameters[3] =  (MS_U8)pPWMSimIRCfg->u8IRKeycode;
        MB_Command.u8Parameters[4] =  (MS_U8)pPWMSimIRCfg->u8IRRepeat;
        MB_Command.u8Parameters[5] =  (MS_U8)pPWMSimIRCfg->u8Reserved0;
        MB_Command.u8Parameters[6] =  (MS_U8)pPWMSimIRCfg->u8Reserved1;
        MB_Command.u8Parameters[7] =  (MS_U8)pPWMSimIRCfg->u8Reserved2;
        mbxResult = MDrv_MBX_SendMsg(&MB_Command);

        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }
    // Waiting for PM51 PWM  config done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_PWM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
                //Error Handling here:
                return E_PM_FAIL;
          }
       }
    }
    while((gPmPWMAckFlags & E_PM_PWM_ACKFLG_WAIT_CONF)
           &&(mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PWM_Config Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PWM_Config  OK\n");
    }

    return PmResult;

}


//-------------------------------------------------------------------------------------------------
/// IR record initial
/// @param None
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Init(void)
{
    MBX_Result MbxResult = E_MBX_UNKNOW_ERROR;

    PM_DBG_INFO(" ........MDrv_IR_Record_Init ....\n");
    MbxResult = MDrv_MBX_RegisterMSGWithCallBack(E_MBX_CLASS_PM_PWM_NOWAIT,PM_MBX_QUEUESIZE,_PM_PWM_MailBoxCallbackHandler);
    if(MbxResult!=E_MBX_SUCCESS)
    {
        PM_DBG_ERR("Mailbox registered with callback fails!\n");
        return E_PM_FAIL;
    }

    MbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_PWM_WAIT,PM_MBX_QUEUESIZE);
    if(MbxResult != E_MBX_SUCCESS)
    {
        PM_DBG_ERR("Mailbox registered fails!\n");
        return E_PM_FAIL;
    }

     return E_PM_OK;

}
//-------------------------------------------------------------------------------------------------
/// Set PM Recieved IR Pulse require
/// @param  u32BufferAddr        \b IN: Buffer Address
/// @param  u16BufferSize        \b IN: Buffer Size
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Receive_Require(MS_U32 u32BufferAddr,MS_U16 u16BufferSize)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;
    MS_VIRT virtSrcAddr = 0;

    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    virtSrcAddr = (MS_VIRT)u32BufferAddr;

    gPmPWMAckFlags |= E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_REQUIRE;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_PWM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_PWM_IRRECORD_RECEIVED_REQUIRE;
        MB_Command.u8ParameterCount = 6;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);
        MB_Command.u8Parameters[4] =  (MS_U8)(u16BufferSize>>8);
        MB_Command.u8Parameters[5] =  (MS_U8)(u16BufferSize&0x00FFUL);


        mbxResult = MDrv_MBX_SendMsg(&MB_Command);

        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }
    // Waiting for PM51 Ir Record Recieve Require done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_PWM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
                //Error Handling here:
                return E_PM_FAIL;
          }
       }
    }
    while((gPmPWMAckFlags & E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_REQUIRE)
           &&(mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PWM_IRRecord_Receive_Require Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PWM_IRRecord_Receive_Require  OK\n");
    }

    return PmResult;

}



//-------------------------------------------------------------------------------------------------
/// SN Set PM Recieved IR Pulse Complete and save to file
/// @param  u32BufferAddr   \b IN: Buffer Address
/// @param  u32BuferrSize   \b IN: Buffer Size
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Receive_Complete( MS_U32 u32BufferAddr,MS_U16 u16BufferSize)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;
    MS_VIRT virtSrcAddr = 0;

    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    virtSrcAddr = (MS_VIRT)u32BufferAddr;

    gPmPWMAckFlags |= E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_COMPLETE;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_PWM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_PWM_IRRECORD_RECEIVED_COMPLETE;
        MB_Command.u8ParameterCount = 6;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);
        MB_Command.u8Parameters[4] =  (MS_U8)(u16BufferSize>>8);
        MB_Command.u8Parameters[5] =  (MS_U8)(u16BufferSize&0x00FFUL);


        mbxResult = MDrv_MBX_SendMsg(&MB_Command);

        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }
    // Waiting for PM51 Ir Record Recieve complete done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_PWM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
                //Error Handling here:
                return E_PM_FAIL;
          }
       }
    }
    while((gPmPWMAckFlags & E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_COMPLETE)
           &&(mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PWM_IRRecord_Receive_Complete Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PWM_IRRecord_Receive_Complete  OK\n");
    }

    return PmResult;

}

//-------------------------------------------------------------------------------------------------
/// send PM to require for Ir pluse PWM transmit
/// @param  u32BufferAddr   \b IN: Buffer Address
/// @param  u32BuferrSize   \b IN: Buffer Size
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Transmit(MS_U32 u32BufferAddr,MS_U16 u16BufferSize)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;
    MS_VIRT virtSrcAddr = 0;

    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    virtSrcAddr = (MS_VIRT)u32BufferAddr;

    gPmPWMAckFlags |= E_PM_PWM_ACKFLG_WAIT_IRRECORD_TRANSMIT;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_PWM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_PWM_IRRECORD_TRANSMIT;
        MB_Command.u8ParameterCount = 6;
        MB_Command.u8Parameters[0] =  (MS_U8)(virtSrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(virtSrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(virtSrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(virtSrcAddr&0x000000FFUL);
        MB_Command.u8Parameters[4] =  (MS_U8)(u16BufferSize>>8);
        MB_Command.u8Parameters[5] =  (MS_U8)(u16BufferSize&0x00FFUL);


        mbxResult = MDrv_MBX_SendMsg(&MB_Command);

        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }
     // Waiting for PM51 Ir Record Transmit done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_PWM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
                //Error Handling here:
                return E_PM_FAIL;
          }
       }
    }
    while((gPmPWMAckFlags & E_PM_PWM_ACKFLG_WAIT_IRRECORD_TRANSMIT)
           &&(mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PWM_IRRecord_TRANSMIT Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PWM_IRRecord_TRANSMIT  OK\n");
    }

    return PmResult;

}

//-------------------------------------------------------------------------------------------------
/// set IR record call back function
/// @param  pCallback   \b IN: Ir Record call back funtion
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_SetCallBackFunction(IRRecord_Callback pCallback)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;
    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gfIRRecordCallback  = pCallback;
    gPmPWMAckFlags |= E_PM_PWM_ACKFLG_WAIT_IRRECORD_SETCALLBACK;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_PWM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_PWM_IRRECORD_RECEIVED_SETCALLBACK;
        MB_Command.u8ParameterCount = 0;
        mbxResult = MDrv_MBX_SendMsg(&MB_Command);

        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }
     // Waiting for PM51 Ir Record set call back  done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_PWM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
                //Error Handling here:
                return E_PM_FAIL;
          }
       }
    }
    while((gPmPWMAckFlags & E_PM_PWM_ACKFLG_WAIT_IRRECORD_SETCALLBACK)
           &&(mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PWM_IRRecord_SetCallBackFunction Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PWM_IRRecord_SetCallBackFunction  OK\n");
    }

    return PmResult;

}


//-------------------------------------------------------------------------------------------------
/// set IR record Receive Exit
/// @param  none
/// @return @ref PM_Result
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Receive_Exit(void)
{
    PM_Result PmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;

    MS_U32 MBX_Resendcnt = 0;
    _MDrv_PM_DummyMiuFifo();

    PM_DBG_FUNC();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gPmPWMAckFlags |= E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_EXIT;

    // send to PM51
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_PWM_NOWAIT;
        MB_Command.u8Index = PM_CMDIDX_PWM_IRRECORD_RECEIVED_EXIT;
        MB_Command.u8ParameterCount = 0;
        mbxResult = MDrv_MBX_SendMsg(&MB_Command);

        while(E_MBX_SUCCESS != mbxResult)
        {
            //Error Handling here:
            return E_PM_FAIL;
        }
    }
    // Waiting for PM51 Ir Record RecieveExit done
    do
    {
       if(MBX_Resendcnt<10)
       {
        mbxResult = _MDrv_PM_PWM_MailBoxHandler(&PmResult);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          mbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != mbxResult)
          {
                //Error Handling here:
                return E_PM_FAIL;
          }
       }
    }
    while((gPmPWMAckFlags & E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_EXIT)
           &&(mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        PmResult = E_PM_FAIL;
        PM_DBG_ERR("MDrv_PM_PWM_IRRecord_Receive_Exit Fail\n");
    }
    else
    {
        PmResult = E_PM_OK;
        PM_DBG_INFO("MDrv_PM_PWM_IRRecord_Receive_Exit  OK\n");
    }

    return PmResult;

}
////////////////////////////////////////////////////////////////////////////////
/// @param bTrigger      \b In: bTrigger
////////////////////////////////////////////////////////////////////////////////
PM_Result MDrv_PM_ActiveStandbyMode(MS_BOOL bTrigger)
{
    MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base fail!\n");
        return E_PM_FAIL;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);
    HAL_PM_SetBackGroundTrigger(bTrigger);
    return E_PM_OK;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
PM_Result MDrv_PM_IsActiveStandbyMode(MS_BOOL *bActive)
{
    (*bActive) = FALSE;
    MS_VIRT virtBaseAddr;
    MS_PHY  u32BaseSize;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base fail!\n");
        return E_PM_FAIL;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);
    (*bActive) = HAL_PM_GetBackGroundActive();
    return E_PM_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// Set GPIO4 for standby PIN
/// @param  bOn   \b IN: enable set gpio high ,disable set gpio Low
/// @return @ref PM_Result
////////////////////////////////////////////////////////////////////////////////
PM_Result MDrv_PM_GPIO4_SetPower(MS_BOOL bOn)
{
    MS_VIRT virtBaseAddr=0;
    MS_PHY  u32BaseSize=0;
    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize,MS_MODULE_PM))
    {
        PM_DBG_ERR("Get IOMAP Base fail!\n");
        return E_PM_FAIL;
    }

    HAL_PM_SetIOMapBase(virtBaseAddr);
    HAL_PM_GPIO4_SetPower(bOn);
    return E_PM_OK;
}

//-------------------------------------------------------------------------------------------------
/// MDrv_PM_RunTimePM_Disable_PassWord
/// @param  none
/// @return  none
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RunTimePM_Disable_PassWord(void)
{
    MS_U16 u16Val = 0;
    MS_VIRT virtBaseAddr = 0;
    MS_PHY  u32BaseSize = 0;

    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
    {
        PM_DBG_ERR("%s Get IOMAP Base failed!\n", __FUNCTION__);
        return;
    }
    HAL_PM_SetIOMapBase(virtBaseAddr);
    PM_DBG_STR(ULOGD(TAG_PM, "[RT_PM] Start Write PM Run Time PassWord!\n"));
    PM_DBG_STR(ULOGD(TAG_PM, "[Send] 0x0EAA = %x\n",HAL_PM_Read2Byte(0x0EAAUL)));
    HAL_PM_SetScratchPad(4,0xBABEUL);
    PM_DBG_STR(ULOGD(TAG_PM, "[End] 0x0EAA = %x\n",HAL_PM_Read2Byte(0x0EAAUL)));
    PM_DBG_STR(ULOGD(TAG_PM, "Wait for PM51 Ideal ...........\n"));
    do
    {
        if(HAL_PM_GetScratchPad(4,&u16Val) == FALSE)
        {
            continue;
        }
    }
    while(u16Val != 0xAAAAUL);
    PM_DBG_STR(ULOGD(TAG_PM, "[RT_PM] End  Write PM Run Time PassWord!\n"));
}

//-------------------------------------------------------------------------------------------------
/// MDrv_PM_STR_CheckFactoryPowerOnModePassword
/// @param  none
/// @return  none
//-------------------------------------------------------------------------------------------------
void MDrv_PM_STR_CheckFactoryPowerOnModePassword(void)
{
    PM_DBG_STR(ULOGD(TAG_PM, "[PM] MDrv_PM_STR_CheckFactoryPowerOnMode\n"));
    HAL_PM_SetScratchPad(4,0xBBBBUL);
    PM_DBG_STR(ULOGD(TAG_PM, "[PM] STR memory factory need AC ON\n"));
}

//-------------------------------------------------------------------------------------------------
/// MDrv_PM_STR_CheckFactoryPowerOnModePassword
/// @param  TRUE or FALSE
/// @return  none
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_STR_CheckFactoryPowerOnMode_Second(MS_BOOL bCheck)
{
    MS_U16 u16Val = 0;
    MS_BOOL bRet;
    if(bCheck)
    {
       bRet=HAL_PM_GetScratchPad(4,&u16Val);
       if((u16Val==0xCCCCUL)||(bRet == FALSE))
           return E_PM_FAIL;
       else
           return E_PM_OK;
    }
    else
       return E_PM_OK;
}


//-------------------------------------------------------------------------------------------------
/// MDrv_PM_STR_CheckFactoryPowerOnModePassword
/// @param  TRUE or FALSE
/// @return  none
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_Check_Version(MS_BOOL bCheck)
{
    MS_U8 u8Val = 0;
    MS_BOOL bRet;
    if(bCheck)
    {
       bRet=HAL_PM_GetVersion(&u8Val);
       if((u8Val==0x20UL)&&(bRet == TRUE))
           return E_PM_FAIL;
       else
           return E_PM_OK;
    }
    else
       return E_PM_OK;
}

MS_U16 MDrv_PM_GetSRAMSize(void)
{   
    return HAL_PM_GetSRAMSize();

}

MS_U8 MDrv_PM_GetIRPowerOnKey(void)
{
    return HAL_PM_GetIRPowerOnKey();
}

void MDrv_PM_GetRT51Status(void)
{
    HAL_PM_GetRT51Status();
}

#ifdef MSOS_TYPE_LINUX
//-------------------------------------------------------------------------------------------------
///
/// @param
/// @return
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_IO_INIT(void)
{
    if (_s32FdPM < 0)   //First time open
    {
        _s32FdPM = open("/dev/pm", O_RDWR);
        if (_s32FdPM < 0)
        {
            return E_PM_FAIL;
        }
        else
        {
            return E_PM_OK;
        }
    }
    return E_PM_FAIL;
}

//-------------------------------------------------------------------------------------------------
///
/// @param
/// @return
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_WriteDramInfo_Code(MS_U32 u32Addr, MS_U32 u32Size)
{
    MS_U32 temp[2];

    temp[0] = u32Addr;
    temp[1] = u32Size;

    if (ioctl(_s32FdPM, IOCTL_PM_SET_DRAM_CODE_ADDRESS, &temp))
    {
        return E_PM_OK;
    }
    else
    {
        return E_PM_FAIL;
    }
}

//-------------------------------------------------------------------------------------------------
///
/// @param
/// @return
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_WriteDramInfo_Data(MS_U32 u32Addr, MS_U32 u32Size)
{
    MS_U32 temp[2];

    temp[0] = u32Addr;
    temp[1] = u32Size;

    if (ioctl(_s32FdPM, IOCTL_PM_SET_DRAM_DATA_ADDRESS, &temp))
    {
        return E_PM_OK;
    }
    else
    {
        return E_PM_FAIL;
    }
}
#endif