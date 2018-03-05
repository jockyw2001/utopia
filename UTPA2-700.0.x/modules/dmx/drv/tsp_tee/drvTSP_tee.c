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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015-2020 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSP_tee.c
/// @brief  Transport Stream Processer (TSP) Driver Interface for TEE
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>

#include "asmCPU.h"

// Internal Definition
#include "halTSP_tee.h"
#include "drvTSP_tee.h"
#include "drvMMIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
static MS_U8 u8TSPFirmware[] = {
    #include "fwTSP_tee.dat"
};

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/// Init
/// @return TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Init(void)
{
    MS_VIRT virtBank = 0;
    MS_PHY  u32BankSize = 0;

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_HW))
    {
        printf("MDrv_MMIO_GetBASE fail\n");
        return FALSE;
    }

    //printf("[%s] virtBank %x\n", __FUNCTION__, (int)virtBank);
    HAL_TSP_Tee_SetBank(virtBank);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Copy fwtsp.dat to FW Buffer
/// @param  virtAddr                    \b IN: virtual address of FW buffer
/// @param  phyAddr                    \b IN: Physical address of VQ buffer
/// @param  u32Size                    \b IN: FW buffer size
/// @return TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_CopyFw(MS_VIRT virtAddr, MS_PHY phyAddr, MS_U32 u32Size)
{
    //MS_U8 *pu8 = 0;

    //printf("[%s][%d] virtAddr %x , phyAddr %x, u32Size %x \n", __FUNCTION__, __LINE__, (int)virtAddr, (int)phyAddr, (int)u32Size);

    if(u32Size < sizeof(u8TSPFirmware))
    {
        return FALSE;
    }

    memcpy((void*)virtAddr, (void*)u8TSPFirmware, sizeof(u8TSPFirmware));
    MAsm_CPU_Sync();
    MsOS_FlushMemory();

    #if 0
    pu8 = (MS_U8*)virtAddr;
    printf("[%s][%d] \n", __FUNCTION__, __LINE__);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", pu8[256], pu8[257],pu8[258],pu8[259],pu8[260],pu8[261],pu8[262],pu8[263]);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", pu8[264], pu8[265],pu8[266],pu8[267],pu8[268],pu8[269],pu8[270],pu8[271]);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", u8TSPFirmware[256], u8TSPFirmware[257],u8TSPFirmware[258],u8TSPFirmware[259],u8TSPFirmware[260],u8TSPFirmware[261],u8TSPFirmware[262],u8TSPFirmware[263]);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", u8TSPFirmware[264], u8TSPFirmware[265],u8TSPFirmware[266],u8TSPFirmware[267],u8TSPFirmware[268],u8TSPFirmware[269],u8TSPFirmware[270],u8TSPFirmware[271]);
    #endif

    HAL_TSP_Tee_Set_FWBuf(phyAddr, u32Size);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Set VQ buffer
/// @param  phyAddr                    \b IN: Physical address of VQ buffer
/// @param  u32Size                     \b IN: VQ buffer size
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_VQBuffer(MS_PHY phyAddr, MS_U32 u32Size)
{
    return HAL_TSP_Tee_Set_VQBuf(phyAddr, u32Size);
}

//-------------------------------------------------------------------------------------------------
/// Set PVR buffer
/// @param  u8PvrId                     \b IN: PVR engine ID
/// @param  phyAddr0                   \b IN: Physical address of PVR buffer 0
/// @param  u32Size0                    \b IN: Buffer size of PVR buffer 0
/// @param  phyAddr1                   \b IN: Physical address of PVR buffer 1
/// @param  u32Size1                    \b IN: Buffer size of PVR buffer 1
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_PVRBuffer(MS_U8 u8PvrId, MS_PHY phyAddr0, MS_U32 u32Size0, MS_PHY phyAddr1, MS_U32 u32Size1)
{
    return HAL_TSP_Tee_Set_PvrBuf(u8PvrId, phyAddr0, u32Size0, phyAddr1, u32Size1);
}

//-------------------------------------------------------------------------------------------------
/// Set PVR buffer
/// @param  u8PvrId                      \b IN: PVR engine ID
/// @param  pphyAddr                    \b IN: Pointer to Physical address of PVR buffer 0
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Get_PVRWriteAddr(MS_U8 u8PvrId, MS_PHY *pphyAddr)
{
    return HAL_TSP_Tee_Get_PVRWriteAddr(u8PvrId, pphyAddr);
}

//-------------------------------------------------------------------------------------------------
/// Transfer virtual filein engine ID to physical ID
/// @param  u8VirEngId                      \b IN: Virtual filein engine ID
/// @return  Physical engine ID
//-------------------------------------------------------------------------------------------------
MS_U32  MDrv_TSP_Tee_FileEngId_Vir2Phy(MS_U8 u8VirEngId)
{
    return HAL_TSP_Tee_FilePath2Tsif_Mapping(u8VirEngId);
}

//-------------------------------------------------------------------------------------------------
/// Set Filein buffer
/// @param  u8EngId                     \b IN: Filein engine ID
/// @param  phyAddr                     \b IN: Physical address of Filein buffer
/// @param  u32Size                      \b IN: Buffer size of Filein buffer
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_FileinBuffer(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size)
{
    return HAL_TSP_Tee_Set_FileinBuf(u8EngId, phyAddr, u32Size);
}

//-------------------------------------------------------------------------------------------------
/// Set MMFI buffer
/// @param  u8EngId                     \b IN: Filein engine ID
/// @param  phyAddr                     \b IN: Physical address of Filein buffer
/// @param  u32Size                      \b IN: Buffer size of Filein buffer
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_MMFIBuffer(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size)
{
    return HAL_TSP_Tee_Set_MMFIBuf(u8EngId, phyAddr, u32Size);
}
