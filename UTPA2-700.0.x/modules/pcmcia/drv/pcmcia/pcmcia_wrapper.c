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

///////////////////////////////////////////////////////////////////////////////
///
/// file    pcmcia_wrapper.c
/// @brief  PCMCIA wrapper for backward compatible
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
/*                       Header Files                                        */
/*****************************************************************************/
#include "MsCommon.h"
#include "utopia.h"
#include "drvPCMCIA.h"
#include "pcmcia.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif


/*****************************************************************************/
/*                       Define                                              */
/*****************************************************************************/

/*****************************************************************************/
/*                       Global Variables                                    */
/*****************************************************************************/
void* pInstant = NULL;
/*****************************************************************************/
/*                       Macro                                               */
/*****************************************************************************/

/*****************************************************************************/
/*                       Local Variables                                     */
/*****************************************************************************/

/*****************************************************************************/
/*                       Local Functions                                     */
/*****************************************************************************/

/*****************************************************************************/
/*                       Global Functions                                    */
/*****************************************************************************/

void MDrv_PCMCIA_Init(MS_BOOL bCD_Reverse)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_CI, &pInstant, 0, NULL))
        {
            ULOGE("PCMCIA", "%s, open MODULE_PCMCIA failed\n", __FUNCTION__);
            return;
        }
    }
    
    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Init, &bCD_Reverse);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

void MDrv_PCMCIA_Exit( void )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
        return;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Exit, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s][%d]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
    }

    u32Ret = UtopiaClose(pInstant);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s][%d]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
    }
    pInstant = NULL;
}

MS_BOOL MDrv_PCMCIA_DetectV2( PCMCIA_MODULE eModule )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_DETECT stPcmDetect;

    if(NULL == pInstant)
        return FALSE;

    stPcmDetect.eModule = eModule;
    stPcmDetect.bDetected = FALSE;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Detect, &stPcmDetect);

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return stPcmDetect.bDetected;
    }
}

MS_BOOL MDrv_PCMCIA_PollingV2( PCMCIA_MODULE eModule )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_POLL stPcmPoll;

    if(NULL == pInstant)
        return FALSE;

    stPcmPoll.eModule = eModule;
    stPcmPoll.bChanged = FALSE;

    UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Polling, &stPcmPoll);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return stPcmPoll.bChanged;
    }
}

void MDrv_PCMCIA_Set_HW_ResetDuration( MS_U8 u8HW_ResetDuration )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
        return;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Set_HW_ResetDuration, &u8HW_ResetDuration);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

void MDrv_PCMCIA_ResetHW_V2( PCMCIA_MODULE eModule)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
        return;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_ResetHW, &eModule);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

}

void MDrv_PCMCIA_WriteAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_WRITE_ATTRIB_MEM stWrite;
    if(NULL == pInstant)
        return;

    stWrite.eModule = eModule;
    stWrite.u16Addr = u16Addr;
    stWrite.u8Value = u8Value;
    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_WriteAttribMem, &stWrite);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

void MDrv_PCMCIA_ReadAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 *pDest )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_READ_ATTRIB_MEM stRead;
    if(NULL == pInstant)
        return;

    stRead.eModule = eModule;
    stRead.u16Addr = u16Addr;
    stRead.u8Data = (MS_U8)-1;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_ReadAttribMem, &stRead);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        *pDest = 0xFF;
    }
    else
    {
        *pDest = stRead.u8Data;
    }
}

void MDrv_PCMCIA_WriteIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_WRITE_IO_MEM stWrite;
    if(NULL == pInstant)
        return;

    stWrite.eModule = eModule;
    stWrite.u16Addr = u16Addr;
    stWrite.u8Value = u8Value;
    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_WriteIOMem, &stWrite);

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

void MDrv_PCMCIA_WriteIOMemLongV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    PCM_WRITE_IO_MEM_LONG stWrite;
    if(NULL == pInstant)
        return;

    stWrite.eModule = eModule;
    stWrite.u16Addr = u16Addr;
    stWrite.u8Value = u8Value;
    stWrite.u16DataLen = u16DataLen;
    stWrite.pu8WriteBuffer = u8pWriteBuffer;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_WriteIOMemLong, &stWrite);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

