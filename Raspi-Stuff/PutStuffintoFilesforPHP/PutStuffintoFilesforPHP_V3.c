// author: thetoastiestbread
// date: 19.02.20
// filename: PutStuffIntoFilesforPHP_V3.c
// desc: Writes Messergebnisse into file for PHP script to come get. Also gets commands from PHP script like, HEY YOU FRICK, GO MEASURE, and such stuff. Also an additional line for stuff that maybe the C script will want to tell the PHP stuff... not sure about that yet.

#include <stdio.h>

void setupFile(char filepath[]){
	FILE *fp = fopen(filepath, "w");
	fprintf(fp, "[00000;000.00]\n[f]\n[f]");		// second column: command given from php, third column: state this C file is in right now (i.e. free, working on the svg file and so on)
	fclose(fp);
}

void writeToFile(char filepath[], int rpm, float wg){
	FILE *fp = fopen(filepath, "r+");
	rewind(fp);									// back to the start of the file
	fprintf(fp, "[%05d;%06.2f]", rpm, wg);		// '6' means length of ALL of the output string
	fclose(fp);
}

char readCmdFromFile(char filepath[]){
	char buff;
	
	FILE *fp = fopen(filepath, "r+");
	fseek(fp, 16, SEEK_SET);
	buff = fgetc(fp);
	fclose(fp);
	
	return buff;
}


int main(){
	char filepath[] = "cmdanddata.txt";
	
	int rpm = 1874;
	float wg = 96.81;
	
	char cmd;
	
	setupFile(filepath);					// Initialize first proper file state
	
	writeToFile(filepath, rpm, wg);			// write your rpm and wg measurings to the file
	
	cmd = readCmdFromFile(filepath);				// read your given command from file
	
	printf("My given command is: [%c]", cmd);
	
	return 0;
}