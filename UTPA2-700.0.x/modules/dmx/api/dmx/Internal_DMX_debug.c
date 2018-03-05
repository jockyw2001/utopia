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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDMX.c
/// @brief  Demux API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MSOS_TYPE_LINUX_KERNEL
#else
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "drvWDT.h"
#include "drvTSP.h"
#include "apiDMX.h"
#include "drvMMIO.h"
#include "Internal_DMX_debug.h"

#if DMX_DEBUG
#if defined(CHIP_NAPOLI)
#include "../../hal/napoli/tsp/halTSP.h"
#elif defined(CHIP_CLIPPERS)
#include "../../hal/clippers/tsp/halTSP.h"
#elif defined(CHIP_EIFFEL)
#include "../../hal/eiffel/tsp/halTSP.h"
#elif defined(CHIP_MADISON)
#include "../../hal/madison/tsp/halTSP.h"
#elif defined(CHIP_MIAMI)
#include "../../hal/miami/tsp/halTSP.h"
#elif defined(CHIP_MONACO)
#include "../../hal/monaco/tsp/halTSP.h"
#elif defined(CHIP_NIKE)
#include "../../hal/nike/tsp/halTSP.h"
#elif defined(CHIP_MUNICH)
#include "../../hal/munich/tsp/halTSP.h"
#else
#error "no tsp path specify\n"
#endif

//------------------------------------------------------------------------------
// Compile options
//------------------------------------------------------------------------------
static int _DMX_DBG_Log(char* param1);
static int _DMX_DBG_Pvr(char* param1);
static int _DMX_DBG_Flt(char* param1);
static int _DMX_DBG_Misc(char* param1);

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Constant definition
//------------------------------------------------------------------------------
#define DMX_DBG_STACK_SIZE 4096
#define DMX_DBG_TASK_DELAY 100
#define MAX_NUM_PID_MONITOR 16
#define DMX_DBG_PVR_TASK_NAME "DMX_DBG_Task"
#define DMX_DBG_TASK_NAME "DMX_DBG_Task"
#define DMX_DBG_FILE_NAME "DMXDBG"

#define DMX_DBG_FILE_PATH_ENV "ENV_DMX_DBG" 
//------------------------------------------------------------------------------
// Internal data structure
//------------------------------------------------------------------------------
typedef int (*dbg_func)(char* param1);
typedef struct 
{
    dbg_func  pfun;
    const char *cmd_str;
    const char *cmd_help;
} DMX_DBG_CMD;

//------------------------------------------------------------------------------
// Helper Macros
//------------------------------------------------------------------------------
#define GET_NUM(pCh, u32x)                          \
    if(strncmp(pCh, "0x", 2) == 0)                  \
    {sscanf(pCh, "%x", (unsigned int*)&u32x);}else  \
    {sscanf(pCh, "%d", (unsigned int*)&u32x);}      
        
#define GET_PARAM32(_pCh_, _id_, _bFound_, _u32x_)        \
    {                                                       \
     strncpy(_u8Tmp, _pCh_, 128);                           \
     char* _pc_ = strstr(_u8Tmp, _id_);                     \
     if(_pc_)                                               \
        {_bFound_ = TRUE;}else{_bFound_ = FALSE;}           \
     if(_pc_ && strtok(_pc_+2, " "))                        \
     {                                                      \
        _pc_ = strtok(_pc_+2, " ");                         \
        GET_NUM(_pc_, _u32x_);                              \
     }                                                      \
    }

#define GET_PARAM_STR(_pCh_, _id_, _bFound_, _path_)        \
    {                                                       \
     strncpy(_u8Tmp, _pCh_, 128);                           \
     char* _pc_ = strstr(_u8Tmp, _id_);                     \
     if(_pc_)                                               \
        {_bFound_ = TRUE;}else{_bFound_ = FALSE;}           \
     if(_pc_ && strtok(_pc_+2, " "))                        \
     {                                                      \
        _pc_ = strtok(_pc_+2, " ");                         \
        if(strlen(_pc_) > 128)                              \
        {dmx_dbg_print("[%s][%d] file path overflow %d\n", __FUNCTION__, __LINE__, (int)strlen(_pc_));break;} \
        memset(_path_, 0x0, 128);                           \
        sscanf(_pc_, "%s", (char*)_path_);                  \
     }                                                      \
    }

#define GET_PARAM_CH(_pCh_, _id_, _bFound_, _ch_)           \
    {                                                       \
     strncpy(_u8Tmp, _pCh_, 128);                           \
     char* _pc_ = strstr(_u8Tmp, _id_);                     \
     if(_pc_)                                               \
        {_bFound_ = TRUE;}else{_bFound_ = FALSE;}           \
     if(_pc_ && strtok(_pc_+2, " "))                        \
     {                                                      \
        _pc_ = strtok(_pc_+2, " ");                         \
        sscanf(_pc_, "%c", (char*)&_ch_);                   \
     }                                                      \
    }

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    #define PID_FLT_P(_p_, _id_)                                \
        _p_ = &(_TspPid[0].Flt[_id_]);                              // low half of the pid filter for eiffel~ 
    
    #define SEC_FLT_P(_p_, _id_)                                \
        _p_ = (REG_SecFlt*)&(_TspSec1[0].Flt[i]);
#else
    #define PID_FLT_P(_p_, _id_)                                \
        _p_ = HAL_TSP_GetPidFltReg(0, _id_);
    
    #define SEC_FLT_P(_p_, _id_)                                \
        _p_ = (REG_SecFlt*)&(_TspSec[0].Flt[i]);
#endif    
    

//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
#define DMX_DBG_CMD_LOG  0
#define DMX_DBG_CMD_PVR  1
#define DMX_DBG_CMD_FLT  2
#define DMX_DBG_CMD_MISC 3

#define DMX_DBG_LOG_OPT_TYPE    "-s" // what kind of info to show
#define DMX_DBG_LOG_OPT_PERIOD  "-t" // the period of time to show info
#define DMX_DBG_LOG_OPT_PID     "-p" // PIDs to be monitor
#define DMX_DBG_LOG_OPT_RESET   "-r" // reset the log
#define DMX_DBG_LOG_OPT_LV      "-l" // debug level
#define DMX_DBG_LOG_OPT_FILE    "-F" // file path to store dump section data
#define DMX_DBG_LOG_OPT_FLTID   "-f" // DMX filter id to monitor

#define DMX_DBG_PVR_OPT_PID     "-p" // PIDs to record

#define DMX_DBG_FLT_OPT_FLTTYPE "-t" // DMX filter type to open
#define DMX_DBG_FLT_OPT_FLTID   "-f" // DMX filter id
#define DMX_DBG_FLT_OPT_PID     "-p" // PID
#define DMX_DBG_FLT_OPT_BUFADDR "-a" // physical address of section buffer
#define DMX_DBG_FLT_OPT_BUFSIZE "-s" // size of section buffer
//#define DMX_DBG_FLT_OPT_PATTERN "-m" // match pattern

