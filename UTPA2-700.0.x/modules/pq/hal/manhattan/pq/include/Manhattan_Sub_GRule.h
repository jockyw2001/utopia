#if (PQ_GRULE_NR_ENABLE) || (PQ_GRULE_OSD_BW_ENABLE) || (PQ_GRULE_MPEG_NR_ENABLE) || (PQ_GRULE_FILM_MODE_ENABLE) || (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE) || (PQ_GRULE_PTP_ENABLE)
typedef enum
{
#if PQ_GRULE_NR_ENABLE
    PQ_GRule_NR_Sub,
#endif

#if PQ_GRULE_OSD_BW_ENABLE
    PQ_GRule_OSD_BW_Sub,
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
    PQ_GRule_MPEG_NR_Sub,
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
    PQ_GRule_FILM_MODE_Sub,
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
    PQ_GRule_DYNAMIC_CONTRAST_Sub,
#endif

#if PQ_GRULE_PTP_ENABLE
    PQ_GRule_PTP_Sub,
#endif

}
MST_GRule_Index_Sub;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_NR_Off_Sub,
    PQ_GRule_NR_Low_Sub,
    PQ_GRule_NR_Middle_Sub,
    PQ_GRule_NR_High_Sub,
    PQ_GRule_NR_Auto_Low_L_Sub,
    PQ_GRule_NR_Auto_Low_M_Sub,
    PQ_GRule_NR_Auto_Low_H_Sub,
    PQ_GRule_NR_Auto_Middle_L_Sub,
    PQ_GRule_NR_Auto_Middle_M_Sub,
    PQ_GRule_NR_Auto_Middle_H_Sub,
    PQ_GRule_NR_Auto_High_L_Sub,
    PQ_GRule_NR_Auto_High_M_Sub,
    PQ_GRule_NR_Auto_High_H_Sub,
    PQ_GRule_NR_Low_PIP_Sub,
    PQ_GRule_NR_Middle_PIP_Sub,
    PQ_GRule_NR_High_PIP_Sub,
    PQ_GRule_NR_Low_4K2K_Sub,
    PQ_GRule_NR_Middle_4K2K_Sub,
    PQ_GRule_NR_High_4K2K_Sub,
}
MST_GRule_NR_Index_Sub;
#endif

#if PQ_GRULE_OSD_BW_ENABLE
typedef enum
{
    PQ_GRule_OSD_BW_On_1920_Sub,
    PQ_GRule_OSD_BW_On_1920_PIP_Sub,
    PQ_GRule_OSD_BW_On_1920_POP_Sub,
    PQ_GRule_OSD_BW_On_1366_Sub,
    PQ_GRule_OSD_BW_On_1366_PIP_Sub,
    PQ_GRule_OSD_BW_On_1366_POP_Sub,
    PQ_GRule_OSD_BW_On_4K2K_Sub,
    PQ_GRule_OSD_BW_On_4K2K_PIP_Sub,
    PQ_GRule_OSD_BW_On_4K2K_POP_Sub,
}
MST_GRule_OSD_BW_Index_Sub;
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_MPEG_NR_Off_Sub,
    PQ_GRule_MPEG_NR_Low_Sub,
    PQ_GRule_MPEG_NR_Middle_Sub,
    PQ_GRule_MPEG_NR_High_Sub,
}
MST_GRule_MPEG_NR_Index_Sub;
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
typedef enum
{
    PQ_GRule_FILM_MODE_Off_Sub,
    PQ_GRule_FILM_MODE_On_Sub,
}
MST_GRule_FILM_MODE_Index_Sub;
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
typedef enum
{
    PQ_GRule_DYNAMIC_CONTRAST_Off_Sub,
    PQ_GRule_DYNAMIC_CONTRAST_On_Sub,
}
MST_GRule_DYNAMIC_CONTRAST_Index_Sub;
#endif

#if PQ_GRULE_PTP_ENABLE
typedef enum
{
    PQ_GRule_PTP_On_PTP_Sub,
}
MST_GRule_PTP_Index_Sub;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_NR_Off_Sub,
    PQ_GRule_Lvl_NR_Low_Sub,
    PQ_GRule_Lvl_NR_Middle_Sub,
    PQ_GRule_Lvl_NR_High_Sub,
    PQ_GRule_Lvl_NR_Auto_Low_L_Sub,
    PQ_GRule_Lvl_NR_Auto_Low_M_Sub,
    PQ_GRule_Lvl_NR_Auto_Low_H_Sub,
    PQ_GRule_Lvl_NR_Auto_Middle_L_Sub,
    PQ_GRule_Lvl_NR_Auto_Middle_M_Sub,
    PQ_GRule_Lvl_NR_Auto_Middle_H_Sub,
    PQ_GRule_Lvl_NR_Auto_High_L_Sub,
    PQ_GRule_Lvl_NR_Auto_High_M_Sub,
    PQ_GRule_Lvl_NR_Auto_High_H_Sub,
}
MST_GRule_NR_LvL_Index_Sub;
#endif

