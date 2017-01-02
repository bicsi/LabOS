#include "matrix.h"
#include "file_no_buff.h"

void dump_matrix(File *file) {
	int rows, cols;
	matrix_read_dimensions(file, &rows, &cols);
	printf("Matrix[%d, %d]:\n", rows, cols);
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j)
			printf("%f ", matrix_get(file, i, j));
		printf("\n");	
	}
}

int main(int argc, char **argv) {
	if(argc != 4) {
		puts("Error! Not enough parameters specified!");
		printf("Usage is: %s file1 file2 o_file\n", argv[0]);
		return 0;
	}
	
	File *in1 = open_file(argv[1], O_RDONLY, S_IREAD);
	File *in2 = open_file(argv[2], O_RDONLY, S_IREAD);
	File *out = open_file(
		argv[3], 
		O_RDWR | O_CREAT | O_TRUNC, 
		S_IWRITE | S_IREAD
	);

	
	
	int rows1, cols1, rows2, cols2;
	matrix_read_dimensions(in1, &rows1, &cols1);	
	matrix_read_dimensions(in2, &rows2, &cols2);

	if(cols1 != cols2 || rows1 != rows2) {
		puts("Invalid dimensions!");
		return -1;	
	}
	
	matrix_new(out, rows1, cols2);
	
	for(int i = 0; i < rows1; ++i)
		for(int j = 0; j < cols1; ++j) {
			float a = matrix_get(in1, i, j),
			      b = matrix_get(in2, i, j);
			float acc = a + b;
			//printf("%d %d %f\n", i, j, acc);		
			matrix_set(out, i, j, acc);
		}
	
	flush_file(out);
	return 0;
}