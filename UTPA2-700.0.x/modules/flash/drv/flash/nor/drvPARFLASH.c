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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
/// file    drvPARFLASH.c
/// @brief  Parallel Flash Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

// Common Definition
#include "MsCommon.h"
#include "drvPARFLASH.h"
#include "MsOS.h"

// Internal Definition
#include "regPARFLASH.h"
#include "halPARFLASH.h"

#include "drvMMIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// TODO: Review OS and non-OS API
#if defined (MSOS_TYPE_NOS)
    #define MsOS_CreateMutex(a, b, c)  1
    #define MsOS_In_Interrupt()     FALSE
    #define MsOS_ObtainMutex(a, b)  TRUE
    #define MsOS_ReleaseMutex(a)
#endif

//--- Mxic --------//
#define PF_MAIC_MANUFACTURE_ID      0xC2
#define PF_MX29GLSERIES_0           0x7E
#define PF_MX29GLSERIES_1           0x01
#define PF_MX29LV400CT              0xB9
#define PF_MX29LV400CB              0xBA
#define PF_MX29LV800CT              0xDA
#define PF_MX29LV800CB              0x5B
#define PF_MX29LV160CT              0xC4
#define PF_MX29LV160CB              0x49
#define PF_MX29LV640BT              0xC9
#define PF_MX29LV640BB              0xCB
#define PF_MX29GL256EH              0x22
#define PF_MX29GA256E_SERIES   		0x38
#define PF_MX29GA128E_SERIES        0x37
#define PF_MX29GAXXXE_H		        0x10 //bit 4
#define PF_MX29GAXXXE_L		        0x00 //bit 4

//---- Spansion -----//
#define PF_SPAN_MANUFACTURE_ID      0x01
#define PF_SPAN_S29SERIES_0         0x227E
#define PF_SPAN_S29GL128P           0x2221
#define PF_SPAN_S29GL256P           0x2222
#define PF_SPAN_S29GL512P           0x2223
#define PF_SPAN_S29GL01GP           0x2228
#define PF_SPAN_S29GL128P_S         0x2237
#define PF_SPAN_S29GL256P_S         0x2238
#define PF_SPAN_S29GL512P_S         0x2239
#define PF_SPAN_S29GL01GP_S         0x2240
#define PF_SPAN_S29SERIES_1         0x2201

//------------ Commends ----------------------
#define PFSH_CMD0                        0xAA
#define PFSH_CMD1                        0x55
#define PFSHF_CMD_RID                    0x90
#define PFSH_CMD_ERASE                   0x80
#define PFSH_CMD_CE                      0x10
#define PFSH_CMD_SE                      0x30
#define PFSH_CMD_PA                      0xA0
#define PFSH_CMD_RESET                   0xF0
#define PFSH_CMD_CFI_READ                0x98

#define PFSH_CMDADDR0                    0xAAA
#define PFSH_CMDADDR1                    0x555
#define PFSH_DEVID_ADDR1                 0x02
#define PFSH_DEVID_ADDR2                 0x1C
#define PFSH_DEVID_ADDR3                 0x1E
#define PFSH_SECUREDID_ADDR              0x06
#define PFSH_SECPROVER_ADDR              0x04

//serial flash mutex wait time
#define PARFLASH_MUTEX_WAIT_TIME    3000
#define SERFLASH_SAFETY_FACTOR      10


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U16 u16CmdAdd0;
    MS_U16 u16CmdAdd1;
    MS_U16 u16DevIDAdd1;
    MS_U16 u16DevIDAdd2;
    MS_U16 u16DevIDAdd3;
    MS_U16 u16SecProVerAddr;
}PARFLASH_CmdAddr_List;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Extern function
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_norf_version = {
    .DDI = { NORF_DRV_VERSION },
};

//---- The sector table of fisrt 64K -----
// Array[0]: number of sectors
// Array[1] ~ Array[n]: sector size, unit is 1K bytes
//-----------------------------------------------------------
static MS_U8 _MX29LV400CB_SECTBL_64K[] = { 4, 16, 8, 8, 32};
static MS_U8 _MX29LV640BT_SECTBL_64K[] = { 8,  8, 8, 8, 8, 8, 8, 8, 8};
//----------------------------------------------------------

//TopBoot,unifirmSec, SecNum, ByteSize, SecTblPtr
static PARFLASH_Content _drv_pf_content[] =
{
    {   1,    0,   11,   0x80000, _MX29LV400CB_SECTBL_64K }, //512K //PF_MX29LV400CT
    {   0,    0,   11,   0x80000, _MX29LV400CB_SECTBL_64K }, //512K //PF_MX29LV400CB
    {   1,    0,   19,  0x100000, _MX29LV400CB_SECTBL_64K }, //1M   //PF_MX29LV800CT
    {   0,    0,   19,  0x100000, _MX29LV400CB_SECTBL_64K }, //1M   //PF_MX29LV800CB
    {   1,    0,   35,  0x200000, _MX29LV400CB_SECTBL_64K }, //2M   //PF_MX29LV160CT
    {   0,    0,   35,  0x200000, _MX29LV400CB_SECTBL_64K }, //2M   //PF_MX29LV160CB
    {   1,    0,  135,  0x800000, _MX29LV640BT_SECTBL_64K }, //8M   //PF_MX29LV640BT
    {   0,    0,  135,  0x800000, _MX29LV640BT_SECTBL_64K }, //8M   //PF_MX29LV640BB
    {   0,    1,  256, 0x2000000,                       0 }, //32M  //PF_MX29GL256EH
    {   0,    1,  128, 0x2000000,                       0 }, //32M  //PF_MX29GA128EL
    {   0,    1,  128, 0x2000000,                       0 }, //32M  //PF_MX29GA128EH
    {   0,    1,  256, 0x2000000,                       0 }, //32M  //PF_MX29GA256EL
    {   0,    1,  256, 0x2000000,                       0 }, //32M  //PF_MX29GA256EH

    {   0,    1,  128, 0x1000000,                       0 }, //16M  //PF_SPAN_S29GL128P
    {   0,    1,  256, 0x2000000,                       0 }, //32M  //PF_SPAN_S29GL256P
    {   0,    1,  512, 0x4000000,                       0 }, //64M  //PF_SPAN_S29GL512P
    {   0,    1, 1024, 0x8000000,                       0 }, //64M  //PF_SPAN_S29GL01GP

    {   0,    1,  128, 0x1000000,                       0 }, //16M  //PF_SPAN_S29GL128P_S
    {   0,    1,  256, 0x2000000,                       0 }, //32M  //PF_SPAN_S29GL256P_S
    {   0,    1,  512, 0x4000000,                       0 }, //64M  //PF_SPAN_S29GL512P_S


};

