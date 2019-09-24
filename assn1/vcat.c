/*****************************************************************************
* vcat.c
* C implementation of the Linux utility cat 
* NAME: Norman Kuang
*****************************************************************************/

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Constants
#define MAX_FILE_LINE_LEN INT_MAX  // Arbitrary file line length.	

// Function declarations
void print_file(char *file_name);											 

int main(int argc, char *argv[]) {

	// We may have multiple files, in which case we'll loop through
	// and print out the contents of each. Loop starts at 1 since the first
	// file is found at argv[1].
	for (int i = 1; i < argc; i++) {
		// Let's extract the filename and store it.
		char *file_name = argv[i];

		print_file(file_name);
	}
	
	return 0;
}

/* print_file() takes the name of a file and prints out its contents */
void print_file(char *file_name) {
	// Open the file
	FILE *stream = fopen(file_name, "r");

	// Ensure we are able to open the file
	if (stream == NULL) {
		printf("vcat: cannot open file\n");  // Error message.
		exit(1);
	}

	/*
	*  This procedure uses fgetc(). Get the next char from the file stream, 
	*  print the character, and advance the file position indicator. Advances 
	*  char-by-char until EOF.
	*/
	char ch;

	while ((ch = fgetc(stream)) != EOF) {
		printf("%c", ch);
	}

	// Finished. Let's close the file.
	fclose(stream);
}