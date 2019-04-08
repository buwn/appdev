#include <stdio.h>
#include <math.h>
#include "sound.h"
#include "screen.h"

//this function takes 1 second of samples (16000 in our
//case) and calculated 80 pieces of RMS value, and then
//turn these values into decibels, and display them as 
//a barchart
void displayWAVDATA(short s[]){
	double rms[80];		//because we have 16000 samples,16000/80=200
	int dB[80];			//used to send decibel values to barchart
	int i,j;			//nested loop couters
	short *ptr =  s;		//use pointer point ot the beginning of the samples

	for(i = 0; i<80; i++){		//outer loop repeater 80 times
		double sum = 0;		//initialize the sum
		for(j = 0; j<200; j++){
			sum += (*ptr)*(*ptr);	//accumulate the sum
			ptr++;					//pointer increments
		}
		rms[i] = sqrt(sum/200);
#ifdef DEBUG
		printf("RMS[%d] = %f\n", i, rms[i]);
#endif
		dB[i] = 20*log10(rms[i]);
	}
#ifndef DEBUG
	barChart(dB);
#endif
}

void showID(char *name,char *value){
	int i;
	printf("%s : ", name);
	for(i=0; i<4; i++)
		printf("%c", value[i]);
	puts("");	//\n
}

void displayWAVHDR(struct WAVHDR h){
#ifdef DEBUG
	showID("ChunkID", h.ChunkID);
	printf("Chunk size: %d\n", h.ChunkSize);
	showID("Format", h.Format);
	showID("Subchunk1ID",h.Subchunk1ID);
	printf("Subchunk1 size: %d\n", h.Subchunk1Size);
	printf("Audio Format: %d\n", h.AudioFormat);
	printf("Num of Channels: %d\n", h.NumChannels);
	printf("Sample rate: %d\n", h.SampleRate);
	printf("Byte rate: %d\n", h.ByteRate);
	printf("Block Align:%d\n", h.BlockAlign);
	printf("Bits per sample: %d\n", h.BitsPerSample);
	showID("Subchunk 2 ID: ",h.Subchunk2ID);
	printf("Subchunk 2 size: %d\n", h.Subchunk2Size);
#else
	setColors(WHITE, bg(RED));
	printf("\033[1;1H");
	printf("test.wav   ");
	setColors(YELLOW, bg(BLUE));
	printf("\033[1;21H");
	printf("Sample rate:%d",h.SampleRate);
	setColors(CYAN,bg(MAGENTA));
	printf("\033[1;41H");
	printf("Duration:%.2fsec ",(float)h.Subchunk2Size/h.ByteRate);
	setColors(RED, bg(YELLOW));
#endif
}
