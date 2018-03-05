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
// (¡uMStar Confidential Information¡v) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"

#include "mdrv_mfe_io.h"
#include "mdrv_mfe_st.h"

#ifdef _FPGA_
    #include "fpga_def.h"
#endif

#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
    #include <linux/autoconf.h>
    #include <linux/module.h>
    #include <linux/moduleparam.h>
    #include <linux/init.h>
    #include <linux/sched.h>
    #include <linux/kernel.h>
    //#include "chip_int.h"

    #include <linux/fs.h>
    #include <linux/cdev.h>
    #include <linux/errno.h>
    #include <linux/delay.h>
    #include <linux/kdev_t.h>
    #include <linux/slab.h>
    #include <linux/mm.h>
    #include <linux/ioport.h>
    #include <linux/interrupt.h>
    #include <linux/workqueue.h>
    #include <linux/poll.h>
    #include <linux/wait.h>
    #include <linux/cdev.h>
    #include <linux/time.h>  //added
    #include <linux/timer.h> //added
    #include <asm/io.h>
    #include "mdrv_probe.h"
//    #include "pthread_map.h"
#else
    #include <semaphore.h>
    #include "pthread.h"
#endif

#ifdef _ENABLE_ISR_
    static int attach_ISR=1; //for AEON ISR attach
#endif

#ifdef _AEON_PLATFORM_
    #ifdef _MFE_T8_
        #include "mdrv_irq.h"
        #include "drvISR.h"
    #endif

#endif

#if defined(WIN32) && defined(_ENABLE_ISR_)
    //#include <malloc.h>
    #include <windows.h>
    #include "pthread.h"
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&(!defined(_KERNEL_MODE_))
    #include "MsCommon.h"
    #include "MsOS.h"
    #include "drvMMIO.h"
    #include "pthread_map.h"
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
    #include <linux/kernel.h>
    #include <linux/string.h>
       #include <linux/semaphore.h>
    #include <linux/interrupt.h>
    #include <linux/kdev_t.h>
    #include "chip_int.h"
    #include "mst_devid.h"
    #include "mdrv_probe.h"
    #include "mst_devid.h"
    //#include "./Aeon/drvMFE.h"
#else
    //#include "drvMFE.h"
    #include <string.h>
#endif

#include "mhal_mfe.h"
#include "memmap.h"
#include "mdrv_mfe.h"
#include "stdarg.h"

#include "BufMng.h"


#ifdef WIN32
    #include "sigdump_api.h"
#endif


#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
    #include "mdrv_system.h" //for addr_phy2log
#endif

MFE_HW_STATUS hw_status;

OutBitSBUF outbitsbuf[MAX_OUTBUFFER];
int rbits_index=0;
static MFE_U32 _MFE_U32MFEEvent = 0;
BITSFRAME_INFO bitsframe[MAX_OUTBUFFER];

MFE_CONFIG gMfeConfig, *gpMfeConfig = &gMfeConfig;
GOPINFO gGopInfo, *gpGopInfo = &gGopInfo;

#ifdef CRC_CHECK
#include "CRC64.h"
CRC_INFO gCRC_AllFrames;
CRC_INFO gCRC_DramBitstream;
#endif
volatile MFE_STAT encode_state;
MFE_BOOL g_bForceIframe = FALSE;

MFE_U32 MFE_U32MFEMIUOffset=0;

MFE_U32 MFEClockLevel = 3;  //0 to 3 = 123,144,172,192 MHz

#ifdef _WIN32 //!defined(_AEON_PLATFORM_) && !defined(_MIPS_PLATFORM_)
MFE_S8 Script_Buffer[2048];
#endif

// This is for Main() FPGA test-case verification.
#define _MAIN_TEST_
#if defined(_MAIN_TEST_)
#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)

#else
//    extern int TEST_CASE_FORMAT;
//    extern int TEST_CASE_NUM;
#endif
#endif

#ifdef DRV_MFE_TIME_MEASURE
    #include "mdrv_types.h"
    #include "../scaler/titania8/mhal_utility.h"
    #include "../scaler/titania8/mhal_scaler_reg.h"

void MFE_MsOS_START_TIMER (void)
{
    REG_WR(REG_PIU_TIMER0(0x00), 0x0);
    REG_WR(REG_PIU_TIMER0(0x00), 0x1);
}

static inline void MFE_MsOS_PRINT_TIMER (void)
{
    MFE_U32 MFE_U32Timer;
    MFE_U32Timer = ((MFE_U32)REG_RR(REG_PIU_TIMER0(0x04))) |
               ((MFE_U32)REG_RR(REG_PIU_TIMER0(0x05)) << 16); // h0005:h0004
    //printk("MFE encode time=%u us\n", MFE_U32Timer/12);
    printk("%u\n", MFE_U32Timer/12);
}
#endif



#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
    struct MFE_Dev  g_MFE_Dev;
    static wait_queue_head_t        _MFE_wait_queue;
    static spinlock_t               _spinlock;

    MFE_S32 MDrv_MFE_Open(struct inode *inode, struct file *filp);
    MFE_S32 MDrv_MFE_Release(struct inode *inode, struct file *filp);
    MFE_S32 MDrv_MFE__mmap(struct file *filp, struct vm_area_struct *vma);
    static MFE_U32 MDrv_MFE_poll(struct file *filp, poll_table *wait);

       int MDrv_MFE_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
    struct file_operations MFE_Fops = {
        .owner =    THIS_MODULE,
        .ioctl =      MDrv_MFE_Ioctl,
        .open =     MDrv_MFE_Open,
        .release =  MDrv_MFE_Release,
        .mmap =        MDrv_MFE__mmap,
        .poll =     MDrv_MFE_poll,
    };
#else
    #if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
        MFE_S32 _MFE_wait_queue;
    #else
        sem_t        _MFE_wait_queue;
    #endif
    pthread_mutex_t _spinlock;
    #define wake_up(a) sem_post(a)
    #define spin_lock pthread_mutex_lock
    #define spin_unlock pthread_mutex_unlock
    #define spin_lock_init pthread_mutex_init
    #ifdef _WIN32
        #define PROBE_IO_ENTRY
        #define _IOC_NR
        #define PROBE_IO_EXIT
    #else
        #define PROBE_IO_ENTRY(fmt, args...)
        //#define _IOC_NR(fmt, args...)
        #define PROBE_IO_EXIT(fmt, args...)
    #endif
    #define MDRV_MAJOR_MFE 0
#endif //defined(_MFE_T8_)&&defined(_KERNEL_MODE_)


static void MDrv_MFE_Sem_Init(void)
{
    char name[] = "DRV_MFE_SEM";
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
     _MFE_wait_queue = MsOS_CreateSemaphore(0,E_MSOS_FIFO ,name) ;
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
     sem_init(&_MFE_wait_queue, 0, 0);
#endif

}

static MFE_BOOL MDrv_MFE_Sem_Wait(MFE_U32 u32WaitMs)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
     return MsOS_ObtainSemaphore(_MFE_wait_queue, u32WaitMs);
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
     sem_wait(&_MFE_wait_queue);
#endif

}

static void MDrv_MFE_Sem_Destroy(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
//    MsOS_ReleaseSemaphore(_MFE_wait_queue);
    MsOS_DeleteSemaphore(_MFE_wait_queue);
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    sem_destroy(&_MFE_wait_queue);
#endif

}

