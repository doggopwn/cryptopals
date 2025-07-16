#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include "read_stdin.h"
#include "get_flagon.h"
#include "aes128_ecb.h"
#include "aes128_cbc.h"
#include "printbytes.h"
#include "pkcs7.h"
#include "fxor.h"


int aes128_cbc(AES128* ctx, unsigned char** out, size_t* out_len){	
	if (ctx == NULL){
		fprintf(stderr, "You need to initialize the cipher with a context.\n");
		return 1;
	}
	if (ctx->key == NULL || strlen((char*)ctx->key) != 16) {
		fprintf(stderr, "Key must be exactly 16 bytes long\n");
		return 1;
	}
	if (!ctx->encrypt && ctx->enc_len % 16){
		fprintf(stderr, "Input data is not a multiple of 16 lengthwise.\n");
		return 1;
	}
	if (ctx->encrypt && ctx->iv == NULL){
		fprintf(stderr, "IV needs to be provided for encryption.\n");
		return 1;
	}

	pkcs7_pad(&ctx->enc, &ctx->enc_len, 16);

	unsigned char* output = malloc(ctx->enc_len);
	unsigned char* prev = malloc(16); // used for xoring
	unsigned char* buffer = malloc(16); // contains input

	for (int i = 0; i < ctx->enc_len; i+=16){
		memcpy(buffer, ctx->enc+i, 16);
		if (i == 0){
			memcpy(prev, ctx->iv, 16);
		} else {
			memcpy(prev, ctx->encrypt ? output+i-16 : ctx->enc+i-16, 16);
		}

		if (ctx->encrypt){
			fxor(buffer, 16, prev, 16);
		}

		ctx->enc = buffer;
		aes128_ecb(ctx, &buffer, NULL);

		if (!ctx->encrypt){
			fxor(buffer, 16, prev, 16);
		}
		memcpy(output+i, buffer, 16);
	}
/*
	for (int i = 0; i < ctx->enc_len; i+=16){
		if (i == 0){
			memcpy(ciphertext, ctx->iv, 16);
		} else {
			//previous ciphertext
			memcpy(ciphertext, enc+i-16, 16);
		}

		aes128_ecb(ctx->enc+i, 16, &(ctx->enc+i), NULL, ctx->key, ctx->encrypt);
		fxor(ctx->enc+i, 16, ciphertext, 16);
		memcpy(output+i, enc+i, 16);
	}
	*/

	*out = output;
	*out_len = ctx->enc_len;

	free(prev);
	free(buffer);
	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	if (argc < 2){
		fprintf(stderr, "Usage: aes128_ecb [-d] <key>\nIf encrypting, first 16 bytes of stdin are used as the IV.\n");
		return 1;
	}

	int argidx = 1;
	bool decrypt = get_flagon(&argidx, argc, argv, 'd');

	unsigned char* key = (unsigned char*)argv[argidx];

	size_t input_len;
	char* input;
	input = read_stdin(&input_len);
	if (input_len < 16){
		fprintf(stderr, "Input needs to be longer than 16 bytes.\n");
		return 1;
	}

	unsigned char* iv = malloc(16);
	memcpy(iv, input, 16);

	size_t enc_len = input_len-16;
	unsigned char* enc = malloc(enc_len);
	memcpy(enc, input+16, enc_len);

	free(input);

	AES128 cipher = {
		.enc = enc,
		.enc_len = enc_len, 
		.iv = iv,
		.key = key,
		.encrypt = !decrypt
	};

	size_t output_len;
	unsigned char* output;

	int err;
	err = aes128_cbc(&cipher, &output, &output_len);

	int ret;
	if (!err){
		printbytes(output, output_len, PRINTBYTES_STRING);
		free(output);
		ret = 0;
	} else {
		ret = 1;
	}	

	free(cipher.enc);
	free(cipher.iv);
	return ret;
}
#endif
