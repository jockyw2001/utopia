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
// (“MStar Confidential Information”) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
/// @file INTERN_DVBT.c
/// @brief INTERN_DVBT DVBT
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvMMIO.h"
#include "drvDMD_common.h"
#include "drvDMD_VD_MBX.h"
#include "halDMD_INTERN_common.h"
#include "ULog.h"

#if defined (__aeon__)          // Non-OS
    #define BASEADDR_RIU 0xA0000000UL
//#elif ( OS_TYPE == linux )    // Linux
//    #define RIU_BASE u32RegOSBase    // MDrv_MIOMap_GetBASE(u32RegOSBase, puSize, MAP_NONPM_BANK)
#else                           // ecos
    #define BASEADDR_RIU 0xBF800000UL
#endif

#define RIU_MACRO_START     do {
#define RIU_MACRO_END       } while (0)

// Address bus of RIU is 16 bits.
#define RIU_READ_BYTE(addr)         ( READ_BYTE( _hal_DMD.virtDMDBaseAddr + (addr) ) )
#define RIU_READ_2BYTE(addr)        ( READ_WORD( _hal_DMD.virtDMDBaseAddr + (addr) ) )
#define RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( _hal_DMD.virtDMDBaseAddr + (addr), val) }
#define RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( _hal_DMD.virtDMDBaseAddr + (addr), val) }

//=============================================================
// Standard Form

#define RIU_ReadByte( u32Reg )   RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define RIU_Read2Byte( u32Reg )    (RIU_READ_2BYTE((u32Reg)<<1))

#define RIU_ReadRegBit( u32Reg, u8Mask )   (RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define RIU_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    RIU_MACRO_END

#define RIU_WriteByte( u32Reg, u8Val )                                                 \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);   \
    RIU_MACRO_END

#define RIU_Write2Byte( u32Reg, u16Val )                                               \
    RIU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                               \
        RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                  \
        RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        RIU_WRITE_2BYTE( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    RIU_MACRO_END

#define RIU_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END


typedef struct
{
    MS_VIRT  virtDMDBaseAddr;
    MS_BOOL bBaseAddrInitialized;
} hal_DMD_t;

static hal_DMD_t _hal_DMD = // TODO: review, it would be init in Config()
{
    .virtDMDBaseAddr = BASEADDR_RIU,
    .bBaseAddrInitialized = 0,
};

extern s_I2C_Interface_func sI2cInterfaceFunc;    

MS_BOOL HAL_DMD_RegInit (void)
{
    MS_VIRT virtNonPMBank; 
    MS_PHY phyNonPMBankSize;


    ULOGD("DEMOD","bryan check DMD init!!\n");
    if (!MDrv_MMIO_GetBASE( &virtNonPMBank, &phyNonPMBankSize, MS_MODULE_PM))
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","HAL_DMD_RegInit failure to get MS_MODULE_PM\n");
        #endif
        _hal_DMD.virtDMDBaseAddr = BASEADDR_RIU; // TODO what to do if failed??
        _hal_DMD.bBaseAddrInitialized = 0;
        return FALSE;
    }

    //HAL_ParFlash_Config(u32NonPMBank);
    _hal_DMD.virtDMDBaseAddr=virtNonPMBank;
    _hal_DMD.bBaseAddrInitialized = 1;
    return TRUE;
}

MS_U8 HAL_DMD_RIU_ReadByte(MS_U32 u32Addr)
{
    if (_hal_DMD.bBaseAddrInitialized)
    {
        return RIU_ReadByte(u32Addr);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","%s base address un-initialized\n", __FUNCTION__);
        #endif
    }
    return 0;
}

MS_U8 HAL_DMD_RIU_ReadRegBit(MS_U32 u32Addr, MS_U8 u8Mask)
{
    if (_hal_DMD.bBaseAddrInitialized)
    {
        return RIU_ReadRegBit(u32Addr, u8Mask);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","%s base address un-initialized\n", __FUNCTION__);
        #endif
    }
    return 0;
}
MS_U8 HAL_DMD_IIC_ReadByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data)
{

    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};
    
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;

    u8MsbData[0] = 0x35;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 5, u8MsbData);
    bRet &= sI2cInterfaceFunc.I2C_ReadBytes(u16SlaveAddr, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}
