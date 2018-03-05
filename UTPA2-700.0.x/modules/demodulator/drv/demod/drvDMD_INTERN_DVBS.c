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
/// file    drvDMD_INTERN_DVBS.c
/// @brief  DVBS Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "drvDMD_INTERN_DVBS.h"
#include "drvDMD_INTERN_DVBS_v2.h"
//#include <string.h>
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvSYS.h"
#include "drvDMD_VD_MBX.h"
#include "halDMD_INTERN_DVBS.h"
#include "halDMD_INTERN_common.h"
#include "../../include/drvSAR.h"  // for Utopia2
#include "utopia.h"   
#include "utopia_dapi.h"
#include "ULog.h"
#include "../../include/drvDMD_INTERN_DVBS_v2.h"  // for Utopia2
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DVBS2FEC_REG_BASE            0x3D00 
//#define MS_DEBUG 1
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_S16      _s16CurrentCFO;
extern MS_U16      _u16ChannelInfoArray[2][1000];
extern MS_U16      _u16ChannelInfoIndex;
extern MS_U16      _u16LockedCenterFreq;
extern MS_U16      _u16LockedSymbolRate;
extern MS_U8       modulation_order;
extern DMD_DVBS_Info sDMD_DVBS_Info;
extern MS_U8 u8DemodLockFlag;
extern MS_U8        g_dvbs_lock;
extern MS_BOOL     _bDemodType;
extern DMD_DVBS_DbgLv _u8DMD_DVBS_DbgLevel;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/*
static MS_U16             _u16SignalLevel[185][2]=
{//AV2028 SR=22M, 2/3 CN=5.9
    {32100,    920},{32200,    915},{32350,    910},{32390,    905},{32480,    900},{32550,    895},{32620,    890},{32680,    885},{32750,    880},{32830,    875},
    {32930,    870},{33010,    865},{33100,    860},{33200,    855},{33310,    850},{33410,    845},{33520,    840},{33640,    835},{33770,    830},{33900,    825},
    {34030,    820},{34150,    815},{34290,    810},{34390,    805},{34490,    800},{34580,    795},{34700,    790},{34800,    785},{34880,    780},{34940,    775},
    {35030,    770},{35130,    765},{35180,    760},{35260,    755},{35310,    750},{35340,    745},{35380,    740},{35400,    735},{35450,    730},{35550,    725},
    {35620,    720},{35700,    715},{35800,    710},{35890,    705},{36000,    700},{36120,    695},{36180,    690},{36280,    685},{36400,    680},{36570,    675},
    {36730,    670},{36910,    665},{37060,    660},{37100,    655},{37260,    650},{37340,    645},{37410,    640},{37580,    635},{37670,    630},{37700,    625},
    {37750,    620},{37800,    615},{37860,    610},{37980,    605},{38050,    600},{38170,    595},{38370,    590},{38540,    585},{38710,    580},{38870,    575},
    {39020,    570},{39070,    565},{39100,    560},{39180,    555},{39280,    550},{39460,    545},{39510,    540},{39600,    535},{39620,    530},{39680,    525},
    {39720,    520},{39830,    515},{39880,    510},{39930,    505},{39960,    500},{40000,    495},{40200,    490},{40360,    485},{40540,    480},{40730,    475},
    {40880,    470},{41020,    465},{41150,    460},{41280,    455},{41410,    450},{41520,    445},{41620,    440},{41730,    435},{41840,    430},{41930,    425},
    {42010,    420},{42100,    415},{42180,    410},{42260,    405},{42350,    400},{42440,    395},{42520,    390},{42580,    385},{42660,    380},{42730,    375},
    {42800,    370},{42870,    365},{42940,    360},{43000,    355},{43060,    350},{43130,    345},{43180,    340},{43250,    335},{43310,    330},{43370,    325},
    {43420,    320},{43460,    315},{43520,    310},{43570,    305},{43620,    300},{43660,    295},{43710,    290},{43750,    285},{43810,    280},{43860,    275},
    {43910,    270},{43940,    265},{43990,    260},{44020,    255},{44060,    250},{44110,    245},{44140,    240},{44190,    235},{44230,    230},{44270,    225},
    {44320,    220},{44370,    215},{44400,    210},{44450,    205},{44490,    200},{44530,    195},{44590,    190},{44630,    185},{44660,    180},{44720,    175},
    {44750,    170},{44790,    165},{44830,    160},{44880,    155},{44910,    150},{44960,    145},{45000,    140},{45030,    135},{45070,    130},{45100,    125},
    {45130,    120},{45160,    115},{45200,    110},{45240,    105},{45270,    100},{45300,     95},{45330,     90},{45360,     85},{45400,     80},{45430,     75},
    {45460,     70},{45490,     65},{45530,     60},{45560,     55},{45590,     50},{45630,     45},{45670,     40},{45690,     35},{45740,     30},{45760,     25},
    {45800,     20},{45830,     15},{45860,     10},{45880,      5},{45920,      0}
};
*/
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

static    void* ppDVBSInstant = NULL;
static MS_U32 u32DVBSopen = 0;
static MS_U8 u8DVBSUtopiaOpen = 0;   //for SetStillImagePara is earlier called than Init  

static DMD_DVBS_InitData   AgcSsi_Para;
//static float intern_dvb_c_qam_ref[] = {3.0, 0.0, 0.0, 0.0, 0.0, 80.0}; //16q,32q,64q,128q,256q, and others

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX
#if 1
static float _LogApproxTableX[80] =
{ 1.00, 1.30, 1.69, 2.20, 2.86, 3.71, 4.83, 6.27, 8.16, 10.60, 13.79,
  17.92, 23.30, 30.29, 39.37, 51.19, 66.54, 86.50, 112.46, 146.19,
  190.05, 247.06, 321.18, 417.54, 542.80, 705.64, 917.33, 1192.53,
  1550.29, 2015.38, 2620.00, 3405.99, 4427.79, 5756.13, 7482.97,
  9727.86, 12646.22, 16440.08, 21372.11, 27783.74, 36118.86,
  46954.52, 61040.88, 79353.15, 103159.09, 134106.82, 174338.86,
  226640.52, 294632.68, 383022.48, 497929.22, 647307.99, 841500.39, 1093950.50,
  1422135.65, 1848776.35, 2403409.25, 3124432.03, 4061761.64, 5280290.13,
  6864377.17, 8923690.32, 11600797.42, 15081036.65, 19605347.64, 25486951.94,
  33133037.52, 43072948.77, 55994833.40, 72793283.42, 94631268.45,
  123020648.99, 159926843.68, 207904896.79, 270276365.82, 351359275.57,
  456767058.24, 593797175.72, 771936328.43, 1003517226.96
};

static float _LogApproxTableY[80] =
{ 0.00, 0.11, 0.23, 0.34, 0.46, 0.57, 0.68, 0.80, 0.91, 1.03, 1.14, 1.25,
  1.37, 1.48, 1.60, 1.71, 1.82, 1.94, 2.05, 2.16, 2.28, 2.39, 2.51, 2.62,
  2.73, 2.85, 2.96, 3.08, 3.19, 3.30, 3.42, 3.53, 3.65, 3.76, 3.87, 3.99,
  4.10, 4.22, 4.33, 4.44, 4.56, 4.67, 4.79, 4.90, 5.01, 5.13, 5.24, 5.36,
  5.47, 5.58, 5.70, 5.81, 5.93, 6.04, 6.15, 6.27, 6.04, 6.15, 6.27, 6.38,
  6.49, 6.61, 6.72, 6.84, 6.95, 7.06, 7.18, 7.29, 7.41, 7.52, 7.63, 7.75,
  7.86, 7.98, 8.09, 8.20, 8.32, 8.43, 8.55, 8.66
};

static float Log10Approx(float flt_x)
{
    MS_U8  indx = 0;

    do {
        if (flt_x < _LogApproxTableX[indx])
            break;
        indx++;
    }while (indx < 79);   //stop at indx = 80

    return _LogApproxTableY[indx];
}
#else
static float Log10Approx(float flt_x)
{
    MS_U32       u32_temp = 1;
    MS_U8        indx = 0;

    do {
        u32_temp = u32_temp << 1;
        if (flt_x < (float)u32_temp)
            break;
    }while (++indx < 32);

    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (float)0.3 * indx;
}
#endif
#endif

#ifdef UFO_SUPPORT_KERNEL_FLOATING
#define MAX_INT 0x7FFFFFFF
#define Pow2_62 0x4000000000000000

MS_U32 ABS_32(MS_S32 input)
{
    MS_U32 result;

    if(input < 0)
    {
        result = (-1)*input;
    }
    else
        result = input;

    return result;
}

MS_U64 ABS_64(MS_S64 input)
{
    MS_U64 result;

    if(input < 0)
    {
        result = (-1)*input;
    }
    else
        result = input;

    return result;
}

MS_U8 Find_MSB(MS_S64 input)
{
    MS_S8 iter = -1;
    MS_U64 data = ABS_64(input);

    while(0 != data)
    {
        ++iter;
        data >>= 1;
    }
    
    if(iter >= 0)
    {
        return iter;
    }   

    return 0;
 }

void Normalize(MS_FLOAT_ST *input)
{
    MS_U8 LSB, sign_flag;
    
    MS_S8 EXP;
    MS_U32 data;

    if((*input).DATA == 0)
    {
        (*input).EXP = 0;
    }
    else
    {
        LSB = 0;

        if((*input).DATA < 0) // negative value
            sign_flag = 1;
        else
            sign_flag = 0;

        data = ABS_32((*input).DATA);
        EXP = (*input).EXP;

        if(EXP != 0)
        {
           while( (data & 0x01) == 0x00 )
           {
            ++LSB;
            data >>= 1;
           }

           EXP += LSB;

           (*input).DATA = data;
           (*input).EXP = EXP;

           if(sign_flag == 1)
           {
            (*input).DATA *= (-1);
           }
        }
    }
}

