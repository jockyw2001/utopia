#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "mdrv_system_io.h"
#include "mdrv_types.h"
#include "mdrv_system_st.h"


#define SYS_MODULE_KERNAL_NAME   "/dev/system"
#define SYS_PRINT(fmt, args...)  printf("[SYSTEM (user mode)][%05d] " fmt, __LINE__, ## args)

static int SYS_fd = 0;


void MAdp_SYS_Init(void)
{
    if (SYS_fd==0)   //First time open
    {
        SYS_fd = open(SYS_MODULE_KERNAL_NAME, O_RDWR);
        SYS_PRINT("open System Kernal finish\n");
    }
    else
    {
        SYS_PRINT("Fail to open System Kernal Module\n");
    }
    //printf("\nStart T8 demo \n");
    //Init Scaler
    //printf("SYS_fd= %d\n",SYS_fd);
    SYS_PRINT("Start to Init System\n");
}

void MAdp_SPI_Load(U32 start,U32 len,U8 *bin)
{
	IO_SYS_SPI_t spi;
	spi.u32Start=start;
	spi.u32Len=len;
	spi.u8data=bin;	
	
        ioctl(SYS_fd,IOCTL_SYS_SPI_LOAD,&spi);

//	printf("\nstart 0x%08x   , size %d bytes \n",spi.u32Start,spi.u32Len);
//	unsigned int i;
//	printf("\nMAdp_SYS_SPI_LOAD\n");
//	for (i=0;i<0x10;i++)	printf("0x%02x ",*(spi.u8data+i));

}
