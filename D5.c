#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

void recursive_delete(char *path) {
	printf("Deleting %s...\n", path);

	DIR *root_dir = opendir(path);
	
	if(root_dir == NULL) {
		if(errno == ENOTDIR) {
			// Not a dir
			unlink(path);					
		} else {
			perror("Error while opening directory");
			exit(errno);
		}	
	} else {
		struct dirent *child;
		// Check for directories to recurse into
		while(child = readdir(root_dir)) {
			if(child->d_name[0] == '.')
				continue;

			char new_path[512];
			strcpy(new_path, path);
			strcat(new_path, "/");
			strcat(new_path, child->d_name);

			recursive_delete(new_path);
		}
		
		rmdir(path);			
	}	

	printf("Deleted %s!\n", path);
}

int main(int argc, char **argv) {
	if(argc != 2) {
		puts("Error! Not enough parameters specified!");
		printf("Usage is: %s path\n", argv[0]);
		return 0;
	}

	recursive_delete(argv[1]);

	return 0;
}