MS_FLOAT_ST MS_FLOAT_OP(MS_FLOAT_ST stRn,MS_FLOAT_ST stRd, OP_type eOpcode)
{
    MS_FLOAT_ST result;

    MS_S32 data1, data2;
    MS_U32 udata1, udata2;
    MS_S8 EXP1, EXP2;
    MS_S8 iter, MSB, MSB_temp;

    MS_S64 temp;

    Normalize(&stRn);
    Normalize(&stRd);

    data1 = stRn.DATA;
    data2 = stRd.DATA;

    udata1 = ABS_32(data1);
    udata2 = ABS_32(data2);

    EXP1 = stRn.EXP;
    EXP2 = stRd.EXP;

    switch(eOpcode)
    {
        case add:
        {
            if(EXP1 == EXP2)
            {
                temp = data1;
                temp += data2;

                if(temp > MAX_INT || temp < (-1)*MAX_INT)
                {
                    temp >>= 1;
                    result.DATA = temp;
                    result.EXP = (EXP1 + 1);
                }
                else
                {
                    result.DATA = (data1 + data2);
                    result.EXP = EXP1;
                }
            }
            else if(EXP1 > EXP2)
            {
                temp = data1;       

                MSB = Find_MSB(temp);  
                
                if( (MSB - EXP2) < 63)              
                {
                    for(iter = EXP1; iter > EXP2;--iter)
                    {
                        temp = (temp << 1);                    
                    }

                    temp += data2;

                    if(temp > MAX_INT || temp < (-1)*MAX_INT)
                    {
                        MSB = Find_MSB(temp);

                        temp >>= (MSB-30);
                        result.DATA = temp;
                        result.EXP = (EXP2 + (MSB-30));
                    }
                    else
                    {
                        result.DATA = temp;
                        result.EXP = EXP2;
                    }
                }
                else
                {
                    result.DATA = data1;
                    result.EXP = data1;
                }
            }
            else
            {
                return MS_FLOAT_OP(stRd, stRn, add);
            }
        }
        break;

        case minus:
        {
            stRd.DATA *= (-1);
            return MS_FLOAT_OP(stRn, stRd, add);
        }
        break;

        case multiply:
        {
            if(data1 == 0 || data2 == 0)
            {
                result.DATA = 0;
                result.EXP = 0;
            }                
            else
            {
                temp = data1;
                temp *= data2;

                if( (temp <= MAX_INT) && (temp >= (-1*MAX_INT) ) )
                {
                    result.DATA = data1 * data2;
                    result.EXP = EXP1 + EXP2;
                }
                else // overflow
                {
                    MSB = Find_MSB(temp);

                    temp = temp >> (MSB-30);

                    result.DATA = (MS_S32)temp;
                    result.EXP = EXP1 + EXP2 + (MSB-30);
                }       
            }
        }
        break;

        case divide:
        {
            if(data1 != 0 && data2 != 0)
            {
                if(udata1 < udata2)
                {
                    temp = Pow2_62;
                    temp = temp / data2 * data1;                    

                    MSB = Find_MSB(temp);

                    if(MSB > 30)
                    {                         
                        temp >>= (MSB-30);
                        result.DATA = temp;
                        result.EXP = EXP1 - EXP2 + (MSB-30) - 62;                        
                    }
                    else
                    {
                        result.DATA = temp;
                        result.EXP = EXP1 - EXP2 - 62;
                    }                   
                }
                else if(udata1 == udata2)
                {
                    result.DATA = data1 / data2;
                    result.EXP = EXP1 - EXP2;
                }
                else // udata1 > udata2
                {
                    MSB = Find_MSB(data1);     
                    MSB_temp = Find_MSB(data2);

                    EXP2 -= ((MSB-MSB_temp) + 1);

                    temp = Pow2_62;
                    temp = temp / ( ((MS_S64)data2) << ((MSB-MSB_temp) + 1)) * data1;                
                    
                    MSB = Find_MSB(temp);       

                    if(MSB > 30)
                    {                         
                        temp >>= (MSB-30);
                        result.DATA = temp;
                        result.EXP = EXP1 - EXP2 + (MSB-30) - 62;                        
                    }
                    else
                    {
                        result.DATA = temp;
                        result.EXP = EXP1 - EXP2 - 62;
                    }                        
                }                
            }
            else
            {
                result.DATA = 0;
                result.EXP = 0;
            }
        }
        break;

        default:
        break;
    }            

    Normalize(&result);

    return result;
}
#endif

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
MS_BOOL MDrv_DMD_DVBS_Reg_INT_CB(fpIntCallBack fpCBReg)
{
    DVBS_INT_MODE Drv_DVBS_INT_MODE;
    Drv_DVBS_INT_MODE.fpCB = fpCBReg;
    Drv_DVBS_INT_MODE.ret = FALSE;

    if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_INT_MODE,&Drv_DVBS_INT_MODE);
    }

    return Drv_DVBS_INT_MODE.ret;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_DVBS_Init(DMD_DVBS_InitData *pDMD_DVBS_InitData, MS_U32 u32InitDataLen)
{
    void* pAttribte = NULL;

    #ifdef MS_DEBUG
    ULOGD("DEMOD","******check driver layer DVBS init!!*******\n");
    #endif
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_Init\n"));

    DVBS_Init_PARAM Drv_DVBS_Init_PARAM;
    DMD_DVBS_InitData_Transform Init_Para_Temp;

    Init_Para_Temp.u8SarChannel=pDMD_DVBS_InitData->u8SarChannel;
    Init_Para_Temp.u8DMD_DVBS_DSPRegInitExt=pDMD_DVBS_InitData->u8DMD_DVBS_DSPRegInitExt;
    Init_Para_Temp.u8DMD_DVBS_DSPRegInitSize=pDMD_DVBS_InitData->u8DMD_DVBS_DSPRegInitSize;
    Init_Para_Temp.u8DMD_DVBS_InitExt=pDMD_DVBS_InitData->u8DMD_DVBS_InitExt;
    /*
    Init_Para_Temp.pTuner_RfagcSsi=pDMD_DVBS_InitData->pTuner_RfagcSsi;
    Init_Para_Temp.u16Tuner_RfagcSsi_Size=pDMD_DVBS_InitData->u16Tuner_RfagcSsi_Size;
    Init_Para_Temp.pTuner_IfagcSsi_LoRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_LoRef;
    Init_Para_Temp.u16Tuner_IfagcSsi_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_LoRef_Size;  
    Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    Init_Para_Temp.u16Tuner_IfagcSsi_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_HiRef_Size;
    Init_Para_Temp.pTuner_IfagcErr_LoRef=pDMD_DVBS_InitData->pTuner_IfagcErr_LoRef;
    Init_Para_Temp.u16Tuner_IfagcErr_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_LoRef_Size;
    Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    Init_Para_Temp.u16Tuner_IfagcErr_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_HiRef_Size;
    Init_Para_Temp.pSqiCnNordigP1=pDMD_DVBS_InitData->pSqiCnNordigP1;
    Init_Para_Temp.u16SqiCnNordigP1_Size=pDMD_DVBS_InitData->u16SqiCnNordigP1_Size;
    */
    
    AgcSsi_Para.pTuner_RfagcSsi=pDMD_DVBS_InitData->pTuner_RfagcSsi;
    AgcSsi_Para.u16Tuner_RfagcSsi_Size=pDMD_DVBS_InitData->u16Tuner_RfagcSsi_Size;
    AgcSsi_Para.pTuner_IfagcSsi_LoRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_LoRef;
    AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_LoRef_Size;  
    AgcSsi_Para.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    AgcSsi_Para.u16Tuner_IfagcSsi_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_HiRef_Size;
    AgcSsi_Para.pTuner_IfagcErr_LoRef=pDMD_DVBS_InitData->pTuner_IfagcErr_LoRef;
    AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_LoRef_Size;
    AgcSsi_Para.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    AgcSsi_Para.u16Tuner_IfagcErr_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_HiRef_Size;

    Drv_DVBS_Init_PARAM.u32InitDataLen=sizeof(Init_Para_Temp);
    Drv_DVBS_Init_PARAM.pDMD_DVBS_InitData=&Init_Para_Temp;
    Drv_DVBS_Init_PARAM.ret=false;    
     
     if(u8DVBSUtopiaOpen == 0)  // First time open
    { 
        if(UtopiaOpen(MODULE_DVBS/*|KERNEL_MODE*/ , &ppDVBSInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //kernel space
        //if(UtopiaOpen(MODULE_DVBS , &ppDVBSInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //user space
        {
            u32DVBSopen = 1;
            ULOGD("DEMOD","Leo check DVBS utopia open sucessful!!\n");
      //return_val=true;
           // ULOGD("DEMOD","\r\n ======== DVBS Open Successful %x =========", (WORD)u32DVBSopen);
        }
        else
        {
          //  ULOGD("DEMOD","\r\n ======== DVBS Open Fail %x =========", (WORD)u32DVBSopen);
       //return_val=false;
          ULOGD("DEMOD","DVBS utopia open fail!!\n");
          return false;
        }

        u8DVBSUtopiaOpen = 1;
    }
    
    if(u32DVBSopen==1)
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_Init,&Drv_DVBS_Init_PARAM);
    else
        return false;

    return Drv_DVBS_Init_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_Exit(void)
{
    DVBS_EXIT_PARAM_PARAM Drv_DVBS_EXIT_PARAM_PARAM;
    Drv_DVBS_EXIT_PARAM_PARAM.ret=false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_Exit\n"));

       if(u32DVBSopen==1)
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_Exit,&Drv_DVBS_EXIT_PARAM_PARAM);
      else
        return false;

    return Drv_DVBS_EXIT_PARAM_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_SetDbgLevel(DMD_DVBS_DbgLv u8DbgLevel)
{
    DVBS_SetDbgLevel_PARAM Drv_DVBS_SetDbgLevel_PARAM;
    Drv_DVBS_SetDbgLevel_PARAM.u8DbgLevel=u8DbgLevel;
    Drv_DVBS_SetDbgLevel_PARAM.ret=false;
  
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_SetDbgLevel\n"));
    if(u32DVBSopen==1)
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetDbgLevel,&Drv_DVBS_SetDbgLevel_PARAM);
    else
      return false;
      
    return Drv_DVBS_SetDbgLevel_PARAM.ret;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
const DMD_DVBS_Info* MDrv_DMD_DVBS_GetInfo(void)
{
  DVBS_GetInfo_PARAM Drv_DVBS_GetInfo_PARAM;
  Drv_DVBS_GetInfo_PARAM.ret_info=NULL;
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetInfo,&Drv_DVBS_GetInfo_PARAM);
  }
  else
  {
    return false;
  }


  return Drv_DVBS_GetInfo_PARAM.ret_info;

}
#endif

MS_BOOL MDrv_DMD_DVBS_GetLibVer(const MSIF_Version **ppVersion)
{
   DVBS_GetLibVer_PARAM Drv_DVBS_GetLibVer_PARAM;
   Drv_DVBS_GetLibVer_PARAM.ppVersion=ppVersion;
    Drv_DVBS_GetLibVer_PARAM.ret= false;


   if(u32DVBSopen==1)
   {
       UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetLibVer,&Drv_DVBS_GetLibVer_PARAM);
	    Drv_DVBS_GetLibVer_PARAM.ret= true;
   }
   else
   {
       return false;
   }
  
   return Drv_DVBS_GetLibVer_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_GetFWVer(MS_U16 *ver)
{
  DVBS_GetFWVer_PARAM Drv_DVBS_GetFWVer_PARAM;
  Drv_DVBS_GetFWVer_PARAM.ver=ver;
  Drv_DVBS_GetFWVer_PARAM.ret=false;
  
  if(u32DVBSopen==1)
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetFWVer,&Drv_DVBS_GetFWVer_PARAM);
  else
  {
    return false;
  }

       return Drv_DVBS_GetFWVer_PARAM.ret;

}

MS_BOOL MDrv_DMD_DVBS_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
#if defined(CHIP_K1)
  DVBS_GetDSPReg_PARAM Drv_DVBS_GetDSPReg_PARAM;
  Drv_DVBS_GetDSPReg_PARAM.u16Addr=u16Addr;
  Drv_DVBS_GetDSPReg_PARAM.pu8Data=pu8Data;
  
   if(u32DVBSopen==1)
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetDSPReg,&Drv_DVBS_GetDSPReg_PARAM);
  else
  {
    return false;
  }

  return Drv_DVBS_GetDSPReg_PARAM.ret;
#else
    ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBS_GetDSPReg\n");
  return FALSE;
#endif
}

MS_BOOL MDrv_DMD_DVBS_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data)
{
#if defined(CHIP_K1)
  DVBS_SetDSPReg_PARAM Drv_DVBS_SetDSPReg_PARAM;

  Drv_DVBS_SetDSPReg_PARAM.pu8Data=pu8Data;
  Drv_DVBS_SetDSPReg_PARAM.u16Addr=u16Addr;
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetDSPReg,&Drv_DVBS_SetDSPReg_PARAM);
  }
  else
  {
    return false;
  }

    return Drv_DVBS_SetDSPReg_PARAM.ret;
#else
    ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBS_SetDSPReg\n");
  return FALSE;
#endif
}


MS_BOOL MDrv_DMD_DVBS_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
  DVBS_GetReg_PARAM Drv_DVBS_GetReg_PARAM;
  Drv_DVBS_GetReg_PARAM.u16Addr=u16Addr;
  Drv_DVBS_GetReg_PARAM.pu8Data=pu8Data;
  Drv_DVBS_GetReg_PARAM.ret=false;

  if(u32DVBSopen==1)
    {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetReg,&Drv_DVBS_GetReg_PARAM);
   }
    else
    {
      return false;
    }

    return Drv_DVBS_GetReg_PARAM.ret;
  
}

MS_BOOL MDrv_DMD_DVBS_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
  DVBS_SetReg_PARAM Drv_DVBS_SetReg_PARAM;
  Drv_DVBS_SetReg_PARAM.u16Addr=u16Addr;
  Drv_DVBS_SetReg_PARAM.u8Data=u8Data;
  Drv_DVBS_SetReg_PARAM.ret=false; 

  if(u32DVBSopen==1)
    {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetReg,&Drv_DVBS_SetReg_PARAM);
   }
    else
    {
      return false;
    }

    return Drv_DVBS_SetReg_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_SetSerialControl(MS_BOOL bEnable)
{
      DVBS_SetSerialControl_PARAM Drv_DVBS_SetSerialControl_PARAM;
      Drv_DVBS_SetSerialControl_PARAM.bEnable=bEnable;
  Drv_DVBS_SetSerialControl_PARAM.ret=false;

  if(u32DVBSopen==1)
    {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetSerialControl,&Drv_DVBS_SetSerialControl_PARAM);
   }
    else
    {
      return false;
    }    

    
    return Drv_DVBS_SetSerialControl_PARAM.ret;
}

#ifdef UFO_DEMOD_BLINDSCAN_NEW_FLOW
#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_BlindScan_Config(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS)
{
    return MDrv_DMD_DVBS_BlindScan_Config_symbol_rate_list(u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, NULL, 0);
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_Config_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{
  MS_U32   u32CurrentSR;
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS BlindScan_Config_symbol_rate_list!!\n");
  #endif

  DVBS_BlindScan_Config_Symbol_rate_list_PARAM Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.u32SymbolRate=u32SymbolRate;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.eQamMode=eQamMode;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.u32IFFreq=u32IFFreq;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.bSpecInv=bSpecInv;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.bSerialTS=bSerialTS;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.pu16_symbol_rate_list=pu16_symbol_rate_list;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.u8_symbol_rate_list_num=u8_symbol_rate_list_num;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret=false;  
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_Config_symbol_rate_list,&Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM);
    g_dvbs_lock = 0;
    u8DemodLockFlag=0;
    u32CurrentSR = u32SymbolRate/1000;  //KHz
    Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, u32CurrentSR&0xff);
    Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, (u32CurrentSR>>8)&0xff);
    Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, (u32CurrentSR>>16)&0xff);
    ULOGD("DEMOD","drv_DMD_DVBS_BlindScan_Config u32SymbolRate = %d \n", u32CurrentSR);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret;
}
#endif
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_SetConfig(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS)
{
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS SetConfig!!\n");
  #endif
  
   return MDrv_DMD_DVBS_SetConfig_symbol_rate_list(u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, NULL, 0);
}

