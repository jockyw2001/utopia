#!/bin/bash
DD_BIN="vdecoder.bin"
DD_DAT="vdecoder.dat"
UTOPIA_PRJ="../../../../../project/j2_linux"
SN_PATH="../../../../../../../DAILEO/temp_branch/Supernova_CTK/projects/"

pwd
test -e ./$DD_BIN && echo -e "\n\n ****** Found $DD_BIN *** " || echo -e "\n\n ****** $DD_BIN NOT Found ****** "
test -e ./$DD_BIN || exit 0
echo -e " Generate $DD_DAT from $DD_BIN"

perl ./bin2hex.pl $DD_BIN 1 > $DD_DAT
ls -l $DD_DAT

cp $DD_DAT ../

echo -e "\n *** Build Utopia VDEC_EX library *** "
cd $UTOPIA_PRJ; make clean vdec_ex bsp

echo -e "\n *** Update SuperNova VDEC_EX library *** "
cp bsp/lib/libapiVDEC.so ../../../../DAILEO/temp_branch/Supernova_CTK/target/j2/mslib/utopia/libapiVDEC.so
ls -l ../../../../DAILEO/temp_branch/Supernova_CTK/target/j2/mslib/utopia/libapiVDEC.so
cd -

echo -e "\n *** Make SuperNova library image *** "
#pwd
cd $SN_PATH; #pwd
source env_j2_120A.cfg
make image_all
echo -e "\nOn target please enter:\nmstar mscript/ubifs/[[mslib\n"