#define DMX_DBG_MISC_OPT_ADDR   "-a" // physical address of memory to dump
#define DMX_DBG_MISC_OPT_SIZE   "-s" // size to dump
#define DMX_DBG_MISC_OPT_BANK   "-b" // bank number
#define DMX_DBG_MISC_OPT_OFSET  "-o" // address offset (16bit)
#define DMX_DBG_MISC_OPT_VALUE  "-v" // value write to register
#define DMX_DBG_MISC_OPT_FILE   "-f" // file path
static    DMX_DBG_CMD _stCmd[] = {    
          {_DMX_DBG_Log, "log", "[0]"COLOR_BROWN" DMX log selection\n\t"COLOR_DBG
                                "Usage : "COLOR_BROWN"log"COLOR_DBG" [options]\n"
                                "\tOptions   :\n"
                                "\t\t"DMX_DBG_LOG_OPT_TYPE": bit field of log selection\n"
                                    "\t\t\t0x00000001 : filter open\n"
                                    "\t\t\t0x00000002 : filter set PID\n"
                                    "\t\t\t0x00000004 : filter close\n"
                                    "\t\t\t0x00000008 : filter enable / disable\n"
                                    "\t\t\t0x00000010 : filter info\n"
                                    "\t\t\t0x00000020 : filter callback\n"
                                    "\t\t\t0x00000080 : file in operation\n"

                                    "\t\t\t0x00000100 : copy data\n"
                                    "\t\t\t0x00000200 : dump copied data\n"
                                    "\t\t\t0x00000400 : DMX flow setting\n"
                                    "\t\t\t0x00000800 : section overflow\n"
                                    "\t\t\t0x00001000 : section arrival\n"                                    
                                    "\t\t\t0x00002000 : STC change\n"
                                    "\t\t\t0x00004000 : PCR\n"

                                    "\t\t\t0x00080000 : show DMX interrupt counter\n"                                    
                                    "\t\t\t0x00100000 : show file in read address\n"
                                    "\t\t\t0x00200000 : show PVR write address\n"                                    
                                    "\t\t\t0x00400000 : a/v fifo reset and status\n"
                                    "\t\t\t0x02000000 : a/v discontinuity packet count\n"
                                    "\t\t\t0x04000000 : a/v drop packet count\n"
                                    "\t\t\t0x08000000 : lock packet count\n"
                                    "\t\t\t0x10000000 : STC\n"
                                    "\t\t\t0x20000000 : a/v packet count\n"
                                    "\t\t\t0x40000000 : a/v overflow\n"
                                    "\t\t\t0x80000000 : check AEON alive\n"
                                "\t\t"DMX_DBG_LOG_OPT_LV": debug level\n"
                                "\t\t"DMX_DBG_LOG_OPT_PERIOD": period of time(ms) to show info, default is 100ms\n"
                                "\t\t"DMX_DBG_LOG_OPT_PID": PID to monitor("DMX_DBG_LOG_OPT_PID"0x100,...)\n"
                                "\t\t"DMX_DBG_LOG_OPT_RESET": reset log to default\n"
                                "\t\t"DMX_DBG_LOG_OPT_FILE": file path to store dump section data\n\n"
                                "\tex. log "DMX_DBG_LOG_OPT_TYPE"0x00000001\t(show filter open event)\n"
                                "\tex. log "DMX_DBG_LOG_OPT_LV"5\t(show all the API call event)\n"
                                "\tex. log "DMX_DBG_LOG_OPT_TYPE"0x00000200\t"DMX_DBG_LOG_OPT_PID"0x0\t"DMX_DBG_LOG_OPT_FILE"/temp/pat.bin (dump pat to /temp/pat.bin)\n"
                                "\tex. log "DMX_DBG_LOG_OPT_TYPE"0x0000000F "DMX_DBG_LOG_OPT_PID"0x0 0x100 0x200\n"
                                "\tex. log "DMX_DBG_LOG_OPT_TYPE"0x00004000 "DMX_DBG_LOG_OPT_PERIOD"10\t(show PCR every 10 ms)\n\n\n"
                                },

          {_DMX_DBG_Pvr, "pvr", "[1]"COLOR_BROWN" Record stream\n\t"COLOR_DBG
                                "Usage : "COLOR_BROWN"pvr"COLOR_DBG" file RecordAll CA address size [options]\n"
                                "\t\tfile      : writable file path to store recorded data\n"
                                "\t\tRecordAll : record all stream\n"
                                "\t\tCA        : record stream through dscmb engine\n"
                                "\t\taddress   : physical address of record buffer\n"
                                "\t\tsize      : size of record buffer\n\n"
                                "\t\tOptions   : \n"
                                "\t\t"DMX_DBG_PVR_OPT_PID": PIDs to record\n\n"
                                "\tex. pvr /temp/pvr.bin 0 0 0x12345678 0x10000\t(record current a/v PID)\n"
                                "\tex. pvr /temp/pvr.bin 0 0 0x12345678 0x10000 "DMX_DBG_PVR_OPT_PID"0x0"
                                "\t(record pid 0x0 to /temp/pvr.bin)\n\n"
                                "\tps. fire again to stop pvr\n\n\n"},

          {_DMX_DBG_Flt, "flt", "[2]"COLOR_BROWN" DMX filter operation\n\t"COLOR_DBG
                                "Usage : "COLOR_BROWN"flt"COLOR_DBG" s/o/c [options]\n"
                                "\t\ts : show filter information\n"
                                "\t\to : open filter\n"
                                "\t\tc : close filter\n\n"
                                "\t\tOptions   : \n"
                                "\t\t"DMX_DBG_FLT_OPT_FLTTYPE": DMX filter type to open\n"
                                "\t\t\ta : audio filter\n"
                                "\t\t\tv : video filter\n"
                                "\t\t\ts : section filter\n"
                                "\t\t\tp : packet filter\n"
                                "\t\t"DMX_DBG_FLT_OPT_FLTID": DMX filter index\n"
                                "\t\t"DMX_DBG_FLT_OPT_PID": PID to open / close\n"
                                "\t\t"DMX_DBG_FLT_OPT_BUFADDR": physicall address section buffer\n\n"
                                "\t\t"DMX_DBG_FLT_OPT_BUFSIZE": size of section buffer\n\n"
                                
//                                "\t\t"DMX_DBG_FLT_OPT_PATTERN": match/mask/nmask\n"
                                "\tex. flt c "DMX_DBG_FLT_OPT_FLTID"10\t(close filter 10)\n"
                                "\tex. flt c "DMX_DBG_FLT_OPT_PID"0x100\t(close filters which PID = 0x100)\n"
                                "\tex. flt o "DMX_DBG_FLT_OPT_FLTTYPE"v "DMX_DBG_FLT_OPT_PID"0x100\t(open PID = 0x100 video filter)\n"
                                "\tex. flt o "DMX_DBG_FLT_OPT_FLTTYPE"s "DMX_DBG_FLT_OPT_PID"0x0 "DMX_DBG_FLT_OPT_BUFADDR"0x800000 "DMX_DBG_FLT_OPT_BUFADDR"0x10000\n"
                                "\tex. flt s "DMX_DBG_FLT_OPT_FLTID"10\t(show filter 10 info)\n"
                                "\tex. flt s "DMX_DBG_FLT_OPT_PID"0x100\t(show filter info which PID = 0x100)\n\n\n"
                                                                },
          {_DMX_DBG_Misc, "misc", "[3]"COLOR_BROWN" Miscellaneous\n\t"COLOR_DBG
                                  "Usage : "COLOR_BROWN"misc"COLOR_DBG" d/c/b/m [options]\n"
                                  "\t\td : show DMX interface configuration\n"
                                  "\t\tc : show DMX capability(TODO)\n"
                                  "\t\tb : dump bank register\n"
                                  "\t\tm : dump memory(physical) [to file]\n\n"
                                  "\t\tOptions   : \n"
                                  "\t\t"DMX_DBG_MISC_OPT_FILE": file path\n"
                                  "\t\t"DMX_DBG_MISC_OPT_ADDR": physical address to dump\n"
                                  "\t\t"DMX_DBG_MISC_OPT_SIZE": length to dump\n"
                                  "\t\t"DMX_DBG_MISC_OPT_BANK": bank address\n\n"
                                  "\tex. misc d\n"
                                  "\tex. misc m "DMX_DBG_MISC_OPT_ADDR"0x100000 "DMX_DBG_MISC_OPT_SIZE"0x1000 -f/Customer/driver/dump\t(dump address 0x100000~0x101000 to /Customer/driver/dump)\n"
                                  "\tex. misc b "DMX_DBG_MISC_OPT_BANK"0x101E\t(dump register bank 0x101E)\n"
                                  "\tex. misc b "DMX_DBG_MISC_OPT_BANK"0x101E "DMX_DBG_MISC_OPT_OFSET"0x20 "DMX_DBG_MISC_OPT_VALUE"0x5A\t(write 0x5A to bank 0x101E, offset 0x20)\n\n\n"
                                                                },                                                                
          {NULL, NULL, NULL}
          };
          
static    DMX_DMX_INPUT_STR _stDmxInput[] = {
          {DMX_FLOW_INPUT_DEMOD,        "Int. DEMOD"},
          {DMX_FLOW_INPUT_MEM,          "MEM"},
          {DMX_FLOW_INPUT_EXT_INPUT0,   "Ext. Input0"},
          {DMX_FLOW_INPUT_EXT_INPUT1,   "Ext. Input1"},
          {DMX_FLOW_INPUT_EXT_INPUT2,   "Ext. Input2"},
          {DMX_FLOW_INPUT_EXT_INPUT3,   "Ext. Input3"},
          {DMX_FLOW_INPUT_MEM_NOPASSCA, "MEM, not through CA"},
          {DMX_FLOW_INPUT_DEMOD1,       "DEMOD1"},
          {DMX_FLOW_INPUT_TSO,          "TSO_0"},
          {DMX_FLOW_INPUT_TSO1,         "TSO_1"},
          {DMX_FLOW_INPUT_MEM_PASSCA1,  "MEM, through CA1"},
          {DMX_FLOW_INPUT_DISABLE,      "no Input"}};    

          DMX_AV_FIFO_STR stDmxType[] = {
          {DMX_FILTER_TYPE_VIDEO,       "VID"},
          {DMX_FILTER_TYPE_AUDIO,       "AUD"},
          {DMX_FILTER_TYPE_AUDIO2,      "AUD2"},
          {DMX_FILTER_TYPE_SECTION,     "SEC"},
          {DMX_FILTER_TYPE_PES,         "PES"},
          {DMX_FILTER_TYPE_PACKET,      "PKT"},
          {DMX_FILTER_TYPE_PCR,         "PCR"},
          {DMX_FILTER_TYPE_TELETEXT,    "TTX"},
          {DMX_FILTER_TYPE_AUDIO3,      "AUD3"},
          {DMX_FILTER_TYPE_AUDIO4,      "AUD4"},
          {DMX_FILTER_TYPE_REC,         "PVR"},
          {DMX_FILTER_TYPE_SCMBCHK,     "SCMBCHK"},
          {DMX_FILTER_TYPE_SECTION_VER, "VER"}};
            
static    MS_U8 _u8PvrStack[DMX_DBG_STACK_SIZE];
static    MS_U8 _u8DbgStack[DMX_DBG_STACK_SIZE];
static    FILE* pfPVR = NULL;
FILE*     pfSEC = NULL;
static    FILE* pfDmxDbg = NULL;
static    MS_U8 u8PvrIdx[16] = {0};

static    MS_U32 u32PhyWirte = 0;
static    MS_U32 u32VirWrite = 0;
static    MS_U32 u32VirRead = 0;
static    MS_U32 u32VirStart = 0;
static    MS_U32 u32VirEnd = 0;

static    MS_S32 s32DmxDbgPvrTaskId = -1;
static    MS_S32 s32DmxDbgTaskId = -1;

static    MS_BOOL bPvrCA = FALSE;
static    MS_U32 u32DmxDbgTaskDelay = DMX_DBG_TASK_DELAY;

static    MS_U32 u32PrevPCR, u32PrevSTC, u32PrevTime;

