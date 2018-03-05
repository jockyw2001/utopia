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
// File Name: DDC2BI.c
// Description: For Slave I2C functions.
//
// Registers read and write command list:
// DATA WRITE : START + 0x6E + 0x10 + addrh + addrl + data + STOP
// DATA READ  : START + 0x6E + 0x10 + addrh + addrl + START + 0x6F + data +STOP
//
// Functions read and write command list:
// write contrast   : START + 0x6E + 0x20 +  01  +   xxh +   data  + STOP
// read  contrast   : START + 0x6E + 0x20 +  01  +   xxh +   START + 0x6F + (data) + STOP
// write Brightness : START + 0x6E + 0x20 +  02  +   xxh +   data  + STOP
// read  Brightness : START + 0x6E + 0x20 +  02  +   xxh +   START + 0x6F + (data) + STOP
// ON/OFF Panel set : START + 0x6E + 0x20 +  10  +   xxh +   0:OFF 1:ON  + STOP
// ON/OFF Panel Rd  : START + 0x6E + 0x20 +  10  +   xxh +   START + 0x6F + (data) + STOP
//
////////////////////////////////////////////////////////////////////////////////


#define _DRVDDC2BI_C_

#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "ddc2bi_hwreg_utility2.h"
#include "drvMMIO.h"
#include "drvDDC2BI.h"
#include "mhal_DDC2BI.h"
#include "ULog.h"



MS_VIRT DDC2BI_RIU_BASE;

// functions in this file are called by MDrv_DDC2BI_IIC() which
// is called from EX1_ISR, and should be register bank independent

#define DDC_MSG(x) x

#define UNUSED( var )       ((void)(var))

#define TAG_DDC2BI "DDC2BI"

static const MS_U8 nullMsg1[3] =
{
    0x6E, 0x80, 0xBE
};

static MSIF_Version _drv_DDC2BI_version = {
    .DDI = { DDC2BI_DRV_VERSION },
};

static MS_DDC2BI_Status _DDC2BI_Status = {.bIsInitialized = FALSE, .bIsRunning = FALSE,};
static MS_DDC2BI_INFO   _DDC2BI_Info   = {};
MS_U16 _DDC2BI_DbgSwitch = FALSE;

MS_U8 DDCBuffer[MaxBufLen];
MS_U8 txStatus;
MS_U8 const *txBufferPtr;
MS_U8 txLength;
MS_U8 rxStatus;
MS_U8 rxLength;
MS_U8 rxChkSum;
MS_U8 rxIndex;

MS_U16 DDClength;
MS_U8 DDC_StatusFlag;

MS_U8 g_ucCI_Port;
MS_U8 g_ucReceiveCounter;

#define RxBusyFlagBit               0x01UL
#define NMVAddrFlagBit              0x02UL

#define Set_RxBusyFlag()            (DDC_StatusFlag |= RxBusyFlagBit)
#define Clr_RxBusyFlag()            (DDC_StatusFlag &= ~RxBusyFlagBit)
#define Get_RxBusyFlag()            (DDC_StatusFlag & RxBusyFlagBit)

#define Set_DDC_NMVAddrFlag()       (DDC_StatusFlag |= NMVAddrFlagBit)
#define Clr_DDC_NMVAddrFlag()       (DDC_StatusFlag &= ~NMVAddrFlagBit)
#define Get_DDC_NMVAddrFlag()       (DDC_StatusFlag & NMVAddrFlagBit)

//=======================================================================
//debug message about DDCCI
//extern void puthexUART1 (MS_U8 x);
//extern void putcharUART1 (char x);

//=======================================================================
MS_U8 drvDDC2BI_AlignControl_NULL(MS_U8 *pDDCBuffData)
{
    MS_U8 DDCTemp;
    DDCTemp = *pDDCBuffData;
    UNUSED(DDCTemp);
    return 0;
}

void _MDrv_DDC2BI_init_riu_base(void)
{
    MS_VIRT _PMRIUBaseAddress = 0;
    MS_PHY u32PMBankSize = 0;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &_PMRIUBaseAddress, &u32PMBankSize, MS_MODULE_PM ) != TRUE)
    {
        ULOGE(TAG_DDC2BI, "DDC2Bi GetBase failed\n");
        return;
    }
    else
    {
    }

    mhal_DDC2BI_init_riu_base(_PMRIUBaseAddress, _PMRIUBaseAddress);
}


