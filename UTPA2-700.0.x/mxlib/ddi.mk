
DRV_DIR = \
            demod             \
            audsp                                                   \
            avd                                                     \
            vif                                                     \
            cmdq                                                     \
            bdma                                                    \
            cpu                                                     \
            ir_tx                                                     \
            ddc2bi                                                  \
            urdma		                                    \
            flash/serial                                            \
            flash/nor                                               \
            mpif                                                    \
            ge                                                      \
            gpio                                                    \
            hvd                                                     \
            hvd_v3                                                 \
            hvd_lite                                                \
            hwi2c                                                   \
            ipauth                                                  \
            ir                                                      \
            irq                                                     \
            jpd                                                     \
            mbx                                                     \
            miu                                                     \
            mjpeg_v3                                                     \
            mjpeg_lite                                                     \
            mmio                                                    \
            mvd                                                     \
            mvd_v3                                                     \
            mvd_lite                                                \
            mvop                                                    \
            nds                                                     \
            nsk2                                                    \
            pcmcia                                                  \
            pwm                                                     \
            pq                                                      \
            rasp                                                    \
            rvd                                                     \
            sar                                                     \
            sc                                                      \
            sem                                                     \
            sys                                                     \
            tsp                                                     \
            tsp2                                                    \
            tsp3                                                    \
            tsp4                                                    \
            tsp_tee                                                 \
            pvr_iframelut                                           \
            uart                                                    \
            vbi                                                     \
            ve                                                      \
            wdt                                                     \
            rtc														\
            pws                                                     \
            wble                                                    \
            pm       	                                            \
            mfc                                                     \
            nga                                                     \
            dmd                                                     \
            dip                                                     \
            usb_ecos/newhost                                        \
            tso                                                     \
            tso2                                                    \
            mspi                                                    \
            seal                                                    \
            ch34                                                    \
            dvb_extdemod                                            \
            clkm													\
            otv														\
            hdcp													\
            ldm                                                     \
            ldma

API_DIR = \
            ace                                                     \
            dmx                                                     \
            dmx_tee                                                 \
            gop                                              	    \
            gfx                                                     \
            cec                                                     \
            dlc                                                     \
            pnl                                                     \
            xc                                                      \
            vdec                                                    \
            vdec_ex                                                 \
            vdec_v3                                                 \
            vdec_lite                                               \
            audio                                                   \
            swi2c                                                   \
            jpeg                                                    \
            secure                                                  \
            hsl                                                     \
            gopsc                                                   \
            gopscd                                                  \
            gpd                                                     \
            mhl                                                     \
            nsk2hdi                                                 \

MMFI_CHIPS = t12 j2 a2 a1 k1 a5 a7 a3 k2 a5p amethyst e3 macaw12 emerald eden euler edison keres kirin munich
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI_CHIPS)))
DRV_DIR += mmfi
endif

MMFI2_CHIPS = agate einstein eiffel einstein3 keres kirin monaco muji miami monet manhattan messi maserati kano curry k6 maxim k7u M7821 M7621
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI2_CHIPS)))
DRV_DIR += mmfi2
endif

TCF_CHIPS = manhattan maserati k6lite M7821
ifeq ($(CHIP), $(filter $(CHIP),$(TCF_CHIPS)))
DRV_DIR += tcf
endif

ifeq ($(OS_TYPE), linux)
MFE_CHIPS = t8 j2 t12 a2 a5 a3 e3 kaiser munich
ifeq ($(CHIP), $(filter $(CHIP),$(MFE_CHIPS)))
API_DIR += mfe
endif

MFE_EX_CHIPS = kano curry k6 k7u
ifeq ($(CHIP), $(filter $(CHIP),$(MFE_EX_CHIPS)))
API_DIR += mfe_ex
endif

ifeq ($(PLATFORM), android) # android
API_DIR += mfe_ex
endif
endif

BOX_CHIPS = u3 u4 k1 k2 kaiser keltic keres kirin kris clippers kano curry k6 k6lite k7u

ifeq ($(CHIP), $(filter $(CHIP),$(BOX_CHIPS)))
API_DIR +=  hdmitx                                                  \
            hdmitx2                                                  \
            dac                                                     \
            acp                                                     \

endif

TV_HDMITX_CHIPS = agate maxim M7621
ifeq ($(CHIP), $(filter $(CHIP),$(TV_HDMITX_CHIPS)))
API_DIR +=  hdmitx                                                  \
            hdmitx2                                                  \

endif

NJPD_CHIPS = agate eagle e3 eiffel edison einstein nike madison miami einstein3 monaco clippers muji monet manhattan messi maserati maxim kano curry k6 k6lite k7u M7821 M7621
ifeq ($(CHIP), $(filter $(CHIP),$(NJPD_CHIPS)))
API_DIR += njpeg_ex
else
API_DIR += jpeg_ex
endif

ifeq ($(CONFIG_API_LOCALDIMM), y)
API_DIR += localdimm
endif

NEW_CA_CHIPS = kaiser keltic keres kirin kano curry k6 k6lite k7u

ifeq ($(CHIP), $(filter $(CHIP),$(NEW_CA_CHIPS)))
DRV_DIR +=  dscmb2
DRV_DIR +=  cipher
DRV_DIR +=  ca2
else
DRV_DIR += dscmb
DRV_DIR += aesdma
DRV_DIR += ca
endif


# to avoid "make" implicitly becomes "make amazon"
all:

# external demod
amazon:
	@echo [CC] halDMD_EXTERN_ISDBT.c
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) $(DRV_CC_INCS) -I$(ROOT)/mxlib/include -o libAMAZON.o $(ROOT)/mxlib/hal/amazon/halDMD_EXTERN_ISDBT.c
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) libAMAZON.o -s -o libAMAZON.$(LIB_EXT)
else
	@$(AR) cru libAMAZON.$(LIB_EXT) libAMAZON.o
	@$(STRIP) -S libAMAZON.$(LIB_EXT)
endif
	@rm -rf libAMAZON.o
	@mv libAMAZON.$(LIB_EXT) $(DDI_PATH)

galileo:
	@echo [CC] halDMD_EXTERN_ATSC.c
	@$(CC) $(CC_OPTS) $(CC_DEFS) -Dgalileo $(CC_INCS) $(DRV_CC_INCS) -I$(ROOT)/mxlib/include -o libgalileo.o $(ROOT)/mxlib/hal/galileo/halDMD_EXTERN_ATSC.c
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) libgalileo.o -s -o libgalileo.$(LIB_EXT)
else
	@$(AR) cru libgalileo.$(LIB_EXT) libgalileo.o
	@$(STRIP) -S libgalileo.$(LIB_EXT)
endif
	@rm -rf libgalileo.o
	@mv libgalileo.$(LIB_EXT) $(DDI_PATH)

cashmere:
	@echo [CC] halDMD_EXTERN_DTMB.c
	@$(CC) $(CC_OPTS) $(CC_DEFS) -Dcashmere $(CC_INCS) $(DRV_CC_INCS) -I$(ROOT)/mxlib/include -o libcashmere.o $(ROOT)/mxlib/hal/cashmere/halDMD_EXTERN_DTMB.c
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) libcashmere.o -s -o libcashmere.$(LIB_EXT)
else
	@$(AR) cru libcashmere.$(LIB_EXT) libcashmere.o
	@$(STRIP) -S libcashmere.$(LIB_EXT)
endif
	@rm -rf libcashmere.o
	@mv libcashmere.$(LIB_EXT) $(DDI_PATH)
