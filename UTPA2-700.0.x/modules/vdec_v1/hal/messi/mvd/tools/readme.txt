Steps to update MVD firmware:
1. Copy mvd4_fw.bin to \\THEALE\utopia\mxlib\hal\t4\mvd\tools\
2. Execute MVD_bin2hex.bat
   (fwMVD.h will be updated after executing this batch file)
3. Copy new fwMVD.h & mvd4_interface.h to \\THEALE\utopia\mxlib\hal\t4\mvd\
4. Change dir to \\THEALE\utopia\project\t4_nos
    make vdec
5. Copy \\THEALE\utopia\lib\t4_nos\aeon_debug\libapiVDEC.a to Chakra2
