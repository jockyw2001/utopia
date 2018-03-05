#ifndef MSOS_TYPE_LINUX_KERNEL
#include "stdlib.h"
#endif
#include "MsTypes.h"

#ifndef _UTOPIA_H_
#define _UTOPIA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "utopia_driver_id.h"

/*******************************************************************************
Status define rule:
0xMMRRRRSS  //M:Module, R:Reserve(please keep 0), S:Status code
MM = 0 for common status.
MM = 1 for Utopia Framwork and Utopia module

Will support GetErrorStr(Status, *str);
*******************************************************************************/
/*! \defgroup UTPA UTOPIA CORE Interface

  <b> Features </b>
  -# Move driver gloable variable to Instant / Module / Resource layer.
  -# Unify call entry to ioctl()
  -# trace support
  -# debug support
  -# multi-process with Share memory method (SHM)
  -# multi-process interrupt handle
  -# Polimorphism
  -# Version control
  -# STR/STD solution

  \image html utopia2_pic1.png
  \image html utopia2_pic2.png

  We allow each driver to put their private data struce into each layer, like following:
  -# pInstant->private
  -# pModule->private
  -# pModuleShm private as resource been put into Resource
Ex: treat device interrupt status a resource, please see multi-process interrupt for detail
  -# pResource->private

  \image html utopia2_pic3.png

  <b>Operation Flow</b>
  -# UtopiaInit();
  -# UtopiaOpen(MS_U32 u32ModuleID, void** pInstant, MS_U32 u32ModuleVersion, void* pvAttribute);
  -# UtopiaIoctl(void* pInstant, MS_U32 u32Cmd, MS_U32* pu32Args);
  -# UtopiaClose(void* pInstant);

 * */


#define UTOPIA_STATUS_SUCCESS               0x00000000
#define UTOPIA_STATUS_FAIL                  0x00000001
#define UTOPIA_STATUS_NOT_SUPPORTED         0x00000002
#define UTOPIA_STATUS_PARAMETER_ERROR       0x00000003
#define UTOPIA_STATUS_DONT_CARE             0x00000004
#define UTOPIA_STATUS_ERR_SYS               0x00000005       ///< operating system call failure
#define UTOPIA_STATUS_ERR_INV               0x00000006       ///< invalid argument passed
#define UTOPIA_STATUS_ERR_TIMEDOUT          0x00000007       ///< timeout occured
#define UTOPIA_STATUS_ERR_DEFERRED          0x00000008       ///< action has been deferred
#define UTOPIA_STATUS_ERR_UNIMPLEM          0x00000009       ///< Unimplemented service
#define UTOPIA_STATUS_ERR_LOCKED            0x00000010       ///< object locked
#define UTOPIA_STATUS_ERR_DELETED           0x00000011       ///< Object has been deleted
#define UTOPIA_STATUS_ERR_HEAP              0x00000012       ///< Heap full
#define UTOPIA_STATUS_ERR_UNINIT            0x00000013       ///< module is not initialised
#define UTOPIA_STATUS_ERR_UNIQUE            0x00000014       ///< unique id needed
#define UTOPIA_STATUS_ERR_STATE             0x00000015       ///< invalid state to invoke function
#define UTOPIA_STATUS_ERR_DEV               0x00000016       ///< no valid device exists
#define UTOPIA_STATUS_ERR_NOT_FOUND         0x00000017       ///< Not found
#define UTOPIA_STATUS_ERR_EOL               0x00000018       ///< a list traversal has ended
#define UTOPIA_STATUS_ERR_TERM              0x00000019       ///< module has terminated
#define UTOPIA_STATUS_ERR_LIMIT             0x00000020       ///< System limit was reached.
#define UTOPIA_STATUS_ERR_RESOURCE          0x00000021       ///< insufficient resources for request
#define UTOPIA_STATUS_ERR_NOT_AVAIL         0x00000022       ///< resources not available
#define UTOPIA_STATUS_ERR_BAD_DATA          0x00000023       ///< Bad data in transport stream
#define UTOPIA_STATUS_ERR_INLINE_SUCCESS    0x00000024
#define UTOPIA_STATUS_ERR_DEFERRED_SUCCESS  0x00000025
#define UTOPIA_STATUS_ERR_IN_USE            0x00000026       ///< Object is in use.
#define UTOPIA_STATUS_ERR_INTERMEDIATE      0x00000027       ///< Intermediate return 0x0000000 successful but not complete
#define UTOPIA_STATUS_ERR_OVERRUN           0x00000028       ///< Buffer overrun occured (e.g. in filters).
#define UTOPIA_STATUS_ERR_ABORT             0x00000029       ///< Request not supported.
#define UTOPIA_STATUS_ERR_NOTIMPLEMENTED    0x00000030       ///< Request not implemented.
#define UTOPIA_STATUS_ERR_INVALID_HANDLE    0x00000031
#define UTOPIA_STATUS_ERR_NOMEM             0x00000032       ///< No Memory
#define UTOPIA_STATUS_SHM_EXIST             0x00000033       ///< target share memory already exists
#define UTOPIA_STATUS_RPOOL_ESTABLISHED     0x00000034       ///< target resource pool is already established
#define UTOPIA_STATUS_NO_RESOURCE           0x00000035       ///< try to obtain resource but fail
#define UTOPIA_STATUS_SIGNAL_OK             0x00000036
#define UTOPIA_STATUS_ERR_USER              0x00000099       ///< start of subsystem specific errors