static    MS_U32 u32TimeStartPvr;
static    MS_U32 u32SizeRec;
static    MS_BOOL bMonitorAV = FALSE;
static    MS_U8 u8PvrIdxVid = 0;
static    MS_U8 u8PvrIdxAud = 0;
static    MS_U16 u16PvrPidAud = 0;
static    MS_U16 u16PvrPidVid = 0;
static    char _u8Tmp[128];
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
MS_U32      u32PidMonitor[MAX_NUM_PID_MONITOR] = {-1UL};
MS_U32      u32NumFltMonitor = 0;
MS_U32      u32DbgLogFlag = 0;
MS_S32      s32DmxDbgMutex = -1;
MS_U32      u32DmxDbgFltId = 0xFFFFFFFF;
DMX_FLT_DBG_INFO    stDmxDbgInfo[TSP_SECFLT_NUM];

//------------------------------------------------------------------------------
// Internal implementation
//------------------------------------------------------------------------------
void _DMX_DBG_LogSel(MS_U32 u32Flag)
{
    u32DbgLogFlag = u32Flag;
}

void _DMX_DBG_PidMonitor(MS_U32 *u32Pids, MS_U32 u32Num)
{
    if(u32Num > MAX_NUM_PID_MONITOR)
    {
        dmx_dbg_print("[%s] monitor number exceed limitation\n", __FUNCTION__);
        return;
    }

    DMX_DBG_ENTRY();
    u32NumFltMonitor = u32Num;
    memcpy(u32PidMonitor, u32Pids, sizeof(MS_U32) * u32Num);
    
    DMX_DBG_EXIT();
    
}

MS_U32 _DMX_DBG_GetPID(DMX_FILTER_TYPE FltType, MS_U16* u16Pid)
{
    int i;
    MS_U32 u32MaxFlt = 0;
    DMX_Flt_info info;
    DMX_FILTER_TYPE type;
    
    *u16Pid = 0x1FFF;
    
    MApi_DMX_GetCap(DMX_CAP_PID_FILTER_NUM, &u32MaxFlt);

    for(i = 0; i< u32MaxFlt; i++)
    {
        MApi_DMX_Info(i, &info, &type, FALSE);
        if(type == FltType)
        {
            MApi_DMX_Pid(i, u16Pid, FALSE);
            return i;
        }
    }
    return 0xFFFFFFFF;
}

static int _DMX_DBG_Log(char* param1)
{
    char *pCh;
    int i = 0;
    MS_U32 u32DbgLv = 0, u32dummy;
    MS_BOOL bWrongParam = FALSE;
    MS_BOOL bFound, bRst;
    char path[128] = {0x0};
    
    if(strlen(param1) > 128)
    {
        dmx_dbg_print("[%s][%d] buffer overflow \n", __FUNCTION__, __LINE__);
        return 0;
    }    

    do
    {
        DMX_DBG_ENTRY();
        u32DmxDbgTaskDelay = DMX_DBG_TASK_DELAY;
        u32NumFltMonitor = 0;
        memset(u32PidMonitor, 0x0, sizeof(MS_U32)*MAX_NUM_PID_MONITOR);
        DMX_DBG_EXIT();
    
        GET_PARAM32(param1, DMX_DBG_LOG_OPT_TYPE, bFound, u32DbgLogFlag);    
        
        if(bFound)
            ULOGD("DMX", "u32DbgLogFlag = 0x%x\n", (unsigned int)u32DbgLogFlag);
            
        GET_PARAM32(param1, DMX_DBG_LOG_OPT_PERIOD, bFound, u32DmxDbgTaskDelay);    

        if(bFound)
            ULOGD("DMX", "u32DmxDbgTaskDelay = %d ms\n", (unsigned int)u32DmxDbgTaskDelay);

        if((!(u32DbgLogFlag & DMX_DBG_LOG_COPYDATA1)) && (pfSEC != NULL))
        {
            fclose(pfSEC);
            pfSEC = NULL;
        }               
        
        GET_PARAM32(param1, DMX_DBG_LOG_OPT_FLTID, bFound, u32DmxDbgFltId);
        if(bFound)
        {
           dmx_dbg_print("\tmonitor flt %d\n", (int)u32DmxDbgFltId); 
        } 
        else
        {
            u32DmxDbgFltId = 0xFFFFFFFF;
        }        
        GET_PARAM32(param1, DMX_DBG_LOG_OPT_RESET, bRst, u32dummy);
        if(bRst)
        {
            u32DbgLogFlag = 0x0;
            u32DmxDbgTaskDelay = DMX_DBG_TASK_DELAY;
    
            DMX_DBG_ENTRY();
            u32NumFltMonitor = 0;
            memset(u32PidMonitor, 0x0, sizeof(MS_U32)*MAX_NUM_PID_MONITOR);
            DMX_DBG_EXIT();
            
            if(pfSEC != NULL)
            {
                fclose(pfSEC);
                pfSEC = NULL;
            }
        }

        
        GET_PARAM32(param1, DMX_DBG_LOG_OPT_LV, bFound, u32DbgLv);    
        if(bFound)
        {
            ULOGD("DMX", "u32DbgLv = %d ms\n", (unsigned int)u32DbgLv);
            if(u32DbgLv > DMX_DBG_FUNC)
            {
                dmx_dbg_print("wrong parameter [DEBUG log level %d]\n", (int)u32DbgLv);
                bWrongParam = TRUE;
                break;
            }
            else
            {
                MApi_DMX_SetDbgLevel((DMX_DBGMSG_LEVEL)u32DbgLv);
            }
        }
                
        GET_PARAM_STR(param1, DMX_DBG_LOG_OPT_FILE, bFound, path);
        if(bFound && (u32DbgLogFlag & DMX_DBG_LOG_COPYDATA1))
        {
            if(pfSEC != NULL)
            {
                fclose(pfSEC);
                pfSEC = NULL;
            }
                
            pfSEC = fopen(path, "wb");
            if(pfSEC == NULL)
            {
                dmx_dbg_print("[%s] open file %s failed\n", __FUNCTION__, path);
                break;
            }
            else
            {
                dmx_dbg_print("dump section data to %s\n", path);
            }
        }
                
        strncpy(_u8Tmp, param1, 128);
        pCh = strstr(_u8Tmp, DMX_DBG_LOG_OPT_PID);
        if(pCh && strtok(pCh+2, " "))
        {
            dmx_dbg_print("PIDs to monitor\n");
            pCh+=2;
            pCh = strtok(pCh, "-\n");
            pCh = strtok(pCh, " ,");
            for(i = 0; i < MAX_NUM_PID_MONITOR; i++)
            {
                GET_NUM(pCh, u32PidMonitor[i]);
                dmx_dbg_print("PID[0x%x]\n", (unsigned int)u32PidMonitor[i]);
                if(u32PidMonitor[i] > 0x1FFF)
                {
                    dmx_dbg_print("wrong parameter [PID 0x%x]\n", (unsigned int)u32PidMonitor[i]);
                    bWrongParam = TRUE;
                    break;
                }
                
                u32NumFltMonitor++;
                pCh = strtok(NULL, ", ");
                if(pCh == NULL)
                    break;
            }
        }
    }while(0);
    // show command help information
    if(bWrongParam)
    {
        DMX_DBG_ENTRY();
        u32NumFltMonitor = 0;
        memset(u32PidMonitor, 0x0, sizeof(MS_U32)*MAX_NUM_PID_MONITOR);
        DMX_DBG_EXIT();

        u32DmxDbgTaskDelay = DMX_DBG_TASK_DELAY;
                    
        dmx_dbg_print("%s\n", _stCmd[DMX_DBG_CMD_LOG].cmd_help);
    }
    return 0;
}

