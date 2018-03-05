//Compile parameter=================================================================================
//1. unit test:
#define SPT_UNIT_TEST 0
//2. compile options:
#define SPT_FORCE_MALLOC            0
//3. debug:
#define FlagPrint(FLAG)               if(FLAG)db_printf
#define SPT_DB_SEQMM_MAX_USAGE      0
#define SPT_DB_MALLOC_FREE          0
#define SPT_DB_MALLOC_FREE_DETAIL   0
#define SPT_DB_MEMINFO 0
#define SPT_DB_DETAIL               0
//==================================================================================================
#if SPT_UNIT_TEST == 0
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/seq_file.h>
#include <linux/compat.h>

#define MALLOC(size)                    kmalloc(size,GFP_KERNEL)
#define FREE(ptr)                      kfree(ptr)
#define CPY_FROM_USER                   copy_from_user
#define CPY_to_USER                     copy_to_user
//#define db_printf(argc,...)
#define db_printf printk

typedef unsigned long (*fcpy_t)(void *to, const void *from, unsigned long n);

#include "utopia.h"
#include "utopia_adp.h"

#else//SPT_UNIT_TEST==0
#include "ut_spt_inc.h"
#endif//SPT_UNIT_TEST==0
#define SPT_MAX_SIZE 100

//==================================internal tools===============================================
#define SRC_MODE_COPY       0x01
#define SRC_MODE_CPY_FROM_USR       0x02
#define SRC_MODE_MALLOC     0x10

static void SkipCpy(void *to, const void *from,const UADP_STRUCT_POINTER_TABLE *pTable,fcpy_t fcpy)
{
    int i;
    char* pcTo   =(char*)to;
    char* pcFrom =(char*)from;
    unsigned int uiStart=0, uiEnd=0, uiLen=0, uiSelfSize=0;
    FlagPrint(SPT_DB_DETAIL)("to=%p, from=%p\n",pcTo, pcFrom);
    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:
                    uiSelfSize = pTable[i].size_or_next;
                    break;
            case UADP_SPT_POINTER_TO_NEXT:
                    uiEnd = pTable[i].offset - 1;
                    uiLen = uiEnd+1-uiStart;     //include uiEnd last byte
                    FlagPrint(SPT_DB_DETAIL)("UADP_SPT_POINTER_TO_NEXT uiStart=%u,uiEnd=%u,uiLen=%u,off=%u,ptr_size=%u\n",uiStart,uiEnd,uiLen,pTable[i].offset,sizeof(pcTo));
                    fcpy(pcTo+uiStart,pcFrom+uiStart,uiLen);
                    uiStart = pTable[i].offset+sizeof(pcTo);
                    break;
            case UADP_SPT_END:
                    uiEnd = uiSelfSize - 1;
                    uiLen = uiEnd+1-uiStart;     //include uiEnd last byte
                    FlagPrint(SPT_DB_DETAIL)("UADP_SPT_END uiStart=%u,uiEnd=%u,uiLen=%u,size=%u\n",uiStart,uiEnd,uiLen,uiSelfSize);
                    fcpy(pcTo+uiStart,pcFrom+uiStart,uiLen);
                    return;
            case UADP_SDT_AT:
            case UADP_SDT_ES:
					break;
        }
    }
}

#ifdef CONFIG_COMPAT