//default value is word mode commend
static PARFLASH_CmdAddr_List _pfsh_cmdaddr_list =
{
    .u16CmdAdd0 = PFSH_CMDADDR0,
    .u16CmdAdd1 = PFSH_CMDADDR1,
    .u16DevIDAdd1 = PFSH_DEVID_ADDR1,
    .u16DevIDAdd2 = PFSH_DEVID_ADDR2,
    .u16DevIDAdd3 = PFSH_DEVID_ADDR3,
    .u16SecProVerAddr = PFSH_SECPROVER_ADDR,
};

static PARFLASH_Info _ParFlashInfo =
{
    .enDevice = evPF_NUM,
    .bbytemode = 0,
    .u16SecNum = 0,
    .u32TotBytes = 0,
};

static MS_U32 _u32pfsh_CmdAddAry[PFSH_MAX_CMDRUN];
static MS_U16 _u16pfsh_CmdDataAry[PFSH_MAX_CMDRUN];

static MS_S32 _s32ParFlash_Mutex;
static PARFLASH_DevSts _ParFlashDrvStatus;

static MS_U8 _u81KSecTblShift = 9;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//if byte mode, return byte size; if word mode, return word size
static MS_U32 _Drv_PARFLASH_GetSectorSize(MS_U16 u16sector)
{
    MS_U32 u32size = 64;
    MS_U8* pu8tr = 0;
    MS_U16 u16temp;

    if(_ParFlashInfo.enDevice >= evPF_NUM)
    {
        printf("_ParFlashInfo.enDevice is invalid\n");
        return 0;
    }

    if(_drv_pf_content[_ParFlashInfo.enDevice].bUniformSecTbl)
    {
        u32size <<= (MS_U32)(_u81KSecTblShift & 0xFF);
        return u32size;
    }

    pu8tr = _drv_pf_content[_ParFlashInfo.enDevice].pu8SecTbl;
    if(_drv_pf_content[_ParFlashInfo.enDevice].bTopBoot)
    {
        if(u16sector >= (_drv_pf_content[_ParFlashInfo.enDevice].u16SecNum - (MS_U16)(pu8tr[0] & 0xFF)))
        {
            u16temp = (MS_U16)(pu8tr[0] & 0xFF) - _drv_pf_content[_ParFlashInfo.enDevice].u16SecNum - u16sector + 1;
            u32size = (MS_U32)(pu8tr[u16temp] & 0xFF);
        }
    }
    else
    {
        if(u16sector < (MS_U16)(pu8tr[0] & 0xFF))
            u32size = (MS_U32)(pu8tr[++u16sector] & 0xFF);
    }
    u32size <<= (MS_U32)_u81KSecTblShift;

    return u32size;
}

