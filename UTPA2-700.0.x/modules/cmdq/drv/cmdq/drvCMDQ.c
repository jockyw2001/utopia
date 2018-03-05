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
////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// file    drvCMDQ.c
// @brief  CMDQ Driver
// @author MStar Semiconductor,Inc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvMMIO.h"
#include "halCMDQ.h"
#include "drvCMDQ.h"
#include "regCMDQ.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvCMDQ_priv.h"

#include "halMPool.h"
#include "halCHIP.h"

#include "ULog.h"
//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------
// To replace following shared memory pointer.
// cmdq_init, cmdq_miu_select, cmdq_buffer_write_pointer, cmdq_buffer_start, cmdq_buffer_end
typedef struct {
    MS_U32 init;
    MS_U32 miu_select;
    MS_PHY buffer_wr;
    MS_PHY buffer_start;
    MS_PHY buffer_end;
} ShmCtx;

/// struct to storing statistical data for profiling and debugging.
typedef struct {
    //to replace count_command_set, record how many commands.
    MS_U32 cmd_count;

    //user can set a bunch of commands one time, we record this.
    MS_U32 cmd_setcount;

    //counting NULL command that added for padding by drvier. (not by user)
    MS_U32 cmd_nullcnt;

} StatsCtx;

typedef struct {
    //
    //MS_U32  count_command_set;
    StatsCtx stats;
    // only agate will use DIRECT_MODE, but agate will no longer use
    MS_U32  CMDQ_MODE;
    //
    MS_SIZE TOTAL_BUFFER_SIZE;
    //
    MS_U32  CMDQ_BUFFER_MIU;
    // pa, the Buffer_Start Address, no need to be protected, fixed
    MS_PHY  buffer_start;
    // pa, the Buffer_End Address, no need to be protected, fixed
    MS_PHY  buffer_end;
    // pa, the Write_Pointer for CMDQ Buffer, pointing to the CMDQ Buffer start_addr of
    // next writing command, need to protected among processes
    MS_PHY  buffer_wr;
    // va for buffer_wr, no need to be protected, varied with buffer_wr
    MS_U8*  buffer_wrptr;

    MS_S32  mutex;
    char    *mutex_name; //this name is for MsOS_CreateMutex

    ShmCtx  *shm;
    char    *shm_name;   //this id used for MsOS_SHM_GetId
} Context;
#if (NUMBER_OF_CMDQ_HW == 1)
static Context _ctx[NUMBER_OF_CMDQ_HW] = {
    { {0,0,0}, RING_BUFFER_MODE, 0, 0, 0, 0, 0, NULL, -1, "cmdq0.mutex", NULL, "cmdq0.shm" }
};
#elif (NUMBER_OF_CMDQ_HW == 2)
static Context _ctx[NUMBER_OF_CMDQ_HW] = {
    { {0,0,0}, RING_BUFFER_MODE, 0, 0, 0, 0, 0, NULL, -1, "cmdq0.mutex", NULL, "cmdq0.shm" },
    { {0,0,0}, RING_BUFFER_MODE, 0, 0, 0, 0, 0, NULL, -1, "cmdq1.mutex", NULL, "cmdq1.shm" }
};
#else
#endif
static MS_U32 IS_STR_RESUME = 0;  // for STR, if suspend to resume, we will set this to be 1, and the init_calls will be done again
static DrvCMDQ_DbgLevel _u32CMDQDBGLevel = 0;
static MSIF_Version _drv_comq_version = {
    .DDI = { CMDQ_DRV_VERSION, },
};

//define mutex(FIX: mutex should remove if utopia2.0)
#define CMDQ_MUTEX_CREATE(hnd)          _ctx[hnd].mutex = MsOS_CreateMutex(E_MSOS_FIFO, _ctx[hnd].mutex_name ,MSOS_PROCESS_SHARED)
#define CMDQ_MUTEX_LOCK(hnd)            MsOS_ObtainMutex(_ctx[hnd].mutex, MSOS_WAIT_FOREVER)
#define CMDQ_MUTEX_UNLOCK(hnd)          MsOS_ReleaseMutex(_ctx[hnd].mutex)
#define CMDQ_MUTEX_DELETE(hnd)          MsOS_DeleteMutex(_ctx[hnd].mutex)

//physical address to virtual address
#define MS_CMDQ2PA(_addr_)           (MS_PHY)MsOS_VA2PA((_addr_))
#define MS_PA2KSEG0(_addr_)          (MS_VIRT)MsOS_PA2KSEG0((_addr_))
#define MS_PA2KSEG1(_addr_)          (MS_VIRT)MsOS_PA2KSEG1((_addr_))

#define CMDQ_FPGA_ASIC_VERIFY        0UL
#define MIU_NOT_SUPPORT              2
//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define TAG_DRV_CMDQ "DRV_CMDQ"
#define DEBUG_CMDQ(debug_level, x)     do { if (_u32CMDQDBGLevel >= (debug_level)) (x); } while(0)

#if (defined ANDROID) && (defined TV_OS)
#include <cutils/log.h>
#define printf LOGD
#ifndef LOGD // android 4.1 rename LOGx to ALOGx
#define LOGD ALOGD
#endif

#if 1
#define _msg(fmt, ...)
#define _dbg(fmt, ...)
#define _err(fmt, ...)
#define _cri(fmt, ...)
#else
#define _msg(fmt, ...) do { if(_u32CMDQDBGLevel) LOGD(fmt, ##__VA_ARGS__); } while(0)
#define _dbg LOGD
#define _err LOGD
#define _cri LOGD
#endif

#elif defined (MSOS_TYPE_OPTEE)
#define _msg(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define _dbg(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define _err(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define _cri(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
static void _log(int lv, const char *fn, int ln, char *fmt, ...) {
    va_list ap;
    char *tag[] = { "NA", "INFO", "DBG", "ERR", "WTF"};
    int i;

    if(_u32CMDQDBGLevel==0) return;

    if(_u32CMDQDBGLevel > lv) return;

    i = (lv==4)?3:1;
    va_start(ap, fmt);

    do {
        ULOGE(TAG_DRV_CMDQ, "[cmdq][%s][%s:%d] ", tag[lv], fn, ln);
        #ifdef MSOS_TYPE_LINUX_KERNEL
        vprintk(fmt, ap);
        #else
        vprintf(fmt, ap);
        #endif
    } while(--i);

    va_end(ap);
}

#define _msg(fmt, ...) //_log(1, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define _dbg(fmt, ...) _log(2, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define _err(fmt, ...) _log(3, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define _cri(fmt, ...) _log(4, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#endif

//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//  Backward compatibile API
//--------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Init(MS_U32 CMDQ_MIU_SELECT) {
    return MDrv_CMDQv2_Init(0, CMDQ_MIU_SELECT);
}
DRVCMDQ_RESULT MDrv_CMDQ_Start(MS_BOOL bStart) {
    return MDrv_CMDQv2_Start(0, bStart);
}
DRVCMDQ_RESULT MDrv_CMDQ_Get_Memory_Size(MS_PHY SmallAddr, MS_PHY BigAddr, MS_U32 miu_select) {
    return MDrv_CMDQv2_Get_Memory_Size(0, SmallAddr, BigAddr, miu_select);
}
DRVCMDQ_RESULT MDrv_CMDQ_Set_Buffer(MS_PHY StartAddr, MS_PHY EndAddr) {
    return MDrv_CMDQv2_Set_Buffer(0, StartAddr, EndAddr);
}
DRVCMDQ_RESULT MDrv_CMDQ_Reset(void) { return MDrv_CMDQv2_Reset(0); }
DRVCMDQ_RESULT MDrv_CMDQ_Exit(void)  { return MDrv_CMDQv2_Exit(0);  }
DRVCMDQ_RESULT MDrv_CMDQ_Stop(void)  { return MDrv_CMDQv2_Stop(0);  }
DRVCMDQ_RESULT MDrv_CMDQ_Receive(CH_Struct_Pointer New_IP_Head) {
    return MDrv_CMDQv2_Receive(0, New_IP_Head);
}
void Insert_Redundant_Null(MS_U32 Need_Null_Number) {
    MDrv_CMDQv2_Insert_Redundant_Null(0, Need_Null_Number);
}
void MDrv_CMDQ_Transfer(CAF_Struct_Pointer Array_Command, MS_SIZE Number_Array_Command) {
    MDrv_CMDQv2_Transfer(0, Array_Command, Number_Array_Command);
}
void MDrv_CMDQ_Set_dummy_Address(MS_U32 DummyValue) {
    MDrv_CMDQv2_Set_dummy_Address(0, DummyValue);
}
void MDrv_CMDQ_Set_MIU_SELECT(MS_U32 miu_select) {
    MDrv_CMDQv2_Set_MIU_SELECT(0, miu_select);
}
DRVCMDQ_RESULT MDrv_CMDQ_Set_timer_ratio(MS_U32 time,MS_U32 ratio) {
    return MDrv_CMDQv2_Set_timer_ratio(0, time, ratio);
}
DRVCMDQ_RESULT MDrv_CMDQ_Printf_Crash_Command(void) {
    return MDrv_CMDQv2_Printf_Crash_Command(0);
}
void MDrv_CMDQ_Gen_WaitTrigger_Bus_Command(CAF_Struct_Pointer cmd, EN_CMDQTriggerBus_ID id, MS_BOOL falledge) {
    MDrv_CMDQv2_Gen_WaitTrigger_Bus_Command(0, cmd, id, falledge);
}
MS_U32 MDrv_CMDQ_GetWritePoint(void) {
    return MDrv_CMDQv2_GetWritePoint(0);
}
MS_U32 MDrv_CMDQ_SetPowerState(EN_POWER_MODE u16PowerState) {
    return MDrv_CMDQv2_SetPowerState(0, u16PowerState);
}
MS_U32 MDrv_CMDQ_GetConfig(MS_PHY *SmallAddr, MS_PHY *BigAddr, MS_U32 *miu_select){
    return MDrv_CMDQv2_GetConfig(0,SmallAddr,BigAddr,miu_select);	
}
//--------------------------------------------------------------------------------------------------
//  Internal utility function
//--------------------------------------------------------------------------------------------------
static inline DRVCMDQ_RESULT IsHandleValid(int hnd) {
    if(hnd >= NUMBER_OF_CMDQ_HW || hnd < 0) {
        _err("Invalid Handle %d.\n", hnd);
        return DRVCMDQ_FAIL;
    }
    return DRVCMDQ_OK;
}

