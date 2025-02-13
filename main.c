#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIG_SIZE 8
#define SIGNATURE "\x89\x50\x4e\x47\x0d\x0a\x1a\xa"

/*
 * integers are unsigned unless specified.
 * integers spanning multiple bytes are stored in big-endian.
 */

int main(int argc, char* argv[]) {
	char* filename;
	unsigned char* sig = malloc(SIG_SIZE);

	if(argc >= 2) filename = argv[1];
	else { printf("No file provided.\n"); return 0; } 
	printf("Reading: %s\n", filename);

	FILE* img = fopen(filename, "r");
	if(img == NULL) { printf("File cannot be accessed.\n"); return 0; }

	fread(sig, sizeof(char), SIG_SIZE, img);
	//for(int i=0; i<SIG_SIZE; i++) printf("%x\n", *(sig+i));
	int sigcheck = strncmp(sig, SIGNATURE, SIG_SIZE);
	if(sigcheck != 0) { printf("File is not a PNG file.\n"); return 0; }

	

	fclose(img);
	return 0;
}
