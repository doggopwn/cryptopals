#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "read_stdin.h"

char* read_stdin(size_t* read_len_ptr){
	size_t buf_size = 128;
	char* input = malloc(buf_size);

	int read_len = 0;
	int c;
	while ( (c = getchar()) != EOF ){
		input[read_len++] = c;
		if (read_len == buf_size){
			input = realloc(input, buf_size+=128);
			if (input == NULL){
				fprintf(stderr, "Failed to allocate memory for input buffer\n");
				free(input);
				return NULL;
			}
		}
	}

	if (input[read_len-1] == '\n'){
		input = realloc(input, read_len-1);
		if (input == NULL){
			fprintf(stderr, "Failed to reallocate memory for input buffer\n");
			free(input);
			return "";
		}
		read_len--;
	}

	if (read_len_ptr != NULL){
		*read_len_ptr = (size_t)read_len;
	}

	return realloc(input, read_len);
}

void nextline_init(LineReader* reader,char* input, size_t input_len, unsigned char separator){
	reader->input = input;
	reader->input_len = input_len;
	reader->line = NULL;
	reader->separator = separator;
	reader->cursor = 0;
	reader->error = false;
	reader->done = false;
};	

void nextline_free(LineReader* reader){
	free(reader->line);
	reader->line = NULL;
}

size_t nextline(LineReader* reader){
	size_t buf_size = 128;
	int read_len = 0;
	char* line = malloc(buf_size);

	char c;
	while ( (c = reader->input[reader->cursor++]) != reader->separator){
		line[read_len++] = c;
		if (read_len == buf_size){
			line = realloc(line, buf_size+=128);
			if (line == NULL){
				fprintf(stderr, "Failed to allocate memory for input buffer\n");
				reader->error = true;
				free(line);
				return 0;
			}
		}
		if (reader->input_len == reader->cursor){
			reader->done = true;
			break;
		}
	}

	free(reader->line);
	line = realloc(line, read_len);
	reader->line = line;
	return (size_t)read_len;
}
