
//************************Kernel Module Define********************************//
//Please Don't Change Code Order.
#include "MsTypes.h"
#include "utopia_driver_id.h"

#if 0
typedef enum{
#define PREFIX(MODULE)  CONFIG_API_##MODULE##_MODULE=0,
    INCLUDED_MODULE
#undef PREFIX
}eCONFIG_API_MODULE;

typedef enum{
#define PREFIX(MODULE)  CONFIG_DRV_##MODULE##_MODULE=0,
    INCLUDED_MODULE
#undef PREFIX
}eCONFIG_DRV_MODULE;
#endif

#ifdef CONFIG_ENABLE_MENUCONFIG
#include <autoconf.h>
#endif
//*************************************************************************//

#include "MsCommon.h"
#include "utopia_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "drvMMIO.h"

#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

UTOPIA_PRIVATE* psUtopiaPrivate;

char moduleNames[][40] = {
#define PREFIX(MODULE) "MODULE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char rpoolNames[][40] = {
#define PREFIX(MODULE) "RPOOL_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char ResourceNames[][40] = {
#define PREFIX(MODULE) "RESOURCE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

unsigned int moduleMode[] = {
#define PREFIX(MODULE) 0,
	INCLUDED_MODULE
#undef PREFIX
};
	
#if 0//defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)

#define PREFIX(MODULE) MS_U32 MODULE_##MODULE##_OPEN = MODULE_##MODULE | \
    ((CONFIG_API_##MODULE##_MODULE | CONFIG_DRV_##MODULE##_MODULE) << 31);
INCLUDED_MODULE
#undef PREFIX

#endif

#define UTOPIA_VERSION "libutopia-v02.01-20170812"
MS_U32 UTOPIARegisterToUtopia(FUtopiaOpen ModuleType);
#define PREFIX(MODULE) \
	extern MS_U32 MODULE##RegisterToUtopia(FUtopiaOpen ModuleType);
_INCLUDED_MODULE
#undef PREFIX

//#define CONFIG_UTOPIA_MODE_BY_BOOTARQ

#ifdef CONFIG_UTOPIA_MODE_BY_BOOTARQ

#define BOOTARG_SIZE 2048
static unsigned int UtopiaParserBootArg(char* source,char* pattern,unsigned int size)
{
    char sCmdline[BOOTARG_SIZE]={0};
    FILE* fpProcCmdlineFile = NULL;
    int u32ReadCount = 0;
    char* pFindString = NULL;
    char* sTemp = NULL;
	
    unsigned int u32CmdlineLen = 0;
    

    fpProcCmdlineFile = fopen("/proc/cmdline","r");

    if(fpProcCmdlineFile==NULL)
    {
        printu("[utopia info]%d : can't open /proc/cmdline",__LINE__);
        return 0;
    }

    u32ReadCount = fread(sCmdline,1,BOOTARG_SIZE,fpProcCmdlineFile);

    if(u32ReadCount > BOOTARG_SIZE)
    {
        printu("[utopia info]%d warring : cmdline info more than buffer size\n",__LINE__);
    }
    u32CmdlineLen = strlen(sCmdline);
    
    //printu("[utopia info]:cmdline = %s \n",sCmdline); 
    //printu("[utopia info]:string len = %d \n",u32CmdlineLen);

    pFindString=strstr(sCmdline,pattern);

    if(pFindString==NULL)
    {
		printu("[utopia info] can't serch %s in bootargs \n",pattern);
        return 0;
    }
    sTemp = sCmdline;
    while(strsep(&sTemp," "));  //covert space to "\0"
    
    sTemp=strsep(&pFindString,"=");
    
    u32ReadCount = strlen(pFindString);
    
    if(u32ReadCount>size)
    {
        strncpy(source, pFindString, size);
        u32ReadCount = size;
    }else
    {
        strncpy(source, pFindString, u32ReadCount);
    }
    
    //printu("%s\n",pFindString);
    //printu("lens = %d\n",strlen(pFindString));
    
    return u32ReadCount;
}
#endif
#ifndef CONFIG_UTOPIA_MODE_DEFAULT_KERNEL
static MS_U32 UtopiaConfigReadFile(char* filePath)
{
    char sModule[40], sEqual[20];
    unsigned int u32moduleMode = 0;
    unsigned int u32Cnt =0;
    char sConf[50], *tok=" \n", *pch;
    int len = 0;

    FILE* fpUtopiaModuleMode = NULL;

#ifdef CONFIG_UTOPIA_MODE_BY_BOOTARQ
    char sFilePathFormBootArq[40] = {0};
    unsigned int u32RetParserBootArg = 0;
    u32RetParserBootArg = UtopiaParserBootArg(sFilePathFormBootArq,"utopia_mode",40);
    
    if(u32RetParserBootArg == 0)
        return 0;
    
    //printu("utopia_conf path : %s \n",sFilePathFormBootArq);
    printu("[utopia info] open: %s\n", sFilePathFormBootArq);
    fpUtopiaModuleMode = fopen(sFilePathFormBootArq, "r");
#else
    printu("[utopia info] open: %s\n", CONFIG_UTOPIAXP_CONF_PATH);
    fpUtopiaModuleMode = fopen(CONFIG_UTOPIAXP_CONF_PATH, "r");
    if(fpUtopiaModuleMode==NULL)  //forward compatible ,remove later
    {
        fpUtopiaModuleMode = fopen("/data/utopia.conf", "r");
    }
#endif
    if(fpUtopiaModuleMode==NULL)
    {
        printu("[utopia info] don't have utopia.conf\n");
        return 0;
    }

    // coverity[secure_coding]
    while(fgets(sConf,sizeof(sConf),fpUtopiaModuleMode)!=NULL)
    {
        if(strncmp(sConf,"MODULE_",7)!=0)
        {
            continue;
        }
        memset(sModule,0,sizeof(sModule));
        memset(sEqual,0,sizeof(sEqual));
        pch = strtok(sConf, tok);
        len = (strlen(pch)>sizeof(sModule))?sizeof(sModule):strlen(pch);
        strncpy(sModule,pch,len);
        pch = strtok(NULL, tok);
        len = (strlen(pch)>sizeof(sEqual))?sizeof(sEqual):strlen(pch);
        strncpy(sEqual,pch,len);
        pch = strtok(NULL, tok);
        u32moduleMode = atoi(pch);
        //printf("11. %s %s %d\n", sModule, sEqual, u32moduleMode);

        for(u32Cnt = 0;u32Cnt<eMODULE_END;u32Cnt++)
        {
            if (0== strcmp((const char*)sModule, (const char*)moduleNames[u32Cnt]))
            {
                if(u32moduleMode == 1)
                {
                    moduleMode[u32Cnt] = KERNEL_MODE;
                    printu("%s = %d \n",moduleNames[u32Cnt],u32moduleMode);
                }else
                {
                    moduleMode[u32Cnt] = 0;
                }
                break;
            }
			
        }
        u32moduleMode = 0 ; //init
    }

    fclose(fpUtopiaModuleMode);	
    return 0;
}
#endif

MS_U32 UtopiaInit()
{
	MS_U32 u32Ret = 0;

	printu("[utopia info] utopia init version: %s\n", UTOPIA_VERSION);
    psUtopiaPrivate = (UTOPIA_PRIVATE*)malloc(sizeof(UTOPIA_PRIVATE));
	memset(psUtopiaPrivate, 0, sizeof(UTOPIA_PRIVATE));
	psUtopiaPrivate->u32MutexID = MsOS_CreateMutex(E_MSOS_FIFO
			, "UtopiaPrivateMutex", MSOS_PROCESS_SHARED);

	MsOS_ObtainMutex(psUtopiaPrivate->u32MutexID, MSOS_WAIT_FOREVER);
#ifdef CONFIG_UTOPIA_MODE_DEFAULT_KERNEL
#define MODULE_DEFAULT_KERNEL \
    PREFIX(XC) \
    PREFIX(DIP) \
    PREFIX(PNL) \
    PREFIX(ACE) \
    PREFIX(DLC) \
    PREFIX(PQ) \
    PREFIX(TVENCODER) \
    PREFIX(HDMIRX) \
    PREFIX(BDMA) \
    PREFIX(AESDMA) \
    PREFIX(HWI2C) \
    PREFIX(MIU) \
    PREFIX(VIF) \
    PREFIX(AVD) \
    PREFIX(MVOP) \
    PREFIX(GOP) \
    PREFIX(GFX) \
    PREFIX(GPD) \
    PREFIX(DTMB) \
    PREFIX(CEC) \
    PREFIX(MHL) \
    PREFIX(AUDIO) \
    PREFIX(NJPEG_EX) \
    PREFIX(VDEC_EX) \
    PREFIX(DSCMB) \
    PREFIX(DMX) \
    PREFIX(VBI) \

#define PREFIX(MODULE) moduleMode[MODULE_##MODULE] = KERNEL_MODE;
    MODULE_DEFAULT_KERNEL
#undef PREFIX
#else
	UtopiaConfigReadFile(CONFIG_UTOPIAXP_CONF_PATH);
#endif
    UTOPIARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_UTOPIA_FULL);
#define PREFIX(MODULE) \
	if (MODULE##RegisterToUtopia != NULL && MODULE_TYPE_##MODULE##_FULL) \
	{ \
		u32Ret |= MODULE##RegisterToUtopia((FUtopiaOpen)MODULE_TYPE_##MODULE##_FULL); \
	}
	_INCLUDED_MODULE
#undef PREFIX
	MsOS_ReleaseMutex(psUtopiaPrivate->u32MutexID);

	return u32Ret;
}

MS_U32 UtopiaOpen(MS_U32 u32ModuleID, void** ppInstanceTmp
		, MS_U32 u32ModuleVersion, const void* const pAttribute)
{
    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    //UTOPIA_USER_INSTANCE ** ppInstance= (UTOPIA_USER_INSTANCE**)ppInstanceTmp;
    MS_S32              _s32UtopiaFd = -1;
	UTOPIA_DDI_OPEN_ARG sOpenArg;
	UTOPIA_USER_INSTANCE* pInstance = NULL;
	while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == (u32ModuleID&~KERNEL_MODE))
        {
            MS_U32 u32ModuleIndex = u32ModuleID&~KERNEL_MODE;
            pInstance= malloc(sizeof(UTOPIA_USER_INSTANCE));

            if(pInstance == NULL)
            {
                printu("utopia.c : malloc fail %d \n",__LINE__);
                return UTOPIA_STATUS_FAIL;
            }else
            {
//                printu("utopia.c : malloc ok,Module= %s \n",moduleNames[u32ModuleIndex]);
            }

             //if(u32ModuleID & KERNEL_MODE)
             if(moduleMode[u32ModuleIndex] & KERNEL_MODE)
			 {
			     if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
			     {
			         printu("aa : 0x%x  Open /proc/utopia fail aa\n", u32ModuleID);
                                 free(pInstance);//coverity Resource leak
                                 return UTOPIA_STATUS_FAIL;
			     }
//                 printu("Module : %s  kernel mode\n",moduleNames[u32ModuleIndex]);
//				 printu("_s32UtopiaFd : %d  \n",_s32UtopiaFd);
				 sOpenArg.u32ModuleID = psUtopiaModule->u32ModuleID;
				 sOpenArg.u32ModuleVersion = u32ModuleVersion;
				 sOpenArg.pAttribute = (void*)pAttribute;
				 if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_SetMODULE,&sOpenArg)!=UTOPIA_STATUS_SUCCESS)
				 {
				     printu("utopia.c : UtopiaOpen fail %d \n",__LINE__);
				     free(pInstance);
					 close(_s32UtopiaFd);
				     return UTOPIA_STATUS_FAIL;
				 }
				 //while(1);
			     pInstance->psUtopiaInstant = 0;
				 pInstance->s32Fd = _s32UtopiaFd;
			     pInstance->u32KernelSpaceIdentify =KERNEL_MODE;
				 pInstance->u32ModuleID = psUtopiaModule->u32ModuleID;
				 *ppInstanceTmp = pInstance;
				 return UTOPIA_STATUS_SUCCESS;
             }else
             {
//                 printu("Module : %s  user mode\n",moduleNames[u32ModuleIndex]);
                 MS_U32 ret = psUtopiaModule->fpOpen(&(pInstance->psUtopiaInstant), pAttribute);

				 if(MSOS_BRANCH_PREDICTION_UNLIKELY(ret))
                 {
                     printu("[utopia open error] fail to create instance\n");
				     RET_OR_BLOCK(ret);
                 }
                 ((UTOPIA_INSTANCE*)(pInstance->psUtopiaInstant))->psModule =psUtopiaModule;
				 ((UTOPIA_INSTANCE*)(pInstance->psUtopiaInstant))->u32AppRequireModuleVersion = u32ModuleVersion;
				 //(*ppInstance)->psModule = psUtopiaModule;
			     //(*ppInstance)->u32AppRequireModuleVersion = u32ModuleVersion;
				 pInstance->u32KernelSpaceIdentify = 0;
				 pInstance->s32Fd = 0;

				 *ppInstanceTmp = pInstance;
				 return ret; /* depend on fpOpen, may not be UTOPIA_STATUS_SUCCESS */
             }
        }
		psUtopiaModule = psUtopiaModule->psNext;
    }
	return UTOPIA_STATUS_FAIL;
