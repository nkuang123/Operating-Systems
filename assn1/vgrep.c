/*****************************************************************************
* vgrep.c
* C implementation of the Linux utility grep 
* NAME: Norman Kuang
*****************************************************************************/

// Libraries
#include <stdio.h>
#include <stdlib.h>		
#include <string.h>

// Function declarations
void print_lines_that_match(FILE* stream, char *search_term);

int main(int argc, char *argv[]) {

	// Check to see if arguments were supplied.
	if (argc < 2) {
		// No arguments were passed.
		printf("vgrep: searchterm [file ...]\n");  // Tip message.
		exit(1);
	}

	// Extract the search term.
	char *search_term = argv[1];

	// Only search term supplied, no files, read from stdin stream.
	if (argc == 2) {
		print_lines_that_match(stdin, search_term);
	}

	// Loop executes if files are supplied as arguments. For each iteration,
	// extract the file name, attempt to open it, and pass it to the 
	// print_lines_that_match() function for further processing.
	for (int i = 2; i < argc; i++) {
		// Extract 
		char *file_name = argv[i];

		FILE *stream = fopen(file_name, "r");

		if (stream == NULL) {
			printf("vgrep: cannot open file\n");  // Error Message.
			exit(1);
		}

		print_lines_that_match(stream, search_term);
	}

	return 0;
}

/* 
*  print_lines_that_match() prints all lines in a given stream that match the 
*  supplied search term string. 
*/
void print_lines_that_match(FILE *stream, char *search_term) {

	// Variable declarations
	char *line = NULL;  // File line.
	size_t len = 0;  // Line length.

	// getline() returns the number of characters read on a successful read
	// and -1 on an unsuccessful read or EOF. We'll loop until the latter 
	// is true.
	while (getline(&line, &len, stream) != -1) {
		// Current line is stored in variable line. Use strstr() in order to 
		// match substrings. strstr() returns a pointer to the first char of 
		// the search_term found in line. If this pointer exists, we'll print
		// the line.
		char *substring_ptr = strstr(line, search_term);

		if (substring_ptr) {
			printf("%s", line);
		} 

	}

	// Close the file and free any allocated memory.
	// strstr() is implemented using malloc(); therefore we need to free the
	// line buffer.
	fclose(stream);
	free(line);
}