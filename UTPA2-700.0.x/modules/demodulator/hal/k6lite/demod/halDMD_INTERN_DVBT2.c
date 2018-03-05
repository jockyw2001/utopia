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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
/// @file INTERN_DVBT2.c
/// @brief INTERN_DVBT2 DVBT2
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

#define _INTERN_DVBT2_C_
#ifdef MSOS_TYPE_LINUX
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
//#include "apiPWS.h"

#include "MsTypes.h"
//#include "BinInfo.h"
#include "drvDMD_VD_MBX.h"
#include "drvDMD_INTERN_DVBT2.h"
#include "halDMD_INTERN_DVBT2.h"
#include "halDMD_INTERN_common.h"

extern void *memcpy(void *destination, const void *source, size_t num);

#define TEST_EMBEDED_DEMOD 0
//U8 load_data_variable=1;
//-----------------------------------------------------------------------
#define BIN_ID_INTERN_DVBT2_DEMOD BIN_ID_INTERN_DVBT

#define	TDE_REG_BASE  0x2400
#define	DIV_REG_BASE  0x2500
#define TR_REG_BASE   0x2600
#define FTN_REG_BASE  0x2700
#define FTNEXT_REG_BASE 0x2800



#if 0//ENABLE_SCAN_ONELINE_MSG
#define DBG_INTERN_DVBT2_ONELINE(x)  x
#else
#define DBG_INTERN_DVBT2_ONELINE(x) //  x
#endif

#ifdef MS_DEBUG
#define DBG_INTERN_DVBT2(x) x
#define DBG_GET_SIGNAL(x)  x
#define DBG_INTERN_DVBT2_TIME(x) x
#define DBG_INTERN_DVBT2_LOCK(x)  x
#else
#define DBG_INTERN_DVBT2(x) //x
#define DBG_GET_SIGNAL(x)  //x
#define DBG_INTERN_DVBT2_TIME(x) // x
#define DBG_INTERN_DVBT2_LOCK(x)  //x
#endif
#define DBG_DUMP_LOAD_DSP_TIME 0

#define INTERN_DVBT2_TS_SERIAL_INVERSION         0
#define INTERN_DVBT2_TS_PARALLEL_INVERSION       1
#define INTERN_DVBT2_DTV_DRIVING_LEVEL           1
#define INTERN_DVBT2_INTERNAL_DEBUG              1

#define SIGNAL_LEVEL_OFFSET     0.00
#define TAKEOVERPOINT           -59.0
#define TAKEOVERRANGE           0.5
#define LOG10_OFFSET            -0.21
#define INTERN_DVBT2_USE_SAR_3_ENABLE 0
#define INTERN_DVBT2_GET_TIME msAPI_Timer_GetTime0()


#if 0//(FRONTEND_TUNER_TYPE==MSTAR_AVATAR2)
#define TUNER_VPP  2
#define IF_AGC_VPP 2
#else
#define TUNER_VPP  1
#define IF_AGC_VPP 2
#endif

#if (TUNER_VPP == 1)
#define ADC_CH_I_PGA_GAIN_CTRL      0x5 // gain = 14.0/5.0
#elif (TUNER_VPP == 2)  // For Avatar tuner,ADC peak to peak voltage is 1 V
#define ADC_CH_I_PGA_GAIN_CTRL      0x5 // gain = 14.0/14.0
#endif

/*BEG INTERN_DVBT2_DSPREG_TABLE*/
#define DVBT2_FS     24000

// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL               0x04
// FC: FC = FS = 5000 = 0x1388     (5.0MHz IF)
#define T2_FC_L_VAL            0x88    // 5.0M
#define T2_FC_H_VAL            0x13
#define T2_TS_SERIAL_VAL        0x00
#define T2_TS_CLK_RATE_VAL      0x06
#define T2_TS_OUT_INV_VAL       0x00
#define T2_TS_DATA_SWAP_VAL     0x00
#define T2_IF_AGC_INV_PWM_EN_VAL 0x00
#define T2_LITE_VAL 0x00
#define T2_AGC_REF_VAL 0x40

//#define DVBT2_BER_TH_HY 0.1

/*END INTERN_DVBT2_DSPREG_TABLE*/
//-----------------------------------------------------------------------
/****************************************************************
*Local Variables                                                                                              *
****************************************************************/
static MS_BOOL bFECLock=0;
static MS_BOOL bP1Lock = 0;
static MS_U32 u32ChkScanTimeStart = 0;
static MS_U32 u32FecFirstLockTime=0;
static MS_U32 u32FecLastLockTime=0;
//static float fLDPCBerFiltered=-1;
//static float fBerFilteredDVBT2 = -1.0;

//Global Variables
//S_CMDPKTREG gsCmdPacket;
//U8 gCalIdacCh0, gCalIdacCh1;
extern MS_U32  u32DMD_DVBT2_DRAM_START_ADDR;
extern MS_U32  u32DMD_DVBT2_EQ_START_ADDR;
extern MS_U32  u32DMD_DVBT2_TDI_START_ADDR;
extern MS_U32  u32DMD_DVBT2_DJB_START_ADDR;
extern MS_U32  u32DMD_DVBT2_FW_START_ADDR;

#ifdef INTERN_DVBT2_LOAD_FW_FROM_CODE_MEMORY
MS_U8 INTERN_DVBT2_table[] = {
    #include "fwDMD_INTERN_DVBT2.dat"
};

#endif
/*
static DMD_T2_SSI_DBM_NORDIGP1 dvbt2_ssi_dbm_nordigp1[] =
{
    {_T2_QPSK, _T2_CR1Y2, -95.7},
    {_T2_QPSK, _T2_CR3Y5, -94.4},
    {_T2_QPSK, _T2_CR2Y3, -93.6},
    {_T2_QPSK, _T2_CR3Y4, -92.6},
    {_T2_QPSK, _T2_CR4Y5, -92.0},
    {_T2_QPSK, _T2_CR5Y6, -91.5},

    {_T2_16QAM, _T2_CR1Y2, -90.8},
    {_T2_16QAM, _T2_CR3Y5, -89.1},
    {_T2_16QAM, _T2_CR2Y3, -87.9},
    {_T2_16QAM, _T2_CR3Y4, -86.7},
    {_T2_16QAM, _T2_CR4Y5, -85.8},
    {_T2_16QAM, _T2_CR5Y6, -85.2},

    {_T2_64QAM, _T2_CR1Y2, -86.9},
    {_T2_64QAM, _T2_CR3Y5, -84.6},
    {_T2_64QAM, _T2_CR2Y3, -83.2},
    {_T2_64QAM, _T2_CR3Y4, -81.4},
    {_T2_64QAM, _T2_CR4Y5, -80.3},
    {_T2_64QAM, _T2_CR5Y6, -79.7},

    {_T2_256QAM, _T2_CR1Y2, -83.5},
    {_T2_256QAM, _T2_CR3Y5, -80.4},
    {_T2_256QAM, _T2_CR2Y3, -78.6},
    {_T2_256QAM, _T2_CR3Y4, -76.0},
    {_T2_256QAM, _T2_CR4Y5, -74.4},
    {_T2_256QAM, _T2_CR5Y6, -73.3},
    {_T2_QAM_UNKNOWN, _T2_CR_UNKNOWN, 0.0}
};
*/

/*
static float dvbt2_ssi_dbm_nordigp1[][6] =
{
    { -95.7, -94.4, -93.6, -92.6, -92.0, -91.5},
    { -90.8, -89.1, -87.9, -86.7, -85.8, -85.2},
    { -86.9, -84.6, -83.2, -81.4, -80.3, -79.7},
    { -83.5, -80.4, -78.6, -76.0, -74.4, -73.3},
};

// cr, 3/5(1),	2/3(2), 3/4 (3)
float fT2_SSI_formula[][12]=
{
	{1.0/5,  97.0,	3.0/2,	82.0, 16.0/5,  50.0, 29.0/10.0, 21.0, 18.0/15, 3.0, 3.0/5, 0.0}, // CR3/5
	{2.0/3,  95.0,	9.0/5,	77.0, 17.0/5,  43.0, 14.0/5.0,	15.0, 13.0/15, 2.0, 2.0/5, 0.0}, // CR2/3
	{1.0/2,  93.0, 19.0/10, 74.0, 31.0/10, 43.0, 22.0/10.0, 21.0, 18.0/15, 3.0, 3.0/5, 0.0}, // CR3/4
};
*/

//static void INTERN_DVBT2_SignalQualityReset(void);
MS_BOOL INTERN_DVBT2_Show_Demod_Version(void);

#if 0
static void INTERN_DVBT2_SignalQualityReset(void)
{
    u32FecFirstLockTime=0;
    fLDPCBerFiltered=-1;
}
#endif

MS_BOOL INTERN_DVBT2_DSPReg_Init(const MS_U8 *u8DVBT2_DSPReg,  MS_U8 u8Size)
{
    MS_U8   idx = 0, u8RegRead = 0, u8RegWrite = 0, u8Mask = 0;
    MS_BOOL status = TRUE;
    MS_U16 u16DspAddr = 0;

    DBG_INTERN_DVBT2(printf("INTERN_DVBT2_DSPReg_Init\n"));

    //for (idx = 0; idx<sizeof(INTERN_DVBT_DSPREG); idx++)
    //    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(idx, INTERN_DVBT_DSPREG[idx]);
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_BW, T2_BW_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_FC_L, T2_FC_L_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_FC_H, T2_FC_H_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_TS_SERIAL, T2_TS_SERIAL_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }
    //if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_TS_CLK_RATE, T2_TS_CLK_RATE_VAL) != TRUE)
    //{
    //    printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    //}
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_TS_OUT_INV, T2_TS_OUT_INV_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_TS_DATA_SWAP, T2_TS_DATA_SWAP_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }
    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_LITE, T2_LITE_VAL) != TRUE)
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }

    if(MDrv_SYS_DMD_VD_MBX_WriteDSPReg((MS_U32)E_DMD_T2_AGC_REF, T2_AGC_REF_VAL) != TRUE)		//brown:0x40->agc_ref
    {
        printf("INTERN_DVBT2_DSPReg_Init NG\n"); return FALSE;
    }

    if (u8DVBT2_DSPReg != NULL)
    {
        /*temp solution until new dsp table applied.*/
        // if (INTERN_DVBT2_DSPREG[E_DMD_DVBT_PARAM_VERSION] == u8DVBT_DSPReg[0])
        if (u8DVBT2_DSPReg[0] >= 1)
        {
            u8DVBT2_DSPReg+=2;
            for (idx = 0; idx<u8Size; idx++)
            {
                u16DspAddr = *u8DVBT2_DSPReg;
                u8DVBT2_DSPReg++;
                u16DspAddr = (u16DspAddr) + ((*u8DVBT2_DSPReg)<<8);
                u8DVBT2_DSPReg++;
                u8Mask = *u8DVBT2_DSPReg;
                u8DVBT2_DSPReg++;
                status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16DspAddr, &u8RegRead);
                u8RegWrite = (u8RegRead & (~u8Mask)) | ((*u8DVBT2_DSPReg) & (u8Mask));
                u8DVBT2_DSPReg++;
                DBG_INTERN_DVBT2(printf("DSP addr:%x mask:%x read:%x write:%x\n", u16DspAddr, u8Mask, u8RegRead, u8RegWrite));
                status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(u16DspAddr, u8RegWrite);
            }
        }
        else
        {
            printf("FATAL: parameter version incorrect\n");
        }
    }

    return status;
}

