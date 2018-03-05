
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>

#else
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include <linux/namei.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/seq_file.h>

#include "utopia_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "drvMMIO.h"
#include "utopia_driver_id.h"
#include "MsTypes.h"
#include "utopia_adp.h"
//#include "drvMIU.h"
#include "drvSYS.h"
#include "drvDMD_VD_MBX.h"
#include "utopia_dev.h"
#ifdef CONFIG_UTOPIA_GARBAGE_COLLECTION
#include <linux/sched.h>
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if 1
#define PREFIX(MODULE) \
	extern __attribute__((weak)) MS_U32 MODULE##_adp_Init(FUtopiaIOctl* pIoctl);
INCLUDED_MODULE
#undef PREFIX
#endif
//u32Ret |= MODULE##_adp_Init(((UTOPIA2K_ADP*)filp->private_data)->fpIoctl);

#define MPOOL_DIVIDE_32M_WORKAROUND_PATCH   0

typedef unsigned char               MS_NULL;
UTOPIA_AUTH_IPCHECK_ARG* gIpAuthVars = NULL;
MS_U8 gCusID[] = {0x00,0x00};
MS_U8 gCusHash[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);
void MDrv_AUTH_GetHashInfo (MS_U8 *hashinfo);
static DEFINE_MUTEX(_Msos_Mapping_Mutex);
static DEFINE_MUTEX(_MdbFpioctlList_Mutex);

#if MPOOL_DIVIDE_32M_WORKAROUND_PATCH
#define DIVIDE_SIZE     (32<<20)
static MS_BOOL _MPOOL_Divide_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache);
#endif

#ifdef CONFIG_UTOPIA_GARBAGE_COLLECTION
extern void Utopia2K_resource_collection_Register(void (*callback)(pid_t pid));
#endif
//--------------//
// Global Variable
//--------------//
UTOPIA_PID_LIST *g_Utopia_PID_root = NULL;
extern struct mutex _Utopia_Instance_Mutex;


#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT) || defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_32BIT)

typedef MS_U32 (*FUIOCTLSetModule_adp_Init)(FUtopiaIOctl* pIoctl);

MS_U32 emptyIoctlSetModule(FUtopiaIOctl* pIoctl){
    //printu("[utopia info] emptyIoctlSetModule\n" );
    return 0;
}

#define PREFIX(MODULE) \
    FUIOCTLSetModule_adp_Init MODULE##IOCTLSetModule_adp_Init = emptyIoctlSetModule;
    INCLUDED_MODULE
#undef PREFIX
    
