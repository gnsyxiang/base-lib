# ===============================================================
# 
# Release under GPLv2.
# 
# @file    Makefile
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    28/02 2018 17:02
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        28/02 2018      create the file
# 
#     last modified: 28/02 2018 17:02
# ===============================================================

# --------------
# target setting
# --------------
TARGET_LIB_NAME ?= base-lib

include configs/com-var-def.mk
include configs/com-ruler-def.mk

subdir-src 		:= src/utils

###########################################
all: $(DEPEND_DIR)
	$(run-dir-makefile-make)

include configs/com-target-def.mk

