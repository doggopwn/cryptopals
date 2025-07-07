#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "read_stdin.h"
#include "get_flagon.h"
#include "sxor_crack.h"
#include "rxor.h"

int bitcount(unsigned char byte){
	int count = 0;

	size_t bitsize = 8*sizeof(byte);

	for (int i = 0; i < bitsize; i++){
		if(byte & (1 << i)){
			count++;
		}
	}

	return count;
}

int hamming_dist(unsigned char* a, size_t a_len, unsigned char* b, size_t b_len){
	int dist = 0;

	if (a_len != b_len){
		return -1;
	}

	for (int i = 0; i < a_len; i++){
		dist += bitcount(a[i] ^ b[i]);
	}
	return dist;
}

typedef struct {
	int keysize;
	double distance;
} Keysize;

int compare_keysize(const void* a, const void* b){
	const Keysize* ka = (const Keysize*)a;
	const Keysize* kb = (const Keysize*)b;

	if (ka->distance < kb->distance){
		return -1;
	}
   	if (ka->distance > kb->distance){
		return 1;
	}
	return 0;
}

int find_keysize(unsigned char* ciphered, size_t ciphered_len){
	int keysize = 2;
	int max_keysize = 40;
	
	int candidates_len = max_keysize-keysize+1;
	Keysize* candidates = malloc(candidates_len*sizeof(Keysize));
	for (int i = keysize; i <= max_keysize; i++){
		Keysize candidate;

		int total_distance = 0;
		int distance_count = 0;	

		unsigned char* block1 = malloc(i);
		unsigned char* block2 = malloc(i);
		for (int j = 0; (j*i + 2*i) < ciphered_len; j++){
			memcpy(block1, ciphered+j*i, i);
			memcpy(block2, ciphered+j*i + i, i);
			total_distance += hamming_dist(block1, i, block2, i);
			distance_count++;
		}
		free(block1);
		free(block2);

		double avg_distance = (double)total_distance / (double)distance_count;
		double normalized_dist = avg_distance / i;

		candidate.distance = normalized_dist;	
		candidate.keysize = i;

		candidates[i-keysize] = candidate;
	}

	qsort(candidates, candidates_len, sizeof(Keysize), compare_keysize);

	int winner_keysize = candidates[0].keysize;
	free(candidates);
	return winner_keysize;
}

void make_sxor_blocks(unsigned char* input, size_t input_len, unsigned char* out, int out_lens[], int keysize){

	for (int i = 0; i < keysize; i++){
		if (i > 0){
			out_lens[i] = out_lens[i-1];
		} else {
			out_lens[i] = 0;
		}

		for (int j = 0; (j+i) < input_len; j+=keysize){
			out[out_lens[i]] = input[j+i];
			out_lens[i]++;
		}
	}

	return;
}

// int crack_sxor(unsigned char* cipher, size_t cipher_len, unsigned char** decipher, unsigned char* key, bool silent)
// int rxor(unsigned char* data, size_t data_len, unsigned char* key, size_t key_len);
int crack_rxor(unsigned char* input, size_t input_len, unsigned char** key, bool silent){
	int keysize = find_keysize(input, input_len);
	if (!silent) printf("Determined keysize >>%i<<\n", keysize);

	unsigned char* blocks = malloc(input_len);
	int* lens = malloc(keysize*sizeof(int));
	unsigned char* decrypt_key = malloc(keysize+1);

	make_sxor_blocks(input, input_len, blocks, lens, keysize);

	for (int i = 0; i < keysize; i++){
		unsigned char decrypt_key_char;
		int offset = i > 0 ? lens[i-1] : 0;
	   	if (crack_sxor(blocks+offset, lens[i]-offset, NULL, &decrypt_key_char, true)){
			fprintf(stderr, "Failed to crack sxor step.\n");
			free(lens);
			free(blocks);
			return 1;
		}

		decrypt_key[i] = decrypt_key_char;
	}
	decrypt_key[keysize] = '\0';

	if (!silent) printf("Cracked cipher key: >>%s<<\n", decrypt_key);
	if (key != NULL) *key = decrypt_key;

	rxor(input, input_len, decrypt_key, keysize);

	free(lens);
	free(blocks);
	return 0;
}


#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
        bool silent = get_flagon(NULL, argc, argv, 's');

        char* input;
        size_t input_len;
        input = read_stdin(&input_len);

        unsigned char* key;

		int err = crack_rxor((unsigned char*)input, input_len, &key, silent); // input is deciphered in-place
		if (err){
			free(input);
			free(key);
			return 1;
		} else {
			if (!silent) printf("==Decrypted data =======\n");
			fwrite(input, 1, input_len, stdout);
			if (!silent) printf("==END=================\n");
			fflush(stdout);
		}

		free(input);
		free(key);
		return 0;
}
#endif
