#define PQ_GRULE_UFSC_MPEG_NR_ENABLE 1
#define PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE 1
#define PQ_GRULE_UFSC_SR_CONTROL_ENABLE 1
#define PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE 1
#define PQ_GRULE_UFSC_HDR_ENABLE 1
#define PQ_GRULE_DEFINE_AUTO_GEN 1
#if (PQ_GRULE_UFSC_MPEG_NR_ENABLE) || (PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE) || (PQ_GRULE_UFSC_SR_CONTROL_ENABLE) || (PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) || (PQ_GRULE_UFSC_HDR_ENABLE)
typedef enum
{
#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
    PQ_GRule_MPEG_NR_UFSC,
#endif

#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
    PQ_GRule_DYNAMIC_CONTRAST_UFSC,
#endif

#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
    PQ_GRule_SR_CONTROL_UFSC,
#endif

#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
    PQ_GRule_DS_PARAMETERS_UFSC,
#endif

#if PQ_GRULE_UFSC_HDR_ENABLE
    PQ_GRule_HDR_UFSC,
#endif

}
MST_GRule_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_MPEG_NR_Off_UFSC,
    PQ_GRule_MPEG_NR_Low_UFSC,
    PQ_GRule_MPEG_NR_Middle_UFSC,
    PQ_GRule_MPEG_NR_High_UFSC,
}
MST_GRule_MPEG_NR_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
typedef enum
{
    PQ_GRule_DYNAMIC_CONTRAST_Off_UFSC,
    PQ_GRule_DYNAMIC_CONTRAST_On_UFSC,
}
MST_GRule_DYNAMIC_CONTRAST_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
typedef enum
{
    PQ_GRule_SR_CONTROL_Off_UFSC,
    PQ_GRule_SR_CONTROL_Low_UFSC,
    PQ_GRule_SR_CONTROL_Middle_UFSC,
    PQ_GRule_SR_CONTROL_On_High_UFSC,
}
MST_GRule_SR_CONTROL_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
typedef enum
{
    PQ_GRule_DS_PARAMETERS_L_SD_UFSC,
    PQ_GRule_DS_PARAMETERS_SD_UFSC,
    PQ_GRule_DS_PARAMETERS_FHD_UFSC,
    PQ_GRule_DS_PARAMETERS_4K_UFSC,
}
MST_GRule_DS_PARAMETERS_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_HDR_ENABLE
typedef enum
{
    PQ_GRule_HDR_On_UFSC,
    PQ_GRule_HDR_Ref_UFSC,
    PQ_GRule_HDR_HDR_OFF_1920_UFSC,
    PQ_GRule_HDR_HDR_OFF_1366_UFSC,
    PQ_GRule_HDR_HDR_OFF_4K_UFSC,
    PQ_GRule_HDR_Dolby_ON_UFSC,
    PQ_GRule_HDR_Dolby_User_UFSC,
}
MST_GRule_HDR_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_MPEG_NR_Off_UFSC,
    PQ_GRule_Lvl_MPEG_NR_Low_UFSC,
    PQ_GRule_Lvl_MPEG_NR_Middle_UFSC,
    PQ_GRule_Lvl_MPEG_NR_High_UFSC,
}
MST_GRule_MPEG_NR_LvL_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
typedef enum
{
    PQ_GRule_Lvl_DYNAMIC_CONTRAST_Off_UFSC,
    PQ_GRule_Lvl_DYNAMIC_CONTRAST_On_UFSC,
}
MST_GRule_DYNAMIC_CONTRAST_LvL_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
typedef enum
{
    PQ_GRule_Lvl_SR_CONTROL_Off_UFSC,
    PQ_GRule_Lvl_SR_CONTROL_Low_UFSC,
    PQ_GRule_Lvl_SR_CONTROL_Middle_UFSC,
    PQ_GRule_Lvl_SR_CONTROL_High_UFSC,
}
MST_GRule_SR_CONTROL_LvL_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
typedef enum
{
    PQ_GRule_Lvl_DS_PARAMETERS_L_SD_UFSC,
    PQ_GRule_Lvl_DS_PARAMETERS_SD_UFSC,
    PQ_GRule_Lvl_DS_PARAMETERS_FHD_UFSC,
    PQ_GRule_Lvl_DS_PARAMETERS_4K_UFSC,
}
MST_GRule_DS_PARAMETERS_LvL_Index_UFSC;
#endif

#if PQ_GRULE_UFSC_HDR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_HDR_Off_UFSC,
    PQ_GRule_Lvl_HDR_Open_Auto_UFSC,
    PQ_GRule_Lvl_HDR_Open_High_UFSC,
    PQ_GRule_Lvl_HDR_Open_Mid_UFSC,
    PQ_GRule_Lvl_HDR_Open_Low_UFSC,
    PQ_GRule_Lvl_HDR_Open_Ref_UFSC,
    PQ_GRule_Lvl_HDR_Dolby_Vivid_UFSC,
    PQ_GRule_Lvl_HDR_Dolby_User_UFSC,
    PQ_GRule_Lvl_HDR_Dolby_Brightness_UFSC,
    PQ_GRule_Lvl_HDR_Dolby_Dark_UFSC,
}
MST_GRule_HDR_LvL_Index_UFSC;
#endif


