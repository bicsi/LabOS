#include "file.h"
#include <stdio.h>

int main(int argc, char** argv) {
	if(argc != 3) {
		puts("Error! Not enough parameters specified!");
		printf("Usage is: %s file1 file2\n", argv[0]);
		return 0;
	}

	File *in = open_file(argv[1], O_RDONLY, S_IREAD);
	File *out = open_file(
		argv[2], 
		O_WRONLY | O_CREAT | O_TRUNC, 
		S_IWRITE | S_IREAD
	);
	
	int x;
	while(read_int_binary(in, &x)) {
		write_int(out, x);
		write_char(out, ' ');
	}
	write_char(out, '\n');

	close_file(in);
	flush_file(out);	
	close_file(out);
	
	// puts("Program ended OK!");
	return 0;
}