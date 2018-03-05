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
// ********************************************************
//
//  $Workfile: PCMCIA.c $
//
//   Author/Copyright   Gero Kuehn / GkWare e.K.
//                      Humboldtstrasse 177
//                      45149 Essen
//                      GERMANY
//                      Tel: +49 174 520 8026
//                      Email: support@gkware.com
//                      Web: http://www.gkware.com
//
//  $Date: 2006-06-22 23:05:10 +0200 (Do, 22 Jun 2006) $
//  $Revision: 21 $
//
// ********************************************************
//
//  This file contains the portable PCMCIA CIS decoder
//

///////////////////////////////////////////////////////////////////////////////
///
/// file    drvPCMCIA.c
/// @brief  PCMCIA Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
/*                       Header Files                                        */
/*****************************************************************************/
// Common Definition

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvPCMCIA.h"
#include "drvMMIO.h"
#include "halPCMCIA.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "pcmcia_private.h"
#include <string.h>

#if defined ( MSOS_TYPE_LINUX )
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
    #include "mdrv_system_io.h"
#endif

#ifdef CONFIG_PCMCIA_MSPI
#include "drvMSPI.h"
#endif
/*****************************************************************************/
/*                       Define                                              */
/*****************************************************************************/
#define PCMCIA_MAX_DETECT_COUNT         1UL
#define PCMCIA_MAX_POLLING_COUNT        20000UL
#define PCMCIA_DEFAULT_RESET_DURATION   20UL
#define PCMCIA_HW_MAX_RETRY_COUNT       100UL      //  PCMCIA hardware register maximum access times

#define PCMCIA_DEBUG_ENABLE             FALSE
#define PCMCIA_DUMP_REG                 FALSE

#define PCMCIA_UTOPIA2                  TRUE

//#define ECOS_OBERON
#ifdef ECOS_OBERON
#undef PCMCIA_IRQ_ENABLE
#define PCMCIA_IRQ_ENABLE 0UL
extern int  diag_printf( const char *fmt, ... );  /* Formatted print      */
#define printf                          diag_printf
#define REG(addr)                       (*(volatile MS_U32 *)(addr))
#endif

//#define AEON_CHAKRA
#ifdef AEON_CHAKRA
#undef PCMCIA_IRQ_ENABLE
#define PCMCIA_IRQ_ENABLE 0UL
extern void msAPI_Timer_Delayms(MS_U32 u32DelayTime);
extern MS_U32 msAPI_Timer_GetTime0(void);
#define MsOS_DelayTask msAPI_Timer_Delayms
#define MsOS_GetSystemTime msAPI_Timer_GetTime0
#endif

/*****************************************************************************/
/*                       Global Variables                                    */
/*****************************************************************************/
extern PCMCIA_RESOURCE_PRIVATE* pPcmEnv;
#if PCMCIA_IRQ_ENABLE
static MS_BOOL  _gbPCMCIA_Irq[E_PCMCIA_MODULE_MAX] = {DISABLE};
static MS_BOOL  _gbPCMCIA_IrqStatus[E_PCMCIA_MODULE_MAX] = {FALSE};
#endif
static MS_U8    _gu8PCMCIA_Command[E_PCMCIA_MODULE_MAX] = {0};
static MS_U8    _gu8HW_ResetDuration    = PCMCIA_DEFAULT_RESET_DURATION;

#if defined ( MSOS_TYPE_LINUX )
static MS_S32 SYS_fd    = -1;

#if PCMCIA_UTOPIA2
    extern void* pModulePcm;
    extern void* psResource;
#else
    static MS_S32 Pcmcia_Mutex= -1;
#endif
/* PCMCIA_MAP_IOC_INFO */
typedef struct
{
    MS_U16                  u16Addr;
    MS_U8                   u8Value;
    MS_U8                   u8Type;     // 1: AttribMem, 2: IOMem
    MS_U16                  u16DataLen;
    MS_U8 *                 u8pReadBuffer;
    MS_U8 *                 u8pWriteBuffer;
} PCMCIA_Map_Info_t;
#endif

#ifdef CONFIG_PCMCIA_MSPI_BURST
static MS_U8 u8MspiBuf[256];
#endif
/*****************************************************************************/
/*                       Macro                                               */
/*****************************************************************************/
#if PCMCIA_DEBUG_ENABLE
#define PCMCIA_DEBUG( x )           printf x
#else
#define PCMCIA_DEBUG( x )
#endif

#define pcmcia_min( x, y )          ( (MS_U8)x < (MS_U8)y ? (MS_U8)x : (MS_U8)y )

#if defined ( MSOS_TYPE_LINUX )
    #if PCMCIA_UTOPIA2
        #define PCM_ENTER()     if(UTOPIA_STATUS_FAIL == UtopiaResourceObtain(pModulePcm, E_PCMCIA_RESOURCE, &psResource)) \
                                {                                                                       \
                                    ULOGE("PCMCIA", "[PCMCIA][%06d] Mutex taking timeout\n", __LINE__);          \
                                }               
                                
        #define PCM_EXIT()      UtopiaResourceRelease(psResource)
    #else
        #define PCM_ENTER()     if (!MsOS_ObtainMutex(Pcmcia_Mutex, MSOS_WAIT_FOREVER))                 \
                                {                                                                       \
                                    ULOGE("PCMCIA", "[PCMCIA][%06d] Mutex taking timeout\n", __LINE__);          \
                                }
        
        #define PCM_EXIT()      MsOS_ReleaseMutex(Pcmcia_Mutex)    
    #endif    
#else
    #define PCM_ENTER()
    #define PCM_EXIT()
#endif

#define BIT0  0x0001UL
#define BIT1  0x0002UL
#define BIT2  0x0004UL
#define BIT3  0x0008UL
#define BIT4  0x0010UL
#define BIT5  0x0020UL
#define BIT6  0x0040UL
#define BIT7  0x0080UL
#define BIT8  0x0100UL
#define BIT9  0x0200UL
#define BIT10 0x0400UL
#define BIT11 0x0800UL
#define BIT12 0x1000UL
#define BIT13 0x2000UL
#define BIT14 0x4000UL
#define BIT15 0x8000UL

#define PCMCIA_FIRE_COMMAND         BIT0
#define PCMCIA_CLEAN_STATE_RD_DONE  BIT1
#define PCMCIA_STATE_RD_DONE        BIT0
#define PCMCIA_STATE_BUS_IDLE       BIT1
#define PCMCIA_DETECT_PIN_MODULEA   BIT2
#define PCMCIA_DETECT_PIN_MODULEB   BIT3

/* Table 2-6 Tuple Summary Tabl (Spec P.24)*/
/* Layer 1 Tuples */
#define CISTPL_NULL                 0x00UL
#define CISTPL_DEVICE               0x01UL
#define CISTPL_LONGLINK_CB          0x02UL
#define CISTPL_INDIRECT             0x03UL
#define CISTPL_CONFIG_CB            0x04UL
#define CISTPL_CFTABLE_ENTRY_CB     0x05UL
#define CISTPL_LONGLINK_MFC         0x06UL
#define CISTPL_BAR                  0x07UL
#define CISTPL_PWR_MGMNT            0x08UL
#define CISTPL_EXTDEVICE            0x09UL
#define CISTPL_CHECKSUM             0x10UL
#define CISTPL_LONGLINK_A           0x11UL
#define CISTPL_LONGLINK_C           0x12UL
#define CISTPL_LINKTARGET           0x13UL
#define CISTPL_NO_LINK              0x14UL
#define CISTPL_VERS_1               0x15UL
#define CISTPL_ALTSTR               0x16UL
#define CISTPL_DEVICE_A             0x17UL
#define CISTPL_JEDEC_C              0x18UL
#define CISTPL_JEDEC_A              0x19UL
#define CISTPL_CONFIG               0x1AUL
#define CISTPL_CFTABLE_ENTRY        0x1BUL
#define CISTPL_DEVICE_OC            0x1CUL
#define CISTPL_DEVICE_OA            0x1DUL
#define CISTPL_DEVICE_GEO           0x1EUL
#define CISTPL_DEVICE_GEO_A         0x1FUL
#define CISTPL_MANFID               0x20UL
#define CISTPL_FUNCID               0x21UL
#define CISTPL_FUNCE                0x22UL
#define CISTPL_END                  0xFFUL
/* Layer 2 Tuples */
#define CISTPL_SWIL                 0x23UL
#define CISTPL_VERS_2               0x40UL
#define CISTPL_FORMAT               0x41UL
#define CISTPL_GEOMETRY             0x42UL
#define CISTPL_BYTEORDER            0x43UL
#define CISTPL_DATE                 0x44UL
#define CISTPL_BATTERY              0x45UL
#define CISTPL_FORMAT_A             0x47UL
/* Layer 3 Tuples */
#define CISTPL_ORG                  0x46UL
/* Layer 4 Tuples */
#define CISTPL_SPCL                 0x90UL

/*****************************************************************************/
/*                       Local Variables                                     */
/*****************************************************************************/
/// Version string
static MSIF_Version _drv_pcmcia_version = {
    .DDI = { PCMCIA_DRV_VERSION, },
};

static MS_BOOL _gbHighActive;
static MS_BOOL _gbCardInside[E_PCMCIA_MODULE_MAX];
static MS_BOOL _gbPCMCIA_Detect_Enable;
static MS_U32 _gu32PCMCIA_CD_To_HWRST_Timer[E_PCMCIA_MODULE_MAX];
//static MS_U8    _gu8PCMCIACurModule = PCMCIA_DEFAULT_MODULE;
#if PCMCIA_IRQ_ENABLE
static IsrCallback _fnIsrCallback[E_PCMCIA_MODULE_MAX] = {NULL};
static PCMCIA_ISR _gPCMCIA_ISR;
#endif

/*****************************************************************************/
/*                       Local Functions                                     */
/*****************************************************************************/
static MS_BOOL _MDrv_PCMCIA_ReadReg( MS_U32 u32Addr, MS_U8 *pu8Value );
static MS_BOOL _MDrv_PCMCIA_WriteReg( MS_U32 u32Addr, MS_U8 u8Value );

#ifdef CONFIG_PCMCIA_MSPI_BURST
static MS_BOOL _MDrv_PCMCIA_WriteRegMask( MS_U32 u32Addr, MS_U8 u8Value , MS_U8 u8Mask);
static MS_BOOL _MSPI_RWLong(MS_U8 u8Cmd, MS_U16 u16Addr, MS_U8* u8data, MS_U16 u16Len);
static void _MSPI_BurstRst(void);
static MS_BOOL _MSPI_MIU_CheckDone(void);
#endif

static MS_BOOL _MDrv_PCMCIA_ReadReg( MS_U32 u32Addr, MS_U8 *pu8Value )
{
    if ( NULL == pu8Value )
    {
        return FALSE;
    }
    *pu8Value = HAL_PCMCIA_Read_Byte( u32Addr );

#if PCMCIA_DUMP_REG
    ULOGD("PCMCIA",  "R: Addr %02X, Value %02X\n", u8Addr, *pu8Value );
#endif

    return TRUE;
}