#if 0
static void CmpSkipCpy(void *to, const void *from,const UADP_STRUCT_POINTER_TABLE *pTable,fcpy_t fcpy)
{
    int i;
    char* pcTo   =(char*)compat_ptr(to);
    char* pcFrom =(char*)from;
    MS_PHY pKStart=0, pKEnd=0, pKLen=0, pKSelfSize=0;
    MS_PHY pUStart=0, pUEnd=0, pULen=0, pUSelfSize=0;
    FlagPrint(SPT_DB_DETAIL)("[%s][%d]to=%p, from=%p\n",__FUNCTION__,__LINE__, pcTo, pcFrom);
    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:            
                    pKSelfSize = pTable[i].size_or_next;
                    break;
            case UADP_SDT_P2N:
                if(pTable[i].offset!=pKStart /* 1. size =0-1 cause error; 2. Over size*/)
                {
                    pKEnd = pTable[i].offset-1;
                    pKLen = pKEnd+1-pKStart;     //include uiEnd last byte                    
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_P2N UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    fcpy(pcTo+pUStart,pcFrom+pKStart,pKLen);					
                }
                    pKStart = pTable[i].offset+sizeof(void*);                    
                    pUStart = pTable[i].offset-(i-1)*4+4;

                    break;
            case UADP_SDT_AT:                
                if(pTable[i].offset!=pKStart /* 1. size =0-1 cause error; 2. Over size*/)
                {
                    pKEnd = pTable[i].offset-1;
                    pKLen = pKEnd+1-pKStart;     //include uiEnd last byte                             
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_AT UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    fcpy(pcTo+pUStart,pcFrom+pKStart,pKLen);
                }
                    pKStart = pTable[i].offset;
                    pUStart = pTable[i].offset-(i-1)*4;
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_AT UAdrr=%p KAdrr=%p\n",__FUNCTION__,__LINE__,(pcTo+pUStart), (pcFrom+pKStart));
                    fcpy(pcTo+pUStart,pcFrom+pKStart, 4);

                    pKStart = pTable[i].offset+sizeof(void*);
                    pUStart += 4;
                    break;
            case UADP_SDT_ES:
                {
                    int k=0;
                    unsigned long StructureOffset=0;
                    UADP_STRUCT_POINTER_TABLE *pTable_ES=NULL;
                    char* to =(pcTo+pUStart);
                    char* from =(pcFrom+pKStart);

                    if( pTable[i].offset!=pKStart /* 1. size =0-1 cause error; 2. Over size*/)
                    {
                        pKEnd = pTable[i].offset-1;
                        pKLen = pKEnd +1-pKStart;     //include uiEnd last byte                    
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                        fcpy(pcTo+pKStart,pcFrom+pUStart,pKLen);
        }
                    
                    pTable_ES = (UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next); 
                    memcpy(&pTable[i].EmbededStr, &pTable_ES, sizeof(void*));       //Record
                    memcpy(&pTable[i].ESfrom, &from, sizeof(void*));                //Record
                    memcpy(&pTable[i].ESto, &to, sizeof(void*));                    //Record
                    StructureOffset =pTable_ES[0].size_or_next;
                    for(k=0;k<SPT_MAX_SIZE;k++)
                    {                    
                        if(pTable_ES[k].type ==UADP_SDT_END)
                        break;
                    }               
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES pTable_ES=%p from=%p to=%p  break=%p StructureOffset=0x%x\n",__FUNCTION__,__LINE__,pTable_ES,(pcFrom+pUStart),(pcTo+pKStart),k,StructureOffset);
                    pKStart += StructureOffset;
                    pUStart += StructureOffset-(k-1)*4;
                    break;
                }
                    
            case UADP_SPT_END:            
                    pKEnd = pKSelfSize - 1;
                    pKLen = pKEnd+1-pKStart;     //include uiEnd last byte
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SPT_END UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    fcpy(pcTo+pUStart,pcFrom+pKStart,pKLen);
                    return;
        }
    }
}


