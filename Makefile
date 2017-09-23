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

Q := @
RM := $(Q)rm -rf

MAJOR_VERSION := 1
MINOR_VERSION := 0
BUG_VERSION := 0

TARGET := base_lib
TARGET_SO := lib$(TARGET).so.$(MAJOR_VERSION).$(MINOR_VERSION).$(BUG_VERSION)

MSG_LIB := LIB_COPY

# --------
# compiler
# --------
GCC_PATH := ~/Desktop/mips-gcc520-32bit/bin
GCC_NAME := mips-linux-gnu-

# CROSS_TOOL := $(GCC_PATH)/$(GCC_NAME)

CC := $(CROSS_TOOL)gcc

# ------
# cflags
# ------
CFLAGS := -Wall -I$(INC_DIR) -fPIC

MSG_CC := CC

# -------
# ldflags
# -------
SO_NAME := -Wl,-soname,lib$(TARGET).so.$(MAJOR_VERSION)

LDFLAGS := $(SO_NAME) -shared

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

all: clean-lib $(TARGET_SO)

$(TARGET_SO): $(OBJS)
	$(Q)echo $(MSG_LD) $<
	$(Q)$(CC) $(LDFLAGS) $(OBJS)  -o $(LIB_DIR)/$(TARGET_SO)
	$(Q)ln -s $(TARGET_SO) $(LIB_DIR)/lib$(TARGET).so
	$(Q)ln -s $(TARGET_SO) $(LIB_DIR)/lib$(TARGET).so.$(MAJOR_VERSION)
	$(Q)cp $(LIB_DIR)/* ../lib/ -ar
	$(Q)cp $(INC_C) ../include/ -ar

#
# make *.c
#
$(OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(Q)mkdir -p $(dir $@)
	$(Q)echo $(MSG_CC) $<
	$(Q)$(CC) -c $(CFLAGS) $< -o $@

$(DEP_C): $(OBJ_DIR)/%.d : %.c
	$(Q)mkdir -p $(dir $@);
	$(Q)$(CC) -MM $(CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(DEPS)

clean-lib:
	$(RM) $(LIB_DIR)/lib$(TARGET).so
	$(RM) $(LIB_DIR)/lib$(TARGET).so.$(MAJOR_VERSION)
	$(RM) $(LIB_DIR)/$(TARGET_SO)

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TARGET)

distclean: clean index-clean
	$(RM) $(OBJ_DIR)

index: index-clean
	$(Q)echo generate index
	$(Q)ctags -R
	$(Q)cscope -Rbkq

index-clean:
	$(RM) *.out
	$(RM) tags

debug:
	echo $(SRC_DIR)

.PHONY: all clean distclean debug