static inline void InsertOneCommand(int hnd, MS_U32 a, MS_U32 b, MS_U32 c, MS_U32 d, MS_U32 e, MS_U32 f, MS_U32 g, MS_U32 h)
{
    _msg(" cmdq(%d) cmd=0x%02X -> |L| %02x %02x %02x %02x %02x %02x %02x %02x |H|\n",
         hnd, ((h&0xF0)>>4), a,b,c,d,e,f,g,h);

    _ctx[hnd].buffer_wrptr[0] = a;
    _ctx[hnd].buffer_wrptr[1] = b;
    _ctx[hnd].buffer_wrptr[2] = c;
    _ctx[hnd].buffer_wrptr[3] = d;
    _ctx[hnd].buffer_wrptr[4] = e;
    _ctx[hnd].buffer_wrptr[5] = f;
    _ctx[hnd].buffer_wrptr[6] = g;
    _ctx[hnd].buffer_wrptr[7] = h;
    _ctx[hnd].buffer_wrptr = _ctx[hnd].buffer_wrptr + 8;
    _ctx[hnd].stats.cmd_count++;
}

static DRVCMDQ_RESULT IssueCommand(int hnd, CAF_Struct_Pointer cmd) {
    MS_U32 DDR_1 = 0, DDR_2 = 0, DDR_3 = 0, DDR_4 = 0, DDR_5 = 0, DDR_6 = 0, DDR_7 = 0, DDR_8 = 0;
    MS_U32 u1 = 0, u2 = 0, u3 = 0;
    MS_U32 mask_of_BF = 0x00ffffff;
    MS_U32 bit_4 = 0xff;

    if(cmd == NULL) {
        _err("illegal command pointer (NULL)\n");
        return DRVCMDQ_INVALID_PARAM;
    }

    _msg("Command: op: 0x%02x (%c) dest_addr: 0x%08x, dest_value: %04x, mask: %04x\n",
         cmd->operation, cmd->operation, cmd->destionation_address, cmd->destionation_value, cmd->mask);

    switch(cmd->operation) {
    case 'w':
        // 0x77 is for write, using dat as input, usually used in FPGA/ASIC verification,
        // the target_addr will be like 0xBF246BE0, write to "bank 0x123500, 8_bit F0"

        //RIU_addr << 1 | 0xBF000000 => destination_address.
        u1 = (cmd->destionation_address & mask_of_BF) >> 2;
        u2 = cmd->destionation_value;
        u3 = cmd->mask;

        DDR_1 = 0x10;
        DDR_2 = u1 >> 16;
        DDR_3 = (u1 >> 8 & (bit_4));
        DDR_4 = (u1) & (bit_4);
        DDR_5 = u2 >> 8;         // write value, high bits
        DDR_6 = u2 & (bit_4);    // write value, low  bits
        DDR_7 = u3 >> 8;         // mask, high bits
        DDR_8 = u3 & (bit_4);    // mask, low  bits
        InsertOneCommand(hnd, DDR_8, DDR_7, DDR_6, DDR_5, DDR_4, DDR_3, DDR_2, DDR_1);
        break;

    case 'W':
        // 0x57 is for write, using cmdq_api to gen command, usually used in other drver code,
        // the target_addr will be like 0x001235F0, write to "bank 0x123500, 8_bit F0"
        u1 = cmd->destionation_address >> 1;
        u2 = cmd->destionation_value;
        u3 = cmd->mask;
        DDR_1 = 0x10;                //command type
        DDR_2 = u1 >> 16;            //target RIU
        DDR_3 = (u1 >> 8 & (bit_4)); //target RIU
        DDR_4 = (u1) & (bit_4);      //target RIU
        DDR_5 = u2 >> 8;             //write value
        DDR_6 = u2 & (bit_4);        //write value
        DDR_7 = u3 >> 8;             //write mask
        DDR_8 = u3 & (bit_4);        //write mask
        InsertOneCommand(hnd, DDR_8, DDR_7, DDR_6, DDR_5, DDR_4, DDR_3, DDR_2, DDR_1);
        break;

    case 'p':
        // 0x70 is for polling_eq
        _msg("\033[35mthis is polling equal command, which will finished with software interrupt: 0x1\033[m\n"); // joe.liu

        // RIU_addr << 1 | 0xBF000000 => destionation_address
        u1 = (cmd->destionation_address & mask_of_BF) >> 1;
        u2 = cmd->destionation_value;
        u3 = cmd->mask;
        DDR_1 = 0x31;                // add software interrupt: 1
        DDR_2 = u1>>16;
        DDR_3 = (u1 >> 8 & (bit_4));
        DDR_4 = (u1) & (bit_4);
        DDR_5 = u2 >> 8;
        DDR_6 = u2 & (bit_4);
        DDR_7 = u3 >> 8;
        DDR_8 = u3 & (bit_4);
        InsertOneCommand(hnd, DDR_8, DDR_7, DDR_6, DDR_5, DDR_4, DDR_3, DDR_2, DDR_1);
        break;

    case 'C':
        // 0x43 is for polling_neq
        _dbg("\033[35mthis is polling non-equal command, which will finished with software interrupt: 0x2\033[m\n"); // joe.liu
        // RIU_addr << 1 | 0xBF000000 => destionation_address
        u1 = (cmd->destionation_address & mask_of_BF) >> 1;
        u2 = cmd->destionation_value;
        u3 = cmd->mask;
        DDR_1 = 0xb2;                // add software interrupt: 2
        DDR_2 = u1>>16;
        DDR_3 = (u1 >> 8 & (bit_4));
        DDR_4 = (u1) & (bit_4);
        DDR_5 = u2 >> 8;
        DDR_6 = u2 & (bit_4);
        DDR_7 = u3 >> 8;
        DDR_8 = u3 & (bit_4);
        InsertOneCommand(hnd, DDR_8, DDR_7, DDR_6, DDR_5, DDR_4, DDR_3, DDR_2, DDR_1);
        break;

    case 'F':
        // 0x46 is for wait command for trigger_bus
        _dbg("\033[35mthis is wait trigger bus signal command, which will finished with software interrupt: 0x4\033[m\n"); // joe.liu
        DDR_1 = 0x24;    // add software interrupt: 4
        DDR_2 = 0x00;
        DDR_3 = 0x00;
        DDR_4 = (cmd->destionation_address==0)?0x00:0x01;  // 0: detect rising, 1: detect falling
        DDR_5 = 0x00;
        DDR_6 = 0x00;
        DDR_7 = cmd->mask >> 8;
        DDR_8 = cmd->mask & (bit_4);
        InsertOneCommand(hnd, DDR_8, DDR_7, DDR_6, DDR_5, DDR_4, DDR_3, DDR_2, DDR_1);
        break;

    case 'N': //null command.
        _msg("add null command\n");
        InsertOneCommand(hnd, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0);
        break;

    default:
        _err("Invalid command operation \"0x%0X\" issued.\n", cmd->operation);
        return DRVCMDQ_INVALID_PARAM;
        break;
    }
    return DRVCMDQ_OK;
}

