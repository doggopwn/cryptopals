#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

char* read_file(const char* filename, size_t* read_size_ptr){
	FILE* file;

	file = fopen(filename, "rb");

	if (file == NULL){
		return "";
	}

	int fseek_err = fseek(file, 0, SEEK_END);
	if (fseek_err){
		fprintf(stderr, "Failed to seek to end of file");
		return "";
	}

	size_t file_size = ftell(file);
	if (file_size == -1L){
		fprintf(stderr, "Failed to receive file position indicator");
		return "";
	}
	rewind(file);

	char* buffer = malloc(file_size);
	if (buffer == NULL){
		fprintf(stderr, "Failed to allocate memory for file buffer");
		return "";
	}

	size_t read_len = fread(buffer, 1, file_size, file);
	if (read_len != file_size){
		fprintf(stderr, "Failed to read file content");
		return "";
	}
	
	fclose(file);
	
	if (buffer[read_len-1] == 0x0a){
		buffer = realloc(buffer, read_len-1);
		if (buffer == NULL){
			fprintf(stderr, "Failed to reallocate memory for file buffer");
			return "";
		}
		read_len--;
	}

	if (read_size_ptr != NULL){
		*read_size_ptr = read_len;
	}

	return buffer;
}