MS_BOOL MDrv_DMD_DVBS_SetConfig_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{ 
  MS_U32   u32CurrentSR;
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS SetConfig_symbol_rate_list!!\n");
  #endif

  DVBS_SetConfig_Symbol_rate_list_PARAM Drv_DVBS_SetConfig_Symbol_rate_list_PARAM;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.u16SymbolRate=u32SymbolRate;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.eQamMode=eQamMode;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.u32IFFreq=u32IFFreq;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.bSpecInv=bSpecInv;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.bSerialTS=bSerialTS;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.pu16_symbol_rate_list=pu16_symbol_rate_list;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.u8_symbol_rate_list_num=u8_symbol_rate_list_num;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret=false;  
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetConfig_symbol_rate_list,&Drv_DVBS_SetConfig_Symbol_rate_list_PARAM);
    g_dvbs_lock = 0;
    u8DemodLockFlag=0;
    u32CurrentSR = u32SymbolRate/1000;  //KHz
    Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, u32CurrentSR&0xff);
    Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, (u32CurrentSR>>8)&0xff);
    Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, (u32CurrentSR>>16)&0xff);
    ULOGD("DEMOD","drv_DMD_DVBS_SetConfig u32SymbolRate = %d \n", (int)u32CurrentSR);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret;
}
#endif
MS_BOOL MDrv_DMD_DVBS_SetActive(MS_BOOL bEnable)
{
  DVBS_SetActive_PARAM Drv_DVBS_SetActive_PARAM;
  Drv_DVBS_SetActive_PARAM.bEnable=bEnable;
  Drv_DVBS_SetActive_PARAM.ret=false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_SetActive\n"));

  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS SetActive!!\n");
  #endif

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetActive,&Drv_DVBS_SetActive_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_SetActive_PARAM.ret;
}
#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetLock(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus)
{
    return MDrv_DMD_DVBS_GetLockWithRFPower(eType, eLockStatus, 200.0f, -200.0f);
}
#endif

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
MS_BOOL MDrv_DMD_DVBS_GetParam(HAL_DEMOD_MS_SAT_CARRIER_PARAM *pParam)
{
    DVBS_GET_INFO Drv_DVBS_GET_INFO;        

    Drv_DVBS_GET_INFO.ret = FALSE;
    
    if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetParam, &Drv_DVBS_GET_INFO);
    }

    pParam->eCodeRate = Drv_DVBS_GET_INFO.eCodeRate;
    pParam->eRollOff = Drv_DVBS_GET_INFO.eRollOff;
    pParam->eConstellation = Drv_DVBS_GET_INFO.eConstellation;
    pParam->eDemodType = Drv_DVBS_GET_INFO.eDemodType;
    pParam->eFreqoff = Drv_DVBS_GET_INFO.eFreqoff;
    pParam->eIQ_Mode = Drv_DVBS_GET_INFO.eIQ_Mode;
    
#ifdef MS_DEBUG
    ULOGD("DEMOD","Code rate :%d\n",pParam->eCodeRate);
    ULOGD("DEMOD","Roll-off Factor :%d\n",pParam->eRollOff);
    ULOGD("DEMOD","Modulation mode :%d\n",pParam->eConstellation);
    ULOGD("DEMOD","Demod type :%d\n",pParam->eDemodType);
    ULOGD("DEMOD","CFO :%f\n",pParam->eFreqoff);
#endif

    return Drv_DVBS_GET_INFO.ret;       
}
#endif