//---------------------------------------------------------------------------------------------------------------------
///Get MMIO_Base and set struct _REG_CMDQCtrl * _CMDQCtrl
///Remember the caller need to do "MsOS_MPool_Mapping()" and "MDrv_CMDQ_Get_Memory_Size()" for setting the buffer!!
//---------------------------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQv2_Init(int hnd, MS_U32 CMDQ_MIU_SELECT)
{
    MS_VIRT u32Bank = 0x0;
    MS_VIRT u32shmAddr;
    MS_PHY u32BankSize = 0x0;
    MS_U32 u32Ret, u32ShmId, u32shmBufSize;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    if(CMDQ_MIU_SELECT >= MIU_NOT_SUPPORT)
    {
        _err("\033[35mCMDQ_MIU_SELECT is %d, we only support MIU0 or MIU1\033[m\n", CMDQ_MIU_SELECT);
        return DRVCMDQ_FAIL;
    }

    if(FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_CMDQ))
    {
        _err("\033[35mu32Bank is 0x%X and u32BankSize is 0x%X\033[m\n", (unsigned int)u32Bank, (unsigned int)u32BankSize);
        return DRVCMDQ_FAIL;
    }

    CMDQ_MUTEX_CREATE(hnd);
    CMDQ_MUTEX_LOCK(hnd);
    HAL_CMDQ_SetBank(hnd, u32Bank); // not sure if this HAL_CMDQ_SetBank() need to be done for every process if STR

    /* get a str_flag */
    u32Ret = MsOS_SHM_GetId((MS_U8*)"cmdq_str_flag", sizeof(MS_U32), &u32ShmId, &u32shmAddr, &u32shmBufSize, MSOS_SHM_QUERY);
    if(u32Ret == TRUE)
    {
        IS_STR_RESUME = *(MS_U32*)u32shmAddr;
    }

    //below, if we check shm and return ok, but we still keep miu_select info in our context.
    _ctx[hnd].CMDQ_BUFFER_MIU = CMDQ_MIU_SELECT;

    /* use SHM to check if inited, if one process already inited it, other processes do not need to init */
    u32Ret = MsOS_SHM_GetId((MS_U8*)_ctx[hnd].shm_name, sizeof(ShmCtx), &u32ShmId, (MS_VIRT*)&_ctx[hnd].shm, &u32shmBufSize, MSOS_SHM_QUERY);
    if( (u32Ret == TRUE) && (IS_STR_RESUME == 0) )
    {
        _dbg("\033[35mCMDQ already inited, and IS_STR_RESUME is %d\033[m\n", IS_STR_RESUME);
        CMDQ_MUTEX_UNLOCK(hnd);
        return DRVCMDQ_OK;
    }

    /* following is default setiing, we need to modify the "mapping region" and "start/end region" according to CMDQ_BUFFER_MIU
     * if you set cmdq buffer at MIU_1, you must set the "cmdq miu_sel bit" to be 1, we only support MIU_0 or MIU_1
     * the MDrv_CMDQ_Get_Memory_Size() will set the "start/end address of cmdq buffer",
     * for simplifying, we all use phy_addr instead of miu_offset to specify "start/end address of cmdq buffer" in RIU
     * however, the para passed into MDrv_CMDQ_Get_Memory_Size() should be miu_offset, we will convert it to phy_addr in MDrv_CMDQ_Get_Memory_Size()
     * IMPORTANT: CMDQ does not support MIU2/3!!
     */
    if(_ctx[hnd].CMDQ_BUFFER_MIU)
    {
        // set select_MIU to MIU_1
        MDrv_CMDQv2_Set_MIU_SELECT(hnd, _ctx[hnd].CMDQ_BUFFER_MIU);
    }
    else
    {
        // set select_MIU to MIU_0
        MDrv_CMDQv2_Set_MIU_SELECT(hnd, _ctx[hnd].CMDQ_BUFFER_MIU);
    }

    HAL_CMDQ_Enable(hnd);
    HAL_CMDQ_Reset(hnd);

    if(FALSE == HAL_CMDQ_Set_Mode(hnd, _ctx[hnd].CMDQ_MODE))
    {
        _err("\033[35mCMDQ Set Mode FAIL!!\033[m\n");
        CMDQ_MUTEX_UNLOCK(hnd);
        return DRVCMDQ_FAIL;
    }

    /* the polling_timer is for re-checking polling value, if the time_interval is too small, cmdq will polling RIU frequently, so that RIU will very busy */
    MDrv_CMDQv2_Set_timer_ratio(hnd, 0x000000FF, 0x0000000F);        // set time / ratio, total wait time is (wait_time * (ratio + 1)) => ( FF * (F + 1) / 216MHz ) = sec

    // create a shm for cmdq str init
    if(FALSE == MsOS_SHM_GetId((MS_U8*)"cmdq_str_flag", sizeof(MS_U32), &u32ShmId, &u32shmAddr, &u32shmBufSize, MSOS_SHM_CREATE))
    {
        _cri("[cmdq get shm error] something wrong in MsOS_SHM_GetId\n");
        _cri("is SHM_SIZE reached?\n");
        _cri("is MAX_SHM_CLIENT_NUM reached?\n");
        CMDQ_MUTEX_UNLOCK(hnd);
        return DRVCMDQ_FAIL;
    }

    if(FALSE == MsOS_SHM_GetId((MS_U8*)_ctx[hnd].shm_name, sizeof(ShmCtx), &u32ShmId, (MS_VIRT*)&_ctx[hnd].shm, &u32shmBufSize, MSOS_SHM_CREATE))
    {
        _cri(" FAIL TO CREATE SHM(%s). with size %d.\n", _ctx[hnd].shm_name, sizeof(ShmCtx));
        _cri("is SHM_SIZE reached?\n");
        _cri("is MAX_SHM_CLIENT_NUM reached?\n");
        CMDQ_MUTEX_UNLOCK(hnd);
        return DRVCMDQ_FAIL;
    }

	if(_ctx[hnd].shm->init == 0)	/* only first init will assign shm data, and not form STR_RESUME */
	{
		_ctx[hnd].shm->init = 1;
    	_ctx[hnd].shm->miu_select = _ctx[hnd].CMDQ_BUFFER_MIU;
    	_ctx[hnd].shm->buffer_wr = 0; //following code flow check 0 to know its not setup.
	}

	_msg("set _ctx[%d].shm->buffer_wr to 0x%X\n", hnd, _ctx[hnd].shm->buffer_wr);
	_dbg("set _ctx[%d].shm->buffer_wr to 0x%X\n", hnd, _ctx[hnd].shm->buffer_wr);
	_err("set _ctx[%d].shm->buffer_wr to 0x%X\n", hnd, _ctx[hnd].shm->buffer_wr);
	_cri("set _ctx[%d].shm->buffer_wr to 0x%X\n", hnd, _ctx[hnd].shm->buffer_wr);

	_msg("IS_STR_RESUME is %d\n", IS_STR_RESUME);
	_dbg("IS_STR_RESUME is %d\n", IS_STR_RESUME);
	_err("IS_STR_RESUME is %d\n", IS_STR_RESUME);
	_cri("IS_STR_RESUME is %d\n", IS_STR_RESUME);

    CMDQ_MUTEX_UNLOCK(hnd);
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return DRVCMDQ_FAIL;
    }

    *ppVersion = &_drv_comq_version;
    return DRVCMDQ_OK;
}

//--------------------------------------------------------------------------------------------------
// In ring-buffer mode, this function will trig for update reg_sw_wr_mi_wadr
// The CMDQ will keep on executing cmd until reg_rd_mi_radr reach reg_sw_wr_mi_wadr
//--------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQv2_Start(int hnd, MS_BOOL bStart)
{
    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_INVALID_PARAM;

    if(bStart == 1)
    {
        _msg("trigger cmdq to start!\n");
        HAL_CMDQ_Start(hnd);
        return DRVCMDQ_OK;
    }
    else
    {
        _msg("bStart %d \n",bStart);
        return DRVCMDQ_FAIL;
    }
}