// [file path] [RecordAll] [CA] [buffer address] [size] [PIDs]
static int _DMX_DBG_Pvr(char* param1)
{
    char* pch = NULL;
    char* pfRecord = NULL;
    MS_U32 u32NumPids = 0;
    unsigned int u32Pids[16] = {0};
    unsigned int u32Addr, u32Size, bRecordAll, bCA;
    MS_BOOL bWrongParam = FALSE;
    int i = 0;
    char tmp[128] = {0x0};
    
    if(strlen(param1) > 128)
    {
        dmx_dbg_print("[%s][%d] buffer overflow \n", __FUNCTION__, __LINE__);
        return 0;
    }    

    do
    {
        strncpy(tmp, param1, 128);
    
        pfRecord = strtok(param1, " ,");
        if(pfRecord == NULL)
        {
            dmx_dbg_print("wrong parameter [file path]\n");
            bWrongParam = TRUE;
            break;
        }
        
        pch = strtok(NULL, " ,");
        if(pch == NULL)
        {
            dmx_dbg_print("wrong parameter [RecordAll]\n");
            bWrongParam = TRUE;
            break;
        }
        GET_NUM(pch, bRecordAll);
    //    bRecordAll = (MS_BOOL)atoi(pch);
    
        pch = strtok(NULL, " ,");
        if(pch == NULL)
        {
            dmx_dbg_print("wrong parameter [CA]\n");
            bWrongParam = TRUE;
            break;
        }
        GET_NUM(pch, bCA);
    //    bCA = (MS_BOOL)atoi(pch);

        pch = strtok(NULL, " ,");
        if(pch == NULL)
        {
            dmx_dbg_print("wrong parameter [buffer address]\n");
            bWrongParam = TRUE;
            break;
        }
        GET_NUM(pch, u32Addr);
        //u32Addr = (MS_BOOL)atoi(pch);
    
        pch = strtok(NULL, " ,");
        if(pch == NULL)
        {
            dmx_dbg_print("wrong parameter [buffer size]\n");
            //bWrongParam = TRUE;
            break;
        }
        GET_NUM(pch, u32Size);
    //    u32Size = (MS_BOOL)atoi(pch);
        
        pch = strstr(tmp, DMX_DBG_LOG_OPT_PID);
        
        if(pch && strtok(pch+2, " "))
        {
            
            u32NumPids = 0;
            memset(u32Pids, 0x0, sizeof(MS_U32)*16);
            
            //ULOGD("DMX", "1 pch = %s\n", pch);
            pch+=2;
            pch = strtok(pch, " ,");
            
            //ULOGD("DMX", "2 pch = %s\n", pch);
            for(i = 0; i < MAX_NUM_PID_MONITOR; i++)
            {
                GET_NUM(pch, u32Pids[i]);
                dmx_dbg_print("record PID = 0x%x\n", (unsigned int)u32Pids[i]);
    
                if(u32Pids[i] > 0x1FFF)
                    break;
    
                u32NumPids++;
                pch = strtok(NULL, ", ");
                if(pch == NULL)
                    break;
            }
        }
        
    }while(0);
    dmx_dbg_print("pfRecord        = %s\n", pfRecord);
    dmx_dbg_print("bRecordAll      = %d\n", (int)bRecordAll);
    dmx_dbg_print("bCA             = %d\n", (int)bCA);
    dmx_dbg_print("buffer address  = 0x%x\n", (unsigned int)u32Addr);
    dmx_dbg_print("buffer size     = 0x%x\n", (unsigned int)u32Size);
    dmx_dbg_print("# PID to record = %d\n",(int)u32NumPids);
    if(!bRecordAll)
    {
        if(u32NumPids == 0)
        {
            _DMX_DBG_GetPID(DMX_FILTER_TYPE_VIDEO, &u16PvrPidVid);
            _DMX_DBG_GetPID(DMX_FILTER_TYPE_AUDIO, &u16PvrPidAud);
            dmx_dbg_print("PID not specify, record current Video[0x%x] Audio[0x%x]\n", 
                            (unsigned int)u16PvrPidVid, (unsigned int)u16PvrPidAud);
                           
            if(u16PvrPidVid != 0x1FFF)                            
            {
                u32Pids[u32NumPids] = u16PvrPidVid;
                u32NumPids++;
            }

            if(u16PvrPidAud != 0x1FFF)                            
            {
                u32Pids[u32NumPids] = u16PvrPidAud;
                u32NumPids++;
            }
            bMonitorAV = TRUE;
         }
    }    
    
    if(bWrongParam == TRUE)
    {
        dmx_dbg_print("%s\n", _stCmd[DMX_DBG_CMD_PVR].cmd_help);
    }
    
    //ULOGD("DMX", "pfPVR = 0x%x\n", (unsigned int)pfPVR);
    //ULOGD("DMX", "bWrongParam = 0x%x\n", (int)bWrongParam);
    //ULOGD("DMX", "u32NumPids = 0x%x\n", (unsigned int)u32NumPids);
    if((u32TimeStartPvr == 0) && (bWrongParam == FALSE))
    {
        ULOGD("DMX", "_DMX_DBG_Record_Start\n");
        _DMX_DBG_Record_Start(pfRecord, (MS_U32*)u32Pids, u32NumPids, u32Addr, u32Size, bCA, bRecordAll);
        if(bMonitorAV == TRUE)
        {
            u8PvrIdxVid = u8PvrIdx[0];            
            u8PvrIdxAud = u8PvrIdx[1];            
        }
    }
    else if(u32TimeStartPvr != 0)
    {
        dmx_dbg_print("stop recording\n");
        _DMX_DBG_Record_Stop();
    }
    return 0;
}

static MS_U8 u8TmpSecBuf[4096];
void _DMX_DbgCb(MS_U8 u8DmxId, DMX_EVENT enEvent)
{
    MS_U32 u32Read = 0;
    MS_U32 u32Rmn = 0;
    if(enEvent == DMX_EVENT_DATA_READY)
    {
        dmx_dbg_print("[%s] Flt[%d] data ready\n", __FUNCTION__, (int)u8DmxId);
        if(DMX_FILTER_STATUS_OK != MApi_DMX_CopyData(u8DmxId, u8TmpSecBuf, 4096, &u32Read, &u32Rmn, NULL))
        {
            dmx_dbg_print("Flt[%d] copy data error\n", (int)u8DmxId);
        }
    }

    if(enEvent == DMX_EVENT_BUF_OVERFLOW)
    {
        dmx_dbg_print("[%s] Flt[%d] buffer overflow\n", __FUNCTION__, (int)u8DmxId);
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Start(u8DmxId);
    }    
}

