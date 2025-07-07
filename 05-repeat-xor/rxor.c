#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "read_stdin.h"
#include "read_file.h"
#include "get_flagon.h"
#include "htoa.h"

int rxor(unsigned char* data, size_t data_len, unsigned char* key, size_t key_len){
	if (!data_len){
		fprintf(stderr, "Data provided is empty");
		return 1;
	}
	
	for (int i = 0; i < data_len; i++){
		data[i] ^= key[i % key_len];
	}

	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	int argidx = 1;
	bool unhex = get_flagon(&argidx, argc, argv, 'h');

	if (argc < 2 || argidx == argc){
		fprintf(stderr, "Usage: rxor [-h] <xor-string>");
		return 1;
	}

	unsigned char* xor_string = (unsigned char*)argv[argidx++];

	size_t input_len;
	char* input;
	input = read_stdin(&input_len);

	if (unhex){
		if (htoa(input, input_len, &input, &input_len)){
			return 1;
		}
	}

	int err = rxor((unsigned char*)input, input_len, xor_string, strlen((char*)xor_string));

	int ret;
	if (!err){
		fwrite(input, 1, input_len, stdout);
		ret = 0;
	} else {
		ret = 1;
	}	

	free(input);
	return ret;
}
#endif
