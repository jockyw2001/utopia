#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/compat.h>

#include "MsTypes.h"
//#include <stdio.h>
//#include "utopia_DDI.h"
#include "utopia.h"
#include "apiXC_Ace.h"
#include "apiXC_Ace_v2.h"

#include "utopia_adp.h"
#include "ACE_adp.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                                    if(copy_from_user(a,b,c) != 0) { break; }
#define CPY_to_USER(a,b,c)                                      if(copy_to_user(a,b,c) != 0) { break; }
#else
#define CPY_FROM_USER                                           memcpy
#define CPY_to_USER                                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                                           compat_ptr(a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)compat_ptr((unsigned long)a)))
#define IS_CMP_TASK()                                             is_compat_task()

#define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
#define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }

#else
#define COMPAT_PTR(a)                                           (a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)a))
#define IS_CMP_TASK()                                             (FALSE)
#define CMP_CPY_FROM_USER                                       CPY_FROM_USER
#define CMP_CPY_TO_USER                                         CPY_to_USER
#endif //CONFIG_COMPAT

#ifdef CONFIG_COMPAT
typedef struct
{
    compat_uptr_t pstXC_ACE_InitData;
    MS_U32 u32InitDataLen;
    MS_BOOL bReturn;
} _cmp_stACE_Init;
typedef struct
{
    MS_BOOL    eWindow;                  ///< initial eWindow

    compat_uptr_t    S16ColorCorrectionMatrix; ///< initial Color Correction Matrix

    MS_U32 u32AlignmentDummy0;

    compat_uptr_t    S16RGB;                   ///< initial RGB

    MS_U32 u32AlignmentDummy1;

    MS_U16     u16MWEHstart;             ///< initial Horizontal start


    MS_U16     u16MWEVstart;             ///< initial Vertical start

    MS_U16     u16MWEWidth;              ///< initial MWE Width

    MS_U16     u16MWEHeight;             ///< initial MWE Height

    MS_U16     u16MWE_Disp_Hstart;       ///< initial MWE Display Horizontal start

    MS_U16     u16MWE_Disp_Vstart;       ///< initial MWE Display Vertical start

    MS_U16     u16MWE_Disp_Width;        ///< initial MWE Display width

    MS_U16     u16MWE_Disp_Height;       ///< initial MWE Display height

    MS_BOOL    bMWE_Enable;              ///< Enable or not
} _cmp_XC_ACE_InitData;
#endif
//Function parameter
UADP_SPT_NAMEnNXT_DEF(XC_ACE_InitData,3);
UADP_SPT_NAMEnNXT_DEF(ACE_INIT,1);

