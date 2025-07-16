//LIBONLY//
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>

#define PRINTBYTES_STRING 0 // simply dump to stdout
#define PRINTBYTES_VERBOSE 1 // enumerate byte-by-byte on a single line
#define PRINTBYTES_VVERBOSE 2 // enumerate byte-by-byte including ascii hex numbers on line-per-byte

void printbytes(unsigned char* buf, size_t buf_len, int mode);
