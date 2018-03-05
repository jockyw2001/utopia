
#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
code U8 MST_GRule_MPEG_NR_IP_Index_UFSC[PQ_GRULE_MPEG_NR_IP_NUM_UFSC]=
{
    PQ_IP_DMS_UFSC,
    PQ_IP_DMS_H_UFSC,
    PQ_IP_DMS_V_UFSC,
};
#endif


#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
code U8 MST_GRule_DYNAMIC_CONTRAST_IP_Index_UFSC[PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_UFSC]=
{
    PQ_IP_VIP_DLC_UFSC,
};
#endif


#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
code U8 MST_GRule_SR_CONTROL_IP_Index_UFSC[PQ_GRULE_SR_CONTROL_IP_NUM_UFSC]=
{
    PQ_IP_PreEnhance_PK3x3_UFSC,
    PQ_IP_AA_UFSC,
    PQ_IP_ERC_UFSC,
    PQ_IP_P2M_UFSC,
    PQ_IP_DS_UFSC,
    PQ_IP_VIP_Post3x3PK_UFSC,
};
#endif


#if PQ_GRULE_UFSC_MPEG_NR_ENABLE
code U8 MST_GRule_MPEG_NR_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_MPEG_NR_NUM_UFSC][PQ_GRULE_MPEG_NR_IP_NUM_UFSC]=
{
    #if PQ_QM_HMDI
    {//4K2K_RGB444, 0
        {//Off
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Low
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Middle
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//High
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//4K2K_YUV444, 1
        {//Off
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Low
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Middle
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//High
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//4K2K_YUV422, 2
        {//Off
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Low
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Middle
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//High
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_RGB444, 3
        {//Off
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Low
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Middle
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//High
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_YUV444, 4
        {//Off
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Low
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Middle
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//High
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_YUV422, 5
        {//Off
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Low
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//Middle
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
        {//High
            PQ_IP_DMS_OFF_UFSC,
            PQ_IP_DMS_H_OFF_UFSC,
            PQ_IP_DMS_V_OFF_UFSC,
        },
    },
    #endif
};
#endif


#if PQ_GRULE_UFSC_DYNAMIC_CONTRAST_ENABLE
code U8 MST_GRule_DYNAMIC_CONTRAST_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_DYNAMIC_CONTRAST_NUM_UFSC][PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_UFSC]=
{
    #if PQ_QM_HMDI
    {//4K2K_RGB444, 0
        {//Off
            PQ_IP_VIP_DLC_OFF_UFSC,
        },
        {//On
            PQ_IP_VIP_DLC_ON_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//4K2K_YUV444, 1
        {//Off
            PQ_IP_VIP_DLC_OFF_UFSC,
        },
        {//On
            PQ_IP_VIP_DLC_ON_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//4K2K_YUV422, 2
        {//Off
            PQ_IP_VIP_DLC_OFF_UFSC,
        },
        {//On
            PQ_IP_VIP_DLC_ON_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_RGB444, 3
        {//Off
            PQ_IP_VIP_DLC_OFF_UFSC,
        },
        {//On
            PQ_IP_VIP_DLC_ON_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_YUV444, 4
        {//Off
            PQ_IP_VIP_DLC_OFF_UFSC,
        },
        {//On
            PQ_IP_VIP_DLC_ON_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_YUV422, 5
        {//Off
            PQ_IP_VIP_DLC_OFF_UFSC,
        },
        {//On
            PQ_IP_VIP_DLC_ON_UFSC,
        },
    },
    #endif
};
#endif


#if PQ_GRULE_UFSC_SR_CONTROL_ENABLE
code U8 MST_GRule_SR_CONTROL_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_GRULE_SR_CONTROL_NUM_UFSC][PQ_GRULE_SR_CONTROL_IP_NUM_UFSC]=
{
    #if PQ_QM_HMDI
    {//4K2K_RGB444, 0
        {//Off
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_OFF_UFSC,
            PQ_IP_DS_OFF_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Low
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_SR_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Middle
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W0_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//On_High
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W1_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//4K2K_YUV444, 1
        {//Off
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_OFF_UFSC,
            PQ_IP_DS_OFF_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Low
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_SR_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Middle
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W0_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//On_High
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W1_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//4K2K_YUV422, 2
        {//Off
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_OFF_UFSC,
            PQ_IP_DS_OFF_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Low
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_SR_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Middle
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W0_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//On_High
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W1_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_RGB444, 3
        {//Off
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_OFF_UFSC,
            PQ_IP_DS_OFF_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Low
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_SR_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Middle
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W0_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//On_High
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W1_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_YUV444, 4
        {//Off
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_OFF_UFSC,
            PQ_IP_DS_OFF_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Low
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_SR_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Middle
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W0_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//On_High
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W1_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
    },
    #endif
    #if PQ_QM_HMDI
    {//FHD_YUV422, 5
        {//Off
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_OFF_UFSC,
            PQ_IP_DS_OFF_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Low
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_OFF_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_SR_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//Middle
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W0_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
        {//On_High
            PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
            PQ_IP_AA_S1_UFSC,
            PQ_IP_ERC_OFF_UFSC,
            PQ_IP_P2M_W1_UFSC,
            PQ_IP_DS_W1_UFSC,
            PQ_IP_VIP_Post3x3PK_OFF_UFSC,
        },
    },
    #endif
};
#endif