#ifdef UFO_DEMOD_GET_AGC_INFO
MS_BOOL MDrv_DMD_DVBS_GetAGCInfo(MS_U8 u8dbg_mode, MS_U16 *pu16Data)
{
    DVBS_GET_AGC_INFO Drv_DVBS_GET_AGC_INFO;

    Drv_DVBS_GET_AGC_INFO.u8dbg_mode = u8dbg_mode;
    Drv_DVBS_GET_AGC_INFO.pu16Data = pu16Data;
    Drv_DVBS_GET_AGC_INFO.ret = false;

    if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetAGCInfo, &Drv_DVBS_GET_AGC_INFO);
    }
    
    return Drv_DVBS_GET_AGC_INFO.ret;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetLockWithRFPower(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm)
{
  MS_BOOL status = true;
  
  DMD_IFAGC_SSI   *ifagc_ssi;
  DMD_IFAGC_ERR   *ifagc_err;
  float   ch_power_rf=0.0f;
  float   ch_power_db=0.0f;
  float   ch_power_if=0.0f, ch_power_ifa = 0.0f, ch_power_ifb =0.0f;
  MS_U16  if_agc_val =0, if_agc_vala =0, if_agc_valb =0, if_agc_val_lsb =0, i;
  float   ch_power_takeover=0.0f;
  MS_U8   ssi_tbl_len = 0, err_tbl_len = 0;
  
  MS_U8 ifagc_reg;
  MS_U8 ifagc_reg_lsb;
  MS_U16 ifagc_err_reg;
    MS_U32      u32SymbolRate=0;
    //float       fSymbolRate;
    //MS_U8 ISSY_EN = 0;
    MS_U8 code_rate_idx = 0;
    MS_U8 pilot_flag = 0;
    MS_U8 fec_type_idx = 0;
    MS_U16 k_bch_array[2][42] =
    {
        {
            16008, 21408, 25728, 32208, 38688, 43040, 48408, 51648, 53840, 57472,
            58192, 14208, 18528, 28968, 32208, 34368, 35448, 35808, 37248, 37248,
            38688, 40128, 41208, 41568, 43008, 44448, 44808, 45888, 46608, 47328,
            47328, 48408, 50208, 50208, 55248,     0,     0,     0,     0,     0,
            0,     0,
        },
        {
            3072,  5232,  6312,  7032,  9552, 10632, 11712, 12432, 13152, 14232,
            0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
            0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
            0,     0,     0,     0,     0,  3792,  4152,  4872,  7392,  8472,
            9192, 11352
        }
    };
    MS_U16 n_ldpc_array[2] = {64800, 16200};
    
    MS_FLOAT pilot_term = 0;
    MS_FLOAT k_bch;    
    MS_FLOAT n_ldpc;
    MS_FLOAT ts_div_num_offset = 1.5;
    MS_FLOAT ts_div_num_margin_ratio = 1.03;
    MS_FLOAT ftmp;
    //MS_U32 u32Time_start,u32Time_end;
    MS_U32 u32temp=0;
    //MS_FLOAT pkt_interval;
    MS_U8 system_type_reg=0;
    MS_U8 code_rate_reg=0;
    MS_FLOAT fTSDivNum;
    MS_U8 u8Data;
 
  DVBS_GetLockWithRFPower Drv_DVBS_GetLockWithRFPower;

  Drv_DVBS_GetLockWithRFPower.eType=eType;
  Drv_DVBS_GetLockWithRFPower.eLockStatus=eLockStatus;
  //Drv_DVBS_GetLockWithRFPower.u32CurrRFPowerDbm=(MS_U32)(fCurrRFPowerDbm*10);
  //Drv_DVBS_GetLockWithRFPower.u32NoChannelRFPowerDbm=(MS_U32)(fNoChannelRFPowerDbm*10);
  Drv_DVBS_GetLockWithRFPower.ret=false;
  
  DVBS_GetIFAGC_PARAM Drv_DVBS_GetIFAGC_PARAM;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg=&ifagc_reg;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg_lsb=&ifagc_reg_lsb;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_err_reg=&ifagc_err_reg;
  Drv_DVBS_GetIFAGC_PARAM.ret=false;

  DVBS_GetTsDivNum_PARAM Drv_DVBS_GetTsDivNum_PARAM;
  Drv_DVBS_GetTsDivNum_PARAM.u32SymbolRate=&u32SymbolRate;
  Drv_DVBS_GetTsDivNum_PARAM.system_type_reg=&system_type_reg;
  Drv_DVBS_GetTsDivNum_PARAM.code_rate_idx=&code_rate_idx;
  Drv_DVBS_GetTsDivNum_PARAM.fec_type_idx=&fec_type_idx;
  Drv_DVBS_GetTsDivNum_PARAM.pilot_flag=&pilot_flag;
  Drv_DVBS_GetTsDivNum_PARAM.u32temp=&u32temp;
  Drv_DVBS_GetTsDivNum_PARAM.code_rate_reg=&code_rate_reg;
  Drv_DVBS_GetTsDivNum_PARAM.bRet=false;
  
  if(u32DVBSopen==1)
  {
  	if((AgcSsi_Para.pTuner_IfagcSsi_HiRef != NULL) && (AgcSsi_Para.pTuner_IfagcSsi_LoRef != NULL))
    {
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetIFAGC,&Drv_DVBS_GetIFAGC_PARAM);
        status &= Drv_DVBS_GetIFAGC_PARAM.ret;
    
        ch_power_rf = fCurrRFPowerDbm;
        if_agc_val = ifagc_reg;
        if_agc_val_lsb = ifagc_reg_lsb;
    
        ifagc_ssi = AgcSsi_Para.pTuner_IfagcSsi_LoRef;
        ssi_tbl_len = AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size;
        ifagc_err = AgcSsi_Para.pTuner_IfagcErr_LoRef;
        err_tbl_len = AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size;   
    
        ch_power_if=ifagc_ssi[0].power_db;
        if (if_agc_val >=ifagc_ssi[0].agc_val)
        {
                for(i = 1; i < ssi_tbl_len; i++)
                {
                    if (if_agc_val < ifagc_ssi[i].agc_val)
                    {
                        if_agc_valb = ifagc_ssi[i].agc_val;
                        ch_power_ifb = ifagc_ssi[i].power_db;
        
                        i--;
                        if_agc_vala = ifagc_ssi[i].agc_val;
                        ch_power_ifa=ifagc_ssi[i].power_db;
                        while ((i>1) && (if_agc_vala==ifagc_ssi[i-1].agc_val))
                        {
                            ch_power_ifa=ifagc_ssi[i-1].power_db;
                            i--;
                        }
                        ch_power_if = ch_power_ifa+(ch_power_ifb-ch_power_ifa)*(float)((if_agc_val-if_agc_vala)*256+if_agc_val_lsb)/((if_agc_valb-if_agc_vala)*256);
                        break;
                    }
                }
        }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if prev %f %x\n", ch_power_ifa, if_agc_vala);
            ULOGD("DEMOD","if next %f %x\n", ch_power_ifb, if_agc_valb);  
            #endif

            
            for(i = 0; i < ssi_tbl_len; i++)
            {
                if (ifagc_ssi[i].agc_val <= ifagc_ssi[i+1].agc_val)
                {
                    ch_power_takeover = ifagc_ssi[i+1].power_db;
                    break;
                }
            }

            #ifdef MS_DEBUG
            //ULOGD("DEMOD","ch_power_rf = %f\n", ch_power_rf);
            ULOGD("DEMOD","ch_power_if = %f\n", ch_power_if);
            ULOGD("DEMOD","ch_power_takeover = %f\n", ch_power_takeover);
            #endif

            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;

            if(ch_power_rf > (ch_power_takeover + 0.5))
            {
                ch_power_db = ch_power_rf;
            }
            else if(ch_power_if < (ch_power_takeover - 0.5))
            {
                ch_power_db = ch_power_if;
            }
            else
            {
                ch_power_db = (ch_power_if + ch_power_rf)/2;
            }
          
            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_if : ch_power_rf;
    
        if(if_agc_val == 0xff)
        {
            for(i = 0; i < err_tbl_len; i++)
            {
                    if ( ifagc_err_reg <= ifagc_err[i].agc_err )        // signed char comparison
                    {
                        ch_power_db += ifagc_err[i].attn_db;
                        break;
                    }
            }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if_agc_err = 0x%x\n", ifagc_err_reg);
            #endif
        }
    }
    else
    {
    	  #ifdef MS_DEBUG
        if (fCurrRFPowerDbm>=100.0) // unreasonable input value, get RF level from RFAGG
        {
            ULOGD("DEMOD","Error!! please add AGC table\n");
        }
        #endif
        ch_power_db = fCurrRFPowerDbm;
    }
    
    Drv_DVBS_GetLockWithRFPower.u32CurrRFPowerDbm=(MS_U32)(ch_power_db);
    Drv_DVBS_GetLockWithRFPower.u32NoChannelRFPowerDbm=(MS_U32)(fNoChannelRFPowerDbm);
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetLockWithRFPower,&Drv_DVBS_GetLockWithRFPower);	
    status &= Drv_DVBS_GetLockWithRFPower.ret;
    if(u8DemodLockFlag==1)
    {
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetTsDivNum, &Drv_DVBS_GetTsDivNum_PARAM);
        status &= Drv_DVBS_GetTsDivNum_PARAM.bRet;

        //fec_type_idx = *Drv_DVBS_GetTsDivNum_PARAM.fec_type_idx;
        //code_rate_idx = *Drv_DVBS_GetTsDivNum_PARAM.code_rate_idx;
        //pilot_flag = *Drv_DVBS_GetTsDivNum_PARAM.pilot_flag;
         
         if(!system_type_reg)//DVBS2
         {
                k_bch = k_bch_array[fec_type_idx][code_rate_idx];
                n_ldpc = n_ldpc_array[fec_type_idx];
                pilot_term = ((float) n_ldpc / modulation_order / 1440 * 36) * pilot_flag;

                //data rate
                ftmp = k_bch/((n_ldpc/modulation_order+90+pilot_term)/u32SymbolRate);
                
                if(sDMD_DVBS_Info.bSerialTS)//serial mode
                {
                    fTSDivNum = 288000.0 / (ftmp) / 2.0 - 1;
                    //fTSDivNum = fTSDivNum/2-1;// since  288/(2(fTSDivNum+1)) = 288/TS_RATE = A  ==> fTSDivNum = A/2-1
                }
                else//parallel mode
                {
                    fTSDivNum = 288000.0 / (ftmp/8.0) / 2.0 - 1;
                    //fTSDivNum = (fTSDivNum-1)/2;
                }
                
                fTSDivNum -= ts_div_num_offset;

                if(fTSDivNum > 0x1F)
                    fTSDivNum = 0x1F;
                else if(fTSDivNum < 0x01)
                    fTSDivNum = 0x01;

                //ULOGD("DEMOD", ">>>Modulation Order = 0x%x<<<\n", modulation_order);
                //ULOGD("DEMOD", ">>>FEC Type = 0x%x<<<\n", fec_type_idx);
                //ULOGD("DEMOD", ">>>CR Type = 0x%x<<<\n", code_rate_idx);
                //ULOGD("DEMOD", ">>>Pilot Type = 0x%x<<<\n", pilot_flag);
         }
         else                                            //S
         {
             code_rate_reg = *Drv_DVBS_GetTsDivNum_PARAM.code_rate_reg;
             switch (code_rate_reg)
             {
                case 0x00: //CR 1/2
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x01: //CR 2/3
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x02: //CR 3/4
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                      //fTSDivNum = fTSDivNum/2-1-5;
                    break;
                case 0x03: //CR 5/6
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x04: //CR 7/8
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                default:
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
             }
         } 
         
         //fTSDivNum = 0x01;
         u8Data = (MS_U8)fTSDivNum;        
         
	  #ifdef MS_DEBUG
         ULOGD("DEMOD", ">>>INTERN_DVBS_GetLock TsClkDivNum = 0x%x<<<\n", u8Data);
         #endif
         
         if (u8Data > 0x1F) 
             u8Data = 0x1F;

             HAL_DMD_RIU_WriteByte(0x103300, u8Data);
                        
             //Ts Output Enable
             HAL_DMD_RIU_WriteByte(0x101eaa,0x10);
    }
    	
  }
  else
  {
    return false;
  }

  #ifdef MS_DEBUG 
  //if(*(Drv_DVBS_GetLockWithRFPower_Transform.eLockStatus)==DMD_DVBS_LOCK)
  if(*(Drv_DVBS_GetLockWithRFPower.eLockStatus)==DMD_DVBS_LOCK)
  {
    ULOGD("DEMOD","check in drv layer DVBS demod locked!!\n");
  }
  else
  {
    ULOGD("DEMOD","check in drv layer DVBS demod unlock!!\n");
    return false;
  }
  #endif

  return status;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DVBS_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{ 
  MS_BOOL status = true;

  DMD_IFAGC_SSI   *ifagc_ssi;
  DMD_IFAGC_ERR   *ifagc_err;
  float   ch_power_rf=0.0f;
  float   ch_power_db=0.0f, ch_power_db_rel=0.0f;
  float   ch_power_if=0.0f, ch_power_ifa = 0.0f, ch_power_ifb =0.0f;
  MS_U16  if_agc_val =0, if_agc_vala =0, if_agc_valb =0, if_agc_val_lsb =0, i;
  float   ch_power_takeover=0.0f;
  MS_U8   ssi_tbl_len = 0, err_tbl_len = 0;
  
  MS_U8 ifagc_reg;
  MS_U8 ifagc_reg_lsb;
  MS_U16 ifagc_err_reg;

 // float   ch_power_db=0.0f, ch_power_db_rel=0.0f;
  DMD_DVBS_DEMOD_TYPE pDemodType;
  MS_U8  u8_DVBS2_CurrentCodeRateLocal;
  MS_U8  u8_DVBS2_CurrentConstellationLocal;
  //MS_U16 u16Data=0;
  //MS_U8 u8Index;
  //DMD_DVBS_MODULATION_TYPE Qam_mode;
 // MS_U16 SymbolRate;
 // float FreqOff;
  
  //DVBS_GetSignalStrengthWithRFPower_PARAM Drv_DVBS_GetSignalStrengthWithRFPower_PARAM;
  //Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.u16Strength=u16Strength;
  //Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.fRFPowerDbm=fRFPowerDbm;

  DVBS_GetIFAGC_PARAM Drv_DVBS_GetIFAGC_PARAM;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg=&ifagc_reg;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg_lsb=&ifagc_reg_lsb;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_err_reg=&ifagc_err_reg;
  Drv_DVBS_GetIFAGC_PARAM.ret=false;

/*
  DVBS_GetTunrSignalLevel_PWR_PARAM Drv_DVBS_GetTunrSignalLevel_PWR_PARAM;
  Drv_DVBS_GetTunrSignalLevel_PWR_PARAM.u16Data=&u16Data;
  Drv_DVBS_GetTunrSignalLevel_PWR_PARAM.bRet=false;
  */
  DVBS_GetSignalStrengthWithRFPower_PARAM Drv_DVBS_GetSignalStrengthWithRFPower_PARAM;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.fRFPowerDbm=(MS_U16)fRFPowerDbm;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.pDemodType=&pDemodType;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.u8_DVBS2_CurrentCodeRateLocal=&u8_DVBS2_CurrentCodeRateLocal;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.u8_DVBS2_CurrentConstellationLocal=&u8_DVBS2_CurrentConstellationLocal;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.ret=false;
  
  if(u32DVBSopen==1)
  {

    if((AgcSsi_Para.pTuner_IfagcSsi_HiRef != NULL) && (AgcSsi_Para.pTuner_IfagcSsi_LoRef != NULL))
    {
    	  UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetIFAGC,&Drv_DVBS_GetIFAGC_PARAM);
        status &= Drv_DVBS_GetIFAGC_PARAM.ret;
        
        ch_power_rf = fRFPowerDbm;
    
        if_agc_val = ifagc_reg;
        if_agc_val_lsb = ifagc_reg_lsb;
    
        ifagc_ssi = AgcSsi_Para.pTuner_IfagcSsi_LoRef;
        ssi_tbl_len = AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size;
        ifagc_err = AgcSsi_Para.pTuner_IfagcErr_LoRef;
        err_tbl_len = AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size;

        ch_power_if=ifagc_ssi[0].power_db;
        if (if_agc_val >=ifagc_ssi[0].agc_val)
        {
                for(i = 1; i < ssi_tbl_len; i++)
                {
                    if (if_agc_val < ifagc_ssi[i].agc_val)
                    {
                        if_agc_valb = ifagc_ssi[i].agc_val;
                        ch_power_ifb = ifagc_ssi[i].power_db;
        
                        i--;
                        if_agc_vala = ifagc_ssi[i].agc_val;
                        ch_power_ifa=ifagc_ssi[i].power_db;
                        while ((i>1) && (if_agc_vala==ifagc_ssi[i-1].agc_val))
                        {
                            ch_power_ifa=ifagc_ssi[i-1].power_db;
                            i--;
                        }
                        ch_power_if = ch_power_ifa+(ch_power_ifb-ch_power_ifa)*(float)((if_agc_val-if_agc_vala)*256+if_agc_val_lsb)/((if_agc_valb-if_agc_vala)*256);
                        break;
                    }
                }
        }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if prev %f %x\n", ch_power_ifa, if_agc_vala);
            ULOGD("DEMOD","if next %f %x\n", ch_power_ifb, if_agc_valb);  
            #endif
            
            for(i = 0; i < ssi_tbl_len; i++)
            {
                if (ifagc_ssi[i].agc_val <= ifagc_ssi[i+1].agc_val)
                {
                    ch_power_takeover = ifagc_ssi[i+1].power_db;
                    break;
                }
            }

            #ifdef MS_DEBUG
            ULOGD("DEMOD","ch_power_rf = %f\n", ch_power_rf);
            ULOGD("DEMOD","ch_power_if = %f\n", ch_power_if);
            ULOGD("DEMOD","ch_power_takeover = %f\n", ch_power_takeover);
            #endif

            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;

            if(ch_power_rf > (ch_power_takeover + 0.5))
            {
                ch_power_db = ch_power_rf;
            }
            else if(ch_power_if < (ch_power_takeover - 0.5))
            {
                ch_power_db = ch_power_if;
            }
            else
            {
                ch_power_db = (ch_power_if + ch_power_rf)/2;
            }
          
            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_if : ch_power_rf;
    
        if(if_agc_val == 0xff)
        {
            for(i = 0; i < err_tbl_len; i++)
            {
                    if ( ifagc_err_reg <= ifagc_err[i].agc_err )        // signed char comparison
                    {
                        ch_power_db += ifagc_err[i].attn_db;
                        break;
                    }
            }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if_agc_err = 0x%x\n", ifagc_err_reg);
           #endif
        }
    }
    else
    {
    	  #ifdef MS_DEBUG
        if (fRFPowerDbm>=100.0) // unreasonable input value, get RF level from RFAGG
        {
            ULOGD("DEMOD","Error!! please add AGC table\n");
        }
        #endif
        ch_power_db = fRFPowerDbm;
    }
    /*
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetTunrSignalLevel_PWR,&Drv_DVBS_GetTunrSignalLevel_PWR_PARAM);
        status &= Drv_DVBS_GetTunrSignalLevel_PWR_PARAM.bRet;
    if (u8DemodLockFlag == 0 || status == FALSE)//Demod unlock
    {
        ch_power_db = 0;
	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]INTERN_DVBS GetSignalStrength failed\n"));
    }
    else
    {
        for (u8Index=0; u8Index < (sizeof(_u16SignalLevel)/sizeof(_u16SignalLevel[0])); u8Index++)
        {
            if ((65535 - u16Data) <= _u16SignalLevel[u8Index][0])
            {
                if (u8Index >=1)
                {
                    ch_power_db = (float)(_u16SignalLevel[u8Index][1])+((float)(_u16SignalLevel[u8Index][0] - (65535 - u16Data)) / (float)(_u16SignalLevel[u8Index][0] - _u16SignalLevel[u8Index-1][0]))*(float)(_u16SignalLevel[u8Index-1][1] - _u16SignalLevel[u8Index][1]);
                }
                else
                {
                    ch_power_db = _u16SignalLevel[u8Index][1];
                }
                break;
            }
        }
//----------
        if (ch_power_db >= 350)
            ch_power_db = ch_power_db - 35;
        else if ((ch_power_db < 350) && (ch_power_db >= 250))
            ch_power_db = ch_power_db - 25;
        else
            ch_power_db = ch_power_db - 5;

        if (ch_power_db < 0)
            ch_power_db = 0;
        if (ch_power_db > 920)
            ch_power_db = 920;

        ch_power_db = (-1.0)*(ch_power_db/10.0);
        //DBG_INTERN_DVBS(printf("INTERN_DVBS GetSignalStrength %f\n", ch_power_db));
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]INTERN_DVBS GetSignalStrength %f\n", ch_power_db));
    }
    */
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetSignalStrengthWithRFPower,&Drv_DVBS_GetSignalStrengthWithRFPower_PARAM);
    status &= Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.ret;

    #if 1
    if((MS_U8)pDemodType == (MS_U8)DMD_SAT_DVBS)//S
    {
        float fDVBS_SSI_Pref[]=
        {
            //0,       1,       2,       3,       4
            -78.9,   -77.15,  -76.14,  -75.19,  -74.57,//QPSK
        };
        //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE + 0x84, &u8Data);
        //u8_DVBS2_CurrentCodeRateLocal = (u8Data & 0x07);
        ch_power_db_rel = ch_power_db - fDVBS_SSI_Pref[u8_DVBS2_CurrentCodeRateLocal];
    }
    else
    {
        #if 0
        float fDVBS2_SSI_Pref[][11]=
        {
            //  0,    1,       2,       3,       4,       5,       6,       7,       8,        9,       10
            //1/4,    1/3,     2/5,     1/2,     3/5,     2/3,     3/4,     4/5,     5/6,      8/9,     9/10
            {-85.17, -84.08,  -83.15,  -81.86,  -80.63,  -79.77,  -78.84,  -78.19,  -77.69,   -76.68,  -76.46}, //QPSK
            {   0.0,    0.0,     0.0,     0.0,  -77.36,  -76.24,  -74.95,     0.0,  -73.52,   -72.18,  -71.84}  //8PSK
        };
        #endif
        /*
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD7, &u8Data);
        u8_DVBS2_CurrentCodeRateLocal = (u8Data & 0x3C)>>2;

        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD7, &u8Data);
       status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD6, &u8Data2);

        if(((u8Data & 0x03)==0x01) && ((u8Data2 & 0x80)==0x00))
        {
            _u8_DVBS2_CurrentConstellationLocal = DMD_DVBS_QPSK;
        }
        else if (((u8Data & 0x03)==0x01) && ((u8Data2 & 0x80)==0x80))
        {
            _u8_DVBS2_CurrentConstellationLocal = DMD_DVBS_8PSK;//8PSK
        }
        */
        ch_power_db_rel = ch_power_db - (-80);//fDVBS2_SSI_Pref[u8_DVBS2_CurrentConstellationLocal][u8_DVBS2_CurrentCodeRateLocal];
    }
    #endif

    if(ch_power_db_rel <= -15.0f)
    {
        *u16Strength= 0;
    }
    else if (ch_power_db_rel <= 0.0f)
    {
        *u16Strength = (MS_U16)(2.0f/3 * (ch_power_db_rel+15.0f));
    }
    else if (ch_power_db_rel <= 20.0f)
    {
        *u16Strength = (MS_U16)(4.0f * ch_power_db_rel + 10.0f);
    }
    else if (ch_power_db_rel <= 35.0f)
    {
        *u16Strength = (MS_U16)(2.0f/3 * (ch_power_db_rel-20.0f) + 90.0);
    }
    else
    {
        *u16Strength = 100;
    }

	//ULOGD("DEMOD","Signal Strength(SSI) = %d \n", (int)*u16Strength);
	sDMD_DVBS_Info.u16Strength=*u16Strength;
  }

  else
  {
    return false;
  }

  return status;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalQuality(MS_U16 *u16Quality)
{
    return MDrv_DMD_DVBS_GetSignalQualityWithRFPower(u16Quality, 200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    MS_BOOL bRet;
    float       fber = 0.0;
    //float       log_ber;
    MS_BOOL     status = TRUE;
    float       f_snr = 0.0, ber_sqi = 0.0, cn_rel = 0.0;
    //MS_U8       u8Data =0;
    DMD_DVBS_CODE_RATE_TYPE       _u8_DVBS2_CurrentCodeRateLocal ;
    MS_U16     bchpkt_error,BCH_Eflag2_Window;
    //fRFPowerDbm = fRFPowerDbm;
    float snr_poly =0.0;
    float Fixed_SNR =0.0;
    double eFlag_PER=0.0;
	

  if(u32DVBSopen==1)
  {
    if (u8DemodLockFlag == 1)	
   // if (TRUE == INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK, 200.0f, -200.0f, 0))
    {
        if(_bDemodType)  //S2
        {

           MDrv_DMD_DVBS_GetSNR(&f_snr);
           snr_poly = 0.005261367463671*pow(f_snr, 3)-0.116517828301214*pow(f_snr, 2)+0.744836970505452*pow(f_snr, 1)-0.86727609780167;
           Fixed_SNR = f_snr + snr_poly;

           if (Fixed_SNR < 17.0)
              Fixed_SNR = Fixed_SNR;
           else if ((Fixed_SNR < 20.0) && (Fixed_SNR >= 17.0))
              Fixed_SNR = Fixed_SNR - 0.8;
           else if ((Fixed_SNR < 22.5) && (Fixed_SNR >= 20.0))
              Fixed_SNR = Fixed_SNR - 2.0;
           else if ((Fixed_SNR < 27.0) && (Fixed_SNR >= 22.5))
              Fixed_SNR = Fixed_SNR - 3.0;
           else if ((Fixed_SNR < 29.0) && (Fixed_SNR >= 27.0))
              Fixed_SNR = Fixed_SNR - 3.5;
           else if (Fixed_SNR >= 29.0)
              Fixed_SNR = Fixed_SNR - 3.0;


           if (Fixed_SNR < 1.0)
              Fixed_SNR = 1.0;
           if (Fixed_SNR > 30.0)
              Fixed_SNR = 30.0;

            //BCH EFLAG2_Window,  window size 0x2000
            BCH_Eflag2_Window=0x2000;
            MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE + 0x25*2 + 1, (BCH_Eflag2_Window>>8));
            MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE + 0x25*2 , (BCH_Eflag2_Window&0xff));
            MDrv_DMD_DVBS_GetPacketErr(&bchpkt_error);
            eFlag_PER = (float)(bchpkt_error)/(float)(BCH_Eflag2_Window);
            if(eFlag_PER>0)
              fber = 0.089267531133002*pow(eFlag_PER, 2) + 0.019640560289510*eFlag_PER + 0.0000001;
            else
              fber = 0;

#ifdef MSOS_TYPE_LINUX
                    //log_ber = ( - 1) *log10f(1 / fber);
                    if (fber > 1.0E-1)
                        ber_sqi = (log10f(1.0f/fber))*20.0f + 8.0f;
                    else if(fber > 8.5E-7)
                        ber_sqi = (log10f(1.0f/fber))*20.0f - 30.0f;
                    else
                        ber_sqi = 100.0;
#else
                    //log_ber = ( - 1) *Log10Approx(1 / fber);
                    if (fber > 1.0E-1)
                        ber_sqi = (Log10Approx(1.0f/fber))*20.0f + 8.0f;
                    else if(fber > 8.5E-7)
                        ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 30.0f;
                    else
                        ber_sqi = 100.0;

#endif

            *u16Quality = Fixed_SNR/30*ber_sqi;
            //DBG_INTERN_DVBS(printf(" Fixed_SNR %f\n",Fixed_SNR));
            //DBG_INTERN_DVBS(printf(" BCH_Eflag2_Window %d\n",BCH_Eflag2_Window));
            //DBG_INTERN_DVBS(printf(" eFlag_PER [%f]\n fber [%8.3e]\n ber_sqi [%f]\n",eFlag_PER,fber,ber_sqi));
        }
        else  //S
        {
            if (MDrv_DMD_DVBS_GetPostViterbiBer(&fber) == FALSE)//ViterbiBer
            {         
                ULOGD("DEMOD","GetPostViterbiBer Fail! \n");
                bRet= FALSE;
            }
            //_fPostBer=fber;


            if (status==FALSE)
            {
                ULOGD("DEMOD","Maserati_GetSignalQuality GetPostViterbiBer Fail! \n");
                bRet= FALSE;
            }
            float fDVBS_SQI_CNref[]=
            {   //0,    1,    2,    3,    4
                4.2,   5.9,  6,  6.9,  7.5,//QPSK
            };

           DVBS_GetCurrentDemodCodeRate_PARAM Drv_DVBS_GetCurrentDemodCodeRate_PARAM;
           Drv_DVBS_GetCurrentDemodCodeRate_PARAM.pCodeRate=&_u8_DVBS2_CurrentCodeRateLocal;
           Drv_DVBS_GetCurrentDemodCodeRate_PARAM.bRet=false;
	   UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetCurrentDemodCodeRate,&Drv_DVBS_GetCurrentDemodCodeRate_PARAM);
#if 0
#ifdef MSOS_TYPE_LINUX
            log_ber = ( - 1.0f) *log10f(1.0f / fber);           //BY modify
#else
            log_ber = ( - 1.0f) *Log10Approx(1.0f / fber);      //BY modify
#endif
            DBG_INTERN_DVBS(printf("\nLog(BER) = %f\n",log_ber));
#endif
            if (fber > 2.5E-2)
                ber_sqi = 0.0;
            else if(fber > 8.5E-7)
#ifdef MSOS_TYPE_LINUX
                ber_sqi = (log10f(1.0f/fber))*20.0f - 32.0f; //40.0f;
#else
                ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 32.0f;//40.0f;
#endif
            else
                ber_sqi = 100.0;

            status &= MDrv_DMD_DVBS_GetSNR(&f_snr);
            ULOGD("DEMOD","drv_DVBS_GetSNR = %d \n", (int)f_snr );
            cn_rel = f_snr - fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal];
			
            ULOGD("DEMOD","fber = %f\n", fber );
            ULOGD("DEMOD","f_snr = %f\n", f_snr );
            ULOGD("DEMOD","cn_nordig_s1 = %f\n", fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal] );
	     ULOGD("DEMOD","cn_rel = %f\n", cn_rel );
            ULOGD("DEMOD","ber_sqi = %f\n", ber_sqi );

            if (cn_rel < -7.0f)
            {
                *u16Quality = 0;
            }
            else if (cn_rel < 3.0)
            {
                *u16Quality = (MS_U16)(ber_sqi*((cn_rel - 3.0)/10.0 + 1.0));
            }
            else
            {
                *u16Quality = (MS_U16)ber_sqi;
            }


        }
            //INTERN_DVBS_GetTunrSignalLevel_PWR();//For Debug.
            //ULOGD("DEMOD","Signal Quility(SQI) = %d\n", *u16Quality );
            bRet= TRUE;
    }
    else
    {
        *u16Quality = 0;
    }
     bRet= TRUE;
     sDMD_DVBS_Info.u16Quality=*u16Quality;
  }
  else
  {
    return false;
  }
  return bRet;
}
#endif

