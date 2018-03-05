
//=============================================================================
// Includs
//=============================================================================

#include <stdio.h>
#include <string.h>

#include "drvMBX.h"
#include "apiMBX.h"
#include "MsOS.h"


//=============================================================================
// Compile options
//=============================================================================

//=============================================================================
// Local Defines
//=============================================================================

#define MAX_DMBX_CLASS 20
#define MAX_DMBX_NAME_LEN (MAX_MBX_PARAM_SIZE-1)

//=============================================================================
// Debug Macros
//=============================================================================
        
//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Local Variables
//=============================================================================

//Record Dynamic Register Mailbox Class 
typedef struct _Dynmaic_MBX_Class
{
	char name[MAX_DMBX_NAME_LEN];
	MS_U16 mbx_class;
} Dynamic_MBX_Class;

typedef enum _Dynamic_MBX_Index
{
	E_Dynamic_GenerateClass = 0,
	E_Dynamic_QueryCalsss = 1,
} Dynamic_MBX_Index;

//=============================================================================
// Global Variables
//=============================================================================

static MS_U8 Count_DMBX_Class = 0;
static Dynamic_MBX_Class Tab_DMBX_Class[MAX_DMBX_CLASS];

#ifndef ANDROID
#define VPRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#define VPRINTF //ALOGD
#endif


#if 0//def MSOS_TYPE_LINUX

MS_U32 s32APIMutexMBX;


#define API_MBX_InitLock()  {\
                                    s32APIMutexMBX = MsOS_CreateMutex(E_MSOS_FIFO, "MBX_Send_Mutex", MSOS_PROCESS_SHARED);\
                                    if(s32APIMutexMBX<0)\
                                    {\
                                        printf("!!!Acquire Send Mutex for MBX Failed!\n");\
                                        return E_MBX_UNKNOW_ERROR;\
                                    }\
                            }

#define API_MBX_DeInitLock()   {\
                                 MsOS_DeleteMutex(s32APIMutexMBX);\
                               }

#define API_MBX_LockSend()   {\
                                {\
                                 MsOS_ObtainMutex(s32APIMutexMBX, MSOS_WAIT_FOREVER);\
                                 }\
                             }

#define API_MBX_UnLockSend()   {\
                                    {\
                                        MsOS_ReleaseMutex(s32APIMutexMBX);\
                                    }\
                               }

#else
#define API_MBX_InitLock()
#define API_MBX_DeInitLock()
#define API_MBX_LockSend() 
#define API_MBX_UnLockSend()
#endif


//=============================================================================
// Local Function Prototypes
//=============================================================================

static MBX_Result _MApi_Search_ClassNum(char *name, MS_U8 *pmbx_class)
{
	MS_U16 i = 0;

	*pmbx_class = 0x00;

	//judge if name length exceed  MAX_DMBX_NAME_LEN or NULL
	if ((name==NULL) || (strlen(name)>(MAX_DMBX_NAME_LEN-1)))
		return E_MBX_UNKNOW_ERROR;
	
	//Search Dynamic Class Table
	for (i = 0; i < Count_DMBX_Class; i++)
	{
		if (strncmp(Tab_DMBX_Class[i].name, name, MAX_DMBX_NAME_LEN) == 0)
		{
			*pmbx_class = Tab_DMBX_Class[i].mbx_class;
			return E_MBX_SUCCESS;
		}
	}

	return  E_MBX_UNKNOW_ERROR;

}


static void _MApi_Conduct_GenerateClass(MBX_Msg *pMsg)
{
	//prepare receive mailbox transmit class & name info
	int flag_repeat = 0;
	int i;
	char temp_name[MAX_DMBX_NAME_LEN] = {0};
	MS_U8 temp_class = 0;
	
	if (pMsg->u8ParameterCount > 0)
	{
		temp_class = pMsg->u8Parameters[0];
		for (i = 1; i<pMsg->u8ParameterCount; i++)
		{
			temp_name[i-1] =  pMsg->u8Parameters[i];
		}
		//printf("%s, temp_class=0x%x, temp_name=%s\n", __FUNCTION__, temp_class, temp_name);
	}
	else
	{
		//printf("Error Send Message\n");
		return;
	}

	//Search Dynamic Class Table
	for (i = 0; i < Count_DMBX_Class; i++)
	{
		if (strncmp(Tab_DMBX_Class[i].name, temp_name, MAX_DMBX_NAME_LEN) == 0)
		{
			flag_repeat = 1;
			break;
		}
	}

	if (flag_repeat == 0)
	{
		Tab_DMBX_Class[Count_DMBX_Class].name[0] = 0; //clear name string
		
		Tab_DMBX_Class[Count_DMBX_Class].mbx_class = temp_class;
		strncpy(Tab_DMBX_Class[Count_DMBX_Class].name, temp_name, MAX_DMBX_NAME_LEN);
		Count_DMBX_Class++;
	}

    MBX_Msg MB_Command;
	
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_CPU_MIPS;//E_MBX_CPU_R2M;//E_MBX_ROLE_CP;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass =  E_MBX_CLASS_SECURE_WAIT; // E_MBX_CLASS_PM_NOWAIT;
    MB_Command.u8Index = 0;//PM_CMDIDX_GLOBAL_CHIP_RESET;
    MB_Command.u8ParameterCount = 1;
	MB_Command.u8Parameters[0] = flag_repeat;

    MApi_MBX_SendMsg(&MB_Command);
	
}

