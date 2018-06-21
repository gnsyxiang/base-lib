#!/bin/bash

# ===============================================================
# 
# Release under GPLv2.
# 
# @file    handle-vad-event.sh
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/06 2018 14:49
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/06 2018      create the file
# 
#     last modified: 21/06 2018 14:49
# ===============================================================

./main -s ./vad-test.wav | tee time_stamp.txt
