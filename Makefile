build:
	gcc -c -fPIC hashmap.c -o hashmap.o
	ar rcs libhashmap.a hashmap.o


