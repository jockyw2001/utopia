# arg1 = The LOCAL path    
LOCAL_PATH=$1
BACKUP=$2
# **********************************************
# Environment
# **********************************************
ROOTLIB=$LOCAL_PATH'/mxlib'
BSP_PATH=$LOCAL_PATH'/build/bsp'
BSPINC_PATH=$LOCAL_PATH'/build/bsp/include'
Build_PATH=$LOCAL_PATH'/build'
LOG_PATH=$LOCAL_PATH'/build/outputlog'

MAX_SCAN=255
#Backup the original mxlib's data
function do_Backup_mxlib_include()
{
	if [ "$BACKUP" == "BACKUP_INC" ];then
		if [ -d $ROOTLIB/include_backup ]; then
			rm -rf $ROOTLIB/include
			cp -avr $ROOTLIB/include_backup $ROOTLIB/include>> $LOG_PATH/copymessage.log
		else
			cp -a $ROOTLIB/include $ROOTLIB/include_backup >> $LOG_PATH/copymessage.log
		fi
	else
		if [ -d $ROOTLIB/include_backup ]; then
			printf "\E[0;31;40m"
			echo "=========================================================="
			echo " Find include_backupk"
			echo " The folder be used Backup mxlib/include"
			echo " Now move the mxlib/include_backup to mxlib/Include!!!"
			echo "=========================================================="
			printf "\E[0m"	
			rm -rf $ROOTLIB/include
			mv  $ROOTLIB/include_backup $ROOTLIB/include>> $LOG_PATH/copymessage.log		
		fi
	fi
}
rm -rf $LOG_PATH
mkdir $LOG_PATH
if [ ! -d $BSP_PATH ]; then
mkdir $BSP_PATH
fi
rm -rf $BSPINC_PATH
mkdir $BSPINC_PATH
do_Backup_mxlib_include

