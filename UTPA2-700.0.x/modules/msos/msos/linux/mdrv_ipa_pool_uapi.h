#ifndef _MDRV_IPA_POOL_UAPI_H_
#define _MDRV_IPA_POOL_UAPI_H_

#define MAP_USAGE_APPLICATION 0x00000001//application(upper layer of utopia) will access virtual address
#define MAP_USAGE_UTOPIA 0x00000002   //utopia will access Virtual address internally
#define IPAPOOL_HEAP_NAME_MAX_LEN 32

#define IPAPOOL_NAME_MAX_LONG 128

//enum for mapping virtual address type
enum IPA_MAP_VA_TYPE
{
    IPA_MAP_VA_NO_NEEDED = 0,  //we don't need to map VA
    IPA_VA_CACHE_WRITE_BACK = 1, //we need to map VA with cache type "write back", which is normally used
    IPA_VA_CACHE_WRITE_THROUGH = 2, //we need to map VA with cache type "write through", which is //mostly used by graphic system
    IPA_VA_CACHE_NONE_CACHE = 3,  //we don't need data cache
    IPA_VA_TYPE_MAX = 0xFFFFFFFF//force enum size to be 4 byte alignment
};
//enum for IPA_HEAP_TYPE
enum IPA_HEAP_TYPE
{
    IPA_HEAP_TYPE_PA = 0,  //directly PA heap
    IPA_HEAP_TYPE_MTLB = 1, //MTLB heap
    IPA_HEAP_TYPE_CMA = 2, //CMA heap
    IPA_IPA_HEAP_TYPE_INVALID = 0xFFFFFFFF//force enum size to be 4 byte alignment
};

struct IPA_Pool_Init_Args
{
    MS_U32 heap_id;     //in: maybe shared with more than one pools which based on this heap
    MS_U8 pool_name[IPAPOOL_NAME_MAX_LONG];   //global identify name for pool to shared between multiple process (char*)

    MS_U64 offset_in_heap;    //in: pool location in heap address heap
    MS_U64 len;       //in: pool length inheap address heap

    MS_U32 map_usage;// MAP_USAGE_APPLICATION or MAP_USAGE_UTOPIA or both

    MS_U32 pool_handle_id; //out: generate pool id based on heap specified by heap id
    MS_U32 miu;  //out: miu id this heap belongs, index from 0.
    MS_U32 heap_type;//out: return heap type to application (enum IPA_HEAP_TYPE )
    MS_S32 error_code; // error code when failed

    MS_U64 heap_length; //out: heap leagth
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu, meaningless in case of MTLB heap
};

struct IPA_Pool_Deinit_Args
{
    MS_U32 pool_handle_id; //out: generate pool id based on heap specified by heap id
};

struct IPA_Pool_Alloc_Args
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_U64 length;               //in  alloc length
    MS_U32 timeout;//in :if using polling thread,will set timeout value(unit in ms),else is 0    
    MS_S32 error_code; // out: error code when failed
};

#ifdef STR_ALLOC_FREE_DEBUG//only  debug code
struct IPA_Pool_STR_Alloc_Args
{
    MS_U64 start; //in: start pos refer to bus
    MS_U64 length;               //in  alloc length
    MS_S32 error_code;
};
#endif


struct IPA_Pool_free_Args
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_U64 length;               //in  free length
};

#ifdef STR_ALLOC_FREE_DEBUG//only  debug code
struct IPA_Pool_STR_free_Args
{
    MS_U64 start; //in: start pos refer to bus
    MS_U64 length;               //in  free length
    MS_S32 error_code;
};
#endif

struct IPA_Pool_Map_Args
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_U64 length;               //in  mapping length
    MS_U32 map_va_type;//in: indicate dcache type of Virtual address mapping (enum IPA_MAP_VA_TYPE)
    MS_U64 virt_addr;          //out: if map_usage: MAP_USAGE_APPLICATION is setted & map_va_type
                                       //doesn't equal to IPA_MAP_VA_NO_NEEDED
    MS_S32 error_code; // out: error code when failed
};

struct IPA_Pool_Unmap_Args
{
    MS_U64 virt_addr; //in: the VA need to unmap
    MS_U64 length;               //in: unmap length
};