static void _MApi_Conduct_QueryClass(MBX_Msg *pMsg)
{
	int i;
	char temp_name[MAX_DMBX_NAME_LEN] = {0};
	MS_U8 temp_class = 0;

	if (pMsg->u8ParameterCount > 0)
	{
		for (i = 1; i<pMsg->u8ParameterCount; i++)
		{
			temp_name[i-1] =  pMsg->u8Parameters[i];
		}
		//printf("%s, temp_class=0x%x, temp_name=%s\n", __FUNCTION__, temp_class, temp_name);
	}
	else
	{
		//printf("Error Send Message\n");
		return;
	}

	_MApi_Search_ClassNum(temp_name, &temp_class);

	MBX_Msg MB_Command;
	
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_CPU_MIPS;//E_MBX_CPU_R2M;//E_MBX_ROLE_CP;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECURE_WAIT; // E_MBX_CLASS_PM_NOWAIT;
    MB_Command.u8Index = 2;//PM_CMDIDX_GLOBAL_CHIP_RESET;
    MB_Command.u8ParameterCount = 1;
	MB_Command.u8Parameters[0] = temp_class;

    MApi_MBX_SendMsg(&MB_Command);

}


static void _MApi_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{ 

	*pbAddToQueue = FALSE;

	switch (pMsg->u8Index)
	{
		case E_Dynamic_GenerateClass:
			_MApi_Conduct_GenerateClass(pMsg);
			break;
		case E_Dynamic_QueryCalsss:
			_MApi_Conduct_QueryClass(pMsg);
			break;
		default:
			return ;
	}
}

MS_BOOL _gbFlagReg_Q = FALSE;
MS_BOOL _gbFlagReg_S = FALSE;
extern MBX_Result  _MDrv_MBX_Init_(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs);

static MS_BOOL MApi_MBX_ISMsQEmpty(MBX_Class eTargetClass)
{

	MBX_MSGQ_Status eMsgQStatus;
	MBX_Result ret = E_MBX_SUCCESS;
	
	ret = MDrv_MBX_GetMsgQueueStatus(eTargetClass, &eMsgQStatus);	

	if (ret != E_MBX_SUCCESS)
		return TRUE;

	return (((eMsgQStatus.u32NormalMsgCount+eMsgQStatus.u32InstantMsgCount) > 0)? FALSE:TRUE);
}

#define Wait_MSQ_Empty(eTargetClass) {	\
											while(MApi_MBX_ISMsQEmpty(eTargetClass) == FALSE);	\
										}

static MBX_Result MApi_MBX_RegisterQueryMBX(void)
{
	MBX_Result ret = E_MBX_SUCCESS;

	if (_gbFlagReg_Q == FALSE)
	{
		Wait_MSQ_Empty(E_MBX_CLASS_QUERY_CLASS);
		ret = MApi_MBX_RegisterMSGWithCallBack(E_MBX_CLASS_QUERY_CLASS, 5, _MApi_HandleDynamicMBX);

		if (ret == E_MBX_SUCCESS)
		{
			_gbFlagReg_Q = TRUE;
		}
		else
		{
			return ret;
		}	
	}
		
	if (_gbFlagReg_S == FALSE)
	{
		Wait_MSQ_Empty(E_MBX_CLASS_SECURE_WAIT);
		ret = MApi_MBX_RegisterMSG(E_MBX_CLASS_SECURE_WAIT, 10);

		if (ret == E_MBX_SUCCESS)
		{
			_gbFlagReg_S = TRUE;
		}
		else
		{
			return ret;
		}	
	}

	return ret;

}



