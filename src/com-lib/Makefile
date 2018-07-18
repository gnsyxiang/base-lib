# ===============================================================
# 
# Release under GPLv2.
# 
# @file    Makefile
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    15/05 2018 14:08
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        15/05 2018      create the file
# 
#     last modified: 15/05 2018 14:08
# ===============================================================

include ../../configs/com-var-def.mk
include ../../configs/com-ruler-def.mk

ZLIB_DIR 	:= zlib
ZLIB_NEW_RELEASE 	:= v1.2.9
ZLIB_GIT_PATH 	 	:= https://github.com/madler/zlib.git

ALSA_LIB_NEW_RELEASE 	:= alsa-lib-1.1.6
ALSA_LIB_FTP_PATH 		:= ftp://ftp.alsa-project.org/pub/lib

JPEG_DIR 				:= jpeg-9
JPEG_NEW_RELEASE 		:= jpegsrc.v9
JPEG_FTP_PATH 			:= http://www.ijg.org/files

#################################################################
all: prepare $(ZLIB_DIR) $(ALSA_LIB_NEW_RELEASE) $(JPEG_DIR)
	$(MKDIR) ../../lib/com-lib
	$(CP) ./install/* ../../lib/com-lib

prepare:
	$(MAKE1) clean
	$(MKDIR) $(BUILD_DIR) $(INSTALL_DIR)

######################################
#
# alsa-lib
#
######################################
$(JPEG_DIR): $(JPEG_NEW_RELEASE)-src $(JPEG_DIR)-make

$(JPEG_DIR)-make:
	$(MKDIR) $(BUILD_DIR)/$(@:-make=)
	cd $(BUILD_DIR)/$(@:-make=) \
		&& $(ROOT)/$(@:-make=)/configure --host=$(HOST) --prefix=$(PREFIX) CC=$(CC) --enable-shared \
		&& $(MAKE) && make install

$(JPEG_NEW_RELEASE)-src:
ifneq ($(JPEG_DIR), $(wildcard $(JPEG_DIR)))
	$(WGET) $(JPEG_FTP_PATH)/$(@:-src=).tar.gz
	$(TAR_GZ) $(@:-src=).tar.gz
	$(RM) $(@:-src=).tar.gz
endif

######################################
#
# alsa-lib
#
######################################
$(ALSA_LIB_NEW_RELEASE): $(ALSA_LIB_NEW_RELEASE)-src $(ALSA_LIB_NEW_RELEASE)-make

$(ALSA_LIB_NEW_RELEASE)-make:
	$(MKDIR) $(BUILD_DIR)/$(@:-make=)
	cd $(BUILD_DIR)/$(@:-make=) \
		&& $(ROOT)/$(@:-make=)/configure --host=$(HOST) --prefix=$(PREFIX) CC=$(CC) --enable-shared --disable-python \
		&& $(MAKE) && make install \
		&& $(STRIP) --strip-unneeded $(INSTALL_DIR)/lib/libasound.so.2.0.0

$(ALSA_LIB_NEW_RELEASE)-src:
ifneq ($(ALSA_LIB_NEW_RELEASE), $(wildcard $(ALSA_LIB_NEW_RELEASE)))
	$(WGET) $(ALSA_LIB_FTP_PATH)/$(@:-src=).tar.bz2
	$(TAR_BZ2) $(@:-src=).tar.bz2
	$(RM) $(@:-src=).tar.bz2
endif

######################################
#
# zlib: 用来支持数据压缩或解压的函数库
#
######################################
$(ZLIB_DIR): $(ZLIB_DIR)-src $(ZLIB_DIR)-make

$(ZLIB_DIR)-make:
	$(MKDIR) $(BUILD_DIR)/$(@:-make=)
	cd $(BUILD_DIR)/$(@:-make=) \
		&& export CC=$(CC) export CXX=$(CXX) \
		&& $(ROOT)/$(@:-make=)/configure --prefix=$(PREFIX) \
		&& $(MAKE) && make install \
		&& $(STRIP) --strip-unneeded $(INSTALL_DIR)/lib/libz.so.1.2.9
	
$(ZLIB_DIR)-src:
ifneq ($(ZLIB_DIR), $(wildcard $(ZLIB_DIR)))
	git clone $(ZLIB_GIT_PATH)
	cd $(@:-src=) && git co -b dev $(ZLIB_NEW_RELEASE)
endif

#################################################
err_no_targets:
	@echo "error: use \"targets = your_target\" to specify your target to make!"
	exit 1

ifeq ($(V),1)
slient_targets=err_no_targets
endif

.SILENT: $(slient_targets)
#################################################

clean:
	$(RM) $(BUILD_DIR) $(INSTALL_DIR)

distclean: clean
	$(RM) $(ZLIB_DIR) $(ALSA_LIB_NEW_RELEASE) $(JPEG_DIR)

.PHONY: all clean distclean

