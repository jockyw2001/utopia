#del fwHVD.dat
#for %%i in (*.bin) do miniperl fwHVD_bin2hex.pl %%i 1 >> fwHVD.dat
del fwHVD.dat
del fwHVD.bin
miniperl fwHVD_bin2hex.pl hvd_vd.bin 1 >> fwHVD.dat
rename hvd_vd.bin fwHVD.bin
