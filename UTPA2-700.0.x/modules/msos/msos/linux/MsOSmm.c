

#include <string.h>
#include "drvIPAPool.h"
#include "MsOSmm.h"
#include "MsOS.h"
#include "ULog.h"
#include  "drvList.h"
#include "mdrv_cma_pool_io.h"
#include "mdrv_cma_pool_st.h"
#include "drvCMAPool.h"
#include "mdrv_ipa_pool_uapi.h"


#ifndef ANDROID
#define VPRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>
#define VPRINTF ALOGD
#endif




#if defined(CONFIG_MSOSMM_CMA) || defined(CONFIG_MSOSMM_MPOOL)

#define MAX_MSOSMM_MPOOLSIZE 64UL
#define MPOOL_POOL_ID_OFFSET 65536UL



typedef struct _Msosmm_MM_INFO
{
   MS_BOOL bIsUsed;
   char *pool_name;
   MS_S32 pool_id;
   MS_S32 msos_mm_type;
   MS_PHY phyaddr;
   MS_U64 len;
} Msosmm_MM_INFO;


typedef struct _mmaping_info_object
{
    MS_VIRT start_va;
    MS_U64 start_pa;
    MS_U64 len;
    struct list_head  list;                                
}mmaping_info_object;


static list_t  mmaping_info_list;  
static MS_S32 msos_mpool_count = MPOOL_POOL_ID_OFFSET;
static Msosmm_MM_INFO Msosmm_MM_Info[MAX_MSOSMM_MPOOLSIZE];

static MS_S32 _s32_Msosmm_Mutex=-1;


static MS_BOOL _ExistId_InMM_Pool_Table(MS_S32 pool_id)
{
      MS_BOOL ret = FALSE;
      MS_S32 i;

      for(i=0;i<MAX_MSOSMM_MPOOLSIZE;i++)
       {
             if(Msosmm_MM_Info[i].pool_id == pool_id)
             	{
             	    ret = TRUE;
                  break;
             	}
       }

       return ret;
}

static void _RemoveId_InMM_Pool_Table(MS_S32 pool_id)
{

	   MS_S32 i;

         for(i=0;i<MAX_MSOSMM_MPOOLSIZE;i++)
         {
             if(Msosmm_MM_Info[i].pool_id == pool_id)
             	{
             	    Msosmm_MM_Info[i].pool_id = 0;
             	    Msosmm_MM_Info[i].bIsUsed = FALSE;
		    Msosmm_MM_Info[i].len = 0;
                  Msosmm_MM_Info[i].msos_mm_type = 0;
		    Msosmm_MM_Info[i].phyaddr = 0; 
		    free(Msosmm_MM_Info[i].pool_name);
                  break;
             	}
         }

}

static MS_BOOL _findId_InMM_Pool_Table(MS_PHY phyaddr,char* pool_name,MS_U64 length ,MS_U32 *index)
{
       MS_BOOL ret = FALSE;
       MS_S32 i;

       for(i=0;i<MAX_MSOSMM_MPOOLSIZE;i++)
       {
           if((Msosmm_MM_Info[i].phyaddr == phyaddr) && (Msosmm_MM_Info[i].len == length) && (strcmp(Msosmm_MM_Info[i].pool_name,pool_name) == 0))
           {
               *index = Msosmm_MM_Info[i].pool_id;
		 ret = TRUE;
		 break;
           }
       }

       return ret;
}


static MS_BOOL _findEmpty_MM_Entry(MS_U32 *index)
{
    MS_BOOL find = FALSE;
    MS_U32 i;

    *index = 0;
    for (i = 0; i < MAX_MSOSMM_MPOOLSIZE; i++)
    {
        if(Msosmm_MM_Info[i].bIsUsed == FALSE)
        {
            find = TRUE;
            *index = i;
            break;
        }
    }

    if(find == FALSE)
        VPRINTF("Not enough MM Pool, must increase MAX_MSOSMM_MPOOLSIZE!!\n");
    
    return find;
}



MS_BOOL Get_Mm_Phyaddr(MS_S32 pool_id,MS_PHY *phyaddr,MS_S32 *index)
{

    MS_S32 i;
    MS_BOOL ret = FALSE;

    for(i=0;i<MAX_MSOSMM_MPOOLSIZE;i++)
    {
         if(Msosmm_MM_Info[i].pool_id == pool_id)
         {
             *phyaddr = Msosmm_MM_Info[i].phyaddr;
             *index = i;
               ret = TRUE;
               break;
         }
    }

    return ret;

}