MS_U16 HAL_DMD_RIU_Read2Byte(MS_U32 u32Addr)
{
    if (_hal_DMD.bBaseAddrInitialized)
    {
        return RIU_Read2Byte(u32Addr);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","%s base address un-initialized\n", __FUNCTION__);
        #endif
    }
    return 0;
}

MS_U8 HAL_DMD_IIC_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};
    
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, u8MsbData);
    u8MsbData[0] = 0x10;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 6, u8MsbData);
    u8MsbData[0] = 0x34;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}
MS_U8 HAL_DMD_IIC_WriteBytes(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *u8Data, MS_U8 u8Len)
{
    MS_BOOL bRet=TRUE;
    MS_U16 index;
    MS_U8 Data[0x80+5];

    Data[0] = 0x10;
    Data[1] = 0x00;
    Data[2] = 0x00;
    Data[3] = (u32Addr >> 8) &0xff;
    Data[4] = u32Addr &0xff;

    for(index = 0; index < u8Len ; index++)
    {
         Data[5+index] = u8Data[index];
    }

    Data[0] = 0x35;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, Data);
    Data[0] = 0x10;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, Data);
    sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, (5 + u8Len), Data);
    Data[0] = 0x34;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, Data);

    return bRet;    
}

MS_BOOL HAL_DMD_I2C_Channel_Set(MS_U16 u16SlaveAddr, MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    //Exit
    Data[0] = 0x34;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0]=(ch_num & 0x01)? 0x36 : 0x45;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    //Init
    Data[0] = 0x53;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 5, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 5);
    Data[0]=(ch_num & 0x04)? 0x80 : 0x81;
    bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    if ((ch_num==4)||(ch_num==5)||(ch_num==1))
        Data[0]=0x82;
    else
        Data[0] = 0x83;
     bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);

    if ((ch_num==4)||(ch_num==5))
        Data[0]=0x85;
    else
        Data[0] = 0x84;

     bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0]=(ch_num & 0x01)? 0x51 : 0x53;
     bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0]=(ch_num & 0x01)? 0x37 : 0x7F;
     bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0] = 0x35;
     bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0] = 0x71;
     bRet &= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
//     MsOS_ReleaseMutex(_s32MutexId);
     return bRet;
}

MS_BOOL HAL_DMD_I2C_Channel_Change(MS_U16 u16SlaveAddr, MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    Data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet&= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, Data);
    Data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet&= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, Data);
    Data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet&= sI2cInterfaceFunc.I2C_WriteBytes(u16SlaveAddr, 0, 0, 1, Data);

    return bRet;
}
void HAL_DMD_RIU_WriteByte(MS_U32 u32Addr, MS_U8 u8Value)
{
    if (_hal_DMD.bBaseAddrInitialized)
    {
        RIU_WriteByte(u32Addr, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","%s base address un-initialized\n", __FUNCTION__);
        #endif
    }
}

void HAL_DMD_RIU_WriteRegBit(MS_U32 u32Addr, MS_BOOL bEnable, MS_U8 u8Mask)
{
    if (_hal_DMD.bBaseAddrInitialized)
    {
        RIU_WriteRegBit(u32Addr, bEnable, u8Mask);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","%s base address un-initialized\n", __FUNCTION__);
        #endif
    }
}

void HAL_DMD_RIU_WriteByteMask(MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask)
{
    if (_hal_DMD.bBaseAddrInitialized)
    {
        RIU_WriteByteMask(u32Addr, u8Value, u8Mask);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","%s base address un-initialized\n", __FUNCTION__);
        #endif
    }
}

void HAL_DMD_RIU_Write2Byte(MS_U32 u32Addr, MS_U16 u16Value)
{
    if (_hal_DMD.bBaseAddrInitialized)
    {
        RIU_Write2Byte(u32Addr, u16Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGD("DEMOD","%s base address un-initialized\n", __FUNCTION__);
        #endif
    }
}

//waiting add
MS_BOOL HAL_DMD_IFAGC_RegRead(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err)
{
	MS_U8   status = true;
	MS_U8   reg_tmp = 0, reg_tmp2 =0, reg_frz = 0;
	// bank 5 0x24 [15:0] reg_agc_gain2_out
  // use only high byte value

  // select IF gain to read
  status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x22, 0x03);
  status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x05, &reg_frz);
  status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x05, reg_frz | 0x80);
  status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x25, &reg_tmp);
  *ifagc_reg = reg_tmp;
  status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x24, &reg_tmp);
  *ifagc_reg_lsb = reg_tmp;
  status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x05, reg_frz);

  #ifdef MS_DEBUG
  ULOGD("DEMOD","SSI_IFAGC_H = 0x%x 0x%x\n", *ifagc_reg,*ifagc_reg_lsb);
  #endif

  *ifagc_err = 0;
  if(*ifagc_reg == 0xff)
  {
    // bank 5 0x04 [15] reg_tdp_lat
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x22, 0x00);                
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x05, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x05, reg_frz | 0x80);

    // bank 5 0x2c [9:0] reg_agc_error
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x25, &reg_tmp);
    // if_agc_err = reg_tmp & 0x03;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x24, &reg_tmp2);
    // if_agc_err = (if_agc_err << 6)|(reg_tmp >> 2);

    if(reg_tmp&0x2)
    {
       *ifagc_err = ((((~reg_tmp)&0x03)<<8)|((~reg_tmp2)&0xff)) + 1;
    }
    else
    {
       *ifagc_err = reg_tmp<<8|reg_tmp2;
    }

    // release latch
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x05, reg_frz);
  }

  
  return status;
}

