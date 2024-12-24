ifeq ($(WIFI_DRIVER_DIR),)
DRV_PATH := $(PWD)
else
DRV_PATH := $(WIFI_DRIVER_DIR)
endif


obj-m += hawk_usb.o

hawk_usb-y= wifi_drv.o
hawk_usb-y+= oss/osal.o
hawk_usb-y+= oss/mmal.o
hawk_usb-y+= oss/taskq.o
hawk_usb-y+= oss/mbuf.o
hawk_usb-y+= oss/w_debug.o
hawk_usb-y+= oss/net_if.o
hawk_usb-y+= oss/oss.o
hawk_usb-y+= oss/usb.o

#wproto
#hawk_usb-y+= wproto/net80211_main.o
hawk_usb-y+= wproto/net80211/ieee80211.o
hawk_usb-y+= wproto/net80211/ieee80211_action.o
hawk_usb-y+= wproto/net80211/ieee80211_ageq.o
hawk_usb-y+= wproto/net80211/ieee80211_crypto.o
hawk_usb-y+= wproto/net80211/ieee80211_crypto_ccmp.o
hawk_usb-y+= wproto/net80211/ieee80211_crypto_none.o
hawk_usb-y+= wproto/net80211/ieee80211_crypto_tkip.o
hawk_usb-y+= wproto/net80211/ieee80211_crypto_wep.o
hawk_usb-y+= wproto/net80211/ieee80211_hostap.o
hawk_usb-y+= wproto/net80211/ieee80211_ht.o
hawk_usb-y+= wproto/net80211/ieee80211_input.o
hawk_usb-y+= wproto/net80211/ieee80211_ioctl.o
hawk_usb-y+= wproto/net80211/ieee80211_monitor.o
hawk_usb-y+= wproto/net80211/ieee80211_node.o
hawk_usb-y+= wproto/net80211/ieee80211_output.o
hawk_usb-y+= wproto/net80211/ieee80211_phy.o
hawk_usb-y+= wproto/net80211/ieee80211_power.o
hawk_usb-y+= wproto/net80211/ieee80211_proto.o
hawk_usb-y+= wproto/net80211/ieee80211_regdomain.o
hawk_usb-y+= wproto/net80211/ieee80211_scan.o
hawk_usb-y+= wproto/net80211/ieee80211_scan_sta.o
hawk_usb-y+= wproto/net80211/ieee80211_scan_sw.o
hawk_usb-y+= wproto/net80211/ieee80211_sim.o
hawk_usb-y+= wproto/net80211/ieee80211_sta.o
hawk_usb-y+= wproto/net80211/ieee80211_xauth.o
hawk_usb-y+= wproto/net80211/ieee80211_extap.o

#wdrv
hawk_usb-y+= wdrv/driver_main.o
hawk_usb-y+= wdrv/mwl/if_hawk.o
hawk_usb-y+= wdrv/mwl/if_hawk_port.o
hawk_usb-y+= wdrv/mwl/hawkhal.o
hawk_usb-y+= wdrv/mwl/wq_hal_rx.o
hawk_usb-y+= wdrv/mwl/wq_ipc.o
hawk_usb-y+= wdrv/mwl/wq_htc_desc.o


subdir-ccflags-y +=  -I$(DRV_PATH)/wdrv -I$(DRV_PATH)/wdrv/mwl  -I$(DRV_PATH)/oss -I$(DRV_PATH)/wproto

ccflags-y += -DWQ_DEBUG
#ccflags-y += -DWQ_SIM_HOST_ONLY
#ccflags-y += -DWQ_BUS_TEST
ccflags-y += -DWQ_FW_DL
#ccflags-y += -DWQ_ADD_AUTH_ASOC_IE
#ccflags-y += -DWQ_LOAD_CALI_FROM_FILE

ifeq ($(WIFI_DRIVER_DIR),)
ccflags-y += -DSTA_VIF_NAME='"hawk_sta"'
ccflags-y += -DSOFTAP_VIF_NAME='"hawk_ap"'
ccflags-y += -DMON_VIF_NAME='"mon.hawk_ap"'
ccflags-y += -DTASKNAME_PRINT
else
ccflags-y += -DSTA_VIF_NAME='"wlan0"'
ccflags-y += -DSOFTAP_VIF_NAME='"wlan1"'
ccflags-y += -DMON_VIF_NAME='"mon.wlan1"'
endif

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
CONFIG_USB_MSG_TP ?= 3
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
ifeq ($(WIFI_DRIVER_DIR),)
	make V=1 -C /lib/modules/`uname -r`/build M=`pwd` modules
else
	make V=1 -C $(KSRC) M=$(DRV_PATH) modules
endif


clean:
ifeq ($(WIFI_DRIVER_DIR),)
	make -C /lib/modules/`uname -r`/build M=`pwd` clean
else
	make -C $(KSRC) M=$(DRV_PATH) clean
endif
