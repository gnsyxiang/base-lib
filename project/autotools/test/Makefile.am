# ===============================================================
# 
# Release under GPLv2.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    25/07 2018 14:56
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        25/07 2018      create the file
# 
#     last modified: 25/07 2018 14:56
# ===============================================================

bin_PROGRAMS = main
main_SOURCES = hello.c

LDADD = $(top_srcdir)/src/libdemo.a
AM_CFLAGS = -I$(top_srcdir)/inc


distclean-local:
	-rm Makefile.in

