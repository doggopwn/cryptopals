#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_stdin.h"


// HexTOAscii
int htoa(const char* hex, size_t hex_len, char** out, size_t* out_len){
	if (!hex_len){
		fprintf(stderr, "htoa: Received empty string\n");
		return 1;
	}
	if (hex_len % 2){
		fprintf(stderr, "htoa: Hex string has an odd length\n");
		return 1;
	}

	char* ascii = malloc(hex_len/2);
	char* asciiw;
	asciiw = ascii;

	char byte[3];
	for (int i = 0; i < hex_len; i+=2){
		byte[0] = hex[i];
		byte[1] = hex[i+1];
		byte[2] = '\0';
		long int val = strtol(byte, NULL, 16);
		if (val == 0L && strncmp(byte, "00", 2) != 0){
			fprintf(stderr, "htoa: Invalid hex string\n");
			free(ascii);
			return 1;
		}

		*asciiw++ = (signed char)(unsigned char)val;
	}

	*out = ascii;
	*out_len = hex_len/2;

	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	char* input;
	size_t input_len;
	input = read_stdin(&input_len);

	char* ascii;
	size_t ascii_len;

	LineReader reader;
	nextline_init(&reader, input, input_len, '\n');
	while (!reader.done){
		int len = nextline(&reader);
		if (len == 0 || reader.error){
			fprintf(stderr, "Failed to read input\n");
			return 1;
		}

		int err = htoa(reader.line, len, &ascii, &ascii_len);
		if (!err){
			fwrite(ascii, 1, ascii_len, stdout);
			free(ascii);
		} else {
			free(ascii);
			free(input);
			nextline_free(&reader);
			return 1;
		}
	}

	nextline_free(&reader);
	free(input);
}
#endif