MS_U8 MDrv_PCMCIA_ReadIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_READ_IO_MEM stRead;
    if(NULL == pInstant)
        return (MS_U8)-1;

    stRead.eModule = eModule;
    stRead.u16Addr = u16Addr;
    stRead.u8Data = (MS_U8)-1;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_ReadIOMem, &stRead);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return stRead.u8Data;
    }
}

MS_BOOL MDrv_PCMCIA_IsModuleStillPluggedV2( PCMCIA_MODULE eModule )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_MODULE_PLUGIN stPlug;
	memset(&stPlug, 0, sizeof(PCM_MODULE_PLUGIN));
    if(NULL == pInstant)
        return FALSE;

    stPlug.eModule = eModule;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_IsModuleStillPlugged, &stPlug);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return stPlug.bPlugIn;
    }
}

void MDrv_PCMCIA_Set_Detect_Trigger( MS_BOOL bHighActive )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
        return;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Set_Detect_Trigger, &bHighActive);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

void MDrv_PCMCIA_Set_Detect_Enable( MS_BOOL bEnable )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
        return;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Set_Detect_Enable, &bEnable);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

MS_U32 MDrv_PCMCIA_Get_CD_IntervalV2( PCMCIA_MODULE eModule )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    PCM_GET_CD_INTERVAL stInterval;
    if(NULL == pInstant)
        return (MS_U32)-1;

    stInterval.eModule = eModule;
    stInterval.u32Interval = (MS_U32)-1;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Get_CD_Interval, &stInterval);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return (MS_U32)-1;
    }
    else
    {
        return stInterval.u32Interval;
    }
}

void MDrv_PCMCIA_ParseAttribMem( MS_U8 *pAttribMem, MS_U16 dwLen, PCMCIA_INFO *pInfo )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_PARSE_ATTRIBUTE stParse;
    if(NULL == pInstant)
        return;

    stParse.pu8AttribMem = pAttribMem;
    stParse.u16DwLen = dwLen;
    stParse.pInfo = pInfo;
    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_ParseAttribMem, &stParse);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

void MDrv_PCMCIA_SetCommandBitV2( PCMCIA_MODULE eModule, MS_U8 u8CommandBit, MS_BOOL bValue )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_SETCOMMANDBIT stCmdBit;
    if(NULL == pInstant)
        return;

    stCmdBit.eModule = eModule;
    stCmdBit.u8CmdBit = u8CommandBit;
    stCmdBit.bValue = bValue;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_SetCommandBit, &stCmdBit);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

MS_BOOL MDrv_PCMCIA_ResetInterfaceV2( PCMCIA_MODULE eModule )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_RESET_INTERFACE stRst;
    if(NULL == pInstant)
        return FALSE;

    stRst.eModule = eModule;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_ResetInterface, &stRst);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_BOOL MDrv_PCMCIA_IsDataAvailableV2( PCMCIA_MODULE eModule )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_DATA_AVAILABLE stData;
    if(NULL == pInstant)
        return FALSE;

    stData.eModule = eModule;
    stData.bDA = FALSE;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_IsDataAvailable, &stData);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return stData.bDA;
    }
}

MS_U16 MDrv_PCMCIA_ReadDataV2( PCMCIA_MODULE eModule, MS_U8* u8pReadBuffer, MS_U16 u16ReadBufferSize )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_READ_DATA stRead;
    if(NULL == pInstant)
        return (MS_U16)-1;

    stRead.eModule              = eModule;
    stRead.pu8ReadBuffer        = u8pReadBuffer;
    stRead.u16ReadBufferSize    = u16ReadBufferSize;
    stRead.u16SizeRead          = 0;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_ReadData, &stRead);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }

    return stRead.u16SizeRead;
}