//waiting mark
#if(0)
MS_BOOL HAL_DMD_GetRFLevel(float *fRFPowerDbmResult, float fRFPowerDbm, MS_U8 u8SarValue,
                                                     DMD_RFAGC_SSI *pRfagcSsi, MS_U16 u16RfagcSsi_Size,
                                                     DMD_IFAGC_SSI *pIfagcSsi_HiRef, MS_U16 u16IfagcSsi_HiRef_Size,
                                                     DMD_IFAGC_SSI *pIfagcSsi_LoRef, MS_U16 u16IfagcSsi_LoRef_Size,
                                                     DMD_IFAGC_ERR *pIfagcErr_LoRef, MS_U16 u16IfagcErr_LoRef_Size,
                                                     DMD_IFAGC_ERR *pIfagcErr_HiRef, MS_U16 u16IfagcErr_HiRef_Size)
{
    DMD_IFAGC_SSI   *ifagc_ssi;
    DMD_IFAGC_ERR   *ifagc_err;    
    float   ch_power_db=0.0f;
    float   ch_power_rf=0.0f;
    float   ch_power_if=0.0f, ch_power_ifa = 0.0f, ch_power_ifb =0.0f;
    float   ch_power_takeover=0.0f;
    MS_U16  if_agc_err = 0;    
    MS_U8   status = true;
    MS_U8   reg_tmp = 0, reg_tmp2 =0, reg_frz = 0,rf_agc_val =0,if_agc_val =0, if_agc_vala =0, if_agc_valb =0, if_agc_val_lsb =0, i;
    MS_U8   ssi_tbl_len = 0, err_tbl_len = 0;    
    
    if ((pIfagcSsi_HiRef != NULL) && (pIfagcSsi_LoRef !=NULL))
    {      
        // get RFAGC level
        if (fRFPowerDbm>=100.0) // unreasonable input value, get RF level from RFAGG
        {
            rf_agc_val = u8SarValue;
                
            ch_power_rf=pRfagcSsi[u16RfagcSsi_Size-1].power_db;
            if (rf_agc_val >=pRfagcSsi[0].sar3_val)
            {
                float   ch_power_rfa = 0, ch_power_rfb =0;
                MS_U8 rf_agc_vala =0, rf_agc_valb =0;
                for(i = 1; i < u16RfagcSsi_Size; i++)
                {
                    if (rf_agc_val < pRfagcSsi[i].sar3_val)
                    {
                        rf_agc_valb = pRfagcSsi[i].sar3_val;
                        ch_power_rfb = pRfagcSsi[i].power_db;
        
                        i--;
                        rf_agc_vala = pRfagcSsi[i].sar3_val;
                        ch_power_rfa=pRfagcSsi[i].power_db;
                        while ((i>1) && (rf_agc_vala==pRfagcSsi[i-1].sar3_val))
                        {
                            ch_power_rfa=pRfagcSsi[i-1].power_db;
                            i--;
                        }
                        ch_power_rf = ch_power_rfa+(ch_power_rfb-ch_power_rfa)*(float)(rf_agc_val-rf_agc_vala)/(rf_agc_valb-rf_agc_vala);
                        break;
                    }
                }
                #ifdef MS_DEBUG
                ULOGD("DEMOD","RF Level from SAR:%f\n", ch_power_rf);
                ULOGD("DEMOD","SSI_RFAGC (SAR-4) = 0x%x\n", rf_agc_val);
                ULOGD("DEMOD","rf prev %f %x\n", ch_power_rfa, rf_agc_vala);
                ULOGD("DEMOD","rf next %f %x\n", ch_power_rfb, rf_agc_valb);                    
                #endif                    
            }
        }
        else
        {
            #ifdef MS_DEBUG
            ULOGD("DEMOD","RF Level from tuner: %f\n",fRFPowerDbm);
            #endif
            ch_power_rf = fRFPowerDbm;
        }          

        // get IFAGC status
        {
            status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x13, &reg_tmp);

            #ifdef MS_DEBUG
            ULOGD("DEMOD","AGC_REF = %d\n", (MS_U16)reg_tmp);
            #endif

            if (reg_tmp > 200)
            {
                ifagc_ssi = pIfagcSsi_HiRef;
                ssi_tbl_len = u16IfagcSsi_HiRef_Size;
                ifagc_err = pIfagcErr_HiRef;
                err_tbl_len = u16IfagcErr_HiRef_Size;
            }
            else
            {
                ifagc_ssi = pIfagcSsi_LoRef;
                ssi_tbl_len = u16IfagcSsi_LoRef_Size;
                ifagc_err = pIfagcErr_LoRef;
                err_tbl_len = u16IfagcErr_LoRef_Size;
            }

            // bank 5 0x24 [15:0] reg_agc_gain2_out
            // use only high byte value

            // select IF gain to read
            status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x16, 0x03);
            status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x03, &reg_frz);
            status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x03, reg_frz | 0x80);
            status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x19, &reg_tmp);
            if_agc_val = reg_tmp;
            status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x18, &reg_tmp);
            if_agc_val_lsb = reg_tmp;    
            status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x03, reg_frz);
            #ifdef MS_DEBUG
            ULOGD("DEMOD","SSI_IFAGC_H = 0x%x 0x%x\n", if_agc_val,if_agc_val_lsb);
            #endif

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

            ///////// IF-AGC Error for Add. Attnuation /////////////
            if(if_agc_val == 0xff)
            {
#if 0
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD )
                status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x22, &reg_tmp);
                status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x22, (reg_tmp&0xf0));
