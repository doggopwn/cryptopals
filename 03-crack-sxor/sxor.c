#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "read_stdin.h"
#include "read_file.h"
#include "get_flagon.h"
#include "htoa.h"

int sxor(unsigned char* data, size_t data_len, unsigned char xor_char){
	if (!data_len){
		fprintf(stderr, "Data provided is empty");
		return 1;
	}

	
	for (int i = 0; i < data_len; i++){
		data[i] ^= xor_char;
	}

	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	int argidx = 1;
	bool unhex = get_flagon(&argidx, argc, argv, 'h');

	if (argc < 2 || argidx == argc){
		fprintf(stderr, "Usage: sxor [-h] <xor-char>");
		return 1;
	}

	unsigned char xor_char = argv[argidx++][0];

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
	}

	int err = sxor((unsigned char*)input, input_len, xor_char);

	int ret;
	if (!err){
		fwrite(input, 1, input_len, stdout);
		ret = 0;
	} else {
		ret = 1;
	}	

	if (argc-unhex == 2 || unhex) {
		free(input);
	}

	return ret;
}
#endif
