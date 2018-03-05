Utopia_Path=\
(
./
./projects
./mxlib
./mxlib/include
./mxlib/msfs
./modules/msos
./modules/xc
./modules/acp
./modules/ch34
./modules/dac
./modules/dlc
./modules/mfc
./modules/pq
./modules/sc
./modules/ve
./modules/wble
./modules/audio
./modules/demodulator
./modules/dscmb
./modules/graphic
./modules/hdmi
./modules/mfe
./modules/pcmcia
./modules/bdma
./modules/ir_tx
./modules/cmdq
./modules/ddc2bi
./modules/flash
./modules/hwi2c
./modules/ipauth
./modules/ir
./modules/mspi
./modules/pwm
./modules/rtc
./modules/sar
./modules/uart
./modules/wdt
./modules/security
./modules/hsl
./modules/mbx
./modules/swi2c
./modules/cpu
./modules/gpio
./modules/irq
./modules/miu
./modules/pm
./modules/pws
./modules/seal
./modules/sem
./modules/smbx
./modules/sys
./modules/urdma
./modules/dmx
./modules/usb
./modules/vd
./modules/gpd
./modules/vdec_v1
./modules/vdec_v2
./modules/vdec_v3
./modules/vdec_lite
./modules/ojpd_vdec_v1
./modules/ojpd_vdec_v2
./modules/mvop
./modules/njpd
./modules/stbdc
./modules/vif
./modules/clkm
./modules/ldm
./modules/pvr_iframelut
)

Utopia_projectName=\
(
UTPA2-700.0.x
projects
mxlib
include
msfs
msos
xc
acp
ch34
dac
dlc
mfc
pq
sc
ve
wble
audio
demodulator
dscmb
graphic
hdmi
mfe
pcmcia
bdma
cmdq
ddc2bi
flash
hwi2c
ipauth
ir
mspi
pwm
rtc
sar
uart
wdt
security
hsl
mbx
swi2c
cpu
gpio
irq
miu
pm
pws
seal
sem
smbx
sys
urdma
dmx
usb
vd
gpd
vdec_v1
vdec_v2
vdec_v3
vdec_lite
ojpd_vdec_v1
ojpd_vdec_v2
mvop
njpd
stbdc
vif
clkm
ldm
pvr_iframelut
)

function ShowHelp(){
	echo -e Usage:
	echo -e TagLog.sh TAG1 TAG2
}

if [ ! "$1" ] || [ ! "$2" ];then
	echo Please input tagnames.
	ShowHelp;
	exit
fi

PROJHOME=$PWD
for ((jndex=0; jndex<${#Utopia_Path[@]}; jndex++))
do
cd ${Utopia_Path[$jndex]}
GITPATH=$(git remote -v)
OUTPUT=$(git log $1...$2 --oneline -n1) #try git log
if [ "$OUTPUT" ];then
    echo -e "\033[41;36m------${Utopia_projectName[$jndex]}------\033[0m"
    git log $1...$2 --oneline
    echo -e ""
    echo -e ""
fi
cd $PROJHOME
done
