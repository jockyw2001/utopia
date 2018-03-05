# How to use:
#@sh	Reduced.sh api_with_header_file.csv OneUtopia1.0 ../mxlib/ 
# arg1 = The LOCAL path  
# **********************************************
# Environment
# **********************************************
LOCAL_PATH=$1
ROOTLIB=$LOCAL_PATH'/mxlib'
BSP_PATH=$LOCAL_PATH'/build/bsp'
BSPINC_PATH=$LOCAL_PATH'/build/bsp/include'
Build_PATH=$LOCAL_PATH'/build'
LOG_PATH=$LOCAL_PATH'/build/outputlog'
SN_Specil_SRC=\
(
MsTypes.h
drvXC_IOPort.h
MsDevice.h
MsCommon.h
MsIRQ.h
UFO.h
apiHSL.h
apiHSL_SHM.h
drvAUDIO_if.h
drvDMD_INTERN_ATSC.h
MsVersion.h
drvMIU_v2.h
drvPWM_v2.h
apiGOP_v2.h
drvHWI2C_v2.h
drvMSPI_v2.h
drvCPU_v2.h
drvBDMA_v2.h
drvAESDMA_v2.h
drvSERFLASH_v2.h
MLog.h
ULog.h
label.h
)
Bifrost_Specil_SRC=\
(
apiXC_PCMonitor_v2.h
apiXC_v2.h
apiXC_Ace_v2.h
apiPNL_v2.h
apiXC_Adc_v2.h
drvXC_HDMI_if_v2.h
apiXC_PCMonitor_EX.h
drvCIPHER.h
apiXC_Ace_EX.h
apiXC_DWIN_EX.h
MLog.h
ULog.h
label.h
)
AN_Specil_SRC=\
(
drvDIP.h
drvCMAPool.h
apiXC_DWIN_v2.h
api_mfe_frmformat.h
drv_mfe_st.h
ULog.h
label.h
)

function CP_File2BSP()
{
  for ((jndex=0; jndex<${#SN_Specil_SRC[@]}; jndex++)); do
    find -L $ROOTLIB -name ${SN_Specil_SRC[$jndex]} -exec cp {} $BSPINC_PATH/ \; 2> $LOG_PATH/error.log
  done
  for ((jndex=0; jndex<${#Bifrost_Specil_SRC[@]}; jndex++)); do
    find -L $ROOTLIB -name ${Bifrost_Specil_SRC[$jndex]} -exec cp {} $BSPINC_PATH/ \; 2> $LOG_PATH/error.log
  done
  for ((jndex=0; jndex<${#AN_Specil_SRC[@]}; jndex++)); do
    find -L $ROOTLIB -name ${AN_Specil_SRC[$jndex]} -exec cp {} $BSPINC_PATH/ \; 2> $LOG_PATH/error.log
  done
  cp -f $ROOTLIB/include/apiVOIPMFE.h $BSPINC_PATH/ \; 2> $LOG_PATH/error.log
}
CP_File2BSP
