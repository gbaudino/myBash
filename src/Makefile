TARGET=mybash
CC=gcc
CPPFLAGS=`pkg-config --cflags glib-2.0`
CFLAGS=-std=gnu11 -Wall -Wextra -Wbad-function-cast -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -Werror=vla -g -pedantic
LDFLAGS=`pkg-config --libs glib-2.0`

# Propagar entorno a make en tests/
export CC CPPFLAGS CFLAGS LDFLAGS

SOURCES=$(shell echo *.c)
OBJECTS=$(SOURCES:.c=.o)
PRECOMPILED=parser.o lexer.o

# Agregar objects-arch a los directorios de busqueda de los .o precompilados
ARCHDIR=objects-$(shell uname -m)
vpath parser.o $(ARCHDIR)
vpath lexer.o $(ARCHDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(PRECOMPILED)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~ lexer.o parser.o
	make -C tests clean
	make -C myTests clean

test: $(OBJECTS)
	cp objects-x86_64/parser.o .
	cp objects-x86_64/lexer.o .
	make -C tests test

test-command: command.o
	make -C tests test-command

test-parsing: command.o parsing.o parser.o
	cp objects-x86_64/parser.o .
	cp objects-x86_64/lexer.o .
	make -C tests test-parsing

memtest: $(OBJECTS)
	make -C tests memtest

mem:
	make all
	valgrind --leak-check=full --show-reachable=yes ./$(TARGET)

.depend: $(SOURCES)
	$(CC) $(CPPFLAGS) -MM $^ > $@

-include .depend

.PHONY: clean all
