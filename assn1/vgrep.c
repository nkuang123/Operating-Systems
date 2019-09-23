/* C implementation of the Linux utility grep */

#include <stdio.h>
#include <stdlib.h>		
#include <string.h>		
#include <stdbool.h>

// Function declarations
void print_lines_that_match(FILE* stream, char *search_term);						 

/* 
	Example of how vgrep will be called:
	$ ./vgrep foo bar.txt 
*/

int main(int argc, char *argv[]) {

	if (argc < 2) {
		// No arguments were passed.
		printf("vgrep: searchterm [file ...]\n");
		exit(1);
	}

	bool search_term_supplied = true;

	// Check if search-term was supplied by attempting to open the first 
	// argument.
	char *first_arg = argv[1];
	FILE *stream = fopen(first_arg, "r");

	if (stream) {
		// First argument is not a search term, file successfully opened.
		search_term_supplied = false;
	}

	fclose(stream);

	// if (search_term_supplied) {
	// 	printf("search term detected\n");
	// } else {
	// 	// Match none
	// 	exit(0);
	// }

	// Extract the search term.
	char *search_term = argv[1];

	// Only search term supplied, read from stdin
	if (argc == 2) {
		print_lines_that_match(stdin, search_term);
	}

	for (int i = 2; i < argc; i++) {
		// Extract 
		char *file_name = argv[i];

		FILE *stream = fopen(file_name, "r");

		if (stream == NULL) {
			printf("vgrep: cannot open file \'%s\'\n", file_name);
			exit(1);
		}

		print_lines_that_match(stream, search_term);
	}

	
	
	return 0;
}

void print_lines_that_match(FILE *stream, char *search_term) {

	char *line = NULL;
	size_t len = 0;

	while (getline(&line, &len, stream) != -1) {
		// Current line is stored in variable line
		char *ss_ptr = strstr(line, search_term);

		if (ss_ptr) {
			printf("%s", line);
		} 

	}

	// We need to print a newline since EOF doesn't do that
	// printf("\n");

	// Close the file and free any allocated memory.
	// strstr() is implemented using malloc(); therefore we need to free the
	// line buffer.
	fclose(stream);
	free(line);
}

