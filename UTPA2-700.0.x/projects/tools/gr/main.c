////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    main.c
/// @brief  main entry point of the application program
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include <fcntl.h>
#include "mdrv_types.h"

int SPILoad(void)
{
	FILE *fp;
	int size;
	unsigned int start;
	unsigned char *pp=0;

	char fn[256]; 
	printf("--- Start to load file from SPI or DRAM ---\n");
	printf("SPI base: 0xBFC00000, DRAM base: 0x80000000\n");
	printf("file name: ");	
	scanf("%s",&fn);
	printf("start address: 0x ");	
	scanf("%x",&start);
	printf("size (bytes): ");
	scanf("%d",&size);
       
	//pp = malloc( DEFAULT_SIZE ) ;
	pp = malloc( size+4 ) ;
	if( 0==pp ){
		printf( "alloc fail\n" ) ;
		return 0 ;
	}
 	
	//MAdp_getSPI(pp);
	MAdp_SPI_Load(start,size,pp);
	printf( "\nfirst: 0x%02X-0x%02X-0x%02X-0x%02X\n", pp[0],pp[1],pp[2],pp[3] );
	fp = fopen( fn, "w+" ) ;
	fwrite( pp, 1, size, fp ) ;
	fclose(fp) ;

	free(pp) ;

}


extern void MAdp_SYS_Init(void);
int main(int argc, char* argv[])
{
    //----------------------------------------------------------------------------------------------
    //  System Initialization
    //----------------------------------------------------------------------------------------------
    MAdp_SYS_Init(); //call adaptation function 

    SPILoad();   //gr 
    return 0;
}



