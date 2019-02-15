
all:
	echo "make play or make record"


record:
	gcc lrecord.c sndwav_common.c  wav_parser.c -lasound -L../work-pro/alsa/docs/install/lib/ -I../work-pro/alsa/docs/install/include/

play:
	gcc lplay.c sndwav_common.c  wav_parser.c -lasound -L../work-pro/alsa/docs/install/lib/ -I../work-pro/alsa/docs/install/include/