#define PQ_GRULE_RULE_NUM_UFSC 5

#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
#define PQ_GRULE_MPEG_NR_IP_NUM_UFSC 3
#define PQ_GRULE_MPEG_NR_NUM_UFSC 4
#define PQ_GRULE_MPEG_NR_LVL_NUM_UFSC 4
#endif

#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
#define PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_UFSC 1
#define PQ_GRULE_DYNAMIC_CONTRAST_NUM_UFSC 2
#define PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_UFSC 2
#endif

#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
#define PQ_GRULE_SR_CONTROL_IP_NUM_UFSC 6
#define PQ_GRULE_SR_CONTROL_NUM_UFSC 4
#define PQ_GRULE_SR_CONTROL_LVL_NUM_UFSC 4
#endif

#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
#define PQ_GRULE_DS_PARAMETERS_IP_NUM_UFSC 5
#define PQ_GRULE_DS_PARAMETERS_NUM_UFSC 4
#define PQ_GRULE_DS_PARAMETERS_LVL_NUM_UFSC 4
#endif

#if PQ_GRULE_UFSC_HDR_ENABLE
#define PQ_GRULE_HDR_IP_NUM_UFSC 5
#define PQ_GRULE_HDR_NUM_UFSC 7
#define PQ_GRULE_HDR_LVL_NUM_UFSC 10
#endif

#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
extern code U8 MST_GRule_MPEG_NR_IP_Index_UFSC[PQ_GRULE_MPEG_NR_IP_NUM_UFSC];
extern code U8 MST_GRule_MPEG_NR_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_MPEG_NR_NUM_UFSC][PQ_GRULE_MPEG_NR_IP_NUM_UFSC];
#endif

#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_DYNAMIC_CONTRAST_IP_Index_UFSC[PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_UFSC];
extern code U8 MST_GRule_DYNAMIC_CONTRAST_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_DYNAMIC_CONTRAST_NUM_UFSC][PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_UFSC];
#endif

#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
extern code U8 MST_GRule_SR_CONTROL_IP_Index_UFSC[PQ_GRULE_SR_CONTROL_IP_NUM_UFSC];
extern code U8 MST_GRule_SR_CONTROL_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_SR_CONTROL_NUM_UFSC][PQ_GRULE_SR_CONTROL_IP_NUM_UFSC];
#endif

#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_DS_PARAMETERS_IP_Index_UFSC[PQ_GRULE_DS_PARAMETERS_IP_NUM_UFSC];
extern code U8 MST_GRule_DS_PARAMETERS_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_DS_PARAMETERS_NUM_UFSC][PQ_GRULE_DS_PARAMETERS_IP_NUM_UFSC];
#endif

#if PQ_GRULE_UFSC_HDR_ENABLE
extern code U8 MST_GRule_HDR_IP_Index_UFSC[PQ_GRULE_HDR_IP_NUM_UFSC];
extern code U8 MST_GRule_HDR_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_HDR_NUM_UFSC][PQ_GRULE_HDR_IP_NUM_UFSC];
#endif

#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
extern code U8 MST_GRule_1920_MPEG_NR_UFSC[PQ_GRULE_MPEG_NR_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1920_DYNAMIC_CONTRAST_UFSC[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
extern code U8 MST_GRule_1920_SR_CONTROL_UFSC[PQ_GRULE_SR_CONTROL_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1920_DS_PARAMETERS_UFSC[PQ_GRULE_DS_PARAMETERS_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_HDR_ENABLE
extern code U8 MST_GRule_1920_HDR_UFSC[PQ_GRULE_HDR_LVL_NUM_UFSC];
#endif

#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
extern code U8 MST_GRule_1366_MPEG_NR_UFSC[PQ_GRULE_MPEG_NR_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_1366_DYNAMIC_CONTRAST_UFSC[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
extern code U8 MST_GRule_1366_SR_CONTROL_UFSC[PQ_GRULE_SR_CONTROL_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_1366_DS_PARAMETERS_UFSC[PQ_GRULE_DS_PARAMETERS_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_HDR_ENABLE
extern code U8 MST_GRule_1366_HDR_UFSC[PQ_GRULE_HDR_LVL_NUM_UFSC];
#endif

#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
extern code U8 MST_GRule_4K_MPEG_NR_UFSC[PQ_GRULE_MPEG_NR_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
extern code U8 MST_GRule_4K_DYNAMIC_CONTRAST_UFSC[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
extern code U8 MST_GRule_4K_SR_CONTROL_UFSC[PQ_GRULE_SR_CONTROL_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
extern code U8 MST_GRule_4K_DS_PARAMETERS_UFSC[PQ_GRULE_DS_PARAMETERS_LVL_NUM_UFSC];
#endif
#if PQ_GRULE_UFSC_HDR_ENABLE
extern code U8 MST_GRule_4K_HDR_UFSC[PQ_GRULE_HDR_LVL_NUM_UFSC];
#endif