#endif


#if defined (CONFIG_MSOSMM_IPA) && !defined (CONFIG_MSOSMM_CMA) &&  !defined (CONFIG_MSOSMM_MPOOL)


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_Init(void)
{
      return (MS_S32)0;
}


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_DeInit(void)
{
      return (MS_S32)0;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Init(struct MSOS_MMPool_Init_Param * Init_Param)
{

     MS_BOOL ret=FALSE;
     struct IPA_Pool_Init_Param ipaInitParam;
     struct PA_In_IPA_Pool_Param in_ipa_pool_info;
     
     if(Init_Param == NULL || Init_Param->pool_name == NULL)
     {
         VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
	  goto done;
     }
	 
       memset(&ipaInitParam, 0, sizeof(ipaInitParam));

      in_ipa_pool_info.PA = Init_Param->PoolStartPa;

      if ((ret =PA_in_IPA_POOL_info(&in_ipa_pool_info)) == false ||  !in_ipa_pool_info.in_heap)
      {
             VPRINTF("\033[1;31m[%s][%d]  PA_in_IPA_POOL_info fail !!! \033[m\n",__FUNCTION__,__LINE__); 
              Init_Param->error_code = MSOSMMERROR_CREATE_POOL;
		goto done;       
      }

	 
      ipaInitParam.space_id = in_ipa_pool_info.space_id;
      ipaInitParam.pool_name = Init_Param->pool_name;
      ipaInitParam.offset_in_heap = in_ipa_pool_info.pa_offset_in_heap;
      ipaInitParam.len = Init_Param->len;

	 
      ret = MApi_IPA_Pool_Init(&ipaInitParam);

      if(ret == false)
      {
	   Init_Param->error_code = in_ipa_pool_info.error_code;
	   VPRINTF("\033[1;31m[%s][%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__); 
      }
      else
      {
          Init_Param->pool_handle_id =  ipaInitParam.pool_handle_id;
	   Init_Param->miu = ipaInitParam.miu;
	   Init_Param->heap_type = ipaInitParam.space_type;
	   Init_Param->heap_length = ipaInitParam.space_length;
	   Init_Param->heap_miu_start_offset = ipaInitParam.space_miu_start_offset;
      }
      
	 
done:
     return ret;

}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Release(MS_U32 pool_handle_id)
{

      MS_BOOL ret=FALSE;

      if((ret = MApi_IPA_Pool_Release(pool_handle_id)) == FALSE)
      {
            VPRINTF("\033[1;41m[%s][%d]  Id %d FAIL\033[m\n",__FUNCTION__,__LINE__,pool_handle_id);
      }

      return ret;

}


MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem(struct MSOS_MMPool_GetMem_Param * get_param)
{

      MS_BOOL ret=FALSE;
      struct IPA_Pool_GetMem_Param IPA_get_param;
    
      if(get_param == NULL)
      {
            VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
            get_param->error_code = MSOSMMERROR_GET_MEM;
            goto done;
      }

      memset(&IPA_get_param, 0, sizeof(IPA_get_param));

      IPA_get_param.pool_handle_id = get_param->chunk.pool_handle_id;
      IPA_get_param.offset_in_pool = get_param->chunk.offset_in_pool;
      IPA_get_param.length = get_param->chunk.length;

      if((ret=MApi_IPA_Pool_GetMem(&IPA_get_param)) == FALSE)
      {
            get_param->error_code = IPA_get_param.error_code;
            VPRINTF("\033[1;31m[%s][%d]  Id %d fail !!! \033[m\n",__FUNCTION__,__LINE__,IPA_get_param.pool_handle_id);     
      }
	 

done:

     return ret;

        
}



MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem_Timeout(struct MSOS_MMPool_GetMem_Param* get_param,MS_U32 timeout)
{


      MS_BOOL ret=FALSE;
      struct IPA_Pool_GetMem_Param IPA_get_param;
    
      if(get_param == NULL)
      {
            VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
            goto done;
      }

      memset(&IPA_get_param, 0, sizeof(IPA_get_param));

      IPA_get_param.pool_handle_id = get_param->chunk.pool_handle_id;
      IPA_get_param.offset_in_pool = get_param->chunk.offset_in_pool;
      IPA_get_param.length = get_param->chunk.length;

      if((ret=MApi_IPA_Pool_GetMem_Timeout(&IPA_get_param , timeout)) == false)
      {
            get_param->error_code = IPA_get_param.error_code;
            VPRINTF("\033[1;31m[%s] [%d]  Id %d fail !!! \033[m\n",__FUNCTION__,__LINE__,IPA_get_param.pool_handle_id);     
      }
	 

done:

      return ret;

}

MS_BOOL __attribute__((weak)) MSOS_MMPool_PutMem(struct MSOS_MMPool_PutMem_Param * put_param)
{
      MS_BOOL ret = FALSE;
      struct IPA_Pool_PutMem_Param IPA_put_param;
      memset(&IPA_put_param, 0, sizeof(IPA_put_param));

      if(put_param == NULL)
      {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
           goto done;
      }

      IPA_put_param.pool_handle_id =put_param->chunk.pool_handle_id;
      IPA_put_param.length = put_param->chunk.length;
      IPA_put_param.offset_in_pool = put_param->chunk.offset_in_pool;    

      if((ret=MApi_IPA_Pool_PutMem(&IPA_put_param)) == FALSE)
      {
            VPRINTF("\033[1;41m[%s] [%d] Id %d FAIL\033[m\n",__FUNCTION__,__LINE__,put_param->chunk.pool_handle_id);
      }


done:
	return ret;
	
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_MapVA(struct MSOS_MMPool_Map_Param * map_param)
{

     MS_BOOL ret=FALSE;
     struct IPA_Pool_Map_Param IPA_map_param;
     memset(&IPA_map_param, 0, sizeof(IPA_map_param));

     if(map_param == NULL)
     {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
           goto done;
     }

     map_param->virt_addr =0;

     IPA_map_param.pool_handle_id = map_param->chunk.pool_handle_id;
     IPA_map_param.offset_in_pool = map_param->chunk.offset_in_pool;
     IPA_map_param.length =  map_param->chunk.length ; 
     IPA_map_param.cache_type = map_param->cache_type;

      if((ret = MApi_IPA_Pool_MapVA(&IPA_map_param)) == false)
      {
           map_param->error_code = IPA_map_param.error_code;
	    VPRINTF("\033[1;31m[%s][%d]  Id %d  fail !!! \033[m\n",__FUNCTION__,__LINE__,IPA_map_param.pool_handle_id); 
      }
      else
      {
           map_param->virt_addr = (MS_U8*)IPA_map_param.virt_addr;
      }
	  
done:

	return ret;
}

void    __attribute__((weak)) MSOS_MMPool_UnmapVA(struct MSOS_MMPool_Unmap_Param * unmap_param)
{


     struct IPA_Pool_Unmap_Param IPA_unmap_param;

     if(unmap_param == NULL)
     {
         VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
	  goto done;
     }

     memset(&IPA_unmap_param, 0, sizeof(IPA_unmap_param));

     IPA_unmap_param.virt_addr = unmap_param->virt_addr;
     IPA_unmap_param.length = unmap_param->length;

     MApi_IPA_Pool_UnmapVA(&IPA_unmap_param);

done:

	return;
      
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Notify(struct MSOS_MMPool_Notify_Param *notify_param)
{

     MS_BOOL ret =FALSE;
     struct IPA_Pool_Polling_Param stIPAPoolPollingParam;
     memset(&stIPAPoolPollingParam, 0, sizeof(stIPAPoolPollingParam));

     if(notify_param == NULL)
     {
        VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
	 goto done;
     }

     stIPAPoolPollingParam.pool_handle_id = notify_param->pool_handle_id;
     stIPAPoolPollingParam.polling_callback = notify_param->notify_callback;


     if ((ret =MApi_IPA_Pool_Notify(&stIPAPoolPollingParam)) == TRUE)
     {
           VPRINTF("sucess to call [%s].\n",__FUNCTION__);
     }
     else
     {
           VPRINTF("fail to call [%s].\n",__FUNCTION__);
     }
	 

done:
     
     return ret;
	 
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Locate_Chunk(struct MSOS_MMPool_Locate_Chunk_Param * chunk_param)
{
      MS_BOOL ret = FALSE;
      struct Pool_To_PA_Param stPoolToPaParam;
     

      if(chunk_param == NULL)
      {
            VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
	     goto done;
      }

      memset(&stPoolToPaParam, 0, sizeof(stPoolToPaParam));

      stPoolToPaParam.pool_handle_id = chunk_param->pool_handle_id;
      stPoolToPaParam.offset_in_pool = chunk_param->offset_in_pool;
      

       if ((ret=IN_IPA_POOL_To_PA(&stPoolToPaParam)) == FALSE)
       {
            VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
            goto done;
       }

       chunk_param->PA =  stPoolToPaParam.PA;
       chunk_param->miu = stPoolToPaParam.miu;
       chunk_param->error_code = stPoolToPaParam.error_code;
       chunk_param->heap_id = stPoolToPaParam.heap_id;

       ret = TRUE;

done:

      return ret;
}



#endif


#if defined(CONFIG_MSOSMM_CMA) && !defined(CONFIG_MSOSMM_IPA)  && !defined(CONFIG_MSOSMM_MPOOL)


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_Init(void)
{

      MS_U32 i;

      _s32_Msosmm_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*) "Msosmm_Mutex", MSOS_PROCESS_SHARED);
	  
      memset(Msosmm_MM_Info, 0, sizeof(Msosmm_MM_INFO)*MAX_MSOSMM_MPOOLSIZE);

      ms_list_init(&mmaping_info_list);

      
        for (i = 0; i < MAX_MSOSMM_MPOOLSIZE; i++)
        {
             Msosmm_MM_Info[i].bIsUsed = FALSE;
	      Msosmm_MM_Info[i].len = 0;
	      Msosmm_MM_Info[i].phyaddr = 0;
             Msosmm_MM_Info[i].pool_name = NULL;
        }
	  
      return (MS_S32)0;
}


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_DeInit(void)
{
      return (MS_S32)0;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Init(struct MSOS_MMPool_Init_Param * Init_Param)
{
        MS_BOOL ret=FALSE;
        struct PA_In_CMA_Pool_Param in_cma_pool_info;
        struct CMA_Pool_Init_Param cmaInitParam;
        MS_U32 idx;

        MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);
     
        if(Init_Param == NULL || Init_Param->pool_name == NULL)
        {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
	    goto done;
        }
	 
        memset(&cmaInitParam, 0, sizeof(cmaInitParam));

        in_cma_pool_info.PA = Init_Param->PoolStartPa;

        
        if ((ret =MApi_CMA_Pool_PA_in_pool_info(&in_cma_pool_info)) == false ||  !in_cma_pool_info.in_heap)
        {


                 ret = _findId_InMM_Pool_Table(Init_Param->PoolStartPa,Init_Param->pool_name,Init_Param->len ,&idx);
		
		   if(ret == FALSE)
		   {
                        ret = _findEmpty_MM_Entry(&idx);
						
                        if(ret == FALSE)
                        {
                             goto done;
                        }
		   
                        Msosmm_MM_Info[idx].pool_id = msos_mpool_count;
			   Msosmm_MM_Info[idx].pool_name = strdup(Init_Param->pool_name);

                        if(Msosmm_MM_Info[idx].pool_name == NULL)
                        {
                                VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
                                Init_Param->error_code = MSOSMMERROR_CREATE_POOL;
				    ret = FALSE;
                                goto done;
                        }
 
			   Msosmm_MM_Info[idx].len = Init_Param->len;
                        Msosmm_MM_Info[idx].phyaddr = Init_Param->PoolStartPa;
                        Msosmm_MM_Info[idx].bIsUsed = TRUE;
		          Init_Param->pool_handle_id = Msosmm_MM_Info[idx].pool_id; 
		          msos_mpool_count++;
		          
		   }
		   else
		   {          
                       VPRINTF("\033[1;31m[%s] [%d]  pool name:%s has initialized  !!! \033[m\n",__FUNCTION__,__LINE__,Init_Param->pool_name);
		   }
        }
	else
	{
	         memset(&cmaInitParam, 0, sizeof(cmaInitParam));
		  cmaInitParam.heap_id = in_cma_pool_info.heap_id;
		  cmaInitParam.flags = CMA_FLAG_MAP_VMA | CMA_FLAG_CACHED;

		  if ((ret = MApi_CMA_Pool_Init(&cmaInitParam)) == FALSE)
		  {
		       VPRINTF("\033[1;31m[%s][%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__); 
                     Init_Param->error_code = MSOSMMERROR_CREATE_POOL;
                     goto done;
		  }

		  Init_Param->pool_handle_id = cmaInitParam.pool_handle_id;
		  Init_Param->miu = cmaInitParam.miu;
		  Init_Param->heap_miu_start_offset = cmaInitParam.heap_miu_start_offset;
		  Init_Param->heap_length = cmaInitParam.heap_length;
		  
	}

       ret = TRUE;
done:

	 MsOS_ReleaseMutex(_s32_Msosmm_Mutex);       

	 return ret;

}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Release(MS_U32 pool_handle_id)
{
      MS_BOOL ret=FALSE;
      MS_U32 idx;

      MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);

      ret = _ExistId_InMM_Pool_Table(pool_handle_id);

      if(ret == TRUE)
      	{
      	   _RemoveId_InMM_Pool_Table(pool_handle_id);
      	    goto done;
      	}


      if((ret=MApi_CMA_Pool_Release(pool_handle_id)) == false)
      {
           VPRINTF("\033[1;41m[%s] [%d] Id %d FAIL\033[m\n",__FUNCTION__,__LINE__,pool_handle_id);
      }

done:

      MsOS_ReleaseMutex(_s32_Msosmm_Mutex);       
   
      return ret;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem(struct MSOS_MMPool_GetMem_Param * get_param)
{

      MS_BOOL ret=FALSE;
      MS_PHY phyaddr;
      MS_S32 index;
      struct CMA_Pool_Alloc_Param alloc_param;
    
      if(get_param == NULL)
      {
            VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
            goto done;
      }

      
      ret = Get_Mm_Phyaddr(get_param->chunk.pool_handle_id, &phyaddr,&index);

      if(ret == TRUE)
      	{  
      	    get_param->error_code = IPAERROR_OK;
      	    goto done;
      	}


      memset(&alloc_param, 0, sizeof(alloc_param));

      alloc_param.pool_handle_id = get_param->chunk.pool_handle_id;
      alloc_param.offset_in_pool  = get_param->chunk.offset_in_pool;
      alloc_param.length = get_param->chunk.length;
      alloc_param.flags = CMA_FLAG_VIRT_ADDR;

        if ((ret=MApi_CMA_Pool_GetMem(&alloc_param)) == false)
        {
              VPRINTF("\033[1;31m[%s][%d] Id %d fail !!! \033[m\n",__FUNCTION__,__LINE__,get_param->chunk.pool_handle_id); 
              get_param->error_code = MSOSMMERROR_GET_MEM;
              goto done;
        }

done:

	return ret;
}



MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem_Timeout(struct MSOS_MMPool_GetMem_Param* get_param,MS_U32 timeout)
{


      return MSOS_MMPool_GetMem(get_param);

}

MS_BOOL __attribute__((weak)) MSOS_MMPool_PutMem(struct MSOS_MMPool_PutMem_Param * put_param)
{
     MS_BOOL ret = FALSE;
     MS_PHY phyaddr;
     MS_S32 index;
     struct CMA_Pool_Free_Param free_param;
      memset(&free_param, 0, sizeof(free_param));

     if(put_param == NULL)
     {
         VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
         goto done;
     }


     /*if the the pool id is for mpool ,we go to end directly*/
      ret = Get_Mm_Phyaddr(put_param->chunk.pool_handle_id, &phyaddr,&index);

      if(ret == TRUE)
      	{
      	    goto done;
      	}


     free_param.pool_handle_id =put_param->chunk.pool_handle_id;
     free_param.length = put_param->chunk.length;
     free_param.offset_in_pool = put_param->chunk.offset_in_pool;    

    if((ret=MApi_CMA_Pool_PutMem(&free_param))==FALSE)
    {
         VPRINTF("\033[1;41m[%s] [%d]  FAIL\033[m\n",__FUNCTION__,__LINE__,put_param->chunk.pool_handle_id);
    }

	
 done:

    return ret;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_MapVA(struct MSOS_MMPool_Map_Param * map_param)
{
      MS_PHY phyaddr;
      MS_VIRT pdummy = 0;
      MS_BOOL ret = false;
      MS_U32 u32MiuOffset = 0;
      MS_U8 u8MiuSel = 0;
      MS_S32 index;
      mmaping_info_object *object;
      MS_PHY offset_phyaddr;
      list_t *node;
      mmaping_info_object* mmaping_info_ptr;

      MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);

      if(map_param == NULL)
      {
          VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
          goto done;
      }

      map_param->virt_addr =0;

      ret = Get_Mm_Phyaddr(map_param->chunk.pool_handle_id, &phyaddr,&index);

       offset_phyaddr = phyaddr + map_param->chunk.offset_in_pool;


      if(ret == FALSE)
      	{
             if(map_param->cache_type & MSOSMM_VA_CACHE_WRITE_BACK_Param)
            {
                 map_param->virt_addr = (MS_U8*)MsOS_PA2KSEG0(offset_phyaddr);

		   if(map_param->virt_addr )
		        ret = TRUE;
            }
            else  if(map_param->cache_type & MSOSMM_VA_CACHE_NONE_CACHE_Param)
            {
                 map_param->virt_addr = (MS_U8*)MsOS_PA2KSEG1(offset_phyaddr);

                if(map_param->virt_addr )
                       ret = TRUE;
            }
		
      	     goto done;
      	}

      

       list_for_each_entry(mmaping_info_ptr,&mmaping_info_list,list)
      {
             if(mmaping_info_ptr->start_pa == offset_phyaddr &&  mmaping_info_ptr->len  == map_param->chunk.length)
             {
                   VPRINTF("\033[1;31m pool_handle_id:%d  has mapped\033[m\n",map_param->chunk.pool_handle_id);
		     VPRINTF("\033[1;31m[%s][%d] mapping info (start physical address:0x%x , end physical address:0x%x) contains physical address :0x%x, length:0x%x\033[m\n",mmaping_info_ptr->start_pa ,(mmaping_info_ptr->start_pa + mmaping_info_ptr->len),offset_phyaddr,map_param->chunk.length);
				   
                   ret = TRUE;
		     goto done;
             }
      }
      
       if(map_param->cache_type & MSOSMM_VA_CACHE_WRITE_BACK_Param)
       {
           map_param->virt_addr = (MS_U8*)MsOS_PA2KSEG0(offset_phyaddr);
       }
       else  if(map_param->cache_type & MSOSMM_VA_CACHE_NONE_CACHE_Param)
       {

              pdummy = MsOS_PA2KSEG1(offset_phyaddr);

              if(pdummy == 0)
	       {
	            _phy_to_miu_offset(u8MiuSel, u32MiuOffset,offset_phyaddr);

	            if(MsOS_MPool_Mapping(u8MiuSel, (u32MiuOffset), map_param->chunk.length, TRUE) == TRUE)
                   {
                         pdummy = MsOS_PA2KSEG1(offset_phyaddr);
                   }
	            else
	           {
                        VPRINTF("\033[1;31m[%s][%d] fail !!! \033[m\n",__FUNCTION__,__LINE__);
                        map_param->error_code = MSOSMMERROR_MAPVA;
                        ret = FALSE;
                        goto done;
		     }

                   map_param->virt_addr = (MS_U8*)pdummy;
	       }
			  
       }


        object = malloc(sizeof(mmaping_info_object));

	 if(object == NULL)
	 {
	      VPRINTF("\033[1;31m[%s][%d] fail !!! \033[m\n",__FUNCTION__,__LINE__);
             map_param->error_code = MSOSMMERROR_MAPVA;
	      goto done;
	 }
	 
        object->start_pa = offset_phyaddr;
	 object->start_va = (MS_U8*)pdummy;
	 object->len = map_param->chunk.length;
	 ms_insert_list_after(&(mmaping_info_list), &(object->list));
     
       ret = TRUE;
done:


       MsOS_ReleaseMutex(_s32_Msosmm_Mutex);

      return ret;
}

void    __attribute__((weak)) MSOS_MMPool_UnmapVA(struct MSOS_MMPool_Unmap_Param * unmap_param)
{
      MS_BOOL  ret = FALSE;
      list_t *node;
      mmaping_info_object* mmaping_info_ptr;

      MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);

      if(unmap_param == NULL || unmap_param->length == 0)
      {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
          goto done;
      }


      list_for_each_entry(mmaping_info_ptr,&mmaping_info_list,list)
      {
            if((mmaping_info_ptr->start_va == unmap_param->virt_addr) && mmaping_info_ptr->len == unmap_param->length)
            {
                 ret = TRUE;
            }
      }

	if(ret == FALSE)
	{
	     goto done;
	}

       MsOS_MPool_UnMapping(unmap_param->virt_addr,unmap_param->length);


       list_remove(&mmaping_info_ptr->list);             
       free(mmaping_info_ptr);
                  
      

done: 

      MsOS_ReleaseMutex(_s32_Msosmm_Mutex);
	
      return ;
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Notify(struct MSOS_MMPool_Notify_Param *notify_param)
{

      return FALSE;
	 
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Locate_Chunk(struct MSOS_MMPool_Locate_Chunk_Param * chunk_param)
{
      if(chunk_param != NULL)
      {
           chunk_param->error_code = MSOSMMERROR_DONOT_SUPPORT;
      }
	  
       return FALSE;
}


#endif


#if defined(CONFIG_MSOSMM_MPOOL) && !defined(CONFIG_MSOSMM_IPA)  && !defined(CONFIG_MSOSMM_CMA)


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_Init(void)
{

        MS_U32 i;

        _s32_Msosmm_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*) "Msosmm_Mutex", MSOS_PROCESS_SHARED);
        memset(Msosmm_MM_Info, 0, sizeof(Msosmm_MM_INFO)*MAX_MSOSMM_MPOOLSIZE);

        ms_list_init(&mmaping_info_list);

      
        for (i = 0; i < MAX_MSOSMM_MPOOLSIZE; i++)
        {
             Msosmm_MM_Info[i].bIsUsed = FALSE;
	      Msosmm_MM_Info[i].len = 0;
	      Msosmm_MM_Info[i].phyaddr = 0;
             Msosmm_MM_Info[i].pool_name = NULL;
        }

      return (MS_S32)0;
}


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_DeInit(void)
{
      return (MS_S32)0;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Init(struct MSOS_MMPool_Init_Param * Init_Param)
{
        MS_BOOL ret=FALSE;
       
        MS_U32 idx;

        MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);
     
        if(Init_Param == NULL || Init_Param->pool_name == NULL)
        {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
	    goto done;
         }

         ret = _findId_InMM_Pool_Table(Init_Param->PoolStartPa,Init_Param->pool_name,Init_Param->len ,&idx);
		
         if(ret == FALSE)
         {
               ret = _findEmpty_MM_Entry(&idx);
						
               if(ret == FALSE)
               {
                     Init_Param->error_code = MSOSMMERROR_CREATE_POOL;
                     goto done;
                }
		   
                Msosmm_MM_Info[idx].pool_id = msos_mpool_count;
                Msosmm_MM_Info[idx].pool_name = strdup(Init_Param->pool_name);

                if(Msosmm_MM_Info[idx].pool_name == NULL)
                {
                     VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
                     Init_Param->error_code = MSOSMMERROR_CREATE_POOL;
			ret = FALSE;
                     goto done;
                }

                Msosmm_MM_Info[idx].len = Init_Param->len;
                Msosmm_MM_Info[idx].phyaddr = Init_Param->PoolStartPa;
                Msosmm_MM_Info[idx].bIsUsed = TRUE;
		  Init_Param->pool_handle_id = Msosmm_MM_Info[idx].pool_id;
		  msos_mpool_count++;
		          
          }
          else
          {
                 VPRINTF("\033[1;31m[%s] [%d]  pool name:%s has initialized  !!! \033[m\n",__FUNCTION__,__LINE__,Init_Param->pool_name);
	          Msosmm_MM_Info[idx].pool_id =  Msosmm_MM_Info[idx].pool_id;
          }

done:

	 MsOS_ReleaseMutex(_s32_Msosmm_Mutex);  
	 return ret;

}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Release(MS_U32 pool_handle_id)
{

       MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);
       _RemoveId_InMM_Pool_Table(pool_handle_id);
	MsOS_ReleaseMutex(_s32_Msosmm_Mutex);  
       return TRUE;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem(struct MSOS_MMPool_GetMem_Param * get_param)
{
       MS_BOOL ret = FALSE;

       if(get_param == NULL)
       {
             VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
		goto done;
       }

        ret = TRUE;
	 get_param->error_code = 0;

done:
	
        return ret;
}



MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem_Timeout(struct MSOS_MMPool_GetMem_Param* get_param,MS_U32 timeout)
{


      return MSOS_MMPool_GetMem(get_param);

}

MS_BOOL __attribute__((weak)) MSOS_MMPool_PutMem(struct MSOS_MMPool_PutMem_Param * put_param)
{
     return TRUE;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_MapVA(struct MSOS_MMPool_Map_Param * map_param)
{

      MS_PHY phyaddr;
      MS_PHY offset_phyaddr;
      MS_VIRT pdummy = 0;
      MS_BOOL ret = FALSE;
      MS_U32 u32MiuOffset = 0;
      MS_U8 u8MiuSel = 0;
      MS_S32 index;
      mmaping_info_object *object;
      list_t *node;
      mmaping_info_object* mmaping_info_ptr;

      MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);

      if(map_param == NULL)
      {
          VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
          goto done;
      }

      map_param->virt_addr =0;

      ret = Get_Mm_Phyaddr(map_param->chunk.pool_handle_id, &phyaddr,&index);

      if(ret == FALSE)
      	{
      	    map_param->error_code = MSOSMMERROR_MAPVA;
           VPRINTF("\033[1;31m[%s] [%d]  fail ,use pa method,but not find,may not init pool !!! \033[m\n",__FUNCTION__,__LINE__);
      	    goto done;
      	}

        offset_phyaddr = phyaddr + map_param->chunk.offset_in_pool;

        
        list_for_each_entry(mmaping_info_ptr,&mmaping_info_list,list)
       {
              if(mmaping_info_ptr->start_pa == offset_phyaddr && mmaping_info_ptr->len  ==  map_param->chunk.length)
             {
                   
		      VPRINTF("\033[1;31m pool_handle_id:%d  has mapped\033[m\n",map_param->chunk.pool_handle_id);
		      VPRINTF("\033[1;31m[%s][%d] mapping info (start physical address:0x%x , end physical address:0x%x) contains physical address :0x%x, length:0x%x\033[m\n",mmaping_info_ptr->start_pa ,(mmaping_info_ptr->start_pa + mmaping_info_ptr->len),offset_phyaddr,map_param->chunk.length);
				   
                   ret = TRUE;
		     goto done;
             }
       }
      
       
        
       if(map_param->cache_type & MSOSMM_VA_CACHE_WRITE_BACK_Param)
       {
           map_param->virt_addr = (MS_U8*)MsOS_PA2KSEG0(offset_phyaddr);
       }
       else  if(map_param->cache_type & MSOSMM_VA_CACHE_NONE_CACHE_Param)
       {

              pdummy = MsOS_PA2KSEG1(offset_phyaddr);

              if(pdummy == 0)
	       {
	            _phy_to_miu_offset(u8MiuSel, u32MiuOffset,offset_phyaddr);

	            if(MsOS_MPool_Mapping(u8MiuSel, (u32MiuOffset), map_param->chunk.length, TRUE) == TRUE)
                   {
                         pdummy = MsOS_PA2KSEG1(offset_phyaddr);
                   }
	            else
	           {
                        VPRINTF("\033[1;31m[%s][%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
                        ret = FALSE;
                        map_param->error_code = MSOSMMERROR_MAPVA;
                        goto done;
		     }

                   map_param->virt_addr = (MS_U8*)pdummy;
	       }
			  
       }

        object = malloc(sizeof(mmaping_info_object));

	 if(object == NULL)
	 {
	      VPRINTF("\033[1;31m[%s][%d] fail !!! \033[m\n",__FUNCTION__,__LINE__);
             map_param->error_code = MSOSMMERROR_MAPVA;
             ret = FALSE;
	      goto done;
	 }


	 object->start_pa = offset_phyaddr;
	 object->start_va = (MS_U8*)pdummy;
	 object->len = map_param->chunk.length;
	 ms_insert_list_after(&(mmaping_info_list), &(object->list));
	   

     
       ret = TRUE;

  
done:


       MsOS_ReleaseMutex(_s32_Msosmm_Mutex);

      return ret;
	
}

void    __attribute__((weak)) MSOS_MMPool_UnmapVA(struct MSOS_MMPool_Unmap_Param * unmap_param)
{
      MS_BOOL  ret = FALSE;
      list_t *node;
      mmaping_info_object* mmaping_info_ptr;

     MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);

      if(unmap_param == NULL || unmap_param->length == 0)
      {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
          goto done;
      }

      list_for_each_entry(mmaping_info_ptr,&mmaping_info_list,list)
      {
            if((mmaping_info_ptr->start_va == unmap_param->virt_addr) && mmaping_info_ptr->len == unmap_param->length)
            {
                 ret = TRUE;
            }
      }

	if(ret == FALSE)
	{
	     goto done;
	}

      if(MsOS_MPool_UnMapping(mmaping_info_ptr->start_va,mmaping_info_ptr->len) == FALSE)
      {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
      }

      
      list_remove(&mmaping_info_ptr->list);             
      free(mmaping_info_ptr);
		 

done: 

      MsOS_ReleaseMutex(_s32_Msosmm_Mutex);
	
      return ;
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Notify(struct MSOS_MMPool_Notify_Param *notify_param)
{

      return FALSE;
	 
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Locate_Chunk(struct MSOS_MMPool_Locate_Chunk_Param * chunk_param)
{
     if(chunk_param != NULL)
     {
          chunk_param->error_code = MSOSMMERROR_DONOT_SUPPORT;
     }


      return FALSE;
}


#endif



//#if !defined(CONFIG_MSOSMM_MPOOL) && !defined(CONFIG_MSOSMM_IPA)  && !defined(CONFIG_MSOSMM_CMA)

    //  #error unknown msosmm api!!

//#endif

