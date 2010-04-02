#include <avr/io.h>          
#include <avr/power.h>
#include "adc.h"

void init_measurement(uint8_t mux, uint8_t voltageReference) {
	//switch on adc
	power_adc_enable();
	
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);    // Frequenzvorteiler 
	// setzen auf 8 (1) und ADC aktivieren (1)
	
	ADMUX = mux;                      // Kanal waehlen
	ADMUX |= voltageReference;
	
	ADCSRA|=(1<<ADEN);//enable ADC with dummy conversion
	//do one dummy measurement
	uint16_t result = read_channel(mux, voltageReference);
	result=0;
}

void finish_measurement() {
	ADCSRA &= (unsigned char)~((1<<ADEN));             // ADC deaktivieren (2)
	//switch off adc
	power_adc_disable();
}

int16_t read_channel(uint8_t mux, uint8_t voltageReference) {	
	uint16_t result;
	ADMUX = mux;                      // Kanal waehlen
	ADMUX |= voltageReference;
	
	ADCSRA |= (1<<ADSC);  // eine ADC-Wandlung 
	while ( ADCSRA & (1<<ADSC) ) {
		;     // auf Abschluss der Konvertierung warten 
	}
	result = ADCW;
	return result;
}