/***********************************************************************************
  Subject:    SoftStop
  Function:   INTERN_DVBT2_SoftStop
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/

MS_BOOL INTERN_DVBT2_SoftStop ( void )
{
    MS_U16     u8WaitCnt=0;
    if (HAL_DMD_RIU_ReadByte(MBRegBase + 0x00))
    {
        printf(">> MB Busy!\n");
        return FALSE;
    }

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set read mode

    HAL_DMD_RIU_WriteByte(0x103483, 0x02);                         // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    while(HAL_DMD_RIU_ReadByte(MBRegBase + 0x00)!= 0x5A)           // wait MB_CNTL set done
    {
        if (u8WaitCnt++ >= 0xFFF)
        {
            printf(">> DVBT2 SoftStop Fail!\n");
            return FALSE;
        }
    }

    //HAL_DMD_RIU_WriteByte(0x103480, 0x01);                         // reset VD_MCU
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear
    return TRUE;
}

MS_BOOL INTERN_DVBT2_SoftReset ( void )
{
    MS_BOOL bRet=TRUE;
    //MS_U8 u8Data, fdp_fifo_done, djb_fifo_done, tdi_fifo_done;
    MS_U8 u8Data = 0, fdp_fifo_done = 0, tdi_fifo_done = 0;
    MS_U8 u8_timeout = 0;

    DBG_INTERN_DVBT2(printf(" @INTERN_DVBT2_SoftReset\n"));

    //stop FSM_EN
    HAL_DMD_RIU_WriteByte(MBRegBase + (0x0e)*2, 0x00);   // FSM_EN

    MsOS_DelayTask(5);

    bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE+0x02, &u8Data);
    DBG_INTERN_DVBT2(printf("@@@TOP_RESET:0x%x\n", u8Data));
    // MIU hold function
    if((u8Data & 0x20) == 0x00)
    {
        // mask miu service with fdp, djb, tdi
        //fdp 0x17 [12] reg_fdp_fifo_stop=1'b1
        bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2FDP_REG_BASE+(0x17*2)+1, &u8Data);
        bRet = MDrv_SYS_DMD_VD_MBX_WriteReg(T2FDP_REG_BASE+(0x17*2)+1, (u8Data|0x10));
        // [8] reg_fdp_load, fdp register dynamic change protection, 1->load register
        bRet = MDrv_SYS_DMD_VD_MBX_WriteReg(T2FDP_REG_BASE+0xff, 0x10);
        //bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2FDP_REG_BASE+(0x17*2)+1, &u8Data);
        //printf("@@@@@@ DVBT2 [reg_fdp_fifo_stop]=0x%x\n", u8Data);
        //djb 0x65 [0] reg_stop_mu_request
        bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2DJB_REG_BASE+(0x65*2), &u8Data);
        bRet = MDrv_SYS_DMD_VD_MBX_WriteReg(T2DJB_REG_BASE+(0x65*2), (u8Data|0x01));
        //bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2DJB_REG_BASE+(0x65*2), &u8Data);
        //printf("@@@@@@ DVBT2 [reg_stop_mu_request]=0x%x\n", u8Data);
        //snr 0x23 [8] reg_tdi_miu_off
        bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2SNR_REG_BASE+(0x23*2)+1, &u8Data);
        bRet = MDrv_SYS_DMD_VD_MBX_WriteReg(T2SNR_REG_BASE+(0x23*2)+1, (u8Data|0x01));
        //bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2SNR_REG_BASE+(0x23*2)+1, &u8Data);
        //printf("@@@@@@ DVBT2 [reg_tdi_miu_off]=0x%x\n", u8Data);
        // ---------------------------------------------
        // Wait MIU mask or timeout!
        // DVBT2_TIMER_INT[ 7:0] : indicator of the selected Timer's max count(15:8) (r)
        // DVBT2_TIMER_INT[11:8] : timer3~timer0 interrupt (r)
        // ---------------------------------------------
        //fdp 0x18 [2] reg_fdp_fifo_req_done
        //djb 0x65 [8] reg_miu_req_terminate_done
        //tdi 0x23 [9] reg_tdi_miu_off_done
        do
        {
            // Wait MIU mask done or timeout!
            bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2FDP_REG_BASE+(0x18*2), &u8Data);
            fdp_fifo_done = u8Data & 0x04;
            //bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2DJB_REG_BASE+(0x65*2)+1, &u8Data);
            //djb_fifo_done = u8Data & 0x01;
            bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(T2SNR_REG_BASE+(0x23*2)+1, &u8Data);
            tdi_fifo_done = u8Data & 0x02;

            u8_timeout++;
        }
        //while(((fdp_fifo_done != 0x04)||(djb_fifo_done != 0x01)||(tdi_fifo_done != 0x02))
        while(((fdp_fifo_done != 0x04)||(tdi_fifo_done != 0x02))
            && u8_timeout != 0x7f);

        //printf(">> DVBT2 fdp_fifo_done=%d, djb_fifo_done=%d, tdi_fifo_done=%d \n", fdp_fifo_done, djb_fifo_done, tdi_fifo_done);
        printf(">> DVBT2 [fdp_fifo_done]=%d, [tdi_fifo_done]=%d \n", fdp_fifo_done, tdi_fifo_done);

        MsOS_DelayTask(2);

        if(u8_timeout == 0x7f)
        {
            printf(">> DVBT2 MIU hold function Fail!\n");
            //return FALSE;
        }
        else
        {
            printf(">> DVBT2 MIU hold function done!!\n");
        }
    }
    else
        printf(">> No need DVBT2 MIU hold function!!\n");

    // demod_top reset
    bRet = MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE+0x02, &u8Data);
    bRet = MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE+0x02, (u8Data|0x20));

    MsOS_DelayTask(1);

    bRet = MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE+0x02, (u8Data&(~0x20)));

    DBG_INTERN_DVBT2(printf("@INTERN_DVBT2_SoftReset done!!\n"));

    return bRet;
}


/***********************************************************************************
  Subject:    Reset
  Function:   INTERN_DVBT2_Reset
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
extern void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void);
MS_BOOL INTERN_DVBT2_Reset ( void )
{
    DBG_INTERN_DVBT2(printf(" @INTERN_DVBT2_reset\n"));

    DBG_INTERN_DVBT2_TIME(printf("INTERN_DVBT2_Reset, t = %ld\n",MsOS_GetSystemTime()));

    INTERN_DVBT2_SoftStop();


    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x01);     // reset DMD_MCU
    MsOS_DelayTask(5);
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);     // clear MB_CNTL

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);
    MsOS_DelayTask(5);

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);

    bFECLock = FALSE;
    bP1Lock = FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();
    return TRUE;
}

/***********************************************************************************
  Subject:    Exit
  Function:   INTERN_DVBT2_Exit
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBT2_Exit ( void )
{
    DBG_INTERN_DVBT2(printf(" @INTERN_DVBT2_Exit\n"));



    //diable clk gen
    //HAL_DMD_RIU_WriteByte(0x103314, 0x01);   // reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    //HAL_DMD_RIU_WriteByte(0x103315, 0x01);   // reg_ckg_dvbtc_innc@0x0a[11:8]
/*
    HAL_DMD_RIU_WriteByte(0x10330a, 0x01);   // reg_ckg_atsc_adcd_sync@0x05[3:0] : ADCCLK
    HAL_DMD_RIU_WriteByte(0x10330b, 0x00);

    HAL_DMD_RIU_WriteByte(0x10330c, 0x01);   // reg_ckg_dvbtc_inner1x@0x06[3:0] : MPLLDIV10/4=21.5MHz
    HAL_DMD_RIU_WriteByte(0x10330d, 0x01);   // reg_ckg_dvbtc_inner2x@0x06[11:8]: MPLLDIV10/2=43.2MHz

    HAL_DMD_RIU_WriteByte(0x10330e, 0x01);   // reg_ckg_dvbtc_inner4x@0x07[3:0] : MPLLDIV10=86.4MHz
    HAL_DMD_RIU_WriteByte(0x10330f, 0x00);

    HAL_DMD_RIU_WriteByte(0x103310, 0x01);   // reg_ckg_dvbtc_outer1x@0x08[3:0] : MPLLDIV10/2=43.2MHz
    HAL_DMD_RIU_WriteByte(0x103311, 0x01);   // reg_ckg_dvbtc_outer2x@0x08[11:8]: MPLLDIV10=86.4MHz

    HAL_DMD_RIU_WriteByte(0x103312, 0x01);   // dvbt_t:0x0000, dvb_c: 0x0004
    HAL_DMD_RIU_WriteByte(0x103313, 0x00);

    HAL_DMD_RIU_WriteByte(0x103314, 0x01);   // reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    HAL_DMD_RIU_WriteByte(0x103315, 0x01);   // reg_ckg_dvbtc_innc@0x0a[11:8]

    HAL_DMD_RIU_WriteByte(0x103316, 0x01);   // reg_ckg_dvbtc_eq8x@0x0b[3:0] : MPLLDIV3/2=144MHz
    HAL_DMD_RIU_WriteByte(0x103317, 0x01);   // reg_ckg_dvbtc_eq@0x0b[11:8] : MPLLDIV3/16=18MHz

    HAL_DMD_RIU_WriteByte(0x103318, 0x11);   // reg_ckg_dvbtc_sram0~3@0x0c[13:0]
    HAL_DMD_RIU_WriteByte(0x103319, 0x11);

    HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
    HAL_DMD_RIU_WriteByte(0x103309, 0x05);   // reg_ckg_dvbtc_ts@0x04

    HAL_DMD_RIU_WriteByte(0x101E3E, 0x00);   // DVBT = BIT1 clear
*/
    return INTERN_DVBT2_SoftStop();
}
/*
MS_BOOL INTERN_DVBT2_Load2Sdram(MS_U8 *u8_ptr, MS_U16 data_length)
{

    DBG_INTERN_DVBT2(printf("INTERN_DVBT2_Load2Sdram, len=0x%x, \n",data_length));
    MS_U8 addrhi, addrlo;
    int i, j, k, old_i=0;
    int sdram_start_addr = 0;//1024 >> 2; //StrToInt(ed_sdram_start->Text)>>2; // 4KB alignment

    //I2C_CH_Exit();			// exit CH4
    //I2C_CH5_Reset();		// switch to CH5
    //MDrv_DMD_I2C_Channel_Change(5);
    //--------------------------------------------------------------------------
    //  Set xData map for DRAM
    //--------------------------------------------------------------------------

    //banknum = 0x1d; //dmdmcu51_xdmiu

    //set xData map upper and low bound for 64k DRAM window
    MDrv_SYS_DMD_VD_MBX_Write2Reg(XDMIU_REG_BASE, 0x63, 0x2020);
    if(SLAVE_I2CWrite16(banknum,0x63,0x2020)==false)
      { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }

    //set xData map offset for 64k DRAM window
    MDrv_SYS_DMD_VD_MBX_Write2Reg(XDMIU_REG_BASE, 0x64, 0x0000);
    if(SLAVE_I2CWrite16(banknum,0x64,0x0000)==false)
      { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }

    //set xData map upper and low bound for 4k DRAM window
    MDrv_SYS_DMD_VD_MBX_Write2Reg(XDMIU_REG_BASE, 0x65, 0x2420);
	if(SLAVE_I2CWrite16(banknum,0x65,0x2420)==false)
      { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }

    //set xData map offset for 4k DRAM window
    MDrv_SYS_DMD_VD_MBX_Write2Reg(XDMIU_REG_BASE, 0x66, sdram_start_addr);
    if(SLAVE_I2CWrite16(banknum,0x66,sdram_start_addr)==false)
      { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }

    //I2C_CH_Exit();			// exit CH5
    //EnterDebugMode(1);     // switch to CH1

    //enable xData map for DRAM
    MDrv_SYS_DMD_VD_MBX_Write2Reg(XDMIU_REG_BASE, 0x63, 0x0007);
    if(SLAVE_I2CWrite16(banknum,0x62,0x0007)==false)
      { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }


    for ( i = 0, j = SDRAM_BASE, k = sdram_start_addr + 0x01; i < size;)
    {
        if (j == SDRAM_BASE + 0x1000)
        {
            //I2C_CH_Exit();			// exit CH1
            //I2C_CH5_Reset();		// switch to CH5
            //set xData map offset for 4k DRAM window
            MDrv_SYS_DMD_VD_MBX_Write2Reg(XDMIU_REG_BASE, 0x66, k++);
            if(SLAVE_I2CWrite16(banknum,0x66,k++)==false)
              { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }
            j = SDRAM_BASE;

            //I2C_CH_Exit();			// exit CH5
            //EnterDebugMode(1);     // switch to CH1

        }

        addrhi = (j >> 8) & 0xff;
        addrlo = j & 0xff;

        if (i+EZUSB_Write_Buffer<size)
        {
            if(I2C_WriteBytes(addrhi,addrlo,in_buf+i,EZUSB_Write_Buffer)==FALSE)
              { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }

            j=j+EZUSB_Write_Buffer;
            i=i+EZUSB_Write_Buffer;
        }
        else
        {
            if(I2C_WriteBytes(addrhi,addrlo,in_buf+i,size-i)==FALSE)
              { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }

            i=size;
        }

        if ((i-old_i)>=2048)
        {
            ShowMCUDL_Progress(0,3*i,size);
            old_i=i;
        }
    }//end for


    FWDLRichEdit->Lines->Add(">SDRAM Down Load OK!");

    I2C_CH_Exit();			// exit CH1
    I2C_CH5_Reset();		// switch to CH5

    //--------------------------------------------------------------------------
    //  Release xData map for SDRAM
    //--------------------------------------------------------------------------

    if(SLAVE_I2CWrite16(banknum,0x62,0x0000)==false)
      { MessageBox(NULL,"Fail to write register!!","write register",MB_OK|MB_ICONERROR); return FALSE; }

}
*/
/***********************************************************************************
  Subject:    Load DSP code to chip
  Function:   INTERN_DVBT2_LoadDSPCode
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
static MS_BOOL INTERN_DVBT2_LoadDSPCode(void)
{
    MS_U8  u8data = 0x00;
    MS_U16 i;
    MS_U16 fail_cnt=0;
    //MS_U16  u16AddressOffset;
    MS_U32 u32VA_DramCodeAddr;

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    MS_U32 u32Time;
#endif


#ifndef INTERN_DVBT2_LOAD_FW_FROM_CODE_MEMORY
    BININFO BinInfo;
    MS_BOOL bResult;
    MS_U32 u32GEAddr;
    MS_U8 Data;
    MS_S8 op;
    MS_U32 srcaddr;
    MS_U32 len;
    MS_U32 SizeBy4K;
    MS_U16 u16Counter=0;
    MS_U8 *pU8Data;
#endif

#if 0
    if(HAL_DMD_RIU_ReadByte(0x101E3E))
    {
        printf("Warring! Reg[0x101E3E]=%d\n", HAL_DMD_RIU_ReadByte(0x101E3E));
        return FALSE;
    }
#endif

  //  MDrv_Sys_DisableWatchDog();


    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00,  0x01);        // reset VD_MCU
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01,  0x00);        // disable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x50);        // enable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x51);        // enable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte

    ////  Load code thru VDMCU_IF ////
    DBG_INTERN_DVBT2(printf(">Load Code...\n"));
//#ifdef INTERN_DVBT2_LOAD_FW_FROM_CODE_MEMORY
    //for ( i = 0; i < sizeof(INTERN_DVBT2_table); i++)
    //{
    //    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x0C, INTERN_DVBT2_table[i]); // write data to VD MCU 51 code sram
    //}
    if (sizeof(INTERN_DVBT2_table) < 0x8000)
    {
        printf("----->Bin file Size is not match...\n");
    }
    else
    {
        // load half code to SRAM
        for ( i = 0; i < 0x8000; i++)
        {
            HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x0C, INTERN_DVBT2_table[i]); // write data to VD MCU 51 code sram
        }
        DBG_INTERN_DVBT2(printf(">Load SRAM code done...\n"));


        if((u32DMD_DVBT2_FW_START_ADDR & 0x8000) != 0x8000)
        {
            printf(">DVB-T2 DRAM Start address is not correct!!\n");
        }
        else
        {
            // load another half code to SDRAM
            // VA = MsOS_PA2KSEG1(PA); //NonCache
            DBG_INTERN_DVBT2(printf(">>> DVBT2_FW_START_ADDR=0x%lx \n", u32DMD_DVBT2_FW_START_ADDR));
            u32VA_DramCodeAddr = MsOS_PA2KSEG1(u32DMD_DVBT2_FW_START_ADDR);
            memcpy((void*)(MS_VIRT)u32VA_DramCodeAddr, &INTERN_DVBT2_table[0x8000], sizeof(INTERN_DVBT2_table) - 0x8000);

            DBG_INTERN_DVBT2(printf(">Load DRAM code done...\n"));
        }
    }

//#endif

    ////  Content verification ////
    DBG_INTERN_DVBT2(printf(">Verify Code...\n"));

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04, 0x00);         // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05, 0x00);         // sram address high byte

#ifdef INTERN_DVBT2_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < 0x8000; i++)
    {
        u8data = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
        if (u8data != INTERN_DVBT2_table[i])
        {
            printf(">fail add = 0x%x\n", i);
            printf(">code = 0x%x\n", INTERN_DVBT2_table[i]);
            printf(">data = 0x%x\n", u8data);

            if (fail_cnt++ > 10)
            {
                printf(">DVB-T2 DSP SRAM Loadcode fail!\n");
                return false;
            }
        }
    }
#else
    for (i=0;i<=SizeBy4K;i++)
    {
        if(i==SizeBy4K)
            len=BinInfo.B_Len%0x1000;
        else
            len=0x1000;

        srcaddr = u32GEAddr+(0x1000*i);
        //printf("\t i = %08LX\n", i);
        //printf("\t len = %08LX\n", len);
        op = 1;
        u16Counter = 0 ;
        //printf("\t (B=0x%bx)(Src=0x%x)Data =",i,srcaddr);
        while(len--)
        {
            u16Counter ++ ;
            //printf("file: %s, line: %d\n", __FILE__, __LINE__);
            //pU8Data = (U8 *)(srcaddr|0x80000000);
            #if OBA2
            pU8Data = (U8 *)(srcaddr);
            #else
            pU8Data = (U8 *)(srcaddr|0x80000000);
            #endif
            Data  = *pU8Data;

            #if 0
            if(u16Counter < 0x100)
                printf("0x%bx,", Data);
            #endif
            u8data = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
            if (u8data != Data)
            {
                printf(">fail add = 0x%lx\n", (U32)((i*0x1000)+(0x1000-len)));
                printf(">code = 0x%x\n", Data);
                printf(">data = 0x%x\n", u8data);

                if (fail_cnt++ > 10)
                {
                    printf(">DVB-T DSP Loadcode fail!");
                    return false;
                }
            }

            srcaddr += op;
        }
     //   printf("\n\n\n");
    }
#endif

    // add T2 DRAM bufer start address into fixed location
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x30);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte

    // write Start address to VD MCU 51 code sram
//    //0x30~0x33
//    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_DRAM_START_ADDR);
//    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_DRAM_START_ADDR >> 8));
//    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_DRAM_START_ADDR >> 16));
//    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_DRAM_START_ADDR >> 24));
    //0x30~0x33
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)u32DMD_DVBT2_EQ_START_ADDR);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_EQ_START_ADDR >> 8));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_EQ_START_ADDR >> 16));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_EQ_START_ADDR >> 24));
    //0x34~0x37
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)u32DMD_DVBT2_TDI_START_ADDR);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_TDI_START_ADDR >> 8));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_TDI_START_ADDR >> 16));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_TDI_START_ADDR >> 24));
    //0x38~0x3b
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)u32DMD_DVBT2_DJB_START_ADDR);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_DJB_START_ADDR >> 8));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_DJB_START_ADDR >> 16));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_DJB_START_ADDR >> 24));
    //0x3c~0x3f
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)u32DMD_DVBT2_FW_START_ADDR);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_FW_START_ADDR >> 8));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_FW_START_ADDR >> 16));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBT2_FW_START_ADDR >> 24));

    DBG_INTERN_DVBT2(printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_EQ_START_ADDR=0x%lx \n", u32DMD_DVBT2_EQ_START_ADDR));
    DBG_INTERN_DVBT2(printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_TDI_START_ADDR=0x%lx \n", u32DMD_DVBT2_TDI_START_ADDR));
    DBG_INTERN_DVBT2(printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_DJB_START_ADDR=0x%lx \n", u32DMD_DVBT2_DJB_START_ADDR));
    DBG_INTERN_DVBT2(printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_FW_START_ADDR=0x%lx \n", u32DMD_DVBT2_FW_START_ADDR));

#if 0
	// DEBUG
//    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04, 0x30);         // sram address low byte
//    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05, 0x00);         // sram address high byte

//    for ( i = 0; i < 16; i++)
//    {
//        u8data = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
//        printf(">add = 0x%x\t", i);
//        printf(">data = 0x%x\n", u8data);
//	}

	printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_EQ_START_ADDR=0x%lx \n", u32DMD_DVBT2_EQ_START_ADDR);
	printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_TDI_START_ADDR=0x%lx \n", u32DMD_DVBT2_TDI_START_ADDR);
	printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_DJB_START_ADDR=0x%lx \n", u32DMD_DVBT2_DJB_START_ADDR);
	printf(">>> [INTERN_DVBT2_LoadDSPCode]DVBT2_FW_START_ADDR=0x%lx \n", u32DMD_DVBT2_FW_START_ADDR);
#endif

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x50);     // diable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x00);     // disable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01, 0x01);     // enable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);     // release VD_MCU

    DBG_INTERN_DVBT2(printf(">DSP Loadcode done."));
    //while(load_data_variable);

    return TRUE;
}

/***********************************************************************************
  Subject:    DVB-T CLKGEN initialized function
  Function:   INTERN_DVBT2_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
void INTERN_DVBT2_InitClkgen(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 temp_val;
    MS_U16 u16_temp_val;

    DBG_INTERN_DVBT2(printf("INTERN_DVBT2_InitClkgen\n"));

    HAL_DMD_RIU_WriteByte(0x101e39,0x00);
    //HAL_DMD_RIU_WriteByte(0x112003, (HAL_DMD_RIU_ReadByte(0x112003)&(~(BIT(5)))));      // Release Ana misc resest
    // ----------------------------------------------
    //  start demod CLKGEN setting
    // ----------------------------------------------
    // *** Set register at CLKGEN1
    // enable DMD MCU clock "bit[0] set 0"
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0f, 2'b11, 16'h001c);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0f, 2'b11, 16'h001c);
    // CLK_DMDMCU clock setting
    // [0] disable clock
    // [1] invert clock
    // [4:2]
    //         000:170 MHz(MPLL_DIV_BUf)
    //         001:160MHz
    //         010:144MHz
    //         011:123MHz
    //         100:108MHz
    //         101:mem_clcok
    //         110:mem_clock div 2
    //         111:select XTAL
    HAL_DMD_RIU_WriteByte(0x10331f,0x00);
//    HAL_DMD_RIU_WriteByte(0x10331e,0x1c); // 24MHz
    HAL_DMD_RIU_WriteByte(0x10331e,0x10); // 108MHz

    // set parallet ts clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    //HAL_DMD_RIU_WriteByte(0x103301,0x07);//0615
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x05;
//	temp_val|=0x07;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);

    HAL_DMD_RIU_WriteByte(0x103300,0x10);

    // enable DVBTC ts clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x103309,0x00);
    HAL_DMD_RIU_WriteByte(0x103308,0x00);

    // enable dvbc adc clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x103315,0x00);
    HAL_DMD_RIU_WriteByte(0x103314,0x00);

    // ----------------------------------------------
    //  start demod_0 CLKGEN setting
    // ----------------------------------------------

    // enable clk_atsc_adcd_sync
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    HAL_DMD_RIU_WriteByte(0x111f0a,0x00);

    //reg_ckg_dvbt_inner
    HAL_DMD_RIU_WriteByte(0x111f21,0x11);
    HAL_DMD_RIU_WriteByte(0x111f20,0x10);

    //reg_ckg_dvbt_outer
    HAL_DMD_RIU_WriteByte(0x111f23,0x01);
    HAL_DMD_RIU_WriteByte(0x111f22,0x11);

    //reg_ckg_acifir
    HAL_DMD_RIU_WriteByte(0x111f25,0x04);

    //reg_ckg_dvbtm_sram_t1o2x_t22x
    HAL_DMD_RIU_WriteByte(0x111f29,0x00);
    HAL_DMD_RIU_WriteByte(0x111f28,0x00);

    //reg_ckg_dvbtm_sram_adc_t22x
    HAL_DMD_RIU_WriteByte(0x111f2d,0x00);
    HAL_DMD_RIU_WriteByte(0x111f2c,0x01);

    //reg_ckg_dvbtm_sram_t12x_t24x
    HAL_DMD_RIU_WriteByte(0x111f2f,0x00);
    HAL_DMD_RIU_WriteByte(0x111f2e,0x00);

    //reg_ckg_dvbtm_ts_in
    HAL_DMD_RIU_WriteByte(0x111f31,0x04);
    HAL_DMD_RIU_WriteByte(0x111f30,0x00);

    HAL_DMD_RIU_WriteByte(0x111f33,0x3c);
    HAL_DMD_RIU_WriteByte(0x111f32,0x00);

    HAL_DMD_RIU_WriteByte(0x111f35,0x00);
    HAL_DMD_RIU_WriteByte(0x111f34,0x00);

    HAL_DMD_RIU_WriteByte(0x111f37,0x00);
    HAL_DMD_RIU_WriteByte(0x111f36,0x00);

    HAL_DMD_RIU_WriteByte(0x111f3b,0x00);
    HAL_DMD_RIU_WriteByte(0x111f3a,0x00);

    HAL_DMD_RIU_WriteByte(0x111f3d,0x00);
    HAL_DMD_RIU_WriteByte(0x111f3c,0x00);

    HAL_DMD_RIU_WriteByte(0x111f43,0x00);
    HAL_DMD_RIU_WriteByte(0x111f42,0x00);

    HAL_DMD_RIU_WriteByte(0x111f45,0x00);
    HAL_DMD_RIU_WriteByte(0x111f44,0x00);

    HAL_DMD_RIU_WriteByte(0x111fe1,0x00);
    HAL_DMD_RIU_WriteByte(0x111fe0,0x00);

    HAL_DMD_RIU_WriteByte(0x111fe3,0x00);
    HAL_DMD_RIU_WriteByte(0x111fe2,0x00);

    HAL_DMD_RIU_WriteByte(0x111fe5,0x00);
    HAL_DMD_RIU_WriteByte(0x111fe4,0x00);

    HAL_DMD_RIU_WriteByte(0x111fe7,0x00);
    HAL_DMD_RIU_WriteByte(0x111fe6,0x00);

    HAL_DMD_RIU_WriteByte(0x111fe9,0x00);
    HAL_DMD_RIU_WriteByte(0x111fe8,0x00);

    HAL_DMD_RIU_WriteByte(0x111feb,0xc8);
    HAL_DMD_RIU_WriteByte(0x111fea,0x00);

    HAL_DMD_RIU_WriteByte(0x111fed,0x00);
    HAL_DMD_RIU_WriteByte(0x111fec,0x0c);

    HAL_DMD_RIU_WriteByte(0x111fef,0x00);
    HAL_DMD_RIU_WriteByte(0x111fee,0x00);

		// Maserati special
    HAL_DMD_RIU_WriteByte(0x152971,0x10);
    HAL_DMD_RIU_WriteByte(0x152970,0x01);

    HAL_DMD_RIU_WriteByte(0x111ff0,0x00);

    // Mulan special
    // TEQ CLK for DVBT2
//    HAL_DMD_RIU_WriteByte(0x111f24,0x00);

    // SRAM share
    HAL_DMD_RIU_WriteByte(0x111f75,0x00);
    HAL_DMD_RIU_WriteByte(0x111f74,0x00);

    HAL_DMD_RIU_WriteByte(0x111f77,0x00);
    HAL_DMD_RIU_WriteByte(0x111f76,0x00);

    HAL_DMD_RIU_WriteByte(0x111f79,0x00);
    HAL_DMD_RIU_WriteByte(0x111f78,0x00);

    HAL_DMD_RIU_WriteByte(0x111f7b,0x00);
    HAL_DMD_RIU_WriteByte(0x111f7a,0x00);

    HAL_DMD_RIU_WriteByte(0x111f7d,0x00);
    HAL_DMD_RIU_WriteByte(0x111f7c,0x00);

    HAL_DMD_RIU_WriteByte(0x111f7f,0x00);
    HAL_DMD_RIU_WriteByte(0x111f7e,0x00);

    // 32+4K xdata sram
    HAL_DMD_RIU_WriteByte(0x1117e0,0x23);
    HAL_DMD_RIU_WriteByte(0x1117e1,0x21);
    HAL_DMD_RIU_WriteByte(0x1117e4,0x01);
    HAL_DMD_RIU_WriteByte(0x1117e6,0x11);

    // SRAM allocation
    HAL_DMD_RIU_WriteByte(0x111701,0x00);
    HAL_DMD_RIU_WriteByte(0x111700,0x00);

    HAL_DMD_RIU_WriteByte(0x111705,0x00);
    HAL_DMD_RIU_WriteByte(0x111704,0x00);

    HAL_DMD_RIU_WriteByte(0x111703,0x00);
    HAL_DMD_RIU_WriteByte(0x111702,0x00);

    HAL_DMD_RIU_WriteByte(0x111707,0x7f);
    HAL_DMD_RIU_WriteByte(0x111706,0xff);

    // SDRAM address offset
    u16_temp_val = (MS_U16)(u32DMD_DVBT2_FW_START_ADDR>>16);
    HAL_DMD_RIU_WriteByte(0x11171b,(MS_U8)(u16_temp_val>>8));
    HAL_DMD_RIU_WriteByte(0x11171a,(MS_U8)u16_temp_val);

    // DRAM allocation
    HAL_DMD_RIU_WriteByte(0x111709,0x00);
    HAL_DMD_RIU_WriteByte(0x111708,0x00);

    HAL_DMD_RIU_WriteByte(0x11170d,0x80);
    HAL_DMD_RIU_WriteByte(0x11170c,0x00);

    HAL_DMD_RIU_WriteByte(0x11170b,0x00);
    HAL_DMD_RIU_WriteByte(0x11170a,0x00);

    HAL_DMD_RIU_WriteByte(0x11170f,0xff);
    HAL_DMD_RIU_WriteByte(0x11170e,0xff);

    // DRAM EN
    HAL_DMD_RIU_WriteByte(0x111718,0x04);

    // [0]switch dram address mode:
    // 0: address from dmdmcu51 bank (old mode)
    // 1: address from dmdmcu51_top bank (new mode)
    HAL_DMD_RIU_WriteByte(0x11171c,0x01);

    // ----------------------------------------------
    //  start demod CLKGEN setting
    // ----------------------------------------------
    //  select DMD MCU
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h1c, 2'b10, 16'h0300);
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h1c, 2'b10, 16'h0300);
    HAL_DMD_RIU_WriteByte(0x101e39,0x03);

    // stream2miu_en, activate rst_wadr
    // `RIU_W((`RIUBASE_TSP0 >>1) + `REG16_HW_CONFIG8, 2'b11, 16'h0012);
    HAL_DMD_RIU_WriteByte(0x103c0e,0x01);
    // stream2miu_en, turn off rst_wadr
    // `RIU_W((`RIUBASE_TSP0 >>1) + `REG16_HW_CONFIG8, 2'b11, 16'h0102);

}

/***********************************************************************************
  Subject:    Power on initialized function
  Function:   INTERN_DVBT2_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/

MS_BOOL INTERN_DVBT2_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBT2_DSPRegInitExt, MS_U8 u8DMD_DVBT2_DSPRegInitSize)
{
    MS_U16            status = true;

//    MS_U8 temp_val;
    //MS_U8   cData = 0;
    //U8            cal_done;
    DBG_INTERN_DVBT2(printf("INTERN_DVBT2_Power_On_Initialization\n"));

#if defined(PWS_ENABLE)
    Mapi_PWS_Stop_VDMCU();
#endif
// No definition for Mulan
#if 0
    // Global demod reset. To fix DVBS -> DVBT2 or DVBS blind scan -> DVBT2 unlock issue.
    temp_val=HAL_DMD_RIU_ReadByte(0x101e3a);
    HAL_DMD_RIU_WriteByte(0x101e3a,temp_val|0x02);

    MsOS_DelayTask(1);

    HAL_DMD_RIU_WriteByte(0x101e3a,temp_val&(~0x02));
#endif

    INTERN_DVBT2_InitClkgen(bRFAGCTristateEnable);
    HAL_DMD_ADC_IQ_Switch(u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain);
    //// Firmware download //////////
    DBG_INTERN_DVBT2(printf("INTERN_DVBT2 Load DSP...\n"));
    //MsOS_DelayTask(100);

    //if (HAL_DMD_RIU_ReadByte(0x101E3E) != 0x02) // DVBT = BIT1 -> 0x02
    {
        if (INTERN_DVBT2_LoadDSPCode() == FALSE)
        {
            printf("DVB-T2 Load DSP Code Fail\n");
            return FALSE;
        }
        else
        {
            DBG_INTERN_DVBT2(printf("DVB-T2 Load DSP Code OK\n"));
        }
    }


    //// MCU Reset //////////
    DBG_INTERN_DVBT2(printf("INTERN_DVBT2 Reset...\n"));
    if (INTERN_DVBT2_Reset() == FALSE)
    {
        DBG_INTERN_DVBT2(printf("Fail\n"));
        return FALSE;
    }
    else
    {
        DBG_INTERN_DVBT2(printf("OK\n"));
    }

    // SRAM setting, DVB-T use it.
    // 0x2204, Bit0, 0:DVB-T use, 1: VIF use
    //MDrv_SYS_DMD_VD_MBX_ReadReg(0x2204,&cData);
    //MDrv_SYS_DMD_VD_MBX_WriteReg(0x2204, cData&0xFE);

    status &= INTERN_DVBT2_DSPReg_Init(u8DMD_DVBT2_DSPRegInitExt, u8DMD_DVBT2_DSPRegInitSize);
    return status;
}

/************************************************************************************************
  Subject:    Driving control
  Function:   INTERN_DVBT2_Driving_Control
  Parmeter:   bInversionEnable : TRUE For High
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBT2_Driving_Control(MS_BOOL bEnable)
{
    MS_U8    u8Temp;

    u8Temp = HAL_DMD_RIU_ReadByte(0x101E10);

    if (bEnable)
    {
       u8Temp = u8Temp | 0x01; //bit0: clk, bit1~8:data , bit9: sync, bit10:valid
    }
    else
    {
       u8Temp = u8Temp & (~0x01);
    }

    DBG_INTERN_DVBT2(printf("---> INTERN_DVBT2_Driving_Control(Bit0) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x101E10, u8Temp);
}
/************************************************************************************************
  Subject:    Clk Inversion control
  Function:   INTERN_DVBT2_Clk_Inversion_Control
  Parmeter:   bInversionEnable : TRUE For Inversion Action
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBT2_Clk_Inversion_Control(MS_BOOL bInversionEnable)
{
    MS_U8   u8Temp;

    u8Temp = HAL_DMD_RIU_ReadByte(0x103301);

    if (bInversionEnable)
    {
       u8Temp = u8Temp | 0x02; //bit 9: clk inv
    }
    else
    {
       u8Temp = u8Temp & (~0x02);
    }

    DBG_INTERN_DVBT2(printf("---> Inversion(Bit9) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x103301, u8Temp);
}
/************************************************************************************************
  Subject:    Transport stream serial/parallel control
  Function:   INTERN_DVBT2_Serial_Control
  Parmeter:   bEnable : TRUE For serial
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBT2_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk)
{
    MS_U8   status = true;
    MS_U8   temp_val;
    DBG_INTERN_DVBT2(printf(" @INTERN_DVBT2_ts... u8TSClk=%d\n",u8TSClk));

    if (u8TSClk == 0xFF) u8TSClk=0x13;
    if (bEnable)    //Serial mode for TS pad
    {
        // serial
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        HAL_DMD_RIU_WriteByte(0x103300, 0x00);   // serial mode 0x0400
#if(INTERN_DVBT2_TS_SERIAL_INVERSION == 0)
//        HAL_DMD_RIU_WriteByte(0x103301, 0x04);   // reg_ckg_dvbtmk_ts_out_mode@0x00

    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x04;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
//        HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x07;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)&0xEF);   // PAD_TS1 is used as output
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xE3);   // PAD_TS1 Disable TS CLK PAD
    }
    else
    {
        //parallel
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001
        HAL_DMD_RIU_WriteByte(0x103309, 0x00);   // reg_ckg_dvbtc_ts@0x04

        //HAL_DMD_RIU_WriteByte(0x103300, 0x11);   // parallel mode: 0x0511 => ts_clk=288/(2*(0x11+1))=8MHz
        HAL_DMD_RIU_WriteByte(0x103300, u8TSClk);   // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
#if(INTERN_DVBT2_TS_PARALLEL_INVERSION == 0)
//        HAL_DMD_RIU_WriteByte(0x103301, 0x05);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x05;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
//        HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x07;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif

        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)|0x10);   // PAD_TS1 is used as output
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, (HAL_DMD_RIU_ReadByte(0x101EAF)&0xC0)|0x11);   // PAD_TS1 enable TS clk pad
    }

    //DBG_INTERN_DVBT2(printf("---> Inversion(Bit5) = 0x%x \n",gsCmdPacket.param[1] ));

    INTERN_DVBT2_Driving_Control(INTERN_DVBT2_DTV_DRIVING_LEVEL);
    return status;
}

/************************************************************************************************
  Subject:    TS1 output control
  Function:   INTERN_DVBT2_PAD_TS1_Enable
  Parmeter:   flag : TRUE For Turn on TS1, FALSE For Turn off TS1
  Return:     void
  Remark:
*************************************************************************************************/
void INTERN_DVBT2_PAD_TS1_Enable(MS_BOOL flag)
{
    DBG_INTERN_DVBT2(printf(" @INTERN_DVBT2_TS1_Enable... \n"));

    if(flag) // PAD_TS1 Enable TS CLK PAD
    {
        //printf("=== TS1_Enable ===\n");
        //HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)|0x10);   //For T3
        //HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)|0x18);   //For T4
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)|0x11);   //For T8
    }
    else // PAD_TS1 Disable TS CLK PAD
    {
        //printf("=== TS1_Disable ===\n");
        //HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)&0xEF);   //For T3
        //HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xE3);   //For T4
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xC0);   //For T8
    }
}