static MS_BOOL _MDrv_PCMCIA_WriteReg( MS_U32 u32Addr, MS_U8 u8Value )
{
    HAL_PCMCIA_Write_Byte( u32Addr , u8Value );

#if PCMCIA_DUMP_REG
    ULOGD("PCMCIA",  "W: Addr %02X, Value %02X\n", u8Addr, u8Value );
#endif

    return TRUE;
}
#ifdef CONFIG_PCMCIA_MSPI_BURST
static MS_BOOL _MDrv_PCMCIA_WriteRegMask( MS_U32 u32Addr, MS_U8 u8Value , MS_U8 u8Mask)
{
    HAL_PCMCIA_Write_ByteMask( u32Addr , u8Value, u8Mask);
    return TRUE;    
}
#endif

static void _MDrv_PCMCIA_SwitchModule(PCMCIA_MODULE eModule)
{
    MS_U8 u8Reg = 0x0;
    
    if(pPcmEnv->u8PCMCIACurModule == eModule)
        return;
        
    _MDrv_PCMCIA_ReadReg( REG_PCMCIA_MODULE_VCC_OOB, (MS_U8 *)&u8Reg );   
    u8Reg &= ~(BIT0|BIT1);

    //  MODULE_SEL[1:0]	1:0	Module select.
    //  00: No destination selected.
    //  01: Select module A.
    //  10: Select module B.
    //  11: Reserved.
    
    if(eModule == E_PCMCIA_MODULE_A)
    {
        u8Reg |= BIT0;
    }
    else
    {
        u8Reg |= BIT1;        
    }
    
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_MODULE_VCC_OOB, u8Reg );     
    pPcmEnv->u8PCMCIACurModule = eModule;
}

#ifdef CONFIG_PCMCIA_MSPI_BURST
static MS_BOOL _MSPI_RWLong(MS_U8 u8Cmd, MS_U16 u16Addr, MS_U8* u8data, MS_U16 u16Len)
{
    MS_U16 i = 0;
    MS_U16 Size = 0;
    MS_U16 SizePcmBurst = 0;
    MS_BOOL bRet = TRUE;
    MS_U8 u8reg = 0;
    if(u8Cmd == PCMCIA_ATTRIBMEMORY_WRITE || u8Cmd == PCMCIA_IO_WRITE)
    {
        while(u16Len)
        {
            SizePcmBurst = (u16Len >  MAX_PCMCIA_BURST_WRITE_SIZE) ? MAX_PCMCIA_BURST_WRITE_SIZE : u16Len;
            //printf("PCM Burst %d, left %d\n", (int)SizePcmBurst, (int)u16Len);
            
            u16Len -= SizePcmBurst;
            i=0;
            u8MspiBuf[i++] = MSPI_CMD_MIU_W;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >>  0) & 0xFF;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >>  8) & 0xFF;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >> 16) & 0xFF;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >> 24) & 0xFF;
            u8MspiBuf[i++] = 0x0; // reg_status_expect
            u8MspiBuf[i++] = 0x0; // reg_status_mask
            u8MspiBuf[i++] = 0x0; // reg_pcm_burst_addr_offset
            u8MspiBuf[i++] = 0x0;            
            u8MspiBuf[i++] = u8Cmd;                  // reg_pcm_cmd
            u8MspiBuf[i++] = 0x0;
            u8MspiBuf[i++] = u16Addr & 0xFF;          // reg_adr
            u8MspiBuf[i++] = ( u16Addr >> 8 ) & 0xFF;
            u8MspiBuf[i++] = SizePcmBurst;       // reg_total_burst_num
            u8MspiBuf[i++] = 0;
            u8MspiBuf[i++] = PCMBURST_WRITE & 0xFF;
            u8MspiBuf[i++] = (PCMBURST_WRITE >> 8 ) & 0xFF;
                            
            MDrv_MSPI_RWBytes(MSPI_READ_OPERATION, 0); // clean rbf_size 
            while(SizePcmBurst)
            {
                Size = (SizePcmBurst >  MAX_MSPI_BURST_WRITE_SIZE - i) ? 
                    (MAX_MSPI_BURST_WRITE_SIZE - i) : SizePcmBurst; 


                MDrv_MSPI_SlaveEnable(TRUE);   
                MDrv_MSPI_Write(u8MspiBuf, i);

                //MsOS_DelayTask(100); 
                MDrv_MSPI_Write(u8data, Size);
                MDrv_MSPI_SlaveEnable(FALSE);
                SizePcmBurst -= Size;
                u8data += Size;
                //MsOS_DelayTask(100);                
                i=0;
                u8MspiBuf[i++] = MSPI_CMD_MIU_W;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_WFIFO >>  0) & 0xFF;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_WFIFO >>  8) & 0xFF;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_WFIFO >> 16) & 0xFF;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_WFIFO >> 24) & 0xFF;                
            }
            
            for(i=0;i<MAX_MSPI_STATUS_COUNT;i++)
            {
                _MDrv_PCMCIA_ReadReg(REG_PCM_BURST_STATUS_0, &u8reg);
                if(u8reg & REG_PCM_BURST_WRITE_DONE)
                {
                    _MDrv_PCMCIA_WriteRegMask(REG_PCM_BURST_STATUS_CLR, 
                            REG_PCM_WRITE_FINISH_CLR, REG_PCM_WRITE_FINISH_CLR);
                    break;               
                }
            }
            if(i == MAX_MSPI_STATUS_COUNT)
                printf("[PCM] warning, BURST Write not finish\n");
                
            _MDrv_PCMCIA_ReadReg(REG_PCM_BURST_WFIFO_RMN, &u8reg);
            if(u8reg != 0)
                printf("[PCM] warning, WFIFO not empty %d\n", (int)u8reg);

        }        
    }
    else if(u8Cmd == PCMCIA_ATTRIBMEMORY_READ || u8Cmd == PCMCIA_IO_READ)
    {
        while(u16Len)
        {
            MS_U16 index = 0;
            SizePcmBurst = (u16Len >  MAX_PCMCIA_BURST_READ_SIZE) ? MAX_PCMCIA_BURST_READ_SIZE : u16Len;
            //printf("PCM Burst %d, left %d\n", (int)SizePcmBurst, (int)u16Len);
            
            u16Len -= SizePcmBurst;
            
            i = 0;
            u8MspiBuf[i++] = MSPI_CMD_MIU_W;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >>  0) & 0xFF;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >>  8) & 0xFF;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >> 16) & 0xFF;
            u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_ADDR >> 24) & 0xFF;
            u8MspiBuf[i++] = 0x0; // reg_status_expect
            u8MspiBuf[i++] = 0x0; // reg_status_mask
            u8MspiBuf[i++] = 0x0; // reg_pcm_burst_addr_offset
            u8MspiBuf[i++] = 0x0;             
            u8MspiBuf[i++] = u8Cmd;                  // reg_pcm_cmd
            u8MspiBuf[i++] = 0x0;
            u8MspiBuf[i++] = u16Addr & 0xFF;          // reg_adr
            u8MspiBuf[i++] = ( u16Addr >> 8 ) & 0xFF;
            u8MspiBuf[i++] = SizePcmBurst;       // reg_total_burst_num
            u8MspiBuf[i++] = 0;
            u8MspiBuf[i++] = PCMBURST_READ & 0xFF;
            u8MspiBuf[i++] = (PCMBURST_READ >> 8 ) & 0xFF;
            
            MDrv_MSPI_RWBytes(MSPI_READ_OPERATION, 0); // clean rbf_size 
            MDrv_MSPI_SlaveEnable(TRUE);  
            MDrv_MSPI_Write(u8MspiBuf, i);
            MDrv_MSPI_SlaveEnable(FALSE);
            
            for(i=0;i<MAX_MSPI_STATUS_COUNT;i++)
            {
                _MDrv_PCMCIA_ReadReg(REG_PCM_BURST_STATUS_0, &u8reg);
                if(u8reg & REG_PCM_BURST_READ_DONE)
                {
                    _MDrv_PCMCIA_WriteRegMask(REG_PCM_BURST_STATUS_CLR, 
                            REG_PCM_READ_FINISH_CLR, REG_PCM_READ_FINISH_CLR);
                    break;

                }    
                MsOS_DelayTask(1);
            }
            if(i == MAX_MSPI_STATUS_COUNT)
            {
                printf("[PCM] warning, BURST Write not finish\n");
            }

            while(SizePcmBurst)
            {
                MS_U8 u8TmpBuf[MAX_MSPI_BURST_READ_SIZE + 1];
                Size = ( SizePcmBurst > MAX_MSPI_BURST_READ_SIZE ) ? MAX_MSPI_BURST_READ_SIZE : SizePcmBurst;              
                _MSPI_MIU_CheckDone();

                
                i = 0;
                u8MspiBuf[i++] = MSPI_CMD_MIU_R;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_RFIFO >>  0) & 0xFF;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_RFIFO >>  8) & 0xFF;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_RFIFO >> 16) & 0xFF;
                u8MspiBuf[i++] = (REG_COMPANION_PCMBURST_RFIFO >> 24) & 0xFF;
                u8MspiBuf[i++] = Size + 1; // 1 for MSPI status
                MDrv_MSPI_RWBytes(MSPI_READ_OPERATION, 0); // clean rbf_size 
                MDrv_MSPI_SlaveEnable(TRUE);   
                MDrv_MSPI_Write(u8MspiBuf, i);
                MDrv_MSPI_SlaveEnable(FALSE);
                
                _MSPI_MIU_CheckDone();
                u8MspiBuf[0] = MSPI_CMD_MIU_ST;
                MDrv_MSPI_SlaveEnable(TRUE);   
                MDrv_MSPI_Write(u8MspiBuf, 1);
                MDrv_MSPI_RWBytes(MSPI_WRITE_OPERATION, 0); // clean rbf_size 
                MDrv_MSPI_Read(u8TmpBuf, Size + 1);
                MDrv_MSPI_SlaveEnable(FALSE);
                if(u8TmpBuf[0] != 0x0A)
                {
                    printf("[PCM] warning, MIU_ST status 0x%x\n", (int)u8TmpBuf[0]);
                }
                
                memcpy(u8data, &u8TmpBuf[1], Size);
                SizePcmBurst -= Size;
                u8data += Size;
                _MDrv_PCMCIA_ReadReg(REG_PCM_BURST_RFIFO_RMN, &u8reg);
                if(u8reg != SizePcmBurst)
                    printf("[PCM] warning, RFIFO rmn %d, SizePcmBurst %d\n", (int)u8reg, (int)SizePcmBurst);
                
                MsOS_DelayTask(10);                
            }
              
            _MDrv_PCMCIA_ReadReg(REG_PCM_BURST_RFIFO_RMN, &u8reg);
            if(u8reg != 0)
                printf("[PCM] warning, RFIFO not empty, %d\n", (int)u8reg);
        }        
    }
    return bRet;
}