//--------------------------------------------------------------------------------------------------
// Set the Start and End address of CMDQ DRAM(use miu_offset and miu_select to get phy_addr)
// @ \b in: SmallAddr => Start address     (miu_offset)
// @ \b in: BigAddr   => End address    (miu_offset)
//--------------------------------------------------------------------------------------------------
// this function should be called for setting CMDQ Buffer(System Side), need to add in ioctl for utopia2
DRVCMDQ_RESULT MDrv_CMDQv2_Get_Memory_Size(int hnd, MS_PHY SmallAddr, MS_PHY BigAddr, MS_U32 miu_select)
{
    MS_PHY pa_start = 0;
    MS_VIRT u32shmAddr;
    MS_VIRT va_SmallAddr = 0, va_BigAddr = 0;
    MS_U32 u32Ret, u32ShmId, u32shmBufSize;
    MS_PHY CMDQ_BUFFER_START = SmallAddr;
    MS_PHY CMDQ_BUFFER_END = BigAddr;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    if(miu_select >= MIU_NOT_SUPPORT)
    {
        _err("\033[35mmiu_select is %d, we only support MIU0 or MIU1\033[m\n", miu_select);
        return DRVCMDQ_FAIL;
    }

        BigAddr -= 16;    // cause the ori BigAddr is other IP buffer head, we need to decrease one bus line

    _miu_offset_to_phy(miu_select,0,pa_start);
    /* get the start/end va_addr, remember the caller should do mmaping first */
    va_SmallAddr    = MS_PA2KSEG1(SmallAddr + pa_start);
    va_BigAddr      = MS_PA2KSEG1(BigAddr + pa_start);

    if( (va_SmallAddr == 0x0UL) || (va_BigAddr == 0x0UL) )
    {
        _cri("is the region from SmallAddr to BigAddr mmaped?\n");
        return DRVCMDQ_FAIL;
    }

    /* caller can pass miu_offset directly, and we will help to change to cmdq_used(only see command line, each line having 16-byte, two commands) */
    SmallAddr    = SmallAddr >> BUS_SHIFT;
    BigAddr      = BigAddr   >> BUS_SHIFT;
    pa_start     = pa_start  >> BUS_SHIFT;

    CMDQ_MUTEX_LOCK(hnd);

    /* get a str_flag */
    u32Ret = MsOS_SHM_GetId((MS_U8*)"cmdq_str_flag", sizeof(MS_U32), &u32ShmId, &u32shmAddr, &u32shmBufSize, MSOS_SHM_QUERY);
    if(u32Ret == TRUE)
    {
        _msg("succeed to get STR flag from shared memory.\n");
        IS_STR_RESUME = *(MS_U32*)u32shmAddr;
    }
	_msg("IS_STR_RESUME is %d\n", IS_STR_RESUME);
	_dbg("IS_STR_RESUME is %d\n", IS_STR_RESUME);
	_err("IS_STR_RESUME is %d\n", IS_STR_RESUME);
	_cri("IS_STR_RESUME is %d\n", IS_STR_RESUME);

    /* use SHM to check if inited, if one process already inited it, other processes do not need to init */
    u32Ret = MsOS_SHM_GetId((MS_U8*)_ctx[hnd].shm_name, sizeof(ShmCtx), &u32ShmId, (MS_VIRT*)&_ctx[hnd].shm, &u32shmBufSize, MSOS_SHM_QUERY);
    if( u32Ret == FALSE )
    {
        _cri("[cmdq get shm error] something wrong in MsOS_SHM_GetId\n");
        _cri("is SHM_SIZE reached?\n");
        _cri("is MAX_SHM_CLIENT_NUM reached?\n");
        _cri("its very weird die here. after CMDQ_Init(), shm should be query-able!!\n");
        return DRVCMDQ_FAIL;
    }
    _msg("succeed to get shared memory of handle %d\n", hnd);

    if( (IS_STR_RESUME == 0) && (_ctx[hnd].shm->buffer_wr != 0) )
    {
        _dbg("\033[35mget Memory_Start_Address from shm\033[m\n");
        // get from SHM, to prevent multi-process using different wr_adr
        _ctx[hnd].buffer_wr      = _ctx[hnd].shm->buffer_wr;
        _ctx[hnd].buffer_start   = SmallAddr + pa_start; // fix to be buffer_start
        _ctx[hnd].buffer_end     = BigAddr   + pa_start; // fix to be buffer_end
    }
    else
    {
        if(IS_STR_RESUME == 1)
        {
            _ctx[hnd].buffer_wr  = _ctx[hnd].shm->buffer_wr;
            _msg("still _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
            _dbg("still _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
            _err("still _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
            _cri("still _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
        }
        else //first time to setup buffer_wr.
        {
            _ctx[hnd].buffer_wr      = SmallAddr + pa_start;
            _ctx[hnd].shm->buffer_wr = _ctx[hnd].buffer_wr;
            _msg("[1] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
            _dbg("[1] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
            _err("[1] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
            _cri("[1] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
        }

        _ctx[hnd].buffer_start   = SmallAddr + pa_start; // fix to be buffer_start
        _ctx[hnd].buffer_end     = BigAddr   + pa_start; // fix to be buffer_end
        MDrv_CMDQv2_Set_Buffer(hnd, _ctx[hnd].buffer_start, _ctx[hnd].buffer_end);
    }
    // Number of Command Line
    _ctx[hnd].TOTAL_BUFFER_SIZE = ( (va_BigAddr >> 4) - (va_SmallAddr >> 4) + 1 );

    /* save a str_flag(reset to 0) */
    *(MS_U32*)u32shmAddr = 0;
    _ctx[hnd].shm->miu_select = miu_select;
    _ctx[hnd].shm->buffer_start = CMDQ_BUFFER_START;
    _ctx[hnd].shm->buffer_end   = CMDQ_BUFFER_END;

    CMDQ_MUTEX_UNLOCK(hnd);
    _msg("DONE.\n");
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQv2_Set_Buffer(int hnd, MS_PHY StartAddr, MS_PHY EndAddr)
{
    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    HAL_CMDQ_Set_Start_Pointer(hnd, StartAddr);
    HAL_CMDQ_Set_End_Pointer(hnd, EndAddr);
    _msg("set cmd%d buffer from %llu to %llu\n", hnd, StartAddr, EndAddr);
    return DRVCMDQ_OK;
}

// currently not used
DRVCMDQ_RESULT MDrv_CMDQv2_Reset(int hnd)
{
    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    CMDQ_MUTEX_LOCK(hnd);
    HAL_CMDQ_Reset(hnd);
    CMDQ_MUTEX_UNLOCK(hnd);
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQv2_Exit(int hnd)
{
    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    CMDQ_MUTEX_LOCK(hnd);
    MDrv_CMDQv2_Set_dummy_Address(hnd, 0x5566);
    HAL_CMDQ_Stop(hnd);
    CMDQ_MUTEX_UNLOCK(hnd);
    CMDQ_MUTEX_DELETE(hnd);
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQv2_Stop(int hnd)
{
    MS_U32 size;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    CMDQ_MUTEX_LOCK(hnd);
    //reset
    _msg("SW RST pull low\n");
    HAL_CMDQ_Reset2(hnd, 0); //low

    //clear buffer
    _msg("clear buffer\n");
    size = _ctx[hnd].TOTAL_BUFFER_SIZE << 4;
    memset((void*)MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_start<<4), 0, size);
    MsOS_Dcache_Flush(MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_start << 4), size-1);
    MsOS_FlushMemory();

    _msg("reset wr pointer\n");
    //update write pointer.
    _ctx[hnd].buffer_wr = _ctx[hnd].shm->buffer_wr = _ctx[hnd].buffer_start;

    _msg("[2] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
    _dbg("[2] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
    _err("[2] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
    _cri("[2] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
    HAL_CMDQ_Write_Pointer(hnd, _ctx[hnd].buffer_wr);

    _msg("SW RST pull high\n");
    HAL_CMDQ_Reset2(hnd, 1); //high

    _dbg("We DO stop and flush buffer when Stop() called. now\n");
    MDrv_CMDQv2_Set_dummy_Address(hnd, 0x7878);

    CMDQ_MUTEX_UNLOCK(hnd);
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_SetDbgLevel(DrvCMDQ_DbgLevel DbgLevel)
{
    _u32CMDQDBGLevel = 1; //make following _dbg() always show.
    _dbg("setup debug level => %lu\n", DbgLevel);
    _u32CMDQDBGLevel = DbgLevel;
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQv2_Receive(int hnd, CH_Struct_Pointer New_IP_Head)
{
    MS_SIZE New_CommandArray_Number = 0;        // Command_Line Number in DRAM
    MS_SIZE Transfer_Number = 0;                // Command Number
    MS_U32  avail;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    CMDQ_MUTEX_LOCK(hnd);
    CAF_Struct_Pointer New_CommandArray_Pointer = New_IP_Head->Pointer_To_CAFArray;
    Transfer_Number = New_IP_Head->Command_Number;
    New_CommandArray_Number = New_IP_Head->Command_Number;

    if(New_CommandArray_Number & 0x0000001UL)  // new receive struct odd or even
    {
        New_CommandArray_Number = (New_CommandArray_Number+1) / 2;
    }
    else
    {
        New_CommandArray_Number = New_CommandArray_Number/2;
    }

    if(New_CommandArray_Number > _ctx[hnd].TOTAL_BUFFER_SIZE-1)
    {
        _err("request command size %lu is larger than total command size %lu\n",
             New_CommandArray_Number, _ctx[hnd].TOTAL_BUFFER_SIZE-1);
        CMDQ_MUTEX_UNLOCK(hnd);
        return DRVCMDQ_FAIL;
    }

    // get from SHM, to prevent multi-process using different wr_adr
    _ctx[hnd].buffer_wr = _ctx[hnd].shm->buffer_wr;

    avail = MDrv_CMDQv2_Get_FreeCmdLine_Size(hnd);
    if(avail < New_CommandArray_Number)
    {
        _err("not enough free space for %d command(128bit). available = %d\n",
             New_CommandArray_Number, avail);
        CMDQ_MUTEX_UNLOCK(hnd);
        return DRVCMDQ_FAIL;
    }

    _msg("Memory_Start_Address %llu\n", (unsigned long long)_ctx[hnd].buffer_wr);
    _msg("New_CommandArray_Number %llu\n", (unsigned long long)New_CommandArray_Number);

    //to get a VA by using PA, Remember to use MsOS_Mapping to get CMDQ_Buffer Mapping
    _ctx[hnd].buffer_wrptr = (MS_U8*)MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_wr << 4);
    _msg("buffer_wrptr start from 0x%llx\n", (unsigned long)_ctx[hnd].buffer_wrptr);

    // step_1: Write CMD to CMDQ Buffer
    MDrv_CMDQv2_Transfer(hnd, New_CommandArray_Pointer, Transfer_Number);
    //we do flush in _Transfer(), because might be one or two segment of memory.

    // if buffer_wr == buffer_start
    //   step_2: Update software write position of command in miu
    //   (Need to take care of if using ring buffer, which will occurs when New_CommandArray_Number = TOTAL_BUFFER_SIZE)
    // else
    //   step_2: Update software write position of command in miu (Need to take care of if using ring buffer)

    // will do unsign_ext for New_CommandArray_Number first
    _ctx[hnd].buffer_wr += New_CommandArray_Number;

    if(_ctx[hnd].buffer_wr > _ctx[hnd].buffer_end)
    {
        // actually, in this case, New_CommandArray_Number will large/equal to TOTAL_BUFFER_SIZE,
        // this case should return DRVCMDQ_FAIL at upper check
        _msg("Adjust Memory_Start_Address FROM 0x%016llx\n",
             (unsigned long long)_ctx[hnd].buffer_wr);
        _ctx[hnd].buffer_wr =
            _ctx[hnd].buffer_wr -
            _ctx[hnd].buffer_end - 1 +
            _ctx[hnd].buffer_start;
        _msg("                            TO   0x%016llx\n",
             (unsigned long long)_ctx[hnd].buffer_wr);
    }

    // step_3: update buffer_wr to shm
    _ctx[hnd].shm->buffer_wr = (MS_VIRT)_ctx[hnd].buffer_wr;
	//_msg("[cmdq_fire] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
	//_dbg("[cmdq_fire] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
	//_err("[cmdq_fire] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
	//_cri("[cmdq_fire] update _ctx[%d].shm->buffer_wr to 0x%lX\n", hnd, _ctx[hnd].shm->buffer_wr);
    /*
     * update wr_adr to let cmdq know the stop_execution addr(wr_adr),
     * when read_addr = wr_adr, stop read and execute what have read
     * so, after trigger, the cmdq will execute until reach wr_adr
     */
    HAL_CMDQ_Write_Pointer(hnd, _ctx[hnd].buffer_wr);

    // step_4: trigger to start
    MDrv_CMDQv2_Start(hnd, 1);

    _ctx[hnd].stats.cmd_setcount++;
    CMDQ_MUTEX_UNLOCK(hnd);
    return DRVCMDQ_OK;
}

void MDrv_CMDQv2_Insert_Redundant_Null(int hnd, MS_U32 Need_Null_Number)
{
    MS_U32 j = 0;
    Need_Null_Number *= 2;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
    {
        _cri("Invalid handle plz. %d\n", hnd);
        MS_ASSERT(0);
    }

    for(j = 0; j < Need_Null_Number; j++)
    {
        //@FIXME, here, we might exceed end of buffer address.!!!
        _msg("Insert #%d Null_Commands\n", j);
        InsertOneCommand(hnd, 0,0,0,0,0,0,0,0);
    }
}

//--------------------------------------------------------------------------------------------------
// Write the CAF content to CMDQ Buffer
// @ \b in: Array_Command => Pointer to CAF
// @ \b in: Number_Array_Command => Number of Commands
//--------------------------------------------------------------------------------------------------
void MDrv_CMDQv2_Transfer(int hnd, CAF_Struct_Pointer Array_Command, MS_SIZE Number_Array_Command)
{
    MS_U32 i = 0;
    DRVCMDQ_RESULT ret;
    MS_U8 topad;
    MS_U8 twoflush = 0;
    MS_SIZE a,b;
    MS_U8 *endptr;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
    {
        _cri("Invalid handle plz. %d\n", hnd);
        MS_ASSERT(0);
    }

    topad = (Number_Array_Command & 0x1UL)?1:0;

    //@NOTE: we use VA to compare with wrptr, because VA2PA(wrptr)
    // that wrptr might exceed buffer address, which address is not mmio-ed.
    endptr = (MS_U8*)MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_end << 4);
    endptr += 16;

    for(i = 0; i < (Number_Array_Command + topad); i++)
    {
        /* RING BUFFER CONDITION
           |S|______R.........W______|E| when W exceed |E|, need to be handled.

           |S|......W_________R......|E| when W exceed R  , need to be handled.
           BUT, we handle this in Receive() func. it never happen here.
         */

        // This Command is over buffer_end, change to Buffer Head
        // However, the buffer_end can insert CMD
        if(_ctx[hnd].buffer_wrptr >= endptr)
        {
            _msg("@%d/%d cmds\n", i, Number_Array_Command);
            _msg("Restart Ring buffer: buffer_end is  0x%016llx\n",
                 endptr);
            _msg("          Current Write Address is  0x%016llx\n",
                 _ctx[hnd].buffer_wrptr);
            _msg("Change buffer_wrptr to Buffer Head: 0x%016llx\n",
                 (MS_U32)MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_start << 4));

            _ctx[hnd].buffer_wrptr = (MS_U8*)MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_start << 4);
            twoflush = 1;
        }

        if(topad && (i==Number_Array_Command)) {
            _ctx[hnd].stats.cmd_nullcnt++;
#if 1
            InsertOneCommand(hnd, 0xFF,0xFF,0,0,0,0,0,0);
#else
            // test if CMDQ works
            InsertOneCommand(hnd, 0x00, 0x00, 0x34, 0x12, 0xF8, 0x1A, 0x09, 0x10);
#endif
            _dbg("\033[35mOdd instructions\033[m\n");
        }
        else
        {
            _msg("%d issue %p\n", i, _ctx[hnd].buffer_wrptr);
            ret = IssueCommand(hnd, &Array_Command[i]);
            if(ret != DRVCMDQ_OK)
            {
                _err("\033[35mError, Unknown opertion!! Array_Command[%d].operation is %u\033[m\n", i, Array_Command[i].operation);
            }
        }
    }

    if(twoflush)
    {
        // |S|__b____R.........W___a___|E|
        a = (_ctx[hnd].buffer_end-_ctx[hnd].buffer_wr+1)<<4;
        //flush from wr to end
        //@FIXME: this is very weird implementation of Dcache_Flush.
        //that we need to -1, in case it internally do touch address of (ptr+size).
        //if the second parameter is size.....
        MsOS_Dcache_Flush(MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_wr<<4), a-1);
        MsOS_FlushMemory();
        _msg("Flush seg #a from 0x%016llx with %lu bytes\n", _ctx[hnd].buffer_wr<<4, a);
        //flush from start to data end.
        b = ((Number_Array_Command+(topad?1:0)) << 3) - a;
        if(b!=0)
        {
            MsOS_Dcache_Flush(MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_start<<4), b);
            MsOS_FlushMemory();
            _msg("Flush seg #b from 0x%016llx with %lu bytes\n", _ctx[hnd].buffer_start<<4, b);
        }
    }
    else
    {
        a = ((Number_Array_Command+(topad?1:0)) << 3);
        //@FIXME: this is very werid implementation of Dcache_Flush.
        //that we need to -1, in case wr+a is end of buffer,
        //that cause VA2PA fail internally in Flush() which misunderstanding msg show.
        MsOS_Dcache_Flush(MS_PA2KSEG1((MS_PHY)_ctx[hnd].buffer_wr<<4), a-1);
        MsOS_FlushMemory();  //flush OCP data
        _msg("Flush from 0x%016llx with %d bytes\n", (_ctx[hnd].buffer_wr<<4), a);
    }
}

void MDrv_CMDQv2_Set_dummy_Address(int hnd, MS_U32 DummyValue)
{
    if(IsHandleValid(hnd) != DRVCMDQ_OK)
    {
        _cri("Invalid handle plz. %d\n", hnd);
        MS_ASSERT(0);
    }

    HAL_CMDQ_Write_Dummy_Register(hnd, DummyValue);
}

void MDrv_CMDQv2_Set_MIU_SELECT(int hnd, MS_U32 miu_select)
{
    if(IsHandleValid(hnd) != DRVCMDQ_OK)
    {
        _cri("Invalid handle plz. %d\n", hnd);
        MS_ASSERT(0);
    }

    _dbg("CMDQ(%d) MIU set to %lu\n", hnd, miu_select);
    HAL_CMDQ_Set_MIU_SELECT(hnd, miu_select);
}

DRVCMDQ_RESULT MDrv_CMDQv2_Set_timer_ratio(int hnd, MS_U32 time, MS_U32 ratio)
{
    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    HAL_CMDQ_Set_Timer(hnd, time);
    HAL_CMDQ_Set_Ratio(hnd, ratio);
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQv2_Printf_Crash_Command(int hnd)
{
    MS_U32 Command_15_0_bit, Command_31_16_bit, Command_55_32_bit, Command_63_56_bit;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
        return DRVCMDQ_FAIL;

    /* see low 16-bit */
    Command_15_0_bit = HAL_CMDQ_Error_Command(hnd, CMDQ_CRASH_15_0_BIT);
    Command_31_16_bit = HAL_CMDQ_Error_Command(hnd, CMDQ_CRASH_31_16_BIT);
    Command_55_32_bit = HAL_CMDQ_Error_Command(hnd, CMDQ_CRASH_55_32_BIT);
    Command_63_56_bit = HAL_CMDQ_Error_Command(hnd, CMDQ_CRASH_63_56_BIT);

    _dbg("\033[35mCommand _bits: 0x%X, 0x%X, 0x%X, 0x%X\033[m\n", Command_63_56_bit, Command_55_32_bit, Command_31_16_bit, Command_15_0_bit);
    return DRVCMDQ_OK;
}

//--------------------------------------------------------------------------------------------------
// Gen A CAF to Wait A Trigger Bus Bit
// @ \b in: WaitTrigger_Bus_Command => Pointer to the CAF, we will insert WaitTrigger_Bus_Command to this struct
// @ \b in: u16Tigger_ID => The trigger bit needed to be waited
// @ \b in: bDetect_Falling_Mode => Check Rising/Falling Signal (0/1)
//--------------------------------------------------------------------------------------------------
void MDrv_CMDQv2_Gen_WaitTrigger_Bus_Command(int hnd, CAF_Struct_Pointer WaitTrigger_Bus_Command, EN_CMDQTriggerBus_ID u16Tigger_ID, MS_BOOL bDetect_Falling_Mode)
{
    MS_U16 u16Trgger_type;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
    {
        _cri("Invalid handle plz. %d\n", hnd);
        MS_ASSERT(0);
    }

    _dbg("\033[35mget a WaitTrigger_Bus_Command\033[m\n");

    switch(u16Tigger_ID)
    {
    case GE_GE2CMDQ_TRI:
        u16Trgger_type = HAL_GE_GE2CMDQ_TRI;
        break;
    case BDMA_INT_BDMA_0:
        u16Trgger_type = HAL_BDMA_INT_BDMA_0;
        break;
    case BDMA_INT_BDMA_1:
        u16Trgger_type = HAL_BDMA_INT_BDMA_1;
        break;
    case GPU_GPU2CMDQ_TRIG:
        u16Trgger_type = HAL_GPU_GPU2CMDQ_TRIG;
        break;
    case GOPG0_GOP_CMDQ_INT_0:
        u16Trgger_type = HAL_GOPG0_GOP_CMDQ_INT_0;
        break;
    case GOPG1_GOP_CMDQ_INT_1:
        u16Trgger_type = HAL_GOPG1_GOP_CMDQ_INT_1;
        break;
    case GOPG2_GOP_CMDQ_INT_2:
        u16Trgger_type = HAL_GOPG2_GOP_CMDQ_INT_2;
        break;
    case GOPG3_GOP_CMDQ_INT_3:
        u16Trgger_type = HAL_GOPG3_GOP_CMDQ_INT_3;
        break;
    case SCALAR_SC2CMDQ_TRIG_0:
        u16Trgger_type = HAL_SCALAR_SC2CMDQ_TRIG_0;
        break;
    case SCALAR_SC2CMDQ_TRIG_1:
        u16Trgger_type = HAL_SCALAR_SC2CMDQ_TRIG_1;
        break;
    case SCALAR_SC2CMDQ_TRIG_2:
        u16Trgger_type = HAL_SCALAR_SC2CMDQ_TRIG_2;
        break;
    case SCALAR_SC2CMDQ_TRIG_3:
        u16Trgger_type = HAL_SCALAR_SC2CMDQ_TRIG_3;
        break;
    case AESDMA_CMDQDMA_FINISHED:
        u16Trgger_type = HAL_AESDMA_CMDQDMA_FINISHED;
        break;
    case VOP_DCM2GOP_VDE:
        u16Trgger_type = HAL_VOP_DCM2GOP_VDE;
        break;
    case VOP_DCS2GOP_VDE:
        u16Trgger_type = HAL_VOP_DCS2GOP_VDE;
        break;
    case XC_DIP_CMDQ_INT:
        u16Trgger_type = HAL_XC_DIP_CMDQ_INT;
        break;
    case AESDMA_CMDQDMA_FINISHED_2:
        u16Trgger_type = HAL_AESDMA_CMDQDMA_FINISHED_2;
        break;
    case NONE_USED_SIGNAL: //AESDMA_CMDQDMA_FINISHED_2ND
        u16Trgger_type = 0xF;
        break;
    default:
        u16Trgger_type = 0xFFFF;
        break;
    }

    if(u16Trgger_type == 0xFFFF)
    {
        _err("\033[35mcan not gen WaitTrigger_Bus_Command\033[m\n");
        return;
    }

    WaitTrigger_Bus_Command->operation = 'F';    // this is wait command for trigger_bus

    /* the destionation_address here is used to be a decect mode setting, WaitTrigger_Bus_Command is not wait for an address!! */
    if(bDetect_Falling_Mode)
        WaitTrigger_Bus_Command->destionation_address = 1;
    else
        WaitTrigger_Bus_Command->destionation_address = 0;

    WaitTrigger_Bus_Command->mask = 0xFFFF & (~(1 << u16Trgger_type));
}

MS_U32 MDrv_CMDQv2_GetWritePoint(int hnd)
{
    /* this is not used, we will get Write_Pointer(buffer_wr) from shm */
    MS_U32 Write_Pointer = 0;

    if(IsHandleValid(hnd) != DRVCMDQ_OK)
    {
        _cri("Invalid handle %d\n", hnd);
        MS_ASSERT(0);
        return 0;
    }

    Write_Pointer = HAL_CMDQ_Get_Write_Pointer(hnd);

    _dbg("\033[35mGet Write_Pointer from RIU: 0x%X\033[m\n", (MS_U32)Write_Pointer);
    return Write_Pointer;
}

MS_U32 MDrv_CMDQv2_GetConfig(int hnd,MS_PHY *SmallAddr, MS_PHY *BigAddr, MS_U32 *miu_select)
{
    MS_U32 u32ShmId, u32shmBufSize;

    if (FALSE == MsOS_SHM_GetId((MS_U8*)_ctx[hnd].shm_name, sizeof(ShmCtx), &u32ShmId, (MS_VIRT*)&_ctx[hnd].shm, &u32shmBufSize, MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)_ctx[hnd].shm_name, sizeof(ShmCtx), &u32ShmId, (MS_VIRT*)&_ctx[hnd].shm, &u32shmBufSize, MSOS_SHM_CREATE))
        {
	return DRVCMDQ_FAIL;
        }
    }

    if( _ctx[hnd].shm->buffer_start == 0 )
         return DRVCMDQ_FAIL;

    *SmallAddr = _ctx[hnd].shm->buffer_start;
    *BigAddr = _ctx[hnd].shm->buffer_end; 
    *miu_select = _ctx[hnd].shm->miu_select; 
    return DRVCMDQ_OK;
}
/////////////////////////////////////////////////////////////////
//////// UTOPIA2 CODE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////
enum
{
    CMDQ_POOL_ID_CMDQ0 = 0,
} eCMDQ_PoolID;

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register module
//--------------------------------------------------------------------------------------------------
void CMDQRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_CMDQ, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)CMDQOpen, (FUtopiaClose)CMDQClose, (FUtopiaIOctl)CMDQIoctl);

    // 2. Resource register
    void* psResource = NULL;

    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, CMDQ_POOL_ID_CMDQ0);

    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("cmdq0", sizeof(CMDQ_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, CMDQ_POOL_ID_CMDQ0);

    //UtopiaResourceCreate("cmdq1", sizeof(CMDQ_RESOURCE_PRIVATE), &psResource);
    //UtopiaResourceRegister(pUtopiaModule, psResource, CMDQ_POOL_ID_CMDQ0);

    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, CMDQ_POOL_ID_CMDQ0);

    // You can get the number of init times, this can prevent multiple INIT
    //NOTE: we only export handle 0 to utopia ioctl driver.
    MDrv_CMDQ_Init(_ctx[0].CMDQ_BUFFER_MIU);
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use CMDQ
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 CMDQOpen(void** ppInstance, void* pAttribute)
{
    MS_U32 u32Ret = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // for multi-process safe, check if already other Instance exist
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_CMDQ, &pModule);

    if(UtopiaResourceObtain(pModule, CMDQ_POOL_ID_CMDQ0, &pResource) != 0)
    {
        _err("UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pCMDQResPri = NULL;
    UtopiaResourceGetPrivate(pResource, &pCMDQResPri);

    if(*(MS_U32 *)pCMDQResPri == 0)
    {
        *(MS_U32 *)pCMDQResPri = 1;
        UtopiaResourceRelease(pResource);
    }
    else
    {
        _err("\033[35m[CMDQ INFO] can not open an INSTANCE\033[m\n"); // joe.liu
        UtopiaResourceRelease(pResource);
        return 1;
    }
#endif

    _dbg("\033[35m[CMDQ INFO] OPEN INSTANCE...\033[m\n"); // joe.liu
    CMDQ_INSTANT_PRIVATE *pCMDQPri = NULL;

    // instance is allocated here, also can allocate private for internal use, ex, CMDQ_INSTANT_PRIVATE, ppInstance will point to a pointer of the created UTOPIA_INSTANCE
    u32Ret = UtopiaInstanceCreate(sizeof(CMDQ_INSTANT_PRIVATE), ppInstance);
    if(u32Ret)
    {
        _err("\033[35m[CMDQ OPEN] Create Instance Failed!!\033[m\n");
        return 1;
    }
    // set the pCMDQPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pCMDQPri);

    // setup func in private and assign the calling func in func ptr in instance private
    pCMDQPri->fpCMDQGetLibVer = (IOCTL_CMDQ_GETLIBVER)MDrv_CMDQ_GetLibVer;
    pCMDQPri->fpCMDQPrintf_Crash_Command = (IOCTL_CMDQ_PRINTFCRASHCOMMAND)MDrv_CMDQ_Printf_Crash_Command;
    pCMDQPri->fpCMDQReceive = (IOCTL_CMDQ_RECEIVE)MDrv_CMDQ_Receive;
    pCMDQPri->fpCMDQSet_Timer_Ratio = (IOCTL_CMDQ_SETTIMERRATIO)MDrv_CMDQ_Set_timer_ratio;
    pCMDQPri->fpCMDQSetDbgLevel = (IOCTL_CMDQ_SETDBGLEVEL)MDrv_CMDQ_SetDbgLevel;

    // 0 is pass for UtopiaOpen
    return 0;
}

MS_U32 CMDQIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    CMDQ_INSTANT_PRIVATE* psCMDQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psCMDQInstPri);

    PCMDQ_GETLIBVER_PARAM pGetLibVerParam = NULL;
    PCMDQ_RECEIVE_PARAM pReceiveParam = NULL;
    PCMDQ_SETTIMERRATIO_PARAM pSetTimerRatioParam = NULL;
    PCMDQ_SETDBGLEVEL_PARAM pSetDbgLevelParam = NULL;

    MS_U32 u32Ret;

    // no need to get resource, only one instance at one time
    switch(u32Cmd)
    {
        case MDrv_CMD_CMDQ_GetLibVer:
            _dbg("CMDQIoctl - MDrv_CMD_CMDQ_GetLibVer\n");
            pGetLibVerParam = (PCMDQ_GETLIBVER_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQGetLibVer(pGetLibVerParam->ppVersion);
            return u32Ret;
        case MDrv_CMD_CMDQ_Printf_Crash_Command:
            _dbg("CMDQIoctl - MDrv_CMD_CMDQ_Printf_Crash_Command\n");
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQPrintf_Crash_Command();
            return u32Ret;
        case MDrv_CMD_CMDQ_Receive:
            _dbg("CMDQIoctl - MDrv_CMD_CMDQ_Receive\n");
            pReceiveParam = (PCMDQ_RECEIVE_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQReceive(pReceiveParam->New_IP_Head);
            return u32Ret;
        case MDrv_CMD_CMDQ_Set_timer_ratio:
            _dbg("CMDQIoctl - MDrv_CMD_CMDQ_Set_timer_ratio\n");
            pSetTimerRatioParam = (PCMDQ_SETTIMERRATIO_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQSet_Timer_Ratio(pSetTimerRatioParam->time, pSetTimerRatioParam->ratio);
            return u32Ret;
        case MDrv_CMD_CMDQ_SetDbgLevel:
            _dbg("CMDQIoctl - MDrv_CMD_CMDQ_SetDbgLevel\n");
            pSetDbgLevelParam = (PCMDQ_SETDBGLEVEL_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQSetDbgLevel(pSetDbgLevelParam->DbgLevel);
            return u32Ret;
        default:
            break;
    };

    return 1; // FIXME: error code(if no this cmd, return fail)
}

MS_U32 CMDQClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // Restore resource pri_shm content
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_CMDQ, &pModule);

    if(UtopiaResourceObtain(pModule, CMDQ_POOL_ID_CMDQ0, &pResource) != 0)
    {
        _err("UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pCMDQResPri = NULL;
    UtopiaResourceGetPrivate(pResource, &pCMDQResPri);

    if(*(MS_U32 *)pCMDQResPri == 0)
    {
        _err("\033[35m[CMDQClose] Strange resource pri_shm content!!\033[m\n"); // joe.liu
        UtopiaResourceRelease(pResource);
        return 1;
    }
    else
    {
        _dbg("\033[35m[Multi-process Safe] Release an Instance!!\033[m\n");
        *(MS_U32 *)pCMDQResPri = 0;
        UtopiaResourceRelease(pResource);
        return 0;
    }
#endif

    return 0;
}

MS_U32 MDrv_CMDQv2_SetPowerState(int hnd, EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_U32 u32Ret, u32ShmId, u32shmBufSize;
    MS_VIRT u32shmAddr;

    MS_U32 u32STR_cmdq_miu_select;
    MS_PHY u32STR_cmdq_buffer_start, u32STR_cmdq_buffer_end;
    MS_VIRT u32VIRT_Buffer_Start;
    MS_PHY pa_start;

    _msg("\033[35mdoing MDrv_CMDQ_SetPowerState with u16PowerState is %d\033[m\n", u16PowerState);
    _dbg("\033[35mdoing MDrv_CMDQ_SetPowerState with u16PowerState is %d\033[m\n", u16PowerState);
    _err("\033[35mdoing MDrv_CMDQ_SetPowerState with u16PowerState is %d\033[m\n", u16PowerState);
    _cri("\033[35mdoing MDrv_CMDQ_SetPowerState with u16PowerState is %d\033[m\n", u16PowerState);

    if (u16PowerState == E_POWER_SUSPEND)
    {
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS; //SUSPEND_OK
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)    // we set E_POWER_SUSPEND, and then set E_POWER_RESUME
        {
            _prev_u16PowerState = u16PowerState;
            IS_STR_RESUME = 1;

            /* get a str_flag */
            u32Ret = MsOS_SHM_GetId((MS_U8*)"cmdq_str_flag", sizeof(MS_U32), &u32ShmId, &u32shmAddr, &u32shmBufSize, MSOS_SHM_QUERY);
            if(u32Ret == TRUE)
            {
                *(MS_U32*)u32shmAddr = IS_STR_RESUME;
                _dbg("\033[35mset cmdq_str_flag to be %d\033[m\n", IS_STR_RESUME);
            }
            else
            {
                _err("\033[35mget str_flag failed!!!\033[m\n");
                MS_ASSERT(0);
                return UTOPIA_STATUS_FAIL;
            }

            u32Ret = MsOS_SHM_GetId((MS_U8*)_ctx[hnd].shm_name, sizeof(ShmCtx), &u32ShmId, (MS_VIRT*)&_ctx[hnd].shm, &u32shmBufSize, MSOS_SHM_QUERY);
            if(u32Ret != TRUE)
            {
                _cri("FAIL TO ACCES SHM. can not do anything here. assert(0)!!!\n");
                u32STR_cmdq_miu_select = MIU_NOT_SUPPORT;
                u32STR_cmdq_buffer_start = 0;
                u32STR_cmdq_buffer_end = 0;
                MS_ASSERT(0);
            }
            else
            {
                u32STR_cmdq_miu_select   = _ctx[hnd].shm->miu_select;
                _dbg("do cmdq_init for miu %d\033[m\n", u32STR_cmdq_miu_select);
                u32STR_cmdq_buffer_start = _ctx[hnd].shm->buffer_start;
                u32STR_cmdq_buffer_end   = _ctx[hnd].shm->buffer_end;
            }
            MDrv_CMDQ_Init(u32STR_cmdq_miu_select);
            MDrv_CMDQ_Get_Memory_Size(u32STR_cmdq_buffer_start, u32STR_cmdq_buffer_end, u32STR_cmdq_miu_select);

            /* clear cmdq_buffer(non-cache) */
           
                _dbg("\033[35mbuffer clear for MIU1 CMDQ_BUFFER\033[m\n");
            _miu_offset_to_phy(u32STR_cmdq_miu_select,0,pa_start);	
            u32VIRT_Buffer_Start = MS_PA2KSEG1(u32STR_cmdq_buffer_start + pa_start);
            if(u32VIRT_Buffer_Start == NULL)
            {
                _dbg("\033[35mu32VIRT_Buffer_Start is 0x%X, not mapped\033[m\n", (unsigned int)u32VIRT_Buffer_Start);
            }
            else
            {
                memset((void *)u32VIRT_Buffer_Start, 0 , (u32STR_cmdq_buffer_end - u32STR_cmdq_buffer_start));
            }

            u32Return = UTOPIA_STATUS_SUCCESS; //RESUME_OK
        }
        else
        {
            _err("[%s,%5d]It is not suspended yet. We should not resume\n");
            u32Return = UTOPIA_STATUS_FAIL; //SUSPEND_FAILED
        }
		IS_STR_RESUME = 0;  /* set IS_STR_RESUME to 0, this is because all STR_RESUME action is done */
    }
    else
    {
        _err("[%s,%5d]Do Nothing: %d\n",u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;
}


///Get current number of command can be pushed into buffer (free slot)
MS_U32 MDrv_CMDQv2_Get_FreeCmdLine_Size(int hnd) {
    MS_PHY s, e, r, w;

    s = _ctx[hnd].buffer_start;
    e = _ctx[hnd].buffer_end;
    w = _ctx[hnd].buffer_wr;
    r = HAL_CMDQ_Read_Pointer(hnd);

    //@NOTE need to handle out of range and rdptr reseted value.
    if(r > e || r < s)
    {   _err("buffer_start=%#x, buffer_end=%#x, buffer_wr=%#x\n", s, e, w);
        _err("rdptr = %08x, out of buffer range.\n", r);
        _err("we assume CMDQ was in inited status.\n");
        //return _ctx[hnd].TOTAL_BUFFER_SIZE;
        r = s;
    }

    if(_ctx[hnd].CMDQ_MODE == RING_BUFFER_MODE)
    {
        //@NOTE: actually, rdptr will equal to wrptr.
        if (w==r) {
            //@NOTE: because if we fill buffer full, that rd=wr will not trigger.
            return _ctx[hnd].TOTAL_BUFFER_SIZE-1;
        } else if ( r > w ) {
            //|S|......W_________R......|E|
            return (r-w);
        } else if ( w > r ) {
            //|S|______R.........W______|E|
            return ((r-s)+(e-w)+1);
        } else {
            _cri("Internal error!\n");
        }
        return 0;
    }
    else
    {
        _cri("We only support RING_BUFFER_MODE ONLY!\n");
        MS_ASSERT(0);
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------
//  Debug function.
//--------------------------------------------------------------------------------------------------
void MDrv_CMDQv2_Dbg_WriteDummy(int hnd, MS_U32 value) {
    HAL_CMDQ_Write_Dummy_Register(hnd, value);
}

MS_U32 MDrv_CMDQv2_Dbg_ReadDummy(int hnd) {
    return HAL_CMDQ_Read_Dummy_Register(hnd);
}

void MDrv_CMDQv2_Dbg_Pause(int hnd) {
    HAL_CMDQ_Reset2(hnd, 0);
}

void MDrv_CMDQv2_Dbg_Continue(int hnd) {
    HAL_CMDQ_Reset2(hnd, 1);
}

void MDrv_CMDQv2_DumpContext(void)
{
    int i;
    MS_U32 r;

    _dbg("-- Total %d CMDQ HW ----------------------------------------------------\n", NUMBER_OF_CMDQ_HW);
    for(i=0;i<NUMBER_OF_CMDQ_HW;i++)
    {
        _dbg(" ====================================================== CMDQ #%d ===============\n", i);
        _dbg(" Mode: (%d) %s\n",
             _ctx[i].CMDQ_MODE, (_ctx[i].CMDQ_MODE==0)?"ALWAYS_WORK":"TRIGGER_MODE");

        _dbg(" Basis: miu     size              start -                end             wr_ptr\n");
        _dbg("        %3d %8lu 0x%016llX - 0x%016llX 0x%016llX\n",
             _ctx[i].CMDQ_BUFFER_MIU, _ctx[i].TOTAL_BUFFER_SIZE,
             _ctx[i].buffer_start, _ctx[i].buffer_end, _ctx[i].buffer_wr);


        _dbg(" Shm:       init miu              start -                end             wr_ptr\n");
        if(_ctx[i].shm != NULL)
        {
            _dbg("      %10s %3d 0x%016llX - 0x%016llX 0x%016llX\n",
                 (_ctx[i].shm->init==0)?"NOINIT":"INITED", _ctx[i].shm->miu_select,
                 _ctx[i].shm->buffer_start, _ctx[i].shm->buffer_end,
                 _ctx[i].shm->buffer_wr);
        }
        else
        {
            _dbg("      SHM not allocated yet.\n");
            continue;
        }

        _dbg(" rdptr: 0x%016llX\n", HAL_CMDQ_Read_Pointer(i));

        _dbg(" Stats: cmd_setcount    cmd_count     null_cnt\n");
        _dbg("        %12lu %12lu %12lu\n",
             _ctx[i].stats.cmd_setcount, _ctx[i].stats.cmd_count, _ctx[i].stats.cmd_nullcnt);

        r = HAL_CMDQ_Read_Misc_Status(i);
        _dbg(" Misc: cmdptr %s, sram %s, sram bist %s\n",
             (r&CMDQ_CMD_PTR_VLD)?"  VALID":"INVALID",
             (r&CMDQ_RD_SRAM_EMPTY)?" EMPTY":"!EMPTY",
             (r&CMDQ_SRAM_BIST_FAIL)?"FAIL":"  OK");

        r = HAL_CMDQ_Get_Debug_Step_Mode(i);
        _dbg(" Debug: debug mode %s, step mode %s\n",
             (r&CMDQ_DEBUG_MODE_ENABLE)?"ON":"OFF",
             (r&CMDQ_DEBUG_ONESTEP_ENABLE)?"DEBUG_MODE":"NORMAL_MODE");

        //access some statics or debug-assisted register here. Ex. IRQ
        {
            MS_PHY s, e, r, w;
            s = _ctx[i].buffer_start;
            e = _ctx[i].buffer_end;
            w = _ctx[i].buffer_wr;
            r = HAL_CMDQ_Read_Pointer(i);
            _dbg("s,e,w,r: %16llx, %16llx, %16llx, %16llx\n", s,e,w,r);
        }

    }
    _dbg("-- CMDQ Dump Context ----------------------------------------------------------\n");
}


