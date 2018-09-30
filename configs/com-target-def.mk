# ===============================================================
# 
# Release under GPLv2.
# 
# @file    com-target-def.mk
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    30/09 2018 10:37
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        30/09 2018      create the file
# 
#     last modified: 30/09 2018 10:37
# ===============================================================


index: index-clean
	$(ECHO) generate index
	ctags -R
	cscope -Rbkq

index-clean:
	$(RM) *.out
	$(RM) tags

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TARGET_DEMO_OBJS)
	$(RM) $(TARGET_DEMO_DEPS)
	$(RM) $(TARGET_DEMO)
	$(RM) $(CON_DIR)

distclean: clean index-clean
	$(RM) $(OBJ_DIR)
	$(RM) $(LIB_DIR)

depend:
	$(RM) $(CON_DIR)
	$(LN) $(TO_TOP_DIR)/$(CON_DIR) $(CON_DIR)

$(TARGET_LIB): $(TARGET_PATH) handle_lib

$(TARGET_PATH): $(OBJS)
	$(ECHO) $(MSG_LD) $@
	$(MKDIR) $(LIB_DIR)
	$(CC) $(CFLAGS) $^ $(LIB_LDFLAGS) -o $@
	$(strip_obj)

$(TARGET_DEMO): $(TARGET_DEMO_OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	$(strip_obj)

handle_lib: clean_lib
	$(LN) $(TARGET_LIB) $(LIB_DIR)/$(TARGET_LIB_MAJ)
	$(LN) $(TARGET_LIB_MAJ) $(LIB_DIR)/$(TARGET_LIB_SO)

clean_lib:
	$(RM) $(LIB_DIR)/$(TARGET_LIB_MAJ)
	$(RM) $(LIB_DIR)/$(TARGET_LIB_SO)

note:
	doxygen configs/Doxyfile

#########################################################
err_no_targets:
	@echo "error: use \"targets = your_target\" to specify your target to make!"
	exit 1

ifeq ($(V),1)
slient_targets=err_no_targets
endif

.SILENT: $(slient_targets)

