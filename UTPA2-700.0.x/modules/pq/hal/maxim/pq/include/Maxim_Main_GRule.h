#define PQ_GRULE_NR_ENABLE 1
#define PQ_GRULE_OSD_BW_ENABLE 1
#define PQ_GRULE_MPEG_NR_ENABLE 1
#define PQ_GRULE_FILM_MODE_ENABLE 1
#define PQ_GRULE_ULTRAT_CLEAR_ENABLE 1
#define PQ_GRULE_PTP_ENABLE 1
#define PQ_GRULE_GAME_MODE_ENABLE 1
#define PQ_GRULE_DS_PARAMETERS_ENABLE 1
#define PQ_GRULE_HDR_ENABLE 1
#define PQ_GRULE_3D_ENABLE 1
#define PQ_GRULE_DEFINE_AUTO_GEN 1
#if (PQ_GRULE_NR_ENABLE) || (PQ_GRULE_OSD_BW_ENABLE) || (PQ_GRULE_MPEG_NR_ENABLE) || (PQ_GRULE_FILM_MODE_ENABLE) || (PQ_GRULE_ULTRAT_CLEAR_ENABLE) || (PQ_GRULE_PTP_ENABLE) || (PQ_GRULE_GAME_MODE_ENABLE) || (PQ_GRULE_DS_PARAMETERS_ENABLE) || (PQ_GRULE_HDR_ENABLE) || (PQ_GRULE_3D_ENABLE)
typedef enum
{
#if PQ_GRULE_NR_ENABLE
    PQ_GRule_NR_Main,
#endif

#if PQ_GRULE_OSD_BW_ENABLE
    PQ_GRule_OSD_BW_Main,
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
    PQ_GRule_MPEG_NR_Main,
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
    PQ_GRule_FILM_MODE_Main,
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
    PQ_GRule_ULTRAT_CLEAR_Main,
#endif

#if PQ_GRULE_PTP_ENABLE
    PQ_GRule_PTP_Main,
#endif

#if PQ_GRULE_GAME_MODE_ENABLE
    PQ_GRule_GAME_MODE_Main,
#endif

#if PQ_GRULE_DS_PARAMETERS_ENABLE
    PQ_GRule_DS_PARAMETERS_Main,
#endif

#if PQ_GRULE_HDR_ENABLE
    PQ_GRule_HDR_Main,
#endif

#if PQ_GRULE_3D_ENABLE
    PQ_GRule_3D_Main,
#endif

}
MST_GRule_Index_Main;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_NR_Off_Main,
    PQ_GRule_NR_Low_Main,
    PQ_GRule_NR_Middle_Main,
    PQ_GRule_NR_High_Main,
    PQ_GRule_NR_Low_3D_Main,
    PQ_GRule_NR_Middle_3D_Main,
    PQ_GRule_NR_High_3D_Main,
    PQ_GRule_NR_Auto_Low_L_Main,
    PQ_GRule_NR_Auto_Low_M_Main,
    PQ_GRule_NR_Auto_Low_H_Main,
    PQ_GRule_NR_Auto_Middle_L_Main,
    PQ_GRule_NR_Auto_Middle_M_Main,
    PQ_GRule_NR_Auto_Middle_H_Main,
    PQ_GRule_NR_Auto_High_L_Main,
    PQ_GRule_NR_Auto_High_M_Main,
    PQ_GRule_NR_Auto_High_H_Main,
    PQ_GRule_NR_Low_PIP_Main,
    PQ_GRule_NR_Middle_PIP_Main,
    PQ_GRule_NR_High_PIP_Main,
    PQ_GRule_NR_Low_4K2K_Main,
    PQ_GRule_NR_Middle_4K2K_Main,
    PQ_GRule_NR_High_4K2K_Main,
}
MST_GRule_NR_Index_Main;
#endif

#if PQ_GRULE_OSD_BW_ENABLE
typedef enum
{
    PQ_GRule_OSD_BW_On_1920_Main,
    PQ_GRule_OSD_BW_On_1920_PIP_Main,
    PQ_GRule_OSD_BW_On_1920_POP_Main,
    PQ_GRule_OSD_BW_On_1366_Main,
    PQ_GRule_OSD_BW_On_1366_PIP_Main,
    PQ_GRule_OSD_BW_On_1366_POP_Main,
    PQ_GRule_OSD_BW_On_4K2K_Main,
    PQ_GRule_OSD_BW_On_4K2K_PIP_Main,
    PQ_GRule_OSD_BW_On_4K2K_POP_Main,
}
MST_GRule_OSD_BW_Index_Main;
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_MPEG_NR_Off_Main,
    PQ_GRule_MPEG_NR_Low_Main,
    PQ_GRule_MPEG_NR_Middle_Main,
    PQ_GRule_MPEG_NR_High_Main,
}
MST_GRule_MPEG_NR_Index_Main;
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
typedef enum
{
    PQ_GRule_FILM_MODE_Off_Main,
    PQ_GRule_FILM_MODE_On_Main,
}
MST_GRule_FILM_MODE_Index_Main;
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
typedef enum
{
    PQ_GRule_ULTRAT_CLEAR_Off_Main,
    PQ_GRule_ULTRAT_CLEAR_On_Main,
    PQ_GRule_ULTRAT_CLEAR_On_4K2K_Main,
}
MST_GRule_ULTRAT_CLEAR_Index_Main;
#endif

