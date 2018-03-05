//#include <nuttx/config.h>
#include <stdio.h>
//#include "MsTypes.h"
//#include "stdlib.h"
#include "utopia.h" // for utopia open/ioctl/close
#include "utopia_driver_id.h" // for module id
#include "MsOS.h" // for MsOS_ObtainMutex test
//#include "drvMMIO.h"
//#include <string.h>
//#include <unistd.h>
//#include "drvMIU.h"

void __aeabi_unwind_cpp_pr1(void) {}
void __aeabi_unwind_cpp_pr0(void) {}

#define SEM
//#define SYS_DMD_VD_MBX
//#define CMDQ
//#define SYS

#ifdef SEM
#include "main_sem.h"
#endif
#ifdef SYS_DMD_VD_MBX
#endif
#ifdef CMDQ
#endif
#ifdef SYS
#include "main_sys.h"
#endif


int main(int argc, char *argv[])
{
	unsigned int ret = 0;
    UtopiaInit();

	void* pInstance = NULL;
	void* pArgs = NULL;

#ifdef SEM
    // SEM
    ret = UtopiaOpen(MODULE_SEM, &pInstance, 0, NULL);
    if (!ret)
    {
        printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
    }
    if(pInstance != NULL)
    {
        //sleep(5);
        // try to use ioctl by the instance we got
        ////////
        pArgs = malloc(sizeof(SEM_RESETRESOURCE_PARAM));

        ((PSEM_GETRESOURCE_PARAM)(pArgs))->u8SemID = 0x07;

        UtopiaIoctl(pInstance, MDrv_CMD_SEM_Reset_Resource, pArgs);
        free(pArgs);
        
        ////////
        pArgs = malloc(sizeof(SEM_GETRESOURCE_PARAM));

        ((PSEM_GETRESOURCE_PARAM)(pArgs))->u8SemID = 0x07;
        ((PSEM_GETRESOURCE_PARAM)(pArgs))->u16ResId = 2;

        UtopiaIoctl(pInstance, MDrv_CMD_SEM_Get_Resource, pArgs);
        free(pArgs);
       
        /////////
        /*
        pArgs = malloc(sizeof(SEM_FREERESOURCE_PARAM));

        ((PSEM_GETRESOURCE_PARAM)(pArgs))->u8SemID = 0x07;
        ((PSEM_GETRESOURCE_PARAM)(pArgs))->u16ResId = 1;

        UtopiaIoctl(pInstance, MDrv_CMD_SEM_Free_Resource, pArgs);
        free(pArgs);
        */

        /////////
        pArgs = malloc(sizeof(SEM_GETRESOURCEID_PARAM));

        ((PSEM_GETRESOURCEID_PARAM)(pArgs))->u8SemID = 0x07;
        ((PSEM_GETRESOURCEID_PARAM)(pArgs))->pu16ResId = malloc(sizeof(MS_U16));

        UtopiaIoctl(pInstance, MDrv_CMD_SEM_Get_ResourceID, pArgs);
        printf("\033[35mFunction = %s, Line = %d, u16ResId is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, *((PSEM_GETRESOURCEID_PARAM)(pArgs))->pu16ResId); // joe.liu
        free(pArgs);
        
        /////////
        pArgs = malloc(sizeof(MS_U32));

        UtopiaIoctl(pInstance, MDrv_CMD_SEM_Get_Num, pArgs);
        printf("\033[35mFunction = %s, Line = %d, SEM_MAX_NUM is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, *((MS_U32 *)(pArgs))); // joe.liu
        free(pArgs);
        
        
        
        
        
        
        printf("\n\033[35mFunction = %s, Line = %d, Instance closing\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
        UtopiaClose(pInstance);
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, No Instance can be closed\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
    }
    printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
#endif

#ifdef SYS_DMD_VD_MBX
    // SYS_DMD_VD_MBX
    ret = UtopiaOpen(MODULE_SYS_DMD_VD_MBX, &pInstance, 0, NULL);
    if (!ret)
    {
        printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
    }
    if(pInstance != NULL)
    {
        sleep(5);
        printf("\033[35mFunction = %s, Line = %d, Instance closing\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
        UtopiaClose(pInstance);
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, No Instance can be closed\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
    }
    printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
#endif

#ifdef CMDQ
    // CMDQ
    ret = UtopiaOpen(MODULE_CMDQ, &pInstance, 0, NULL);
	if (!ret)
    {
		printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
    }
    if(pInstance != NULL)
    {
        sleep(5);
        printf("\033[35mFunction = %s, Line = %d, Instance closing\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
        UtopiaClose(pInstance); 
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, No Instance can be closed\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
    }
    printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
#endif

#ifdef SYS
    // SYS
    ret = UtopiaOpen(MODULE_SYS, &pInstance, 0, NULL);
	if (!ret)
    {
		printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
    }
    if(pInstance != NULL)
    {
        //sleep(5);
        // try to use ioctl by the instance we got
        pArgs = malloc(sizeof(int));
        UtopiaIoctl(pInstance, MDrv_CMD_SYS_GetChipRev, pArgs);
        printf("\033[35mFunction = %s, Line = %d, ChipRev is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, *(MS_U8 *)pArgs); // joe.liu
        free(pArgs);

        pArgs = malloc(sizeof(int));
        UtopiaIoctl(pInstance, MDrv_CMD_SYS_GetChipID, pArgs);
        printf("\033[35mFunction = %s, Line = %d, ChipID vaule is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, *(MS_U16 *)pArgs); // joe.liu
        free(pArgs);

        pArgs = malloc(10 * sizeof(int));
        UtopiaIoctl(pInstance, MDrv_CMD_SYS_GetInfo, pArgs);
        printf("\033[35mFunction = %s, Line = %d, Return address is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, pArgs); // joe.liu
        free(pArgs);
        
        pArgs = malloc(20 * sizeof(int));
        UtopiaIoctl(pInstance, MDrv_CMD_SYS_GetLibVer, pArgs);
        printf("\033[35mFunction = %s, Line = %d, Return address is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, pArgs); // joe.liu
        free(pArgs);
        
        printf("\n\033[35mFunction = %s, Line = %d, Instance closing\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
        UtopiaClose(pInstance); 
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, No Instance can be closed\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
    }
    printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
#endif

/*
    // test for open second Instance
    ret = UtopiaOpen(MODULE_CMDQ, &pInstance, 0, NULL);
	if (!ret)
    {
		printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
    }
    printf("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu

    // test for release Instance, then open again
    UtopiaClose(pInstance); 
    ret = UtopiaOpen(MODULE_CMDQ, &pInstance, 0, NULL);
	if (!ret)
    {
		printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
    }
    printf("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu

    // test for open second Instance
    ret = UtopiaOpen(MODULE_CMDQ, &pInstance, 0, NULL);
	if (!ret)
    {
		printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
    }
    printf("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
*/
    return 0;
}
