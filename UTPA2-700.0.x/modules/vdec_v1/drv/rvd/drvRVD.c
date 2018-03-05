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
/// file    drvRVD.c
/// @brief  RVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "regRVD.h"
//load H/w Setting
////////////////
#include "osalRVD.h"
#include "halRVD.h"
#include "drvRVD.h"
// Internal Definition
#include "drvMMIO.h"

#include "drvBDMA.h"
#define dmacpy( DESTADDR, SRCADDR , LEN)   MDrv_BDMA_CopyHnd((MS_PHYADDR)(SRCADDR), (MS_PHYADDR)(DESTADDR), (LEN), E_BDMA_SDRAM2SDRAM1, BDMA_OPCFG_DEF)
#define BDMAcpy(DESTADDR, SRCADDR, LEN , Flag)   MDrv_BDMA_CopyHnd((MS_PHYADDR)(SRCADDR), (MS_PHYADDR)(DESTADDR), (LEN), (Flag), BDMA_OPCFG_DEF)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
//#define __TESTCODE__

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if RVD_ENABLE_EMBEDDED_FW_BINARY
static MS_U8 VLC_TAB[] =
{
    #include "fwRVDvlcmem.dat"
};
#endif

#define USING_ID_LINK 1
#define QMEM_BBU_TABLE_SIZE 0x4000
#define RVD_BBU_DEPTH 1600
#define BYTECOUNT_LIMIT 16777216 // 2^24
#define RVD_ISR_ENABLE 0 //Maybe used at Build broken index problem

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
//Version
//#define RVD_DRV_VERSION     "xx.xxx.xxxx"
#define RVD_FW_CMD_TIMEOUT_DEFAULT    100      ///<Check FW cmd timeout threshold unit: ms
//extern MS_U32 eRvdDynCfgByChip;                ///<Dynamic scaling config bit (Got from hal layer)
#define RVD_CTRL_INIT_FINISHED        BIT(0)   ///<RVD init status :Finish
#define RVD_CTRL_PROCESSING           BIT(1)   ///<RVD init status :processing
#define RVD_CTRL_DATA_END             BIT(2)   ///< RVD init status : data end
#define RVD_CTRL_DISPLAY_CTRL         BIT(3)   ///< RVD init status :Set Disp ctrl mode
#define RVD_CTRL_DISP_INFO_RDY        BIT(4)   ///< RVD init status Get Disp Info Rdy
#define RVD_CTRL_CODE_MIU_1           BIT(5)   ///< RVD init status :Open Bin Addr in MIU1
#define RVD_CTRL_ES_MIU_1             BIT(6)   ///< RVD init status :ES buffer in MIU1

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
/// Version string
static MSIF_Version _drv_rvd_version = {
    .DDI = { RVD_DRV_VERSION, },
};
// Used for F/W Qmem exchange
static MS_U32 gu32QmemExchangeInfoAddr;
static MS_U32 gu32QmemBbuAddr = 0;
static RVD_DrvInfo DrvRVDInfo;
static MS_U8 u8RVD_DbgLevel = E_RVD_DEBUG_NONE;
static MS_U32 u32RVDCtrlMode = 0;
static MS_U32 u32StepPlayCnt = 0; // check if step play done
static MS_U32 u32StepDecodeCnt = 0; // check if step play done
static MS_U32 u32BytePos = 0;
static MS_U16 u16QmemBbuWptr = 0;
MS_U32 u32FWBinAddr = 0;
static MS_BOOL bRVDIsInit = FALSE;
static RVD_ISR_Ctrl gRVDISRCtrl = { FALSE  , 0  , NULL };
RVD_MEMCfg *MEMCfgRst;
RVD_FileInfo *FileInfoRst;
RVD_MiuInfo gRVDInfo;
extern MS_U32 u32SVDRegOSBase;
extern MS_U32 MsOS_GetSystemTime(void);

static MS_BOOL gbEnableRVTurboFWMode = FALSE;

#define RVD_ENABLE_TIME_MEASURE 0
#if RVD_ENABLE_TIME_MEASURE
#define RVD_GetSysTime_ms()    MsOS_GetSystemTime()
MS_U32 u32RvdInitSysTimeBase = 0;
#endif

#define MW_RVD_BROKEN_BY_US   0x00800000
#define RVD_FW_IDLE_THRESHOLD 100
#define RVD_GetSystemTime  MsOS_GetSystemTime
#define RVD_VLC_TABLE_SIZE 0x20000

/************* RVD debug define *************/
#define RVD_MSG_INF(format, args...)  do{if(MDrv_RVD_IsDbgEnable(E_RVD_DEBUG_INF)) printf(format, ##args);}while(0) ///< output general message
#define RVD_MSG_DRV(format, args...)  do{if(MDrv_RVD_IsDbgEnable(E_RVD_DEBUG_DRV)) printf(format, ##args);}while(0)///< output drv layer message
#define RVD_MSG_HAL(format, args...)  do{if(MDrv_RVD_IsDbgEnable(E_RVD_DEBUG_HAL)) printf(format, ##args);}while(0)///< output hal layer message
#define RVD_MSG_ERR(format, args...)  do{if(MDrv_RVD_IsDbgEnable(E_RVD_DEBUG_ERR)) printf(format, ##args);}while(0)///< Only output error message

