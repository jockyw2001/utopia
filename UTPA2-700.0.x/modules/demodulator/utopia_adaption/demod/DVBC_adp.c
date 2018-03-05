#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"

#include "drvDMD_INTERN_DVBC_v2.h"

//#include "MsOS.h"

//#include <linux/kernel.h>
//Top
UADP_STRUCT_POINTER_TABLE spt_DVBC_BOOL_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_U8_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_U16_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_U32_VAL[5];

//pointer parameter
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBC_InitData_Transform[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBC_Info[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBC_MODULATION_TYPE[5];

UADP_STRUCT_POINTER_TABLE spt_DVBC_Dual_Public_Init_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_Dual_Individual_Init_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_Init_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_EXIT_PARAM_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_SetDbgLevel_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetInfo_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetFWVer_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetDSPReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_SetDSPReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_SetReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_SetSerialControl_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_SetConfig_Symbol_rate_list_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_SetActive_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetLockWithRFPower[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_ActiveDmdSwitch_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetPacketErr_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetCellID_PARAM[5];

UADP_STRUCT_POINTER_TABLE spt_DVBC_LOCK_STATUS_PARAM[5];
//bryan temp used
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetStatus_PARAM[7];
UADP_STRUCT_POINTER_TABLE spt_DVBC_SetPowerState_PARAM[5];

//waiting add
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetIFAGC_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetSNR_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetPostViterbiBer_PARAM[5];

#if UFO_DEMOD_DVBC_GET_AGC_INFO
//bryan add 
UADP_STRUCT_POINTER_TABLE spt_DVBC_GetAGCInfo_PARAM[5];
#endif

UADP_STRUCT_POINTER_TABLE spt_DVBC_8BYTE_PARAM[5];

MS_U32 DVBC_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[2048];


    switch(u32Cmd)
    {
#if defined(CHIP_KAISER)||defined(CHIP_K6LITE)
	case DMD_DVBC_DRV_CMD_Dual_Public_Init:
		#ifdef MS_DEBUG
	  printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_Dual_Public_Init\n");            
	  #endif
         u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBC_Dual_Public_Init_PARAM, spt_DVBC_Dual_Public_Init_PARAM, buffer_arg, sizeof(buffer_arg));
	break;
	
       case DMD_DVBC_DRV_CMD_Dual_Individual_Init:
    #ifdef MS_DEBUG
	  printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_Dual_Individual_Init\n");            
	  #endif
         u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBC_Dual_Individual_Init_PARAM, spt_DVBC_Dual_Individual_Init_PARAM, buffer_arg, sizeof(buffer_arg));
    	break;
    	
    	#endif
    	
        case DMD_DVBC_DRV_CMD_Init:
      #ifdef MS_DEBUG       
	    printk("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_Init\n");            
	    #endif
           u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBC_Init_PARAM, spt_DVBC_Init_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBC_DRV_CMD_Exit:
        #ifdef MS_DEBUG
            printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_Exit\n");            
       #endif
	     u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL,spt_DVBC_EXIT_PARAM_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBC_DRV_CMD_SetDbgLevel:
        	#ifdef MS_DEBUG
        	printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_SetDbgLevels\n");         
          #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_SetDbgLevel_PARAM,spt_DVBC_SetDbgLevel_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBC_DRV_CMD_GetInfo:
        	#ifdef MS_DEBUG
				  printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetInfo\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetInfo_PARAM,spt_DVBC_GetInfo_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBC_DRV_CMD_GetLibVer:
        	#ifdef MS_DEBUG
				  printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetLibVer\n");         
					#endif
	  //waiting temp mark
	  /*
	  u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetLibVer_PARAM,spt_DVBC_GetLibVer_PARAM, buffer_arg, sizeof(buffer_arg));				
    u32Ret = psDVBCInstPri->fpDVBC_GetLibVer(((PDVBC_GetLibVer_PARAM)pArgs)->ppVersion);\
           ((PDVBC_GetLibVer_PARAM)pArgs)->ret=bRet);
    */
        break;      
        
        case DMD_DVBC_DRV_CMD_GetFWVer:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetFWVer\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetFWVer_PARAM,spt_DVBC_GetFWVer_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBC_DRV_CMD_GetDSPReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetDSPReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetDSPReg_PARAM,spt_DVBC_GetDSPReg_PARAM, buffer_arg, sizeof(buffer_arg));
        break;

        case DMD_DVBC_DRV_CMD_SetDSPReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_SetDSPReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_SetDSPReg_PARAM,spt_DVBC_SetDSPReg_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;
        
        case DMD_DVBC_DRV_CMD_GetReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetReg_PARAM,spt_DVBC_GetReg_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;
            
        case DMD_DVBC_DRV_CMD_SetReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_SetReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_SetReg_PARAM,spt_DVBC_SetReg_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;
        
        case DMD_DVBC_DRV_CMD_SetSerialControl:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_SetSerialControl\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_SetSerialControl_PARAM,spt_DVBC_SetSerialControl_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;

        case DMD_DVBC_DRV_CMD_SetSetConfig_symbol_rate_list:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_SetSetConfig_symbol_rate_list\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_SetConfig_Symbol_rate_list_PARAM,spt_DVBC_SetConfig_Symbol_rate_list_PARAM, buffer_arg, sizeof(buffer_arg));			
        break;
        
        case DMD_DVBC_DRV_CMD_SetActive:
        	#ifdef MS_DEBUG
        	printk("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_SetActive\n"); 
        	#endif     
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_SetActive_PARAM,spt_DVBC_SetActive_PARAM, buffer_arg, sizeof(buffer_arg));			
        break;

        case DMD_DVBC_DRV_CMD_GetLockWithRFPower:  
        	#ifdef MS_DEBUG
        	printk("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetLockWithRFPower\n"); 
        	#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetLockWithRFPower,spt_DVBC_GetLockWithRFPower, buffer_arg, sizeof(buffer_arg));			
	      break;
	      
	      /*
        case DMD_DVBC_DRV_CMD_GetSignalStrengthWithRFPower:
        	#ifdef MS_DEBUG
        	printk("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetSignalStrengthWithRFPower\n"); 
        	#endif
		u32Ret = psDVBCInstPri->DMD_DVBC_GetSignalStrengthWithRFPower(((PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs)->u16Strength,\
		         ((PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs)->ret=bRet);
        break;

	      case DMD_DVBC_DRV_CMD_GetSignalQualityWithRFPower:
	      	#ifdef MS_DEBUG
        	printk("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetSignalQualityWithRFPower\n"); 
        	#endif
		u32Ret = psDVBCInstPri->fpDVBC_GetSignalQualityWithRFPower((PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs->u16Strength,\
		         ((PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs->ret=bRet);
	      break;
	      */
	      
       case DMD_DVBC_DRV_CMD_ActiveDmdSwitch:
       	 #ifdef MS_DEBUG
				 printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_ActiveDmdSwitch\n");         
				 #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_ActiveDmdSwitch_PARAM,spt_DVBC_ActiveDmdSwitch_PARAM, buffer_arg, sizeof(buffer_arg));			
       break;
       
       /*
       case DMD_DVBC_DRV_CMD_GetSNR:
       	 #ifdef MS_DEBUG
				 printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetSNR\n");         
				 #endif
    u32Ret = psDVBCInstPri->fpDVBC_GetSNR((PDVBC_GetSNR_PARAM)pArgs->snr_reg);\
       	     ((PDVBC_GetSNR_PARAM)pArgs->ret=bRet);
       break;

       case DMD_DVBC_DRV_CMD_GetPostViterbiBer:
       	 #ifdef MS_DEBUG
				 printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetPostViterbiBer\n");         
				 #endif
		u32Ret = psDVBCInstPri->fpDVBC_GetPostViterbiBer((PDVBC_GetPostViterbiBer_PARAM)pArgs->ber_reg);
		       ((PDVBC_GetPostViterbiBer_PARAM)pArgs->ret=bRet);
       break;
       */
	
       case DMD_DVBC_DRV_CMD_GetPacketErr:
       	 #ifdef MS_DEBUG
		     printf(KERN_EMERG"bryan kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetPacketErr\n");         
			   #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetPacketErr_PARAM,spt_DVBC_GetPacketErr_PARAM, buffer_arg, sizeof(buffer_arg));			
       break;

	     case DMD_DVBC_DRV_CMD_GetCellID:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetCellID\n");         
				 #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetCellID_PARAM,spt_DVBC_GetCellID_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBC_DRV_CMD_GetStatus:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetStatus\n");         
				 #endif      
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetStatus_PARAM,spt_DVBC_GetStatus_PARAM, buffer_arg, sizeof(buffer_arg));			
		   break;

	     case DMD_DVBC_DRV_CMD_SetPowerState:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_SetPowerState\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_SetPowerState_PARAM,spt_DVBC_SetPowerState_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     //waiting add
	     case DMD_DVBC_DRV_CMD_GetIFAGC:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetIFAGC\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetIFAGC_PARAM,spt_DVBC_GetIFAGC_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     //waiting add
	     case DMD_DVBC_DRV_CMD_GetSNR:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetSNR\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetSNR_PARAM,spt_DVBC_GetSNR_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     //waiting add
	     case DMD_DVBC_DRV_CMD_GetPostViterbiBer:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetPostViterbiBer\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetPostViterbiBer_PARAM,spt_DVBC_GetPostViterbiBer_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     #ifdef UFO_DEMOD_DVBC_GET_AGC_INFO:
	     case DMD_DVBC_DRV_CMD_GetAGCInfo:	
	     	 #ifdef MS_DEBUG
		     printf("kernal mode DVBCIoctl - DMD_DVBC_DRV_CMD_GetAGCInfo\n");         
		     #endif
	     u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBC_GetAGCInfo_PARAM,spt_DVBC_GetAGCInfo_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     #endif
			 
			 
			 
        default:
        break;

    }

    return u32Ret;

}

