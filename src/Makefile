CC=gcc
CFLAGS=-Wall -g -lgcrypt `libgcrypt-config --libs` -D_GNU_SOURCE
GTKFLAG=`pkg-config --cflags --libs gtk+-2.0`

all: io_editor.c
	${CC} io_editor.c ${CFLAGS} -o io ${GTKFLAG}

clean:
	rm -rfv io

install:
	cp -Rv io /usr/bin
	mkdir -v /usr/share/io
	cp -Rv ../themes /usr/share/io
	cp -Rv ../images/io_logo.png /usr/share/io
	cp -Rv ../images/io_about_logo.png /usr/share/io
