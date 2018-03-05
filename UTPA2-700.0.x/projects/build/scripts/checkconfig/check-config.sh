#!/bin/bash 

# all config.required.xxx are based on CL621094

# these identifiers can't be disabled!!!!!!
REQUIRED_CONFIG_PATH=scripts/checkconfig
#ANDROID_IDENTIFIER=CONFIG_ANDROID
ANDROID_IDENTIFIER=CONFIG_ANDROID_LOGGER
EMMC_IDENTIFIER=CONFIG_EMMC_PARTITION
NAND_IDENTIFIER=CONFIG_MSTAR_NAND

cat .config | \grep -v ^# | \grep -v ^$ | sort > config.tmp

check_miss()
{
	if [ "$1" == "0" ]; then
		#echo [WARNING] you missed some required configs, press enter to continue
		#read
		error=1
	fi
}

# platform
grep $ANDROID_IDENTIFIER config.tmp > /dev/null
if [ "$?" == "0" ]; then
	echo check .config with config.required.android
	echo -ne '\E[37;44m'
	grep $REQUIRED_CONFIG_PATH/config.required.android -v -f config.tmp
	check_miss $?
	echo -ne '\033[0m'
	#sort $REQUIRED_CONFIG_PATH/config.required.android > config.platform
	#diff config.tmp config.platform | \grep \> 
else
	echo check .config with config.required.supernova
	echo -ne '\E[37;44m'
	grep $REQUIRED_CONFIG_PATH/config.required.supernova -v -f config.tmp
	check_miss $?
	echo -ne '\033[0m'
	#sort $REQUIRED_CONFIG_PATH/config.required.supernova > config.platform
	#diff config.tmp config.platform | \grep \> 
fi

# storage
grep $EMMC_IDENTIFIER config.tmp > /dev/null
if [ "$?" == "0" ]; then
	echo check .config with config.required.emmc
	echo -ne '\E[37;44m'
	grep $REQUIRED_CONFIG_PATH/config.required.emmc -v -f config.tmp
	check_miss $?
	echo -ne '\033[0m'
	#sort $REQUIRED_CONFIG_PATH/config.required.emmc > config.storage
	#diff config.tmp config.storage | \grep \> 
fi
grep $NAND_IDENTIFIER config.tmp > /dev/null
if [ "$?" == "0" ]; then
	echo check .config with config.required.nand
	echo -ne '\E[37;44m'
	grep $REQUIRED_CONFIG_PATH/config.required.nand -v -f config.tmp
	check_miss $?
	echo -ne '\033[0m'
	#sort $REQUIRED_CONFIG_PATH/config.required.nand > config.storage
	#diff config.tmp config.storage | \grep \> 
fi

echo check .config with config.required.all
echo -ne '\E[37;44m'
grep $REQUIRED_CONFIG_PATH/config.required.all -v -f config.tmp
check_miss $?
echo -ne '\033[0m'

if [ "$error" == "1" ]; then
	echo -ne '\E[37;44m'
	echo you missed the above required configs, y to continue, n to exit: 
	echo \(or email bob.fu@mstarsemi.com to ask about it\)
	echo -ne '\033[0m'
	read choice
	
	case "$choice" in
		y)
		;;
		n)
		exit 1
		;;
		*)
		echo is that funny?
		exit 1
esac

fi
