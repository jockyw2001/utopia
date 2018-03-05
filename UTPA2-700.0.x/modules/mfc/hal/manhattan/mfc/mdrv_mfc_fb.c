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
#define _MSFB_C_

#include "mdrv_mfc_platform.h"
#include "mdrv_mfc.h"
#include "mdrv_mfc_fb.h"


BOOL S7M = 0;

#if(CODESIZE_SEL == CODESIZE_ALL)

code MST_MFC_RegUnitType_t tInitializeMiuU3_DDR2_12xx[]=
{
    {0x1202, 0x92},
    {0x1203, 0x03},
    {0x1204, 0x0A},
    {0x1205, 0x00},
    {0x1206, 0x38},
    {0x1207, 0x04},
    {0x1208, 0x77},
    {0x1209, 0x18},
    {0x120A, 0x46},
    {0x120B, 0x1F},
    {0x120C, 0x86},
    {0x120D, 0x94}, //Wilson recommand 090722
    {0x120E, 0x44},
    {0x120F, 0x20},

    {0x1210, 0x73},
    {0x1211, 0x0e},
    {0x1212, 0x04},
    {0x1213, 0x40},
    {0x1214, 0x00},
    {0x1215, 0x80},
    {0x1216, 0x00},
    {0x1217, 0xc0},
    {0x1218, 0x00},
    {0x1219, 0x00},
    {0x121A, 0x00},
    {0x121B, 0x00},
    {0x121C, 0x00},
    {0x121D, 0x00},
    {0x121E, 0x00},
    {0x121F, 0x0c},

    {0x1222, 0x00},
    {0x1223, 0x03},

    {0x1226, 0x44},
    {0x1227, 0x00},
    {0x1228, 0x00},
    {0x1229, 0x00},
    {0x122A, 0xaa},
    {0x122B, 0xaa},
    {0x122C, 0x00},
    {0x122D, 0x46},
    {0x122E, 0x44},
    {0x122F, 0x00},

    {0x1230, 0x44},
    {0x1231, 0x00},
    {0x1236, 0x3f},
    {0x1237, 0x00},
    {0x1238, 0x3f},
    {0x1239, 0x00},
    {0x123C, 0x00},
    {0x123D, 0x00},
    {0x123E, 0x03},
    {0x123F, 0x00},

    {0x1240, 0x01},
    {0x1241, 0x80},
    {0x1242, 0x00},
    {0x1243, 0x00},
    {0x1244, 0x00},
    {0x1245, 0x00},
    {0x1246, 0x00},
    {0x1247, 0x00},
    {0x1248, 0xff},
    {0x1249, 0xff},
    {0x124A, 0x10},
    {0x124B, 0x32},
    {0x124C, 0x54},
    {0x124D, 0x76},
    {0x124E, 0x98},
    {0x124F, 0xba},

    {0x1250, 0xdc},
    {0x1251, 0xfe},
    {0x1252, 0x00},
    {0x1253, 0x00},
    {0x1254, 0x00},
    {0x1255, 0x00},
    {0x1256, 0x00},
    {0x1257, 0x00},
    {0x1258, 0x00},
    {0x1259, 0x00},
    {0x125A, 0x00},
    {0x125B, 0x00},
    {0x125C, 0x00},
    {0x125D, 0x00},
    {0x125E, 0x00},
    {0x125F, 0x00},


    {0x12E2, 0xFF},
    {0x12E3, 0x3F},
    {0x12E4, 0x01},
    {0x12E5, 0x10},
    {0x12E8, 0xA5},
    {0x12E9, 0x5A},

{_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeMiuU3_DDR2_13xx[]=
{
    {0x1308, 0xFF},
    {0x1309, 0xFF},
    {0x130A, 0x07},

    {0x1320, 0x29},

    {0x1346, 0x44},
    {0x1347, 0x44},
    {0x134E, 0x44},
    {0x134F, 0x44},

    {0x1360, 0x38},
    {0x1361, 0x23},
    {0x1362, 0x7F},

    {0x1372, 0x03},
    {0x137A, 0x07},

    {_END_OF_TBL_, _END_OF_TBL_},
};


code MST_MFC_RegUnitType_t tInitializeMiuU3_DDR3_12xx[]=
{
    {0x1202, 0xa3},
    {0x1203, 0x03},
    {0x1204, 0x0B},
    {0x1205, 0x00},
    {0x1206, 0x50},
    {0x1207, 0x06},
    {0x1208, 0xCC},
    {0x1209, 0x1D},
    {0x120A, 0x86},
    {0x120B, 0x28},
    {0x120C, 0xC8},
    {0x120D, 0x96},
    {0x120E, 0x58},
    {0x120F, 0x40},

    {0x1210, 0x70},
    {0x1211, 0x0C},
    {0x1213, 0x40},
    {0x1214, 0x18},
    {0x1215, 0x80},
    {0x1216, 0x00},
    {0x1217, 0xc0},
    {0x1218, 0x00},
    {0x1219, 0x00},
    {0x121A, 0x00},
    {0x121B, 0x00},
    {0x121C, 0x00},
    {0x121D, 0x00},
    {0x121E, 0x00},
    {0x121F, 0x0c},

    {0x1222, 0x00},
    {0x1223, 0x03},

    {0x1226, 0x44},
    {0x1227, 0x00},
    {0x1228, 0x40}, //00
    {0x1229, 0x00},
    {0x122A, 0xaa},
    {0x122B, 0xaa},
    {0x122C, 0x80},
    {0x122D, 0x06},
    {0x122E, 0x00},
    {0x122F, 0x00},

    {0x1230, 0x55},
    {0x1231, 0x00},
    {0x1236, 0x00},
    {0x1237, 0x01},
    {0x1238, 0x00},
    {0x1239, 0x01},
    {0x123C, 0x00},
    {0x123D, 0x00},
    {0x123E, 0x05},
    {0x123F, 0x00},

    {0x1240, 0x01},
    {0x1241, 0x80},
    {0x1242, 0x00},
    {0x1243, 0x00},
    {0x1244, 0x00},
    {0x1245, 0x00},
    {0x1246, 0x00},
    {0x1247, 0x00},
    {0x1248, 0xff},
    {0x1249, 0xff},
    {0x124A, 0x10},
    {0x124B, 0x32},
    {0x124C, 0x54},
    {0x124D, 0x76},
    {0x124E, 0x98},
    {0x124F, 0xba},

    {0x1250, 0xdc},
    {0x1251, 0xfe},
    {0x1252, 0x00},
    {0x1253, 0x00},
    {0x1254, 0x00},
    {0x1255, 0x00},
    {0x1256, 0x00},
    {0x1257, 0x00},
    {0x1258, 0x00},
    {0x1259, 0x00},
    {0x125A, 0x00},
    {0x125B, 0x00},
    {0x125C, 0x00},
    {0x125D, 0x00},
    {0x125E, 0x00},
    {0x125F, 0x00},

    {0x12E2, 0xFF},
    {0x12E3, 0x3F},
    {0x12E4, 0x01},
    {0x12E5, 0x10},
    {0x12E8, 0xA5},
    {0x12E9, 0x5A},

    {_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeMiuU3_DDR3_13xx[]=
{
    {0x1308, 0xFF},
    {0x1309, 0xFF},
    {0x130A, 0x07},

    {0x1320, 0x29},

    {0x1346, 0x44},
    {0x1347, 0x44},
    {0x134E, 0x44},
    {0x134F, 0x44},

    {0x1360, 0x38},
    {0x1361, 0x23},
    {0x1362, 0x7F},

    {0x1372, 0x05},
    {0x137A, 0x07},

    {_END_OF_TBL_, _END_OF_TBL_},
};


#if (ENABLE_MEM_DQS_SELF_TEST)
            #define AMT_PRINTF(a, b)    printf(a, b)

    code U8 g_tAutoMemPreset[][2] =
{
    {0xC0, 0x00}, {0xC1, 0x00}, {0xC2, 0x00}, {0xC3, 0x00},
    {0xC4, 0x00}, {0xC5, 0x00},    {0xC6, 0x00}, {0xC7, 0x00},
    {0xC8, 0x00}, {0xC9, 0x00}, {0xCA, 0x00}, {0xCB, 0x00},
    {0xCC, 0x00}, {0xCD, 0x00}, {0xCE, 0x00}, {0xCF, 0x00},
    {0xD8, 0x00}, {0xD9, 0x00}, // Disable miu protect

    //{0xE2, 0x00}, {0xE3, 0x00}, // test base address (1K bytes)
    {0xE4, 0x01}, {0xE5, 0x10}, {0xE6, 0x00}, // test lenth(8 bytes)
    {0xE7, 0x00}, // test data mask
    {0xE8, 0x5A}, {0xE9, 0xA5}, // test data
    {0x30, 0x44}, {0x31, 0x44}
};

    code U8 g_tMode[] =
{
    0x03, 0x05, 0x07, 0x09, 0x0B
};

void MDrv_MFC_AutoMemPhase(void)
{
     U8 i, j, k;
     U8 ucHiBytePhase, ucLoBytePhase, ucHiCnt, ucLoCnt;
     U8 ucPhase, ucData;

     // Preset the memory table
     for (i=0; i<sizeof(g_tAutoMemPreset)/2; i++)
          MDrv_MFC_WriteByte(0x1200+g_tAutoMemPreset[i][0], g_tAutoMemPreset[i][1]);

#if 1
     AMT_PRINTF("\r\nMem. preTest Start... %d", 0);
     AMT_PRINTF("\r\n0x%x ", MDrv_MFC_Read2Bytes(0x1230));
    for (j=0; j<sizeof(g_tMode); j++)
    {
        MDrv_MFC_WriteByte(0x12E0, 0x00);
        AMT_PRINTF("%x", LONIBBLE(g_tMode[j]));
        MDrv_MFC_WriteByte(0x12E0, g_tMode[j]);
        k = 0;
        while(1)
        {
            k++;
            mfcSleepMs(1);
            ucData = MDrv_MFC_ReadByte(0x12E1);
            if(_bit7_(ucData))
                break;
            if(k>250)
                break;
        }

        if (!_bit7_(ucData) || _bit6_(ucData))
        {
            AMT_PRINTF("=[%x] NG", ucData);
             AMT_PRINTF("\r\nMem. preTest Fail...%d", 0);
            MDrv_MFC_WriteByte(0x12E0, 0x00);
            return;
        }
    }
     AMT_PRINTF("\r\nMem. preTest Pass...%d", 0);
#endif

     AMT_PRINTF("\r\nMem. Test Start...%d", 0);
        if ((gmfcSysInfo.u16MFCMemoryType &(PICS_MASK|MEM_BIT_MASK)) == (_1PICS|_32BITS)
            || (gmfcSysInfo.u16MFCMemoryType &(PICS_MASK|MEM_BIT_MASK)) == (_2PICS|_16BITS))
   {
       ucHiBytePhase = 0;
    ucLoBytePhase = 0;
    ucHiCnt = 0;
    ucLoCnt = 0;

     for (i=0; i<32; i++)
    {
        ucPhase = (i<16)?(0x40+i):(0x04+((i-16)<<4));
        MDrv_MFC_WriteByte(0x1230, ucPhase);
        //AMT_PRINTF((ucPhase<0x10)?("\r\n0x1230[0%x]:"):("\r\n0x1230[%x]:"), ucPhase);//j081016

        for (j=0; j<sizeof(g_tMode); j++)
        {
            MDrv_MFC_WriteByte(0x12E0, 0x00);
            //AMT_PRINTF("%x", LONIBBLE(g_tMode[j]));
            MDrv_MFC_WriteByte(0x12E0, g_tMode[j]);
            k = 0;
            while(1)
            {
                k++;
                mfcSleepMs(1);
                ucData = MDrv_MFC_ReadByte(0x12E1);
                if(_bit7_(ucData))
                    break;
                if(k>250)
                    break;
            }

            if (!_bit7_(ucData) || _bit6_(ucData))
            {
                AMT_PRINTF("=[%x] NG", ucData);
                break;
            }
        }

        if (_bit7_(ucData) && !_bit6_(ucData))
        {
            if (i<16)
            {
                ucLoCnt++;
                ucLoBytePhase += (ucPhase&0x0F);
            }
            else
            {
                ucHiCnt++;
                ucHiBytePhase += (ucPhase&0xF0)>>4;
            }
        }
    }

    ucHiBytePhase = (ucHiBytePhase/ucHiCnt)<<4;
    ucLoBytePhase = (ucLoBytePhase/ucLoCnt);
    MDrv_MFC_WriteByte(0x1230, ucHiBytePhase+ucLoBytePhase);
    ///AMT_PRINTF("\r\n0x1230 Optimum Phase[%x]", ucHiBytePhase+ucLoBytePhase);
    }

       ucHiBytePhase = 0;
    ucLoBytePhase = 0;
    ucHiCnt = 0;
    ucLoCnt = 0;

     for (i=0; i<32; i++)
    {
        ucPhase = (i<16)?(0x40+i):(0x04+((i-16)<<4));
        MDrv_MFC_WriteByte(0x1231, ucPhase);
        ///AMT_PRINTF((ucPhase<0x10)?("\r\n0x1231[0%x]:"):("\r\n0x1231[%x]:"), ucPhase); //j081016

        for (j=0; j<sizeof(g_tMode); j++)
        {
            MDrv_MFC_WriteByte(0x12E0, 0x00);
            ///AMT_PRINTF("%x", LONIBBLE(g_tMode[j]));
            MDrv_MFC_WriteByte(0x12E0, g_tMode[j]);
            k = 0;
            while(1)
            {
                k++;
                mfcSleepMs(1);
                ucData = MDrv_MFC_ReadByte(0x12E1);
                if(_bit7_(ucData))
                    break;
                if(k>250)
                    break;
            }

            if (!_bit7_(ucData) || _bit6_(ucData))
            {
                AMT_PRINTF("=[%x] NG", ucData);
                break;
            }
        }

        if (_bit7_(ucData) && !_bit6_(ucData))
        {
            if (i<16)
            {
                ucLoCnt++;
                ucLoBytePhase += (ucPhase&0x0F);
            }
            else
            {
                ucHiCnt++;
                ucHiBytePhase += (ucPhase&0xF0)>>4;
            }
        }
    }

    ucHiBytePhase = (ucHiBytePhase/ucHiCnt)<<4;
    ucLoBytePhase = (ucLoBytePhase/ucLoCnt);
    MDrv_MFC_WriteByte(0x1231, ucHiBytePhase+ucLoBytePhase);
    ///AMT_PRINTF("\r\n0x1231 Optimum Phase[%x]", ucHiBytePhase+ucLoBytePhase);//j081016
    MDrv_MFC_WriteByte(0x12E0, 0x00);
    AMT_PUTSTR("\r\nMem. Test Finish...");
}
#endif
//#include "Uart.h"
void MDrv_MFC_SetMiuSSC(U16 u16KHz, U16  u16Percent, BOOL bEnable)
{
    // SPAN value, recommend value is 20KHz ~ 30KHz
    // STEP percent value, recommend is under 3%
    U8  u8DDFM, u8DDRPLL_LOOP_DIV_1, u8DDRPLL_LOOP_DIV_2,
        u8DDRPLL_INPUT_DIV_1, u8DDRPLL_INPUT_DIV_2, u8temp;
    U16  u16Span, u16Step, u16DDFSET, u16MemoryClk;

    //if((u16KHz<200)||(u16KHz>300)) u16KHz = 220;
    //if(u16Percent > 300) u16Percent = 135;
    if(bEnable)
    {
        //DDFM

        // DIV_1
        u8temp = MDrv_MFC_ReadByte(0x1225);
        u8DDRPLL_INPUT_DIV_1 = 0x01<<((u8temp&0x30)>>4);
        u8DDRPLL_LOOP_DIV_1 =  0x01<<((u8temp&0xc0)>>6);
        //printf("\n\r\n\r u8DDRPLL_LOOP_DIV_1=[0x%x]  ", u8DDRPLL_LOOP_DIV_1);
        //printf("\n\r\n\r u8DDRPLL_INPUT_DIV_1=[0x%x]  ", u8DDRPLL_INPUT_DIV_1);

        // DIV_2
        u8temp = MDrv_MFC_ReadByte(0x1222);
        u8DDRPLL_INPUT_DIV_2 = (u8temp)? u8temp: 1;
        u8temp = MDrv_MFC_ReadByte(0x1223);
        u8DDRPLL_LOOP_DIV_2 = (u8temp)? u8temp: 1;
        //printf("\n\r\n\r u8DDRPLL_LOOP_DIV_2=[0x%x]  ", u8DDRPLL_LOOP_DIV_2);
       // printf("\n\r\n\r u8DDRPLL_INPUT_DIV_2=[0x%x]  ", u8DDRPLL_INPUT_DIV_2);

        #if 0
        // DDFM
        u8DDFM = (u8DDRPLL_LOOP_DIV_1 / u8DDRPLL_LOOP_DIV_2)/
                 (u8DDRPLL_INPUT_DIV_1 / u8DDRPLL_INPUT_DIV_2);
        #else
        u8DDFM=(u8DDRPLL_LOOP_DIV_1*u8DDRPLL_LOOP_DIV_2)/(u8DDRPLL_INPUT_DIV_1*u8DDRPLL_INPUT_DIV_2);
        #endif
        //printf("\n\r\n\r u8DDFM=[0x%x]  ", u8DDFM);

        // DDFSET
        u8temp = MDrv_MFC_ReadByte(0x1220);
        u16DDFSET = MDrv_MFC_ReadByte(0x1221)&0x03;
        u16DDFSET = (u16DDFSET << 8) | u8temp ;
        //printf("\n\r\n\r u16DDFSET=[0x%x]  ", u8DDFM);

        u16MemoryClk = ((U32)u8DDFM*216*128)/u16DDFSET;

        //printf("\n\r\n\r FDDR=[0x%x]  ", u16MemoryClk);

        u16Span = ((U32)u16MemoryClk*10000)/(4*u8DDFM*u16KHz);
        u16Step = ((U32)u16Percent*u16DDFSET*1024*2)/((U32)u16Span*2*10000);

        MDrv_MFC_WriteByte(0x132A, (U8)u16Span);//Write SPAN 10 bits
        MDrv_MFC_WriteByte(0x132B, (U8)((u16Span&0x0300)>>8));
        MDrv_MFC_WriteByte(0x1328, (U8)u16Step);//Write Step 8 bits
        MDrv_MFC_WriteBit(0x1329, _ENABLE, _BIT7);
        //printf("\n\r\n\r 0x132A,0x132B u16Step=[0x%x]  ", u16Step);
        //printf(" 0x1328 u16Span=[0x%x]", u16Span);
    }
    else
    {
        MDrv_MFC_WriteBit(0x1329, _DISABLE, _BIT7);
    }
}

void MDrv_MFC_MIUPLL_Reset(void)
{
        MDrv_MFC_WriteBit(0x1221, 0, _BIT7);
     MDrv_MFC_WriteBit(0x1221, 0, _BIT6);
        MDrv_MFC_WriteBit(0x1221, 0, _BIT5);

  /*
        MDrv_MFC_WriteByteMask(0x1221, 0, _BIT6|_BIT5);
     MDrv_MFC_WriteBit(0x1221, 0, _BIT7);
  */

}


void MDrv_MFC_SetMclk_MIU(void)
{
    U32 Temp1,Temp2;

    //Set Mclk
    if ((gmfcSysInfo.u16MFCMemoryType  & DDR_MASK) == _DDR2)  //DDR2
    {
        if (gmfcSysInfo.u16MFCMemoryClk == 800)
        {
            Temp1 = 0x019e;
            Temp2 = 0x4000;
        }
        else if (gmfcSysInfo.u16MFCMemoryClk == 1300)
        {
            Temp1 = 0x01f1;
            Temp2 = 0x8000;
        }
        else //(gmfcSysInfo.u16MFCMemoryClk == 1000)  //1G  Normal
        {
            Temp1 = 0x026e;
            Temp2 = 0x8000;
        }
    }
    else //DDR3
    {
        if(gmfcSysInfo.u16MFCMemoryClk ==1600)  //1.6G
        {
            Temp1 = 0x019e;
            Temp2 = 0x8000;
        }
        else if (gmfcSysInfo.u16MFCMemoryClk == 1400)  //1.4G  Normal
        {
            Temp1 = 0x01d9;
            if(S7M)
            {
                Temp2 = 0x8002;
            }
            else
            {
                Temp2 = 0x8000;
            }
        }
        else if(gmfcSysInfo.u16MFCMemoryClk == 1300)  //1.3G
        {
            if(S7M)
            {
                Temp1 = 0x0200;
                Temp2 = 0x8002;
            }
            else
            {
                Temp1 = 0x01fe;
                Temp2 = 0x8000;
            }
        }
        else if(gmfcSysInfo.u16MFCMemoryClk == 1200)  //1.2G
        {
            Temp1 = 0x0228;
            Temp2 = 0x8000;
        }
        else  //1G  Normal
        {
            if(S7M)
            {
                Temp1 = 0x026e;
                Temp2 = 0x8002;
            }
            else
            {
            Temp1 = 0x02e6;
            Temp2 = 0x8000;
        }
    }
    }
    MDrv_MFC_MIUPLL_Reset();
    MDrv_MFC_Write2Bytes(0x1220, Temp1);
    MDrv_MFC_Write2Bytes(0x1224, Temp2);
    //printk("MDrv_MFC_SetMclk_MIU()\n");
}


#ifndef ENABLE_MIU_SSC
#define ENABLE_MIU_SSC 0
#define MIU_SSC_SPAN_DEFAULT        200
#define MIU_SSC_STEP_DEFAULT        100
#endif

void MDrv_MFC_InitializeMiu(void)
{
    U8 ucIndex;

    for( ucIndex = 0; ucIndex <= 64; ucIndex ++ )
        MDrv_MFC_WriteByte(0x12C0+ucIndex, 0x00);

    for( ucIndex = 0; ucIndex <= 128; ucIndex ++ )
        MDrv_MFC_WriteByte(0x1300+ucIndex, 0x00);

    if ((gmfcSysInfo.u16MFCMemoryType  & DDR_MASK) == _DDR2)  //DDR2
    {
        MDrv_MFC_WriteRegsTbl(0x1200, tInitializeMiuU3_DDR2_12xx); // initialize all of bank
        MDrv_MFC_WriteRegsTbl(0x1300, tInitializeMiuU3_DDR2_13xx); // initialize all of bank
           #ifdef Packet_QFP
            MDrv_MFC_WriteByte(0x1300, 0x01); //QFP
           #else
            MDrv_MFC_WriteByte(0x1300, 0x00); //BGA (default)
           #endif
    }
    else //DDR3
    {
        MDrv_MFC_WriteRegsTbl(0x1200, tInitializeMiuU3_DDR3_12xx); // initialize all of bank
        if(S7M)
        {
            MDrv_MFC_WriteByte(0x1212, 0x46);
        }
        else
        {
            MDrv_MFC_WriteByte(0x1212, 0x06);
        }
        MDrv_MFC_WriteRegsTbl(0x1300, tInitializeMiuU3_DDR3_13xx); // initialize all of bank

        if(gmfcSysInfo.u8ChipRevision==U02)
        {
            MDrv_MFC_WriteByte(0x1230, 0x66);
            MDrv_MFC_WriteByte(0x1372, 0x04);
            if(S7M)
            {
                MDrv_MFC_WriteByte(0x1346, 0x24);
                MDrv_MFC_WriteByte(0x134E, 0x24);
            }
            else
            {
                MDrv_MFC_WriteByte(0x1346, 0x04);
                MDrv_MFC_WriteByte(0x134E, 0x04);
            }
        }

    }
    MDrv_MFC_SetMclk_MIU();

    if(gmfcSysInfo.u8ChipRevision==U02)
    {
        MDrv_MFC_WriteByte(0x1365, 0xF2);

        if(MDrv_MFC_ReadByte(0x1372)==0x03)
            MDrv_MFC_Write2Bytes(0x137E, 0x0300);
        else if(MDrv_MFC_ReadByte(0x1372)==0x05)
            MDrv_MFC_Write2Bytes(0x137E, 0x0500);
        else if(MDrv_MFC_ReadByte(0x1372)==0x04)
            MDrv_MFC_Write2Bytes(0x137E, 0x0400);
    }  //U01
    else
        MDrv_MFC_WriteByte(0x1365, 0xE2);


    //need process
    MDrv_MFC_Write2Bytes(0x121E, 0x0c01);
    MDrv_MFC_Write2Bytes(0x121E, 0x0c00);
    MDrv_MFC_Write2Bytes(0x1200, 0x0000);
    mfcSleepMs(1);
    MDrv_MFC_Write2Bytes(0x1200, 0x0008);
    mfcSleepMs(1);
    MDrv_MFC_Write2Bytes(0x1200, 0x000c);
    mfcSleepMs(1);
    MDrv_MFC_Write2Bytes(0x1200, 0x000e);
    mfcSleepMs(1);
    MDrv_MFC_Write2Bytes(0x1200, 0x000f);
    mfcSleepMs(1);
    MDrv_MFC_Write2Bytes(0x1200, 0x001f);

        MDrv_MFC_SetMiuSSC(MIU_SSC_SPAN_DEFAULT, MIU_SSC_STEP_DEFAULT, ENABLE_MIU_SSC);

    //printf("MDrv_MFC_InitializeMiu() %d\n", 0);
}

#endif