static MS_BOOL _MSPI_MIU_CheckDone(void)
{
    MS_U8 u8tmp = 0;
    int i = 0;
    u8MspiBuf[0] = MSPI_CMD_MIU_ST;
    
    for(;i<MAX_MSPI_STATUS_COUNT;i++)
    {
        MDrv_MSPI_SlaveEnable(TRUE); 
        MDrv_MSPI_RWBytes(MSPI_READ_OPERATION, 0); // clean rbf_size  
        MDrv_MSPI_Write(u8MspiBuf, 1);
        MDrv_MSPI_RWBytes(MSPI_WRITE_OPERATION, 0); // clean rbf_size 
        MDrv_MSPI_Read(&u8tmp, 1);  
        MDrv_MSPI_SlaveEnable(FALSE);
        if(u8tmp == MSPI_MIU_STATUS_DONE || u8tmp == MSPI_MIU_STATUS_NONE)
            break;
    }
    MDrv_MSPI_RWBytes(MSPI_READ_OPERATION, 0); // clean rbf_size  
    MDrv_MSPI_RWBytes(MSPI_WRITE_OPERATION, 0); // clean rbf_size  
    
    if(i == MAX_MSPI_STATUS_COUNT)
    {
        printf("[PCM] MSPI MIU timeout, status 0x%x\n", (int)u8tmp);
        return FALSE ;
    }
    return TRUE ;
}
 
static void _MSPI_BurstRst(void)
{
    _MDrv_PCMCIA_WriteRegMask(REG_PCM_BURST_CTRL, REG_PCM_BURST_SW_RST_ON, REG_PCM_BURST_SW_RST_MASK);
    _MDrv_PCMCIA_WriteRegMask(REG_PCM_BURST_CTRL, REG_PCM_BURST_SW_RST_OFF, REG_PCM_BURST_SW_RST_MASK);
}
#endif

#if PCMCIA_IRQ_ENABLE
static void _MDrv_PCMCIA_Isr( void )
{
    ISR_STS IsrSts;
    PCMCIA_MODULE eModule = E_PCMCIA_MODULE_A;
    memset(&_gPCMCIA_ISR, 0x00, sizeof(_gPCMCIA_ISR));
    memset(&IsrSts, 0x00, sizeof(ISR_STS));

    /* MASK PCMCIA IRQ */
    HAL_PCMCIA_GetIntStatus(&IsrSts);
    HAL_PCMCIA_MaskInt(0x0, TRUE);
    HAL_PCMCIA_ClrInt(0x0);
    
    if(IsrSts.bCardAInsert || IsrSts.bCardARemove || IsrSts.bCardAData) 
    {
        if (_gbPCMCIA_Irq[E_PCMCIA_MODULE_A])
        {
            _gbPCMCIA_IrqStatus[E_PCMCIA_MODULE_A] = TRUE;
        }   
        if(IsrSts.bCardAInsert)
        {
            _gPCMCIA_ISR.bISRCardInsert=TRUE;
        }     
        if(IsrSts.bCardARemove)
        {
            _gPCMCIA_ISR.bISRCardRemove=TRUE;
        }     
        if(IsrSts.bCardAData)
        {
            _gPCMCIA_ISR.bISRCardData=TRUE;
        }     
        if (NULL != _fnIsrCallback[E_PCMCIA_MODULE_A])
        {
            eModule = E_PCMCIA_MODULE_A;
            _fnIsrCallback[E_PCMCIA_MODULE_A]((void*)(&_gPCMCIA_ISR), (void*)&eModule);
        }        
    }    
    else if(IsrSts.bCardBInsert || IsrSts.bCardBRemove || IsrSts.bCardBData) 
    {
        if (_gbPCMCIA_Irq[E_PCMCIA_MODULE_B])
        {
            _gbPCMCIA_IrqStatus[E_PCMCIA_MODULE_B] = TRUE;
        }   
        if(IsrSts.bCardBInsert)
        {
            _gPCMCIA_ISR.bISRCardInsert=TRUE;
        }     
        if(IsrSts.bCardBRemove)
        {
            _gPCMCIA_ISR.bISRCardRemove=TRUE;
        }     
        if(IsrSts.bCardBData)
        {
            _gPCMCIA_ISR.bISRCardData=TRUE;
        }     
        if (NULL != _fnIsrCallback[E_PCMCIA_MODULE_B])
        {
            eModule = E_PCMCIA_MODULE_B;
            _fnIsrCallback[E_PCMCIA_MODULE_B]((void*)(&_gPCMCIA_ISR), (void*)&eModule);
        }        
    }    
    else
    {
#ifndef CONFIG_PCMCIA_MSPI
        ULOGE("PCMCIA", "[PCMCIA] IRQ but nothing happen\n");
        //MS_ASSERT( 0 );
#endif
    }



    /* Enable HK PCMCIA IRQ */
    MsOS_EnableInterrupt(E_INT_IRQ_PCM);
    MsOS_CompleteInterrupt(E_INT_IRQ_PCM);
    
    /* UNMASK PCMCIA IRQ */
    HAL_PCMCIA_MaskInt(0x0, FALSE);
    
}
#endif

void _MDrv_PCMCIA_Exit( MS_BOOL bSuspend )
{
    if(FALSE == bSuspend)
    {
#if defined ( MSOS_TYPE_LINUX )

    #if PCMCIA_UTOPIA2
    #else    
        MsOS_DeleteMutex(Pcmcia_Mutex); 
        Pcmcia_Mutex = -1;       

        if(Pcmcia_Mutex == -1)
        {
            ULOGE("PCMCIA", "[%s] PCMCIA mutex not exist!\n", __FUNCTION__);        
            return;
        }
        MsOS_ObtainMutex(Pcmcia_Mutex, MSOS_WAIT_FOREVER);
    #endif    
    
        //printf ("PCMCIA close /dev/system >> SYS_fd= %ld\n", SYS_fd);
        if (close (SYS_fd) == -1)
        {
            MS_ASSERT(0);
            perror ("close");
        }
        else
        {
            //printf ("<<< %s >>> LINE %d , _MDrv_PCMCIA_Exit\r\n", __FILE__, __LINE__);
        }
        SYS_fd = -1;

    #if PCMCIA_IRQ_ENABLE
        MsOS_DisableInterrupt( E_INT_IRQ_PCM );                             // Disable PCMCIA interrupt
        MsOS_DetachInterrupt( E_INT_IRQ_PCM );                              // Detach PCMCIA interrupt
    #endif
#endif // MSOS_TYPE_LINUX
    }
    else
    {
        // suspend        
    #if PCMCIA_IRQ_ENABLE
        MsOS_DisableInterrupt( E_INT_IRQ_PCM );                             // Disable PCMCIA interrupt
    #endif        
    }
    HAL_PCMCIA_ClkCtrl(FALSE);
}

/*****************************************************************************/
/*                       Global Functions                                    */
/*****************************************************************************/
void _MDrv_PCMCIA_InitSW( MS_BOOL bHighActiveTrigger )
{
    MS_VIRT u32PCMCIA_RiuBaseAddr = 0;
    MS_PHY u32NonPMBankSize;
    MS_U32 i = 0;
    
#if defined ( MSOS_TYPE_LINUX )  
    if(SYS_fd != -1)
        return;
        
    #define SYS_MODULE_KERNAL_NAME      "/dev/system"

    SYS_fd = open(SYS_MODULE_KERNAL_NAME, O_RDWR);
    if (0 > SYS_fd)   //First time open
    {
        MS_ASSERT(0);
        ULOGE("PCMCIA", "[%s][%d] open fail /dev/system\n", __FUNCTION__, __LINE__);
    }
    else
    {
        printf ("PCMCIA open /dev/system success >> SYS_fd= %d\n", (int)SYS_fd);
    }    
#endif
            
    // get MMIO base
    if ( MDrv_MMIO_GetBASE( &u32PCMCIA_RiuBaseAddr, &u32NonPMBankSize, MS_MODULE_PCMCIA ) != TRUE )
    {
        #if ( defined ( MCU_AEON ) || defined ( MCU_MIPS ))
            u32PCMCIA_RiuBaseAddr = PCMCIA_RIU_DEFAULT_BASE_ADDR;
        #else
            ULOGE("PCMCIA", "PCMCIA error RIU base address assigned \n");
        #endif

        ULOGE("PCMCIA",  "MDrv_PCMCIA_Init GetBASE failure, use default base address: [ %td ]\n", (ptrdiff_t)u32PCMCIA_RiuBaseAddr );
    }
    else
    {
        PCMCIA_DEBUG( ( "PCMCIA Base Addr: [ %td ]\n", u32PCMCIA_RiuBaseAddr ) );
    }

    _gbHighActive = bHighActiveTrigger;

    HAL_PCMCIA_Set_RIU_base( u32PCMCIA_RiuBaseAddr );    
    
#if defined ( MSOS_TYPE_LINUX ) && !(PCMCIA_UTOPIA2)
    if(-1 != Pcmcia_Mutex)
    {
        ULOGE("PCMCIA", "[%s] open PCMCIA mutex more than once\n", __FUNCTION__);
        return;
    }
    
    Pcmcia_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "PCMCIA mutex", MSOS_PROCESS_SHARED);
    if(-1 == Pcmcia_Mutex)
    {
        ULOGE("PCMCIA", "[%s] open PCMCIA mutex fail\n", __FUNCTION__);
        return;
    }
#endif

    for(i = 0; i < E_PCMCIA_MODULE_MAX; i++)
    {
        _gbCardInside[i] = FALSE;
        _gu32PCMCIA_CD_To_HWRST_Timer[i] = 0;
        _gu8PCMCIA_Command[i] = 0;
        _fnIsrCallback[i] = NULL;
        _gbPCMCIA_Irq[i] = DISABLE;
        _gbPCMCIA_IrqStatus[i] = FALSE;
    }
    pPcmEnv->u8PCMCIACurModule = PCMCIA_DEFAULT_MODULE;
    _gbPCMCIA_Detect_Enable = TRUE;    
}

void _MDrv_PCMCIA_InitHW( MS_BOOL bResume )
{

    MS_U8 u8Reg;

    HAL_PCMCIA_ClkCtrl(TRUE);

#ifdef CONFIG_PCMCIA_MSPI_BURST
    _MSPI_BurstRst();
#endif     
    /* Initailze PCMCIA Registers. */
    _MDrv_PCMCIA_ReadReg( REG_PCMCIA_MODULE_VCC_OOB, (MS_U8 *)&u8Reg );

    u8Reg = ( BIT6 | BIT0 ); // reg_module_sel(BIT 1~0): module select
                                    //                          01: select module A
                                    // reg_single_card(BIT4):   Only support single card
                                    //                          1: support 1 card only
                                    // reg_vcc_en(BIT5):        0: VCC Disable
                                    // reg_oob_en(BIT6):        1: OOB enable

    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_MODULE_VCC_OOB, u8Reg );
   

#if PCMCIA_IRQ_ENABLE
    
    if(FALSE == bResume)
    {
        MsOS_DisableInterrupt( E_INT_IRQ_PCM );                             // Disable PCMCIA interrupt
        MsOS_DetachInterrupt( E_INT_IRQ_PCM );                              // Detach PCMCIA interrupt
    
        MsOS_AttachInterrupt( E_INT_IRQ_PCM, (InterruptCb)_MDrv_PCMCIA_Isr );// Attach PCMCIA interrupt
    }
            
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_INT_MASK_CLEAR, 0x7C );           // unmask cardA insert/ remove
    MsOS_EnableInterrupt( E_INT_IRQ_PCM );                              // Enable PCMCIA interrupt