#endif
#endif
                // bank 5 0x04 [15] reg_tdp_lat
                status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x16, 0x00);                
                status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x03, &reg_frz);
                status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x03, reg_frz | 0x80);
#if 0
        //#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD )
                        // bank 5 0x2c [9:0] reg_agc_error
                        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x25, &reg_tmp);
                        // if_agc_err = reg_tmp & 0x03;
                        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x24, &reg_tmp2);
                        // if_agc_err = (if_agc_err << 6)|(reg_tmp >> 2);
        //#else
#endif
                // bank 5 0x2c [9:0] reg_agc_error
                status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x19, &reg_tmp);
                // if_agc_err = reg_tmp & 0x03;
                status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x18, &reg_tmp2);
                // if_agc_err = (if_agc_err << 6)|(reg_tmp >> 2);
        //#endif

                if(reg_tmp&0x2)
                {
                    if_agc_err = ((((~reg_tmp)&0x03)<<8)|((~reg_tmp2)&0xff)) + 1;
                }
                else
                {
                    if_agc_err = reg_tmp<<8|reg_tmp2;
                }

                // release latch
                status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x03, reg_frz);

                for(i = 0; i < err_tbl_len; i++)
                {
                    if ( if_agc_err <= ifagc_err[i].agc_err )        // signed char comparison
                    {
                        ch_power_db += ifagc_err[i].attn_db;
                        break;
                    }
                }
                #ifdef MS_DEBUG
                ULOGD("DEMOD","if_agc_err = 0x%x\n", if_agc_err);
                #endif
                }

                // BY 20110812 temporaily remove ch_power_db += SIGNAL_LEVEL_OFFSET;                
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
    *fRFPowerDbmResult=ch_power_db;
    return status;
}
#endif

