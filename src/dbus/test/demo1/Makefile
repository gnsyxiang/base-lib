


CFLAGS = -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include



all:
	gcc client.c -ldbus-1  $(CFLAGS) -o client
	gcc service.c -ldbus-1 $(CFLAGS) -o service