MS_U32 DVBC_adp_Init(FUtopiaIOctl* pIoctl)
{
    //top parameter
    UADP_SPT_BGN(&spt_DVBC_BOOL_VAL[0], sizeof(MS_BOOL));
    UADP_SPT_FIN(&spt_DVBC_BOOL_VAL[1]);  

    UADP_SPT_BGN(&spt_DVBC_U8_VAL[0], sizeof(MS_U8));
    UADP_SPT_FIN(&spt_DVBC_U8_VAL[1]);  

    UADP_SPT_BGN(&spt_DVBC_U16_VAL[0],sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBC_U16_VAL[1]);
    
    UADP_SPT_BGN(&spt_DVBC_U32_VAL[0],sizeof(MS_U32));
    UADP_SPT_FIN(&spt_DVBC_U32_VAL[1]);
    
    UADP_SPT_BGN(&spt_DVBC_8BYTE_PARAM[0],8);
    UADP_SPT_FIN(&spt_DVBC_8BYTE_PARAM[1]);
    
    
      

    //pointer parameter
    UADP_SPT_BGN(&spt_DMD_DVBC_InitData_Transform[0],sizeof(DMD_DVBC_InitData_Transform));
    UADP_SDT_KIND(&spt_DMD_DVBC_InitData_Transform[1], DMD_DVBC_InitData_Transform, UADP_SDT_P2N, u8DMD_DVBC_DSPRegInitExt, spt_DVBC_8BYTE_PARAM);
    UADP_SDT_KIND(&spt_DMD_DVBC_InitData_Transform[2], DMD_DVBC_InitData_Transform, UADP_SDT_P2N, u8DMD_DVBC_InitExt, spt_DVBC_8BYTE_PARAM);
    UADP_SPT_FIN(&spt_DMD_DVBC_InitData_Transform[3]);  

    UADP_SPT_BGN(&spt_DMD_DVBC_Info[0],sizeof(DMD_DVBC_Info));
    UADP_SPT_FIN(&spt_DMD_DVBC_Info[1]);

    UADP_SPT_BGN(&spt_DMD_DVBC_MODULATION_TYPE[0],sizeof(DMD_DVBC_MODULATION_TYPE));
    UADP_SPT_FIN(&spt_DMD_DVBC_MODULATION_TYPE[1]);
    
    UADP_SPT_BGN(&spt_DVBC_LOCK_STATUS_PARAM[0],sizeof(DMD_DVBC_LOCK_STATUS));
    UADP_SPT_FIN(&spt_DVBC_LOCK_STATUS_PARAM[1]);
    
    //function input/output parameter
    UADP_SPT_BGN(&spt_DVBC_Dual_Public_Init_PARAM[0], sizeof(DVBC_Dual_Public_Init_PARAM));
    UADP_SPT_FIN(&spt_DVBC_Dual_Public_Init_PARAM[1]);  

    UADP_SPT_BGN(&spt_DVBC_Dual_Individual_Init_PARAM[0], sizeof(DVBC_Dual_Individual_Init_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_Dual_Individual_Init_PARAM[1], DVBC_Dual_Individual_Init_PARAM, pDMD_DVBC_InitData, spt_DMD_DVBC_InitData_Transform);
    UADP_SDT_KIND(&spt_DVBC_Dual_Individual_Init_PARAM[1], DVBC_Dual_Individual_Init_PARAM, UADP_SDT_P2N, pDMD_DVBC_InitData, spt_DMD_DVBC_InitData_Transform);
    UADP_SPT_FIN(&spt_DVBC_Dual_Individual_Init_PARAM[2]);  

    UADP_SPT_BGN(&spt_DVBC_Init_PARAM[0],sizeof(DVBC_Init_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_Init_PARAM[1], DVBC_Init_PARAM, pDMD_DVBC_InitData, spt_DMD_DVBC_InitData_Transform);
    UADP_SDT_KIND(&spt_DVBC_Init_PARAM[1], DVBC_Init_PARAM, UADP_SDT_P2N, pDMD_DVBC_InitData, spt_DMD_DVBC_InitData_Transform);
    UADP_SPT_FIN(&spt_DVBC_Init_PARAM[2]);  

    UADP_SPT_BGN(&spt_DVBC_EXIT_PARAM_PARAM[0],sizeof(DVBC_EXIT_PARAM_PARAM));
    UADP_SPT_FIN(&spt_DVBC_EXIT_PARAM_PARAM[1]);  

    UADP_SPT_BGN(&spt_DVBC_SetDbgLevel_PARAM[0],sizeof(DVBC_SetDbgLevel_PARAM));
    UADP_SPT_FIN(&spt_DVBC_SetDbgLevel_PARAM[1]); 

    UADP_SPT_BGN(&spt_DVBC_GetInfo_PARAM[0],sizeof(DVBC_GetInfo_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_GetInfo_PARAM[1],DVBC_GetInfo_PARAM,ret_info,spt_DMD_DVBC_Info);
    UADP_SDT_KIND(&spt_DVBC_GetInfo_PARAM[1],DVBC_GetInfo_PARAM,UADP_SDT_P2N,ret_info,spt_DMD_DVBC_Info);
    UADP_SPT_FIN(&spt_DVBC_GetInfo_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBC_GetFWVer_PARAM[0],sizeof(DVBC_GetFWVer_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_GetFWVer_PARAM[1],DVBC_GetFWVer_PARAM,ver,spt_DVBC_U16_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetFWVer_PARAM[1],DVBC_GetFWVer_PARAM,UADP_SDT_P2N,ver,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetFWVer_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBC_GetDSPReg_PARAM[0],sizeof(DVBC_GetDSPReg_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_GetDSPReg_PARAM[1],DVBC_GetDSPReg_PARAM,pu8Data,spt_DVBC_U8_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetDSPReg_PARAM[1],DVBC_GetDSPReg_PARAM,UADP_SDT_P2N,pu8Data,spt_DVBC_U8_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetDSPReg_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBC_SetDSPReg_PARAM[0],sizeof(DVBC_SetDSPReg_PARAM));
    UADP_SPT_FIN(&spt_DVBC_SetDSPReg_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBC_GetReg_PARAM[0],sizeof(DVBC_GetReg_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_GetReg_PARAM[1],DVBC_GetReg_PARAM,pu8Data,spt_DVBC_U8_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetReg_PARAM[1],DVBC_GetReg_PARAM,UADP_SDT_P2N,pu8Data,spt_DVBC_U8_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetReg_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBC_SetReg_PARAM[0],sizeof(DVBC_SetReg_PARAM));
    UADP_SPT_FIN(&spt_DVBC_SetReg_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBC_SetSerialControl_PARAM[0],sizeof(DVBC_SetSerialControl_PARAM));
    UADP_SPT_FIN(&spt_DVBC_SetSerialControl_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBC_SetConfig_Symbol_rate_list_PARAM[0],sizeof(DVBC_SetConfig_Symbol_rate_list_PARAM));
	// oga add 20160106
	UADP_SDT_KIND(&spt_DVBC_SetConfig_Symbol_rate_list_PARAM[1], DVBC_SetConfig_Symbol_rate_list_PARAM,UADP_SDT_P2N,pu16_symbol_rate_list,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_SetConfig_Symbol_rate_list_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBC_SetActive_PARAM[0],sizeof(DVBC_SetActive_PARAM));
    UADP_SPT_FIN(&spt_DVBC_SetActive_PARAM[1]);   

    UADP_SPT_BGN(&spt_DVBC_GetLockWithRFPower[0],sizeof(DVBC_GetLockWithRFPower));
    //UADP_SPT_NXT(&spt_DVBC_GetLockWithRFPower_Transform[1],DVBC_GetLockWithRFPower_Transform,eLockStatus,spt_DVBC_LOCK_STATUS_PARAM);
    UADP_SDT_KIND(&spt_DVBC_GetLockWithRFPower[1],DVBC_GetLockWithRFPower,UADP_SDT_P2N,eLockStatus,spt_DVBC_LOCK_STATUS_PARAM);
    UADP_SPT_FIN(&spt_DVBC_GetLockWithRFPower[2]);

    UADP_SPT_BGN(&spt_DVBC_ActiveDmdSwitch_PARAM[0],sizeof(DVBC_ActiveDmdSwitch_PARAM));
    UADP_SPT_FIN(&spt_DVBC_ActiveDmdSwitch_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBC_GetPacketErr_PARAM[0],sizeof(DVBC_GetPacketErr_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_GetPacketErr_PARAM[1],DVBC_GetPacketErr_PARAM,pktErr,spt_DVBC_U16_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetPacketErr_PARAM[1],DVBC_GetPacketErr_PARAM,UADP_SDT_P2N,pktErr,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetPacketErr_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBC_GetCellID_PARAM[0],sizeof(DVBC_GetCellID_PARAM));
    //UADP_SPT_NXT(&spt_DVBC_GetCellID_PARAM[1],DVBC_GetCellID_PARAM,u16CellID,spt_DVBC_U16_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetCellID_PARAM[1],DVBC_GetCellID_PARAM,UADP_SDT_P2N,u16CellID,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetCellID_PARAM[2]);

     //bryan temp debug
     UADP_SPT_BGN(&spt_DVBC_GetStatus_PARAM[0],sizeof(DVBC_GetStatus_PARAM));
     //UADP_SPT_NXT(&spt_DVBC_GetStatus_PARAM_Transform[1],DVBC_GetStatus_PARAM_Transform,pQAMMode,spt_DMD_DVBC_MODULATION_TYPE);
     //UADP_SPT_NXT(&spt_DVBC_GetStatus_PARAM_Transform[2],DVBC_GetStatus_PARAM_Transform,u16SymbolRate,spt_DVBC_U16_VAL);
     UADP_SDT_KIND(&spt_DVBC_GetStatus_PARAM[1],DVBC_GetStatus_PARAM,UADP_SDT_P2N,pQAMMode,spt_DMD_DVBC_MODULATION_TYPE);
     UADP_SDT_KIND(&spt_DVBC_GetStatus_PARAM[2],DVBC_GetStatus_PARAM,UADP_SDT_P2N,u16SymbolRate,spt_DVBC_U16_VAL);
     UADP_SDT_KIND(&spt_DVBC_GetStatus_PARAM[3],DVBC_GetStatus_PARAM,UADP_SDT_P2N,config_Fc_reg,spt_DVBC_U32_VAL);
     UADP_SDT_KIND(&spt_DVBC_GetStatus_PARAM[4],DVBC_GetStatus_PARAM,UADP_SDT_P2N,Fc_over_Fs_reg,spt_DVBC_U32_VAL);
     UADP_SDT_KIND(&spt_DVBC_GetStatus_PARAM[5],DVBC_GetStatus_PARAM,UADP_SDT_P2N,Cfo_offset_reg,spt_DVBC_U16_VAL);
     UADP_SPT_FIN(&spt_DVBC_GetStatus_PARAM[6]);
	
	
    UADP_SPT_BGN(&spt_DVBC_SetPowerState_PARAM[0],sizeof(DVBC_SetPowerState_PARAM));
    UADP_SPT_FIN(&spt_DVBC_SetPowerState_PARAM[1]);
    
    
    //waiting add
    //UADP_SPT_BGN(&spt_DVBC_FreqOffset_PARAM[0],sizeof(DMD_DVBC_FreqOffset));
    //UADP_SPT_FIN(&spt_DVBC_FreqOffset_PARAM[1]);
    
    //waiting add
    UADP_SPT_BGN(&spt_DVBC_GetIFAGC_PARAM[0],sizeof(DVBC_GetIFAGC_PARAM));
    UADP_SDT_KIND(&spt_DVBC_GetIFAGC_PARAM[1],DVBC_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_reg,spt_DVBC_U16_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetIFAGC_PARAM[2],DVBC_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_reg_lsb,spt_DVBC_U16_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetIFAGC_PARAM[3],DVBC_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_err_reg,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetIFAGC_PARAM[4]);
    
    //waiting add
    UADP_SPT_BGN(&spt_DVBC_GetSNR_PARAM[0],sizeof(DVBC_GetSNR_PARAM));
    UADP_SDT_KIND(&spt_DVBC_GetSNR_PARAM[1],DVBC_GetSNR_PARAM,UADP_SDT_P2N,snr_reg,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetSNR_PARAM[2]);
    
    //waiting add
    UADP_SPT_BGN(&spt_DVBC_GetPostViterbiBer_PARAM[0],sizeof(DVBC_GetPostViterbiBer_PARAM));
    UADP_SDT_KIND(&spt_DVBC_GetPostViterbiBer_PARAM[1],DVBC_GetPostViterbiBer_PARAM,UADP_SDT_P2N,BitErr_reg,spt_DVBC_U32_VAL);
    UADP_SDT_KIND(&spt_DVBC_GetPostViterbiBer_PARAM[2],DVBC_GetPostViterbiBer_PARAM,UADP_SDT_P2N,BitErrPeriod_reg,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetPostViterbiBer_PARAM[3]);
    
    //bryan add
    #if UFO_DEMOD_DVBC_GET_AGC_INFO
    UADP_SPT_BGN(&spt_DVBC_GetAGCInfo_PARAM[0],sizeof(DVBC_GetAGCInfo_PARAM));
    UADP_SDT_KIND(&spt_DVBC_GetAGCInfo_PARAM[1],DVBC_GetAGCInfo_PARAM,UADP_SDT_P2N,pu16Data,spt_DVBC_U16_VAL);
    UADP_SPT_FIN(&spt_DVBC_GetAGCInfo_PARAM[2]);
		#endif
		
   *pIoctl= (FUtopiaIOctl)DVBC_adp_Ioctl;

    return TRUE;
}