#endif

    return;
}

MS_U32 _MDrv_PCMCIA_SetPowerState(EN_POWER_MODE u16PowerState)
{
       switch (u16PowerState)
       {
           case E_POWER_RESUME:
                _MDrv_PCMCIA_InitHW(TRUE);
           break;
           case E_POWER_SUSPEND:
                _MDrv_PCMCIA_Exit(TRUE);
                break;
           case E_POWER_MECHANICAL:                
           case E_POWER_SOFT_OFF:
           default:
                break;
       }
       return UTOPIA_STATUS_SUCCESS;
}

/*
void MDrv_PCMCIA_Exit( void )
{
    _MDrv_PCMCIA_Exit();
}
*/
MS_BOOL _MDrv_PCMCIA_DetectV2( PCMCIA_MODULE eModule )
{
    MS_U8 u8value = 0;
    
    MS_U8 u8DetectPin = 
        (eModule == E_PCMCIA_MODULE_A) ? PCMCIA_DETECT_PIN_MODULEA : PCMCIA_DETECT_PIN_MODULEB;
 
//    if(eModule >= E_PCMCIA_MODULE_MAX)
//    {
//        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
//        return FALSE;
//    }
        
    if(!_gbPCMCIA_Detect_Enable)
        return FALSE;

    _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, &u8value );
    if((u8value & u8DetectPin) != 0)
        return (TRUE == _gbHighActive);
    else
        return (FALSE == _gbHighActive);
}

MS_BOOL _MDrv_PCMCIA_PollingV2( PCMCIA_MODULE eModule )
{
    MS_BOOL bCardDetect;
    MS_BOOL bModuleStatusChange = FALSE;
    
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }
    
    bCardDetect = MDrv_PCMCIA_DetectV2(eModule);
    
    if(_gbCardInside[eModule] != bCardDetect)
        bModuleStatusChange = TRUE;

    _gbCardInside[eModule] = bCardDetect;

    if(bModuleStatusChange)
    {
        if(_gbCardInside[eModule])
        {
            _gu32PCMCIA_CD_To_HWRST_Timer[eModule] = MsOS_GetSystemTime();
            PCMCIA_DEBUG( ( "Card detected\n" ) );
        }
        else
        {
            PCMCIA_DEBUG( ( "Card removed\n" ) );
        }
    }

    return bModuleStatusChange;
}

void _MDrv_PCMCIA_Set_HW_ResetDuration( MS_U8 u8HW_ResetDuration )
{
    _gu8HW_ResetDuration = u8HW_ResetDuration;
}

void _MDrv_PCMCIA_ResetHW_V2( PCMCIA_MODULE eModule)
{
    MS_U8 u8Reg = 0;
    
    MS_U8 bit = (eModule == E_PCMCIA_MODULE_A) ? BIT2 : BIT3;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    
    PCM_ENTER();
    
#if PCMCIA_IRQ_ENABLE
    MDrv_PCMCIA_Enable_Interrupt( DISABLE );
#endif

    _MDrv_PCMCIA_ReadReg( REG_PCMCIA_MODULE_VCC_OOB, (MS_U8 *)&u8Reg );
    u8Reg |= bit;           //                          1: RESET = HIGH
                            
                            
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_MODULE_VCC_OOB, u8Reg );
    MsOS_DelayTask( _gu8HW_ResetDuration );   // MUST...for HW reset
    u8Reg &= ~bit;          //                          0: RESET = LOW

    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_MODULE_VCC_OOB, u8Reg );

    /* Comment? */  // FIXME_Alec
    _MDrv_PCMCIA_WriteReg( 0x18, 0xBD );
    _MDrv_PCMCIA_WriteReg( 0x19, 0x00 );
    _MDrv_PCMCIA_WriteReg( 0x1A, 0x31 );

    /* Reset PCMCIA IRQ Mask. */
#if PCMCIA_IRQ_ENABLE
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_INT_MASK_CLEAR, 0x7C );  //unmask IRQ cardA insert/ remove
#else
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_INT_MASK_CLEAR, 0x7F );
#endif
    PCM_EXIT();
}

#if defined ( MSOS_TYPE_LINUX ) && !defined (CONFIG_PCMCIA_MSPI)
void _MDrv_PCMCIA_WriteAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value)
{
    PCMCIA_Map_Info_t       stMapInfo;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;    

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    
    stMapInfo.u16Addr = u16Addr ;
    stMapInfo.u8Value = u8Value ;
    stMapInfo.u8Type = 1 ;
    
    PCM_ENTER();
    _MDrv_PCMCIA_SwitchModule(eModule);
    u32Ret = ioctl(SYS_fd, IOCTL_SYS_PCMCIA_WRITE , &stMapInfo);
    PCM_EXIT();

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s][%d] write data fail\n", __FUNCTION__, __LINE__);
    }
}

void _MDrv_PCMCIA_ReadAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 *pDest )
{
    PCMCIA_Map_Info_t       stMapInfo;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;    

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    
    stMapInfo.u16Addr = u16Addr ;
    stMapInfo.u8Type = 1 ;

    PCM_ENTER();
    _MDrv_PCMCIA_SwitchModule(eModule);
        
    u32Ret = ioctl(SYS_fd, IOCTL_SYS_PCMCIA_READ , &stMapInfo);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        *pDest = 0xFF;
    }
    else
    {
        *pDest = stMapInfo.u8Value;
    }
    PCM_EXIT();
}

void _MDrv_PCMCIA_WriteIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value)
{
    PCMCIA_Map_Info_t       stMapInfo;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;    
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    
    stMapInfo.u16Addr = u16Addr ;
    stMapInfo.u8Value = u8Value;
    stMapInfo.u8Type = 2;
    
    PCM_ENTER();
    _MDrv_PCMCIA_SwitchModule(eModule);
    u32Ret = ioctl(SYS_fd, IOCTL_SYS_PCMCIA_WRITE , &stMapInfo);
    PCM_EXIT();

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s][%d] write data fail\n", __FUNCTION__, __LINE__);
    }
}

void _MDrv_PCMCIA_WriteIOMemLongV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer)
{
    PCMCIA_Map_Info_t       stMapInfo;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    stMapInfo.u16Addr = u16Addr ;
    stMapInfo.u8Value = u8Value;
    stMapInfo.u8Type = 3;
    stMapInfo.u16DataLen = u16DataLen;
    stMapInfo.u8pWriteBuffer = u8pWriteBuffer;
    PCM_ENTER();
    _MDrv_PCMCIA_SwitchModule(eModule);    
    u32Ret = ioctl(SYS_fd, IOCTL_SYS_PCMCIA_WRITE , &stMapInfo);
    PCM_EXIT();
    
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("PCMCIA", "[%s][%d] write data fail\n", __FUNCTION__, __LINE__);
    }    
}

//! This function is read one byte of from the card IO memory at address wAddr.
MS_U8 _MDrv_PCMCIA_ReadIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr )
{
    PCMCIA_Map_Info_t       stMapInfo;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return 0;
    }
    stMapInfo.u16Addr = u16Addr ;
    stMapInfo.u8Value = 0;
    stMapInfo.u8Type = 2 ;
    PCM_ENTER();
    _MDrv_PCMCIA_SwitchModule(eModule);    
    u32Ret = ioctl(SYS_fd, IOCTL_SYS_PCMCIA_READ , &stMapInfo);
    PCM_EXIT();

    if((u16Addr == PCMCIA_PHYS_REG_COMMANDSTATUS))
    {
        if(stMapInfo.u8Value & PCMCIA_STATUS_WRITEERROR)
        {
            printf("PCMCIA WRITE ERROR!!!\n");
        }
        else if(stMapInfo.u8Value & PCMCIA_STATUS_READERROR)
        {
            printf("PCMCIA READ ERROR!!!\n");
        }
    } 
    
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        return 0xFF;
    }
    else
    {
        return stMapInfo.u8Value;
    }
}
#else
void _MDrv_PCMCIA_WriteAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value )
{
    MS_U8 u8Reg = 0;
    MS_U16 i;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    
    _MDrv_PCMCIA_SwitchModule(eModule);
    
    // select attribute memory write, low byte
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_PCM_MEM_IO_CMD, PCMCIA_ATTRIBMEMORY_WRITE );

    // write address
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR1, ( u16Addr >> 8 ) );
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR0, u16Addr );

    // write data
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_WRITE_DATA, u8Value );

    // fire command
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, PCMCIA_FIRE_COMMAND );


    //polling if fire is done
    for ( i = 0; i < PCMCIA_MAX_POLLING_COUNT; i++ )
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, (MS_U8 *)&u8Reg );

        if ( !( u8Reg & PCMCIA_FIRE_COMMAND ) )
        {
            break;
        }
    }

    // polling if bus is idle
    for ( i = 0; i < PCMCIA_MAX_POLLING_COUNT; i++ )
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, (MS_U8 *)&u8Reg );

        if ( u8Reg & PCMCIA_STATE_BUS_IDLE )
        {
            break;
        }
    }

    return;
}

void _MDrv_PCMCIA_ReadAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 *pDest )
{
    MS_U16 u16TryLoop = 0;
    MS_U8 u8Reg     = 0;
    
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    // CIS readout with 8Bit I/O accesses
    // requires that we read only every second
    // byte. (The result of reading the even addresses does not seem to work on most modules)

    _MDrv_PCMCIA_SwitchModule(eModule);

    // select attribute memory read, low byte
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_PCM_MEM_IO_CMD, PCMCIA_ATTRIBMEMORY_READ );

    // read address
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR1, (MS_U8)( ( u16Addr * 2 ) >> 8 ) );
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR0, (MS_U8)( u16Addr * 2 ) );

    // fire command
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, PCMCIA_FIRE_COMMAND );

    //polling if fire is done
    while (1)
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, (MS_U8 *)&u8Reg );

        if ( !( u8Reg & PCMCIA_FIRE_COMMAND ) )
        {
            break;
        }
        else
        {
            u16TryLoop++;
            if ( u16TryLoop > PCMCIA_HW_MAX_RETRY_COUNT )
            {
                u16TryLoop = 0;
                ULOGE("PCMCIA",  "[%s:%d][Warning!][PCMCIA] Timeout!\n", __FILE__, __LINE__ );

                return ;
            }
        }
    }

    //polling if data ready
    while (1)
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, (MS_U8 *)&u8Reg );

        if ( u8Reg & PCMCIA_STATE_RD_DONE )
        {
            _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA, pDest );
            break;
        }
        else
        {
            u16TryLoop++;
            if ( u16TryLoop > PCMCIA_HW_MAX_RETRY_COUNT )
            {
                u16TryLoop = 0;
                ULOGE("PCMCIA",  "[%s:%d][Warning!][PCMCIA] Timeout!\n", __FILE__, __LINE__ );

                return ;
            }
        }
    }

    // clean stat_rd done
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, PCMCIA_CLEAN_STATE_RD_DONE );

    // polling if bus is idle
    while (1)
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, (MS_U8 *)&u8Reg );

        if ( PCMCIA_STATE_BUS_IDLE == ( u8Reg & ( PCMCIA_STATE_BUS_IDLE | PCMCIA_STATE_RD_DONE ) ) )
        {
            break;
        }
        else
        {
            u16TryLoop++;
            if ( u16TryLoop > PCMCIA_HW_MAX_RETRY_COUNT )
            {
                u16TryLoop = 0;
                ULOGE("PCMCIA",  "[%s:%d][Warning!][PCMCIA] Timeout!\n", __FILE__, __LINE__ );

                return ;
            }
        }
    }

    //printk("Read Type %bx, Addr %x, value %bx\n", u8AccessType, Addr, u8mem);
}