static MBX_Result MApi_MBX_UnRegisterQueryMBX(void)
{
	MBX_Result ret = E_MBX_SUCCESS;

	if (_gbFlagReg_Q == TRUE)
	{
		Wait_MSQ_Empty(E_MBX_CLASS_QUERY_CLASS);
		ret = MApi_MBX_UnRegisterMSG(E_MBX_CLASS_QUERY_CLASS, TRUE);
		if (ret == E_MBX_SUCCESS)
		{
			_gbFlagReg_Q = FALSE;
		}
		else
		{
			return ret;
		}
	}

		
	if (_gbFlagReg_S == TRUE)
	{
		Wait_MSQ_Empty(E_MBX_CLASS_SECURE_WAIT);
		ret = MApi_MBX_UnRegisterMSG(E_MBX_CLASS_SECURE_WAIT, TRUE);

		if (ret == E_MBX_SUCCESS)
		{
			_gbFlagReg_S = FALSE;
		}
		else
		{
			return ret;
		}	
	}

	return ret;

}



MBX_Result  MApi_MBX_Init(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs)
{
	MBX_Result ret;

	ret =  _MDrv_MBX_Init_(eHKCPU, eHostRole, u32TimeoutMillSecs);

	API_MBX_InitLock();
	
#if defined(MSOS_TYPE_NUTTX)
	if (ret == E_MBX_SUCCESS)
	{
		ret = MApi_MBX_RegisterQueryMBX();
	}
#endif

	return ret;
}

MBX_Result  MApi_MBX_DeInit(MS_BOOL bForceDiscardPendingMsg)
{
	return MDrv_MBX_DeInit(bForceDiscardPendingMsg);
}

MBX_Result  MApi_MBX_RegisterMSG(MBX_Class eMsgClass, MS_U16 u16MsgQueueSize)
{
	return MDrv_MBX_RegisterMSG(eMsgClass, u16MsgQueueSize);
}

MBX_Result  MApi_MBX_RegisterMSGWithCallBack(MBX_Class eMsgClass, MS_U16 u16MsgQueueSize, MBX_MAIL_ARRIVE_NOTIFY_FUNC notifier)
{
	return MDrv_MBX_RegisterMSGWithCallBack(eMsgClass, u16MsgQueueSize, notifier);
}


MBX_Result  MApi_MBX_UnRegisterMSG(MBX_Class eMsgClass, MS_BOOL bForceDiscardMsgQueue)
{
	return MDrv_MBX_UnRegisterMSG(eMsgClass, bForceDiscardMsgQueue);
}

MBX_Result  MApi_MBX_ClearMSG(MBX_Class eMsgClass)
{
	return MDrv_MBX_ClearMSG(eMsgClass);
}

MBX_Result  MApi_MBX_SendMsg(MBX_Msg *pMsg)
{
	return MDrv_MBX_SendMsg(pMsg);
}

MBX_Result  MApi_MBX_GetMsgQueueStatus(MBX_Class eTargetClass, MBX_MSGQ_Status *pMsgQueueStatus)
{
	return MDrv_MBX_GetMsgQueueStatus(eTargetClass, pMsgQueueStatus);
}

MBX_Result  MApi_MBX_RecvMsg(MBX_Class eTargetClass, MBX_Msg *pMsg, MS_U32 u32WaitMillSecs, MS_U32 u32Flag)
{
	return MDrv_MBX_RecvMsg(eTargetClass, pMsg, u32WaitMillSecs, u32Flag);
}

MBX_Result  MApi_MBX_CheckMsg(MBX_Class eTargetClass, MBX_Msg *pMsg, MS_U32 u32WaitMillSecs, MS_U32 u32Flag)
{
	return MDrv_MBX_CheckMsg(eTargetClass, pMsg, u32WaitMillSecs, u32Flag);
}

MBX_Result  MApi_MBX_SendMsgLoopback(MBX_Msg *pMsg, MBX_ROLE_ID eSrcRoleId)
{
	return MDrv_MBX_SendMsgLoopback(pMsg, eSrcRoleId);
}

MBX_Result  MApi_MBX_Enable(MS_BOOL bEnable)
{
	return MDrv_MBX_Enable(bEnable);
}

MBX_Result  MApi_MBX_RemoveLatestMsg(void)
{
	return MDrv_MBX_RemoveLatestMsg();
}

