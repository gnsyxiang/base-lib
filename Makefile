# ===============================================================
# Filename: Makefile
# -------------------------------
# Copyright(C),
# Author: zhenquan.qiu
# Version: V1.0.0
# Last modified: 22/09 2017 21:39
# Description:
#
# Change Log:
# NO.	Author		    Date		Modified
# 00	zhenquan.qiu	22/09 2017
# ===============================================================

ROOT := $(shell pwd)

OBJ_DIR ?= objs
LIB_DIR ?= lib
INC_DIR ?= include
SRC_DIR ?= src
TST_DIR ?= test

Q := @

RM := $(Q)rm -rf
ECHO := $(Q)echo
MKDIR := $(Q)mkdir -p
LN := $(Q)ln -s
CP := $(Q)cp -ar

MAJOR_VERSION := 1
MINOR_VERSION := 0
BUG_VERSION := 0

TARGET := base_lib
TARGET_SO := lib$(TARGET).so.$(MAJOR_VERSION).$(MINOR_VERSION).$(BUG_VERSION)
TARGET_PATH := $(LIB_DIR)/$(TARGET_SO)
TST_DEMO := main

MSG_LIB := LIB_COPY

# --------
# compiler
# --------
GCC_PATH := ~/office/ingenic/gcc/mips-gcc520-32bit/bin
GCC_NAME := mips-linux-gnu-

#TARGET_SYSTEM   := x1800

ifeq ($(TARGET_SYSTEM), x1800)
	CROSS_TOOL := $(GCC_PATH)/$(GCC_NAME)
	LDFLAGS := -T configs/ldscript-mips.lds
else
	LDFLAGS := -T configs/ldscript.lds
endif


CC := $(Q)$(CROSS_TOOL)gcc

# ------
# cflags
# ------
CFLAGS := -Wall -Werror -g -I$(INC_DIR) -std=gnu99
LIB_CFLAGS := $(CFLAGS) -fPIC

MSG_CC := CC

# -------
# ldflags
# -------
SO_NAME := -Wl,-soname,lib$(TARGET).so.$(MAJOR_VERSION)

LDFLAGS += -lbase_lib -lpthread -L./lib -Wl,-rpath=./lib
LIB_LDFLAGS := $(SO_NAME) -shared

MSG_LD := LD


# -------
# h files
# -------
INC_C := $(wildcard $(INC_DIR)/*.h)

MSG_INC := INC_COPY
# -------
# c files
# -------
SRC_C := $(wildcard $(SRC_DIR)/*.c)
OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_C))
DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(SRC_C))
DEPS  ?= $(DEP_C)
OBJS  ?= $(OBJ_C)

TST_SRC_C := $(wildcard $(TST_DIR)/*.c)
TST_OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(TST_SRC_C))
TST_DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(TST_SRC_C))
TST_DEPS  ?= $(TST_DEP_C)
TST_OBJS  ?= $(TST_OBJ_C)

all: $(TARGET_PATH) handle_lib $(TST_DEMO)

$(TARGET_PATH): $(OBJS)
	$(ECHO) $(MSG_LD) $<
	$(MKDIR) $(LIB_DIR)
	$(CC) $(OBJS) $(LIB_LDFLAGS) -o $@

$(TST_DEMO): $(TST_OBJS)
	$(ECHO) $(MSG_LD) $<
	$(MKDIR) $(LIB_DIR)
	$(CC) $(TST_OBJS) $(LDFLAGS) -o $@

handle_lib: clean_lib ln_lib cp_lib
	$(ECHO) "copy lib and inc successful"

clean_lib:
	$(RM) $(LIB_DIR)/lib$(TARGET).so
	$(RM) $(LIB_DIR)/lib$(TARGET).so.$(MAJOR_VERSION)

ln_lib:
	$(LN) $(TARGET_SO) $(LIB_DIR)/lib$(TARGET).so
	$(LN) $(TARGET_SO) $(LIB_DIR)/lib$(TARGET).so.$(MAJOR_VERSION)

cp_lib:
	$(CP) $(LIB_DIR)/* ../lib/
	$(CP) $(INC_C) ../include/

#
# make *.c
#
$(OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(LIB_CFLAGS) $< -o $@

$(DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(LIB_CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(DEPS)

$(TST_OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(CFLAGS) $< -o $@

$(TST_DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(TST_DEPS)

#################################################

push:
	adb push ./lib /xia/base_lib/lib/
	adb push $(TST_DEMO) /xia/base_lib/

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TST_OBJS)
	$(RM) $(TST_DEPS)
	$(RM) $(TST_DEMO)

distclean: clean index-clean
	$(RM) $(OBJ_DIR)
	$(RM) $(LIB_DIR)

index: index-clean
	$(ECHO) generate index
	$(Q)ctags -R
	$(Q)cscope -Rbkq

index-clean:
	$(RM) *.out
	$(RM) tags

note:
	doxygen configs/Doxyfile

debug:
	echo $(SRC_DIR)

.PHONY: all clean distclean debug

