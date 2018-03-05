del fwVPU.dat
del fwVPU.bin
miniperl bin2hex.pl vdecoder.bin 1 >> fwVPU.dat
copy vdecoder.bin fwVPU.bin /V
copy fwVPU.bin ..\fwVPU.bin
copy fwVPU.dat ..\fwVPU.dat
dir ..\fwVPU.*