/************************************************************************************************
  Subject:    channel change config
  Function:   INTERN_DVBT2_Config
  Parmeter:   BW: bandwidth
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBT2_Config(DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U32 u32IFFreq, MS_U8 u8PlpID)
{
    MS_U8   bandwidth;
    MS_U8   status = true;
    //MS_U8   temp_val;
    DBG_INTERN_DVBT2(printf(" @INTERN_DVBT2_config %d %d %d %ld %d\n", BW, bSerialTS, u8TSClk, u32IFFreq, u8PlpID));
    DBG_INTERN_DVBT2_TIME(printf("INTERN_DVBT2_Config, t = %ld\n",MsOS_GetSystemTime()));

    if (u8TSClk == 0xFF) u8TSClk=0x13;
    switch(BW)
    {
        case E_DMD_T2_RF_BAND_5MHz:
            bandwidth = 1;
            break;
        case E_DMD_T2_RF_BAND_6MHz:
            bandwidth = 2;
            break;
        case E_DMD_T2_RF_BAND_7MHz:
            bandwidth = 3;
            break;
        case E_DMD_T2_RF_BAND_10MHz:
            bandwidth = 5;
            break;
        case E_DMD_T2_RF_BAND_1p7MHz:
            bandwidth = 0;
        break;
        case E_DMD_T2_RF_BAND_8MHz:
        default:
            bandwidth = 4;
            break;
    }

    status &= INTERN_DVBT2_Reset();

    // BW mode
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_T2_BW, BW);
    // TS mode
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_T2_TS_SERIAL, bSerialTS? 0x01:0x00);
    // FC
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_T2_FC_L, u32IFFreq&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_T2_FC_H, (u32IFFreq>>8)&0xff);
    // PLP_ID
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_T2_PLP_ID, u8PlpID);

/*
    if(bSerialTS)
    {
        // serial
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        HAL_DMD_RIU_WriteByte(0x103300, 0x00);   // parallel mode: 0x0511 /serial mode 0x0400
#if(INTERN_DVBT2_TS_SERIAL_INVERSION == 0)
//        HAL_DMD_RIU_WriteByte(0x103301, 0x04);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x04;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
//        HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x07;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
    }
    else
    {
        //parallel
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x00);   // reg_ckg_dvbtc_ts@0x04

        //HAL_DMD_RIU_WriteByte(0x103300, 0x11);   // parallel mode: 0x0511 => ts_clk=288/(2*(0x11+1))=8MHz
        HAL_DMD_RIU_WriteByte(0x103300, u8TSClk);  // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
#if(INTERN_DVBT2_TS_PARALLEL_INVERSION == 0)
//        HAL_DMD_RIU_WriteByte(0x103301, 0x05);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x05;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
//        HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x07;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
    }
*/
    return status;
}
/************************************************************************************************
  Subject:    enable hw to lock channel
  Function:   INTERN_DVBT2_Active
  Parmeter:   bEnable
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBT2_Active(MS_BOOL bEnable)
{
    MS_U8   status = true;

    DBG_INTERN_DVBT2(printf(" @INTERN_DVBT2_active\n"));

    //// INTERN_DVBT2 Finite State Machine on/off //////////
    HAL_DMD_RIU_WriteByte(MBRegBase + (0x0e)*2, 0x01);   // FSM_EN

//    INTERN_DVBT2_SignalQualityReset();
    return status;
}
/************************************************************************************************
  Subject:    Return lock status
  Function:   INTERN_DVBT2_Lock
  Parmeter:   eStatus :
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
DMD_T2_LOCK_STATUS INTERN_DVBT2_Lock(MS_U16 u16DMD_DVBT2_P1_Timeout, MS_U16 u16DMD_DVBT2_FEC_Timeout)
{
//    float fBER=0.0f;

    if (INTERN_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK))
    {
#if 0
        // copy from msb1240 >>>>>
        if(INTERN_DVBT2_GetPostLdpcBer(&fBER) == TRUE)
        {
            if ((fBerFilteredDVBT2 <= 0.0) || ((fBerFilteredDVBT2/fBER) > 30.0 || (fBerFilteredDVBT2/fBER) < 0.03))
                fBerFilteredDVBT2 = fBER;
            else
                fBerFilteredDVBT2 = 0.9f*fBerFilteredDVBT2+0.1f*fBER;
        }
        // <<<<< copy from msb1240
#endif

        if (bFECLock ==  FALSE)
        {
            u32FecFirstLockTime = MsOS_GetSystemTime();
            DBG_INTERN_DVBT2(printf("++++++++[utopia]dvbt2 lock\n"));
        }
#if 0
        if(INTERN_DVBT2_GetPostLdpcBer(&fBER) == TRUE)
        {
            if((fBER > 1.0E-8) && (fBER < 1.0E-1))
            {
                if(fLDPCBerFiltered <= 0.0)
                    fLDPCBerFiltered = fBER;
                else
                    fLDPCBerFiltered = 0.9f*fLDPCBerFiltered+0.1f*fBER;
            }
            DBG_INTERN_DVBT2(printf("[dvbt2]f_ber=%8.3e, g_ldpc_ber=%8.3e\n",fBER,fLDPCBerFiltered));
        }
#endif
        u32FecLastLockTime = MsOS_GetSystemTime();
        bFECLock = TRUE;
        return E_DMD_T2_LOCK;
    }
    else
    {
#if 0
        INTERN_DVBT2_SignalQualityReset();
#endif
        if (bFECLock == TRUE)
        {
            if ((MsOS_GetSystemTime() - u32FecLastLockTime) < 1000)
            {
                return E_DMD_T2_LOCK;
            }
        }
        bFECLock = FALSE;
    }
/*
#ifdef CHIP_KRITI
    if (INTERN_DVBT2_GetLock(E_DMD_DVBT2_NO_CHANNEL))
    {
    //	DBG_INTERN_DVBT2(printf("==> INTERN_DVBT2_Lock -- E_DMD_DVBT2_NO_CHANNEL \n"););
        return E_DMD_T2_UNLOCK;
    }
#endif
*/
    if(!bP1Lock)
    {
        if (INTERN_DVBT2_GetLock(E_DMD_DVBT2_P1_EVER_LOCK))
        {
            DBG_INTERN_DVBT2(printf("==> INTERN_DVBT2_Lock -- P1Lock \n"));
            bP1Lock = TRUE;
        }
    }
    if(bP1Lock)
    {
        DBG_INTERN_DVBT2(printf("P1Lock %ld\n",MsOS_GetSystemTime()));
        if(MsOS_Timer_DiffTimeFromNow(u32ChkScanTimeStart) < u16DMD_DVBT2_FEC_Timeout)
        {
            return E_DMD_T2_CHECKING;
        }
    }
    else
    {
        if(MsOS_Timer_DiffTimeFromNow(u32ChkScanTimeStart) < u16DMD_DVBT2_P1_Timeout)
        {
            return E_DMD_T2_CHECKING;
        }
    }
    return E_DMD_T2_UNLOCK;

}


