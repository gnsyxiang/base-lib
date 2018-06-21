#!/bin/bash


awk 'BEGIN{i=0;} \
	/time:/ \
	{printf "%03d.wav %s %s \n", i++, $2, $3}' \
	haha.txt \
	> xixi.txt

cat xixi.txt | awk '{system("./main -f "$1" -s "$2" -p "$3"")}'



