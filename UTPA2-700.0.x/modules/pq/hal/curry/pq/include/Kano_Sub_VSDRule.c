
code U8 MST_VSDRule_IP_Index_Sub[PQ_VSDRule_IP_NUM_Sub]=
{
    PQ_IP_VSD_Sub,
    PQ_IP_VSP_PreVBound_Sub,
};


code U8 MST_VSDRule_Array_Sub[PQ_VSDRule_NUM_Sub][PQ_VSDRule_IP_NUM_Sub]=
{
    {//PreV_ScalingDown_Interlace, 0
        PQ_IP_VSD_Bilinear_Sub, PQ_IP_VSP_PreVBound_ON_Sub, 
    },
    {//PreV_ScalingDown_Progressive, 1
        PQ_IP_VSD_CB_Sub, PQ_IP_VSP_PreVBound_ON_Sub, 
    },
    {//ScalingDown_00x_YUV, 2
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_00x_RGB, 3
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_01x_YUV, 4
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_01x_RGB, 5
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_02x_YUV, 6
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_02x_RGB, 7
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_03x_YUV, 8
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_03x_RGB, 9
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_04x_YUV, 10
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_04x_RGB, 11
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_05x_YUV, 12
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_05x_RGB, 13
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_06x_YUV, 14
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_06x_RGB, 15
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_07x_YUV, 16
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_07x_RGB, 17
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_08x_YUV, 18
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_08x_RGB, 19
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_09x_YUV, 20
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_09x_RGB, 21
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_10x_YUV, 22
        PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_10x_RGB, 23
        PQ_IP_NULL, PQ_IP_NULL, 
    },
};
