#ifndef _SYS_REG_H_
#define _SYS_REG_H_

#include "MsTypes.h"

#define REG_CHIPTOP_OFFSET        0x101e00
#define REG_CHIPTOP_REG_NUM       (128)

#define REG_CHIPTOP_REG(x)        (REG_CHIPTOP_OFFSET+(x*2))
#define REG_TS_MODE_OFFSET        (REG_CHIPTOP_OFFSET+(0x02*2))    //reg_ts0_mode
#define REG_TS0_MODE_SHIFT        0x0
#define REG_TS0_MODE_MASK         0x3
#define REG_TS1_MODE_SHIFT        0x2
#define REG_TS1_MODE_MASK         0x3
#define REG_TS2_MODE_SHIFT        0x4
#define REG_TS2_MODE_MASK         0x7
#define REG_TS_MODE_1P            0x1
#define REG_TS_MODE_1S            0x5
#define REG_TS_MODE_MSPI          0x0

#define REG_MCU_JTAG_MODE_OFFSET  (REG_CHIPTOP_OFFSET+(0x04*2))    //reg_mcu_jtag_mode
#define REG_MSPI1_MODE1_SHIFT     0x6
#define REG_MSPI1_MODE1_MASK      0x3
#define REG_MSPI1_MODE1_TS0       0x2
#define REG_MSPI1_MODE2_SHIFT     0x8
#define REG_MSPI1_MODE2_MASK      0x3
#define REG_MSPI1_MODE3_SHIFT     10
#define REG_MSPI1_MODE3_MASK      0x3
#define REG_MSPI1_MODE1_TS1       0x1
#define REG_MSPI1_MODE1_TS2       0x2


#define REG_PWM0_MODE_OFFSET     (REG_CHIPTOP_OFFSET+(0x07*2))    //reg_pwm0_mode
#define REG_TS_OUT_MODE_SHIFT     14
#define REG_TS_OUT_MODE_MASK      0x3
#define REG_TS_OUT_MODE_TSO      0x1
#define REG_TS_OUT_MODE_S2P      0x2
#define REG_TS_OUT_MODE_S2P1     0x3


#define REG_I2CCM_MODE_OFFSET     (REG_CHIPTOP_OFFSET+(0x09*2))    //reg_i2cm0_mode
#define REG_FUART_MODE_SHIFT     10
#define REG_FUART_MODE_MASK      0x3

#define REG_TS0_3_WIRE_EN_OFFSET     (REG_CHIPTOP_OFFSET+(0x11*2))    //reg_ts0_3_wire_en
#define REG_TS0_CLK_DRV             0x4
#define REG_TS1_CLK_DRV             0x8
#define REG_TS0_D0_DRV              0x10
#define REG_TS1_D0_DRV              0x20
#define REG_TS0_SYNC_DRV            0x40
#define REG_TS1_SYNC_DRV            0x80
#define REG_TS0_VLD_DRV             0x100
#define REG_TS1_VLD_DRV             0x200


#endif
