/*****************************************************************************
* vunzip.c
* C implementation of the Linux utility unzip 
* NAME: Norman Kuang
*****************************************************************************/

// Libraries
#include <stdio.h>
#include <stdlib.h>		
#include <string.h>		
#include <stdbool.h>

// Function declarations
void uncompress_file(FILE *stream);

int main(int argc, char *argv[]) {

	// Check to see if arguments were supplied.
	if (argc < 2) {
		printf("vunzip: file1 [file2 ...]\n");  // Tip message.
		exit(1);	
	}
	/*
	*	For each iteration, we'll attempt to open the given file and pass it
	*	to the uncompress_file() function for decompression.
	*/
	for (int i = 1; i < argc; i++) {
		char *file_name = argv[i];

		FILE *stream = fopen(file_name, "r");

		if (stream == NULL) {
			printf("vunzip: cannot open file\n");  // Error message.
			exit(1);
		} 

		uncompress_file(stream);
	}

	return 0;
}

/* 
*  uncompress_file() attempts to fread 5 bytes at a time, broken down into a 
*  4-byte read (extracting the integer portion of RLE) followed by a 1-byte
*  read (extracting the ASCII character portion of RLE). A boolean flag is 
*  used to keep track of EOF or unsuccessful freads.
*/
void uncompress_file(FILE *stream) {

	// Variable declarations
	int count = 0;
	char ch = 0;
	bool read_flag = true;

	while (read_flag) {

		// At the beginning of each iteration, count and ch will store the 
		// previous iteration's compression count and ASCII value. These can
		// then be printed using printf.
		for (int i = 0; i < count; i++) {
			printf("%c", ch);
		}

		// fread will return 4 if it successfully reads in a 4-byte integer, 
		// else the read was unsuccessful or EOF reached. In addition, a 
		// successful read will store the integer at the address of the count
		// variable.
		if ((fread(&count, 1, sizeof(int), stream)) != 4) {
			read_flag = false;  // Terminate.
		}

		// fread will return 1 if it successfully reads in a 1-byte ASCII, 
		// else the read was unsuccessful or EOF reached. In addition, a 
		// successful read will store the char at the address of the ch
		// variable.
		if ((fread(&ch, 1, sizeof(char), stream)) != 1) {
			read_flag = false;  // Terminate.
		}

	}

	// Close the file.
	fclose(stream);
}