MS_BOOL INTERN_DVBT2_GetLock(DMD_DVBT2_GETLOCK_TYPE eStatus)
{
    MS_U16 u16Address = 0;
    MS_U8 cData = 0;
    MS_U8 cBitMask = 0;
    MS_U8 use_dsp_reg = 0;

    switch( eStatus )
    {
        case E_DMD_DVBT2_FEC_LOCK:
            use_dsp_reg = 1;
            u16Address =  E_DMD_T2_DVBT2_LOCK_HIS; //FEC lock,
            cBitMask = BIT(7);
            break;

        case E_DMD_DVBT2_P1_LOCK:
            u16Address =  0x3082; //P1 HW Lock,
            cBitMask = BIT(3);
            break;

        case E_DMD_DVBT2_DCR_LOCK:
            use_dsp_reg = 1;
            u16Address =  E_DMD_T2_DVBT2_LOCK_HIS; //DCR Lock,
            cBitMask = BIT(2);
            break;

        case E_DMD_DVBT2_AGC_LOCK:
            use_dsp_reg = 1;
            u16Address =  E_DMD_T2_DVBT2_LOCK_HIS; //AGC Lock,
            cBitMask = BIT(0);
            break;

        case E_DMD_DVBT2_MODE_DET:
            u16Address =  0x3082; //Mode CP Detect,
            cBitMask = BIT(1);
            break;

        case E_DMD_DVBT2_P1_EVER_LOCK:
            use_dsp_reg = 1;
            u16Address =  E_DMD_T2_DVBT2_LOCK_HIS;  //P1 Ever Lock,
            cBitMask = BIT(5);
            break;

        case E_DMD_DVBT2_L1_CRC_LOCK:
            u16Address =  0x2B41;  //P1 Ever Lock,
            cBitMask = BIT(5)|BIT(6)|BIT(7);
            break;

	case E_DMD_DVBT2_NO_CHANNEL:
            u16Address =  0x20C0;  // JL or FS no channel detection flag, 1 means no channel.
            cBitMask = BIT(7);
            break;


        default:
            return FALSE;
    }

    if (use_dsp_reg == 1)
    {
        if (MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16Address, &cData) == FALSE)
        {
            DBG_INTERN_DVBT2(printf(">INTERN_DVBT2_GetLock MBX_ReadDspReg fail \n"));
            return FALSE;
        }
    }
    else
    {
        if (MDrv_SYS_DMD_VD_MBX_ReadReg(u16Address, &cData) == FALSE)
        {
            DBG_INTERN_DVBT2(printf(">INTERN_DVBT2_GetLock MBX_ReadReg fail \n"));
            return FALSE;
        }
    }

#ifdef MS_DEBUG
    MS_U8 u8tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(0x20c4, &u8tmp);
    DBG_INTERN_DVBT2(printf(">>>>>>>>>> DVBT2 State=%d \n", u8tmp));
#endif

    if ((cData & cBitMask) == cBitMask)
    {
        DBG_INTERN_DVBT2(printf(">INTERN_DVBT2_GetLock eStatus=%d is lock \n", eStatus));
        return TRUE;
    }
    else
    {
        DBG_INTERN_DVBT2(printf(">INTERN_DVBT2_GetLock eStatus=%d is unlock \n", eStatus));
        return FALSE;
    }

}

/****************************************************************************
  Subject:    To get the Post LDPC BER
  Function:   INTERN_DVBT2_GetPostLdpcBer
  Parmeter:  Quility
  Return:       E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT2_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
**************************************************************************/
MS_BOOL INTERN_DVBT2_GetPostLdpcBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType)
{
    MS_BOOL          status = true;
    MS_U8              reg=0;
//    MS_U16            BitErrPeriod;
//    MS_U32            BitErr;
//    MS_U16            FecType = 0;

    /////////// Post-Viterbi BER /////////////
    /////////// Data BER /////////////
    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x01);    // avoid confliction

    // bank 0x33 0x12 Data BER Window[15:0]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x25, &reg);
    *BitErrPeriod_reg = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x24, &reg);
    *BitErrPeriod_reg = (*BitErrPeriod_reg << 8) | reg;

    // bank 0x33 0x34 Data BER count[15:0]
    // bank 0x33 0x35 Data BER count[31:16]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 3, &reg);
    *BitErr_reg = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 2, &reg);
    *BitErr_reg = (*BitErr_reg << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 1, &reg);
    *BitErr_reg = (*BitErr_reg << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 0, &reg);
    *BitErr_reg = (*BitErr_reg << 8) | reg;

    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x00);     // avoid confliction

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8f, &reg);    //FEC Type[8:7]
    *FecType = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8e, &reg);    //FEC Type[8:7]
    *FecType = (*FecType << 8) | reg;

    return status;
}

