#define __BIT(x)    ((MS_U16)(1UL << (x)))
#define __BIT0       __BIT(0UL)
#define __BIT1       __BIT(1UL)
#define __BIT2       __BIT(2UL)
#define __BIT3       __BIT(3UL)
#define __BIT4       __BIT(4UL)
#define __BIT5       __BIT(5UL)
#define __BIT6       __BIT(6UL)
#define __BIT7       __BIT(7UL)
#define __BIT8       __BIT(8UL)
#define __BIT9       __BIT(9UL)
#define __BIT10      __BIT(10UL)
#define __BIT11      __BIT(11UL)
#define __BIT12      __BIT(12UL)
#define __BIT13      __BIT(13UL)
#define __BIT14      __BIT(14UL)
#define __BIT15      __BIT(15UL)


#define PM_REG_BASE                  (0x0E00UL)
#define IR_TX_REG_BASE               (0x1800UL)
#define IR_TX_MEM_BASE               (0x1900UL)

#define PM_SET_REG(x)		     	 (PM_REG_BASE+(x))
#define IR_TX_SET_REG(x)             (IR_TX_REG_BASE+(x))
#define IR_TX_MEM_SET_REG(x)         (IR_TX_MEM_BASE+(x))



#define PM_REG_Set_IRTX_MODE            PM_SET_REG(0x4eUL)
//---------------------------------------------
// definition for IR TX control
//---------------------------------------------

#define IR_TX_REG_Unit00_ShotCount_H    IR_TX_SET_REG(0x00UL)
#define IR_TX_REG_Unit00_ShotCount_L    IR_TX_SET_REG(0x02UL)
#define IR_TX_REG_Unit01_ShotCount_H    IR_TX_SET_REG(0x04UL)
#define IR_TX_REG_Unit01_ShotCount_L    IR_TX_SET_REG(0x06UL)
#define IR_TX_REG_Unit02_ShotCount_H    IR_TX_SET_REG(0x08UL)
#define IR_TX_REG_Unit02_ShotCount_L    IR_TX_SET_REG(0x0aUL)
#define IR_TX_REG_Unit03_ShotCount_H    IR_TX_SET_REG(0x0cUL)
#define IR_TX_REG_Unit03_ShotCount_L    IR_TX_SET_REG(0x0eUL)

#define IR_TX_REG_Unit04_ShotCount_H    IR_TX_SET_REG(0x10UL)
#define IR_TX_REG_Unit04_ShotCount_L    IR_TX_SET_REG(0x12UL)
#define IR_TX_REG_Unit05_ShotCount_H    IR_TX_SET_REG(0x14UL)
#define IR_TX_REG_Unit05_ShotCount_L    IR_TX_SET_REG(0x16UL)
#define IR_TX_REG_Unit06_ShotCount_H    IR_TX_SET_REG(0x18UL)
#define IR_TX_REG_Unit06_ShotCount_L    IR_TX_SET_REG(0x1aUL)
#define IR_TX_REG_Unit07_ShotCount_H    IR_TX_SET_REG(0x1cUL)
#define IR_TX_REG_Unit07_ShotCount_L    IR_TX_SET_REG(0x1eUL)

#define IR_TX_REG_Unit08_ShotCount_H    IR_TX_SET_REG(0x20UL)
#define IR_TX_REG_Unit08_ShotCount_L    IR_TX_SET_REG(0x22UL)
#define IR_TX_REG_Unit09_ShotCount_H    IR_TX_SET_REG(0x24UL)
#define IR_TX_REG_Unit09_ShotCount_L    IR_TX_SET_REG(0x26UL)
#define IR_TX_REG_Unit10_ShotCount_H    IR_TX_SET_REG(0x28UL)
#define IR_TX_REG_Unit10_ShotCount_L    IR_TX_SET_REG(0x2aUL)
#define IR_TX_REG_Unit11_ShotCount_H    IR_TX_SET_REG(0x2cUL)
#define IR_TX_REG_Unit11_ShotCount_L    IR_TX_SET_REG(0x2eUL)