//waiting mark
#if(0)
void HAL_DMD_GetNordigSSI(float fPrel, MS_U16 *strength)
{
    if (fPrel<-15.0f)
    {
        *strength = 0;
    }
    else if (fPrel<0.0f)
    {
        *strength = (MS_U16)((2.0f/3.0f)*(fPrel+15.0f));
    }
    else if (fPrel<20.0f)
    {
        *strength = (MS_U16)(4.0f*fPrel+10.0f);
    }
    else if (fPrel<35.0f)
    {
        *strength = (MS_U16)((2.0f/3.0f)*(fPrel-20.0f)+90.0f);
    }
    else
    {
        *strength = 100;
    }

}
#endif
/*
from Steven.Hung
2. 要把T12 TS1 TS bus tristate
    Set Bank CHIPTOP, 0x57[13:11]=3’h0; (reg_ts1config[2:0]=0)
3. 要把T12 IFAGC tristate
    Set Bank CHIPTOP, 0x2[12]=1’h1; (reg_if_agc_pad_oen=1)
*/
void HAL_DMD_TS1_Tristate(MS_BOOL bEnable)
{
    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_DMD_TS1_Tristate %d\n",bEnable);
    #endif
    if (bEnable)
    {
        HAL_DMD_RIU_WriteByteMask(0x101EAF, 0, BMASK(5:3));
    }
    else
    {
        HAL_DMD_RIU_WriteByteMask(0x101EAF, (BIT_(5))|(BIT_(4))|(BIT_(3)), BMASK(5:3));
    }
}

void HAL_DMD_RFAGC_Tristate(MS_BOOL bEnable)
{
    MS_U8 u8RegMuxBackup = 0;

    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_DMD_RFAGC_Tristate %d\n",bEnable);
    #endif
    u8RegMuxBackup = HAL_DMD_RIU_ReadByte(0x101E39);
    HAL_DMD_RIU_WriteByte(0x101E39, 0x00); //mux from DMD MCU to HK
    if (bEnable)
    {
        HAL_DMD_RIU_WriteByteMask(0x11286C, 0, (BIT_(0)));
        
    }
    else
    {
        HAL_DMD_RIU_WriteByteMask(0x11286C, (BIT_(0)), (BIT_(0)));
    }
    HAL_DMD_RIU_WriteByte(0x101E39, u8RegMuxBackup); //mux from HK to DMD MCU
}

void HAL_DMD_IFAGC_Tristate(MS_BOOL bEnable)
{
    MS_U8 u8RegMuxBackup = 0;

    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_DMD_IFAGC_Tristate %d\n",bEnable);
    #endif
    u8RegMuxBackup = HAL_DMD_RIU_ReadByte(0x101E39);
    HAL_DMD_RIU_WriteByte(0x101E39, 0x00); //mux from DMD MCU to HK
    if (bEnable)
    {
        HAL_DMD_RIU_WriteByteMask(0x11286C, 0, (BIT_(4)));
    }
    else
    {
        HAL_DMD_RIU_WriteByteMask(0x11286C, (BIT_(4)), (BIT_(4)));
    }
    HAL_DMD_RIU_WriteByte(0x101E39, u8RegMuxBackup); //mux from HK to DMD MCU
}

void HAL_DMD_IFAGC_TS_Tristate(MS_BOOL bEnable)
{
    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_DMD_IFAGC_TS_Tristate %d\n",bEnable);
    #endif
    HAL_DMD_TS1_Tristate(bEnable);
    HAL_DMD_IFAGC_Tristate(bEnable);
}

#if(0)
MS_BOOL HAL_DMD_TS_GetClockRate(float *fTS_CLK)
{
    // from Raymond
    *fTS_CLK=(HAL_DMD_RIU_ReadRegBit(0x103301, BIT_(0)) ? 288.0 : 348.0)/(2*((HAL_DMD_RIU_ReadByte(0x103300)&BMASK(4:0))+1));
    return TRUE;
}
#endif
static void HAL_DMD_ADC_IMUX_Sel(MS_U8 u8PadSel, MS_BOOL bPGAEnable)
{
        if (u8PadSel==0)
        {
            HAL_DMD_RIU_WriteByteMask(0x112802, 4<<4, BMASK(6:4));                
        }
        else
        {
            if (bPGAEnable) 
            {
                HAL_DMD_RIU_WriteByteMask(0x112802, 1<<4, BMASK(6:4));
            }
            else
            {
                HAL_DMD_RIU_WriteByteMask(0x112802, 2<<4, BMASK(6:4));
            }                
        }
}

