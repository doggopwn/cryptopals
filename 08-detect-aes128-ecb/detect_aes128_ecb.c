#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "read_stdin.h"
#include "get_flagon.h"

// 1 = true
// 0 = false
// -1 = error
int detect_aes128_ecb(unsigned char* enc, size_t enc_len){
	if (enc_len % 16){
		fprintf(stderr, "Input must be in 16-byte sized blocks.\n");
		return -1;
	}

	int block_count = enc_len / 16;
	if (block_count < 2){
		fprintf(stderr, "Input must contain at least two blocks to analyze.\n");
		return -1;
	}

	for (int i = 0; i < block_count; i++){
		for (int j = i+1; j < block_count; j++){
			if (memcmp(enc+16*i, enc+16*j, 16) == 0) return 1;
		}
	}
	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	bool silent = get_flagon(NULL, argc, argv, 's');

	size_t input_len;
	char* input;
	input = read_stdin(&input_len);

	int is_aes = detect_aes128_ecb((unsigned char*)input, input_len);
	if (is_aes == -1){
		free(input);
		return 1;
	}

	if (silent){
		free(input);
		return 1 - is_aes;
	} else {
		fwrite(input, 1, input_len, stdout);
		printf(": is_aes128_ecb: %i\n", is_aes);
		fflush(stdout);
		free(input);
		return 0;
	}
}
#endif
