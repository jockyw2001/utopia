CHIP=\
(
manhattan
maserati
maxim
kano
k6
k6lite
curry
M7821
M7621
mustang
)

MODULE_FOLDER=\
(
acp
audio
bdma
ch34
clkm
cmdq
cpu
dac
ddc2bi
demodulator
dlc
dmx
dscmb
flash
gpd
gpio
graphic
hdmi
hsl
hwi2c
ipauth
ir
irq
ldm
mbx
mfc
mfe
miu
msos
mspi
mvop
njpd
ojpd_vdec_v1
ojpd_vdec_v2
pcmcia
pm
pq
pvr_iframelut
pwm
pws
rtc
sar
sc
seal
security
sem
smbx
stbdc
swi2c
sys
uart
urdma
usb
vd
vdec_lite
vdec_v1
vdec_v2
vdec_v3
ve
vif
wble
wdt
xc
ir_tx
)

function ClearArray(){
	DRV_FOLDER=()
	API_FOLDER=()
	HAL_FOLDER=()
	ADAPTION_FOLDER=()
}


#Project
#Remove old links
rm -f $PWD/build
rm -f $PWD/project
rm -f $PWD/tools
rm -f $PWD/verify
rm -f $PWD/version
rm -f $PWD/tmplib
ln -s -v $PWD/projects/build $PWD/build
ln -s -v $PWD/projects/project $PWD/project
ln -s -v $PWD/projects/tools $PWD/tools
ln -s -v $PWD/projects/verify $PWD/verify
ln -s -v $PWD/projects/version $PWD/version
ln -s -v $PWD/projects/version $PWD/tmplib

mkdir -p $PWD/mxlib/utopia_adaption
#msos
#Remove old links
rm -f $PWD/mxlib/msos
rm -f $PWD/mxlib/utopia_core
rm -f $PWD/mxlib/utopia_api_relation

ln -s -v $PWD/modules/msos/msos $PWD/mxlib/msos
ln -s -v $PWD/modules/msos/utopia_core $PWD/mxlib/utopia_core
ln -s -v $PWD/modules/msos/utopia_api_relation $PWD/mxlib/utopia_api_relation

#Process modules folder
for ((index=0; index<${#MODULE_FOLDER[@]}; index++))
do
ClearArray;
source scripts/${MODULE_FOLDER[$index]}.sh

#Remove old and generate api links
if [ ${#API_FOLDER[@]} -gt 0 ];then
for ((jndex=0; jndex<${#API_FOLDER[@]}; jndex++))
do
rm -f $PWD/mxlib/api/${API_FOLDER[$jndex]}
ln -s -v $PWD/modules/${MODULE_FOLDER[$index]}/api/${API_FOLDER[$jndex]} $PWD/mxlib/api/${API_FOLDER[$jndex]}
done
fi

#Remove old and generate drv links
if [ ${#DRV_FOLDER[@]} -gt 0 ];then
for ((jndex=0; jndex<${#DRV_FOLDER[@]}; jndex++))
do
rm -f $PWD/mxlib/drv/${DRV_FOLDER[$jndex]}
ln -s -v $PWD/modules/${MODULE_FOLDER[$index]}/drv/${DRV_FOLDER[$jndex]} $PWD/mxlib/drv/${DRV_FOLDER[$jndex]}
done
fi

#Remove old and generate adaption links
if [ ${#ADAPTION_FOLDER[@]} -gt 0 ];then
for ((jndex=0; jndex<${#ADAPTION_FOLDER[@]}; jndex++))
do
rm -f $PWD/mxlib/utopia_adaption/${ADAPTION_FOLDER[$jndex]}
ln -s -v $PWD/modules/${MODULE_FOLDER[$index]}/utopia_adaption/${ADAPTION_FOLDER[$jndex]} $PWD/mxlib/utopia_adaption/${ADAPTION_FOLDER[$jndex]}
done
fi

#Remove old and generate hal links
if [ ${#HAL_FOLDER[@]} -gt 0 ];then
for ((jndex=0; jndex<${#CHIP[@]}; jndex++))
do
for ((kndex=0; kndex<${#HAL_FOLDER[@]}; kndex++))
do
rm -f $PWD/mxlib/hal/${CHIP[$jndex]}/${HAL_FOLDER[$kndex]}
ln -s -v $PWD/modules/${MODULE_FOLDER[$index]}/hal/${CHIP[$jndex]}/${HAL_FOLDER[$kndex]} $PWD/mxlib/hal/${CHIP[$jndex]}/${HAL_FOLDER[$kndex]}
done
done
fi

done
