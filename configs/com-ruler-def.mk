# ===============================================================
# 
# Release under GPLv2.
# 
# @file    com-ruler-def.mk
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    25/05 2018 16:15
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        25/05 2018      create the file
# 
#     last modified: 25/05 2018 16:15
# ===============================================================


define run-dir-makefile-clean-distclean
	for dir in $(subdir-src); 			\
	do 									\
		$(MAKE) -C $$dir $@ || exit 1; 	\
	done
endef

define run-dir-makefile-make
	for dir in $(subdir-src); 			\
	do 									\
		$(ECHO) "--->> "$$dir; 			\
		$(MAKE) -C $$dir || exit 1; 	\
	done
endef

define cp-lib-inc
	$(MKDIR) $(TO_TOP_DIR)/$(INC_DIR)/$(TARGET_LIB_NAME)
	\
	$(CP) $(LIB_DIR)/$(TARGET_LIB_SO)* 	$(TO_TOP_DIR)/$(LIB_DIR)
	$(CP) $(INC_DIR)/* 					$(TO_TOP_DIR)/$(INC_DIR)/$(TARGET_LIB_NAME)
endef

ifeq ($(STRIP_OBJ), y)
define strip_obj
		$(STRIP) --strip-unneeded $@;
endef
endif


