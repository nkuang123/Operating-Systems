/*****************************************************************************
* vzip.c
* C implementation of the Linux utility zip 
* NAME: Norman Kuang
*****************************************************************************/

// Libraries
#include <stdio.h>
#include <stdlib.h>		
#include <string.h>	

// Function declarations
void compress_file(FILE *stream, char *last_char, int *curr_count);

int main(int argc, char *argv[]) {

	// Check to see if arguments were supplied.
	if (argc < 2) {
		printf("vzip: file1 [file2 ...]\n");  // Tip message.
		exit(1);	
	}
	/*
		In the case of multiple files, we'll need to keep track of 2 things,
		namely the last character of the (ith - 1) file and the count of that
		character. This is so that we can roll over to the ith file and
		continue the compression as if all of the files' contents were in one 
		file. I've chosen to use pointers as this eases the process of 
		transferring data between functions by taking advantage of pass-by-
		reference.
	*/
	char last_char = (char) 0;
	char *last_char_ptr = &last_char;

	int curr_count = 0;
	int *curr_count_ptr = &curr_count;

	// Loop executes if files are supplied as arguments. For each iteration,
	// extract the file name, attempt to open it, and pass it to the 
	// compress_file() function for compression.
	for (int i = 1; i < argc; i++) {
		char *file_name = argv[i];

		FILE *stream = fopen(file_name, "r");

		if (stream == NULL) {
			printf("vzip: cannot open file\n");  // Error message.
			exit(1);
		} 

		compress_file(stream, last_char_ptr, curr_count_ptr);
	}

	// We need to manually compress the last char and its count of the very
	// last file since EOF terminates early. This is easy since the pointers
	// contain the last file's last character and its count.
	fwrite(curr_count_ptr, sizeof(int), 1, stdout);
	fwrite(last_char_ptr, sizeof(char), 1, stdout);

	return 0;
}

/* 
*  compress_file() goes through all characters in a given stream and 
*  compresses them following the run-length encoding format. The 
*  last_char_ptr and curr_count_ptr get updated at the EOF for file roll-over
*  purposes.
*/
void compress_file(FILE *stream, char *last_char_ptr, int *curr_count_ptr) {

	// Variable declarations
	char ch;
	char next;
	int count = 0;

	// Compare the first character of the stream with the previous file's last
	// character for roll-over purposes. If they are the same, we update the
	// count with the last character's current count.
	ch = fgetc(stream);

	if (ch == *last_char_ptr) {
		count = *curr_count_ptr;
	}

	ungetc(ch, stream);

	/*
		Read char-by-char until EOF. For each iteration, we update the count of
		the current character. We then compare the next character 
		(using fgetc -> ungetc, essentially peeking) with the 
		current character; if they are the same, continue on. If they differ,
		we fwrite the count of the current character and the current character
		to stdout. In the special case of EOF, we update the two input pointers
		and break out of the loop.
	*/
	while ((ch = fgetc(stream)) != EOF) {

		count++;

		if ((next = fgetc(stream)) != ch) {  // next char != current char.

			if (next == EOF) {
				*last_char_ptr = ch;
				*curr_count_ptr = count;
				return;
			}

			ungetc(next, stream);  // fgetc advances pointer; move it back.

			// Compress.
			fwrite(&count, sizeof(int), 1, stdout);
			fwrite(&ch, sizeof(ch), 1, stdout);

			// Reset the count for the next new character.
			count = 0;

		} else {  // next char == current char.
			ungetc(next, stream);  // fgetc advances pointer; move it back.
		}
	}

	// Close the file.
	fclose(stream);
}

