#include <stdio.h>
#include <stdlib.h>
#include "pkcs7.h"
#include "printbytes.h"

static unsigned char str[16] = {'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E' }	;
int main(void){
	unsigned char* buf = malloc(16);
	for (unsigned char i = 0; i < 16; i++){
		buf[i] = str[i];
	}
	size_t buf_len = 16;
	printf("old length: %lu\n", buf_len);

	printbytes(buf, buf_len, PRINTBYTES_VVERBOSE);
	
	pkcs7_pad(&buf, &buf_len, 20);
	
	printf("\nnew length: %lu\n", buf_len);
	printbytes(buf, buf_len, PRINTBYTES_VVERBOSE);

	free(buf);
}
