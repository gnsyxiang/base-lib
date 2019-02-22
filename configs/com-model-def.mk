# ===============================================================
# 
# Release under GPLv2.
# 
# @file    com-model-def.mk
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    22/02 2019 15:31
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        22/02 2019      create the file
# 
#     last modified: 22/02 2019 15:31
# ===============================================================

TARGET_LIB 	  	:= lib$(TARGET_LIB_NAME).so.$(MAJOR_V).$(MINOR_V).$(RELEASE_V)
TARGET_LIB_MAJ 	:= lib$(TARGET_LIB_NAME).so.$(MAJOR_V)
TARGET_LIB_SO 	:= lib$(TARGET_LIB_NAME).so

TARGET_PATH := $(LIB_DIR)/$(TARGET_LIB)

# ------
# cflags
# ------
CFLAGS     += -I$(INC_DIR) \
			  -I$(SRC_DIR) \
              -I$(TO_TOP_DIR)/$(INC_DIR)
CFLAGS 	   += -Wno-error=unused-function \
			  -Wno-error=unused-variable \
			  -Wno-error=pointer-to-int-cast \
			  -Wno-error=int-to-pointer-cast

LIB_CFLAGS += $(CFLAGS) -fPIC

# -------
# ldflags
# -------
LD_COM_FLAG := $(SYSTEM_32_64)

LDFLAGS 	+= -T $(TO_TOP_DIR)/configs/ldscript.lds
LDFLAGS 	+= $(LD_COM_FLAG)
LDFLAGS 	+= -Wl,-rpath=$(LIB_DIR):$(TO_TOP_DIR)/lib
LDFLAGS 	+= -L$(LIB_DIR) \
			   -L$(TO_TOP_DIR)/lib
LDFLAGS 	+= -l$(TARGET_LIB_NAME) \
			   -lpthread

LIB_LDFLAGS := -Wl,-soname=$(TARGET_LIB_MAJ)
LIB_LDFLAGS += -shared $(LD_COM_FLAG)

# -------
# h files
# -------
INC_H := $(wildcard $(INC_DIR)/*.h)

# -------
# c files
# -------
SRC_C := $(wildcard $(SRC_DIR)/*.c)
OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_C))
DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(SRC_C))
DEPS  ?= $(DEP_C)
OBJS  ?= $(OBJ_C)

DEMO_SRC_C := $(wildcard $(TST_DIR)/*.c)
DEMO_OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(DEMO_SRC_C))
DEMO_DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(DEMO_SRC_C))
DEMO_DEPS  ?= $(DEMO_DEP_C)
DEMO_OBJS  ?= $(DEMO_OBJ_C)