#ifdef CHIP_KAISER
MS_BOOL MDrv_DMD_DVBS_ActiveDmdSwitch(MS_U8 demod_no)
{
  DVBS_ActiveDmdSwitch_PARAM Drv_DVBS_ActiveDmdSwitch_PARAM;
  Drv_DVBS_ActiveDmdSwitch_PARAM.demod_no=demod_no;

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_ActiveDmdSwitch,&Drv_DVBS_ActiveDmdSwitch_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_ActiveDmdSwitch_PARAM.ret;
 }
#else
MS_BOOL MDrv_DMD_DVBS_ActiveDmdSwitch(MS_U8 demod_no)
{
  ULOGD("DEMOD","Doesn't support DVBS_ActiveDmdSwitch function!!!\n");
  return false;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSNR(float *fSNR)
{
    MS_U32 u32NDA_SNR_A =0;
    MS_U32 u32NDA_SNR_AB =0;
    //float NDA_SNR_A =0.0;
    //float NDA_SNR_AB =0.0;
    float NDA_SNR =0.0;
    //double NDA_SNR_LINEAR=0.0;

  DVBS_GetSNR_PARAM Drv_DVBS_GetSNR_PARAM;
  Drv_DVBS_GetSNR_PARAM.u32NDA_SNR_A=&u32NDA_SNR_A;
  Drv_DVBS_GetSNR_PARAM.u32NDA_SNR_AB=&u32NDA_SNR_AB;
  Drv_DVBS_GetSNR_PARAM.ret = false;

  if(u32DVBSopen==1)
  {
     if  (u8DemodLockFlag == 0)
    {
    	Drv_DVBS_GetSNR_PARAM.ret=FALSE;
    }
    else
    {
       UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetSNR,&Drv_DVBS_GetSNR_PARAM);
    }

     if (Drv_DVBS_GetSNR_PARAM.ret== FALSE)
    {
       ULOGD("DEMOD","DVBS_GetSNR Fail! \n");
    }
    else
    {
        #if 0
        //NDA SNR
        NDA_SNR_A=(float)u32NDA_SNR_A/65536;
        NDA_SNR_AB=(float)u32NDA_SNR_AB/4194304;
        //
        //since support 16,32APSK we need to add judgement
        if(modulation_order==4)
            NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.252295758529242));//for 16APSK CR2/3
        else if(modulation_order==5)//(2-1.41333232789)
            NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.41333232789));//for 32APSK CR3/4
        else
            NDA_SNR_AB=(float)sqrt(NDA_SNR_AB);

        NDA_SNR_LINEAR =(1/((NDA_SNR_A/NDA_SNR_AB)-1)) ;

        if(NDA_SNR_LINEAR<=0)
            NDA_SNR=1.0;
        else
             NDA_SNR=10*log10(NDA_SNR_LINEAR);

        //printf("[DVBS]: NDA_SNR ================================: %.1f\n", NDA_SNR);
        //_f_DVBS_CurrentSNR = NDA_SNR;
        /*
            //[DVBS/S2, QPSK/8PSK, 1/2~9/10 the same CN]
            snr_poly = 0.0;     //use Polynomial curve fitting to fix SNR
            snr_poly = 0.005261367463671*pow(NDA_SNR, 3)-0.116517828301214*pow(NDA_SNR, 2)+0.744836970505452*pow(NDA_SNR, 1)-0.86727609780167;
            Fixed_SNR = NDA_SNR + snr_poly;
            //printf("[DVBS]: NDA_SNR + snr_poly =====================: %.1f\n", Fixed_SNR);

            if (Fixed_SNR < 17.0)
                Fixed_SNR = Fixed_SNR;
            else if ((Fixed_SNR < 20.0) && (Fixed_SNR >= 17.0))
                Fixed_SNR = Fixed_SNR - 0.8;
            else if ((Fixed_SNR < 22.5) && (Fixed_SNR >= 20.0))
                Fixed_SNR = Fixed_SNR - 2.0;
            else if ((Fixed_SNR < 27.0) && (Fixed_SNR >= 22.5))
                Fixed_SNR = Fixed_SNR - 3.0;
            else if ((Fixed_SNR < 29.0) && (Fixed_SNR >= 27.0))
                Fixed_SNR = Fixed_SNR - 3.5;
            else if (Fixed_SNR >= 29.0)
                Fixed_SNR = Fixed_SNR - 3.0;

            if (Fixed_SNR < 1.0)
                Fixed_SNR = 1.0;
            if (Fixed_SNR > 30.0)
                Fixed_SNR = 30.0;
        */

        #endif            

        NDA_SNR = u32NDA_SNR_A + ((float)u32NDA_SNR_AB / 256.0);
        
        *fSNR = NDA_SNR;
        #ifdef MS_DEBUG
         ULOGD("DEMOD", "[DVBS]: NDA_SNR=============================: %.1f\n", NDA_SNR);
        #endif
    	}

	Drv_DVBS_GetSNR_PARAM.ret = true;
  }
  else
  {
    return false;
  }   

  return Drv_DVBS_GetSNR_PARAM.ret;
}
#endif

