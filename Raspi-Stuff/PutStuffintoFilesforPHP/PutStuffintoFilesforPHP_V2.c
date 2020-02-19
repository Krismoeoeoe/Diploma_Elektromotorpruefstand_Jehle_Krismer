// author: thetoastiestbread
// date: 18.02.20
// filename: PutStuffIntoFilesforPHP_V2.c
// desc: Never closes the file :/

#include <stdio.h>

const char filepath[] = "cmdanddata.txt";

void setupFile(FILE *datafile){
	fprintf(datafile, "[00000;000.00]\n[f]\n[f]");		// second column: command given from php, third column: state this C file is in right now (i.e. free, working on the svg file and so on)
}

void writeToFile(FILE *datafile, int rpm, float wg){
	rewind(datafile);									// back to the start of the file
	fprintf(datafile, "[%05d;%06.2f]", rpm, wg);		// '6' means length of ALL of the output string
}

char readCmdFromFile(FILE *datafile){
	char buff;
	
	fseek(datafile, 16, SEEK_SET);
	// fuckfuckfuckfuck
	buff = fgetc(datafile);
	return buff;
}


int main(){
	int rpm = 1874;
	float wg = 96.81;

	FILE *datafile;
	char cmd;
	
	datafile = fopen(filepath, "w+t");
	
	setupFile(datafile);					// Initialize first proper file state
	
	writeToFile(datafile, rpm, wg);			// write your rpm and wg measurings to the file
	
	cmd = readCmdFromFile(datafile);				// read your given command from file
	
	printf("My given command is: [%c]", cmd);
	
	fclose(datafile);
	
	return 0;
}
