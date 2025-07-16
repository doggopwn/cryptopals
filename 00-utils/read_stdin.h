//LIBONLY//
#include <stdbool.h>
#include <stddef.h>
char* read_stdin();

typedef struct {
        char* input;
		size_t input_len;

        char* line;
        unsigned char separator;
        int cursor;

        bool error;
		bool done;
} LineReader;

size_t nextline(LineReader* reader);
void nextline_init(LineReader* reader, char* input, size_t input_len, unsigned char separator);
void nextline_free(LineReader* reader);
