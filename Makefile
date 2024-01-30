CFLAGS=`pkg-config --cflags libcurl yajl mpv`
OFLAGS=-O2 -march=native `pkg-config --libs libcurl yajl mpv` -pthread

WFLAGS=-Wall -Wpedantic -Wextra -Wconversion -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion
DFLAGS=-g -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address -fsanitize=undefined -DJF_DEBUG

SOURCES=$(wildcard *.c)
OBJS=$(SOURCES:%.c=build/%.o)

.PHONY: all

all: clean build/jukeboxfin

install: all
	install -Dm555 build/jukeboxfin /usr/bin/jukeboxfin

clean: 
	rm -f build/*

build/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

build/jukeboxfin: $(OBJS)
	$(CC) $(OBJS) $(OFLAGS) -o $@