static void CmpCpy(void *to, const void *from,const UADP_STRUCT_POINTER_TABLE *pTable,fcpy_t fcpy)
{
    int i;
    char* pcTo   =(char*)to;
    char* pcFrom =(char*)compat_ptr(from);
    MS_PHY pKStart=0, pKEnd=0, pKLen=0, pKSelfSize=0;
    MS_PHY pUStart=0, pUEnd=0, pULen=0, pUSelfSize=0;
    
    MS_U64* u64p=NULL;
    FlagPrint(SPT_DB_DETAIL)("[%s][%d]to=%p, from=%p\n",__FUNCTION__,__LINE__,pcTo, pcFrom);

    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:            
                    pKSelfSize = pTable[i].size_or_next;
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]pKSelfSize=%p\n",__FUNCTION__,__LINE__, pKSelfSize);
                    break;
            case UADP_SDT_P2N:                        
            case UADP_SDT_AT:
                if( pTable[i].offset!=pKStart /* 1. size =0-1 cause error; 2. Over size*/)
                {
                    pKEnd = pTable[i].offset-1;
                    pKLen = pKEnd +1-pKStart;     //include uiEnd last byte                    
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_P2N UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    fcpy(pcTo+pKStart,pcFrom+pUStart,pKLen);
                }
                    u64p = (pcTo+ pTable[i].offset);
                    pUStart = pTable[i].offset-(i-1)*4;   					
                    fcpy(u64p,pcFrom+pUStart,4);
                    *u64p =(0xFFFFFFFF)& (*u64p);
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_AT UAdrr=%p KAdrr=%p   NextUaddr=%p\n",__FUNCTION__,__LINE__,(pcTo+pKStart), (pcFrom+pUStart), *u64p);
                        
                    pKStart = pTable[i].offset+sizeof(void*);                    
                    pUStart += 4;   
                    break;
            case UADP_SDT_ES:
                {
                    int k=0;
                    unsigned long StructureOffset=0;
                    UADP_STRUCT_POINTER_TABLE *pTable_ES=NULL;
                    char* to =(pcTo+pKStart);
                    char* from =(pcFrom+pUStart);
                    
                    if( pTable[i].offset!=pKStart /* 1. size =0-1 cause error; 2. Over size*/)
                    {
                        pKEnd = pTable[i].offset-1;
                        pKLen = pKEnd +1-pKStart;     //include uiEnd last byte                    
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                        fcpy(pcTo+pKStart,pcFrom+pUStart,pKLen);
                    }
                    
                    pTable_ES = (UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next); 
                    memcpy(&pTable[i].EmbededStr, &pTable_ES, sizeof(void*));       //Record
                    memcpy(&pTable[i].ESto, &to, sizeof(void*));                    //Record                    
                    memcpy(&pTable[i].ESfrom, &from, sizeof(void*));                //Record
                    StructureOffset =pTable_ES[0].size_or_next;
                    for(k=0;k<SPT_MAX_SIZE;k++)
                    {                    
                        if(pTable_ES[k].type ==UADP_SDT_END)
                            break;
                    }               
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES pTable_ES=%p from=%p to=%p  break=%p StructureOffset=0x%x\n",__FUNCTION__,__LINE__,pTable_ES,(pcFrom+pUStart),(pcTo+pKStart),k,StructureOffset);
                    pKStart += StructureOffset;
                    pUStart += StructureOffset-(k-1)*4;
                    break;
                }
            case UADP_SPT_END:                    
                    pKEnd = pKSelfSize-1;
                    pKLen = pKEnd+1-pKStart;     //include uiEnd last byte
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SPT_END UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    fcpy(pcTo+pKStart,pcFrom+pUStart,pKLen);
                    return;
        }
    }
}
#endif

static void CPY(void *to, const void *from,unsigned int Start,unsigned int Offset, MS_PHY *Len ,fcpy_t fcpy, int iMode)
{
    char* pcTo=NULL;
    char* pcFrom=NULL;
    MS_PHY KLen=0,KEnd=0;
    if(iMode&SRC_MODE_CPY_FROM_USR)
    {
        pcTo   =(char*)to;
        pcFrom =(char*)compat_ptr(from);
    }
    else
    {
        pcTo   =(char*)compat_ptr(to);
        pcFrom =(char*)from;
    }

    if(Offset!=Start /* 1. size =0-1 cause error; 2. Over size*/)
    {
        KEnd = Offset-1;
        KLen = KEnd+1-Start;        
        fcpy(pcTo,pcFrom,KLen);
    }
    else
    {
        KLen=0;
    }

    *Len = KLen;
    return;
}