#define _DRV_RVD_Entry()      { u32RVDCtrlMode |=  RVD_CTRL_PROCESSING; }
#define _DRV_RVD_Return(_ret) { u32RVDCtrlMode &= (~RVD_CTRL_PROCESSING); return _ret; }

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetBBU_ID()
/// @brief \b Function \b Description:  Push the BBU Table ID in F/W remapping area
/// @param  -u32Address \b IN  F/W RISC AREA
/// @param  -u32Byte_Pos \b IN BBU Byte Position
/// @param  -u32ID_High \b IN BBU ID High Address
/// @param  -u32ID_Low \b IN BBU ID Low Address
//-----------------------------------------------------------------------------
void MDrv_RVD_SetBBU_ID(MS_U32 u32Address,MS_U32 u32Byte_Pos, MS_U32 u32ID_High, MS_U32 u32ID_Low)
{
     HAL_RVD_MemWrite( u32Address,u32Byte_Pos);
     HAL_RVD_MemWrite( (u32Address+4),u32ID_Low);
     HAL_RVD_MemWrite( (u32Address+8),u32ID_High);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetBBU_OffsetLength()
/// @brief \b Function \b Description:  Push the offset and length in F/W remapping area
/// @param  -u32Address \b IN  F/W RISC AREA
/// @param  -u32offset \b IN BBU offet
/// @param  -u32length \b IN BBU length
//-----------------------------------------------------------------------------
void MDrv_RVD_SetBBU_OffsetLength(MS_U32 u32Address,MS_U32 u32offset, MS_U32 u32length)
{
     HAL_RVD_MemWrite( u32Address,u32offset);
     HAL_RVD_MemWrite( (u32Address+4),u32length);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetBBUStartAddr()
/// @brief \b Function \b Description:  Get  BBU Statr address
/// @return - BBU Statr address
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetBBUStartAddr(void)
{
    return  HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_OFFSET_BBU_ADDR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetDecErrCnt()
/// @brief \b Function \b Description:  Get  BBU Statr address
/// @return - BBU Statr address
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetDecErrCnt(void)
{
    return  HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_DECERRCNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetDataErrCnt()
/// @brief \b Function \b Description:  Get Data Error Count
/// @return - Data Error count
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetDataErrCnt(void)
{
    return  HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_DATAERRCNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetSkipCnt()
/// @brief \b Function \b Description:  Get Frame Skip Count
/// @return - Skip frame number
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetSkipCnt(void)
{
    return  HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_SKIP_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetDropCnt()
/// @brief \b Function \b Description:  Get Frame drop Count
/// @return - Drop frame number
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetDropCnt(void)
{
    return  HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_DROP_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetDispQueNum()
/// @brief \b Function \b Description:  Get Display queue number
/// @return - Display queue number
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetDispQueNum(void)
{
    return  HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_DISP_QUE_NUM);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetBBUDepth()
/// @brief \b Function \b Description:  Set vaild BBU Queue depth.
/// @param  -u32Rptr \b IN from MDrv_RVD_GetQMEM_Info
//-----------------------------------------------------------------------------
void MDrv_RVD_SetBBUDepth(MS_U32 u32Rptr)
{
     HAL_RVD_QMemWrite((gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_BBU_DEPTH), u32Rptr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetBBUDepth()
/// @brief \b Function \b Description:  Set vaild BBU Queue depth.
/// @param  -u32Rptr \b IN from MDrv_RVD_GetQMEM_Info
//-----------------------------------------------------------------------------
void MDrv_RVD_SetPBInfo(MS_PHYADDR u32Addr , MS_U32 u32Size)
{
    if(u32Size)
    {
        HAL_RVD_QMemWrite((gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DUMMY_OFFSET), u32Addr &(HAL_MIU1_BASE -1));
        HAL_RVD_QMemWrite((gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DUMMY_LENGTH), u32Size);
    }
    else
    {
        RVD_MSG_ERR("<Warning>PB Size 0x%lx !!!\n",u32Size);
    }

}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetCfg()
/// @brief \b Function \b Description:  Set RVD Memory map and FW info
/// @param -memCfg \b IN RVD_MEMCfg
/// @return - Display Count
/// @retval      - E_RVD_RET_SUCCESS, Success
/// @retval      - E_RVD_RET_INVALID_PARAM, the config setting invalid
//------------------------------------------------------------------
RVD_Result MDrv_RVD_SetCfg( RVD_MEMCfg* memCfg)
{
    if (!memCfg)
        return E_RVD_RET_INVALID_PARAM;

    //config memory

    HAL_RVD_MemSetMap(E_RVD_MMAP_FW, memCfg->u32FWAddr, memCfg->u32FWSize);
    HAL_RVD_MemSetMap(E_RVD_MMAP_FB, memCfg->u32FBAddr, memCfg->u32FBSize);
    HAL_RVD_MemSetMap(E_RVD_MMAP_BS, memCfg->u32BSAddr, memCfg->u32BSSize);
    HAL_RVD_MemSetMap(E_RVD_MMAP_PB, memCfg->u32PBAddr, memCfg->u32PBSize);
    HAL_RVD_MemSetMap(E_RVD_MMAP_FW_BIN, memCfg->u32FWBinAddr, memCfg->u32FWBinSize);
    HAL_RVD_MemSetMap(E_RVD_MMAP_VLC_BIN, memCfg->u32VLCBinAddr, memCfg->u32VLCBinSize);
    return E_RVD_RET_SUCCESS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetOSRegBase()
/// @brief \b Function \b Description:  Set system register base
/// @param -u32RegBaseAddr \b IN :  system register base
//-----------------------------------------------------------------------------
void MDrv_RVD_SetOSRegBase(MS_U32 u32RegBaseAddr )
{
    HAL_RVD_InitRegBase(u32RegBaseAddr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetQmemSwBbuVacancy()
/// @brief \b Function \b Description:  get the vacancy of BBU queue.
/// @return - TRUE / FALSE
/// @retval     -0: queue is full.
/// @retval     -not zero: queue is not full.
//-----------------------------------------------------------------------------
MS_S16 MDrv_RVD_GetQmemSwBbuVacancy(void)
{
    MS_S16 s16Vac;
    MS_S16 s16Rptr;
    MS_S16 s16Wptr;
    MS_S16 s16BBUDepth;

#if USING_ID_LINK
    s16Rptr = (MS_S16)MDrv_RVD_GetIDReadPtr();
    //s16Wptr = (MS_S16)MDrv_RVD_GetIDWritePtr();
  #else
    s16Rptr = (MS_S16)MDrv_RVD_GetBBUReadPtr();
    //s16Wptr = (MS_S16)MDrv_RVD_GetBBUWritePtr();
#endif
    s16Wptr =(MS_S16)u16QmemBbuWptr;
    s16BBUDepth = (MS_S16)MDrv_RVD_GetBBUDepth();

    if(s16Wptr < s16Rptr)
    {
        s16Vac = s16Rptr - s16Wptr - 1;
    }
    else if(s16Wptr >= s16Rptr)
    {
        s16Vac = (s16BBUDepth-1)  - s16Wptr + s16Rptr;
    }

    return s16Vac;
}

#if RVD_ENABLE_BDMA_FW_FLASH_2_SDRAM
#if 0
BDMA_CpyType MDrv_RVD_BDMAFlag(MS_BOOL bDirect)
{
    if(bDirect)
        return E_BDMA_FLASH2SDRAM1;
    else
        return E_BDMA_FLASH2SDRAM;
}
#endif
#endif

BDMA_CpyType MDrv_RVD_BDMAFlag(MS_BOOL bDst ,MS_BOOL bSrc, RVD_FWSourceType eType)
{
    RVD_MSG_HAL("Dst : %d Src :%d\n SourceType: %d",bDst,bSrc,eType);
    switch(eType)
    {
        case E_RVD_FW_SOURCE_DRAM:
            if(bDst < bSrc)
            {
                return E_BDMA_SDRAM12SDRAM;
            }
            else if(bDst > bSrc )
            {
                return E_BDMA_SDRAM2SDRAM1;
            }
            else
            {
                if(bSrc)
                    return E_BDMA_SDRAM12SDRAM1;
                else
                    return E_BDMA_SDRAM2SDRAM;
            }
        case E_RVD_FW_SOURCE_FLASH:
            if(bDst)
                return E_BDMA_FLASH2SDRAM1;
            else
                return E_BDMA_FLASH2SDRAM;
        default:
            return (BDMA_CpyType)0;
    }
}

BDMA_CpyType MDrv_RVD_BDMA_SDFlag(MS_BOOL bDst ,MS_BOOL bSrc)
{
    RVD_MSG_HAL("Dst : %d Src :%d\n",bDst,bSrc);
    if(bDst < bSrc)
    {
        return E_BDMA_SDRAM12SDRAM;
    }
    else if(bDst > bSrc )
    {
        return E_BDMA_SDRAM2SDRAM1;
    }
    else
    {
        if(bSrc)
            return E_BDMA_SDRAM12SDRAM1;
        else
            return E_BDMA_SDRAM2SDRAM;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetFrameSize()
/// @brief \b Function \b Description:  get the frame size from Qmem used for RPR
/// @param -u16Width \b IN Frame Size Width
/// @param -u16Height \b IN Frame Size Height
/// @return status
//-----------------------------------------------------------------------------
void MDrv_RVD_GetFrameSize(MS_U16* u16Width,MS_U16* u16Height)
{
    *u16Width =  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_FRAME0_WIDTH);
    *u16Height =  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_FRAME0_HEIGHT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetLibVer()
/// @brief \b Function \b Description:  get the RVD Drv and FW Version
/// @return none
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return E_RVD_RET_FAIL;

    *ppVersion = &_drv_rvd_version;
    return E_RVD_RET_SUCCESS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetDispRepeatCnt()
/// @brief \b Function \b Description:  Set Disp repeat cnt to f/w
/// @param -u32Num \b IN Repeat cnt
/// @return status
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetDispRepeatCnt(MS_U32 u32Num)
{
    return MDrv_RVD_SetCmd(E_RVD_CMD_DISP_REPEAT_MAX, u32Num);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetInfo()
/// @brief \b Function \b Description:  Get information of RVD driver.
/// @return - driver information
//-----------------------------------------------------------------------------
const RVD_DrvInfo* MDrv_RVD_GetInfo( void )
{
    if(!bRVDIsInit)
        return 0;
    DrvRVDInfo.bRVD=MDrv_RVD_CheckCaps();
    return (&DrvRVDInfo);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetTimeStamp()
/// @brief \b Function \b Description:  get the pts of current displayed video frame
/// @return - PTS
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetTimeStamp(void)
{
    MS_U32 u32FrameInfoAddr;

    u32FrameInfoAddr = HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DISP_FRAME_INFO_ADDR);
    return HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_TIMESTAMP );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetDecodeCnt()
/// @brief \b Function \b Description:  get accumulated decoded frame Count
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetDecodeCnt(void)
{
    return  HAL_RVD_QMemRead( (gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DECODECNT) );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetFrameCnt()
/// @brief \b Function \b Description:  get accumulated display Count
/// @return - Display Count
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetFrameCnt(void)
{
    return  HAL_RVD_QMemRead( (gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DISPLAYCNT) );
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetBBUReadPtr()
/// @brief \b Function \b Description:  Set BBU Read point to f/w
/// @param  -u32Rptr \b IN BBU Read Pointer
//-----------------------------------------------------------------------------
void MDrv_RVD_SetBBUReadPtr(MS_U32 u32Rptr)
{
     HAL_RVD_QMemWrite((gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_BBU_RPTR), u32Rptr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetBBUWritePtr()
/// @brief \b Function \b Description:  Set BBU write point to f/w
/// @param  -u32Wptr \b IN BBU Read Pointer
//-----------------------------------------------------------------------------
void MDrv_RVD_SetBBUWritePtr(MS_U32 u32Wptr)
{
     HAL_RVD_QMemWrite((gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_BBU_WPTR), u32Wptr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetIDReadPtr()
/// @brief \b Function \b Description:  Set BBU read point to f/w (Flush only)
/// @param  -u32Wptr \b IN BBU Read Pointer
//-----------------------------------------------------------------------------
void MDrv_RVD_SetIDReadPtr(MS_U32 u32Wptr)
{
     HAL_RVD_QMemWrite((gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_ID_RPTR), u32Wptr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetIDWritePtr()
/// @brief \b Function \b Description:  Set BBU write point to f/w
/// @param  -u32Wptr \b IN BBU Read Pointer
//-----------------------------------------------------------------------------
void MDrv_RVD_SetIDWritePtr(MS_U32 u32Wptr)
{
     HAL_RVD_QMemWrite((gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_ID_WPTR), u32Wptr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetBBUReadPtr()
/// @brief \b Function \b Description:  Get BBU Read point from f/w
/// @return - bbu read pointer
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetBBUReadPtr(void)
{
    return  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_BBU_RPTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetFrameInfo()
/// @brief \b Function \b Description:  Get Frame Info
/// @return - bbu read pointer
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_GetFrameInfo(RVD_FrameBuff_Info *framebuffinfo ,MS_BOOL bGETDISP)
{
    MS_U32 u32FrameInfoAddr = 0;
    MS_U32 u32MIUBaseAddr = 0;

    if (bGETDISP)
       u32FrameInfoAddr = HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DISP_FRAME_INFO_ADDR);
    else
       u32FrameInfoAddr = HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DEC_FRAME_INFO_ADDR);

    RVD_MSG_HAL("u32FrameInfoAddr = 0x%lx",u32FrameInfoAddr);
    if (u32FrameInfoAddr)
    {
        if (gRVDInfo.bRVD_HWinMIU1)
            u32MIUBaseAddr = HAL_MIU1_BASE;

        framebuffinfo->u32LumaAddr = HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_LUMAADDR ) |u32MIUBaseAddr;
        framebuffinfo->u32ChromaAddr = HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_CHROMADDR )|u32MIUBaseAddr;
        framebuffinfo->u32TimeStamp =  HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_TIMESTAMP );
        framebuffinfo->u32ID_L = HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_ID_L );
        framebuffinfo->u32ID_H = HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_ID_H );
        framebuffinfo->u16FrmType = (MS_U16)(HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_FRAME_TYPE )&0x0000ffff);
        framebuffinfo->u16Pitch = (MS_U16)((HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_PITCH)&0xffff0000)>>16);
        framebuffinfo->u16Width = (MS_U16) (HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_WIDTH)&0x0000ffff);
        framebuffinfo->u16Height = (MS_U16)((HAL_RVD_MemRead(u32FrameInfoAddr + E_RVD_FRAMEBUFF_INFO_HEIGHT)&0xffff0000)>>16);
        RVD_MSG_HAL("Luma = 0x%lx  Chroma = 0x%lx Width =0x%x Height =0x%x Pitch = 0x%x",framebuffinfo->u32LumaAddr, framebuffinfo->u32ChromaAddr,
             framebuffinfo->u16Width, framebuffinfo->u16Height, framebuffinfo->u16Pitch );
        return E_RVD_RET_SUCCESS;
    }
    else
    {
        return E_RVD_RET_FAIL;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetIDReadPtr()
/// @brief \b Function \b Description:  Get BBU Read point from f/w
/// @return - bbu read pointer
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetIDReadPtr(void)
{
    return  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_ID_RPTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetIDWritePtr()
/// @brief \b Function \b Description:  Get ID write point from f/w
/// @return - bbu write pointer
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetIDWritePtr(void)
{
    return  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_ID_WPTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetBBUWritePtr()
/// @brief \b Function \b Description:  Get BBU write point from f/w
/// @return - bbu write pointer
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetBBUWritePtr(void)
{
    return  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_BBU_WPTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetBBUDepth()
/// @brief \b Function \b Description:  Get BBU Depth
/// @return - BBU Depth
//-----------------------------------------------------------------------------
MS_U16 MDrv_RVD_GetBBUDepth(void)
{
    return  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_BBU_DEPTH);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetPictureSize()
/// @brief \b Function \b Description:  Set Picture width and Height to F/W
/// @param -u16Width \b IN The frame Width
/// @param -u16Height \b IN  The frame Height
//-----------------------------------------------------------------------------
void MDrv_RVD_SetPictureSize(MS_U16 u16Width, MS_U16 u16Height)
{
     HAL_RVD_QMemWrite( (gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_PIC_WIDTH) , u16Width);
     HAL_RVD_QMemWrite( (gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_PIC_HEIGHT) , u16Height);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetPictureSize()
/// @brief \b Function \b Description:  Set Picture width and Height to F/W
/// @param -u32Width \b IN The frame Width
/// @param -u32Height \b IN  The frame Height
//-----------------------------------------------------------------------------
void MDrv_RVD_GetPictureSize(MS_U16* u16Width, MS_U16* u16Height)
{
    *u16Width = HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_PIC_WIDTH);
    *u16Height = HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_OFFSET_PIC_HEIGHT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetPlayMode()
/// @brief \b Function \b Description:  HK Command ()
/// @param -u32PlayMode \b IN RVD_PlayMode
//-----------------------------------------------------------------------------
void MDrv_RVD_SetDisplay(RVD_DisplayMode u32PlayMode)
{
    HAL_RVD_QMemWrite( gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_PLAY_MODE, u32PlayMode);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_PushBBU()
/// @brief \b Function \b Description:  push one entry into the decoding table(BBU table).
/// @param -u32offset \b IN :  the start physical address of the entry.(unit:byte)
/// @param -u32length \b IN :  the data size of the entry. (unit:byte)
/// @param -rvdPackerInfo \b IN :  the ID  of the entry. (unit:byte)
//-----------------------------------------------------------------------------
void MDrv_RVD_PushBBU (MS_U32 u32offset, MS_U32 u32length ,RVD_Packet_ID *rvdPackerInfo)
{
    MS_U32 u32Addr;
    //MS_U16 u16QmemIDWptr = 0;

    if(u32offset&MW_RVD_BROKEN_BY_US)
    {
        u32offset = (u32offset & (~MW_RVD_BROKEN_BY_US)) | BROKEN_BY_US;
        RVD_MSG_DRV("Broken by us is True\n");
    }
    RVD_MSG_INF("OFFSET LENGTH<%lx  , %lx>\n",u32offset,u32length);
    #if 0
    #if USING_ID_LINK
    u16QmemBbuWptr = (MS_U16)MDrv_RVD_GetIDWritePtr();
    #else
    u16QmemBbuWptr = (MS_U16)MDrv_RVD_GetBBUWritePtr();
    #endif
    #endif

    u32Addr = gu32QmemBbuAddr + ( ((MS_U32)u16QmemBbuWptr) << 3); //offset + length : 8 byte
    MDrv_RVD_SetBBU_OffsetLength(u32Addr,u32offset,u32length);

    u32Addr = gu32QmemBbuAddr + QMEM_BBU_TABLE_SIZE + ( ((MS_U32)u16QmemBbuWptr) * 12); // byte_pos, ID_L, ID_H
    MDrv_RVD_SetBBU_ID(u32Addr,u32BytePos,rvdPackerInfo->u32PacketID_HI,rvdPackerInfo->u32PacketID_LO);

    u32BytePos += u32length;
    u32BytePos %=BYTECOUNT_LIMIT;

    if ( u16QmemBbuWptr == (RVD_BBU_DEPTH - 1) )
    {
        u16QmemBbuWptr = 0;
    }
    else
    {
        u16QmemBbuWptr++;
    }

    #if 0
    MDrv_RVD_SetIDWritePtr((MS_U32)(u16QmemBbuWptr));
    MDrv_RVD_SetBBUWritePtr((MS_U32)(u16QmemBbuWptr));
    #endif
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_FireDecCmd()
/// @brief \b Function \b Description: Fire the decode command which be pushed since last fired
/// @return Status
/// @retval      - E_RVD_RET_SUCCESS, Success
/// @retval      - E_RVD_RET_FAIL, Failed
//------------------------------------------------------------------------------
RVD_Result MDrv_RVD_FireDecCmd(void)
{
    MDrv_RVD_SetBBUWritePtr(u16QmemBbuWptr);
    MDrv_RVD_SetIDWritePtr(u16QmemBbuWptr);
    return E_RVD_RET_SUCCESS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetFileInfo()
/// @brief \b Function \b Description:  Set RM File information for F/W INFO Table.
/// @param -pFileInfo \b IN :  the file information of RM file
//-----------------------------------------------------------------------------
void MDrv_RVD_SetFileInfo( RVD_FileInfo *pFileInfo)
{
    MS_U8 i = 0;
    MS_U32 u32Addr = 0;

    if( NULL == pFileInfo )
    {
        return ;
    }

    // file info addr = bin addr + bin size
    u32Addr = OPENRISC_BIN_SIZE;

    if ( pFileInfo->RV_Version == 0 ) // RV8
    {
        HAL_RVD_SetRVInfo(pFileInfo->RV_Version);

        // write RV_Version = 0
        HAL_RVD_MemWrite(u32Addr, pFileInfo->RV_Version);
        u32Addr+=4;

        // write ulNumSizes
        HAL_RVD_MemWrite(u32Addr, pFileInfo->ulNumSizes);
        u32Addr+=4;

        // write ulPicSizes_w[8]
        for( i = 0; i<8; i++ )
        {
            HAL_RVD_MemWrite(u32Addr, pFileInfo->ulPicSizes_w[i]);
            u32Addr+=4;
        }

        // write ulPicSizes_h[8]
        for( i = 0; i<8; i++ )
        {
            HAL_RVD_MemWrite(u32Addr, pFileInfo->ulPicSizes_h[i]);
            u32Addr+=4;
        }
    }
    else
    {
        HAL_RVD_SetRVInfo(pFileInfo->RV_Version );

        // write RV_Version = 1
        HAL_RVD_MemWrite(u32Addr, 1);
        u32Addr+=4;

        // write ulNumSizes = 0
        HAL_RVD_MemWrite(u32Addr, 0);
        u32Addr+=4;

        // jump ulPicSizes_w[8], ulPicSizes_h[8]
        u32Addr += (4 * 8 * 2); // 4 bytes * 8 * 2
    }

    // write buffer mode
    HAL_RVD_MemWrite(u32Addr, pFileInfo->ulBufMode);
    u32Addr+=4;

    // write frame buffer start addr
    HAL_RVD_MemWrite(u32Addr, pFileInfo->ulFBAddr);
    u32Addr+=4;

    // write frame buffer size (total frame buffer size + IAP size)
    HAL_RVD_MemWrite(u32Addr, pFileInfo->ulFBSize);
    u32Addr+=4;

    // write VLC addr = FB addr + FB size
    HAL_RVD_MemWrite(u32Addr, pFileInfo->ulVLCAddr );
    u32Addr+=4;
#if 0
    // write DS setting
    HAL_RVD_MemWrite(u32Addr, pFileInfo->RV_DynScaling.u32DynAddr);
    u32Addr+=4;

    HAL_RVD_MemWrite(u32Addr, pFileInfo->RV_DynScaling.u32Depth);
    u32Addr+=4;

    if (pFileInfo->RV_DynScaling.bEnable)
        HAL_RVD_MemWrite(u32Addr, eRvdDynCfgByChip);
    else
        HAL_RVD_MemWrite(u32Addr, 0);
#endif
    u32Addr+=4;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetQMEM_Info()
/// @brief \b Function \b Description:  Get QmemExchangeInfoAddr From F/W
//-----------------------------------------------------------------------------
void MDrv_RVD_GetQMEM_Info(void)
{
    gu32QmemExchangeInfoAddr = HAL_RVD_GetExchangeInfoAddr();
    RVD_MSG_INF("QmemExchangeInfoAddr = 0x%lx\n", gu32QmemExchangeInfoAddr);

    gu32QmemBbuAddr = MDrv_RVD_GetBBUStartAddr();
    RVD_MSG_INF("QmemBbuAddr = 0x%lx\n", gu32QmemBbuAddr);

    MDrv_RVD_SetBBUDepth(RVD_BBU_DEPTH);
    RVD_MSG_INF("<gdbg>RVD_BBU_DEPTH = %d\n", RVD_BBU_DEPTH);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetTrickRefInfo()
/// @brief \b Function \b Description: Mark the dummy data for flush BBU
//-----------------------------------------------------------------------------
void MDrv_RVD_SetTrickRefInfo(MS_PHYADDR u32DstAddr ,MS_PHYADDR u32ScrAddr)
{
    MS_U32 tmpAdr ,tmpLen,tmpTotal;
    RVD_FWSourceType eType = E_RVD_FW_SOURCE_DRAM;
    
    HAL_RVD_MemGetMap(E_RVD_MMAP_FW, &tmpAdr, &tmpLen,1);
    tmpTotal = tmpAdr + tmpLen - 8;
    WRITE_LONG((MS_U32)tmpTotal,(MS_U32)0xffffffff);
    WRITE_LONG((MS_U32)((tmpTotal)+4),(MS_U32)0xffffffff);
    tmpTotal = u32ScrAddr + tmpLen - 8;
    gRVDInfo.u32BDMAFlag = MDrv_RVD_BDMAFlag(gRVDInfo.bRVD_HWinMIU1,gRVDInfo.bRVD_FWinMIU1,eType);//E_RVD_FW_SOURCE_DRAM);
    if (E_BDMA_OK != BDMAcpy( u32DstAddr,
                              tmpTotal,
                              16,
                              (BDMA_CpyType)gRVDInfo.u32BDMAFlag) )
    {
        RVD_MSG_ERR("RVD ERR: BDMAcpy Flash 2 DRAM failed: dest:0x%lx src:0x%lx flag:%lu\n" ,u32DstAddr, tmpTotal, (MS_U32)gRVDInfo.u32BDMAFlag) ;
    }

    RVD_MSG_INF("DummyDataAddr: 0x%lx\n", u32DstAddr);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetFWInfo()
/// @brief \b Function \b Description:  Get RVD FW and HW version(reported from f/w)
//-----------------------------------------------------------------------------
void MDrv_RVD_GetFWInfo(void)
{
    #define FWVerAddr 0 //base : Open bin addr
    #define HWVerAddr 4 //base : Open bin addr

    DrvRVDInfo.FWversion = HAL_RVD_MemRead(FWVerAddr);
    DrvRVDInfo.HWversion = HAL_RVD_MemRead(HWVerAddr);
    RVD_MSG_INF("FW INFO>>RVD.FWversion:0x%lx\t RVD.HWversion: 0x%lx\n", DrvRVDInfo.FWversion, DrvRVDInfo.HWversion);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_LoadVLCTable()
/// @brief \b Function \b Description:  Load VLC table to Dram
//-----------------------------------------------------------------------------
void MDrv_RVD_LoadVLCTable(MS_U32 u32Addr ,RVD_FWSourceType eType)
{
    RVD_MSG_INF("vlc_addr  = 0x%lx\n",u32Addr);
#if (!RVD_ENABLE_BDMA_FW_FLASH_2_SDRAM)
    MS_U32 tmpAdr , tmpLen;
    //RVD_FWSourceType eTypetmp = E_RVD_FW_SOURCE_DRAM;

    eType = E_RVD_FW_SOURCE_DRAM;
    gRVDInfo.u32BDMAFlag = MDrv_RVD_BDMAFlag(gRVDInfo.bRVD_HWinMIU1,gRVDInfo.bRVD_FWinMIU1,eType);
    gRVDInfo.u32VLC_ScrAddr = gRVDInfo.u32FW_PhyAddr;
    HAL_RVD_MemGetMap(E_RVD_MMAP_FW, &tmpAdr, &tmpLen, 1);
    gRVDInfo.u32VLC_ScrSize = sizeof(VLC_TAB);
    RVD_memcpy((void*)tmpAdr,(void*)VLC_TAB, gRVDInfo.u32VLC_ScrSize);
    RVD_MSG_INF("RVD Check: BDMAcpy DRAM 2 DRAM: dest:0x%lx src:0x%lx size:0x%lx flag:%lx\n" ,u32Addr, gRVDInfo.u32VLC_ScrAddr , gRVDInfo.u32VLC_ScrSize , (MS_U32)gRVDInfo.u32BDMAFlag) ;
    if (E_BDMA_OK != BDMAcpy( u32Addr,
                              gRVDInfo.u32VLC_ScrAddr,
                              gRVDInfo.u32VLC_ScrSize,
                              (BDMA_CpyType)gRVDInfo.u32BDMAFlag) )
    {
        RVD_MSG_ERR("RVD ERR: BDMAcpy Flash 2 DRAM failed: dest:0x%lx src:0x%lx size:0x%lx flag:%lu\n" ,u32Addr, gRVDInfo.u32VLC_ScrAddr , gRVDInfo.u32VLC_ScrSize , (MS_U32)gRVDInfo.u32BDMAFlag) ;
    }
#else
    gRVDInfo.u32BDMAFlag = MDrv_RVD_BDMAFlag(gRVDInfo.bRVD_HWinMIU1, gRVDInfo.bRVD_VLCBinInMIU1,eType);
    if(E_BDMA_OK != BDMAcpy(u32Addr, gRVDInfo.u32VLC_ScrAddr, gRVDInfo.u32VLC_ScrSize,  (BDMA_CpyType)gRVDInfo.u32BDMAFlag) )
    {
        RVD_MSG_ERR("RVD ERR: BDMAcpy Flash 2 DRAM failed: dest:0x%lx src:0x%lx size:0x%lx flag:%lu\n" ,u32Addr  , gRVDInfo.u32VLC_ScrAddr , gRVDInfo.u32VLC_ScrSize , (MS_U32)gRVDInfo.u32BDMAFlag) ;
    }
#endif
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_ResetVariable()
/// @brief \b Function \b Description:  Reset Variable
//-----------------------------------------------------------------------------
void MDrv_RVD_ResetVariable(void)
{
    u32StepPlayCnt= 0 ; // check if step play done
    u32StepDecodeCnt= 0 ; // check if step play done
    u32BytePos= 0 ;
    u16QmemBbuWptr = 0;
    RVD_memset((void*)&gRVDInfo, 0, sizeof(RVD_MiuInfo));
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_Init()
/// @brief \b Function \b Description:  RVD Initial
/// @param -pFileInfo \b IN :  the file information of RM file
/// @param -memCfg \b IN :  the RVD memmap config
/// @return -The result of RVD Init
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_Init(RVD_FileInfo *pFileInfo, RVD_MEMCfg* memCfg)
{
    MS_U32 tmpAdr, tmpLen;
    MS_U8 u8Ret = E_RVD_RET_FAIL;
    MS_U32 u32TmpAddr;
    MS_BOOL bNeedReloadFW = TRUE;

#if RVD_ENABLE_TIME_MEASURE
    u32RvdInitSysTimeBase = RVD_GetSysTime_ms();
#endif
    if (NULL == pFileInfo)
    {
        RVD_MSG_ERR("Hal_RVD_Init Err : don't have file_info!!\n");
        return E_RVD_RET_FAIL;
    }

    MDrv_RVD_ResetVariable();
    if (MDrv_RVD_SetCfg(memCfg) != E_RVD_RET_SUCCESS)
        return E_RVD_RET_INVALID_PARAM;

    //the very first step: Get register base addr
    //notice: must do this first before accessing any register.
#if 0
    MS_U32 u32NonPMBankSize;
    MS_U32 u32NonPMBankAddr;
    if( !MDrv_MMIO_GetBASE( &u32NonPMBankAddr, &u32NonPMBankSize, MS_MODULE_RVD))
    {
        RVD_MSG_INF("_MDrv_RVD_Init: IOMap failure\n");
        return E_RVD_RET_FAIL;
    }
    else
    {
        MDrv_RVD_SetOSRegBase(u32NonPMBankAddr);
        RVD_MSG_INF("_MDrv_RVD_Init: u32RiuBaseAdd = %lx\n", u32NonPMBankAddr);
    }
#endif

    if (bRVDIsInit == TRUE)
    {
        return E_RVD_RET_RE_INIT;
    }

    RVD_MSG_INF("GO RVD INIT\n");
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

#if RVD_ISR_ENABLE
    //Release RVD ISR
    HAL_RVD_Enable_ISR(FALSE);
#endif

    HAL_RVD_Reset_RVD_SVD();
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    // Using FWAddr as tmp buffer to mark flush pattern
    //MDrv_RVD_SetTrickRefInfo(memCfg->u32BSAddr, memCfg->u32FWAddr);
    MDrv_RVD_SetTrickRefInfo(memCfg->u32PBAddr, memCfg->u32FWAddr);
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    //Check whether need to reload fw or not
    if((TRUE == gbEnableRVTurboFWMode)
    && (FALSE == HAL_RVD_IsNeedReload()))
    {
        bNeedReloadFW = FALSE;
    }

    if(TRUE == bNeedReloadFW)
    {
        //If we need to reload fw, need to reset fw decoder type first.
        HAL_RVD_SetFWDecoder(FALSE);

        u32TmpAddr = memCfg->u32FBAddr + memCfg->u32FBSize -RVD_VLC_TABLE_SIZE;
        MDrv_RVD_LoadVLCTable(u32TmpAddr ,memCfg->eFWSourceType);
#if RVD_ENABLE_TIME_MEASURE
        RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

#if (!RVD_ENABLE_BDMA_FW_FLASH_2_SDRAM)
        HAL_RVD_LoadCode();
#else
        gRVDInfo.u32BDMAFlag = MDrv_RVD_BDMAFlag(gRVDInfo.bRVD_FWinMIU1,
                                              gRVDInfo.bRVD_FWBinInMIU1,memCfg->eFWSourceType);
        if (E_BDMA_OK != BDMAcpy( gRVDInfo.u32FW_DstAddr,
                                  gRVDInfo.u32FW_ScrAddr,
                                  gRVDInfo.u32FW_ScrSize,
                                  (BDMA_CpyType)gRVDInfo.u32BDMAFlag) )
        {
            RVD_MSG_ERR("RVD ERR: BDMAcpy Flash 2 DRAM failed: dest:0x%lx src:0x%lx size:0x%lx flag:%lu\n" ,gRVDInfo.u32FW_DstAddr, gRVDInfo.u32FW_ScrAddr , gRVDInfo.u32FW_ScrSize , (MS_U32)gRVDInfo.u32BDMAFlag) ;
        }
#endif

        //When load successfully, set FW decoder to RVD
        HAL_RVD_SetFWDecoder(TRUE);
    }

#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    // get virtual bin addr
    HAL_RVD_MemGetMap(E_RVD_MMAP_FW, &tmpAdr, &tmpLen, 1);
    u32FWBinAddr = tmpAdr;
    RVD_MSG_INF("u32FWBinAddr =0x%lx\n", u32FWBinAddr);
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    HAL_RVD_VerChiptop_Init();
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    HAL_RVD_Release_RVD_SVD();
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    u8Ret = E_RVD_RET_FAIL;
    while (!u8Ret)
    {
       u8Ret = HAL_RVD_CheckSWRSTDone();
    }
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    u8Ret = E_RVD_RET_FAIL;
    while (!u8Ret)
    {
       u8Ret = HAL_RVD_CheckHKSWRSTDone();
    }
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    HAL_RVD_BBU_SET_CPU();
    HAL_RVD_CPUSetting(1);
    HAL_RVD_MemoryProtect(0);
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    // get physical addr
    HAL_RVD_MemGetMap(E_RVD_MMAP_FB, &tmpAdr, &tmpLen, 0);
    pFileInfo->ulFBAddr = tmpAdr;
    pFileInfo->ulFBSize = tmpLen - RVD_VLC_TABLE_SIZE;
    pFileInfo->ulVLCAddr = tmpAdr + tmpLen - RVD_VLC_TABLE_SIZE;
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    MDrv_RVD_SetFileInfo( pFileInfo ); // write file info to DRAM
    HAL_RVD_SetBufferAddr();
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms()  -  u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    while (1)
    {
        RVD_MSG_INF("Wait RM_HI_MBOX1 \n");
        if (HAL_RVD_MBoxReady(RVD_HI_MBOX1))
        {
            break;
        }
    }
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif
    RVD_MSG_INF("11 \n");

    // set physical bin addr
    HAL_RVD_MemGetMap(E_RVD_MMAP_FW, &tmpAdr, &tmpLen, 0);
    HAL_RVD_MBoxSet(RVD_HI_MBOX1, tmpAdr);//A0000000
    RVD_MSG_INF("12 \n");

    while (1)
    {
        RVD_MSG_INF("Wait RM_HI_MBOX0 \n");
        if (HAL_RVD_MBoxReady(RVD_HI_MBOX0))
        {
            break;
        }
    }
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif
    RVD_MSG_INF("21 \n");

    HAL_RVD_WriteDimInfo2RVD();
    RVD_MSG_INF("22 \n");
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    HAL_RVD_CPURstRelse();
    RVD_MSG_INF("23 \n");
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    if ( HAL_RVD_Check_INIT_QMEM_SUCCEED1() == FALSE )
        return E_RVD_RET_FAIL;
    RVD_MSG_INF("3 \n");
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    MDrv_RVD_GetQMEM_Info();
    RVD_MSG_INF("4 \n");
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure:%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif

    //Set Picture info in Qmem
    RVD_MSG_INF("SetPicInfo W=%d H=%d\n", pFileInfo->ulPicSizes_w[0], pFileInfo->ulPicSizes_h[0]);
    MDrv_RVD_SetPictureSize(pFileInfo->ulPicSizes_w[0], pFileInfo->ulPicSizes_h[0]);
    MDrv_RVD_GetFWInfo();
    MDrv_RVD_SetPBInfo((memCfg->u32PBAddr - memCfg->u32BSAddr), memCfg->u32PBSize);
    u32RVDCtrlMode |= RVD_CTRL_INIT_FINISHED;
    bRVDIsInit = TRUE;
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure(Init finish):%ld (%s %d) \n" , RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif
    return E_RVD_RET_SUCCESS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_EnableDynamicScaling()
/// @brief \b Function \b Description:  Enable Dynamic Scaling
/// @param -bEnable \b IN :  Enable Bit
/// @retval     -E_RVD_RET_FAIL:Command Timeout
/// @retval     -E_RVD_RET_SUCCESS:Ready
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_EnableDynamicScaling(MS_BOOL bEnable)
{
    if (bEnable)
        return MDrv_RVD_SetCmd(E_RVD_CMD_DYNAMIC_SCALE, RVD_DS_MIU_BIT);
    else
        return MDrv_RVD_SetCmd(E_RVD_CMD_DYNAMIC_SCALE, 0); // disable
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetVirtualBox()
/// @brief \b Function \b Description:  Set DS width and Height to F/W
/// @param -u16Width \b IN :  frame width
/// @param -u16Height \b IN :  frame height
//-----------------------------------------------------------------------------
void MDrv_RVD_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height)
{
    HAL_RVD_QMemWrite( (gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DS_VIRTUAL_BOX_W), u16Width);
    HAL_RVD_QMemWrite( (gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DS_VIRTUAL_BOX_H), u16Height);
}

//-----------------------------------------------------------------------------
/// @brief Pass scalar parameters to f/w
/// @return -The result of command.
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetDynScalingParam(MS_U32 u32StAddr, MS_U32 u32Size)
{
    MS_U32 tmpAdr, tmpLen;
    MS_U32 u32ScalerInfoAddr;

    if ((u32StAddr==NULL) || (u32Size==0))
    {
        RVD_MSG_ERR("%s invalid para u32StAddr=0x%lx, u32Size=0x%lx\n", __FUNCTION__, u32StAddr, u32Size);
        return E_RVD_RET_INVALID_PARAM;
    }

    HAL_RVD_MemGetMap(E_RVD_MMAP_FW, &tmpAdr, &tmpLen, 1); // get virtual bin addr
    u32ScalerInfoAddr = tmpAdr + HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_SCALER_INFO_ADDR);

    //copy data
    u32Size = ((u32Size + 3) >> 2) << 2;
    RVD_MSG_INF("u32Size= 0x%lx, u32SrcAdd= 0x%lx\n", u32Size, u32StAddr);
    RVD_memcpy((void*)u32ScalerInfoAddr, (void*) u32StAddr, u32Size);

    //notify f/w
    return MDrv_RVD_SetCmd(E_RVD_CMD_SCALER_INFO, NULL);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_FlushQueue()
/// @brief \b Function \b Description:  Flush Queue and Reset Read/Write pointer
/// @return - Flush Queue is done or not
/// @retval     -E_RVD_RET_NOTREADY: f/w not ready for handle the pre cmd
/// @retval     -E_RVD_RET_FAIL:Command Timeout
/// @retval     -E_RVD_RET_SUCCESS:Ready
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_FlushQueue(void)
{
    MS_U32 u32Timeout = 0;

#if RVD_ENABLE_TIME_MEASURE
    u32RvdInitSysTimeBase = RVD_GetSysTime_ms();
#endif
    if(MDrv_RVD_Pause() == E_RVD_RET_FAIL)
    {
        RVD_MSG_ERR("[RM]THe Pause Command not be deal !!\n");
    }
    RVD_MSG_INF("<gdbg>BBU Flush \n");
    //MDrv_RVD_SetTrickRefInfo();
    MDrv_RVD_SetCmd(E_RVD_CMD_FLUSH, NULL);

    u32Timeout = RVD_GetSystemTime() + 1000;
    while(1)
    {
        RVD_MSG_HAL("<gdbg PC: 0x%lx>\n", HAL_RVD_ReadSVDProgramCounter());
        if (MDrv_RVD_ChkCmdRdy()==E_RVD_RET_SUCCESS)
        {
            RVD_MSG_INF("<gdbg>Flush done now...\n");
            break;
        }
        if (RVD_GetSystemTime() > u32Timeout)
        {
            RVD_MSG_ERR("[RM] Hal_RVD_FlushBuf timeout\n");
            return E_RVD_RET_TIMEOUT;
        }
    }
    u32BytePos = HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_BYTE_POS);
    RVD_MSG_INF("u32BytePos =%ld\n", u32BytePos);
    RVD_MSG_INF("<gdbg>hold here!!!\n");
    HAL_RVD_ReadSVDProgramCounter();
    u16QmemBbuWptr = 0;
    MDrv_RVD_SetBBUReadPtr(0);
    MDrv_RVD_SetBBUWritePtr(0);
    MDrv_RVD_SetIDReadPtr(0);
    MDrv_RVD_SetIDWritePtr(0);
#if RVD_ENABLE_TIME_MEASURE
    RVD_MSG_DRV( "RVD Time Measure(Init finish):%ld (%s %d)\n", RVD_GetSysTime_ms() - u32RvdInitSysTimeBase , __FUNCTION__, __LINE__ );
#endif
    return E_RVD_RET_SUCCESS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetESReadPtr()
/// @brief \b Function \b Description:  Get BBU Read Adderess
/// @return - The BBU Read address
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetESReadPtr(void)
{
    return HAL_RVD_GetESBufferRptr();
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_ReadSVDProgramCounter()
/// @brief \b Function \b Description:  Read Program coUnter
/// @return - Program counter
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_ReadSVDProgramCounter(void)
{
    return HAL_RVD_ReadSVDProgramCounter();
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetESWritePtr()
/// @brief \b Function \b Description:  Get Elementary Stream buffer write point
/// @return - ES buffer write point offset from bitstream buffer base
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetESWritePtr(void)
{
    MS_U32 u32Wptr = 0;
    MS_U32 u32Offset =0;
    MS_U32 u32Length =0;

    //u32Wptr = MDrv_RVD_GetBBUWritePtr();
    u32Wptr = (MS_U32)u16QmemBbuWptr;
    if (u32Wptr)
        u32Wptr = u32Wptr - 1;
    else
        u32Wptr = RVD_BBU_DEPTH - 1;

    u32Wptr = gu32QmemBbuAddr + (u32Wptr << 3);
    u32Offset = HAL_RVD_MemRead(u32Wptr) & (~BROKEN_BY_US);
    u32Length = HAL_RVD_MemRead(u32Wptr + 4);
    RVD_MSG_INF("ES Write address = %ld\n",(u32Offset+u32Length));
    return (u32Offset+u32Length);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsFrameShowed()
/// @brief \b Function \b Description:  Is RVD Frame showed after TriggerDisp(TRUE) or first frame showed
/// @return - Is frame showed or not
/// @retval     -FALSE(0): New Framed showed
/// @retval     -TRUE(1): Not showed
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsFrameShowed(void)
{
    MS_U32 u32DisplayCnt = 0;

    u32DisplayCnt =  HAL_RVD_QMemRead( gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DISPLAYCNT);
    RVD_MSG_INF("[RVD]Display Count = %ld \n",u32DisplayCnt);
    if(u32DisplayCnt)
        return TRUE;
    else
        return FALSE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_CheckDispInfoRdy()
/// @brief \b Function \b Description:  check display info ready and correct or not
/// @return -The result of command check display infor ready or not
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_CheckDispInfoRdy(void)
{
    if (HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DECODECNT))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_CheckDispInfoRdy()
/// @brief \b Function \b Description:  check display info ready and correct or not
/// @return -The result of command check display infor ready or not
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_DispSetupDone(void)
{
    if (HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DECODECNT))
        return TRUE;
    else
        return FALSE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_CheckCaps()
/// @brief \b Function \b Description:  check if HW support this format
/// @return - TRUE/ FALSE
/// @retval     -FALSE(0): not supported by HW
/// @retval     -TRUE(1): supported by HW
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_CheckCaps(void)
{
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsTSPlaying()
/// @brief \b Function \b Description:  Is Playing
/// @return - TRUE/ FALSE
/// @retval     -FALSE(0): Stop
/// @retval     -TRUE(1): Playing
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsTSPlaying(void)
{
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_ChkCmdRdy()
/// @brief \b Function \b Description:  Is pre Command had be process by F/W
/// @return - Done or Not
/// @retval     -E_RVD_RET_FAIL: Not yet
/// @retval     -E_RVD_RET_SUCCESS: Could handle new command
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_ChkCmdRdy(void)
{
    if (HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_COMMAND_ID) == E_RVD_CMD_NULL)
        return E_RVD_RET_SUCCESS;
    else
        return E_RVD_RET_FAIL;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetCmd()
/// @brief \b Function \b Description:  Set Command and Argument to F/w
/// @param  u32Command \b RVD Command ID
/// @param  u32Argument \b The RVD Argument
/// @return Status
/// @retval      - E_RVD_RET_SUCCESS, Success
/// @retval      - E_RVD_RET_FAIL, Failed
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetCmd(RVD_User_Cmd u32Command, MS_U32 u32Argument)
{
    RVD_Result eRet;
    MS_U32 u32Timeout = 0;

    _DRV_RVD_Entry();

    if (MDrv_RVD_ChkCmdRdy()==E_RVD_RET_FAIL)
    {
        RVD_MSG_INF("<gdbg>Warning: previous command will be overwritten!");
    }

    HAL_RVD_QMemWrite( gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_ARGUMENT, u32Argument);
    HAL_RVD_QMemWrite( gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_COMMAND_ID, u32Command);

    u32Timeout = RVD_GetSystemTime() + 33;
    while (1)
    {
        if (MDrv_RVD_ChkCmdRdy())
        {
            RVD_MSG_INF("Cmd status :<ID> 0x%lx <Arg>0x%lx\n", (MS_U32)u32Command, u32Argument);
            eRet = E_RVD_RET_SUCCESS;
            break;
        }
        if (RVD_GetSystemTime() > u32Timeout)
        {
            RVD_MSG_ERR("<gdbg>Error: MDrv_RVD_SetCmd(0x%lx, 0x%lx) timeout!\n", (MS_U32)u32Command, u32Argument);
            eRet = E_RVD_RET_TIMEOUT;
            break;
        }
    }
    _DRV_RVD_Return(eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetSpeed()
/// @brief \b Function \b Description:  specify the display speed type.
/// @param -u32DispSpeed \b IN : display speed type
/// @return Status
/// @retval      - E_RVD_RET_SUCCESS, Success
/// @retval      - E_RVD_RET_FAIL, Failed
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetSpeed(RVD_Disp_Speed u32DispSpeed)
{
    return MDrv_RVD_SetCmd(E_RVD_CMD_DISP_SPEED, u32DispSpeed);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_JumpToPTS()
/// @brief \b Function \b Description:  Jump to PTS
/// @param -u32time_ms \b IN : Jump to PTS position
/// @return Status
/// @retval      - E_RVD_RET_SUCCESS, Success
/// @retval      - E_RVD_RET_FAIL, Failed
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_JumpToPTS(MS_U32 u32time_ms)
{
    return MDrv_RVD_SetCmd(E_RVD_CMD_JUMP_TO_PTS, u32time_ms);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SkipToPTS()
/// @brief \b Function \b Description:  Skip to PTS
/// @param -u32time_ms \b IN : Skip to PTS position
/// @return Status
/// @retval      - E_RVD_RET_SUCCESS, Success
/// @retval      - E_RVD_RET_FAIL, Failed
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SkipToPTS(MS_U32 u32time_ms)
{
    return MDrv_RVD_SetCmd(E_RVD_CMD_SKIP_TO_PTS, u32time_ms);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetTrickMode()
/// @brief \b Function \b Description:  specify the trick mode
/// @param -u32TrickMode \b IN : trick mode  speed type
//-----------------------------------------------------------------------------
void MDrv_RVD_SetTrickMode(RVD_TrickMode u32TrickMode)
{
    HAL_RVD_QMemWrite(gu32QmemExchangeInfoAddr+RVD_QMEM_INFO_TRICK_MODE, u32TrickMode);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetAction()
/// @brief \b Function \b Description:  get F/W action status
/// @return - error count
//-----------------------------------------------------------------------------
static MS_U32 MDrv_RVD_GetAction(void)
{
    return HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_ACTION);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_Play()
/// @brief \b Function \b Description:  Play RVD
/// @return -The result of command play
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_Play(void)
{
    MDrv_RVD_SetDisplay(E_RVD_DISPLAYMODE_NULL);

    if (MDrv_RVD_GetAction()==E_RVD_ACTIONMODE_PLAY)
        return E_RVD_RET_SUCCESS;

    return MDrv_RVD_SetCmd(E_RVD_CMD_PLAY, NULL);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_StepDecode()
/// @brief \b Function \b Description:  Step decode
/// @return -The result of Step Decode
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_StepDecode(void)
{
     u32StepDecodeCnt = MDrv_RVD_GetDecodeCnt();
     return MDrv_RVD_SetCmd(E_RVD_CMD_STEP_DECODE, NULL);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_Rst()
/// @brief \b Function \b Description:  Reset RVD decoder
/// @return -The result of Step Decode
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_Rst(void)
{
    return MDrv_RVD_Exit();
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsStepDecodeDone()
/// @brief \b Function \b Description:  IS Step decode done
/// @return -The result of Step Decode
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsStepDecodeDone(void)
{
    if (MDrv_RVD_GetDecodeCnt() > u32StepDecodeCnt)
    {
        u32StepDecodeCnt = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_AVSyncOn()
/// @brief \b Function \b Description:  Enable or Disbale AVSync mode
/// @param -bEnable \b IN : enable or not
/// @param -u16SyncDelay \b IN : Sync Delay time (unit ms)
/// @param -u16SyncTolerance \b IN : Tolerance(unit ms)
/// @return -The result of AVSync
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_AVSyncOn(MS_BOOL  bEnable,MS_U16 u16SyncDelay, MS_U16 u16SyncTolerance)
{
    RVD_Result eRet;

    if (bEnable)
    {
        MDrv_RVD_SetCmd(E_RVD_CMD_SYNC_TOLERANCE, u16SyncTolerance);
        MDrv_RVD_SetCmd(E_RVD_CMD_SYNC_VIDEO_DELAY, u16SyncDelay);
        eRet = MDrv_RVD_SetCmd(E_RVD_CMD_SYNC_ACTIVE, 1);
    }
    else
    {
        eRet = MDrv_RVD_SetCmd(E_RVD_CMD_SYNC_ACTIVE, 0);
    }
    return eRet;
}

static MS_BOOL MDrv_RVD_IsFwStatusOn(RVD_StatusType type)
{
    if (HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_STATUS) & type)
        return TRUE;
    else
        return FALSE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsAVSyncOn()
/// @brief \b Function \b Description:  AV Sync mode had enable or not
/// @return -The mode state of AVSync
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsAVSyncOn(void)
{
    return MDrv_RVD_IsFwStatusOn(E_RVD_STATUSTYPE_AVSYNCON);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_Pause()
/// @brief \b Function \b Description:  Pause RVD
/// @return -The result of command pause
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_Pause(void)
{
    if (MDrv_RVD_GetAction()==E_RVD_ACTIONMODE_PAUSE)
        return E_RVD_RET_SUCCESS;

    return MDrv_RVD_SetCmd(E_RVD_CMD_PAUSE, NULL);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_Exit()
/// @brief \b Function \b Description:  Stop RVD and release resource.
/// @return -The result of command stop
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_Exit(void)
{
    _DRV_RVD_Entry();

    if (bRVDIsInit != TRUE)
    {
        RVD_MSG_ERR("MDrv_RVD_Exit: not-init yet\n");
        _DRV_RVD_Return(E_RVD_RET_FAIL);
    }

    bRVDIsInit = FALSE;

#if RVD_ISR_ENABLE
    HAL_RVD_Enable_ISR(FALSE);
    OSAL_RVD_ISR_Disable();
    if( gRVDISRCtrl.bRegISR )
    {
        OSAL_RVD_ISR_Detach();
    }
#endif
    MDrv_RVD_ResetVariable();

    MDrv_RVD_SetCmd(E_RVD_CMD_STOP, NULL);
    RVD_MSG_INF("[RM] RVD Decoder is Stop, then exit!!\n");
    HAL_RVD_DeInit();
    RVD_memset( (void*)&DrvRVDInfo  , 0 , sizeof(RVD_DrvInfo) );
    _DRV_RVD_Return(E_RVD_RET_SUCCESS);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_Resume()
/// @brief \b Function \b Description:  RVD Resume
/// @return -The result of command resume
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_Resume(void)
{
    return MDrv_RVD_Play();
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_EnableLastFrameShowed()
/// @brief \b Function \b Description:  MDrv_RVD_EnableLastFrameShowed
/// @return -The result of command resume
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_EnableLastFrameShowed(MS_BOOL bEnabled)
{
     return MDrv_RVD_SetCmd(E_RVD_CMD_ENABLE_LAST_FRAME_SHOW, bEnabled);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetTrickMode()
/// @brief \b Function \b Description:  Get current play mode status.
/// @return - mode status
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetTrickMode(void)
{
    return HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_TRICK_MODE);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_StepPlay()
/// @brief \b Function \b Description:  Step display one frame
/// @return - E_RVD_RET_SUCCESS :command ready
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_StepPlay(void)
{
    u32StepPlayCnt = MDrv_RVD_GetFrameCnt();

    MDrv_RVD_SetDisplay(E_RVD_DISPLAYMODE_SHOW);

    if (MDrv_RVD_GetAction()==E_RVD_ACTIONMODE_PLAY)
        return E_RVD_RET_SUCCESS;

    return MDrv_RVD_SetCmd(E_RVD_CMD_PLAY, NULL);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_StepFreezeImg()
/// @brief \b Function \b Description:  Step Disp in Freeze mode
/// @param -bEnabled \b IN : enable or not
/// @return -E_RVD_RET_SUCCESS :command ready
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetFreezeImg(MS_BOOL bEnabled)
{
    return MDrv_RVD_SetCmd(E_RVD_CMD_FREEZE_IMG, bEnabled);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetBlueScreen()
/// @brief \b Function \b Description:  Set Blue screen
/// @param -bEnabled \b IN : enable or not
/// @return -E_RVD_RET_SUCCESS :command ready
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetBlueScreen(MS_BOOL bEnabled)
{
    return MDrv_RVD_SetCmd(E_RVD_CMD_BLUE_SCREEN, bEnabled);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsStepPlayDone()
/// @brief \b Function \b Description:  The frame had be showed(Only work after Step Play)
/// @return - TRUE/ FALSE
/// @retval     -FALSE(0): Not yet
/// @retval     -TRUE(1): Showed
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsStepPlayDone(void)
{
    if (MDrv_RVD_GetFrameCnt() > u32StepPlayCnt)
        return TRUE;
    else
        return FALSE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsIdle()
/// @brief \b Function \b Description:  check decoder is idle or not
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): decoder is not idle
/// @retval     -TRUE(1): decoder is idle
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsIdle(void)
{
    if (HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_IDLECNT) >= RVD_FW_IDLE_THRESHOLD )
        return TRUE;
    else
        return FALSE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsDispFinish()
/// @brief \b Function \b Description:  Check this file mode display is finish or not.
/// @return - Is finish or not
/// @retval     -FALSE(0): Not finish or Not in file mode playback
/// @retval     -TRUE(1): Display Finished.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsDispFinish(void)
{
    return MDrv_RVD_IsFwStatusOn(E_RVD_STATUSTYPE_PLAYBACK_FINISH);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_1stFrameRdy()
/// @brief \b Function \b Description:  check if I frame found after Init() or flush().
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): I frame is not found.
/// @retval     -TRUE(1): I frame has been found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_Is1stFrameRdy(void)
{
    return MDrv_RVD_IsFwStatusOn(E_RVD_STATUSTYPE_1ST_FRAMERDY);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsIFrameFound()
/// @brief \b Function \b Description:  check if I frame found after Init() or flush().
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): I frame is not found.
/// @retval     -TRUE(1): I frame has been found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsIFrameFound(void)
{
    return MDrv_RVD_IsFwStatusOn(E_RVD_STATUSTYPE_IFRAMEFOUND);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetErrCode()
/// @brief \b Function \b Description:  get error code
/// @return - error code number
//-----------------------------------------------------------------------------
MS_U32 MDrv_RVD_GetErrCode(void)
{
    MS_U32 errCode;

    errCode = HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_ERROR_CODE);

    if (errCode == E_ERROR_OUT_OF_MEMORY)
        return errCode;
    else
        return E_ERROR_NONE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetDbgLevel()
/// @brief \b Function \b Description:  Set Dbg level
/// @param -u8DbgLevel \b IN : enable level
//-----------------------------------------------------------------------------
void MDrv_RVD_SetDbgLevel(MS_U8 u8DbgLevel)
{
    u8RVD_DbgLevel = u8DbgLevel;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetDbgLevel()
/// @brief \b Function \b Description:  Get Dbg level
/// @return - u8RVD_DbgLevel
//-----------------------------------------------------------------------------
MS_U8 MDrv_RVD_GetDbgLevel(VOID)
{
    return u8RVD_DbgLevel;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_IsDbgEnable()
/// @brief \b Function \b Description:  Check the Dbg mode enable or not
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Disable
/// @retval     -TRUE(1): Enable
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_IsDbgEnable(MS_U8 u8DbgLevel)
{
    if (u8RVD_DbgLevel & u8DbgLevel)
        return TRUE;
    else
        return FALSE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetStatus()
/// @brief \b Function \b Description:  Get status of RVD driver
/// @param -pstatus \b OUT : driver status
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_GetStatus(RVD_DrvStatus *pstatus)
{
    pstatus->bInit = u32RVDCtrlMode & RVD_CTRL_INIT_FINISHED;
    pstatus->bBusy = u32RVDCtrlMode & RVD_CTRL_PROCESSING;
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetDispInfo()
/// @brief \b Function \b Description:  Set Par and Crop Info to AP
/// @param -pinfo \b IN : Display Infomation
/// @return - E_RVD_RET_SUCCESS / E_RVD_RET_FAIL
/// @retval     -E_RVD_RET_FAIL(0): FAIL
/// @retval     -E_RVD_RET_SUCCESS(1): SUCCESS
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_GetDispInfo(RVD_FrameInfo *pinfo)
{
    #define PAR_FACTOR 20
    #define SAR_FACTOR 1000

    if (HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DECODECNT))
    {
        MS_U16 u16PicWidth = 0, u16PicHeight = 0;
        MS_U16 u16Frame0Width = 0, u16Frame0Height = 0;

        MDrv_RVD_GetPictureSize(&u16PicWidth ,&u16PicHeight);
        MDrv_RVD_GetFrameSize(&u16Frame0Width ,&u16Frame0Height);
        RVD_MSG_INF("Picture Width = %d Height =%d\n",u16PicWidth,u16PicHeight);
        RVD_MSG_INF("Frame Width = %d Height =%d\n",u16Frame0Width,u16Frame0Height);

        // reset picture size as frame size
        MDrv_RVD_SetPictureSize(u16Frame0Width ,u16Frame0Height);

        pinfo->u16FrameRate = 60000;
        pinfo->u16HorSize = u16Frame0Width; //u16PicWidth;
        pinfo->u16VerSize = u16Frame0Height; //u16PicHeight;
        pinfo->u8par_width = u16PicWidth * PAR_FACTOR / u16Frame0Width;
        pinfo->u8par_height = u16PicHeight* PAR_FACTOR / u16Frame0Height;
        pinfo->u16Sar_width = u16PicWidth * SAR_FACTOR / u16Frame0Width;
        pinfo->u16Sar_height = u16PicHeight* SAR_FACTOR / u16Frame0Height;
        pinfo->u32AspectWidth = (MS_U32) u16PicWidth;
        pinfo->u32AspectHeight = (MS_U32) u16PicHeight;
        pinfo->u16CropBottom = 0;
        pinfo->u16CropTop = 0;
        pinfo->u16CropLeft = 0;
        pinfo->bEnableMIUSel = gRVDInfo.bRVD_FWinMIU1;//(eRvdDynCfgByChip ==128)?TRUE:FALSE;
        pinfo->u32DynScalingAddr = gRVDInfo.u32FW_PhyAddr | HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DS_ADDR);
        pinfo->u8DynScalingDepth = (MS_U8)HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_DS_DEPTH);
        RVD_MSG_INF("DynScalingAddr = 0x%lx DynScalingDepth =%x\n",pinfo->u32DynScalingAddr,pinfo->u8DynScalingDepth);
        if (pinfo->u16HorSize & 0x07)
        {
            pinfo->u16CropRight = 8 - (pinfo->u16HorSize & 0x0007);
            pinfo->u16HorSize = ((pinfo->u16HorSize >> 3) + 1) << 3;
        }
        else
        {
            pinfo->u16CropRight = 0;
        }
        return E_RVD_RET_SUCCESS;
    }
    else
    {
        RVD_MSG_ERR("GetDispInfo fail!!!\n");
        return E_RVD_RET_FAIL;
    }
}

// Fixme
RVD_Result MDrv_RVD_DbgSetCmd(RVD_User_Cmd u32Command, MS_U32 u32Argument)
{
    return MDrv_RVD_SetCmd(u32Command, u32Argument);
}

void MDrv_RVD_DbgSetData(MS_U32 u32Addr, MS_U32 data)
{
    HAL_RVD_QMemWrite(gu32QmemExchangeInfoAddr + u32Addr, data);
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_DbgGetData()
/// @brief \b Function \b Description:  Get DBG Data from Qmem
/// @param -u32Addr \b IN : address of debug data
/// @param -u32Data \b IN : pointer to the debug data
/// @return -return RVD_Result success/fail to get debug data
//------------------------------------------------------------------------------
RVD_Result MDrv_RVD_DbgGetData(MS_U32 u32Addr, MS_U32 *u32Data)
{
    switch (u32Addr)
    {
        case RVD_QMEM_INFO_OFFSET_PIC_WIDTH :
        case RVD_QMEM_INFO_OFFSET_PIC_HEIGHT :
        case RVD_QMEM_INFO_OFFSET_BBU_WPTR :
        case RVD_QMEM_INFO_OFFSET_BBU_RPTR :
        case RVD_QMEM_INFO_OFFSET_BBU_ADDR :
        case RVD_QMEM_INFO_OFFSET_BBU_DEPTH :
        case RVD_QMEM_INFO_PLAY_MODE :
        case RVD_QMEM_INFO_TRICK_MODE:
        case RVD_QMEM_INFO_DISP_FRAME_INFO_ADDR :
        case RVD_QMEM_INFO_IDLECNT :
        case RVD_QMEM_INFO_DISPLAYCNT :
        case RVD_QMEM_INFO_DECODECNT :
        case RVD_QMEM_INFO_MAX_BBU_DEPTH :
        case RVD_QMEM_INFO_DEC_FRAME_INFO_ADDR :
        case RVD_QMEM_INFO_FRAME0_WIDTH :
        case RVD_QMEM_INFO_FRAME0_HEIGHT :
        case RVD_QMEM_INFO_FRAME_RATE :
        case RVD_QMEM_INFO_ERROR_CODE :
        case RVD_QMEM_INFO_COMMAND_ID :
        case RVD_QMEM_INFO_ARGUMENT :
        case RVD_QMEM_INFO_ACTION :
        case RVD_QMEM_INFO_STATUS :
             *u32Data = HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr+u32Addr);
             return E_RVD_RET_SUCCESS;
        default:
             return E_RVD_RET_FAIL;
    }
}

#if defined(REDLION_LINUX_KERNEL_ENVI)
MS_S32 _DRV_RVD_ISRHandler(void)
#else
void _DRV_RVD_ISRHandler(void)
#endif
{
    if (gRVDISRCtrl.pfnISRCallBack != NULL )
    {
        //FIXME LATE
        //gRVDISRCtrl.u32ISRInfo = HAL_HVD_GetData( E_HVD_GDATA_HVD_ISR_STATUS );
        HAL_RVD_Enable_ISR(FALSE);
        gRVDISRCtrl.pfnISRCallBack();
        HAL_RVD_Enable_ISR(TRUE);
    }
    else
    {
        RVD_MSG_ERR("DRV HRVD Err: ISR callback is NULL.\n");
    }
    HAL_RVD_SetClearISR();
    OSAL_RVD_ISR_Enable();
    #if defined(REDLION_LINUX_KERNEL_ENVI)
    return 1;
    #endif
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetISREvent()
/// @brief \b Function \b Description: Set the ISR event type sended by RVD fw.
/// @param -eEvent \b IN : event types
/// @param -fnISRHandler \b IN : function pointer to a interrupt handler.
/// @return -The result of command set ISR event.
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetISREvent(RVD_ISR_Event eEvent, RVD_InterruptCb fnISRHandler)
{
    RVD_Result eRet = E_RVD_RET_ILLEGAL_ACCESS;

    //_DRV_HVD_Inited(eRet);
    _DRV_RVD_Entry();
    if ( eEvent == E_RVD_ISR_NONE )
    {
        HAL_RVD_Enable_ISR(FALSE);
        OSAL_RVD_ISR_Disable();
        OSAL_RVD_ISR_Detach();
        gRVDISRCtrl.bRegISR=FALSE;
        gRVDISRCtrl.pfnISRCallBack=NULL;
        eRet = E_RVD_RET_SUCCESS;
    }
    else
    {
        if (fnISRHandler != NULL)
        {
            HAL_RVD_Enable_ISR(FALSE);
            if ( gRVDISRCtrl.bRegISR )
            {
                OSAL_RVD_ISR_Detach();
                gRVDISRCtrl.bRegISR=FALSE;
            }
            gRVDISRCtrl.pfnISRCallBack = (RVD_ISRCallBack)fnISRHandler;
            OSAL_RVD_ISR_Attach( (void*)_DRV_RVD_ISRHandler );
            OSAL_RVD_ISR_Enable();
/*
            {
                MS_U32 i=0;
                for (i=0; i<68; i++)
                {
                    MsOS_AttachInterrupt(i, (void*)_DRV_HVD_ISRHandler);//(InterruptCb)fnISRHandler);
                    MsOS_EnableInterrupt(i);
                }
            }
*/
            //FIXME LATE
            //RVD_MSG_INF("attach ISR number:%d\n", RVD_ISR_VECTOR);

            // TODO: set ISR event and wait ACK
            //HAL_RVD_Enable_ISR(TRUE);
            gRVDISRCtrl.bRegISR = TRUE;
            eRet = E_RVD_RET_SUCCESS;
        }
        else
        {
            RVD_MSG_ERR("HVD DrvErr: SetISREvent with NULL pointer. ISR type:%lu\n", (MS_U32)eEvent);
            eRet = E_RVD_RET_INVALID_PARAM;
        }
    }
    _DRV_RVD_Return(eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetEnableISR()
/// @brief \b Function \b Description: Enable/Disable RVD interrupt.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable interrupt.
///                 -TRUE(1): Enable interrupt.
/// @return -The result of command set enable ISR.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_SetEnableISR(MS_BOOL bEnable)
{
    HAL_RVD_Enable_ISR(bEnable);
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetForceISR()
/// @brief \b Function \b Description: test the HVD interrupt function. Force FW send one interrupt.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Clear force interrupt status from HK.
///                 -TRUE(1): force one interrupt from HK.
/// @return -The result of command set force ISR.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_SetForceISR(MS_BOOL bEnable)
{
    HAL_RVD_SetForceISR(bEnable);
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_GetISRInfo()
/// @brief \b Function \b Description:  Get information of HVD driver interrupt
/// @param -eType \b OUT : ISR information
/// @return -the result of get ISR information
//-----------------------------------------------------------------------------
MS_BOOL MDrv_RVD_GetISRInfo(RVD_ISR_Event* eType)
{
    //_DRV_HVD_Inited(FALSE);
    *eType = (RVD_ISR_Event)gRVDISRCtrl.u32ISRInfo;
    return TRUE;
}

void MDrv_RVD_Dump_Status(void)
{
    RVD_MSG_DRV("Queue: DispQueNum:%ld Dec:%ld Disp:%ld ESR:%ld ESW:%ld\n" ,
        MDrv_RVD_GetDispQueNum(), MDrv_RVD_GetDecodeCnt(), MDrv_RVD_GetFrameCnt(),
        MDrv_RVD_GetESReadPtr(), MDrv_RVD_GetESWritePtr() );

    RVD_MSG_DRV("Counter: Dec:%ld Disp:%ld Err_Data:%ld Err_Dec:%ld Skip:%ld Drop:%ld Idle:%ld\n" ,
        MDrv_RVD_GetDecodeCnt(), MDrv_RVD_GetFrameCnt(), MDrv_RVD_GetDataErrCnt(),
        MDrv_RVD_GetDecErrCnt(), MDrv_RVD_GetSkipCnt(),MDrv_RVD_GetDropCnt(),
        HAL_RVD_QMemRead(gu32QmemExchangeInfoAddr + RVD_QMEM_INFO_IDLECNT) );

    RVD_MSG_DRV("====================================\n");
}

#if 0
void MDrv_RVD_Dbg_TimeMode(MS_U32 u32Type, MS_U32 u32TimeCnt)
{
    if ((MsOS_GetSystemTime() - u32RvdInitSysTimeBase) >= u32TimeCnt)
    {
        u32RvdInitSysTimeBase = MsOS_GetSystemTime();
        MDrv_RVD_Dump_FW_Status();
    }
}
#endif

MS_BOOL MDrv_RVD_LinkWeakSymbolPatch(void)
{
    return TRUE;
}

void MDrv_RVD_EnableTurboFWMode(MS_BOOL bEnable)
{
    gbEnableRVTurboFWMode = bEnable;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_RVD_SetAVSyncFreerunThreshold()
/// @brief \b Function \b Description:  Set AV sync free run threshold.
/// @param -u32Th \b IN : The value of threshold.
/// @return -E_RVD_RET_SUCCESS :command ready
//-----------------------------------------------------------------------------
RVD_Result MDrv_RVD_SetAVSyncFreerunThreshold(MS_U32 u32Th)
{
     return MDrv_RVD_SetCmd(E_RVD_CMD_FREERUN_THRESHOLD, u32Th);
}