MS_U32 autoIoctlSetModule(void)
{

#define PREFIX(MODULE) \
    if (MODULE##_adp_Init != NULL) \
    { \
        MODULE##IOCTLSetModule_adp_Init = MODULE##_adp_Init; \
    }
    INCLUDED_MODULE
#undef PREFIX


    return 0;
}

#endif

#if defined(CONFIG_COMPAT)
static long utopia_proc_ioctl_compat(struct file *filp, unsigned int cmd, void* arg)
{
	int	err= 0;
	switch(cmd){
		case UTOPIA_IOCTL_SetMODULE_COMPAT :
		{
			//printk("\033[0;32;31m [Ian] %s %d Enter\033[m\n",__func__,__LINE__);
			compat_uint_t i;
			compat_uptr_t p;
			UTOPIA_DDI_OPEN_ARG_32 __user *data32;
			UTOPIA_DDI_OPEN_ARG __user *data;
			data = compat_alloc_user_space(sizeof(*data));
			if (data == NULL)
				return -EFAULT;
			data32 = compat_ptr(arg);
			err = get_user(i, &data32->u32ModuleID);
			err |= put_user(i, &data->u32ModuleID);
			err |= get_user(i, &data32->u32ModuleVersion);
			err |= put_user(i, &data->u32ModuleVersion);
			err |= get_user(p, &data32->pAttribute);
			err |= put_user(compat_ptr(p), &data->pAttribute);
			if (err)
				return err;
			//printk("\033[0;32;31m [Ian] %s %d Exit\033[m\n",__func__,__LINE__);
			return filp->f_op->unlocked_ioctl(filp, UTOPIA_IOCTL_SetMODULE,data);
		}
		case UTOPIA_IOCTL_IoctlMODULE_COMPAT : 
		{
			//printk("\033[0;32;31m [Ian] %s %d Enter\033[m\n",__func__,__LINE__);
			compat_uint_t i;
			compat_uptr_t p;
			UTOPIA_DDI_IOCTL_ARG_32 __user *data32;
			UTOPIA_DDI_IOCTL_ARG __user *data;
			data = compat_alloc_user_space(sizeof(*data));
			if (data == NULL)
				return -EFAULT;
			data32 = compat_ptr(arg);
			err = get_user(i, &data32->u32Cmd);
			err |= put_user(i, &data->u32Cmd);
			err |= get_user(p, &data32->pArg);
			err |= put_user(compat_ptr(p), &data->pArg);
			if (err)
				return err;
			//printk("\033[0;32;31m [Ian] %s %d Exit\033[m\n",__func__,__LINE__);
			return filp->f_op->unlocked_ioctl(filp, UTOPIA_IOCTL_IoctlMODULE,data);
		}

		case UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING_COMPAT : 
		{
			//printk("\033[0;32;31m [Ian] %s %d Enter\033[m\n",__func__,__LINE__);
			compat_uint_t i;
			compat_uptr_t p;
			KDrvMPool_Info_t_ARG_32 __user *data32;
			KDrvMPool_Info_t __user *data;
			data = compat_alloc_user_space(sizeof(*data));
			if (data == NULL)
				return -EFAULT;
			data32 = compat_ptr(arg);
			err = get_user(i, &data32->u32Addr);
			err |= put_user(i, &data->u32Addr);
			err |= get_user(i, &data32->u32Size);
			err |= put_user(i, &data->u32Size);
			err |= get_user(i, &data32->u32Interval);
			err |= put_user(i, &data->u32Interval);
			err |= get_user(i, &data32->u8MiuSel);
			err |= put_user(i, &data->u8MiuSel);
			err |= get_user(i, &data32->bcached);
			err |= put_user(i, &data->bcached);

			//err |= put_user(compat_ptr(p), &data->pArg);
			if (err)
				return err;
			//printk("\033[0;32;31m [Ian] %s %d Exit\033[m\n",__func__,__LINE__);
			return filp->f_op->unlocked_ioctl(filp, UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING,data);
		}
		default: //Other case
			return filp->f_op->unlocked_ioctl(filp, cmd,(unsigned long)compat_ptr(arg));
	}
}
#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
volatile  void *tmpFP = VDEC_EX_adp_Init;
static long utopia_proc_ioctl(struct file *filp, unsigned int cmd, void* arg)
{
   //UADPBypassIoctl(struct file *filp, unsigned int cmd, void* arg,UADP_STRUCT_POINTER_TABLE *pInTable, UADP_STRUCT_POINTER_TABLE *pOutTable);
    UTOPIA_DDI_OPEN_ARG sOpenArg;
    UTOPIA_DDI_IOCTL_ARG sIoctlArg;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    MS_U32 ret = UTOPIA_STATUS_FAIL;
    KDrvMPool_Info_t i;
    UTOPIA_AUTH_IPCHECK_ARG sIPCheckArg;
    UTOPIA_AUTH_HASHINFO_ARG sHashInfoArg;

    switch(cmd){
    case UTOPIA_IOCTL_SetMODULE :
		copy_from_user(&sOpenArg,arg,sizeof(UTOPIA_DDI_OPEN_ARG));

		switch(sOpenArg.u32ModuleID){
#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT) || defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_32BIT)

        #define PREFIX(MODULE) \
		case MODULE_##MODULE : \
				if (MODULE##_adp_Init != NULL) \
				{ \
					printk("utopia_proc_ioctl: setModule : %d  \n", MODULE_##MODULE); \
                        u32Ret = (*(MODULE##IOCTLSetModule_adp_Init))(&(((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl)); \
				}else  \
				{ \
				    printk("utopia_proc_ioctl: setModule error : %d  \n", MODULE_##MODULE); \
				    u32Ret = UTOPIA_STATUS_FAIL; \
				    break; \
				} \
                u32Ret = UtopiaOpen(sOpenArg.u32ModuleID,&(((UTOPIA2K_ADP*)(filp->private_data))->pInstant),sOpenArg.u32ModuleVersion,sOpenArg.pAttribute); \
		        break;
        INCLUDED_MODULE
        #undef PREFIX

#else

        #define PREFIX(MODULE) \
		case MODULE_##MODULE : \
				if (MODULE##_adp_Init != NULL) \
				{ \
					printk("utopia_proc_ioctl: setModule : %d  \n", MODULE_##MODULE); \
					u32Ret |= MODULE##_adp_Init(&(((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl)); \
				}else  \
				{ \
				    printk("utopia_proc_ioctl: setModule error : %d  \n", MODULE_##MODULE); \
				    u32Ret = UTOPIA_STATUS_FAIL; \
				    break; \
				} \
                u32Ret = UtopiaOpen(sOpenArg.u32ModuleID,&(((UTOPIA2K_ADP*)(filp->private_data))->pInstant),sOpenArg.u32ModuleVersion,sOpenArg.pAttribute); \
		        break;
        INCLUDED_MODULE
        #undef PREFIX

#endif //CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT
		default:
		        printk("utopia_proc_ioctl: setModule error, no this module: %ld  \n",sOpenArg.u32ModuleID);
			    break;
			}
		break;
	case UTOPIA_IOCTL_IoctlMODULE:
		copy_from_user(&sIoctlArg,arg,sizeof(UTOPIA_DDI_IOCTL_ARG));
		u32Ret = ((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl(((UTOPIA2K_ADP*)(filp->private_data))->pInstant,sIoctlArg.u32Cmd,sIoctlArg.pArg);
        //u32Ret = ((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl(((UTOPIA2K_ADP*)(filp->private_data))->pInstant,cmd,arg);
        break;

	case UTOPIA_IOCTL_CloseMODULE:
		//copy_from_user(&sIoctlArg,arg,sizeof(UTOPIA_DDI_IOCTL_ARG));
		u32Ret = UtopiaClose((((UTOPIA2K_ADP*)(filp->private_data))->pInstant));
		break;


    case UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING:

        mutex_lock(&_Msos_Mapping_Mutex);

        if(copy_from_user(&i, (void __user *)arg, sizeof(i)))
        {
            u32Ret = UTOPIA_STATUS_FAIL;
			mutex_unlock(&_Msos_Mapping_Mutex);
			break;
        }

#if MPOOL_DIVIDE_32M_WORKAROUND_PATCH
		if(_MPOOL_Divide_Mapping(i.u8MiuSel,i.u32Addr,i.u32Size,!i.bcached))
		{
		    u32Ret = UTOPIA_STATUS_SUCCESS;
		}else
		{
		    u32Ret = UTOPIA_STATUS_FAIL;
		}
#else
		if(MsOS_MPool_Mapping_Dynamic(i.u8MiuSel,i.u32Addr,i.u32Size,!i.bcached))
		{
		    u32Ret = UTOPIA_STATUS_SUCCESS;
		}else
		{
		    u32Ret = UTOPIA_STATUS_FAIL;
		}
#endif


        //u32Ret = MsOS_MPool_Mapping_Dynamic(i.u8MiuSel,i.u32Addr,i.u32Size,!i.bcached);
        mutex_unlock(&_Msos_Mapping_Mutex);

        break;

    case UTOPIA_IOCTL_AUTH_SetIPCheck:
        copy_from_user(&sIPCheckArg,arg,sizeof(UTOPIA_AUTH_IPCHECK_ARG));
        if(gIpAuthVars==NULL)
        {
            gIpAuthVars=malloc(sizeof(UTOPIA_AUTH_IPCHECK_ARG));
        }
        memcpy(gIpAuthVars, &sIPCheckArg, sizeof(UTOPIA_AUTH_IPCHECK_ARG));
        break;
    case UTOPIA_IOCTL_AUTH_SetHashInfo:
        copy_from_user(&sHashInfoArg,arg,sizeof(UTOPIA_AUTH_HASHINFO_ARG));
        memcpy(gCusID, sHashInfoArg.u8CusID, sizeof(gCusID));
        memcpy(gCusHash, sHashInfoArg.u8CusHash, sizeof(gCusHash));
        break;

	default:
		printk("utopia_proc_ioctl error :default \n");
        //u32Ret = ((UTOPIA2K_ADP*)(filp->private_data))->fpIoctl(((UTOPIA2K_ADP*)(filp->private_data))->pInstant,cmd,arg);
        break;

	}

	return u32Ret;

}

MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit)
{
    MS_U8 rval=0;
    MS_U8 i=0;

    if(gIpAuthVars==NULL)
    {
        printk("Hash key AUTH not set!\n");
        return 0;
    }
    rval=gIpAuthVars->u8IpControl[31-u8Bit/8] & (1<<(u8Bit%8));
    //printk("MDrv_AUTH_IPCheck, retval: %d\n", rval);
    return rval;
}

void MDrv_AUTH_GetHashInfo (MS_U8 *hashinfo)
{
    int i;
    hashinfo[0] = gCusID[0];
    hashinfo[1] = gCusID[1];

    for(i=0;i<16;i++)
    {
        hashinfo[i+2] = gCusHash[i];
    }
}

#if MPOOL_DIVIDE_32M_WORKAROUND_PATCH
static MS_BOOL _MPOOL_Divide_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache)
{
    MS_U32 u32MapAddr, u32RemainSize;

    u32MapAddr = u32Offset;
    u32RemainSize = u32MapSize;

    while(u32RemainSize > DIVIDE_SIZE)
    {
        if (MsOS_MPool_Mapping_Dynamic(u8MiuSel, u32MapAddr, DIVIDE_SIZE, bNonCache))
        {
            u32MapAddr += DIVIDE_SIZE;
            u32RemainSize -= DIVIDE_SIZE;
        }
        else
        {
            return FALSE;
        }
    }

    return MsOS_MPool_Mapping_Dynamic(u8MiuSel, u32MapAddr, u32RemainSize, bNonCache);
}
#endif

static ssize_t utopia_proc_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
    return 0;
}

static int utopia_proc_open(struct inode *inode, struct file *file)
{
	UTOPIA2K_ADP *Tmp;

    Tmp = kzalloc(sizeof(UTOPIA2K_ADP), GFP_KERNEL);
	if (Tmp == NULL)
		return -ENOMEM;

	memset(Tmp,0,sizeof(UTOPIA2K_ADP));

	file->private_data = Tmp;
	return 0;
}

static int utopia_proc_release(struct inode *inode, struct file * file)
{
	 UTOPIA2K_ADP *mmapData = file->private_data ;

	 //file->private_data->fpClose((UTOPIA2K_DDI*)file->private_data);
	 //UtopiaClose((((UTOPIA2K_ADP*)(file->private_data))->pInstant));
	 kfree(mmapData);

	 // iounmap(dev->dmaBuf) ;
	 return 0;
}

#ifdef CONFIG_UTOPIA_GARBAGE_COLLECTION
static void utopia_garbage_collection(pid_t pid)
{
    // 1. Trace PID root to search the pid which need to be garbage_collection
    MS_BOOL bFind = FALSE;
    UTOPIA_PID_LIST *CurrNode;
    int count;

    //if pid = tgid => process do_exit
    if (pid != current->tgid)
        return;

    //UTOPIA_PID_LIST could be modified by multi-process, so it need mutex to protect.
    //But, CurrNode->instance_list would not be changed, because this process is killed.
    //  so, CurrNode->instance_list does not need protect during garbage_collection.
    mutex_lock(&_Utopia_Instance_Mutex);
    CurrNode = g_Utopia_PID_root;

    while(CurrNode)
    {
        if (pid == CurrNode->pid)
        {
            bFind = TRUE;
            break;
        }
        CurrNode = CurrNode->pNext;
    }
    mutex_unlock(&_Utopia_Instance_Mutex);

    if (!bFind) return; //No need to do garbage_collection.

    // 2. Do Garbage_Collection
    for (count = 0; count < INSTANCE_MAX; count++)
    {
        void* pModule;
        void* pResource;
        MS_U32 u32ResourceStatus = UTOPIA_STATUS_SUCCESS;

        if (NULL == CurrNode->instance_list[count])
            continue;

        // 3. Send ioctl cmd.
        if (UTOPIA_STATUS_SIGNAL_OK != UtopiaIoctl(CurrNode->instance_list[count], CMD_USIGNAL_PROCESS_KILL, &pid))
            //continue;
        {
            // 4. Check Resource.
            //Get Module Pointer
            UtopiaInstanceGetModule(CurrNode->instance_list[count], &pModule);
            //Get Module Resource(s)
            pResource = NULL;
            while(UTOPIA_STATUS_SUCCESS == u32ResourceStatus)
            {
                //Travel Each Resource of this Module.
                u32ResourceStatus = UtopiaResourceGetNext(pModule, &pResource);

                //Check Resource pid.
                if (pResource && (UTOPIA_STATUS_SUCCESS == u32ResourceStatus))
                {
                    if (pid == UtopiaResourceGetPid(pResource))
                    {
                        //Recycle resource.
                        UtopiaResourceRelease(pResource);
                    }
                }
            }
        }

        // 5. Close
        UtopiaClose(CurrNode->instance_list[count]);
    }

    return;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions MPool Info
//-------------------------------------------------------------------------------------------------
static ssize_t mpoolinfo_proc_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    printk("===== MPool Info =====\n");
    MsOS_MPool_InfoMsg();
    printk("======================\n");
    return 0;
}
static int mpoolinfo_proc_open(struct inode *inode, struct file *file)
{
    return 0;
}
static int mpoolinfo_proc_release(struct inode *inode, struct file * file)
{
    return 0;
}


const struct file_operations proc_utopia_operations = {
	//.read       = utopia_read,
	.write      = utopia_proc_write,
	.open       = utopia_proc_open,
	.release    = utopia_proc_release,
    .unlocked_ioctl  = (void*)utopia_proc_ioctl,
#if defined(CONFIG_COMPAT)
	.compat_ioctl = utopia_proc_ioctl_compat,
#endif
};

const struct file_operations proc_mpoolinfo_operations = {
    .read       = mpoolinfo_proc_read,
    .open       = mpoolinfo_proc_open,
    .release    = mpoolinfo_proc_release,
};

extern char moduleNames[][40];

#if CONFIG_MSTAR_UTOPIA2K_STR
static int serch_moduleID(char* sModuleName)
{
    int u32cnt=0;
	int u32ret=0;
	for(u32cnt = 0;u32cnt<eMODULE_END;u32cnt++)
    {
        if (0== strcmp((const char*)sModuleName, (const char*)moduleNames[u32cnt]))
	    {
	        u32ret = u32cnt;
			break;		
	    }
				
	}

    return u32ret;
}

extern int mdrv_utopia2k_str_module_selftest(void* pModuleTmp,char* sModuleName);

static ssize_t STR_verify_proc_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
    //printk("Jway...STR_verify_proc_write ..%z\n",count);
    char sModuleName[40] = {0};
	int u32ModuleID = 0;
	void* pInstanceTemp = NULL;
	void* pModuleTemp = NULL;

	if(count > 40)
	{
		printk("utopia_dev.c : str_verify input too long\n");
		return 0;
	}
	copy_from_user(sModuleName,buf,count);
	
	u32ModuleID = serch_moduleID(sModuleName);
	
	//printk("STR_verify_proc_write : name = %s ID = %d \n",sModuleName,u32ModuleID);
	if(u32ModuleID == 0)
	{
	   printk("utopia_dev.c : str_verify input module name error\n");
	   return 0;
	}
	
	if(UtopiaOpen(u32ModuleID,&pInstanceTemp,NULL,NULL)!=  UTOPIA_STATUS_SUCCESS)
	{
	    printk("utopia_dev.c : UtopiaOpen fail , Module %s \n");
		return 0;
	}

    if(UtopiaInstanceGetModule(pInstanceTemp,&pModuleTemp)!=  UTOPIA_STATUS_SUCCESS)
    {
        printk("utopia_dev.c : UtopiaInstanceGetModule fail , Module %s \n");
		UtopiaClose(pInstanceTemp);
		return 0;
    }

	//printk("STR_verify_proc_write : UtopiaInstanceGetModule = %p \n",pModuleTemp);

	mdrv_utopia2k_str_module_selftest(pModuleTemp,sModuleName);
	UtopiaClose(pInstanceTemp);
    return count;
}

ssize_t STR_verify_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int u32cnt = 0;

    printk("\n\n=======You can type the following modules:========\n\n");

    for(u32cnt = 0; u32cnt < eMODULE_END; u32cnt++)
    {
        printk("%-20s", (const char*)moduleNames[u32cnt]);
        if ((u32cnt % 5 == 4) || (u32cnt == eMODULE_END - 1))
            printk("\n");
    }
    return 0;
}

static int STR_verify_proc_open(struct inode *inode, struct file *file)
{
    return 0;
}
static int STR_verify_proc_release(struct inode *inode, struct file * file)
{
    return 0;
}


const struct file_operations proc_str_operations = {
    .write      = STR_verify_proc_write,
    .read       = STR_verify_proc_read,
    .open       = STR_verify_proc_open,
    .release    = STR_verify_proc_release,
};

#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
static MS_BOOL bAgentInit = FALSE;
struct mdb_fpioctl_list{
    char iname[MDB_NODE_NAME_MAX];
    FUtopiaMdbIoctl fpMdbIoctl;
    MS_BOOL used;
};
static struct mdb_fpioctl_list _mdb_fpioctl_list[MDB_FPIOCTL_LIST_MAX];

static ssize_t mdb_node_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char tmpbuf[MDB_CMDLINE_MAX];
    MDBCMD_CMDLINE_PARAMETER paraCmdLine;
    int i=0;
    if(buf && !copy_from_user(tmpbuf, buf, count))
    {
        tmpbuf[count-1] = '\0';
    }
    for(i=0; i<MDB_FPIOCTL_LIST_MAX; i++)
    {
        if(TRUE==_mdb_fpioctl_list[i].used && 0==strncmp(_mdb_fpioctl_list[i].iname,(file->f_path.dentry->d_iname),strlen((file->f_path.dentry->d_iname))))
        {
            paraCmdLine.u64ReqHdl = file;
            paraCmdLine.u32CmdSize = count-1;
            memcpy(paraCmdLine.pcCmdLine, tmpbuf, MDB_CMDLINE_MAX);
            _mdb_fpioctl_list[i].fpMdbIoctl(MDBCMD_CMDLINE,&paraCmdLine);
            break;
        }
    }
    return count;
}

static ssize_t mdb_node_proc_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    MDBCMD_GETINFO_PARAMETER paraGetInfo;
    int i=0;
    for(i=0; i<MDB_FPIOCTL_LIST_MAX; i++)
    {
        if(TRUE==_mdb_fpioctl_list[i].used && 0==strncmp(_mdb_fpioctl_list[i].iname,(file->f_path.dentry->d_iname),strlen((file->f_path.dentry->d_iname))))
        {
            paraGetInfo.u64ReqHdl = file;
            _mdb_fpioctl_list[i].fpMdbIoctl(MDBCMD_GETINFO,&paraGetInfo);
            break;
        }
    }
    return 0;
}

struct file_operations proc_mdb_node_operations = {
    .write      = mdb_node_proc_write,
    .read       = mdb_node_proc_read,
};

static ssize_t mdb_agent_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    return count;
}

static ssize_t mdb_agent_proc_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    return 0;
}

const struct file_operations proc_mdb_agent_operations = {
    .write      = mdb_agent_proc_write,
    .read       = mdb_agent_proc_read,
};

void mdb_node_create_table(char sMdbNodeName[MDB_NODE_NAME_MAX], FUtopiaMdbIoctl fpMdbIoctl)
{
    char sMdbNodeNameTmp[MDB_NODE_NAME_MAX*2];
    int i=0;
    sprintf(sMdbNodeNameTmp,"utopia_mdb/%s",sMdbNodeName);
    mutex_lock(&_MdbFpioctlList_Mutex);
    for(i=0; i<MDB_FPIOCTL_LIST_MAX; i++)
    {
        if(FALSE==_mdb_fpioctl_list[i].used)
        {
            memcpy(_mdb_fpioctl_list[i].iname,sMdbNodeName,strlen(sMdbNodeName));
            _mdb_fpioctl_list[i].fpMdbIoctl = fpMdbIoctl;
            _mdb_fpioctl_list[i].used = TRUE;
            break;
        }
    }
    mutex_unlock(&_MdbFpioctlList_Mutex);
    return 0;
}

void mdb_node_create(void)
{
    char sMdbNodeNameTmp[MDB_NODE_NAME_MAX*2];
    int i;

    for(i=0; i<MDB_FPIOCTL_LIST_MAX; i++)
    {
        if(TRUE==_mdb_fpioctl_list[i].used)
        {
            sprintf(sMdbNodeNameTmp,"utopia_mdb/%s",_mdb_fpioctl_list[i].iname);
            proc_create(sMdbNodeNameTmp, S_IRUSR | S_IWUSR, NULL, &proc_mdb_node_operations);
            printk("--------%s\n", _mdb_fpioctl_list[i].iname);
        }
    }
    bAgentInit = TRUE;
    return 0;
}
#endif

static int __init init_procfs_msg(void)
{
    int i=0;
    struct path path;
    int error;
	proc_create("utopia", S_IRUSR | S_IWUSR, NULL, &proc_utopia_operations);

    //For utopia2k mpool info msg.
    proc_mkdir("utopia2k", NULL);
    proc_create("utopia2k/mpoolinfo", S_IRUSR | S_IWUSR, NULL, &proc_mpoolinfo_operations);

	#if CONFIG_MSTAR_UTOPIA2K_STR
	proc_create("utopia2k/str_verify", S_IRUSR | S_IWUSR, NULL, &proc_str_operations);
	#endif
    #ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    //create /proc/utopia_mdb (directory) and /proc/utopia_mdb/agent (node)
    error=kern_path("/proc/utopia_mdb",LOOKUP_FOLLOW, &path);
    if(error)
    {
        proc_mkdir("utopia_mdb", NULL);
    }
    proc_create("utopia_mdb/agent", S_IRUSR | S_IWUSR, NULL, &proc_mdb_agent_operations);
    //initial mdb_fpioctl_list
    for(i=0; i<MDB_FPIOCTL_LIST_MAX; i++)
    {
        memset(_mdb_fpioctl_list[i].iname,0,sizeof(_mdb_fpioctl_list[i].iname));
        _mdb_fpioctl_list[i].fpMdbIoctl = NULL;
        _mdb_fpioctl_list[i].used = FALSE;
    }
    #endif
	return 0;
}


#define MAJOR_NUM          99
#define MODULE_NAME                "UTOPIA"

void* pInstant = NULL;
void* pArg = NULL;


UADP_SPT_0NXT_DEF(MS_U8);
UADP_SPT_0NXT_DEF(MS_U16);
UADP_SPT_0NXT_DEF(MS_U32);
UADP_SPT_0NXT_DEF(MS_NULL);

static int __init utopia_init(void)
{

	init_procfs_msg();

    #if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT) || defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_32BIT)
    autoIoctlSetModule();
	#endif

#if 0
    if (register_chrdev(MAJOR_NUM, "utopia", &proc_utopia_operations) < 0)
	{
      printk("<1>%s: can't get major %d\n", MODULE_NAME, MAJOR_NUM);
      return (-EBUSY);
    }
    printk("<1>%s: started\n", MODULE_NAME);
    // return 0;
#endif
	MsOS_MPool_Init();
	MsOS_Init();
	MDrv_SYS_Init();
	MDrv_SYS_DMD_VD_MBX_Init();
	//MDrv_MIU_Init();
#if 0   //test code.
    _MPOOL_Divide_Mapping(0, 0x1F000000, (300<<20), TRUE);
#endif

        #ifdef CONFIG_MSTAR_UTOPIA2K_STR
		printk("utopia support STR\n");
	//mstar_set_utopia2k_cbf_pm(UtopiaSave_PM, UtopiaRestore_PM);
        #endif

    g_Utopia_PID_root = NULL;
#ifdef CONFIG_UTOPIA_GARBAGE_COLLECTION
    Utopia2K_resource_collection_Register(utopia_garbage_collection);
#endif

	UADP_SPT_0NXT(MS_U8);
	UADP_SPT_0NXT(MS_U16);
	UADP_SPT_0NXT(MS_U32);	
	UADP_SPT_0NXT(MS_NULL);	
	
	return 0;
}

MS_BOOL  MApi_MBX_Enable(MS_BOOL bEnable)
{
	return 0;
}

MS_BOOL  MApi_MBX_Init(MS_U32 eHKCPU, MS_U32 eHostRole, MS_U32 u32TimeoutMillSecs)
{ 
	return 0;
}

EXPORT_SYMBOL(MApi_MBX_Enable);
EXPORT_SYMBOL(MApi_MBX_Init);


EXPORT_SYMBOL(spt_MS_U8);
EXPORT_SYMBOL(spt_MS_U16);
EXPORT_SYMBOL(spt_MS_U32);
EXPORT_SYMBOL(spt_MS_NULL);

EXPORT_SYMBOL(MDrv_AUTH_IPCheck);
EXPORT_SYMBOL(MDrv_AUTH_GetHashInfo);

static void utopia_exit(void) {
   //unregister_chrdev(MAJOR_NUM, "utopia");
   //printk("<1>%s: removed\n", MODULE_NAME);
}

module_init(utopia_init);
module_exit(utopia_exit);

