#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "read_stdin.h"

static char* b64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void b64_buf_len(size_t a_len, int* unpadded_len, int* padded_len, int* padding_len){
	*unpadded_len = (4*a_len + 2)/3;
	*padded_len = (*unpadded_len + 3) & ~(3);
	*padding_len = *padded_len - *unpadded_len;
}

// AsciiTOBase64
int atob64(const char* signed_ascii, int ascii_len){
	const unsigned char* ascii = (const unsigned char*)signed_ascii;
	if (!ascii_len){
		fprintf(stderr, "atob64: Received empty string\n");
		return 1;
	}

	int b64_unpadded_len;
	int b64_padded_len;
	int b64_pad_len;
	b64_buf_len(ascii_len, &b64_unpadded_len, &b64_padded_len, &b64_pad_len);


	char* b64 = malloc(b64_padded_len+1);
	char* b64w = b64;
	for (int i = 0; i < ascii_len;){
		int octet1 = ascii[i++] << 16;
		int octet2 = (i < ascii_len ? ascii[i++] : 0) << 8;
		int octet3 = (i < ascii_len ? ascii[i++] : 0);

		int triplet = octet1 | octet2 | octet3;

		int loop_cnt = i%3 == 0 ? 0 : 3-(i%3);
		for (int j = 3; j >= loop_cnt; j--){
			int8_t indice = (triplet >> 6*j) & 0x3F;
			*b64w++ = b64_alphabet[indice];
		}
	}

	for (int i = 0; i < b64_pad_len;i++){
		*b64w++ = '=';
	}
	*b64w = '\0';

	printf("%s", b64);
	fflush(stdout);

	free(b64);

	return 0;
}

int main(int argc, char* argv[]){
	if (argc == 2){
		atob64(argv[1], strlen(argv[1]));
	} else {
		size_t len;
		char* input = read_stdin(&len);
		atob64(input, len);
	}
}
