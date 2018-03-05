
#if PQ_GRULE_NR_ENABLE
code U8 MST_GRule_4K2K_POP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub]=
{
    PQ_GRule_NR_Off_Sub,
    PQ_GRule_NR_Low_4K2K_Sub,
    PQ_GRule_NR_Middle_4K2K_Sub,
    PQ_GRule_NR_High_4K2K_Sub,
    PQ_GRule_NR_Low_4K2K_Sub,
    PQ_GRule_NR_Middle_4K2K_Sub,
    PQ_GRule_NR_High_4K2K_Sub,
    PQ_GRule_NR_Low_4K2K_Sub,
    PQ_GRule_NR_Middle_4K2K_Sub,
    PQ_GRule_NR_High_4K2K_Sub,
    PQ_GRule_NR_Low_4K2K_Sub,
    PQ_GRule_NR_Middle_4K2K_Sub,
    PQ_GRule_NR_High_4K2K_Sub,
};
#endif


#if PQ_GRULE_OSD_BW_ENABLE
code U8 MST_GRule_4K2K_POP_OSD_BW_Sub[PQ_GRULE_OSD_BW_LVL_NUM_Sub]=
{
    PQ_GRule_OSD_BW_On_4K2K_POP_Sub,
};
#endif


#if PQ_GRULE_MPEG_NR_ENABLE
code U8 MST_GRule_4K2K_POP_MPEG_NR_Sub[PQ_GRULE_MPEG_NR_LVL_NUM_Sub]=
{
    PQ_GRule_MPEG_NR_Off_Sub,
    PQ_GRule_MPEG_NR_Low_Sub,
    PQ_GRule_MPEG_NR_Middle_Sub,
    PQ_GRule_MPEG_NR_High_Sub,
};
#endif


#if PQ_GRULE_FILM_MODE_ENABLE
code U8 MST_GRule_4K2K_POP_FILM_MODE_Sub[PQ_GRULE_FILM_MODE_LVL_NUM_Sub]=
{
    PQ_GRule_FILM_MODE_Off_Sub,
    PQ_GRule_FILM_MODE_On_Sub,
};
#endif


#if PQ_GRULE_PTP_ENABLE
code U8 MST_GRule_4K2K_POP_PTP_Sub[PQ_GRULE_PTP_LVL_NUM_Sub]=
{
    PQ_GRule_PTP_On_PTP_Sub,
};
#endif