static int CmpStructRecursiveCpy(void** ppDest, void* pSrc,const UADP_STRUCT_POINTER_TABLE *pTable,fcpy_t fcpy, int iMode, bool isES, unsigned int* TurnUsrAddrOffset)
{
    int i=0,ret=0;
    char* pcBuffer=NULL;
	void* pCmptr=NULL;
    FlagPrint(SPT_DB_DETAIL)("ppDest=%p,pTable=%p,pSrc=%p isES=%d  TurnUsrAddrOffset=%p\n",ppDest,pTable,pSrc,isES, *TurnUsrAddrOffset);

    char* pcTo   =NULL;
    char* pcFrom =NULL;    
    MS_U64* u64p=NULL;        
    unsigned int ReferenceOffset=0;
    MS_PHY pKStart=0, pKEnd=0, pKLen=0, pKSelfSize=0;
    MS_PHY pUStart=0, pUEnd=0, pULen=0, pUSelfSize=0;

    if(pTable==NULL)
    {
        db_printf("TABLE is NULL\n");
        return ret;
    }

    *TurnUsrAddrOffset=0;
    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        FlagPrint(SPT_DB_DETAIL)("[%s][%d]i=%d type=%d isES=%d \n",__FUNCTION__,__LINE__,i,pTable[i].type,isES);              
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:
            {                
                    if(TRUE!=isES)//ES don't need malloc
                    {     
                        if(iMode&SRC_MODE_MALLOC && pTable[i].size_or_next!=0)
                        {
                            pcBuffer = MALLOC(pTable[i].size_or_next);                     
                            *ppDest = pcBuffer;                        
                            FlagPrint(SPT_DB_MALLOC_FREE)("[%s][%d]<malloc %p, size=%u>\n",__FUNCTION__,__LINE__,*ppDest,pTable[i].size_or_next);
    }
}
                    if(iMode&SRC_MODE_CPY_FROM_USR)
                    {
                        pcFrom =(char*)compat_ptr(pSrc);
                        pcTo   =(char*)(*ppDest);
                    }
                    else
                    {
                        pcFrom =(char*)(pSrc);                    
                        pcTo   =(char*)compat_ptr(*ppDest);
                    }                
                    pKSelfSize = pTable[i].size_or_next;
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d] UADP_SPT_SELF_SIZE pTable[%d].size_or_next=%d \n",__FUNCTION__,__LINE__, i, pKSelfSize);                    
            break;
            case UADP_SDT_P2N:
                        
                    if(iMode&SRC_MODE_CPY_FROM_USR)
                    {
                        //1. Copy before SDT_elements
                        CPY(pcTo+pKStart,pcFrom+pUStart,pKStart,pTable[i].offset,&pKLen,fcpy,iMode);                    
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_P2N pcFrom=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcFrom+pUStart),pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));

                        //2. Copy SDT_elements
                        u64p = (pcTo+ pTable[i].offset);
                        pUStart += pKLen;
                        fcpy(u64p,pcFrom+pUStart,4);
                        *u64p =(0xFFFFFFFF)& (*u64p);
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_P2N UAdrr=%p KAdrr=%p   NextUaddr=%p\n",__FUNCTION__,__LINE__, (pcFrom+pUStart),u64p, *u64p);

                        //3. P2N CPY
                        pCmptr =  compat_ptr(*((void**)(pcFrom+pUStart)));// compat_ptr(*((void**)(pcFrom+pTable[i].offset-(i-1)*4)));
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_P2N pTable[%d].offset=%p pCmptr=%p\n",__FUNCTION__,__LINE__,i,(void*)pTable[i].offset,pCmptr);
                        if(pCmptr==NULL)
                        {                        
                            printk("Next pointer is NULL pointer, ignore it.\n");
                            //Case: maybe pCmptr==NULL, but need to create buffer for Free
                            CmpStructRecursiveCpy((void**)(pcTo+pTable[i].offset),pCmptr,(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),CPY_FROM_USER,SRC_MODE_MALLOC|SRC_MODE_CPY_FROM_USR, FALSE, (unsigned int*)&ReferenceOffset);              
                        }
                        else
                        {
                            ret += CmpStructRecursiveCpy((void**)(pcTo+pTable[i].offset),pCmptr ,(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),fcpy, iMode, FALSE, (unsigned int*)&ReferenceOffset);  
                        }
                    }
                    else
                    {                    
                        //1. Copy before SDT_elements
                        CPY(pcTo+pUStart,pcFrom+pKStart,pKStart,pTable[i].offset,&pKLen,fcpy,iMode);                       
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_P2N pcTo=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcTo+pUStart), pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));

                        //2. Copy SDT_elements
                        pUStart += pKLen;//pTable[i].offset-(i-1)*4;                   
                        pCmptr = compat_ptr(*(void**)(pcTo+pUStart));//compat_ptr((void**)(pcTo+pTable[i].offset-(i-1)*4));
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_P2N pTable[%d].offset=%p  pCmptr=%p\n",__FUNCTION__,__LINE__,i,(void*)pTable[i].offset,pCmptr);
                        if(pCmptr==NULL)
                        {
                            printk("Next pointer is NULL pointer, ignore it.\n");
                        }
                        else
                        {
                            ret += CmpStructRecursiveCpy(&pCmptr,*((void**)(pcFrom+pTable[i].offset)),(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),fcpy, iMode, FALSE,(unsigned int*)&ReferenceOffset);
                        }
                   }
     
                    //4. Calculate new start
                    pKStart = pTable[i].offset+sizeof(void*);         
                    pUStart +=4; 

                    //5. if ES, record for return                                                                
                    if(TRUE==isES)
                    {                        
                        *TurnUsrAddrOffset += 4;   
                    }        
                break;
                case UADP_SDT_AT:
                    if (!(iMode&SRC_MODE_COPY))
                        break;
                    
                    if(iMode&SRC_MODE_CPY_FROM_USR)
                    {
                        //1. Copy before SDT_elements
                        CPY(pcTo+pKStart,pcFrom+pUStart,pKStart,pTable[i].offset,&pKLen,fcpy,iMode);                    
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_AT pcFrom=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcFrom+pUStart),pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    
                        //2. Copy SDT_elements
                        u64p = (pcTo+ pTable[i].offset);
                        pUStart += pKLen;//pTable[i].offset-(i-1)*4;                    
                        fcpy(u64p,pcFrom+pUStart,4);
                        *u64p =(0xFFFFFFFF)& (*u64p);
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_AT UAdrr=%p KAdrr=%p   NextUaddr=%p\n",__FUNCTION__,__LINE__,(pcFrom+pUStart),u64p,*u64p);
                    }
                    else
                    {                
                        //1. Copy before SDT_elements
                        CPY(pcTo+pUStart,pcFrom+pKStart,pKStart,pTable[i].offset,&pKLen,fcpy,iMode);                       
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_AT pcTo=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcTo+pUStart), pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    
                        //2. Copy SDT_elements
                        pKStart = pTable[i].offset;
                        pUStart += pKLen;//pTable[i].offset-(i-1)*4;
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_AT UAdrr=%p KAdrr=%p\n",__FUNCTION__,__LINE__,(pcTo+pUStart), (pcFrom+pKStart));
                        fcpy(pcTo+pUStart,pcFrom+pKStart, 4);
                    }
                    
                    //3. Calculate new start                                 
                    pKStart = pTable[i].offset+sizeof(void*);                    
                    pUStart += 4;

                    //4. if ES, record for return                                                                  
                    if(TRUE==isES)
                    {                        
                        *TurnUsrAddrOffset += 4;   
                    }        
                break;
                case UADP_SDT_ES:
                {
                    if (!(iMode&SRC_MODE_COPY))
                        break;
                    
                    unsigned long StructureOffset=0;
                    UADP_STRUCT_POINTER_TABLE *pTable_ES=NULL;
                    char* pVal=NULL;
                    if(iMode&SRC_MODE_CPY_FROM_USR)
                    {
                        //1. Copy before SDT_elements
                        CPY(pcTo+pKStart,pcFrom+pUStart,pKStart,pTable[i].offset,&pKLen,fcpy,iMode);                    
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES pcFrom=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcFrom+pUStart),pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    
                        //2. Copy SDT_elements
                        pKStart += pKLen;
                        pUStart += pKLen;//pTable[i].offset-(i-1)*4;                    
                        pCmptr = compat_ptr(pcFrom+pUStart);//compat_ptr(pcFrom+pTable[i].offset-(i-1)*4);
                        pVal = pcTo+pTable[i].offset;
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES pCmptr=%p \n",__FUNCTION__,__LINE__,pCmptr);
                        ret += CmpStructRecursiveCpy((void**)&pVal,(void*)pCmptr ,(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),fcpy, iMode, TRUE, (unsigned int*)&ReferenceOffset);
                    }
                    else
                    {                
                        //1. Copy before SDT_elements
                        CPY(pcTo+pUStart,pcFrom+pKStart,pKStart,pTable[i].offset,&pKLen,fcpy,iMode);                       
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES pcTo=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcTo+pUStart), pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));

                        //2. Copy SDT_elements                        
                        pKStart += pKLen;
                        pUStart += pKLen;
                        pCmptr = compat_ptr(pcTo+pUStart);//compat_ptr(pcTo+pTable[i].offset-(i-1)*4);
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES pCmptr=%p \n",__FUNCTION__,__LINE__,pCmptr);
                        ret += CmpStructRecursiveCpy((void**)&pCmptr,(void*)(pcFrom+pTable[i].offset),(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),fcpy, iMode, TRUE, (unsigned int*)&ReferenceOffset);
                    }
 
                    //3. Calculate ES offset
                    pTable_ES = (UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next); 
                    StructureOffset =pTable_ES[0].size_or_next;              
                    FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SDT_ES pTable_ES=%p from=%p to=%p  StructureOffset=%d\n",__FUNCTION__,__LINE__,pTable_ES,(pcFrom+pUStart),(pcTo+pKStart),StructureOffset);
                    pKStart += StructureOffset;
                    pUStart = pUStart+StructureOffset-ReferenceOffset;
                    
                    //4. if ES, record for return                                                                      
                    if(TRUE==isES)
                    {                        
                        *TurnUsrAddrOffset += ReferenceOffset;   
                    }        
                }
                break;
                case UADP_SPT_END:     
                    if (!(iMode&SRC_MODE_COPY))
                        return ret;
                    
                    //Copy elements
                    if(iMode&SRC_MODE_CPY_FROM_USR)
                    {                        
                        CPY(pcTo+pKStart,pcFrom+pUStart,pKStart,pKSelfSize,&pKLen,fcpy,iMode);                    
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SPT_END pcFrom=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcFrom+pUStart),pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));                    
                    }
                    else
                    {
                        CPY(pcTo+pUStart,pcFrom+pKStart,pKStart,pKSelfSize,&pKLen,fcpy,iMode);                       
                        FlagPrint(SPT_DB_DETAIL)("[%s][%d]UADP_SPT_END pcTo=%p UStart=%p KStart=%p,KEnd=%p,KLen=%p,off=%p,ptr_size=%p\n",__FUNCTION__,__LINE__,(pcTo+pUStart), pUStart, pKStart,pKEnd,pKLen,pTable[i].offset,sizeof(pcTo));
                    }
                    return ret;
            }
        }
    }

    db_printf("SPT table size over flow > %d\n",SPT_MAX_SIZE);
    ret += 1;
    return ret;
}
#endif

