#ifndef PNG_H

#define PNG_H
#define SIG_SIZE 8
#define SIGNATURE "\x89\x50\x4e\x47\x0d\x0a\x1a\xa"

typedef struct PNG_CHUNK {
	unsigned int length;
	unsigned char name[4];
	unsigned int propbits[4];
	unsigned char* data;
	unsigned char crc[4];
} PNG_CHUNK;

#endif
