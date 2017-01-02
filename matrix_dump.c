#include <stdio.h>
#include "file_no_buff.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		puts("Error! Not enough parameters specified!");
		printf("Usage is: %s file\n", argv[0]);
		return 0;
	}

	File *in = open_file(argv[1], O_RDONLY, S_IREAD);

	int a, b;
	read_int_binary(in, &a);
	read_int_binary(in, &b);
	
	printf("Matrix[%d %d]:\n", a, b);
	for(int i = 0; i < a; ++i) {
		for(int j = 0; j < b; ++j) {
			float f;
			read_float_binary(in, &f);
			printf("%f ", f);		
		}
		printf("\n");
	}

	close_file(in);
	// puts("Program ended OK!");
	return 0;
}