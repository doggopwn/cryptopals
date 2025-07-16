#include "randombytes.h"
#include <time.h>
#include <stdlib.h>
#include <aes128_ecb.h>
#include <aes128_cbc.h>
unsigned char* aes128_oracle(unsigned char* input){
	unsigned char* key = randombytes(16);
	
}
