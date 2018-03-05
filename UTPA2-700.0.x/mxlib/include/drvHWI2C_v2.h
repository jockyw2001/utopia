#ifndef _DRV_HWI2C_V2_H_
#define _DRV_HWI2C_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif


//#include "drvHWI2C.h"
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MDrv_CMD_HWI2C_Init,
    MDrv_CMD_HWI2C_Start,
    MDrv_CMD_HWI2C_Stop,
    MDrv_CMD_HWI2C_GetPortIndex,
    MDrv_CMD_HWI2C_SelectPort,
    MDrv_CMD_HWI2C_SetClk,
    MDrv_CMD_HWI2C_SetReadMode,
    MDrv_CMD_HWI2C_WriteByte,
    MDrv_CMD_HWI2C_WriteBytes,
    MDrv_CMD_HWI2C_ReadByte,
    MDrv_CMD_HWI2C_ReadBytes,
    MDrv_CMD_HWI2C_SelectPort1,
	MDrv_CMD_HWI2C_SetClkP1,
	MDrv_CMD_HWI2C_SetReadModeP1,
	MDrv_CMD_HWI2C_WriteByteP1,
	MDrv_CMD_HWI2C_WriteBytesP1,
	MDrv_CMD_HWI2C_ReadByteP1,
	MDrv_CMD_HWI2C_ReadBytesP1,
	MDrv_CMD_HWI2C_SetDbgLevel,
	MDrv_CMD_HWI2C_GetLibVer,
	MDrv_CMD_HWI2C_GetStatus,
	MDrv_CMD_HWI2C_GetInfo,
	MDrv_CMD_HWI2C_ConfigDelay,
	MDrv_CMD_HWI2C_SetPowerState
} eHWI2CIoctlOpt;

#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3)
typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_Init
{
   HWI2C_UnitCfg *psCfg;
} HWI2C_PRIVATE_PARAM_Init, *PHWI2C_PRIVATE_PARAM_Init;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_GetPortIndex
{
   HWI2C_PORT ePort;
   MS_U8* pu8Port;
} HWI2C_PRIVATE_PARAM_GetPortIndex, *PHWI2C_PRIVATE_PARAM_GetPortIndex;

typedef struct _HWI2C_PRIVATE_PARAM_SelectPort
{
   HWI2C_PORT ePort;
} HWI2C_PRIVATE_PARAM_SelectPort, *PHWI2C_PRIVATE_PARAM_SelectPort;

typedef struct _HWI2C_PRIVATE_PARAM_SetClk
{
   HWI2C_CLKSEL eClk;
} HWI2C_PRIVATE_PARAM_SetClk, *PHWI2C_PRIVATE_PARAM_SetClk;

typedef struct _HWI2C_PRIVATE_PARAM_SetReadMode
{
   HWI2C_ReadMode eReadMode;
} HWI2C_PRIVATE_PARAM_SetReadMode, *PHWI2C_PRIVATE_PARAM_SetReadMode;

