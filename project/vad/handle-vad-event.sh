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

wav_path=wav
src_path=${wav_path}/src
dst_path=${wav_path}/dst

./main -s ${src_path}/vad-test.wav | 
tee ${wav_path}/time_stamp.txt