struct IPA_Pool_GetIpcHandle_Args
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U32 ipc_handle_id;  //out: returned by kernel
    MS_S32 error_code; // out: error code when failed
};

struct IPA_Pool_InstallIpcHandle_Args
{
    MS_U32 ipc_handle_id;  //in: returned by kernel, when get IPC handle
    MS_U32 pool_handle_id; //out: pool handle id
    MS_S32 error_code; // out: error code when failed
};

enum IPA_DCACHE_FLUSH_TYPE
{
    IPA_DCACHE_FLUSH,//flush dcache into DRAM
    IPA_DCACHE_INVALID,// invalid dcache lines
    IPA_DCACHE_FLUSH_INVALID// flush and invalid dcache lines
};

struct IPA_Pool_DCacheFlush_Args
{
    MS_U64 virt_addr; //in: the VA need to flush
    MS_U64 length;               //in: flush length
    MS_U32 flush_type;// in: flush type (enum IPA_DCACHE_FLUSH_TYPE)
};

struct IPA_Pool_Heap_Attr
{
    MS_U32 heap_id;     //in: maybe shared with more than one pools which based on this heap

    char   name[IPAPOOL_HEAP_NAME_MAX_LEN]; //out: heap name
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu
    MS_U64 heap_length; //out: heap leagth
    MS_U32 miu;  //out: miu id this heap belongs, index from 0.
    MS_U32 heap_type;//out: return heap type to application (enum IPA_HEAP_TYPE )
    MS_S32 error_code; // error code when failed
};

enum IPA_event_Args
{
  IPA_EVENT_CONFLICT = (1<<0),
  IPA_EVENT_NO_WAIT =(1<<1),
  IPA_EVENT_NUM
};

struct IPA_Pool_Event_Args
{
    MS_U32 pool_handle_id;//in 
    enum IPA_event_Args event;//out
    MS_U64 start;//out
    MS_U64 length;//out
};

//if PA in a pool but not allocated,this state meanless for user .
//so here state in pool means allocated,not in pool means not allocated.
enum PA_In_IPA_Pool_State
{
    PA_NOT_IN_MIU = (1<<0),//PA not in any miu
    PA_NOT_IN_HEAP =(1<<1),//PA in a miu,but not in any heap
    PA_NOT_IN_POOL =(1<<2),//PA in a heap ,but not in any pool(means not allocated)
    PA_ALLOCATED =(1<<3),//PA in a pool(means pa allocated)
    PA_CASE_NUM
};

struct PA_In_IPA_Pool_Args
{
    MS_U64 PA;//in:   PA value
    MS_S32 miu;//out:  in which miu
    
    MS_BOOL in_heap;//out:  whether in heap.If not, relative value can not get.
    MS_S32 heap_id;     //out: heap id the pa in 
    enum IPA_HEAP_TYPE heap_type;//out: return heap type to application
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu    
    MS_U64 heap_length; //out: heap leagth
    MS_U64 pa_offset_in_heap;    //out: pa location in heap
    
    MS_BOOL allocated;//out :whether already allocated,if not , relative value can not get.
    //MS_S32 pool_handle_id; //out: allocated by which pool.
    MS_U8 pool_name[IPAPOOL_NAME_MAX_LONG];//out: name of pool.
    MS_U64 pool_len;       //out: pool length in  heap 
    MS_U64 pool_offset_in_heap;    //out: pool location in heap
    MS_U64 pa_offset_in_pool;    //out: pa location in pool
    
    enum PA_In_IPA_Pool_State pa_state;//out:pa state summary
    MS_S32 error_code; // error code when failed,not used yet
};

/*N.B.
 PA:base ARM_MIU0_BUS_BASE/ARM_MIU1_BUS_BASE/ARM_MIU2_BUS_BASE 
if PA value inside [ARM_MIU0_BUS_BASE,ARM_MIU1_BUS_BASE),PA is in miu0
if PA value inside [ARM_MIU1_BUS_BASE,ARM_MIU2_BUS_BASE),PA is in miu1
......


to meet modules less use/store global variables request,
input of this API only have pool_handle_id and offset_in_pool.
*/
struct Pool_To_PA_Args
{
    MS_U32 pool_handle_id;//in : pa in which pool.
    MS_U64 offset_in_pool;//in: pa offset in pool.

