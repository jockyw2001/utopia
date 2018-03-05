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
#copy mxlib/include to ../mxlib/include
function do_CPBackup2mxlib()
{
	rm -rf $ROOTLIB/include
	cp -avr  $BSPINC_PATH $ROOTLIB/include >> $LOG_PATH/copymessage.log
}
do_CPBackup2mxlib