static void HAL_DMD_ADC_QMUX_Sel(MS_U8 u8PadSel, MS_BOOL bPGAEnable)
{
        if (u8PadSel==0)
        {
            HAL_DMD_RIU_WriteByteMask(0x112803, 4, BMASK(2:0));                
        }
        else
        {
            if (bPGAEnable) 
            {
                HAL_DMD_RIU_WriteByteMask(0x112803, 1, BMASK(2:0));
            }
            else
            {
                HAL_DMD_RIU_WriteByteMask(0x112803, 2, BMASK(2:0));
            }                
        }
}


static void HAL_DMD_SIF_PGA_Ctl(MS_BOOL bPGAEnable)
{      
    HAL_DMD_RIU_WriteRegBit(0x112840, bPGAEnable, BIT_(5)); // enable SIF PGA
    HAL_DMD_RIU_WriteRegBit(0x112802, bPGAEnable, BIT_(2)); // disable IMUX clamping
}

static void HAL_DMD_VIF_PGA_Ctl(MS_BOOL bPGAEnable)
{      
    HAL_DMD_RIU_WriteRegBit(0x112840, bPGAEnable, BIT_(6)); // enable VIF PGA
    HAL_DMD_RIU_WriteRegBit(0x112802, bPGAEnable, BIT_(3)); // disable IMUX clamping
}

/************************************************************************************************
  Subject:    ADC I/Q Switch (After Init CLKGen)
  Function:   HAL_DMD_ADC_IQ_Switch
  Parmeter:   u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
  Parmeter:   u8PadSel : 0=Normal, 1=analog pad
  Parmeter:   bPGAEnable : 0=disable, 1=enable
  Parmeter:   u8PGAGain : default 5
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL HAL_DMD_ADC_IQ_Switch(MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain)
{
    MS_U8 u8RegMuxBackup = 0;
    u8PGAGain=u8PGAGain;
    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_DMD_ADC_IQ_Switch %d %d %d %d\n",u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain);
    #endif

    u8RegMuxBackup = HAL_DMD_RIU_ReadByte(0x101E39);
    HAL_DMD_RIU_WriteByte(0x101E39, 0x00); //mux from DMD MCU to HK
    #ifdef MS_DEBUG
    ULOGD("DEMOD","before 0x112803 %x\n",HAL_DMD_RIU_ReadByte(0x112803));
    #endif
    switch(u8ADCIQMode)
    {
        case 0://Normal case, I path
        default:	
            HAL_DMD_RIU_WriteRegBit(0x112818, 0, BIT_(0)); // power on I ADC
            HAL_DMD_RIU_WriteRegBit(0x112818, 1, BIT_(1)); // power down Q ADC
            HAL_DMD_RIU_WriteRegBit(0x112803, 0, BIT_(4)); // ADC clock out swap 0:no swap, 1:swap            
            HAL_DMD_RIU_WriteRegBit(0x112803, 0, BIT_(5)); // ADC clock out select 0:I, 1:Q
            HAL_DMD_ADC_IMUX_Sel(u8PadSel, bPGAEnable);
            HAL_DMD_SIF_PGA_Ctl(bPGAEnable);
            HAL_DMD_VIF_PGA_Ctl(FALSE);
            break;
        case 1://VIF, Q path, for internal signal saw
            HAL_DMD_RIU_WriteRegBit(0x112818, 1, BIT_(0)); // power down I ADC
            HAL_DMD_RIU_WriteRegBit(0x112818, 0, BIT_(1)); // power on Q ADC
            HAL_DMD_RIU_WriteRegBit(0x112803, 1, BIT_(4)); // ADC clock out swap 0:no swap, 1:swap       
            HAL_DMD_RIU_WriteRegBit(0x112803, 1, BIT_(5)); // ADC clock out select 0:I, 1:Q
            HAL_DMD_ADC_QMUX_Sel(u8PadSel, bPGAEnable);
            HAL_DMD_SIF_PGA_Ctl(FALSE);
            HAL_DMD_VIF_PGA_Ctl(bPGAEnable);        
            break;
        case 2://both IQ, for ZIF tuner
            break;
    }
    #ifdef MS_DEBUG
    ULOGD("DEMOD","after 0x112803 %x\n",HAL_DMD_RIU_ReadByte(0x112803));
    #endif
    HAL_DMD_RIU_WriteByte(0x101E39, u8RegMuxBackup); //mux from HK to DMD MCU
    return TRUE;
} 

/************************************************************************************************
  Subject:    HAL_DMD_TSO_Clk_Control
  Function:   ts output clock frequency and phase configure
  Parmeter:   u8cmd_array, clock div,           0x01, div (0x00~0x1f), 
                           clock phase inv,     0x02, inv_en (0,1),
                           clock phase tuning,  0x03, phase_tuning_en (0,1), tuning_num (0x00~0x1f)
  Return:     MS_BOOL     
  Remark:
*************************************************************************************************/
MS_BOOL HAL_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array)
{
    MS_U8   u8Temp;

    u8Temp = HAL_DMD_RIU_ReadByte(0x103301);

    if ( (u8Temp&0x01) == 0x00)
    {
        ULOGD("DEMOD","[utopia][halDMD]Error!!!, we shall select clk_dmplldiv3\n");
        return false;
    }
    switch (u8cmd_array[0])
    {
        case 0x01: // clock frequency,div
            {
                MS_U8 u8data = 0;
                u8data = HAL_DMD_RIU_ReadByte(0x103300);
                u8data &= (0xff-0x1f);
                u8data |= (u8cmd_array[1]&0x1f);
                HAL_DMD_RIU_WriteByte(0x103300, u8data);
            }
            break;
        case 0x02: // clock phase inv or not.
            {
                MS_U8 u8data = 0;
                u8data = HAL_DMD_RIU_ReadByte(0x103301);
                u8data &= (0xff-0x02);
                u8data |= ((u8cmd_array[1]&0x01)<<1);
                HAL_DMD_RIU_WriteByte(0x103301, u8data);                
            }
            break;            
        case 0x03:
            {
                MS_U8 u8data = 0;
                
                u8data = HAL_DMD_RIU_ReadByte(0x103301);
                u8data &= (0xff-0x10);
                u8data |= ((u8cmd_array[1]&0x01)<<4);
                HAL_DMD_RIU_WriteByte(0x103301, u8data);

                u8data = HAL_DMD_RIU_ReadByte(0x103300+(0x05<<1)+1);
                u8data &= (0xff-0x1f);
                u8data |= (u8cmd_array[2]&0x1f);
                HAL_DMD_RIU_WriteByte(0x103300+(0x05<<1)+1, u8data);   
            }
            break;
        default:
            ULOGD("DEMOD","[utopia][halDMD]Error!!!, cmd invalid\n");
            break;

    }
#ifdef MS_DEBUG
    ULOGD("DEMOD","0x103300: 0x%x\n",HAL_DMD_RIU_ReadByte(0x103300));
    ULOGD("DEMOD","0x103301: 0x%x\n",HAL_DMD_RIU_ReadByte(0x103301));
    ULOGD("DEMOD","0x10330B: 0x%x\n",HAL_DMD_RIU_ReadByte(0x10330B));    
#endif
    return true;
}