//if byte mode, return byte offset; if word mode, return word offset
static MS_U32 _Drv_PARFLASH_GetSectorOffset(MS_U16 u16sector)
{
    MS_U32 u32SecOffset = 0;
    MS_U8* pu8tr = 0;
    MS_U16 u16LastUnif, u16ii = 0;

    if(_ParFlashInfo.enDevice >= evPF_NUM)
        return 0xFFFFFFFF;

    if(_drv_pf_content[_ParFlashInfo.enDevice].bUniformSecTbl)
    {
        u32SecOffset = (u16sector << 6) << _u81KSecTblShift; //64K sector
        return u32SecOffset;
    }

    pu8tr = _drv_pf_content[_ParFlashInfo.enDevice].pu8SecTbl;
    if(_drv_pf_content[_ParFlashInfo.enDevice].bTopBoot)
    {
        u16LastUnif = _drv_pf_content[_ParFlashInfo.enDevice].u16SecNum - (MS_U16)(pu8tr[0] & 0xFF) - 1;
        //caclulate uniform sector
        if(u16sector <= u16LastUnif)
            u16ii = u16sector;
        else
            u16ii = u16LastUnif;
        u32SecOffset = (MS_U32)(u16ii & 0xFFFF) << 6; //64K sector
        //caclulate un-uniform sector
        while(++u16ii <= u16sector)
            u32SecOffset += pu8tr[u16ii - u16LastUnif];
    }
    else
    {
        //caclulate un-uniform sector
        while((u16ii < u16sector) && (u16ii < (MS_U16)(pu8tr[0] & 0xFF)))
            u32SecOffset += (MS_U32)(pu8tr[++u16ii] & 0xFF);
        //caclulate uniform sector
        if(u16ii < u16sector)
            u32SecOffset += ((MS_U32)((u16sector - u16ii) & 0xFFFF) << 6); //64K sector
    }

    return (u32SecOffset << _u81KSecTblShift);
}
//--- for future using -----
#if 0
static MS_U16 _Drv_PARFLASH_GetSectorIndex(MS_U32 u32addr)
{
    MS_U16 u16SecIdx = 0;
    MS_U8* pu8tr = 0, u8ii;
    MS_U32 u32UnUnitOffset, u32Offset = 0;

    if(_ParFlashInfo.enDevice >= evPF_NUM)
        return 0xFFFF;

    if(_drv_pf_content[_ParFlashInfo.enDevice].bUniformSecTbl)
    {
        u16SecIdx = (MS_U16)((u32addr >> (MS_U32)(_u81KSecTblShift & 0xFF)) >> 6);
        return u16SecIdx;
    }

    pu8tr = _drv_pf_content[_ParFlashInfo.enDevice].pu8SecTbl;
    for(u8ii = 1; u8ii <= pu8tr[0]; u8ii++)
        u32UnUnitOffset = (MS_U32)(pu8tr[u8ii] & 0xFF) << (MS_U32)(_u81KSecTblShift & 0xFF);

    //uniform sector
    if(_drv_pf_content[_ParFlashInfo.enDevice].bTopBoot)
    {
        if(_ParFlashInfo.bbytemode)
            u32UnUnitOffset = _ParFlashInfo.u32TotBytes - u32UnUnitOffset;
        else
            u32UnUnitOffset = (_ParFlashInfo.u32TotBytes >> 1) - u32UnUnitOffset;
        if(u32addr <= u32UnUnitOffset)
        {
            u16SecIdx = (u32addr >> (MS_U32)(_u81KSecTblShift & 0xFF)) >> 6;
            return u16SecIdx;
        }
        else
            u16SecIdx = (u32UnUnitOffset >> (MS_U32)(_u81KSecTblShift & 0xFF)) >> 6;
    }
    else
    {
        if(u32addr >= u32UnUnitOffset)
        {
            u16SecIdx = (MS_U16)(pu8tr[0] & 0xFF);
            u16SecIdx += (((u32addr - u32UnUnitOffset) >> (MS_U32)(_u81KSecTblShift & 0xFF)) >> 6);
            return u16SecIdx;
        }
    }

    //un-uniform sector
    for(u8ii = 1; u8ii <= pu8tr[0]; u8ii++)
    {
        if(u32addr >= u32Offset)
            break;
        u32Offset += ((MS_U32)(pu8tr[u8ii] & 0xFF) << (MS_U32)(_u81KSecTblShift & 0xFF));
        u16SecIdx++;
    }

    return u16SecIdx;
}
#endif

static MS_BOOL _Drv_PARFLASH_Reset(void)
{
    _u32pfsh_CmdAddAry[3] = 0;
    _u16pfsh_CmdDataAry[3] = PFSH_CMD_RESET;

    if(!HAL_PARFLASH_Cmd_Write(1, &(_u32pfsh_CmdAddAry[3]), &(_u16pfsh_CmdDataAry[3]))) //1 runs, direct mode
        return FALSE;

    return TRUE;
}

static MS_BOOL _Drv_PARFLASH_Toggle(MS_U32 u32dst)
{
    MS_U16 u16data, u16data2;
    MS_BOOL  bresult = FALSE;

    while (TRUE)
    {
        if(!HAL_PARFLASH_Read(u32dst, &u16data))
            return FALSE;
        if(!HAL_PARFLASH_Read(u32dst, &u16data2))
            return FALSE;
        //printf("tog 0 %x %x\n", u16data, u16data2);

        if ((u16data&0x40) != (u16data2&0x40)) //Q6 toggleing
        {
            if ((u16data2 & 0x20) == 0x20) // Q5 is 1, exceed time limit
            {
                if(!HAL_PARFLASH_Read(u32dst, &u16data))
                    return FALSE;
                if(!HAL_PARFLASH_Read(u32dst, &u16data2))
                    return FALSE;

                if ((u16data&0x40) != (u16data2&0x40)) //Q6 toggleing
                {
                    _Drv_PARFLASH_Reset(); // Toggle bit algorithm exceed time limit
                    return FALSE;;
                }
                else
                {
                    return TRUE; // Stop toggle.
                }
            }
        }
        else
        {
            return TRUE;
        }
    }

    return bresult;
}

static MS_BOOL _Drv_PARFLASH_ReadID(MS_U32 u32IDAddr, MS_U16* pu16ID)
{
    *pu16ID = 0;

    _u32pfsh_CmdAddAry[2] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u16pfsh_CmdDataAry[2] = PFSHF_CMD_RID;

    //read ID
    if(!HAL_PARFLASH_Cmd_Write(3, _u32pfsh_CmdAddAry, _u16pfsh_CmdDataAry)) //3 runs, direct mode
        return FALSE;

    if(!HAL_PARFLASH_Read(u32IDAddr, pu16ID))
        return FALSE;

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("ID addr %x, data %04x\n", (unsigned int)u32IDAddr, (unsigned short)(*pu16ID)));

    if(!_Drv_PARFLASH_Reset())
        return FALSE;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Get the information of Parallel Flash
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
const PARFLASH_Info *MDrv_PARFLASH_GetInfo(void)
{
    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO,\
                    printf("MDrv_PARFLASH_GetInfo()\n\
                           \tenDevice = %d\n\
                           \tbbytemode = %d\n\
                           \tu16SecNum   = %d\n\
                           \tu32TotBytes      = %d\n",\
                           (int)(_ParFlashInfo.enDevice),
                           (int)(_ParFlashInfo.bbytemode),
                           (int)(_ParFlashInfo.u16SecNum),
                           (int)(_ParFlashInfo.u32TotBytes)
                           )
                    );

    return &_ParFlashInfo;
}

