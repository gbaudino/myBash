#!/bin/bash

echo "Testeando el funcionamiento de los modulos"
echo "Se muestra el output en myTests/test.log"
echo "Se muestra la diferencia entre el output esperado y el obtenido en myTests/diff.log"
echo "Se muestra el sumario de memory leaks en myTests/leaks.log"
cp ../objects-x86_64/lexer.o ../lexer.o
cp ../objects-x86_64/parser.o ../parser.o
cp ../objects-x86_64/parsing.o ../parsing.o
# Hacemos uso del object parsing de los profes
gcc -std=gnu11 -Wall -Wextra -Wbad-function-cast -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -Werror=vla -g -pedantic test_original_parse.c ../lexer.o ../parser.o ../parsing.o ../command.c ../strextra.c -o runtest_original_parse `pkg-config --cflags --libs glib-2.0` -lcheck
./runtest_original_parse
# Compilamos nuestro parsing.c
gcc -std=gnu11 -Wall -Wextra -Wbad-function-cast -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -Werror=vla -g -pedantic ../execute.c ../lexer.o ../parser.o ../parsing.c ../command.c ../builtin.c ../strextra.c test.c -o runtest_parse `pkg-config --cflags --libs glib-2.0` -lcheck
./runtest_parse > test.log 2>&1
diff expected.txt output.txt > diff.log
valgrind --leak-check=full --show-reachable=yes -v ./runtest_parse > leaks.log 2>&1
sleep 2
rm ../lexer.o ../parser.o ../parsing.o runtest* output.txt expected.txt