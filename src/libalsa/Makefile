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

TO_TOP_DIR 	:= ../..

include $(TO_TOP_DIR)/configs/com-var-def.mk
include $(TO_TOP_DIR)/configs/com-ruler-def.mk

# --------------
# target setting
# --------------
TARGET_DEMO 	?= main
TARGET_LIB_NAME ?= alsa-record

# -------
# version
# -------
MAJOR_VERSION 	?= 1
MINOR_VERSION 	?= 0
RELEASE_VERSION ?= 0

TARGET_LIB 	  	:= lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
TARGET_LIB_MAJ 	:= lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION)
TARGET_LIB_SO 	:= lib$(TARGET_LIB_NAME).so

TARGET_PATH := $(LIB_DIR)/$(TARGET_LIB)

# ------
# cflags
# ------

DEBUG_SWITCH := debug

ifeq ($(DEBUG_SWITCH), debug)
	CFLAGS     += -g
else
	CFLAGS     += -O2 -Wno-error=unused-result -Werror=return-type
endif

CFLAGS     += -Wall -Werror -std=gnu99 $(SYSTEM_32_64) $(HOOK)
CFLAGS     += -I$(INC_DIR) -I$(TO_TOP_DIR)/include
CFLAGS 	   += -Wno-error=unused-function -Wno-error=unused-variable
LIB_CFLAGS += $(CFLAGS) -fPIC

# -------
# ldflags
# -------
SO_NAME 	:= -Wl,-soname=$(TARGET_LIB_MAJ)

LD_COM_FLAG := $(SYSTEM_32_64)

LDFLAGS 	:=
LDFLAGS 	+= -Wl,-rpath=$(LIB_DIR):$(TO_TOP_DIR)/lib $(LD_COM_FLAG)
LDFLAGS 	+= -L$(LIB_DIR) -L$(TO_TOP_DIR)/lib
LDFLAGS 	+= -l$(TARGET_LIB_NAME) -lasound -lbase-lib
LDFLAGS 	+= -lpthread
LIB_LDFLAGS := $(SO_NAME) -shared $(LD_COM_FLAG)

# -------
# h files
# -------
INC_C := $(wildcard $(INC_DIR)/*.h)

# -------
# c files
# -------
SRC_C := $(wildcard $(SRC_DIR)/*.c)
OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_C))
DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(SRC_C))
DEPS  ?= $(DEP_C)
OBJS  ?= $(OBJ_C)

TARGET_DEMO_SRC_C := $(wildcard $(TST_DIR)/*.c)
TARGET_DEMO_OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(TARGET_DEMO_SRC_C))
TARGET_DEMO_DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(TARGET_DEMO_SRC_C))
TARGET_DEMO_DEPS  ?= $(TARGET_DEMO_DEP_C)
TARGET_DEMO_OBJS  ?= $(TARGET_DEMO_OBJ_C)

###########################################
all: $(TARGET_LIB) $(TARGET_DEMO)
	$(CP) $(LIB_DIR)/$(TARGET_LIB_SO)* 	$(TO_TOP_DIR)/lib
	$(CP) $(INC_DIR)/* 					$(TO_TOP_DIR)/include

$(TARGET_LIB): $(TARGET_PATH) handle_lib

$(TARGET_PATH): $(OBJS)
	$(ECHO) $(MSG_LD) $@
	$(MKDIR) $(LIB_DIR)
	$(CC) $^ $(LIB_LDFLAGS) -o $@
	#$(STRIP) --strip-unneeded $@

$(TARGET_DEMO): $(TARGET_DEMO_OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $^ $(LDFLAGS) -o $@
	#$(STRIP) --strip-unneeded $@

handle_lib: clean_lib
	$(LN) $(TARGET_LIB) $(LIB_DIR)/$(TARGET_LIB_MAJ)
	$(LN) $(TARGET_LIB_MAJ) $(LIB_DIR)/$(TARGET_LIB_SO)

clean_lib:
	$(RM) $(LIB_DIR)/$(TARGET_LIB_MAJ)
	$(RM) $(LIB_DIR)/$(TARGET_LIB_SO)

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

$(TARGET_DEMO_OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET_DEMO_DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(TARGET_DEMO_DEPS)

#########################################################
err_no_targets:
	@echo "error: use \"targets = your_target\" to specify your target to make!"
	exit 1

ifeq ($(V),1)
slient_targets=err_no_targets
endif

.SILENT: $(slient_targets)
#########################################################
DEVICE_TEST_PATH_LIB 	:= $(DEVICE_TEST_PATH)/lib

push:
	$(ADB_SHELL) $(MKDIR) 					$(DEVICE_TEST_PATH)
	$(ADB_SHELL) $(MKDIR)  					$(DEVICE_TEST_PATH_LIB)
	\
	$(ADB_PUSH) $(TARGET_DEMO) 				$(DEVICE_TEST_PATH)
	$(ADB_PUSH) $(LIB_DIR)/$(TARGET_LIB)  	$(DEVICE_TEST_PATH_LIB)
	\
	$(ADB_SHELL) $(LN) $(TARGET_LIB) 		$(DEVICE_TEST_PATH_LIB)/$(TARGET_LIB_MAJ)

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TARGET_DEMO_OBJS)
	$(RM) $(TARGET_DEMO_DEPS)
	$(RM) $(TARGET_DEMO)

distclean: clean index-clean
	$(RM) $(OBJ_DIR)
	$(RM) $(LIB_DIR)

index: index-clean
	$(ECHO) generate index
	ctags -R
	cscope -Rbkq

index-clean:
	$(RM) *.out
	$(RM) tags

note:
	doxygen configs/Doxyfile

debug:
	echo $(LDFLAGS)

.PHONY: all clean distclean debug

