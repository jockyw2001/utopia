# How to use:
#@sh    CreateKsym.sh
# step:
# 1. build utpa2k.ko first
# 2. run script
# 3. (if necessary) fix symbol which is exported twice
# 4. mv Ksym/Ksym.c to /modules/msos/msos/linux_kernel_V2
# 5. rebuild utpa2k.ko

function CreateKsymCFile()
{
    if [ -e Ksym ];then
        rm -rf Ksym
    fi
    rm -f nm_symbol.txt preprocess.txt
    mkdir Ksym
    echo "#include \"MsTypes.h\"" >> Ksym/Ksym.c
    echo "#include \"MsCommon.h\"" >> Ksym/Ksym.c
    echo "#include \"MLog.h\"" >> Ksym/Ksym.c
    echo "#include \"apiACP.h\"" >> Ksym/Ksym.c
    echo "#include \"apiAUDIO.h\"" >> Ksym/Ksym.c
    echo "#include \"apiCEC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDAC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDMS.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDMX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDMX_tee.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDMX_tsio.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGFX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGOP.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGOPSC_Ex.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGOP_SC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGPD.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGPD_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"apiHDMITx.h\"" >> Ksym/Ksym.c
    echo "#include \"apiHDMITx2.h\"" >> Ksym/Ksym.c
    echo "#include \"apiJPEG.h\"" >> Ksym/Ksym.c
    echo "#include \"apiJPEG_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"apiLD.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMBX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiMFE_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"apiMHL.h\"" >> Ksym/Ksym.c
    echo "#include \"apiPNL.h\"" >> Ksym/Ksym.c
    echo "#include \"apiPNL_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiSWI2C.h\"" >> Ksym/Ksym.c
    echo "#include \"apiVDEC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiVDEC_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiVDEC_EX_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"apiVOIPMFE.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Ace.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Ace_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Adc.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Adc_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Auto.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Auto_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_DWIN.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_PCMonitor.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_ModeParse.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_DWIN_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Dlc.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_ModeParse_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_PCMonitor_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_VMark.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_VMark_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"asmCPU.h\"" >> Ksym/Ksym.c
    echo "#include \"drvAESDMA.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCIPHER.h\"" >> Ksym/Ksym.c
    echo "#include \"drvAUDIO.h\"" >> Ksym/Ksym.c
    echo "#include \"drvAVD.h\"" >> Ksym/Ksym.c
    echo "#include \"drvBDMA.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCA.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCA_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCH34.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCIPHER_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCLKM.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCMAPool_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCMDQ.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCPU.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDDC2BI.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDIP.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_ATSC.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_ATSC_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_DTMB.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_DTMB_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_EXTERN_MSB123xc.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_EXTERN_MSB124x.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_common.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_ISDBT.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_ISDBT_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_VD_MBX.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDSCMB.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDSCMB_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvEMMFlt.h\"" >> Ksym/Ksym.c
    echo "#include \"drvGPIO.h\"" >> Ksym/Ksym.c
    echo "#include \"drvHDCP.h\"" >> Ksym/Ksym.c
    echo "#include \"drvHVD.h\"" >> Ksym/Ksym.c
    echo "#include \"drvHWI2C.h\"" >> Ksym/Ksym.c
    echo "#include \"drvIPAUTH.h\"" >> Ksym/Ksym.c
    echo "#include \"drvIR.h\"" >> Ksym/Ksym.c
    echo "#include \"drvIRQ.h\"" >> Ksym/Ksym.c
    echo "#include \"drvLDM.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMFC.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMIU.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMMIO.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMPIF.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMSPI.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMVD.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMVOP.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMVOP_v2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvNDS.h\"" >> Ksym/Ksym.c
    echo "#include \"drvNSK2.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPARFLASH.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPCMCIA.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPM.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPQ.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPQ_cus.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPVR_IframeLUT.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPWM.h\"" >> Ksym/Ksym.c
    echo "#include \"drvPWS.h\"" >> Ksym/Ksym.c
    echo "#include \"drvRTC.h\"" >> Ksym/Ksym.c
    echo "#include \"drvRVD.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSAR.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSC.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSEAL.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSEM.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSERFLASH.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSMBX.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSYS.h\"" >> Ksym/Ksym.c
    echo "#include \"drvTCF.h\"" >> Ksym/Ksym.c
    echo "#include \"drvTVEncoder.h\"" >> Ksym/Ksym.c
    echo "#include \"drvUART.h\"" >> Ksym/Ksym.c
    echo "#include \"drvURDMA.h\"" >> Ksym/Ksym.c
    echo "#include \"drvVBI.h\"" >> Ksym/Ksym.c
    echo "#include \"drvVIF.h\"" >> Ksym/Ksym.c
    echo "#include \"drvWBLE.h\"" >> Ksym/Ksym.c
    echo "#include \"drvWDT.h\"" >> Ksym/Ksym.c
    echo "#include \"drvXC_HDMI_if.h\"" >> Ksym/Ksym.c
    echo "#include \"drvXC_HDMI_if_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"drv_IR_TX.h\"" >> Ksym/Ksym.c
    echo "#include \"drv_gop_sc.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGFX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiVDEC_EX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC.h\"" >> Ksym/Ksym.c
    echo "#include \"drvCIPHER.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_common.h\"" >> Ksym/Ksym.c
    echo "#include \"drvLDM.h\"" >> Ksym/Ksym.c
    echo "#include \"drvSAR.h\"" >> Ksym/Ksym.c
    echo "#include \"drvTVEncoder.h\"" >> Ksym/Ksym.c
    echo "#include \"MsOS.h\"" >> Ksym/Ksym.c
    echo "#include \"apiAUDIO.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDAC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDMS.h\"" >> Ksym/Ksym.c
    echo "#include \"apiDMX.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGOP.h\"" >> Ksym/Ksym.c
    echo "#include \"apiPNL.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_DWIN.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_Dlc.h\"" >> Ksym/Ksym.c
    echo "#include \"drvDMD_ATSC.h\"" >> Ksym/Ksym.c
    echo "#include \"drvMVOP.h\"" >> Ksym/Ksym.c
    echo "#include \"apiGOP.h\"" >> Ksym/Ksym.c
    echo "#include \"apiXC_DBC.h\"" >> Ksym/Ksym.c
    echo "#include \"apiMBX.h\"" >> Ksym/Ksym.c
    echo "#include \"cusSAR.h\"" >> Ksym/Ksym.c
    arm-none-linux-gnueabi-nm utpa2k.ko | grep " T " | awk '{print $3}' | grep "^MDrv\|^MApi\|^MsOS" | awk '{print "EXPORT_SYMBOL("$1");"}' > nm_symbol.txt
    find $PWD/../../mxlib/include/ -name "*.h" | xargs gcc -fpreprocessed -dD -E > preprocess.txt
    find $PWD/../../modules/pq -name "drvPQ.h" | xargs gcc -fpreprocessed -dD -E >> preprocess.txt
    find $PWD/../../modules/pq -name "drvPQ_cus.h" | xargs gcc -fpreprocessed -dD -E >> preprocess.txt
    grep "^\s*\#if\|^\s*\#ifdef\|^\s*\#else\|^\s*\#elif\|^\s*\#endif\|\s\s*MDrv_\|\s\s*MApi_\|\s\s*MsOS_" preprocess.txt | \
        sed 's/^[ ]*//g' | \
        grep -v "\#define\|typedef" | \
        grep -v "\\\\" | \
        awk 'BEGIN{FS="("}{if(substr($1,1,1)=="#"){print}else{if(NF>1){for(i=1;i<=(NF-1);i++){if((match($i,"MDrv_")>0)||(match($i,"MApi_")>0)||(match($i,"MsOS_")>0))print $i}}}}' | \
        awk '{if(substr($1,1,1)=="#"){print}else{print "EXPORT_SYMBOL("$NF");";}}' \
        >> Ksym/Ksym.c
    mv Ksym/Ksym.c Ksym/Ksym_tmp.c
    while read line
    do
        echo $line | grep "^\#ifndef.*_H_$\|^\#ifndef.*_H$" -q
        if [ $? == "0" ] ; then
            echo $line | sed 's/#ifndef/#ifdef/g' >> Ksym/Ksym.c
            continue
        fi
        echo $line | grep "^#" -q
        if [ $? == "0" ] ; then
            echo $line >> Ksym/Ksym.c
            continue
        fi
        grep "$line" nm_symbol.txt -q
        if [ $? == "0" ] ; then
            echo $line >> Ksym/Ksym.c
        fi
    done < Ksym/Ksym_tmp.c
    rm Ksym/Ksym_tmp.c
# exception start
# function
    echo "EXPORT_SYMBOL(mdrv_gpio_set_high);" >> Ksym/Ksym.c
    echo "EXPORT_SYMBOL(mdrv_gpio_set_low);" >> Ksym/Ksym.c
    echo "EXPORT_SYMBOL(MsAPI_CecTxSendPollingMsg);" >> Ksym/Ksym.c
# global var
    echo "EXPORT_SYMBOL(g_IPanel);" >> Ksym/Ksym.c
    echo "EXPORT_SYMBOL(g_IPanelEx);" >> Ksym/Ksym.c
    echo "EXPORT_SYMBOL(g_ACPMissingCounter);" >> Ksym/Ksym.c
    echo "EXPORT_SYMBOL(g_AVMUTEMissingCounter);" >> Ksym/Ksym.c
    echo "EXPORT_SYMBOL(g_AVIMissingCounter);" >> Ksym/Ksym.c
    echo "EXPORT_SYMBOL(g_HdmiPollingStatus);" >> Ksym/Ksym.c
# exception end
}
CreateKsymCFile
