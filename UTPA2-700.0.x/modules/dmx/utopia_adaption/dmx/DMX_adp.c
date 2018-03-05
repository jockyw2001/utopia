//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file  DMX_adp.c
/// @brief  Demux adaption API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/compat.h>

#include "apiDMX.h"
#include "apiDMX_tsio.h"
#include "apiDMX_private.h"
#include "DMX_adp.h"

#include "utopia.h"
#include "utopia_adp.h"

extern UADP_STRUCT_POINTER_TABLE spt_MS_NULL[];
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

// MApi_CMD_DMX_TSPInit_GetConfig
UADP_SDT_NAMEn_DEF(DMX_TSPParam, 0);

// MApi_CMD_DMX_Info_Ex
UADP_SDT_NAMEn_DEF(DMX_FLT_INFO_PARAM_Ex, 1);
UADP_SDT_NAMEn_DEF(DMX_Flt_info_Ex, 1);
UADP_SDT_NAMEn_DEF(DMX_Sect_info_Ex, 2);
UADP_SDT_NAMEn_DEF(DMX_Sect_info, 2);
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                                    if(copy_from_user(a,b,c) != 0) { return UTOPIA_STATUS_FAIL; }
#define CPY_to_USER(a,b,c)                                      if(copy_to_user(a,b,c) != 0) { return UTOPIA_STATUS_FAIL; }
#else
#define CPY_FROM_USER                                           memcpy
#define CPY_to_USER                                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                                           compat_ptr(a)
#define IS_CMP_TASK()                                           is_compat_task()
#define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  return UTOPIA_STATUS_FAIL; }
#define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { return UTOPIA_STATUS_FAIL;  }

#else
#define COMPAT_PTR(a)                                           (a)
#define IS_CMP_TASK()                                           (FALSE)
#define CMP_CPY_FROM_USER                                       CPY_FROM_USER
#define CMP_CPY_TO_USER                                         CPY_to_USER
#endif //CONFIG_COMPAT


#define DMX_ADP_MSG(_f, _a...)                                  //printf(_f, ##_a)

#define LEN_U32_X1                                              4UL
#define LEN_U32_X2                                              8UL
#define LEN_U32_X3                                              12UL
#define LEN_U32_X4                                              16UL

#ifdef BIFROST_32BIT_MODE
#define LEN_MSPHY                                               4UL
#else
#define LEN_MSPHY                                               sizeof(MS_PHY)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 DMX_adp_Init(FUtopiaIOctl *pIoctl)
{
    //MApi_CMD_DMX_TSPInit_GetConfig
    UADP_SPT_BGN(&spt_DMX_TSPParam[0], sizeof(DMX_TSPParam));
    UADP_SPT_FIN(&spt_DMX_TSPParam[1]);

    //MApi_CMD_DMX_Info_Ex
    //MApi_CMD_DMX_Open_Ex

    UADP_SPT_BGN(&spt_DMX_Sect_info[0], sizeof(DMX_Sect_info));
    UADP_SDT_KIND(&spt_DMX_Sect_info[1], DMX_Sect_info, UADP_SDT_AT, pNotify, spt_MS_NULL);
    UADP_SDT_KIND(&spt_DMX_Sect_info[2], DMX_Sect_info, UADP_SDT_AT, pType2Notify, spt_MS_NULL);
    UADP_SPT_FIN(&spt_DMX_Sect_info[3]);

    UADP_SPT_BGN(&spt_DMX_Sect_info_Ex[0], sizeof(DMX_Sect_info_Ex));
    //UADP_SDT_KIND(&spt_DMX_Sect_info_Ex[1], DMX_Sect_info_Ex, UADP_SDT_ES, Info, spt_DMX_Sect_info);
    UADP_SDT_KIND(&spt_DMX_Sect_info_Ex[1], DMX_Sect_info_Ex, UADP_SDT_AT, pType2Notify_Ex, spt_MS_NULL);
    UADP_SDT_KIND(&spt_DMX_Sect_info_Ex[2], DMX_Sect_info_Ex, UADP_SDT_AT, pType2NotifyParamEx, spt_MS_NULL);
    UADP_SPT_FIN(&spt_DMX_Sect_info_Ex[3]);

    UADP_SPT_BGN(&spt_DMX_Flt_info_Ex[0], sizeof(DMX_Flt_info_Ex));
    UADP_SDT_KIND(&spt_DMX_Flt_info_Ex[1], DMX_Flt_info_Ex, UADP_SDT_ES, SectInfoEx, spt_DMX_Sect_info_Ex);
    UADP_SPT_FIN(&spt_DMX_Flt_info_Ex[2]);

    UADP_SPT_BGN(&spt_DMX_FLT_INFO_PARAM_Ex[0], sizeof(DMX_FLT_INFO_PARAM_Ex));
    UADP_SDT_KIND(&spt_DMX_FLT_INFO_PARAM_Ex[1], DMX_FLT_INFO_PARAM_Ex, UADP_SDT_P2N, pDmxFltInfo, spt_DMX_Flt_info_Ex);
    UADP_SPT_FIN(&spt_DMX_FLT_INFO_PARAM_Ex[2]);

    //MApi_CMD_DMX_Open_Ex
    *pIoctl= (FUtopiaIOctl)DMX_adp_Ioctl;
    return 0;
}