static int StructRecursiveCpy(void** ppDest, void* pSrc,const UADP_STRUCT_POINTER_TABLE *pTable,fcpy_t fcpy, int iMode)
{
    int ret=0;
    int i;
    char* pcBuffer=NULL;
    char* pcSrc=(char*)pSrc;

    FlagPrint(SPT_DB_DETAIL)("ppDest=%p,pTable=%p,pSrc=%p,",ppDest,pTable,pSrc);
    
    if(pTable==NULL)
    {
        db_printf("TABLE is NULL\n");
        return ret;
    }

    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:
                    if(iMode&SRC_MODE_MALLOC)
                    {
                        pcBuffer = MALLOC(pTable[i].size_or_next);
                        *ppDest = pcBuffer;
                        FlagPrint(SPT_DB_MALLOC_FREE)("<malloc %p, size=%u>\n",*ppDest,pTable[i].size_or_next);
						if(iMode&SRC_MODE_COPY)fcpy(pcBuffer,pSrc,pTable[i].size_or_next);
                    }
                    else
                    {
                        pcBuffer = *ppDest;
                        FlagPrint(SPT_DB_MALLOC_FREE)("<use buf %p>\n",*ppDest);
						if(iMode&SRC_MODE_COPY)SkipCpy(pcBuffer,pSrc,pTable,fcpy);
                    }
                    FlagPrint(SPT_DB_DETAIL)("pTable[%d].size_or_next=%d\n",i,pTable[i].size_or_next);                    
                    break;
            case UADP_SPT_POINTER_TO_NEXT:
                    FlagPrint(SPT_DB_DETAIL)("pTable[%d].offset=%p\n",i,(void*)pTable[i].offset);
                    FlagPrint(SPT_DB_DETAIL)("pcSrc+pTable[i].offset=%p\n",pcSrc+pTable[i].offset);
                    if((*((void**)(pcSrc+pTable[i].offset)))==NULL)
                    {
                        FlagPrint(SPT_DB_DETAIL)("Next pointer is NULL pointer, ignore it.\n");
                        break;
                    }
                    ret += StructRecursiveCpy((void**)(pcBuffer+pTable[i].offset),*((void**)(pcSrc+pTable[i].offset)),(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next),fcpy, iMode);
                    break;
            case UADP_SDT_AT:
            case UADP_SDT_ES:
					break;
            case UADP_SPT_END:
                    return ret;
        }
    }

    db_printf("SPT table size over flow > %d\n",SPT_MAX_SIZE);
    ret += 1;
    return ret;
}

