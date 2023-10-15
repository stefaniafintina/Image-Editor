# Copyright 2020 Darius Neatu <neatudarius@gmail.com>

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c -o image_editor -lm -g

pack:
	zip -FSr 3XYCA_FirstnameLastname_Tema1.zip README Makefile *.c 

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
