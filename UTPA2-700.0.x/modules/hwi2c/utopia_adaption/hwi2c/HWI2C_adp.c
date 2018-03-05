#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/uaccess.h>
#include <linux/compat.h>
#include "MsTypes.h"
#include "utopia.h"
#include "drvHWI2C.h"
#include "drvHWI2C_v2.h"
#include "HWI2C_adp.h"
#include "utopia_adp.h"
#include "drvHWI2C_private.h"

// no pointer member
//UADP_SPT_0NXT_DEF(MS_U8);
//UADP_SPT_0NXT_DEF(HWI2C_Status);
extern void* spt_MS_U8;

//#define CONFIG_COMPAT
#define MSOS_TYPE_LINUX_KERNEL
//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------

#ifdef CONFIG_COMPAT
typedef struct DLL_PACKED
{
   MS_U16 u16SlaveCfg;
   MS_U32 uAddrCnt;
   compat_uptr_t pRegAddr;
   MS_U32 uSize;
   compat_uptr_t pData;
   MS_BOOL bReturn;
} _compat_stHWI2C_PRIVATE_PARAM_WriteBytes;

typedef struct DLL_PACKED
{
   MS_U16 u16SlaveCfg;
   MS_U32 uAddrCnt;
   compat_uptr_t pRegAddr;
   MS_U32 uSize;
   compat_uptr_t pData;
   MS_BOOL bReturn;
} _compat_stHWI2C_PRIVATE_PARAM_ReadBytes;
#endif


//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                                    if(copy_from_user(a,b,c) != 0) { break; }
#define CPY_to_USER(a,b,c)                                      if(copy_to_user(a,b,c) != 0) { break; }
#else
#define CPY_FROM_USER                                           memcpy
#define CPY_to_USER                                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                                           compat_ptr(a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)compat_ptr((unsigned long)a)))
#define IS_CMP_TASK()                                             is_compat_task()
 
#define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
#define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }

#else 
#define COMPAT_PTR(a)                                           (a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)a))
#define IS_CMP_TASK()                                             (FALSE)
#define CMP_CPY_FROM_USER                                       CPY_FROM_USER
#define CMP_CPY_TO_USER                                         CPY_to_USER
#endif //CONFIG_COMPAT


UADP_SPT_0NXT_DEF(HWI2C_UnitCfg);
 

UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SelectPort);
UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetClk);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetReadMode); 
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_WriteByte); 
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SelectPort1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetClkP1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetReadModeP1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_WriteByteP1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetDbgLevel);
// one pointer member

//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_Init);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_GetPortIndex);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_ReadByte);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_ReadByteP1);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_GetLibVer);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_GetStatus);
// two pointer member
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_WriteBytes);
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_ReadBytes);
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_WriteBytesP1);
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_ReadBytesP1);
 
UADP_SDT_0_DEF(IIC_CFG_INIT);
UADP_SDT_1_DEF(HWIIC_PRIVATE_INIT);


MS_U32 HWI2C_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct 
    UADP_SPT_0NXT(HWI2C_UnitCfg);
//set table
    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SelectPort);
    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetClk);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetReadMode);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_WriteByte);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SelectPort1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetClkP1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetReadModeP1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_WriteByteP1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetDbgLevel);
    
    //UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_Init, psCfg, HWI2C_UnitCfg);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_GetPortIndex, pu8Port, MS_U8);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_ReadByte, pData, MS_U8);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_ReadByteP1, pData, MS_U8);
    //UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_GetLibVer, ppVersion, MSIF_Version);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_GetStatus, pStatus, HWI2C_Status);
    
    //UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_WriteBytes, pRegAddr, MS_U8, pData, MS_U8);
    //UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_ReadBytes, pRegAddr, MS_U8, pData, MS_U8);
  //  UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_WriteBytesP1, pRegAddr, MS_U8, pData, MS_U8);
  //  UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_ReadBytesP1, pRegAddr, MS_U8, pData, MS_U8);
//set table
 
    UADP_SDT_NAME0(IIC_CFG_INIT,HWI2C_UnitCfg);
    UADP_SDT_NAME1(HWIIC_PRIVATE_INIT,HWI2C_PRIVATE_PARAM_Init,UADP_SDT_P2N,psCfg,IIC_CFG_INIT);
 
    *pIoctl= (FUtopiaIOctl)HWI2C_adp_Ioctl;
    return 0;	
}

