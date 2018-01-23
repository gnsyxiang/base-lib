# ===============================================================
# Filename: Makefile
# -------------------------------
# Author: zhenquan.qiu
# Version: V1.0.0
# Last modified: 23/01 2018 16:35
# Description:
# 
# Change Log:
# NO.	Author		    Date		Modified
# 00	zhenquan.qiu	23/01 2018
# ===============================================================

ROOT := $(shell pwd)
TARGET := one-two-pointer

RM := $(Q)rm -rf

# cflags
CFLAGS := -Wall -g
CFLAGS += -c
CFLAGS += -I.

# ldflags
LDFLAGS :=

# c files
C_SRC := $(wildcard *.c)
O_SRC := $(patsubst %.c, %.o, $(C_SRC))

all:
	gcc $(CFLAGS) $(C_SRC)
	gcc $(LDFLAGS) $(O_SRC) -o $(TARGET)

clean:
	$(RM) $(O_SRC)
	$(RM) $(TARGET)

debug:
	echo $(C_SRC)
	echo $(O_SRC)

