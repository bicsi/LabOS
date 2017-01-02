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
	char buffer[128];
	int cursor, buff_len, desc;
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
	
	ret->cursor = ret->buff_len = 0;
	return ret;
}
void close_file(File *file) {
	close(file->desc);
	free(file);
	file = NULL;
}

int read_char(File *file, char *c) {
	if(file->cursor == file->buff_len) {
		file->buff_len = read(file->desc, file->buffer, sizeof(file->buffer));
		if(file->buff_len == -1) {
			perror("Error while reading");
			exit(errno);	
		}		
		file->cursor = 0;	
	}

	if(file->buff_len == 0) 
		return 0;
	
	
	*c = file->buffer[(file->cursor)++];	
	return 1;
}
int read_int(File *file, int *x) {
	char c = 0;	
	while(!isdigit(c)) 
		if(!read_char(file, &c))
			return 0;

	*x = 0;
	while(isdigit(c)) {		
		*x = (*x) * 10 + c - '0';
		if(!read_char(file, &c))
			break;
	}	
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
void flush_file(File *file) {
	write(file->desc, file->buffer, file->cursor);
	file->buff_len = sizeof(file->buffer);
	file->cursor = 0;
}
void write_char(File *file, char c) {	
	file->buffer[(file->cursor)++] = c;
	if(file->cursor >= file->buff_len)
		flush_file(file);
}
void write_int(File *file, int x) {
	char out[32];
	int cnt = sprintf(out, "%d", x);
	for(int i = 0; i < cnt; ++i)
		write_char(file, out[i]);
}
void write_int_binary(File *file, int x) {
	for(int i = 0; i < sizeof(int); ++i) {
		write_char(file, (x & 0x000000ff));
		x >>= 8;	
	}
}
#endif