static CALLBACK_AlignControl AlignControlCallback = NULL;
static CALLBACK_AlignControl StandardAlignControlCallback = NULL;
static CALLBACK_AlignControl CustomerAlignControlCallback = NULL;


void MDrv_DDC2BI_Set_Callback(CALLBACK_AlignControl align_control)
{
    AlignControlCallback = align_control;
}


void MDrv_DDC2BI_Set_StandardCallBack(CALLBACK_AlignControl align_control)
{
    StandardAlignControlCallback = align_control;
}


void MDrv_DDC2BI_Set_CustomerCallBack(CALLBACK_AlignControl align_control)
{
    CustomerAlignControlCallback = align_control;
}

void MDrv_DDC2BI_InitRx(void)
{

//    putcharUART1('I');
    rxStatus = DDC2B_CLEAR; // clear the rxState
    rxIndex = 0;            // the current buffer for a new message
    g_ucReceiveCounter = 0;

}

static MS_U8 MDrv_DDC2BI_Get_INT_FLAG(void)
{

    return MDrv_ReadByte(DDC2BI_ADC_INT_FLAG);

}

static void MDrv_DDC2BI_Set_INT_FLAG(MS_U32 u32RegAddr, MS_U8 u8Flag)
{
    MS_U16 u16WaitingCount = 0 ;
    // FIXME: if IP has write one clear is better
    // DDC runs at XTAL clock, but MCU is 144MHz or higher.
    // here we use function call to get some delay between
    // write to the same register twice.
    // (u32RegAddr is variable, real function will be called)
    MDrv_WriteByte(u32RegAddr, u8Flag);

    for ( u16WaitingCount = 0 ; u16WaitingCount < DDC_WAITING_LOOPSCOUNT ; u16WaitingCount++)
        asm("") ;

    MDrv_WriteByte(u32RegAddr, 0x00);

}

static void MDrv_DDC2BI_Set_INT_Mask(MS_U8 u8Mask)
{

    MDrv_WriteByte(DDC2BI_ADC_INT_MASK, u8Mask);

}

static void MDrv_DDC2BI_Write_Buf(MS_U8 u8Data)
{

    MDrv_WriteByte(DDC2BI_ADC_RB_WP, u8Data);

}

static MS_U8 MDrv_DDC2BI_Read_Buf(void)
{

    return MDrv_ReadByte(DDC2BI_ADC_WB_RP);

}

void MDrv_DDC2BI_Receive_ISR(void)
{

    if(g_ucReceiveCounter)
    {
        if((--g_ucReceiveCounter)==0)
        {
            MDrv_DDC2BI_InitRx();
        }
    }

}

MS_U8 MDrv_DDC2BI_ComputeChecksum(MS_U8 count)
{

    MS_U8 checksum;
    MS_U8 i = 0;

    if (Get_DDC_NMVAddrFlag())
        checksum = INITNMVTxCK;
    else
        checksum = INITTxCK;

    for (; count > 0; count--)
        checksum ^= DDCBuffer[i++];

    return checksum;

}

void MDrv_DDC2BI_GetTxBuffer(void)
{

    MS_U8  i;
    for(i = MAX_TXWAIT_DELAY; i > 0; i--)
    {
        if (txStatus == DDC2B_MSGON)
            MsOS_DelayTask(1);
    }
    txStatus = DDC2B_CLEAR;

}

void MDrv_DDC2BI_MessageReady(void)
{
#if 1
    DDClength = (DDCBuffer[0] & ~DDC2Bi_CONTROL_STATUS_FLAG) + 1;
    DDCBuffer[DDClength] = MDrv_DDC2BI_ComputeChecksum(DDClength); //put checksum
    MDrv_DDC2BI_GetTxBuffer();

    MsOS_DisableInterrupt(E_INT_IRQ_DDC2BI);
    txBufferPtr = &DDCBuffer[0];
    txLength = DDClength + 1;
    MsOS_EnableInterrupt(E_INT_IRQ_DDC2BI);

#else
    MDrv_DDC2BI_GetTxBuffer();

    mhal_interrupt_lock();

    txBufferPtr = &DDCBuffer[0];
    txLength = 127;//test

    mhal_interrupt_unlock();
#endif
}

