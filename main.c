
#include <stdlib.h>		//for random numbers
#include <signal.h>
//#include <time.h>		//for randomization (using system time)
#include "screen.h"
#include <stdio.h>
#include "sound.h"
#include "comm.h"

int main(){
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
