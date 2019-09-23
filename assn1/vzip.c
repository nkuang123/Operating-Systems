/* C implementation of the Linux utility zip */

#include <stdio.h>
#include <stdlib.h>		
#include <string.h>		
#include <stdbool.h>

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("vzip: file1 [file2 ...]\n");
		exit(1);	}

	char *file_name = argv[1];

	FILE *stream = fopen(file_name, "r");
	FILE *write_ptr = fopen("test.bin","wb");

	char ch;
	char next;
	int count = 0;

	while ((ch = fgetc(stream)) != EOF) {

		count++;

		if ((next = fgetc(stream)) != ch) {
			ungetc(next, stream);

			fwrite(&count, sizeof(int), 1, stdout);
			fwrite(&ch, 1, sizeof(ch), stdout);
			// printf("%x%c", count, ch);


			count = 0;
		} else {
			ungetc(next, stream);
		}
	}

	printf("\n");

	fclose(stream);
	fclose(write_ptr);



	return 0;
}