#include <stdbool.h>
int aes128_ecb(unsigned char* enc, size_t enc_len, unsigned char** out, size_t* out_len, unsigned char* key, bool encrypt);
