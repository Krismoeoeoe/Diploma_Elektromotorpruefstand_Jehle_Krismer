// author: thetoastiestbread
// date: 09.02.20
// filename: rxData_V3.c
// fucking gets fucking data from fucking arduino and will fucking give it to you it finally goddamn does shit right, now getting to fucking filestuff


#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


// UART SETUP:
int ser_init(){
	int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);	// open that thing, READ AND WRITE, thing about terminal not giving up or shit, and no delay
	if(fd == -1) printf("ERROR - Unable to open UART!\n");
	
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;	// 115200 Baud | 8 bits | ignore modem status lines | enable receiver
	options.c_iflag = IGNPAR;				// ignore parity errors
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(fd, TCIFLUSH);					// flush data rxd but not read
	tcsetattr(fd, TCSANOW, &options);		// DOITNAUW
	return fd;
}


char rx_byte(int fd){
	char buff[1];
	char retbuff;
	
	int howmany = read(fd, (void*)buff, 1);			// read 1 byte into buff
	if(howmany <= 0) return -1;
	else{
		retbuff = buff[0];
		return retbuff;
	}
}

void rx_res(int fd, float *wg, int *rpm){

	float wg_rxres = 0.00;
	int rpm_rxres = 0;
	int cnt = 0;
	char buff = 0;
	float fbuff = 0;

	for(;;){
		buff = rx_byte(fd);
		if(buff == 2) break;
	}

	for(;;){
		
		buff = rx_byte(fd);						// get a byte/bite
		
		if(buff == -1) continue;				// if nothing was rxd start over
		else if(buff == '\n') break;
		else{
			if(isdigit(buff) != 0){
				fbuff = buff - 48;
				switch(cnt){						// calc that shit
					case 0: wg_rxres += fbuff * 100; break;
					case 1: wg_rxres += fbuff * 10; break;
					case 2: wg_rxres += fbuff; break;
					case 3: wg_rxres += fbuff / 10; break;
					case 4: wg_rxres += fbuff / 100; break;
					case 5: rpm_rxres += fbuff * 10000; break;
					case 6: rpm_rxres += fbuff * 1000; break;
					case 7: rpm_rxres += fbuff * 100; break;
					case 8: rpm_rxres += fbuff * 10; break;
					case 9: rpm_rxres += fbuff; break;
					default: break;
				}
				cnt++;
			}
		}
	}
	*wg = wg_rxres;
	*rpm = rpm_rxres;
}


int main(){
	float wg = 0;
	int rpm = 0;
	
	int usb_ser_fd = 0;
	
	usb_ser_fd = ser_init();
	
	for(;;){
		rx_res(usb_ser_fd, &wg, &rpm);
		printf("%03.2f;%05d\n",wg, rpm);
	}
	close(usb_ser_fd);
	return 0;
}