#if PQ_GRULE_PTP_ENABLE
typedef enum
{
    PQ_GRule_PTP_On_PTP_Main,
}
MST_GRule_PTP_Index_Main;
#endif

#if PQ_GRULE_GAME_MODE_ENABLE
typedef enum
{
    PQ_GRule_GAME_MODE_On_Main,
}
MST_GRule_GAME_MODE_Index_Main;
#endif

#if PQ_GRULE_DS_PARAMETERS_ENABLE
typedef enum
{
    PQ_GRule_DS_PARAMETERS_L_SD_Main,
    PQ_GRule_DS_PARAMETERS_SD_Main,
    PQ_GRule_DS_PARAMETERS_FHD_Main,
    PQ_GRule_DS_PARAMETERS_4K_Main,
}
MST_GRule_DS_PARAMETERS_Index_Main;
#endif

#if PQ_GRULE_HDR_ENABLE
typedef enum
{
    PQ_GRule_HDR_On_Main,
    PQ_GRule_HDR_Ref_Main,
    PQ_GRule_HDR_HDR_OFF_1920_Main,
    PQ_GRule_HDR_HDR_OFF_1366_Main,
    PQ_GRule_HDR_HDR_OFF_4K_Main,
    PQ_GRule_HDR_Dolby_ON_Main,
    PQ_GRule_HDR_Dolby_User_Main,
}
MST_GRule_HDR_Index_Main;
#endif

#if PQ_GRULE_3D_ENABLE
typedef enum
{
    PQ_GRule_3D_Off_Main,
    PQ_GRule_3D_On_Main,
}
MST_GRule_3D_Index_Main;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_NR_Off_Main,
    PQ_GRule_Lvl_NR_Low_Main,
    PQ_GRule_Lvl_NR_Middle_Main,
    PQ_GRule_Lvl_NR_High_Main,
    PQ_GRule_Lvl_NR_Low_3D_Main,
    PQ_GRule_Lvl_NR_Middle_3D_Main,
    PQ_GRule_Lvl_NR_High_3D_Main,
    PQ_GRule_Lvl_NR_Auto_Low_L_Main,
    PQ_GRule_Lvl_NR_Auto_Low_M_Main,
    PQ_GRule_Lvl_NR_Auto_Low_H_Main,
    PQ_GRule_Lvl_NR_Auto_Middle_L_Main,
    PQ_GRule_Lvl_NR_Auto_Middle_M_Main,
    PQ_GRule_Lvl_NR_Auto_Middle_H_Main,
    PQ_GRule_Lvl_NR_Auto_High_L_Main,
    PQ_GRule_Lvl_NR_Auto_High_M_Main,
    PQ_GRule_Lvl_NR_Auto_High_H_Main,
}
MST_GRule_NR_LvL_Index_Main;
#endif

#if PQ_GRULE_OSD_BW_ENABLE
typedef enum
{
    PQ_GRule_Lvl_OSD_BW_On_Main,
}
MST_GRule_OSD_BW_LvL_Index_Main;
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_MPEG_NR_Off_Main,
    PQ_GRule_Lvl_MPEG_NR_Low_Main,
    PQ_GRule_Lvl_MPEG_NR_Middle_Main,
    PQ_GRule_Lvl_MPEG_NR_High_Main,
}
MST_GRule_MPEG_NR_LvL_Index_Main;
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
typedef enum
{
    PQ_GRule_Lvl_FILM_MODE_Off_Main,
    PQ_GRule_Lvl_FILM_MODE_On_Main,
}
MST_GRule_FILM_MODE_LvL_Index_Main;
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_ULTRAT_CLEAR_Off_Main,
    PQ_GRule_Lvl_ULTRAT_CLEAR_On_Main,
}
MST_GRule_ULTRAT_CLEAR_LvL_Index_Main;
#endif