MS_U32 HWI2C_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
 	char      buffer_arg[128];
 
    switch(u32Cmd)
    {
       case MDrv_CMD_HWI2C_Init :
          //u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_HWI2C_PRIVATE_PARAM_Init, NULL,buffer_arg,sizeof(buffer_arg));
          u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_HWIIC_PRIVATE_INIT, spt_HWIIC_PRIVATE_INIT,buffer_arg,sizeof(buffer_arg));
          break;
        case MDrv_CMD_HWI2C_Start:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_HWI2C_Stop:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_HWI2C_SelectPort:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_HWI2C_PRIVATE_PARAM_SelectPort, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_HWI2C_SetClk:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_HWI2C_PRIVATE_PARAM_SetClk, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_HWI2C_WriteBytes:
	{
#ifdef CONFIG_COMPAT
		if(IS_CMP_TASK())
		{
			HWI2C_PRIVATE_PARAM_WriteBytes tmp;
			_compat_stHWI2C_PRIVATE_PARAM_WriteBytes tmp_compat;			
			CMP_CPY_FROM_USER(&tmp_compat, pArgs, sizeof(_compat_stHWI2C_PRIVATE_PARAM_WriteBytes)); 
			tmp.pRegAddr = kmalloc(sizeof(MS_U8)*tmp_compat.uAddrCnt,GFP_KERNEL);
			tmp.pData = kmalloc(sizeof(MS_U8)*tmp_compat.uSize,GFP_KERNEL);
			CMP_CPY_FROM_USER(tmp.pRegAddr, tmp_compat.pRegAddr, sizeof(MS_U8)*tmp_compat.uAddrCnt);
			CMP_CPY_FROM_USER(tmp.pData, tmp_compat.pData, sizeof(MS_U8)*tmp_compat.uSize);
			tmp.u16SlaveCfg=tmp_compat.u16SlaveCfg;
			tmp.uAddrCnt=tmp_compat.uAddrCnt;
			tmp.uSize=tmp_compat.uSize;
			u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp);
			tmp_compat.bReturn = tmp.bReturn;
			CMP_CPY_TO_USER(pArgs,&tmp_compat,sizeof(_compat_stHWI2C_PRIVATE_PARAM_WriteBytes));
			kfree(tmp.pRegAddr);
			kfree(tmp.pData);
		}	   
            else  
#endif    
            {
                 
	     HWI2C_PRIVATE_PARAM_WriteBytes paramW;
			CPY_FROM_USER(&paramW,pArgs,sizeof(HWI2C_PRIVATE_PARAM_WriteBytes));
	     MS_U8 u8AddLen = (paramW.uAddrCnt>0?paramW.uAddrCnt:1);
	     MS_U8 u8DataLen = (paramW.uSize>0?paramW.uSize:1);
	     MS_U8 u8Addr[u8AddLen];
	     MS_U8 u8Data[u8DataLen];
			       
             if (paramW.uAddrCnt > 0)
				CPY_FROM_USER(u8Addr, paramW.pRegAddr,paramW.uAddrCnt);
	     if (paramW.uSize > 0)
				CPY_FROM_USER(u8Data, paramW.pData, paramW.uSize);
	     paramW.pData = u8Data;
	     paramW.pRegAddr = u8Addr;
             u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&paramW);
			CPY_to_USER(&((HWI2C_PRIVATE_PARAM_WriteBytes *)pArgs)->bReturn , &paramW.bReturn, sizeof(MS_BOOL));
             }    
            break;
        }
        case MDrv_CMD_HWI2C_ReadBytes:
	{
#ifdef CONFIG_COMPAT		
		if(IS_CMP_TASK())
		{
			HWI2C_PRIVATE_PARAM_ReadBytes tmpr;
			_compat_stHWI2C_PRIVATE_PARAM_ReadBytes tmpr_compat;			
			CMP_CPY_FROM_USER(&tmpr_compat, pArgs, sizeof(_compat_stHWI2C_PRIVATE_PARAM_ReadBytes)); 
			tmpr.pRegAddr = kmalloc(sizeof(MS_U8)*tmpr_compat.uAddrCnt,GFP_KERNEL);
			tmpr.pData = kmalloc(sizeof(MS_U8)*tmpr_compat.uSize,GFP_KERNEL);
			CMP_CPY_FROM_USER(tmpr.pRegAddr, tmpr_compat.pRegAddr, sizeof(MS_U8)*tmpr_compat.uAddrCnt);
			tmpr.u16SlaveCfg=tmpr_compat.u16SlaveCfg;
			tmpr.uAddrCnt=tmpr_compat.uAddrCnt;
			tmpr.uSize=tmpr_compat.uSize;
			u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmpr);
			tmpr_compat.bReturn = tmpr.bReturn;
			CMP_CPY_TO_USER(tmpr_compat.pData,tmpr.pData,sizeof(MS_U8)*tmpr_compat.uSize);   
			CMP_CPY_TO_USER(pArgs,&tmpr_compat,sizeof(_compat_stHWI2C_PRIVATE_PARAM_ReadBytes));
			kfree(tmpr.pRegAddr);
			kfree(tmpr.pData);
		}
            else
#endif				
            {
            
	     HWI2C_PRIVATE_PARAM_ReadBytes paramR;
			CPY_FROM_USER(&paramR,pArgs,sizeof(HWI2C_PRIVATE_PARAM_ReadBytes));
	     MS_U8 u8AddLen = (paramR.uAddrCnt>0?paramR.uAddrCnt:1);
	     MS_U8 u8DataLen = (paramR.uSize>0?paramR.uSize:1);
	     MS_U8 u8Addr[u8AddLen];
	     MS_U8 u8Data[u8DataLen];
             if (paramR.uAddrCnt > 0)
				CPY_FROM_USER(u8Addr, paramR.pRegAddr,paramR.uAddrCnt);
             paramR.pRegAddr = u8Addr;			
             paramR.pData = u8Data;
             u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&paramR);			 
	     if (paramR.uSize > 0)
				CPY_to_USER(((HWI2C_PRIVATE_PARAM_WriteBytes *)pArgs)->pData, u8Data, paramR.uSize);
				CPY_to_USER(&((HWI2C_PRIVATE_PARAM_WriteBytes *)pArgs)->bReturn , &paramR.bReturn, sizeof(MS_BOOL));
            }

            break;
        }
        default:
            break;
    }
	return u32Ret;
}