static int StructRecursiveFree(void** ppDest,const UADP_STRUCT_POINTER_TABLE *pTable)
{
    int ret=0;
    int i;

    FlagPrint(SPT_DB_DETAIL)("ppDest=%p,pTable=%p,\n",ppDest,pTable);
    if(pTable==NULL)return ret;

    for(i=0;i<SPT_MAX_SIZE;i++)
    {
        switch(pTable[i].type)
        {
            case UADP_SPT_SELF_SIZE:
            case UADP_SDT_AT:                
            case UADP_SDT_ES:
                    break;
            case UADP_SPT_POINTER_TO_NEXT:
                    FlagPrint(SPT_DB_DETAIL)("pTable[%d].offset=%p\n",i,(void*)pTable[i].offset);
                    ret += StructRecursiveFree((void**)(*ppDest+pTable[i].offset),(UADP_STRUCT_POINTER_TABLE *)(pTable[i].size_or_next));
                    break;
            case UADP_SPT_END:
                    FlagPrint(SPT_DB_DETAIL)("<free *ppDest=%p>\n",*ppDest);
                    FREE(*ppDest);
                    *ppDest=NULL;
                    return ret;
        }
    }

    db_printf("SPT table size over flow > %d\n",SPT_MAX_SIZE);
    ret += 1;
    return ret;
}