#else
    UTOPIA_INSTANCE** ppInstance = (UTOPIA_INSTANCE**)ppInstanceTmp;


    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == u32ModuleID)
        {
            MS_U32 ret = psUtopiaModule->fpOpen((void**)ppInstance, pAttribute);

            if(MSOS_BRANCH_PREDICTION_UNLIKELY(ret))
            {
                printu("[utopia open error] fail to create instance\n");
				RET_OR_BLOCK(ret);
            }

            (*ppInstance)->psModule = psUtopiaModule;
			(*ppInstance)->u32AppRequireModuleVersion = u32ModuleVersion;
            return ret; /* depend on fpOpen, may not be UTOPIA_STATUS_SUCCESS */
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }
	return UTOPIA_STATUS_FAIL;
#endif
}

MS_U32 UtopiaIoctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    UTOPIA_DDI_IOCTL_ARG sIOCTLArg;
    UTOPIA_USER_INSTANCE* pInstance = (UTOPIA_USER_INSTANCE*)pInstanceTmp;
    MS_U32 ret = 0;

	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pInstance == NULL))
	{
		printu("[utopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	if((pInstance->u32KernelSpaceIdentify) & KERNEL_MODE)
	{
		sIOCTLArg.u32Cmd = u32Cmd;
		sIOCTLArg.pArg = pArgs;

		ret = ioctl((pInstance->s32Fd),UTOPIA_IOCTL_IoctlMODULE,&sIOCTLArg);
		if(ret != 0)
		{

		    //printf("UtopiaIoctl error %d \n",ret);
			//printf("Module: %s  Cmd: %d \n",moduleNames[pInstance->u32ModuleID] , u32Cmd);
			//print_trace();
		    //RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
		}

	    //ioctl(((MS_S32)(pInstance->psUtopiaInstant)),u32Cmd,pArgs);
		return ret;
	}else
	{
	   return TO_INSTANCE_PTR((pInstance->psUtopiaInstant))->psModule->fpIoctl((pInstance->psUtopiaInstant),
			u32Cmd, pArgs);
	}
#else
	UTOPIA_INSTANCE* pInstance = (UTOPIA_INSTANCE*)pInstanceTmp;
	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pInstance == NULL))
	{
		printu("[utopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	return TO_INSTANCE_PTR(pInstance)->psModule->fpIoctl(pInstance,
			u32Cmd, pArgs);
#endif
}