#define MS_MODULE_NULL 0
//#define MS_MODULE_UTOPIA 0xFFFF0000
#define MS_UTOPIA_DB_LEVEL_TRACE 0x80000000

#define utopia_malloc malloc
#define utopia_free free

#if defined(MSOS_TYPE_LINUX_KERNEL)
#define free kfree
#define malloc(size) kmalloc((size), GFP_KERNEL)
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 MdbPrint(MS_U64* u64ReqHdl, const char* fmt, ...);
#endif

#define KERNEL_MODE 0x80000000

#define UTOPIA_IOC_MAGIC  'U'
#define UTOPIA_IOCTL_SetMODULE   _IOWR(UTOPIA_IOC_MAGIC, 1, UTOPIA_DDI_OPEN_ARG)
#define UTOPIA_IOCTL_OpenMODULE  _IOWR(UTOPIA_IOC_MAGIC, 2, int)
#define UTOPIA_IOCTL_IoctlMODULE  _IOWR(UTOPIA_IOC_MAGIC, 3, UTOPIA_DDI_IOCTL_ARG)
#define UTOPIA_IOCTL_CloseMODULE  _IOWR(UTOPIA_IOC_MAGIC, 4, int)

#define UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING  _IOWR(UTOPIA_IOC_MAGIC, 5,KDrvMPool_Info_t)
#define UTOPIA_IOCTL_AUTH_SetIPCheck  _IOWR(UTOPIA_IOC_MAGIC, 6, UTOPIA_AUTH_IPCHECK_ARG)
#define UTOPIA_IOCTL_AUTH_SetHashInfo  _IOWR(UTOPIA_IOC_MAGIC, 7, UTOPIA_AUTH_HASHINFO_ARG)

#if defined(CONFIG_COMPAT)
#define UTOPIA_IOCTL_SetMODULE_COMPAT   _IOWR(UTOPIA_IOC_MAGIC, 1, UTOPIA_DDI_OPEN_ARG_32)
#define UTOPIA_IOCTL_IoctlMODULE_COMPAT  _IOWR(UTOPIA_IOC_MAGIC, 3, UTOPIA_DDI_IOCTL_ARG_32)
#define UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING_COMPAT  _IOWR(UTOPIA_IOC_MAGIC, 5,KDrvMPool_Info_t)

#endif

//-------------------------------------------------------------------------------------------------
/// UTOPIA core driver initialize 
/// @ingroup UTPA
/// @return UTOPIA_STATUS_SUCCESS - Success
/// @return UTOPIA_STATUS_FAIL - Failure
//-------------------------------------------------------------------------------------------------
MS_U32 UtopiaInit(void);

#ifdef CONFIG_MBOOT
MS_U32 UtopiaInit_str(void);
#endif

//-------------------------------------------------------------------------------------------------
/// UTOPIA core driver open by Module ID
/// @ingroup UTPA
/// @param  u32ModuleID                \b IN: Module ID
/// @param  pInstant                   \b IN: Instant from Open
/// @param  u32ModuleVersion           \b IN: The Module Version if needed 
/// @param  pAttribte                  \b IN: Module Attrubite
/// @return UTOPIA_STATUS_SUCCESS - Success
/// @return UTOPIA_STATUS_FAIL - Failure
//-------------------------------------------------------------------------------------------------
MS_U32 UtopiaOpen(MS_U32 u32ModuleID, void** pInstant, MS_U32 u32ModuleVersion, 
		const void* const pAttribte);
    
//-------------------------------------------------------------------------------------------------
/// UTOPIA core driver all operation control by Ioctl
/// @ingroup UTPA
/// @param  pInstant                   \b IN: Instant that refer to a Module 
/// @param  u32Cmd                     \b IN: The module command 
/// @param  pArgs                      \b IN: Command arguments
/// @return UTOPIA_STATUS_SUCCESS - Success
/// @return UTOPIA_STATUS_FAIL - Failure
//-------------------------------------------------------------------------------------------------
MS_U32 UtopiaIoctl(void* pInstant, MS_U32 u32Cmd, void* const pArgs);

//-------------------------------------------------------------------------------------------------
/// UTOPIA core driver close
/// @ingroup UTPA
/// @param  pInstant                   \b IN: Instant for close
/// @return UTOPIA_STATUS_SUCCESS - Success
/// @return UTOPIA_STATUS_FAIL - Failure
//-------------------------------------------------------------------------------------------------
MS_U32 UtopiaClose(void* pInstant);

#ifdef __cplusplus
}
#endif

#endif