#if SPT_DB_MEMINFO
static void ShowMemInfo(char* str)
{
    char buff[100];
    db_printf("%s\n",str);
    sprintf(buff,"pmap -x %d | grep total",getpid());
    system(buff);
    db_printf("\n");
}
#else//SPT_DB_MEMINFO
#define ShowMemInfo(a)
#endif//SPT_DB_MEMINFO

#if SPT_DB_SEQMM_MAX_USAGE
static void OverFlowAssert(void* pInstance,unsigned int u32Cmd,void* pMemorySpace)
{
    
    unsigned int u32ModuleID;
    if(SequenceMMCheckFlag(pMemorySpace,SEQMM_FLAG_OVERFLOW))
    {
        UtopiaInstanceGetModuleID(pInstance,&u32ModuleID);
        db_printf("UADPBypassIoctl overflow: MODULE=%x, CMD=%x, usage/space=%u/%u\n",u32ModuleID,u32Cmd,SequenceMMGetUsage(pMemorySpace),SequenceMMGetSpace(pMemorySpace));
        while(1);
    }
}
#else //SPT_DB_SEQMM_MAX_USAGE
#define SeqMMAssert(a)
#endif//SPT_DB_SEQMM_MAX_USAGE
//==================================public functions===============================================
int UADPBypassSetSPT(UADP_STRUCT_POINTER_TABLE* pSpt, unsigned int uiType, unsigned int uiOffset, unsigned long uiSizeOrNext)
{
    FlagPrint(SPT_DB_DETAIL)("pSpt=%p, uiType=%x,uiOffset=%x,uiSizeOrNext_x=%p,uiSizeOrNext_u=%lx\n",pSpt, uiType,uiOffset,uiSizeOrNext,uiSizeOrNext);
    pSpt->type          = uiType;
    pSpt->offset       = uiOffset;
    pSpt->size_or_next   = uiSizeOrNext;

    return 0;   //FIXME: It didn't define return value.
}

