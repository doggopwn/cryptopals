#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "read_stdin.h"
#include "sxor.h"
#include "get_flagon.h"

typedef struct {
	double distance;
	unsigned char xor_char;
	unsigned char* decrypted;
} XorResult;

// first = 32; last = 126
static double english_freq_prob[95] = { 17.1662, 0.0072, 0.2442, 0.0179, 0.0561, 0.0160, 0.0226, 0.2447, 0.2178, 0.2233, 0.0628, 0.0215, 0.7384, 1.3734, 1.5124, 0.1549, 0.5516, 0.4594, 0.3322, 0.1847, 0.1348, 0.1663, 0.1153, 0.1030, 0.1054, 0.1024, 0.4354, 0.1214, 0.1225, 0.0227, 0.1242, 0.1474, 0.0073, 0.3132, 0.2163, 0.3906, 0.3151, 0.2673, 0.1416, 0.1876, 0.2321, 0.3211, 0.1726, 0.0687, 0.1884, 0.3529, 0.2085, 0.1842, 0.2614, 0.0316, 0.2519, 0.4003, 0.3322, 0.0814, 0.0892, 0.2527, 0.0343, 0.0304, 0.0076, 0.0086, 0.0016, 0.0088, 0.0003, 0.1159, 0.0009, 5.1880, 1.0195, 2.1129, 2.5071, 8.5771, 1.3725, 1.5597, 2.7444, 4.9019, 0.0867, 0.6753, 3.1750, 1.6437, 4.9701, 5.7701, 1.5482, 0.0747, 4.2586, 4.3686, 6.3700, 2.0999, 0.8462, 1.3034, 0.1950, 1.1330, 0.0596, 0.0026, 0.0007, 0.0026, 0.0003 };


static bool printable(unsigned char* string, size_t string_len){
	for (int i = 0; i < string_len; i++){
		unsigned char res_char = string[i];
		if ((res_char < 0x20 || res_char > 0x7E) && res_char != '\n' && res_char != '\r' && res_char != '\t') return false;
	}
	return true;
}

static int* letter_freq(unsigned char* string, size_t string_len){
	int* freq = malloc(95*sizeof(int)); // printable ascii, offset = 0x20
	for (int i = 0; i < 95; i++){
		freq[i] = 0;
	}

	for (int i = 0; i < string_len; i++){
		if (string[i] == '\n' || string[i] == '\r' || string[i]  == '\t') continue;
		freq[(unsigned char)(char)tolower(string[i]) - 0x20]++;
	}
	return freq;
}

static unsigned char most_freq(unsigned char* string, size_t string_len){
	int* freq = letter_freq(string, string_len);

	int freq_winner = 0;
	for (int i = 0; i < 95; i++){
		if (freq[i] > freq[freq_winner]){
			freq_winner = i;
		}
	}

	free(freq);

	return (unsigned char)(freq_winner + 0x20);
}

static double english_score(unsigned char* string, size_t string_len){
	int* freq = letter_freq(string, string_len);

	double BC = 0;
	for (int i = 0; i < string_len; i++){
		unsigned char curr = (unsigned char)string[i];
		if (curr < 0x20 || curr > 0x7E) continue; 

		double P = (double)freq[curr-0x20] / (double)string_len;
		double Q = english_freq_prob[curr-0x20] / 100.0;
		BC += sqrt(P * Q);	
	}

	free(freq);

	double dist = -log(BC);
	return dist;
}

static int compareXorResults(const void* a, const void* b){
	const XorResult* res_a = (const XorResult*)a;
	const XorResult* res_b = (const XorResult*)b;
	if (res_a->distance < res_b->distance){
		return -1;
	} else {
		return 1;
	}
}


int crack_sxor(unsigned char* cipher, size_t cipher_len, unsigned char** decipher, unsigned char* key, bool silent){
	if (cipher_len == 0){
		fprintf(stderr, "crack_sxor: Received empty data.");
		return 1;
	}

	XorResult results[255] = {0};
	int results_len = 0;

	unsigned char* buffer = malloc(cipher_len+1);
	buffer[cipher_len] = '\0';
	for (int i = 0; i <= 255; i++){
		unsigned char xor_char = i;
		memcpy(buffer, cipher, cipher_len); // make a copy
		sxor(buffer, cipher_len, xor_char);

		if (!printable(buffer, cipher_len)){
			continue;
		}

		unsigned char* decrypted_copy = malloc(cipher_len+1);
		memcpy(decrypted_copy, buffer, cipher_len+1);	

		results[results_len].distance = english_score(buffer, cipher_len);
		results[results_len].xor_char = xor_char;
		results[results_len].decrypted = decrypted_copy;

		results_len++;
	}
	if (results_len == 0){
		free(buffer);
		return 1;
	}

	qsort(results, results_len, sizeof(XorResult), compareXorResults);

	for (int i = 0; i < results_len; i++){
		XorResult result = results[i];
		if (!silent){
			printf("key: %c, distance: %+f, out: %s\n", result.xor_char, result.distance, result.decrypted);
		}

		if(i==0){
			if (key != NULL) *key = result.xor_char;
			if (decipher != NULL){
				*decipher = result.decrypted;
			} else {
				free(result.decrypted);
			}
		} else {
			free(result.decrypted);
		}
	}

	free(buffer);
	return 0;
}

#ifndef COMPILE_LIB
int main(int argc, char* argv[]){
	bool silent = get_flagon(NULL, argc, argv, 's'); 

	char* input;
	size_t input_len;
	input = read_stdin(&input_len);

	unsigned char key;
	unsigned char* deciphered;

	int err = crack_sxor((unsigned char*)input, input_len, &deciphered, &key, silent);
	if (err){
		free(input);
		free(deciphered);
		return 1;
	} else {
		if (deciphered == NULL){
			if (!silent){
				printf("No matching sxor key found.\n");
			}
			return 1;
		} else {
			printf("key: %c, deciphered: %s\n", key, deciphered);
		}
	}

	free(input);
	free(deciphered);
	return 0;
}
#endif
