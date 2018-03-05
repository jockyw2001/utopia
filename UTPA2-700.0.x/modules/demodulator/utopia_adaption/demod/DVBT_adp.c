#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"

#include "drvDMD_INTERN_DVBT_v2.h"

//#include "MsOS.h"

//#include <linux/kernel.h>

//Top
UADP_STRUCT_POINTER_TABLE spt_DVBT_BOOL_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_U8_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_U16_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_U32_VAL[5];
//UADP_STRUCT_POINTER_TABLE spt_DVBT_FLOAT_VAL[5];

UADP_STRUCT_POINTER_TABLE spt_DVBT_DBG_LEVEL_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_EXIT_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBT_InitData[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_INIT_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GETINFO_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBT_Info[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GETINFO_RETURN_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GETREG_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_SETREG_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_SetSerialControl_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_SetConfig_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_SetConfigHPLP_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_SetConfigHPLPSetIF_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_SetActive_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_LOCK_STATUS[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetLock_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetSignalStrength_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetSignalStrengthWithRFPower_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetSignalQuality_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetSignalQualityWithRFPower_PARAM[5];

/******bryan add******/
UADP_STRUCT_POINTER_TABLE spt_DVBT_GETLIBVER_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_MSIF_Version[5];

UADP_STRUCT_POINTER_TABLE spt_DVBT_GETFWVER_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_U16_Val[5];


UADP_STRUCT_POINTER_TABLE spt_PDVBT_GetFreqOffset_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBT_InitData_Transform[5];
/***** end of bryan add*****/

//arthur
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetIFAGC_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetSNR_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetPostViterbiBer_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetPacketErr_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetTPSInfo_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_GetCellID_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBT_SetPowerState_PARAM[5];

UADP_STRUCT_POINTER_TABLE spt_DVBT_8BYTE_PARAM[5];
//UADP_STRUCT_POINTER_TABLE spt_DVBC_8BYTE_PARAM[5];

MS_U32 DVBT_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[2048];

    switch(u32Cmd)
    {
        case DMD_DVBT_DRV_CMD_Init:
        		#ifdef MS_DEBUG
            printf("kernal mode DVBTIoctl - MDrv_DMD_DVBT_Init\n");            
            #endif
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_INIT_PARAM, spt_DVBT_INIT_PARAM, buffer_arg, sizeof(buffer_arg));
        break;    
        case DMD_DVBT_DRV_CMD_Exit:
        		#ifdef MS_DEBUG
            printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_Exit\n");
            #endif
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_EXIT_PARAM, spt_DVBT_EXIT_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        case DMD_DVBT_DRV_CMD_SetDbgLeve :
        		#ifdef MS_DEBUG
            printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_SetDbgLevel\n");            
            #endif
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_DBG_LEVEL_PARAM, spt_DVBT_DBG_LEVEL_PARAM, buffer_arg, sizeof(buffer_arg));
        break;

        /*bryan temp mark*/
        /*
        case DMD_DVBT_DRV_CMD_GetInfo:
            printk("bryan kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetInfo\n");                
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GETREG_PARAM, spt_DVBT_GETREG_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        */
	 case DMD_DVBT_DRV_CMD_GetLibVer:
	 			#ifdef MS_DEBUG
	      printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetLibVer\n");    	
	      #endif
	     u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GETLIBVER_PARAM, spt_DVBT_GETLIBVER_PARAM, buffer_arg, sizeof(buffer_arg));
	 break;
	case DMD_DVBT_DRV_CMD_GetFWVer:
		 #ifdef MS_DEBUG
		 printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetFWVer\n");    	
		 #endif
	     u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GETFWVER_PARAM, spt_DVBT_GETFWVER_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	case DMD_DVBT_DRV_CMD_GetReg:
		#ifdef MS_DEBUG
		printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetReg\n");
		#endif
		 u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GETREG_PARAM, spt_DVBT_GETREG_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	case DMD_DVBT_DRV_CMD_SetReg:
		#ifdef MS_DEBUG
		printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_SetReg\n");
		#endif
		 u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_SETREG_PARAM, spt_DVBT_SETREG_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	case DMD_DVBT_DRV_CMD_SetSerialControl:
		#ifdef MS_DEBUG
		printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_SetSerialControl\n");
		#endif
		u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_SetSerialControl_PARAM, spt_DVBT_SetSerialControl_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	case DMD_DVBT_DRV_CMD_SetConfigHPLPSetIF:
		#ifdef MS_DEBUG
		printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_SetConfigHPLPSetIF\n");
		#endif
		u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_SetConfigHPLPSetIF_PARAM, spt_DVBT_SetConfigHPLPSetIF_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	 case DMD_DVBT_DRV_CMD_SetActive:
	 	#ifdef MS_DEBUG
	 	printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_SetSetActive\n");
	 	#endif
	 	u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_SetActive_PARAM, spt_DVBT_SetActive_PARAM, buffer_arg, sizeof(buffer_arg));
	 break;
	 case DMD_DVBT_DRV_CMD_GetLock:
	 	#ifdef MS_DEBUG
	 	//printf("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetLock\n");
	 	#endif
	 	u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GetLock_PARAM, spt_DVBT_GetLock_PARAM, buffer_arg, sizeof(buffer_arg));
	 break;

	 /*bryan temp mark*/
	 #if(0)
	  case DMD_DVBT_DRV_CMD_GetSignalStrengthWithRFPower:
	 #ifdef MS_DEBUG
	 printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetSignalStrengthWithRFPower:\n");
	 #endif
	 u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GetSignalStrengthWithRFPower_PARAM, spt_DVBT_GetSignalStrengthWithRFPower_PARAM, buffer_arg, sizeof(buffer_arg));
	 break;
	 case DMD_DVBT_DRV_CMD_GetSignalQualityWithRFPower:
	 #ifdef MS_DEBUG
	 printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetSignalQualityWithRFPower:\n");
	 #endif
	 u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GetSignalQualityWithRFPower_PARAM, spt_DVBT_GetSignalQualityWithRFPower_PARAM, buffer_arg, sizeof(buffer_arg));
	 break;
	 #endif
	 /* bryan temp mark */
	 #if(1)
	case DMD_DVBT_DRV_CMD_GetSNR:
	#ifdef MS_DEBUG
	printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetSNR:\n");
	#endif
	u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GetSNR_PARAM, spt_DVBT_GetSNR_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	#endif

	 case DMD_DVBT_DRV_CMD_GetPacketErr:
	 #ifdef MS_DEBUG
	 printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetSignalQualityWithRFPower:\n");
	 #endif
	 u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_U16_VAL, spt_DVBT_U16_VAL, buffer_arg, sizeof(buffer_arg));
	 break;

	case DMD_DVBT_DRV_CMD_GetTPSInfo:
	 #ifdef MS_DEBUG
	 printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetSignalQualityWithRFPower:\n");
	 #endif
	 u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GetTPSInfo_PARAM, spt_DVBT_GetTPSInfo_PARAM, buffer_arg, sizeof(buffer_arg));
	 break;


	case DMD_DVBT_DRV_CMD_GetCellID:
	#ifdef MS_DEBUG
	printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetCellID:\n");
	#endif
	u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_GetCellID_PARAM, spt_DVBT_GetCellID_PARAM, buffer_arg, sizeof(buffer_arg));
	break;


	/*bryan temp mark*/
	#if(0)
	case DMD_DVBT_DRV_CMD_GetFreqOffset:
	#ifdef MS_DEBUG
	printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_GetCellID:\n");
	#endif
	u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_PDVBT_GetFreqOffset_PARAM, spt_PDVBT_GetFreqOffset_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	#endif

	#if(0)
	  case DMD_DVBT_DRV_CMD_NORDIGSSITableWrite:
  
         break;
         
         case DMD_DVBT_DRV_CMD_NORDIGSSITableRead:

         break;
	#endif
	
         case DMD_DVBT_DRV_CMD_SetPowerState:
    #ifdef MS_DEBUG
	  printk("kernal mode DVBTIoctl - MDrv_DMD_DVBT_SetPowerState:\n");
	  #endif
	  u32Ret =UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBT_SetPowerState_PARAM, spt_DVBT_SetPowerState_PARAM, buffer_arg, sizeof(buffer_arg));
         break;
         
       //arthur
	     case DMD_DVBT_DRV_CMD_GetPostViterbiBer:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBTIoctl - DMD_DVBT_DRV_CMD_GetPostViterbiBer\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBT_GetPostViterbiBer_PARAM,spt_DVBT_GetPostViterbiBer_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     case DMD_DVBT_DRV_CMD_GetIFAGC:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBTIoctl - DMD_DVBT_DRV_CMD_GetIFAGC\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBT_GetIFAGC_PARAM,spt_DVBT_GetIFAGC_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

        default:
        break;

    }

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

