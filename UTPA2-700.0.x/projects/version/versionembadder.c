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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define VERSIONEMBADDER_ERROR(fmt, args...)           printf("[VERSION_EMBADDER ERR][%06d]     " fmt, __LINE__, ## args)
#define VERSIONEMBADDER_DEBUG(fmt, args...)           //printf("[VERSION_EMBADDER DEBUG][%06d]     " fmt, __LINE__, ## args)
#define STRLEN 32
#define CHNAGEINDX 9

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
static char UtopiaVersionStr[]="UTOPIA_VERSION_2.0";
static char UtopiaChangeListStr[]="$Change:";

typedef struct _MS_SW_VERSION_NUM
{  
    char    UtopiaBspVersion[STRLEN];  
    int     UtopiaVerNum; 
}MS_SW_VERSION_NUM;

struct version_node
{
    char* pVer;
    struct version_node *next;
};

//-------------------------------------------------------------------------------------------------
// Local functions
//-------------------------------------------------------------------------------------------------
int push(struct version_node **pHead, char* pVer)
{
    struct version_node *pNode = (struct version_node *)malloc(sizeof(struct version_node));

    if(!pNode)
    {
        VERSIONEMBADDER_ERROR("pNode creation failed\n");
        return 1;
    }

    pNode->pVer = pVer;
    pNode->next = *pHead;
    *pHead = pNode;
    
    return 0;
}

char* pop(struct version_node **pHead)
{
    char* pVer;
    struct version_node *pPreHead;
    
    if((*pHead)==NULL)
    {
        VERSIONEMBADDER_DEBUG("pHead is empty\n");
        return NULL;
    }
    
    pPreHead = *pHead;
    pVer = (*pHead)->pVer;
    *pHead = (*pHead)->next;
    free(pPreHead);

    return pVer;
}

//-------------------------------------------------------------------------------------------------
// Memory dump
//-------------------------------------------------------------------------------------------------
void memdump(char*ptr, size_t size)
{
    long count;

    for(count=0; count < size; count++)
    {
        printf("%x ", *(ptr+count));
        if((count%16) == 15)
        {
           printf("\n");
        }
    }
    
    printf("\n");
}

//-------------------------------------------------------------------------------------------------
// Main function
//-------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    struct version_node *pVesList = NULL;
    FILE* pFileLibR = NULL;
    FILE* pFileLibW = NULL;
    FILE* pFileVerR = NULL;
    size_t LibRSize;
    size_t VerRSize;
    char* pbufLibR;
    char* pbufVerR;
    char* pbuf;
    size_t result;
    long count;
    unsigned int* pLibVer;
    unsigned int changelist;

    pFileLibR = fopen(argv[1], "rb");
    pFileLibW = fopen(argv[2], "wb");
    pFileVerR = fopen(argv[3], "rb");
    
    /* obtain files */
    if(NULL == pFileLibR)
    {
        VERSIONEMBADDER_ERROR("fopen(%s) error!!!\n", argv[1]);
        return 0;
    }
   
    if(NULL == pFileLibW)
    {
        VERSIONEMBADDER_ERROR("fopen(%s) error!!!\n", argv[2]);
        return 0;
    }
   
    if(NULL == pFileVerR)
    {
        VERSIONEMBADDER_ERROR("fopen(%s) error!!!\n", argv[3]);
        return 0;
    }

    /* obtain original library file size */
    fseek (pFileLibR , 0 , SEEK_END);
    LibRSize = ftell(pFileLibR);
    rewind (pFileLibR);
    
    fseek (pFileVerR , 0 , SEEK_END);
    VerRSize = ftell(pFileVerR);
    rewind (pFileVerR);
   
    /* allocate memory to contain the whole file */
    pbufLibR = (char*) malloc ((size_t)LibRSize);
    if(pbufLibR == NULL) 
    {
        VERSIONEMBADDER_ERROR("malloc error!!!\n"); 
        return 0;
    }
    
    pbufVerR = (char*) malloc ((size_t)VerRSize);
    if(pbufVerR == NULL) 
    {
        VERSIONEMBADDER_ERROR("malloc error!!!\n"); 
        return 0;
    }

    /* copy the file into the buffer */
    result = fread (pbufLibR, 1, LibRSize, pFileLibR);
    if (result != LibRSize) 
    {
        fclose(pFileLibR);
        fclose(pFileLibW);
        fclose(pFileVerR);
        free(pbufLibR);
        free(pbufVerR);
        VERSIONEMBADDER_ERROR ("fread(%s) error!!! with result:%ld\n", argv[1], result); 
        return 0;
    }
    
    result = fread (pbufVerR, 1, VerRSize, pFileVerR);
    if (result != VerRSize) 
    {
        fclose(pFileLibR);
        fclose(pFileLibW);
        fclose(pFileVerR);
        free(pbufLibR);
        free(pbufVerR);
        VERSIONEMBADDER_ERROR ("fread(%s) error!!! with result:%ld\n", argv[3], result); 
        return 0;
    }

    VERSIONEMBADDER_DEBUG("###### version info in original utopia lib ######\n");
    
    /* in utopia library, find out where "UtopiaVersionStr" is */
    pbuf = NULL;

    for(count=0; count < LibRSize; count++)
    {
        if (strncmp(pbufLibR+count, UtopiaVersionStr, sizeof(UtopiaVersionStr)) == 0)
        {
            pbuf = pbufLibR+count;
            VERSIONEMBADDER_DEBUG("pbuf: %s\n", pbuf);
            VERSIONEMBADDER_DEBUG("offset_in_file: 0x%lx\n", count);
            push(&pVesList, pbuf + STRLEN);

        }
    }
    
    if(pbuf == NULL)
    {
        VERSIONEMBADDER_ERROR("%s no found!!!\n", UtopiaVersionStr);
        fwrite (pbufLibR , sizeof(char), LibRSize, pFileLibW);
        fclose(pFileLibR);
        fclose(pFileLibW);
        fclose(pFileVerR);
        free(pbufLibR);
        free(pbufVerR);
        return 0;
    }
    
    VERSIONEMBADDER_DEBUG("###### version info in version_info ######\n");
    
    /* in version_info, find change list number*/
    pbuf = NULL;
    for(count = 0; count < VerRSize; count++)
    {
        if (strncmp(pbufVerR + count, UtopiaChangeListStr, sizeof(UtopiaChangeListStr) - 1) == 0)
        {
            pbuf = pbufVerR+count;
            VERSIONEMBADDER_DEBUG("pbuf: %s\n", pbuf);
            VERSIONEMBADDER_DEBUG("offset_in_file: 0x%lx\n", count);
        }
    }
    
    if(!pbuf)
    {
        VERSIONEMBADDER_ERROR("can not find %s in %s\n", UtopiaChangeListStr, argv[3]);
        fclose(pFileLibR);
        fclose(pFileLibW);
        fclose(pFileVerR);
        free(pbufLibR);
        free(pbufVerR);
        return 0;
    }
    
    pbuf = pbuf + CHNAGEINDX;
    changelist = strtol(pbuf, NULL, 10);
    VERSIONEMBADDER_DEBUG("changelist: %d\n", changelist);
    
    /* update UtopiaVerNum in utopia library */
    while((pLibVer = (unsigned int* )pop(&pVesList)) != NULL)
    {
        VERSIONEMBADDER_DEBUG("pLibVer=0x%x\n", pLibVer);
        VERSIONEMBADDER_DEBUG("UtopiaVerNum = 0x%x\n", *pLibVer);
        *pLibVer = changelist;
    }
    
    /* copy the buffer into the file */
    result = fwrite (pbufLibR , sizeof(char), LibRSize, pFileLibW);
    if (result != LibRSize) 
    {
        fclose(pFileLibR);
        fclose(pFileLibW);
        fclose(pFileVerR);
        free(pbufLibR);
        free(pbufVerR);
        VERSIONEMBADDER_ERROR ("fwrite error!!! with result:%ld\n",result); 
        return 0;
    }
    
    /* terminate */
    fclose(pFileLibR);
    fclose(pFileLibW);
    fclose(pFileVerR);
    free(pbufLibR);
    free(pbufVerR);
    
    return 0;
}