int UADPBypassIoctl(void* pInstance, unsigned int u32Cmd, void* pArgs,const UADP_STRUCT_POINTER_TABLE *pInTable,const UADP_STRUCT_POINTER_TABLE *pOutTable, void* pWorkBuffer, int iWorkBufferSize)
{
    void* karg=NULL;
    const UADP_STRUCT_POINTER_TABLE *FreeTable=NULL;
    void* data=NULL;
    unsigned int TurnUsrAddrOffset=0;
    MS_U32 ret=0;

    //1. generate (in_table_id,kernel_argc) space, and copy user_argc to (in_table_id,kernel_argc)
    if(SPT_DB_MEMINFO)ShowMemInfo("before malloc:\n");
    if(pInTable) //malloc(for in/out) and copy
    {
        FreeTable=pInTable;
	 	if(is_compat_task()==0)
	 	{
			StructRecursiveCpy(&karg,pArgs,pInTable,CPY_FROM_USER,SRC_MODE_COPY|SRC_MODE_MALLOC|SRC_MODE_CPY_FROM_USR);
    }
#ifdef CONFIG_COMPAT		
	 	else if(is_compat_task()==1)	
	 	{
			data = (void*)compat_ptr(pArgs);
	        CmpStructRecursiveCpy(&karg,data,pInTable,CPY_FROM_USER,SRC_MODE_COPY|SRC_MODE_MALLOC|SRC_MODE_CPY_FROM_USR, FALSE, (unsigned int*)&TurnUsrAddrOffset);
		}
#endif			
    }
    else if(pOutTable) //malloc(for out) only
    {
        //need malloc space for karg for output space
        FreeTable=pOutTable;
	 	if(is_compat_task()==0)
	 	{
            StructRecursiveCpy(&karg,pArgs,pOutTable,CPY_FROM_USER,SRC_MODE_MALLOC|SRC_MODE_CPY_FROM_USR);
        }
#ifdef CONFIG_COMPAT				
	 	else if(is_compat_task()==1)	
	 	{
        	data = (void*)compat_ptr(pArgs);
        	CmpStructRecursiveCpy(&karg,data,pOutTable,CPY_FROM_USER,SRC_MODE_MALLOC|SRC_MODE_CPY_FROM_USR, FALSE, (unsigned int*)&TurnUsrAddrOffset);
		}
#endif			
    }
    if(SPT_DB_MEMINFO)ShowMemInfo("after malloc:\n");
    //2. call fd->CMD_ID(fd, CMD_ID, kernel_argc)
    ret = UtopiaIoctl(pInstance,u32Cmd,karg);
    //3. copy (out_table_id,kernel_argc) to user_argc
    if(pOutTable)
    {
		if(is_compat_task()==0)
		{
        StructRecursiveCpy(&pArgs,karg,pOutTable,CPY_to_USER,SRC_MODE_COPY);
    }
#ifdef CONFIG_COMPAT		    
	 	else if(is_compat_task()==1)	
	 	{
        	data = (void*)compat_ptr(pArgs);
        	CmpStructRecursiveCpy(&data,karg,pOutTable,CPY_to_USER,SRC_MODE_COPY,FALSE, (unsigned int*)&TurnUsrAddrOffset);
	 	}
#endif			
    }
    //4. free(in_table_id,kernel_argc);
    if(FreeTable)
    {
        StructRecursiveFree(&karg,FreeTable);
    }
    if(SPT_DB_MEMINFO)ShowMemInfo("after free:\n");

    return ret;   //FIXME: It didn't define return value.
}