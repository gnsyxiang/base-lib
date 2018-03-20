# ===============================================================
# 
# Release under GPLv2.
# 
# @file    Makefile
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    14/03 2018 14:57
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        14/03 2018      create the file
# 
#     last modified: 14/03 2018 14:57
# ===============================================================

# --------------
# target setting
# --------------
TARGET_LIB_NAME ?= cjson

# ------------------
# output information
# ------------------
MSG_CC 	?= CC
MSG_LD 	?= LD

# ----------------
# cmd redefinition
# ----------------
Q 		:= @

RM 		:= $(Q)rm -rf
ECHO 	:= $(Q)echo
MKDIR 	:= $(Q)mkdir -p
LN 		:= $(Q)ln -s
CP 		:= $(Q)cp -ar

# -------
# version
# -------
MAJOR_VERSION 	?= 1
MINOR_VERSION 	?= 7
RELEASE_VERSION ?= 4

TARGET_LIB 	  	:= lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
TARGET_LIB_MAJ 	:= lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION)
TARGET_LIB_SO 	:= lib$(TARGET_LIB_NAME).so

# --------
# compiler
# --------
GCC_PATH 	:= ~/office/ingenic/gcc/mips-gcc520-32bit/bin
GCC_NAME 	:= mips-linux-gnu-

#CROSS_TOOL 	:= $(GCC_PATH)/$(GCC_NAME)

CC 	 	 	:= $(Q)$(CROSS_TOOL)gcc
CXX 	 	:= $(Q)$(CROSS_TOOL)g++
STRIP  	 	:= $(Q)$(CROSS_TOOL)strip

CFLAGS     	:=
LIB_CFLAGS 	:=
LDFLAGS 	:=

# ------
# cflags
# ------
#DEBUG_SWITCH := debug

ifeq ($(DEBUG_SWITCH), debug)
	CFLAGS 	+= -g
else
	CFLAGS 	+= -O2 -Wno-error=unused-result -Werror=return-type
endif

CFLAGS     	+= -Wall -Werror -std=gnu99
CFLAGS     	+= -I.
LIB_CFLAGS 	+= $(CFLAGS) -fPIC

# -------
# ldflags
# -------
SO_NAME 	:= -Wl,-soname=$(TARGET_LIB_MAJ)

LIB_LDFLAGS := $(SO_NAME) -shared

# -------
# c files
# -------
SRC_C := cJSON.c

OBJ_C := $(patsubst %.c, %.o, $(SRC_C))
DEP_C := $(patsubst %.c, %.d, $(SRC_C))
DEPS  ?= $(DEP_C)
OBJS  ?= $(OBJ_C)

#################################################

all: $(TARGET_LIB_NAME)

$(TARGET_LIB_NAME): $(TARGET_LIB) handle_lib

$(TARGET_LIB): $(OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $^ $(LIB_LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

handle_lib: clean_lib
	$(LN) $(TARGET_LIB) $(TARGET_LIB_MAJ)
	$(LN) $(TARGET_LIB_MAJ) $(TARGET_LIB_SO)

clean_lib:
	$(RM) $(TARGET_LIB_MAJ)
	$(RM) $(TARGET_LIB_SO)

# --------
# make *.c
# --------
$(OBJ_C): %.o : %.c
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(LIB_CFLAGS) $< -o $@

$(DEP_C): %.d : %.c
	$(CC) -MM $(LIB_CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(DEPS)

#################################################

debug:

push:
	adb push ./lib /xia/base_lib/lib/
	adb push $(TARGET_DEMO) /xia/base_lib/

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TARGET_DEMO_OBJS)
	$(RM) $(TARGET_DEMO_DEPS)

distclean: clean index-clean
	$(RM) $(TARGET_DEMO)
	$(RM) $(TARGET_LIB)
	$(RM) $(TARGET_LIB_MAJ)
	$(RM) $(TARGET_LIB_SO)

index: index-clean
	$(ECHO) generate index
	$(Q)ctags -R
	$(Q)cscope -Rbkq

index-clean:
	$(RM) *.out
	$(RM) tags

.PHONY: all clean distclean debug

