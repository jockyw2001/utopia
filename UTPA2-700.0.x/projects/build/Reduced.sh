# How to use:
#@sh	Reduced.sh api_with_header_file.csv OneUtopia1.0 ../mxlib/ 
# arg1 = CSV file 					ex:path =api_with_header_file.csv
# arg2 = The LOCAL path   
# arg3 = Use Additional Contents    ex:OneUtopia1.0
 
filename=$1
LOCAL_PATH=$2
UtopiaVersion=$3
dos2unix $filename

# **********************************************
# Environment
# **********************************************
ROOTLIB=$LOCAL_PATH'/mxlib'
BSP_PATH=$LOCAL_PATH'/build/bsp'
BSPINC_PATH=$LOCAL_PATH'/build/bsp/include'
Build_PATH=$LOCAL_PATH'/build'
LOG_PATH=$LOCAL_PATH'/build/outputlog'

MAX_SCAN=255

#Scan First Row and pair
function FindSuitableCell()
{	
	#Find a suitable pair about filename
	for ((jndex=1;jndex<$MAX_SCAN; jndex++)); do
	used=$(awk -F"," -v var=$jndex 'NR==1 {print $var}' $filename)
	#The pairing is successful
	if [ "$used" == "API_NAME" ]; then
		AIP_NAMECell=$jndex
		break
	fi
	done
	
	 #Find a suitable pair about topiaVersion
	for ((jndex=1;jndex<$MAX_SCAN; jndex++)); do
		used=$(awk -F"," -v var=$jndex 'NR==1 {print $var}' $filename)
		#The pairing is successful
		if [ "$used" == "HEAD_FILE" ]; then
			HEAD_FILECell=$jndex
			break
		fi
	done
	
    #Find a suitable pair about topiaVersion
	for ((jndex=1;jndex<$MAX_SCAN; jndex++)); do
	used=$(awk -F"," -v var=$jndex 'NR==1 {print $var}' $filename)
	#The pairing is successful
	if [ "$used" == $"" ];then
			break
	else
		if [ "$used" == "$UtopiaVersion" ];then
			UtopiaVersionCell=$jndex
			break	
		elif [ "$used" != $"" ];then
			UtopiaVersionCell=$jndex
		fi
	fi
	done
	#printf "%d" $AIP_NAMECell
	#printf "%d" $HEAD_FILECell
	#printf "%d" $UtopiaVersionCell	
	#printf "Use : %s\n"  $(awk -F"," -v var=$UtopiaVersionCell 'NR==1 {print $var}' $filename)
}
function ReadCVSFile()
{
    itemndex=0
	temp=$itemndex
	temp_AIP_File=0
    while read line
	do 
		TARGET_AIP_NAME[$itemndex]=$(echo $line | awk -F"," -v var=$AIP_NAMECell '{print $var}')
		TARGET_AIP_use[$itemndex]=$(echo $line | awk -F"," -v var=$UtopiaVersionCell '{print $var}')
		TARGET_AIP_File[$itemndex]=$(echo $line | awk -F"," -v var=$HEAD_FILECell '{print $var}' )
		if [ ${TARGET_AIP_use[$itemndex]} == "1" ]; then	
			if [ "${TARGET_AIP_File[$itemndex]}" != "$temp_AIP_File" ]; then
				temp2="${TARGET_AIP_File[$itemndex]}"
				temp_AIP_File=$temp2
				printf "%s" ${TARGET_AIP_File[$itemndex]},>>$LOG_PATH/filename.csv
				printf "\n">>$LOG_PATH/filename.csv
			fi
		fi		
		#printf "get %s %s\n"	${TARGET_AIP_NAME[$itemndex]},	 ${TARGET_AIP_use[$itemndex]}	
		temp=$itemndex
		itemndex=$[$temp+1];

	done < $filename

}
function do_parsingHeadfile2()
{
	while read line
	do
		hfile=$(echo $line | awk -F"," '{print $1}' )
		printf " %s \n",$hfile
		find -L $ROOTLIB -name $hfile -exec cp {} $BSPINC_PATH/ \; 2> $LOG_PATH/error.log
	done < $LOG_PATH/filename.csv  
	cp -f $ROOTLIB/api/njpeg_ex/apiJPEG_priv.h  $BSPINC_PATH/ \; 2> $LOG_PATH/error.log

}
function do_parsingHeadfile_Contents()
{
  #Remove_First_line=0
	for ((jndex=1; jndex<${#TARGET_AIP_NAME[@]}; jndex++)); do	
			# Choose must remove API function
			if [ ${TARGET_AIP_use[$jndex]} == "0" ]; then
				# Check the file exists on bsp/include/
				if [  -f $BSPINC_PATH/${TARGET_AIP_File[$jndex]} ]; then
					#remove unused item				
					sh $Build_PATH/mcomment.sh ${TARGET_AIP_NAME[$jndex]} $BSPINC_PATH/${TARGET_AIP_File[$jndex]} " "
					#printf "mark API: %s" ${TARGET_AIP_NAME[$jndex]}
					#printf "    h_file: %s" $BSPINC_PATH/${TARGET_AIP_File[$jndex]}
					#printf "\n"
				fi	
					#printf "mark API: %s" $api_name
					#printf "h_file: %s"$hfile
					#printf "\n"
			fi
	done
}
function do_RemoveTab()
{
    rm -rf $LOG_PATH/filename.txt
	ls $BSPINC_PATH/ --color=never >$LOG_PATH/filename.txt 
	
	
   while read line
   do
	hfile=$(echo $line | awk -F"\n" '{print $1}' )
    #printf "==>%s\n" $hfile
	sed 's/\t/    /g' $BSPINC_PATH/$hfile >$LOG_PATH/temp.h
	mv -f $LOG_PATH/temp.h $BSPINC_PATH/$hfile

   done < $LOG_PATH/filename.txt     
}
FindSuitableCell
ReadCVSFile
do_parsingHeadfile2>>$LOG_PATH/test.log
sh $Build_PATH/special_case.sh $LOCAL_PATH
do_RemoveTab
do_parsingHeadfile_Contents>>$LOG_PATH/test.log
#do_CPBackup2mxlib

