#include <stdio.h>
#include "file_no_buff.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		puts("Error! Not enough parameters specified!");
		printf("Usage is: %s file\n", argv[0]);
		return 0;
	}

	File *out = open_file(
		argv[1], 
		O_WRONLY | O_CREAT | O_TRUNC, 
		S_IWRITE | S_IREAD
	);

	int a, b;
	printf("Enter dimensions: ");
	scanf("%d %d", &a, &b);
	
	printf("Enter matrix, one line at a time\n");
	write_int_binary(out, a);
	write_int_binary(out, b);

	for(int i = 0; i < a; ++i)
		for(int j = 0; j < b; ++j) {
			float f;
			scanf("%f", &f);
			write_float_binary(out, f);		
		}

	flush_file(out);	
	close_file(out);
	puts("Done generating matrix!");
	// puts("Program ended OK!");
	return 0;
}