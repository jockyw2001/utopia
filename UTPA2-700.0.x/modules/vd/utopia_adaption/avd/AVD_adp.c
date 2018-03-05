#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>




#include "MsTypes.h"
#include "utopia.h"
#include "drvAVD.h"
#include "drvAVD_v2.h"
//#include "drvAVD_priv.h"
#include "AVD_adp.h"


#include "utopia_adp.h"
//#include "MsOS.h"



//#include <linux/kernel.h>




//Top
UADP_STRUCT_POINTER_TABLE spt_AVD_CopyToUser[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_pu8VD_DSP[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_stCapWin[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_u16LatchStatus[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_MSIF_Version[10];
UADP_STRUCT_POINTER_TABLE u8AVDValue[10];
UADP_STRUCT_POINTER_TABLE u16AVDValue[10];
UADP_STRUCT_POINTER_TABLE u32AVDValue[10];

UADP_STRUCT_POINTER_TABLE bAVDEnable[10];
UADP_STRUCT_POINTER_TABLE eVideoFreq[10];
UADP_STRUCT_POINTER_TABLE eFreerunfreq[10];
//
  UADP_STRUCT_POINTER_TABLE spt_AVD_GetStandardDetection[10];
//

UADP_STRUCT_POINTER_TABLE spt_AVD_INIT[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_LOADDSP[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_BACKPORCHWINPOS[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_SETREGVALUE[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_SETINPUT[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_SETVIDEOSTANDARD[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_SETSHIFTCLK[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_SETFACTORYPARA[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_SET3DCOMBSPEED[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_GETCAPTUREWINDOW[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_SETBURSTWINSTART[10];

//Bottom
UADP_STRUCT_POINTER_TABLE spt_AVD_VdDecInitializeExt[10];
UADP_STRUCT_POINTER_TABLE spt_AVD_VD_DSP_Code_Address[10];

UADP_STRUCT_POINTER_TABLE spt_DrvAVD_VD_INITDATA[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_Still_Image_Param[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_SCAN_HSYNC_CHECK[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_FORCEVIDEOSTANDARD[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_SETDBGLEVEL[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_MBXREADBYTEBYVDMBOX[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_GETREGVALUE[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_SETPOWERSTATE[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAVD_AVD_GETLIBVER[10];

MS_U32 AVD_adp_Init(FUtopiaIOctl* pIoctl)
{

//set table
    printk("AVD_adp_Init\n");

    UADP_SPT_BGN(&spt_AVD_pu8VD_DSP[0],sizeof(MS_U8));
    UADP_SPT_FIN(&spt_AVD_pu8VD_DSP[1]);

    UADP_SPT_BGN(&spt_AVD_stCapWin[0],sizeof(void));
    UADP_SPT_FIN(&spt_AVD_stCapWin[1]);

    UADP_SPT_BGN(&spt_AVD_u16LatchStatus[0],sizeof(MS_U16));
    UADP_SPT_FIN(&spt_AVD_u16LatchStatus[1]);

    UADP_SPT_BGN(&spt_AVD_GetStandardDetection[0], sizeof(AVD_GETSTANDARDDETECTION));
    //UADP_SPT_NXT(&spt_AVD_GetStandardDetection[1],AVD_GETSTANDARDDETECTION,vdLatchStatus, spt_AVD_u16LatchStatus);
    UADP_SPT_FIN(&spt_AVD_GetStandardDetection[1]);

    UADP_SPT_BGN(&spt_AVD_MSIF_Version[0],sizeof(MSIF_Version));
    UADP_SPT_FIN(&spt_AVD_MSIF_Version[1]);

//========
    UADP_SPT_BGN(&u8AVDValue[0],sizeof(MS_U8));
    UADP_SPT_FIN(&u8AVDValue[1]);
    UADP_SPT_BGN(&u16AVDValue[0],sizeof(MS_U16));
    UADP_SPT_FIN(&u16AVDValue[1]);
    UADP_SPT_BGN(&u32AVDValue[0],sizeof(MS_U32));
    UADP_SPT_FIN(&u32AVDValue[1]);
    UADP_SPT_BGN(&bAVDEnable[0],sizeof(MS_BOOL));
    UADP_SPT_FIN(&bAVDEnable[1]);

    UADP_SPT_BGN(&eVideoFreq[0],sizeof(AVD_VideoFreq));
    UADP_SPT_FIN(&eVideoFreq[1]);
    UADP_SPT_BGN(&eFreerunfreq[0],sizeof(AVD_FreeRunFreq));
    UADP_SPT_FIN(&eFreerunfreq[1]);

    UADP_SPT_BGN(&spt_AVD_INIT[0], sizeof(AVD_INIT));
    //UADP_SPT_NXT(&spt_AVD_INIT[1],AVD_INIT,pVD_InitData, spt_DrvAVD_VD_INITDATA);
    UADP_SPT_FIN(&spt_AVD_INIT[1]);

    UADP_SPT_BGN(&spt_AVD_LOADDSP[0], sizeof(AVD_LOADDSP));
    UADP_SPT_NXT(&spt_AVD_LOADDSP[1],AVD_LOADDSP,pu8VD_DSP, spt_AVD_pu8VD_DSP);
    UADP_SPT_FIN(&spt_AVD_LOADDSP[2]);


    UADP_SPT_BGN(&spt_AVD_BACKPORCHWINPOS[0],sizeof(AVD_BACKPORCHWINPOS));
    UADP_SPT_FIN(&spt_AVD_BACKPORCHWINPOS[1]);

    UADP_SPT_BGN(&spt_AVD_SETREGVALUE[0],sizeof(AVD_SETREGVALUE));
    UADP_SPT_FIN(&spt_AVD_SETREGVALUE[1]);

    UADP_SPT_BGN(&spt_AVD_SETBURSTWINSTART[0],sizeof(MS_U8));
    UADP_SPT_FIN(&spt_AVD_SETBURSTWINSTART[1]);

    UADP_SPT_BGN(&spt_AVD_SETINPUT[0],sizeof(AVD_SETINPUT));
    UADP_SPT_FIN(&spt_AVD_SETINPUT[1]);

    UADP_SPT_BGN(&spt_AVD_SETVIDEOSTANDARD[0],sizeof(AVD_SETVIDEOSTANDARD));
    UADP_SPT_FIN(&spt_AVD_SETVIDEOSTANDARD[1]);

    UADP_SPT_BGN(&spt_AVD_SETSHIFTCLK[0], sizeof(AVD_SETSHIFTCLK));
    UADP_SPT_FIN(&spt_AVD_SETSHIFTCLK[1]);

    UADP_SPT_BGN(&spt_AVD_SETFACTORYPARA[0], sizeof(AVD_SETFACTORYPARA));
    UADP_SPT_FIN(&spt_AVD_SETFACTORYPARA[1]);

    UADP_SPT_BGN(&spt_AVD_SET3DCOMBSPEED[0], sizeof(AVD_SET3DCOMBSPEED));
    UADP_SPT_FIN(&spt_AVD_SET3DCOMBSPEED[1]);

    UADP_SDT_BGN(&spt_AVD_GETCAPTUREWINDOW[0], sizeof(AVD_GETCAPTUREWINDOW));
    //UADP_SPT_NXT(&spt_AVD_LOADDSP[1],AVD_GETCAPTUREWINDOW,stCapWin, spt_AVD_stCapWin);
    UADP_SDT_KIND(&spt_AVD_GETCAPTUREWINDOW[1],AVD_GETCAPTUREWINDOW,UADP_SDT_P2N,stCapWin, spt_AVD_stCapWin);
    UADP_SDT_FIN(&spt_AVD_GETCAPTUREWINDOW[2]);

    UADP_SPT_BGN(&spt_AVD_CopyToUser[0],sizeof(AVD_COPYTOUSER));
    UADP_SPT_FIN(&spt_AVD_CopyToUser[1]);

    //bottom
    UADP_SPT_BGN(&spt_AVD_VdDecInitializeExt[0],sizeof(MS_U8));
    UADP_SPT_FIN(&spt_AVD_VdDecInitializeExt[1]);

    UADP_SPT_BGN(&spt_AVD_VD_DSP_Code_Address[0],sizeof(MS_U8));
    UADP_SPT_FIN(&spt_AVD_VD_DSP_Code_Address[1]);

    UADP_SPT_BGN(&spt_DrvAVD_VD_INITDATA[0],sizeof(VD_INITDATA));
    UADP_SPT_NXT(&spt_DrvAVD_VD_INITDATA[1],VD_INITDATA,u8VdDecInitializeExt, spt_AVD_VdDecInitializeExt);
    UADP_SPT_NXT(&spt_DrvAVD_VD_INITDATA[2],VD_INITDATA,pu8VD_DSP_Code_Address, spt_AVD_VD_DSP_Code_Address);
    UADP_SPT_FIN(&spt_DrvAVD_VD_INITDATA[3]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_Still_Image_Param[0],sizeof(AVD_Still_Image_Param));
    UADP_SPT_FIN(&spt_DrvAVD_AVD_Still_Image_Param[1]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_SCAN_HSYNC_CHECK[0],sizeof(AVD_SCANHSYNCCHECK));
    UADP_SPT_FIN(&spt_DrvAVD_AVD_SCAN_HSYNC_CHECK[1]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_FORCEVIDEOSTANDARD[0],sizeof(AVD_FORCEVIDEOSTANDARD));
    UADP_SPT_FIN(&spt_DrvAVD_AVD_FORCEVIDEOSTANDARD[1]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_SETDBGLEVEL[0],sizeof(AVD_SETDBGLEVEL));
    UADP_SPT_FIN(&spt_DrvAVD_AVD_SETDBGLEVEL[1]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_MBXREADBYTEBYVDMBOX[0],sizeof(AVD_MBXREADBYTEBYVDMBOX));
    UADP_SPT_FIN(&spt_DrvAVD_AVD_MBXREADBYTEBYVDMBOX[1]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_GETREGVALUE[0],sizeof(AVD_GETREGVALUE));
    UADP_SPT_FIN(&spt_DrvAVD_AVD_GETREGVALUE[1]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_SETPOWERSTATE[0],sizeof(AVD_SETPOWERSTATE));
    UADP_SPT_FIN(&spt_DrvAVD_AVD_SETPOWERSTATE[1]);

    UADP_SPT_BGN(&spt_DrvAVD_AVD_GETLIBVER[0],sizeof(AVD_GETLIBVER));
    UADP_SPT_NXT(&spt_DrvAVD_AVD_GETLIBVER[1],AVD_GETLIBVER,ppVersion, spt_AVD_MSIF_Version);
    UADP_SPT_FIN(&spt_DrvAVD_AVD_GETLIBVER[2]);

    *pIoctl= (FUtopiaIOctl)AVD_adp_Ioctl;
    return 0;

}

MS_U32 AVD_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[2048];
    switch(u32Cmd)
    {

        case MDrv_CMD_AVD_Init :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_INIT,spt_AVD_INIT,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_Exit:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
        break;
        case MDrv_CMD_AVD_ResetMCU:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_FreezeMCU:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,bAVDEnable,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_Scan_HsyncCheck:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_SCAN_HSYNC_CHECK,spt_DrvAVD_AVD_SCAN_HSYNC_CHECK,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_StartAutoStandardDetection:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
        break;
        case MDrv_CMD_AVD_ForceVideoStandard:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_FORCEVIDEOSTANDARD,spt_DrvAVD_AVD_FORCEVIDEOSTANDARD,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_3DCombSpeedup:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
        break;
        case MDrv_CMD_AVD_LoadDSP :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_LOADDSP,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_BackPorchWindowPositon :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_BACKPORCHWINPOS,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_MBX_ReadByteByVDMbox:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_MBXREADBYTEBYVDMBOX,spt_DrvAVD_AVD_MBXREADBYTEBYVDMBOX,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetFlag:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,u32AVDValue,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetRegValue :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_SETREGVALUE,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetRegFromDSP:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
        break;
        case MDrv_CMD_AVD_SetInput :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_SETINPUT,spt_AVD_SETINPUT,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetVideoStandard :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_SETVIDEOSTANDARD,spt_AVD_SETVIDEOSTANDARD,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetChannelChange:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
        break;
        case MDrv_CMD_AVD_SetHsyncDetectionForTuning:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,bAVDEnable,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_Set3dComb:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,bAVDEnable,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetShiftClk :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_SETSHIFTCLK,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetFreerunPLL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,eVideoFreq,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetFreerunFreq:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,eFreerunfreq,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetFactoryPara :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_SETFACTORYPARA,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_Set_Htt_UserMD :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,u16AVDValue,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetDbgLevel:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_SETDBGLEVEL,spt_DrvAVD_AVD_SETDBGLEVEL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetPQFineTune:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
        break;
        case MDrv_CMD_AVD_Set3dCombSpeed:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_SET3DCOMBSPEED,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetStillImageParam:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_Still_Image_Param,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetAFECD4Factory:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,u8AVDValue,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_Set2D3DPatchOnOff:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,bAVDEnable,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetAutoFineGainToFixed:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,u8AVDValue,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetFlag:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetRegValue:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_GETREGVALUE,spt_DrvAVD_AVD_GETREGVALUE,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetNoiseMag:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetVTotal:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetStandardDetection :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_GetStandardDetection,spt_AVD_GetStandardDetection,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetLastDetectedStandard :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetCaptureWindow :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_GETCAPTUREWINDOW,spt_AVD_GETCAPTUREWINDOW,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetVerticalFreq:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetHsyncEdge:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetDSPFineGain:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetDSPVersion:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetLibVer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_GETLIBVER,spt_DrvAVD_AVD_GETLIBVER,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_IsSyncLocked:
            UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_IsSignalInterlaced:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_IsColorOn:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetPowerState:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DrvAVD_AVD_SETPOWERSTATE,spt_DrvAVD_AVD_SETPOWERSTATE,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetMacroVisionDetect:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_GetCGMSDetect:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_SetBurstWinStart:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AVD_SETBURSTWINSTART,NULL,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_AliveCheck:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        case MDrv_CMD_AVD_IsLockAudioCarrier:
            UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_AVD_CopyToUser,buffer_arg,sizeof(buffer_arg));
        break;
        default:
        break;

    }

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}
