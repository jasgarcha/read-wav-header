#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

struct wavheader {
	uint8_t chunkid[4];
	uint8_t chunksize[4];
	uint8_t format[4];
	uint8_t subchunk1id[4];
	uint8_t subchunk1size[4];
	uint8_t audioformat[2];
	uint8_t numchannels[2];
	uint8_t samplerate[4];
	uint8_t byterate[4];
	uint8_t blockalign[2];
	uint8_t bitspersample[2];
	uint8_t subchunk2id[4];
	uint8_t subchunk2size[4];
};

uint32_t btoi(uint8_t bytes[4])
{
	union btoi {
		uint8_t b[4];
		uint32_t i;
	};
	union btoi u;
	memcpy(u.b, bytes, 4);
	return u.i;
}

uint16_t btos(uint8_t bytes[2])
{
	union btos {
		uint8_t b[2];
		uint16_t s;
	};
	union btos u;
	memcpy(u.b, bytes, 2);
	return u.s;
}

int parse(FILE *wavfile, struct wavheader *wh)
{
	const uint8_t CHUNK_ID[4] = {0x52, 0x49, 0x46, 0x46}; //"RIFF" big endian.
	const uint8_t FORMAT[4] = {0x57, 0x41, 0x56, 0x45}; //"WAVE" big endian.
	const uint8_t SUBCHUNK1_ID[4] = {0x66, 0x6d, 0x74, 0x20}; //"fmt " big endian.
	const uint8_t SUBCHUNK2_ID[4] = {0x64, 0x61, 0x74, 0x61}; //"data" big endian.	
	if(!wavfile || !wh)
		return -1;		
	fread(wh->chunkid, 4, 1, wavfile);		
	fread(wh->chunksize, 4, 1, wavfile);		
	fread(wh->format, 4, 1, wavfile);
	fread(wh->subchunk1id, 4, 1, wavfile);
	fread(wh->subchunk1size, 4, 1, wavfile);
	fread(wh->audioformat, 2, 1, wavfile);
	fread(wh->numchannels, 2, 1, wavfile);
	fread(wh->samplerate, 4, 1, wavfile);
	fread(wh->byterate, 4, 1, wavfile);
	fread(wh->blockalign, 2, 1, wavfile);
	fread(wh->bitspersample, 2, 1, wavfile);
	fread(wh->subchunk2id, 4, 1, wavfile);
	fread(wh->subchunk2size, 4, 1, wavfile);	
	for(int i = 0; i < 4; i++)
		if(*wh->chunkid != CHUNK_ID[i] || *wh->format != FORMAT[i] || *wh->subchunk1id != SUBCHUNK1_ID[i] || *wh->subchunk2id != SUBCHUNK2_ID[i])
			return -1;			
	return 1;
}

int print(struct wavheader *wh)
{
	if(!wh)
		return -1;
	printf("Chunk Id: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->chunkid[i]);
	}
	printf("\nChunk Size: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->chunksize[i]);
	}
	printf(" = %i little endian.", btoi(wh->chunksize));
	printf("\nFormat: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->format[i]);
	}
	printf("\nSubchunk 1 Id: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->subchunk1id[i]);
	}
	printf("\nSubchunk 1 Size: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->subchunk1size[i]);
	}
	printf(" = %i little endian.", btos(wh->subchunk1size));
	printf("\nAudio Format: 0x");
	for(int i = 0; i < 2; i++)
	{
		printf("%02X", wh->audioformat[i]);
	}
	printf("\nNumber of Channels: 0x");
	for(int i = 0; i < 2; i++) 
	{
		printf("%02X", wh->numchannels[i]);	
	}
	printf(" = %i little endian.", btos(wh->numchannels));
	printf("\nSample Rate: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->samplerate[i]);
	}
	printf(" = %i little endian.", btos(wh->samplerate));
	printf("\nByte Rate: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->byterate[i]);
	}
	printf(" = %i little endian.", btos(wh->byterate));
	printf("\nBlock Align: 0x");
	for(int i = 0; i < 2; i++)
	{
		printf("%02X", wh->blockalign[i]);
	}
	printf(" = %i little endian.", btos(wh->blockalign));
	printf("\nBits Per Sample: 0x");	
	for(int i = 0; i < 2; i++)
	{
		printf("%02X", wh->bitspersample[i]);
	}
	printf(" = %i little endian.", btos(wh->bitspersample));
	printf("\nSubchunk 2 Id: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->subchunk2id[i]);
	}
	printf("\nSubchunk 2 Size: 0x");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X", wh->subchunk2size[i]);
	}
	printf(" = %i little endian.", btos(wh->subchunk2size));
	return 1;
}

int main(int argc, char *argv[])
{ 
	if(argc == 1) 
	{
		printf("Enter the header of the wav file as a hexadecimal string: ");
		/*uint8_t wh[sizeof(struct wavheader)];
		if(!parse(wavfile, &wh))
		{
			fprintf(stderr, "Wav header does not match the canonical wav file format.");
			return -1;
		}
		if(!print(&wh))
		{
			fprintf(stderr, "Pointer to wav file is null.");
			return -1;
		}
		return 0;*/
	}	
	else if(argc > 1 && argc <= 2)
	{
		FILE *wavfile = fopen(argv[1], "r");
		struct wavheader wh;
		if(!wavfile)
		{
			fprintf(stderr, "Error: %s.",  strerror(errno));
			return -1;
		}				
		if(!parse(wavfile, &wh))
		{
			fprintf(stderr, "Wav header does not match the canonical wav file format.");
			return -1;
		}
		if(!print(&wh))
		{
			fprintf(stderr, "Pointer to wav file is null.");
			return -1;
		}
		if(fclose(wavfile) != 0)
		{
			fprintf(stderr, "Error: %s.",  strerror(errno));
			return -1;
		}		
		return 0;
	}
	else
	{
		fprintf(stderr, "Invalid arguments. Enter file path or hexadecimal string.");
		return -1;
	}
}