void _MDrv_PCMCIA_WriteIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value )
{
    MS_U8 u8Reg = 0;
    MS_U16 i;
    
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    
    _MDrv_PCMCIA_SwitchModule(eModule);
    
    // select attribute memory write, low byte
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_PCM_MEM_IO_CMD, PCMCIA_IO_WRITE );

    // write address
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR1, ( u16Addr >> 8 ) );
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR0, u16Addr );
    // write data
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_WRITE_DATA, u8Value );

    // fire command
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, PCMCIA_FIRE_COMMAND );


    //polling if fire is done
    for ( i = 0; i < PCMCIA_MAX_POLLING_COUNT; i++ )
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, (MS_U8 *)&u8Reg );

        if ( !( u8Reg & PCMCIA_FIRE_COMMAND ) )
        {
            break;
        }
        else
        {
            if ( !_MDrv_PCMCIA_IsModuleStillPluggedV2(eModule) )
            {
                return;
            }
        }
    }

    // polling if bus is idle
    for ( i = 0; i < PCMCIA_MAX_POLLING_COUNT; i++ )
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, (MS_U8 *)&u8Reg );

        if ( u8Reg & PCMCIA_STATE_BUS_IDLE )
        {
            break;
        }
        else
        {
            if ( !_MDrv_PCMCIA_IsModuleStillPluggedV2(eModule) )
            {
                return;
            }
        }
    }
}

void _MDrv_PCMCIA_WriteIOMemLongV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer)
{
    
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
    _MDrv_PCMCIA_SwitchModule(eModule);   

#ifdef CONFIG_PCMCIA_MSPI_BURST
    _MSPI_RWLong(PCMCIA_IO_WRITE, u16Addr, u8pWriteBuffer, u16DataLen);    
#else
    MS_U8 u8Reg = 0;
    MS_U16 i,j;
    
    // select attribute memory write, low byte
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_PCM_MEM_IO_CMD, PCMCIA_IO_WRITE );

    // write address
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR1, ( u16Addr >> 8 ) );
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR0, u16Addr );

    for ( i = 0; i < u16DataLen; i++ )
    {
        // write data
        _MDrv_PCMCIA_WriteReg( REG_PCMCIA_WRITE_DATA, u8pWriteBuffer[i] );

        // fire command
        _MDrv_PCMCIA_WriteReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, PCMCIA_FIRE_COMMAND );

        //polling if fire is done
        for ( j = 0; j < PCMCIA_MAX_POLLING_COUNT; ++j )
        {
            _MDrv_PCMCIA_ReadReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, (MS_U8 *)&u8Reg );

            if ( !( u8Reg & PCMCIA_FIRE_COMMAND ) )
            {
                break;
            }
            else
            {
                if ( !_MDrv_PCMCIA_IsModuleStillPluggedV2(eModule) )
                {
                    return;
                }
            }
        }

        // polling if bus is idle
        for ( j = 0; j < PCMCIA_MAX_POLLING_COUNT; ++j )
        {
            _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, (MS_U8 *)&u8Reg );

            if ( u8Reg & PCMCIA_STATE_BUS_IDLE )
            {
                break;
            }
            else
            {
                if ( !MDrv_PCMCIA_IsModuleStillPlugged() )
                {
                    return;
                }
            }
        }
    }
#endif

}

//! This function is read one byte of from the card IO memory at address wAddr.
MS_U8 _MDrv_PCMCIA_ReadIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr )
{
    MS_U8 u8Reg     = 0;
    MS_U8 u8Value   = 0;
    MS_U16 i;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return 0;
    }
    _MDrv_PCMCIA_SwitchModule(eModule); 
        
    // select attribute memory read, low byte
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_PCM_MEM_IO_CMD, PCMCIA_IO_READ );

    // read address
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR1, ( u16Addr >> 8 ) );
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_ADDR0, u16Addr);

    // fire command
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, PCMCIA_FIRE_COMMAND );

    //polling if fire is done
    for ( i = 0; i < PCMCIA_MAX_POLLING_COUNT; i++ )
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, (MS_U8 *)&u8Reg );

        if ( !( u8Reg & PCMCIA_FIRE_COMMAND ) )
        {
            break;
        }
        else
        {
            if ( !MDrv_PCMCIA_IsModuleStillPlugged() )
            {
                return 0x00;
            }
        }
    }

    //polling if data ready
    for ( i = 0; i < PCMCIA_MAX_POLLING_COUNT; i++ )
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, (MS_U8 *)&u8Reg );

        if ( u8Reg & PCMCIA_STATE_RD_DONE )
        {
            //_MDrv_PCMCIA_GetSemaphore();
            {
                _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA, (MS_U8 *)&u8Value );
            }
            //_MDrv_PCMCIA_ReleaseSemaphore();
            break;
        }
        else
        {
            if ( !_MDrv_PCMCIA_IsModuleStillPluggedV2(eModule) )
            {
                return 0x00;
            }
        }
    }

    // clean stat_rd done
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_FIRE_READ_DATA_CLEAR, PCMCIA_CLEAN_STATE_RD_DONE );

    // polling if bus is idle
    for ( i = 0; i < PCMCIA_MAX_POLLING_COUNT; i++ )
    {
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_READ_DATA_DONE_BUS_IDLE, (MS_U8 *)&u8Reg );

        if ( PCMCIA_STATE_BUS_IDLE == ( u8Reg & ( PCMCIA_STATE_BUS_IDLE | PCMCIA_STATE_RD_DONE ) ) )
        {
            break;
        }
        else
        {
            if ( !_MDrv_PCMCIA_IsModuleStillPluggedV2(eModule) )
            {
                return 0x00;
            }
        }
    }

    //printk("Read Addr %x, value %bx\n", u16Addr, u8mem);

    return u8Value;
}
#endif

MS_BOOL _MDrv_PCMCIA_IsModuleStillPluggedV2( PCMCIA_MODULE eModule )
{
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }
    
    return ( _gbCardInside[eModule] );
}

void _MDrv_PCMCIA_Set_Detect_Trigger( MS_BOOL bHighActive )
{
    _gbHighActive = bHighActive;
    return;
}

void _MDrv_PCMCIA_Set_Detect_Enable( MS_BOOL bEnable )
{
    _gbPCMCIA_Detect_Enable = bEnable;
}

MS_U32 _MDrv_PCMCIA_Get_CD_IntervalV2( PCMCIA_MODULE eModule )
{
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return 0;
    }
        
    return _gu32PCMCIA_CD_To_HWRST_Timer[eModule];
}

