//#include <nuttx/config.h>
#include <stdio.h>
//#include "MsTypes.h"
//#include "stdlib.h"
#include "utopia.h" // for utopia open/ioctl/close
#include "utopia_driver_id.h" // for module id
#include "MsOS.h" // for MsOS_ObtainMutex test
//#include "drvMMIO.h"
//#include <string.h>
//#include <unistd.h>
//#include "drvMIU.h"

#include "drvSYS.h"
#include "drvSYS_priv.h"

static const char optee_test[] = "opteearmtz00";
char *_device = (char *)optee_test;


TEEC_Context mstar_teec_ctx;
/* This UUID is generated with the ITU-T UUID generator at
   http://www.itu.int/ITU-T/asn1/uuid.html 
    7cac5480-a1fb-11e5-acb4-0002a5d5c51b */
#define MSTAR_INTERNAL_TEST_UUID {0x7cac5480, 0xa1fb, 0x11e5, \
                        {0xac, 0xb4, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }
#define SYS_TEEC_OPERATION_INITIALIZER { 0, 0, { { { 0 } } } }
#define CMD_TRACE   0
#define CMD_PARAMS  1
#define CMD_SELF_TESTS  2
#define CMD_PRINT   3
#define CMD_MULTIPLY_VALUE 4
#define CMD_MEMORY_WRITE 5
#define CMD_MEMORY_READ 6

int main(int argc, char *argv[])
{
	unsigned int ret, ret_orig= 0;
    TEEC_Session session = { 0 };
    TEEC_UUID uuid = MSTAR_INTERNAL_TEST_UUID;
    TEEC_Operation op = SYS_TEEC_OPERATION_INITIALIZER;
    int cmd = 0;
    uint8_t buf[32];

    if (argc < 2) {
        printf("./verify 0 or ./verify [0..5]\n");
        goto end_test;
    }
   
    cmd = atoi(argv[1]); 
    printf("cmd:%d\n",cmd);
    

    switch (cmd)
    { 
        case CMD_TRACE:
        case CMD_PARAMS:
        case CMD_SELF_TESTS:
        case CMD_PRINT:
            if (ret = MDrv_SYS_TEEC_InitializeContext(_device, &mstar_teec_ctx)) {
                printf("%s %d: Init Context failed\n",__func__,__LINE__);
                goto end_test;
            }
    
            if (ret = MDrv_SYS_TEEC_Open(&mstar_teec_ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, &op, &ret_orig)) {
                printf("%s %d: TEEC Open session failed\n",__func__,__LINE__);
                goto end_test;
            }
            if (ret = MDrv_SYS_TEEC_InvokeCmd(&session, cmd, &op, &ret_orig)) {
                printf("%s %d: TEEC Invoke command failed\n",__func__,__LINE__);
                goto end_test1;
            }
        break;
        case CMD_MULTIPLY_VALUE:
            if (argc < 4) {
                printf("./verify 4 num1 num2\n");
                printf("num1 is a numeric\n");
                printf("num2 is a numeric\n");
                goto end_test;
            }
            if (ret = MDrv_SYS_TEEC_InitializeContext(_device, &mstar_teec_ctx)) {
                printf("%s %d: Init Context failed\n",__func__,__LINE__);
                goto end_test;
            }
    
            if (ret = MDrv_SYS_TEEC_Open(&mstar_teec_ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, &op, &ret_orig)) {
                printf("%s %d: TEEC Open session failed\n",__func__,__LINE__);
                goto end_test;
            }
            
            op.params[0].value.a = atoi(argv[2]); 
            op.params[0].value.b = atoi(argv[3]);
            op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
                     TEEC_NONE, TEEC_NONE);
            
            if (ret = MDrv_SYS_TEEC_InvokeCmd(&session, CMD_MULTIPLY_VALUE, &op, &ret_orig)) {
                printf("%s %d: TEEC Invoke command failed\n",__func__,__LINE__);
                goto end_test1;
            }
            printf("%d * %d = %d\n",op.params[0].value.a, op.params[0].value.b, op.params[1].value.a);
        break; 
        case CMD_MEMORY_WRITE:
        case CMD_MEMORY_READ:
           {
            static uint8_t test_input[3] = {0x5A, 0xA5, 0x5A};
            static uint8_t test_out[3] = {0xBB, 0xCC, 0xDD};
            printf("test_input:%x, %x, %x, test_out:%x, %x, %x\n",test_input[0], test_input[1], test_input[2], test_out[0], test_out[1], test_out[2]); 
            if (ret = MDrv_SYS_TEEC_InitializeContext(_device, &mstar_teec_ctx)) {
                printf("%s %d: Init Context failed\n",__func__,__LINE__);
                goto end_test;
            }
    
            if (ret = MDrv_SYS_TEEC_Open(&mstar_teec_ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, &op, &ret_orig)) {
                printf("%s %d: TEEC Open session failed\n",__func__,__LINE__);
                goto end_test;
            }
            op.params[0].tmpref.buffer = test_input;
            op.params[0].tmpref.size = sizeof(test_input); 
            op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
                     TEEC_NONE, TEEC_NONE);
            if (ret = MDrv_SYS_TEEC_InvokeCmd(&session, CMD_MEMORY_WRITE, &op, &ret_orig)) {
                printf("%s %d: TEEC Invoke command failed\n",__func__,__LINE__);
                goto end_test1;
            }
            
            op.params[0].tmpref.buffer = test_out;
            op.params[0].tmpref.size = sizeof(test_out); 
            op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE,
                     TEEC_NONE, TEEC_NONE);
            if (ret = MDrv_SYS_TEEC_InvokeCmd(&session, CMD_MEMORY_READ, &op, &ret_orig)) {
                printf("%s %d: TEEC Invoke command failed\n",__func__,__LINE__);
                goto end_test1;
            }
            printf("test_input:%x, %x, %x, test_out:%x, %x, %x\n",test_input[0], test_input[1], test_input[2], test_out[0], test_out[1], test_out[2]); 
            }
        break;
    }

end_test1:
    MDrv_SYS_TEEC_Close(&session);
end_test:
printf("%s %d:ret:%u\n",__func__,__LINE__,ret);
    return 0;
}
