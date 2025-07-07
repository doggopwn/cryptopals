#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "read_stdin.h"
#include "read_file.h"
#include "get_flagon.h"
#include "htoa.h"

int fxor(unsigned char* data, size_t data_len, unsigned char* xor_data, size_t xor_data_len){
	if (!xor_data_len){
		fprintf(stderr, "Xor file provided is empty");
		return 1;
	}
	if (!data_len){
		fprintf(stderr, "Data provided is empty");
		return 1;
	}
	if (xor_data_len != data_len){
		fprintf(stderr, "Xor file data and message data must be the same length.");
		return 1;
	}

	for (int i = 0; i < data_len; i++){
		data[i] ^= xor_data[i];
	}

	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	int argidx = 1;
	bool unhex = get_flagon(&argidx, argc, argv, 'h');

	if (argc < 2 || argidx == argc){
		fprintf(stderr, "Usage: fxor [-h] <xor-file>");
		return 1;
	}

	size_t xor_file_len;
	char* xor_file = read_file(argv[argidx++], &xor_file_len);

	size_t input_len;
	char* input;
	if (argc-unhex == 2){
		input = read_stdin(&input_len);
	} else {
		input = argv[argidx];
		input_len = strlen(input);
	}

	if (unhex){
		if (htoa(input, input_len, &input, &input_len)){
			return 1;
		}
		if (htoa(xor_file, xor_file_len, &xor_file, &xor_file_len)){
			return 1;
		}
	}

	int err = fxor((unsigned char*)input, input_len, (unsigned char*)xor_file, xor_file_len);

	int ret;
	if (!err){
		fwrite(input, 1, input_len, stdout);
		ret = 0;
	} else {
		ret = 1;
	}	

	free(xor_file);
	if (argc-unhex == 2 || unhex) {
		free(input);
	}

	return ret;
}
#endif