static int _DMX_DBG_Flt(char* param1)
{
    char* pch = NULL;
    MS_U32 u32FltOp = 0;// 1 : show, 2 : close, 3 : open
    MS_BOOL bWrongParam = FALSE, bFound;
    MS_U32 u32Pid   = 0xFFFFFFFF;
    MS_U32 u32FltId = 0xFFFFFFFF;
    MS_U32 u32Addr = 0;
    MS_U32 u32Size = 0;
//    MS_U32 u32Match, u32Mask, u32NMask;
    MS_U8 u8FltType = 0;
    MS_U32 u32MaxFlt = 0;
    int i;
    
    if(strlen(param1) > 128)
    {
        dmx_dbg_print("[%s][%d] buffer overflow \n", __FUNCTION__, __LINE__);
        return 0;
    }
    
    MApi_DMX_GetCap(DMX_CAP_PID_FILTER_NUM, &u32MaxFlt);
    do
    {
        GET_PARAM_CH(param1, DMX_DBG_FLT_OPT_FLTTYPE, bFound, u8FltType);
        if(bFound && (u8FltType!= 'a') && (u8FltType != 'v') && (u8FltType != 's') && (u8FltType != 'p'))
        {
            dmx_dbg_print("wrong parameter [DMX filter type %c]\n", (char)u8FltType);
            bWrongParam = TRUE;
            break;
        }

        GET_PARAM32(param1, DMX_DBG_FLT_OPT_PID, bFound, u32Pid);
        if(bFound)
        {
            ULOGD("DMX", "PID = 0x%x\n", (unsigned int)u32Pid);
            if(u32Pid > 0x1FFF)
            {
                dmx_dbg_print("wrong parameter [PID 0x%x]\n", (unsigned int)u32Pid);
                bWrongParam = TRUE;
                break;
            }
        }

        GET_PARAM32(param1, DMX_DBG_FLT_OPT_BUFADDR, bFound, u32Addr);
        GET_PARAM32(param1, DMX_DBG_FLT_OPT_BUFSIZE, bFound, u32Size);


#if 0        
        strncpy(tmp, param1, 128);
        pch = strstr(tmp, DMX_DBG_FLT_OPT_PATTERN);
        if(pch)
        {
            ULOGD("DMX", "-m pch = %s\n", pch);
            GET_NUM(pch+2, u32Match);
            ULOGD("DMX", "u32Mask = 0x%x\n", (unsigned int)u32Match);
            pch = strtok(pch+2, "-\n");
            pch = strtok(pch+2, " ,");
            ULOGD("DMX", "1 pch = %s\n", pch);
            pch = strtok(NULL, " ,");
            ULOGD("DMX", "2 pch = %s\n", pch);
            GET_NUM(pch, u32Mask);
            ULOGD("DMX", "u32Ptn = 0x%x\n", (unsigned int)u32Mask);        
            pch = strtok(NULL, " ,");
            ULOGD("DMX", "3 pch = %s\n", pch);
            GET_NUM(pch, u32NMask);
            ULOGD("DMX", "u32Ptn = 0x%x\n", (unsigned int)u32NMask);   
        }  
#endif

        GET_PARAM32(param1, DMX_DBG_FLT_OPT_FLTID, bFound, u32FltId);
        if(bFound && (u32FltId >= u32MaxFlt))
        {
            ULOGD("DMX", "u32FltId = %d\n", (int)u32FltId);
            dmx_dbg_print("wrong parameter [DMX filter index %d]\n", (int)u32FltId);
            bWrongParam = TRUE;
            break;
        }
            
        pch = strtok(param1, " ,");
        if(pch[0] == 's')
        {
            u32FltOp = 1;
        }
        else if(pch[0] == 'c')
        {
            u32FltOp = 2;
        }
        else if(pch[0] == 'o')
        {
            u32FltOp = 3;
        }
        else
        {
            dmx_dbg_print("\n\nwrong parameter [unknown operation %c]\n\n", (char)pch[0]);
            bWrongParam = TRUE;
            break;
        }
        
        if(u32FltOp == 1) // show filter information
        {
            DMX_Flt_info info;
            DMX_FILTER_TYPE type;

            dmx_dbg_print("======== DMX filters ========\n");
            if((u32FltId != 0xFFFFFFFF) || (u32Pid != 0xFFFFFFFF))
            {
                REG_SecFlt *pSecFilter;
                
                for(i = 0; i < u32MaxFlt; i++)
                {
                    REG_PidFlt* pPidFilter;
                    PID_FLT_P(pPidFilter, i);
                    
                    if((i == u32FltId) || 
                       ((u32Pid != 0xFFFFFFFF) && 
                       (u32Pid == (HAL_REG32_IndR((REG32 *)pPidFilter) & DMX_PID_NULL))))
                    {
                        SEC_FLT_P(pSecFilter, i);
                        dmx_dbg_print("[DMX filter %d] 0x%08X\n", (int)i, 
                                (unsigned int)HAL_REG32_IndR((REG32 *)pPidFilter));
                        dmx_dbg_print("[CTRL  ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl));
                        dmx_dbg_print("[MATCH0]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Match[0]));
                        dmx_dbg_print("[MATCH1]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Match[1]));
                        dmx_dbg_print("[MATCH2]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Match[2]));
                        dmx_dbg_print("[MATCH3]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Match[3]));
                        dmx_dbg_print("[MASK0 ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Mask[0]));
                        dmx_dbg_print("[MASK1 ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Mask[1]));
                        dmx_dbg_print("[MASK2 ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Mask[2]));
                        dmx_dbg_print("[MASK3 ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->Mask[3]));
                        dmx_dbg_print("[START ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->BufStart));
                        dmx_dbg_print("[END   ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->BufEnd));
                        dmx_dbg_print("[READ  ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->BufRead));
                        dmx_dbg_print("[WRITE ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->BufWrite));
                        dmx_dbg_print("[CURENT]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->BufCur));
                        dmx_dbg_print("[REMAIN]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->RmnReqCnt));
                        dmx_dbg_print("[CRC32 ]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->CRC32));
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
                        dmx_dbg_print("[NMASK0]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->_x50[0]));
                        dmx_dbg_print("[NMASK1]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->_x50[1]));
                        dmx_dbg_print("[NMASK2]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->_x50[2]));
                        dmx_dbg_print("[NMASK3]\t0x%08X\n\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->_x50[3]));
#else
                        dmx_dbg_print("[NMASK0]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->NMatch[0]));
                        dmx_dbg_print("[NMASK1]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->NMatch[1]));
                        dmx_dbg_print("[NMASK2]\t0x%08X\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->NMatch[2]));
                        dmx_dbg_print("[NMASK3]\t0x%08X\n\n", (unsigned int)HAL_REG32_IndR((REG32 *)&pSecFilter->NMatch[3]));
#endif                        
                    }
                }
            }
            else // not specified, show all filters
            {
                REG_PidFlt *pPidFlt;
                for(i = 0; i < u32MaxFlt; i++)
                {
                    PID_FLT_P(pPidFlt, i);
                    dmx_dbg_print("[%03d][0x%06x]\t0x%08X ", (int)i, (unsigned int)((MS_U32)pPidFlt >> 1), 
                        (unsigned int)HAL_REG32_IndR((REG32 *)pPidFlt));
                        
                    MApi_DMX_Info(i, &info, &type, FALSE);
                    
                    if(type != 0xFF)
                    {
                        if(type == DMX_FILTER_TYPE_SECTION ||
                           type == DMX_FILTER_TYPE_PES     ||
                           type == DMX_FILTER_TYPE_PACKET  ||
                           type == DMX_FILTER_TYPE_TELETEXT||
                           type == DMX_FILTER_TYPE_SECTION_VER)
                        {
//                            MS_U32 u32Time = MsOS_GetSystemTime();
//                            MS_U32 u32Active = (stDmxDbgInfo[i].u32TimeLastDisable == u32Time)?
//                                    stDmxDbgInfo[i].u32TimeActive : 
//                                    stDmxDbgInfo[i].u32TimeActive + (u32Time - stDmxDbgInfo[i].u32TimeLastEnable);
                                    
                            dmx_dbg_print("[%s] SecMode[0x%02X] Life[%04d s] Intr(r/o/c)[%06d/%02d/%02d] CB(r/o)[%06d/%02d]\n", 
                            stDmxType[type].str, 
                            (unsigned int)info.Info.SectInfo.SectMode,  
                            (unsigned int)(((MsOS_GetSystemTime() - stDmxDbgInfo[i].u32TimeCreate)/1000)), 
                            (unsigned int)stDmxDbgInfo[i].u32Int_rdy,
                            (unsigned int)stDmxDbgInfo[i].u32Int_ovf,
                            (unsigned int)stDmxDbgInfo[i].u32Int_crc,
                            (unsigned int)stDmxDbgInfo[i].u32CB_rdy,
                            (unsigned int)stDmxDbgInfo[i].u32CB_ovf);
                        }
                        else
                        {
                            dmx_dbg_print("[%s]\n", stDmxType[type].str);
                        }
                    }
                    else
                    {
                        dmx_dbg_print("\n");
                    }
                    
                }
            }
            dmx_dbg_print("====== DMX filters End ======\n");
        }
        else if(u32FltOp == 2) // close filter
        {
            if(u32FltId == 0xFFFFFFFF && u32Pid == 0xFFFFFFFF)
            {
                dmx_dbg_print("wrong parameter [unknown DMX filter index/PID to close]\n");
                bWrongParam = TRUE;
                break;
            }
            else
            {
                if(u32FltId < u32MaxFlt)
                {
                    if(DMX_FILTER_STATUS_ERROR != MApi_DMX_Close(u32FltId))
                        dmx_dbg_print("close DMX filter %d\n", (int)u32FltId);
                    else
                        dmx_dbg_print("close DMX filter %d failed\n", (int)u32FltId);
                }
                
                if(u32Pid != 0xFFFFFFFF)
                {
                    REG_PidFlt *pPidFlt;
                    for(i = 0; i < u32MaxFlt; i++)
                    {
                        PID_FLT_P(pPidFlt, i);
                        if(u32Pid == (HAL_REG32_IndR((REG32 *)pPidFlt) & DMX_PID_NULL))
                        {
                            if(DMX_FILTER_STATUS_ERROR != MApi_DMX_Close(i))
                                dmx_dbg_print("close DMX filter %d\n", (int)i);
                            else
                                dmx_dbg_print("close DMX filter %d failed\n", (int)i);
                        }
                    }
                }
            }
        }
        else if(u32FltOp == 3) // open filter
        {
            MS_U8 u8FltOpen = 0;
            MS_U32 u32FltType = 0;
            DMX_Flt_info info;
            memset(&info, 0x0, sizeof(DMX_Flt_info));
            if((u8FltType == 0) || (u32Pid == 0xFFFFFFFF) || 
                ((u8FltType == 's' || u8FltType == 'p') && 
                   (u32Addr == 0 || u32Size == 0)))
            {
                dmx_dbg_print("wrong parameter type[%c] PID[0x%x] addr[0x%x] size[0x%x]\n",
                (unsigned int)u8FltType, 
                (unsigned int)u32Pid,
                (unsigned int)u32Addr, 
                (unsigned int)u32Size);
                
                bWrongParam = TRUE;
                break;
            }
            
            switch(u8FltType)
            {
                case 'v':
                    u32FltType = DMX_FILTER_TYPE_VIDEO;
                    break;
                case 'a':
                    u32FltType = DMX_FILTER_TYPE_AUDIO;
                    break;
                case 's':
                    u32FltType = DMX_FILTER_TYPE_SECTION;
                    info.Info.SectInfo.SectBufAddr = (MS_PHYADDR)u32Addr;
                    info.Info.SectInfo.SectBufSize = (MS_U32)u32Size;
                    info.Info.SectInfo.SectMode    = (DMX_SECT_MODE)(DMX_SECT_MODE_CONTI | DMX_SECT_MODE_CRCCHK);
                    info.Info.SectInfo.Event       = (DMX_EVENT)(DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_SEC_CRCERROR | DMX_EVENT_CB_SELF);
                    info.Info.SectInfo.pNotify     = _DMX_DbgCb;
                    break;
                case 'p':
                    u32FltType = DMX_FILTER_TYPE_PACKET;
                    info.Info.SectInfo.SectBufAddr = (MS_PHYADDR)u32Addr;
                    info.Info.SectInfo.SectBufSize = (MS_U32)u32Size;
                    info.Info.SectInfo.SectMode    = (DMX_SECT_MODE)(DMX_SECT_MODE_CONTI | DMX_SECT_MODE_CRCCHK);
                    info.Info.SectInfo.Event       = (DMX_EVENT)(DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF);
                    info.Info.SectInfo.pNotify     = _DMX_DbgCb;
                    break;
                default:
                    dmx_dbg_print("Wrong parameter [DMX filter type %c]\n", (char)u8FltType);
                    bWrongParam = TRUE;
                    break;                
            }
            
            if(DMX_FILTER_STATUS_OK == MApi_DMX_Open((DMX_FILTER_TYPE)u32FltType, &u8FltOpen))
            {
                dmx_dbg_print("Open DMX filter %d\n", (int)u8FltOpen);
            }
            else
            {
                dmx_dbg_print("Open DMX filter failed\n");
                break;
            }
            
            MApi_DMX_Pid(u8FltOpen, (MS_U16*)&u32Pid, TRUE);
            
            MApi_DMX_Info(u8FltOpen, &info, NULL, TRUE);
            
            MApi_DMX_Start(u8FltOpen);
        }
    }while(0);
    
    if(bWrongParam)
    {
        dmx_dbg_print("%s\n", _stCmd[DMX_DBG_CMD_FLT].cmd_help);
    }
        
    return 0;    
}

static int _DMX_DBG_Misc(char* param1)
{
    MS_U32 u32Bank = 0xFFFFFFFF;
    MS_U32 u32Addr = 0xFFFFFFFF;
    MS_U32 u32Size = 0xFFFFFFFF;
    MS_U32 u32Ofset = 0xFFFFFFFF;
    MS_U32 u32Value = 0xFFFFFFFF;
    
    MS_U32 u32Addr1 = 0;
    MS_U32 u32Addr2 = 0;
    MS_BOOL bWrongParam = FALSE, bFound, bFile;
    char* pch;
    char path[128];
    int i;
    FILE* pfile = NULL;
    
    if(strlen(param1) > 128)
    {
        dmx_dbg_print("[%s][%d] buffer overflow \n", __FUNCTION__, __LINE__);
        return 0;
    }
    
    do
    {
        GET_PARAM32(param1, DMX_DBG_MISC_OPT_ADDR, bFound, u32Addr);

        GET_PARAM32(param1, DMX_DBG_MISC_OPT_SIZE, bFound, u32Size);

        GET_PARAM32(param1, DMX_DBG_MISC_OPT_BANK, bFound, u32Bank);

        GET_PARAM32(param1, DMX_DBG_MISC_OPT_OFSET, bFound, u32Ofset);

        GET_PARAM32(param1, DMX_DBG_MISC_OPT_VALUE, bFound, u32Value);
        
        GET_PARAM_STR(param1, DMX_DBG_MISC_OPT_FILE, bFile, path);
/*
        if(bFound)
        {
            pfile = fopen(path, "wb");
            if(pfile == NULL)
            {
                dmx_dbg_print("[%s] open file %s failed\n", __FUNCTION__, path);
                break;
            }
        }
*/
        pch = strtok(param1, " ,");
        if(pch[0] == 'd')
        {
            DMX_FLOW_INPUT input;
            MS_BOOL bInv, bExt, bPara;   
            if(DMX_FILTER_STATUS_OK == MApi_DMX_Get_FlowInput_Status(DMX_FLOW_PLAYBACK, &input, &bInv, &bExt, &bPara))
            {
                dmx_dbg_print("[TS Playback]\tInput[%s]\tInver[%d]\tExt Sync[%d]\tParallel[%d]\n", 
                _stDmxInput[input].str, 
                (int)bInv, (int)bExt, (int)bPara);
            }
            else
            {
                dmx_dbg_print("[TS Playback] can't get configuration\n");
            }
                
            if(DMX_FILTER_STATUS_OK == MApi_DMX_Get_FlowInput_Status(DMX_FLOW_PVR, &input, &bInv, &bExt, &bPara))
            {
                dmx_dbg_print("[TS PVR]\tInput[%s]\tInver[%d]\tExt Sync[%d]\tParallel[%d]\n", 
                _stDmxInput[input].str, 
                (int)bInv, (int)bExt, (int)bPara);
            }
            else
            {
                dmx_dbg_print("[TS PVR] can't get configuration\n");
            }
                
        }
        else if(pch[0] == 'c')
        {
            // @ TODO
        }
        else if(pch[0] == 'b')
        {
            typedef struct {
                MS_U16 L;
                MS_U16 dummy;
                MS_U16 H;
                MS_U16 dummy1;
            } MSTAR_REG;
            
            volatile MSTAR_REG* reg;
            
            if(u32Bank > 0x2000)
            {
                dmx_dbg_print("wrong parameter [Bank address 0x%x]\n", (unsigned int)u32Bank);
                bWrongParam = TRUE;
                break;
            }
                        
            MDrv_MMIO_GetBASE(&u32Addr, &u32Size, MS_MODULE_PM);
            u32Addr += (u32Bank << 9);
        
            
            reg = (volatile MSTAR_REG*)u32Addr;
                
            if(u32Ofset != 0xFFFFFFFF)
            {
                reg += (u32Ofset >> 1);
                if(u32Ofset & 0x1)
                {
                    reg->H = u32Value & 0xFFFF;
                }
                else
                {
                    reg->L = u32Value & 0xFFFF;
                }
                
                dmx_dbg_print("\twrite 0x%x to bank 0x%x, offset 0x%x\n", 
                (unsigned int)u32Value, (unsigned int)u32Bank, (unsigned int)u32Ofset);                
            }

            dmx_dbg_print("\t[register dump 0x%x]\n", (unsigned int)u32Bank);
                
            ULOGD("DMX", "\t0/8  1/9  2/A  3/B  4/C  5/D  6/E  7/F\n");
            for(i = 0; i < 64; i++)
            {
                if(i%4 == 0)
                {
                    ULOGD("DMX", "\n%02X\t", (int)i*2);
                }
                    
                ULOGD("DMX", "%04X %04X ", reg->L, reg->H);
                reg++;
            }
            ULOGD("DMX", "\n");
            
        }
        else if(pch[0] == 'm')
        {
            if(u32Addr == 0xFFFFFFFF || u32Size == 0xFFFFFFFF)
            {
                dmx_dbg_print("wrong parameter address[0x%x], size[0x%x]\n", 
                                (unsigned int)u32Addr, (unsigned int)u32Size);
                bWrongParam = TRUE;
                break;
            }
            else
            {
                dmx_dbg_print("\t[memory dump 0x%x ~ 0x%x(physical)]\n", (unsigned int)u32Addr, (unsigned int)(u32Addr + u32Size));
                u32Addr1    = (MS_U32)MS_PA2KSEG1(u32Addr);
                u32Addr2    = (MS_U32)MS_PA2KSEG1(u32Addr + u32Size);
                if((u32Addr1 == NULL) || (u32Addr2 == NULL))
                {
                    dmx_dbg_print("address can't convert to virtual PA[0x%x - 0x%x] size[0x%x]\n", 
                                    (unsigned int)u32Addr, 
                                    (unsigned int)(u32Addr + u32Size),
                                    (unsigned int)u32Size);
                    bWrongParam = TRUE;
                    break;
                }
                
                if(bFile)
                {
                    pfile = fopen(path, "wb");
                    if(pfile == NULL)
                    {
                        dmx_dbg_print("[%s] open file %s failed\n", __FUNCTION__, path);
                        break;
                    }                    
                }    

                if(pfile)
                {
                    if(u32Size != fwrite((char*)u32Addr1, 1, u32Size, pfile))
                    {
                        dmx_dbg_print("write file error, length %d\n", (int)u32Size);
                        fclose(pfile);
                        break;
                    }
                    else
                    {
                        dmx_dbg_print("write %d bytes to file %s succeed\n", (int)u32Size, path);
                    }
                    fclose(pfile);
                }
                else
                {
                    for(u32Addr = u32Addr1; u32Addr < u32Addr2; u32Addr++)
                    {
                        if(u32Addr % 16 ==0)
                            ULOGD("DMX", "\n0x%08X : ", (unsigned int)MS_VA2PA((MS_U32)u32Addr));
                        
                        ULOGD("DMX", "%02X ", *(volatile unsigned char*)u32Addr);
                    }
                }
                ULOGD("DMX", "\n");
            }
        }
        else
        {
            dmx_dbg_print("\n\nwrong parameter [unknown operation %c]\n\n", (char)pch[0]);
            bWrongParam = TRUE;
            break;
        }        
    }while(0);
    
    if(bWrongParam)
    {
        dmx_dbg_print("%s\n", _stCmd[DMX_DBG_CMD_MISC].cmd_help);
    }    
    return 0;
}

static void _DMX_DBG_CmdHandle(char* cmd, MS_U32 size)
{
    
    int i=0;
    char* pP0;

    char *pch = strtok(cmd, " ,");
    if(pch != NULL)
    {
        while(_stCmd[i].pfun != NULL)
        {
            //dmx_dbg_print("%s", _stCmd[i].cmd_str);
            if(strcmp(_stCmd[i].cmd_str, pch) == 0)
            {
                pP0 = (char*)strtok(NULL, "\n");
                
                _stCmd[i].pfun(pP0);
                return;
            }
            i++;
        }
        
        i=0;
        dmx_dbg_print("===================== DMX debug menu =====================\n");
        while(_stCmd[i].pfun != NULL)
        {
            dmx_dbg_print("%s", _stCmd[i].cmd_help);
            i++;
        }
        dmx_dbg_print("=================== DMX debug menu end ===================\n");
        
    }
}

static void _DMX_DBG_Task(MS_U32 argc, MS_U32* argv)
{
#define STRING_LENGTH 256    
    char cmd[STRING_LENGTH];
    char time[STRING_LENGTH];
    char *pCmd, *pTime;
    int CmdSize, TimeSize;

    do
    {
        MsOS_DelayTask(u32DmxDbgTaskDelay);
        if(pfDmxDbg == NULL)
        {
            //dmx_dbg_print("[%s] no dbg file exist\n", __FUNCTION__);
            //return;
        }
        else
        {
        #if 0
            char* ps = gets(cmd);
            dmx_dbg_print("%s\n", ps);
            _DMX_DBG_CmdHandle(ps, STRING_LENGTH);
           
        #else
            
            memset(cmd, 0x0, STRING_LENGTH);
            cmd[STRING_LENGTH-2] = '\r';
            cmd[STRING_LENGTH-1] = '\n';
            
            if(fgets(cmd, STRING_LENGTH, pfDmxDbg))
            {
                dmx_dbg_print("%s\n", cmd);
                _DMX_DBG_CmdHandle(cmd, STRING_LENGTH);
            }
            
            if(ferror(pfDmxDbg))
            {
                dmx_dbg_print("[%s] error reading DMX command file\n", __FUNCTION__);
            }
        #endif
        }   
        
// ========= debug information ============================================================
       
        memset(cmd, 0x0, STRING_LENGTH);
        memset(time, 0x0, STRING_LENGTH);
        pCmd = cmd;
        pTime = time;
        CmdSize = TimeSize = STRING_LENGTH;
        
        if(u32DbgLogFlag & DMX_DBG_LOG_AV_OVF)
        {
            // no function call yet
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_FI_READ)
        {
            MS_U32 u32Read;
            MApi_DMX_Filein_GetReadAddr(&u32Read);
            dmx_dbg_print("File in read [0x%x]\n", (unsigned int)u32Read);            
        }

        if(u32DbgLogFlag & DMX_DBG_LOG_PVR_WRITE)
        {
            MS_U32 u32Write;
            MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_EGN0, &u32Write);
            dmx_dbg_print("PVR write [0x%x\n]\n", (unsigned int)u32Write);            
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_AVFIFO)
        {
            MS_U32 u32StatusA;
            MS_U32 u32StatusV;
            MS_U32 u32StatusA2;
            MS_U32 u32StatusV3D;
            
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO, &u32StatusV);
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_AUDIO, &u32StatusA);
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_AUDIO2, &u32StatusA2);
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO3D, &u32StatusV3D);
            dmx_dbg_print("FIFO Status V[%02d] A[%02d] A2[%02d] V3D[%02d]\n", 
            (int)u32StatusV, (int)u32StatusA, (int)u32StatusA2, (int)u32StatusV3D);            
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_AV_DISCONTI)
        {
            MS_U32 u32DisContiV, u32DisContiA;
            DMX_DisContiCnt_info info;
            info.TspCmd = DMX_DEBUG_CMD_NONE;
            info.TspSrc = DMX_DEBUG_SRC_TS0;
            info.TspFifo = DMX_DEBUG_FIFO_VIDEO;
            MApi_DMX_Get_DisContiCnt(&info, &u32DisContiV);
            info.TspFifo = DMX_DEBUG_FIFO_AUDIO;
            MApi_DMX_Get_DisContiCnt(&info, &u32DisContiA);
            
            snprintf(cmd, STRING_LENGTH, "TS0 DisContiCC [V(%02d) A(%02d)]\t", (int)u32DisContiV, (int)u32DisContiA);
            //dmx_dbg_print("TS0 discontinuity video[%d] audio[%d]\n", (int)u32DisContiV, (int)u32DisContiA);
            pCmd = cmd + strlen(cmd);
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_AV_DROP)
        {
            MS_U32 u32DropV, u32DropA;
            DMX_DropPktCnt_info info;
            info.TspCmd = DMX_DEBUG_CMD_NONE;
            info.TspSrc = DMX_DEBUG_SRC_TS0;
            info.TspFifo = DMX_DEBUG_FIFO_VIDEO;
            MApi_DMX_Get_DropPktCnt(&info, &u32DropV);
            info.TspFifo = DMX_DEBUG_FIFO_AUDIO;
            MApi_DMX_Get_DropPktCnt(&info, &u32DropA);
            //dmx_dbg_print("TS0 video drop[%d], audio drop[%d]\n", (int)u32DropV, (int)u32DropA);
            snprintf(pCmd, STRING_LENGTH - (pCmd - cmd), "Drop [V(%02d) A(%02d)]\t", (int)u32DropV, (int)u32DropA);
            pCmd = cmd + strlen(cmd);
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_LOCK)
        {
            MS_U32 u32LockCnt = 0;
            
            DMX_LockPktCnt_info info;
            info.TspCmd = DMX_DEBUG_CMD_NONE;
            info.TspTsif = DMX_DEBUG_TSIF_TS0;
            MApi_DMX_Get_LockPktCnt(&info, &u32LockCnt);
            //dmx_dbg_print("TS0 Lock packet cnt[%d]\n", (int)u32LockCnt);
            snprintf(pCmd, STRING_LENGTH - (pCmd - cmd), "Lock [%02d]\t", (int)u32LockCnt);
            pCmd = cmd + strlen(cmd);
        }

        if(u32DbgLogFlag & (DMX_DBG_LOG_PCR | DMX_DBG_LOG_STC))
        {
            MS_U32 u32Tmp = MsOS_GetSystemTime();
            snprintf(time, STRING_LENGTH, "%sTime [0x%08X, diff(0x%08X)]\t", time, (int)u32Tmp, (int)(u32Tmp - u32PrevTime));    
            pTime = time + strlen(time);
            u32PrevTime = u32Tmp;
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_PCR)
        {
            MS_U32 u32Pcr, u32Pcr32;
            MApi_DMX_Pcr_Get(&u32Pcr32, &u32Pcr);
            //dmx_dbg_print("PCR[0x%X]\n", (unsigned int)u32Pcr);
            snprintf(pTime, STRING_LENGTH - (pTime - time), "PCR [0x%08X, diff(0x%08X)]\t", (int)u32Pcr, (int)(u32Pcr - u32PrevPCR));
            pTime = time + strlen(time);
            u32PrevPCR = u32Pcr;
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_STC)
        {
            MS_U32 u32Stc, u32Stc32;
            MApi_DMX_Stc_Get(&u32Stc32, &u32Stc);
            //dmx_dbg_print("STC[0x%X]\n", (unsigned int)u32Stc);
            snprintf(pTime, STRING_LENGTH, "STC [0x%08X, diff(0x%08X)]\t", (int)u32Stc, (int)(u32Stc - u32PrevSTC));
            pTime = time + strlen(time);
            u32PrevSTC = u32Stc;
        }
        
        if(u32DbgLogFlag & DMX_DBG_LOG_AV_PKT)       
        {
            DMX_AVPktCnt_info info;
            MS_U32 u32CntA = 0;
            MS_U32 u32CntV = 0;
            
            info.TspCmd = DMX_DEBUG_CMD_NONE;
            info.TspFifo = DMX_DEBUG_FIFO_VIDEO;
            info.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_0;
            
            MApi_DMX_Get_AVPktCnt(&info, &u32CntV);
            info.TspFifo = DMX_DEBUG_FIFO_AUDIO;
            MApi_DMX_Get_AVPktCnt(&info, &u32CntA);
            snprintf(pCmd, STRING_LENGTH - (pCmd - cmd), "AV Pkt[V(%02d) A(%02d)]\t", (int)u32CntV, (int)u32CntA);
            pCmd = cmd + strlen(cmd);
        }

        if(u32DbgLogFlag & DMX_DBG_LOG_AEON_ALIVE)
        {
            if(MApi_DMX_ChkAlive() == DMX_FILTER_STATUS_ERROR)
                dmx_dbg_print("DMX AEON dead\n");
            else
                dmx_dbg_print("DMX AEON alive\n");
        }        

        if(u32DbgLogFlag & DMX_DBG_LOG_INTR)
        {
            MS_U32 u32IntrCnt = 0;
            MApi_DMX_Get_Intr_Count(&u32IntrCnt);
            dmx_dbg_print("[DMX INTR]\tCounter[%d]\n", (int)u32IntrCnt);
        }        


        if(cmd[0])
            dmx_dbg_print("%s\n", cmd);
            
        if(time[0])            
            dmx_dbg_print("%s\n", time);

// ========= debug information ============================================================

    }while(1);    
}

static void _DMX_DBG_Pvr_Task(MS_U32 argc, MS_U32* argv)
{
    MS_U32 u32WriteSiz = 0;
    u32TimeStartPvr = MsOS_GetSystemTime();
    ULOGD("DMX", "u32TimeStartPvr = %d\n", (int)u32TimeStartPvr);
    u32SizeRec = 0;
    do
    {
        MsOS_DelayTask(100);
        
        if(pfPVR != NULL)
        {
            if(bMonitorAV == TRUE)
            {
                MS_U16 u16VPid, u16APid;
                _DMX_DBG_GetPID(DMX_FILTER_TYPE_VIDEO, &u16VPid);
                _DMX_DBG_GetPID(DMX_FILTER_TYPE_AUDIO, &u16APid);     
                
                if((u16VPid != u16PvrPidVid) || (u16APid != u16PvrPidAud))
                {
                    dmx_dbg_print("Change PID to Video(0x%x), Audio(0x%x)\n", 
                    (unsigned int)u16VPid, 
                    (unsigned int)u16APid);

                    MApi_DMX_Pvr_Eng_Pid_Close(DMX_PVR_EGN0, u8PvrIdxVid);
                    MApi_DMX_Pvr_Eng_Pid_Close(DMX_PVR_EGN0, u8PvrIdxAud);
                    if(DMX_FILTER_STATUS_OK == MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_EGN0, (MS_U32)u16VPid, &u8PvrIdxVid))
                    {
                        ULOGD("DMX", "[%s], open PVR filter %d for pid 0x%x\n", __FUNCTION__, (int)u8PvrIdxVid, (unsigned int)u16VPid);
                    }                        

                    if(DMX_FILTER_STATUS_OK == MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_EGN0, (MS_U32)u16APid, &u8PvrIdxAud))
                    {
                        ULOGD("DMX", "[%s], open PVR filter %d for pid 0x%x\n", __FUNCTION__, (int)u8PvrIdxAud, (unsigned int)u16APid);
                    }                        

                    u16PvrPidVid = u16VPid;
                    u16PvrPidAud = u16APid;
                }
            }
            MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_EGN0, &u32PhyWirte);
            //ULOGD("DMX", "PVR write(phy) = 0x%08x\n", (unsigned int)u32PhyWirte);
            
            u32VirWrite = (MS_U32)MS_PA2KSEG1(u32PhyWirte);
            if(u32VirWrite == 0)
            {
                dmx_dbg_print("[%s] ERROR : can't convert physical 0x%x to virtual\n", __FUNCTION__, (unsigned int)u32PhyWirte);
            }
            //ULOGD("DMX", "PVR write = 0x%x\n", (unsigned int)u32VirWrite);
            //ULOGD("DMX", "PVR read  = 0x%x\n", (unsigned int)u32VirRead);
            //ULOGD("DMX", "u32SizeRec = %d\n", (unsigned int)(u32SizeRec << 3));
            //ULOGD("DMX", "time = %d\n", (unsigned int)(((u32Time - u32TimeStart)/1000)));
            //if(((u32Time - u32TimeStart)/1000) != 0)
            //    ULOGD("DMX", "BitRate[%08d bps]\n", (int)((u32SizeRec << 3) / ((u32Time - u32TimeStart)/1000)));
                
            if(u32VirWrite > u32VirRead)
            {
                u32WriteSiz = u32VirWrite - u32VirRead;
                if(u32WriteSiz != fwrite((char*)u32VirRead, 1, u32WriteSiz, pfPVR))
                {
                    dmx_dbg_print("[%s] ERROR : write %d bytes to file failed\n", __FUNCTION__, (int)u32WriteSiz);
                    fclose(pfPVR);
                    pfPVR = NULL;
                }
                u32SizeRec += u32WriteSiz;
            }
            else if(u32VirWrite < u32VirRead)
            {
                u32WriteSiz = u32VirEnd - u32VirRead;
                if(u32WriteSiz != fwrite((char*)u32VirRead, 1, u32WriteSiz, pfPVR))
                {
                    dmx_dbg_print("[%s] ERROR : write %d bytes to file failed\n", __FUNCTION__, (int)u32WriteSiz);
                    fclose(pfPVR);
                    pfPVR = NULL;
                }
                u32SizeRec += u32WriteSiz;
    
                u32WriteSiz = u32VirWrite - u32VirStart;
                if(u32WriteSiz != fwrite((char*)u32VirStart, 1, u32WriteSiz, pfPVR))
                {
                    dmx_dbg_print("[%s] ERROR : write %d bytes to file failed\n", __FUNCTION__, (int)u32WriteSiz);
                    fclose(pfPVR);
                    pfPVR = NULL;
                }
                u32SizeRec += u32WriteSiz;
            }

            u32VirRead = u32VirWrite;
            
        }
        else
        {
            dmx_dbg_print("pfPVR = NULL\n");
            return;
        }        
    }while(1);
}
//-------------------------------------------------------------------------------------------------
// Record BitStream from DMX driver
// param path                \b IN: file path to write record data 
// param source              \b IN: source of the record
// param bRecordAll          \b IN: true to record all 
// return DMX_FILTER_STATUS_OK - Success
// return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL _DMX_DBG_Record_Start(const char path[], 
                                            MS_U32* u32Pid, 
                                            MS_U32 u32NumPids, 
                                            MS_PHYADDR pBuf,
                                            MS_U32 u32Size,
                                            MS_BOOL bCA, 
                                            MS_BOOL bRecordAll)
{
    DMX_FLOW_INPUT dmxInput = DMX_FLOW_PLAYBACK;
    MS_BOOL bClkInv = FALSE;
    MS_BOOL bExtSync = FALSE;
    MS_BOOL bParallel = FALSE;
    DMX_Pvr_info pvrInfo;
    bPvrCA = bCA;
    int idx = 0;
    if((pBuf == NULL) || (u32Size == 0) || (pBuf & 0xf) || (u32Size & 0xf))
    {
        dmx_dbg_print("%s, buffer[0x%x], size[0x%x] illegal !!!\n", __FUNCTION__, (unsigned int)pBuf, (unsigned int)u32Size);
        return FALSE;
    }
    
    if((!bRecordAll) && (u32NumPids == 0))
    {
        dmx_dbg_print("%s, nothing to record !!!\n", __FUNCTION__);
        return FALSE;
    }


    s32DmxDbgPvrTaskId = MsOS_CreateTask((TaskEntry)_DMX_DBG_Pvr_Task, 
                                      (MS_U32)NULL, 
                                      E_TASK_PRI_MEDIUM, 
                                      TRUE, 
                                      _u8PvrStack, 
                                      DMX_DBG_STACK_SIZE, 
                                      DMX_DBG_PVR_TASK_NAME);
    
    if(s32DmxDbgPvrTaskId == -1)
    {
        dmx_dbg_print("[%s] Create Task failed\n", __FUNCTION__);
        return FALSE;
    }
    
    pfPVR = fopen(path, "wb");
    if(pfPVR == NULL)
    {
        dmx_dbg_print("[%s] ERROR, Open file %s failed\n", __FUNCTION__, path);
        return FALSE;
    }    
                                      
    MApi_DMX_Get_FlowInput_Status(DMX_FLOW_PLAYBACK, &dmxInput, &bClkInv, &bExtSync, &bParallel);
    
    if(bCA)
        MApi_DMX_FlowSet(DMX_FLOW_PVRCA, dmxInput, bClkInv, bExtSync, bParallel);
    else
        MApi_DMX_FlowSet(DMX_FLOW_PVR, dmxInput, bClkInv, bExtSync, bParallel);
    
    #define PVR_ALIGNMENT (188 * 16)
    u32Size = ( u32Size / PVR_ALIGNMENT ) * PVR_ALIGNMENT;
        
    pvrInfo.pPvrBuf0        = pBuf;
    pvrInfo.PvrBufSize0     = u32Size / 2;
    pvrInfo.pPvrBuf1        = pBuf + (u32Size / 2);
    pvrInfo.PvrBufSize1     = u32Size / 2;

    MApi_DMX_Pvr_Eng_Open(DMX_PVR_EGN0, &pvrInfo);

    u32VirRead  = (MS_U32)MS_PA2KSEG1((MS_U32)pBuf);
    u32VirStart = (MS_U32)MS_PA2KSEG1((MS_U32)pBuf);
    u32VirEnd   = (u32VirStart + u32Size); // the last address of MIU block can't be converted to virtual address
    
    /*
    ULOGD("DMX", "pPvrBuf0    = 0x%08x(0x%08x)\n", (unsigned int)pvrInfo.pPvrBuf0, (unsigned int)MS_PA2KSEG1((MS_U32)pvrInfo.pPvrBuf0));
    ULOGD("DMX", "PvrBufSize0 = 0x%08x\n", (unsigned int)pvrInfo.PvrBufSize0);
    ULOGD("DMX", "pPvrBuf1    = 0x%08x\n", (unsigned int)pvrInfo.pPvrBuf1);
    ULOGD("DMX", "PvrBufSize1 = 0x%08x\n", (unsigned int)pvrInfo.PvrBufSize1);
    ULOGD("DMX", "Read  = 0x%x\n", (unsigned int)u32VirRead);
    ULOGD("DMX", "Start = 0x%x\n", (unsigned int)u32VirStart);
    ULOGD("DMX", "End   = 0x%x\n", (unsigned int)u32VirEnd);
    */
    
    if(bRecordAll)
    {
        MApi_DMX_Pvr_Eng_Start(DMX_PVR_EGN0, TRUE);
    }
    else
    {
        for(idx = 0; idx < u32NumPids; idx++)
        {
            if(bCA)
            {
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
                if(DMX_FILTER_STATUS_OK == MApi_DMX_PvrCA_Eng_Pid_Open(DMX_PVR_EGN0, u32Pid[idx], &u8PvrIdx[idx], FALSE))
                {
                    ULOGD("DMX", "[%s], open PVR CA filter %d for pid 0x%x\n", __FUNCTION__, (int)u8PvrIdx[idx], (unsigned int)u32Pid[idx]);
                }
                else
                {
                    dmx_dbg_print("[%s], open PVR filter failed\n", __FUNCTION__);
                }
#endif
            }
            else
            {
                if(DMX_FILTER_STATUS_OK == MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_EGN0, u32Pid[idx], &u8PvrIdx[idx]))
                {
                    ULOGD("DMX", "[%s], open PVR filter %d for pid 0x%x\n", __FUNCTION__, (int)u8PvrIdx[idx], (unsigned int)u32Pid[idx]);
                }
                else
                {
                    dmx_dbg_print("[%s], open PVR filter failed\n", __FUNCTION__);
                }
            }
        }
        
        MApi_DMX_Pvr_Eng_Start(DMX_PVR_EGN0, FALSE);
    }        
    return TRUE;
}

MS_BOOL _DMX_DBG_Record_Stop(void)
{
    if(bPvrCA)
        MApi_DMX_PvrCA_Eng_Stop(DMX_PVR_EGN0);
        
    if(pfPVR != NULL)
    {
        MS_U32 u32Time = MsOS_GetSystemTime();
        
        dmx_dbg_print("\tRecord %d bytes\n", (int)u32SizeRec);
        dmx_dbg_print("\tRecord %d s\n", (int)((u32Time - u32TimeStartPvr)/1000));

        if(((u32Time - u32TimeStartPvr)/1000) != 0)
        {
            dmx_dbg_print("\tBitRate[%08d bps]\n", (int)((u32SizeRec << 3) / ((u32Time - u32TimeStartPvr)/1000)));
        }

       fclose(pfPVR);
       pfPVR = NULL;
    }

    if(s32DmxDbgPvrTaskId > 0)
    {
        MsOS_DeleteTask(s32DmxDbgPvrTaskId);
        s32DmxDbgPvrTaskId = -1;
    }
    u32TimeStartPvr = 0;
    bMonitorAV = FALSE;
    return TRUE;
}

void _DMX_Debug(char path[])
{
    char filename[128];
    char* pch = path;

    if(pch == NULL)
    {
        pch = getenv(DMX_DBG_FILE_PATH_ENV);
        dmx_dbg_print("DMX debug file path = %s\n", pch);
        if(pch == NULL)
            return;
    }

    sprintf(filename, "%s/%s", pch, DMX_DBG_FILE_NAME);
    dmx_dbg_print("path = %s\n", filename);
    pfDmxDbg = fopen(filename, "w+");
    //pfDmxDbg = fopen(filename, "a+"); // append 
    if(pfDmxDbg == NULL)
    {
        dmx_dbg_print("[%s] ERROR, Open file %s failed\n", __FUNCTION__, filename);
    }
    
    s32DmxDbgTaskId = MsOS_CreateTask((TaskEntry)_DMX_DBG_Task, 
                                      (MS_U32)NULL, 
                                      E_TASK_PRI_MEDIUM, 
                                      TRUE, 
                                      _u8DbgStack, 
                                      DMX_DBG_STACK_SIZE, 
                                      DMX_DBG_TASK_NAME);
    
    if(s32DmxDbgTaskId == -1)
    {
        dmx_dbg_print("[%s] Create Task failed\n", __FUNCTION__);
        return;
    }
    
    // disable WDT
//    dmx_dbg_print("[%s] stop watch dog\n", __FUNCTION__);
//    MDrv_WDT_Stop(0);
    
    s32DmxDbgMutex = MsOS_CreateMutex(E_MSOS_FIFO, "DMX DEBUG", MSOS_PROCESS_SHARED);
    
    memset(stDmxDbgInfo, 0x0, sizeof(DMX_FLT_DBG_INFO) * TSP_SECFLT_NUM);
}

#endif // DMX_DEBUG
