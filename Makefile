ifeq ($(WIFI_DRIVER_DIR),)
DRV_PATH := $(PWD)
else
DRV_PATH := $(WIFI_DRIVER_DIR)
endif

ARCH ?= mips
CROSS_COMPILE ?= mipsel-linux-

export ARCH
export CROSS_COMPILE

obj-m += wq9001.o

wq9001-y= wifi_drv.o
wq9001-y+= oss/osal.o
wq9001-y+= oss/mmal.o
wq9001-y+= oss/taskq.o
wq9001-y+= oss/mbuf.o
wq9001-y+= oss/w_debug.o
wq9001-y+= oss/net_if.o
wq9001-y+= oss/oss.o
wq9001-y+= oss/usb.o

#wproto
#wq9001-y+= wproto/net80211_main.o
wq9001-y+= wproto/net80211/ieee80211.o
wq9001-y+= wproto/net80211/ieee80211_action.o
wq9001-y+= wproto/net80211/ieee80211_ageq.o
wq9001-y+= wproto/net80211/ieee80211_crypto.o
wq9001-y+= wproto/net80211/ieee80211_crypto_ccmp.o
wq9001-y+= wproto/net80211/ieee80211_crypto_none.o
wq9001-y+= wproto/net80211/ieee80211_crypto_tkip.o
wq9001-y+= wproto/net80211/ieee80211_crypto_wep.o
wq9001-y+= wproto/net80211/ieee80211_hostap.o
wq9001-y+= wproto/net80211/ieee80211_ht.o
wq9001-y+= wproto/net80211/ieee80211_input.o
wq9001-y+= wproto/net80211/ieee80211_ioctl.o
wq9001-y+= wproto/net80211/ieee80211_monitor.o
wq9001-y+= wproto/net80211/ieee80211_node.o
wq9001-y+= wproto/net80211/ieee80211_output.o
wq9001-y+= wproto/net80211/ieee80211_phy.o
wq9001-y+= wproto/net80211/ieee80211_power.o
wq9001-y+= wproto/net80211/ieee80211_proto.o
wq9001-y+= wproto/net80211/ieee80211_regdomain.o
wq9001-y+= wproto/net80211/ieee80211_scan.o
wq9001-y+= wproto/net80211/ieee80211_scan_sta.o
wq9001-y+= wproto/net80211/ieee80211_scan_sw.o
wq9001-y+= wproto/net80211/ieee80211_sim.o
wq9001-y+= wproto/net80211/ieee80211_sta.o
wq9001-y+= wproto/net80211/ieee80211_xauth.o
wq9001-y+= wproto/net80211/ieee80211_extap.o

#wdrv
wq9001-y+= wdrv/driver_main.o
wq9001-y+= wdrv/mwl/if_hawk.o
wq9001-y+= wdrv/mwl/if_hawk_port.o
wq9001-y+= wdrv/mwl/hawkhal.o
wq9001-y+= wdrv/mwl/wq_hal_rx.o
wq9001-y+= wdrv/mwl/wq_ipc.o
wq9001-y+= wdrv/mwl/wq_htc_desc.o


subdir-ccflags-y +=  -I$(DRV_PATH)/wdrv -I$(DRV_PATH)/wdrv/mwl  -I$(DRV_PATH)/oss -I$(DRV_PATH)/wproto

ccflags-y += -DWQ_DEBUG
#ccflags-y += -DWQ_SIM_HOST_ONLY
#ccflags-y += -DWQ_BUS_TEST
ccflags-y += -DWQ_FW_DL
#ccflags-y += -DWQ_ADD_AUTH_ASOC_IE
#ccflags-y += -DWQ_LOAD_CALI_FROM_FILE

ccflags-y += -DSTA_VIF_NAME='"wlan0"'
ccflags-y += -DSOFTAP_VIF_NAME='"wlan1"'
ccflags-y += -DMON_VIF_NAME='"mon.wlan1"'

#log: 0(none), 1(oops), 2(err), 3(wrn), 4(inf)
ccflags-y += -DWQ_LOG_USB_SEV=4
ccflags-y += -DWQ_LOG_IPC_SEV=4

#System type: 0(32bit), 1(64bit)
CONFIG_64_BIT=0
ccflags-y += -DWQ_CONFIG_64=$(CONFIG_64_BIT)

ifeq ($(CONFIG_64_BIT),1)
CONFIG_USB_MSG_TP =2
endif

#EP_TYPE: 2(BULK), 3(INT)
CONFIG_USB_MSG_TP ?= 2
ccflags-y += -DWQ_MSG_IN_EP_TYPE=$(CONFIG_USB_MSG_TP)
ccflags-y += -DWQ_MSG_OUT_EP_TYPE=$(CONFIG_USB_MSG_TP)

#USB_FIFO_NUM
CONFIG_USB_FIFO_NUM ?= 8
ccflags-y += -DWQ_USB_FIFO_NUM=$(CONFIG_USB_FIFO_NUM)

#USB_RX_TYPE: 1(DMA), 2(CPU)
CONFIG_USB_RX_TYPE ?= 1
ccflags-y += -DWQ_USB_RX_TYPE=$(CONFIG_USB_RX_TYPE)

#fw source: 0(.h), 1(file)
ccflags-y += -DWQ_FW_SRC=0

ccflags-y += -fmax-errors=5
ccflags-y += -Werror
#workaround TODO
ccflags-y += -Wno-strict-prototypes
#ccflags-y += -Wno-int-to-pointer-cast

#ccflags-y += -O0

#branch version: 0(master), 1(TP)
CONFIG_BRANCH_VERSION ?= 0
ccflags-y += -DBRANCH_VERSION=$(CONFIG_BRANCH_VERSION)

#build version
CONFIG_BUILD_VERSION ?= 989
ccflags-y += -DBUILD_VERSION=$(CONFIG_BUILD_VERSION)

#mac addr: 0(random addr from driver), 1(from efuse)
CONFIG_MAC_FROM_EFUSE ?= 0
ccflags-y += -DWQ_MAC_FROM_EFUSE=$(CONFIG_MAC_FROM_EFUSE)

all:
ifeq ($(KSRC),)
	$(error KSRC must be defined. Please specify the kernel source directory)
endif
	$(MAKE) V=1 -C $(KSRC) M=$(DRV_PATH) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		modules

clean:
	rm -f *.o *.ko *.mod.c *.mod.o .*.cmd modules.order Module.symvers
	rm -rf .tmp_versions
	find . -name '*.o' -delete
	find . -name '.*.o.d' -delete
	find . -name '.*.o.cmd' -delete
