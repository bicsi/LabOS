#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <string.h>
#include "file_no_buff.h"
#include <assert.h>

void matrix_read_dimensions(File *file, int *rows, int *cols) {
	assert(file_seek(file, 0, SEEK_SET) == 0);
	assert(read_int_binary(file, rows));
	assert(read_int_binary(file, cols));
}

void matrix_new(File *file, int rows, int cols) {
	file_seek(file, 0, SEEK_SET);
	
	write_int_binary(file, rows);
	write_int_binary(file, cols);
	
	for(int i = 0; i < rows * cols; ++i)
		write_float_binary(file, 0);	
	flush_file(file);
}

void _matrix_seek_to(File *file, int row, int col) {
	int rows, cols;
	matrix_read_dimensions(file, &rows, &cols);
	
	off_t offset = cols * row + col;
	file_seek(file, 2 * sizeof(int) + sizeof(float) * offset, SEEK_SET);
}

float matrix_get(File *file, int row, int col) {
	_matrix_seek_to(file, row, col);
	float ret = 0;
	assert(read_float_binary(file, &ret));
	return ret;
}

void matrix_set(File *file, int row, int col, float value) {
	_matrix_seek_to(file, row, col);
	write_float_binary(file, value);
}

#endif