#ifdef UFO_SUPPORT_KERNEL_FLOATING
MS_BOOL MDrv_DMD_DVBS_GetSNR_Kernel(MS_FLOAT_ST *fSNR)
{
    MS_U32 u32NDA_SNR_A =0;
    MS_U32 u32NDA_SNR_AB =0;
    //float NDA_SNR_A =0.0;
    //float NDA_SNR_AB =0.0;
    //float NDA_SNR =0.0;
    MS_FLOAT_ST NDA_SNR;
    //double NDA_SNR_LINEAR=0.0;

  DVBS_GetSNR_PARAM Drv_DVBS_GetSNR_PARAM;
  Drv_DVBS_GetSNR_PARAM.u32NDA_SNR_A=&u32NDA_SNR_A;
  Drv_DVBS_GetSNR_PARAM.u32NDA_SNR_AB=&u32NDA_SNR_AB;
  Drv_DVBS_GetSNR_PARAM.ret = false;

  if(u32DVBSopen==1)
  {
     if  (u8DemodLockFlag == 0)
    {
    	Drv_DVBS_GetSNR_PARAM.ret=FALSE;
    }
    else
    {
       UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetSNR,&Drv_DVBS_GetSNR_PARAM);
    }

     if (Drv_DVBS_GetSNR_PARAM.ret== FALSE)
    {
       ULOGD("DEMOD","DVBS_GetSNR Fail! \n");
    }
    else
    {
        #if 0
        //NDA SNR
        NDA_SNR_A=(float)u32NDA_SNR_A/65536;
        NDA_SNR_AB=(float)u32NDA_SNR_AB/4194304;
        //
        //since support 16,32APSK we need to add judgement
        if(modulation_order==4)
            NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.252295758529242));//for 16APSK CR2/3
        else if(modulation_order==5)//(2-1.41333232789)
            NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.41333232789));//for 32APSK CR3/4
        else
            NDA_SNR_AB=(float)sqrt(NDA_SNR_AB);

        NDA_SNR_LINEAR =(1/((NDA_SNR_A/NDA_SNR_AB)-1)) ;

        if(NDA_SNR_LINEAR<=0)
            NDA_SNR=1.0;
        else
             NDA_SNR=10*log10(NDA_SNR_LINEAR);

        //printf("[DVBS]: NDA_SNR ================================: %.1f\n", NDA_SNR);
        //_f_DVBS_CurrentSNR = NDA_SNR;
        /*
            //[DVBS/S2, QPSK/8PSK, 1/2~9/10 the same CN]
            snr_poly = 0.0;     //use Polynomial curve fitting to fix SNR
            snr_poly = 0.005261367463671*pow(NDA_SNR, 3)-0.116517828301214*pow(NDA_SNR, 2)+0.744836970505452*pow(NDA_SNR, 1)-0.86727609780167;
            Fixed_SNR = NDA_SNR + snr_poly;
            //printf("[DVBS]: NDA_SNR + snr_poly =====================: %.1f\n", Fixed_SNR);

            if (Fixed_SNR < 17.0)
                Fixed_SNR = Fixed_SNR;
            else if ((Fixed_SNR < 20.0) && (Fixed_SNR >= 17.0))
                Fixed_SNR = Fixed_SNR - 0.8;
            else if ((Fixed_SNR < 22.5) && (Fixed_SNR >= 20.0))
                Fixed_SNR = Fixed_SNR - 2.0;
            else if ((Fixed_SNR < 27.0) && (Fixed_SNR >= 22.5))
                Fixed_SNR = Fixed_SNR - 3.0;
            else if ((Fixed_SNR < 29.0) && (Fixed_SNR >= 27.0))
                Fixed_SNR = Fixed_SNR - 3.5;
            else if (Fixed_SNR >= 29.0)
                Fixed_SNR = Fixed_SNR - 3.0;

            if (Fixed_SNR < 1.0)
                Fixed_SNR = 1.0;
            if (Fixed_SNR > 30.0)
                Fixed_SNR = 30.0;
        */

        #endif       

        //NDA_SNR = u32NDA_SNR_A + ((float)u32NDA_SNR_AB / 256.0);

        NDA_SNR.DATA = ((u32NDA_SNR_A << 8) | u32NDA_SNR_AB);
        NDA_SNR.EXP = -8;
        
        *fSNR = NDA_SNR;
        #ifdef MS_DEBUG
         ULOGD("DEMOD", "[DVBS]: NDA_SNR=============================: %d*2^%d\n", NDA_SNR.DATA, NDA_SNR.EXP);
        #endif
    	}
  }

  return Drv_DVBS_GetSNR_PARAM.ret;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetPostViterbiBer(float *ber)
{
  MS_U16 BitErrPeriod_reg;
  MS_U32 BitErr_reg;
  
  DVBS_GetPostViterbiBer_PARAM Drv_DVBS_GetPostViterbiBer_PARAM;
  Drv_DVBS_GetPostViterbiBer_PARAM.BitErr_reg=&BitErr_reg;
  Drv_DVBS_GetPostViterbiBer_PARAM.BitErrPeriod_reg=&BitErrPeriod_reg;
  Drv_DVBS_GetPostViterbiBer_PARAM.ret=false;   

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetPostViterbiBer,&Drv_DVBS_GetPostViterbiBer_PARAM);

    if (BitErrPeriod_reg == 0 )    //PRD
        BitErrPeriod_reg = 1;    

    if(_bDemodType) //DVBS2
    {
        if (BitErr_reg <= 0 )
            *ber = 0.5f / (float)(BitErrPeriod_reg) / 64800.0;
        else
            *ber = (float)BitErr_reg / (float)(BitErrPeriod_reg) / 64800.0;            

        if (*ber <= 0.0f)
            *ber = 1.0e-10f;
        
        DMD_DBG(ULOGD("DEMOD","BER = %8.3e \n ", *ber));
    }
    else
    {
        if (BitErr_reg <= 0 )
            *ber = 0.5f / ((float)BitErrPeriod_reg*128*188*8);
        else
            *ber = (float)BitErr_reg / ((float)BitErrPeriod_reg*128*188*8);

        if (*ber <= 0.0f)
            *ber = 1.0e-10f;

        DMD_DBG(ULOGD("DEMOD","PostVitBER = %8.3e \n ", *ber));
    }
    
    Drv_DVBS_GetPostViterbiBer_PARAM.ret=true; 
  }

  return Drv_DVBS_GetPostViterbiBer_PARAM.ret;
}
#endif

#ifdef UFO_SUPPORT_KERNEL_FLOATING
MS_BOOL MDrv_DMD_DVBS_GetBER_Kernel(MS_FLOAT_ST *ber)
{
  MS_U16 BitErrPeriod_reg;
  MS_U32 BitErr_reg;

  MS_FLOAT_ST temp;
  
  DVBS_GetPostViterbiBer_PARAM Drv_DVBS_GetPostViterbiBer_PARAM;
  Drv_DVBS_GetPostViterbiBer_PARAM.BitErr_reg=&BitErr_reg;
  Drv_DVBS_GetPostViterbiBer_PARAM.BitErrPeriod_reg=&BitErrPeriod_reg;
  Drv_DVBS_GetPostViterbiBer_PARAM.ret=false;   

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetPostViterbiBer,&Drv_DVBS_GetPostViterbiBer_PARAM);

    if(BitErrPeriod_reg == 0)
    {
        (*ber).DATA = 1;
        (*ber).EXP = 0;        
    }
    else if(BitErr_reg == 0)
    {
        (*ber).DATA = 0;
        (*ber).EXP = 0;
    }
    else
    {
        (*ber).DATA = BitErr_reg;
        (*ber).EXP = 0;    

        if(_bDemodType) //DVBS2
        {
            temp.DATA = BitErrPeriod_reg*64800;
            temp.EXP = 0;
        }
        else
        {            
            temp.DATA = BitErrPeriod_reg*128*188*8;
            temp.EXP = 0;
        }

        *ber = MS_FLOAT_OP(*ber, temp, divide);  
    }   
    
    DMD_DBG(ULOGD("DEMOD","BER = %d*2^%d \n", (*ber).DATA, (*ber).EXP));                    
  }

  return Drv_DVBS_GetPostViterbiBer_PARAM.ret;
}
#endif
MS_BOOL MDrv_DMD_DVBS_GetPacketErr(MS_U16 *pktErr)
{
    DVBS_GetPacketErr_PARAM Drv_DVBS_GetPacketErr_PARAM;
    Drv_DVBS_GetPacketErr_PARAM.pktErr=pktErr;
    Drv_DVBS_GetPacketErr_PARAM.ret=false;

    if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetPacketErr,&Drv_DVBS_GetPacketErr_PARAM);
    }

    return Drv_DVBS_GetPacketErr_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_GetCellID(MS_U16 *u16CellID)
{
  DVBS_GetCellID_PARAM Drv_DVBS_GetCellID_PARAM;
  Drv_DVBS_GetCellID_PARAM.u16CellID=u16CellID;
  Drv_DVBS_GetCellID_PARAM.ret=false;

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetCellID,&Drv_DVBS_GetCellID_PARAM);
  }
  else
  {
    return false;
  }   

  return Drv_DVBS_GetCellID_PARAM.ret;
  
}


#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetStatus(DMD_DVBS_MODULATION_TYPE *pQAMMode, MS_U32 *u32SymbolRate, float *pFreqOff)
{
  MS_S16 s16CFO=0;
  
  DVBS_GetStatus_PARAM Drv_DVBS_GetStatus_PARAM;
  Drv_DVBS_GetStatus_PARAM.pQAMMode=pQAMMode;
  Drv_DVBS_GetStatus_PARAM.u32SymbolRate=u32SymbolRate;
  Drv_DVBS_GetStatus_PARAM.s16CFO=&s16CFO;
  Drv_DVBS_GetStatus_PARAM.ret=false;
  
  //float f_Fc, FreqCfo_offset;
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetStatus,&Drv_DVBS_GetStatus_PARAM);
      if(abs(s16CFO)%1000 >= 500)
      {
    	   if(s16CFO < 0)
    		*pFreqOff=(s16CFO/1000)-1.0;
    	   else
    		*pFreqOff=(s16CFO/1000)+1.0;	
      }
      else
    	   *pFreqOff = s16CFO/1000;

    *pQAMMode = *(Drv_DVBS_GetStatus_PARAM.pQAMMode);
    *u32SymbolRate = *(Drv_DVBS_GetStatus_PARAM.u32SymbolRate);
      
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetStatus %d %d %f \n", *pQAMMode, *u32SymbolRate, *pFreqOff);
    }
    #endif
  }
  else
  {
    return false;
  }   

  return Drv_DVBS_GetStatus_PARAM.ret; 
}
#endif

MS_U32 MDrv_DMD_DVBS_SetPowerState(EN_POWER_MODE u16PowerState)
{
    DVBS_SetPowerState_PARAM Drv_DVBS_SetPowerState_PARAM;
    Drv_DVBS_SetPowerState_PARAM.u16PowerState=u16PowerState;
    Drv_DVBS_SetPowerState_PARAM.ret_U32=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_SetPowerState\n"));
   if(u32DVBSopen==1)
   {
  UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetPowerState,&Drv_DVBS_SetPowerState_PARAM);
   }
   else
   {
  return false;
    }   

    return Drv_DVBS_SetPowerState_PARAM.ret_U32;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    MS_S16 s16CFO=0;
    DVBS_Get_FreqOffset_PARAM Drv_DVBS_Get_FreqOffset_PARAM;
    Drv_DVBS_Get_FreqOffset_PARAM.s16CFO=&s16CFO;
    Drv_DVBS_Get_FreqOffset_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_Get_FreqOffset\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_Get_FreqOffset,&Drv_DVBS_Get_FreqOffset_PARAM);
      if(abs(s16CFO)%1000 >= 500)
      {
    	   if(s16CFO < 0)
    		*pFreqOff=(s16CFO/1000)-1.0;
    	   else
    		*pFreqOff=(s16CFO/1000)+1.0;	
      }
      else
    	   *pFreqOff = s16CFO/1000;
	  #ifdef MS_DEBUG
	   ULOGD("DEMOD","MDrv_DMD_DVBS_Get_FreqOffset %f \n", *pFreqOff);
         #endif 
          //DBG_INTERN_DVBS(printf(">>> INTERN_DVBS_Get_FreqOffset *pFreqOff = %d[MHz] <<<\n", (MS_S16)*pFreqOff));
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_Get_FreqOffset_PARAM.bRet;
}
#endif

