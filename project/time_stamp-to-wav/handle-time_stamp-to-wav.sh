#!/bin/bash

# ===============================================================
# 
# Release under GPLv2.
# 
# @file    handle-time_stamp-to-wav.sh
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/06 2018 14:25
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/06 2018      create the file
# 
#     last modified: 21/06 2018 14:25
# ===============================================================

wav_path=wav
src_path=${wav_path}/src
dst_path=${wav_path}/dst

src_file_name=${src_path}/vad-test.wav

if [ $# -ne 1 ]; then
	echo "eg: $0 time_stamp.txt"
	exit 1
elif [ $1 == "-h" ]; then
	echo "eg: $0 time_stamp.txt"
	exit 1
fi

if [ ! -d ${dst_path} ]; then
	mkdir -p ${dst_path} 
fi

awk 'BEGIN{i=0;} /time:/ {printf "%s/%03d.wav %s %s \n", "'${dst_path}'", i++, $2, $3}'  $1 | \
awk '{system("./main -t '${src_file_name}' -f "$1" -s "$2" -p "$3"")}'