static void MDrv_MFE_Sem_Wake_Up(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
    MsOS_ReleaseSemaphore(_MFE_wait_queue);
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    wake_up(&_MFE_wait_queue);
#endif

}

static MFE_BOOL MDrv_MFE_CheckEncodeDone(MFE_CONFIG *pConfig);

// user mode function
#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
void MDrv_MFE_SetOSRegBase( MFE_U32 MFE_U32RegBaseAddr )
{
    MHAL_MFE_InitRegBase( (MFE_U32)MFE_U32RegBaseAddr );
}
#endif

//////////////////////////////////////////////////////////////////////////
// Kernel driver specific
//////////////////////////////////////////////////////////////////////////

#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)

S32 MDrv_MFE_Open(struct inode *inode, struct file *filp)
{
    return 0;

}

S32 MDrv_MFE_Release(struct inode *inode, struct file *filp)
{
//    free_irq(E_IRQ_MFE, NULL)

    return 0;
}

S32 MDrv_MFE__mmap(struct file *filp, struct vm_area_struct *vma)
{
    return 0;
}

static unsigned int MDrv_MFE_poll(struct file *filp, poll_table *wait)
{
    unsigned int ret;
//    ms_dprintk(DRV_L4, "MDrv_MFE_poll is invoked 0x%x\n", &_MFE_wait_queue);
    poll_wait(filp, &_MFE_wait_queue,  wait);
  //  ms_dprintk(DRV_L4, "MDrv_MFE_poll is waken up %d\n", _MFE_U32MFEEvent);
    spin_lock(&_spinlock);
    ret = _MFE_U32MFEEvent ? POLLIN : 0;
    ms_dprintk(DRV_L4, "leave MDrv_MFE_poll, %d %d\n", ret, _MFE_U32MFEEvent);
    spin_unlock(&_spinlock);
    return  ret;//  | POLLPRI;
}

static int __init MDrv_MFE_ModuleInit(void)
{
    dev_t       devno;
    int         result;
    ms_dprintk(DRV_L4, "in MDrv_MFE_ModuleInit\n");
    devno = MKDEV(MDRV_MAJOR_MFE, MDRV_MINOR_MFE);
    result = register_chrdev_region(devno, 1, MDRV_NAME_MFE);
    if ( result < 0)
    {
        ms_dprintk(DRV_L4, KERN_WARNING "MFE: can't get major %d\n", MDRV_MAJOR_MFE);
        return result;
    }
    g_MFE_Dev.cdev.owner = THIS_MODULE;
    g_MFE_Dev.cdev.kobj.name = MDRV_NAME_MFE;
    g_MFE_Dev.cdev.ops = &MFE_Fops;
    cdev_init(&(g_MFE_Dev.cdev), &MFE_Fops);
    result = cdev_add (&(g_MFE_Dev.cdev), devno, 1);
    /* Fail gracefully if need be */
    if (result) {
        ms_dprintk(DRV_L4, KERN_NOTICE "Error add MFE device");
    }
    spin_lock_init(&_spinlock);
    init_waitqueue_head(&_MFE_wait_queue);
    return 0;
}

static void __exit MDrv_MFE_ModuleExit(void)
{
    cdev_del(&g_MFE_Dev.cdev);
    unregister_chrdev_region(MKDEV(MDRV_MAJOR_MFE, MDRV_MINOR_MFE), 1);
}

module_init(MDrv_MFE_ModuleInit);
module_exit(MDrv_MFE_ModuleExit);

//ken LGE
MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("MFE driver");
MODULE_LICENSE("MSTAR");

#endif //#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)



//////////////////////////////////////////////////////////////////////////
// ioctl API's
//////////////////////////////////////////////////////////////////////////

static void MDrv_MFE_ENC_Continue(MFE_CONFIG *pConfig)
{
    MFE_ASSERT(outbitsbuf[pConfig->nOBufIndex].used_size==0);
    ms_dprintk(DRV_L3, "Continue with buf[%d]\n",(unsigned int) pConfig->nOBufIndex);
       MHal_MFE_set_outbitsbuf(&outbitsbuf[pConfig->nOBufIndex]);
    encode_state = WAIT_FRAME_DONE;
}


static void MDrv_MFE_clear_bitsbuf(MFE_CONFIG *pConfig, MFE_U32 clear_num)
{

    ms_dprintk(DRV_L3, "clear rbits_index: %d state=%d\n", rbits_index, encode_state);
    ms_dprintk(DRV_L1,"clear_num = %d\n",(unsigned int)clear_num );
    hw_status = GETBITS_NOT_READY;

    spin_lock(&_spinlock);
    MFE_ASSERT(clear_num==1);
    ms_dprintk(DRV_L3, "clear rbits_index: %d state=%d\n", rbits_index, encode_state);

    outbitsbuf[rbits_index].used_size = 0;
    rbits_index = MFE_MUX((rbits_index+1)==pConfig->ctxBufInfo.m_nOutBuffer, 0, rbits_index+1);

    spin_unlock(&_spinlock);
    if (encode_state==WAIT_AVAIL_BITSBUF)
        MDrv_MFE_ENC_Continue(pConfig);

}

static int MDrv_MFE_GetDispOrder(MFE_CONFIG *pConfig, GOPINFO *pGopInfo, MFE_U32 *pDispOrder)
{


    if (!pDispOrder) {
        ms_dprintk(DRV_L4, "[Error] MDrv_MFE_GetDispOrder: pDispOrder is NULL pointer.\n");
        return 0;
    }
    mfeSetVopType(pConfig, pGopInfo);
    *pDispOrder = pGopInfo->nDispOrder;

    ms_dprintk(DRV_L4, "MDrv_MFE_GetDispOrder: Enc=%d Disp=%d\n", (unsigned int)pGopInfo->nCodingOrder, (unsigned int)*pDispOrder);
    return 1;
}


// Consumer of copybits2file()
static int MDrv_MFE_getbits(BITSFRAME_INFO *bits_info)
{
    int i=0;
    int index;
    int num_of_buffers = gpMfeConfig->ctxBufInfo.m_nOutBuffer;
    ms_dprintk(DRV_L3, "MDrv_MFE_getbits...\n");
#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
//    Kernel mode will sleep in MDrv_MFE_poll
#else
//    MDrv_MFE_Sem_Wait(MSOS_WAIT_FOREVER);
    if(MDrv_MFE_Sem_Wait(300000)==FALSE) {
        ms_dprintk(DRV_L3, "MDrv_MFE_Sem_Wait...fail!!\n");
        return 1;
    }
    MDrv_MFE_CheckEncodeDone(gpMfeConfig);
#endif

    spin_lock(&_spinlock);
    index = rbits_index;
    // Now we only copy one buffer
    if (_MFE_U32MFEEvent!=0) {
        bits_info[i].miuAddress = bitsframe[index].miuAddress;
        bits_info[i].miuPointer = bitsframe[index].miuPointer;
        bits_info[i].size = bitsframe[index].size;
        bits_info[i].status = bitsframe[index].status;
        bits_info[i].voptype = bitsframe[index].voptype;
        bits_info[i].is_more_bits = 1;
        bits_info[i].IVOP_address= bitsframe[index].IVOP_address;
        _MFE_U32MFEEvent ^= (1<<index);
        index = MFE_MUX((index+1)==num_of_buffers, 0, index+1);
        i++;
        if (i>num_of_buffers) {
            ms_dprintk(DRV_L1, "MDrv_MFE_getbits error!! use too many bits buffer\n");
        }
    }

    bits_info[i-1].is_more_bits = 0;
    spin_unlock(&_spinlock);

    ms_dprintk(DRV_L3, "Leave MDrv_MFE_getbits.\n");
    return 0;
}


