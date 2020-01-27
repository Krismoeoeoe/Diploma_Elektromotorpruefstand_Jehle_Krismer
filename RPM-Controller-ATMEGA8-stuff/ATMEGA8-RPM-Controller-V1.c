/*
 * ATMEGA8-RPM-Controller-V1.c
 *
 * Created: 27.01.2020 17:55:09
 * Author : thetoastiestbread
 */ 

#include <avr/io.h>

#define F_CPU 1000000UL
#define BAUD 4800

#define MYUBRR (F_CPU/(16*BAUD)-1)

#define ADC_INPUT 0

// ACTIVE CONTROL STUFF

int Serial_readChr(){
	// Return Value currently in the Data Register
	return UDR;
}

unsigned Serial_avail(){
	if( !( UCSRA & (1<<RXC) ) ) return 0;
	else return 1;
}

void Update_CtrlCntr1(unsigned int newOCRB){
	// Change the ADC-converted value into the Compare Value for
	// the timer: 1000 = 1ms ON-time ; 2000 = 2ms ON-time
	newOCRB = 1000 + newOCRB * 0.9765625;

	// Write the value into Compare Register so On-Time will
	// be right.
	OCR1B = newOCRB;
}

unsigned int ADC_read(unsigned int adc_chan){
	// Set ADC channel to 1 so that Input Buffer isn't active
	ADMUX = (ADMUX & ~(0x1F)) | (adc_chan & 0x1F);
	// Activate ADC-Conversion
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1<<ADSC) ) {}	// waiting for end of conversion
	ADMUX = (ADMUX & ~(0x1F));

	return ADCW;
}

// INIT STUFF

void Init_ADC(){
	ADMUX = 0;
	ADCSRA = 0;

	/* ADMUX beginning */
	// AREF Pin with internal Vref off
	ADMUX = 0;

	// ADLAR ADC Left Adjust stuff... don't really care for 
	// that... I think
	// Other stuff in ADMUX just for selecting input so, that later

	/* ADCSRA beginning */
	// Enable the ADC. Make it be an ADC, basically [ADEN]
	ADCSRA |= (1 << ADEN);

	// Next would be ADSC - Start ADC Conversion. [ADSC]
	// Obviously, wont set it now. Will clear itself
	// after Conversion

	// [ADFR] - ADC Free Running Select. Yeah, no, thanks. If 1 would
	// basically just keep on adcing.

	// [ADIF] - ADC Interrupt Flag. Set when Conversion 
	// is complete. Not using it.

	// [ADIE] - ADC Interrupt Enable. Does what the name says
	// Not using it.

	// [ADPS2:0] - ADC Prescaler Select. Division factor 
	// clock to ADC frequency. No idea what to use... Let's
	// do 32 or sthg.
	ADCSRA |= (1 << ADPS2) | (1 << ADPS0);

	/* [ADC9:0] ADCL and ADCH - The ADC Data Register.
	Where the converted stuff is put. Read from here. */
}

void Init_CtrlCntr1(void){
	// Set up 50Hz Controller Timer1 -> 50Hz Signal with 1-2ms...
	// ...ON-Time. OCRB = 2000 for 1ms ; = 4000 for 2ms
	TCCR1A = 0;
	TCCR1B = 0;
	OCR1A = 0;
	OCR1B = 0;
	
	// Set Mode 15 (16Bit "PWM")
	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);

	// Set Compare Output Modes
	TCCR1A |= (1 << COM1B1);

	// Set TOP for Timer OCRnA ; 20000 at 1MHz Clk for 50Hz
	OCR1A = 20000;

	// set Comp value for OCRnB (1000 for 1ms, 2000 for 2ms)
	OCR1B = 1000;

	// set Prescaler (clk/1)
	TCCR1B |= (1 << CS10);

	// Pin 16 as Output (OC1B/PB2)
	DDRB = 0b00000100;
}

void Serial_init(unsigned int ubrr){
	UBRRH = 0;
	UBRRL = 0;
	UCSRB = 0;
	UCSRC = 0;

	// Set UBRR for BAUD
	UBRRH = ubrr >> 8;
	UBRRL = ubrr & 0xFF;
	// Enable Receiver and/or Transmitter (Here only TX)
	UCSRB |= (1 << RXEN);
	// Set Modes (Here Asynchronous modes, no parity, 8 data bits, 1 stop bit)
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);
}

int main(void)
{
    Serial_init( MYUBRR );
	Init_CtrlCntr1();
	Init_ADC();

	unsigned int mot_ctrl;
	char cmd = 'f';

    while(1)
    {
		if(cmd == 'f') mot_ctrl = ADC_read( ADC_INPUT );
		Update_CtrlCntr1( mot_ctrl );

		if(Serial_avail() == 1){
			cmd = Serial_readChr();
			switch(cmd){
				case 'f': break;
				case 'A': mot_ctrl = 1000; break;
				case 'B': mot_ctrl = 1111; break;
				case 'C': mot_ctrl = 1222; break;
				case 'D': mot_ctrl = 1333; break;
				case 'E': mot_ctrl = 1444; break;
				case 'F': mot_ctrl = 1555; break;
				case 'G': mot_ctrl = 1666; break;
				case 'H': mot_ctrl = 1777; break;
				case 'I': mot_ctrl = 1888; break;
				case 'J': mot_ctrl = 2000; break;
			}
		}
    }
}

