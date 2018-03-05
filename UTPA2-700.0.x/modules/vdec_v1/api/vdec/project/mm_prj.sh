
echo "making t3_nos"
make -fMakefile.mm all install PRJ=t3_nos TARGET=t3_nos FW_TYPE=embedded > make_t3_nos.log
grep 'LINT Errors' make_t3_nos.log
echo

echo "making u3_nos"
make -fMakefile.mm all install PRJ=u3_nos TARGET=u3_nos FW_TYPE=embedded > make_u3_nos.log
grep 'LINT Errors' make_u3_nos.log
echo
