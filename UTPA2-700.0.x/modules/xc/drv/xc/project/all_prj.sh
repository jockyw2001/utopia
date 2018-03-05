# $1 = all, wlbe, cec, pnl, ace, dlc, xc, ddc2bi, ve, hdmitx, dac, mfc
echo Build $1 $2
echo "parameter 1: [all|wble|cec|pnl|ace|dlc|xc|ddc2bi|ve|hdmitx|dac|mfc]"
echo "parameter 2: [install]"
echo "You can check lint_test.log in utopia/project/* folders"

#echo "making t2_nos"
#make $1 $2 PRJ=t2_nos TARGET=s4le_nos > make_t2_nos.log
#grep 'LINT Errors' make_t2_nos.log
#echo

#echo "making t2_nos_mips"
#make $1 $2 PRJ=t2_nos_mips TARGET=s4le_nos_mips > make_t2_nos_mips.log
#grep 'LINT Errors' make_t2_nos_mips.log
#echo

#echo "making t2_linux"
#make $1 $2 PRJ=t2_linux > make_t2_linux.log
#grep 'LINT Errors' make_t2_linux.log
#echo

#echo "making t3_nos"
#make $1 $2 mfc PRJ=t3_nos TARGET=s7_nos > make_t3_nos.log
#grep 'LINT Errors' make_t3_nos.log
#echo

#echo "making t3_nos_mips"
#make $1 $2 mfc PRJ=t3_nos_mips TARGET=s7_nos_mips > make_t3_nos_mips.log
#grep 'LINT Errors' make_t3_nos_mips.log
#echo

#echo "making janus_nos"
#make $1 $2 mfc PRJ=janus_nos TARGET=s7j_nos > make_janus_nos.log
#grep 'LINT Errors' make_janus_nos.log
#echo

#echo "making janus_r2"
#make $1 $2 mfc PRJ=janus_r2 TARGET=s7j_nos_r2m > make_janus_r2.log
#grep 'LINT Errors' make_janus_r2.log
#echo

echo "making t9_nos_mips"
make $1 $2 PRJ=t9_nos_mips TARGET=s7l_nos_mips > make_t9_nos_mips.log
grep 'LINT Errors' make_t9_nos_mips.log
echo

echo "making t7_nos_mips"
make $1 $2 PRJ=t7_nos_mips TARGET=s7la_nos_mips > make_t7_nos_mips.log
grep 'LINT Errors' make_t7_nos_mips.log
echo

#echo "making t4_nos"
#make $1 $2 PRJ=t4_nos TARGET=s7ld_nos > make_t4_nos.log
#grep 'LINT Errors' make_t4_nos.log
#echo

#echo "making t4_nos_mips"
#make $1 $2 PRJ=t4_nos_mips TARGET=s7ld_nos_mips > make_t4_nos_mips.log
#grep 'LINT Errors' make_t4_nos_mips.log
#echo

#echo "making t8_nos"
#make $1 $2 PRJ=t8_nos TARGET=s8_nos > make_t8_nos.log
#grep 'LINT Errors' make_t8_nos.log
#echo

echo "making t8_nos_mips"
make $1 $2 PRJ=t8_nos_mips TARGET=s8_nos_mips > make_t8_nos_mips.log
grep 'LINT Errors' make_t8_nos_mips.log
echo

#echo "making t8_nos_r2m"
#make $1 $2 PRJ=t8_nos_r2 TARGET=s8_nos_r2m > make_t8_nos_r2m.log
#grep 'LINT Errors' make_t8_nos_r2m.log
#echo

#echo "making u3_nos"
#make $1 $2 PRJ=u3_nos TARGET=u3_nos > make_u3_nos.log
#grep 'LINT Errors' make_u3_nos.log
#echo

#echo "making u4_nos"
#make $1 $2 PRJ=u4_nos TARGET=u4_nos > make_u4_nos.log
#grep 'LINT Errors' make_u4_nos.log
#echo

#echo "making u4_linux"
#make $1 $2 PRJ=u4_linux > make_u4_linux.log
#grep 'LINT Errors' make_u4_linux.log
#echo

#Obsolete
#echo "making t2 Obama"
#make $1 $2 PRJ=t2_linux OUT_LIB_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t2/lib OUT_INC_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t2/include LINK_TYPE=dynamic > make_t2_linux.log
#grep 'LINT Errors' make_t2_linux.log
#echo

#echo "making u3 Obama"
#make $1 $2 PRJ=u3_linux OUT_LIB_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/u3/lib OUT_INC_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/u3/include LINK_TYPE=dynamic > make_u3_linux.log
#grep 'LINT Errors' make_u3_linux.log
#echo

#echo "making t3 Obama"
#make $1 $2 mfc PRJ=t3_linux OUT_LIB_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t3/lib OUT_INC_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t3/include LINK_TYPE=dynamic > make_t3_linux.log
#grep 'LINT Errors' make_t3_linux.log
#echo