//------------------------------------------------------------------------------
/// Description: Show the NORF driver version
/// @param  ppVersion \b OUT: output NORF driver version
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_norf_version;

    return TRUE;
}

//------------------------------------------------------------------------------
/// Get Parallel Flash driver status
/// @param  pDrvStatus \b OUT: poniter to store the returning driver status
/// @return TRUE : succeed
/// @return FALSE : failed to get the driver status
//------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_GetStatus(PARFLASH_DevSts* pDrvStatus)
{
    memcpy(pDrvStatus, &_ParFlashDrvStatus, sizeof(_ParFlashDrvStatus));

    return TRUE;
}


//------------------------------------------------------------------------------
/// Set detailed level of Parallel Flash driver debug message
/// @param u8DbgLevel  \b IN  debug level for Parallel Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8ParFlashDbgLevel = u8DbgLevel;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize Parallel Flash
/// @return None
/// @note   called only once at system initialization
//-------------------------------------------------------------------------------------------------
void MDrv_PARFLASH_Init(void)
{
    MS_U32 u32NonPMBank, u32NonPMBankSize;
	MS_U32 u32PMBank, u32PMBankSize;

    _u8ParFlashDbgLevel = PARFLASH_DBGLV_INFO; // init debug level first

    _s32ParFlash_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex ParFlash", MSOS_PROCESS_SHARED);
    MS_ASSERT(_s32ParFlash_Mutex >= 0);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    //
    //  1. HAL init
    //
    if (!MDrv_MMIO_GetBASE( &u32NonPMBank, &u32NonPMBankSize, MS_MODULE_PFSH))
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("IOMap failure to get MS_MODULE_PFSH\n"));
        u32NonPMBank = BASEADDR_RIU;
    }
	if (!MDrv_MMIO_GetBASE( &u32PMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("IOMap failure to get MS_MODULE_PIU\n"));
        u32PMBank = BASEADDR_RIU;
    }
    HAL_PARFLASH_Config(u32PMBank, u32NonPMBank);

    HAL_PARFLASH_Init(&(_ParFlashInfo.bbytemode));

    //
    //  2. init other data structure of Parallel Flash driver
    //
    if(_ParFlashInfo.bbytemode)
    {
        _u81KSecTblShift = 10;
        _pfsh_cmdaddr_list.u16CmdAdd0 = PFSH_CMDADDR0;
        _pfsh_cmdaddr_list.u16CmdAdd1 = PFSH_CMDADDR1;
        _pfsh_cmdaddr_list.u16DevIDAdd1 = PFSH_DEVID_ADDR1;
        _pfsh_cmdaddr_list.u16DevIDAdd2 = PFSH_DEVID_ADDR2;
        _pfsh_cmdaddr_list.u16DevIDAdd3 = PFSH_DEVID_ADDR3;
        _pfsh_cmdaddr_list.u16SecProVerAddr = PFSH_SECPROVER_ADDR;
    }
    else
    {
        _u81KSecTblShift = 9;
        _pfsh_cmdaddr_list.u16CmdAdd0 = PFSH_CMDADDR0 >> 1;
        _pfsh_cmdaddr_list.u16CmdAdd1 = PFSH_CMDADDR1 >> 1;
        _pfsh_cmdaddr_list.u16DevIDAdd1 = PFSH_DEVID_ADDR1 >> 1;
        _pfsh_cmdaddr_list.u16DevIDAdd2 = PFSH_DEVID_ADDR2 >> 1;
        _pfsh_cmdaddr_list.u16DevIDAdd3 = PFSH_DEVID_ADDR3 >> 1;
        _pfsh_cmdaddr_list.u16SecProVerAddr = PFSH_SECPROVER_ADDR >> 1;
    }

    //-- init commend data array
    _u32pfsh_CmdAddAry[0] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u32pfsh_CmdAddAry[1] = _pfsh_cmdaddr_list.u16CmdAdd1;
    _u16pfsh_CmdDataAry[0] = PFSH_CMD0;
    _u16pfsh_CmdDataAry[1] = PFSH_CMD1;

    _ParFlashDrvStatus.bIsBusy = FALSE;

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);
}