#if defined(_UDMA_)||defined(_FPGA_)
MFE_U32 DMAGetFile(char *tmpname, DWORD start, DWORD size, int type, char *Buffer)
{
#if defined(_UDMA_) || defined(_HIF_)
    FPGA_MIUReadFile(tmpname, start, size, type, Buffer);
#elif defined(_TRACE32_CMM_)
    fprintf(fp_script, "B::DATA.SAVE.BINARY %s 0x%08X--0x%08X\n", tmpname,
        addr_phy2log(start), addr_phy2log(start+size));
#endif
    return TRUE;
}

int DMAPutMem(char *from_ptr, int size, MFE_U32 to_addr, int type)
{
#if defined(_UDMA_) || defined(_HIF_)
    FPGA_MIUWrite(from_ptr, size, to_addr, type);
#elif defined(_AEON_PLATFORM_)
    memcpy((char *)addr_phy2log(to_addr), from_ptr, size);
#elif defined(_TRACE32_CMM_)
    //????
#else
    memcpy((char *)to_addr, from_ptr, size);
#endif
    return 0;
}
#endif //#if defined(_UDMA_)||defined(_FPGA_)

#ifdef CRC_CHECK
static void printf_CRC(int order,unsigned int size, unsigned char* crc)
{

    ms_dprintk(DRV_L4, "Frame#%d size = 0x%x CRC = %02x%02x %02x%02x %02x%02x %02x%02x\n",
    order,size,crc[1],crc[0],crc[3],crc[2],crc[5],crc[4],crc[7],crc[6]);

}
static void PrintCRC(MFE_CONFIG *pConfig, MFE_U8 bGetHwCRC, MFE_U8* addr, int size, MFE_U8 bFrameDone)
{
    MFE_U8 crc[8];
    int frame_count = (int) pConfig->nCodingOrder;
    unsigned int BitstreamLen = (unsigned int) MHal_MFE_GetBitstreamEncodedLen();
    int i;
#ifdef _CRC_PATTERN_GEN_
        unsigned char crc_patt[8];
#endif

//    ms_dprintk(DRV_L4, "start PrintCRC, bGetHwCRC = %d\n",bGetHwCRC);
    if (bGetHwCRC) {
        MHal_MFE_GetCRC(crc);
        ms_dprintk(DRV_L2, "[CRC HWRE] ");
        printf_CRC(frame_count,BitstreamLen,crc);

        CRC_ACC_HW_Generator(&gCRC_AllFrames, crc);
        ms_dprintk(DRV_L2, "[ACC  CRC] ");
        printf_CRC(frame_count,BitstreamLen,gCRC_AllFrames.CRCValue);

#ifdef _CRC_PATTERN_GEN_
            for(i=0;i<8;i++)
                crc_patt[i] = pConfig->CModel_pattern[8*pConfig->nCodingOrder+i];

            for(i=0;i<8;i++){
                if(crc[i] !=crc_patt[i]){
                    ms_dprintk(DRV_L2, "_CRC_PATTERN_GEN_ check error!\n");
                    ms_dprintk(DRV_L2, "[CRC PAT] ");
                    printf_CRC(frame_count,BitstreamLen,crc_patt);
                }
            }
            ms_dprintk(DRV_L2, "_CRC_PATTERN_GEN_ HW check OK!\n");
#endif
    }
    else {
              unsigned char* crc_dram;
        gCRC_DramBitstream.pBuf = addr;
        gCRC_DramBitstream.nBufSize = size;
        gCRC_DramBitstream.bIsLast = bFrameDone ? 1 : 0;
        CRC_Generator(&gCRC_DramBitstream);
        ms_dprintk(DRV_L2, "[CRC DRAM] ");
        printf_CRC((int)pConfig->nCodingOrder, (unsigned int)size,gCRC_DramBitstream.CRCValue);

            //compare with HW CRC
            MHal_MFE_GetCRC(crc);
            crc_dram = (unsigned char*)(gCRC_DramBitstream.CRCValue);
            for(i=0;i<8;i++){
                if(crc[i] !=crc_dram[i] ){
                    ms_dprintk(DRV_L2, "_CRC_PATTERN_GEN_ check error!\n");
 //                   ms_dprintk(DRV_L2, "[CRC PAT] ");
 //                   printf_CRC(frame_count,BitstreamLen,crc_patt);
                }
            }
//            ms_dprintk(DRV_L2, "_CRC_PATTERN_GEN_ HW check OK!\n");

//        get_CRC_ACC_CRC(&gCRC_AllFrames, &gCRC_DramBitstream);
//        ms_dprintk(DRV_L2, "[ACC  CRC] ");
//        printf_CRC((int)pConfig->nCodingOrder, (unsigned int)size,gCRC_AllFrames.CRCValue);

    }
//    ms_dprintk(DRV_L4, "end PrintCRC, bGetHwCRC = %d\n",bGetHwCRC);
}
#endif

static int mdrvMFE_EncodeVOP_End(MFE_CONFIG *pConfig)
{
    switch (pConfig->nCodecType) {
    case REG_ENC_MODE_MPG4:
    case REG_ENC_MODE_H263:
        mfeM4VE_UpdateFrame(pConfig);
        break;
    case REG_ENC_MODE_H264:
        mfeH264_UpdateFrame(pConfig);
        break;
    case REG_ENC_MODE_JPEG:
        MFE_ASSERT(0);
        break;
    }

#ifdef CRC_CHECK
    PrintCRC(pConfig, 1, NULL, 0, 1);
#endif

    ms_dprintk(DRV_L4, "encode one frame end\n");
    return 0;
}

static MFE_U32 get_hw_encode_done_info(MFE_U32 start)
{
    MFE_U16 u16Reg1, u16Reg2;
    MFE_U32 hwlen;
    MFE_U32 wptr;
       MFE_U32 size;
    // Final frame bit count -> byte count
    ReadRegMFE(0x42, &u16Reg1);
    ReadRegMFE(0x43, &u16Reg2);
    //ms_dprintk(DRV_L4, "u16Reg2 = 0x%x, u16Reg1 = 0x%x\n", u16Reg2, u16Reg1);
    hwlen = (((u16Reg2)<<16) | u16Reg1)>>3;
    ms_dprintk(DRV_L1, "copybits2file():HW counter total bytes = 0x%x\n",(unsigned int) hwlen);

    // The last outbuf filled-size
    ReadRegMFE(0x44, &u16Reg1);
    ReadRegMFE(0x45, &u16Reg2);
    wptr = ((u16Reg2<<16) | u16Reg1)<<3;
    ms_dprintk(DRV_L4, "copybits2file():bspobuf write pointer (8 byte unit) wptr = 0x%x\n",(unsigned int) wptr);
    size = wptr - start + ((hwlen&7)?(hwlen&7):8);
    if (size<=0)
        ms_dprintk(DRV_L4, "[Error] copybits2file(): size %d<0. start=0x%x wptr=0x%x hwlen=0x%x\n",
            (unsigned int)size,(unsigned int) start,(unsigned int) wptr,(unsigned int) hwlen);

    return size;
}

