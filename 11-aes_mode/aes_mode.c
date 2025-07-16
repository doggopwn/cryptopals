#include <stdlib.h>
#include "read_stdin.h"
#include "get_flagon.h"


int aes_mode(unsigned char* enc, size_t enc_len){

}

#ifndef COMPILE_LIB
// 0 = ecb
// 1 = cbc
int main(int argc, char** argv){
	bool silent = get_flagon(NULL, argc, argv, 's');

	size_t input_len;
	char* input = read_stdin(&input_len);

	int mode = aes_mode(input, input_len);

	free(input);
	return mode;
}
#endif
