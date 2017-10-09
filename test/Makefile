# ===============================================================
# Filename: Makefile
# -------------------------------
# Copyright(C),
# Author: zhenquan.qiu
# Version: V1.0.0
# Last modified: 23/09 2017 17:06
# Description:
#
# Change Log:
# NO.	Author		    Date		Modified
# 00	zhenquan.qiu	23/09 2017
# ===============================================================

ROOT := $(shell pwd)
TARGET ?= main

OBJ_DIR ?= objs
INC_DIR ?= ../../include
LIB_DIR ?= ../../lib

Q := @
RM := $(Q)rm -rf

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
CFLAGS := -Wall
CFLAGS += -I. -I$(INC_DIR)

MSG_CC := CC

# -------
# ldflags
# -------

LDFLAGS :=
LDFLAGS += -L$(LIB_DIR)
LDFLAGS += -lbase_lib
LDFLAGS += -lpthread
LDFLAGS += -Wl,-rpath ~/test/skills/lib

MSG_LD := LD
# -------
# c files
# -------
SRC_C := $(wildcard *.c)
OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_C))
DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(SRC_C))
DEPS  ?= $(DEP_C)
OBJS  ?= $(OBJ_C)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(Q)echo $(MSG_LD) $<
	$(Q)$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

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

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TARGET)

distclean: clean
	$(RM) $(OBJ_DIR)

debug:

.PHONY: all clean distclean debug

