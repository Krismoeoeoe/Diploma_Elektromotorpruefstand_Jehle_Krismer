// author: thetoastiestbread
// date: 18.02.20
// filename: PutStuffINtoFilesforPHP_V1.c
// desc:

#include <stdio.h>

const char filepath[] = "cmdanddata.txt";

int main(){
	int rpm = 1874;
	float wg = 96.81;
	
	FILE datafile;
	
	datafile = fopen(filepath, "w");
	fprintf(datafile, "[%05d;%06.2f]", rpm, wg);		// '6' means length of ALL of the output string
	fclose(datafile);
	
	return 0;
}