ifeq ($(CONFIG),FOUNDER)
BIN_SUFFIX:=$(BIN_SUFFIX)-founder
CPPFLAGS+=-DRT1052_FOUNDER=1
MCU=MIMXRT1052
$(info Config = FOUNDER)

else
$(error Missing/Unknown config '$(CONFIG)' (set SCCC_CONFIG))

endif
