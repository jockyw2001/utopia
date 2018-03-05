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
/// file    drvNDS_NSK.c
/// @brief  NDS NSK Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#ifdef MSOS_TYPE_LINUX
#include "string.h"
#endif

#include "asmCPU.h"

#include "drvDSCMB.h"
extern MS_BOOL MDrv_DSCMB_SlotSwitchWrite(MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U32 u32SW0, MS_U32 u32SW1, MS_U32 u32SW2);

#if defined(CHIP_U3)
#include "../tsp/drvTSP.h"
#else
#include "../tsp2/drvTSP2.h"
#endif

#include "regNDS.h"
#include "drvNDS.h"
#include "halNDS.h"

#include "ddiNDS_HDI.h"

#include "nds.h"

extern void HALHDI_InitSVP_HW ( unsigned long  instance );
extern unsigned short HALHDI_InterruptServiceRoutine ( unsigned long instance, unsigned long interrupt_cause );


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define NSK_DEBUG_ENABLE                                0


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define NSK_TASK_STACK_SIZE                             8192//4096

#define NDS_NSK_IRQ                                     E_INT_IRQ_CA_I3//E_INT_IRQ_CA_SVP

#if defined(CHIP_U3)
#define NSK_SW_ESA_SEL_MASK                             0x000F0000
#define NSK_SW_ESA_SEL_SHFT                             16
#define NSK_SW_ESA_VARI_MASK                            0x00300000
#define NSK_SW_ESA_VARI_SHFT                            20
#define NSK_SW_ESA_MODE_MASK                            0x07000000
#define NSK_SW_ESA_MODE_SHFT                            24
#define NSK_SW_NSA_MASK                                 0xF0000000
#define NSK_SW_NSAD_MDD                                 0x10000000
#define NSK_SW_NSAS_MDD                                 0x20000000
#define NSK_SW_NSAD_MDI                                 0x40000000
#define NSK_SW_NSAS_MDI                                 0x80000000
#endif

/* Appendix A: Return value */
#define NSK_STATUS_OK                                   0x0000
#define NSK_STATUS_FAILED                               0x0001
#define NSK_INVALID_XCONNECTION                         0x0008
#define NSK_INVALID_PID                                 0x0009
#define NSK_INVALID_SCB                                 0x000A
#define NSK_INVALID_INSTANCE                            0x0011
#define NSK_INVALID_SCRAMBLING_SETTINGS                 0x0015
#define NSK_INVALID_ESA_SELECT                          0x0016
#define NSK_INVALID_ESA_SUBSELECT                       0x0017
#define NSK_INVALID_ESA_VARIANT                         0x0018
#define NSK_INVALID_LENGTH_OFFSET                       0x0019
#define NSK_INVALID_ENTRY_NUM                           0x001A
#define NSK_INVALID_CM_CHANNEL_HANDLE                   0x001C
#define NSK_INSUFFICIENT_BUFFER                         0x001D
#define NSK_INVALID_INPUT_SCB                           0x001E
#define NSK_INVALID_OUTPUT_SCB                          0x001F

/* Appendix B: Interrupt register definition */
#define NSK_CMD_COMPLETE                                0x00000001
#define NSK_ICAM_SECTION                                0x00000002
#define NSK_TIMER_EVENT                                 0x00000004
#define NSK_BAD_REG_ACCESS                              0x00000008

/* Appendix C: Scrambler / Descrambler */
#define SVPHDI_DESCRAMBLER_CONTROL_ENABLE_DESCRAMBLING  0x80
#define SVPHDI_DESCRAMBLER_CONTROL_DISABLE_DESCRAMBLING 0x00
#define SVPHDI_DESCRAMBLE_TRANSPORT_PACKET_LEVEL        0x00
#define SVPHDI_DESCRAMBLE_PES_PACKET_LEVEL              0x40

#define SVPHDI_BASE_CONTENT_MODULE_DESCRIPTOR           0x20


/* Appendix D: SCB */
#define NSK_CLEAR                                       0x00
#define NSK_EVEN                                        0x02
#define NSK_ODD                                         0x03
#define NSK_ALL_POLARITIES                              0x04
#define NSK_SCB_CLEAR                                   0x80


/* Appendix E: Scrambling Settings Constants */

// MDD / MDI length of descriptor is one
#define MDD_MDI_DESCRIPTOR_TAG                          0x10 // MDI/MDD/MSC Descriptor
#define MDD_MDI_MSC_DESCRIPTOR_TAG_LEN                  0x01
// MDD/MDI/MSC Scrambling ¡V byte 0 of the data
#define MDD_MDI_SCRAM_BIT_MASK                          0x01 // Bit 0: 0¡VMDD 1-MDI */
#define MDD_MDI_DESCRAM_BIT_MASK                        0x02 // Bit 1: 0¡VMDD 1-MDI*/

