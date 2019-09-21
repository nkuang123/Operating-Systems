/* C implementation of the Linux utility zip */

#include <stdio.h>
#include <stdlib.h>		
#include <string.h>		
#include <stdbool.h>

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("vzip: file1 [file2 ...]\n");
		exit(1);
	}

	char *file_name = argv[1];



	return 0;
}