//-------------------------------------------------------------------------------------------------
/// Select chip
/// @param  u8cs    \b IN: Flash index. value: 0~1
/// @return FALSE : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_ChipSelect(MS_U8 u8cs)
{
    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%d)\n", __FUNCTION__, (int)u8cs));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_ParFlash_Reset fails!\n"));
        return FALSE;
    }

    _ParFlashInfo.bbytemode = HAL_PARFLASH_SelectChip(u8cs);

    if(_ParFlashInfo.bbytemode)
    {
        _u81KSecTblShift = 10;
        _pfsh_cmdaddr_list.u16CmdAdd0 = PFSH_CMDADDR0;
        _pfsh_cmdaddr_list.u16CmdAdd1 = PFSH_CMDADDR1;
        _pfsh_cmdaddr_list.u16DevIDAdd1 = PFSH_DEVID_ADDR1;
        _pfsh_cmdaddr_list.u16DevIDAdd2 = PFSH_DEVID_ADDR2;
        _pfsh_cmdaddr_list.u16DevIDAdd3 = PFSH_DEVID_ADDR3;
        _pfsh_cmdaddr_list.u16SecProVerAddr = PFSH_SECPROVER_ADDR;
    }
    else
    {
        _u81KSecTblShift = 9;
        _pfsh_cmdaddr_list.u16CmdAdd0 = PFSH_CMDADDR0 >> 1;
        _pfsh_cmdaddr_list.u16CmdAdd1 = PFSH_CMDADDR1 >> 1;
        _pfsh_cmdaddr_list.u16DevIDAdd1 = PFSH_DEVID_ADDR1 >> 1;
        _pfsh_cmdaddr_list.u16DevIDAdd2 = PFSH_DEVID_ADDR2 >> 1;
        _pfsh_cmdaddr_list.u16DevIDAdd3 = PFSH_DEVID_ADDR3 >> 1;
        _pfsh_cmdaddr_list.u16SecProVerAddr = PFSH_SECPROVER_ADDR >> 1;
    }

    //-- init commend data array
    _u32pfsh_CmdAddAry[0] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u32pfsh_CmdAddAry[1] = _pfsh_cmdaddr_list.u16CmdAdd1;
    _u16pfsh_CmdDataAry[0] = PFSH_CMD0;
    _u16pfsh_CmdDataAry[1] = PFSH_CMD1;

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Reset Parallel Flash
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_Reset(void)
{
    MS_BOOL bres = FALSE;

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_ParFlash_Reset fails!\n"));
        return FALSE;
    }

    bres = _Drv_PARFLASH_Reset();

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return bres;
}