    MS_U64 PA;//out:   PA value.
    MS_S32 error_code; // out: reason why failed.

    //give more out ,which nowadays no modules use,but may future use.
    MS_U32 miu;//out:  in which miu.
    MS_U32 heap_id;     //out: heap id the pa in .After get this,with the help of MApi_IPA_Pool_HEAP_ATTR,can get more more info.
};

//-------------------------------------------------------------------------------------------------
//  IO command
//-------------------------------------------------------------------------------------------------
#define IPA_POOL_IOC_MAGIC   'P'

#define IPA_POOL_IOC_INIT   _IOWR(IPA_POOL_IOC_MAGIC, 0x00, struct IPA_Pool_Init_Args)
#define IPA_POOL_IOC_ALLOC  _IOWR(IPA_POOL_IOC_MAGIC, 0x01, struct IPA_Pool_Alloc_Args)
#define IPA_POOL_IOC_FREE   _IOW(IPA_POOL_IOC_MAGIC, 0x02, struct IPA_Pool_free_Args)
#define IPA_POOL_IOC_MAP   _IOWR(IPA_POOL_IOC_MAGIC, 0x03, struct IPA_Pool_Map_Args)
#define IPA_POOL_IOC_UNMAP   _IOW(IPA_POOL_IOC_MAGIC, 0x04, struct IPA_Pool_Unmap_Args)
#define IPA_POOL_IOC_FLUSH   _IOW(IPA_POOL_IOC_MAGIC, 0x05, struct IPA_Pool_DCacheFlush_Args)
#define IPA_POOL_IOC_DEINIT   _IOW(IPA_POOL_IOC_MAGIC, 0x06, struct IPA_Pool_Deinit_Args)
#define IPA_POOL_IOC_HEAP_ATTR   _IOWR(IPA_POOL_IOC_MAGIC, 0x07, struct IPA_Pool_Heap_Attr)
#define IPA_POOL_IOC_GETIPCHANDLE _IOWR(IPA_POOL_IOC_MAGIC, 0x08, struct IPA_Pool_GetIpcHandle_Args)
#define IPA_POOL_IOC_INSTALLIPCHANDLE _IOWR(IPA_POOL_IOC_MAGIC, 0x09, struct IPA_Pool_InstallIpcHandle_Args)
#define IPA_POOL_IOC_KERNEL_MAP   _IOWR(IPA_POOL_IOC_MAGIC, 0x0A, struct IPA_Pool_Map_Args)
#define IPA_POOL_IOC_KERNEL_UNMAP   _IOW(IPA_POOL_IOC_MAGIC, 0x0B, struct IPA_Pool_Unmap_Args)
#define IPA_POOL_IOC_PA_INFO   _IOWR(IPA_POOL_IOC_MAGIC, 0x0C, struct PA_In_IPA_Pool_Args)
#define IPA_POOL_IOC_POOL_TO_PA   _IOWR(IPA_POOL_IOC_MAGIC, 0x0D, struct Pool_To_PA_Args)

#ifdef STR_ALLOC_FREE_DEBUG//only debug code 
#define IPA_POOL_IOC_STR_ALLOC _IOWR(IPA_POOL_IOC_MAGIC, 0x10, struct IPA_Pool_STR_Alloc_Args)
#define IPA_POOL_IOC_STR_FREE _IOW(IPA_POOL_IOC_MAGIC, 0x11, struct IPA_Pool_STR_free_Args)

#endif

#define IPAERROR_OK     0
#define IPAERROR_RANGE_INVLAID    1
#define IPAERROR_RANGE_NOALIGN    2
#define IPAERROR_CREATE_POOL      3
#define IPAERROR_ATTACH_POOL      4
#define IPAERROR_NO_POOL           5
#define IPAERROR_POOL_NOT_INIT    6
#define IPAERROR_ADDR_TYPE_INV    7
#define IPAERROR_CREATE_VM        8
#define IPAERROR_CANT_MAP         9
#define IPAERROR_MAP_FAIL         10
#define IPAERROR_KERNEL_NOMEM    11
#define IPAERROR_RANGE_NOTMAPPED 12
#endif