MS_BOOL MDrv_DMD_DVBS_BlindScan_Start(MS_U16 u16StartFreq, MS_U16 u16EndFreq)
{
    DVBS_BlindScan_Start_PARAM Drv_DVBS_BlindScan_Start_PARAM;
    Drv_DVBS_BlindScan_Start_PARAM.u16StartFreq=u16StartFreq;
    Drv_DVBS_BlindScan_Start_PARAM.u16EndFreq=u16EndFreq;
    Drv_DVBS_BlindScan_Start_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_Start\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_Start,&Drv_DVBS_BlindScan_Start_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_Start_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd)
{
    DVBS_BlindScan_NextFreq_PARAM Drv_DVBS_BlindScan_NextFreq_PARAM;
    Drv_DVBS_BlindScan_NextFreq_PARAM.bBlindScanEnd=bBlindScanEnd;
    Drv_DVBS_BlindScan_NextFreq_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_NextFreq\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_NextFreq,&Drv_DVBS_BlindScan_NextFreq_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_NextFreq_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_Cancel(void)
{
    DVBS_BlindScan_Cancel_PARAM Drv_DVBS_BlindScan_Cancel_PARAM;
    Drv_DVBS_BlindScan_Cancel_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_Cancel\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_Cancel,&Drv_DVBS_BlindScan_Cancel_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_Cancel_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_End(void)
{
    DVBS_BlindScan_End_PARAM Drv_DVBS_Blindscan_End_PARAM;
    Drv_DVBS_Blindscan_End_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_End\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_End,&Drv_DVBS_Blindscan_End_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_Blindscan_End_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_GetChannel(MS_U16 u16ReadStart, MS_U16* u16TPNum, HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    DVBS_BlindScan_GetChannel_PARAM Drv_DVBS_BlindScan_Getchannel_PARAM;
    Drv_DVBS_BlindScan_Getchannel_PARAM.u16ReadStart=u16ReadStart;
    Drv_DVBS_BlindScan_Getchannel_PARAM.u16TPNum=u16TPNum;
    Drv_DVBS_BlindScan_Getchannel_PARAM.pTable=pTable;
    Drv_DVBS_BlindScan_Getchannel_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_GetChannel\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_GetChannel,&Drv_DVBS_BlindScan_Getchannel_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_Getchannel_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFreq)
{
    DVBS_BlindScan_GetCurrentFreq_PARAM Drv_DVBS_BlindScan_GetCurrentFreq_PARAM;
    Drv_DVBS_BlindScan_GetCurrentFreq_PARAM.u32CurrentFreq=u32CurrentFreq;
    Drv_DVBS_BlindScan_GetCurrentFreq_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_GetCurrentFreq\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_GetCurrentFreq,&Drv_DVBS_BlindScan_GetCurrentFreq_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_GetCurrentFreq_PARAM.bRet;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_U8 substate_reg=2;
    MS_U32 u32Data=0;
    MS_U16 symbolrate_reg=0;
    MS_U16 CFO_reg;
    DVBS_BlindScan_WaitCurFreqFinished_PARAM Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.u8Progress=u8Progress;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.u8FindNum=u8FindNum;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.substate_reg=&substate_reg;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.u32Data=&u32Data;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.symbolrate_reg=&symbolrate_reg;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.CFO_reg=&CFO_reg;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_WaitCurFreqFinished,&Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM);

        substate_reg = *Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.substate_reg;
        u32Data = *Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.u32Data;
        symbolrate_reg = *Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.symbolrate_reg;
        CFO_reg = *Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.CFO_reg;
        //CFO_reg = 0;
      
      if(substate_reg==0)
      {
        _u16ChannelInfoArray[0][_u16ChannelInfoIndex]=((u32Data+500)/1000);
        _u16LockedCenterFreq=((u32Data+500)/1000);                //Center Freq
        _u16ChannelInfoArray[1][_u16ChannelInfoIndex]=(symbolrate_reg);//Symbol Rate
        _u16LockedSymbolRate=symbolrate_reg;
        _u16ChannelInfoIndex++;
        *u8FindNum=_u16ChannelInfoIndex;
	 if (CFO_reg*1000 >= 0x8000)
        {
           CFO_reg=0x10000- CFO_reg*1000;
           _s16CurrentCFO=-1*CFO_reg/1000;
        }
        else
        {
           _s16CurrentCFO=CFO_reg;
        }
        DMD_DBG(ULOGD("DEMOD","Current Locked CF:%d BW:%d CFO:%d \n ", _u16LockedCenterFreq, _u16LockedSymbolRate, _s16CurrentCFO));
      }
      else if(substate_reg==1) 
      {
         if (CFO_reg*1000 >= 0x8000)
         {
                CFO_reg=0x1000- CFO_reg*1000;
                _s16CurrentCFO=-1*CFO_reg/1000;
         }
         else
         {
                _s16CurrentCFO=CFO_reg;
         }
         DMD_DBG(ULOGD("DEMOD","Pre Locked CF:%d BW:%d CFO:%d \n ", _u16LockedCenterFreq, _u16LockedSymbolRate, _s16CurrentCFO));
      }	
      DMD_DBG(ULOGD("DEMOD","MDrv_Demod_BlindScan_WaitCurFreqFinished- u8Progress: %d u8FindNum %d\n", *u8Progress, *u8FindNum));

   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.bRet;
}
#endif

MS_BOOL MDrv_DMD_DVBS_BlindScan_GetTunerFreq_EX(MS_U16 *u16TunerCenterFreq, MS_U16 *u16TunerCutOffFreq)
{
    DVBS_BlindScan_GetTunerFreq_PARAM Drv_DVBS_BlindScan_GetTunerFreq_PARAM;
    Drv_DVBS_BlindScan_GetTunerFreq_PARAM.u16TunerCenterFreq=u16TunerCenterFreq;
    Drv_DVBS_BlindScan_GetTunerFreq_PARAM.u16TunerCutOffFreq=u16TunerCutOffFreq;
    Drv_DVBS_BlindScan_GetTunerFreq_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_GetTunerFreq_EX\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_GetTunerFreq,&Drv_DVBS_BlindScan_GetTunerFreq_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_GetTunerFreq_PARAM.bRet;
}

//------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_DVBS_DiSEqC_Init(void)
{
    DVBS_DiSEqC_Init_PARAM Drv_DVBS_DiSEqC_Init_PARAM;
    Drv_DVBS_DiSEqC_Init_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_Init\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_Init,&Drv_DVBS_DiSEqC_Init_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_Init_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetLNBOut(MS_BOOL bLow)
{
    DVBS_DiSEqC_SetLNBOut_PARAM Drv_DVBS_DiSEqC_SetLNBOut_PARAM;
    Drv_DVBS_DiSEqC_SetLNBOut_PARAM.bLow=bLow;
    Drv_DVBS_DiSEqC_SetLNBOut_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SetLNBOut\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SetLNBOut,&Drv_DVBS_DiSEqC_SetLNBOut_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SetLNBOut_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow)
{
    DVBS_DiSEqC_GetLNBOut_PARAM Drv_DVBS_DiSEqC_GetLNBOut_PARAM;
    Drv_DVBS_DiSEqC_GetLNBOut_PARAM.bLNBOutLow=bLNBOutLow;
    Drv_DVBS_DiSEqC_GetLNBOut_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_GetLNBOut\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_GetLNBOut,&Drv_DVBS_DiSEqC_GetLNBOut_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_GetLNBOut_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_Set22kOnOff(MS_BOOL b22kOn)
{
    DVBS_DiSEqC_Set22kOnOff_PARAM Drv_DVBS_DiSEqC_Set22kOnOff_PARAM;
    Drv_DVBS_DiSEqC_Set22kOnOff_PARAM.b22kOn=b22kOn;
    Drv_DVBS_DiSEqC_Set22kOnOff_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_Set22kOnOff\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_Set22kOnOff,&Drv_DVBS_DiSEqC_Set22kOnOff_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_Set22kOnOff_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn)
{
    DVBS_DiSEqC_Get22kOnOff_PARAM Drv_DVBS_DiSEqC_Get22kOnOff_PARAM;
    Drv_DVBS_DiSEqC_Get22kOnOff_PARAM.b22kOn=b22kOn;
    Drv_DVBS_DiSEqC_Get22kOnOff_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_Get22kOnOff\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_Get22kOnOff,&Drv_DVBS_DiSEqC_Get22kOnOff_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_Get22kOnOff_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
    DVBS_DiSEqC_SendCmd_PARAM Drv_DVBS_DiSEqC_SendCmd_PARAM;
    Drv_DVBS_DiSEqC_SendCmd_PARAM.pCmd=pCmd;
    Drv_DVBS_DiSEqC_SendCmd_PARAM.u8CmdSize=u8CmdSize;
    Drv_DVBS_DiSEqC_SendCmd_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SendCmd\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SendCmd,&Drv_DVBS_DiSEqC_SendCmd_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SendCmd_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetTxToneMode(MS_BOOL bTxTone22kOff)
{
    DVBS_DiSEqC_SetTxToneMode_PARAM Drv_DVBS_DiSEqC_SetTxToneMode_PARAM;
    Drv_DVBS_DiSEqC_SetTxToneMode_PARAM.bTxTone22kOff=bTxTone22kOff;
    Drv_DVBS_DiSEqC_SetTxToneMode_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SetTxToneMode\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SetTxToneMode,&Drv_DVBS_DiSEqC_SetTxToneMode_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SetTxToneMode_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetTone(MS_BOOL bTone1)
{
    DVBS_DiSEqC_SetTone_PARAM Drv_DVBS_DiSEqC_SetTone_PARAM;
    Drv_DVBS_DiSEqC_SetTone_PARAM.bTone1=bTone1;
    Drv_DVBS_DiSEqC_SetTone_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SetTone\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SetTone,&Drv_DVBS_DiSEqC_SetTone_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SetTone_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_UnicableAGCCheckPower(MS_BOOL pbAGCCheckPower)
{
    DVBS_UnicableAGCCheckPower_PARAM Drv_DVBS_UnicableAGCCheckPower_PARAM;
    Drv_DVBS_UnicableAGCCheckPower_PARAM.pbAGCCheckPower=pbAGCCheckPower;
    Drv_DVBS_UnicableAGCCheckPower_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_UnicableAGCCheckPower\n"));
   if(u32DVBSopen==1)   
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_UnicableAGCCheckPower,&Drv_DVBS_UnicableAGCCheckPower_PARAM);   
   }
   else   
   {
      return false;
   }
   return Drv_DVBS_UnicableAGCCheckPower_PARAM.bRet;
}

#if defined UFO_SUPPORT_VCM || defined UFO_DEMOD_DVBS_SUPPORT_DMD_INT
MS_BOOL MDrv_DMD_DVBS2_TS_DivNum_Calculation(void)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_TS_DivNum_Calculation\n"));

    MS_U32      u32SymbolRate=0;
    //float       fSymbolRate;
    //MS_U8 ISSY_EN = 0;
    MS_U8 code_rate_idx = 0;
    MS_U8 pilot_flag = 0;
    MS_U8 fec_type_idx = 0;
    MS_U16 k_bch_array[2][42] =
    {
        {
            16008, 21408, 25728, 32208, 38688, 43040, 48408, 51648, 53840, 57472,
            58192, 14208, 18528, 28968, 32208, 34368, 35448, 35808, 37248, 37248,
            38688, 40128, 41208, 41568, 43008, 44448, 44808, 45888, 46608, 47328,
            47328, 48408, 50208, 50208, 55248,     0,     0,     0,     0,     0,
            0,     0,
        },
        {
            3072,  5232,  6312,  7032,  9552, 10632, 11712, 12432, 13152, 14232,
            0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
            0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
            0,     0,     0,     0,     0,  3792,  4152,  4872,  7392,  8472,
            9192, 11352
        }
    };
    MS_U16 n_ldpc_array[2] = {64800, 16200};
    
    MS_FLOAT pilot_term = 0;
    MS_FLOAT k_bch;    
    MS_FLOAT n_ldpc;
    MS_FLOAT ts_div_num_offset = 1.5;
    MS_FLOAT ts_div_num_margin_ratio = 1.03;    
    MS_FLOAT ftmp;
    MS_U32 u32temp;
    MS_U8 system_type_reg;
    MS_U8 code_rate_reg;
    MS_FLOAT fTSDivNum;
    MS_U8 u8Data;
    MS_BOOL status = TRUE;

    DVBS_GetTsDivNum_PARAM Drv_DVBS_GetTsDivNum_PARAM;
    Drv_DVBS_GetTsDivNum_PARAM.u32SymbolRate=&u32SymbolRate;
    Drv_DVBS_GetTsDivNum_PARAM.system_type_reg=&system_type_reg;
    Drv_DVBS_GetTsDivNum_PARAM.code_rate_idx=&code_rate_idx;
    Drv_DVBS_GetTsDivNum_PARAM.fec_type_idx=&fec_type_idx;
    Drv_DVBS_GetTsDivNum_PARAM.pilot_flag=&pilot_flag;
    Drv_DVBS_GetTsDivNum_PARAM.u32temp=&u32temp;
    Drv_DVBS_GetTsDivNum_PARAM.code_rate_reg=&code_rate_reg;
    Drv_DVBS_GetTsDivNum_PARAM.bRet=false;

    if(u8DemodLockFlag==1)
    {    
         UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_GetTsDivNum, &Drv_DVBS_GetTsDivNum_PARAM);
         status &= Drv_DVBS_GetTsDivNum_PARAM.bRet;
         
         if(!system_type_reg)//DVBS2
         {
                k_bch = k_bch_array[fec_type_idx][code_rate_idx];
                n_ldpc = n_ldpc_array[fec_type_idx];
                pilot_term = ((float) n_ldpc / modulation_order / 1440 * 36) * pilot_flag;

                //data rate
                ftmp = k_bch/((n_ldpc/modulation_order+90+pilot_term)/u32SymbolRate);
                
                if(sDMD_DVBS_Info.bSerialTS)//serial mode
                {
                    fTSDivNum = 288000.0 / ftmp / 2.0 - 1;
                    //fTSDivNum = fTSDivNum/2-1;// since  288/(2(fTSDivNum+1)) = 288/TS_RATE = A  ==> fTSDivNum = A/2-1
                }
                else//parallel mode
                {
                    fTSDivNum = 288000.0 / (ftmp/8.0) / 2.0 - 1;
                    //fTSDivNum = (fTSDivNum-1)/2;
                }
                
                fTSDivNum -= ts_div_num_offset;
         }
         else
        {
             code_rate_reg = *Drv_DVBS_GetTsDivNum_PARAM.code_rate_reg;
             switch (code_rate_reg)
             {
                case 0x00: //CR 1/2
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x01: //CR 2/3
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x02: //CR 3/4
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                      //fTSDivNum = fTSDivNum/2-1-5;
                    break;
                case 0x03: //CR 5/6
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x04: //CR 7/8
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
                default:
                    if(sDMD_DVBS_Info.bSerialTS)
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2)) /2)-ts_div_num_offset;
                    else
                        fTSDivNum = ((288000/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2/8.0))/2)-ts_div_num_offset;

                    //fTSDivNum = fTSDivNum/2-1-5;

                    break;
             }
         }

        if(fTSDivNum > 0x1F)
            fTSDivNum = 0x1F;
        else if(fTSDivNum < 0x01)
            fTSDivNum = 0x01;

         u8Data = (MS_U8)fTSDivNum;         
         
	  #ifdef MS_DEBUG
         ULOGD("DEMOD", ">>>INTERN_DVBS_GetLock TsClkDivNum = 0x%x<<<\n", u8Data);
         #endif         

         HAL_DMD_RIU_WriteByte(0x103300, u8Data);
                    
         //Ts Output Enable
         HAL_DMD_RIU_WriteByte(0x101eaa,0x10);
    }    

    return status;
}

MS_BOOL MDrv_DMD_DVBS2_Set_Default_IS_ID(MS_U8 *u8IS_ID_table)
{
    MS_U8 IS_ID = 0;
    DVBS_SetDefaultVCM_PARAM Drv_DVBS_SetDefaultVCM_PARAM;
    Drv_DVBS_SetDefaultVCM_PARAM.u8IS_ID = &IS_ID;
    Drv_DVBS_SetDefaultVCM_PARAM.u8IS_ID_table = u8IS_ID_table;
    Drv_DVBS_SetDefaultVCM_PARAM.ret = false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_Set_Default_IS_ID\n"));

   if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_Set_Default_IS_ID, &Drv_DVBS_SetDefaultVCM_PARAM);
    }

   ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]Defalut IS_ID = %d\n",IS_ID);

    return Drv_DVBS_SetDefaultVCM_PARAM.ret;    
}


MS_BOOL MDrv_DMD_DVBS2_Set_IS_ID(MS_U8 u8IS_ID)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_Set_IS_ID\n"));

    DVBS_SetVCM_PARAM Drv_DVBS_SetVCM_PARAM;
    Drv_DVBS_SetVCM_PARAM.u8IS_ID=u8IS_ID;
    Drv_DVBS_SetVCM_PARAM.ret=false;
   if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_Set_IS_ID, &Drv_DVBS_SetVCM_PARAM);
    }
    return Drv_DVBS_SetVCM_PARAM.ret;
}


MS_BOOL MDrv_DMD_DVBS2_Get_IS_ID_INFO(MS_U8 *u8IS_ID, MS_U8 *u8IS_ID_table)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_Get_IS_ID_INFO\n"));

    DVBS_GetVCM_PARAM Drv_DVBS_GetVCM_PARAM;
    Drv_DVBS_GetVCM_PARAM.u8IS_ID=u8IS_ID;
    Drv_DVBS_GetVCM_PARAM.u8IS_ID_table=u8IS_ID_table;
    Drv_DVBS_GetVCM_PARAM.ret=false;
    if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_Get_IS_ID_INFO, &Drv_DVBS_GetVCM_PARAM); 
    }
    
    return Drv_DVBS_GetVCM_PARAM.ret;    
}

