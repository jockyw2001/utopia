//#include <nuttx/config.h>
#include <stdio.h>

#include "MsTypes.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * user_start
 ****************************************************************************/
#include "stdlib.h"
//#include "utopia_core.h"
#include "utopia.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "drvMMIO.h"
#include <stdio.h>
#include "drvBDMA_priv.h"
#include <string.h>
#include <unistd.h>
#include "drvMIU.h"




// enable or diable the BDMA test app
#define BDMA_UTOPIA_TEST

#ifdef BDMA_UTOPIA_TEST
#define BDMA_MEMCOPY_TESTCOUNT      (10)
#endif


void __aeabi_unwind_cpp_pr1(void)
{
}

void __aeabi_unwind_cpp_pr0(void)
{
}

void simulateMsosShmInit(void)
{
	MsOS_SHM_Init();
}

/*__attribute__((weak)) void AESDMARegisterToUtopia(void)
{	
	printf("\nweak\n");
}
__attribute__((weak)) void DSCMBRegisterToUtopia(void)
{	
	printf("\nweak\n");
}*/

#ifdef BDMA_UTOPIA_TEST
MS_U32 BDMA_MemChkSum(MS_U32 Addr, MS_U32 len)
{
    MS_U8 *s = (MS_U8 *)MsOS_PA2KSEG1(Addr);
    MS_U32 idx, val = 0x98765432;

    for (idx = 0; idx < len; idx++)
        val ^= s[idx];

    return val;
}
#endif //

int main(int argc, char *argv[])
{
	// simple test
	#if 0
	AESDMARegisterToUtopia();
	return 0;
	#endif



#if 1
    MS_U32 bEnable;
    MS_U32 virtaddr;
    MS_U32* pVirtaddr;
    bEnable = 1;
	void* pAttribute = NULL;
	int i;
	

    MS_U32 bSegment;
    bSegment = 1;

    // Below is BDMA variable declare part
#ifdef BDMA_UTOPIA_TEST
    void* pInstantBdma;
    PBDMA_MEMCOPY_PARAM pMemCpyParam;
    PBDMA_SEARCH_PARAM pSearchParam;
    MS_U32 virtual_addr;
    MS_U8* pbyte;
    MS_U32 checksum=0;
    MS_U8 freecounter=0;
    MS_U32 processid=0;
    MS_U32 memcopycount=0;
    MS_U32 u32SearchAddr;
#endif


    MDrv_MMIO_Init();
    if(TRUE != MsOS_MPool_Init())
    {
        printf("MsOS_MPool_Init fail\n");
    }
    MDrv_MIU_SetIOMapBase();

    MsOS_MPool_SetDbgLevel(1);
    MsOS_MPool_Mapping(0, 0x10000000, 0x10000, 2);
    MsOS_MPool_Mapping(0, 0x10000000, 0x10000, 0);
    UtopiaInit();

    virtaddr = MsOS_MPool_PA2KSEG1(0x10000000);

    printf("virtaddr = %x\n", virtaddr);
    pVirtaddr = (MS_U32 *)virtaddr;
    #if 1
    *pVirtaddr = 0x01020304;
    *(pVirtaddr+1) = 0x05060708;
    *(pVirtaddr+2) = 0x090A0B0C;
    *(pVirtaddr+3) = 0x0D0E0F00;
    #else
    *pVirtaddr = 0x0BD7B8E4;
    *(pVirtaddr+1) = 0x284939A7;
    *(pVirtaddr+2) = 0x8C54998D;
    *(pVirtaddr+3) = 0x1E2F3F57;
    #endif
    MsOS_Dcache_Flush(virtaddr, 4);

    // below is the bdma test code
#ifdef BDMA_UTOPIA_TEST
    processid = (MS_U32)getpid();
	void** ppModule;
	printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
    if(UtopiaOpen(MODULE_BDMA, &pInstantBdma, 0, pAttribute) < 0)

    {
        printf("Open BDMA fail\n");
		return 0;
    }
    // BDMA search pattern test code
	printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
    pSearchParam = (PBDMA_SEARCH_PARAM)utopia_malloc(sizeof(BDMA_SEARCH_PARAM));
    pSearchParam->u32Addr = 0x10000000;
    pSearchParam->u32Len = 0x100;
    pSearchParam->u32Pattern = 0xABABABAB;
    pSearchParam->u32ExcluBit = 0;
    pSearchParam->eDev = E_BDMA_SRCDEV_MIU0;
    virtual_addr=MsOS_PA2KSEG1(pSearchParam->u32Addr);
    pbyte = (MS_U8*)virtual_addr;
    pbyte[16] = 0xAB;
    pbyte[17] = 0xAB;
    pbyte[18] = 0xAB;
    pbyte[19] = 0xAB;
    u32SearchAddr= UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_Search,(MS_U32*)pSearchParam);
    printf("Search Addr is: %lx\n",u32SearchAddr);
    utopia_free(pSearchParam);

    // BDMA mem copy test code
    pMemCpyParam = (PBDMA_MEMCOPY_PARAM)utopia_malloc(sizeof(BDMA_MEMCOPY_PARAM));
    pMemCpyParam->u32SrcAddr = 0x10000000+(processid%10)*0x100;
    pMemCpyParam->u32DstAddr = 0x10001000+(processid%10)*0x100;
    pMemCpyParam->u32Len = 0x80;
    virtual_addr=MsOS_PA2KSEG1(pMemCpyParam->u32SrcAddr);
    pbyte = (MS_U8*)virtual_addr;
    while (1)
    {
        for (i=0;i<pMemCpyParam->u32Len;i++)
        {
            pbyte[i]=freecounter++;
        }
        checksum = BDMA_MemChkSum(pMemCpyParam->u32SrcAddr,pMemCpyParam->u32Len);
        if(UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_MemCopy,(MS_U32*)pMemCpyParam) < 0)
        {
            printf("Ioctl BDMA Start fail\n");
            utopia_free(pMemCpyParam);
    		return 0;
        }
        if (checksum == BDMA_MemChkSum(pMemCpyParam->u32DstAddr,pMemCpyParam->u32Len))
            printf("BDMA Copy OK by Process:%d !!\n,",processid);
        else
            printf("BDMA Copy FAIL by Process:%d !!\n",processid);
        //MsOS_DelayTask(50);
        memcopycount++;
        if (memcopycount >= BDMA_MEMCOPY_TESTCOUNT)
            break;
    }
    utopia_free(pMemCpyParam);

    if(UtopiaClose(pInstantBdma) < 0)
    {
        printf("close BDMA fail\n");
		return 0;
    }
    // bdma test code end here
#endif

	printf("\n%s, %d, utopia2 verifation end\n", __FUNCTION__, __LINE__); 
	return 0;
#endif
}
