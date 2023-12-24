CFLAGS=-g
LFLAGS=-g
SOURCES=main.c jukeboxfin.c jf_config.c jf_api.c
OBJECTS=build/main.o build/jukeboxfin.o build/jf_config.o  build/jf_api.o
BUILD_DIR := build

.PHONY: all

all: ${BUILD_DIR}/jukeboxfin

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}/jukeboxfin: ${OBJECTS}
	$(CC) $(LFLAGS) -o $@ $^

build/main.o: main.c jukeboxfin.h
	$(CC) $(CFLAGS) -c -o $@ main.c

build/jukeboxfin.o: jukeboxfin.c jukeboxfin.h
	$(CC) $(CFLAGS) -c -o $@ jukeboxfin.c

build/jf_config.o: jf_config.c jukeboxfin.h
	$(CC) $(CFLAGS) -c -o $@ jf_config.c

build/jf_api.o: jf_api.c jukeboxfin.h
	$(CC) $(CFLAGS) -c -o $@ jf_api.c
