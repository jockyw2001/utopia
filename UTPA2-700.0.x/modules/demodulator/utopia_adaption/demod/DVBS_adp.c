#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"

#include "drvDMD_INTERN_DVBS_v2.h"

//#include "MsOS.h"

//#include <linux/kernel.h>
//Top
UADP_STRUCT_POINTER_TABLE spt_DVBS_BOOL_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_U8_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_U16_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_S16_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_U32_VAL[5];
UADP_STRUCT_POINTER_TABLE spt_HAL_DEMOD_MS_FE_CARRIER_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBS_CODE_RATE_TYPE[5];
//pointer parameter
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBS_InitData_Transform[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBS_Info[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBS_MODULATION_TYPE[5];
UADP_STRUCT_POINTER_TABLE spt_DMD_DVBS_DEMOD_TYPE[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_Init_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_EXIT_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_SetDbgLevel_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetInfo_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetFWVer_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetDSPReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_SetDSPReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_SetReg_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_SetSerialControl_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_SetConfig_Symbol_rate_list_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_Config_Symbol_rate_list_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_SetActive_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetLockWithRFPower[5];
//UADP_STRUCT_POINTER_TABLE spt_DVBS_GetSignalQualityWithRFPower[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetSignalStrengthWithRFPower[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_ActiveDmdSwitch_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetPacketErr_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetCellID_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_LOCK_STATUS_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_Get_FreqOffset_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetStatus_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_SetPowerState_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetIFAGC_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetSNR_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetPostViterbiBer_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_Start_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_NextFreq_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_Cancel_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_End_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_GetChannel_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_GetCurrentFreq_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[8];
UADP_STRUCT_POINTER_TABLE spt_DVBS_BlindScan_GetTunerFreq_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_Init_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_SetLNBOut_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_GetLNBOut_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_Set22kOnOff_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_Get22kOnOff_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_SendCmd_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_SetTxToneMode_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_DiSEqC_SetTone_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_UnicableAGCCheckPower_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetTsDivNum_PARAM[9];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetCurrentDemodCodeRate_PARAM[5];
UADP_STRUCT_POINTER_TABLE spt_DVBS_GetTunrSignalLevel_PWR_PARAM[5];

UADP_STRUCT_POINTER_TABLE spt_DVBS_8BYTE_PARAM[5];

MS_U32 DVBS_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[2048];


    switch(u32Cmd)
    {    	
        case DMD_DVBS_DRV_CMD_Init:
      #ifdef MS_DEBUG       
	    printk("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_Init\n");            
	    #endif
           u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DVBS_Init_PARAM, spt_DVBS_Init_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBS_DRV_CMD_Exit:
        #ifdef MS_DEBUG
            printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_Exit\n");            
       #endif
	     u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL,spt_DVBS_EXIT_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBS_DRV_CMD_SetDbgLevel:
        	#ifdef MS_DEBUG
        	printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_SetDbgLevels\n");         
          #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_SetDbgLevel_PARAM,spt_DVBS_SetDbgLevel_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBS_DRV_CMD_GetInfo:
        	#ifdef MS_DEBUG
				  printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetInfo\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetInfo_PARAM,spt_DVBS_GetInfo_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBS_DRV_CMD_GetLibVer:
        	#ifdef MS_DEBUG
				  printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetLibVer\n");         
					#endif
	  //waiting temp mark
	  /*
	  u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetLibVer_PARAM,spt_DVBS_GetLibVer_PARAM, buffer_arg, sizeof(buffer_arg));				
    u32Ret = psDVBSInstPri->fpDVBS_GetLibVer(((PDVBS_GetLibVer_PARAM)pArgs)->ppVersion);\
           ((PDVBS_GetLibVer_PARAM)pArgs)->ret=bRet);
    */
        break;      
        
        case DMD_DVBS_DRV_CMD_GetFWVer:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetFWVer\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetFWVer_PARAM,spt_DVBS_GetFWVer_PARAM, buffer_arg, sizeof(buffer_arg));
        break;
        
        case DMD_DVBS_DRV_CMD_GetDSPReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetDSPReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetDSPReg_PARAM,spt_DVBS_GetDSPReg_PARAM, buffer_arg, sizeof(buffer_arg));
        break;

        case DMD_DVBS_DRV_CMD_SetDSPReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_SetDSPReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_SetDSPReg_PARAM,spt_DVBS_SetDSPReg_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;
        
        case DMD_DVBS_DRV_CMD_GetReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetReg_PARAM,spt_DVBS_GetReg_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;
            
        case DMD_DVBS_DRV_CMD_SetReg:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_SetReg\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_SetReg_PARAM,spt_DVBS_SetReg_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;
        
        case DMD_DVBS_DRV_CMD_SetSerialControl:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_SetSerialControl\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_SetSerialControl_PARAM,spt_DVBS_SetSerialControl_PARAM, buffer_arg, sizeof(buffer_arg));		
        break;

        case DMD_DVBS_DRV_CMD_BlindScan_Config_symbol_rate_list:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_Config_symbol_rate_list\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_Config_Symbol_rate_list_PARAM,spt_DVBS_BlindScan_Config_Symbol_rate_list_PARAM, buffer_arg, sizeof(buffer_arg));			
        break;
		
        case DMD_DVBS_DRV_CMD_SetConfig_symbol_rate_list:
        	#ifdef MS_DEBUG
					printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_SetSetConfig_symbol_rate_list\n");         
					#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_SetConfig_Symbol_rate_list_PARAM,spt_DVBS_SetConfig_Symbol_rate_list_PARAM, buffer_arg, sizeof(buffer_arg));			
        break;
        
        case DMD_DVBS_DRV_CMD_SetActive:
        	#ifdef MS_DEBUG
        	printk("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_SetActive\n"); 
        	#endif     
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_SetActive_PARAM,spt_DVBS_SetActive_PARAM, buffer_arg, sizeof(buffer_arg));			
        break;

        case DMD_DVBS_DRV_CMD_GetLockWithRFPower:  
        	#ifdef MS_DEBUG
        	printk("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetLockWithRFPower\n"); 
        	#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetLockWithRFPower,spt_DVBS_GetLockWithRFPower, buffer_arg, sizeof(buffer_arg));			
	      break;
	      
	 
        case DMD_DVBS_DRV_CMD_GetSignalStrengthWithRFPower:
        	#ifdef MS_DEBUG
        	printk("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetSignalStrengthWithRFPower\n"); 
        	#endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetSignalStrengthWithRFPower,spt_DVBS_GetSignalStrengthWithRFPower, buffer_arg, sizeof(buffer_arg));			
        break;
     /*
	      case DMD_DVBS_DRV_CMD_GetSignalQualityWithRFPower:
	      	#ifdef MS_DEBUG
        	printk("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetSignalQualityWithRFPower\n"); 
        	#endif
		u32Ret = psDVBSInstPri->fpDVBS_GetSignalQualityWithRFPower((PDVBS_GetSignalStrengthWithRFPower_PARAM)pArgs->u16Strength,\
		         ((PDVBS_GetSignalStrengthWithRFPower_PARAM)pArgs->ret=bRet);
	      break;
	      */
	      
       case DMD_DVBS_DRV_CMD_ActiveDmdSwitch:
       	 #ifdef MS_DEBUG
				 printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_ActiveDmdSwitch\n");         
				 #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_ActiveDmdSwitch_PARAM,spt_DVBS_ActiveDmdSwitch_PARAM, buffer_arg, sizeof(buffer_arg));			
       break;
       
       /*
       case DMD_DVBS_DRV_CMD_GetSNR:
       	 #ifdef MS_DEBUG
				 printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetSNR\n");         
				 #endif
    u32Ret = psDVBSInstPri->fpDVBS_GetSNR((PDVBS_GetSNR_PARAM)pArgs->snr_reg);\
       	     ((PDVBS_GetSNR_PARAM)pArgs->ret=bRet);
       break;

       case DMD_DVBS_DRV_CMD_GetPostViterbiBer:
       	 #ifdef MS_DEBUG
				 printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetPostViterbiBer\n");         
				 #endif
		u32Ret = psDVBSInstPri->fpDVBS_GetPostViterbiBer((PDVBS_GetPostViterbiBer_PARAM)pArgs->ber_reg);
		       ((PDVBS_GetPostViterbiBer_PARAM)pArgs->ret=bRet);
       break;
       */
	
       case DMD_DVBS_DRV_CMD_GetPacketErr:
       	 #ifdef MS_DEBUG
		     printf(KERN_EMERG"bryan kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetPacketErr\n");         
			   #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetPacketErr_PARAM,spt_DVBS_GetPacketErr_PARAM, buffer_arg, sizeof(buffer_arg));			
       break;

	     case DMD_DVBS_DRV_CMD_GetCellID:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetCellID\n");         
				 #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetCellID_PARAM,spt_DVBS_GetCellID_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_GetStatus:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetStatus\n");         
				 #endif      
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetStatus_PARAM,spt_DVBS_GetStatus_PARAM, buffer_arg, sizeof(buffer_arg));			
		   break;

	     case DMD_DVBS_DRV_CMD_SetPowerState:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_SetPowerState\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_SetPowerState_PARAM,spt_DVBS_SetPowerState_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     case DMD_DVBS_DRV_CMD_GetIFAGC:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetIFAGC\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetIFAGC_PARAM,spt_DVBS_GetIFAGC_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     case DMD_DVBS_DRV_CMD_GetSNR:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetSNR\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetSNR_PARAM,spt_DVBS_GetSNR_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
	     
	     case DMD_DVBS_DRV_CMD_GetPostViterbiBer:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetPostViterbiBer\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetPostViterbiBer_PARAM,spt_DVBS_GetPostViterbiBer_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_Get_FreqOffset:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_Get_FreqOffset\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_Get_FreqOffset_PARAM,spt_DVBS_Get_FreqOffset_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_BlindScan_Start:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_Start\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_Start_PARAM,spt_DVBS_BlindScan_Start_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_BlindScan_NextFreq:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_NextFreq\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_NextFreq_PARAM,spt_DVBS_BlindScan_NextFreq_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_BlindScan_Cancel:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_Cancel\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_Cancel_PARAM,spt_DVBS_BlindScan_Cancel_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_BlindScan_End:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_End\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_End_PARAM,spt_DVBS_BlindScan_End_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_BlindScan_GetChannel:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_GetChannel\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_GetChannel_PARAM,spt_DVBS_BlindScan_GetChannel_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;		 

	     case DMD_DVBS_DRV_CMD_BlindScan_GetCurrentFreq:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_GetCurrentFreq\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_GetCurrentFreq_PARAM,spt_DVBS_BlindScan_GetCurrentFreq_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_BlindScan_WaitCurFreqFinished:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_WaitCurFreqFinished\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM,spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_BlindScan_GetTunerFreq:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_BlindScan_GetTunerFreq\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_BlindScan_GetTunerFreq_PARAM,spt_DVBS_BlindScan_GetTunerFreq_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;		

	     case DMD_DVBS_DRV_CMD_DiSEqC_Init:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_Init\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_Init_PARAM,spt_DVBS_DiSEqC_Init_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_DiSEqC_SetLNBOut:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_SetLNBOut\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_SetLNBOut_PARAM,spt_DVBS_DiSEqC_SetLNBOut_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_DiSEqC_GetLNBOut:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_GetLNBOut\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_GetLNBOut_PARAM,spt_DVBS_DiSEqC_GetLNBOut_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_DiSEqC_Set22kOnOff:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_Set22kOnOff\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_Set22kOnOff_PARAM,spt_DVBS_DiSEqC_Set22kOnOff_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_DiSEqC_Get22kOnOff:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_Get22kOnOff\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_Get22kOnOff_PARAM,spt_DVBS_DiSEqC_Get22kOnOff_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_DiSEqC_SendCmd:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_SendCmd\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_SendCmd_PARAM,spt_DVBS_DiSEqC_SendCmd_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_DiSEqC_SetTxToneMode:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_SetTxToneMode\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_SetTxToneMode_PARAM,spt_DVBS_DiSEqC_SetTxToneMode_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_DiSEqC_SetTone:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_DiSEqC_SetTone\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_DiSEqC_SetTone_PARAM,spt_DVBS_DiSEqC_SetTone_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_UnicableAGCCheckPower:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_UnicableAGCCheckPower\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_UnicableAGCCheckPower_PARAM,spt_DVBS_UnicableAGCCheckPower_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;	

	     case DMD_DVBS_DRV_CMD_GetTsDivNum:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetTsDivNum\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetTsDivNum_PARAM,spt_DVBS_GetTsDivNum_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_GetCurrentDemodCodeRate:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetCurrentDemodCodeRate\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetCurrentDemodCodeRate_PARAM,spt_DVBS_GetCurrentDemodCodeRate_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;

	     case DMD_DVBS_DRV_CMD_GetTunrSignalLevel_PWR:
		     #ifdef MS_DEBUG
		     printf("kernal mode DVBSIoctl - DMD_DVBS_DRV_CMD_GetTunrSignalLevel_PWR\n");         
		     #endif
		u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_DVBS_GetTunrSignalLevel_PWR_PARAM,spt_DVBS_GetTunrSignalLevel_PWR_PARAM, buffer_arg, sizeof(buffer_arg));			
	     break;
		 
        default:
        break;

    }

    return u32Ret;

}

