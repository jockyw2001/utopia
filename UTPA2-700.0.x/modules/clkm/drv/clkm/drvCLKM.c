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
///////////////////////////////////////////////////////////////////////////////
#define _DRVBYTEDMA_C

////////////////////////////////////////////////////////////////////////////////
/// file drvBDMA.c
/// @author MStar Semiconductor Inc.
/// @brief DRAM byte DMA control driver
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvCLKM.h"
#include "drvclkm_io.h"
//#include "halclkm.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "halCHIP.h"
#include "utopia.h"
#include "drvSEM.h"
#include "MsTypes.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////

static CLKM_DbgLvl _gCLKMDbgLevel = E_CLKM_DBGLVL_ERROR;

#define CLKM_UTOPIA20       (1)

#define ERR_HANDLE (-1)
#define NOT_FOUND (-1)

#define SUCCESS_SET_SRC 0
#define ERR_NOT_FOUND_TABLE -1
#define ERR_NOT_SUPPORT_SRC -2

#define CLK_GATE_ENABLE 1
#define CLK_GATE_DISABLE 0


#define CLKM_DBG_FUNC()               if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(printf("\t====   %s   ====\n", __FUNCTION__));}
#define CLKM_DBG_INFO(x, args...)     if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(printf("[%s]: ", __FUNCTION__); printf(x, ##args));}
#define CLKM_DBG_ERR(x, args...)      if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(printf("[%s]: ", __FUNCTION__); printf(x, ##args);)}
#define CLKM_DBG_WARN(x, args...)     if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(printf("[%s]: ", __FUNCTION__); printf(x, ##args);)}

#define CLKM_OFFSET(x)       ((x)<<1)

#define CHK_BIT(x, n) ( ((x) & (1<< (n)))!=0 )

#define CLKM_MUTEX_WAIT_TIME        (5000)


static MSIF_Version _clkmVersion = {
    .DDI = { CLKM_DRV_VERSION },
};

static CLKM_Info _gsInfo;

#define CLKM_MODULE_KERNAL_NAME       "/dev/clkm"
      
MS_BOOL Drv_Clkm_Init(void)
{

    MS_PHY u32BaseSize = 0;

     if (_gsInfo.bInit)
     { 
         return _gsInfo.bInit;
     }

      _gsInfo.s32Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex CLKM", MSOS_PROCESS_SHARED);
      MS_ASSERT(_gsInfo.s32Mutex >= 0);

      _gsInfo.s32fd = open(CLKM_MODULE_KERNAL_NAME, O_RDWR);

      if( _gsInfo.s32fd < 0)
      {
           CLKM_DBG_ERR("Fail to open CLKM Kernal Module!\n");
	    _gsInfo.bInit = FALSE;
           return E_CLKM_FAIL;
       }
	
 

    _gsInfo.bInit = TRUE;
    
   
    return true;
}


static MS_BOOL _Drv_CLKM_Is_Init(void)
{
   
    if (!_gsInfo.bInit)
        CLKM_DBG_ERR("Call Drv_Clkm_Init first!\n");
    return _gsInfo.bInit;
}


CLKM_Result Drv_CLKM_Exit()
{

     if (!_gsInfo.bInit)
     {
        CLKM_DBG_INFO("CLKM not init!\n");
        return E_CLKM_FAIL;
     }

      close(_gsInfo.s32fd);
      MsOS_DeleteMutex(_gsInfo.s32Mutex);
       _gsInfo.bInit = FALSE;

      return E_CLKM_OK;


}

CLKM_Result Drv_CLKM_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return E_CLKM_FAIL;
    }

    *ppVersion = &_clkmVersion;

    return E_CLKM_OK;
}




 MS_S32 Drv_Clkm_Get_Handle(char *name)
{

       CLKM_GetHandle_PARAM gethndleparam;
	   	
       MS_S32 s32Handle = ERR_HANDLE;

	int iResult;

	if (!_Drv_CLKM_Is_Init())
       {
            Drv_Clkm_Init();
       }

	 if (FALSE == MsOS_ObtainMutex(_gsInfo.s32Mutex, CLKM_MUTEX_WAIT_TIME))
        {
             CLKM_DBG_ERR("%s ENTRY fails!\n", __FUNCTION__);
             return E_CLKM_FAIL;
        }

	 gethndleparam.s8_Handle_Name = name;

	 if(ioctl(_gsInfo.s32fd, CMD_CLKM_GET_HANDLE, &gethndleparam) < 0)
        {
           CLKM_DBG_ERR("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
           
        }

	  s32Handle = gethndleparam.s32_Handle;

	  MsOS_ReleaseMutex(_gsInfo.s32Mutex);

	 return s32Handle;
}




void Drv_Clkm_Clk_Gate_Disable(MS_S32 s32Handle)
{
      
	
	CLKM_CLK_GATE_DISABLE_PARAM clkgatedisable;

	 if (!_Drv_CLKM_Is_Init())
        {
            Drv_Clkm_Init();
        }


	 if (FALSE == MsOS_ObtainMutex(_gsInfo.s32Mutex, CLKM_MUTEX_WAIT_TIME))
        {
             CLKM_DBG_ERR("%s ENTRY fails!\n", __FUNCTION__);
             return E_CLKM_FAIL;
        }

        clkgatedisable.s32_Handle = s32Handle;

        if(ioctl(_gsInfo.s32fd, CMD_CLKM_CLK_GATE_DISABLE, &clkgatedisable) <0 )
        {
           CLKM_DBG_ERR("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);  
         }

        MsOS_ReleaseMutex(_gsInfo.s32Mutex);
	
}


void Drv_Clkm_Set_Clk_Source(MS_S32 s32Handle,char *clk_src_name)
{
       MS_BOOL ret = TRUE;
       char *s8_Src_Name;
       MS_S32 src_offset;
       s8_Src_Name = clk_src_name;
       MS_U32 u32RegAddr;
       CLKM_SET_CLK_SRC_PARAM setclksrcparam;

       if (!_Drv_CLKM_Is_Init())
       { 
           Drv_Clkm_Init();
       }

       if (FALSE == MsOS_ObtainMutex(_gsInfo.s32Mutex, CLKM_MUTEX_WAIT_TIME))
       {
           CLKM_DBG_ERR("%s ENTRY fails!\n", __FUNCTION__);
           return E_CLKM_FAIL;
       }

       setclksrcparam.s32_Handle = s32Handle;
       setclksrcparam.clk_src_name = clk_src_name;

       if(ioctl(_gsInfo.s32fd, CMD_CLKM_SET_CLK_SOURCE, &setclksrcparam) < 0)
       {
           CLKM_DBG_ERR("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);    
       }
    

	MsOS_ReleaseMutex(_gsInfo.s32Mutex);

	return ret;
}


MS_U16 Drv_Get_Sram_Sd_Info(char* name)
{
     /*  
       MS_U32 u32i;
	sram_sd_table *ptr = sram_sd_param->sram_sd_table_array_ref;
       sram_sd_table *p_sd_table = NULL;
       MS_U32 u32_regaddr ;
	MS_U16 u16Value; 
	MS_S8* s8tmpname = name;

	if (!_Drv_CLKM_Is_Init())
       {
           Drv_Clkm_Init();
        }


	return u16Value;
*/

return 0;

}


