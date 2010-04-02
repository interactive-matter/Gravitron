#include <avr/io.h>
#include <stdint.h>
#include "leds.h"

void leds_off(void) {
	//disable all leds	
	PORTC = 0;  
	PORTD = 0;	
}

void light_led(uint8_t number) {
	number = abs(number % 12);
	if (number<4) {
		//led is on PORTC
		PORTC = _BV(number);
		PORTD = 0;
	} else if (number<12) {
		//led is on PORTB
		PORTC = 0;
		PORTD = _BV(number-4);
	} else {
		//it is simply ignored
	}
}

void light_pattern(uint16_t pattern) {
	PORTC = (uint8_t) (pattern & 15); //lower 2 bit
	PORTD = (uint8_t) ((pattern >> 4)); //upper 8 bit
}
