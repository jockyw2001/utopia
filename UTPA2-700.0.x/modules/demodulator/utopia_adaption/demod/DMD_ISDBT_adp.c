#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "utopia.h"
#include "utopia_adp.h"

#include "drvDMD_ISDBT_v2.h"

#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_PTR_VAL[5];

UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_DBG_LEVEL_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_Info[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_INFO_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_ID_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_InitData[6];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_INIT_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_EXIT_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_SET_CONFIG_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_SET_POWER_STATE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_LOCK_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_ModulationMode[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_MODULATION_MODE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_SIGNAL_STRENGTH_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_SIGNAL_QUALITY_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_SnrData[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_SNR_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_BerData[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_PKT_ERR_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_BER_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_CfoData[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GET_FREQ_OFFSET_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_SET_SERIAL_CONTROL_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_IIC_BYPASS_MODE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_SWITCH_SSPI_GPIO_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GPIO_LEVEL_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_GPIO_OUT_ENABLE_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_DO_IQ_SWAP_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_ISDBT_REG_PARAM[5];


MS_U32 DMD_ISDBT_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs);

MS_U32 DMD_ISDBT_adp_Init(FUtopiaIOctl* pIoctl)
{
    printk("DMD_ISDBT_adp_Init\n");

    UADP_SPT_BGN(&spt_DMD_ISDBT_PTR_VAL[0], 8);
    UADP_SPT_FIN(&spt_DMD_ISDBT_PTR_VAL[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_DBG_LEVEL_PARAM[0], sizeof(ISDBT_DBG_LEVEL_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_DBG_LEVEL_PARAM[1]);
    
    UADP_SDT_BGN(&spt_DMD_ISDBT_Info[0], sizeof(DMD_ISDBT_Info));
    UADP_SDT_FIN(&spt_DMD_ISDBT_Info[1]);
    UADP_SDT_BGN(&spt_DMD_ISDBT_GET_INFO_PARAM[0], sizeof(ISDBT_GET_INFO_PARAM));
    UADP_SDT_KIND(&spt_DMD_ISDBT_GET_INFO_PARAM[1], ISDBT_GET_INFO_PARAM,UADP_SDT_P2N,pInfo, spt_DMD_ISDBT_Info);
    UADP_SDT_FIN(&spt_DMD_ISDBT_GET_INFO_PARAM[2]);

    UADP_SPT_BGN(&spt_DMD_ISDBT_ID_PARAM[0], sizeof(ISDBT_ID_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_ID_PARAM[1]);
    
    UADP_SDT_BGN(&spt_DMD_ISDBT_InitData[0], sizeof(DMD_ISDBT_InitData));
    UADP_SDT_KIND(&spt_DMD_ISDBT_InitData[1], DMD_ISDBT_InitData, UADP_SDT_P2N, u8DMD_ISDBT_DSPRegInitExt, spt_DMD_ISDBT_PTR_VAL);
    UADP_SDT_KIND(&spt_DMD_ISDBT_InitData[2], DMD_ISDBT_InitData, UADP_SDT_P2N, u8DMD_ISDBT_InitExt, spt_DMD_ISDBT_PTR_VAL);
    UADP_SDT_KIND(&spt_DMD_ISDBT_InitData[3], DMD_ISDBT_InitData, UADP_SDT_AT, I2C_WriteBytes, spt_DMD_ISDBT_PTR_VAL);
    UADP_SDT_KIND(&spt_DMD_ISDBT_InitData[4], DMD_ISDBT_InitData, UADP_SDT_AT, I2C_ReadBytes, spt_DMD_ISDBT_PTR_VAL);
    UADP_SDT_FIN(&spt_DMD_ISDBT_InitData[5]);
    UADP_SDT_BGN(&spt_DMD_ISDBT_INIT_PARAM[0], sizeof(ISDBT_INIT_PARAM));
    UADP_SDT_KIND(&spt_DMD_ISDBT_INIT_PARAM[1], ISDBT_INIT_PARAM,UADP_SDT_P2N,pDMD_ISDBT_InitData, spt_DMD_ISDBT_InitData);
    UADP_SDT_FIN(&spt_DMD_ISDBT_INIT_PARAM[2]);

    UADP_SPT_BGN(&spt_DMD_ISDBT_SET_CONFIG_PARAM[0], sizeof(ISDBT_SET_CONFIG_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_SET_CONFIG_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_SET_POWER_STATE_PARAM[0], sizeof(ISDBT_SET_POWER_STATE_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_SET_POWER_STATE_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_LOCK_PARAM[0], sizeof(ISDBT_GET_LOCK_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_LOCK_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_ISDBT_ModulationMode[0], sizeof(sISDBT_MODULATION_MODE));
    UADP_SPT_FIN(&spt_DMD_ISDBT_ModulationMode[1])
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_MODULATION_MODE_PARAM[0], sizeof(ISDBT_GET_MODULATION_MODE_PARAM));
    UADP_SPT_NXT(&spt_DMD_ISDBT_GET_MODULATION_MODE_PARAM[1], ISDBT_GET_MODULATION_MODE_PARAM, IsdbtModulationMode, spt_DMD_ISDBT_ModulationMode);
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_MODULATION_MODE_PARAM[2]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_SIGNAL_STRENGTH_PARAM[0], sizeof(ISDBT_GET_SIGNAL_STRENGTH_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_SIGNAL_STRENGTH_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_SIGNAL_QUALITY_PARAM[0], sizeof(ISDBT_GET_SIGNAL_QUALITY_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_SIGNAL_QUALITY_PARAM[1]);

    UADP_SPT_BGN(&spt_DMD_ISDBT_SnrData[0], sizeof(DMD_ISDBT_SNR_DATA));
    UADP_SPT_FIN(&spt_DMD_ISDBT_SnrData[1]);
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_SNR_PARAM[0], sizeof(ISDBT_GET_SNR_PARAM));
    UADP_SPT_NXT(&spt_DMD_ISDBT_GET_SNR_PARAM[1], ISDBT_GET_SNR_PARAM, snr, spt_DMD_ISDBT_SnrData);
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_SNR_PARAM[2]);

    UADP_SPT_BGN(&spt_DMD_ISDBT_BerData[0], sizeof(DMD_ISDBT_GET_BER_VALUE));
    UADP_SPT_FIN(&spt_DMD_ISDBT_BerData[1]);
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_BER_PARAM[0], sizeof(ISDBT_GET_BER_PARAM));
    UADP_SPT_NXT(&spt_DMD_ISDBT_GET_BER_PARAM[1], ISDBT_GET_BER_PARAM, ber, spt_DMD_ISDBT_BerData);
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_BER_PARAM[2]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_PKT_ERR_PARAM[0], sizeof(ISDBT_READ_PKT_ERR_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_PKT_ERR_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_GET_FREQ_OFFSET_PARAM[0], sizeof(ISDBT_GET_FREQ_OFFSET_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_GET_FREQ_OFFSET_PARAM[2]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_SET_SERIAL_CONTROL_PARAM[0], sizeof(ISDBT_SET_SERIAL_CONTROL_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_SET_SERIAL_CONTROL_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_IIC_BYPASS_MODE_PARAM[0], sizeof(ISDBT_IIC_BYPASS_MODE_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_IIC_BYPASS_MODE_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_SWITCH_SSPI_GPIO_PARAM[0], sizeof(ISDBT_SWITCH_SSPI_GPIO_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_SWITCH_SSPI_GPIO_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_GPIO_LEVEL_PARAM[0], sizeof(ISDBT_GPIO_LEVEL_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_GPIO_LEVEL_PARAM[1]);
    
    UADP_SPT_BGN(&spt_DMD_ISDBT_GPIO_OUT_ENABLE_PARAM[0], sizeof(ISDBT_GPIO_OUT_ENABLE_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_GPIO_OUT_ENABLE_PARAM[1]);
        
    UADP_SPT_BGN(&spt_DMD_ISDBT_REG_PARAM[0], sizeof(ISDBT_REG_PARAM));
    UADP_SPT_FIN(&spt_DMD_ISDBT_REG_PARAM[1]); 
    
    *pIoctl= (FUtopiaIOctl)DMD_ISDBT_adp_Ioctl;
    
    return 0;
}

MS_U32 DMD_ISDBT_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[2048];
    
    switch(u32Cmd)
    {
        case DMD_ISDBT_DRV_CMD_SetDbgLevel:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_SetDbgLevel\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_DBG_LEVEL_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetInfo:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetInfo\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_DMD_ISDBT_GET_INFO_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetLibVer:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetLibVer\n"));
            u32Ret = TRUE;
            break;
        case DMD_ISDBT_DRV_CMD_Init:
        case DMD_ISDBT_DRV_CMD_MD_Init:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_Init\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_INIT_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_Exit:
        case DMD_ISDBT_DRV_CMD_MD_Exit:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_Exit\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_ID_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetConfig:    
        case DMD_ISDBT_DRV_CMD_MD_GetConfig:
            DMD_DBG(printk("ISDBT_adp_Ioctl - MDrv_DMD_ISDBT_MD_GetConfig\n"));
	        u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_INIT_PARAM, spt_DMD_ISDBT_INIT_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_AdvSetConfig:
        case DMD_ISDBT_DRV_CMD_MD_AdvSetConfig:  
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_SetConfig\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_SET_CONFIG_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_SetPowerState:
        case DMD_ISDBT_DRV_CMD_MD_SetPowerState:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_SetPowerState\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_SET_POWER_STATE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetLock:
        case DMD_ISDBT_DRV_CMD_MD_GetLock:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetLock\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_LOCK_PARAM, spt_DMD_ISDBT_GET_LOCK_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetModulationMode:
        case DMD_ISDBT_DRV_CMD_MD_GetModulationMode:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetModulationMode\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_MODULATION_MODE_PARAM, spt_DMD_ISDBT_GET_MODULATION_MODE_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetSignalStrength:
        case DMD_ISDBT_DRV_CMD_MD_GetSignalStrength:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetSignalStrength\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_SIGNAL_STRENGTH_PARAM, spt_DMD_ISDBT_GET_SIGNAL_STRENGTH_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetSignalQuality:
        case DMD_ISDBT_DRV_CMD_MD_GetSignalQuality:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetSignalQuality\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_SIGNAL_QUALITY_PARAM, spt_DMD_ISDBT_GET_SIGNAL_QUALITY_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetSNR:
        case DMD_ISDBT_DRV_CMD_MD_GetSNR:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetSNR\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_SNR_PARAM, spt_DMD_ISDBT_GET_SNR_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GET_PKT_ERR:
        case DMD_ISDBT_DRV_CMD_MD_Get_PKT_ERR:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_Read_uCPKT_ERR\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_PKT_ERR_PARAM, spt_DMD_ISDBT_GET_PKT_ERR_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetPreViterbiBer:
        case DMD_ISDBT_DRV_CMD_MD_GetPreViterbiBer:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetPreViterbiBer\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_BER_PARAM, spt_DMD_ISDBT_GET_BER_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetPostViterbiBer:
        case DMD_ISDBT_DRV_CMD_MD_GetPostViterbiBer:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetPostViterbiBer\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_BER_PARAM, spt_DMD_ISDBT_GET_BER_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GetFreqOffset:
        case DMD_ISDBT_DRV_CMD_MD_GetFreqOffset:
            DMD_DBG(printk("ISDBTIoctl - DMD_ISDBT_DRV_CMD_GetFreqOffset\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GET_FREQ_OFFSET_PARAM, spt_DMD_ISDBT_GET_FREQ_OFFSET_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_SetSerialControl:
        case DMD_ISDBT_DRV_CMD_MD_SetSerialControl:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_SetSerialControl\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_SET_SERIAL_CONTROL_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_IIC_BYPASS_MODE:
        case DMD_ISDBT_DRV_CMD_MD_IIC_BYPASS_MODE:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_IIC_BYPASS_MODE\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_IIC_BYPASS_MODE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_SWITCH_SSPI_GPIO:
        case DMD_ISDBT_DRV_CMD_MD_SWITCH_SSPI_GPIO:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_SWITCH_SSPI_GPIO\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_SWITCH_SSPI_GPIO_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GPIO_GET_LEVEL:
        case DMD_ISDBT_DRV_CMD_MD_GPIO_GET_LEVEL:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GPIO_GET_LEVEL\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GPIO_LEVEL_PARAM, spt_DMD_ISDBT_GPIO_LEVEL_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GPIO_SET_LEVEL:
        case DMD_ISDBT_DRV_CMD_MD_GPIO_SET_LEVEL:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GPIO_SET_LEVEL\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GPIO_LEVEL_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_GPIO_OUT_ENABLE:
        case DMD_ISDBT_DRV_CMD_MD_GPIO_OUT_ENABLE:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GPIO_OUT_ENABLE\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_GPIO_OUT_ENABLE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_DoIQSwap:
        case DMD_ISDBT_DRV_CMD_MD_DoIQSwap:
            DMD_DBG(printk("ISDBT_adp_Ioctl - MDrv_DMD_ISDBT_MD_DO_IQ_SWAP\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_DO_IQ_SWAP_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case DMD_ISDBT_DRV_CMD_GetReg:
        case DMD_ISDBT_DRV_CMD_MD_GetReg:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_GetReg\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_REG_PARAM, spt_DMD_ISDBT_REG_PARAM, buffer_arg, sizeof(buffer_arg));
            break;
        case DMD_ISDBT_DRV_CMD_SetReg:
        case DMD_ISDBT_DRV_CMD_MD_SetReg:
            DMD_DBG(printk("ISDBTIoctl - MDrv_DMD_ISDBT_SetReg\n"));
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DMD_ISDBT_REG_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        default:
            break;
    }

    return u32Ret;
}
