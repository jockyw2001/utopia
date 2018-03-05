echo "making t4_nos VESTEL_FEATURE"
make vdec bsp PRJ=t4_nos TARGET=s7ld_nos CHIPS=s7ld FW_TYPE=embedded VESTEL_FEATURE=enable > make_t4_nos.log
mv ../../../../project/t4_nos/bsp/lib/libapiVDEC.a ../../../../project/t4_nos/libapiVDEC_8061.a 
grep 'LINT Errors' make_t4_nos.log
echo

echo "making t4_nos"
make vdec bsp PRJ=t4_nos TARGET=s7ld_nos CHIPS=s7ld FW_TYPE=embedded VESTEL_FEATURE=disable > make_t4_nos.log
grep 'LINT Errors' make_t4_nos.log
echo

mv ../../../../project/t4_nos/libapiVDEC_8061.a ../../../../project/t4_nos/bsp/lib

echo "making t4_nos_mips VESTEL_FEATURE"
make vdec bsp PRJ=t4_nos_mips TARGET=s7ld_nos_mips CHIPS=s7ld FW_TYPE=embedded VESTEL_FEATURE=enable > make_t4_nos_mips.log
mv ../../../../project/t4_nos_mips/bsp/lib/libapiVDEC.a ../../../../project/t4_nos_mips/libapiVDEC_8061.a 
grep 'LINT Errors' make_t4_nos_mips.log
echo

echo "making t4_nos_mips"
make vdec bsp PRJ=t4_nos_mips TARGET=s7ld_nos_mips CHIPS=s7ld FW_TYPE=embedded VESTEL_FEATURE=disable > make_t4_nos_mips.log
grep 'LINT Errors' make_t4_nos_mips.log
echo

mv ../../../../project/t4_nos_mips/libapiVDEC_8061.a ../../../../project/t4_nos_mips/bsp/lib
