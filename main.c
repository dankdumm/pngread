#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"png.h"

/*
 * integers are unsigned unless specified.
 * integers spanning multiple bytes are stored in big-endian.
 * most modern systems are little-endian.
To keep things simple, I will not be computing the CRC.

CHUNK SIZE = 12 + N
LENGTH - 4 (Length only ctounds the CHUNK DATA, nothing else)
CHUNK TYPE CODE - 4
CHUNK DATA - N 
CRC - 4
*/
//TODO: Add special enums/definitions for CHUNK TYPE NAMES, and use those to identify
//how to process the CHUNK DATA.

void freadchunk(PNG_CHUNK* chunk, FILE* image) {
	unsigned char* data = malloc(8); 
	//Reads LENGTH, CHUNK TYPE CODE
	fread(data, sizeof(char), 8, image);
	
	chunk->length = 0;
	chunk->length += (unsigned int)(data[0]) << 24;
	chunk->length += (unsigned int)(data[1]) << 16;
	chunk->length += (unsigned int)(data[2]) << 8;
	chunk->length += (unsigned int)(data[3]);
	printf("\nCHUNK LENGTH = %i\n", chunk->length);
	
	unsigned int bit5 = 1 << 5;
	chunk->name[0] = data[4];
	chunk->name[1] = data[5];
	chunk->name[2] = data[6];
	chunk->name[3] = data[7];
	printf("CHUNK TYPE CODE: %s\n", chunk->name);
	printf(">Ancillary Bit:%i\n>Private Bit:%i\n>Reserved Bit:%i\n>Safe-To-Copy Bit:%i\n",
		data[4]	& bit5,
		data[5] & bit5,
		data[6] & bit5,
		data[7] & bit5);
	free(data);

	//Reads CHUNK DATA
	data = malloc(chunk->length);
	fread(data, sizeof(char), chunk->length, image);
	for(int i=0; i<chunk->length && chunk->length<256; i++) printf("%i\n", data[i]);
	free(data);

	//Reads CHUNK CRC, but is not used; Only read to ensure the file pointer is advanced.
	data = malloc(4);
	fread(data, sizeof(char), 4, image);
	free(data);
}

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
	else printf("The specified file has the PNG signature.\n");
	
	printf("\n[ %s - INFORMATION ]\n", filename);
	PNG_CHUNK chunk;
	while(strncmp(chunk.name, "IEND", 4) != 0) {
		freadchunk(&chunk, img);
	}

	fclose(img);
	return 0;
}
