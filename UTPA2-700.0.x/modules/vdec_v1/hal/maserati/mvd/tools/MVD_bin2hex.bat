del fwMVD.dat
del fwMVD.bin
miniperl fwMVD_bin2hex.pl mvd4_fw.bin 1 >> fwMVD.dat
rename mvd4_fw.bin fwMVD.bin