//-------------------------------------------------------------------------------------------------
/// Read ID in Parallel Flash
/// @param  pu16Data    \b IN: ID data, max length is 4 words
/// @param  pu8r_len    \b IN: ID data count
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_ReadID(MS_U16 *pu16Data, MS_U8 *pu8r_len)
{
    MS_U16 u16data;
    MS_BOOL bres = FALSE;
    MS_U8 u8len = 0;

    *pu8r_len = 0;

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%p, %p)\n", __FUNCTION__, pu16Data, pu8r_len));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_PARFLASH_ReadID fails!\n"));
        return FALSE;
    }

    //-- read manufacture ID -----
    if(!(bres = _Drv_PARFLASH_ReadID(0, &u16data)))
        goto END;
    pu16Data[u8len++] = u16data;

    //--- device id 1 ------
    if(!(bres = _Drv_PARFLASH_ReadID((MS_U32)(_pfsh_cmdaddr_list.u16DevIDAdd1 & 0xFFFF), &u16data)))
        goto END;
    pu16Data[u8len++] = u16data;

    if(pu16Data[0] == PF_SPAN_MANUFACTURE_ID ||
            (pu16Data[0] == PF_MAIC_MANUFACTURE_ID && ((pu16Data[1] & 0xFF)  == PF_MX29GLSERIES_0)))
    {
        //--- device id 2 ------
        if(!(bres = _Drv_PARFLASH_ReadID((MS_U32)(_pfsh_cmdaddr_list.u16DevIDAdd2 & 0xFFFF), &u16data)))
            goto END;
        pu16Data[u8len++] = u16data;

        //--- device id 3 ------
        if(!(bres = _Drv_PARFLASH_ReadID((MS_U32)(_pfsh_cmdaddr_list.u16DevIDAdd3 & 0xFFFF), &u16data)))
            goto END;
        pu16Data[u8len++] = u16data;

     	//--- Secured Silicon ------
		if(!(bres = _Drv_PARFLASH_ReadID((MS_U32)(PFSH_SECUREDID_ADDR & 0xFFFF), &u16data)))
            goto END;
        pu16Data[u8len++] = u16data;
    }

    _ParFlashInfo.enDevice = evPF_NUM; //unknown type
    _ParFlashInfo.u16SecNum = 0;
    _ParFlashInfo.u32TotBytes = 0;
    if(pu16Data[0] == PF_MAIC_MANUFACTURE_ID)
    {
        switch((pu16Data[1] & 0xFF))
        {
            case PF_MX29LV400CT:
                _ParFlashInfo.enDevice = enPF_MX29LV400CT;
                _ParFlashInfo.u16SecNum = 11;
                _ParFlashInfo.u32TotBytes = 524288;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV400CT\n"));
                break;
            case PF_MX29LV400CB:
                _ParFlashInfo.enDevice = enPF_MX29LV400CB;
                _ParFlashInfo.u16SecNum = 11;
                _ParFlashInfo.u32TotBytes = 524288;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV400CB\n"));
                break;
            case PF_MX29LV800CT:
                _ParFlashInfo.enDevice = enPF_MX29LV800CT;
                _ParFlashInfo.u16SecNum = 19;
                _ParFlashInfo.u32TotBytes = 1048576;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV800CT\n"));
                break;
            case PF_MX29LV800CB:
                _ParFlashInfo.enDevice = enPF_MX29LV800CB;
                _ParFlashInfo.u16SecNum = 19;
                _ParFlashInfo.u32TotBytes = 1048576;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV800CB\n"));
                break;
            case PF_MX29LV160CT:
                _ParFlashInfo.enDevice = enPF_MX29LV160CT;
                _ParFlashInfo.u16SecNum = 35;
                _ParFlashInfo.u32TotBytes = 2097152;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV160CT\n"));
                break;
            case PF_MX29LV160CB:
                _ParFlashInfo.enDevice = enPF_MX29LV160CB;
                _ParFlashInfo.u16SecNum = 35;
                _ParFlashInfo.u32TotBytes = 2097152;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV160CB\n"));
                break;
            case PF_MX29LV640BT:
                _ParFlashInfo.enDevice = enPF_MX29LV640BT;
                _ParFlashInfo.u16SecNum = 135;
                _ParFlashInfo.u32TotBytes = 0x800000;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV640BT\n"));
                break;
            case PF_MX29LV640BB:
                _ParFlashInfo.enDevice = enPF_MX29LV640BB;
                _ParFlashInfo.u16SecNum = 256;
                _ParFlashInfo.u32TotBytes = 0x2000000;
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29LV640BB\n"));
                break;
            case PF_MX29GLSERIES_0:
                if((pu16Data[2] & 0xFF) == PF_MX29GL256EH)
                {
                    _ParFlashInfo.enDevice = enPF_MX29GL256EH;
                    _ParFlashInfo.u16SecNum = 256;
                    _ParFlashInfo.u32TotBytes = 0x2000000;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29GL256EH\n"));
                }
				else if((pu16Data[2] & 0xFF) == PF_MX29GA256E_SERIES)
				{
					if(pu16Data[3] & PF_MX29GAXXXE_H)
					{
						_ParFlashInfo.enDevice = enPF_MX29GA256EH;
						DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29GA256EH\n"));
					}
					else
					{
						_ParFlashInfo.enDevice = enPF_MX29GA256EL;
						DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29GA256EL\n"));
					}
					_ParFlashInfo.u16SecNum = 256;
                    _ParFlashInfo.u32TotBytes = 0x2000000;
				}
				else if((pu16Data[2] & 0xFF) == PF_MX29GA128E_SERIES)
				{
					if(pu16Data[3] & PF_MX29GAXXXE_H)
					{
						_ParFlashInfo.enDevice = enPF_MX29GA128EH;
						DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29GA128EH\n"));
					}
					else
					{
						_ParFlashInfo.enDevice = enPF_MX29GA128EL;
						DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MX29GA128EL\n"));
					}
					_ParFlashInfo.u16SecNum = 128;
                    _ParFlashInfo.u32TotBytes = 16777216;
				}
                else
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: PF_MX29GLSERIES_0 Unknown\n"));
                break;
            default:
                DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: MXIC Unkonown\n"));
                break;
        }
    }
    else if(pu16Data[0] == PF_SPAN_MANUFACTURE_ID)
    {
        if(pu16Data[1] == PF_SPAN_S29SERIES_0)
        {
            switch(pu16Data[2])
            {
                case PF_SPAN_S29GL128P:
                    _ParFlashInfo.enDevice = enPF_SPAN_S29GL128P;
                    _ParFlashInfo.u16SecNum = 128;
                    _ParFlashInfo.u32TotBytes = 16777216;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: S29GL128P\n"));
                    break;
                case PF_SPAN_S29GL256P:
                    _ParFlashInfo.enDevice = enPF_SPAN_S29GL256P;
                    _ParFlashInfo.u16SecNum = 256;
                    _ParFlashInfo.u32TotBytes = 33554432;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: S29GL256P\n"));
                    break;
                case PF_SPAN_S29GL512P:
                    _ParFlashInfo.enDevice = enPF_SPAN_S29GL512P;
                    _ParFlashInfo.u16SecNum = 512;
                    _ParFlashInfo.u32TotBytes = 67108864;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: S29GL512P\n"));
                    break;
                case PF_SPAN_S29GL01GP:
                    _ParFlashInfo.enDevice = enPF_SPAN_S29GL01GP;
                    _ParFlashInfo.u16SecNum = 1024;
                    _ParFlashInfo.u32TotBytes = 134217728;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: S29GL01GP\n"));
                    break;
                case PF_SPAN_S29GL128P_S:
                    _ParFlashInfo.enDevice = enPF_SPAN_S29GL128P_S;
                    _ParFlashInfo.u16SecNum = 128;
                    _ParFlashInfo.u32TotBytes = 16777216;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: S29GL128P_S\n"));
                    break;
                case PF_SPAN_S29GL256P_S:
                    _ParFlashInfo.enDevice = enPF_SPAN_S29GL256P_S;
                    _ParFlashInfo.u16SecNum = 256;
                    _ParFlashInfo.u32TotBytes = 33554432;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: S29GL256P_S\n"));
                    break;
                case PF_SPAN_S29GL512P_S:
                    _ParFlashInfo.enDevice = enPF_SPAN_S29GL512P_S;
                    _ParFlashInfo.u16SecNum = 512;
                    _ParFlashInfo.u32TotBytes = 67108864;
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: S29GL512P_S\n"));

                    break;
                default:
                    DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: SPANSION S29SERIES Unknown\n"));
                    break;
            }
        }
        else
            DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: SPANSION Unknown\n"));
    }
    else
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_INFO, printf("Nor Flash: Unknown\n"));

    *pu8r_len = u8len;

    bres = TRUE;

END:

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("PFSH ID (%d): %04x %04x %04x %04x\n",
        (int)(*pu8r_len & 0xFF), pu16Data[0], pu16Data[1], pu16Data[2], pu16Data[3]));

    return bres;
}

