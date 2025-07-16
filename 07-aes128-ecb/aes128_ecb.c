#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include "read_stdin.h"
#include "get_flagon.h"
#include "pkcs7.h"
#include "aes128_cbc.h"

#define ECB 1
#include "aes.h"

int aes128_ecb(AES128* ctx, unsigned char** out, size_t* out_len){
	if (!ctx->encrypt && ctx->enc_len % 16){
		fprintf(stderr, "Input data is not a multiple of 16 lengthwise.\n");
		return 1;
	}

	unsigned char* buffer = malloc(ctx->enc_len);
	memcpy(buffer, ctx->enc, ctx->enc_len);
	
	size_t buffer_len = ctx->enc_len;
	pkcs7_pad(&buffer, &buffer_len, 16);	

	for (int i = 0; i < buffer_len; i += 16){
		if (ctx->encrypt){
			AES128_ECB_encrypt(buffer+i, ctx->key, buffer+i);
		} else {
			AES128_ECB_decrypt(buffer+i, ctx->key, buffer+i);
		}
	}

	if (out != NULL) {
		free(*out);
		*out = buffer;
	}
	if (out_len != NULL) *out_len = buffer_len;
	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	if (argc < 2){
		fprintf(stderr, "Usage: aes128_ecb [-d] <key>");
		return 1;
	}

	int argidx = 1;
	bool decrypt = get_flagon(&argidx, argc, argv, 'd');

	unsigned char* key = (unsigned char*)argv[argidx];

	size_t input_len;
	char* input = read_stdin(&input_len);

	AES128 cipher = {
		.enc = input,
		.enc_len = input_len,
		.key = key,
		.iv = NULL,
		.encrypt = !decrypt
	};
	
	size_t output_len;
	unsigned char* output;
	int err;
	err = aes128_ecb(&cipher, &output, &output_len);

	int ret;
	if (!err){
		fwrite(output, 1, output_len, stdout);
		free(output);
		ret = 0;
	} else {
		ret = 1;
	}	

	free(input);
	return ret;
}
#endif