#define DESCRAMBLER_ALGORITHM_DESCRIPTOR_TAG            0x01
#define DESCRAMBLER_ALGORITHM_DESCRIPTOR_TAG_LEN        0x02
#define MSC_SCRAM_BIT_MASK                              0x01 // Bit 0: Defined in IC-Q063
#define MSC_DESCRAM_BIT_MASK                            0x02 // Bit 1: Defined in IC-Q063
#define DESCRAMBLER_ALGORITHM_FLAGS_OFFSET              0
#define DESCRAMBLER_ALGORITHM_ESA_VARIENT_OFFSET        1

#define CSH1_DESCRIPTOR_TAG                             0x02 /* CSH */

#define SVP_CM_CHANNEL_NUM                              8
#define SVP_CM_ESPID_NUM                                16

#if NSK_DEBUG_ENABLE
#define NSK_DBG(_fmt, _args...)                         printf(_fmt, ##_args)
#else
#define NSK_DBG(_fmt, _args...)                         { }
#endif

#ifdef MS_DEBUG //---------------------------------------------------------------------------------
#define NSK_ERR(_fmt, _args...)                         printf(_fmt, ##_args)
#define NSK_ASSERT(_cnd, _fmt, _args...)                                            \
                                                        if (!(_cnd)) {              \
                                                            printf(_fmt, ##_args);  \
                                                            MAsm_CPU_SwDbgBp();     \
                                                            while(1);               \
                                                        }
#else //-------------------------------------------------------------------------------------------
#define NSK_ERR(_fmt, _args...)                         { }
#define NSK_ASSERT(_cnd, _fmt, _args...)                                            \
                                                        if (!(_cnd)) {              \
                                                            printf(_fmt, ##_args);  \
                                                        }  // CAUTION: It should be before GE_ENTRY
#endif //------------------------------------------------------------------------------------------

#define SVP_ENTRY()                                     { }
#define SVP_RETURN(_ret)                                { return (_ret); }


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct _SVP_CM_EsPid
{
    MS_BOOL                         alloc;
    MS_U32                          pid;
    MS_U32                          flt_id;
    MS_U32                          dscmb_odd;
    MS_U32                          dscmb_even;
    MS_U32                          dscmb_clear;

} SVP_CM_EsPid;

typedef struct _SVP_CM_Channel
{
    MS_BOOL                         alloc;
    MS_BOOL                         enable;
    MS_U32                          nsk_sw;
//    MS_U32                          esa_sel;
//    MS_U32                          esa_subsel;
    MS_U32                          dsc_id;
    DSCMB_Type                      dsc_type;


    SVP_CM_EsPid                    es[SVP_CM_ESPID_NUM];
} SVP_CM_Channel;


//[HAL] {
typedef struct _SVP_CM_Prop_Base
{
    MS_U8                           tag;
    MS_U8                           len;
    MS_U8                           hwid[4]; // big-endian
    MS_U8                           ch_num[2]; // big-endian
} SVP_CM_Prop_Base;

typedef struct _SVP_CM_Prop
{
    SVP_CM_Prop_Base                base;
} SVP_CM_Prop;
//[HAL] }


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MS_U32                       u32NSK_Stack[NSK_TASK_STACK_SIZE/sizeof(MS_U32)];
static MS_S32                       s32NSK_TaskId = -1;
static MS_S32                       s32NSK_EventId = -1;


SVP_CM_Channel                      svp_cm[SVP_CM_CHANNEL_NUM];

//[HAL]
SVP_CM_Prop                         svp_prop = {
                                        {   SVPHDI_BASE_CONTENT_MODULE_DESCRIPTOR,
                                            6,                  // length
                                            { 0, 0, 0, 0 },     // hardware id
                                            { 8, 0 },           // max channel number
                                        },
                                    };
//[HAL]



//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------- (HAL) ----------
unsigned short HDIHAL_WriteNSKReg(unsigned long     instance,
                                  unsigned long     offset,
                                  unsigned long     length,
                                  unsigned long*    reg_data_out)
{
//    MS_U32              *reg_ptr;
    int                 i;

    NSK_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);

    if (instance >= NSK_DEV_NUM)
    {
        return NSK_INVALID_INSTANCE;
    }

    if (instance == 0)
    {
        for (i = 0; i < length ; i++)
        {
            NSK_REG_W(offset+i, *reg_data_out++);
        }
    }
    else
    {
        NSK_ASSERT(FALSE, "[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return NSK_INVALID_INSTANCE;
    }
/*
    reg_ptr = (MS_U32*)(0xBE000000 + (instance*NSK_BANK_SIZE) + (offset<<2));

    for (i = 0; i < length; i++)
    {
        *reg_ptr++ = *reg_data_out++;
    }
*/
    return NSK_STATUS_OK;
}


unsigned short HDIHAL_ReadNSKReg(unsigned long  instance,
                                 unsigned long  offset,
                                 unsigned long  length,
                                 unsigned long* reg_data_in)
{
//    MS_U32              *reg_ptr, i=0;
    int                 i;

    NSK_DBG("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    if (instance >= NSK_DEV_NUM)
    {
        return NSK_INVALID_INSTANCE;
    }

    if (instance == 0)
    {
        for (i = 0; i < length ; i++)
        {
            *reg_data_in++ = NSK_REG_R(offset+i);
        }
    }
    else
    {
        NSK_ASSERT(FALSE, "[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return NSK_INVALID_INSTANCE;
    }
/*
    reg_ptr = (MS_U32*)(0xBE000000 + (instance*NSK_BANK_SIZE) + (offset<<2));

    for (i = 0; i < length; i++)
    {
        *reg_data_in++ = *reg_ptr++;
    }
*/

    return NSK_STATUS_OK;
}


MS_U32 SWAP(MS_U8 *value)
{
    return ((MS_U32)*(value)<<24) | ((MS_U32)*(value+1)<<16) | ((MS_U32)*(value+2)<<8) | ((MS_U32)*(value+3));
}


unsigned short HDIHAL_WriteNSKData(unsigned long    instance,
                                   unsigned long    offset,
                                   unsigned long    length,
                                   unsigned char*   data_out)
{
//    MS_U32              *reg_ptr;
    MS_U32              tmp;
//    MS_U8               *tmp_ptr;
    NSK_DBG("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    if (instance >= NSK_DEV_NUM)
    {
        return NSK_INVALID_INSTANCE;
    }

    if (instance == 0)
    {
        while (length)
        {
            if (length >= 4)
            {
                tmp = SWAP(data_out);
                data_out += 4;
                length -= 4;
            }
            else
            {
                tmp = 0;
                while (length)
                {
                    tmp |= ((MS_U32)*(data_out)) << ((3-length)<<3);
                    data_out++;
                    length--;
                }
            }
            NSK_REG_W(offset, tmp);
            offset++;
        }
    }
    else
    {
        NSK_ASSERT(FALSE, "[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return NSK_INVALID_INSTANCE;
    }

/*
    reg_ptr = (MS_U32*)(0xBE000000 + (instance*NSK_BANK_SIZE) + (offset<<2));

    while (length)
    {
        if (length >= 4)
        {
            L2B(data_out);
            *reg_ptr = *(MS_U32*)(data_out);
            reg_ptr++;
            data_out += 4;
            length -= 4;
        }
        else
        {
            tmp = 0;
            tmp_ptr = (MS_U8 *)&tmp;
            while(length)
            {
                *tmp_ptr = *(data_out);
                tmp_ptr++;
                data_out++;
                length--;
            }
            L2B((unsigned char*)&tmp);
            *reg_ptr = tmp;
        }
    }
*/

    return NSK_STATUS_OK;
}




unsigned short HDIHAL_ReadNSKData(unsigned long     instance,
                                  unsigned long     offset,
                                  unsigned long     length,
                                  unsigned char*    data_in)
{
//    MS_U32              *reg_ptr;
    MS_U32              tmp;
    NSK_DBG("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    if (instance >= NSK_DEV_NUM)
    {
        return NSK_INVALID_INSTANCE;
    }

    if (instance == 0)
    {
        while (length)
        {
            tmp = NSK_REG_R(offset);
            if (length >= 4)
            {
                *(MS_U32*)data_in = SWAP((MS_U8*)&tmp);
                data_in += 4;
                length -= 4;
            }
            else
            {
                while (length)
                {
                    *(data_in) = tmp & 0xFF;
                    tmp >>= 8;
                    data_in++;
                    length--;
                }
            }
            offset++;
        }
    }
    else
    {
        NSK_ASSERT(FALSE, "[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return NSK_INVALID_INSTANCE;
    }


/*
    reg_ptr = (MS_U32*)(0xBE000000 + (instance*NSK_BANK_SIZE) + (offset<<2));

    while (length)
    {
        if (length >= 4)
        {
            *(MS_U32*)(data_in) = *reg_ptr;
            LE2BE(data_in);
            reg_ptr++;
            data_in += 4;
            length -= 4;

        }
        else
        {
            tmp = *reg_ptr;
            LE2BE((unsigned char*)&tmp);
            while(length)
            {
                *(data_in) = tmp & 0xFF;
                tmp >>= 4;
                data_in++;
            }
        }
    }
*/
    return NSK_STATUS_OK;
}

/*unsigned short HALHDI_InterruptServiceRoutine(
                                    unsigned long instance,
                                    unsigned long interrupt_source)
{
    return NSK_STATUS_OK;
}


unsigned short HALHDI_InitSVP_HW(unsigned long instance)
{
    return NSK_STATUS_OK;
}*/

#define NSK_INT_EVENT_0                 0x00000001
#define NSK_INT_GROUP                   0x0000FFFF

void _NSK_Isr(void)
{

    MsOS_DisableInterrupt(NDS_NSK_IRQ);
    //reg_ptr = (MS_U32*)(0xBE000000 + (instance*NSK_BANK_SIZE) + (offset<<2));
    MsOS_SetEvent(s32NSK_EventId, NSK_INT_EVENT_0); //[[ NDS BUG ]] not match in spec doc id : 482-sec 4.3

}


static void _NSK_Isr_Task(void)
{
    MS_U32              u32Reg;
    MS_U32              u32Events = 0;
    unsigned long       instance = 0;

    while(1)
    {
        MsOS_WaitEvent(s32NSK_EventId, NSK_INT_GROUP, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        //MsOS_DisableInterrupt(NDS_NSK_IRQ);
        if (u32Events & NSK_INT_EVENT_0)
        {
            u32Reg = NSK_REG_R(REG_NSK_INT);
            NSK_REG_W(REG_NSK_INT, u32Reg);

#ifndef CDI_CA
            // NDS Library
            HALHDI_InterruptServiceRoutine( instance, u32Reg );
#endif
        }
        else
        {
            u32Reg = NSK_REG_R(REG_NSK_INT);
            NSK_REG_W(REG_NSK_INT, u32Reg);
        }
        MsOS_EnableInterrupt(NDS_NSK_IRQ);

        MsOS_DelayTask(5);
    }
}


unsigned short _SVP_EnableEsPidCmChannel(unsigned int cm_channel_handle, unsigned int espid_id)
{
//TODO: maybe nothing to do
    return NSK_STATUS_OK;
}

unsigned short _SVP_RemoveEsPidCmChannel(unsigned int cm_channel_handle, unsigned int espid_id)
{

//TODO:
/*
    MS_U16              ret;
    ret = MDrv_TSP_PidFlt_Free(svp_cm[cm_channel_handle].es[cm_espid_id].flt_id);
    NSK_ASSERT((ret == TRUE), "%s FAIL\n", __FUNCTION__);
*/

    svp_cm[cm_channel_handle].es[espid_id].alloc = FALSE;

    return NSK_STATUS_OK;
}


unsigned short _SVP_DisableCmChannel(unsigned int cm_channel_handle)
{
//TODO: disable CmChannel
    return NSK_STATUS_OK;
}


unsigned short _SVP_DeallocateCmChannel(unsigned int cm_channel_handle)
{
    int                 j;

    _SVP_DisableCmChannel(cm_channel_handle);
    for (j = 0; j < SVP_CM_ESPID_NUM; j++)
    {
        if (svp_cm[cm_channel_handle].es[j].alloc == TRUE)
        {
            _SVP_RemoveEsPidCmChannel(cm_channel_handle, j);
        }
    }

    // Free CmChannel
//TODO:
/*
    MS_U16              ret;
    NSK_ASSERT((ret == TRUE), "%s FAIL\n", __FUNCTION__);
*/
    svp_cm[cm_channel_handle].alloc = FALSE;

    return NSK_STATUS_OK;
}


unsigned short _SVP_WriteKey_Parse(unsigned int     cm_channel_handle,
                                   unsigned char    scb,
                                   unsigned int     settings_len,
                                   unsigned char*   settings)
{
    MS_U32              u32SetLen = settings_len;
    DSCMB_Key_Type      key_type = E_DSCMB_KEY_CLEAR;

    switch (scb)
    {
    case NSK_CLEAR:
        key_type = E_DSCMB_KEY_CLEAR;
        break;
    case NSK_ODD:
        key_type = E_DSCMB_KEY_ODD;
        break;
    case NSK_EVEN:
        key_type = E_DSCMB_KEY_EVEN;
        break;
    default:
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    //TODO: should we support more nsk_sw for other than switch13?
    while (u32SetLen)
    {
        if (MDD_MDI_DESCRIPTOR_TAG == *settings)
        {
            svp_cm[cm_channel_handle].nsk_sw &= ~DSCMB_SW_NSA_MASK;

            if ((u32SetLen >= (MDD_MDI_MSC_DESCRIPTOR_TAG_LEN+2) &&
                *(settings+1) == MDD_MDI_MSC_DESCRIPTOR_TAG_LEN)    )
            {
                if (*(settings+2) & MDD_MDI_SCRAM_BIT_MASK)
                {
                    svp_cm[cm_channel_handle].nsk_sw |= DSCMB_SW_NSAS_MDI;
                }
                else
                {
                    svp_cm[cm_channel_handle].nsk_sw |= DSCMB_SW_NSAS_MDD;
                }
                if (*(settings+2) & MDD_MDI_DESCRAM_BIT_MASK)
                {
                    svp_cm[cm_channel_handle].nsk_sw |= DSCMB_SW_NSAD_MDI;
                }
                else
                {
                    svp_cm[cm_channel_handle].nsk_sw |= DSCMB_SW_NSAD_MDD;
                }
                u32SetLen -= MDD_MDI_MSC_DESCRIPTOR_TAG_LEN + 2;
                settings += MDD_MDI_MSC_DESCRIPTOR_TAG_LEN + 2;
            }
            else
            {
                NSK_ASSERT(TRUE, "MDD_MDI_TAG fail\n");
                return NSK_INVALID_SCRAMBLING_SETTINGS;
            }
        }
        else if (DESCRAMBLER_ALGORITHM_DESCRIPTOR_TAG == *settings)
        {
            svp_cm[cm_channel_handle].nsk_sw &= ~DSCMB_SW_ESA_VAR_MASK;

            if ((u32SetLen >= (DESCRAMBLER_ALGORITHM_DESCRIPTOR_TAG_LEN+2)   &&
                *(settings+1) == DESCRAMBLER_ALGORITHM_DESCRIPTOR_TAG_LEN)      )
            {
                // non-SVP ESA MSC (must stay clear)
                if (*(settings+2) & MSC_SCRAM_BIT_MASK)
                {
                    //TODO:
                    MAsm_CPU_SwDbgBp();
                    printf("#### Not support #### %s %d\n", __FUNCTION__, __LINE__);
                }

                if (*(settings+2) & MSC_DESCRAM_BIT_MASK)
                {
                    //TODO:
                    MAsm_CPU_SwDbgBp();
                    printf("#### Not support #### %s %d\n", __FUNCTION__, __LINE__);
                }

                // non-SVP ESA variant
                svp_cm[cm_channel_handle].nsk_sw |= ((*(settings+3)) << DSCMB_SW_ESA_VAR_SHFT) & DSCMB_SW_ESA_VAR_MASK;

                u32SetLen -= DESCRAMBLER_ALGORITHM_DESCRIPTOR_TAG_LEN + 2;
                settings += DESCRAMBLER_ALGORITHM_DESCRIPTOR_TAG_LEN + 2;

            }
            else
            {
                NSK_ASSERT(TRUE, "DSCMB_TAG fail\n");
                return NSK_INVALID_SCRAMBLING_SETTINGS;
            }
        }
        else
        {
            NSK_ASSERT(TRUE, "TAG fail\n");
            return NSK_INVALID_SCRAMBLING_SETTINGS;
        }

        if (u32SetLen < 0)
        {
            return NSK_INVALID_SCRAMBLING_SETTINGS;
        }
    }

#if defined(CHIP_U3)
    //MDrv_DSCMB_CleanSCB();
#endif
    MDrv_DSCMB_FltKeySet(svp_cm[cm_channel_handle].dsc_id, key_type, NULL); // Dummy control word for SVP mode
#if defined(CHIP_U3)
    MDrv_DSCMB_FltTypeSet(svp_cm[cm_channel_handle].dsc_id, svp_cm[cm_channel_handle].dsc_type, svp_cm[cm_channel_handle].nsk_sw, key_type);
#else

    MS_U32 i = 0 ;
    for ( i = 0 ; i < SVP_CM_ESPID_NUM ; i++ )
    {
        if (TRUE == svp_cm[cm_channel_handle].es[i].alloc)
        {
            MDrv_DSCMB_SlotSwitchWrite(svp_cm[cm_channel_handle].es[i].flt_id 
                                      ,key_type
                                      ,svp_cm[cm_channel_handle].nsk_sw, 0, 0);
            break ; 
        }
    }
#endif


    return NSK_STATUS_OK;
}

unsigned short SVPHDI_ReadSerialData(unsigned int   offset,
                                     unsigned int   length,
                                     unsigned char* data)
{
    return NSK_STATUS_OK;
}


unsigned short SVPHDI_AllocateCmChannel(unsigned long   xconn,
                                        unsigned int    esa_select,
                                        unsigned int    esa_subselect,
                                        unsigned int*   cm_channel_handle)
{
    int                 i;
//    DSCMB_Type          dsc_type = E_DSCMB_TYPE_CSA;

    SVP_ENTRY();

//TODO: x_connection
//    SVP_RETURN(NSK_INVALID_XCONNECTION);

    for(i = 0; i < SVP_CM_CHANNEL_NUM; i++)
    {
        if (svp_cm[i].alloc == FALSE)
        {
            break;
        }
    }
    if (i == SVP_CM_CHANNEL_NUM)
    {
        SVP_RETURN(NSK_STATUS_FAILED);
    }

#if defined(CHIP_U3)
    //TODO: remove it? TDES is different than DES! ECM,etc...is defined by esa_select!
    switch (esa_select)
    {
    case 0x0: // AES
        dsc_type = E_DSCMB_TYPE_AES;
        break;
    case 0x1: // DVB CSA
        dsc_type = E_DSCMB_TYPE_CSA;
        break;
    case 0x2: // DES
    case 0x3: // TDES
        dsc_type = E_DSCMB_TYPE_DES;
        break;
    case 0xF: // SW
        // regardless esa_subselect
        break;
    default:
        SVP_RETURN(NSK_INVALID_ESA_SELECT)
    }
    svp_cm[i].dsc_type = dsc_type;
#else
    MS_U32 sw1, sw2, sw3, sw_cnt;
    sw_cnt = HAL_NDS_KTE_GetSwitch(esa_select, esa_subselect, &sw1, &sw2, &sw3);
    if (sw_cnt == 0)
    {
        SVP_RETURN(NSK_INVALID_ESA_SELECT);
    }
    svp_cm[i].nsk_sw = sw1; // U3/U4
#endif

    if (DRV_DSCMB_FLT_NULL == (svp_cm[i].dsc_id = MDrv_DSCMB_FltAlloc()))
    {
        SVP_RETURN(NSK_STATUS_FAILED);
    }

    // allocate descarmbler filter slot
//TODO:
/*
    MS_U16              ret;
    MDrv_DSCMB_FltAlloc
    if (ret != TRUE)
    {
        SVP_RETURN(NSK_STATUS_FAILED);
    }
*/

    svp_cm[i].alloc = TRUE;

    *cm_channel_handle = i;

    SVP_RETURN(NSK_STATUS_OK);
}


unsigned short SVPHDI_DeallocateCmChannel(unsigned int  cm_channel_handle)
{
    SVP_ENTRY();

    if ((cm_channel_handle >= SVP_CM_CHANNEL_NUM) || (svp_cm[cm_channel_handle].alloc != TRUE))
    {
        SVP_RETURN(NSK_INVALID_CM_CHANNEL_HANDLE);
    }

    // disable cm_channel
    if (_SVP_DisableCmChannel(cm_channel_handle) != NSK_STATUS_OK)
    {
        SVP_RETURN(NSK_STATUS_FAILED);
    }


    // Free Channel
//TODO:
/*
    MS_U16              ret;
    if (ret != TRUE)
    {
        SVP_RETURN(NSK_STATUS_FAILED);
    }
*/

    SVP_RETURN(NSK_STATUS_OK);
}


unsigned short SVPHDI_AddEsPidCmChannel(unsigned int    cm_channel_handle,
                                        unsigned short  es_pid)
{
    int                 j;

    SVP_ENTRY();
    if ((cm_channel_handle >= SVP_CM_CHANNEL_NUM) || (svp_cm[cm_channel_handle].alloc != TRUE))
    {
        SVP_RETURN(NSK_INVALID_CM_CHANNEL_HANDLE);
    }

    for (j = 0; j < SVP_CM_ESPID_NUM; j++)
    {
        if (svp_cm[cm_channel_handle].es[j].alloc != TRUE)
        {
            break;
        }
    }
    if (j == SVP_CM_ESPID_NUM)
    {
        SVP_RETURN(NSK_INVALID_PID);
    }

    // allocate a PID slot
//TODO:
/*
    if (ret != TRUE);
    {
        SVP_RETURN(NSK_INVALID_PID);
    }
    // enable espid if channel is enabled
    if (svp_cm[cm_channel_handle].enable == TRUE)
    {
        if (_SVP_EnableEsPidCmChannel(cm_channel_handle, j) != NSK_STATUS_OK)
        {
            SVP_RETURN(NSK_STATUS_FAILED);
        }
    }
*/
    svp_cm[cm_channel_handle].enable = TRUE;


    MS_U32          flt_id, fltnum = 0 , pid = 0 ;
#if 0
//tt
//FIXME: --jerry
    //////////////// Fix Me for SVP test ///////////////
    //-------------- Alloc and Set pid -----------------
    TSP_FltType     flt_type;
    if (0xFFFF != es_pid)
    {

        if (es_pid == 0x200)
            flt_type = E_TSP_FLT_FIFO_VIDEO;
        else if (es_pid == 0x280)
            flt_type = E_TSP_FLT_FIFO_AUDIO;
        else
            flt_type = E_TSP_FLT_USER_SEC;

        //if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
        if (E_TSP_OK != MDrv_TSP_FLT_Alloc(0, flt_type, &flt_id))
        {
            SVP_RETURN(NSK_INVALID_PID);
        }
        pid = es_pid;
        //if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(DmxIdSect, &u16Pid, TRUE))
        if (E_TSP_OK != MDrv_TSP_FLT_SetPID(flt_id, (MS_U32)pid))
        {
            SVP_RETURN(NSK_INVALID_PID);
        }
    }
//tt
#else

    if ( E_TSP_OK != MDrv_TSP_GetCaps(E_TSP_CAP_PIDFLT_NUM, &fltnum))
    {
        SVP_RETURN(NSK_STATUS_FAILED);
    }
    else
    {
        for( flt_id = 0 ; flt_id < fltnum ; flt_id ++ )
        {
            pid = 0 ;
            if (E_TSP_OK != MDrv_TSP_FLT_GetPID(flt_id, &pid))
            {
                SVP_RETURN(NSK_INVALID_PID);
            }

            if ( pid == es_pid)
            {
                break ;
            }
        }
        if (flt_id == fltnum )
        {
            SVP_RETURN(NSK_INVALID_PID);
        }
    }
#endif

    svp_cm[cm_channel_handle].es[j].alloc = TRUE;
    svp_cm[cm_channel_handle].es[j].pid = (MS_U32)es_pid;
    svp_cm[cm_channel_handle].es[j].flt_id = flt_id;

    MDrv_DSCMB_FltConnectPid(svp_cm[cm_channel_handle].dsc_id, (MS_U32)es_pid);

    SVP_RETURN(NSK_STATUS_OK);
}


unsigned short SVPHDI_RemoveEsPidCmChannel(unsigned int     cm_channel_handle,
                                           unsigned short   es_pid)
{
    int                 j;

    SVP_ENTRY();
    if ((cm_channel_handle >= SVP_CM_CHANNEL_NUM) || (svp_cm[cm_channel_handle].alloc != TRUE))
    {
        SVP_RETURN(NSK_INVALID_CM_CHANNEL_HANDLE);
    }
    for (j = 0; j < SVP_CM_ESPID_NUM; j++)
    {
        if ( (svp_cm[cm_channel_handle].es[j].alloc == TRUE)   &&
             (svp_cm[cm_channel_handle].es[j].pid == es_pid)  )
        {
            if (_SVP_RemoveEsPidCmChannel(cm_channel_handle, j))
            {
                SVP_RETURN(NSK_STATUS_FAILED);
            }
        }
    }
    SVP_RETURN(NSK_STATUS_OK);
}


unsigned short SVPHDI_EnableCmChannel(unsigned int cm_channel_handle)
{
    int                 j;

    SVP_ENTRY();
    if ((cm_channel_handle >= SVP_CM_CHANNEL_NUM) || (svp_cm[cm_channel_handle].alloc != TRUE))
    {
        SVP_RETURN(NSK_INVALID_CM_CHANNEL_HANDLE);
    }

    for (j = 0; j < SVP_CM_ESPID_NUM; j++)
    {
#if 1 //temp
        if ((svp_cm[cm_channel_handle].es[j].alloc == TRUE) && (0x1FFFF != svp_cm[cm_channel_handle].es[j].pid))
        {
            //if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(svp_cm[cm_channel_handle].es[j].flt_id))
            if (E_TSP_OK != MDrv_TSP_FLT_Enable(svp_cm[cm_channel_handle].es[j].flt_id, TRUE))
            {
                SVP_RETURN(NSK_INVALID_CM_CHANNEL_HANDLE);
            }
        }
#else
//FIXME: Please no more DMX/TSP in SVP code
        if (svp_cm[cm_channel_handle].es[j].alloc == TRUE)
        {
            _SVP_EnableEsPidCmChannel(cm_channel_handle, j);
        }
// -- jerry
#endif
    }

//TODO: enable CmChannel

    SVP_RETURN(NSK_STATUS_OK);
}


unsigned short SVPHDI_DisableCmChannel(unsigned int cm_channel_handle)
{
    SVP_ENTRY();
    if ((cm_channel_handle >= SVP_CM_CHANNEL_NUM) || (svp_cm[cm_channel_handle].alloc != TRUE))
    {
        SVP_RETURN(NSK_INVALID_CM_CHANNEL_HANDLE);
    }

    _SVP_DisableCmChannel(cm_channel_handle);

    SVP_RETURN(NSK_STATUS_OK);
}


unsigned short SVPHDI_WriteKey(unsigned long    instance,
                               unsigned int     cm_channel_handle,
                               unsigned char    scb,
                               unsigned int     scrambling_settings_len,
                               unsigned char*   scrambling_settings)
{
    SVP_ENTRY();

    _SVP_WriteKey_Parse(cm_channel_handle, scb, scrambling_settings_len, scrambling_settings);
    //TODO WriteKTE

    SVP_RETURN(NSK_STATUS_OK);
}


unsigned short SVPHDI_GetContentModuleProperties(unsigned int   cm_prop_length,
                                                 unsigned int*  actual_cm_prop_len,
                                                 unsigned char* cm_properties)
{
    SVP_ENTRY();

    *actual_cm_prop_len = sizeof(svp_prop); // will the size more than sepcified coz alignment?

    if (cm_prop_length != 0)
    {
        memcpy(cm_properties, &svp_prop, MIN(cm_prop_length, sizeof(svp_prop)));
    }

    if (cm_prop_length < sizeof(svp_prop))
    {
        SVP_RETURN(NSK_INSUFFICIENT_BUFFER);
    }

    SVP_RETURN(NSK_STATUS_OK);
}


/////------ no support now ------
unsigned short SVPHDI_SetSecondaryVideoPid
(
    unsigned int cm_channel_handle,
    unsigned short primary_pid,
    unsigned short secondary_pid
)
{
    return NSK_STATUS_OK;
}



unsigned short SVPHDI_ReleaseSecondaryVideoPid
(
    unsigned int cm_channel_handle,
    unsigned short secondary_pid
)
{
    return NSK_STATUS_OK;
}



unsigned short SVPHDI_StartPacketInjection
(
    unsigned long xconn,
    unsigned char* transport_packet,
    unsigned int injection_type,
    unsigned int injection_frequency
)
{
    return NSK_STATUS_OK;
}


unsigned short SVPHDI_InjectSinglePacket
(
    unsigned long xconn,
    unsigned char* transport_packet
)
{
    return NSK_STATUS_OK;
}



unsigned short SVPHDI_ContinousReplacePacket
(
    unsigned long xconn,
    unsigned short replace_pid,
    unsigned char* transport_packet
)
{
    return NSK_STATUS_OK;
}



unsigned short SVPHDI_TerminatePacketWrite
(
    unsigned long xconn,
    unsigned short pid
)
{
    return NSK_STATUS_OK;
}


unsigned short HDIHAL_WriteDescrambler
(
    unsigned long xconn,
    unsigned short pid,
    unsigned char scb,
    unsigned char flags
)
{
    //---- no support this now ----
    return NSK_STATUS_OK;
}


NDS_Result NDS_NSK_Init(MS_U32 nds_id)
{
    int                 i, j;

    s32NSK_EventId = MsOS_CreateEventGroup("NDS_NSK_Event");
    if (s32NSK_EventId < 0)
    {
        return E_NDS_FAIL;
    }

    s32NSK_TaskId = MsOS_CreateTask((TaskEntry)_NSK_Isr_Task,
                                    (MS_U32)NULL,
                                    E_TASK_PRI_SYS,
                                    TRUE,
                                    u32NSK_Stack,
                                    NSK_TASK_STACK_SIZE,
                                    "NDS_NSK_Task");
    if (s32NSK_TaskId < 0)
    {
        MsOS_DeleteEventGroup(s32NSK_EventId);
        return E_NDS_FAIL;
    }

    for (i = 0; i < SVP_CM_CHANNEL_NUM; i++)
    {
        svp_cm[i].alloc = FALSE;
        svp_cm[i].enable = FALSE;
        for (j = 0; j < SVP_CM_ESPID_NUM; j++)
        {
            svp_cm[i].es[j].alloc = FALSE;
        }
    }

    // Initialize hardware
    NSK_REG_W(REG_NSK_INT, 0x8000000F);//enable nsk int
    MsOS_DisableInterrupt(NDS_NSK_IRQ);
    MsOS_DetachInterrupt(NDS_NSK_IRQ);
    MsOS_AttachInterrupt(NDS_NSK_IRQ, (InterruptCb)_NSK_Isr);
    MsOS_EnableInterrupt(NDS_NSK_IRQ);

#ifndef CDI_CA
    // NDS Library
    HALHDI_InitSVP_HW(0);
#endif

    return E_NDS_OK;
}


NDS_Result NDS_NSK_Exit(MS_U32 nds_id)
{
    MsOS_DetachInterrupt(NDS_NSK_IRQ);
    MsOS_DisableInterrupt(NDS_NSK_IRQ);

    NSK_REG_W(REG_NSK_INT, 0x8000); // enable nsk int

    MsOS_DeleteTask(s32NSK_TaskId);
    MsOS_DeleteEventGroup(s32NSK_EventId);

    return E_NDS_OK;
}