MS_U32 UtopiaClose(void* pInstantTmp)
{
#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    UTOPIA_USER_INSTANCE* pInstance = (UTOPIA_USER_INSTANCE*)pInstantTmp;
    //UTOPIA_INSTANCE* pInstant = (UTOPIA_INSTANCE*)pInstantTmp;

    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
	MS_U32 u32Ret = UTOPIA_STATUS_FAIL;

	if((pInstance->u32KernelSpaceIdentify) & KERNEL_MODE)
	{
	   close((int)(pInstance->s32Fd));
	   free(pInstance);
	   return 0;//fix me
	}else
	{
        while(psUtopiaModule != NULL)
        {
            if(psUtopiaModule->u32ModuleID == (TO_INSTANCE_PTR(pInstance->psUtopiaInstant))->psModule->u32ModuleID)
            {
                u32Ret = psUtopiaModule->fpClose(pInstance->psUtopiaInstant);
				if(u32Ret==UTOPIA_STATUS_SUCCESS)
				{
                    //printf("utopia.c : UtopiaClose ok,Module= %s \n",moduleNames[psUtopiaModule->u32ModuleID]);
					free(pInstantTmp);
				}
				else
					printf("UtopiaClose fail : Module  = %s \n",moduleNames[psUtopiaModule->u32ModuleID]);


				return u32Ret;
            }
            psUtopiaModule = psUtopiaModule->psNext;
        }
	return 0;
	}
#else
    UTOPIA_INSTANCE* pInstance = (UTOPIA_INSTANCE*)pInstantTmp;

	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pInstance == NULL))
	{
		printu("[utopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == pInstance->psModule->u32ModuleID)
        {
            return psUtopiaModule->fpClose(pInstance);
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }
	return 0;
#endif
}