#if 0
MS_BOOL INTERN_DVBT2_GetPostLdpcBer(float *ber)
{
    MS_BOOL          status = true;
    MS_U8              reg=0;
    MS_U16            BitErrPeriod;
    MS_U32            BitErr;
    MS_U16            FecType = 0;

    /////////// Post-Viterbi BER /////////////

    if ( MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime) < 300)
    {
        *ber = (float)-1.0;
        return false;
    }

    /////////// Data BER /////////////
    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x01);    // avoid confliction

    // bank 0x33 0x12 Data BER Window[15:0]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x25, &reg);
    BitErrPeriod = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x24, &reg);
    BitErrPeriod = (BitErrPeriod << 8) | reg;

    // bank 0x33 0x34 Data BER count[15:0]
    // bank 0x33 0x35 Data BER count[31:16]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 3, &reg);
    BitErr = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 2, &reg);
    BitErr = (BitErr << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 1, &reg);
    BitErr = (BitErr << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x34 << 1) + 0, &reg);
    BitErr = (BitErr << 8) | reg;

    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x00);     // avoid confliction

    if (BitErrPeriod == 0)
        //protect 0
        BitErrPeriod = 1;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8f, &reg);    //FEC Type[8:7]
    FecType = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8e, &reg);    //FEC Type[8:7]
    FecType = (FecType << 8) | reg;

    if (FecType & 0x0180)
    {
        if (BitErr == 0)
            *ber = (float)0.5 / (float)(BitErrPeriod * 64800);
        else
            *ber = (float)BitErr / (float)(BitErrPeriod * 64800);
    }
    else
    {
        if (BitErr == 0)
            *ber = (float)0.5 / (float)(BitErrPeriod * 16200);
        else
            *ber = (float)BitErr / (float)(BitErrPeriod * 16200);
    }

    DBG_GET_SIGNAL(printf("INTERN_DVBT2 PostLDPCBER = %8.3e \n ", *ber));

    if (status == FALSE)
    {
        printf("INTERN_DVBT2_GetPostLdpcBer Fail!\n");
        return FALSE;
    }

    return status;
}
#endif

/****************************************************************************
  Subject:    To get the Pre LDPC BER
  Function:   INTERN_DVBT2_GetPreLdpcBer
  Parmeter:   ber
  Return:     E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT2_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBT2_GetPreLdpcBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType)
{
    MS_U8            status = true;
    MS_U8            reg=0;
//    MS_U16           BitErrPeriod;
//    MS_U32           BitErr;
//    MS_U16          FecType = 0;

    /////////// Data BER /////////////
    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x01);    // avoid confliction

    // bank 0x33 0x12 Data BER Window[15:0]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x25, &reg);
    *BitErrPeriod_reg = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x24, &reg);
    *BitErrPeriod_reg = (*BitErrPeriod_reg << 8) | reg;

    // bank 0x33 0x34 Data BER count[15:0]
    // bank 0x33 0x35 Data BER count[31:16]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 3, &reg);
    *BitErr_reg = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 2, &reg);
    *BitErr_reg = (*BitErr_reg << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 1, &reg);
    *BitErr_reg = (*BitErr_reg << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 0, &reg);
    *BitErr_reg = (*BitErr_reg << 8) | reg;

    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x00);     // avoid confliction

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8f, &reg);    //FEC Type[8:7]
    *FecType = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8e, &reg);    //FEC Type[8:7]
    *FecType = (*FecType << 8) | reg;

    return status;
}

#if 0
/****************************************************************************
  Subject:    To get the Pre LDPC BER
  Function:   INTERN_DVBT2_GetPreLdpcBer
  Parmeter:   ber
  Return:     E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT2_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBT2_GetPreLdpcBer(float *ber)
{
    MS_U8            status = true;
    MS_U8            reg=0;
    MS_U16           BitErrPeriod;
    MS_U32           BitErr;
    MS_U16          FecType = 0;

    /////////// Data BER /////////////
    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x01);    // avoid confliction

    // bank 0x33 0x12 Data BER Window[15:0]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x25, &reg);
    BitErrPeriod = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x24, &reg);
    BitErrPeriod = (BitErrPeriod << 8) | reg;

    // bank 0x33 0x34 Data BER count[15:0]
    // bank 0x33 0x35 Data BER count[31:16]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 3, &reg);
    BitErr = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 2, &reg);
    BitErr = (BitErr << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 1, &reg);
    BitErr = (BitErr << 8) | reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE + (0x32 << 1) + 0, &reg);
    BitErr = (BitErr << 8) | reg;

    // bank 0x33 0x02 [0] freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x00);     // avoid confliction

    if (BitErrPeriod == 0)
        //protect 0
        BitErrPeriod = 1;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8f, &reg);    //FEC Type[8:7]
    FecType = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x8e, &reg);    //FEC Type[8:7]
    FecType = (FecType << 8) | reg;

    if (FecType & 0x0180)
    {
        if (BitErr == 0)
            *ber = (float)0.5 / (float)(BitErrPeriod * 64800);
        else
            *ber = (float)BitErr / (float)(BitErrPeriod * 64800);
    }
    else
    {
        if (BitErr == 0)
            *ber = (float)0.5 / (float)(BitErrPeriod * 16200);
        else
            *ber = (float)BitErr / (float)(BitErrPeriod * 16200);
    }

    DBG_GET_SIGNAL(printf("INTERN_DVBT2 PreLDPCBER = %8.3e \n ", *ber));

    if (status == FALSE)
    {
        printf("INTERN_DVBT2_GetPreLdpcBer Fail!\n");
        return FALSE;
    }

    return status;
}
#endif

/****************************************************************************
  Subject:    To get the Packet error
  Function:   INTERN_DVBT2_GetPacketErr
  Parmeter:   pktErr
  Return:     E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT2_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBT2_GetPacketErr(MS_U16 *u16PktErr)
{
    MS_BOOL          status = true;
    MS_U8            reg = 0;
    MS_U16           PktErr;

    //freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x01);
    //read packet error
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x5B, &reg);
    PktErr = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FEC_REG_BASE+0x5A, &reg);
    PktErr = (PktErr << 8) | reg;

    *u16PktErr = PktErr;
    //release
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2FEC_REG_BASE+0x04, 0x00);

    DBG_GET_SIGNAL(printf("INTERN_DVBT2 PktErr = %d \n ", (int)PktErr));

    *u16PktErr = PktErr;

    return status;
}

/****************************************************************************
  Subject:    To get the DVBT2 parameter
  Function:   INTERN_DVBT2_Get_L1_Info
  Parmeter:   point to return parameter
  Return:     TRUE
              FALSE
  Remark:   The TPS parameters will be available after TPS lock
*****************************************************************************/
MS_BOOL INTERN_DVBT2_Get_L1_Parameter( MS_U16 * pu16L1_parameter, DMD_DVBT2_SIGNAL_INFO eSignalType)
{
    MS_U8 u8Data = 0;
    MS_U16    FecType = 0;
	MS_U16	  u16Data = 0;
    if (INTERN_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK) == TRUE)
    {
        if (eSignalType == T2_MODUL_MODE)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x47 * 2), &u8Data) == FALSE)
                return FALSE;

            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT(5) | BIT(4) | BIT(3))) >> 3;
        }
        else  if (eSignalType == T2_FFT_VALUE)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2TDP_REG_BASE + (0x40 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT(2) | BIT(1) | BIT(0)));
        }
        else  if (eSignalType == T2_GUARD_INTERVAL)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2TDP_REG_BASE + (0x40 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT(6) | BIT(5) | BIT(4))) >> 4;
        }
        else  if (eSignalType == T2_CODE_RATE)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x47 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT(2) | BIT(1) | BIT(0)));
        }
        else if (eSignalType == T2_PREAMBLE)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT(4))) >> 4;
        }
        else if (eSignalType == T2_S1_SIGNALLING)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT(3) | BIT(2) | BIT(1))) >> 1;
        }
        else if (eSignalType == T2_PILOT_PATTERN)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x36 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0x0F);
        }
        else if (eSignalType == T2_BW_EXT)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT(0)));
        }
        else if (eSignalType == T2_PAPR_REDUCTION)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x31 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0xF0) >> 4;
        }
        else if (eSignalType == T2_OFDM_SYMBOLS_PER_FRAME)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x3C * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (MS_U16) u8Data;
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x3C * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter |= (((MS_U16) u8Data) & 0x0F) << 8;
        }
        else if (eSignalType == T2_PLP_ROTATION)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x47 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & BIT(6)) >> 6;
        }
        else if (eSignalType == T2_PLP_FEC_TYPE)
        {
            //FEC Type[8:7]
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + 0x8f, &u8Data) == FALSE) return FALSE;
            FecType = u8Data;
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + 0x8e, &u8Data) == FALSE) return FALSE;
            FecType = (FecType << 8) | u8Data;

            *pu16L1_parameter = (FecType & 0x0180) >> 7;
        }
        else if (eSignalType == T2_NUM_PLP)
        {
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x42 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (MS_U16)u8Data;
        }
		else if (eSignalType == T2_PLP_TYPE)
		{
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x45 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = ((MS_U16) u8Data) & 0x07;
		}
		else if (eSignalType == T2_PLP_TIME_IL_TYPE)
		{
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x48 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0x10) >> 4;
		}
		else if (eSignalType == T2_PLP_TIME_IL_LENGTH)
		{
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x49 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = ((MS_U16) u8Data) & 0xFF;
		}
		else if (eSignalType == T2_DAT_ISSY)
		{
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2DJB_REG_BASE + (0x61 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0x10) >> 4;
		}
		else if (eSignalType == T2_PLP_MODE)
		{
		    if (MDrv_SYS_DMD_VD_MBX_WriteReg(T2DJB_REG_BASE + (0x60 * 2) + 1, 0x01) == FALSE)
            {
                return FALSE;
            }
		    if (MDrv_SYS_DMD_VD_MBX_WriteReg(T2DJB_REG_BASE + (0x60 * 2), 0x16) == FALSE)
            {
                return FALSE;
            }
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2DJB_REG_BASE + (0x61 * 2), &u8Data) == FALSE)
			{
				return FALSE;
			}
		    if (MDrv_SYS_DMD_VD_MBX_WriteReg(T2DJB_REG_BASE + (0x60 * 2) + 1, 0x00) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = ((MS_U16) u8Data) & 0x03;
		}
		else if (eSignalType == T2_L1_MODULATION)
		{
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x31 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = ((MS_U16) u8Data) & 0x0F;
		}
		else if (eSignalType == T2_NUM_T2_FRAMES)
		{
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x3b * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = ((MS_U16) u8Data) & 0xFF;
		}
		else if (eSignalType == T2_PLP_NUM_BLOCKS_MAX)
		{
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x55 * 2) + 1, &u8Data) == FALSE) return FALSE;
            u16Data = u8Data & 0x03;
            if (MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x55 * 2), &u8Data) == FALSE) return FALSE;
            u16Data = (u16Data << 8) | u8Data;

            *pu16L1_parameter = u16Data;
		}
		else if (eSignalType == T2_FEF_ENABLE)
		{

			if (MDrv_SYS_DMD_VD_MBX_ReadDSPReg(0x00F1, &u8Data) == FALSE)
			{
				DBG_INTERN_DVBT2(printf(">INTERN_DVBT2_GetLock MBX_ReadDspReg fail \n"));
				return FALSE;
			}
            *pu16L1_parameter  = ((MS_U16) u8Data) & 0x01;
		}
        else
        {
            return FALSE;
        }

        return TRUE;

    }

    return FALSE;
}


/****************************************************************************
  Subject:    Read the signal to noise ratio (SNR)
  Function:   INTERN_DVBT2_GetSNR
  Parmeter:   None
  Return:     -1 mean I2C fail, otherwise I2C success then return SNR value
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBT2_GetSNR (MS_U16 *u16_snr100,  MS_U8 *snr_cali, MS_U8 *u8_gi)
{
    MS_U8            status = true;
    MS_U8            reg=0, reg_frz=0;
//    MS_U16          u16_snr100 = 0;
//    float            f_snr;
//    MS_U8       u8_win = 0;
//    MS_U8       u8_gi = 0;

    // freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE+0xef, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE+0xef, reg_frz|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg((MS_U32)E_DMD_T2_SNR_H,&reg);
    *u16_snr100 = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg((MS_U32)E_DMD_T2_SNR_L,&reg);
    *u16_snr100 = (*u16_snr100<<8)|reg;

    // unfreeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE+0xef, reg_frz);

//    f_snr = (float)u16_snr100/100.0;

    // snr cali
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FDP_REG_BASE+0x01*2, &reg);
    *snr_cali = (reg>>2)&0x01;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x31*2, &reg);
    *u8_gi = (reg>>1)&0x07;

    return status;
#if 0
    if (u8_win == 1)
    {
        float snr_offset = 0.0;
        float snr_cali = 0.0;

        if (u8_gi == 0) snr_offset = 0.157;
        else if(u8_gi == 1) snr_offset = 0.317;
        else if(u8_gi == 2) snr_offset = 0.645;
        else if(u8_gi == 3) snr_offset = 1.335;
        else if(u8_gi == 4) snr_offset = 0.039;
        else if(u8_gi == 5) snr_offset = 0.771;
        else if(u8_gi == 6) snr_offset = 0.378;

        snr_cali = f_snr - snr_offset;
        if (snr_cali > 0.0) f_snr = snr_cali;
    }
    //use Polynomial curve fitting to fix snr
    //snr_poly = 0.0027945*pow(*fSNR,3) - 0.2266*pow(*fSNR,2) + 6.0101*(*fSNR) - 53.3621;
    //f_snr = f_snr + snr_poly;

    if (status == true)
        return f_snr;
    else
        return -1;
#endif

}

#if 0
float INTERN_DVBT2_GetSNR (void)
{
    MS_U8            status = true;
    MS_U8            reg=0, reg_frz=0;
    MS_U16          u16_snr100 = 0;
    float            f_snr;
    MS_U8       u8_win = 0;
    MS_U8       u8_gi = 0;

    // freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE+0xef, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE+0xef, reg_frz|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg((MS_U32)E_DMD_T2_SNR_H,&reg);
    u16_snr100 = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg((MS_U32)E_DMD_T2_SNR_L,&reg);
    u16_snr100 = (u16_snr100<<8)|reg;

    // unfreeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE+0xef, reg_frz);

    f_snr = (float)u16_snr100/100.0;

    // snr cali
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2FDP_REG_BASE+0x01*2, &reg);
    u8_win = (reg>>2)&0x01;

    if (u8_win == 1)
    {
        float snr_offset = 0.0;
        float snr_cali = 0.0;

        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE+0x31*2, &reg);
        u8_gi = (reg>>1)&0x07;

        if (u8_gi == 0) snr_offset = 0.157;
        else if(u8_gi == 1) snr_offset = 0.317;
        else if(u8_gi == 2) snr_offset = 0.645;
        else if(u8_gi == 3) snr_offset = 1.335;
        else if(u8_gi == 4) snr_offset = 0.039;
        else if(u8_gi == 5) snr_offset = 0.771;
        else if(u8_gi == 6) snr_offset = 0.378;

        snr_cali = f_snr - snr_offset;
        if (snr_cali > 0.0) f_snr = snr_cali;
    }
    //use Polynomial curve fitting to fix snr
    //snr_poly = 0.0027945*pow(*fSNR,3) - 0.2266*pow(*fSNR,2) + 6.0101*(*fSNR) - 53.3621;
    //f_snr = f_snr + snr_poly;

    if (status == true)
        return f_snr;
    else
        return -1;

}
#endif

#if 0
MS_BOOL INTERN_DVBT2_GetSignalStrength(MS_U16 *strength,const DMD_DVBT2_InitData *sDMD_DVBT2_InitData, MS_U8 u8SarValue, float fRFPowerDbm)
{
    MS_U8   status = true;
    float   ch_power_db = 0.0f;
    float   ch_power_ref = 11.0f;
    float   ch_power_rel = 0.0f;
    //MS_U8   u8_index = 0;
    MS_U16  L1_info_qam, L1_info_cr;
//    MS_U8  demodState = 0;

    if (FALSE == INTERN_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK) )
    {
        *strength = 0;
        return TRUE;
    }
    DBG_INTERN_DVBT2_TIME(printf("INTERN_DVBT2_GetSignalStrength, t=%ld\n",MsOS_GetSystemTime()));

    // if (INTERN_DVBT2_Lock(COFDM_TPS_LOCK))
        //if (INTERN_DVBT2_Lock(COFDM_AGC_LOCK))
        /* Actually, it's more reasonable, that signal level depended on cable input power level
        * thougth the signal isn't dvb-t signal.
        */

