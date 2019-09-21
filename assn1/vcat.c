/* C implementation of the Linux utility cat */

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_LINE_LEN 999  // Arbitrary file line length.	

// Function declaration
void print_file(char *file_name);											 

int main(int argc, char *argv[]) {

	// We may have multiple files, in which case we'll loop through
	// and print out the contents of each. Loop starts at 1 since the first
	// file is found at argv[1].
	for (int i = 1; i < argc; i++) {
		// Let's extract the filename and store it.
		// Elements of argv are pointers to a char array
		char *file_name = argv[i];

		print_file(file_name);
	}
	
	return 0;
}

/* print_file() takes the name of a file and prints out its contents */
void print_file(char *file_name) {
	// Open the file
	FILE *f_ptr = fopen(file_name, "r");

	// Ensure we are able to open the file
	if (f_ptr == NULL) {
		printf("vcat: cannot open file\n");
		exit(1);
	}

	// Method 1) Using fgetc(). Get the next char from the file and advance
	// 	the file position indicator. Advances char-by-char until EOF.
	char ch;

	while ((ch = fgetc(f_ptr)) != EOF) {
		printf("%c", ch);
	}

	// Method 2) Using fgets(). Since fgets requires that we specify a char 
	// array to store the line to be read, we need to instantiate a char
	// array and pass its length argument into fgets. 
	// Reads (MAX_FILE_LINE_LEN - 1) or until newline or EOF, whichever comes
	// first

	// char str[MAX_FILE_LINE_LEN];

	// while (1) {  // Loop until we reach EOF
	// 	if (fgets(str, MAX_FILE_LINE_LEN, f_ptr)) {
	// 		printf("%s", str);
	// 	} else {
	// 		break;
	// 	}
	// } 

	// Finished. Let's close the file.
	fclose(f_ptr);
}