void MDrv_DDC2BI_ParseCommand(void)
{

    MS_U8 length; //,command;

    length = DDCBuffer[LENGTH] & 0x7F;

    if ( rxIndex <= 0 || rxIndex >= MaxBufLen )
    {
		rxIndex = 0;
		return;
    }

    if(length <= 0)
    {
        DDC_MSG(ULOGD(TAG_DDC2BI, "length return \n"));

        MsOS_DisableInterrupt(E_INT_IRQ_DDC2BI);
        txBufferPtr = &nullMsg1[1];
        txLength = sizeof (nullMsg1);
        MsOS_EnableInterrupt(E_INT_IRQ_DDC2BI);

        MDrv_DDC2BI_Write_Buf(DDC2B_DEST_ADDRESS);    //0x6E
        return;
    }

    length = (*AlignControlCallback)(DDCBuffer);

    if (length <= 0)
    {
        DDCBuffer[0] = 0x80;
    }
    else if (length == 0xFE || length == 0xFD)
    {
        DDCBuffer[0] = 0x80;
        // return;
    }

    MDrv_DDC2BI_MessageReady();

#if 1
    if(Get_DDC_NMVAddrFlag())
    {
        MDrv_DDC2BI_Write_Buf(DDC2B_SRC_ADDRESS);   //0x51
        Clr_DDC_NMVAddrFlag();
    }
    else
    {
        MDrv_DDC2BI_Write_Buf(DDC2B_DEST_ADDRESS);  //0x6E
    }
#else
    MDrv_DDC2BI_Write_Buf(*txBufferPtr);  //send first byte.
    txBufferPtr++;
#endif
}
//--------------------------------------------------
//extern void PIU_UART_putchar(char c);
//extern void PIU_UART_puthex(MS_U8 c);
//----------------------------------------------------
void MDrv_DDC2BI_CommandHandler(void)
{
    //#ifdef DDC_TODO
    if (rxStatus == DDC2B_COMPLETED)
    {
        DDC_MSG(ULOGD(TAG_DDC2BI, "DDC2B_COMPLETED \n"));
        MDrv_DDC2BI_Set_INT_Mask(0x7f);//(0x00); // disable all interrypt

        MDrv_DDC2BI_ParseCommand();
        Clr_RxBusyFlag();
        MDrv_DDC2BI_InitRx();

        MDrv_DDC2BI_Set_INT_Mask(0x67);//(0x3E); // enable the write and read interrupt
    }
   // #endif
}

