#include <stdbool.h>

#ifndef AES_H
#define AES_H
#include "aes_def.h"
#endif

int aes128_cbc(AES128* ctx, unsigned char** out, size_t* out_len);