#if PQ_GRULE_OSD_BW_ENABLE
typedef enum
{
    PQ_GRule_Lvl_OSD_BW_On_Sub,
}
MST_GRule_OSD_BW_LvL_Index_Sub;
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_MPEG_NR_Off_Sub,
    PQ_GRule_Lvl_MPEG_NR_Low_Sub,
    PQ_GRule_Lvl_MPEG_NR_Middle_Sub,
    PQ_GRule_Lvl_MPEG_NR_High_Sub,
}
MST_GRule_MPEG_NR_LvL_Index_Sub;
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
typedef enum
{
    PQ_GRule_Lvl_FILM_MODE_Off_Sub,
    PQ_GRule_Lvl_FILM_MODE_On_Sub,
}
MST_GRule_FILM_MODE_LvL_Index_Sub;
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
typedef enum
{
    PQ_GRule_Lvl_DYNAMIC_CONTRAST_Off_Sub,
    PQ_GRule_Lvl_DYNAMIC_CONTRAST_On_Sub,
}
MST_GRule_DYNAMIC_CONTRAST_LvL_Index_Sub;
#endif

#if PQ_GRULE_PTP_ENABLE
typedef enum
{
    PQ_GRule_Lvl_PTP_On_Sub,
}
MST_GRule_PTP_LvL_Index_Sub;
#endif


#define PQ_GRULE_RULE_NUM_Sub 6

#if PQ_GRULE_NR_ENABLE
#define PQ_GRULE_NR_IP_NUM_Sub 8
#define PQ_GRULE_NR_NUM_Sub 19
#define PQ_GRULE_NR_LVL_NUM_Sub 13
#endif

#if PQ_GRULE_OSD_BW_ENABLE
#define PQ_GRULE_OSD_BW_IP_NUM_Sub 2
#define PQ_GRULE_OSD_BW_NUM_Sub 9
#define PQ_GRULE_OSD_BW_LVL_NUM_Sub 1
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
#define PQ_GRULE_MPEG_NR_IP_NUM_Sub 6
#define PQ_GRULE_MPEG_NR_NUM_Sub 4
#define PQ_GRULE_MPEG_NR_LVL_NUM_Sub 4
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
#define PQ_GRULE_FILM_MODE_IP_NUM_Sub 3
#define PQ_GRULE_FILM_MODE_NUM_Sub 2
#define PQ_GRULE_FILM_MODE_LVL_NUM_Sub 2
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
#define PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Sub 1
#define PQ_GRULE_DYNAMIC_CONTRAST_NUM_Sub 2
#define PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub 2
#endif

#if PQ_GRULE_PTP_ENABLE
#define PQ_GRULE_PTP_IP_NUM_Sub 2
#define PQ_GRULE_PTP_NUM_Sub 1
#define PQ_GRULE_PTP_LVL_NUM_Sub 1
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_NR_IP_Index_Sub[PQ_GRULE_NR_IP_NUM_Sub];
extern code U8 MST_GRule_NR_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_NR_NUM_Sub][PQ_GRULE_NR_IP_NUM_Sub];
#endif

#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_OSD_BW_IP_Index_Sub[PQ_GRULE_OSD_BW_IP_NUM_Sub];
extern code U8 MST_GRule_OSD_BW_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_OSD_BW_NUM_Sub][PQ_GRULE_OSD_BW_IP_NUM_Sub];
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_MPEG_NR_IP_Index_Sub[PQ_GRULE_MPEG_NR_IP_NUM_Sub];
extern code U8 MST_GRule_MPEG_NR_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_MPEG_NR_NUM_Sub][PQ_GRULE_MPEG_NR_IP_NUM_Sub];
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_FILM_MODE_IP_Index_Sub[PQ_GRULE_FILM_MODE_IP_NUM_Sub];
extern code U8 MST_GRule_FILM_MODE_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_FILM_MODE_NUM_Sub][PQ_GRULE_FILM_MODE_IP_NUM_Sub];
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_DYNAMIC_CONTRAST_IP_Index_Sub[PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Sub];
extern code U8 MST_GRule_DYNAMIC_CONTRAST_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_DYNAMIC_CONTRAST_NUM_Sub][PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Sub];
#endif

#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_PTP_IP_Index_Sub[PQ_GRULE_PTP_IP_NUM_Sub];
extern code U8 MST_GRule_PTP_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_PTP_NUM_Sub][PQ_GRULE_PTP_IP_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1920_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1920_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1920_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1920_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1920_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1920_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1920_PIP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1920_PIP_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1920_PIP_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1920_PIP_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1920_PIP_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1920_PIP_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1920_POP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1920_POP_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1920_POP_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1920_POP_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1920_POP_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1920_POP_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1366_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1366_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1366_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1366_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1366_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1366_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1366_PIP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1366_PIP_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1366_PIP_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1366_PIP_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1366_PIP_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1366_PIP_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_1366_POP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_1366_POP_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_1366_POP_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_1366_POP_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1366_POP_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_1366_POP_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_4K2K_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_4K2K_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_4K2K_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_4K2K_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_4K2K_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_4K2K_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_4K2K_PIP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_4K2K_PIP_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_4K2K_PIP_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_4K2K_PIP_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_4K2K_PIP_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_4K2K_PIP_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern code U8 MST_GRule_4K2K_POP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern code U8 MST_GRule_4K2K_POP_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern code U8 MST_GRule_4K2K_POP_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern code U8 MST_GRule_4K2K_POP_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_4K2K_POP_DYNAMIC_CONTRAST_Sub[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Sub];
#endif
#if PQ_GRULE_PTP_ENABLE
extern code U8 MST_GRule_4K2K_POP_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub];
#endif