//-------------------------------------------------------------------------------------------------
/// Read Sector protect condition
/// @param  u16sec      \b IN: Sector index
/// @param  pbProtect    \b Out: BOOL point to store protect condition.
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_SectorProtectVerify(MS_U16 u16sec, MS_BOOL* pbProtect)
{
    MS_BOOL bres = FALSE;
    MS_U16 u16data;
    MS_U32 u32secaddr;

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%04x, %p)\n", __FUNCTION__, u16sec, pbProtect));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_ParFlash_ReadID fails!\n"));
        return FALSE;
    }

    u32secaddr = _Drv_PARFLASH_GetSectorOffset(u16sec) | (MS_U32)(_pfsh_cmdaddr_list.u16SecProVerAddr & 0xFFFF);

    //-- read manufacture ID -----
    if(!(bres = _Drv_PARFLASH_ReadID(u32secaddr, &u16data)))
        goto END;

    *pbProtect = (MS_BOOL)u16data;

    bres = TRUE;

END:

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return bres;
}

//-------------------------------------------------------------------------------------------------
/// Read Sector protect condition
/// @param  u8addr      \b IN: CFI read address
/// @param  pu16data    \b IN: pointer to store CFI read data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_CFI_Read(MS_U8 u8addr, MS_U16* pu16data)
{
    MS_BOOL bres = FALSE;

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%02x, %p)\n", __FUNCTION__, u8addr, pu16data));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_PARFLASH_ReadID fails!\n"));
        return FALSE;
    }

    _u32pfsh_CmdAddAry[2] = (MS_U32)(_pfsh_cmdaddr_list.u16CmdAdd0 & 0xFF);
    _u16pfsh_CmdDataAry[2] = PFSH_CMD_CFI_READ;

    if(!(bres = HAL_PARFLASH_Cmd_Write(1, &(_u32pfsh_CmdAddAry[2]), &(_u16pfsh_CmdDataAry[2]))))
        goto END;

    if(!HAL_PARFLASH_Read((MS_U32)(u8addr & 0xFF), pu16data))
        goto END;

    bres = _Drv_PARFLASH_Reset();

END:

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return bres;
}


//-------------------------------------------------------------------------------------------------
/// Erase all sectors in Parallel Flash
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_EraseChip(void)
{
    MS_BOOL bres = FALSE;

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_PARFLASH_EraseSec fails!\n"));
        return FALSE;
    }

    _u32pfsh_CmdAddAry[2] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u16pfsh_CmdDataAry[2] = PFSH_CMD_ERASE;
    _u32pfsh_CmdAddAry[3] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u16pfsh_CmdDataAry[3] = PFSH_CMD0;
    _u32pfsh_CmdAddAry[4] = _pfsh_cmdaddr_list.u16CmdAdd1;
    _u16pfsh_CmdDataAry[4] = PFSH_CMD1;
    _u32pfsh_CmdAddAry[5] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u16pfsh_CmdDataAry[5] = PFSH_CMD_CE;

    if(!(bres = HAL_PARFLASH_Cmd_Write(6, _u32pfsh_CmdAddAry, _u16pfsh_CmdDataAry)))
        goto END;

    bres = _Drv_PARFLASH_Toggle(0);

END:

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return TRUE ;
}

//-------------------------------------------------------------------------------------------------
/// Erase certain sectors in Parallel Flash
/// @param  u16StartSec    \b IN: start sector
/// @param  u16EndSec      \b IN: end sector
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_EraseSec(MS_U16 u16StartSec, MS_U16 u16EndSec)
{
    MS_BOOL bres = FALSE;
    MS_U16 u16loop;

    if(_ParFlashInfo.enDevice >= evPF_NUM)
        return false;

    if ( u16StartSec > u16EndSec || u16EndSec >= _drv_pf_content[_ParFlashInfo.enDevice].u16SecNum)
    {
        return FALSE;
    }

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(0x%04X, 0x%04X)\n", __FUNCTION__, (int)u16StartSec, (int)u16EndSec));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_PARFLASH_EraseSec fails!\n"));
        return FALSE;
    }

    _u32pfsh_CmdAddAry[2] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u16pfsh_CmdDataAry[2] = PFSH_CMD_ERASE;
    _u32pfsh_CmdAddAry[3] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u16pfsh_CmdDataAry[3] = PFSH_CMD0;
    _u32pfsh_CmdAddAry[4] = _pfsh_cmdaddr_list.u16CmdAdd1;
    _u16pfsh_CmdDataAry[4] = PFSH_CMD1;
    _u32pfsh_CmdAddAry[5] = 0;
    _u16pfsh_CmdDataAry[5] = PFSH_CMD_SE;

    for (u16loop = u16StartSec; u16loop <= u16EndSec; u16loop++)
    {
        _u32pfsh_CmdAddAry[5] = _Drv_PARFLASH_GetSectorOffset(u16loop);

        if(!(bres = HAL_PARFLASH_Cmd_Write(6, _u32pfsh_CmdAddAry, _u16pfsh_CmdDataAry)))
            goto END;
        if(!(bres = _Drv_PARFLASH_Toggle(_u32pfsh_CmdAddAry[5])))
            goto END;
    }

    bres = TRUE;

END:

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return bres ;
}

