#echo "making t2_nos"
#make all install PRJ=t2_nos TARGET=s4le_nos CHIPS=s4le FW_TYPE=external > make_t2_nos.log
#grep 'LINT Errors' make_t2_nos.log
#echo

#echo "making t2_nos_mips"
#make all install PRJ=t2_nos_mips TARGET=s4le_nos_mips CHIPS=s4le FW_TYPE=external > make_t2_nos_mips.log
#grep 'LINT Errors' make_t2_nos_mips.log
#echo

echo "making t3_nos"
make all install PRJ=t3_nos TARGET=s7_nos CHIPS=s7 FW_TYPE=external > make_t3_nos.log
grep 'LINT Errors' make_t3_nos.log
echo

echo "making t3_nos_mips"
make all install PRJ=t3_nos_mips TARGET=s7_nos_mips CHIPS=s7 FW_TYPE=external > make_t3_nos_mips.log
grep 'LINT Errors' make_t3_nos_mips.log
echo

echo "making u3_nos"
make all install PRJ=u3_nos TARGET=u3_nos CHIPS=u3 FW_TYPE=external > make_u3_nos.log
grep 'LINT Errors' make_u3_nos.log
echo

#Obsolete
#echo "making t2 Obama"
#make all install PRJ=t2_linux OUT_LIB_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t2/lib OUT_INC_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t2/include LINK_TYPE=dynamic > make_t2_linux.log
#grep 'LINT Errors' make_t2_linux.log
#echo

#echo "making u3 Obama"
#make all install PRJ=u3_linux OUT_LIB_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/u3/lib OUT_INC_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/u3/include LINK_TYPE=dynamic > make_u3_linux.log
#grep 'LINT Errors' make_u3_linux.log
#echo

#echo "making t3 Obama"
#make all install PRJ=t3_linux ADD=mfc OUT_LIB_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t3/lib OUT_INC_DIR=./../../../../../../DAILEO/Obama/SDK/vendor/t3/include LINK_TYPE=dynamic > make_t3_linux.log
#grep 'LINT Errors' make_t3_linux.log
#echo