typedef struct _HWI2C_PRIVATE_PARAM_WriteByte
{
   MS_U16 u16SlaveCfg;
   MS_U8 u8RegAddr;
   MS_U8 u8Data;
   MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_WriteByte, *PHWI2C_PRIVATE_PARAM_WriteByte;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_WriteBytes
{
   MS_U16 u16SlaveCfg;
   MS_U32 uAddrCnt;
   MS_U8 *pRegAddr;
   MS_U32 uSize;
   MS_U8 *pData;
   MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_WriteBytes, *PHWI2C_PRIVATE_PARAM_WriteBytes;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadByte
{
   MS_U16 u16SlaveCfg;
   MS_U8 u8RegAddr;
   MS_U8 *pData;
   MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_ReadByte, *PHWI2C_PRIVATE_PARAM_ReadByte;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadBytes
{
   MS_U16 u16SlaveCfg;
   MS_U32 uAddrCnt;
   MS_U8 *pRegAddr;
   MS_U32 uSize;
   MS_U8 *pData;
   MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_ReadBytes, *PHWI2C_PRIVATE_PARAM_ReadBytes;

typedef struct _HWI2C_PRIVATE_PARAM_SelectPort1
{
   HWI2C_PORT ePort;
} HWI2C_PRIVATE_PARAM_SelectPort1, *PHWI2C_PRIVATE_PARAM_SelectPort1;

typedef struct _HWI2C_PRIVATE_PARAM_SetClkP1
{
   HWI2C_CLKSEL eClk;
} HWI2C_PRIVATE_PARAM_SetClkP1, *PHWI2C_PRIVATE_PARAM_SetClkP1;

typedef struct _HWI2C_PRIVATE_PARAM_SetReadModeP1
{
   HWI2C_ReadMode eReadMode;
} HWI2C_PRIVATE_PARAM_SetReadModeP1, *PHWI2C_PRIVATE_PARAM_SetReadModeP1;

typedef struct _HWI2C_PRIVATE_PARAM_WriteByteP1
{
   MS_U16 u16SlaveCfg;
   MS_U8 u8RegAddr;
   MS_U8 u8Data;
} HWI2C_PRIVATE_PARAM_WriteByteP1, *PHWI2C_PRIVATE_PARAM_WriteByteP1;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_WriteBytesP1
{
   MS_U16 u16SlaveCfg;
   MS_U32 uAddrCnt;
   MS_U8 *pRegAddr;
   MS_U32 uSize;
   MS_U8 *pData;
} HWI2C_PRIVATE_PARAM_WriteBytesP1, *PHWI2C_PRIVATE_PARAM_WriteBytesP1;


typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadByteP1
{
   MS_U16 u16SlaveCfg;
   MS_U8 u8RegAddr;
   MS_U8 *pData;
} HWI2C_PRIVATE_PARAM_ReadByteP1, *PHWI2C_PRIVATE_PARAM_ReadByteP1;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadBytesP1
{
   MS_U16 u16SlaveCfg;
   MS_U32 uAddrCnt;
   MS_U8 *pRegAddr;
   MS_U32 uSize;
   MS_U8 *pData;
} HWI2C_PRIVATE_PARAM_ReadBytesP1, *PHWI2C_PRIVATE_PARAM_ReadBytesP1;

typedef struct _HWI2C_PRIVATE_PARAM_SetDbgLevel
{
   HWI2C_DbgLv eLevel;
} HWI2C_PRIVATE_PARAM_SetDbgLevel, *PHWI2C_PRIVATE_PARAM_SetDbgLevel;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_GetLibVer
{
   const MSIF_Version **ppVersion;
} HWI2C_PRIVATE_PARAM_GetLibVer, *PHWI2C_PRIVATE_PARAM_GetLibVer;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_GetStatus
{
   HWI2C_Status *pStatus;
} HWI2C_PRIVATE_PARAM_GetStatus, *PHWI2C_PRIVATE_PARAM_GetStatus;

#if 0
typedef struct _HWI2C_PRIVATE_PARAM_SetPowerState
{
   EN_POWER_MODE u16PowerState;
} HWI2C_PRIVATE_PARAM_SetPowerState;
#endif
#if 1
typedef union _HWI2C_PRIVATE_PARAM
{
	HWI2C_PRIVATE_PARAM_Init privateInit;
	HWI2C_PRIVATE_PARAM_GetPortIndex  privateGetPortIndex;
	HWI2C_PRIVATE_PARAM_SelectPort privateSelectPort;
	HWI2C_PRIVATE_PARAM_SetClk privateSetClk;
	HWI2C_PRIVATE_PARAM_SetReadMode privateSetReadMode;
	HWI2C_PRIVATE_PARAM_WriteByte privateWriteByte;
	HWI2C_PRIVATE_PARAM_WriteBytes privateWriteBytes;
	HWI2C_PRIVATE_PARAM_ReadByte privateReadByte;
	HWI2C_PRIVATE_PARAM_ReadBytes privateReadBytes;
	HWI2C_PRIVATE_PARAM_SetReadModeP1 privateSetReadModeP1;
	HWI2C_PRIVATE_PARAM_WriteByteP1 privateWriteByteP1;
	HWI2C_PRIVATE_PARAM_WriteBytesP1 privateWriteBytesP1;
	HWI2C_PRIVATE_PARAM_ReadByteP1 privateReadByteP1;
	HWI2C_PRIVATE_PARAM_ReadBytesP1 privateReadBytesP1;
	HWI2C_PRIVATE_PARAM_SetDbgLevel privateSetDbgLevel;
	HWI2C_PRIVATE_PARAM_GetStatus privateGetStatus;
	//HWI2C_PRIVATE_PARAM_SetPowerState privateSetPowerState;
} HWI2C_PRIVATE_PARAM;
#endif

#endif

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_HWI2C_V2_H_


