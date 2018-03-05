#ifndef _BOBBI_H_
#define _BOBBI_H_

enum eBOBBI_IOCTL_CMD
{
	BOBBI_IOCTL_CMD1,
	BOBBI_IOCTL_CMD2,
	BOBBI_IOCTL_CMD3,
};

MS_U32 BOBBIOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 BOBBIClose(void* pInstance);
MS_U32 BOBBIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#endif
