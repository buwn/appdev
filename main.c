
#include <stdlib.h>		//for a random numbers
#include <signal.h>
//#include <time.h>		//for randomization (using system time)
#include "screen.h"
#include <stdio.h>
#include "sound.h"
#include "comm.h"

int main(int argc, char **argv){
	if(argv>1){		//if the user has given some command line argument
		printf("Test tone generator\n");
		int fR, fL, ch;
		float duration;
		printf("No. of channels (1 or 2):");
		scanf("%d", &ch);
		if(ch== 1){
			printf("Mono Frequency: ");
			scanf("%d", &fL);
		}
		else if(ch== 2){
			printf("Give me Left and Right freq: ");
			scanf("%d %d", &fL, &fR);
		}
		else{
			printf("Wrong number of channels\n");
			return 1;
		}
		printf("Duration of sound: ");
		scanf("%f", &duration);
		testTone(ch, fL, fR, duration);
		return 0;
	}
	FILE *f;
	short sd[RATE];		//for all samples in 1 sec
	for(;;){

		int ret = system(RCMD);
		if(ret == SIGINT) break;
		f = fopen("test.wav", "r");
		clearScreen();
		if(f == NULL){
			printf("Cannot open the file\n");
			return 1;
		}
		struct WAVHDR h;
		fread(&h, sizeof(h),1,f);		//read WAV header
		displayWAVHDR(h);
		fread(&sd, sizeof(sd), 1, f);
		displayWAVDATA(sd);
		fclose(f);		//close the opened file
		sendDATA(sd);
	}
	resetColors();

}
