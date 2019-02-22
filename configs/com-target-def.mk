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

###################################################
$(DEPEND_DIR):
	$(MKDIR) $(LIB_DIR)

###################################################
$(TARGET_LIB): $(TARGET_PATH) handle_lib

$(TARGET_PATH): $(OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $(CFLAGS) $^ $(LIB_LDFLAGS) -o $@
	$(strip_obj)

handle_lib: clean_lib
	$(LN) $(TARGET_LIB) $(LIB_DIR)/$(TARGET_LIB_MAJ)
	$(LN) $(TARGET_LIB_MAJ) $(LIB_DIR)/$(TARGET_LIB_SO)

clean_lib:
	$(RM) $(LIB_DIR)/$(TARGET_LIB_MAJ)
	$(RM) $(LIB_DIR)/$(TARGET_LIB_SO)

###################################################
$(DEMO): $(DEMO_OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	$(strip_obj)

###################################################
# --------
# make *.c
# --------
$(OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(LIB_CFLAGS) $< -o $@

$(DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(LIB_CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(DEPS)

$(DEMO_OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(CFLAGS) $< -o $@

$(DEMO_DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(DEMO_DEPS)

###################################################
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
	$(RM) $(DEMO_OBJS)
	$(RM) $(DEMO_DEPS)
	$(RM) $(DEMO)

distclean: clean index-clean
ifeq ($(TARGET_LIB_NAME)x, base-libx)
	$(run-dir-makefile-clean-distclean)
	$(RM) $(INC_DIR)
endif
	$(RM) $(OBJ_DIR)
	$(RM) $(LIB_DIR)

###################################################
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

#########################################################
.PHONY: all clean distclean debug

