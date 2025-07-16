#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pkcs7_pad(unsigned char** payload, size_t* payload_len, int block_len){
	int block_count = (int)ceil(*payload_len/(double)block_len);
	int buffer_len = block_count*block_len; // length of full padded buffer
	
	if (*payload_len == buffer_len) return;

	unsigned int padding_len = buffer_len - *payload_len;
	unsigned char padding_char = (unsigned char)padding_len;

	unsigned char* buffer = malloc(buffer_len);
	memcpy(buffer, *payload, *payload_len);

	for (int i = *payload_len; i < buffer_len; i++){
		buffer[i] = padding_char;
	}	

	free(*payload);
	*payload = buffer;
	*payload_len = buffer_len;
}
