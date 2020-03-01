check: test.o string_sw.o
	gcc -o test test.o string_sw.o

check.o: test.c
	gcc -c test.c

string_sw.o: string_sw.c
	gcc -c string_sw.h string_sw.c

clean:
	rm -f test
