
#include <unistd.h>
#include "string.h"
#include "MsVersion.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "MsOSmm.h"
#include "ULog.h"
#include "halCHIP.h"
#include  "drvList.h"


#define VPRINTF(format,args...) printf("MSOSMM", format, ##args)

 
#define MAX_MSOSMM_MPOOLSIZE 64

 
typedef struct _Msosmm_MPOOL_INFO
{
   MS_BOOL bIsUsed;
   char *pool_name;
   MS_U64 len;
   MS_U64 phyaddr;
} Msosmm_MPOOL_INFO;


typedef struct _mmaping_info_object
{
    MS_VIRT start_va;
    MS_U64 start_pa;
    MS_U64 len;
    MS_U32 cache_type;
    struct list_head  list;                                
}mmaping_info_object;



//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

static Msosmm_MPOOL_INFO Msosmm_MPool_Info[MAX_MSOSMM_MPOOLSIZE];
static list_t  mmaping_info_list;
static MS_S32 _s32_Msosmm_Mutex=-1;


static MS_BOOL _check_MM_Pool_Id(MS_U32 id)
{
     if(id >= MAX_MSOSMM_MPOOLSIZE)
	 return FALSE;
     else
	 return TRUE;
}


static MS_BOOL _findId_InMM_Pool_Table(MS_PHY phyaddr,char* pool_name,MS_U64 length ,MS_U32 *index)
{
       MS_BOOL ret = FALSE;
       MS_S32 i;

       for(i=0;i<MAX_MSOSMM_MPOOLSIZE;i++)
       {
           if((Msosmm_MPool_Info[i].phyaddr == phyaddr) && (Msosmm_MPool_Info[i].len == length) && (strcmp(Msosmm_MPool_Info[i].pool_name,pool_name) == 0))
           {
               *index = i;
		 ret = TRUE;
		 break;
           }
       }

       return ret;
}