#define IR_TX_REG_Unit12_ShotCount_H    IR_TX_SET_REG(0x30UL)
#define IR_TX_REG_Unit12_ShotCount_L    IR_TX_SET_REG(0x32UL)
#define IR_TX_REG_Unit13_ShotCount_H    IR_TX_SET_REG(0x34UL)
#define IR_TX_REG_Unit13_ShotCount_L    IR_TX_SET_REG(0x36UL)

//------IR_TX_REG_Unit00_ShoutCount_H---------------------------------------
// definIR_TX_REG_Unit00_ShoutCount_Lition for IR TX control
//---------------------------------------------


//------IR_TX_REG_Unit00_Value_LIR_TX_REG_Unit00_ShoutCount_L---------------------------------------
// definition for IR TX control
//---------------------------------------------
#define IR_TX_MODE         __BIT12


#define IR_TX_EN           __BIT0
#define IR_TX_RESET        __BIT1
#define IR_TX_INT_EN       __BIT2
#define IR_TX_OUT_INV      __BIT3
#define IR_TX_CARRIER_EN   __BIT4

#define IR_TX_TRIGGER      __BIT0
#define IR_TX_DONE         __BIT0
#define IR_TX_CLEAR_DONE   __BIT0

//---------------------------------------------
// definition for IR TX status
//---------------------------------------------
#define IR_TX_REG_Unit_Value_H    IR_TX_SET_REG(0x38UL)
#define IR_TX_REG_Unit_Value_L    IR_TX_SET_REG(0x3AUL)
#define IR_TX_FLAG                IR_TX_SET_REG(0x46UL)
#define IR_TX_STATUS              IR_TX_SET_REG(0x44UL)
#define IR_TX_REG_CycleTime_H     IR_TX_SET_REG(0x3CUL)
#define IR_TX_REG_CycleTime_L     IR_TX_SET_REG(0x3EUL)
#define IR_TX_REG_ClkDiv          IR_TX_SET_REG(0x40UL)
#define IR_TX_CARRIER_CNT         IR_TX_SET_REG(0x42UL)

#define IR_TX_MEM_REG_ADDR     IR_TX_MEM_SET_REG(0x04UL)
#define IR_TX_MEM_REG_DATA     IR_TX_MEM_SET_REG(0x0CUL)
#define IR_TX_MEM_REG_STATUS   IR_TX_MEM_SET_REG(0x02UL)
//---------------------------------------------
// definition for IR TX status
//---------------------------------------------

#define IR_TX_Unit00_H     __BIT0
#define IR_TX_Unit00_L     __BIT1
#define IR_TX_Unit01_H     __BIT2
#define IR_TX_Unit01_L     __BIT3
#define IR_TX_Unit02_H     __BIT4
#define IR_TX_Unit02_L     __BIT5
#define IR_TX_Unit03_H     __BIT6
#define IR_TX_Unit03_L     __BIT7
#define IR_TX_Unit04_H     __BIT8
#define IR_TX_Unit04_L     __BIT9
#define IR_TX_Unit05_H     __BIT10
#define IR_TX_Unit05_L     __BIT11
#define IR_TX_Unit06_H     __BIT12
#define IR_TX_Unit06_L     __BIT13
#define IR_TX_Unit07_H     __BIT14
#define IR_TX_Unit07_L     __BIT15

#define IR_TX_Unit08_H     __BIT0
#define IR_TX_Unit08_L     __BIT1
#define IR_TX_Unit09_H     __BIT2
#define IR_TX_Unit09_L     __BIT3
#define IR_TX_Unit10_H     __BIT4
#define IR_TX_Unit10_L     __BIT5
#define IR_TX_Unit11_H     __BIT6
#define IR_TX_Unit11_L     __BIT7
#define IR_TX_Unit12_H     __BIT8
#define IR_TX_Unit12_L     __BIT9
#define IR_TX_Unit13_H     __BIT10
#define IR_TX_Unit13_L     __BIT11

//---------------------------------------------
// definition for IR TX status
//---------------------------------------------


