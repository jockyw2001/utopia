

typedef struct _KERNEL_MODULE_ADP UTOPIA2K_ADP;
typedef struct _UTOPIA_IOCTL_ARG UTOPIA_IOCTL_ARG;




struct _KERNEL_MODULE_ADP
{
    FUtopiaIOctl                  fpIoctl;
	void*                   pInstant;
};

struct _UTOPIA_IOCTL_ARG
{
   
	void*                   pModuleArg;
	MS_U32                  u32ModuleCmd;
};