static MS_BOOL _findEmpty_MM_Pool_Entry(MS_U32 *index)
{
    MS_BOOL find = FALSE;
    MS_U32 i;

    *index = 0;
    for (i = 0; i < MAX_MSOSMM_MPOOLSIZE; i++)
    {
        if(Msosmm_MPool_Info[i].bIsUsed == FALSE)
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

static void _RemoveId_InMM_Pool_Table(MS_S32 pool_id)
{

	   MS_S32 i;

	   if(_check_MM_Pool_Id(pool_id) == FALSE)
	      return;

          Msosmm_MPool_Info[pool_id].bIsUsed = FALSE;
	   Msosmm_MPool_Info[pool_id].len = 0;
	    Msosmm_MPool_Info[pool_id].phyaddr = 0;
          free(Msosmm_MPool_Info[pool_id].pool_name);
          
}


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_Init(void)
{
       MS_U32 i;

	ms_list_init(&mmaping_info_list);
	   
        _s32_Msosmm_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*) "Msosmm_Mutex", MSOS_PROCESS_SHARED);
		
        for (i = 0; i < MAX_MSOSMM_MPOOLSIZE; i++)
        {
             Msosmm_MPool_Info[i].bIsUsed = FALSE;
        }

		
       return (MS_S32)0;
}


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_DeInit(void)
{
      MsOS_DeleteMutex(_s32_Msosmm_Mutex);
      return (MS_S32)0;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Init(struct MSOS_MMPool_Init_Param * Init_Param)
{
        MS_BOOL ret = FALSE;
	 MS_U32 index;
	 MS_U32 new_pool_id;

        if(Init_Param == NULL || Init_Param->pool_name == NULL)
        {
             VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
             goto done;
        }


        MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);


        ret =  _findId_InMM_Pool_Table(Init_Param->PoolStartPa,Init_Param->pool_name,Init_Param->len,&index);

        if(ret == TRUE)
        {
             Init_Param->pool_handle_id = index; 
             VPRINTF("\033[1;31m[%s] [%d]  pool name:%s has initialized  !!! \033[m\n",__FUNCTION__,__LINE__,Init_Param->pool_name);
             goto done;
        }

        if((ret = _findEmpty_MM_Pool_Entry(&new_pool_id)) == FALSE)
	 {
	       VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
              Init_Param->error_code = MSOSMMERROR_CREATE_POOL;
	       goto done;
	 }

	  Msosmm_MPool_Info[new_pool_id].pool_name = strdup(Init_Param->pool_name);

	  if(Msosmm_MPool_Info[new_pool_id].pool_name == NULL)
         {
              VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
               Init_Param->error_code = MSOSMMERROR_CREATE_POOL;
		 ret = FALSE;
               goto done;
         }
       
         Msosmm_MPool_Info[new_pool_id].phyaddr = Init_Param->PoolStartPa;
         Msosmm_MPool_Info[new_pool_id].bIsUsed = TRUE;
        
	 Msosmm_MPool_Info[new_pool_id].len = Init_Param->len;

        Init_Param->pool_handle_id = new_pool_id;

        
	 ret = TRUE;
	       
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

       if(get_param == NULL)
       {
             VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
		goto done;
       }

	get_param->error_code = 0;

done:

     return TRUE;
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
        MS_BOOL ret = FALSE;
        MS_VIRT pdummy = 0;
        MS_U32 u32MiuOffset = 0;
        MS_U8 u8MiuSel = 0;
        mmaping_info_object* mmaping_info_ptr;
        mmaping_info_object *object;
	 

      MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);
     
      if(map_param == NULL)
      {
         VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
         goto done;		 
      }

      map_param->virt_addr = 0;
    
      if(_check_MM_Pool_Id(map_param->chunk.pool_handle_id) == FALSE || Msosmm_MPool_Info[map_param->chunk.pool_handle_id].bIsUsed == FALSE)
      {
           VPRINTF("\033[1;31m[%s] [%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
           map_param->error_code = MSOSMMERROR_MAPVA;
           goto done;
      }

       list_for_each_entry(mmaping_info_ptr,&mmaping_info_list,list)
      {
            if(mmaping_info_ptr->start_pa == Msosmm_MPool_Info[map_param->chunk.pool_handle_id].phyaddr  && mmaping_info_ptr->len == map_param->chunk.length && mmaping_info_ptr->cache_type == map_param->cache_type)
            {
                   map_param->virt_addr = mmaping_info_ptr->start_va;
                   ret = TRUE;
                   goto done;
            }
      }


      if(map_param->cache_type  & MSOSMM_VA_CACHE_WRITE_BACK_Param)
       {
                VPRINTF("\033[1;31m[%s] Warning : Do not support  MSOSMM_VA_CACHE_WRITE_BACK_Param\033[m\n",__FUNCTION__,__LINE__);
                goto done;
       }
       else if(map_param->cache_type &  MSOSMM_VA_CACHE_NONE_CACHE_Param)
       {

             pdummy = MsOS_PA2KSEG1(Msosmm_MPool_Info[map_param->chunk.pool_handle_id].phyaddr);

	      if(pdummy == 0)
	      {

	       
	           _phy_to_miu_offset(u8MiuSel, u32MiuOffset, Msosmm_MPool_Info[map_param->chunk.pool_handle_id].phyaddr);

	          if((ret = MsOS_MPool_Mapping(u8MiuSel, (u32MiuOffset), map_param->chunk.length, TRUE))== TRUE)
                 {
                       pdummy = MsOS_PA2KSEG1(Msosmm_MPool_Info[map_param->chunk.pool_handle_id].phyaddr);
                 }
	          else
	          {
                      VPRINTF("\033[1;31m[%s][%d]  fail !!! \033[m\n",__FUNCTION__,__LINE__);
			 map_param->error_code = MSOSMMERROR_MAPVA;
                      goto done;
		   }
	      }

	      map_param->virt_addr = (MS_U8*)pdummy;

	      object = malloc(sizeof(mmaping_info_object));

	      if(object == NULL)
	      {
	          VPRINTF("\033[1;31m[%s][%d] fail !!! \033[m\n",__FUNCTION__,__LINE__);
                 map_param->error_code = MSOSMMERROR_MAPVA;
                 ret = FALSE;
	          goto done;
	      }

             object->start_pa = Msosmm_MPool_Info[map_param->chunk.pool_handle_id].phyaddr;
	      object->start_va = (MS_U8*)pdummy;
	      object->len = map_param->chunk.length;
	      object->cache_type = map_param->cache_type ;
     	      ms_insert_list_after(&(mmaping_info_list), &(object->list));
             ret = TRUE;

       }    
	  
      

done:

     MsOS_ReleaseMutex(_s32_Msosmm_Mutex);       

     return ret;
}

void    __attribute__((weak)) MSOS_MMPool_UnmapVA(struct MSOS_MMPool_Unmap_Param * unmap_param)
{
      mmaping_info_object* mmaping_info_ptr;
      MS_BOOL ret = FALSE;

      MsOS_ObtainMutex(_s32_Msosmm_Mutex, MSOS_WAIT_FOREVER);

       list_for_each_entry(mmaping_info_ptr,&mmaping_info_list,list)
      {
            if(mmaping_info_ptr->start_va == (MS_VIRT)unmap_param->virt_addr && mmaping_info_ptr->len == unmap_param->length)
            {
                   list_remove(&mmaping_info_ptr->list);  
                   ret = TRUE;
                   goto done;
            }
      }

 done:   

      if(ret == FALSE)
      {
           VPRINTF("\033[1;31m Warning: can not find virtual address:0x%p,length=0x%llx!!! \033[m\n",(MS_PHY)unmap_param->virt_addr, unmap_param->length);
      }
	  
      MsOS_ReleaseMutex(_s32_Msosmm_Mutex);

      return;      
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Notify(struct MSOS_MMPool_Notify_Param *notify_param)
{
     return FALSE;	 
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Locate_Chunk(struct MSOS_MMPool_Locate_Chunk_Param * chunk_param)
{
      if(chunk_param != NULL)
           chunk_param->error_code = MSOSMMERROR_DONOT_SUPPORT;

      return FALSE;
}