MS_BOOL MDrv_PCMCIA_WriteDataV2( PCMCIA_MODULE eModule, MS_U8* u8pWriteBuffer, MS_U16 u16DataLen )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_WRITE_DATA stWrite;
    if(NULL == pInstant)
        return FALSE;

    stWrite.eModule         = eModule;
    stWrite.pu8WriteBuffer  = u8pWriteBuffer;
    stWrite.u16DataLen      = u16DataLen;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_WriteData, &stWrite);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_BOOL MDrv_PCMCIA_SwitchToIOmodeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_SWITCH_TO_IOMODE stCmd;
    if(NULL == pInstant)
        return FALSE;

    stCmd.eModule   = eModule;
    stCmd.pInfo     = pInfo;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_SwitchToIOmode, &stCmd);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_U16 MDrv_PCMCIA_NegotiateBufferSizeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_NETOTIAGE_BUFSIZE stArgs;
    if(NULL == pInstant)
        return (MS_U16)-1;

    stArgs.eModule      = eModule;
    stArgs.pInfo        = pInfo;
    stArgs.u16BufSize   = (MS_U16)-1;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_NegotiateBufferSize, &stArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return 0xFF;
    }
    else
    {
        return stArgs.u16BufSize;
    }
}

void MDrv_PCMCIA_WriteBufferSizeV2( PCMCIA_MODULE eModule, MS_U16 u16BufferSize )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_WRITE_BUF_SIZE stArgs;
    if(NULL == pInstant)
        return;

    stArgs.eModule          = eModule;
    stArgs.u16BufferSize    = u16BufferSize;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_WriteBufferSize, &stArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

MS_BOOL MDrv_PCMCIA_GetLibVer( const MSIF_Version **ppVersion )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(NULL == pInstant)
        return FALSE;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_GetLibVer, ppVersion);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
    return TRUE;
}

void MDrv_PCMCIA_Enable_InterruptV2( PCMCIA_MODULE eModule, MS_BOOL bEnable )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_ENABLE_INT stArgs;
    if(NULL == pInstant)
        return;

    stArgs.eModule  = eModule;
    stArgs.bEnable  = bEnable;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Enable_Interrupt, &stArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

void MDrv_PCMCIA_Set_InterruptStatusV2( PCMCIA_MODULE eModule, MS_BOOL Status )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_SET_INTR_STATUS stArgs;
    if(NULL == pInstant)
        return;

    stArgs.eModule = eModule;
    stArgs.bStatus = Status;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Set_InterruptStatus, &stArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

MS_BOOL MDrv_PCMCIA_Get_InterruptStatusV2( PCMCIA_MODULE eModule )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_GET_INT_STATUS stArgs;
    if(NULL == pInstant)
        return FALSE;

    stArgs.eModule = eModule;
    stArgs.bIntStatus = FALSE;
    
    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_Get_InterruptStatus, &stArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return stArgs.bIntStatus;
    }
}

void MDrv_PCMCIA_InstarllIsrCallbackV2( PCMCIA_MODULE eModule, IsrCallback fnIsrCallback )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_INSTCB stArgs;
    if(NULL == pInstant)
        return;

    stArgs.eModule = eModule;
    stArgs.pfcb    = fnIsrCallback;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_InstallIsrCallback, &stArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
    }
}

MS_BOOL MDrv_PCMCIA_WaitForStatusBitV2( PCMCIA_MODULE eModule, MS_U8 u8StatusBit )
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    PCM_WAIT_STATUS stArgs;
    if(NULL == pInstant)
        return FALSE;

    stArgs.eModule      = eModule;
    stArgs.u8StatusBit  = u8StatusBit;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_WaitForStatusBit, &stArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_U32 MDrv_PCMCIA_SetPowerState(EN_POWER_MODE u16PowerState)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
        return FALSE;

    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_SetPowerState, (void* const)&u16PowerState);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return TRUE;
    }    
}

MS_BOOL MDrv_PCMCIA_GetConfig(MS_BOOL* bCD_Reverse)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(NULL == pInstant)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_CI, &pInstant, 0, NULL))
        {
            ULOGE("PCMCIA", "%s, open MODULE_PCMCIA failed\n", __FUNCTION__);
            return FALSE;
        }
    }
    
    u32Ret = UtopiaIoctl(pInstant, E_MDRV_CMD_PCM_GetConfig, bCD_Reverse);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}