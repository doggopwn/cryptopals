#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_stdin.h"


// AsciiTOHex
int atoh(const char* ascii, size_t ascii_len, char** out, size_t* out_len){
	if (!ascii_len){
		fprintf(stderr, "atoh: Received empty string\n");
		return 1;
	}

	char* hex = malloc(ascii_len*2);
	char* hexw = hex;
	char byte[3];
	for (int i = 0; i < ascii_len; i++){
		snprintf(byte, 3, "%02x", (unsigned char)ascii[i]);
		*hexw++ = byte[0];
		*hexw++ = byte[1];
	}

	*out = hex;
	*out_len = ascii_len*2;

	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	char* input;
	size_t input_len;
	if (argc == 2){
		input = argv[1];
		input_len = strlen(input);
	} else {
		input = read_stdin(&input_len);
	}

	char* hex;
	size_t hex_len;
	int err = atoh(input, input_len, &hex, &hex_len);

	if (!err){
		fwrite(hex, 1, hex_len, stdout);
		free(hex);
		return 0;
	} else {
		free(hex);
		return 1;
	}
}
#endif
