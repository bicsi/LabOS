#ifndef _FILE_H_
#define _FILE_H_

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

typedef struct _File {
	int desc;
	int oflags;
} File;

File* open_file(char *filename, int oflags, mode_t mode) {
	File *ret = (File*)malloc(sizeof(File));	
	ret->desc = open(filename, oflags, mode);
	ret->oflags = oflags;

	if(ret->desc == -1) {
		char error_message[128];
		sprintf(error_message, "Error while opening %s", filename);
		perror(error_message);
		exit(errno);
	}
	
	return ret;
}
void close_file(File *file) {
	close(file->desc);
	free(file);
	file = NULL;
}

int read_char(File *file, char *c) {
	*c = 0;
	int x = read(file->desc, c, sizeof(char));
	if(x == -1) {
		perror("Error while reading");
		exit(errno);	
	}
	return x;
}
int read_int(File *file, int *x) {
	char c = 0;	
	int mul = 1;
	while(!isdigit(c) && c != '-') 
		if(!read_char(file, &c))
			return 0;
	
	if(c == '-') { 
		mul = -mul;	
		read_char(file, &c);
	}
	
	*x = 0;
	while(isdigit(c)) {		
		*x = (*x) * 10 + c - '0';
		if(!read_char(file, &c))
			break;
	}	
	(*x) *= mul;
	return 1;
}
int read_float(File *file, float *f) {
	char c = 0;
	
	*f = 0;
	while(!isdigit(c) && c != '.' && c != '-')
		if(!read_char(file, &c))
			return 0;
	
	float mul = 1;
	if(c == '-') {
		mul = -mul;
		read_char(file, &c);	
	}
	
	while(isdigit(c)) {
		(*f) = (*f) * 10 + c - '0';
		if(!read_char(file, &c))
			break;	
	}

	if(c == '.') {
		float pw = .1;
		while(1) {
			if(!read_char(file, &c))
				break;
			*f += pw * (c - '0');
			pw *= .1;		
		}
	}
	
	(*f) *= mul;
	return 1;	
}
int read_int_binary(File *file, int *x) {	
	char c;

	*x = 0;	
	for(int i = 0; i < sizeof(int); ++i) {
		if(!read_char(file, &c))
			return 0;
		(*x) |= (c << (8 * i));	
	}
	return 1;
}
int read_float_binary(File *file, float *f) {
	return read(file->desc, f, sizeof(float));
}
void flush_file(File *file) {
	return;
}
void write_char(File *file, char c) {	
	write(file->desc, &c, sizeof(char));
}
void write_int(File *file, int x) {
	char out[32];
	int cnt = sprintf(out, "%d", x);
	for(int i = 0; i < cnt; ++i)
		write_char(file, out[i]);
}
void write_float_binary(File *file, float f) {
	write(file->desc, &f, sizeof(float));
}
void write_int_binary(File *file, int x) {
	for(int i = 0; i < sizeof(int); ++i) {
		write_char(file, (x & 0x000000ff));
		x >>= 8;	
	}
}
off_t file_seek(File *file, off_t offset, int whence) {
	off_t x = lseek(file->desc, offset, whence);
	if(x < 0) {
		perror("Error seeking file");
		printf("offset: %ld, whence: %d\n", offset, whence);
	}
	return x;
}

#endif