static MBX_Result  _MApi_MBX_DynamicClass(MBX_ROLE_ID eRoleID, char *name, MS_U8 *pmbx_class, Dynamic_MBX_Index eDMBX_Index)
{
	MS_U16 i;
	MBX_Result ret = E_MBX_SUCCESS;

	API_MBX_LockSend();

	if (_MApi_Search_ClassNum(name, pmbx_class) == E_MBX_SUCCESS)
	{
		API_MBX_UnLockSend();
		return E_MBX_SUCCESS;
	}

	MBX_Msg smsg;

	Tab_DMBX_Class[Count_DMBX_Class].name[0] = 0; //clear name string
	strncpy(Tab_DMBX_Class[Count_DMBX_Class].name, name, MAX_DMBX_NAME_LEN);
	Tab_DMBX_Class[Count_DMBX_Class].mbx_class = E_MBX_CLASS_NUM + Count_DMBX_Class + 1;
	//printf("\33[33m name=%s: \33[0m\n",name);
	if(E_Dynamic_QueryCalsss==eDMBX_Index)
	{

		MApi_MBX_RegisterQueryMBX();
		
	   //printf("\33[33m %s:%s::%d\33[0m\n",__FILE__,__func__,__LINE__);
		memset((void*)&smsg, 0, sizeof(MBX_Msg));	
		smsg.eRoleID = eRoleID;
		smsg.eMsgType = E_MBX_MSG_TYPE_INSTANT;
		smsg.u8Ctrl = 0;
		smsg.u8MsgClass = E_MBX_CLASS_QUERY_CLASS;
		smsg.u8Index = eDMBX_Index;
		smsg.u8ParameterCount = strlen(Tab_DMBX_Class[Count_DMBX_Class].name) + 2; // string end token

		smsg.u8Parameters[0] =  (MS_U8) Tab_DMBX_Class[Count_DMBX_Class].mbx_class;
		for (i = 0; i < (smsg.u8ParameterCount); i++)
			smsg.u8Parameters[i+1] = (MS_U8) Tab_DMBX_Class[Count_DMBX_Class].name[i];


		ret = MDrv_MBX_SendMsg(&smsg);
		if (ret != E_MBX_SUCCESS)
		{
			API_MBX_UnLockSend();
			//printf("Send Mailbox Failed, status=%d\n", ret);
			return ret;
		}

	   //printf("\33[33m %s:%s::%d\33[0m\n",__FILE__,__func__,__LINE__);

		memset((void*)&smsg, 0, sizeof(MBX_Msg));	
		//ret = MDrv_MBX_RecvMsg(E_MBX_CLASS_SECURE_WAIT, &smsg, 1000, MBX_CHECK_INSTANT_MSG);
		//if (ret != E_MBX_SUCCESS)
		while(E_MBX_SUCCESS!=MDrv_MBX_RecvMsg(E_MBX_CLASS_SECURE_WAIT, &smsg, 1000, MBX_CHECK_INSTANT_MSG))
		{
			//printf("Receive Mailbox Failed, status=%d\n", ret);
			//return ret;
		}
	   //printf("\33[33m %s:%s::%d\33[0m\n",__FILE__,__func__,__LINE__);

	   MApi_MBX_UnRegisterQueryMBX();
	  
	}

	   //printf("\33[33m %s:%s::%d,%x\33[0m\n",__FILE__,__func__,__LINE__,eDMBX_Index);
	switch (eDMBX_Index)
	{
		case E_Dynamic_GenerateClass:
			*pmbx_class = Tab_DMBX_Class[Count_DMBX_Class].mbx_class;
			Count_DMBX_Class++;
			break;
		case E_Dynamic_QueryCalsss:
		     
	   //printf("\33[33m %s:%s::%d,%x,%x\33[0m\n",__FILE__,__func__,__LINE__,smsg.u8ParameterCount,smsg.u8Parameters[0]);
			 if (smsg.u8ParameterCount > 0 && smsg.u8Parameters[0] != 0)
			 {
			 	Tab_DMBX_Class[Count_DMBX_Class].mbx_class = smsg.u8Parameters[0];
				*pmbx_class = Tab_DMBX_Class[Count_DMBX_Class].mbx_class;
				Count_DMBX_Class++;
			 }
			break;
	}

	API_MBX_UnLockSend();
	return ret;
}


MBX_Result  MApi_MBX_QueryDynamicClass(MBX_ROLE_ID eRoleID, char *name, MS_U8 *pmbx_class)
{
	return _MApi_MBX_DynamicClass(eRoleID, name, pmbx_class, E_Dynamic_QueryCalsss);
}

MBX_Result  MApi_MBX_GenerateDynamicClass(MBX_ROLE_ID eRoleID, char *name, MS_U8 *pmbx_class)
{
	return _MApi_MBX_DynamicClass(eRoleID, name, pmbx_class, E_Dynamic_GenerateClass);
}





