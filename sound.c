#include <stdio.h>
#include <math.h>
#include "sound.h"
#include "screen.h"

//this function is only called by displayWAVDATA(), so no need to put
// a declaration in sound.h.The function finds how many peaks from 80-pieces
//of decibel values
int findPeaks(int d[]){
	int c = 0, i;
	for (i=1; i<80; i++){
		if(d[i] >=75 && d[i-1] <75) c++;
	}
	if(d[0] >= 75) c++;
	return c;
}

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
	int peaks = findPeaks(dB);
	setColors(WHITE, bg(BLACK));
	printf("\033[1;61H");
	printf("Peaks: %d          \n",peaks);
#endif
}

void fillID(char *d, const char *s)
{
	for(int i=0; i<4; i++)
		*d++ = *s++;
}
void testTone(int c,int fl, int fr, float d)
{
	if(c<1 || c>2){
		printf("Wrong number of channels.\n");
		return;
	}
	if(fl<50 || fl>16000){
		printf("Frequency is out of range\n");
		return;
	}
	if(d<1.0 || d>10.0){
		printf("Wrong duration!\n");
		return;
	}
	// all the arguments are okay, the rest of code is to
	//1)make a correct wave header,2) generate correct samples
	//3)write both header and samples to a file
	struct WAVHDR h;
	int samples = 44100*d;
	fillID(h.ChunkID, "RIFF");
	fillID(h.Format, "WAVE");
	fillID(h.Subchunk1ID, "fmt ");
	fillID(h.Subchunk2ID, "data");
	h.Subchunk1Size = 16;		//constanct value
	h.AudioFormat = 1;
	h.SampleRate = 44100;
	h.BitsPerSample = 16;
	h.BlockAlign = c*16/8;
	h.NumChannels = c;
	h.ByteRate = 44100*c*16/8;
	h.Subchunk2Size = samples*c*16/8;
	h.ChunkSize = h.Subchunk2Size + 36;
	FILE *fp = fopen("testTone.wav", "w");
	if(fp == NULL){
		printf("Cannot open a file\n");
		return;
	}
	fwrite(&h, sizeof(h), 1 , fp);	//write the header to file
	// generate samples, and write to file
	for(int i = 0; i<samples;i++){
		short sL = 32767.0 * sin(2*PI*fl*i/44100);
		fwrite(&sL, sizeof(short), 1, fp);
		if(c==2){
			short sR = 32767.0 * sin(2*PI*fr*i/44100);
			fwrite(&sR, sizeof(short), 1, fp);
		}
	}	//end of for
	fclose(fp);		//close the file
	printf("TestTone is generated!\n");
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