void _MDrv_PCMCIA_ParseAttribMem( MS_U8 *pAttribMem, MS_U16 dwLen, PCMCIA_INFO *pInfo )
{
    MS_U8 bTag          = 0;
    MS_U8 bTagLen       = 0;
    MS_U16 i            = 0;
    MS_U8 FieldSize     = 0;
    //MS_U8 LastIndex     = 0;
    //MS_U8 MaskByte      = 0;
    //MS_U8 SubTagByte    = 0;
    MS_U8 SubTagLen     = 0;
    //MS_U16 STCI_IFN     = 0;
    MS_U8 fInterface    = 0;
    char CiDetStr[20];

    if ( ( NULL == pAttribMem ) || ( NULL == pInfo ) )
    {
        return;
    }

    memset( pInfo, 0x00, sizeof( PCMCIA_INFO ) );
    do
    {
        bTag = pAttribMem[0];
        bTagLen = pAttribMem[1];
        if ( (MS_U16)bTagLen + 2 > dwLen )
        {
            return ;
        }

        dwLen -= ( bTagLen + 2 );
        PCMCIA_DEBUG( ( "Parse_PCMCIA_AttribMem:bTag[0x%02X] dwLen[%d bytes]\n", bTag, dwLen ) );
        switch ( bTag )
        {
            case CISTPL_NULL:
                PCMCIA_DEBUG( ( "Parse_PCMCIA_AttribMem case 0x00\n" ) );
                break;

            case CISTPL_DEVICE:
                PCMCIA_DEBUG( ( "CISTPL_DEVICE\n" ) );
                // Memory type
                // Access speed
                // Memory Size
                break;
            case CISTPL_LINKTARGET:
                PCMCIA_DEBUG( ( "CISTPL_LINKTARGET\n" ) );
                // "CIS"
                break;
            case CISTPL_NO_LINK:
                //PCMCIA_DEBUG(("CISTPL_NOLINK\n"));
                break;
            case CISTPL_VERS_1:
                pInfo->wPCMCIAStdRev = (MS_U16)pAttribMem[2] << 8 | (MS_U16)pAttribMem[3];
                pInfo->dwValidFlags |= PCMCIAINFO_VERS1_VALID;
                i = 4;
                if ( bTagLen < 2 )
                {
                    break;
                } // error
                memcpy( pInfo->pszManufacturerName, pAttribMem + i,
                        pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 ) );
                pInfo->pszManufacturerName[pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 )] = '\0';
                i += strlen( ( char* )pAttribMem + i ) + 1;
                if ( i < bTagLen + 2 )
                {
                    memcpy( pInfo->pszProductName, pAttribMem + i,
                            pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 ) );
                    pInfo->pszProductName[pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 )] = '\0';
                    i += strlen( ( char* )pAttribMem + i ) + 1;
                }

                if ( 0xFF != *( pAttribMem + i ) )
                {
                    if ( i < bTagLen + 2 )
                    {
                        memcpy( pInfo->pszProductInfo1, pAttribMem + i,
                                pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 ) );
                        pInfo->pszProductInfo1[pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 )] = '\0';
                        i += strlen( ( char* )pAttribMem + i ) + 1;

                        /* $compatible[ciplus=1]$  */
                        {
                            MS_U8 u8aCI_PLUS_STRING1[] = "$compatible[ciplus=";
                            MS_U8 u8aCI_PLUS_STRING2[] = "$COMPATIBLE[CIPLUS=";
                            if ( ( 0 == memcmp( pInfo->pszProductInfo1, u8aCI_PLUS_STRING1, sizeof( u8aCI_PLUS_STRING1 ) - 1 ) ) ||
                                 ( 0 == memcmp( pInfo->pszProductInfo1, u8aCI_PLUS_STRING2, sizeof( u8aCI_PLUS_STRING2 ) - 1 ) ) )
                            {
                                PCMCIA_DEBUG( ( "CI+ CAM\n" ) );
                                pInfo->bCI_PLUS = 1;
                            }
                        }
                    }

                    if ( i < bTagLen + 2 )
                    {
                        memcpy( pInfo->pszProductInfo2, pAttribMem + i,
                                pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 ) );
                        pInfo->pszProductInfo2[pcmcia_min( bTagLen + 2 - i, MAX_PCMCIA_STRLEN - 1 )] = '\0';
                    }
                }
                #if 0//PCMCIA_DEBUG_ENABLE
                    ULOGD("PCMCIA",  "ManufacturerName %s \n", pInfo->pszManufacturerName );
                    ULOGD("PCMCIA",  "ProductName %s \n", pInfo->pszProductName );
                    ULOGD("PCMCIA",  "ProductInfo1 %s \n", pInfo->pszProductInfo1 );
                    ULOGD("PCMCIA",  "ProductInfo2 %s \n", pInfo->pszProductInfo2 );
                #endif
                break;
            case CISTPL_DEVICE_A:
                //PCMCIA_DEBUG( ( "CISTPL_DEVICE_A\n" ) );
                // Like 0x01... for device(s) in attribute memory
                // Memory type
                // Access speed
                // Memory Size
                break;
            case CISTPL_CONFIG:
                PCMCIA_DEBUG( ( "CISTPL_CONFIG\n" ) );
                /*
                {
                    MS_U8 bNumAddrBytes;
                    MS_U8 bNumConfigRegs;
                    MS_U8 bLastIndex;
                    bNumAddrBytes = (pAttribMem[2]&0x03)+1;
                    bNumConfigRegs = ((pAttribMem[2]>>2)&0x0F)+1;
                    bLastIndex = pAttribMem[3]&0x3F;
                    for ( i = 0; i < bNumAddrBytes; i++ )
                    {
                        pInfo->ConfigOffset = pInfo->ConfigOffset<<8;
                        pInfo->ConfigOffset |= pAttribMem[4+i];
                        PCMCIA_DEBUG( ( "ConfigOffset %08lX\n", pInfo->ConfigOffset ) );
                    }
                    i++;
                }
                */
                FieldSize = pAttribMem[2];
                //LastIndex = pAttribMem[3];
                if ( FieldSize == 0 )
                {
                    pInfo->ConfigOffset = pAttribMem[4];
                }
                else if ( FieldSize == 1 )
                {
                    pInfo->ConfigOffset = (MS_U32)pAttribMem[5] << 8 | (MS_U32)pAttribMem[4];
                }
                PCMCIA_DEBUG( ( "ConfigOffset 0x%lx\n", pInfo->ConfigOffset ) );
                //MaskByte = pAttribMem[5 + FieldSize];
                //SubTagByte = pAttribMem[6 + FieldSize];
                SubTagLen = pAttribMem[7 + FieldSize];
                //STCI_IFN = (MS_U16)pAttribMem[9 + FieldSize] << 8 | (MS_U16)pAttribMem[8 + FieldSize];
                memcpy( CiDetStr, pAttribMem + 10 + FieldSize, pcmcia_min( SubTagLen - 2, sizeof( CiDetStr ) - 1 ) );
                CiDetStr[pcmcia_min( SubTagLen - 2, sizeof( CiDetStr ) - 1 )] = 0;
                //PCMCIA_DEBUG( ( "FieldSize %d, LastIndex %d MaskByte %02X SubTag %02X ID %02X %s\n", FieldSize, LastIndex, MaskByte, SubTagByte, STCI_IFN, CiDetStr ) );
                break;
            case CISTPL_CFTABLE_ENTRY:
                PCMCIA_DEBUG( ( "CISTPL_CFTABLE_ENTRY\n" ) );
                {
                    //MS_U8 bIF = 0;
                    MS_U8 bFlags = 0;
                    MS_U16 j = 0;

                    pInfo->bINT = FALSE; // It's MUST because there are multiple CISTPL_CFTABLE_ENTRYs.

                    pInfo->Config[pInfo->bNumConfigs].bConfigIndex = ( pAttribMem[2] & 0x3F );
                    PCMCIA_DEBUG(("Config Entry value 0x02%X\n",pInfo->Config[pInfo->bNumConfigs].bConfigIndex));
                    if ( pAttribMem[2] & 0x40 )
                    {
                        ;
                    } // Default
                    if ( pAttribMem[2] & 0x80 )
                    {
                        //bIF = pAttribMem[3];
                        i = 4;
                    }
                    else
                    {
                        i = 3;
                    }
                    bFlags = pAttribMem[i]; // TPCE_FS: Feature Selection Byte
                    i++;

                    if ( bFlags & (BIT0 | BIT1) ) // BIT0 | BIT1 = TPCE_FS: Power
                    {/* TPCE_PD: Power Description Structure */
                        MS_U8 bPowerBits = pAttribMem[i];   // Parameter Selection Byte

                        //PCMCIA_DEBUG( ( "PowerDesc %02X\n", bPowerBits ) );
                        i++;
                        for ( j = 0; j < 7; j++ )
                        {
                            if ( ( bPowerBits >> j ) & 0x01 )
                            {
                                i++;
                            }
                            while ( pAttribMem[i] & 0x80 )
                            {
                                i++;
                            } // extension byte
                        }
                    }

                    if ( bFlags & BIT2 ) // BIT2 = Timing
                    {/* TPCE_TD: Configuration Timing Information */
                        //PCMCIA_DEBUG( ( "TimingDesc %02X\n", pAttribMem[i] ) );
                        i++;
                    }

                    if ( bFlags & BIT3 ) // BIT3 = IO Space
                    {/* TPCE_IO: I/O Space Addresses Required For This Configuration */
                        if ( pAttribMem[i] & 0x80 ) // BIT7 = Range
                        {
                            /* Range = 1: The I/O Space definition byte is followed by an I/O Range Descriptor byte, and
                                          one or more I/O Address Range Description fields.
                            */
                            MS_U8 bAddrBytes;
                            MS_U8 bLenBytes;
                            //MS_U8 bNumDesc;
                            MS_U32 dwEALen = 0;
                            MS_U32 dwEAAddr = 0;
                            //bNumDesc = pAttribMem[i + 1] & 0x0F;            // I/O Range Descriptor Byte: Number of I/O Address Ranges (Bit 0~3))
                            bAddrBytes = ( pAttribMem[i + 1] >> 4 ) & 0x03; // I/O Range Descriptor Byte: Size of Address (Bit 4~5)
                            bLenBytes = ( pAttribMem[i + 1] >> 6 ) & 0x03;  // I/O Range Descriptor Byte: Size of Length (Bit 6~7)
                            //PCMCIA_DEBUG( ( "EADesc %02X %d %d %d\n", pAttribMem[i + 1], bNumDesc, bAddrBytes, bLenBytes ) );
                            i += 2;
                            switch ( bAddrBytes )
                            {
                                case 1:
                                    dwEAAddr = pAttribMem[i];
                                    break;
                                case 2:
                                    dwEAAddr = (MS_U32)pAttribMem[i] | (MS_U32)pAttribMem[i + 1] << 8;
                                    break;
                            }
                            pInfo->Config[pInfo->bNumConfigs].dwEAAddr = dwEAAddr;
                            i += bLenBytes;
                            switch ( bLenBytes )
                            {
                                case 1:
                                    dwEALen = pAttribMem[i];
                                    break;
                                case 2:
                                    dwEALen = (MS_U32)pAttribMem[i] | (MS_U32)pAttribMem[i + 1] << 8;
                                    break;
                            }
                            pInfo->Config[pInfo->bNumConfigs].dwEALen = dwEALen;
                            i += bAddrBytes;
                            //PCMCIA_DEBUG( ( "Addr %04X Len %04X", dwEAAddr, dwEALen ) );
                        }
                        else
                        {
                            i++;
                        }
                    }

                    if ( bFlags & BIT4 ) // BIT4 = IRQ
                    {/* TPCE_IR: Interrupt Request Description Structure */
                        PCMCIA_DEBUG( ( "IrqDesc: 0x%02X\n", pAttribMem[i] ) );
                        pInfo->Config[pInfo->bNumConfigs].bIRQDesc1 = pAttribMem[i];
                        if ( ( pAttribMem[i] & BIT5) && ( !(pAttribMem[i] & (~BIT5) ) ) )
                        {
                            pInfo->bINT = ENABLE;
                        }
                        else if ( pAttribMem[i] & BIT4 )
                        {
                            pInfo->Config[pInfo->bNumConfigs].wIRQData = (MS_U16)pAttribMem[i + 1] << 8 |
                                                                         (MS_U16)pAttribMem[i + 2];
                            i += 2;
                        }
                        i++;
                    }
                    if ( bFlags & 0x60 )
                    {
                        PCMCIA_DEBUG( ( "MemoryDesc\n" ) );
                        i++;
                    }
                    if ( bFlags & 0x80 )
                    {
                        PCMCIA_DEBUG( ( "MixedDesc\n" ) );
                        i++;
                    }

                    while ( i < ( bTagLen + 2 ) )
                    {
                        PCMCIA_DEBUG( ( "SubTag 0x%02X %d %d\n", pAttribMem[i], i, bTagLen ) );
                        if ( pAttribMem[i] == 0xc0 )
                        {
                            if ( strcmp( ( char* )pAttribMem + i + 2, "DVB_HOST" ) == 0 )
                            {
                                pInfo->Config[pInfo->bNumConfigs].fCITagsPresent |= 0x01;
                            }
                            PCMCIA_DEBUG( ( "%s\n", pAttribMem + i + 2 ) );
                        }
                        if ( pAttribMem[i] == 0xc1 )
                        {
                            if ( strcmp( ( char* )pAttribMem + i + 2, "DVB_CI_MODULE" ) == 0 )
                            {
                                pInfo->Config[pInfo->bNumConfigs].fCITagsPresent |= 0x02;
                            }
                            PCMCIA_DEBUG( ( "%s\n", pAttribMem + i + 2 ) );
                        }
                        i += pAttribMem[i + 1] + 2;
                    }

                    pInfo->bNumConfigs++;
                }
                fInterface = ( pAttribMem[2] >> 6 ) & 0x01;
                if ( fInterface )
                {
                    //PCMCIA_DEBUG( ( "IF %02X ", pAttribMem[3] ) );
                }

                #if 0
                PCMCIA_DEBUG( ( "\n" ) );

                for ( i = 0; i < bTagLen; i++ )
                {
                    PCMCIA_DEBUG( ( "%02X ", pAttribMem[2 + i] ) );
                }
                PCMCIA_DEBUG( ( "\n" ) );
                for ( i = 0; i < bTagLen; i++ )
                {
                    PCMCIA_DEBUG( ( "%c  ", pAttribMem[2 + i] ) );
                }
                PCMCIA_DEBUG( ( "\n" ) );
                #endif
                break;
            case CISTPL_DEVICE_OC:
                PCMCIA_DEBUG( ( "CISTPL_DEVICE_OC\n" ) );
                break;
            case CISTPL_DEVICE_OA:
                PCMCIA_DEBUG( ( "CISTPL_DEVICE_OA\n" ) );
                break;
            case CISTPL_MANFID:
                PCMCIA_DEBUG( ( "CISTPL_MANFID\n" ) );
                pInfo->dwValidFlags |= PCMCIAINFO_MANID_VALID;
                pInfo->wManufacturerId = (MS_U16)pAttribMem[2] << 8 | (MS_U16)pAttribMem[3];
                pInfo->wCardID = (MS_U16)pAttribMem[4] << 8 | (MS_U16)pAttribMem[5];
                PCMCIA_DEBUG( ( "Manufacturer code %04X Product Code %04X\n",
                                pInfo->wManufacturerId, pInfo->wCardID ) );
                #if 0//PCMCIA_DUMP_CISMANID
                ULOGD("PCMCIA",  "wManufacturerId %x \n", pInfo->wManufacturerId );
                ULOGD("PCMCIA",  "CardID %x \n", pInfo->wCardID );
                #endif
                break;
            case CISTPL_FUNCID:
                PCMCIA_DEBUG( ( "CISTPL_FUNCID\n" ) );
                pInfo->dwValidFlags |= PCMCIAINFO_FUNCID_VALID;
                pInfo->FuncType = ( PCMCIA_FUNCTYPE )pAttribMem[2];
                pInfo->bFuncIDSysInfo = pAttribMem[3];
                break;
            case CISTPL_VERS_2:
                // CISTPL_VERS2
                PCMCIA_DEBUG( ( "Parse_PCMCIA_AttribMem case 0x40\n" ) );
                break;
            default:
                #if 0
                PCMCIA_DEBUG( ( "Parse_PCMCIA_AttribMem case else\n" ) );
                PCMCIA_DEBUG( ( "Tag %02X, Len %d\n", bTag, bTagLen ) );
                for ( i = 0; i < bTagLen; i++ )
                {
                    PCMCIA_DEBUG( ( "%02X ", pAttribMem[2 + i] ) );
                }
                PCMCIA_DEBUG( ( "\n" ) );
                for ( i = 0; i < bTagLen; i++ )
                {
                    PCMCIA_DEBUG( ( "%c", pAttribMem[2 + i] ) );
                }
                PCMCIA_DEBUG( ( "\n" ) );
                #endif
                break;
        }
        pAttribMem += ( 2 + (MS_U16)bTagLen );
    }
    while ( ( bTag != 0x14 ) && ( bTag != 0xFF ) && ( dwLen ) );

    return;
}

