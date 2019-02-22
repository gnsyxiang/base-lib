# ===============================================================
# 
# Release under GPLv2.
# 
# @file    Makefile
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    29/05 2018 09:10
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        29/05 2018      create the file
# 
#     last modified: 29/05 2018 09:10
# ===============================================================

# --------------
# target setting
# --------------
DEMO 			?= main
TARGET_LIB_NAME ?= network

# -------
# version
# -------
MAJOR_V   ?= 1
MINOR_V   ?= 0
RELEASE_V ?= 0

TO_TOP_DIR 	:= ../..

include $(TO_TOP_DIR)/configs/com-var-def.mk
include $(TO_TOP_DIR)/configs/com-ruler-def.mk
include $(TO_TOP_DIR)/configs/com-model-def.mk

# ------
# cflags
# ------
# CFLAGS     +=

# -------
# ldflags
# -------
LDFLAGS 	+= -lutils

###########################################
all: $(DEPEND_DIR) $(TARGET_LIB) $(DEMO)
	$(cp-lib-inc)

include $(TO_TOP_DIR)/configs/com-target-def.mk

debug:
	echo $(LDFLAGS)

