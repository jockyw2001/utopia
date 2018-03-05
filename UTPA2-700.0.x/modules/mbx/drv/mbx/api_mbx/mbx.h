//#include "apiMBX.h"
#include "drvMBX.h"

#include "MsTypes.h"
enum {
	MDrv_CMD_MBX_RegisterMSGWithCallBack,
	MDrv_CMD_MBX_RegisterMSG,
	MDrv_CMD_MBX_SendMsg,
	MDrv_CMD_MBX_RecvMsg,
	MDrv_CMD_MBX_Enable,
	MDrv_CMD_MBX_QueryClass,
} eMbxIoctlOpt;

//private data
typedef struct _MBX_PRIVATE_PARAM_RegisterMSGWithCallBack
{
    MBX_Class		eMsgClass;
	MS_U16			u16MsgQueueSize;
	MBX_MAIL_ARRIVE_NOTIFY_FUNC		notifier;
}MBX_PRIVATE_PARAM_RegisterMSGWithCallBack;

typedef struct _MBX_PRIVATE_PARAM_RegisterMSG
{
    MBX_Class		eMsgClass;
	MS_U16			u16MsgQueueSize;
}MBX_PRIVATE_PARAM_RegisterMSG;


typedef struct _MBX_PRIVATE_PARAM_SendMsg
{
    MBX_Msg 		*pMsg;
}MBX_PRIVATE_PARAM_SendMsg;

typedef struct _MBX_PRIVATE_PARAM_RecvMsg
{
    MBX_Class 		eTargetClass;
	MBX_Msg 		*pMsg;
	MS_U32 			u32WaitMillSecs;
	MS_U32 			u32Flag;
}MBX_PRIVATE_PARAM_RecvMsg;

typedef struct _MBX_PRIVATE_PARAM_Enable
{
    MS_BOOL 		bEnable;
}MBX_PRIVATE_PARAM_Enable;

typedef struct _MBX_PRIVATE_PARAM_QClass
{
	MBX_ROLE_ID eRoleID;
    char name[50];
	MS_U8 mbx_class;
}MBX_PRIVATE_PARAM_QClass;

typedef union _MBX_RESOURCE_PRIVATE
{
	MBX_PRIVATE_PARAM_RegisterMSGWithCallBack		privateRegisterMSGWithCallBack;
	MBX_PRIVATE_PARAM_RegisterMSG					privateRegisterMSG;
	MBX_PRIVATE_PARAM_SendMsg						privateSendMsg;
	MBX_PRIVATE_PARAM_RecvMsg						privateRecvMsg;
	MBX_PRIVATE_PARAM_Enable						privateEnable;
	MBX_PRIVATE_PARAM_QClass						privateQClass;
} MBX_RESOURCE_PRIVATE;

//instant private function
typedef MBX_Result (*IOCTL_RegisterMSGWithCallBack)(MBX_Class , MS_U16 , MBX_MAIL_ARRIVE_NOTIFY_FUNC );
typedef MBX_Result (*IOCTL_RegisterMSG) (MBX_Class , MS_U16 );
typedef MBX_Result (*IOCTL_SendMsg)(MBX_Msg * );
typedef MBX_Result (*IOCTL_RecvMsg)(MBX_Class , MBX_Msg *, MS_U32 , MS_U32 );
typedef MBX_Result (*IOCTL_Enable)(MS_BOOL  );
typedef MBX_Result (*IOCTL_QueryDynamicClass) (MBX_ROLE_ID , char *, MS_U8 *);


typedef struct _MBX_INSTANT_PRIVATE
{
    IOCTL_RegisterMSGWithCallBack			fpRegisterMSGWithCallBack;
	IOCTL_RegisterMSG						fpRegisterMSG;
	IOCTL_SendMsg						  	fpSendMsg;
	IOCTL_RecvMsg       					fpRecvMsg;
	IOCTL_Enable							fpEnable;
	IOCTL_QueryDynamicClass					fpQueryDynamicClass;
}MBX_INSTANT_PRIVATE;


//instant
typedef struct _MBX_INSTANT_ATTRIBUTE
{
    MBX_CPU_ID		eHKCPU;
	MBX_ROLE_ID		eHostRole;
	MS_U32       	u32TimeoutMillSecs;
}MBX_INSTANT_ATTRIBUTE;


void MBXRegisterToUtopia(FUtopiaOpen ModuleType);

MS_U32 MBXOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 MBXClose(void** pInstantTmp);
MS_U32 MBXIoctl(void** pInstantTmp, MS_U32 u32Cmd, MS_U32* pu32Args);