static void MDrv_DDC2BI_IIC(InterruptNum eIntNum)
{
    MS_U8 rxByte, intFlag;

    UNUSED(eIntNum);

    intFlag = MDrv_DDC2BI_Get_INT_FLAG();
    //PIU_UART_putchar('T');
    // puthexUART1(intFlag);
    if(intFlag == 0)
        return;

    switch (intFlag & (TXBI | RCBI ))
    {
        case TXBI:
            //PIU_UART_putchar('T');
            MDrv_DDC2BI_InitRx();

            //PIU_UART_putchar('+');
            //PIU_UART_puthex (txLength);
            txStatus = DDC2B_MSGON;
            // no characters left
            if (!(txLength))
            {
                // clear the transmit status
                txStatus = DDC2B_CLEAR;
                MDrv_DDC2BI_Write_Buf(0x00);
                // EX0=1;
                //return ;
            }
            else
            {
                // send out the current byte
                MDrv_DDC2BI_Write_Buf(*txBufferPtr++);
                txLength--;
             }
            // PIU_UART_putchar('\n');PIU_UART_putchar('\r');
            // intFlag &= (~TXBI); //clear interrupt flag ,for saturn write 1 to clear interrupt
        break;
        case RCBI:
        {
            txLength=0;
            txStatus = DDC2B_CLEAR;
            // read the received byte

            rxByte = MDrv_DDC2BI_Read_Buf();
            //PIU_UART_putchar('R');
            //PIU_UART_putchar('-');
            //PIU_UART_puthex (rxByte);

            //puthexUART1 (rxStatus);
            // depending of the message status
            switch (rxStatus)
            {
                // in case there is nothing received yet
                case DDC2B_CLEAR:

                    if (rxByte == DDC2B_SRC_ADDRESS)
                    {
                        //PIU_UART_putchar('A');
                        rxStatus++;// = DDC2B_SRCADDRESS;
                        rxChkSum = INITRxCK;
                        g_ucReceiveCounter = 200; //ms

                        if(StandardAlignControlCallback == NULL)
                            MDrv_DDC2BI_Set_Callback(drvDDC2BI_AlignControl_NULL);
                        else
                            MDrv_DDC2BI_Set_Callback(StandardAlignControlCallback);
                    }
                    else if (rxByte == DDC2B_SRC_ADDRESS_CUSTOMER)
                    {
                        rxStatus++;// = DDC2B_SRCADDRESS;
                        rxChkSum = (DDC2B_SRC_ADDRESS_CUSTOMER ^ DDC2B_DEST_ADDRESS);
                        g_ucReceiveCounter = 200; //ms
                        if(CustomerAlignControlCallback == NULL)
                            MDrv_DDC2BI_Set_Callback(drvDDC2BI_AlignControl_NULL);
                        else
                            MDrv_DDC2BI_Set_Callback(CustomerAlignControlCallback);
                    }
                    else
                    {
                        MDrv_DDC2BI_InitRx();
                    }
                    break;
                case DDC2B_SRCADDRESS:
                    // get the length
                    rxLength = rxByte & ~DDC2Bi_CONTROL_STATUS_FLAG;    //clear first bit 0x84 -> 0x04

                    // put the received byte in DDCBuffer
                    DDCBuffer[rxIndex++] = rxByte; //DDCBuffer[0] = length
                    rxChkSum ^= rxByte;

                    // set the receive body state
                    rxStatus++; //= DDC2B_COMMAND;

                    //if it is a NULL message
                    if (rxLength == 0 || rxLength >= MaxBufLen)
                    {
                        // wait for CK
                        rxStatus = DDC2B_COMPLETED;
                        Set_RxBusyFlag();
                    }
                break;
                // get the command
                case DDC2B_COMMAND:
                    // save the commandbyte
                    rxStatus++; //= DDC2B_RECBODY;

                // get the message body
                case DDC2B_RECBODY:
                    //PIU_UART_putchar('-');
                    DDCBuffer[rxIndex++] = rxByte;  //DDCBuffer[1] = Command

                    rxChkSum ^= rxByte ;
                    rxLength--;

                    // the last byte in the message body
                    if (rxLength == 0 || rxLength >= MaxBufLen)
                    {
                    rxStatus++; //= DDC2B_WAITFORCK;
                    }
                break;
                // ...here we are waiting for checksum...
                case DDC2B_WAITFORCK:
                   //PIU_UART_puthex(rxChkSum);
                    if (rxChkSum == rxByte) // if checksum match
                    {
                        //PIU_UART_putchar('Y');
                        rxStatus = DDC2B_COMPLETED;
                        Set_RxBusyFlag();
                    }
                    else // else checksum error
                    {
                        //PIU_UART_putchar('N');
                        // if checksum error re-initialize the receive buffer
                        MDrv_DDC2BI_InitRx();
                    }
                    break;

                // error condition
                default:
                    // clear the rxState and the current buffer for a new message
                    if (!Get_RxBusyFlag())
                        MDrv_DDC2BI_InitRx();
                    break;
            } // switch (rxStatus)
            //PIU_UART_putchar('\n');PIU_UART_putchar('\r');

            // intFlag &= (~RCBI); // clear interrupt flag ,for saturn write 1 to clear interrupt
        }
        break;
    }

    intFlag &= 0xBE;
    MDrv_DDC2BI_Set_INT_FLAG(DDC2BI_ADC_INT_CLR, intFlag);
    MsOS_EnableInterrupt(E_INT_IRQ_DDC2BI);
}

