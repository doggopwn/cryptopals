#include <stdbool.h>
typedef struct {
	unsigned char* enc;
	size_t enc_len;
	unsigned char* iv; // if not set, assume ECB
	unsigned char* key;
	bool encrypt;
} AES128;

int aes128_cbc(AES128* ctx, unsigned char** out, size_t* out_len);