/****************************************************************************
  Subject:    Function providing approx. result of Log10(X)
  Function:   Log10Approx
  Parmeter:   Operand X in float
  Return:     Approx. value of Log10(X) in float
  Remark:      Ouput range from 0.0, 0.3 to 9.6 (input 1 to 2^32)
*****************************************************************************/
/*
#if(0)
#if 1
const float _LogApproxTableX[80] =
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

const float _LogApproxTableY[80] =
{ 0.00, 0.11, 0.23, 0.34, 0.46, 0.57, 0.68, 0.80, 0.91, 1.03, 1.14, 1.25,
  1.37, 1.48, 1.60, 1.71, 1.82, 1.94, 2.05, 2.16, 2.28, 2.39, 2.51, 2.62,
  2.73, 2.85, 2.96, 3.08, 3.19, 3.30, 3.42, 3.53, 3.65, 3.76, 3.87, 3.99,
  4.10, 4.22, 4.33, 4.44, 4.56, 4.67, 4.79, 4.90, 5.01, 5.13, 5.24, 5.36,
  5.47, 5.58, 5.70, 5.81, 5.93, 6.04, 6.15, 6.27, 6.04, 6.15, 6.27, 6.38,
  6.49, 6.61, 6.72, 6.84, 6.95, 7.06, 7.18, 7.29, 7.41, 7.52, 7.63, 7.75,
  7.86, 7.98, 8.09, 8.20, 8.32, 8.43, 8.55, 8.66
};

float Log10Approx(float flt_x)
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
float Log10Approx(float flt_x)
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
*/