// Supplier of MDrv_MFE_getbits()
static MFE_BOOL copybits2file(MFE_CONFIG *pConfig, MFE_WORD val_1e)
{
    MFE_S32 wbits_index;
    MFE_U8   voptype = pConfig->vopPredType;
    MFE_U32 Y_start = pConfig->ctxBufInfo.m_nRecYAddr.miuAddress;
    MFE_U32 start = 0;
    MFE_S32 size = 0;
    MFE_S32 bFrameDone = CHECK_IRQ_STATUS(val_1e, IRQ_FRAME_DONE);
    MFE_S32 bBufFull = CHECK_IRQ_STATUS(val_1e, IRQ_BSPOBUF_FULL);    // Output buffer full.
    MFE_S32 bImgFull = CHECK_IRQ_STATUS(val_1e, IRQ_IMG_BUF_FULL);    // Input buffer full. Only when JPEG row-mode.

    start = outbitsbuf[pConfig->nOBufIndex].start_addr;

    if (bFrameDone) {
        ms_dprintk(DRV_L1, "copybits2file(): Frame done.\n");
        DumpAllReg(); //must define DRV_L5
        MFE_ASSERT(!bBufFull);    // This is guaranteed by HW design.

              size = get_hw_encode_done_info(start);
    }
    else if (bBufFull) {
        ms_dprintk(DRV_L1, "copybits2file(): Buffer full.\n");
        size =  pConfig->ctxBufInfo.m_OutBufferSize;
    }
    else if (bImgFull) {
        start = 0;
        size = 0;
    }
    else //if((bFrameDone | bBufFull | bImgFull) == 0)
    {
        ms_dprintk(DRV_L4, "copybits2file(): Error IRQ=0x%x not handled!\n", val_1e);
        return FALSE;
    }

    //clear all set irq, need to set before change bitstream buffer
    wbits_index = pConfig->nOBufIndex;
    pConfig->nOBufIndex = (pConfig->nOBufIndex+1)%pConfig->ctxBufInfo.m_nOutBuffer; //next outbuf
    //ms_dprintk(DRV_L4, "OBufIndex=%d, state=%d\n", pConfig->nOBufIndex, encode_state);


    if (outbitsbuf[pConfig->nOBufIndex].used_size>0 && (!bFrameDone)) {
        ms_dprintk(DRV_L1, "MFE encoding bitsbuf full! %d %d\n",(unsigned int) pConfig->nOBufIndex, (int)size);
        encode_state = WAIT_AVAIL_BITSBUF;
    } else {
        if (!bFrameDone) {
            MHal_MFE_set_outbitsbuf(&outbitsbuf[pConfig->nOBufIndex]);
            encode_state = WAIT_FRAME_DONE;
        }
    }

    MHal_MFE_ClearIRQ(val_1e);    // Conrad: Should this be here or above??

    // Output info filling
    bitsframe[wbits_index].miuAddress = start;
    bitsframe[wbits_index].miuPointer = (MFE_U32)addr_phy2log(start + MFE_U32MFEMIUOffset);
    bitsframe[wbits_index].size = size;

    if (bFrameDone)
        bitsframe[wbits_index].status = BITS_INFO_FRAMEDONE;
    else if(bBufFull)
        bitsframe[wbits_index].status = BITS_INFO_BUFFULL;
    else if (bImgFull)
        bitsframe[wbits_index].status = BITS_INFO_IMG_BUFFULL;
/*
    if((bFrameDone | bBufFull | bImgFull) == 0) {
        assert(0);
//        bitsframe[wbits_index].status = BITS_INFO_UNKNOW;
    }
*/
    bitsframe[wbits_index].voptype = voptype;
    // For I-VOP Thumbnail
    if (voptype==I_VOP) {
        bitsframe[wbits_index].IVOP_address = Y_start;
    } else {
        bitsframe[wbits_index].IVOP_address = 0;
    }

#ifdef _WIN32
    //we don't lock on mips, Aeon, ARM because these codes are in ISR in those platforms.
    spin_lock(&_spinlock);
#endif
    _MFE_U32MFEEvent |= 1<<wbits_index;
#ifdef _WIN32
    spin_unlock(&_spinlock);
#endif


    // NOTE: When acts as kernel mode, wait up for poll()
    //       When acts as user mode, release semaphore (paired with MDrv_MFE_getbits())
    //printk("next: 0x%x  prev: 0x%x\n", _MFE_wait_queue.task_list.next, _MFE_wait_queue.task_list.prev);
    if (bFrameDone) {
    mdrvMFE_EncodeVOP_End(pConfig);
#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
    //MIU MODE, check type(0:BPS,1:MC.2:MV,3:GN)
    MHal_MFE_Enable_MIU_Protection_Check(TEST_MIU_PROTECTION_MODE,0);
#endif
    encode_state = WAIT_INPUT_FRAME;
    }

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) && defined(_KERNEL_MODE_)
    if (bFrameDone || bBufFull) {
        //wake_up(&_MFE_wait_queue);
        MDrv_MFE_Sem_Wake_Up();
    }
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
    if (bFrameDone || bBufFull)
        return TRUE;
    else {
        ms_dprintk(DRV_L1, "[ERROR] CHECK_IRQ_STATUS not bFrameDone or bBufFull.\n");
        return FALSE;
    }
#endif
    return TRUE;
}

static MFE_BOOL MDrv_MFE_CheckEncodeDone(MFE_CONFIG *pConfig)
{
    MFE_U16 val_1e;
    //MFE_U8  voptype = pConfig->vopPredType;
    //int sgc_frame_done;

    MHal_MFE_GetIRQ(&val_1e);
    if (val_1e==0) {
        ms_dprintk(DRV_L4, "[WARN] CheckEncodeDone() reg1e=0\n");
        return FALSE;
    }

    //h1e: [7:0]  status of interrupt on CPU side
    //    {2'b0, net_trigger, fs_fail_irq, txip_time_out, bspobuf_full_irq, img_buf_full_irq, marb_bspobuf_ful, frame_done_irq}
    //sgc_frame_done = CHECK_IRQ_STATUS(val_1e, IRQ_FRAME_DONE);
    copybits2file(pConfig, val_1e);

    ms_dprintk(DRV_L4, "encode_state: %d\n", encode_state);
    return TRUE;
}

#ifdef _ENABLE_ISR_

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& defined(_KERNEL_MODE_)
static irqreturn_t MDrv_MFE_ISR(int irq, void *dev_id )
{

    ms_dprintk(DRV_L1, "\\\\\\\\\\\\\\\\\\\\\\\\\\\nGet in MDrv_MFE_ISR!\n\\\\\\\\\\\\\\\\\\\\\\\\\\\n");

#ifdef DRV_MFE_TIME_MEASURE
    MFE_MsOS_PRINT_TIMER();
//    ms_dprintk(DRV_L6, "[MFE_TIME_ISR] timetick_end = %ld,  [%ld] \n",timetick_end,timetick_end-timetick_start);
#endif
    MDrv_MFE_CheckEncodeDone(gpMfeConfig);
    return IRQ_HANDLED;
//    cyg_interrupt_unmask(vector); //Unmask the interrup that has been masked in _enc_isr()
}