#if 0
    // use pointer of IFAGC table to identify
    // case 1: RFAGC from SAR, IFAGC controlled by demod
    // case 2: RFAGC from tuner, ,IFAGC controlled by demod
    status &= HAL_DMD_GetRFLevel(&ch_power_db, fRFPowerDbm, u8SarValue,
                                                                sDMD_DVBT2_InitData->pTuner_RfagcSsi, sDMD_DVBT2_InitData->u16Tuner_RfagcSsi_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcSsi_HiRef, sDMD_DVBT2_InitData->u16Tuner_IfagcSsi_HiRef_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcSsi_LoRef, sDMD_DVBT2_InitData->u16Tuner_IfagcSsi_LoRef_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcErr_HiRef, sDMD_DVBT2_InitData->u16Tuner_IfagcErr_HiRef_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcErr_LoRef, sDMD_DVBT2_InitData->u16Tuner_IfagcErr_LoRef_Size);
#endif

    if(INTERN_DVBT2_Get_L1_Parameter(&L1_info_qam, T2_MODUL_MODE) == FALSE)
        printf("[dvbt2] QAM parameter retrieve failure\n");

    if(INTERN_DVBT2_Get_L1_Parameter(&L1_info_cr, T2_CODE_RATE) == FALSE)
        printf("[dvbt2]code rate parameter retrieve failure\n");

/*
    while(dvbt2_ssi_dbm_nordigp1[u8_index].constel != _UNKNOW_QAM)
    {
        if ( (dvbt2_ssi_dbm_nordigp1[u8_index].constel == (DMD_T2_CONSTEL)L1_info_qam)
            && (dvbt2_ssi_dbm_nordigp1[u8_index].code_rate == (DMD_T2_CODERATE)L1_info_cr))
        {
           ch_power_ref = dvbt2_ssi_dbm_nordigp1[u8_index].p_ref;
           break;
        }
        else
        {
           u8_index++;
        }
    }
*/
    ch_power_ref = dvbt2_ssi_dbm_nordigp1[(MS_U8)L1_info_qam][(MS_U8)L1_info_cr];

//    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + (0x62*2), &demodState);

    if (ch_power_ref > 10.0f)
        *strength = 0;
    else
    {
		// For Nordig's SSI test items
		if ( (L1_info_qam == 3) //256qam
			&& (L1_info_cr > 0 && L1_info_cr < 4) // CR 3/5,2/3,3/4
			)
		{
			MS_U8 u8_x = L1_info_cr - 1;
			float f_ssi = 0.0;

			if(ch_power_db >= -45)f_ssi = 100;
			else if (ch_power_db >= -50)  f_ssi = fT2_SSI_formula[u8_x][0]*(ch_power_db + 50) + fT2_SSI_formula[u8_x][1];
			else if (ch_power_db >= -60)  f_ssi = fT2_SSI_formula[u8_x][2]*(ch_power_db + 60) + fT2_SSI_formula[u8_x][3];
			else if (ch_power_db >= -70)  f_ssi = fT2_SSI_formula[u8_x][4]*(ch_power_db + 70) + fT2_SSI_formula[u8_x][5];
			else if (ch_power_db >= -80)  f_ssi = fT2_SSI_formula[u8_x][6]*(ch_power_db + 80) + fT2_SSI_formula[u8_x][7];
			else if (ch_power_db >= -95)  f_ssi = fT2_SSI_formula[u8_x][8]*(ch_power_db + 95) + fT2_SSI_formula[u8_x][9];
			else if (ch_power_db >= -100) f_ssi = fT2_SSI_formula[u8_x][10]*(ch_power_db + 100) + fT2_SSI_formula[u8_x][11];

			if (f_ssi > 100) *strength = 100;
			else if (f_ssi < 0) *strength = 0;
			else *strength = (MS_U16)(f_ssi+0.5);

			DBG_GET_SIGNAL(printf(">>> SSI... RF_level=%d, f_ssi=%d, ssi=%d, cr=%d, mod=%d\n", (MS_S16)ch_power_db, (MS_S16)f_ssi, (MS_S16)(*strength), L1_info_cr, L1_info_qam));
		}
		else
		{
			ch_power_rel = ch_power_db - ch_power_ref;
			/*
		        if (demodState != 0x09)
		        {
		            ch_power_rel = ch_power_db - (-50.0f);
		        }
		        else
		        {
		            ch_power_rel = ch_power_db - ch_power_ref;
		        }
			*/
	        if ( ch_power_rel < -15.0f )
	        {
	            *strength = 0;
	        }
	        else if ( ch_power_rel < 0.0f )
	        {
	            *strength = (MS_U16)(2.0f/3*(ch_power_rel + 15.0f));
	        }
	        else if ( ch_power_rel < 20 )
	        {
	            *strength = (MS_U16)(4.0f*ch_power_rel + 10.0f);
	        }
	        else if ( ch_power_rel < 35.0f )
	        {
	            *strength = (MS_U16)(2.0f/3*(ch_power_rel - 20.0f) + 90.0f);
	        }
	        else
	        {
	            *strength = 100;
          }
		}
    }

    if (FALSE == INTERN_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK) )
    {
        *strength = 0;
        return TRUE;
    }

    DBG_GET_SIGNAL(printf(">>> ch_power_ref(dB) = %d , ch_power_db(dB) = %d, ch_power_rel(dB) = %d<<<\n", (MS_S16)ch_power_ref, (MS_S16)ch_power_db, (MS_S16)ch_power_rel));
    DBG_GET_SIGNAL(printf(">>> SSI_CH_PWR(dB) = %d , Score = %d<<<\n", (MS_S16)ch_power_db, *strength));
    DBG_GET_SIGNAL(printf(">>> SSI = %d <<<\n", (int)*strength));

    return status;
}
#endif

#if 0
/****************************************************************************
  Subject:    To get the DVT Signal quility
  Function:   INTERN_DVBT2_GetSignalQuality
  Parmeter:  Quility
  Return:      E_RESULT_SUCCESS
                   E_RESULT_FAILURE
  Remark:    Here we have 4 level range
                  <1>.First Range => Quility =100  (You can define it by INTERN_DVBT2_SIGNAL_BASE_100)
                  <2>.2th Range => 60 < Quality < 100 (You can define it by INTERN_DVBT2_SIGNAL_BASE_60)
                  <3>.3th Range => 10 < Quality < 60  (You can define it by INTERN_DVBT2_SIGNAL_BASE_10)
                  <4>.4th Range => Quality <10
*****************************************************************************/
MS_BOOL INTERN_DVBT2_GetSignalQuality(MS_U16 *quality, const DMD_DVBT2_InitData *sDMD_DVBT2_InitData, MS_U8 u8SarValue, float fRFPowerDbm)
{
//    float   ber_sqi, SQI;
    float   fber;
    float   cn_rec = 0;
    float   cn_ref = 0;
    float   cn_rel = 0;

#if 0
    float   fBerTH1[] = {1E-4, 1E-4*(1.0-DVBT2_BER_TH_HY), 1E-4*(1.0+DVBT2_BER_TH_HY), 1E-4};
    float   fBerTH2[] = {3E-7, 3E-7, 3E-7*(1.0-DVBT2_BER_TH_HY), 3E-7*(1.0+DVBT2_BER_TH_HY)};
    float   BER_SQI = (float)0.0;
    float   SQI = (float)0.0;
    static MS_U8 u8SQIState = 0;
#endif

    MS_U8   status = true;
    MS_U16   L1_info_qam = 0, L1_info_cr = 0, i = 0;

    DBG_INTERN_DVBT2_TIME(printf("INTERN_DVBT2_GetSignalQuality, t=%ld\n",MsOS_GetSystemTime()));

    if (TRUE == INTERN_DVBT2_GetLock(E_DMD_DVBT2_P1_LOCK) )
    {
#if 1 // copy from msb1240
        if (fBerFilteredDVBT2 < 0.0)
        {
            if (INTERN_DVBT2_GetPostLdpcBer(&fber) == FALSE)
            {
                DBG_INTERN_DVBT2(printf("GetPostViterbiBer Fail!\n"));
                return FALSE;
            }
            fBerFilteredDVBT2 = fber;
        }
        else
        {
            fber = fBerFilteredDVBT2;
        }

        if (fber > fBerTH1[u8SQIState])
        {
           BER_SQI = 0.0;
           u8SQIState = 1;
        }
        else if (fber >=fBerTH2[u8SQIState])
        {
           BER_SQI = 100.0/15;
           u8SQIState = 2;
        }
        else
        {
            BER_SQI = 100.0/6;
            u8SQIState = 3;
        }

        cn_rec = INTERN_DVBT2_GetSNR();
        if (cn_rec < 0.0)
            return FALSE;

        ///////// Get Constellation and Code Rate to determine Ref. C/N //////////
        ///////// (refer to Teracom min. spec 2.0 4.1.1.7) /////
        L1_info_qam = 0xff;
        L1_info_cr = 0xff;

        cn_ref = (float)-1.0;
        if(INTERN_DVBT2_Get_L1_Parameter(&L1_info_qam, T2_MODUL_MODE) == FALSE)
            printf("[dvbt2] QAM parameter retrieve failure\n");

        if(INTERN_DVBT2_Get_L1_Parameter(&L1_info_cr, T2_CODE_RATE) == FALSE)
            printf("[dvbt2]code rate parameter retrieve failure\n");

        for(i = 0; i < sDMD_DVBT2_InitData->u16SqiCnNordigP1_Size; i++)
        {
            if ( (L1_info_qam == sDMD_DVBT2_InitData->pSqiCnNordigP1[i].constel)
            && (L1_info_cr == sDMD_DVBT2_InitData->pSqiCnNordigP1[i].code_rate) )
            {
                cn_ref = sDMD_DVBT2_InitData->pSqiCnNordigP1[i].cn_ref;
                break;
            }
        }

        if (cn_ref < 0.0)
        {
            SQI = (float)0.0;
            printf("SQI is zero, 1\n");
        }
        else
        {
            // 0.7, snr offset
            cn_rel = cn_rec - cn_ref + 0.7f;
            if (cn_rel > 3.0)
                SQI = 100;
            else if (cn_rel >= -3)
            {
                SQI = (cn_rel+3)*BER_SQI;
                if (SQI > 100.0) SQI = 100.0;
                else if (SQI < 0.0) SQI = 0.0;
            }
            else
            {
                SQI = (float)0.0;
                printf("SQI is zero, 2\n");
            }
        }

        *quality = (MS_U16)SQI;
#else
        if ( MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime) < 300)
        {
          MsOS_DelayTask(300 - MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime));
        }
        ///////// Get Pre-BCH (Post-LDPC) BER to determine BER_SQI //////////
        if(fLDPCBerFiltered<= 0.0)
        {
            if (INTERN_DVBT2_GetPostLdpcBer(&fber) == FALSE)
            {
                DBG_INTERN_DVBT2(printf("GetPostViterbiBer Fail!\n"));
                return FALSE;
            }
            fLDPCBerFiltered = fber;
        }
        else
        {
            fber = fLDPCBerFiltered;
        }
/*
        if (fber > 1.0E-3)
            ber_sqi = 0.0;
        else if (fber > 8.5E-7)
#ifdef MSOS_TYPE_LINUX
            ber_sqi = (log10f(1.0f/fber))*20.0f - 22.0f;
#else
            ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 22.0f;
#endif
        else
            ber_sqi = 100.0;
*/
        if (fber > 1E-4)
            ber_sqi = 0.0;
        else if (fber >= 1E-7)
            ber_sqi = 100.0 / 15;
        else
            ber_sqi = 100.0 / 6;

        cn_rec = INTERN_DVBT2_GetSNR();

        if (cn_rec == -1)   //get SNR return fail
            status = false;

        ///////// Get Constellation and Code Rate to determine Ref. C/N //////////
        ///////// (refer to Teracom min. spec 2.0 4.1.1.7) /////
        L1_info_qam = 0xff;
        L1_info_cr = 0xff;

        cn_ref = (float)-1.0;
    if(INTERN_DVBT2_Get_L1_Parameter(&L1_info_qam, T2_MODUL_MODE) == FALSE)
        printf("[dvbt2] QAM parameter retrieve failure\n");

    if(INTERN_DVBT2_Get_L1_Parameter(&L1_info_cr, T2_CODE_RATE) == FALSE)
        printf("[dvbt2]code rate parameter retrieve failure\n");

        for(i = 0; i < sDMD_DVBT2_InitData->u16SqiCnNordigP1_Size; i++)
        {
            if ( (L1_info_qam == sDMD_DVBT2_InitData->pSqiCnNordigP1[i].constel)
            && (L1_info_cr == sDMD_DVBT2_InitData->pSqiCnNordigP1[i].code_rate) )
            {
                cn_ref = sDMD_DVBT2_InitData->pSqiCnNordigP1[i].cn_ref;
                break;
            }
        }

         if (cn_ref == -1.0)
            SQI = (float)0.0;
        else
        {
            cn_rel = cn_rec - cn_ref;
            if (cn_rel > 3.0)
                SQI = 100;
            else if (cn_rel >= -3)
            {
                SQI = (cn_rel+3)*ber_sqi;
                if (SQI > 100.0) SQI = 100.0;
                else if (SQI < 0.0) SQI = 0.0;
            }
            else
                SQI = (float)0.0;
        }

        // SQI patch, 256qam, R3/4 CN=20.8, SQI=0~13
        if ((L1_info_qam==_T2_256QAM) && (L1_info_cr==_T2_CR3Y4))
        {
           if ( (cn_rec > 20.6) && (cn_rec < 20.9))
           {
               if (SQI > 3) SQI -= 3;
           }
           else if ( (cn_rec >= 20.9) && (cn_rec < 21.2))
           {
               if (SQI > 9) SQI -= 9;
           }
        }

        *quality = (MS_U16)SQI;
#endif
    }
    else
    {
        *quality = 0;
    }

    DBG_GET_SIGNAL(printf("SNR = %f, QAM = %d, code Rate = %d\n", cn_rec, L1_info_qam, L1_info_cr));
    DBG_GET_SIGNAL(printf("BER = %8.3e\n", fber));
    DBG_GET_SIGNAL(printf("Signal Quility = %d\n", *quality));
    return status;
}
#endif

