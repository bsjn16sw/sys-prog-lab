check: collect.o string_sw.o
		gcc -o collect collect.o string_sw.o

check.o: collect.c
		gcc -c collect.c

string_sw.o: string_sw.c
		gcc -c string_sw.h string_sw.c

clean:
		rm -f collect
