
#if PQ_QM_HMDI
static code char TEXT_INPUTTYPE_4K2K_RGB444_UFSC[] = "4K2K_RGB444"; //0
#endif
#if PQ_QM_HMDI
static code char TEXT_INPUTTYPE_4K2K_YUV444_UFSC[] = "4K2K_YUV444"; //1
#endif
#if PQ_QM_HMDI
static code char TEXT_INPUTTYPE_4K2K_YUV422_UFSC[] = "4K2K_YUV422"; //2
#endif
#if PQ_QM_HMDI
static code char TEXT_INPUTTYPE_FHD_RGB444_UFSC[] = "FHD_RGB444"; //3
#endif
#if PQ_QM_HMDI
static code char TEXT_INPUTTYPE_FHD_YUV444_UFSC[] = "FHD_YUV444"; //4
#endif
#if PQ_QM_HMDI
static code char TEXT_INPUTTYPE_FHD_YUV422_UFSC[] = "FHD_YUV422"; //5
#endif


code char* PQ_INPUTTYPE_TEXT_UFSC[]=
{
    #if PQ_QM_HMDI
    TEXT_INPUTTYPE_4K2K_RGB444_UFSC,//0
    #endif 
    #if PQ_QM_HMDI
    TEXT_INPUTTYPE_4K2K_YUV444_UFSC,//1
    #endif 
    #if PQ_QM_HMDI
    TEXT_INPUTTYPE_4K2K_YUV422_UFSC,//2
    #endif 
    #if PQ_QM_HMDI
    TEXT_INPUTTYPE_FHD_RGB444_UFSC,//3
    #endif 
    #if PQ_QM_HMDI
    TEXT_INPUTTYPE_FHD_YUV444_UFSC,//4
    #endif 
    #if PQ_QM_HMDI
    TEXT_INPUTTYPE_FHD_YUV422_UFSC,//5
    #endif 
};

