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
TARGET_DEMO 	?= main

# ------------------
# output information
# ------------------
MSG_CC 		?= CC
MSG_CC_E 	?= CC -E
MSG_LD 		?= LD

# ----------------
# cmd redefinition
# ----------------
Q 		:= @

RM 		:= $(Q)rm -rf
ECHO 	:= $(Q)echo
MKDIR 	:= $(Q)mkdir -p
LN 		:= $(Q)ln -s
CP 		:= $(Q)cp -ar

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
LIB_LDFLAGS :=

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
CFLAGS     	+= -Icjson-helper -Icjson-lib

# -------
# ldflags
# -------
SO_NAME 	:= -Wl,-soname=$(TARGET_LIB_MAJ)

LDFLAGS 	+= -Wl,-rpath=cjson-lib:cjson-helper
LDFLAGS 	+= -Lcjson-helper -Lcjson-lib
LDFLAGS 	+= -lcjson -lcjson-helper

# -------
# c files
# -------
TARGET_DEMO_SRC_C := test.c

OBJ_I 			  := $(patsubst %.c, %.i, $(TARGET_DEMO_SRC_C))
TARGET_DEMO_OBJ_C := $(patsubst %.c, %.o, $(TARGET_DEMO_SRC_C))
TARGET_DEMO_DEP_C := $(patsubst %.c, %.d, $(TARGET_DEMO_SRC_C))
TARGET_DEMO_DEPS  ?= $(TARGET_DEMO_DEP_C)
TARGET_DEMO_OBJS  ?= $(TARGET_DEMO_OBJ_C)

#################################################

all: $(TARGET_LIB_NAME) $(TARGET_DEMO)

$(TARGET_DEMO): $(TARGET_DEMO_OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $^ $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

lib:
	cd cjson-lib && make -f Makefile
	cd cjson-helper && make -f Makefile

# --------
# make *.c
# --------
$(TARGET_DEMO_OBJ_C): %.o : %.c
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_I): %.i : %.c
	$(ECHO) $(MSG_CC_E) $<
	$(CC) -E $(CFLAGS) $< -o $@

$(TARGET_DEMO_DEP_C): %.d : %.c
	$(CC) -MM $(CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(TARGET_DEMO_DEPS)

#################################################

pre-compile:$(OBJ_I)

debug:

push:
	adb push ./lib /xia/base_lib/lib/
	adb push $(TARGET_DEMO) /xia/base_lib/

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(OBJ_I)
	$(RM) $(TARGET_DEMO_OBJS)
	$(RM) $(TARGET_DEMO_DEPS)
	$(RM) $(TARGET_DEMO)
	cd cjson-lib && make -f Makefile clean
	cd cjson-helper && make -f Makefile clean

distclean: clean index-clean
	$(RM) $(TARGET_DEMO)
	$(RM) $(TARGET_LIB)
	$(RM) $(TARGET_LIB_MAJ)
	$(RM) $(TARGET_LIB_SO)
	cd cjson-lib && make -f Makefile distclean
	cd cjson-helper && make -f Makefile distclean

index: index-clean
	$(ECHO) generate index
	$(Q)ctags -R
	$(Q)cscope -Rbkq

index-clean:
	$(RM) *.out
	$(RM) tags

note:
	doxygen configs/Doxyfile

.PHONY: all clean distclean debug