void _MDrv_PCMCIA_SetCommandBitV2( PCMCIA_MODULE eModule, MS_U8 u8CommandBit, MS_BOOL bValue )
{
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }
        
    if ( bValue )
        _gu8PCMCIA_Command[eModule] |= u8CommandBit;
    else
        _gu8PCMCIA_Command[eModule] &= ( ~u8CommandBit );

    _MDrv_PCMCIA_WriteIOMemV2( eModule, PCMCIA_PHYS_REG_COMMANDSTATUS, _gu8PCMCIA_Command[eModule] );
}

MS_BOOL _MDrv_PCMCIA_ResetInterfaceV2( PCMCIA_MODULE eModule )
{
    MS_BOOL bRet = TRUE;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }

    _gu8PCMCIA_Command[eModule] = 0x00;  // Reset CI Command

    MsOS_DelayTask( 300 );  // The waiting time in for loop of _MDrv_PCMCIA_WaitForStatusBit() is the key factor.

    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_RESET, TRUE );    // Up RS

    bRet = _MDrv_PCMCIA_WaitForStatusBitV2( eModule, PCMCIA_STATUS_FREE );

    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_RESET, FALSE );   // Down RS
    
    // For some specific CAMs, it sets the Free bit after RS bit is clear.
    if(bRet == FALSE)
    {
        bRet = MDrv_PCMCIA_WaitForStatusBitV2( eModule, PCMCIA_STATUS_FREE );
    }
    return bRet;
}

MS_BOOL _MDrv_PCMCIA_IsDataAvailableV2( PCMCIA_MODULE eModule )
{
    MS_U8 u8StatusBit = 0;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }
    u8StatusBit = _MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_COMMANDSTATUS );

    return ( u8StatusBit & PCMCIA_STATUS_DATAAVAILABLE );
}

MS_U16 _MDrv_PCMCIA_ReadDataV2( PCMCIA_MODULE eModule, MS_U8* u8pReadBuffer, MS_U16 u16ReadBufferSize )
{
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }    
    
#if defined ( MSOS_TYPE_LINUX ) && !defined (CONFIG_PCMCIA_MSPI)
    PCMCIA_Map_Info_t       stMapInfo;
    stMapInfo.u16DataLen = u16ReadBufferSize;
    stMapInfo.u8pReadBuffer = u8pReadBuffer;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;

    PCM_ENTER();
    _MDrv_PCMCIA_SwitchModule(eModule);

    u32Ret = ioctl(SYS_fd, IOCTL_SYS_PCMCIA_READ_DATA , &stMapInfo);
    
    PCM_EXIT();
    
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        return 0;
    }
    else
    {
        return stMapInfo.u16DataLen;
    }
#else
    MS_U16 u16DataLen = 0;

    u16DataLen = (MS_U16)_MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_SIZEHIGH ) << 8 |
                 (MS_U16)_MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_SIZELOW );

    if ( ( 0 != u16ReadBufferSize ) & ( u16DataLen > u16ReadBufferSize ) )
        u16DataLen = u16ReadBufferSize;

#ifdef CONFIG_PCMCIA_MSPI_BURST
    _MSPI_RWLong(PCMCIA_IO_READ, PCMCIA_PHYS_REG_DATA, u8pReadBuffer, u16DataLen);
#else
    MS_U16 i = 0;
    for ( i = 0; i < u16DataLen; i++ )
    {
        u8pReadBuffer[i] = _MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_DATA );
    }
#endif
    
    return u16DataLen;
#endif
}

MS_BOOL _MDrv_PCMCIA_WriteDataV2( PCMCIA_MODULE eModule, MS_U8* u8pWriteBuffer, MS_U16 u16DataLen )
{
    MS_U16  u16TryLoop = 0;
    MS_U16 i = 0;
    
    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }    

    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, TRUE );      // Up HC
    while ( ( !( _MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_COMMANDSTATUS ) & PCMCIA_STATUS_FREE ) ) &&
            ( u16TryLoop < PCMCIA_HW_MAX_RETRY_COUNT ) )
    {
        _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, FALSE ); // Down HC
        MsOS_DelayTask( 1 );
        _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, TRUE );  // Up HC
        u16TryLoop++;
    }

    if ( PCMCIA_HW_MAX_RETRY_COUNT == u16TryLoop )
    {
        ULOGE("PCMCIA",  "MDrv_PCMCIA_WriteData: not \"free\", retry %d times fail!\r\n", u16TryLoop);
        return FALSE;
    }

    _MDrv_PCMCIA_WriteIOMemV2( eModule, PCMCIA_PHYS_REG_SIZELOW, (MS_U8)( u16DataLen & 0xFF ) );
    _MDrv_PCMCIA_WriteIOMemV2( eModule, PCMCIA_PHYS_REG_SIZEHIGH, (MS_U8)( u16DataLen >> 8 ) );
    _MDrv_PCMCIA_WriteIOMemLongV2( eModule, PCMCIA_PHYS_REG_DATA, u8pWriteBuffer[i], u16DataLen, u8pWriteBuffer );

    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, FALSE );     // Down HC
    if(_MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_COMMANDSTATUS ) & PCMCIA_STATUS_WRITEERROR)
    {
        printf("%s WE = 1\n", __FUNCTION__);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_BOOL _MDrv_PCMCIA_SwitchToIOmodeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo )
{
    MS_U8 i = 0;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }    
    
    MsOS_DelayTask( 1000 );

    for ( i = 0; i < pInfo->bNumConfigs; i++ )
    {
        if ( pInfo->Config[i].fCITagsPresent == 0x03 )
        {
            _MDrv_PCMCIA_WriteAttribMemV2( eModule, (MS_U16)( pInfo->ConfigOffset ), pInfo->Config[i].bConfigIndex );    //switch to i/o mode

            return TRUE;
        }
    }

    return FALSE;
}

MS_U16 _MDrv_PCMCIA_NegotiateBufferSizeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo )
{
    MS_U16 u16BufferSize;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return 0;
    }    
    
    if ( NULL == pInfo )
    {
        PCMCIA_DEBUG( ( "ERROR: MDrv_PCMCIA_NegotiateBufferSize: NULL == pInfo!\n" ) );
        return 0;
    }

    // write size read
    PCMCIA_DEBUG( ( "Write Size Read\n" ) );
    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_SIZEREAD, TRUE );         // Up SR
    if ( !_MDrv_PCMCIA_WaitForStatusBitV2( eModule, PCMCIA_STATUS_DATAAVAILABLE ) )
    {
        PCMCIA_DEBUG( ( "ERROR: MDrv_PCMCIA_NegotiateBufferSize NG!\n" ) );
        _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_SIZEREAD, FALSE );    // Down SR
        return 0;
    }

    // Dump Buffersize
    // read the size (this is always 0002, byteswapped)
    u16BufferSize = (MS_U16)_MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_SIZEHIGH ) << 8 |
                    (MS_U16)_MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_SIZELOW );
    PCMCIA_DEBUG( ( "Datalen %04X\n", u16BufferSize ) );
    // if the module returned an invalid data size, initiate a reset
    if ( u16BufferSize != 0x0002 )
    {
        PCMCIA_DEBUG( ( "ERROR: MDrv_PCMCIA_NegotiateBufferSize: Invalid BufferSize!\n" ) );
        _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_SIZEREAD, FALSE );    // Down SR
        return 0;
    }
    u16BufferSize = (MS_U16)_MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_DATA ) << 8;
    u16BufferSize |= (MS_U16)_MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_DATA );
    PCMCIA_DEBUG( ( "BufferSize %02X\n", u16BufferSize ) );

    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_SIZEREAD, FALSE );        // Down SR

    return u16BufferSize;
}

void _MDrv_PCMCIA_WriteBufferSizeV2( PCMCIA_MODULE eModule, MS_U16 u16BufferSize )
{
    MS_U16 u16TryLoop = 0;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }    
    
    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_SIZEWRITE, TRUE );        // Up SW
    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, TRUE );      // Up HC

    while ( ( !( _MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_COMMANDSTATUS ) & PCMCIA_STATUS_FREE ) ) &&
            ( u16TryLoop < PCMCIA_HW_MAX_RETRY_COUNT ) )
    {
        _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, FALSE ); // Down HC
        MsOS_DelayTask( 1 );
        _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, TRUE );  // Up HC
        u16TryLoop++;
    }

    if ( PCMCIA_HW_MAX_RETRY_COUNT == u16TryLoop )
    {
        ULOGE("PCMCIA",  "ERROR: MDrv_PCMCIA_WriteBufferSize NG!\n" );
    }

    _MDrv_PCMCIA_WriteIOMemV2( eModule, PCMCIA_PHYS_REG_SIZELOW, 0x02 );
    _MDrv_PCMCIA_WriteIOMemV2( eModule, PCMCIA_PHYS_REG_SIZEHIGH, 0x00 );
    _MDrv_PCMCIA_WriteIOMemV2( eModule, PCMCIA_PHYS_REG_DATA, (MS_U8)( u16BufferSize >> 8 ) );
    _MDrv_PCMCIA_WriteIOMemV2( eModule, PCMCIA_PHYS_REG_DATA, (MS_U8)( u16BufferSize ) );

    MsOS_DelayTask( 50 );

    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_HOSTCONTROL, FALSE );     // Down HC
    _MDrv_PCMCIA_SetCommandBitV2( eModule, PCMCIA_COMMAND_SIZEWRITE, FALSE );       // Down SW

    PCMCIA_DEBUG( ( "Write Buffersize: 0x%04X bytes\n", u16BufferSize ) );
    MsOS_DelayTask( 100 );//Black Viacess
}