MS_U32 DVBT_adp_Init(FUtopiaIOctl* pIoctl)
{

//set table
    UADP_SPT_BGN(&spt_DVBT_DBG_LEVEL_PARAM[0], sizeof(DVBT_SETDBG_LEVEL_PARAM));
    UADP_SPT_FIN(&spt_DVBT_DBG_LEVEL_PARAM[1]);  

    UADP_SPT_BGN(&spt_DVBT_EXIT_PARAM[0],sizeof(DVBT_EXIT_PARAM));
    UADP_SPT_FIN(&spt_DVBT_EXIT_PARAM[1]); 

//    UADP_SPT_BGN(&spt_DMD_DVBT_InitData[0], sizeof(DMD_DVBT_InitData_Transform));
//    UADP_SPT_FIN(&spt_DMD_DVBT_InitData[1]);


	  UADP_SPT_BGN(&spt_DVBT_8BYTE_PARAM[0],8);
    UADP_SPT_FIN(&spt_DVBT_8BYTE_PARAM[1]);

    //bryan test
    #if(0)
    UADP_SPT_BGN(&spt_DVBT_INIT_PARAM[0], sizeof(DVBT_INIT_PARAM));
    UADP_SPT_FIN(&spt_DVBT_INIT_PARAM[1]);  //UADP_SPT_NXT(&spt_DVBT_INIT_PARAM[1], DVBT_INIT_PARAM, pDMD_DVBT_InitData, spt_DMD_DVBT_InitData);
    #else
    
    #if(0)
    UADP_SPT_BGN(&spt_DVBT_INIT_PARAM[0], sizeof(DVBT_INIT_PARAM));
    UADP_SPT_NXT(&spt_DVBT_INIT_PARAM[1], DVBT_INIT_PARAM, DMD_DVBT_InitData, spt_DMD_DVBT_InitData);
    UADP_SPT_FIN(&spt_DVBT_INIT_PARAM[2]);  //UADP_SPT_NXT(&spt_DVBT_INIT_PARAM[1], DVBT_INIT_PARAM, pDMD_DVBT_InitData, spt_DMD_DVBT_InitData);
    #else
    UADP_SPT_BGN(&spt_DMD_DVBT_InitData_Transform[0], sizeof(DMD_DVBT_InitData_Transform));
    UADP_SDT_KIND(&spt_DMD_DVBT_InitData_Transform[1], DMD_DVBT_InitData_Transform, UADP_SDT_P2N, u8DMD_DVBT_DSPRegInitExt, spt_DVBT_8BYTE_PARAM);
    UADP_SDT_KIND(&spt_DMD_DVBT_InitData_Transform[2], DMD_DVBT_InitData_Transform, UADP_SDT_P2N, u8DMD_DVBT_InitExt, spt_DVBT_8BYTE_PARAM);
    UADP_SPT_FIN(&spt_DMD_DVBT_InitData_Transform[3]);    

    UADP_SPT_BGN(&spt_DVBT_INIT_PARAM[0], sizeof(DVBT_INIT_PARAM));
    UADP_SDT_KIND(&spt_DVBT_INIT_PARAM[1], DVBT_INIT_PARAM, UADP_SDT_P2N,DMD_DVBT_InitData, spt_DMD_DVBT_InitData_Transform);
    UADP_SPT_FIN(&spt_DVBT_INIT_PARAM[2]);  //UADP_SPT_NXT(&spt_DVBT_INIT_PARAM[1], DVBT_INIT_PARAM, pDMD_DVBT_InitData, spt_DMD_DVBT_InitData);

    #endif
    
    #endif
/*
    UADP_SPT_BGN(&spt_DVBT_GETINFO_PARAM[0], sizeof(DVBT_GETINFO_PARAM));
    UADP_SPT_FIN(&spt_DVBT_GETINFO_PARAM[1]);
*/

//bryan add
    UADP_SPT_BGN(&spt_DVBT_GETLIBVER_PARAM[0], sizeof(DVBT_GETLIBVER_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GETLIBVER_PARAM[1], DVBT_GETLIBVER_PARAM, ppVersion, spt_DVBT_MSIF_Version);
    UADP_SPT_FIN(&spt_DVBT_GETLIBVER_PARAM[2]);	

    UADP_SPT_BGN(&spt_DVBT_GETFWVER_PARAM[0],sizeof(DVBT_GETFWVER_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GETFWVER_PARAM[1], DVBT_GETFWVER_PARAM, ver, spt_U16_Val);
    UADP_SPT_FIN(&spt_DVBT_GETFWVER_PARAM[2]);	

/*bryam temp mark*/
    #if(0)
    UADP_SPT_BGN(&spt_PDVBT_GetFreqOffset_PARAM[0], sizeof(DVBT_GetFreqOffset_PARAM));
    UADP_SPT_NXT(&spt_PDVBT_GetFreqOffset_PARAM[1], DVBT_GetFreqOffset_PARAM,pFreqOff,spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_PDVBT_GetFreqOffset_PARAM[2]);      
    #endif
    
//end of bryan add

/*bryan temp mark*/
#if(0)
    UADP_SPT_BGN(&spt_DMD_DVBT_Info[0], sizeof(DMD_DVBT_Info));
    UADP_SPT_FIN(&spt_DMD_DVBT_Info[1]);
#endif
    /*UADP_SPT_BGN(&spt_DVBT_GETINFO_RETURN_PARAM[0], sizeof(DVBT_GETINFO_RETURN_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GETINFO_RETURN_PARAM[1], DVBT_GETINFO_RETURN_PARAM, pInfo, spt_DMD_DVBT_Info);
    UADP_SPT_FIN(&spt_DVBT_GETINFO_RETURN_PARAM[2]);  
    */
    UADP_SPT_BGN(&spt_DVBT_U8_VAL[0], sizeof(MS_U8));
    UADP_SPT_FIN(&spt_DVBT_U8_VAL[1]);
    UADP_SPT_BGN(&spt_DVBT_GETREG_PARAM[0], sizeof(DVBT_GETREG_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GETREG_PARAM[1], DVBT_GETREG_PARAM, pu8Data, spt_DVBT_U8_VAL);
    UADP_SPT_FIN(&spt_DVBT_GETREG_PARAM[2]);          
        
    UADP_SPT_BGN(&spt_DVBT_SETREG_PARAM[0], sizeof(DVBT_SETREG_PARAM));
    UADP_SPT_FIN(&spt_DVBT_SETREG_PARAM[1]);          

    UADP_SPT_BGN(&spt_DVBT_SetSerialControl_PARAM[0], sizeof(DVBT_SetSerialControl_PARAM));
    UADP_SPT_FIN(&spt_DVBT_SetSerialControl_PARAM[1]);     

    UADP_SPT_BGN(&spt_DVBT_SetConfig_PARAM[0], sizeof(DVBT_SetConfig_PARAM));
    UADP_SPT_FIN(&spt_DVBT_SetConfig_PARAM[1]);     

    UADP_SPT_BGN(&spt_DVBT_SetConfigHPLP_PARAM[0], sizeof(DVBT_SetConfigHPLP_PARAM));
    UADP_SPT_FIN(&spt_DVBT_SetConfigHPLP_PARAM[1]);         

    UADP_SPT_BGN(&spt_DVBT_SetConfigHPLPSetIF_PARAM[0], sizeof(DVBT_SetConfigHPLPSetIF_PARAM));
    UADP_SPT_FIN(&spt_DVBT_SetConfigHPLPSetIF_PARAM[1]);     

    UADP_SPT_BGN(&spt_DVBT_SetActive_PARAM[0], sizeof(DVBT_SetActive_PARAM));
    UADP_SPT_FIN(&spt_DVBT_SetActive_PARAM[1]);        

    UADP_SPT_BGN(&spt_DMD_LOCK_STATUS[0], sizeof(DMD_LOCK_STATUS));
    UADP_SPT_FIN(&spt_DMD_LOCK_STATUS[1]);
    
    UADP_SPT_BGN(&spt_DVBT_GetLock_PARAM[0], sizeof(DVBT_GetLock_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetLock_PARAM[1], DVBT_GetLock_PARAM, eLockStatus, spt_DMD_LOCK_STATUS);
    UADP_SPT_FIN(&spt_DVBT_GetLock_PARAM[2]);      
    
    UADP_SPT_BGN(&spt_DVBT_U16_VAL[0], sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBT_U16_VAL[1]);

    /*bryan temp mark*/
    #if(0)
    UADP_SPT_BGN(&spt_DVBT_GetSignalStrength_PARAM[0], sizeof(DVBT_GetSignalStrength_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetSignalStrength_PARAM[1], DVBT_GetSignalStrength_PARAM, u16Strength, spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetSignalStrength_PARAM[2]);     
    #endif

    
    UADP_SPT_BGN(&spt_DVBT_U16_VAL[0], sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBT_U16_VAL[1]);

    /*bryan temp mark*/
    #if(0)
    UADP_SPT_BGN(&spt_DVBT_GetSignalStrengthWithRFPower_PARAM[0], sizeof(DVBT_GetSignalStrengthWithRFPower_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetSignalStrengthWithRFPower_PARAM[1], DVBT_GetSignalStrengthWithRFPower_PARAM, u16Strength, spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetSignalStrengthWithRFPower_PARAM[2]);     
    #endif
    
    UADP_SPT_BGN(&spt_DVBT_U16_VAL[0], sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBT_U16_VAL[1]);

    /*bryan temp mark*/
    #if(0)
    UADP_SPT_BGN(&spt_DVBT_GetSignalQuality_PARAM[0], sizeof(DVBT_GetSignalQuality_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetSignalQuality_PARAM[1], DVBT_GetSignalQuality_PARAM, u16Quality, spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetSignalQuality_PARAM[2]);         
    #endif
    
    UADP_SPT_BGN(&spt_DVBT_U16_VAL[0], sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBT_U16_VAL[1]);

    /*bryan temp mark*/
    #if(0)  
    UADP_SPT_BGN(&spt_DVBT_GetSignalQualityWithRFPower_PARAM[0], sizeof(DVBT_GetSignalQualityWithRFPower_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetSignalQualityWithRFPower_PARAM[1], DVBT_GetSignalQualityWithRFPower_PARAM, u16Quality, spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetSignalQualityWithRFPower_PARAM[2]);         
    #endif
    
#if 0    
    UADP_SPT_BGN(&spt_DVBT_FLOAT_VAL[0], sizeof(float));
    UADP_SPT_FIN(&spt_DVBT_FLOAT_VAL[1]);
#endif    
    //arthur
    UADP_SPT_BGN(&spt_DVBT_GetSNR_PARAM[0], sizeof(DVBT_GetSNR_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetSNR_PARAM[1], DVBT_GetSNR_PARAM, noise_power_reg, spt_DVBT_U32_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetSNR_PARAM[2]); 


    UADP_SPT_BGN(&spt_DVBT_U16_VAL[0], sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBT_U16_VAL[1]);
    UADP_SPT_BGN(&spt_DVBT_GetPacketErr_PARAM[0], sizeof(DVBT_GetPacketErr_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetPacketErr_PARAM[1], DVBT_GetPacketErr_PARAM, pktErr, spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetPacketErr_PARAM[2]);       

    UADP_SPT_BGN(&spt_DVBT_U16_VAL[0], sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBT_U16_VAL[1]);
    UADP_SPT_BGN(&spt_DVBT_GetTPSInfo_PARAM[0], sizeof(DVBT_GetTPSInfo_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetTPSInfo_PARAM[1], DVBT_GetTPSInfo_PARAM, u16Info, spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetTPSInfo_PARAM[2]);        

    UADP_SPT_BGN(&spt_DVBT_U16_VAL[0], sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBT_U16_VAL[1]);
    
    UADP_SPT_BGN(&spt_DVBT_GetCellID_PARAM[0], sizeof(DVBT_GetCellID_PARAM));
    UADP_SPT_NXT(&spt_DVBT_GetCellID_PARAM[1], DVBT_GetCellID_PARAM, u16CellID, spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetCellID_PARAM[2]);         

    UADP_SPT_BGN(&spt_DVBT_SetPowerState_PARAM[0], sizeof(DVBT_SetPowerState_PARAM));
    UADP_SPT_FIN(&spt_DVBT_SetPowerState_PARAM[1]);
    
    //arthur
    UADP_SPT_BGN(&spt_DVBT_GetPostViterbiBer_PARAM[0],sizeof(DVBT_GetPostViterbiBer_PARAM));
    UADP_SDT_KIND(&spt_DVBT_GetPostViterbiBer_PARAM[1],DVBT_GetPostViterbiBer_PARAM,UADP_SDT_P2N,BitErr_reg,spt_DVBT_U32_VAL);
    UADP_SDT_KIND(&spt_DVBT_GetPostViterbiBer_PARAM[2],DVBT_GetPostViterbiBer_PARAM,UADP_SDT_P2N,BitErrPeriod_reg,spt_DVBT_U16_VAL);
    UADP_SDT_KIND(&spt_DVBT_GetPostViterbiBer_PARAM[3],DVBT_GetPostViterbiBer_PARAM,UADP_SDT_P2N,PktErr_reg,spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetPostViterbiBer_PARAM[4]);
    
    UADP_SPT_BGN(&spt_DVBT_GetIFAGC_PARAM[0],sizeof(DVBT_GetIFAGC_PARAM));
    UADP_SDT_KIND(&spt_DVBT_GetIFAGC_PARAM[1],DVBT_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_reg,spt_DVBT_U16_VAL);
    UADP_SDT_KIND(&spt_DVBT_GetIFAGC_PARAM[2],DVBT_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_reg_lsb,spt_DVBT_U16_VAL);
    UADP_SDT_KIND(&spt_DVBT_GetIFAGC_PARAM[3],DVBT_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_err_reg,spt_DVBT_U16_VAL);
    UADP_SPT_FIN(&spt_DVBT_GetIFAGC_PARAM[4]);

   *pIoctl= (FUtopiaIOctl)DVBT_adp_Ioctl;

    return TRUE;
}