#if PQ_GRULE_PTP_ENABLE
typedef enum
{
    PQ_GRule_Lvl_PTP_On_Main,
}
MST_GRule_PTP_LvL_Index_Main;
#endif

#if PQ_GRULE_GAME_MODE_ENABLE
typedef enum
{
    PQ_GRule_Lvl_GAME_MODE_On_Main,
}
MST_GRule_GAME_MODE_LvL_Index_Main;
#endif

#if PQ_GRULE_DS_PARAMETERS_ENABLE
typedef enum
{
    PQ_GRule_Lvl_DS_PARAMETERS_L_SD_Main,
    PQ_GRule_Lvl_DS_PARAMETERS_SD_Main,
    PQ_GRule_Lvl_DS_PARAMETERS_FHD_Main,
    PQ_GRule_Lvl_DS_PARAMETERS_4K_Main,
}
MST_GRule_DS_PARAMETERS_LvL_Index_Main;
#endif

#if PQ_GRULE_HDR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_HDR_Off_Main,
    PQ_GRule_Lvl_HDR_Open_Auto_Main,
    PQ_GRule_Lvl_HDR_Open_High_Main,
    PQ_GRule_Lvl_HDR_Open_Mid_Main,
    PQ_GRule_Lvl_HDR_Open_Low_Main,
    PQ_GRule_Lvl_HDR_Open_Ref_Main,
    PQ_GRule_Lvl_HDR_Dolby_Vivid_Main,
    PQ_GRule_Lvl_HDR_Dolby_User_Main,
}
MST_GRule_HDR_LvL_Index_Main;
#endif

#if PQ_GRULE_3D_ENABLE
typedef enum
{
    PQ_GRule_Lvl_3D_Off_Main,
    PQ_GRule_Lvl_3D_On_Main,
}
MST_GRule_3D_LvL_Index_Main;
#endif


#define PQ_GRULE_RULE_NUM_Main 10

#if PQ_GRULE_NR_ENABLE
#define PQ_GRULE_NR_IP_NUM_Main 5
#define PQ_GRULE_NR_NUM_Main 22
#define PQ_GRULE_NR_LVL_NUM_Main 16
#endif

#if PQ_GRULE_OSD_BW_ENABLE
#define PQ_GRULE_OSD_BW_IP_NUM_Main 3
#define PQ_GRULE_OSD_BW_NUM_Main 9
#define PQ_GRULE_OSD_BW_LVL_NUM_Main 1
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
#define PQ_GRULE_MPEG_NR_IP_NUM_Main 7
#define PQ_GRULE_MPEG_NR_NUM_Main 4
#define PQ_GRULE_MPEG_NR_LVL_NUM_Main 4
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
#define PQ_GRULE_FILM_MODE_IP_NUM_Main 4
#define PQ_GRULE_FILM_MODE_NUM_Main 2
#define PQ_GRULE_FILM_MODE_LVL_NUM_Main 2
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
#define PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main 2
#define PQ_GRULE_ULTRAT_CLEAR_NUM_Main 3
#define PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main 2
#endif

#if PQ_GRULE_PTP_ENABLE
#define PQ_GRULE_PTP_IP_NUM_Main 2
#define PQ_GRULE_PTP_NUM_Main 1
#define PQ_GRULE_PTP_LVL_NUM_Main 1
#endif

#if PQ_GRULE_GAME_MODE_ENABLE
#define PQ_GRULE_GAME_MODE_IP_NUM_Main 13
#define PQ_GRULE_GAME_MODE_NUM_Main 1
#define PQ_GRULE_GAME_MODE_LVL_NUM_Main 1
#endif

#if PQ_GRULE_DS_PARAMETERS_ENABLE
#define PQ_GRULE_DS_PARAMETERS_IP_NUM_Main 8
#define PQ_GRULE_DS_PARAMETERS_NUM_Main 4
#define PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main 4
#endif

#if PQ_GRULE_HDR_ENABLE
#define PQ_GRULE_HDR_IP_NUM_Main 12
#define PQ_GRULE_HDR_NUM_Main 7
#define PQ_GRULE_HDR_LVL_NUM_Main 8
#endif

#if PQ_GRULE_3D_ENABLE
#define PQ_GRULE_3D_IP_NUM_Main 1
#define PQ_GRULE_3D_NUM_Main 2
#define PQ_GRULE_3D_LVL_NUM_Main 2
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_NR_IP_Index_Main[PQ_GRULE_NR_IP_NUM_Main];
extern code U8 MST_GRule_NR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_NR_NUM_Main][PQ_GRULE_NR_IP_NUM_Main];
#endif