/****************************************************************************
  Subject:    To get the DVBT Carrier Freq Offset
  Function:   INTERN_DVBT2_Get_FreqOffset
  Parmeter:   Frequency offset (in KHz), bandwidth
  Return:     E_RESULT_SUCCESS
              E_RESULT_FAILURE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBT2_Get_FreqOffset(MS_U32 *CfoTd_reg, MS_U32 *CfoFd_reg, MS_U32 *Icfo_reg, MS_U8 *fft_reg)
{
//    float         N, FreqB;
//    float         FreqCfoTd, FreqCfoFd, FreqIcfo;
//    MS_U32           RegCfoTd, RegCfoFd, RegIcfo;
    MS_U8            reg_frz=0, reg=0;
    MS_U8            status;

#if 0
    FreqB = (float)u8BW * 8 / 7;
#endif

    status = MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x05, &reg_frz);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c8, &reg);
    *CfoTd_reg = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c7, &reg);
    *CfoTd_reg = (*CfoTd_reg << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c6, &reg);
    *CfoTd_reg = (*CfoTd_reg << 8)|reg;
#if 0
    FreqCfoTd = (float)RegCfoTd;

    if (RegCfoTd & 0x800000)
        FreqCfoTd = FreqCfoTd - (float)0x1000000;

    FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;
#endif

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz&(~0x80));

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0xfe, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz|0x01);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x33, &reg);
    *CfoFd_reg = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x32, &reg);
    *CfoFd_reg = (*CfoFd_reg << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x31, &reg);
    *CfoFd_reg = (*CfoFd_reg << 8)|reg;

#if 0
    FreqCfoFd = (float)RegCfoFd;

    if (RegCfoFd & 0x800000)
        FreqCfoFd = FreqCfoFd - (float)0x1000000;

    FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;
#endif

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x19, &reg);
    *Icfo_reg = reg & 0x07;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x18, &reg);
    *Icfo_reg = (*Icfo_reg << 8)|reg;

#if 0
    FreqIcfo = (float)RegIcfo;

    if (RegIcfo & 0x400)
        FreqIcfo = FreqIcfo - (float)0x800;
#endif

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &reg);
    *fft_reg = reg & 0x30;

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz&(~0x01));
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe+1, 0x01);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);

#if 0
    switch (*fft_reg)
    {
        case 0x00:  N = 2048;  break;
        case 0x20:  N = 4096;  break;
        case 0x10:
        default:    N = 8192;  break;
    }

    FreqIcfo = FreqIcfo * FreqB / N * 1000;         //unit: kHz


    //*pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000;
    *pFreqOff = (-1.0f)*(FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000);
    // DBG_GET_SIGNAL(printf("FCFO = %f\n", FreqCfoFd));
    // DBG_GET_SIGNAL(printf("TCFO = %f\n", FreqCfoTd));
    // DBG_GET_SIGNAL(printf("ICFO = %f\n", FreqIcfo));
    DBG_GET_SIGNAL(printf("CFOE = %f\n", *pFreqOff));
#endif

    if (status == TRUE)
        return TRUE;
    else
        return FALSE;
}

#if 0
/****************************************************************************
  Subject:    To get the DVBT Carrier Freq Offset
  Function:   INTERN_DVBT2_Get_FreqOffset
  Parmeter:   Frequency offset (in KHz), bandwidth
  Return:     E_RESULT_SUCCESS
              E_RESULT_FAILURE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBT2_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    float         N, FreqB;
    float         FreqCfoTd, FreqCfoFd, FreqIcfo;
    MS_U32           RegCfoTd, RegCfoFd, RegIcfo;
    MS_U8            reg_frz=0, reg=0;
    MS_U8            status;

    FreqB = (float)u8BW * 8 / 7;

    status = MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x05, &reg_frz);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c8, &reg);
    RegCfoTd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c7, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c6, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    FreqCfoTd = (float)RegCfoTd;

    if (RegCfoTd & 0x800000)
        FreqCfoTd = FreqCfoTd - (float)0x1000000;

    FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz&(~0x80));

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0xfe, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz|0x01);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x33, &reg);
    RegCfoFd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x32, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x31, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    FreqCfoFd = (float)RegCfoFd;

    if (RegCfoFd & 0x800000)
        FreqCfoFd = FreqCfoFd - (float)0x1000000;

    FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x19, &reg);
    RegIcfo = reg & 0x07;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x18, &reg);
    RegIcfo = (RegIcfo << 8)|reg;

    FreqIcfo = (float)RegIcfo;

    if (RegIcfo & 0x400)
        FreqIcfo = FreqIcfo - (float)0x800;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &reg);
    reg = reg & 0x30;

    switch (reg)
    {
        case 0x00:  N = 2048;  break;
        case 0x20:  N = 4096;  break;
        case 0x10:
        default:    N = 8192;  break;
    }

    FreqIcfo = FreqIcfo * FreqB / N * 1000;         //unit: kHz
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz&(~0x01));
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe+1, 0x01);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);
    //*pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000;
    *pFreqOff = (-1.0f)*(FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000);
    // DBG_GET_SIGNAL(printf("FCFO = %f\n", FreqCfoFd));
    // DBG_GET_SIGNAL(printf("TCFO = %f\n", FreqCfoTd));
    // DBG_GET_SIGNAL(printf("ICFO = %f\n", FreqIcfo));
    DBG_GET_SIGNAL(printf("CFOE = %f\n", *pFreqOff));

    if (status == TRUE)
        return TRUE;
    else
        return FALSE;
}
#endif

void INTERN_DVBT2_Power_ON_OFF(MS_U8 bPowerOn)
{

    bPowerOn = bPowerOn;
}

MS_BOOL INTERN_DVBT2_Power_Save(void)
{

    return TRUE;
}

MS_BOOL INTERN_DVBT2_Version(MS_U16 *ver)
{

    MS_U8 status = true;
    MS_U8 tmp = 0;
    MS_U16 u16_INTERN_DVBT2_Version;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC1, &tmp);
    u16_INTERN_DVBT2_Version = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC2, &tmp);
    u16_INTERN_DVBT2_Version = u16_INTERN_DVBT2_Version<<8|tmp;
    *ver = u16_INTERN_DVBT2_Version;

    return status;
}

MS_BOOL INTERN_DVBT2_Version_minor(MS_U8 *ver2)
{

    MS_U8 status = true;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC3, ver2);

    return status;
}


MS_BOOL INTERN_DVBT2_Show_Demod_Version(void)
{

    MS_BOOL status = true;
    MS_U16 u16_INTERN_DVBT2_Version = 0;
    MS_U8  u8_minor_ver = 0;

    status &= INTERN_DVBT2_Version(&u16_INTERN_DVBT2_Version);
    status &= INTERN_DVBT2_Version_minor(&u8_minor_ver);
    printf("[DVBT]Version = 0x%x,0x%x\n",u16_INTERN_DVBT2_Version,u8_minor_ver);

    return status;
}

#if 0
MS_BOOL INTERN_DVBT2_NORDIG_SSI_Table_Write(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float write_value)
{
    dvbt2_ssi_dbm_nordigp1[constel][code_rate] = write_value;
    return TRUE;
/*
    MS_U8   u8_index = 0;
    MS_BOOL bRet     = false;

    while(dvbt2_ssi_dbm_nordigp1[u8_index].constel != _UNKNOW_QAM)
    {
        if ( (dvbt2_ssi_dbm_nordigp1[u8_index].constel == (DMD_T2_CONSTEL)constel)
            && (dvbt2_ssi_dbm_nordigp1[u8_index].code_rate == (DMD_T2_CODERATE)code_rate))
        {
           dvbt2_ssi_dbm_nordigp1[u8_index].p_ref = write_value;
           bRet = true;
           break;
        }
        else
        {
           u8_index++;
        }
    }
    return bRet;
*/
}

MS_BOOL INTERN_DVBT2_NORDIG_SSI_Table_Read(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float *read_value)
{
    *read_value = dvbt2_ssi_dbm_nordigp1[constel][code_rate];
    return TRUE;
/*
    MS_U8   u8_index = 0;
    MS_BOOL bRet     = false;

    while(dvbt2_ssi_dbm_nordigp1[u8_index].constel != _UNKNOW_QAM)
    {
        if ( (dvbt2_ssi_dbm_nordigp1[u8_index].constel == (DMD_T2_CONSTEL)constel)
            && (dvbt2_ssi_dbm_nordigp1[u8_index].code_rate == (DMD_T2_CODERATE)code_rate))
        {
           *read_value = dvbt2_ssi_dbm_nordigp1[u8_index].p_ref;
           bRet = true;
           break;
        }
        else
        {
           u8_index++;
        }
    }
    return bRet;
    */
}
#endif

MS_BOOL INTERN_DVBT2_GetPlpBitMap(MS_U8* u8PlpBitMap)
{
    MS_BOOL   status = TRUE;
    MS_U8     u8Data = 0;
    MS_U8     indx = 0;

    DBG_INTERN_DVBT2(printf("INTERN_DVBT2_GetPlpBitMap\n"));

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_T2_L1_FLAG, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
    {
        DBG_INTERN_DVBT2(printf("\n[INTERN_DVBT2_GetPlpBitMap] Check L1 NOT Ready !! E_DMD_T2_L1_FLAG = 0x%x\n", u8Data));
        return FALSE;
    }
    while (indx < 32)
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_T2_PLP_ID_ARR + indx, &u8Data);
        u8PlpBitMap[indx] = u8Data;
        indx++;
    }

    if (status)
    {
        DBG_INTERN_DVBT2(printf("\n+++++++++u8PlpBitMap data+++++++++++++++\n"));
        for (indx = 0; indx < 32; indx++)
            DBG_INTERN_DVBT2(printf("[%d] ", u8PlpBitMap[indx]));
        DBG_INTERN_DVBT2(printf("\n+++++++++u8PlpBitMap end+++++++++++++++\n"));
    }
    return status;
}

MS_BOOL INTERN_DVBT2_GetPlpGroupID(MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    MS_BOOL   status = TRUE;
    MS_U8 u8Data = 0;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_T2_L1_FLAG, &u8Data);         // check L1 ready
    if (u8Data != 0x30)
    {
        printf(">>>dvbt2 L1 not ready yet\n");
        return FALSE;
    }
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_T2_DVBT2_LOCK_HIS, &u8Data);

    if ((u8Data & BIT(7)) == 0x00)
    {
        printf(">>>dvbt2 is un-lock\n");
        return FALSE;
    }
    // assign PLP-ID value
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2L1_REG_BASE + (0x78) * 2, u8PlpID);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2L1_REG_BASE + (0x01) * 2 + 1, 0x01); // MEM_EN
    MsOS_DelayTask(1);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(T2L1_REG_BASE + (0x79) * 2, u8GroupID);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(T2L1_REG_BASE + (0x01) * 2 + 1, 0x00); // ~MEM_EN

    return status;
}

MS_BOOL INTERN_DVBT2_SetPlpGroupID(MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    MS_BOOL   status = TRUE;

    // assign Group-ID and PLP-ID value (must be written in order)
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_T2_GROUP_ID, u8GroupID);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_T2_PLP_ID, u8PlpID);

    return status;
}

#if (INTERN_DVBT2_INTERNAL_DEBUG == 1)
void INTERN_DVBT2_get_demod_state(MS_U8* state)
{
   MDrv_SYS_DMD_VD_MBX_ReadReg(0x23E0, state);
   return;
}

MS_BOOL INTERN_DVBT2_Show_ChannelLength(void)
{
    MS_U8 status = true;
    MS_U8 tmp = 0;
    MS_U16 len = 0;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x71,&tmp);
    len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x70,&tmp);
    len = (len<<8)|tmp;
    printf("[dvbt]Hw_channel=%d\n",len);
    return status;
}

MS_BOOL INTERN_DVBT2_Show_SW_ChannelLength(void)
{
    MS_U8 status = true;
    MS_U8 tmp = 0,peak_num = 0,insideGI = 0,stoptracking = 0,flag_short_echo = 0,fsa_mode = 0;
    MS_U16 sw_len = 0;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C4,&tmp);
    sw_len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C3,&tmp);
    sw_len = (sw_len<<8)|tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C2,&tmp);
    peak_num = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C5,&tmp);
    insideGI = tmp&0x01;
    stoptracking = (tmp&0x02)>>1;
    flag_short_echo = (tmp&0x0C)>>2;
    fsa_mode = (tmp&0x30)>>4;

    printf("[dvbt]SW_len=%d, peak_num=%d, insideGI=%d, stoptrack=%d, short_echo=%d, fsa_mode=%d\n",
        sw_len,peak_num,insideGI,stoptracking,flag_short_echo,fsa_mode);

    return status;
}

MS_BOOL INTERN_DVBT2_Show_ACI_CI(void)
{

    #define BIT4 0x10
    MS_U8 status = true;
    MS_U8 digACI =0 ,flag_CI = 0,td_coef = 0,tmp = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2357,&tmp);
    digACI = (tmp&BIT4)>>4;

    // get flag_CI
    // 0: No interference
    // 1: CCI
    // 2: in-band ACI
    // 3: N+1 ACI
    // flag_ci = (tmp&0xc0)>>6;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2356,&tmp);
    flag_CI = (tmp&0xC0)>>6;
    td_coef = (tmp&0x0C)>>2;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20E8,&tmp);

    printf("[dvbt]DigACI=%d, Flag_CI=%d, td_coef=%d\n",digACI,flag_CI,td_coef);

    return status;
}

MS_BOOL INTERN_DVBT2_Show_FD_CH_LEN_S_SEL(void)
{
    MS_U8 status = true;
    MS_U8 tmp = 0,fd = 0,ch_len = 0,snr_sel = 0,pertone_num = 0;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2355, &tmp);
    fd = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2354, &tmp);
    ch_len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x46, &tmp);
    snr_sel = (tmp>>4)&0x03;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x22AC, &tmp);
    pertone_num = tmp;

    printf("[dvbt]fd=0x%x, ch_len=0x%x, snr_sel=0x%x, pertone_num=0x%x\n",fd,ch_len,snr_sel,pertone_num);

    return status;
}

MS_BOOL INTERN_DVBT2_Get_CFO(void)
{
#if 0
    float         N = 0, FreqB = 0;
    float         FreqCfoTd = 0, FreqCfoFd = 0, FreqIcfo = 0, total_cfo = 0;
    MS_U32        RegCfoTd = 0, RegCfoFd = 0, RegIcfo = 0;
    MS_U8         reg_frz = 0, reg = 0;
    MS_U8         status = 0;
    MS_U8         u8BW = 8;

    FreqB = (float)u8BW * 8 / 7;

    status = MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x05, &reg_frz);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c8, &reg);
    RegCfoTd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c7, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c6, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    FreqCfoTd = (float)RegCfoTd;

    if (RegCfoTd & 0x800000)
        FreqCfoTd = FreqCfoTd - (float)0x1000000;

    FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz&(~0x80));

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0xfe, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz|0x01);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x33, &reg);
    RegCfoFd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x32, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x31, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    FreqCfoFd = (float)RegCfoFd;

    if (RegCfoFd & 0x800000)
        FreqCfoFd = FreqCfoFd - (float)0x1000000;

    FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x19, &reg);
    RegIcfo = reg & 0x07;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x18, &reg);
    RegIcfo = (RegIcfo << 8)|reg;

    FreqIcfo = (float)RegIcfo;

    if (RegIcfo & 0x400)
        FreqIcfo = FreqIcfo - (float)0x800;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &reg);
    reg = reg & 0x30;

    switch (reg)
    {
        case 0x00:  N = 2048;  break;
        case 0x20:  N = 4096;  break;
        case 0x10:
        default:    N = 8192;  break;
    }

    FreqIcfo = FreqIcfo * FreqB / N * 1000;         //unit: kHz
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz&(~0x01));
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe+1, 0x01);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);
    total_cfo = FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000;

    printf("[CFO]t_cfo=%f Hz, f_cfo=%f Hz, icfo=%f KHz, cfo=%f KHz\n", FreqCfoTd,FreqCfoFd,FreqIcfo,total_cfo);
    return status;
