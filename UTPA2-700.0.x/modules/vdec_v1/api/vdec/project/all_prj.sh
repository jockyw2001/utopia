#echo "making t2_nos"
#make all install PRJ=t2_nos TARGET=s4le_nos CHIPS=s4le FW_TYPE=external > make_t2_nos.log
#grep 'LINT Errors' make_t2_nos.log
#echo

#echo "making t2_nos_mips"
#make all install PRJ=t2_nos_mips TARGET=s4le_nos_mips CHIPS=s4le FW_TYPE=external > make_t2_nos_mips.log
#grep 'LINT Errors' make_t2_nos_mips.log
#echo

echo "making janus_nos"
make all install PRJ=janus_nos TARGET=s7j_nos CHIPS=s7j FW_TYPE=external > make_janus_nos.log
grep 'LINT Errors' make_janus_nos.log
echo

echo "making janus_r2"
make all install PRJ=janus_r2 TARGET=s7j_nos_r2m CHIPS=s7j FW_TYPE=external > make_janus_r2.log
grep 'LINT Errors' make_janus_r2.log
echo

#echo "making t7_nos_mips"
#make all install PRJ=t7_nos_mips TARGET=s7la_nos_mips CHIPS=s7la FW_TYPE=external > make_t7_nos_mips.log
#grep 'LINT Errors' make_t7_nos_mips.log
#echo

echo "making t4_nos"
make all install PRJ=t4_nos TARGET=s7ld_nos CHIPS=s7ld FW_TYPE=external > make_t4_nos.log
grep 'LINT Errors' make_t4_nos.log
echo

echo "making t4_nos_mips"
make all install PRJ=t4_nos_mips TARGET=s7ld_nos_mips CHIPS=s7ld FW_TYPE=external > make_t4_nos_mips.log
grep 'LINT Errors' make_t4_nos_mips.log
echo

echo "making U4_nos"
make all install PRJ=u4_nos TARGET=u4_nos CHIPS=u4 FW_TYPE=external > make_U4_nos.log
grep 'LINT Errors' make_U4_nos.log
echo

echo "making U4_nos_mips"
make all install PRJ=u4_nos_mips TARGET=u4_nos_mips CHIPS=u4 FW_TYPE=external > make_U4_nos_mips.log
grep 'LINT Errors' make_U4_nos_mips.log
echo