#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_OSD_BW_IP_Index_Main[PQ_GRULE_OSD_BW_IP_NUM_Main];
extern code U8 MST_GRule_OSD_BW_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_OSD_BW_NUM_Main][PQ_GRULE_OSD_BW_IP_NUM_Main];
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_MPEG_NR_IP_Index_Main[PQ_GRULE_MPEG_NR_IP_NUM_Main];
extern code U8 MST_GRule_MPEG_NR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_MPEG_NR_NUM_Main][PQ_GRULE_MPEG_NR_IP_NUM_Main];
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_FILM_MODE_IP_Index_Main[PQ_GRULE_FILM_MODE_IP_NUM_Main];
extern code U8 MST_GRule_FILM_MODE_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_FILM_MODE_NUM_Main][PQ_GRULE_FILM_MODE_IP_NUM_Main];
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_ULTRAT_CLEAR_IP_Index_Main[PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main];
extern code U8 MST_GRule_ULTRAT_CLEAR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_ULTRAT_CLEAR_NUM_Main][PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main];
#endif

#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_PTP_IP_Index_Main[PQ_GRULE_PTP_IP_NUM_Main];
extern code U8 MST_GRule_PTP_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_PTP_NUM_Main][PQ_GRULE_PTP_IP_NUM_Main];
#endif

#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_GAME_MODE_IP_Index_Main[PQ_GRULE_GAME_MODE_IP_NUM_Main];
extern code U8 MST_GRule_GAME_MODE_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_GAME_MODE_NUM_Main][PQ_GRULE_GAME_MODE_IP_NUM_Main];
#endif

#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_DS_PARAMETERS_IP_Index_Main[PQ_GRULE_DS_PARAMETERS_IP_NUM_Main];
extern code U8 MST_GRule_DS_PARAMETERS_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_DS_PARAMETERS_NUM_Main][PQ_GRULE_DS_PARAMETERS_IP_NUM_Main];
#endif

#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_HDR_IP_Index_Main[PQ_GRULE_HDR_IP_NUM_Main];
extern code U8 MST_GRule_HDR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_HDR_NUM_Main][PQ_GRULE_HDR_IP_NUM_Main];
#endif

#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_3D_IP_Index_Main[PQ_GRULE_3D_IP_NUM_Main];
extern code U8 MST_GRule_3D_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_3D_NUM_Main][PQ_GRULE_3D_IP_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1920_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1920_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1920_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1920_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_1920_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1920_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_1920_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1920_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_1920_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_1920_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1920_PIP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1920_PIP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1920_PIP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1920_PIP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_1920_PIP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1920_PIP_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_1920_PIP_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1920_PIP_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_1920_PIP_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_1920_PIP_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1920_POP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1920_POP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1920_POP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1920_POP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_1920_POP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1920_POP_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_1920_POP_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1920_POP_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_1920_POP_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_1920_POP_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1366_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1366_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1366_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1366_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_1366_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1366_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_1366_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1366_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_1366_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_1366_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1366_PIP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1366_PIP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1366_PIP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1366_PIP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_1366_PIP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1366_PIP_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_1366_PIP_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1366_PIP_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_1366_PIP_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_1366_PIP_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1366_POP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1366_POP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1366_POP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1366_POP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_1366_POP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1366_POP_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_1366_POP_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1366_POP_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_1366_POP_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_1366_POP_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_4K2K_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_4K2K_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_4K2K_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_4K2K_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_4K2K_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_4K2K_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_4K2K_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_4K2K_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_4K2K_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_4K2K_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_4K2K_PIP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_4K2K_PIP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_4K2K_PIP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_4K2K_PIP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_4K2K_PIP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_4K2K_PIP_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_4K2K_PIP_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_4K2K_PIP_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_4K2K_PIP_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_4K2K_PIP_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_4K2K_POP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_4K2K_POP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_4K2K_POP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_4K2K_POP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern code U8 MST_GRule_4K2K_POP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_4K2K_POP_PTP_Main[PQ_GRULE_PTP_LVL_NUM_Main];
#endif
#if PQ_GRULE_GAME_MODE_ENABLE
extern code U8 MST_GRule_4K2K_POP_GAME_MODE_Main[PQ_GRULE_GAME_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_4K2K_POP_DS_PARAMETERS_Main[PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main];
#endif
#if PQ_GRULE_HDR_ENABLE
extern code U8 MST_GRule_4K2K_POP_HDR_Main[PQ_GRULE_HDR_LVL_NUM_Main];
#endif
#if PQ_GRULE_3D_ENABLE
extern code U8 MST_GRule_4K2K_POP_3D_Main[PQ_GRULE_3D_LVL_NUM_Main];
#endif