#endif
    return true;
}
MS_BOOL INTERN_DVBT2_Get_SFO(void)
{
#if 0
    MS_U32 Reg_TDP_SFO = 0, Reg_FDP_SFO = 0, Reg_FSA_SFO = 0, Reg_FSA_IN = 0;
    MS_BOOL status = true;
    MS_U8  reg = 0;
    float  FreqB = 9.143, FreqS = 45.473;  //20.48
    float  Float_TDP_SFO = 0, Float_FDP_SFO = 0, Float_FSA_SFO = 0, Float_FSA_IN = 0;
    float  sfo_value = 0;

    // get Reg_TDP_SFO,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0xCC, &reg);
    Reg_TDP_SFO = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0xCB, &reg);
    Reg_TDP_SFO = (Reg_TDP_SFO<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0xCA, &reg);
    Reg_TDP_SFO = (Reg_TDP_SFO<<8)|reg;

    Float_TDP_SFO = (float)((MS_S32)(Reg_TDP_SFO<<8))/256*FreqB/FreqS*0.0018626;

    // get Reg_FDP_SFO,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x30, &reg);
    Reg_FDP_SFO = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x2F, &reg);
    Reg_FDP_SFO = (Reg_FDP_SFO<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x2E, &reg);
    Reg_FDP_SFO = (Reg_FDP_SFO<<8)|reg;

    Float_FDP_SFO = (float)((MS_S32)(Reg_FDP_SFO<<8))/256*FreqB/FreqS*0.0018626;

    // get Reg_FSA_SFO,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8C, &reg);
    Reg_FSA_SFO = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8B, &reg);
    Reg_FSA_SFO = (Reg_FSA_SFO<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8A, &reg);
    Reg_FSA_SFO = (Reg_FSA_SFO<<8)|reg;

    // get Reg_FSA_IN,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8F, &reg);
    Reg_FSA_IN = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8E, &reg);
    Reg_FSA_IN = (Reg_FSA_IN<<8)|reg;
    Float_FSA_IN = (float)((MS_S32)(Reg_FSA_IN<<19))/512/2048;

    //Float_FSA_SFO = (float)((MS_S16)(Reg_FSA_SFO<<8))/256*FreqB/FreqS*0.0037253;
    Float_FSA_SFO = (float)((MS_S32)(Reg_FSA_SFO<<8))/256*FreqB/FreqS*0.0018626;

    sfo_value = Float_TDP_SFO + Float_FDP_SFO + Float_FSA_SFO;
    // printf("\nReg_FSA_SFO = 0x%x\n",Reg_FSA_SFO);
    printf("[SFO]tdp_sfo=%f, fdp_sfo=%f, fsa_sfo=%f, Tot_sfo=%f, fsa_sfo_in=%f\n",Float_TDP_SFO,Float_FDP_SFO,Float_FSA_SFO,sfo_value,Float_FSA_IN);


    return status;
#endif
    return true;
}

void INTERN_DVBT2_Get_SYA_status(void)
{
    MS_U8  status = true;
    MS_U8  sya_k = 0,reg = 0;
    MS_U16 sya_th = 0,len_a = 0,len_b = 0,len_m = 0,sya_offset = 0,tracking_reg = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x6F, &reg);
    sya_k = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x69, &reg);
    sya_th = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x68, &reg);
    sya_th = (sya_th<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x95, &reg);
    sya_offset = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x94, &reg);
    sya_offset = (sya_offset<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x71, &reg);
    len_m = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x70, &reg);
    len_m = (len_m<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x87, &reg);
    len_b = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x86, &reg);
    len_b = (len_b<<8)|reg;


    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x89, &reg);
    len_a = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x88, &reg);
    len_a = (len_a<<8)|reg;


    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x66, &reg);
    tracking_reg = reg;


    printf("[SYA][1]sya_k = 0x%x, sya_th = 0x%x, sya_offset=0x%x\n",sya_k,sya_th,sya_offset);
    printf("[SYA][2]track_reg=0x%x, len_m = %d, len_e = %d [%d,%d]\n",tracking_reg,len_m,len_b-len_a,len_a,len_b);

    return;
}

void INTERN_DVBT2_Get_cci_status(void)
{
    MS_U8  status = true;
    MS_U8 cci_fsweep = 0,cci_kp = 0,reg = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x08, &reg);
    cci_fsweep = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x0A, &reg);
    cci_kp = reg;

    printf("[CCI]fsweep=0x%x, k=0x%x\n",cci_fsweep,cci_kp);

    return;
}

MS_BOOL INTERN_DVBT2_Show_PRESFO_Info(void)
{
    MS_U8 tmp = 0;
    MS_BOOL status = TRUE;
    printf("\n[SFO]");
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D0,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D1,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D2,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D3,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D4,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D5,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D6,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D7,&tmp);
    printf("[%x][End]",tmp);

    return status;
}

MS_BOOL INTERN_DVBT2_Get_Lock_Time_Info(MS_U16 *locktime)
{
    MS_BOOL status = true;

    *locktime = 0xffff;
    printf("[dvbt]INTERN_DVBT2_Get_Lock_Time_Info not implement\n");

    status = false;
    return status;
}


MS_BOOL INTERN_DVBT2_Show_Lock_Time_Info(void)
{
    MS_U16 locktime = 0;
    MS_BOOL status = TRUE;
    status &= INTERN_DVBT2_Get_Lock_Time_Info(&locktime);
    printf("[DVBT]lock_time = %d ms\n",locktime);
    return status;
}

MS_BOOL INTERN_DVBT2_Show_BER_Info(void)
{
    MS_U8 tmp = 0;
    MS_BOOL status = TRUE;
    printf("\n[BER]");
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C6,&tmp);
    printf("[%x,",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C7,&tmp);
    printf("%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C8,&tmp);
    printf("[%x,",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C9,&tmp);
    printf("%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20CA,&tmp);
    printf("[%x,",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20CB,&tmp);
    printf("%x][End]",tmp);

    return status;

}


MS_BOOL INTERN_DVBT2_Show_AGC_Info(void)
{
    MS_U8 tmp = 0;
    MS_U8 agc_k = 0,agc_ref = 0,d1_k = 0,d1_ref = 0,d2_k = 0,d2_ref = 0;
    MS_U16 if_agc_gain = 0,d1_gain = 0,d2_gain = 0;
    MS_U16 if_agc_err = 0;
    MS_BOOL status = TRUE;
    MS_U8  agc_lock = 0, d1_lock = 0, d2_lock = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x05,&agc_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x07,&agc_ref);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x82,&d1_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x84,&d1_ref);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x00,&d2_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x01,&d2_ref);


    // select IF gain to read
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x16, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTN_REG_BASE + 0x16, (tmp&0xF0)|0x03);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x19, &tmp);
    if_agc_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x18, &tmp);
    if_agc_gain = (if_agc_gain<<8)|tmp;


    // select d1 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x8c, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTN_REG_BASE + 0x8c, (tmp&0xF0)|0x02);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x89, &tmp);
    d1_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x88, &tmp);
    d1_gain = (d1_gain<<8)|tmp;

    // select d2 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x06, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTNEXT_REG_BASE + 0x06, (tmp&0xF0)|0x02);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x09, &tmp);
    d2_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x08, &tmp);
    d2_gain = (d2_gain<<8)|tmp;

    // select IF gain err to read
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x16, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTN_REG_BASE + 0x16, (tmp&0xF0)|0x00);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x19, &tmp);
    if_agc_err = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x18, &tmp);
    if_agc_err = (if_agc_err<<8)|tmp;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x1d, &agc_lock);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x99, &d1_lock);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x05, &d2_lock);



    printf("[dvbt]agc_k=0x%x, agc_ref=0x%x, d1_k=0x%x, d1_ref=0x%x, d2_k=0x%x, d2_ref=0x%x\n",
        agc_k,agc_ref,d1_k,d1_ref,d2_k,d2_ref);

    printf("[dvbt]agc_g=0x%x, d1_g=0x%x, d2_g=0x%x, agc_err=0x%x\n",if_agc_gain,d1_gain,d2_gain,if_agc_err);
    printf("[dvbt]agc_lock=0x%x, d1_lock=0x%x, d2_lock=0x%x\n",agc_lock,d1_lock,d2_lock);

    return status;
}

MS_BOOL INTERN_DVBT2_Show_WIN_Info(void)
{
    MS_U8 tmp = 0;
    MS_U8 trigger = 0;
    MS_U16 win_len = 0;

    MS_BOOL status = TRUE;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x0B,&tmp);
    win_len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x0A,&tmp);
    win_len = (win_len<<8)|tmp;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x08,&trigger);

    printf("[dvbt]win_len = %d, trigger=0x%x\n",win_len,trigger);

    return status;
}

void INTERN_DVBT2_Show_td_coeff(void)
{
    MS_U8  status = true;
    MS_U8 w1 = 0,w2 = 0,reg = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2561, &reg);
    w1 = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2562, &reg);
    w2 = reg;

    printf("[td]w1=0x%x, w2=0x%x\n",w1,w2);

    return;
}

/********************************************************
*Constellation (b2 ~ b0)  : 0~3 => QPSK, 16QAM, 64QAM, 256QAM
*Code Rate (b5 ~ b3)   : 0~5 => 1/2, 3/5, 2/3, 3/4, 4/5, 5/6
*GI (b8 ~ b6)           : 0~6 => 1/32, 1/16, 1/8, 1/4, 1/128, 19/128, 19/256
*FFT (b11 ~ b9)        : 0~7 => 2K, 8K, 4K, 1K, 16K, 32K, 8KE, 32KE
*Preamble(b12)      : 0~1 => mixed, not_mixed
*S1_Signaling(b14~b13)   : 0~3 => t2_siso, t2_miso, "non_t2, reserved
*pilot_pattern(b18~b15)    : 0~8 => PP1, PP2, PP3, PP4, PP5, PP6, PP7, PP8
*BW_Extend(b19)             : 0~1 => normal, extension
*PAPR(b22~b20)              : 0~4 => none, ace, tr, tr_and_ace, reserved
 ********************************/
MS_BOOL INTERN_DVBT2_Show_Modulation_info(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16    u16Data = 0;

    char*  cConStr[] = {"qpsk", "16qam", "64qam", "256qam"};
    char*  cCRStr[] = {"1_2", "3_5", "2_3", "3_4", "4_5", "5_6"};
    char*  cGIStr[] = {"1_32", "1_16", "1_8", "1_4", "1_128", "19_128", "19_256"};
    char*  cFFTStr[] = {"2k", "8k", "4k", "1k", "16k", "32k", "8k", "32k"};
    char*  cPreAStr[] = {"mixed", "not_mixed"};
    char*  cS1SStr[] = {"t2_siso", "t2_miso", "non_t2", "reserved"};
    char*  cPPSStr[] = {"PP1", "PP2", "PP3", "PP4", "PP5", "PP6", "PP7", "PP8", "reserved"};
    char*  cBWStr[] = {"normal", "extension"};
    char*  cPAPRStr[] = {"none", "ace", "tr", "tr_and_ace", "reserved"};

    if (INTERN_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK) == TRUE)
    {

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_MODUL_MODE) == FALSE)
        {
            printf("T2_MODUL_MODE Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x07;
        //*L1_Info = (MS_U64)(u16Data);
        printf("T2 Constellation:%s\n", cConStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_CODE_RATE) == FALSE)
        {
            printf(("T2_CODE_RATE Error!\n"));
            bRet = FALSE;
        }
        u16Data &= 0x07;
        //*L1_Info |= (MS_U64)(u16Data << 3);
        printf("T2 Code Rate:%s\n", cCRStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_GUARD_INTERVAL) == FALSE)
        {
            printf("T2_GUARD_INTERVAL Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x07;
        //*L1_Info |= (MS_U64)(u16Data << 6);
        printf("T2 GI:%s\n", cGIStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_FFT_VALUE) == FALSE)
        {
            printf("T2_FFT_VALUE Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x07;
        //*L1_Info |= (MS_U64)(u16Data << 9);
        printf("T2 FFT:%s\n", cFFTStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_PREAMBLE) == FALSE)
        {
            printf("T2_PREAMBLE Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x01;
        //*L1_Info |= (MS_U64)(u16Data << 12);
        printf("Preamble:%s\n", cPreAStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_S1_SIGNALLING) == FALSE)
        {
            printf("T2_S1_SIGNALLING Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x03;
        if (u16Data > 2)
            u16Data = 3;
        //*L1_Info |= (MS_U64)(u16Data << 13);
        printf("S1 Signalling:%s\n", cS1SStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_PILOT_PATTERN) == FALSE)
        {
            printf("T2_PILOT_PATTERN Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x0F;
        if (u16Data > 7)
            u16Data = 8;
        //*L1_Info |= (MS_U64)(u16Data << 15);
        printf("PilotPattern:%s\n", cPPSStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_BW_EXT) == FALSE)
        {
            printf("T2_BW_EXT Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x01;
        //*L1_Info |= (MS_U64)(u16Data << 19);
        printf("BW EXT:%s\n", cBWStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_PAPR_REDUCTION) == FALSE)
        {
            printf("T2_PAPR_REDUCTION Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0x07;
        if (u16Data > 3)
            u16Data = 4;
        //*L1_Info |= (MS_U64)(u16Data << 20);
        printf("T2 PAPR:%s\n", cPAPRStr[u16Data]);

        if (INTERN_DVBT2_Get_L1_Parameter(&u16Data, T2_OFDM_SYMBOLS_PER_FRAME) == FALSE)
        {
            printf("T2_OFDM_SYMBOLS_PER_FRAME Error!\n");
            bRet = FALSE;
        }
        u16Data &= 0xFFF;
        //*L1_Info |= (MS_U64)(u16Data << 23);
        printf("T2 OFDM Symbols:%u\n", u16Data);
    }
    else
    {
        printf("INVALID\n");
        return FALSE;
    }

    return bRet;

}


void INTERN_DVBT2_Show_BER_PacketErr(void)
{
//  float  f_ber = 0;
  MS_U16 packetErr = 0;
//  INTERN_DVBT2_GetPostLdpcBer(&f_ber);
  INTERN_DVBT2_GetPacketErr(&packetErr);

//  printf("[dvbt]ber=%f, Err=%d\n",f_ber, packetErr);
  return;
}

MS_BOOL INTERN_DVBT2_Show_Lock_Info(void)
{

  printf("[dvbt]INTERN_DVBT2_Show_Lock_Info not implement!!!\n");
  return false;
}


MS_BOOL INTERN_DVBT2_Show_Demod_Info(void)
{
  MS_U8         demod_state = 0;
  MS_BOOL       status = true;
  static MS_U8  counter = 0;

  INTERN_DVBT2_get_demod_state(&demod_state);

  printf("==========[dvbt]state=%d\n",demod_state);
  if (demod_state < 5)
  {
    INTERN_DVBT2_Show_Demod_Version();
    INTERN_DVBT2_Show_AGC_Info();
    INTERN_DVBT2_Show_ACI_CI();
  }
  else if(demod_state < 8)
  {
    INTERN_DVBT2_Show_Demod_Version();
    INTERN_DVBT2_Show_AGC_Info();
    INTERN_DVBT2_Show_ACI_CI();
    INTERN_DVBT2_Show_ChannelLength();
    INTERN_DVBT2_Get_CFO();
    INTERN_DVBT2_Get_SFO();
    INTERN_DVBT2_Show_td_coeff();
  }
  else if(demod_state < 11)
  {
    INTERN_DVBT2_Show_Demod_Version();
    INTERN_DVBT2_Show_AGC_Info();
    INTERN_DVBT2_Show_ACI_CI();
    INTERN_DVBT2_Show_ChannelLength();
    INTERN_DVBT2_Get_CFO();
    INTERN_DVBT2_Get_SFO();
    INTERN_DVBT2_Show_FD_CH_LEN_S_SEL();
    INTERN_DVBT2_Get_SYA_status();
    INTERN_DVBT2_Show_td_coeff();
  }
  else if((demod_state == 11) && ((counter%4) == 0))
  {
    INTERN_DVBT2_Show_Demod_Version();
    INTERN_DVBT2_Show_AGC_Info();
    INTERN_DVBT2_Show_ACI_CI();
    INTERN_DVBT2_Show_ChannelLength();
    INTERN_DVBT2_Get_CFO();
    INTERN_DVBT2_Get_SFO();
    INTERN_DVBT2_Show_FD_CH_LEN_S_SEL();
    INTERN_DVBT2_Get_SYA_status();
    INTERN_DVBT2_Show_td_coeff();
    INTERN_DVBT2_Show_Modulation_info();
    INTERN_DVBT2_Show_BER_PacketErr();
  }
  else
    status = false;

  printf("===========================\n");
  counter++;

  return status;
}
#endif