MS_U32 DVBS_adp_Init(FUtopiaIOctl* pIoctl)
{
    //top parameter
    UADP_SPT_BGN(&spt_DVBS_BOOL_VAL[0], sizeof(MS_BOOL));
    UADP_SPT_FIN(&spt_DVBS_BOOL_VAL[1]);  

    UADP_SPT_BGN(&spt_DVBS_U8_VAL[0], sizeof(MS_U8));
    UADP_SPT_FIN(&spt_DVBS_U8_VAL[1]);  

    UADP_SPT_BGN(&spt_DVBS_U16_VAL[0],sizeof(MS_U16));
    UADP_SPT_FIN(&spt_DVBS_U16_VAL[1]);
    
    UADP_SPT_BGN(&spt_DVBS_U32_VAL[0],sizeof(MS_U32));
    UADP_SPT_FIN(&spt_DVBS_U32_VAL[1]);
    
    UADP_SPT_BGN(&spt_DVBS_8BYTE_PARAM[0],8);
    UADP_SPT_FIN(&spt_DVBS_8BYTE_PARAM[1]);
    
    
      

    //pointer parameter
    UADP_SPT_BGN(&spt_DMD_DVBS_InitData_Transform[0],sizeof(DMD_DVBS_InitData_Transform));
    UADP_SDT_KIND(&spt_DMD_DVBS_InitData_Transform[1], DMD_DVBS_InitData_Transform, UADP_SDT_P2N, u8DMD_DVBS_DSPRegInitExt, spt_DVBS_8BYTE_PARAM);
    UADP_SDT_KIND(&spt_DMD_DVBS_InitData_Transform[2], DMD_DVBS_InitData_Transform, UADP_SDT_P2N, u8DMD_DVBS_InitExt, spt_DVBS_8BYTE_PARAM);
    UADP_SPT_FIN(&spt_DMD_DVBS_InitData_Transform[3]);  

    UADP_SPT_BGN(&spt_DMD_DVBS_Info[0],sizeof(DMD_DVBS_Info));
    UADP_SPT_FIN(&spt_DMD_DVBS_Info[1]);

    UADP_SPT_BGN(&spt_DMD_DVBS_MODULATION_TYPE[0],sizeof(DMD_DVBS_MODULATION_TYPE));
    UADP_SPT_FIN(&spt_DMD_DVBS_MODULATION_TYPE[1]);

    UADP_SPT_BGN(&spt_DMD_DVBS_DEMOD_TYPE[0],sizeof(DMD_DVBS_DEMOD_TYPE));
    UADP_SPT_FIN(&spt_DMD_DVBS_DEMOD_TYPE[1]);
	
    UADP_SPT_BGN(&spt_DVBS_LOCK_STATUS_PARAM[0],sizeof(DMD_DVBS_LOCK_STATUS));
    UADP_SPT_FIN(&spt_DVBS_LOCK_STATUS_PARAM[1]);
    
    //function input/output parameter

    UADP_SPT_BGN(&spt_DVBS_Init_PARAM[0],sizeof(DVBS_Init_PARAM));
    //UADP_SPT_NXT(&spt_DVBS_Init_PARAM[1], DVBS_Init_PARAM, pDMD_DVBS_InitData, spt_DMD_DVBS_InitData_Transform);
    UADP_SDT_KIND(&spt_DVBS_Init_PARAM[1], DVBS_Init_PARAM, UADP_SDT_P2N, pDMD_DVBS_InitData, spt_DMD_DVBS_InitData_Transform);
    UADP_SPT_FIN(&spt_DVBS_Init_PARAM[2]);  

    UADP_SPT_BGN(&spt_DVBS_EXIT_PARAM[0],sizeof(DVBS_EXIT_PARAM_PARAM));
    UADP_SPT_FIN(&spt_DVBS_EXIT_PARAM[1]);  

    UADP_SPT_BGN(&spt_DVBS_SetDbgLevel_PARAM[0],sizeof(DVBS_SetDbgLevel_PARAM));
    UADP_SPT_FIN(&spt_DVBS_SetDbgLevel_PARAM[1]); 

    UADP_SPT_BGN(&spt_DVBS_GetInfo_PARAM[0],sizeof(DVBS_GetInfo_PARAM));
    //UADP_SPT_NXT(&spt_DVBS_GetInfo_PARAM[1],DVBS_GetInfo_PARAM,ret_info,spt_DMD_DVBS_Info);
    UADP_SDT_KIND(&spt_DVBS_GetInfo_PARAM[1],DVBS_GetInfo_PARAM,UADP_SDT_P2N,ret_info,spt_DMD_DVBS_Info);
    UADP_SPT_FIN(&spt_DVBS_GetInfo_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_GetFWVer_PARAM[0],sizeof(DVBS_GetFWVer_PARAM));
    //UADP_SPT_NXT(&spt_DVBS_GetFWVer_PARAM[1],DVBS_GetFWVer_PARAM,ver,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetFWVer_PARAM[1],DVBS_GetFWVer_PARAM,UADP_SDT_P2N,ver,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetFWVer_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_GetDSPReg_PARAM[0],sizeof(DVBS_GetDSPReg_PARAM));
    //UADP_SPT_NXT(&spt_DVBS_GetDSPReg_PARAM[1],DVBS_GetDSPReg_PARAM,pu8Data,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetDSPReg_PARAM[1],DVBS_GetDSPReg_PARAM,UADP_SDT_P2N,pu8Data,spt_DVBS_U8_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetDSPReg_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_SetDSPReg_PARAM[0],sizeof(DVBS_SetDSPReg_PARAM));
    UADP_SPT_FIN(&spt_DVBS_SetDSPReg_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBS_GetReg_PARAM[0],sizeof(DVBS_GetReg_PARAM));
    //UADP_SPT_NXT(&spt_DVBS_GetReg_PARAM[1],DVBS_GetReg_PARAM,pu8Data,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetReg_PARAM[1],DVBS_GetReg_PARAM,UADP_SDT_P2N,pu8Data,spt_DVBS_U8_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetReg_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_SetReg_PARAM[0],sizeof(DVBS_SetReg_PARAM));
    UADP_SPT_FIN(&spt_DVBS_SetReg_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBS_SetSerialControl_PARAM[0],sizeof(DVBS_SetSerialControl_PARAM));
    UADP_SPT_FIN(&spt_DVBS_SetSerialControl_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBS_SetConfig_Symbol_rate_list_PARAM[0],sizeof(DVBS_SetConfig_Symbol_rate_list_PARAM));
    UADP_SDT_KIND(&spt_DVBS_SetConfig_Symbol_rate_list_PARAM[1], DVBS_SetConfig_Symbol_rate_list_PARAM,UADP_SDT_P2N,pu16_symbol_rate_list,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_SetConfig_Symbol_rate_list_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_BlindScan_Config_Symbol_rate_list_PARAM[0],sizeof(DVBS_BlindScan_Config_Symbol_rate_list_PARAM));
	// oga add 20160106
	UADP_SDT_KIND(&spt_DVBS_BlindScan_Config_Symbol_rate_list_PARAM[1], DVBS_BlindScan_Config_Symbol_rate_list_PARAM,UADP_SDT_P2N,pu16_symbol_rate_list,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_BlindScan_Config_Symbol_rate_list_PARAM[2]);
	
    UADP_SPT_BGN(&spt_DVBS_SetActive_PARAM[0],sizeof(DVBS_SetActive_PARAM));
    UADP_SPT_FIN(&spt_DVBS_SetActive_PARAM[1]);   

    UADP_SPT_BGN(&spt_DVBS_GetLockWithRFPower[0],sizeof(DVBS_GetLockWithRFPower));
    //UADP_SPT_NXT(&spt_DVBS_GetLockWithRFPower_Transform[1],DVBS_GetLockWithRFPower_Transform,eLockStatus,spt_DVBS_LOCK_STATUS_PARAM);
    UADP_SDT_KIND(&spt_DVBS_GetLockWithRFPower[1],DVBS_GetLockWithRFPower,UADP_SDT_P2N,eLockStatus,spt_DVBS_LOCK_STATUS_PARAM);
    UADP_SPT_FIN(&spt_DVBS_GetLockWithRFPower[2]);

    UADP_SPT_BGN(&spt_DVBS_GetSignalStrengthWithRFPower[0],sizeof(DVBS_GetSignalStrengthWithRFPower_PARAM));
    UADP_SDT_KIND(&spt_DVBS_GetSignalStrengthWithRFPower[1],DVBS_GetSignalStrengthWithRFPower_PARAM,UADP_SDT_P2N,pDemodType,spt_DMD_DVBS_DEMOD_TYPE);
    UADP_SDT_KIND(&spt_DVBS_GetSignalStrengthWithRFPower[2],DVBS_GetSignalStrengthWithRFPower_PARAM,UADP_SDT_P2N,u8_DVBS2_CurrentCodeRateLocal,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetSignalStrengthWithRFPower[3],DVBS_GetSignalStrengthWithRFPower_PARAM,UADP_SDT_P2N,u8_DVBS2_CurrentConstellationLocal,spt_DVBS_U8_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetSignalStrengthWithRFPower[4]);

    UADP_SPT_BGN(&spt_DVBS_ActiveDmdSwitch_PARAM[0],sizeof(DVBS_ActiveDmdSwitch_PARAM));
    UADP_SPT_FIN(&spt_DVBS_ActiveDmdSwitch_PARAM[1]);

    UADP_SPT_BGN(&spt_DVBS_GetPacketErr_PARAM[0],sizeof(DVBS_GetPacketErr_PARAM));
    //UADP_SPT_NXT(&spt_DVBS_GetPacketErr_PARAM[1],DVBS_GetPacketErr_PARAM,pktErr,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetPacketErr_PARAM[1],DVBS_GetPacketErr_PARAM,UADP_SDT_P2N,pktErr,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetPacketErr_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_GetCellID_PARAM[0],sizeof(DVBS_GetCellID_PARAM));
    //UADP_SPT_NXT(&spt_DVBS_GetCellID_PARAM[1],DVBS_GetCellID_PARAM,u16CellID,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetCellID_PARAM[1],DVBS_GetCellID_PARAM,UADP_SDT_P2N,u16CellID,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetCellID_PARAM[2]);

     UADP_SPT_BGN(&spt_DVBS_GetStatus_PARAM[0],sizeof(DVBS_GetStatus_PARAM));
     //UADP_SPT_NXT(&spt_DVBS_GetStatus_PARAM_Transform[1],DVBS_GetStatus_PARAM_Transform,pQAMMode,spt_DMD_DVBS_MODULATION_TYPE);
     //UADP_SPT_NXT(&spt_DVBS_GetStatus_PARAM_Transform[2],DVBS_GetStatus_PARAM_Transform,u16SymbolRate,spt_DVBS_U16_VAL);
     UADP_SDT_KIND(&spt_DVBS_GetStatus_PARAM[1],DVBS_GetStatus_PARAM,UADP_SDT_P2N,pQAMMode,spt_DMD_DVBS_MODULATION_TYPE);
     UADP_SDT_KIND(&spt_DVBS_GetStatus_PARAM[2],DVBS_GetStatus_PARAM,UADP_SDT_P2N,u32SymbolRate,spt_DVBS_U32_VAL);
     UADP_SDT_KIND(&spt_DVBS_GetStatus_PARAM[3],DVBS_GetStatus_PARAM,UADP_SDT_P2N,s16CFO,spt_DVBS_S16_VAL);
     UADP_SPT_FIN(&spt_DVBS_GetStatus_PARAM[4]);
	
	
    UADP_SPT_BGN(&spt_DVBS_SetPowerState_PARAM[0],sizeof(DVBS_SetPowerState_PARAM));
    UADP_SPT_FIN(&spt_DVBS_SetPowerState_PARAM[1]);  

    UADP_SPT_BGN(&spt_DVBS_Get_FreqOffset_PARAM[0],sizeof(DVBS_Get_FreqOffset_PARAM));
    UADP_SDT_KIND(&spt_DVBS_Get_FreqOffset_PARAM[1],DVBS_GetStatus_PARAM,UADP_SDT_P2N,s16CFO,spt_DVBS_S16_VAL);
    UADP_SPT_FIN(&spt_DVBS_Get_FreqOffset_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_GetIFAGC_PARAM[0],sizeof(DVBS_GetIFAGC_PARAM));
    UADP_SDT_KIND(&spt_DVBS_GetIFAGC_PARAM[1],DVBS_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_reg,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetIFAGC_PARAM[2],DVBS_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_reg_lsb,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetIFAGC_PARAM[3],DVBS_GetIFAGC_PARAM,UADP_SDT_P2N,ifagc_err_reg,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetIFAGC_PARAM[4]);

    UADP_SPT_BGN(&spt_DVBS_GetSNR_PARAM[0],sizeof(DVBS_GetSNR_PARAM));
    UADP_SDT_KIND(&spt_DVBS_GetSNR_PARAM[1],DVBS_GetSNR_PARAM,UADP_SDT_P2N,u32NDA_SNR_A,spt_DVBS_U32_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetSNR_PARAM[2],DVBS_GetSNR_PARAM,UADP_SDT_P2N,u32NDA_SNR_AB,spt_DVBS_U32_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetSNR_PARAM[3]);

    UADP_SPT_BGN(&spt_DVBS_GetPostViterbiBer_PARAM[0],sizeof(DVBS_GetPostViterbiBer_PARAM));
    UADP_SDT_KIND(&spt_DVBS_GetPostViterbiBer_PARAM[1],DVBS_GetPostViterbiBer_PARAM,UADP_SDT_P2N,BitErr_reg,spt_DVBS_U32_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetPostViterbiBer_PARAM[2],DVBS_GetPostViterbiBer_PARAM,UADP_SDT_P2N,BitErrPeriod_reg,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetPostViterbiBer_PARAM[3]);

    UADP_SPT_BGN(&spt_DVBS_BlindScan_NextFreq_PARAM[0],sizeof(DVBS_BlindScan_NextFreq_PARAM));
    UADP_SDT_KIND(&spt_DVBS_BlindScan_NextFreq_PARAM[1],DVBS_BlindScan_NextFreq_PARAM,UADP_SDT_P2N,bBlindScanEnd,spt_DVBS_BOOL_VAL);
    UADP_SPT_FIN(&spt_DVBS_BlindScan_NextFreq_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_BlindScan_GetChannel_PARAM[0],sizeof(DVBS_BlindScan_GetChannel_PARAM));
    UADP_SDT_KIND(&spt_DVBS_BlindScan_GetChannel_PARAM[1],DVBS_BlindScan_GetChannel_PARAM,UADP_SDT_P2N,u16TPNum,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_BlindScan_GetChannel_PARAM[2],DVBS_BlindScan_GetChannel_PARAM,UADP_SDT_P2N,pTable,spt_HAL_DEMOD_MS_FE_CARRIER_PARAM);
    UADP_SPT_FIN(&spt_DVBS_BlindScan_GetChannel_PARAM[3]);	

    UADP_SPT_BGN(&spt_DVBS_BlindScan_GetCurrentFreq_PARAM[0],sizeof(DVBS_BlindScan_GetCurrentFreq_PARAM));
    UADP_SDT_KIND(&spt_DVBS_BlindScan_GetCurrentFreq_PARAM[1],DVBS_BlindScan_GetCurrentFreq_PARAM,UADP_SDT_P2N,u32CurrentFreq,spt_DVBS_U32_VAL);
    UADP_SPT_FIN(&spt_DVBS_BlindScan_GetCurrentFreq_PARAM[2]);

    UADP_SPT_BGN(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[0],sizeof(DVBS_BlindScan_WaitCurFreqFinished_PARAM));
    UADP_SDT_KIND(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[1],DVBS_BlindScan_WaitCurFreqFinished_PARAM,UADP_SDT_P2N,u8Progress,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[2],DVBS_BlindScan_WaitCurFreqFinished_PARAM,UADP_SDT_P2N,u8FindNum,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[3],DVBS_BlindScan_WaitCurFreqFinished_PARAM,UADP_SDT_P2N,substate_reg,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[4],DVBS_BlindScan_WaitCurFreqFinished_PARAM,UADP_SDT_P2N,u32Data,spt_DVBS_U32_VAL);
    UADP_SDT_KIND(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[5],DVBS_BlindScan_WaitCurFreqFinished_PARAM,UADP_SDT_P2N,symbolrate_reg,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[6],DVBS_BlindScan_WaitCurFreqFinished_PARAM,UADP_SDT_P2N,CFO_reg,spt_DVBS_U16_VAL);	
    UADP_SPT_FIN(&spt_DVBS_BlindScan_WaitCurFreqFinished_PARAM[7]);	

    UADP_SPT_BGN(&spt_DVBS_BlindScan_GetTunerFreq_PARAM[0],sizeof(DVBS_BlindScan_GetTunerFreq_PARAM));
    UADP_SDT_KIND(&spt_DVBS_BlindScan_GetTunerFreq_PARAM[1],DVBS_BlindScan_GetTunerFreq_PARAM,UADP_SDT_P2N,u16TunerCenterFreq,spt_DVBS_U16_VAL);
    UADP_SDT_KIND(&spt_DVBS_BlindScan_GetTunerFreq_PARAM[2],DVBS_BlindScan_GetTunerFreq_PARAM,UADP_SDT_P2N,u16TunerCutOffFreq,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_BlindScan_GetTunerFreq_PARAM[3]);	

    UADP_SPT_BGN(&spt_DVBS_DiSEqC_GetLNBOut_PARAM[0],sizeof(DVBS_DiSEqC_GetLNBOut_PARAM));
    UADP_SDT_KIND(&spt_DVBS_DiSEqC_GetLNBOut_PARAM[1],DVBS_DiSEqC_GetLNBOut_PARAM,UADP_SDT_P2N,bLNBOutLow,spt_DVBS_BOOL_VAL);
    UADP_SPT_FIN(&spt_DVBS_DiSEqC_GetLNBOut_PARAM[2]);	

    UADP_SPT_BGN(&spt_DVBS_DiSEqC_Get22kOnOff_PARAM[0],sizeof(DVBS_DiSEqC_Get22kOnOff_PARAM));
    UADP_SDT_KIND(&spt_DVBS_DiSEqC_Get22kOnOff_PARAM[1],DVBS_DiSEqC_Get22kOnOff_PARAM,UADP_SDT_P2N,b22kOn,spt_DVBS_BOOL_VAL);
    UADP_SPT_FIN(&spt_DVBS_DiSEqC_Get22kOnOff_PARAM[2]);	

    UADP_SPT_BGN(&spt_DVBS_DiSEqC_SendCmd_PARAM[0],sizeof(DVBS_DiSEqC_SendCmd_PARAM));
    UADP_SDT_KIND(&spt_DVBS_DiSEqC_SendCmd_PARAM[1],DVBS_DiSEqC_SendCmd_PARAM,UADP_SDT_P2N,pCmd,spt_DVBS_U8_VAL);
    UADP_SPT_FIN(&spt_DVBS_DiSEqC_SendCmd_PARAM[2]);	

    UADP_SPT_BGN(&spt_DVBS_GetTsDivNum_PARAM[0],sizeof(DVBS_GetTsDivNum_PARAM));
    UADP_SDT_KIND(&spt_DVBS_GetTsDivNum_PARAM[1],DVBS_GetTsDivNum_PARAM,UADP_SDT_P2N,u32SymbolRate,spt_DVBS_U32_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetTsDivNum_PARAM[2],DVBS_GetTsDivNum_PARAM,UADP_SDT_P2N,system_type_reg,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetTsDivNum_PARAM[3],DVBS_GetTsDivNum_PARAM,UADP_SDT_P2N,code_rate_idx,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetTsDivNum_PARAM[4],DVBS_GetTsDivNum_PARAM,UADP_SDT_P2N,fec_type_idx,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetTsDivNum_PARAM[5],DVBS_GetTsDivNum_PARAM,UADP_SDT_P2N,pilot_flag,spt_DVBS_U8_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetTsDivNum_PARAM[6],DVBS_GetTsDivNum_PARAM,UADP_SDT_P2N,u32temp,spt_DVBS_U32_VAL);
    UADP_SDT_KIND(&spt_DVBS_GetTsDivNum_PARAM[7],DVBS_GetTsDivNum_PARAM,UADP_SDT_P2N,code_rate_reg,spt_DVBS_U8_VAL);		
    UADP_SPT_FIN(&spt_DVBS_GetTsDivNum_PARAM[8]);	

    UADP_SPT_BGN(&spt_DVBS_GetCurrentDemodCodeRate_PARAM[0],sizeof(DVBS_GetCurrentDemodCodeRate_PARAM));
    UADP_SDT_KIND(&spt_DVBS_GetCurrentDemodCodeRate_PARAM[1],DVBS_GetCurrentDemodCodeRate_PARAM,UADP_SDT_P2N,pCodeRate,spt_DMD_DVBS_CODE_RATE_TYPE);
    UADP_SPT_FIN(&spt_DVBS_GetCurrentDemodCodeRate_PARAM[2]);	

    UADP_SPT_BGN(&spt_DVBS_GetTunrSignalLevel_PWR_PARAM[0],sizeof(DVBS_GetTunrSignalLevel_PWR_PARAM));
    UADP_SDT_KIND(&spt_DVBS_GetTunrSignalLevel_PWR_PARAM[1],DVBS_GetTunrSignalLevel_PWR_PARAM,UADP_SDT_P2N,u16Data,spt_DVBS_U16_VAL);
    UADP_SPT_FIN(&spt_DVBS_GetTunrSignalLevel_PWR_PARAM[2]);	
	
   *pIoctl= (FUtopiaIOctl)DVBS_adp_Ioctl;

    return TRUE;
}






