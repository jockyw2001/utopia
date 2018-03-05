#ifndef _PWS_SCENE_INFO_TABLE_H_
#define _PWS_SCENE_INFO_TABLE_H_

#include "MsTypes.h"
#include "drvMVOP.h"

typedef struct {
    E_PWS_SCENE_name eSceneName;
    E_PWS_IpPowerCtrl eIpPowerCtrl;
    MS_U32    u32RegAddr;
    MS_U8     u8RegMask;
    MS_U8     u8RegVal;
} PWS_SCENE_TABLE_INFO;

typedef struct {
    E_PWS_SCENE_name eSceneName;
    E_PWS_IpPowerCtrl eIpPowerCtrl;
    P_PWS_Callback callback;
} PWS_SCENE_CB;

static const PWS_SCENE_CB pws_scene_cb[] =
{
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_ON, MDrv_MVOP_Init},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_OFF, MDrv_MVOP_Exit},
  {E_PWS_SCENE_MM_SUB, E_PWS_IP_ON, MDrv_MVOP_SubInit},
  {E_PWS_SCENE_MM_SUB, E_PWS_IP_OFF, MDrv_MVOP_SubExit},
  {E_PWS_SCENE_MAX, E_PWS_IP_OFF, NULL},
};

static const PWS_SCENE_TABLE_INFO pws_scene_info_table[] =
{
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_ON,  0x100bb7, 0x01, 0x00},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_ON,  0x100bab, 0x01, 0x00},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_ON,  0x100baa, 0x01, 0x00},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_ON,  0x133f80, 0xFE, 0xFE},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_ON,  0x133f81, 0x01, 0x01},

  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_OFF, 0x100bb7, 0x01, 0x01},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_OFF, 0x100bab, 0x01, 0x01},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_OFF, 0x100baa, 0x01, 0x01},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_OFF, 0x133f80, 0xFE, 0x00},
  {E_PWS_SCENE_MM_MAIN, E_PWS_IP_OFF, 0x133f81, 0x01, 0x00},
  {E_PWS_SCENE_MAX, E_PWS_IP_OFF,     0x0,      0x0,  0x0},
};

#endif