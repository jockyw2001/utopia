#include "MsTypes.h"
#include <stdio.h>
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvUART.h"
#include "drvUART_private.h"
#include "MsOS.h"

#include "ULog.h"

#define TAG_UART "UART"

enum
{
    UART_POOL_ID_UART=0
} eUARTPoolID;

void UARTRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_UART, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)UARTOpen, (FUtopiaClose)UARTClose, (FUtopiaIOctl)UARTIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, UART_POOL_ID_UART);
    UtopiaResourceCreate("UART", sizeof(UART_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, UART_POOL_ID_UART);
    UtopiaModuleAddResourceEnd(pUtopiaModule, UART_POOL_ID_UART);

    // FLASHRegisterToUtopia only call once, so driver init code can put here,
}

MS_U32 UARTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    UART_INSTANT_PRIVATE *pUartPri = NULL;
    UtopiaInstanceCreate(sizeof(UART_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pUartPri);

    // setup func in private and assign the calling func in func ptr in instance private

    pUartPri->fpUARTConnect = mdrv_uart_connect;
    pUartPri->fpUARTSetBaudRate = mdrv_uart_set_baudrate;
    pUartPri->fpUARTOpen= mdrv_uart_open;
    pUartPri->fpUARTClose = mdrv_uart_close;

    UART_INSTANT_ATTRIBUTE* pUartAttribue = (UART_INSTANT_ATTRIBUTE*) pAttribute;

    MDrv_UART_Init( pUartAttribue->eUartType, pUartAttribue->u32BaudRate);

    return TRUE;

}

MS_U32 UARTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    UART_INSTANT_PRIVATE* psUARTInstPri ;
    void* pResource = NULL;
    void* pModule = NULL;

    PUART_OPEN pUartOpen = NULL;
    PUART_CLOSE pUartClose = NULL;
    PUART_CONNECT pUartConnect = NULL;
    PUART_SET_BUAD_RATE pUartSetBaudRate = NULL;
    MS_U32 u32Ret;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psUARTInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);

    switch(u32Cmd)
    {
        case MDrv_CMD_UART_Open:
            pUartOpen = (PUART_OPEN)pArgs;
            if(UtopiaResourceObtain(pModule, UART_POOL_ID_UART, &pResource) != 0)
            {
                ULOGE(TAG_UART, "UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psUARTInstPri->fpUARTOpen(pUartOpen->eUartType);
            return u32Ret;

        case MDrv_CMD_UART_Close:
            pUartClose = (PUART_CLOSE)pArgs;
            if(UtopiaResourceObtain(pModule, UART_POOL_ID_UART, &pResource) != 0)
            {
                ULOGE(TAG_UART, "UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }

            u32Ret = (MS_U32)psUARTInstPri->fpUARTClose(pUartClose->u32Fd);
            return u32Ret;

        case MDrv_CMD_UART_Connect:
            pUartConnect = (PUART_CONNECT)pArgs;
            if(UtopiaResourceObtain(pModule, UART_POOL_ID_UART, &pResource) != 0)
            {
                ULOGE(TAG_UART, "UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }

            u32Ret = (MS_U32)psUARTInstPri->fpUARTConnect(pUartConnect->ePortType, pUartConnect->eDeviceType);
            return u32Ret;

        case MDrv_CMD_UART_SetBaudRate:
            pUartSetBaudRate = (PUART_SET_BUAD_RATE)pArgs;
            if(UtopiaResourceObtain(pModule, UART_POOL_ID_UART, &pResource) != 0)
            {
                ULOGE(TAG_UART, "UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }

            u32Ret = (MS_U32)psUARTInstPri->fpUARTSetBaudRate(pUartSetBaudRate->u32Uart, pUartSetBaudRate->u32BaudRate);
            return u32Ret;
        default:
            return 0;
    }
}

MS_U32 UARTClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);
    return TRUE;
}


