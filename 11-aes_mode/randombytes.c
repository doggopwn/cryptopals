#include <stdlib.h>
#include <time.h>
#include "printbytes.h"
unsigned char* randombytes(int count){
	srand(time(NULL));
	unsigned char* buffer = malloc(count);
	for (int i = 0; i < count; i++){
		buffer[i] = (unsigned char)(rand() % 256);
	}
	return buffer;
}

#ifndef COMPILE_LIB
int main(int argc, char** argv){
	if (argc != 2) return 1;
	int count = atoi(argv[1]);

	unsigned char* bytes = randombytes(count);

	printbytes(bytes, count, PRINTBYTES_STRING);

	free(bytes);
	return 0;
}
#endif
