//#include "MsTypes.h"
#include <stdio.h>
#include "utopia_dapi.h"
#include "MsOS.h"
#include "bobbi.h"

// resource pool id
enum
{
	BOBBI_RPOOL0,
	BOBBI_RPOOL1,
	BOBBI_RPOOL2,
	BOBBI_RPOOL3,
	BOBBI_RPOOL4,
} eBobbiRPoolID;

// add resource with same size for convenience
#define BOBBI_RESOURCE_SIZE 5
void BOBBIRegisterToUtopia(void)
{
	// create module
    void* pModule = NULL;
	UtopiaModuleCreate(MODULE_BOBBI, 8, &pModule);
	printf("\n%s, %d, bobbi pModule = %p\n", __FUNCTION__, __LINE__, pModule); // bob.fu

	// add to utoipa framework
    UtopiaModuleRegister(pModule);

    // register operations
	UtopiaModuleSetupFunctionPtr(pModule, (FUtopiaOpen)BOBBIOpen
			, (FUtopiaClose)BOBBIClose, (FUtopiaIOctl)BOBBIIoctl);

    void* pResource = NULL;
	//############################
	//## add resource to rpool0 ##
	//############################
	// prepare to register resource
	UtopiaModuleAddResourceStart(pModule, BOBBI_RPOOL0);

    // create and register rsource
    UtopiaResourceCreate("rpool0_resource0", BOBBI_RESOURCE_SIZE, &pResource);
    UtopiaResourceRegister(pModule, pResource, BOBBI_RPOOL0);

    // create and register rsource
    UtopiaResourceCreate("rpool0_resource1", BOBBI_RESOURCE_SIZE, &pResource);
    UtopiaResourceRegister(pModule, pResource, BOBBI_RPOOL0);

    // notify utopia framework to count resource nubmer in specific rpool
	UtopiaModuleAddResourceEnd(pModule, BOBBI_RPOOL0);

	// skip rpool1 for purpose

	//############################
	//## add resource to rpool2 ##
	//############################
	// prepare to register resource
	UtopiaModuleAddResourceStart(pModule, BOBBI_RPOOL2);

    // create and register rsource
    UtopiaResourceCreate("rpool2_resource0", BOBBI_RESOURCE_SIZE, &pResource);
    UtopiaResourceRegister(pModule, pResource, BOBBI_RPOOL2);

    // create and register rsource
    UtopiaResourceCreate("rpool2_resource1", BOBBI_RESOURCE_SIZE, &pResource);
    UtopiaResourceRegister(pModule, pResource, BOBBI_RPOOL2);

    // notify utopia framework to count resource nubmer in specific rpool
	UtopiaModuleAddResourceEnd(pModule, BOBBI_RPOOL2);
}

#define BOBBI_INSTANCE_PRIVATE_SIZE 20
MS_U32 BOBBIOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	void* pModule = NULL;
	printf("module BOBBI is opened\n"); // bob.fu
	printf("create instance with private\n"); // bob.fu
    UtopiaInstanceCreate(BOBBI_INSTANCE_PRIVATE_SIZE, ppInstance);
	UtopiaModuleGetPtr(MODULE_BOBBI, &pModule);
	printf("\n%s, %d, bobbi pModule = %p\n", __FUNCTION__, __LINE__, pModule); // bob.fu
	printf("\n%s, %d, *ppInstance = 0x%X\n", __FUNCTION__, __LINE__, *ppInstance); // bob.fu

	return 0; // FIXME: error code
}

MS_U32 BOBBIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	// get module
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
	enum eBOBBI_IOCTL_CMD eCmd = (enum eBOBBI_IOCTL_CMD)u32Cmd;
	printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu

    MS_U32 u32Ret = 0; // FIXME: error code

    switch(eCmd)
    {
		void* pResource = NULL;
        case BOBBI_IOCTL_CMD1:
			printf("bobbi ioctl cmd1\n");// bob.fu
            if(UtopiaResourceObtain(pModule, BOBBI_RPOOL0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainfail\n");
	            return 0xFFFFFFFF;
            }
			// do something
			sleep(10);
            UtopiaResourceRelease(pResource);
        default:
            break;
    };

	return u32Ret; // FIXME: error code
}

MS_U32 BOBBIClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return 0; // FIXME: error code
}



