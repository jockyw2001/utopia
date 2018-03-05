#echo "making t2_nos"
#make all install PRJ=t2_nos TARGET=s4le_nos > make_t2_nos.log
#grep 'LINT Errors' make_t2_nos.log
#echo

#echo "making t2_nos_mips"
#make all install PRJ=t2_nos_mips TARGET=s4le_nos_mips > make_t2_nos_mips.log
#grep 'LINT Errors' make_t2_nos_mips.log
#echo

echo "making janus_nos"
make all install PRJ=janus_nos TARGET=s7j_nos > make_janus_nos.log
grep 'LINT Errors' make_janus_nos.log
echo

echo "making janus_r2"
make all install PRJ=janus_r2 TARGET=s7j_nos_r2m > make_janus_r2.log
grep 'LINT Errors' make_janus_r2.log
echo

echo "making t4_nos"
make all install PRJ=t4_nos TARGET=s7ld_nos > make_t4_nos.log
grep 'LINT Errors' make_t4_nos.log
echo

echo "making t4_nos_mips"
make all install PRJ=t4_nos_mips TARGET=s7ld_nos_mips > make_t4_nos_mips.log
grep 'LINT Errors' make_t4_nos_mips.log
echo

echo "making t7_nos_mips"
make all install PRJ=t7_nos_mips TARGET=s7la_nos_mips > make_t7_nos_mips.log
grep 'LINT Errors' make_t7_nos_mips.log
echo

echo "making t8_nos_mips"
make all install PRJ=t8_nos_mips TARGET=s8_nos_mips > make_t8_nos_mips.log
grep 'LINT Errors' make_t8_nos_mips.log
echo

echo "making t9_nos_mips"
make all install PRJ=t9_nos_mips TARGET=s7l_nos_mips > make_t9_nos_mips.log
grep 'LINT Errors' make_t9_nos_mips.log
echo

