CC=gcc
CFLAGS=-Wall -g
EXECUTABLE=add_blank_to_wave


OBJS=test_wav.o wav.o wav_helper.o dir_helper.o


default:$(EXECUTABLE)


$(EXECUTABLE):$(OBJS)
	$(CC) -o $(EXECUTABLE) $(CFLAGS) $(OBJS)


%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)
