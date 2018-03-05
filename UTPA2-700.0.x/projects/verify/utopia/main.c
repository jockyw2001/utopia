//#include <nuttx/config.h>
#include <stdio.h>
//#include "MsTypes.h"
//#include "stdlib.h"
#include "utopia.h" // for utopia open/ioctl/close
#include "utopia_driver_id.h" // for module id
#include "bobbi.h" // for ioctl command option
#include "MsOS.h" // for MsOS_ObtainMutex test
//#include "drvMMIO.h"
//#include <string.h>
//#include <unistd.h>
//#include "drvMIU.h"

void __aeabi_unwind_cpp_pr1(void) {}
void __aeabi_unwind_cpp_pr0(void) {}


int main(int argc, char *argv[])
{

	/*MS_U32 u32MutexID = MsOS_CreateMutex(E_MSOS_FIFO, "test", MSOS_PROCESS_SHARED);
	printf("\n%s, %d, 0x%X\n", __FUNCTION__, __LINE__, u32MutexID); // bob.fu
	MsOS_ObtainMutex(u32MutexID, MSOS_WAIT_FOREVER);
	sleep(7);
	printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
	MsOS_ReleaseMutex(u32MutexID);
	return 0;*/
	// bdma would need this
    //MDrv_MMIO_Init(); // FIXME: should this be called in UtopiaInit()?
	unsigned int ret = 0;
    UtopiaInit();

	void* pInstance = NULL;
	void* pArgs = NULL;
	printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
	ret = UtopiaOpen(MODULE_BOBBI, &pInstance, 0, NULL);
	if (ret) // if error
		printf("something wrong in UtopiaOpen, error = %d\n", ret); // bob.fu
	UtopiaIoctl(pInstance, BOBBI_IOCTL_CMD1, pArgs);
	printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
	

	return 0;
}