void MFE_ISR_Enable(MFE_U8 y)
{
    int result;
    ms_dprintk(DRV_L4, "Enable MFE ISR\n");
    //enable_irq(E_IRQ_M4VE);
    result = request_irq(E_IRQ_MFE, MDrv_MFE_ISR, SA_INTERRUPT, "MFE_IRQ", NULL);
    if(result==1)
        ms_dprintk(DRV_L1, "ERROR Enable MFE ISR: result = %d\n",result);
    else if(result==0)
           ms_dprintk(DRV_L1, "Success Enable MFE ISR: result = %d\n",result);
    else
           ms_dprintk(DRV_L1, "ERROR Enable MFE ISR: result = %d(undefine value)\n",result);
}

#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)

static MS_BOOL MFE_ISR_Enable(InterruptNum y)
{
    MS_BOOL ret;

    ms_dprintk(DRV_L4, "Enable MFE ISR\n");
    ret = MsOS_EnableInterrupt (y);

    return ret;
}

static MS_BOOL MFE_ISR_Disable(InterruptNum y)
{
    ms_dprintk(DRV_L1, "Disable MFE ISR\n");
    return MsOS_DisableInterrupt (y);
}

static void MDrv_MFE_ISR(InterruptNum eIntNum)
{
//    char temp[30];
//    char real[30];

    ms_dprintk(DRV_L4, "\n\n\nget in MDrv_MFE_ISR\n\n\n");
//    ms_dprintk(TIME_L1, "[Time] get in MDrv_M4VE_ISR %d\n",(unsigned int) MsOS_GetSystemTime());

    //MFE_ISR_Disable(eIntNum);
    MDrv_MFE_Sem_Wake_Up();

    //move to MDrv_MFE_getbits(). ISR should not be used so much cpu time.
    //MDrv_MFE_CheckEncodeDone(gpMfeConfig);
    //MFE_ISR_Enable (eIntNum);
    hw_status = GETBITS_READY;
}

static MS_BOOL MFE_ISR_Attach(InterruptNum y,InterruptCb pIntCb)
{
    ms_dprintk(DRV_L1, "Attach MFE ISR\n");
    return MsOS_AttachInterrupt(y,pIntCb);

}
static MS_BOOL MFE_ISR_Detach(InterruptNum y)
{
    ms_dprintk(DRV_L1, "Detach MFE ISR\n");
    return MsOS_DetachInterrupt(y);
}

#elif defined(_WIN32)
// Simulate interrupts by one thread
static void* MDrv_MFE_ISR( void *dev_id )
{
#if !defined(_FPGA_)
    MFE_U32 nOutBufEndAddr;
    MFE_U32 nFrameBits = 0;
#endif
    U16 val_1e = 0;

    ms_dprintk(DRV_L4, "get in MDrv_MFE_ISR\n");
    srand(1000);
    while(1) {
        Sleep(100);
        if (encode_state==WAIT_FRAME_DONE) {
#if defined(_FPGA_)
            MHal_MFE_GetIRQ(&val_1e);
            if (val_1e)
                MDrv_MFE_CheckEncodeDone(gpMfeConfig);
#else
            if (rand()%4) {     //buffer full
                nOutBufEndAddr = ((outbitsbuf[gMfeConfig.nOBufIndex].start_addr+gpMfeConfig->ctxBufInfo.m_OutBufferSize)>>MIU_SHIFT)-1;
                nFrameBits += gpMfeConfig->ctxBufInfo.m_OutBufferSize*8;
                WriteRegMFE(0x1e, (1<<IRQ_BSPOBUF_FULL), "", 0, "");
                WriteRegMFE(0x44, LOWORD(nOutBufEndAddr), "", 0, "");
                WriteRegMFE(0x45, HIWORD(nOutBufEndAddr), "", 0, "");
            }
            else { //frame done
                unsigned long temp;
                temp = rand()%gpMfeConfig->ctxBufInfo.m_OutBufferSize;
                nOutBufEndAddr = ((outbitsbuf[gMfeConfig.nOBufIndex].start_addr+gpMfeConfig->ctxBufInfo.m_OutBufferSize)>>MIU_SHIFT)-1;
                nFrameBits += temp*8;
                WriteRegMFE(0x1e, (U16)((1<<IRQ_FRAME_DONE)/*|((my_temp==(OBUF_SIZE-1))<<IRQ_BSPOBUF_FULL)*/), "", 0, "");
                WriteRegMFE(0x44, LOWORD(nOutBufEndAddr), "", 0, "");
                WriteRegMFE(0x45, HIWORD(nOutBufEndAddr), "", 0, "");
                WriteRegMFE(0x42, LOWORD(nFrameBits), "", 0, "");
                WriteRegMFE(0x43, HIWORD(nFrameBits), "", 0, "");
                nFrameBits = 0;
            }
            MDrv_MFE_CheckEncodeDone(gpMfeConfig);
#endif
        }
    }

    return 0;//IRQ_HANDLED;
//    cyg_interrupt_unmask(vector); //Unmask the interrupt that has been masked in _enc_isr()
}

