#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

bool get_flagon(int* start_index, int argc, char* argv[], char flag){
	char flagstring[2] = { flag, '\0' };

	bool result = false;
	int c;
	while ((c = getopt(argc, argv, flagstring)) != -1){
		if (c == flag){
			result = true;
			break;
		}
	}

	if (result && start_index != NULL){
		*start_index = optind;
	}

	return result;
}