UADP_STRUCT_POINTER_TABLE spt_ACE_EXIT[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DMS[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETPCYUV2RGB[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_YUVtoRGBMatrix[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SELECTYUVTORGBMATRIX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETCOLORCORRECTIONTABLE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETPCSRGBTABLE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_GETCOLORMATRIX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETCOLORMATRIX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETBYPASSCOLORMATRIX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETIHC[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PATCHDTGCOLORCHECKER[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_GETACEINFO[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_GETACEINFOEX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETCONTRAST[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETBRIGHTNESS[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETBRIGHTNESSPRECISE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSET_PRE_Y_OFFSET[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICGET_PRE_Y_OFFSET[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETHUE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SKIPWAITVSYNC[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETSATURATION[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETSHARPNESS[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETCOLORTEMP[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETCOLORTEMPEX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETPOSTCOLORTEMP_V02[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETFLESHTONE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETBLACKADJUST[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SET_IHC_SRAM[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SET_ICC_SRAM[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_ENABLEMWE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_MWESETMIRRORMODE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_MWESETREGTABLE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETMWELOADFROMPQ[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_MWESTATUS[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_MWEFUNCSEL[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_MWESETDISPWIN[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_3DCLONEPQMAP[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_BLENDING_NRTBL[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_BLENDING_MISC[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_BLENDING[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_GET_PRECISIONSHIFT[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_GETMOTION_WEIGHT[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_GETLUMA_WEIGHT[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_GETNOISE_WEIGHT[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_GETMOTION[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_SETPARAM[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_GETPARAM[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_DNR_SETNRTBL[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_GETINFO[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_GETSTATUS[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETDBGLEVEL[10];
UADP_SPT_NAMEnNXT_DEF(ColorCorrectionTable,0);
UADP_SPT_NAMEnNXT_DEF(ACE_COLORCORRECTIONTABLE,1);
UADP_STRUCT_POINTER_TABLE spt_ACE_SETCOLORMATRIXCONTROL[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SETRBCHANNELRANGE[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETPOSTCOLORTEMP[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETPOSTCOLORTEMPEX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETPOSTCOLORTEMP2[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_PICSETPOSTCOLORTEMP2EX[10];
UADP_STRUCT_POINTER_TABLE spt_ACE_SET_POWERSTATE[10];

//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_DrvACE_InitData[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_InfoEx[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_Color_Temp[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_Color_TempEx[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_Color_TempEx2[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_TAB_Info[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_ApiStatus[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_Color_TempEx2Ex[10];
UADP_STRUCT_POINTER_TABLE spt_DrvACE_ApiInfo[10];


MS_U32 ACE_adp_Init(FUtopiaIOctl* pIoctl)
{

//set table
    MS_U32 temp = 0;

    //Function parameter
    //stACE_Init* pACE_INIT = (stACE_Init*)temp;
    //stACE_Exit* pACE_EXIT = (stACE_Exit*)temp;
    //stACE_DMS* pACE_DMS = (stACE_DMS*)temp;
    //stACE_SetPCYUV2RGB* pACE_SETPCYUV2RGB = (stACE_SetPCYUV2RGB*)temp;
    //stACE_SelectYUVtoRGBMatrix* pACE_SELECTYUVTORGBMATRIX = (stACE_SelectYUVtoRGBMatrix*)temp;
    //stACE_SetColorCorrectionTable* pACE_SETCOLORCORRECTIONTABLE = (stACE_SetColorCorrectionTable*)temp;
    //stACE_SetPCsRGBTable* pACE_SETPCSRGBTABLE = (stACE_SetPCsRGBTable*)temp;
    //stACE_ColorMatrix* pACE_GETCOLORMATRIX = (stACE_ColorMatrix*)temp;
    //stACE_ColorMatrix* pACE_SETCOLORMATRIX = (stACE_ColorMatrix*)temp;
    //stACE_SetBypassColorMatrix* pACE_SETBYPASSCOLORMATRIX = (stACE_SetBypassColorMatrix*)temp;
    //stACE_SetIHC* pACE_SETIHC = (stACE_SetIHC*)temp;
    //stACE_PatchDTGColorChecker* pACE_PATCHDTGCOLORCHECKER = (stACE_PatchDTGColorChecker*)temp;
    //stACE_GetACEInfo* pACE_GETACEINFO = (stACE_GetACEInfo*)temp;
    stACE_GetACEInfoEx* pACE_GETACEINFOEX = (stACE_GetACEInfoEx*)temp;
    //stACE_PicSetContrast* pACE_PICSETCONTRAST = (stACE_PicSetContrast*)temp;
    //stACE_PicSetBrightness* pACE_PICSETBRIGHTNESS = (stACE_PicSetBrightness*)temp;
    //stACE_PicSetBrightnessPrecise* pACE_PICSETBRIGHTNESSPRECISE = (stACE_PicSetBrightnessPrecise*)temp;
    //stACE_PreYOffset* pACE_PICSET_PRE_Y_OFFSET = (stACE_PreYOffset*)temp;
    //stACE_PreYOffset* pACE_PICGET_PRE_Y_OFFSET = (stACE_PreYOffset*)temp;
    //stACE_PicSetHue* pACE_PICSETHUE = (stACE_PicSetHue*)temp;
    //stACE_SkipWaitVsync* pACE_SKIPWAITVSYNC = (stACE_SkipWaitVsync*)temp;
    //stACE_PicSetSaturation* pACE_PICSETSATURATION = (stACE_PicSetSaturation*)temp;
    //stACE_PicSetSharpness* pACE_PICSETSHARPNESS = (stACE_PicSetSharpness*)temp;
    stACE_PicSetColorTemp* pACE_PICSETCOLORTEMP = (stACE_PicSetColorTemp*)temp;
    stACE_PicSetColorTempEx* pACE_PICSETCOLORTEMPEX = (stACE_PicSetColorTempEx*)temp;
    stACE_PicSetPostColorTemp_V02* pACE_PICSETPOSTCOLORTEMP_V02 = (stACE_PicSetPostColorTemp_V02*)temp;
    //stACE_SetFleshTone* pACE_SETFLESHTONE = (stACE_SetFleshTone*)temp;
    //stACE_SetBlackAdjust* pACE_SETBLACKADJUST = (stACE_SetBlackAdjust*)temp;
    //stACE_Set_IHC_SRAM* pACE_SET_IHC_SRAM = (stACE_Set_IHC_SRAM*)temp;
    //stACE_Set_ICC_SRAM* pACE_SET_ICC_SRAM = (stACE_Set_ICC_SRAM*)temp;
    //stACE_EnableMWE* pACE_ENABLEMWE = (stACE_EnableMWE*)temp;
    //stACE_MWESetMirrorMode* pACE_MWESETMIRRORMODE = (stACE_MWESetMirrorMode*)temp;
    stACE_MWESetRegTable* pACE_MWESETREGTABLE = (stACE_MWESetRegTable*)temp;
    //stACE_SetMWELoadFromPQ* pACE_SETMWELOADFROMPQ = (stACE_SetMWELoadFromPQ*)temp;
    //stACE_MWEStatus* pACE_MWESTATUS = (stACE_MWEStatus*)temp;
    //stACE_MWEFuncSel* pACE_MWEFUNCSEL = (stACE_MWEFuncSel*)temp;
    //stACE_MWESetDispWin* pACE_MWESETDISPWIN = (stACE_MWESetDispWin*)temp;
    //stACE_3DClonePQMap* pACE_3DCLONEPQMAP = (stACE_3DClonePQMap*)temp;
    //stACE_DNR_Blending_NRTbl* pACE_DNR_BLENDING_NRTBL = (stACE_DNR_Blending_NRTbl*)temp;
    //stACE_DNR_Blending_MISC* pACE_DNR_BLENDING_MISC = (stACE_DNR_Blending_MISC*)temp;
    //stACE_DNR_Blending* pACE_DNR_BLENDING = (stACE_DNR_Blending*)temp;
    //stACE_DNR_Get_PrecisionShift* pACE_DNR_GET_PRECISIONSHIFT = (stACE_DNR_Get_PrecisionShift*)temp;
    //stACE_DNR_GetMotion_Weight* pACE_DNR_GETMOTION_WEIGHT = (stACE_DNR_GetMotion_Weight*)temp;
    //stACE_DNR_GetLuma_Weight* pACE_DNR_GETLUMA_WEIGHT = (stACE_DNR_GetLuma_Weight*)temp;
    //stACE_DNR_GetNoise_Weight* pACE_DNR_GETNOISE_WEIGHT = (stACE_DNR_GetNoise_Weight*)temp;
    //stACE_DNR_GetMotion* pACE_DNR_GETMOTION = (stACE_DNR_GetMotion*)temp;
    //stACE_DNR_SetParam* pACE_DNR_SETPARAM = (stACE_DNR_SetParam*)temp;
    //stACE_DNR_GetParam* pACE_DNR_GETPARAM = (stACE_DNR_GetParam*)temp;
    //stACE_DNR_SetNRTbl* pACE_DNR_SETNRTBL = (stACE_DNR_SetNRTbl*)temp;
    stACE_GetInfo* pACE_GETINFO = (stACE_GetInfo*)temp;
    stACE_GetStatus* pACE_GETSTATUS = (stACE_GetStatus*)temp;
    //stACE_SetDbgLevel* pACE_SETDBGLEVEL = (stACE_SetDbgLevel*)temp;
    //stACE_SetColorCorrectionTable* pACE_COLORCORRECTIONTABLE = (stACE_SetColorCorrectionTable*)temp;
    //stACE_SetColorMatrixControl* pACE_SETCOLORMATRIXCONTROL = (stACE_SetColorMatrixControl*)temp;
    //stACE_SetRBChannelRange* pACE_SETRBCHANNELRANGE = (stACE_SetRBChannelRange*)temp;
    stACE_PicSetPostColorTemp* pACE_PICSETPOSTCOLORTEMP = (stACE_PicSetPostColorTemp*)temp;
    stACE_PicSetPostColorTempEx* pACE_PICSETPOSTCOLORTEMPEX = (stACE_PicSetPostColorTempEx*)temp;
    stACE_PicSetPostColorTemp* pACE_PICSETPOSTCOLORTEMP2 = (stACE_PicSetPostColorTemp*)temp;
    stACE_PicSetPostColorTempEx* pACE_PICSETPOSTCOLORTEMP2EX = (stACE_PicSetPostColorTempEx*)temp;
    //stACE_Set_PowerState* pACE_SET_POWERSTATE = (stACE_Set_PowerState*)temp;

    //Function parameter's pointer
    //XC_ACE_InitData* pDrvACE_InitData = (XC_ACE_InitData*)temp;
    //XC_ACE_InfoEx* pDrvACE_InfoEx = (XC_ACE_InfoEx*)temp;
    //XC_ACE_color_temp* pDrvACE_color_temp = (XC_ACE_color_temp*)temp;
    //XC_ACE_color_temp_ex* pDrvACE_color_temp_ex = (XC_ACE_color_temp_ex*)temp;
    //XC_ACE_color_temp_ex2* pDrvACE_color_temp_ex2 = (XC_ACE_color_temp_ex2*)temp;
    //XC_ACE_TAB_Info* pDrvACE_TAB_Info = (XC_ACE_TAB_Info*)temp;
    //XC_ACE_ApiStatus* pDrvACE_ApiStatus = (XC_ACE_ApiStatus*)temp;
    //XC_ACE_ApiInfo* pDrvACE_ApiInfo = (XC_ACE_ApiInfo*)temp;

    //Function parameter
    UADPBypassSetSPT(&spt_ACE_YUVtoRGBMatrix[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_S16)*9);
    UADPBypassSetSPT(&spt_ACE_YUVtoRGBMatrix[1],UADP_SPT_END , 0, 0);

    UADP_SPT_BGN(&spt_ColorCorrectionTable[0],sizeof(MS_S16) * 32);
    UADP_SPT_FIN(&spt_ColorCorrectionTable[1]);
    UADP_SPT_NAME2NXT(XC_ACE_InitData,XC_ACE_InitData,S16ColorCorrectionMatrix,ColorCorrectionTable,S16RGB,ACE_YUVtoRGBMatrix);
    UADP_SPT_NAME1NXT(ACE_INIT,stACE_Init,pstXC_ACE_InitData,XC_ACE_InitData);

    UADPBypassSetSPT(&spt_ACE_EXIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_Exit));
    UADPBypassSetSPT(&spt_ACE_EXIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DMS[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DMS));
    UADPBypassSetSPT(&spt_ACE_DMS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETPCYUV2RGB[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetPCYUV2RGB));
    UADPBypassSetSPT(&spt_ACE_SETPCYUV2RGB[1],UADP_SPT_END , 0, 0);

    UADP_SPT_NAME1NXT(ACE_SELECTYUVTORGBMATRIX,stACE_SelectYUVtoRGBMatrix,psUserYUVtoRGBMatrix,ACE_YUVtoRGBMatrix);

    UADPBypassSetSPT(&spt_ACE_SETCOLORCORRECTIONTABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetColorCorrectionTable));
    UADPBypassSetSPT(&spt_ACE_SETCOLORCORRECTIONTABLE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETPCSRGBTABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetPCsRGBTable));
    UADPBypassSetSPT(&spt_ACE_SETPCSRGBTABLE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_GETCOLORMATRIX[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_ColorMatrix));
    UADPBypassSetSPT(&spt_ACE_GETCOLORMATRIX[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETCOLORMATRIX[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_ColorMatrix));
    UADPBypassSetSPT(&spt_ACE_SETCOLORMATRIX[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETBYPASSCOLORMATRIX[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetBypassColorMatrix));
    UADPBypassSetSPT(&spt_ACE_SETBYPASSCOLORMATRIX[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETIHC[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetIHC));
    UADPBypassSetSPT(&spt_ACE_SETIHC[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PATCHDTGCOLORCHECKER[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PatchDTGColorChecker));
    UADPBypassSetSPT(&spt_ACE_PATCHDTGCOLORCHECKER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_GETACEINFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_GetACEInfo));
    UADPBypassSetSPT(&spt_ACE_GETACEINFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_GETACEINFOEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_GetACEInfoEx));
    UADPBypassSetSPT(&spt_ACE_GETACEINFOEX[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_GETACEINFOEX->pInfoEx)) - (unsigned long)pACE_GETACEINFOEX), (unsigned long)&spt_DrvACE_InfoEx);
    UADPBypassSetSPT(&spt_ACE_GETACEINFOEX[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETCONTRAST[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetContrast));
    UADPBypassSetSPT(&spt_ACE_PICSETCONTRAST[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETBRIGHTNESS[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetBrightness));
    UADPBypassSetSPT(&spt_ACE_PICSETBRIGHTNESS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETBRIGHTNESSPRECISE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetBrightnessPrecise));
    UADPBypassSetSPT(&spt_ACE_PICSETBRIGHTNESSPRECISE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSET_PRE_Y_OFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PreYOffset));
    UADPBypassSetSPT(&spt_ACE_PICSET_PRE_Y_OFFSET[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICGET_PRE_Y_OFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PreYOffset));
    UADPBypassSetSPT(&spt_ACE_PICGET_PRE_Y_OFFSET[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETHUE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetHue));
    UADPBypassSetSPT(&spt_ACE_PICSETHUE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SKIPWAITVSYNC[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SkipWaitVsync));
    UADPBypassSetSPT(&spt_ACE_SKIPWAITVSYNC[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETSATURATION[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetSaturation));
    UADPBypassSetSPT(&spt_ACE_PICSETSATURATION[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETSHARPNESS[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetSharpness));
    UADPBypassSetSPT(&spt_ACE_PICSETSHARPNESS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETCOLORTEMP[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetColorTemp));
    UADPBypassSetSPT(&spt_ACE_PICSETCOLORTEMP[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_PICSETCOLORTEMP->pstColorTemp)) - (unsigned long)pACE_PICSETCOLORTEMP), (unsigned long)&spt_DrvACE_Color_Temp);
    UADPBypassSetSPT(&spt_ACE_PICSETCOLORTEMP[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETCOLORTEMPEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetColorTempEx));
    UADPBypassSetSPT(&spt_ACE_PICSETCOLORTEMPEX[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_PICSETCOLORTEMPEX->pstColorTemp)) - (unsigned long)pACE_PICSETCOLORTEMPEX), (unsigned long)&spt_DrvACE_Color_TempEx);
    UADPBypassSetSPT(&spt_ACE_PICSETCOLORTEMPEX[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP_V02[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetPostColorTemp_V02));
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP_V02[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_PICSETPOSTCOLORTEMP_V02->pstColorTemp)) - (unsigned long)pACE_PICSETPOSTCOLORTEMP_V02), (unsigned long)&spt_DrvACE_Color_TempEx2);
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP_V02[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETFLESHTONE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetFleshTone));
    UADPBypassSetSPT(&spt_ACE_SETFLESHTONE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETBLACKADJUST[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetBlackAdjust));
    UADPBypassSetSPT(&spt_ACE_SETBLACKADJUST[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SET_IHC_SRAM[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_Set_IHC_SRAM));
    UADPBypassSetSPT(&spt_ACE_SET_IHC_SRAM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SET_ICC_SRAM[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_Set_ICC_SRAM));
    UADPBypassSetSPT(&spt_ACE_SET_ICC_SRAM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_ENABLEMWE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_EnableMWE));
    UADPBypassSetSPT(&spt_ACE_ENABLEMWE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_MWESETMIRRORMODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_MWESetMirrorMode));
    UADPBypassSetSPT(&spt_ACE_MWESETMIRRORMODE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_MWESETREGTABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_MWESetRegTable));
    UADPBypassSetSPT(&spt_ACE_MWESETREGTABLE[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_MWESETREGTABLE->pMWETable)) - (unsigned long)pACE_MWESETREGTABLE), (unsigned long)&spt_DrvACE_TAB_Info);
    UADPBypassSetSPT(&spt_ACE_MWESETREGTABLE[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETMWELOADFROMPQ[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetMWELoadFromPQ));
    UADPBypassSetSPT(&spt_ACE_SETMWELOADFROMPQ[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_MWESTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_MWEStatus));
    UADPBypassSetSPT(&spt_ACE_MWESTATUS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_MWEFUNCSEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_MWEFuncSel));
    UADPBypassSetSPT(&spt_ACE_MWEFUNCSEL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_MWESETDISPWIN[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_MWESetDispWin));
    UADPBypassSetSPT(&spt_ACE_MWESETDISPWIN[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_3DCLONEPQMAP[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_3DClonePQMap));
    UADPBypassSetSPT(&spt_ACE_3DCLONEPQMAP[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_BLENDING_NRTBL[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_Blending_NRTbl));
    UADPBypassSetSPT(&spt_ACE_DNR_BLENDING_NRTBL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_BLENDING_MISC[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_Blending_MISC));
    UADPBypassSetSPT(&spt_ACE_DNR_BLENDING_MISC[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_BLENDING[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_Blending));
    UADPBypassSetSPT(&spt_ACE_DNR_BLENDING[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_GET_PRECISIONSHIFT[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_Get_PrecisionShift));
    UADPBypassSetSPT(&spt_ACE_DNR_GET_PRECISIONSHIFT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_GETMOTION_WEIGHT[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_GetMotion_Weight));
    UADPBypassSetSPT(&spt_ACE_DNR_GETMOTION_WEIGHT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_GETLUMA_WEIGHT[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_GetLuma_Weight));
    UADPBypassSetSPT(&spt_ACE_DNR_GETLUMA_WEIGHT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_GETNOISE_WEIGHT[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_GetNoise_Weight));
    UADPBypassSetSPT(&spt_ACE_DNR_GETNOISE_WEIGHT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_GETMOTION[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_GetMotion));
    UADPBypassSetSPT(&spt_ACE_DNR_GETMOTION[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_SETPARAM[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_SetParam));
    UADPBypassSetSPT(&spt_ACE_DNR_SETPARAM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_GETPARAM[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_GetParam));
    UADPBypassSetSPT(&spt_ACE_DNR_GETPARAM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_DNR_SETNRTBL[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_DNR_SetNRTbl));
    UADPBypassSetSPT(&spt_ACE_DNR_SETNRTBL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_GETINFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_GetInfo));
    UADPBypassSetSPT(&spt_ACE_GETINFO[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_GETINFO->pstApiInfo)) - (unsigned long)pACE_GETINFO), (unsigned long)&spt_DrvACE_ApiInfo);
    UADPBypassSetSPT(&spt_ACE_GETINFO[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_GETSTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_GetStatus));
    UADPBypassSetSPT(&spt_ACE_GETSTATUS[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_GETSTATUS->pDrvStatus)) - (unsigned long)pACE_GETSTATUS), (unsigned long)&spt_DrvACE_ApiStatus);
    UADPBypassSetSPT(&spt_ACE_GETSTATUS[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETDBGLEVEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetDbgLevel));
    UADPBypassSetSPT(&spt_ACE_SETDBGLEVEL[1],UADP_SPT_END , 0, 0);

    UADP_SPT_NAME1NXT(ACE_COLORCORRECTIONTABLE,stACE_ColorCorrectionTable,psColorCorrectionTable,ColorCorrectionTable);

    UADPBypassSetSPT(&spt_ACE_SETCOLORMATRIXCONTROL[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetColorMatrixControl));
    UADPBypassSetSPT(&spt_ACE_SETCOLORMATRIXCONTROL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SETRBCHANNELRANGE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_SetRBChannelRange));
    UADPBypassSetSPT(&spt_ACE_SETRBCHANNELRANGE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetPostColorTemp));
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_PICSETPOSTCOLORTEMP->pstColorTemp)) - (unsigned long)pACE_PICSETPOSTCOLORTEMP), (unsigned long)&spt_DrvACE_Color_Temp);
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMPEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetPostColorTempEx));
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMPEX[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_PICSETPOSTCOLORTEMPEX->pstColorTemp)) - (unsigned long)pACE_PICSETPOSTCOLORTEMPEX), (unsigned long)&spt_DrvACE_Color_TempEx);
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMPEX[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP2[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetPostColorTemp));
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP2[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_PICSETPOSTCOLORTEMP2->pstColorTemp)) - (unsigned long)pACE_PICSETPOSTCOLORTEMP2), (unsigned long)&spt_DrvACE_Color_Temp);
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP2[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP2EX[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_PicSetPostColorTempEx));
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP2EX[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pACE_PICSETPOSTCOLORTEMP2EX->pstColorTemp)) - (unsigned long)pACE_PICSETPOSTCOLORTEMP2EX), (unsigned long)&spt_DrvACE_Color_TempEx);
    UADPBypassSetSPT(&spt_ACE_PICSETPOSTCOLORTEMP2EX[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_ACE_SET_POWERSTATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stACE_Set_PowerState));
    UADPBypassSetSPT(&spt_ACE_SET_POWERSTATE[1],UADP_SPT_END , 0, 0);


    //Function parameter's pointer
    UADPBypassSetSPT(&spt_DrvACE_InitData[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_InitData));
    UADPBypassSetSPT(&spt_DrvACE_InitData[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DrvACE_InfoEx[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_InfoEx));
    UADPBypassSetSPT(&spt_DrvACE_InfoEx[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DrvACE_Color_Temp[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_color_temp));
    UADPBypassSetSPT(&spt_DrvACE_Color_Temp[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DrvACE_Color_TempEx[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_color_temp_ex));
    UADPBypassSetSPT(&spt_DrvACE_Color_TempEx[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DrvACE_Color_TempEx2[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_color_temp_ex2));
    UADPBypassSetSPT(&spt_DrvACE_Color_TempEx2[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DrvACE_TAB_Info[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_TAB_Info));
    UADPBypassSetSPT(&spt_DrvACE_TAB_Info[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DrvACE_ApiStatus[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_ApiStatus));
    UADPBypassSetSPT(&spt_DrvACE_ApiStatus[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DrvACE_ApiInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_ACE_ApiInfo));
    UADPBypassSetSPT(&spt_DrvACE_ApiInfo[1],UADP_SPT_END , 0, 0);

    *pIoctl= (FUtopiaIOctl)ACE_adp_Ioctl;

    return 0;

}

MS_U32 ACE_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0xFF;
    char buffer_arg[2048];
    switch(u32Cmd)
    {

        case E_ACE_CMD_INIT :
        {
#ifdef CONFIG_COMPAT
            stACE_Init tmp;
            _cmp_stACE_Init tmp_compat;
            if(IS_CMP_TASK())
            {
                _cmp_XC_ACE_InitData *ptr;
                void *ptr1;
                void *ptr2;
                CMP_CPY_FROM_USER(&tmp_compat, pArgs, sizeof(_cmp_stACE_Init));
                ptr = tmp_compat.pstXC_ACE_InitData;
                tmp.pstXC_ACE_InitData = malloc(sizeof(_cmp_XC_ACE_InitData));
                tmp.u32InitDataLen = tmp_compat.u32InitDataLen;
                tmp.bReturn = tmp_compat.bReturn;
                CMP_CPY_FROM_USER(tmp.pstXC_ACE_InitData,ptr,sizeof(_cmp_XC_ACE_InitData));

                ptr1 = tmp.pstXC_ACE_InitData->S16ColorCorrectionMatrix;
                ptr2 = tmp.pstXC_ACE_InitData->S16RGB;
                tmp.pstXC_ACE_InitData->S16ColorCorrectionMatrix = malloc(sizeof(MS_S16)*32);
                tmp.pstXC_ACE_InitData->S16RGB = malloc(sizeof(MS_S16)*9);
                CMP_CPY_FROM_USER(tmp.pstXC_ACE_InitData->S16ColorCorrectionMatrix,ptr1,sizeof(MS_S16)*32);
                CMP_CPY_FROM_USER(tmp.pstXC_ACE_InitData->S16RGB,ptr2,sizeof(MS_S16)*9);

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp);
                tmp_compat.bReturn = tmp.bReturn;

                CMP_CPY_TO_USER(pArgs,&tmp_compat,sizeof(_cmp_stACE_Init));
            }else
#endif
            {
                u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_INIT, spt_ACE_INIT, buffer_arg,sizeof(buffer_arg));
            }
            break;
        }

        case E_ACE_CMD_EXIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, spt_ACE_EXIT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DMS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DMS, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETPCYUV2RGB:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETPCYUV2RGB, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SELECTYUVTORGBMATRIX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SELECTYUVTORGBMATRIX, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETCOLORCORRECTIONTABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETCOLORCORRECTIONTABLE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETPCSRGBTABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETPCSRGBTABLE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_GETCOLORMATRIX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_GETCOLORMATRIX, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETCOLORMATRIX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETCOLORMATRIX, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETBYPASSCOLORMATRIX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETBYPASSCOLORMATRIX, spt_ACE_SETBYPASSCOLORMATRIX, buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETIHC:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETIHC, spt_ACE_SETIHC, buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PATCHDTGCOLORCHECKER:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PATCHDTGCOLORCHECKER, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_GETACEINFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_GETACEINFO, spt_ACE_GETACEINFO,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_GETACEINFOEX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_GETACEINFOEX, spt_ACE_GETACEINFOEX,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETCONTRAST:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETCONTRAST, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETBRIGHTNESS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETBRIGHTNESS, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETBRIGHTNESSPRECISE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETBRIGHTNESSPRECISE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSET_PRE_Y_OFFSET:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSET_PRE_Y_OFFSET, spt_ACE_PICSET_PRE_Y_OFFSET,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICGET_PRE_Y_OFFSET:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICGET_PRE_Y_OFFSET, spt_ACE_PICGET_PRE_Y_OFFSET,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETHUE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETHUE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SKIPWAITVSYNC:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SKIPWAITVSYNC, spt_ACE_SKIPWAITVSYNC,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETSATURATION:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETSATURATION, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETSHARPNESS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETSHARPNESS, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETCOLORTEMP:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETCOLORTEMP, spt_ACE_PICSETCOLORTEMP,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETCOLORTEMPEX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETCOLORTEMPEX, spt_ACE_PICSETCOLORTEMPEX,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETPOSTCOLORTEMP_V02:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETPOSTCOLORTEMP_V02, spt_ACE_PICSETPOSTCOLORTEMP_V02,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETFLESHTONE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETFLESHTONE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETBLACKADJUST:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETBLACKADJUST, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SET_IHC_SRAM:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SET_IHC_SRAM, spt_ACE_SET_IHC_SRAM,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SET_ICC_SRAM:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SET_ICC_SRAM, spt_ACE_SET_ICC_SRAM,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_ENABLEMWE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_ENABLEMWE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_MWESETMIRRORMODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_MWESETMIRRORMODE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_MWECLONEVISUALEFFECT:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;

        case E_ACE_CMD_MWESETREGTABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_MWESETREGTABLE, spt_ACE_MWESETREGTABLE,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETMWELOADFROMPQ:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETMWELOADFROMPQ, spt_ACE_SETMWELOADFROMPQ,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_MWESTATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, spt_ACE_MWESTATUS, buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_MWEHANDLE:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;

        case E_ACE_CMD_MWEFUNCSEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_MWEFUNCSEL, NULL,buffer_arg,sizeof(buffer_arg));
            break;


        case E_ACE_CMD_MWESETDISPWIN:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_MWESETDISPWIN, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_3DCLONEPQMAP:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_3DCLONEPQMAP, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_BLENDING_NRTBL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_BLENDING_NRTBL, spt_ACE_DNR_BLENDING_NRTBL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_BLENDING_MISC:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_BLENDING_MISC, spt_ACE_DNR_BLENDING_MISC,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_BLENDING:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_BLENDING, spt_ACE_DNR_BLENDING,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_GET_PRECISIONSHIFT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, spt_ACE_DNR_GET_PRECISIONSHIFT, buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_GETMOTION_WEIGHT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_GETMOTION_WEIGHT, spt_ACE_DNR_GETMOTION_WEIGHT,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_GETLUMA_WEIGHT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_GETLUMA_WEIGHT, spt_ACE_DNR_GETLUMA_WEIGHT,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_GETNOISE_WEIGHT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_GETNOISE_WEIGHT, spt_ACE_DNR_GETNOISE_WEIGHT,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_GETMOTION:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_GETMOTION, spt_ACE_DNR_GETMOTION,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_INIT_MOTION:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;

        case E_ACE_CMD_DNR_INIT_LUMA:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;

        case E_ACE_CMD_DNR_INIT_NOISE:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;

        case E_ACE_CMD_DNR_SETPARAM:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_SETPARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_GETPARAM:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_GETPARAM, spt_ACE_DNR_GETPARAM,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_DNR_SETNRTBL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_DNR_SETNRTBL, spt_ACE_DNR_SETNRTBL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_GETLIBVER:
            // ???
            break;

        case E_ACE_CMD_GETINFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, spt_ACE_GETINFO, buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_GETSTATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_GETSTATUS, spt_ACE_GETSTATUS,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETDBGLEVEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETDBGLEVEL, spt_ACE_SETDBGLEVEL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_COLORCORRECTIONTABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_COLORCORRECTIONTABLE, spt_ACE_COLORCORRECTIONTABLE,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETCOLORMATRIXCONTROL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETCOLORMATRIXCONTROL, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SETRBCHANNELRANGE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SETRBCHANNELRANGE, NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETPOSTCOLORTEMP:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETPOSTCOLORTEMP, spt_ACE_PICSETPOSTCOLORTEMP,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETPOSTCOLORTEMPEX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETPOSTCOLORTEMPEX, spt_ACE_PICSETPOSTCOLORTEMPEX,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETPOSTCOLORTEMP2:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETPOSTCOLORTEMP2, spt_ACE_PICSETPOSTCOLORTEMP2,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_PICSETPOSTCOLORTEMP2EX:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_PICSETPOSTCOLORTEMP2EX, spt_ACE_PICSETPOSTCOLORTEMP2EX,buffer_arg,sizeof(buffer_arg));
            break;

        case E_ACE_CMD_SET_POWERSTATE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_ACE_SET_POWERSTATE, spt_ACE_SET_POWERSTATE,buffer_arg,sizeof(buffer_arg));
            break;

        default:
            break;

    }

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