MS_BOOL MDrv_DMD_DVBS2_VCM_Init(DMD_DVBS_VCM_OPT u8VCM_OPT, MS_U8 u8IS_ID, MS_U32 u32DVBS2_DJB_START_ADDR)
{
    void* pAttribte = NULL;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_VCM_Init\n"));    

    DVBS_VCM_INIT_PARAM Drv_DVBS_VCM_INIT_PARAM;
    Drv_DVBS_VCM_INIT_PARAM.u8VCM_OPT = u8VCM_OPT;
    Drv_DVBS_VCM_INIT_PARAM.u8IS_ID = u8IS_ID;
    Drv_DVBS_VCM_INIT_PARAM.u32DVBS2_DJB_START_ADDR = u32DVBS2_DJB_START_ADDR;
    Drv_DVBS_VCM_INIT_PARAM.ret = false;

     if(u8DVBSUtopiaOpen == 0)  // First time open
    { 
        if(UtopiaOpen(MODULE_DVBS/*|KERNEL_MODE*/ , &ppDVBSInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //kernel space
        //if(UtopiaOpen(MODULE_DVBS , &ppDVBSInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //user space
        {
            u32DVBSopen = 1;
            ULOGD("DEMOD","Leo check DVBS utopia open sucessful!!\n");
      //return_val=true;
           // ULOGD("DEMOD","\r\n ======== DVBS Open Successful %x =========", (WORD)u32DVBSopen);
        }
        else
        {
          //  ULOGD("DEMOD","\r\n ======== DVBS Open Fail %x =========", (WORD)u32DVBSopen);
       //return_val=false;
          ULOGD("DEMOD","DVBS utopia open fail!!\n");
          return false;
        }

        u8DVBSUtopiaOpen = 1;
    }
    
    if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_VCM_Init, &Drv_DVBS_VCM_INIT_PARAM); 
    }
    
    return Drv_DVBS_VCM_INIT_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS2_VCM_Check(void)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_VCM_Check\n"));  

    DVBS_VCM_CHECK Drv_DVBS_VCM_CHECK;
    Drv_DVBS_VCM_CHECK.ret = false;
    
   if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_VCM_Check, &Drv_DVBS_VCM_CHECK); 
    }    

   return Drv_DVBS_VCM_CHECK.ret;
}

MS_BOOL MDrv_DMD_DVBS2_VCM_Enabled(MS_U8 u8VCM_ENABLED)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_VCM_Enabled\n"));  
    DVBS_VCM_ENABLED Drv_DVBS_VCM_ENABLED;
    Drv_DVBS_VCM_ENABLED.u8VCM_ENABLED = u8VCM_ENABLED;
    Drv_DVBS_VCM_ENABLED.ret = false;

   if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_VCM_Enabled, &Drv_DVBS_VCM_ENABLED); 
    }    

   return Drv_DVBS_VCM_ENABLED.ret;
    
}

MS_BOOL MDrv_DMD_DVBS2_VCM_Change_Mode(DMD_DVBS_VCM_OPT u8VCM_OPT)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_VCM_Change_Mode\n"));  
    DVBS_VCM_MODE Drv_DVBS_VCM_MODE;
    Drv_DVBS_VCM_MODE.u8VCM_OPT = u8VCM_OPT;
    Drv_DVBS_VCM_MODE.ret = false;

   if(u32DVBSopen==1)
    {
        UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_VCM_Mode, &Drv_DVBS_VCM_MODE); 
    }    

   return Drv_DVBS_VCM_MODE.ret;    
}
#endif

#ifdef UFO_SUPPORT_KERNEL_FLOATING
MS_BOOL MDrv_DMD_DVBS_TS_DivNum_Calculation_Kernel(void)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS2_TS_DivNum_Calculation\n"));

    MS_U32      u32SymbolRate=0;
    //float       fSymbolRate;
    //MS_U8 ISSY_EN = 0;
    MS_U8 code_rate_idx = 0;
    MS_U8 pilot_flag = 0;
    MS_U8 fec_type_idx = 0;
    MS_U16 k_bch_array[2][42] =
    {
        {
            16008, 21408, 25728, 32208, 38688, 43040, 48408, 51648, 53840, 57472,
            58192, 14208, 18528, 28968, 32208, 34368, 35448, 35808, 37248, 37248,
            38688, 40128, 41208, 41568, 43008, 44448, 44808, 45888, 46608, 47328,
            47328, 48408, 50208, 50208, 55248,     0,     0,     0,     0,     0,
            0,     0,
        },
        {
            3072,  5232,  6312,  7032,  9552, 10632, 11712, 12432, 13152, 14232,
            0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
            0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
            0,     0,     0,     0,     0,  3792,  4152,  4872,  7392,  8472,
            9192, 11352
        }
    };
    MS_U16 n_ldpc_array[2] = {64800, 16200};

    MS_U8 DVBS_CR_TABLE[5] = {2,3,4,6,8}; // 1/2, 2/3, 3/4, 5/6, 7/8
    
    MS_FLOAT_ST pilot_term;
    MS_FLOAT_ST k_bch;    
    MS_FLOAT_ST n_ldpc;
    MS_FLOAT_ST ts_div_num_offset; // 1.5

    ts_div_num_offset.DATA = 3;
    ts_div_num_offset.EXP = -1;

    MS_FLOAT_ST ts_div_num_margin_ratio; // 1.03

    ts_div_num_margin_ratio.DATA = 1105954079;
    ts_div_num_margin_ratio.EXP = -30;    
    
    MS_FLOAT_ST ftmp;
    MS_FLOAT_ST temp, temp1;
    
    MS_U32 u32temp;
    MS_U8 system_type_reg;
    MS_U8 code_rate_reg;
    MS_FLOAT_ST fTSDivNum;
    MS_U8 u8Data;
    MS_BOOL status = TRUE;

    DVBS_GetTsDivNum_PARAM Drv_DVBS_GetTsDivNum_PARAM;
    Drv_DVBS_GetTsDivNum_PARAM.u32SymbolRate=&u32SymbolRate;
    Drv_DVBS_GetTsDivNum_PARAM.system_type_reg=&system_type_reg;
    Drv_DVBS_GetTsDivNum_PARAM.code_rate_idx=&code_rate_idx;
    Drv_DVBS_GetTsDivNum_PARAM.fec_type_idx=&fec_type_idx;
    Drv_DVBS_GetTsDivNum_PARAM.pilot_flag=&pilot_flag;
    Drv_DVBS_GetTsDivNum_PARAM.u32temp=&u32temp;
    Drv_DVBS_GetTsDivNum_PARAM.code_rate_reg=&code_rate_reg;
    Drv_DVBS_GetTsDivNum_PARAM.bRet=false;

    if(u8DemodLockFlag==1)
    {    
         UtopiaIoctl(ppDVBSInstant, DMD_DVBS_DRV_CMD_GetTsDivNum, &Drv_DVBS_GetTsDivNum_PARAM);
         status &= Drv_DVBS_GetTsDivNum_PARAM.bRet;
         
         if(!system_type_reg) // DVBS2
         {
                k_bch.DATA = k_bch_array[fec_type_idx][code_rate_idx];
                k_bch.EXP = 0;
                n_ldpc.DATA = n_ldpc_array[fec_type_idx];
                n_ldpc.EXP = 0;        
                
                pilot_term.DATA = ( n_ldpc.DATA / modulation_order / 1440 * 36) * pilot_flag + 90; // pilot_term + header
                pilot_term.EXP = 0;

                //data rate
                //ftmp = k_bch/(  (n_ldpc/modulation_order+90+pilot_term)  / u32SymbolRate);

                temp.DATA = modulation_order;
                temp.EXP = 0;

                temp = MS_FLOAT_OP(n_ldpc, temp, divide); // temp = n_ldpc / modulation_order
                //ULOGD("DEMOD", ">>>n_ldpc / modulation_order = %d*2^%d<<<\n", temp.DATA, temp.EXP);                         

                temp = MS_FLOAT_OP(temp, pilot_term, add); // temp += pilot_term + header(90)
                //ULOGD("DEMOD", ">>>n_ldpc / modulation_order + pilot_term + header(90) = %d*2^%d<<<\n", temp.DATA, temp.EXP);                         

                ftmp.DATA = u32SymbolRate;
                ftmp.EXP = 0;

                ftmp = MS_FLOAT_OP(temp, ftmp, divide); // ftmp = temp /= u32SymbolRate = (n_ldpc/modulation_order+pilot_term+header(90)) / u32SymbolRate

                ftmp = MS_FLOAT_OP(k_bch, ftmp, divide); // ftmp = k_bch / ( (n_ldpc/modulation_order+pilot_term+header(90)) / u32SymbolRate)

                //ULOGD("DEMOD", ">>>ftmp = %d*2^%d<<<\n", ftmp.DATA, ftmp.EXP);                         
                
                if(sDMD_DVBS_Info.bSerialTS)//serial mode
                {
                    //fTSDivNum = 288000.0 / ftmp / 2.0 - 1;

                    temp.DATA = 288000;
                    temp.EXP = 0;
                    
                    fTSDivNum = MS_FLOAT_OP(temp, ftmp, divide); // fTSDivNum = 288000 / ftmp

                    fTSDivNum.EXP -= 1; // fTSDivNum /= 2

                    temp.DATA = 1;
                    temp.EXP = 0;

                    fTSDivNum = MS_FLOAT_OP(fTSDivNum, temp, minus); // fTSDivNum -= 1
                }
                else//parallel mode
                {
                    //fTSDivNum = 288000.0 / (ftmp/8.0) / 2.0 - 1;

                    temp.DATA = 288000;
                    temp.EXP = 3;                    
                    
                    fTSDivNum = MS_FLOAT_OP(temp, ftmp, divide); // fTSDivNum = 288000 / (ftmp/8)

                    fTSDivNum.EXP -= 1; // fTSDivNum /= 2

                    temp.DATA = 1;
                    temp.EXP = 0;

                    fTSDivNum = MS_FLOAT_OP(fTSDivNum, temp, minus); // fTSDivNum -= 1

                    //ULOGD("DEMOD", ">>>fTSDivNum = %d*2^%d<<<\n", fTSDivNum.DATA, fTSDivNum.EXP);                                             
                }
                
                fTSDivNum = MS_FLOAT_OP(fTSDivNum, ts_div_num_offset, minus); // fTSDivNum -= ts_div_num_offset
         }
         else // DVBS
         {           
            if(code_rate_reg < 5)
                code_rate_reg = DVBS_CR_TABLE[code_rate_reg]; // mapping code_rate_reg => CR
            else
                code_rate_reg = 8; // default set CR = 7/8

            temp.DATA = code_rate_reg;
            temp.EXP = 0;
            temp1.DATA = code_rate_reg-1;
            temp1.EXP = 0;

            ftmp = MS_FLOAT_OP(temp1, temp, divide); // ftmp = (CR_REG-1) / CR_REG = CR
            
            temp.DATA = 204;
            temp.EXP = 0;
            temp1.DATA = 188;
            temp1.EXP = 0;

            temp = MS_FLOAT_OP(temp1, temp, divide); // temp = 188 / 204

            ftmp = MS_FLOAT_OP(ftmp, temp, multiply); // ftmp = CR * (188/204)    

            temp.DATA = u32SymbolRate*2;
            temp.EXP = 0;

            temp = MS_FLOAT_OP(temp, ts_div_num_margin_ratio, multiply); // temp = u32SymbolRate * 2 * ts_div_num_margin_ratio

            ftmp = MS_FLOAT_OP(ftmp, temp, multiply); // ftmp = CR * (188/204) * u32SymbolRate * 2 * ts_div_num_margin_ratio;

            if(sDMD_DVBS_Info.bSerialTS) // serial mode            
            {
                temp.DATA = 288000;
                temp.EXP = 0;
            }
            else // parallel mode
            {
                temp.DATA = 288000*8;
                temp.EXP = 0;            
            }

            fTSDivNum = MS_FLOAT_OP(temp, ftmp, divide); // fTSDivNum = (288000*(parallel mode*8) / (CR*(188.0/204.0)*u32SymbolRate*ts_div_num_margin_ratio*2));                                                                  

            fTSDivNum.EXP -= 1;// fTSDivNum /= 2

            fTSDivNum = MS_FLOAT_OP(fTSDivNum, ts_div_num_offset, minus); // fTSDivNum -= ts_div_num_offset            
         }

        temp.DATA = 0x1F;
        temp.EXP = 0;

        if( MS_FLOAT_OP(fTSDivNum, temp, minus).DATA > 0 ) // fTSDivNum > 0x1F
        {
            fTSDivNum.DATA = 0x1F;
            fTSDivNum.EXP = 0;
        }

        temp.DATA = 0x01;
        temp.EXP = 0;

        if( MS_FLOAT_OP(fTSDivNum, temp, minus).DATA < 0 ) // fTSDivNum < 0x01
        {
            fTSDivNum.DATA = 0x01;
            fTSDivNum.EXP = 0;
        }                       

        // shift bits
         if(fTSDivNum.EXP >= 0)
            u8Data = (fTSDivNum.DATA) << (fTSDivNum.EXP);
         else
            u8Data = (fTSDivNum.DATA) >> ((-1)*(fTSDivNum.EXP));         
         
	  #ifdef MS_DEBUG
         ULOGD("DEMOD", ">>>INTERN_DVBS_Kernel_Mode TsClkDivNum = 0x%x<<<\n", u8Data);
         #endif
         
         HAL_DMD_RIU_WriteByte(0x103300, u8Data);
                    
         //Ts Output Enable
         HAL_DMD_RIU_WriteByte(0x101eaa,0x10);
    }    
    
    return status;
}
#endif
