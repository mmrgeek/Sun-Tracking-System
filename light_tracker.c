#include <avr/io.h>//for the basic output and input 
#include <avr/interrupt.h> //for the interrupt
#include <stdlib.h> //for the unsigned 8-bit and 16-bit
int right = 0;
int left = 0;
int main(void){
	//make PINB0 and PINB1 as output
	DDRB |=1<<PINB0 | 1<<PINB1;
	//Enable a prescaler and internal or external clock
	ADCSRA |= 1<<ADPS2;
	//use the external analog reference on AVCC pin
	ADMUX |= 1<<REFS0;
	//Enable the interrupts function in ADC
	ADCSRA |= 1<<ADIE;
	//turn on the ADC
	ADCSRA |=1<<ADEN;
	//enable global interrupts
	sei();
	//start the conversion
	ADCSRA |= 1<<ADSC;
    while (1) {
		if(left > right){
			PORTB |= 1<<PINB0;
			PORTB &= ~(1<<PINB1);
		}else if(right > left){
			PORTB |= 1<<PINB1;
			PORTB &= ~(1<<PINB0);
		}else{
			PORTB &= ~(1<<PINB0);
			PORTB &= ~(1<<PINB1);
		}
    }
	return 0;
}
//the interrupt function
ISR(ADC_vect){
	uint8_t low = ADCL;
	uint16_t full = ADCH<<8 | low;
	switch(ADMUX){
		case 0x40:
			right = full;
			ADMUX |= 1<<MUX0;
			break;
		
		case 0x41:
			left = full;
			ADMUX &= ~(1<<MUX0);
			break;
	}
	//to start the conversion again
	ADCSRA |= 1<<ADSC;
}