//-------------------------------------------------------------------------------------------------
/// Write data to Parallel Flash
/// @param  u32Addr \b IN: start address (4-B aligned)
/// @param  pu8Data \b IN: data to be written (2-B aligned)
/// @param  u32Size \b IN: size in Bytes (4-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_Write(MS_U32 u32Addr, MS_U8 *pu8Data, MS_U32 u32Size)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32ii, u32Satrt;
    MS_U16 u16temp;
    MS_U8* pu8tr = pu8Data;

    MS_ASSERT(u32Addr+u32Size <= _ParFlashInfo.u32TotBytes);
    MS_ASSERT(u32Addr%4 == 0);
    MS_ASSERT(u32Size%4 == 0);
    MS_ASSERT(u32Size >= 4);
    MS_ASSERT((MS_U32)pu8Data%2 == 0);

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(0x%08x,%p,%d)\n", __FUNCTION__, (unsigned int)u32Addr, (void*)pu8Data,(int)u32Size));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_PARFLASH_Write fails!\n"));
        return FALSE;
    }

    _u32pfsh_CmdAddAry[2] = _pfsh_cmdaddr_list.u16CmdAdd0;
    _u16pfsh_CmdDataAry[2] = PFSH_CMD_PA;

    if(!HAL_PARFLASH_PrepareCmdWrite(3, _u32pfsh_CmdAddAry, _u16pfsh_CmdDataAry))
        goto END;

    u32Satrt = u32Addr;
    if(!_ParFlashInfo.bbytemode)
        u32Satrt >>= 1;
    for(u32ii = 0; u32ii < u32Size; u32ii++)
    {
        u16temp = (MS_U16)(*pu8tr & 0xFF);
        if(!_ParFlashInfo.bbytemode)
        {
            u32ii++;
            pu8tr++;
            u16temp |= ((MS_U16)(*pu8tr & 0xFF) << 8);
        }
        //printf("write data (%d): %04x\n", (unsigned int)u32Satrt, u16temp);
        if(!HAL_PARFLASH_LastCmdTrig(4, u32Satrt, u16temp))
            goto END;

        if(!_Drv_PARFLASH_Toggle(u32Satrt))
            goto END;

        pu8tr++;
        u32Satrt++;
    }

    bRet = TRUE;

END:
    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Read data from Parallel Flash
/// @param  u32Addr \b IN: start address (4-B aligned)
/// @param  pu8Data \b OUT: data ptr to store the read data (2-B aligned)
/// @param  u32Size \b IN: size in Bytes (4-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_Read(MS_U32 u32Addr, MS_U8 *pu8Data, MS_U32 u32Size)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32ii, u32Satrt;
    MS_U16 u16temp;
    MS_U8* pu8tr = pu8Data;

    MS_ASSERT(u32Addr+u32Size <= _ParFlashInfo.u32TotBytes);
    MS_ASSERT(u32Addr%4 ==0);
    MS_ASSERT(u32Size%4 ==0);
    MS_ASSERT(u32Size >= 4);
    MS_ASSERT((MS_U32)pu8Data%2 == 0);

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(0x%08x,%p, %d)\n", __FUNCTION__, (unsigned int)u32Addr,pu8Data, (int)u32Size));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_PARFLASH_Read fails!\n"));
        return FALSE;
    }

    u32Satrt = u32Addr;
    if(!_ParFlashInfo.bbytemode)
        u32Satrt >>= 1;
    for(u32ii = 0; u32ii < u32Size; u32ii++)
    {
        if(!HAL_PARFLASH_Read(u32Satrt, &u16temp))
            goto END;
        //printf("read dat (%d): %04x\n", (unsigned int)u32Satrt, u16temp);
        *pu8tr = (MS_U8)(u16temp & 0xFF);
        pu8tr++;
        if(!_ParFlashInfo.bbytemode)
        {
            *pu8tr = (MS_U8)((u16temp >> 8) & 0xFF);
            pu8tr++;
            u32ii++;
        }
        u32Satrt++;
    }

    bRet = TRUE;

END:
    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Write write commend directly
/// @param  u8cycles   \b IN: Bus cycles of commend. Maximun value is 8.
/// @param  pu32Addrs  \b IN: Commend Address array to be written.
/// @param  pu16Data   \b IN: Commend Data array to be written.
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_WriteCmdCycle(MS_U8 u8cycles, MS_U32 *pu32Addrs, MS_U16 *pu16Data)
{
    MS_BOOL bRet = FALSE;

    if(u8cycles > 8)
        return FALSE;

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    DEBUG_PAR_FLASH(PARFLASH_DBGLV_DEBUG, printf("%s(%d, %p, %p)\n", __FUNCTION__, (int)u8cycles,pu32Addrs,pu16Data));

    if (MsOS_ObtainMutex(_s32ParFlash_Mutex, PARFLASH_MUTEX_WAIT_TIME) == FALSE)
    {
        DEBUG_PAR_FLASH(PARFLASH_DBGLV_ERR, printf("ObtainMutex in MDrv_PARFLASH_Read fails!\n"));
        return FALSE;
    }

    bRet = HAL_PARFLASH_Cmd_Write(u8cycles, pu32Addrs, pu16Data);

    MsOS_ReleaseMutex(_s32ParFlash_Mutex);

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Getting Sector Offset address in byte unit
/// @param  u16sector   \b IN: Sector index
/// @return unsinged long value: Offset address
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PARFLASH_GetSectorOffset(MS_U16 u16sector)
{
    MS_U32 u32offset;

    u32offset = _Drv_PARFLASH_GetSectorOffset(u16sector);

    if(!_ParFlashInfo.bbytemode)
        u32offset <<= 1;

    return u32offset;
}

//-------------------------------------------------------------------------------------------------
/// Getting Sector size in byte unit
/// @param  u16startsec   \b IN: Start Sector index
/// @param  u16endsec     \b IN: End Sector index
/// @return unsinged long value: sector size
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PARFLASH_GetSectorSize(MS_U16 u16startsec, MS_U16 u16endsec)
{
    MS_U32 u32size = 0;
    MS_U16 u16ii;

    for(u16ii = u16startsec; u16ii <= u16endsec; u16ii++)
        u32size += _Drv_PARFLASH_GetSectorSize(u16ii);

    if(!_ParFlashInfo.bbytemode)
        u32size <<= 1;

    return u32size;
}