void MDrv_DDC2BI_Init(void)
{
    _MDrv_DDC2BI_init_riu_base();

    rxLength = 0;
    rxChkSum = 0;
    rxIndex = 0;
    txLength = 0;

    DDC_StatusFlag = 0;         // initialize various flags and data items
    MDrv_DDC2BI_InitRx();
    txStatus = DDC2B_CLEAR;     // initialize transmit status

    // MDrv_WriteByte(DDC2BI_ADC_ID, 0xB7);          //Set SI2C slave address 0x6E , bit7:Enable SI2C
    MDrv_WriteByte(DDC2BI_ADC_ID, 0xB7);

    //ADC
    MDrv_DDC2BI_Set_INT_FLAG(DDC2BI_ADC_INT_CLR, 0x7F); //Clear ADC all interrupt flag

    //DVI0
    MDrv_DDC2BI_Set_INT_FLAG(DDC2BI_DVI0_INT_CLR, 0x7F); //Clear ADC all interrupt flag

    //DVI1
    MDrv_DDC2BI_Set_INT_FLAG(DDC2BI_DVI1_INT_CLR, 0x7F); //Clear ADC all interrupt flag

    //DVI2
    MDrv_DDC2BI_Set_INT_FLAG(DDC2BI_DVI2_INT_CLR, 0x7F); //Clear ADC all interrupt flag

    //DVI3
    MDrv_DDC2BI_Set_INT_FLAG(DDC2BI_DVI3_INT_CLR, 0x7F); //Clear ADC all interrupt flag


    //MDrv_WriteByte(DDC2BI_CTRL, 0x00);
    MDrv_WriteByte(DDC2BI_CTRL, 0x00);

    //Stop/R/W/RW/Word  : enable
    //Start/ID          : disable
    //MDrv_WriteByte(DDC2BI_ADC_INT_MASK, 0x3E);     //Enable SI2C interrup
    MDrv_WriteByte(DDC2BI_ADC_INT_MASK, 0x67);  // enable read and write status interrupt

    // DVI_0
    //MDrv_WriteByte(DDC2BI_DVI0_ID, 0xb7);
    // DVI_1
    //MDrv_WriteByte(DDC2BI_DVI1_ID, 0xb7);
    // DVI_2
    //MDrv_WriteByte(DDC2BI_DVI2_ID, 0xb7);
    // DVI_3
    //MDrv_WriteByte(DDC2BI_DVI3_ID, 0xb7);

    MDrv_WriteByte(DDC2BI_DVI0_INT_MASK, 0x67);  // enable read and write status interrupt
    MDrv_WriteByte(DDC2BI_DVI1_INT_MASK, 0x67);  // enable read and write status interrupt
    MDrv_WriteByte(DDC2BI_DVI2_INT_MASK, 0x67);  // enable read and write status interrupt
    MDrv_WriteByte(DDC2BI_DVI3_INT_MASK, 0x67);  // enable read and write status interrupt


    //enable interrupt mask
    MsOS_AttachInterrupt(E_INT_IRQ_DDC2BI, MDrv_DDC2BI_IIC);
    MsOS_EnableInterrupt(E_INT_IRQ_DDC2BI);

    _DDC2BI_Status.bIsInitialized = TRUE;
    _DDC2BI_Status.bIsRunning     = TRUE;
}

MS_BOOL MDrv_DDC2BI_Exit(void)
{
    // Mask all interrupt
    MDrv_WriteByte(DDC2BI_ADC_INT_MASK, 0xFF);
    MDrv_WriteByte(DDC2BI_DVI0_INT_MASK, 0xFF);
    MDrv_WriteByte(DDC2BI_DVI1_INT_MASK, 0xFF);
    MDrv_WriteByte(DDC2BI_DVI2_INT_MASK, 0xFF);
    MDrv_WriteByte(DDC2BI_DVI3_INT_MASK, 0xFF);

    MsOS_DetachInterrupt(E_INT_IRQ_DDC2BI);
    return TRUE;
}
E_DDC2BI_ReturnValue MDrv_DDC2BI_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_DDC2BI_RET_FAIL;
    }

    *ppVersion = &_drv_DDC2BI_version;
    return E_DDC2BI_RET_OK;
}

E_DDC2BI_ReturnValue  MDrv_DDC2BI_GetInfo(MS_DDC2BI_INFO *pInfo)
{
    memcpy((void*)pInfo, (void*)&_DDC2BI_Info, sizeof(MS_DDC2BI_INFO));
    return E_DDC2BI_RET_OK;
}

E_DDC2BI_ReturnValue  MDrv_DDC2BI_GetStatus(MS_DDC2BI_Status *pStatus)
{
    memcpy((void*)pStatus, (void*)&_DDC2BI_Status, sizeof(MS_DDC2BI_Status));
    return E_DDC2BI_RET_OK;
}

E_DDC2BI_ReturnValue  MDrv_DDC2BI_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    _DDC2BI_DbgSwitch = u16DbgSwitch;
   UNUSED(_DDC2BI_DbgSwitch);
    return E_DDC2BI_RET_OK;
}


