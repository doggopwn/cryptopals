#include "printbytes.h"
#include <stdbool.h>
static bool printable(unsigned char* str, size_t str_len){
	for (int i = 0; i < str_len; i++){
		if (!isprint(str[i])){
			return false;
		}
	}
	return true;
}

void printbytes(unsigned char* buf, size_t buf_len, int mode){
	switch (mode){
		case PRINTBYTES_STRING:
			fwrite(buf, sizeof(buf[0]), buf_len, stdout);
			printf("\n");
			break;
		case PRINTBYTES_VERBOSE:
			bool hex = !printable(buf, buf_len);
			for (int i = 0; i < buf_len; i++){
				if (hex){
					printf("0x%02x, ", buf[i]);
				} else {
					printf("%c, ", buf[i]);
				}
			}
			printf("\n");
			break;
		case PRINTBYTES_VVERBOSE:
			for (int i = 0; i < buf_len; i++){
				unsigned char printChar = isprint(buf[i]) ? buf[i] : ' ';
				printf("%c (0x%02x)\n", printChar, buf[i]);
			}
			break;
	}
}
