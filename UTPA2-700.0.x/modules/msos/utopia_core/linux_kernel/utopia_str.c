#include "MsTypes.h"
#include "utopia.h"

MS_U32 UtopiaStrWaitCondition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage)
{
#if CONFIG_MSTAR_UTOPIA2K_STR
    extern int mdrv_utopia2k_str_wait_condition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage);
    int ret = mdrv_utopia2k_str_wait_condition(u8ModuleName, u32Mode, u32Stage);
    if (ret < 0)
    {
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return UTOPIA_STATUS_SUCCESS;
    }
#else
    return UTOPIA_STATUS_FAIL;
#endif
}

MS_U32 UtopiaStrSendCondition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage)
{
#if CONFIG_MSTAR_UTOPIA2K_STR
    extern int mdrv_utopia2k_str_send_condition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage);
    int ret = mdrv_utopia2k_str_send_condition(u8ModuleName, u32Mode, u32Stage);
    if (ret < 0)
    {
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return UTOPIA_STATUS_SUCCESS;
    }
#else
    return UTOPIA_STATUS_FAIL;
#endif
}

MS_U32 UtopiaStrSetData(char *key, char *value)
{
#if CONFIG_MSTAR_UTOPIA2K_STR
    extern int mdrv_utopia2k_str_set_data(char *key, char *value);
    int ret = mdrv_utopia2k_str_set_data(key, value);
    if (ret < 0)
    {
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return UTOPIA_STATUS_SUCCESS;
    }
#else
    return UTOPIA_STATUS_FAIL;
#endif
}

MS_U32 UtopiaStrGetData(char *key, char *value)
{
#if CONFIG_MSTAR_UTOPIA2K_STR
    extern int mdrv_utopia2k_str_get_data(char *key, char *value);
    int ret = mdrv_utopia2k_str_get_data(key, value);
    if (ret < 0)
    {
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return UTOPIA_STATUS_SUCCESS;
    }
#else
    return UTOPIA_STATUS_FAIL;
#endif
}
