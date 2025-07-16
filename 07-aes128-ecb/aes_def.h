typedef struct {
        unsigned char* enc;
        size_t enc_len;
        unsigned char* iv; // if not set, assume ECB
        unsigned char* key;
        bool encrypt;
} AES128;