MS_U8 MDrv_PCMCIA_ReadyStatus( void )
{
    MS_U8 u8Reg = 0,  u8Reg2 = 0, temp = 0;

    _MDrv_PCMCIA_ReadReg( REG_PCMCIA_INT_MASK_CLEAR, (MS_U8 *) &temp );
    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_INT_MASK_CLEAR, 0x7B );

    _MDrv_PCMCIA_ReadReg( REG_PCMCIA_STAT_INT_RAW_INT, (MS_U8 *) &u8Reg );
        ULOGD("PCMCIA", "REG_PCMCIA_STAT_INT_RAW_INT %x\n",u8Reg );

    _MDrv_PCMCIA_ReadReg( REG_PCMCIA_STAT_INT_RAW_INT1, (MS_U8 *)&u8Reg2 );
        ULOGD("PCMCIA", "REG_PCMCIA_STAT_INT_RAW_INT1 %x\n",u8Reg2 );

    _MDrv_PCMCIA_WriteReg( REG_PCMCIA_INT_MASK_CLEAR, temp );

    return ( u8Reg );

}
//------------------------------------------------------------------------------
/// Get PCMCIA driver version
/// @return -the pointer to the driver version
//------------------------------------------------------------------------------
MS_BOOL _MDrv_PCMCIA_GetLibVer( const MSIF_Version **ppVersion )
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_pcmcia_version;
    return TRUE;
}

#if PCMCIA_IRQ_ENABLE
void _MDrv_PCMCIA_Enable_InterruptV2( PCMCIA_MODULE eModule, MS_BOOL bEnable )
{
    MS_U8 u8Reg;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return;
    }  
        
    _MDrv_PCMCIA_Set_InterruptStatusV2( eModule, FALSE );

    if ( ENABLE == bEnable )
    {
        _gbPCMCIA_Irq[eModule] = ENABLE;

        /* Enable MPU PCMCIA IRQ. */
        MsOS_EnableInterrupt( E_INT_IRQ_PCM );

        /* Enable IP PCMCIA IRQ. */
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_INT_MASK_CLEAR, (MS_U8 *)&u8Reg );
        if(eModule == E_PCMCIA_MODULE_A)
        {
            u8Reg &= ( ~BIT2 );
            u8Reg &= ( ~BIT1 );
            u8Reg &= ( ~BIT0 );
        }
        else 
        { // Module B
            u8Reg &= ( ~BIT5 );
            u8Reg &= ( ~BIT4 );
            u8Reg &= ( ~BIT3 );
        }
        _MDrv_PCMCIA_WriteReg( REG_PCMCIA_INT_MASK_CLEAR, u8Reg );
    }
    else
    {
        _gbPCMCIA_Irq[eModule] = DISABLE;

        /* Here DON"T Disable MPU PCMCIA IRQ. */
        /* Disable IP PCMCIA IRQ. */
        _MDrv_PCMCIA_ReadReg( REG_PCMCIA_INT_MASK_CLEAR, (MS_U8 *)&u8Reg );
        if(eModule == E_PCMCIA_MODULE_A)
        {
            u8Reg |= BIT2; //Don't mask cardA insert/remove
        }
        else
        { // Module B
            u8Reg |= BIT5;
        }
        _MDrv_PCMCIA_WriteReg( REG_PCMCIA_INT_MASK_CLEAR, u8Reg );
    }
}

void _MDrv_PCMCIA_Set_InterruptStatusV2( PCMCIA_MODULE eModule, MS_BOOL Status )
{
    _gbPCMCIA_IrqStatus[eModule] = Status;
}

MS_BOOL _MDrv_PCMCIA_Get_InterruptStatusV2( PCMCIA_MODULE eModule )
{
    return _gbPCMCIA_IrqStatus[eModule];
}

void _MDrv_PCMCIA_InstarllIsrCallbackV2( PCMCIA_MODULE eModule, IsrCallback fnIsrCallback )
{
    _fnIsrCallback[eModule] = fnIsrCallback;
}
#endif

MS_BOOL _MDrv_PCMCIA_WaitForStatusBitV2( PCMCIA_MODULE eModule, MS_U8 u8StatusBit )
{
    MS_U16 i = 0;

    if(eModule >= E_PCMCIA_MODULE_MAX)
    {
        ULOGE("PCMCIA", "ERROR: Module 0x%x not support\n", (int)eModule);
        return FALSE;
    }    
    
    for ( i = 0; i < 500; i++ )
    {
        if ( MDrv_PCMCIA_ReadIOMemV2( eModule, PCMCIA_PHYS_REG_COMMANDSTATUS ) & u8StatusBit )
        {
            return TRUE;
        }
        MsOS_DelayTask( 15 );
        if(!MDrv_PCMCIA_DetectV2(eModule))
        {
            PCMCIA_DEBUG( ( "ERROR: Card Removed\n" ) );
            return FALSE;
        }
    }

    PCMCIA_DEBUG( ( "ERROR: Waiting Timeout for PCMCIA Status Bit\n" ) );

    return FALSE;
}

// backward compatible

#if PCMCIA_IRQ_ENABLE

void MDrv_PCMCIA_Enable_Interrupt( MS_BOOL bEnable )
{
    MDrv_PCMCIA_Enable_InterruptV2(PCMCIA_DEFAULT_MODULE, bEnable);
}

void MDrv_PCMCIA_Set_InterruptStatus( MS_BOOL Status )
{
    MDrv_PCMCIA_Set_InterruptStatusV2(PCMCIA_DEFAULT_MODULE, Status);
}

MS_BOOL MDrv_PCMCIA_Get_InterruptStatus( void )
{
    return MDrv_PCMCIA_Get_InterruptStatusV2(PCMCIA_DEFAULT_MODULE);
}

void MDrv_PCMCIA_InstarllIsrCallback( IsrCallback fnIsrCallback )
{
    MDrv_PCMCIA_InstarllIsrCallbackV2(PCMCIA_DEFAULT_MODULE, fnIsrCallback);
}

#endif // PCMCIA_IRQ_ENABLE

MS_BOOL MDrv_PCMCIA_Polling( void )
{
    return MDrv_PCMCIA_PollingV2(PCMCIA_DEFAULT_MODULE);
}

MS_BOOL MDrv_PCMCIA_IsModuleStillPlugged( void )
{
    return MDrv_PCMCIA_IsModuleStillPluggedV2(PCMCIA_DEFAULT_MODULE);
}

void MDrv_PCMCIA_SetCommandBit( MS_U8 u8CommandBit, MS_BOOL bValue )
{
    MDrv_PCMCIA_SetCommandBitV2(PCMCIA_DEFAULT_MODULE, u8CommandBit, bValue);
}

MS_BOOL MDrv_PCMCIA_ResetInterface( void )
{
    return MDrv_PCMCIA_ResetInterfaceV2(PCMCIA_DEFAULT_MODULE);
}

MS_BOOL MDrv_PCMCIA_IsDataAvailable( void )
{
    return MDrv_PCMCIA_IsDataAvailableV2(PCMCIA_DEFAULT_MODULE);
}

MS_U16 MDrv_PCMCIA_ReadData( MS_U8* u8pReadBuffer, MS_U16 u16ReadBufferSize )
{
    return MDrv_PCMCIA_ReadDataV2(PCMCIA_DEFAULT_MODULE, u8pReadBuffer, u16ReadBufferSize);
}

MS_BOOL MDrv_PCMCIA_WriteData( MS_U8* u8pWriteBuffer, MS_U16 u16DataLen )
{
    return MDrv_PCMCIA_WriteDataV2(PCMCIA_DEFAULT_MODULE, u8pWriteBuffer, u16DataLen);
}

MS_BOOL MDrv_PCMCIA_SwitchToIOmode( PCMCIA_INFO *pInfo )
{
    return MDrv_PCMCIA_SwitchToIOmodeV2(PCMCIA_DEFAULT_MODULE, pInfo);
}

MS_U16 MDrv_PCMCIA_NegotiateBufferSize( PCMCIA_INFO *pInfo )
{
    return MDrv_PCMCIA_NegotiateBufferSizeV2(PCMCIA_DEFAULT_MODULE, pInfo);
}

void MDrv_PCMCIA_WriteBufferSize( MS_U16 u16BufferSize )
{
    MDrv_PCMCIA_WriteBufferSizeV2(PCMCIA_DEFAULT_MODULE, u16BufferSize);
}

MS_BOOL MDrv_PCMCIA_WaitForStatusBit( MS_U8 u8StatusBit )
{
    return MDrv_PCMCIA_WaitForStatusBitV2(PCMCIA_DEFAULT_MODULE, u8StatusBit);
}

void MDrv_PCMCIA_ResetHW( void )
{
    MDrv_PCMCIA_ResetHW_V2(PCMCIA_DEFAULT_MODULE);
}

void MDrv_PCMCIA_WriteAttribMem( MS_U16 wAddr, MS_U8 bData )
{
    MDrv_PCMCIA_WriteAttribMemV2(PCMCIA_DEFAULT_MODULE, wAddr, bData);
}

void MDrv_PCMCIA_ReadAttribMem( MS_U16 u16Addr, MS_U8 *pDest )
{
    MDrv_PCMCIA_ReadAttribMemV2(PCMCIA_DEFAULT_MODULE, u16Addr, pDest);
}

void MDrv_PCMCIA_WriteIOMem( MS_U16 wAddr, MS_U8 bData )
{
    MDrv_PCMCIA_WriteIOMemV2(PCMCIA_DEFAULT_MODULE, wAddr, bData);
}

void MDrv_PCMCIA_WriteIOMemLong( MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer)
{
    MDrv_PCMCIA_WriteIOMemLongV2(PCMCIA_DEFAULT_MODULE, u16Addr, u8Value, u16DataLen, u8pWriteBuffer);
}

MS_U8 MDrv_PCMCIA_ReadIOMem( MS_U16 wAddr )
{
    return MDrv_PCMCIA_ReadIOMemV2(PCMCIA_DEFAULT_MODULE, wAddr);
}

MS_U32 MDrv_PCMCIA_Get_CD_Interval( void )
{
    return MDrv_PCMCIA_Get_CD_IntervalV2(PCMCIA_DEFAULT_MODULE);
}