MS_U32 DMX_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_S8*                          ps8AdpSrc = (MS_S8*)pArgs;

    MS_U32                          u32ii = 0, u32Ret = UTOPIA_STATUS_FAIL, u32data = 1;
    MS_U32                          u32InSize = 0, u32OutSize = 0;

    MS_S8                           buffer_arg[640];
    MS_S8                           buffer_array0[320];
    MS_S8                           buffer_array1[320];
    MS_S8                           buffer_array2[64];
    MS_S8                           buffer_array3[16];
    MS_S8                           buffer_array4[16];
    MS_U32                          u32NextPtr = 0, u32NextPtrOut = 0, u32NextPtr1 = 0, u32NextPtr2 = 0, u32NextPtr3 = 0;
    void                            *pNxtPtr = 0, *pNxtPtrOut = 0, *pNxtPtr1 = 0;

    DMX_CAP_PARAM                   stCapParam;
    PDMX_TSO_OUTCLK_PARAM           pstTsoOutClkDes     = NULL;

    DMX_TSPParam                    *pTspParamDec       = NULL;
    PDMX_WP_PARAM                   pstWPParamDes       = NULL;
    PDMX_CAP_PARAM                  pstCapParamDes      = NULL;
    PDMX_CAP_EX_PARAM               pstCapExParamDes    = NULL;
    PDMX_FLT_INFO_PARAM             pstFltInfoDes       = NULL;
    PDMX_FLT_EVENT_PARAM            pFltEventDes        = NULL;
    PDMX_FLT_SEC_PAT_PARAM          pFltSecPatDes       = NULL;
    PDMX_CMDRUN_PARAM               pstCmdRunDes        = NULL;
    PDMX_PKT_DISCONCNT_PARAM        pstDicConCntDes     = NULL;
    PDMX_PKT_DROPCNT_PARAM          pstDropCntDes       = NULL;
    PDMX_PKT_LOCKCNT_PARAM          pstLockCntDes       = NULL;
    PDMX_PKT_AVCNT_PARAM            pstAVPktCntDes      = NULL;
    PDMX_TSO_LOCSTRID_PARAM         pstTsoLocStrIdDes   = NULL;
    PDMX_FQ_INIT_PARAM              pstFqInitDes        = NULL;
    PDMX_MSTR_SYNCBY_PARAM          pstMstrSynByDes     = NULL;
    PDMX_POWSTATE_PARAM             pstPowerStateDes    = NULL;
    PDMX_PVR_INFO_PARAM             pstPvrInfoDes       = NULL;
    PDMX_FILE_START_PARAM           pstFileStartDes     = NULL;
    PDMX_FILE_INFO_PARAM            pstFileInfoDes      = NULL;
    PDMX_MMFI_START_PARAM           pstMMFIStartDes     = NULL;
    DMX_OutputPad_Cfg               *pstOutPadCfgDes    = NULL;
    PDMX_FLT_COPY_PARAM             pstDmxFltCpyDes     = NULL;
    DMX_TSO_Cfg                     *pstTSOCfg          = NULL;
    DMX_FQ_Cfg                      *pstFqCfg           = NULL;
    PDMX_TSIO_Service_SetDMAoutVC_PARAM         pstTsioDoutDes      = NULL;
    PDMX_TSIO_Service_SetDMAinVC_PARAM          pstTsioDinDes       = NULL;
    PDMX_TSIO_Service_SetLocdecKey_PARAM        pstTsioLocDes       = NULL;
    PDMX_TSIO_Service_PidOpen_PARAM             pstTsioPidDes       = NULL;
    PDMX_TSIO_Service_Alloc_PARAM               pstTsioAlcDes       = NULL;
    PDMX_TSIO_RegisterIntCb_PARAM               pstTsioCbDes        = NULL;
    PDMX_TSIO_Init_PARAM                        pstTsioInitDes      = NULL;
    PDMX_TSIO_CC_PARAM                          pstTsioCcDes        = NULL;
    PDMX_TSIO_GetInfo_PARAM                     pstTsioGetInfoDes   = NULL;
    PDMX_TSIO_Service_DMAout_WriteGet_PARAM     pstTsioDWriteDes    = NULL;

    //printf("[DMX] S %d\n", (int)u32Cmd);

    switch(u32Cmd)
    {
        // General API
        case MApi_CMD_DMX_Init:
        case MApi_CMD_DMX_Resume:
            pTspParamDec = (DMX_TSPParam*)buffer_arg;

            if(IS_CMP_TASK())
            {
                pTspParamDec->phyFWAddr = 0;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->phyFWAddr), ps8AdpSrc, LEN_MSPHY);
                ps8AdpSrc += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->u32FWSize), ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                pTspParamDec->phyVQAddr = 0;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->phyVQAddr), ps8AdpSrc, LEN_MSPHY);
                ps8AdpSrc += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->u32VQSize), ps8AdpSrc, 20);  //U32 x 5
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pTspParamDec->u32Res), LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSPParam));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSPParam));
            }
            break;

        case MApi_CMD_DMX_WProtectEnable:
            pstWPParamDes = (PDMX_WP_PARAM)buffer_arg;

            stCapParam.Quetype = DMX_CAP_TSP_HWWP_SET_NUM;
            stCapParam.pdata = (void*)&u32data;
            u32Ret = UtopiaIoctl(pInstanceTmp,MApi_CMD_DMX_GetCap,(void*)&stCapParam);

            if(IS_CMP_TASK())
            {
                MS_PHY* pphy = (MS_PHY*)buffer_array0;
                MS_PHY* pphy1 = (MS_PHY*)buffer_array1;

                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 1);
                ps8AdpSrc += 1;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr1, ps8AdpSrc, LEN_U32_X1);
                for(u32ii = 0; u32ii < u32data; u32ii++, pphy++, pphy1++)
                {
                    *pphy = 0;  *pphy1 = 0;
                    CMP_CPY_FROM_USER((void*)pphy, u32NextPtr, LEN_MSPHY);
                    CMP_CPY_FROM_USER((void*)pphy1, u32NextPtr1, LEN_MSPHY);
                    u32NextPtr += LEN_MSPHY;
                    u32NextPtr1 += LEN_MSPHY;
                }
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_WP_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstWPParamDes->pu32StartAddr, sizeof(MS_PHY) * u32data);
                CPY_FROM_USER((void*)buffer_array1, (void*)pstWPParamDes->pu32EndAddr, sizeof(MS_PHY) * u32data);
            }
            pstWPParamDes->pu32StartAddr = (MS_PHY*)buffer_array0;
            pstWPParamDes->pu32EndAddr = (MS_PHY*)buffer_array1;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            break;

        case MApi_CMD_DMX_SetPowerState:
            pstPowerStateDes = (PDMX_POWSTATE_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                pstPowerStateDes->u32FWAddr = 0;
                CMP_CPY_FROM_USER((void*)&(pstPowerStateDes->u32FWAddr), ps8AdpSrc, LEN_MSPHY);
                ps8AdpSrc += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pstPowerStateDes->u32FWSize), ps8AdpSrc, LEN_U32_X1);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)&(pstPowerStateDes->u32Res), LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_POWSTATE_PARAM));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, buffer_arg, sizeof(DMX_POWSTATE_PARAM));
            }
            break;

        case MApi_CMD_DMX_GetCap:
            pstCapParamDes = (PDMX_CAP_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                if((pstCapParamDes->Quetype >= DMX_CAP_SHAREKEY_FLT_RANGE) && (pstCapParamDes->Quetype <= DMX_CAP_SHAREKEY_FLT2_RANGE))
                {
                    u32data = 2;
                }
                pstCapParamDes->pdata = (void*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(u32NextPtr, buffer_array0, sizeof(MS_U32) * u32data);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_CAP_PARAM));
                if((pstCapParamDes->Quetype >= DMX_CAP_SHAREKEY_FLT_RANGE) && (pstCapParamDes->Quetype <= DMX_CAP_SHAREKEY_FLT2_RANGE))
                {
                    u32data = 2;
                }
                pNxtPtr = pstCapParamDes->pdata;
                pstCapParamDes->pdata = (void*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pNxtPtr, pstCapParamDes->pdata, sizeof(MS_U32) * u32data);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_GetCapEx:
            pstCapExParamDes = (PDMX_CAP_EX_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X3);
                ps8AdpSrc += LEN_U32_X3;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr1, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, pstCapExParamDes->InputStrLen);
                pstCapExParamDes->StrQuetype = (char*)buffer_array0;
                pstCapExParamDes->pdata = (void*)buffer_array1;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
                CMP_CPY_TO_USER(u32NextPtr1, pstCapExParamDes->pdata, pstCapExParamDes->OutputSize);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_CAP_EX_PARAM));
                CPY_FROM_USER((void*)buffer_array0, pstCapExParamDes->StrQuetype, pstCapExParamDes->InputStrLen);
                pNxtPtr = pstCapExParamDes->pdata;
                pstCapExParamDes->StrQuetype = (char*)buffer_array0;
                pstCapExParamDes->pdata = (void*)buffer_array1;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
                CPY_to_USER(pNxtPtr, pstCapExParamDes->pdata, pstCapExParamDes->OutputSize);
            }
            break;

        case MApi_CMD_DMX_OutPadCfg:
            pstOutPadCfgDes = (DMX_OutputPad_Cfg*)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstOutPadCfgDes->pu32Resv = (void*)buffer_array0;

                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 17);
                ps8AdpSrc += 17;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)pstOutPadCfgDes->pu32Resv, u32NextPtr, LEN_U32_X1);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_OutputPad_Cfg));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstOutPadCfgDes->pu32Resv, LEN_U32_X1);
                pstOutPadCfgDes->pu32Resv = (void*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_Info:
            pstFltInfoDes = (PDMX_FLT_INFO_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstFltInfoDes->pDmxFltInfo = (DMX_Flt_info*)buffer_array0;
                pstFltInfoDes->pDmxFltType = (DMX_FILTER_TYPE*)buffer_array1;

                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr1, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER((void*)&(pstFltInfoDes->bSet), ps8AdpSrc, 1);
                if(pstFltInfoDes->bSet == TRUE)
                {
                    CMP_CPY_FROM_USER((void*)buffer_array1, u32NextPtr1, LEN_U32_X1);
                    if((*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_VIDEO) || (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO) ||
                        (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO2) || (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_VIDEO3D) ||
                        (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO3) || (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO4))
                    {
                        CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, LEN_U32_X1);
                    }
                    else
                    {
                        DMX_Sect_info* pinfo = &(pstFltInfoDes->pDmxFltInfo->Info.SectInfo);

                        pinfo->SectBufAddr = 0;
                        CMP_CPY_FROM_USER((void*)&(pinfo->SectBufAddr), u32NextPtr, LEN_MSPHY);
                        u32NextPtr += LEN_MSPHY;
                        CMP_CPY_FROM_USER((void*)&(pinfo->SectBufSize), u32NextPtr, LEN_U32_X3);
                        u32NextPtr += LEN_U32_X3;
                        pinfo->pNotify = 0;
                        CMP_CPY_FROM_USER((void*)&(pinfo->pNotify), u32NextPtr, LEN_U32_X1);
                        u32NextPtr += LEN_U32_X1;
                        pinfo->pType2Notify = 0;
                        CMP_CPY_FROM_USER((void*)&(pinfo->pType2Notify), u32NextPtr, LEN_U32_X1);
                        u32NextPtr += LEN_U32_X1;
                        CMP_CPY_FROM_USER((void*)&(pinfo->Type2NotifyParam1), u32NextPtr, LEN_U32_X1);
                    }
                }
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                if(pstFltInfoDes->bSet == FALSE)
                {
                    CMP_CPY_TO_USER(u32NextPtr1, (void*)buffer_array1, LEN_U32_X1);
                    if((*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_VIDEO) || (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO) ||
                        (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO2) || (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_VIDEO3D) ||
                        (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO3) || (*(pstFltInfoDes->pDmxFltType) == DMX_FILTER_TYPE_AUDIO4))
                    {
                        CMP_CPY_TO_USER(u32NextPtr, (void*)buffer_array0, LEN_U32_X1);
                    }
                    else
                    {
                        DMX_Sect_info* pinfo = &(pstFltInfoDes->pDmxFltInfo->Info.SectInfo);

                        pinfo->SectBufAddr = 0;
                        CMP_CPY_TO_USER(u32NextPtr, &(pinfo->SectBufAddr), LEN_MSPHY);
                        u32NextPtr += LEN_MSPHY;
                        CMP_CPY_TO_USER(u32NextPtr, &(pinfo->SectBufSize), LEN_U32_X3);
                        u32NextPtr += LEN_U32_X3;
                        CMP_CPY_TO_USER(u32NextPtr, (void*)&(pinfo->pNotify), LEN_U32_X1);
                        u32NextPtr += LEN_U32_X1;
                        CMP_CPY_TO_USER(u32NextPtr, (void*)&(pinfo->pType2Notify), LEN_U32_X1);
                        u32NextPtr += LEN_U32_X1;
                        CMP_CPY_TO_USER(u32NextPtr, &(pinfo->Type2NotifyParam1), LEN_U32_X1);
                    }
                }
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_INFO_PARAM));
                if(pstFltInfoDes->bSet == TRUE)
                {
                    CPY_FROM_USER((void*)buffer_array0, (void*)pstFltInfoDes->pDmxFltInfo, sizeof(DMX_Flt_info));
                    CPY_FROM_USER((void*)buffer_array1, (void*)pstFltInfoDes->pDmxFltType, sizeof(DMX_FILTER_TYPE));
                }
                pNxtPtr = (void*)pstFltInfoDes->pDmxFltInfo;
                pNxtPtr1 = (void*)pstFltInfoDes->pDmxFltType;
                pstFltInfoDes->pDmxFltInfo = (DMX_Flt_info*)buffer_array0;
                pstFltInfoDes->pDmxFltType = (DMX_FILTER_TYPE*)buffer_array1;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);

                if(pstFltInfoDes->bSet == FALSE)
                {
                    CPY_to_USER(pNxtPtr, (void*)buffer_array0, sizeof(DMX_Flt_info));
                    CPY_to_USER(pNxtPtr1, (void*)buffer_array1, sizeof(DMX_FILTER_TYPE));
                }
                CPY_to_USER(pArgs, (void*)&(pstFltInfoDes->u32Res), LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_Info_Ex:
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DMX_FLT_INFO_PARAM_Ex,spt_DMX_FLT_INFO_PARAM_Ex,NULL,0);
            break;

        case MApi_CMD_DMX_CopyData:  //core function must do copy_to_user for mem copy
            pstDmxFltCpyDes = (PDMX_FLT_COPY_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 20);
                ps8AdpSrc += 20;
                pstDmxFltCpyDes->pfCheckCB = 0;
                CMP_CPY_FROM_USER((void*)&(pstDmxFltCpyDes->pfCheckCB), ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                pstDmxFltCpyDes->pu8Buf = 0;
                CMP_CPY_FROM_USER((void*)&(pstDmxFltCpyDes->pu8Buf), ps8AdpSrc, LEN_U32_X1);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, 20);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_COPY_PARAM));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER((void*)pArgs, (void*)buffer_arg, sizeof(DMX_FLT_COPY_PARAM));
            }
            break;

        case MApi_CMD_DMX_Proc:
            pFltEventDes = (PDMX_FLT_EVENT_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pFltEventDes->pEvent = (DMX_EVENT*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                u32NextPtrOut = u32NextPtr;
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_EVENT));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(u32NextPtrOut, (void*)buffer_array0, sizeof(DMX_EVENT));
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_EVENT_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pFltEventDes->pEvent, sizeof(DMX_EVENT));
                pNxtPtrOut = pFltEventDes->pEvent;
                pFltEventDes->pEvent = (DMX_EVENT*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pNxtPtrOut, (void*)pFltEventDes->pEvent, sizeof(DMX_EVENT));
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_Sec_SetPattern:
            pFltSecPatDes = (PDMX_FLT_SEC_PAT_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pFltSecPatDes->pu8Pattern   = (MS_U8*)buffer_array0;
                pFltSecPatDes->pu8Mask      = (MS_U8*)buffer_array1;
                pFltSecPatDes->pu8NotMask   = (MS_U8*)buffer_array2;

                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X3);
                ps8AdpSrc += LEN_U32_X3;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr1, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr2, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, pFltSecPatDes->u32MatchSize);
                CMP_CPY_FROM_USER((void*)buffer_array1, u32NextPtr1, pFltSecPatDes->u32MatchSize);
                CMP_CPY_FROM_USER((void*)buffer_array2, u32NextPtr2, pFltSecPatDes->u32MatchSize);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_SEC_PAT_PARAM));
                CPY_FROM_USER(buffer_array0, pFltSecPatDes->pu8Pattern, pFltSecPatDes->u32MatchSize);
                CPY_FROM_USER(buffer_array1, pFltSecPatDes->pu8Mask, pFltSecPatDes->u32MatchSize);
                CPY_FROM_USER(buffer_array2, pFltSecPatDes->pu8NotMask, pFltSecPatDes->u32MatchSize);
                pFltSecPatDes->pu8Pattern   = (MS_U8*)buffer_array0;
                pFltSecPatDes->pu8Mask      = (MS_U8*)buffer_array1;
                pFltSecPatDes->pu8NotMask   = (MS_U8*)buffer_array2;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_Pvr_Open:
            pstPvrInfoDes = (PDMX_PVR_INFO_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstPvrInfoDes->pPvrInfo = (DMX_Pvr_info*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X2);
                u32NextPtrOut = u32NextPtr;
                pstPvrInfoDes->pPvrInfo->pPvrBuf0 = 0;
                CMP_CPY_FROM_USER((void*)&(pstPvrInfoDes->pPvrInfo->pPvrBuf0), u32NextPtr, LEN_MSPHY);
                u32NextPtr += LEN_MSPHY;
                pstPvrInfoDes->pPvrInfo->pPvrBuf1 = 0;
                CMP_CPY_FROM_USER((void*)&(pstPvrInfoDes->pPvrInfo->pPvrBuf1), u32NextPtr, LEN_MSPHY);
                u32NextPtr += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pstPvrInfoDes->pPvrInfo->PvrBufSize0), u32NextPtr, LEN_U32_X3);
                u32NextPtr += LEN_U32_X3;
                CMP_CPY_FROM_USER((void*)&(pstPvrInfoDes->pPvrInfo->pNotify), u32NextPtr, LEN_U32_X1);
                u32NextPtr += LEN_U32_X1;
                CMP_CPY_FROM_USER((void*)&(pstPvrInfoDes->pPvrInfo->u16CaPvrModel), u32NextPtr, 3);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                if(pstPvrInfoDes->u32Res == DMX_FILTER_STATUS_OK)
                {
                    CMP_CPY_TO_USER(u32NextPtrOut, (void*)&(pstPvrInfoDes->pPvrInfo->pPvrBuf0), LEN_MSPHY);
                    u32NextPtrOut += LEN_MSPHY;
                    CMP_CPY_TO_USER(u32NextPtrOut, (void*)&(pstPvrInfoDes->pPvrInfo->pPvrBuf1), LEN_MSPHY);
                    u32NextPtrOut += LEN_MSPHY;
                    CMP_CPY_TO_USER(u32NextPtrOut, (void*)&(pstPvrInfoDes->pPvrInfo->PvrBufSize0), LEN_U32_X2);
                }
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PVR_INFO_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstPvrInfoDes->pPvrInfo, sizeof(DMX_Pvr_info));
                pNxtPtr = (void*)pstPvrInfoDes->pPvrInfo;
                pstPvrInfoDes->pPvrInfo = (DMX_Pvr_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER((void*)pNxtPtr, (void*)buffer_array0, sizeof(DMX_Pvr_info));
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_FI_Start:
            pstFileStartDes = (PDMX_FILE_START_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X3);
                ps8AdpSrc += LEN_U32_X3;
                pstFileStartDes->pBuf = 0;
                CMP_CPY_FROM_USER((void*)&(pstFileStartDes->pBuf), ps8AdpSrc, LEN_MSPHY);
                ps8AdpSrc += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pstFileStartDes->u32BufSize), ps8AdpSrc, LEN_U32_X1);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                if(pstFileStartDes->u32Res == DMX_FILTER_STATUS_OK)
                {
                    ps8AdpSrc = (MS_S8*)pArgs;
                    ps8AdpSrc += LEN_U32_X3;
                    CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstFileStartDes->pBuf), LEN_MSPHY);
                    ps8AdpSrc += LEN_MSPHY;
                    CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstFileStartDes->u32BufSize), LEN_U32_X1);
                }
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_START_PARAM));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_FI_Info:
        case MApi_CMD_DMX_FI_Eng_Info:
            pstFileInfoDes = (PDMX_FILE_INFO_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_Filein_info));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_INFO_PARAM));
                CPY_FROM_USER((void*)buffer_array0, pstFileInfoDes->pFileinInfo, sizeof(DMX_Filein_info));
                pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_MMFI_Start:
            pstMMFIStartDes = (PDMX_MMFI_START_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X3);
                ps8AdpSrc += LEN_U32_X3;
                pstMMFIStartDes->pBuf = 0;
                CMP_CPY_FROM_USER((void*)&(pstMMFIStartDes->pBuf), ps8AdpSrc, LEN_MSPHY);
                ps8AdpSrc += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pstMMFIStartDes->u32BufSize), ps8AdpSrc, LEN_U32_X1);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                if(pstMMFIStartDes->u32Res == DMX_FILTER_STATUS_OK)
                {
                    ps8AdpSrc = (MS_S8*)pArgs;
                    ps8AdpSrc += LEN_U32_X3;
                    CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstMMFIStartDes->pBuf), LEN_MSPHY);
                    ps8AdpSrc += LEN_MSPHY;
                    CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstMMFIStartDes->u32BufSize), LEN_U32_X1);
                }
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_MMFI_START_PARAM));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_CmdRun:
            pstCmdRunDes = (PDMX_CMDRUN_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstCmdRunDes->pData = (void*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X4);
                ps8AdpSrc += LEN_U32_X4;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                if(pstCmdRunDes->u32DataNum > 0)
                {
                    CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(MS_U32) * pstCmdRunDes->u32DataNum);
                }
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_CMDRUN_PARAM));
                if(pstCmdRunDes->u32DataNum > 0)
                {
                    CPY_FROM_USER((void*)buffer_array0, pstCmdRunDes->pData, sizeof(MS_U32) * pstCmdRunDes->u32DataNum);
                }
                pstCmdRunDes->pData = (void*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSPInit_1_0:
            pTspParamDec = (DMX_TSPParam*)buffer_arg;

            if(IS_CMP_TASK())
            {
                pTspParamDec->phyFWAddr = 0;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->phyFWAddr), ps8AdpSrc, LEN_MSPHY);
                ps8AdpSrc += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->u32FWSize), ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                pTspParamDec->phyVQAddr = 0;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->phyVQAddr), ps8AdpSrc, LEN_MSPHY);
                ps8AdpSrc += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pTspParamDec->u32VQSize), ps8AdpSrc, 20);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pTspParamDec->u32Res), LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSPParam));
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSPParam));
            }
            break;

        case MApi_CMD_DMX_TSPInit_GetConfig:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMX_TSPParam, spt_DMX_TSPParam, buffer_arg, sizeof(buffer_arg));
            break;

        case MApi_CMD_DMX_TSO_FileInfo:
            pstFileInfoDes = (PDMX_FILE_INFO_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_Filein_info));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_INFO_PARAM));
                CPY_FROM_USER((void*)buffer_array0, pstFileInfoDes->pFileinInfo, sizeof(DMX_Filein_info));
                pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        // DBG INFO API
        case MApi_CMD_DMX_DBG_GET_DISCONCNT:
            pstDicConCntDes = (PDMX_PKT_DISCONCNT_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstDicConCntDes->pInfo = (DMX_DisContiCnt_info*)buffer_array0;
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_DisContiCnt_info));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                ps8AdpSrc = (MS_S8*)pArgs;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)buffer_arg, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstDicConCntDes->u32Cnt), LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_DISCONCNT_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)(pstDicConCntDes->pInfo), sizeof(DMX_DisContiCnt_info));
                pstDicConCntDes->pInfo = (DMX_DisContiCnt_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_DISCONCNT_PARAM));
            }
            break;

        case MApi_CMD_DMX_DBG_GET_DROPPKTCNT:
            pstDropCntDes = (PDMX_PKT_DROPCNT_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstDropCntDes->pInfo = (DMX_DropPktCnt_info*)buffer_array0;
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_DropPktCnt_info));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                ps8AdpSrc = (MS_S8*)pArgs;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)buffer_arg, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstDropCntDes->u32Cnt), LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_DROPCNT_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)(pstDropCntDes->pInfo), sizeof(DMX_DropPktCnt_info));
                pstDropCntDes->pInfo = (DMX_DropPktCnt_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_DROPCNT_PARAM));
            }
            break;

        case MApi_CMD_DMX_DBG_GET_LOCKPKTCNT:
            pstLockCntDes = (PDMX_PKT_LOCKCNT_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstLockCntDes->pInfo = (DMX_LockPktCnt_info*)buffer_array0;
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_LockPktCnt_info));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                ps8AdpSrc = (MS_S8*)pArgs;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)buffer_arg, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstLockCntDes->u32Cnt), LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_LOCKCNT_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)(pstLockCntDes->pInfo), sizeof(DMX_LockPktCnt_info));
                pstLockCntDes->pInfo = (DMX_LockPktCnt_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                ps8AdpSrc = (MS_S8*)pArgs;
                CPY_to_USER((void*)ps8AdpSrc, (void*)buffer_arg, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X2;
                CPY_to_USER((void*)ps8AdpSrc, (void*)&(pstLockCntDes->u32Cnt), LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_DBG_GET_AVPKTCNT:
            pstAVPktCntDes = (PDMX_PKT_AVCNT_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstAVPktCntDes->pInfo = (DMX_AVPktCnt_info*)buffer_array0;
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_AVPktCnt_info));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                ps8AdpSrc = (MS_S8*)pArgs;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)buffer_arg, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_TO_USER(ps8AdpSrc, (void*)&(pstAVPktCntDes->u32Cnt), LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_AVCNT_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)(pstAVPktCntDes->pInfo), sizeof(DMX_AVPktCnt_info));
                pstAVPktCntDes->pInfo = (DMX_AVPktCnt_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                ps8AdpSrc = (MS_S8*)pArgs;
                CPY_to_USER((void*)ps8AdpSrc, (void*)buffer_arg, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X2;
                CPY_to_USER((void*)ps8AdpSrc, (void*)&(pstAVPktCntDes->u32Cnt), LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSO_LocStrId:
            pstTsoLocStrIdDes = (PDMX_TSO_LOCSTRID_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstTsoLocStrIdDes->pu8StrId = (MS_U8*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 9);
                ps8AdpSrc += 9;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER((void*)&(pstTsoLocStrIdDes->bSet), ps8AdpSrc, 1);
                if(pstTsoLocStrIdDes->bSet == TRUE)
                {
                    CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, 1);
                }
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                if(pstTsoLocStrIdDes->bSet == FALSE)
                {
                    CMP_CPY_TO_USER(u32NextPtr, (void*)buffer_array0, 1);
                }
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSO_LOCSTRID_PARAM));
                if(pstTsoLocStrIdDes->bSet == TRUE)
                {
                    CPY_FROM_USER((void*)buffer_array0, (void*)(pstTsoLocStrIdDes->pu8StrId), sizeof(MS_U8));
                }
                pNxtPtr = (void*)pstTsoLocStrIdDes->pu8StrId;
                pstTsoLocStrIdDes->pu8StrId = (MS_U8*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);

                if(pstTsoLocStrIdDes->bSet == FALSE)
                {
                    CPY_to_USER(pNxtPtr, (void*)(pstTsoLocStrIdDes->pu8StrId),sizeof(MS_U8));
                }
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_FQ_Init:
            pstFqInitDes = (PDMX_FQ_INIT_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstFqInitDes->pInfo = (DMX_FQ_Info*)buffer_array0;

                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)&(pstFqInitDes->pInfo->u8AddrMode), u32NextPtr, 1);
                u32NextPtr += 1;
                pstFqInitDes->pInfo->u32BufStart = 0;
                CMP_CPY_FROM_USER((void*)&(pstFqInitDes->pInfo->u32BufStart), u32NextPtr, LEN_MSPHY);
                u32NextPtr += LEN_MSPHY;
                CMP_CPY_FROM_USER((void*)&(pstFqInitDes->pInfo->u32BufSize), u32NextPtr, LEN_U32_X1);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)pstFqInitDes, pArgs, sizeof(DMX_FQ_INIT_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)(pstFqInitDes->pInfo), sizeof(DMX_FQ_Info));
                pstFqInitDes->pInfo = (DMX_FQ_Info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_MStr_SyncBy:
            pstMstrSynByDes = (PDMX_MSTR_SYNCBY_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstMstrSynByDes->pu8SyncByte = (MS_U8*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 9);
                ps8AdpSrc += 9;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER((void*)&(pstMstrSynByDes->bSet), ps8AdpSrc, 1);
                if(pstMstrSynByDes->bSet == TRUE)
                {
                    CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, 1);
                }
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                if(pstMstrSynByDes->bSet == FALSE)
                {
                    CMP_CPY_TO_USER(u32NextPtr, (void*)buffer_array0, 1);
                }
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)pstMstrSynByDes, pArgs, sizeof(DMX_MSTR_SYNCBY_PARAM));
                if(pstMstrSynByDes->bSet == TRUE)
                {
                    CPY_FROM_USER((void*)buffer_array0, (void*)pstMstrSynByDes->pu8SyncByte, sizeof(MS_U8));
                }
                pNxtPtr = (void*)pstMstrSynByDes->pu8SyncByte;
                pstMstrSynByDes->pu8SyncByte = (MS_U8*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                if(pstMstrSynByDes->bSet == FALSE)
                {
                    CPY_to_USER(pNxtPtr, (void*)pstMstrSynByDes->pu8SyncByte,sizeof(MS_U8));
                }
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_MMFI_Info:
            pstFileInfoDes = (PDMX_FILE_INFO_PARAM)buffer_arg;

            if(IS_CMP_TASK())
            {
                pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X2);
                ps8AdpSrc += LEN_U32_X2;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X2);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(DMX_Filein_info));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_INFO_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstFileInfoDes->pFileinInfo, sizeof(DMX_Filein_info));
                pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSO_Configure:
            pstTSOCfg = (DMX_TSO_Cfg*)buffer_arg;
            u32data = sizeof(DMX_TSO_Cfg);

            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4 + 1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4 + 1);
            }
            if((pstTSOCfg->u32TsoCfgDataSize > 0) && (pstTSOCfg->u32TsoCfgDataSize <= u32data))
                u32data = pstTSOCfg->u32TsoCfgDataSize;
            u32InSize = u32data;
            u32OutSize = u32data;
            break;

        case MApi_CMD_DMX_FQ_Configure:
            pstFqCfg = (DMX_FQ_Cfg*)pArgs;
            u32data = sizeof(DMX_FQ_Cfg);

            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4 + 1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4 + 1);
            }
            if((pstFqCfg->u32CfgDataSize > 0) && (pstFqCfg->u32CfgDataSize <= u32data))
                u32data = pstFqCfg->u32CfgDataSize;
            u32InSize = u32data;
            u32OutSize = u32data;
            break;

        case MApi_CMD_DMX_MMFI_BypassStamp:
        case MApi_CMD_DMX_FI_BypassFileTimestamp:
            u32InSize = LEN_U32_X2;
            break;

        case MApi_CMD_DMX_OrzWProtectEnable:
            u32InSize = 20;
            break;

        case MApi_CMD_DMX_Exit:
        case MApi_CMD_DMX_ForceExit:
        case MApi_CMD_DMX_Suspend:
        case MApi_CMD_DMX_ChkFwAlive:
        case MApi_CMD_DMX_Reset:
        case MApi_CMD_DMX_ReleaseAccess:
        case MApi_CMD_DMX_GetFwVer:
        case MApi_CMD_DMX_Init_1_0:
        case MApi_CMD_DMX_ReleseHWSemp:
        case MApi_CMD_DMX_GetLibVer:
            u32OutSize = LEN_U32_X1;
            break;

        case MApi_CMD_DMX_Close:
        case MApi_CMD_DMX_Start:
        case MApi_CMD_DMX_Stop:
        case MApi_CMD_DMX_SetDbgLevel:
        case MApi_CMD_DMX_DropEn:
        case MApi_CMD_DMX_File_PVR_Stop:
        case MApi_CMD_DMX_Sec_Reset:
        case MApi_CMD_DMX_TSO_FileStop:
        case MApi_CMD_DMX_GetAccess:
        case MApi_CMD_DMX_AUBD_Enable:
        case MApi_CMD_DMX_Pvr_Close:
        case MApi_CMD_DMX_FI_Stop:
        case MApi_CMD_DMX_FI_Pause:
        case MApi_CMD_DMX_FI_Resume:
        case MApi_CMD_DMX_FI_IsIdle:
        case MApi_CMD_DMX_FI_IsBusy:
        case MApi_CMD_DMX_FI_IsPause:
        case MApi_CMD_DMX_FI_CmdQReset:
        case MApi_CMD_DMX_MMFI_IsIdle:
        case MApi_CMD_DMX_MMFI_IsBusy:
        case MApi_CMD_DMX_MMFI_CmdQReset:
        case MApi_CMD_DMX_SetHK:
        case MApi_CMD_DMX_STC64ModeEnable:
        case MApi_CMD_DMX_RESET_SECDISCON_PKTCNT:
        case MApi_CMD_DMX_TSO_CmdQReset:
        case MApi_CMD_DMX_GetIntCnt:
        case MApi_CMD_DMX_RESET_SECTEI_PKTCNT:
        case MApi_CMD_DMX_TSO_FileIsIdle:
            u32InSize = LEN_U32_X2;
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_ReadDropCount:
        case MApi_CMD_DMX_TSO_OutputEn:
        case MApi_CMD_DMX_FI_CmdQGetEmptyNum:
        case MApi_CMD_DMX_FI_CmdQGetFifoLevel:
        case MApi_CMD_DMX_FI_GetFileTimeStamp:
        case MApi_CMD_DMX_Flow_ParlInvert:
        case MApi_CMD_DMX_Flow_ENABLE:
        case MApi_CMD_DMX_Pvr_IsStart:
        case MApi_CMD_DMX_IsStart:
        case MApi_CMD_DMX_GetOwner:
        case MApi_CMD_DMX_GetPESScmbSts:
        case MApi_CMD_DMX_GetTsScmbSts:
        case MApi_CMD_DMX_MMFI_Timestamp_En:
        case MApi_CMD_DMX_TSO_FileGetTimeStamp:
        case MApi_CMD_DMX_TSO_FileBypassStamp:
        case MApi_CMD_DMX_TSO_FileTimeStampEnable:
        case MApi_CMD_DMX_GET_SECDISCON_PKTCNT:
        case MApi_CMD_DMX_STCEng_Sel:
        case MApi_CMD_DMX_File_PVR_PidClose:
        case MApi_CMD_DMX_File_PVR_Start:
        case MApi_CMD_DMX_FI_TimestampMode:
        case MApi_CMD_DMX_Pvr_Stop:
        case MApi_CMD_DMX_Pvr_SetPktMode:
        case MApi_CMD_DMX_Pvr_SetRecordStamp:
        case MApi_CMD_DMX_Pvr_GetRecordStamp:
        case MApi_CMD_DMX_MMFI_GetEmptyNum:
        case MApi_CMD_DMX_MMFI_GetFileTimeStamp:
        case MApi_CMD_DMX_MMFI_PidClose:
        case MApi_CMD_DMX_MMFI_GetFifoLevel:
        case MApi_CMD_DMX_MMFI_PlayStamp_Set:
        case MApi_CMD_DMX_MMFI_PlayStamp_Get:
        case MApi_CMD_DMX_MMFI_SetTimeStampClk:
        case MApi_CMD_DMX_SetMergeStrSync:
        case MApi_CMD_DMX_TSO_FileGetCmdQEmptyNum:
        case MApi_CMD_DMX_TSO_PidClose:
        case MApi_CMD_DMX_FQ_Exit:
        case MApi_CMD_DMX_FQ_RushEnable:
        case MApi_CMD_DMX_TeiRmErrPkt:
        case MApi_CMD_DMX_Get_DbgPortInfo:
        case MApi_CMD_DMX_Pvr_Pause:
        case MApi_CMD_DMX_Stc_UpdateCtrl:
        case MApi_CMD_DMX_RES_ALLOC:
        case MApi_CMD_DMX_RES_FREE:
        case MApi_CMD_DMX_PCR_GET_MAP_STC:
            u32InSize = LEN_U32_X3;
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_SetFwMiuDataAddr:
        case MApi_CMD_DMX_MMFI_SetMOBFKey:
        case MApi_CMD_DMX_TSO_FI_GetReadAddr:
        case MApi_CMD_DMX_Open_MultiFlt:
        case MApi_CMD_DMX_SetOwner:
        case MApi_CMD_DMX_FQ_SetFltRushPass:
        case MApi_CMD_DMX_TsOutPhase:
        case MApi_CMD_DMX_GetPcr:
        case MApi_CMD_DMX_Stc_Get:
        case MApi_CMD_DMX_Stc_Set:
        case MApi_CMD_DMX_Stc_SetOffset:
        case MApi_CMD_DMX_Stc_ClkAdjust:
        case MApi_CMD_DMX_Pvr_CBSize:
        case MApi_CMD_DMX_Pvr_SetCaMode:
        case MApi_CMD_DMX_Pid:
        case MApi_CMD_DMX_Sec_GetWriteAddr:
        case MApi_CMD_DMX_Sec_GetStartAddr:
        case MApi_CMD_DMX_Sec_GetEndAddr:
        case MApi_CMD_DMX_GetTTXWrite:
        case MApi_CMD_DMX_FI_PlayStamp:
        case MApi_CMD_DMX_Pvr_PidClose:
        case MApi_CMD_DMX_Pvr_Start:
        case MApi_CMD_DMX_Pvr_GetWriteAddr:
        case MApi_CMD_DMX_FI_GetReadAddr:
        case MApi_CMD_DMX_FI_SetMobfKey:
        case MApi_CMD_DMX_SetFw:
        case MApi_CMD_DMX_TSO_FilePlayStamp:
        case MApi_CMD_DMX_PVR_MOBFEn:
        case MApi_CMD_DMX_Get_PipeId:
            u32InSize = LEN_U32_X4;
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_GetFltScmbSts:
        case MApi_CMD_DMX_Sec_ReadAddr:
        case MApi_CMD_DMX_Pvr_SetMobfKey:
        case MApi_CMD_DMX_TSO_FileStart:
        case MApi_CMD_DMX_TSO_PidOpen:
        case MApi_CMD_DMX_TSO_SvqBufSet:
        case MApi_CMD_DMX_SetFwDbgParam:
        case MApi_CMD_DMX_File_PVR_PidOpen:
        case MApi_CMD_DMX_Pvr_Pid:
            u32InSize = 20;
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_SetBurstLen:
            u32InSize = sizeof(DMX_BURSTTYPE);
            break;

        case MApi_CMD_DMX_SetPktMode:
            u32InSize = sizeof(DMX_PKT_MODE_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_FQ_SkipRushData:
            u32InSize = sizeof(DMX_FQ_SKIP_RUSH_DATA_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_Flow:
            u32InSize = sizeof(DMX_FLOW_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_PVR_Flow:
            u32InSize = sizeof(DMX_PVRFLOWPARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_FlowDscmbEng:
            u32InSize = sizeof(DMX_FLOW_DSCMBENG_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_DropScmbPkt:
            u32InSize = sizeof(DMX_DROP_SCMB_PKT_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_Open:
        case MApi_CMD_DMX_Open_Ex:
        case MApi_CMD_DMX_ChangeFltSrc:
            u32InSize =  sizeof(DMX_FLT_TYPE_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_AVFIFO_Reset:
        case MApi_CMD_DMX_AVFIFO_Status:
        case MApi_CMD_DMX_MMFI_RemoveDupAVPkt:
            u32InSize = sizeof(DMX_AVFIFO_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_RemoveDupAVFifoPkt:
            u32InSize = sizeof(DMX_AVFIFO_DROP_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_MMFI_PidOpen:
            u32InSize = sizeof(DMX_MMFI_FLT_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_FI_SetPlaybackStampClk:
        case MApi_CMD_DMX_Pvr_SetRecordStampClk:
            u32InSize = sizeof(DMX_TIMESTAMP_CLK_PARAM);
            break;

        case MApi_CMD_DMX_Pvr_PidOpen:
            u32InSize = sizeof(DMX_PVR_FLTTYPE_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_GET_SECTEI_PKTCNT:
            u32InSize = sizeof(DMX_PKT_SECTEI_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSO_OutClk:
            u32InSize = sizeof(DMX_TSO_OUTCLK_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_WaitTspCbEvt:
            u32InSize = sizeof(DMX_TSP_IOSIGNAL);
            u32OutSize = u32InSize;
            break;
        case MApi_CMD_DMX_TSO_InputCfg:
            u32InSize = sizeof(DMX_TSO_InputCfg);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSO_OutputCfg:
            u32InSize = sizeof(DMX_TSO_OutputCfg);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_Service_SetDMAoutVC:
            pstTsioDoutDes = (PDMX_TSIO_Service_SetDMAoutVC_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioDoutDes->eSet = (TSIO_DMAOUT_SET *)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 6);
                ps8AdpSrc += 6;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(TSIO_DMAOUT_SET));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_Service_SetDMAoutVC_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioDoutDes->eSet, sizeof(TSIO_DMAOUT_SET));
                pstTsioDoutDes->eSet = (TSIO_DMAOUT_SET*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSIO_Service_SetDestination:
            u32InSize = sizeof(DMX_TSIO_Service_SetDestination_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_Service_SetDMAinVC:
            pstTsioDinDes = (PDMX_TSIO_Service_SetDMAinVC_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioDinDes->eSet = (TSIO_DMAIN_SET *)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 6);
                ps8AdpSrc += 6;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(TSIO_DMAIN_SET));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_Service_SetDMAinVC_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioDinDes->eSet, sizeof(TSIO_DMAIN_SET));
                pstTsioDinDes->eSet = (TSIO_DMAIN_SET*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSIO_Service_SetLocdecKey:
            pstTsioLocDes = (PDMX_TSIO_Service_SetLocdecKey_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioLocDes->eSet = (TSIO_LOCKEY_SET *)buffer_array0;
                pstTsioLocDes->eSet->u8Evenkey = (MS_U8 *)buffer_array1;
                pstTsioLocDes->eSet->u8Oddkey = (MS_U8 *)buffer_array2;

                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 6);
                ps8AdpSrc += 6;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void *)&(pstTsioLocDes->eSet->bEnable), u32NextPtr, sizeof(MS_BOOL));
                u32NextPtr+=sizeof(MS_BOOL);
                CMP_CPY_FROM_USER((void *)&(pstTsioLocDes->eSet->bEvenkeyenable), u32NextPtr, sizeof(MS_BOOL));
                u32NextPtr+=3;
                CMP_CPY_FROM_USER(&u32NextPtr1, u32NextPtr, LEN_U32_X1);
                u32NextPtr+=LEN_U32_X1;
                CMP_CPY_FROM_USER((void *)&(pstTsioLocDes->eSet->u16Evenkeylen), u32NextPtr, sizeof(MS_U16));
                u32NextPtr+=sizeof(MS_U16);
                CMP_CPY_FROM_USER((void *)&(pstTsioLocDes->eSet->bOddkeyenable), u32NextPtr, sizeof(MS_BOOL));
                u32NextPtr+=2;
                CMP_CPY_FROM_USER(&u32NextPtr2, u32NextPtr, LEN_U32_X1);
                u32NextPtr+=LEN_U32_X1;
                CMP_CPY_FROM_USER((void *)&(pstTsioLocDes->eSet->u16Oddkeylen), u32NextPtr, sizeof(MS_U16));
                CMP_CPY_FROM_USER((void *)buffer_array1, u32NextPtr1, pstTsioLocDes->eSet->u16Evenkeylen);
                CMP_CPY_FROM_USER((void *)buffer_array2, u32NextPtr2, pstTsioLocDes->eSet->u16Oddkeylen);
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_Service_SetLocdecKey_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioLocDes->eSet, sizeof(TSIO_LOCKEY_SET));
                pstTsioLocDes->eSet = (TSIO_LOCKEY_SET *)buffer_array0;

                if (pstTsioLocDes->eSet->bEvenkeyenable == TRUE)
                {
                    CPY_FROM_USER((void*)buffer_array1, (void*)pstTsioLocDes->eSet->u8Evenkey,pstTsioLocDes->eSet->u16Evenkeylen);
                    pstTsioLocDes->eSet->u8Evenkey = (MS_U8 *)buffer_array1;
                }

                if (pstTsioLocDes->eSet->bOddkeyenable == TRUE)
                {
                    CPY_FROM_USER((void*)buffer_array2, (void*)pstTsioLocDes->eSet->u8Oddkey, pstTsioLocDes->eSet->u16Oddkeylen);
                    pstTsioLocDes->eSet->u8Oddkey = (MS_U8 *)buffer_array2;
                }
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSIO_Service_PidOpen:
            pstTsioPidDes = (PDMX_TSIO_Service_PidOpen_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioPidDes->pu16DmxId = (MS_U16 *)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X3);
                ps8AdpSrc += LEN_U32_X3;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(MS_U16));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                CMP_CPY_TO_USER(u32NextPtr, (void*)buffer_array0, sizeof(MS_U16));
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_Service_PidOpen_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioPidDes->pu16DmxId, sizeof(MS_U16));
                pNxtPtr = (void *)pstTsioPidDes->pu16DmxId;
                pstTsioPidDes->pu16DmxId = (MS_U16*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                CMP_CPY_TO_USER(pNxtPtr, (void*)buffer_array0, sizeof(MS_U16));
            }
            break;

        case MApi_CMD_DMX_TSIO_Service_PidClose:
            u32InSize = sizeof(DMX_TSIO_Service_PidClose_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_Service_Alloc:
            pstTsioAlcDes = (PDMX_TSIO_Service_Alloc_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioAlcDes->u16Handle = (MS_U16 *)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 5);
                ps8AdpSrc += 5;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(MS_U16));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                CMP_CPY_TO_USER(u32NextPtr, (void*)buffer_array0, sizeof(MS_U16));
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_Service_Alloc_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioAlcDes->u16Handle, sizeof(MS_U16));
                pNxtPtr = (void *)pstTsioAlcDes->u16Handle;
                pstTsioAlcDes->u16Handle = (MS_U16*)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                CPY_to_USER(pNxtPtr, (void*)buffer_array0, sizeof(MS_U16));
            }
            break;

        case MApi_CMD_DMX_TSIO_Service_Free:
            u32InSize = sizeof(DMX_TSIO_Service_Free_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_RegisterIntCb: // not ready
            pstTsioCbDes = (PDMX_TSIO_RegisterIntCb_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioCbDes->efn = NULL;//(TSIOIntTypeCB)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                //CMP_CPY_FROM_USER((void*)(buffer_array0), ps8AdpSrc, sizeof(TSIOIntTypeCB));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_RegisterIntCb_PARAM));
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

         case MApi_CMD_DMX_TSIO_Init:
             pstTsioInitDes = (PDMX_TSIO_Init_PARAM)buffer_arg;
             if(IS_CMP_TASK())
             {
                 pstTsioInitDes->eParm = (TSIO_Init_Set *)buffer_array0;
                 CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X1);
                 ps8AdpSrc += LEN_U32_X1;
                 CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                 CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(TSIO_Init_Set));
                 u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                 CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
             }
             else
             {
                 CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_Init_PARAM));
                 CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioInitDes->eParm, sizeof(TSIO_Init_Set));
                 pstTsioInitDes->eParm = (TSIO_Init_Set*)buffer_array0;
                 u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                 CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
             }
             break;

        case MApi_CMD_DMX_TSIO_Cmd:
            u32InSize = sizeof(DMX_TSIO_Cmd_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_Exit:
            u32InSize = sizeof(MS_U32);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_CC:
            pstTsioCcDes = (PDMX_TSIO_CC_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioCcDes->Parm = (TSIO_CC_Set *)buffer_array2;
                pstTsioCcDes->pResult = (TSIO_CC_RESULT *)buffer_array3;
                pstTsioCcDes->Parm->pIn = (MS_U8 *)buffer_array0;
                pstTsioCcDes->Parm->pOut = (MS_U8 *)buffer_array1;
                pstTsioCcDes->Parm->pOutLen= (MS_U16 *)buffer_array4;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                ps8AdpSrc += LEN_U32_X1;
                CMP_CPY_FROM_USER(&u32NextPtrOut, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void *)&u32NextPtr1, u32NextPtr, LEN_U32_X1);
                u32NextPtr+=LEN_U32_X1;
                CMP_CPY_FROM_USER((void *)&(pstTsioCcDes->Parm->u16InLen), u32NextPtr, sizeof(MS_U16));
                u32NextPtr+=LEN_U32_X1;
                CMP_CPY_FROM_USER((void *)&u32NextPtr2, u32NextPtr, LEN_U32_X1);
                u32NextPtr+=LEN_U32_X1;
                CMP_CPY_FROM_USER((void *)&u32NextPtr3, u32NextPtr, LEN_U32_X1);
                u32NextPtr+=LEN_U32_X1;
                CMP_CPY_FROM_USER((void *)buffer_array0, u32NextPtr1, pstTsioCcDes->Parm->u16InLen);
                CMP_CPY_FROM_USER((void *)buffer_array1, u32NextPtr2, sizeof(MS_U8));
                CMP_CPY_FROM_USER((void *)buffer_array4, u32NextPtr3, sizeof(MS_U16));
                CMP_CPY_FROM_USER((void *)&(pstTsioCcDes->Parm->u32Timeout), u32NextPtr, sizeof(MS_U32));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                if (*pstTsioCcDes->pResult == TSIO_CC_RESULT_OK)
                {
                    CMP_CPY_TO_USER(u32NextPtr2, (void*)buffer_array1, *pstTsioCcDes->Parm->pOutLen);
                    CMP_CPY_TO_USER(u32NextPtr3, (void*)buffer_array4, sizeof(MS_U16));
                }
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_CC_PARAM));
                CPY_FROM_USER((void*)buffer_array2, (void*)pstTsioCcDes->Parm, sizeof(TSIO_CC_Set));
                pstTsioCcDes->Parm = (TSIO_CC_Set *)buffer_array2;
                if (pstTsioCcDes->Parm->pIn != NULL)
                {
                    CPY_FROM_USER((void*)buffer_array0, pstTsioCcDes->Parm->pIn, pstTsioCcDes->Parm->u16InLen);
                    pstTsioCcDes->Parm->pIn = (MS_U8 *)buffer_array0;
                }
                if (pstTsioCcDes->Parm->pOut != NULL)
                    pstTsioCcDes->Parm->pOut = (MS_U8 *)buffer_array1;

                if (pstTsioCcDes->Parm->pOutLen != NULL)
                    pstTsioCcDes->Parm->pOut = (MS_U8 *)buffer_array4;
                pstTsioCcDes->pResult = (TSIO_CC_RESULT *)buffer_array3;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSIO_CC_PARAM));
            }
            break;

        case MApi_CMD_DMX_TSIO_GetInfo:
            pstTsioGetInfoDes = (PDMX_TSIO_GetInfo_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioGetInfoDes->u32Retinfo = (MS_U32 *)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, LEN_U32_X3);
                ps8AdpSrc += LEN_U32_X3;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(MS_U32));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
                CMP_CPY_TO_USER(u32NextPtr, (void*)buffer_array0, sizeof(MS_U32));
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_GetInfo_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioGetInfoDes->u32Retinfo, sizeof(MS_U32));
                pNxtPtr = (void *)pstTsioGetInfoDes->u32Retinfo;
                pstTsioGetInfoDes->u32Retinfo = (MS_U32 *)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pNxtPtr, (void*)buffer_array0, sizeof(MS_U32*));
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSIO_Service_DMAinVC_Start:
            u32InSize = sizeof(DMX_TSIO_Service_DMAinVC_Start_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_Service_LocdecKeyEnable:
            u32InSize = sizeof(DMX_TSIO_Service_LocdecKeyEnable_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_Service_DMAout_WriteGet:
            pstTsioDWriteDes = (PDMX_TSIO_Service_DMAout_WriteGet_PARAM)buffer_arg;
            if(IS_CMP_TASK())
            {
                pstTsioDWriteDes->pphyWrite = (MS_PHY *)buffer_array0;
                CMP_CPY_FROM_USER((void*)buffer_arg, ps8AdpSrc, 6);
                ps8AdpSrc += 6;
                CMP_CPY_FROM_USER(&u32NextPtr, ps8AdpSrc, LEN_U32_X1);
                CMP_CPY_FROM_USER((void*)buffer_array0, u32NextPtr, sizeof(MS_PHY*));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
                CMP_CPY_TO_USER(pArgs, buffer_arg, LEN_U32_X1);
                CMP_CPY_TO_USER(u32NextPtr, buffer_array0, sizeof(MS_PHY*));
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSIO_Service_DMAout_WriteGet_PARAM));
                CPY_FROM_USER((void*)buffer_array0, (void*)pstTsioDWriteDes->pphyWrite, sizeof(MS_PHY));
                pNxtPtr = (void *)pstTsioDWriteDes->pphyWrite;
                pstTsioDWriteDes->pphyWrite = (MS_PHY *)buffer_array0;
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
                CPY_to_USER(pNxtPtr, (void*)buffer_array0, sizeof(MS_PHY*));
                CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            }
            break;

        case MApi_CMD_DMX_TSIO_Open:
            u32InSize = sizeof(DMX_TSIO_Open_PARAM);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_Close:
            u32InSize = sizeof(MS_U32);
            u32OutSize = u32InSize;
            break;

        case MApi_CMD_DMX_TSIO_WaitCbEvt:
            u32InSize = sizeof(DMX_TSIO_IOSIGNAL);
            u32OutSize = u32InSize;
            break;

        default:
            DMX_ADP_MSG("DMXADP - Unknown commend!!!\n");
            break;
    }

    if((u32InSize > 0) || (u32OutSize > 0))
    {
        if(u32InSize > 0)
        {
            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER((void*)buffer_arg, pArgs, u32InSize);
            }
            else
            {
                CPY_FROM_USER((void*)buffer_arg, pArgs, u32InSize);
            }
        }
        u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        if(u32OutSize > 0)
        {
            if(IS_CMP_TASK())
            {
                CMP_CPY_TO_USER(pArgs, (void*)buffer_arg, u32OutSize);
            }
            else
            {
                CPY_to_USER(pArgs, (void*)buffer_arg, u32OutSize);
            }
        }
    }
    //printf("[DMX] E %d\n", (int)u32Ret);

    return u32Ret;

}

#endif


