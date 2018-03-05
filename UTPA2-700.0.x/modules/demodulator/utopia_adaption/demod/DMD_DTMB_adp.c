#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "utopia.h"
#include "utopia_adp.h"

#include "drvDMD_DTMB_v2.h"

#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_PTR_VAL[5];

UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_DBG_LEVEL_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_Info[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GET_INFO_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_ID_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_InitData[10];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_INIT_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_SET_CONFIG_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_SET_QAM_SR_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_SET_ACTIVE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_SET_POWER_STATE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GET_LOCK_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GET_MODULATION_MODE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GET_SIGNAL_STRENGTH_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_READ_FREQ_OFFSET_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GET_SIGNAL_QUALITY_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GET_BER_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GET_SNR_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_SET_SERIAL_CONTROL_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_IIC_BYPASS_MODE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_SWITCH_SSPI_GPIO_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GPIO_LEVEL_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_GPIO_OUT_ENABLE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_DO_IQSWAP_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DTMB_REG_PARAM[5];

MS_U32 DTMB_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs);

MS_U32 DTMB_adp_Init(FUtopiaIOctl* pIoctl)
{
    DMD_DBG(printk("DTMB_adp_Init\n"));

    UADP_SPT_BGN(&spt_DMD_DTMB_PTR_VAL[0], 8);
    UADP_SPT_FIN(&spt_DMD_DTMB_PTR_VAL[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_DBG_LEVEL_PARAM[0], sizeof(DTMB_DBG_LEVEL_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_DBG_LEVEL_PARAM[1]);
    
    UADP_SDT_BGN(&spt_DMD_DTMB_Info[0], sizeof(DMD_DTMB_Info));
    UADP_SDT_FIN(&spt_DMD_DTMB_Info[1]);
    UADP_SDT_BGN(&spt_DMD_DTMB_GET_INFO_PARAM[0], sizeof(DTMB_GET_INFO_PARAM));
    UADP_SDT_KIND(&spt_DMD_DTMB_GET_INFO_PARAM[1], DTMB_GET_INFO_PARAM, UADP_SDT_P2N, pInfo, spt_DMD_DTMB_Info);
    UADP_SDT_FIN(&spt_DMD_DTMB_GET_INFO_PARAM[2]);

    UADP_SPT_BGN(&spt_DMD_DTMB_ID_PARAM[0], sizeof(DTMB_ID_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_ID_PARAM[1]);
    
    UADP_SDT_BGN(&spt_DMD_DTMB_InitData[0], sizeof(DMD_DTMB_InitData));
    UADP_SDT_KIND(&spt_DMD_DTMB_InitData[1], DMD_DTMB_InitData, UADP_SDT_P2N, u8DMD_DTMB_DSPRegInitExt, spt_DMD_DTMB_PTR_VAL);
    UADP_SDT_KIND(&spt_DMD_DTMB_InitData[2], DMD_DTMB_InitData, UADP_SDT_P2N, u8DMD_DTMB_InitExt, spt_DMD_DTMB_PTR_VAL);
    UADP_SDT_KIND(&spt_DMD_DTMB_InitData[3], DMD_DTMB_InitData, UADP_SDT_AT, I2C_WriteBytes, spt_DMD_DTMB_PTR_VAL);
    UADP_SDT_KIND(&spt_DMD_DTMB_InitData[4], DMD_DTMB_InitData, UADP_SDT_AT, I2C_ReadBytes, spt_DMD_DTMB_PTR_VAL);
    UADP_SDT_FIN(&spt_DMD_DTMB_InitData[5]);
    UADP_SDT_BGN(&spt_DMD_DTMB_INIT_PARAM[0], sizeof(DTMB_INIT_PARAM));
    UADP_SDT_KIND(&spt_DMD_DTMB_INIT_PARAM[1], DTMB_INIT_PARAM, UADP_SDT_P2N, pDMD_DTMB_InitData, spt_DMD_DTMB_InitData);
    UADP_SDT_FIN(&spt_DMD_DTMB_INIT_PARAM[2]);
    
    UADP_SPT_BGN(&spt_DMD_DTMB_SET_CONFIG_PARAM[0], sizeof(DTMB_SET_CONFIG_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_SET_CONFIG_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_SET_QAM_SR_PARAM[0], sizeof(DTMB_SET_QAM_SR_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_SET_QAM_SR_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_SET_ACTIVE_PARAM[0], sizeof(DTMB_SET_ACTIVE_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_SET_ACTIVE_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_SET_POWER_STATE_PARAM[0], sizeof(DTMB_SET_POWER_STATE_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_SET_POWER_STATE_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GET_LOCK_PARAM[0], sizeof(DTMB_GET_LOCK_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GET_LOCK_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GET_MODULATION_MODE_PARAM[0], sizeof(DTMB_GET_MODULATION_MODE_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GET_MODULATION_MODE_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GET_SIGNAL_STRENGTH_PARAM[0], sizeof(DTMB_GET_SIGNAL_STRENGTH_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GET_SIGNAL_STRENGTH_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_READ_FREQ_OFFSET_PARAM[0], sizeof(DTMB_READ_FREQ_OFFSET_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_READ_FREQ_OFFSET_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GET_SIGNAL_QUALITY_PARAM[0], sizeof(DTMB_GET_SIGNAL_QUALITY_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GET_SIGNAL_QUALITY_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GET_BER_PARAM[0], sizeof(DTMB_GET_BER_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GET_BER_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GET_SNR_PARAM[0], sizeof(DTMB_GET_SNR_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GET_SNR_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_SET_SERIAL_CONTROL_PARAM[0], sizeof(DTMB_SET_SERIAL_CONTROL_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_SET_SERIAL_CONTROL_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_IIC_BYPASS_MODE_PARAM[0], sizeof(DTMB_IIC_BYPASS_MODE_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_IIC_BYPASS_MODE_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_SWITCH_SSPI_GPIO_PARAM[0], sizeof(DTMB_SWITCH_SSPI_GPIO_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_SWITCH_SSPI_GPIO_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GPIO_LEVEL_PARAM[0], sizeof(DTMB_GPIO_LEVEL_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GPIO_LEVEL_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_GPIO_OUT_ENABLE_PARAM[0], sizeof(DTMB_GPIO_OUT_ENABLE_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_GPIO_OUT_ENABLE_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_DO_IQSWAP_PARAM[0], sizeof(DTMB_DO_IQSWAP_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_DO_IQSWAP_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_DTMB_REG_PARAM[0], sizeof(DTMB_REG_PARAM));
    UADP_SPT_FIN(&spt_DMD_DTMB_REG_PARAM[1]);

    *pIoctl= (FUtopiaIOctl)DTMB_adp_Ioctl;

    return 0;
}

MS_U32 DTMB_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    char buffer_arg[2048];

    switch(u32Cmd)
    {
        case DMD_DTMB_DRV_CMD_SetDbgLevel:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_SetDbgLevel\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_DBG_LEVEL_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetInfo:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_GetInfo\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GET_INFO_PARAM, spt_DMD_DTMB_GET_INFO_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetLibVer:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_GetLibVer\n"));
            break;
        case DMD_DTMB_DRV_CMD_Init:
        case DMD_DTMB_DRV_CMD_MD_Init:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_Init\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_INIT_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_Exit:
        case DMD_DTMB_DRV_CMD_MD_Exit:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_Exit\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_ID_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetConfig:  
        case DMD_DTMB_DRV_CMD_MD_GetConfig:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetConfig\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_INIT_PARAM, spt_DMD_DTMB_INIT_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_SetConfig:
        case DMD_DTMB_DRV_CMD_MD_SetConfig:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_SetConfig\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_SET_CONFIG_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_SetReset:
        case DMD_DTMB_DRV_CMD_MD_SetReset:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_SetReset\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_ID_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_Set_QAM_SR:
        case DMD_DTMB_DRV_CMD_MD_Set_QAM_SR:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_Set_QAM_SR\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_SET_QAM_SR_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_SetActive:
        case DMD_DTMB_DRV_CMD_MD_SetActive:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_SetActive\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_SET_ACTIVE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_SetPowerState:
        case DMD_DTMB_DRV_CMD_MD_SetPowerState:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_SetPowerState\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_SET_POWER_STATE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetLock:
        case DMD_DTMB_DRV_CMD_MD_GetLock:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetLock\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GET_LOCK_PARAM, spt_DMD_DTMB_GET_LOCK_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetModulationMode:
        case DMD_DTMB_DRV_CMD_MD_GetModulationMode:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetModulationMode\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GET_MODULATION_MODE_PARAM, spt_DMD_DTMB_GET_MODULATION_MODE_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetSignalStrength:
        case DMD_DTMB_DRV_CMD_MD_GetSignalStrength:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_GetSignalStrength\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GET_SIGNAL_STRENGTH_PARAM, spt_DMD_DTMB_GET_SIGNAL_STRENGTH_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_ReadFrequencyOffset:
        case DMD_DTMB_DRV_CMD_MD_ReadFrequencyOffset:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_ReadFrequencyOffset\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_READ_FREQ_OFFSET_PARAM, spt_DMD_DTMB_READ_FREQ_OFFSET_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetSignalQuality:
        case DMD_DTMB_DRV_CMD_MD_GetSignalQuality:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetSignalQuality\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GET_SIGNAL_QUALITY_PARAM, spt_DMD_DTMB_GET_SIGNAL_QUALITY_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetPreLdpcBer:
        case DMD_DTMB_DRV_CMD_MD_GetPreLdpcBer:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetPreLdpcBer\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GET_BER_PARAM, spt_DMD_DTMB_GET_BER_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetPreViterbiBer:
        case DMD_DTMB_DRV_CMD_MD_GetPreViterbiBer:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetPreViterbiBer\n"));
            break;
        case DMD_DTMB_DRV_CMD_GetPostViterbiBer:
        case DMD_DTMB_DRV_CMD_MD_GetPostViterbiBer:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetPostViterbiBer\n"));
            break;
        case DMD_DTMB_DRV_CMD_GetSNR:
        case DMD_DTMB_DRV_CMD_MD_GetSNR:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_DM_GetSNR\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GET_SNR_PARAM, spt_DMD_DTMB_GET_SNR_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_SetSerialControl:
        case DMD_DTMB_DRV_CMD_MD_SetSerialControl:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_SetSerialControl\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_SET_SERIAL_CONTROL_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_IIC_BYPASS_MODE:
        case DMD_DTMB_DRV_CMD_MD_IIC_BYPASS_MODE:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_IIC_BYPASS_MODE\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_IIC_BYPASS_MODE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_SWITCH_SSPI_GPIO:
        case DMD_DTMB_DRV_CMD_MD_SWITCH_SSPI_GPIO:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_SWITCH_SSPI_GPIO\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_SWITCH_SSPI_GPIO_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GPIO_GET_LEVEL:
        case DMD_DTMB_DRV_CMD_MD_GPIO_GET_LEVEL:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GPIO_GET_LEVEL\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GPIO_LEVEL_PARAM, spt_DMD_DTMB_GPIO_LEVEL_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GPIO_SET_LEVEL:
        case DMD_DTMB_DRV_CMD_MD_GPIO_SET_LEVEL:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GPIO_SET_LEVEL\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GPIO_LEVEL_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GPIO_OUT_ENABLE:
        case DMD_DTMB_DRV_CMD_MD_GPIO_OUT_ENABLE:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GPIO_OUT_ENABLE\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_GPIO_OUT_ENABLE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_DoIQSwap:
        case DMD_DTMB_DRV_CMD_MD_DoIQSwap:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_DoIQSwap\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_DO_IQSWAP_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_GetReg:
        case DMD_DTMB_DRV_CMD_MD_GetReg:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_GetReg\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_REG_PARAM, spt_DMD_DTMB_REG_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_DTMB_DRV_CMD_SetReg:
        case DMD_DTMB_DRV_CMD_MD_SetReg:
            DMD_DBG(printk("DTMB_adp_Ioctl - MDrv_DMD_DTMB_MD_SetReg\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_DTMB_REG_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        default:
            break;
    }

    return u32Ret;
}