void MFE_ISR_Enable(MFE_U8 y)
{
    //int irq;
    pthread_t thread_info;
    pthread_attr_t thrattr;
    struct sched_param thrsched;

    pthread_attr_init(&thrattr);
    pthread_attr_setdetachstate(&thrattr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setinheritsched(&thrattr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&thrattr, SCHED_RR);
    pthread_attr_getschedparam(&thrattr, &thrsched);
    thrsched.sched_priority = 80;
    pthread_attr_setschedparam(&thrattr, &thrsched);
    pthread_attr_getschedparam(&thrattr, &thrsched);
    if (0 != pthread_create(&thread_info, &thrattr, MDrv_MFE_ISR, NULL)) {
        ms_dprintk(DRV_L4, "Mhal MFE ISR create thread error\n");
    }
}

#endif // Platforms
#endif //_ENABLE_ISR


void msAPI_MFE_EncodeVOP(MFE_CONFIG *pConfig, GOPINFO* pGopInfo)
{
//    MHal_MFE_SetCLKCTL();

    ms_dprintk(DRV_L1, "msAPI_MFE_EncodeVOP : %d(I/P/B)\n", pConfig->vopPredType);

    // Call underlying encoding routine
#if defined(_FPGA_)
    {
        MEMMAP_t *pAddr = &pConfig->ctxBufInfo.m_nCurYAddr;
        ms_dprintk(DRV_L4, "[UMDA] Put CurY/C: LogAddr=0x%08x --> PhyAddr=0x%08x\n", pAddr->miuPointer, pAddr->miuAddress);
        DMAPutMem(pAddr->miuPointer, pConfig->nBufWidth*pConfig->nBufHeight*3/2, pAddr->miuAddress, 0);
    }
#endif

    switch (pConfig->nCodecType) {
    case REG_ENC_MODE_MPG4:
    case REG_ENC_MODE_H263:
        mfeM4VE_EncodeFrame(pConfig, pGopInfo);
        break;
    case REG_ENC_MODE_H264:
        mfeH264_EncodeFrame(pConfig, pGopInfo);
        break;
    case REG_ENC_MODE_JPEG:
        MFE_ASSERT(0);
        break;
    }
    encode_state = WAIT_FRAME_DONE;

#ifdef DRV_MFE_TIME_MEASURE
        MFE_MsOS_START_TIMER();
//    timetick_start = jiffies; //MDrv_SYS_GetSyetemTime();
//    ms_dprintk(DRV_L6, "[MFE_TIME_STR] timetick_str = %ld\n",timetick_start);
//ftime(&tp1);
#endif

    ms_dprintk(DRV_L4, "In msAPI_MFE_EncodeVOP() After encode_state = WAIT_FRAME_DONE\n");
}

#if defined(_MAIN_TEST_)
void mdrvLoadTestCasePre(MFE_CONFIG *pConfig)
{

    if (pConfig->nCodecType==0) {    // MPEG-4
        if (pConfig->test_case_num==0) {
            pConfig->bQuantType = 0;
        }
        else if (pConfig->test_case_num==1) {
            pConfig->bQuantType = 0;
            pConfig->nVPSizeInBits = 12000;
        }
        else if (pConfig->test_case_num==2) {
            pConfig->bQuantType = 1;
        }
        else if (pConfig->test_case_num==3) {
            pConfig->bQuantType = 1;
        }
        else if (pConfig->test_case_num==4) {
            pConfig->bQuantType = 1;
        }
        else if (pConfig->test_case_num==5) {
            pConfig->bQuantType = 0;
        }
        else if (pConfig->test_case_num==99) {
            pConfig->bQuantType = 1;
        }
              else
                MFE_ASSERT(0);
    }
}
void mdrvLoadTestCasePost(MFE_CONFIG *pConfig)
{
    if (pConfig->nCodecType==0) {    // MPEG-4
        M4VEINFO* pInfo = &pConfig->ctxM4veInfo;
        if (pConfig->test_case_num==1) {
            pInfo->iSearchRangeForward = 16;
            if (pInfo->SEARCH_RANGE_Y<=16 && pInfo->iSearchRangeForward<=16)
                pInfo->iFCode = 1;
            else
                pInfo->iFCode = 2;
        }
    }

    if (pConfig->nCodecType==2) {    // H264
        H264INFO* pInfo = &pConfig->ctxH264Info;
        if (pConfig->test_case_num==1) {
            pInfo->InterSearch[0][BLOCK16x8] = 0;
            pInfo->InterSearch[0][BLOCK8x16] = 0;
            pInfo->InterSearch[0][BLOCK8x4] = 0;
            pInfo->InterSearch[0][BLOCK4x8] = 0;
            pInfo->InterSearch[0][BLOCK4x4] = 0;
        }
        else if (pConfig->test_case_num==2) {
            int nTotalMBs = (pConfig->nBufWidth>>4) * (pConfig->nBufHeight>>4);
            pInfo->nMaxP8x8Count = nTotalMBs * /*input->p8x8_percentage*/10 / 100;
            pInfo->nMaxP8x8Count = (pInfo->nMaxP8x8Count>>4)<<4;
            if (pInfo->nMaxP8x8Count<16)    // HW minimal unit is 16
                pInfo->nMaxP8x8Count = 16;
            if (pInfo->nMaxP8x8Count>63*16)    // HW reg 6 bits
                pInfo->nMaxP8x8Count = 63*16;
        }
        else if (pConfig->test_case_num==3) {
            pInfo->iSearchRangeForward = 16;
        }
        else if (pConfig->test_case_num==4) {
            pInfo->iSearchRangeForward = 16;
            pInfo->num_ref_frames = 1;
            pInfo->InterSearch[0][0] = 0;    // PSKIP
            pInfo->InterSearch[0][BLOCK16x8] = 0;
            pInfo->InterSearch[0][BLOCK8x16] = 0;
            pInfo->InterSearch[0][BLOCK8x4] = 0;
            pInfo->InterSearch[0][BLOCK4x8] = 0;
            pInfo->InterSearch[0][BLOCK4x4] = 0;
            pInfo->ieap_last_mode = 2;
        }
    }
}
#endif

static MFE_BOOL mfe_open_ISR(void)
{

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
    ms_dprintk(DRV_L4, "attach ISR %d\n", attach_ISR);
    if (attach_ISR) {
        MFE_ISR_Enable(1);
        attach_ISR=0;
    }
#elif defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
    if (attach_ISR) {
        if(MFE_ISR_Attach((InterruptNum)E_INT_IRQ_MFE,MDrv_MFE_ISR)==FALSE)
            ms_dprintk(DRV_L1, "MFE_ISR_Attach Fail\n");

        if(MFE_ISR_Enable((InterruptNum)E_INT_IRQ_MFE)==FALSE)
            ms_dprintk(DRV_L1, "MFE_ISR_Enable Fail\n");
            attach_ISR=0;
    }
#else
    ms_dprintk(DRV_L4, "attach ISR %d\n", attach_ISR);
    if (attach_ISR) {
        MFE_ISR_Enable(1);
        attach_ISR=0;
    }
#endif

    return TRUE;
}
static MFE_BOOL mfe_close_ISR(void)
{

    MFE_BOOL CloseISRret= TRUE;
#if defined(_MIPS_PLATFORM_) && defined(_MFE_T8_)&& defined(_KERNEL_MODE_)&&defined(_ENABLE_ISR_)

    ms_dprintk(DRV_L4, "In mdrv_MFE_Finish, free irq.\n");
    free_irq(E_IRQ_MFE, NULL);
    attach_ISR=1;

#elif defined(_MIPS_PLATFORM_) && defined(_MFE_T8_)&&defined(_ENABLE_ISR_)
    if(MFE_ISR_Detach(E_INT_IRQ_MFE)==FALSE)
        CloseISRret = FALSE;
    if(MFE_ISR_Disable(E_INT_IRQ_MFE)==FALSE)
        CloseISRret = FALSE;
#endif

 return CloseISRret;

}

static void MDrv_MFE_Init(MFE_CONFIG *pConfig)
{
    int i;

    ms_dprintk(DRV_L4, "In MDrv_MFE_Init..CodecType=%d\n",pConfig->nCodecType);

       hw_status = GETBITS_NOT_READY;

#if defined(_MAIN_TEST_)
    mdrvLoadTestCasePre(pConfig);
#endif
    switch (pConfig->nCodecType) {
    case REG_ENC_MODE_MPG4:
    case REG_ENC_MODE_H263:
        mfeM4VE_Init(pConfig);
        break;
    case REG_ENC_MODE_H264:
        mfeH264_Init(pConfig);
        break;
    case REG_ENC_MODE_JPEG:
        MFE_ASSERT(0);
        break;
    }
#if defined(_MAIN_TEST_)
    mdrvLoadTestCasePost(pConfig);
#endif

    ms_dprintk(DRV_L4, "In MDrv_MFE_Init..Before PowerOFF\n");
    MHal_MFE_PowerOff(0,MFEClockLevel);

    MMAPInit((MFE_U32)pConfig->dram_base, (MFE_U32)pConfig->dram_size, (MFE_U32*)&pConfig->dram_ptr);

#ifdef _FPGA_
    //reg_scan(reg_m4ve_mask, 0x80, WriteRegM4VE, ReadRegM4VE);
    while (FPGA_Connect() == 0) {
        ms_dprintk(DRV_L4, "FPGA Connection Fail!! Retry.....\n");
    }
    ms_dprintk(DRV_L4, "FPGA Connection Success!! \n");
#endif

    encode_state = WAIT_INPUT_FRAME; //init encode_state
    MHal_MFE_ResetReg();
    //DumpAllReg();

    switch (pConfig->nCodecType) {
        case REG_ENC_MODE_MPG4:
        case REG_ENC_MODE_H263:
            m4veAllocDRAM(pConfig);
            break;
        case REG_ENC_MODE_H264:
            h264AllocDRAM(pConfig);
            break;
        case REG_ENC_MODE_JPEG:
            MFE_ASSERT(0);
            break;
    }

    for (i=0; i<pConfig->ctxBufInfo.m_nOutBuffer; i++) {
        outbitsbuf[i].start_addr  = pConfig->ctxBufInfo.m_nOutBufAddr[i].miuAddress;
        outbitsbuf[i].end_addr   = pConfig->ctxBufInfo.m_nOutBufAddr[i].miuAddress+pConfig->ctxBufInfo.m_OutBufferSize;
        outbitsbuf[i].used_size  = 0;
     ms_dprintk(DRV_L4, "Obuf index %d: PhyAddr=%08x\n", i,(unsigned int) outbitsbuf[i].start_addr);
    }
    pConfig->nOBufIndex = 0;

#ifdef _ENABLE_ISR_
    attach_ISR = 1;
    mfe_open_ISR();
#endif


    MHal_MFE_ClearIRQ(0x3f);

#ifdef CRC_CHECK
    CRC_Init(&gCRC_AllFrames);
#endif

    //Sleep(400);
#ifdef _TRACE32_CMM_
    M4VE_SCRIPT(fprintf(fp_script, "B::wait.1000ms\n"));
#endif

}


static int MDrv_MFE_ENC_OneFrame(MFE_U32 YUV_addr)
{
    int i;
    MEMMAP_t cur_frame, cur_frame_C;
    cur_frame_C.size=0;
    cur_frame.size=0;
    encode_state = WRITE_MFE_REG;

#ifdef CRC_CHECK
#if defined(_MFE_T8_)&&defined(CHECK_OUTPUT_FROM_DRAM)
            CRC_Init(&gCRC_AllFrames);
#endif
            CRC_Init(&gCRC_DramBitstream);
#endif

    for (i=0; i<gpMfeConfig->ctxBufInfo.m_nOutBuffer; i++) {
        MFE_ASSERT(outbitsbuf[i].used_size==0);
        outbitsbuf[i].used_size = 0;
    }

    ms_dprintk(ADP_L1, "start to encode frame #%d(Enc) #%d(Disp)\n", (unsigned int)gpGopInfo->nCodingOrder, (unsigned int)gpGopInfo->nDispOrder);

    cur_frame.miuAddress = YUV_addr;
    cur_frame.miuPointer = (MFE_U8*)addr_phy2log(YUV_addr + MFE_U32MFEMIUOffset);

    cur_frame_C.miuAddress = cur_frame.miuAddress + gpMfeConfig->nBufWidth*gpMfeConfig->nBufHeight;
    cur_frame_C.miuPointer = (MFE_U8*)addr_phy2log(cur_frame_C.miuAddress + MFE_U32MFEMIUOffset);

    gpMfeConfig->ctxBufInfo.m_nCurYAddr = cur_frame;
    gpMfeConfig->ctxBufInfo.m_nCurCAddr = cur_frame_C;

    ms_dprintk(DRV_L4, "cur_frame: (miuAddress)0x%x  (miuPointer)0x%x\n", (unsigned int)cur_frame.miuAddress, (unsigned int)cur_frame.miuPointer);

    msAPI_MFE_EncodeVOP(gpMfeConfig, gpGopInfo);

    gpGopInfo->nCodingOrder++;
    return 0;
}


static int MDrv_MFE_Finish(void)
{

#if !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    MDrv_MFE_Sem_Destroy();
    pthread_mutex_destroy(&_spinlock);
#endif

    mfe_close_ISR();

#ifdef WIN32
    sd_close(sd_sw_cfg1);
    sd_close(sd_sw_cfg3);
#endif
    MHal_MFE_PowerOff(1,MFEClockLevel);

    return 0;
}

static void MDrv_MFE_SetMIUOffset( MFE_U32 MFE_U32MIUOffset )
{
    ms_dprintk(DRV_L4, "MFE_U32MFEMIUOffset= 0x%x\n",(unsigned int)(MFE_U32MIUOffset));
    MFE_U32MFEMIUOffset = MFE_U32MIUOffset;
}

// Output: Fill 16bit byte-length into pOutBuf[0] (LSB) and pOutBuf[1] (MSB)
//         Then following pOutBuf+2 is the byte stream of video headers.
static void MDrv_MFE_GetConfigBytes(MFE_CONFIG *pConfig, MFE_U8* pOutBuf)
{
    OutStream* pStream = NULL;
    MFE_U8* pBuf = NULL;
    int nSize = 0;

    if (pOutBuf==NULL || pConfig==NULL)
        return;

    pStream = &pConfig->m_OutStream;
    pBuf = pOutBuf;

    switch (pConfig->nCodecType) {
    case REG_ENC_MODE_MPG4:
        nSize = codeM4vConfigHeaders(pConfig);
        break;
    case REG_ENC_MODE_H264:
        nSize = codeH264ConfigHeaders(pConfig);
        break;
    case REG_ENC_MODE_H263:
    case REG_ENC_MODE_JPEG:
        pBuf[0] = 0;
        pBuf[1] = 0;
        return;
    }

    pBuf[0] = nSize&0xFF;
    pBuf[1] = (nSize>>8)&0xFF;
    pBuf += 2;
    MFE_ASSERT(pStream->m_nByteCount < 65535);
    memcpy(pBuf, pStream->m_pbFrameBuffer, pStream->m_nByteCount);

}


static void MDrv_MFE_Check_Status(MFE_U32* in_status)
{
     *in_status = hw_status;
}

#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
int MDrv_MFE_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
int MDrv_MFE_Ioctl(MFE_S32 MFE_fd, unsigned int cmd, unsigned long  arg)
#endif
{
    PVR_Info *pappPVR_Info;
    BITSFRAME_INFO *bits_info;

#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
    PROBE_IO_ENTRY(MDRV_MAJOR_MFE, _IOC_NR(cmd));
#endif

    switch(cmd)
    {
    case MFE_IOC_INIT:
        {
          ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_INIT\n");


#if !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
            MDrv_MFE_Sem_Init();
            pthread_mutex_init(&_spinlock, NULL);
#endif
#ifdef WIN32
        init_handle();
        sd_open(sd_sw_cfg1, "sw_cfg1_drv.txt");
        sd_open(sd_sw_cfg3, "sw_cfg3_drv.txt");
#endif
             pappPVR_Info = (PVR_Info *)arg;
             rbits_index = 0;
             gpGopInfo->nCodingOrder = 0;
             _MFE_U32MFEEvent = 0;

        memset(&gMfeConfig, 0, sizeof(MFE_CONFIG));

             ms_dprintk(DRV_L4, "in MDrv_MFE.c W=%d H=%d\n", pappPVR_Info->width, pappPVR_Info->height);
        gMfeConfig.nCodecType = pappPVR_Info->nCodecType;
        gMfeConfig.nDispWidth = pappPVR_Info->nDispWidth;
        gMfeConfig.nDispHeight = pappPVR_Info->nDispHeight;
             gMfeConfig.nBufWidth = pappPVR_Info->nBufWidth;
             gMfeConfig.nBufHeight = pappPVR_Info->nBufHeight;
        gMfeConfig.dram_base = (MFE_U8*)pappPVR_Info->BitsBuffStart;
        gMfeConfig.dram_size = pappPVR_Info->BitsBuffSize;
        gMfeConfig.bInterlace = pappPVR_Info->bInterlace;
        gMfeConfig.nPbetweenI = pappPVR_Info->nPbetweenI;
        gMfeConfig.nBbetweenP = pappPVR_Info->nBbetweenP;
        gMfeConfig.FrameRatex100 = pappPVR_Info->FrameRatex100;
        gMfeConfig.nBitrate = pappPVR_Info->nBitrate;
        gMfeConfig.test_case_format = pappPVR_Info->test_case_format;
        gMfeConfig.test_case_num = pappPVR_Info->test_case_num;
        gMfeConfig.nP_is_infinite = pappPVR_Info->nP_is_infinite;
             gMfeConfig.ctxBufInfo.m_nOutBuffer = pappPVR_Info->nOutBuffer;
             gMfeConfig.ctxBufInfo.m_OutBufferSize = pappPVR_Info->OutBufferSize;
        memcpy(gMfeConfig.CModel_pattern, pappPVR_Info->CModel_pattern,20*8);

        MDrv_MFE_Init(gpMfeConfig);

        }
        break;
    case MFE_IOC_ENC_ONEFRAME:
        {
        if(MFE_ISR_Enable((InterruptNum)E_INT_IRQ_MFE)==FALSE)
            ms_dprintk(DRV_L1, "MFE_ISR_Enable Fail\n");

            MDrv_MFE_ENC_OneFrame(arg);
        }
        break;
    case MFE_IOC_GET_DISPORDER:
        {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_GET_DISPORDER\n");
            MDrv_MFE_GetDispOrder(gpMfeConfig, gpGopInfo, (MFE_U32*)arg);
        }
        break;
    case MFE_IOC_GETBITS:
        {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_GETBITS\n");
            bits_info = (BITSFRAME_INFO *)arg;
            return MDrv_MFE_getbits(bits_info);
        }
        break;
    case MFE_IOC_SETBITRATE:
        {
         ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_SETBITRATE\n");
            pappPVR_Info = (PVR_Info *)arg;
            gMfeConfig.nBitrate = pappPVR_Info->nBitrate;
            ms_dprintk(DRV_L2, "[IOCTL] MFE_IOC_SETBITRATE =%d\n",gMfeConfig.nBitrate);
        }
        break;
    case MFE_IOC_RESET_BITRATE:
        {
         ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_RESET_BITRATE\n");
            pappPVR_Info = (PVR_Info *)arg;
            gMfeConfig.nBitrate = pappPVR_Info->nBitrate;
            MfeDrvRateControlInit(gpMfeConfig);
            ms_dprintk(DRV_L2, "[IOCTL] MFE_IOC_RESET_BITRATE =%d\n",gMfeConfig.nBitrate);
        }
        break;
    case MFE_IOC_RESET_FRAMERATE:
        {
         ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_RESET_FRAMERATE\n");
            pappPVR_Info = (PVR_Info *)arg;
            gMfeConfig.FrameRatex100 = pappPVR_Info->FrameRatex100;
            MfeDrvRateControlInit(gpMfeConfig);
            ms_dprintk(DRV_L2, "[IOCTL] MFE_IOC_RESET_FRAMERATE =%d\n",gMfeConfig.FrameRatex100);
        }
        break;
    case MFE_IOC_RESET_BITRATE_AND_FRAMERATE:
        {
         ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_RESET_BITRATE_AND_FRAMERATE\n");
            pappPVR_Info = (PVR_Info *)arg;
            gMfeConfig.FrameRatex100 = pappPVR_Info->FrameRatex100;
            gMfeConfig.nBitrate = pappPVR_Info->nBitrate;
            MfeDrvRateControlInit(gpMfeConfig);
            ms_dprintk(DRV_L2, "[IOCTL] MFE_IOC_RESET_BITRATE =%d,FRAMERATE=%d\n",
                gMfeConfig.nBitrate,gMfeConfig.FrameRatex100);
        }
        break;
    case MFE_IOC_CLEAR_BITSBUF:
        {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_CLEAR_BITSBUF\n");
            MDrv_MFE_clear_bitsbuf(gpMfeConfig, arg);
        }
        break;
    case MFE_IOC_FINISH:
        {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_FINISH\n");
            MDrv_MFE_Finish();
        }
        break;
    case MFE_IOC_SET_FORCEDI:   // no implement
        {
            ms_dprintk(DRV_L1, "ioctl: MDRV: M4VE_IOC_SET_FORCEDI\n");
            g_bForceIframe = TRUE;
        }
        break;
    case MFE_IOC_SET_MIU_OFFSET:
        {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_SET_MIU_OFFSET\n");
            MDrv_MFE_SetMIUOffset(arg);
        }
        break;
    case MFE_IOC_POWEROFF:
        {
            POWERON_t *tmp_Info;
            tmp_Info = (POWERON_t *)arg;
            if((tmp_Info->clock_level >>2) == 0 )
                MFEClockLevel = (MFE_U32)tmp_Info->clock_level ;
            else{
                ms_dprintk(DRV_L1, "[IOCTL] MFE clock level must be 0,1,2 or 3..default is 2.\n");
                MFEClockLevel = 2;
            }
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_POWEROFF, clock level= %d\n",tmp_Info->clock_level);
            MHal_MFE_PowerOff((MFE_U32)tmp_Info->is_off,(MFE_U32)tmp_Info->clock_level);
        }
        break;
    case MFE_IOC_GET_VOL:
        {
            MFE_U8* pBuf = (MFE_U8*)arg;
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_GET_VOL\n");
            MDrv_MFE_GetConfigBytes(gpMfeConfig, pBuf);
        }
       break;
    case MFE_IOC_CHECK_STATUS:
        {
            MDrv_MFE_Check_Status((MFE_U32*)arg);
        }
       break;
    case MFE_IOC_SET_RIU_BASE:
        {
#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_SET_RIU_BASE 0x%x\n",(unsigned int)arg);
            MDrv_MFE_SetOSRegBase(arg);
#else
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_SET_RIU_BASE only use for utopia.\n");
            return 1;
#endif
        }
       break;
    default:
            ms_dprintk(DRV_L1, "[IOCTL] MFE receive non-defined Ioctl\n");
        break;
    }

#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
    PROBE_IO_EXIT(MDRV_MAJOR_MFE, _IOC_NR(cmd));
#endif

    return 0;
}


